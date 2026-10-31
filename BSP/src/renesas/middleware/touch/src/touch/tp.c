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
* @file         tp.c
* @brief        TouchPanel Driver functions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include    <stdio.h>
#include    <string.h>

#include    "r_typedefs.h"
#include    "compiler_settings.h"
#include    "r_os_abstraction_api.h"
#include    "lcd_controller_if.h"
#include    "tp.h"
#include    "tp_task.h"

#if (LCD_PANEL == LCD_PANEL_RSK)
#include    "r_riic_drv_api.h"
#endif

/******************************************************************************
 Macro definitions
******************************************************************************/
#define LCD_EEPROM_ADDRESS (0xAA)

/******************************************************************************
 Enumerated Types
******************************************************************************/


/******************************************************************************
 Structures
******************************************************************************/


/******************************************************************************
 Global Vaiables
******************************************************************************/
int32_t   sTpSemIdTsk;
os_task_t *p_os_task;
int32_t   g_eep_sem_id_acc;

/*******************************************************************************
 Private global variables and functions
*******************************************************************************/
SCOPE_STATIC void TP_Callback_LcdEvent( uint32_t drvnum );

SCOPE_STATIC uint32_t unTpEvtMsg;
SCOPE_STATIC TpEvt_LockState TpEvtLockInf ;     /*!< Touch panel all event lock state */
/*! Touch panel event entry data
 @n The length of the array is not too long, so data chain structure is not applied. */
SCOPE_STATIC TPEVT_ENTRY TpEvt_Entry[ TPEVT_ENTRY_MAX ] ;

/*! Touch Panel screen width */
SCOPE_STATIC int_t    ScreenWidth ;
/*! Touch Panel screen height */
SCOPE_STATIC int_t    ScreenHeight ;

SCOPE_STATIC int_t    nEvtEntryId;

#if (LCD_PANEL == LCD_PANEL_RSK)
SCOPE_STATIC int_t    sRskRftAppBoardVersion;
#endif

/**************************************************************************//**
* Function Name: TP_Init
* @brief         Initialize TouchPanel driver.
*
*                Description:<br>
*                
* @param         None. 
* @retval        None.
******************************************************************************/
void TP_Init( void )
{

#if (LCD_PANEL == LCD_PANEL_RSK)
	int_t       nret = 0;
    int_t       nriic_handle;
    st_r_drv_riic_transfer_t eep_i2c_info;
    uint8_t     untft_ver_info;
#endif
    
    R_LCD_Init();

    memset( &TpEvt_Entry, 0, sizeof TpEvt_Entry ) ;
    nEvtEntryId  = -1;
    TpEvtLockInf = TP_EVT_UNLOCK ;          /** Unlocked */
#if (LCD_PANEL == LCD_PANEL_RSK)
    /** create synchronous semaphore for access to EEPROM */
    if( R_OS_SemaphoreCreate( &g_eep_sem_id_acc, 1 ) != true)
    {
    	DBG_printf_ERR
        ( "[ERROR] semaphore for acces to EEPROM is not created\n" );
        nret = -1;
    }
    
    /** Open RIIC channel for access to EEPROM */
    if( nret >= 0 )
    {
        if( R_OS_SemaphoreWait( &g_eep_sem_id_acc, 0 ) != true)
        {
        	DBG_printf_ERR( "[ERROR] semaphore for task is not get\n" );
            nret = -1;
        }
        else
        {
            nret = open(DEVICE_INDENTIFIER "riic3", O_RDWR);
            R_OS_SemaphoreRelease( &g_eep_sem_id_acc );
        }
    
        if( nret == (-1) )
        {
        	DBG_printf_ERR
            ( "[ERROR] RIIC driver channel is not opened -- ret:%d\n", nret );
        }
        else
        {
            nriic_handle = nret;
        }
    }
    
    if( nret >= 0 )
    {        
        eep_i2c_info.device_address = LCD_EEPROM_ADDRESS;
        eep_i2c_info.sub_address_type = RIIC_SUB_ADDR_WIDTH_8_BITS;
        eep_i2c_info.sub_address = 0x00;
        eep_i2c_info.number_of_bytes = 1;
        eep_i2c_info.p_data_buffer = &untft_ver_info;
        
        nret = control(nriic_handle, CTL_RIIC_READ, &eep_i2c_info);
        if( nret != DRV_ERROR )
        {
        	DBG_printf_ERR( "[OK] Touch panel version information read\n" );
            
            if (0x00 == untft_ver_info)
            {
                /* The read byte is 0x00 - version 1 board */
                sRskRftAppBoardVersion = 1;
            }
            else if (0xFF == untft_ver_info)
            {
                /* The read byte is 0xFF - version 2 board */
                sRskRftAppBoardVersion = 2;
            }
            else
            {
                /* The version is unknown, default to version 1 */
                sRskRftAppBoardVersion = 1;
            }
        }
        else
        {
        	DBG_printf_ERR
            ( "[ERROR] EEPROM slave address set is failed -- ret:%d\n", nret );
            /* No EEPROM is fitted - so version 1 board */
            sRskRftAppBoardVersion = 1;
        }
    }
    
    if( R_OS_SemaphoreWait( &g_eep_sem_id_acc, 0 ) != true)
    {
    	DBG_printf_ERR( "[ERROR] semaphore for task is not get\n" );
        nret = -1;
    }
    else
    {
        nret = close( nriic_handle );
        R_OS_SemaphoreRelease( &g_eep_sem_id_acc );
        
        R_OS_SemaphoreDelete( &g_eep_sem_id_acc );
    }
#endif
}


