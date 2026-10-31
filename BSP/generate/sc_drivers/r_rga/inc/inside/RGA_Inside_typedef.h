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
* File Name : rga_inside_typedef.h
* Version   : 1.12
******************************************************************************/

#include  "rz_co_typedef.h"
#include  "frame_buffer_typedef.h"

#ifndef  RGA_INSIDE_TYPEDEF_H
#define  RGA_INSIDE_TYPEDEF_H

#ifdef  __cplusplus
extern "C" {
#endif

/*=================================================================*/
/* <<< [CRT/RZ_A2M/CRT.h] >>> */
/*=================================================================*/

/***********************************************************************
* Define: RGA_INSIDE_RELEASE
*    1 or not define.
************************************************************************/
#define  RGA_INSIDE_RELEASE


/***********************************************************************
  <<< [static_release] qualifier >>> 
************************************************************************/
#ifndef R_OSPL_NDEBUG
    #define  static_release  static
#else
    #define  static_release 
#endif

/***********************************************************************
  <<< [CHECK_ARG] macro >>> 
************************************************************************/
#ifndef  CHECK_ARG
    #define  CHECK_ARG  (1)
#endif


/**************************************************************************
  <<< [TransParentDirEnum] >>> 
***************************************************************************/
enum _TransParentDirEnum {  
    TRANSPARENT_DIR_IS_00 = 0,
    TRANSPARENT_DIR_IS_FF = 0xFF
};

typedef enum 
    _TransParentDirEnum  TransParentDirEnum;

/**************************************************************************
  <<< [Color32] >>> 
***************************************************************************/
typedef  unsigned int  Color32;  

/**************************************************************************
  <<< [Color8] >>> 
***************************************************************************/
typedef  unsigned char  Color8; 

#define  SOFT_BIT_BLT_CALC_ALPHA  ( (unsigned int) 0xFFFFFFFF )  /*[SOFT_BIT_BLT_CALC_ALPHA]*/

/***********************************************************************
  <<< [SoftBitBltFrameBufferClass] type >>> 
************************************************************************/
typedef struct _SoftBitBltFrameBufferClass  SoftBitBltFrameBufferClass; 
struct _SoftBitBltFrameBufferClass { 
    uint8_t*  Address;  /* Address of top left (top down) or bottom left (bottom up) */  
    int       width; 
    int       height;
    int       stride;  /* Byte to next upper line, minus(top down) or plus(bottom up) */ 
};

/***********************************************************************
  <<< [SoftBitBltRectangleClass] type >>> 
************************************************************************/
typedef struct _SoftBitBltRectangleClass  SoftBitBltRectangleClass; 
struct _SoftBitBltRectangleClass { 
    int  Left;  
    int  Top;  

    /* include right  bound pixel */
    int  Right;     

    /* include bottom bound pixel */
    int  Bottom;    

    /* top < bottom (top down), bottom < top (bottom up) */
};

/***********************************************************************
  <<< (SoftBitBlt) >>> 
************************************************************************/

/***********************************************************************
* Macro: IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
************************************************************************/
/* #define  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE */ /* Old specification 120709. This will be delete. */


/***********************************************************************
* Constant: E_Culling
************************************************************************/
enum { E_Culling  = 3182 }; 


/***********************************************************************
* Enumeration: SoftBitBltEnum
*    SoftFillRectangleBlendEnum
*
*    : SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS - 1
************************************************************************/
typedef enum {
    SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS = 1
} SoftBitBltEnum;  


/***********************************************************************
* Type: SoftBitBltFlags
************************************************************************/
typedef bit_flags_fast32_t /*<SoftBitBltEnum>*/  SoftBitBltFlags; 


/***********************************************************************
* Enumeration: SoftFillRectangleBlendEnum
*    SoftFillRectangleBlendEnum
*
*    : SOFT_FILL_RECTANGLE_BLEND_DESTINATION_OUT - 7
************************************************************************/
typedef enum {
    SOFT_FILL_RECTANGLE_BLEND_DESTINATION_OUT = 7
} SoftFillRectangleBlendEnum; 


/***********************************************************************
* Type: SoftFillRectangleBlendFlags
************************************************************************/

/*<SoftFillRectangleBlendEnum>*/
typedef bit_flags_fast32_t  SoftFillRectangleBlendFlags; 


/***********************************************************************
* Structure: UIntPtrStartOverType
************************************************************************/
typedef struct _UIntPtrStartOverType  UIntPtrStartOverType; 
struct _UIntPtrStartOverType {  
    uintptr_t  Start; 
    uintptr_t  Over; 
};


/***********************************************************************
* Structure: SoftBitBlt_ConfigClass
************************************************************************/
typedef struct _SoftBitBlt_ConfigClass  SoftBitBlt_ConfigClass; 
struct _SoftBitBlt_ConfigClass { 
    bit_flags_fast32_t  Flags;                      /* F_SOFT_BITBLT_LayerAlpha | ... */ 
    int_fast32_t      LayerAlphaValue;              /* Flags|= F_SOFT_BITBLT_LayerAlphaValue, if enabled */ 
    SoftBitBltFrameBufferClass*  AlphaFrameBuffer;  /* Flags|= F_SOFT_BITBLT_AlphaFrameBuffer, if enabled */  
    SoftBitBltRectangleClass*    AlphaRect;         /* Flags|= F_SOFT_BITBLT_AlphaRect, if enabled */  
    byte_per_pixel_t  AlphaFrameBuffer_BytePerPixel;/* Flags|= F_SOFT_BITBLT_AlphaFrameBuffer_BytePerPixel, if enabled */ 
    int_fast32_t      DestinationAlphaValue;        /* Flags|= F_SOFT_BITBLT_DestinationAlphaValue, if enabled */ 
    pixel_format_t    SourcePixelFormat;            /* Flags|= F_SOFT_BITBLT_SourcePixelFormat, if enabled */ 
    byte_per_pixel_t  SourceBytePerPixel;           /* Flags|= F_SOFT_BITBLT_SourceBytePerPixel, if enabled */ 
    pixel_format_t    DestinationPixelFormat;       /* Flags|= F_SOFT_BITBLT_DestinationPixelFormat, if enabled */ 
    byte_per_pixel_t  DestinationBytePerPixel;      /* Flags|= F_SOFT_BITBLT_DestinationBytePerPixel, if enabled */ 
};
enum { 
    F_SOFT_BITBLT_LayerAlphaValue               = 0x0001,
    F_SOFT_BITBLT_AlphaFrameBuffer              = 0x0010,
    F_SOFT_BITBLT_AlphaRect                     = 0x0020,
    F_SOFT_BITBLT_AlphaFrameBuffer_BytePerPixel = 0x0040,
    F_SOFT_BITBLT_DestinationAlphaValue         = 0x0080,
    F_SOFT_BITBLT_SourcePixelFormat             = 0x0100,
    F_SOFT_BITBLT_SourceBytePerPixel            = 0x0200,
    F_SOFT_BITBLT_DestinationPixelFormat        = 0x0400,
    F_SOFT_BITBLT_DestinationBytePerPixel       = 0x0800,

    SOFT_BITBLT_SOURCE_IS_PREMULTIPLIED_ALPHA      = 0x1000,
    SOFT_BITBLT_DESTINATION_IS_PREMULTIPLED_ALPHA = 0x2000,  /* need */

    /* SOFT_BITBLT_BILINEAR_FILTERING */ /* not supported */
};


/***********************************************************************
* Structure: decode_image_parameters_t
************************************************************************/
typedef struct st_decode_image_parameters_t  decode_image_parameters_t;
struct st_decode_image_parameters_t { 
    int_fast32_t  MinX; 
    int_fast32_t  MinY; 
    IntBoxType*
        ClippingRectangle; 
};


/***********************************************************************
* Constants: graphics_JCU_option_t
************************************************************************/
typedef enum {
    F_GRAPHICS_JCU_ENABLE_JCU_ONLY = 1,
    F_GRAPHICS_JCU_FOR_INT_TRANSLATE = 2
} graphics_JCU_option_t; 


/***********************************************************************
* Type: graphics_JCU_options_t
************************************************************************/
typedef bit_flags_fast32_t /*<graphics_JCU_option_t>*/  graphics_JCU_options_t; 


/***********************************************************************
* Constants: graphics_PNG_option_t
************************************************************************/
typedef enum {
    F_GRAPHICS_PNG_ENABLE_PNG_ONLY = 1,
    F_GRAPHICS_PNG_FOR_INT_TRANSLATE = 2,
    F_GRAPHICS_PNG_FORCE_PNG_ONLY = 4
} graphics_PNG_option_t; 


/***********************************************************************
* Type: graphics_PNG_options_t
************************************************************************/
typedef bit_flags_fast32_t /*<graphics_PNG_option_t>*/  graphics_PNG_options_t; 


#ifdef  __cplusplus
}  /* extern "C" */
#endif

#endif  /* RGA_INSIDE_TYPEDEF_H */
