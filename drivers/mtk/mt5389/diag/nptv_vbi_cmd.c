/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nptv_vbi_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"

#include "source_table.h"
#include "sv_const.h"
#include "drv_vbi.h"
#include "hw_vbi.h"
/* For Cache and DRAM consistence */
#include "x_hal_arm.h"

// Local enum & structures
//State machine for TTX Parser 
typedef enum
{
    E_Dump_Disable = 0, 
    E_Dump_Init, 
    E_Dump_Start,
    E_Dump_Stop,
}TTX_Parse_State_T;

//State machine for TTX Parser 
typedef enum
{
    E_Pkt_Header = 0, 
    E_Pkt_Normal, 
    E_Pkt_Enhanned,
    E_Pkt_27_Link,
    E_Pkt_ERR,
}TTX_Pkt_Type_T;

typedef enum 
{
    E_Ham_NoErr = 0,   //No error.
    E_Ham_Correct = 1, //1 Bit error, Can be Corrected. 
    E_Ham_Error = 2,   // > 2 Bit Error, can't be corrected. 
}HAM_Check_Rst;

typedef enum
{
    E_Mon_UnKnow = 0,
    E_Mon_Register ,
    E_Mon_GD_Status ,
}Mon_VBI_Type;


typedef enum
{
    E_CC_NO_Check = 0,
    E_CC_CC1_Check,
    E_CC_CC2_Check,
    E_CC_CC3_Check,
    E_CC_CC4_Check,
    E_CC_Internal_Check,
    E_VChip_Check,
    E_CC_IC_Check,
    E_CC_NO_Print,
    E_CC_TG39_Test,
    
}CC_Parse_Type;


typedef enum
{
    E_CC_CHK_Disable = 0, 
    E_CC_CHK_Init, 
    E_CC_CHK_Start,
    E_CC_CHK_Stop,
    
}CC_Chk_State_T;

typedef enum
{
    E_CC_UnKnow, 
    E_CC_AZ, 
    E_CC_az , 
    E_CC_Symbol , 
    E_CC_SpecChar, 
    E_CC_Num, 
}CC_Chk_Type_T;



typedef enum
{
    E_CC_TG39_UnKnow, 
    E_CC_Title1, 
    E_CC_Title2, 
    E_CC_Row1,
    E_CC_Row2, 
    E_CC_Row3, 
    E_CC_Row4, 
    E_CC_Row5, 
    E_CC_Row6, 
    E_CC_Row7, 
    E_CC_Row8, 
    E_CC_Row9, 
    E_CC_Row10, 
    E_CC_Row11, 
    E_CC_Row12, 
    E_CC_Row13, 
    E_CC_Row14, 
    E_CC_Row15, 
    E_CC_Row1_Indent,
    E_CC_Row3_Indent,
    E_CC_BackSpace, 
    E_CC_Flash, 
    E_CC_ParityChk, 
    E_CC_POPUL, 
    E_CC_POPUR, 
    E_CC_POPDR, 
    E_CC_POPDL, 
    E_CC_RowUp, 
    E_CC_Special_Title, 
    E_CC_Special_Content, 
    E_CC_TG39_CHK_MAX
}CC_TG39_Chk_Type_T;



typedef struct _ParseCCInfo_T
{
    CC_Parse_Type CCParseType;
    BOOL bIsFirstCheck;
    BOOL bEnVChip;
    UINT8 u1TestCnt;
    UINT8 u1FailCnt;
    
}ParseCCInfo_T;

typedef struct _ParseTTXInfo_T
{
    UINT8 u1MagNum;
    UINT8 u1PAGTen; 
    UINT8 u1PAGUint;
    BOOL bEnDumpPage;
    BOOL bEnDumpNICNI;
    BOOL bEnChkHam84_B01; //Chk Ham84 for first 2 bytes for all packet
    BOOL bEnChkHam84_B29; //Chk Ham84 byte 2~9 for Page header 
    BOOL bEnChkParity;
    BOOL bEnCharDisplay;
    BOOL bEnPaketCount;
    BOOL bEnChkQlabPage;
    UINT8 u1PagePktCnt;
    UINT8 u1FailPktCnt;
    UINT8 u1ChkTTXMode;
    UINT16 u2SubPage; //TODO ?? dump Subpage ?? 
}ParseTTXInfo_T;

typedef struct _MonVBIInfo_T
{
    Mon_VBI_Type MonType;
    UINT32 u4RegisterAddr;
    UINT8 u1MonSpeed;
}MonVBIInfo_T;

typedef enum _VChipType
{
    E_MPA = 0,
    E_US_TV,
    E_Can_English,
    E_Can_French,
    E_Reserved,
}VChipType;

typedef enum _ScanLState_T
{
    E_Init = 0, 
    E_ScanNextL,
    E_WaitScan,
    E_Finish,
}ScanLState_T;



/* Local Define */
#ifndef CC_CLI
#define CC_CLI
#endif

#define ChkTTXPageData 1
#define ChkCCInternalData 1
#define ChkCCInTG39 1


#define BYTE_IVS(x) (HEX_REVERSE_TABLE2[x>>4] | (HEX_REVERSE_TABLE2[x&0xF]<<4))

