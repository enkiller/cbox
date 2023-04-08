/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-08     tyx          first implementation
 */

#include "cb_queue.h"

void cb_queue_item_remove(cb_queue_item_t *item)
{
    if (item != cb_null)
    {
        cb_list_remove(&item->node);
    }
}

cb_queue_t *cb_queue_init(cb_queue_t *object)
{
    cb_list_init(&object->head);
    return object;
}

void cb_queue_put(cb_queue_t *object, cb_queue_item_t *item)
{
    cb_queue_item_remove(item);
    cb_list_insert_before(&object->head, &item->node);
}

cb_queue_item_t *cb_queue_get(cb_queue_t *object)
{
    cb_queue_item_t *item = cb_queue_pick(object);
    cb_queue_item_remove(item);
    return item;
}

cb_queue_item_t *cb_queue_pick(cb_queue_t *object)
{
    cb_queue_item_t *item = cb_null;

    if (!cb_list_isempty(&object->head))
    {
        item = cb_container_of(cb_list_first(&object->head), cb_queue_item_t, node);
    }
    return item;
}

cb_list_t *cb_queue_search(cb_queue_t *object, int index)
{
    cb_list_t *n;

    if (index >= 0)
    {
        n = object->head.next;
        while (index != 0 && n != &object->head)
        {
            n = n->next;
            index -= 1;
        }
    }
    else
    {
        index += 1;
        n = object->head.prev;
        while (index != 0 && n != &object->head)
        {
            n = n->prev;
            index += 1;
        }
    }
    return n;
}

void cb_queue_insert(cb_queue_t *object, cb_queue_item_t *item, int index)
{
    cb_list_t *n;

    cb_queue_item_remove(item);
    n = cb_queue_search(object, index);
    if (index >= 0)
    {
        cb_list_insert_before(n, &item->node);
    }
    else
    {
        cb_list_insert_after(n, &item->node);
    }
}
