/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-05     tyx          first implementation
 */

#ifndef __CB_SKIPLIST_H__
#define __CB_SKIPLIST_H__

#include "cb_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CB_SKIPLIST_MAX_LEVEL   (3U)

typedef struct cb_skiplist_node
{
    cb_list_t row[CB_SKIPLIST_MAX_LEVEL];
}cb_skiplist_node_t;

struct cb_skiplist;
typedef struct cb_skiplist cb_skiplist_t;
struct cb_skiplist
{
    cb_skiplist_node_t head;
    int reverse;
    int (*cmp)(const cb_skiplist_node_t* v1, const cb_skiplist_node_t* v2);
};

typedef struct cb_skiplist_iterator
{
    cb_skiplist_t *skiplist;
    int level;
    cb_list_t* list[CB_SKIPLIST_MAX_LEVEL];
    cb_list_t* next;
}cb_skiplist_iter;

cb_inline int cb_skiplist_iter_skip(cb_skiplist_iter *iter)
{
    if (iter->level > 0)
    {
        iter->next = iter->list[iter->level] - 1;
    }
    if (iter->level >= 0)
    {
        iter->level -= 1;
    }
    return iter->level;
}

cb_inline cb_skiplist_iter *cb_skiplist_iter_begin(cb_skiplist_iter *iter, cb_skiplist_t* skiplist)
{
    iter->skiplist = skiplist;
    iter->level = CB_SKIPLIST_MAX_LEVEL - 1;
    //iter->list[iter->level] = &skiplist->head.row[iter->level];
    iter->next = &skiplist->head.row[iter->level];
    return iter;
}

cb_skiplist_node_t *cb_skiplist_iter_next(cb_skiplist_iter *iter);

cb_skiplist_t *cb_skiplist_init(cb_skiplist_t *skiplist, int reverse,
    int (*cmp)(const cb_skiplist_node_t *v1, const cb_skiplist_node_t *v2));
cb_skiplist_node_t *cb_skiplist_node_init(cb_skiplist_node_t *item);
cb_skiplist_node_t *cb_skiplist_insert(cb_skiplist_t* skiplist, cb_skiplist_node_t* item);
cb_skiplist_node_t *cb_skiplist_remove(cb_skiplist_node_t* item);
cb_skiplist_node_t *cb_skiplist_first(cb_skiplist_t *skiplist);
int cb_skiplist_isempty(cb_skiplist_t *skiplist);

cb_inline cb_skiplist_node_t* cb_skiplist_of(cb_list_t* next, unsigned int lvl)
{
    cb_list_t* l = cb_container_of(next, cb_list_t, next);
    return (cb_skiplist_node_t*)cb_container_of(l, cb_skiplist_node_t, row[lvl]);
}

cb_inline cb_skiplist_node_t* cb_skiplist_next(cb_skiplist_node_t* item)
{
    return (cb_skiplist_node_t*)cb_skiplist_of(item->row[0].next, 0);
}

#define cb_skiplist_for_each(pos, skiplist) \
    for (cb_skiplist_node_t* pos = cb_skiplist_next(&(skiplist)->head), \
        *nn = cb_skiplist_next(pos); pos != &((skiplist)->head); pos = nn, nn = cb_skiplist_next(nn))

#ifdef __cplusplus
}
#endif

#endif
