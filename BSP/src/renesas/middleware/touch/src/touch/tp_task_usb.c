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
* @file         tp_task_usb.c
* @brief        TouchPanel Driver task functions with USB
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"
#include    "iodefine.h"
#include    "dev_drv.h"
#include    "tp.h"
#include    "tp_task.h"
#include    "tp_task_usb.h"
#include    "lcd_controller_if.h"
#include    "r_usb_gui_control.h"
#include    "r_usb_hhid_dev.h"
#include    "r_usb_hhid_apl.h"

/******************************************************************************
Macro definitions
******************************************************************************/

#define     TPEVT_MOVE_MINTHD_USB     (0UL)
 
#define     SIGN_BIT_MASK_8BIT         (0x80)
#define     SIGN_BIT_MASK_16BIT        (0x8000)

/* MULTIPLE_VALUE_INT_CALC is used to avoid truncation after decimals when converting 
   coordinate data values to screen resolution unit values. */
#define     MULTIPLE_VALUE_INT_CALC    (1000)

/******************************************************************************
 Enumerated Types
******************************************************************************/


/******************************************************************************
 Structures
******************************************************************************/

/***********************************************************************************
 Global Vaiables
***********************************************************************************/
bool_t g_task_start_flag = false;

/*******************************************************************************
 Private global variables and functions
*******************************************************************************/
SCOPE_STATIC void  TP_SetFingerInfoUsb( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const uint32_t drvnum  );
SCOPE_STATIC void  TP_SetPositionUsbMouse( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const uint32_t drvnum );
SCOPE_STATIC void  TP_SetPositionUsbTouch( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const uint32_t drvnum );
SCOPE_STATIC int_t TP_EventHandleUsbMouse( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords );
SCOPE_STATIC int_t TP_EventHandleUsbTouch( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords );
SCOPE_STATIC void  TP_GetPositionData(const uint8_t* const puReadBuf, int16_t *PosX, int16_t *PosY, const uint32_t data_len, uint32_t posx_offset, uint32_t posy_offset );

extern TP_TASKSTAT_et  eTpTaskStat;

