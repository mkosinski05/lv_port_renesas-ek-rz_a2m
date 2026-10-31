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
* File Name   : r_ospl_old_macro.h
* Description : To disable OSPL version 1.xx macros.
* Version     : 1.12
************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "rz_co_typedef.h"

#ifndef R_OSPL_OLD_MACRO_H
#define R_OSPL_OLD_MACRO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/


/*Sub class of E_STATE_CLEAR */
#define  E_OLS_STATE_CLEAR_1  (0x0C010001)


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Macro: IF
*    Breaks and transits to error state, if condition expression is not 0
*
* Arguments:
*    Condition - Condition expression
*
* Return Value:
*    None
**
* Description:
*    This is for portablity.
*    Error handling system was changed to <R_ERROR_Set>.
************************************************************************/
#define  IF  if 


/***********************************************************************
* Macros: IF_DQ
*    Error check on DEBUG and QAC version for faster
************************************************************************/
#define  IF_DQ( x )  if ( 0 ) 


/***********************************************************************
* Function: R_DEBUG_BREAK_IF_ERROR
*    Breaks here, if it is error state
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    This is for portablity.
************************************************************************/
#define  R_DEBUG_BREAK_IF_ERROR()


/***********************************************************************
* Function: R_OSPL_SET_BREAK_ERROR_ID
*    Register to break at raising error at the moment
*
* Arguments:
*    ID - Breaking number of error
*
* Return Value:
*    None
*
* Description:
*    This function does nothing, if "R_OSPL_ERROR_BREAK" macro was defined
*    to be 0. The following descriptions are available, if "R_OSPL_ERROR_BREAK"
*    macro was defined to be 1.
*
*    Set a break point at "R_DebugBreak" function, when the process breaks
*    at the error raised code.
*
*    The number of "ErrorID" argument can be known by "R_DEBUG_BREAK_IF_ERROR"
*    macro or "R_OSPL_GET_ERROR_ID" macro.
*    In multi-threading environment, the number of "ErrorID" argument is the
*    number of raised errors in all threads.
*
*    The following code breaks at first error.
*    > R_OSPL_SET_BREAK_ERROR_ID( 1 );
*
*    The following code breaks at next error after resuming from meny errors.
*    > R_OSPL_SET_BREAK_ERROR_ID( R_OSPL_GET_ERROR_ID() + 1 );
************************************************************************/
#define  R_OSPL_SET_BREAK_ERROR_ID( x )


/***********************************************************************
* Function: R_DEBUG_BREAK
*    Breaks here
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    Does break by calling "R_DebugBreak" function.
*    This macro is not influenced the setting of "R_OSPL_ERROR_BREAK" macro.
************************************************************************/
#define  R_DEBUG_BREAK  R_DebugBreak 


/***********************************************************************
* Macro: ASSERT_R
*    Assertion (Programming By Contract)
*
* Arguments:
*    Condition          - The condition expression expected true
*    goto_fin_Statement - The operation doing at condition is false
*
* Return Value:
*    None
*
* Description:
*    This is for portablity.
************************************************************************/
#ifndef  __cplusplus
    #define  ASSERT_R( Condition, goto_fin_Statement ) \
        do{ if(!(Condition)) { goto_fin_Statement; } } while(0)  
#else
    #define  ASSERT_R( Condition, goto_fin_Statement ) \
        { IF(!(Condition)) { goto_fin_Statement; } }  /* no C5236(I) */   
#endif


/***********************************************************************
* Macro: ASSERT_D
*    Assertion (Programming By Contract) (for Debug configuration only)
*
* Arguments:
*    Condition          - The condition expression expected true
*    goto_fin_Statement - The operation doing at condition is false
*
* Return Value:
*    None
*
* Description:
*    This is for portablity.
************************************************************************/
#if 0 
    #define  ASSERT_D  ASSERT_R 
#else
    /* ->MISRA 19.7 : Function's argument can not get "goto_fin_Statement" */ /* ->SEC M5.1.3 */
    #define  ASSERT_D( Condition, goto_fin_Statement )
    /* <-MISRA 19.7 */ /* <-SEC M5.1.3 */
#endif


/***********************************************************************
* Macro: R_OSPL_SECTION
*    Names section name to function or varaible
*
* Description:
*    This is for portablity.
************************************************************************/
#define  R_OSPL_SECTION  R_VARIABLE_SECTION 


/***********************************************************************
* Macro: R_OSPL_SECTION_FOR_ZERO_INIT
*    Names section name to zero initialized varaible
*
* Description:
*    This is for portablity.
************************************************************************/
#define  R_OSPL_SECTION_FOR_ZERO_INIT  R_VARIABLE_SECTION_FOR_ZERO_INIT 


/***********************************************************************
* Macro: R_OSPL_ALIGNMENT
*    Alignments first addres of global variable
*
* Arguments:
*    ByteCount                     - Value of alignment
*    Declaration_without_Semicolon - Declaration of the variable
*
* Description:
*    This is for portablity.
*
* Example:
*    > R_OSPL_ALIGNMENT( 0x100,
*    > int  g_Buffer[0x1000] );
************************************************************************/
#define  R_OSPL_ALIGNMENT  R_VARIABLE_ALIGNMENT 