#ifdef CC_CLI
/* Functions for Teletext */
static INT32 _i4TTXSlicer(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4CCSlicer(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4TTX_VPS_CNI(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4TTX_WSS(INT32 i4Argc, const CHAR **szArgv);
#endif /*#ifdef CC_CLI*/

/* Functions for VBI */
#if VBI_CLI_SUPPORT
static INT32 _i4VBI_EnableDramDump(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_SetVbiMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_CoreRst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_DbgPort(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_LineEn(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_LSC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_ZC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_FIFO(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_OutputMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_Source(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_CcLine(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_WssLine(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_TtxLine(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VpsLine(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VerCC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VerCC2X(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VerUsWSS(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VerEuWSS(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VerVPS(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VerTTX(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_SetDumpTTXPage(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_SetDumpNICNI(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_VBIRegDump(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_Mon_VBI_Status(INT32 i4Argc, const CHAR **szArgv);
#if ChkTTXPageData
static INT32 _i4VBI_CheckTTXPage(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _i4VBI_CheckCCData(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_ScanVBILine(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_ScanRange(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4VBI_AdjGain(INT32 i4Argc, const CHAR **szArgv);
#if AUTO_CC_DETECT
static INT32 _i4VBI_AutoCCOnOff(INT32 i4Argc, const CHAR **szArgv);
#endif

#endif

// Local Variables
static ParseTTXInfo_T _ParseTTX_Info = {0};
static HAM_Check_Rst _HamChk_Level = E_Ham_Correct;
static MonVBIInfo_T  _MonVbiInfo;
static ParseCCInfo_T _ParseCC_Info;
static TTX_Parse_State_T TTXParseState = E_Dump_Init;


UINT8 HEX_REVERSE_TABLE2[16] =
{
	0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
};


#if ChkTTXPageData

static UINT8 _Ver_Pkt1_23[40] = 
{
    0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,
    0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,
    0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,
    0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f,0xfe,0x7f
};

static UINT8 _Ver_QLab_Pkt0[] = 
{
    0x01,0x02,0x00,0x08,0x00,0x00,0x00,0x01,0x20,0x54,
    0x45,0x58,0x54,0x9e,0x31,0x32,0x31,0x91,0x7f,0x13,
    0x7f,0x92,0x7f,0x16,0x7f,0x94,0x7f,0x15,0x7f,0x83,
    0x20,0x20
};

static UINT8 _Ver_Pkt0[40] = 
{
    0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x20,0x20,
    0x20,0x43,0xae,0xc4,0xae,0xd5,0xae,0x86,0x54,0x45,
    0xd3,0x54,0x20,0xd0,0xc1,0xc7,0x45,0x20,0x32,0x20,
    0x20,0x02,0x20,0x31,0xb0,0xb0,0x20,0x20,0x20,0x20
};

#endif 


#if ChkCCInternalData
//A~Z

static UINT8 _Ver_CC1_LineAZ[52] = 
{
    0x94 ,0x2c,  // Index 00 <--
    0x94 ,0x2c,  // Index 02 <-- Erase Displayed memory
    0x94 ,0xae,  // Index 04 <-- 
    0x94 ,0xae,  // Index 06 <-- Erase Non-Displayed memory
    0x94 ,0x2f,  // Index 08 <--
    0x94 ,0x2f,  // Index 10 <-- End - of Caption
    0x94 ,0xa7,  // Index 12 <--
    0x94 ,0xa7,  // Index 14 <-- Row Up Captions 4 Rows
    0x13 ,0x46,  // Index 16 <--
    0x13 ,0x46,  // Index 18 <-- Row 12: Cyan color 
    0x10 ,0xa4,  // Index 20 <--
    0x10 ,0xa4,  // Index 22 <-- BackGround Blue
    0x20 ,0xc1,  // Index 24 <--  ,A
    0xc2 ,0x43,  // Index 26 <-- B,c
    0xc4 ,0x45,  // Index 28 <-- D,E
    0x46 ,0xc7,  // Index 30 <-- F,G
    0xc8 ,0x49,  // Index 32 <-- H,I
    0x4a ,0xcb,  // Index 34 <-- J,K
    0x4c ,0xcd,  // Index 34 <-- L,M
    0xce ,0x4f,  // Index 38 <-- N,O
    0xd0 ,0x51,  // Index 40 <-- P,Q
    0x52 ,0xd3,  // Index 42 <-- R,S
    0x54 ,0xd5,  // Index 44 <-- T,U
    0xd6 ,0x57,  // Index 46 <-- V,W
    0x58 ,0xd9,  // Index 48 <-- X,Y
    0xda ,0x20,  // Index 50 <-- Z, 
};

//a~z
static UINT8 _Ver_CC1_Lineaz[36] = 
{
    0x13 ,0xce, // Index 00 <--
    0x13 ,0xce, // Index 02 <-- Row 12: White Itaics
    0x10 ,0x25, // Index 04 <--
    0x10 ,0x25, // Index 06 <-- BackGround , Semi-Bule
    0x20 ,0x61, // Index 08 <--  ,a
    0x62 ,0xe3, // Index 10 <-- b,c
    0x64 ,0xe5, // Index 12 <-- d,e
    0xe6 ,0x67, // Index 14 <-- f,g
    0x68 ,0xe9, // Index 16 <-- h,i
    0xea ,0x6b, // Index 18 <-- j,k
    0xec ,0x6d, // Index 20 <-- l,m
    0x6e ,0xef, // Index 22 <-- n,o
    0x70 ,0xf1, // Index 24 <-- p,q
    0xf2 ,0x73, // Index 26 <-- r,s
    0xf4 ,0x75, // Index 28 <-- t,u
    0x76 ,0xf7, // Index 30 <-- v,w
    0xf8 ,0x79, // Index 32 <-- x,y
    0x7a ,0x20, // Index 34 <-- z, 
};

//Symbol
static UINT8 _Ver_CC1_LineSymbol[40] = 
{
    0x13 ,0x45,    // Index 00
    0x13 ,0x45,    // Index 02< -- Row 12 Blue Underline
    0x10 ,0x20,    // Index 04
    0x10 ,0x20,    // Index 06< -- Background Attribute White
    0x20 ,0xa1,    // Index 08< --   !
    0x2c ,0xae,    // Index 10< -- , .
    0x3b ,0xba,    // Index 12< -- ; :
    0xa7 ,0xa2,    // Index 14< -- ' "
    0x23 ,0x25,    // Index 16< -- # %
    0x26 ,0x40,    // Index 18< -- & @
    0x2f ,0xa8,    // Index 20< -- / (
    0x29 ,0x5b,    // Index 22< -- ) [
    0x5d ,0xab,    // Index 24< -- ] +
    0xad ,0x7c,    // Index 26< -- - /(devide)
    0xbc ,0x3d,    // Index 28< -- < =
    0x3e ,0xbf,    // Index 30< -- > ?
    0x91 ,0xb5,    // Index 32< -- Cents sign (c+/)                       : special caharcters  
    0xa4 ,0x80,    // Index 34< -- $ 
    0x91 ,0xb6,    // Index 36< -- Pounds Sterling sign (L+-)             : special characters 
    0x20 ,0x80,    // Index 38< --   

};

// Special Character
UINT8 _Ver_CC1_SecialChar[54] = 
{
    0x13 ,0xc8 ,    // Index 00 <
    0x13 ,0xc8 ,    // Index 02 <-- Row 12 Red
    0x10 ,0xa1 ,    // Index 04 <--
    0x10 ,0xa1 ,    // Index 06 <-- Backgound White, Semi-Transparent
    0x20 ,0x2a ,    // Index 08 <-- Space ,Lower case a with acute accent 
    0x91 ,0x38 ,    // Index 10 <-- Lower case a with grave accent                                      : Special Character.
    0x91 ,0x3b ,    // Index 12 <-- Lower case a with circumflex                                        : Special Character. 
    0xfb ,0xdc ,    // Index 14 <-- Lower case c with cedilla , Lower-case e with acute accent(¥ªºJ¤W¼Ð)
    0x91 ,0xba ,    // Index 16 <-- Lower case e with grave accent                                      : Special Character. 
    0x91 ,0xbc ,    // Index 18 <-- Lower case e with circumflex                                        : Special Character. 
    0x5e ,0x80 ,    // Index 20 <-- Lower case i with acute accent  
    0x91 ,0x3d ,    // Index 22 <-- Lower case i with circumflex                                        : Special Character. 
    0xfd ,0xfe ,    // Index 24 <-- Upper case N with Tilde, Lower case n with Tilde ,
    0x91 ,0x3e ,    // Index 26 <-- Lower case o with circumflex                                        : Special Character.  
    0xdf ,0x80 ,    // Index 28 <-- Lower case o with acute accent  
    0x91 ,0xbf ,    // Index 30 <-- Lower case u with circumflex                                        : Special Character.
    0xe0 ,0x20 ,    // Index 32 <-- Lower case u with acute accent
    0x91 ,0xb0 ,    // Index 34 <-- Registerd mark symbol (R+O)                                         : Special Character.
    0x91 ,0x31 ,    // Index 36 <-- Degree Sign (o)                                                     : Special Character.  
    0x91 ,0x34 ,    // Index 38 <-- Trademark symbol (TM)                                               : Special Character. 
    0x91 ,0x32 ,    // Index 40 <-- 1/2                                                                 : Special Character. 
    0x91 ,0xb3 ,    // Index 42 <-- Inverse query                                                       : Special Character. 
    0x91 ,0x37 ,    // Index 44 <-- Music note                                                          : Special Character. 
    0x20 ,0x80 ,    // Index 46 <-- Space
    0x91 ,0xb9 ,    // Index 48 <-- Transparent Space                                                   : Special Character. 
    0x20 ,0x7f ,    // Index 50 <-- Space , Solid block
    0x20 ,0x80 ,    // Index 52 <-- Space
};

static UINT8 _Ver_CC1_Number[] = 
{
    0x13,0x4c ,   // Index 00 <--
    0x13,0x4c ,   // Index 02 <-- Row 12 Magenta
    0x20,0x80 ,   // Index 04 <-- 
    0x10,0xa8 ,   // Index 06 <--
    0x10,0xa8 ,   // Index 08 <-- Background Red.
    0x20,0x80 ,   // Index 10 <-- 
    0x97,0xae ,   // Index 12 <--
    0x97,0xae ,   // Index 14 <-- Foreground black. 
    0x97,0x25 ,   // Index 16 <--
    0x97,0x25 ,   // Index 18 <-- Standard line 21, Double size characters. 
    0x20,0x20 ,   // Index 20 <--  
    0x20,0xb0 ,   // Index 22 <--   , 0
    0x31,0x32 ,   // Index 24 <-- 1 , 2
    0xb3,0x34 ,   // Index 26 <-- 3 , 4
    0xb5,0xb6 ,   // Index 28 <-- 5 , 6
    0x37,0x38 ,   // Index 30 <-- 7 , 8
    0xb9,0x20 ,   // Index 32 <-- 9
    0x20,0x80 ,   // Index 34
};
#endif

#if ChkCCInTG39

static UINT8 _Ver_TG39_Title1[] = 
{
    0x91,0x40, // [0x11] [0x40]:Preamble Addr Codes
    0x54,0xc7, // [0x54] [0x47]:Char T ,G
    0xb3,0xb9, // [0x33] [0x39]:Char 3 ,9
    0xc1,0x43, // [0x41] [0x43]:Char A ,C
    0x20,0x20, // [0x20] [0x20]:Char   ,
    0x43,0x4c, // [0x43] [0x4c]:Char C ,L
    0x4f,0xd3, // [0x4f] [0x53]:Char O ,S
    0x45,0xc4, // [0x45] [0x44]:Char E ,D
    0x20,0x20, // [0x20] [0x20]:Char   ,
    0x43,0xc1, // [0x43] [0x41]:Char C ,A
    0xd0,0x54, // [0x50] [0x54]:Char P ,T
    0x49,0x4f, // [0x49] [0x4f]:Char I ,O
    0xce,0x80, // [0x4e] [0x00]:Char N ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Title2[] = 
{
    0x91,0xe0, // [0x11] [0x60]:Preamble Addr Codes
    0x43,0xc1, // [0x43] [0x41]:Char C ,A
    0xd0,0x54, // [0x50] [0x54]:Char P ,T
    0x49,0x4f, // [0x49] [0x4f]:Char I ,O
    0xce,0x20, // [0x4e] [0x20]:Char N ,
    0xc4,0xc1, // [0x44] [0x41]:Char D ,A
    0x54,0xc1, // [0x54] [0x41]:Char T ,A
    0x20,0x31, // [0x20] [0x31]:Char   ,1
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row1[] = 
{
    0x91,0x40, // [0x11] [0x40]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0x31,0x20, // [0x31] [0x20]:Char 1 ,
    0x57,0x68, // [0x57] [0x68]:Char W ,h
    0xe9,0xf4, // [0x69] [0x74]:Char i ,t
    0xe5,0x20, // [0x65] [0x20]:Char e ,
    0x43,0x68, // [0x43] [0x68]:Char C ,h
    0x61,0xf2, // [0x61] [0x72]:Char a ,r
    0x61,0xe3, // [0x61] [0x63]:Char a ,c
    0xf4,0xe5, // [0x74] [0x65]:Char t ,e
    0xf2,0x20, // [0x72] [0x20]:Char r ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_Row2[] = 
{
    0x91,0x61, // [0x11] [0x61]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0x32,0x20, // [0x32] [0x20]:Char 2 ,
    0x57,0x68, // [0x57] [0x68]:Char W ,h
    0xe9,0xf4, // [0x69] [0x74]:Char i ,t
    0xe5,0x20, // [0x65] [0x20]:Char e ,
    0xd5,0x6e, // [0x55] [0x6e]:Char U ,n
    0x64,0xe5, // [0x64] [0x65]:Char d ,e
    0xf2,0xec, // [0x72] [0x6c]:Char r ,l
    0xe9,0x6e, // [0x69] [0x6e]:Char i ,n
    0xe5,0x20, // [0x65] [0x20]:Char e ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_Row3[] = 
{
    0x92,0xc2, // [0x12] [0x42]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0xb3,0x20, // [0x33] [0x20]:Char 3 ,
    0xc7,0xf2, // [0x47] [0x72]:Char G ,r
    0xe5,0xe5, // [0x65] [0x65]:Char e ,e
    0x6e,0x20, // [0x6e] [0x20]:Char n ,
    0x43,0x68, // [0x43] [0x68]:Char C ,h
    0x61,0xf2, // [0x61] [0x72]:Char a ,r
    0x61,0xe3, // [0x61] [0x63]:Char a ,c
    0xf4,0xe5, // [0x74] [0x65]:Char t ,e
    0xf2,0x20, // [0x72] [0x20]:Char r ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};



static UINT8 _Ver_TG39_Row4[] = 
{
    0x92,0xe3, // [0x12] [0x63]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0x34,0x20, // [0x34] [0x20]:Char 4 ,
    0xc7,0xf2, // [0x47] [0x72]:Char G ,r
    0xe5,0xe5, // [0x65] [0x65]:Char e ,e
    0x6e,0x20, // [0x6e] [0x20]:Char n ,
    0xd5,0x6e, // [0x55] [0x6e]:Char U ,n
    0x64,0xe5, // [0x64] [0x65]:Char d ,e
    0xf2,0xec, // [0x72] [0x6c]:Char r ,l
    0xe9,0x6e, // [0x69] [0x6e]:Char i ,n
    0xe5,0x20, // [0x65] [0x20]:Char e ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_Row5[] = 
{
    0x15,0xc4, // [0x15] [0x44]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0xb5,0x20, // [0x35] [0x20]:Char 5 ,
    0xc2,0xec, // [0x42] [0x6c]:Char B ,l
    0x75,0xe5, // [0x75] [0x65]:Char u ,e
    0x20,0x43, // [0x20] [0x43]:Char   ,C
    0x68,0x61, // [0x68] [0x61]:Char h ,a
    0xf2,0x61, // [0x72] [0x61]:Char r ,a
    0xe3,0xf4, // [0x63] [0x74]:Char c ,t
    0xe5,0xf2, // [0x65] [0x72]:Char e ,r
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row6[] = 
{
    0x15,0xe5, // [0x15] [0x65]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0xb6,0x20, // [0x36] [0x20]:Char 6 ,
    0xc2,0xec, // [0x42] [0x6c]:Char B ,l
    0x75,0xe5, // [0x75] [0x65]:Char u ,e
    0x20,0xd5, // [0x20] [0x55]:Char   ,U
    0x6e,0x64, // [0x6e] [0x64]:Char n ,d
    0xe5,0xf2, // [0x65] [0x72]:Char e ,r
    0xec,0xe9, // [0x6c] [0x69]:Char l ,i
    0x6e,0xe5, // [0x6e] [0x65]:Char n ,e
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Code
};


static UINT8 _Ver_TG39_Row7[] = 
{
    0x16,0x46, // [0x16] [0x46]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0x37,0x20, // [0x37] [0x20]:Char 7 ,
    0x43,0x79, // [0x43] [0x79]:Char C ,y
    0x61,0x6e, // [0x61] [0x6e]:Char a ,n
    0x20,0x43, // [0x20] [0x43]:Char   ,C
    0x68,0x61, // [0x68] [0x61]:Char h ,a
    0xf2,0x61, // [0x72] [0x61]:Char r ,a
    0xe3,0xf4, // [0x63] [0x74]:Char c ,t
    0xe5,0xf2, // [0x65] [0x72]:Char e ,r
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row8[] = 
{
    0x16,0x67, // [0x16] [0x67]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0x38,0x20, // [0x38] [0x20]:Char 8 ,
    0x43,0x79, // [0x43] [0x79]:Char C ,y
    0x61,0x6e, // [0x61] [0x6e]:Char a ,n
    0x20,0xd5, // [0x20] [0x55]:Char   ,U
    0x6e,0x64, // [0x6e] [0x64]:Char n ,d
    0xe5,0xf2, // [0x65] [0x72]:Char e ,r
    0xec,0xe9, // [0x6c] [0x69]:Char l ,i
    0x6e,0xe5, // [0x6e] [0x65]:Char n ,e
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row9[] = 
{
    0x97,0xc8, // [0x17] [0x48]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0xb9,0x20, // [0x39] [0x20]:Char 9 ,
    0x52,0xe5, // [0x52] [0x65]:Char R ,e
    0x64,0x20, // [0x64] [0x20]:Char d ,
    0x43,0x68, // [0x43] [0x68]:Char C ,h
    0x61,0xf2, // [0x61] [0x72]:Char a ,r
    0x61,0xe3, // [0x61] [0x63]:Char a ,c
    0xf4,0xe5, // [0x74] [0x65]:Char t ,e
    0xf2,0x20, // [0x72] [0x20]:Char r ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row10[] = 
{
    0x97,0xe9, // [0x17] [0x69]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x31, // [0x77] [0x31]:Char w ,1
    0xb0,0x20, // [0x30] [0x20]:Char 0 ,
    0x52,0xe5, // [0x52] [0x65]:Char R ,e
    0x64,0x20, // [0x64] [0x20]:Char d ,
    0xd5,0x6e, // [0x55] [0x6e]:Char U ,n
    0x64,0xe5, // [0x64] [0x65]:Char d ,e
    0xf2,0xec, // [0x72] [0x6c]:Char r ,l
    0xe9,0x6e, // [0x69] [0x6e]:Char i ,n
    0xe5,0x20, // [0x65] [0x20]:Char e ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row11[] = 
{
    0x10,0x4a, // [0x10] [0x4a]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x31, // [0x77] [0x31]:Char w ,1
    0x31,0x20, // [0x31] [0x20]:Char 1 ,
    0xd9,0xe5, // [0x59] [0x65]:Char Y ,e
    0xec,0xec, // [0x6c] [0x6c]:Char l ,l
    0xef,0xf7, // [0x6f] [0x77]:Char o ,w
    0x20,0x43, // [0x20] [0x43]:Char   ,C
    0x68,0x61, // [0x68] [0x61]:Char h ,a
    0xf2,0x61, // [0x72] [0x61]:Char r ,a
    0xe3,0xf4, // [0x63] [0x74]:Char c ,t
    0xe5,0xf2, // [0x65] [0x72]:Char e ,r
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row12[] = 
{
    0x13,0xcb, // [0x13] [0x4b]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x31, // [0x77] [0x31]:Char w ,1
    0x32,0x20, // [0x32] [0x20]:Char 2 ,
    0xd9,0xe5, // [0x59] [0x65]:Char Y ,e
    0xec,0xec, // [0x6c] [0x6c]:Char l ,l
    0xef,0xf7, // [0x6f] [0x77]:Char o ,w
    0x20,0xd5, // [0x20] [0x55]:Char   ,U
    0x6e,0x64, // [0x6e] [0x64]:Char n ,d
    0xe5,0xf2, // [0x65] [0x72]:Char e ,r
    0xec,0xe9, // [0x6c] [0x69]:Char l ,i
    0x6e,0xe5, // [0x6e] [0x65]:Char n ,e
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row13[] =
{
    0x13,0xec, // [0x13] [0x6c]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x31, // [0x77] [0x31]:Char w ,1
    0xb3,0x20, // [0x33] [0x20]:Char 3 ,
    0xcd,0x61, // [0x4d] [0x61]:Char M ,a
    0x67,0xe5, // [0x67] [0x65]:Char g ,e
    0x6e,0xf4, // [0x6e] [0x74]:Char n ,t
    0x61,0x20, // [0x61] [0x20]:Char a ,
    0x43,0x68, // [0x43] [0x68]:Char C ,h
    0x61,0xf2, // [0x61] [0x72]:Char a ,r
    0x61,0xe3, // [0x61] [0x63]:Char a ,c
    0xf4,0xe5, // [0x74] [0x65]:Char t ,e
    0xf2,0x20, // [0x72] [0x20]:Char r ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row14[] =
{
    0x94,0xcd, // [0x14] [0x4d]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x31, // [0x77] [0x31]:Char w ,1
    0x34,0x20, // [0x34] [0x20]:Char 4 ,
    0xcd,0x61, // [0x4d] [0x61]:Char M ,a
    0x67,0xe5, // [0x67] [0x65]:Char g ,e
    0x6e,0xf4, // [0x6e] [0x74]:Char n ,t
    0x61,0x20, // [0x61] [0x20]:Char a ,
    0xd5,0x6e, // [0x55] [0x6e]:Char U ,n
    0x64,0xe5, // [0x64] [0x65]:Char d ,e
    0xf2,0xec, // [0x72] [0x6c]:Char r ,l
    0xe9,0x6e, // [0x69] [0x6e]:Char i ,n
    0xe5,0x20, // [0x65] [0x20]:Char e ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_Row15[] =
{
    0x91,0xae, // [0x11] [0x2e]:Mid Row Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x31, // [0x77] [0x31]:Char w ,1
    0xb5,0x20, // [0x35] [0x20]:Char 5 ,
    0xc7,0xf2, // [0x47] [0x72]:Char G ,r
    0xe5,0xe5, // [0x65] [0x65]:Char e ,e
    0x6e,0x20, // [0x6e] [0x20]:Char n ,
    0x49,0xf4, // [0x49] [0x74]:Char I ,t
    0x61,0xec, // [0x61] [0x6c]:Char a ,l
    0xe9,0xe3, // [0x69] [0x63]:Char i ,c
    0x73,0x20, // [0x73] [0x20]:Char s ,
    0x43,0x68, // [0x43] [0x68]:Char C ,h
    0x61,0xf2, // [0x61] [0x72]:Char a ,r
    0x61,0xe3, // [0x61] [0x63]:Char a ,c
    0xf4,0xe5, // [0x74] [0x65]:Char t ,e
    0xf2,0x20, // [0x72] [0x20]:Char r ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_Row1_Indent[] =
{
    0x91,0xd0, // [0x11] [0x50]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0x31,0x20, // [0x31] [0x20]:Char 1 ,
    0x49,0x6e, // [0x49] [0x6e]:Char I ,n
    0x64,0xe5, // [0x64] [0x65]:Char d ,e
    0x6e,0xf4, // [0x6e] [0x74]:Char n ,t
    0x20,0xb0, // [0x20] [0x30]:Char   ,0
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row3_Indent[] =
{
    0x92,0x52, // [0x12] [0x52]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x20, // [0x77] [0x20]:Char w ,
    0xb3,0x20, // [0x33] [0x20]:Char 3 ,
    0x49,0x6e, // [0x49] [0x6e]:Char I ,n
    0x64,0xe5, // [0x64] [0x65]:Char d ,e
    0x6e,0xf4, // [0x6e] [0x74]:Char n ,t
    0x20,0x34, // [0x20] [0x34]:Char   ,4
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_BackSpace[] =
{
    0x13,0x40, // [0x13] [0x40]:Preamble Addr Codes
    0xc2,0x61, // [0x42] [0x61]:Char B ,a
    0xe3,0x6b, // [0x63] [0x6b]:Char c ,k
    0x73,0x70, // [0x73] [0x70]:Char s ,p
    0x61,0xe3, // [0x61] [0x63]:Char a ,c
    0xe5,0x20, // [0x65] [0x20]:Char e ,
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xf7,0x31, // [0x77] [0x31]:Char w ,1
    0x32,0x20, // [0x32] [0x20]:Char 2 ,
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_Flash[] =
{
    0x94,0x40, // [0x14] [0x40]:Preamble Addr Codes
    0x46,0xec, // [0x46] [0x6c]:Char F ,l
    0x61,0x73, // [0x61] [0x73]:Char a ,s
    0x68,0x20, // [0x68] [0x20]:Char h ,
    0x4f,0xe6, // [0x4f] [0x66]:Char O ,f
    0xe6,0x20, // [0x66] [0x20]:Char f ,
    0x20,0x80, // [0x20] [0x00]:Char   ,Padding
    0x94,0xa8, // [0x14] [0x28]:Misc Ctrl Codes
    0x94,0xa8, // [0x14] [0x28]:Misc Ctrl Codes
    0x46,0xec, // [0x46] [0x6c]:Char F ,l
    0x61,0x73, // [0x61] [0x73]:Char a ,s
    0x68,0x20, // [0x68] [0x20]:Char h ,
    0x4f,0x6e, // [0x4f] [0x6e]:Char O ,n
    0x20,0x20, // [0x20] [0x20]:Char   ,
    0x91,0x20, // [0x11] [0x20]:Mid Row Codes
    0x91,0x20, // [0x11] [0x20]:Mid Row Codes
    0x46,0xec, // [0x46] [0x6c]:Char F ,l
    0x61,0x73, // [0x61] [0x73]:Char a ,s
    0x68,0x20, // [0x68] [0x20]:Char h ,
    0x4f,0xe6, // [0x4f] [0x66]:Char O ,f
    0xe6,0x80, // [0x66] [0x00]:Char f ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_ParityChk[] =
{
    0x94,0xe0, // [0x14] [0x60]:Preamble Addr Codes
    0xd0,0x61, // [0x50] [0x61]:Char P ,a
    0xf2,0xe9, // [0x72] [0x69]:Char r ,i
    0xf4,0x79, // [0x74] [0x79]:Char t ,y
    0x20,0x43, // [0x20] [0x43]:Char   ,C
    0x68,0xe5, // [0x68] [0x65]:Char h ,e
    0xe3,0x6b, // [0x63] [0x6b]:Char c ,k
    0x20,0x20, // [0x20] [0x20]:Char   ,
    0x20,0x20, // [0x20] [0x20]:Char   ,
    0xcd,0x49, // [0x4d] [0x49]:Char M ,I
    0xd3,0xd3, // [0x53] [0x53]:Char S ,S
    0x20,0x20, // [0x20] [0x20]:Char   ,
    //0x4d,0xc9, // Data 1 Parity Error Data 2 Parity Error!! Unknow Type
    //0x53,0x53, // Data 1 Parity Error Data 2 Parity Error!! Unknow Type
    //0x20,0x20, // [0x20] [0x20]:Char   ,
    //0x54,0xc1, // [0x54] [0x41]:Char T ,A
    //0xcb,0x45, // [0x4b] [0x45]:Char K ,E
    //0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    //0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_POPUL[] =
{
    0x91,0x40, // [0x11] [0x40]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x31,0x80, // [0x31] [0x00]:Char 1 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x91,0xe0, // [0x11] [0x60]:Preamble Addr Codes
    0x91,0xe0, // [0x11] [0x60]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x32,0x80, // [0x32] [0x00]:Char 2 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x92,0x40, // [0x12] [0x40]:Preamble Addr Codes
    0x92,0x40, // [0x12] [0x40]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0xb3,0x80, // [0x33] [0x00]:Char 3 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x92,0xe0, // [0x12] [0x60]:Preamble Addr Codes
    0x92,0xe0, // [0x12] [0x60]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x34,0x80, // [0x34] [0x00]:Char 4 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};



static UINT8 _Ver_TG39_POPUR[] =
{
    0x91,0xdc, // [0x11] [0x5c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x31,0x80, // [0x31] [0x00]:Char 1 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x91,0x7c, // [0x11] [0x7c]:Preamble Addr Codes
    0x91,0x7c, // [0x11] [0x7c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x32,0x80, // [0x32] [0x00]:Char 2 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x92,0xdc, // [0x12] [0x5c]:Preamble Addr Codes
    0x92,0xdc, // [0x12] [0x5c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0xb3,0x80, // [0x33] [0x00]:Char 3 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x92,0x7c, // [0x12] [0x7c]:Preamble Addr Codes
    0x92,0x7c, // [0x12] [0x7c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x34,0x80, // [0x34] [0x00]:Char 4 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0x2f, // [0x14] [0x2f]:Misc Ctrl Codes
    0x94,0x2f, // [0x14] [0x2f]:Misc Ctrl Codes
};


static UINT8 _Ver_TG39_POPDR[] =
{
    0x13,0xdc, // [0x13] [0x5c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x31,0x80, // [0x31] [0x00]:Char 1 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x13,0x7c, // [0x13] [0x7c]:Preamble Addr Codes
    0x13,0x7c, // [0x13] [0x7c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x32,0x80, // [0x32] [0x00]:Char 2 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xdc, // [0x14] [0x5c]:Preamble Addr Codes
    0x94,0xdc, // [0x14] [0x5c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0xb3,0x80, // [0x33] [0x00]:Char 3 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0x7c, // [0x14] [0x7c]:Preamble Addr Codes
    0x94,0x7c, // [0x14] [0x7c]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O>
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x34,0x80, // [0x34] [0x00]:Char 4 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0x2f, // [0x14] [0x2f]:Misc Ctrl Codes
    0x94,0x2f, // [0x14] [0x2f]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_POPDL[] =
{
    0x13,0x40, // [0x13] [0x40]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x31,0x80, // [0x31] [0x00]:Char 1 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x13,0xe0, // [0x13] [0x60]:Preamble Addr Codes
    0x13,0xe0, // [0x13] [0x60]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x32,0x80, // [0x32] [0x00]:Char 2 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0x40, // [0x14] [0x40]:Preamble Addr Codes
    0x94,0x40, // [0x14] [0x40]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0xb3,0x80, // [0x33] [0x00]:Char 3 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xe0, // [0x14] [0x60]:Preamble Addr Codes
    0x94,0xe0, // [0x14] [0x60]:Preamble Addr Codes
    0xd0,0x4f, // [0x50] [0x4f]:Char P ,O
    0xd0,0x20, // [0x50] [0x20]:Char P ,
    0x4f,0xce, // [0x4f] [0x4e]:Char O ,N
    0x34,0x80, // [0x34] [0x00]:Char 4 ,Padding
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0x2f, // [0x14] [0x2f]:Misc Ctrl Codes
    0x94,0x2f, // [0x14] [0x2f]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_RowUp[] =
{
    0x94,0xe0, // [0x14] [0x60]:Preamble Addr Codes
    0x52,0xef, // [0x52] [0x6f]:Char R ,o
    0xec,0xec, // [0x6c] [0x6c]:Char l ,l
    0xad,0x75, // [0x2d] [0x75]:Char - ,u
    0x70,0x20, // [0x70] [0x20]:Char p ,
    0x43,0x61, // [0x43] [0x61]:Char C ,a
    0x70,0xf4, // [0x70] [0x74]:Char p ,t
    0xe9,0xef, // [0x69] [0x6f]:Char i ,o
    0x6e,0x73, // [0x6e] [0x73]:Char n ,s
    0xad,0x34, // [0x2d] [0x34]:Char - ,4
    0x20,0x52, // [0x20] [0x52]:Char   ,R
    0xef,0xf7, // [0x6f] [0x77]:Char o ,w
    //0x31,0x32, // [0x31] [0x32]:Char 1 ,2
    //0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    //0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row1_Special_Title[] =
{
    0x91,0x40, // [0x11] [0x40]:Preamble Addr Codes
    0xd3,0xd0, // [0x53] [0x50]:Char S ,P
    0x45,0x43, // [0x45] [0x43]:Char E ,C
    0x49,0xc1, // [0x49] [0x41]:Char I ,A
    0x4c,0x20, // [0x4c] [0x20]:Char L ,
    0x43,0xc8, // [0x43] [0x48]:Char C ,H
    0xc1,0x52, // [0x41] [0x52]:Char A ,R
    0xc1,0x43, // [0x41] [0x43]:Char A ,C
    0x54,0x45, // [0x54] [0x45]:Char T ,E
    0x52,0xd3, // [0x52] [0x53]:Char R ,S
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

static UINT8 _Ver_TG39_Row1_Special_Content[] =
{
    0x91,0xe0, // [0x11] [0x60]:Preamble Addr Codes
    0x91,0xb0, // [0x11] [0x30]:Special Character
    0x91,0xb0, // [0x11] [0x30]:Special Character
    0x91,0x31, // [0x11] [0x31]:Special Character
    0x91,0x31, // [0x11] [0x31]:Special Character
    0x91,0x32, // [0x11] [0x32]:Special Character
    0x91,0x32, // [0x11] [0x32]:Special Character
    0x91,0xb3, // [0x11] [0x33]:Special Character
    0x91,0xb3, // [0x11] [0x33]:Special Character
    0x91,0x34, // [0x11] [0x34]:Special Character
    0x91,0x34, // [0x11] [0x34]:Special Character
    0x91,0xb5, // [0x11] [0x35]:Special Character
    0x91,0xb5, // [0x11] [0x35]:Special Character
    0x91,0xb6, // [0x11] [0x36]:Special Character
    0x91,0xb6, // [0x11] [0x36]:Special Character
    0x91,0x37, // [0x11] [0x37]:Special Character
    0x91,0x37, // [0x11] [0x37]:Special Character
    0x91,0x38, // [0x11] [0x38]:Special Character
    0x91,0x38, // [0x11] [0x38]:Special Character
    0x91,0xb9, // [0x11] [0x39]:Special Character
    0x91,0xb9, // [0x11] [0x39]:Special Character
    0x91,0xba, // [0x11] [0x3a]:Special Character
    0x91,0xba, // [0x11] [0x3a]:Special Character
    0x91,0x3b, // [0x11] [0x3b]:Special Character
    0x91,0x3b, // [0x11] [0x3b]:Special Character
    0x91,0xbc, // [0x11] [0x3c]:Special Character
    0x91,0xbc, // [0x11] [0x3c]:Special Character
    0x91,0x3d, // [0x11] [0x3d]:Special Character
    0x91,0x3d, // [0x11] [0x3d]:Special Character
    0x91,0x3e, // [0x11] [0x3e]:Special Character
    0x91,0x3e, // [0x11] [0x3e]:Special Character
    0x91,0xbf, // [0x11] [0x3f]:Special Character
    0x91,0xbf, // [0x11] [0x3f]:Special Character
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
    0x94,0xad, // [0x14] [0x2d]:Misc Ctrl Codes
};

#endif 



//VBI Scan Line variables
static UINT32 _u4ChkField = 0xFF;
static VBI_SERVICE_T _eChkType = VBI_SERVICE_NUM;
static UINT8 _u1Suite = 0xFF; 
static UINT8 _u1SuccNo = 0x0;
static UINT8 _u1TestNo = 0x0;

#define SCAN_LINE_START  1  // Scan Line Start 
#define SCAN_LINE_END   25  // Scan Line End
#define SCAN_TIMES      10  // 1 Line Scan Times (Uint: Vsync)
#define GetArrayNum(a)  (sizeof(a) / sizeof(a[0]))




//Local Functions 
static UINT32 _odd_parity_check(UINT8 data);


/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for Teletext command entry
CLI_EXEC_T arTTXCmdTbl[] = {
#ifdef CC_CLI
  {"TTX", "ttx", _i4TTXSlicer, NULL, "Teletext", CLI_SUPERVISOR},
  {"CC", "cc", _i4CCSlicer, NULL, "Closed Caption", CLI_SUPERVISOR},
  {"VPS_CNI", "vps", _i4TTX_VPS_CNI, NULL, "VPS CNI code", CLI_SUPERVISOR},
  {"WSS", "wss", _i4TTX_WSS, NULL, "WSS", CLI_SUPERVISOR},
 #endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// Table for VBI command entry
CLI_EXEC_T arVbiCmdTbl[] = {
#if VBI_CLI_SUPPORT
    {"EnableDramDump", "DramEn", _i4VBI_EnableDramDump, NULL, "Enable Dram Dump command", CLI_GUEST},
    {"SetVbiMode", "mode", _i4VBI_SetVbiMode, NULL, "Set VBI mode command", CLI_GUEST},
    {"CoreRst", "rst", _i4VBI_CoreRst, NULL, "Rest VBI core command", CLI_GUEST},
    {"SetDbgPort", "dbg", _i4VBI_DbgPort, NULL, "Rest VBI debug port command", CLI_GUEST},
    {"len", "len", _i4VBI_LineEn, NULL, "Get Set Line Enabled command", CLI_GUEST},
    {"LSC", "lsc", _i4VBI_LSC, NULL, "Get Set LSC command", CLI_GUEST},
    {"ZC", "zc", _i4VBI_ZC, NULL, "Get Set ZC command", CLI_GUEST},
    {"Fifo", "fifo", _i4VBI_FIFO, NULL, "Get FIFO Param command", CLI_GUEST},
    {"Output", "omode", _i4VBI_OutputMode, NULL, "Get Set Output Mode command", CLI_GUEST},
    {"Source", "src", _i4VBI_Source, NULL, "Get Set source command", CLI_GUEST},
    {"CcLine", "ccl", _i4VBI_CcLine, NULL, "Get Set CC line command", CLI_GUEST},
    {"TtxLine", "ttxl", _i4VBI_TtxLine, NULL, "Get Set TTX line command", CLI_GUEST},
    {"WssLine", "wssl", _i4VBI_WssLine, NULL, "Get Set WSS line command", CLI_GUEST},
    {"VpsLine", "vpsl", _i4VBI_VpsLine, NULL, "Get Set VPS line command", CLI_GUEST},
    {"VerCC", "vcc", _i4VBI_VerCC, NULL, "Verify CC command", CLI_GUEST},
    {"VerCC2X", "vcc2x", _i4VBI_VerCC2X, NULL, "Verify CC2X command", CLI_GUEST},
    {"VerUsWSS", "vwss525", _i4VBI_VerUsWSS, NULL, "Verify US WSS525 command", CLI_GUEST},
    {"VerEuWSS", "vwss625", _i4VBI_VerEuWSS, NULL, "Verify EU WSS625 command", CLI_GUEST},
    {"VerWSS", "vvps", _i4VBI_VerVPS, NULL, "Verify VPS command", CLI_GUEST},
    {"VerTTX", "vttx", _i4VBI_VerTTX, NULL, "Verify TXX command", CLI_GUEST},
    {"DumpTTXPage","dttxpage", _i4VBI_SetDumpTTXPage, NULL, "Set Dump TXX Page command", CLI_GUEST},
    {"DumpNICNI","dnicni", _i4VBI_SetDumpNICNI, NULL, "Set Dump NI CNI command", CLI_GUEST},
    {"DumpVBIRegister","dr", _i4VBI_VBIRegDump, NULL, "Dump VBI Register command", CLI_GUEST},
    {"MonVBIStatus","mvbi", _i4VBI_Mon_VBI_Status, NULL, "Monitor VBI Status command", CLI_GUEST},
    
    #if ChkTTXPageData
    {"CheckTTXPage","cttxpage", _i4VBI_CheckTTXPage, NULL, "Set Check TTX Page", CLI_GUEST},
    #endif
    {"CheckCCData", "chkcc", _i4VBI_CheckCCData, NULL, "Set Check CC Data", CLI_GUEST},
    {"ScanVBILine", "sline", _i4VBI_ScanVBILine, NULL, "Scan VBI Line", CLI_GUEST},
    {"ScanVBIRegRange", "srange", _i4VBI_ScanRange, NULL, "Scan VBI Reg Range", CLI_GUEST},
    {"AdjVBIGain", "adjgain", _i4VBI_AdjGain, NULL, "Set VBI Gain for tvd path", CLI_GUEST},
    #if AUTO_CC_DETECT
    {"AutoCCEnable", "autocc", _i4VBI_AutoCCOnOff, NULL, "Enable/Disable Auto CC Detection Function", CLI_GUEST},
    #endif
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/* Verification of Teletext at 2006 5 30 */
static INT32 _i4TTXSlicer(INT32 i4Argc, const CHAR **szArgv)
{
#if SUPPORT_TELETEXT
    UINT8 u1Path=SV_VP_MAIN;
    UINT32 u4Amount;
    UINT8* pu1MBase0;
    UINT32 u4i, u4j;
    UINT8 u1Suite;

    if((i4Argc != 3) && (i4Argc != 4))
    {
        Printf("Usage: %s <MAIN/SUB> <ON/OFF>\n", szArgv[0]);
        Printf("Usage: %s DUMP <Packet Number> <0/1>\n", szArgv[0]);
        return 0;
    }
    if(x_strcmp(szArgv[1], "MAIN") ==0 )
    {
        u1Path = SV_VP_MAIN;
    }
    else if(x_strcmp(szArgv[1], "SUB") ==0 )
    {
        u1Path = SV_VP_PIP;
    }
    else if(x_strcmp(szArgv[1], "DUMP") ==0 )
    {
        u4Amount = StrToInt(szArgv[2]);
        u1Suite = StrToInt(szArgv[3]);
        u1Suite = (u1Suite == 0) ? VBI0 : VBI2;
        pu1MBase0 = (UINT8 *)VBI_GetMbase0(u1Suite);
        
        for(u4i=0; u4i<u4Amount; u4i++)
        {
            for(u4j=0; u4j<42; u4j++)
            {
                Printf("%X ", *(pu1MBase0+u4j));
            }
            pu1MBase0 += 48;
            Printf("\n");
        }
    }
    
    if(x_strcmp(szArgv[2], "ON") ==0 )
    {
        /* 1. Initialize VBI driver in Teletext mode */
        VBI_Init(VBI_TTX_MODE);
        /* 2. Enable VBI hardware */
        VBI_Enable();
        /* 3. Enable FIFO DRAM output */
        x_memset((void*)0x3EB1830, 0, 0x96000);
        HalFlushInvalidateDCache(); /* To make sure that Cache and DRAM are consistent */
        VBI_StartNotifyTTXOnOff(u1Path, SV_ON, NULL);
        Printf("VBI hardware is enabled for Teletext\n");
    }
    else if(x_strcmp(szArgv[2], "OFF") ==0 )
    {
        VBI_StartNotifyTTXOnOff(u1Path, SV_OFF, NULL);
        VBI_Disable();
        HalFlushInvalidateDCache(); /* To make sure that Cache and DRAM are consistent */
        Printf("VBI hardware is disabled \n");
    }
#else
    Printf("SUPPORT_TELETEXT macro is disabled\n");
#endif

    return 1;
}

static INT32 _i4CCSlicer(INT32 i4Argc, const CHAR **szArgv)
{
#if SUPPORT_CLOSE_CAPTION_VCHIP
    UINT8 u1Path;
    if(i4Argc != 3)
    {
        Printf("Usage: %s <MAIN/SUB> <ON/OFF>\n", szArgv[0]);
        return 0;
    }

    u1Path = SV_VP_MAIN;
    if(x_strcmp(szArgv[1], "MAIN") ==0 )
    {
        u1Path = SV_VP_MAIN;
    }
    else if(x_strcmp(szArgv[1], "SUB") ==0 )
    {
        u1Path = SV_VP_PIP;
    }

    if(x_strcmp(szArgv[2], "ON") ==0 )
    {
        /* 1. Initialize VBI driver in CC mode */
        VBI_Init(VBI_CC_VCHIP_MODE);
        /* 2. Enable VBI hardware */
        VBI_Enable();
        VBI_StartNotifyCCOnOff(u1Path, SV_ON, NULL);
        Printf("VBI hardware is enabled for CC\n");
    }
    else if(x_strcmp(szArgv[2], "OFF") ==0 )
    {
        VBI_Disable();
        VBI_StartNotifyCCOnOff(u1Path, SV_OFF, NULL);
        Printf("VBI hardware is disabled \n");
    }
#else
    Printf("SUPPORT_CLOSE_CAPTION macro is disabled\n");
#endif

    return 1;
}

static INT32 _i4TTX_VPS_CNI(INT32 i4Argc, const CHAR **szArgv)
{
#if SUPPORT_AUTO_NAMING
    if(i4Argc != 2)
    {
        Printf("Usage: %s <ON/OFF>\n", szArgv[0]);
        return 0;
    }
    Printf("VPS testing\n");
    if(x_strcmp(szArgv[1], "ON") ==0 )
    {
        VBI_StartNotifyVPSOnOff(SV_ON, NULL);
    }
    else if(x_strcmp(szArgv[1], "OFF") ==0 )
    {
        VBI_StartNotifyVPSOnOff(SV_OFF, NULL);
    }
#else
    Printf("SUPPORT_AUTO_NAMING macro is disabled\n");
#endif
    return 0;
}

static INT32 _i4TTX_WSS(INT32 i4Argc, const CHAR **szArgv)
{
#if SUPPORT_WSS
    UINT8 u1Path=SV_VP_MAIN;
    if(i4Argc != 3)
    {
        Printf("Usage: %s <MAIN/SUB> <ON/OFF>\n", szArgv[0]);
        return 0;
    }
    if(x_strcmp(szArgv[1], "MAIN") ==0 )
    {
        u1Path = SV_VP_MAIN;
    }
    else if(x_strcmp(szArgv[1], "SUB") ==0 )
    {
        u1Path = SV_VP_PIP;
    }

    Printf("WSS testing\n");
    if(x_strcmp(szArgv[2], "ON") ==0 )
    {
        VBI_StartNotifyWSSOnOff(u1Path, SV_ON, NULL);
    }
    else if(x_strcmp(szArgv[2], "OFF") ==0 )
    {
        VBI_StartNotifyWSSOnOff(u1Path, SV_OFF, NULL);
    }
#else
    Printf("SUPPORT_WSS macro is disabled\n");
#endif
    return 0;
}

#endif //#ifdef CC_CLI

#if VBI_CLI_SUPPORT
static INT32 _i4VBI_EnableDramDump(INT32 i4Argc, const CHAR **szArgv)
{    
    UINT8 u1OnOff;
     if (i4Argc != 2)
    {
        Printf("n.vbi.DramEn 1   --> Enable Dram Dump\n");
        Printf("n.vbi.DramEn 0   --> Disable Dram Dump\n");
        return 0;
    }
    
    u1OnOff = StrToInt(szArgv[1]);
    VBI_DramDumpOnOff(u1OnOff);        
    return 0;
}

static INT32 _i4VBI_SetVbiMode(INT32 i4Argc, const CHAR **szArgv)
{    
        UINT8 u1Suite;
        UINT32 u4NewLineEn;
        VBI_MODE_T eNewMode;

        switch (i4Argc)
        {
            case 3: 
                //Set Check VBI Slicer Type
                u1Suite = StrToInt(szArgv[1]);
                u1Suite = (u1Suite == 0) ? VBI0 : VBI2;
                //Check Mode setting.
                if (x_strcmp(szArgv[2], "ttx") == 0)
                {
                    eNewMode = VBI_TTX_MODE;
                    u4NewLineEn = TT_LINE_EN;
                }
                else if (x_strcmp(szArgv[2], "cc") == 0)
                {
                    eNewMode = VBI_CC_VCHIP_MODE;
                    u4NewLineEn = CC_LINE_EN;
                }
                else if (x_strcmp(szArgv[2], "wss525") == 0)
                {
                    eNewMode = VBI_CC_VCHIP_MODE;
                    u4NewLineEn = WSS_LINE_EN;
                }
                else if (x_strcmp(szArgv[2], "wss625") == 0)
                {
                    eNewMode = VBI_TTX_MODE;
                    u4NewLineEn = WSS_LINE_EN;
                }
                else if (x_strcmp(szArgv[2], "cc2x") == 0)
                {
                    eNewMode = VBI_CC_VCHIP_MODE;
                    u4NewLineEn = TT_LINE_EN;
                    u1Suite = VBI0;
                }
                else if (x_strcmp(szArgv[2], "vps") == 0)
                {
                    eNewMode = VBI_ANAS_MODE;
                    u4NewLineEn = VPS_LINE_EN;
                    u1Suite = VBI2;
                }
                else
                {
                    goto EXIT;
                }
                break;
    
            default:
                goto EXIT;
    
        }
        //Init Scan Setting
        VBI_Init(eNewMode);
        VBI_ResetSrc(SV_VP_MAIN);
        
        VBI_SetLineEn(u1Suite, u4NewLineEn);
       
        return 0;
    
    EXIT:
        Printf("n.vbi.mode 0 ttx    --> Scan VBI0 TTX Line\n");
        Printf("n.vbi.mode 0 cc     --> Scan VBI0 CC Line\n");
        Printf("n.vbi.mode 0 wss525 --> Scan VBI0 WSS525 Line\n");
        Printf("n.vbi.mode 1 wss625 --> Scan VBI2 WSS625 Line\n");
        Printf("n.vbi.mode 0 cc2x     --> Scan VBI0 CC2X Line\n");
        Printf("n.vbi.mode 1 vps      --> Scan VBI2 VPS Line\n");
        return 0;

}


static INT32 _i4VBI_CoreRst(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;

	if (i4Argc != 2)
  	{
        Printf("n.vbi.rst 0 --> Reset VBI0 \n");
        Printf("n.vbi.rst 2 --> Reset VBI2 \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;

    if (u1Suite == VBI0)
        Printf("[VBI_CLI] Reset VBI0 \n");
    else
        Printf("[VBI_CLI] Reset VBI2 \n");

    VBI_CoreRst(u1Suite);

	return 0;
}

static INT32 _i4VBI_DbgPort(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;

	if (i4Argc != 2)
  	{
        Printf("n.vbi.dbg 0 --> VBI0_DRAM \n");
        Printf("n.vbi.dbg 1 --> VBI2_DRAM \n");
        Printf("n.vbi.dbg 2 --> VBI0_DBG \n");
        Printf("n.vbi.dbg 3 --> VBI2_DBG \n");
        return 0;
  	}

    u4Mode = StrToInt(szArgv[1]);

    if (u4Mode == 0) Printf("n.vbi.dbg 0 --> VBI0_DRAM \n");
    if (u4Mode == 1) Printf("n.vbi.dbg 1 --> VBI2_DRAM \n");
    if (u4Mode == 2) Printf("n.vbi.dbg 2 --> VBI0_DBG \n");
    if (u4Mode == 3) Printf("n.vbi.dbg 3 --> VBI2_DBG \n");

    VBI_SetDbgPort(u4Mode);

	return 0;
}

static INT32 _i4VBI_LineEn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;

	if (i4Argc != 2 && i4Argc != 4)
  	{
        Printf("n.vbi.len 0 --> Get VBI0 Enabled Lines \n");
        Printf("n.vbi.len 2 tt on --> Enable VBI2 TT lines \n");
        Printf("n.vbi.len 2 tt off --> Enable VBI2 TT lines \n");
        Printf("n.vbi.len 2 wss on --> Enable VBI2 WSS lines \n");
        Printf("n.vbi.len 2 wss off --> Enable VBI2 WSS lines \n");
        Printf("n.vbi.len 2 cc on --> Enable VBI2 CC lines \n");
        Printf("n.vbi.len 2 cc off --> Enable VBI2 CC lines \n");
        Printf("n.vbi.len 2 vps on --> Enable VBI2 VPS lines \n");
        Printf("n.vbi.len 2 vps off --> Enable VBI2 VPS lines \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;

    if (i4Argc == 2)
    {
        UINT32 u4LineEn = VBI_GetLineEn(u1Suite);

        if (u4LineEn & TT_LINE_EN)
            Printf("[VBI_CLI] TT lines are enabled \n");
        if (u4LineEn & WSS_LINE_EN)
            Printf("[VBI_CLI] WSS lines are enabled \n");
        if (u4LineEn & CC_LINE_EN)
            Printf("[VBI_CLI] CC lines are enabled \n");
        if (u4LineEn & VPS_LINE_EN)
            Printf("[VBI_CLI] VPS lines are enabled \n");
    }
    else
    {
        UINT32 u4LineEn = VBI_GetLineEn(u1Suite);
        UINT8 u1OnOff = (x_strcmp(szArgv[3], "on") == 0) ? SV_ON : SV_OFF;

        if (x_strcmp(szArgv[2], "tt") == 0)
        {
            if (u1OnOff == SV_ON)   u4LineEn |= TT_LINE_EN;
            else                    u4LineEn &= ~(TT_LINE_EN);
        }
        if (x_strcmp(szArgv[2], "wss") == 0)
        {
            if (u1OnOff == SV_ON)   u4LineEn |= WSS_LINE_EN;
            else                    u4LineEn &= ~(WSS_LINE_EN);
        }
        if (x_strcmp(szArgv[2], "cc") == 0)
        {
            if (u1OnOff == SV_ON)   u4LineEn |= CC_LINE_EN;
            else                    u4LineEn &= ~(CC_LINE_EN);
        }
        if (x_strcmp(szArgv[2], "vps") == 0)
        {
            if (u1OnOff == SV_ON)   u4LineEn |= VPS_LINE_EN;
            else                    u4LineEn &= ~(VPS_LINE_EN);
        }

        VBI_SetLineEn(u1Suite, u4LineEn);

        u4LineEn = VBI_GetLineEn(u1Suite);

        if (u4LineEn & TT_LINE_EN)
            Printf("[VBI_CLI] TT lines are enabled \n");
        if (u4LineEn & WSS_LINE_EN)
            Printf("[VBI_CLI] WSS lines are enabled \n");
        if (u4LineEn & CC_LINE_EN)
            Printf("[VBI_CLI] CC lines are enabled \n");
        if (u4LineEn & VPS_LINE_EN)
            Printf("[VBI_CLI] VPS lines are enabled \n");
    }

	return 0;
}

static INT32 _i4VBI_LSC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT32 u4LSC;

	if (i4Argc < 2 || i4Argc > 3)
  	{
        Printf("n.vbi.lsc 0 --> Get VBI0 LSC \n");
        Printf("n.vbi.lsc 2 1400 --> Set VBI2 LSC to 1400 \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;

    if (i4Argc == 2)
    {
        u4LSC = VBI_GetLSC(u1Suite);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Get VBI0 LSC 0x%lx \n", u4LSC);
        else
            Printf("[VBI_CLI] Get VBI2 LSC 0x%lx \n", u4LSC);
    }
    else
    {
        u4LSC = StrToInt(szArgv[2]);

        VBI_SetLSC(u1Suite, u4LSC);

        u4LSC = VBI_GetLSC(u1Suite);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Set VBI0 LSC 0x%lx \n", u4LSC);
        else
            Printf("[VBI_CLI] Set VBI2 LSC 0x%lx \n", u4LSC);
    }

	return 0;
}

static INT32 _i4VBI_ZC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT32 u4ZC;

	if (i4Argc < 2 || i4Argc > 3)
  	{
        Printf("n.vbi.lsc 0 --> Get VBI0 ZC \n");
        Printf("n.vbi.lsc 2 0x58 --> Set VBI2 ZC to 0x58 \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;

    if (i4Argc == 2)
    {
        u4ZC = VBI_GetZC(u1Suite);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Get VBI0 ZC 0x%lx \n", u4ZC);
        else
            Printf("[VBI_CLI] Get VBI2 ZC 0x%lx \n", u4ZC);
    }
    else
    {
        u4ZC = StrToInt(szArgv[2]);

        VBI_SetZC(u1Suite, u4ZC);

        u4ZC = VBI_GetZC(u1Suite);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Set VBI0 ZC 0x%lx \n", u4ZC);
        else
            Printf("[VBI_CLI] Set VBI2 ZC 0x%lx \n", u4ZC);
    }

	return 0;
}

static INT32 _i4VBI_FIFO(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT32 u4FifoStartAddr;
    UINT32 u4FifoSz;

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;

    u4FifoStartAddr = VBI_GetMbase0(u1Suite);
    Printf("[VBI_CLI] FIFO start addr : 0x%lx \n", u4FifoStartAddr);

    u4FifoSz = VBI_GetMOffset(u1Suite);
    Printf("[VBI_CLI] FIFO size : 0x%lx \n", u4FifoSz);

    return 0;
}

static INT32 _i4VBI_OutputMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT32 u4Mode = VBI_OMODE_NO;

	if (i4Argc < 2 || i4Argc > 3)
  	{
        Printf("n.vbi.omode 0 --> Get VBI0 Output Mode \n");
        Printf("n.vbi.omode 2 fifo --> Set VBI2 Output Mode to FIFO \n");
        Printf("n.vbi.omode 2 reg --> Set VBI2 Output Mode to Reg \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;

    if (i4Argc == 2)
    {
        u4Mode = VBI_GetOutputMode(u1Suite);

        if (u4Mode == VBI_OMODE_NO)
            Printf("[VBI_CLI] Register Mode \n");

        if (u4Mode == VBI_OMODE_DEC)
            Printf("[VBI_CLI] FIFO Mode \n");
    }
    else
    {
        if (x_strcmp(szArgv[2], "fifo") == 0)
        {
            u4Mode = VBI_OMODE_DEC;
        }
        else if (x_strcmp(szArgv[2], "reg") == 0)
        {
            u4Mode = VBI_OMODE_NO;
        }
        else
        {
            Printf("n.vbi.omode 0 --> Get VBI0 Output Mode \n");
            Printf("n.vbi.omode 2 fifo --> Set VBI2 Output Mode to FIFO \n");
            Printf("n.vbi.omode 2 reg --> Set VBI2 Output Mode to Reg \n");
        }

        VBI_SetOutputMode(u1Suite, u4Mode);

        u4Mode = VBI_GetOutputMode(u1Suite);

        if (u4Mode == VBI_OMODE_NO)
            Printf("[VBI_CLI] Register Mode \n");

        if (u4Mode == VBI_OMODE_DEC)
            Printf("[VBI_CLI] FIFO Mode \n");

    }

    return 0;
}

static INT32 _i4VBI_Source(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT8 bSrc = VBI_VSRC_DVI;

	if (i4Argc < 2 || i4Argc > 3)
  	{
        Printf("n.vbi.src 0 --> Get VBI0 source \n");
        Printf("n.vbi.src 0 tvd --> Set VBI0 source to TVD \n");
        Printf("n.vbi.src 2 hdtv --> Set VBI2 source to YPbPr \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;

    if (i4Argc == 2)
    {
        bSrc = VBI_GetSrc(u1Suite);

        if (u1Suite == VBI0)
        {
            if (bSrc == VBI_VSRC_TVD3D)
                Printf("[VBI_CLI] Get VBI0 source --> TVD \n");
            else if (bSrc == VBI_VSRC_HDTV)
                Printf("[VBI_CLI] Get VBI0 source --> HDTV \n");
            else
                Printf("[VBI_CLI] Get VBI0 source --> Unknown \n");
        }
        else
        {
            if (bSrc == VBI_VSRC_TVD3D)
                Printf("[VBI_CLI] Get VBI2 source --> TVD \n");
            else if (bSrc == VBI_VSRC_HDTV)
                Printf("[VBI_CLI] Get VBI2 source --> HDTV \n");
            else
                Printf("[VBI_CLI] Get VBI2 source --> Unknown \n");
        }
    }
    else
    {
        if (x_strcmp(szArgv[2], "tvd") == 0)
        {
            bSrc = VBI_VSRC_TVD3D;
        }
        else if (x_strcmp(szArgv[2], "hdtv") == 0)
        {
            bSrc = VBI_VSRC_HDTV;
        }
        else
        {
            Printf("n.vbi.src 0 --> Get VBI0 source \n");
            Printf("n.vbi.src 0 tvd --> Set VBI0 source to TVD \n");
            Printf("n.vbi.src 2 hdtv --> Set VBI2 source to YPbPr \n");
        }

        VBI_SetSrc(u1Suite, bSrc);

        bSrc = VBI_GetSrc(u1Suite);

        if (u1Suite == VBI0)
        {
            if (bSrc == VBI_VSRC_TVD3D)
                Printf("[VBI_CLI] Get VBI0 source --> TVD \n");
            else if (bSrc == VBI_VSRC_HDTV)
                Printf("[VBI_CLI] Get VBI0 source --> HDTV \n");
            else
                Printf("[VBI_CLI] Get VBI0 source --> Unknown \n");
        }
        else
        {
            if (bSrc == VBI_VSRC_TVD3D)
                Printf("[VBI_CLI] Get VBI2 source --> TVD \n");
            else if (bSrc == VBI_VSRC_HDTV)
                Printf("[VBI_CLI] Get VBI2 source --> HDTV \n");
            else
                Printf("[VBI_CLI] Get VBI2 source --> Unknown \n");
        }
    }

	return 0;
}

static INT32 _i4VBI_TtxLine(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT8 u1Field;
    UINT8 u1TtxStart;
    UINT8 u1TtxEnd;

	if (i4Argc != 3 && i4Argc != 5)
  	{
        Printf("n.vbi.ttxl 0 0 --> Get VBI0 Field-0 TTX line \n");
        Printf("n.vbi.ttxl 0 1 --> Get VBI0 Field-1 TTX line \n");
        Printf("n.vbi.ttxl 2 0 6 23 --> Set VBI2 Field-0 TTX line 6~23 \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;
    u1Field = StrToInt(szArgv[2]);

    if (i4Argc == 3)
    {
        VBI_GetTtxLine(u1Suite, u1Field, &u1TtxStart, &u1TtxEnd);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Get VBI0 TTX Field%d %d %d \n", u1Field, u1TtxStart, u1TtxEnd);
        else
            Printf("[VBI_CLI] Get VBI2 TTX Field%d %d %d \n", u1Field, u1TtxStart, u1TtxEnd);
    }
    else
    {
        u1TtxStart = StrToInt(szArgv[3]);
        u1TtxEnd = StrToInt(szArgv[4]);

        VBI_SetTtxLine(u1Suite, u1Field, u1TtxStart, u1TtxEnd);

        VBI_GetTtxLine(u1Suite, u1Field, &u1TtxStart, &u1TtxEnd);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Set VBI0 TTX Field%d %d %d \n", u1Field, u1TtxStart, u1TtxEnd);
        else
            Printf("[VBI_CLI] Set VBI2 TTX Field%d %d %d \n", u1Field, u1TtxStart, u1TtxEnd);
    }
    return 0;
}


static INT32 _i4VBI_VpsLine(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT8 u1Field;
    UINT8 u1VpsStart;
    UINT8 u1VpsEnd;

	if (i4Argc != 3 && i4Argc != 5)
  	{
        Printf("n.vbi.vpsl 0 0 --> Get VBI0 Field-0 VPS line \n");
        Printf("n.vbi.vpsl 0 1 --> Get VBI0 Field-1 VPS line \n");
        Printf("n.vbi.vpsl 2 0 23 24 --> Set VBI2 Field-0 VPS line 23~24 \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;
    u1Field = StrToInt(szArgv[2]);

    if (i4Argc == 3)
    {
        VBI_GetVpsLine(u1Suite, u1Field, &u1VpsStart, &u1VpsEnd);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Get VBI0 VPS Field%d %d %d \n", u1Field, u1VpsStart, u1VpsEnd);
        else
            Printf("[VBI_CLI] Get VBI2 VPS Field%d %d %d \n", u1Field, u1VpsStart, u1VpsEnd);
    }
    else
    {
        u1VpsStart = StrToInt(szArgv[3]);
        u1VpsEnd = StrToInt(szArgv[4]);

        VBI_SetVpsLine(u1Suite, u1Field, u1VpsStart, u1VpsEnd);

        VBI_GetVpsLine(u1Suite, u1Field, &u1VpsStart, &u1VpsEnd);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Set VBI0 VPS Field%d %d %d \n", u1Field, u1VpsStart, u1VpsEnd);
        else
            Printf("[VBI_CLI] Set VBI2 VPS Field%d %d %d \n", u1Field, u1VpsStart, u1VpsEnd);
    }
    return 0;
}

static INT32 _i4VBI_CcLine(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT8 u1Field;
    UINT8 u1CcStart;
    UINT8 u1CcEnd;

	if (i4Argc != 3 && i4Argc != 5)
  	{
        Printf("n.vbi.ccsl 0 0 --> Get VBI0 Field-0 CC line \n");
        Printf("n.vbi.ccsl 0 1 --> Get VBI0 Field-1 CC line \n");
        Printf("n.vbi.ccsl 2 0 17 18 --> Set VBI2 Field-0 CC line 17~18 \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;
    u1Field = StrToInt(szArgv[2]);

    if (i4Argc == 3)
    {
        VBI_GetCcLine(u1Suite, u1Field, &u1CcStart, &u1CcEnd);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Get VBI0 CC Field%d %d %d \n", u1Field, u1CcStart, u1CcEnd);
        else
            Printf("[VBI_CLI] Get VBI2 CC Field%d %d %d \n", u1Field, u1CcStart, u1CcEnd);
    }
    else
    {
        u1CcStart = StrToInt(szArgv[3]);
        u1CcEnd = StrToInt(szArgv[4]);

        VBI_SetCcLine(u1Suite, u1Field, u1CcStart, u1CcEnd);

        VBI_GetCcLine(u1Suite, u1Field, &u1CcStart, &u1CcEnd);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Set VBI0 CC Field%d %d %d \n", u1Field, u1CcStart, u1CcEnd);
        else
            Printf("[VBI_CLI] Set VBI2 CC Field%d %d %d \n", u1Field, u1CcStart, u1CcEnd);
    }

    return 0;
}

static INT32 _i4VBI_WssLine(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Suite;
    UINT8 u1Field;
    UINT8 u1WssLine;

	if (i4Argc != 3 && i4Argc != 4)
  	{
        Printf("n.vbi.wssl 0 0 --> Get VBI0 Field-0 WSS line \n");
        Printf("n.vbi.wssl 0 1 --> Get VBI0 Field-1 WSS line \n");
        Printf("n.vbi.wssl 2 0 17 --> Set VBI2 Field-0 WSS line 17 \n");
        return 0;
  	}

    u1Suite = StrToInt(szArgv[1]);
    u1Suite = (u1Suite == 0) ? VBI0 : VBI2;
    u1Field = StrToInt(szArgv[2]);

    if (i4Argc == 3)
    {
        VBI_GetWssLine(u1Suite, u1Field, &u1WssLine);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Get VBI0 WSS Field%d %d \n", u1Field, u1WssLine);
        else
            Printf("[VBI_CLI] Get VBI2 WSS Field%d %d \n", u1Field, u1WssLine);
    }
    else
    {
        u1WssLine = StrToInt(szArgv[3]);

        VBI_SetWssLine(u1Suite, u1Field, u1WssLine);

        VBI_GetWssLine(u1Suite, u1Field, &u1WssLine);

        if (u1Suite == VBI0)
            Printf("[VBI_CLI] Set VBI0 WSS Field%d %d \n", u1Field, u1WssLine);
        else
            Printf("[VBI_CLI] Set VBI2 WSS Field%d %d \n", u1Field, u1WssLine);
    }

    return 0;
}



void _vVBI_CC_ICVerified(UINT8 u1Path, UINT8 u1data1, UINT8 u1data2, UINT8 u1field)
{

    static UINT32 u4Field0Counter = 0xFFFFFFFF;
    static UINT32 u4Field1Counter = 0xFFFFFFFF;
    static UINT8 u1Fail0Count = 0;
    static UINT8 u1Fail1Count = 0;

    UINT8 au1CCField1data[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
    UINT8 au1CCField0data[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    UINT8 u1i;

    if ((u1field == 1) && (u4Field1Counter == 0xFFFFFFFF))
    {
        for(u1i=0; u1i<8; u1i++)
        {
            if(u1data1 == au1CCField1data[u1i])
            {
                u4Field1Counter = u1i;
                break;
            }
        }
    }
    else if ((u1field == 0) && (u4Field0Counter == 0xFFFFFFFF))
    {
        for(u1i=0; u1i<8; u1i++)
        {
            if(u1data1 == au1CCField0data[u1i])
            {
                u4Field0Counter = u1i;
                break;
            }
        }

    }

    if (u1Path == SV_VP_MAIN)
    {
        Printf("MAIN: CC field_%d 0x%02x 0x%02x ", u1field, u1data1, u1data2);
    }
    else
    {
        Printf("PIP: CC field_%d 0x%02x 0x%02x ", u1field, u1data1, u1data2);
    }

    if (u1field == 0)
    {
        if ((u1data1 != au1CCField0data[u4Field0Counter]) || (u1data2 != au1CCField0data[u4Field0Counter]))
        {
            Printf(" (X) \n");
            //reset verify 
            if (u1Fail0Count == 255)
            {
                u4Field0Counter = 0xFFFFFFFF;
                u1Fail0Count = 0;
            }
            u1Fail0Count++;
        }
        else
        {
            Printf(" (O) \n");
        }
        if (u4Field0Counter < 0xFFFFFFFF)
            u4Field0Counter ++;
        if (u4Field0Counter == 8)
            u4Field0Counter = 0;
    }
    else
    {
        if ((u1data1 != au1CCField1data[u4Field1Counter]) || (u1data2 != au1CCField1data[u4Field1Counter]))
        {
            Printf(" (X) \n");

            //reset verify 
            u1Fail1Count++;
            
            if (u1Fail1Count == 255)
            {
                u4Field1Counter = 0xFFFFFFFF;
                u1Fail1Count = 0;
            }
        }
        else
        {
            Printf(" (O) \n");
        }

        if (u4Field1Counter < 0xFFFFFFFF)
            u4Field1Counter ++;
        if (u4Field1Counter == 8)
            u4Field1Counter = 0;
        
    }

}

#if ChkCCInTG39
#define IS_MATCH(Input, Array) ( Input[0]== Array[0] &&\
                                 Input[1]== Array[1] &&\
                                 Input[2]== Array[2] &&\
                                 Input[3]== Array[3] )                                

void _VBI_ChkCCInTG39(UINT8 u1data1, UINT8 u1data2)
{
    static CC_Chk_State_T CC_State = E_CC_CHK_Init;
    static CC_TG39_Chk_Type_T  CC_Type  = E_CC_TG39_UnKnow;
    static CC_TG39_Chk_Type_T  Pre_CC_Chk_Type = E_CC_TG39_UnKnow;
    static UINT8 CC_Input[4] = {0xFF};
    static UINT8 u1Index = 0;
    static UINT8 u1ArraySize = 0;
    static UINT8 *pCompareArray = NULL;

    switch (CC_State) 
    {
        case E_CC_CHK_Init:
            CC_Input[0] = CC_Input[2];
            CC_Input[1] = CC_Input[3];
            CC_Input[2] = u1data1;
            CC_Input[3] = u1data2;
            CC_State = E_CC_CHK_Start;
            if (IS_MATCH(CC_Input,_Ver_TG39_Title1))           
            {
                CC_Type = E_CC_Title1;
                u1ArraySize = GetArrayNum(_Ver_TG39_Title1);
                pCompareArray = _Ver_TG39_Title1;
                Printf("Chk Title1\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Title2))
            {
                CC_Type = E_CC_Title2;
                u1ArraySize = GetArrayNum(_Ver_TG39_Title2);
                pCompareArray = _Ver_TG39_Title2;
                Printf("Chk Title2\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row1))
            {
                CC_Type = E_CC_Row1;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row1);
                pCompareArray = _Ver_TG39_Row1;
                Printf("Chk ROW1\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row2))
            {
                CC_Type = E_CC_Row2;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row2);
                pCompareArray = _Ver_TG39_Row2;
                Printf("Chk ROW2\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row3))
            {
                CC_Type = E_CC_Row3;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row3);
                pCompareArray = _Ver_TG39_Row3;
                Printf("Chk ROW3\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row4))
            {
                CC_Type = E_CC_Row4;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row4);
                pCompareArray = _Ver_TG39_Row4;
                Printf("Chk ROW4\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row5))
            {
                CC_Type = E_CC_Row5;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row5);
                pCompareArray = _Ver_TG39_Row5;
                Printf("Chk ROW5\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row6))
            {
                CC_Type = E_CC_Row6;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row6);
                pCompareArray = _Ver_TG39_Row6;
                Printf("Chk ROW6\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row7))
            {
                CC_Type = E_CC_Row7;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row7);
                pCompareArray = _Ver_TG39_Row7;
                Printf("Chk ROW7\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row8))
            {
                CC_Type = E_CC_Row8;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row8);
                pCompareArray = _Ver_TG39_Row8;
                Printf("Chk ROW8\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row9))
            {
                CC_Type = E_CC_Row9;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row9);
                pCompareArray = _Ver_TG39_Row9;
                Printf("Chk ROW9\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row10))
            {
                CC_Type = E_CC_Row10;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row10);
                pCompareArray = _Ver_TG39_Row10;
                Printf("Chk ROW10\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row11))
            {
                CC_Type = E_CC_Row11;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row11);
                pCompareArray = _Ver_TG39_Row11;
                Printf("Chk ROW11\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row12))
            {
                CC_Type = E_CC_Row12;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row12);
                pCompareArray = _Ver_TG39_Row12;
                Printf("Chk ROW12\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row13))
            {
                CC_Type = E_CC_Row13;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row13);
                pCompareArray = _Ver_TG39_Row13;
                Printf("Chk ROW13\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row14))
            {
                CC_Type = E_CC_Row14;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row14);
                pCompareArray = _Ver_TG39_Row14;
                Printf("Chk ROW14\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row15))
            {
                CC_Type = E_CC_Row15;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row15);
                pCompareArray = _Ver_TG39_Row15;
                Printf("Chk ROW15\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row1_Indent))
            {
                CC_Type = E_CC_Row1_Indent;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row1_Indent);
                pCompareArray = _Ver_TG39_Row1_Indent;
                Printf("Chk ROW1_Indent\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row3_Indent))
            {
                CC_Type = E_CC_Row3_Indent;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row3_Indent);
                pCompareArray = _Ver_TG39_Row3_Indent;
                Printf("Chk ROW3_Indent\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_BackSpace))
            {
                CC_Type = E_CC_BackSpace;
                u1ArraySize = GetArrayNum(_Ver_TG39_BackSpace);
                pCompareArray = _Ver_TG39_BackSpace;
                Printf("Chk BackSpace\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Flash))
            {
                CC_Type = E_CC_Flash;
                u1ArraySize = GetArrayNum(_Ver_TG39_Flash);
                pCompareArray = _Ver_TG39_Flash;
                Printf("Chk Flash\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_ParityChk))
            {
                CC_Type = E_CC_ParityChk;
                u1ArraySize = GetArrayNum(_Ver_TG39_ParityChk);
                pCompareArray = _Ver_TG39_ParityChk;
                Printf("Chk ParityChk\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_POPUL))
            {
                CC_Type = E_CC_POPUL;
                u1ArraySize = GetArrayNum(_Ver_TG39_POPUL);
                pCompareArray = _Ver_TG39_POPUL;
                Printf("Chk POPUL\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_POPUR))
            {
                CC_Type = E_CC_POPUR;
                u1ArraySize = GetArrayNum(_Ver_TG39_POPUR);
                pCompareArray = _Ver_TG39_POPUR;
                Printf("Chk POPUR\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_POPDR))
            {
                CC_Type = E_CC_POPDR;
                u1ArraySize = GetArrayNum(_Ver_TG39_POPDR);
                pCompareArray = _Ver_TG39_POPDR;
                Printf("Chk POPDR\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_POPDL))
            {
                CC_Type = E_CC_POPDL;
                u1ArraySize = GetArrayNum(_Ver_TG39_POPDL);
                pCompareArray = _Ver_TG39_POPDL;
                Printf("Chk POPDL\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_RowUp))
            {
                CC_Type = E_CC_RowUp;
                u1ArraySize = GetArrayNum(_Ver_TG39_RowUp);
                pCompareArray = _Ver_TG39_RowUp;
                Printf("Chk RowUp\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row1_Special_Title))
            {
                CC_Type = E_CC_Special_Title;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row1_Special_Title);
                pCompareArray = _Ver_TG39_Row1_Special_Title;
                Printf("Chk SPECIAL TITLE\n");
            }
            else if (IS_MATCH(CC_Input,_Ver_TG39_Row1_Special_Content))
            {
                CC_Type = E_CC_Special_Content;
                u1ArraySize = GetArrayNum(_Ver_TG39_Row1_Special_Content);
                pCompareArray = _Ver_TG39_Row1_Special_Content;
                Printf("Chk SPECIAL CONTENT\n");
            }
            else 
            {
                CC_Type = E_CC_TG39_UnKnow;
                u1ArraySize = 0;
                pCompareArray = NULL;
                CC_State = E_CC_CHK_Init;
            }
            // Check Lost Line
            if (CC_Type != E_CC_TG39_UnKnow)
            {

                if ((Pre_CC_Chk_Type == E_CC_RowUp) && (CC_Type == E_CC_RowUp))
                {
                    Pre_CC_Chk_Type = E_CC_RowUp;
                }
                else
                {
                    Pre_CC_Chk_Type = (CC_TG39_Chk_Type_T)((Pre_CC_Chk_Type + 1) % E_CC_TG39_CHK_MAX);
                    if (Pre_CC_Chk_Type == E_CC_TG39_UnKnow)
                    {
                        Pre_CC_Chk_Type = E_CC_Title1;
                    }
                }                
                if (Pre_CC_Chk_Type !=  CC_Type)
                {
                    if (_ParseCC_Info.bIsFirstCheck)
                    {
                        _ParseCC_Info.bIsFirstCheck = FALSE;
                        _ParseCC_Info.u1TestCnt = 0;
                        _ParseCC_Info.u1FailCnt = 0;
                    }
                    else
                    {
                        _ParseCC_Info.u1FailCnt ++;
                        if (CC_Type > Pre_CC_Chk_Type)
                        {
                            _ParseCC_Info.u1TestCnt = _ParseCC_Info.u1TestCnt + (CC_Type - Pre_CC_Chk_Type)+1;
                        }
                        else if (Pre_CC_Chk_Type > CC_Type)
                        {
                            _ParseCC_Info.u1TestCnt = _ParseCC_Info.u1TestCnt + (E_CC_Special_Content - Pre_CC_Chk_Type) + CC_Type;
                        }
                        //Printf ("##CC_Mismatch");
                        Printf(" Compare Fail -- Lost Line Pre/Cur Line = [%d, %d]\n", Pre_CC_Chk_Type, CC_Type);
                    }
                    Pre_CC_Chk_Type = CC_Type;
                }
                _ParseCC_Info.bIsFirstCheck = FALSE;

                //Printf("CC test CNT = [%d]\n", _ParseCC_Info.u1TestCnt);
            }            
            u1Index = 4;
            break;
        case E_CC_CHK_Start: 
            {
                if (pCompareArray && (u1Index+1) < u1ArraySize)
                {
                    if ((u1data1 != pCompareArray[u1Index]) || (u1data2 != pCompareArray[u1Index+1]))
                    {
                        if (_ParseCC_Info.bIsFirstCheck == FALSE)
                        {
                            Printf (" Compare Fail in Line Index = %d Data = O[%2x][%2x] X [%2x][%2x]\n ", 
                                     u1Index, pCompareArray[u1Index], pCompareArray[u1Index+1], u1data1, u1data2);
                            //Printf ("##CC_Mismatch");
                            _ParseCC_Info.u1FailCnt ++;
                            _ParseCC_Info.u1TestCnt ++;
                        }
                        CC_State = E_CC_CHK_Init;
                        CC_Type = E_CC_TG39_UnKnow;
                        u1Index = 0;
                    }
                    else
                    {
                        u1Index +=2;
                    }
                }
                else
                {
                    //Printf ("##CC_Match");
                    _ParseCC_Info.u1TestCnt++;
                    CC_State = E_CC_CHK_Init;
                    CC_Type = E_CC_TG39_UnKnow;
                    u1Index = 0;
                }

                if ( _ParseCC_Info.u1TestCnt  > (E_CC_TG39_CHK_MAX+2))
                {
                    if (_ParseCC_Info.u1FailCnt > 1)
                    {
                        Printf ("##CC_Mismatch");
                    }
                    else
                    {
                        Printf("##CC_Match");
                    }
                    _ParseCC_Info.u1TestCnt = 0;
                    _ParseCC_Info.u1FailCnt = 0;
                }  

                //Printf("CC test CNT = [%d]\n", _ParseCC_Info.u1TestCnt);

            }
            break; 
        case E_CC_CHK_Stop:
            break;
        default: 
            Printf(" Unexpected CC Chk state %d\n", CC_State );
            break;
    }
}

#endif



#if ChkCCInternalData

void _VBI_ChkCCInternalCCData(UINT8 u1data1, UINT8 u1data2)
{
    static CC_Chk_State_T CC_State = E_CC_CHK_Init;
    static CC_Chk_Type_T  CC_Type  = E_CC_UnKnow;
    static UINT8 u1Index = 0;

    switch (CC_State) 
    {
        case E_CC_CHK_Init:
            if ((u1data1 == _Ver_CC1_LineAZ[0]) && (u1data2 == _Ver_CC1_LineAZ[1]))           
            {
                CC_State = E_CC_CHK_Start;
                CC_Type = E_CC_AZ;
                u1Index = 2;
                Printf("Chk E_CC_AZ\n");
            }
            else if ((u1data1 == _Ver_CC1_Lineaz[0]) && (u1data2 == _Ver_CC1_Lineaz[1]))
            {
                CC_State = E_CC_CHK_Start;
                CC_Type = E_CC_az;
                u1Index = 2;
                Printf("Chk E_CC_az\n");
            }
            else if ((u1data1 == _Ver_CC1_LineSymbol[0]) && (u1data2 == _Ver_CC1_LineSymbol[1]))
            {
                CC_State = E_CC_CHK_Start;
                CC_Type = E_CC_Symbol;
                u1Index = 2;
                Printf("Chk E_CC_Symbol\n");
            }
            else if ((u1data1 == _Ver_CC1_SecialChar[0]) && (u1data2 == _Ver_CC1_SecialChar[1]))
            {
                CC_State = E_CC_CHK_Start;
                CC_Type = E_CC_SpecChar;
                u1Index = 2;
                Printf("Chk E_CC_SpecChar\n");
            }
            else if ((u1data1 == _Ver_CC1_Number[0]) && (u1data2 == _Ver_CC1_Number[1]))
            {
                CC_State = E_CC_CHK_Start;
                CC_Type = E_CC_Num;
                u1Index = 2;
                Printf("Chk E_CC_Num\n");
            }
            else
            {
                CC_State = E_CC_CHK_Init;
                CC_Type = E_CC_UnKnow;
                u1Index = 2;
            }

            break;
        case E_CC_CHK_Start: 
            switch (CC_Type)
            {
                case E_CC_AZ:
                    if ((u1Index + 1) < GetArrayNum(_Ver_CC1_LineAZ))
                    {
                        if ((u1data1 != _Ver_CC1_LineAZ[u1Index]) || (u1data2 != _Ver_CC1_LineAZ[u1Index+1]))
                        {
                            Printf (" COmpare Fail in A~Z Line Index = %d Data = O[%2x][%2x] X [%2x][%2x]\n ", 
                                     u1Index, _Ver_CC1_LineAZ[u1Index], _Ver_CC1_LineAZ[u1Index+1], u1data1, u1data2);

                            CC_State = E_CC_CHK_Init;
                            CC_Type = E_CC_UnKnow;
                            u1Index = 0;
                        }
                        else
                        {
                            u1Index +=2;
                        }
                    }
                    else
                    {
                        CC_State = E_CC_CHK_Init;
                        CC_Type = E_CC_UnKnow;
                        u1Index = 0;
                    }
                    break;
                    
                case E_CC_az:     
                    if ((u1Index + 1) < GetArrayNum(_Ver_CC1_Lineaz))
                    {
                        if ((u1data1 != _Ver_CC1_Lineaz[u1Index]) || (u1data2 != _Ver_CC1_Lineaz[u1Index+1]))
                        {
                            Printf (" COmpare Fail in a~z Line Index = %d Data = O[%2x][%2x] X [%2x][%2x]\n " ,
                                     u1Index, _Ver_CC1_Lineaz[u1Index], _Ver_CC1_Lineaz[u1Index+1], u1data1, u1data2);

                            CC_State = E_CC_CHK_Init;
                            CC_Type = E_CC_UnKnow;
                            u1Index = 0;
                        }
                        else
                        {
                            u1Index +=2;
                        }
                    }
                    else
                    {
                        CC_State = E_CC_CHK_Init;
                        CC_Type = E_CC_UnKnow;
                        u1Index = 0;
                    }
                    break;
                    
                case E_CC_Symbol:     
                    if ((u1Index + 1) < GetArrayNum(_Ver_CC1_LineSymbol))
                    {
                        if ((u1data1 != _Ver_CC1_LineSymbol[u1Index]) || (u1data2 != _Ver_CC1_LineSymbol[u1Index+1]))
                        {
                            Printf (" COmpare Fail in Symbol Line Index = %d Data = O[%2x][%2x] X [%2x][%2x]\n " ,
                                     u1Index, _Ver_CC1_LineSymbol[u1Index], _Ver_CC1_LineSymbol[u1Index+1], u1data1, u1data2);

                            CC_State = E_CC_CHK_Init;
                            CC_Type = E_CC_UnKnow;
                            u1Index = 0;
                        }
                        else
                        {
                            u1Index +=2;
                        }
                    }
                    else
                    {
                        CC_State = E_CC_CHK_Init;
                        CC_Type = E_CC_UnKnow;
                        u1Index = 0;
                    }
                    break;
                    
                case E_CC_SpecChar:     
                    if ((u1Index + 1) < GetArrayNum(_Ver_CC1_SecialChar))
                    {
                        if ((u1data1 != _Ver_CC1_SecialChar[u1Index]) || (u1data2 != _Ver_CC1_SecialChar[u1Index+1]))
                        {
                            Printf (" COmpare Fail in Symbol Line Index = %d Data = O[%2x][%2x] X [%2x][%2x]\n " ,
                                     u1Index, _Ver_CC1_SecialChar[u1Index], _Ver_CC1_SecialChar[u1Index+1], u1data1, u1data2);

                            CC_State = E_CC_CHK_Init;
                            CC_Type = E_CC_UnKnow;
                            u1Index = 0;
                        }
                        else
                        {
                            u1Index +=2;
                        }
                    }
                    else
                    {
                        CC_State = E_CC_CHK_Init;
                        CC_Type = E_CC_UnKnow;
                        u1Index = 0;
                    }
                    break;

                case E_CC_Num:     
                    if ((u1Index + 1) < GetArrayNum(_Ver_CC1_Number))
                    {
                        if ((u1data1 != _Ver_CC1_Number[u1Index]) || (u1data2 != _Ver_CC1_Number[u1Index+1]))
                        {
                            Printf (" COmpare Fail in Number Line Index = %d Data = O[%2x][%2x] X [%2x][%2x]\n " ,
                                     u1Index, _Ver_CC1_SecialChar[u1Index], _Ver_CC1_SecialChar[u1Index+1], u1data1, u1data2);

                            CC_State = E_CC_CHK_Init;
                            CC_Type = E_CC_UnKnow;
                            u1Index = 0;
                        }
                        else
                        {
                            u1Index +=2;
                        }
                    }
                    else
                    {
                        CC_State = E_CC_CHK_Init;
                        CC_Type = E_CC_UnKnow;
                        u1Index = 0;
                    }
                    break;
				case E_CC_UnKnow:
					break;
                    
            }
            break; 
        case E_CC_CHK_Stop:
            break;
        default: 
            Printf(" Unexpected CC Chk state %d\n", CC_State );
            break;
    }
}

#endif



void _vVBI_ParseCC(CC_Parse_Type CCType, UINT8 u1data1, UINT8 u1data2, UINT8 u1Field)
{
    UINT8 u1DataChannel; 
    BOOL bError = FALSE;

    //Set Data Channel;
    u1DataChannel = ((CCType == E_CC_CC1_Check) || (CCType == E_CC_CC3_Check)) ? 1 : 2;

    //Check Odd Parity 
    if (_odd_parity_check (u1data1) == FALSE)
    {
        Printf("Data 1 Parity Error ");
        bError = TRUE;
    }
    if (_odd_parity_check (u1data2) == FALSE)
    {
        Printf("Data 2 Parity Error");
        bError = TRUE;
    }

    if (bError)
    {
        goto EXIT;
    }
    else
    {
        u1data1 &= 0x7F;
        u1data2 &= 0x7F;
    }

    Printf("[0x%02x] [0x%02x]:", u1data1, u1data2);

    //Step 0 Check XDS data. 
    if ((u1Field == 1) && (u1data1 >= 0x1) && (u1data1 <= 0xF))
    {
        Printf("XDS data");
    }
    //Step 1 Filter Preamble Address Codes. 
    else if ((u1data1 <= 0x1F) && (u1data1 >= 0x10) &&
        (u1data2 <= 0x7F) && (u1data2 >= 0x40))
    {
        Printf("Preamble Addr Codes");
    }
    //Step 2 Filter Normal Character 
    else if ((u1data1 <= 0x7F) && (u1data1 >= 0x20))    
    {
        Printf("Char %c ,", u1data1);

        if ((u1data2 <= 0x7F) && (u1data2 >= 0x20))
        {
            Printf("%c ", u1data2);
        }
        else if (u1data2 == 00)
        {
            Printf ("Padding");
        }
    }
    //Step 3 Filter Other commands. 
    else
    {
        switch (u1data1)
        {

            case 0x00:
                if ((u1data2 <= 0x7F) && (u1data2 >= 0x20))
                {
                    Printf ("Padding, %c", u1data2);
                }
                else if (u1data2 == 0x00)
                {
                    Printf("Padding, Padding ");
                }
                else 
                {
                    goto EXIT;
                }
                break;
        
            case 0x10:
                if (u1DataChannel == 1)
                {
                    Printf("BackGround Attributes");
                }
                else
                {
                    goto EXIT;
                }
                break;
                
            case 0x11:
                if (u1DataChannel == 1)
                {
                    if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                    {
                        Printf("Mid Row Codes");
                    }
                    else if ((u1data2 <= 0x3F) && (u1data2 >= 0x30))
                    {
                        Printf("Special Character");
                    }
                    else
                    {
                        goto EXIT;
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

            case 0x12:
                if (u1DataChannel == 1)
                {
                    if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                    {
                        Printf("Spanish Ext Char");
                    }
                    else if ((u1data2 <= 0x3F) && (u1data2 >= 0x30))
                    {
                        Printf("Freach Ext Char");
                    }
                    else
                    {
                        goto EXIT;
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

            case 0x13:
                if (u1DataChannel == 1)
                {
                    if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                    {
                        Printf("Portugese Ext Char");
                    }
                    else if ((u1data2 <= 0x37) && (u1data2 >= 0x30))
                    {
                        Printf("German Ext Char");
                    }
                    else if ((u1data2 <= 0x3F) && (u1data2 >= 0x38))
                    {
                        Printf("Danish Ext Char");
                    }
                    else
                    {
                        goto EXIT;
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

            case 0x14:
                if (u1DataChannel == 1)
                {
                    if (u1Field == 0)
                    {
                        if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                        {
                            Printf("Misc Ctrl Codes");
                        }
                        else 
                        {
                            goto EXIT;
                        }
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

            case 0x15:
                if (u1DataChannel == 1)
                {
                    if (u1Field == 1)
                    {
                        if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                        {
                            Printf("Misc Ctrl Codes");
                        }
                        else 
                        {
                            goto EXIT;
                        }
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;
               
            case 0x17:
                if (u1DataChannel == 1)
                {
                    if ((u1data2 <= 0x2A) && (u1data2 >= 0x24))
                    {
                        Printf("Special Assignments");
                    }
                    else if ((u1data2 <= 0x23) && (u1data2 >= 0x21))
                    {
                        Printf("Misc Ctrl Codes TO1~TO3");
                    }
                    else if ((u1data2 <= 0x2F) && (u1data2 >= 0x2D))
                    {
                        Printf("Foreground Attributes");
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

            case 0x18: 
                if (u1DataChannel == 2)
                {
                    Printf("BackGround Attributes");
                }
                else
                {
                    goto EXIT;
                }
                break;
                
            case 0x19:
                if (u1DataChannel == 2)
                {
                    if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                    {
                        Printf("Mid Row Codes");
                    }
                    else if ((u1data2 <= 0x3F) && (u1data2 >= 0x30))
                    {
                        Printf("Special Character");
                    }
                    else
                    {
                        goto EXIT;
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;


            case 0x1A:
                if (u1DataChannel == 2)
                {
                    if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                    {
                        Printf("Spanish Ext Char");
                    }
                    else if ((u1data2 <= 0x3F) && (u1data2 >= 0x30))
                    {
                        Printf("Freach Ext Char");
                    }
                    else
                    {
                        goto EXIT;
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

            case 0x1B:
                if (u1DataChannel == 2)
                {
                    if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                    {
                        Printf("Portugese Ext Char");
                    }
                    else if ((u1data2 <= 0x37) && (u1data2 >= 0x30))
                    {
                        Printf("German Ext Char");
                    }
                    else if ((u1data2 <= 0x3F) && (u1data2 >= 0x38))
                    {
                        Printf("Danish Ext Char");
                    }
                    else
                    {
                        goto EXIT;
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;
 
            case 0x1C:
                if (u1DataChannel == 2)
                {
                    if (u1Field == 0)
                    {
                        if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                        {
                            Printf("Misc Ctrl Codes");
                        }
                        else 
                        {
                            goto EXIT;
                        }
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

                
            case 0x1D: 
                if (u1DataChannel == 2)
                {
                    if (u1Field == 1)
                    {
                        if ((u1data2 <= 0x2F) && (u1data2 >= 0x20))
                        {
                            Printf("Misc Ctrl Codes");
                        }
                        else 
                        {
                            goto EXIT;
                        }
                    }
                }
                else
                {
                    goto EXIT;
                }
                
                break;

            case 0x1F:
                if (u1DataChannel == 2)
                {
                    if ((u1data2 <= 0x2A) && (u1data2 >= 0x24))
                    {
                        Printf("Special Assignments");
                    }
                    else if ((u1data2 <= 0x23) && (u1data2 >= 0x21))
                    {
                        Printf("Misc Ctrl Codes TO1~TO3");
                    }
                    else if ((u1data2 <= 0x2F) && (u1data2 >= 0x2D))
                    {
                        Printf("Foreground Attributes");
                    }
                }
                else
                {
                    goto EXIT;
                }
                break;

            default:
                goto EXIT;
        }
    }

    return;
    EXIT:
        Printf("!! Unknow Type");
        return;

}

void _vVBI_ParseVchipLevl2(VChipType EVchipType, UINT16 u2VchipData)
{
    UINT8 u1Data;

    if (EVchipType == E_MPA)
    {
        u1Data = (u2VchipData >> 4) & 0x7; //r2 r1 r0;
    }
    else
    {
        u1Data = (u2VchipData >> 7) & 0x7; //g2 g1 g0;
    }   
    switch (u1Data)
    {
        case 0:
            switch(EVchipType)
            {
                case E_MPA:
                    Printf ("N/A");
                    break;
                case E_US_TV:
                    Printf ("None");
                    break;
                case E_Can_English:
                    Printf ("E");
                    break;
                case E_Can_French:
                    Printf ("E");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;
            
        case 1:
            switch(EVchipType)
            {
                case E_MPA:
                    Printf ("G");
                    break;
                case E_US_TV:
                    Printf ("TV-Y");                        
                    break;
                case E_Can_English:
                    Printf ("C");
                    break;
                case E_Can_French:
                    Printf ("G");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;
        case 2:
            switch(EVchipType)  
            {
                case E_MPA:
                    Printf ("PG");
                    break;
                case E_US_TV:
                    Printf ("TV-Y7");                                                
                    break;
                case E_Can_English:
                    Printf ("C8+");
                    break;
                case E_Can_French:
                    Printf ("8 ans +");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;
        case 3:
            switch(EVchipType)
            {
                case E_MPA:
                    Printf ("PG-13");
                    break;
                case E_US_TV:
                    Printf ("TV-G");                                               
                    break;
                case E_Can_English:
                    Printf ("G");
                    break;
                case E_Can_French:
                    Printf ("16 ans +");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;

        case 4:
            switch(EVchipType)
            {
                case E_MPA:
                    Printf ("R");
                    break;
                case E_US_TV:
                    Printf ("TV-PG");                                                                       
                    break;
                case E_Can_English:
                    Printf ("PG");
                    break;
                case E_Can_French:
                    Printf ("18 ans +");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;
            
        case 5:
            switch(EVchipType)
            {
                case E_MPA:
                    Printf ("NC-17");
                    break;
                case E_US_TV:
                    Printf ("TV-14");  
                    break;
                case E_Can_English:
                    Printf ("14+");
                    break;
                case E_Can_French:
                    Printf ("UnKnow");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;

        case 6:
            switch(EVchipType)
            {
                case E_MPA:
                    Printf ("X");
                    break;
                case E_US_TV:
                    Printf ("TV-MA");  
                    break;
                case E_Can_English:
                    Printf ("18+");
                    break;
                case E_Can_French:
                    Printf ("UnKnow");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;
        case 7:
            switch(EVchipType)
            {
                case E_MPA:
                    Printf ("Not Rated");
                    break;
                case E_US_TV:
                    Printf ("None");  
                    break;
                case E_Can_English:
                    Printf ("UnKnown");
                    break;
                case E_Can_French:
                    Printf ("UnKnow");
                    break;
                case E_Reserved:
                default:
                    break;
            }
            break;
           
            
        default:
            break;
    }

}


void _vVBI_ParseVChip(UINT8 u1VchipData[])
{
    UINT8 u1i = 0;
    UINT16 u2Temp = 0;
    Printf("/*******Start Parsing VCHIP Data**********/\n ");
    Printf ("Row VchipData  ");

    for (u1i = 0; u1i < 6; u1i++)
    {
        Printf("[%02x]  ", u1VchipData[u1i]);
        u2Temp += u1VchipData[u1i];
    }
    Printf ("\n");

    if ((u2Temp & 0x7F) != 0)
    {
        Printf("!!!CRC Check Error\n ");

    }
    else 
    {
        u2Temp =0; 
        //ReArrange Data  g2 g1 g0 r2 r1 r0 a3 a2 a1 a0
        u2Temp = ((u1VchipData[2]>>3) & 0x7) | //a2 a1 a0
                  (u1VchipData[3] & 0x8) | //a3
                  ((u1VchipData[2] & 0x7) << 4 ) | //r2 r1 r0
                  ((u1VchipData[3] & 0x7) << 7 ); //g2 g1 g0
        switch (u2Temp & 0x3)
        {
            case 0:
            case 2: 
                Printf (" MPA -"); 
                _vVBI_ParseVchipLevl2(E_MPA ,u2Temp);
                break;
            case 1:
                Printf (" U.S. -");
                _vVBI_ParseVchipLevl2(E_US_TV,u2Temp);
                break;

            case 3:
                switch((u2Temp>>2) &0x3)
                {
                    case 0:
                        Printf (" Canadian English - ");
                        _vVBI_ParseVchipLevl2(E_Can_English,u2Temp);
                        break;
                    case 1:
                        Printf (" Canadian French - ");
                        _vVBI_ParseVchipLevl2(E_Can_French,u2Temp);
                        break;
                    case 2:
                    case 3:
                        Printf (" Reserved Capction Mode");
                        break;
                }
                break;

            default:
                break;
        }
        Printf ("\n");
    }
    Printf("/*******Stop Parsing VCHIP Data**********/\n ");

}


void _vVBI_ParseCCData(ParseCCInfo_T *pParseCCInfo,UINT8 u1data1, UINT8 u1data2, UINT8 u1field )
{
    static BOOL bIsVchip = FALSE;
    static UINT8 u1Cnt = 0;
    static UINT8 u1VchipData[6];


    if (((u1field == 1) &&
        (_ParseCC_Info.bEnVChip &&
        ((u1data1&0x7F) == 0x1) &&
        ((u1data2&0x7F) == 0x5))) 
        || ((u1field == 1) && bIsVchip))
    {
        bIsVchip = TRUE;
        if (u1Cnt < 3)
        {
            u1VchipData[u1Cnt*2] = u1data1;
            u1VchipData[u1Cnt*2 + 1] = u1data2;
        }
        u1Cnt ++; 
        
        if (u1Cnt >= 3)
        {
            u1Cnt = 0;
            bIsVchip = FALSE;
            _vVBI_ParseVChip(u1VchipData);
        }
    }

    else 
    {
        switch (_ParseCC_Info.CCParseType) 
        {
            case E_CC_NO_Check:
                Printf("CC field_%d 0x%02x 0x%02x \n", u1field, u1data1, u1data2);
                break;

            case E_CC_IC_Check:
                break;

            case E_CC_CC1_Check:
                Printf("CC1 field_%d 0x%02x 0x%02x ", u1field, u1data1, u1data2);
                Printf(" ==> ");
                if (u1field == 0)
                {
                    _vVBI_ParseCC(E_CC_CC1_Check, u1data1, u1data2, u1field);
                }
                Printf("\n");
                break;

            case E_CC_CC2_Check:
                Printf("CC2 field_%d 0x%02x 0x%02x ", u1field, u1data1, u1data2);
                Printf(" ==> ");
                if (u1field == 0)
                {
                    _vVBI_ParseCC(E_CC_CC2_Check, u1data1, u1data2, u1field);
                }
                Printf("\n");
                break;

            case E_CC_CC3_Check:
                Printf("CC3 field_%d 0x%02x 0x%02x ", u1field, u1data1, u1data2);
                Printf(" ==> ");
                if (u1field == 1)
                {
                    _vVBI_ParseCC(E_CC_CC3_Check ,u1data1, u1data2, u1field);
                }
                Printf("\n");
                break;

            case E_CC_CC4_Check:
                Printf("CC4 field_%d 0x%02x 0x%02x ", u1field, u1data1, u1data2);
                Printf(" ==> ");
                if (u1field == 1)
                {
                    _vVBI_ParseCC(E_CC_CC4_Check ,u1data1, u1data2, u1field);
                }
                Printf("\n");
                break;
                
            case E_CC_Internal_Check:
                
                #if ChkCCInternalData
                if (u1field == 0)
                {
                    _VBI_ChkCCInternalCCData(u1data1, u1data2);            
                }
                #endif

                break;

            case E_CC_TG39_Test:
                #if ChkCCInTG39
                if (u1field == 0)
                {
                    _VBI_ChkCCInTG39(u1data1, u1data2);            
                }
                #endif
                
            case E_CC_NO_Print:
                break;
                
            default: 
                Printf("!! Unknow CC Check Type\n");
                break;
        }
    }

}





void _vVBI_CCNotify(UINT8 u1Path, UINT8 u1data1, UINT8 u1data2, UINT8 u1field)
{
    if (_ParseCC_Info.CCParseType == E_CC_IC_Check) 
    {
        _vVBI_CC_ICVerified(u1Path, u1data1, u1data2, u1field);    
    }
    else 
    {
        _vVBI_ParseCCData(&_ParseCC_Info, u1data1, u1data2, u1field);  
    }

    
}

static INT32 _i4VBI_VerCC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;
    static BOOL _bPastOnOff = TRUE;

	if (i4Argc != 2)
  	{
        Printf("n.vbi.vcc 2 --> Enable VBI CC CLI notification with MW path \n");
        Printf("n.vbi.vcc 1 --> Enable VBI CC CLI notification only \n");
        Printf("n.vbi.vcc 0 --> Disable VBI CC CLI notification \n");
        return 0;
  	}

    u1OnOff = StrToInt(szArgv[1]);

    if (u1OnOff)
    {
        _bPastOnOff = VBI_QueryNotifyOnOffStatus(VBI_CC_SERVICE, SV_VP_MAIN);
        VBI_StartNotifyCCOnOff(SV_VP_MAIN, SV_ON, NULL);
        VBI_SetCLINotifyCCFP(_vVBI_CCNotify);
        _ParseCC_Info.bIsFirstCheck = TRUE;
        _ParseCC_Info.u1TestCnt = 0;
        _ParseCC_Info.u1FailCnt = 0;
    }
    else
    {
        VBI_StartNotifyCCOnOff(SV_VP_MAIN, _bPastOnOff, NULL);
        VBI_SetCLINotifyCCFP(NULL);
    }

    VBI_SetCLINotifyMode(u1OnOff);

    return 0;
}

void _vVBI_CC2XNotify(UINT8 u1Path, UINT32 u4NumPktAvail, UINT32 u4VbiReadPtr)
{
    UINT32 u4PktIndex;
    UINT8* pu1RdPtr = (UINT8 *)u4VbiReadPtr;

    for (u4PktIndex=0; u4PktIndex<u4NumPktAvail; u4PktIndex++)
    {
        Printf("Addr: 0x%lx ", u4VbiReadPtr);
        Printf("Line: %04d --> ", (pu1RdPtr[4] & 0x7F));
        Printf("0x%02x 0x%02x 0x%02x 0x%02x \n", pu1RdPtr[0],
                                                 pu1RdPtr[1],
                                                 pu1RdPtr[2],
                                                 pu1RdPtr[3]);
        pu1RdPtr += 16;
        u4VbiReadPtr += 16;
    }

    return;
}

static INT32 _i4VBI_VerCC2X(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;
    static BOOL _bPastOnOff = FALSE;

    if (i4Argc != 2)
    {
        Printf("n.vbi.vcc2x 2 --> Enable VBI CC2X CLI notification with MW path\n");
        Printf("n.vbi.vcc2x 1 --> Enable VBI CC2X CLI notification only\n");
        Printf("n.vbi.vcc2x 0 --> Disable VBI CC2X CLI notification \n");
        return 0;
    }

    u1OnOff = StrToInt(szArgv[1]);

    if (u1OnOff)
    {
        _bPastOnOff = VBI_QueryNotifyOnOffStatus(VBI_CC2X_SERVICE, SV_VP_MAIN);
        VBI_StartNotifyCC2XOnOff(SV_VP_MAIN, SV_ON, NULL);
        VBI_SetCLINotifyCC2XFP(_vVBI_CC2XNotify);
    }
    else
    {
        VBI_StartNotifyCC2XOnOff(SV_VP_MAIN, _bPastOnOff, NULL);
        VBI_SetCLINotifyCC2XFP(NULL);
    }
    VBI_SetCLINotifyMode(u1OnOff);
    return 0;
}

void _vVBI_UsWSSNotify(UINT8 u1Path, UINT32 u4Data)
{
    UINT8 u1Index = 0xff;
    UINT8 u1Loop = 0;
    UINT32 au4UsWss[] = { 0xb3fff, 0xfd555, 0x93c00 };

    for(u1Loop=0; u1Loop<3; u1Loop++)
    {
        if(u4Data == au4UsWss[u1Loop])
        {
            u1Index = u1Loop;
            break;
        }
    }


    if (u1Path == SV_VP_MAIN)
    {
        Printf("MAIN: US WSS525 0x%lx ", u4Data);
    }
    else
    {
        Printf("PIP: US WSS525 0x%lx ", u4Data);
    }

    if (u1Index == 0xFF)
    {
        //u1Index = 0xff;
        Printf(" (X) \n");
    }
    else
    {
        //u1Index = (u1Index++) % 12;
        Printf(" (O) \n");
    }
}

static INT32 _i4VBI_VerUsWSS(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;
    static BOOL _bPastOnOff = TRUE;

    if (i4Argc != 2)
    {
        Printf("n.vbi.vwss 2 --> Enable VBI WSS525 CLI notification with MW path \n");
        Printf("n.vbi.vwss 1 --> Enable VBI WSS525 CLI notification only\n");
        Printf("n.vbi.vwss 0 --> Disable VBI WSS525 CLI notification \n");
        return 0;
    }

    u1OnOff = StrToInt(szArgv[1]);

    if (u1OnOff)
    {
        _bPastOnOff = VBI_QueryNotifyOnOffStatus(VBI_WSS525_SERVICE, SV_VP_MAIN);    
        VBI_StartNotifyUSWSSOnOff(SV_VP_MAIN, SV_ON, NULL);
        VBI_SetCLINotifyWSSFP(_vVBI_UsWSSNotify);
    }
    else
    {
        VBI_StartNotifyUSWSSOnOff(SV_VP_MAIN, _bPastOnOff, NULL);
        VBI_SetCLINotifyWSSFP(NULL);
    }
    VBI_SetCLINotifyMode(u1OnOff);
    return 0;
}

void _vVBI_EuWSSNotify(UINT8 u1Path, UINT32 u4Data)
{
    if (u1Path == SV_VP_MAIN)
    {
        Printf("MAIN: EU WSS625 0x%lx \n", u4Data);
    }
    else
    {
        Printf("PIP: EU WSS625 0x%lx \n", u4Data);
    }
}

static INT32 _i4VBI_VerEuWSS(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;
    static BOOL _bPastOnOff = TRUE;

    if (i4Argc != 2)
    {
        Printf("n.vbi.vwss 2 --> Enable VBI WSS625 CLI notification with MW Path \n");
        Printf("n.vbi.vwss 1 --> Enable VBI WSS625 CLI notification only\n");
        Printf("n.vbi.vwss 0 --> Disable VBI WSS625 CLI notification \n");
        return 0;
    }

    u1OnOff = StrToInt(szArgv[1]);

    if (u1OnOff)
    {
        _bPastOnOff = VBI_QueryNotifyOnOffStatus(VBI_WSS625_SERVICE, SV_VP_MAIN);        
        VBI_StartNotifyWSSOnOff(SV_VP_MAIN, SV_ON, NULL);
        VBI_SetCLINotifyWSSFP(_vVBI_EuWSSNotify);
    }
    else
    {
        VBI_StartNotifyWSSOnOff(SV_VP_MAIN, _bPastOnOff, NULL);
        VBI_SetCLINotifyWSSFP(NULL);
    }
    VBI_SetCLINotifyMode(u1OnOff);
    return 0;
}

static INT32 _i4VBI_SetDumpNICNI(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 _u4Input;
    _ParseTTX_Info.bEnCharDisplay = FALSE;

	if (i4Argc != 2)
  	{
        Printf("n.vbi.dnicni 1 --> enable Dump NI CNI \n");
        Printf("n.vbi.dnicni 0 --> disable Dump NI CNI \n");
        return 0;
  	}

    _u4Input = StrToInt(szArgv[1]);

    if (_u4Input)
    {
        _ParseTTX_Info.bEnDumpNICNI = TRUE;
    }
    else
    {
        _ParseTTX_Info.bEnDumpNICNI = FALSE;
    }
    
    Printf("Please Enable CLI VerTTX Cmd to dump TTX NI CNI\n");
    Printf("Please Enable CLI VerVPS Cmd to dump VPS CNI\n");

    return 1;
}


static INT32 _i4VBI_SetDumpTTXPage(INT32 i4Argc, const CHAR **szArgv)
{

    UINT8 u1CharMode;
    UINT8 u1StrLen;
    UINT32 u4temp;
    
    //Reset Parameters. 
    _ParseTTX_Info.bEnDumpPage    = FALSE;
    _ParseTTX_Info.bEnCharDisplay = FALSE;
    _ParseTTX_Info.bEnChkHam84_B01= FALSE;
    _ParseTTX_Info.bEnChkHam84_B29= FALSE; 
    _ParseTTX_Info.bEnChkParity   = FALSE; 
    _ParseTTX_Info.u1ChkTTXMode   = 0;
    _ParseTTX_Info.bEnPaketCount  = FALSE;
    _ParseTTX_Info.bEnChkQlabPage = FALSE;
    _ParseTTX_Info.u1FailPktCnt   = 0;

	if (i4Argc != 3)
  	{
        goto ERROR;
    }

    u1StrLen = x_strlen(szArgv[1]);

    if (u1StrLen != 3)
  	{
        goto ERROR;
    }

    u4temp = StrToHex(szArgv[1],u1StrLen);
    
    _ParseTTX_Info.u1MagNum = (u4temp>>8)&0xF;
    _ParseTTX_Info.u1PAGTen = (u4temp>>4)&0xF;
    _ParseTTX_Info.u1PAGUint = u4temp&0xF;
    u1CharMode = szArgv[2][0];
        
    if ((_ParseTTX_Info.u1MagNum > 8) ||
        (_ParseTTX_Info.u1PAGUint > 0xF) || 
        (_ParseTTX_Info.u1PAGTen > 0xF) )
    {
        Printf("Invalid Page Number %x%x%x\n", _ParseTTX_Info.u1MagNum,_ParseTTX_Info.u1PAGTen,_ParseTTX_Info.u1PAGUint);
        goto ERROR;
    }    

    if ( (u1CharMode == 0x43)||(u1CharMode == 0x63)) //ASCII "C = 0x43, c = 0x63"        _ParseTTX_Info.bEnCharDisplay = TRUE;
    {
        _ParseTTX_Info.bEnCharDisplay = TRUE;
    }

    //Enable Dump Page
    _ParseTTX_Info.bEnDumpPage = TRUE;
    _ParseTTX_Info.bEnChkHam84_B01= TRUE;

    Printf("Set Dump TXX Page %x%x%x in %c Mode Rdy\n", _ParseTTX_Info.u1MagNum,
                                                        _ParseTTX_Info.u1PAGTen,
                                                        _ParseTTX_Info.u1PAGUint,
                                                        u1CharMode);
    Printf("Enable CLI VerTTX (n.vbi.vttx) to dump TTX Page\n");

    return 1;

ERROR:
    Printf("n.vbi.dttxpage 899 C --> Set TTX Dump Page 899 in Char Mode\n");
    Printf("n.vbi.dttxpage 899 H --> Set TTX Dump Page 899 in Hex  Mode\n");
        return 0;
   
}

static void _PrintTTX_Data (UINT8 u1PktNumber, BOOL bCharOut, UINT8 *pLineData)
{
    TTX_Pkt_Type_T PktType = E_Pkt_ERR;
    UINT8 u1i;
   
    if (u1PktNumber == 0)
    {
        PktType = E_Pkt_Header;
    }
    else if (u1PktNumber < 25)
    {
        PktType = E_Pkt_Normal;
    }
    else if (u1PktNumber == 0xFF)
    {
        PktType = E_Pkt_ERR;
    }
    else 
    {
        return ;
    }
        


    // else X/26 X/27 Need to do

    switch (PktType)
    {
        case E_Pkt_Header: 
            Printf("[pkd][%2d]:", u1PktNumber);

            for (u1i = 0; u1i<=9; u1i++)
            {
                Printf("%02x", pLineData[u1i]);
            }

            for (u1i = 10; u1i<42; u1i++)
            {
                if (bCharOut)
                {
                    if ((pLineData[u1i]&0x7F) >= 0x20)
                        Printf("%2c", (pLineData[u1i]&0x7F) );
                    else 
                        Printf("%2c", 0x3F); // Printf ?

                }
                else
                {
                    Printf("%02x", pLineData[u1i]);
                }
            }
            Printf("*L:%2X-*%02d", pLineData[42],pLineData[46]);
            break; 

        case E_Pkt_Normal: 
            Printf("[pkd][%2d]:", u1PktNumber);
            if (bCharOut)
            {
                for (u1i = 2; u1i<42; u1i++)
                {
                    if ((pLineData[u1i]&0x7F) >= 0x20)
                        Printf("%2c", (pLineData[u1i]&0x7F) );
                    else 
                        Printf("%2c", 0x1);
                }

            }
            else
            {
                for (u1i = 0; u1i<42; u1i++)
                {
                    Printf("%02x", pLineData[u1i]);
                }
            }
            Printf("*L:%2X-*%02d", pLineData[42],pLineData[46]);

            break; 

        case E_Pkt_ERR:
            Printf("[pkd][%2d][Ham Err: UnKnow]:", u1PktNumber);
            if (bCharOut)
            {
                for (u1i = 2; u1i<42; u1i++)
                {
                    if ((pLineData[u1i]&0x7F) >= 0x20)
                        Printf("%2c", (pLineData[u1i]&0x7F) );
                    else 
                        Printf("%2c", 0x1);
                }

            }
            else
            {
                for (u1i = 0; u1i<42; u1i++)
                {
                    Printf("%02x", pLineData[u1i]);
                }
            }
            Printf("*L:%2X-*%02d", pLineData[42],pLineData[46]);
            break;

        default : 
            break; 

    }
    Printf("\n");


}

static INT32 _i4VBI_CheckCCData(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    if (i4Argc == 2)
    {
        u1Mode = StrToInt(szArgv[1]);
        switch (u1Mode)
        {
            case 0:
                _ParseCC_Info.CCParseType = E_CC_NO_Check; 
                _ParseCC_Info.bEnVChip = FALSE; 
                break;

            case 1:
                _ParseCC_Info.CCParseType = E_CC_CC1_Check; 
                break;

            case 2:
                _ParseCC_Info.CCParseType = E_CC_CC2_Check; 
                break;

            case 3:
                _ParseCC_Info.CCParseType = E_CC_CC3_Check; 
                break;

            case 4:
                _ParseCC_Info.CCParseType = E_CC_CC4_Check; 
                break;

            case 5:
                _ParseCC_Info.CCParseType = E_CC_Internal_Check; 
                break;

            case 6:
                _ParseCC_Info.bEnVChip = TRUE; 
                break;

            case 7:
                _ParseCC_Info.CCParseType = E_CC_IC_Check; 
                break;

            case 8:
                _ParseCC_Info.CCParseType = E_CC_NO_Print; 
                break;

            case 9:
                _ParseCC_Info.CCParseType = E_CC_TG39_Test; 
                break;
                
        
            default: 
                goto EXIT;
        }
    }
    else
    {
        goto EXIT;
    }

    Printf("Set Check CC in Mode %d ready\n", u1Mode);
    Printf("Enable CLI notify to check CC data (Type: n.vbi.vcc) \n");

    return 0;

EXIT:
    Printf("n.vbi.chkcc 0 --> Only Print CC Row data\n");
    Printf("n.vbi.chkcc 1 --> Mode 1: Print CC row data and do CC1 parsing\n");
    Printf("n.vbi.chkcc 2 --> Mode 2: Print CC row data and do CC2 parsing\n");
    Printf("n.vbi.chkcc 3 --> Mode 3: Print CC row data and do CC3 parsing\n");
    Printf("n.vbi.chkcc 4 --> Mode 4: Print CC row data and do CC4 parsing\n");
    #if ChkCCInternalData
    Printf("n.vbi.chkcc 5 --> Mode 5: Check CC row data with with internal defined (Fluke CC1)\n");
    #endif 
    Printf("n.vbi.chkcc 6 --> Mode 6: Enable Check VChip Data\n");
    Printf("n.vbi.chkcc 7 --> Mode 7: Check CC row data with Specific IC verified pattern \n");
    Printf("n.vbi.chkcc 8 --> Mode 8: Disable Print CC Row data \n");
    #if ChkCCInTG39
    Printf("n.vbi.chkcc 9 --> Mode 9: Check CC row data (TG 39 CC1) \n");
    #endif
    return 0;

}



#if ChkTTXPageData
static INT32 _i4VBI_CheckTTXPage(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1StrLen;
    UINT32 u4temp;
    UINT8 u1Mode;
    
    //Reset Parameters. 
    _ParseTTX_Info.bEnDumpPage = FALSE;
    _ParseTTX_Info.bEnChkHam84_B01= FALSE;
    _ParseTTX_Info.bEnChkHam84_B29= FALSE; 
    _ParseTTX_Info.bEnChkParity = FALSE; 
    _ParseTTX_Info.bEnCharDisplay = FALSE;
    _ParseTTX_Info.u1ChkTTXMode   = 0;
    _ParseTTX_Info.bEnPaketCount  = FALSE;
    _ParseTTX_Info.bEnChkQlabPage = FALSE;

    if (i4Argc == 3)
    {
        u1StrLen = x_strlen(szArgv[1]);

        if (u1StrLen != 3)
        {
            goto ERROR;
  	    }

        u4temp = StrToHex(szArgv[1],u1StrLen);
        u1Mode = StrToInt(szArgv[2]);

        _ParseTTX_Info.u1MagNum = (u4temp>>8)&0xF;
        _ParseTTX_Info.u1PAGTen = (u4temp>>4)&0xF;
        _ParseTTX_Info.u1PAGUint = u4temp&0xF;
    
        if ((_ParseTTX_Info.u1MagNum > 8 )    ||
            (_ParseTTX_Info.u1PAGUint > 0xF ) || 
            (_ParseTTX_Info.u1PAGTen > 0xF ) )
        {
            Printf("Invalid Page Number %x%x%x\n", _ParseTTX_Info.u1MagNum,_ParseTTX_Info.u1PAGTen,_ParseTTX_Info.u1PAGUint);
            goto ERROR;
        }    

        if ((_ParseTTX_Info.u1MagNum == 1 ) &&
            (_ParseTTX_Info.u1PAGUint == 1) && 
            (_ParseTTX_Info.u1PAGTen == 2 ) )
        {
            _ParseTTX_Info.bEnChkQlabPage = TRUE;
        }

        switch (u1Mode) 
        {
            case 0:
                break;
            case 1:
                _ParseTTX_Info.u1ChkTTXMode = 1;
                _ParseTTX_Info.bEnChkHam84_B01= TRUE;
                _ParseTTX_Info.bEnPaketCount  = TRUE;
                break;
    
            default:
                goto ERROR;
        }    
        _ParseTTX_Info.bEnCharDisplay = FALSE;

        Printf("Set Chek TXX Page %x%x%x Rdy\n", _ParseTTX_Info.u1MagNum,_ParseTTX_Info.u1PAGTen,_ParseTTX_Info.u1PAGUint );
        Printf("Enable CLI VerTTX (n.vbi.vttx) to dump TTX Page\n");
    }
    else if (i4Argc == 2)
    {
        u1Mode = StrToInt(szArgv[1]);
        if (u1Mode == 2)
        {
            _ParseTTX_Info.u1ChkTTXMode = 2;
            _ParseTTX_Info.bEnChkHam84_B01= TRUE;
            _ParseTTX_Info.bEnChkHam84_B29= TRUE; 
            _ParseTTX_Info.bEnChkParity  = TRUE; 
        }
        else
        {
            goto ERROR;
        }

    }
    else
    {
        goto ERROR;
    }


    return 1;

ERROR:
    Printf("n.vbi.cttxpage 100 0 --> Disable to check ttx 100 page (Compare with internal define)\n");
    Printf("n.vbi.cttxpage 100 1 --> Mode 1: Dispaly Hamming Error and Check ttx 100 page (Compare with internal define)\n");
    Printf("n.vbi.cttxpage 121 1 --> Mode 1: Dispaly Hamming Error and Check ttx 121 page (Compare with QLAB TTX data)\n");
    Printf("n.vbi.cttxpage 2     --> Mode 2: Dispaly Hamming Error and Check Parity Error(X1~X25) for all input\n");

    return 0;

}


static void _CheckTTX_Data (UINT8 u1PktNumber, BOOL bCharOut, UINT8 *pLineData)
{
    TTX_Pkt_Type_T PktType = E_Pkt_ERR;
    UINT8 u1i;
    BOOL bFailed = FALSE;
    UINT8 *pPk0ArryPtr = NULL;
    UINT8 u1Pkt0ArraySize = 0;
    
    if (_ParseTTX_Info.bEnChkQlabPage)
    {
        pPk0ArryPtr = (UINT8*)_Ver_QLab_Pkt0;
        u1Pkt0ArraySize = GetArrayNum(_Ver_QLab_Pkt0);
    }
    else
    {
        pPk0ArryPtr = (UINT8*)_Ver_Pkt0;
        u1Pkt0ArraySize = GetArrayNum(_Ver_Pkt0);
    }
    
    if (u1PktNumber > 29)
        return;
    
    if (u1PktNumber == 0)
    {
        PktType = E_Pkt_Header;
    }
    else if (u1PktNumber < 25)
    {
        PktType = E_Pkt_Normal;
    }
    // else X/26 X/27 Need to do

    switch (PktType)
    {
        case E_Pkt_Header:             
            for (u1i = 0; u1i<u1Pkt0ArraySize; u1i++)
            {
                if (pLineData[u1i+2] != pPk0ArryPtr[u1i])
                {
                    bFailed = TRUE;
                    break;
                }                
            }
            if (bFailed)
            {
                Printf("Fail:");
                _PrintTTX_Data(u1PktNumber, FALSE, pLineData);
                _PrintTTX_Data(u1PktNumber, TRUE, pLineData);
                if(_ParseTTX_Info.bEnChkQlabPage)
                {
                    _ParseTTX_Info.u1FailPktCnt++;
                    //Printf("##TT_Mismatch");
                }
            }
    
            break; 

        case E_Pkt_Normal: 
            for (u1i = 0; u1i<GetArrayNum(_Ver_Pkt1_23); u1i++)
            {
                if (pLineData[u1i+2] != _Ver_Pkt1_23[u1i])
                {
                    bFailed = TRUE;
                    break;
                }                
            }
            if (bFailed)
            {
                Printf("Fail:");
                _PrintTTX_Data(u1PktNumber, FALSE, pLineData);
                _PrintTTX_Data(u1PktNumber, TRUE, pLineData);
                if(_ParseTTX_Info.bEnChkQlabPage)
                {
                    _ParseTTX_Info.u1FailPktCnt++;
                    //Printf("##TT_Mismatch");
                }
            }
            break; 

        default : 
            break; 

    }

}


#else

static void _CheckTTX_Data (UINT8 u1PktNumber, BOOL bCharOut, UINT8 *pLineData)
{
    Printf("Warning !! unsupported CheckTTX CLI Cmd(n.vbi.cttxpage)\n");
}

#endif

static HAM_Check_Rst _hamming_84_decoding(BYTE* src,BYTE* des)
{
	BYTE A,B,C,D;
	BYTE D1,D2,D3,D4;
	BYTE P1,P2,P3,P4;
	BYTE hamming_value=0;
	HAM_Check_Rst error_check = E_Ham_Correct;/* CD_FAIL;*/
		
	//BIT  0  1   2   3   4   5   6   7
	//    P1  D1  P2  D2  P3  D3  P4  D4
	P1 = *src&0x01;
	D1 = (*src>>1)&0x01;
	P2 = (*src>>2)&0x01;
	D2 = (*src>>3)&0x01;
	P3 = (*src>>4)&0x01;
	D3 = (*src>>5)&0x01;
	P4 = (*src>>6)&0x01;
	D4 = (*src>>7)&0x01;
		
	A = P1 ^ D1 ^ D3 ^ D4; 	
	B = D1 ^ P2 ^ D2 ^ D4; 
	C = D1 ^ D2 ^ P3 ^ D3; 
	D = P1 ^ D1 ^ P2 ^ D2 ^ P3 ^ D3 ^ P4 ^ D4;
	
	hamming_value = (A<<3)+(B<<2)+(C<<1)+D;
	
	switch(hamming_value)
	{
	    case 0: //D1
	 	     D1 = ~D1&0x01; break;
	    case 2: //D4
	 	     D4 = ~D4&0x01; break;
	    case 4: //D3
	 	     D3 = ~D3&0x01; break;
	    case 6: //P1
	 	     P1 = ~P1&0x01; break;	 	
	    case 8: //D2
	 	     D2 = ~D2&0x01; break;	 	
	    case 10: //P2
	 	     P2 = ~P2&0x01; break;	 	
	    case 12: //P3
	         P3 = ~P3&0x01; break;
	    case 14: //P4
	 	     P4 = ~P4&0x01; break;
	    case 15://no error
	         error_check = E_Ham_NoErr;
	         break;	 	
	    default: //double erroe
	 	     error_check = E_Ham_Error; 
	 	     break;
	}
	
	*des = D1 + (D2<<1) + (D3<<2) + (D4<<3);
		
	return error_check;
}


//Retrun:  True: success / False: Fail
static UINT32 _odd_parity_check(UINT8 data)
{
	UINT8 i,odd_parity = 0;

	for(i=0;i<8;i++)
	{
		odd_parity ^= (data >> i)& 0x01; 
	}

	if(odd_parity) return TRUE;
	else              return FALSE;
}


static UINT8 _pkt_830_decode(UINT8* pui1_pkt)
{
	UINT8 ui1_i;
	
	_hamming_84_decoding(pui1_pkt,pui1_pkt);

	/* Check Designate code */
	if ((*pui1_pkt == 0) ||(*pui1_pkt == 1)) /* 8/30 format 1 */
	{
	     for (ui1_i=1;ui1_i<=6;ui1_i++)
            {
	            _hamming_84_decoding(pui1_pkt+ui1_i,pui1_pkt+ui1_i);
	     }

	     for (ui1_i=20;ui1_i<40;ui1_i++)
	     {
	            _odd_parity_check(*(pui1_pkt+ui1_i));					
		     *(pui1_pkt+ui1_i) = *(pui1_pkt+ui1_i) & 0x7f;
	     }
	}
	else if ((*pui1_pkt == 2) ||(*pui1_pkt == 3)) /* 8/30 format 2 */
	{
	     for (ui1_i=1;ui1_i<=19;ui1_i++)
	     {
	           _hamming_84_decoding(pui1_pkt+ui1_i,pui1_pkt+ui1_i);
	     }

	     for (ui1_i=20;ui1_i<40;ui1_i++)
	     {
	            _odd_parity_check(*(pui1_pkt+ui1_i));
		     *(pui1_pkt+ui1_i) = *(pui1_pkt+ui1_i) & 0x7f;
	     }	   
	   
	}      
	
	return TRUE;
}


static BOOL _vVBI_ParseTTXPage(ParseTTXInfo_T *pParseTTXInfo, UINT32 u4NumPktAvail, UINT32 u4VbiReadPtr)
{
    BYTE *pbRdr = (BYTE *) u4VbiReadPtr ;
    UINT16 u2Index;
    UINT32 mag, pkt;  
    UINT16 ui2_ni,ui2_cni;
    UINT8 aui1_ttx_data[48]; //42 Byte Data + 6Byte Dbg Msg
    UINT8 u1i, u1PageUnit = 0, u1PageTen = 0;
    static BOOL bParallelMode = FALSE;
    UINT8 u1Temp0, u1Temp1;
    UINT16 u2ControlBit = 0;
    UINT16 u2SubPage = 0;

    HalFlushInvalidateDCache(); /* To make sure that Cache and DRAM are consistent */

    for (u2Index = 0; u2Index < u4NumPktAvail; u2Index++)
    {
        x_memcpy( (VOID *) aui1_ttx_data,(VOID *) pbRdr, 48);

        //Haming 84 for Pkt & Mag Number
        if ((_hamming_84_decoding(&(aui1_ttx_data[0]),&(u1Temp0)) > _HamChk_Level) ||  
            (_hamming_84_decoding(&(aui1_ttx_data[1]),&(u1Temp1)) > _HamChk_Level))
        {
            
            if ((pParseTTXInfo->bEnDumpPage) ||  (pParseTTXInfo->u1ChkTTXMode == 1))
            {
                if (pParseTTXInfo ->bEnChkHam84_B01 && (TTXParseState == E_Dump_Start))
                {
                    Printf("XXXXXXXXXXXHaming Error in Magazin & Row NoXXXXXXXXXXXX\n");
                    _PrintTTX_Data(0xFF, FALSE, aui1_ttx_data);
                    //_PrintTTX_Data(25, TRUE, aui1_ttx_data);

                }
            }
            else if (pParseTTXInfo->u1ChkTTXMode == 2)
            {
                if (pParseTTXInfo ->bEnChkHam84_B01)
                {
                    Printf("Haming Error in Magazin & Row No\n");
                }
            }
            
            goto NEXT;
        }

        mag = u1Temp0 & 0x07;

        if (mag == 0) 
        {
        	  mag = 8;
        }
        pkt =  ((u1Temp1<<1) | ((u1Temp0&0x08)>>3));

        //Haming 84 for PagNo Subcode Ctl bits. 
        if (pkt == 0)
        {
            for (u1i = 2; u1i <= 9; u1i++)
            {
                if (_hamming_84_decoding(&aui1_ttx_data[u1i],&aui1_ttx_data[u1i]) > _HamChk_Level)
                {
                    if (pParseTTXInfo ->bEnChkHam84_B29)
                    {
                        Printf("Haming Error in Mag[%ld]Pkt0 PageNo, Sub-Code, Controlbits\n",mag);
                    }
                
                    goto NEXT;
                }            
            }  
            u1PageUnit =  aui1_ttx_data[2];
            u1PageTen =   aui1_ttx_data[3];
            u2SubPage = ((aui1_ttx_data[7]&0x3)<<12)    |\
                         (aui1_ttx_data[6]<<8)          |\
                        ((aui1_ttx_data[5]&0x7)<<4)     |\
                          aui1_ttx_data[4];
            
            u2ControlBit =  aui1_ttx_data[9]<<11        |\
                            aui1_ttx_data[8]<<7         |\
                           (aui1_ttx_data[7] & 0xC)<<3  |\
                           (aui1_ttx_data[5] & 0x8)<<1; 
        }
        
        /* Check Parity Error Process */
        if (pParseTTXInfo->bEnChkParity)
        {
            if (aui1_ttx_data[46]!=0)
            {
                Printf("Fail: [Mag][%ld]",mag);
                _PrintTTX_Data(pkt, FALSE, aui1_ttx_data);
                _PrintTTX_Data(pkt, TRUE, aui1_ttx_data);
            }
        }
        
        /* Dump PAGE Process */
        if (pParseTTXInfo->bEnDumpPage || (pParseTTXInfo->u1ChkTTXMode == 1))
        {
            static BOOL bLastisPk0 = FALSE;
            switch (TTXParseState)
            {
                //Check Pkt 0 is correct or not
                case E_Dump_Init:
                    if ((mag == pParseTTXInfo->u1MagNum) && 
                        (pkt == 0) &&  
                        (u1PageUnit == pParseTTXInfo->u1PAGUint) && 
                    (u1PageTen == pParseTTXInfo->u1PAGTen))
                    {
                        //C11 should be 1 for serious mode
                        bParallelMode = (((u2ControlBit >>11)&1) == 0) ? TRUE : FALSE;
                        if (pParseTTXInfo->u1ChkTTXMode == 1)
                        {
                            _CheckTTX_Data(pkt, pParseTTXInfo->bEnCharDisplay, aui1_ttx_data);
                        }
                        else
                        {
                            Printf("###### Start Dump Page[%x%x%x] SubPage[%04x] CBit[%04x] ######\n", 
                                                            pParseTTXInfo->u1MagNum, 
                                                            pParseTTXInfo->u1PAGTen, 
                                                            pParseTTXInfo->u1PAGUint, 
                                                            u2SubPage,
                                                            u2ControlBit);
                        
                            _PrintTTX_Data(pkt, pParseTTXInfo->bEnCharDisplay, aui1_ttx_data);
                        }
                        TTXParseState = E_Dump_Start;
                        bLastisPk0 = TRUE;
                        if (_ParseTTX_Info.bEnPaketCount)
                        {
                            _ParseTTX_Info.u1PagePktCnt = 0;
                        }
                        if(_ParseTTX_Info.bEnChkQlabPage)
                        {
                            if (_ParseTTX_Info.u1FailPktCnt > 0)
                            {
                                Printf("##TT_Mismatch");
                            }
                            else
                            {
                                Printf("##TT_Match");
                            }
                            _ParseTTX_Info.u1FailPktCnt = 0;
                        }
                    }
                    break;


                case E_Dump_Start:
                    // Dump Non Pak 0 items
                    if (pkt != 0)
                    {
                        if ( mag == pParseTTXInfo->u1MagNum && ((pkt <= 23) && (pkt >= 1)) ) 
                        {
                            if (pParseTTXInfo->u1ChkTTXMode == 1)
                                _CheckTTX_Data(pkt, pParseTTXInfo->bEnCharDisplay, aui1_ttx_data);
                            else
                                _PrintTTX_Data(pkt, pParseTTXInfo->bEnCharDisplay, aui1_ttx_data);

                            if (_ParseTTX_Info.bEnPaketCount)
                            {
                                _ParseTTX_Info.u1PagePktCnt ++;
                            }
                        }
                                bLastisPk0 = FALSE;
                    }
                    //Pkt 0 Process
                    else 
                    {
                        if (bParallelMode) //End of The same MagNumber 
                        {
                            if (mag != pParseTTXInfo->u1MagNum)
                            {
                                goto NEXT;
                            }
                        }

                        if (bLastisPk0 == FALSE)
                        {
                            if (pParseTTXInfo->u1ChkTTXMode == 0)
                            {
                                Printf("#################   End Dump Page [%lx%x%x]   #################\n\n",mag, u1PageTen, u1PageUnit);
                                //_PrintTTX_Data(pkt, pParseTTXInfo->bEnCharDisplay, aui1_ttx_data);
                            }

                            if (_ParseTTX_Info.bEnPaketCount && _ParseTTX_Info.u1PagePktCnt != 23)
                            {
                                Printf("Fail Lose Packet - Page Pkt Count = %d\n", _ParseTTX_Info.u1PagePktCnt);
                                if(_ParseTTX_Info.bEnChkQlabPage)
                                {
                                    _ParseTTX_Info.u1FailPktCnt ++ ;
                                    //Printf("##TT_Mismatch");
                                }
                            }
                            bLastisPk0 = TRUE;
                            TTXParseState = E_Dump_Init;
                        }
                        else 
                        {
                            goto NEXT;
                        }
                        
                    }
                    break;

                case E_Dump_Stop:
                    Printf(" UnSupport to Dump this TTX Page\n");                  
                    break;

                case E_Dump_Disable:
                    break;

                }

            }
        else
        {
            TTXParseState = E_Dump_Init; // Reset to Init Page.
        }

        /* dump NICNI */
        if (pParseTTXInfo->bEnDumpNICNI)
        {
            if((mag==8) && (pkt==30))
            {
                _pkt_830_decode(&(aui1_ttx_data[2]));

                switch (aui1_ttx_data[2])
                {
                    case 0: /* 8/30 format 1 */
                    case 1:	                                       
                        ui2_ni = ((UINT16)((HEX_REVERSE_TABLE2[aui1_ttx_data[9] & 0xf] << 4) | HEX_REVERSE_TABLE2[aui1_ttx_data[9] >> 4]) << 8) | ((HEX_REVERSE_TABLE2[aui1_ttx_data[10] & 0xf] << 4) | HEX_REVERSE_TABLE2[aui1_ttx_data[10] >> 4]);
                        Printf("TTX NI code:%x\n",ui2_ni);         	                             
                    break;
                    case 2: /* 8/30 format 2 */
                    case 3:	                           		                           	          
                        ui2_cni = ((UINT16)((HEX_REVERSE_TABLE2[aui1_ttx_data[11]] << 4) | ((HEX_REVERSE_TABLE2[aui1_ttx_data[17]] & 0x3) << 2) | (HEX_REVERSE_TABLE2[aui1_ttx_data[18]] >> 2)) << 8) | (((HEX_REVERSE_TABLE2[aui1_ttx_data[12]] & 0xc) << 4) | ((HEX_REVERSE_TABLE2[aui1_ttx_data[18]] & 0x3) << 4) | (HEX_REVERSE_TABLE2[aui1_ttx_data[19]]));
                        Printf("TTX CNI code:%x\n",ui2_cni);     
                    break;
                }
            }
        }

        
NEXT:
        pbRdr += 48 ;
    }

    return TRUE;
    
}

void _vVBI_VPSNotify(UINT8 au1Data[])
{
    UINT8 u1Idx;

    if (_ParseTTX_Info.bEnDumpNICNI)
    {
        UINT16 u2CNI;
        //b12 - b16  (enhancement VPS): Byte(5-3) bit 0-3
        //b8  - b11  (country)        : Byte(13-3)bit 0-1 and Byte(14-3) bit 6-7
        //b0  - b7   (network)        : Byte(11-3)bit 6-7 and Byte(14-3) bit 0-5  
        u2CNI = ((BYTE_IVS(au1Data[2])&0x0F)<<12) | 
                ((((BYTE_IVS(au1Data[10])&0x3)<<2) | ((BYTE_IVS(au1Data[11])&0xC0)>>6))<<8) |
                ((BYTE_IVS(au1Data[8])&0xC0) | (BYTE_IVS(au1Data[11])&0x3F));
                
        Printf("VPS CNI = 0x%x \n", u2CNI);
    }
    else
    {
        for (u1Idx=0; u1Idx<13; u1Idx++)
        {
            Printf(" 0x%x ", au1Data[u1Idx]);
        }
        Printf("0x%x\n", au1Data[u1Idx]);
    }
}

static INT32 _i4VBI_VerVPS(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;
    static BOOL _bPastOnOff = TRUE;

    if (i4Argc != 2)
    {
        Printf("n.vbi.vvps 2 --> Enable VBI VPS CLI notification with MW path\n");
        Printf("n.vbi.vvps 1 --> Enable VBI VPS CLI notification only\n");
        Printf("n.vbi.vvps 0 --> Disable VBI VPS CLI notification \n");
        return 0;
    }

    u1OnOff = StrToInt(szArgv[1]);

    if (u1OnOff)
    {
        _bPastOnOff = VBI_QueryNotifyOnOffStatus(VBI_VPS_SERVICE, SV_VP_MAIN);        
        VBI_StartNotifyVPSOnOff(SV_ON, NULL);
        VBI_SetCLINotifyVPSFP(_vVBI_VPSNotify);        
    }
    else
    {
        VBI_StartNotifyVPSOnOff(_bPastOnOff, NULL);
        VBI_SetCLINotifyVPSFP(NULL);        
    }
    VBI_SetCLINotifyMode(u1OnOff);

    return 0;
}

void _vVBI_TTXNotify(UINT8 u1Path, UINT32 u4NumPktAvail, UINT32 u4VbiReadPtr)
{

    if (_ParseTTX_Info.bEnCharDisplay || 
        _ParseTTX_Info.bEnDumpNICNI   || 
        _ParseTTX_Info.u1ChkTTXMode   || 
        _ParseTTX_Info.bEnChkHam84_B01|| 
        _ParseTTX_Info.bEnChkHam84_B29||
        _ParseTTX_Info.bEnChkParity)
    {    
        _vVBI_ParseTTXPage(&_ParseTTX_Info ,u4NumPktAvail, u4VbiReadPtr);
    }
    else
    {
        Printf("Received TTX %ld Pkt from Path %d\n", u4NumPktAvail, u1Path);
    }
    
    /* For IC Verifed */
    #ifdef CHKPKT_INDEX
    UINT8 u1Index;
    UINT8 *pu1Data;
    for (u1Index = 0; u1Index < u4NumPktAvail; u1Index++)
    {
        pu1Data = (UINT8 *)u4VbiReadPtr;
        //Print First Byte
        Printf(" %2x", pu1Data[2]);
        u4VbiReadPtr += 48 ;
    }
    Printf("\n");
    #endif
}

static INT32 _i4VBI_VerTTX(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 ulPath;
    UINT8 u1OnOff;
    static BOOL _bPastOnOff = TRUE;

	if (i4Argc != 3)
  	{
        Printf("n.vbi.vttx 0 2 --> Enable Main Path TTX CLI notification with MW path \n");
        Printf("n.vbi.vttx 0 1 --> Enable Main Path TTX CLI notification only \n");
        Printf("n.vbi.vttx 0 0 --> Disable Main Path TTX CLI notification \n");
        Printf("n.vbi.vttx 2 1 --> Enable Sub Path CLI notification \n");
        Printf("n.vbi.vttx 2 0 --> Disable Sub Path TTX CLI notification \n");
        return 0;
  	}

    if (StrToInt(szArgv[1]))
        ulPath = SV_VP_PIP;
    else
        ulPath = SV_VP_MAIN;

    u1OnOff = StrToInt(szArgv[2]);

    if (u1OnOff)
    {
        _bPastOnOff = VBI_QueryNotifyOnOffStatus(VBI_TTX_SERVICE, ulPath);        
        VBI_StartNotifyTTXOnOff(ulPath, SV_ON, NULL);
        VBI_SetCLINotifyTTXFP(_vVBI_TTXNotify);
    }
    else
    {
        TTXParseState = E_Dump_Init;
        _ParseTTX_Info.u1FailPktCnt = 0;
        VBI_StartNotifyTTXOnOff(ulPath, _bPastOnOff, NULL);
        VBI_SetCLINotifyTTXFP(NULL);
    }
    VBI_SetCLINotifyMode(u1OnOff);

    return 0;
}

static INT32 _i4VBI_VBIRegDump(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DumpFrom;
    UINT8 u1DumpTo;
    UINT8 u1i;
    UINT32 u4RegAddr;
    UINT32 u4WriteAddr;
    
    switch (i4Argc)
    {
        case 3:
            u1DumpFrom = StrToInt(szArgv[1]);
            u1DumpTo   = StrToInt(szArgv[2]);
            //Dump VBI 0 Register to VBI 2
            if ((u1DumpFrom == 0) && (u1DumpTo == 2)) 
            {
                for (u1i = 0; u1i<=0xD; u1i++)
                {
                    u4RegAddr = VBI_00+(u1i*4);
                    u4WriteAddr = VBI2_00+(u1i*4);
                    vHwVBIWriteReg(u4WriteAddr, vHwVBIReadReg(u4RegAddr));
                }
            }
            //Dump VBI 2 Register to VBI 0
            else if ((u1DumpFrom == 2) && (u1DumpTo == 0))
            {
                for (u1i = 0; u1i<=0xD; u1i++)
                {
                    u4RegAddr = VBI2_00+(u1i*4);
                    u4WriteAddr = VBI_00+(u1i*4);
                    vHwVBIWriteReg(u4WriteAddr, vHwVBIReadReg(u4RegAddr));
                }
            }
            else
            {
                goto EXIT;
            }
            return 0;

        case 2: 
            u1DumpFrom = StrToInt(szArgv[1]);
            if (u1DumpFrom == 0)
            {
                Printf("VBI 0 Control Registers\n");
                for (u1i = 0; u1i<=0xD; u1i++)
                {
                    u4RegAddr = VBI_00+(u1i*4);
                    Printf("VBI0 0x%lx register = 0x%08lx \n",u4RegAddr, vHwVBIReadReg(u4RegAddr));
                }
                Printf("VBI 0 Status Registers\n");
                for (u1i = 0; u1i<=0x7; u1i++)
                {
                    u4RegAddr = VBI_STA_00+(u1i*4);
                    Printf("VBI0 STA 0x%lx register = 0x%08lx \n",u4RegAddr, vHwVBIReadReg(u4RegAddr));
                }
            }
            else if (u1DumpFrom == 2)
            {
                Printf("VBI 2 Control Registers\n");
                for (u1i = 0; u1i<=0xD; u1i++)
                {
                    u4RegAddr = VBI2_00+(u1i*4);
                    Printf("VBI2 0x%lx register = 0x%08lx \n",u4RegAddr, vHwVBIReadReg(u4RegAddr));
                }
                Printf("VBI 2 Status Registers\n");
                for (u1i = 0; u1i<=0x7; u1i++)
                {
                    u4RegAddr = VBI2_STA_00+(u1i*4);
                    Printf("VBI2 STA 0x%lx register = 0x%08lx \n",u4RegAddr, vHwVBIReadReg(u4RegAddr));
                }
            }
            else
            {
                goto EXIT;
            }
            return 0;
            
        default: 
            goto EXIT;


    }
    
EXIT: 
    Printf("n.vbi.dR 0 --> Dump Vbi0 register \n");
    Printf("n.vbi.dR 2 --> Dump Vbi2 register \n");
    Printf("n.vbi.dR 0 2 --> Dump Vbi0 register setting to Vbi2 \n");
    Printf("n.vbi.dR 2 0 --> Dump Vbi2 register setting to Vbi0 \n");
    return 0;
}

static void _VBI_Pfn_CLI_Nfy (void *pInput)
{
    UINT32 u4regValue;
    static UINT16 u2Counter;

    u2Counter ++ ;
    switch ( _MonVbiInfo.MonType)
    {
        case E_Mon_GD_Status: 
            if (_MonVbiInfo.u1MonSpeed)
            {
                if ((u2Counter % _MonVbiInfo.u1MonSpeed) == 0)
                {
                    // VBI_STA_01 Bit 31~21 is GD statsu
                    u4regValue = vHwVBIReadReg(VBI_STA_01);
                    Printf(" GD Status = 0x%03lx\n", (u4regValue >> 21));
                }
            }
            break;

        case E_Mon_Register: 
            if (_MonVbiInfo.u1MonSpeed)
            {
                if ((u2Counter % _MonVbiInfo.u1MonSpeed) == 0)
                {
                    // Monitor specific register. 
                    u4regValue = *((volatile UINT32 *) _MonVbiInfo.u4RegisterAddr);
                    Printf(" Addr[0x%lx] = 0x%08lx\n",_MonVbiInfo.u4RegisterAddr, u4regValue);
                }
            }
            break;
            
        case E_Mon_UnKnow:
            break;
        default: 
            break;
    }

}

static INT32 _i4VBI_Mon_VBI_Status(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Input, u4Len;
    UINT8 u1Speed;



    switch (i4Argc)
    {
        case 2: 
            u4Input = StrToInt(szArgv[1]);
            if (u4Input == 0)
            {
                VBI_SetCLINotifyPFN(NULL);
                _MonVbiInfo.MonType = E_Mon_UnKnow;
            }
            else
            {
                goto EXIT;
            }
            break;
            
        case 3: 
            u4Input = StrToInt(szArgv[1]);
            u1Speed = StrToInt(szArgv[2]);
            if ((u4Input == 1) && (u1Speed > 0))
            {
                VBI_SetCLINotifyPFN(_VBI_Pfn_CLI_Nfy);
                _MonVbiInfo.MonType = E_Mon_GD_Status; 
                _MonVbiInfo.u1MonSpeed = u1Speed; 
            }
            else if (u1Speed > 0)
            {
            	u4Len = x_strlen(szArgv[1]);
            	if (u4Len > 2)
            	{
            		if ((szArgv[1][0] == '0') && (szArgv[1][1] == 'x'))
        		    {
                        VBI_SetCLINotifyPFN(_VBI_Pfn_CLI_Nfy);
                        _MonVbiInfo.MonType = E_Mon_Register; 
                        _MonVbiInfo.u4RegisterAddr = u4Input;
                        _MonVbiInfo.u1MonSpeed = u1Speed; 
        		    }
        	    }
            }
            else
            {
                goto EXIT;
            }
            break;


        default: 
            goto EXIT;  
    }
    
    return 0;
EXIT: 
    Printf("n.vbi.mvbi 0 --> Disable Monitor VBI Status\n");
    Printf("n.vbi.mvbi 1 32 --> Monitor VBI GD LMS DC Status, Speed: 32 Vsync 1 log\n");
    Printf("n.vbi.mvbi 0x20022240 64 --> Monitor 0x20022240 register, Speed: 64 Vsync 1 log\n");
    return 0;

}



static void _VBI_CLI_Nfy_ChkService (void *pInput)
{
    UINT32 u4CurField;

    if (_u1Suite == VBI0)
    {
        u4CurField = (u4GetVbiField() == 0) ? 0 : 1;
    }
    else 
    {
        u4CurField = (u4GetVbi2Field() == 0) ? 0 : 1;
    }

    
    if (_u4ChkField == u4CurField)
    {
        if ((_u1Suite == VBI0) && IsVBI0ISR())
        {
            switch (_eChkType)
            {
                case VBI_TTX_SERVICE:
                    if (IsTTRdy())
                    {
                        _u1SuccNo++;
                    }
                    break;
                case VBI_CC_SERVICE:
                    if (IsCCRdy())
                    {
                        _u1SuccNo++;
                    }
                    break;
                case VBI_CC2X_SERVICE:
                    if (IsCC2XRdy())
                    {
                        _u1SuccNo++;
                    }
                    break;
                case VBI_WSS525_SERVICE:
                case VBI_WSS625_SERVICE:
                    if (IsWSSRdy())
                    {
                        _u1SuccNo++;
                    }
                    break;
                default:
                    break;
            }
            _u1TestNo++;

        }
        else if ((_u1Suite == VBI2)&& IsVBI2ISR())
        {
            switch (_eChkType)
            {
                case VBI_TTX_SERVICE:
                    if (IsTT2Rdy())
                    {
                        _u1SuccNo++;
                    }
                    break;
                case VBI_CC_SERVICE:
                    if (IsCC2Rdy())
                    {
                        _u1SuccNo++;
                    }
                    break;
                case VBI_WSS525_SERVICE:
                case VBI_WSS625_SERVICE:
                    if (IsWSS2Rdy())
                    {
                        _u1SuccNo++;
                    }
                    break;
                case VBI_VPS_SERVICE:
                    if (IsVPS2Rdy())
                    {
                        _u1SuccNo++;
                    }
                    break;

                default:
                    break;
            }
            _u1TestNo++;

        }
        
    }
    
}

static void _vSetScanLine(UINT8         u1Suite,
                          UINT32        u4ChkField, 
                          UINT8         u1ChkLine, 
                          VBI_SERVICE_T eVBIType)
{
    switch (eVBIType)
    {
        case VBI_CC2X_SERVICE:
        case VBI_TTX_SERVICE:
            if (u4ChkField == 0)
            {
                vHwVbiSetTTInterval0(u1Suite, u1ChkLine, u1ChkLine);
            }
            else if(u4ChkField == 1)
            {
                vHwVbiSetTTInterval1(u1Suite, u1ChkLine, u1ChkLine);
            }
            break;
            
        case VBI_CC_SERVICE:
            if (u4ChkField == 0)
            {
                vHwVbiSetCCLine(u1Suite, u1ChkLine, 0x1);
            }
            else if(u4ChkField == 1)
            {
                vHwVbiSetCCLine(u1Suite, 0x1, u1ChkLine);
            }
            break;
            
        case VBI_WSS525_SERVICE:
        case VBI_WSS625_SERVICE:
            if (u4ChkField == 0)
            {
                vHwVbiSetWSSLine(u1Suite, u1ChkLine, 0x1);
            }
            else if(u4ChkField == 1)
            {
                vHwVbiSetWSSLine(u1Suite, 0x1, u1ChkLine);
            }
            break;
            
        case VBI_VPS_SERVICE:
            vHwVbiSetVPSInterval(VBI2, u1ChkLine, u1ChkLine);
            break;
            
        default:
            break;
    }
}


static void _vScanVbiLine(UINT8 u1Suite, UINT32 u4ChkField, VBI_SERVICE_T eVBIType)
{

    static UINT32 u4MaxCnt = 0x200;
    static UINT8  u1ScanLine = SCAN_LINE_START;  //Scan Line Range from Line4 ~ Line 25
    static ScanLState_T ScanState = E_Init;
    while(u4MaxCnt--)
    {
        switch (ScanState)
        {
            case E_Init:
                Printf("Scan Init\n");
                _u4ChkField = u4ChkField;
                _eChkType = eVBIType;
                _u1Suite = u1Suite;
                _vSetScanLine(_u1Suite, _u4ChkField, u1ScanLine, _eChkType);
                ScanState = E_WaitScan;
                VBI_SetCLINotifyPFN(_VBI_CLI_Nfy_ChkService);
                break;
            case E_ScanNextL:
                Printf("[Field, Line, Success, Total] = [%ld,%d,%d,%d]\n",
                        _u4ChkField,
                        u1ScanLine,
                        _u1SuccNo,
                        _u1TestNo);
                VBI_SetCLINotifyPFN(NULL);
                _u1SuccNo = 0;
                _u1TestNo = 0;
                if (u1ScanLine < SCAN_LINE_END)
                {
                    u1ScanLine ++;
                    _vSetScanLine(_u1Suite, _u4ChkField, u1ScanLine, eVBIType);
                    VBI_SetCLINotifyPFN(_VBI_CLI_Nfy_ChkService);
                    ScanState = E_WaitScan;

                }
                else
                {
                    ScanState = E_Finish;
                }
                break;
            case E_WaitScan:
                //Printf("Scan Wait \n");
                if (_u1TestNo > SCAN_TIMES)
                {
                    ScanState = E_ScanNextL;
                }
               
                break;
            case E_Finish:
                Printf("Scan Finish\n");
                VBI_SetCLINotifyPFN(NULL);
                _u4ChkField = 0xFF;
                _eChkType = VBI_SERVICE_NUM;
                _u1Suite = 0xFF;
                goto EXIT;
        }
        x_thread_delay(50);
    }

EXIT:
    ScanState = E_Init;
    u1ScanLine = SCAN_LINE_START;
    u4MaxCnt = 0x200;
    _u1SuccNo = 0;
    _u1TestNo = 0;
    return;

}


static void _vScanVbiRange(UINT8 u1Suite, 
                            UINT32 u4ChkField, 
                            VBI_SERVICE_T eVBIType, 
                            const UINT32 u4ConReg, 
                            UINT8 u1Starbit, 
                            UINT8 u1EndBit,
                            UINT8 u1RegRange)
{
    static ScanLState_T eScanRageState = E_Init;
    static UINT32 u4MaxCnt = 0x400;
    static UINT8  u1ScanRegVal = 0;  //Scan Line Range from Line4 ~ Line 25
    static UINT32 u4OriRegValue = 0;
    UINT32 u4BitRange = 1<<((u1EndBit - u1Starbit)+1); //ex: bit3~6: 2<<((6-3)+1) = 0x10
    UINT32 u4RegMask = (u4BitRange-1) << (u1Starbit); //ex: bit3~6: 0xF<<3 = 0x78
    static UINT32 u4TestRegValue = 0;

    u4OriRegValue = (*(volatile UINT32 *)u4ConReg);
    u4TestRegValue = (u4OriRegValue & u4RegMask) >> u1Starbit;
    u4TestRegValue -= u1RegRange;

    while(u4MaxCnt--)
    {
        switch (eScanRageState)
        {
            case E_Init:
                Printf("Scan Init\n");
                _u4ChkField = u4ChkField;
                _eChkType = eVBIType;
                _u1Suite = u1Suite;
                eScanRageState = E_WaitScan;
                (*(volatile UINT32 *)u4ConReg) = (*(volatile UINT32 *)u4ConReg & ~u4RegMask);
                (*(volatile UINT32 *)u4ConReg) = (*(volatile UINT32 *)u4ConReg | (u4TestRegValue<<u1Starbit));
                VBI_SetCLINotifyPFN(_VBI_CLI_Nfy_ChkService);
                break;
            case E_ScanNextL:
                Printf("[Field, TestVal, RegVal, Success, Total] = [%ld,0x%lx, 0x%lx, %d,%d]\n",
                        _u4ChkField,
                        u4TestRegValue,
                        (*(volatile UINT32 *)u4ConReg),
                        _u1SuccNo,
                        _u1TestNo);
                VBI_SetCLINotifyPFN(NULL);
                _u1SuccNo = 0;
                _u1TestNo = 0;
                if (u1ScanRegVal < (u1RegRange*2))
                {
                    u1ScanRegVal ++;
                    (*(volatile UINT32 *)u4ConReg) = (*(volatile UINT32 *)u4ConReg & ~u4RegMask);
                    u4TestRegValue ++;
                    (*(volatile UINT32 *)u4ConReg) = (*(volatile UINT32 *)u4ConReg | (u4TestRegValue<<u1Starbit));
                    VBI_SetCLINotifyPFN(_VBI_CLI_Nfy_ChkService);
                    
                    eScanRageState = E_WaitScan;
                }
                else
                {
                    eScanRageState = E_Finish;
                }
                break;
            case E_WaitScan:
                //Printf("Scan Wait \n");
                if (_u1TestNo > SCAN_TIMES)
                {
                    eScanRageState = E_ScanNextL;
                }
               
                break;
            case E_Finish:
                Printf("Scan Finish\n");
                VBI_SetCLINotifyPFN(NULL);
                _u4ChkField = 0xFF;
                _eChkType = VBI_SERVICE_NUM;
                _u1Suite = 0xFF;
                goto EXIT;
        }
        x_thread_delay(50);
    }

EXIT:
    eScanRageState = E_Init;
    u1ScanRegVal = 0;
    u4MaxCnt = 0x400;
    _u1SuccNo = 0;
    _u1TestNo = 0;
    (*(volatile UINT32 *)u4ConReg) = u4OriRegValue;
    return;

}


static INT32 _i4VBI_ScanVBILine(INT32 i4Argc, const CHAR **szArgv)
{
    static VBI_MODE_T ePreVbiMode;
    UINT8 u1Suite;
    UINT32 u4NewLineEn;
    VBI_MODE_T eNewMode;
    VBI_SERVICE_T eChkType;
    
    //Get Previous VBI Mode And Line Setting. 
    ePreVbiMode = VBI_GetVBIMode();

    switch (i4Argc)
    {
        case 2: 
            if (x_strcmp(szArgv[1], "cc2x") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = TT_LINE_EN;
                u1Suite = VBI0;
                eChkType = VBI_CC2X_SERVICE;
            }
            else if (x_strcmp(szArgv[1], "vps") == 0)
            {
                eNewMode = VBI_ANAS_MODE;
                u4NewLineEn = VPS_LINE_EN;
                u1Suite = VBI2;
                eChkType = VBI_VPS_SERVICE;
            }
            else
            {
                goto EXIT;
            }
            break;

        case 3: 
            //Set Check VBI Slicer Type
            u1Suite = StrToInt(szArgv[1]);
            u1Suite = (u1Suite == 0) ? VBI0 : VBI2;
            //Check Mode setting.
            if (x_strcmp(szArgv[2], "ttx") == 0)
            {
                eNewMode = VBI_TTX_MODE;
                u4NewLineEn = TT_LINE_EN;
                eChkType = VBI_TTX_SERVICE;
            }
            else if (x_strcmp(szArgv[2], "cc") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = CC_LINE_EN;
                eChkType = VBI_CC_SERVICE;
            }
            else if (x_strcmp(szArgv[2], "wss525") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = WSS_LINE_EN;
                eChkType = VBI_WSS525_SERVICE;                
            }
            else if (x_strcmp(szArgv[2], "wss625") == 0)
            {
                eNewMode = VBI_TTX_MODE;
                u4NewLineEn = WSS_LINE_EN;
                eChkType = VBI_WSS625_SERVICE;               
            }
            else
            {
                goto EXIT;
            }
            break;

        default:
            goto EXIT;

    }
    //Init Scan Setting
    VBI_Init(eNewMode);
    VBI_ResetSrc(SV_VP_MAIN);
    VBI_SetLineEn(u1Suite, u4NewLineEn);

    //Process Scan Line
    _vScanVbiLine(u1Suite, 0, eChkType);
    _vScanVbiLine(u1Suite, 1, eChkType);

    //Restore Original Setting.
    VBI_Init(ePreVbiMode);
    VBI_ResetSrc(SV_VP_MAIN);

    return 0;

EXIT:
    Printf("n.vbi.scanline 0 ttx    --> Scan VBI0 TTX Line\n");
    Printf("n.vbi.scanline 0 cc     --> Scan VBI0 CC Line\n");
    Printf("n.vbi.scanline 0 wss525 --> Scan VBI0 WSS525 Line\n");
    Printf("n.vbi.scanline 1 wss625 --> Scan VBI2 WSS625 Line\n");
    Printf("n.vbi.scanline cc2x     --> Scan VBI0 CC2X Line\n");
    Printf("n.vbi.scanline vps      --> Scan VBI2 VPS Line\n");
    return 0;
}


static INT32 _i4VBI_ScanRange(INT32 i4Argc, const CHAR **szArgv)
{
    static VBI_MODE_T ePreVbiMode;
    UINT8 u1Suite, u1starbit, u1stopbit, u1RegRange;
    INT16  i2bitragne;
    UINT32 u4NewLineEn;
    UINT32 u4RegAddr, u4Len;
    VBI_MODE_T eNewMode;
    VBI_SERVICE_T eChkType;
    
    //Get Previous VBI Mode And Line Setting. 
    ePreVbiMode = VBI_GetVBIMode();

    switch (i4Argc)
    {
        case 6: 
            u1starbit = StrToInt(szArgv[3]);
            u1stopbit = StrToInt(szArgv[4]);
            u1RegRange = StrToInt(szArgv[5]);
            i2bitragne = (u1stopbit - u1starbit)+1; 
            if (i2bitragne <= 0 || u1RegRange <= 0)
            {
                goto EXIT;
            }

        	u4Len = x_strlen(szArgv[2]);
        	if (u4Len > 2)
        	{
        		if ((szArgv[2][0] == '0') && (szArgv[2][1] == 'x'))
    		    {
                    u4RegAddr = StrToInt(szArgv[2]);
    		    }
        		else
    		    {
                    goto EXIT;
    		    }
    	    }
        	else
    	    {
                goto EXIT;
    	    }

            if (x_strcmp(szArgv[1], "cc2x") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = TT_LINE_EN;
                u1Suite = VBI0;
                eChkType = VBI_CC2X_SERVICE;                
            }
            else if (x_strcmp(szArgv[1], "vps") == 0)
            {
                eNewMode = VBI_ANAS_MODE;
                u4NewLineEn = VPS_LINE_EN;
                u1Suite = VBI2;
                eChkType = VBI_VPS_SERVICE;
            }
            else if (x_strcmp(szArgv[1], "ttx[0]") == 0)
            {
                eNewMode = VBI_TTX_MODE;
                u4NewLineEn = TT_LINE_EN;
                eChkType = VBI_TTX_SERVICE;
                u1Suite = VBI0;
            }
            else if (x_strcmp(szArgv[1], "ttx[1]") == 0)
            {
                eNewMode = VBI_TTX_MODE;
                u4NewLineEn = TT_LINE_EN;
                eChkType = VBI_TTX_SERVICE;
                u1Suite = VBI2;
            }
            else if (x_strcmp(szArgv[1], "cc[0]") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = CC_LINE_EN;
                eChkType = VBI_CC_SERVICE;
                u1Suite = VBI0;
            }
            else if (x_strcmp(szArgv[1], "cc[1]") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = CC_LINE_EN;
                eChkType = VBI_CC_SERVICE;
                u1Suite = VBI2;
            }
            else if (x_strcmp(szArgv[1], "wss525[0]") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = CC_LINE_EN;
                eChkType = VBI_CC_SERVICE;
                u1Suite = VBI0;
            }
            else if (x_strcmp(szArgv[1], "wss525[1]") == 0)
            {
                eNewMode = VBI_CC_VCHIP_MODE;
                u4NewLineEn = WSS_LINE_EN;
                eChkType = VBI_WSS525_SERVICE;  
                u1Suite = VBI2;
            }
            else if (x_strcmp(szArgv[1], "wss625[0]") == 0)
            {
                eNewMode = VBI_TTX_MODE;
                u4NewLineEn = WSS_LINE_EN;
                eChkType = VBI_WSS625_SERVICE;               
                u1Suite = VBI0;
            }
            else if (x_strcmp(szArgv[1], "wss625[1]") == 0)
            {
                eNewMode = VBI_TTX_MODE;
                u4NewLineEn = WSS_LINE_EN;
                eChkType = VBI_WSS625_SERVICE;               
                u1Suite = VBI2;
            }
            else
            {
                goto EXIT;
            }
            break;
        default:
            goto EXIT;

    }
    //Init Scan Setting
    VBI_Init(eNewMode);
    VBI_ResetSrc(SV_VP_MAIN);
    VBI_SetLineEn(u1Suite, u4NewLineEn);

    //Process Scan Range
    _vScanVbiRange(u1Suite, 0, eChkType, u4RegAddr, u1starbit, u1stopbit, u1RegRange);
    _vScanVbiRange(u1Suite, 1, eChkType, u4RegAddr, u1starbit, u1stopbit, u1RegRange);

    //Restore Original Setting.
    VBI_Init(ePreVbiMode);
    VBI_ResetSrc(SV_VP_MAIN);

    return 0;

EXIT:
    Printf("n.vbi.srange ttx[0]     0x200228c0 20 27  32 --> Scan VBI0 TTX 0x200228c0 bit20~bit27 range Def value +- 32\n");
    Printf("n.vbi.srange cc[0]      0x200228c0  0  3  64 --> Scan VBI0 CC  0x200228c0 bit0~bit3 range Def value +- 64\n");
    Printf("n.vbi.srange wss525[0]  0x200228c0 20 27  16 --> Scan VBI0 WSS525  0x200228c0 bit20~bit27 Def value +- 16\n");
    Printf("n.vbi.srange wss625[1]  0x200228c0 20 27  64 --> Scan VBI2 WSS625  0x200228c0 bit20~bit27 Def value +- 64\n");
    Printf("n.vbi.srange cc2x       0x200228c0 20 27  32 --> Scan VBI0 CC2X  0x200228c0 bit20~bit27 Def value +- 32\n");
    Printf("n.vbi.srange vps        0x200228c0 20 27  16 --> Scan VBI2 VPS  0x200228c0 bit20~bit27 Def value +- 16\n");
    return 0;
}

static INT32 _i4VBI_AdjGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Gain;

    if (i4Argc == 2)
    {
        u1Gain = StrToInt(szArgv[1]);
        if (VBI_AdjVBIGain(u1Gain))
        {
            Printf("Set VBI Gain [%d] ready\n", u1Gain);
        }
        return 0;
    }
    else 
    {
        goto EXIT;
    }

EXIT:
    Printf("n.vbi.adjgain 0~F  --> Adjust VBI(TVD path) gain from 0 to 3.75\n");

    return 0;
}

#if AUTO_CC_DETECT
static INT32 _i4VBI_AutoCCOnOff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1OnOff;
     if (i4Argc != 2)
    {
        Printf("n.vbi.autocc 1 --> Enable VBI Auto CC Detection\n");
        Printf("n.vbi.autocc 0 --> Disable VBI Auto CC Detection\n");
        return 0;
    }
    
    u1OnOff = StrToInt(szArgv[1]);
    VBI_AutoCCOnOff(u1OnOff);
    return 0;
}
#endif

#endif  //#ifdef VBI_CLI_SUPPORT

