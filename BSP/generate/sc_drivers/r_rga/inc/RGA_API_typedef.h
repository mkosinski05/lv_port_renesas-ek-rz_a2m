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
* File Name   : rga_api_typedef.h
* Description : RGA graphics middleware and driver. Main class is <graphics_t>.
* Version     : 1.12
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "use_config_rga.h"
#include  "r_typedefs.h"
#include  "rga_config.h"
#include  "frame_buffer_typedef.h"
#include  "rga_raw_image_typedef.h"
#include  "rz_co_typedef.h"
#ifdef  USE_CONFIG_RGA_INSIDE
#include  "dave_driver.h"
#include  "clib_drivers_typedef.h"
#include  "frame_buffer_format_typedef.h"
#endif

#ifndef R_RGA_API_TYPEDEF_H
#define R_RGA_API_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/***********************************************************************
* Class: graphics_t
*    Main graphics drawing context of RGA
*
*    - Methods: <graphics_t_methods>
*    - Data Structure: <st_graphics_t>
************************************************************************/

/***********************************************************************
* Class: graphics_config_t
*    Configuration of <graphics_t> type
*
*    - Methods: <graphics_config_t_methods>
*    - Data Structure: <st_graphics_config_t>
*
* Description:
*    If "flags" member variable is not used, this structure must be initialized by
*    <R_GRAPHICS_CONFIG_SetEmpty> function.
*    To customize default values or assemble essential settings, use the function
*    described in <R_GRAPHICS_OnInitialize> or <R_GRAPHICS_OnInitialized>.
************************************************************************/

/***********************************************************************
* Class: graphics_pattern_t
*    Image-arranged pattern
*
*    - Methods: <graphics_pattern_t_methods>
*    - Data Structure: <st_graphics_pattern_t>
************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/

/* Section: Constants_of_RGA */

/* Constant: RGA_FRAME_BUFFER_ADDRESS_ALIGNMENT
*    Address alignment of drawing target frame buffer (bytes) */
#define  RGA_FRAME_BUFFER_ADDRESS_ALIGNMENT  ( 32 )

/* Constant: RGA_SOURCE_IMAGE_STRIDE_ALIGNMENT
*    Byte count alignment to next line (stride) of source image (bytes) */
#define  RGA_SOURCE_IMAGE_STRIDE_ALIGNMENT  ( 1 )

/* Constant: RGAS_SOURCE_IMAGE_STRIDE_ALIGNMENT
*    Byte count alignment to next line (stride) of source image (bytes) for RGAS */
#define  RGAS_SOURCE_IMAGE_STRIDE_ALIGNMENT  ( 4 )

/* Constant: RGA_DESTINATION_STRIDE_ALIGNMENT
*    Byte count alignment to next line (stride) of drawing target (bytes) */
#define  RGA_DESTINATION_STRIDE_ALIGNMENT  ( 32 )

/* Constant: RGA_JPEG_ADDRESS_ALIGNMENT
*    Address alignment of JPEG data (bytes) */
#define  RGA_JPEG_ADDRESS_ALIGNMENT  ( 8 )

/* Constant: RGA_JPEG_MAX_WIDTH_ALIGNMENT
*    Maximum width of MCU of source JPEG image. (pixel) */
#define  RGA_JPEG_MAX_WIDTH_ALIGNMENT  ( 16 )

/* Constant: RGA_JPEG_MAX_HEIGHT_ALIGNMENT
*    Maximum height of MCU of source JPEG image. (pixel) */
#define  RGA_JPEG_MAX_HEIGHT_ALIGNMENT  ( 16 )

/* Constant: RGA_VDC5_BUFFER_ADDRESS_ALIGNMENT
*    First address alignment of the frame buffer shown by Video Display Controller 5 (VDC5) (bytes) */
#define  RGA_VDC5_BUFFER_ADDRESS_ALIGNMENT  ( 64 )

