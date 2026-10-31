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
* File        : rga_rz_a2m_drw.c
* Description : RGA wrapper for RZ/A2M DRW.
*               Main function is "R_GRAPHICS_H_Initialize".
* Version     : 1.12
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include  "use_config_drw.h"
#include  <math.h>
#include  <string.h>
#include  "clib_drivers.h"
#include  "dave_driver.h"
#include  "dave_base.h"
#include  "frame_buffer_inline.h"
#include  "r_drw_pl.h"
#include  "r_memory_map.h"
#include  "rz_co.h"
#include  "rga_api.h"
#include  "rga_errnum.h"
#include  "rga_inside.h"
#include  "rga_local.h"
#include  "rga_rz_a2m_drw.h"
#ifdef  USE_CONFIG_REDIRECT_DRW_API
#include  "dave_log.h"
#endif


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Constants: Macros in this file scope. Global static defines (GS)
************************************************************************/
#define  GS_EMPTY_AS_BOOL  ((bool_t) 2 )
#define  GS_EMPTY_AS_ENUM  (99)
#define  GS_EMPTY_AS_SIZE  (0xFFFFFFFFu)
#define  GS_PIXEL_FORMAT_FOR_FILL_OPERATION  (PIXEL_FORMAT_ARGB4444)


/******************************************************************************
Typedef definitions
******************************************************************************/

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
static errnum_t  gs_DrawImageResized_Sub( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );
static errnum_t  gs_DrawImageUsingMatrix_Sub( graphics_t*  self,  graphics_image_properties_t*  image_prop,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight );
static errnum_t  gs_FillRectPattern( graphics_t* self,
    int_fast32_t  in_MinX,   int_fast32_t  in_MinY,
    int_fast32_t  in_Width,  int_fast32_t  in_Height );
static errnum_t  gs_SetTextureOperation( d2_device*  d2_handle,
    graphics_composite_operation_t  in_Operation,  pixel_format_t  in_ImagePixelFormat,
    pixel_format_t  in_FramePixelFormat,  d2_u32  in_ColorOfA8 );
static errnum_t  gs_GetPixelFormatForDRW_Sub( pixel_format_t  in_PixelFormat,  d2_u32*  out_PixelFormat );

/* Variable: gs_SameAsImage */
static const int_fast32_t  gs_SameAsImage = -1;


/***********************************************************************
* Function Name: gs_ChangeMatrixIntToFloat
************************************************************************/
static errnum_t  gs_ChangeMatrixIntToFloat( graphics_t* self ) 
{
#if ! defined( RGA_INSIDE_RELEASE )
    if ( !( self->IsUseIntMatrix )) { return E_RGA_OTHER_50; }  /* Error: <Private/> */
#endif

    /* Cast to an appropriate type */
    self->Matrix2x3[0] = (graphics_matrix_float_t) 1.0;

    /* Cast to an appropriate type */
    self->Matrix2x3[1] = (graphics_matrix_float_t) 0.0;

    /* Cast to an appropriate type */
    self->Matrix2x3[2] = (graphics_matrix_float_t) 0.0;

    /* Cast to an appropriate type */
    self->Matrix2x3[3] = (graphics_matrix_float_t) 1.0;

    /* Cast to an appropriate type */
    self->Matrix2x3[4] = (graphics_matrix_float_t) self->IntMatrix_i_2[0];

    /* Cast to an appropriate type */
    self->Matrix2x3[5] = (graphics_matrix_float_t) self->IntMatrix_i_2[1];
    self->IsUseIntMatrix = false;
    return  0;
} /* End of function gs_ChangeMatrixIntToFloat */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetMatrixIntegerTranslate
************************************************************************/
errnum_t 
    R_GRAPHICS_H_GetMatrixIntegerTranslate 
            ( graphics_t*  self,  int_translate_t*  out )
{
    if ( self->IsUseIntMatrix )
    {
        int_fast32_t*  int_matrix_i_2 = self->IntMatrix_i_2;

        out->IsEnabled = true;

        out->Tx = int_matrix_i_2[0];
        out->Ty = int_matrix_i_2[1];
    }
    else
    {
        graphics_matrix_float_t*  matrix = self->Matrix2x3;

        bool_t  is_int_translate = (
            (matrix[0] == 1.0)  &&  (matrix[1] == 0.0)  &&
            (matrix[2] == 0.0)  &&  (matrix[3] == 1.0) );

        if ( is_int_translate )
        {
            /* Cast to an appropriate type */
            out->Tx = (int_fast32_t) matrix[4];

            /* Cast to an appropriate type */
            out->Ty = (int_fast32_t) matrix[5];

            out->IsEnabled = (

                /* Cast to an appropriate type */
                (matrix[4] == (graphics_matrix_float_t) out->Tx)  &&

                /* Cast to an appropriate type */
                (matrix[5] == (graphics_matrix_float_t) out->Ty) );

                /* Enables, if integer value */

            if ( out->IsEnabled )
            {
                self->IntMatrix_i_2[0] = out->Tx;
                self->IntMatrix_i_2[1] = out->Ty;
                self->IsUseIntMatrix = true;
            }
        }
        else
            { out->IsEnabled = false; }
    }
    return  0;
} /* End of function R_GRAPHICS_H_GetMatrixIntegerTranslate */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Initialize
************************************************************************/
errnum_t  R_GRAPHICS_H_Initialize 
            ( graphics_t*  self,  graphics_config_t*  in_out_Config )
{
    errnum_t    e;
    d2_device*  d2_handle;

    /* Guard */
    if (!( self->DRW_D2 == NULL ))
    {
        e=E_RGA_STATE_4;
        goto fin;
    }

    /* Error: "graphics_t::DRW_D2" variable is not initialized by 0 initializer. */


    /* in_out_Config = empty value */
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_HAS_FRAME_PIPELINE ) )
    {
        /* Cast to an appropriate type */
        in_out_Config->has_frame_pipeline = GS_EMPTY_AS_BOOL;
        in_out_Config->flags |= F_GRAPHICS_HAS_FRAME_PIPELINE;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_JPEG_DECODER ) )
    {
        /* Cast to an appropriate type */
        in_out_Config->jpeg_decoder = GS_EMPTY_AS_ENUM;
        in_out_Config->flags |= F_GRAPHICS_JPEG_DECODER;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_WORK_BUFFER_B_ADDRESS ) )
    {
        /* Cast to an appropriate type */
        in_out_Config->work_buffer_b_address = NULL;
        in_out_Config->flags |= F_GRAPHICS_WORK_BUFFER_B_ADDRESS;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_WORK_BUFFER_B_SIZE ) )
    {
        in_out_Config->work_buffer_b_size = GS_EMPTY_AS_SIZE;
        in_out_Config->flags |= F_GRAPHICS_WORK_BUFFER_B_SIZE;
    }


    /* in_out_Config = default value */
    if ( in_out_Config->has_frame_pipeline == GS_EMPTY_AS_BOOL )
    {
        in_out_Config->has_frame_pipeline = false;
    }
    if ( in_out_Config->jpeg_decoder == GS_EMPTY_AS_ENUM )
    {
        in_out_Config->jpeg_decoder = GRAPHICS_JPEG_DECODER_HARD;
    }
    if ( in_out_Config->work_buffer_b_size == GS_EMPTY_AS_SIZE )
    {
        in_out_Config->work_buffer_b_size = 0;
    }


    /* (variables) = ... */
    self->HasFramePipeline = in_out_Config->has_frame_pipeline;
    self->JPEG_Decoder = in_out_Config->jpeg_decoder;
    self->WorkBufferB_Address = in_out_Config->work_buffer_b_address;
    self->WorkBufferB_Size = in_out_Config->work_buffer_b_size;
    self->IsPreviousFrameEmpty = true;

    /* Cast to an appropriate type */
    self->DRW_RenderBuffer = NULL;
    self->DRW_TextureMode = 0;
    self->DRW_D2 = d2_opendevice( 0 );
    d2_handle = self->DRW_D2;


    /* d2_inithw */
    e= d2_inithw( d2_handle,  0 );
    if(e)
    {
        goto fin;
    }
    if ( self->HasFramePipeline )
    {

        e= d2_startframe( d2_handle );
        if(e)
        {
            goto fin;
        }

        /* Cast to an appropriate type */
        self->DRW_RenderBuffer = NULL;
    }
    else
    {
        d2_u32  display_list_block_size = d2_getdlistblocksize( d2_handle );
        self->DRW_RenderBuffer = d2_newrenderbuffer( d2_handle,
            display_list_block_size,
            display_list_block_size );

        /* Cast to an appropriate type */
        if ( self->DRW_RenderBuffer == NULL )
        {
            e=E_RGA_FEW_MEMORY_A;
            goto fin;
        }
            /* Error: <E_FEW_MEMORY/> */

        e= d2_selectrenderbuffer( d2_handle,  self->DRW_RenderBuffer );
        if(e)
        {
            goto fin;
        }
    }


    /* R_RGA_InitializeJCU */
    if ( self->JPEG_Decoder == GRAPHICS_JPEG_DECODER_HARD )
    {
        e= R_RGA_InitializeJCU();
        if(e)
        {
            goto fin;
        }
    }


    /* FrameBuffer = ... */
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_FRAME_BUFFER ) )
    {
        e=E_RGA_OTHER_DE;
        goto fin;
    } /* Error: Not set "F_GRAPHICS_FRAME_BUFFER" */
    if ( in_out_Config->frame_buffer == NULL )
    {
        e=E_RGA_OTHER_DF;
        goto fin;
    } /* Error: Not set "graphics_config_t::frame_buffer" */

    self->FrameBuffer = in_out_Config->frame_buffer;

    /* Cast to an appropriate type */
    self->CurrentFrameBufferUserAddress = NULL;

    /* Cast to an appropriate type */
    self->CurrentFrameBufferCachedAddress = NULL;
    self->GlobalCompositeOperation = GRAPHICS_SOURCE_OVER;

    e= gs_SetFrameBuffer_Sub( self );

    if(e)
    {
        goto fin;
    }


    /* Set default settings of DRW */
    e= d2_setblendmode( d2_handle,  d2_bm_alpha,  d2_bm_one_minus_alpha );
    if(e)
    {
        goto fin;
    }
    e= d2_setalphamode( d2_handle,  d2_am_constant );
    if(e)
    {
        goto fin;
    }

    e=0;
fin:
    if ( e )
    {
        R_GRAPHICS_H_Finalize( self,  e );
    }
    R_ERROR_Set( e );
    return  e;
} /* End of function R_GRAPHICS_H_Initialize */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Finalize
************************************************************************/
errnum_t  R_GRAPHICS_H_Finalize( graphics_t* self,  errnum_t e ) 
{
    errnum_t  ee;


    /* R_RGA_FinalizeJCU */
    if ( self->JPEG_Decoder == GRAPHICS_JPEG_DECODER_HARD )
    {
        ee= R_RGA_FinalizeJCU();
        if(e==0)
        {
            e=ee;
        }
    }


    /* d2_closedevice */
    if ( self->DRW_D2 != NULL )
    {
        d2_device*  d2_handle = self->DRW_D2;

        if ( self->HasFramePipeline )
        {
            ee= d2_endframe( d2_handle );
            if(e==0)
            {
                e=ee;
            }  /* Wait for H/W for previous previous frame */
        }
        else
        {
            /* Cast to an appropriate type */
            if ( self->DRW_RenderBuffer != NULL )
            {
                ee= d2_freerenderbuffer( d2_handle,  self->DRW_RenderBuffer );
                if(e==0)
                {
                    e=ee;
                }
            }
        }
        ee= d2_closedevice( d2_handle );
        if(e==0)
        {
            e=ee;
        }

        /* Cast to an appropriate type */
        self->DRW_D2 = NULL;
    }

    R_ERROR_Set( e );
    return  e;
} /* End of function R_GRAPHICS_H_Finalize */


