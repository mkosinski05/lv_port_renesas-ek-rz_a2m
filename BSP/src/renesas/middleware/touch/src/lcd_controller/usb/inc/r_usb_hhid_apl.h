/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014(2016) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_husbh0_hid_apl.h
 * Description  : USB HID application code
 ******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 26.12.2014 1.10 RX71M is added
 *         : 30.09.2015 1.11 RX63N/RX631 is added.
 *         : 30.09.2016 1.20 RX65N/RX651 is added.
 ******************************************************************************/


 /******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Access to peripherals and board defines. */
#include "r_usbh0_basic_if.h"
#include "r_usbh0_hhid.h"
#include "r_usbh0_hhid_if.h"
#include "r_usbh0_hhid_config.h"

#include "r_usbh1_basic_if.h"
#include "r_usbh1_hhid.h"
#include "r_usbh1_hhid_if.h"
#include "r_usbh1_hhid_config.h"

#include "lcd_controller_if.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define HID_NORMAL                          (0)             /* NORMAL mode */
#define HID_DEMO                            (1)             /* DEMO mode */
#define HID_ECHO                            (2)             /* Loop back(Echo) mode */

#define USE_USBIP0                          (0x01)
#define USE_USBIP1                          (0x02)

#define     USB_APL_DISABLE                 (0)
#define     USB_APL_ENABLE                  (1)

#define USB_CLASS_REQEST_QUE_MAX            (6u)

#define MAX_DEVICE_ADDRESS                  (4 +1)

/* Set Report (Keyborad LED ) */
#define NUM_LOCK                            (0x01)
#define CAPS_LOCK                           (0x02)
#define SCROLL_LOCK                         (0x04)
#define KBD_LED_CLEAR                       (0x00)
#define SET_REPORT_LENGTH                   (1)

/* LCD Display Position */
#define X_AXIS_POS                          (0u)            /* Mouse X axis */
#define Y_AXIS_POS                          (4u)            /* Mouse Y axis */

#define LINE1                               (LCD_POS_U0)    /* Line 1 */
#define LINE2                               (LCD_POS_D0)    /* Line 2 */
#define LINE3                               (LINE2 + 8)     /* Line 3 */
#define LINE4                               (LINE3 + 8)     /* Line 4 */
#define LINE5                               (LINE4 + 8)     /* Line 5 */
#define LINE6                               (LINE5 + 8)     /* Line 6 */
#define LINE7                               (LINE6 + 8)     /* Line 7 */
#define LINE8                               (LINE7 + 8)     /* Line 8 */
#define LINE9                               (LINE8 + 8)     /* Line 9 */

#define DISP_CHAR_LENGTH                    (8+1)           /* LCD Display 8characters +USB_NULL */

/* Mouse Button Defines */
#define LEFT_BUTTON                         (0x01)          /* Left Button */
#define RIGHT_BUTTON                        (0x02)          /* Right Button */
#define WHEEL_BUTTON                        (0x04)          /* Wheel Button */

/* LED Position for USB Mouse */
#define LED_POS_LEFT_BTN                    (0x00)          /* Left Button */
#define LED_POS_RIGHT_BTN                   (0x01)          /* Right Button */
#define LED_POS_WHEEL_BTN                   (0x02)          /* Wheel Button */

/* USB Mouse no movement */
#define MSE_NOT_MOVE                        (0x00)

/* Mouse data Store Position Index */
#define MSE_BTN_DATA_INDEX                  (0x00)          /* Input Key */
#define MSE_X_DATA_INDEX                    (0x01)          /* X-axis */
#define MSE_Y_DATA_INDEX                    (0x02)          /* Y-axis */

#define KBD_NOT_PUSH                        (0x00)          /* USB Keyboard no key input */

/* Keyboard data Store Position Index */
#define KBD_KEYCODE1_INDEX                  (0x02)          /* Input Key */

/* USB HID Keyboard Usage ID */
#define KBD_CODE_1                          (0x1E)          /* 1 */
#define KBD_CODE_0                          (0x27)          /* 0 */
#define KBD_CODE_ENTER                      (0x28)          /* '\n' */
#define KBD_CODE_BS                         (0x2A)          /* 'BS' */
#define KBD_CODE_SPACE                      (0x2C)          /* ' ' */
#define KBD_CODE_HYPHEN                     (0x2D)          /* '-' */
#define KBD_CODE_COLON                      (0x34)          /* ':' */
#define KBD_CODE_DOT                        (0x37)          /* '.' */
#define KBD_CODE_DIAGONAL                   (0x38)          /* '/' */
#define KBD_CODE_UNDERSCORE                 (0x87)          /* '_' */
#define KBD_CODE_YEN                        (0x89)          /* '\' */
#define KPD_CODE_START                      (0x58)          /* Keypad Number 1 */
#define KPD_CODE_END                        (0x62)          /* Keypad Number 0 */
#define KPD_CODE_DIAGONAL                   (0x54)          /* Keypad '/' */
#define KPD_CODE_HYPHEN                     (0x56)          /* Keypad '-' */
#define KPD_CODE_PLUS                       (0x57)          /* Keypad '+' */
#define KPD_CODE_DOT                        (0x63)          /* Keypad '.' */

