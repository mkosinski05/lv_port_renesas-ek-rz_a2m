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
* File Name   : rga_inside.h
* Description : Graphics Library
* Version     : 1.12
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "rga_inside_typedef.h"
#include  "frame_buffer_typedef.h"
#include  "rga_api_typedef.h"
#include  "rga_raw_image_typedef.h"
#include  "rga_errnum.h"
#include  "frame_buffer_format_typedef.h"
#include  "rz_co_typedef.h"
#include  "r_ospl_old_macro.h"
#include  "clib_drivers_config.h"
#include  "clib_drivers_typedef.h"

#ifndef  RGA_INSIDE_H
#define  RGA_INSIDE_H

#ifdef  __cplusplus
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/
/* in "rga_inside_typedef.h" */

/***********************************************************************
* Macro: DEBUG_CODE
************************************************************************/
#ifndef R_OSPL_NDEBUG
    #define  DEBUG_CODE( Statement ) \
        do { Statement; } while(0)  /* do-while is CERT standard PRE10-C */ 
#else
    #define  DEBUG_CODE( Statement )  /* no execute */
#endif


/***********************************************************************
* Macro: RGA_INTERNAL_INLINE
************************************************************************/
#ifdef  RGA_MAKE_INTERNAL_INLINE_BODY
    #define  RGA_INTERNAL_INLINE  /* not inline */
#else
    #define  RGA_INTERNAL_INLINE  STATIC_INLINE 
#endif


/******************************************************************************
Typedef definitions
******************************************************************************/
/* in "rga_inside_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* in "rga_inside_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/

/* Group: C99Type */

/***********************************************************************
* Function Name: R_int32_t_to_int16_t
************************************************************************/
errnum_t  R_int32_t_to_int16_t( int32_t input, int16_t* output ); 


/***********************************************************************
* Function Name: R_int32_t_to_uint8_t
************************************************************************/
errnum_t  R_int32_t_to_uint8_t( int32_t input, uint8_t* output );


/***********************************************************************
* Function Name: R_int32_t_to_uint16_t
************************************************************************/
errnum_t  R_int32_t_to_uint16_t( int32_t input, uint16_t* output );


/***********************************************************************
* Function Name: Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum
************************************************************************/
LIB_STATIC errnum_t  Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum( uint8_t* out, int in );


/***********************************************************************
* Function Name: GraphicsImageClass_isRawFormat
************************************************************************/
bool_t  GraphicsImageClass_isRawFormat( const graphics_image_t* self );


/***********************************************************************
* Function Name: memset32
************************************************************************/
void  memset32( void* ArrayFirst, unsigned Value, size_t ArrayByteSize ); 


/***********************************************************************
* Function Name: minus_mod_int_equal_plus
************************************************************************/
int  minus_mod_int_equal_plus( int LeftValue, int RightValue ); 


/***********************************************************************
* Function Name: UIntPtrStartOverType_isOverlap
************************************************************************/
errnum_t  UIntPtrStartOverType_isOverlap(
    UIntPtrStartOverType* A,  UIntPtrStartOverType* B,  bool_t* out_IsOverlap );


/***********************************************************************
* Function Name: R_RGA_ChangePixelFormat
************************************************************************/
errnum_t  R_RGA_ChangePixelFormat 
    ( uint32_t  in_InputColor,  pixel_format_t  in_InputFormat,
    uint32_t*  out_Color,  pixel_format_t  in_OutputFormat );


/***********************************************************************
* Function Name: ARGB8888Type_to_YUV422Type
************************************************************************/
yuv422_t  ARGB8888Type_to_YUV422Type( argb8888_t Colors[2] );


/***********************************************************************
* Function Name: YUV422Type_to_ARGB8888Type
************************************************************************/
void  YUV422Type_to_ARGB8888Type
    ( yuv422_t SourceColors, argb8888_t out_DestinationColors[2] );


/***********************************************************************
* Function Name: mod_2n
************************************************************************/
RGA_INTERNAL_INLINE int  mod_2n( int Value, int Divide ) 
{
    ASSERT_D( R_OSPL_IsSetBitsCount1( (uint32_t) Divide ), R_NOOP() );
    return  Value & ( Divide - 1 );
}/* End of function mod_2n */


/***********************************************************************
* Function Name: ceil_2n
************************************************************************/
STATIC_INLINE int  ceil_2n( int Value, int Align )
{
    ASSERT_D( R_OSPL_IsSetBitsCount1( (uint32_t) Align ), R_NOOP() );
    Align -= 1;
    return  ( Value + Align ) & (~Align);
}/* End of function ceil_2n */


/***********************************************************************
* Macro: shift_left
*    Safe shift.
************************************************************************/
#define  shift_left(  x, shift )  ( (x) << (shift) ) 


/***********************************************************************
* Macro: shift_right
*    Safe shift.
************************************************************************/
#define  shift_right( x, shift )  ( (unsigned int)(x) >> (shift) ) 


