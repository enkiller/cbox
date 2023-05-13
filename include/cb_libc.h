/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-13     tyx          first implementation
 */

#ifndef __CB_LIBC_H__
#define __CB_LIBC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* string */
long cb_strcmp(const char* s1, const char* s2);
void* cb_memcpy(void* dst, const void* src, unsigned int count);

/* stdlib */

#ifdef __cplusplus
}
#endif

#endif
