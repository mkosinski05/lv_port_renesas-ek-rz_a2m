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
* File Name   : frame_buffer.h
* Description : Frame Buffer and Color Format
* Version     : 1.12
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stdint.h>
#include  "frame_buffer_typedef.h"
#include  "rz_co_typedef.h"

#ifndef  FRAME_BUFFER_H
#define  FRAME_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
/* In "frame_buffer_typedef.h" */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* In "frame_buffer_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* In "frame_buffer_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function Name: R_BIT_PER_PIXEL_GetBytePerPixel
*    Convert from byte per pixel to bit per pixel
*
* Arguments:
*    in_BitPerPixel - Input
*
* Return Value:
*    The value as byte_per_pixel_t
************************************************************************/
STATIC_INLINE byte_per_pixel_t
    R_BIT_PER_PIXEL_GetBytePerPixel( int_fast32_t  in_BitPerPixel ) 
{
    int_fast32_t  bpp_mod_8 = in_BitPerPixel & 0x7;  /* in_BitPerPixel % 8 */ 

    if ( bpp_mod_8 == 0 )
    {
        /*[CheckedNext]Cast to larger type*/
        return  (byte_per_pixel_t)( (uint_fast32_t) in_BitPerPixel / 8 );
    }
    else
    {
        return  in_BitPerPixel << R_BYTE_PER_PIXEL_SHIFT;
    }
} /* End of function R_BIT_PER_PIXEL_GetBytePerPixel */


/***********************************************************************
* Function Name: R_BYTE_PER_PIXEL_GetBitPerPixel
*    Convert from bit per pixel to byte per pixel
*
* Arguments:
*    in_BytePerPixel - Input
*
* Return Value:
*    BitPerPixel
************************************************************************/
STATIC_INLINE int_fast32_t
    R_BYTE_PER_PIXEL_GetBitPerPixel( byte_per_pixel_t  in_BytePerPixel ) 
{
    /*[CheckedNext]Cast to larger type*/
    return  (int_fast32_t)(                                                       /*[CheckedNext]Cast to larger type*/
        ( ( (bit_flags_fast32_t) in_BytePerPixel & R_BYTE_PER_PIXEL_MASK ) * 8 ) |/*[CheckedNext]Cast to larger type*/
        (bit_flags_fast32_t)( ( in_BytePerPixel & R_BIT_PER_PIXEL_MASK ) >> R_BIT_PER_PIXEL_SHIFT ) );
} /* End of function R_BYTE_PER_PIXEL_GetBitPerPixel */


/***********************************************************************
* Function Name: R_BYTE_PER_PIXEL_IsInteger
*    R_BYTE_PER_PIXEL_IsInteger
*
* Arguments:
*    in_BytePerPixel - byte_per_pixel_t
*
* Return Value:
*    Whether "in_BytePerPixel" is integer type.
************************************************************************/
STATIC_INLINE bool_t
    R_BYTE_PER_PIXEL_IsInteger( byte_per_pixel_t  in_BytePerPixel ) 
{
    /*[CheckedNext]Cast to larger type*/
    return  ( (bit_flags_fast32_t) in_BytePerPixel & R_BIT_PER_PIXEL_MASK ) == 0;
} /* End of function R_BYTE_PER_PIXEL_IsInteger */


#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* FRAME_BUFFER_H */
