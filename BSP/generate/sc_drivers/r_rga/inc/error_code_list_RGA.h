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
/******************************************************************************
* File: error_code_list_RGA.h
*    Defailed Error Code List
*
* - $ModuleRevision: {Masters}\RGA\driver\RZA2_Test\19_en $
* - $Rev: $
* - $Date: 2018-11-16T11:33:24+09:00 $
******************************************************************************/

/* Section: DetailErrorCode_RGA */

/* Constant: E_JCU_OBLIVIOUS_1
*    <0x0B450001>.  */

/* Constant: E_JCU_OBLIVIOUS_2
*    <0x0B450002>.  */

/* Constant: E_JCU_OBLIVIOUS_3
*    <0x0B450003>.  */

/* Constant: E_RGA_OBLIVIOUS_1
*    <0x0B730001>. Slow because pixel format is not same as image and frame buffer. */

/* Constant: E_RGA_OBLIVIOUS_2
*    <0x0B730002>. RGA's "*_IMAGE_*_NONANTIALIASED" and "*_RENDERING_*_ANTIALIASED" can not be set at the same time. */

/* Constant: E_RGA_OBLIVIOUS_4
*    <0x0B730004>. This setting is slow, because image width and height is not 2 to the n th power */

/* Constant: E_RGA_OBLIVIOUS_5
*    <0x0B730005>. RGA's "*_IMAGE_*_ANTIALIASED" and "*_RENDERING_*_NONANTIALIASED can not be set at the same time. */

/* Constant: E_RGA_OBLIVIOUS_6
*    <0x0B730006>. "GRAPHICS_IMAGE_DHITHER_ON" was ignored. */

/* Constant: E_RGA_OBLIVIOUS_7
*    <0x0B730007>. RGA's "*_RENDERING_*" was ignored. Use "*_IMAGE_*" */

/* Constant: E_DRW_OBLIVIOUS_1
*    <0x0B790001>. Error in "R_DRW_OnInitialize" was ignored. */

/* Constant: E_DRW_OBLIVIOUS_2
*    <0x0B790002>. Error in "R_DRW_EnableInterrupt" was ignored. */

/* Constant: E_DRW_OBLIVIOUS_3
*    <0x0B790003>. Error in "R_DRW_DisableInterrupt" was ignored. */

/* Constant: E_DRW_OBLIVIOUS_4
*    <0x0B790004>. Not called "R_DRW_RegisterTextureVariables". */

/* Constant: E_DRW_OBLIVIOUS_5
*    <0x0B790005>. If "graphics_config_t::is_hybrid = true", logger output only a part of log. */

/* Constant: E_DRW_OBLIVIOUS_6
*    <0x0B790006>. DRW is not supported multi thread. */

/* Constant: E_DRW_THREAD_DENIED_1
*    <0x12790001>. DRW is not supported multi context. */

/* Constant: E_JCU_UNRECOVERABLE_1
*    <0x0D450001>. Not return ack from standby request */

/* Constant: E_DRW_NOVIDEOMEM_1
*    <0x01790C01>. Changing to physical address was failed in "R_DRW_ToPhysicalAddress" function. */

/* Constant: E_DRW_NOVIDEOMEM_2
*    <0x01790C02>. Changing to physical address was failed in "R_DRW_ToPhysicalAddress" function. */

/* Constant: E_RGA_OTHER_CF
*    <0x017300CF>. Not set work buffer B. */

/* Constant: E_RGA_OTHER_D4
*    <0x017300D4>. Not set work buffer B. */

/* Constant: E_RGA_OTHER_D6
*    <0x017300D6>. "graphics_config_t::work_buffer_b_size" is too small than JPEG image size. */

/* Constant: E_RGA_OTHER_DC
*    <0x017300DC>. Not set "F_GRAPHICS_FRAME_BUFFER" */

/* Constant: E_RGA_OTHER_DD
*    <0x017300DD>. Not set "graphics_config_t::frame_buffer" */

/* Constant: E_RGA_OTHER_DE
*    <0x017300DE>. Not set "F_GRAPHICS_FRAME_BUFFER" */

