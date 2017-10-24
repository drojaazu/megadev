/**
 * \file stdint.h
 * \brief POSIX compatible stdint
 */

#ifndef MEGADEV__STDINT_H
#define MEGADEV__STDINT_H

typedef char int8_t;
/**
 *  \typedef s8
 *      8 bits signed integer (equivalent to char).
 */
typedef int8_t s8;


typedef short int16_t;
/**
 *  \typedef s16
 *      16 bits signed integer (equivalent to short).
 */
typedef int16_t s16;


typedef long int32_t;
/**
 *  \typedef s32
 *      32 bits signed integer (equivalent to long).
 */
typedef int32_t s32;


typedef unsigned char uint8_t;
/**
 *  \typedef u8
 *      8 bits unsigned integer (equivalent to unsigned char).
 */
typedef uint8_t u8;


typedef unsigned short uint16_t;
/**
 *  \typedef u16
 *      16 bits unsigned integer (equivalent to unsigned short).
 */
typedef uint16_t u16;


typedef unsigned long uint32_t;
typedef uint32_t u32;

typedef signed long intptr_t;
typedef intptr_t sptr;

typedef unsigned long uintptr_t;
typedef uintptr_t uptr;


#define INT8_MIN    (-0x7f - 1)
#define INT16_MIN   (-0x7fff - 1)
#define INT32_MIN   (-0x7fffffff - 1)

#define INT8_MAX    0x7f
#define INT16_MAX   0x7fff
#define INT32_MAX   0x7fffffff

#define UINT8_MAX   0xff
#define UINT16_MAX  0xffff
#define UINT32_MAX  0xffffffff

#endif
