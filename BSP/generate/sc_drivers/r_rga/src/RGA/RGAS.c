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
* File Name   : rgas.c
* Version     : 1.12
******************************************************************************/

#include  <string.h>
#include  "clib_drivers.h"
#include  "frame_buffer_inline.h"
#include  "rz_co.h"
#include  "r_ospl_old_macro.h"
#include  "rga_errnum.h"
#include  "rga_api.h"
#include  "rga_inside.h"
#include  "rga_local.h"
#include  "rga_port_inline.h"

/*=================================================================*/
/* <<< [frame_buffer/clib_n/frame_buffer.c] >>> */ 
/*=================================================================*/

/*-------------------------------------------------------------------------*/
/* <<<< ### (frame_buffer_t) >>>> */ 
/*-------------------------------------------------------------------------*/



/***********************************************************************
* Function Name: frame_buffer_class_clear_by8bit 
************************************************************************/
LIB_STATIC errnum_t  frame_buffer_class_clear_by8bit 
                        ( frame_buffer_t* self, uint8_t Color8bit )
{
    void*   addr = self->buffer_address[ self->draw_buffer_index ];
    size_t  size = self->stride * self->height;

    /* Cast to an appropriate type */
    memset( addr, (int) Color8bit, size );
    return  0;
} /* End of function frame_buffer_class_clear_by8bit */

/***********************************************************************
* Function Name: frame_buffer_class_get_address_of_pixel 
************************************************************************/
LIB_STATIC errnum_t  frame_buffer_class_get_address_of_pixel( frame_buffer_t* self,
    int BufferIndex, int X, int Y, void** out_AddressOfPixel )
{
    errnum_t  e;

    #if  CHECK_ARG

        /* Cast to an appropriate type */
        ASSERT_R( X >= 0  &&  X < (int) self->width, goto err ); 

        /* Cast to an appropriate type */
        ASSERT_R( Y >= 0  &&  Y < (int) self->height, goto err ); 

        /* Cast to an appropriate type */
        ASSERT_R  
        ( BufferIndex >= 0  &&  BufferIndex < (int) self->buffer_count, goto err );
    #endif

    /* Cast to an appropriate type */
    *out_AddressOfPixel = (int8_t*) self->buffer_address[ BufferIndex ] +
        (Y * self->stride) + (X * self->byte_per_pixel);
    return  0;

err:
    {
        e = (0x01730001);
    } /* Error: <Private/> */
    R_ERROR_Set( e );
    return  e;
} /* End of function frame_buffer_class_get_address_of_pixel */

/***********************************************************************
* Function Name: frame_buffer_class_set_pixel_color 
************************************************************************/
LIB_STATIC errnum_t
    frame_buffer_class_set_pixel_color 
    ( frame_buffer_t* self,
    int BufferIndex, int X, int Y, uint32_t Color )
{
    errnum_t  e;
    void*     address;

    e= frame_buffer_class_get_address_of_pixel( self, BufferIndex, X, Y, &address );
    IF(e)
    {
        goto fin;
    }

    switch ( self->byte_per_pixel )
    {
        /* Cast to an appropriate type */
        case  1:
        /* Cast to an appropriate type */
            *(uint8_t*)  address = (uint8_t)  Color;  break;

        /* Cast to an appropriate type */
        case  2:  *(uint16_t*) address = (uint16_t) Color;  break;

        /* Cast to an appropriate type */
        case  4:  *(uint32_t*) address = (uint32_t) Color;  break;

        default:
            ASSERT_R 
            ( self->byte_per_pixel == 3,  goto err );

            /* Cast to an appropriate type */
            *( (uint8_t*) address     ) = (uint8_t) ( Color & 0x000000FF );

            /* Cast to an appropriate type */
            *( (uint8_t*) address + 1 ) = (uint8_t)(( Color & 0x0000FF00 ) >> 8 );

            /* Cast to an appropriate type */
            *( (uint8_t*) address + 2 ) = (uint8_t)(( Color & 0x00FF0000 ) >> 16 );
            break;
    }

    e=0;
fin:
    {
        return e;
    }

err:
    {
        e = E_RGA_OTHER_2;  goto fin;  /* Error: <Private/> */
    }
} /* End of function frame_buffer_class_set_pixel_color */

/***********************************************************************
* Function Name: SoftFillRectangle_checkRectangle 
************************************************************************/
LIB_STATIC errnum_t  
    SoftFillRectangle_checkRectangle(  
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle,
    int* out_Height )
{
    ASSERT_R( Rectangle->Left <= Rectangle->Right, goto err ); 

    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        if ( frame_buffer->stride < 0 ) {
            ASSERT_R( Rectangle->Top <= Rectangle->Bottom, goto err );
            *out_Height = Rectangle->Bottom - Rectangle->Top;
        }
        else {
            ASSERT_R( frame_buffer->stride != 0, goto err );

            ASSERT_R( Rectangle->Bottom <= Rectangle->Top, goto err );
            *out_Height = Rectangle->Top - Rectangle->Bottom;
        }
    #else
        if ( frame_buffer->stride > 0 )
        {
            ASSERT_R( Rectangle->Top <= Rectangle->Bottom, goto err ); 
            *out_Height = Rectangle->Bottom - Rectangle->Top;
        }
        else
        {
            ASSERT_R( frame_buffer->stride != 0, goto err );  

            ASSERT_R( Rectangle->Bottom <= Rectangle->Top, goto err ); 
            *out_Height = Rectangle->Top - Rectangle->Bottom;
        }
    #endif

    return  0;

err:
    {
        return (0x01730003);
    } /* Error: <Private/> */
} /* End of function SoftFillRectangle_checkRectangle */

/***********************************************************************
* Function Name: SoftBitBlt_checkRectangle 
************************************************************************/
LIB_STATIC errnum_t
    SoftBitBlt_checkRectangle( 
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    int* out_SourceHeight,  int* out_DestinationHeight )
{
    errnum_t  e;

    e= SoftFillRectangle_checkRectangle( SourceFrameBuffer, SourceRect, out_SourceHeight );
    IF(e)
    {
        goto fin;
    }

    e= SoftFillRectangle_checkRectangle( DestinationFrameBuffer, DestinationRect, out_DestinationHeight );
    IF(e)
    {
        goto fin;
    }

    e=0;
fin:
    {
        return e;
    }
} /* End of function SoftBitBlt_checkRectangle */

/***********************************************************************
* Function Name: SoftBitBlt_checkTypeA 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_checkTypeA(     
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect )
{
    errnum_t  e;
    bool_t  is_overlap;
    int   src_height; 
    int   dst_height; 

    ASSERT_R( SourceRect->Left >= 0, goto err ); 
    ASSERT_R( SourceRect->Top  >= 0, goto err );  
    ASSERT_R( SourceRect->Right  < SourceFrameBuffer->width,  goto err ); 
    ASSERT_R( SourceRect->Bottom < SourceFrameBuffer->height, goto err ); 

    e= SoftBitBlt_checkRectangle( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
        &src_height, &dst_height );
    IF(e)
    {
        goto fin;
    }

    ASSERT_R
    ( SourceRect->Right - SourceRect->Left == DestinationRect->Right - DestinationRect->Left, goto err );
    ASSERT_R( src_height == dst_height, goto err );  

    e= SoftBitBlt_isOverlapFrameBuffer( SourceFrameBuffer, DestinationFrameBuffer, &is_overlap );
    IF(e)
    {
        goto fin;
    }
    ASSERT_R( ! is_overlap, goto err ); 

    e=0;
fin:
    {
        return e;
    }

err:
    {
        e = E_RGA_OTHER_4;
    }

    goto fin;  /* Error: <Private/> */
} /* End of function SoftBitBlt_checkTypeA */

/***********************************************************************
* Function Name: SoftBitBlt_checkTypeB 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_checkTypeB(     
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect )
{
    errnum_t  e;
    bool_t  is_same_frame;
    bool_t  is_overlap;
    int   src_height; 
    int   dst_height; 

    ASSERT_R( SourceRect->Left >= 0, goto err ); 
    ASSERT_R( SourceRect->Top  >= 0, goto err ); 
    ASSERT_R( SourceRect->Right  < SourceFrameBuffer->width,  goto err ); 
    ASSERT_R( SourceRect->Bottom < SourceFrameBuffer->height, goto err ); 

    e= SoftBitBlt_checkRectangle( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
        &src_height, &dst_height );
    IF(e)
    {
        goto fin;
    }

    ASSERT_R
    ( SourceRect->Right - SourceRect->Left == DestinationRect->Right - DestinationRect->Left,
        goto err );
    ASSERT_R( src_height == dst_height, goto err ); 

    is_same_frame = (
        (SourceFrameBuffer->Address == DestinationFrameBuffer->Address)  &&
        (SourceFrameBuffer->stride  == DestinationFrameBuffer->stride) );

    if ( ! is_same_frame )
    {
        e= SoftBitBlt_isOverlapFrameBuffer( SourceFrameBuffer, DestinationFrameBuffer, &is_overlap );
        IF(e)
        {
            goto fin;
        }
        ASSERT_R( ! is_overlap, goto err ); 
    }

    e=0;
fin:
    {
        return e;
    }

err:
    {
        e = E_RGA_OTHER_5;
        goto fin;  /* Error: <Private/> */
    }
} /* End of function SoftBitBlt_checkTypeB */

/***********************************************************************
* Function Name: SoftBitBltSub_getLineHeads 
************************************************************************/
#ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
LIB_STATIC void  SoftBitBltSub_getLineHeads(
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    uint8_t**  out_SourceLineHead,
    uint8_t**  out_SourceLineHeadTop,
    uint8_t**  out_DestinationLineHead )
#else
LIB_STATIC void 
    SoftBitBltSub_getLineHeads( 
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    uint8_t**  out_SourceLineHead,
    uint8_t**  out_SourceLineHeadBottom,
    uint8_t**  out_DestinationLineHead )
#endif
{
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        if ( SourceFrameBuffer->stride < 0 ) {
            *out_SourceLineHead = SourceFrameBuffer->Address -
                SourceFrameBuffer->stride * SourceRect->Bottom;
            *out_SourceLineHeadTop = SourceFrameBuffer->Address -
                SourceFrameBuffer->stride * SourceRect->Top;
        }
        else {
            *out_SourceLineHead = SourceFrameBuffer->Address +
                SourceFrameBuffer->stride * SourceRect->Bottom;
            *out_SourceLineHeadTop = SourceFrameBuffer->Address +
                SourceFrameBuffer->stride * SourceRect->Top;
        }

        if ( DestinationFrameBuffer->stride < 0 ) {
            *out_DestinationLineHead = DestinationFrameBuffer->Address -
                DestinationFrameBuffer->stride * DestinationRect->Bottom;
        }
        else {
            *out_DestinationLineHead = DestinationFrameBuffer->Address +
                DestinationFrameBuffer->stride * DestinationRect->Bottom;
        }
    #else
        if ( SourceFrameBuffer->stride > 0 )
        {
            *out_SourceLineHead = SourceFrameBuffer->Address +
                (SourceFrameBuffer->stride * SourceRect->Top);
            *out_SourceLineHeadBottom = SourceFrameBuffer->Address +
                (SourceFrameBuffer->stride * SourceRect->Bottom);
        }
        else
        {
            *out_SourceLineHead = SourceFrameBuffer->Address -
                (SourceFrameBuffer->stride * SourceRect->Top);
            *out_SourceLineHeadBottom = SourceFrameBuffer->Address -
                (SourceFrameBuffer->stride * SourceRect->Bottom);
        }

        if ( DestinationFrameBuffer->stride > 0 )
        {
            *out_DestinationLineHead = DestinationFrameBuffer->Address +
                (DestinationFrameBuffer->stride * DestinationRect->Top);
        }
        else
        {
            *out_DestinationLineHead = DestinationFrameBuffer->Address -
                (DestinationFrameBuffer->stride * DestinationRect->Top);
        }
    #endif
} /* End of function SoftBitBltSub_getLineHeads */

/***********************************************************************
* Function Name: SoftFillRectangleSub_getLineHeads 
************************************************************************/
#ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE

