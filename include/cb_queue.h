/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-08     tyx          first implementation
 */

#include "cb_list.h"

#ifndef __CB_QUEUE_H__
#define __CB_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cb_queue
{
    cb_list_t head;
}cb_queue_t;

typedef struct cb_queue_item
{
    cb_list_t node;
}cb_queue_item_t;

cb_queue_t *cb_queue_init(cb_queue_t *object);
void cb_queue_put(cb_queue_t *object, cb_queue_item_t *item);
cb_queue_item_t *cb_queue_get(cb_queue_t *object);
cb_queue_item_t *cb_queue_pick(cb_queue_t *object);
void cb_queue_insert(cb_queue_t *object, cb_queue_item_t *item, int index);

cb_inline cb_queue_item_t *cb_queue_item_init(cb_queue_item_t *item)
{
    cb_list_init(&item->node);
    return item;
}

#ifdef __cplusplus
}
#endif

#endif
