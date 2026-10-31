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
* File Name: dave_log.h
*    Macro redirect of D/AVE 2D API.
* - Description: Target API is defined in "dave_driver.h".
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_drw.h"

#ifndef DAVE_LOG_H
#define DAVE_LOG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef  USE_CONFIG_REDIRECT_DRW_API


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Macros: Macro redirect
************************************************************************/
#define  d2_getversion                        d2_getversion_to_Log
#define  d2_getversion_from_Log               d2_getversion
#define  d2_getversionstring                  d2_getversionstring_to_Log
#define  d2_getversionstring_from_Log         d2_getversionstring
#define  d2_opendevice                        d2_opendevice_to_Log
#define  d2_opendevice_from_Log               d2_opendevice
#define  d2_closedevice                       d2_closedevice_to_Log
#define  d2_closedevice_from_Log              d2_closedevice
#define  d2_geterror                          d2_geterror_to_Log
#define  d2_geterror_from_Log                 d2_geterror
#define  d2_geterrorstring                    d2_geterrorstring_to_Log
#define  d2_geterrorstring_from_Log           d2_geterrorstring
#define  d2_translateerror                    d2_translateerror_to_Log
#define  d2_translateerror_from_Log           d2_translateerror
#define  d2_inithw                            d2_inithw_to_Log
#define  d2_inithw_from_Log                   d2_inithw
#define  d2_deinithw                          d2_deinithw_to_Log
#define  d2_deinithw_from_Log                 d2_deinithw
#define  d2_level1interface                   d2_level1interface_to_Log
#define  d2_level1interface_from_Log          d2_level1interface
#define  d2_getrevisionhw                     d2_getrevisionhw_to_Log
#define  d2_getrevisionhw_from_Log            d2_getrevisionhw
#define  d2_getrevisionstringhw               d2_getrevisionstringhw_to_Log
#define  d2_getrevisionstringhw_from_Log      d2_getrevisionstringhw
#define  d2_setdlistblocksize                 d2_setdlistblocksize_to_Log
#define  d2_setdlistblocksize_from_Log        d2_setdlistblocksize
#define  d2_getdlistblocksize                 d2_getdlistblocksize_to_Log
#define  d2_getdlistblocksize_from_Log        d2_getdlistblocksize
#define  d2_getdlistblockcount                d2_getdlistblockcount_to_Log
#define  d2_getdlistblockcount_from_Log       d2_getdlistblockcount
#define  d2_commandspending                   d2_commandspending_to_Log
#define  d2_commandspending_from_Log          d2_commandspending
#define  d2_lowlocalmemmode                   d2_lowlocalmemmode_to_Log
#define  d2_lowlocalmemmode_from_Log          d2_lowlocalmemmode
#define  d2_newcontext                        d2_newcontext_to_Log
#define  d2_newcontext_from_Log               d2_newcontext
#define  d2_freecontext                       d2_freecontext_to_Log
#define  d2_freecontext_from_Log              d2_freecontext
#define  d2_selectcontext                     d2_selectcontext_to_Log
#define  d2_selectcontext_from_Log            d2_selectcontext
#define  d2_solidcontext                      d2_solidcontext_to_Log
#define  d2_solidcontext_from_Log             d2_solidcontext
#define  d2_outlinecontext                    d2_outlinecontext_to_Log
#define  d2_outlinecontext_from_Log           d2_outlinecontext
#define  d2_getcontext                        d2_getcontext_to_Log
#define  d2_getcontext_from_Log               d2_getcontext
#define  d2_framebuffer                       d2_framebuffer_to_Log
#define  d2_framebuffer_from_Log              d2_framebuffer
#define  d2_cliprect                          d2_cliprect_to_Log
#define  d2_cliprect_from_Log                 d2_cliprect
#define  d2_flushframe                        d2_flushframe_to_Log
#define  d2_flushframe_from_Log               d2_flushframe
#define  d2_startframe                        d2_startframe_to_Log
#define  d2_startframe_from_Log               d2_startframe
#define  d2_endframe                          d2_endframe_to_Log
#define  d2_endframe_from_Log                 d2_endframe
#define  d2_relocateframe                     d2_relocateframe_to_Log
#define  d2_relocateframe_from_Log            d2_relocateframe
#define  d2_gethaspreviousframe               d2_gethaspreviousframe_to_Log
#define  d2_gethaspreviousframe_from_Log      d2_gethaspreviousframe
#define  d2_getisemptydlistentry              d2_getisemptydlistentry_to_Log
#define  d2_getisemptydlistentry_from_Log     d2_getisemptydlistentry
#define  d2_clear                             d2_clear_to_Log
#define  d2_clear_from_Log                    d2_clear
#define  d2_getcliprect                       d2_getcliprect_to_Log
#define  d2_getcliprect_from_Log              d2_getcliprect
#define  d2_getframebuffer                    d2_getframebuffer_to_Log
#define  d2_getframebuffer_from_Log           d2_getframebuffer
#define  d2_selectrendermode                  d2_selectrendermode_to_Log
#define  d2_selectrendermode_from_Log         d2_selectrendermode
#define  d2_outlinewidth                      d2_outlinewidth_to_Log
#define  d2_outlinewidth_from_Log             d2_outlinewidth
#define  d2_shadowoffset                      d2_shadowoffset_to_Log
#define  d2_shadowoffset_from_Log             d2_shadowoffset
#define  d2_getrendermode                     d2_getrendermode_to_Log
#define  d2_getrendermode_from_Log            d2_getrendermode
#define  d2_layermerge                        d2_layermerge_to_Log
#define  d2_layermerge_from_Log               d2_layermerge
#define  d2_newrenderbuffer                   d2_newrenderbuffer_to_Log
#define  d2_newrenderbuffer_from_Log          d2_newrenderbuffer
#define  d2_freerenderbuffer                  d2_freerenderbuffer_to_Log
#define  d2_freerenderbuffer_from_Log         d2_freerenderbuffer
#define  d2_selectrenderbuffer                d2_selectrenderbuffer_to_Log
#define  d2_selectrenderbuffer_from_Log       d2_selectrenderbuffer
#define  d2_executerenderbuffer               d2_executerenderbuffer_to_Log
#define  d2_executerenderbuffer_from_Log      d2_executerenderbuffer
#define  d2_getrenderbuffer                   d2_getrenderbuffer_to_Log
#define  d2_getrenderbuffer_from_Log          d2_getrenderbuffer
#define  d2_dumprenderbuffer                  d2_dumprenderbuffer_to_Log
#define  d2_dumprenderbuffer_from_Log         d2_dumprenderbuffer
#define  d2_getrenderbuffersize               d2_getrenderbuffersize_to_Log
#define  d2_getrenderbuffersize_from_Log      d2_getrenderbuffersize
#define  d2_freedumpedbuffer                  d2_freedumpedbuffer_to_Log
#define  d2_freedumpedbuffer_from_Log         d2_freedumpedbuffer
#define  d2_setcolor                          d2_setcolor_to_Log
#define  d2_setcolor_from_Log                 d2_setcolor
#define  d2_setalpha                          d2_setalpha_to_Log
#define  d2_setalpha_from_Log                 d2_setalpha
#define  d2_setalphaex                        d2_setalphaex_to_Log
#define  d2_setalphaex_from_Log               d2_setalphaex
#define  d2_setblur                           d2_setblur_to_Log
#define  d2_setblur_from_Log                  d2_setblur
#define  d2_setblendmode                      d2_setblendmode_to_Log
#define  d2_setblendmode_from_Log             d2_setblendmode
#define  d2_setalphablendmode                 d2_setalphablendmode_to_Log
#define  d2_setalphablendmode_from_Log        d2_setalphablendmode
#define  d2_setalphablendmodeex               d2_setalphablendmodeex_to_Log
#define  d2_setalphablendmodeex_from_Log      d2_setalphablendmodeex
#define  d2_setalphagradient                  d2_setalphagradient_to_Log
#define  d2_setalphagradient_from_Log         d2_setalphagradient
#define  d2_setclipgradient                   d2_setclipgradient_to_Log
#define  d2_setclipgradient_from_Log          d2_setclipgradient
#define  d2_setalphamode                      d2_setalphamode_to_Log
#define  d2_setalphamode_from_Log             d2_setalphamode
#define  d2_setantialiasing                   d2_setantialiasing_to_Log
#define  d2_setantialiasing_from_Log          d2_setantialiasing
#define  d2_setpatternalpha                   d2_setpatternalpha_to_Log
#define  d2_setpatternalpha_from_Log          d2_setpatternalpha
#define  d2_setfillmode                       d2_setfillmode_to_Log
#define  d2_setfillmode_from_Log              d2_setfillmode
#define  d2_setpattern                        d2_setpattern_to_Log
#define  d2_setpattern_from_Log               d2_setpattern
#define  d2_setpatternparam                   d2_setpatternparam_to_Log
#define  d2_setpatternparam_from_Log          d2_setpatternparam
#define  d2_setpatternmode                    d2_setpatternmode_to_Log
#define  d2_setpatternmode_from_Log           d2_setpatternmode
#define  d2_setpatternsize                    d2_setpatternsize_to_Log
#define  d2_setpatternsize_from_Log           d2_setpatternsize
#define  d2_setlinecap                        d2_setlinecap_to_Log
#define  d2_setlinecap_from_Log               d2_setlinecap
#define  d2_setlinejoin                       d2_setlinejoin_to_Log
#define  d2_setlinejoin_from_Log              d2_setlinejoin
#define  d2_setlinepattern                    d2_setlinepattern_to_Log
#define  d2_setlinepattern_from_Log           d2_setlinepattern
#define  d2_setmiterlimit                     d2_setmiterlimit_to_Log
#define  d2_setmiterlimit_from_Log            d2_setmiterlimit
#define  d2_settexture                        d2_settexture_to_Log
#define  d2_settexture_from_Log               d2_settexture
#define  d2_settexturemode                    d2_settexturemode_to_Log
#define  d2_settexturemode_from_Log           d2_settexturemode
#define  d2_settextureoperation               d2_settextureoperation_to_Log
#define  d2_settextureoperation_from_Log      d2_settextureoperation
#define  d2_settexopparam                     d2_settexopparam_to_Log
#define  d2_settexopparam_from_Log            d2_settexopparam
#define  d2_settexturemapping                 d2_settexturemapping_to_Log
#define  d2_settexturemapping_from_Log        d2_settexturemapping
#define  d2_settexelcenter                    d2_settexelcenter_to_Log
#define  d2_settexelcenter_from_Log           d2_settexelcenter
#define  d2_settexclut                        d2_settexclut_to_Log
#define  d2_settexclut_from_Log               d2_settexclut
#define  d2_settexclut_part                   d2_settexclut_part_to_Log
#define  d2_settexclut_part_from_Log          d2_settexclut_part
#define  d2_writetexclut_direct               d2_writetexclut_direct_to_Log
#define  d2_writetexclut_direct_from_Log      d2_writetexclut_direct
#define  d2_settexclut_offset                 d2_settexclut_offset_to_Log
#define  d2_settexclut_offset_from_Log        d2_settexclut_offset
#define  d2_settexclut_format                 d2_settexclut_format_to_Log
#define  d2_settexclut_format_from_Log        d2_settexclut_format
#define  d2_setcolorkey                       d2_setcolorkey_to_Log
#define  d2_setcolorkey_from_Log              d2_setcolorkey
#define  d2_setcircleextend                   d2_setcircleextend_to_Log
#define  d2_setcircleextend_from_Log          d2_setcircleextend
#define  d2_getcolor                          d2_getcolor_to_Log
#define  d2_getcolor_from_Log                 d2_getcolor
#define  d2_getalpha                          d2_getalpha_to_Log
#define  d2_getalpha_from_Log                 d2_getalpha
#define  d2_getalphaex                        d2_getalphaex_to_Log
#define  d2_getalphaex_from_Log               d2_getalphaex
#define  d2_getblur                           d2_getblur_to_Log
#define  d2_getblur_from_Log                  d2_getblur
#define  d2_getblendmodesrc                   d2_getblendmodesrc_to_Log
#define  d2_getblendmodesrc_from_Log          d2_getblendmodesrc
#define  d2_getblendmodedst                   d2_getblendmodedst_to_Log
#define  d2_getblendmodedst_from_Log          d2_getblendmodedst
#define  d2_getalphablendmodesrc              d2_getalphablendmodesrc_to_Log
#define  d2_getalphablendmodesrc_from_Log     d2_getalphablendmodesrc
#define  d2_getalphablendmodedst              d2_getalphablendmodedst_to_Log
#define  d2_getalphablendmodedst_from_Log     d2_getalphablendmodedst
#define  d2_getalphablendmodeflags            d2_getalphablendmodeflags_to_Log
#define  d2_getalphablendmodeflags_from_Log   d2_getalphablendmodeflags
#define  d2_getalphamode                      d2_getalphamode_to_Log
#define  d2_getalphamode_from_Log             d2_getalphamode
#define  d2_getantialiasing                   d2_getantialiasing_to_Log
#define  d2_getantialiasing_from_Log          d2_getantialiasing
#define  d2_getpatternalpha                   d2_getpatternalpha_to_Log
#define  d2_getpatternalpha_from_Log          d2_getpatternalpha
#define  d2_getfillmode                       d2_getfillmode_to_Log
#define  d2_getfillmode_from_Log              d2_getfillmode
#define  d2_getpattern                        d2_getpattern_to_Log
#define  d2_getpattern_from_Log               d2_getpattern
#define  d2_getpatternmode                    d2_getpatternmode_to_Log
#define  d2_getpatternmode_from_Log           d2_getpatternmode
#define  d2_getpatternsize                    d2_getpatternsize_to_Log
#define  d2_getpatternsize_from_Log           d2_getpatternsize
#define  d2_getlinecap                        d2_getlinecap_to_Log
#define  d2_getlinecap_from_Log               d2_getlinecap
#define  d2_getlinejoin                       d2_getlinejoin_to_Log
#define  d2_getlinejoin_from_Log              d2_getlinejoin
#define  d2_gettextureoperationa              d2_gettextureoperationa_to_Log
#define  d2_gettextureoperationa_from_Log     d2_gettextureoperationa
#define  d2_gettextureoperationr              d2_gettextureoperationr_to_Log
#define  d2_gettextureoperationr_from_Log     d2_gettextureoperationr
#define  d2_gettextureoperationg              d2_gettextureoperationg_to_Log
#define  d2_gettextureoperationg_from_Log     d2_gettextureoperationg
#define  d2_gettextureoperationb              d2_gettextureoperationb_to_Log
#define  d2_gettextureoperationb_from_Log     d2_gettextureoperationb
#define  d2_gettexopparam1                    d2_gettexopparam1_to_Log
#define  d2_gettexopparam1_from_Log           d2_gettexopparam1
#define  d2_gettexopparam2                    d2_gettexopparam2_to_Log
#define  d2_gettexopparam2_from_Log           d2_gettexopparam2
#define  d2_renderbox                         d2_renderbox_to_Log
#define  d2_renderbox_from_Log                d2_renderbox
#define  d2_renderline                        d2_renderline_to_Log
#define  d2_renderline_from_Log               d2_renderline
#define  d2_rendertri                         d2_rendertri_to_Log
#define  d2_rendertri_from_Log                d2_rendertri
#define  d2_renderquad                        d2_renderquad_to_Log
#define  d2_renderquad_from_Log               d2_renderquad
#define  d2_rendercircle                      d2_rendercircle_to_Log
#define  d2_rendercircle_from_Log             d2_rendercircle
#define  d2_renderwedge                       d2_renderwedge_to_Log
#define  d2_renderwedge_from_Log              d2_renderwedge
#define  d2_renderline2                       d2_renderline2_to_Log
#define  d2_renderline2_from_Log              d2_renderline2
#define  d2_renderpolyline                    d2_renderpolyline_to_Log
#define  d2_renderpolyline_from_Log           d2_renderpolyline
#define  d2_renderpolyline2                   d2_renderpolyline2_to_Log
#define  d2_renderpolyline2_from_Log          d2_renderpolyline2
#define  d2_rendertrilist                     d2_rendertrilist_to_Log
#define  d2_rendertrilist_from_Log            d2_rendertrilist
#define  d2_rendertrifan                      d2_rendertrifan_to_Log
#define  d2_rendertrifan_from_Log             d2_rendertrifan
#define  d2_rendertristrip                    d2_rendertristrip_to_Log
#define  d2_rendertristrip_from_Log           d2_rendertristrip
#define  d2_renderpolygon                     d2_renderpolygon_to_Log
#define  d2_renderpolygon_from_Log            d2_renderpolygon
#define  d2_setblitsrc                        d2_setblitsrc_to_Log
#define  d2_setblitsrc_from_Log               d2_setblitsrc
#define  d2_blitcopy                          d2_blitcopy_to_Log
#define  d2_blitcopy_from_Log                 d2_blitcopy
#define  d2_setperfcountevent                 d2_setperfcountevent_to_Log
#define  d2_setperfcountevent_from_Log        d2_setperfcountevent
#define  d2_setperfcountvalue                 d2_setperfcountvalue_to_Log
#define  d2_setperfcountvalue_from_Log        d2_setperfcountvalue
#define  d2_getperfcountvalue                 d2_getperfcountvalue_to_Log
#define  d2_getperfcountvalue_from_Log        d2_getperfcountvalue
#define  d2_utility_maptriangle               d2_utility_maptriangle_to_Log
#define  d2_utility_maptriangle_from_Log      d2_utility_maptriangle
#define  d2_utility_perspectivewarp           d2_utility_perspectivewarp_to_Log
#define  d2_utility_perspectivewarp_from_Log  d2_utility_perspectivewarp
#define  d2_utility_fbblitcopy                d2_utility_fbblitcopy_to_Log
#define  d2_utility_fbblitcopy_from_Log       d2_utility_fbblitcopy
#define  d2_rendercircle_no_hilimiterprecision           d2_rendercircle_no_hilimiterprecision_to_Log
#define  d2_rendercircle_no_hilimiterprecision_from_Log  d2_rendercircle_no_hilimiterprecision
#define  d2_executedlist                      d2_executedlist_to_Log
#define  d2_executedlist_from_Log             d2_executedlist
#define  d2_adddlist                          d2_adddlist_to_Log
#define  d2_adddlist_from_Log                 d2_adddlist


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/*---------------------------------------------------------------------------
 * basic functions */

