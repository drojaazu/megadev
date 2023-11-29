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

#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1

/**
 * @typedef int8_t
 * @brief 8 bits signed integer
 */
typedef char int8_t;

/**
 * @typedef s8
 * @brief 8 bits signed integer
 */
typedef int8_t s8;

/**
 * @typedef int16_t
 * @brief 16 bits signed integer
 */
typedef short int16_t;

/**
 * @typedef s16
 * @brief 16 bits signed integer
 */
typedef int16_t s16;

/**
 * @typedef int32_t
 * @brief 32 bits signed integer
 */
typedef long int32_t;

/**
 * @typedef s32
 * @brief 32 bits signed integer
 */
typedef int32_t s32;

/**
 * @typedef uint8_t
 * @brief 8 bits unsigned integer
 */
typedef unsigned char uint8_t;

/**
 * @typedef u8
 * @brief 8 bits unsigned integer
 */
typedef uint8_t u8;

/**
 * @typedef uint16_t
 * @brief 16 bits unsigned integer
 */
typedef unsigned short uint16_t;

/**
 * @typedef u16
 * @brief 16 bits unsigned integer
 */
typedef uint16_t u16;

/**
 * @typedef uint32_t
 * @brief 32 bits unsigned integer
 */
typedef unsigned long uint32_t;

/**
 * @typedef u32
 * @brief 32 bits unsigned integer
 */
typedef uint32_t u32;

typedef signed long intptr_t;
typedef intptr_t sptr;

typedef unsigned long uintptr_t;
typedef uintptr_t uptr;

#define INT8_MIN (-0x7f - 1)
#define INT16_MIN (-0x7fff - 1)
#define INT32_MIN (-0x7fffffff - 1)

#define INT8_MAX 0x7f
#define INT16_MAX 0x7fff
#define INT32_MAX 0x7fffffff

#define UINT8_MAX 0xff
#define UINT16_MAX 0xffff
#define UINT32_MAX 0xffffffff

#endif