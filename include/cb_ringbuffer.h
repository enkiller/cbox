/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-24     tyx          first implementation
 */

#include "cb_def.h"

#ifndef __CB_RINGBUFFER_H__
#define __CB_RINGBUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cb_ringbuffer
{
    void *buff;
    unsigned int size;
    unsigned int in;
    unsigned int out;
    unsigned int in_idx;
    unsigned int out_idx;
}cb_ringbuffer_t;

cb_ringbuffer_t *cb_ringbuffer_init(cb_ringbuffer_t *rb, void *buff, unsigned int size);
unsigned int cb_ringbuffer_write(cb_ringbuffer_t *rb, const void *data, unsigned int length);
unsigned int cb_ringbuffer_overwrite(cb_ringbuffer_t *rb, const void *data, unsigned int length);
unsigned int cb_ringbuffer_read(cb_ringbuffer_t *rb, void *data, unsigned int length);
unsigned int cb_ringbuffer_discard(cb_ringbuffer_t *rb, unsigned int length);
unsigned int cb_ringbuffer_peek(cb_ringbuffer_t *rb, void *data, unsigned int length);
unsigned int cb_ringbuffer_isempty(cb_ringbuffer_t *rb);
unsigned int cb_ringbuffer_isfull(cb_ringbuffer_t *rb);
unsigned int cb_ringbuffer_free(cb_ringbuffer_t *rb);
unsigned int cb_ringbuffer_used(cb_ringbuffer_t *rb);
unsigned int cb_ringbuffer_total(cb_ringbuffer_t *rb);

#ifdef __cplusplus
}
#endif

#endif
