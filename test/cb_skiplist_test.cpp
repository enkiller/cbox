#include <gtest/gtest.h>
#include <stdio.h>
#include "cb_skiplist.h"

struct cb_skiplist_test_node
{
    cb_skiplist_node_t parent;
    int value;
};

static int cb_skiplist_test_cmp(const cb_skiplist_node_t *v1, const cb_skiplist_node_t *v2)
{
    struct cb_skiplist_test_node *t1 = cb_container_of(v1, struct cb_skiplist_test_node, parent);
    struct cb_skiplist_test_node *t2 = cb_container_of(v2, struct cb_skiplist_test_node, parent);
    return t1->value - t2->value;
}

TEST(testCase, cb_skiplist_test01)
{
    struct cb_skiplist_test_node node_tab[16];
    cb_skiplist_t skl;
    unsigned int i;

    EXPECT_EQ(cb_skiplist_init(&skl, 0, cb_skiplist_test_cmp), &skl);
    EXPECT_EQ(!!cb_skiplist_isempty(&skl), 1);
    for (i = 0; i < CB_ARRAY_SIZE(node_tab); i++)
    {
        node_tab[i].value = i;
        EXPECT_EQ(cb_skiplist_node_init(&node_tab[i].parent), &node_tab[i].parent);
        EXPECT_EQ(cb_skiplist_insert(&skl, &node_tab[i].parent), &node_tab[i].parent);
        EXPECT_EQ(!!cb_skiplist_isempty(&skl), 0);
        EXPECT_EQ(cb_skiplist_first(&skl), &node_tab[0].parent);
    }
}

TEST(testCase, cb_skiplist_test02)
{
    struct cb_skiplist_test_node node_tab[16];
    cb_skiplist_t skl;
    unsigned int i;

    EXPECT_EQ(cb_skiplist_init(&skl, 1, cb_skiplist_test_cmp), &skl);
    EXPECT_EQ(!!cb_skiplist_isempty(&skl), 1);
    for (i = 0; i < CB_ARRAY_SIZE(node_tab); i++)
    {
        node_tab[i].value = i;
        EXPECT_EQ(cb_skiplist_node_init(&node_tab[i].parent), &node_tab[i].parent);
        EXPECT_EQ(cb_skiplist_insert(&skl, &node_tab[i].parent), &node_tab[i].parent);
        EXPECT_EQ(!!cb_skiplist_isempty(&skl), 0);
        EXPECT_EQ(cb_skiplist_first(&skl), &node_tab[i].parent);
    }
}

TEST(testCase, cb_skiplist_test03)
{
    struct cb_skiplist_test_node node_tab[16];
    cb_skiplist_t skl;
    unsigned int i, j;

    cb_skiplist_init(&skl, 0, cb_skiplist_test_cmp);
    for (i = 0; i < CB_ARRAY_SIZE(node_tab); i++)
    {
        cb_skiplist_node_init(&node_tab[i].parent);
        node_tab[i].value = i;
        cb_skiplist_insert(&skl, &node_tab[i].parent);
        j = 0;
        cb_skiplist_for_each(item, &skl)
        {
            struct cb_skiplist_test_node *t = cb_container_of(item, struct cb_skiplist_test_node, parent);
            EXPECT_EQ(t->value, j);
            j += 1;
        }
    }
}

TEST(testCase, cb_skiplist_test04)
{
    struct cb_skiplist_test_node node_tab[16];
    cb_skiplist_t skl;
    unsigned int i, j;

    cb_skiplist_init(&skl, 1, cb_skiplist_test_cmp);
    for (i = 0; i < CB_ARRAY_SIZE(node_tab); i++)
    {
        cb_skiplist_node_init(&node_tab[i].parent);
        node_tab[i].value = i;
        cb_skiplist_insert(&skl, &node_tab[i].parent);
        j = i;
        cb_skiplist_for_each(item, &skl)
        {
            struct cb_skiplist_test_node *t = cb_container_of(item, struct cb_skiplist_test_node, parent);
            EXPECT_EQ(t->value, j);
            j -= 1;
        }
    }
}
