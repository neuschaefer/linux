/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: imgrz_dif_reg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_dif_reg.h
 *  image resizer driver hardware registers
 */

/* 
    lint inhibition(s)
        46   : suppress MISRA rule 111, field type should be int
        960  : suppress MISRA rule 110, bitfields inside union
               here size of bitfield has been carefully calculated 
               to fit union size
*/

/*lint -save -e46 -e960 */

#ifndef IMGRZ_DIF_REG_H
#define IMGRZ_DIF_REG_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// size of imgrz register space (in unit sizeof(uint32))
#define IMGRZ_REG_FILE_SIZE     (INT32)(RZ_REG_END)

#define IMGRZ_FIELD_MODE        1
#define IMGRZ_FRAME_MODE        0

#define IMGRZ_CMD_BUF_CYLIC     0
#define IMGRZ_CMD_BUF_SINGLE    1

#define IMGRZ_SOFT_RESET        3

#define IMGRZ_HW_IDLE           1
#define IMGRZ_HW_BUSY           0

#define IMGRZ_ENGINE_FIRE       1


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// hardware register enumeration
typedef enum _E_RZ_REG_T
{
    RZ_REG_RZSTART       = 0x00,     // 0x20000
    RZ_REG_RZTYPE        = 0x01,     // 0x20004
    RZ_REG_RZJPGMODE     = 0x02,     // 0x20008
    RZ_REG_RZMIFMODE     = 0x03,     // 0x2000C
    RZ_REG_RZSRCBUFL     = 0x04,     // 0x20010
    RZ_REG_RZJPGINTF     = 0x05,     // 0x20014
    RZ_REG_RZTGBUFL      = 0x06,     // 0x20018
    RZ_REG_RZSRCYB1      = 0x07,     // 0x2001C
    RZ_REG_RZSRCYB2      = 0x08,     // 0x20020
    RZ_REG_RZSRCCBB1     = 0x09,     // 0x20024
    RZ_REG_RZSRCCBB2     = 0x0A,     // 0x20028
    RZ_REG_RZSRCCRB1     = 0x0B,     // 0x2002C
    RZ_REG_RZSRCCRB2     = 0x0C,     // 0x20030
    RZ_REG_RZTGYB        = 0x0D,     // 0x20034
    RZ_REG_RZTGCB        = 0x0E,     // 0x20038
    RZ_REG_0x2003C       = 0x0F,     // 0x2003C
    RZ_REG_RZSRCSZY      = 0x10,     // 0x20040
    RZ_REG_RZSRCSZCB     = 0x11,     // 0x20044
    RZ_REG_RZSRCSZCR     = 0x12,     // 0x20048
    RZ_REG_RZTGSZ        = 0x13,     // 0x2004C
    RZ_REG_0x20050       = 0x14,     // 0x20050
    RZ_REG_RZSRCOFTY     = 0x15,     // 0x20054
    RZ_REG_RZSRCOFTCB    = 0x16,     // 0x20058
    RZ_REG_RZSRCOFTCR    = 0x17,     // 0x2005C
    RZ_REG_RZTGOFT       = 0x18,     // 0x20060
#ifdef CC_MT5363
    RZ_REG_RZH8TAPFACY   = 0x19,     // 0x20064
    RZ_REG_RZH8TAPFACCB  = 0x1A,     // 0x20068
    RZ_REG_RZH8TAPFACCR  = 0x1B,     // 0x2006C
#else
    RZ_REG_RZH8TAPY      = 0x19,     // 0x20064
    RZ_REG_RZH8TAPCB     = 0x1A,     // 0x20068
    RZ_REG_RZH8TAPCR     = 0x1B,     // 0x2006C
#endif    
    RZ_REG_RZHSAY        = 0x1C,     // 0x20070
    RZ_REG_RZHSACB       = 0x1D,     // 0x20074
    RZ_REG_RZHSACR       = 0x1E,     // 0x20078
    RZ_REG_RZVSCLY       = 0x1F,     // 0x2007C
    RZ_REG_RZVSCLCB      = 0x20,     // 0x20080
    RZ_REG_RZVSCLCR      = 0x21,     // 0x20084
#ifdef CC_MT5363
    RZ_REG_RZYV4TAPFAC   = 0x22,     // 0x20088
    RZ_REG_RZCV4TAPFAC   = 0x23,     // 0x2008C
    RZ_REG_0x20090       = 0x24,     // 0x20090
#else
    RZ_REG_RZV4TAPY      = 0x22,     // 0x20088
    RZ_REG_RZV4TAPCB     = 0x23,     // 0x2008C
    RZ_REG_RZV4TAPCR     = 0x24,     // 0x20090
#endif    
    RZ_REG_RZTMPB        = 0x25,     // 0x20094
#if 1//defined (CC_MT5387) || defined (CC_MT5363)   
    RZ_REG_RZPRELOADY    = 0x26,     // 0x20098
    RZ_REG_RZPRELOADC    = 0x27,     // 0x2009c
    RZ_REG_RESUME        = 0x28,     // 0x200A0
    RZ_REG_SRCCNTY       = 0x29,     // 0x200A4
    RZ_REG_SRCCNTCB      = 0x2A,     // 0x200A8
#else
    RZ_REG_RZJPGVY       = 0x26,     // 0x20098
    RZ_REG_RZJPGVCB      = 0x27,     // 0x2009C
    RZ_REG_RZJPGVCR      = 0x28,     // 0x200A0
    RZ_REG_RZPRELOADY    = 0x29,     // 0x200A4
    RZ_REG_RZPRELOADC    = 0x2A,     // 0x200A8
#endif        
    RZ_REG_SRCCNTCR      = 0x2B,     // 0x200AC
    RZ_REG_VWROFTY       = 0x2C,     // 0x200B0
    RZ_REG_VWROFTCB      = 0x2D,     // 0x200B4
    RZ_REG_VWROFTCR      = 0x2E,     // 0x200B8
    RZ_REG_VNEXTCOEFY    = 0x2F,     // 0x200BC
    RZ_REG_VNEXTCOEFCB   = 0x30,     // 0x200C0
    RZ_REG_VNEXTCOEFCR   = 0x31,     // 0x200C4
    RZ_REG_YH8TAPO       = 0x32,     // 0x200C8
    RZ_REG_CBH8TAPO      = 0x33,     // 0x200CC
    RZ_REG_CRH8TAPO      = 0x34,     // 0x200D0
    RZ_REG_YV4TAPO       = 0x35,     // 0x200D4
    RZ_REG_CV4TAPO       = 0x36,     // 0x200D8
    RZ_REG_0x200DC       = 0x37,     // 0x200DC
    RZ_REG_0x200E0       = 0x38,     // 0x200E0
    RZ_REG_0x200E4       = 0x39,     // 0x200E4
#ifdef CC_MT5363
    RZ_REG_RZLINECNT   = 0x3A,     // 0x200E8
#else
    RZ_REG_RZCHKSUM      = 0x3A,     // 0x200E8
#endif
    RZ_REG_RZINTFMON     = 0x3B,     // 0x200EC
    RZ_REG_RZFSMMON      = 0x3C,     // 0x200F0
    RZ_REG_RZSTMON       = 0x3D,     // 0x200F4
    RZ_REG_RZDATAMON     = 0x3E,     // 0x200F8
    RZ_REG_RZDONE        = 0x3F,     // 0x200FC
    RZ_REG_RZOSDMODE     = 0x40,     // 0x20100
    RZ_REG_RZOSDCTRL     = 0x41,     // 0x20104
    RZ_REG_RZATABLE      = 0x42,     // 0x20108
    RZ_REG_RZOSDTRAN0    = 0x43,     // 0x2010C
    RZ_REG_RZOSDTRAN1    = 0x44,     // 0x20110
    RZ_REG_RZOSDTRAN2    = 0x45,     // 0x20114
    RZ_REG_RZOSDTRAN3    = 0x46,     // 0x20118
    RZ_REG_RZCPTADDR     = 0x47,     // 0x2011C
    RZ_REG_RZCPTDATA     = 0x48,     // 0x20120
    RZ_REG_RZHCOEFF0     = 0x49,     // 0x20124
    RZ_REG_RZHCOEFF1     = 0x4A,     // 0x20128
    RZ_REG_RZHCOEFF2     = 0x4B,     // 0x2012C
    RZ_REG_RZHCOEFF3     = 0x4C,     // 0x20130
    RZ_REG_RZHCOEFF4     = 0x4D,     // 0x20134
    RZ_REG_RZHCOEFF5     = 0x4E,     // 0x20138
    RZ_REG_RZHCOEFF6     = 0x4F,     // 0x2013C
    RZ_REG_RZHCOEFF7     = 0x50,     // 0x20140
    RZ_REG_RZHCOEFF8     = 0x51,     // 0x20144
    RZ_REG_RZHCOEFF9     = 0x52,     // 0x20148
    RZ_REG_RZHCOEFF10    = 0x53,     // 0x2014C
    RZ_REG_RZHCOEFF11    = 0x54,     // 0x20150
    RZ_REG_RZHCOEFF12    = 0x55,     // 0x20154
    RZ_REG_RZHCOEFF13    = 0x56,     // 0x20158
    RZ_REG_RZHCOEFF14    = 0x57,     // 0x2015C
    RZ_REG_RZHCOEFF15    = 0x58,     // 0x20160
    RZ_REG_RZHCOEFF16    = 0x59,     // 0x20164
    RZ_REG_RZHCOEFF17    = 0x5A,     // 0x20168
    RZ_REG_RZVCOEFF0     = 0x5B,     // 0x2016C
    RZ_REG_RZVCOEFF1     = 0x5C,     // 0x20170
    RZ_REG_RZVCOEFF2     = 0x5D,     // 0x20174
    RZ_REG_RZVCOEFF3     = 0x5E,     // 0x20178
    RZ_REG_RZVCOEFF4     = 0x5F,     // 0x2017C
    RZ_REG_RZVCOEFF5     = 0x60,     // 0x20180
    RZ_REG_RZVCOEFF6     = 0x61,     // 0x20184
    RZ_REG_RZVCOEFF7     = 0x62,     // 0x20188
    RZ_REG_RZVCOEFF8     = 0x63,     // 0x2018C
    RZ_REG_RZDITHER      = 0x64,     // 0x20190
    RZ_REG_RZCSC         = 0x65,     // 0x20194
    RZ_REG_RZCCOEFF11    = 0x66,     // 0x20198
    RZ_REG_RZCCOEFF12    = 0x67,     // 0x2019C
    RZ_REG_RZCCOEFF13    = 0x68,     // 0x201A0
    RZ_REG_RZCCOEFF21    = 0x69,     // 0x201A4
    RZ_REG_RZCCOEFF22    = 0x6A,     // 0x201A8
    RZ_REG_RZCCOEFF23    = 0x6B,     // 0x201AC
    RZ_REG_RZCCOEFF31    = 0x6C,     // 0x201B0
    RZ_REG_RZCCOEFF32    = 0x6D,     // 0x201B4
    RZ_REG_RZCCOEFF33    = 0x6E,     // 0x201B8
    RZ_REG_LUMAKEY       = 0x6F,     // 0x201BC
    RZ_REG_0x201C0       = 0x70,     // 0x201C0
#if 1//defined (CC_MT5387) || defined (CC_MT5363)
    RZ_REG_RZJPGVY       = 0x71,     // 0x201C4
    RZ_REG_RZJPGVCB      = 0x72,     // 0x201C8
    RZ_REG_RZJPGVCR      = 0x73,     // 0x201CC
#else
    RZ_REG_DRAMQ_CFG     = 0x71,     // 0x201C4
    RZ_REG_DRAMQ_STAD    = 0x72,     // 0x201C8
    RZ_REG_DRAMQ_LEN     = 0x73,     // 0x201CC
#endif

#ifdef CC_MT5363
    RZ_REG_RPR           = 0x74,     // 0x201D0
    RZ_REG_0x201D4       = 0x75,     // 0x201D4
    RZ_REG_FLIP          = 0x76,     // 0x201D8
#else
    RZ_REG_DRAMQ_RDPTR   = 0x74,     // 0x201D0
    RZ_REG_DRAMQ_EXPTR   = 0x75,     // 0x201D4
    RZ_REG_DUMMY         = 0x76,     // 0x201D8
#endif
    RZ_REG_0x201DC       = 0x77,     // 0x201DC

    RZ_REG_LAST          = 0x78,      // 0x201E0
    RZ_REG_0x201E4		= 0x79,      // 0x201E4
    RZ_REG_0x201E8		= 0x7a,      // 0x201E8
#if 1//defined (CC_MT5360B) || defined (CC_MT5392B)
    RZ_CONFIG_REG        = 0x7B,      // 0x201EC
#endif
    RZ_REG_HEC_CKSum           =0x7C, 		// 0x201F0
    RZ_REG_VSCL_CKSum          =0x7D,     // 0x201F4
    RZ_REG_COEF_CKSum          =0x7E,     // 0x201F8
    RZ_REG_MULT_CKSum          =0x7F,     // 0x201FC
    RZ_REG_ALCOM_LOOP          =0x80,     // 0x20200
    RZ_REG_ALU_CFG1            =0x81,     // 0x20204   
    RZ_REG_END          =0x82
} E_RZ_REG_T;


