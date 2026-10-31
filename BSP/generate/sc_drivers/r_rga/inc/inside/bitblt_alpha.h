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
/******************************************************************************
 * File Name    : bitblt_alpha.h
 * Version      : 1.12
 *****************************************************************************/
#include  "r_typedefs.h"
#include  "rz_co_typedef.h"

#ifndef _HEADER_BITBLT_ALPHA_H 
#define _HEADER_BITBLT_ALPHA_H 

#define BLEND_RATIO_BASE    (0xFF)    /*! 2^8 */
#define SHIFT_SIZE          (8)
#define DIVIDE_SIZE         (255)


#define BITBLT_RGB565        (0)
#define BITBLT_RGB555        (1)
#define BITBLT_ARGB1555      (2)
#define BITBLT_ARGB4444      (3)
#define BITBLT_RGB565I_A8    (4)
#define BITBLT_RGB565C_A8    (5)
#define BITBLT_B32PALET256   (6)
#define BITBLT_B32PALET16    (7)
#define BITBLT_B32PALET2     (8)
#define BITBLT_RGB888        (9)
#define BITBLT_RGB8888       (10)
#define BITBLT_YUV422        (11)
#define BITBLT_B16PALET256   (12)
#define BITBLT_RGBA5551      (13)
#define BITBLT_RGBA4444      (14)
#define BITBLT_RGB666        (15)
#define BITBLT_ARGB6666      (16)
#define BITBLT_RGBA8888      (17)
#define BITBLT_RGB888_A      (18)
#define BITBLT_RGB888_C      (19)
#define BITBLT_A8            (20)
#define BITBLT_A4            (21)
#define BITBLT_A1            (22)
#define BITBLT_UNKNOWN       (99)


typedef struct {
    void *Buf; 
    uint16_t    Width; 
    uint16_t    Height; 
    uint8_t     Type; 
} T_SURFACE_INF; 

typedef struct {
    uint16_t    X; 
    uint16_t    Y; 
    uint16_t    W; 
    uint16_t    H; 
} T_RECT; 

/* Type: T_IMAGE_INF */
typedef struct {
    bit_flags_fast32_t  Flags; 
    const void* Image; 
    const void* Alpha; 
    uint16_t    Width; 
    uint16_t    Height; 
    uint8_t     Type; 
    uint8_t     Type2; 
    uint32_t    Color; 
    uint32_t    ImageSize; 
    uint32_t    AlphaSize; 
    uint32_t    TotalSize; 
    uint32_t    CheckSum; 
} T_IMAGE_INF; 

enum { /* T_IMAGE_INF::Flags */
    F_T_IMAGE_INF_RAW_MASK    = 0x01,
    F_T_IMAGE_INF_RAW_ADDRESS = 0x00,  /* T_IMAGE_INF::Image and Alpha are pointers */
    F_T_IMAGE_INF_RAW_OFFSET  = 0x01,  /* T_IMAGE_INF::Image and Alpha are offsets */

    F_T_IMAGE_INF_BLEND_MASK  = 0x02,
    F_T_IMAGE_INF_BLEND_COPY  = 0x00,  /* Not alpha blending */
    F_T_IMAGE_INF_BLEND_ALPHA = 0x02,  /* Do alpha blending */
};

typedef struct {
    int16_t     X; 
    int16_t     Y; 
} T_POINT; 

/******************************************************************************
 * Function Name: SetClipRect
 *****************************************************************************/
void  SetClipRect(
    int16_t   left,
    int16_t   top,
    int16_t   right,
    int16_t   bottom);


/******************************************************************************
 * Function Name: RGL_BitBlt
 *****************************************************************************/
void  RGL_BitBlt(
    const T_SURFACE_INF *dst,
    const T_RECT *dst_rect,
    const T_IMAGE_INF *src,
    const T_POINT *src_pos);

typedef struct
{
    int width; 
    int height; 
    const uint16_t*  palette; 
    const uint8_t*   image; 
} RGL_IMG8_t; 

typedef struct
{
    int width; 
    int height; 
    const uint16_t*  image; 
} RGL_IMG16_t; 

typedef struct
{
    int x; 
    int y; 
    int blk_x; 
    int blk_y; 
    int blk_w; 
    int blk_h; 
} RGL_BLK_POS_t; 

typedef struct {
    uint16_t    red;
    uint16_t    green;
    uint16_t    blue;
} IMG_RGB; 

typedef struct {
    uint32_t    red;
    uint32_t    green;
    uint32_t    blue;
} IMG_RGB32; 

/******************************************************************************
 * Function Name: rgl_blt_32
 *****************************************************************************/
void  rgl_blt_32(
    const RGL_IMG16_t   *frameBuf,
    const RGL_BLK_POS_t *position,
    const RGL_IMG16_t   *bltData);

/******************************************************************************
 * Function Name: rgl_blt_Palet32
 *****************************************************************************/
void  rgl_blt_Palet32(
    const RGL_IMG16_t   *frameBuf,
    const RGL_BLK_POS_t *position,
    const RGL_IMG16_t   *bltData,
    const uint32_t      *paletTBL);

/******************************************************************************
 * Function Name: rgl_ablt_mask_32
 *****************************************************************************/
void  rgl_ablt_mask_32(
    const RGL_IMG16_t   *frameBuf,
    const RGL_BLK_POS_t *position,
    const RGL_IMG16_t   *bltData,
    const uint8_t       *alphaValue);

/******************************************************************************
 * Function Name: rgl_blt_16
 *****************************************************************************/
void  rgl_blt_16(
    const RGL_IMG16_t   *frameBuf,
    const RGL_BLK_POS_t *position,
    const RGL_IMG16_t   *bltData);

/******************************************************************************
 * Function Name: rgl_ablt_mask_16
 *****************************************************************************/
void  rgl_ablt_mask_16(
    const RGL_IMG16_t   *frameBuf,
    const RGL_BLK_POS_t *position,
    const RGL_IMG16_t   *bltData,
    const uint8_t       *alphaValue);

#endif /* _HEADER_BITBLT_ALPHA_H */