D2_EXTERN d2_s32         d2_getversion_to_Log( void );
D2_EXTERN const d2_char *d2_getversionstring_to_Log( void );
D2_EXTERN d2_device *    d2_opendevice_to_Log( d2_u32 flags );
D2_EXTERN d2_s32         d2_closedevice_to_Log( d2_device *handle );
D2_EXTERN d2_s32         d2_geterror_to_Log( const d2_device *handle );
D2_EXTERN const d2_char *d2_geterrorstring_to_Log( const d2_device *handle );
D2_EXTERN const d2_char *d2_translateerror_to_Log( d2_s32 errorcode );
D2_EXTERN d2_s32         d2_inithw_to_Log( d2_device *handle, d2_u32 flags );
D2_EXTERN d2_s32         d2_deinithw_to_Log( d2_device *handle );
D2_EXTERN d1_device *    d2_level1interface_to_Log( const d2_device *handle );
D2_EXTERN d2_u32         d2_getrevisionhw_to_Log( const d2_device *handle );
D2_EXTERN const d2_char* d2_getrevisionstringhw_to_Log( const d2_device *handle);
D2_EXTERN d2_s32         d2_setdlistblocksize_to_Log(d2_device *handle, d2_u32 size);
D2_EXTERN d2_u32         d2_getdlistblocksize_to_Log(const d2_device *handle);
D2_EXTERN d2_u32         d2_getdlistblockcount_to_Log(d2_device *handle);
D2_EXTERN d2_s32         d2_commandspending_to_Log(d2_device *handle);
D2_EXTERN d2_s32         d2_lowlocalmemmode_to_Log(d2_device *handle, d2_u32 dlistblockfactor, d2_u32 dlistblocks);

