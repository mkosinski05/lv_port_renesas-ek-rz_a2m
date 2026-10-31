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
* File        : rga_rz_png.c
* Description : Sample functions
* Version     : 1.12
******************************************************************************/
/******************************************************************************
* Area List
*   Includes   <System Includes> , "Project Includes"
*   Macro definitions
*   Typedef definitions
*   Imported global variables and functions (from other files)
*   Exported global variables and functions (to be accessed by other files)
*   Private global variables and functions
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stdlib.h>
#include  <stdio.h>
#include  <string.h>
#include  "bitblt_alpha.h"
#include  "clib_drivers.h"
#include  "png.h"
#include  "r_memory_map.h"
#include  "rga.h"
#include  "rga_inside.h"
#include  "rga_local.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
#define  GS_STACK_LEVEL_MAX  (1)


/***********************************************************************
* Type: r_PNG_decompress_t
*    Soft PNG image decompress work variables.
*
* Member Variables:
*    LocalHeap     - Current malloc/free memory area
*    LocalHeapOver - Over of malloc/free memory area
*    StackAddressInLocalHeap - Previous used current malloc/free memory area
*    ReadOffset    - File pointer in PNG
*    ErrorCode     - ErrorCode
************************************************************************/
typedef struct st_r_PNG_decompress_t  r_PNG_decompress_t; 
struct st_r_PNG_decompress_t { 
    uint8_t*    LocalHeap;  
    uint8_t*    LocalHeapOver;  
    void*       StackAddressInLocalHeap[ GS_STACK_LEVEL_MAX ]; 
    png_size_t  ReadOffset; 
    errnum_t    ErrorCode; 
};

/* Not thread safe */
static r_PNG_decompress_t*  gs_Work;


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static png_voidp*  gs_MallocForPNG( png_structp  in_out_LibPNG,  png_alloc_size_t  in_DataSize );
static void  gs_FreeForPNG( png_structp  in_out_LibPNG,  png_voidp  in_FreeingPointer );
static png_error_ptr  gs_ErrorFromPNG( png_structp  in_out_LibPNG,  void*  in_Dummy );
static png_rw_ptr  gs_ReadPNG( png_structp  in_out_LibPNG,  png_bytep  out_Data,  png_size_t  in_DataLength );


