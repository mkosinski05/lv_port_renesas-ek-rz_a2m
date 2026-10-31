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
* File        : rga_rz_a2m_rgas.c
* Description : RGA wrapper for RGAS.
*               Main function is "R_GRAPHICS_S_Initialize".
* Version     : 1.12
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include  "use_config_rga.h"
#include  "clib_drivers.h"
#include  "frame_buffer_inline.h"
#include  "frame_buffer_format_typedef.h"
#include  "r_drw_pl.h"
#include  "rz_co.h"
#include  "rga_api.h"
#include  "rga_inside.h"
#include  "rga_local.h"
#include  "rga_raw_image.h"
#include  "rga_rz_a2m_rgas.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define  FASTER_YUV422Type_to_ARGB8888Type  (1) 

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Struct: GraphicsIntTranslateType 
************************************************************************/
typedef struct st_GraphicsIntTranslateType  GraphicsIntTranslateType; 
struct st_GraphicsIntTranslateType { 
    bool_t        IsEnabled; 
    int_fast32_t  tx;
    int_fast32_t  ty;
};


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static errnum_t  gs_SetFrameBuffer_Sub( graphics_t* self );
static errnum_t  gs_FillRect_Sub( graphics_t*  self,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height,
    bool_t  in_IsClear );
static errnum_t  gs_ChangePixelFormat( uint32_t InputColor, pixel_format_t InputFormat,
    uint32_t* out_Color, pixel_format_t OutputFormat );
static void  gs_YUV422Type_to_ARGB8888Type( yuv422_t SourceColors, argb8888_t out_DestinationColors[2] );
static yuv422_t  gs_ARGB8888Type_to_YUV422Type( argb8888_t Colors[2] );
static errnum_t  gs_drawImageChildR8G8B8A8( graphics_t* self,  const graphics_image_t* image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight );


/***********************************************************************
* Function Name: GraphicsClass_getTranslateMatrixI
************************************************************************/
static errnum_t
    GraphicsClass_getTranslateMatrixI 
        ( graphics_t* self,  GraphicsIntTranslateType* out )
{
    errnum_t  R_GRAPHICS_H_GetMatrixIntegerTranslate( graphics_t*  self,  GraphicsIntTranslateType*  out );

    return  R_GRAPHICS_H_GetMatrixIntegerTranslate( self, out );
}/* End of function GraphicsClass_getTranslateMatrixI */


/***********************************************************************
* Function Name: R_GRAPHICS_S_Initialize
************************************************************************/
errnum_t  R_GRAPHICS_S_Initialize 
            ( graphics_t*  self,  graphics_config_t*  in_out_Config )
{
    errnum_t  e;

    if (!( self->RGAS_State == 0 ))
    {
        e=E_RGA_STATE_2;
        goto fin;
    }

        /* Error: "graphics_t::RGAS_State" variable is not initialized by 0 initializer. */

    /* Set "self->frame_buffer" */
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_FRAME_BUFFER ) )
    {
        e=E_RGA_OTHER_DC;
        goto fin;
    } /* Error: Not set "F_GRAPHICS_FRAME_BUFFER" */

    /* Cast to an appropriate type */
    if ( in_out_Config->frame_buffer == NULL )
    {
        e=E_RGA_OTHER_DD;
        goto fin;
    } /* Error: Not set "graphics_config_t::frame_buffer" */

    self->RGAS_State = GRAPHICS_STATE_INITIALIZED;
    self->FrameBuffer = in_out_Config->frame_buffer;

    /* Cast to an appropriate type */
    self->CurrentFrameBufferUserAddress = NULL;

    /* Cast to an appropriate type */
    self->CurrentFrameBufferCachedAddress = NULL;

    e= gs_SetFrameBuffer_Sub( self );
    if ( ( e & E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_MASK ) == E_RGA_NOT_SUPPORTED_PIXEL_FORMAT )
    {
        R_ERROR_Set( E_RGA_STATE_CLEAR_2 );
        e=0;
    }  /* Error: <Private/> */
    if(e)
    {
        goto fin;
    }

    /* Set default settings */
    if ( self->IsSupportedPixelFromatForRGAS )
    {
        e= R_GRAPHICS_S_SetBackgroundColor( self,  R_RGA_Get_R8G8B8A8( 255,255,255,0 ) );
        if(e)
        {
            goto fin;
        }
    }

    e=0;
