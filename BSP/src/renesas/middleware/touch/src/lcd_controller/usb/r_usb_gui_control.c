/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
 * File Name :    r_usb_gui_control.c
 * @file          r_usb_gui_control.c
 * @brief         display sprite for mouse cursor
 ******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "r_os_abstraction_api.h"
#include "iodefine.h"
#include "r_typedefs.h"
#include "lcd_panel.h"
#include "r_rvapi_vdc.h"
#include "r_rvapi_spea.h"
#include "r_usb_gui_control.h"
#include "r_cache_lld_rza2m.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define    ARROW_BUFFER_AREA     (32u * 32u)
#define    CAUTION_WINDOW_WIDTH  (576u)
#define    CAUTION_WINDOW_HEIGHT (160u)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void create_mouse_off_task(void);
static void mouse_off_task(void);

spea_window_conf_t spea_window_arrow   = { WINDOW_01, SPEA_ON, {  0,   0},  {32 ,32}, NULL };
spea_window_conf_t spea_window_caution = { WINDOW_00, SPEA_ON, {128, 188}, {544,128}, NULL };
static uint32_t    arrow_buffer[ARROW_BUFFER_AREA] __attribute__ ((section("Graphics_RAM")));
static uint32_t    malloc_caution_buffer;

volatile uint32_t  arrow_off_flg = 0;

/******************************************************************************
* Function Name: init_mouse
* Description  : initialize mouse cursor
* Arguments    : init_x
*              : init_y
*              : display_hw
*              : display_vw
* Return Value : -
******************************************************************************/
void init_mouse(uint16_t init_x, uint16_t init_y, uint16_t display_hw, uint16_t display_vw)
{
    vdc_error_t              error       = VDC_OK;
    spea_error_t             spea_error  = SPEA_OK;
    vdc_channel_t            vdc_ch      = VDC_CHANNEL_0;

    /* malloc address buff init */
    malloc_caution_buffer = 0;
    {
        gr_surface_disp_config_t gr_disp_cnf;

        /***********************************************************************/
        /* Create SPEA Layer ( VDC Graphics Layer 3) */
        /***********************************************************************/
        /* Layer ID */
        gr_disp_cnf.layer_id  = VDC_LAYER_ID_3_RD;
        /* Display Area */
        gr_disp_cnf.disp_area.hs_rel = 0u;
        gr_disp_cnf.disp_area.vs_rel = 0u;
        gr_disp_cnf.disp_area.hw_rel = display_hw;
        gr_disp_cnf.disp_area.vw_rel = display_vw;

        error = R_RVAPI_GraphCreateSurfaceSPEA(vdc_ch, &gr_disp_cnf);
    }

    if (VDC_OK == error)
    {
        /***********************************************************************/
        /* Setting SPEA Layer offset */
        /***********************************************************************/
        (void)R_RVAPI_WindowOffsetSPEA(VDC_LAYER_ID_3_RD, 0, 0);

        /***********************************************************************/
        /* Setting SPEA Layer for arrow */
        /***********************************************************************/

        /* Substitute image data into buffer */
        memcpy((void *)arrow_buffer, (void *)g_arrow_data, ARROW_SIZE);

        /* Substitute buffer */
        spea_window_arrow.buffer = (void *)arrow_buffer;

        /* Setting initialize position of mouse cursor*/
        spea_window_arrow.pos.x = init_x;
        spea_window_arrow.pos.y = init_y;

        spea_error = R_RVAPI_SetWindowSPEA(
            VDC_LAYER_ID_3_RD,
            spea_window_arrow.window_id,
            spea_window_arrow.sken,
            &spea_window_arrow.size,
            &spea_window_arrow.pos,
            spea_window_arrow.buffer);

        if(SPEA_OK == spea_error)
        {
            /* no return */
            (void)R_RVAPI_WindowUpdateSPEA(VDC_LAYER_ID_3_RD);
        }
    }

    /* Task create */
    create_mouse_off_task();

    arrow_off_flg = 0;

    return;
}   /* End of function init_mouse() */

/******************************************************************************
* Function Name: change_mouse_position
* Description  : change mouse cursor x/y coordinate
* Arguments    : init_x
*              : init_y
*              : display_hw
*              : display_vw
* Return Value : -
******************************************************************************/
void change_mouse_position(uint16_t mouse_x, uint16_t mouse_y, uint16_t display_hw, uint16_t display_vw)
{
    /* Odd positions are not supported in sprite engine. */
    mouse_x &= 0xFFFE;
    mouse_y &= 0xFFFE;

    /* Display on */
    spea_window_arrow.sken = SPEA_ON;

    /* Setting mouse cursor position */
    spea_window_arrow.pos.x = (display_hw - mouse_x);
    spea_window_arrow.pos.y = (display_vw - mouse_y);

    /* no return */
    (void)R_RVAPI_SetWindowSPEA(VDC_LAYER_ID_3_RD,
                                spea_window_arrow.window_id,
                                spea_window_arrow.sken,
                                &spea_window_arrow.size,
                                &spea_window_arrow.pos,
                                spea_window_arrow.buffer);

    /* no return */
    (void)R_RVAPI_WindowUpdateSPEA(VDC_LAYER_ID_3_RD);

    arrow_off_flg = 0;

    return;
}   /* End of function change_mouse_position() */

