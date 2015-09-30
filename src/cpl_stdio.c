#include "cpl_stdio.h"
#include "cpl_string.h"
#include <stdarg.h>
#include <stdio.h>

void drApiLogPrintf(const s8 *fmt, ...)
{

}
s32 cpl_printf(const s8 *format, ...)
{
}
static u32 write_Int(s8 *pBuf, s32 value) {

    u8 stack[10];
    u8 length = 0;
    s8 *_temp = pBuf;

    if (value < 0) {
        *(pBuf++) = '-';
        value *= -1;
    }

    do {
        stack[length] = '0' + (value % 10);
        value /= 10;
        length++;
    } while (value);

    while (length) {
        *pBuf = stack[length - 1];
        length--;
        pBuf++;
    }
    return pBuf - _temp;
}

s32 cpl_sprintf(s8* buffer, const s8* format, ...) {

    va_list arg_ptr;
    u32 format_length = cpl_strlen(format);
    u32 index = 0;

    s32 temp;
    s8 ctemp;
    s8* stemp;

    va_start(arg_ptr, format);
    for (index = 0; index < format_length; ++index) {
        if (format[index] != '%') {
            (*buffer++) = format[index];
        } else {
            switch (format[index + 1]) {
                case 'd':
                    temp = va_arg(arg_ptr,s32);
                    buffer = buffer + write_Int(buffer, temp);
                    break;

                case 's':
                    stemp = (s8*) va_arg(arg_ptr,s8*);
                    cpl_strcpy(buffer, stemp);
                    buffer += cpl_strlen(stemp);
                    break;

                case 'c':
                    ctemp = va_arg(arg_ptr,s32);
                    *(buffer++) = ctemp;
                    break;
            }
            index++;
        }
    }
    *buffer = 0;
    va_end(arg_ptr);

    return --index;
}
