/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-05     tyx          first implementation
 */

#include <gtest/gtest.h>
#include "cb_radixtree.h"
#include <stdint.h>

#define zeroptr (0)

#define INVALID_VALUE   0x7FFFFFFF

class TestRT: public cb_rtroot_t
{
public:
    TestRT(unsigned long fail_index = ~0x0UL) {
        this->fail_index = fail_index;
        this->alloc_cnt = 0;
        this->free_cnt = 0;

        this->alloc_node = alloc_node_func;
        this->free_node = free_node_func;
        this->def_value = (void *)INVALID_VALUE;
    }

    virtual ~TestRT() { }

    static cb_rtslots_t *alloc_node_func(cb_rtroot_t *root, unsigned char slots)
    {
        TestRT *test_root = static_cast<TestRT *>(root);
        cb_rtslots_t *s = zeroptr;
        if (test_root->fail_index != test_root->alloc_cnt)
        {
            test_root->alloc_cnt += 1;
            s = new cb_rtslots_t[slots];
        }
        else
        {
            test_root->fail_index = ~0x0UL;
        }
        return s;
    }

    static void free_node_func(cb_rtroot_t* root, cb_rtslots_t *n, unsigned char slots)
    {
        TestRT *test_root = static_cast<TestRT *>(root);
        (void)slots;
        delete[] n;
        test_root->free_cnt += 1;
    }

    cb_rtroot_t *parent(void)
    {
        cb_rtroot_t *p = this;
        return p;
    }

    int insert(uintptr_t key, uintptr_t value)
    {
        return cb_radix_tree_insert(parent(), (void *)key, (void *)value);
    }

    uintptr_t lookup(uintptr_t key)
    {
        return (uintptr_t)cb_radix_tree_lookup(parent(), (void *)key);
    }

    uintptr_t remove(uintptr_t key)
    {
        return (uintptr_t)cb_radix_tree_remove(parent(), (void *)key);
    }

 public:
    unsigned long alloc_cnt;
    unsigned long free_cnt;
    unsigned long fail_index;
};

TEST(testCase, cb_radix_tree_init_test01)
{
    TestRT test_root;
    cb_rtroot_t *ptr;
    uintptr_t zero = zeroptr;
    uintptr_t key = zero + 1;
    uintptr_t value;
    unsigned long i;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, CB_RADIX_TREE_MAP_SIZE);
    for (i = 0; i < sizeof(void *) * 8 - 1; i++)
    {
        value = test_root.lookup(key);
        EXPECT_EQ(value, INVALID_VALUE);
        key = zero + ((key - zero) << 0x1) - 1;
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}

TEST(testCase, cb_radix_tree_test02)
{
    TestRT test_root;
    cb_rtroot_t *ptr;
    uintptr_t zero = zeroptr;
    uintptr_t key = zero + 1;
    uintptr_t value;
    unsigned long i;
    int res;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, CB_RADIX_TREE_MAP_SIZE);
    for (i = 0; i < sizeof(void *) * 8; i++)
    {
        res = test_root.insert(key, key);
        EXPECT_EQ(res, 0);
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
        key = zero + ((key - zero) << 0x1) - 1;
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}

TEST(testCase, cb_radix_tree_test03)
{
    TestRT test_root;
    cb_rtroot_t *ptr;
    uintptr_t zero = zeroptr;
    uintptr_t key;
    uintptr_t value;
    unsigned long i;
    int res;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, CB_RADIX_TREE_MAP_SIZE);
    for (i = 0, key = zero + 1; i < sizeof(void *) * 8; i++)
    {
        res = test_root.insert(key, key);
        EXPECT_EQ(res, 0);
        key = zero + ((key - zero) << 0x1) - 1;
    }
    for (i = 0, key = zero + 1; i < sizeof(void *) * 8; i++)
    {
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
        key = zero + ((key - zero) << 0x1) - 1;
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}

TEST(testCase, cb_radix_tree_test04)
{
    TestRT test_root;
    cb_rtroot_t *ptr;
    uintptr_t zero = zeroptr;
    uintptr_t key = zero + 1;
    uintptr_t value;
    unsigned long i;
    int res;
    unsigned long long tmp;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, CB_RADIX_TREE_MAP_SIZE);
    for (i = 0; i < sizeof(void *) * 8 - 1; i++)
    {
        key = zero + ((key - zero) << 0x1) - 1;
    }
    for (i = 0; i < sizeof(void *) * 8; i++)
    {
        res = test_root.insert(key, key);
        EXPECT_EQ(res, 0);
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
        tmp = key - zero;
        key = zero + (tmp >> 0x1ULL);
    }
    for (i = 0, key = zero + 1; i < sizeof(void *) * 8; i++)
    {
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
        key = zero + ((key - zero) << 0x1) - 1;
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}

TEST(testCase, cb_radix_tree_test05)
{
    TestRT test_root;
    cb_rtroot_t *ptr;
    uintptr_t zero = zeroptr;
    uintptr_t key;
    uintptr_t value;
    unsigned long i, c;
    int res;

    for (c = 1; c < 8; c++)
    {
        ptr = test_root.parent();
        ptr = cb_radix_tree_init(ptr, 0x1U << c);
        for (i = 0, key = zero + 1; i < sizeof(void *) * 8; i++)
        {
            res = test_root.insert(key, key);
            EXPECT_EQ(res, 0);
            key = zero + ((key - zero) << 0x1) - 1;
        }
        for (i = 0, key = zero + 1; i < sizeof(void *) * 8; i++)
        {
            value = test_root.lookup(key);
            EXPECT_EQ(value, key);
            key = zero + ((key - zero) << 0x1) - 1;
        }
        cb_radix_tree_deinit(ptr);
        EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
    }
}