/*---------------------------------------------------------------------------
 * context management */

D2_EXTERN d2_context * d2_newcontext_to_Log( d2_device *handle );
D2_EXTERN d2_s32       d2_freecontext_to_Log( d2_device *handle, d2_context *ctx );
D2_EXTERN d2_s32       d2_selectcontext_to_Log( d2_device *handle, d2_context *ctx );
D2_EXTERN d2_s32       d2_solidcontext_to_Log( d2_device *handle, d2_context *ctx );
D2_EXTERN d2_s32       d2_outlinecontext_to_Log( d2_device *handle, d2_context *ctx );
D2_EXTERN d2_context * d2_getcontext_to_Log( d2_device *handle, d2_s32 mode );

/*---------------------------------------------------------------------------
 * device */

D2_EXTERN d2_s32 d2_framebuffer_to_Log( d2_device *handle, void *ptr, d2_s32 pitch, d2_u32 width, d2_u32 height, d2_s32 format );
D2_EXTERN d2_s32 d2_cliprect_to_Log( d2_device *handle, d2_border xmin, d2_border ymin, d2_border xmax, d2_border ymax );
D2_EXTERN d2_s32 d2_flushframe_to_Log( d2_device *handle );
D2_EXTERN d2_s32 d2_startframe_to_Log( d2_device *handle );
D2_EXTERN d2_s32 d2_endframe_to_Log( d2_device *handle );
D2_EXTERN d2_s32 d2_relocateframe_to_Log( d2_device *handle, const void *ptr );
D2_EXTERN d2_s32 d2_gethaspreviousframe_to_Log( d2_device *handle );
D2_EXTERN d2_s32 d2_getisemptydlistentry_to_Log( d2_device *handle );
D2_EXTERN d2_s32 d2_clear_to_Log( d2_device *handle, d2_color color );
D2_EXTERN d2_s32 d2_getcliprect_to_Log( d2_device *handle, d2_border *xmin, d2_border *ymin, d2_border *xmax, d2_border *ymax );
D2_EXTERN d2_s32 d2_getframebuffer_to_Log( d2_device *handle, void** ptr, d2_s32* pitch, d2_u32* width, d2_u32* height, d2_s32* format);

