/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#ifndef CB_BITOPT_H_
#define CB_BITOPT_H_

#include "cb_def.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

cb_inline void *cb_setbit(void *addr, unsigned int n)
{
    cb_uint8_t *ptr = (cb_uint8_t *)addr;
    ptr[n / 8] |= 0x1 << n % 8;
    return addr;
}

cb_inline void *cb_clearbit(void *addr, unsigned int n)
{
    cb_uint8_t *ptr = (cb_uint8_t *)addr;
    ptr[n / 8] &= ~(0x1 << n % 8);
    return addr;
}

cb_inline int cb_testbit(void *addr, unsigned int n)
{
    cb_uint8_t *ptr = (cb_uint8_t *)addr;
    return !!(ptr[n / 8] & (0x1 << n % 8));
}

/**
 * @brief Find First bit Set in word(From LSB to MSB).
 * BIT counts from 0 and ranges from [0-31].
 * When the input value is 0x1, the return value is 0
 * When the input value is 0x2, the return value is 1
 * When the input value is 0x80000000, the return value is 31
 * When the input value is 0, the return value is also 0
 */
cb_inline int cb_ffs32(cb_uint32_t x)
{
    int r = 0;

    if (x)
    {
        if (!(x & 0xFFFFU))
        {
            x >>= 16;
            r += 16;
        }
        if (!(x & 0xFFU))
        {
            x >>= 8;
            r += 8;
        }
        if (!(x & 0xFU))
        {
            x >>= 4;
            r += 4;
        }
        if (!(x & 0x3U))
        {
            x >>= 2;
            r += 2;
        }
        if (!(x & 0x1U))
        {
            x >>= 1;
            r += 1;
        }
    }
    return r;
}

/**
 * @brief Find First bit Set in word(From LSB to MSB).
 * BIT counts from 0 and ranges from [0-63].
 * When the input value is 0x1, the return value is 0
 * When the input value is 0x2, the return value is 1
 * When the input value is 0x8000000000000000, the return value is 63
 * When the input value is 0, the return value is also 0
 */
cb_inline int cb_ffs64(cb_uint64_t x)
{
    int r = 0;

    if (x)
    {
        if (!(x & 0xFFFFFFFFULL))
        {
            x >>= 32;
            r += 32;
        }
        if (!(x & 0xFFFFULL))
        {
            x >>= 16;
            r += 16;
        }
        if (!(x & 0xFFULL))
        {
            x >>= 8;
            r += 8;
        }
        if (!(x & 0xFULL))
        {
            x >>= 4;
            r += 4;
        }
        if (!(x & 0x3ULL))
        {
            x >>= 2;
            r += 2;
        }
        if (!(x & 0x1ULL))
        {
            x >>= 1;
            r += 1;
        }
    }
    return r;
}

/**
 * @brief Find First bit Set in word(From LSB to MSB).
 * If the value of *addr is 0, the return value is meaningless
 */
cb_inline int cb_ffs(unsigned long x)
{
    return cb_ffs64(x);
}

/**
 * @brief Find First zero in word(From LSB to MSB).
 * BIT counts from 0 and ranges from [0-31].
 * When the input value is 0x0, the return value is 0
 * When the input value is 0x1, the return value is 1
 * When the input value is 0x2, the return value is 0
 * When the input value is 0x000000FF, the return value is 8
 * When the input value is 0xFFFFFFFF, the return value is also 0
 */
cb_inline int cb_ffz32(cb_uint32_t x)
{
    return cb_ffs32(~x);
}

/**
 * @brief Find First zero in word(From LSB to MSB).
 * BIT counts from 0 and ranges from [0-63].
 * When the input value is 0x0, the return value is 0
 * When the input value is 0x1, the return value is 1
 * When the input value is 0x2, the return value is 0
 * When the input value is 0x00000000FFFFFFFF, the return value is 32
 * When the input value is 0xFFFFFFFFFFFFFFFF, the return value is also 0
 */
cb_inline int cb_ffz64(cb_uint64_t x)
{
    return cb_ffs64(~x);
}

/**
 * @brief Find First zero in word(From LSB to MSB).
 * If *addr is the maximum machine word length, the return value is meaningless
 */
cb_inline int cb_ffz(unsigned long x)
{
    return cb_ffz64((~0x0ULL << CB_BITS_LONG) | x);
}

/**
 * @brief Count Leading Zeroes.
 * The value ranges from 0 to 32
 * When the input value is 0x0, the return value is 32
 * When the input value is 0x1, the return value is 31
 * When the input value is 0x2, the return value is 30
 * When the input value is 0xF0, the return value is 24
 * When the input value is 0xFFFFFFFF, the return value is 0
 */
cb_inline int cb_clz32(cb_uint32_t x)
{
    int r = 0;

    if (x)
    {
        if (!(x & 0xFFFF0000U))
        {
            x <<= 16;
            r += 16;
        }
        if (!(x & 0xFF000000U))
        {
            x <<= 8;
            r += 8;
        }
        if (!(x & 0xF0000000U))
        {
            x <<= 4;
            r += 4;
        }
        if (!(x & 0xC0000000U))
        {
            x <<= 2;
            r += 2;
        }
        if (!(x & 0x80000000U))
        {
            x <<= 1;
            r += 1;
        }
    }
    else
    {
        r = 32;
    }
    return r;
}

