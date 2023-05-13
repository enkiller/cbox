/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-13     tyx          first implementation
 */

#include "cb_libc.h"

long cb_strcmp(const char* s1, const char* s2)
{
    long r = 0;

    while ((r = *s1 - *s2++) == 0 && *s1++);
    return r;
}

