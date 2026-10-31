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
* File Name   : rgas_common.c
* Version     : 1.12
******************************************************************************/

#include  <string.h>
#include  "clib_drivers.h"
#include  "rz_co.h"
#include  "rga_inside.h"
#include  "rga_api_typedef_1.h"


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
* Function Name: R_RGA_Calculate0to1Bezier
************************************************************************/
float_t 
    R_RGA_Calculate0to1Bezier( float_t  in_T_Index, 
    float_t  in_ControlPoint1_Value,  float_t  in_ControlPoint2_Value )
{
    float_t  t  = in_T_Index;
    float_t  tt = 1.0f - in_T_Index;

    float_t  t2  = t  * t;
    float_t  t3  = t2 * t;
    float_t  tt2 = tt * tt;

    float_t  return_value =
        (3 * ( tt2 * t * in_ControlPoint1_Value )) +
        (3 * ( tt * t2 * in_ControlPoint2_Value )) +
        t3;

    /*
    * return_value =
    *    ( tt * tt * tt * 0.0f ) +
    *    3 * ( tt * tt * t * in_ControlPoint1_Value ) +
    *    3 * ( tt * t  * t * in_ControlPoint2_Value ) +
    *    ( t * t * t * 1.0f )
    */

    return  return_value;
}/* End of function R_RGA_Calculate0to1Bezier */

/***********************************************************************
* Function Name: R_RGA_CalculateDerivativeOf0to1Bezier
*    Calculate f' (f prime).
************************************************************************/
float_t 
    R_RGA_CalculateDerivativeOf0to1Bezier  
    ( float_t  in_ClampT, float_t  in_ControlPoint1_Value,  float_t  in_ControlPoint2_Value )
{
    float_t  t  = in_ClampT;
    float_t  t2 = t * t;

    float_t  return_value =
        (t2 * (  (((9 * in_ControlPoint1_Value) - 9) * in_ControlPoint2_Value) + 3 )) +
        (t * ( ((-12) * in_ControlPoint1_Value) + (6 * in_ControlPoint2_Value) )) +
        ( 3 * in_ControlPoint1_Value );

    return  return_value;

    /*
    * return_value = derivative of
    *    ( tt * tt * tt * 0.0f ) +
    *    3 * ( tt * tt * t * in_ControlPoint1_Value ) +
    *    3 * ( tt * t  * t * in_ControlPoint2_Value ) +
    *    ( t * t * t * 1.0f );
    *
    * cp1 = in_ControlPoint1_Value;
    * cp2 = in_ControlPoint2_Value;
    * tt * tt * t  =  (1-t) * (1-t) * t  =  ( 1 - 2*t + t2 ) * t  =  t - 2*t2 + t3;
    * tt * t * t  =  (1-t) * t * t  =  t2 - t3;
    *
    * return_value = derivative of
    *    3 * cp1 * t  -  2 * 3 * cp1 * t2  +  3 * cp1 * t3 +
    *    3 * cp2 * t2  -  3 * cp2 * t3 +
    *    t3;
    *
    * return_value = derivative of
    *    t3 * ( 3 * cp1 - 3 * cp2 + 1 ) +
    *    t2 * ( -2 * 3 * cp1 + 3 * cp2 ) +
    *    t  * ( 3 * cp1 );
    *
    * return_value =
    *    3 * t2 * ( 3 * cp1 - 3 * cp2 + 1 ) +
    *    2 * t * ( -6 * cp1 + 3 * cp2 ) +
    *    ( 3 * cp1 );
    *
    * return_value =
    *    t2 * ( 9 * cp1 - 9 * cp2 + 3 ) +
    *    t * ( -12 * cp1 + 6 * cp2 ) +
    *    ( 3 * cp1 );
    */
}/* End of function R_RGA_CalculateDerivativeOf0to1Bezier */



