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

#define CB_TEST(__v)    \
    do {    \
        printf("%s F:%s L:%d\n", (__v) ? "PASS" : "FAIL", __FUNCTION__, __LINE__);  \
    } while(0)  \

static const struct cb_hashmap_ops _ops = 
{
    cb_hash_string,
    cb_hash_string_cmp,
};

cb_linkedhashmap_t *cb_linkedhashmap_init(cb_linkedhashmap_t *object, struct cb_hashmap_table *table,
    cb_size_t table_size, const struct cb_hashmap_ops *ops);
void cb_linkedhashmap_remove_all(cb_linkedhashmap_t *object,
    void (*free_item)(cb_linkedhashmap_t *, cb_linkedhashmap_item_t *));
void cb_linkedhashmap_push(cb_linkedhashmap_t *object, cb_linkedhashmap_item_t *item);
cb_linkedhashmap_item_t *cb_linkedhashmap_pop(cb_linkedhashmap_t *obj);
cb_linkedhashmap_item_t *cb_linkedhashmap_peak(cb_linkedhashmap_t *obj);
cb_linkedhashmap_item_t *cb_linkedhashmap_top(cb_linkedhashmap_t *obj, const void *key);
cb_size_t cb_linkedhashmap_size(cb_linkedhashmap_t *object);
cb_linkedhashmap_iter_t *cb_linkedhashmap_iterator_init(cb_linkedhashmap_t *object, cb_linkedhashmap_iter_t *ctx);
cb_linkedhashmap_item_t *cb_linkedhashmap_iterator(cb_linkedhashmap_t *object, cb_linkedhashmap_iter_t *ctx);
// item
cb_linkedhashmap_item_t *cb_linkedhashmap_item_init(cb_linkedhashmap_item_t *item, const void *key);
const void *cb_linkedhashmap_item_key(cb_linkedhashmap_item_t *item);

void cb_linkedhashmap_test01(void)
{
    cb_linkedhashmap_t *linkedhashmap, _linkedhashmap;
    struct cb_hashmap_table table[1];
    struct cb_linkedhashmap_test_data data[2];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    linkedhashmap = cb_linkedhashmap_init(&_linkedhashmap, table, CB_ARRAY_SIZE(table), &_ops);
    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    CB_TEST(cb_linkedhashmap_pop(linkedhashmap) == &data[0].item);
    CB_TEST(cb_linkedhashmap_pop(linkedhashmap) == cb_null);

    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    CB_TEST(cb_linkedhashmap_size(linkedhashmap) == 1);
    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    CB_TEST(cb_linkedhashmap_size(linkedhashmap) == 1);

    CB_TEST(cb_linkedhashmap_peak(linkedhashmap) == &data[0].item);
    CB_TEST(cb_linkedhashmap_pop(linkedhashmap) == &data[0].item);
    CB_TEST(cb_linkedhashmap_peak(linkedhashmap) == cb_null);
    CB_TEST(cb_linkedhashmap_pop(linkedhashmap) == cb_null);
    CB_TEST(cb_linkedhashmap_size(linkedhashmap) == 0);

    cb_linkedhashmap_remove_all(linkedhashmap, cb_null);
}

void cb_linkedhashmap_test02(void)
{
    cb_linkedhashmap_t *linkedhashmap, _linkedhashmap;
    struct cb_hashmap_table table[2];
    struct cb_linkedhashmap_test_data data[2];

    cb_hashmap_test_data_init(data, CB_ARRAY_SIZE(data));
    linkedhashmap = cb_linkedhashmap_init(&_linkedhashmap, table, CB_ARRAY_SIZE(table), &_ops);

    cb_linkedhashmap_push(linkedhashmap, &data[0].item);
    CB_TEST(cb_linkedhashmap_peak(linkedhashmap) == &data[0].item);
    cb_linkedhashmap_push(linkedhashmap, &data[1].item);
    CB_TEST(cb_linkedhashmap_size(linkedhashmap) == 2);
    CB_TEST(cb_linkedhashmap_peak(linkedhashmap) == &data[0].item);
    CB_TEST(cb_linkedhashmap_top(linkedhashmap, data[1].key) == &data[1].item);
    CB_TEST(cb_linkedhashmap_peak(linkedhashmap) == &data[1].item);
    CB_TEST(cb_linkedhashmap_top(linkedhashmap, data[1].key) == &data[1].item);
    CB_TEST(cb_linkedhashmap_peak(linkedhashmap) == &data[1].item);
    CB_TEST(cb_linkedhashmap_top(linkedhashmap, data[0].key) == &data[0].item);
    CB_TEST(cb_linkedhashmap_peak(linkedhashmap) == &data[0].item);
    CB_TEST(cb_linkedhashmap_size(linkedhashmap) == 2);

    cb_linkedhashmap_remove_all(linkedhashmap, cb_null);
}

void cb_linkedhashmap_test03(void)
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
    while ((item = cb_linkedhashmap_iterator(linkedhashmap, &iter)) != cb_null)
    {
        cb_linkedhashmap_item_remove(item);
        printf("iter:%s\n", (char *)cb_linkedhashmap_item_key(item));
        memset(item, 0, sizeof(*item));
    }
    CB_TEST(cb_linkedhashmap_size(linkedhashmap) == 0);

    cb_linkedhashmap_remove_all(linkedhashmap, cb_null);
}

void cb_linkedhashmap_test(void)
{
    cb_linkedhashmap_test01();
    cb_linkedhashmap_test02();
    cb_linkedhashmap_test03();
}
