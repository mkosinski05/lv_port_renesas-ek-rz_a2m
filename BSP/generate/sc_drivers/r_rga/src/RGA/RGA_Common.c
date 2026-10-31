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
* File Name   : rga_common.c
* Version     : 1.12
******************************************************************************/

#include  <stdio.h>
#include  <string.h>
#include  "bitblt_alpha.h"
#include  "clib_drivers.h"
#include  "r_drw_pl.h"
#include  "rga_api.h"
#include  "rga_config.h"
#include  "rga_inside.h"
#include  "rga_local.h"
#include  "rga_rz_a2m_drw.h"
#include  "rz_co.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define  GS_N  "\r\n"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
static errnum_t  gs_InitByShareFrameBuffer_Sub( graphics_image_t* self,  frame_buffer_t*  in_FrameBuffer,
    bool_t  in_IsFramePipelineEnabled );


/*-------------------------------------------------------------------------*/
/* <<<< ### (graphics_static_t) implement >>>> */ 
/*-------------------------------------------------------------------------*/

/***********************************************************************
* Function Name: R_GRAPHICS_STATIC_GetVersion
************************************************************************/
errnum_t  R_GRAPHICS_STATIC_GetVersion  
            ( uint32_t*  out_Version )
{
    *out_Version = RGA_VERSION;
    return  0;
} /* End of function R_GRAPHICS_STATIC_GetVersion */



/***********************************************************************
* Function: GetPixelFormatEnumFrom_T_IMAGE_INF 
************************************************************************/
static int  g_GetPixelFormatEnumFrom_T_IMAGE_INF[ ] = { 
    PIXEL_FORMAT_RGB565,   /* BITBLT_RGB565 */
    PIXEL_FORMAT_UNKNOWN,                    /* BITBLT_RGB555 */
    PIXEL_FORMAT_ARGB1555, /* BITBLT_ARGB1555 */
    PIXEL_FORMAT_ARGB4444, /* BITBLT_ARGB4444 */
    -1,                    /* BITBLT_RGB565I_A8 */
    -1,                    /* BITBLT_RGB565C_A8 */
    PIXEL_FORMAT_CLUT8,    /* BITBLT_B32PALET256 */
    PIXEL_FORMAT_CLUT4,    /* BITBLT_B32PALET16 */
    PIXEL_FORMAT_CLUT1,    /* BITBLT_B32PALET2 */
    PIXEL_FORMAT_XRGB8888, /* BITBLT_RGB888 */
    PIXEL_FORMAT_ARGB8888, /* BITBLT_RGB8888 */
    PIXEL_FORMAT_YUV422,   /* BITBLT_YUV422 */
    -1,                    /* BITBLT_B16PALET256 */
    -1,                    /* BITBLT_RGBA5551 */
    -1,                    /* BITBLT_RGBA4444 */
    -1,                    /* BITBLT_RGB666 */
    -1,                    /* BITBLT_ARGB6666 */
    PIXEL_FORMAT_R8G8B8A8, /* BITBLT_RGBA8888 */
    -1,                    /* BITBLT_RGB888_A */
    -1,                    /* BITBLT_RGB888_C */
    PIXEL_FORMAT_A8,       /* BITBLT_A8 */
    PIXEL_FORMAT_A4,       /* BITBLT_A4 */
    PIXEL_FORMAT_A1,       /* BITBLT_A1 */
};