/* Constant: E_RGA_OTHER_DF
*    <0x017300DF>. Not set "graphics_config_t::frame_buffer" */

/* Constant: E_RGA_OTHER_F0
*    <0x017300F0>. Image format is not RGA Raw format. */

/* Constant: E_RGA_OTHER_F1
*    <0x017300F1>. "R_GRAPHICS_FinishPreviousFrame" can not be used, when "graphics_config_t::is_hybrid == true". */

/* Constant: E_RGA_OTHER_F2
*    <0x017300F2>. "R_GRAPHICS_StartPreviousFrame" can not be used, when "graphics_config_t::is_hybrid == true". */

/* Constant: E_RGA_OTHER_F4
*    <0x017300F4>. "R_GRAPHICS_Start" can not be used, when "graphics_config_t::has_frame_pipeline == true". */

/* Constant: E_RGA_OTHER_F5
*    <0x017300F5>. A number of frame buffer is too small. */

/* Constant: E_RGA_OTHER_F6
*    <0x017300F6>. Start address of JPEG data is not multiple of 8. */

/* Constant: E_RGA_OTHER_F7
*    <0x017300F7>. Start address of work buffer B is not multiple of 8. */

/* Constant: E_RGA_OTHER_F8
*    <0x017300F8>. The image witdh is 0 or smaller than 0. */

/* Constant: E_RGA_OTHER_F9
*    <0x017300F9>. The image height is 0 or smaller than 0. */

/* Constant: E_RGA_OTHER_FA
*    <0x017300FA>. Matrix setting is not translation matrix. */

/* Constant: E_RGA_OTHER_FB
*    <0x017300FB>. Matrix setting is not translation matrix. */

/* Constant: E_RGA_OTHER_FC
*    <0x017300FC>. Not set "graphics_config_t::frame_buffer" */

/* Constant: E_RGA_OTHER_FD
*    <0x017300FD>. X coodinate is not multiple of 2 in YUV422. */

/* Constant: E_RGA_OTHER_FE
*    <0x017300FE>. The width is not multiple of 2 in YUV422. */

/* Constant: JCU_ERROR_PARAM_A
*    <0x0145010A>. Parameter error */

/* Constant: JCU_ERROR_PARAM_B
*    <0x0145010B>. Parameter error */

/* Constant: JCU_ERROR_PARAM_C
*    <0x0145010C>. Parameter error */

/* Constant: JCU_ERROR_PARAM_D
*    <0x0145010D>. Parameter error */

/* Constant: JCU_ERROR_PARAM_F
*    <0x0145010F>. Parameter error */

/* Constant: JCU_ERROR_PARAM_1A
*    <0x0145011A>. Parameter error */

/* Constant: JCU_ERROR_PARAM_1B
*    <0x0145011B>. The destination address is not multiple of 8. */

/* Constant: JCU_ERROR_PARAM_1C
*    <0x0145011C>. Bad value in "outputCbCrOffset" */

/* Constant: JCU_ERROR_PARAM_1D
*    <0x0145011D>. Parameter error */

/* Constant: JCU_ERROR_PARAM_1E
*    <0x0145011E>. Parameter error */

/* Constant: JCU_ERROR_PARAM_1F
*    <0x0145011F>. Parameter error */

/* Constant: JCU_ERROR_PARAM_2A
*    <0x0145012A>. Parameter error */

/* Constant: JCU_ERROR_PARAM_2B
*    <0x0145012B>. Parameter error */

/* Constant: JCU_ERROR_PARAM_2C
*    <0x0145012C>. Parameter error */

/* Constant: JCU_ERROR_PARAM_2D
*    <0x0145012D>. Parameter error */

/* Constant: JCU_ERROR_PARAM_2E
*    <0x0145012E>. Parameter error */

/* Constant: JCU_ERROR_PARAM_2F
*    <0x0145012F>. Parameter error */

/* Constant: E_RGA_OTHER_1A
*    <0x0173001A>. Too small frame buffer width. */

/* Constant: E_RGA_OTHER_1B
*    <0x0173001B>. Too small frame buffer height. */

/* Constant: E_RGA_OTHER_3A
*    <0x0173003A>.  */

/* Constant: E_RGA_OTHER_3B
*    <0x0173003B>.  */

