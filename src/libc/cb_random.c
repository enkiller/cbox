/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-13     tyx          first implementation
 */

#include "cb_libc.h"

static unsigned int _seed = 0;

void cb_srandom(unsigned int seed)
{
    _seed = seed;
}

int cb_random(void)
{
    _seed = (1103515245U * _seed + 12345) & 0x7fffffff;
    return (int)_seed;
}
