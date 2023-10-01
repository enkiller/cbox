#include <string.h>
#include <gtest/gtest.h>
#include "cb_ringbuffer.h"

TEST(testCase, cb_ringbuffer_test01)
{
    cb_ringbuffer_t ringbuff;
    cb_ringbuffer_t *rb;
    char buff[16];

    rb = cb_ringbuffer_init(&ringbuff, buff, sizeof(buff));
    EXPECT_NE(cb_ringbuffer_isempty(rb), 0);
    EXPECT_EQ(cb_ringbuffer_isfull(rb), 0);
    EXPECT_EQ(cb_ringbuffer_total(rb), sizeof(buff));
    EXPECT_EQ(cb_ringbuffer_free(rb), cb_ringbuffer_total(rb));
    EXPECT_EQ(cb_ringbuffer_used(rb), 0);
    EXPECT_EQ(cb_ringbuffer_peek(rb, buff, sizeof(buff)), 0);
    EXPECT_EQ(cb_ringbuffer_discard(rb, sizeof(buff)), 0);
    EXPECT_EQ(cb_ringbuffer_read(rb, buff, sizeof(buff)), 0);
}

TEST(testCase, cb_ringbuffer_test02)
{
    cb_ringbuffer_t ringbuff;
    cb_ringbuffer_t* rb;
    char buff[17];
    char tmp[sizeof(buff)];
    unsigned int buff_size;

    for (unsigned int i = 0; i < sizeof(buff); i++)
    {
        buff[i] = (char)(i & 0xff);
    }
    for (unsigned int i = 0; i < sizeof(buff); i++)
    {
        buff_size = i;
        rb = cb_ringbuffer_init(&ringbuff, buff, buff_size);
        for (unsigned int a = 0; a < buff_size; a++)
        {
            EXPECT_EQ(cb_ringbuffer_write(rb, buff, a), a);
            EXPECT_EQ(!!cb_ringbuffer_isempty(rb), !a);
            EXPECT_EQ(!!cb_ringbuffer_isfull(rb), a == buff_size);
            EXPECT_EQ(cb_ringbuffer_used(rb), a);
            EXPECT_EQ(cb_ringbuffer_free(rb), buff_size - a);
            memset(tmp, '#', sizeof(tmp));
            for (unsigned int b = 0; b < a; b++)
            {
                EXPECT_EQ(cb_ringbuffer_peek(rb, tmp, b), b);
                EXPECT_EQ(memcmp(buff, tmp, b), 0);
                memset(tmp, '#', sizeof(tmp));
                EXPECT_EQ(cb_ringbuffer_peek(rb, tmp, b), b);
                EXPECT_EQ(memcmp(buff, tmp, b), 0);
            }
            memset(tmp, '#', sizeof(tmp));
            EXPECT_EQ(cb_ringbuffer_read(rb, tmp, a), a);
            EXPECT_EQ(memcmp(buff, tmp, a), 0);
            EXPECT_EQ(cb_ringbuffer_write(rb, buff, a), a);
            EXPECT_EQ(cb_ringbuffer_discard(rb, a), a);
            EXPECT_EQ(cb_ringbuffer_free(rb), cb_ringbuffer_total(rb));
        }
    }
}

TEST(testCase, cb_ringbuffer_test03)
{
    cb_ringbuffer_t ringbuff;
    cb_ringbuffer_t* rb;
    char buff[17];
    char tmp[sizeof(buff)];
    unsigned int buff_size;

    for (unsigned int i = 0; i < sizeof(buff); i++)
    {
        buff[i] = (char)(i & 0xff);
    }
    for (unsigned int i = 0; i < sizeof(buff); i++)
    {
        buff_size = i;
        rb = cb_ringbuffer_init(&ringbuff, buff, buff_size);
        for (unsigned int a = 0; a < buff_size; a++)
        {
            for (unsigned int b = 0; b < a; b++)
            {
                EXPECT_EQ(cb_ringbuffer_write(rb, buff, a), a);
                memset(tmp, '#', sizeof(tmp));
                EXPECT_EQ(cb_ringbuffer_peek(rb, tmp, a), a);
                EXPECT_EQ(memcmp(buff, tmp, a), 0);
                memset(tmp, '#', sizeof(tmp));
                EXPECT_EQ(cb_ringbuffer_read(rb, tmp, b), b);
                EXPECT_EQ(cb_ringbuffer_read(rb, tmp + b, a - b), a - b);
                EXPECT_EQ(memcmp(buff, tmp, a), 0);
            }
        }
    }
}

TEST(testCase, cb_ringbuffer_test04)
{
    cb_ringbuffer_t ringbuff;
    cb_ringbuffer_t* rb;
    char buff[17];
    char tmp[sizeof(buff)];
    unsigned int buff_size;

    for (unsigned int i = 0; i < sizeof(buff); i++)
    {
        buff[i] = (char)(i & 0xff);
    }
    for (unsigned int i = 1; i < sizeof(buff); i++)
    {
        buff_size = i;
        rb = cb_ringbuffer_init(&ringbuff, buff, buff_size);
        for (unsigned int a = 0; a < 32; a++)
        {
            EXPECT_EQ(cb_ringbuffer_write(rb, buff, buff_size), buff_size);
            memset(tmp, '#', sizeof(tmp));
            EXPECT_EQ(cb_ringbuffer_peek(rb, tmp, buff_size), buff_size);
            EXPECT_EQ(memcmp(buff, tmp, buff_size), 0);
            memset(tmp, '#', sizeof(tmp));
            EXPECT_EQ(cb_ringbuffer_read(rb, tmp, buff_size), buff_size);
            EXPECT_EQ(memcmp(buff, tmp, buff_size), 0);
        }
    }
}

TEST(testCase, cb_ringbuffer_test05)
{
    cb_ringbuffer_t ringbuff;
    cb_ringbuffer_t* rb;
    char buff[8];
    char read_buf[sizeof(buff) * 2];
    char src_buf[sizeof(buff) * 2];
    unsigned int buff_size;

    for (unsigned int i = 0; i < sizeof(src_buf); i++)
    {
        src_buf[i] = (char)(i & 0xff);
    }

    rb = cb_ringbuffer_init(&ringbuff, buff, sizeof(buff));
    for (unsigned int i = 0; i < sizeof(buff) * 2; i++)
    {
        for (unsigned int j = 0; j < i; j++)
        {
            cb_ringbuffer_overwrite(rb, &src_buf[j], 1);
        }
        if (i < sizeof(buff))
        {
            buff_size = i;
        }
        else
        {
            buff_size = sizeof(buff);
        }
        EXPECT_EQ(cb_ringbuffer_read(rb, read_buf, i), buff_size);
        if (i < sizeof(buff))
        {
            for (unsigned j = 0; j < buff_size; j++)
            {
                EXPECT_EQ(read_buf[j], src_buf[j]);
            }
        }
        else
        {
            for (unsigned j = 0; j < buff_size; j++)
            {
                EXPECT_EQ(read_buf[j], src_buf[j + i - sizeof(buff)]);
            }
        }
    }
}