/***********************************************************************
* Function Name: R_RGA_DecodeWithPNG
*    R_RGA_DecodeWithPNG
*
* Arguments:
*    self - graphics_t.
*    in_Image - graphics_image_t.
*    in_Parameters - decode_image_parameters_t.
*    out_ImageInBuffer - graphics_image_t.
*    in_Options - graphics_PNG_options_t.
*
* Return Value:
*    Error Code. 0=No Error.
************************************************************************/
errnum_t  R_RGA_DecodeWithPNG 
    ( graphics_t* self,  const graphics_image_t* in_Image,
    const decode_image_parameters_t* in_Parameters,
    graphics_image_t* out_ImageInBuffer,  graphics_PNG_options_t  in_Options )
{
    errnum_t  e;
    r_PNG_decompress_t    work_body;
    r_PNG_decompress_t*   work = &work_body; 
    frame_buffer_t*       frame; 
    bool_t                is_used_work_buffer;
    pixel_format_t        frame_pixel_format;
    pixel_format_t        output_pixel_format;
    uint8_t*              frame_buffer_address; 
    size_t                frame_stride;

    /* Cast to an appropriate type */
    uint8_t*              decode_buffer = NULL; 

    /* Cast to an appropriate type */
    png_structp   libPNG = NULL; 
    png_infop     info_of_reading;

    /* Cast to an appropriate type */
    png_infopp    address_of_info_of_reading = NULL;
    IntBoxType    box_of_PNG; 
    png_uint_32   line_num;
    int_fast32_t  bit_depth_of_PNG; 
    int_fast32_t  color_type_of_PNG; 
    int_fast32_t  interlace_type;
    int_fast32_t  x_in_frame;
    int_fast32_t  y_in_frame;
    uint8_t*      work_pointer; 
    uint8_t*      work_pointer_over; 
    bool_t        is_width_odd;


    gs_Work = work;

    /* frame = ... */
    e= R_GRAPHICS_GetFrameBuffer( self,  &frame );
    if(e)
    {
        goto fin;
    }
    frame_pixel_format = frame->pixel_format;


    /* Culling image at out of frame buffer */
    if ( IS_BIT_SET( in_Options, F_GRAPHICS_PNG_FOR_INT_TRANSLATE ) )
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


    /* Set "is_used_work_buffer" */
    is_used_work_buffer =
        IS_BIT_NOT_SET( in_Options, F_GRAPHICS_PNG_ENABLE_PNG_ONLY );

    if ( (in_Parameters->MinX < 0)  ||  (in_Parameters->MinY < 0) )
    {
        is_used_work_buffer = true;
    }


    /* Initialize "work" */
    /* [ For PNG   | For Decoder ] */
    /* ^LocalHeap  ^work_pointer  ^work_pointer_over */
    /* ^WorkBufferB_Address */
    #if CHECK_ARG

        /* Cast to an appropriate type */
        ASSERT_R 
        ( self->WorkBufferB_Address != NULL,  e=E_RGA_OTHER_39; goto fin );  /* Error: */
    #endif

    e= R_MEMORY_ToCachedAddress( self->WorkBufferB_Address,  &work_pointer );
    IF(e)
    {
        e=E_RGA_ACCESS_DENIED_A;
        goto fin;
    }  /* work_pointer = . */

    /* Error: The memory of work buffer B must be cached memory. */
    work_pointer_over = work_pointer + self->WorkBufferB_Size;

    work->LocalHeap = work_pointer;
    work->LocalHeapOver = work_pointer + self->WorkSizeForLibPNG;
    work_pointer += self->WorkSizeForLibPNG;

    /* Cast to an appropriate type */
    work->StackAddressInLocalHeap[0] = NULL;
    work->ReadOffset = 0;
    work->ErrorCode = 0;

    ASSERT_R( work_pointer <= work_pointer_over,  e=E_RGA_FEW_ARRAY_5; goto fin ); 

    /* Error: "work_buffer_b_size" is smaller than "work_size_for_libPNG". */


    /* Read informations of PNG image */
    libPNG = png_create_read_struct_2( PNG_LIBPNG_VER_STRING,

        /* Cast to an appropriate type */
        NULL,  (png_error_ptr) gs_ErrorFromPNG,  NULL,

        /* Cast to an appropriate type */
        (png_voidp) work->LocalHeap,

        /* Cast to an appropriate type */
        (png_malloc_ptr) gs_MallocForPNG,  (png_free_ptr) gs_FreeForPNG );

    info_of_reading = png_create_info_struct( libPNG );

    /* Cast to an appropriate type */
    ASSERT_R( info_of_reading != NULL,  work->ErrorCode = E_RGA_PNG_1;  goto fin ); 

    /* Error: <Private/> */
    address_of_info_of_reading = &info_of_reading;

    /* Cast to an appropriate type */
    ASSERT_R
    ( ! png_sig_cmp( (png_const_bytep) in_Image, 0, 8 ),  work->ErrorCode = E_RGA_PNG_2;  goto fin );

    /* Error: <Private/> */

#ifdef PNG_SETJMP_SUPPORTED
    IF ( setjmp( png_jmpbuf( libPNG ) ) ) {
        work->ErrorCode = E_RGA_PNG_3;  /* Error: <Private/> */
        goto fin;
    }
#endif
    box_of_PNG.MinX = in_Parameters->MinX;
    box_of_PNG.MinY = in_Parameters->MinY;

    /* Cast to an appropriate type */
    png_set_read_fn( libPNG, (png_voidp) in_Image, (png_rw_ptr) gs_ReadPNG );
    png_read_info( libPNG, info_of_reading );
    {
        png_uint_32  width;
        png_uint_32  height;

        png_get_IHDR( libPNG, info_of_reading, &width, &height, &bit_depth_of_PNG,

                /* Cast to an appropriate type */
                &color_type_of_PNG, &interlace_type, NULL, NULL);

        /* Cast to an appropriate type */
        box_of_PNG.Width  = (int_fast32_t) width;

        /* Cast to an appropriate type */
        box_of_PNG.Height = (int_fast32_t) height;
    }
    if ( color_type_of_PNG == PNG_COLOR_TYPE_PALETTE )
    {
        png_set_palette_to_rgb( libPNG );
    }

    if ( png_get_valid( libPNG, info_of_reading, PNG_INFO_tRNS ) )
    {
        png_set_tRNS_to_alpha( libPNG );
    }

    /* Make alpha channel from tRNS chank in PNG data */
    else if ( IS_BIT_NOT_SET( color_type_of_PNG, PNG_COLOR_MASK_ALPHA ) )
    {
        png_set_add_alpha( libPNG, 0xff, PNG_FILLER_AFTER );
    }
    else
    {
        /* DO NOTHING */
        ;
    }

    if ( IS_BIT_SET( color_type_of_PNG, PNG_COLOR_MASK_COLOR ) )  /* Not gray */
        { png_set_bgr( libPNG ); }  /* If this was called, 19010 usec -> 18958 usec */
    png_set_swap( libPNG );  /* For little endian CPU */
    png_read_update_info( libPNG, info_of_reading );  /* Flush to call "png_set_*" functions */


    if ( IS_BIT_SET( in_Options, F_GRAPHICS_PNG_FORCE_PNG_ONLY ) )
    {
        is_used_work_buffer = false;
    }
    else if ( IS_BIT_SET( color_type_of_PNG, PNG_COLOR_MASK_ALPHA ) )
    {
        is_used_work_buffer = true;
    }
    else
    {
        /* DO NOTHING */
        ;
    }
    is_width_odd = ( R_Mod_2s( box_of_PNG.Width ) != 0 );


    if (
        ((in_Parameters->MinX + box_of_PNG.Width)  > frame->width)  ||
        ((in_Parameters->MinY + box_of_PNG.Height) > frame->height) )
    {
        is_used_work_buffer = true;
    }


    {
        IntBoxType  clipped_box;
        bool_t      is_same;

        e= IntBoxType_and( &clipped_box, &box_of_PNG, in_Parameters->ClippingRectangle );
        IF(e)
        {
            goto fin;
        }
        e= IntBoxType_isSame( &clipped_box, &box_of_PNG, &is_same );
        IF(e)
        {
            goto fin;
        }
        if ( ! is_same )
            { is_used_work_buffer = true; }
    }


    /* Set "frame_buffer_address", "plus_of_destination" */
    if ( is_used_work_buffer )
    {
        size_t  frame_stride_not_aligned = box_of_PNG.Width * frame->byte_per_pixel;


        #if  RGA_SOURCE_IMAGE_STRIDE_ALIGNMENT == 1  /* Check of not R_CEIL_#U */
            frame_stride = frame_stride_not_aligned;
        #elif  RGA_SOURCE_IMAGE_STRIDE_ALIGNMENT == 4
            frame_stride = R_CEIL_4U( frame_stride_not_aligned );
        #elif  RGA_SOURCE_IMAGE_STRIDE_ALIGNMENT == 32
            frame_stride = R_CEIL_32U( frame_stride_not_aligned );
        #else
            #error
        #endif

        frame_buffer_address = work_pointer;
        work_pointer += (frame_stride * box_of_PNG.Height);

        if ( frame->byte_per_pixel == 2 )
        {
            work_pointer += (( box_of_PNG.Width + 2 ) * 2);

            /* Last 1 line must be 4 byte per pixel */
                /* 2 is buffer of ceil operation */
        }
        else
        {
            ASSERT_R 
            ( frame->byte_per_pixel == 4,  e=E_RGA_OTHER_3A; goto fin );  /* Error: */
        }

        x_in_frame = 0;
        y_in_frame = 0;

        if ( frame->byte_per_pixel == 2 )
        {
            if ( IS_BIT_SET( color_type_of_PNG,  PNG_COLOR_MASK_ALPHA )  ||
                IS_BIT_NOT_SET( in_Options,  F_GRAPHICS_PNG_ENABLE_PNG_ONLY ) )
            {
                output_pixel_format = PIXEL_FORMAT_ARGB4444;
            }
            else
            {
                output_pixel_format = frame_pixel_format;
            }
        }
        else
        {
            if ( frame_pixel_format == PIXEL_FORMAT_XRGB8888 )
            {
                output_pixel_format = PIXEL_FORMAT_ARGB8888;
            }
            else
            {
                output_pixel_format = frame_pixel_format;
            }
        }
    }
    else {
        e= R_MEMORY_ToCachedAddress( frame->buffer_address[ frame->draw_buffer_index ],
            &frame_buffer_address );
        IF(e)
        {
            e=E_RGA_ACCESS_DENIED_1F;
            goto fin;
        }
        /* Error: The memory of frame buffer must be cached memory. */

        frame_stride = frame->stride;
        x_in_frame = in_Parameters->MinX;
        y_in_frame = in_Parameters->MinY;

        if ( frame->byte_per_pixel == 2 )
        {
            decode_buffer = work_pointer;
            work_pointer += (box_of_PNG.Width * 4);
        }
        else
        {
            ASSERT_R
            ( frame->byte_per_pixel == 4,  e=E_RGA_OTHER_3B; goto fin );  /* Error: */
        }
        output_pixel_format = frame_pixel_format;
    }

    ASSERT_R( work_pointer <= work_pointer_over,  e=E_RGA_FEW_ARRAY_6; goto fin ); 
        /* Error: "work_buffer_b_size" is too small. */


    e= R_GRAPHICS_BeginCachedRendering( self );
    IF(e)
    {
        goto fin;
    }


    /* Read pixels of PNG image */
    for ( line_num = 0;  line_num < box_of_PNG.Height;  line_num += 1 )
    {
        uint8_t*      p_destination;    /* p = pointer */
        uint8_t*      decode_target; 
        int_fast32_t  x;
        argb8888_t*   color_of_RGB; 


        p_destination = &frame_buffer_address[
            (( y_in_frame + line_num ) * frame_stride) +
            (( x_in_frame            ) * frame->byte_per_pixel) ];

        /* Cast to an appropriate type */
        if ( decode_buffer != NULL )
            { decode_target = decode_buffer; }
        else
            { decode_target = p_destination; }

        /* Cast to an appropriate type */
        png_read_rows( libPNG, (png_bytepp)(& decode_target ), NULL, 1 );
        IF ( work->ErrorCode != 0 )
        {
            goto fin;
        }


        x = 0;

        /* Cast to an appropriate type */
        color_of_RGB = (argb8888_t*) decode_target;

        if ( is_width_odd )
        {
            if ( output_pixel_format == PIXEL_FORMAT_RGB565 )
            {

                /* ARGB8888Type_to_RGB565Type */
                *( (uint16_t*) p_destination ) =
                    ( ( color_of_RGB[0].Value & 0x00F80000 ) >> 8 ) |
                    ( ( color_of_RGB[0].Value & 0x0000FC00 ) >> 5 ) |
                    ( ( color_of_RGB[0].Value & 0x000000F8 ) >> 3 );

                /* Move pointers */
                p_destination += sizeof(uint16_t);
            }
            else if ( output_pixel_format == PIXEL_FORMAT_ARGB4444 )
            {

                /* ARGB8888Type_to_ARGB4444Type */
                *( (uint32_t*) p_destination ) =
                    ( ( color_of_RGB[0].Value & 0xF0000000 ) >> 16 ) |
                    ( ( color_of_RGB[0].Value & 0x00F00000 ) >> 12 ) |
                    ( ( color_of_RGB[0].Value & 0x0000F000 ) >>  8 ) |
                    ( ( color_of_RGB[0].Value & 0x000000F0 ) >>  4 );

                /* Move pointers */
                p_destination += sizeof(uint16_t);
            }
            else if ( output_pixel_format == PIXEL_FORMAT_ARGB1555 )
            {

                /* ARGB8888Type_to_ARGB1555Type */
                *( (uint32_t*) p_destination ) =
                    ( ( color_of_RGB[0].Value & 0x80000000 ) >> 16 ) |
                    ( ( color_of_RGB[0].Value & 0x00F80000 ) >>  9 ) |
                    ( ( color_of_RGB[0].Value & 0x0000F800 ) >>  6 ) |
                    ( ( color_of_RGB[0].Value & 0x000000F8 ) >>  3 );

                /* Move pointers */
                p_destination += sizeof(uint16_t);
            }
            else
            {
                ASSERT_D( frame->byte_per_pixel == 4,  e=E_RGA_OTHER_3C; goto fin );  /* Error: */
            }
            x += 1;
        }
        if ( output_pixel_format == PIXEL_FORMAT_RGB565 )
        {
            for ( /* x */;  x < box_of_PNG.Width;  x += 2 )
            {
                uint32_t  value0 = color_of_RGB[x+0].Value;
                uint32_t  value1 = color_of_RGB[x+1].Value;

                /* ARGB8888Type_to_RGB565Type */
                *( (uint32_t*) p_destination ) =
                    ( ( value1 & 0x00F80000 ) <<  8 ) |
                    ( ( value1 & 0x0000FC00 ) << 11 ) |
                    ( ( value1 & 0x000000F8 ) << 13 ) |
                    ( ( value0 & 0x00F80000 ) >>  8 ) |
                    ( ( value0 & 0x0000FC00 ) >>  5 ) |
                    ( ( value0 & 0x000000F8 ) >>  3 );

                /* Move pointers */
                p_destination += ( sizeof(uint16_t) * 2 );
            }
        } else if ( output_pixel_format == PIXEL_FORMAT_ARGB4444 )
        {
            for ( /* x */;  x < box_of_PNG.Width;  x += 2 )
            {
                uint32_t  value0 = color_of_RGB[x+0].Value;
                uint32_t  value1 = color_of_RGB[x+1].Value;

                /* ARGB8888Type_to_ARGB4444Type */
                *( (uint32_t*) p_destination ) =
                    ( ( value1 & 0xF0000000 ) ) |
                    ( ( value1 & 0x00F00000 ) <<  4 ) |
                    ( ( value1 & 0x0000F000 ) <<  8 ) |
                    ( ( value1 & 0x000000F0 ) << 12 ) |
                    ( ( value0 & 0xF0000000 ) >> 16 ) |
                    ( ( value0 & 0x00F00000 ) >> 12 ) |
                    ( ( value0 & 0x0000F000 ) >>  8 ) |
                    ( ( value0 & 0x000000F0 ) >>  4 );

                /* Move pointers */
                p_destination += ( sizeof(uint16_t) * 2 );
            }
        } else if ( output_pixel_format == PIXEL_FORMAT_ARGB1555 )
        {
            for ( /* x */;  x < box_of_PNG.Width;  x += 2 )
            {
                uint32_t  value0 = color_of_RGB[x+0].Value;
                uint32_t  value1 = color_of_RGB[x+1].Value;

                /* ARGB8888Type_to_ARGB1555Type */
                *( (uint32_t*) p_destination ) =
                    ( ( value1 & 0x80000000 ) ) |
                    ( ( value1 & 0x00F80000 ) <<  7 ) |
                    ( ( value1 & 0x0000F800 ) << 10 ) |
                    ( ( value1 & 0x000000F8 ) << 13 ) |
                    ( ( value0 & 0x80000000 ) >> 16 ) |
                    ( ( value0 & 0x00F80000 ) >>  9 ) |
                    ( ( value0 & 0x0000F800 ) >>  6 ) |
                    ( ( value0 & 0x000000F8 ) >>  3 );

                /* Move pointers */
                p_destination += ( sizeof(uint16_t) * 2 );
            }
        }
        else
        {
            ASSERT_D( frame->byte_per_pixel == 4,  e=E_RGA_OTHER_3D; goto fin );  /* Error: */
        }
    }


    e= R_GRAPHICS_EndCachedRendering( self );
    IF(e)
    {
        goto fin;
    }


    if ( (frame_pixel_format == PIXEL_FORMAT_XRGB8888)  &&
            IS_BIT_NOT_SET( color_type_of_PNG, PNG_COLOR_MASK_ALPHA ) )
    {
        out_ImageInBuffer->flags  = F_T_IMAGE_INF_BLEND_COPY;
    }
    else
    {
        out_ImageInBuffer->flags  = F_T_IMAGE_INF_BLEND_ALPHA;
    }

    /* Cast to an appropriate type */
    out_ImageInBuffer->offset_to_image = (uintptr_t) frame_buffer_address;
    out_ImageInBuffer->offset_to_alpha = 0;

    /* Cast to an appropriate type */
    out_ImageInBuffer->height = (uint16_t) box_of_PNG.Height;
    out_ImageInBuffer->type2 = 0;
    if ( frame_stride == (box_of_PNG.Width * frame->byte_per_pixel) )
    {
        out_ImageInBuffer->color = 0;
    }
    else
    {
        if ( frame->byte_per_pixel == 2 )
        {
            out_ImageInBuffer->color = frame_stride / 2;
        }
        else
        {
            out_ImageInBuffer->color = frame_stride / 4;
        }
        out_ImageInBuffer->flags |= F_T_IMAGE_INF_PADDING_AT_RIGHT;
    }
    if ( is_used_work_buffer )
    {
        out_ImageInBuffer->width = box_of_PNG.Width;
        e= Get_T_IMAGE_INF_PixelFormat_From_PixelFormatEnum( &out_ImageInBuffer->type,
            output_pixel_format );
        IF(e)
        {
            goto fin;
        }
    } else
    {
        out_ImageInBuffer->width = 0;  /* Drawn by PNG decode only */
        out_ImageInBuffer->type = BITBLT_YUV422;  /* Dummy */
    }

    e=0;
fin:

    /* Cast to an appropriate type */
    if ( info_of_reading != NULL )
    {
        png_read_end( libPNG, info_of_reading );
    }

    /* Cast to an appropriate type */
    if ( libPNG != NULL )
    {
        /* Cast to an appropriate type */
        png_destroy_read_struct( &libPNG,  address_of_info_of_reading, (png_infopp) NULL );
    }
    if ( work->ErrorCode != 0 )
    {
        if ( e == 0 )
        {
            e = work->ErrorCode;
        }
    }
    e= R_GRAPHICS_EndRenderingInFin( self, e );

    return  e;
}/* End of function R_RGA_DecodeWithPNG */


