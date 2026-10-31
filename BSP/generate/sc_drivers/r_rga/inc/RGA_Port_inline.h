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
* File Name   : rga_port_inline.h
* Description : This file is included from "RGA_Port.h".
* Version     : 1.12
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "clib_drivers.h"
#include  "frame_buffer_typedef.h"
#include  "rga_port_typedef.h"

#ifndef  RGA_PORT_INLINE_H
#define  RGA_PORT_INLINE_H
#ifndef  NOT_DEFINE_INLINE_FUNCTION

#ifdef  __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
/* in "rga_port_typedef.h" */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* in "rga_port_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* in "rga_port_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function Name: SwapEndian2Byte
*    SwapEndian to 2Byte
*
* Arguments:
*    in_out_2byteVariable - Address to 2 byte
*
* Return Value:
*    None
************************************************************************/
STATIC_INLINE void  SwapEndian2Byte( void* in_out_2byteVariable ) 
{
    uint8_t   buffer[2]; 

    /* Cast to an appropriate type */
    uint8_t*  data  

        /* Cast to an appropriate type */
        = (uint8_t*) in_out_2byteVariable;

    buffer[0] = data[0];
    buffer[1] = data[1];

    data[0] = buffer[1];
    data[1] = buffer[0];
}/* End of function SwapEndian2Byte */


/***********************************************************************
* Function Name: SwapEndian4Byte
*    SwapEndian to 4Byte
*
* Arguments:
*    in_out_2byteVariable - Address to 4 byte
*
* Return Value:
*    None
************************************************************************/
STATIC_INLINE void  SwapEndian4Byte( void* in_out_4byteVariable ) 
{
    uint8_t   buffer[4]; 

    /* Cast to an appropriate type */
    uint8_t*  data   

        /* Cast to an appropriate type */
        = (uint8_t*) in_out_4byteVariable;

    buffer[0] = data[0];
    buffer[1] = data[1];
    buffer[2] = data[2];
    buffer[3] = data[3];

    data[0] = buffer[3];
    data[1] = buffer[2];
    data[2] = buffer[1];
    data[3] = buffer[0];
} /* End of function SwapEndian4Byte */


/***********************************************************************
* Function Name: uint32_not_aligned_t_set
*    Sets the value to the address not aligned 4byte
*
* Arguments:
*    out_Variable - <uint32_not_aligned_t>
*    Value - Set value
*
* Return Value:
*    None
************************************************************************/
STATIC_INLINE void  uint32_not_aligned_t_set( uint32_not_aligned_t* out_Variable,  uint32_t in_Value )
{
    #if BYTE_ENDIAN == BYTE_LITTLE_ENDIAN

        /* Cast to an appropriate type */
        out_Variable->byte[0] = (uint8_t)( ( in_Value & 0x000000FF ) );

        /* Cast to an appropriate type */
        out_Variable->byte[1] = (uint8_t)( ( in_Value & 0x0000FF00 ) >>  8 );

        /* Cast to an appropriate type */
        out_Variable->byte[2] = (uint8_t)( ( in_Value & 0x00FF0000 ) >> 16 );

        /* Cast to an appropriate type */
        out_Variable->byte[3] = (uint8_t)( ( in_Value & 0xFF000000 ) >> 24 );
    #else
        out_Variable->byte[0] = (uint8_t)( ( in_Value & 0xFF000000 ) >> 24 );
        out_Variable->byte[1] = (uint8_t)( ( in_Value & 0x00FF0000 ) >> 16 );
        out_Variable->byte[2] = (uint8_t)( ( in_Value & 0x0000FF00 ) >>  8 );
        out_Variable->byte[3] = (uint8_t)( ( in_Value & 0x000000FF ) );
    #endif
} /* End of function uint32_not_aligned_t_set */


/***********************************************************************
* Class: byte_per_pixel_t
************************************************************************/


#ifdef  __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif  /* NOT_DEFINE_INLINE_FUNCTION */
#endif  /* RGA_PORT_INLINE_H */
