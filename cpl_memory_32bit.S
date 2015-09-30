
cpl_memory.o:     file format elf32-littlearm


Disassembly of section .text.cpl_malloc:

00000000 <cpl_malloc>:
 * \parma line The line number of the function call.
 *
 * \return A void pointer to the allocated space, or NULL if there is insufficient memory available.
 */
void *cpl_malloc(s32 size, u8 defaultValue, const s8 *pFilename, const s8 *pFuncName, s32 line)
{
   0:	b570      	push	{r4, r5, r6, lr}
   2:	460e      	mov	r6, r1
   4:	4605      	mov	r5, r0
    u8 *pMem = (u8 *)malloc(size);
   6:	f7ff fffe 	bl	0 <malloc>
    if (pMem)
   a:	4604      	mov	r4, r0
   c:	b118      	cbz	r0, 16 <cpl_malloc+0x16>
    {
        memset(pMem, defaultValue, size);
   e:	4631      	mov	r1, r6
  10:	462a      	mov	r2, r5
  12:	f7ff fffe 	bl	0 <memset>
    }
    return (void *)pMem;
}
  16:	4620      	mov	r0, r4
  18:	bd70      	pop	{r4, r5, r6, pc}
  1a:	bf00      	nop

Disassembly of section .text.cpl_realloc:

00000000 <cpl_realloc>:
 *      the original block is left unchanged. and NULL is returned. If size is zero, then the block
 *      pointed to by pSrc is freed; the return value is NULL. and memblock is left pointing at a freed block.
 */
void *cpl_realloc(void *pSrc, s32 size, const s8 *pFilename, const s8 *pFuncName, s32 line)
{
    return realloc((u8 *)pSrc, size);
   0:	f7ff bffe 	b.w	0 <realloc>

Disassembly of section .text.cpl_free:

00000000 <cpl_free>:
 *
 * \param Previously allocated memory block to be freed.
 */
void cpl_free(void *pMemAddr)
{
    free(pMemAddr);
   0:	f7ff bffe 	b.w	0 <free>