/* Constant: RGA_WORK_BUFFER_B_ADDRESS_ALIGNMENT
*    First address alignment of work buffer B (bytes) */
#define  RGA_WORK_BUFFER_B_ADDRESS_ALIGNMENT  ( 32 )

/* Constant: RGA_PREMULTIPLIED_ALPHA_FRAME
*    Whhether the target frame buffer is premultiplied alpha format.  0 or 1.  RZ/A1H=0,  RZ/A2M=1 */
#define  RGA_PREMULTIPLIED_ALPHA_FRAME  ( 1 )

/* Constant: RGA_DRW_VERSION
*    Using RGA sub module and the module's version */
#define  RGA_DRW_VERSION   ( RGA_VERSION )


/******************************************************************************
Typedef definitions
******************************************************************************/

/* Section: repetition_t_constants */
/***********************************************************************
* Enumeration: repetition_t
*    A type that specifies the pattern repetition method. See <repetition_t_constants>
************************************************************************/
typedef enum st_repetition_t { 

    /* Constant: GRAPHICS_REPEAT
    *    Repeating */
    GRAPHICS_REPEAT = 1

} repetition_t; 


/* Section: graphics_jpeg_decoder_t_constants */
/***********************************************************************
* Enumeration: graphics_jpeg_decoder_t
*    A kind of JPEG decoder. See <graphics_jpeg_decoder_t_constants>
************************************************************************/
typedef enum {

    /* Constant: GRAPHICS_JPEG_DECODER_NONE
    *    Not used JPEG decoder
    *
    * Description:
    *    JPEG hardware decoder can be used from any other than RGA. */
    GRAPHICS_JPEG_DECODER_NONE = 0,


    /* Constant: GRAPHICS_JPEG_DECODER_HARD
    *    JPEG hardware decoder. (default) */
    GRAPHICS_JPEG_DECODER_HARD = 1

} graphics_jpeg_decoder_t; 


/* Section: graphics_quality_flag_t_constants */
/***********************************************************************
* Type: graphics_quality_flags_t
*    Bit flags of <graphics_quality_flag_t>
************************************************************************/
typedef bit_flags_fast32_t  graphics_quality_flags_t;


/***********************************************************************
* Enumeration: graphics_quality_flag_t
*    Quality flag. See <graphics_quality_flag_t_constants> and <Section_DefaultableFlags>
************************************************************************/
typedef  bit_flags32_t  graphics_quality_flag_t;


/* Constant: GRAPHICS_RENDERING_QUALITY_ANTIALIASED
*    Enable the antialiasing at the borders */
#define  GRAPHICS_RENDERING_QUALITY_ANTIALIASED  ( 0x0001u )

/* Constant: GRAPHICS_RENDERING_QUALITY_NONANTIALIASED
*    Disable the antialiasing at the borders */
#define  GRAPHICS_RENDERING_QUALITY_NONANTIALIASED  ( GRAPHICS_RENDERING_QUALITY_ANTIALIASED << 16 )


/* Constant: GRAPHICS_IMAGE_QUALITY_ANTIALIASED
*    Enable the interpolation filter for image
*
* Description:
*    In RZ/A2M, this flag must be set with GRAPHICS_RENDERING_QUALITY_ANTIALIASED. */
#define  GRAPHICS_IMAGE_QUALITY_ANTIALIASED      ( 0x0002u )

/* Constant: GRAPHICS_IMAGE_QUALITY_NONANTIALIASED
*    Disable the interpolation filter for image
*
* Description:
*    In RZ/A2M, this flag must be set with GRAPHICS_RENDERING_QUALITY_NONANTIALIASED. */
#define  GRAPHICS_IMAGE_QUALITY_NONANTIALIASED      ( GRAPHICS_IMAGE_QUALITY_ANTIALIASED << 16 )


/* Constant: GRAPHICS_IMAGE_DHITHER_ON
*    Enable dither
*
* Description:
*    In RZ/A2M, this flag is not supported. */
#define  GRAPHICS_IMAGE_DHITHER_ON               ( 0x0004u )

