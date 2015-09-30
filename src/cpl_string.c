#include "cpl_string.h"

s8 *cpl_strcpy(s8 *pDestStr, const s8 *pSrcStr) {

    s8 *pTmpStr = pDestStr;

    if (NULL == pDestStr || NULL == pSrcStr) {
        return NULL;
    }

    if (pDestStr == pSrcStr) {
        return pDestStr;
    }

    while ((*pDestStr++ = *pSrcStr++) != '\0')
        ;

    return pTmpStr;
}

s8 *cpl_strncpy(s8 *pDestStr, const s8 *pSrcStr, u32 count) {

    u32 i = 0;
    s8 *pTmpStr = pDestStr;

    if (NULL == pDestStr || NULL == pSrcStr || 0 >= count) {
        return NULL;
    }

    if (pDestStr == pSrcStr) {
        return pDestStr;
    }

    while (((*pDestStr++ = *pSrcStr++) != '\0') && (i++ < count))
        ;

    return pTmpStr;
}

u32 cpl_strlen(const s8 *pStr) {
    u32 len = 0;

    if (NULL == pStr) {
        return 0;
    }

    while (*pStr++ != '\0') {
        len++;
    }

    return len;
}

void cpl_memcpy(void *pDest, const void *pSrc, u32 count) {

    s8 *pDestTmp = (s8 *) pDest;
    s8 *pSrcTmp = (s8 *) pSrc;

    if (NULL == pDest || NULL == pSrc || 0 == count) {
        return;
    }

    if (pDestTmp >= pSrcTmp + count || pSrcTmp >= pDestTmp + count) {

        u32 i = 0;

        while (i++ < count) {
            *pDestTmp++ = *pSrcTmp++;
        }
    }
}

void *cpl_memset(void *pSrc, s32 c, u32 count) {

    u8 *pTmp = pSrc;

    if (NULL != pSrc) {
        u32 i = 0;

        while (i++ < count) {
            *pTmp++ = (u8) c;
        }
    }

    return pSrc;
}
