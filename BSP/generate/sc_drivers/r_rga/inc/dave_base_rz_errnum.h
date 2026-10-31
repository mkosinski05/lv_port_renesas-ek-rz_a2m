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
* Copyright (C) 2017 - 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/***********************************************************************
* File: dave_base_rz_errnum.h
*    Error Code from dave_errorcodes.h and r_ospl_errnum.h
*
* - $Version: $
* - $ModuleRevision: {Masters}\DRW\D_AVE_2D\RZA2\ErrorCode\04 $
* - $Date: 2018-11-05T18:08:46+09:00 $
************************************************************************/

#ifndef DAVE_BASE_RZ_ERRNUM_H
#define DAVE_BASE_RZ_ERRNUM_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
* Constant: E_DRW_
*    Error code group of the DRW in the library. See "E_DRW".
******************************************************************************/

#define  E_DRW_NOMEMORY_MASK                ( 0x7FFFFF00 )
#define  E_DRW_NOMEMORY                     ( 0x01790100 )

#define  E_DRW_INVALIDDEVICE_MASK           ( 0x7FFFFF00 )
#define  E_DRW_INVALIDDEVICE                ( 0x01790200 )

#define  E_DRW_INVALIDCONTEXT_MASK          ( 0x7FFFFF00 )
#define  E_DRW_INVALIDCONTEXT               ( 0x01790300 )

#define  E_DRW_INVALIDBUFFER_MASK           ( 0x7FFFFF00 )
#define  E_DRW_INVALIDBUFFER                ( 0x01790400 )

#define  E_DRW_HWINUSE_MASK                 ( 0x7FFFFF00 )
#define  E_DRW_HWINUSE                      ( 0x01790500 )

#define  E_DRW_DEVASSIGNED_MASK             ( 0x7FFFFF00 )
#define  E_DRW_DEVASSIGNED                  ( 0x01790600 )

#define  E_DRW_DEFCONTEXT_MASK              ( 0x7FFFFF00 )
#define  E_DRW_DEFCONTEXT                   ( 0x01790700 )

#define  E_DRW_INVALIDINDEX_MASK            ( 0x7FFFFF00 )
#define  E_DRW_INVALIDINDEX                 ( 0x01790800 )

#define  E_DRW_ILLEGALMODE_MASK             ( 0x7FFFFF00 )
#define  E_DRW_ILLEGALMODE                  ( 0x01790900 )

#define  E_DRW_INVALIDWIDTH_MASK            ( 0x7FFFFF00 )
#define  E_DRW_INVALIDWIDTH                 ( 0x01790A00 )

#define  E_DRW_INVALIDHEIGHT_MASK           ( 0x7FFFFF00 )
#define  E_DRW_INVALIDHEIGHT                ( 0x01790B00 )

#define  E_DRW_NOVIDEOMEM_MASK              ( 0x7FFFFF00 )
#define  E_DRW_NOVIDEOMEM                   ( 0x01790C00 )
#define  E_DRW_NOVIDEOMEM_1                 ( 0x01790C01 )
#define  E_DRW_NOVIDEOMEM_2                 ( 0x01790C02 )

#define  E_DRW_VALUETOOSMALL_MASK           ( 0x7FFFFF00 )
#define  E_DRW_VALUETOOSMALL                ( 0x01790D00 )

#define  E_DRW_VALUENEGATIVE_MASK           ( 0x7FFFFF00 )
#define  E_DRW_VALUENEGATIVE                ( 0x01790E00 )

#define  E_DRW_VALUETOOBIG_MASK             ( 0x7FFFFF00 )
#define  E_DRW_VALUETOOBIG                  ( 0x01790F00 )

#define  E_DRW_INVALIDENUM_MASK             ( 0x7FFFFF00 )
#define  E_DRW_INVALIDENUM                  ( 0x01791000 )

#define  E_DRW_NULLPOINTER_MASK             ( 0x7FFFFF00 )
#define  E_DRW_NULLPOINTER                  ( 0x01791100 )

#define  E_DRW_DEVICEBUSY_MASK              ( 0x7FFFFF00 )
#define  E_DRW_DEVICEBUSY                   ( 0x01791200 )

#define  E_DRW_DEFBUFFER_MASK               ( 0x7FFFFF00 )
#define  E_DRW_DEFBUFFER                    ( 0x01791300 )

#define  E_DRW_NO_DISPLAYLIST_MASK          ( 0x7FFFFF00 )
#define  E_DRW_NO_DISPLAYLIST               ( 0x01791400 )

#define  E_DRW_NOT_ENOUGH_DLISTBLOCKS_MASK  ( 0x7FFFFF00 )
#define  E_DRW_NOT_ENOUGH_DLISTBLOCKS       ( 0x01791500 )

/* Sub class of E_OTHER */
#define  E_DRW_OTHER_1  ( 0x01790001 )
#define  E_DRW_OTHER_2  ( 0x01790002 )
#define  E_DRW_OTHER_3  ( 0x01790003 )
#define  E_DRW_OTHER_4  ( 0x01790004 )
#define  E_DRW_OTHER_5  ( 0x01790005 )


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#ifdef  __cplusplus
}  /* extern "C" */
#endif

#endif /* DAVE_BASE_RZ_ERRNUM_H */