/******************************************************************************
* Function Name: display_off_mouse
* Description  : display off mouse cursor
* Arguments    : -
* Return Value : -
******************************************************************************/
void display_off_mouse(void)
{
    /* Display off */
    spea_window_arrow.sken  = SPEA_OFF;

    /* no return */
    (void)R_RVAPI_SetWindowSPEA(VDC_LAYER_ID_3_RD,
                                spea_window_arrow.window_id,
                                spea_window_arrow.sken,
                                &spea_window_arrow.size,
                                &spea_window_arrow.pos,
                                spea_window_arrow.buffer);

    /* no return */
    (void)R_RVAPI_WindowUpdateSPEA(VDC_LAYER_ID_3_RD);

    return;
}   /* End of function display_off_mouse() */

/******************************************************************************
* Function Name: display_on_mouse
* Description  : display on mouse cursor
* Arguments    : -
* Return Value : -
******************************************************************************/
void display_on_mouse(void)
{
    /* Display on */
    spea_window_arrow.sken = SPEA_ON;

    /* no return */
    (void)R_RVAPI_SetWindowSPEA(VDC_LAYER_ID_3_RD,
                                spea_window_arrow.window_id,
                                spea_window_arrow.sken,
                                NULL,
                                NULL,
                                spea_window_arrow.buffer);

    /* no return */
    (void)R_RVAPI_WindowUpdateSPEA(VDC_LAYER_ID_3_RD);

    arrow_off_flg = 0;

} /* End of function display_on_mouse */


/******************************************************************************
* Function Name: display_on_usbport
* Description  : display on caution dialog
* Arguments    : -
* Return Value : -
******************************************************************************/
void display_on_usbport(void)
{
    spea_error_t  spea_error = SPEA_OK;
    uint32_t caution_buffer;

    /* no  */
    malloc_caution_buffer = (uint32_t)R_OS_Malloc((CAUTION_WINDOW_WIDTH * CAUTION_WINDOW_HEIGHT * 4), R_MEMORY_REGION_DEFAULT);

    if( malloc_caution_buffer != NULL )
    {
        caution_buffer  = malloc_caution_buffer;
        caution_buffer += 7;
        caution_buffer &= ~(0x7);

        /* Substitute image data into buffer */
        memcpy((void *)caution_buffer, (void *)g_caution_data, CAUTION_SIZE);

        R_CACHE_L1DataCleanLine((void*)caution_buffer, (CAUTION_WINDOW_WIDTH * CAUTION_WINDOW_HEIGHT * 4));

        /* Substitute buffer */
        spea_window_caution.buffer = (void *)caution_buffer;

        /* Display on */
        spea_window_caution.sken  = SPEA_ON;
        spea_error = R_RVAPI_SetWindowSPEA(
            VDC_LAYER_ID_3_RD,
            spea_window_caution.window_id,
            spea_window_caution.sken,
            &spea_window_caution.size,
            &spea_window_caution.pos,
            spea_window_caution.buffer);

        if(SPEA_OK == spea_error)
        {
            /* no return */
            (void)R_RVAPI_WindowUpdateSPEA(VDC_LAYER_ID_3_RD);
        }
    }

    return;
}   /* End of function display_on_usbport() */

/******************************************************************************
* Function Name: display_off_usbport
* Description  : display off caution dialog
* Arguments    : -
* Return Value : -
******************************************************************************/
void display_off_usbport(void)
{
    spea_error_t  spea_error = SPEA_OK;

    if( malloc_caution_buffer != NULL )
    {
        /* Display off */
        spea_window_caution.sken  = SPEA_OFF;

        spea_error = R_RVAPI_SetWindowSPEA(
            VDC_LAYER_ID_3_RD,
            spea_window_caution.window_id,
            spea_window_caution.sken,
            &spea_window_caution.size,
            &spea_window_caution.pos,
            spea_window_caution.buffer);

        if(SPEA_OK == spea_error)
        {
            /* no return */
            (void)R_RVAPI_WindowUpdateSPEA(VDC_LAYER_ID_3_RD);
        }

        R_OS_TaskSleep(16);

        R_OS_Free((void *)&malloc_caution_buffer);
    }
    return;

}   /* End of function display_on_usbport() */

/***********************************************************************************************************************
 * Function Name: create_mouse_off_task
 * Description  : create mouse off task
 * Arguments    : -
 * Return Value : -
 ***********************************************************************************************************************/
static void create_mouse_off_task(void)
{
    os_task_t* p_graphics_task;

    printf("Mouse Off Task Start.\r\n");

    /* initialize NULL */
    p_graphics_task = NULL;

    /* argument3 NULL */
    p_graphics_task = R_OS_TaskCreate("CHANGE", mouse_off_task, NULL, R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, 7 );

    /* NULL check */
    if (NULL == p_graphics_task)
    {
        printf("Sprite Engine Driver can not be loaded, demo system does not work correctly\r\n");
    }
} /* End of function create_mouse_off_task */

/******************************************************************************
* Function Name   : mouse_off_task
* Description     : mouse off task of sprite engine
* Arguments       : -
* Return Value    : -
******************************************************************************/
static void mouse_off_task(void)
{
    while(1)
    {
        R_OS_TaskSleep(1000);
        if(arrow_off_flg < 3)
        {
            arrow_off_flg++;
            if(3 == arrow_off_flg)
            {
                display_off_mouse();
            }
        }
    }
} /* End of function mouse_off_task */

/*******************************************************************************
 End of function r_usb_gui_control.c
 ******************************************************************************/

/* End of File */
