#ifndef _CPL_MEMORY_H_
#define _CPL_MEMORY_H_

#include <typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

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
void *cpl_malloc(s32 size, u8 defaultValue, const s8 *pFilename, const s8 *pFuncName, s32 line);

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
void *cpl_realloc(void *pSrc, s32 size, const s8 *pFilename, const s8 *pFuncName, s32 line);

/**
 * \brief Deallocates or frees a memory block.
 *
 * \param Previously allocated memory block to be freed.
 */
void cpl_free(void *pMemAddr);

#ifdef __cplusplus
}
#endif

#endif /* _CPL_MEMORY_H_ */

