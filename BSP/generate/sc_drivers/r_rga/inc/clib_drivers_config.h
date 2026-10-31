/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2013 - 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/***********************************************************************
* File: clib_drivers_config.h
*    Common code for drivers and more.
*
* - $ModuleRevision: {Masters}\CLibCommon\OS_less_RGA_RZA2_Test\33 $
* - $Date: 2018-11-05T18:07:32+09:00 $
************************************************************************/

#ifndef  CLIB_DRIVERS_CONFIG_H
#define  CLIB_DRIVERS_CONFIG_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Macro: IS_SUPPORT_SAME_TYPEDEF
*    Whether compiler suppots duplicated same typedef. 0 or 1.
************************************************************************/
#define  IS_SUPPORT_SAME_TYPEDEF  ( 1 )


/***********************************************************************
* Macro: LIB_STATIC
*    Private function of library
*
* Description:
*    Public library should be complied by set "LIB_STATIC" static and
*    pack source files to one file.
************************************************************************/
#ifndef LIB_STATIC
#define LIB_STATIC
#endif


/***********************************************************************
* Macro: static_release
*    Hide if release version.
************************************************************************/
#ifndef R_OSPL_NDEBUG
	#define  static_release  static  /* @suppress("Macro naming") @suppress("Macro expansion") */ 
#else
	#define  static_release          /* @suppress("Macro naming") @suppress("Macro expansion") */ 
#endif


/***********************************************************************
* Macros: BYTE_ENDIAN
*    BYTE_ENDIAN
*
*    : BYTE_LITTLE_ENDIAN - 1
*    : BYTE_BIG_ENDIAN    - 2
************************************************************************/
#define  BYTE_ENDIAN  ( BYTE_LITTLE_ENDIAN )

#define  BYTE_LITTLE_ENDIAN    ( 1 )
#define  BYTE_BIG_ENDIAN       ( 2 )


/***********************************************************************
* Macros: BIT_FIELD_ENDIAN
*    BIT_FIELD_ENDIAN
*
*    : BIT_FIELD_LITTLE_ENDIAN - 1
*    : BIT_FIELD_BIG_ENDIAN    - 2
************************************************************************/
#define  BIT_FIELD_ENDIAN  ( BIT_FIELD_LITTLE_ENDIAN )

#define  BIT_FIELD_LITTLE_ENDIAN    ( 1 )
#define  BIT_FIELD_BIG_ENDIAN       ( 2 )


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif  /* CLIB_DRIVERS_CONFIG_H */ 


 
