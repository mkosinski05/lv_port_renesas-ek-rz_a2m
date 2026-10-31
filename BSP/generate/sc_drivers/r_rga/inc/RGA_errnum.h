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
* File: RGA_errnum.h
*    Error Code
*
* - $Version: $
* - $ModuleRevision: {Masters}\RGA\driver\RZA2_Test\19_en $
* - $Date: 2018-11-14T10:48:36+09:00 $
************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#ifndef RGA_ERRNUM_H
#define RGA_ERRNUM_H

#ifdef  __cplusplus
extern "C" {
#endif


/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Constant: E_RGA
*    Error code group of the RGA.
************************************************************************/
/* Constant: E_RGA_MASK
*    Mask for checking E_RGA. */
#define  E_RGA            ( 0x00730000 )
#define  E_RGA_MASK       ( 0x00FF0000 )

/* Sub class of E_OTHER */
#define  E_RGA_OTHER_1    ( 0x01730001 )
#define  E_RGA_OTHER_2    ( 0x01730002 )
#define  E_RGA_OTHER_3    ( 0x01730003 )
#define  E_RGA_OTHER_4    ( 0x01730004 )
#define  E_RGA_OTHER_5    ( 0x01730005 )
#define  E_RGA_OTHER_6    ( 0x01730006 )
#define  E_RGA_OTHER_7    ( 0x01730007 )
#define  E_RGA_OTHER_8    ( 0x01730008 )
#define  E_RGA_OTHER_9    ( 0x01730009 )
#define  E_RGA_OTHER_A    ( 0x0173000A )
#define  E_RGA_OTHER_B    ( 0x0173000B )
#define  E_RGA_OTHER_C    ( 0x0173000C )
#define  E_RGA_OTHER_D    ( 0x0173000D )
#define  E_RGA_OTHER_E    ( 0x0173000E )
#define  E_RGA_OTHER_F    ( 0x0173000F )
#define  E_RGA_OTHER_10   ( 0x01730010 )
#define  E_RGA_OTHER_11   ( 0x01730011 )
#define  E_RGA_OTHER_12   ( 0x01730012 )
#define  E_RGA_OTHER_13   ( 0x01730013 )
#define  E_RGA_OTHER_14   ( 0x01730014 )
#define  E_RGA_OTHER_15   ( 0x01730015 )
#define  E_RGA_OTHER_16   ( 0x01730016 )
#define  E_RGA_OTHER_17   ( 0x01730017 )
#define  E_RGA_OTHER_18   ( 0x01730018 )
#define  E_RGA_OTHER_19   ( 0x01730019 )
#define  E_RGA_OTHER_1A   ( 0x0173001A )
#define  E_RGA_OTHER_1B   ( 0x0173001B )
#define  E_RGA_OTHER_1C   ( 0x0173001C )
#define  E_RGA_OTHER_1D   ( 0x0173001D )
#define  E_RGA_OTHER_1E   ( 0x0173001E )
#define  E_RGA_OTHER_1F   ( 0x0173001F )
#define  E_RGA_OTHER_20   ( 0x01730020 )
#define  E_RGA_OTHER_21   ( 0x01730021 )
#define  E_RGA_OTHER_22   ( 0x01730022 )
#define  E_RGA_OTHER_23   ( 0x01730023 )
#define  E_RGA_OTHER_24   ( 0x01730024 )
#define  E_RGA_OTHER_25   ( 0x01730025 )
#define  E_RGA_OTHER_26   ( 0x01730026 )
#define  E_RGA_OTHER_27   ( 0x01730027 )
#define  E_RGA_OTHER_28   ( 0x01730028 )
#define  E_RGA_OTHER_29   ( 0x01730029 )
#define  E_RGA_OTHER_2A   ( 0x0173002A )
#define  E_RGA_OTHER_2B   ( 0x0173002B )
#define  E_RGA_OTHER_2C   ( 0x0173002C )
#define  E_RGA_OTHER_2D   ( 0x0173002D )
#define  E_RGA_OTHER_2E   ( 0x0173002E )
#define  E_RGA_OTHER_2F   ( 0x0173002F )
#define  E_RGA_OTHER_30   ( 0x01730030 )
#define  E_RGA_OTHER_31   ( 0x01730031 )
#define  E_RGA_OTHER_32   ( 0x01730032 )
#define  E_RGA_OTHER_33   ( 0x01730033 )
#define  E_RGA_OTHER_34   ( 0x01730034 )
#define  E_RGA_OTHER_35   ( 0x01730035 )
#define  E_RGA_OTHER_36   ( 0x01730036 )
#define  E_RGA_OTHER_37   ( 0x01730037 )
#define  E_RGA_OTHER_38   ( 0x01730038 )
#define  E_RGA_OTHER_39   ( 0x01730039 )
#define  E_RGA_OTHER_3A   ( 0x0173003A )
#define  E_RGA_OTHER_3B   ( 0x0173003B )
#define  E_RGA_OTHER_3C   ( 0x0173003C )
#define  E_RGA_OTHER_3D   ( 0x0173003D )
#define  E_RGA_OTHER_3E   ( 0x0173003E )
#define  E_RGA_OTHER_3F   ( 0x0173003F )
#define  E_RGA_OTHER_40   ( 0x01730040 )
#define  E_RGA_OTHER_41   ( 0x01730041 )
#define  E_RGA_OTHER_42   ( 0x01730042 )
#define  E_RGA_OTHER_43   ( 0x01730043 )
#define  E_RGA_OTHER_44   ( 0x01730044 )
#define  E_RGA_OTHER_45   ( 0x01730045 )
#define  E_RGA_OTHER_46   ( 0x01730046 )
#define  E_RGA_OTHER_47   ( 0x01730047 )
#define  E_RGA_OTHER_48   ( 0x01730048 )
#define  E_RGA_OTHER_49   ( 0x01730049 )
#define  E_RGA_OTHER_4A   ( 0x0173004A )
#define  E_RGA_OTHER_4B   ( 0x0173004B )
#define  E_RGA_OTHER_4C   ( 0x0173004C )
#define  E_RGA_OTHER_4D   ( 0x0173004D )
#define  E_RGA_OTHER_4E   ( 0x0173004E )
#define  E_RGA_OTHER_4F   ( 0x0173004F )
#define  E_RGA_OTHER_50   ( 0x01730050 )
#define  E_RGA_OTHER_51   ( 0x01730051 )
#define  E_RGA_OTHER_52   ( 0x01730052 )
#define  E_RGA_OTHER_53   ( 0x01730053 )
#define  E_RGA_OTHER_54   ( 0x01730054 )
#define  E_RGA_OTHER_55   ( 0x01730055 )
#define  E_RGA_OTHER_56   ( 0x01730056 )
#define  E_RGA_OTHER_57   ( 0x01730057 )
#define  E_RGA_OTHER_58   ( 0x01730058 )
#define  E_RGA_OTHER_59   ( 0x01730059 )
#define  E_RGA_OTHER_5A   ( 0x0173005A )
#define  E_RGA_OTHER_5B   ( 0x0173005B )
#define  E_RGA_OTHER_5C   ( 0x0173005C )
#define  E_RGA_OTHER_5D   ( 0x0173005D )
#define  E_RGA_OTHER_5E   ( 0x0173005E )
#define  E_RGA_OTHER_5F   ( 0x0173005F )
#define  E_RGA_OTHER_60   ( 0x01730060 )
#define  E_RGA_OTHER_61   ( 0x01730061 )
#define  E_RGA_OTHER_62   ( 0x01730062 )
#define  E_RGA_OTHER_63   ( 0x01730063 )
#define  E_RGA_OTHER_64   ( 0x01730064 )
#define  E_RGA_OTHER_65   ( 0x01730065 )
#define  E_RGA_OTHER_66   ( 0x01730066 )
#define  E_RGA_OTHER_67   ( 0x01730067 )
#define  E_RGA_OTHER_68   ( 0x01730068 )
#define  E_RGA_OTHER_69   ( 0x01730069 )
#define  E_RGA_OTHER_6A   ( 0x0173006A )
#define  E_RGA_OTHER_6B   ( 0x0173006B )
#define  E_RGA_OTHER_6C   ( 0x0173006C )
#define  E_RGA_OTHER_6D   ( 0x0173006D )
#define  E_RGA_OTHER_6E   ( 0x0173006E )
#define  E_RGA_OTHER_6F   ( 0x0173006F )
#define  E_RGA_OTHER_70   ( 0x01730070 )
#define  E_RGA_OTHER_71   ( 0x01730071 )
#define  E_RGA_OTHER_72   ( 0x01730072 )
#define  E_RGA_OTHER_73   ( 0x01730073 )
#define  E_RGA_OTHER_74   ( 0x01730074 )
#define  E_RGA_OTHER_75   ( 0x01730075 )
#define  E_RGA_OTHER_76   ( 0x01730076 )
#define  E_RGA_OTHER_77   ( 0x01730077 )
#define  E_RGA_OTHER_78   ( 0x01730078 )
#define  E_RGA_OTHER_79   ( 0x01730079 )
#define  E_RGA_OTHER_7A   ( 0x0173007A )
#define  E_RGA_OTHER_7B   ( 0x0173007B )
#define  E_RGA_OTHER_7C   ( 0x0173007C )
#define  E_RGA_OTHER_7D   ( 0x0173007D )
#define  E_RGA_OTHER_7E   ( 0x0173007E )
#define  E_RGA_OTHER_7F   ( 0x0173007F )
#define  E_RGA_OTHER_80   ( 0x01730080 )
#define  E_RGA_OTHER_81   ( 0x01730081 )
#define  E_RGA_OTHER_82   ( 0x01730082 )
#define  E_RGA_OTHER_83   ( 0x01730083 )
#define  E_RGA_OTHER_84   ( 0x01730084 )
#define  E_RGA_OTHER_85   ( 0x01730085 )
#define  E_RGA_OTHER_86   ( 0x01730086 )
#define  E_RGA_OTHER_87   ( 0x01730087 )
#define  E_RGA_OTHER_88   ( 0x01730088 )
#define  E_RGA_OTHER_89   ( 0x01730089 )
#define  E_RGA_OTHER_8A   ( 0x0173008A )
#define  E_RGA_OTHER_8B   ( 0x0173008B )
#define  E_RGA_OTHER_8C   ( 0x0173008C )
#define  E_RGA_OTHER_8D   ( 0x0173008D )
#define  E_RGA_OTHER_8E   ( 0x0173008E )
#define  E_RGA_OTHER_8F   ( 0x0173008F )
#define  E_RGA_OTHER_90   ( 0x01730090 )
#define  E_RGA_OTHER_91   ( 0x01730091 )
#define  E_RGA_OTHER_92   ( 0x01730092 )
#define  E_RGA_OTHER_93   ( 0x01730093 )
#define  E_RGA_OTHER_94   ( 0x01730094 )
#define  E_RGA_OTHER_95   ( 0x01730095 )
#define  E_RGA_OTHER_96   ( 0x01730096 )
#define  E_RGA_OTHER_97   ( 0x01730097 )
#define  E_RGA_OTHER_98   ( 0x01730098 )
#define  E_RGA_OTHER_99   ( 0x01730099 )
#define  E_RGA_OTHER_9A   ( 0x0173009A )
#define  E_RGA_OTHER_9B   ( 0x0173009B )
#define  E_RGA_OTHER_9C   ( 0x0173009C )
#define  E_RGA_OTHER_9D   ( 0x0173009D )
#define  E_RGA_OTHER_9E   ( 0x0173009E )
#define  E_RGA_OTHER_9F   ( 0x0173009F )
#define  E_RGA_OTHER_A0   ( 0x017300A0 )
#define  E_RGA_OTHER_A1   ( 0x017300A1 )
#define  E_RGA_OTHER_A2   ( 0x017300A2 )
#define  E_RGA_OTHER_A3   ( 0x017300A3 )
#define  E_RGA_OTHER_A4   ( 0x017300A4 )
#define  E_RGA_OTHER_A5   ( 0x017300A5 )
#define  E_RGA_OTHER_A6   ( 0x017300A6 )
#define  E_RGA_OTHER_A7   ( 0x017300A7 )
#define  E_RGA_OTHER_A8   ( 0x017300A8 )
#define  E_RGA_OTHER_A9   ( 0x017300A9 )
#define  E_RGA_OTHER_AA   ( 0x017300AA )
#define  E_RGA_OTHER_AB   ( 0x017300AB )
#define  E_RGA_OTHER_AC   ( 0x017300AC )
#define  E_RGA_OTHER_AD   ( 0x017300AD )
#define  E_RGA_OTHER_AE   ( 0x017300AE )
#define  E_RGA_OTHER_AF   ( 0x017300AF )
#define  E_RGA_OTHER_B0   ( 0x017300B0 )
#define  E_RGA_OTHER_B1   ( 0x017300B1 )
#define  E_RGA_OTHER_B2   ( 0x017300B2 )
#define  E_RGA_OTHER_B3   ( 0x017300B3 )
#define  E_RGA_OTHER_B4   ( 0x017300B4 )
#define  E_RGA_OTHER_B5   ( 0x017300B5 )
#define  E_RGA_OTHER_B6   ( 0x017300B6 )
#define  E_RGA_OTHER_B7   ( 0x017300B7 )
#define  E_RGA_OTHER_B8   ( 0x017300B8 )
#define  E_RGA_OTHER_B9   ( 0x017300B9 )
#define  E_RGA_OTHER_BA   ( 0x017300BA )
#define  E_RGA_OTHER_BB   ( 0x017300BB )
#define  E_RGA_OTHER_BC   ( 0x017300BC )
#define  E_RGA_OTHER_BD   ( 0x017300BD )
#define  E_RGA_OTHER_BE   ( 0x017300BE )
#define  E_RGA_OTHER_BF   ( 0x017300BF )
#define  E_RGA_OTHER_C0   ( 0x017300C0 )
#define  E_RGA_OTHER_C1   ( 0x017300C1 )
#define  E_RGA_OTHER_C2   ( 0x017300C2 )
#define  E_RGA_OTHER_C3   ( 0x017300C3 )
#define  E_RGA_OTHER_C4   ( 0x017300C4 )
#define  E_RGA_OTHER_C5   ( 0x017300C5 )
#define  E_RGA_OTHER_C6   ( 0x017300C6 )
#define  E_RGA_OTHER_C7   ( 0x017300C7 )
#define  E_RGA_OTHER_C8   ( 0x017300C8 )
#define  E_RGA_OTHER_C9   ( 0x017300C9 )
#define  E_RGA_OTHER_CA   ( 0x017300CA )
#define  E_RGA_OTHER_CB   ( 0x017300CB )
#define  E_RGA_OTHER_CC   ( 0x017300CC )
#define  E_RGA_OTHER_CD   ( 0x017300CD )
#define  E_RGA_OTHER_CE   ( 0x017300CE )
#define  E_RGA_OTHER_CF   ( 0x017300CF )
#define  E_RGA_OTHER_D0   ( 0x017300D0 )
#define  E_RGA_OTHER_D1   ( 0x017300D1 )
#define  E_RGA_OTHER_D2   ( 0x017300D2 )
#define  E_RGA_OTHER_D3   ( 0x017300D3 )
#define  E_RGA_OTHER_D4   ( 0x017300D4 )
#define  E_RGA_OTHER_D5   ( 0x017300D5 )
#define  E_RGA_OTHER_D6   ( 0x017300D6 )
#define  E_RGA_OTHER_D7   ( 0x017300D7 )
#define  E_RGA_OTHER_D8   ( 0x017300D8 )
#define  E_RGA_OTHER_D9   ( 0x017300D9 )
#define  E_RGA_OTHER_DA   ( 0x017300DA )
#define  E_RGA_OTHER_DB   ( 0x017300DB )
#define  E_RGA_OTHER_DC   ( 0x017300DC )
#define  E_RGA_OTHER_DD   ( 0x017300DD )
#define  E_RGA_OTHER_DE   ( 0x017300DE )
#define  E_RGA_OTHER_DF   ( 0x017300DF )
#define  E_RGA_OTHER_F0   ( 0x017300F0 )
#define  E_RGA_OTHER_F1   ( 0x017300F1 )
#define  E_RGA_OTHER_F2   ( 0x017300F2 )
#define  E_RGA_OTHER_F3   ( 0x017300F3 )
#define  E_RGA_OTHER_F4   ( 0x017300F4 )
#define  E_RGA_OTHER_F5   ( 0x017300F5 )
#define  E_RGA_OTHER_F6   ( 0x017300F6 )
#define  E_RGA_OTHER_F7   ( 0x017300F7 )
#define  E_RGA_OTHER_F8   ( 0x017300F8 )
#define  E_RGA_OTHER_F9   ( 0x017300F9 )
#define  E_RGA_OTHER_FA   ( 0x017300FA )
#define  E_RGA_OTHER_FB   ( 0x017300FB )
#define  E_RGA_OTHER_FC   ( 0x017300FC )
#define  E_RGA_OTHER_FD   ( 0x017300FD )
#define  E_RGA_OTHER_FE   ( 0x017300FE )
#define  E_RGA_OTHER_FF   ( 0x017300FF )
#define  E_RGA_OTHER_100  ( 0x01730100 )

/* Sub class of E_FEW_ARRAY */
#define  E_RGA_FEW_ARRAY_1   ( 0x02730001 )
#define  E_RGA_FEW_ARRAY_2   ( 0x02730002 )
#define  E_RGA_FEW_ARRAY_3   ( 0x02730003 )
#define  E_RGA_FEW_ARRAY_4   ( 0x02730004 )
#define  E_RGA_FEW_ARRAY_5   ( 0x02730005 )
#define  E_RGA_FEW_ARRAY_6   ( 0x02730006 )
#define  E_RGA_FEW_ARRAY_7   ( 0x02730007 )
#define  E_RGA_FEW_ARRAY_8   ( 0x02730008 )
#define  E_RGA_FEW_ARRAY_9   ( 0x02730009 )

/* Sub class of E_FEW_MEMORY */
#define  E_RGA_FEW_MEMORY_1   ( 0x03730001 )
#define  E_RGA_FEW_MEMORY_2   ( 0x03730002 )
#define  E_RGA_FEW_MEMORY_3   ( 0x03730003 )
#define  E_RGA_FEW_MEMORY_4   ( 0x03730004 )
#define  E_RGA_FEW_MEMORY_5   ( 0x03730005 )
#define  E_RGA_FEW_MEMORY_6   ( 0x03730006 )
#define  E_RGA_FEW_MEMORY_7   ( 0x03730007 )
#define  E_RGA_FEW_MEMORY_8   ( 0x03730008 )
#define  E_RGA_FEW_MEMORY_9   ( 0x03730009 )
#define  E_RGA_FEW_MEMORY_A   ( 0x0373000A )

/* Sub class of E_ACCESS_DENIED */
#define  E_RGA_ACCESS_DENIED_1   ( 0x07730001 )
#define  E_RGA_ACCESS_DENIED_2   ( 0x07730002 )
#define  E_RGA_ACCESS_DENIED_3   ( 0x07730003 )
#define  E_RGA_ACCESS_DENIED_4   ( 0x07730004 )
#define  E_RGA_ACCESS_DENIED_5   ( 0x07730005 )
#define  E_RGA_ACCESS_DENIED_6   ( 0x07730006 )
#define  E_RGA_ACCESS_DENIED_7   ( 0x07730007 )
#define  E_RGA_ACCESS_DENIED_8   ( 0x07730008 )
#define  E_RGA_ACCESS_DENIED_9   ( 0x07730009 )
#define  E_RGA_ACCESS_DENIED_A   ( 0x0773000A )
#define  E_RGA_ACCESS_DENIED_B   ( 0x0773000B )
#define  E_RGA_ACCESS_DENIED_C   ( 0x0773000C )
#define  E_RGA_ACCESS_DENIED_D   ( 0x0773000D )
#define  E_RGA_ACCESS_DENIED_E   ( 0x0773000E )
#define  E_RGA_ACCESS_DENIED_F   ( 0x0773000F )
#define  E_RGA_ACCESS_DENIED_10  ( 0x07730010 )
#define  E_RGA_ACCESS_DENIED_11  ( 0x07730011 )
#define  E_RGA_ACCESS_DENIED_12  ( 0x07730012 )
#define  E_RGA_ACCESS_DENIED_13  ( 0x07730013 )
#define  E_RGA_ACCESS_DENIED_14  ( 0x07730014 )
#define  E_RGA_ACCESS_DENIED_15  ( 0x07730015 )
#define  E_RGA_ACCESS_DENIED_16  ( 0x07730016 )
#define  E_RGA_ACCESS_DENIED_17  ( 0x07730017 )
#define  E_RGA_ACCESS_DENIED_18  ( 0x07730018 )
#define  E_RGA_ACCESS_DENIED_19  ( 0x07730019 )
#define  E_RGA_ACCESS_DENIED_1A  ( 0x0773001A )
#define  E_RGA_ACCESS_DENIED_1B  ( 0x0773001B )
#define  E_RGA_ACCESS_DENIED_1C  ( 0x0773001C )
#define  E_RGA_ACCESS_DENIED_1D  ( 0x0773001D )
#define  E_RGA_ACCESS_DENIED_1E  ( 0x0773001E )
#define  E_RGA_ACCESS_DENIED_1F  ( 0x0773001F )
#define  E_RGA_ACCESS_DENIED_20  ( 0x07730020 )
#define  E_RGA_ACCESS_DENIED_21  ( 0x07730021 )

/* Sub class of E_NOT_IMPLEMENT_YET */
#define  E_RGA_NOT_IMPLEMENT_YET_1  ( 0x09730001 )
#define  E_RGA_NOT_IMPLEMENT_YET_2  ( 0x09730002 )

/* Sub class of E_OBLIVIOUS */
#define  E_RGA_OBLIVIOUS_1    ( 0x0B730001 )
#define  E_RGA_OBLIVIOUS_2    ( 0x0B730002 )
#define  E_RGA_OBLIVIOUS_3    ( 0x0B730003 )
#define  E_RGA_OBLIVIOUS_4    ( 0x0B730004 )
#define  E_RGA_OBLIVIOUS_5    ( 0x0B730005 )
#define  E_RGA_OBLIVIOUS_6    ( 0x0B730006 )
#define  E_RGA_OBLIVIOUS_7    ( 0x0B730007 )
#define  E_RGA_OBLIVIOUS_8    ( 0x0B730008 )

/* Sub class of E_STATE_CLEAR */
#define  E_RGA_STATE_CLEAR_1  ( 0x0C730001 )
#define  E_RGA_STATE_CLEAR_2  ( 0x0C730002 )
#define  E_RGA_STATE_CLEAR_3  ( 0x0C730003 )

/* Sub class of E_LIMITATION */
#define  E_RGA_LIMITATION_1   ( 0x0F730001 )
#define  E_RGA_LIMITATION_2   ( 0x0F730002 )
#define  E_RGA_LIMITATION_3   ( 0x0F730003 )

/* Sub class of E_STATE */
#define  E_RGA_STATE_1        ( 0x10730001 )
#define  E_RGA_STATE_2        ( 0x10730002 )
#define  E_RGA_STATE_3        ( 0x10730003 )
#define  E_RGA_STATE_4        ( 0x10730004 )
#define  E_RGA_STATE_5        ( 0x10730005 )
#define  E_RGA_STATE_6        ( 0x10730006 )


/***********************************************************************
* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT
*    Error code that means Not suppported pixel format.
*
* Description:
*    This is same as old specification of "E_NOT_SUPPORTED_PIXEL_FORMAT".
************************************************************************/
/* Constant: E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_MASK
*    Mask for checking E_RGA_NOT_SUPPORTED_PIXEL_FORMAT. */
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT       ( 0x49730000 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_MASK  ( 0x7FFFFF00 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1     ( 0x49730001 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2     ( 0x49730002 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3     ( 0x49730003 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_4     ( 0x49730004 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_5     ( 0x49730005 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_6     ( 0x49730006 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_7     ( 0x49730007 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_8     ( 0x49730008 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_9     ( 0x49730009 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_A     ( 0x4973000A )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_B     ( 0x4973000B )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_C     ( 0x4973000C )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_D     ( 0x4973000D )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_E     ( 0x4973000E )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_F     ( 0x4973000F )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_10    ( 0x49730010 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_11    ( 0x49730011 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_12    ( 0x49730012 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_13    ( 0x49730013 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_14    ( 0x49730014 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_15    ( 0x49730015 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_16    ( 0x49730016 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_17    ( 0x49730017 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_18    ( 0x49730018 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_19    ( 0x49730019 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1A    ( 0x4973001A )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1B    ( 0x4973001B )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1C    ( 0x4973001C )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1D    ( 0x4973001D )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1E    ( 0x4973001E )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_1F    ( 0x4973001F )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_20    ( 0x49730020 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_21    ( 0x49730021 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_22    ( 0x49730022 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_23    ( 0x49730023 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_24    ( 0x49730024 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_25    ( 0x49730025 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_26    ( 0x49730026 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_27    ( 0x49730027 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_28    ( 0x49730028 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_29    ( 0x49730029 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2A    ( 0x4973002A )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2B    ( 0x4973002B )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2C    ( 0x4973002C )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2D    ( 0x4973002D )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2E    ( 0x4973002E )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_2F    ( 0x4973002F )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_30    ( 0x49730030 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_31    ( 0x49730031 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_32    ( 0x49730032 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_33    ( 0x49730033 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_34    ( 0x49730034 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_35    ( 0x49730035 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_36    ( 0x49730036 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_37    ( 0x49730037 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_38    ( 0x49730038 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_39    ( 0x49730039 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3A    ( 0x4973003A )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3B    ( 0x4973003B )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3C    ( 0x4973003C )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3D    ( 0x4973003D )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3E    ( 0x4973003E )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_3F    ( 0x4973003F )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_40    ( 0x49730040 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_41    ( 0x49730041 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_42    ( 0x49730042 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_43    ( 0x49730043 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_44    ( 0x49730044 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_45    ( 0x49730045 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_46    ( 0x49730046 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_47    ( 0x49730047 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_48    ( 0x49730048 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_49    ( 0x49730049 )
#define  E_RGA_NOT_SUPPORTED_PIXEL_FORMAT_4A    ( 0x4973004A )


/***********************************************************************
* Constant: E_RGA_NULL
*    Error code that means NULL check failed.
************************************************************************/
/* Constant: E_RGA_NULL_MASK
*    Mask for checking E_RGA_NULL. */
#define  E_RGA_NULL           ( 0x01732100 )
#define  E_RGA_NULL_MASK      ( 0x7FFFFF00 )
#define  E_RGA_NULL_1         ( 0x01732101 )
#define  E_RGA_NULL_2         ( 0x01732102 )
#define  E_RGA_NULL_3         ( 0x01732103 )
#define  E_RGA_NULL_4         ( 0x01732104 )
#define  E_RGA_NULL_5         ( 0x01732105 )
#define  E_RGA_NULL_6         ( 0x01732106 )
#define  E_RGA_NULL_7         ( 0x01732107 )
#define  E_RGA_NULL_8         ( 0x01732108 )
#define  E_RGA_NULL_9         ( 0x01732109 )
#define  E_RGA_NULL_A         ( 0x0173210A )
#define  E_RGA_NULL_B         ( 0x0173210B )
#define  E_RGA_NULL_C         ( 0x0173210C )
#define  E_RGA_NULL_D         ( 0x0173210D )
#define  E_RGA_NULL_E         ( 0x0173210E )
#define  E_RGA_NULL_F         ( 0x0173210F )
#define  E_RGA_NULL_10        ( 0x01732110 )
#define  E_RGA_NULL_11        ( 0x01732111 )
#define  E_RGA_NULL_12        ( 0x01732112 )
#define  E_RGA_NULL_13        ( 0x01732113 )
#define  E_RGA_NULL_14        ( 0x01732114 )
#define  E_RGA_NULL_15        ( 0x01732115 )
#define  E_RGA_NULL_16        ( 0x01732116 )
#define  E_RGA_NULL_17        ( 0x01732117 )
#define  E_RGA_NULL_18        ( 0x01732118 )
#define  E_RGA_NULL_19        ( 0x01732119 )
#define  E_RGA_NULL_20        ( 0x01732120 )
#define  E_RGA_NULL_21        ( 0x01732121 )
#define  E_RGA_NULL_22        ( 0x01732122 )
#define  E_RGA_NULL_23        ( 0x01732123 )
#define  E_RGA_NULL_24        ( 0x01732124 )
#define  E_RGA_NULL_25        ( 0x01732125 )
#define  E_RGA_NULL_26        ( 0x01732126 )
#define  E_RGA_NULL_27        ( 0x01732127 )
#define  E_RGA_NULL_28        ( 0x01732128 )


/***********************************************************************
* Constant: E_RGA_ALIGNMENT
*    Error code that means bad address alignment.
************************************************************************/
/* Constant: E_RGA_ALIGNMENT_MASK
*    Mask for checking E_RGA_ALIGNMENT. */
#define  E_RGA_ALIGNMENT      ( 0x01732200 )
#define  E_RGA_ALIGNMENT_MASK ( 0x7FFFFF00 )
#define  E_RGA_ALIGNMENT_1    ( 0x01732201 )
#define  E_RGA_ALIGNMENT_2    ( 0x01732202 )
#define  E_RGA_ALIGNMENT_3    ( 0x01732203 )
#define  E_RGA_ALIGNMENT_4    ( 0x01732204 )
#define  E_RGA_ALIGNMENT_5    ( 0x01732205 )
#define  E_RGA_ALIGNMENT_6    ( 0x01732206 )
#define  E_RGA_ALIGNMENT_7    ( 0x01732207 )
#define  E_RGA_ALIGNMENT_8    ( 0x01732208 )
#define  E_RGA_ALIGNMENT_9    ( 0x01732209 )
#define  E_RGA_ALIGNMENT_A    ( 0x0173220A )
#define  E_RGA_ALIGNMENT_B    ( 0x0173220B )
#define  E_RGA_ALIGNMENT_C    ( 0x0173220C )
#define  E_RGA_ALIGNMENT_D    ( 0x0173220D )
#define  E_RGA_ALIGNMENT_E    ( 0x0173220E )
#define  E_RGA_ALIGNMENT_F    ( 0x0173220F )
#define  E_RGA_ALIGNMENT_10   ( 0x01732210 )
#define  E_RGA_ALIGNMENT_11   ( 0x01732211 )
#define  E_RGA_ALIGNMENT_12   ( 0x01732212 )
#define  E_RGA_ALIGNMENT_13   ( 0x01732213 )
#define  E_RGA_ALIGNMENT_14   ( 0x01732214 )
#define  E_RGA_ALIGNMENT_15   ( 0x01732215 )
#define  E_RGA_ALIGNMENT_16   ( 0x01732216 )
#define  E_RGA_ALIGNMENT_17   ( 0x01732217 )
#define  E_RGA_ALIGNMENT_18   ( 0x01732218 )
#define  E_RGA_ALIGNMENT_19   ( 0x01732219 )
#define  E_RGA_ALIGNMENT_1A   ( 0x0173221A )
#define  E_RGA_ALIGNMENT_1B   ( 0x0173221B )
#define  E_RGA_ALIGNMENT_1C   ( 0x0173221C )
#define  E_RGA_ALIGNMENT_1D   ( 0x0173221D )
#define  E_RGA_ALIGNMENT_1E   ( 0x0173221E )
#define  E_RGA_ALIGNMENT_1F   ( 0x0173221F )
#define  E_RGA_ALIGNMENT_20   ( 0x01732220 )
#define  E_RGA_ALIGNMENT_21   ( 0x01732221 )
#define  E_RGA_ALIGNMENT_22   ( 0x01732222 )
#define  E_RGA_ALIGNMENT_23   ( 0x01732223 )
#define  E_RGA_ALIGNMENT_24   ( 0x01732224 )
#define  E_RGA_ALIGNMENT_25   ( 0x01732225 )
#define  E_RGA_ALIGNMENT_26   ( 0x01732226 )
#define  E_RGA_ALIGNMENT_27   ( 0x01732227 )
#define  E_RGA_ALIGNMENT_28   ( 0x01732228 )
#define  E_RGA_ALIGNMENT_29   ( 0x01732229 )
#define  E_RGA_ALIGNMENT_2A   ( 0x0173222A )
#define  E_RGA_ALIGNMENT_2B   ( 0x0173222B )


/***********************************************************************
* Constant: E_RGA_TOO_BIG
*    Error code that means too big number.
************************************************************************/
/* Constant: E_RGA_TOO_BIG_MASK
*    Mask for checking E_RGA_TOO_BIG. */
#define  E_RGA_TOO_BIG        ( 0x01732300 )
#define  E_RGA_TOO_BIG_MASK   ( 0x7FFFFF00 )
#define  E_RGA_TOO_BIG_1      ( 0x01732301 )
#define  E_RGA_TOO_BIG_2      ( 0x01732302 )
#define  E_RGA_TOO_BIG_3      ( 0x01732303 )
#define  E_RGA_TOO_BIG_4      ( 0x01732304 )
#define  E_RGA_TOO_BIG_5      ( 0x01732305 )
#define  E_RGA_TOO_BIG_6      ( 0x01732306 )
#define  E_RGA_TOO_BIG_7      ( 0x01732307 )
#define  E_RGA_TOO_BIG_8      ( 0x01732308 )


/***********************************************************************
* Constant: E_RGA_EFFECT
*    Error code that means .
************************************************************************/
/* Constant: E_RGA_EFFECT_MASK
*    Mask for checking E_RGA_EFFECT. */
#define  E_RGA_EFFECT         ( 0x01732400 )
#define  E_RGA_EFFECT_MASK    ( 0x7FFFFF00 )
#define  E_RGA_EFFECT_1       ( 0x01732401 )
#define  E_RGA_EFFECT_2       ( 0x01732402 )


/***********************************************************************
* Constant: E_RGA_MATRIX
*    Error code that means .
************************************************************************/
/* Constant: E_RGA_MATRIX_MASK
*    Mask for checking E_RGA_MATRIX. */
#define  E_RGA_MATRIX         ( 0x01732500 )
#define  E_RGA_MATRIX_MASK    ( 0x7FFFFF00 )
#define  E_RGA_MATRIX_1       ( 0x01732501 )


/***********************************************************************
* Constant: E_RGA_OVERLAP_BLT
*    Error code that means bitblt overlap.
************************************************************************/
/* Constant: E_RGA_OVERLAP_BLT_MASK
*    Mask for checking E_RGA_OVERLAP_BLT. */
#define  E_RGA_OVERLAP_BLT      ( 0x01732600 )
#define  E_RGA_OVERLAP_BLT_MASK ( 0x7FFFFF00 )
#define  E_RGA_OVERLAP_BLT_1    ( 0x01732601 )
#define  E_RGA_OVERLAP_BLT_2    ( 0x01732602 )


/***********************************************************************
* Constant: E_RGA_ZOOM
*    Error code that means zooming is not supported.
************************************************************************/
/* Constant: E_RGA_ZOOM_MASK
*    Mask for checking E_RGA_ZOOM. */
#define  E_RGA_ZOOM             ( 0x01732700 )
#define  E_RGA_ZOOM_MASK        ( 0x7FFFFF00 )
#define  E_RGA_ZOOM_1           ( 0x01732701 )
#define  E_RGA_ZOOM_2           ( 0x01732702 )
#define  E_RGA_ZOOM_3           ( 0x01732703 )


/***********************************************************************
* Constant: E_RGA_NOT_REGULAR_MATRIX
*    Error code that means not regular matrix.
************************************************************************/
/* Constant: E_RGA_NOT_REGULAR_MATRIX_MASK
*    Mask for checking E_RGA_NOT_REGULAR_MATRIX. */
#define  E_RGA_NOT_REGULAR_MATRIX       ( 0x01732800 )
#define  E_RGA_NOT_REGULAR_MATRIX_MASK  ( 0x7FFFFF00 )
#define  E_RGA_NOT_REGULAR_MATRIX_1     ( 0x01732801 )
#define  E_RGA_NOT_REGULAR_MATRIX_2     ( 0x01732802 )


/***********************************************************************
* Constant: E_RGA_3X3MATRIX
*    Error code that means not supported 3X3 matrix.
************************************************************************/
/* Constant: E_RGA_3X3MATRIX_MASK
*    Mask for checking E_RGA_3X3MATRIX. */
#define  E_RGA_3X3MATRIX        ( 0x01732900 )
#define  E_RGA_3X3MATRIX_MASK   ( 0x7FFFFF00 )
#define  E_RGA_3X3MATRIX_1      ( 0x01732901 )
#define  E_RGA_3X3MATRIX_2      ( 0x01732902 )
#define  E_RGA_3X3MATRIX_3      ( 0x01732903 )
#define  E_RGA_3X3MATRIX_4      ( 0x01732904 )
#define  E_RGA_3X3MATRIX_5      ( 0x01732905 )
#define  E_RGA_3X3MATRIX_6      ( 0x01732906 )


/***********************************************************************
* Constant: E_RGA_RGAS
*    Error code of RGAS.
************************************************************************/
/* Constant: E_RGA_RGAS_MASK
*    Mask for checking E_RGA_RGAS. */
#define  E_RGA_RGAS             ( 0x01738000 )
#define  E_RGA_RGAS_MASK        ( 0x7FFFFF00 )
#define  E_RGA_RGAS_PARAM       ( 0x01738001 )
#define  E_RGA_RGAS_NOT_INIT    ( 0x01738002 )
#define  E_RGA_RGAS_INIT_TWICE  ( 0x01738003 )
#define  E_RGA_RGAS_STACK       ( 0x01738004 )


/***********************************************************************
* Constant: E_RGA_PNG
*    Error code of PNG in RGA.
************************************************************************/
/* Constant: E_RGA_PNG_MASK
*    Mask for checking E_RGA_PNG. */
#define  E_RGA_PNG       ( 0x01736000 )
#define  E_RGA_PNG_MASK  ( 0x7FFFFF00 )
#define  E_RGA_PNG_1     ( 0x01736001 )
#define  E_RGA_PNG_2     ( 0x01736002 )
#define  E_RGA_PNG_3     ( 0x01736003 )
#define  E_RGA_PNG_4     ( 0x01736004 )

/* Sub class of E_FEW_MEMORY */
#define  E_RGA_PNG_FEW_MEMORY_1  ( 0x03736001 )


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

#endif /* RGA_ERRNUM_H */
