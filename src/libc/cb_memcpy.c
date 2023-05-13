/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-13     tyx          first implementation
 */

#include "cb_libc.h"

void* cb_memcpy(void* dst, const void* src, unsigned int count)
{
    char* dst_ptr = (char *)dst;
    char* src_ptr = (char *)src;

    if (count > (sizeof(long) << 2))
    {
        if (((dst_ptr - ((char *)0)) & (sizeof(void *) - 1)) == 0)
        {
            if (((src_ptr - ((char*)0)) & (sizeof(void *) - 1)) == 0)
            {
                long *aligned_dst = (long*)dst_ptr;
                long *aligned_src = (long*)src_ptr;
                /* Copy 4X long words at a time if possible. */
                while (count >= (sizeof(long) << 2))
                {
                    *aligned_dst++ = *aligned_src++;
                    *aligned_dst++ = *aligned_src++;
                    *aligned_dst++ = *aligned_src++;
                    *aligned_dst++ = *aligned_src++;
                    count -= (sizeof(long) << 2);
                }
                /* Copy one long word at a time if possible. */
                while (count >= sizeof(long))
                {
                    *aligned_dst++ = *aligned_src++;
                    count -= sizeof(long);
                }
                /* Pick up any residual with a byte copier. */
                dst_ptr = (char*)aligned_dst;
                src_ptr = (char*)aligned_src;
            }
        }
    }
    while (count >= (sizeof(char) << 3))
    {
        *dst_ptr++ = *src_ptr++;
        *dst_ptr++ = *src_ptr++;
        *dst_ptr++ = *src_ptr++;
        *dst_ptr++ = *src_ptr++;
        *dst_ptr++ = *src_ptr++;
        *dst_ptr++ = *src_ptr++;
        *dst_ptr++ = *src_ptr++;
        *dst_ptr++ = *src_ptr++;
        count -= (sizeof(char) << 3);
    }
    while (count--)
    {
        *dst_ptr++ = *src_ptr++;
    }
    return dst;
}