LIB_STATIC void  SoftFillRectangleSub_getLineHeads(
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle,
    uint8_t**  out_LineHead,
    uint8_t**  out_LineHeadTop )
{
    if ( frame_buffer->stride < 0 ) {
        *out_LineHead = frame_buffer->Address - frame_buffer->stride * Rectangle->Bottom;
        *out_LineHeadTop = frame_buffer->Address - frame_buffer->stride * Rectangle->Top;
    }
    else {
        *out_LineHead = frame_buffer->Address + frame_buffer->stride * Rectangle->Bottom;
        *out_LineHeadTop = frame_buffer->Address + frame_buffer->stride * Rectangle->Top;
    }
}

#else

LIB_STATIC void  
    SoftFillRectangleSub_getLineHeads( 
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle,
    uint8_t**  out_LineHead,
    uint8_t**  out_LineHeadBottom )
{
    if ( frame_buffer->stride > 0 )
    {
        *out_LineHead = frame_buffer->Address + (frame_buffer->stride * Rectangle->Top);
        *out_LineHeadBottom = frame_buffer->Address + (frame_buffer->stride * Rectangle->Bottom);
    }
    else
    {
        *out_LineHead = frame_buffer->Address - (frame_buffer->stride * Rectangle->Top);
        *out_LineHeadBottom = frame_buffer->Address - (frame_buffer->stride * Rectangle->Bottom);
    }
} /* End of function SoftFillRectangleSub_getLineHeads */

#endif