/***********************************************************************
* Function Name: R_RGA_SearchClampT_InIncreasing0to1Bezier
************************************************************************/
float_t
R_RGA_SearchClampT_InIncreasing0to1Bezier 
    ( float_t  in_TargetValue,  float_t  in_GuessedClampT,
    float_t  in_ControlPoint1_Value,  float_t  in_ControlPoint2_Value,  float_t  in_Epsilon,
    float_t  in_MinimumClampT,  float_t  in_MaximumClampT )
{
    const float_t  epsilon_newton = 0.5f; 

    enum {        max_loop = 20 }; 
    float_t       t = in_GuessedClampT;
    float_t       t_next;
    float_t       t_candidate = in_GuessedClampT;
    float_t       f_t;        /* f(t) = bezier(t) - v */
    float_t       f_prime_t;  /* f'(t) */
    float_t       f_t_min;
    float_t       cp1 = in_ControlPoint1_Value;
    float_t       cp2 = in_ControlPoint2_Value;
    float_t       min_t = in_MinimumClampT;
    float_t       max_t = in_MaximumClampT;
    int_fast32_t  i;


    ASSERT_D( in_Epsilon >= 0.0f,  R_NOOP() );


    /* Set "f_t_min" */
    if ( in_TargetValue > in_GuessedClampT )
    {
        f_t_min = in_TargetValue - in_GuessedClampT;
    }
    else
    {
        f_t_min = in_GuessedClampT - in_TargetValue;
    }


    /* ... */
    for ( i = 0;  i < max_loop;  i += 1 )
    {

        f_t = R_RGA_Calculate0to1Bezier( t, cp1, cp2 ) - in_TargetValue;
        if ( f_t >= 0.0f )
        {
            if ( f_t < f_t_min )
            {
                f_t_min = f_t;
                t_candidate = t;
            }
            if ( t < max_t )
            {
                max_t = t;
            }

            if ( f_t <= in_Epsilon )
            {
                break;
            }
        }
        else
        {
            float_t  minus_f_t = -f_t;

            if ( minus_f_t < f_t_min )
            {
                f_t_min = minus_f_t;
                t_candidate = t;
            }
            if ( t > min_t )
            {
                min_t = t;
            }

            if ( minus_f_t <= in_Epsilon )
            {
                break;
            }
        }


        f_prime_t = R_RGA_CalculateDerivativeOf0to1Bezier( t, cp1, cp2 );
        if ( f_prime_t != 0.0f )
        {


            t_next = t - ( f_t / f_prime_t );


        }
        else
        {
            /* Bad case of Newton Raphson Method */
            if ( f_t < 0.0f )
            {
                t_next = 1.0f + epsilon_newton + 1.0f;  /* For next if */
            }
            else
            {
                t_next = (0.0f - epsilon_newton) - 1.0f;  /* For next if */
            }
        }


        if ( t_next > (1.0f + epsilon_newton) )
        {
            min_t = t;
            t = ( min_t + max_t ) / 2.0f;  /* Binary Search */
        }
        else if ( t_next < (0.0f - epsilon_newton) )
        {
            max_t = t;
            t = ( min_t + max_t ) / 2.0f;  /* Binary Search */
        }
        else
        {
            t = t_next;  /* Newton Raphson Method */
        }
    }

    return  t_candidate;
}/* End of function R_RGA_SearchClampT_InIncreasing0to1Bezier */

/***********************************************************************
* Function Name: R_RGA_GetY_From0to1Bezier_IncreasingX
************************************************************************/
errnum_t 
    R_RGA_GetY_From0to1Bezier_IncreasingX
    ( float_t  in_X,  float_t*  out_Y,
    float_t*  in_out_T_Index,
    float_t  in_ControlPoint1_X,  float_t  in_ControlPoint1_Y,
    float_t  in_ControlPoint2_X,  float_t  in_ControlPoint2_Y,
    float_t  in_EpsilonX )
{
    errnum_t  e;
    float_t   t = *in_out_T_Index;
    float_t   y;


    if ( in_ControlPoint1_X < 0.0f )
    {
        e= E_RGA_OTHER_1D; goto fin;  /* Error: Too small "in_ControlPoint1_X" argument. */
    }

    if ( in_ControlPoint2_X > 1.0f )
    {
        e= E_RGA_OTHER_1E; goto fin;  /* Error: Too small "in_ControlPoint2_X" argument. */
    }

    t = R_RGA_SearchClampT_InIncreasing0to1Bezier( in_X,  t,
        in_ControlPoint1_X,  in_ControlPoint2_X,  in_EpsilonX,
        0.0f,  1.0f );

    y = R_RGA_Calculate0to1Bezier( t,
        in_ControlPoint1_Y,  in_ControlPoint2_Y );

    *in_out_T_Index = t;
    *out_Y = y;

    e=0;
fin:
    {
        return e;
    }
}/* End of function R_RGA_GetY_From0to1Bezier_IncreasingX */