/* Constant: E_RGA_OTHER_3C
*    <0x0173003C>.  */

/* Constant: E_RGA_OTHER_3D
*    <0x0173003D>.  */

/* Constant: E_RGA_OTHER_4F
*    <0x0173004F>. The first member variable of "graphics_status_t" type is not initialized by 0 initializer. */

/* Constant: E_RGA_FEW_MEMORY_A
*    <0x0373000A>. Few heap memory area */

/* Constant: E_RGA_ACCESS_DENIED_A
*    <0x0773000A>. The memory of work buffer B must be cached memory. */

/* Constant: E_RGA_ACCESS_DENIED_1B
*    <0x0773001B>.  */

/* Constant: E_RGA_ACCESS_DENIED_1C
*    <0x0773001C>.  */

/* Constant: E_RGA_ACCESS_DENIED_1D
*    <0x0773001D>.  */

/* Constant: E_RGA_ACCESS_DENIED_1E
*    <0x0773001E>.  */

/* Constant: E_RGA_ACCESS_DENIED_1F
*    <0x0773001F>. The memory of frame buffer must be cached memory. */

/* Constant: JCU_ERROR_STATUS_A
*    <0x1045020A>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_B
*    <0x1045020B>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_C
*    <0x1045020C>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_D
*    <0x1045020D>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_E
*    <0x1045020E>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_F
*    <0x1045020F>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_PARAM_1
*    <0x01450101>. Parameter error */

/* Constant: JCU_ERROR_PARAM_2
*    <0x01450102>. Parameter error */

/* Constant: JCU_ERROR_PARAM_3
*    <0x01450103>. Parameter error */

/* Constant: JCU_ERROR_PARAM_4
*    <0x01450104>. Parameter error */

/* Constant: JCU_ERROR_PARAM_5
*    <0x01450105>. Parameter error */

/* Constant: JCU_ERROR_PARAM_6
*    <0x01450106>. Parameter error */

/* Constant: JCU_ERROR_PARAM_7
*    <0x01450107>. Parameter error */

/* Constant: JCU_ERROR_PARAM_8
*    <0x01450108>. Parameter error */

/* Constant: JCU_ERROR_PARAM_9
*    <0x01450109>. Parameter error */

/* Constant: JCU_ERROR_PARAM_19
*    <0x01450119>. Parameter error */

/* Constant: JCU_ERROR_PARAM_20
*    <0x01450120>. Parameter error */

/* Constant: JCU_ERROR_PARAM_21
*    <0x01450121>. Parameter error */

/* Constant: JCU_ERROR_PARAM_22
*    <0x01450122>. Parameter error */

/* Constant: JCU_ERROR_PARAM_23
*    <0x01450123>. Parameter error */

/* Constant: JCU_ERROR_PARAM_24
*    <0x01450124>. Parameter error */

/* Constant: JCU_ERROR_PARAM_25
*    <0x01450125>. Parameter error */

/* Constant: JCU_ERROR_PARAM_26
*    <0x01450126>. Parameter error */

/* Constant: JCU_ERROR_PARAM_27
*    <0x01450127>. Parameter error */

/* Constant: JCU_ERROR_PARAM_30
*    <0x01450130>. Parameter error */

/* Constant: JCU_ERROR_PARAM_31
*    <0x01450131>. Parameter error */

/* Constant: JCU_ERROR_PARAM_32
*    <0x01450132>. Parameter error */

/* Constant: JCU_ERROR_PARAM_33
*    <0x01450133>. Parameter error */

/* Constant: JCU_ERROR_PARAM_34
*    <0x01450134>. Bad value in "outputCbCrOffset" */

/* Constant: JCU_ERROR_PARAM_35
*    <0x01450135>. The source address is not multiple of 8. */

/* Constant: JCU_ERROR_PARAM_36
*    <0x01450136>. Arguments of "R_JCU_TerminateAsync" and "R_JCU_StartAsync" are same synchronized objects. */

/* Constant: JCU_ERROR_PARAM_61
*    <0x01450161>. RegistIntFunc */

/* Constant: JCU_ERROR_PARAM_62
*    <0x01450162>. IntPriority */

