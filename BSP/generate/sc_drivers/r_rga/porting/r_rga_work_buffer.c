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
* File Name   : r_rga_work_buffer.c
* Version     : 1.12
************************************************************************/

#include  "use_config_rga.h"
#include  "r_typedefs.h"
#include  "rga.h"
#include  "r_rga_work_buffer.h"


/***********************************************************************
* Define: MAX_WIDTH_OF_WORK_BUFFER_B
************************************************************************/
#define  MAX_WIDTH_OF_WORK_BUFFER_B  ( 128 )


/***********************************************************************
* Define: MAX_HEIGHT_OF_WORK_BUFFER_B
************************************************************************/
#define  MAX_HEIGHT_OF_WORK_BUFFER_B  ( 88 )


/***********************************************************************
* Constant: WORK_BUFFER_B_SIZE
*
* Description:
*    > WorkBufferB [ For PNG | For Decoder ]
************************************************************************/
#ifdef  USE_CONFIG_RGA_WORK_BUFFER_B
    #define  WORK_BUFFER_B_SIZE \
    ( \
        WORK_SIZE_FOR_LIB_PNG + \
    \
        R_RGA_CalcWorkBufferB_Size( \
            MAX_WIDTH_OF_WORK_BUFFER_B,  MAX_HEIGHT_OF_WORK_BUFFER_B,  4 \
        ) \
    )
#else
    #define  WORK_BUFFER_B_SIZE  ( 4 )
#endif


/***********************************************************************
* Variable: gs_work_buffer_B_memory
************************************************************************/
static uint8_t  gs_work_buffer_B_memory[ WORK_BUFFER_B_SIZE ]
    __attribute__ ((section("Graphics_RAM")));


/***********************************************************************
* Function Name: R_MEMORY_SECTION_GetMemory_RGA_WorkBuffer
*     Get Memory of RGA WorkBuffer
*
* Arguments:
*    out_WorkBufferAddress   - uint8_t*
*    out_WorkBufferSize      - size_t
*    out_WorkBufferB_Address - uint8_t*
*    out_WorkBufferB_Size    - size_t
*
* Return Value:
*    None
************************************************************************/
void  R_MEMORY_SECTION_GetMemory_RGA_WorkBuffer(
    uint8_t** out_WorkBufferAddress,  size_t* out_WorkBufferSize,
    uint8_t** out_WorkBufferB_Address,  size_t* out_WorkBufferB_Size )
{
    /* Cast to an appropriate type */
    if ( out_WorkBufferAddress != NULL )
    {
        /* Cast to an appropriate type */
        *out_WorkBufferAddress = NULL;
    }

    /* Cast to an appropriate type */
    if ( out_WorkBufferSize != NULL )
    {
        *out_WorkBufferSize = 0;
    }

    *out_WorkBufferB_Address = gs_work_buffer_B_memory;
    *out_WorkBufferB_Size = sizeof( gs_work_buffer_B_memory );
} /* End of function R_MEMORY_SECTION_GetMemory_RGA_WorkBuffer */