/* Constant: GRAPHICS_IMAGE_DHITHER_OFF
*    Disable dither */
#define  GRAPHICS_IMAGE_DHITHER_OFF                 ( GRAPHICS_IMAGE_DHITHER_ON << 16 )


/* Section: graphics_composite_operation_t */
/***********************************************************************
* Enumeration: graphics_composite_operation_t
************************************************************************/
typedef enum st_graphics_composite_operation_t {

    /* Constant: GRAPHICS_SOURCE_OVER
    *
    * Desctiption:
    *    Performs calculation (SRC over DST) of general alpha blend. (default)
    *
    *    Whether the calculation formula is the premultiplied alpha expression
    *    depends on whether alpha is present in the flag included in the header of
    *    an image to be drawn or in the drawing target pixel format.
    *
    *    Drawing target format is the premultiplied alpha in RZ/A2M.
    */
    GRAPHICS_SOURCE_OVER = 1,


    /* Constant: GRAPHICS_DESTINATION_OUT
    *
    * Desctiption:
    *    This type is used for drawing the inverted alpha mask.
    *    Only the alpha component of drawing destination varies according to the following expression.
    *    
    *    > a_dst = a_dst * ( 1 - a_src )
    *
    *    When drawing an image on a frame buffer without alpha component,
    *    GRAPHICS_DESTINATION_OUT cannot be set.
    *    When the final drawing destination is XRGB8888,
    *    draw the inverted alpha mask on the back buffer of ARGB8888.
    *    Set 255 for the <R_GRAPHICS_SetGlobalAlpha> function or the globalAlpha property.
    */
    GRAPHICS_DESTINATION_OUT = 7,


    /* Constant: GRAPHICS_COPY
    *
    * Desctiption:
    *    Performs no alpha blend and copy from source image data. It may be fast at that rate.
    *    For faster, when PNG image with alpha channel was decoded to the back buffer,
    *    if CRAPHICS_COPY was specified, it is not necessary to clear the back buffer on ahead.
    */
    GRAPHICS_COPY = 0

} graphics_composite_operation_t; 


/* Section: graphics_state_t */
/***********************************************************************
* Enumeration: graphics_state_t
************************************************************************/
#ifdef  USE_CONFIG_RGA_INSIDE
typedef enum {

    /* Constant: GRAPHICS_STATE_UNINITIALIZED
    *    State of uninitialized */
    GRAPHICS_STATE_UNINITIALIZED = 0,

    /* Constant: GRAPHICS_STATE_INITIALIZED
    *    State of initialized */
    GRAPHICS_STATE_INITIALIZED,

    /* Constant: GRAPHICS_STATE_DISABLED
    *    State of disabled */
    GRAPHICS_STATE_DISABLED,

} graphics_state_t; 
#endif


/***********************************************************************
* Structure: st_graphics_config_t
*    Data structure of <graphics_config_t>
************************************************************************/
typedef struct {

    /* Variable: flags
    *    For compatibility. See <graphics_config_t_flags> and <Section_ValueMeaningEmptyAndFlaggedParameter>
    */
    bit_flags32_t  flags;


    /* Variable: frame_buffer
    *    The address of <frame_buffer_t> type structure.
    *
    * Description:
    *    Member variables in the structure must be set.
    */
    frame_buffer_t*  frame_buffer; 


    /* Variable: has_frame_pipeline
    *    A on/off setting to increase rendering pipeline by 1 stage
    *
    * Description:
    *    If this setting is "on", the drawing contents by API calling
    *    will be shown after 2 times swapping.
    *    If this was omitted, it is the same as "false".
    */
    bool_t  has_frame_pipeline;


    /* Variable: is_hybrid
    *    A on/off setting of hybrid configuration of hardware rendering and software rendering
    *
    * Description:
    *    If "has_frame_pipeline = true", "is_hybrid" must be "false".
    *    If this was omitted, if "has_frame_pipeline = false", it is the same as "true".
    *    If "has_frame_pipeline = true", it is the same as "false".
    */
    bool_t  is_hybrid;


    /* Variable: jpeg_decoder
    *    TODO: GRAPHICS_JPEG_DECODER_HARD or GRAPHICS_JPEG_DECODER_NONE. flags |= F_GRAPHICS_JPEG_DECODER  */
    graphics_jpeg_decoder_t  jpeg_decoder;


    /* Variable: work_buffer_b_address
    *    flags |= F_GRAPHICS_WORK_BUFFER_B_ADDRESS */
    uint8_t*  work_buffer_b_address; 


    /* Variable: work_buffer_b_size
    *    flags |= F_GRAPHICS_WORK_BUFFER_B_SIZE */
    size_t  work_buffer_b_size;


    /* Variable: work_size_for_libPNG
    *    flags |= F_GRAPHICS_WORK_SIZE_FOR_LIBPNG */
    size_t  work_size_for_libPNG; 

} graphics_config_t; 


