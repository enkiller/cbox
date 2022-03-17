/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#include "cb_def.h"

#ifndef __CB_HASHFUN_H__
#define __CB_HASHFUN_H__

#ifdef __cplusplus
extern "C" {
#endif

cb_uint32_t cb_hash_string(const void *k);
cb_bool_t cb_hash_string_cmp(const void *d, const void *s);
cb_uint32_t cb_hash_uint32(const void *k);
cb_bool_t cb_hash_uint32_cmp(const void *d, const void *s);

#ifdef __cplusplus
}
#endif

#endif
