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
/***********************************************************************
* File Name   : rga_rz_a2m_drw.h
* Description : RGA wrapper for DRW.
* Version     : 1.12
************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "rz_co_typedef.h"
#include  "rga_api_typedef.h"
#include  "rga_raw_image_typedef.h"
#include  "rga_rz_a2m_drw_typedef.h"

#ifndef RGA_RZ_A2M_DRW_H
#define RGA_RZ_A2M_DRW_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
/* In "rga_rz_a2m_drw_typedef.h" */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* In "rga_rz_a2m_drw_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* In "rga_rz_a2m_drw_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/
/******************************************************************************
 * Function Name: R_GRAPHICS_H_Initialize
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Initialize( graphics_t*  self,  graphics_config_t*  in_Config );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_Finalize
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Finalize( graphics_t* self,  errnum_t e );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_SetFrameBuffer
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_SetFrameBuffer( graphics_t*  self,  frame_buffer_t*  in_FrameBuffer );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetFrameBuffer
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_GetFrameBuffer( graphics_t*  self,  frame_buffer_t**  out_FrameBuffer );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_Start
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Start( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_Finish
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Finish( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_FinishPreviousFrame
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_FinishPreviousFrame( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_StartPreviousFrame
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_StartPreviousFrame( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetHasFramePipeline
 *****************************************************************************/
bool_t    R_GRAPHICS_H_GetHasFramePipeline( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetIsPreviousFrameEmpty
 *****************************************************************************/
bool_t    R_GRAPHICS_H_GetIsPreviousFrameEmpty( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetIsCurrentFrameEmpty
 *****************************************************************************/
bool_t    R_GRAPHICS_H_GetIsCurrentFrameEmpty( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_Save
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Save( graphics_t* self, graphics_status_t* out_Status );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_Restore
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Restore( graphics_t* self,  graphics_status_t*  in_Status,  errnum_t  e );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_ResetMatrix
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_ResetMatrix( graphics_t*  self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_SetMatrix_2x3
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_SetMatrix_2x3( graphics_t*  self,
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_SetMatrix_3x3
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_SetMatrix_3x3( graphics_t*  self,  const graphics_matrix_float_t*  in_Matrix );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetMatrix_3x3
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_GetMatrix_3x3( graphics_t*  self,  graphics_matrix_float_t*  out_Matrix );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_TranslateMatrixI
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_TranslateMatrixI( graphics_t* self,
    int_fast32_t  in_Tx,  int_fast32_t  in_Ty );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_TranslateMatrix
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_TranslateMatrix( graphics_t* self,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_ScaleMatrix
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_ScaleMatrix( graphics_t* self,
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Sy );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_RotateMatrixDegree
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_RotateMatrixDegree( graphics_t* self,  graphics_matrix_float_t  in_Degree );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_ShearMatrix
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_ShearMatrix( graphics_t*  self,
    graphics_matrix_float_t  in_Shx,  graphics_matrix_float_t  in_Shy );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_TransformMatrix
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_TransformMatrix( graphics_t*  self,
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_MultiplyMatrix
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_MultiplyMatrix( graphics_t* self,  graphics_matrix_float_t*  in_Matrix_3x3 );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetMatrixIntegerTranslate
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_GetMatrixIntegerTranslate( graphics_t*  self,  int_translate_t*  out );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_DrawImage
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_DrawImage( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_DrawImageResized
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_DrawImageResized( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_DrawImageChild
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_DrawImageChild( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_DrawAlphaZeroAtOutOfImage( graphics_t*  self,  graphics_image_t*  in_ImageInBuffer );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_SetBackgroundColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_SetBackgroundColor( graphics_t*  self,  r8g8b8a8_t  in_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetBackgroundColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_GetBackgroundColor( graphics_t*  self,  r8g8b8a8_t*  out_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetClearColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_GetClearColor( graphics_t* self,  r8g8b8a8_t* out_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_SetFillColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_SetFillColor( graphics_t* self,  r8g8b8a8_t  in_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_SetStrokeColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_SetStrokeColor( graphics_t* self,  r8g8b8a8_t  in_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_Clear
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Clear( graphics_t*  self,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_FillRect
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_FillRect( graphics_t*  self,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_StrokeRect
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_StrokeRect( graphics_t*  self,  int_fast32_t  in_MinX,  int_fast32_t  in_MinY,
    int_fast32_t  in_Width,  int_fast32_t  in_Height );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_Clip
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_Clip( graphics_t* self );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_SetQualityFlags
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_SetQualityFlags( graphics_t*  self,  graphics_quality_flags_t  in_Qualities );

/******************************************************************************
 * Function Name: R_GRAPHICS_H_GetQualityFlags
 *****************************************************************************/
errnum_t  R_GRAPHICS_H_GetQualityFlags( graphics_t*  self,  graphics_quality_flags_t*  out_Qualities );


/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* RGA_RZ_A2M_DRW_H */
