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
* File Name   : rga_pl.h
* Description : OS Porting Layer of RGA graphics middleware and driver.
* Version     : 1.12
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "r_typedefs.h"

#ifndef RGA_PL_H
#define RGA_PL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
/* In "RGA_pl_typedef.h" */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* In "RGA_pl_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* In "RGA_pl_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function Name: R_GRAPHICS_OnInitialize
*    Callback function setting a default value of <graphics_config_t>
*
* Arguments:
*    self              - Address of the object (non-initialized) at which initialization starts
*    in_out_Config     - Target of setting default value
*    out_CalleeDefined - Output: A value defined by this function.
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    The following list is an example of "out_CalleeDefined" argument.
*    - an address of memory allocated in this function
************************************************************************/
errnum_t  R_GRAPHICS_OnInitialize 
            ( graphics_t*  self,  graphics_config_t*  in_out_Config, void**  out_CalleeDefined );


/***********************************************************************
* Function Name: R_GRAPHICS_OnInitialized
*    Callback function called from the end of <R_GRAPHICS_Initialize>
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_OnInitialized( graphics_t*  self ); 


/***********************************************************************
* Function Name: R_GRAPHICS_OnFinalize
*    Callback function that releases memory allocated in <R_GRAPHICS_OnInitialize> and so on
*
* Arguments:
*    self              - Address of the object that has been finalized
*    in_CalleeDefined  - Output value of "*out_CalleeDefined" argument of <R_GRAPHICS_OnInitialize>
*    e                 - Errors that have occurred. No error = 0
*
* Return Value:
*    Error code or e. 0=No error and "e=0". <errnum_t> type.
************************************************************************/
errnum_t  R_GRAPHICS_OnFinalize 
            ( graphics_t*  self,  void*  in_CalleeDefined,  errnum_t  e );


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_ClearCodecEvent
*    Clears JCU interrupt signaled event
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    This function clears the event that is set by <R_GRAPHICS_JCU_SetCodecEvent>.
************************************************************************/
void  R_GRAPHICS_JCU_ClearCodecEvent(void);


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_SetCodecEvent
*    Notifies from end of JCU interrupt service to the thread
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    See <R_DRW_SetInterruptEvent>.
************************************************************************/
void  R_GRAPHICS_JCU_SetCodecEvent(void);


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_WaitForCodecEvent
*    Waits for JCU interrupt
*
* Arguments:
*    None
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    See <R_DRW_WaitForInterruptEvent>.
************************************************************************/
errnum_t  R_GRAPHICS_JCU_WaitForCodecEvent(void);


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_ClearTerminateEvent
*    Clears JCU terminated event
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    This function clears the event that is set by <R_GRAPHICS_JCU_SetTerminateEvent>.
************************************************************************/
void  R_GRAPHICS_JCU_ClearTerminateEvent(void);


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_SetTerminateEvent
*    Notifies from JCU terminated event to the thread
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    See <R_DRW_SetInterruptEvent>.
************************************************************************/
void  R_GRAPHICS_JCU_SetTerminateEvent(void);


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_WaitForTerminateEvent
*    Waits for JCU terminated event
*
* Arguments:
*    None
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    See <R_DRW_WaitForInterruptEvent>.
************************************************************************/
errnum_t  R_GRAPHICS_JCU_WaitForTerminateEvent(void);


/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* RGA_PL_H */