/*---------------------------------------------------------------------------
 * device global attributes */

D2_EXTERN d2_s32 d2_selectrendermode_to_Log( d2_device *handle, d2_u32 mode );
D2_EXTERN d2_s32 d2_outlinewidth_to_Log( d2_device *handle, d2_width width );
D2_EXTERN d2_s32 d2_shadowoffset_to_Log( d2_device *handle, d2_point x, d2_point y );
D2_EXTERN d2_u32 d2_getrendermode_to_Log( const d2_device *handle );
D2_EXTERN d2_s32 d2_layermerge_to_Log( d2_device *handle );

/*---------------------------------------------------------------------------
 * renderbuffer management */

D2_EXTERN d2_renderbuffer * d2_newrenderbuffer_to_Log( d2_device *handle, d2_u32 initialsize, d2_u32 stepsize );
D2_EXTERN d2_s32            d2_freerenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer );
D2_EXTERN d2_s32            d2_selectrenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer );
D2_EXTERN d2_s32            d2_executerenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer, d2_u32 flags );
D2_EXTERN d2_renderbuffer * d2_getrenderbuffer_to_Log( d2_device *handle, d2_s32 index );
D2_EXTERN d2_s32            d2_dumprenderbuffer_to_Log( d2_device *handle, d2_renderbuffer *buffer, void **rdata, d2_s32 *rsize );
D2_EXTERN d2_u32            d2_getrenderbuffersize_to_Log(d2_device *handle, d2_renderbuffer *rb);
D2_EXTERN d2_s32            d2_freedumpedbuffer_to_Log( d2_device *handle, void *data );