/***********************************************************************
* Function Name: GetPixelFormatEnumFrom_T_IMAGE_INF
************************************************************************/
errnum_t  GetPixelFormatEnumFrom_T_IMAGE_INF( pixel_format_t* out, uint8_t in ) 
{
    errnum_t  e;
    int       out2; 

    ASSERT_R( in < R_COUNT_OF( g_GetPixelFormatEnumFrom_T_IMAGE_INF ), 
        e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_22; goto fin );

    /* Error: <E_NOT_SUPPORTED_PIXEL_FORMAT/> */

    out2 = g_GetPixelFormatEnumFrom_T_IMAGE_INF[ in ];
    ASSERT_R( out2 != -1, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_23; goto fin ); 

    /* Error: <E_NOT_SUPPORTED_PIXEL_FORMAT/> */

    /* Cast to an appropriate type */
    *out = (pixel_format_t)out2;
    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function GetPixelFormatEnumFrom_T_IMAGE_INF */


/***********************************************************************
* Function: Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat 
************************************************************************/
static pixel_format_t  g_Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat[ ] = {
    PIXEL_FORMAT_RGB565,    /* BITBLT_RGB565 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGB555 */
    PIXEL_FORMAT_ARGB1555,  /* BITBLT_ARGB1555 */
    PIXEL_FORMAT_ARGB4444,  /* BITBLT_ARGB4444 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGB565I_A8 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGB565C_A8 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_B32PALET256 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_B32PALET16 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_B32PALET2 */
    PIXEL_FORMAT_XRGB8888,  /* BITBLT_RGB888 */
    PIXEL_FORMAT_ARGB8888,  /* BITBLT_RGB8888 */
    PIXEL_FORMAT_YUV422,    /* BITBLT_YUV422 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_B16PALET256 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGBA5551 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGBA4444 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGB666 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_ARGB6666 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGBA8888 */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGB888_A */
    PIXEL_FORMAT_UNKNOWN,   /* BITBLT_RGB888_C */
    PIXEL_FORMAT_A8,        /* BITBLT_A8 */
    PIXEL_FORMAT_A4,        /* BITBLT_A4 */
    PIXEL_FORMAT_A1,        /* BITBLT_A1 */
};

/***********************************************************************
* Function Name: Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat
************************************************************************/
LIB_STATIC errnum_t 
    Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat  
        ( pixel_format_t* out, uint8_t in )
{
    errnum_t  e;

    /* Cast to an appropriate type */
    ASSERT_R 
        ( (int_t) in >= 0  &&  in < R_COUNT_OF( g_Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat ),
            e=E_RGA_OTHER_18; goto fin );  /* Error: <Private/> */
    *out = g_Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat[ in ];
    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat */

/***********************************************************************
* Function Name: R_RGA_IsSupportedPixelFormatForRGAH
************************************************************************/
bool_t 
    R_RGA_IsSupportedPixelFormatForRGAH 
        ( pixel_format_t pixel_format )
{
    switch ( pixel_format )
    {
        case  PIXEL_FORMAT_RGB565:
        case  PIXEL_FORMAT_ARGB4444:
        case  PIXEL_FORMAT_ARGB8888:
        case  PIXEL_FORMAT_XRGB8888:
            return  true;

        default:
            return  false;
    }
} /* End of function R_RGA_IsSupportedPixelFormatForRGAH */


/***********************************************************************
* Function Name: R_RGA_GetBytePerPixelFromPixelFormat
************************************************************************/
errnum_t  R_RGA_GetBytePerPixelFromPixelFormat( pixel_format_t  in_PixelFormat, 
    byte_per_pixel_t*  out_BytePerPixel )
{
    errnum_t          e;
    byte_per_pixel_t  byte_per_pixel;

    switch ( in_PixelFormat )
    {
        case  PIXEL_FORMAT_RGB565:
        case  PIXEL_FORMAT_ARGB4444:
            byte_per_pixel = 2;
            break;

        case  PIXEL_FORMAT_ARGB8888:
        case  PIXEL_FORMAT_XRGB8888:
        case  PIXEL_FORMAT_R8G8B8A8:
            byte_per_pixel = 4;
            break;

        case  PIXEL_FORMAT_A8:
            byte_per_pixel = 1;
            break;

        default:
            e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_48;  goto fin;

            /* Error: */
            break;
    }

    e=0;
fin:
    {
        *out_BytePerPixel = byte_per_pixel;
    }
    R_ERROR_Set( e );
    return  e;
} /* End of function R_RGA_GetBytePerPixelFromPixelFormat */


/* <<< End of Class implement >>> */

/***********************************************************************
* Function Name: R_RGA_IsPixelFormatHavingAlpha
************************************************************************/
errnum_t  R_RGA_IsPixelFormatHavingAlpha 
            ( pixel_format_t  in_PixelFormat,  bool_t*  out_IsAlpha )
{
    switch ( in_PixelFormat )
    {
        case  PIXEL_FORMAT_ARGB8888:
        case  PIXEL_FORMAT_ARGB1555:
        case  PIXEL_FORMAT_ARGB4444:
        case  PIXEL_FORMAT_R8G8B8A8:
        case  PIXEL_FORMAT_A8:
        case  PIXEL_FORMAT_A4:
        case  PIXEL_FORMAT_A1:
            *out_IsAlpha = true;
            break;

        case  PIXEL_FORMAT_XRGB8888:
        case  PIXEL_FORMAT_RGB565:
        case  PIXEL_FORMAT_YUV422:
        case  PIXEL_FORMAT_CLUT8:
        case  PIXEL_FORMAT_CLUT4:
        case  PIXEL_FORMAT_CLUT1:

            *out_IsAlpha = false;
            break;

        default: {
            errnum_t  e = E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3C;
            R_ERROR_Set( e );
            ASSERT_R( false, return  e ); 

                /* Error: R_RGA_IsPixelFormatHavingAlpha(): Specified pixel format is not supported. */
        }
    }
    return  0;
} /* End of function R_RGA_IsPixelFormatHavingAlpha */


/***********************************************************************
* Function Name: R_GRAPHICS_BeginPath
************************************************************************/
errnum_t  R_GRAPHICS_BeginPath( graphics_t* self ) 
{
    self->PathDataLength = 0;
    return  0;
} /* End of function R_GRAPHICS_BeginPath */


/***********************************************************************
* Function Name: R_GRAPHICS_Rect
************************************************************************/
errnum_t  R_GRAPHICS_Rect
    ( graphics_t* self,  int_fast32_t  in_MinX,  int_fast32_t  in_MinY,
        int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t  e;

    if ( !( self->PathDataLength == 0 ))
    {
        e=E_RGA_FEW_ARRAY_7;
        goto fin;
    }

    /* Error: A path is already registered. */

    self->PathDataRectangle.MinX = in_MinX;
    self->PathDataRectangle.MinY = in_MinY;
    self->PathDataRectangle.Width  = in_Width;
    self->PathDataRectangle.Height = in_Height;
    self->PathDataLength += 1;

    e=0;
fin:
    {
        return e;
    }
} /* End of function R_GRAPHICS_Rect */


/***********************************************************************
* Function Name: R_GRAPHICS_SetGlobalAlpha
************************************************************************/
errnum_t  R_GRAPHICS_SetGlobalAlpha 
            ( graphics_t*  self,  uint_fast8_t  in_AlphaValue )
{
    /* Cast to an appropriate type */
    self->GlobalAlpha = (uint8_t) in_AlphaValue;
    return  0;
} /* End of function R_GRAPHICS_SetGlobalAlpha */


/***********************************************************************
* Function Name: R_GRAPHICS_GetGlobalAlpha
************************************************************************/
errnum_t  R_GRAPHICS_GetGlobalAlpha 
            ( graphics_t*  self,  uint8_t*  out_AlphaValue )
{
    *out_AlphaValue = self->GlobalAlpha;
    return  0;
} /* End of function R_GRAPHICS_GetGlobalAlpha */


/***********************************************************************
* Function Name: R_GRAPHICS_SetGlobalCompositeOperation
************************************************************************/
errnum_t
    R_GRAPHICS_SetGlobalCompositeOperation 
        ( graphics_t* self, graphics_composite_operation_t CompositeOperation )
{
    self->GlobalCompositeOperation = CompositeOperation;
    return  0;
}/* End of function R_GRAPHICS_SetGlobalCompositeOperation */


/***********************************************************************
* Function Name: R_GRAPHICS_GetGlobalCompositeOperation
************************************************************************/
errnum_t  R_GRAPHICS_GetGlobalCompositeOperation( graphics_t* self, 
    graphics_composite_operation_t* out_CompositeOperation )
{
    *out_CompositeOperation = self->GlobalCompositeOperation;
    return  0;
}/* End of function R_GRAPHICS_GetGlobalCompositeOperation */

/***********************************************************************
* Function Name: GraphicsClass_debugSHS
************************************************************************/
#ifndef R_OSPL_NDEBUG
LIB_STATIC void  GraphicsClass_debugSHS( const char* Label ) 
{
    #if 0 
        static int  count = 0;

        count += 1;
        printf( "SHS[%d]>%s\n", count, Label );
    #else
        /* Cast to an appropriate type */
        R_UNREFERENCED_VARIABLE( Label );
    #endif
} /* End of function GraphicsClass_debugSHS */
#endif


/* <<< End of Class implement >>> */

/*-------------------------------------------------------------------------*/
/* <<<< ### (graphics_image_t) implement >>>> */
/*-------------------------------------------------------------------------*/

/***********************************************************************
* Function Name: R_GRAPHICS_IMAGE_InitCopyFrameBufferR8G8B8A8
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitCopyFrameBufferR8G8B8A8(
    graphics_image_t* self,  void*  in_ImageDataArray,  size_t  in_ImageDataArraySize,
    graphics_t*  Context, int MinX,  int MinY,  int width,  int height )
{
    errnum_t  e;
    SoftBitBltFrameBufferClass  source_frame;
    SoftBitBltFrameBufferClass  destinaion_frame;
    SoftBitBltRectangleClass    source_rect;
    SoftBitBltRectangleClass    destinaion_rect;
    frame_buffer_t*             frame = Context->FrameBuffer; 
    size_t                      image_size;

    source_frame.Address = frame->buffer_address[ frame->draw_buffer_index ];
    source_frame.width   = frame->width;
    source_frame.height  = frame->height;
    source_frame.stride  = frame->stride;

    destinaion_frame.Address = in_ImageDataArray;
    destinaion_frame.width   = width;
    destinaion_frame.height  = height;
    destinaion_frame.stride  = width * 4;  /* 4 = R8G8B8A8 */

    image_size = destinaion_frame.stride * destinaion_frame.height;

    ASSERT_R   
        ( image_size <= in_ImageDataArraySize, e=E_RGA_FEW_ARRAY_2; goto fin );

    /* Error: in_ImageDataArraySize argument is too small. */

    source_rect.Left   = MinX;
    source_rect.Top    = MinY;
    source_rect.Right  = (MinX + width) - 1;
    source_rect.Bottom = (MinY + height) - 1;

    destinaion_rect.Left   = 0;
    destinaion_rect.Top    = 0;
    destinaion_rect.Right  = width - 1;
    destinaion_rect.Bottom = height - 1;

    e= SoftBitBlt_toInClippedFrame( &source_frame, &destinaion_frame, &source_rect, &destinaion_rect,
        &destinaion_rect );
    IF(e)
    {
        goto fin;
    }


    e= R_GRAPHICS_BeginSoftwareRendering( Context );
    IF(e)
    {
        goto fin;
    }

    /* Set 0 in out of frame */
    if ( (source_rect.Left > source_rect.Right) ||
        ((destinaion_rect.Bottom - destinaion_rect.Top) != (height - 1)) ||
        ((destinaion_rect.Right - destinaion_rect.Left) != (width - 1)) )
    {
        bool_t  is_alpha;
        r8g8b8a8_t  black;

        e= R_RGA_IsPixelFormatHavingAlpha( frame->pixel_format, &is_alpha );
        IF(e)
        {
            goto fin;
        }
        if ( is_alpha )
        {
            memset32( in_ImageDataArray,  0,  image_size );
        }
        else
        {
            black.Value = 0;
            black.u.Alpha = 255;

            /* Cast to an appropriate type */
            memset32( in_ImageDataArray,  (unsigned) black.Value,  image_size );
        }
    }


    /* Get pixels */
    if ( source_rect.Left <= source_rect.Right )
    {
        switch ( frame->pixel_format )
        {
            case  PIXEL_FORMAT_ARGB8888:
                e= SoftBitBlt_ARGB8888_to_R8G8B8A8_NoBlend_NoStretch(
                        &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
                IF(e)
                {
                    goto fin;
                }
            break;

            case  PIXEL_FORMAT_XRGB8888:
                e= SoftBitBlt_XRGB8888_to_R8G8B8A8_NoBlend_NoStretch(
                        &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
                IF(e)
                {
                    goto fin;
                }
            break;

            case  PIXEL_FORMAT_RGB565:
                e= SoftBitBlt_RGB565_to_R8G8B8A8_NoBlend_NoStretch(
                        &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
                IF(e)
                {
                    goto fin;
                }
            break;

            case  PIXEL_FORMAT_ARGB1555:
                e= SoftBitBlt_ARGB1555_to_R8G8B8A8_NoBlend_NoStretch(
                        &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
                IF(e)
                {
                    goto fin;
                }
            break;

            case  PIXEL_FORMAT_ARGB4444:
                e= SoftBitBlt_ARGB4444_to_R8G8B8A8_NoBlend_NoStretch(
                        &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
                IF(e)
                {
                    goto fin;
                }
            break;

            default:
                ASSERT_R 
                ( frame->pixel_format == PIXEL_FORMAT_YUV422, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3; goto fin );
            
                e= SoftBitBlt_YUV422_to_R8G8B8A8_NoBlend_NoStretch(
                    &source_frame, &destinaion_frame, &source_rect, &destinaion_rect, 0 );
                IF(e)
                {
                    goto fin;
                }
            break;
        }
    }

    e= R_GRAPHICS_EndSoftwareRendering( Context );
    IF(e)
    {
        goto fin;
    }

    self->flags = F_T_IMAGE_INF_RAW_ADDRESS;

    /* Cast to an appropriate type */
    self->offset_to_image = (uint32_t) in_ImageDataArray;
    self->offset_to_alpha = 0;

    /* Cast to an appropriate type */
    self->width  = (uint16_t) width;

    /* Cast to an appropriate type */
    self->height = (uint16_t) height;
    self->type = BITBLT_RGBA8888;
    self->type2 = 0;
    self->color = 0;
    #if GRAPHICS_IMAGE_CLASS_IS_T_IMAGE_INF
        self->ImageSize = image_size;
        self->AlphaSize = 0;
        self->TotalSize = image_size;
        self->CheckSum  = 0;
    #endif

    e=0;
fin:
    {
        e= R_GRAPHICS_EndRenderingInFin( Context, e );
    }
    R_ERROR_Set( e );
    return  e;
} /* End of function R_GRAPHICS_IMAGE_InitCopyFrameBufferR8G8B8A8 */


/***********************************************************************
* Function Name: GraphicsImageClass_isRawFormat
************************************************************************/
bool_t  GraphicsImageClass_isRawFormat( const graphics_image_t* self ) 
{
    return  ( ( self->flags & (~F_T_IMAGE_INF_USED_MASK) ) == 0 );
} /* End of function GraphicsImageClass_isRawFormat */

/***********************************************************************
* Function: Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum
************************************************************************/
static uint8_t  g_Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum[ ] = {

    /* index = pixel_format_t & 0x0F */
    BITBLT_RGB888,     /* PIXEL_FORMAT_XRGB8888 = 0x40 */
    BITBLT_RGB8888,    /* PIXEL_FORMAT_ARGB8888 = 1 */
    BITBLT_YUV422,     /* PIXEL_FORMAT_YUV422 = 0x10002 */
    BITBLT_RGB565,     /* PIXEL_FORMAT_RGB565 = 3 */
    BITBLT_ARGB1555,   /* PIXEL_FORMAT_ARGB1555 = 0x44 */
    BITBLT_ARGB4444,   /* PIXEL_FORMAT_ARGB4444 = 5 */
    (uint8_t) -1,      /* 6 */
    (uint8_t) -1,      /* 7 */
    (uint8_t) -1,      /* 8 */
    (uint8_t) -1,      /* 9 */
    (uint8_t) -1,      /* 10 */
    BITBLT_A8,         /* PIXEL_FORMAT_A8 = 11 */
    (uint8_t) -1,      /* 12 */
    BITBLT_A1,         /* PIXEL_FORMAT_A1 = 13 */
    BITBLT_A4,         /* PIXEL_FORMAT_A4 = 14 */
    (uint8_t) -1       /* PIXEL_FORMAT_RGB888 = 15 */
};

/***********************************************************************
* Function Name: Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum
************************************************************************/
LIB_STATIC errnum_t  Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum 
                        ( uint8_t* out, int in )
{
    errnum_t  e;
    uint8_t   out2;

    out2 = g_Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum[ in & 0xF ];

    /* Cast to an appropriate type */
    ASSERT_R 
        ( out2 != (uint8_t) -1,  e=E_RGA_OTHER_17; goto fin );  /* Error: <Private/> */

    *out = out2;
    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    R_ERROR_Set( e );
    return  e;
} /* End of function Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum */


/***********************************************************************
* Function Name: R_GRAPHICS_IMAGE_InitByShareFrameBuffer
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitByShareFrameBuffer 
            ( graphics_image_t*  self,  frame_buffer_t*  in_FrameBuffer )
{
    return  gs_InitByShareFrameBuffer_Sub( self,  in_FrameBuffer,  false );
} /* End of function R_GRAPHICS_IMAGE_InitByShareFrameBuffer */


/***********************************************************************
* Function Name: R_GRAPHICS_IMAGE_InitByShareFrameBufferEx
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitByShareFrameBufferEx 
            ( graphics_image_t* self,  frame_buffer_t*  in_FrameBuffer, graphics_t*  in_GraphicsContext )
{
    return  gs_InitByShareFrameBuffer_Sub( self,  in_FrameBuffer,  in_GraphicsContext->HasFramePipeline );
} /* End of function R_GRAPHICS_IMAGE_InitByShareFrameBufferEx */


/***********************************************************************
* Function Name: R_GRAPHICS_IMAGE_InitR8G8B8A8
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitR8G8B8A8( 
    graphics_image_t*  self,  void*  in_ImageDataArray,  size_t  in_ImageDataArraySize,
    int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t  e;
    size_t    image_size = 4 * in_Width * in_Height;

    if ( !( in_Width  > 0 ))
    {
        e=E_RGA_OTHER_F8;
        goto fin;
    } /* Error: The image witdh is 0 or smaller than 0. */
    if ( !( in_Height > 0 ))
    {
        e=E_RGA_OTHER_F9;
        goto fin;
    } /* Error: The image height is 0 or smaller than 0. */
    if ( !( image_size <= (int) in_ImageDataArraySize ))
    {
        e=E_RGA_FEW_ARRAY_9;
        goto fin;
    }

    /* Error: Too small array size. */

    memset( in_ImageDataArray,  0,  image_size );

    self->flags = F_T_IMAGE_INF_RAW_ADDRESS;

    /* Cast to an appropriate type */
    self->offset_to_image = (uint32_t) in_ImageDataArray;
    self->offset_to_alpha = 0;

    /* Cast to an appropriate type */
    self->width  = (uint16_t) in_Width;

    /* Cast to an appropriate type */
    self->height = (uint16_t) in_Height;
    self->type = BITBLT_RGBA8888;
    self->type2 = 0;
    self->color = 0;
    #if GRAPHICS_IMAGE_CLASS_IS_T_IMAGE_INF
        self->ImageSize = image_size;
        self->AlphaSize = 0;
        self->TotalSize = image_size;
        self->CheckSum  = 0;
    #endif

    e=0;
fin:
    {
        return e;
    }
} /* End of function R_GRAPHICS_IMAGE_InitR8G8B8A8 */


/***********************************************************************
* Function Name: R_GRAPHICS_IMAGE_InitSameSizeR8G8B8A8
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitSameSizeR8G8B8A8( 
    graphics_image_t* self,  void*  in_ImageDataArray,  size_t  in_ImageDataArraySize,
    graphics_image_t* SameSizeImage )
{
    return  R_GRAPHICS_IMAGE_InitR8G8B8A8( self,  in_ImageDataArray,  in_ImageDataArraySize,

        /* Cast to an appropriate type */
        (int) SameSizeImage->width, (int) SameSizeImage->height );
} /* End of function R_GRAPHICS_IMAGE_InitSameSizeR8G8B8A8 */


/***********************************************************************
* Function Name: gs_InitByShareFrameBuffer_Sub
************************************************************************/
static errnum_t  gs_InitByShareFrameBuffer_Sub 
                    ( graphics_image_t* self,  frame_buffer_t*  in_FrameBuffer, bool_t  in_HasFramePipeline )
{
    errnum_t      e;
    bool_t        is_alpha = false;  /* Avoid Warning: C4017W: is_alpha may be used before being set */
    int_fast32_t  previous_buffer_index;
    int_fast32_t  minus_value;
#if GRAPHICS_IMAGE_CLASS_IS_T_IMAGE_INF
    size_t    image_size;
#endif

    ASSERT_R( in_FrameBuffer->width  > 0,  e=E_RGA_OTHER_1A; goto fin ); 

        /* Error: Too small frame buffer width. */
    ASSERT_R( in_FrameBuffer->height > 0,  e=E_RGA_OTHER_1B; goto fin ); 

        /* Error: Too small frame buffer height. */
    ASSERT_R 
        ( in_FrameBuffer->buffer_count >= 1,  e=E_RGA_OTHER_F5; goto fin );

        /* Error: A number of frame buffer is too small. */

    #if GRAPHICS_IMAGE_CLASS_IS_T_IMAGE_INF
        image_size = in_FrameBuffer->stride * in_FrameBuffer->height;
    #endif

    R_RGA_IsPixelFormatHavingAlpha( in_FrameBuffer->pixel_format,  &is_alpha );  /* is_alpha = . */


    /* previous_buffer_index = ... */
    if ( in_HasFramePipeline )
    {
        minus_value = 2;
    }
    else
    {
        minus_value = 1;
    }
    previous_buffer_index = in_FrameBuffer->draw_buffer_index - minus_value;
    while ( previous_buffer_index < 0 )
    {
        previous_buffer_index += in_FrameBuffer->buffer_count;
    }


    /* self = ... */
    if ( is_alpha )
    {
        self->flags = F_T_IMAGE_INF_RAW_ADDRESS | F_T_IMAGE_INF_BLEND_ALPHA;
    }
    else
    {
        self->flags = F_T_IMAGE_INF_RAW_ADDRESS | F_T_IMAGE_INF_BLEND_COPY;
    }

    /* Cast to an appropriate type */
    self->offset_to_image = (uintptr_t) in_FrameBuffer->buffer_address[ previous_buffer_index ];
    self->offset_to_alpha = 0;

    /* Cast to an appropriate type */
    self->width  = (uint16_t) in_FrameBuffer->width;

    /* Cast to an appropriate type */
    self->height = (uint16_t) in_FrameBuffer->height;
    e= Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum( &self->type,  in_FrameBuffer->pixel_format );
    IF(e)
    {
        goto fin;
    }  /* self->type = . */
    self->type2 = 0;
    self->color = 0;
    #if GRAPHICS_IMAGE_CLASS_IS_T_IMAGE_INF
        self->ImageSize = image_size;
        self->AlphaSize = 0;
        self->TotalSize = image_size;
        self->CheckSum  = 0;
    #endif

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function gs_InitByShareFrameBuffer_Sub */


/***********************************************************************
* Function Name: R_GRAPHICS_IMAGE_GetProperties
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_GetProperties 
            ( const graphics_image_t* self, graphics_image_properties_t* out_Properties )
{
    errnum_t  e;

    ASSERT_R 
        ( GraphicsImageClass_isRawFormat( self ),  e=E_RGA_OTHER_F0; goto fin );

        /* Error: Image format is not RGA Raw format. */

    out_Properties->width  = self->width;
    out_Properties->height = self->height;
    if ( ( self->flags & F_T_IMAGE_INF_RAW_MASK ) == F_T_IMAGE_INF_RAW_ADDRESS )
    {
        /* Cast to an appropriate type */
        out_Properties->pixels = (void*) self->offset_to_image;
    }
    else
    {
        /* F_T_IMAGE_INF_RAW_OFFSET */
        out_Properties->pixels = (uint8_t*) self + (uintptr_t) self->offset_to_image;
    }
    e= GetPixelFormatEnumFrom_T_IMAGE_INF( &out_Properties->pixelFormat, self->type );
    IF(e)
    {
        goto fin;
    }
    switch ( out_Properties->pixelFormat )
    {
        case PIXEL_FORMAT_R8G8B8A8:

            /* Cast to an appropriate type */
            out_Properties->CLUT = NULL;
            out_Properties->CLUT_count = 0;
            out_Properties->data = out_Properties->pixels;
            break;
        case PIXEL_FORMAT_CLUT8:
            out_Properties->CLUT = out_Properties->pixels;
            out_Properties->CLUT_count = 256;

            /* Cast to an appropriate type */
            out_Properties->data = NULL;
            break;
        case PIXEL_FORMAT_CLUT4:
            out_Properties->CLUT = out_Properties->pixels;
            out_Properties->CLUT_count = 16;

            /* Cast to an appropriate type */
            out_Properties->data = NULL;
            break;
        case PIXEL_FORMAT_CLUT1:
            out_Properties->CLUT = out_Properties->pixels;
            out_Properties->CLUT_count = 2;

            /* Cast to an appropriate type */
            out_Properties->data = NULL;
            break;
        default:

            /* Cast to an appropriate type */
            out_Properties->CLUT = NULL;
            out_Properties->CLUT_count = 0;

            /* Cast to an appropriate type */
            out_Properties->data = NULL;
            break;
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_GRAPHICS_IMAGE_GetProperties */

/***********************************************************************
* Function Name: R_GRAPHICS_IMAGE_GetImageFormat
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_GetImageFormat( const graphics_image_t* self, 
    pixel_format_t* out_Format )
{
    errnum_t  e;

    #if    BYTE_ENDIAN == BYTE_BIG_ENDIAN
        enum { signature_of_JPEG = 0xFFD8 }; 
        enum { signature_of_PNG  = 0x89504E47 }; 
        enum { signature_of_GIF  = 0x47494638 }; 
    #else
        enum { signature_of_JPEG = 0xD8FF }; 
        enum { signature_of_PNG  = 0x474E5089 }; 
        enum { signature_of_GIF  = 0x38464947 }; 
    #endif

    /* Cast to an appropriate type */
    if ( (*(uint16_t*) self) == signature_of_JPEG )
    {
        *out_Format = PIXEL_FORMAT_JPEG;
    }
    else
    {
        /* Cast to an appropriate type */
        uint32_t  signature = (*(uint32_t*) self);

        if ( signature == signature_of_PNG )
        {
            *out_Format = PIXEL_FORMAT_PNG;
        }
        else if ( signature == signature_of_GIF )
        {
            *out_Format = PIXEL_FORMAT_GIF;
        }
        else
        {
            ASSERT_R( ( signature & 0xFFFFFF00 ) == 0x00000000, 
                e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3A; goto fin );

                /* Error: Not supported image format. */

            e= Get_PixelFormatEnum_From_T_IMAGE_INF_PixelFormat( out_Format, self->type );
            IF(e)
            {
                goto fin;
            }
        }
    }
    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_GRAPHICS_IMAGE_GetImageFormat */

/*-------------------------------------------------------------------------*/
/* <<<< ### (graphics_pattern_t) implement >>>> */
/*-------------------------------------------------------------------------*/

/***********************************************************************
* Function Name: R_GRAPHICS_PATTERN_Initialize
************************************************************************/
errnum_t  R_GRAPHICS_PATTERN_Initialize( graphics_pattern_t* self, 
    const graphics_image_t* image,  repetition_t repetition,  graphics_t* Context )
{
    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( Context );

    ASSERT_R 
        ( repetition == GRAPHICS_REPEAT, return E_RGA_OTHER_1C );  /* Error: <Private/> */

    /* Cast to an appropriate type */
    self->image = (graphics_image_t*) image;
    self->repetition = repetition;
    return  0;
} /* End of function R_GRAPHICS_PATTERN_Initialize */


/* <<< End of Class implement >>> */

/***********************************************************************
* Function Name: R_RGA_CheckStructSize
************************************************************************/
static errnum_t  R_RGA_CheckStructSize_Sub( const char* TypeName, size_t SizeInApp, size_t SizeInLib );


errnum_t  R_RGA_CheckStructSize 
            ( size_t SizeOf_GraphicsClass, size_t SizeOf_GraphicsStatusClass,
    size_t SizeOf_GraphicsImageClass,  size_t Zero )
{
    errnum_t  e;

    /* Cast to an appropriate type */
    R_UNREFERENCED_VARIABLE( Zero );

    #ifndef R_OSPL_NDEBUG
        printf( "This is Debug configuration"GS_N );
    #else
        printf( "This is Release configuration"GS_N );
    #endif

    printf( "Type, size_in_app, size_in_lib"GS_N );

    e= R_RGA_CheckStructSize_Sub( "graphics_t", SizeOf_GraphicsClass, sizeof( graphics_t ) );
    IF(e)
    {
        goto fin;
    }

    e= R_RGA_CheckStructSize_Sub( "graphics_status_t", SizeOf_GraphicsStatusClass, sizeof( graphics_status_t ) );
    IF(e)
    {
        goto fin;
    }

    e= R_RGA_CheckStructSize_Sub( "graphics_image_t", SizeOf_GraphicsImageClass, sizeof( graphics_image_t ) );
    IF(e)
    {
        goto fin;
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_RGA_CheckStructSize */

/***********************************************************************
* Function Name: R_RGA_CheckStructSize_Sub
************************************************************************/
errnum_t 
    R_RGA_CheckStructSize_Sub  
            ( const char* TypeName, size_t SizeInApp, size_t SizeInLib )
{
    errnum_t  e;

    printf( "%s, 0x%X, 0x%X"GS_N, TypeName, SizeInApp, SizeInLib );
    ASSERT_R 
        ( SizeInApp == SizeInLib,  e=E_RGA_OTHER_20; goto fin );  /* Error: <Private/> */

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_RGA_CheckStructSize_Sub */

/***********************************************************************
* Function Name: R_GRAPHICS_BeginCachedRendering
************************************************************************/
errnum_t  R_GRAPHICS_BeginCachedRendering( graphics_t* self ) 
{
    errnum_t  e;

    if ( self->SHS_IsRequestedHardwareRendering  ||  self->SHS_IsRunningHardwareRendering )
    {

        e= R_GRAPHICS_H_Finish( self );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRequestedHardwareRendering = false;
        self->SHS_IsRunningHardwareRendering = false;
    }

    if ( self->SHS_RequestedSoftwareRenderingLevel >= 1 )
    {
        /* Cast to an appropriate type */
        R_DRW_FlushCache( (const void*) 0,  0xFFFFFFFFu );
        self->SHS_RequestedSoftwareRenderingLevel = 0;
    }

    self->SHS_Union.Requesting.IsCachedRendering = true;

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_GRAPHICS_BeginCachedRendering */


/***********************************************************************
* Function Name: R_GRAPHICS_EndCachedRendering
************************************************************************/
errnum_t  R_GRAPHICS_EndCachedRendering( graphics_t* self ) 
{
    self->SHS_Union.Requesting.IsCachedRendering = false;
    self->SHS_IsRequestedCachedRendering = true;
    return  0;
} /* End of function R_GRAPHICS_EndCachedRendering */


