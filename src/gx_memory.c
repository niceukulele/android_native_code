/*
 * gx_memory.h
 *
 *  Created on: 2015-1-29
 *      Author: hupeng
 */



#include "gx_typedef.h"

extern void*  malloc(size_t);
extern int    memcmp(const void *, const void *, size_t);
extern void*  memcpy(void *, const void *, size_t);
extern void*  memset(void *, int, size_t);
extern void   free(void *);

void* gx_Malloc(unsigned int size)
{
	return malloc(size);
}

void* MemMalloc(unsigned int size)
{
	return malloc(size);
}

void gx_Free(void *buffer)
{
	free(buffer);
}

void MemFree(void *buffer)
{
	free(buffer);
}

int gx_Memcmp(void *buffer1, void *buffer2, size_t size)
{
	return memcmp(buffer1,buffer2,size);
}

void* gx_Memcpy(void* destination, const void* source, size_t num)
{
	return memcpy(destination,source,num);
}

void*  gx_Memset(void *src, int b, size_t num)
{
	return memset(src, b, num);
}


