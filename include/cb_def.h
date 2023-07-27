/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

#ifndef CB_DEF_H_
#define CB_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

// version information
#define CB_MAINVERSION                  1L              /**< major version number */
#define CB_SUBVERSION                   0L              /**< minor version number */
#define CB_REVISION                     0L              /**< revise version number */
#define CB_VERSION                      "1.1.0"

// basic
typedef signed char                 cb_int8_t;
typedef unsigned char               cb_uint8_t;
typedef signed short                cb_int16_t;
typedef unsigned short              cb_uint16_t;
typedef signed int                  cb_int32_t;
typedef unsigned int                cb_uint32_t;
typedef unsigned long               cb_size_t;

// bool values
#define cb_true                     (1)
#define cb_false                    (0)

// null
#define cb_null                  (0)

// compiler
#if defined(__ARMCC_VERSION)           /* ARM Compiler */
    #define cb_inline                   static __inline
#elif defined (__IAR_SYSTEMS_ICC__)                     /* for IAR Compiler */
    #define cb_inline                   static inline
#elif defined (__GNUC__)                                /* GNU GCC Compiler */
    #define cb_inline                   static __inline
#elif defined (__ADSPBLACKFIN__)                        /* for VisualDSP++ Compiler */
    #define cb_inline                   static inline
#elif defined (_MSC_VER)
    #define cb_inline                   static __inline
#elif defined (__TI_COMPILER_VERSION__)
    #define cb_inline                   static inline
#elif defined (__TASKING__)
    #define cb_inline                   static inline
#else
    #error not suppocbed tool chain
#endif

// define
#define CB_ARRAY_SIZE(_array)   (sizeof(_array) / sizeof((_array)[0]))

#ifdef __cplusplus
}
#endif

#endif /* __CB_DEF_H__ */
