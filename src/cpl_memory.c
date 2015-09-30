#include <stdlib.h>
#include <string.h>
#include "cpl_memory.h"
/**
 * \brief Allocates memory blocks.
 *
 * \param size Bytes to allocate.
 *
 * \param defaultValue Default value for the allocating memory.
 *
 * \param pFilename The filename of the function call.
 *
 * \parma line The line number of the function call.
 *
 * \return A void pointer to the allocated space, or NULL if there is insufficient memory available.
 */
void *cpl_malloc(s32 size, u8 defaultValue, const s8 *pFilename, const s8 *pFuncName, s32 line)
{
    u8 *pMem = (u8 *)malloc(size);
    if (pMem)
    {
        memset(pMem, defaultValue, size);
    }
    return (void *)pMem;
}

/**
 * \brief Reallocates memory blocks.
 *
 * \param pSrc Pointer to previously allocated memory block.
 *
 * \param size Bytes to allocate.
 *
 * \param defaultValue Default value for the allocating memory.
 *
 * \param pFilename The filename of the function call.
 *
 * \parma line The line number of the function call.
 *
 * \return A void pointer to the reallocated(and possibly moved) memory block.
 *      If there is not enough available memory to expand the block to the given size,
 *      the original block is left unchanged. and NULL is returned. If size is zero, then the block
 *      pointed to by pSrc is freed; the return value is NULL. and memblock is left pointing at a freed block.
 */
void *cpl_realloc(void *pSrc, s32 size, const s8 *pFilename, const s8 *pFuncName, s32 line)
{
    return realloc((u8 *)pSrc, size);
}

/**
 * \brief Deallocates or frees a memory block.
 *
 * \param Previously allocated memory block to be freed.
 */
void cpl_free(void *pMemAddr)
{
    free(pMemAddr);
}