/* Constant: JCU_ERROR_PARAM_63
*    <0x01450163>. RegistIntFunc */

/* Constant: JCU_ERROR_PARAM_64
*    <0x01450164>. IntPriority */

/* Constant: E_RGA_OTHER_39
*    <0x01730039>.  */

/* Constant: E_RGA_OTHER_59
*    <0x01730059>. "graphics_config_t::jpeg_decoder" is "GRAPHICS_JPEG_DECODER_NONE". */

/* Constant: E_RGA_NOT_REGULAR_MATRIX_1
*    <0x01732801>. It is not regular matrix. */

/* Constant: E_RGA_NOT_REGULAR_MATRIX_2
*    <0x01732802>. It is not regular matrix. */

/* Constant: E_RGA_3x3MATRIX_1
*    <0x01732901>. Matrix is not 2x3. */

/* Constant: E_RGA_3x3MATRIX_2
*    <0x01732902>. Matrix is not 2x3. */

/* Constant: E_RGA_3x3MATRIX_3
*    <0x01732903>. Matrix is not 2x3. */

/* Constant: E_RGA_3x3MATRIX_4
*    <0x01732904>. Matrix is not 2x3. */

/* Constant: E_RGA_3x3MATRIX_5
*    <0x01732905>. Matrix is not 2x3. */

/* Constant: E_RGA_3x3MATRIX_6
*    <0x01732906>. Matrix is not 2x3. */

/* Constant: E_RGA_PL_OTHER_1
*    <0x01740001>. Work buffer B, only one of address and size is set. */

/* Constant: E_RGA_PL_OTHER_2
*    <0x01740002>. Work buffer B is already used. */

/* Constant: E_RGA_FEW_ARRAY_1
*    <0x02730001>. Too long path data. */

/* Constant: E_RGA_FEW_ARRAY_2
*    <0x02730002>. in_ImageDataArraySize argument is too small. */

/* Constant: E_RGA_FEW_ARRAY_5
*    <0x02730005>. "work_buffer_b_size" is smaller than "work_size_for_libPNG". */

/* Constant: E_RGA_FEW_ARRAY_6
*    <0x02730006>. "work_buffer_b_size" is too small. */

/* Constant: E_RGA_FEW_ARRAY_7
*    <0x02730007>. A path is already registered. */

/* Constant: E_RGA_FEW_ARRAY_9
*    <0x02730009>. Too small array size. */

/* Constant: E_RGA_PNG_FEW_MEMORY_1
*    <0x03736001>. The work memory for PNG is too small. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3A
*    <0x4973003A>. Not supported image format. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3C
*    <0x4973003C>. R_RGA_IsPixelFormatHavingAlpha(): Specified pixel format is not supported. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3D
*    <0x4973003D>.  */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3E
*    <0x4973003E>.  */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3F
*    <0x4973003F>. The pixel format of frame buffer is not supported. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_4A
*    <0x4973004A>.  */

/* Constant: E_MEMORY_ACCESS_DENIED_1
*    <0x07000001>. The specified address is not in cached area or cannot change to cached area. */

/* Constant: E_MEMORY_ACCESS_DENIED_2
*    <0x07000002>. The specified address is not in uncached area or cannot change to uncached area. */

/* Constant: E_MEMORY_ACCESS_DENIED_3
*    <0x07000003>. Changed address is not in cached area. */

/* Constant: E_MEMORY_ACCESS_DENIED_4
*    <0x07000004>. Changed address is not in uncached area. */

/* Constant: E_MEMORY_ACCESS_DENIED_5
*    <0x07000005>. The specified physical address is not same between before and after. */

/* Constant: E_MEMORY_ACCESS_DENIED_6
*    <0x07000006>. The specified physical address is not same between before and after. */

/* Constant: E_RGA_ACCESS_DENIED_15
*    <0x07730015>. The memory of frame buffer must be cached memory. */

/* Constant: E_RGA_ACCESS_DENIED_20
*    <0x07730020>. The memory of frame buffer must be cached memory. */

/* Constant: E_STATE
*    <0x10000000>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_1
*    <0x10450201>. Already initialized. */

