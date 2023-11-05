/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-04     tyx          first implementation
 */

#include "cb_radixtree.h"
#include "cb_bitopt.h"
#include "cb_libc.h"

static cb_rtslots_t *cb_radix_tree_alloc_node(cb_rtroot_t* root, const void *def_value)
{
    cb_rtslots_t *new_slots = cb_null;
    unsigned char i;

    new_slots = root->alloc_node(root, root->slot_cnt);
    if (new_slots != cb_null)
    {
        for (i = 0; i < root->slot_cnt; i++)
        {
            new_slots[i].const_value = def_value;
        }
    }
    return new_slots;
}

static void cb_radix_tree_free_node(cb_rtroot_t* root, cb_rtslots_t *slot)
{
    root->free_node(root, slot, root->slot_cnt);
}

static unsigned long long cb_radix_tree_maxkey_get(unsigned char height, unsigned char slots)
{
    unsigned long long key = 0;
    unsigned char mask = 0xFFU;
    unsigned char maskbits;

    // Gets the number of consecutive 0 bits starting from the low position
    maskbits = cb_ctz(slots) & 0xFFU;
    // Calculate level mask
    mask = ~(mask << maskbits);
    key = mask;
    while (height--)
    {
        key = (key << maskbits) + mask;
    }
    return key;
}

static unsigned char cb_radix_tree_max_height(const void* key, unsigned char bits)
{
    const unsigned char *zero = cb_null;
    const unsigned char *ptr = key;
    unsigned char last_bit = cb_fls64(ptr - zero) & 0xFFU;
    return (last_bit - 1) / bits;
}

static void cb_radix_tree_delete(cb_rtroot_t* root, cb_rtslots_t* slots, unsigned char height)
{
    unsigned char i;

    if (slots != cb_null)
    {
        if (height > 0)
        {
            for (i = 0; i < root->slot_cnt; i++)
            {
                cb_radix_tree_delete(root, slots[i].next_level, height - 1);
            }
        }
        cb_radix_tree_free_node(root, slots);
    }
}

static int cb_radix_tree_lessthen(unsigned long long max_key, const void *key)
{
    const unsigned char *ptr = key;
    const unsigned char *zero = cb_null;
    unsigned long long uintkey = ptr - zero;
    return max_key < uintkey;
}

cb_rtroot_t *cb_radix_tree_init(cb_rtroot_t *root, unsigned char slots)
{
    root->slot_cnt = (0x1U << (cb_fls(slots) - 1)) & 0xFFU;
    root->mask_bits = cb_ctz(slots) & 0xFFU;
    root->max_key = cb_radix_tree_maxkey_get(0, root->slot_cnt);
    root->root_slots = cb_null;
    root->height = 0;
    return root;
}

void cb_radix_tree_deinit(cb_rtroot_t* root)
{
    cb_radix_tree_delete(root, root->root_slots, root->height);
    root->root_slots = cb_null;
    root->height = 0;
    root->max_key = cb_radix_tree_maxkey_get(0, root->slot_cnt);
}

static int cb_radix_tree_height_expand(cb_rtroot_t* root, const void* key)
{
    int res = -1;
    unsigned char new_height, increment, i;
    cb_rtslots_t* slots = cb_null;
    cb_rtslots_t* head = cb_null;

    if (cb_radix_tree_lessthen(root->max_key, key))
    {
        new_height = cb_radix_tree_max_height(key, root->mask_bits);
        if (new_height > root->height)
        {
            increment = new_height - root->height;
            head = cb_radix_tree_alloc_node(root, cb_null);
            slots = head;
            for (i = 1; slots != cb_null && i < increment; i++)
            {
                slots[0].next_level = cb_radix_tree_alloc_node(root, cb_null);
                slots = slots[0].next_level;
            }
            if (slots != cb_null)
            {
                slots[0].next_level = root->root_slots;
                root->root_slots = head;
                root->height = new_height;
                root->max_key = cb_radix_tree_maxkey_get(new_height, root->slot_cnt);
                res = 0;
            }
            else
            {
                cb_radix_tree_delete(root, head, increment);
            }
        }
    }
    else
    {
        res = 0;
    }
    return res;
}

