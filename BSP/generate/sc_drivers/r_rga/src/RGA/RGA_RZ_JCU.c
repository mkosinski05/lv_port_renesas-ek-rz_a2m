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
* File        : rga_rz_jcu.c
* Description : RGA image decoder for JCU.
* Version     : 1.12
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include  "bitblt_alpha.h"
#include  "clib_drivers.h"
#include  "jcu_swap.h"
#include  "r_memory_map.h"
#include  "r_drw_pl.h"
#include  "r_jcu.h"
#include  "rga.h"
#include  "rga_pl.h"
#include  "rga_inside.h"
#include  "rz_co.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static bool_t  gs_IsInitialized = false; 


/***********************************************************************
* Function Name: R_RGA_InitializeJCU
*    new "GraphicsDecoderClass_initialize"
************************************************************************/
errnum_t  R_RGA_InitializeJCU(void) 
{
    errnum_t  e;

    /* Cast to an appropriate type */
    e= R_JCU_Initialize( NULL );
    if(e)
    {
        goto fin;
    }
    e= R_JCU_SelectCodec( JCU_DECODE );
    if(e)
    {
        goto fin;
    }
    gs_IsInitialized = true;

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_RGA_InitializeJCU */


/***********************************************************************
* Function Name: R_RGA_FinalizeJCU
*    new "GraphicsDecoderClass_finalize"
************************************************************************/
errnum_t  R_RGA_FinalizeJCU(void) 
{
    errnum_t  e;

    if ( gs_IsInitialized )
    {
        gs_IsInitialized = false;

        /* Cast to an appropriate type */
        e= R_JCU_TerminateAsync( (r_co_function_t) R_GRAPHICS_JCU_SetTerminateEvent,  NULL );
        if(e)
        {
            goto fin;
        }
        e= R_GRAPHICS_JCU_WaitForTerminateEvent();
        if(e)
        {
            goto fin;
        }
        e= R_JCU_GetAsyncError();
        if(e)
        {
            goto fin;
        }
    }

    e=0;
fin:
    {
        R_ERROR_Set(e);
    }
    return  e;
}/* End of function R_RGA_FinalizeJCU */


/***********************************************************************
* Function Name: R_RGA_DecodeWithJCU
*    new "GraphicsClass_drawImage_JCU_JPEG"
************************************************************************/
errnum_t  R_RGA_DecodeWithJCU  
    ( graphics_t* self,  const graphics_image_t*  in_Image,
    const decode_image_parameters_t*  in_Parameters,
    graphics_image_t*  out_ImageInBuffer,  graphics_JCU_options_t  in_Options )
{
    errnum_t             e;
    errnum_t             ee;
    frame_buffer_t*      frame; 
    jcu_decode_param_t   decode;
    jcu_buffer_param_t   buffer;
    jcu_image_info_t     image_info;
    int_t                x;
    bool_t               is_used_work_buffer;

#ifdef  __CC_ARM
#pragma push
#pragma diag_suppress 4017
#endif

    int_t       byte_per_pixel; /* C4017W  No problem, because goto */
    int_t       log_2_byte_per_pixel;  /* logarithm 2 */ /* C4017W  No problem, because goto */
    int_t       width_of_MCU;   /* C4017W  No problem, because goto */ 
    int_t       height_of_MCU;  /* C4017W  No problem, because goto */ 
    IntBoxType  box_of_MCUs; 
    int_t       width_alignment_of_RGAH = DUMMY_INITIAL_VALUE; 

#ifdef  __CC_ARM
#pragma pop
#endif

    /* frame = ... */
    e= R_GRAPHICS_GetFrameBuffer( self,  &frame );
    if(e)
    {
        goto fin;
    }


    /* Culling image at out of frame buffer */

    if ( IS_BIT_SET( in_Options,  F_GRAPHICS_JCU_FOR_INT_TRANSLATE ) )
    {
        /* Cast to an appropriate type */
        if ( (in_Parameters->MinX >= (signed) frame->width)  ||

                /* Cast to an appropriate type */
                (in_Parameters->MinY >= (signed) frame->height) )
        {
            e=0;
            goto fin;
        }
    }

#if RGA_LIMITATION_04_AYNCHRONOOUS_JCU
    e= R_GRAPHICS_Finish( self ); if(e){goto fin;}
#endif


    /* is_used_work_buffer = ... */
    is_used_work_buffer = false;
    x = in_Parameters->MinX;
    if ( frame->byte_per_pixel == 4 )
    {
        x = R_Floor_2s( x );
    }
    else     /* byte_per_pixel == 2 */
    {
        x = R_Floor_4s( x );
    }
    is_used_work_buffer |=
        (( x != in_Parameters->MinX ) ||
        ( IS_BIT_NOT_SET( in_Options,  F_GRAPHICS_JCU_ENABLE_JCU_ONLY )) );

    switch ( frame->pixel_format )
    {
        case  PIXEL_FORMAT_ARGB8888:
        case  PIXEL_FORMAT_XRGB8888:
        case  PIXEL_FORMAT_RGB565:
            break;
        default:
            is_used_work_buffer = true;
            break;
    }

    if ( (in_Parameters->MinX < 0)  ||  (in_Parameters->MinY < 0) )
        { is_used_work_buffer = true; }


    /* Call "R_JCU_SetDecodeParam" */
    {
        uintptr_t  physical_address_of_JPEG; 
        uintptr_t  physical_address_of_work_buffer;
        uintptr_t  physical_address_of_frame_buffer;

        buffer.source.swapSetting = JCU_SWAP_JPEG_NONE;
        e= R_MEMORY_ToPhysicalAddress( in_Image,  &physical_address_of_JPEG );
        if(e)
        {
            e=E_RGA_ACCESS_DENIED_1B;
            goto fin;
        } /* Error: */
        buffer.source.address = (uint32_t*) physical_address_of_JPEG;
        if ( is_used_work_buffer )
        {
            #if CHECK_ARG
                /* Cast to an appropriate type */
                ASSERT_R 
                ( self->WorkBufferB_Address != NULL,  e=E_RGA_OTHER_CF; goto fin );

                /* Error: Not set work buffer B. */
            #endif

            R_DRW_FlushCache( 0,  0xFFFFFFFFu );

            e= R_MEMORY_ToPhysicalAddress( self->WorkBufferB_Address,  &physical_address_of_work_buffer );
            if(e)
            {
                e=E_RGA_ACCESS_DENIED_1C;
                goto fin;
            } /* Error: */
            buffer.destination.address = (uint32_t*) physical_address_of_work_buffer;
            buffer.lineOffset = 0;  /* Set again after reading "image_info" */
        }
        else
        {
            e= R_MEMORY_ToPhysicalAddress( frame->buffer_address[ frame->draw_buffer_index ] +
                (frame->byte_per_pixel * x) + (frame->stride * in_Parameters->MinY),
                &physical_address_of_frame_buffer );
            if(e)
            {
                e=E_RGA_ACCESS_DENIED_1D;
                goto fin;
            } /* Error: */

            /* Cast to an appropriate type */
            buffer.destination.address = (uint32_t*) physical_address_of_frame_buffer;

            /* Cast to an appropriate type */
            buffer.lineOffset = (int16_t) frame->width;
        }
        decode.horizontalSubSampling    = JCU_SUB_SAMPLING_1_1;
        decode.verticalSubSampling      = JCU_SUB_SAMPLING_1_1;
        switch ( frame->pixel_format )
        {
            case  PIXEL_FORMAT_ARGB8888:
            case  PIXEL_FORMAT_XRGB8888:
                decode.decodeFormat = JCU_OUTPUT_ARGB8888;
                decode.outputCbCrOffset = JCU_CBCR_OFFSET_0;
                buffer.destination.swapSetting  = JCU_SWAP_ARGB8888_PIXEL01;
                byte_per_pixel = 4;
                log_2_byte_per_pixel = 2;
                break;
            case  PIXEL_FORMAT_RGB565:
            case  PIXEL_FORMAT_ARGB1555:
            case  PIXEL_FORMAT_ARGB4444:
                decode.decodeFormat = JCU_OUTPUT_RGB565;
                decode.outputCbCrOffset = JCU_CBCR_OFFSET_0;
                buffer.destination.swapSetting  = JCU_SWAP_RGB565_PIXEL0123;
                byte_per_pixel = 2;
                log_2_byte_per_pixel = 1;
                break;
            case  PIXEL_FORMAT_YUV422:
                decode.decodeFormat = JCU_OUTPUT_YCbCr422;
                decode.outputCbCrOffset = JCU_CBCR_OFFSET_128;
                byte_per_pixel = 2;
                log_2_byte_per_pixel = 1;
                buffer.destination.swapSetting  = JCU_SWAP_CB_Y0_CR_Y1_PIXEL0123;
                break;
            default:
                ASSERT_R 
                ( false, e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3D; goto fin );  /* Error: */
        }
        decode.alpha = 0xFF;

        #define  mod_xx  R_Mod_8u  
        R_STATIC_ASSERT
        ( RGA_JPEG_ADDRESS_ALIGNMENT == 8, "" );  /* check of mod_xx */

        /* Cast to an appropriate type */
        ASSERT_R
        ( mod_xx( (uintptr_t) buffer.source.address ) == 0,  e=E_RGA_OTHER_F6; goto fin );

        /* Error: Start address of JPEG data is not multiple of 8. */
        /* Cast to an appropriate type */
        ASSERT_R 
        ( mod_xx( (uintptr_t) buffer.destination.address ) == 0,  e=E_RGA_OTHER_F7; goto fin );

        /* Error: Start address of work buffer B is not multiple of 8. */
        #undef  mod_xx

        e= R_JCU_SetPauseForImageInfo( true );
        if(e)
        {
            goto fin;
        }

        e= R_JCU_SetDecodeParam( &decode, &buffer );
        if(e)
        {
            goto fin;
        }
    }


    /* Set "image_info" from JPEG data */
    {
        R_GRAPHICS_JCU_ClearCodecEvent();

        /* Cast to an appropriate type */
        e= R_JCU_StartAsync( (r_co_function_t) R_GRAPHICS_JCU_SetCodecEvent,  NULL );
        if(e)
        {
            goto fin;
        }
        e= R_GRAPHICS_JCU_WaitForCodecEvent();
        if(e)
        {
            goto fin;
        }
        e= R_JCU_GetAsyncError();
        if(e)
        {
            goto fin;
        }

        e= R_JCU_GetImageInfo( &image_info );
        if(e)
        {
            goto fin;
        }
    }


    /* Culling image at out of frame buffer */
    if ( IS_BIT_SET( in_Options,  F_GRAPHICS_JCU_FOR_INT_TRANSLATE ) )
    {
        /* Cast to an appropriate type */
        if ( ((in_Parameters->MinX + (int) image_info.width )< 0)  ||

                /* Cast to an appropriate type */
                ((in_Parameters->MinY + (int) image_info.height) < 0) )
        {
            e=0;
            goto fin;
        }
    }

    /* Set "width_of_MCU", "height_of_MCU" */
    switch ( image_info.encodedFormat )
    {
        case  JCU_JPEG_YCbCr420:
            width_of_MCU = 16;  height_of_MCU = 16;  break;
        case  JCU_JPEG_YCbCr422:
            width_of_MCU = 16;  height_of_MCU =  8;  break;
        case  JCU_JPEG_YCbCr444:
            width_of_MCU =  8;  height_of_MCU =  8;  break;
        default:
            ASSERT_R 
            ( false,  e=E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3E; goto fin );  /* Error: */
    }
    box_of_MCUs.MinX = in_Parameters->MinX;
    box_of_MCUs.MinY = in_Parameters->MinY;

    /* Cast to an appropriate type */
    box_of_MCUs.Width  = ceil_2n( (int_t) image_info.width,  width_of_MCU );

    /* Cast to an appropriate type */
    box_of_MCUs.Height = ceil_2n( (int_t) image_info.height, height_of_MCU );

    if ( ! is_used_work_buffer )
    {
        bool_t  is_used = false;  /* For set "is_used_work_buffer" = true */

        /* if image width/height is not aligned by MCU size */
        if ( (image_info.width != box_of_MCUs.Width)  ||  (image_info.height != box_of_MCUs.Height) )
            { is_used = true; }

        /* if image was drawn at cross with frame bound */
        if ( ((in_Parameters->MinX + box_of_MCUs.Width) > frame->width)  ||
                ((in_Parameters->MinY + box_of_MCUs.Height) > frame->height) )
            { is_used = true; }

        /* if image will be clipped */
        {
            IntBoxType  clipped_box;
            bool_t      is_same;

            e= IntBoxType_and( &clipped_box, &box_of_MCUs, in_Parameters->ClippingRectangle );
            if(e)
            {
                goto fin;
            }
            e= IntBoxType_isSame( &clipped_box, &box_of_MCUs, &is_same );
            if(e)
            {
                goto fin;
            }
            if ( ! is_same )
                { is_used = true; }
        }


        if ( is_used )
        {
            uintptr_t  physical_address_of_work_buffer;

            is_used_work_buffer = true;

            #if CHECK_ARG

                /* Cast to an appropriate type */
                ASSERT_R 
                ( self->WorkBufferB_Address != NULL,  e=E_RGA_OTHER_D4; goto fin );

                /* Error: Not set work buffer B. */
            #endif
            e= R_MEMORY_ToPhysicalAddress( self->WorkBufferB_Address,  &physical_address_of_work_buffer );
            if(e)
            {
                e=E_RGA_ACCESS_DENIED_1E;
                goto fin;
            }  /* Error: */
            buffer.destination.address = (uint32_t*) physical_address_of_work_buffer;
        }
    }


    /* Set deocde parameters again */
    if ( is_used_work_buffer )
    {

        /* Set alignmented "buffer.lineOffset" */
        /*   width_alignment_of_RGAH, width_of_MCU */

        int_t  width_alignment;

        /* Cast to an appropriate type */
        width_alignment_of_RGAH = shift_right( RGA_SOURCE_IMAGE_STRIDE_ALIGNMENT,
            log_2_byte_per_pixel );
        if ( width_alignment_of_RGAH == 0 )
            { width_alignment_of_RGAH = 1; }

        if ( width_of_MCU > width_alignment_of_RGAH )
            { width_alignment = width_of_MCU; }
        else
            { width_alignment = width_alignment_of_RGAH; }

        /* Cast to an appropriate type */
        buffer.lineOffset = (int16_t) ceil_2n( (int_t) image_info.width, width_alignment );
#if defined( RGAH_VERSION )
        /* Change to ARGB8888 output, if "buffer.lineOffset" != "image_info.width" , and pixel_format != YUV422 */
        if ( buffer.lineOffset != image_info.width  &&
            frame->pixel_format != PIXEL_FORMAT_YUV422 &&
            IS_BIT_NOT_SET( in_Options,  F_GRAPHICS_JCU_FOR_INT_TRANSLATE ) )
        {
            decode.decodeFormat = JCU_OUTPUT_ARGB8888;
            decode.outputCbCrOffset = JCU_CBCR_OFFSET_0;
            buffer.destination.swapSetting  = JCU_SWAP_ARGB8888_PIXEL01;
            byte_per_pixel = 4;
            log_2_byte_per_pixel = 2;
            width_alignment_of_RGAH = shift_right( RGA_SOURCE_IMAGE_STRIDE_ALIGNMENT,
                log_2_byte_per_pixel );
            if ( width_alignment_of_RGAH == 0 )
                { width_alignment_of_RGAH = 1; }
        }
#endif

        /* Set deocde parameters again */
        e= R_JCU_SetDecodeParam( &decode, &buffer );
        if(e)
        {
            goto fin;
        }


        #if CHECK_ARG
        {
            /* Check "work_buffer_b_size" */
            ASSERT_R
            ( (int_fast32_t)( buffer.lineOffset * byte_per_pixel * box_of_MCUs.Height ) <=
                (int_fast32_t) self->WorkBufferB_Size,  e=E_RGA_OTHER_D6; goto fin );

                /* Error: "graphics_config_t::work_buffer_b_size" is too small than JPEG image size. */
                /* Casting to int_fast32_t is checking for minus value. */
        }
        #endif
    }


    /* Start decoding image */
    R_GRAPHICS_JCU_ClearCodecEvent();

    /* Cast to an appropriate type */
    e= R_JCU_ContinueAsync( JCU_IMAGE_INFO,  (r_co_function_t) R_GRAPHICS_JCU_SetCodecEvent,  NULL );
    if(e)
    {
        goto fin;
    }
    e= R_GRAPHICS_JCU_WaitForCodecEvent();
    if(e)
    {
        goto fin;
    }
    e= R_JCU_GetAsyncError();
    if(e)
    {
        goto fin;
    }


    /* Set "out_ImageInBuffer" : for drawing from work buffer at caller function */
    if ( is_used_work_buffer )
    {
        /* Cast to an appropriate type */
        int_t  width_aligned_by_RGAH = ceil_2n( (int_t) image_info.width, 
                                                        width_alignment_of_RGAH );

        out_ImageInBuffer->flags = F_T_IMAGE_INF_BLEND_COPY;

        /* Cast to an appropriate type */
        out_ImageInBuffer->offset_to_image = (uint32_t) self->WorkBufferB_Address;
        out_ImageInBuffer->offset_to_alpha = 0;

        /* Cast to an appropriate type */
        out_ImageInBuffer->width  = (uint16_t) image_info.width;

        /* Cast to an appropriate type */
        out_ImageInBuffer->height = (uint16_t) image_info.height;
        if ( decode.decodeFormat == JCU_OUTPUT_ARGB8888 )
            { out_ImageInBuffer->type = BITBLT_RGB8888; }
        else if ( decode.decodeFormat == JCU_OUTPUT_YCbCr422 )
            { out_ImageInBuffer->type = BITBLT_YUV422; }
        else
            { out_ImageInBuffer->type = BITBLT_RGB565; }
        out_ImageInBuffer->type2  = 0;
#if defined( RGAH_VERSION )
        if ( ( width_aligned_by_RGAH == box_of_MCUs.Width ) && ( image_info.width == box_of_MCUs.Width ) )
            { out_ImageInBuffer->color = 0; }
        else {
            out_ImageInBuffer->color = buffer.lineOffset;
            out_ImageInBuffer->flags |= F_T_IMAGE_INF_PADDING_AT_RIGHT;
        }
#else
        if ( image_info.width == box_of_MCUs.Width )
            { out_ImageInBuffer->color = 0; }
        else
        {
            out_ImageInBuffer->color = buffer.lineOffset;
            out_ImageInBuffer->flags |= F_T_IMAGE_INF_PADDING_AT_RIGHT;
        }
#endif
    }
    else {
        out_ImageInBuffer->width = 0;
    }

    e=0;
fin:
    {
        return e;
    }
}/* End of function R_RGA_DecodeWithJCU */


