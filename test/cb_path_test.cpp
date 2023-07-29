/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-29     tyx          first implementation
 */

#include <gtest/gtest.h>
#include "cb_path.h"

TEST(testCase, cb_path_test01)
{
    EXPECT_EQ(cb_fnmatch("", ""), 0);
    EXPECT_EQ(cb_fnmatch("123", "123"), 0);
    EXPECT_EQ(cb_fnmatch("1?3", "123"), 0);
    EXPECT_EQ(cb_fnmatch("12?", "123"), 0);
    EXPECT_EQ(cb_fnmatch("?23", "123"), 0);
    EXPECT_EQ(cb_fnmatch("?2?", "123"), 0);
    EXPECT_EQ(cb_fnmatch("1*3", "123"), 0);
    EXPECT_EQ(cb_fnmatch("*23", "123"), 0);
    EXPECT_EQ(cb_fnmatch("12*", "123"), 0);
    EXPECT_EQ(cb_fnmatch("12[3]", "123"), 0);
    EXPECT_EQ(cb_fnmatch("12[34]", "123"), 0);
    EXPECT_EQ(cb_fnmatch("12[43]", "123"), 0);
    EXPECT_EQ(cb_fnmatch("12[!3]", "124"), 0);
    EXPECT_EQ(cb_fnmatch("12**", "123"), 0);
    EXPECT_EQ(cb_fnmatch("1**3", "123"), 0);
    EXPECT_EQ(cb_fnmatch("**23", "123"), 0);
    EXPECT_EQ(cb_fnmatch("1*2*3", "123"), 0);
    EXPECT_EQ(cb_fnmatch("1*2*3", "112233"), 0);
}

TEST(testCase, cb_path_test02)
{
    EXPECT_NE(cb_fnmatch("", "1"), 0);
    EXPECT_NE(cb_fnmatch("123", "1234"), 0);
    EXPECT_NE(cb_fnmatch("1?3", "12"), 0);
    EXPECT_NE(cb_fnmatch("12?", "132"), 0);
    EXPECT_NE(cb_fnmatch("?2?", "23"), 0);
    EXPECT_NE(cb_fnmatch("1*3", "1234"), 0);
    EXPECT_NE(cb_fnmatch("*23", "1234"), 0);
    EXPECT_NE(cb_fnmatch("12*", "134"), 0);
    EXPECT_NE(cb_fnmatch("12[3]", "124"), 0);
    EXPECT_NE(cb_fnmatch("12[34]", "125"), 0);
    EXPECT_NE(cb_fnmatch("12[43]", "125"), 0);
    EXPECT_NE(cb_fnmatch("12[!3]", "123"), 0);
    EXPECT_NE(cb_fnmatch("12[3", "123"), 0);
    EXPECT_NE(cb_fnmatch("12**", "133"), 0);
    EXPECT_NE(cb_fnmatch("1**3", "1234"), 0);
    EXPECT_NE(cb_fnmatch("**23", "12332"), 0);
    EXPECT_NE(cb_fnmatch("1*2*3", "1231234"), 0);
    EXPECT_NE(cb_fnmatch("1*2*3", "1122334"), 0);
}
