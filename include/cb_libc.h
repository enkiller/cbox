/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-13     tyx          first implementation
 */

#ifndef CB_LIBC_H_
#define CB_LIBC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* string */
long cb_strcmp(const char* s1, const char* s2);
void* cb_memcpy(void* dst, const void* src, unsigned long count);
void* cb_memset(void* dst, int c, unsigned long count);

/* stdlib */
void cb_srandom(unsigned int seed);
int cb_random(void);

#ifdef __cplusplus
}
#endif

#endif
