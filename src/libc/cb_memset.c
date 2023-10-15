/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-14     tyx          first implementation
 */

#include "cb_libc.h"
#include "cb_def.h"

void* cb_memset(void* dst, int c, unsigned long count)
{
    unsigned char* dst_ptr = dst;
    unsigned char* zero = cb_null;
    unsigned char v = c & 0xFFU;
    unsigned long lv = v, i;

    for (i = 1; i < sizeof(unsigned long); i = i << 1)
    {
        lv = lv + (lv << (8 * i));
    }

    if (count > (sizeof(unsigned long) << 2))
    {
        if (((dst_ptr - zero) & (sizeof(void *) - 1)) == 0)
        {
            unsigned long *aligned_dst = (unsigned long*)dst_ptr;

            /* Copy 4X long words at a time if possible. */
            while (count >= (sizeof(unsigned long) << 2))
            {
                *aligned_dst++ = lv;
                *aligned_dst++ = lv;
                *aligned_dst++ = lv;
                *aligned_dst++ = lv;
                count -= (sizeof(unsigned long) << 2);
            }
            /* Copy one long word at a time if possible. */
            while (count >= sizeof(unsigned long))
            {
                *aligned_dst++ = lv;
                count -= sizeof(unsigned long);
            }
            /* Pick up any residual with a byte copier. */
            dst_ptr = (unsigned char*)aligned_dst;
        }
    }
    while (count >= (sizeof(unsigned char) << 3))
    {
        *dst_ptr++ = v;
        *dst_ptr++ = v;
        *dst_ptr++ = v;
        *dst_ptr++ = v;
        *dst_ptr++ = v;
        *dst_ptr++ = v;
        *dst_ptr++ = v;
        *dst_ptr++ = v;
        count -= (sizeof(char) << 3);
    }
    while (count--)
    {
        *dst_ptr++ = v;
    }
    return dst;
}
