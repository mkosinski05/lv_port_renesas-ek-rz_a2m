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
 * File Name :   r_usb_hhid_dev.h
 * @file         r_usb_hhid_dev.h
 * @version      1.0
 * @brief
 ******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_usbh1_basic_local.h"

#ifndef  R_USB_HHID_DEV_H
#define  R_USB_HHID_DEV_H

/******************************************************************************
Macro definitions
******************************************************************************/

#define USB_IF_MOUSE       0
#define USB_IF_TOUCH       1
#define USB_IF_KEYBOARD    2

#define USB_IF_NODEVICE    0xFF

#define USB_POS_TYPE_ABSOLUTE    0
#define USB_POS_TYPE_RELATIVE    1

/* USB touchpanel define */
#define USB_TOUCH_ONLAP_1102I     0   /* ON-Lap 1102I */

#define USB_TOUCH_DEVICE          USB_TOUCH_ONLAP_1102I

/******************************************************************************
Structures
******************************************************************************/

typedef struct {
    uint32_t     buffer_size;         /* copy data size to TP task from USB */
} r_usb_keyboard_info_t;

typedef struct {
    uint32_t     buffer_size;         /* copy data size to TP task from USB */
    uint32_t     pos_data_type;       /* position data type (absolute / relative) */
    uint32_t     data_len_pos;        /* byte length of X coodinate value */ 
    uint32_t     data_offset_posX1;  /* position of X coordinate in buffer */
    uint32_t     data_offset_posY1;  /* position of Y coordinate in buffer */
    uint32_t     dev_resolutionX;     /* X resolution value of device */
    uint32_t     dev_max_posX;        /* max X coordinate value of device */
    uint32_t     dev_resolutionY;     /* Y resolution value of device */
    uint32_t     dev_max_posY;        /* max Y coordinate value of device */
    uint32_t     data_offset_click;  /* position of click data, mouse only */
    uint8_t      left_click_bit;      /* position of left click bit, mouse only */
    uint8_t      rigith_click_bit;    /* position of right click bit, mouse only */
    uint8_t      center_button_bit;   /* position of center button bit, mouse only */
    uint32_t     data_offset_wheel;  /* position of wheel in buffwheelX value, mouse only */ 
    uint32_t     cursor_speed;        /* mouse cursor speed magnification, mouse only */
    
} r_usb_mouse_info_t;

typedef struct {
    uint32_t     buffer_size;         /* copy data size to TP task from USB */
    uint32_t     pos_data_type;       /* position data type (absolute / relative) */
    uint32_t     data_len_pos;        /* byte length of X coodinate value */ 
    uint32_t     data_offset_posX1;  /* position of X coordinate in buffer */
    uint32_t     data_offset_posY1;  /* position of Y coordinate in buffer */
    uint32_t     dev_resolutionX;     /* X resolution value of device */
    uint32_t     dev_max_posX;        /* max X coordinate value of device */
    uint32_t     dev_resolutionY;     /* Y resolution value of device */
    uint32_t     dev_max_posY;        /* max Y coordinate value of device */
    uint32_t     finger_info_pos;     /* position of finger info in buffer, touch only */
    
} r_usb_touch_info_t;

typedef struct {
    uint32_t               interface;           /* control interface */
    uint32_t               copy_data_len;       /* copy data size to TP task from USB */
    r_usb_keyboard_info_t  *keyboard;           /* parameter of keyboard */
    r_usb_mouse_info_t     *mouse;              /* parameter of mouse */
    r_usb_touch_info_t     *touch;              /* parameter of touch panel */
} r_usb_dev_info_t;

/******************************************************************************
Variable Externs
******************************************************************************/

extern r_usb_dev_info_t  gs_usb_dev_info[USBH1_MAXDEVADDR];

/* define parameter of USB device */
static r_usb_mouse_info_t usb_keyboard_info = 
/* no support, dummy define */
{
    0,
};

static r_usb_mouse_info_t usb_mouse_info = 
/* USB mouse 853-410151-201-A */
{
    4,                       /* data buffer length */
    USB_POS_TYPE_RELATIVE,   /* position data type */
    1,                       /* codinate data byte length */
    1,                       /* x coordinate data offset */
    2,                       /* y coordinate data offset */
    0,                       /* x resolution is none */
    800,                     /* max x coordnate value */
    0,                       /* y resolution is none */
    480,                     /* max y coordnate value */
    0,                       /* click data offset */
    0x01,                    /* left click data bit */
    0x02,                    /* right click data bit */
    0x04,                    /* center click data bit */
    3,                       /* wheel data offset */
    2                        /* mouse cursor speed */
};

static r_usb_touch_info_t usb_touch_info = 
#if (USB_TOUCH_DEVICE == USB_TOUCH_ONLAP_1102I)
/* USB touch On-Lap 1102I */
{
    64,                      /* data buffer length */
    USB_POS_TYPE_ABSOLUTE,   /* position data type */
    2,                       /* codinate data byte length */
    2,                       /* x coordinate data offset */
    4,                       /* y coordinate data offset */
    1920,                    /* x resolution */
    16383,                   /* max x coordnate value */
    1080,                    /* y resolution */
    9599,                    /* max y coordnate value */
    56                       /* position finger information */
};
#endif

/******************************************************************************
Functions Prototypes
******************************************************************************/


#endif /* R_USB_HHID_DEV_H */