// register name
typedef struct _E_RZ_DIF_REG_T 
{
    UINT32 u4_RZSTART;
    UINT32 u4_RZTYPE;
    UINT32 u4_RZJPGMODE;
    UINT32 u4_RZMIFMODE;
    UINT32 u4_RZSRCBUFL;
    UINT32 u4_RZJPGINTF;
    UINT32 u4_RZTGBUFL;
    UINT32 u4_RZSRCYB1;
    UINT32 u4_RZSRCYB2;
    UINT32 u4_RZSRCCBB1;
    UINT32 u4_RZSRCCBB2;
    UINT32 u4_RZSRCCRB1;
    UINT32 u4_RZSRCCRB2;
    UINT32 u4_RZTGYB;
    UINT32 u4_RZTGCB;
    UINT32 u4_0x2003C;
    UINT32 u4_RZSRCSZY;
    UINT32 u4_RZSRCSZCB;
    UINT32 u4_RZSRCSZCR;
    UINT32 u4_RZTGSZ;
    UINT32 u4_0x20050;
    UINT32 u4_RZSRCCOFTY;
    UINT32 u4_RZSRCCOFTCB;
    UINT32 u4_RZSRCCOFTCR;
    UINT32 u4_RZTGOFT;
#ifdef CC_MT5363
    UINT32 u4_RZH8TAPFACY;
    UINT32 u4_RZH8TAPFACCB;
    UINT32 u4_RZH8TAPFACCR;
#else
    UINT32 u4_RZH8TAPY;
    UINT32 u4_RZH8TAPCB;
    UINT32 u4_RZH8TAPCR;
#endif    
    UINT32 u4_RZHSAY;
    UINT32 u4_RZHSACB;
    UINT32 u4_RZHSACR;
    UINT32 u4_RZVSCLY;
    UINT32 u4_RZVSCLCB;
    UINT32 u4_RZVSCLCR;
#ifdef CC_MT5363
    UINT32 u4_RZYV4TAPFAC;
    UINT32 u4_RZCV4TAPFAC;
    UINT32 u4_0x20090;
#else
    UINT32 u4_RZV4TAPY;
    UINT32 u4_RZV4TAPCB;
    UINT32 u4_RZV4TAPCR;
#endif    
    UINT32 u4_RZTMPB;
#if 1//defined (CC_MT5387) || defined (CC_MT5363)
    UINT32 u4_RZPRELOADY;
    UINT32 u4_RZPRELOADC;
    UINT32 u4_RZRESUME;
    UINT32 u4_RZSRCNTY;
    UINT32 u4_RZSRCNTCB;
#else
    UINT32 u4_RZJPGVY;
    UINT32 u4_RZJPGVCB;
    UINT32 u4_RZJPGVCR;
    UINT32 u4_RZPRELOADY;
    UINT32 u4_RZPRELOADC;
#endif        
    UINT32 u4_RZSRCNTCR;
    UINT32 u4_RZVWROFTY;
    UINT32 u4_RZVWROFTCB;
    UINT32 u4_RZVWROFTCR;
    UINT32 u4_RZVNEXTCOEFY;
    UINT32 u4_RZVNEXTCOEFCB;
    UINT32 u4_RZVNEXTCOEFCR;
    UINT32 u4_RZYH8TAPO;
    UINT32 u4_RZCBH8TAPO;
    UINT32 u4_RZCRH8TAPO;
    UINT32 u4_RZYV4TAPO;
    UINT32 u4_RZCV4TAPO;
    UINT32 u4_0x200DC;
    UINT32 u4_0x200E0;
    UINT32 u4_0x200E4;
    UINT32 u4_RZCHKSUM;
    UINT32 u4_RZINTFMON;
    UINT32 u4_RZFSMMON;
    UINT32 u4_RZSTMON;
    UINT32 u4_RZDATAMON;
    UINT32 u4_RZDONE;
    UINT32 u4_RZOSDMODE;
    UINT32 u4_RZOSDCTRL;
    UINT32 u4_RZATABLE;
    UINT32 u4_RZOSDTRAN0;
    UINT32 u4_RZOSDTRAN1;
    UINT32 u4_RZOSDTRAN2;
    UINT32 u4_RZOSDTRAN3;
    UINT32 u4_RZCPTADDR;
    UINT32 u4_RZCPTDATA;
    UINT32 u4_RZHCOEFF0;
    UINT32 u4_RZHCOEFF1;
    UINT32 u4_RZHCOEFF2;
    UINT32 u4_RZHCOEFF3;
    UINT32 u4_RZHCOEFF4;
    UINT32 u4_RZHCOEFF5;
    UINT32 u4_RZHCOEFF6;
    UINT32 u4_RZHCOEFF7;
    UINT32 u4_RZHCOEFF8;
    UINT32 u4_RZHCOEFF9;
    UINT32 u4_RZHCOEFF10;
    UINT32 u4_RZHCOEFF11;
    UINT32 u4_RZHCOEFF12;
    UINT32 u4_RZHCOEFF13;
    UINT32 u4_RZHCOEFF14;
    UINT32 u4_RZHCOEFF15;
    UINT32 u4_RZHCOEFF16;
    UINT32 u4_RZHCOEFF17;
    UINT32 u4_RZVCOEFF0;
    UINT32 u4_RZVCOEFF1;
    UINT32 u4_RZVCOEFF2;
    UINT32 u4_RZVCOEFF3;
    UINT32 u4_RZVCOEFF4;
    UINT32 u4_RZVCOEFF5;
    UINT32 u4_RZVCOEFF6;
    UINT32 u4_RZVCOEFF7;
    UINT32 u4_RZVCOEFF8;
    UINT32 u4_RZDITHER;
    UINT32 u4_RZCSC;
    UINT32 u4_RZCCOEFF11;
    UINT32 u4_RZCCOEFF12;
    UINT32 u4_RZCCOEFF13;
    UINT32 u4_RZCCOEFF21;
    UINT32 u4_RZCCOEFF22;
    UINT32 u4_RZCCOEFF23;
    UINT32 u4_RZCCOEFF31;
    UINT32 u4_RZCCOEFF32;
    UINT32 u4_RZCCOEFF33;
    UINT32 u4_LUMAKEY;
    UINT32 u4_0x201C0;
#if 1//defined (CC_MT5387) || defined (CC_MT5363)
    UINT32 u4_RZJPGVY;
    UINT32 u4_RZJPGVCB;
    UINT32 u4_RZJPGVCR;
#else
    UINT32 u4_DRAMQ_CFG;
    UINT32 u4_DRAMQ_STAD;
    UINT32 u4_DRAMQ_LEN;
#endif

#ifdef CC_MT5363
    UINT32 u4_RPR;
    UINT32 u4_0x201D4;
    UINT32 u4_FLIP;
#else
    UINT32 u4_DRAMQ_RDPTR;
    UINT32 u4_DRAMQ_EXPTR;
    UINT32 u4_DRAMQ_DUMMY;
#endif    
    UINT32 u4_0x201DC;

    UINT32 u4_LAST;
//#ifdef CC_MT5387//defined (CC_MT5360B) || defined (CC_MT5392B)
#if defined(CC_MT5387) && !defined(CC_MT5363) 
    UINT32 u4_0x201E4;
    UINT32 u4_0x201E8;
    UINT32 u4_Config;
#endif    

} E_RZ_DIF_REG_T;


