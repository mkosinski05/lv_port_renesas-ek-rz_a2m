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
* File Name: dave_log.c
*    Macro redirect of D/AVE 2D API.
* - Description: Target API is defined in "dave_driver.h".
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_drw.h"
#include  <stdio.h>
#include  "dave_driver.h"
#include  "dave_log.h"
#include  "dave_base_rz_errnum.h"
#include  "r_drw.h"
#include  "r_memory_map.h"
#include  "rz_co.h"
#ifdef  USE_CONFIG_REDIRECT_DRW_API
#include  "r_compiler_abstraction_api.h"
#endif

#ifdef  USE_CONFIG_REDIRECT_DRW_API


/******************************************************************************
Macro definitions
******************************************************************************/
#define  GS_N  "\r\n"

/***********************************************************************
* Macros: Undefine a part of macro redirect
************************************************************************/
#undef  d2_getversion
#undef  d2_getversionstring
#undef  d2_opendevice
#undef  d2_closedevice
#undef  d2_geterror
#undef  d2_geterrorstring
#undef  d2_translateerror
#undef  d2_inithw
#undef  d2_deinithw
#undef  d2_level1interface
#undef  d2_getrevisionhw
#undef  d2_getrevisionstringhw
#undef  d2_setdlistblocksize
#undef  d2_getdlistblocksize
#undef  d2_getdlistblockcount
#undef  d2_commandspending
#undef  d2_lowlocalmemmode
#undef  d2_newcontext
#undef  d2_freecontext
#undef  d2_selectcontext
#undef  d2_solidcontext
#undef  d2_outlinecontext
#undef  d2_getcontext
#undef  d2_framebuffer
#undef  d2_cliprect
#undef  d2_flushframe
#undef  d2_startframe
#undef  d2_endframe
#undef  d2_relocateframe
#undef  d2_gethaspreviousframe
#undef  d2_getisemptydlistentry
#undef  d2_clear
#undef  d2_getcliprect
#undef  d2_getframebuffer
#undef  d2_selectrendermode
#undef  d2_outlinewidth
#undef  d2_shadowoffset
#undef  d2_getrendermode
#undef  d2_layermerge
#undef  d2_newrenderbuffer
#undef  d2_freerenderbuffer
#undef  d2_selectrenderbuffer
#undef  d2_executerenderbuffer
#undef  d2_getrenderbuffer
#undef  d2_dumprenderbuffer
#undef  d2_getrenderbuffersize
#undef  d2_freedumpedbuffer
#undef  d2_setcolor
#undef  d2_setalpha
#undef  d2_setalphaex
#undef  d2_setblur
#undef  d2_setblendmode
#undef  d2_setalphablendmode
#undef  d2_setalphablendmodeex
#undef  d2_setalphagradient
#undef  d2_setclipgradient
#undef  d2_setalphamode
#undef  d2_setantialiasing
#undef  d2_setpatternalpha
#undef  d2_setfillmode
#undef  d2_setpattern
#undef  d2_setpatternparam
#undef  d2_setpatternmode
#undef  d2_setpatternsize
#undef  d2_setlinecap
#undef  d2_setlinejoin
#undef  d2_setlinepattern
#undef  d2_setmiterlimit
#undef  d2_settexture
#undef  d2_settexturemode
#undef  d2_settextureoperation
#undef  d2_settexopparam
#undef  d2_settexturemapping
#undef  d2_settexelcenter
#undef  d2_settexclut
#undef  d2_settexclut_part
#undef  d2_writetexclut_direct
#undef  d2_settexclut_offset
#undef  d2_settexclut_format
#undef  d2_setcolorkey
#undef  d2_setcircleextend
#undef  d2_getcolor
#undef  d2_getalpha
#undef  d2_getalphaex
#undef  d2_getblur
#undef  d2_getblendmodesrc
#undef  d2_getblendmodedst
#undef  d2_getalphablendmodesrc
#undef  d2_getalphablendmodedst
#undef  d2_getalphablendmodeflags
#undef  d2_getalphamode
#undef  d2_getantialiasing
#undef  d2_getpatternalpha
#undef  d2_getfillmode
#undef  d2_getpattern
#undef  d2_getpatternmode
#undef  d2_getpatternsize
#undef  d2_getlinecap
#undef  d2_getlinejoin
#undef  d2_gettextureoperationa
#undef  d2_gettextureoperationr
#undef  d2_gettextureoperationg
#undef  d2_gettextureoperationb
#undef  d2_gettexopparam1
#undef  d2_gettexopparam2
#undef  d2_renderbox
#undef  d2_renderline
#undef  d2_rendertri
#undef  d2_renderquad
#undef  d2_rendercircle
#undef  d2_renderwedge
#undef  d2_renderline2
#undef  d2_renderpolyline
#undef  d2_renderpolyline2
#undef  d2_rendertrilist
#undef  d2_rendertrifan
#undef  d2_rendertristrip
#undef  d2_renderpolygon
#undef  d2_setblitsrc
#undef  d2_blitcopy
#undef  d2_setperfcountevent
#undef  d2_setperfcountvalue
#undef  d2_getperfcountvalue
#undef  d2_utility_maptriangle
#undef  d2_utility_perspectivewarp
#undef  d2_utility_fbblitcopy
#undef  d2_rendercircle_no_hilimiterprecision
#undef  d2_executedlist
#undef  d2_adddlist


/***********************************************************************
* Define: USE_DRW_API_PRINT_LOG
*    0 or 1
*
* Description:
*    Logging code is cut by "USE_CONFIG_REDIRECT_DRW_API" not cut by this define.
************************************************************************/
#define  USE_DRW_API_PRINT_LOG  (1)


/******************************************************************************
Typedef definitions
******************************************************************************/

typedef struct {
    const void*    Address;
    const char_t*  String;
} gs_address_and_string_t;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
void  gs_PrintVariable( const void*  in_Address );
char_t*  gs_GetStringOf_d2_textureoperations( d2_u8  in_Value );
char_t*  gs_GetStringOf_d2_blendmodes( d2_u32  in_Value );
static const gs_address_and_string_t*  gs_Variables;
static int_fast32_t  gs_VariableAddressesCount;


/***********************************************************************
* Function: gs_IsLogEnabled
************************************************************************/
static bool_t  gs_IsLogEnabled()
{
    return  true;
}


