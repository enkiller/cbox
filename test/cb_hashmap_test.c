/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-18     tyx          first version.
 */

#include "cb_hashmap.h"
#include "cb_hashfun.h"
#include <stdio.h>
#include <string.h>

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

#define CB_TEST(__v)    \
    do {    \
        printf("%s F:%s L:%d\n", (__v) ? "PASS" : "FAIL", __FUNCTION__, __LINE__);  \
    } while(0)  \

static const struct cb_hashmap_ops _ops = 
{
    cb_hash_string,
    cb_hash_string_cmp,
};

void cb_hashmap_test01(void)
{
    cb_hashmap_t _hashmap;
    cb_hashmap_t *hashmap;
    struct cb_hashmap_table table[1];
    struct cb_hashmap_test_data data[1];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    hashmap = cb_hashmap_init(&_hashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_hashmap_put(hashmap, &data[0].item);
    CB_TEST(cb_hashmap_get(hashmap, data[0].key) == &data[0].item);
    CB_TEST(cb_hashmap_size(hashmap) == 1);
    CB_TEST(cb_hashmap_remove(hashmap, data[0].key) == &data[0].item);
    CB_TEST(cb_hashmap_size(hashmap) == 0);
    CB_TEST(cb_hashmap_get(hashmap, data[0].key) == cb_null);
    cb_hashmap_remove_all(hashmap, cb_null);
}

void cb_hashmap_test02(void)
{
    cb_hashmap_t _hashmap;
    cb_hashmap_t *hashmap;
    struct cb_hashmap_table table[1];
    struct cb_hashmap_test_data data[2];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    hashmap = cb_hashmap_init(&_hashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_hashmap_put(hashmap, &data[0].item);
    CB_TEST(cb_hashmap_replace(hashmap, &data[0].item) == &data[0].item);
    CB_TEST(cb_hashmap_size(hashmap) == 1);
    CB_TEST(cb_hashmap_replace(hashmap, &data[1].item) == cb_null);
    CB_TEST(cb_hashmap_size(hashmap) == 2);
    CB_TEST(cb_hashmap_replace(hashmap, &data[1].item) == &data[1].item);
    CB_TEST(cb_hashmap_size(hashmap) == 2);
    CB_TEST(cb_hashmap_replace(hashmap, &data[0].item) == &data[0].item);
    CB_TEST(cb_hashmap_size(hashmap) == 2);
    CB_TEST(cb_hashmap_get(hashmap, data[0].key) == &data[0].item);
    CB_TEST(cb_hashmap_get(hashmap, data[1].key) == &data[1].item);
    cb_hashmap_remove_all(hashmap, cb_null);
}

void cb_hashmap_test03(void)
{
    cb_hashmap_t _hashmap;
    cb_hashmap_t *hashmap;
    struct cb_hashmap_table table[2];
    struct cb_hashmap_test_data data[3];
    cb_hashmap_item_t *item;

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    hashmap = cb_hashmap_init(&_hashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_hashmap_put(hashmap, &data[0].item);
    CB_TEST(cb_hashmap_size(hashmap) == 1);
    cb_hashmap_put(hashmap, &data[0].item);
    cb_hashmap_put(hashmap, &data[0].item);
    CB_TEST(cb_hashmap_size(hashmap) == 1);
    cb_hashmap_put(hashmap, &data[1].item);
    cb_hashmap_put(hashmap, &data[2].item);
    CB_TEST(cb_hashmap_size(hashmap) == 3);

    cb_hashmap_iter_t iter = CB_HASHMAP_ITER_INIT(hashmap);
    while ((item = cb_hashmap_iterator(hashmap, &iter)) != cb_null)
    {
        cb_hashmap_remove(hashmap, item->key);
        printf("iter:%s\n", (char *)item->key);
        memset(item, 0, sizeof(*item));
    }
    CB_TEST(cb_hashmap_size(hashmap) == 0);
    cb_hashmap_remove_all(hashmap, cb_null);
}

void cb_hashmap_test(void)
{
    cb_hashmap_test01();
    cb_hashmap_test02();
    cb_hashmap_test03();
}