/***********************************************************************
* Function Name: R_GRAPHICS_H_SetFrameBuffer
************************************************************************/
errnum_t  R_GRAPHICS_H_SetFrameBuffer 
            ( graphics_t*  self,  frame_buffer_t*  in_FrameBuffer )
{
    self->FrameBuffer = in_FrameBuffer;

    /* Cast to an appropriate type */
    self->CurrentFrameBufferCachedAddress = NULL;  /* For in next function */
    return  gs_SetFrameBuffer_Sub( self );
} /* End of function R_GRAPHICS_H_SetFrameBuffer */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetFrameBuffer
************************************************************************/
errnum_t  R_GRAPHICS_H_GetFrameBuffer 
            ( graphics_t*  self,  frame_buffer_t**  out_FrameBuffer )
{
    *out_FrameBuffer = self->FrameBuffer;
    return  0;
} /* End of function R_GRAPHICS_H_GetFrameBuffer */


/***********************************************************************
* Function Name: gs_SetFrameBuffer_Sub
************************************************************************/
static errnum_t  gs_SetFrameBuffer_Sub( graphics_t* self ) 
{
    errnum_t         e;
    d2_device*       d2_handle = self->DRW_D2;
    frame_buffer_t*  frame = self->FrameBuffer;
    uint8_t*         frame_buffer_address;
    uint8_t*         frame_buffer_cached_address;
    uintptr_t        frame_buffer_physical_address;
    d2_u32           d2_pixel_format;


    /* (variables) = ... */
    if ( self->HasFramePipeline )
    {
        frame_buffer_address = frame->buffer_address[ frame->show_buffer_index ];  /* next next frame */
    }
    else
    {
        frame_buffer_address = frame->buffer_address[ frame->draw_buffer_index ];  /* next frame */
    }
    self->CurrentFrameBufferUserAddress = frame_buffer_address;


    /* Guard */
    if ( !(
        (frame->pixel_format == PIXEL_FORMAT_RGB565)  ||
        (frame->pixel_format == PIXEL_FORMAT_ARGB4444)  ||
        (frame->pixel_format == PIXEL_FORMAT_ARGB8888)  ||
        (frame->pixel_format == PIXEL_FORMAT_XRGB8888)  ||
        (frame->pixel_format == PIXEL_FORMAT_A8 )))
        {
            e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3F;
            goto fin;
        }

            /* Error: The pixel format of frame buffer is not supported. */
            /* DRW hardware is not supported ARGB1555 of frame buffer. */

    /* frame_buffer_cached_address = ... */
    e= R_DRW_ToCachedAddress( frame_buffer_address,  &frame_buffer_cached_address );
    if ( e )
    {
        e=E_RGA_ACCESS_DENIED_20;
        goto fin;
    }

    /* Error: The memory of frame buffer must be cached memory. */

    /* Skip the following operations for faster, if "CurrentFrameBufferCachedAddress" is not changed. */
    if ( frame_buffer_cached_address == self->CurrentFrameBufferCachedAddress )
    {
        e=0;
        goto fin;
    }


    /* (variables) = ... */
    self->CurrentFrameBufferCachedAddress = frame_buffer_cached_address;
    e= gs_GetPixelFormatForDRW_Sub( frame->pixel_format,  &d2_pixel_format );
    if(e)
    {
        goto fin;
    }

        /* d2_pixel_format = . */


    /* d2_framebuffer */
    e= d2_framebuffer( d2_handle,

        /* Cast to an appropriate type */
        (void*) frame_buffer_cached_address,
        frame->stride / frame->byte_per_pixel,
        frame->width,  frame->height,  d2_pixel_format );
    if(e)
    {
        goto fin;
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function gs_SetFrameBuffer_Sub */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Start
************************************************************************/
errnum_t  R_GRAPHICS_H_Start( graphics_t*  self ) 
{
    errnum_t  e;
    bool_t    back_up = self->HasFramePipeline;

    self->HasFramePipeline = true;  /* For avoiding checking */

    e= R_GRAPHICS_H_FinishPreviousFrame( self );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_H_StartPreviousFrame( self );
    if(e)
    {
        goto fin;
    }

    e=0;
fin:
    {
        self->HasFramePipeline = back_up;
    }
    R_ERROR_Set( e );
    return  e;
} /* End of function R_GRAPHICS_H_Start */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Finish
*    This function is slow. See R_GRAPHICS_H_FinishPreviousFrame
************************************************************************/
errnum_t  R_GRAPHICS_H_Finish( graphics_t*  self ) 
{
    errnum_t  e;
    d2_device*  d2_handle = self->DRW_D2;

    if ( self->SHS_IsRequestedHardwareRendering )
    {

        e= d2_executerenderbuffer( d2_handle,  self->DRW_RenderBuffer,  0 );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRunningHardwareRendering = true;
    }
    if ( self->SHS_IsRunningHardwareRendering )
    {

        e= d2_flushframe( d2_handle );
        if(e)
        {
            goto fin;
        }
        e= d2_selectrenderbuffer( d2_handle,  self->DRW_RenderBuffer );
        if(e)
        {
            goto fin;
        }

        self->SHS_IsRequestedHardwareRendering = false;
    }
    self->IsPreviousFrameEmpty = true;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_Finish */


/***********************************************************************
* Function Name: R_GRAPHICS_H_FinishPreviousFrame
************************************************************************/
errnum_t  R_GRAPHICS_H_FinishPreviousFrame( graphics_t*  self ) 
{
    errnum_t    e;
    d2_device*  d2_handle = self->DRW_D2;

    if ( !( self->HasFramePipeline ))
    {
        e=E_RGA_STATE_5;
        goto fin;
    }

    /* Error: Not supported in "graphics_config_t::has_frame_pipeline == false". */

    e= d2_endframe( d2_handle );
    if(e)
    {
        goto fin;
    }
    self->IsPreviousFrameEmpty = true;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_FinishPreviousFrame */


/***********************************************************************
* Function Name: R_GRAPHICS_H_StartPreviousFrame
************************************************************************/
errnum_t  R_GRAPHICS_H_StartPreviousFrame( graphics_t*  self ) 
{
    errnum_t    e;
    d2_device*  d2_handle = self->DRW_D2;

    if ( !( self->HasFramePipeline ))
    {
        e=E_RGA_STATE_6;
        goto fin;
    }

    /* Error: Not supported in "graphics_config_t::has_frame_pipeline == false". */

    e= d2_startframe( d2_handle );
    if(e)
    {
        goto fin;
    }
    self->IsPreviousFrameEmpty = false;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_StartPreviousFrame */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetHasFramePipeline
************************************************************************/
bool_t  R_GRAPHICS_H_GetHasFramePipeline( graphics_t*  self ) 
{
    return  self->HasFramePipeline;
} /* End of function R_GRAPHICS_H_GetHasFramePipeline */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetIsPreviousFrameEmpty
************************************************************************/
bool_t
    R_GRAPHICS_H_GetIsPreviousFrameEmpty  
        ( graphics_t*  self )
{
    return  self->IsPreviousFrameEmpty;
} /* End of function R_GRAPHICS_H_GetIsPreviousFrameEmpty */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetIsCurrentFrameEmpty
************************************************************************/
bool_t  R_GRAPHICS_H_GetIsCurrentFrameEmpty( graphics_t*  self ) 
{
    return  d2_getisemptydlistentry( self->DRW_D2 );
} /* End of function R_GRAPHICS_H_GetIsCurrentFrameEmpty */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Save
************************************************************************/
errnum_t  R_GRAPHICS_H_Save 
            ( graphics_t* self,  graphics_status_t*  out_Status )
{
    errnum_t  e;

    if ( out_Status->IsEnabled )
    {
        e=E_RGA_OTHER_4F;
        goto fin;
    }

    /* Error: The first member variable of "graphics_status_t" type is not initialized by 0 initializer. */

    out_Status->IsEnabled = true;

    R_GRAPHICS_GetBackgroundColor( self, &out_Status->ClearColor );

    {
        int_fast32_t*  source      =       self->IntMatrix_i_2;
        int_fast32_t*  destination = out_Status->IntMatrix_i_2;

        destination[0] = source[0];
        destination[1] = source[1];
        out_Status->IsUseIntMatrix = self->IsUseIntMatrix;
    }

    out_Status->DRW_FillColor   = self->DRW_FillColor;
    out_Status->DRW_FillAlpha   = self->DRW_FillAlpha;
    out_Status->DRW_StrokeColor = self->DRW_StrokeColor;
    out_Status->DRW_StrokeAlpha = self->DRW_StrokeAlpha;
    out_Status->DRW_TextureMode = self->DRW_TextureMode;
    out_Status->GlobalAlpha     = self->GlobalAlpha;
    out_Status->GlobalCompositeOperation = self->GlobalCompositeOperation;
    out_Status->ClippingRectangle = self->ClippingRectangle;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_Save */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Restore
************************************************************************/
errnum_t  R_GRAPHICS_H_Restore 
            ( graphics_t* self,  graphics_status_t*  in_Status,  errnum_t  e )
{
    if ( ! in_Status->IsEnabled )
    {
        goto fin;
    }

    in_Status->IsEnabled = false;

    R_GRAPHICS_SetBackgroundColor( self,  in_Status->ClearColor );

    {
        int_fast32_t*  source      = in_Status->IntMatrix_i_2;
        int_fast32_t*  destination =      self->IntMatrix_i_2;

        destination[0] = source[0];
        destination[1] = source[1];
        self->IsUseIntMatrix = in_Status->IsUseIntMatrix;
    }

    self->DRW_FillColor   = in_Status->DRW_FillColor;
    self->DRW_FillAlpha   = in_Status->DRW_FillAlpha;
    self->DRW_StrokeColor = in_Status->DRW_StrokeColor;
    self->DRW_StrokeAlpha = in_Status->DRW_StrokeAlpha;
    self->DRW_TextureMode = in_Status->DRW_TextureMode;
    self->GlobalAlpha     = in_Status->GlobalAlpha;
    self->GlobalCompositeOperation = in_Status->GlobalCompositeOperation;
    self->ClippingRectangle = in_Status->ClippingRectangle;

fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_Restore */


/***********************************************************************
* Function Name: R_GRAPHICS_H_ResetMatrix
************************************************************************/
errnum_t  R_GRAPHICS_H_ResetMatrix( graphics_t*  self ) 
{
    self->IsUseIntMatrix = true;
    self->IntMatrix_i_2[0] = 0;
    self->IntMatrix_i_2[1] = 0;
    return  0;
} /* End of function R_GRAPHICS_H_ResetMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_H_SetMatrix_2x3
************************************************************************/
errnum_t  R_GRAPHICS_H_SetMatrix_2x3( graphics_t*  self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty )
{
    self->Matrix2x3[0] = in_Sx;
    self->Matrix2x3[1] = in_Ky;
    self->Matrix2x3[2] = in_Kx;
    self->Matrix2x3[3] = in_Sy;
    self->Matrix2x3[4] = in_Tx;
    self->Matrix2x3[5] = in_Ty;
    self->IsUseIntMatrix = false;
    return  0;
} /* End of function R_GRAPHICS_H_SetMatrix_2x3 */


/***********************************************************************
* Function Name: R_GRAPHICS_H_SetMatrix_3x3
************************************************************************/
errnum_t  R_GRAPHICS_H_SetMatrix_3x3 
            ( graphics_t*  self,  const graphics_matrix_float_t*  in_Matrix )
{
    errnum_t  e;

    /* Guard */
    if ( !( in_Matrix[2] == 0.0 ))
    {
        e=E_RGA_3X3MATRIX_1;
        goto fin;
    } /* Error: Matrix is not 2x3. */
    if ( !( in_Matrix[5] == 0.0 ))
    {
        e=E_RGA_3X3MATRIX_2;
        goto fin;
    } /* Error: Matrix is not 2x3. */
    if ( !( in_Matrix[8] == 1.0 ))
    {
        e=E_RGA_3X3MATRIX_3;
        goto fin;
    } /* Error: Matrix is not 2x3. */

    /* Matrix2x3 = ... */
    self->Matrix2x3[0] = in_Matrix[0];
    self->Matrix2x3[1] = in_Matrix[1];
    self->Matrix2x3[2] = in_Matrix[3];
    self->Matrix2x3[3] = in_Matrix[4];
    self->Matrix2x3[4] = in_Matrix[6];
    self->Matrix2x3[5] = in_Matrix[7];
    self->IsUseIntMatrix = false;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_SetMatrix_3x3 */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetMatrix_3x3
************************************************************************/
errnum_t  R_GRAPHICS_H_GetMatrix_3x3 
            ( graphics_t*  self,  graphics_matrix_float_t*  out_Matrix )
{
    if ( self->IsUseIntMatrix )
    {
        gs_ChangeMatrixIntToFloat( self );
    }
    out_Matrix[0] = self->Matrix2x3[0];
    out_Matrix[1] = self->Matrix2x3[1];
    out_Matrix[2] = 0.0;
    out_Matrix[3] = self->Matrix2x3[2];
    out_Matrix[4] = self->Matrix2x3[3];
    out_Matrix[5] = 0.0;
    out_Matrix[6] = self->Matrix2x3[4];
    out_Matrix[7] = self->Matrix2x3[5];
    out_Matrix[8] = 1.0;
    return  0;
} /* End of function R_GRAPHICS_H_GetMatrix_3x3 */


/***********************************************************************
* Function Name: R_GRAPHICS_H_TranslateMatrixI
************************************************************************/
errnum_t  R_GRAPHICS_H_TranslateMatrixI( graphics_t* self, 
    int_fast32_t  in_Tx,  int_fast32_t  in_Ty )
{
    /* IntMatrix_i_2 = ... */
    if ( self->IsUseIntMatrix )
    {
        self->IntMatrix_i_2[0] += in_Tx;
        self->IntMatrix_i_2[1] += in_Ty;
    }

    /* Matrix2x3 = ... */
    else
    {
        graphics_matrix_float_t*  matrix = self->Matrix2x3;

        /*
        * [ 0 2 4 ]   [ 0 2 4 ][ 1.0  0.0  tx  ]
        * [ 1 3 5 ] = [ 1 3 5 ][ 0.0  1.0  ty  ]
        * [ - - - ]   [ - - - ][ 0.0  0.0  1.0 ]
        */

        #if 0 
        matrix[0] = matrix[0];
        matrix[1] = matrix[1];
        matrix[2] = matrix[2];
        matrix[3] = matrix[3];
        #endif
        matrix[4] = (matrix[0] * in_Tx)  +  (matrix[2] * in_Ty)  +  matrix[4];
        matrix[5] = (matrix[1] * in_Tx)  +  (matrix[3] * in_Ty)  +  matrix[5];
    }
    return  0;
} /* End of function R_GRAPHICS_H_TranslateMatrixI */


/***********************************************************************
* Function Name: R_GRAPHICS_H_TranslateMatrix
************************************************************************/
errnum_t  R_GRAPHICS_H_TranslateMatrix( graphics_t* self, 
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty )
{
    graphics_matrix_float_t*  matrix = self->Matrix2x3;


    /* IntMatrix_i_2 = ... */
    if ( self->IsUseIntMatrix )
    {
        /* Cast to an appropriate type */
        int_fast32_t   tx_int = (int_fast32_t) in_Tx;

        /* Cast to an appropriate type */
        int_fast32_t   ty_int = (int_fast32_t) in_Ty;

        if (
            /* Cast to an appropriate type */
            ((graphics_matrix_float_t) tx_int == in_Tx)  &&

            /* Cast to an appropriate type */
            ((graphics_matrix_float_t) ty_int == in_Ty) )
        {
            self->IntMatrix_i_2[0] += tx_int;
            self->IntMatrix_i_2[1] += ty_int;
            goto  fin;
        }

        gs_ChangeMatrixIntToFloat( self );
    }


    /* Matrix2x3 = ... */
    /*
    * [ 0 2 4 ]   [ 0 2 4 ][ 1.0  0.0  tx  ]
    * [ 1 3 5 ] = [ 1 3 5 ][ 0.0  1.0  ty  ]
    * [ - - - ]   [ - - - ][ 0.0  0.0  1.0 ]
    */

    #if 0 
    matrix[0] = matrix[0];
    matrix[1] = matrix[1];
    matrix[2] = matrix[2];
    matrix[3] = matrix[3];
    #endif
    matrix[4] = (matrix[0] * in_Tx)  +  (matrix[2] * in_Ty)  +  matrix[4];
    matrix[5] = (matrix[1] * in_Tx)  +  (matrix[3] * in_Ty)  +  matrix[5];

fin:
    {
        return 0;
    }
} /* End of function R_GRAPHICS_H_TranslateMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_H_ScaleMatrix
************************************************************************/
errnum_t  R_GRAPHICS_H_ScaleMatrix( graphics_t* self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Sy )
{
    graphics_matrix_float_t*  matrix = self->Matrix2x3;

    if ( self->IsUseIntMatrix )
    {
        gs_ChangeMatrixIntToFloat( self );
    }

    /*
    * [ 0 2 4 ]   [ 0 2 4 ][ sx   0.0  0.0 ]
    * [ 1 3 5 ] = [ 1 3 5 ][ 0.0  sy   0.0 ]
    * [ - - - ]   [ - - - ][ 0.0  0.0  1.0 ]
    */

    matrix[0] = matrix[0] * in_Sx;
    matrix[1] = matrix[1] * in_Sx;
    matrix[2] = matrix[2] * in_Sy;
    matrix[3] = matrix[3] * in_Sy;
    #if 0 
    matrix[4] = matrix[4];
    matrix[5] = matrix[5];
    #endif
    return  0;
} /* End of function R_GRAPHICS_H_ScaleMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_H_RotateMatrixDegree
************************************************************************/
errnum_t  R_GRAPHICS_H_RotateMatrixDegree
            ( graphics_t* self,  graphics_matrix_float_t  in_Degree )
{
    graphics_matrix_float_t*  new_matrix = self->Matrix2x3;
    graphics_matrix_float_t   matrix[2*3];
    graphics_matrix_float_t   radian = in_Degree * ( 3.14159 / 180 );
    graphics_matrix_float_t   sin_value = sinf( radian );
    graphics_matrix_float_t   cos_value = cosf( radian );


    /* matrix = ... : For avoiding collision with "new_matrix" */
    if ( self->IsUseIntMatrix )
    {
        gs_ChangeMatrixIntToFloat( self );
    }
    matrix[0] = self->Matrix2x3[0];
    matrix[1] = self->Matrix2x3[1];
    matrix[2] = self->Matrix2x3[2];
    matrix[3] = self->Matrix2x3[3];


    /* new_matrix = ... */
    /*
    * [ 0 2 4 ]   [ 0 2 4 ][ cos -sin  0.0 ]
    * [ 1 3 5 ] = [ 1 3 5 ][ sin  cos  0.0 ]
    * [ - - - ]   [ - - - ][ 0.0  0.0  1.0 ]
    */

    new_matrix[0] =   (matrix[0] * cos_value)  +  (matrix[2] * sin_value);
    new_matrix[1] =   (matrix[1] * cos_value)  +  (matrix[3] * sin_value);
    new_matrix[2] = ((- matrix[0]) * sin_value)  +  (matrix[2] * cos_value);
    new_matrix[3] = ((- matrix[1]) * sin_value)  +  (matrix[3] * cos_value);
    #if 0 
    new_matrix[4] = matrix[4];
    new_matrix[5] = matrix[5];
    #endif
    return  0;
} /* End of function R_GRAPHICS_H_RotateMatrixDegree */


/***********************************************************************
* Function Name: R_GRAPHICS_H_ShearMatrix
************************************************************************/
errnum_t  R_GRAPHICS_H_ShearMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Shx,  graphics_matrix_float_t  in_Shy )
{
    graphics_matrix_float_t*  new_matrix = self->Matrix2x3;
    graphics_matrix_float_t   matrix[2*3];


    /* matrix = ... : For avoiding collision with "new_matrix" */
    if ( self->IsUseIntMatrix )
    {
        gs_ChangeMatrixIntToFloat( self );
    }
    matrix[0] = self->Matrix2x3[0];
    matrix[1] = self->Matrix2x3[1];
    matrix[2] = self->Matrix2x3[2];
    matrix[3] = self->Matrix2x3[3];


    /* new_matrix = ... */
    /*
    * [ 0 2 4 ]   [ 0 2 4 ][ 1.0  shx  0.0 ]
    * [ 1 3 5 ] = [ 1 3 5 ][ shy  1.0  0.0 ]
    * [ - - - ]   [ - - - ][ 0.0  0.0  1.0 ]
    */

    new_matrix[0] = matrix[0]           +  (matrix[2] * in_Shy);
    new_matrix[1] = matrix[1]           +  (matrix[3] * in_Shy);
    new_matrix[2] = (matrix[0] * in_Shx)  +  matrix[2];
    new_matrix[3] = (matrix[1] * in_Shx)  +  matrix[3];
    #if 0 
    new_matrix[4] = matrix[4];
    new_matrix[5] = matrix[5];
    #endif
    return  0;
} /* End of function R_GRAPHICS_H_ShearMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_H_TransformMatrix
************************************************************************/
errnum_t  R_GRAPHICS_H_TransformMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty )
{
    graphics_matrix_float_t*  new_matrix = self->Matrix2x3;
    graphics_matrix_float_t   matrix[2*3];


    /* matrix = ... : For avoiding collision with "new_matrix" */
    if ( self->IsUseIntMatrix )
    {
        gs_ChangeMatrixIntToFloat( self );
    }
    matrix[0] = self->Matrix2x3[0];
    matrix[1] = self->Matrix2x3[1];
    matrix[2] = self->Matrix2x3[2];
    matrix[3] = self->Matrix2x3[3];
    matrix[4] = self->Matrix2x3[4];
    matrix[5] = self->Matrix2x3[5];


    /* new_matrix = ... */
    /*
    * [ 0 2 4 ]   [ 0 2 4 ][ sx   kx   tx  ]
    * [ 1 3 5 ] = [ 1 3 5 ][ ky   sy   ty  ]
    * [ - - - ]   [ - - - ][ 0.0  0.0  1.0 ]
    */

    new_matrix[0] = (matrix[0] * in_Sx)  +  (matrix[2] * in_Ky);
    new_matrix[1] = (matrix[1] * in_Sx)  +  (matrix[3] * in_Ky);
    new_matrix[2] = (matrix[0] * in_Kx)  +  (matrix[2] * in_Sy);
    new_matrix[3] = (matrix[1] * in_Kx)  +  (matrix[3] * in_Sy);
    new_matrix[4] = (matrix[0] * in_Tx)  +  (matrix[2] * in_Ty)  +  matrix[4];
    new_matrix[5] = (matrix[1] * in_Tx)  +  (matrix[3] * in_Ty)  +  matrix[5];
    return  0;
} /* End of function R_GRAPHICS_H_TransformMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_H_MultiplyMatrix
************************************************************************/
errnum_t  R_GRAPHICS_H_MultiplyMatrix 
            ( graphics_t* self,  graphics_matrix_float_t*  in_Matrix_3x3 )
{
    errnum_t  e;

    /* Guard */
    if ( !( in_Matrix_3x3[2] == 0.0 ))
    {
        e=E_RGA_3X3MATRIX_4;
        goto fin;
    } /* Error: Matrix is not 2x3. */
    if ( !( in_Matrix_3x3[5] == 0.0 ))
    {
        e=E_RGA_3X3MATRIX_5;
        goto fin;
    } /* Error: Matrix is not 2x3. */
    if ( !( in_Matrix_3x3[8] == 1.0 ))
    {
        e=E_RGA_3X3MATRIX_6;
        goto fin;
    } /* Error: Matrix is not 2x3. */

    /* R_GRAPHICS_H_TransformMatrix */
    e= R_GRAPHICS_H_TransformMatrix( self,
        in_Matrix_3x3[0],  in_Matrix_3x3[1],
        in_Matrix_3x3[3],  in_Matrix_3x3[4],
        in_Matrix_3x3[6],  in_Matrix_3x3[7] );
    if(e)
    {
        goto fin;
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_MultiplyMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_H_DrawImage
************************************************************************/
errnum_t  R_GRAPHICS_H_DrawImage
            ( graphics_t*  self,  const graphics_image_t*  in_Image,
                int_fast32_t  in_MinX,  int_fast32_t  in_MinY )
{
    return  gs_DrawImageResized_Sub( self,  in_Image,
        in_MinX,  in_MinY,  gs_SameAsImage,  gs_SameAsImage );
}/* End of function R_GRAPHICS_H_DrawImage */


/***********************************************************************
* Function Name: R_GRAPHICS_H_DrawImageResized
************************************************************************/
errnum_t  R_GRAPHICS_H_DrawImageResized 
    ( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    return  gs_DrawImageResized_Sub( self,  in_Image,
        in_MinX,  in_MinY,  in_Width,  in_Height );
} /* End of function R_GRAPHICS_H_DrawImageResized */


/***********************************************************************
* Function Name: gs_DrawImageResized_Sub
************************************************************************/
static errnum_t  gs_DrawImageResized_Sub  
                    ( graphics_t*  self,  const graphics_image_t*  in_Image,
                    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t      e;
    d2_device*    d2_handle = self->DRW_D2;
    d2_u32        image_pixel_format;
    uintptr_t     image_physical_address;
    int_fast32_t  destination_width;
    int_fast32_t  destination_height;
    int32_t       dxu;
    int32_t       dyv;
    enum {        fix4  =  4 };  /* Count of bit in fixed point */ 
    enum {        fix16 = 16 };  /* Count of bit in fixed point */ 

    graphics_image_properties_t  image_prop;
    int_translate_t              int_translate;


    /* R_GRAPHICS_DrawImageChild */
    if ( in_Image->flags  &  F_T_IMAGE_INF_PADDING_AT_RIGHT )
    {
        int_fast32_t  image_width  = in_Image->width;
        int_fast32_t  image_height = in_Image->height;

        if ( (in_Width == gs_SameAsImage)  &&  (in_Height == gs_SameAsImage) )
        {

            e= R_GRAPHICS_DrawImageChild( self,  in_Image,
                0, 0,               image_width,  image_height,
                in_MinX,  in_MinY,  image_width,  image_height );
            if(e)
            {
                goto fin;
            }
        }
        else
        {
            e= R_GRAPHICS_DrawImageChild( self,  in_Image,
                0, 0,  image_width,  image_height,
                in_MinX,  in_MinY,  in_Width,  in_Height );
            if(e)
            {
                goto fin;
            }
        }
        e=0;
        goto fin;
    }


    /* (variables) = ... */
    {
        /* image_prop = ... */
        e= R_GRAPHICS_IMAGE_GetProperties( in_Image,  &image_prop );
        if(e)
        {
            goto fin;
        }

        /* image_physical_address = ... */
        e= R_MEMORY_ToPhysicalAddress( image_prop.pixels,  &image_physical_address );
        if(e)
        {
            goto fin;
        }

        /* destination_width = ... */
        if ( in_Width == gs_SameAsImage )
        {
            destination_width = image_prop.width;
        }
        else
        {
            destination_width = in_Width;
        }

        /* destination_height = ... */
        if ( in_Height == gs_SameAsImage )
        {
            destination_height = image_prop.height;
        }
        else
        {
            destination_height = in_Height;
        }
    }


    /* gs_DrawImageUsingMatrix_Sub */
    e= R_GRAPHICS_H_GetMatrixIntegerTranslate( self,  &int_translate );
    if(e)
    {
        goto fin;
    }
    if ( ! int_translate.IsEnabled )
    {

        e= gs_DrawImageUsingMatrix_Sub( self,  &image_prop,
            0,  0,  image_prop.width,  image_prop.height,
            in_MinX,  in_MinY,  destination_width,  destination_height );
        if(e)
        {
            goto fin;
        }
        e=0;
        goto fin;
    }
    in_MinX += int_translate.Tx;
    in_MinY += int_translate.Ty;


    /* (variables) = ... */
    {
        /* image_pixel_format = ... */
        e= gs_GetPixelFormatForDRW_Sub( image_prop.pixelFormat,  &image_pixel_format );
        if(e)
        {goto fin;
        }

        /* dxu = ... */
        if ( in_Width == gs_SameAsImage )
        {
            dxu = 1 << fix16;
        }
        else
        {
            /* Cast to an appropriate type */
            dxu = (int32_t)( (float_t)( image_prop.width << fix16 ) / ( in_Width ) );

            /* float_t is for faster */
        }

        /* dyv = ... */
        if ( in_Height == gs_SameAsImage )
        {
            dyv = 1 << fix16;
        }
        else
        {
            /* Cast to an appropriate type */
            dyv = (int32_t)( (float_t)( image_prop.height << fix16 ) / ( in_Height ) );

            /* float_t is for faster */
        }
    }

    /* d2_renderbox */
    e= d2_setfillmode( d2_handle,  d2_fm_texture );
    if(e)
    {
        goto fin;
    }
    e= d2_settexturemode( d2_handle,  d2_tm_wrapu | d2_tm_wrapv );
    if(e)
    {
        goto fin;
    }
    e= d2_setalpha( d2_handle,  self->GlobalAlpha );
    if(e)
    {
        goto fin;
    }
    e= gs_SetTextureOperation( d2_handle,  self->GlobalCompositeOperation,  image_prop.pixelFormat,
        self->FrameBuffer->pixel_format,  ( self->DRW_FillAlpha << 24 ) | ( self->DRW_FillColor ) );
    if(e)
    {
        goto fin;
    }

    /* Cast to an appropriate type */
    e= d2_settexture( d2_handle,  (void*) image_physical_address,
        image_prop.width,  image_prop.width,  image_prop.height,  image_pixel_format );
    if(e)
    {
        goto fin;
    }
    e= d2_settexturemapping( d2_handle,
        in_MinX << fix4,  in_MinY << fix4,
        0 << fix4,   0 << fix4,
        dxu,         0 << fix16,
        0 << fix16,  dyv );
    if(e)
    {
        goto fin;
    }

    e= d2_renderbox( d2_handle,
        in_MinX           << fix4,  in_MinY            << fix4,
        destination_width << fix4,  destination_height << fix4 );
    if(e)
    {
        goto fin;
    }
    self->SHS_IsRequestedHardwareRendering = true;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function gs_DrawImageResized_Sub */


/***********************************************************************
* Function Name: R_GRAPHICS_H_DrawImageChild
************************************************************************/
errnum_t  R_GRAPHICS_H_DrawImageChild 
    ( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight )
{
    errnum_t      e;
    d2_device*    d2_handle = self->DRW_D2;
    d2_u32        image_pixel_format;
    uintptr_t     image_physical_address;
    int32_t       dxu;
    int32_t       dyv;
    enum {        fix4  =  4 };  /* Count of bit in fixed point */ 
    enum {        fix16 = 16 };  /* Count of bit in fixed point */ 

    graphics_image_properties_t  image_prop;
    int_translate_t              int_translate;
    graphics_image_t             image_in_buffer;


    /* (variables) = ... */
    {
        /* in_Image = ... */
        if ( in_Image->flags  &  F_T_IMAGE_INF_PADDING_AT_RIGHT )
        {
            int_fast32_t  image_pitch = in_Image->color;
            image_in_buffer = *in_Image;

            /* Cast to an appropriate type */
            image_in_buffer.width = (uint16_t) image_pitch;
            image_in_buffer.flags &= (~F_T_IMAGE_INF_PADDING_AT_RIGHT);
            in_Image = &image_in_buffer;
        }

        /* int_translate = ... */
        e= R_GRAPHICS_H_GetMatrixIntegerTranslate( self,  &int_translate );
        if(e)
        {
            goto fin;
        }

        /* image_prop = ... */
        e= R_GRAPHICS_IMAGE_GetProperties( in_Image,  &image_prop );
        if(e)
        {
            goto fin;
        }

        /* image_physical_address = ... */
        e= R_MEMORY_ToPhysicalAddress( image_prop.pixels,  &image_physical_address );
        if(e)
        {
            goto fin;
        }
    }


    /* gs_DrawImageUsingMatrix_Sub */
    if ( ! int_translate.IsEnabled )
    {

        e= gs_DrawImageUsingMatrix_Sub( self,  &image_prop,
            in_SourceMinX,  in_SourceMinY,  in_SourceWidth,  in_SourceHeight,
            in_DestinationMinX,  in_DestinationMinY,  in_DestinationWidth,  in_DestinationHeight );
        goto fin;
    }
    in_DestinationMinX += int_translate.Tx;
    in_DestinationMinY += int_translate.Ty;


    /* image_pixel_format = ... */
    e= gs_GetPixelFormatForDRW_Sub( image_prop.pixelFormat,  &image_pixel_format );
    if(e)
    {
        goto fin;
    }

    /* (variables) = ... */
    dxu = (int32_t)( (float_t)( in_SourceWidth  << fix16 ) / ( in_DestinationWidth  ) );

    /* Cast to an appropriate type */
    dyv = (int32_t)( (float_t)( in_SourceHeight << fix16 ) / ( in_DestinationHeight ) );

    /* float_t is for faster */


    /* d2_renderbox */
    e= d2_setfillmode( d2_handle,  d2_fm_texture );
    if(e)
    {
        goto fin;
    }
    e= d2_settexturemode( d2_handle,  self->DRW_TextureMode );
    if(e)
    {
        goto fin;
    }
    e= d2_setalpha( d2_handle,  self->GlobalAlpha );
    if(e)
    {
        goto fin;
    }
    e= gs_SetTextureOperation( d2_handle,  self->GlobalCompositeOperation,  image_prop.pixelFormat,
        self->FrameBuffer->pixel_format,  ( self->DRW_FillAlpha << 24 ) | ( self->DRW_FillColor ) );
    if(e)
    {
        goto fin;
    }

    /* Cast to an appropriate type */
    e= d2_settexture( d2_handle,  (void*) image_physical_address,
        image_prop.width,  image_prop.width,  image_prop.height,  image_pixel_format );

    if(e)
    {
        goto fin;
    }
    e= d2_settexturemapping( d2_handle,
        in_DestinationMinX << fix4,  in_DestinationMinY << fix4,
        in_SourceMinX      <<fix16,  in_SourceMinY      <<fix16,
        dxu,                         0 << fix16,
        0 << fix16,                  dyv );
    if(e)
    {
        goto fin;
    }

    e= d2_renderbox( d2_handle,
        in_DestinationMinX  << fix4,  in_DestinationMinY   << fix4,
        in_DestinationWidth << fix4,  in_DestinationHeight << fix4 );
    if(e)
    {
        goto fin;
    }
    self->SHS_IsRequestedHardwareRendering = true;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_H_DrawImageChild */


/***********************************************************************
* Function Name: gs_DrawImageUsingMatrix_Sub
************************************************************************/
static errnum_t  gs_DrawImageUsingMatrix_Sub  
    ( graphics_t*  self,  graphics_image_properties_t*  image_prop,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight )
{
    errnum_t      e;
    errnum_t      ee;
    d2_device*    d2_handle = self->DRW_D2;
    d2_u32        image_pixel_format;
    uintptr_t     image_physical_address;
    int_fast32_t  x;
    int_fast32_t  y;
    d2_s32        dxu;
    d2_s32        dyu;
    d2_s32        dxv;
    d2_s32        dyv;
    enum {        fix4  = 4 };  /* Count of bit in fixed point */ 
    float_t       fix4_float  =    16.0;  /* For  4-bit shift fixed point */
    float_t       fix16_float = 65536.0;  /* For 16-bit shift fixed point */
    enum {        num_2_triangles  = 2 }; 

    /* Variable "destination" : x, y of quad points */
    struct { 
        d2_point  x;
        d2_point  y;
    } destination[4];

    graphics_matrix_float_t*  matrix = self->Matrix2x3;
    graphics_matrix_float_t   user_matrix[2*3];
    bool_t                    is_user_matrix = false;
    graphics_matrix_float_t   inverse_matrix[2*3];
    graphics_matrix_float_t   det;

    memcpy( user_matrix,  matrix,  sizeof( user_matrix ) );
    is_user_matrix = true;





    /* matrix = ( +0.5 => user_matrix => -0.5 ) */
    /* Bacause for the case of example "R_GRAPHICS_ScaleMatrix( self,  1.0,  -1.0 )" in "S_GlassReflection" */
    e= R_GRAPHICS_H_ResetMatrix( self );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_H_TranslateMatrix( self,  -0.5,  -0.5 );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_H_TransformMatrix( self,  user_matrix[0],  user_matrix[1],
        user_matrix[2],  user_matrix[3],  user_matrix[4],  user_matrix[5] );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_H_TranslateMatrix( self,  +0.5,  +0.5 );
    if(e)
    {
        goto fin;
    }


    /* (variables) = ... */
    {
        /* image_pixel_format = ... */
        e= gs_GetPixelFormatForDRW_Sub( image_prop->pixelFormat,  &image_pixel_format );
        if(e)
        {
            goto fin;
        }

        /* image_physical_address = ... */
        e= R_MEMORY_ToPhysicalAddress( image_prop->pixels,  &image_physical_address );
        if(e)
        {
            goto fin;
        }

        /* destination = ... */
        x = in_DestinationMinX;
        y = in_DestinationMinY;

        /* Cast to an appropriate type */
        destination[0].x = (d2_point)( fix4_float * ( (matrix[0] * x)  +  (matrix[2] * y)  +  matrix[4] ));

        /* Cast to an appropriate type */
        destination[0].y = (d2_point)( fix4_float * ( (matrix[1] * x)  +  (matrix[3] * y)  +  matrix[5] ));

        x = (in_DestinationMinX + in_DestinationWidth) - 1;
        y = in_DestinationMinY;

        /* Cast to an appropriate type */
        destination[1].x = (d2_point)( fix4_float * ( (matrix[0] * x)  +  (matrix[2] * y)  +  matrix[4] ));

        /* Cast to an appropriate type */
        destination[1].y = (d2_point)( fix4_float * ( (matrix[1] * x)  +  (matrix[3] * y)  +  matrix[5] ));

        x = in_DestinationMinX;
        y = (in_DestinationMinY + in_DestinationHeight) - 1;

        /* Cast to an appropriate type */
        destination[2].x = (d2_point)( fix4_float * ( (matrix[0] * x)  +  (matrix[2] * y)  +  matrix[4] ));

        /* Cast to an appropriate type */
        destination[2].y = (d2_point)( fix4_float * ( (matrix[1] * x)  +  (matrix[3] * y)  +  matrix[5] ));

        x = (in_DestinationMinX + in_DestinationWidth)  - 1;
        y = (in_DestinationMinY + in_DestinationHeight) - 1;

        /* Cast to an appropriate type */
        destination[3].x = (d2_point)( fix4_float * ( (matrix[0] * x)  +  (matrix[2] * y)  +  matrix[4] ));

        /* Cast to an appropriate type */
        destination[3].y = (d2_point)( fix4_float * ( (matrix[1] * x)  +  (matrix[3] * y)  +  matrix[5] ));

        /* inverse_matrix = ... */
        det = 1 / ( ( matrix[0] * matrix[3] ) - ( matrix[2] * matrix[1] ) );
        inverse_matrix[0] =   det * matrix[3];
        inverse_matrix[1] = (- det) * matrix[1];
        inverse_matrix[2] = (- det) * matrix[2];
        inverse_matrix[3] =   det * matrix[0];

        /* Cast to an appropriate type */
        dxu = (d2_s32)( inverse_matrix[0] * fix16_float );

        /* Cast to an appropriate type */
        dxv = (d2_s32)( inverse_matrix[1] * fix16_float );

        /* Cast to an appropriate type */
        dyu = (d2_s32)( inverse_matrix[2] * fix16_float );

        /* Cast to an appropriate type */
        dyv = (d2_s32)( inverse_matrix[3] * fix16_float );


        /* Change "destination" orientation from counterclockwise to clockwise. */
        if ( det < 0 )
        {
            d2_point  swap;

            swap = destination[2].x;  destination[2].x = destination[1].x;  destination[1].x = swap;
            swap = destination[2].y;  destination[2].y = destination[1].y;  destination[1].y = swap;
        }
    }

    /* d2_renderquad */
    e= d2_setfillmode( d2_handle,  d2_fm_texture );
    if(e)
    {
        goto fin;
    }
    e= d2_settexturemode( d2_handle,  self->DRW_TextureMode );
    if(e)
    {
        goto fin;
    }
    e= d2_setalpha( d2_handle,  self->GlobalAlpha );
    if(e)
    {
        goto fin;
    }
    e= gs_SetTextureOperation( d2_handle,  self->GlobalCompositeOperation,  image_prop->pixelFormat,
        self->FrameBuffer->pixel_format,  ( self->DRW_FillAlpha << 24 ) | ( self->DRW_FillColor ) );
    if(e)
    {
        goto fin;
    }

    /* Cast to an appropriate type */
    e= d2_settexture( d2_handle,  (void*) image_physical_address,
        image_prop->width,  image_prop->width,  image_prop->height,  image_pixel_format );
    if(e)
    {
        goto fin;
    }
    e= d2_settexturemapping( d2_handle,
        destination[0].x,       destination[0].y,
        in_SourceMinX << fix4,  in_SourceMinY << fix4,
        dxu,                    dyu,
        dxv,                    dyv );
    if(e)
    {
        goto fin;
    }

    e= d2_renderquad( d2_handle,
        destination[0].x,  destination[0].y,
        destination[1].x,  destination[1].y,
        destination[3].x,  destination[3].y,
        destination[2].x,  destination[2].y,
        0 );
    if(e)
    {
        goto fin;
    }
    self->SHS_IsRequestedHardwareRendering = true;

    e=0;
fin:
    if ( is_user_matrix )
    {
        memcpy( matrix,  user_matrix,  sizeof( user_matrix ) );
    }
    R_ERROR_Set( e );
    return  e;
} /* End of function gs_DrawImageUsingMatrix_Sub */


/***********************************************************************
* Function Name: R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage
************************************************************************/
errnum_t  R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage 
            ( graphics_t*  self,  graphics_image_t*  in_ImageInBuffer )
{
    errnum_t      e;
    errnum_t      ee;
    d2_device*    d2_handle = self->DRW_D2;
    enum {        fix4  =  4 };  /* Count of bit in fixed point */ 

    /* Cast to an appropriate type */
    void*   frame_address = NULL;
    d2_s32  frame_pitch;
    d2_u32  frame_width;
    d2_u32  frame_height;
    d2_s32  frame_format;

    int_fast32_t  image_pitch;


    if ( ! ( in_ImageInBuffer->flags & F_T_IMAGE_INF_PADDING_AT_RIGHT ) )
    {
        e=0; goto fin;
    }


    /* d2_framebuffer */
    {
        pixel_format_t  image_pixel_format;
        d2_u32          image_pixel_format_d2;

        void*  image_cached_address =

            /* Cast to an appropriate type */
            (uint8_t*)  in_ImageInBuffer +

            /* Cast to an appropriate type */
            (uintptr_t) in_ImageInBuffer->offset_to_image;

        image_pitch = in_ImageInBuffer->color;  /* pitch = stride / byte_per_pixel */
        e= GetPixelFormatEnumFrom_T_IMAGE_INF( &image_pixel_format,  in_ImageInBuffer->type );
        if(e)
        {
            goto fin;
        }

        /* image_pixel_format = . */
        e= gs_GetPixelFormatForDRW_Sub( image_pixel_format,  &image_pixel_format_d2 );
        if(e)
        {
            goto fin;
        }

        /* image_pixel_format_d2 = . */
        e= d2_getframebuffer( d2_handle,
            &frame_address,  &frame_pitch,  &frame_width,  &frame_height,  &frame_format );
        if(e)
        {
            goto fin;
        }

        e= d2_framebuffer( d2_handle,
            image_cached_address,
            image_pitch,
            in_ImageInBuffer->width,  in_ImageInBuffer->height,  image_pixel_format_d2 );
        if(e)
        {
            goto fin;
        }
    }


    /* destination out */
    e= d2_setfillmode( d2_handle,  d2_fm_color );
    if(e)
    {
        goto fin;
    }
    e= d2_settextureoperation( d2_handle,  d2_to_blend,  d2_to_copy,  d2_to_copy,  d2_to_copy );
    if(e)
    {
        goto fin;
    }
    e= d2_setblendmode(      d2_handle,  d2_bm_zero,  d2_bm_one );
    if(e)
    {
        goto fin;
    }
    e= d2_setalphablendmode( d2_handle,  d2_bm_zero,  d2_bm_one_minus_alpha );
    if(e)
    {
        goto fin;
    }


    /* d2_renderbox */
    {
        int_fast32_t  filling_width = image_pitch - in_ImageInBuffer->width;
        int_fast32_t  filling_height = in_ImageInBuffer->height;

        e= d2_setalpha( d2_handle,  0xFF );
        if(e)
        {
            goto fin;
        }

        e= d2_renderbox( d2_handle,
            0,  0,
            filling_width << fix4,  filling_height << fix4 );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRequestedHardwareRendering = true;
    }

    e=0;
fin:

    /* Cast to an appropriate type */
    if ( frame_address != NULL )
    {
        ee= d2_framebuffer( d2_handle,
            frame_address,  frame_pitch,  frame_width,  frame_height,  frame_format );
            if(e==0)
            {
                e=ee;
            }
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage */


/***********************************************************************
* Function Name: R_GRAPHICS_H_SetBackgroundColor
************************************************************************/
errnum_t  R_GRAPHICS_H_SetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t  in_Color )
{
    errnum_t  e;
    bool_t    has_frame_alpha;

    e= R_RGA_IsPixelFormatHavingAlpha( self->FrameBuffer->pixel_format,  &has_frame_alpha );
    if(e)
    {
        goto fin;
    }  /* has_frame_alpha = . */


    /* BackgroundColor = ... */
    self->BackgroundColor = in_Color;


    /* DRW_ClearColor = ... */
    if ( has_frame_alpha )
    {
        /* transparent black */
        self->DRW_ClearColor = 0x000000;
    }
    else
    {
        self->DRW_ClearColor =
            ( in_Color.u.Red   << 16 ) |
            ( in_Color.u.Green <<  8 ) |
            ( in_Color.u.Blue  <<  0 );
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_GRAPHICS_H_SetBackgroundColor */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetBackgroundColor
************************************************************************/
errnum_t  R_GRAPHICS_H_GetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t*  out_Color )
{
    *out_Color = self->BackgroundColor;
    return  0;
}/* End of function R_GRAPHICS_H_GetBackgroundColor */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetClearColor
************************************************************************/
errnum_t  R_GRAPHICS_H_GetClearColor 
            ( graphics_t* self,  r8g8b8a8_t*  out_Color )
{
    out_Color->u.Red   = ( self->DRW_ClearColor & 0xFF0000 ) >> 16;
    out_Color->u.Green = ( self->DRW_ClearColor & 0x00FF00 ) >>  8;
    out_Color->u.Blue  = ( self->DRW_ClearColor & 0x0000FF ) >>  0;
    return  0;
}/* End of function R_GRAPHICS_H_GetClearColor */


/***********************************************************************
* Function Name: R_GRAPHICS_H_SetFillColor
************************************************************************/
errnum_t  R_GRAPHICS_H_SetFillColor( graphics_t* self,  r8g8b8a8_t  in_Color ) 
{
    self->DRW_FillColor =
        ( in_Color.u.Red   << 16 ) |
        ( in_Color.u.Green <<  8 ) |
        ( in_Color.u.Blue  <<  0 );
    self->DRW_FillAlpha = in_Color.u.Alpha;
    return  0;
}/* End of function R_GRAPHICS_H_SetFillColor */


/***********************************************************************
* Function Name: R_GRAPHICS_H_SetStrokeColor
************************************************************************/
errnum_t  R_GRAPHICS_H_SetStrokeColor 
            ( graphics_t* self,  r8g8b8a8_t  in_Color )
{
    self->DRW_StrokeColor =
        ( in_Color.u.Red   << 16 ) |
        ( in_Color.u.Green <<  8 ) |
        ( in_Color.u.Blue  <<  0 );
    self->DRW_StrokeAlpha = in_Color.u.Alpha;
    return  0;
}/* End of function R_GRAPHICS_H_SetStrokeColor */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Clear
************************************************************************/
errnum_t  R_GRAPHICS_H_Clear( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t        e;
    errnum_t        ee;
    d2_device*      d2_handle = self->DRW_D2;
    enum {          fix4  =  4 };  /* Count of bit in fixed point */ 
    bool_t          has_frame_alpha;
    const d2_alpha  not_initialized_alpha = 0x01; 
    d2_alpha        filling_alpha = not_initialized_alpha;


    /* filling_alpha = ... */
    e= R_RGA_IsPixelFormatHavingAlpha( self->FrameBuffer->pixel_format,  &has_frame_alpha );
    if(e)
    {
        goto fin;
    }
    if ( has_frame_alpha )
    {
        filling_alpha = 0x00;
    }
    else
    {
        filling_alpha = 0xFF;
    }


    /* d2_renderbox */
    e= gs_SetTextureOperation( d2_handle,  GRAPHICS_COPY,  GS_PIXEL_FORMAT_FOR_FILL_OPERATION,
        self->FrameBuffer->pixel_format,  0 );
    if(e)
    {
        goto fin;
    }
    e= d2_setfillmode( d2_handle,  d2_fm_color );
    if(e)
    {
        goto fin;
    }
    e= d2_setcolor( d2_handle,  0,  self->DRW_ClearColor );
    if(e)
    {
        goto fin;
    }
    e= d2_setalpha( d2_handle,  filling_alpha );
    if(e)
    {
        goto fin;
    }

    e= d2_renderbox( d2_handle,
        in_MinX  << fix4,  in_MinY   << fix4,
        in_Width << fix4,  in_Height << fix4 );
    if(e)
    {
        goto fin;
    }
    self->SHS_IsRequestedHardwareRendering = true;

    e=0;
fin:
    if ( filling_alpha != not_initialized_alpha )
    {
        ee= gs_SetTextureOperation( d2_handle,  self->GlobalCompositeOperation,  self->FrameBuffer->pixel_format,
            self->FrameBuffer->pixel_format,  0 );
        if(e==0)
        {
            e=ee;
        }
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_H_Clear */


/***********************************************************************
* Function Name: R_GRAPHICS_H_FillRect
************************************************************************/
errnum_t  R_GRAPHICS_H_FillRect( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t    e;
    d2_device*  d2_handle = self->DRW_D2;
    enum {      fix4  =  4 };  /* Count of bit in fixed point */ 

    int_translate_t  int_translate;

    /* Guard */
    e= R_GRAPHICS_H_GetMatrixIntegerTranslate( self,  &int_translate );
    if(e)
    {
        goto fin;
    }
    if ( ! int_translate.IsEnabled )
    {
        e= E_RGA_OTHER_FA;  /* Error: Matrix setting is not translation matrix. */
        goto fin;
    }

    /* Cast to an appropriate type */
    if ( self->FillPattern != NULL )
    {

        /* gs_FillRectPattern */
        e= gs_FillRectPattern( self,  in_MinX,  in_MinY,  in_Width,  in_Height );
        if(e)
        {
            goto fin;
        }
    }
    else
    {
        /* int_translate */
        in_MinX += int_translate.Tx;
        in_MinY += int_translate.Ty;

        /* d2_renderbox */
        e= d2_setfillmode( d2_handle,  d2_fm_color );
        if(e)
        {
            goto fin;
        }
        e= d2_setcolor( d2_handle,  0,  self->DRW_FillColor );
        if(e)
        {
            goto fin;
        }
        e= d2_setalpha( d2_handle,      (self->DRW_FillAlpha * self->GlobalAlpha) / 0xFF );
        if(e)
        {
            goto fin;
        }
        e= gs_SetTextureOperation( d2_handle,  self->GlobalCompositeOperation,  GS_PIXEL_FORMAT_FOR_FILL_OPERATION,
            self->FrameBuffer->pixel_format,  0 );
        if(e)
        {
            goto fin;
        }

        e= d2_renderbox( d2_handle,
            in_MinX  << fix4,  in_MinY   << fix4,
            in_Width << fix4,  in_Height << fix4 );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRequestedHardwareRendering = true;
    }
    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_GRAPHICS_H_FillRect */


/***********************************************************************
* Function Name: R_GRAPHICS_H_StrokeRect
************************************************************************/
errnum_t  R_GRAPHICS_H_StrokeRect 
    ( graphics_t*  self,  int_fast32_t  in_MinX,  int_fast32_t  in_MinY,
    int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t    e;
    errnum_t    ee;
    d2_device*  d2_handle = self->DRW_D2;
    enum {      fix4  =  4 };  /* Count of bit in fixed point */ 

    int_translate_t  int_translate;


    /* Guard */
    e= R_GRAPHICS_H_GetMatrixIntegerTranslate( self,  &int_translate );
    if(e)
    {
        goto fin;
    }
    if ( ! int_translate.IsEnabled )
    {
        e= E_RGA_OTHER_FB;  /* Error: Matrix setting is not translation matrix. */
        goto fin;
    }


    /* int_translate */
    in_MinX += int_translate.Tx;
    in_MinY += int_translate.Ty;


    /* Adjust coord */
    in_MinX += 1;
    in_MinY += 1;
    in_Width  -= 2;
    in_Height -= 2;


    /* d2_renderbox */
    e= d2_setfillmode( d2_handle,  d2_fm_color );
    if(e)
    {
        goto fin;
    }
    e= d2_setcolor( d2_handle,  0,  self->DRW_StrokeColor );
    if(e)
    {
        goto fin;
    }
    e= d2_setalpha( d2_handle,      (self->DRW_StrokeAlpha * self->GlobalAlpha) / 0xFF );
    if(e)
    {
        goto fin;
    }
    e= d2_selectrendermode( d2_handle,  d2_rm_outline );
    if(e)
    {
        goto fin;
    }
    e= d2_outlinewidth( d2_handle,  1 << fix4 );
    if(e)
    {
        goto fin;
    }

    e= d2_renderbox( d2_handle,
        in_MinX  << fix4,  in_MinY   << fix4,
        in_Width << fix4,  in_Height << fix4 );
    if(e)
    {
        goto fin;
    }
    self->SHS_IsRequestedHardwareRendering = true;

    e=0;
fin:
    {
        ee = d2_selectrendermode(d2_handle, d2_rm_solid);
        if(e==0)
        {
            e=ee;
        }
    }

    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_H_StrokeRect */


/***********************************************************************
* Function Name: gs_FillRectPattern
************************************************************************/
static errnum_t  gs_FillRectPattern
    ( graphics_t* self,
    int_fast32_t  in_MinX,   int_fast32_t  in_MinY,
    int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t          e;
    d2_device*        d2_handle = self->DRW_D2;
    pixel_format_t    pixel_format;
    graphics_image_t  image_in_buffer;
    bool_t            is_hardware_texture_wrap;
    d2_u32            image_pixel_format_d2;
    uintptr_t         image_physical_address;
    enum {            fix4  =  4 };  /* Count of bit in fixed point */ 
    enum {            fix16 = 16 };  /* Count of bit in fixed point */ 
    bool_t            is_int_matrix_pushed = false;
    int_translate_t   int_translate;

    const graphics_image_t*      image;
    graphics_image_properties_t  image_prop;


    /* image = ... */
    image = (const graphics_image_t*) self->FillPattern->image;
    e= R_GRAPHICS_IMAGE_GetImageFormat( image,  &pixel_format );
    if(e)
    {
        goto fin;
    }

    if ( (pixel_format == PIXEL_FORMAT_JPEG)  ||  (pixel_format == PIXEL_FORMAT_PNG) )
    {
        decode_image_parameters_t  parameters;

        /* R_RGA_DecodeWithJCU  or  R_RGA_DecodeWithPNG */
        parameters.ClippingRectangle = &self->ClippingRectangle;
        parameters.MinX = in_MinX;
        parameters.MinY = in_MinY;

        if ( pixel_format == PIXEL_FORMAT_JPEG )
        {
            e= R_RGA_DecodeWithJCU( self,  image,  &parameters,
                &image_in_buffer,  0 );
            if(e)
            {
                goto fin;
            }
        }
        else
        {
            /* pixel_format == PIXEL_FORMAT_PNG */
            e= R_RGA_DecodeWithPNG( self,  image,  &parameters,
                &image_in_buffer,  0 );
            if(e)
            {
                goto fin;
            }
        }

        /* Cast to an appropriate type */
        image = (const graphics_image_t*) &image_in_buffer;
    }


    /* image_* = ... */
    e= R_GRAPHICS_IMAGE_GetProperties( image,  &image_prop );
    if(e)
    {
        goto fin;
    }
    e= R_MEMORY_ToPhysicalAddress( image_prop.pixels,  &image_physical_address );
    if(e)
    {
        goto fin;
    }

    /* image_physical_address = . */
    e= gs_GetPixelFormatForDRW_Sub( image_prop.pixelFormat,  &image_pixel_format_d2 );
    if(e)
    {
        goto fin;
    }

    /* image_pixel_format_d2 = . */


    /* is_hardware_texture_wrap = ... */
    {
        /* Cast to an appropriate type */
        int_fast32_t  leading_bit_num_of_width  = 31 - R_CO_CountLeadingZeros( image_prop.width );

        /* Cast to an appropriate type */
        int_fast32_t  leading_bit_num_of_height = 31 - R_CO_CountLeadingZeros( image_prop.height );


        bool_t        is_width_2_to_the_n_th_power  = ( image_prop.width  == (1 << leading_bit_num_of_width) );
        bool_t        is_height_2_to_the_n_th_power = ( image_prop.height == (1 << leading_bit_num_of_height) );

        is_hardware_texture_wrap = is_width_2_to_the_n_th_power && is_height_2_to_the_n_th_power;
    }


    if ( is_hardware_texture_wrap )
    {

        /* d2_renderbox */
        e= d2_setfillmode( d2_handle,  d2_fm_texture );
        if(e)
        {
            goto fin;
        }
        e= d2_settexturemode( d2_handle,  d2_tm_wrapu | d2_tm_wrapv );
        if(e)
        {
            goto fin;
        }
        e= d2_setalpha( d2_handle,  self->GlobalAlpha );
        if(e)
        {
            goto fin;
        }
        e= gs_SetTextureOperation( d2_handle,  self->GlobalCompositeOperation,  image_prop.pixelFormat,
            self->FrameBuffer->pixel_format,  ( self->DRW_FillAlpha << 24 ) | ( self->DRW_FillColor ) );
        if(e)
        {
            goto fin;
        }

        /* Cast to an appropriate type */
        e= d2_settexture( d2_handle,  (void*) image_physical_address,
            in_Width,  in_Width,  in_Height,  image_pixel_format_d2 );
        if(e)
        {
            goto fin;
        }
        e= d2_settexturemapping( d2_handle,
            in_MinX << fix4,  in_MinY << fix4,
            0 << fix4,   0 << fix4,
            1 << fix16,  0 << fix16,
            0 << fix16,  1 << fix16 );
        if(e)
        {
            goto fin;
        }

        e= d2_renderbox( d2_handle,
            ( in_MinX      ) << fix4,  ( in_MinY       ) << fix4,
            ( in_Width - 1 ) << fix4,  ( in_Height - 1 ) << fix4 );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRequestedHardwareRendering = true;
    }
    else
    {
        int_fast32_t  source_min_x;
        int_fast32_t  source_min_y;
        int_fast32_t  source_min_x_first;
        int_fast32_t  destination_min_x;
        int_fast32_t  destination_min_y;
        int_fast32_t  destination_min_x_first;
        int_fast32_t  width_2;
        int_fast32_t  height_2;
        int_fast32_t  left_width;
        int_fast32_t  top_height;
        int_fast32_t  drawn_width;
        int_fast32_t  drawn_height;
        int_fast32_t  image_width;

        R_ERROR_Set( E_RGA_OBLIVIOUS_4 );   /* Error: This setting is slow, */

                                            /*because image width and height is not 2 to the power */

        e= R_GRAPHICS_H_GetMatrixIntegerTranslate( self,  &int_translate );
        if(e)
        {
            goto fin;
        }
        is_int_matrix_pushed = true;
        self->IntMatrix_i_2[0] = 0;
        self->IntMatrix_i_2[1] = 0;

        /* Cast to an appropriate type */
        source_min_x = minus_mod_int_equal_plus( in_MinX,  (int_fast32_t) image->width );

        /* Cast to an appropriate type */
        source_min_y = minus_mod_int_equal_plus( in_MinY,  (int_fast32_t) image->height );
        source_min_x_first = source_min_x;
        destination_min_x = in_MinX + int_translate.Tx;
        destination_min_y = in_MinY + int_translate.Ty;
        destination_min_x_first = destination_min_x;

        if ( (source_min_x + in_Width) < image->width )
        {
            left_width = in_Width;
        }
        else
        {
            left_width = image->width - source_min_x;
        }

        if ( (source_min_y + in_Height) < image->height )
            { top_height = in_Height; }
        else
            { top_height = image->height - source_min_y; }

        image_width = image->width;
        if ( image->flags & F_T_IMAGE_INF_PADDING_AT_RIGHT )
        {
            int_fast32_t  image_aligned_width;
            ASSERT_R 
                ( image == &image_in_buffer,  e=E_RGA_OTHER_82; goto fin );  /* Error: <Private/> */

            /* Cast to an appropriate type */
            image_aligned_width = (uint16_t) image_in_buffer.color;
            image_in_buffer.width = image_aligned_width;
        }

        height_2 = top_height;
        for ( drawn_height = 0;  drawn_height < in_Height;  )
        {
            if ( (drawn_height + height_2) > in_Height )
            {
                height_2 = in_Height - drawn_height;
            }

            source_min_x = source_min_x_first;
            destination_min_x = destination_min_x_first;
            width_2 = left_width;
            for ( drawn_width = 0;  drawn_width < in_Width;  )
            {
                if ( (drawn_width + width_2) > in_Width )
                {
                    width_2 = in_Width - drawn_width;
                }

                e= R_GRAPHICS_H_DrawImageChild( self, image,
                    source_min_x,  source_min_y,  width_2,  height_2,
                    destination_min_x,  destination_min_y,  width_2,  height_2 );
                if(e)
                {
                    goto fin;
                }

                drawn_width += width_2;
                source_min_x = 0;
                destination_min_x += width_2;
                width_2 = image_width;
            }
            drawn_height += height_2;
            source_min_y = 0;
            destination_min_y += height_2;
            height_2 = image->height;
        }
    }

    e=0;
fin:
    if ( is_int_matrix_pushed )
    {
        self->IntMatrix_i_2[0] = int_translate.Tx;
        self->IntMatrix_i_2[1] = int_translate.Ty;
    }
    return  e;
}/* End of function gs_FillRectPattern */


/***********************************************************************
* Function Name: R_GRAPHICS_H_Clip
************************************************************************/
errnum_t  R_GRAPHICS_H_Clip( graphics_t* self ) 
{
    errnum_t    e;
    d2_device*  d2_handle = self->DRW_D2;


    /* d2_cliprect */
    if ( self->PathDataLength == 0 )
    {
        void*   address;
        d2_s32  pitch;
        d2_s32  frame_width;
        d2_s32  frame_height;
        d2_s32  format;

        e= d2_getframebuffer( d2_handle,  &address,  &pitch,  &frame_width,  &frame_height,  &format );
        if(e)
        {
            goto fin;
        }

        e= d2_cliprect( d2_handle,  0,  0,  frame_width - 1,  frame_height - 1 );
        if(e)
        {
            goto fin;
        }
    }
    else
    {
        e= d2_cliprect( d2_handle,
            self->PathDataRectangle.MinX,
            self->PathDataRectangle.MinY,
            (self->PathDataRectangle.MinX + self->PathDataRectangle.Width)  - 1,
            (self->PathDataRectangle.MinY + self->PathDataRectangle.Height) - 1 );
        if(e)
        {
            goto fin;
        }
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_GRAPHICS_H_Clip */


/***********************************************************************
* Function Name: R_GRAPHICS_H_SetQualityFlags
************************************************************************/
errnum_t  R_GRAPHICS_H_SetQualityFlags( graphics_t*  self, 
    graphics_quality_flags_t  in_Qualities )
{
    errnum_t  e = 0;

    if ( in_Qualities & GRAPHICS_IMAGE_QUALITY_ANTIALIASED )
    {

        self->DRW_TextureMode |= (d2_tm_filteru | d2_tm_filterv);
        if ( in_Qualities & GRAPHICS_RENDERING_QUALITY_NONANTIALIASED )
        {
            e = E_RGA_OBLIVIOUS_5;

            /* Error: RGA's "*_IMAGE_*_ANTIALIASED" and "*_RENDERING_*_NONANTIALIASED*/
            /* can not be set at the same time. */
        }
    }

    if ( in_Qualities & GRAPHICS_IMAGE_QUALITY_NONANTIALIASED )
    {

        self->DRW_TextureMode &= (~( d2_tm_filteru | d2_tm_filterv ));
        if ( in_Qualities & GRAPHICS_RENDERING_QUALITY_ANTIALIASED )
        {
            e = E_RGA_OBLIVIOUS_2;

            /* Error: RGA's "*_IMAGE_*_NONANTIALIASED" and "*_RENDERING_*_ANTIALIASED"*/
            /* can not be set at the same time. */
        }
    }

    if ( IS_ALL_BITS_NOT_SET( in_Qualities,
        GRAPHICS_IMAGE_QUALITY_ANTIALIASED |
        GRAPHICS_IMAGE_QUALITY_NONANTIALIASED ) )
    {
        if ( IS_ANY_BITS_SET( in_Qualities,
                GRAPHICS_RENDERING_QUALITY_NONANTIALIASED |
                GRAPHICS_RENDERING_QUALITY_ANTIALIASED ) )
        {
            e = E_RGA_OBLIVIOUS_7;

            /* Error: RGA's "*_RENDERING_*" was ignored. Use "*_IMAGE_*" */
        }
    }

    if ( in_Qualities & GRAPHICS_IMAGE_DHITHER_ON )
    {
        e = E_RGA_OBLIVIOUS_6;

        /* Error: "GRAPHICS_IMAGE_DHITHER_ON" was ignored. */
    }
    if ( in_Qualities & GRAPHICS_IMAGE_DHITHER_OFF )
    {
        /* DO NOTHING */
        ;
    }

    R_ERROR_Set( e );
    if ( ( e & E_OBLIVIOUS_MASK ) == E_OBLIVIOUS )
    {
        e = 0;
    }
    return  e;
}/* End of function R_GRAPHICS_H_SetQualityFlags */


/***********************************************************************
* Function Name: R_GRAPHICS_H_GetQualityFlags
************************************************************************/
errnum_t  R_GRAPHICS_H_GetQualityFlags( graphics_t*  self, 
    graphics_quality_flags_t*  out_Qualities )
{
    graphics_quality_flags_t  qualities = 0;

    if ( IS_ANY_BITS_SET( self->DRW_TextureMode,  d2_tm_filteru | d2_tm_filterv ) )
    {
        qualities |= (GRAPHICS_IMAGE_QUALITY_ANTIALIASED | GRAPHICS_RENDERING_QUALITY_ANTIALIASED);
    }
    else
    {
        qualities |= (GRAPHICS_IMAGE_QUALITY_NONANTIALIASED | GRAPHICS_RENDERING_QUALITY_NONANTIALIASED);
    }
    qualities |= GRAPHICS_IMAGE_DHITHER_OFF;

    *out_Qualities = qualities;
    return  0;
}/* End of function R_GRAPHICS_H_GetQualityFlags */


/***********************************************************************
* Function Name: gs_SetTextureOperation
************************************************************************/
static errnum_t  gs_SetTextureOperation( d2_device*  d2_handle, 
    graphics_composite_operation_t  in_Operation,  pixel_format_t  in_ImagePixelFormat,
    pixel_format_t  in_FramePixelFormat,  d2_u32  in_ColorOfA8 )
{
    errnum_t  e;
    bool_t    has_image_alpha;


    /* has_image_alpha = . */
    e= R_RGA_IsPixelFormatHavingAlpha( in_ImagePixelFormat,  &has_image_alpha );
    if(e)
    {
        goto fin;
    }


    /* d2_settextureoperation,  d2_setalphablendmode */
    if ( in_Operation == GRAPHICS_SOURCE_OVER )
    {
        bool_t  is_image_format_RGB = ( in_ImagePixelFormat != PIXEL_FORMAT_A8 );

        if ( is_image_format_RGB )
        {
            if ( has_image_alpha )
            {
                e= d2_settextureoperation( d2_handle,
                    d2_to_blend,  d2_to_blend,  d2_to_blend,  d2_to_blend );
                if(e)
                {
                    goto fin;
                }
                e= d2_setblendmode(      d2_handle,  d2_bm_alpha,  d2_bm_one_minus_alpha );
                if(e)
                {
                    goto fin;
                }
                e= d2_setalphablendmodeex( d2_handle,  d2_bm_one,  d2_bm_one_minus_alpha,  d2_blendf_blenddst );
                if(e)
                {
                    goto fin;
                }
            }
            else
            {
                e= d2_settextureoperation( d2_handle,
                    d2_to_one,  d2_to_blend,  d2_to_blend,  d2_to_blend );
                if(e)
                {
                    goto fin;
                }

                /* If amode argument is "d2_to_blend", blue value is used as alpha channel in RGB565 mode. */
                e= d2_setblendmode(      d2_handle,  d2_bm_alpha,  d2_bm_one_minus_alpha );
                if(e)
                {
                    goto fin;
                }
                e= d2_setalphablendmodeex( d2_handle,  d2_bm_one,  d2_bm_one_minus_alpha,  d2_blendf_blenddst );
                if(e)
                {
                    goto fin;
                }
            }
        }
        else
        {
            /* in_ImagePixelFormat == PIXEL_FORMAT_A8 */
            bool_t  has_frame_alpha;
            e= R_RGA_IsPixelFormatHavingAlpha( in_FramePixelFormat,  &has_frame_alpha );  /* has_frame_alpha = . */
            if(e)
            {
                goto fin;
            }

            if ( has_frame_alpha )
            {
                e= d2_settextureoperation( d2_handle,
                    d2_to_blend,  d2_to_blend,  d2_to_blend,  d2_to_blend );
                if(e)
                {
                    goto fin;
                }
                e= d2_setblendmode(        d2_handle,  d2_bm_alpha,  d2_bm_one_minus_alpha );
                if(e)
                {
                    goto fin;
                }
                e= d2_setalphablendmodeex( d2_handle,  d2_bm_one,    d2_bm_one_minus_alpha,  d2_blendf_blenddst );
                if(e)
                {
                    goto fin;
                }
            }
            else
            {
                e= d2_settextureoperation( d2_handle,
                    d2_to_blend,  d2_to_blend,  d2_to_blend,  d2_to_blend );
                if(e)
                {
                    goto fin;
                }
                e= d2_setblendmode(        d2_handle,  d2_bm_alpha,  d2_bm_one_minus_alpha );
                if(e)
                {
                    goto fin;
                }
                e= d2_setalphablendmodeex( d2_handle,  d2_bm_zero,   d2_bm_one,  d2_blendf_blenddst );
                if(e)
                {
                    goto fin;
                }
            }
        }
    }
    else if ( in_Operation == GRAPHICS_COPY )
    {
        if ( has_image_alpha )
        {
            e= d2_settextureoperation( d2_handle,
                d2_to_copy,  d2_to_copy,  d2_to_copy,  d2_to_copy );
            if(e)
            {
                goto fin;
            }
        }
        else
        {
            e= d2_settextureoperation( d2_handle,
                d2_to_one,  d2_to_copy,  d2_to_copy,  d2_to_copy );
            if(e)
            {
                goto fin;
            }
                /* If amode argument is "d2_to_blend", blue value is used as alpha channel in RGB565 mode. */
        }
        e= d2_setblendmode(      d2_handle,  d2_bm_one,  d2_bm_zero );
        if(e)
        {
            goto fin;
        }
        e= d2_setalphablendmode( d2_handle,  d2_bm_one,  d2_bm_zero );
        if(e)
        {
            goto fin;
        }
    }
    else if ( in_Operation == GRAPHICS_DESTINATION_OUT )
    {
        bool_t  has_frame_alpha;
        e= R_RGA_IsPixelFormatHavingAlpha( in_FramePixelFormat,  &has_frame_alpha );  /* has_frame_alpha = . */
        if(e)
        {
            goto fin;
        }

        if ( (! has_image_alpha)  ||  (! has_frame_alpha) )
        {
            e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_47;
            goto fin;
        }

        /* Error: "GRAPHICS_DESTINATION_OUT" must be set the format having alpha */

        e= d2_settextureoperation( d2_handle,
            d2_to_blend,  d2_to_copy,  d2_to_copy,  d2_to_copy );
        if(e)
        {
            goto fin;
        }
        e= d2_setblendmode(      d2_handle,  d2_bm_zero,  d2_bm_one );
        if(e)
        {
            goto fin;
        }
        e= d2_setalphablendmode( d2_handle,  d2_bm_zero,  d2_bm_one_minus_alpha );
        if(e)
        {
            goto fin;
        }
    }
    else
    {
        e = E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_40;
        goto fin;

        /* Error: Not supported value as graphics_composite_operation_t. */
    }

    if ( in_ImagePixelFormat == PIXEL_FORMAT_A8 )
    {
        d2_u32  alpha = ( in_ColorOfA8 & 0xFF000000u ) >> 24;
        d2_u32  red   = ( in_ColorOfA8 & 0x00FF0000u ) >> 16;
        d2_u32  green = ( in_ColorOfA8 & 0x0000FF00u ) >>  8;
        d2_u32  blue  = ( in_ColorOfA8 & 0x000000FFu ) >>  0;

        e= d2_settexopparam( d2_handle,  d2_cc_alpha,  0,  alpha );
        if(e)
        {
            goto fin;
        }
        e= d2_settexopparam( d2_handle,  d2_cc_red,    0,  red   );
        if(e)
        {
            goto fin;
        }
        e= d2_settexopparam( d2_handle,  d2_cc_green,  0,  green );
        if(e)
        {
            goto fin;
        }
        e= d2_settexopparam( d2_handle,  d2_cc_blue,   0,  blue  );
        if(e)
        {
            goto fin;
        }
    }
    else
    {
        e= d2_settexopparam( d2_handle,  d2_cc_alpha,  0,  0xFF );
        if(e)
        {
            goto fin;
        }
        e= d2_settexopparam( d2_handle,  d2_cc_red,    0,  0xFF );
        if(e)
        {
            goto fin;
        }
        e= d2_settexopparam( d2_handle,  d2_cc_green,  0,  0xFF );
        if(e)
        {
            goto fin;
        }
        e= d2_settexopparam( d2_handle,  d2_cc_blue,   0,  0xFF );
        if(e)
        {
            goto fin;
        }
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_SetTextureOperation */


/***********************************************************************
* Function Name: gs_GetPixelFormatForDRW_Sub
************************************************************************/
static errnum_t  gs_GetPixelFormatForDRW_Sub 
                    ( pixel_format_t  in_PixelFormat,  d2_u32*  out_PixelFormat )
{
    errnum_t  e;

    if ( in_PixelFormat == PIXEL_FORMAT_RGB565 )
    {
        *out_PixelFormat = d2_mode_rgb565;

    } else if ( in_PixelFormat == PIXEL_FORMAT_ARGB4444 )
    {
        *out_PixelFormat = d2_mode_argb4444;

    } else if ( in_PixelFormat == PIXEL_FORMAT_ARGB1555 )
    {
        *out_PixelFormat = d2_mode_argb1555;

    } else if ( in_PixelFormat == PIXEL_FORMAT_ARGB8888 )
    {
        *out_PixelFormat = d2_mode_argb8888;

    } else if ( in_PixelFormat == PIXEL_FORMAT_XRGB8888 )
    {
        *out_PixelFormat = d2_mode_rgb888;

    } else if ( in_PixelFormat == PIXEL_FORMAT_A8 )
    {
        *out_PixelFormat = d2_mode_alpha8;

    }
    else
    {
        e = E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_41;  /* Error: Not supported pixel format. */
        goto fin;
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_GetPixelFormatForDRW_Sub */