/*---------------------------------------------------------------------------
 * context attribute writes */

D2_EXTERN d2_s32 d2_setcolor_to_Log( d2_device *handle, d2_s32 index, d2_color color );
D2_EXTERN d2_s32 d2_setalpha_to_Log( d2_device *handle, d2_alpha alpha );
D2_EXTERN d2_s32 d2_setalphaex_to_Log( d2_device *handle, d2_s32 index, d2_alpha alpha );
D2_EXTERN d2_s32 d2_setblur_to_Log( d2_device *handle, d2_width blur );
D2_EXTERN d2_s32 d2_setblendmode_to_Log( d2_device *handle, d2_u32 srcfactor, d2_u32 dstfactor );
D2_EXTERN d2_s32 d2_setalphablendmode_to_Log( d2_device *handle, d2_u32 srcfactor, d2_u32 dstfactor );
D2_EXTERN d2_s32 d2_setalphablendmodeex_to_Log( d2_device *handle, d2_u32 srcfactor, d2_u32 dstfactor, d2_u32 blendflags );
D2_EXTERN d2_s32 d2_setalphagradient_to_Log( d2_device *handle, d2_s32 index, d2_point x, d2_point y, d2_point dx, d2_point dy );
D2_EXTERN d2_s32 d2_setclipgradient_to_Log( d2_device *handle, d2_s32 index, d2_point x, d2_point y, d2_s32 nx, d2_s32 ny, d2_u32 flags );
D2_EXTERN d2_s32 d2_setalphamode_to_Log( d2_device *handle, d2_u32 mode );
D2_EXTERN d2_s32 d2_setantialiasing_to_Log( d2_device *handle, d2_s32 enable );
D2_EXTERN d2_s32 d2_setpatternalpha_to_Log( d2_device *handle, d2_s32 index, d2_alpha alpha );
D2_EXTERN d2_s32 d2_setfillmode_to_Log( d2_device *handle, d2_u32 mode );
D2_EXTERN d2_s32 d2_setpattern_to_Log( d2_device *handle, d2_pattern pattern );
D2_EXTERN d2_s32 d2_setpatternparam_to_Log( d2_device *handle, d2_point x, d2_point y, d2_width dx, d2_width dy );
D2_EXTERN d2_s32 d2_setpatternmode_to_Log( d2_device *handle, d2_u32 mode );
D2_EXTERN d2_s32 d2_setpatternsize_to_Log( d2_device *handle, d2_s32 size );
D2_EXTERN d2_s32 d2_setlinecap_to_Log( d2_device *handle, d2_u32 mode );
D2_EXTERN d2_s32 d2_setlinejoin_to_Log( d2_device *handle, d2_u32 mode );
D2_EXTERN d2_s32 d2_setlinepattern_to_Log( d2_device *handle, d2_width scale, d2_s32 offset );
D2_EXTERN d2_s32 d2_setmiterlimit_to_Log( d2_device *handle, d2_width miter );
D2_EXTERN d2_s32 d2_settexture_to_Log( d2_device *handle, void *ptr, d2_s32 pitch, d2_s32 width, d2_s32 height, d2_u32 format );
D2_EXTERN d2_s32 d2_settexturemode_to_Log( d2_device *handle, d2_u32 mode );
D2_EXTERN d2_s32 d2_settextureoperation_to_Log( d2_device *handle, d2_u8 amode, d2_u8 rmode, d2_u8 gmode, d2_u8 bmode );
D2_EXTERN d2_s32 d2_settexopparam_to_Log( d2_device *handle, d2_u32 index, d2_u32 p1, d2_u32 p2 );
D2_EXTERN d2_s32 d2_settexturemapping_to_Log( d2_device *handle, d2_point x, d2_point y, d2_s32 u0, d2_s32 v0, d2_s32 dxu, d2_s32 dyu, d2_s32 dxv, d2_s32 dyv );
D2_EXTERN d2_s32 d2_settexelcenter_to_Log( d2_device *handle, d2_point x, d2_point y );
D2_EXTERN d2_s32 d2_settexclut_to_Log( d2_device *handle, d2_color* clut );
D2_EXTERN d2_s32 d2_settexclut_part_to_Log( d2_device *handle, const d2_color* clut_part, d2_u32 start_index, d2_u32 length );
D2_EXTERN d2_s32 d2_writetexclut_direct_to_Log( d2_device *handle, const d2_color* clut_part, d2_u32 start_index, d2_u32 length );
D2_EXTERN d2_s32 d2_settexclut_offset_to_Log( d2_device *handle, d2_u32 offset );
D2_EXTERN d2_s32 d2_settexclut_format_to_Log(  d2_device *handle, d2_u32 format );
D2_EXTERN d2_s32 d2_setcolorkey_to_Log( d2_device *handle, d2_s32 enable, d2_color color_key );
D2_EXTERN d2_s32 d2_setcircleextend_to_Log( d2_device *handle, d2_width offset );