fin:
    if ( e )
    {
        R_GRAPHICS_S_Finalize( self,  e );
    }
    if ( ! self->IsSupportedPixelFromatForRGAS )
    {
        e= E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_44;

        /* Error: The pixel format of frame buffer is not supported. */
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_S_Initialize */


/***********************************************************************
* Function Name: R_GRAPHICS_S_Finalize
************************************************************************/
errnum_t  R_GRAPHICS_S_Finalize( graphics_t* self,  errnum_t e ) 
{
    self->RGAS_State = GRAPHICS_STATE_UNINITIALIZED;
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_S_Finalize */


/***********************************************************************
* Function Name: R_GRAPHICS_S_SetFrameBuffer
************************************************************************/
errnum_t  R_GRAPHICS_S_SetFrameBuffer 
            ( graphics_t*  self,  frame_buffer_t*  in_FrameBuffer )
{
    self->FrameBuffer = in_FrameBuffer;

    /* Cast to an appropriate type */
    self->CurrentFrameBufferCachedAddress = NULL;  /* For in next function */
    return  gs_SetFrameBuffer_Sub( self );
}/* End of function R_GRAPHICS_S_SetFrameBuffer */


/***********************************************************************
* Function Name: gs_SetFrameBuffer_Sub
************************************************************************/
static errnum_t  gs_SetFrameBuffer_Sub( graphics_t* self ) 
{
    errnum_t           e;
    frame_buffer_t*    frame = self->FrameBuffer; 
    uint8_t*frame_buffer_address 
                    = frame->buffer_address[ frame->draw_buffer_index ];
    uint8_t*           frame_buffer_cached_address; 

    self->CurrentFrameBufferUserAddress = frame_buffer_address;

    e= R_DRW_ToCachedAddress( frame_buffer_address, &frame_buffer_cached_address );
    ASSERT_R( e == 0,  e=E_RGA_ACCESS_DENIED_15; goto fin ); 

    /* Error: The memory of frame buffer must be cached memory. */

    if ( frame_buffer_cached_address == self->CurrentFrameBufferCachedAddress )
    {
        return  0;
    }

    self->CurrentFrameBufferCachedAddress = frame_buffer_cached_address;

    self->ClippingRectangle.MinX   = 0;
    self->ClippingRectangle.MinY   = 0;
    self->ClippingRectangle.Width  = frame->width;
    self->ClippingRectangle.Height = frame->height;

    self->IsSupportedPixelFromatForRGAS = (
        ( frame->pixel_format == PIXEL_FORMAT_ARGB8888 ) ||
        ( frame->pixel_format == PIXEL_FORMAT_XRGB8888 ) ||
        ( frame->pixel_format == PIXEL_FORMAT_RGB565   ) ||
        ( frame->pixel_format == PIXEL_FORMAT_ARGB1555 ) ||
        ( frame->pixel_format == PIXEL_FORMAT_ARGB4444 ) );

    if ( ! self->IsSupportedPixelFromatForRGAS )
    {
        e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_43;
        goto fin;
    }

    /* Error: The pixel format of frame buffer is not supported. */

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_SetFrameBuffer_Sub */


/***********************************************************************
* Function Name: R_GRAPHICS_S_DrawImage
************************************************************************/
errnum_t  R_GRAPHICS_S_DrawImage
    ( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY )
{
    errnum_t  e;
    frame_buffer_t*              frame = self->FrameBuffer; 
    graphics_image_properties_t  image_prop;

    e= R_GRAPHICS_IMAGE_GetProperties( in_Image,  &image_prop );
    if(e)
    {
        goto fin;
    }


    if ( image_prop.pixelFormat == PIXEL_FORMAT_R8G8B8A8 )
    {
        e= gs_drawImageChildR8G8B8A8( self,  in_Image,

            /* Cast to an appropriate type */
            0, 0,               (int_fast32_t) image_prop.width,  (int_fast32_t) image_prop.height,

            /* Cast to an appropriate type */
            in_MinX,  in_MinY,  (int_fast32_t) image_prop.width,  (int_fast32_t) image_prop.height );
            if(e)
            {
                goto fin;
            }
    }
    else
    {
        SoftBitBltFrameBufferClass   source_frame;
        SoftBitBltFrameBufferClass   destination_frame;
        SoftBitBltRectangleClass     source_rect;
        SoftBitBltRectangleClass     destination_rect;
        SoftBitBltRectangleClass     clipped_rect;

        source_frame.Address = image_prop.pixels;
        source_frame.width   = image_prop.width;
        source_frame.height  = image_prop.height;
        if ( IS_BIT_SET( in_Image->flags,  F_T_IMAGE_INF_PADDING_AT_RIGHT ) )
        {
            source_frame.stride  = in_Image->color * frame->byte_per_pixel;
        }
        else
        {
            source_frame.stride  = source_frame.width * frame->byte_per_pixel;
        }
        source_frame.stride  = R_Ceil_4s( source_frame.stride );
        destination_frame.Address = self->CurrentFrameBufferCachedAddress;
        destination_frame.width   = frame->width;
        destination_frame.height  = frame->height;
        destination_frame.stride  = frame->stride;
        source_rect.Left   = 0;
        source_rect.Top    = 0;
        source_rect.Right  = image_prop.width - 1;
        source_rect.Bottom = image_prop.height - 1;
        destination_rect.Left   = in_MinX;
        destination_rect.Top    = in_MinY;
        destination_rect.Right  = (in_MinX + image_prop.width)  - 1;
        destination_rect.Bottom = (in_MinY + image_prop.height) - 1;
        clipped_rect.Left   = self->ClippingRectangle.MinX;
        clipped_rect.Top    = self->ClippingRectangle.MinY;
        clipped_rect.Right  = (self->ClippingRectangle.MinX + self->ClippingRectangle.Width)  - 1;
        clipped_rect.Bottom = (self->ClippingRectangle.MinY + self->ClippingRectangle.Height) - 1;

        e= SoftBitBlt_toInClippedFrame( &source_frame,  &destination_frame,
            &source_rect,  &destination_rect,  &clipped_rect );
        IF(e)
        {
            goto fin;
        }
        if ( source_rect.Left > source_rect.Right )
        {
            /* Do Nothing */
            ;
        }
        else if ( frame->byte_per_pixel == 2 )
        {
            e= R_GRAPHICS_BeginCachedRendering( self );
            IF(e)
            {
                goto fin;
            }

            e= SoftBitBlt_16BitColor_NoBlend_NoStretch( &source_frame,  &destination_frame,
                &source_rect,  &destination_rect,
                SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS );
            IF(e)
            {
                goto fin;
            }
            e= R_GRAPHICS_EndCachedRendering( self );
            IF(e)
            {
                goto fin;
            }
        }
        else
        {
            ASSERT_R 
                ( frame->byte_per_pixel == 4,  e=E_RGA_OTHER_AB; goto fin );  /* Error: <Private/> */
            e= R_GRAPHICS_BeginCachedRendering( self );
            IF(e)
            {
                goto fin;
            }

            e= SoftBitBlt_ARGB8888_NoBlend_NoStretch( &source_frame,  &destination_frame,
                &source_rect,  &destination_rect,
                SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS );
            IF(e)
            {
                goto fin;
            }
            e= R_GRAPHICS_EndCachedRendering( self );
            IF(e)
            {
                goto fin;
            }
        }
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_GRAPHICS_S_DrawImage */


/***********************************************************************
* Function Name: R_GRAPHICS_S_DrawImageChild
*    R_GRAPHICS_S_DrawImageChild no resize version
************************************************************************/
errnum_t  R_GRAPHICS_S_DrawImageChild  
    ( graphics_t* self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight )
{
    errnum_t  e;
    frame_buffer_t*              frame = self->FrameBuffer; 
    graphics_image_properties_t  image_prop;


    e= R_GRAPHICS_IMAGE_GetProperties( in_Image,  &image_prop );
    IF(e)
    {
        goto fin;
    }

    if ( image_prop.pixelFormat == PIXEL_FORMAT_R8G8B8A8 )
    {
        e= gs_drawImageChildR8G8B8A8( self,  in_Image,
            in_SourceMinX,       in_SourceMinY,       in_SourceWidth,       in_SourceHeight,
            in_DestinationMinX,  in_DestinationMinY,  in_DestinationWidth,  in_DestinationHeight );
        if(e)
        {
            goto fin;
        }
    }
    else
    {
        SoftBitBltFrameBufferClass   source_frame;
        SoftBitBltFrameBufferClass   destination_frame;
        SoftBitBltRectangleClass     source_rect;
        SoftBitBltRectangleClass     destination_rect;
        SoftBitBltRectangleClass     clipped_rect;

        ASSERT_R 
            ( image_prop.pixelFormat == frame->pixel_format,  e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_46; goto fin );

        /* Error: <E_NOT_SUPPORTED_PIXEL_FORMAT/> */

        if ( image_prop.pixelFormat == PIXEL_FORMAT_YUV422 )
        {
            ASSERT_R 
            ( R_Mod_2s( in_DestinationMinX  ) == 0, e=E_RGA_OTHER_FD; goto fin );

            /* Error: X coodinate is not multiple of 2 in YUV422. */

            ASSERT_R 
            ( R_Mod_2s( in_DestinationWidth ) == 0, e=E_RGA_OTHER_FE; goto fin );

            /* Error: The width is not multiple of 2 in YUV422. */
        }


        source_frame.Address = image_prop.pixels;
        source_frame.width   = image_prop.width;
        source_frame.height  = image_prop.height;

        if ( IS_BIT_SET( in_Image->flags, F_T_IMAGE_INF_PADDING_AT_RIGHT ) )
        {
            source_frame.stride  = in_Image->color * frame->byte_per_pixel;
        }
        else
        {
            source_frame.stride  = source_frame.width * frame->byte_per_pixel;
        }
        source_frame.stride  = R_Ceil_4s( source_frame.stride );

        source_rect.Left   = in_SourceMinX;
        source_rect.Top    = in_SourceMinY;
        source_rect.Right  = (in_SourceMinX + in_SourceWidth)  - 1;
        source_rect.Bottom = (in_SourceMinY + in_SourceHeight) - 1;

        destination_frame.Address = self->CurrentFrameBufferCachedAddress;
        destination_frame.width   = frame->width;
        destination_frame.height  = frame->height;
        destination_frame.stride  = frame->stride;

        destination_rect.Left   = in_DestinationMinX;
        destination_rect.Top    = in_DestinationMinY;
        destination_rect.Right  = (in_DestinationMinX + in_DestinationWidth)  - 1;
        destination_rect.Bottom = (in_DestinationMinY + in_DestinationHeight) - 1;

        clipped_rect.Left   = self->ClippingRectangle.MinX;
        clipped_rect.Top    = self->ClippingRectangle.MinY;
        clipped_rect.Right  = (self->ClippingRectangle.MinX + self->ClippingRectangle.Width)  - 1;
        clipped_rect.Bottom = (self->ClippingRectangle.MinY + self->ClippingRectangle.Height) - 1;

        e= SoftBitBlt_toInClippedFrame( &source_frame, &destination_frame,
            &source_rect, &destination_rect, &clipped_rect );
        IF(e)
        {
            goto fin;
        }
        if ( source_rect.Left > source_rect.Right )
        {
            /* Do Nothing */
            ;
        }
        else if ( frame->byte_per_pixel == 2 )
        {
            e= R_GRAPHICS_BeginCachedRendering( self );
            IF(e)
            {
                goto fin;
            }
            e= SoftBitBlt_16BitColor_NoBlend_NoStretch( &source_frame, &destination_frame,
                &source_rect, &destination_rect,
                SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS );
            IF(e)
            {
                goto fin;
            }
            e= R_GRAPHICS_EndCachedRendering( self );
            IF(e)
            {
                goto fin;
            }
        }
        else
        {
            ASSERT_R 
            ( frame->byte_per_pixel == 4,  e=E_RGA_OTHER_100; goto fin );

            e= R_GRAPHICS_BeginCachedRendering( self );
            IF(e)
            {
                goto fin;
            }
            e= SoftBitBlt_ARGB8888_NoBlend_NoStretch( &source_frame, &destination_frame,
                &source_rect, &destination_rect,
                SOFT_BIT_BLT_ENABLE_UNALIGNED_ACCESS );
            IF(e)
            {
                goto fin;
            }
            e= R_GRAPHICS_EndCachedRendering( self );
            IF(e)
            {
                goto fin;
            }
        }
    }

    e=0;
fin:
    {
        e= R_GRAPHICS_EndRenderingInFin( self, e );
    }
    return  e;
}/* End of function R_GRAPHICS_S_DrawImageChild */


/***********************************************************************
* Function Name: R_GRAPHICS_S_SetBackgroundColor
************************************************************************/
errnum_t  R_GRAPHICS_S_SetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t  in_Color )
{
    errnum_t        e;
    uint32_t        color;
    pixel_format_t  pixel_format;

    pixel_format = self->FrameBuffer->pixel_format;


    /* BackgroundColor = ... */
    self->BackgroundColor = in_Color;


    /* "in_Color.Value = 0", if "R_RGA_IsPixelFormatHavingAlpha" */
    {
        bool_t  is_alpha;

        e= R_RGA_IsPixelFormatHavingAlpha( pixel_format,  &is_alpha );
        IF(e)
        {
            goto fin;
        }

        if ( is_alpha )
        {
            in_Color.Value = 0;  /* transparent black */
        }
    }


    /* Clear16BitColor = ...,  Clear32BitColor = ... */
    if( ( pixel_format == PIXEL_FORMAT_CLUT8 ) || ( pixel_format == PIXEL_FORMAT_CLUT4 )
            || ( pixel_format == PIXEL_FORMAT_CLUT1 ) )
    {

        self->Clear16BitColor = in_Color.Value;
    }
    else if ( self->IsSupportedPixelFromatForRGAS )
    {
        e= gs_ChangePixelFormat( in_Color.Value,  PIXEL_FORMAT_R8G8B8A8,
            &color,  pixel_format );
        IF(e)
        {
            goto fin;
        }

        if ( self->FrameBuffer->byte_per_pixel == 2 )
        {
            if ( pixel_format == PIXEL_FORMAT_YUV422 )
            {
                self->Clear32BitColor.Value = color;
            }
            else
            {
                /* Cast to an appropriate type */
                self->Clear16BitColor = (uint16_t) color;
            }
        }
        else {
            self->Clear32BitColor.Value = color;
        }
    }
    else
    {
        /* DO NOTHING */
        ;
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }

    return  e;
}/* End of function R_GRAPHICS_S_SetBackgroundColor */


/***********************************************************************
* Function Name: R_GRAPHICS_S_GetBackgroundColor
************************************************************************/
errnum_t  R_GRAPHICS_S_GetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t*  out_Color )
{
    *out_Color = self->BackgroundColor;
    return  0;
}/* End of function R_GRAPHICS_S_GetBackgroundColor */


/***********************************************************************
* Function Name: R_GRAPHICS_S_GetClearColor
************************************************************************/
errnum_t  R_GRAPHICS_S_GetClearColor 
            ( graphics_t* self,  r8g8b8a8_t*  out_Color )
{
    errnum_t  e;
    uint32_t  color;

    pixel_format_t  pixel_format;


    /* out_Color = ... */
    pixel_format = self->FrameBuffer->pixel_format;
    if (
        (pixel_format == PIXEL_FORMAT_CLUT8)  ||
        (pixel_format == PIXEL_FORMAT_CLUT4)  ||
        (pixel_format == PIXEL_FORMAT_CLUT1) )
    {
        out_Color->Value = self->Clear16BitColor;
    }
    else
    {
        if ( self->FrameBuffer->byte_per_pixel == 2 )
        {
            if ( pixel_format == PIXEL_FORMAT_YUV422 )
            {
                color = self->Clear32BitColor.Value;
            }
            else
            {
                color = self->Clear16BitColor;
            }
        }
        else
        {
            color = self->Clear32BitColor.Value;
        }

        e= gs_ChangePixelFormat( color,  pixel_format,
            &out_Color->Value,  PIXEL_FORMAT_R8G8B8A8 );
        if(e)
        {
            goto fin;
        }

            /* out_Color->Value = . */
    }

    e=0;
fin:
    {
    R_ERROR_Set( e );
    }
    return  e;
}/* End of function R_GRAPHICS_S_GetClearColor */


/***********************************************************************
* Function Name: R_GRAPHICS_S_SetFillColor
************************************************************************/
errnum_t  R_GRAPHICS_S_SetFillColor( graphics_t* self,  r8g8b8a8_t  in_Color ) 
{
    errnum_t  e;
    uint32_t  color;
    pixel_format_t  pixel_format;


    /* Fill16BitColor = ...,  Fill32BitColor = ...,  FillAlpha = ... */
    pixel_format = self->FrameBuffer->pixel_format;
    if (
        (pixel_format == PIXEL_FORMAT_CLUT8)  ||
        (pixel_format == PIXEL_FORMAT_CLUT4)  ||
        (pixel_format == PIXEL_FORMAT_CLUT1) )
    {
        /* Cast to an appropriate type */
        self->Fill16BitColor = (uint16_t) in_Color.Value;
    }
    else if ( self->IsSupportedPixelFromatForRGAS )
    {
        e= gs_ChangePixelFormat( in_Color.Value,  PIXEL_FORMAT_R8G8B8A8,
            &color,  pixel_format );
        IF(e)
        {
            goto fin;
        }

        if ( self->FrameBuffer->byte_per_pixel == 2 )
        {
            if ( pixel_format == PIXEL_FORMAT_YUV422 )
            {
                self->Fill32BitColor.Value = color;
            }
            else
            {
                /* Cast to an appropriate type */
                self->Fill16BitColor = (uint16_t) color;
            }
        }
        else
        {
            self->Fill32BitColor.Value = color;
        }
    }
    else
    {
        /* DO NOTHING */
        ;
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
}/* End of function R_GRAPHICS_S_SetFillColor */


/***********************************************************************
* Function Name: R_GRAPHICS_S_Clear
************************************************************************/
errnum_t  R_GRAPHICS_S_Clear( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    return  gs_FillRect_Sub( self,  in_MinX,  in_MinY,  in_Width,  in_Height,  true );
}/* End of function R_GRAPHICS_S_Clear */


/***********************************************************************
* Function Name: R_GRAPHICS_S_FillRect
************************************************************************/
errnum_t  R_GRAPHICS_S_FillRect( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    return  gs_FillRect_Sub( self,  in_MinX,  in_MinY,  in_Width,  in_Height,  false );
}/* End of function R_GRAPHICS_S_FillRect */


/***********************************************************************
* Function Name: gs_FillRect_Sub
************************************************************************/
static errnum_t  gs_FillRect_Sub( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height,
    bool_t  in_IsClear )
{
    errnum_t  e;
    frame_buffer_t*  frame = self->FrameBuffer; 

    GraphicsIntTranslateType  int_translate;

    e= gs_SetFrameBuffer_Sub( self );
    if(e)
    {
        goto fin;
    }

    e= GraphicsClass_getTranslateMatrixI( self,  &int_translate );
    if(e)
    {
        goto fin;
    }


    /* "SoftFillRectangle_ARGB8888" */
    {
        SoftBitBltFrameBufferClass  blt_frame;
        SoftBitBltRectangleClass    blt_rect;
        IntBoxType                  clipped_rect;

        #ifdef  __CC_ARM
        #pragma push
        #pragma diag_suppress 4017
        #endif

        argb8888_t  color32;  /* C4017W,  No problem, Because "byte_per_pixel" */
        uint16_t    color16;  /* C4017W,  No problem, Because "byte_per_pixel" */

        #ifdef  __CC_ARM
        #pragma pop
        #endif


        e= R_GRAPHICS_BeginCachedRendering( self );
        IF(e)
        {
            goto fin;
        }

        if ( frame->byte_per_pixel == 4 )
        {
            if ( in_IsClear )
            {
                color32 = self->Clear32BitColor;
            }
            else
            {
                color32 = self->Fill32BitColor;
            }
        } else
        {
            ASSERT_R
                ( frame->byte_per_pixel == 2,  e=E_RGA_OTHER_C5; goto fin );  /* Error: <Private/> */

            if ( in_IsClear )
            {
                color16 = self->Clear16BitColor;
            }
            else
            {
                color16 = self->Fill16BitColor;
            }
        }

        blt_frame.Address = self->CurrentFrameBufferCachedAddress;
        blt_frame.width   = frame->width;
        blt_frame.stride  = frame->stride;
        blt_frame.height  = frame->height;

        clipped_rect.MinX   = in_MinX + int_translate.tx;
        clipped_rect.MinY   = in_MinY + int_translate.ty;
        clipped_rect.Width  = in_Width;
        clipped_rect.Height = in_Height;
        e= IntBoxType_and( &clipped_rect, &clipped_rect, &self->ClippingRectangle );
        IF(e)
        {
            goto fin;
        }

        blt_rect.Left = clipped_rect.MinX;
        blt_rect.Top  = clipped_rect.MinY;
        blt_rect.Right  = (clipped_rect.MinX + clipped_rect.Width) - 1;
        blt_rect.Bottom = (clipped_rect.MinY + clipped_rect.Height) - 1;

        e= SoftFillRectangle_toInFrame( &blt_frame, &blt_rect );
        IF(e)
        {
            goto fin;
        }
        if ( blt_rect.Left <= blt_rect.Right )
        {
            if ( frame->byte_per_pixel == 4 )
            {

                e= SoftFillRectangle_ARGB8888( &blt_frame,  &blt_rect,  color32, 0 );
                IF(e)
                {
                    goto fin;
                }
            } else
            {

                e= SoftFillRectangle_16BitColor( &blt_frame,  &blt_rect,  color16, 0 );
                IF(e)
                {
                    goto fin;
                }
            }
        }
        e= R_GRAPHICS_EndCachedRendering( self );
        IF(e)
        {
            goto fin;
        }
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
}/* End of function gs_FillRect_Sub */


/***********************************************************************
* Function Name: gs_drawImageChildR8G8B8A8
************************************************************************/
static errnum_t  gs_drawImageChildR8G8B8A8  
    ( graphics_t* self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight )
{
    errnum_t  e;
    SoftBitBltFrameBufferClass    source_frame;
    SoftBitBltFrameBufferClass    destinaion_frame;
    SoftBitBltRectangleClass      source_rect;
    SoftBitBltRectangleClass      destinaion_rect;
    frame_buffer_t*               frame = self->FrameBuffer; 
    graphics_image_properties_t   image_prop;

    e= R_GRAPHICS_IMAGE_GetProperties( in_Image, &image_prop );
    IF(e)
    {
        goto fin;
    }
    source_frame.Address = image_prop.pixels;
    source_frame.width   = image_prop.width;
    source_frame.height  = image_prop.height;
    source_frame.stride  = source_frame.width * 4;  /* 4 = R8G8B8A8 */

    destinaion_frame.Address = self->CurrentFrameBufferCachedAddress;
    destinaion_frame.width   = frame->width;
    destinaion_frame.height  = frame->height;
    destinaion_frame.stride  = frame->stride;

    source_rect.Left   = in_SourceMinX;
    source_rect.Top    = in_SourceMinY;
    source_rect.Right  = (in_SourceMinX + in_SourceWidth)  - 1;
    source_rect.Bottom = (in_SourceMinY + in_SourceHeight) - 1;

    destinaion_rect.Left   = in_DestinationMinX;
    destinaion_rect.Top    = in_DestinationMinY;
    destinaion_rect.Right  = (in_DestinationMinX + in_DestinationWidth)  - 1;
    destinaion_rect.Bottom = (in_DestinationMinY + in_DestinationHeight) - 1;

#if RGA_LIMITATION_11_SOURCE_FLUSH_B
    e= R_GRAPHICS_BeginSoftwareRenderingB( self, destinaion_frame.Address, source_frame.Address );
        IF(e){goto fin;}
    e= R_GRAPHICS_EndSoftwareRendering( self ); IF(e){goto fin;}
    e= R_GRAPHICS_BeginCachedRendering( self ); IF(e){goto fin;} /* self->T_Surface.Buf */
#else
    e= R_GRAPHICS_BeginCachedRendering( self );
    IF(e)
    {
        goto fin;
    } /* destinaion_frame.Address */
#endif

    switch ( frame->pixel_format )
    {
        case  PIXEL_FORMAT_ARGB8888:
            e= SoftBitBlt_R8G8B8A8_to_ARGB8888_NoBlend_NoStretch(
                &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
            IF(e)
            {
                goto fin;
            }
            break;

        case  PIXEL_FORMAT_XRGB8888:
            e= SoftBitBlt_R8G8B8A8_to_XRGB8888_NoBlend_NoStretch(
                &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
            IF(e)
            {
                goto fin;
            }
            break;

        case  PIXEL_FORMAT_RGB565:
            e= SoftBitBlt_R8G8B8A8_to_RGB565_NoBlend_NoStretch(
                &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
            IF(e)
            {
                goto fin;
            }
            break;

        case  PIXEL_FORMAT_ARGB1555:
            e= SoftBitBlt_R8G8B8A8_to_ARGB1555_NoBlend_NoStretch(
                &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
            IF(e)
            {
                goto fin;
            }
            break;

        case  PIXEL_FORMAT_ARGB4444:
            e= SoftBitBlt_R8G8B8A8_to_ARGB4444_NoBlend_NoStretch(
                &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
            IF(e)
            {
                goto fin;
            }
            break;

        default:
            ASSERT_R 
            ( frame->pixel_format == PIXEL_FORMAT_YUV422, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_45; goto fin );

            /* Error: <E_NOT_SUPPORTED_PIXEL_FORMAT/> */
            e= SoftBitBlt_R8G8B8A8_to_YUV422_NoBlend_NoStretch(
                &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
            IF(e)
            {
                goto fin;
            }
            break;
    }
    e= R_GRAPHICS_EndSoftwareRendering( self );
    IF(e)
    {
        goto fin;
    }

    e=0;
fin:
    {
        e= R_GRAPHICS_EndRenderingInFin( self, e );
    }
    return  e;
}/* End of function gs_drawImageChildR8G8B8A8 */


/***********************************************************************
* Function Name: R_GRAPHICS_S_Finish
************************************************************************/
errnum_t  R_GRAPHICS_S_Finish( graphics_t*  self ) 
{
    if ( self->SHS_IsRequestedCachedRendering )
    {
        frame_buffer_t*  frame = self->FrameBuffer; 
        uint8_t* start_address 
                    = frame->buffer_address[ frame->draw_buffer_index ];
        uint32_t         size = frame->stride * frame->height;

        self->SHS_IsRequestedCachedRendering = true;

        R_DRW_FlushCache( start_address,  size );
    }
    return  0;
}/* End of function R_GRAPHICS_S_Finish */


/***********************************************************************
* Function Name: gs_ChangePixelFormat
************************************************************************/
static errnum_t  gs_ChangePixelFormat  
    ( uint32_t InputColor, pixel_format_t InputFormat,
    uint32_t* out_Color, pixel_format_t OutputFormat )
{
    errnum_t      e;

#ifdef  __CC_ARM
#pragma push
#pragma diag_suppress 4017
#endif
#ifdef  argb8888_t
    argb8888_t  argb8888;
#endif
#ifdef  rgb565_t
    rgb565_t    rgb565 = {0};  /* Avoid Warning: C4017W: rgb565 may be used before being set */
#endif
#ifdef  argb1555_t
    argb1555_t  argb1555 = {0};  /* Avoid Warning: C4017W: argb1555 may be used before being set */
#endif
#ifdef  argb4444_t
    argb4444_t  argb4444 = {0};  /* Avoid Warning: C4017W: argb4444 may be used before being set */
#endif
#ifdef  USE_CONFIG_R8G8B8A8_T  /* r8g8b8a8_t */
    r8g8b8a8_t  r8g8b8a8;
#endif
#ifdef  yuv422_t
    yuv422_t    yuv422;
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

            #ifdef  xrgb8888_t
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

            #ifdef  USE_CONFIG_R8G8B8A8_T  /* r8g8b8a8_t */
        case  PIXEL_FORMAT_R8G8B8A8:
            r8g8b8a8.Value = InputColor;
            argb8888 = r8g8b8a8_t_to_argb8888_t( r8g8b8a8 );
            break;
            #endif

            #ifdef  yuv422_t
        case  PIXEL_FORMAT_YUV422: {
            argb8888_t  argb[2];

            yuv422.Value = InputColor;
            gs_YUV422Type_to_ARGB8888Type( yuv422, argb );
            argb8888 = argb[0];
            break;
        }
        #endif

        default:
            ASSERT_R  
            ( false, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_36; goto fin );  /* Error: <Private/> */
    }


    /* Change out color format */
    switch ( OutputFormat )
    {
        #ifdef  argb8888_t
        case  PIXEL_FORMAT_ARGB8888:
            *out_Color = argb8888.Value;
            break;
            #endif

            #ifdef  xrgb8888_t
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

            #ifdef  USE_CONFIG_R8G8B8A8_T  /* r8g8b8a8_t */
        case  PIXEL_FORMAT_R8G8B8A8:
            r8g8b8a8 = argb8888_t_to_r8g8b8a8_t( argb8888 );
            *out_Color = r8g8b8a8.Value;
            break;
            #endif

            #ifdef  yuv422_t
        case  PIXEL_FORMAT_YUV422: {
            argb8888_t  argb[2];

            argb[0] = argb8888;
            argb[1] = argb8888;
            yuv422 = gs_ARGB8888Type_to_YUV422Type( argb );
            *out_Color = yuv422.Value;
            break;
        }
        #endif

        default:
            ASSERT_R 
            ( false, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_39; goto fin );  /* Error: <Private/> */
    }

    e=0;
fin:
    {
        return  e;
    }
}/* End of function gs_ChangePixelFormat */


/***********************************************************************
* Function Name: gs_YUV422Type_to_ARGB8888Type
************************************************************************/
static void  gs_YUV422Type_to_ARGB8888Type 
                ( yuv422_t SourceColors, argb8888_t out_DestinationColors[2] )
{
    const yuv422_t  src = SourceColors; 
    argb8888_t*     dst = out_DestinationColors; 

    /* Cast to an appropriate type */
    int  cr_01 = (int) src.u.Cr_01; 

    /* Cast to an appropriate type */
    int  cb_01 = (int) src.u.Cb_01; 
    int  cr_cb_red; 
    int  cr_cb_green; 
    int  cr_cb_blue; 
    int  y; 
    int  red; 
    int  green; 
    int  blue; 
    #if ! FASTER_YUV422Type_to_ARGB8888Type
    const float  round = 0.5f;
    #endif

    #if ! FASTER_YUV422Type_to_ARGB8888Type
        if ( cr_01 <   0 )  { cr_01 =   0; }
        if ( cr_01 > 255 )  { cr_01 = 255; }
        if ( cb_01 <   0 )  { cb_01 =   0; }
        if ( cb_01 > 255 )  { cb_01 = 255; }
    #endif

    cr_01 = cr_01 - 0x80;
    cb_01 = cb_01 - 0x80;

    #if FASTER_YUV422Type_to_ARGB8888Type

        /* Cast to an appropriate type */
        cr_cb_red   = (int)( (+ (int)(1.402f*256)) * cr_01 ) >> 8;

        /* Cast to an appropriate type */
        cr_cb_green = (int)( ((- (int)(0.714f*256)) * cr_01) - ((int)(0.344f*256) * cb_01) ) >> 8;

        /* Cast to an appropriate type */
        cr_cb_blue  = (int)( (+ (int)(1.772f*256)) * cb_01 ) >> 8;
    #else
        cr_cb_red   = (int)( + 1.402f * cr_01 + round );
        cr_cb_green = (int)( - 0.714f * cr_01 - 0.344f * cb_01 + round );
        cr_cb_blue  = (int)( + 1.772f * cb_01 + round );
    #endif

    y = src.u.Y_0;
    #if ! FASTER_YUV422Type_to_ARGB8888Type
        if ( y <   0 )  { y =   0; }
        if ( y > 255 )  { y = 255; }
    #endif

    red   = y + cr_cb_red;
    green = y + cr_cb_green;
    blue  = y + cr_cb_blue;

    if ( red   < 0 )
    {
        red   = 0;
    }
    if ( green < 0 )
    {
        green = 0;
    }
    if ( blue  < 0 )
    {
        blue  = 0;
    }
    if ( red   > 255 )
    {
        red   = 255;
    }
    if ( green > 255 )
    {
        green = 255;
    }
    if ( blue  > 255 )
    {
        blue  = 255;
    }

    /* Cast to an appropriate type */
    dst[0].u.Red   = (uint8_t) red;

    /* Cast to an appropriate type */
    dst[0].u.Green = (uint8_t) green;

    /* Cast to an appropriate type */
    dst[0].u.Blue  = (uint8_t) blue;
    dst[0].u.Alpha = 0xFF;

    y = src.u.Y_1;
    #if ! FASTER_YUV422Type_to_ARGB8888Type
        if ( y <   0 )  { y =   0; }
        if ( y > 255 )  { y = 255; }
    #endif

    red   = y + cr_cb_red;
    green = y + cr_cb_green;
    blue  = y + cr_cb_blue;

    if ( red   < 0 )
    {
        red   = 0;
    }
    if ( green < 0 )
    {
        green = 0;
    }
    if ( blue  < 0 )
    {
        blue  = 0;
    }
    if ( red   > 255 )
    {
        red   = 255;
    }
    if ( green > 255 )
    {
        green = 255;
    }
    if ( blue  > 255 )
    {
        blue  = 255;
    }

    /* Cast to an appropriate type */
    dst[1].u.Red   = (uint8_t) red;

    /* Cast to an appropriate type */
    dst[1].u.Green = (uint8_t) green;

    /* Cast to an appropriate type */
    dst[1].u.Blue  = (uint8_t) blue;
    dst[1].u.Alpha = 0xFF;
}/* End of function gs_YUV422Type_to_ARGB8888Type */


/***********************************************************************
* Function Name: gs_ARGB8888Type_to_YUV422Type
************************************************************************/
static yuv422_t  gs_ARGB8888Type_to_YUV422Type( argb8888_t Colors[2] ) 
{
    yuv422_t  ret = {0};  /* Avoid Warning: C4017W: ret may be used before being set */

    /* Cast to an appropriate type */
    int_fast32_t  red_01   = ( (int_fast32_t) Colors[0].u.Red   + Colors[1].u.Red )   / 2;

    /* Cast to an appropriate type */
    int_fast32_t  green_01 = ( (int_fast32_t) Colors[0].u.Green + Colors[1].u.Green ) / 2;

    /* Cast to an appropriate type */
    int_fast32_t  blue_01  = ( (int_fast32_t) Colors[0].u.Blue  + Colors[1].u.Blue )  / 2;
    int_fast32_t  y;
    int_fast32_t  cr;
    int_fast32_t  cb;
#if defined(_SH) || defined(__ARMCC_VERSION)
    const float  round = 0.1f;
#else
    /* 0.0 = green can be return */
    const float  round = 0.0f;  
#endif

    /* Cast to an appropriate type */
    y = (int)( (0.2989f * Colors[0].u.Red) + (0.5866f * Colors[0].u.Green) + (0.1145f * Colors[0].u.Blue) + round );
    if ( y <   0 )
    {
        y =   0;
    }
    if ( y > 255 )
    {
        y = 255;
    }

    /* Cast to an appropriate type */
    ret.u.Y_0 = (uint8_t) y;

    /* Cast to an appropriate type */
    y = (int)( (0.2989f * Colors[1].u.Red) + (0.5866f * Colors[1].u.Green) + (0.1145f * Colors[1].u.Blue) + round );
    if ( y <   0 )
    {
        y =   0;
    }
    if ( y > 255 )
    {
        y = 255;
    }

    /* Cast to an appropriate type */
    ret.u.Y_1 = (uint8_t) y;

    /* Cast to an appropriate type */
    cr = (int)( (((0.5000f * red_01) - (0.4183f * green_01)) - (0.0816f * blue_01)) + round + 0x80 );

    /* + 0x80 must do before casting to int. Because (int)(0.9) == (int)(-0.9) == 0 */

    if ( cr <   0 )
    {
        cr =   0;
    }
    if ( cr > 255 )
    {
        cr = 255;
    }

    /* Cast to an appropriate type */
    ret.u.Cr_01 = (uint8_t) cr;

    /* Cast to an appropriate type */
    cb = (int)( (((-0.1687f) * red_01) - (0.3312f * green_01)) + (0.5000f * blue_01) + round + 0x80 );
    if ( cb <   0 )
    {
        cb =   0;
    }
    if ( cb > 255 )
    {
        cb = 255;
    }

    /* Cast to an appropriate type */
    ret.u.Cb_01 = (uint8_t) cb;

    return  ret;
}/* End of function gs_ARGB8888Type_to_YUV422Type */


/***********************************************************************
* Function Name: R_RGA_ChangePixelFormat
************************************************************************/
/* new "change_pixel_format" */
errnum_t  R_RGA_ChangePixelFormat  
    ( uint32_t  in_InputColor,  pixel_format_t  in_InputFormat,
    uint32_t*  out_Color,  pixel_format_t  in_OutputFormat )
{
    errnum_t      e;

#ifdef  __CC_ARM
#pragma push
#pragma diag_suppress 4017
#endif
#ifdef  argb8888_t
    argb8888_t  argb8888;
#endif
#ifdef  rgb565_t
    rgb565_t    rgb565 = {0};  /* Avoid Warning: C4017W: rgb565 may be used before being set */
#endif
#ifdef  argb1555_t
    argb1555_t  argb1555 = {0};  /* Avoid Warning: C4017W: argb1555 may be used before being set */
#endif
#ifdef  argb4444_t
    argb4444_t  argb4444 = {0};  /* Avoid Warning: C4017W: argb4444 may be used before being set */
#endif
#ifdef  USE_CONFIG_R8G8B8A8_T  /* r8g8b8a8_t */
    r8g8b8a8_t  r8g8b8a8;
#endif
#ifdef  yuv422_t
    yuv422_t    yuv422;
#endif
#ifdef  __CC_ARM
#pragma pop
#endif


    /* Change input color to ARGB8888 */
    switch ( in_InputFormat )
    {
        #ifdef  argb8888_t
        case  PIXEL_FORMAT_ARGB8888:
            argb8888.Value = in_InputColor;
            break;
            #endif

            #ifdef  xrgb8888_t
        case  PIXEL_FORMAT_XRGB8888:
            argb8888.Value = in_InputColor;
            argb8888.u.Alpha = 255;
            break;
            #endif

            #ifdef  rgb565_t
        case  PIXEL_FORMAT_RGB565:

            /* Cast to an appropriate type */
            rgb565.Value = (uint16_t) in_InputColor;
            argb8888 = rgb565_t_to_argb8888_t( rgb565 );
            break;
            #endif

            #ifdef  argb1555_t
        case  PIXEL_FORMAT_ARGB1555:

            /* Cast to an appropriate type */
            argb1555.Value = (uint16_t) in_InputColor;
            argb8888 = argb1555_t_to_argb8888_t( argb1555 );
            break;
            #endif

            #ifdef  argb4444_t
        case  PIXEL_FORMAT_ARGB4444:

            /* Cast to an appropriate type */
            argb4444.Value = (uint16_t) in_InputColor;
            argb8888 = argb4444_t_to_argb8888_t( argb4444 );
            break;
            #endif

            #ifdef  USE_CONFIG_R8G8B8A8_T  /* r8g8b8a8_t */
        case  PIXEL_FORMAT_R8G8B8A8:
            r8g8b8a8.Value = in_InputColor;
            argb8888 = r8g8b8a8_t_to_argb8888_t( r8g8b8a8 );
            break;
            #endif

            #ifdef  yuv422_t
        case  PIXEL_FORMAT_YUV422: {
            argb8888_t  argb[2];

            yuv422.Value = in_InputColor;
            YUV422Type_to_ARGB8888Type( yuv422, argb );
            argb8888 = argb[0];
            break;
        }
        #endif

        default:
            ASSERT_R 
            ( false, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_49; goto fin );  /* Error: */
    }


    /* Change out color format */
    switch ( in_OutputFormat )
    {
        #ifdef  argb8888_t
        case  PIXEL_FORMAT_ARGB8888:
            *out_Color = argb8888.Value;
            break;
            #endif

            #ifdef  xrgb8888_t
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

            #ifdef  USE_CONFIG_R8G8B8A8_T  /* r8g8b8a8_t */
        case  PIXEL_FORMAT_R8G8B8A8:
            r8g8b8a8 = argb8888_t_to_r8g8b8a8_t( argb8888 );
            *out_Color = r8g8b8a8.Value;
            break;
            #endif

            #ifdef  yuv422_t
        case  PIXEL_FORMAT_YUV422: {
            argb8888_t  argb[2];

            argb[0] = argb8888;
            argb[1] = argb8888;
            yuv422 = ARGB8888Type_to_YUV422Type( argb );
            *out_Color = yuv422.Value;
            break;
        }
        #endif

        default:
            ASSERT_R 
            ( false, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_4A; goto fin );  /* Error: */
    }

    e=0;
fin:
    {
        return  e;
    }
}/* End of function R_RGA_ChangePixelFormat */


/***********************************************************************
* Function Name: YUV422Type_to_ARGB8888Type
************************************************************************/
void  YUV422Type_to_ARGB8888Type 
        ( yuv422_t  in_SourceColors,  argb8888_t  out_DestinationColors[2] )
{
    gs_YUV422Type_to_ARGB8888Type( in_SourceColors,  out_DestinationColors );
}/* End of function YUV422Type_to_ARGB8888Type */


/***********************************************************************
* Function Name: ARGB8888Type_to_YUV422Type
************************************************************************/
yuv422_t  ARGB8888Type_to_YUV422Type( argb8888_t  in_Colors[2] ) 
{
    return  gs_ARGB8888Type_to_YUV422Type( in_Colors );
}/* End of function ARGB8888Type_to_YUV422Type */


