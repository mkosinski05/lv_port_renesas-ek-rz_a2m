/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/******************************************************************************
* File Name   : frame_buffer_typedef.h
* Description : Frame Buffer and Color Format
*
* Data structure:
* - <frame_buffer_t>
* - | <pixel_format_t>
* - | <byte_per_pixel_t>
* - | <frame_buffer_delegate_t>
* Version     : 1.12
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stdint.h>

#ifndef  FRAME_BUFFER_TYPEDEF_H
#define  FRAME_BUFFER_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
#define  USE_CONFIG_R8G8B8A8_T  /* r8g8b8a8_t */

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Structure: r8g8b8a8_t
*    Pixel format in which bytes are arranged in the order of red, green, blue, and alpha
*
* Description:
*    A value of 0 (black, transparent) to 255 (light, opaque)
*    is specified for each of red, green, blue and alpha.
************************************************************************/
typedef union { 
    struct { 
        /* Variable: u.Red */
        uint8_t  Red; 

        /* Variable: u.Green */
        uint8_t  Green; 

        /* Variable: u.Blue */
        uint8_t  Blue; 

        /* Variable: u.Alpha
        *    Alpha, opaque level */
        uint8_t  Alpha; 
    } u;  /* 1st member "u" can be used by initializer */

    /* Variable: Value */
    uint32_t
        Value;  /* This is depend on CPU endian. Use only copy or compare */ 

} r8g8b8a8_t; 


/* Section: byte_per_pixel_t */
/***********************************************************************
* Type: byte_per_pixel_t
*    A type of number of bytes per pixel
*
* Description:
*    When one pixel is less than one byte (BitPerPixel < 8),
*    a value shifted from the number of bits per pixel is set.
*
*    Related constants are <R_BYTE_PER_PIXEL_MASK>, <R_BIT_PER_PIXEL_MASK>,
*    <R_BYTE_PER_PIXEL_SHIFT>, <R_BIT_PER_PIXEL_SHIFT>.
*
*    See <Section_byte_per_pixel_t>.
************************************************************************/
typedef int_fast32_t  byte_per_pixel_t;

    /* Constant: R_BYTE_PER_PIXEL_MASK
    *    Mask value of number of byte per pixel */
#define  R_BYTE_PER_PIXEL_MASK  ( 0x00FFu )  /* unsigned */

    /* Constant: R_BIT_PER_PIXEL_MASK
    *    Mask value of number of bit per pixel */
#define  R_BIT_PER_PIXEL_MASK   ( 0xFF00u )  /* unsigned */

    /* Constant: R_BYTE_PER_PIXEL_SHIFT
    *    Shift value of number of byte per pixel */
#define  R_BYTE_PER_PIXEL_SHIFT  ( 0 )  /* signed constant */

    /* Constant: R_BIT_PER_PIXEL_SHIFT
    *    number of bit per pixel */
#define  R_BIT_PER_PIXEL_SHIFT  ( 8 )  /* signed constant */


/* Section: frame_buffer_delegate_t */
/***********************************************************************
* Type: frame_buffer_delegate_t
*    User defined type of variable. RGA does not access it.
*
* Description:
*    This is a class of objects referenced from the delegate member variable of <frame_buffer_t>.
*    This class can be defined by libraries that use <frame_buffer_t> or by the application.
*    This class is defined as follows by default. Therefore, frame_buffer_t::delegate is a void* type.
*
*    > #include  "frame_buffer_typedef.h"
*    > typedef  void  frame_buffer_delegate_t;
*
*    To change the type, define the type before performing #include for
*    the header file that defines frame_buffer_delegate_t,
*    and then perform #define frame_buffer_delegate_t frame_buffer_delegate_t.
*
*    > typedef  MyFrameBufferClass       frame_buffer_delegate_t;
*    > #define  frame_buffer_delegate_t  frame_buffer_delegate_t
*    > 
*    > #include  "frame_buffer_typedef.h"  // default define frame_buffer_delegate_t
************************************************************************/
typedef void  frame_buffer_delegate_t;


