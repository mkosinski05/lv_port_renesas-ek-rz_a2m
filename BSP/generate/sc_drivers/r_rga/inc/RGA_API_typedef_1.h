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
* File Name   : rga_api_typedef_1.h
* Description : RGA middleware and driver.
*               Main class is "graphics_t".
* Version     : 1.12
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <math.h>
#include  <stdint.h>
#include  "use_config_rga.h"

#ifndef R_RGA_API_TYPEDEF_1_H
#define R_RGA_API_TYPEDEF_1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

#ifdef  USE_CONFIG_RGA_INSIDE
/***********************************************************************
* Macro: GS_ANIMATION_CURVE
*
*    GS_ANIMATION_CURVE_IS_LINEAR - 1
*    GS_ANIMATION_CURVE_IS_BEZIER - 2
************************************************************************/
#define  GS_ANIMATION_CURVE            GS_ANIMATION_CURVE_IS_BEZIER 
#define  GS_ANIMATION_CURVE_IS_LINEAR  ( 1 )
#define  GS_ANIMATION_CURVE_IS_BEZIER  ( 2 )
#endif


/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Type: animation_timing_function_t
************************************************************************/
typedef struct {
#ifdef  USE_CONFIG_RGA_INSIDE
#if  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_LINEAR

    /* Variable: ClampValues */
    float_t*  ClampValues;

    /* Variable: ClampValuesLength */
    uint32_t    ClampValuesLength;

#elif  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_BEZIER

    /* Variable: ControlPoint1_X */
    /* Variable: ControlPoint1_Y */
    float_t   ControlPoint1_X; 
    float_t   ControlPoint1_Y; 

    /* Variable: ControlPoint2_X */
    /* Variable: ControlPoint2_Y */
    float_t   ControlPoint2_X; 
    float_t   ControlPoint2_Y; 

    /* Variable: EpsilonX */
    float_t   EpsilonX; 
#endif
#else
    uint8_t  __Members[0x18];   // TODO: 
#endif

} animation_timing_function_t; 


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* In "rga_api.h" */

/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* R_RGA_API_TYPEDEF_1_H */
