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
* Copyright (C) 2012 - 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File: RGA_raw_image_typedef.h
*    Raw Format Image
*
* - $Version: $ (=RGA_VERSION)
* - $ModuleRevision: {Masters}\RGA\RawImage\29_en $
* - $Rev: $
* - $Date: 2018-11-16T16:14:42+09:00 $
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stdint.h>

#ifndef  RGA_RAW_IMAGE_TYPEDEF_H
#define  RGA_RAW_IMAGE_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Structure: graphics_image_t
*    Raw image format of RGA and ImagePackager.
*
* Description:
*    The ImagePackager tool converts this type from an image file and
*    outputs this type data.
*
*    It is also possible to dynamically generate a graphics_image_t-class
*    object without using the ImagePackager tool.
*
*    See <Section_RGA_ImageFormat>.
************************************************************************/

#ifdef _MSC_VER
 #pragma pack(push,4)
#endif
#ifdef _SH
 #pragma pack 4
#endif

typedef struct st_graphics_image_t {

	/* Variable: flags */
	uint32_t    flags;  /* See graphics_image_t::flags */

	/* Variable: offset_to_image */
	uint32_t    offset_to_image;

	/* Variable: offset_to_alpha */
	uint32_t    offset_to_alpha;

	/* Variable: width */
	uint16_t    width;

	/* Variable: height */
	uint16_t    height;

	/* Variable: type
	*    T_IMAGE_INF type */
	uint8_t     type;

	/* Variable: type2 */
	uint8_t     type2;

	/* Variable: color */
	uint32_t    color;

} graphics_image_t; // @suppress("Data type naming")

#ifdef _MSC_VER
 #pragma pack(pop)
#endif
#ifdef _SH
 #pragma unpack
#endif


/***********************************************************************
* Constants:
*    Bits of <graphics_image_t>::flags
*
*    : F_T_IMAGE_INF_RAW_MASK    - All bits of F_T_IMAGE_INF_RAW_*.
*    : F_T_IMAGE_INF_RAW_ADDRESS - graphics_image_t::image and alpha are pointers.
*    : F_T_IMAGE_INF_RAW_OFFSET  - graphics_image_t::image and alpha are offsets.
*
*    : F_T_IMAGE_INF_PREMULTIPLIED_ALPHA - Premultiplied alpha
*    : F_T_IMAGE_INF_PADDING_AT_RIGHT - Color member variable is line pitch
*    : F_T_IMAGE_INF_USED_MASK - Reserved bits
************************************************************************/

#define  F_T_IMAGE_INF_RAW_MASK             ( 0x01u )
#define  F_T_IMAGE_INF_RAW_ADDRESS          ( 0x00u )
#define  F_T_IMAGE_INF_RAW_OFFSET           ( 0x01u )

#define  F_T_IMAGE_INF_PREMULTIPLIED_ALPHA  ( 0x04u )

#define  F_T_IMAGE_INF_PADDING_AT_RIGHT     ( 0x08u )   /* F_T_IMAGE_INF_LINE_OFFSET */

#define  F_T_IMAGE_INF_USED_MASK      ( 0x0000000Fu )


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* In "RGA_raw_image.h" */


#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif  /* RGA_RAW_IMAGE_TYPEDEF_H */