/*---------------------------------------------------------------------------
 * context attritbute reads */

D2_EXTERN d2_color d2_getcolor_to_Log( d2_device *handle, d2_s32 index );
D2_EXTERN d2_alpha d2_getalpha_to_Log( d2_device *handle );
D2_EXTERN d2_alpha d2_getalphaex_to_Log( d2_device *handle, d2_s32 index );
D2_EXTERN d2_width d2_getblur_to_Log( d2_device *handle );
D2_EXTERN d2_u32 d2_getblendmodesrc_to_Log( d2_device *handle );
D2_EXTERN d2_u32 d2_getblendmodedst_to_Log( d2_device *handle );
D2_EXTERN d2_u32 d2_getalphablendmodesrc_to_Log( d2_device *handle );
D2_EXTERN d2_u32 d2_getalphablendmodedst_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_getalphablendmodeflags_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_getalphamode_to_Log( d2_device *handle );
D2_EXTERN d2_s32 d2_getantialiasing_to_Log( d2_device *handle );
D2_EXTERN d2_alpha d2_getpatternalpha_to_Log( d2_device *handle, d2_s32 index );
D2_EXTERN d2_u8  d2_getfillmode_to_Log( d2_device *handle );
D2_EXTERN d2_pattern d2_getpattern_to_Log( d2_device *handle );
D2_EXTERN d2_u32 d2_getpatternmode_to_Log( d2_device *handle );
D2_EXTERN d2_s32 d2_getpatternsize_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_getlinecap_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_getlinejoin_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_gettextureoperationa_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_gettextureoperationr_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_gettextureoperationg_to_Log( d2_device *handle );
D2_EXTERN d2_u8  d2_gettextureoperationb_to_Log( d2_device *handle );
D2_EXTERN d2_alpha d2_gettexopparam1_to_Log( d2_device *handle, d2_u32 index );
D2_EXTERN d2_alpha d2_gettexopparam2_to_Log( d2_device *handle, d2_u32 index );

