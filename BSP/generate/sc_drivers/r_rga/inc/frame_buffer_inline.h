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
* File        : frame_buffer_inline.h
* Description : This file is included from "frame_buffer.h"
* Version     : 1.12
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "clib_drivers_typedef.h"
#include  "frame_buffer_typedef.h"
#include  "frame_buffer_format_typedef.h"
#include  "rz_co_typedef.h"

#ifndef  FRAME_BUFFER_INLINE_H
#define  FRAME_BUFFER_INLINE_H
#ifndef  NOT_DEFINE_INLINE_FUNCTION

#ifdef  __cplusplus
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/

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
* Function Name: argb8888_t_to_rgb565_t
************************************************************************/
#ifdef _SH
#pragma inline  argb8888_t_to_rgb565_t
#endif
STATIC_INLINE  rgb565_t  argb8888_t_to_rgb565_t( argb8888_t in_Color )
{
    rgb565_t  ret = {0}; 

        /* Warning: C4017W: ret may be used before being set */
        /* This warning is not correct */

    /* Cast to an appropriate type */
    ret.Value = (uint16_t)(
        ( ( in_Color.Value & 0x00F80000 ) >> 8 ) |
        ( ( in_Color.Value & 0x0000FC00 ) >> 5 ) |
        ( ( in_Color.Value & 0x000000F8 ) >> 3 ) );

    return  ret;
}/* End of function argb8888_t_to_rgb565_t */


/***********************************************************************
* Function Name: argb8888_t_to_argb1555_t
************************************************************************/
#ifdef _SH
#pragma inline  argb8888_t_to_argb1555_t
#endif
STATIC_INLINE  argb1555_t  argb8888_t_to_argb1555_t( argb8888_t in_Color )
{
    argb1555_t  ret = {0}; 

        /* Warning: C4017W: ret may be used before being set */
        /* This warning is not correct */

    /* Cast to an appropriate type */
    ret.Value = (uint16_t)(
        ( ( in_Color.Value & 0x80000000 ) >> 16 ) |  /* A */
        ( ( in_Color.Value & 0x00F80000 ) >>  9 ) |  /* R */
        ( ( in_Color.Value & 0x0000F800 ) >>  6 ) |  /* G */
        ( ( in_Color.Value & 0x000000F8 ) >>  3 ) ); /* B */

    return  ret;
}/* End of function argb8888_t_to_argb1555_t */


/***********************************************************************
* Function Name: argb8888_t_to_argb4444_t
************************************************************************/
#ifdef _SH
#pragma inline  argb8888_t_to_argb4444_t
#endif
STATIC_INLINE  argb4444_t  argb8888_t_to_argb4444_t( argb8888_t in_Color )
{
    argb4444_t  ret = {0}; 

        /* Warning: C4017W: ret may be used before being set */
        /* This warning is not correct */

    /* Cast to an appropriate type */
    ret.Value = (uint16_t)(
        ( ( in_Color.Value & 0xF0000000 ) >> 16 ) |
        ( ( in_Color.Value & 0x00F00000 ) >> 12 ) |
        ( ( in_Color.Value & 0x0000F000 ) >>  8 ) |
        ( ( in_Color.Value & 0x000000F0 ) >>  4 ) );

    return  ret;
}/* End of function argb8888_t_to_argb4444_t */


/***********************************************************************
* Function Name: argb8888_t_to_r8g8b8a8_t
************************************************************************/
#ifdef _SH
#pragma inline  argb8888_t_to_r8g8b8a8_t
#endif
STATIC_INLINE  r8g8b8a8_t  argb8888_t_to_r8g8b8a8_t( argb8888_t in_Color )
{
    r8g8b8a8_t  ret; 
        ret.Value = 0;

        /* Warning: C4017W: ret may be used before being set */
        /* This warning is not correct */

    ret.u.Red   = in_Color.u.Red;
    ret.u.Green = in_Color.u.Green;
    ret.u.Blue  = in_Color.u.Blue;
    ret.u.Alpha = in_Color.u.Alpha;

    return  ret;
}/* End of function argb8888_t_to_r8g8b8a8_t */


