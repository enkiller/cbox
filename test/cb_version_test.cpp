/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-27     tyx          first implementation
 */

#include <string.h>
#include <gtest/gtest.h>
#include "cb_version.h"

TEST(testCase, cb_version_test01)
{
    ASSERT_STREQ(cb_version(), CB_VERSION);
    EXPECT_EQ(cb_major_version(), CB_MAINVERSION);
    EXPECT_EQ(cb_minor_version(), CB_SUBVERSION);
    EXPECT_EQ(cb_revise_version(), CB_REVISION);
}
