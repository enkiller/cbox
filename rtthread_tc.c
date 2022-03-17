/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#include "rtthread.h"

extern int cb_testcase(int argc, char *argv[]);

MSH_CMD_EXPORT_ALIAS(cb_testcase, cbox_test, cbox test cmd);
