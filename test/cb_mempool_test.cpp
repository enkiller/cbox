/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-01     tyx          first implementation
 */

#include <gtest/gtest.h>
#include "cb_mempool.h"

TEST(testCase, cb_mempool_test01)
{
    cb_memp_t mempool;
    cb_memp_t *mptr;
    unsigned long long mem[8];
    char *ptr = (char *)mem;

    mptr = cb_mempool_init(&mempool, ptr, sizeof(void *), sizeof(mem));
    EXPECT_EQ((char *)mptr->next_free, &ptr[0]);
    mptr = cb_mempool_init(&mempool, ptr + 1, sizeof(void *), sizeof(mem) - 1);
    EXPECT_EQ((char *)mptr->next_free, &ptr[sizeof(void *)]);
    mptr = cb_mempool_init(&mempool, ptr, sizeof(void *), sizeof(void *));
    EXPECT_EQ((char *)mptr->next_free, &ptr[0]);
    mptr = cb_mempool_init(&mempool, ptr, sizeof(void *), sizeof(void *) - 1);
    EXPECT_EQ((char *)mptr->next_free, (char *)cb_null);
}

TEST(testCase, cb_mempool_test02)
{
    cb_memp_t mempool;
    cb_memp_t *mptr;
    unsigned long long mem[8];
    char *ptr = (char *)mem;
    void *ptr_tab[sizeof(mem) / sizeof(void *)];
    void *tmp_ptr;

    mptr = cb_mempool_init(&mempool, ptr, sizeof(void *), sizeof(mem));
    for (unsigned cnt = 3; cnt != 0; cnt --)
    {
        for (unsigned i = 0; i < sizeof(ptr_tab) / sizeof(ptr_tab[0]); i++)
        {
            ptr_tab[i] = cb_mempool_alloc(mptr);
            EXPECT_NE(ptr_tab[i], (void *)cb_null);
        }
        tmp_ptr = cb_mempool_alloc(mptr);
        EXPECT_EQ(tmp_ptr, (void *)cb_null);
        for (unsigned i = 0; i < sizeof(ptr_tab) / sizeof(ptr_tab[0]); i++)
        {
            cb_mempool_free(mptr, ptr_tab[i]);
        }
    }
}

static int memcharcmp(const void* ptr, int val, size_t size)
{
    int c = 0;

    while (c == 0 && size-- > 0)
    {
        c = *((char *)ptr) - val;
    }
    return c;
}

TEST(testCase, cb_mempool_test03)
{
    cb_memp_t mempool;
    cb_memp_t *mptr;
    unsigned long long mem[8];
    char *ptr = (char *)mem;
    void *ptr_tab[sizeof(mem) / sizeof(void *)];

    for (unsigned blk_size = 0; blk_size < sizeof(void *) * 2; blk_size ++)
    {
        mptr = cb_mempool_init(&mempool, ptr, blk_size, sizeof(mem));
        for (unsigned i = 0; i < sizeof(ptr_tab) / sizeof(ptr_tab[0]); i++)
        {
            ptr_tab[i] = cb_mempool_alloc(mptr);
            if (ptr_tab[i])
            {
                memset(ptr_tab[i], i, blk_size);
            }
        }
        for (unsigned i = 0; i < sizeof(ptr_tab) / sizeof(ptr_tab[0]); i++)
        {
            if (ptr_tab[i])
            {
                int cmp = memcharcmp(ptr_tab[i], i, blk_size);
                EXPECT_EQ(cmp, 0);
            }
        }
        for (unsigned i = 0; i < sizeof(ptr_tab) / sizeof(ptr_tab[0]); i++)
        {
            cb_mempool_free(mptr, ptr_tab[i]);
        }
    }
}