/* Section: pixel_format_t_constants */
/***********************************************************************
* Enumeration: pixel_format_t
*    Type of identifier of pixel format. See <pixel_format_t_constants>
*
* Description:
*    See <Figure_RGA_PixelFormat>.
************************************************************************/
typedef enum {

    /* Constant: PIXEL_FORMAT_UNKNOWN
    *    Unknown as <pixel_format_t> */
    PIXEL_FORMAT_UNKNOWN  =  0,

    /* Constant: PIXEL_FORMAT_ARGB8888
    *    ARGB8888 as <pixel_format_t> */
    PIXEL_FORMAT_ARGB8888 =  1,

    /* Constant: PIXEL_FORMAT_RGB565
    *    RGB565 as <pixel_format_t> */
    PIXEL_FORMAT_RGB565   =  3,

    /* Constant: PIXEL_FORMAT_ARGB4444
    *    ARGB4444 as <pixel_format_t> */
    PIXEL_FORMAT_ARGB4444 =  5,

    /* Constant: PIXEL_FORMAT_A8
    *    8-bit alpha as <pixel_format_t> */
    PIXEL_FORMAT_A8       = 11,

    /* Constant: PIXEL_FORMAT_A4
    *    4-bit alpha as <pixel_format_t> */
    PIXEL_FORMAT_A4       = 14,

    /* Constant: PIXEL_FORMAT_A1
    *    1-bit alpha as <pixel_format_t> */
    PIXEL_FORMAT_A1       = 13,

    /* Constant: PIXEL_FORMAT_RGB888
    *    RGB888 as <pixel_format_t> */
    PIXEL_FORMAT_RGB888   = 15,

    /* Constant: PIXEL_FORMAT_R8G8B8A8
    *    R8G8B8A8 as <pixel_format_t> */
    PIXEL_FORMAT_R8G8B8A8 =  6 | (1 << 4),

    /* Constant: PIXEL_FORMAT_XRGB8888
    *    XRGB8888 as <pixel_format_t> */
    PIXEL_FORMAT_XRGB8888 =  0 | (1 << 6),

    /* Constant: PIXEL_FORMAT_ARGB1555
    *    ARGB1555 as <pixel_format_t> */
    PIXEL_FORMAT_ARGB1555 =  4 | (1 << 6),

    /* Constant: PIXEL_FORMAT_YCbCr422
    *    YCbCr422 as <pixel_format_t> */
    PIXEL_FORMAT_YCbCr422 =  2 | (1 << 16),

    /* Constant: PIXEL_FORMAT_YUV422
    *    YUV422 as <pixel_format_t> */
    PIXEL_FORMAT_YUV422   =  2 | (1 << 16),

    /* Constant: PIXEL_FORMAT_YUV422_GRAY_SCALE_IS_0x80
    *    YUV422 as <pixel_format_t>. Cb, Cr value of gray scale is 0x80 */
    PIXEL_FORMAT_YUV422_GRAY_SCALE_IS_0x80  = 2 | (1 << 16),

    /* Constant: PIXEL_FORMAT_JPEG
    *    JPEG as <pixel_format_t> */
    PIXEL_FORMAT_JPEG     = 12 | (2 << 8),

    /* Constant: PIXEL_FORMAT_PNG
    *    PNG as <pixel_format_t> */
    PIXEL_FORMAT_PNG      = 12 | (3 << 8),

    /* Constant: PIXEL_FORMAT_GIF
    *    GIF as <pixel_format_t> */
    PIXEL_FORMAT_GIF      = 12 | (4 << 8),

    /* Constant: PIXEL_FORMAT_CLUT1
    *    1-bit CLUT as <pixel_format_t>. CLUT is color look up table */
    PIXEL_FORMAT_CLUT1    = 12 | (1 << 12),

    /* Constant: PIXEL_FORMAT_CLUT4
    *    4-bit CLUT as <pixel_format_t>. CLUT is color look up table */
    PIXEL_FORMAT_CLUT4    = 12 | (4 << 12),

    /* Constant: PIXEL_FORMAT_CLUT8
    *    8-bit CLUT as <pixel_format_t>. CLUT is color look up table */
    PIXEL_FORMAT_CLUT8    = 12 | (8 << 12)

} pixel_format_t; 


