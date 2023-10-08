/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-05     tyx          first implementation
 */

#include <stdint.h>
#include <gtest/gtest.h>
#include "cb_bitopt.h"

TEST(testCase, cb_bitopt_setbit_test01)
{
    uint32_t v;
    uint32_t one = 0x1U;

    for (unsigned int i = 0; i < 32U; i++)
    {
        v = 0U;
        cb_setbit(&v, i);
        EXPECT_EQ(v, one << i);
    }
}

TEST(testCase, cb_bitopt_setbit_test02)
{
    uint32_t v1 = 0U, v2 = 0U;
    uint32_t one = 0x1U;

    for (unsigned int i = 0; i < 32U; i++)
    {
        cb_setbit(&v1, i);
        v2 |= one << i;
        EXPECT_EQ(v1, v2);
    }
}

TEST(testCase, cb_bitopt_clearbit_test01)
{
    uint32_t v1, v2;
    uint32_t zero = 0x0U, one = 0x1U;

    for (unsigned int i = 0; i < 32U; i++)
    {
        v1 = ~zero;
        cb_clearbit(&v1, i);
        v2 = (~zero) & (~(one << i));
        EXPECT_EQ(v1, v2);
    }
}

TEST(testCase, cb_bitopt_clearbit_test02)
{
    uint32_t v1;
    uint32_t v2;
    uint32_t zero = 0x0U, one = 0x1U;

    v1 = ~zero;
    v2 = ~zero;
    for (unsigned int i = 0; i < 32U; i++)
    {
        cb_clearbit(&v1, i);
        v2 = v2 & (~(one << i));
        EXPECT_EQ(v1, v2);
    }
}

TEST(testCase, cb_bitopt_testbit_test01)
{
    uint32_t v;
    uint32_t zero = 0x0U, one = 0x1U;

    for (unsigned int i = 0; i < 32U; i++)
    {
        v = one << i;
        EXPECT_TRUE(cb_testbit(&v, i));
        v = (~zero) & (~(one << i));
        EXPECT_FALSE(cb_testbit(&v, i));
    }
}

TEST(testCase, cb_bitopt_ffs32_test01)
{
    uint32_t v;
    uint32_t zero = 0x0U, one = 0x1U;

    EXPECT_EQ(cb_ffs32(0x1), 0);
    EXPECT_EQ(cb_ffs32(0x2), 1);
    EXPECT_EQ(cb_ffs32(0x80000000), 31);
    EXPECT_EQ(cb_ffs32(0), 0);

    for (unsigned int i = 0; i < 32U; i++)
    {
        v = one << i;
        EXPECT_EQ(cb_ffs32(v), i);
        v = (~zero) << i;
        EXPECT_EQ(cb_ffs32(v), i);
    }
}

TEST(testCase, cb_bitopt_ffs64_test01)
{
    uint64_t v;

    EXPECT_EQ(cb_ffs64(0x1ULL), 0);
    EXPECT_EQ(cb_ffs64(0x2ULL), 1);
    EXPECT_EQ(cb_ffs64(0x80000000ULL), 31);
    EXPECT_EQ(cb_ffs64(0x8000000000000000ULL), 63);
    EXPECT_EQ(cb_ffs64(0), 0);

    for (unsigned int i = 0; i < 64U; i++)
    {
        v = 0x1ULL << i;
        EXPECT_EQ(cb_ffs64(v), i);
        v = 0xFFFFFFFFFFFFFFFFULL << i;
        EXPECT_EQ(cb_ffs64(v), i);
    }
}

TEST(testCase, cb_bitopt_ffs_test01)
{
    unsigned long v;
    unsigned long one = 1UL;
    unsigned long zero = 0UL;

    EXPECT_EQ(cb_ffs(0), 0);
    for (unsigned long i = 0; i < sizeof(unsigned long) * 8UL; i++)
    {
        v = one << i;
        EXPECT_EQ(cb_ffs(v), i);
        v = (~zero) << i;
        EXPECT_EQ(cb_ffs(v), i);
    }
}

