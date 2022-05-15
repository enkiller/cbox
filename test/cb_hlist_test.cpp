/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-14     tyx          first implementation
 */

#include "cb_list.h"
#include <gtest/gtest.h>

TEST(testCase, cb_hlist_test01)
{
    cb_hlist_t l = CB_HASH_NODE_OBJECT_INIT(l);

    EXPECT_EQ(l.next, nullptr);
    EXPECT_EQ(l.pprev, nullptr);
}

TEST(testCase, cb_hlist_test02)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);

    EXPECT_EQ(h.first, nullptr);
}

TEST(testCase, cb_hlist_test03)
{
    cb_hlist_t l;

    cb_hlist_init(&l);
    EXPECT_EQ(l.next, nullptr);
    EXPECT_EQ(l.pprev, nullptr);
}

TEST(testCase, cb_hlist_test04)
{
    cb_hhead_t h;

    cb_hlist_head_init(&h);
    EXPECT_EQ(h.first, nullptr);
}

TEST(testCase, cb_hlist_test05)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_insert_head(&h, &n1);
    EXPECT_EQ(h.first, &n1);
    EXPECT_EQ(n1.next, nullptr);
    EXPECT_EQ(n1.pprev, &h.first);
    cb_hlist_insert_head(&h, &n0);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, &n1);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n1.next, nullptr);
    EXPECT_EQ(n1.pprev, &n0.next);
}

TEST(testCase, cb_hlist_test06)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1, n2, n3;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_init(&n2);
    cb_hlist_init(&n3);

    cb_hlist_insert_head(&h, &n0);
    cb_hlist_insert_after(&n0, &n2);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, &n2);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n2.next, nullptr);
    EXPECT_EQ(n2.pprev, &n0.next);
    cb_hlist_insert_after(&n0, &n1);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, &n1);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n1.pprev, &n0.next);
    EXPECT_EQ(n2.next, nullptr);
    EXPECT_EQ(n2.pprev, &n1.next);
    cb_hlist_insert_after(&n2, &n3);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, &n1);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n1.pprev, &n0.next);
    EXPECT_EQ(n2.next, &n3);
    EXPECT_EQ(n2.pprev, &n1.next);
    EXPECT_EQ(n3.next, nullptr);
    EXPECT_EQ(n3.pprev, &n2.next);
}

TEST(testCase, cb_hlist_test07)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1, n2, n3;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_init(&n2);
    cb_hlist_init(&n3);

    cb_hlist_insert_head(&h, &n3);
    cb_hlist_insert_before(&n3, &n1);
    EXPECT_EQ(h.first, &n1);
    EXPECT_EQ(n1.next, &n3);
    EXPECT_EQ(n1.pprev, &h.first);
    EXPECT_EQ(n3.next, nullptr);
    EXPECT_EQ(n3.pprev, &n1.next);
    cb_hlist_insert_before(&n1, &n0);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, &n1);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n1.next, &n3);
    EXPECT_EQ(n1.pprev, &n0.next);
    EXPECT_EQ(n3.next, nullptr);
    EXPECT_EQ(n3.pprev, &n1.next);
    cb_hlist_insert_before(&n3, &n2);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, &n1);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n1.pprev, &n0.next);
    EXPECT_EQ(n2.next, &n3);
    EXPECT_EQ(n2.pprev, &n1.next);
    EXPECT_EQ(n3.next, nullptr);
    EXPECT_EQ(n3.pprev, &n2.next);
}

TEST(testCase, cb_hlist_test08)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1, n2;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_init(&n2);

    cb_hlist_insert_head(&h, &n2);
    cb_hlist_insert_head(&h, &n1);
    cb_hlist_insert_head(&h, &n0);
    cb_hlist_remove(&n1);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, &n2);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n1.next, nullptr);
    EXPECT_EQ(n1.pprev, nullptr);
    EXPECT_EQ(n2.next, nullptr);
    EXPECT_EQ(n2.pprev, &n0.next);
    cb_hlist_remove(&n2);
    EXPECT_EQ(h.first, &n0);
    EXPECT_EQ(n0.next, nullptr);
    EXPECT_EQ(n0.pprev, &h.first);
    EXPECT_EQ(n1.next, nullptr);
    EXPECT_EQ(n1.pprev, nullptr);
    EXPECT_EQ(n2.next, nullptr);
    EXPECT_EQ(n2.pprev, nullptr);
    cb_hlist_remove(&n0);
    EXPECT_EQ(h.first, nullptr);
    EXPECT_EQ(n0.next, nullptr);
    EXPECT_EQ(n0.pprev, nullptr);
    EXPECT_EQ(n1.next, nullptr);
    EXPECT_EQ(n1.pprev, nullptr);
    EXPECT_EQ(n2.next, nullptr);
    EXPECT_EQ(n2.pprev, nullptr);
}

TEST(testCase, cb_hlist_test09)
{
    cb_hhead_t h0 = CB_HASH_HEAD_OBJECT_INIT(h0);
    cb_hhead_t h1 = CB_HASH_HEAD_OBJECT_INIT(h1);
    cb_hlist_t n00, n01, n02;
    cb_hlist_t n10, n11, n12;

    cb_hlist_init(&n00);
    cb_hlist_init(&n01);
    cb_hlist_init(&n02);
    cb_hlist_init(&n10);
    cb_hlist_init(&n11);
    cb_hlist_init(&n12);

    cb_hlist_move(&h0, &h1);
    EXPECT_EQ(h0.first, nullptr);
    EXPECT_EQ(h1.first, nullptr);
    cb_hlist_insert_head(&h1, &n12);
    cb_hlist_insert_head(&h1, &n11);
    cb_hlist_insert_head(&h1, &n10);
    cb_hlist_move(&h0, &h1);
    EXPECT_EQ(h0.first, &n10);
    EXPECT_EQ(h1.first, nullptr);
    EXPECT_EQ(n10.pprev, &h0.first);
    cb_hlist_move(&h1, &h0);
    EXPECT_EQ(h0.first, nullptr);
    EXPECT_EQ(h1.first, &n10);
    EXPECT_EQ(n10.pprev, &h1.first);
}

