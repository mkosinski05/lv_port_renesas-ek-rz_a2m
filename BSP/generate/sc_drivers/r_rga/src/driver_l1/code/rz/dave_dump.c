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
/*******************************************************************************
****************************************************************************
* File: dave_dump.c
*    Register Dump of D/AVE 2D.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_drw.h"
#include  <stdio.h>
#include  "dave_base_rz_errnum.h"
#include  "dave_driver.h"
#include  "dave_types.h"
#include  "dave_intern.h"
#include  "dave_dlist.h"
#include  "r_memory_map.h"
#include  "rz_co.h"
#ifdef  USE_LIST_RZA2M
#include  "rza_io_regrw.h"
#endif

#ifdef  USE_CONFIG_REDIRECT_REGISTER_ACCESS
#define  GS_N  "\r\n"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
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
uint32_t  g_DRW_RegistersCopy[64];
void  gs_PrintRegisterIndex( int_fast32_t  in_Offset,  uint32_t  in_WritingValue );
void  gs_PrintSubsequentIndexOfSpecialIndex( uint_fast8_t  in_SubsequentIndex );


/***********************************************************************
* Function: R_DRW_PrintRegisters
************************************************************************/
void  R_DRW_PrintRegisters()
{
    #ifdef  USE_CONFIG_DUMP_DRW_REGISTER
        printf( "DRW.CONTROL2    = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_CONTROL2    ] );
        printf( "DRW.CACHECTL    = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_CACHECTL    ] );
        printf( "DRW.TEXPITCH    = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_TEXPITCH    ] );
        printf( "DRW.TEXMASK     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_TEXMASK     ] );
        printf( "DRW.COLOR1      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_COLOR1      ] );
        printf( "DRW.COLOR2      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_COLOR2      ] );
        printf( "DRW.LUXADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LUXADD      ] );
        printf( "DRW.LVYADDI     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LVYADDI     ] );
        printf( "DRW.SIZE        = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_SIZE        ] );
        printf( "DRW.PITCH       = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_PITCH       ] );
        printf( "DRW.PERFTRIGGER = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_PERFTRIGGER ] );
        printf( "DRW.TEXORIGIN   = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_TEXORIGIN   ] );
        printf( "DRW.ORIGIN      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_ORIGIN      ] );
        printf( ""GS_N );
        printf( "DRW.CONTROL     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_CONTROL     ] );
        printf( "DRW.L1START     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L1START     ] );
        printf( "DRW.L2START     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L2START     ] );
        printf( "DRW.L3START     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L3START     ] );
        printf( "DRW.L4START     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L4START     ] );
        printf( "DRW.L5START     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L5START     ] );
        printf( "DRW.L6START     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L6START     ] );
        printf( "DRW.L1XADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L1XADD      ] );
        printf( "DRW.L2XADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L2XADD      ] );
        printf( "DRW.L3XADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L3XADD      ] );
        printf( "DRW.L4XADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L4XADD      ] );
        printf( "DRW.L5XADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L5XADD      ] );
        printf( "DRW.L6XADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L6XADD      ] );
        printf( "DRW.L1YADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L1YADD      ] );
        printf( "DRW.L2YADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L2YADD      ] );
        printf( "DRW.L3YADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L3YADD      ] );
        printf( "DRW.L4YADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L4YADD      ] );
        printf( "DRW.L5YADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L5YADD      ] );
        printf( "DRW.L6YADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L6YADD      ] );
        printf( "DRW.L1BAND      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L1BAND      ] );
        printf( "DRW.L2BAND      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_L2BAND      ] );
        printf( "DRW.PATTERN     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_PATTERN     ] );
        printf( "DRW.LUSTART     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LUSTART     ] );
        printf( "DRW.LUXADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LUXADD      ] );
        printf( "DRW.LUYADD      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LUYADD      ] );
        printf( "DRW.LVSTARTI    = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LVSTARTI    ] );
        printf( "DRW.LVSTARTF    = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LVSTARTF    ] );
        printf( "DRW.LVXADDI     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LVXADDI     ] );
        printf( "DRW.LVYADDI     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LVYADDI     ] );
        printf( "DRW.LVYXADDF    = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_LVYXADDF    ] );
        printf( "DRW.IRQCTL      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_IRQCTL      ] );
        printf( "DRW.TEXCLUT     = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_TEXCLUT     ] );
        printf( "DRW.TEXCLUT_ADDR= 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_TEXCLUT_ADDR] );
        printf( "DRW.TEXCLUT_DATA= 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_TEXCLUT_DATA] );
        printf( "DRW.TEXCLUT_OFFSET = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_TEXCLUT_OFFSET ] );
        printf( "DRW.COLKEY      = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_COLKEY      ] );
        printf( "DRW.QUANTITY    = 0x%08X"GS_N,  g_DRW_RegistersCopy[ D2_QUANTITY    ] );
        printf( ""GS_N );
    #endif /* USE_CONFIG_DUMP_DRW_REGISTER */
}


/***********************************************************************
* Function: R_DRW_DUMP_OnWriteToRegister
*
* Description:
*    Also, to changing by display list is supported.
************************************************************************/
void  R_DRW_DUMP_OnWriteToRegister( void*  in_BaseAddress,  int_fast32_t  in_Offset,  uint32_t  in_WritingValue )
{
    errnum_t  e;

    if ( !( in_Offset >= 0  &&  in_Offset < R_COUNT_OF( g_DRW_RegistersCopy ) ))
        { e=E_DRW_OTHER_2;  goto fin; }  /* Error: <Private/> */


    /* g_DRW_RegistersCopy[] = in_WritingValue */
    g_DRW_RegistersCopy[ in_Offset ] = in_WritingValue;


    /* g_DRW_RegistersCopy[] = ... : From "display_list_uncached_start_address" */
    if ( in_Offset == D2_DLISTSTART ) {
        uintptr_t        display_list_physical_start_address = in_WritingValue;
        uint32_t*        display_list_uncached_start_address;
        uintptr_t        physical_address_from_uncached;
        int_fast32_t     index_index;
        int_fast32_t     over_index_index;
        int_fast32_t     offset;
        d2_dlist_entry*  display_list_entry;
        bool_t           is_display_list_end = false;
        enum {           num_0x80_gap_index = 0x80 };  /* nop code */
        enum {           num_0xFF_special_index = 0xFF };


        #if  defined( USE_CONFIG_DUMP_DRW_REGISTER ) || defined( USE_CONFIG_PRINT_DISPLAY_LIST )
            R_DRW_PrintRegisters();
        #endif
        #ifdef  USE_CONFIG_PRINT_DISPLAY_LIST
            printf( "Start of display list ==========================="GS_N );
        #endif


        /* display_list_uncached_start_address = ... */
        e= R_MEMORY_ToUncachedAddress(
            (const volatile void*) display_list_physical_start_address,
            &display_list_uncached_start_address ); if(e){goto fin;}
        e= R_MEMORY_ToPhysicalAddress(
            display_list_uncached_start_address,
            &physical_address_from_uncached ); if(e){goto fin;}
        if ( !( physical_address_from_uncached == display_list_physical_start_address ))
            { e=E_DRW_OTHER_5;  goto fin; }  /* Error: <Private/> */


        /* Iterate in display list */
        display_list_entry = (d2_dlist_entry*) display_list_uncached_start_address;
        do {
            bool_t  is_special_index = ( display_list_entry->address.array[ 0 ] == num_0xFF_special_index );
            bool_t  is_normal_case = ! is_special_index;
            enum {  num_4_count_of_index_index = 4 };

            if ( is_normal_case ) {
                for ( index_index = 0;  index_index < num_4_count_of_index_index;  index_index += 1 ) {
                    int_fast32_t  index = display_list_entry->address.array[ index_index ];
                    bool_t        is_register_index = ( index >= 0x00  &&  index <= 0x7F );


                    /* g_DRW_RegistersCopy[] = ... */
                    if ( is_register_index ) {
                        uint32_t  writing_value;

                        offset = index;
                        if ( !( offset >= 0  &&  offset < R_COUNT_OF( g_DRW_RegistersCopy ) ))
                            { e=E_DRW_OTHER_3;  goto fin; }  /* Error: <Private/> */
                        writing_value = display_list_entry->value[ index_index ];

                        g_DRW_RegistersCopy[ offset ] = writing_value;

                        #ifdef  USE_CONFIG_PRINT_DISPLAY_LIST
                            gs_PrintRegisterIndex( offset,  writing_value );
                        #endif
                    }


                    /* ... */
                    else if ( index == num_0x80_gap_index ) {
                        break;
                    }
                    else {
                        e=E_DRW_OTHER_4;  goto fin;  /* Error: <Private/> */
                    }
                }
                over_index_index = index_index;
            }
            else {  /* if "is_special_index" */
                uint_fast8_t  subsequent_index = display_list_entry->address.array[ 1 ];

                is_display_list_end = IS_BIT_SET( subsequent_index,  0x01u );
                over_index_index = 0;

                #ifdef  USE_CONFIG_PRINT_DISPLAY_LIST
                    gs_PrintSubsequentIndexOfSpecialIndex( subsequent_index );
                #endif
            }
            display_list_entry = (d2_dlist_entry*) &display_list_entry->value[ over_index_index ];
        } while ( ! is_display_list_end );


        #ifdef  USE_CONFIG_PRINT_DISPLAY_LIST
            printf( "End of display list ==========================="GS_N );
            R_DRW_PrintRegisters();
        #endif
    }


    #ifdef  USE_LIST_RZA2M
        /* RZA_IO_RegWrite_32 */
        RZA_IO_RegWrite_32( &((unsigned long *) in_BaseAddress )[ in_Offset ],  in_WritingValue,  0,  0xFFFFFFFFu );
    #endif
    e=0;
fin:
    R_ERROR_Set( e );
}


/***********************************************************************
* Variable: gs_RegisterNames
************************************************************************/
char_t*  gs_RegisterNames[] = {
    "D2_CONTROL",
    "D2_CONTROL2",
    "",
    "",
    "D2_L1START",
    "D2_L2START",
    "D2_L3START",
    "D2_L4START",
    "D2_L5START",
    "D2_L6START",
    "D2_L1XADD",
    "D2_L2XADD",
    "D2_L3XADD",
    "D2_L4XADD",
    "D2_L5XADD",
    "D2_L6XADD",
    "D2_L1YADD",
    "D2_L2YADD",
    "D2_L3YADD",
    "D2_L4YADD",
    "D2_L5YADD",
    "D2_L6YADD",
    "D2_L1BAND",
    "D2_L2BAND",
    "",
    "D2_COLOR1",
    "D2_COLOR2",
    "",
    "",
    "D2_PATTERN",
    "D2_SIZE",
    "D2_PITCH",
    "D2_ORIGIN",
    "",
    "",
    "",
    "D2_LUSTART",
    "D2_LUXADD",
    "D2_LUYADD",
    "D2_LVSTARTI",
    "D2_LVSTARTF",
    "D2_LVXADDI",
    "D2_LVYADDI",
    "D2_LVYXADDF",
    "",
    "D2_TEXPITCH",
    "D2_TEXMASK",
    "D2_TEXORIGIN",
    "D2_IRQCTL",
    "D2_CACHECTL",
    "D2_DLISTSTART",
    "D2_PERFCOUNT1",
    "D2_PERFCOUNT2",
    "D2_PERFTRIGGER",
    "D2_TEXCLUT",
    "D2_TEXCLUT_ADDR",
    "D2_TEXCLUT_DATA",
    "D2_TEXCLUT_OFFSET",
    "D2_COLKEY",
    "D2_QUANTITY",
};


/***********************************************************************
* Function: gs_PrintRegisterIndex
************************************************************************/
#ifdef  USE_CONFIG_PRINT_DISPLAY_LIST
void  gs_PrintRegisterIndex( int_fast32_t  in_Offset,  uint32_t  in_WritingValue )
{
    char_t*  register_name;

    if ( in_Offset >= 0  &&  in_Offset < R_COUNT_OF( gs_RegisterNames ) ) {
        printf( "DRW.%-17s = 0x%08X"GS_N,  gs_RegisterNames[ in_Offset ],  in_WritingValue );
    } else {
        printf( "DRW.%-17d = 0x%08X"GS_N,  in_Offset,  in_WritingValue );
    }
}
#endif


/***********************************************************************
* Function: gs_PrintSubsequentIndexOfSpecialIndex
************************************************************************/
#ifdef  USE_CONFIG_PRINT_DISPLAY_LIST
void  gs_PrintSubsequentIndexOfSpecialIndex( uint_fast8_t  in_SubsequentIndex )
{
    char_t*  bit_name[3];


    /* bit_name = ... */
    if ( IS_BIT_SET( in_SubsequentIndex,  0x01u ) ) {
        bit_name[0] = " DisplayListEnd";
    } else {
        bit_name[0] = "";
    }
    if ( IS_BIT_SET( in_SubsequentIndex,  0x02u ) ) {
        bit_name[1] = " FlushPipelineAndWait";
    } else {
        bit_name[1] = "";
    }
    if ( IS_BIT_SET( in_SubsequentIndex,  0x04u ) ) {
        bit_name[2] = " WaitForWriteBack";
    } else {
        bit_name[2] = "";
    }


    /* printf */
    printf( "SubsequentIndexOfSpecialIndex = 0x%02X%s%s%s"GS_N,
        in_SubsequentIndex,  bit_name[0],  bit_name[1],  bit_name[2] );
}
#endif


#endif  /* USE_CONFIG_REDIRECT_REGISTER_ACCESS */
