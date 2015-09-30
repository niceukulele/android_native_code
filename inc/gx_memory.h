/*
 * _log.h
 *
 *  Created on: 2015-1-29
 *      Author: hupeng
 */

#ifndef _GX_MEMORY_H_
#define _GX_MEMORY_H_

void*   gx_Malloc(size_t size);
void    gx_Free(void *buffer);
void*   gx_Realloc(void *buffer, size_t new_size);
int     gx_Memcmp(void *buffer1, void *buffer2, size_t size);
void*   gx_Memcpy(void* destination, const void* source, size_t num);
void*   gx_Memset(void *src, int b, unsigned int num);


#endif /* _GX_MEMORY_H_ */


