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
/******************************************************************************
* File: use_config_RGA.h
*    Use Config Ruled Configuration
*
* - $ModuleRevision: {Masters}\RGA\driver\RZA2_Internal\22_jp_B_off $
* - $Rev: $
* - $Date: 2018-11-27T18:51:10+09:00 $
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_debug.h"

#ifndef USE_CONFIG_RGA_H
#define USE_CONFIG_RGA_H

/* USE_LIST_* macro is also defined in other files. */
#define  USE_LIST_RZA2M  /* Define: USE_LIST_RZA2M */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Defines:
*    Whether there is #include related modules.
*
* Description:
*    Macro name must be start from "USE_CONFIG_".
*    Setting must be Define or Not Define in this file.
*    The name of this file must be "use_list_*.h" or "use_list_*_fix.h".
*
*    Code of refering these macros can be written at #if before
*    #include "other_module.h" or at config header file of each modules.
*
*    "use_list_*_fix.h" file must not be changed.
************************************************************************/

   #define  USE_CONFIG_RGA_WORK_BUFFER_B     /* Define: USE_CONFIG_RGA_WORK_BUFFER_B */


/***********************************************************************
* Defines:
*    These define must not be changed.
************************************************************************/
   #define  USE_CONFIG_RGA_INSIDE     /* Define: USE_CONFIG_RGA_INSIDE */
/* #define  USE_CONFIG_FRAME_PIPELINE */     /* Define: USE_CONFIG_FRAME_PIPELINE */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* USE_CONFIG_RGA_H */
