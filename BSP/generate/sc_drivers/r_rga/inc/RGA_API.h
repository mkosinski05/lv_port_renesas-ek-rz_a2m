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
* File Name   : rga_api.h
* Description : RGA graphics middleware and driver. Main class is <graphics_t>.
* Version     : 1.12
************************************************************************/

/* Title: Reference Documents */
/* Group: References */
/***********************************************************************
* Document: AN4455EJ0100
*    Application Note. Graphics Library "RGA"
*
* Link:
*    - https://www.renesas.com/us/en/doc/products/mpumcu/apn/rz/r01an4455ej0100-rza2m.pdf
*    - https://www.renesas.com/jp/ja/doc/products/mpumcu/apn/rz/r01an4455jj0100-rza2m.pdf
************************************************************************/
/* Reference: Section_RGA_Initialize
*    Search  "Section_RGA_Initialize" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_RGA_Finalize
*    Search  "Section_RGA_Finalize" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_RGA_ImageFormat
*    Search  "Section_RGA_ImageFormat" in the application note <AN4455EJ0100>.
*/
/* Reference: Figure_RGA_PixelFormat
*    Search  "Figure_RGA_PixelFormat" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_byte_per_pixel_t
*    Search  "Section_byte_per_pixel_t" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_RGA_Shear
*    Search  "Section_RGA_Shear" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_RGA_ImagePackager
*    Search  "Section_RGA_ImagePackager" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_RGA_IdentifyingImageFormat
*    Search  "Section_RGA_IdentifyingImageFormat" in the application note <AN4455EJ0100>.
*/
/* Reference: Figure_RGA_DrawImageChild
*    Search  "Figure_RGA_DrawImageChild" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_RGA_AnimationFunction
*    Search  "Section_RGA_AnimationFunction" in the application note <AN4455EJ0100>.
*/
/* Reference: Section_DefaultableFlags
*    Search  "Section_DefaultableFlags" in the application note <AN4455EJ0100>.
*/
/* Reference: Figure_SequenceOfWaitForInterrupEvent
*    Search  "Figure_SequenceOfWaitForInterrupEvent" in the application note <AN4455EJ0100>.
*/
/* Reference: Figure_RGA_MaxFrameSize
*    Search  "Figure_RGA_MaxFrameSize" in the application note <AN4455EJ0100>.
*/
/* Reference: Table_RGA_SupportedFrameBufferFormat
*    Search  "Table_RGA_SupportedFrameBufferFormat" in the application note <AN4455EJ0100>.
*/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <math.h>
#include  "clib_drivers_typedef.h"
#include  "frame_buffer_typedef.h"
#include  "rga_api_typedef_1.h"
#include  "rga_api_typedef.h"
#include  "rga_raw_image_typedef.h"
#include  "rz_co_typedef.h"

#ifndef RGA_API_H
#define RGA_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/
/* In "rga_api_typedef.h" */

/******************************************************************************
Typedef definitions
******************************************************************************/
/* In "rga_api_typedef.h" */

/******************************************************************************
Variable Externs
******************************************************************************/
/* In "rga_api_typedef.h" */

/******************************************************************************
Functions Prototypes
******************************************************************************/

/* Section: graphics_t_methods */
/* Group: Initializing */
/***********************************************************************
* Function Name:R_GRAPHICS_InitConst
*    Initializes the constant part used by the RGA
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    None
*
* Description:
*    Insted of calling this function, You can use value meaning empty.
*    See <Section_ValueMeaningEmptyAndFlaggedParameter> and
*    <Section_InitConstFunction>.
************************************************************************/
void  R_GRAPHICS_InitConst  
        ( graphics_t*  self );


/***********************************************************************
* Function Name:R_GRAPHICS_Initialize
*    Initializes the graphics drawing context object
*
* Arguments:
*    self          - Graphics drawing context
*    in_out_Config - Configuration
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Initializes internal variables.
*    Initializes 2D Drawing Engine (DRW) and JPEG Codec Unit (JCU).
*
*    When "self" is no longer be used, call <R_GRAPHICS_Finalize>.
*
*    There is only one context. When two or more frame buffers was drawn,
*    change the frame buffer by <R_GRAPHICS_SetFrameBuffer> function.
*
*    See <Section_RGA_Initialize>.
************************************************************************/
errnum_t  R_GRAPHICS_Initialize 
            ( graphics_t*  self,  graphics_config_t*  in_out_Config );


/***********************************************************************
* Function Name:R_GRAPHICS_Finalize
*    Finalizes the graphics drawing context object
*
* Arguments:
*    self - Graphics drawing context
*    e    - Errors that have occurred. No error = 0
*
* Return Value:
*    Error code or e, 0 = successful and input e=0
*
* Description:
*    Internal object is deleted, even if error was raised.
*
*    See <Section_RGA_Finalize>
************************************************************************/
errnum_t  R_GRAPHICS_Finalize( graphics_t*  self,  errnum_t  e ); 


/***********************************************************************
* Function Name:R_GRAPHICS_STATIC_GetVersion
*    Gets version number of RGA
*
* Arguments:
*    out_Version - Output: version number of RGA
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    If "*out_Version == 310", version number is 3.10.
************************************************************************/
errnum_t  R_GRAPHICS_STATIC_GetVersion( uint32_t*  out_Version );


/***********************************************************************
* Function Name:R_GRAPHICS_PrintRegisters
*    Prints registers
*
* Arguments:
*    None
*
* Return Value:
*    None
************************************************************************/
void  R_GRAPHICS_PrintRegisters(void); 


/* Group: Frame */
/***********************************************************************
* Function Name:R_GRAPHICS_SetFrameBuffer
*    Changes the drawing target
*
* Arguments:
*    self           - Graphics drawing context
*    in_FrameBuffer - Frame buffer to be drawn
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This changes the drawing target to VRAM area at
*
*    > in_FrameBuffer->buffer_address [ in_FrameBuffer->draw_bufferIndex ]
*
*    Also, this function does the same operation as <R_GRAPHICS_Finish> function.
************************************************************************/
errnum_t  R_GRAPHICS_SetFrameBuffer 
            ( graphics_t*  self,  frame_buffer_t*  in_FrameBuffer );


