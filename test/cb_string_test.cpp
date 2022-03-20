#include <gtest/gtest.h>
#include <string.h>
#include "cb_string.h"

TEST(testCase, cb_strcmp_test01)
{
    EXPECT_EQ(cb_strcmp("123", "123"), 0);  // v1 == v2
    EXPECT_LT(cb_strcmp("123", "1234"), 0); // v1 < v2
    EXPECT_GT(cb_strcmp("1234", "123"), 0); // v1 > v2
    EXPECT_GT(cb_strcmp("123", ""), 0); // v1 > v2
    EXPECT_LT(cb_strcmp("", "123"), 0); // v1 < v2
}