/* Constant: JCU_ERROR_STATUS_2
*    <0x10450202>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_3
*    <0x10450203>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_4
*    <0x10450204>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_5
*    <0x10450205>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_6
*    <0x10450206>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_7
*    <0x10450207>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_8
*    <0x10450208>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_9
*    <0x10450209>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_10
*    <0x10450210>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_11
*    <0x10450211>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_12
*    <0x10450212>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_13
*    <0x10450213>. State error. This function must be called after changing to correct state. */

/* Constant: JCU_ERROR_STATUS_14
*    <0x10450214>.  */

/* Constant: JCU_ERROR_STATUS_16
*    <0x10450216>. Not called "R_JCU_SelectCodec" yet */

/* Constant: E_RGA_STATE_2
*    <0x10730002>. "graphics_t::RGAS_State" variable is not initialized by 0 initializer. */

/* Constant: E_RGA_STATE_3
*    <0x10730003>. "graphics_t" variable is not initialized by 0 initializer. */

/* Constant: E_RGA_STATE_4
*    <0x10730004>. "graphics_t::DRW_D2" variable is not initialized by 0 initializer. */

/* Constant: E_RGA_STATE_5
*    <0x10730005>. Not supported in "graphics_config_t::has_frame_pipeline == false". */

/* Constant: E_RGA_STATE_6
*    <0x10730006>. Not supported in "graphics_config_t::has_frame_pipeline == false". */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_22
*    <0x49730022>. Not supported pixel format. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_23
*    <0x49730023>. Not supported pixel format. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_40
*    <0x49730040>. Not supported value as graphics_composite_operation_t. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_41
*    <0x49730041>. Not supported pixel format. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_43
*    <0x49730043>. The pixel format of frame buffer is not supported. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_44
*    <0x49730044>. The pixel format of frame buffer is not supported. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_45
*    <0x49730045>. Not supported pixel format. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_46
*    <0x49730046>. Not supported pixel format. */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_47
*    <0x49730047>. "GRAPHICS_DESTINATION_OUT" must be set the format having alpha */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_48
*    <0x49730048>.  */

/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_49
*    <0x49730049>.  */


/* Title: NumOfDetailErrorCode_RGA */

/* Reference: 0x0B450001
*    <E_JCU_OBLIVIOUS_1> */

/* Reference: 0x0B450002
*    <E_JCU_OBLIVIOUS_2> */

/* Reference: 0x0B450003
*    <E_JCU_OBLIVIOUS_3> */

/* Reference: 0x0B730001
*    <E_RGA_OBLIVIOUS_1> */

/* Reference: 0x0B730002
*    <E_RGA_OBLIVIOUS_2> */

/* Reference: 0x0B730004
*    <E_RGA_OBLIVIOUS_4> */

/* Reference: 0x0B730005
*    <E_RGA_OBLIVIOUS_5> */

/* Reference: 0x0B730006
*    <E_RGA_OBLIVIOUS_6> */

/* Reference: 0x0B730007
*    <E_RGA_OBLIVIOUS_7> */

/* Reference: 0x0B790001
*    <E_DRW_OBLIVIOUS_1> */

/* Reference: 0x0B790002
*    <E_DRW_OBLIVIOUS_2> */

/* Reference: 0x0B790003
*    <E_DRW_OBLIVIOUS_3> */

/* Reference: 0x0B790004
*    <E_DRW_OBLIVIOUS_4> */

/* Reference: 0x0B790005
*    <E_DRW_OBLIVIOUS_5> */

/* Reference: 0x0B790006
*    <E_DRW_OBLIVIOUS_6> */

/* Reference: 0x12790001
*    <E_DRW_THREAD_DENIED_1> */

/* Reference: 0x0D450001
*    <E_JCU_UNRECOVERABLE_1> */

/* Reference: 0x01790C01
*    <E_DRW_NOVIDEOMEM_1> */

/* Reference: 0x01790C02
*    <E_DRW_NOVIDEOMEM_2> */

/* Reference: 0x017300CF
*    <E_RGA_OTHER_CF> */

/* Reference: 0x017300D4
*    <E_RGA_OTHER_D4> */