/*---------------------------------------------------------------------------
 * rendering commands */

D2_EXTERN d2_s32 d2_renderbox_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_width w, d2_width h );
D2_EXTERN d2_s32 d2_renderline_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_width w, d2_u32 flags );
D2_EXTERN d2_s32 d2_rendertri_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_point x3, d2_point y3, d2_u32 flags );
D2_EXTERN d2_s32 d2_renderquad_to_Log( d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_point x3, d2_point y3, d2_point x4, d2_point y4, d2_u32 flags );
D2_EXTERN d2_s32 d2_rendercircle_to_Log( d2_device *handle, d2_point x, d2_point y, d2_width r, d2_width w );
D2_EXTERN d2_s32 d2_renderwedge_to_Log( d2_device *handle, d2_point x, d2_point y, d2_width r, d2_width w, d2_s32 nx1, d2_s32 ny1, d2_s32 nx2, d2_s32 ny2, d2_u32 flags );
D2_EXTERN d2_s32 d2_renderline2_to_Log(  d2_device *handle, d2_point x1, d2_point y1, d2_point x2, d2_point y2, d2_width w1, d2_width w2, d2_u32 flags );

D2_EXTERN d2_s32 d2_renderpolyline_to_Log( d2_device *handle, const d2_point *data, d2_u32 count, d2_width w, d2_u32 flags);
D2_EXTERN d2_s32 d2_renderpolyline2_to_Log( d2_device *handle, const d2_point *data, d2_u32 count, const d2_width *w, d2_u32 flags);
D2_EXTERN d2_s32 d2_rendertrilist_to_Log( d2_device *handle, const d2_point *data, const d2_u32 *flags, d2_u32 count);
D2_EXTERN d2_s32 d2_rendertrifan_to_Log( d2_device *handle, const d2_point *data, const d2_u32 *flags, d2_u32 count);
D2_EXTERN d2_s32 d2_rendertristrip_to_Log( d2_device *handle, const d2_point *data, const d2_u32 *flags, d2_u32 count);
D2_EXTERN d2_s32 d2_renderpolygon_to_Log( d2_device *handle, const d2_point *data, d2_u32 count, d2_u32 flags);

