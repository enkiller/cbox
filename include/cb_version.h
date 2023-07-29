/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-26     tyx          first implementation
 */

#ifndef CB_VERSION_H_
#define CB_VERSION_H_

#include "cb_def.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *cb_version(void);
unsigned cb_major_version(void);
unsigned cb_minor_version(void);
unsigned cb_revise_version(void);

#ifdef __cplusplus
}
#endif

#endif
