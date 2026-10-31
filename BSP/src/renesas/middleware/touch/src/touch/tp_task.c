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
* @file         tp_task.c
* @brief        TouchPanel Driver task functions
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
#include    "r_usb_gui_control.h"

/******************************************************************************
Macro definitions
******************************************************************************/

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

TP_TASKSTAT_et  eTpTaskStat;

/**************************************************************************//**
* Function Name: TP_Task
* @brief         Task function.
*
*                Description:<br>
*
* @param         [in]void const *vpArg  : pointer to argument
* @retval        None.
******************************************************************************/
void TP_Task(void *params)
{
    UNUSED_PARAM(params);

#if (LCD_CONTROLL_DEV == LCD_CONTROLL_RIIC)
    TP_TaskProcessRiic();
#else /* (LCD_CONTROLL_DEV == LCD_CONTROLL_RIIC) */
    TP_TaskProcessUsb();
#endif /* (LCD_CONTROLL_DEV == LCD_CONTROLL_RIIC) */
}

/**************************************************************************//**
* Function Name: TP_Kitb01_GetTaskStatus
* @brief         Get task status.
*
*                Description:<br>
*
* @param         None.
* @retval        Task status.
******************************************************************************/
TP_TASKSTAT_et  TP_GetTaskStatus( void )
{
    return eTpTaskStat;
}

/**************************************************************************//**
* Function Name: TP_LineUpPos
* @brief         Line up touch position.
*
*                Description:<br>
*
* @param         [in]int16_t nXin       : before X-coordinate
* @param         [in]int16_t nYin       : before Y-coordinate
* @param         [out]int32_t *pnXout   : pointer to after X-coordinate
* @param         [out]int32_t *pnYout   : pointer to after Y-coordinate
* @retval        None.
******************************************************************************/
void TP_LineUpPos( const int16_t nXin, const int16_t nYin, int32_t *pnXout, int32_t *pnYout )
{
    int_t   nScrWidth;
    int_t   nScrHeight;

    TP_GetScreenSize( &nScrWidth, &nScrHeight );
#if (LCD_PANEL == LCD_PANEL_RSK)
    if (1 == tp_get_app_board_version())
    {
        *pnXout = nXin;
        *pnYout = nYin;
    }
    else if (2 == tp_get_app_board_version())
    {
        *pnXout = nScrWidth - nXin;
        *pnYout = nScrHeight - nYin;
    }
    else
    {
        *pnXout = nXin;
        *pnYout = nYin;
    }
#else
    *pnXout = nXin;
    *pnYout = nYin;
#endif

    DBG_printf_DBG( "[TPDBG] S(%d:%d), I(%d:%d), O(%d:%d)\n", nScrWidth, nScrHeight, nXin, nYin, *pnXout, *pnYout );
    if(*pnXout < 0)
    {
        *pnXout = 0 ;
    }
    else if(*pnXout >= nScrWidth)
    {
        *pnXout = nScrWidth - 1 ;
    }
    else
    {
        /* do nothing */
    }

    if(*pnYout < 0)
    {
        *pnYout = 0 ;
    }
    else if (*pnYout >= nScrHeight)
    {
        *pnYout = nScrHeight - 1 ;
    }
    else
    {
        /* do nothing */
    }
}

/**************************************************************************//**
* Function Name: TP_CheckDiffMin
* @brief         Check amount of slide.
*
*                Description:<br>
*
* @param         [in]TPEVT_COORDINATES *psPox_1     : pointer to last coordinate
* @param         [in]TPEVT_COORDINATES *psPox_2     : pointer to current coordinate
* @param         [in]int32_t unThreshold            : threshold value
* @retval          1 : Move.
*                  0 : Not move.
******************************************************************************/
int_t TP_CheckDiffMin( const TPEVT_COORDINATES* const psPox_1, const TPEVT_COORDINATES* const psPox_2, const uint32_t unThreshold )
{
    int32_t     nX, nY;
    uint32_t    unData;

    nX = psPox_1->x - psPox_2->x ;
    nY = psPox_1->y - psPox_2->y ;
    unData = ( uint32_t )( nX * nX ) + ( uint32_t )( nY * nY ) ;

    return(( unData > unThreshold ) ? 1 : 0 ) ;
}


/**************************************************************************//**
* Function Name: TP_DirectSrchCb
* @brief         Search and call callback function.
*
*                Description:<br>
*
* @param         [in]TP_TouchEvent_st   : pointer to touch information
* @retval        Found event number
******************************************************************************/
int_t TP_DirectSrchCb( TP_TouchEvent_st *psTouchEvt )
{
    int_t           nRet;
    int_t           nI, nJ;
    TPEVT_ENTRY     *psEntry;

    nRet    = 0;

    DBG_printf_LOG( "[TOUCH],%d,%d,%d,%d,%d,%d\n",
                    psTouchEvt->sFinger[0].eState, psTouchEvt->sFinger[0].unPosX, psTouchEvt->sFinger[0].unPosY,
                    psTouchEvt->sFinger[1].eState, psTouchEvt->sFinger[1].unPosX, psTouchEvt->sFinger[1].unPosY );

    psEntry = TP_GetEventTable( 0 );
    for( nI = 0 ; nI < TPEVT_ENTRY_MAX ; nI++ )
    {
        if( psEntry->evtlock != TP_EVT_UNLOCK )
        {
            continue ;                  /* Locked */
        }
        for( nJ = 0; nJ < TP_TOUCHNUM_MAX; nJ ++ )
        {
            if( (( psTouchEvt->sFinger[nJ].eState & psEntry->mode ) != TPEVT_ENTRY_NONE) &&
                (( psTouchEvt->sFinger[nJ].unPosX >= psEntry->st.x ) && ( psTouchEvt->sFinger[nJ].unPosX < psEntry->ed.x )) &&
                (( psTouchEvt->sFinger[nJ].unPosY >= psEntry->st.y ) && ( psTouchEvt->sFinger[nJ].unPosY < psEntry->ed.y )) )
            {
                /** Event notification callback function */
                psEntry->function( nI, psTouchEvt );
                nRet ++;                       /* Exist */
                break;
            }
        }
        psEntry++;
    }

    return nRet ;
}


