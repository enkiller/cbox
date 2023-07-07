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
    while (height < CB_SKIPLIST_MAX_LEVEL && (cb_random() % 4) == 0)
    {
        height += 1;
    }
    return height;
}

cb_skiplist_t* cb_skiplist_init(cb_skiplist_t* skiplist, int reverse,
    cb_bool_t(*cmp)(const cb_skiplist_node_t* v1, const cb_skiplist_node_t* v2))
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
    int lvl = CB_SKIPLIST_MAX_LEVEL - 1, i;
    cb_list_t* tab[CB_SKIPLIST_MAX_LEVEL];
    cb_skiplist_node_t*node;

    tab[lvl] = &skiplist->head.row[lvl];
    while (lvl >= 0)
    {
        for (; tab[lvl] != skiplist->head.row[lvl].prev; tab[lvl] = tab[lvl]->next)
        {
            node = cb_skiplist_of(tab[lvl]->next, lvl);
            if (skiplist->cmp(node, item) * skiplist->reverse > 0)
            {
                break;
            }
        }
        if (lvl > 0)
        {
            tab[lvl - 1] = tab[lvl] - 1;
        }
        lvl -= 1;
    }
    lvl = cb_skiplist_height();
    for (i = 0; i < lvl; i++)
    {
        cb_list_insert_after(tab[i], &item->row[i]);
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