/***********************************************************************
* Function Name: rgb565_t_to_argb8888_t
************************************************************************/
#ifdef _SH
#pragma inline  rgb565_t_to_argb8888_t
#endif
STATIC_INLINE  argb8888_t  rgb565_t_to_argb8888_t( rgb565_t in_Color )
{
    argb8888_t  ret; 

    ret.Value = 0xFF000000 | /* A */
        ( ( in_Color.Value & 0xF800u ) << 8 ) | ( ( in_Color.Value & 0xE000u ) << 3 ) |  /* R */
        ( ( in_Color.Value & 0x07E0u ) << 5 ) | ( ( in_Color.Value & 0x0600u ) >> 1 ) |  /* G */
        ( ( in_Color.Value & 0x001Fu ) << 3 ) | ( ( in_Color.Value & 0x001Cu ) >> 2 );   /* B */

    return  ret;
}/* End of function rgb565_t_to_argb8888_t */


/***********************************************************************
* Function Name: argb1555_t_to_argb8888_t
************************************************************************/
#ifdef _SH
#pragma inline  argb1555_t_to_argb8888_t
#endif
STATIC_INLINE  argb8888_t  argb1555_t_to_argb8888_t( argb1555_t in_Color )
{
    argb8888_t  ret; 

    ret.Value =
        ( ( in_Color.Value & 0x7C00u ) << 9 ) | ( ( in_Color.Value & 0x7000u ) << 4 ) |  /* R */
        ( ( in_Color.Value & 0x03E0u ) << 6 ) | ( ( in_Color.Value & 0x0380u ) << 1 ) |  /* G */
        ( ( in_Color.Value & 0x001Fu ) << 3 ) | ( ( in_Color.Value & 0x001Cu ) >> 2 );   /* B */

    if ( in_Color.u.Alpha )
        { ret.Value |= 0xFF000000; }  /* A */

    return  ret;
}/* End of function argb1555_t_to_argb8888_t */


/***********************************************************************
* Function Name: argb4444_t_to_argb8888_t
************************************************************************/
#ifdef _SH
#pragma inline  argb4444_t_to_argb8888_t
#endif
STATIC_INLINE  argb8888_t  argb4444_t_to_argb8888_t( argb4444_t in_Color )
{
    argb8888_t  ret; 

    ret.Value =
        ( ( in_Color.Value & 0xF000u ) << 16 ) | ( ( in_Color.Value & 0xF000u ) << 12 ) |  /* A */
        ( ( in_Color.Value & 0x0F00u ) << 12 ) | ( ( in_Color.Value & 0x0F00u ) <<  8 ) |  /* R */
        ( ( in_Color.Value & 0x00F0u ) <<  8 ) | ( ( in_Color.Value & 0x00F0u ) <<  4 ) |  /* G */
        ( ( in_Color.Value & 0x000Fu ) <<  4 ) | ( ( in_Color.Value & 0x000Fu )       );   /* B */

    return  ret;
}/* End of function argb4444_t_to_argb8888_t */


/***********************************************************************
* Function Name: rgba8888_t_to_argb8888_t
************************************************************************/
#ifdef _SH
#pragma inline  rgba8888_t_to_argb8888_t
#endif
STATIC_INLINE argb8888_t  rgba8888_t_to_argb8888_t( rgba8888_t in_Color )
{
    argb8888_t  ret; 

    ret.Value = (
        ( ( in_Color.Value & 0xFF000000 ) >>  8 ) |  /* R */
        ( ( in_Color.Value & 0x00FF0000 ) >>  8 ) |  /* G */
        ( ( in_Color.Value & 0x0000FF00 ) >>  8 ) |  /* B */
        ( ( in_Color.Value & 0x000000FF ) << 24 ) ); /* A */

    return  ret;
}/* End of function rgba8888_t_to_argb8888_t */