/***********************************************************************
* Function Name: SoftBitBlt_multiplyAlpha8
************************************************************************/
unsigned int  SoftBitBlt_multiplyAlpha8 
    ( unsigned int Alpha1, uint8_t Alpha2 );


/***********************************************************************
* Function Name: SoftBitBlt_multiplyARGB8888_AndAlpha8
************************************************************************/
argb8888_t  SoftBitBlt_multiplyARGB8888_AndAlpha8( argb8888_t Color, unsigned int Alpha,
    unsigned int DestinationAlphaOverwrite );


/***********************************************************************
* Function Name: SoftBitBlt_blendARGB8888_PremultipliedAlpha
************************************************************************/
argb8888_t  SoftBitBlt_blendARGB8888_PremultipliedAlpha( argb8888_t Source, argb8888_t Destination,
    unsigned int SourceAlpha,  unsigned int DestinationAlphaOverwrite );


/***********************************************************************
* Function Name: SoftBitBlt_multiplyARGB4444_AndAlpha4
************************************************************************/
argb4444_t  SoftBitBlt_multiplyARGB4444_AndAlpha4( argb4444_t Color, unsigned int Alpha,
    unsigned int DestinationAlphaOverwrite );


/***********************************************************************
* Function Name: SoftBitBlt_blendARGB4444_PremultipliedAlpha
************************************************************************/
argb4444_t  SoftBitBlt_blendARGB4444_PremultipliedAlpha( argb4444_t Source, argb4444_t Destination,
    unsigned int SourceAlpha,  unsigned int DestinationAlphaOverwrite );


/***********************************************************************
* Function Name: SoftBitBltFrameBufferClass_getMemorySize
************************************************************************/
RGA_INTERNAL_INLINE errnum_t
SoftBitBltFrameBufferClass_getMemorySize  
    ( SoftBitBltFrameBufferClass* self, size_t*  out_Size )

{
    if ( self->stride > 0 )
    {
        /* Cast to an appropriate type */
        *out_Size = (size_t)( self->stride * self->height );
    }
    else
    {
        ASSERT_R( self->stride != 0, return E_RGA_OTHER_DB );

        /* Cast to an appropriate type */
        *out_Size = (size_t)( (- self->stride) * self->height );
    }
    return  0;
}/* End of function SoftBitBltFrameBufferClass_getMemorySize */


/* Group: SoftBitBltRectangle */

/***********************************************************************
* Function Name: SoftBitBlt_ARGB8888_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_ARGB8888_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t  Flags );


/***********************************************************************
* Function Name: SoftBitBlt_16BitColor_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_16BitColor_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    SoftBitBltFlags  Flags );


/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_ARGB8888_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_R8G8B8A8_to_ARGB8888_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_XRGB8888_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_R8G8B8A8_to_XRGB8888_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_RGB565_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_R8G8B8A8_to_RGB565_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_ARGB1555_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_R8G8B8A8_to_ARGB1555_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_ARGB4444_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_R8G8B8A8_to_ARGB4444_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_YUV422_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_R8G8B8A8_to_YUV422_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_ARGB8888_to_R8G8B8A8_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_ARGB8888_to_R8G8B8A8_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_XRGB8888_to_R8G8B8A8_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_XRGB8888_to_R8G8B8A8_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8_to_R8G8B8A8_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_R8G8B8_to_R8G8B8A8_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_RGB565_to_R8G8B8A8_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_RGB565_to_R8G8B8A8_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_ARGB1555_to_R8G8B8A8_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_ARGB1555_to_R8G8B8A8_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_ARGB4444_to_R8G8B8A8_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_ARGB4444_to_R8G8B8A8_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_YUV422_to_R8G8B8A8_NoBlend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_YUV422_to_R8G8B8A8_NoBlend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftBitBlt_Blend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_Blend_NoStretch(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    SoftBitBlt_ConfigClass*      in_out_Config );


/***********************************************************************
* Function Name: SoftBitBlt_toInFrame
************************************************************************/
errnum_t  SoftBitBlt_toInFrame(
    SoftBitBltFrameBufferClass* SourceFrameBuffer,
    SoftBitBltFrameBufferClass* DestinationFrameBuffer,
    SoftBitBltRectangleClass* SourceRect,
    SoftBitBltRectangleClass* DestinationRect );


/***********************************************************************
* Function Name: SoftBitBlt_toInClippedFrame
************************************************************************/
errnum_t  SoftBitBlt_toInClippedFrame(
    const SoftBitBltFrameBufferClass*  in_SourceFrameBuffer,
    const SoftBitBltFrameBufferClass*  in_DestinationFrameBuffer,
    SoftBitBltRectangleClass*      in_out_SourceRect,
    SoftBitBltRectangleClass*      in_out_DestinationRect,
    const SoftBitBltRectangleClass*    in_ClippingRect );