TEST(testCase, cb_bitopt_ffz32_test01)
{
    uint32_t v;
    uint32_t zero = 0x0U, one = 0x1U;

    EXPECT_EQ(cb_ffz32(zero), 0);
    EXPECT_EQ(cb_ffz32(one), 1);
    EXPECT_EQ(cb_ffz32(2), 0);
    EXPECT_EQ(cb_ffz32(0xFF), 8);
    EXPECT_EQ(cb_ffz32(~zero), 0);

    for (unsigned int i = 0, v = 0; i < 31U; i++)
    {
        EXPECT_EQ(cb_ffz32(v), i);
        v = v | (one << i);
    }
    for (unsigned int i = 0; i < 32U; i++)
    {
        v = (~zero) & (~(one << i));
        EXPECT_EQ(cb_ffz32(v), i);
    }
}

TEST(testCase, cb_bitopt_ffz64_test01)
{
    uint64_t v = 0ULL;
    unsigned long i;

    EXPECT_EQ(cb_ffz64(0x0ULL), 0);
    EXPECT_EQ(cb_ffz64(0x1ULL), 1);
    EXPECT_EQ(cb_ffz64(0x2ULL), 0);
    EXPECT_EQ(cb_ffz64(0x00000000FFFFFFFFULL), 32);
    EXPECT_EQ(cb_ffz64(0xFFFFFFFFFFFFFFFFULL), 0);

    for (i = 0, v = 0; i < 63UL; i++)
    {
        EXPECT_EQ(cb_ffz64(v), i);
        v = v | (0x1ULL << i);
    }
    for (i = 0; i < 64UL; i++)
    {
        v = 0xFFFFFFFFFFFFFFFFULL & (~(0x1ULL << i));
        EXPECT_EQ(cb_ffz64(v), i);
    }
}

TEST(testCase, cb_bitopt_ffz_test01)
{
    unsigned long v;
    unsigned long one = 1UL;
    unsigned long zero = 0UL;
    unsigned long i;

    EXPECT_EQ(cb_ffz(~zero), 0);
    for (i = 0, v = 0; i < sizeof(unsigned long) * 8UL; i++)
    {
        EXPECT_EQ(cb_ffz(v), i);
        v = v | (0x1UL << i);
    }
    for (i = 0; i < sizeof(unsigned long) * 8UL; i++)
    {
        v = (~zero) & (~(one << i));
        EXPECT_EQ(cb_ffz(v), i);
    }
}

TEST(testCase, cb_bitopt_clz32_test01)
{
    uint32_t v;
    uint32_t zero = 0x0U, one = 0x1U;
    unsigned long i;

    EXPECT_EQ(cb_clz32(0), 32);
    EXPECT_EQ(cb_clz32(1), 31);
    EXPECT_EQ(cb_clz32(2), 30);
    EXPECT_EQ(cb_clz32(0xF0), 24);
    EXPECT_EQ(cb_clz32(~zero), 0);

    for (i = 0, v = ~zero; i <= 32UL; i++)
    {
        EXPECT_EQ(cb_clz32(v), i);
        v = v >> 1U;
    }
    for (i = 0, v = one << 31U; i <= 32UL; i++)
    {
        EXPECT_EQ(cb_clz32(v), i);
        v = v >> 1U;
    }
}

