#ifndef _CPL_STDIO_H_
#define _CPL_STDIO_H_

#include <typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

s32 cpl_sprintf(s8* buffer, const s8* format, ...);

s32 cpl_printf(const s8 *format, ...);

#ifdef DEBUG
#define FP_ALGO_LOGD(...)     cpl_printf(__VA_ARGS__)
#else
#define FP_ALGO_LOGD(...)
#endif

#ifdef __cplusplus
}
#endif

#endif //_CPL_STDIO_H_
