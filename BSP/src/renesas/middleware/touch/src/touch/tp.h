/******************************************************************************
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
 * Renesas reserves the right, without notice, to make changes to this
 * software and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) ${year} Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/*****************************************************************************
 * File Name    : tp.h
 * Description  : TouchPanel Driver internal hedder
 *****************************************************************************/
/******************************************************************************
Includes <System Includes>, "Project Includes"
******************************************************************************/
#ifndef     TP_H
#define     TP_H

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include "r_os_abstraction_api.h"
#include "tp_if.h"
#include "lcd_controller_if.h"

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
#define     TPEVT_ENTRY_MAX         (16)

#define     TP_DEVADDR_SHIFT        (16)
#define     TP_DEVADDR_MASK         (0xFFFF0000)

#define     TP_PENSTATUS_RELEASE    (0)
#define     TP_PENSTATUS_PUSH       (1)

/* event flag */
#define        TP_EVTFLG_NONE             (0x00000000)
#define        TP_EVTFLG_PENIRQ           (0x00000001)      /*! pen interrupt */
#define        TP_EVTFLG_ATTACH           (0x00000002)      /*! only USB event flag, attach interrupt */
#define        TP_EVTFLG_DETACH           (0x00000004)      /*! only USB event flag, detach interrupt */
#define        TP_EVTFLG_EXIT             (0x00000080)      /*! exit and delete task */

/******************************************************************************
 Enumerated Types
******************************************************************************/
/*! Touch panel event lock state */
typedef enum {
    TP_EVT_UNLOCK = 0,  /*!< Unlocked */
    TP_EVT_LOCK         /*!< Locked */
} TpEvt_LockState ;


/******************************************************************************
 Structures
******************************************************************************/
/*! Coordinate structure */
typedef struct {
    int32_t x ;        /*!< x-coordinate [pixel] */
    int32_t y ;        /*!< y-coordinate [pixel] */
} TPEVT_COORDINATES ;

/*! Event entry struct */
typedef struct {
    TpEvt_EntryType mode ;      /*!< The type of touch panel event entry */
    TPEVT_COORDINATES st ;      /*!< The upper-left coordinates of the rectangular area in which touch event can be received. [pixel] */
    TPEVT_COORDINATES ed ;      /*!< The lower-right coordinates of the rectangular area in which touch event can be received. [pixel] */
    void (*function)( int_t, TP_TouchEvent_st* ) ;  /*!< Event notification callback function pointer */
    TpEvt_LockState evtlock ;   /*!< Event lock state */
} TPEVT_ENTRY ;


/******************************************************************************
 Global Vaiables
******************************************************************************/
extern os_task_t*  p_os_task;


/******************************************************************************
 Function Prototypes
******************************************************************************/

/*****************************************************************************
 * Function Name: TP_Init
 * Description  : Initializes internal variables of the touch panel driver.
 *                - Securing of touch panel event entry area
 *                - Setting of internal variable nEvtEntryId  to -1
 *                - Setting of internal variable TpEvtLockInf to TP_EVT_UNLOCK
 * Arguments    : None
 * Return Value : None
 *****************************************************************************/
void            TP_Init( void );

