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
 * File Name    : rga_local.h
 * Version      : 1.12
 *****************************************************************************/

#include  "rz_co_typedef.h"
#include  "rga_api_typedef.h"

#ifndef  RGA_LOCAL_H
#define  RGA_LOCAL_H

/***********************************************************************
  <<< RGA local >>> 
************************************************************************/
/******************************************************************************
 * Function Name: GetPixelFormatEnumFrom_T_IMAGE_INF
 *****************************************************************************/
errnum_t  GetPixelFormatEnumFrom_T_IMAGE_INF( pixel_format_t* out, uint8_t in ); 


/******************************************************************************
 * Function Name: R_RGA_IsPixelFormatHavingAlpha
 *****************************************************************************/
errnum_t  R_RGA_IsPixelFormatHavingAlpha
    ( pixel_format_t  in_PixelFormat,  bool_t*  out_IsAlpha );


/***********************************************************************
  <<< [RGA_SHS] Synchronize hardware and software rendering >>> 
************************************************************************/

/******************************************************************************
 * Function Name: R_GRAPHICS_BeginCachedRendering
 *****************************************************************************/
errnum_t  R_GRAPHICS_BeginCachedRendering( graphics_t* self ); 


/******************************************************************************
 * Function Name: R_GRAPHICS_EndCachedRendering
 *****************************************************************************/
errnum_t  R_GRAPHICS_EndCachedRendering( graphics_t* self );

/******************************************************************************
 * Function Name: GraphicsClass_initializeSHS
 *****************************************************************************/
STATIC_INLINE  errnum_t  GraphicsClass_initializeSHS( graphics_t* self );

/* INLINE  errnum_t  R_GRAPHICS_BeginSoftwareRendering( graphics_t* self ); */
/* INLINE  errnum_t  R_GRAPHICS_BeginSoftwareRendering2( graphics_t* self ); */
/* INLINE  errnum_t  R_GRAPHICS_BeginSoftwareRenderingA( graphics_t* self, void* Address ); */
/* INLINE  errnum_t  R_GRAPHICS_EndSoftwareRendering( graphics_t* self ); */
/* INLINE  errnum_t  R_GRAPHICS_EndRenderingInFin( graphics_t* self, errnum_t e ); */
/* Flush is in R_GRAPHICS_Finish() */
/******************************************************************************
 * Function Name: R_GRAPHICS_FlushSoftRender_Sub
 *****************************************************************************/
void  R_GRAPHICS_FlushSoftRender_Sub( graphics_t* self, bool_t IsForceFlush );

#ifndef R_OSPL_NDEBUG
/******************************************************************************
 * Function Name: GraphicsClass_debugSHS
 *****************************************************************************/
void  GraphicsClass_debugSHS( const char* Label );
#endif


/***********************************************************************
* Function Name: GraphicsClass_initializeSHS
************************************************************************/
STATIC_INLINE  errnum_t  GraphicsClass_initializeSHS( graphics_t* self ) 
{
    self->SHS_Union.RequestingFlags = 0;
    self->SHS_RequestedSoftwareRenderingLevel = 0;
    self->SHS_IsRequestedCachedRendering = false;
    self->SHS_IsRequestedHardwareRendering = false;
    self->SHS_IsRunningHardwareRendering = false;
    return  0;
}/*End of function GraphicsClass_initializeSHS */

#endif  /* RGA_LOCAL_H */
