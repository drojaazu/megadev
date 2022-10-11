/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file stdint.h
 * @brief POSIX-y stdint
 */

#ifndef MEGADEV__STDINT_H
#define MEGADEV__STDINT_H

/**
 * @typedef int8_t
 * @brief 8 bits signed integer (equivalent to char).
 */
typedef char int8_t;

/**
 * @typedef s8
 * @brief 8 bits signed integer (equivalent to char).
 */
typedef int8_t s8;

/**
 * @typedef int16_t
 * @brief 16 bits signed integer (equivalent to short).
 */
typedef short int16_t;

/**
 * @typedef s16
 * @brief 16 bits signed integer (equivalent to short).
 */
typedef int16_t s16;

/**
 * @typedef int32_t
 * @brief 32 bits signed integer (equivalent to long).
 */
typedef long int32_t;

/**
 * @typedef s32
 * @brief 32 bits signed integer (equivalent to long).
 */
typedef int32_t s32;

/**
 * @typedef uint8_t
 * @brief 8 bits unsigned integer (equivalent to unsigned char).
 */
typedef unsigned char uint8_t;

/**
 * @typedef u8
 * @brief 8 bits unsigned integer (equivalent to unsigned char).
 */
typedef uint8_t u8;

/**
 * @typedef uint16_t
 * @brief 16 bits unsigned integer (equivalent to unsigned short)
 */
typedef unsigned short uint16_t;

/**
 * @typedef u16
 * @brief 16 bits unsigned integer (equivalent to unsigned short)
 */
typedef uint16_t u16;

/**
 * @typedef uint32_t
 * @brief 32 bits unsigned integer (equivalent to unsigned long)
 */
typedef unsigned long uint32_t;

/**
 * @typedef u32
 * @brief 32 bits unsigned integer (equivalent to unsigned long)
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
