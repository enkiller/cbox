/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-09-13     tyx          first implementation
 */

#include "cb_mempool.h"

cb_inline unsigned long _blk_align(unsigned long blk_size)
{
    // Cannot be smaller than the size of an item
    if (blk_size < sizeof(struct cb_mempool_item))
    {
        blk_size = sizeof(struct cb_mempool_item);
    }
    // machine word length
    blk_size = CB_ALIGN(blk_size, sizeof(void *));
    return blk_size;
}

cb_inline cb_uint8_t *_addr_align(cb_uint8_t *addr)
{
    cb_uint8_t *s = cb_null;
    cb_uint8_t pos = (addr - s) & 0xf;
    cb_uint8_t align_pos = (CB_ALIGN(pos, sizeof(void *)) & 0xff) - pos;
    return addr + align_pos;
}

cb_inline struct cb_mempool_item *_next_item(void *ptr, unsigned long blk_size)
{
    cb_uint8_t *addr = ptr;
    addr += blk_size;
    return (struct cb_mempool_item *)addr;
}

cb_memp_t *cb_mempool_init(cb_memp_t *memp, void *addr, unsigned long blk_size, unsigned long total_size)
{
    struct cb_mempool_item *item, *next;
    void *ptr =  _addr_align(addr);

    total_size -= ((cb_uint8_t *)ptr - (cb_uint8_t *)addr) & 0xff;
    blk_size = _blk_align(blk_size);
    if (total_size >= blk_size)
    {
        // Initialize the first node
        item = ptr;
        next = _next_item(item, blk_size);
        memp->next_free = item;
        total_size -= blk_size;
        // Initialize the remaining nodes
        while (total_size >= blk_size)
        {
            item->next = next;
            item = next;
            next = _next_item(next, blk_size);
            total_size -= blk_size;
        }
        item->next = cb_null;
    }
    else
    {
        memp->next_free = cb_null;
    }
    return memp;
}

void *cb_mempool_alloc(cb_memp_t *memp)
{
    void *ptr = cb_null;

    if (memp->next_free != cb_null)
    {
        ptr = memp->next_free;
        memp->next_free = memp->next_free->next;
    }
    return ptr;
}

void cb_mempool_free(cb_memp_t *memp, void *ptr)
{
    struct cb_mempool_item *item = ptr;

    if (item != cb_null)
    {
        item->next = memp->next_free;
        memp->next_free = item;
    }
}
