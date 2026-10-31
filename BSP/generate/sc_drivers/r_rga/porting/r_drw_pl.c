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
* File        : r_drw_pl.c
* Description : OS Porting Layer of DRW driver - OS abstraction version.
* Version     : 1.12
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "r_drw_pl.h"
#include  "r_drw.h"
#include  "iodefine.h"
#include  "r_intc_drv_api.h"
#include  "r_stb_lld_rza2m.h"
#include  "r_cache_lld_rza2m.h"
#include  "r_os_abstraction_api.h"
#include  "r_memory_map.h"
#include  "rz_co.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Defines: Macros in this file scope. Global static defines (GS)
************************************************************************/

/* Define: GS_DRW_IRQ_PRIORITY */
#define  GS_DRW_IRQ_PRIORITY  ( 28 )


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    gs_on_interrupting( uint32_t const  int_sense );
static bool_t  gs_is_enabled_DRW_interrupt = false; 
static uint32_t  gs_semaphore_as_event_of_DRW;      

/* Cast to an appropriate type */
static os_task_t*  gs_owner_thread = NULL;


/***********************************************************************
* Function Name: R_DRW_OnInitialize
************************************************************************/
errnum_t  R_DRW_OnInitialize(void) 
{
    errnum_t          e;
    int32_t           er;
    bool_t            s;  /* is_success */
    int_fast32_t      oblivious = 0;

    #ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #endif

    volatile uint8_t  dummy_buf;

    #ifdef __GNUC__
    #pragma GCC diagnostic pop
    #endif


    /* Cast to an appropriate type */
    if ( gs_owner_thread != NULL )
    {
        e = E_DRW_THREAD_DENIED_1;
        goto fin;

        /* Error: DRW is not supported multi context. */
    }
    gs_owner_thread = R_OS_TaskGetCurrentHandle();


    /* Starts to supply DRW clock. */
    CPG.STBCR7.BIT.MSTP76 = 0;
    dummy_buf = CPG.STBCR7.BYTE;  /* Waiting to set */ 
    while ( R_STB_RequestModuleStart( MODULE_DRW ) == STB_AGAIN )
    {
        /* DO NOTHING */
        ;
    }

    /* Registers DRW interrupt callback function calling <R_DRW_OnInterrupting> function. */
    er= R_INTC_RegistIntFunc( INTC_ID_DAVE_DRWI,  gs_on_interrupting );
    if ( er )
    {
        oblivious = __LINE__;
    }

    /* Sets DRW interrupt priority */
    er= R_INTC_SetPriority( INTC_ID_DAVE_DRWI,  GS_DRW_IRQ_PRIORITY );
    if ( er )
    {
        oblivious = __LINE__;
    }

    /* R_OS_SemaphoreCreate() */
    /* R_OS_SemaphoreWait() : Count down semaphore resource */
    s= R_OS_SemaphoreCreate( &gs_semaphore_as_event_of_DRW,  1 );
    if(!s)
    {
        e=__LINE__;
        goto fin;
    }
    s= R_OS_SemaphoreWait(   &gs_semaphore_as_event_of_DRW,  0 ); /* Clear. Time out must be ignored. */

    if ( oblivious != 0 )
    {
        e=E_DRW_OBLIVIOUS_1;

        /* Error: Error in "R_DRW_OnInitialize" was ignored. */
    }
    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    if ( oblivious != 0 )
    {
        e=0;
    }
    return  e;
} /* End of function R_DRW_OnInitialize */


/***********************************************************************
* Function Name: R_DRW_OnFinalize
************************************************************************/
void  R_DRW_OnFinalize(void) 
{
    #ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #endif

    volatile uint8_t  dummy_buf;

    #ifdef __GNUC__
    #pragma GCC diagnostic pop
    #endif

    /* Cast to an appropriate type */
    if ( gs_owner_thread == NULL )
    {
        return;
    }

    /* R_OS_SemaphoreDelete */
    R_OS_SemaphoreDelete( &gs_semaphore_as_event_of_DRW );

    /* Stop to supply DRW clock. */
    while ( R_STB_RequestModuleStop( MODULE_DRW ) == STB_AGAIN )
    {
        /* DO NOTHING */
        ;
    }

    /* Cast to an appropriate type */
    CPG.STBCR7.BIT.MSTP76 = 1;
    dummy_buf = CPG.STBCR7.BYTE;  /* Waiting to set */ 

    /* ... */
    gs_owner_thread = NULL;
} /* End of function R_DRW_OnFinalize */