/**
 * @brief Count Leading Zeroes.
 * The value ranges from 0 to 64
 * When the input value is 0x0, the return value is 64
 * When the input value is 0x1, the return value is 63
 * When the input value is 0x2, the return value is 62
 * When the input value is 0xF0, the return value is 56
 * When the input value is 0x00000000FFFFFFFF, the return value is 32
 * When the input value is 0xFFFFFFFFFFFFFFFF, the return value is 0
 */
cb_inline int cb_clz64(cb_uint64_t x)
{
    int r = 0;

    if (x)
    {
        if (!(x & 0xFFFFFFFF00000000ULL))
        {
            x <<= 32;
            r += 32;
        }
        if (!(x & 0xFFFF000000000000ULL))
        {
            x <<= 16;
            r += 16;
        }
        if (!(x & 0xFF00000000000000ULL))
        {
            x <<= 8;
            r += 8;
        }
        if (!(x & 0xF000000000000000ULL))
        {
            x <<= 4;
            r += 4;
        }
        if (!(x & 0xC000000000000000ULL))
        {
            x <<= 2;
            r += 2;
        }
        if (!(x & 0x8000000000000000ULL))
        {
            x <<= 1;
            r += 1;
        }
    }
    else
    {
        r = 64;
    }
    return r;
}

/**
 * @brief Count Leading Zeroes.
 */
cb_inline int cb_clz(unsigned long x)
{
    cb_uint64_t t = x;

    t = t << (64 - CB_BITS_LONG);
    t |= ~0x0ULL >> (64 - CB_BITS_LONG);
    return cb_clz64(t);
}

/**
 * @brief Find Last bit set
 * The value ranges from 0 to 32
 * When the input value is 0x0, the return value is 0
 * When the input value is 0x1, the return value is 1
 * When the input value is 0xF0, the return value is 8
 * When the input value is 0x80000000, the return value is 32
 * When the input value is 0x80000001, the return value is 32
 */
cb_inline int cb_fls32(cb_uint32_t x)
{
    return 32 - cb_clz32(x);
}

/**
 * @brief Find Last bit set
 * The value ranges from 0 to 64
 * When the input value is 0x0, the return value is 0
 * When the input value is 0x1, the return value is 1
 * When the input value is 0xF0, the return value is 8
 * When the input value is 0x80000000, the return value is 32
 * When the input value is 0x8000000000000000, the return value is 64
 * When the input value is 0x8000000000000001, the return value is 64
 */
cb_inline int cb_fls64(cb_uint64_t x)
{
    return 64 - cb_clz64(x);
}

/**
 * @brief Find Last bit set
 */
cb_inline int cb_fls(unsigned long x)
{
    return CB_BITS_LONG - cb_clz(x);
}

/**
 * @brief Count Trailing Zeroes
 * The value ranges from 0 to 32
 * When the input value is 0x0, the return value is 32
 * When the input value is 0x2, the return value is 1
 * When the input value is 0x80000000, the return value is 31
 * When the input value is 0xFFFFFFFF, the return value is 0
 */
cb_inline int cb_ctz32(cb_uint32_t x)
{
    int r;

    if (x != 0)
    {
        r = cb_ffs32(x);
    }
    else
    {
        r = 32;
    }
    return r;
}

/**
 * @brief Count Trailing Zeroes
 * The value ranges from 0 to 64
 * When the input value is 0x0, the return value is 64
 * When the input value is 0x2, the return value is 1
 * When the input value is 0x3, the return value is 0
 * When the input value is 0x80000000, the return value is 31
 * When the input value is 0xFFFFFFFFFFFFFFFF, the return value is 0
 */
cb_inline int cb_ctz64(cb_uint64_t x)
{
    int r;

    if (x != 0)
    {
        r = cb_ffs64(x);
    }
    else
    {
        r = 64;
    }
    return r;
}

/**
 * @brief Count Trailing Zeroes
 */
cb_inline int cb_ctz(unsigned long x)
{
    int r;

    if (x != 0)
    {
        r = cb_ffs(x);
    }
    else
    {
        r = CB_BITS_LONG;
    }
    return r;
}

/**
 * @brief Finds the location of the first set from a section of memory
 */
cb_inline unsigned long cb_find_next_bit(const unsigned long* addr,
    unsigned long size, unsigned long offset)
{
    unsigned long res = size;
    unsigned long unalign_size, value = 0;
    unsigned long pos = offset;

    if (offset < size)
    {
        pos = offset;
        addr += offset / CB_BITS_LONG;
        unalign_size = offset % CB_BITS_LONG;
        // Head alignment processing
        value = (*addr++) >> unalign_size;
        offset += CB_BITS_LONG - unalign_size;
        if (offset > size)
        {
            offset = size;
        }
        // find
        while (value == 0 && offset < size)
        {
            value = *addr++;
            pos = offset;
            offset += CB_BITS_LONG;
        }
        // Tail alignment processing
        if (offset > size)
        {
            value = value & (~0x0UL >> (offset - size));
        }
        if (value != 0)
        {
            res = pos + cb_ffs(value);
        }
    }
    return res;
}

cb_inline unsigned long cb_find_first_bit(const unsigned long* addr,
    unsigned long size)
{
    return cb_find_next_bit(addr, size, 0);
}

#define cb_for_each_set_bit(bit, addr, size) \
    for ((bit) = cb_find_first_bit(addr, size); (bit) < (size); \
    (bit) = cb_find_next_bit(addr, size, (bit) + 1))

#ifdef __cplusplus
}
#endif

#endif /* CB_BITOPT_H_ */