/**************************************************************************//**
* Function Name: TP_Open
* @brief         Open TouchPanel driver.
*
*                Description:<br>
*                
* @param         [in]int_t nWidth       : screen width
* @param         [in]int_t nHeight      : screen height
* @param         [in]uint32_t unIrqLv   : IRQ interrupt level
* @param         [in]int16_t nTskPri    : task priority
* @param         [in]uint32_t unTskStk  : task stack size
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_Open( const int_t nWidth, const int_t nHeight, const uint32_t unIrqLv, const int16_t nTskPri, const uint32_t unTskStk )
{
    int_t       nRet;

    nRet            = 0;
    ScreenWidth     = nWidth ;
    ScreenHeight    = nHeight ;
    unTpEvtMsg      = 0;
    uint32_t unTskStkTmp = unTskStk;

    /** create synchronous semaphore for task */
    if( nRet >= 0 )
    {
        if( R_OS_SemaphoreCreate( &sTpSemIdTsk, 1 ) != true)
        {
            DBG_printf_ERR( "[ERROR] semaphore for task is not created\n" );
            nRet = -1;
        }
        if( R_OS_SemaphoreWait( &sTpSemIdTsk, 0 ) != true)
        {
            DBG_printf_ERR( "[ERROR] semaphore for task is not get\n" );
            nRet = -1;
        }
    }

    /** create task */
    if( nRet >= 0 )
    {
        if( unTskStkTmp == R_OS_ABSTRACTION_TINY_STACK_SIZE )
        {
            unTskStkTmp = R_OS_ABSTRACTION_SMALL_STACK_SIZE;
        }
        p_os_task = R_OS_TaskCreate("Touch", TP_Task, NULL, unTskStkTmp, nTskPri );
        if(( p_os_task == NULL )||(R_OS_ABSTRACTION_INVALID_HANDLE == p_os_task))
        {
            DBG_printf_ERR( "[ERROR] task is not created\n" );
            nRet = -1;
            R_OS_SemaphoreDelete( &sTpSemIdTsk );
        }
        else
        {
            DBG_printf_LOG( "[PRIORITY] %d\n", nTskPri );
        }
    }

    if( nRet >= 0 )
    {
        nRet = R_LCD_Open( unIrqLv, nTskPri, unTskStkTmp );
        if( nRet < 0 )
        {
            R_OS_TaskDelete( p_os_task );
            R_OS_SemaphoreDelete( &sTpSemIdTsk );
        }
    }

    if( nRet >= 0 )
    {
        nRet = R_LCD_EventEntry( LCDEVT_ENTRY_TP, &TP_Callback_LcdEvent );
        if( nRet < 0 )
        {
            DBG_printf_ERR( "[ERROR] LCD event entry is not registered\n" );
            (void)R_LCD_Close();
            R_OS_TaskDelete( p_os_task );
            R_OS_SemaphoreDelete( &sTpSemIdTsk );
        }
        else
        {
            nEvtEntryId = nRet;
        }
    }

    return nRet;
}


