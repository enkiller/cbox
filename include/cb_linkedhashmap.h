/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#include "cb_hashmap.h"

#ifndef __CB_LINKEDHASHMAP_H__
#define __CB_LINEKDHASHMAP_H__

#ifdef __cplusplus
extern "C" {
#endif

struct cb_linkedhashmap_item
{
    cb_hashmap_item_t parent;
    cb_list_t n;
};
typedef struct cb_linkedhashmap_item cb_linkedhashmap_item_t;

struct cb_linkedhashmap
{
    cb_hashmap_t parent;
    cb_list_t head;
};
typedef struct cb_linkedhashmap cb_linkedhashmap_t;

struct cb_linkedhashmap_iterator
{
    cb_list_t *node;
};
typedef struct cb_linkedhashmap_iterator cb_linkedhashmap_iter_t;

#define CB_LINKEDHASHMAP_ITER_INIT(_object) { (_object)->head.next }

// linkedhashmap
cb_linkedhashmap_t *cb_linkedhashmap_init(cb_linkedhashmap_t *object, struct cb_hashmap_table *table,
    cb_size_t table_size, const struct cb_hashmap_ops *ops);
void cb_linkedhashmap_remove_all(cb_linkedhashmap_t *object,
    void (*free_item)(cb_linkedhashmap_t *, cb_linkedhashmap_item_t *));
cb_linkedhashmap_item_t *cb_linkedhashmap_remove(cb_linkedhashmap_t *object, const void *key);
void cb_linkedhashmap_item_remove(cb_linkedhashmap_item_t *item);
void cb_linkedhashmap_push(cb_linkedhashmap_t *object, cb_linkedhashmap_item_t *item);
cb_linkedhashmap_item_t *cb_linkedhashmap_pop(cb_linkedhashmap_t *obj);
cb_linkedhashmap_item_t *cb_linkedhashmap_peak(cb_linkedhashmap_t *obj);
cb_linkedhashmap_item_t *cb_linkedhashmap_top(cb_linkedhashmap_t *obj, const void *key);
cb_size_t cb_linkedhashmap_size(cb_linkedhashmap_t *object);
cb_linkedhashmap_iter_t *cb_linkedhashmap_iterator_init(cb_linkedhashmap_t *object, cb_linkedhashmap_iter_t *ctx);
cb_linkedhashmap_item_t *cb_linkedhashmap_iterator(cb_linkedhashmap_t *object, cb_linkedhashmap_iter_t *ctx);
// item
cb_linkedhashmap_item_t *cb_linkedhashmap_item_init(cb_linkedhashmap_item_t *item, const void *key);
const void *cb_linkedhashmap_item_key(cb_linkedhashmap_item_t *item);

#ifdef __cplusplus
}
#endif

#endif
