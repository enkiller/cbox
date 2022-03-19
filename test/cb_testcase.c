/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#include <string.h>

struct cb_test_cmd_tab
{
    const char *tc;
    void (*tc_func)(void);
};

extern void cb_hashmap_test(void);
extern void cb_linkedhashmap_test(void);

static const struct cb_test_cmd_tab _table[] = {
    {"hashmap", cb_hashmap_test},
    {"linkedhashmap", cb_linkedhashmap_test},
};

int cb_testcase(int argc, char *argv[])
{
    int i;

    for (i = 0; i < sizeof(_table) / sizeof(_table[0]); i++)
    {
        _table[i].tc_func();
    }
    return 0;
}
