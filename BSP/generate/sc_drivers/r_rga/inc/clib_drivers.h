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
* File Name   : clib_drivers.h
* Description : Common code for drivers and more.
* Version     : 1.12
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "clib_drivers_typedef.h"
#include  "rz_co_typedef.h"
#include  "r_typedefs.h"

#ifndef  CLIB_DRIVERS_H
#define  CLIB_DRIVERS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Typedef definitions
******************************************************************************/
/* in clib_drivers_typedef.h */

/******************************************************************************
Macro definitions
******************************************************************************/
/* in clib_drivers_typedef.h */

/******************************************************************************
Variable Externs
******************************************************************************/
/* in clib_drivers_typedef.h */

/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function Name: R_int32_t_to_int8_t
************************************************************************/
errnum_t  R_int32_t_to_int8_t( int32_t input, int8_t* output );


/***********************************************************************
* Function Name: R_int32_t_to_int16_t
************************************************************************/
errnum_t  R_int32_t_to_int16_t( int32_t input, int16_t* output );


/***********************************************************************
* Function Name: R_int32_t_to_uint8_t
************************************************************************/
errnum_t  R_int32_t_to_uint8_t( int32_t input, uint8_t* output );


/***********************************************************************
* Function Name: IntBoxType_isSame
************************************************************************/
errnum_t  IntBoxType_isSame( const IntBoxType* in_BoxA, const IntBoxType* in_BoxB, bool_t* out_IsSame );


/***********************************************************************
* Function Name: sprintf_s
************************************************************************/
int_t  sprintf_s( char_t* String, size_t StringSize, const char_t* Format, ... );


/***********************************************************************
* Function Name: sprintf_s_ds_gcc
************************************************************************/
#if  defined( __GNUC__ )
int_t  sprintf_s_ds_gcc( char* in_String,  size_t  in_StringSize,  const char* in_Format,
    int_t  a1,  char_t* a2 );
#endif


/***********************************************************************
* Function Name: sprintf_s_dds_gcc
************************************************************************/
#if  defined( __GNUC__ )
int_t  sprintf_s_dds_gcc( char* in_String,  size_t  in_StringSize,  const char* in_Format,
    int_t  a1,  int_t  a2,  char_t* a3 );
#endif


/***********************************************************************
* Function Name: R_int_t_to_int8_t
*    Cast with range check
*
* Arguments:
*    input  - Input value
*    output - Output value
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
STATIC_INLINE errnum_t  R_int_t_to_int8_t  
    ( int_t input, int8_t* output )
{
    return  R_int32_t_to_int8_t( input, output );
}/* End of function R_int_t_to_int8_t */


/***********************************************************************
* Function Name: R_Mod_N
*    Fast mod operation
*
* Arguments:
*    value - Left operand
*
* Return Value:
*    value % N
*
* Description:
* - This is shared description of
*   R_Mod_2s, R_Mod_4s, R_Mod_8s, R_Mod_16s, R_Mod_32s, R_Mod_64s,
*   R_Mod_2u, R_Mod_4u, R_Mod_8u, R_Mod_16u, R_Mod_32u, R_Mod_64u.
* - Porting layer of faster than % operator.
* - MISRA & IPA SEC confirmed version.
* - Minus value is expected 2's complement.
* - Not same C99 % operator.
*   Sample: R_Mod_4s( -7 ) == +1.  Sample: -7 % 4 == -3
************************************************************************/


/***********************************************************************
* Function Name: R_Mod_2s
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Mod_2s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 1;  

    /* SEC M1.10.1 */
    return  ((int_fast32_t)((uint_fast32_t)(value) & (uint_fast32_t)(mask)));
}/* End of function R_Mod_2s */


/***********************************************************************
* Function Name: R_Mod_4s
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Mod_4s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 3;  

    /* SEC M1.10.1 */
    return  ((int_fast32_t)((uint_fast32_t)(value) & (uint_fast32_t)(mask)));
}/* End of function R_Mod_4s */


/***********************************************************************
* Function Name: R_Mod_8s
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Mod_8s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 7;    

    /* SEC M1.10.1 */
    return  ((int_fast32_t)((uint_fast32_t)(value) & (uint_fast32_t)(mask)));
}/* End of function R_Mod_8s */


