/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-19     tyx          first implementation
 */

#ifndef CB_PATH_H_
#define CB_PATH_H_

#include "cb_def.h"

#ifdef __cplusplus
extern "C" {
#endif

int cb_fnmatch(const char * pattern, const char * string);

#ifdef __cplusplus
}
#endif

#endif