/*-------------------------------------------------------------------------*/
/* <<<< ### (animation_timing_function_t) implement >>>> */ 
/*-------------------------------------------------------------------------*/

/***********************************************************************
* Function Name: gs_CSS_animation_timing_function_* 
************************************************************************/

#if  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_LINEAR
#define  TIMING_FUNCTION_DIV  (30)  /* 10 or 30 */
#if TIMING_FUNCTION_DIV == 10
static float_t  gs_CSS_animation_timing_function_ease_values[] = {
    0.0f, 0.1087962962962963f, 0.3037037037037037f, 0.4898962962962963f, 0.6768f, 0.8029f,
    0.8745962962962963f, 0.9328703703703705f, 0.9747f, 0.9970629629629629f, 1.0f,
};
#else
static float_t  gs_CSS_animation_timing_function_ease_values[] = {
    0.0f, 0.017924828532235943f, 0.052300000000000006f, 0.09834183813443073f,
    0.15430137174211248f, 0.21842962962962964f, 0.28897764060356657f,
    0.36419643347050756f, 0.4423370370370371f, 0.505777914951989f, 0.569073936899863f,
    0.6313296296296296f, 0.6768f, 0.7208037037037036f, 0.7629629629629628f, 0.8029f,
    0.8281035665294925f, 0.8634973936899862f, 0.8853223593964334f, 0.9056f,
    0.9242183813443072f, 0.9410655692729767f, 0.9560296296296296f, 0.9689986282578875f,
    0.9747f, 0.9844665294924554f, 0.9885037037037037f, 0.9948159122085047f,
    0.9970629629629629f, 0.999668998628258f, 1.0f,
};
#endif
static animation_timing_function_t  gs_CSS_animation_timing_function_ease = {
    gs_CSS_animation_timing_function_ease_values,
    R_COUNT_OF( gs_CSS_animation_timing_function_ease_values ),
};
#elif  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_BEZIER
static animation_timing_function_t  gs_CSS_animation_timing_function_ease = {
    0.25f, 0.1f, 0.25f, 1.0f, 1e-5
};
#endif

#if  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_LINEAR
static float_t  gs_CSS_animation_timing_function_linear_values[] = {
    0.0f,  1.0f,
};
static animation_timing_function_t  gs_CSS_animation_timing_function_linear = {
    gs_CSS_animation_timing_function_linear_values,
    R_COUNT_OF( gs_CSS_animation_timing_function_linear_values ),
};
#elif  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_BEZIER
static animation_timing_function_t  gs_CSS_animation_timing_function_linear = {
    1.0f/3, 1.0f/3, 2.0f/3, 2.0f/3, 1e-5
};
#endif