/**************************************************************************//**
* Function Name: TP_TaskTP_TaskProcessUsb
* @brief         Task function process of USB.
*
*                Description:<br>
*
* @param         [in]void const *vpArg  : pointer to argument
* @retval        None.
******************************************************************************/
void TP_TaskProcessUsb(void)
{
    int_t               nRet;
    int32_t             nEvtVal;
    uint8_t             auReadBuf[USB_BUF_LEN];
    TP_TouchEvent_st    sTouchEvt;
    TPEVT_COORDINATES   sLastCoords[TP_TOUCHNUM_MAX];
    int_t               nPenStatus[TP_TOUCHNUM_MAX];
    int_t               nI;
    int_t               nScrWidth;
    int_t               nScrHeight;
    uint32_t            drvnum;

    TP_GetScreenSize( &nScrWidth, &nScrHeight );

    eTpTaskStat = TP_TASKSTAT_ACTIVE;
    memset( &sTouchEvt, 0, sizeof(TP_TouchEvent_st) );
    memset( &sLastCoords, 0, sizeof(TPEVT_COORDINATES) );
    memset( nPenStatus, 0, sizeof(nPenStatus) );
    memset( auReadBuf, 0, sizeof(auReadBuf) );
    
    /* Wait for attaching if USB device is connected */
    R_OS_TaskSleep(1000);

    /* init sprite for mouse arrow and caution window */
    init_mouse(0, 0, nScrWidth, nScrHeight);
    
    display_off_mouse();

    /* Open the caution window */
    display_on_usbport();

    /* set initial coordinate */
    sTouchEvt.sFinger[nI].unPosX = nScrWidth;
    sTouchEvt.sFinger[nI].unPosY = nScrHeight;

    g_task_start_flag = true;

    while( 1 )
    {
        nEvtVal = TP_WaitEvtMsg();
        if( (nEvtVal & TP_EVTFLG_EXIT) == TP_EVTFLG_EXIT )
        {
            TP_ClearEvtMsg( TP_EVTFLG_EXIT );
            eTpTaskStat = TP_TASKSTAT_IDLE;

            R_OS_TaskDelete(p_os_task);
            p_os_task = NULL;

        }
        else if( (nEvtVal & TP_EVTFLG_PENIRQ) == TP_EVTFLG_PENIRQ )
        {
            drvnum = ((nEvtVal & TP_DEVADDR_MASK) >> TP_DEVADDR_SHIFT);
            
            TP_ClearEvtMsg( TP_EVTFLG_PENIRQ );

            /* get touch information via USB */
            nRet = R_LCD_ReadCmd(0, 0, auReadBuf, gs_usb_dev_info[drvnum].copy_data_len);
            if( nRet != DEVDRV_SUCCESS )
            {
                DBG_printf_ERR( "[ERROR] touch status is not got\n" );
            }
            else
            {
                /* make touch information for output */
                for( nI = 0; nI < TP_TOUCHNUM_MAX; nI ++ )
                {
                    TP_SetFingerInfoUsb( &sTouchEvt.sFinger[nI], &sLastCoords[nI], &nPenStatus[nI], auReadBuf, drvnum );
                }
            }

            /* execute callback function */
            if( nRet >= 0 )
            {
                if( TP_GetEventLockInf() == TP_EVT_UNLOCK )
                {
                    nRet = TP_DirectSrchCb( &sTouchEvt ) ;
                }
            }
        }
        else if ( (nEvtVal & TP_EVTFLG_ATTACH) == TP_EVTFLG_ATTACH )
        {
            TP_ClearEvtMsg( TP_EVTFLG_ATTACH );

            /* Close the caution window */
            display_off_usbport();
            
            drvnum = ((nEvtVal & TP_DEVADDR_MASK) >> TP_DEVADDR_SHIFT);
            if (USB_IF_MOUSE == gs_usb_dev_info[drvnum].interface)
            {
                display_on_mouse();
            }

        }
        else if ( (nEvtVal & TP_EVTFLG_DETACH) == TP_EVTFLG_DETACH )
        {
            TP_ClearEvtMsg( TP_EVTFLG_DETACH );

            /* Open the caution window */
            display_on_usbport();
        }
        else
        {
            /* do nothing */
        }
    }
}

/**************************************************************************//**
* Function Name: TP_SetFingerInfoUsb
* @brief         Branch process with device to set touch information to output table.
*
*                Description:<br>
*
* @param         [out]TP_TouchFinger_st *psTouch        : pointer to output table
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @param         [out]int_t *pnPenStatus                : pointer to pending status
* @param         [in]uint8_t *puReadBuf                 : pointer to receive buffer
* @param         [in]uint32_t drvnum                    : number of USB driver
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_SetFingerInfoUsb( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const uint32_t drvnum )
{
    if (USB_IF_MOUSE == gs_usb_dev_info[drvnum].interface)
    {
        TP_SetPositionUsbMouse(psTouch, psLastCoords, pnPenStatus, puReadBuf, drvnum );
    }
    else if (USB_IF_TOUCH == gs_usb_dev_info[drvnum].interface)
    {
        TP_SetPositionUsbTouch(psTouch, psLastCoords, pnPenStatus, puReadBuf, drvnum );
    }
    else
    {
        DBG_printf_ERR( "[ERROR] unsupported device \n" );
    }
}

/**************************************************************************//**
* Function Name: TP_EventHandleUsbMouse
* @brief         Handle event type of USB mouse.
*
*                Description:<br>
*
* @param         [out]TpEvt_EntryType *eEvent           : pointer to event variable
* @param         [in]int32_t nPosX                      : X-coordinate
* @param         [in]int32_t nPosY                      : Y-coordinate
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
SCOPE_STATIC int_t TP_EventHandleUsbMouse( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords )
{
    int_t                    nRet ;
    int_t                    nOver ;
    TPEVT_COORDINATES        Coords ;
    int_t   nScrWidth;
    int_t   nScrHeight;

    nRet    = 0;

    Coords.x = *nPosX ;
    Coords.y = *nPosY ;

    nOver = TP_CheckDiffMin( psLastCoords, &Coords, TPEVT_MOVE_MINTHD_USB ) ;
    if( 0 != nOver )
    {
        /* display on mouse cursor and change mouse position. */
        psLastCoords->x = *nPosX ;
        psLastCoords->y = *nPosY ;
        TP_GetScreenSize( &nScrWidth, &nScrHeight );
        change_mouse_position(psLastCoords->x, psLastCoords->y, nScrWidth, nScrHeight);
    }
    else
    {
        /* display on mouse cursor. */
        display_on_mouse();
    }

    return nRet ;
}


