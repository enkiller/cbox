#include <string.h>
#include <gtest/gtest.h>
#include "cb_libc.h"

TEST(testCase, cb_strcmp_test01)
{
    EXPECT_EQ(cb_strcmp("123", "123"), 0);  // v1 == v2
    EXPECT_LT(cb_strcmp("123", "1234"), 0); // v1 < v2
    EXPECT_GT(cb_strcmp("1234", "123"), 0); // v1 > v2
    EXPECT_GT(cb_strcmp("123", ""), 0); // v1 > v2
    EXPECT_LT(cb_strcmp("", "123"), 0); // v1 < v2
}

TEST(testCase, cb_memcpy_test01)
{
    long long buf_src[128];
    long long buf_des[sizeof(buf_src) / sizeof(buf_src[0]) * 2];
    long long buf_cmp[sizeof(buf_src) / sizeof(buf_src[0]) * 2];
    unsigned char* ptr_src = (unsigned char*)&buf_src;
    unsigned char* ptr_cmp = (unsigned char *)&buf_cmp;
    unsigned char* ptr_des = (unsigned char*)&buf_des;

    for (unsigned int i = 0; i < sizeof(buf_src); i++)
    {
        ptr_src[i] = i & 0xFF;
    }
    for (unsigned int i = 0; i < sizeof(void *); i++)
    {
        memset(ptr_cmp, '#', sizeof(buf_cmp));
        memset(ptr_des, '#', sizeof(buf_des));
        /* align copy test */
        memcpy(ptr_cmp + i, buf_src, 64);
        cb_memcpy(ptr_des + i, buf_src, 64);
        EXPECT_EQ(memcmp(buf_cmp, buf_des, sizeof(buf_cmp)), 0);
    }
}

TEST(testCase, cb_memcpy_test02)
{
#define CB_MEMCPY_TEST_SIZE_ 64
    char buf_src[CB_MEMCPY_TEST_SIZE_];
    char buf_des[CB_MEMCPY_TEST_SIZE_];
    char buf_cmp[CB_MEMCPY_TEST_SIZE_];

    for (unsigned i = 0; i < CB_MEMCPY_TEST_SIZE_; i++)
    {
        memset(buf_src, '#', CB_MEMCPY_TEST_SIZE_);
        memset(buf_des, 0, CB_MEMCPY_TEST_SIZE_);
        memset(buf_cmp, 0, CB_MEMCPY_TEST_SIZE_);
        cb_memcpy(buf_des, buf_src + i, CB_MEMCPY_TEST_SIZE_ - i);
        memcpy(buf_cmp, buf_src + i, CB_MEMCPY_TEST_SIZE_ - i);
        EXPECT_EQ(memcmp(buf_cmp, buf_des, sizeof(buf_cmp)), 0);
    }

    for (unsigned i = 0; i < CB_MEMCPY_TEST_SIZE_; i++)
    {
        memset(buf_src, '#', CB_MEMCPY_TEST_SIZE_);
        memset(buf_des, 0, CB_MEMCPY_TEST_SIZE_);
        memset(buf_cmp, 0, CB_MEMCPY_TEST_SIZE_);
        cb_memcpy(buf_des + i, buf_src, CB_MEMCPY_TEST_SIZE_ - i);
        memcpy(buf_cmp + i, buf_src, CB_MEMCPY_TEST_SIZE_ - i);
        EXPECT_EQ(memcmp(buf_cmp, buf_des, sizeof(buf_cmp)), 0);
    }

    for (unsigned i = 0; i < CB_MEMCPY_TEST_SIZE_; i++)
    {
        memset(buf_src, '#', CB_MEMCPY_TEST_SIZE_);
        memset(buf_des, 0, CB_MEMCPY_TEST_SIZE_);
        memset(buf_cmp, 0, CB_MEMCPY_TEST_SIZE_);
        cb_memcpy(buf_des + i, buf_src + i, CB_MEMCPY_TEST_SIZE_ - i);
        memcpy(buf_cmp + i, buf_src + i, CB_MEMCPY_TEST_SIZE_ - i);
        EXPECT_EQ(memcmp(buf_cmp, buf_des, sizeof(buf_cmp)), 0);
    }
}
