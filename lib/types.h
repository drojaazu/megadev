/**
 * [ M E G A D E V ]   a Sega Mega CD devkit

 * @file types.h
 * @brief Commonly used typedefs
 */

#ifndef MEGADEV__TYPES_H
#define MEGADEV__TYPES_H

#define NULL ((void *) 0)

typedef long ptrdiff_t;

typedef unsigned long size_t;

#define bool													_Bool
#define true													1
#define false													0
#define __bool_true_false_are_defined 1

/**
 * @typedef int8_t
 * @brief 8 bit signed integer
 */
typedef char int8_t;

/**
 * @typedef s8
 * @brief 8 bit signed integer
 */
typedef int8_t s8;

/**
 * @typedef int16_t
 * @brief 16 bit signed integer
 */
typedef short int16_t;

/**
 * @typedef s16
 * @brief 16 bit signed integer
 */
typedef int16_t s16;

/**
 * @typedef int32_t
 * @brief 32 bit signed integer
 */
typedef long int32_t;

/**
 * @typedef s32
 * @brief 32 bit signed integer
 */
typedef int32_t s32;

/**
 * @typedef uint8_t
 * @brief 8 bit unsigned integer
 */
typedef unsigned char uint8_t;

/**
 * @typedef u8
 * @brief 8 bit unsigned integer
 */
typedef uint8_t u8;

/**
 * @typedef uint16_t
 * @brief 16 bit unsigned integer
 */
typedef unsigned short uint16_t;

/**
 * @typedef u16
 * @brief 16 bit unsigned integer
 */
typedef uint16_t u16;

/**
 * @typedef uint32_t
 * @brief 32 bit unsigned integer
 */
typedef unsigned long uint32_t;

/**
 * @typedef u32
 * @brief 32 bit unsigned integer
 */
typedef uint32_t u32;

typedef signed long intptr_t;
typedef intptr_t		sptr;

typedef unsigned long uintptr_t;
typedef uintptr_t			uptr;

#define INT8_MIN	(-0x7F - 1)
#define INT16_MIN (-0x7FFF - 1)
#define INT32_MIN (-0x7FFFFFFF - 1)

#define INT8_MAX	0x7F
#define INT16_MAX 0x7FFF
#define INT32_MAX 0x7FFFFFFF

#define UINT8_MAX	 0xFF
#define UINT16_MAX 0xFFFF
#define UINT32_MAX 0xFFFFFFFF

#endif