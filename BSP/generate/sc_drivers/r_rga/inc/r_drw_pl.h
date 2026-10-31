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
* File: r_drw_pl.h
*    OS Porting Layer of DRW driver.
*
* - $ModuleRevision: {Masters}\DRW\D_AVE_2D\RZA2\D_AVE_2D\Driver\26_jp $
* - $Rev: $
* - $Date: 2018-11-09T16:53:20+09:00 $
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "r_typedefs.h"
#include  "r_drw_typedef.h"
#include  "rz_co_typedef.h"

#ifndef R_DRW_PL_H
#define R_DRW_PL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
/* In "r_drw_pl_typedef.h" */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* In "r_drw_pl_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* In "r_drw_pl_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function: R_DRW_OnInitialize
*    Callback function called at initializing DRW
*
* Arguments:
*    None
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    DRW driver expects this function to do the following operation.
*    - Starts to supply DRW clock
*    - Registers DRW interrupt callback function calling <R_DRW_OnInterrupting> function
*    - Sets DRW interrupt priority
************************************************************************/
errnum_t  R_DRW_OnInitialize(void);


/***********************************************************************
* Function: R_DRW_OnFinalize
*    Callback function called at finalizing DRW
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    DRW driver expects this function to do the following operation.
*    - Stop to supply DRW clock
************************************************************************/
void  R_DRW_OnFinalize(void);


/***********************************************************************
* Function: R_DRW_EnableInterrupt
*    Enables DRW interrupt
*
* Arguments:
*    None
*
* Return Value:
*    None
************************************************************************/
void  R_DRW_EnableInterrupt(void);


/***********************************************************************
* Function: R_DRW_DisableInterrupt
*    Disables DRW interrupt
*
* Arguments:
*    None
*
* Return Value:
*    Whether DRW interrupt have been enabled until now
************************************************************************/
bool_t  R_DRW_DisableInterrupt(void);


/***********************************************************************
* Function: R_DRW_SetInterruptEvent
*    Notifies from DRW interrupt to the thread
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    This function notifies to waiting RTOS thread calling <R_DRW_WaitForInterruptEvent>
*    function.
*
*    If this function is called from interrupt, this function will be called from
*    the inside of <R_DRW_OnInterrupting> function.
*    Not only interrupt function but also any thread calls this function.
*    Even if you call this function continuously, <R_DRW_WaitForInterruptEvent>
*    will not return continuously.
************************************************************************/
void  R_DRW_SetInterruptEvent(void);


/***********************************************************************
* Function: R_DRW_WaitForInterruptEvent
*    Waits for DRW interrupt
*
* Arguments:
*    None
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    In the OS-less environment, this function does the polling inside.
*    In RTOS environment, change this function define to the code of
*    calling binary semaphore, event group or thread attached event API.
*
*    The thread calling this function is same as the thread calling
*    <R_DRW_OnInitialize> function.
*
*    This function returns after calling <R_DRW_SetInterruptEvent>
*    (Case1 of <Figure_SequenceOfWaitForInterrupEvent> ).
*
*    If <R_DRW_SetInterruptEvent> function was already called,
*    this function returns immediately
*    (Case2 of <Figure_SequenceOfWaitForInterrupEvent> ).
*
*    If <R_DRW_SetInterruptEvent> function was called only before previous calling this function,
*    even if <R_DRW_SetInterruptEvent> function was called many times,
*    this function does not return immediately
*    (Case3 of <Figure_SequenceOfWaitForInterrupEvent> ).
************************************************************************/
errnum_t  R_DRW_WaitForInterruptEvent(void);


/***********************************************************************
* Function: R_DRW_CheckThread
*    Checks if the current thread is locked thread
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    If the thread calling this function is not same as the thread calling
*    <R_DRW_OnInitialize> function (=the thread locking DRW),
*    there may be a problem related exclusive control.
*    Calling RGA API must be from one thread by using thread communication for example.
*    If your environment is OS less, this function can return without any operations.
************************************************************************/
void  R_DRW_CheckThread(void);


/***********************************************************************
* Function: R_DRW_FlushCache
*    Flushes level 1 CPU cache
*
* Arguments:
*    in_StartAddress - Start address of to flush
*    in_Size         - Size (byte) to flush
*
* Return Value:
*    None
*
* Description:
*    Flush operation of this function is to writeback and to discard.
*    Arguments of this function can be ignored, if all cache lines are flushed.
************************************************************************/
void  R_DRW_FlushCache( const void*  in_StartAddress,  uint32_t  in_Size );


/***********************************************************************
* Function: R_DRW_ToPhysicalAddress
*    Changes to physical address
*
* Arguments:
*    in_Address          - Virtual address
*    out_PhysicalAddress - Output: Virtual address for cached area
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Example:
*    > // Case that physical address is integer type:
*    > uintptr_t  physical_address;
*    > e= R_DRW_ToPhysicalAddress( address, &physical_address ); if(e){goto fin;}
*    >
*    > // Case that physical address is pointer type:
*    > uintptr_t  physical_address;
*    > void*  pointer;
*    > e= R_DRW_ToPhysicalAddress( address, &physical_address ); if(e){goto fin;}
*    > pointer = (void*) physical_address;
*
* Description:
*    The define of this function must be modified to same setting with MMU setting.
************************************************************************/
errnum_t  R_DRW_ToPhysicalAddress( const volatile void* const  in_Address,  uintptr_t* const  out_PhysicalAddress );


/***********************************************************************
* Function: R_DRW_ToCachedAddress
*    Checks if the address is in the L1 cache area or changes to L1 cache area
*
* Arguments:
*    in_Address        - Virtual address
*    out_CachedAddress - Output: Virtual address for cached area
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    The define of this function must be modified to same setting with MMU setting.
*    "out_CachedAddress" argument must be passed the address of pointer.
*
*    If the target environment did not have mirror area and "in_Address" argument
*    was passed uncached address, an error is raised.
*
*    If an error was raised, you may know the variable by looking at value of
*    "in_Address" argument and map file.
*
*    If the environment did not have any mirror area, it is not necessary
*    to call the function to change the address. But, the function can be used
*    by checking whether passed address is in cached area or uncached area.
************************************************************************/
errnum_t  R_DRW_ToCachedAddress( const volatile void* const  in_Address,  void* const  out_CachedAddress );


/***********************************************************************
* Function: R_DRW_ToUncachedAddress
*    Checks if the address is in L1 uncached area or changes to L1 uncached area
*
* Arguments:
*    in_Address          - Virtual address
*    out_UncachedAddress - Output: Virtual address for uncached area
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    The define of this function must be modified to same setting with MMU setting.
*    "out_UncachedAddress" argument is passed the address of pointer.
*
*    If the target environment did not have mirror area and "in_Address" argument
*    was passed cached address, an error is raised.
*
*    If an error was raised, you may know the variable by looking at value of
*    "in_Address" argument and map file.
*
*    If the environment did not have any mirror area, it is not necessary to call
*    the function to change the address. But, the function can be used by checking
*    whether passed address is in cached area or uncached area.
************************************************************************/
errnum_t  R_DRW_ToUncachedAddress( const volatile void* const  in_Address,  void* const  out_UncachedAddress );


/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* R_DRW_PL_H */
