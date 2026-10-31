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
* @file         tp_task_riic.c
* @brief        TouchPanel Driver task functions with RIIC
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
#include    "lcd_controller_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/

#define     TPEVT_MOVE_MINTHD_RIIC   (2UL)
#define     TPEVT_PRESS_THD     (1000)

/******************************************************************************
 Enumerated Types
******************************************************************************/


/******************************************************************************
 Structures
******************************************************************************/

/***********************************************************************************
 Global Vaiables
***********************************************************************************/

/*******************************************************************************
 Private global variables and functions
*******************************************************************************/
SCOPE_STATIC void TP_SetFingerInfoRiic( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const int_t nFinger );
SCOPE_STATIC int_t TP_EventHandleRiic( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords );

extern TP_TASKSTAT_et  eTpTaskStat;

/**************************************************************************//**
* Function Name: TP_TaskProcessRiic
* @brief         Task function process of RIIC.
*
*                Description:<br>
*
* @param         [in]void const *vpArg  : pointer to argument
* @retval        None.
******************************************************************************/
void TP_TaskProcessRiic(void)
{
    int_t               nRet;
    int32_t             nEvtVal;
    uint8_t             auReadBuf[0x20];
    TP_TouchEvent_st    sTouchEvt;
    TPEVT_COORDINATES   sLastCoords[TP_TOUCHNUM_MAX];
    int_t               nPenStatus[TP_TOUCHNUM_MAX];
    int_t               nI, nJ;
    uint8_t             nTouchEndFlag;
    uint32_t            uiCnt;
    int_t               nScrWidth;
    int_t               nScrHeight;

    TP_GetScreenSize( &nScrWidth, &nScrHeight );

    eTpTaskStat = TP_TASKSTAT_ACTIVE;
    memset( &sTouchEvt, 0, sizeof(TP_TouchEvent_st) );
    memset( &sLastCoords, 0, sizeof(TPEVT_COORDINATES) );
    memset( nPenStatus, 0, sizeof(nPenStatus) );

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
            TP_ClearEvtMsg( TP_EVTFLG_PENIRQ );
            while( 1 )
            {
                /* get touch information via RIIC */
                nRet = R_LCD_ReadCmd(LCD_SLAVE_ADDRESS, 0, auReadBuf, 9);
                if( nRet != DEVDRV_SUCCESS )
                {
                    DBG_printf_ERR( "[ERROR] touch status is not got\n" );
                    R_LCD_Restart();
                    break;
                }
                else
                {
                    /* make touch information for output */
                    for( nI = 0; nI < TP_TOUCHNUM_MAX; nI ++ )
                    {
                        TP_SetFingerInfoRiic( &sTouchEvt.sFinger[nI], &sLastCoords[nI], &nPenStatus[nI], auReadBuf, nI );
                    }
                }
                /* determine unmoved finger's state */
                for( nI = 0; nI < TP_TOUCHNUM_MAX; nI ++ )
                {
                    if( sTouchEvt.sFinger[nI].eState == TPEVT_ENTRY_UNKNOWN )
                    {
                        sTouchEvt.sFinger[nI].eState = TPEVT_ENTRY_NONE;
                        for( nJ = 0; nJ < TP_TOUCHNUM_MAX; nJ ++ )
                        {
                            if( (sTouchEvt.sFinger[nJ].eState == TPEVT_ENTRY_UP  ) ||
                                (sTouchEvt.sFinger[nJ].eState == TPEVT_ENTRY_DOWN) ||
                                (sTouchEvt.sFinger[nJ].eState == TPEVT_ENTRY_MOVE) )
                            {
                                sTouchEvt.sFinger[nI].eState = TPEVT_ENTRY_MOVE;
                                break;
                            }
                        }
                    }
                }

                /* refer to previous coordinate if state is UP */
                for( nI = 0; nI < TP_TOUCHNUM_MAX; nI ++ )
                {
                    if( sTouchEvt.sFinger[nI].eState == TPEVT_ENTRY_UP )
                    {
                        sTouchEvt.sFinger[nI].unPosX = sLastCoords[nI].x;
                        sTouchEvt.sFinger[nI].unPosY = sLastCoords[nI].y;
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

                /* touch flag clear */
                nTouchEndFlag = 0;

                /* move to waiting mode when touch is released */
                for(uiCnt = 0; uiCnt < TP_TOUCHNUM_MAX; uiCnt ++){
                    nTouchEndFlag |= nPenStatus[uiCnt];
                }

                if( (nRet >= 0) && (nTouchEndFlag == 0) )
                {
                    DBG_printf_DBG( "[ESCAPE]\n" );
                    break;
                }
                R_OS_TaskSleep( TP_TASK_DELAY );
            }
            /* enable LCD touch event interrupt */
            nRet = R_LCD_StartInt( LCDEVT_ENTRY_TP );
            if( nRet < 0 )
            {
                DBG_printf_ERR( "[ERROR] TP interrupt is not started\n" );
                R_LCD_Restart();
            }
        }
        else
        {
            /* do nothing */
        }
    }
}

