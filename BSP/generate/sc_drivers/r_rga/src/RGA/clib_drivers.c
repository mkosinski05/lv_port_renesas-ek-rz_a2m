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
* File Name   : clib_drivers.c
* Description : Common code for drivers and more.
* Version     : 1.12
************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stdarg.h>
#include  <stdio.h>
#include  "clib_drivers.h"
#include  "r_ospl_old_macro.h"
#include  "rz_co.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
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

/***********************************************************************
* Function Name: R_int32_t_to_int8_t
*    Cast with range check
*
* Arguments:
*    input  - Input value
*    output - Output value
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
errnum_t  R_int32_t_to_int8_t( int32_t const  input, int8_t* const  output ) 
{
    errnum_t  e;

    IF ( (input < INT8_MIN)  ||  (input > INT8_MAX) )
    {
        e=E_CLIB_OTHER_2;
        goto fin;
    }
    IF_DQ( output == NULL )
    {
        e=E_CLIB_OTHER_3;
        goto fin;
    }

    /* Cast to an appropriate type */
    *output = (int8_t) input;

    e=0;
fin:
    {
        return e;
    }
} /* End of function R_int32_t_to_int8_t */


/***********************************************************************
* Function Name: R_int32_t_to_int16_t
*    Cast with range check
*
* Arguments:
*    input  - Input value
*    output - Output value
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
errnum_t  R_int32_t_to_int16_t  
            ( int32_t input, int16_t* output )
{
    IF ( (input < (-32768))  ||  (input > 32767) )
    {
        return  E_CLIB_OTHER_4;
    }

    /* Cast to an appropriate type */
    *output = (int16_t) input;
    return  0;
} /* End of function R_int32_t_to_int16_t */


/***********************************************************************
* Function Name: R_int32_t_to_uint8_t
*    Cast with range check
*
* Arguments:
*    input  - Input value
*    output - Output value
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
errnum_t  R_int32_t_to_uint8_t  
            ( int32_t input, uint8_t* output )
{
    IF ( (input < 0)  ||  (input > 255) )
    {
        return  E_CLIB_OTHER_5;
    }

    /* Cast to an appropriate type */
    *output = (uint8_t) input;

    return  0;
} /* End of function R_int32_t_to_uint8_t */


/***********************************************************************
* Function Name: IntBoxType_isSame
*    Get whether 2 "IntBoxType"s are same area.
*
* Arguments:
*    in_BoxA    - A box
*    in_BoxB    - Other box
*    out_IsSame - Whether 2 "IntBoxType"s are same area.
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
errnum_t  IntBoxType_isSame 
            ( const IntBoxType* in_BoxA, const IntBoxType* in_BoxB, bool_t* out_IsSame )
{
    *out_IsSame = (
        (in_BoxA->MinX   == in_BoxB->MinX)  &&
        (in_BoxA->MinY   == in_BoxB->MinY)  &&
        (in_BoxA->Width  == in_BoxB->Width)  &&
        (in_BoxA->Height == in_BoxB->Height) );

    return  0;
} /* End of function IntBoxType_isSame */


/***********************************************************************
* Function Name: sprintf_s
*    Safe "sprintf".
*
* Arguments:
*    in_String     - Address of string as streaming output
*    in_StringSize - Max character count + 1 of "in_String" argument
*    in_Format     - "printf" format
*
* Return Value:
*    Length of output. -1=Error
************************************************************************/
int_t  sprintf_s( char_t* in_String, size_t in_StringSize, const char_t* in_Format, ... )
{
    va_list  va;
    int_t  ret;

    va_start( va, in_Format );
    ret = vsprintf( in_String, in_Format, va );
    va_end( va );

    ASSERT_R( ret <= in_StringSize,  R_ERROR_Set( E_CLIB_OBLIVIOUS_1 ) ); 
    return  ret;
} /* End of function sprintf_s */


/***********************************************************************
* Function Name: sprintf_s_ds_gcc
*    "sprintf" for gcc. Types are int, int, char*.
*
* Arguments:
*    in_String     - Address of string as streaming output
*    in_StringSize - Max character count + 1 of "in_String" argument
*    in_Format     - "printf" format
*    a1, a2, a3    - "printf" arguments
*
* Return Value:
*    Length of output. -1=Error
*
* Description:
*    This function is a proxy of "sprintf_s".
*    "vsprintf" with "%s" can not run in gcc.
************************************************************************/
#if  defined( __GNUC__ )
int_t  sprintf_s_ds_gcc( char_t* in_String,  size_t  in_StringSize,  const char_t* in_Format,
    int_t  a1,  char_t* a2 )
{
    int_t  ret;

    ret = sprintf( in_String, in_Format, a1, a2 );

    ASSERT_R( ret <= in_StringSize,  R_ERROR_Set( E_CLIB_OBLIVIOUS_2 ) ); 
    return  ret;
} /* End of function sprintf_s_ds_gcc */
#endif


/***********************************************************************
* Function Name: sprintf_s_dds_gcc
*    "sprintf" for gcc. Types are int, int, char*.
*
* Arguments:
*    in_String     - Address of string as streaming output
*    in_StringSize - Max character count + 1 of "in_String" argument
*    in_Format     - "printf" format
*    a1, a2, a3    - "printf" arguments
*
* Return Value:
*    Length of output. -1=Error
*
* Description:
*    This function is a proxy of "sprintf_s".
*    "vsprintf" with "%s" can not run in gcc.
************************************************************************/
#if  defined( __GNUC__ )
int_t  sprintf_s_dds_gcc( char_t* in_String,  size_t  in_StringSize,  const char_t* in_Format,
    int_t  a1,  int_t  a2,  char_t* a3 )
{
    int_t  ret;

    ret = sprintf( in_String, in_Format, a1, a2, a3 );

    ASSERT_R( ret <= in_StringSize,  R_ERROR_Set( E_CLIB_OBLIVIOUS_3 ) ); 
    return  ret;
} /* End of function sprintf_s_dds_gcc */
#endif


