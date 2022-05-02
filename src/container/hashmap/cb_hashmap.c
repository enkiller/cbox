/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#include "cb_hashmap.h"

cb_inline cb_hhead_t *_tab_head_get(cb_hashmap_t *object, const void *key)
{
    int tabid = object->ops->hash(key) % object->table_size;
    return &object->table[tabid].hh;
}

cb_hashmap_item_t *cb_hashmap_pick(cb_hashmap_t *object,
    const void *key, cb_bool_t removed, cb_hhead_t **hh_ptr)
{
    cb_hhead_t *hh;
    cb_hashmap_item_t *item;

    /* get hash list head */
    hh = _tab_head_get(object, key);
    if (hh_ptr != cb_null)
    {
        *hh_ptr = hh;
    }
    /* traversing the list and get item */
    cb_hlist_for_each(i, hh)
    {
        item = cb_hlist_entry(i, cb_hashmap_item_t, n);
        /* Whether the key values are equal */
        if (object->ops->cmp(key, item->key))
        {
            if (removed)
            {
                cb_hlist_remove(&item->n);
            }
            return item;
        }
    }
    /* Key value not found */
    return cb_null;
}

cb_hashmap_t *cb_hashmap_init(cb_hashmap_t *object, struct cb_hashmap_table *table,
    cb_size_t table_size, const struct cb_hashmap_ops *ops)
{
    if (object != cb_null)
    {
        // init hashmap object
        object->table = table;
        object->table_size = table_size;
        object->ops = ops;
        // init hash head
        for (cb_size_t i = 0; i < table_size; i++)
        {
            cb_hlist_head_init(&table[i].hh);
        }
    }
    return object;
}

cb_size_t cb_hashmap_size(cb_hashmap_t *object)
{
    cb_size_t size = 0;

    for (cb_size_t i = 0; i < object->table_size; i++)
    {
        size += cb_hlist_len(&object->table[i].hh);
    }
    return size;
}

void cb_hashmap_put(cb_hashmap_t *object, cb_hashmap_item_t *item)
{
    cb_hhead_t *hh;

    hh = _tab_head_get(object, item->key);
    if (!cb_hlist_unhashed(&item->n))
    {
        cb_hashmap_item_remove(item);
    }
    cb_hlist_insert_head(hh, &item->n);
}

cb_hashmap_item_t *cb_hashmap_replace(cb_hashmap_t *object, cb_hashmap_item_t *item)
{
    cb_hashmap_item_t *current_item;
    cb_hhead_t *hh;

    current_item = cb_hashmap_pick(object, item->key, cb_true, &hh);
    cb_hlist_insert_head(hh, &item->n);
    return current_item;
}

cb_hashmap_item_t *cb_hashmap_get(cb_hashmap_t *object, const void *key)
{
    return cb_hashmap_pick(object, key, cb_false, cb_null);
}

cb_hashmap_item_t *cb_hashmap_remove(cb_hashmap_t *object, const void *key)
{
    return cb_hashmap_pick(object, key, cb_true, cb_null);
}

void cb_hashmap_item_remove(cb_hashmap_item_t *item)
{
    cb_hlist_remove(&item->n);
}

void cb_hashmap_remove_all(cb_hashmap_t *object, void (*free_item)(cb_hashmap_t *, cb_hashmap_item_t *))
{
    cb_hashmap_iter_t iter = CB_HASHMAP_ITER_INIT(object);
    cb_hashmap_item_t *item;

    while ((item = cb_hashmap_iterator(&iter)) != cb_null)
    {
        cb_hashmap_item_remove(item);
        if (free_item != cb_null)
        {
            free_item(object, item);
        }
    }
}

cb_hashmap_item_t *cb_hashmap_iterator(cb_hashmap_iter_t *ctx)
{
    cb_hashmap_t *object = ctx->hashmap;

    if (ctx->index >= object->table_size)
        return cb_null;

    for (cb_size_t i = ctx->index; i < object->table_size; i++)
    {
        for (cb_hlist_t *pos = ctx->node, *nn = pos ? pos->next : cb_null; pos != cb_null; \
            pos = nn, nn = pos ? pos->next : cb_null)
        {
            // save next node
            if (nn == cb_null)
            {
                if (++i < object->table_size)
                {
                    ctx->node = cb_hlist_first(&object->table[i].hh);
                }
                ctx->index = i;
            }
            else
            {
                ctx->node = nn;
            }
            return cb_hlist_entry(pos, cb_hashmap_item_t, n);
        }
    }
    return cb_null;
}

cb_hashmap_item_t *cb_hashmap_item_init(cb_hashmap_item_t *item, const void *key)
{
    if (item)
    {
        cb_hlist_init(&item->n);
        item->key = key;
    }
    return item;
}

cb_hashmap_iter_t *cb_hashmap_iter_init(cb_hashmap_t *object, cb_hashmap_iter_t *iter)
{
    if (iter && object && object->table_size)
    {
        iter->index = 0;
        iter->node = cb_hlist_first(&object->table[0].hh);
        iter->hashmap = object;
    }
    return iter;
}

//void hashmap_trace(hashmap_t map)
//{
//    rt_uint32_t i, len;
//    rt_slist_t *l, *n;
//    struct hashmap_item *item;
//
//    rt_kprintf("hashmap size:%d\n", hashmap_size(map));
//    for (i = 0; i < map->tab_size; i++)
//    {
//        l = &map->tab[i].list;
//        len = rt_slist_len(l);
//        if (len == 0)
//            continue;
//        rt_kprintf("tab[%d] count : %d\n", i, len);
//        rt_slist_for_each(n, l)
//        {
//            item = rt_slist_entry(n, struct hashmap_item, node);
//            rt_kprintf("    hash:%08x k:%p addr:%p\n",
//                map->ops->hash(item->key),
//                item->key,
//                item);
//        }
//    }
//}