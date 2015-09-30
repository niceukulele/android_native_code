#ifndef _CPL_STRING_H_
#define _CPL_STRING_H_

#include <typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

s8 *cpl_strcpy(s8 *pDestStr, const s8 *pSrcStr);

s8 *cpl_strncpy(s8 *pDestStr, const s8 *pSrcStr, u32 count);

u32 cpl_strlen(const s8 *pStr);

void cpl_memcpy(void *pDest, const void *pSrc, u32 count);

void *cpl_memset(void *pSrc, s32 c, u32 count);

#ifdef __cplusplus
}
#endif

#endif //_CPL_STRING_H_
