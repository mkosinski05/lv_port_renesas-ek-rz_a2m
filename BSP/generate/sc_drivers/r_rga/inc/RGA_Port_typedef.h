/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/***********************************************************************
* File Name   : rga_port_typedef.h
* Description : Graphics Library. Type Define for Porting Layer.
* Version     : 1.12
************************************************************************/

#ifndef  RGA_PORT_TYPEDEF_H
#define  RGA_PORT_TYPEDEF_H
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "clib_drivers.h"
#include  "rga_port_typedef.h"


#ifdef  __cplusplus
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Constants: BI_BITFIELDS
************************************************************************/
enum { BI_BITFIELDS = 3 };  


/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Structure: uint16_not_aligned_t
************************************************************************/
#pragma pack (push,1)
/* uint16_not_aligned_t* variable raises warning, when it was passed to uint16_t* argument. */
typedef struct _uint16_not_aligned_t {
    uint8_t  byte[2]; 
} uint16_not_aligned_t;
#pragma pack (pop)


/***********************************************************************
* Structure: uint32_not_aligned_t
************************************************************************/
#pragma pack (push,1)
/* uint32_not_aligned_t* variable raises warning, when it was passed to uint32_t* argument. */
typedef struct _uint32_not_aligned_t {
    uint8_t  byte[4]; 
} uint32_not_aligned_t;
#pragma pack (pop)


/***********************************************************************
* Structure: BITMAPFILEHEADER
************************************************************************/
#pragma pack (push,1)
typedef struct tagBITMAPFILEHEADER {
    uint16_t  bfType;  
    uint32_t  bfSize;  
    uint16_t  bfReserved1;  
    uint16_t  bfReserved2;  
    uint32_t  bfOffBits;  
} BITMAPFILEHEADER; 
#pragma pack (pop)


/***********************************************************************
* Structure: BITMAPINFOHEADER
************************************************************************/
#pragma pack (push,1)
typedef struct tagBITMAPINFOHEADER{
    uint32_t  biSize;  
    int32_t   biWidth;  
    int32_t   biHeight;  
    uint16_t  biPlanes;  
    uint16_t  biBitCount;  
    uint32_t  biCompression;  
    uint32_t  biSizeImage;  
    int32_t   biXPelsPerMeter;  
    int32_t   biYPelsPerMeter;  
    uint32_t  biClrUsed;  
    uint32_t  biClrImportant;  
} BITMAPINFOHEADER;  
#pragma pack (pop)


/***********************************************************************
* Structure: vram_ex_stack_t
************************************************************************/
typedef struct _vram_ex_stack_t  vram_ex_stack_t;
struct _vram_ex_stack_t { 
    uint8_t*  Start;    
    uint8_t*  Over;    
    uint8_t*  StackPointer;    
};


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* in RGA_Port.h */

#ifdef  __cplusplus
}  /* extern "C" */
#endif

#endif  /* RGA_PORT_TYPEDEF_H */
