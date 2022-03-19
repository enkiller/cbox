/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-19     tyx          first implementation
 */

#include <stdio.h>
#include <stdlib.h>

extern int cb_testcase(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    return cb_testcase(argc, argv);
}