/* Section: graphics_config_t_flags
*     Bit values to set <graphics_config_t.flags> */

/* Constant: F_GRAPHICS_FRAME_BUFFER
*    0x0001. Enables <graphics_config_t.frame_buffer> */
#define  F_GRAPHICS_FRAME_BUFFER           ( 0x0001u )

/* Constant: F_GRAPHICS_HAS_FRAME_PIPELINE
*    0x0002. Enables <graphics_config_t.has_frame_pipeline> */
#define  F_GRAPHICS_HAS_FRAME_PIPELINE     ( 0x0002u )

/* Constant: F_GRAPHICS_IS_HYBRID
*    0x0004. Enables <graphics_config_t.is_hybrid> */
#define  F_GRAPHICS_IS_HYBRID              ( 0x0004u )

/* Constant: F_GRAPHICS_JPEG_DECODER
*    0x0008. Enables <graphics_config_t.jpeg_decoder> */
#define  F_GRAPHICS_JPEG_DECODER           ( 0x0008u )

/* Constant: F_GRAPHICS_WORK_BUFFER_B_ADDRESS
*    0x0010. Enables <graphics_config_t.work_buffer_b_address> */
#define  F_GRAPHICS_WORK_BUFFER_B_ADDRESS  ( 0x0010u )

/* Constant: F_GRAPHICS_WORK_BUFFER_B_SIZE
*    0x0020. Enables <graphics_config_t.work_buffer_b_size> */
#define  F_GRAPHICS_WORK_BUFFER_B_SIZE     ( 0x0020u )

/* Constant: F_GRAPHICS_WORK_SIZE_FOR_LIBPNG
*    0x0040. Enables <graphics_config_t.work_size_for_libPNG> */
#define  F_GRAPHICS_WORK_SIZE_FOR_LIBPNG   ( 0x0040u )


/***********************************************************************
* Type: graphics_matrix_float_t
*    A matrix element type
************************************************************************/
typedef  float  graphics_matrix_float_t;


/***********************************************************************
* Structure: graphics_matrix3x3_t
************************************************************************/
typedef union {
    graphics_matrix_float_t   xy[3][3];  /* [x][y] */
    graphics_matrix_float_t   array[9];  /* { sx, shy, w0, shx, sy, w1, tx, ty, w2 } */
} graphics_matrix3x3_t; 


/***********************************************************************
* Structure: graphics_image_properties_t
*    A type of image properties
*
* Description:
*    Member variables are set by <R_GRAPHICS_IMAGE_GetProperties>.
*
*    If the source image is created in the program,
*    call <R_GRAPHICS_IMAGE_InitByShareFrameBuffer> function.
************************************************************************/
typedef struct {

    /* Variable: width
    *    Width of image (pixel) */
    int_fast32_t     width;

    /* Variable: height
    *    Height of image (pixel) */
    int_fast32_t     height;

    /* Variable: data
    *    First address of array of image pixels. If the image format is not R8G8B8A8, this value is NULL */
    uint8_t*         data; 

    /* Variable: pixels
    *    First address of array of image pixels. Even if the image format is not R8G8B8A8, this value is available */
    void*            pixels;

    /* Variable: pixelFormat
    *    Pixel format */
    pixel_format_t   pixelFormat; 

    /* Variable: CLUT
    *    CLUT (Color Look Up Table, Palette)
    *
    * Description:
    *    Array of colors. If the image does not have CLUT, this value is NULL.
    *    Set to <layer_attributes_t>. */
    uint32_t*        CLUT;  

    /* Variable: CLUT_count
    *    Count of array elements of CLUT
    *
    * Description:
    *    Set to <layer_attributes_t>. */
    int_fast32_t     CLUT_count; 

} graphics_image_properties_t; 


