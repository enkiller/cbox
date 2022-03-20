/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     tyx          first implementation
 */

// version information
#define CB_MAINVERSION                  1L              /**< major version number */
#define CB_SUBVERSION                   0L              /**< minor version number */
#define CB_REVISION                     0L              /**< revise version number */

#define __CB_STR_(s)            #s
#define CB_STR(s)     __CB_STR_(s)

#define CB_VERSION      CB_STR(CB_MAINVERSION) "." \
                        CB_STR(CB_SUBVERSION) "." \
                        CB_STR(CB_REVISION)

// basic
typedef signed char                 cb_int8_t;
typedef unsigned char               cb_uint8_t;
typedef signed short                cb_int16_t;
typedef unsigned short              cb_uint16_t;
typedef signed int                  cb_int32_t;
typedef unsigned int                cb_uint32_t;
typedef int                         cb_bool_t;
typedef unsigned long               cb_size_t;

// bool values
#define cb_true                     ((cb_bool_t)1)
#define cb_false                    ((cb_bool_t)0)

// null
#ifdef __cplusplus
#   define cb_null                  (nullptr)
#else
#   define cb_null                  ((void *)0)
#endif

// compiler
#if defined(__CC_ARM) || defined(__CLANG_ARM)           /* ARM Compiler */
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
#define CB_ARRAY_SIZE(_array)   (sizeof(_array) / sizeof(_array[0]))
