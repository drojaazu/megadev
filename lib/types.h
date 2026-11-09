/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file types.h
 * @brief Commonly used typedefs
 *
 * @details
 * The standard integer and size types are defined from the compiler's own
 * builtin type macros, so they always agree with what the compiler assumes
 * for memcpy and friends.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup libc C library subset
 * @brief The freestanding subset of the hosted C library that Megadev
 * provides: `types.h`, `string.h`, `stdlib.h`, `stdio.h`, `ctype.h` and
 * `assert.h`. A standard name has its standard signature and semantics.
 * See the Standard Library section of `docs/dev_in_c.md` for what is
 * deliberately absent.
 * @{
 */

/** @brief Null pointer constant */
#ifdef __cplusplus
#define NULL nullptr
#else
#define NULL ((void *) 0)
#endif

/** @brief Signed result of subtracting two pointers */
typedef __PTRDIFF_TYPE__ ptrdiff_t;

/** @brief Unsigned size of an object, as sizeof yields */
typedef __SIZE_TYPE__ size_t;

/**
 * @brief Set as stdbool.h sets it; bool, true and false are C23 keywords
 * @details The kit is compiled as C23 (`-std=gnu23`), so no definitions are
 * needed. Code that needs stdbool.h for another compiler can include it.
 */
#define __bool_true_false_are_defined 1

/**
 * @typedef int8_t
 * @brief 8 bit signed integer
 */
typedef __INT8_TYPE__ int8_t;

/**
 * @typedef int16_t
 * @brief 16 bit signed integer
 */
typedef __INT16_TYPE__ int16_t;

/**
 * @typedef int32_t
 * @brief 32 bit signed integer
 */
typedef __INT32_TYPE__ int32_t;

/**
 * @typedef uint8_t
 * @brief 8 bit unsigned integer
 */
typedef __UINT8_TYPE__ uint8_t;

/**
 * @typedef uint16_t
 * @brief 16 bit unsigned integer
 */
typedef __UINT16_TYPE__ uint16_t;

/**
 * @typedef uint32_t
 * @brief 32 bit unsigned integer
 */
typedef __UINT32_TYPE__ uint32_t;

/** @brief Signed integer wide enough to hold a pointer */
typedef __INTPTR_TYPE__ intptr_t;

/** @brief Unsigned integer wide enough to hold a pointer */
typedef __UINTPTR_TYPE__ uintptr_t;

#define INT8_MIN  (-0x7F - 1)       ///< Smallest int8_t
#define INT16_MIN (-0x7FFF - 1)     ///< Smallest int16_t
#define INT32_MIN (-0x7FFFFFFF - 1) ///< Smallest int32_t

#define INT8_MAX  0x7F       ///< Largest int8_t
#define INT16_MAX 0x7FFF     ///< Largest int16_t
#define INT32_MAX 0x7FFFFFFF ///< Largest int32_t

#define UINT8_MAX  0xFF       ///< Largest uint8_t
#define UINT16_MAX 0xFFFF     ///< Largest uint16_t
#define UINT32_MAX 0xFFFFFFFF ///< Largest uint32_t

/** @} */

#ifdef __cplusplus
}
#endif
