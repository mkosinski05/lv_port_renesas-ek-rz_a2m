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
* File Name    : frame_buffer_format_typedef.h
* Description  : Frame Buffer Color Format
* Version      : 1.12
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "clib_drivers_config.h"
#include  "r_typedefs.h"

#ifndef  FRAME_BUFFER_FORMAT_TYPEDEF_H
#define  FRAME_BUFFER_FORMAT_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Structure: argb8888_t
*    One of color type
************************************************************************/
#define        argb8888_t  argb8888_t  
typedef union _ARGB8888Type  argb8888_t; 
union _ARGB8888Type {  
    uint32_t  Value;  /* CPU Endian. Not frame buffer endian */ 
    struct {
        #ifndef  BYTE_ENDIAN
            #error
        #endif
        #if BYTE_ENDIAN == BYTE_LITTLE_ENDIAN

            /* Variable: u.Blue
                Blue is Value & 0x000000FF */
            uint8_t  Blue; 

            /* Variable: u.Green */
            uint8_t  Green; 

            /* Variable: u.Red */
            uint8_t  Red; 

            /* Variable: u.Alpha */
            uint8_t  Alpha; 
        #else
            uint8_t  Alpha;
            uint8_t  Red;
            uint8_t  Green;
            uint8_t  Blue;   /* Blue is Value & 0x000000FF */
        #endif
    } u;
};


/***********************************************************************
* Structure: xrgb8888_t
*    One of color type
************************************************************************/
#define        xrgb8888_t  xrgb8888_t  
typedef union _XRGB8888Type  xrgb8888_t;
union _XRGB8888Type { 
    uint32_t  Value;  /* CPU Endian. Not frame buffer endian */ 
    struct {
        #if BYTE_ENDIAN == BYTE_LITTLE_ENDIAN

            /* Variable: u.Blue
                Blue is Value & 0x000000FF */
            uint8_t  Blue; 

            /* Variable: u.Green */
            uint8_t  Green; 

            /* Variable: u.Red */
            uint8_t  Red; 

            /* Variable: u.X */
            uint8_t  X;      /* zero */ 
        #else
            uint8_t  X;      /* zero */
            uint8_t  Red;
            uint8_t  Green;
            uint8_t  Blue;   /* Blue is Value & 0x000000FF */
        #endif
    } u;
};


/***********************************************************************
* Structure: rgb565_t
*    One of color type
************************************************************************/
#define        rgb565_t  rgb565_t  
typedef union _RGB565Type  rgb565_t;
union _RGB565Type { 
    uint16_t  Value;  /* CPU Endian. Not frame buffer endian */ 
    struct {
        #if  BIT_FIELD_ENDIAN == BIT_FIELD_LITTLE_ENDIAN

            /* Variable: u.Blue
                Blue is at LSB */
            unsigned  Blue  : 5;  

            /* Variable: u.Green */
            unsigned  Green : 6;  

            /* Variable: u.Red */
            unsigned  Red   : 5;  
        #else
            unsigned  Red   : 5;
            unsigned  Green : 6;
            unsigned  Blue  : 5;  /* Blue is at LSB */
        #endif
    } u;
};


/***********************************************************************
* Structure: argb1555_t
*    One of color type
************************************************************************/
#define        argb1555_t  argb1555_t  
typedef union _ARGB1555Type  argb1555_t;
union _ARGB1555Type { 
    uint16_t  Value;  /* CPU Endian. Not frame buffer endian */ 
    struct {
        #if  BIT_FIELD_ENDIAN == BIT_FIELD_LITTLE_ENDIAN

            /* Variable: u.Blue
                Blue is at LSB */
            unsigned  Blue  : 5;  

            /* Variable: u.Green */
            unsigned  Green : 5;  

            /* Variable: u.Red */
            unsigned  Red   : 5;  

            /* Variable: u.Alpha */
            unsigned  Alpha : 1;  
        #else
            unsigned  Alpha : 1;
            unsigned  Red   : 5;
            unsigned  Green : 5;
            unsigned  Blue  : 5;  /* Blue is at LSB */
        #endif
    } u;
};


/***********************************************************************
* Structure: argb4444_t
*    One of color type
************************************************************************/
#define        argb4444_t  argb4444_t  
typedef union _ARGB4444Type  argb4444_t;
union _ARGB4444Type { 
    uint16_t  Value;  /* CPU Endian. Not frame buffer endian */ 
    struct {
        #if  BIT_FIELD_ENDIAN == BIT_FIELD_LITTLE_ENDIAN

            /* Variable: u.Blue
                Blue is at LSB */
            unsigned  Blue  : 4;  

            /* Variable: u.Green */
            unsigned  Green : 4;  

            /* Variable: u.Red */
            unsigned  Red   : 4;  

            /* Variable: u.Alpha */
            unsigned  Alpha : 4;  
        #else
            unsigned  Alpha : 4;
            unsigned  Red   : 4;
            unsigned  Green : 4;
            unsigned  Blue  : 4;  /* Blue is at LSB */
        #endif
    } u;
};


/***********************************************************************
* Structure: rgba8888_t
*    One of color type
************************************************************************/
#define        rgba8888_t  rgba8888_t  
typedef union _RGBA8888Type  rgba8888_t;
union _RGBA8888Type { 
    uint32_t  Value;  /* CPU Endian. Not frame buffer endian */ 
    struct {
        #if  BIT_FIELD_ENDIAN == BIT_FIELD_LITTLE_ENDIAN

            /* Variable: u.Alpha
                Alpha is at LSB */
            unsigned  Alpha : 8;  

            /* Variable: u.Blue */
            unsigned  Blue  : 8;  

            /* Variable: u.Green */
            unsigned  Green : 8;  

            /* Variable: u.Red */
            unsigned  Red   : 8;  
        #else
            unsigned  Red   : 8;
            unsigned  Green : 8;
            unsigned  Blue  : 8;
            unsigned  Alpha : 8;  /* Alpha is at LSB */
        #endif
    } u;
};


/***********************************************************************
* Structure: y_cb_cr_422_t
*    Same as <yuv422_t>
************************************************************************/
/***********************************************************************
* Structure: yuv422_t
*    One of color type
************************************************************************/
#define          y_cb_cr_422_t  y_cb_cr_422_t  
typedef union st_y_cb_cr_422_t  y_cb_cr_422_t;
#define          yuv422_t       yuv422_t  
typedef union st_y_cb_cr_422_t  yuv422_t;
union st_y_cb_cr_422_t { 
    struct {
        /* Variable: u.Cb_01
            U */
        uint8_t  Cb_01; 

        /* Variable: u.Y_0 */
        uint8_t  Y_0; 

        /* Variable: u.Cr_01
            V */
        uint8_t  Cr_01; 

        /* Variable: u.Y_1 */
        uint8_t  Y_1; 

    } u;  /* 1st member "u" can be used by initializer */

    struct {
        /* Variable: uu.U_01 */
        uint8_t  U_01; 

        /* Variable: uu.Y_0 */
        uint8_t  Y_0; 

        /* Variable: uu.V_01 */
        uint8_t  V_01; 

        /* Variable: uu.Y_1 */
        uint8_t  Y_1; 
    } uu;

    uint32_t  Value; 
};


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* In "frame_buffer_format.h" */

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* FRAME_BUFFER_FORMAT_TYPEDEF_H */