/**************************************************************************//**
* Function Name: TP_Close
* @brief         Close TouchPanel driver.
*
*                Description:<br>
*                
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_Close( void )
{
    int_t           nRet;
    int_t           nI;
    TP_TASKSTAT_et  eTaskStat;

    nRet = R_LCD_Close();

    nRet |= R_LCD_EventErase( nEvtEntryId );
    if( nRet >= 0 )
    {
        nEvtEntryId = -1;
    }
    else
    {
        DBG_printf_ERR( "[ERROR] LCD event entry is not erased\n" );
    }

    /* erase all events */
    for( nI = 0; nI < TPEVT_ENTRY_MAX; nI ++ )
    {
        nRet = TP_EventErase( nI );
        if( nRet < 0)
        {
            DBG_printf_ERR( "[ERROR] erase event is not complete(nI=%d)\n",nI );
            break;
        }
    }

    /* stop TP task */
    nRet |= TP_SendEvtMsg( TP_EVTFLG_EXIT );
    if( nRet < 0 )
    {
        DBG_printf_ERR( "[ERROR] exit event message is not sent\n" );
    }
    else
    {
        while( 1 )
        {
            eTaskStat = TP_GetTaskStatus();
            if( eTaskStat == TP_TASKSTAT_IDLE )
            {
                break;
            }
            R_OS_TaskSleep( 10 );
        }
    }

    R_OS_SemaphoreDelete( &sTpSemIdTsk );

    return nRet;
}


/**************************************************************************//**
* Function Name: TP_EventEntry
* @brief         Enter callback event to event table.
*
*                Description:<br>
*                
* @param         [in]TpEvt_EntryType eMode  : event type
* @param         [in]int32_t nPosX          : X-coordinate of LCD area
* @param         [in]int32_t nPosY          : Y-coordinate of LCD area
* @param         [in]int32_t nWidth         : width of LCD area
* @param         [in]int32_t nHeight        : height of LCD area
* @param         [in]void *function         : callback function
* @retval          0..(TPEVT_ENTRY_MAX-1) : event ID.
*                 -1                      : Error occured.
******************************************************************************/
int_t TP_EventEntry( const TpEvt_EntryType eMode, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight, const TpCBFunc function )
{
    int_t nI ;
    TPEVT_ENTRY *Entry ;

    Entry = &TpEvt_Entry[ 0 ] ;
    for( nI = 0 ; nI < TPEVT_ENTRY_MAX ; nI++)
    {
        if( Entry->mode == TPEVT_ENTRY_NONE )      /*Serch a no entry space */
        {
            Entry->mode = eMode ;
            Entry->st.x = nPosX ;
            Entry->st.y = nPosY ;
            Entry->ed.x = nPosX + nWidth ;
            Entry->ed.y = nPosY + nHeight ;
            Entry->function = function ;
            break ;
        }

        Entry++;
    }
    /** Return ERROR ( -1 ), when the event entry fails. */
    return(( nI < TPEVT_ENTRY_MAX ) ? nI : ( -1 )) ;
}


