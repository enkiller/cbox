/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-24     tyx          first implementation
 */

#include "cb_ringbuffer.h"
#include "cb_libc.h"

cb_inline unsigned int cb_ringbuffer_min(unsigned int a, unsigned int b)
{
    return a > b ? b : a;
}

cb_ringbuffer_t *cb_ringbuffer_init(cb_ringbuffer_t *rb, void *buff, unsigned int size)
{
    rb->buff = buff;
    rb->size = size;
    rb->in = 0;
    rb->out = 0;
    rb->out_idx = 0;
    rb->in_idx = 0;
    return rb;
}

unsigned int cb_ringbuffer_write(cb_ringbuffer_t *rb, const void *data, unsigned int length)
{
    unsigned int freelen = cb_ringbuffer_min(cb_ringbuffer_free(rb), length);
    unsigned int right_len = cb_ringbuffer_min(freelen, rb->size - rb->out_idx);

    // Backup actual write length
    length = freelen;
    // write
    cb_memcpy((char *)rb->buff + rb->out_idx, data, right_len);
    freelen -= right_len;
    cb_memcpy(rb->buff, (char *)data + right_len, freelen);
    rb->out_idx = (rb->out_idx + length) % rb->size;
    rb->out = rb->out + length;
    return length;
}

unsigned int cb_ringbuffer_overwrite(cb_ringbuffer_t *rb, const void *data, unsigned int length)
{
    unsigned int freelen = cb_ringbuffer_free(rb);
    if (freelen >= length)
    {
        length = cb_ringbuffer_write(rb, data, length);
    }
    else
    {
        cb_ringbuffer_discard(rb, length - freelen);
        if (rb->size >= length)
        {
            length = cb_ringbuffer_write(rb, data, length);
        }
        else
        {
            length = cb_ringbuffer_write(rb, (char *)data + (length - rb->size), rb->size);
        }
    }
    return length;
}

unsigned int cb_ringbuffer_read(cb_ringbuffer_t *rb, void *data, unsigned int length)
{
    length = cb_ringbuffer_peek(rb, data, length);
    cb_ringbuffer_discard(rb, length);
    return length;
}

unsigned int cb_ringbuffer_discard(cb_ringbuffer_t *rb, unsigned int length)
{
    unsigned int datalen = cb_ringbuffer_min(cb_ringbuffer_used(rb), length);
    rb->in_idx = (rb->in_idx + datalen) % rb->size;
    rb->in = rb->in + datalen;
    return datalen;
}

unsigned int cb_ringbuffer_peek(cb_ringbuffer_t* rb, void* data, unsigned int length)
{
    unsigned int datalen = cb_ringbuffer_min(cb_ringbuffer_used(rb), length);
    unsigned int right_len = cb_ringbuffer_min(datalen, rb->size - rb->in_idx);

    // Backup actual read length
    length = datalen;
    cb_memcpy(data, (char *)rb->buff + rb->in_idx, right_len);
    datalen -= right_len;
    cb_memcpy((char *)data + right_len, rb->buff, datalen);
    return length;
}

unsigned int cb_ringbuffer_used(cb_ringbuffer_t* rb)
{
    return rb->out - rb->in;
}

unsigned int cb_ringbuffer_isempty(cb_ringbuffer_t* rb)
{
    return rb->out == rb->in;
}

unsigned int cb_ringbuffer_isfull(cb_ringbuffer_t *rb)
{
    return rb->size == cb_ringbuffer_used(rb);
}

unsigned int cb_ringbuffer_free(cb_ringbuffer_t *rb)
{
    return rb->size - cb_ringbuffer_used(rb);
}

unsigned int cb_ringbuffer_total(cb_ringbuffer_t *rb)
{
    return rb->size;
}
