/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-05     tyx          first implementation
 */

#include "cb_skiplist.h"
#include "cb_libc.h"

unsigned int cb_skiplist_height(void)
{
    unsigned int height = 1;
    // The probability of 1/4 increases by one level
    while (height < CB_SKIPLIST_MAX_LEVEL && (cb_random() & 0xFFFF) < (0xFFFF >> 2))
    {
        height += 1;
    }
    return height;
}

cb_skiplist_t* cb_skiplist_init(cb_skiplist_t* skiplist, int reverse,
    int (*cmp)(const cb_skiplist_node_t* v1, const cb_skiplist_node_t* v2))
{
    cb_skiplist_node_init(&skiplist->head);
    skiplist->cmp = cmp;
    // Positive and reverse insertion marks
    skiplist->reverse = reverse ? -1 : 1;
    return skiplist;
}

cb_skiplist_node_t* cb_skiplist_node_init(cb_skiplist_node_t* item)
{
    unsigned int i = 0;

    for (i = 0; i < CB_ARRAY_SIZE(item->row); i++)
    {
        cb_list_init(&item->row[i]);
    }
    return item;
}

cb_skiplist_node_t *cb_skiplist_insert(cb_skiplist_t* skiplist, cb_skiplist_node_t* item)
{
    cb_skiplist_iter iter;
    cb_skiplist_node_t *node;
    unsigned int height, i;

    cb_skiplist_iter_begin(&iter, skiplist);
    // Find a suitable insertion location from an ordered linked list
    while ((node = cb_skiplist_iter_next(&iter)) != cb_null)
    {
        if (skiplist->cmp(node, item) * skiplist->reverse > 0)
        {
            // Jump to the next level to continue searching
            cb_skiplist_iter_skip(&iter);
        }
    }
    // Insert
    height = cb_skiplist_height();
    for (i = 0; i < height; i++)
    {
        cb_list_insert_after(iter.list[i], &item->row[i]);
    }
    return item;
}

cb_skiplist_node_t *cb_skiplist_remove(cb_skiplist_node_t* item)
{
    unsigned int i = 0;

    for (i = 0; i < CB_ARRAY_SIZE(item->row); i++)
    {
        cb_list_remove(&item->row[i]);
    }
    return item;
}

cb_skiplist_node_t* cb_skiplist_first(cb_skiplist_t* skiplist)
{
    return (cb_skiplist_node_t*)cb_container_of(skiplist->head.row[0].next, cb_list_t, next);
}

int cb_skiplist_isempty(cb_skiplist_t* skiplist)
{
    return cb_list_isempty(&skiplist->head.row[0]);
}

cb_skiplist_node_t *cb_skiplist_iter_next(cb_skiplist_iter *iter)
{
    int lvl = iter->level;
    cb_skiplist_node_t *node = cb_null;

    while (node == cb_null && lvl >= 0)
    {
        cb_list_t* last = iter->skiplist->head.row[lvl].prev;
        cb_list_t ** list = iter->list;
        list[lvl] = iter->next;
        if (list[lvl] != last)
        {
            node = cb_skiplist_of(list[lvl]->next, lvl);
            iter->next = list[lvl]->next;
        }
        else
        {
            lvl = cb_skiplist_iter_skip(iter);
        }
    }
    return node;
}
