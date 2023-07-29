/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-26     tyx          first implementation
 */

#include "cb_version.h"

const char *cb_version(void)
{
    return CB_VERSION;
}

unsigned cb_major_version(void)
{
    return CB_MAINVERSION;
}

unsigned cb_minor_version(void)
{
    return CB_SUBVERSION;
}

unsigned cb_revise_version(void)
{
    return CB_REVISION;
}