/**************************************************************************//**
* Function Name: TP_SetFingerInfoRiic
* @brief         Set touch information to output table.
*
*                Description:<br>
*
* @param         [out]TP_TouchFinger_st *psTouch        : pointer to output table
* @param         [out]TPEVT_COORDINATES *psLastCoords   : pointer to last coordinate
* @param         [out]int_t *pnPenStatus                : pointer to pending status
* @param         [in]uint8_t *puReadBuf                 : pointer to receive buffer
* @param         [in]int_t nFinger                      : number of finger index
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_SetFingerInfoRiic( TP_TouchFinger_st *psTouch, TPEVT_COORDINATES *psLastCoords, int_t *pnPenStatus, const uint8_t* const puReadBuf, const int_t nFinger )
{
    int16_t                  nXin;
    int16_t                  nYin;
    int32_t                  nXout;
    int32_t                  nYout;

    uint8_t                  unState;
    int32_t                  nPressure;
    nPressure = TPEVT_PRESS_THD;

    /* decide touch coordinate */
    nXin = ((puReadBuf[1 + (nFinger * 6)] & 0x7) << 8) + puReadBuf[2 + (nFinger * 6)];
    nYin = ((puReadBuf[3 + (nFinger * 6)] & 0x7) << 8) + puReadBuf[4 + (nFinger * 6)];

    TP_LineUpPos( nXin, nYin, &nXout, &nYout );
    psTouch->unPosX = (uint16_t)nXout;
    psTouch->unPosY = (uint16_t)nYout;

    if(puReadBuf[0] > nFinger){
        unState = 1;
    }
    else{
        unState = 0;
    }

    if( unState == 0 )
    {
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
    else
    {
        if( nPressure < TPEVT_PRESS_THD )
        {
            /* EVENT : NONE */
            psTouch->eState = TPEVT_ENTRY_NONE;
        }
        else if( *pnPenStatus == TP_PENSTATUS_RELEASE )
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
        (void)TP_EventHandleRiic( &psTouch->eState, &nXout, &nYout, psLastCoords );
    }
}

/**************************************************************************//**
* Function Name: TP_EventHandleRiic
* @brief         Handle event type of RIIC.
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
SCOPE_STATIC int_t TP_EventHandleRiic( TpEvt_EntryType *eEvent, const int32_t* const nPosX, const int32_t* const nPosY, TPEVT_COORDINATES *psLastCoords )
{
    int_t                    nRet ;
    int_t                    nOver ;
    TPEVT_COORDINATES        Coords ;
    int_t   nScrWidth;
    int_t   nScrHeight;

    nRet    = 0;

    Coords.x = *nPosX ;
    Coords.y = *nPosY ;
    switch( *eEvent ){
        case TPEVT_ENTRY_DOWN :
            psLastCoords->x = *nPosX ;
            psLastCoords->y = *nPosY ;
            break ;
        case TPEVT_ENTRY_MOVE :
            nOver = TP_CheckDiffMin( psLastCoords, &Coords, TPEVT_MOVE_MINTHD_RIIC ) ;
            if( nOver == 0 )
            {
                *eEvent = TPEVT_ENTRY_UNKNOWN;
            }
            else
            {
                psLastCoords->x = *nPosX ;
                psLastCoords->y = *nPosY ;
                TP_GetScreenSize( &nScrWidth, &nScrHeight );
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