TEST(testCase, cb_bitopt_clz64_test01)
{
    uint64_t v;
    unsigned long i;

    EXPECT_EQ(cb_clz64(0), 64);
    EXPECT_EQ(cb_clz64(1), 63);
    EXPECT_EQ(cb_clz64(2), 62);
    EXPECT_EQ(cb_clz64(0xF0), 56);
    EXPECT_EQ(cb_clz64(0x00000000FFFFFFFFULL), 32);
    EXPECT_EQ(cb_clz64(0xFFFFFFFFFFFFFFFFULL), 0);

    for (i = 0, v = 0xFFFFFFFFFFFFFFFFULL; i <= 64UL; i++)
    {
        EXPECT_EQ(cb_clz64(v), i);
        v = v >> 1U;
    }
    for (i = 0, v = 0x8000000000000000ULL; i <= 64UL; i++)
    {
        EXPECT_EQ(cb_clz64(v), i);
        v = v >> 1U;
    }
}

TEST(testCase, cb_bitopt_clz_test01)
{
    unsigned long v;
    unsigned long one = 1UL;
    unsigned long zero = 0UL;
    unsigned long i;

    for (i = 0, v = ~zero; i <= sizeof(unsigned long) * 8UL; i++)
    {
        EXPECT_EQ(cb_clz(v), i);
        v = v >> 1UL;
    }
    for (i = 0, v = one << (sizeof(unsigned long) * 8UL - 1UL); i <= sizeof(unsigned long) * 8UL; i++)
    {
        EXPECT_EQ(cb_clz(v), i);
        v = v >> 1UL;
    }
}

TEST(testCase, cb_bitopt_fls32_test01)
{
    uint32_t v;
    uint32_t one = 0x1U;
    unsigned long i;

    EXPECT_EQ(cb_fls32(0), 0);
    EXPECT_EQ(cb_fls32(1), 1);
    EXPECT_EQ(cb_fls32(0xF0), 8);
    EXPECT_EQ(cb_fls32(0x80000000), 32);
    EXPECT_EQ(cb_fls32(0x80000001), 32);

    for (i = 0, v = 0U; i <= 32UL; i++)
    {
        EXPECT_EQ(cb_fls32(v), i);
        v = one << i;
    }
    for (i = 0, v = 0; i <= 32UL; i++)
    {
        EXPECT_EQ(cb_fls32(v), i);
        v |= one << i;
    }
}

TEST(testCase, cb_bitopt_fls64_test01)
{
    uint64_t v;
    unsigned long i;

    EXPECT_EQ(cb_fls64(0), 0);
    EXPECT_EQ(cb_fls64(1), 1);
    EXPECT_EQ(cb_fls64(0xF0), 8);
    EXPECT_EQ(cb_fls64(0x80000000ULL), 32);
    EXPECT_EQ(cb_fls64(0x8000000000000000ULL), 64);
    EXPECT_EQ(cb_fls64(0x8000000000000001ULL), 64);

    for (i = 0, v = 0; i <= 64UL; i++)
    {
        EXPECT_EQ(cb_fls64(v), i);
        v = 0x1ULL << i;
    }
    for (i = 0, v = 0; i <= 64UL; i++)
    {
        EXPECT_EQ(cb_fls64(v), i);
        v |= 0x1ULL << i;
    }
}

TEST(testCase, cb_bitopt_fls_test01)
{
    unsigned long v;
    unsigned long one = 1;
    unsigned long zero = 0;
    unsigned long i;

    for (i = 0, v = 0; i <= sizeof(unsigned long) * 8UL; i++)
    {
        EXPECT_EQ(cb_fls(v), i);
        v = 0x1UL << i;
    }
    for (i = 0, v = 0; i <= sizeof(unsigned long) * 8UL; i++)
    {
        EXPECT_EQ(cb_fls(v), i);
        v |= 0x1UL << i;
    }
}

TEST(testCase, cb_bitopt_ctz32_test01)
{
    uint32_t v;
    unsigned long i;

    EXPECT_EQ(cb_ctz32(0), 32);
    EXPECT_EQ(cb_ctz32(2), 1);
    EXPECT_EQ(cb_ctz32(0x80000000), 31);
    EXPECT_EQ(cb_ctz32(0xFFFFFFFF), 0);

    v = 0U;
    v = ~v;
    for (i = 0; i <= 32UL; i++)
    {
        EXPECT_EQ(cb_ctz32(v), i);
        v = v << 0x1U;
    }
    for (i = 0, v = 1U; i <= 32UL; i++)
    {
        EXPECT_EQ(cb_ctz32(v), i);
        v = v << 0x1U;
    }
}