/***********************************************************************
* Function Name: SoftBitBlt_isOverlapFrameBuffer
************************************************************************/
errnum_t  SoftBitBlt_isOverlapFrameBuffer(
    SoftBitBltFrameBufferClass* SourceFrameBuffer,
    SoftBitBltFrameBufferClass* DestinationFrameBuffer,
    bool_t* out_IsOverlap );


/***********************************************************************
* Function Name: SoftBitBltFrameBufferClass_getStartOver
************************************************************************/
errnum_t  SoftBitBltFrameBufferClass_getStartOver(
    SoftBitBltFrameBufferClass* self,  UIntPtrStartOverType* out_StartOver );


/* Group: SoftFillRectangle */

/***********************************************************************
* Function Name: SoftFillRectangle_toInFrame
************************************************************************/
errnum_t  SoftFillRectangle_toInFrame(
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle );


/***********************************************************************
* Function Name: SoftFillRectangle_ARGB8888
************************************************************************/
errnum_t  SoftFillRectangle_ARGB8888(
    SoftBitBltFrameBufferClass*  FrameBuffer,
    SoftBitBltRectangleClass*    Rectangle,
    argb8888_t  ColorARGB8888,  bit_flags_fast32_t Flags );


/***********************************************************************
* Function Name: SoftFillRectangle_16BitColor
************************************************************************/
errnum_t  SoftFillRectangle_16BitColor(
    SoftBitBltFrameBufferClass*  FrameBuffer,
    SoftBitBltRectangleClass*    Rectangle,
    uint16_t  Color16,  bit_flags_fast32_t Flags );


/***********************************************************************
* Function Name: SoftFillRectangle_YUV422
************************************************************************/
errnum_t  SoftFillRectangle_YUV422(
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle,
    yuv422_t  ColorYUV422,  bit_flags_fast32_t flags );


/***********************************************************************
* Function Name: SoftFillRectangle_ARGB8888_BlendByAlpha
************************************************************************/
errnum_t  SoftFillRectangle_ARGB8888_BlendByAlpha(
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle,
    int_t  Alpha8,  SoftFillRectangleBlendFlags flags );


/* Group: IntBoxType */

/***********************************************************************
* Function Name: IntBoxType_and
************************************************************************/
errnum_t  IntBoxType_and( IntBoxType* out_Box,
    const IntBoxType* BoxA, const IntBoxType* BoxB );


/* Group: General functions */

/***********************************************************************
* Function Name: frame_buffer_class_clear_by8bit
************************************************************************/
errnum_t  frame_buffer_class_clear_by8bit( frame_buffer_t* self, uint8_t Color8bit );


/***********************************************************************
* Function Name: frame_buffer_class_swap_buffers
************************************************************************/
errnum_t  frame_buffer_class_swap_buffers( frame_buffer_t* self );


/***********************************************************************
* Function Name: frame_buffer_class_get_address_of_pixel
************************************************************************/
errnum_t  frame_buffer_class_get_address_of_pixel( frame_buffer_t* self, 
    int BufferIndex, int X, int Y, void** out_AddressOfPixel );


/***********************************************************************
* Function Name: frame_buffer_class_set_pixel_color
************************************************************************/
errnum_t  frame_buffer_class_set_pixel_color( frame_buffer_t* self,
    int BufferIndex, int X, int Y, uint32_t Color );


/***********************************************************************
* Function Name: R_RGA_InitializeJCU
*    new "GraphicsDecoderClass_initialize"
************************************************************************/
errnum_t  R_RGA_InitializeJCU(void); 


/***********************************************************************
* Function Name: R_RGA_FinalizeJCU
*    new "GraphicsDecoderClass_finalize"
************************************************************************/
errnum_t  R_RGA_FinalizeJCU(void); 


/***********************************************************************
* Function Name: R_RGA_DecodeWithJCU
*    new "GraphicsClass_drawImage_JCU_JPEG"
************************************************************************/
errnum_t  R_RGA_DecodeWithJCU 
    ( graphics_t* self,  const graphics_image_t*  in_Image,
    const decode_image_parameters_t*  in_Parameters,
    graphics_image_t*  out_ImageInBuffer,  graphics_JCU_options_t  in_Options );


/***********************************************************************
* Function Name: R_RGA_DecodeWithPNG
*    new "GraphicsClass_drawImage_PNG"
************************************************************************/
errnum_t  R_RGA_DecodeWithPNG 
    ( graphics_t* self,  const graphics_image_t*  in_Image,
    const decode_image_parameters_t*  in_Parameters,
    graphics_image_t*  out_ImageInBuffer,  graphics_PNG_options_t  in_Options );


/***********************************************************************
* Function Name: R_RGA_GetBytePerPixelFromPixelFormat
************************************************************************/
errnum_t  R_RGA_GetBytePerPixelFromPixelFormat( pixel_format_t  in_PixelFormat, 
    byte_per_pixel_t*  out_BytePerPixel );

#ifdef  __cplusplus
}  /* extern "C" */
#endif

#endif  /* RGA_INSIDE_H */
