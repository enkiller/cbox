/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-13     tyx          first implementation
 */

#include <gtest/gtest.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "cb_libc.h"
#include "time.h"

TEST(testCase, cb_random_test01)
{
    cb_srandom((unsigned int)(time(NULL) & 0xFFFFFFFF));
    EXPECT_NE(cb_random(), cb_random());
}

static int make_random(int* buf, int count)
{
    while (count > 8)
    {
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        count -= 8;
    }
    while (count > 4)
    {
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        *buf++ = cb_random();
        count -= 4;
    }
    while (count > 0)
    {
        *buf++ = cb_random();
        count -= 1;
    }
    return count;
}

static void inittab(unsigned char *t)
{
    for (int i = 0; i < 256; i++)
    {
        *t = 0;
        for (int j = 0; j < 8; j++)
        {
            if (i & (0x1 << j))
            {
                *t += 1;
            }
        }
        t += 1;
    }
}

static int bit1cnt(unsigned char* buf, int size)
{
    unsigned char tab[256];
    int cnt = 0;

    inittab(tab);
    while (size > 8)
    {
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        size -= 8;
    }
    while (size > 4)
    {
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        cnt += tab[*buf++];
        size -= 4;
    }
    while (size > 0)
    {
        cnt += tab[*buf++];
        size -= 1;
    }
    return cnt;
}

TEST(testCase, cb_random_test02)
{
    int* testbuf;
    int test_cnt = 1024 *1024;
    int bitcnt;

    testbuf = new int[test_cnt];
    make_random(testbuf, test_cnt);
    bitcnt = bit1cnt((unsigned char *)testbuf, sizeof(int) * test_cnt);
    EXPECT_LT(fabs((double)bitcnt / (sizeof(int) * test_cnt * 8) - 0.5), 0.05);
    delete[] testbuf;
}