/* Reference: 0x017300D6
*    <E_RGA_OTHER_D6> */

/* Reference: 0x017300DC
*    <E_RGA_OTHER_DC> */

/* Reference: 0x017300DD
*    <E_RGA_OTHER_DD> */

/* Reference: 0x017300DE
*    <E_RGA_OTHER_DE> */

/* Reference: 0x017300DF
*    <E_RGA_OTHER_DF> */

/* Reference: 0x017300F0
*    <E_RGA_OTHER_F0> */

/* Reference: 0x017300F1
*    <E_RGA_OTHER_F1> */

/* Reference: 0x017300F2
*    <E_RGA_OTHER_F2> */

/* Reference: 0x017300F4
*    <E_RGA_OTHER_F4> */

/* Reference: 0x017300F5
*    <E_RGA_OTHER_F5> */

/* Reference: 0x017300F6
*    <E_RGA_OTHER_F6> */

/* Reference: 0x017300F7
*    <E_RGA_OTHER_F7> */

/* Reference: 0x017300F8
*    <E_RGA_OTHER_F8> */

/* Reference: 0x017300F9
*    <E_RGA_OTHER_F9> */

/* Reference: 0x017300FA
*    <E_RGA_OTHER_FA> */

/* Reference: 0x017300FB
*    <E_RGA_OTHER_FB> */

/* Reference: 0x017300FC
*    <E_RGA_OTHER_FC> */

/* Reference: 0x017300FD
*    <E_RGA_OTHER_FD> */

/* Reference: 0x017300FE
*    <E_RGA_OTHER_FE> */

/* Reference: 0x0145010A
*    <JCU_ERROR_PARAM_A> */

/* Reference: 0x0145010B
*    <JCU_ERROR_PARAM_B> */

/* Reference: 0x0145010C
*    <JCU_ERROR_PARAM_C> */

/* Reference: 0x0145010D
*    <JCU_ERROR_PARAM_D> */

/* Reference: 0x0145010F
*    <JCU_ERROR_PARAM_F> */

/* Reference: 0x0145011A
*    <JCU_ERROR_PARAM_1A> */

/* Reference: 0x0145011B
*    <JCU_ERROR_PARAM_1B> */

/* Reference: 0x0145011C
*    <JCU_ERROR_PARAM_1C> */

/* Reference: 0x0145011D
*    <JCU_ERROR_PARAM_1D> */

/* Reference: 0x0145011E
*    <JCU_ERROR_PARAM_1E> */

/* Reference: 0x0145011F
*    <JCU_ERROR_PARAM_1F> */

/* Reference: 0x0145012A
*    <JCU_ERROR_PARAM_2A> */

/* Reference: 0x0145012B
*    <JCU_ERROR_PARAM_2B> */

/* Reference: 0x0145012C
*    <JCU_ERROR_PARAM_2C> */

/* Reference: 0x0145012D
*    <JCU_ERROR_PARAM_2D> */

/* Reference: 0x0145012E
*    <JCU_ERROR_PARAM_2E> */

/* Reference: 0x0145012F
*    <JCU_ERROR_PARAM_2F> */

/* Reference: 0x0173001A
*    <E_RGA_OTHER_1A> */

/* Reference: 0x0173001B
*    <E_RGA_OTHER_1B> */

/* Reference: 0x0173003A
*    <E_RGA_OTHER_3A> */

/* Reference: 0x0173003B
*    <E_RGA_OTHER_3B> */

/* Reference: 0x0173003C
*    <E_RGA_OTHER_3C> */

/* Reference: 0x0173003D
*    <E_RGA_OTHER_3D> */

/* Reference: 0x0173004F
*    <E_RGA_OTHER_4F> */

/* Reference: 0x0373000A
*    <E_RGA_FEW_MEMORY_A> */

/* Reference: 0x0773000A
*    <E_RGA_ACCESS_DENIED_A> */

/* Reference: 0x0773001B
*    <E_RGA_ACCESS_DENIED_1B> */

/* Reference: 0x0773001C
*    <E_RGA_ACCESS_DENIED_1C> */

/* Reference: 0x0773001D
*    <E_RGA_ACCESS_DENIED_1D> */

