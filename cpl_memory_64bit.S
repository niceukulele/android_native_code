
cpl_memory.o:     file format elf64-littleaarch64


Disassembly of section .text.cpl_malloc:

0000000000000000 <cpl_malloc>:
 * \parma line The line number of the function call.
 *
 * \return A void pointer to the allocated space, or NULL if there is insufficient memory available.
 */
void *cpl_malloc(s32 size, u8 defaultValue, const s8 *pFilename, const s8 *pFuncName, s32 line)
{
   0:	d10083ff 	sub	sp, sp, #0x20
   4:	a90053f3 	stp	x19, x20, [sp]
    u8 *pMem = (u8 *)malloc(size);
   8:	93407c14 	sxtw	x20, w0
   c:	aa1403e0 	mov	x0, x20
 * \parma line The line number of the function call.
 *
 * \return A void pointer to the allocated space, or NULL if there is insufficient memory available.
 */
void *cpl_malloc(s32 size, u8 defaultValue, const s8 *pFilename, const s8 *pFuncName, s32 line)
{
  10:	a9017bf5 	stp	x21, x30, [sp,#16]
  14:	53001c35 	uxtb	w21, w1
    u8 *pMem = (u8 *)malloc(size);
  18:	94000000 	bl	0 <malloc>
  1c:	aa0003f3 	mov	x19, x0
    if (pMem)
  20:	b4000080 	cbz	x0, 30 <cpl_malloc+0x30>
    {
        memset(pMem, defaultValue, size);
  24:	2a1503e1 	mov	w1, w21
  28:	aa1403e2 	mov	x2, x20
  2c:	94000000 	bl	0 <memset>
    }
    return (void *)pMem;
}
  30:	aa1303e0 	mov	x0, x19
  34:	a94053f3 	ldp	x19, x20, [sp]
  38:	a9417bf5 	ldp	x21, x30, [sp,#16]
  3c:	910083ff 	add	sp, sp, #0x20
  40:	d65f03c0 	ret

Disassembly of section .text.cpl_realloc:

0000000000000000 <cpl_realloc>:
 *      the original block is left unchanged. and NULL is returned. If size is zero, then the block
 *      pointed to by pSrc is freed; the return value is NULL. and memblock is left pointing at a freed block.
 */
void *cpl_realloc(void *pSrc, s32 size, const s8 *pFilename, const s8 *pFuncName, s32 line)
{
    return realloc((u8 *)pSrc, size);
   0:	93407c21 	sxtw	x1, w1
   4:	14000000 	b	0 <realloc>

Disassembly of section .text.cpl_free:

0000000000000000 <cpl_free>:
 *
 * \param Previously allocated memory block to be freed.
 */
void cpl_free(void *pMemAddr)
{
    free(pMemAddr);
   0:	14000000 	b	0 <free>