/**************************************************************************//**
* Function Name: TP_EventHandleUsbTouch
* @brief         Handle event type of USB touch.
*
*                Description:<br>
*
* @param         [out]TpEvt_EntryType *eEvent           : pointer to event variable
* @param         [in]int32_t nPosX                      : X-coordinate
* @param         [in]int32_t nPosY                      : Y-coordinate
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
SCOPE_STATIC int_t TP_EventHandleUsbTouch( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords )
{
    int_t                    nRet ;
    int_t                    nOver ;
    TPEVT_COORDINATES        Coords ;

    nRet    = 0;

    Coords.x = *nPosX ;
    Coords.y = *nPosY ;

    switch( *eEvent ){
        case TPEVT_ENTRY_DOWN :
            psLastCoords->x = *nPosX ;
            psLastCoords->y = *nPosY ;
            break ;
        case TPEVT_ENTRY_MOVE :
            nOver = TP_CheckDiffMin( psLastCoords, &Coords, TPEVT_MOVE_MINTHD_USB ) ;
            if( 0 != nOver )
            {
                /* display on mouse cursor and change mouse position. */
                psLastCoords->x = *nPosX ;
                psLastCoords->y = *nPosY ;
            }
            break ;
        case TPEVT_ENTRY_UP :
            break ;
        default :
            nRet = -1;
            break;
    }


    return nRet ;
}

/**************************************************************************//**
* Function Name: TP_SetPositionUsbMouse
* @brief         Set position information of USB mouse.
*
*                Description:<br>
*
* @param         [out]TP_TouchFinger_st *psTouch        : pointer to output table
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @param         [out]int_t *pnPenStatus                : pointer to pending status
* @param         [in]uint8_t *puReadBuf                 : pointer to receive buffer
* @param         [in]uint32_t drvnum                    : number of USB driver number
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_SetPositionUsbMouse( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const uint32_t drvnum )
{
    int16_t                  nXin;
    int16_t                  nYin;
    int32_t                  nXout;
    int32_t                  nYout;

    int16_t                  tmpPosX;
    int16_t                  tmpPosY;
    
    int_t                    nScrWidth;
    int_t                    nScrHeight;

    TP_GetPositionData(puReadBuf, &tmpPosX, &tmpPosY, gs_usb_dev_info[drvnum].mouse->data_len_pos, 
                       gs_usb_dev_info[drvnum].mouse->data_offset_posX1, gs_usb_dev_info[drvnum].mouse->data_offset_posY1);

    /* calculate coordinates from mouse movement */
    if (USB_POS_TYPE_RELATIVE == gs_usb_dev_info[drvnum].mouse->pos_data_type)
    {
        nXin = (int16_t)(psTouch->unPosX - (tmpPosX * gs_usb_dev_info[drvnum].mouse->cursor_speed) );
        nYin = (int16_t)(psTouch->unPosY - (tmpPosY * gs_usb_dev_info[drvnum].mouse->cursor_speed) );
    }
    else
    {
        TP_GetScreenSize( &nScrWidth, &nScrHeight );
        nXin = nScrWidth - (nXin * gs_usb_dev_info[drvnum].mouse->cursor_speed);
        nYin = nScrHeight - (nYin * gs_usb_dev_info[drvnum].mouse->cursor_speed);
    }       

    TP_LineUpPos( nXin, nYin, &nXout, &nYout );

    psTouch->unPosX = (uint16_t)nXout;
    psTouch->unPosY = (uint16_t)nYout;

    /* state initialize */
    psTouch->eState = TPEVT_ENTRY_NONE;

    /* mouse moving */
    TP_EventHandleUsbMouse( &psTouch->eState, &nXout, &nYout, psLastCoords );

    if( *pnPenStatus == TP_PENSTATUS_PUSH )
    {
        if (0x00 == (puReadBuf[gs_usb_dev_info[drvnum].mouse->data_offset_click] & gs_usb_dev_info[drvnum].mouse->left_click_bit))
        {
            /* release left click, EVENT : UP */
            *pnPenStatus    = TP_PENSTATUS_RELEASE;        /* Touch Panel pen status, 0 : up */
            psTouch->eState = TPEVT_ENTRY_UP;
        }
        else
        {
            /* hold left click, EVENT : MOVE */
            psTouch->eState = TPEVT_ENTRY_MOVE;
        }
    }
    else
    {
        if (0x01 == (puReadBuf[gs_usb_dev_info[drvnum].mouse->data_offset_click] & gs_usb_dev_info[drvnum].mouse->left_click_bit))
        {
            /* push left click, EVENT : DOWN */
            *pnPenStatus    = TP_PENSTATUS_PUSH;        /* Touch Panel pen status, 1 : down */
            psTouch->eState = TPEVT_ENTRY_DOWN;
        }
    }
}