/* Reference: 0x0773001E
*    <E_RGA_ACCESS_DENIED_1E> */

/* Reference: 0x0773001F
*    <E_RGA_ACCESS_DENIED_1F> */

/* Reference: 0x1045020A
*    <JCU_ERROR_STATUS_A> */

/* Reference: 0x1045020B
*    <JCU_ERROR_STATUS_B> */

/* Reference: 0x1045020C
*    <JCU_ERROR_STATUS_C> */

/* Reference: 0x1045020D
*    <JCU_ERROR_STATUS_D> */

/* Reference: 0x1045020E
*    <JCU_ERROR_STATUS_E> */

/* Reference: 0x1045020F
*    <JCU_ERROR_STATUS_F> */

/* Reference: 0x01450101
*    <JCU_ERROR_PARAM_1> */

/* Reference: 0x01450102
*    <JCU_ERROR_PARAM_2> */

/* Reference: 0x01450103
*    <JCU_ERROR_PARAM_3> */

/* Reference: 0x01450104
*    <JCU_ERROR_PARAM_4> */

/* Reference: 0x01450105
*    <JCU_ERROR_PARAM_5> */

/* Reference: 0x01450106
*    <JCU_ERROR_PARAM_6> */

/* Reference: 0x01450107
*    <JCU_ERROR_PARAM_7> */

/* Reference: 0x01450108
*    <JCU_ERROR_PARAM_8> */

/* Reference: 0x01450109
*    <JCU_ERROR_PARAM_9> */

/* Reference: 0x01450119
*    <JCU_ERROR_PARAM_19> */

/* Reference: 0x01450120
*    <JCU_ERROR_PARAM_20> */

/* Reference: 0x01450121
*    <JCU_ERROR_PARAM_21> */

/* Reference: 0x01450122
*    <JCU_ERROR_PARAM_22> */

/* Reference: 0x01450123
*    <JCU_ERROR_PARAM_23> */

/* Reference: 0x01450124
*    <JCU_ERROR_PARAM_24> */

/* Reference: 0x01450125
*    <JCU_ERROR_PARAM_25> */

/* Reference: 0x01450126
*    <JCU_ERROR_PARAM_26> */

/* Reference: 0x01450127
*    <JCU_ERROR_PARAM_27> */

/* Reference: 0x01450130
*    <JCU_ERROR_PARAM_30> */

/* Reference: 0x01450131
*    <JCU_ERROR_PARAM_31> */

/* Reference: 0x01450132
*    <JCU_ERROR_PARAM_32> */

/* Reference: 0x01450133
*    <JCU_ERROR_PARAM_33> */

/* Reference: 0x01450134
*    <JCU_ERROR_PARAM_34> */

/* Reference: 0x01450135
*    <JCU_ERROR_PARAM_35> */

/* Reference: 0x01450136
*    <JCU_ERROR_PARAM_36> */

/* Reference: 0x01450161
*    <JCU_ERROR_PARAM_61> */

/* Reference: 0x01450162
*    <JCU_ERROR_PARAM_62> */

/* Reference: 0x01450163
*    <JCU_ERROR_PARAM_63> */

/* Reference: 0x01450164
*    <JCU_ERROR_PARAM_64> */

/* Reference: 0x01730039
*    <E_RGA_OTHER_39> */

/* Reference: 0x01730059
*    <E_RGA_OTHER_59> */

/* Reference: 0x01732801
*    <E_RGA_NOT_REGULAR_MATRIX_1> */

/* Reference: 0x01732802
*    <E_RGA_NOT_REGULAR_MATRIX_2> */

/* Reference: 0x01732901
*    <E_RGA_3x3MATRIX_1> */

/* Reference: 0x01732902
*    <E_RGA_3x3MATRIX_2> */

/* Reference: 0x01732903
*    <E_RGA_3x3MATRIX_3> */

/* Reference: 0x01732904
*    <E_RGA_3x3MATRIX_4> */

/* Reference: 0x01732905
*    <E_RGA_3x3MATRIX_5> */

/* Reference: 0x01732906
*    <E_RGA_3x3MATRIX_6> */

/* Reference: 0x01740001
*    <E_RGA_PL_OTHER_1> */

