/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-18     tyx          first version.
 */

#include <stdio.h>
#include <string.h>
#include <gtest/gtest.h>
#include "cb_linkedhashmap.h"
#include "cb_hashfun.h"

struct cb_linkedhashmap_test_data
{
    const char *key;
    cb_linkedhashmap_item_t item;
};

static void cb_hashmap_test_data_init(struct cb_linkedhashmap_test_data *data_array, cb_size_t size)
{
#define CB_HASHMAP_TEST_DATA_INIT(_i)   \
    do {    \
        if (size > _i) {    \
            data_array[_i].key = "key"#_i;    \
            cb_linkedhashmap_item_init(&data_array[_i].item, data_array[_i].key); \
        }   \
    } while (0)

    CB_HASHMAP_TEST_DATA_INIT(0);
    CB_HASHMAP_TEST_DATA_INIT(1);
    CB_HASHMAP_TEST_DATA_INIT(2);
    CB_HASHMAP_TEST_DATA_INIT(3);
    CB_HASHMAP_TEST_DATA_INIT(4);
    CB_HASHMAP_TEST_DATA_INIT(5);
    CB_HASHMAP_TEST_DATA_INIT(6);
    CB_HASHMAP_TEST_DATA_INIT(7);
    CB_HASHMAP_TEST_DATA_INIT(8);
    CB_HASHMAP_TEST_DATA_INIT(9);
}

static const struct cb_hashmap_ops _ops = 
{
    cb_hash_string,
    cb_hash_string_cmp,
};

TEST(testCase, cb_linkedhashmap_test01)
{
    cb_linkedhashmap_t *linkedhashmap, _linkedhashmap;
    struct cb_hashmap_table table[1];
    struct cb_linkedhashmap_test_data data[2];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    linkedhashmap = cb_linkedhashmap_init(&_linkedhashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_pop(linkedhashmap), &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_pop(linkedhashmap), nullptr);

    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 1);
    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 1);

    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_pop(linkedhashmap), &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), nullptr);
    EXPECT_EQ(cb_linkedhashmap_pop(linkedhashmap), nullptr);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 0);

    cb_linkedhashmap_remove_all(linkedhashmap, nullptr);
}

TEST(testCase, cb_linkedhashmap_test02)
{
    cb_linkedhashmap_t *linkedhashmap, _linkedhashmap;
    struct cb_hashmap_table table[2];
    struct cb_linkedhashmap_test_data data[2];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    linkedhashmap = cb_linkedhashmap_init(&_linkedhashmap, table, CB_ARRAY_SIZE(table), &_ops);

    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), &data[0].item);
    cb_linkedhashmap_push(linkedhashmap, &data[1].item);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 2);
    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_top(linkedhashmap, data[1].key), &data[1].item);
    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), &data[1].item);
    EXPECT_EQ(cb_linkedhashmap_top(linkedhashmap, data[1].key), &data[1].item);
    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), &data[1].item);
    EXPECT_EQ(cb_linkedhashmap_top(linkedhashmap, data[0].key), &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 2);
    cb_linkedhashmap_remove_all(linkedhashmap, nullptr);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 0);
}

TEST(testCase, cb_linkedhashmap_test03)
{
    cb_linkedhashmap_t *linkedhashmap, _linkedhashmap;
    struct cb_hashmap_table table[2];
    struct cb_linkedhashmap_test_data data[3];
    cb_linkedhashmap_item_t *item;

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    linkedhashmap = cb_linkedhashmap_init(&_linkedhashmap, table, CB_ARRAY_SIZE(table), &_ops);

    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    cb_linkedhashmap_push(linkedhashmap, &data[1].item);
    cb_linkedhashmap_push(linkedhashmap, &data[2].item);

    cb_linkedhashmap_iter_t iter = CB_LINKEDHASHMAP_ITER_INIT(linkedhashmap);
    while ((item = cb_linkedhashmap_iterator(&iter)) != nullptr)
    {
        cb_linkedhashmap_item_remove(item);
        memset(item, 0, sizeof(*item));
    }
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 0);

    cb_linkedhashmap_remove_all(linkedhashmap, nullptr);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 0);
}

TEST(testCase, cb_linkedhashmap_test04)
{
    cb_linkedhashmap_t *linkedhashmap, _linkedhashmap;
    struct cb_hashmap_table table[2];
    struct cb_linkedhashmap_test_data data[3];
    cb_linkedhashmap_item_t *item;

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    linkedhashmap = cb_linkedhashmap_init(&_linkedhashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    cb_linkedhashmap_push(linkedhashmap, &data[1].item);
    cb_linkedhashmap_push(linkedhashmap, &data[2].item);
    item = cb_linkedhashmap_peak(linkedhashmap);
    EXPECT_EQ(item, &data[0].item);
    EXPECT_EQ(cb_linkedhashmap_get(linkedhashmap, cb_linkedhashmap_item_key(item)), item);
    EXPECT_EQ(cb_linkedhashmap_remove(linkedhashmap, cb_linkedhashmap_item_key(item)), item);
    EXPECT_EQ(cb_linkedhashmap_peak(linkedhashmap), &data[1].item);
    cb_linkedhashmap_remove_all(linkedhashmap, nullptr);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap), 0);
}

static void linkedhashmap_free_item(cb_linkedhashmap_t *object, cb_linkedhashmap_item_t *item)
{
    (void)object;
    memset(item, 0, sizeof(*item));
}

TEST(testCase, cb_linkedhashmap_test05)
{
    cb_linkedhashmap_t *linkedhashmap0, _linkedhashmap0;
    cb_linkedhashmap_t *linkedhashmap1, _linkedhashmap1;
    struct cb_hashmap_table table0[2], table1[4];
    struct cb_linkedhashmap_test_data data[3];
    cb_linkedhashmap_item_t *item;
    cb_linkedhashmap_iter_t iter;

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    linkedhashmap0 = cb_linkedhashmap_init(&_linkedhashmap0, table0, CB_ARRAY_SIZE(table0), &_ops);
    cb_linkedhashmap_push(linkedhashmap0, &data[0].item);
    cb_linkedhashmap_push(linkedhashmap0, &data[1].item);
    cb_linkedhashmap_push(linkedhashmap0, &data[2].item);

    linkedhashmap1 = cb_linkedhashmap_init(&_linkedhashmap1, table1, CB_ARRAY_SIZE(table1), &_ops);
    cb_linkedhashmap_iterator_init(linkedhashmap0, &iter);
    while ((item = cb_linkedhashmap_iterator(&iter)) != nullptr)
    {
        cb_linkedhashmap_item_remove(item);
        cb_linkedhashmap_push(linkedhashmap1, item);
    }
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap0), 0);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap1), 3);
    cb_linkedhashmap_remove_all(linkedhashmap1, linkedhashmap_free_item);
    EXPECT_EQ(cb_linkedhashmap_size(linkedhashmap1), 0);
}