static cb_rtslots_t* cb_radix_tree_slots_expand(cb_rtroot_t* root, const void* key)
{
    cb_rtslots_t* slots = cb_null;
    unsigned char i, s, h;
    unsigned char mask = 0xFFU;
    unsigned char alloc_flag = 0;
    const unsigned char *ptr = key;
    const unsigned char *zero = cb_null;
    unsigned long long uintkey = ptr - zero;

    if (cb_radix_tree_lessthen(root->max_key, key))
    {
        cb_radix_tree_height_expand(root, key);
    }
    if (!cb_radix_tree_lessthen(root->max_key, key))
    {
        mask = ~(mask << root->mask_bits);
        i = root->height;
        h = 0;
        slots = root->root_slots;
        s = (uintkey >> (i * root->mask_bits)) & mask;
        while (slots != cb_null && h < root->height)
        {
            if (slots[s].next_level == cb_null)
            {
                slots[s].next_level = cb_radix_tree_alloc_node(root, cb_null);
                alloc_flag = 1;
            }
            slots = slots[s].next_level;
            i -= 1;
            s = (uintkey >> (i * root->mask_bits)) & mask;
            h += 1;
        }
        if (slots != cb_null)
        {
            if (alloc_flag != 0)
            {
                for (i = 0; i < root->slot_cnt; i++)
                {
                    slots[i].const_value = root->def_value;
                }
            }
            slots = &slots[s];
        }
    }
    return slots;
}

int cb_radix_tree_insert(cb_rtroot_t* root, const void* key, const void* value)
{
    int res = -1;
    cb_rtslots_t *slots;

    if (root->root_slots == cb_null)
    {
        root->root_slots = cb_radix_tree_alloc_node(root, root->def_value);
    }
    slots = cb_radix_tree_slots_expand(root, key);
    if (slots != cb_null)
    {
        slots->const_value = value;
        res = 0;
    }
    return res;
}

cb_rtslots_t* cb_radix_tree_slot_get(cb_rtroot_t* root, const void* key)
{
    cb_rtslots_t* slots = cb_null;
    unsigned char i, s, h;
    unsigned char mask = 0xFFU;
    const unsigned char *ptr = key;
    const unsigned char *zero = cb_null;
    unsigned long long ikey = ptr - zero;

    if (!cb_radix_tree_lessthen(root->max_key, key))
    {
        mask = ~(mask << root->mask_bits);
        i = root->height;
        h = 0;
        s = (ikey >> (root->height * root->mask_bits)) & mask;
        slots = root->root_slots;
        while (slots != cb_null && h < root->height)
        {
            slots = slots[s].next_level;
            i -= 1;
            s = (ikey >> (i * root->mask_bits)) & mask;
            h += 1;
        }
        if (slots != cb_null)
        {
            slots = &slots[s];
        }
    }
    return slots;
}

void* cb_radix_tree_lookup(cb_rtroot_t* root, const void* key)
{
    void *value = root->def_value;
    cb_rtslots_t* slots = cb_radix_tree_slot_get(root, key);

    if (slots != cb_null)
    {
        value = slots->value;
    }
    return value;
}

void* cb_radix_tree_remove(cb_rtroot_t* root, const void* key)
{
    void *value = root->def_value;
    cb_rtslots_t* slots = cb_radix_tree_slot_get(root, key);

    if (slots != cb_null)
    {
        value = slots->value;
        slots->value = root->def_value;
    }
    return value;
}

static cb_rtslots_t *_radix_tree_shrink(cb_rtroot_t* root, cb_rtslots_t* slots, unsigned char height)
{
    unsigned char i, used;

    if (slots != cb_null)
    {
        used = 0;
        if (height > 0)
        {
            for (i = 0; i < root->slot_cnt; i++)
            {
                slots[i].next_level = _radix_tree_shrink(root, slots[i].next_level, height - 1);
                used |= slots[i].next_level != cb_null;
            }
        }
        else
        {
            for (i = 0; used == 0 && i < root->slot_cnt; i++)
            {
                used = slots[i].const_value != root->def_value;
            }
        }
        if (used == 0)
        {
            cb_radix_tree_free_node(root, slots);
            slots = cb_null;
        }
    }
    return slots;
}

void cb_radix_tree_shrink(cb_rtroot_t* root)
{
    unsigned char height = root->height;
    unsigned char used = 0;
    unsigned char i;
    cb_rtslots_t *slots;

    slots = _radix_tree_shrink(root, root->root_slots, root->height);
    root->root_slots = slots;

    while (used == 0 && height > 0 && slots != cb_null)
    {
        for (i = root->slot_cnt - 1; used == 0 && i > 0; i--)
        {
            used = slots[i].next_level != cb_null;
        }
        if (used == 0)
        {
            root->root_slots = slots[0].next_level;
            cb_radix_tree_free_node(root, slots);
            root->height -= 1;
            slots = root->root_slots;
        }
    }
}