/* Reference: 0x01740002
*    <E_RGA_PL_OTHER_2> */

/* Reference: 0x02730001
*    <E_RGA_FEW_ARRAY_1> */

/* Reference: 0x02730002
*    <E_RGA_FEW_ARRAY_2> */

/* Reference: 0x02730005
*    <E_RGA_FEW_ARRAY_5> */

/* Reference: 0x02730006
*    <E_RGA_FEW_ARRAY_6> */

/* Reference: 0x02730007
*    <E_RGA_FEW_ARRAY_7> */

/* Reference: 0x02730009
*    <E_RGA_FEW_ARRAY_9> */

/* Reference: 0x03736001
*    <E_RGA_PNG_FEW_MEMORY_1> */

/* Reference: 0x4973003A
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3A> */

/* Reference: 0x4973003C
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3C> */

/* Reference: 0x4973003D
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3D> */

/* Reference: 0x4973003E
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3E> */

/* Reference: 0x4973003F
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3F> */

/* Reference: 0x4973004A
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_4A> */

/* Reference: 0x07000001
*    <E_MEMORY_ACCESS_DENIED_1> */

/* Reference: 0x07000002
*    <E_MEMORY_ACCESS_DENIED_2> */

/* Reference: 0x07000003
*    <E_MEMORY_ACCESS_DENIED_3> */

/* Reference: 0x07000004
*    <E_MEMORY_ACCESS_DENIED_4> */

/* Reference: 0x07000005
*    <E_MEMORY_ACCESS_DENIED_5> */

/* Reference: 0x07000006
*    <E_MEMORY_ACCESS_DENIED_6> */

/* Reference: 0x07730015
*    <E_RGA_ACCESS_DENIED_15> */

/* Reference: 0x07730020
*    <E_RGA_ACCESS_DENIED_20> */

/* Reference: 0x10000000
*    <E_STATE> */

/* Reference: 0x10450201
*    <JCU_ERROR_STATUS_1> */

/* Reference: 0x10450202
*    <JCU_ERROR_STATUS_2> */

/* Reference: 0x10450203
*    <JCU_ERROR_STATUS_3> */

/* Reference: 0x10450204
*    <JCU_ERROR_STATUS_4> */

/* Reference: 0x10450205
*    <JCU_ERROR_STATUS_5> */

/* Reference: 0x10450206
*    <JCU_ERROR_STATUS_6> */

/* Reference: 0x10450207
*    <JCU_ERROR_STATUS_7> */

/* Reference: 0x10450208
*    <JCU_ERROR_STATUS_8> */

/* Reference: 0x10450209
*    <JCU_ERROR_STATUS_9> */

/* Reference: 0x10450210
*    <JCU_ERROR_STATUS_10> */

/* Reference: 0x10450211
*    <JCU_ERROR_STATUS_11> */

/* Reference: 0x10450212
*    <JCU_ERROR_STATUS_12> */

/* Reference: 0x10450213
*    <JCU_ERROR_STATUS_13> */

/* Reference: 0x10450214
*    <JCU_ERROR_STATUS_14> */

/* Reference: 0x10450216
*    <JCU_ERROR_STATUS_16> */

/* Reference: 0x10730002
*    <E_RGA_STATE_2> */

/* Reference: 0x10730003
*    <E_RGA_STATE_3> */

/* Reference: 0x10730004
*    <E_RGA_STATE_4> */

/* Reference: 0x10730005
*    <E_RGA_STATE_5> */

/* Reference: 0x10730006
*    <E_RGA_STATE_6> */

/* Reference: 0x49730022
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_22> */

/* Reference: 0x49730023
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_23> */

/* Reference: 0x49730040
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_40> */

/* Reference: 0x49730041
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_41> */

/* Reference: 0x49730043
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_43> */

/* Reference: 0x49730044
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_44> */

/* Reference: 0x49730045
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_45> */

/* Reference: 0x49730046
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_46> */

/* Reference: 0x49730047
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_47> */

/* Reference: 0x49730048
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_48> */

/* Reference: 0x49730049
*    <E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_49> */

#ifndef  A
#define  A
#endif
