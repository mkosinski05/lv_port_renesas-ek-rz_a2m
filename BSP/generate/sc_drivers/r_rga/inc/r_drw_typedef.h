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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/***********************************************************************
* File: r_drw_typedef.h
*    OS Porting Layer of DRW driver.
*
* - $ModuleRevision: {Masters}\DRW\D_AVE_2D\RZA2\D_AVE_2D\Driver\26_jp $
* - $Rev: $
* - $Date: 2018-11-05T16:40:48+09:00 $
************************************************************************/

#ifndef R_DRW_TYPEDEF_H
#define R_DRW_TYPEDEF_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
* Constant: E_DRW
*    Error code group of the DRW. 
******************************************************************************/
/* Constant: E_DRW_MASK
*    Mask for checking E_DRW. */
#define  E_DRW_MASK  ( 0x00FF0000 )
#define  E_DRW       ( 0x00790000 )

/* Sub class of E_OBLIVIOUS */
#define  E_DRW_OBLIVIOUS_1  ( 0x0B790001 )
#define  E_DRW_OBLIVIOUS_2  ( 0x0B790002 )
#define  E_DRW_OBLIVIOUS_3  ( 0x0B790003 )
#define  E_DRW_OBLIVIOUS_4  ( 0x0B790004 )
#define  E_DRW_OBLIVIOUS_5  ( 0x0B790005 )
#define  E_DRW_OBLIVIOUS_6  ( 0x0B790006 )

/* Sub class of E_THREAD_DENIED */
#define  E_DRW_THREAD_DENIED_1  ( 0x12790001 )


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* In "r_drw_pl.h" */

/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* R_DRW_TYPEDEF_H */