/***********************************************************************
* Function: d2_getversion_to_Log
************************************************************************/
D2_EXTERN d2_s32         d2_getversion_to_Log( void )
{
    d2_s32  return_value = d2_getversion_from_Log();

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getversion */ {"GS_N );
        printf( "DRW_API:        d2_s32  return_value = d2_getversion();"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getversionstring_to_Log
************************************************************************/
D2_EXTERN const d2_char *d2_getversionstring_to_Log( void )
{
    const d2_char*  return_value = d2_getversionstring_from_Log();

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getversionstring */ {"GS_N );
        printf( "DRW_API:        d2_char*  return_value = d2_getversionstring();"GS_N );
        printf( "DRW_API:            /* return_value = %s */"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_opendevice_to_Log
************************************************************************/
D2_EXTERN d2_device*  d2_opendevice_to_Log( d2_u32 flags )
{
    d2_device*  return_value = d2_opendevice_from_Log( flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_opendevice */ {"GS_N );
        printf( "DRW_API:        d2_u32  flags = 0x%08X;"GS_N,  flags );
        printf( "DRW_API:        d2_device*  return_value = d2_opendevice( flags );"GS_N );
        printf( "DRW_API:            /* return_value = 0x%08X */"GS_N,  return_value );
        printf( "DRW_API:            handle = (d2_device*) return_value;"GS_N );
        printf( "DRW_API:            if ( return_value == NULL ) { e=__LINE__; goto fin; }"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_closedevice_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_closedevice_to_Log( d2_device *handle )
{
    d2_s32  return_value = d2_closedevice_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_closedevice */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32  return_value = d2_closedevice( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_geterror_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_geterror_to_Log( const d2_device *handle )
{
    d2_s32  return_value = d2_geterror_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_geterror_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_geterrorstring_to_Log
************************************************************************/
D2_EXTERN const d2_char *d2_geterrorstring_to_Log( const d2_device *handle )
{
    const d2_char * return_value = d2_geterrorstring_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_geterrorstring_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_translateerror_to_Log
************************************************************************/
D2_EXTERN const d2_char *d2_translateerror_to_Log( d2_s32 errorcode )
{
    const d2_char * return_value = d2_translateerror_from_Log( errorcode );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_translateerror_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_inithw_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_inithw_to_Log( d2_device *handle, d2_u32 flags )
{
    d2_s32  return_value = d2_inithw_from_Log( handle,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_inithw */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 flags = 0x%08X;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_inithw( handle,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_deinithw_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_deinithw_to_Log( d2_device *handle )
{
    d2_s32  return_value = d2_deinithw_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_deinithw_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_level1interface_to_Log
************************************************************************/
D2_EXTERN d1_device *    d2_level1interface_to_Log( const d2_device *handle )
{
    d1_device *  return_value = d2_level1interface_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_level1interface_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getrevisionhw_to_Log
************************************************************************/
D2_EXTERN d2_u32  d2_getrevisionhw_to_Log( const d2_device *handle )
{
    d2_u32  return_value = d2_getrevisionhw_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getrevisionhw_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getrevisionstringhw_to_Log
************************************************************************/
D2_EXTERN const d2_char* d2_getrevisionstringhw_to_Log( const d2_device *handle)
{
    const d2_char*  return_value = d2_getrevisionstringhw_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getrevisionstringhw */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_char*  return_value = d2_getrevisionstringhw( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setdlistblocksize_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_setdlistblocksize_to_Log(d2_device *handle, d2_u32 size)
{
    d2_s32  return_value = d2_setdlistblocksize_from_Log( handle,  size );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setdlistblocksize */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32    size = 0x%08X;"GS_N,  size );
        printf( "DRW_API:        d2_s32  return_value = d2_setdlistblocksize( handle,  size );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getdlistblocksize_to_Log
************************************************************************/
D2_EXTERN d2_u32  d2_getdlistblocksize_to_Log(const d2_device *handle)
{
    d2_u32  return_value = d2_getdlistblocksize_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getdlistblocksize */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32  return_value = d2_getdlistblocksize( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getdlistblockcount_to_Log
************************************************************************/
D2_EXTERN d2_u32  d2_getdlistblockcount_to_Log(d2_device *handle)
{
    d2_u32  return_value = d2_getdlistblockcount_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getdlistblockcount */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32  return_value = d2_getdlistblockcount( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_commandspending_to_Log
************************************************************************/
D2_EXTERN d2_s32         d2_commandspending_to_Log(d2_device *handle)
{
    d2_s32  return_value = d2_commandspending_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_commandspending_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_lowlocalmemmode_to_Log
************************************************************************/
D2_EXTERN d2_s32         d2_lowlocalmemmode_to_Log(d2_device *handle, d2_u32 dlistblockfactor, d2_u32 dlistblocks)
{
    d2_s32  return_value = d2_lowlocalmemmode_from_Log( handle,  dlistblockfactor,  dlistblocks );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_lowlocalmemmode_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_newcontext_to_Log
************************************************************************/
D2_EXTERN d2_context * d2_newcontext_to_Log( d2_device *handle )
{
    d2_context *  return_value = d2_newcontext_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_newcontext_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_freecontext_to_Log
************************************************************************/
D2_EXTERN d2_s32       d2_freecontext_to_Log( d2_device *handle, d2_context *ctx )
{
    d2_s32  return_value = d2_freecontext_from_Log( handle,  ctx );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_freecontext_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_selectcontext_to_Log
************************************************************************/
D2_EXTERN d2_s32       d2_selectcontext_to_Log( d2_device *handle, d2_context *ctx )
{
    d2_s32  return_value = d2_selectcontext_from_Log( handle,  ctx );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_selectcontext_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_solidcontext_to_Log
************************************************************************/
D2_EXTERN d2_s32       d2_solidcontext_to_Log( d2_device *handle, d2_context *ctx )
{
    d2_s32  return_value = d2_solidcontext_from_Log( handle,  ctx );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_solidcontext_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_outlinecontext_to_Log
************************************************************************/
D2_EXTERN d2_s32       d2_outlinecontext_to_Log( d2_device *handle, d2_context *ctx )
{
    d2_s32  return_value = d2_outlinecontext_from_Log( handle,  ctx );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_outlinecontext_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getcontext_to_Log
************************************************************************/
D2_EXTERN d2_context * d2_getcontext_to_Log( d2_device *handle, d2_s32 mode )
{
    d2_context *  return_value = d2_getcontext_from_Log( handle,  mode );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getcontext_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_framebuffer_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_framebuffer_to_Log
        ( d2_device *handle, void *ptr, d2_s32 pitch, d2_u32 width, d2_u32 height, d2_s32 format )
{
    d2_s32  return_value = d2_framebuffer_from_Log( handle,  ptr,  pitch,  width,  height,  format );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_framebuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        void *ptr = (void*) 0x%08X;"GS_N,  ptr );
        printf( "DRW_API:        d2_s32 pitch = %d;"GS_N,  pitch );
        printf( "DRW_API:        d2_u32 width = %d;"GS_N,  width );
        printf( "DRW_API:        d2_u32 height = %d;"GS_N,  height );
        printf( "DRW_API:        d2_s32 format = %d;"GS_N,  format );
        printf( "DRW_API:        d2_s32  return_value = d2_framebuffer( handle,  ptr,  pitch,  width,  height,  format );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:        frame_address = ptr;"GS_N );
        printf( "DRW_API:        frame_pitch  = pitch;"GS_N );
        printf( "DRW_API:        frame_width  = width;"GS_N );
        printf( "DRW_API:        frame_height = height;"GS_N );
        printf( "DRW_API:        frame_format = format;"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_cliprect_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_cliprect_to_Log( d2_device *handle, d2_border xmin, d2_border ymin, d2_border xmax, d2_border ymax )
{
    d2_s32  return_value = d2_cliprect_from_Log( handle,  xmin,  ymin,  xmax,  ymax );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_cliprect */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_border xmin = %d;"GS_N,  xmin );
        printf( "DRW_API:        d2_border ymin = %d;"GS_N,  ymin );
        printf( "DRW_API:        d2_border xmax = %d;"GS_N,  xmax );
        printf( "DRW_API:        d2_border ymax = %d;"GS_N,  ymax );
        printf( "DRW_API:        d2_s32  return_value = d2_cliprect( handle,  xmin,  ymin,  xmax,  ymax );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_flushframe_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_flushframe_to_Log( d2_device *handle )
{
    d2_s32  return_value = d2_flushframe_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_flushframe */ {"GS_N );
        printf( "DRW_API:        d2_s32  return_value = d2_flushframe( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:        ON_FLUSH_FRAME"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_startframe_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_startframe_to_Log( d2_device *handle )
{
    d2_s32  return_value = d2_startframe_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_startframe */ {"GS_N );
        printf( "DRW_API:        d2_device*  handle = 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32  return_value = d2_startframe( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_endframe_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_endframe_to_Log( d2_device *handle )
{
    d2_s32  return_value = d2_endframe_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_endframe */ {"GS_N );
        printf( "DRW_API:        d2_device*  handle = 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32  return_value = d2_endframe( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_relocateframe_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_relocateframe_to_Log( d2_device *handle, const void *ptr )
{
    d2_s32  return_value = d2_relocateframe_from_Log( handle,  ptr );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_relocateframe_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_gethaspreviousframe_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_gethaspreviousframe_to_Log( d2_device *handle )
{
    d2_s32  return_value = d2_gethaspreviousframe_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_gethaspreviousframe_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getisemptydlistentry_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_getisemptydlistentry_to_Log( d2_device *handle )
{
    d2_s32  return_value = d2_getisemptydlistentry_from_Log( handle );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getisemptydlistentry */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32  return_value = d2_getisemptydlistentry( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_clear_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_clear_to_Log( d2_device *handle, d2_color color )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_clear_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getcliprect_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_getcliprect_to_Log( d2_device *handle, d2_border *xmin, d2_border *ymin, d2_border *xmax, d2_border *ymax )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getcliprect_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getframebuffer_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_getframebuffer_to_Log( d2_device *handle, void** ptr, d2_s32* pitch, d2_u32* width, d2_u32* height, d2_s32* format)
{
    d2_s32  return_value = d2_getframebuffer_from_Log( handle,  ptr,  pitch,  width,  height,  format );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getframebuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        void*   ptr = (void*) 0x%08X;"GS_N,  *ptr );
        printf( "DRW_API:        d2_s32  pitch = %d;"GS_N,  *pitch );
        printf( "DRW_API:        d2_u32  width = %d;"GS_N,  *width );
        printf( "DRW_API:        d2_u32  height = %d;"GS_N,  *height );
        printf( "DRW_API:        d2_s32  format = %d;"GS_N,  *format );
        printf( "DRW_API:        d2_s32  return_value = d2_getframebuffer( handle,  &ptr,  &pitch,  &width,  &height,  &format );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_selectrendermode_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_selectrendermode_to_Log( d2_device *handle, d2_u32 mode )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_selectrendermode_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_outlinewidth_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_outlinewidth_to_Log( d2_device *handle, d2_width width )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_outlinewidth_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_shadowoffset_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_shadowoffset_to_Log( d2_device *handle, d2_point x, d2_point y )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_shadowoffset_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getrendermode_to_Log
************************************************************************/
D2_EXTERN d2_u32 d2_getrendermode_to_Log( const d2_device *handle )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getrendermode_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_layermerge_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_layermerge_to_Log( d2_device *handle )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_layermerge_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_newrenderbuffer_to_Log
************************************************************************/
D2_EXTERN d2_renderbuffer * d2_newrenderbuffer_to_Log( d2_device *handle, d2_u32 initialsize, d2_u32 stepsize )
{
    d2_renderbuffer *  return_value = d2_newrenderbuffer_from_Log( handle,  initialsize,  stepsize );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_newrenderbuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32  initialsize = 0x%08X;"GS_N,  initialsize );
        printf( "DRW_API:        d2_u32  stepsize    = 0x%08X;"GS_N,  stepsize );
        printf( "DRW_API:        d2_renderbuffer*  return_value = d2_newrenderbuffer( handle,  initialsize,  stepsize );"GS_N );
        printf( "DRW_API:            /* return_value = 0x%08X */"GS_N,  return_value );
        printf( "DRW_API:            buffer = (d2_renderbuffer*) return_value;"GS_N );
        printf( "DRW_API:            if ( return_value == NULL ) { e=__LINE__; goto fin; }"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_freerenderbuffer_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_freerenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer )
{
    d2_s32  return_value = d2_freerenderbuffer_from_Log( handle,  buffer );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_freerenderbuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_renderbuffer *buffer__ = (d2_renderbuffer*) 0x%08X;"GS_N,  buffer );
        printf( "DRW_API:        d2_s32  return_value = d2_freerenderbuffer( handle,  buffer );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_selectrenderbuffer_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_selectrenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer )
{
    d2_s32  return_value = d2_selectrenderbuffer_from_Log( handle,  buffer );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_selectrenderbuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_renderbuffer *buffer__ = (d2_renderbuffer*) 0x%08X;"GS_N,  buffer );
        printf( "DRW_API:        d2_s32  return_value = d2_selectrenderbuffer( handle,  buffer );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_executerenderbuffer_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_executerenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer, d2_u32 flags )
{
    d2_s32  return_value = d2_executerenderbuffer_from_Log( handle,  buffer,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_executerenderbuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_renderbuffer *buffer__ = (d2_renderbuffer*) 0x%08X;"GS_N,  buffer );
        printf( "DRW_API:        d2_u32 flags = 0x%08X;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_executerenderbuffer( handle,  buffer,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getrenderbuffer_to_Log
************************************************************************/
D2_EXTERN d2_renderbuffer * d2_getrenderbuffer_to_Log( d2_device *handle, d2_s32 index )
{
    d2_renderbuffer *  return_value = d2_getrenderbuffer_from_Log( handle,  index );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getrenderbuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32 index = %d;"GS_N,  index );
        printf( "DRW_API:        d2_renderbuffer*  return_value = d2_getrenderbuffer( handle,  index );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_dumprenderbuffer_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_dumprenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer, void **rdata, d2_s32 *rsize )
{
    d2_s32  return_value = d2_dumprenderbuffer_from_Log( handle,  buffer,  rdata,  rsize );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_dumprenderbuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_renderbuffer *buffer__ = (d2_renderbuffer*) 0x%08X;"GS_N,  buffer );
        printf( "DRW_API:        void **rdata = 0x%08X;"GS_N,  rdata );
        printf( "DRW_API:        d2_s32 *rsize = 0x%08X;"GS_N,  rsize );
        printf( "DRW_API:        d2_s32  return_value = d2_dumprenderbuffer( handle,  buffer,  rdata,  rsize );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getrenderbuffersize_to_Log
************************************************************************/
D2_EXTERN d2_u32  d2_getrenderbuffersize_to_Log(d2_device *handle, d2_renderbuffer *rb)
{
    d2_s32  return_value = d2_getrenderbuffersize_from_Log( handle,  rb );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_getrenderbuffersize */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_renderbuffer *rb = 0x%08X;"GS_N,  rb );
        printf( "DRW_API:        d2_u32  return_value = d2_getrenderbuffersize( handle,  rb );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_freedumpedbuffer_to_Log
************************************************************************/
D2_EXTERN d2_s32  d2_freedumpedbuffer_to_Log( d2_device *handle, void *data )
{
    d2_s32  return_value = d2_freedumpedbuffer_from_Log( handle,  data );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_freedumpedbuffer */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        void *data = 0x%08X;"GS_N,  data );
        printf( "DRW_API:        d2_s32  return_value = d2_freedumpedbuffer( handle,  data );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setcolor_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setcolor_to_Log( d2_device *handle, d2_s32 index, d2_color color )
{
    d2_s32  return_value = d2_setcolor_from_Log( handle,  index,  color );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setcolor */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32 index = %d;"GS_N,  index );
        printf( "DRW_API:        d2_color color = 0x%08X;"GS_N,  color );
        printf( "DRW_API:        d2_s32  return_value = d2_setcolor( handle,  index,  color );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setalpha_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setalpha_to_Log( d2_device *handle, d2_alpha alpha )
{
    d2_s32  return_value = d2_setalpha_from_Log( handle,  alpha );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setalpha */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_alpha alpha = 0x%02X;"GS_N,  alpha );
        printf( "DRW_API:        d2_s32  return_value = d2_setalpha( handle,  alpha );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setalphaex_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setalphaex_to_Log( d2_device *handle, d2_s32 index, d2_alpha alpha )
{
    d2_s32  return_value = d2_setalphaex_from_Log( handle,  index,  alpha );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setalphaex */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32 index = %d;"GS_N,  index );
        printf( "DRW_API:        d2_alpha alpha = 0x%02X;"GS_N,  alpha );
        printf( "DRW_API:        d2_s32  return_value = d2_setalphaex( handle,  index,  alpha );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setblur_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setblur_to_Log( d2_device *handle, d2_width blur )
{
    d2_s32  return_value = d2_setblur_from_Log( handle,  blur );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setblur */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_width blur = %d;"GS_N,  blur );
        printf( "DRW_API:        d2_s32  return_value = d2_setblur( handle,  blur );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setblendmode_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setblendmode_to_Log( d2_device *handle, d2_u32 srcfactor, d2_u32 dstfactor )
{
    d2_s32  return_value = d2_setblendmode_from_Log( handle,  srcfactor,  dstfactor );
    char_t* (*  to_string )( d2_u32 ) = gs_GetStringOf_d2_blendmodes;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setblendmode */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 srcfactor = %d; /* %s */"GS_N,  srcfactor,  to_string( srcfactor ) );
        printf( "DRW_API:        d2_u32 dstfactor = %d; /* %s */"GS_N,  dstfactor,  to_string( dstfactor ) );
        printf( "DRW_API:        d2_s32  return_value = d2_setblendmode( handle,  srcfactor,  dstfactor );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setalphablendmode_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setalphablendmode_to_Log( d2_device *handle, d2_u32 srcfactor, d2_u32 dstfactor )
{
    d2_s32  return_value = d2_setalphablendmode_from_Log( handle,  srcfactor,  dstfactor );
    char_t* (*  to_string )( d2_u32 ) = gs_GetStringOf_d2_blendmodes;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setalphablendmode */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 srcfactor = %d; /* %s */"GS_N,  srcfactor,  to_string( srcfactor ) );
        printf( "DRW_API:        d2_u32 dstfactor = %d; /* %s */"GS_N,  dstfactor,  to_string( dstfactor ) );
        printf( "DRW_API:        d2_s32  return_value = d2_setalphablendmode( handle,  srcfactor,  dstfactor );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setalphablendmodeex_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setalphablendmodeex_to_Log( d2_device *handle, d2_u32 srcfactor, d2_u32 dstfactor, d2_u32 blendflags )
{
    d2_s32  return_value = d2_setalphablendmodeex_from_Log( handle,  srcfactor,  dstfactor,  blendflags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setalphablendmodeex */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 srcfactor = %d;"GS_N,  srcfactor );
        printf( "DRW_API:        d2_u32 dstfactor = %d;"GS_N,  dstfactor );
        printf( "DRW_API:        d2_u32 blendflags = %d;"GS_N,  blendflags );
        printf( "DRW_API:        d2_s32  return_value = d2_setalphablendmodeex( handle,  srcfactor,  dstfactor,  blendflags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setalphagradient_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setalphagradient_to_Log( d2_device *handle, d2_s32 index, d2_point x, d2_point y, d2_point dx, d2_point dy )
{
    d2_s32  return_value = d2_setalphagradient_from_Log( handle,  index,  x,  y,  dx,  dy );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setalphagradient */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32  index = %d;"GS_N,  index );
        printf( "DRW_API:        d2_point  x = %d;"GS_N,  x );
        printf( "DRW_API:        d2_point  y = %d;"GS_N,  y );
        printf( "DRW_API:        d2_point  dx = %d;"GS_N,  dx );
        printf( "DRW_API:        d2_point  dy = %d;"GS_N,  dy );
        printf( "DRW_API:        d2_s32  return_value = d2_setalphagradient( handle,  index,  x,  y,  dx,  dy );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setclipgradient_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setclipgradient_to_Log( d2_device *handle, d2_s32 index, d2_point x, d2_point y, d2_s32 nx, d2_s32 ny, d2_u32 flags )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setclipgradient_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setalphamode_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setalphamode_to_Log( d2_device *handle, d2_u32 mode )
{
    d2_s32  return_value = d2_setalphamode_from_Log( handle,  mode );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setalphamode */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 mode = %d;"GS_N,  mode );
        printf( "DRW_API:        d2_s32  return_value = d2_setalphamode( handle,  mode );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setantialiasing_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setantialiasing_to_Log( d2_device *handle, d2_s32 enable )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setantialiasing_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setpatternalpha_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setpatternalpha_to_Log( d2_device *handle, d2_s32 index, d2_alpha alpha )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setpatternalpha_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setfillmode_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setfillmode_to_Log( d2_device *handle, d2_u32 mode )
{
    d2_s32  return_value = d2_setfillmode( handle,  mode );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_setfillmode */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 mode = 0x%08X;"GS_N,  mode );
        printf( "DRW_API:        d2_s32  return_value = d2_setfillmode( handle,  mode );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setpattern_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setpattern_to_Log( d2_device *handle, d2_pattern pattern )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setpattern_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setpatternparam_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setpatternparam_to_Log( d2_device *handle, d2_point x, d2_point y, d2_width dx, d2_width dy )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setpatternparam_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setpatternmode_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setpatternmode_to_Log( d2_device *handle, d2_u32 mode )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setpatternmode_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setpatternsize_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setpatternsize_to_Log( d2_device *handle, d2_s32 size )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setpatternsize_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setlinecap_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setlinecap_to_Log( d2_device *handle, d2_u32 mode )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setlinecap_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setlinejoin_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setlinejoin_to_Log( d2_device *handle, d2_u32 mode )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setlinejoin_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setlinepattern_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setlinepattern_to_Log( d2_device *handle, d2_width scale, d2_s32 offset )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setlinepattern_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setmiterlimit_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setmiterlimit_to_Log( d2_device *handle, d2_width miter )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setmiterlimit_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexture_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexture_to_Log( d2_device *handle, void *ptr, d2_s32 pitch, d2_s32 width, d2_s32 height, d2_u32 format )
{
    d2_s32  return_value = d2_settexture_from_Log( handle,  ptr,  pitch,  width,  height,  format );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_settexture */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        void *ptr = (void*) " );
        gs_PrintVariable( ptr );
        printf( ";"GS_N );
        printf( "DRW_API:        d2_s32 pitch = %d;"GS_N,  pitch );
        printf( "DRW_API:        d2_s32 width = %d;"GS_N,  width );
        printf( "DRW_API:        d2_s32 height = %d;"GS_N,  height );
        printf( "DRW_API:        d2_u32 format = 0x%X;"GS_N,  format );
        printf( "DRW_API:        d2_s32  return_value = d2_settexture( handle,  ptr,  pitch,  width,  height,  format );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexturemode_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexturemode_to_Log( d2_device *handle, d2_u32 mode )
{
    d2_s32  return_value = d2_settexturemode_from_Log( handle,  mode );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_settexturemode */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 mode = 0x%08X;"GS_N,  mode );
        printf( "DRW_API:        d2_s32  return_value = d2_settexturemode( handle,  mode );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settextureoperation_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settextureoperation_to_Log( d2_device *handle, d2_u8 amode, d2_u8 rmode, d2_u8 gmode, d2_u8 bmode )
{
    d2_s32  return_value = d2_settextureoperation_from_Log( handle,  amode,  rmode,  gmode,  bmode );
    char_t* (*  to_string )( d2_u8 ) = gs_GetStringOf_d2_textureoperations;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_settextureoperation */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u8 amode = 0x%02X; /* %s */"GS_N,  amode,  to_string( amode ) );
        printf( "DRW_API:        d2_u8 rmode = 0x%02X; /* %s */"GS_N,  rmode,  to_string( gmode ) );
        printf( "DRW_API:        d2_u8 gmode = 0x%02X; /* %s */"GS_N,  gmode,  to_string( bmode ) );
        printf( "DRW_API:        d2_u8 bmode = 0x%02X; /* %s */"GS_N,  bmode,  to_string( amode ) );
        printf( "DRW_API:        d2_s32  return_value = d2_settextureoperation( handle,  amode,  rmode,  gmode,  bmode );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexopparam_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexopparam_to_Log( d2_device *handle, d2_u32 index, d2_u32 p1, d2_u32 p2 )
{
    d2_s32  return_value = d2_settexopparam_from_Log( handle,  index,  p1,  p2 );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_settexopparam */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_u32 index = 0x%08X;"GS_N,  index );
        printf( "DRW_API:        d2_u32 p1 = 0x%08X;"GS_N,  p1 );
        printf( "DRW_API:        d2_u32 p2 = 0x%08X;"GS_N,  p2 );
        printf( "DRW_API:        d2_s32  return_value = d2_settexopparam( handle,  index,  p1,  p2 );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexturemapping_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexturemapping_to_Log( d2_device *handle, d2_point x, d2_point y, d2_s32 u0, d2_s32 v0, d2_s32 dxu, d2_s32 dyu, d2_s32 dxv, d2_s32 dyv )
{
    d2_s32  return_value = d2_settexturemapping_from_Log( handle,  x,  y,  u0,  v0,  dxu,  dyu,  dxv,  dyv );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_settexturemapping */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point x = %d; /* %f */"GS_N,  x,  x / 16.0f );
        printf( "DRW_API:        d2_point y = %d; /* %f */"GS_N,  y,  y / 16.0f );
        printf( "DRW_API:        d2_s32 u0 = %d; /* %f */"GS_N,  u0,  u0 / 65536.0f );
        printf( "DRW_API:        d2_s32 v0 = %d; /* %f */"GS_N,  v0,  v0 / 65536.0f );
        printf( "DRW_API:        d2_s32 dxu = %d; /* %f */"GS_N,  dxu,  dxu / 65536.0f );
        printf( "DRW_API:        d2_s32 dyu = %d; /* %f */"GS_N,  dyu,  dyu / 65536.0f );
        printf( "DRW_API:        d2_s32 dxv = %d; /* %f */"GS_N,  dxv,  dxv / 65536.0f );
        printf( "DRW_API:        d2_s32 dyv = %d; /* %f */"GS_N,  dyv,  dyv / 65536.0f );
        printf( "DRW_API:        d2_s32  return_value = d2_settexturemapping( handle,  x,  y,  u0,  v0,  dxu,  dyu,  dxv,  dyv );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexelcenter_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexelcenter_to_Log( d2_device *handle, d2_point x, d2_point y )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_settexelcenter_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexclut_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexclut_to_Log( d2_device *handle, d2_color* clut )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_settexclut_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexclut_part_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexclut_part_to_Log( d2_device *handle, const d2_color* clut_part, d2_u32 start_index, d2_u32 length )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_settexclut_part_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_writetexclut_direct_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_writetexclut_direct_to_Log( d2_device *handle, const d2_color* clut_part, d2_u32 start_index, d2_u32 length )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_writetexclut_direct_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexclut_offset_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexclut_offset_to_Log( d2_device *handle, d2_u32 offset )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_settexclut_offset_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_settexclut_format_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_settexclut_format_to_Log(  d2_device *handle, d2_u32 format )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_settexclut_format_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setcolorkey_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setcolorkey_to_Log( d2_device *handle, d2_s32 enable, d2_color color_key )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setcolorkey_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setcircleextend_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setcircleextend_to_Log( d2_device *handle, d2_width offset )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setcircleextend_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getcolor_to_Log
************************************************************************/
D2_EXTERN d2_color d2_getcolor_to_Log( d2_device *handle, d2_s32 index )
{
    d2_color  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getcolor_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getalpha_to_Log
************************************************************************/
D2_EXTERN d2_alpha d2_getalpha_to_Log( d2_device *handle )
{
    d2_alpha  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getalpha_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getalphaex_to_Log
************************************************************************/
D2_EXTERN d2_alpha d2_getalphaex_to_Log( d2_device *handle, d2_s32 index )
{
    d2_alpha  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getalphaex_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getblur_to_Log
************************************************************************/
D2_EXTERN d2_width d2_getblur_to_Log( d2_device *handle )
{
    d2_width  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getblur_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getblendmodesrc_to_Log
************************************************************************/
D2_EXTERN d2_u32 d2_getblendmodesrc_to_Log( d2_device *handle )
{
    d2_u32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getblendmodesrc_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getblendmodedst_to_Log
************************************************************************/
D2_EXTERN d2_u32 d2_getblendmodedst_to_Log( d2_device *handle )
{
    d2_u32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getblendmodedst_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getalphablendmodesrc_to_Log
************************************************************************/
D2_EXTERN d2_u32 d2_getalphablendmodesrc_to_Log( d2_device *handle )
{
    d2_u32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getalphablendmodesrc_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getalphablendmodedst_to_Log
************************************************************************/
D2_EXTERN d2_u32 d2_getalphablendmodedst_to_Log( d2_device *handle )
{
    d2_u32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getalphablendmodedst_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getalphablendmodeflags_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_getalphablendmodeflags_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getalphablendmodeflags_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getalphamode_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_getalphamode_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getalphamode_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getantialiasing_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_getantialiasing_to_Log( d2_device *handle )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getantialiasing_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getpatternalpha_to_Log
************************************************************************/
D2_EXTERN d2_alpha d2_getpatternalpha_to_Log( d2_device *handle, d2_s32 index )
{
    d2_alpha  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getpatternalpha_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getfillmode_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_getfillmode_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getfillmode_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getpattern_to_Log
************************************************************************/
D2_EXTERN d2_pattern d2_getpattern_to_Log( d2_device *handle )
{
    d2_pattern  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getpattern_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getpatternmode_to_Log
************************************************************************/
D2_EXTERN d2_u32 d2_getpatternmode_to_Log( d2_device *handle )
{
    d2_u32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getpatternmode_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getpatternsize_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_getpatternsize_to_Log( d2_device *handle )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getpatternsize_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getlinecap_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_getlinecap_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getlinecap_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getlinejoin_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_getlinejoin_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getlinejoin_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_gettextureoperationa_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_gettextureoperationa_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_gettextureoperationa_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_gettextureoperationr_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_gettextureoperationr_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_gettextureoperationr_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_gettextureoperationg_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_gettextureoperationg_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_gettextureoperationg_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_gettextureoperationb_to_Log
************************************************************************/
D2_EXTERN d2_u8  d2_gettextureoperationb_to_Log( d2_device *handle )
{
    d2_u8  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_gettextureoperationb_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_gettexopparam1_to_Log
************************************************************************/
D2_EXTERN d2_alpha d2_gettexopparam1_to_Log( d2_device *handle, d2_u32 index )
{
    d2_alpha  return_value = d2_gettexopparam1_from_Log( handle,  index );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_gettexopparam1 */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32  return_value = d2_gettexopparam1( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_gettexopparam2_to_Log
************************************************************************/
D2_EXTERN d2_alpha d2_gettexopparam2_to_Log( d2_device *handle, d2_u32 index )
{
    d2_alpha  return_value = d2_gettexopparam2_from_Log( handle,  index );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_gettexopparam2 */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_s32  return_value = d2_gettexopparam2( handle );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderbox_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderbox_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_width w, d2_width h )
{
    d2_s32  return_value = d2_renderbox_from_Log( handle,  x1,  y1,  w,  h );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderbox */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point x1 = %d; /* %f */"GS_N,  x1,  x1 / 16.0f );
        printf( "DRW_API:        d2_point y1 = %d; /* %f */"GS_N,  y1,  y1 / 16.0f );
        printf( "DRW_API:        d2_width w = %d; /* %f */"GS_N,  w,  w / 16.0f );
        printf( "DRW_API:        d2_width h = %d; /* %f */"GS_N,  h,  h / 16.0f );
        printf( "DRW_API:        d2_s32  return_value = d2_renderbox( handle,  x1,  y1,  w,  h );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderline_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderline_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_width w, d2_u32 flags )
{
    d2_s32  return_value = d2_renderline_from_Log( handle,  x1,  y1,  x2,  y2,  w,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderline */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  x1 = %d; /* %f */"GS_N,  x1,  x1 / 16.0f );
        printf( "DRW_API:        d2_point  y1 = %d; /* %f */"GS_N,  y1,  y1 / 16.0f );
        printf( "DRW_API:        d2_point  x2 = %d; /* %f */"GS_N,  x2,  x2 / 16.0f );
        printf( "DRW_API:        d2_point  y2 = %d; /* %f */"GS_N,  y2,  y2 / 16.0f );
        printf( "DRW_API:        d2_width  w  = %d; /* %f */"GS_N,  w,   w  / 16.0f );
        printf( "DRW_API:        d2_u32    flags = 0x%X;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_renderline( handle,  x1,  y1,  x2,  y2,  w,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_rendertri_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_rendertri_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_point x3, d2_point y3, d2_u32 flags )
{
    d2_s32  return_value = d2_rendertri_from_Log( handle,  x1,  y1,  x2,  y2,  x3,  y3,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_rendertri */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  x1 = %d; /* %f */"GS_N,  x1,  x1 / 16.0f );
        printf( "DRW_API:        d2_point  y1 = %d; /* %f */"GS_N,  y1,  y1 / 16.0f );
        printf( "DRW_API:        d2_point  x2 = %d; /* %f */"GS_N,  x2,  x2 / 16.0f );
        printf( "DRW_API:        d2_point  y2 = %d; /* %f */"GS_N,  y2,  y2 / 16.0f );
        printf( "DRW_API:        d2_point  x3 = %d; /* %f */"GS_N,  x3,  x3 / 16.0f );
        printf( "DRW_API:        d2_point  y3 = %d; /* %f */"GS_N,  y3,  y3 / 16.0f );
        printf( "DRW_API:        d2_u32    flags = 0x%X;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_rendertri( handle,  x1,  y1,  x2,  y2,  x3,  y3,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderquad_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderquad_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_point x3, d2_point y3, d2_point x4, d2_point y4, d2_u32 flags )
{
    d2_s32  return_value = d2_renderquad_from_Log( handle,  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderquad */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  x1 = %d; /* %f */"GS_N,  x1,  x1 / 16.0f );
        printf( "DRW_API:        d2_point  y1 = %d; /* %f */"GS_N,  y1,  y1 / 16.0f );
        printf( "DRW_API:        d2_point  x2 = %d; /* %f */"GS_N,  x2,  x2 / 16.0f );
        printf( "DRW_API:        d2_point  y2 = %d; /* %f */"GS_N,  y2,  y2 / 16.0f );
        printf( "DRW_API:        d2_point  x3 = %d; /* %f */"GS_N,  x3,  x3 / 16.0f );
        printf( "DRW_API:        d2_point  y3 = %d; /* %f */"GS_N,  y3,  y3 / 16.0f );
        printf( "DRW_API:        d2_point  x4 = %d; /* %f */"GS_N,  x4,  x4 / 16.0f );
        printf( "DRW_API:        d2_point  y4 = %d; /* %f */"GS_N,  y4,  y4 / 16.0f );
        printf( "DRW_API:        d2_u32    flags = 0x%X;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_renderquad( handle,  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_rendercircle_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_rendercircle_to_Log( d2_device *handle, d2_point x, d2_point y, d2_width r, d2_width w )
{
    d2_s32  return_value = d2_rendercircle_from_Log( handle,  x,  y,  r,  w );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_rendercircle */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  x = %d; /* %f */"GS_N,  x,  x / 16.0f );
        printf( "DRW_API:        d2_point  y = %d; /* %f */"GS_N,  y,  y / 16.0f );
        printf( "DRW_API:        d2_width  r = %d; /* %f */"GS_N,  r,  r / 16.0f );
        printf( "DRW_API:        d2_width  w = %d; /* %f */"GS_N,  w,  w / 16.0f );
        printf( "DRW_API:        d2_s32  return_value = d2_rendercircle( handle,  x,  y,  r,  w );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderwedge_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderwedge_to_Log( d2_device *handle, d2_point x, d2_point y, d2_width r, d2_width w, d2_s32 nx1, d2_s32 ny1, d2_s32 nx2, d2_s32 ny2, d2_u32 flags )
{
    d2_s32  return_value = d2_renderwedge_from_Log( handle,  x,  y,  r,  w,  nx1,  ny1,  nx2,  ny2,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderwedge */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  x = %d; /* %f */"GS_N,  x,  x / 16.0f );
        printf( "DRW_API:        d2_point  y = %d; /* %f */"GS_N,  y,  y / 16.0f );
        printf( "DRW_API:        d2_width  r = %d; /* %f */"GS_N,  r,  r / 16.0f );
        printf( "DRW_API:        d2_width  w = %d; /* %f */"GS_N,  w,  w / 16.0f );
        printf( "DRW_API:        d2_s32    nx1 = %d; /* %f */"GS_N,  nx1,  nx1 / 16.0f );
        printf( "DRW_API:        d2_s32    ny1 = %d; /* %f */"GS_N,  ny1,  ny1 / 16.0f );
        printf( "DRW_API:        d2_s32    nx2 = %d; /* %f */"GS_N,  nx2,  nx2 / 16.0f );
        printf( "DRW_API:        d2_s32    ny2 = %d; /* %f */"GS_N,  ny2,  ny2 / 16.0f );
        printf( "DRW_API:        d2_u32    flags = 0x%X;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_renderwedge( handle,  x,  y,  r,  w,  nx1,  ny1,  nx2,  ny2,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderline2_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderline2_to_Log(  d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_width w1, d2_width w2, d2_u32 flags )
{
    d2_s32  return_value = d2_renderline2_from_Log( handle,  x1,  y1,  x2,  y2,  w1,  w2,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderline2 */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  x1 = %d; /* %f */"GS_N,  x1,  x1 / 16.0f );
        printf( "DRW_API:        d2_point  y1 = %d; /* %f */"GS_N,  y1,  y1 / 16.0f );
        printf( "DRW_API:        d2_point  x2 = %d; /* %f */"GS_N,  x2,  x2 / 16.0f );
        printf( "DRW_API:        d2_point  y2 = %d; /* %f */"GS_N,  y2,  y2 / 16.0f );
        printf( "DRW_API:        d2_point  w1 = %d; /* %f */"GS_N,  w1,  w1 / 16.0f );
        printf( "DRW_API:        d2_point  w2 = %d; /* %f */"GS_N,  w2,  w2 / 16.0f );
        printf( "DRW_API:        d2_u32    flags = 0x%X;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_renderline2( handle,  x1,  y1,  x2,  y2,  w1,  w2,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderpolyline_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderpolyline_to_Log( d2_device *handle, const d2_point *data, d2_u32 count, d2_width w, d2_u32 flags)
{
    d2_s32  return_value = d2_renderpolyline_from_Log( handle,  data,  count,  w,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderpolyline */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  data = 0x%08X;"GS_N,  data );
        printf( "DRW_API:        d2_u32    count = %d;"GS_N,  count );
        printf( "DRW_API:        d2_width  w = %d;"GS_N,  w );
        printf( "DRW_API:        d2_u32    flags = %d;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_renderpolyline( handle,  data,  count,  w,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderpolyline2_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderpolyline2_to_Log( d2_device *handle, const d2_point *data, d2_u32 count, const d2_width *w, d2_u32 flags)
{
    d2_s32  return_value = d2_renderpolyline2_from_Log( handle,  data,  count,  w,  flags );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderpolyline2 */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point  data = 0x%08X;"GS_N,  data );
        printf( "DRW_API:        d2_u32    count = %d;"GS_N,  count );
        printf( "DRW_API:        d2_width  w = %d;"GS_N,  w );
        printf( "DRW_API:        d2_u32    flags = %d;"GS_N,  flags );
        printf( "DRW_API:        d2_s32  return_value = d2_renderpolyline2( handle,  data,  count,  w,  flags );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_rendertrilist_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_rendertrilist_to_Log( d2_device *handle, const d2_point *data, const d2_u32 *flags, d2_u32 count)
{
    d2_s32  return_value = d2_rendertrilist_from_Log( handle,  data,  flags,  count );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_rendertrilist */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point* data = 0x%08X;"GS_N,  data );
        printf( "DRW_API:        d2_u32*   flags = 0x%08X;"GS_N,  flags );
        printf( "DRW_API:        d2_u32    count = %d;"GS_N,  count );
        printf( "DRW_API:        d2_s32  return_value = d2_rendertrilist( handle,  data,  flags,  count );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_rendertrifan_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_rendertrifan_to_Log( d2_device *handle, const d2_point *data, const d2_u32 *flags, d2_u32 count)
{
    d2_s32  return_value = d2_rendertrifan_from_Log( handle,  data,  flags,  count );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_rendertrifan */{"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        d2_point* data = 0x%08X;"GS_N,  data );
        printf( "DRW_API:        d2_u32*   flags = 0x%08X;"GS_N,  flags );
        printf( "DRW_API:        d2_u32    count = %d;"GS_N,  count );
        printf( "DRW_API:        d2_s32  return_value = d2_rendertrifan( handle,  data,  flags,  count );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_rendertristrip_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_rendertristrip_to_Log( d2_device *handle, const d2_point *data, const d2_u32 *flags, d2_u32 count)
{
    d2_s32  return_value = d2_rendertristrip_from_Log( handle,  data,  flags,  count );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        int_fast32_t  point_index;
        int_fast32_t  point_count = count + 2;

        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_rendertristrip */ {"GS_N );
        printf( "DRW_API:        d2_device *handle__ = (d2_device*) 0x%08X;"GS_N,  handle );
        printf( "DRW_API:        static const d2_point data[] = {"GS_N );
        for ( point_index = 0;  point_index < point_count;  point_index += 1 ) {
            d2_point  x = data[ point_index * 2 + 0 ];
            d2_point  y = data[ point_index * 2 + 1 ];
            printf( "DRW_API:            %d, %d,  /* %f, %f */"GS_N,
                x,  y,  x / 16.0f,  y / 16.0f );
        }
        printf( "DRW_API:        };"GS_N );
        printf( "DRW_API:        const d2_u32 *flags = 0x%08X;"GS_N,  flags );
        printf( "DRW_API:        d2_u32 count = %d;"GS_N,  count );
        printf( "DRW_API:        d2_s32  return_value = d2_rendertristrip( handle,  data,  flags,  count );"GS_N );
        printf( "DRW_API:            if ( return_value != 0x%08X ) { e=__LINE__; goto fin; }"GS_N,  return_value );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_renderpolygon_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_renderpolygon_to_Log( d2_device *handle, const d2_point *data, d2_u32 count, d2_u32 flags)
{
    d2_s32  return_value = d2_renderpolygon_from_Log( handle,  data,  flags,  count );

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    /* d2_renderpolygon */{"GS_N );
        printf( "DRW_API:        /* d2_renderpolygon_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setblitsrc_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setblitsrc_to_Log( d2_device *handle, void *ptr, d2_s32 pitch, d2_s32 width, d2_s32 height, d2_u32 format )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setblitsrc_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_blitcopy_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_blitcopy_to_Log( d2_device *handle, d2_s32 srcwidth, d2_s32 srcheight, d2_blitpos srcx, d2_blitpos srcy, d2_width dstwidth, d2_width dstheight, d2_point dstx, d2_point dsty, d2_u32 flags )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_blitcopy_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setperfcountevent_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setperfcountevent_to_Log( d2_device *handle, d2_u32 counter, d2_u32 event )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setperfcountevent_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_setperfcountvalue_to_Log
************************************************************************/
D2_EXTERN d2_s32 d2_setperfcountvalue_to_Log( d2_device *handle, d2_u32 counter, d2_slong value )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_setperfcountvalue_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_getperfcountvalue_to_Log
************************************************************************/
D2_EXTERN d2_slong d2_getperfcountvalue_to_Log( d2_device *handle, d2_u32 counter )
{
    d2_slong  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_getperfcountvalue_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_utility_maptriangle_to_Log
************************************************************************/
d2_s32 d2_utility_maptriangle_to_Log( d2_device *handle, const d2_f32 *points, const d2_f32 *uvs )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_utility_maptriangle_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_utility_perspectivewarp_to_Log
************************************************************************/
d2_s32 d2_utility_perspectivewarp_to_Log( d2_device *handle, d2_u16 srcwidth, d2_u16 srcheight, d2_s16 srcx, d2_s16 srcy, d2_s16 dstwidth, d2_s16 dstheight, d2_s16 dstx, d2_s16 dsty, d2_u16 wt )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_utility_perspectivewarp_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_utility_fbblitcopy_to_Log
************************************************************************/
d2_s32 d2_utility_fbblitcopy_to_Log( d2_device *handle, d2_u16 width, d2_u16 height, d2_blitpos srcx, d2_blitpos srcy, d2_blitpos dstx, d2_blitpos dsty, d2_u32 flags)
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_utility_fbblitcopy_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_rendercircle_no_hilimiterprecision_to_Log
************************************************************************/
void d2_rendercircle_no_hilimiterprecision_to_Log( d2_device *handle, d2_u32 flag )
{
    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_rendercircle_no_hilimiterprecision_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif
}


/***********************************************************************
* Function: d2_executedlist_to_Log
************************************************************************/
d2_s32 d2_executedlist_to_Log( d2_device *handle, const void *address, d2_u32 flags )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_executedlist_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: d2_adddlist_to_Log
************************************************************************/
d2_s32 d2_adddlist_to_Log( d2_device *handle, void *address, d2_s32 size, d2_u32 flags )
{
    d2_s32  return_value = 0;

    #if  USE_DRW_API_PRINT_LOG
    if ( gs_IsLogEnabled() ) {
        bool_t  was_all_enabled = ! __disable_irq();
        printf( "DRW_API:    {"GS_N );
        printf( "DRW_API:        /* d2_adddlist_to_Log() is not implemented */"GS_N );
        printf( "DRW_API:    }"GS_N );
        if ( was_all_enabled ) { __enable_irq(); }
    }
    #endif

    return  return_value;
}


/***********************************************************************
* Function: R_DRW_RegisterTextureVariables
*
* Example:
*    > #include  <stdint.h>
*    > #include  "rz_co.h"
*    > {
*    > extern errnum_t  R_DRW_RegisterTextureVariables( const void**,  int_fast32_t );
*    > static const void*  variables[] = {
*    >    g_JPEG_jpg_RGB565,  "g_JPEG_jpg_RGB565",
*    >    g_Crystal_png_ARGB4444,  "g_Crystal_png_ARGB4444" };
*    >
*    > R_DRW_RegisterTextureVariables( variables,  R_COUNT_OF( variables ) );
*    > }
*
* Example:
*    > // Case of not constant
*    > static const void*  variables[3*2] = {
*    >    g_JPEG_jpg_RGB565,  "g_JPEG_jpg_RGB565",
*    >    g_Crystal_png_ARGB4444,  "g_Crystal_png_ARGB4444" };
*    >    variables[2*2+0] = g_FrameBuffer;
*    >    variables[2*2+1] = "/* g_FrameBuffer *""/";
************************************************************************/
errnum_t  R_DRW_RegisterTextureVariables(
    const void**  in_StaticVariableAddressesAndStrings,
    int_fast32_t  in_VariableAddressesCount )
{
    errnum_t      e;
    int_fast32_t  i;

    gs_Variables = (const gs_address_and_string_t*) in_StaticVariableAddressesAndStrings;
    gs_VariableAddressesCount = in_VariableAddressesCount / 2;


    /* Guard : Check if "in_StaticVariableAddressesAndStrings" was sorted */
    for ( i = 0;  i < gs_VariableAddressesCount - 1;  i += 1 ) {
        const void*  previous_variable = gs_Variables[i].Address;
        const void*  next_variable     = gs_Variables[i+1].Address;

        if ( previous_variable >= next_variable ) {
            e=E_DRW_OTHER_1; goto fin;
        }
    }

    e=0;
fin:
    R_ERROR_Set( e );
    return  e;
}


/***********************************************************************
* Function: gs_PrintVariable
************************************************************************/
void  gs_PrintVariable( const void*  in_PhysicalAddress )
{
    errnum_t      e;
    int_fast32_t  next_index;


    /* next_index = ... */
    for ( next_index = 0;  next_index < gs_VariableAddressesCount;  next_index += 1 ) {
        const void*  variable_virtual_address = gs_Variables[ next_index ].Address;
        uintptr_t    variable_physical_address;
        e= R_MEMORY_ToPhysicalAddress( variable_virtual_address,  &variable_physical_address );
            if(e){
                variable_physical_address = (uintptr_t) variable_virtual_address;
                R_ERROR_Set( E_STATE_CLEAR );
            }

        if ( (uintptr_t) in_PhysicalAddress < variable_physical_address )
            { break; }  /* next_index = . */ 
    }


    /* printf */
    if ( next_index == 0 ) {
        printf( "0x%08X",  in_PhysicalAddress );
        R_ERROR_Set( E_DRW_OBLIVIOUS_4 );  /* Error: Not called "R_DRW_RegisterTextureVariables". */
    } else {
        const void*    variable_address = gs_Variables[ next_index - 1 ].Address;
        const char_t*  variable_name    = gs_Variables[ next_index - 1 ].String;
        ptrdiff_t      offset = (uint8_t*) in_PhysicalAddress - (uint8_t*) variable_address;
        bool_t         is_comment = ( variable_name[0] == '/' );

        if ( ! is_comment ) {
            if ( offset == 0 ) {
                printf( "%s",  variable_name );
            } else {
                printf( "( (const uint8_t*) %s + 0x%08X )",  variable_name,  offset );
            }
        } else { /* is_comment */
            const char_t*  comment = variable_name;

            printf( "0x%08X %s",  in_PhysicalAddress,  comment );
        }
    }
}


/***********************************************************************
* Function: gs_GetStringOf_d2_textureoperations
************************************************************************/
char_t*  gs_GetStringOf_d2_textureoperations( d2_u8  in_Value )
{
    char*  symbol;

    if ( in_Value == d2_to_zero ) {
        symbol = "d2_to_zero";
    } else if ( in_Value == d2_to_one ) {
        symbol = "d2_to_one";
    } else if ( in_Value == d2_to_replace ) {
        symbol = "d2_to_replace";
    } else if ( in_Value == d2_to_copy ) {
        symbol = "d2_to_copy";
    } else if ( in_Value == d2_to_invert ) {
        symbol = "d2_to_invert";
    } else if ( in_Value == d2_to_multiply ) {
        symbol = "d2_to_multiply";
    } else if ( in_Value == d2_to_invmultiply ) {
        symbol = "d2_to_invmultiply";
    } else if ( in_Value == d2_to_blend ) {
        symbol = "d2_to_blend";
    } else {
        symbol = "(unknown)";
    }

    return  symbol;
}


/***********************************************************************
* Function: gs_GetStringOf_d2_blendmodes
************************************************************************/
char_t*  gs_GetStringOf_d2_blendmodes( d2_u32  in_Value )
{
    char_t*  symbol;

    if ( in_Value == d2_bm_zero ) {
        symbol = "d2_bm_zero";
    } else if ( in_Value == d2_bm_one ) {
        symbol = "d2_bm_one";
    } else if ( in_Value == d2_bm_alpha ) {
        symbol = "d2_bm_alpha";
    } else if ( in_Value == d2_bm_one_minus_alpha ) {
        symbol = "d2_bm_one_minus_alpha";
    } else {
        symbol = "(unknown)";
    }

    return  symbol;
}


#endif  /* USE_CONFIG_REDIRECT_DRW_API */