/*---------------------------------------------------------------------------
 * blit attributes write */

D2_EXTERN d2_s32 d2_setblitsrc_to_Log( d2_device *handle, void *ptr, d2_s32 pitch, d2_s32 width, d2_s32 height, d2_u32 format );

/*---------------------------------------------------------------------------
 * blit rendering functions */

D2_EXTERN d2_s32 d2_blitcopy_to_Log( d2_device *handle, d2_s32 srcwidth, d2_s32 srcheight, d2_blitpos srcx, d2_blitpos srcy, d2_width dstwidth, d2_width dstheight, d2_point dstx, d2_point dsty, d2_u32 flags );

/*---------------------------------------------------------------------------
 * performance measurement */
D2_EXTERN d2_s32 d2_setperfcountevent_to_Log( d2_device *handle, d2_u32 counter, d2_u32 event );
D2_EXTERN d2_s32 d2_setperfcountvalue_to_Log( d2_device *handle, d2_u32 counter, d2_slong value );
D2_EXTERN d2_slong d2_getperfcountvalue_to_Log( d2_device *handle, d2_u32 counter );

/*---------------------------------------------------------------------------
 * Utility Functions */

d2_s32 d2_utility_maptriangle_to_Log( d2_device *handle, const d2_f32 *points, const d2_f32 *uvs );
d2_s32 d2_utility_perspectivewarp_to_Log( d2_device *handle, d2_u16 srcwidth, d2_u16 srcheight, d2_s16 srcx, d2_s16 srcy, d2_s16 dstwidth, d2_s16 dstheight, d2_s16 dstx, d2_s16 dsty, d2_u16 wt );
d2_s32 d2_utility_fbblitcopy_to_Log( d2_device *handle, d2_u16 width, d2_u16 height, d2_blitpos srcx, d2_blitpos srcy, d2_blitpos dstx, d2_blitpos dsty, d2_u32 flags);
void d2_rendercircle_no_hilimiterprecision_to_Log( d2_device *handle, d2_u32 flag );



/*---------------------------------------------------------------------------
 * Dlist Functions */

d2_s32 d2_executedlist_to_Log( d2_device *handle, const void *address, d2_u32 flags );
d2_s32 d2_adddlist_to_Log( d2_device *handle, void *address, d2_s32 size, d2_u32 flags );


/***********************************************************************
* End of File:
************************************************************************/
#endif  /* USE_CONFIG_REDIRECT_DRW_API */
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* DAVE_LOG_H */
