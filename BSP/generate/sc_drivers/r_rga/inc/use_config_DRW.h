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
* File Name   : use_config_drw.h
* Description : Use Config for DRW
* Version     : 1.12
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_debug.h"

#ifndef USE_CONFIG_DRW_H
#define USE_CONFIG_DRW_H

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

/* #define  USE_CONFIG_REDIRECT_DRW_API */     /* Define: USE_CONFIG_REDIRECT_DRW_API */
/* #define  USE_CONFIG_PRINT_DISPLAY_LIST */  /* Define: USE_CONFIG_PRINT_DISPLAY_LIST */
/* #define  USE_CONFIG_DUMP_DRW_REGISTER */   /* Define: USE_CONFIG_DUMP_DRW_REGISTER */
/* #define  USE_CONFIG_DRW_INTERRUPT_LOG */   /* Define: USE_CONFIG_DRW_INTERRUPT_LOG */


/***********************************************************************
* Constants:
*    Derived Constants.
************************************************************************/
#if  defined( USE_CONFIG_DUMP_DRW_REGISTER ) || defined( USE_CONFIG_PRINT_DISPLAY_LIST )
    #define  USE_CONFIG_REDIRECT_REGISTER_ACCESS  /* Constant: USE_CONFIG_REDIRECT_REGISTER_ACCESS */
#endif


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

#endif /* USE_CONFIG_DRW_H */