TEST(testCase, cb_bitopt_ctz64_test01)
{
    uint64_t v;
    unsigned long i;

    EXPECT_EQ(cb_ctz64(0), 64);
    EXPECT_EQ(cb_ctz64(2), 1);
    EXPECT_EQ(cb_ctz64(3), 0);
    EXPECT_EQ(cb_ctz64(0x80000000ULL), 31);
    EXPECT_EQ(cb_ctz64(0xFFFFFFFFFFFFFFFFULL), 0);

    v = 0ULL;
    v = ~v;
    for (i = 0; i <= 64UL; i++)
    {
        EXPECT_EQ(cb_ctz64(v), i);
        v = v << 0x1ULL;
    }
    for (i = 0, v = 1ULL; i <= 64UL; i++)
    {
        EXPECT_EQ(cb_ctz64(v), i);
        v = v << 0x1ULL;
    }
}

TEST(testCase, cb_bitopt_ctz_test01)
{
    unsigned long v;
    unsigned long i;

    v = 0UL;
    v = ~v;
    for (i = 0; i <= sizeof(unsigned long) * 8UL; i++)
    {
        EXPECT_EQ(cb_ctz(v), i);
        v = v << 0x1UL;
    }
    for (i = 0, v = 1UL; i <= sizeof(unsigned long) * 8UL; i++)
    {
        EXPECT_EQ(cb_ctz(v), i);
        v = v << 0x1UL;
    }
}

TEST(testCase, cb_bitopt_find_next_bit_test01)
{
    unsigned long bits[4];
    unsigned long i, len;

    for (len = 0; len <= sizeof(bits) * 8; len++)
    {
        memset(bits, 0x00, sizeof(bits));
        for (i = 0; i < len * 2; i++)
        {
            EXPECT_EQ(cb_find_next_bit(bits, len, i), len);
        }

        memset(bits, 0xFF, sizeof(bits));
        for (i = 0; i <= len; i++)
        {
            EXPECT_EQ(cb_find_next_bit(bits, len, i), i);
        }

        for (i = 0; i < len; i++)
        {
            memset(bits, 0x00, sizeof(bits));
            cb_setbit(bits, i);
            EXPECT_EQ(cb_find_next_bit(bits, len, 0), i);
        }
    }
}

TEST(testCase, cb_for_each_set_bit_test01)
{
    unsigned long bits[4];
    unsigned long i, len;
    unsigned long cnt, bit;

    for (len = 0; len <= sizeof(bits) * 8; len++)
    {
        memset(bits, 0x00, sizeof(bits));
        bit = ~0x0UL;
        for (i = 0; i < len * 2; i++)
        {
            cnt = 0;
            cb_for_each_set_bit(bit, bits, len)
            {
                cnt += 1;
            }
            EXPECT_EQ(cnt, 0);
        }

        memset(bits, 0xFF, sizeof(bits));
        for (i = 0; i < len; i++)
        {
            cnt = 0UL;
            cb_for_each_set_bit(bit, bits, len)
            {
                EXPECT_EQ(bit, cnt);
                cnt += 1UL;
            }
            EXPECT_EQ(cnt, len);
        }

        for (i = 0; i < len; i++)
        {
            cnt = 0;
            memset(bits, 0x00, sizeof(bits));
            cb_setbit(bits, i);
            cb_for_each_set_bit(bit, bits, len)
            {
                EXPECT_EQ(bit, i);
                EXPECT_EQ(cnt, 0);
                cnt += 1;
            }
            EXPECT_EQ(cnt, 1);
        }
    }
}