/* Keycode to ASCII code offset */
#define KEYCODE_TO_ASCII_OFFSET0            (0x3D)          /* 'A'-'Z' */
#define KEYCODE_TO_ASCII_OFFSET1            (0x13)          /* '1'-'9' */
#define KEYCODE_TO_ASCII_OFFSET2            (0x09)          /* '0' */
#define KEYCODE_TO_ASCII_OFFSET3            (0x28)          /* Keypad '1' - '9' */
#define KEYCODE_TO_ASCII_OFFSET4            (0x30)          /* Keypad '0' */
#define ASCII_BS                            (0x08)          /* BackSpace */

/* Set Protocol */
#define BOOT_PROTOCOL                       (0)
#define REPORT_PROTOCOL                     (1)

#define EVENT_MAX                           (10)
#define MAXPIPE_NUM                         (9u)
#define REPORT_LENGTH                       (8)

#define USB_BUF_LEN                         (128)

/*****************************************************************************
Enumerated Types
******************************************************************************/
typedef enum
{
    STATE_ATTACH,
    STATE_CLASS_REQUEST,
    STATE_DATA_TRANSFER,
    STATE_SUSPENDED,
}
STATE_t;

typedef enum
{
    EVENT_NONE,
    EVENT_CONFIGERD,
    EVENT_DETACH,
    EVENT_CLASS_REQUEST_START,
    EVENT_CLASS_REQUEST_COMPLETE,
    EVENT_USB_TRANSFER_START,
    EVENT_USB_TRANSFER_COMPLETE,
    EVENT_RESUME,
    EVENT_SWITCH_INPUT
}
EVENT_t;

/* Structure for HID device control */

typedef struct DEV_INFO             /* Structure for HID device control */
{
    uint16_t    state;              /* State for application */
    uint16_t    event_cnt;          /* Event count */
    uint16_t    event[EVENT_MAX];   /* Event no. */
    uint8_t     data[REPORT_LENGTH];/* Receive Data */
}
DEV_INFO_t;

extern DEV_INFO_t  usbh0_hid_dev_info[];    /* MSC device control structure */
extern DEV_INFO_t  usbh1_hid_dev_info[];    /* MSC device control structure */

/*****************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
/* r_usb_husbh0_hid_apl.c */

void        usbh0_hid_main(void);
/* Application fucntions */
uint16_t    usbh0_hid_data_transfer( uint16_t devadr );
uint16_t    usbh0_hid_class_request( uint16_t devadr );
usbh0_err_t usbh0_hhid_class_request_process(void);
/* Initialize functions */
void        usbh0_hid_registration(void);
void        usbh0_hid_init( void );
/* Event control functions */
void        usbh0_hid_event_set(uint16_t devadr, uint16_t event );
uint16_t    usbh0_hid_event_get(uint16_t devadr);
/* State change callback functions */
void        usbh0_hid_suspended (uint16_t devadr);
void        usbh0_hid_configured (uint16_t dev_addr);
void        usbh0_hid_resume_complete (uint16_t devadr);
void        usbh0_hid_detach(uint16_t devadr);
void        usbh0_hid_suspend(uint16_t devadr);
/* Transfer callback functions */
void        usbh0_hid_class_request_complete(st_usbh0_utr_t *utr);
void        usbh0_hhid_read_complete (st_usbh0_utr_t *mess);



void        usbh1_hid_main(uint8_t *usb_data);
/* Application fucntions */
uint16_t    usbh1_hid_data_transfer( uint16_t devadr );
uint16_t    usbh1_hid_class_request( uint16_t devadr );
usbh1_err_t usbh1_hhid_class_request_process(void);
/* Initialize functions */
void        usbh1_hid_registration(void);
void        usbh1_hid_init( void );
/* Event control functions */
void        usbh1_hid_event_set(uint16_t devadr, uint16_t event );
uint16_t    usbh1_hid_event_get(uint16_t devadr);
/* State change callback functions */
void        usbh1_hid_suspended (uint16_t devadr);
void        usbh1_hid_configured (uint16_t dev_addr);
void        usbh1_hid_resume_complete (uint16_t devadr);
void        usbh1_hid_detach(uint16_t devadr);
void        usbh1_hid_suspend(uint16_t devadr);
/* Transfer callback functions */
void        usbh1_hid_class_request_complete(st_usbh1_utr_t *utr);
void        usbh1_hhid_read_complete (st_usbh1_utr_t *mess);


/******************************************************************************
External variables and functions
******************************************************************************/

/* r_usb_husbh0_hid_apl_data.c */

/******************************************************************************
 End  Of File
 ******************************************************************************/