/**************************************************************************//**
* Function Name: TP_SetPositionUsbTouch
* @brief         Set position information of USB touch.
*
*                Description:<br>
*
* @param         [out]TP_TouchFinger_st *psTouch        : pointer to output table
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @param         [out]int_t *pnPenStatus                : pointer to pending status
* @param         [in]uint8_t *puReadBuf                 : pointer to receive buffer
* @param         [in]uint32_t drvnum                    : number of USB driver number
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_SetPositionUsbTouch( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const uint32_t drvnum )
{
    int32_t                  nXin;
    int32_t                  nYin;
    int32_t                  nXout;
    int32_t                  nYout;

    int16_t                  tmpPosX;
    int16_t                  tmpPosY;

    int_t                    nScrWidth;
    int_t                    nScrHeight;
    
    /* check finger data */
    if (0 != puReadBuf[gs_usb_dev_info[drvnum].touch->finger_info_pos])
    {
        TP_GetPositionData(puReadBuf, &tmpPosX, &tmpPosY, gs_usb_dev_info[drvnum].touch->data_len_pos, 
                           gs_usb_dev_info[drvnum].touch->data_offset_posX1, gs_usb_dev_info[drvnum].touch->data_offset_posY1);
        
        /* convert position data to coordinates corresponding to screen resolution */
        nXin = ((((gs_usb_dev_info[drvnum].touch->dev_resolutionX * MULTIPLE_VALUE_INT_CALC) / 
                   gs_usb_dev_info[drvnum].touch->dev_max_posX) * tmpPosX) / MULTIPLE_VALUE_INT_CALC);
        nYin = ((((gs_usb_dev_info[drvnum].touch->dev_resolutionY * MULTIPLE_VALUE_INT_CALC) / 
                   gs_usb_dev_info[drvnum].touch->dev_max_posY) * tmpPosY) / MULTIPLE_VALUE_INT_CALC);

        if (USB_POS_TYPE_ABSOLUTE == gs_usb_dev_info[drvnum].touch->pos_data_type)
        {
            TP_GetScreenSize( &nScrWidth, &nScrHeight );
            nXin = nScrWidth - nXin;
            nYin = nScrHeight - nYin;
        }
        else
        {
            nXin = (int16_t)(psTouch->unPosX - nXin );
            nYin = (int16_t)(psTouch->unPosY - nYin );
        }

        TP_LineUpPos( (int16_t)nXin, (int16_t)nYin, &nXout, &nYout );

        psTouch->unPosX = (uint16_t)nXout;
        psTouch->unPosY = (uint16_t)nYout;

        /* state initialize */
        psTouch->eState = TPEVT_ENTRY_NONE;

        if( *pnPenStatus == TP_PENSTATUS_RELEASE )
        {
            /* EVENT : DOWN */
            *pnPenStatus    = TP_PENSTATUS_PUSH;        /* Touch Panel pen status, 1 : down */
            psTouch->eState = TPEVT_ENTRY_DOWN;
        }
        else
        {
            /* EVENT : MOVE */
            psTouch->eState = TPEVT_ENTRY_MOVE;
        }
        /* mouse moving */
        TP_EventHandleUsbTouch( &psTouch->eState, &nXout, &nYout, psLastCoords );
    }
    else
    {
        psTouch->unPosX = psLastCoords->x;
        psTouch->unPosY = psLastCoords->y;
        
        if( *pnPenStatus == TP_PENSTATUS_PUSH )
        {
            /* EVENT : UP */
            *pnPenStatus    = TP_PENSTATUS_RELEASE;        /* Touch Panel pen status, 0 : up */
            psTouch->eState = TPEVT_ENTRY_UP;
        }
        else
        {
            /* EVENT : NONE */
            psTouch->eState = TPEVT_ENTRY_NONE;
        }
    }
}