#if  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_LINEAR
#if TIMING_FUNCTION_DIV == 10
static float_t  gs_CSS_animation_timing_function_ease_in_values[] = {
    0.0f, 0.01274074074074074f, 0.07407407407407407f, 0.13792592592592592f, 0.216f,
    0.30474074074074065f, 0.45007407407407407f, 0.5499259259259259f, 0.6952592592592592f,
    0.8245925925925925f, 1.0f,
};
#else
static float_t  gs_CSS_animation_timing_function_ease_in_values[] = {
    0.0f, 0.0014595336076817559f, 0.008916323731138544f, 0.017207133058984914f,
    0.028000000000000004f, 0.04116323731138546f, 0.0650644718792867f,
    0.08357750342935528f, 0.10400000000000001f, 0.1262002743484225f, 0.15004663923182443f,
    0.1886145404663923f, 0.216f, 0.24457064471879292f, 0.2741947873799726f,
    0.320318244170096f, 0.35200000000000004f, 0.3842743484224966f, 0.4335089163237311f,
    0.4666886145404664f, 0.5166639231824416f, 0.5499259259259259f, 0.5994074074074074f,
    0.648f, 0.6952592592592592f, 0.7407407407407407f, 0.7839999999999999f,
    0.837454046639232f, 0.896f, 0.9434348422496571f, 1.0f,
};
#endif
static animation_timing_function_t  gs_CSS_animation_timing_function_ease_in = {
    gs_CSS_animation_timing_function_ease_in_values,
    R_COUNT_OF( gs_CSS_animation_timing_function_ease_in_values ),
};
#elif  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_BEZIER
static animation_timing_function_t  gs_CSS_animation_timing_function_ease_in = {
    0.42f, 0.0f, 1.0f, 1.0f, 1e-5
};
#endif

#if  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_LINEAR
#if TIMING_FUNCTION_DIV == 10
static float_t  gs_CSS_animation_timing_function_ease_out_values[] = {
    0.0f, 0.1754074074074074f, 0.30474074074074065f, 0.45007407407407407f,
    0.5499259259259259f, 0.6952592592592592f, 0.7839999999999999f, 0.8620740740740741f,
    0.9259259259259259f, 0.9872592592592593f, 1.0f,
};
#else
static float_t  gs_CSS_animation_timing_function_ease_out_values[] = {
    0.0f, 0.05656515775034293f, 0.10400000000000001f, 0.16254595336076816f, 0.216f,
    0.25925925925925924f, 0.30474074074074065f, 0.35200000000000004f, 0.4005925925925926f,
    0.45007407407407407f, 0.4833360768175582f, 0.5333113854595336f, 0.5664910836762689f,
    0.6157256515775034f, 0.648f, 0.6796817558299041f, 0.7258052126200274f,
    0.7554293552812071f, 0.7839999999999999f, 0.8113854595336077f, 0.8499533607681755f,
    0.8737997256515775f, 0.896f, 0.9164224965706447f, 0.9349355281207133f,
    0.9588367626886146f, 0.972f, 0.9827928669410152f, 0.9910836762688615f,
    0.9985404663923182f, 1.0f,
};
#endif
static animation_timing_function_t  gs_CSS_animation_timing_function_ease_out = {
    gs_CSS_animation_timing_function_ease_out_values,
    R_COUNT_OF( gs_CSS_animation_timing_function_ease_out_values ),
};
#elif  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_BEZIER
static animation_timing_function_t  gs_CSS_animation_timing_function_ease_out = {
    0.0f, 0.0f, 0.58f, 1.0f, 1e-5
};
#endif

#if  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_LINEAR
#if TIMING_FUNCTION_DIV == 10
static float_t  gs_CSS_animation_timing_function_ease_in_out_values[] = {
    0.0f, 0.028000000000000004f, 0.07407407407407407f, 0.1754074074074074f,
    0.35200000000000004f, 0.5f, 0.648f, 0.8245925925925925f, 0.9259259259259259f, 0.972f, 1.0f,
};
#else
static float_t  gs_CSS_animation_timing_function_ease_in_out_values[] = {
    0.0f, 0.0014595336076817559f, 0.008916323731138544f, 0.022299039780521262f,
    0.034293552812071325f, 0.05656515775034293f, 0.08357750342935528f,
    0.11488614540466391f, 0.15004663923182443f, 0.1886145404663923f, 0.2301454046639232f,
    0.2741947873799726f, 0.33607681755829905f, 0.3842743484224966f, 0.45007407407407407f,
    0.5f, 0.5499259259259259f, 0.6157256515775034f, 0.663923182441701f, 0.7258052126200274f,
    0.7698545953360768f, 0.8113854595336077f, 0.8499533607681755f, 0.8851138545953361f,
    0.9164224965706447f, 0.9434348422496571f, 0.9657064471879285f, 0.9777009602194787f,
    0.9910836762688615f, 0.9985404663923182f, 1.0f,
};
#endif
static animation_timing_function_t  gs_CSS_animation_timing_function_ease_in_out = {
    gs_CSS_animation_timing_function_ease_in_out_values,
    R_COUNT_OF( gs_CSS_animation_timing_function_ease_in_out_values ),
};
#elif  GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_BEZIER
static animation_timing_function_t  gs_CSS_animation_timing_function_ease_in_out = {
    0.42f, 0.0f, 0.58f, 1.0f, 1e-5
};
#endif