/* Section: frame_buffer_t */
/***********************************************************************
* Constant: FRAME_BUFFER_T_MAX_BUFFER_COUNT
************************************************************************/
#define  FRAME_BUFFER_T_MAX_BUFFER_COUNT  ( 3 )


/***********************************************************************
* Structure: frame_buffer_t
*    Drawing destination frame buffer type
*
* Example:
*    > static uint8_t  gs_frame_buffer_memory[2][800][480][4];
*    > frame_buffer_t  frame;
*    > frame.buffer_address[0] = gs_frame_buffer_memory[0];
*    > frame.buffer_address[1] = gs_frame_buffer_memory[1];
*    > frame.buffer_count = 2;
*    > frame.show_buffer_index = 0;
*    > frame.draw_buffer_index = 1;
*    > frame.width = 800;
*    > frame.byte_per_pixel = 4;
*    > frame.stride = 800 * 4;
*    > frame.height = 480
*    > frame.pixel_format = PIXEL_FORMAT_ARGB8888;
*    > frame.delegate = NULL;
************************************************************************/
typedef struct {

    /* Variable: buffer_address
    *    Array of starting address of the frame buffer
    *
    * Description:
    *    Logical address of cache area (same as normal variables).
    *    Specify a multiple of 32.
    */
    uint8_t*  buffer_address[ FRAME_BUFFER_T_MAX_BUFFER_COUNT ]; 


    /* Variable: buffer_count
    *    Number of buffer_address array elements
    *
    * Description:
    *    1: Single buffer, 2: Double buffers
    */
    int_fast32_t  buffer_count;


    /* Variable: show_buffer_index
    *    Index number of "buffer_address" array
    *
    * Description:
    *    Buffer number being displayed or used as source image (buffer_address array number)
    *
    *    Case of "graphics_config_t::is_frame_pipeline = true":
    *        It is also same as a buffer number that is a target of RGA API.
    *        But the target of writing by the inside of API is DL (display list)
    *        reading GPU and DL is not the memory area pointed by "buffer_address".
    */
    int_fast32_t  show_buffer_index;


    /* Variable: draw_buffer_index
    *    Index number of "buffer_address" array
    *
    * Description:
    *    Case of "graphics_config_t::is_frame_pipeline = false":
    *        A drawing buffer number
    *
    *    Case of "graphics_config_t::is_frame_pipeline = true":
    *        A target buffer number that drawing by hardware rendering
    */
    int_fast32_t  draw_buffer_index;


    /* Variable: width
    *    Frame buffer width (pixels)
    *
    * Description:
    *    See <Figure_RGA_MaxFrameSize>.
    */
    int_fast32_t  width;


    /* Variable: byte_per_pixel
    *    Number of bytes per pixel
    *
    * Description:
    *    See <Table_RGA_SupportedFrameBufferFormat>.
    */
    byte_per_pixel_t  byte_per_pixel;


    /* Variable: stride
    *    Number of bytes of pixels having the same x coordinate in the previously below line
    *
    * Description:
    *    This value must be a multiple of 32.
    */
    int_fast32_t  stride;


    /* Variable: height
    *    Frame buffer height (pixels)
    *
    * Description:
    *    See <Figure_RGA_MaxFrameSize>.
    */
    int_fast32_t  height;


    /* Variable: pixel_format
    *    Pixel format
    *
    * Description:
    *    See <Table_RGA_SupportedFrameBufferFormat>.
    */
    pixel_format_t  pixel_format;


    /* Variable: delegate
    *    User-defined variable. RGA does not access it.
    */
    frame_buffer_delegate_t*  delegate; 

} frame_buffer_t; 


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* In "frame_buffer.h" */

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif  /* FRAME_BUFFER_TYPEDEF_H */
