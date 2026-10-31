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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
* File Name    : r_usb_hhid_apl.c
* Description  : USB Host HID application code
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.08.2015 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "r_usb_hhid_apl.h"
#include "r_usb_hhid_control.h"
#include "r_usbh0_hhid_config.h"
#include "r_usbh1_hhid_config.h"

#include "r_usbh0_basic_if.h"
#include "r_usbh0_basic_local.h"
#include "r_usbh0_rtos.h"

#include "r_usbh1_basic_if.h"
#include "r_usbh1_basic_local.h"
#include "r_usbh1_rtos.h"

#include "r_os_abstraction_api.h"
#include "driver.h"
#include "iodefine.h"
#include "tp_if.h"

#include "r_usb_hhid_dev.h"
#include "r_usbh1_basic_local.h"
/******************************************************************************
Constant macro definitions
******************************************************************************/
#define USB_KEY_INPUT_BUFF_NUM  (12)
/* ASCII CODE */
#define ASCII_LF                (0x0A)
#define ASCII_CR                (0x0D)
#define ASCII_0                 ('0')
#define ASCII_1                 ('1')

/******************************************************************************
 Global Vaiables
******************************************************************************/
extern uint32_t sLcdSemIdAcc;
uint32_t sLcdSemIdData;

uint8_t g_usb_data[USB_BUF_LEN];

SCOPE_STATIC LCDEVT_ENTRY LcdEvt_Entry[LCDEVT_ENTRY_MAX];

r_usb_dev_info_t  gs_usb_dev_info[USBH1_MAXDEVADDR];

/******************************************************************************
Private global variables and functions
******************************************************************************/
static int_t usb_main_task_t(void);


/******************************************************************************
Function Name   : USB_Open
Description     : Open USB
Arguments       : nTskPri : USB task priority
Return value    : 0  : Operation successfull
                : -1 : Error occurred
******************************************************************************/
int_t USB_Open(const int16_t nTskPri)
{
    int_t nRet = 0;
    
    memset( &LcdEvt_Entry, 0, sizeof(LcdEvt_Entry));
    
    /** create access semaphore for driver */
    if (R_OS_SemaphoreCreate( &sLcdSemIdAcc, 1) != true)
    {
        DBG_printf_ERR("[ERROR] semaphore for internal access is not created\n");
        nRet = -1;
    }

    /* create data copy semaphore */
    if (nRet == 0)
    {
        if (R_OS_SemaphoreCreate( &sLcdSemIdData, 1) != true)
        {
            DBG_printf_ERR("[ERROR] semaphore for data copy is not created\n");
            nRet = -1;
        }
    }
    
    if (nRet == 0)
    {
        usbh0_cstd_PortInit();      /* Initialize USB Port0 */
        usbh1_cstd_PortInit();      /* Initialize USB Port1 */

        R_USBH0_Init();
        R_USBH1_Init();

        usbh0_hid_init();
        usbh1_hid_init();
        
        /* Create a task to run the usb */
        R_OS_TaskCreate("USB_Mouse", usb_main_task_t, NULL, R_OS_ABSTRACTION_DEFAULT_STACK_SIZE, nTskPri);
    }
    else
    {
        R_OS_SemaphoreDelete( &sLcdSemIdAcc);
        R_OS_SemaphoreDelete( &sLcdSemIdData);
    }

    return nRet;
}

/******************************************************************************
 * End of function USB_Open
 *****************************************************************************/

/******************************************************************************
Function Name   : USB_Close
Description     : Close USB
Arguments       : none
Return value    : 0  : Operation successfull
                : -1 : Error occurred
******************************************************************************/
int_t USB_Close(void)
{
    int_t nRet = 0;
    
#if (BSP_CFG_RTOS_USED == 1)
    R_USBH0_HstdMgrClose();      /* USB1 MGR Close */
    R_USBH1_HstdMgrClose();      /* USB1 MGR Close */
#endif

    /** delete access semaphore */
    if (sLcdSemIdAcc != 0)
    {
        R_OS_SemaphoreDelete( &sLcdSemIdAcc);
        sLcdSemIdAcc = 0;
    }
    else
    {
        DBG_printf_ERR("[ERROR] semaphore for internal access is not deleted\n");
        nRet = -1;
    }
    
    /** delete synchronous semaphore */
    if (sLcdSemIdData != 0)
    {
        R_OS_SemaphoreDelete( &sLcdSemIdData);
        sLcdSemIdData = 0;
    }
    else
    {
        DBG_printf_ERR("[ERROR] semaphore for data copy is not deleted\n");
        nRet = -1;
    }

    return nRet;
}

/******************************************************************************
 * End of function USB_Close
 *****************************************************************************/

/******************************************************************************
Function Name   : USB_ReadCmd
Description     : read position data of USB mouse
Arguments       : *puData   : pointer to receive buffer
Return value    : 0  : Operation successfull
                : -1 : Error occurred
******************************************************************************/
int_t USB_ReadCmd(uint8_t *puData, const uint32_t unSize)
{
    int_t nRet = 0;
    uint32_t data_num;
    

    if (R_OS_SemaphoreWait( &sLcdSemIdData, R_OS_ABSTRACTION_EV_WAIT_INFINITE) != true)
    {
        DBG_printf_ERR("[ERROR] semaphore error for data\n");
    }
    else
    {
        for (data_num = 0; data_num < unSize; data_num++)
        {
            *(puData + data_num) = g_usb_data[data_num];
        }
        R_OS_SemaphoreRelease( &sLcdSemIdData );
        
    }

    return nRet;
}

