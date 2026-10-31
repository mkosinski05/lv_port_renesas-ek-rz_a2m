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
* File Name   : rga_rz_a2m.c
* Description : RGA wrapper for RZ/A2M DRW and RGAS.
*               Main function is "R_GRAPHICS_Initialize".
* Version     : 1.12
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include  <stdio.h>
#include  <math.h>
#include  "use_config_drw.h"
#include  "clib_drivers.h"
#include  "frame_buffer.h"
#include  "r_drw.h"
#include  "r_drw_pl.h"
#include  "rz_co.h"
#include  "rga_rz_a2m_rgas.h"
#include  "rga_rz_a2m_drw.h"
#include  "rga_api.h"
#include  "rga_pl.h"
#include  "rga_errnum.h"
#include  "rga_inside.h"
#include  "rga_local.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Defines: Macros in this file scope. Global static defines (GS)
************************************************************************/
#define  GS_N  "\r\n"


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum { gs_DrawingImage = 0,  gs_Filling = 1 }  gs_drawing_type_t;

/***********************************************************************
* Enumeration: gs_function_index_t
************************************************************************/
typedef enum {
    GS_DRAW_IMAGE,
    GS_DRAW_IMAGE_RESIZED,
    GS_DRAW_IMAGE_CHILD
} gs_function_index_t;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static errnum_t  gs_ComputeWarpQuadToSquare(
    graphics_matrix_float_t  in_QuadX0,  graphics_matrix_float_t  in_QuadY0,  /* TopLeft */
    graphics_matrix_float_t  in_QuadX1,  graphics_matrix_float_t  in_QuadY1,  /* TopRight */
    graphics_matrix_float_t  in_QuadX2,  graphics_matrix_float_t  in_QuadY2,  /* BottomLeft */
    graphics_matrix_float_t  in_QuadX3,  graphics_matrix_float_t  in_QuadY3,  /* BottomRight */
    graphics_matrix_float_t  out_Matix[3][3] );
static errnum_t  gs_ComputeWarpSquareToQuad(
    graphics_matrix_float_t  in_QuadX0,  graphics_matrix_float_t  in_QuadY0,  /* TopLeft */
    graphics_matrix_float_t  in_QuadX1,  graphics_matrix_float_t  in_QuadY1,  /* TopRight */
    graphics_matrix_float_t  in_QuadX2,  graphics_matrix_float_t  in_QuadY2,  /* BottomLeft */
    graphics_matrix_float_t  in_QuadX3,  graphics_matrix_float_t  in_QuadY3,  /* BottomRight */
    graphics_matrix_float_t  out_Matix[3][3] );
static errnum_t  gs_IsSoftwareRenderable( graphics_t*  self,  int_fast32_t  in_ImageWidth,
    pixel_format_t  in_ImagePixelFormat,  pixel_format_t  in_FramePixelFormat,
    int_translate_t*  out_IntTranslate,  bool_t*  out_IsSoftwareRenderable,  gs_drawing_type_t  in_DrawingType );
static errnum_t  gs_DecodeImage(
    graphics_t*              self,
    const graphics_image_t*  in_Image,
    int_fast32_t             in_MinX,
    int_fast32_t             in_MinY,
    int_translate_t*         in_IntTranslate,
    gs_function_index_t      in_FunctionIndex,
    const graphics_image_t** out_Image,
    graphics_image_t*        out_ImageInBuffer );
static errnum_t  gs_BeginSoftwareRendering( graphics_t* self,
    int_fast32_t  in_DestinationCacheLevel,  int_fast32_t  in_SourceCacheLevel );
static graphics_t*  gs_self;


/***********************************************************************
* Function Name: R_GRAPHICS_InitConst
************************************************************************/
void  R_GRAPHICS_InitConst( graphics_t* self ) 
{
    /* Cast to an appropriate type */
    *(int_t*) self = 0;
}/* End of function R_GRAPHICS_InitConst */


/***********************************************************************
* Function Name: R_GRAPHICS_Initialize
************************************************************************/
errnum_t  R_GRAPHICS_Initialize  
            ( graphics_t*  self,  graphics_config_t*  in_out_Config )
{
    errnum_t           e;
    graphics_config_t  empty;

    gs_self = self;

    R_GRAPHICS_CONFIG_SetEmpty( &empty );

    /* Cast to an appropriate type */
    if (!( (*(int_t*) self) == 0 ))
    {
        e=E_RGA_STATE_3;
        goto fin;
    }

    /* Error: "graphics_t" variable is not initialized by 0 initializer. */

    /* in_out_Config = empty value */
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_FRAME_BUFFER ) )
    {
        in_out_Config->frame_buffer = empty.frame_buffer;
        in_out_Config->flags |= F_GRAPHICS_FRAME_BUFFER;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_HAS_FRAME_PIPELINE ) )
    {
        in_out_Config->has_frame_pipeline = empty.has_frame_pipeline;
        in_out_Config->flags |= F_GRAPHICS_HAS_FRAME_PIPELINE;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_IS_HYBRID ) )
    {
        in_out_Config->is_hybrid = empty.is_hybrid;
        in_out_Config->flags |= F_GRAPHICS_IS_HYBRID;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_JPEG_DECODER ) )
    {
        in_out_Config->jpeg_decoder = empty.jpeg_decoder;
        in_out_Config->flags |= F_GRAPHICS_JPEG_DECODER;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_WORK_BUFFER_B_ADDRESS ) )
    {
        in_out_Config->work_buffer_b_address = empty.work_buffer_b_address;
        in_out_Config->flags |= F_GRAPHICS_WORK_BUFFER_B_ADDRESS;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_WORK_BUFFER_B_SIZE ) )
    {
        in_out_Config->work_buffer_b_size = empty.work_buffer_b_size;
        in_out_Config->flags |= F_GRAPHICS_WORK_BUFFER_B_SIZE;
    }
    if ( ! ( in_out_Config->flags  &  F_GRAPHICS_WORK_SIZE_FOR_LIBPNG ) )
    {
        in_out_Config->work_size_for_libPNG = empty.work_size_for_libPNG;
        in_out_Config->flags |= F_GRAPHICS_WORK_SIZE_FOR_LIBPNG;
    }


    /* R_GRAPHICS_OnInitialize */
    e= R_GRAPHICS_OnInitialize( self,  in_out_Config,  &self->CalleeDefined );
    if(e)
    {
        goto fin;
    }


    /* in_out_Config = default value */
    if ( in_out_Config->frame_buffer == empty.frame_buffer )
    {
        e= E_RGA_OTHER_FC;  goto fin;

        /* Error: Not set "graphics_config_t::frame_buffer" */
    }
    if ( in_out_Config->has_frame_pipeline == empty.has_frame_pipeline )
    {
        in_out_Config->has_frame_pipeline = false;
    }
    if ( in_out_Config->has_frame_pipeline )
    {
        in_out_Config->is_hybrid = false;
    }
    if ( in_out_Config->is_hybrid == empty.is_hybrid )
    {
        in_out_Config->is_hybrid = true;
    }
    if ( in_out_Config->work_size_for_libPNG == empty.work_size_for_libPNG )
    {
        in_out_Config->work_size_for_libPNG = 0;
    }


    /* Initalize renderers */
    self->DRW_D2 = 0;
    e= R_GRAPHICS_H_Initialize( self,  in_out_Config );
    if(e)
    {
        goto fin;
    }

    if ( in_out_Config->is_hybrid != false )
    {
        self->RGAS_State = 0;
        e= R_GRAPHICS_S_Initialize( self,  in_out_Config );
            if ( ( e & E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_MASK ) == E_RGA_NOT_SUPPORTED_PIXEL_FORMAT )
                { R_ERROR_Set( E_RGA_STATE_CLEAR_3 ); e=0; }  /* Error: <Private/> */
            if(e)
            {
                goto fin;
            }
        #ifdef  USE_CONFIG_REDIRECT_DRW_API
            R_ERROR_Set( E_DRW_OBLIVIOUS_5 );
                /* Error: If "graphics_config_t::is_hybrid = true", logger output only a part of log. */
        #endif
    }
    else
    {
        self->RGAS_State = GRAPHICS_STATE_DISABLED;
    }
    self->WorkSizeForLibPNG = in_out_Config->work_size_for_libPNG;


    /* Set default settings */
    e= GraphicsClass_initializeSHS( self );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_ResetMatrix( self );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_SetGlobalAlpha( self,  0xFFu );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_SetGlobalCompositeOperation( self,  GRAPHICS_SOURCE_OVER );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_BeginPath( self );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_H_Clip( self );
    if(e)
    {
        goto fin;
    }


    /* R_GRAPHICS_OnInitialized */
    e= R_GRAPHICS_OnInitialized( self );
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
}/* End of function R_GRAPHICS_Initialize */


