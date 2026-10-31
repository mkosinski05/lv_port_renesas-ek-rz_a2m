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
* File Name   : rga_rz_a2m_rgas.h
* Description : RGA wrapper for RGAS.
* Version     : 1.12
************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "rz_co_typedef.h"
#include  "rga_api_typedef.h"
#include  "rga_raw_image_typedef.h"

#ifndef RGA_RZ_A2M_RGAS_H
#define RGA_RZ_A2M_RGAS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
/* In "RGA_RZ_A2M_RGAS_typedef.h" */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* In "RGA_RZ_A2M_RGAS_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* In "RGA_RZ_A2M_RGAS_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/
/******************************************************************************
 * Function Name: R_GRAPHICS_S_Initialize
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_Initialize( graphics_t*  self,  graphics_config_t*  in_Config );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_Finalize
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_Finalize( graphics_t* self,  errnum_t e );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_SetFrameBuffer
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_SetFrameBuffer( graphics_t*  self,  frame_buffer_t*  in_FrameBuffer );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_DrawImage
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_DrawImage( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_DrawImageChild
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_DrawImageChild( graphics_t* self,  const graphics_image_t* image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_SetBackgroundColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_SetBackgroundColor( graphics_t*  self,  r8g8b8a8_t  in_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_GetBackgroundColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_GetBackgroundColor( graphics_t*  self,  r8g8b8a8_t*  out_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_GetClearColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_GetClearColor( graphics_t* self,  r8g8b8a8_t* out_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_SetFillColor
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_SetFillColor( graphics_t* self,  r8g8b8a8_t  in_Color );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_Clear
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_Clear( graphics_t*  self,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_FillRect
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_FillRect( graphics_t*  self,
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );

/******************************************************************************
 * Function Name: R_GRAPHICS_S_Finish
 *****************************************************************************/
errnum_t  R_GRAPHICS_S_Finish( graphics_t*  self );


/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* RGA_RZ_A2M_RGAS_H */
