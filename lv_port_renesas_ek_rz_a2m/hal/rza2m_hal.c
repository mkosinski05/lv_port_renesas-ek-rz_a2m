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

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine.h"
#include "r_cpg_drv_api.h"
#include "r_ostm_drv_api.h"
#include "r_scifa_drv_api.h"
#include "r_gpio_drv_api.h"
#include "r_startup_config.h"
#include "compiler_settings.h"
#include "r_os_abstraction_api.h"
#include "r_task_priority.h"
#include "version.h"
#include "tp_if.h"
#include "r_riic_drv_sc_cfg.h"
#include "r_riic_drv_api.h"
#include "lvgl/lvgl.h"
#include "lv_demos/src/lv_demos.h"

#define TP_INT_PRI              (30uL)
#define TP_TSK_PRI              (R_OS_TASK_MAIN_TASK_PRI+1)

#define TFT_PANEL_WIDTH        (800)
#define TFT_PANEL_HEIGHT       (480)

/* Externs from Graphic.cpp */
extern uint8_t framebuffer[2][800 * 480 * 2];
extern void GrpDrv_SetFrameBuffer(void * ptr);
extern void GRAPHIC_init_screen(void);

/* Touch state globals */
static lv_coord_t g_touch_x = 0;
static lv_coord_t g_touch_y = 0;
static lv_indev_state_t g_touch_state = LV_INDEV_STATE_REL;

static int_t gs_my_gpio_handle;
static const char_t * const sp_clear_screen = "\x1b[2J";
static const char_t * const sp_cursor_home  = "\x1b[H";
static int_t           an_tp_evt_id[6];

/* LVGL v9 Display Flush Callback */
static void rz_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
	LV_UNUSED(area);
    R_CACHE_L1DataCleanLine((void *)px_map, FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT);
    GrpDrv_SetFrameBuffer(px_map);
    lv_display_flush_ready(disp);
}

/* LVGL Touch Read Callback */
static void rz_touchpad_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    UNUSED(indev);
    data->point.x = g_touch_x;
    data->point.y = g_touch_y;
    data->state = g_touch_state;
}

static void tpevt_cb_func( int_t nId, TP_TouchEvent_st* psTouchEvt )
{
    UNUSED_PARAM(nId);
    uint32_t disp_x = psTouchEvt->sFinger[0].unPosX;
    uint32_t disp_y = psTouchEvt->sFinger[0].unPosY;

    g_touch_x = (lv_coord_t)(TFT_PANEL_WIDTH - (int)disp_x);
    g_touch_y = (lv_coord_t)(TFT_PANEL_HEIGHT - (int)disp_y);

    switch(psTouchEvt->sFinger[0].eState)
    {
        case TPEVT_ENTRY_DOWN:
        case TPEVT_ENTRY_MOVE:
            g_touch_state = LV_INDEV_STATE_PR;
            break;
        case TPEVT_ENTRY_UP:
        default:
            g_touch_state = LV_INDEV_STATE_REL;
            break;
    }
}

/* LVGL Main Application Task */
static void lvgl_task( void *pvParameters )
{
    (void)pvParameters;

    /* Initialize display hardware */
    GRAPHIC_init_screen();

    /* Initialize LVGL core and ports */
    lv_init();

    lv_display_t * disp = lv_display_create(TFT_PANEL_WIDTH, TFT_PANEL_HEIGHT);
    lv_display_set_buffers(disp, (void*)framebuffer[0], (void*)framebuffer[1],
                           FRAMEBUFFER_STRIDE * FRAMEBUFFER_HEIGHT, LV_DISPLAY_RENDER_MODE_DIRECT);
    lv_display_set_flush_cb(disp, rz_disp_flush);

    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, rz_touchpad_read);

    /* Launch the E-Bike Demo */
    lv_demo_ebike();

    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(5);

    while (1)
    {
        lv_tick_inc(5);
        uint32_t sleep_time_ms = lv_timer_handler();
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(sleep_time_ms < 5 ? 5 : sleep_time_ms));
    }
}

int_t os_main_task_t(void)
{
    st_os_abstraction_info_t ver_info;

    gs_my_gpio_handle = open (DEVICE_INDENTIFIER "gpio", O_RDWR);
    if ( gs_my_gpio_handle < 0 ) { while(1); }

    printf("%s%s", sp_clear_screen, sp_cursor_home);
    printf("RZ/A2M LVGL Application\r\n");

    TouchPanel_Init();
    if(0 > TouchPanel_Open(TFT_PANEL_WIDTH, TFT_PANEL_HEIGHT, TP_INT_PRI, TP_TSK_PRI, R_OS_ABSTRACTION_DEFAULT_STACK_SIZE))
    {
        printf("I2C touch driver could not be loaded\r\n");
    }
    else
    {
        an_tp_evt_id[0] = TouchPanel_EventEntry(TPEVT_ENTRY_ALL, 0, 0, TFT_PANEL_WIDTH, TFT_PANEL_HEIGHT, &tpevt_cb_func);
    }

    /* Create LVGL Worker Task */
    os_task_t *p_os_task = R_OS_TaskCreate("LVGL Task", lvgl_task, NULL, 4096, R_OS_TASK_MAIN_TASK_PRI + 1);
    if (NULL == p_os_task)
    {
        printf("Failed to create LVGL task!\r\n");
    }

    while (1)
    {
        R_OS_TaskSleep(10000);
    }
}

void rza2m_hal_init ( void ) {
    int_t cpg_handle;
    R_DEVLINK_Init();
    cpg_handle = direct_open("cpg", 0);
    if ( cpg_handle < 0 ) { while(1); }
    direct_close(cpg_handle);

    /* Starts FreeRTOS kernel and invokes os_main_task_t */
    R_OS_KernelInit();
}
