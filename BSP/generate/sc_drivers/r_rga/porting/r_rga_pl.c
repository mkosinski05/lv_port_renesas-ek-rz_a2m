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
* File        : r_rga_pl.c
* Description : Porting Layer of RGA.
* Version     : 1.12
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_rga.h"
#include  "clib_drivers.h"
#include  "r_os_abstraction_api.h"
#include  "rga.h"
#include  "rga_pl.h"
#include  "r_rga_work_buffer.h"
#include  "rga_pl_errnum.h"


/******************************************************************************
Macro definitions
******************************************************************************/

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
static bool_t    gs_is_used_work_buffer_B = false;       

static uint32_t  gs_semaphore_as_event_of_JCU;           
static uint32_t  gs_semaphore_as_event_of_JCU_terminate; 


/***********************************************************************
* Function Name: R_GRAPHICS_OnInitialize
************************************************************************/
errnum_t  R_GRAPHICS_OnInitialize 
            ( graphics_t*  self,  graphics_config_t*  in_out_Config, void**  out_CalleeDefined )
{
    errnum_t  e;
    bool_t    s;  /* is_success */
    uint8_t*  work_buffer_B_address;     
    size_t    work_buffer_B_size;      

    graphics_config_t  empty;
    R_UNREFERENCED_VARIABLE_2( self,  out_CalleeDefined ); 
    R_GRAPHICS_CONFIG_SetEmpty( &empty );


    /* has_frame_pipeline = ... */
    if ( in_out_Config->has_frame_pipeline == empty.has_frame_pipeline )
    {
        #ifdef  USE_CONFIG_FRAME_PIPELINE
            in_out_Config->has_frame_pipeline = true;
        #else
            in_out_Config->has_frame_pipeline = false;
        #endif
    }


    /* work_buffer_B_address = ... */
    /* work_buffer_B_size = ... */
    R_MEMORY_SECTION_GetMemory_RGA_WorkBuffer( NULL,  NULL,  &work_buffer_B_address,  &work_buffer_B_size );


    /* in_out_Config->work_buffer_b_address = ... */
    if (
        (in_out_Config->work_buffer_b_address == empty.work_buffer_b_address)  ||
        (in_out_Config->work_buffer_b_size    == empty.work_buffer_b_size) )
    {
        uint8_t*  work_buffer_B_aligned_address;    
        size_t
            work_buffer_B_aligned_size;     

        /* (variables) = ... */
        {
            /* Guard */
            if ( !(
                (in_out_Config->work_buffer_b_address == empty.work_buffer_b_address)  &&
                (in_out_Config->work_buffer_b_size    == empty.work_buffer_b_size) ))
            {
                e=E_RGA_PL_OTHER_1;
                goto fin;
            }

                /* Error: About work buffer B, only one of address and size is set. */
            if ( gs_is_used_work_buffer_B )
            {
                e=E_RGA_PL_OTHER_2;

                /* Error: Work buffer B is already used. */
            }

            /* work_buffer_B_aligned_address = ... */
            #if  RGA_WORK_BUFFER_B_ADDRESS_ALIGNMENT == 32  /* Check of "R_Ceil_32u" */
                work_buffer_B_aligned_address = (uint8_t*) R_Ceil_32u( (uintptr_t) work_buffer_B_address );
            #elif  RGA_WORK_BUFFER_B_ADDRESS_ALIGNMENT == 4  /* Check of "R_Ceil_4u" */
                work_buffer_B_aligned_address = (uint8_t*) R_Ceil_4u( (uintptr_t) work_buffer_B_address );
            #else
                #error
            #endif

            /* work_buffer_B_aligned_size = ... */
            {
                ptrdiff_t  offset = work_buffer_B_aligned_address - work_buffer_B_address;

                /*[CheckedNext]No minus*/
                work_buffer_B_aligned_size = work_buffer_B_size - (size_t) offset;
            }
        }

        /* work_buffer_b_address = ... */
        in_out_Config->work_buffer_b_address = work_buffer_B_aligned_address;
        in_out_Config->work_buffer_b_size    = work_buffer_B_aligned_size;

        gs_is_used_work_buffer_B = true;
    }


    /* work_size_for_libPNG = ... */
    in_out_Config->work_size_for_libPNG = WORK_SIZE_FOR_LIB_PNG;


    /* R_OS_SemaphoreCreate() */
    /* R_OS_SemaphoreWait() : Count down semaphore resource */
    s= R_OS_SemaphoreCreate( &gs_semaphore_as_event_of_JCU,            1 );
    if(!s)
    {
        e=__LINE__;
        goto fin;
    }
    s= R_OS_SemaphoreCreate( &gs_semaphore_as_event_of_JCU_terminate,  1 );
    if(!s)
    {
        e=__LINE__;
        goto fin;
    }
    s= R_OS_SemaphoreWait(   &gs_semaphore_as_event_of_JCU,            0 ); /* Clear. Time out must be ignored. */
    s= R_OS_SemaphoreWait(   &gs_semaphore_as_event_of_JCU_terminate,  0 ); /* Clear. Time out must be ignored. */

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_GRAPHICS_OnInitialize */


/***********************************************************************
* Function Name: R_GRAPHICS_OnInitialized
************************************************************************/
errnum_t  R_GRAPHICS_OnInitialized( graphics_t*  self ) 
{
    errnum_t  e;

    e= R_GRAPHICS_SetFillColor( self,  R_RGA_Get_R8G8B8A8( 0,0,0,255 ) );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_SetStrokeColor( self,  R_RGA_Get_R8G8B8A8( 0,0,0,255 ) );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_SetBackgroundColor( self,  R_RGA_Get_R8G8B8A8( 255,255,255,0 ) );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_SetQualityFlags( self,
        GRAPHICS_IMAGE_QUALITY_NONANTIALIASED |
        GRAPHICS_RENDERING_QUALITY_NONANTIALIASED |
        GRAPHICS_IMAGE_DHITHER_OFF );
    if(e)
    {
        goto fin;
    }

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  e;
} /* End of function R_GRAPHICS_OnInitialized */


/***********************************************************************
* Function Name: R_GRAPHICS_OnFinalize
************************************************************************/
errnum_t  R_GRAPHICS_OnFinalize 
            ( graphics_t*  self,  void*  in_CalleeDefined,  errnum_t  e )
{
    gs_is_used_work_buffer_B = false;
    R_UNREFERENCED_VARIABLE_2( self,  in_CalleeDefined ); 

    /* R_OS_SemaphoreDelete */
    R_OS_SemaphoreDelete( &gs_semaphore_as_event_of_JCU );
    R_OS_SemaphoreDelete( &gs_semaphore_as_event_of_JCU_terminate );

    return  e;
} /* End of function R_GRAPHICS_OnFinalize */


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_WaitForCodecEvent
************************************************************************/
errnum_t
    R_GRAPHICS_JCU_WaitForCodecEvent()  
{
    errnum_t  e;
    bool_t    s;  /* is_success */

    s= R_OS_SemaphoreWait( &gs_semaphore_as_event_of_JCU,  R_OS_ABSTRACTION_EV_WAIT_INFINITE );
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
} /* End of function R_GRAPHICS_JCU_WaitForCodecEvent */


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_SetCodecEvent
************************************************************************/
void  R_GRAPHICS_JCU_SetCodecEvent()  
{
    R_OS_SemaphoreRelease( &gs_semaphore_as_event_of_JCU );
} /* End of function R_GRAPHICS_JCU_SetCodecEvent */


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_ClearCodecEvent
************************************************************************/
void  R_GRAPHICS_JCU_ClearCodecEvent()  
{
    R_OS_SemaphoreWait( &gs_semaphore_as_event_of_JCU,  0 ); /* Clear. Time out must be ignored. */
} /* End of function R_GRAPHICS_JCU_ClearCodecEvent */


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_WaitForTerminateEvent
************************************************************************/
errnum_t  R_GRAPHICS_JCU_WaitForTerminateEvent()   
{
    errnum_t  e;
    bool_t    s;  /* is_success */

    s= R_OS_SemaphoreWait( &gs_semaphore_as_event_of_JCU_terminate,  R_OS_ABSTRACTION_EV_WAIT_INFINITE );
    if(!s)
    {
        e=__LINE__;
        goto fin;
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
} /* End of function R_GRAPHICS_JCU_WaitForTerminateEvent */


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_SetTerminateEvent
************************************************************************/
void  R_GRAPHICS_JCU_SetTerminateEvent()  
{
    R_OS_SemaphoreRelease( &gs_semaphore_as_event_of_JCU_terminate );
} /* End of function R_GRAPHICS_JCU_SetTerminateEvent */


/***********************************************************************
* Function Name: R_GRAPHICS_JCU_ClearTerminateEvent
************************************************************************/
void  R_GRAPHICS_JCU_ClearTerminateEvent()  
{
    R_OS_SemaphoreWait( &gs_semaphore_as_event_of_JCU_terminate,  0 ); /* Clear. Time out must be ignored. */
} /* End of function R_GRAPHICS_JCU_ClearTerminateEvent */