/******************************************************************************
 * End of function USB_ReadCmd
 *****************************************************************************/

/******************************************************************************
Function Name   : USB_EventEntry
Description     : Enter callback event to event table.
Arguments       : eType      : event type
                : *function  : callback function
Return value    : 0  : Operation successfull
                : -1 : Error occurred
******************************************************************************/
int_t USB_EventEntry(const LcdEvt_EntryType eType, const LcdCBFunc function)
{
    int_t nI ;
    LCDEVT_ENTRY *Entry ;

    nI = -1;

    if( (eType > LCDEVT_ENTRY_NONE) && (eType <= LCDEVT_ENTRY_MAX) )
    {
        Entry = &LcdEvt_Entry[ eType-1 ] ;
        if( Entry->mode == LCDEVT_ENTRY_NONE )
        {
            Entry->mode     = eType;
            Entry->function = function ;
            nI              = eType-1;
        }
    }
    /** Return ERROR ( -1 ), when the event entry fails. */
    return nI;
}

/******************************************************************************
 * End of function USB_EventEntry
 *****************************************************************************/

/******************************************************************************
Function Name   : USB_EventErase
Description     : Enter callback event to event table.
Arguments       : nId  : event ID
Return value    : 0  : Operation successfull
                : -1 : Error occurred
******************************************************************************/
int_t USB_EventErase(const int_t nId)
{
    int_t nRet = 0;

    if(( nId < 0 ) || ( nId >= LCDEVT_ENTRY_MAX ))
    {
        nRet = -1;         /* id error */
    }
    else
    {
        LcdEvt_Entry[ nId ].mode = LCDEVT_ENTRY_NONE;
    }

    return nRet;
}

/******************************************************************************
 * End of function USB_EventEntry
 *****************************************************************************/

/******************************************************************************
Function Name   : USB_GetEventTable
Description     : Get assigned callback event.
Arguments       : nId  : event ID
Return value    : pointer to event table,
******************************************************************************/
LCDEVT_ENTRY *USB_GetEventTable(const int_t nId)
{
    return &LcdEvt_Entry[ nId ];
}

/******************************************************************************
 * End of function USB_GetEventTable
 *****************************************************************************/

/******************************************************************************
Function Name   : USB_WaitSyncBuffer
Description     : Synchronize access to buffer.
Arguments       : none
Return value    : 0  : sync success
                  -1 : semaphore wait error
******************************************************************************/
int_t USB_WaitSyncBuffer(void)
{
    int_t nRet = 0;
    
    if (R_OS_SemaphoreWait( &sLcdSemIdData, R_OS_ABSTRACTION_EV_WAIT_INFINITE) != true)
    {
        DBG_printf_ERR("[ERROR] semaphore error for data\n");
        nRet = -1;
    }
    
    return nRet;
}

/******************************************************************************
 * End of function USB_WaitSyncBuffer
 *****************************************************************************/

/******************************************************************************
Function Name   : USB_ReleaseSyncBuffer
Description     : Synchronize access to buffer.
Arguments       : none
Return value    : 0  : sync success
                  -1 : semaphore wait error
******************************************************************************/
void USB_ReleaseSyncBuffer(void)
{
    R_OS_SemaphoreRelease( &sLcdSemIdData );
    
    return;
}

/******************************************************************************
 * End of function USB_ReleaseSyncBuffer
 *****************************************************************************/


/******************************************************************************
Function Name   : usb_main_task_t
Description     : USB task
Arguments       : none
Return value    : none
******************************************************************************/
static int_t usb_main_task_t(void)
{
    usbh0_er_t       h0_err;
    usbh1_er_t       h1_err;

    while(1)
    {
        /* Scheduler */
        R_USBH0_CstdScheduler();
#if (BSP_CFG_RTOS_USED == 0)

        if( USBH0_FLGSET == R_USBH0_CstdCheckSchedule() )
        {
            R_USBH0_HstdMgrTask();          /* MGR task */
            R_USBH0_HhubTask();             /* HUB task */
            R_USBH0_HhidTask();             /* HHID Task */
        }
#endif /* (BSP_CFG_RTOS_USED == 0)*/
        usbh0_hid_main();

        /* Scheduler */
        R_USBH1_CstdScheduler();
#if (BSP_CFG_RTOS_USED == 0)

        if( USBH1_FLGSET == R_USBH1_CstdCheckSchedule() )
        {
            R_USBH1_HstdMgrTask();          /* MGR task */
            R_USBH1_HhubTask();             /* HUB task */
            R_USBH1_HhidTask();             /* HHID Task */
        }
#endif /* (BSP_CFG_RTOS_USED == 0)*/
        usbh1_hid_main(g_usb_data);
        R_OS_TaskSleep( USB_TASK_DELAY );
    }

#if (BSP_CFG_RTOS_USED == 1)
    R_USBH0_HstdMgrClose();      /* USB1 MGR Close */
    R_USBH1_HstdMgrClose();      /* USB1 MGR Close */
#endif
} 
/******************************************************************************
 * End of function usb_main_task_t
 *****************************************************************************/