/***********************************************************************
* Function Name: R_Mod_16s
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Mod_16s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 15;  

    /* SEC M1.10.1 */
    return  ((int_fast32_t)((uint_fast32_t)(value) & (uint_fast32_t)(mask)));
}/* End of function R_Mod_16s */


/***********************************************************************
* Function Name: R_Mod_32s
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Mod_32s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 31;   

    /* SEC M1.10.1 */
    return  ((int_fast32_t)((uint_fast32_t)(value) & (uint_fast32_t)(mask)));
}/* End of function R_Mod_32s */


/***********************************************************************
* Function Name: R_Mod_64s
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Mod_64s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 63;  

    /* SEC M1.10.1 */
    return  ((int_fast32_t)((uint_fast32_t)(value) & (uint_fast32_t)(mask)));
}/* End of function R_Mod_64s */


/***********************************************************************
* Function Name: R_Mod_2u
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Mod_2u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 1u;  

    /* SEC M1.10.1 */
    return  ((value) & mask);
}/* End of function R_Mod_2u */


/***********************************************************************
* Function Name: R_Mod_4u
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Mod_4u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 3u;   

    /* SEC M1.10.1 */
    return  ((value) & mask);
}/* End of function R_Mod_4u */


/***********************************************************************
* Function Name: R_Mod_8u
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Mod_8u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 7u;  

    /* SEC M1.10.1 */
    return  ((value) & mask);
}/* End of function R_Mod_8u */


/***********************************************************************
* Function Name: R_Mod_16u
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Mod_16u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 15u;  

    /* SEC M1.10.1 */
    return  ((value) & mask);
}/* End of function R_Mod_16u */


/***********************************************************************
* Function Name: R_Mod_32u
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Mod_32u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 31u;  

    /* SEC M1.10.1 */
    return  ((value) & mask);
}/* End of function R_Mod_32u */


/***********************************************************************
* Function Name: R_Mod_64u
*    Fast mod operation. See <R_Mod_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Mod_64u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 63u;  

    /* SEC M1.10.1 */
    return  ((value) & mask);
}/* End of function R_Mod_64u */


/***********************************************************************
* Function Name: R_Ceil_N
*    Fast ceil operation
*
* Arguments:
*    value - Left operand
*
* Return Value:
*    Ceil( value / N )
*
* Description:
* - This is shared description of
*   R_Ceil_2s, R_Ceil_4s, R_Ceil_8s, R_Ceil_16s, R_Ceil_32s, R_Ceil_64s,
*   R_Ceil_2u, R_Ceil_4u, R_Ceil_8u, R_Ceil_16u, R_Ceil_32u, R_Ceil_64u.
* - Porting layer of fast ceil operation.
* - Function version is confirmed with MISRA & IPA SEC.
************************************************************************/


/***********************************************************************
* Function Name: R_Ceil_2s
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Ceil_2s( int_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 1;  

    /*[CheckedNext]Cast to larger type*/
    return  ( (int_fast32_t)(( (uint_fast32_t)( value ) + mask ) & (~ mask)) );
}/* End of function R_Ceil_2s */


/***********************************************************************
* Function Name: R_Ceil_4s
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Ceil_4s( int_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 3; 

    /*[CheckedNext]Cast to larger type*/
    return  ((int_fast32_t)(( (uint_fast32_t)( value ) + mask ) & (~ mask)) );
}/* End of function R_Ceil_4s */


/***********************************************************************
* Function Name: R_Ceil_8s
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Ceil_8s( int_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 7; 

    /*[CheckedNext]Cast to larger type*/
    return  ((int_fast32_t)(( (uint_fast32_t)( value ) + mask ) & (~ mask)) );
}/* End of function R_Ceil_8s */


/***********************************************************************
* Function Name: R_Ceil_16s
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Ceil_16s( int_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 15;  

    /*[CheckedNext]Cast to larger type*/
    return  ((int_fast32_t)(( (uint_fast32_t)( value ) + mask ) & (~ mask)) );
}/* End of function R_Ceil_16s */


/***********************************************************************
* Function Name: R_Ceil_32s
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Ceil_32s( int_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 31; 

    /*[CheckedNext]Cast to larger type*/
    return  ((int_fast32_t)(( (uint_fast32_t)( value ) + mask ) & (~ mask)) );
}/* End of function R_Ceil_32s */