/***********************************************************************
* Structure: st_graphics_pattern_t
*    Data structure of <graphics_pattern_t>.
*
* Description:
*    Access to member variables is prohibited.
************************************************************************/
typedef struct {
    graphics_image_t* image; 
    repetition_t      repetition;
} graphics_pattern_t; 


/***********************************************************************
* Structure: graphics_status_t
*    An area type that stores graphics drawing context
*
* Description:
*    First member variable must be initialized by 0.
*
*    > graphics_status_t  status = {0};
************************************************************************/
typedef struct {
#ifdef  USE_CONFIG_RGA_INSIDE
    bool_t             IsEnabled;         

    r8g8b8a8_t         ClearColor;        
    int_fast32_t       IntMatrix_i_2[2];  
    bool_t             IsUseIntMatrix;    

    uint32_t           DRW_FillColor;     
    uint32_t           DRW_StrokeColor;   
    uint8_t            DRW_FillAlpha;     
    uint8_t            DRW_StrokeAlpha;   
    uint32_t           DRW_TextureMode;   
    uint8_t            GlobalAlpha;       
    IntBoxType         ClippingRectangle; 

    graphics_composite_operation_t    GlobalCompositeOperation; 
#else
    uint8_t  __Members[0x3C]; 
#endif
} graphics_status_t; 


