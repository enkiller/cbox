/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-08     tyx          first implementation
 */

#include "cb_queue.h"
#include <gtest/gtest.h>

TEST(testCase, cb_queue_test01)
{
    cb_queue_t queue;

    cb_queue_init(&queue);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);
    EXPECT_EQ(cb_queue_pick(&queue), nullptr);
}

TEST(testCase, cb_queue_test02)
{
    cb_queue_t queue;
    cb_queue_item_t item;

    cb_queue_init(&queue);
    cb_queue_item_init(&item);
    cb_queue_put(&queue, &item);
    EXPECT_EQ(cb_queue_pick(&queue), &item);
    EXPECT_EQ(cb_queue_get(&queue), &item);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);
    cb_queue_put(&queue, &item);
    cb_queue_put(&queue, &item);
    EXPECT_EQ(cb_queue_get(&queue), &item);
    EXPECT_EQ(cb_queue_pick(&queue), nullptr);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);
}

TEST(testCase, cb_queue_test03)
{
    cb_queue_t queue;
    cb_queue_item_t item1, item2;

    cb_queue_init(&queue);
    cb_queue_item_init(&item1);
    cb_queue_item_init(&item2);
    cb_queue_put(&queue, &item1);
    cb_queue_put(&queue, &item2);
    EXPECT_EQ(cb_queue_pick(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), &item1);
    EXPECT_EQ(cb_queue_pick(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);
    cb_queue_put(&queue, &item2);
    cb_queue_put(&queue, &item1);
    EXPECT_EQ(cb_queue_pick(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), &item2);
    EXPECT_EQ(cb_queue_pick(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);
    cb_queue_put(&queue, &item2);
    cb_queue_put(&queue, &item1);
    cb_queue_put(&queue, &item2);
    EXPECT_EQ(cb_queue_pick(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), &item1);
    EXPECT_EQ(cb_queue_pick(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);
}

TEST(testCase, cb_queue_test04)
{
    cb_queue_t queue;
    cb_queue_item_t item1, item2, item3;

    cb_queue_init(&queue);
    cb_queue_item_init(&item1);
    cb_queue_item_init(&item2);
    cb_queue_item_init(&item3);

    cb_queue_put(&queue, &item1);
    cb_queue_put(&queue, &item3);
    cb_queue_insert(&queue, &item2, 0);
    EXPECT_EQ(cb_queue_get(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), &item3);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);

    cb_queue_put(&queue, &item1);
    cb_queue_put(&queue, &item3);
    cb_queue_insert(&queue, &item2, 1);
    EXPECT_EQ(cb_queue_get(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), &item3);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);

    cb_queue_put(&queue, &item1);
    cb_queue_put(&queue, &item3);
    cb_queue_insert(&queue, &item2, -2);
    EXPECT_EQ(cb_queue_get(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), &item3);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);

    cb_queue_put(&queue, &item1);
    cb_queue_put(&queue, &item3);
    cb_queue_insert(&queue, &item2, -2);
    EXPECT_EQ(cb_queue_get(&queue), &item1);
    EXPECT_EQ(cb_queue_get(&queue), &item2);
    EXPECT_EQ(cb_queue_get(&queue), &item3);
    EXPECT_EQ(cb_queue_get(&queue), nullptr);
}