/**************************************************************************//**
* Function Name: TP_EventErase
* @brief         Erase callback event from event table.
*
*                Description:<br>
*                
* @param         [in]int_t nId              : event ID
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_EventErase( const int_t nId )
{
    int_t ercd = 0;

    if(( nId < 0 ) || ( nId >= TPEVT_ENTRY_MAX ))
    {
        ercd = -1;         /* id error */
    }
    else
    {
        TpEvt_Entry[ nId ].mode = TPEVT_ENTRY_NONE ;
    }

    return ercd;
}


/**************************************************************************//**
* Function Name: TP_ChangeEventEntry
* @brief         Change Entered callback event.
*
*                Description:<br>
*                
* @param         [in]int_t nId              : event ID
* @param         [in]int32_t nPosX          : X-coordinate of LCD area
* @param         [in]int32_t nPosY          : Y-coordinate of LCD area
* @param         [in]int32_t nWidth         : width of LCD area
* @param         [in]int32_t nHeight        : height of LCD area
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_ChangeEventEntry( const int_t nId, const int32_t nPosX, const int32_t nPosY, const int32_t nWidth, const int32_t nHeight )
{
    TPEVT_ENTRY *Entry ;
    int_t ercd = 0;

    if(( nId < 0 ) || ( nId >= TPEVT_ENTRY_MAX ))
    {
        ercd = -1;         /* id error */
    }
    else
    {

        Entry = &TpEvt_Entry[ nId ] ;

        if( Entry->mode == TPEVT_ENTRY_NONE )
        {
            ercd = -1;     /* no entry */
        }
        else
        {
            Entry->st.x = nPosX ;
            Entry->st.y = nPosY ;
            Entry->ed.x = nPosX + nWidth ;
            Entry->ed.y = nPosY + nHeight;
        }
    }

    return ercd;
}


/**************************************************************************//**
* Function Name: TP_EventLockAll
* @brief         Lock all callback event.
*
*                Description:<br>
*                
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_EventLockAll( void )
{
    int_t   nRet;
    int_t   nI;

    for( nI = 0; nI < TPEVT_ENTRY_MAX; nI ++ )
    {
        nRet = TP_EventLock( nI );
        if( nRet < 0 )
        {
            break;
        }
    }

    return nRet;
}


/**************************************************************************//**
* Function Name: TP_EventUnlockAll
* @brief         Unlock all callback event.
*
*                Description:<br>
*                
* @param         None.
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_EventUnlockAll( void )
{
    int_t   nRet;
    int_t   nI;

    for( nI = 0; nI < TPEVT_ENTRY_MAX; nI ++ )
    {
        nRet = TP_EventUnlock( nI );
        if( nRet < 0 )
        {
            break;
        }
    }

    return nRet;
}


/**************************************************************************//**
* Function Name: TP_EventLock
* @brief         Lock assigned callback event.
*
*                Description:<br>
*                
* @param         [in]int_t nId              : event ID
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_EventLock( const int_t nId )
{
    int_t ercd = 0;

    if(( nId < 0 ) || ( nId >= TPEVT_ENTRY_MAX ))
    {
        ercd = -1;         /* id error */
    }
    else
    {
        TpEvt_Entry[ nId ].evtlock = TP_EVT_LOCK ;
    }

    return ercd;
}


/**************************************************************************//**
* Function Name: TP_EventUnlock
* @brief         Unlock assigned callback event.
*
*                Description:<br>
*                
* @param         [in]int_t nId              : event ID
* @retval          0 : Operation successfull.
*                 -1 : Error occured.
******************************************************************************/
int_t TP_EventUnlock( const int_t nId )
{
    int_t ercd = 0;

    if(( nId < 0 ) || ( nId >= TPEVT_ENTRY_MAX ))
    {
        ercd = -1;         /* id error */
    }
    else
    {
        TpEvt_Entry[ nId ].evtlock = TP_EVT_UNLOCK ;
    }

    return ercd;
}