/***********************************************************************
* Function Name: R_Ceil_64s
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Ceil_64s( int_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 63; 

    /*[CheckedNext]Cast to larger type*/
    return  ((int_fast32_t)(( (uint_fast32_t)( value ) + mask ) & (~ mask)) );
}/* End of function R_Ceil_64s */


/***********************************************************************
* Function Name: R_Ceil_2u
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Ceil_2u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 1;  
    return  (((value)+mask)&(~mask));
}/* End of function R_Ceil_2u */


/***********************************************************************
* Function Name: R_Ceil_4u
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Ceil_4u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 3;  
    return  (((value)+mask)&(~mask));
}/* End of function R_Ceil_4u */


/***********************************************************************
* Function Name: R_Ceil_8u
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Ceil_8u( uint_fast32_t const  value )  
{
    const uint_fast32_t
        mask = 7;  
    return  (((value)+mask)&(~mask));
}/* End of function R_Ceil_8u */


/***********************************************************************
* Function Name: R_Ceil_16u
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Ceil_16u( uint_fast32_t const  value ) 
{
    const uint_fast32_t
        mask = 15;  
    return  (((value)+mask)&(~mask));
}/* End of function R_Ceil_16u */


/***********************************************************************
* Function Name: R_Ceil_32u
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Ceil_32u( uint_fast32_t const  value ) 
{
    const uint_fast32_t
        mask = 31;  
    return  (((value)+mask)&(~mask));
}/* End of function R_Ceil_32u */


/***********************************************************************
* Function Name: R_Ceil_64u
*    Fast ceil operation. See <R_Ceil_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Ceil_64u( uint_fast32_t const  value ) 
{
    const uint_fast32_t
        mask = 63;  
    return  (((value)+mask)&(~mask));
}/* End of function R_Ceil_64u */


/***********************************************************************
* Function Name: R_Floor_N
*    Fast floor operation
*
* Arguments:
*    value - Left operand
*
* Return Value:
*    Floor( value / N )
*
* Description:
* - This is shared description of
*   R_Floor_2s, R_Floor_4s, R_Floor_8s, R_Floor_16s, R_Floor_32s, R_Floor_64s,
*   R_Floor_2u, R_Floor_4u, R_Floor_8u, R_Floor_16u, R_Floor_32u, R_Floor_64u.
* - Porting layer of fast floor operation.
* - Function version is confirmed with MISRA & IPA SEC.
************************************************************************/


/***********************************************************************
* Function Name: R_Floor_2s
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Floor_2s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 1;  
    return  ((value)&(~mask));  /* [R_Floor_2s] faster than (x)-(x%2) */
}/* End of function R_Floor_2s */


/***********************************************************************
* Function Name: R_Floor_4s
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Floor_4s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 3;  
    return  ((value)&(~mask));  /* [R_Floor_4s] faster than (x)-(x%4) */
}/* End of function R_Floor_4s */


/***********************************************************************
* Function Name: R_Floor_8s
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Floor_8s( int_fast32_t const  value )  
{
    const int_fast32_t
        mask = 7;  
    return  ((value)&(~mask));  /* [R_Floor_8s] faster than (x)-(x%8) */
}/* End of function R_Floor_8s */


/***********************************************************************
* Function Name: R_Floor_16s
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Floor_16s( int_fast32_t const  value ) 
{
    const int_fast32_t
        mask = 15;  
    return  ((value)&(~mask));  /* [R_Floor_16s] faster than (x)-(x%16) */
}/* End of function R_Floor_16s */


/***********************************************************************
* Function Name: R_Floor_32s
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Floor_32s( int_fast32_t const  value ) 
{
    const int_fast32_t
        mask = 31;  
    return  ((value)&(~mask));  /* [R_Floor_32s] faster than (x)-(x%32) */
}/* End of function R_Floor_32s */


/***********************************************************************
* Function Name: R_Floor_64s
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE int_fast32_t
    R_Floor_64s( int_fast32_t const  value ) 
{
    const int_fast32_t
        mask = 63;  
    return  ((value)&(~mask));  /* [R_Floor_64s] faster than (x)-(x%64) */
}/* End of function R_Floor_64s */


/***********************************************************************
* Function Name: R_Floor_2u
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Floor_2u( uint_fast32_t const  value ) 
{
    const uint_fast32_t
        mask = 1;  
    return  ((value)&(~mask));  /* [R_Floor_2u] faster than (x)-(x%2) */
}/* End of function R_Floor_2u */


