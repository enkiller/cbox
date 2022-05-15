/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-14     tyx          first implementation
 */

#include "cb_list.h"
#include <gtest/gtest.h>

TEST(testCase, cb_list_test01)
{
    cb_list_t l = CB_LIST_OBJECT_INIT(l);

    EXPECT_EQ(l.next, l.prev);
    EXPECT_EQ(l.next, &l);
}

TEST(testCase, cb_list_test02)
{
    cb_list_t l;

    cb_list_init(&l);
    EXPECT_EQ(l.next, l.prev);
    EXPECT_EQ(l.next, &l);
}

TEST(testCase, cb_list_test03)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;

    cb_list_init(&n0);
    cb_list_init(&n1);
    cb_list_insert_after(&h, &n1);
    EXPECT_EQ(h.next, &n1);
    EXPECT_EQ(h.prev, &n1);
    EXPECT_EQ(n1.next, &h);
    EXPECT_EQ(n1.prev, &h);
    cb_list_insert_after(&h, &n0);
    EXPECT_EQ(h.next, &n0);
    EXPECT_EQ(h.prev, &n1);
    EXPECT_EQ(n0.next, &n1);
    EXPECT_EQ(n0.prev, &h);
    EXPECT_EQ(n1.next, &h);
    EXPECT_EQ(n1.prev, &n0);
}

TEST(testCase, cb_list_test04)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;

    cb_list_init(&n0);
    cb_list_init(&n1);
    cb_list_insert_before(&h, &n0);
    EXPECT_EQ(h.next, &n0);
    EXPECT_EQ(h.prev, &n0);
    EXPECT_EQ(n0.next, &h);
    EXPECT_EQ(n0.prev, &h);
    cb_list_insert_before(&h, &n1);
    EXPECT_EQ(h.next, &n0);
    EXPECT_EQ(h.prev, &n1);
    EXPECT_EQ(n0.next, &n1);
    EXPECT_EQ(n0.prev, &h);
    EXPECT_EQ(n1.next, &h);
    EXPECT_EQ(n1.prev, &n0);
}

TEST(testCase, cb_list_test05)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;

    cb_list_init(&n0);
    cb_list_init(&n1);
    EXPECT_EQ(cb_list_first(&h), &h);
    cb_list_insert_after(&h, &n1);
    EXPECT_EQ(cb_list_first(&h), &n1);
    cb_list_insert_after(&h, &n0);
    EXPECT_EQ(cb_list_first(&h), &n0);
}

TEST(testCase, cb_list_test06)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;

    cb_list_init(&n0);
    cb_list_init(&n1);
    EXPECT_EQ(cb_list_first(&h), &h);
    cb_list_insert_before(&h, &n0);
    EXPECT_EQ(cb_list_first(&h), &n0);
    cb_list_insert_before(&h, &n1);
    EXPECT_EQ(cb_list_first(&h), &n0);
}

TEST(testCase, cb_list_test07)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;

    cb_list_init(&n0);
    cb_list_init(&n1);
    EXPECT_NE(cb_list_isempty(&h), 0);
    cb_list_insert_after(&h, &n1);
    EXPECT_EQ(cb_list_isempty(&h), 0);
    cb_list_insert_after(&h, &n0);
    EXPECT_EQ(cb_list_isempty(&h), 0);
}

TEST(testCase, cb_list_test08)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;

    cb_list_init(&n0);
    cb_list_init(&n1);
    cb_list_insert_after(&h, &n1);
    cb_list_remove(&n1);
    EXPECT_NE(cb_list_isempty(&h), 0);
    cb_list_insert_after(&h, &n1);
    cb_list_insert_after(&h, &n0);
    cb_list_remove(&n0);
    EXPECT_EQ(cb_list_isempty(&h), 0);
    EXPECT_EQ(cb_list_first(&h), &n1);
    cb_list_insert_after(&h, &n0);
    cb_list_remove(&n1);
    EXPECT_EQ(cb_list_isempty(&h), 0);
    EXPECT_EQ(cb_list_first(&h), &n0);
    cb_list_remove(&n0);
    EXPECT_NE(cb_list_isempty(&h), 0);
}

TEST(testCase, cb_list_test09)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;

    cb_list_init(&n0);
    cb_list_init(&n1);
    EXPECT_EQ(cb_list_len(&h), 0);
    cb_list_insert_after(&h, &n1);
    EXPECT_EQ(cb_list_len(&h), 1);
    cb_list_insert_after(&h, &n0);
    EXPECT_EQ(cb_list_len(&h), 2);
    cb_list_remove(&n1);
    EXPECT_EQ(cb_list_len(&h), 1);
    cb_list_remove(&n0);
    EXPECT_EQ(cb_list_len(&h), 0);
}

TEST(testCase, cb_list_test10)
{
    struct test_object
    {
        cb_uint8_t i0;
        cb_uint8_t i1;
        cb_uint8_t i2;
        cb_uint32_t i4;
    };
    struct test_object obj;

    obj.i0 = 0;
    obj.i1 = 0;
    obj.i2 = 0;
    obj.i4 = 0;
    EXPECT_EQ(cb_container_of(&obj.i0, struct test_object, i0), &obj);
    EXPECT_EQ(cb_container_of(&obj.i1, struct test_object, i1), &obj);
    EXPECT_EQ(cb_container_of(&obj.i2, struct test_object, i2), &obj);
    EXPECT_EQ(cb_container_of(&obj.i4, struct test_object, i4), &obj);
}

TEST(testCase, cb_list_test11)
{
    struct test_object
    {
        cb_list_t n0;
        cb_uint8_t i0;
        cb_list_t n1;
        cb_uint8_t i1;
        cb_list_t n2;
        cb_uint8_t i2;
        cb_list_t n3;
        cb_uint32_t i4;
        cb_list_t n4;
    };
    struct test_object obj;

    obj.i0 = 0;
    obj.i1 = 0;
    obj.i2 = 0;
    obj.i4 = 0;
    cb_list_init(&obj.n0);
    cb_list_init(&obj.n1);
    cb_list_init(&obj.n2);
    cb_list_init(&obj.n3);
    cb_list_init(&obj.n4);
    EXPECT_EQ(cb_list_entry(&obj.n0, struct test_object, n0), &obj);
    EXPECT_EQ(cb_list_entry(&obj.n1, struct test_object, n1), &obj);
    EXPECT_EQ(cb_list_entry(&obj.n2, struct test_object, n2), &obj);
    EXPECT_EQ(cb_list_entry(&obj.n3, struct test_object, n3), &obj);
    EXPECT_EQ(cb_list_entry(&obj.n4, struct test_object, n4), &obj);
}

TEST(testCase, cb_list_test12)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    cb_list_t n0, n1;
    int index = 0;

    cb_list_init(&n0);
    cb_list_init(&n1);
    cb_list_insert_after(&h, &n1);
    cb_list_insert_after(&h, &n0);
    cb_list_for_each(n, &h)
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

TEST(testCase, cb_list_test13)
{
    cb_list_t h = CB_LIST_OBJECT_INIT(h);
    struct test_object
    {
        cb_uint8_t i0;
        cb_list_t n;
    };
    struct test_object obj[2];

    obj[0].i0 = 0;
    obj[1].i0 = 0;
    cb_list_init(&obj[0].n);
    cb_list_init(&obj[1].n);
    cb_list_insert_after(&h, &obj[1].n);
    EXPECT_EQ(cb_list_first_entry(&h, struct test_object, n), &obj[1]);
    cb_list_insert_after(&h, &obj[0].n);
    EXPECT_EQ(cb_list_first_entry(&h, struct test_object, n), &obj[0]);
}
