/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-09-13     tyx          first implementation
 */

#ifndef CB_MEMPOOL_H_
#define CB_MEMPOOL_H_

#include "cb_def.h"

#ifdef __cplusplus
extern "C" {
#endif

struct cb_mempool_item
{
    struct cb_mempool_item *next;
};

typedef struct cb_mempool
{
    struct cb_mempool_item *next_free;
}cb_memp_t;

cb_memp_t *cb_mempool_init(cb_memp_t *memp, void *addr, unsigned long blk_size, unsigned long total_size);
void *cb_mempool_alloc(cb_memp_t *memp);
void cb_mempool_free(cb_memp_t *memp, void *ptr);

#ifdef __cplusplus
}
#endif

#endif