/*****************************************************************************
 * Function Name: TP_Open
 * Description  : Opens the touch panel driver.
 *                -    Setting the LCD size in the driver' s variables 
 *                     ScreenWidth and ScreenHeight
 *                -    Generation of touch panel task synchronization semaphore
 *                -    Generation of touch panel task
 *                -    Setting of task priority of touch panel task
 *                -    Opening of communication environment with LCD board
 *                -    Registration of call-back event when touch panel 
 *                     interrupt occurs in LCD event
 * Arguments    : nWidth   - screen width
 *              : nHeight  - screen height
 *              : unIrqLv  - IRQ interrupt level
 *              : nTskPri  - task priority
 *              : unTskStk - task stack size
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
int_t           TP_Open( const int_t nWidth, const int_t nHeight, const uint32_t unIrqLv, const int16_t nTskPri, const uint32_t unTskStk );

/*****************************************************************************
 * Function Name: TP_Close
 * Description  : Closes the touch panel driver.
 *                -    Removal of call-back event when touch panel interrupt 
 *                     occurs in LCD event\
 *                -    Removal of all touch panel event registrations 
 *                     by the user
 *                -    Removal of touch panel task
 *                -    Removal of semaphore for synchronization with the 
 *                     touch panel task
 * Arguments    : None
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
int_t           TP_Close( void );

/*****************************************************************************
 * Function Name: TP_EventEntry
 * Description  : Registers in the event table a call-back function linked to a 
 *                touch panel interrupt.
 *                After registration finishes, the event ID is sent as 
 *                a return value.
 *
 *                 -    Searching for a free area in the touch panel event 
 *                      table (Up to 16 touch panel events can be registered, 
 *                      and error processing occurs if no free area is 
 *                      available.) 
 *                 -    Making "specified touch action," "X coordinate of 
 *                      specified area," "Y coordinate of specified area," 
 *                      "width of specified area," "height of specified area,"
 *                      "specified call-back function" settings for the touch 
 *                      panel event table free area.
 *               Note:     When "X coordinate of specified area," "Y coordinate
 *                      of specified area," "width of specified area," and 
 *                      "height of specified area" are registered in the touch
 *                      panel event table, the following processing is 
 *                      performed to register the result as a rectangular area:
 *               st.x (X coordinate of area start position) <- "X coordinate of
 *               specified area"
 *               st.y (Y coordinate of area start position) <- "Y coordinate of
 *               specified area"
 *               ed.x (X coordinate of area end position) <- ("X coordinate of
 *               specified area" - "width of specified area")
 *               ed.y (Y coordinate of area end position) <- ("Y coordinate of
 *               specified area" - "height of specified area")
 * Arguments    : eMode    - Specified touch action
 *              : nPosX    - X-coordinate of LCD area
 *              : nPosY    - Y-coordinate of LCD area
 *              : nWidth   - width of LCD area
 *              : nHeight  - height of LCD area
 *              : function - Specified call-back function
 * Return Value : 0 to (TPEVT_ENTRY_MAX -1) - Success event
 *               -1 - Fail returns
 *****************************************************************************/
int_t           TP_EventEntry( const TpEvt_EntryType eMode, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight, const TpCBFunc function );


/*****************************************************************************
 * Function Name: TP_EventErase
 * Description  : Removes an event from the call-back event table of the touch 
 *                panel driver.
 *                -    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)
 *                -    Disabling of event associated with event ID 
 *                     (TPEVT_ENTRY_NON)
 * Arguments    : nId - Event ID
 * Return Value : 0 - Operation successfull
 *               -1 - Error occurred
 *****************************************************************************/
int_t           TP_EventErase( const int_t nId );

/*****************************************************************************
 * Function Name: TP_ChangeEventEntry
 * Description  : The rectangular area to which the event ID specified by the 
 *                1st argument (nId)
 *                is registered is changed to the rectangular area specified 
 *                by the 2nd to 5th arguments.
 *                -    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)
 *                -    Event ID checking (unregistered ID or removed ID)
 *                -    Registration of event in area of specified ID in touch 
 *                     panel event table 
 * Arguments    : nId      - Event ID
 *              : nPosX    - X-coordinate of LCD area
 *              : nPosY    - Y-coordinate of LCD area
 *              : nWidth   - width of specified area
 *              : nHeight  - height of specified area
 * Return Value : 0 - Success event of (TPEVT_ENTRY_MAX -1)
 *               -1 - LCD area change failure
 *****************************************************************************/
int_t           TP_ChangeEventEntry( const int_t nId, const int32_t nPosX, const int32_t nPosY, 
                                        const int32_t nWidth, const int32_t nHeight );

