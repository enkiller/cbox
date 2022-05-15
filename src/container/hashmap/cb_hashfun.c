/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#include "cb_hashfun.h"
#include "cb_string.h"

cb_uint32_t cb_hash_string(const void *k)
{
    register cb_uint32_t hash = 0;
    const char *key = k;

    while (*key) hash = hash * 131 + *key++;
    return hash;
}

cb_bool_t cb_hash_string_cmp(const void *s1, const void *s2)
{
    cb_bool_t r;

    if (s1 == s2)
    {
        r = cb_true;
    }
    else
    {
        r = cb_strcmp(s1, s2) == 0;
    }
    return r;
}

cb_uint32_t cb_hash_uint32(const void *k)
{
    register cb_uint32_t key = *(cb_uint32_t *)k;
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

cb_bool_t cb_hash_uint32_cmp(const void *s1, const void *s2)
{
    cb_bool_t r;

    if (s1 == s2)
    {
        r = cb_true;
    }
    else
    {
        r = (*(cb_uint32_t *)s1 == *(cb_uint32_t *)s2);
    }
    return r;
}