/***********************************************************************
* Function Name: gs_MallocForPNG
*    gs_MallocForPNG
*
* Arguments:
*    in_out_LibPNG - png_structp
*    in_DataSize   - png_alloc_size_t
*
* Return Value:
*    Allocated memory address
************************************************************************/
static png_voidp*  gs_MallocForPNG 
                    ( png_structp  in_out_LibPNG,  png_alloc_size_t  in_DataSize )
{
    errnum_t  e;
    uint32_t  aligned_data_size;
    void*     allocated_address;
    r_PNG_decompress_t*   work = gs_Work; 

    aligned_data_size = ( ( in_DataSize + 0x03 ) & 0xFFFFFFFC );

    /* Cast to an appropriate type */
    ASSERT_R 
        ( (uint32_t) work->LocalHeap + aligned_data_size <= (uint32_t) work->LocalHeapOver,
        allocated_address = NULL;  e=E_RGA_PNG_FEW_MEMORY_1;  goto fin );

    /* Error: The work memory for PNG is too small. */

    work->StackAddressInLocalHeap[0] = work->LocalHeap;

    /* Cast to an appropriate type */
    work->LocalHeap = (void *)( (uint32_t)work->LocalHeap + aligned_data_size );

    allocated_address = work->StackAddressInLocalHeap[0];

    e=0;
fin:
    {
        R_ERROR_Set( e );
    }
    return  allocated_address;
}/* End of function gs_MallocForPNG */