/***********************************************************************
* Function Name: R_Floor_4u
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Floor_4u( uint_fast32_t const  value ) 
{
    const uint_fast32_t
        mask = 3;  
    return  ((value)&(~mask));  /* [R_Floor_4u] faster than (x)-(x%4) */
}/* End of function R_Floor_4u */


/***********************************************************************
* Function Name: R_Floor_8u
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Floor_8u( uint_fast32_t const  value ) 
{
    const uint_fast32_t
        mask = 7;  
    return  ((value)&(~mask));  /* [R_Floor_8u] faster than (x)-(x%8) */
}/* End of function R_Floor_8u */


/***********************************************************************
* Function Name: R_Floor_16u
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Floor_16u( uint_fast32_t const  value )
{
    const uint_fast32_t
        mask = 15;  
    return  ((value)&(~mask));  /* [R_Floor_16u] faster than (x)-(x%16) */
}/* End of function R_Floor_16u */


/***********************************************************************
* Function Name: R_Floor_32u
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Floor_32u( uint_fast32_t const  value )
{
    const uint_fast32_t
        mask = 31;  
    return  ((value)&(~mask));  /* [R_Floor_32u] faster than (x)-(x%32) */
}/* End of function R_Floor_32u */


/***********************************************************************
* Function Name: R_Floor_64u
*    Fast floor operation. See <R_Floor_N>.
************************************************************************/
STATIC_INLINE uint_fast32_t
    R_Floor_64u( uint_fast32_t const  value )
{
    const uint_fast32_t
        mask = 63;  
    return  ((value)&(~mask));  /* [R_Floor_64u] faster than (x)-(x%64) */
}/* End of function R_Floor_64u */


/***********************************************************************
* Function Name: IntBoxType_isEmpty
*    Whether specified box is empty or not.
*
* Arguments:
*    in_Box      - Checking box
*    out_IsEmpty - (output) Whether "in_Box" argment is empty
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
STATIC_INLINE errnum_t  IntBoxType_isEmpty  
    ( const IntBoxType*  in_Box,  bool_t*  out_IsEmpty )
{
    *out_IsEmpty = ( in_Box->Width == 0 );
    return  0;
}/* End of function IntBoxType_isEmpty */


/***********************************************************************
* Function Name: IntBoxType_setEmpty
*    Set empty to specified box.
*
* Arguments:
*    out_Box     - The box set to empty
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
STATIC_INLINE errnum_t  IntBoxType_setEmpty( IntBoxType* out_Box ) 
{
    out_Box->MinX   = 0;
    out_Box->MinY   = 0;
    out_Box->Width  = 0;
    out_Box->Height = 0;
    return  0;
}/* End of function IntBoxType_setEmpty */


/* Section: Global */
#ifdef __cplusplus
}  /* extern "C" */
#endif


/***********************************************************************
* Function Name: is_float_equal
*    Judgement whether float value is equal with some error (epsilon).
************************************************************************/
STATIC_INLINE bool_t  is_float_equal( float a, float b, float epsilon ) 
{
    float  sub = a - b; 

    /*       sub < epsilon   &&   sub >  -epsilon; */
    return  (sub < epsilon)  &&  (sub > (-epsilon));
}/* End of function is_float_equal */


/***********************************************************************
* Function Name: is_double_equal
*    Judgement whether double float value is equal with some error (epsilon).
************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

STATIC_INLINE bool_t  is_double_equal( double a, double b, double epsilon )
{
    double  sub = a - b; 

    /*       sub < epsilon   &&   sub >  -epsilon; */
    return  (sub < epsilon)  &&  (sub > (-epsilon));
}/* End of function is_double_equal */


/* Section: Global */
#ifdef __cplusplus
}  /* extern "C" */
#endif


/***********************************************************************
* Function Name: is_float_equal (double)
*    Judgement whether double float value is equal with some error (epsilon).
************************************************************************/
#ifdef __cplusplus
STATIC_INLINE bool_t  is_float_equal( double a, double b, double epsilon ) 
{
    double  sub = a - b;

    /*       sub < epsilon   &&   sub >  -epsilon; */
    return  (sub < epsilon)  &&  (sub > (-epsilon));
}/* End of function is_float_equal */
#endif

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif  /* CLIB_DRIVERS_H */