/***********************************************************************
* Function Name:R_GRAPHICS_GetFrameBuffer
*    Gets the drawing target
*
* Arguments:
*    self            - Graphics drawing context
*    out_FrameBuffer - (Output) Pointer of frame buffer to be drawn
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_GetFrameBuffer 
            ( graphics_t*  self,  frame_buffer_t**  out_FrameBuffer );


/***********************************************************************
* Function Name:R_GRAPHICS_Start
*    Starts buffered graphics hardware rendering
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function can be called, if frame pipeline mode has been off.
*    The operation by CPU and graphics hardware run parallelly after calling this function.
************************************************************************/
errnum_t  R_GRAPHICS_Start( graphics_t*  self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_Finish
*    Waits until drawing is completed
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function can be called, if <graphics_config_t.has_frame_pipeline> = false.
*
*    This waits for finish drawing by RGA or by the application directly to the
*    frame buffer and changes to the state to able to show the frame buffer.
*    When the CPU directly reads or writes frame buffer data in the cache area
*    or non-cache area after the RGA API is called, enclose by calling
*    <R_GRAPHICS_BeginSoftwareRendering> to <R_GRAPHICS_EndSoftwareRendering>.
*    These functions wait hardware rendering and flush the CPU cache only if necessary.
************************************************************************/
errnum_t  R_GRAPHICS_Finish( graphics_t*  self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_FinishPreviousFrame
*    GPU finishes drawing to "frame_buffer_t::draw_buffer_index".
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function can be called, if <graphics_config_t.has_frame_pipeline> = true.
*    This function must be called before swapping frame buffers.
*
*    Call <R_GRAPHICS_StartPreviousFrame> to start drawing next frame
*    after showing previous frame completed to drawing by this function to the display.
************************************************************************/
errnum_t  R_GRAPHICS_FinishPreviousFrame( graphics_t*  self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_StartPreviousFrame
*    Starts hardware rendering about previous frame
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function can be called, if <graphics_config_t.has_frame_pipeline> = true.
*    This draws the frame buffer of <frame_buffer_t.draw_buffer_index>.
*    This function must be called after setting to draw next frame by
*    <R_GRAPHICS_SetFrameBuffer> function and swapping frame buffers.
************************************************************************/
errnum_t  R_GRAPHICS_StartPreviousFrame( graphics_t*  self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_GetHasFramePipeline
*    Returns on/off mode of frame pipeline mode
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    on/off mode of frame pipeline mode
*
* Description:
*    To set on/off mode of frame pipeline mode, set to
*    "graphics_config_t::has_frame_pipeline".
************************************************************************/
bool_t  R_GRAPHICS_GetHasFramePipeline( graphics_t*  self ); 


/* Group: Save_Restore */
/***********************************************************************
* Function Name:R_GRAPHICS_Save
*    Saves the set value of context to the specified status structure
*
* Arguments:
*    self       - Graphics drawing context
*    out_Status - Output: Set value of context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Example:
*    >     graphics_status_t  status = {0};
*    >     e= R_GRAPHICS_Save( &graphics, &status ); if(e){goto fin;}
*    >         :
*    > fin:
*    >     e= R_GRAPHICS_Restore( &graphics, &status, e );
*    >     R_ERROR_Set( e );
************************************************************************/
errnum_t  R_GRAPHICS_Save( graphics_t* self,  graphics_status_t*  out_Status ); 


/***********************************************************************
* Function Name:R_GRAPHICS_Restore
*    Returns the status structure content to context
*
* Arguments:
*    self      - Graphics drawing context
*    in_Status - Saved context from <R_GRAPHICS_Save>
*    e         - Errors that have occurred. No error = 0
*
* Return Value:
*    Error code or e. 0=No error and "e=0". <errnum_t> type.
************************************************************************/
errnum_t  R_GRAPHICS_Restore 
            ( graphics_t* self,  graphics_status_t*  in_Status,  errnum_t  e );


/* Group: Matrix */
/***********************************************************************
* Function Name:R_GRAPHICS_ResetMatrix
*    Resets the current matrix to be a matrix calculation function target to the unit matrix
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_ResetMatrix( graphics_t*  self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_SetMatrix_2x3
*    Sets each element of the current matrix. (2x3)
*
* Arguments:
*    self  - Graphics drawing context
*    in_Sx - Element of matrix
*    in_Ky - Element of matrix
*    in_Kx - Element of matrix
*    in_Sy - Element of matrix
*    in_Tx - Element of matrix
*    in_Ty - Element of matrix
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > Sx  Kx  Tx
*    > Ky  Sy  Ty
*    > 0   0   1
************************************************************************/
errnum_t  R_GRAPHICS_SetMatrix_2x3( graphics_t*  self,
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty );


/***********************************************************************
* Function Name:R_GRAPHICS_SetMatrix_3x3
*    Sets each element of the current matrix (3x3)
*
* Arguments:
*    self      - Graphics drawing context
*    in_Matrix - Elements of matrix
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > Matrix[0]  Matrix[3]  Matrix[6]
*    > Matrix[1]  Matrix[4]  Matrix[7]
*    > Matrix[2]  Matrix[5]  Matrix[8]
*
*    When the library was ported, take care computing error.
************************************************************************/
errnum_t  R_GRAPHICS_SetMatrix_3x3( graphics_t*  self,  const graphics_matrix_float_t*  in_Matrix );


/***********************************************************************
* Function Name:R_GRAPHICS_GetMatrix_3x3
*    Acquires each element of the current matrix
*
* Arguments:
*    self       - Graphics drawing context
*    out_Matrix - Output: Elements of matrix
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > Matrix[0]  Matrix[3]  Matrix[6]
*    > Matrix[1]  Matrix[4]  Matrix[7]
*    > Matrix[2]  Matrix[5]  Matrix[8]
************************************************************************/
errnum_t  R_GRAPHICS_GetMatrix_3x3( graphics_t*  self,  graphics_matrix_float_t*  out_Matrix );


/***********************************************************************
* Function Name:R_GRAPHICS_TranslateMatrixI
*    Translates the current matrix. (Integer type specified)
*
* Arguments:
*    self  - Graphics drawing context
*    in_Tx - Displacement. When the origin is at the upper left, plus of X means right direction
*    in_Ty - Displacement. When the origin is at the upper left, plus of Y means downward direction
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > 1  0  Tx
*    > 0  1  Ty
*    > 0  0  1
************************************************************************/
errnum_t  R_GRAPHICS_TranslateMatrixI( graphics_t* self, 
    int_fast32_t  in_Tx,  int_fast32_t  in_Ty );


/***********************************************************************
* Function Name:R_GRAPHICS_TranslateMatrix
*    Translates the current matrix. (Floating-point type specified)
*
* Arguments:
*    self  - Graphics drawing context
*    in_Tx - Displacement. When the origin is at the upper left, plus of X means right direction
*    in_Ty - Displacement. When the origin is at the upper left, plus of Y means downward direction
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > 1  0  Tx
*    > 0  1  Ty
*    > 0  0  1
************************************************************************/
errnum_t  R_GRAPHICS_TranslateMatrix( graphics_t* self, 
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty );


/***********************************************************************
* Function Name:R_GRAPHICS_ScaleMatrix
*    Enlarges or reduces the current matrix
*
* Arguments:
*    self  - Graphics drawing context
*    in_Sx - Magnification (Enlargement/reduction. center: Origin)
*    in_Sy - Magnification (Enlargement/reduction. center: Origin)
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > Sx  0   0
*    > 0   Sy  0
*    > 0   0   1
************************************************************************/
errnum_t  R_GRAPHICS_ScaleMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Sy );


/***********************************************************************
* Function Name:R_GRAPHICS_RotateMatrixDegree
*    Rotates the current matrix. The center coordinates of rotation are (0,0)
*
* Arguments:
*    self      - Graphics drawing context
*    in_Degree - Rotation angle (degrees) (When the origin is at the upper left, plus means clockwise direction.)
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > cos( Degree )  -sin( Degree )   0
*    > sin( Degree )   cos( Degree )   0
*    >      0               0          1
************************************************************************/
errnum_t  R_GRAPHICS_RotateMatrixDegree 
            ( graphics_t*  self,  graphics_matrix_float_t  in_Degree );


/***********************************************************************
* Function Name:R_GRAPHICS_ShearMatrix
*     Makes shear deformation of the current matrix
*
* Arguments:
*    self  - Graphics drawing context
*    in_Sx - Rate of shear (Shear center: Origin)
*    in_Sy - Rate of shear (Shear center: Origin)
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    >  1   Shx   0
*    > Shy   1    0
*    >  0    0    1
*
*    See <Section_RGA_Shear>.
************************************************************************/
errnum_t  R_GRAPHICS_ShearMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Shx,  graphics_matrix_float_t  in_Shy );


/***********************************************************************
* Function Name:R_GRAPHICS_TransformMatrix
*    Multiplies the current matrix by the specified 2x3 matrix
*
* Arguments:
*    self  - Graphics drawing context
*    in_Sx - Element of matrix to be multiplied
*    in_Ky - Element of matrix to be multiplied
*    in_Kx - Element of matrix to be multiplied
*    in_Sy - Element of matrix to be multiplied
*    in_Tx - Element of matrix to be multiplied
*    in_Ty - Element of matrix to be multiplied
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > Sx  Kx  Tx
*    > Ky  Sy  Ty
*    > 0   0   1
*
*    When the library was ported, take care computing error.
************************************************************************/
errnum_t  R_GRAPHICS_TransformMatrix( graphics_t*  self, 
    graphics_matrix_float_t  in_Sx,  graphics_matrix_float_t  in_Ky,
    graphics_matrix_float_t  in_Kx,  graphics_matrix_float_t  in_Sy,
    graphics_matrix_float_t  in_Tx,  graphics_matrix_float_t  in_Ty );


/***********************************************************************
* Function Name:R_GRAPHICS_MultiplyMatrix
*    Multiplies the current matrix by the specified 3x3 matrix
*
* Arguments:
*    self          - Graphics drawing context
*    in_Matrix_3x3 - Elements of matrix to be multiplied
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    > Matrix_3x3[0]  Matrix_3x3[3]  Matrix_3x3[6]
*    > Matrix_3x3[1]  Matrix_3x3[4]  Matrix_3x3[7]
*    > Matrix_3x3[2]  Matrix_3x3[5]  Matrix_3x3[8]
*
*    When the library was ported, take care computing error.
************************************************************************/
errnum_t  R_GRAPHICS_MultiplyMatrix 
            ( graphics_t* self,  graphics_matrix_float_t*  in_Matrix_3x3 );


/***********************************************************************
* Function Name:R_GRAPHICS_GetProjectiveMatrix
*    Acquires a matrix that deforms a random profile quadrangle to a random profile quadrangle
*
* Arguments:
*    in_Source*      - Four-point coordinates before conversion
*    in_Destination* - Four-point coordinates after conversion
*    out_Matrix      - Output: 3x3 matrix (array with 9 elements)
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    When the library was ported, take care computing error.
************************************************************************/
errnum_t  R_GRAPHICS_GetProjectiveMatrix( 
    graphics_matrix_float_t  in_SourceTopLeftX,           graphics_matrix_float_t  in_SourceTopLeftY,
    graphics_matrix_float_t  in_SourceTopRightX,          graphics_matrix_float_t  in_SourceTopRightY,
    graphics_matrix_float_t  in_SourceBottomLeftX,        graphics_matrix_float_t  in_SourceBottomLeftY,
    graphics_matrix_float_t  in_SourceBottomRightX,       graphics_matrix_float_t  in_SourceBottomRightY,
    graphics_matrix_float_t  in_DestinationTopLeftX,      graphics_matrix_float_t  in_DestinationTopLeftY,
    graphics_matrix_float_t  in_DestinationTopRightX,     graphics_matrix_float_t  in_DestinationTopRightY,
    graphics_matrix_float_t  in_DestinationBottomLeftX,   graphics_matrix_float_t  in_DestinationBottomLeftY,
    graphics_matrix_float_t  in_DestinationBottomRightX,  graphics_matrix_float_t  in_DestinationBottomRightY,
    graphics_matrix_float_t* out_Matrix );


/* Group: Clear */
/***********************************************************************
* Function Name:R_GRAPHICS_Clear
*    Clears the rectangle area
*
* Arguments:
*    self      - Graphics drawing context
*    in_MinX   - Minimum X coordinates of rectangle area
*    in_MinY   - Minimum Y coordinates of rectangle area
*    in_Width  - Width  of rectangle area
*    in_Height - Height of rectangle area
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Use <R_GRAPHICS_SetBackgroundColor> to set the clear color.
*    When a double-buffer is used, the drawing buffer is cleared.
*    Therefore, the cleared content is not displayed only by calling this function.
*    Use <R_WINDOW_SURFACES_SwapBuffers> to apply display.
*    This function is affected by clipping and matrix.
************************************************************************/
errnum_t  R_GRAPHICS_Clear( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );


/***********************************************************************
* Function Name:R_GRAPHICS_SetBackgroundColor
*    Sets the background color
*
* Arguments:
*    self     - Graphics drawing context
*    in_Color - Use <R_RGA_Get_R8G8B8A8> to acquire the background color
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    When a frame buffer without alpha is to be drawn,
*    the background color is the same as the clear color.
*    The default background color is white.
*    When a frame buffer with alpha is to be drawn,
*    the background color is specified color and
*    the clear color drawing to the target is transparent black
*    even if any color specified. Set the background color
*    for the back layer for correct look.
************************************************************************/
errnum_t  R_GRAPHICS_SetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t  in_Color );


/***********************************************************************
* Function Name:R_GRAPHICS_GetBackgroundColor
*    Acquires the background color
*
* Arguments:
*    self      - Graphics drawing context
*    out_Color - Output: Background color
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_GetBackgroundColor 
            ( graphics_t*  self,  r8g8b8a8_t*  out_Color );


/***********************************************************************
* Function Name:R_GRAPHICS_GetClearColor
*    Acquires the color used for <R_GRAPHICS_Clear>
*
* Arguments:
*    self      - Graphics drawing context
*    out_Color - Output: Clear color
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Use <R_GRAPHICS_SetBackgroundColor> to set the clear color.
************************************************************************/
errnum_t  R_GRAPHICS_GetClearColor( graphics_t* self,  r8g8b8a8_t*  out_Color ); 


/* Group: Image */
/***********************************************************************
* Function Name:R_GRAPHICS_DrawImage
*    Draws an image at coordinates (min_x, min_y) with the same size
*
* Arguments:
*    self     - Graphics drawing context
*    in_Image - Image
*    in_MinX  - Minimum X coordinates
*    in_MinY  - Minimum Y coordinates
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Image data to be specified for the "in_Image" argument can be created
*    by using ImagePackager. See <Section_RGA_ImagePackager>.
*
*    JPEG data can be directly specified for arguments.
*    See <Section_RGA_IdentifyingImageFormat>.
*
*    - This function is affected by <R_GRAPHICS_SetGlobalAlpha>.
*    - This function is also affected by the current matrix and clipping.
*
*    When the alpha component is included in the image and is not included
*    in the drawing target frame buffer, RGB components to be drawn in
*    the frame buffer are blended to values that have been multiplied
*    by the alpha component.
*
*    When the alpha component is included in the drawing target frame buffer,
*    RGB components are blended to values that have not been multiplied
*    by the alpha component.
*
*    - An example of pixel format including the alpha component:
*          ARGB8888, ARGB4444, ARGB1555
*    - An example of pixel format without alpha component:
*          XRGB8888, RGB565, YUV422
*
*    When drawing characters of raster font, specify the image of
*    A8 (Alpha only) format to "in_Image" argument.
************************************************************************/
errnum_t  R_GRAPHICS_DrawImage
            ( graphics_t*  self,  const graphics_image_t*  in_Image,
                int_fast32_t  in_MinX,  int_fast32_t  in_MinY );


/***********************************************************************
* Function Name:R_GRAPHICS_DrawImageResized
*    Enlarges or reduces an image and draws it in the specified rectangle
*
* Arguments:
*    self      - Graphics drawing context
*    in_Image  - Image
*    in_MinX   - Minimum X coordinates
*    in_MinY   - Minimum Y coordinates
*    in_Width  - Width of the drawing target
*    in_Height - Height of the drawing target
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    See the description on the <R_GRAPHICS_DrawImage> function.
************************************************************************/
errnum_t  R_GRAPHICS_DrawImageResized 
            ( graphics_t*  self,  const graphics_image_t*  in_Image, int_fast32_t  in_MinX,
                int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );


/***********************************************************************
* Function Name:R_GRAPHICS_DrawImageChild
*    Draws a part of an image
*
* Arguments:
*    self     - Graphics drawing context
*    in_Image - Image
*    in_SourceMinX   - Minimum X coordinates in the image
*    in_SourceMinY   - Minimum Y coordinates in the image
*    in_SourceWidth  - Width  in the image
*    in_SourceHeight - Height in the image
*    in_DestinationMinX   - Minimum X coordinates of the drawing target
*    in_DestinationMinY   - Minimum Y coordinates of the drawing target
*    in_DestinationWidth  - Width  of the drawing target
*    in_DestinationHeight - Height of the drawing target
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    When "in_SourceWidth" is not "in_DestinationWidth" or
*    "in_SourceHeight" is not "in_DestinationHeight", images are enlarged or reduced.
*    A part of image can be drawn with matrix of translate and zoom.
*    But it cannot be drawn with another matrix.
*
*    See the description on the <R_GRAPHICS_DrawImage> function.
*    See <Figure_RGA_DrawImageChild>.
************************************************************************/
errnum_t  R_GRAPHICS_DrawImageChild
    ( graphics_t*  self,  const graphics_image_t*  in_Image,
    int_fast32_t  in_SourceMinX,        int_fast32_t  in_SourceMinY,
    int_fast32_t  in_SourceWidth,       int_fast32_t  in_SourceHeight,
    int_fast32_t  in_DestinationMinX,   int_fast32_t  in_DestinationMinY,
    int_fast32_t  in_DestinationWidth,  int_fast32_t  in_DestinationHeight );


/***********************************************************************
* Function Name:R_GRAPHICS_IMAGE_InitCopyFrameBufferR8G8B8A8
*    Initializes the image object to which a part of frame buffer being displayed is copied
*
* Arguments:
*    self - Variable to initialize
*    in_ImageDataArray     - Starting address of an array to be a memory area that stores images
*    in_ImageDataArraySize - Size (bytes) of the memory area indicated by "in_ImageDataArray" argument
*    ref_Context - Context with copy source frame buffer to be drawn
*    in_MinX   - Minimum X coordinates (frame buffer coordinates) of the range to be acquired
*    in_MinY   - Minimum Y coordinates (frame buffer coordinates) of the range to be acquired
*    in_Width  - Width of the range to be acquired
*    in_Height - Height of the range to be acquired
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Initializes internal variables.
*    Acquirable image data is arranged in a uint8_t-type array
*    in the order of Red, Green, Blue, and Alpha from the upper-left pixel
*    to the lower-right pixel.
*
*    R8G8B8A8 format is not same as ARGB8888 format.
*    R8G8B8A8 format is drawn by software rendering.
*
*    To create an image of another pixel format,
*    call <R_GRAPHICS_IMAGE_InitByShareFrameBuffer>.
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitCopyFrameBufferR8G8B8A8(
    graphics_image_t*  self,  void*  in_ImageDataArray,  size_t  in_ImageDataArraySize,
    graphics_t*  ref_Context,  int_fast32_t  in_MinX,  int_fast32_t  in_MinY,
    int_fast32_t  in_Width,  int_fast32_t  in_Height );


/***********************************************************************
* Function Name:R_GRAPHICS_IMAGE_InitByShareFrameBuffer
*    Initializes the frame buffer data as an image
*
* Arguments:
*    self               - Variable to initialize
*    in_FrameBuffer     - Frame buffer that contains the image
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Initializes internal variables.
*    The VRAM area indicated by 
*
*    > in_FrameBuffer->buffer_address[ in_FrameBuffer->show_buffer_index ]
*
*    at the time when this function is called is shared with image (self).
*    When wrong stride alignment error was raised, change width of the
*    buffer which becomes to fit stride alignment and copy the image
*    to the buffer. RGA cannot copy at this condition.
*    Also, draw the source image (a part of the buffer) by calling
*    <R_GRAPHICS_DrawImageChild> function.
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitByShareFrameBuffer( graphics_image_t*  self,  frame_buffer_t*  in_FrameBuffer );


/***********************************************************************
* Function Name:R_GRAPHICS_IMAGE_InitByShareFrameBufferEx
*    Initializes the frame buffer data as an image
*
* Arguments:
*    self               - Image to initialize
*    in_FrameBuffer     - Frame buffer that contains the image
*    in_GraphicsContext - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Regardless of pipeline mode, this function executes correctly.
*    The function is same as <R_GRAPHICS_IMAGE_InitByShareFrameBuffer>.
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitByShareFrameBufferEx( graphics_image_t* self,  frame_buffer_t*  in_FrameBuffer,
    graphics_t*  in_GraphicsContext );


/***********************************************************************
* Function Name:R_GRAPHICS_IMAGE_InitR8G8B8A8
*    Initializes the r8g8b8a8_t image object
*
* Arguments:
*    self - Variable to initialize
*    in_ImageDataArray     - Starting address of an array to be a memory area that stores images
*    in_ImageDataArraySize - Size (bytes) of the memory area indicated by "in_ImageDataArray" argument
*    in_Width  - Width of the range to be acquired
*    in_Height - Height of the range to be acquired
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Initializes internal variables.
*    Acquirable image data is arranged in a uint8_t-type array
*    in the order of Red, Green, Blue, and Alpha from the upper-left pixel
*    to the lower-right pixel.
*
*    R8G8B8A8 format is not same as ARGB8888 format.
*    R8G8B8A8 format is drawn by software rendering.
*
*    To create an image of another pixel format,
*    call <R_GRAPHICS_IMAGE_InitByShareFrameBuffer>.
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitR8G8B8A8(
    graphics_image_t*  self,  void*  in_ImageDataArray,  size_t  in_ImageDataArraySize,
    int_fast32_t  in_Width,  int_fast32_t  in_Height );


/***********************************************************************
* Function Name:R_GRAPHICS_IMAGE_InitSameSizeR8G8B8A8
*    Initializes the image object to the same width and height
*
* Arguments:
*    self                  - Variable to initialize
*    in_ImageDataArray     - Starting address of an array to be a memory area that stores images
*    in_ImageDataArraySize - Size (bytes) of the memory area indicated by "in_ImageDataArray" argument
*    in_SameSizeImage      - Image object that references width and height
*
* Description:
*    Initializes internal variables.
*    Acquirable image data is arranged in a uint8_t-type array
*    in the order of Red, Green, Blue, and Alpha from the upper-left pixel
*    to the lower-right pixel.
*
*    R8G8B8A8 format is not same as ARGB8888 format.
*    R8G8B8A8 format is drawn by software rendering.
*
*    To create an image of another pixel format,
*    call <R_GRAPHICS_IMAGE_InitByShareFrameBuffer>.
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_InitSameSizeR8G8B8A8(
    graphics_image_t*  self,  void*  in_ImageDataArray,  size_t  in_ImageDataArraySize,
    graphics_image_t*  in_SameSizeImage );


/***********************************************************************
* Function Name:R_GRAPHICS_IMAGE_GetProperties
*    Get image properties
*
* Arguments:
*    self           - Image to initialize
*    out_Properties - Output: Image properties
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Flush operation must be done before the array pointed by
*    "out_Properties->address" is read or written.
*    When you want to flush, flush CPU cache directly,
*    call <R_GRAPHICS_Finish> or sandwich reading or writing
*    the image data between <R_GRAPHICS_BeginSoftwareRendering>
*    and <R_GRAPHICS_EndSoftwareRendering>.
*
*    This function supports Raw format only.
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_GetProperties
            ( const graphics_image_t* self,  graphics_image_properties_t*  out_Properties );


/***********************************************************************
* Function Name:R_GRAPHICS_IMAGE_GetImageFormat
************************************************************************/
errnum_t  R_GRAPHICS_IMAGE_GetImageFormat( const graphics_image_t* self,  pixel_format_t* out_Format );


/* Group: Filling */
/***********************************************************************
* Function Name:R_GRAPHICS_SetFillColor
*    Changes the paint object of the current fill to single-color fill and sets the fill color
*
* Arguments:
*    self     - Graphics drawing context
*    in_Color - Fill color. Use <R_RGA_Get_R8G8B8A8> for the fill color setting
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    The initial value is opaque black.
************************************************************************/
errnum_t  R_GRAPHICS_SetFillColor( graphics_t* self,  r8g8b8a8_t  in_Color ); 


/***********************************************************************
* Function Name:R_GRAPHICS_FillRect
*    Fills the rectangle area specified by the argument
*
* Arguments:
*    self      - Graphics drawing context
*    in_MinX   - Minimum X coordinates of rectangle area
*    in_MinY   - Minimum Y coordinates of rectangle area
*    in_Width  - Width  of rectangle area
*    in_Height - Height of rectangle area
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function is affected by the current matrix, the current fill, and clipping.
*    No border is drawn. This function is affected by <R_GRAPHICS_SetGlobalAlpha>.
************************************************************************/
errnum_t  R_GRAPHICS_FillRect( graphics_t*  self, 
    int_fast32_t  in_MinX,  int_fast32_t  in_MinY,  int_fast32_t  in_Width,  int_fast32_t  in_Height );


/***********************************************************************
* Function Name:R_GRAPHICS_SetFillPattern
*    Sets the pattern for the current fill paint object
*
* Arguments:
*    self       - Graphics drawing context
*    in_Pattern - Pattern object initialized by <R_GRAPHICS_PATTERN_Initialize>
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    Use <R_GRAPHICS_FillRect> for drawing.
************************************************************************/
errnum_t  R_GRAPHICS_SetFillPattern 
            ( graphics_t*  self,  graphics_pattern_t*  in_Pattern );


/***********************************************************************
* Function Name:R_GRAPHICS_StrokeRect
*    Draws sides of rectangle
*
* Arguments:
*    self      - Graphics drawing context
*    in_MinX   - Minimum X coordinates of rectangle area
*    in_MinY   - Minimum Y coordinates of rectangle area
*    in_Width  - Width  of rectangle area
*    in_Height - Height of rectangle area
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    - This function is affected by line width, border color, and clipping
*    - Fill is not made
*    - When the current matrix is not the unit matrix, an error occurs
*    - This function is affected by R_GRAPHICS_SetGlobalAlpha
************************************************************************/
errnum_t  R_GRAPHICS_StrokeRect
    ( graphics_t*  self,  int_fast32_t  in_MinX,  int_fast32_t  in_MinY,
        int_fast32_t  in_Width,  int_fast32_t  in_Height );


/***********************************************************************
* Function Name:R_GRAPHICS_SetStrokeColor
*    Sets current color of the border
*
* Arguments:
*    self     - Graphics drawing context
*    in_Color - Current color of the border
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_SetStrokeColor( graphics_t*  self,  r8g8b8a8_t  in_Color ); 


/* Group: Path_and_Clipping */
/***********************************************************************
* Function Name:R_GRAPHICS_BeginPath
*    Resets the default path content to null
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_BeginPath( graphics_t* self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_Rect
*    Adds a rectangle to the default path
*
* Arguments:
*    self      - Graphics drawing context
*    in_MinX   - Minimum X coordinates of rectangle
*    in_MinY   - Minimum Y coordinates of rectangle
*    in_Width  - Width of the drawing target of rectangle
*    in_Height - Height of the drawing target of rectangle
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function is used to set the clipping area.
************************************************************************/
errnum_t  R_GRAPHICS_Rect
            ( graphics_t* self,  int_fast32_t  in_MinX,  int_fast32_t  in_MinY,
                int_fast32_t  in_Width,  int_fast32_t  in_Height );


/***********************************************************************
* Function Name:R_GRAPHICS_Clip
*    Sets the shape of the current default path to a clipping area
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    When the current default path is empty or is not a rectangle, an error occurs.
*    When the current default path is empty, drawing is disabled in any area.
*    If this function is called when the current clipping area is a part of the frame buffer,
*    the area common to the previous clipping area and the default path becomes a new clipping area.
*
*    To restore entire drawing, call <R_GRAPHICS_Restore>.
************************************************************************/
errnum_t  R_GRAPHICS_Clip( graphics_t* self ); 


/* Group: Compositing */
/***********************************************************************
* Function Name:R_GRAPHICS_SetGlobalAlpha
*    Sets an alpha value (opacity) to be multiplied by all drawings
*
* Arguments:
*    self          - Graphics drawing context
*    in_AlphaValue - Alpha value (0 to 255). A smaller value makes drawing light
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    The default value is 255.
*    This function affects the following drawing functions.
*
*    - Figure fill functions such as <R_GRAPHICS_FillRect>
*    - Pattern drawing function <R_GRAPHICS_FillRect>
*    - Border drawing functions such as <R_GRAPHICS_StrokeRect>
*    - Image drawing functions such as <R_GRAPHICS_DrawImage>
*
*    This function does not affect the following drawing function.
*    - <R_GRAPHICS_Clear>.
************************************************************************/
errnum_t  R_GRAPHICS_SetGlobalAlpha 
            ( graphics_t*  self,  uint_fast8_t  in_AlphaValue );


/***********************************************************************
* Function Name:R_GRAPHICS_GetGlobalAlpha
*    Acquires an alpha value (opacity) to be multiplied by all drawings
*
* Arguments:
*    self           - Graphics drawing context
*    out_AlphaValue - Output: Alpha value (0 to 255). A smaller value makes drawing light
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_GetGlobalAlpha 
            ( graphics_t*  self,  uint8_t*  out_AlphaValue );


/***********************************************************************
* Function Name:R_GRAPHICS_SetGlobalCompositeOperation
*    Sets the calculation method for alpha blend
*
* Arguments:
*    self                  - Graphics drawing context
*    in_CompositeOperation - Calculation method
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    The following case other than <GRAPHICS_SOURCE_OVER> can be set.
*
*    - Image drawing such as <R_GRAPHICS_DrawImage>
************************************************************************/
errnum_t  R_GRAPHICS_SetGlobalCompositeOperation( graphics_t* self, 
    graphics_composite_operation_t  in_CompositeOperation );


/***********************************************************************
* Function Name:R_GRAPHICS_GetGlobalCompositeOperation
*    Acquires the calculation method for alpha blend
*
* Arguments:
*    self                   - Graphics drawing context
*    out_CompositeOperation - Output: Calculation method
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_GetGlobalCompositeOperation( graphics_t*  self, 
    graphics_composite_operation_t*  out_CompositeOperation );


/* Group: Drawing Quality */
/***********************************************************************
* Function Name:R_GRAPHICS_SetQualityFlags
*    Sets the drawing quality
*
* Arguments:
*    self         - Graphics drawing context
*    in_Qualities - Drawing qualities
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_SetQualityFlags( graphics_t*  self, 
    graphics_quality_flags_t  in_Qualities );


/***********************************************************************
* Function Name:R_GRAPHICS_GetQualityFlags
*    Acquires the current drawing quality
*
* Arguments:
*    self          - Graphics drawing context
*    out_Qualities - Output: Drawing qualities
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
************************************************************************/
errnum_t  R_GRAPHICS_GetQualityFlags( graphics_t*  self, 
    graphics_quality_flags_t*  out_Qualities );


/* Group: Animation */
/***********************************************************************
* Function Name:R_Get_AnimationTimingFunction 
*    Gets defined animation timing
*
* Arguments:
*    in_TimingName    - The name of animation timing.
*    out_TimingObject - Output: The address of animation timing object
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    The following name can be passed at "in_TimingName" argument:
*       - "ease", "linear", "ease_in", "ease_out", "ease_in_out"
************************************************************************/
errnum_t  R_Get_AnimationTimingFunction( const char_t*  in_FunctionName,
    animation_timing_function_t**  out_Function );


/***********************************************************************
* Function Name:R_ANIMATION_TIMING_FUNCTION_GetValue
*    Calculates an attribute value at the specified elapsed time from the time starting animation
*
* Arguments:
*    self                       - The animation timing object
*    in_ClampTime               - The percentage of time from previous key frame (clamp_time=0.0)
*                                 to next key frame (clamp_time=1.0). (decimal from 0.0 to 1.0)
*    in_ValueOfPreviousKeyFrame - The value of attribute at the previous key frame
*    in_ValueOfNextKeyFrame     - The value of attribute at the next key frame
*
* Return Value:
*    The value of attribute at the time of "in_ClampTime" argument
*
* Description:
*    For detail information, See <Section_RGA_AnimationFunction>.
*
* Example:
*    > in_TimingName = "linear";
*    > in_ValueOfPreviousKeyFrame = 10;
*    > in_ValueOfNextKeyFrame = 20;
*    > in_ClampTime = 0.5;
*    This case returns 15.
************************************************************************/
float_t  R_ANIMATION_TIMING_FUNCTION_GetValue( animation_timing_function_t*  self,
    float_t  in_ClampTime,  float_t  in_ValueOfPreviousKeyFrame,  float_t  in_ValueOfNextKeyFrame );


/* Group: Pixel Format */
/***********************************************************************
* Function Name:R_RGA_Get_R8G8B8A8
*    Returns the R8G8B8A8 color value
*
* Arguments:
*    in_Red   - Red   component 0 to 255
*    in_Green - Green component 0 to 255
*    in_Blue  - Blue  component 0 to 255
*    in_Alpha - Alpha component 0 to 255
*
* Return Value:
*    R8G8B8A8 type color value
************************************************************************/
STATIC_INLINE r8g8b8a8_t  R_RGA_Get_R8G8B8A8( 
    int_fast32_t const  in_Red,
    int_fast32_t const  in_Green,
    int_fast32_t const  in_Blue,
    int_fast32_t const  in_Alpha );


/***********************************************************************
* Function Name:R_RGA_BitPerPixelType_To_BytePerPixelType
*    Converts the number of bits per pixel to the number of bytes per pixel (with decimal part)
*
* Arguments:
*    in_BitPerPixel - Number of bits per pixel
*
* Return Value:
*    Number of bytes per pixel (with decimal part)
************************************************************************/
STATIC_INLINE byte_per_pixel_t
    R_RGA_BitPerPixelType_To_BytePerPixelType( int_fast32_t  in_BitPerPixel ); 


/***********************************************************************
* Function Name:R_RGA_BytePerPixelType_To_BitPerPixelType
*    Converts the number of bytes per pixel (with decimal part) to the number of bits per pixel
*
* Arguments:
*    in_BytePerPixel - Number of bytes per pixel (with decimal part)
*
* Return Value:
*    Number of bits per pixel
************************************************************************/
STATIC_INLINE int_fast32_t
    R_RGA_BytePerPixelType_To_BitPerPixelType( byte_per_pixel_t  in_BytePerPixel ); 


/***********************************************************************
* Function Name:R_RGA_BYTE_PER_PIXEL_IsInteger
*    Returns information on whether the number of bytes per pixel is integer
*
* Arguments:
*    in_BytePerPixel - Number of bytes per pixel (with decimal part)
*
* Return Value:
*    Information on whether the number of bits per pixel is integer
************************************************************************/
STATIC_INLINE bool_t
    R_RGA_BYTE_PER_PIXEL_IsInteger( byte_per_pixel_t  in_BytePerPixel ); 


/* Group: Software Rendering */
/***********************************************************************
* Function Name:R_GRAPHICS_BeginSoftwareRendering
*    Notifies the graphics library of the start of software rendering
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function is used before software rendering to the frame bufffer
*    in On-Chip RAM.
*
*    Depending on a condition, this function waits for finishing drawing
*    like <R_GRAPHICS_Finish> function.
************************************************************************/
errnum_t  R_GRAPHICS_BeginSoftwareRendering( graphics_t* self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_BeginSoftwareRendering2
*    Notifies the graphics library of the start of software rendering
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function is for portablity.
************************************************************************/
errnum_t  R_GRAPHICS_BeginSoftwareRendering2( graphics_t* self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_BeginSoftwareRenderingA
*    Notifies the graphics library of the start of software rendering
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    This function is for portablity.
************************************************************************/
errnum_t
    R_GRAPHICS_BeginSoftwareRenderingA( graphics_t* self,  void*  in_Address ); 


/***********************************************************************
* Function Name:R_GRAPHICS_EndSoftwareRendering
*    Notifies the graphics library of the end of software rendering
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    To call this function from functions that support error processing,
*    also call <R_GRAPHICS_EndRenderingInFin> at the end of the function.
************************************************************************/
errnum_t  R_GRAPHICS_EndSoftwareRendering( graphics_t* self ); 


/***********************************************************************
* Function Name:R_GRAPHICS_EndRenderingInFin
*    Call this function from the end of the function that performs software rendering
*
* Arguments:
*    self - Graphics drawing context
*
* Return Value:
*    Error code. 0=No error. <errnum_t> type
*
* Description:
*    To call this function from functions that support error processing,
*    also call <R_GRAPHICS_EndRenderingInFin> at the end of the function.
************************************************************************/
errnum_t  R_GRAPHICS_EndRenderingInFin( graphics_t*  self,  errnum_t  e ); 


/* Section: graphics_config_t_methods */
/***********************************************************************
* Function Name:R_GRAPHICS_CONFIG_SetEmpty
*    Initializes all member variables to the value meaning empty
*
* Arguments:
*    out - A structure initializing the member variable
*
* Return Value:
*    None
*
* Description:
*    See <Section_ValueMeaningEmptyAndFlaggedParameter> about the value meaning empty.
************************************************************************/
void  R_GRAPHICS_CONFIG_SetEmpty( graphics_config_t*  out );


/* Section: graphics_pattern_t_methods */
/***********************************************************************
* Function Name:R_GRAPHICS_PATTERN_Initialize
************************************************************************/
errnum_t  R_GRAPHICS_PATTERN_Initialize( graphics_pattern_t* self,
    const graphics_image_t* image,  repetition_t repetition,  graphics_t* Context );


/***********************************************************************
* Implement: R_RGA_Get_R8G8B8A8
************************************************************************/
STATIC_INLINE r8g8b8a8_t
    R_RGA_Get_R8G8B8A8(  
    int_fast32_t const  in_Red,
    int_fast32_t const  in_Green,
    int_fast32_t const  in_Blue,
    int_fast32_t const  in_Alpha )
{
    r8g8b8a8_t  color_;  /* _ is for MISRA 5.6 */ 

    /*[CheckedNext]overflow is no problem*/
    color_.u.Red   = (uint8_t) in_Red;   /*[CheckedNext]overflow is no problem*/
    color_.u.Green = (uint8_t) in_Green; /*[CheckedNext]overflow is no problem*/
    color_.u.Blue  = (uint8_t) in_Blue;  /*[CheckedNext]overflow is no problem*/
    color_.u.Alpha = (uint8_t) in_Alpha;

    return  color_;
} /* End of function R_RGA_Get_R8G8B8A8 */


/***********************************************************************
* Implement: R_RGA_BitPerPixelType_To_BytePerPixelType
************************************************************************/
STATIC_INLINE byte_per_pixel_t  R_RGA_BitPerPixelType_To_BytePerPixelType( int_fast32_t  in_BitPerPixel )    
{
    int_fast32_t  bpp_mod_8 = in_BitPerPixel & 0x7;  /* in_BitPerPixel % 8 */ 

    if ( bpp_mod_8 == 0 )
    {
        /*[CheckedNext]Cast to larger type*/
        return  (byte_per_pixel_t)( (uint_fast32_t) in_BitPerPixel / 8 );
    }
    else
    {
        return  in_BitPerPixel << R_BYTE_PER_PIXEL_SHIFT;
    }
}/* End of function R_RGA_BitPerPixelType_To_BytePerPixelType */


/***********************************************************************
* Implement: R_RGA_BytePerPixelType_To_BitPerPixelType
************************************************************************/
STATIC_INLINE int_fast32_t  R_RGA_BytePerPixelType_To_BitPerPixelType( byte_per_pixel_t  in_BytePerPixel )    
{
    /*[CheckedNext]Cast to larger type*/
    return  (int_fast32_t)(                                                       /*[CheckedNext]Cast to larger type*/
        ( ( (bit_flags_fast32_t) in_BytePerPixel & R_BYTE_PER_PIXEL_MASK ) * 8 ) |/*[CheckedNext]Cast to larger type*/
        (bit_flags_fast32_t)( ( (uint32_t) in_BytePerPixel & R_BIT_PER_PIXEL_MASK ) >> R_BIT_PER_PIXEL_SHIFT ) );
}/* End of function R_RGA_BytePerPixelType_To_BitPerPixelType */


/***********************************************************************
* Implement: R_RGA_BYTE_PER_PIXEL_IsInteger
************************************************************************/
STATIC_INLINE bool_t
    R_RGA_BYTE_PER_PIXEL_IsInteger  
        ( byte_per_pixel_t  in_BytePerPixel )
{
    /*[CheckedNext]Cast to larger type*/
    return  ( (bit_flags_fast32_t) in_BytePerPixel & R_BIT_PER_PIXEL_MASK ) == 0;
}


/* Title: Document */
/* Group: Sections */
/***********************************************************************
* Reference: Section_InitConstFunction
*    Function to Initialize Internal Variables with Constants (*_initConst Function)
*
* Desciption:
*    In the C language class that includes the finalizing function
*    (*_finalize function), the *_initConst function must be called first
*    (before calling the *_initialize function).
*    This is to prevent any exception from occurring even if the *_finalize
*    function is called when an error occurs from another object
*    before initialization. Before calling the function that may cause
*    an error first among functions, call the *_initConst function first
*    at a time for all objects that only exist in functions (i.e. created
*    and deleted in functions).
*
*    If first member in the structure can be initialized by 0,
*    it is not neccesary to call *_InitConst function.
*
*    Even if the *_InitConst function is called, many functions (methods)
*    are not made available until the *_Initialize function is called.
*
*    In the case of the C++ language API of this library, the *_initConst
*    function responds to the constructor. The *_initialize function
*    responds to object creation functions (such as
*    R_RGA_New_Canvas2D_ContextClass function).
*    The *_finalize function responds to the destroy member function.
*    For example, when an error occurs before an object creation function
*    is called after the constructor was called by the variable declaration,
*    no exception occurs even when the destroy member function is called.
************************************************************************/

/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* RGA_API_H */