/***********************************************************************
* Function: R_NOOP
*    No operation from C++ specification
*
* Arguments:
*    None
*
* Return Value:
*    None
*
* Description:
*    This is for ASSERT_* portablity.
*    Compatible with __noop (MS C++). But our naming rule is not match.
************************************************************************/
#ifdef _SH
#pragma inline R_NOOP
#endif

/******************************************************************************
 * Function Name: R_NOOP
 *****************************************************************************/
STATIC_INLINE void  R_NOOP(void)   
{
    /*DO NOTHING */
    ;
}/* End of function R_NOOP */


/***********************************************************************
* Function: R_OSPL_Delay
*    Waits for a while until passed time
*
* Arguments:
*    in_DelayTime_msec - Time of waiting (millisecond)
*
* Return Value:
*    Error code.  If there is no error, the return value is 0.
************************************************************************/
#define  R_OSPL_Delay  R_FTIMER_Delay 


/***********************************************************************
* Function: R_OSPL_CLEAR_ERROR
*    Clears the error state
*
* Arguments:
*    None
*
* Return Value:
*    None
************************************************************************/
#define  R_OSPL_CLEAR_ERROR()  R_ERROR_Set( E_OLS_STATE_CLEAR_1 ) 


/***********************************************************************
* Function: R_OSPL_ToPhysicalAddress
*    Changes to physical address
*
* Arguments:
*    in_Address          - Virtual address
*    out_PhysicalAddress - Output: Physical address
*
* Return Value:
*    Error code.  If there is no error, the return value is 0.
*
* Description:
*    This function must be modified by MMU setting.
************************************************************************/
#define  R_OSPL_ToPhysicalAddress  R_MEMORY_ToPhysicalAddress 


/***********************************************************************
* Function: R_MEMORY_ToCachedAddress
*    Checks if the address is in the L1 cache area or changes to L1 cache area
*
* Arguments:
*    in_Address        - Virtual address
*    out_CachedAddress - Output: Virtual address for cached area
*
* Return Value:
*    Error code.  If there is no error, the return value is 0.
*
* Description:
*    The define of this function must be modified to same setting with MMU setting.
*    "out_CachedAddress" argument is passed the address of pointer.
*    If the target environment did not have mirror area and "in_Address"
*    argument was passed uncached address, an error is raised.
*    If an error was raised, you may know the variable by
*    looking at value of "in_Address" argument and map file.
*    If the environment did not have any mirror area, it is not necessary
*    to call the function to change the address. But, the function can be
*    used by checking whether passed address is in cached area or uncached area.
************************************************************************/
#define  R_OSPL_ToCachedAddress  R_MEMORY_ToCachedAddress 


/***********************************************************************
* Function: R_MEMORY_ToUncachedAddress
*    Checks if the address is in L1 uncached area or changes to L1 uncached area
*
* Arguments:
*    in_Address          - Virtual address
*    out_UncachedAddress - Output: Virtual address for uncached area
*
* Return Value:
*    Error code.  If there is no error, the return value is 0.
*
* Description:
*    The define of this function must be modified to same setting with MMU setting.
*    "out_UncachedAddress" argument is passed the address of pointer.
*    If the target environment did not have mirror area and "in_Address"
*    argument was passed cached address, an error is raised.
*    If an error was raised, you may know the variable by looking at value
*    of "in_Address" argument and map file.
*    If the environment did not have any mirror area, it is not necessary
*    to call the function to change the address. But, the function can be
*    used by checking whether passed address is in cached area or uncached area.
************************************************************************/
#define  R_OSPL_ToUncachedAddress  R_MEMORY_ToUncachedAddress 


/***********************************************************************
* Function: R_OSPL_MEMORY_Flush
*    Flushes cache memory
*
* Arguments:
*    in_FlushType - The operation of flush
*
* Return Value:
*    None
*
* Description:
*    Call the function of the driver after flushing input output buffer
*    in the cache memory, If the data area accessing by the hardware is
*    on cache and the driver did not manage the cache memory.
*    Whether the driver manages the cache memory is depend on the driver
*    specification.
************************************************************************/
#define  R_OSPL_MEMORY_Flush( x )  R_CACHE_L1DataCleanInvalidAll() 


/***********************************************************************
* Function Name: R_ADDRESS_Add
*    Add an address or pointer's value per byte unit.
*
* Argument:
*    in_BaseAddress - Address number added by "in_Offset"
*    in_Offset      - Adding number
*
* Return Value:
*    Added address number.
*
* Description:
*    > R_ADDRESS_Add( base_address, +offset )
*    is the following calculation.
*    > ( ( (uint8_t*) base_address ) + offset )
*
*    MISRA-C 2004-17.4 requests array instead of pointer.
*    But array can be added per element size only.
*    It is necessary to check return value for accessible range in common
*    with array's index.
************************************************************************/
STATIC_INLINE void*  R_ADDRESS_Add 
    ( const void* const  in_BaseAddress,  int_fast32_t const  in_Offset )
{
    /* Cast to an appropriate type */
    return  ( (uint8_t*) in_BaseAddress ) + in_Offset;
}/* End of function R_ADDRESS_Add */


