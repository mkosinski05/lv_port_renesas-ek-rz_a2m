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
 * File Name    : r_usb_hhid_control.h
 * Description  : USB mouse controll parameter header
 ******************************************************************************/

 /******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "r_typedefs.h"

#include "lcd_controller_if.h"

#ifndef R_USB_HHID_CONTROL_H
#define R_USB_HHID_CONTROL_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/
//#define   __DEBUG
#ifdef  __DEBUG
#define SCOPE_STATIC
#else   /* __DEBUG */
#define SCOPE_STATIC    static
#endif  /* __DEBUG */
 
#define DBG_LEVEL                   (DBG_LEVEL_ERR)
#if (DBG_LEVEL >= DBG_LEVEL_OT)
#define DBG_printf_OT                   printf
#else
#define DBG_printf_OT                   1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_DEF)
#define DBG_printf_DEF                  printf
#else
#define DBG_printf_DEF                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_ERR)
#define DBG_printf_ERR                  printf
#else
#define DBG_printf_ERR                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_MSG)
#define DBG_printf_MSG                  printf
#else
#define DBG_printf_MSG                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_LOG)
#define DBG_printf_LOG                  printf
#else
#define DBG_printf_LOG                  1 ? (int32_t) 0 : printf
#endif
#if (DBG_LEVEL >= DBG_LEVEL_DBG)
#define DBG_printf_DBG                  printf
#else
#define DBG_printf_DBG                  1 ? (int32_t) 0 : printf
#endif

/* LCD event entry number */
#define LCDEVT_ENTRY_MAX                    (1)

/* USB task delay time */
#define USB_TASK_DELAY                      (1)

/* Time that USB event did not occur */
#define USB_NO_EVENT_TIME                   (20)

/*****************************************************************************
Enumerated Types
******************************************************************************/
/*! Touch panel event lock state */
typedef enum {
    LCD_EVT_UNLOCK = 0,  /*!< Unlocked */
    LCD_EVT_LOCK         /*!< Locked */
} LcdEvt_LockState ;


/*! Event entry struct */
typedef struct {
    LcdEvt_EntryType mode ;      /*!< The type of touch panel event entry */
    LcdCBFunc function;
    LcdEvt_LockState evtlock ;   /*!< Event lock state */
} LCDEVT_ENTRY ;

extern uint32_t   sLcdSemIdAcc;

/*****************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
/* r_usb_husbh0_hid_apl.c */
int_t USB_Open(const int16_t nTskPri);
int_t USB_Close(void);
int_t USB_ReadCmd(uint8_t *puData, const uint32_t unSize);
int_t USB_EventEntry(const LcdEvt_EntryType eType, const LcdCBFunc function);
int_t USB_EventErase(const int_t nId);
LCDEVT_ENTRY *USB_GetEventTable(const int_t nId);
int_t USB_CheckSyncBuffer(void);
void USB_ReleaseSyncBuffer(void);

/******************************************************************************
External variables and functions
******************************************************************************/

#endif      /* R_USB_HHID_CONTROL_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/
