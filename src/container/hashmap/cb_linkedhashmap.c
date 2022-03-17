/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#include "cb_linkedhashmap.h"

cb_linkedhashmap_item_t *cb_linkedhashmap_item_init(cb_linkedhashmap_item_t *item, const void *key)
{
    if (item)
    {
        cb_hashmap_item_init(&item->parent, key);
        cb_list_init(&item->n);
    }
    return item;
}

const void *cb_linkedhashmap_item_key(cb_linkedhashmap_item_t *item)
{
    const void *key = cb_null;

    if (item)
    {
        key = item->parent.key;
    }
    return key;
}

cb_linkedhashmap_t *cb_linkedhashmap_init(cb_linkedhashmap_t *object, struct cb_hashmap_table *table,
    cb_size_t table_size, const struct cb_hashmap_ops *ops)
{
    if (object)
    {
        cb_hashmap_init(&object->parent, table, table_size, ops);
        cb_list_init(&object->head);
    }
    return object;
}

cb_linkedhashmap_item_t *cb_linkedhashmap_remove(cb_linkedhashmap_t *object, const void *key)
{
    cb_linkedhashmap_item_t *item;

    item = (cb_linkedhashmap_item_t *)cb_hashmap_remove(&object->parent, key);
    if (item)
    {
        cb_list_remove(&item->n);
    }
    return item;
}

void cb_linkedhashmap_item_remove(cb_linkedhashmap_item_t *item)
{
    cb_hashmap_item_remove(&item->parent);
    cb_list_remove(&item->n);
}

void cb_linkedhashmap_remove_all(cb_linkedhashmap_t *object, void (*free_item)(cb_linkedhashmap_t *, cb_linkedhashmap_item_t *))
{
    cb_linkedhashmap_item_t *item;

    while ((item = cb_linkedhashmap_pop(object)) != cb_null)
    {
        if (free_item)
        {
            free_item(object, item);
        }
    }
}

void cb_linkedhashmap_push(cb_linkedhashmap_t *object, cb_linkedhashmap_item_t *item)
{
    if (cb_list_isempty(&item->n))
    {
        cb_hashmap_put(&object->parent, &item->parent);
    }
    else
    {
        cb_list_remove(&item->n);
    }
    cb_list_insert_before(&object->head, &item->n);
}

cb_linkedhashmap_item_t *cb_linkedhashmap_first(cb_linkedhashmap_t *object, int removed)
{
    cb_linkedhashmap_item_t *item;

    if (cb_list_isempty(&object->head))
    {
        return cb_null;
    }
    // get first
    item = cb_list_first_entry(&object->head, cb_linkedhashmap_item_t, n);
    // is remove
    if (removed)
    {
        cb_list_remove(&item->n);
        cb_hashmap_item_remove(&item->parent);
    }
    return item;
}

cb_linkedhashmap_item_t *cb_linkedhashmap_pop(cb_linkedhashmap_t *object)
{
    return cb_linkedhashmap_first(object, cb_true);
}

cb_linkedhashmap_item_t *cb_linkedhashmap_peak(cb_linkedhashmap_t *object)
{
    return cb_linkedhashmap_first(object, cb_false);
}

cb_linkedhashmap_item_t *cb_linkedhashmap_top(cb_linkedhashmap_t *object, const void *key)
{
    cb_linkedhashmap_item_t *item;

    item = (cb_linkedhashmap_item_t *)cb_hashmap_get(&object->parent, key);
    if (item)
    {
        cb_list_remove(&item->n);
        cb_list_insert_after(&object->head, &item->n);
    }
    return item;
}

cb_linkedhashmap_item_t *cb_linkedhashmap_get(cb_linkedhashmap_t *object, const void *key)
{
    return (cb_linkedhashmap_item_t *)cb_hashmap_get(&object->parent, key);
}

cb_size_t cb_linkedhashmap_size(cb_linkedhashmap_t *object)
{
    return cb_list_len(&object->head);
}

cb_linkedhashmap_iter_t *cb_linkedhashmap_iterator_init(cb_linkedhashmap_t *object, cb_linkedhashmap_iter_t *ctx)
{
    if (ctx)
    {
        ctx->node = cb_list_first(&object->head);
    }
    return ctx;
}

cb_linkedhashmap_item_t *cb_linkedhashmap_iterator(cb_linkedhashmap_t *object, cb_linkedhashmap_iter_t *ctx)
{
    for (cb_list_t *pos = ctx->node, *nn = pos->next; pos != &object->head; pos = nn, nn = pos->next)
    {
        ctx->node = nn;
        return cb_list_entry(pos, cb_linkedhashmap_item_t, n);
    }
    return cb_null;
}
