/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-14     tyx          first implementation
 */

#include "cb_hashfun.h"
#include <gtest/gtest.h>

TEST(testCase, cb_hashfun_test01)
{
    cb_uint32_t h0, h1, h2, h3, h4, h5;

    h0 = cb_hash_string("");
    h1 = cb_hash_string("1");
    h2 = cb_hash_string("2");
    h3 = cb_hash_string("3");
    h4 = cb_hash_string("4");
    h5 = cb_hash_string("5");
    EXPECT_NE(h0 != h1 || h0 != h2 || h0 != h3 || h0 != h4 || h0 != h5, 0);
}

TEST(testCase, cb_hashfun_test02)
{
    char s1[] = "EQ";
    const char *s2 = "EQ";

    EXPECT_NE(cb_hash_string_cmp("", ""), 0);
    EXPECT_EQ(cb_hash_string_cmp("", s1), 0);
    EXPECT_NE(cb_hash_string_cmp(s1, s1), 0);
    EXPECT_NE(cb_hash_string_cmp(s1, s2), 0);
}

TEST(testCase, cb_hashfun_test03)
{
    cb_uint32_t n0 = 0, n1 = 1, n2 = 2, n3 = 3, n4 = 4, n5 = 5;
    cb_uint32_t h0, h1, h2, h3, h4, h5;

    h0 = cb_hash_uint32(&n0);
    h1 = cb_hash_uint32(&n1);
    h2 = cb_hash_uint32(&n2);
    h3 = cb_hash_uint32(&n3);
    h4 = cb_hash_uint32(&n4);
    h5 = cb_hash_uint32(&n5);
    EXPECT_NE(h0 != h1 || h0 != h2 || h0 != h3 || h0 != h4 || h0 != h5, 0);
}

TEST(testCase, cb_hashfun_test04)
{
    cb_uint32_t n0 = 0, n1 = 1, n00 = 0;

    EXPECT_NE(cb_hash_uint32_cmp(&n0, &n0), 0);
    EXPECT_EQ(cb_hash_string_cmp(&n0, &n1), 0);
    EXPECT_NE(cb_hash_uint32_cmp(&n0, &n00), 0);
}