/***********************************************************************
* Function Name: R_DRW_EnableInterrupt
************************************************************************/
void  R_DRW_EnableInterrupt(void) 
{
    errnum_t      e;
    int32_t       er;
    int_fast32_t  oblivious = 0;


    gs_is_enabled_DRW_interrupt = true;

    er= R_INTC_Enable( INTC_ID_DAVE_DRWI ); 
    if ( er )
    {
        oblivious = __LINE__;
    }


    if ( oblivious != 0 )
    {
        e=E_DRW_OBLIVIOUS_2;

        /* Error: Error in "R_DRW_EnableInterrupt" was ignored. */
    }
    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
} /* End of function R_DRW_EnableInterrupt */


/***********************************************************************
* Function Name: R_DRW_DisableInterrupt
************************************************************************/
bool_t  R_DRW_DisableInterrupt(void) 
{
    errnum_t      e;
    int32_t       er;
    int_fast32_t  oblivious = 0;
    bool_t        is_previous_enabled;


    er= R_INTC_Disable( INTC_ID_DAVE_DRWI ); 
    if ( er )
    {
        oblivious = __LINE__;
    }

    is_previous_enabled = gs_is_enabled_DRW_interrupt;
    gs_is_enabled_DRW_interrupt = false;


    if ( oblivious != 0 )
    {
        e=E_DRW_OBLIVIOUS_3;

        /* Error: Error in "R_DRW_DisableInterrupt" was ignored. */
    }
    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  is_previous_enabled;
} /* End of function R_DRW_DisableInterrupt */


/***********************************************************************
* Function Name: gs_on_interrupting
*    Interrupt Handling of DRW.
*
* Arguments:
*    int_sense - interrupt sense.
*
* Return Value:
*    None.
************************************************************************/
static void  gs_on_interrupting( uint32_t const  int_sense )
{
    R_UNREFERENCED_VARIABLE( int_sense ); 

    R_DRW_OnInterrupting();
} /* End of function gs_on_interrupting */


/***********************************************************************
* Function Name: R_DRW_WaitForInterruptEvent
************************************************************************/
errnum_t  R_DRW_WaitForInterruptEvent(void) 
{
    errnum_t  e;
    bool_t    s;  /* is_success */

    s= R_OS_SemaphoreWait( &gs_semaphore_as_event_of_DRW,  R_OS_ABSTRACTION_EV_WAIT_INFINITE );
    if(!s)
    {
        e=__LINE__;
        goto fin;
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_DRW_WaitForInterruptEvent */


/***********************************************************************
* Function Name: R_DRW_SetInterruptEvent
************************************************************************/
void  R_DRW_SetInterruptEvent(void) 
{
    R_OS_SemaphoreRelease( &gs_semaphore_as_event_of_DRW );
} /* End of function R_DRW_SetInterruptEvent */


/***********************************************************************
* Function Name: R_DRW_CheckThread
************************************************************************/
void  R_DRW_CheckThread(void) 
{
    if ( R_OS_TaskGetCurrentHandle() != gs_owner_thread )
    {
        R_ERROR_Set( E_DRW_OBLIVIOUS_6 );

        /* Error: DRW is not supported multi thread. */
    }
} /* End of function R_DRW_CheckThread */


/***********************************************************************
* Function Name: R_DRW_FlushCache
************************************************************************/
void  R_DRW_FlushCache( const void*  in_StartAddress,  uint32_t  in_Size ) 
{
    R_UNREFERENCED_VARIABLE_2( in_StartAddress,  in_Size ); 

    R_CACHE_L1DataCleanInvalidAll();
} /* End of function R_DRW_FlushCache */


/***********************************************************************
* Function Name: R_DRW_ToPhysicalAddress
************************************************************************/
errnum_t  R_DRW_ToPhysicalAddress 
            ( const volatile void* const  in_Address,  uintptr_t* const  out_PhysicalAddress )
{
    return  R_MEMORY_ToPhysicalAddress( in_Address,  out_PhysicalAddress );
} /* End of function R_DRW_ToPhysicalAddress */


/***********************************************************************
* Function Name: R_DRW_ToCachedAddress
************************************************************************/
errnum_t  R_DRW_ToCachedAddress 
            ( const volatile void* const  in_Address,  void* const  out_CachedAddress )
{
    return  R_MEMORY_ToCachedAddress( in_Address,  out_CachedAddress );
} /* End of function R_DRW_ToCachedAddress */


/***********************************************************************
* Function Name: R_DRW_ToUncachedAddress
************************************************************************/
errnum_t  R_DRW_ToUncachedAddress 
            ( const volatile void* const  in_Address,  void* const  out_UncachedAddress )
{
    return  R_MEMORY_ToUncachedAddress( in_Address,  out_UncachedAddress );
} /* End of function R_DRW_ToUncachedAddress */


