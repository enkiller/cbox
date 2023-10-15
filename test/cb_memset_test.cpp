/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-10-15     tyx          first implementation
 */

#include <string.h>
#include <gtest/gtest.h>
#include "cb_libc.h"

TEST(testCase, cb_memset_test01)
{
    unsigned char buf_test[32];
    unsigned char buf_cmp[sizeof(buf_test)];
    unsigned long pos, len;

    for (len = 0; len < sizeof(buf_test); len++)
    {
        for (pos = 0; pos < len; pos++)
        {
            memset(buf_test, '#', sizeof(buf_test));
            memset(buf_cmp, '#', sizeof(buf_cmp));
            cb_memset(&buf_test[pos], 0x11, len - pos);
            memset(&buf_cmp[pos], 0x11, len - pos);
            EXPECT_EQ(memcmp(buf_test, buf_cmp, sizeof(buf_test)), 0);
        }
    }
}