/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_ARGB8888_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_R8G8B8A8_to_ARGB8888_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            r8g8b8a8_t*  src_color;  /* source color */ 
            argb8888_t*  dst_color;  /* destination color */ 

            /* Cast to an appropriate type */
            src_color = (r8g8b8a8_t*) src;

            /* Cast to an appropriate type */
            dst_color = (argb8888_t*) dst;

            dst_color->u.Red   = src_color->u.Red;
            dst_color->u.Green = src_color->u.Green;
            dst_color->u.Blue  = src_color->u.Blue;
            dst_color->u.Alpha = src_color->u.Alpha;

            src += ( src_bpp / 8 );
            dst += ( dst_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )  { break; }
        #else
        if ( src_line_left == src_line_left_bottom )
        {
            break;
        }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return e;
    }
} /* End of function SoftBitBlt_R8G8B8A8_to_ARGB8888_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_XRGB8888_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_R8G8B8A8_to_XRGB8888_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            r8g8b8a8_t*  src_color;  /* source color */ 
            xrgb8888_t*  dst_color;  /* destination color */ 

            src_color = (r8g8b8a8_t*) src; 
            dst_color = (xrgb8888_t*) dst; 

            dst_color->u.Red   = src_color->u.Red;
            dst_color->u.Green = src_color->u.Green;
            dst_color->u.Blue  = src_color->u.Blue;
            dst_color->u.X     = 0;

            src += ( src_bpp / 8 );
            dst += ( dst_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )  { break; }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return e;
    }
} /* End of function SoftBitBlt_R8G8B8A8_to_XRGB8888_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_ARGB8888_to_R8G8B8A8_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_ARGB8888_to_R8G8B8A8_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            argb8888_t  color;

            /* Cast to an appropriate type */
            color.Value = *(uint32_t*) src;

            *dst = color.u.Red;
            dst += 1;
            *dst = color.u.Green;
            dst += 1;
            *dst = color.u.Blue;
            dst += 1;
            *dst = color.u.Alpha;
            dst += 1;

            src += ( src_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )  { break; }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return e;
    }
} /* End of function SoftBitBlt_ARGB8888_to_R8G8B8A8_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_XRGB8888_to_R8G8B8A8_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_XRGB8888_to_R8G8B8A8_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            argb8888_t  color;

            /* Cast to an appropriate type */
            color.Value = *(uint32_t*) src;

            *dst = color.u.Red;
            dst += 1;
            *dst = color.u.Green;
            dst += 1;
            *dst = color.u.Blue;
            dst += 1;
            *dst = 255;
            dst += 1;

            src += ( src_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )  { break; }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return e;
    }
} /* End of function SoftBitBlt_XRGB8888_to_R8G8B8A8_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8_to_R8G8B8A8_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_R8G8B8_to_R8G8B8A8_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       dst_right_offset; 
    int       src_width; 
    int       dst_width; 
    enum    { src_bpp = 24 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeB( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );
    dst_right_offset = DestinationRect->Right * ( dst_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = (src_right_offset  - src_left_offset) + ( src_bpp / 8 );
    dst_width          = (dst_right_offset  - dst_left_offset) + ( dst_bpp / 8 );

    for (;;)
    {

        src = src_line_left + src_width;
        dst = dst_line_left + dst_width;

        while ( src > src_line_left )
        {

            /* Alpha */
            dst -= 1;
            *dst = 255;

            /* Blue */
            src -= 1;
            dst -= 1;
            *dst = *src;

            /* Green */
            src -= 1;
            dst -= 1;
            *dst = *src;

            /* Red */
            src -= 1;
            dst -= 1;
            *dst = *src;
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )  { break; }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return e;
    }
} /* End of function SoftBitBlt_R8G8B8_to_R8G8B8A8_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_RGB565_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_R8G8B8A8_to_RGB565_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 16 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            r8g8b8a8_t  src_color;

            /* Cast to an appropriate type */
            src_color.Value = *(uint32_t*) src;

            /* Cast to an appropriate type */
            *(uint16_t*) dst = (uint16_t) (
                ( ( src_color.u.Red   & 0xF8 ) << 8 ) |
                ( ( src_color.u.Green & 0xFC ) << 3 ) |
                ( ( src_color.u.Blue  & 0xF8 ) >> 3 ) );

            src += ( src_bpp / 8 );
            dst += ( dst_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )  { break; }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }

} /* End of function SoftBitBlt_R8G8B8A8_to_RGB565_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_RGB565_to_R8G8B8A8_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_RGB565_to_R8G8B8A8_NoBlend_NoStretch(    
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 16 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            #ifdef  __CC_ARM
            #pragma push
            #pragma diag_suppress 4017  /* C4017W  No problem, because union */
            #endif

            rgb565_t  color;  /* C4017W  No problem, because union  */

            #ifdef  __CC_ARM
            #pragma pop
            #endif

            /* Cast to an appropriate type */
            color.Value = *(uint16_t*) src;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Red << 3 ) | ( color.u.Red >> 2 ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Green << 2 ) | ( color.u.Green >> 4 ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Blue << 3 ) | ( color.u.Blue >> 2 ) );
            dst += 1;
            *dst = 0xFF;
            dst += 1;

            src += ( src_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )
            {
                break;
            }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }

} /* End of function SoftBitBlt_RGB565_to_R8G8B8A8_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_ARGB1555_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_R8G8B8A8_to_ARGB1555_NoBlend_NoStretch( 
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top;
        uint8_t*  src_line_left_top;
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 16 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            r8g8b8a8_t  src_color;

            /* Cast to an appropriate type */
            src_color.Value = *(uint32_t*) src;

            /* Cast to an appropriate type */
            *(uint16_t*) dst = (uint16_t)(
                ( ( src_color.u.Alpha & 0x80 ) << 8 ) |
                ( ( src_color.u.Red   & 0xF8 ) << 7 ) |
                ( ( src_color.u.Green & 0xF8 ) << 2 ) |
                ( ( src_color.u.Blue  & 0xF8 ) >> 3 ) );

            src += ( src_bpp / 8 );
            dst += ( dst_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )
            {
                break;
            }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_R8G8B8A8_to_ARGB1555_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_ARGB1555_to_R8G8B8A8_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_ARGB1555_to_R8G8B8A8_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top; 
        uint8_t*  src_line_left_top; 
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 

    int       src_width; 
    enum    { src_bpp = 16 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            #ifdef  __CC_ARM
            #pragma push
            #pragma diag_suppress 4017  /* C4017W  No problem, because union */
            #endif

            argb1555_t  color;  /* C4017W  No problem, because union */

            #ifdef  __CC_ARM
            #pragma pop
            #endif


            /* Cast to an appropriate type */
            color.Value = *(uint16_t*) src;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Red << 3 ) | ( color.u.Red >> 2 ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Green << 3 ) | ( color.u.Green >> 2 ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Blue << 3 ) | ( color.u.Blue >> 2 ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( 0xFF * color.u.Alpha );
            dst += 1;

            src += ( src_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )  { break; }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_ARGB1555_to_R8G8B8A8_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_ARGB4444_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_R8G8B8A8_to_ARGB4444_NoBlend_NoStretch(   
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top; 
        uint8_t*  src_line_left_top; 
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 16 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            r8g8b8a8_t  src_color;

            /* Cast to an appropriate type */
            src_color.Value = *(uint32_t*) src;

            /* Cast to an appropriate type */
            *(uint16_t*) dst = (uint16_t)(
                ( ( src_color.u.Alpha & 0xF0 ) << 8 ) |
                ( ( src_color.u.Red   & 0xF0 ) << 4 ) |
                ( ( src_color.u.Green & 0xF0 )      ) |
                ( ( src_color.u.Blue  & 0xF0 ) >> 4 ) );

            src += ( src_bpp / 8 );
            dst += ( dst_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )
            {
                break;
            }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_R8G8B8A8_to_ARGB4444_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_ARGB4444_to_R8G8B8A8_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_ARGB4444_to_R8G8B8A8_NoBlend_NoStretch(    
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top; 
        uint8_t*  src_line_left_top; 
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    enum    { src_bpp = 16 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {

            #ifdef  __CC_ARM
            #pragma push
            #pragma diag_suppress 4017  /* C4017W  No problem, because union */
            #endif

            argb4444_t  color;  /* C4017W  No problem, because union */

            #ifdef  __CC_ARM
            #pragma pop
            #endif

            /* Cast to an appropriate type */
            color.Value = *(uint16_t*) src;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Red << 4 ) | ( color.u.Red ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Green << 4 ) | ( color.u.Green ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Blue << 4 ) | ( color.u.Blue ) );
            dst += 1;

            /* Cast to an appropriate type */
            *dst = (uint8_t)( ( color.u.Alpha << 4 ) | ( color.u.Alpha ) );
            dst += 1;

            src += ( src_bpp / 8 );
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )
            {
                break;
            }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_ARGB4444_to_R8G8B8A8_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_R8G8B8A8_to_YUV422_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_R8G8B8A8_to_YUV422_NoBlend_NoStretch(    
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top; 
        uint8_t*  src_line_left_top; 
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    bool_t      is_dst_left_odd;
    bool_t      is_dst_right_even;
    enum    { src_bpp = 32 }; 
    enum    { dst_bpp = 16 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif

    /* Cast to an appropriate type */
    is_dst_left_odd   = ( R_Mod_2u( (unsigned)DestinationRect->Left  ) == 1 );

    /* Cast to an appropriate type */
    is_dst_right_even = ( R_Mod_2u( (unsigned)DestinationRect->Right ) == 0 );

    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    if ( is_dst_left_odd )
    {
        dst_left_offset  -= ( src_bpp / 8 );
    }

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {
        r8g8b8a8_t  src_colors[2];
        argb8888_t  src_colors_argb[2];

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;
        if ( is_dst_right_even )
        {
            src_line_right -= 4;
        }

        if ( is_dst_left_odd )
        {
            yuv422_t    yuv422;

            /* Cast to an appropriate type */
            src_colors[1].Value = *(uint32_t*) src;
            src += ( src_bpp / 8 );

            src_colors_argb[1] = r8g8b8a8_t_to_argb8888_t( src_colors[1] );
            src_colors_argb[0] = src_colors_argb[1];  /* Set same color at out of image */
            yuv422 = ARGB8888Type_to_YUV422Type( src_colors_argb );

            /* Cast to an appropriate type */
            *(uint32_t*) dst = yuv422.Value;

            dst += (( dst_bpp / 8 ) * 2);
        }

        while ( src <= src_line_right )
        {
            yuv422_t    yuv422;

            /* Cast to an appropriate type */
            src_colors[0].Value = *(uint32_t*) src;
            src += ( src_bpp / 8 );

            /* Cast to an appropriate type */
            src_colors[1].Value = *(uint32_t*) src;
            src += ( src_bpp / 8 );

            src_colors_argb[0] = r8g8b8a8_t_to_argb8888_t( src_colors[0] );
            src_colors_argb[1] = r8g8b8a8_t_to_argb8888_t( src_colors[1] );
            yuv422 = ARGB8888Type_to_YUV422Type( src_colors_argb );

            /* Cast to an appropriate type */
            *(uint32_t*) dst = yuv422.Value;
            dst += (( dst_bpp / 8 ) * 2);
        }

        if ( is_dst_right_even )
        {
            yuv422_t    yuv422;

            /* Cast to an appropriate type */
            src_colors[0].Value = *(uint32_t*) src;
            src += ( src_bpp / 8 );

            src_colors_argb[0] = r8g8b8a8_t_to_argb8888_t( src_colors[0] );
            src_colors_argb[1] = src_colors_argb[0];  /* Set same color at out of image */
            yuv422 = ARGB8888Type_to_YUV422Type( src_colors_argb );

            /* Cast to an appropriate type */
            *(uint32_t*) dst = yuv422.Value;

            /* dst += ( dst_bpp / 8 ) * 2; */  /* dst is not referenced */
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )
            {
                break;
            }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_R8G8B8A8_to_YUV422_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_YUV422_to_R8G8B8A8_NoBlend_NoStretch 
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_YUV422_to_R8G8B8A8_NoBlend_NoStretch(    
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top; 
        uint8_t*  src_line_left_top; 
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 
    bool_t      is_src_left_odd;
    bool_t      is_src_right_even;
    enum    { src_bpp = 16 }; 
    enum    { dst_bpp = 32 }; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #if  CHECK_ARG
        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif

    /* Cast to an appropriate type */
    is_src_left_odd   = ( R_Mod_2u( (unsigned)SourceRect->Left  ) == 1 );

    /* Cast to an appropriate type */
    is_src_right_even = ( R_Mod_2u( (unsigned)SourceRect->Right ) == 0 );

    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset = SourceRect->Left * ( src_bpp / 8 );
    dst_left_offset = DestinationRect->Left * ( dst_bpp / 8 );
    src_right_offset = SourceRect->Right * ( src_bpp / 8 );

    if ( is_src_left_odd )
    {
        src_left_offset  -= ( src_bpp / 8 );
    }
    if ( is_src_right_even )
    {
        src_right_offset += ( src_bpp / 8 );
    }

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    for (;;)
    {
        yuv422_t    src01_color;
        argb8888_t  dst_colors[2];

        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;
        if ( is_src_right_even )
        {
            src_line_right -= 4;
        }

        if ( is_src_left_odd )
        {
            /* Cast to an appropriate type */
            src01_color.Value = *(uint32_t*) src;
            src += (( src_bpp / 8 ) * 2);

            YUV422Type_to_ARGB8888Type( src01_color, dst_colors );

            /* Cast to an appropriate type */
            *(uint32_t*) dst = argb8888_t_to_r8g8b8a8_t( dst_colors[1] ).Value;
            dst += ( dst_bpp / 8 );
        }

        while ( src <= src_line_right )
        {
            /* Cast to an appropriate type */
            src01_color.Value = *(uint32_t*) src;
            src += (( src_bpp / 8 ) * 2);

            YUV422Type_to_ARGB8888Type( src01_color, dst_colors );

            /* Cast to an appropriate type */
            *(uint32_t*) dst = argb8888_t_to_r8g8b8a8_t( dst_colors[0] ).Value;
            dst += ( dst_bpp / 8 );

            /* Cast to an appropriate type */
            *(uint32_t*) dst = argb8888_t_to_r8g8b8a8_t( dst_colors[1] ).Value;
            dst += ( dst_bpp / 8 );
        }

        if ( is_src_right_even )
        {
            /* Cast to an appropriate type */
            src01_color.Value = *(uint32_t*) src;
            src += (( src_bpp / 8 ) * 2);

            YUV422Type_to_ARGB8888Type( src01_color, dst_colors );

            /* Cast to an appropriate type */
            *(uint32_t*) dst = argb8888_t_to_r8g8b8a8_t( dst_colors[0] ).Value;
            /* dst += ( dst_bpp / 8 ); */  /* dst is not referenced */
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )
            {
                break;
            }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_YUV422_to_R8G8B8A8_NoBlend_NoStretch */

/***********************************************************************
* Function Name: ChangePixelFormat
************************************************************************/
errnum_t  ChangePixelFormat( uint32_t InputColor, pixel_format_t InputFormat,    
    uint32_t* out_Color, pixel_format_t OutputFormat )
{
    errnum_t    e;

    #ifdef  __CC_ARM
    #pragma push
    #pragma diag_suppress 4017  /* C4017W  No problem, because union */
    #endif

#ifdef  argb8888_t
    argb8888_t  argb8888;  /* C4017W  No problem, because union */
#endif
#ifdef  rgb565_t
    rgb565_t    rgb565;  /* C4017W  No problem, because union */
#endif
#ifdef  argb1555_t
    argb1555_t  argb1555;  /* C4017W  No problem, because union */
#endif
#ifdef  argb4444_t
    argb4444_t  argb4444;  /* C4017W  No problem, because union */
#endif
#ifdef  r8g8b8a8_t
    r8g8b8a8_t  r8g8b8a8;
#endif
#ifdef  yuv422_t
#if 0 
    yuv422_t    yuv422;
#endif
#endif
    #ifdef  __CC_ARM
    #pragma pop
    #endif


    /* Change input color to ARGB8888 */
    switch ( InputFormat )
    {
        #ifdef  argb8888_t
        case  PIXEL_FORMAT_ARGB8888:
            argb8888.Value = InputColor;
            break;
        #endif 

            #ifdef  XRGB8888Type
        case  PIXEL_FORMAT_XRGB8888:
            argb8888.Value = InputColor;
            argb8888.u.Alpha = 255;
            break;
            #endif

            #ifdef  rgb565_t
        case  PIXEL_FORMAT_RGB565:

            /* Cast to an appropriate type */
            rgb565.Value = (uint16_t) InputColor;
            argb8888 = rgb565_t_to_argb8888_t( rgb565 );
            break;
            #endif

            #ifdef  argb1555_t
        case  PIXEL_FORMAT_ARGB1555:

            /* Cast to an appropriate type */
            argb1555.Value = (uint16_t) InputColor;
            argb8888 = argb1555_t_to_argb8888_t( argb1555 );
            break;
            #endif

            #ifdef  argb4444_t
        case  PIXEL_FORMAT_ARGB4444:

            /* Cast to an appropriate type */
            argb4444.Value = (uint16_t) InputColor;
            argb8888 = argb4444_t_to_argb8888_t( argb4444 );
            break;
            #endif

        #ifdef  r8g8b8a8_t 
        case  PIXEL_FORMAT_R8G8B8A8:
            r8g8b8a8.Value = InputColor;
            argb8888 = r8g8b8a8_t_to_argb8888_t( r8g8b8a8 );
            break;
        #endif 

        #ifdef  yuv422_t 
            #if 0 
        case  PIXEL_FORMAT_YUV422: {
            argb8888_t  argb[2];

            yuv422.Value = InputColor;
            yuv422_t_to_argb8888_t( yuv422, argb );
            argb8888 = argb[0];
            break;
        }
        #endif
        #endif

        default:
            ASSERT_R( false, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1; goto fin ); 
                /* Error: <E_NOT_SUPPORTED_PIXEL_FORMAT/> */
            /* argb8888.Value = InputColor; */ /* Comment out is for ICCARM warinig [Pe111] */

            #ifdef  __CC_ARM
            #pragma push
            #pragma diag_suppress 111  /* #111-D: statement is unreachable */
            #endif

            break;

            #ifdef  __CC_ARM
            #pragma pop
            #endif
    }


    /* Change out color format */
    switch ( OutputFormat )
    {
        #ifdef  argb8888_t
        case  PIXEL_FORMAT_ARGB8888:
            *out_Color = argb8888.Value;
            break;
            #endif

            #ifdef  XRGB8888Type
        case  PIXEL_FORMAT_XRGB8888:
            argb8888.u.Alpha = 0;
            *out_Color = argb8888.Value;
            break;
            #endif

            #ifdef  rgb565_t
        case  PIXEL_FORMAT_RGB565:
            rgb565 = argb8888_t_to_rgb565_t( argb8888 );
            *out_Color = rgb565.Value;
            break;
            #endif

            #ifdef  argb1555_t
        case  PIXEL_FORMAT_ARGB1555:
            argb1555 = argb8888_t_to_argb1555_t( argb8888 );
            *out_Color = argb1555.Value;
            break;
            #endif

            #ifdef  argb4444_t
        case  PIXEL_FORMAT_ARGB4444:
            argb4444 = argb8888_t_to_argb4444_t( argb8888 );
            *out_Color = argb4444.Value;
            break;
            #endif

            #ifdef  r8g8b8a8_t
        case  PIXEL_FORMAT_R8G8B8A8:
            r8g8b8a8 = argb8888_t_to_r8g8b8a8_t( argb8888 );
            *out_Color = r8g8b8a8.Value;
            break;
            #endif

            #ifdef  yuv422_t
            #if 0 
        case  PIXEL_FORMAT_YUV422: {
            argb8888_t  argb[2];

            argb[0] = argb8888;
            argb[1] = argb8888;
            yuv422 = argb8888_t_to_yuv422_t( argb );
            *out_Color = yuv422.Value;
            break;
        }
        #endif
        #endif

        default:
            ASSERT_R( false, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2; goto fin ); 

            /* Error: <E_NOT_SUPPORTED_PIXEL_FORMAT/> */

            #ifdef  __CC_ARM
            #pragma push
            #pragma diag_suppress 111  /* #111-D: statement is unreachable */
            #endif

            break;

            #ifdef  __CC_ARM
            #pragma pop
            #endif
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function ChangePixelFormat */



/***********************************************************************
* Function Name: Get8bitPixelValueIn4bitFrameBufferLine
************************************************************************/
STATIC_INLINE unsigned int  Get8bitPixelValueIn4bitFrameBufferLine 
                            ( uint8_t* AddressOfLineLeft, unsigned int X )
{
    unsigned int  value; 

    value = *( AddressOfLineLeft + (X / 2) );
    value <<= (R_Mod_2u( X ) * 4);  /* MSB is left */
    value &= 0xF0;
    return  ( value ) | ( value >> 4 );
} /* End of function Get8bitPixelValueIn4bitFrameBufferLine */



/***********************************************************************
* Function Name: Get8bitPixelValueIn2bitFrameBufferLine
************************************************************************/
STATIC_INLINE unsigned int  Get8bitPixelValueIn2bitFrameBufferLine 
                            ( uint8_t* AddressOfLineLeft, unsigned int X )
{
    unsigned int  value; 

    value = *( AddressOfLineLeft + (X / 4) );
    value <<= (R_Mod_4u( X ) * 2);  /* MSB is left */
    value &= 0xC0;
    return  ( value ) | ( value >> 2 ) | ( value >> 4 ) | ( value >> 6 );
} /* End of function Get8bitPixelValueIn2bitFrameBufferLine */



/***********************************************************************
* Function Name: Get8bitPixelValueIn1bitFrameBufferLine
************************************************************************/
STATIC_INLINE unsigned int  Get8bitPixelValueIn1bitFrameBufferLine 
                            ( uint8_t* AddressOfLineLeft, unsigned int X )
{
    unsigned int  value; 

    value = *( AddressOfLineLeft + (X / 8) );
    value <<= R_Mod_8u( X );  /* MSB is left */
    if ( value & 0x80 )
    {
        return  0xFF;
    }
    else
    {
        return  0x00;
    }
} /* End of function Get8bitPixelValueIn1bitFrameBufferLine */



/***********************************************************************
* Function Name: Get8bitPixelValueInFrameBufferLine
************************************************************************/
unsigned int  
                Get8bitPixelValueInFrameBufferLine  
                ( uint8_t* AddressOfLineLeft, unsigned int BitPerPixel,
    unsigned int X )
{
    switch ( BitPerPixel )
    {
        case  8:
            return  *( AddressOfLineLeft + X );
        case  4:
            return  Get8bitPixelValueIn4bitFrameBufferLine( AddressOfLineLeft, X );
        case  2:
            return  Get8bitPixelValueIn2bitFrameBufferLine( AddressOfLineLeft, X );
        case  1:
            return  Get8bitPixelValueIn1bitFrameBufferLine( AddressOfLineLeft, X );
        default:
            ASSERT_R( false, R_NOOP() ); 
            return  0;
    }
} /* End of function Get8bitPixelValueInFrameBufferLine */



/***********************************************************************
* Function Name: SoftBitBlt_Blend_NoStretch
************************************************************************/
errnum_t  SoftBitBlt_Blend_NoStretch(     
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    SoftBitBlt_ConfigClass*      in_out_Config )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    unsigned  src_byte_per_pixel; 
    unsigned  dst_byte_per_pixel; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  src_line_head_top; 
        uint8_t*  src_line_left_top; 
    #else
        uint8_t*  src_line_head_bottom; 
        uint8_t*  src_line_left_bottom; 
    #endif
    uint8_t*  src_line_right; 
    int       src_left_offset; 
    int       dst_left_offset; 
    int       src_right_offset; 
    int       src_width; 

    /* Cast to an appropriate type */
    uint8_t*  alpha_line_left = (uint8_t*) DUMMY_INITIAL_VALUE; 

    /* Cast to an appropriate type */
    unsigned  alpha_bit_per_pixel = (unsigned) DUMMY_INITIAL_VALUE; 
    unsigned  x; 
    enum    { alpha_frame_flags = F_SOFT_BITBLT_AlphaFrameBuffer  
        | F_SOFT_BITBLT_AlphaRect | F_SOFT_BITBLT_AlphaFrameBuffer_BytePerPixel };
    enum    { must_flags = F_SOFT_BITBLT_SourcePixelFormat | 
        F_SOFT_BITBLT_SourceBytePerPixel |
        F_SOFT_BITBLT_DestinationPixelFormat | F_SOFT_BITBLT_DestinationBytePerPixel };
    SoftBitBlt_ConfigClass  default_config;
    bit_flags_fast32_t  config_flags;

    /* Cast to an appropriate type */
    if ( in_out_Config == NULL )
    {
        in_out_Config = &default_config;
        in_out_Config->Flags = 0;
    }

    config_flags = in_out_Config->Flags;

    if ( ! ( config_flags & F_SOFT_BITBLT_DestinationAlphaValue ) )
        { in_out_Config->DestinationAlphaValue = 255; }


    #if  CHECK_ARG
        switch ( in_out_Config->SourcePixelFormat )
        {
            case PIXEL_FORMAT_ARGB8888:
            case PIXEL_FORMAT_ARGB1555:
            case PIXEL_FORMAT_ARGB4444:
                break;
            default:
                ASSERT_R( false /* SourcePixelFormat */, goto err ); 

                #ifdef  __CC_ARM
                #pragma push
                #pragma diag_suppress 111  /* #111-D: statement is unreachable */
                #endif

                break;

                #ifdef  __CC_ARM
                #pragma pop
                #endif
        }
        switch ( in_out_Config->DestinationPixelFormat )
        {
            case PIXEL_FORMAT_ARGB8888:
            case PIXEL_FORMAT_XRGB8888:
            case PIXEL_FORMAT_RGB565:
            case PIXEL_FORMAT_ARGB1555:
            case PIXEL_FORMAT_ARGB4444:
                break;
            default:
                ASSERT_R( false /* DestinationPixelFormat */, goto err ); 

                #ifdef  __CC_ARM
                #pragma push
                #pragma diag_suppress 111  /* #111-D: statement is unreachable */
                #endif

                break;

                #ifdef  __CC_ARM
                #pragma pop
                #endif
        }

        ASSERT_R( ( config_flags & must_flags ) == must_flags, goto err ); 
        ASSERT_R( /* alpha_frame_flags */ 
            ( config_flags & alpha_frame_flags ) == alpha_frame_flags ||
            ( config_flags & alpha_frame_flags ) == 0, goto err );

        e= SoftBitBlt_checkTypeA( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect );
        IF(e)
        {
            goto fin;
        }
    #endif /* CHECK_ARG */


    src_byte_per_pixel = in_out_Config->SourceBytePerPixel;
    dst_byte_per_pixel = in_out_Config->DestinationBytePerPixel;


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_top,  &dst_line_head );
    #else
        SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_line_head,  &src_line_head_bottom,  &dst_line_head );
    #endif

    src_left_offset =  SourceRect->Left      * src_byte_per_pixel;
    dst_left_offset =  DestinationRect->Left * dst_byte_per_pixel;
    src_right_offset = SourceRect->Right     * src_byte_per_pixel;

    src_line_left      = src_line_head     + src_left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        src_line_left_top  = src_line_head_top + src_left_offset;
    #else
        src_line_left_bottom  = src_line_head_bottom + src_left_offset;
    #endif
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    if ( config_flags & F_SOFT_BITBLT_AlphaFrameBuffer )
    {
        SoftBitBltFrameBufferClass*
            frame = in_out_Config->AlphaFrameBuffer; 
        SoftBitBltRectangleClass*    rect  = in_out_Config->AlphaRect; 

        if ( frame->stride < 0 )
            { alpha_line_left = frame->Address - (frame->stride * rect->Bottom); }
        else
            { alpha_line_left = frame->Address + (frame->stride * rect->Bottom); }

        alpha_bit_per_pixel = R_RGA_BytePerPixelType_To_BitPerPixelType(
            in_out_Config->AlphaFrameBuffer_BytePerPixel );
    }


    for (;;)
    {
        src = src_line_left;
        dst = dst_line_left;
        x = 0;
        src_line_right = src_line_left + src_width;

        while ( src <= src_line_right )
        {
            argb8888_t  source;  /* source color */
            argb8888_t  destination;  /* destination color */
            uint_fast32_t  alpha = 255;


            /* Set "source" color */
            if ( src_byte_per_pixel == 2 )
            {
                /* Cast to an appropriate type */
                source.Value = *(uint16_t*) src;  /* Read by 16bit */
                e= ChangePixelFormat( source.Value, in_out_Config->SourcePixelFormat,
                    &source.Value, PIXEL_FORMAT_ARGB8888 );
                IF(e)
                {
                    goto fin;
                }
            }
            else
            {
                /* Cast to an appropriate type */
                source.Value = *(uint32_t*) src;
            }


            /* Set "destination" color */
            if ( dst_byte_per_pixel == 2 )
            {
                /* Cast to an appropriate type */
                destination.Value = *(uint16_t*) dst;  /* Read by 16bit */
                e= ChangePixelFormat( destination.Value, in_out_Config->DestinationPixelFormat,
                    &destination.Value, PIXEL_FORMAT_ARGB8888 );
                IF(e)
                {
                    goto fin;
                }
            }
            else
            {
                /* Cast to an appropriate type */
                destination.Value = *(uint32_t*) dst;
            }


            /* Multiple "alpha" and "LayerAlphaValue" */
            if ( config_flags & F_SOFT_BITBLT_LayerAlphaValue )
            {
                /* Cast to an appropriate type */
                alpha = SoftBitBlt_multiplyAlpha8( alpha, (uint8_t) in_out_Config->LayerAlphaValue );
            }


            /* Multiple "alpha" and "AlphaFrameBuffer" (Pixel alpha) */
            if ( config_flags & F_SOFT_BITBLT_AlphaFrameBuffer )
            {
                unsigned int  pixel_alpha; 

                pixel_alpha = Get8bitPixelValueInFrameBufferLine( alpha_line_left, alpha_bit_per_pixel, x );

                /* Cast to an appropriate type */
                alpha = SoftBitBlt_multiplyAlpha8( alpha, (uint8_t) pixel_alpha );
            }


            if ( config_flags & SOFT_BITBLT_SOURCE_IS_PREMULTIPLIED_ALPHA )
            {

                /* Pre-multiply layer alpha and pixel alpha */
                if ( alpha != 255 )
                {
                    source = SoftBitBlt_multiplyARGB8888_AndAlpha8( source, alpha, source.u.Alpha );

                    /* Do not change "source.u.Alpha" */
                }

                /* Multiple "alpha" and "source" alpha for next blending */
                alpha = SoftBitBlt_multiplyAlpha8( alpha, source.u.Alpha );
            }
            else
            {
                /* Multiple "alpha" and "source" alpha */
                alpha = SoftBitBlt_multiplyAlpha8( alpha, source.u.Alpha );

                /* Pre-multiply alpha of "source" color, layer alpha and pixel alpha */
                if ( alpha != 255 )
                {
                    source = SoftBitBlt_multiplyARGB8888_AndAlpha8( source, alpha, source.u.Alpha );

                    /* Do not change "source.u.Alpha" */
                }
            }


            /* Blend "destination" color with pre-multiplied "source" color */
            destination = SoftBitBlt_blendARGB8888_PremultipliedAlpha( source, destination,
                alpha, in_out_Config->DestinationAlphaValue );


            /* Output "destination" */
            if ( dst_byte_per_pixel == 2 )
            {
                e= ChangePixelFormat( destination.Value, PIXEL_FORMAT_ARGB8888,
                    &destination.Value, in_out_Config->DestinationPixelFormat );
                IF(e)
                {
                    goto fin;
                }

                /* Cast to an appropriate type */
                *(uint16_t*) dst = (uint16_t) destination.Value;  /* Write by 16bit */
            }
            else
            {
                /* Cast to an appropriate type */
                *(uint32_t*) dst = destination.Value;
            }


            /* Next */
            src += src_byte_per_pixel;
            dst += dst_byte_per_pixel;
            x += 1;
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( src_line_left == src_line_left_top )
            {
                break;
            }
        #else
            if ( src_line_left == src_line_left_bottom )
            {
                break;
            }
        #endif

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
        if ( config_flags & F_SOFT_BITBLT_AlphaFrameBuffer )
            { alpha_line_left += in_out_Config->AlphaFrameBuffer->stride; }
    }

    e=0;
fin:
    {
        return  e;
    }
err:
    {
        e = E_RGA_OTHER_6;
        goto fin;  /* Error: <Private/> */
    }
} /* End of function SoftBitBlt_Blend_NoStretch */

/***********************************************************************
* Function Name: SoftFillRectangle_toInFrame
************************************************************************/
LIB_STATIC errnum_t  SoftFillRectangle_toInFrame( 
    SoftBitBltFrameBufferClass* frame_buffer,
    SoftBitBltRectangleClass* Rectangle )
{
    int  top; 
    int  bottom; 

    /* fast culling by out of frame buffer horizontal */
    if ( (Rectangle->Left >= frame_buffer->width) ||
        (Rectangle->Right < 0) )
    {
        Rectangle->Right = Rectangle->Left - 1;  /* out of frame */
        return  0;
    }


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE

        /* set "top", "bottom" */
        if ( frame_buffer->stride < 0 )
        {
            top    = Rectangle->Top;
            bottom = Rectangle->Bottom;
        }
        else
        {
            top    = (frame_buffer->height - Rectangle->Top) - 1;
            bottom = (frame_buffer->height - Rectangle->Bottom) - 1;
        }
    #else

        /* set "top", "bottom" */
        if ( frame_buffer->stride > 0 )
        {
            top    = Rectangle->Top;
            bottom = Rectangle->Bottom;
        }
        else
        {
            top    = (frame_buffer->height - Rectangle->Top) - 1;
            bottom = (frame_buffer->height - Rectangle->Bottom) - 1;
        }
    #endif


    /* fast culling by out of frame buffer vertical */
    if ( (top >= frame_buffer->height) ||
        (bottom < 0)  ||
        (top > bottom) )
    {
        Rectangle->Right = Rectangle->Left - 1;  /* out of frame */
        return  0;
    }


    /* set top >= 0 */
    if ( top < 0 )
    {
        top = 0;
    }


    /* set bottom < frame height */
    if ( bottom >= frame_buffer->height )
    {
        bottom = frame_buffer->height - 1;
    }


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE

        /* culling by out of frame buffer vertical for modified top and bottom */
        if ( (top >= frame_buffer->height) ||
            (bottom < 0) )
        {
            Rectangle->Right = (Rectangle->Left - 1);  /* out of frame */
            return  0;
        }


        /* return from "top", "bottom" */
        if ( frame_buffer->stride < 0 )
        {
            Rectangle->Top    = top;
            Rectangle->Bottom = bottom;
        }
        else
        {
            Rectangle->Top    = ((frame_buffer->height - top) - 1);
            Rectangle->Bottom = ((frame_buffer->height - bottom) - 1);
        }
    #else

        /* culling by out of frame buffer vertical for modified top and bottom */
        if ( (top < 0) ||
            (bottom >= frame_buffer->height) )
        {
            Rectangle->Right = Rectangle->Left - 1;  /* out of frame */
            return  0;
        }


        /* return from "top", "bottom" */
        if ( frame_buffer->stride > 0 )
        {
            Rectangle->Top    = top;
            Rectangle->Bottom = bottom;
        }
        else
        {
            Rectangle->Top    = (frame_buffer->height - top) - 1;
            Rectangle->Bottom = (frame_buffer->height - bottom) - 1;
        }
    #endif


    /* set left >= 0 */
    if ( Rectangle->Left < 0 )
    {
        Rectangle->Left = 0;
    }


    /* set right < frame width */
    if ( Rectangle->Right >= frame_buffer->width )
    {
        Rectangle->Right = frame_buffer->width - 1;
    }


    /* culling by out of frame buffer horizontal for modified top and bottom */
    if ( (Rectangle->Left >= frame_buffer->width) ||
        (Rectangle->Right < 0) )
    {
        Rectangle->Right = Rectangle->Left - 1;  /* out of frame */
        return  0;
    }

    return  0;
} /* End of function SoftFillRectangle_toInFrame */

/***********************************************************************
* Function Name: SoftBitBlt_toInFrame 
************************************************************************/
#if 0 
LIB_STATIC errnum_t  SoftBitBlt_toInFrame(
    SoftBitBltFrameBufferClass* SourceFrameBuffer,
    SoftBitBltFrameBufferClass* DestinationFrameBuffer,
    SoftBitBltRectangleClass* SourceRect,
    SoftBitBltRectangleClass* DestinationRect )
{
    errnum_t  e;
    int  src_top;     /* top down coord */
    int  src_bottom;  /* top down coord */
    int  dst_top;     /* top down coord */
    int  dst_bottom;  /* top down coord */
    int  diff;


    ASSERT_D( SourceRect->Right - SourceRect->Left == DestinationRect->Right - DestinationRect->Left,
        e=E_OTHERS; goto fin );
    ASSERT_D( SourceRect->Bottom - SourceRect->Top == DestinationRect->Bottom - DestinationRect->Top,
        e=E_OTHERS; goto fin );


    /* fast culling by out of frame buffer horizontal */
    if (      (SourceRect->Left >= SourceFrameBuffer->width) ||
        (DestinationRect->Left >= DestinationFrameBuffer->width) ||
        (SourceRect->Right < 0) ||(DestinationRect->Right < 0) )
    {
        SourceRect->Right = SourceRect->Left - 1;  /* out of frame */
        e = 0;
        goto fin;
    }


    /* set "src_top", "src_bottom", "dst_top", "dst_bottom" */
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        if ( SourceFrameBuffer->stride < 0 )
        {
            src_top    = SourceRect->Top;
            src_bottom = SourceRect->Bottom;
        }
        else
        {
            src_top    = (SourceFrameBuffer->height - SourceRect->Top) - 1;
            src_bottom = (SourceFrameBuffer->height - SourceRect->Bottom) - 1;
        }

        if ( DestinationFrameBuffer->stride < 0 )
        {
            dst_top    = DestinationRect->Top;
            dst_bottom = DestinationRect->Bottom;
        }
        else
        {
            dst_top    = (DestinationFrameBuffer->height - DestinationRect->Top) - 1;
            dst_bottom = (DestinationFrameBuffer->height - DestinationRect->Bottom) - 1;
        }
    #else
        if ( SourceFrameBuffer->stride > 0 )
        {
            src_top    = SourceRect->Top;
            src_bottom = SourceRect->Bottom;
        }
        else
        {
            src_top    = (SourceFrameBuffer->height - SourceRect->Top) - 1;
            src_bottom = (SourceFrameBuffer->height - SourceRect->Bottom) - 1;
        }

        if ( DestinationFrameBuffer->stride > 0 )
        {
            dst_top    = DestinationRect->Top;
            dst_bottom = DestinationRect->Bottom;
        }
        else
        {
            dst_top    = (DestinationFrameBuffer->height - DestinationRect->Top) - 1;
            dst_bottom = (DestinationFrameBuffer->height - DestinationRect->Bottom) - 1;
        }
    #endif


    /* fast culling by out of frame buffer vertical */
    if ( (src_top >= SourceFrameBuffer->height) ||
        (dst_top >= DestinationFrameBuffer->height) ||
        (src_bottom < 0) || (dst_bottom < 0) )
    {
        SourceRect->Right = SourceRect->Left - 1;  /* out of frame */
        e = 0;
        goto fin;
    }


    /* Set top >= 0 : "src_top >= 0", "dst_top >= 0" */
    if ( src_top < 0 )
    {
        dst_top += (- src_top);
        src_top = 0;
    }

    if ( dst_top < 0 )
    {
        src_top += (- dst_top);
        dst_top = 0;
    }


    /* Set bottom < frame height */
    if ( src_bottom >= SourceFrameBuffer->height )
    {
        diff = (src_bottom - SourceFrameBuffer->height) + 1;
        src_bottom -= diff;
        dst_bottom -= diff;
    }

    if ( dst_bottom >= DestinationFrameBuffer->height )
    {
        diff = (dst_bottom - DestinationFrameBuffer->height) + 1;
        src_bottom -= diff;
        dst_bottom -= diff;
    }


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE

        /* culling by out of frame buffer vertical for modified top and bottom */
        if ( (src_top >= SourceFrameBuffer->height) ||
            (dst_top >= DestinationFrameBuffer->height) ||
            (src_bottom < 0) || (dst_bottom < 0) )
        {
            SourceRect->Right = SourceRect->Left - 1;  /* out of frame */
            e = 0;
            goto fin;
        }


        /* return from "src_top", "src_bottom", "dst_top", "dst_bottom" */
        if ( SourceFrameBuffer->stride < 0 )
        {
            SourceRect->Top    = src_top;
            SourceRect->Bottom = src_bottom;
        } else
        {
            SourceRect->Top    = (SourceFrameBuffer->height - src_top) - 1;
            SourceRect->Bottom = (SourceFrameBuffer->height - src_bottom) - 1;
        }

        if ( DestinationFrameBuffer->stride < 0 )
        {
            DestinationRect->Top    = dst_top;
            DestinationRect->Bottom = dst_bottom;
        }
        else
        {
            DestinationRect->Top    = (DestinationFrameBuffer->height - dst_top) - 1;
            DestinationRect->Bottom = (DestinationFrameBuffer->height - dst_bottom) - 1;
        }
    #else /* Not ifdef IFDEF_SOFT_BIT_BLT_INVERT_STRIDE */

        /* culling by out of frame buffer vertical for modified top and bottom */
        if ( (src_top < 0) ||
            (dst_top < 0) ||
            (src_bottom >= SourceFrameBuffer->height) ||
            (dst_bottom >= DestinationFrameBuffer->height) )
        {
            SourceRect->Right = SourceRect->Left - 1;  /* out of frame */
            e = 0;
            goto fin;
        }


        /* return from "src_top", "src_bottom", "dst_top", "dst_bottom" */
        if ( SourceFrameBuffer->stride > 0 )
        {
            SourceRect->Top    = src_top;
            SourceRect->Bottom = src_bottom;
        }
        else
        {
            SourceRect->Top    = (SourceFrameBuffer->height - src_top) - 1;
            SourceRect->Bottom = (SourceFrameBuffer->height - src_bottom) - 1;
        }

        if ( DestinationFrameBuffer->stride > 0 )
        {
            DestinationRect->Top    = dst_top;
            DestinationRect->Bottom = dst_bottom;
        }
        else
        {
            DestinationRect->Top    = (DestinationFrameBuffer->height - dst_top) - 1;
            DestinationRect->Bottom = (DestinationFrameBuffer->height - dst_bottom) - 1;
        }
    #endif /* IFDEF_SOFT_BIT_BLT_INVERT_STRIDE */


    /* set left >= 0 */
    if ( SourceRect->Left < 0 )
    {
        DestinationRect->Left += (- SourceRect->Left);
        SourceRect->Left = 0;
    }

    if ( DestinationRect->Left < 0 )
    {
        SourceRect->Left += (- DestinationRect->Left);
        DestinationRect->Left = 0;
    }


    /* set right < frame width */
    if ( SourceRect->Right >= SourceFrameBuffer->width )
    {
        diff = (SourceRect->Right - SourceFrameBuffer->width) + 1;
        SourceRect->Right      -= diff;
        DestinationRect->Right -= diff;
    }

    if ( DestinationRect->Right >= DestinationFrameBuffer->width )
    {
        diff = (DestinationRect->Right - DestinationFrameBuffer->width) + 1;
        SourceRect->Right      -= diff;
        DestinationRect->Right -= diff;
    }


    /* culling by out of frame buffer horizontal for modified top and bottom */
    if (      (SourceRect->Left >= SourceFrameBuffer->width) ||
        (DestinationRect->Left >= DestinationFrameBuffer->width) ||
        (SourceRect->Right < 0) ||
        (DestinationRect->Right < 0) )
    {
        SourceRect->Right = (SourceRect->Left - 1);  /* out of frame */
        e = 0;
        goto fin;
    }

    e=0;
fin:
    return  e;
} /* End of function SoftBitBlt_toInFrame */
#endif /* 0 */

/***********************************************************************
* Function Name: SoftBitBlt_toInClippedFrame
************************************************************************/
LIB_STATIC errnum_t
    SoftBitBlt_toInClippedFrame(  
    const SoftBitBltFrameBufferClass*  in_SourceFrameBuffer,
    const SoftBitBltFrameBufferClass*  in_DestinationFrameBuffer,
    SoftBitBltRectangleClass*      in_out_SourceRect,
    SoftBitBltRectangleClass*      in_out_DestinationRect,
    const SoftBitBltRectangleClass*    in_ClippingRect )
{
    errnum_t  e;
    int_fast32_t  src_top;     /* top down coord */
    int_fast32_t  src_bottom;  /* top down coord */
    int_fast32_t  dst_top;     /* top down coord */
    int_fast32_t  dst_bottom;  /* top down coord */
    int_fast32_t  clp_top;     /* top down coord */
    int_fast32_t  clp_bottom;  /* top down coord */
    int_fast32_t  clp_left;    /* top down coord */
    int_fast32_t  clp_right;   /* top down coord */
    int_fast32_t  diff;


    #ifndef  R_OSPL_NDEBUG
    {
        SoftBitBltRectangleClass*  src = in_out_SourceRect;      
        SoftBitBltRectangleClass*  dst = in_out_DestinationRect; 
        int_fast32_t  src_flag;
        int_fast32_t  dst_flag;


        ASSERT_D( src->Right - src->Left == dst->Right - dst->Left,
            e=E_RGA_OTHER_7; goto fin );  /* Error: <Private/> */


        if ( in_SourceFrameBuffer->stride >= 0 )
            { src_flag = +1; }
        else
            { src_flag = -1; }

        if ( in_DestinationFrameBuffer->stride >= 0 )
            { dst_flag = +1; }
        else
            { dst_flag = -1; }

        ASSERT_D( ( src->Bottom - src->Top ) * src_flag == ( dst->Bottom - dst->Top ) * dst_flag,
            e=E_RGA_OTHER_8; goto fin );  /* Error: <Private/> */
    }
    #endif


    /* Set "clp_left", "clp_right" */
    clp_left  = in_ClippingRect->Left;
    clp_right = in_ClippingRect->Right;


    /* fast culling by out of frame buffer horizontal */
    if (      (in_out_SourceRect->Left >= in_SourceFrameBuffer->width) ||
        (in_out_DestinationRect->Left >= in_DestinationFrameBuffer->width) ||
                            (clp_left >= in_DestinationFrameBuffer->width) ||
            (in_out_SourceRect->Right < 0) ||
        (in_out_DestinationRect->Right < 0) ||
                            (clp_right < 0) )
    {
        in_out_SourceRect->Right = in_out_SourceRect->Left - 1;  /* out of frame */
        e = 0;
        goto fin;
    }


    /* Set "clp_left", "clp_right" : Put in "in_DestinationFrameBuffer" */
    if ( clp_left < 0 )
        { clp_left  = 0; }
    if ( clp_right >= in_DestinationFrameBuffer->width )
        { clp_right = in_DestinationFrameBuffer->width - 1; }


    /* set "src_top", "src_bottom", "dst_top", "dst_bottom", "clp_top", "clp_bottom" */
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        if ( in_SourceFrameBuffer->stride < 0 )
        {
            src_top    = in_out_SourceRect->Top;
            src_bottom = in_out_SourceRect->Bottom;
        }
        else
        {
            src_top    = (in_SourceFrameBuffer->height - in_out_SourceRect->Top) - 1;
            src_bottom = (in_SourceFrameBuffer->height - in_out_SourceRect->Bottom) - 1;
        }

        if ( in_DestinationFrameBuffer->stride < 0 )
        {
            dst_top    = in_out_DestinationRect->Top;
            dst_bottom = in_out_DestinationRect->Bottom;
            clp_top    = in_ClippingRect->Top;
            clp_bottom = in_ClippingRect->Bottom;
        }
        else
        {
            dst_top    = (in_DestinationFrameBuffer->height - in_out_DestinationRect->Top) - 1;
            dst_bottom = (in_DestinationFrameBuffer->height - in_out_DestinationRect->Bottom) - 1;
            clp_top    = (in_DestinationFrameBuffer->height - in_ClippingRect->Top) - 1;
            clp_bottom = (in_DestinationFrameBuffer->height - in_ClippingRect->Bottom) - 1;
        }
    #else
        if ( in_SourceFrameBuffer->stride > 0 )
        {
            src_top    = in_out_SourceRect->Top;
            src_bottom = in_out_SourceRect->Bottom;
        }
        else
        {
            src_top    = (in_SourceFrameBuffer->height - in_out_SourceRect->Top) - 1;
            src_bottom = (in_SourceFrameBuffer->height - in_out_SourceRect->Bottom) - 1;
        }

        if ( in_DestinationFrameBuffer->stride > 0 )
        {
            dst_top    = in_out_DestinationRect->Top;
            dst_bottom = in_out_DestinationRect->Bottom;
            clp_top    = in_ClippingRect->Top;
            clp_bottom = in_ClippingRect->Bottom;
        }
        else
        {
            dst_top    = (in_DestinationFrameBuffer->height - in_out_DestinationRect->Top) - 1;
            dst_bottom = (in_DestinationFrameBuffer->height - in_out_DestinationRect->Bottom) - 1;
            clp_top    = (in_DestinationFrameBuffer->height - in_ClippingRect->Top) - 1;
            clp_bottom = (in_DestinationFrameBuffer->height - in_ClippingRect->Bottom) - 1;
        }
    #endif


    /* fast culling by out of frame buffer vertical */
    if (
        (src_top >= in_SourceFrameBuffer->height) ||
        (dst_top >= in_DestinationFrameBuffer->height) ||
        (clp_top >= in_DestinationFrameBuffer->height) ||
        (src_bottom < 0) ||
        (dst_bottom < 0) ||
        (clp_bottom < 0) )
    {
        in_out_SourceRect->Right = in_out_SourceRect->Left - 1;  /* out of frame */
        e = 0;
        goto fin;
    }


    /* Set "clp_top", "clp_bottom" */
    if ( clp_top < 0 )
        { clp_top = 0; }
    if ( clp_bottom >= in_DestinationFrameBuffer->height )
        { clp_bottom = in_DestinationFrameBuffer->height - 1; }


    /* Set top >= 0 : "src_top >= 0", "dst_top >= 0" */
    if ( src_top < 0 )
    {
        dst_top += (- src_top);
        src_top = 0;
    }

    if ( dst_top < clp_top )
    {
        src_top += ( (- dst_top) + clp_top );
        dst_top = clp_top;
    }


    /* Set bottom < frame height */
    if ( src_bottom >= in_SourceFrameBuffer->height )
    {
        diff = (src_bottom - in_SourceFrameBuffer->height) + 1;
        src_bottom -= diff;
        dst_bottom -= diff;
    }

    if ( dst_bottom > clp_bottom )
    {
        src_bottom -= (dst_bottom - clp_bottom);
        dst_bottom = clp_bottom;
    }


    /* culling by out of frame buffer vertical for modified top and bottom */
    if (
        (src_top >= in_SourceFrameBuffer->height) ||
        (dst_top >= in_DestinationFrameBuffer->height) ||
        (src_bottom < 0) ||
        (dst_bottom < 0) )
    {
        in_out_SourceRect->Right = in_out_SourceRect->Left - 1;  /* out of frame */
        e = 0;
        goto fin;
    }


    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE

        /* return from "src_top", "src_bottom", "dst_top", "dst_bottom" */
        if ( in_SourceFrameBuffer->stride < 0 )
        {
            in_out_SourceRect->Top    = src_top;
            in_out_SourceRect->Bottom = src_bottom;
        } else {
            in_out_SourceRect->Top    = (in_SourceFrameBuffer->height - src_top) - 1;
            in_out_SourceRect->Bottom = (in_SourceFrameBuffer->height - src_bottom) - 1;
        }

        if ( in_DestinationFrameBuffer->stride < 0 )
        {
            in_out_DestinationRect->Top    = dst_top;
            in_out_DestinationRect->Bottom = dst_bottom;
        }
        else
        {
            in_out_DestinationRect->Top    = (in_DestinationFrameBuffer->height - dst_top) - 1;
            in_out_DestinationRect->Bottom = (in_DestinationFrameBuffer->height - dst_bottom) - 1;
        }
    #else
        /* return from "src_top", "src_bottom", "dst_top", "dst_bottom" */
        if ( in_SourceFrameBuffer->stride > 0 )
        {
            in_out_SourceRect->Top    = src_top;
            in_out_SourceRect->Bottom = src_bottom;
        } else
        {
            in_out_SourceRect->Top    = (in_SourceFrameBuffer->height - src_top) - 1;
            in_out_SourceRect->Bottom = (in_SourceFrameBuffer->height - src_bottom) - 1;
        }

        if ( in_DestinationFrameBuffer->stride > 0 )
        {
            in_out_DestinationRect->Top    = dst_top;
            in_out_DestinationRect->Bottom = dst_bottom;
        }
        else
        {
            in_out_DestinationRect->Top    = (in_DestinationFrameBuffer->height - dst_top) - 1;
            in_out_DestinationRect->Bottom = (in_DestinationFrameBuffer->height - dst_bottom) - 1;
        }
    #endif


    /* set left >= 0 */
    if ( in_out_SourceRect->Left < 0 )
    {
        in_out_DestinationRect->Left += (- in_out_SourceRect->Left);
        in_out_SourceRect->Left = 0;
    }

    if ( in_out_DestinationRect->Left < clp_left )
    {
        in_out_SourceRect->Left += ( (- in_out_DestinationRect->Left) + clp_left );
        in_out_DestinationRect->Left = clp_left;
    }


    /* set right < frame width */
    if ( in_out_SourceRect->Right >= in_SourceFrameBuffer->width )
    {
        diff = (in_out_SourceRect->Right - in_SourceFrameBuffer->width) + 1;
        in_out_SourceRect->Right      -= diff;
        in_out_DestinationRect->Right -= diff;
    }

    if ( in_out_DestinationRect->Right > clp_right )
    {
        in_out_SourceRect->Right     -= (in_out_DestinationRect->Right - clp_right);
        in_out_DestinationRect->Right = clp_right;
    }


    /* culling by out of frame buffer horizontal for modified left and right */
    if (      (in_out_SourceRect->Left >= in_SourceFrameBuffer->width) ||
        (in_out_DestinationRect->Left  > clp_right) ||
            (in_out_SourceRect->Right < 0) ||
        (in_out_DestinationRect->Right < clp_left) )
    {
        in_out_SourceRect->Right = in_out_SourceRect->Left - 1;  /* out of frame */
        e = 0;
        goto fin;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_toInClippedFrame */

/***********************************************************************
* Function Name: SoftBitBlt_isOverlapFrameBuffer
************************************************************************/
LIB_STATIC errnum_t  SoftBitBlt_isOverlapFrameBuffer( 
    SoftBitBltFrameBufferClass* SourceFrameBuffer,
    SoftBitBltFrameBufferClass* DestinationFrameBuffer,
    bool_t* out_IsOverlap )
{
    errnum_t  e;
    UIntPtrStartOverType  source_memory;
    UIntPtrStartOverType  destination_memory;

    e= SoftBitBltFrameBufferClass_getStartOver( SourceFrameBuffer, &source_memory );
    IF(e)
    {
        goto fin;
    }
    e= SoftBitBltFrameBufferClass_getStartOver( DestinationFrameBuffer, &destination_memory );
    IF(e)
    {
        goto fin;
    }
    e= UIntPtrStartOverType_isOverlap( &source_memory, &destination_memory, out_IsOverlap );
    IF(e)
    {
        goto fin;
    }
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_isOverlapFrameBuffer */

/***********************************************************************
* Function Name: SoftBitBltFrameBufferClass_getStartOver
************************************************************************/
LIB_STATIC errnum_t SoftBitBltFrameBufferClass_getStartOver( 
    SoftBitBltFrameBufferClass* self,  UIntPtrStartOverType* out_StartOver )
{
    /* Cast to an appropriate type */
    out_StartOver->Start = (uintptr_t) self->Address;
    if ( self->stride > 0 )
    {
        /* Cast to an appropriate type */
        out_StartOver->Over  = (uintptr_t) self->Address + (self->stride * self->height);
    }
    else
    {
        /* Cast to an appropriate type */
        out_StartOver->Over = (uintptr_t) self->Address - (self->stride * self->height);
    }
    return  0;
} /* End of function SoftBitBltFrameBufferClass_getStartOver */

/***********************************************************************
* Function Name: SoftBitBlt_ARGB8888_NoBlend_NoStretch
************************************************************************/

#define  ENABLE_UNALIGNED_SoftBitBlt_ARGB8888_NoBlend_NoStretch  (1)
    /* 8.0->7.9msec (Debug, cached) */
    /* 6.4->5.2msec (Release, cached) */

errnum_t
    SoftBitBlt_ARGB8888_NoBlend_NoStretch(  
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    SoftBitBltFlags  Flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head_top; 
    uint8_t*  src_line_left_top; 
    uint8_t*  src_line_right; 
    int_fast32_t  src_left_offset;
    int_fast32_t  dst_left_offset;
    int_fast32_t  src_right_offset;
    int_fast32_t  src_width;

    #ifdef  __CC_ARM
    #pragma push
    #pragma diag_suppress 4017  /* C4017W  No problem, because union */
    #endif

    bool_t    is_width_odd;  /* C4017W,  No problem, Because #if */

    #ifdef  __CC_ARM
    #pragma pop
    #endif


    enum    { byte_per_pixel = 4 }; 


    #if  CHECK_ARG
    {
        bool_t  is_overlap;
        int_fast32_t  src_height;
        int_fast32_t  dst_height;

        e= SoftBitBlt_checkRectangle( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_height, &dst_height );
        IF(e)
        {
            goto fin;
        }

        ASSERT_R
        ( SourceRect->Right - SourceRect->Left == DestinationRect->Right - DestinationRect->Left,
            e=E_RGA_OTHER_9; goto fin );  /* Error: <Private/> */
        ASSERT_R
        ( src_height == dst_height, e=E_RGA_OTHER_CD; goto fin );  /* Error: <Private/> */

        e= SoftBitBlt_isOverlapFrameBuffer( SourceFrameBuffer, DestinationFrameBuffer, &is_overlap );
        IF(e)
        {
            goto fin;
        }
        ASSERT_R
        ( ! is_overlap, e=E_RGA_OTHER_A; goto fin );  /* Error: <Private/> */
    }
    #endif


    SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
        &src_line_head,  &src_line_head_top,  &dst_line_head );

    src_left_offset =  SourceRect->Left      * byte_per_pixel;
    dst_left_offset =  DestinationRect->Left * byte_per_pixel;
    src_right_offset = SourceRect->Right     * byte_per_pixel;

    src_line_left      = src_line_head     + src_left_offset;
    src_line_left_top  = src_line_head_top + src_left_offset;
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    #if  ENABLE_UNALIGNED_SoftBitBlt_ARGB8888_NoBlend_NoStretch
    if ( IS_BIT_SET( Flags, SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS ) )
    {
        if ( R_Mod_8s( src_width ) == 0 )
        {  /* 8 = "byte_per_pixel * 2" */
            is_width_odd = true;
            src_width -= byte_per_pixel;
        }
        else
        {
            is_width_odd = false;
        }
    }
    #endif

    for (;;)
    {
        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        #if  ENABLE_UNALIGNED_SoftBitBlt_ARGB8888_NoBlend_NoStretch
        if ( IS_BIT_SET( Flags, SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS ) )
        {
            while ( src <= src_line_right )
            {
                /* Cast to an appropriate type */
                *(uint64_t*) dst = *(uint64_t*) src;

                    /* This code may be do unaligned access */
                    /* But "uint64_t" can be accecced by 32bit alignment */
                    /*     in cached/uncached area */
                src += (byte_per_pixel * 2);
                dst += (byte_per_pixel * 2);
            }
            if ( is_width_odd )
            {
                /* Cast to an appropriate type */
                *(uint32_t*) dst = *(uint32_t*) src;
            }
        }
        else
        {
        #endif
            while ( src <= src_line_right )
            {
                /* Cast to an appropriate type */
                *(uint32_t*) dst = *(uint32_t*) src;
                src += byte_per_pixel;
                dst += byte_per_pixel;
            }

        #if  ENABLE_UNALIGNED_SoftBitBlt_ARGB8888_NoBlend_NoStretch
        }
        #endif

        if ( src_line_left == src_line_left_top )
        {
            break;
        }

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_ARGB8888_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftBitBlt_16BitColor_NoBlend_NoStretch 
************************************************************************/

#define  ENABLE_UNALIGNED_SoftBitBlt_16BitColor_NoBlend_NoStretch  (1) 
    /* 7.1->7.6msec (Debug, cached) */
    /* 5.2->5.0msec (Release, cached) */

errnum_t
    SoftBitBlt_16BitColor_NoBlend_NoStretch(  
    SoftBitBltFrameBufferClass*  SourceFrameBuffer,
    SoftBitBltFrameBufferClass*  DestinationFrameBuffer,
    SoftBitBltRectangleClass*    SourceRect,
    SoftBitBltRectangleClass*    DestinationRect,
    SoftBitBltFlags  Flags )
{
    errnum_t  e;
    uint8_t*  src; 
    uint8_t*  dst; 
    uint8_t*  src_line_head; 
    uint8_t*  dst_line_head; 
    uint8_t*  src_line_left; 
    uint8_t*  dst_line_left; 
    uint8_t*  src_line_head_top; 
    uint8_t*  src_line_left_top; 
    uint8_t*  src_line_right; 
    int_fast32_t  src_left_offset;
    int_fast32_t  dst_left_offset;
    int_fast32_t  src_right_offset;
    int_fast32_t  src_width;


    #ifdef  __CC_ARM
    #pragma push
    #pragma diag_suppress 4017  /* C4017W  No problem, because union */
    #endif

    int_fast32_t  width_remainder;  /* C4017W,  No problem, Because #if and if */
    bool_t        is_src_left_odd;  /* C4017W,  No problem, Because #if */
    bool_t        is_dst_left_odd;  /* C4017W,  No problem, Because #if */

    #ifdef  __CC_ARM
    #pragma pop
    #endif

    enum    { byte_per_pixel = 2 }; 


    #if  CHECK_ARG
    {
        bool_t  is_overlap;
        int_fast32_t  src_height;
        int_fast32_t  dst_height;

        e= SoftBitBlt_checkRectangle( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
            &src_height, &dst_height );
        IF(e)
        {
            goto fin;
        }

        ASSERT_R 
        ( SourceRect->Right - SourceRect->Left == DestinationRect->Right - DestinationRect->Left,
            e=E_RGA_OTHER_B; goto fin );  /* Error: <Private/> */
        ASSERT_R 
        ( src_height == dst_height, e=E_RGA_OTHER_C; goto fin );  /* Error: <Private/> */

        e= SoftBitBlt_isOverlapFrameBuffer( SourceFrameBuffer, DestinationFrameBuffer, &is_overlap );
        IF(e)
        {
            goto fin;
        }
        ASSERT_R
        ( ! is_overlap, e=E_RGA_OTHER_D; goto fin );  /* Error: <Private/> */
    }
    #endif

    SoftBitBltSub_getLineHeads( SourceFrameBuffer, DestinationFrameBuffer, SourceRect, DestinationRect,
        &src_line_head,  &src_line_head_top,  &dst_line_head );

    src_left_offset =  SourceRect->Left      * byte_per_pixel;
    dst_left_offset =  DestinationRect->Left * byte_per_pixel;
    src_right_offset = SourceRect->Right     * byte_per_pixel;

    src_line_left      = src_line_head     + src_left_offset;
    src_line_left_top  = src_line_head_top + src_left_offset;
    dst_line_left      = dst_line_head     + dst_left_offset;
    src_width          = src_right_offset  - src_left_offset;

    #if  ENABLE_UNALIGNED_SoftBitBlt_16BitColor_NoBlend_NoStretch
    if ( IS_BIT_SET( Flags, SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS ) )
    {
        is_src_left_odd = ( R_Mod_2s( SourceRect->Left ) != 0 );
        is_dst_left_odd = ( R_Mod_2s( DestinationRect->Left ) != 0 );

        if ( is_src_left_odd  &&  is_dst_left_odd )
        {
            src_line_left += byte_per_pixel;
            dst_line_left += byte_per_pixel;
            src_line_left_top += byte_per_pixel;
            src_width -= byte_per_pixel;
        }

        if ( is_src_left_odd == is_dst_left_odd )
        {
            width_remainder = R_Mod_8s( src_width + byte_per_pixel );  /* 8 = "byte_per_pixel * 4" */
            src_width -= width_remainder;
        }
    }
    #endif

    for (;;)
    {
        src = src_line_left;
        dst = dst_line_left;
        src_line_right = src_line_left + src_width;

        #if  ENABLE_UNALIGNED_SoftBitBlt_16BitColor_NoBlend_NoStretch
        if ( IS_BIT_SET( Flags, SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS )  &&
            (is_src_left_odd == is_dst_left_odd) )
        {
            if ( is_dst_left_odd )
            {
                /* Cast to an appropriate type */
                *( ((uint16_t*) dst) - 1 ) = *( ((uint16_t*) src) - 1 );
            }

            while ( src <= src_line_right )
            {
                /* Cast to an appropriate type */
                *(uint64_t*) dst = *(uint64_t*) src;

                    /* This code may be done with unaligned access */
                    /* But "uint64_t" can be accecced by 32bit alignment */
                    /*     in cached/uncached area */
                    /* "uint64_t" can not be accecced by 16bit alignment */
                    /*     in cached/uncached area */
                src += (byte_per_pixel * 4);
                dst += (byte_per_pixel * 4);
            }

            src_line_right += width_remainder;
        }
        #endif

        while ( src <= src_line_right )
        {
            /* Cast to an appropriate type */
            *(uint16_t*) dst = *(uint16_t*) src;
            src += byte_per_pixel;
            dst += byte_per_pixel;
        }

        if ( src_line_left == src_line_left_top )
        {
            break;
        }

        src_line_left += SourceFrameBuffer->stride;
        dst_line_left += DestinationFrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftBitBlt_16BitColor_NoBlend_NoStretch */

/***********************************************************************
* Function Name: SoftFillRectangle_ARGB8888 
************************************************************************/
errnum_t  SoftFillRectangle_ARGB8888(  
    SoftBitBltFrameBufferClass*  FrameBuffer,
    SoftBitBltRectangleClass*    Rectangle,
    argb8888_t  ColorARGB8888,  bit_flags_fast32_t Flags )
{
    errnum_t  e;
    uint8_t*  pixel; 
    uint8_t*  line_head; 
    uint8_t*  line_left; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  line_head_top; 
        uint8_t*  line_left_top; 
    #else
        uint8_t*  line_head_bottom; 
        uint8_t*  line_left_bottom; 
    #endif
    uint8_t*  line_right; 
    int       left_offset; 
    int       right_offset; 
    int       width; 
    const int 
        byte_per_pixel = 4; 

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( Flags );

    #if  CHECK_ARG
    {
        int   height; 

        e= SoftFillRectangle_checkRectangle( FrameBuffer, Rectangle, &height );
        IF(e)
        {
            goto fin;
        }
    }
    #endif

    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftFillRectangleSub_getLineHeads( FrameBuffer, Rectangle, &line_head, &line_head_top );
    #else
        SoftFillRectangleSub_getLineHeads( FrameBuffer, Rectangle, &line_head, &line_head_bottom );
    #endif

    left_offset  = Rectangle->Left  * byte_per_pixel;
    right_offset = Rectangle->Right * byte_per_pixel;

    line_left      = line_head     + left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        line_left_top  = line_head_top + left_offset;
    #else
        line_left_bottom  = line_head_bottom + left_offset;
    #endif
    width          = right_offset  - left_offset;

    for (;;)
    {

        pixel = line_left;
        line_right = line_left + width;

        while ( pixel <= line_right )
        {
            /* Cast to an appropriate type */
            *(uint32_t*) pixel = ColorARGB8888.Value;
            pixel += byte_per_pixel;
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( line_left == line_left_top )
            {
                break;
            }
        #else
            if ( line_left == line_left_bottom )
            {
                break;
            }
        #endif

        line_left += FrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftFillRectangle_ARGB8888 */

/***********************************************************************
* Function Name: SoftFillRectangle_16BitColor 
************************************************************************/
errnum_t  SoftFillRectangle_16BitColor( 
    SoftBitBltFrameBufferClass*  FrameBuffer,
    SoftBitBltRectangleClass*    Rectangle,
    uint16_t  Color16,  bit_flags_fast32_t Flags )
{
    errnum_t  e;
    uint8_t*  pixel; 
    uint8_t*  line_head; 
    uint8_t*  line_left; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  line_head_top; 
        uint8_t*  line_left_top; 
    #else
        uint8_t*  line_head_bottom; 
        uint8_t*  line_left_bottom; 
    #endif
    uint8_t*  line_right; 
    int       left_offset; 
    int       right_offset; 
    int       width; 
    uint32_t  color32 = Color16 | ( Color16 << 16 );
    bool_t    is_left_odd;
    bool_t    is_right_even;
    const int  
        byte_per_pixel = 2;

    #if  CHECK_ARG
    {
        int   height; 

        e= SoftFillRectangle_checkRectangle( FrameBuffer, Rectangle, &height );
        IF(e)
        {
            goto fin;
        }
    }
    #endif

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( Flags );

    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftFillRectangleSub_getLineHeads( FrameBuffer, Rectangle, &line_head, &line_head_top );
    #else
        SoftFillRectangleSub_getLineHeads( FrameBuffer, Rectangle, &line_head, &line_head_bottom );
    #endif

    left_offset  = Rectangle->Left  * byte_per_pixel;
    right_offset = Rectangle->Right * byte_per_pixel;

    line_left      = line_head     + left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        line_left_top  = line_head_top + left_offset;
    #else
        line_left_bottom  = line_head_bottom + left_offset;
    #endif
    width          = right_offset  - left_offset;

    /* Cast to an appropriate type */
    is_left_odd   = (  (uintptr_t) Rectangle->Left  & 1 );

    /* Cast to an appropriate type */
    is_right_even = ( (~(uintptr_t) Rectangle->Right) & 1 );

    for (;;)
    {

        pixel = line_left;
        line_right = line_left + width;

        if ( is_left_odd )
        {
            /* Cast to an appropriate type */
            *(uint16_t*) pixel = Color16;
            pixel += byte_per_pixel;
        }

        while ( pixel <= (line_right - 2) )
        {
            /* Cast to an appropriate type */
            *(uint32_t*) pixel = color32;
            pixel += (byte_per_pixel * 2);
        }

        if ( is_right_even )
        {
            /* Cast to an appropriate type */
            *(uint16_t*) pixel = Color16;
            pixel += byte_per_pixel;
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( line_left == line_left_top )
            {
                break;
            }
        #else
            if ( line_left == line_left_bottom )
            {
                break;
            }
        #endif

        line_left += FrameBuffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftFillRectangle_16BitColor */

/***********************************************************************
* Function Name: SoftFillRectangle_YUV422 
************************************************************************/
LIB_STATIC errnum_t  SoftFillRectangle_YUV422(     
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle,
    yuv422_t  ColorYUV422,  bit_flags_fast32_t flags )
{
    errnum_t  e;
    uint8_t*  pixel; 
    uint8_t*  line_head; 
    uint8_t*  line_left; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  line_head_top; 
        uint8_t*  line_left_top; 
    #else
        uint8_t*  line_head_bottom; 
        uint8_t*  line_left_bottom; 
    #endif
    uint8_t*  line_right; 
    int       left_offset; 
    int       right_offset; 
    int       width; 
    const int  
        byte_per_pixel = 2; 

    #if  CHECK_ARG
    {
        int   height; 

        e= SoftFillRectangle_checkRectangle( frame_buffer, Rectangle, &height );
        IF(e)
        {
            goto fin;
        }
    }
    #endif

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( flags );

    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftFillRectangleSub_getLineHeads( frame_buffer, Rectangle, &line_head, &line_head_top );
    #else
        SoftFillRectangleSub_getLineHeads( frame_buffer, Rectangle, &line_head, &line_head_bottom );
    #endif

    /* Cast to an appropriate type */
    ASSERT_R( R_Mod_2u( (unsigned) Rectangle->Left  ) == 0, goto err ); 

    /* Cast to an appropriate type */
    ASSERT_R( R_Mod_2u( (unsigned) Rectangle->Right ) == 1, goto err ); 

    left_offset  = Rectangle->Left  * byte_per_pixel;
    right_offset = Rectangle->Right * byte_per_pixel;

    line_left      = line_head     + left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        line_left_top  = line_head_top + left_offset;
    #else
        line_left_bottom  = line_head_bottom + left_offset;
    #endif
    width          = right_offset  - left_offset;

    for (;;)
    {

        pixel = line_left;
        line_right = line_left + width;

        while ( pixel <= line_right )
        {
            /* Cast to an appropriate type */
            *(uint32_t*) pixel = ColorYUV422.Value;
            pixel += (byte_per_pixel * 2);
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( line_left == line_left_top )
            {
                break;
            }
        #else
            if ( line_left == line_left_bottom )
            {
                break;
            }
        #endif

        line_left += frame_buffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
err:
    {
        e = E_RGA_OTHER_E;
    }
    goto fin;  /* Error: <Private/> */
} /* End of function SoftFillRectangle_YUV422 */

/***********************************************************************
* Function Name: SoftFillRectangle_ARGB8888_BlendByAlpha 
************************************************************************/
LIB_STATIC errnum_t  SoftFillRectangle_ARGB8888_BlendByAlpha( 
    SoftBitBltFrameBufferClass*  frame_buffer,
    SoftBitBltRectangleClass*    Rectangle,
    int  Alpha8,  SoftFillRectangleBlendFlags flags )
{
    errnum_t  e;
    uint8_t*  alpha_address; 
    int       pixel_alpha; 
    int       alpha8_ex;  /* 8 is 8bit */ 
    uint8_t*  line_head; 
    uint8_t*  line_left; 
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        uint8_t*  line_head_top; 
        uint8_t*  line_left_top; 
    #else
        uint8_t*  line_head_bottom; 
        uint8_t*  line_left_bottom; 
    #endif
    uint8_t*  line_right; 
    int       left_offset; 
    int       right_offset; 
    int       width; 
    enum {  byte_per_pixel = 4 }; 
    enum {  num_3_offset_to_alpha = 3 }; 

    #if  CHECK_ARG
    {
        int   height; 

        e= SoftFillRectangle_checkRectangle( frame_buffer, Rectangle, &height );
        IF(e)
        {
            goto fin;
        }
        ASSERT_R 
        ( Alpha8 >= 0  &&  Alpha8 <= 255,  e=E_RGA_OTHER_F; goto fin );  /* Error: <Private/> */
        IF ( (flags & SOFT_FILL_RECTANGLE_BLEND_DESTINATION_OUT) == 0 )
            { e=E_RGA_OTHER_10; goto fin; }  /* Error: <Private/> */
    }
    #endif

    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        SoftFillRectangleSub_getLineHeads( frame_buffer, Rectangle, &line_head, &line_head_top );
    #else
        SoftFillRectangleSub_getLineHeads( frame_buffer, Rectangle, &line_head, &line_head_bottom );
    #endif

    left_offset  = Rectangle->Left  * byte_per_pixel;
    right_offset = Rectangle->Right * byte_per_pixel;

    line_left = line_head + left_offset;
    #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
        line_left_top = line_head_top + left_offset;
    #else
        line_left_bottom = line_head_bottom + left_offset;
    #endif
    width = right_offset - left_offset;
    alpha8_ex = Alpha8;
    if ( Alpha8 >= 128 )
    {
        alpha8_ex = Alpha8 + 1;
    }

    for (;;)
    {
        alpha_address = line_left + num_3_offset_to_alpha;
        line_right = line_left + width;

        while ( alpha_address <= line_right )
        {
            /* Cast to an appropriate type */
            pixel_alpha = *(uint32_t*) alpha_address;

            /* Cast to an appropriate type */
            pixel_alpha = shift_right( pixel_alpha * alpha8_ex, 8 );

            /* = pixel_alpha * alpha8_ex / 256 */
            *(uint32_t*) alpha_address = pixel_alpha;

            alpha_address += byte_per_pixel;
        }

        #ifdef  IFDEF_SOFT_BIT_BLT_INVERT_STRIDE
            if ( line_left == line_left_top )
            {
                break;
            }
        #else
            if ( line_left == line_left_bottom )
            {
                break;
            }
        #endif

        line_left += frame_buffer->stride;
    }

    e=0;
fin:
    {
        return  e;
    }
} /* End of function SoftFillRectangle_ARGB8888_BlendByAlpha */

/*=================================================================*/
/* <<< [BasicTypes/BasicTypes.c] >>> */ 
/*=================================================================*/

/***********************************************************************
* Function Name: minus_mod_int_equal_plus 
************************************************************************/
LIB_STATIC int  minus_mod_int_equal_plus( int LeftValue, int RightValue )
{
    if ( LeftValue >= 0 )
    {
        return  LeftValue % RightValue;
    }
    else
    {
        return  ( (RightValue-1) + ( (LeftValue+1) % RightValue) );
    }
} /* End of function minus_mod_int_equal_plus */

/***********************************************************************
* Function Name: UIntPtrStartOverType_isOverlap
************************************************************************/

LIB_STATIC errnum_t  UIntPtrStartOverType_isOverlap( 
    UIntPtrStartOverType* A,  UIntPtrStartOverType* B,  bool_t* out_IsOverlap )
{
    ASSERT_R 
    ( A->Start <= A->Over, return  E_RGA_OTHER_11 );  /* Error: <Private/> */
    ASSERT_R 
    ( B->Start <= B->Over, return  E_RGA_OTHER_12 );  /* Error: <Private/> */

    if ( ((B->Start >= A->Start)  &&  (B->Start <  A->Over))  ||
        ((B->Over  >  A->Start)  &&  (B->Over  <= A->Over))  ||
        ((A->Start >= B->Start)  &&  (A->Start <  B->Over))  ||
        ((A->Over  >  B->Start)  &&  (A->Over  <= B->Over)) )
        { *out_IsOverlap = true; }
    else
        { *out_IsOverlap = false; }
    return  0;
} /* End of function UIntPtrStartOverType_isOverlap */

/***********************************************************************
* Function Name: IntBoxType_and 
************************************************************************/
errnum_t  IntBoxType_and( IntBoxType* out_Box, 
    const IntBoxType* BoxA, const IntBoxType* BoxB )
{
    int  box_A_min_x  = BoxA->MinX;  
    int  box_A_min_y  = BoxA->MinY;  
    int  box_A_over_x 
            = BoxA->MinX + BoxA->Width;
    int  box_A_over_y  
            = BoxA->MinY + BoxA->Height;
    int  box_B_min_x  = BoxB->MinX;  
    int  box_B_min_y  = BoxB->MinY;  
    int  box_B_over_x  
            = BoxB->MinX + BoxB->Width;
    int  box_B_over_y   
            = BoxB->MinY + BoxB->Height;
#ifndef  R_OSPL_NDEBUG
    errnum_t  e;
#endif

    ASSERT_D( BoxA->Width  >= 0,  e=E_RGA_OTHER_13; goto fin );  /* Error: <Private/> */
    ASSERT_D( BoxA->Height >= 0,  e=E_RGA_OTHER_14; goto fin );  /* Error: <Private/> */
    ASSERT_D( BoxB->Width  >= 0,  e=E_RGA_OTHER_15; goto fin );  /* Error: <Private/> */
    ASSERT_D( BoxB->Height >= 0,  e=E_RGA_OTHER_16; goto fin );  /* Error: <Private/> */

        /* Internal error */
        /* For the following judgement */

#ifndef  R_OSPL_NDEBUG
    e = 0;
#endif


    /* Cut by "box_B_min_x" */
    if ( box_B_min_x > box_A_min_x )
    {
        if ( box_B_min_x >= box_A_over_x )
        {
            IntBoxType_setEmpty( out_Box );
            goto fin;
        }
        else
        {
            box_A_min_x = box_B_min_x;
        }
    }

    /* Cut by "box_B_min_y" */
    if ( box_B_min_y > box_A_min_y )
    {
        if ( box_B_min_y >= box_A_over_y )
        {
            IntBoxType_setEmpty( out_Box );
            goto fin;
        }
        else
        {
            box_A_min_y = box_B_min_y;
        }
    }

    /* Cut by "box_B_over_x" */
    if ( box_B_over_x < box_A_over_x )
    {
        if ( box_B_over_x <= box_A_min_x )
        {
            IntBoxType_setEmpty( out_Box );
            goto fin;
        }
        else
        {
            box_A_over_x = box_B_over_x;
        }
    }

    /* Cut by "box_B_over_y" */
    if ( box_B_over_y < box_A_over_y )
    {
        if ( box_B_over_y <= box_A_min_y )
        {
            IntBoxType_setEmpty( out_Box );
            goto fin;
        }
        else
        {
            box_A_over_y = box_B_over_y;
        }
    }

    /* If zero width or height */
    if ( (box_A_min_x >= box_A_over_x)  ||  (box_A_min_y >= box_A_over_y) )
    {
        IntBoxType_setEmpty( out_Box );
        goto fin;
    }

    /* Set "out_Box" */
    out_Box->MinX = box_A_min_x;
    out_Box->MinY = box_A_min_y;
    out_Box->Width  = box_A_over_x - box_A_min_x;
    out_Box->Height = box_A_over_y - box_A_min_y;

fin:
#ifndef  R_OSPL_NDEBUG
    {
        return  e;
    }
#else
    return  0;
#endif
} /* End of function IntBoxType_and */

/*=================================================================*/
/* <<< [SetX/SetX.c] >>> */ 
/*=================================================================*/

/***********************************************************************
* Function Name: memset32 
************************************************************************/
LIB_STATIC void  memset32( void* ArrayFirst, unsigned Value, size_t ArrayByteSize )
{
    /* Cast to an appropriate type */
    unsigned*  p = (unsigned*) ArrayFirst; 


    unsigned*  p_over 

                    /* Cast to an appropriate type */
                    = (unsigned*)( (uintptr_t)ArrayFirst + (ArrayByteSize & (~3)) );

    while ( p < p_over )
    {
        *p = Value;
        p++;
    }
} /* End of function memset32 */

