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
#include "cb_hashmap.h"
#include "cb_hashfun.h"

struct cb_hashmap_test_data
{
    const char *key;
    cb_hashmap_item_t item;
};

static void cb_hashmap_test_data_init(struct cb_hashmap_test_data *data_array, cb_size_t size)
{
#define CB_HASHMAP_TEST_DATA_INIT(_i)   \
    do {    \
        if (size > _i) {    \
            data_array[_i].key = "key"#_i;    \
            cb_hashmap_item_init(&data_array[_i].item, data_array[_i].key); \
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

TEST(testCase, cb_hashmap_test01)
{
    cb_hashmap_t _hashmap;
    cb_hashmap_t *hashmap;
    struct cb_hashmap_table table[1];
    struct cb_hashmap_test_data data[1];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    hashmap = cb_hashmap_init(&_hashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_hashmap_put(hashmap, &data[0].item);
    EXPECT_EQ(cb_hashmap_get(hashmap, data[0].key), &data[0].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 1);
    EXPECT_EQ(cb_hashmap_remove(hashmap, data[0].key), &data[0].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 0);
    EXPECT_EQ(cb_hashmap_get(hashmap, data[0].key), nullptr);
    cb_hashmap_remove_all(hashmap, nullptr);
}

TEST(testCase, cb_hashmap_test02)
{
    cb_hashmap_t _hashmap;
    cb_hashmap_t *hashmap;
    struct cb_hashmap_table table[1];
    struct cb_hashmap_test_data data[2];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    hashmap = cb_hashmap_init(&_hashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_hashmap_put(hashmap, &data[0].item);
    EXPECT_EQ(cb_hashmap_replace(hashmap, &data[0].item), &data[0].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 1);
    EXPECT_EQ(cb_hashmap_replace(hashmap, &data[1].item), nullptr);
    EXPECT_EQ(cb_hashmap_size(hashmap), 2);
    EXPECT_EQ(cb_hashmap_replace(hashmap, &data[1].item), &data[1].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 2);
    EXPECT_EQ(cb_hashmap_replace(hashmap, &data[0].item), &data[0].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 2);
    EXPECT_EQ(cb_hashmap_get(hashmap, data[0].key), &data[0].item);
    EXPECT_EQ(cb_hashmap_get(hashmap, data[1].key), &data[1].item);
    cb_hashmap_remove_all(hashmap, nullptr);
}

TEST(testCase, cb_hashmap_test03)
{
    cb_hashmap_t _hashmap;
    cb_hashmap_t *hashmap;
    struct cb_hashmap_table table[2];
    struct cb_hashmap_test_data data[3];
    cb_hashmap_item_t *item;

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    hashmap = cb_hashmap_init(&_hashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_hashmap_put(hashmap, &data[0].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 1);
    cb_hashmap_put(hashmap, &data[0].item);
    cb_hashmap_put(hashmap, &data[0].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 1);
    cb_hashmap_put(hashmap, &data[1].item);
    cb_hashmap_put(hashmap, &data[2].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 3);

    cb_hashmap_iter_t iter = CB_HASHMAP_ITER_INIT(hashmap);
    while ((item = cb_hashmap_iterator(&iter)) != nullptr)
    {
        cb_hashmap_remove(hashmap, item->key);
        memset(item, 0, sizeof(*item));
    }
    EXPECT_EQ(cb_hashmap_size(hashmap), 0);
    cb_hashmap_remove_all(hashmap, nullptr);
}

static void hashmap_free_item(cb_hashmap_t *object, cb_hashmap_item_t *item)
{
    (void)object;
    memset(item, 0, sizeof(*item));
}

TEST(testCase, cb_hashmap_test04)
{
    cb_hashmap_t _hashmap, _hashmap1;
    cb_hashmap_t *hashmap, *hashmap1;
    struct cb_hashmap_table table[2], table1[4];
    struct cb_hashmap_test_data data[3];
    cb_hashmap_item_t *item;

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    hashmap = cb_hashmap_init(&_hashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_hashmap_put(hashmap, &data[0].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 1);
    cb_hashmap_put(hashmap, &data[1].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 2);
    cb_hashmap_put(hashmap, &data[2].item);
    EXPECT_EQ(cb_hashmap_size(hashmap), 3);

    hashmap1 = cb_hashmap_init(&_hashmap1, table1, CB_ARRAY_SIZE(table1), &_ops);
    cb_hashmap_iter_t iter, *iter_ptr;
    iter_ptr = cb_hashmap_iter_init(hashmap, &iter);
    while ((item = cb_hashmap_iterator(iter_ptr)) != nullptr)
    {
        cb_hashmap_remove(hashmap, item->key);
        cb_hashmap_put(hashmap1, item);
    }
    EXPECT_EQ(cb_hashmap_size(hashmap), 0);
    EXPECT_EQ(cb_hashmap_size(hashmap1), 3);
    cb_hashmap_remove_all(hashmap, hashmap_free_item);
    cb_hashmap_remove_all(hashmap1, hashmap_free_item);
    EXPECT_EQ(cb_hashmap_size(hashmap1), 0);
}
