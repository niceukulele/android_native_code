#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _STDINT_H
#define _STDINT_H

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#endif

typedef char s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef int8_t BOOL;
typedef int32_t pointer;
typedef uint8_t fpImg_t;

typedef s16 fixed16_t;
typedef s32 fixed32_t;
typedef s64 fixed64_t;
typedef u16 ufixed16_t;
typedef u32 ufixed32_t;
typedef u64 ufixed64_t;

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef true
#define true  1
#endif

#ifndef false
#define false 0
#endif

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

typedef int32_t fFixedType_t;
typedef int16_t pixel_t;
typedef uint16_t pixel_td;

typedef struct {
    fFixedType_t dbValue;
    int32_t iValue;
} fpValueType_t;

typedef struct {
    fFixedType_t x;
    fFixedType_t y;
} fpCoordinate_fixed_t;

#ifdef __cplusplus
}
#endif

#endif