TEST(testCase, cb_hlist_test10)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    EXPECT_NE(cb_hlist_isempty(&h), 0);
    cb_hlist_insert_head(&h, &n1);
    EXPECT_EQ(cb_hlist_isempty(&h), 0);
    cb_hlist_insert_head(&h, &n0);
    EXPECT_EQ(cb_hlist_isempty(&h), 0);
}

TEST(testCase, cb_hlist_test11)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);

    EXPECT_NE(cb_hlist_unhashed(&n1), 0);
    cb_hlist_insert_head(&h, &n1);
    EXPECT_EQ(cb_hlist_unhashed(&n1), 0);
    cb_hlist_insert_head(&h, &n1);
    EXPECT_EQ(cb_hlist_unhashed(&n1), 0);
}

TEST(testCase, cb_hlist_test12)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1, n2;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_init(&n2);
    EXPECT_EQ(cb_hlist_len(&h), 0);
    cb_hlist_insert_head(&h, &n2);
    EXPECT_EQ(cb_hlist_len(&h), 1);
    cb_hlist_insert_head(&h, &n1);
    EXPECT_EQ(cb_hlist_len(&h), 2);
    cb_hlist_insert_head(&h, &n0);
    EXPECT_EQ(cb_hlist_len(&h), 3);
}

TEST(testCase, cb_hlist_test13)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1, n2;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_init(&n2);
    EXPECT_EQ(cb_hlist_first(&h), nullptr);
    cb_hlist_insert_head(&h, &n2);
    EXPECT_EQ(cb_hlist_first(&h), &n2);
    cb_hlist_insert_head(&h, &n1);
    EXPECT_EQ(cb_hlist_first(&h), &n1);
    cb_hlist_insert_head(&h, &n0);
    EXPECT_EQ(cb_hlist_first(&h), &n0);
}

TEST(testCase, cb_hlist_test14)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1, n2;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_init(&n2);
    EXPECT_EQ(cb_hlist_tail(&h), nullptr);
    cb_hlist_insert_head(&h, &n0);
    EXPECT_EQ(cb_hlist_tail(&h), &n0);
    cb_hlist_insert_after(&n0, &n1);
    EXPECT_EQ(cb_hlist_tail(&h), &n1);
    cb_hlist_insert_after(&n1, &n2);
    EXPECT_EQ(cb_hlist_tail(&h), &n2);
}

TEST(testCase, cb_hlist_test15)
{
    struct test_object
    {
        cb_hlist_t n0;
        cb_uint8_t i0;
        cb_hlist_t n1;
        cb_uint8_t i1;
        cb_hlist_t n2;
        cb_uint8_t i2;
        cb_hlist_t n3;
        cb_uint32_t i4;
        cb_hlist_t n4;
    };
    struct test_object obj;

    obj.i0 = 0;
    obj.i1 = 0;
    obj.i2 = 0;
    obj.i4 = 0;
    cb_hlist_init(&obj.n0);
    cb_hlist_init(&obj.n1);
    cb_hlist_init(&obj.n2);
    cb_hlist_init(&obj.n3);
    cb_hlist_init(&obj.n4);
    EXPECT_EQ(cb_hlist_entry(&obj.n0, struct test_object, n0), &obj);
    EXPECT_EQ(cb_hlist_entry(&obj.n1, struct test_object, n1), &obj);
    EXPECT_EQ(cb_hlist_entry(&obj.n2, struct test_object, n2), &obj);
    EXPECT_EQ(cb_hlist_entry(&obj.n3, struct test_object, n3), &obj);
    EXPECT_EQ(cb_hlist_entry(&obj.n4, struct test_object, n4), &obj);
}

TEST(testCase, cb_hlist_test16)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    cb_hlist_t n0, n1;
    int index = 0;

    cb_hlist_init(&n0);
    cb_hlist_init(&n1);
    cb_hlist_insert_head(&h, &n1);
    cb_hlist_insert_head(&h, &n0);
    cb_hlist_for_each(n, &h)
    {
        switch (index)
        {
        case 0:
            EXPECT_EQ(n, &n0);
            break;
        case 1:
            EXPECT_EQ(n, &n1);
            break;
        }
        index = index + 1;
    }
    EXPECT_EQ(index, 2);
}

TEST(testCase, cb_hlist_test17)
{
    cb_hhead_t h = CB_HASH_HEAD_OBJECT_INIT(h);
    struct test_object
    {
        cb_uint8_t i0;
        cb_hlist_t n;
    };
    struct test_object obj[2];

    obj[0].i0 = 0;
    obj[1].i0 = 0;
    cb_hlist_init(&obj[0].n);
    cb_hlist_init(&obj[1].n);
    cb_hlist_insert_head(&h, &obj[1].n);
    EXPECT_EQ(cb_hlist_first_entry(&h, struct test_object, n), &obj[1]);
    cb_hlist_insert_head(&h, &obj[0].n);
    EXPECT_EQ(cb_hlist_first_entry(&h, struct test_object, n), &obj[0]);
}