/***********************************************************************
* Function Name: gs_FreeForPNG
*    gs_FreeForPNG
*
* Arguments:
*    in_out_LibPNG     - png_structp
*    in_FreeingPointer - png_voidp
*
* Return Value:
*    None
************************************************************************/
static void  gs_FreeForPNG 
                ( png_structp  in_out_LibPNG,  png_voidp  in_FreeingPointer )
{
    r_PNG_decompress_t*  work = gs_Work; 

    /* Cast to an appropriate type */
    if ( ( work->StackAddressInLocalHeap[0] != NULL ) && ( work->LocalHeap == in_FreeingPointer ) )
    {
        work->LocalHeap = work->StackAddressInLocalHeap[0];

        /* Cast to an appropriate type */
        work->StackAddressInLocalHeap[0] = NULL;
    }
}/* End of function gs_FreeForPNG */


/***********************************************************************
* Function Name: gs_ErrorFromPNG
*    gs_ErrorFromPNG
*
* Arguments:
*    in_out_LibPNG - png_structp
*    in_Dummy      - Dummy
*
* Return Value:
*    ?
************************************************************************/
static png_error_ptr  gs_ErrorFromPNG 
                        ( png_structp  in_out_LibPNG,  void*  in_Dummy )
{
    r_PNG_decompress_t*  work = gs_Work; 

    work->ErrorCode = E_RGA_PNG_4;  /* Error: <Private/> */
    return NULL;
}/* End of function gs_ErrorFromPNG */


/***********************************************************************
* Function Name: gs_ReadPNG
*    gs_ReadPNG
*
* Arguments:
*    in_out_LibPNG - png_structp
*    out_Data      - Dummy
*    in_DataLength - 
*
* Return Value:
*    ?
************************************************************************/
static png_rw_ptr  gs_ReadPNG 
                    ( png_structp  in_out_LibPNG,  png_bytep  out_Data,  png_size_t  in_DataLength )
{
    r_PNG_decompress_t*  work = gs_Work; 

    /* Cast to an appropriate type */
    uint8_t*  read_pointer 

        /* Cast to an appropriate type */
        = (uint8_t *)( (uint32_t) png_get_io_ptr( in_out_LibPNG ) + work->ReadOffset );
    uint8_t*  next_read_pointer; 

    memcpy( out_Data, read_pointer, in_DataLength );

    next_read_pointer = read_pointer + in_DataLength;
    work->ReadOffset += in_DataLength;

    /* Cast to an appropriate type */
    return  (png_rw_ptr) next_read_pointer;
}/* End of function gs_ReadPNG */


