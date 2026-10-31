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
* Copyright (C) 2017 - 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/***********************************************************************
* File: RGA_pl_errnum.h
*    Error Code of RGA porting layer
*
* - $Version: $
* - $ModuleRevision: {Masters}\RGA\driver\RZA2_Test\17_jp $
* - $Date: 2018-11-05T18:37:56+09:00 $
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#ifndef RGA_PL_ERRNUM_H
#define RGA_PL_ERRNUM_H

#ifdef  __cplusplus
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Constant: E_RGA_PL
*    Error code group of the RGA porting layer. 
************************************************************************/
/* Constant: E_RGA_PL_MASK
*    Mask for checking E_RGA_PL. */
#define  E_RGA_PL            ( 0x00740000 )
#define  E_RGA_PL_MASK       ( 0x00FF0000 )

/* Sub class of E_OTHER */
#define  E_RGA_PL_OTHER_1    ( 0x01740001 )
#define  E_RGA_PL_OTHER_2    ( 0x01740002 )
#define  E_RGA_PL_OTHER_3    ( 0x01740003 )


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#ifdef  __cplusplus
}  /* extern "C" */
#endif

#endif /* RGA_PL_ERRNUM_H */