/***********************************************************************
* Structure: r_ospl_ftimer_spec_t
*    Free run timer specification
************************************************************************/
#define  r_ospl_ftimer_spec_t  r_ftimer_spec_t 


/***********************************************************************
* Function Name: R_OSPL_FTIMER_InitializeIfNot
*    Set up the free running timer
*
* Arguments:
*    out_Specification - NULL is permitted. Output: The precision of the free run timer
*
* Return Value:
*    Error code.  If there is no error, the return value is 0.
*
* Description:
*    The free running timer does not stop.
*
*    If the counter of the free running timer was overflow, the counter returns to 0.
*    Even in interrupt handler, the counter does count up.
*    OSPL free running timer does not use any interrupt.
*
*    Using timer can be selected by "R_OSPL_FTIMER_IS" macro.
*
*    If the free running timer was already set up, this function does not set up it,
*    outputs to "out_Specification" argument and does not raise any error.
*
*    When OSPL API function with timeout or "R_OSPL_Delay" function was called,
*    "R_OSPL_FTIMER_InitializeIfNot" function is callbacked from these functions.
*
*    There is all interrupt disabled area inside.
************************************************************************/
#define  R_OSPL_FTIMER_InitializeIfNot  R_FTIMER_InitializeIfNot 


/***********************************************************************
* Function Name: R_OSPL_FTIMER_GetSpecification
*    Gets the specification of free running timer.
*
* Arguments:
*    out_Specification - Output: The precision of the free run timer
*
* Return Value:
*    None
************************************************************************/
#define  R_OSPL_FTIMER_GetSpecification  R_FTIMER_GetSpecification 


/***********************************************************************
* Function Name: R_OSPL_FTIMER_Get
*    Get current time of free running timer.
*
* Arguments:
*    None
*
* Return Value:
*    The current clock count of free run timer
*
* Description:
*    Call "R_OSPL_FTIMER_InitializeIfNot" function before calling this function.
*    Call "R_OSPL_FTIMER_IsPast" function, when it is determined whether time passed.
*
* Example:
*    > errnum_t              e;
*    > r_ospl_ftimer_spec_t  ts;
*    > uint32_t              start;
*    > uint32_t              end;
*    >
*    > e= R_OSPL_FTIMER_InitializeIfNot( &ts ); IF(e){goto fin;}
*    > start = R_OSPL_FTIMER_Get();
*    >
*    > // The section of measuring
*    >
*    > end = R_OSPL_FTIMER_Get();
*    > printf( "%d msec\n", R_OSPL_FTIMER_CountToTime(
*    >         &ts, end - start ) );
************************************************************************/
#define  R_OSPL_FTIMER_Get  R_FTIMER_Get 


/***********************************************************************
* Function Name: R_OSPL_FTIMER_IsPast
*    Returns whether specified time was passed
*
* Arguments:
*    ts            - Precision of the free running timer
*    in_Now        - Count of current time
*    in_TargetTime - Count of target time
*    out_IsPast    - Output: Whether the target time was past or not
*
* Return Value:
*    Error code.  If there is no error, the return value is 0.
************************************************************************/
#define  R_OSPL_FTIMER_IsPast  R_FTIMER_IsPast 


/***********************************************************************
* Function Name: R_OSPL_FTIMER_TimeToCount
*    Change from mili-second unit to free running timer unit
*
* Arguments:
*    ts      - Precision of the free running timer
*    in_msec - The value of mili-second unit
*
* Return Value:
*    The value of free running timer unit
*
* Description:
*    The fractional part is been round up. (For waiting time must be more
*    than specified time.)
*
*    This function calculates like the following formula.
*    > ( msec * ts->msec_Denominator + ts->msec_Numerator - 1 ) / ts->msec_Numerator
*
*    Attention: If "ts->msec_Denominator" was more than "ts->msec_Numerator",
*    take care of overflow.
************************************************************************/
#define  R_OSPL_FTIMER_TimeToCount  R_FTIMER_TimeToCount 


/***********************************************************************
* Function Name: R_OSPL_FTIMER_CountToTime
*    Change from free running timer unit to mili-second unit
*
* Arguments:
*    ts       - Precision of the free running timer
*    in_Count - The value of free running timer unit
*
* Return Value:
*    The value of mili-second unit
*
* Description:
*    The fractional part is been round down. (Because overflow does not
*    occur, when "Count = r_ospl_ftimer_spec_t::MaxCount" )
*
*    This function calculates like the following formula.
*    > ( Count * ts->msec_Numerator ) / ts->msec_Denominator
************************************************************************/
#define  R_OSPL_FTIMER_CountToTime  R_FTIMER_CountToTime 


/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* R_OSPL_OLD_MACRO_H */
