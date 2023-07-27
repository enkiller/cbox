/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#ifndef CB_HASHMAP_H_
#define CB_HASHMAP_H_

#include "cb_list.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cb_hashmap_item
{
    cb_hlist_t n;
    const void *key;
};
typedef struct cb_hashmap_item cb_hashmap_item_t;

#define CB_HASHMAP_ITER_INIT(_object) { 0, (_object)->table[0].hh.first, _object }

struct cb_hashmap_ops
{
    cb_uint32_t (*hash)(const void *k);
    long (*cmp)(const void *k1, const void *k2);
};

struct cb_hashmap_table
{
    cb_hhead_t hh;
};

struct cb_hashmap
{
    struct cb_hashmap_table *table;
    cb_size_t table_size;
    const struct cb_hashmap_ops *ops;
};
typedef struct cb_hashmap cb_hashmap_t;

struct cb_hashmap_iter
{
    cb_size_t index;
    cb_hlist_t *node;
    cb_hashmap_t *hashmap;
};
typedef struct cb_hashmap_iter cb_hashmap_iter_t;

// hashmap
cb_hashmap_t *cb_hashmap_init(cb_hashmap_t *object, struct cb_hashmap_table *table,
    cb_size_t table_size, const struct cb_hashmap_ops *ops);
cb_size_t cb_hashmap_size(cb_hashmap_t *object);
void cb_hashmap_put(cb_hashmap_t *object, cb_hashmap_item_t *item);
cb_hashmap_item_t *cb_hashmap_replace(cb_hashmap_t *object, cb_hashmap_item_t *elem);
cb_hashmap_item_t *cb_hashmap_get(cb_hashmap_t *object, const void *key);
cb_hashmap_item_t *cb_hashmap_remove(cb_hashmap_t *object, const void *key);
void cb_hashmap_item_remove(cb_hashmap_item_t *item);
void cb_hashmap_remove_all(cb_hashmap_t *object, void (*free_item)(cb_hashmap_t *, cb_hashmap_item_t *));
cb_hashmap_item_t *cb_hashmap_iterator(cb_hashmap_iter_t *ctx);
// item
cb_hashmap_item_t *cb_hashmap_item_init(cb_hashmap_item_t *item, const void *key);
cb_hashmap_iter_t *cb_hashmap_iter_init(cb_hashmap_t *object, cb_hashmap_iter_t *iter);

#ifdef __cplusplus
}
#endif

#endif