TEST(testCase, cb_radix_tree_test06)
{
    TestRT test_root;
    cb_rtroot_t *ptr;
    int res;
    long i, cnt = 100000;
    uintptr_t zero = zeroptr;
    uintptr_t key = zeroptr;
    uintptr_t value;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, 4);
    for (i = cnt; i >= 0; i--)
    {
        key = zero + i;
        value = key;
        res = test_root.insert(key, value);
        EXPECT_EQ(res, 0);
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
    }
    for (i = 0; i < cnt; i++)
    {
        key = zero + i;
        value = key;
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}

TEST(testCase, cb_radix_tree_test07)
{
    int res;
    unsigned long alloc_cnt;
    unsigned long record[10];
    unsigned long i, cnt = sizeof(record) / sizeof(record[0]);
    uintptr_t zero = zeroptr;
    uintptr_t key = zeroptr;
    uintptr_t value;

    {
        TestRT test_root;
        cb_rtroot_t *ptr;
        ptr = test_root.parent();
        ptr = cb_radix_tree_init(ptr, 4);
        for (i = 0; i < cnt; i++)
        {
            key = zero + i;
            value = key;
            res = test_root.insert(key, value);
            EXPECT_EQ(res, 0);
        }
        cb_radix_tree_deinit(ptr);
        EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
        alloc_cnt = test_root.alloc_cnt;
    }
    for (i = 0; i <= alloc_cnt; i++)
    {
        TestRT test_root(i);
        cb_rtroot_t *ptr;
        memset(record, 0, sizeof(record));
        ptr = test_root.parent();
        ptr = cb_radix_tree_init(ptr, 4);
        for (unsigned long j = 0; j < cnt; j++)
        {
            key = zero + j;
            value = key;
            if (test_root.insert(key, value) == 0)
            {
                record[j] = 1;
            }
        }
        for (unsigned long j = 0; j < cnt; j++)
        {
            key = zero + j;
            value = key;
            if (record[j] != 0)
            {
                value = test_root.lookup(key);
                EXPECT_EQ(value, key);
            }
        }
        cb_radix_tree_deinit(ptr);
        EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
    }
}

TEST(testCase, cb_radix_tree_test08)
{
    int res;
    unsigned long i, cnt = 100;
    uintptr_t zero = zeroptr;
    uintptr_t key = zeroptr;
    uintptr_t value;
    TestRT test_root;
    cb_rtroot_t *ptr;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, 4);
    for (i = 0; i < cnt; i++)
    {
        key = zero + i;
        value = key;
        res = test_root.insert(key, value);
        EXPECT_EQ(res, 0);
        value = test_root.remove(key);
        EXPECT_EQ(value, key);
        value = test_root.lookup(key);
        EXPECT_EQ(value, INVALID_VALUE);
    }
    for (i = 0; i < cnt; i++)
    {
        key = zero + i;
        value = key;
        value = test_root.lookup(key);
        EXPECT_EQ(value, INVALID_VALUE);
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}

TEST(testCase, cb_radix_tree_test09)
{
    int res;
    unsigned long i, cnt = 100;
    uintptr_t zero = zeroptr;
    uintptr_t key = zeroptr;
    uintptr_t value;
    TestRT test_root;
    cb_rtroot_t *ptr;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, 4);
    for (i = 0; i < cnt; i++)
    {
        key = zero + i;
        value = key;
        res = test_root.insert(key, value);
        EXPECT_EQ(res, 0);
    }
    for (i = 0; i < cnt; i++)
    {
        key = zero + i;
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
        value = test_root.remove(key);
        EXPECT_EQ(value, key);
        value = test_root.lookup(key);
        EXPECT_EQ(value, INVALID_VALUE);
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}

TEST(testCase, cb_radix_tree_test10)
{
    int res;
    unsigned long record[10];
    long i, cnt = sizeof(record) / sizeof(record[0]);
    unsigned long used;
    uintptr_t zero = zeroptr;
    uintptr_t key = zeroptr;
    uintptr_t value;
    TestRT test_root;
    cb_rtroot_t *ptr;

    ptr = test_root.parent();
    ptr = cb_radix_tree_init(ptr, 4);
    for (i = 0; i < cnt; i++)
    {
        used = test_root.alloc_cnt - test_root.free_cnt;
        record[i] = used;
        key = zero + i;
        value = key;
        res = test_root.insert(key, value);
        EXPECT_EQ(res, 0);
    }
    for (i = cnt - 1; i >= 0; i--)
    {
        key = zero + i;
        value = test_root.lookup(key);
        EXPECT_EQ(value, key);
        value = test_root.remove(key);
        EXPECT_EQ(value, key);
        cb_radix_tree_shrink(ptr);
        used = test_root.alloc_cnt - test_root.free_cnt;
        EXPECT_EQ(used, record[i]);
    }
    cb_radix_tree_deinit(ptr);
    EXPECT_EQ(test_root.alloc_cnt, test_root.free_cnt);
}