/***********************************************************************
* Function Name: R_Get_AnimationTimingFunction 
************************************************************************/
errnum_t  R_Get_AnimationTimingFunction( const char_t* FunctionName, 
    animation_timing_function_t** out_Function )
{
    errnum_t  e;

    if ( strcmp( FunctionName, "ease" ) == 0 )
    {
        *out_Function = &gs_CSS_animation_timing_function_ease;
    }
    else if ( strcmp( FunctionName, "linear" ) == 0 )
    {
        *out_Function = &gs_CSS_animation_timing_function_linear;
    }
    else if ( strcmp( FunctionName, "ease-in" ) == 0 )
    {
        *out_Function = &gs_CSS_animation_timing_function_ease_in;
    }
    else if ( strcmp( FunctionName, "ease-out" ) == 0 )
    {
        *out_Function = &gs_CSS_animation_timing_function_ease_out;
    }
    else if ( strcmp( FunctionName, "ease-in-out" ) == 0 )
    {
        *out_Function = &gs_CSS_animation_timing_function_ease_in_out;
    }
    else
    {
        e=E_RGA_OTHER_1F;  goto fin;  /* Error: Error of "FunctionName" argument */
    }

    e=0;
fin:
    {
        return e;
    }
}/* End of function R_Get_AnimationTimingFunction */

/***********************************************************************
* Function Name: R_ANIMATION_TIMING_FUNCTION_GetValue 
************************************************************************/
float_t  R_ANIMATION_TIMING_FUNCTION_GetValue
    ( animation_timing_function_t*  self,
    float_t  in_ClampTime,  float_t  in_ValueOfPreviousKeyFrame,  float_t  in_ValueOfNextKeyFrame )
{
    if ( in_ClampTime <= 0.0f )
    {
        return  in_ValueOfPreviousKeyFrame;
    }
    else if ( in_ClampTime >= 1.0f )
    {
        return  in_ValueOfNextKeyFrame;
    }
    else
    {
        float_t  clamp_value;

        #if GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_LINEAR
            float  time_index_float = ( in_ClampTime * (self->ClampValuesLength - 1) );
            int    time_index;
            float  sub_clamp_time;

            time_index = (int)( time_index_float );
            if ( time_index >= (int) self->ClampValuesLength )
                { time_index = self->ClampValuesLength - 1; }

            sub_clamp_time = time_index_float - time_index;

            clamp_value = self->ClampValues[ time_index ] * ( 1.0f - sub_clamp_time ) +
                            self->ClampValues[ time_index + 1 ] * ( sub_clamp_time );

        #elif GS_ANIMATION_CURVE == GS_ANIMATION_CURVE_IS_BEZIER
            errnum_t          e;
            static float_t  gs_t = 0.0f;

            e= R_RGA_GetY_From0to1Bezier_IncreasingX( in_ClampTime,  &clamp_value,  &gs_t,
                self->ControlPoint1_X,  self->ControlPoint1_Y,
                self->ControlPoint2_X,  self->ControlPoint2_Y,  self->EpsilonX );
            if ( e != 0 )
            {
                clamp_value = 0.0f;
                R_ERROR_Set( E_RGA_STATE_CLEAR_1 );
            }
        #else
            #error
        #endif

        return  (in_ValueOfPreviousKeyFrame * ( 1.0f - clamp_value )) +
            (in_ValueOfNextKeyFrame * ( clamp_value ));
    }
}/* End of function R_ANIMATION_TIMING_FUNCTION_GetValue */

