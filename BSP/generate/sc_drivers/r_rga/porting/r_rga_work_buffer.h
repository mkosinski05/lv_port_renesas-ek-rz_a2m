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
* File Name   : r_rga_work_buffer.h
* Version     : 1.12
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_rga.h"
#include  "r_typedefs.h"

#ifndef RGA_WORKBUFFER_H
#define RGA_WORKBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Define: WORK_SIZE_FOR_LIB_PNG
************************************************************************/
#ifdef  USE_CONFIG_RGA_WORK_BUFFER_B
    #define  WORK_SIZE_FOR_LIB_PNG   ( 0x10000 )
#else
    #define  WORK_SIZE_FOR_LIB_PNG   ( 1 )
#endif


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Function Name:R_MEMORY_SECTION_GetMemory_RGA_WorkBuffer
******************************************************************************/
void  R_MEMORY_SECTION_GetMemory_RGA_WorkBuffer(
    uint8_t** out_WorkBufferAddress,  size_t* out_WorkBufferSize,
    uint8_t** out_WorkBufferB_Address,  size_t* out_WorkBufferB_Size );

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* RGA_WORKBUFFER_H */
