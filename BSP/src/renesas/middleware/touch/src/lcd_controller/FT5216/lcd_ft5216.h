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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2013-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file          lcd_ft5216.h
* $Rev: 30 $
* $Date:: 2016-12-21 12:02:44 +0900#$
* @brief         LCD Driver internal hedder
******************************************************************************/
#ifndef     LCD_FT5216_H
#define     LCD_FT5216_H

/******************************************************************************
Includes <System Includes>, "Project Includes"
******************************************************************************/
#include    "r_os_abstraction_api.h"
#include    "lcd_controller_if.h"

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
Macro definitions
******************************************************************************/
#define DBG_LEVEL_OT                (-1)                /* onetime debug */
#define DBG_LEVEL_DEF               (0)                 /* default */
#define DBG_LEVEL_ERR               (1)                 /* error */
#define DBG_LEVEL_MSG               (2)                 /* message */
#define DBG_LEVEL_LOG               (3)                 /* log */
#define DBG_LEVEL_DBG               (4)                 /* debug */

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

//#define   __DEBUG
#ifdef  __DEBUG
#define SCOPE_STATIC
#else   /* __DEBUG */
#define SCOPE_STATIC    static
#endif  /* __DEBUG */

/*! The max number of event entry */
#define     LCDEVT_ENTRY_MAX            (1)


/*************************************************************************
 Enumerated Types
*************************************************************************/
/*! Touch panel event lock state */
typedef enum {
    LCD_EVT_UNLOCK = 0,  /*!< Unlocked */
    LCD_EVT_LOCK         /*!< Locked */
} LcdEvt_LockState ;


/*************************************************************************
 Structures
*************************************************************************/
/*! Event entry struct */
typedef struct {
    LcdEvt_EntryType mode ;      /*!< The type of touch panel event entry */
    LcdCBFunc function;
    LcdEvt_LockState evtlock ;   /*!< Event lock state */
} LCDEVT_ENTRY ;


/***********************************************************************************
 Global Vaiables
***********************************************************************************/
extern uint32_t   sLcdSemIdAcc;


/***********************************************************************************
 Function Prototypes
***********************************************************************************/
int_t        LCD_Ft5216_Open( const uint32_t unIrqLv, int16_t nTskPri, uint32_t unTskStk );
int_t        LCD_Ft5216_Close( void );
int_t        LCD_Ft5216_WriteCmd( const uint16_t unDevAddr, const uint8_t uData, const uint32_t unSize );
int_t        LCD_Ft5216_ReadCmd( const uint16_t unDevAddr, uint8_t *puData, const uint32_t unSize );
int_t        LCD_Ft5216_EventEntry( const LcdEvt_EntryType eType, const LcdCBFunc function );
int_t        LCD_Ft5216_EventErase( const int_t nId );
int_t        LCD_Ft5216_StartInt( const LcdEvt_EntryType eType );
LCDEVT_ENTRY *LCD_Ft5216_GetEventTable( const int_t nId );
int32_t      LCD_Ft5216_SendEvtMsg( const uint32_t unEvtFlg );
int32_t      LCD_Ft5216_WaitEvtMsg( void );
void         LCD_Ft5216_ClearEvtMsg( const uint32_t unEvtFlg );

#if defined(__cplusplus)
}
#endif

#endif      /* LCD_FT5216_H */
