/******************** (C) COPYRIGHT 2013 GOODIX ********************************
* File Name          : gx_image.h
* Author               : peng.hu
* Version              : 1.0
* Date                  : 2015.1.9
* Description         : struct define about image 
*******************************************************************************/

#ifndef _GX_IMAGE_H_
#define _GX_IMAGE_H_

#include <stdint.h>


typedef struct _gx_image_format_t{
	int16_t width;                  /* < width in pixels, -1 indicates unknown value*/
	int16_t height;                 /* < heigt in pixels, -1 indicates unknown value*/
	int16_t ppi;                    /* < sampling intensity, -1 indicates unknown value*/
	int8_t  bits_per_pixels;        /* < Greyscale bit depth of the pixels, -1 indicates unknown value */
	int8_t  channels;               /* < Number of channels */
	int8_t  invert;                 /* < 0 indicates that ridges are black, valleys are white, 1 indicates the inverse situation , -1 indicates unknown value*/
    int8_t  padding[3];             /* < No used*/
} gx_image_format_t;

typedef struct _gx_image_t{
	uint8_t* buffer;                /* < image buffer. */
    gx_image_format_t format;       /* < image format */
    uint32_t capacity;              /* < buffer length  */
	uint16_t frame_count;           /* < always 1 , no support more than one image.*/
	uint8_t  reserve[6];            /* < reservce , no used */
} gx_image_t;

typedef struct _image_quality_t {
	uint32_t coverage;   
	uint32_t quality;    
} image_quality_t;

#endif /* _GX_IMAGE_H_ */