// hw register bitfield
typedef struct _E_IMGRZ_DIF_FIELD_T
{
    // DWORD - RZSTART          (20000h)
    UINT32 fg_START             :  1;
#ifdef CC_MT5363//!defined (CC_MT5360B) && !defined (CC_MT5392B)
    UINT32 fg_CKEN              :  1;
    UINT32 fg_SW_RSTB           :  2;
    UINT32 fg_CSCLR             :  1;
    UINT32 fg_DMA_SW_RST :  1;
    #ifdef IMGRZ_HW_V4
    UINT32 fg_WDLE_DONE   :  1;    
    #else
    UINT32                      		 :  1;
    #endif
    UINT32 fg_INT_MASK          :  1;
    UINT32 fg_SW_RST            :  2;
    UINT32                      :  6;
    UINT32 fg_BST_EN            :  1;
    #ifdef IMGRZ_HW_V4 
    UINT32 			            			:  3;
    UINT32 fg_MMU_RSTB		    :  2;
    UINT32 fg_MMU_CKEN		    :  1;
    UINT32                      : 9;
    #else
    UINT32                      : 15;    
    #endif
#else
    UINT32                      : 31;
#endif

    // DWORD - RZTYPE           (20004h)
    UINT32 fg_JPGMD             :  1;
    UINT32 fg_OSDMD             :  1;
    UINT32 fg_TOPFLD            :  1;
    UINT32 fg_INTER             :  1;
    UINT32 fg_RSIN              :  1;
    UINT32 fg_OSDP              :  1;
    #ifdef IMGRZ_HW_V4 
    UINT32 fg_VDOPMD          :  1;    
    UINT32 fg_VDOPP              :  1;        
    #else
    UINT32                      :  2;
    #endif
    UINT32 fg_V4TAPMD           :  1;
    #ifdef IMGRZ_HW_V5
    UINT32                      :  3;
    UINT32 fg_OSD_TURBO         :  1;
    UINT32                      :  3;
    #else
    UINT32                      :  7;
    #endif
    UINT32 fg_RSOUT             :  1;
    UINT32 fg_OUTMD             :  2;
    UINT32 fg_PADCBCR           :  2;
    #ifdef IMGRZ_HW_V4 
    UINT32 fg_CBCR_SWAP       :  1;
    UINT32                      :  2;
    #else
    UINT32                      :  3;
    #endif
    UINT32 fg_V2OSD             :  1;
    UINT32                      :  7;

    // DWORD - RZJPGMODE        (20008h)
    UINT32 fg_JPGVFAC_CR        :  2;
    UINT32 fg_JPGHFAC_CR        :  2;
    UINT32 fg_JPGVFAC_CB        :  2;
    UINT32 fg_JPGHFAC_CB        :  2;
    UINT32 fg_JPGVFAC_Y         :  2;
    UINT32 fg_JPGHFAC_Y         :  2;
    UINT32 fg_LAST_BL           :  1;
    UINT32 fg_FST_BL            :  1;
    UINT32 fg_YCBCR             :  3;
    UINT32                      :  1;
    UINT32 fg_PRELD             :  1;
#ifdef IMGRZ_HW_V4
	UINT32                      			   : 1;
	UINT32    fg_RV_OFST_CTRL : 1;
	UINT32    								   : 10;	
#else
    UINT32                      			  : 12;
#endif
    UINT32 fg_BLASSIGN          :  1;

    // DWORD - RZMIFMODE        (2000Ch)
    #ifdef IMGRZ_HW_V4
    UINT32 fg_ASWIN             :  3;
    UINT32                      :  1;
    UINT32 fg_ASWOUT            :  3;
    UINT32                      :  1;
    UINT32 fg_BST_LIM           :  5;
    UINT32                      :  3;    
    UINT32 fg_KEEPRD           :  1;
    UINT32                      :  3;
    UINT32  fg_RSTADDR        :  1;
    UINT32                      : 3;
    UINT32 fg_MMU_PRELOADE_ID  :  3;    
    UINT32                      : 1;    
    UINT32 fg_MMU_TMP_ID  		:  3;        
    UINT32 fg_MMU_TMP_MASK  :  1;            
    #else
    UINT32 fg_ASWIN             :  3;
    UINT32                      :  1;
    UINT32 fg_ASWOUT            :  3;
    UINT32                      :  1;
    UINT32 fg_BST_LIM           :  4;
    UINT32 fg_RSTADDR           :  1;
    UINT32                      :  3;
    UINT32 fg_KEEPRD            :  1;
    UINT32                      : 15;    
    #endif

    // DWORD - RZSRCBUFL        (20010h)
    UINT32 fg_SRCBUFL           : 12;
    UINT32 fg_SRCBUFL_C       : 12;
    UINT32                      			: 8;

    // DWORD - RZJPGINTF        (20014h)
    UINT32 fg_INTFON            :  1;
    #ifdef IMGRZ_HW_V4
    UINT32 fg_INTFMD            :  1;
    UINT32 					            :  2;
    UINT32 fg_VDOINT            :  1;    
    UINT32                      : 27;    
    #else
    UINT32                      : 31;
    #endif

    // DWORD - RZTGBUFL         (20018h)
#ifdef CC_MT5363
    UINT32 fg_TGBUFL            : 12;
    UINT32 fg_TGBUFL_C        : 12;
#else
    UINT32 fg_TGBUFL            : 10;
    UINT32                      : 14;
#endif    
    UINT32 fg_LBUFL             :  5;
#ifdef IMGRZ_HW_V4
    UINT32                      			:  1;
	UINT32 fg_EXT16_TBUF      :  1;
#else
    UINT32                      :  2;
#endif
    UINT32 fg_EXT16             :  1;

    // DWORD - RZSRCYB1         (2001Ch)
    #ifdef IMGRZ_HW_V4	
    UINT32 fg_SRCYBASE1         : 28;
    UINT32                      :  4;
    #else
    UINT32 fg_SRCYBASE1         : 26;
    UINT32                      :  6;    
    #endif

    // DWORD - RZSRCYB2         (20020h)
    #ifdef IMGRZ_HW_V4	
    UINT32 fg_SRCYBASE2         : 28;
    UINT32                      :  4;
    #else
    UINT32 fg_SRCYBASE2         : 26;
    UINT32                      :  6;    
    #endif
    // DWORD - RZSRCCBB1        (20024h)
    #ifdef IMGRZ_HW_V4
    UINT32 fg_SRCCBBASE1        : 28;
    UINT32                      :  4;
    #else
    UINT32 fg_SRCCBBASE1        : 26;
    UINT32                      :  6;    
    #endif

    // DWORD - RZSRCCBB2        (20028h)
    #ifdef IMGRZ_HW_V4    
    UINT32 fg_SRCCBBASE2        : 28;
    UINT32                      :  4;
    #else
    UINT32 fg_SRCCBBASE2        : 26;
    UINT32                      :  6;    
    #endif

    // DWORD - RZSRCCRB1        (2002Ch)
    #ifdef IMGRZ_HW_V4
    UINT32 fg_SRCCRBASE1        : 28;
    UINT32                      :  4;
	#else
    UINT32 fg_SRCCRBASE1        : 26;
    UINT32                      :  6;	
	#endif
    // DWORD - RZSRCCRB2        (20030h)
    #ifdef IMGRZ_HW_V4
    UINT32 fg_SRCCRBASE2        : 28;
    UINT32                      :  4;
    #else
    UINT32 fg_SRCCRBASE2        : 26;
    UINT32                      :  6;    
    #endif

    // DWORD - RZTGYB           (20034h)
    #ifdef IMGRZ_HW_V4
    UINT32 fg_TGYBASE           : 28;
    UINT32                      :  4;
    #else
    UINT32 fg_TGYBASE           : 26;
    UINT32                      :  6;    
    #endif

    // DWORD - RZTGCB           (20038h)
    #ifdef IMGRZ_HW_V4
    UINT32 fg_TGCBASE           : 28;
    UINT32                      :  4;
    #else
    UINT32 fg_TGCBASE           : 26;
    UINT32                      :  6;    
    #endif

    // DWORD - Reserved Reg     (2003Ch)
    UINT32 fg_Res2003C          : 32;

    // DWORD - RZSRCSZY         (20040h)
    UINT32 fg_SRC_HEIGHTY       : 16;
    UINT32 fg_SRC_WIDTHY        : 16;

    // DWORD - RZSRCSZCB        (20044h)
    UINT32 fg_SRC_HEIGHTCB      : 16;
    UINT32 fg_SRC_WIDTHCB       : 16;

    // DWORD - RZSRCSZCR        (20048h)
    UINT32 fg_SRC_HEIGHTCR      : 16;
    UINT32 fg_SRC_WIDTHCR       : 16;

    // DWORD - RZTGSZ           (2004Ch)
#ifdef CC_MT5363
    UINT32 fg_TG_HEIGHT         : 14;
    UINT32                      :  2;
    UINT32 fg_TG_WIDTH          : 14;
    UINT32                      :  2;
#else
    UINT32 fg_TG_HEIGHT         : 12;
    UINT32 fg_TG_WIDTH          : 12;
    UINT32                      :  8;
#endif    

    // DWORD - Reserved Reg     (20050h)
    UINT32 fg_Res20050          : 32;

    // DWORD - RZSRCOFTY        (20054h)
    UINT32 fg_SRC_V_OFTY        : 12;
    UINT32 fg_SRC_H_OFTY        : 12;
    UINT32                      :  8;

    // DWORD - RZSRCOFTCB       (20058h)
    UINT32 fg_SRC_V_OFTCB       : 12;
    UINT32 fg_SRC_H_OFTCB       : 12;
    UINT32                      :  8;

    // DWORD - RZSRCOFTCR       (2005Ch)
    UINT32 fg_SRC_V_OFTCR       : 12;
    UINT32 fg_SRC_H_OFTCR       : 12;
    UINT32                      :  8;

    // DWORD - RZTGOFT          (20060h)
    UINT32 fg_TG_V_OFT          : 12;
    UINT32 fg_TG_H_OFT          : 12;
    UINT32                      :  8;

    // DWORD - RZH8TAPY         (20064h)
#ifdef CC_MT5363    
    UINT32 fg_H8TAP_FAC_Y       : 26;
    UINT32                      :  6;

    // DWORD - RZH8TAPCB        (20068h)
    UINT32 fg_H8TAP_FAC_CB      : 26;
    UINT32                      :  6;

    // DWORD - RZH8TAPCR        (2006Ch)
    UINT32 fg_H8TAP_FAC_CR      : 26;
    UINT32                      :  6;
#else   
    UINT32 fg_H8TAP_FAC_Y       : 19;
    UINT32 fg_H8TAP_OFT_Y       : 13;

    // DWORD - RZH8TAPCB        (20068h)
    UINT32 fg_H8TAP_FAC_CB      : 19;
    UINT32 fg_H8TAP_OFT_CB      : 13;

    // DWORD - RZH8TAPCR        (2006Ch)
    UINT32 fg_H8TAP_FAC_CR      : 19;
    UINT32 fg_H8TAP_OFT_CR      : 13;
#endif

    // DWORD - RZHSAY           (20070h)
    UINT32 fg_HSA_FAC_Y         : 12;
    UINT32 fg_HSA_OFT_Y         : 11;
    UINT32                      :  9;

    // DWORD - RZHSACB          (20074h)
    UINT32 fg_HSA_FAC_CB        : 12;
    UINT32 fg_HSA_OFT_CB        : 11;
    UINT32                      :  9;

    // DWORD - RZHSACR          (20078h)
    UINT32 fg_HSA_FAC_CR        : 12;
    UINT32 fg_HSA_OFT_CR        : 11;
    UINT32                      :  9;

    // DWORD - RZVSCLY          (2007Ch)
    UINT32 fg_V_FACTORY         : 11;
    UINT32 fg_VUP_Y             :  1;
    UINT32 fg_V_SCLOFTY         : 11;
    UINT32                      :  9;

    // DWORD - RZVSCLCB         (20080h)
    UINT32 fg_V_FACTORCB        : 11;
    UINT32 fg_VUP_CB            :  1;
    UINT32 fg_V_SCLOFTCB        : 11;
    UINT32                      :  9;

    // DWORD - RZVSCLCR         (20084h)
    UINT32 fg_V_FACTORCR        : 11;
    UINT32 fg_VUP_CR            :  1;
    UINT32 fg_V_SCLOFTCR        : 11;
    UINT32                      :  9;

    // DWORD - RZV4TAPY         (20088h)
#ifdef CC_MT5363
    UINT32 fg_V4TAP_FAC_Y       : 26;
    UINT32                      :  6;

    // DWORD - RZV4TAPCB        (2008Ch)
    UINT32 fg_V4TAP_FAC_C       : 26;
    UINT32                      :  6;

    UINT32 fg_Res20090          : 32;
#else
    UINT32 fg_V4TAP_FAC_Y       : 19;
    UINT32 fg_V4TAP_OFT_Y       : 12;
    UINT32                      :  1;

    // DWORD - RZV4TAPCB        (2008Ch)
    UINT32 fg_V4TAP_FAC_CB      : 19;
    UINT32 fg_V4TAP_OFT_CB      : 12;
    UINT32                      :  1;

    // DWORD - RZV4TAPCR        (20090h)
    UINT32 fg_V4TAP_FAC_CR      : 19;
    UINT32 fg_V4TAP_OFT_CR      : 12;
    UINT32                      :  1;
#endif
    
    // DWORD - RZTMPB           (20094h)
    UINT32 fg_TMPBASE           : 26;
    UINT32                      :  6;

#if 1//defined (CC_MT5387) || defined (CC_MT5363)
    // DWORD - RZPRELOADY       (20098h)    
    UINT32 fg_PRELOADY          : 32;

    // DWORD - RZPRELOADC       (2009Ch)
    UINT32 fg_PRELOADC          : 32;

    // DWORD - RZRESUME         (200A0h)
    UINT32 fg_RESUME            : 1;
    UINT32                      : 31;

    // DWORD - RZSRCCNTY        (200A4h)
    UINT32 fg_SRC_CNT_Y         : 16;
    UINT32                      : 16;
    UINT32 fg_SRC_CNT_CB        : 16;
    UINT32                      : 16;
#else
    // DWORD - RZJPGVY          (20098h)
    UINT32 fg_LNUMY             : 12;
    UINT32                      : 20;

    // DWORD - RZJPGVCB         (2009Ch)
    UINT32 fg_LNUMCB            : 12;
    UINT32                      : 20;

    // DWORD - RZJPGVCR         (200A0h)
    UINT32 fg_LNUMCR            : 12;
    UINT32                      : 20;

    // DWORD - RZPRELOADY       (200A4h)
    UINT32 fg_PRELOADY          : 32;

    // DWORD - RZPRELOADC       (200A8h)
    UINT32 fg_PRELOADC          : 32;
#endif  
    // DWORD - RZSRCCNTCR       (200ACh)
    UINT32 fg_SRC_CNT_CR        : 16;
    UINT32                      : 16;
    UINT32 fg_V_WR_OFT_Y        : 12;
    UINT32                      : 20;
    UINT32 fg_V_WR_OFT_CB       : 12;
    UINT32                      : 20;
    UINT32 fg_V_WR_OFT_CR       : 12;
    UINT32                      : 20;
    UINT32 fg_V_NEXT_COEF_Y     : 12;
    UINT32                      : 20;
    UINT32 fg_V_NEXT_COEF_CB    : 12;
    UINT32                      : 20;
    UINT32 fg_V_NEXT_COEF_CR    : 12;
    UINT32                      : 20;
#ifdef CC_MT5363    
    UINT32 fg_H8TAP_OFT_Y       : 21;		// DWORD - RZYH8TAPO       (200C8h)
    UINT32                      : 11;
    UINT32 fg_H8TAP_OFT_CB      : 21;
    UINT32                      : 11;
    UINT32 fg_H8TAP_OFT_CR      : 21;
    UINT32                      : 11;    
    UINT32 fg_V4TAP_OFT_Y	    : 21;	// DWORD - RZYV4TAPO       (200d4h)
    UINT32                      : 11;        
    UINT32 fg_V4TAP_OFT_C       : 21;
    UINT32                      : 11;            
#else
    UINT32 fg_Res200C8          : 32;
    UINT32 fg_Res200CC          : 32;
    UINT32 fg_Res200D0          : 32;
    UINT32 fg_Res200D4          : 32;
    UINT32 fg_Res200D8          : 32;
#endif
    UINT32 fg_Res200DC          : 32;
    UINT32 fg_Res200E0          : 32;
    UINT32 fg_Res200E4          : 32;

#ifdef CC_MT5363
    // DWORD - RZCNTMON         (200E8h)
    UINT32 fg_REMAIN_CNT        : 14;
    UINT32                      :  2;
    UINT32 fg_VSCL_OUT_CNT      : 14;
    UINT32                      :  2;
#else
    // DWORD - RZCHKSUM         (200E8h)
    UINT32 fg_CHK_SUM           : 32;
#endif
    
    // DWORD - RZINTFMON        (200ECh)
    UINT32 fg_BUSY2             :  1;
    UINT32                      :  3;
    UINT32 fg_BUSY1             :  1;
    UINT32                      :  3;
    UINT32 fg_RDY2              :  1;
    UINT32                      :  3;
    UINT32 fg_RDY1              :  1;
    UINT32                      :  3;
    UINT32 fg_P_LAST_BL         :  1;
    UINT32                      : 15;

    // DWORD - RZFSMMON         (200F0h)
    UINT32 fg_ST_CTRL           :  5;
    UINT32 fg_ST_RMIF           :  3;
    UINT32 fg_ST_WMIF           :  3;
    UINT32 fg_ST_H8TAP          :  2;
    UINT32 fg_ST_MEM_X2         :  3;
    UINT32 fg_ST_OSD            :  2;
    UINT32                      : 14;

    // DWORD - RZSTMON          (200F4h)
    UINT32 fg_RD_REQ            :  1;
    UINT32 fg_DATA_V            :  1;
    UINT32 fg_WR_REQ            :  1;
    UINT32 fg_WFULL             :  1;
#ifdef CC_MT5363
    UINT32                      : 24;
#else
    UINT32 fg_REMAIN_CNT        : 12;
    UINT32 fg_VSCL_OUT_CNT      : 12;
#endif    
    UINT32 fg_CR_COM            :  1;
    UINT32 fg_CB_COM            :  1;
    UINT32 fg_Y_COM             :  1;
    UINT32                      :  1;
    
    // DWORD - RZDATAMON        (200F8h)
    UINT32 fg_RESULT_H          :  8;
    UINT32 fg_H_OK              :  1;
    UINT32                      :  7;
    UINT32 fg_RESULT_V          :  8;
    UINT32 fg_V_OK              :  1;
    UINT32                      :  7;

    // DWORD - RZDONE           (200FCh)
    UINT32 fg_DONE              :  1;
    UINT32 fg_DMA_DONE     :  1;
    UINT32                      :  2;
    UINT32 fg_IDLE              :  1;
    UINT32                      : 27;


    // DWORD - RZOSDMODE        (20100h)
    UINT32 fg_DCMD              :  1;
    #ifdef IMGRZ_HW_V4	
    UINT32 fg_INTYPE            :  3;
    UINT32 fg_B_RPT             :  1;
    UINT32 fg_A_SCL_TYPE        :  2;
    UINT32 fg_A_SCL_BL        		:  1;    
    UINT32 fg_A_SCL_DIST_0     :  1;
    UINT32 fg_OUTTYPE          	 	:  3;
    UINT32 fg_SWTCH             		:  1;
    UINT32 fg_OSD_DISP_422    : 1;    
    UINT32 fg_A_SWITCH_RGBA : 1;
    UINT32 									    : 1;       
    #else
    UINT32 fg_INTYPE            :  2;
    UINT32 fg_B_RPT             :  1;
    UINT32 fg_A_SCL_TYPE        :  2;
    UINT32 fg_A_SCL_BL        		:  1;    
    UINT32 fg_A_SCL_DIST_0     :  1;
    UINT32 fg_OUTTYPE           :  3;
    UINT32 fg_SWTCH             :  1;
    UINT32 fg_OSD_DISP_422    :  1;
    UINT32                      :  3;    
    #endif
    UINT32 fg_USR_DEF_A         :  8;
    UINT32 fg_A_BLEND           :  8;
    
    // DWORD - RZOSDCTRL        (20104h)
    UINT32 fg_INI_CPT           :  1;
    UINT32 fg_CPT_CKEN          :  1;    
    #ifdef IMGRZ_HW_V5
    UINT32                      : 26;
    UINT32 fg_OSD_ALU_EN        : 1;
    UINT32                      : 3;    
    #else
    UINT32                      : 30;
    #endif
    
    // DWORD - RZATABLE         (20108h)
    UINT32 fg_A_TABLE           : 32;

    // DWORD - RZOSDTRAN0       (2010Ch)
    UINT32 fg_COLOR_TRANS0      : 32;
    
    // DWORD - RZOSDTRAN1       (20110h)
    UINT32 fg_COLOR_TRANS1      : 32;

    // DWORD - RZOSDTRAN2       (20114h)
    UINT32 fg_COLOR_TRANS2      : 32;
    
    // DWORD - RZOSDTRAN3       (20118h)
    UINT32 fg_COLOR_TRANS3      : 32;
    
    // DWORD - RZCPTADDR        (2011Ch)
    UINT32 fg_CPT_W_ADDR        :  8;
    UINT32                      : 24;
    
    // DWORD - RZCPTDATA        (20120h)
    UINT32 fg_CPT_W_DATA        : 32;

    // DWORD - RZHCOEFF0        (20124h)
    UINT32 fg_H8TAP_COEFF0      : 32;
    
    // DWORD - RZHCOEFF1        (20128h)
    UINT32 fg_H8TAP_COEFF1      : 32;
    
    // DWORD - RZHCOEFF2        (2012Ch)
    UINT32 fg_H8TAP_COEFF2      : 32;
    
    // DWORD - RZHCOEFF3        (20130h)
    UINT32 fg_H8TAP_COEFF3      : 32;
    
    // DWORD - RZHCOEFF4        (20134h)
    UINT32 fg_H8TAP_COEFF4      : 32;
    
    // DWORD - RZHCOEFF5        (20138h)
    UINT32 fg_H8TAP_COEFF5      : 32;
    
    // DWORD - RZHCOEFF6        (2013Ch)
    UINT32 fg_H8TAP_COEFF6      : 32;
    
    // DWORD - RZHCOEFF7        (20140h)
    UINT32 fg_H8TAP_COEFF7      : 32;
    
    // DWORD - RZHCOEFF8        (20144h)
    UINT32 fg_H8TAP_COEFF8      : 32;
    
    // DWORD - RZHCOEFF9        (20148h)
    UINT32 fg_H8TAP_COEFF9      : 32;
    
    // DWORD - RZHCOEFF10       (2014Ch)
    UINT32 fg_H8TAP_COEFF10     : 32;
    
    // DWORD - RZHCOEFF11       (20150h)
    UINT32 fg_H8TAP_COEFF11     : 32;
    
    // DWORD - RZHCOEFF12       (20154h)
    UINT32 fg_H8TAP_COEFF12     : 32;
    
    // DWORD - RZHCOEFF13       (20158h)
    UINT32 fg_H8TAP_COEFF13     : 32;
    
    // DWORD - RZHCOEFF14       (2015Ch)
    UINT32 fg_H8TAP_COEFF14     : 32;
    
    // DWORD - RZHCOEFF15       (20160h)
    UINT32 fg_H8TAP_COEFF15     : 32;
    
    // DWORD - RZHCOEFF16       (20164h)
    UINT32 fg_H8TAP_COEFF16     : 32;
    
    // DWORD - RZHCOEFF17       (20168h)
    UINT32 fg_H8TAP_COEFF17     : 32;
    
    // DWORD - RZVCOEFF0        (2016Ch)
    UINT32 fg_V4TAP_COEFF0      : 32;
    
    // DWORD - RZVCOEFF1        (20170h)
    UINT32 fg_V4TAP_COEFF1      : 32;
    
    // DWORD - RZVCOEFF2        (20174h)
    UINT32 fg_V4TAP_COEFF2      : 32;
    
    // DWORD - RZVCOEFF3        (20178h)
    UINT32 fg_V4TAP_COEFF3      : 32;
    
    // DWORD - RZVCOEFF4        (2017Ch)
    UINT32 fg_V4TAP_COEFF4      : 32;
    
    // DWORD - RZVCOEFF5        (20180h)
    UINT32 fg_V4TAP_COEFF5      : 32;
    
    // DWORD - RZVCOEFF6        (20184h)
    UINT32 fg_V4TAP_COEFF6      : 32;
    
    // DWORD - RZVCOEFF7        (20188h)
    UINT32 fg_V4TAP_COEFF7      : 32;
    
    // DWORD - RZVCOEFF8        (2018Ch)
    UINT32 fg_V4TAP_COEFF8      : 32;
    
    // DWORD - RZDITHER         (20190h)
    UINT32 fg_FPHASE            :  4;
    UINT32 fg_LSB_OFF           :  1;
    UINT32 fg_RND_EN            :  1;
    UINT32 fg_EDITH_EN          :  1;
    UINT32 fg_RDITH_EN          :  1;
    UINT32                      : 24;

    // DWORD - RZCSC            (20194h)
    UINT32 fg_CSC_EN            :  1;
    UINT32 fg_YIN_D16           :  1;
    UINT32 fg_CIN_D128          :  1;
    UINT32 fg_YOUT_A16          :  1;
    UINT32 fg_COUT_A128         :  1;
    UINT32                      : 27;
    
    // DWORD - RZCCOEFF11       (20198h)
    UINT32 fg_CSC_COEFF11       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF12       (2019Ch)
    UINT32 fg_CSC_COEFF12       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF13       (201A0h)
    UINT32 fg_CSC_COEFF13       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF21       (201A4h)
    UINT32 fg_CSC_COEFF21       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF22       (201A8h)
    UINT32 fg_CSC_COEFF22       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF23       (201ACh)
    UINT32 fg_CSC_COEFF23       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF31       (201B0h)
    UINT32 fg_CSC_COEFF31       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF32       (201B4h)
    UINT32 fg_CSC_COEFF32       : 13;
    UINT32                      : 19;
    
    // DWORD - RZCCOEFF33       (201B8h)
    UINT32 fg_CSC_COEFF33       : 13;
    UINT32                      : 19;
    
    // DWORD - LUMAKEY          (201BCh)
    UINT32 fg_LUMAKEY_EN        :  1;
    UINT32 fg_LK_SCL_TYPE       :  2;
    #ifdef IMGRZ_HW_V4 
    UINT32 fg_LK_SCL_BI       :  1;
    UINT32 fg_LK_YUV_MD       :  1;    
    UINT32                      :  3;    
    #else
    UINT32                      :  5;
    #endif
    UINT32 fg_LUMA_KEY          :  8;
    UINT32                      : 16;
    
    // DWORD - Reserved Reg     (201C0h)
    UINT32 fg_Res201C0          : 32;

#if 1//defined (CC_MT5387) || defined (CC_MT5363)
    // DWORD - RZJPGVY          (201C4h)
    UINT32 fg_LNUMY             : 12;
    UINT32                      : 20;

    // DWORD - RZJPGVCB         (201C8h)
    UINT32 fg_LNUMCB            : 12;
    UINT32                      : 20;
    
    // DWORD - RZJPGVCR         (201CCh)
    UINT32 fg_LNUMCR            : 12;
    UINT32                      : 20;
#else
    // DWORD - DRAMQ_CFG        (201C4h)
    UINT32 fg_EN_DRAMQ          :  1;
    UINT32 fg_DRAMQ_MODE        :  1;
    UINT32                      :  2;
    UINT32 fg_CMDQ_THRS         :  2;
    UINT32                      :  2;
    UINT32 fg_DRAMQ_SIZE        :  3;
    UINT32                      :  5;
    UINT32 fg_STOP              :  1;
    UINT32                      : 13;
    UINT32 fg_SW_CMDQ_RST_B     :  2;
    
    // DWORD - DRAMQ_STAD       (201C8h)
    UINT32 fg_DRAMQ_BSAD        : 30;
    UINT32                      :  2;

    // DWORD - DRAMQ_LEN        (201CCh)
    UINT32 fg_DRAMQ_LEN         : 18;
    UINT32                      : 14;
#endif    
    
#ifdef CC_MT5363
    // DWORD - RZRPR            (201D0h)
    UINT32 fg_RPR               :  1;
    UINT32 fg_URCRPR            :  1;
    UINT32                      :  6;
    UINT32 fg_TRCVDEC_EN        :  1;
    UINT32 fg_TRCVDEC_INT       :  3;
    UINT32                      : 20;
    
    // DWORD -                  (201D4h)
    UINT32 fg_Res201D4          : 32;
    
    // DWORD - RZFLIP           (201D8h)
    UINT32 fg_FLIP_ON           : 32;    
#else
    // DWORD - DRAMQ_RDPTR      (201D0h)
    UINT32 fg_DRAMQ_RDPTR       : 18;
    UINT32                      : 14;
    
    // DWORD - DRAMQ_EXPTR      (201D4h)
    UINT32 fg_DRAMQ_EXPTR       : 18;
    UINT32                      : 14;
    
    // DWORD - DUMMY            (201D8h)
    UINT32 fg_DUMMY             : 32;
#endif    

    // DWORD - Reserved Reg     (201DCh)
    UINT32 fg_Res201DC          : 32;
    
    // DWORD - LAST             (201E0h)
    UINT32 fg_LAST              : 32;

//#ifdef CC_MT5387//defined (CC_MT5360B) || defined (CC_MT5392B)

#ifdef CC_MT5363
    UINT32 fg_Res201E4          : 32;
    UINT32 fg_Res201E8          : 32;

    #ifdef IMGRZ_HW_V4 	    
	UINT32 fg_VENC_SKIP         :  1;	 //(201ECh)	//E_RZ_INOUT_TYPE_VDOENCODE_V2
    UINT32                      : 31;    
    #else
    UINT32 fg_Res201EC          : 32;    
    #endif

#elif defined(CC_MT5387) && !defined(CC_MT5363) 
    UINT32 fg_Res201E4          : 32;
    UINT32 fg_Res201E8          : 32;
    // DWORD - CONFIG           (201ECh)
    UINT32                      :  1;
    UINT32 fg_CKEN              :  1;
    UINT32 fg_SW_RSTB           :  2;
    UINT32 fg_CSCLR             :  1;
    UINT32                      :  2;
    UINT32 fg_INT_MASK          :  1;
    UINT32 fg_SW_RST            :  2;
    UINT32                      : 22;
#endif   
	
	UINT32 fg_HEC_CKSum           : 32;	//(201F0h)    
	UINT32 fg_VSCL_CKSum          : 32;	//(201F4h)    
	UINT32 fg_COEF_CKSum          : 32;	//(201F8h)    	
	UINT32 fg_MULT_CKSum          : 32;	//(201FCh)    	
	
	UINT32 fg_ALCOM_AR             : 8;	//(20200h)   	
	UINT32 fg_ALCOM_OPCODE         : 4;	
	UINT32 fg_ALCOM         	   : 1;
	UINT32          	   		   : 3;	
	UINT32 fg_ALPHA_VALUE  		   : 8;	
	UINT32 fg_PREMULT_DST_WR  		   : 1;
	UINT32 fg_PREMULT_DST_RD  		   : 1;
	UINT32 fg_PREMULT_SRC_RD  		   : 1;
	UINT32 fg_PREMULT_OVERFLOW_EN  		   : 1;
	UINT32 fg_ALU_MODE_SELECTION  		   : 3;	
	UINT32          	   		   : 1;	
	
	UINT32 fg_JAVA_XOR_SRC_ALPHA_CHK          : 1;	//(20204h)   	
	UINT32 fg_SRC_ALPHA_REPLACE          : 1;		
	UINT32 fg_PRE_COLORIZE          : 1;	
	UINT32 fg_ALCOM_MAX_ALPHA          : 1;	
	UINT32 fg_ALPHA_0_RGB_0          : 1;	
	UINT32           				: 7;		
	UINT32 fg_ROP_OPCODE 	          : 4;		
	UINT32 fg_DST_RD_B_ORDER          : 2;		
	UINT32 fg_DST_RD_G_ORDER          : 2;			
	UINT32 fg_DST_RD_R_ORDER    	  : 2;		
	UINT32 fg_DST_RD_A_ORDER          : 2;
	UINT32 fg_SRC_RD_B_ORDER		  : 2;
	UINT32 fg_SRC_RD_G_ORDER		  : 2;	
	UINT32 fg_SRC_RD_R_ORDER		: 2;	
	UINT32 fg_SRC_RD_A_ORDER		: 2;	
} E_RZ_DIF_FIELD_T;


// hw register name/bitfield union
typedef union _E_RZ_DIF_UNION_T
{
    E_RZ_DIF_REG_T   rReg;
    E_RZ_DIF_FIELD_T rField;
} E_RZ_DIF_UNION_T;


#endif // IMGRZ_DIF_REG_H


/*lint -restore */