/***********************************************************************
* Structure: st_graphics_t
*    Data structure of <graphics_t>
*
* Description:
*    Access to member variables is prohibited.
************************************************************************/
typedef struct {
#ifdef  USE_CONFIG_RGA_INSIDE

    /* Variable: Status
    *    See <graphics_state_t> */
    graphics_state_t  RGAS_State; 

    /* Variable: CurrentFrameBufferUserAddress */
    void*  CurrentFrameBufferUserAddress; 

    /* Variable: CurrentFrameBufferCachedAddress */
    void*  CurrentFrameBufferCachedAddress; 

    /* Variable: FrameBuffer */
    frame_buffer_t*  FrameBuffer;  

    /* Variable: CalleeDefined */
    void*  CalleeDefined; 

    /* For RGAS */
        /* Variable: IsSupportedPixelFromatForRGAS */
        bool_t  IsSupportedPixelFromatForRGAS; 

        /* Variable: Clear32BitColor */
        argb8888_t  Clear32BitColor; 

        /* Variable: Fill32BitColor */
        argb8888_t  Fill32BitColor; 

        /* Variable: Clear16BitColor */
        uint16_t  Clear16BitColor; 

        /* Variable: Fill16BitColor */
        uint16_t  Fill16BitColor; 

    /* Shared RGAS and DRW */
        /* Variable: BackgroundColor */
        r8g8b8a8_t  BackgroundColor; 

        /* Variable: FillPattern */
        graphics_pattern_t*
            FillPattern;  

        /* Variable: GlobalAlpha */
        uint8_t  GlobalAlpha; 

        /* Variable: GlobalCompositeOperation */
        graphics_composite_operation_t  GlobalCompositeOperation; 

    /* DRW (D/AVE 2D) */
        /* Variable: DRW_D2 */
        d2_device*  DRW_D2;  

        /* Variable: HasFramePipeline */
        bool_t  HasFramePipeline; 

        /* Variable: IsPreviousFrameEmpty */
        bool_t  IsPreviousFrameEmpty; 

        /* Variable: DRW_RenderBuffer */
        d2_renderbuffer*
            DRW_RenderBuffer;  

        /* Variable: DRW_ClearColor
        *    RGB */
        d2_color  DRW_ClearColor; 

        /* Variable: DRW_FillColor
        *    RGB */
        d2_color  DRW_FillColor; 

        /* Variable: DRW_FillAlpha */
        d2_alpha  DRW_FillAlpha; 

        /* Variable: DRW_StrokeColor
        *    RGB */
        d2_color  DRW_StrokeColor; 

        /* Variable: DRW_StrokeAlpha */
        d2_alpha  DRW_StrokeAlpha; 

        /* Variable: DRW_TextureMode */
        d2_u32  DRW_TextureMode; 

    /* JCU */
        graphics_jpeg_decoder_t  JPEG_Decoder; 

    /* PNG */
        size_t    WorkSizeForLibPNG; 

    /* Work Buffer B */
        uint8_t*
            WorkBufferB_Address;  
        size_t    WorkBufferB_Size;    

    /* Matrix */
        /* Variable: IsUseIntMatrix
        *    true = use "IntMatrix_i_2".  false = use "Matrix2x3". */
        bool_t  IsUseIntMatrix; 

        /* Variable: IntMatrix_i_2 */
        int_fast32_t  IntMatrix_i_2[2]; 

        /* Variable: Matrix2x3 */
        graphics_matrix_float_t  Matrix2x3[2*3]; 

    /* Synchronize hardware and software rendering [RGA_SHS] */
        bool_t  IsFastManualFlush; 
        union {
            struct {
                int8_t   SoftwareRenderingLevel; 
                bool8_t  IsCachedRendering;      
            } Requesting;                        
            bit_flags_fast32_t  RequestingFlags; 
        } SHS_Union;                             
        int_fast32_t  SHS_RequestedSoftwareRenderingLevel; 
        bool_t        SHS_IsRequestedCachedRendering;      
        bool_t        SHS_IsRequestedHardwareRendering;    
        bool_t        SHS_IsRunningHardwareRendering;      

    /* Path */
        /* Variable: PathDataLength */
        int_fast32_t  PathDataLength; 

        /* Variable: PathDataRectangle */
        IntBoxType  PathDataRectangle; 

        /* Variable: ClippingRectangle */
        IntBoxType  ClippingRectangle; 

#else  /* ! USE_CONFIG_RGA_INSIDE */
    uint8_t  __Members[0xC8];   // TODO: 
#endif

} graphics_t; 


/* Section: animation_timing_function_t */
/***********************************************************************
* Type: animation_timing_function_t
*    An object describing timing of animation (bezier function)
************************************************************************/
/* Defined in "rga_api_typedef_1.h" */


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
/* In "rga_api.h" */

/***********************************************************************
* Function: R_RGA_CalcWorkBufferB_Size
*    Calculate size of hardware work buffer B
*
* Arguments:
*    in_MaxWidthOfJPEG - int
*    in_MaxHeightOfJPEG - int
*    in_MaxBytePerPixelOfFrameBuffer - int
*
* Return Value:
*    size of hardware work buffer B
************************************************************************/
#define  R_RGA_CalcWorkBufferB_Size(   \
        in_MaxWidthOfJPEG,  in_MaxHeightOfJPEG,  in_MaxBytePerPixelOfFrameBuffer ) \
    ( R_CEIL_16U( in_MaxWidthOfJPEG ) * R_CEIL_16U( in_MaxHeightOfJPEG ) * (in_MaxBytePerPixelOfFrameBuffer) )
    R_STATIC_ASSERT_GLOBAL 
        ( RGA_JPEG_MAX_WIDTH_ALIGNMENT  == 16, "" );  /* check of R_CEIL_16U */
    R_STATIC_ASSERT_GLOBAL  
        ( RGA_JPEG_MAX_HEIGHT_ALIGNMENT == 16, "" );  /* check of R_CEIL_16U */


/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* R_RGA_API_TYPEDEF_H */