/***********************************************************************
* Function Name: rgba8888_t_to_rgb565_t
************************************************************************/
#ifdef _SH
#pragma inline  rgba8888_t_to_rgb565_t
#endif
STATIC_INLINE rgb565_t  rgba8888_t_to_rgb565_t( rgba8888_t in_Color )
{
    rgb565_t  ret; 

    /* Cast to an appropriate type */
    ret.Value = (uint16_t) (
        ( ( in_Color.Value & 0xF8000000 ) >> 16 ) |  /* R */
        ( ( in_Color.Value & 0x00FC0000 ) >> 13 ) |  /* G */
        ( ( in_Color.Value & 0x0000F800 ) >> 11 ) ); /* B */

    return  ret;
}/* End of function rgba8888_t_to_rgb565_t */


/***********************************************************************
* Function Name: rgba8888_t_to_argb1555_t
************************************************************************/
#ifdef _SH
#pragma inline  rgba8888_t_to_argb1555_t
#endif
STATIC_INLINE argb1555_t  rgba8888_t_to_argb1555_t( rgba8888_t in_Color )
{
    argb1555_t  ret; 

    /* Cast to an appropriate type */
    ret.Value = (uint16_t) (
        ( ( in_Color.Value & 0xF8000000 ) >> 17 ) |  /* R */
        ( ( in_Color.Value & 0x00F80000 ) >> 14 ) |  /* G */
        ( ( in_Color.Value & 0x0000F800 ) >> 11 ) |  /* B */
        ( ( in_Color.Value & 0x00000080 ) <<  8 ) ); /* A */

    return  ret;
}/* End of function rgba8888_t_to_argb1555_t */


/***********************************************************************
* Function Name: rgba8888_t_to_argb4444_t
************************************************************************/
#ifdef _SH
#pragma inline  rgba8888_t_to_argb4444_t
#endif
STATIC_INLINE argb4444_t  rgba8888_t_to_argb4444_t( rgba8888_t in_Color )
{
    argb4444_t  ret; 

    /* Cast to an appropriate type */
    ret.Value = (uint16_t) (
        ( ( in_Color.Value & 0xF0000000 ) >> 20 ) |  /* R */
        ( ( in_Color.Value & 0x00F00000 ) >> 16 ) |  /* G */
        ( ( in_Color.Value & 0x0000F000 ) >> 12 ) |  /* B */
        ( ( in_Color.Value & 0x000000F0 ) <<  8 ) ); /* A */

    return  ret;
}/* End of function rgba8888_t_to_argb4444_t */


/***********************************************************************
* Function Name: r8g8b8a8_t_to_argb8888_t
************************************************************************/
#ifdef _SH
#pragma inline  r8g8b8a8_t_to_argb8888_t
#endif
STATIC_INLINE  argb8888_t  r8g8b8a8_t_to_argb8888_t( r8g8b8a8_t in_Color )
{
    argb8888_t  ret; 

    ret.Value =

        /* Cast to an appropriate type */
        ( (uint_fast32_t) in_Color.u.Alpha << 24 ) |

        /* Cast to an appropriate type */
        ( (uint_fast32_t) in_Color.u.Red   << 16 ) |

        /* Cast to an appropriate type */
        ( (uint_fast32_t) in_Color.u.Green <<  8 ) |

        /* Cast to an appropriate type */
        ( (uint_fast32_t) in_Color.u.Blue );

    return  ret;
}/* End of function r8g8b8a8_t_to_argb8888_t */


#ifdef  __cplusplus
}  /* extern "C" */
#endif

#endif /* NOT_DEFINE_INLINE_FUNCTION */
#endif  /* FRAME_BUFFER_INLINE_H */