/**************************************************************************//**
* Function Name: TP_GetEventTable
* @brief         Get assigned callback event.
*
*                Description:<br>
*                
* @param         [in]int_t nId              : event ID
* @retval        pointer to event.
******************************************************************************/
TPEVT_ENTRY *TP_GetEventTable( const int_t nId )
{
    return &TpEvt_Entry[ nId ];
}


/**************************************************************************//**
* Function Name: TP_GetEventLockInf
* @brief         Get state of total callback event.
*
*                Description:<br>
*                
* @param         None.
* @retval        state of total callback event.
******************************************************************************/
TpEvt_LockState TP_GetEventLockInf( void )
{
    return TpEvtLockInf;
}


/**************************************************************************//**
* Function Name: TP_GetScreenSize
* @brief         Get screen size.
*
*                Description:<br>
*                
* @param         [out]int_t *pnWidth        : pointer to width value
* @param         [out]int_t *pnHeight       : pointer to height value
* @retval        None.
******************************************************************************/
void TP_GetScreenSize( int_t *pnWidth, int_t *pnHeight )
{
    *pnWidth  = ScreenWidth;
    *pnHeight = ScreenHeight;
}

#if (LCD_PANEL == LCD_PANEL_RSK)
/**************************************************************************//**
* Function Name: tp_get_app_board_version
* @brief         Get the version number of the connected TFT App. board.
*
*                Description:<br>
*
* @retval        The version number of the board as an integer.
******************************************************************************/
int_t tp_get_app_board_version()
{
    return sRskRftAppBoardVersion;
}
#endif

/**************************************************************************//**
* Function Name: TP_SendEvtMsg
* @brief         Send event message to synchronism.
*
*                Description:<br>
*                
* @param         [in]uint32_t unEvtFlg      : event flag
* @retval        = 0 : Operation successfull.
*                < 0 : Error occured.
******************************************************************************/
int32_t TP_SendEvtMsg( const uint32_t unEvtFlg )
{
    int32_t     nRet;

    nRet = 0;

    unTpEvtMsg |= unEvtFlg;
    R_OS_SemaphoreRelease( &sTpSemIdTsk );

    DBG_printf_DBG( "[SEND EVT] 0x%08X\n", unTpEvtMsg );

    return nRet;
}


/**************************************************************************//**
* Function Name: TP_WaitEvtMsg
* @brief         Wait event message to synchronism.
*
*                Description:<br>
*                
* @param         None.
* @retval        >= 0 : event flag list.
*                <  0 : Error occured.
******************************************************************************/
int32_t TP_WaitEvtMsg( void )
{
    int32_t nRet;
    bool_t nNum;

    nNum = R_OS_SemaphoreWait( &sTpSemIdTsk, R_OS_ABSTRACTION_EV_WAIT_INFINITE ); 
    if( nNum == true )
    {
        DBG_printf_DBG( "[WAIT EVT] num=%d\n", nNum );
        nRet = (int32_t)unTpEvtMsg;
    }
    else
    {
        DBG_printf_ERR( "[ERROR] message is not got -- ret = %d\n", nNum );
        nRet = -1;
    }

    return nRet;
}


/**************************************************************************//**
* Function Name: TP_ClearEvtMsg
* @brief         Clear assigned event flag.
*
*                Description:<br>
*                
* @param         [in]uint32_t unEvtFlg      : event flag
* @retval        None.
******************************************************************************/
void TP_ClearEvtMsg( const uint32_t unEvtFlg )
{
    unTpEvtMsg &= ~(unEvtFlg | TP_DEVADDR_MASK);
}


/**************************************************************************//**
* Function Name: TP_Callback_LcdEvent
* @brief         Callback function.
*
*                Description:<br>
*                
* @param         [in]uint32_t           : drvnum
* @retval        None.
******************************************************************************/
SCOPE_STATIC void TP_Callback_LcdEvent( uint32_t drvnum )
{
    drvnum = ((drvnum << TP_DEVADDR_SHIFT) & TP_DEVADDR_MASK);
    TP_SendEvtMsg( TP_EVTFLG_PENIRQ | drvnum );
}