/**************************************************************************//**
* Function Name: TP_GetPositionData
* @brief         Get position data from USB Buffer.
*
*                Description:<br>
*
* @param         [in]uint8_t *puReadBuf                 : pointer to receive buffer
* @param         [out]int_t  *PosX                      : X coordinate potision data
* @param         [out]int_t  *PosY                      : Y coordinate potision data
* @param         [in]uint32_t data_len                  : deta length of coordinate value in buffer
* @param         [in]uint32_t posx_offset               : position of X coordinate data in buffer
* @param         [in]uint32_t posy_offset               : position of Y coordinate data in buffer
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_GetPositionData(const uint8_t* const puReadBuf, int16_t *PosX, int16_t *PosY, const uint32_t data_len, uint32_t posx_offset, uint32_t posy_offset )
{
    int16_t tmpPosX = 0;
    int16_t tmpPosY = 0;

    if (1 == data_len)
    {
        /* get 8bit X axis coordinate data, it's a data such as mouse mainly */
        /* check sign bit on X axis data buffer */
        if (0 == (puReadBuf[posx_offset] & SIGN_BIT_MASK_8BIT))
        {
            /* convert 8bit unsigned value to a 16bit positive value */
            *PosX = puReadBuf[posx_offset];
        }
        else
        {
            /* convert 8bit unsigned value to a 16bit negative value */
            *PosX = 0 + (int8_t)puReadBuf[posx_offset];
        }
    }
    else
    {
        /* get 16bit X axis coordinate data, it's a data such as touch mainly */
        /* check sign bit on X axis data buffer */
        if (0 == (puReadBuf[posx_offset] & SIGN_BIT_MASK_16BIT))
        {
            /* convert 8bit unsigned value to a 16bit positive value */
            *PosX = (puReadBuf[posx_offset + 1] << 8) + puReadBuf[posx_offset];
        }
        else
        {
            /* convert 8bit unsigned value to a 16bit negative value */
            *PosX = (int16_t)((puReadBuf[posx_offset + 1] << 8) + puReadBuf[posx_offset]);
        }
    }

    if (1 == data_len)
    {
        /* get 8bit Y axis coordinate data, it's a data such as mouse mainly */
        /* check sign bit on Y axis data buffer */
        if (0 == (puReadBuf[posy_offset] & SIGN_BIT_MASK_8BIT))
        {
            /* convert 8bit unsigned value to a 16bit positive value */
            *PosY = puReadBuf[posy_offset];
        }
        else
        {
            /* convert 8bit unsigned value to a 16bit negative value */
            *PosY = 0 + (int8_t)puReadBuf[posy_offset];
        }
    }
    else
    {
        /* get 16bit Y axis coordinate data, it's a data such as touch mainly */
        /* check sign bit on Y axis data buffer */
        if (0 == (puReadBuf[posy_offset] & SIGN_BIT_MASK_16BIT))
        {
            /* convert 8bit unsigned value to a 16bit positive value */
            *PosY = (puReadBuf[posy_offset + 1] << 8) + puReadBuf[posy_offset];
        }
        else
        {
            /* convert 8bit unsigned value to a 16bit negative value */
            *PosY = (int16_t)((puReadBuf[posy_offset + 1] << 8) + puReadBuf[posy_offset]);
        }
    }

}