/***********************************************************************
* Function Name: R_GRAPHICS_Finalize
************************************************************************/
errnum_t  R_GRAPHICS_Finalize( graphics_t* self,  errnum_t e ) 
{
    e= R_GRAPHICS_H_Finalize( self,  e );

    if ( self->RGAS_State == GRAPHICS_STATE_INITIALIZED )
    {
        e= R_GRAPHICS_S_Finalize( self,  e );
    }

    e= R_GRAPHICS_OnFinalize( self,  self->CalleeDefined,  e );

    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_Finalize */


/***********************************************************************
* Function Name: R_GRAPHICS_SetFrameBuffer
************************************************************************/
errnum_t  R_GRAPHICS_SetFrameBuffer 
            ( graphics_t*  self,  frame_buffer_t*  in_FrameBuffer )
{
    errnum_t  e = 0;
    errnum_t  ee;

    ee= R_GRAPHICS_H_SetFrameBuffer( self,  in_FrameBuffer );
    if(e==0)
    {
        e=ee;
    }

    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  self->IsSupportedPixelFromatForRGAS )
    {
        ee= R_GRAPHICS_S_SetFrameBuffer( self,  in_FrameBuffer );
        if(e==0)
        {
            e=ee;
        }
    }

    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_SetFrameBuffer */


/***********************************************************************
* Function Name: R_GRAPHICS_GetFrameBuffer
************************************************************************/
errnum_t  R_GRAPHICS_GetFrameBuffer 
            ( graphics_t*  self,  frame_buffer_t**  out_FrameBuffer )
{
    return  R_GRAPHICS_H_GetFrameBuffer( self,  out_FrameBuffer );
}/* End of function R_GRAPHICS_GetFrameBuffer */


/***********************************************************************
* Function Name: R_GRAPHICS_Start
************************************************************************/
errnum_t  R_GRAPHICS_Start( graphics_t* self ) 
{
    errnum_t  e;

    if ( self->HasFramePipeline )
    {
        e=E_RGA_OTHER_F4;
        goto fin;
    }

    /* Error: "R_GRAPHICS_Start" can not be used, when "graphics_config_t::has_frame_pipeline == true". */

    if ( (self->SHS_RequestedSoftwareRenderingLevel >= 1)  ||  self->SHS_IsRequestedCachedRendering )
    {

        R_DRW_FlushCache( 0,  0xFFFFFFFFu );
        self->SHS_RequestedSoftwareRenderingLevel = 0;
        self->SHS_IsRequestedCachedRendering = false;
    }

    e= R_GRAPHICS_H_Start( self );
    if(e)
    {
        goto fin;
    }
    self->SHS_IsRequestedHardwareRendering = false;
    self->SHS_IsRunningHardwareRendering = true;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_GRAPHICS_Start */


/***********************************************************************
* Function Name: R_GRAPHICS_Finish
************************************************************************/
errnum_t  R_GRAPHICS_Finish( graphics_t* self ) 
{
    errnum_t  e = 0;
    errnum_t  ee;

    if ( (self->SHS_RequestedSoftwareRenderingLevel >= 1)  ||  self->SHS_IsRequestedCachedRendering )
    {
        R_DRW_FlushCache( 0,  0xFFFFFFFFu );
    }

    ee= R_GRAPHICS_H_Finish( self );
    if(e==0)
    {
        e=ee;
    }
    self->SHS_IsRequestedHardwareRendering = false;
    self->SHS_IsRunningHardwareRendering = false;

    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  self->IsSupportedPixelFromatForRGAS )
    {

        ee= R_GRAPHICS_S_Finish( self );
        if(e==0)
        {
            e=ee;
        }
        self->SHS_RequestedSoftwareRenderingLevel = 0;
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_Finish */


/***********************************************************************
* Function Name: R_GRAPHICS_FinishPreviousFrame
************************************************************************/
errnum_t  R_GRAPHICS_FinishPreviousFrame( graphics_t* self ) 
{
    errnum_t  e;

    if ( !( self->RGAS_State == GRAPHICS_STATE_DISABLED ))
    {
        e=E_RGA_OTHER_F1;
        goto fin;
    }

    /* Error: "R_GRAPHICS_FinishPreviousFrame" can not be used, when "graphics_config_t::is_hybrid == true". */

    e= R_GRAPHICS_H_FinishPreviousFrame( self );
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
}/* End of function R_GRAPHICS_FinishPreviousFrame */


/***********************************************************************
* Function Name: R_GRAPHICS_StartPreviousFrame
************************************************************************/
errnum_t  R_GRAPHICS_StartPreviousFrame( graphics_t* self ) 
{
    errnum_t  e;

    if ( !( self->RGAS_State == GRAPHICS_STATE_DISABLED ))
    {
        e=E_RGA_OTHER_F2;
    }

    /* Error: "R_GRAPHICS_StartPreviousFrame" can not be used, when "graphics_config_t::is_hybrid == true". */

    e= R_GRAPHICS_H_StartPreviousFrame( self );
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
}/* End of function R_GRAPHICS_StartPreviousFrame */


/***********************************************************************
* Function Name: R_GRAPHICS_GetHasFramePipeline
************************************************************************/
bool_t  R_GRAPHICS_GetHasFramePipeline( graphics_t*  self ) 
{
    return  R_GRAPHICS_H_GetHasFramePipeline( self );
}/* End of function R_GRAPHICS_GetHasFramePipeline */


/***********************************************************************
* Function Name: R_GRAPHICS_GetIsPreviousFrameEmpty
*
* Description:
*    If "graphics_config_t::is_frame_pipeline = true",
*    this returns true after calling "R_GRAPHICS_FinishPreviousFrame".
*
*    This API will be not supported.
************************************************************************/
bool_t 
    R_GRAPHICS_GetIsPreviousFrameEmpty  
    ( graphics_t*  self )
{
    return  R_GRAPHICS_H_GetIsPreviousFrameEmpty( self );
}/* End of function R_GRAPHICS_GetIsPreviousFrameEmpty */


/***********************************************************************
* Function Name: R_GRAPHICS_GetIsCurrentFrameEmpty
*
* Description:
*    This returns false after calling "R_GRAPHICS_StartPreviousFrame".
*
*    This API will be not supported.
************************************************************************/
bool_t  
    R_GRAPHICS_GetIsCurrentFrameEmpty( graphics_t*  self )
{
    return  R_GRAPHICS_H_GetIsCurrentFrameEmpty( self );
}/* End of function R_GRAPHICS_GetIsCurrentFrameEmpty */


/***********************************************************************
* Function Name: R_GRAPHICS_Save
************************************************************************/
errnum_t  R_GRAPHICS_Save( graphics_t* self,  graphics_status_t*  out_Status ) 
{
    return  R_GRAPHICS_H_Save( self,  out_Status );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_Save */


/***********************************************************************
* Function Name: R_GRAPHICS_Restore
************************************************************************/
errnum_t  R_GRAPHICS_Restore 
            ( graphics_t* self,  graphics_status_t*  in_Status,  errnum_t  e )
{
    return  R_GRAPHICS_H_Restore( self,  in_Status,  e );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_Restore */


/***********************************************************************
* Function Name: R_GRAPHICS_ResetMatrix
************************************************************************/
errnum_t  R_GRAPHICS_ResetMatrix( graphics_t*  self ) 
{
    return  R_GRAPHICS_H_ResetMatrix( self );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_ResetMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_SetMatrix_2x3
************************************************************************/
errnum_t  R_GRAPHICS_SetMatrix_2x3( graphics_t*  self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty )
{
    return  R_GRAPHICS_H_SetMatrix_2x3( self,
        in_Sx,  in_Ky,
        in_Kx,  in_Sy,
        in_Tx,  in_Ty );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_SetMatrix_2x3 */


/***********************************************************************
* Function Name: R_GRAPHICS_SetMatrix_3x3
************************************************************************/
errnum_t  R_GRAPHICS_SetMatrix_3x3 
            ( graphics_t*  self,  const graphics_matrix_float_t*  in_Matrix )
{
    return  R_GRAPHICS_H_SetMatrix_3x3( self,  in_Matrix );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_SetMatrix_3x3 */


/***********************************************************************
* Function Name: R_GRAPHICS_GetMatrix_3x3
************************************************************************/
errnum_t  R_GRAPHICS_GetMatrix_3x3 
            ( graphics_t*  self,  graphics_matrix_float_t*  out_Matrix )
{
    return  R_GRAPHICS_H_GetMatrix_3x3( self,  out_Matrix );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_GetMatrix_3x3 */


/***********************************************************************
* Function Name: R_GRAPHICS_TranslateMatrixI 
************************************************************************/
errnum_t  R_GRAPHICS_TranslateMatrixI( graphics_t* self, 
    int_fast32_t  in_Tx,  int_fast32_t  in_Ty )
{
    return  R_GRAPHICS_H_TranslateMatrixI( self,  in_Tx,  in_Ty );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_TranslateMatrixI */


/***********************************************************************
* Function Name: R_GRAPHICS_TranslateMatrix
************************************************************************/
errnum_t  R_GRAPHICS_TranslateMatrix( graphics_t* self, 
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty )
{
    return  R_GRAPHICS_H_TranslateMatrix( self,  in_Tx,  in_Ty );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_TranslateMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_ScaleMatrix
************************************************************************/
errnum_t  R_GRAPHICS_ScaleMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Sy )
{
    return  R_GRAPHICS_H_ScaleMatrix( self,  in_Sx,  in_Sy );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_ScaleMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_RotateMatrixDegree
************************************************************************/
errnum_t  R_GRAPHICS_RotateMatrixDegree 
            ( graphics_t*  self,  graphics_matrix_float_t  in_Degree )
{
    return  R_GRAPHICS_H_RotateMatrixDegree( self,  in_Degree );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_RotateMatrixDegree */


/***********************************************************************
* Function Name: R_GRAPHICS_ShearMatrix
************************************************************************/
errnum_t  R_GRAPHICS_ShearMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Shx,  graphics_matrix_float_t  in_Shy )
{
    return  R_GRAPHICS_H_ShearMatrix( self,  in_Shx,  in_Shy );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_ShearMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_TransformMatrix
************************************************************************/
errnum_t  R_GRAPHICS_TransformMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty )
{
    return  R_GRAPHICS_H_TransformMatrix( self,
        in_Sx,  in_Ky,
        in_Kx,  in_Sy,
        in_Tx,  in_Ty );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_TransformMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_MultiplyMatrix
************************************************************************/
errnum_t  R_GRAPHICS_MultiplyMatrix 
            ( graphics_t* self,  graphics_matrix_float_t*  in_Matrix_3x3 )
{
    return  R_GRAPHICS_H_MultiplyMatrix( self,  in_Matrix_3x3 );

    /* R_GRAPHICS_S_* is not supported. */
}/* End of function R_GRAPHICS_MultiplyMatrix */


/***********************************************************************
* Function Name: R_GRAPHICS_GetProjectiveMatrix
************************************************************************/
errnum_t  R_GRAPHICS_GetProjectiveMatrix( 
    graphics_matrix_float_t  in_SourceTopLeftX0,      graphics_matrix_float_t  in_SourceTopLeftY0,
    graphics_matrix_float_t  in_SourceTopRightX1,     graphics_matrix_float_t  in_SourceTopRightY1,
    graphics_matrix_float_t  in_SourceBottomLeftX2,   graphics_matrix_float_t  in_SourceBottomLeftY2,
    graphics_matrix_float_t  in_SourceBottomRightX3,  graphics_matrix_float_t  in_SourceBottomRightY3,
    graphics_matrix_float_t  in_DestinationX0,  graphics_matrix_float_t  in_DestinationY0,
    graphics_matrix_float_t  in_DestinationX1,  graphics_matrix_float_t  in_DestinationY1,
    graphics_matrix_float_t  in_DestinationX2,  graphics_matrix_float_t  in_DestinationY2,
    graphics_matrix_float_t  in_DestinationX3,  graphics_matrix_float_t  in_DestinationY3,
    graphics_matrix_float_t* out_Matrix )
{
    errnum_t  e;
    uint32_t  x;
    uint32_t  y;

    graphics_matrix_float_t     quad_to_square[3][3];
    graphics_matrix_float_t     square_to_quad[3][3];

    /* Cast to an appropriate type */
    graphics_matrix_float_t  (* out )[3] = ( graphics_matrix_float_t (*)[3] ) out_Matrix;

    /* quad_to_square = ... */
    e= gs_ComputeWarpQuadToSquare(
        in_SourceTopLeftX0,     in_SourceTopLeftY0,
        in_SourceTopRightX1,    in_SourceTopRightY1,
        in_SourceBottomLeftX2,  in_SourceBottomLeftY2,
        in_SourceBottomRightX3, in_SourceBottomRightY3,
        quad_to_square );
    if(e)
    {
        goto fin;
    }

    /* square_to_quad = ... */
    e= gs_ComputeWarpSquareToQuad(
        in_DestinationX0,  in_DestinationY0,
        in_DestinationX1,  in_DestinationY1,
        in_DestinationX2,  in_DestinationY2,
        in_DestinationX3,  in_DestinationY3,
        square_to_quad );
    if(e)
    {
        goto fin;
    }

    /* out_Matrix[x][y] = square_to_quad[y][x] * quad_to_square[y][x] */
    for ( y = 0;  y < 3;  y += 1 )
    {
        for ( x = 0;  x < 3;  x += 1 )
        {
            out[x][y] =
                ( square_to_quad[y][0] * quad_to_square[0][x] ) +
                ( square_to_quad[y][1] * quad_to_square[1][x] ) +
                ( square_to_quad[y][2] * quad_to_square[2][x] );
        }
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_GRAPHICS_GetProjectiveMatrix */


/***********************************************************************
* Function Name: gs_IsFloatZero
************************************************************************/
STATIC_INLINE bool_t  gs_IsFloatZero( graphics_matrix_float_t  in_FloatValue ) 
{
    return  fabsf( in_FloatValue ) <= 0.0f;
}/* End of function gs_IsFloatZero */


/***********************************************************************
* Function Name: gs_InvertMatrix
************************************************************************/
static errnum_t  gs_InvertMatrix( 
    graphics_matrix_float_t       out_Matrix[3][3],
    const graphics_matrix_float_t  in_Matrix[3][3])
{
    /* Cast to an appropriate type */
    graphics_matrix_float_t  (* m )[3] = ( graphics_matrix_float_t (*)[3] ) in_Matrix;
    errnum_t  e;

    graphics_matrix_float_t  det00 = (m[1][1] * m[2][2]) - (m[2][1] * m[1][2]);
    graphics_matrix_float_t  det01 = (m[2][0] * m[1][2]) - (m[1][0] * m[2][2]);
    graphics_matrix_float_t  det02 = (m[1][0] * m[2][1]) - (m[2][0] * m[1][1]);
    graphics_matrix_float_t  d     = (m[0][0] * det00) + (m[0][1] * det01) + (m[0][2] * det02);

    /* Guard */
    if ( gs_IsFloatZero( d ) )
    {
        e= E_RGA_NOT_REGULAR_MATRIX_2;
        goto fin;
    }

    /* Error: It is not regular matrix. */

    /* Invert */
    d = 1.0f / d;
    out_Matrix[0][0] = d * det00;
    out_Matrix[1][0] = d * det01;
    out_Matrix[2][0] = d * det02;
    out_Matrix[0][1] = d * ((m[2][1] * m[0][2]) - (m[0][1] * m[2][2]));
    out_Matrix[1][1] = d * ((m[0][0] * m[2][2]) - (m[2][0] * m[0][2]));
    out_Matrix[2][1] = d * ((m[2][0] * m[0][1]) - (m[0][0] * m[2][1]));
    out_Matrix[0][2] = d * ((m[0][1] * m[1][2]) - (m[1][1] * m[0][2]));
    out_Matrix[1][2] = d * ((m[1][0] * m[0][2]) - (m[0][0] * m[1][2]));
    out_Matrix[2][2] = d * ((m[0][0] * m[1][1]) - (m[1][0] * m[0][1]));

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_InvertMatrix */


/***********************************************************************
* Function Name: gs_ComputeWarpQuadToSquare
************************************************************************/
static errnum_t  gs_ComputeWarpQuadToSquare( 
    graphics_matrix_float_t  in_QuadX0,  graphics_matrix_float_t  in_QuadY0,  /* TopLeft */
    graphics_matrix_float_t  in_QuadX1,  graphics_matrix_float_t  in_QuadY1,  /* TopRight */
    graphics_matrix_float_t  in_QuadX2,  graphics_matrix_float_t  in_QuadY2,  /* BottomLeft */
    graphics_matrix_float_t  in_QuadX3,  graphics_matrix_float_t  in_QuadY3,  /* BottomRight */
    graphics_matrix_float_t  out_Matix[3][3] )
{
    errnum_t  e;
    float_t   square_to_quad[3][3];

    e= gs_ComputeWarpSquareToQuad(
        in_QuadX0,  in_QuadY0,
        in_QuadX1,  in_QuadY1,
        in_QuadX2,  in_QuadY2,
        in_QuadX3,  in_QuadY3,
        square_to_quad );
    if(e)
    {
        goto fin;
    }  /* square_to_quad = . */

    e= gs_InvertMatrix( out_Matix,  square_to_quad );
    if(e)
    {
        goto fin;
    }

    /* out_Matix = . */

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_ComputeWarpQuadToSquare */


/***********************************************************************
* Function Name: gs_ComputeWarpSquareToQuad
************************************************************************/
static errnum_t  gs_ComputeWarpSquareToQuad( 
    graphics_matrix_float_t  in_QuadX0,  graphics_matrix_float_t  in_QuadY0,  /* TopLeft */
    graphics_matrix_float_t  in_QuadX1,  graphics_matrix_float_t  in_QuadY1,  /* TopRight */
    graphics_matrix_float_t  in_QuadX2,  graphics_matrix_float_t  in_QuadY2,  /* BottomLeft */
    graphics_matrix_float_t  in_QuadX3,  graphics_matrix_float_t  in_QuadY3,  /* BottomRight */
    graphics_matrix_float_t  out_Matix[3][3] )
{
    errnum_t  e;

    /* (variables) = ... */
    graphics_matrix_float_t  dx_3to1 = in_QuadX1 - in_QuadX3;
    graphics_matrix_float_t  dy_3to1 = in_QuadY1 - in_QuadY3;
    graphics_matrix_float_t  dx_3to2 = in_QuadX2 - in_QuadX3;
    graphics_matrix_float_t  dy_3to2 = in_QuadY2 - in_QuadY3;
    graphics_matrix_float_t  det = ( dx_3to1 * dy_3to2 ) - ( dx_3to2 * dy_3to1 );
    graphics_matrix_float_t  sum_x = ((in_QuadX0 - in_QuadX1) + in_QuadX3) - in_QuadX2;
    graphics_matrix_float_t  sum_y = ((in_QuadY0 - in_QuadY1) + in_QuadY3) - in_QuadY2;

    /* Guard */
    if ( gs_IsFloatZero( det ) )
    {
        e= E_RGA_NOT_REGULAR_MATRIX_1;
        goto fin;
    }

    /* Error: It is not regular matrix. */


    /* Affine mapping */
    if ( gs_IsFloatZero( sum_x )  &&  gs_IsFloatZero( sum_y ) )
    {
        out_Matix[0][0] = in_QuadX1 - in_QuadX0;
        out_Matix[1][0] = in_QuadY1 - in_QuadY0;
        out_Matix[2][0] = 0.0f;
        out_Matix[0][1] = in_QuadX3 - in_QuadX1;
        out_Matix[1][1] = in_QuadY3 - in_QuadY1;
        out_Matix[2][1] = 0.0f;
        out_Matix[0][2] = in_QuadX0;
        out_Matix[1][2] = in_QuadY0;
        out_Matix[2][2] = 1.0f;
    }

    /* Not affine mapping */
    else
    {
        graphics_matrix_float_t  oodet = 1.0f / det;
        graphics_matrix_float_t  w0 = oodet * ( ( sum_x * dy_3to2 ) - ( dx_3to2 * sum_y ) );
        graphics_matrix_float_t  w1 = oodet * ( ( dx_3to1 * sum_y ) - ( sum_x * dy_3to1 ) );

        out_Matix[0][0] = (in_QuadX1 - in_QuadX0) + ( w0 * in_QuadX1 );
        out_Matix[1][0] = (in_QuadY1 - in_QuadY0) + ( w0 * in_QuadY1 );
        out_Matix[2][0] = w0;
        out_Matix[0][1] = (in_QuadX2 - in_QuadX0) + ( w1 * in_QuadX2 );
        out_Matix[1][1] = (in_QuadY2 - in_QuadY0) + ( w1 * in_QuadY2 );
        out_Matix[2][1] = w1;
        out_Matix[0][2] = in_QuadX0;
        out_Matix[1][2] = in_QuadY0;
        out_Matix[2][2] = 1.0f;
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_ComputeWarpSquareToQuad */


/***********************************************************************
* Function Name: gs_IsSoftwareRenderable
************************************************************************/
static errnum_t  gs_IsSoftwareRenderable 
    ( graphics_t*  self,  int_fast32_t  in_ImageWidth,
    pixel_format_t  in_ImagePixelFormat,  pixel_format_t  in_FramePixelFormat,
    int_translate_t*  out_IntTranslate,  bool_t*  out_IsSoftwareRenderable,  gs_drawing_type_t  in_DrawingType )
{
    errnum_t  e;
    bool_t    has_alpha;
    uint8_t   global_alpha;
    bool_t    is_blend;
    bool_t    is_color_convert;
    bool_t    is_software_rendering_format;
    bool_t    is_software_rendering_alignment_fitted;

    byte_per_pixel_t  byte_per_pixel_of_image;


    /* (variables) = ... */
    {
        /* out_IntTranslate = ... */
        e= R_GRAPHICS_H_GetMatrixIntegerTranslate( self,  out_IntTranslate );
        if(e)
        {
            goto fin;
        }

        /* has_alpha = ... */
        if ( in_DrawingType == gs_DrawingImage )
        {
            e= R_RGA_IsPixelFormatHavingAlpha( in_ImagePixelFormat,  &has_alpha );
            if(e)
            {
                goto fin;
            }
        }
        else
        {
            /* in_DrawingType == gs_Filling */
            has_alpha = ( self->DRW_FillAlpha < 0xFF );
        }

        /* global_alpha = ... */
        e= R_GRAPHICS_GetGlobalAlpha( self,  &global_alpha );
        if(e)
        {
            goto fin;
        }

        /* is_blend = ... */
        if ( self->GlobalCompositeOperation == GRAPHICS_COPY )
        {
            is_blend = false;
        }
        else if ( self->GlobalCompositeOperation == GRAPHICS_SOURCE_OVER )
        {
            is_blend = (
                ( has_alpha ) ||
                ( (global_alpha > 0x00)  &&  (global_alpha < 0xFF) )
            );
        }
        else
        {
            is_blend = true;
        }

        /* is_color_convert = ... */
        if ( in_DrawingType == gs_DrawingImage )
        {
            is_color_convert = ( in_ImagePixelFormat != in_FramePixelFormat );
        }
        else
        {
            /* in_DrawingType == gs_Filling */
            is_color_convert = false;
        }

        /* is_software_rendering_format = ... */
        is_software_rendering_format = ( in_ImagePixelFormat == PIXEL_FORMAT_R8G8B8A8 );

        /* is_software_rendering_alignment_fitted = ... */
        e= R_RGA_GetBytePerPixelFromPixelFormat( in_ImagePixelFormat,  &byte_per_pixel_of_image );
        if(e)
        {
            goto fin;
        }
            /* byte_per_pixel_of_image = . */
        is_software_rendering_alignment_fitted = (
            /* in_DrawingType == gs_DrawingImage */
                ( (byte_per_pixel_of_image == 2)  &&  ((in_ImageWidth % 2) == 0) ) ||
                ( byte_per_pixel_of_image == 4 ) ||
                ( byte_per_pixel_of_image == 1 ) ||
            ( in_DrawingType == gs_Filling )
        );
    }

    /* out_IsSoftwareRenderable = ... */
    if ( is_software_rendering_format )
    {
        *out_IsSoftwareRenderable = true;
    }
    else
    {

        *out_IsSoftwareRenderable = (
            ( ! is_color_convert ) &&
            ( ! is_blend ) &&
            ( out_IntTranslate->IsEnabled ) &&

            /* Cast to an appropriate type */
            ( self->FillPattern == NULL ) &&
            ( self->IsSupportedPixelFromatForRGAS ) &&
            ( is_software_rendering_alignment_fitted )
        );
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_IsSoftwareRenderable */


/***********************************************************************
* Function Name: R_GRAPHICS_DrawImage
************************************************************************/
errnum_t  R_GRAPHICS_DrawImage 
    ( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY )
{
    errnum_t          e;
    frame_buffer_t*   frame = self->FrameBuffer;
    pixel_format_t    image_pixel_format;
    int_translate_t   int_translate;
    bool_t            has_alpha;
    uint_fast8_t      global_alpha = self->GlobalAlpha;
    bool_t            is_decoded_in_work_buffer_B;
    bool_t            is_software_renderable;
    graphics_image_t  image_in_buffer;
    bool_t            is_drawn_completed;


    if ( self->RGAS_State != GRAPHICS_STATE_INITIALIZED )
    {
        e= R_GRAPHICS_H_DrawImage( self,  in_Image,  in_MinX,  in_MinY );
        if(e)
        {
            goto fin;
        }
        goto fin;
    }


    /* gs_DecodeImage */
    e= gs_DecodeImage( self,  in_Image,  in_MinX,  in_MinY,  &int_translate,
        GS_DRAW_IMAGE,  &in_Image,  &image_in_buffer );
    if(e)
    {
        goto fin;
    }

        /* int_translate = . */
        /* in_Image = . */
        /* *image_in_buffer = . */
    is_drawn_completed = ( in_Image->width == 0 );
    if ( is_drawn_completed )
    {
        e=0;
        goto fin;
    }  /* Drawn by JCU/libPNG decode only */


    /* (variables) = ... */
    {
        /* image_pixel_format = ... */
        GetPixelFormatEnumFrom_T_IMAGE_INF( &image_pixel_format,  in_Image->type );

        /* int_translate = ... */
        /* is_software_renderable = ... */
        e= gs_IsSoftwareRenderable( self,  in_Image->width,  image_pixel_format,  frame->pixel_format,
            &int_translate,  &is_software_renderable,  gs_DrawingImage );
    }


    /* R_GRAPHICS_S_DrawImage or R_GRAPHICS_H_DrawImage */
    if ( is_software_renderable )
    {
        if ( global_alpha != 0 )
        {
            in_MinX += int_translate.Tx;
            in_MinY += int_translate.Ty;

            e= R_GRAPHICS_S_DrawImage( self,  in_Image,  in_MinX,  in_MinY );
            if(e)
            {
                goto fin;
            }
                /* Because, software copy is faster than hardware copy. */
        }
    }
    else
    {
        /* Guard */
        if ( image_pixel_format != frame->pixel_format )
        {
            static bool_t  is_done = false;

            if ( ! is_done )
            {
                R_ERROR_Set( E_RGA_OBLIVIOUS_1 );

                /* Error: Slow because pixel format is not same as image and frame buffer. */
                /* Also notify in JPEG and other formats. */
                is_done = true;
            }
        }

        /* R_GRAPHICS_H_DrawImage */
        e= R_GRAPHICS_H_DrawImage( self,  in_Image,  in_MinX,  in_MinY );
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
}/* End of function R_GRAPHICS_DrawImage */


/***********************************************************************
* Function Name: R_GRAPHICS_DrawImageResized
************************************************************************/
errnum_t  R_GRAPHICS_DrawImageResized 
    ( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t          e;
    frame_buffer_t*   frame = self->FrameBuffer;
    bool_t            is_hardware = true;
    int_translate_t   int_translate;
    uint_fast8_t      global_alpha = self->GlobalAlpha;
    graphics_image_t  image_in_buffer;
    bool_t            is_drawn_completed;


    if ( self->RGAS_State != GRAPHICS_STATE_INITIALIZED )
    {
        e= R_GRAPHICS_H_DrawImageResized( self,  in_Image,
            in_MinX,  in_MinY,  in_Width,  in_Height );
        if(e)
        {
            goto fin;
        }
        goto fin;
    }


    /* gs_DecodeImage */
    e= gs_DecodeImage( self,  in_Image,  in_MinX,  in_MinY,  &int_translate,
        GS_DRAW_IMAGE_RESIZED,  &in_Image,  &image_in_buffer );
    if(e)
    {
        goto fin;
    }
    is_drawn_completed = ( in_Image->width == 0 );
    if ( is_drawn_completed )
    {
        e=0;
        goto fin;
    }  /* Drawn by JCU/libPNG decode only */

    /* is_hardware = ... */
    {
        graphics_image_properties_t  image_prop;

        /* image_prop = ... */
        e= R_GRAPHICS_IMAGE_GetProperties( in_Image,  &image_prop );
        if(e)
        {
            goto fin;
        }

        if ( (in_Width == image_prop.width)  &&  (in_Height == image_prop.width) )
        {
            bool_t  is_software_renderable;

            /* is_software_renderable = ... */
            /* int_translate = ... */
            e= gs_IsSoftwareRenderable( self,  image_prop.width,  image_prop.pixelFormat,  frame->pixel_format,
                &int_translate,  &is_software_renderable,  gs_DrawingImage );

            /* is_hardware = ... */
            is_hardware = ! is_software_renderable;
        }
    }


    /* R_GRAPHICS_S_DrawImage or R_GRAPHICS_H_DrawImageResized */
    if ( ! is_hardware )
    {
        if ( global_alpha != 0 )
        {
            in_MinX += int_translate.Tx;
            in_MinY += int_translate.Ty;

            e= R_GRAPHICS_S_DrawImage( self,  in_Image,  in_MinX,  in_MinY );
            if(e)
            {
                goto fin;
            }

            /* Because, software copy is faster than hardware copy. */
        }
    }
    else
    {
        e= R_GRAPHICS_H_DrawImageResized( self,  in_Image,
            in_MinX,  in_MinY,  in_Width,  in_Height );
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
}/* End of function R_GRAPHICS_DrawImageResized */


/***********************************************************************
* Function Name: R_GRAPHICS_DrawImageChild
************************************************************************/
errnum_t  R_GRAPHICS_DrawImageChild 
    ( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight )
{
    errnum_t          e;
    frame_buffer_t*   frame = self->FrameBuffer;
    bool_t            is_hardware = true;
    int_translate_t   int_translate;
    uint_fast8_t      global_alpha = self->GlobalAlpha;
    graphics_image_t  image_in_buffer;
    bool_t            is_drawn_completed;


    /* R_GRAPHICS_H_DrawImageChild, if ... */
    if ( self->RGAS_State != GRAPHICS_STATE_INITIALIZED )
    {
        e= R_GRAPHICS_H_DrawImageChild( self,  in_Image,
            in_SourceMinX,  in_SourceMinY,  in_SourceWidth,  in_SourceHeight,
            in_DestinationMinX,  in_DestinationMinY,  in_DestinationWidth,  in_DestinationHeight );
            if(e)
            {
                goto fin;
            }
        goto fin;
    }


    /* gs_DecodeImage */
    e= gs_DecodeImage( self,  in_Image,  in_DestinationMinX,  in_DestinationMinY,  &int_translate,
        GS_DRAW_IMAGE_CHILD,  &in_Image,  &image_in_buffer );
    if(e)
    {
        goto fin;
    }
    is_drawn_completed = ( in_Image->width == 0 );
    if ( is_drawn_completed )
    {
        e=0;
        goto fin;
    }  /* Drawn by JCU/libPNG decode only */


    /* is_hardware = ... */
    if ( (in_SourceWidth == in_DestinationWidth)  &&  (in_SourceHeight == in_DestinationHeight) )
    {
        graphics_image_properties_t  image_prop;

        /* image_prop = ... */
        e= R_GRAPHICS_IMAGE_GetProperties( in_Image,  &image_prop );
        if(e)
        {
            goto fin;
        }

/* TODO: not necessary ?  */
/* if ( in_SourceWidth == image_prop.width  &&  in_SourceHeight == image_prop.height ) { */
        {
            bool_t  is_software_renderable;

            /* is_software_renderable = ... */
            /* int_translate = ... */
            e= gs_IsSoftwareRenderable( self,  image_prop.width,  image_prop.pixelFormat,  frame->pixel_format,
                &int_translate,  &is_software_renderable,  gs_DrawingImage );
            if(e)
            {
                goto fin;
            }

            /* is_hardware = ... */
            is_hardware = ! is_software_renderable;
        }
    }


    /* R_GRAPHICS_S_DrawImage or R_GRAPHICS_H_DrawImageChild */
    if ( ! is_hardware )
    {
        if ( global_alpha != 0 )
        {
            in_DestinationMinX += int_translate.Tx;
            in_DestinationMinY += int_translate.Ty;

            e= R_GRAPHICS_S_DrawImageChild( self,  in_Image,
                in_SourceMinX,        in_SourceMinY,
                in_SourceWidth,       in_SourceHeight,
                in_DestinationMinX,   in_DestinationMinY,
                in_DestinationWidth,  in_DestinationHeight );
            if(e)
            {
                goto fin;
            }
                /* Because, software copy is faster than hardware copy. */
        }
    }
    else
    {
        e= R_GRAPHICS_H_DrawImageChild( self,  in_Image,
            in_SourceMinX,  in_SourceMinY,  in_SourceWidth,  in_SourceHeight,
            in_DestinationMinX,  in_DestinationMinY,  in_DestinationWidth,  in_DestinationHeight );
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
}/* End of function R_GRAPHICS_DrawImageChild */


/***********************************************************************
* Function Name: gs_DecodeImage
************************************************************************/
static errnum_t  gs_DecodeImage( 
    graphics_t*              self,
    const graphics_image_t*  in_Image,
    int_fast32_t             in_MinX,
    int_fast32_t             in_MinY,
    int_translate_t*         out_IntTranslate,
    gs_function_index_t      in_FunctionIndex,
    const graphics_image_t** out_Image,
    graphics_image_t*        out_ImageInBuffer )
{
    errnum_t         e;
    pixel_format_t   image_format;
    frame_buffer_t*  frame = self->FrameBuffer;

    /* Cast to an appropriate type */
    const graphics_image_t*  return_image = NULL;


    /* (variables) = ... */
    {
        /* image_format = ... */
        e= R_GRAPHICS_IMAGE_GetImageFormat( in_Image,  &image_format );
        if(e)
        {
            goto fin;
        }

        /* *out_IntTranslate = ... */
        e= R_GRAPHICS_H_GetMatrixIntegerTranslate( self,  out_IntTranslate );
        if(e)
        {
            goto fin;
        }
    }


    /* R_RGA_DecodeWithJCU  or  R_RGA_DecodeWithPNG */
    if ( (image_format == PIXEL_FORMAT_JPEG)  ||  (image_format == PIXEL_FORMAT_PNG) )
    {
        decode_image_parameters_t  parameters;
        graphics_JCU_options_t     options_of_JCU = 0;
        graphics_PNG_options_t     options_of_PNG = 0;
        bool_t                     is_resizing_API;
        bool_t                     is_drawn_completed;

        if ( image_format == PIXEL_FORMAT_JPEG )
        {
            if ( self->JPEG_Decoder == GRAPHICS_JPEG_DECODER_NONE )
            {
                e=E_RGA_OTHER_59;
                goto fin;
            }

            /* Error: "graphics_config_t::jpeg_decoder" is "GRAPHICS_JPEG_DECODER_NONE". */
        }

        parameters.ClippingRectangle = &self->ClippingRectangle;


        /* parameters.MinX, MinY = ... */
        if ( in_FunctionIndex == GS_DRAW_IMAGE )
        {
            if ( out_IntTranslate->IsEnabled )
            {
                parameters.MinX = in_MinX + out_IntTranslate->Tx;
                parameters.MinY = in_MinY + out_IntTranslate->Ty;
                options_of_JCU |= F_GRAPHICS_JCU_FOR_INT_TRANSLATE;
                options_of_PNG |= F_GRAPHICS_PNG_FOR_INT_TRANSLATE;
            }
            else
            {
                parameters.MinX = in_MinX;
                parameters.MinY = in_MinY;
            }
        }
        else {
            parameters.MinX = 0;
            parameters.MinY = 0;
        }


        /* F_GRAPHICS_JCU_ENABLE_JCU_ONLY */
        is_resizing_API = ( (in_FunctionIndex == GS_DRAW_IMAGE_RESIZED)  ||
                (in_FunctionIndex == GS_DRAW_IMAGE_CHILD) );

        if ( ! is_resizing_API )
        {
            if ( out_IntTranslate->IsEnabled  &&  (self->GlobalAlpha == 255)  &&
                    (self->GlobalCompositeOperation == GRAPHICS_SOURCE_OVER) )
            {

                options_of_JCU |= F_GRAPHICS_JCU_ENABLE_JCU_ONLY;
                options_of_PNG |= F_GRAPHICS_PNG_ENABLE_PNG_ONLY;
            }
        }


        /* F_GRAPHICS_PNG_FORCE_PNG_ONLY */
        if ( self->GlobalCompositeOperation == GRAPHICS_COPY )
        {
            options_of_PNG |= F_GRAPHICS_PNG_FORCE_PNG_ONLY;
        }


        /* R_RGA_DecodeWithJCU  or  R_RGA_DecodeWithPNG */
        if ( image_format == PIXEL_FORMAT_JPEG )
        {

            e= R_RGA_DecodeWithJCU( self,  in_Image,  &parameters,
                out_ImageInBuffer,  options_of_JCU );
            if(e)
            {
                goto fin;
            }
        }
        else
        {
            /* if ( image_format == PIXEL_FORMAT_PNG ) */

            e= R_RGA_DecodeWithPNG( self,  in_Image,  &parameters,
                out_ImageInBuffer,  options_of_JCU );
            if(e)
            {
                goto fin;
            }
        }
        is_drawn_completed = ( out_ImageInBuffer->width == 0 );
        if ( ! is_drawn_completed )
        {

            /* R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage */
            if ( ! out_IntTranslate->IsEnabled )
            {
                e= R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage( self,  out_ImageInBuffer );
                if(e)
                {
                    goto fin;
                }
                R_ERROR_Set( E_RGA_OBLIVIOUS_3 );  /* Error: <Private/> */

                /* TODO: Cut "R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage" ? */
            }
        }

        /* Cast to an appropriate type */
        return_image = (const graphics_image_t*) out_ImageInBuffer;
    }
    else
    { /* If raw format image */
        return_image = in_Image;
    }

    e=0;
fin:
    {
        *out_Image = return_image;
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function gs_DecodeImage */


/***********************************************************************
* Function Name: R_GRAPHICS_SetBackgroundColor
************************************************************************/
errnum_t  R_GRAPHICS_SetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t  in_Color )
{
    errnum_t  e = 0;
    errnum_t  ee;

    ee= R_GRAPHICS_H_SetBackgroundColor( self,  in_Color );
    if(e==0)
    {
        e=ee;
    }
    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  self->IsSupportedPixelFromatForRGAS )
    {
        ee= R_GRAPHICS_S_SetBackgroundColor( self,  in_Color );
        if(e==0)
        {
            e=ee;
        }
    }
    return  e;
}/* End of function R_GRAPHICS_SetBackgroundColor */


/***********************************************************************
* Function Name: R_GRAPHICS_GetBackgroundColor
************************************************************************/
errnum_t  R_GRAPHICS_GetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t*  out_Color )
{
    errnum_t  e = 0;
    errnum_t  ee;

    ee= R_GRAPHICS_H_GetBackgroundColor( self,  out_Color );
    if(e==0)
    {
        e=ee;
    }
    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  self->IsSupportedPixelFromatForRGAS )
    {
        ee= R_GRAPHICS_S_GetBackgroundColor( self,  out_Color );
        if(e==0)
        {
            e=ee;
        }
    }
    return  e;
}/* End of function R_GRAPHICS_GetBackgroundColor */


/***********************************************************************
* Function Name: R_GRAPHICS_GetClearColor
************************************************************************/
errnum_t  R_GRAPHICS_GetClearColor( graphics_t* self,  r8g8b8a8_t*  out_Color ) 
{
    errnum_t  e = 0;
    errnum_t  ee;

    ee=  R_GRAPHICS_H_GetClearColor( self,  out_Color );
    if(e==0)
    {
        e=ee;
    }
    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  self->IsSupportedPixelFromatForRGAS )
    {
        ee=  R_GRAPHICS_S_GetClearColor( self,  out_Color );
        if(e==0)
        {
            e=ee;
        }
    }
    return  e;
}/* End of function R_GRAPHICS_GetClearColor */


/***********************************************************************
* Function Name: R_GRAPHICS_SetFillColor
************************************************************************/
errnum_t  R_GRAPHICS_SetFillColor( graphics_t* self,  r8g8b8a8_t  in_Color ) 
{
    errnum_t  e = 0;
    errnum_t  ee;

    ee=  R_GRAPHICS_H_SetFillColor( self,  in_Color );
    if(e==0)
    {
        e=ee;
    }
    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  self->IsSupportedPixelFromatForRGAS )
    {

        ee= R_GRAPHICS_S_SetFillColor( self,  in_Color );
        if(e==0)
        {
            e=ee;
        }
    }

    /* Cast to an appropriate type */
    self->FillPattern = NULL;
    return  e;
}/* End of function R_GRAPHICS_SetFillColor */


/***********************************************************************
* Function Name: R_GRAPHICS_SetStrokeColor 
************************************************************************/
errnum_t  R_GRAPHICS_SetStrokeColor( graphics_t*  self,  r8g8b8a8_t  in_Color ) 
{
    return  R_GRAPHICS_H_SetStrokeColor( self,  in_Color );
}/* End of function R_GRAPHICS_SetStrokeColor */


/***********************************************************************
* Function Name: R_GRAPHICS_SetFillPattern 
************************************************************************/
errnum_t  R_GRAPHICS_SetFillPattern 
            ( graphics_t* self,  graphics_pattern_t*  in_Pattern )
{
    self->FillPattern = in_Pattern;
    return  0;
}/* End of function R_GRAPHICS_SetFillPattern */


/***********************************************************************
* Function Name: R_GRAPHICS_Clear
************************************************************************/
errnum_t  R_GRAPHICS_Clear( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t  e;

    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  self->IsSupportedPixelFromatForRGAS )
    {
        e= R_GRAPHICS_BeginCachedRendering( self );
        if(e)
        {
            goto fin;
        }

        e= R_GRAPHICS_S_Clear( self,  in_MinX,  in_MinY,  in_Width,  in_Height );
        if(e)
        {
            goto fin;
        }
    }
    else
    {

        e= R_GRAPHICS_H_Clear( self,  in_MinX,  in_MinY,  in_Width,  in_Height );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRequestedHardwareRendering = true;
    }

    e=0;
fin:
    {
        e = R_GRAPHICS_EndRenderingInFin(self, e);
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_Clear */


/***********************************************************************
* Function Name: R_GRAPHICS_FillRect
************************************************************************/
errnum_t  R_GRAPHICS_FillRect( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    errnum_t         e;
    int_translate_t  int_translate;
    bool_t           is_software_renderable;
    pixel_format_t   frame_pixel_foramt = self->FrameBuffer->pixel_format;
    pixel_format_t   image_pixel_foramt = PIXEL_FORMAT_ARGB8888;


    e= gs_IsSoftwareRenderable( self,  in_Width,  image_pixel_foramt,  frame_pixel_foramt,  &int_translate,
        &is_software_renderable,  gs_Filling );  /* is_software_renderable = .,  int_translate = . */
    if(e)
    {
        goto fin;
    }


    /* R_GRAPHICS_S_FillRect  or  R_GRAPHICS_H_FillRect */
    if ( (self->RGAS_State == GRAPHICS_STATE_INITIALIZED)  &&  is_software_renderable )
    {
        e= R_GRAPHICS_BeginCachedRendering( self );
        if(e)
        {
            goto fin;
        }

        e= R_GRAPHICS_S_FillRect( self,  in_MinX,  in_MinY,  in_Width,  in_Height );
        if(e)
        {
            goto fin;
        }
    } else
    {

        e= R_GRAPHICS_H_FillRect( self,  in_MinX,  in_MinY,  in_Width,  in_Height );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRequestedHardwareRendering = true;
    }

    e=0;
fin:
    {
        e = R_GRAPHICS_EndRenderingInFin(self, e);
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_FillRect */


/***********************************************************************
* Function Name: R_GRAPHICS_StrokeRect
************************************************************************/
errnum_t  R_GRAPHICS_StrokeRect 
    ( graphics_t*  self,  int_fast32_t  in_MinX,  int_fast32_t  in_MinY,
    int_fast32_t  in_Width,  int_fast32_t  in_Height )
{
    return  R_GRAPHICS_H_StrokeRect( self,  in_MinX,  in_MinY,  in_Width,  in_Height );
}/* End of function R_GRAPHICS_StrokeRect */


/***********************************************************************
* Function Name: R_GRAPHICS_Clip
************************************************************************/
errnum_t  R_GRAPHICS_Clip( graphics_t*  self ) 
{
    errnum_t  e;

    if ( !( self->PathDataLength <= 1 ))
    {
        e=E_RGA_FEW_ARRAY_1;
        goto fin;
    }
    /* Error: Too long path data.*/

    if ( self->PathDataLength == 0 )
    {
        e= IntBoxType_setEmpty( &self->ClippingRectangle );
        if(e)
        {
            goto fin;
        }
    }
    else
    {
        e= IntBoxType_and( &self->ClippingRectangle,
            &self->ClippingRectangle,  &self->PathDataRectangle );
        if(e)
        {
            goto fin;
        }
    }

    e= R_GRAPHICS_H_Clip( self );
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
}/* End of function R_GRAPHICS_Clip */


/***********************************************************************
* Function Name: R_GRAPHICS_SetQualityFlags
************************************************************************/
errnum_t  R_GRAPHICS_SetQualityFlags( graphics_t*  self, 
    graphics_quality_flags_t  in_Qualities )
{
    return  R_GRAPHICS_H_SetQualityFlags( self,  in_Qualities );
}/* End of function R_GRAPHICS_SetQualityFlags */


/***********************************************************************
* Function Name: R_GRAPHICS_GetQualityFlags
************************************************************************/
errnum_t  R_GRAPHICS_GetQualityFlags( graphics_t*  self, 
    graphics_quality_flags_t*  out_Qualities )
{
    return  R_GRAPHICS_H_GetQualityFlags( self,  out_Qualities );
}/* End of function R_GRAPHICS_GetQualityFlags */


/***********************************************************************
* Function Name: R_GRAPHICS_BeginSoftwareRendering
************************************************************************/
errnum_t  R_GRAPHICS_BeginSoftwareRendering( graphics_t*  self ) 
{
    return  gs_BeginSoftwareRendering( self,  1,  0 );
}/* End of function R_GRAPHICS_BeginSoftwareRendering */

/***********************************************************************
* Function Name: R_GRAPHICS_BeginSoftwareRendering2 
************************************************************************/
errnum_t  R_GRAPHICS_BeginSoftwareRendering2( graphics_t* self ) 
{
    return  gs_BeginSoftwareRendering( self,  1,  0 );

    /* 1 = because the coherency of L2 cache does not occureed. */
}/* End of function R_GRAPHICS_BeginSoftwareRendering2 */

/***********************************************************************
* Function Name: R_GRAPHICS_BeginSoftwareRenderingA
************************************************************************/
errnum_t  R_GRAPHICS_BeginSoftwareRenderingA 
            ( graphics_t*  self,  void*  in_Address )
{
    return  gs_BeginSoftwareRendering( self,  1,  0 );

    /* 1 = because the coherency of L2 cache does not occureed. */
}/* End of function R_GRAPHICS_BeginSoftwareRenderingA */

/***********************************************************************
* Function Name: gs_BeginSoftwareRendering
************************************************************************/
static errnum_t  gs_BeginSoftwareRendering( graphics_t*  self, 
    int_fast32_t  in_DestinationCacheLevel,  int_fast32_t  in_SourceCacheLevel )
{
    errnum_t  e;

    #if ! defined( RGA_INSIDE_RELEASE )
        if ( !( self->SHS_Union.Requesting.SoftwareRenderingLevel == 0 )) { R_ERROR_Set(__LINE__); }
    #endif

    /* Operations the case that user reads the memory written by DRW */
    self->SHS_Union.Requesting.SoftwareRenderingLevel = in_DestinationCacheLevel;
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


    /* Operations the case that user reads the memory written by RGAS */
    if ( self->SHS_IsRequestedCachedRendering )
    {

        e= R_GRAPHICS_S_Finish( self );
        if(e)
        {
            goto fin;
        }
        self->SHS_IsRequestedCachedRendering = false;
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function gs_BeginSoftwareRendering */


/***********************************************************************
* Function Name: R_GRAPHICS_EndSoftwareRendering
************************************************************************/
errnum_t  R_GRAPHICS_EndSoftwareRendering( graphics_t*  self ) 
{
    #if ! defined( RGA_INSIDE_RELEASE )
        if ( !( self->SHS_Union.Requesting.SoftwareRenderingLevel >= 0 )) { R_ERROR_Set(__LINE__); }
    #endif

    if ( self->SHS_RequestedSoftwareRenderingLevel < self->SHS_Union.Requesting.SoftwareRenderingLevel )
    {
        self->SHS_RequestedSoftwareRenderingLevel = self->SHS_Union.Requesting.SoftwareRenderingLevel;
    }
    self->SHS_Union.Requesting.SoftwareRenderingLevel = 0;
    return  0;
}/* End of function R_GRAPHICS_EndSoftwareRendering */


/***********************************************************************
* Function Name: R_GRAPHICS_EndRenderingInFin 
************************************************************************/
errnum_t  R_GRAPHICS_EndRenderingInFin( graphics_t*  self,  errnum_t  e ) 
{
    if ( self->SHS_Union.RequestingFlags != 0 )
    {
        errnum_t  ee;

        if ( self->SHS_Union.Requesting.SoftwareRenderingLevel >= 1 )
        {
            ee= R_GRAPHICS_EndSoftwareRendering( self );
            if(e==0)
            {
                e=ee;
            }
        }
        if ( self->SHS_Union.Requesting.IsCachedRendering )
        {
            ee= R_GRAPHICS_EndCachedRendering( self );
            if(e==0)
            {
                e=ee;
            }
        }
    }
    R_ERROR_Set( e );
    return  e;
}/* End of function R_GRAPHICS_EndRenderingInFin */


/***********************************************************************
* Function Name: R_GRAPHICS_CONFIG_SetEmpty
*
* Description:
*    Search "_SetEmpty" in Developer Intranet.
************************************************************************/
void  R_GRAPHICS_CONFIG_SetEmpty( graphics_config_t*  out ) 
{
    /* Cast to an appropriate type */
    bool_t  empty_as_bool = (bool_t) 2;
    enum {  empty_as_enum = 99 };
    size_t  empty_as_size = 0xFFFFFFFFu;

    /* Cast to an appropriate type */
    void*   empty_as_address = (void*) 0xFFFFFFFFu;

    out->flags = 0xFFFFFFFFu;

    out->frame_buffer       = empty_as_address;
    out->has_frame_pipeline = empty_as_bool;
    out->is_hybrid          = empty_as_bool;
    out->jpeg_decoder       = empty_as_enum;

    out->work_buffer_b_address = empty_as_address;
    out->work_buffer_b_size    = empty_as_size;
    out->work_size_for_libPNG  = empty_as_size;
}/* End of function R_GRAPHICS_CONFIG_SetEmpty */


/***********************************************************************
* Function Name: R_GRAPHICS_PrintRegisters
************************************************************************/
void  R_GRAPHICS_PrintRegisters() 
{
    graphics_t*  self = gs_self;
    errnum_t     e = 0,  ee;

    /* Cast to an appropriate type */
    if ( gs_self == NULL )
    {
        printf( "Not called R_GRAPHICS_Initialize"GS_N );
    }
    else
    {
        uintptr_t  frame_buffer_physical_address;
        uintptr_t  physical_address_of_cached;

        ee= R_GRAPHICS_BeginSoftwareRendering( self );
        if(e==0)
        {
            e=ee;
        }
        ee= R_GRAPHICS_EndSoftwareRendering( self );
        if(e==0)
        {
            e=ee;
        }

        printf( "Address of frame buffer - User     : 0x%08X"GS_N,  self->CurrentFrameBufferUserAddress );

        ee= R_DRW_ToPhysicalAddress( self->CurrentFrameBufferUserAddress,
            &frame_buffer_physical_address );
        if(e==0)
        {
            e=ee;
        }

        ee= R_DRW_ToPhysicalAddress( self->CurrentFrameBufferCachedAddress,
            &physical_address_of_cached );
        if(e==0)
        {
            e=ee;
        }

        printf( "Address of frame buffer - Physical : 0x%08X"GS_N,  frame_buffer_physical_address );
        printf( "Address of frame buffer - Cached   : 0x%08X"GS_N,  self->CurrentFrameBufferCachedAddress );
        printf( "Physical address of cached address : 0x%08X"GS_N,  physical_address_of_cached );

        /* Cast to an appropriate type */
        if ( self->FrameBuffer != NULL )
        {
            frame_buffer_t*  frame = self->FrameBuffer;

            printf( "FrameBuffer->buffer_address[ show_buffer_index ] = 0x%08X"GS_N,
                frame->buffer_address[ frame->show_buffer_index ] );
            printf( "FrameBuffer->buffer_address[ draw_buffer_index ] = 0x%08X"GS_N,
                frame->buffer_address[ frame->draw_buffer_index ] );
            printf( "Size of frame buffer : 0x%08X"GS_N,  self->FrameBuffer->stride * self->FrameBuffer->height );
        }
    }

fin:
    {
        R_ERROR_Set(e);
    }
    return;
}/* End of function R_GRAPHICS_PrintRegisters */