/*****************************************************************************
 * Function Name: TP_EventLockAll
 * Description  : Locks all registered touch panel call-back events.
 *                Calls the function described in TP_EventLock, to set all 
 *                events to the locked state (TP_EVT_LOCK).
 * Arguments    : None
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
int_t           TP_EventLockAll( void );

/*****************************************************************************
 * Function Name: TP_EventUnlockAll
 * Description  : Unlocks all registered touch panel call-back events.
 *                Calls the function described in TP_EventUnlock, to set all 
 *                events to the unlocked state (TP_EVT_UNLOCK).
 * Arguments    : None
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
int_t           TP_EventUnlockAll( void );

/*****************************************************************************
 * Function Name: TP_EventLock
 * Description  : Locks the touch panel call-back event specified by the 1st 
 *                argument (nId).
 *                -    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)
 *                -    Setting the event specified by the event ID to the 
 *                     locked state (TP_EVT_LOCK) in the touch panel event 
                       table.
 * Arguments    : nId - Event ID
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
int_t           TP_EventLock( const int_t nId );

/*****************************************************************************
 * Function Name: TP_EventUnlock
 * Description  : Unlocks the touch panel call-back event specified by the 1st
 *                argument (nId).
 *                -    Event ID checking (within range of 0 to TPEVT_ENTRY_MAX)
 *                -    Setting the event specified by the event ID to the 
 *                     unlocked state (TP_EVT_UNLOCK) in the touch panel event 
 *                     table.
 * Arguments    : nId - Event ID
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
int_t           TP_EventUnlock( const int_t nId );

/*****************************************************************************
 * Function Name: TP_GetEventTable
 * Description  : Acquires from the touch panel driver call-back event table 
 *                the pointer address at which the event ID event information 
 *                is registered.
 * Arguments    : nId - Event ID
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
TPEVT_ENTRY     *TP_GetEventTable( const int_t nId );

/*****************************************************************************
 * Function Name: TP_GetEventLockInf
 * Description  : Acquires the lock state of the touch panel call-back event.
 * Arguments    : None
 * Return Value : TP_EVT_LOCK - In locked state.
 *                TP_EVT_UNLOCK - In unlocked state.
 *****************************************************************************/
TpEvt_LockState TP_GetEventLockInf( void );

/*****************************************************************************
 * Function Name: TP_GetScreenSize
 * Description  : Acquires the screen size of the LCD panel.
 * Arguments    : *pnWidth - pointer to width value
 *              : *pnHeight - pointer to height value
 * Return Value : None
 *****************************************************************************/
void            TP_GetScreenSize( int_t *pnWidth, int_t *pnHeight );

/*****************************************************************************
 * Function Name: TP_SendEvtMsg
 * Description  : Sends a synchronization event message.
 * Arguments    : unEvtFlg - event flag
 * Return Value : 0 - Operation successfull.
 *               -1 - Error occured.
 *****************************************************************************/
int32_t         TP_SendEvtMsg( const uint32_t unEvtFlg );

#if (LCD_PANEL == LCD_PANEL_RSK)
/*****************************************************************************
 * Function Name: TP_WaitEvtMsg
 * Description  : Waits to receive a synchronization event message.
 * Arguments    : None
 * Return Value : TP_EVTFLG_NONE   - No event flags.
 *                TP_EVTFLG_PENIRQ - Interrupt pending.
 *                TP_EVTFLG_EXIT   - End task.
 *                TP_EVTFLG_ALL    - Both Interrupt pending and exit flag. 
 *               -1 - Error occured.
 *****************************************************************************/
int_t           tp_get_app_board_version( void );
#endif

/**
 * @brief         Waits to receive a synchronization event message.
 * @retval        TP_EVTFLG_NONE No event flags
 * @retval        TP_EVTFLG_PENIRQ Interrupt pending
 * @retval        TP_EVTFLG_EXIT End task 
 * @retval        TP_EVTFLG_ALL Both Interrupt pending and exit flag. 
 * @retval        -1 Error occured.
*/
int32_t         TP_WaitEvtMsg( void );

/*****************************************************************************
 * Function Name: TP_ClearEvtMsg
 * Description  : Clears the specified event flag.
 * Arguments    : unEvtFlg - event flag
 * Return Value : None.
 *****************************************************************************/
void            TP_ClearEvtMsg( const uint32_t unEvtFlg );

#if defined(__cplusplus)
}
#endif


#endif      /* TP_H */
