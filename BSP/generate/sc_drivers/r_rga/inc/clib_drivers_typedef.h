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
* File: clib_drivers_typedef.h
*    Common code for drivers and more.
*
* - $ModuleRevision: {Masters}\CLibCommon\OS_less_RGA_RZA2_Test\33 $
* - $Date: 2018-11-16T16:05:46+09:00 $
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stdint.h>
#include  "clib_drivers_config.h"

#ifndef  CLIB_DRIVERS_TYPEDEF_H
#define  CLIB_DRIVERS_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Constants: DUMMY_INITIAL_VALUE
*    Avoids not right warning of not initialized.
*
* Description:
* - 0xDEDEDEDE means "not initialized".
* - Disable VC++ warning C4701 : local variable may be used without having
*   been initialized
* - See <DUMMY_INITIAL_VALUE_8BIT>, <DUMMY_INITIAL_VALUE_16BIT>
************************************************************************/
#ifndef  R_OSPL_NDEBUG
	#define  DUMMY_INITIAL_VALUE  ( 0xDEDEDEDE )
#else
	#define  DUMMY_INITIAL_VALUE  (          0 )
#endif


/***********************************************************************
* Constants: DUMMY_INITIAL_VALUE_8BIT
*    <DUMMY_INITIAL_VALUE> for 8 bit integer.
************************************************************************/
#ifndef  R_OSPL_NDEBUG
	#define  DUMMY_INITIAL_VALUE_8BIT  ( 0xDE )
#else
	#define  DUMMY_INITIAL_VALUE_8BIT  (    0 )
#endif


/***********************************************************************
* Constants: DUMMY_INITIAL_VALUE_16BIT
*    <DUMMY_INITIAL_VALUE> for 16 bit integer.
************************************************************************/
#ifndef  R_OSPL_NDEBUG
	#define  DUMMY_INITIAL_VALUE_16BIT  ( 0xDEDE )
#else
	#define  DUMMY_INITIAL_VALUE_16BIT  (      0 )
#endif


/***********************************************************************
* Constants: U8_255
*    Magic number of generally well known used.
*    unsigned 8bit max value = UINT8_MAX
************************************************************************/
#define  U8_255   ( 255 )


/***********************************************************************
* Macro: R_CEIL_2U
*    Fast ceil operation. See <R_Ceil_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_CEIL_2U( value )   (((value)+1)&~1)


/***********************************************************************
* Macro: R_CEIL_4U
*    Fast ceil operation. See <R_Ceil_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_CEIL_4U( value )   (((value)+3)&~3)


/***********************************************************************
* Macro: R_CEIL_8U
*    Fast ceil operation. See <R_Ceil_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_CEIL_8U( value )   (((value)+7)&~7)


/***********************************************************************
* Macro: R_CEIL_16U
*    Fast ceil operation. See <R_Ceil_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_CEIL_16U( value )  (((value)+15)&~15)


/***********************************************************************
* Macro: R_CEIL_32U
*    Fast ceil operation. See <R_Ceil_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_CEIL_32U( value )  (((value)+31)&~31)


/***********************************************************************
* Macro: R_CEIL_64U
*    Fast ceil operation. See <R_Ceil_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_CEIL_64U( value )  (((value)+63)&~63)


/***********************************************************************
* Macro: R_FLOOR_2U
*    Fast floor operation. See <R_Floor_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_FLOOR_2U( value )   ((value)&~1)


/***********************************************************************
* Macro: R_FLOOR_4U
*    Fast floor operation. See <R_Floor_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_FLOOR_4U( value )   ((value)&~3)


/***********************************************************************
* Macro: R_FLOOR_8U
*    Fast floor operation. See <R_Floor_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_FLOOR_8U( value )   ((value)&~7)


/***********************************************************************
* Macro: R_FLOOR_16U
*    Fast floor operation. See <R_Floor_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_FLOOR_16U( value )  ((value)&~15)


/***********************************************************************
* Macro: R_FLOOR_32U
*    Fast floor operation. See <R_Floor_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_FLOOR_32U( value )  ((value)&~31)


/***********************************************************************
* Macro: R_FLOOR_64U
*    Fast floor operation. See <R_Floor_N>.
*    This calculates with constant value.
************************************************************************/
#define  R_FLOOR_64U( value )  ((value)&~63)


/***********************************************************************
* Macro: sprintf_s_ds
*    A proxy of "sprintf_s" for gcc.
************************************************************************/
#if  defined( __GNUC__ )  &&  defined( USE_OPT_LIB )
#define  sprintf_s_ds  sprintf_s_ds_gcc   /* @suppress("Macro naming") @suppress("Macro expansion") */ 
#else
#define  sprintf_s_ds  sprintf_s          /* @suppress("Macro naming") @suppress("Macro expansion") */ 
#endif


/***********************************************************************
* Macro: sprintf_s_dds
*    A proxy of "sprintf_s" for gcc.
************************************************************************/
#if  defined( __GNUC__ )  &&  defined( USE_OPT_LIB )
#define  sprintf_s_dds  sprintf_s_dds_gcc   /* @suppress("Macro naming") @suppress("Macro expansion") */ 
#else
#define  sprintf_s_dds  sprintf_s           /* @suppress("Macro naming") @suppress("Macro expansion") */ 
#endif


/***********************************************************************
* Constant: E_CLIB
*    Error code group of the common C language. 
************************************************************************/
/* Constant: E_CLIB_MASK
*    Mask for checking E_CLIB. */
#define  E_CLIB       ( 0x00C20000 )
#define  E_CLIB_MASK  ( 0x00FF0000 )

/* Sub class of E_OTHER */
#define  E_CLIB_OTHER_1   ( 0x01C20001 )
#define  E_CLIB_OTHER_2   ( 0x01C20002 )
#define  E_CLIB_OTHER_3   ( 0x01C20003 )
#define  E_CLIB_OTHER_4   ( 0x01C20004 )
#define  E_CLIB_OTHER_5   ( 0x01C20005 )


/* Sub class of E_OBLIVIOUS */
#define  E_CLIB_OBLIVIOUS_1   ( 0x0BC20001 )
#define  E_CLIB_OBLIVIOUS_2   ( 0x0BC20002 )
#define  E_CLIB_OBLIVIOUS_3   ( 0x0BC20003 )


/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Type: IntBoxType
************************************************************************/
typedef struct {
	int_fast32_t  MinX;   // @suppress("Data type member naming")
	int_fast32_t  MinY;   // @suppress("Data type member naming")
	int_fast32_t  Width;  // @suppress("Data type member naming")
	int_fast32_t  Height; // @suppress("Data type member naming")

} IntBoxType; // @suppress("Data type naming")


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* In "clib_drivers.h" */

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif  /* CLIB_DRIVERS_TYPEDEF_H */
