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
#ifndef _TVE_HW_5391_H_
#define _TVE_HW_5391_H_

#include "x_typedef.h"
#include "x_hal_io.h"
#include "x_hal_5381.h"

#if defined(CC_MT5387) && !defined(CC_MT5363)
#define TV_EOCODER_BASE_RG	(IO_VIRT + 0x6600)
#else //5363 or 6595
#define TV_EOCODER_BASE_RG	(IO_VIRT + 0x2d600)
#endif

#define IO_TVE_BASE (IO_VIRT+0x22000)

//Page TVE
#define AFE_0F (IO_TVE_BASE+0x047C)
    #define VDAC_REV Fld(8,24,AC_FULLB3)//[31:24]
    #define MIXER_DAC_SEL Fld(1,18,AC_MSKB2)//[18:18]
    #define MIXER_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define VDAC_TEST_EN Fld(1,4,AC_MSKB0)//[4:4]
#define AFE_10 (IO_TVE_BASE+0x0480)
    #define TRIMVAL Fld(5,24,AC_MSKB3)//[28:24]
    #define PDBIAS Fld(1,20,AC_MSKB2)//[20:20]
    #define PDCD Fld(1,18,AC_MSKB2)//[18:18]
    #define PDBCD Fld(1,16,AC_MSKB2)//[16:16]
    #define BGREF_OFF Fld(1,10,AC_MSKB1)//[10:10]
    #define VREF Fld(1,9,AC_MSKB1)//[9:9]
    #define VDAC_MODE Fld(2,0,AC_MSKB0)//[1:0]
#define AFE_11 (IO_TVE_BASE+0x0484)
    #define ABIST_SIG_GEN_P1 Fld(10,16,AC_MSKW32)//[25:16]
    #define DSP_DATA_SEL_ORDER Fld(1,14,AC_MSKB1)//[14:14]
    #define ABIST_DSP_HI_BIT Fld(1,13,AC_MSKB1)//[13:13]
    #define ABIST_SRC_SEL Fld(1,11,AC_MSKB1)//[11:11]
    #define ABIST_SW_RST_B Fld(1,10,AC_MSKB1)//[10:10]
    #define ABIST_DAC_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define ABIST_GAIN Fld(8,0,AC_FULLB0)//[7:0]
#define AFE_12 (IO_TVE_BASE+0x0488)
    #define SW_RST_B Fld(1,31,AC_MSKB3)//[31:31]
    #define VDOIN_GAIN_OFF Fld(1,25,AC_MSKB3)//[25:25]
    #define VDOIN_GAIN Fld(5,20,AC_MSKW32)//[24:20]
    #define COVERAGE Fld(20,0,AC_MSKDW)//[19:0]
#define AFE_13 (IO_TVE_BASE+0x048C)
    #define ABIST_PAT_SEL Fld(2,28,AC_MSKB3)//[29:28]
    #define ABIST_SQR_HALF_PERIOD Fld(10,16,AC_MSKW32)//[25:16]
    #define VDAC_LSB_OFF Fld(4,12,AC_MSKB1)//[15:12]
    #define ABIST_DC_LEVEL Fld(12,0,AC_MSKW10)//[11:0]
	
#if 1//def MT5391_TVE
#define RW_CVBS_TVE_ENCSYN         0x00
  #define CVBS_ENCRST             (0x3<<30)
#define RW_CVBS_TVE_MODE           0x04
  #define CVBS_ENCOFF             (0x1)
  #define CVBS_CBON               (0x1 << 1)
  #define CVBS_SETUP              (0x1 << 2)
  #define CVBS_CLPF_ON            (0x1 << 5)
  #define CVBS_YLPF_ON            (0x1 << 7)
  #define CVBS_YDEL_MASK          (0xf << 8)
  #define CVBS_YDEL_SHFT              8
  #define CVBS_SYDEL_SHFT            10
  #define CVBS_UV_UPS_ON          (0x1 << 14)
  #define CVBS_DA0_S_CVBS         (0x1 << 16)
  #define CVBS_DA0_YCBCR          (0x2 << 16)
  #define CVBS_DA0_RGB            (0x3 << 16)
  #define CVBS_DA1_CVBS           (0x1 << 18)
  #define CVBS_DA1_YCBCR          (0x2 << 18)
  #define CVBS_DA1_RGB            (0x3 << 18)
  #define CVBS_Y_G_SYNC           (0x1 << 20)
  #define CVBS_VGA                (0x1 << 21)
  #define CVBS_TVE_FLD            (0x1 << 22)
  #define CVBS_SLEW_OFF           (0x1 << 25)  
  #define CVBS_FULL_WIDTH         (0x1 << 27)
  #define CVBS_CB_TYPE            (0x1 << 28)
  #define CVBS_BLACKER            (0x1 << 30)
  #define CVBS_TT_RST             (0x1 << 31)
#define  RW_CVBS_TVE_CC            0x08
  #define TOP_CC                  (0x1 << 16)
  #define BTM_CC                  (0x1 << 17)  
#define RW_CVBS_TVE_YSCALE       0x0C
#define RW_CVBS_TVE_CBGAIN       0x10
#define RW_CVBS_TVE_CRGAIN       0x14
#define RW_CVBS_TVE_UGAIN        0x18
#define RW_CVBS_TVE_VGAIN        0x1C
#define RW_CVBS_TVE_GAMMA0       0x20
#define RW_CVBS_TVE_GAMMA1       0x24
#define RW_CVBS_TVE_GAMMA2       0x28
#define RW_CVBS_TVE_GAMMA3       0x2C
#define RW_CVBS_TVE_GAMMA4       0x30
#define RW_CVBS_TVE_DACTRL       0x34
  #define CVBS_DAC1                   0x02
  #define CVBS_DAC2                   (0x02 << 2)
  #define CVBS_DAC_DET_LEN     (0x3 << 6)
  #define CVBS_DAC_PDET          (0x1 << 8)
  #define CVBS_DAC_TRIMVAL     (0x1F << 24)
#define RW_CVBS_TVE_BURST        0x38
  #define NTSC_SCH                    (0x2c <<8)
  #define PAL_SCH                      (0x2C << 8) 
  #define BF443                          (0x1 << 15)
  #define EXT_GAIN_EN               (0x1 << 25)
#define RW_CVBS_TVE_CHROMA_GAIN      0x3C
  #define CHROMA_GAIN_MASK        ((unsigned)0xff)
  #define CHROMA_GAIN_EN             (0x01 << 8) 
#define RW_CVBS_TVE_WSSP           0x40
#define RW_CVBS_TVE_WSSI            0x44
  #define NTSC_ASP_4_3_NORMAL         0x0
  #define NTSC_ASP_16_9_NORMAL        0x1
  #define NTSC_ASP_4_3_LB             0x2
  #define NTSC_NO_CCI               (0xf << 2)
  #define PAL_ASP_4_3_FULL          0x8
  #define PAL_ASP_16_9_LB           0xb
  #define PAL_ASP_16_9_FULL         0x7
  #define VBI_ON                    (0x1 << 20)
  #define VBI_LVL_MASK              ((unsigned)0xff << 24)
#define RW_CVBS_TVE_TT_CTL        0x48
#define RW_CVBS_TVE_TELTXT1      0x4c  
#define RW_CVBS_TVE_TELTXT2      0x50
#define RW_CVBS_TVE_TELTXT3      0x54
#define RW_CVBS_TVE_TELTXT4      0x58
#define RW_CVBS_TVE_TELTXT5      0x5c
#define RW_CVBS_TVE_UVGAIN       0x68
#define RW_CVBS_TVE_YGLIMIT      0x6c
#define RW_CVBS_TVE_DSIP_AREA    0x70
  #define DISP_END_PXL_MASK        0xfff
  #define NEW_DIN_EN               (0x01 << 15)
  #define DISP_BGN_PXL_MASK        ((unsigned)0xfff << 16) 
#define RW_CVBS_TVE_BURST2          0x74
  #define BURST_END_MASK           (unsigned)0xff 
  #define BURST_START_MASK         ((unsigned)0xff << 8)
  #define MV_BURST_START_MASK      ((unsigned)0xff << 16)
  #define ADV_MV_BURST_START_MASK  ((unsigned)0xff << 24)

#define RW_CVBS_TVE_MUX_CTRL       0x78
  #define CVBS_MUX_EN			(1<<0)
  #define CVBS_P2I_EN     		   	(1<<1)
  #define VDOIN_ROUND_EN           	(1<<2)
  #define PIP_ROUND_EN             	(1<<3)
  #define PRGS_SELF_EN         		(1<<8) 
  #define PRGS_OUT                 	(1<<9) 
  #define TV_TYPE_MASK             	(3<<10) 
  #define PIP_DATA_SYNC_OFF       (1<<12)   
  #define MIXER1_SEL               	(1<<16)   
  #define MIXER2_SEL               	(1<<17)   
  #define MIXER_DAC1_SEL           	(1<<18)   
  #define MIXER_DAC2_SEL           	(1<<19)   

#define RW_CVBS_TVE_SYNC_CTRL      0x7C
  #define SYNC_EN            	(1<<0) //	0	Disable;	1	Enable
  #define ADJ_SYNC_EN        	(1<<2)//: enable the adjustment function ,0	Disable;	1	Enable
  #define ADJ_FORWARD        	(1<<3)// adjustment forward,	0	backward,	1	forward
  #define FIELD_POL          	(1<<5)  // polarity of field signal	0	Low active	1	High active
  #define VSYNC_POL          	(1<<6)  // polarity of vsync signal	0	Low active	1	High active
  #define HSYNC_POL          	(1<<7)// polarity of hsync signal	0	Low active	1	High active
  #define HSYNC_EDGE_SEL     	(1<<8)// select falling or raise edge of hsync to reset internal hcnt1	
  #define VSYNC_EDGE_SEL     	(1<<9)// select falling or raise edge of vsync to reset internal vcnt1	
  #define ADJ_SELF_EN        	(1<<10)// free run enable 0	Disable	1	Enable to free run
  #define ADJ_TOTAK_EN       	(1<<11)  // enable to adjust horizontal total pixel and vertical total line	0	Disable	1	Enable

#define RW_CVBS_TVE_SYNC_TIME0      0x80
  #define ADJ_H_TOTAL_MASK        0xfff
  #define ADJ_V_TOTAL_MASK       ((unsigned)0xfff << 16) 

#define RW_CVBS_TVE_SYNC_TIME1      0x84
  #define ADJ_HSYNC_MASK        0xfff
  #define ADJ_VSYNC_MASK       ((unsigned)0xfff << 16) 

#define RW_CVBS_TVE_SYNC_TIME2      0x88
#define RW_CVBS_TVE_SYNC_TIME3      0x8c

#define RW_CVBS_TVE_OSD_FACTORY      0x98
#if defined(CC_MT5387) || defined(CC_MT5363)	
#define RW_CVBS_TVE_YLF_COEA     0x100
#define RW_CVBS_TVE_YLF_COEB     0x104
#define RW_CVBS_TVE_YLF_COEC     0x108
#define RW_CVBS_TVE_YLF_COED     0x10c
#define RW_CVBS_TVE_YLF_COEE     0x110
#define RW_CVBS_TVE_CLF_COEA     0x114
#define RW_CVBS_TVE_CLF_COEB     0x118
#endif
#if 0//not support MV
#define RW_CVBS_TVE_MV1          0x180
#define RW_CVBS_TVE_MV2          0x184
#define RW_CVBS_TVE_MV3          0x188
#define RW_CVBS_TVE_MV4          0x18C
#define RW_CVBS_TVE_MV5          0x190
#define RW_CVBS_TVE_MV6          0x194
 #define MVOFF           (0x1 << 28)
 #define CPNTMVOFF       (0x1 << 29)
 #define MUVSW           (0x1 << 30)
#define RW_CVBS_TVE_MV7          0x198
#endif
#else
#endif
/*===========================================================================
// Macros for register read/write */

#define TVE_BASE						TV_EOCODER_BASE_RG

#define TVE_READ8(offset)				IO_READ8(TVE_BASE, (offset))
#define TVE_READ16(offset)				IO_READ16(TVE_BASE, (offset))
#define TVE_READ32(offset)				IO_READ32(TVE_BASE, (offset))

#define TVE_WRITE8(offset, value)		IO_WRITE8(TVE_BASE, (offset), (value))
#define TVE_WRITE16(offset, value)		IO_WRITE16(TVE_BASE, (offset), (value))
#define TVE_WRITE32(offset, value)		IO_WRITE32(TVE_BASE, (offset), (value))

#define TVE_REG8(offset)				IO_REG8(TVE_BASE, (offset))
#define TVE_REG16(offset)				IO_REG16(TVE_BASE, (offset))
#define TVE_REG32(offset)				IO_REG32(TVE_BASE, (offset))

/*===========================================================================
// TVE HW "MAIN" register */
#if 0//not support MV
/* Macrovision registers */
#define TVE1_REG_MV1			RW_CVBS_TVE_MV1	//0x400 
#define TVE1_REG_MV2			RW_CVBS_TVE_MV2	//0x404 
#define TVE1_REG_MV3			RW_CVBS_TVE_MV3	//0x408 
#define TVE1_REG_MV4			RW_CVBS_TVE_MV4	//0x40C 
#define TVE1_REG_MV5			RW_CVBS_TVE_MV5	//0x410 
#define TVE1_REG_MV6			RW_CVBS_TVE_MV6	//0x414 
#define TVE1_REG_MV7			RW_CVBS_TVE_MV7	//0x418
#endif
#if defined(CC_MT5387) || defined(CC_MT5363)	
/* Y filter coefficient registers */
#define TVE1_REG_YCOEFF0		RW_CVBS_TVE_YLF_COEA 	//0x500
#define TVE1_REG_YCOEFF1		RW_CVBS_TVE_YLF_COEB 	//0x504
#define TVE1_REG_YCOEFF2		RW_CVBS_TVE_YLF_COEC 	//0x508
#define TVE1_REG_YCOEFF3		RW_CVBS_TVE_YLF_COED 	//0x50C
#define TVE1_REG_YCOEFF4		RW_CVBS_TVE_YLF_COEE 	//0x510

/* UV filter coefficient registers */
#define TVE1_REG_CBCRCOEFF0		RW_CVBS_TVE_CLF_COEA	//0x514
#define TVE1_REG_CBCRCOEFF1		RW_CVBS_TVE_CLF_COEB	//0x518
#endif
/* CbCr filter coefficient registers */
//#define TVE1_REG_CBCRCOEFF0			0x51C           
//#define TVE1_REG_CBCRCOEFF1			0x648           

/* TVE sync for SD and reset */
#define TVE1_REG_ENCSYN			RW_CVBS_TVE_ENCSYN	//0x600
	
/* TVE mode */
#define TVE1_REG_ENCMOD			RW_CVBS_TVE_MODE	//0x604

/* Closed Caption */
#define TVE1_REG_CC					RW_CVBS_TVE_CC		//0x608

/* Y Scale */
#define TVE1_REG_YSCALE			RW_CVBS_TVE_YSCALE	//0x60C

/* Cb Scale */
//#define TVE1_REG_CBSCALE			0x610

/* Cr Scale */
//#define TVE1_REG_CRSCALE			0x614

/* U Scale */
#define TVE1_REG_USCALE			RW_CVBS_TVE_UGAIN 	//0x618

/* V Scale */
#define TVE1_REG_VSCALE			RW_CVBS_TVE_VGAIN	//0x61C

/* Gamma Coefficient 1 to 5 */
#define TVE1_REG_GAMMA1			RW_CVBS_TVE_GAMMA0 	//0x620
#define TVE1_REG_GAMMA2			RW_CVBS_TVE_GAMMA1 	//0x624
#define TVE1_REG_GAMMA3			RW_CVBS_TVE_GAMMA2 	//0x628
#define TVE1_REG_GAMMA4			RW_CVBS_TVE_GAMMA3 	//0x62C
#define TVE1_REG_GAMMA5			RW_CVBS_TVE_GAMMA4 	//0x630

/* DAC Control */
#define TVE1_REG_DACTRL			RW_CVBS_TVE_DACTRL	//0x634

/* Color Burst Configurations */
#define TVE1_REG_BURST			RW_CVBS_TVE_BURST	//0x638
#define TVE1_REG_CHROMAGAIN			RW_CVBS_TVE_CHROMA_GAIN	//0x63C

/* WSS in Progressive Mode */
#define TVE1_REG_WSSP			RW_CVBS_TVE_WSSP  	//0x640

/* WSS in Interlace Mode */
#define TVE1_REG_WSSI			RW_CVBS_TVE_WSSI  	//0x644

/* Cb,Cr Delay and H-Sync Adjust */
//#define TVE1_REG_CBCRDELAY			0x64C

#define TVE1_REG_TT_CTL			RW_CVBS_TVE_TT_CTL //0x648
#define TVE1_REG_TELTXT1			RW_CVBS_TVE_TELTXT1//0x64c
#define TVE1_REG_TELTXT2			RW_CVBS_TVE_TELTXT2//0x650
#define TVE1_REG_TELTXT3			RW_CVBS_TVE_TELTXT3//0x654
#define TVE1_REG_TELTXT4			RW_CVBS_TVE_TELTXT4//0x658
#define TVE1_REG_TELTXT5			RW_CVBS_TVE_TELTXT5//0x65c

#define TVE1_REG_DISP_AREA			RW_CVBS_TVE_DSIP_AREA //0x670
#define TVE1_REG_MUX_CTRL			RW_CVBS_TVE_MUX_CTRL //0x678
#define TVE1_REG_SYNC_CTRL 			RW_CVBS_TVE_SYNC_CTRL //0x67c
#define TVE1_REG_SYNC_TIME0  			RW_CVBS_TVE_SYNC_TIME0 //0x680
#define TVE1_REG_SYNC_TIME1  			RW_CVBS_TVE_SYNC_TIME1 //0x684
#define TVE1_REG_SYNC_TIME2  			RW_CVBS_TVE_SYNC_TIME2 //0x688
#define TVE1_REG_SYNC_TIME3  			RW_CVBS_TVE_SYNC_TIME3 //0x68c

#define TVE1_REG_OSD_FACTORY  			RW_CVBS_TVE_OSD_FACTORY //0x698
/*=======================================================
// TVE SW "MAIN" Registers */

typedef struct _HAL_5391_MAIN_TVE_FIELD_T
{
	/* 0x00 => TVE sync for SD and reset */
	union
	{
		UINT32 u4EncSyn;
		
		struct 
		{
			UINT32 u4Y2HBgn		:	6;	/* Y2HBGN */
			UINT32 u4C2HBgn		:	6;	/* C2HBGN */
			UINT32 fgUvSwap			:	1;	/* UV swap */
			UINT32					:	1;
			UINT32 u4LineShift		:	2;	/* Line shift */

			UINT32 u4SdSynLvl		:	8;	/* SD sync level */
			#if defined(CC_MT5365) || defined(CC_MT5395)
			UINT32					:	1;
			UINT32 u4YLPFCoefSet	:	2;
			UINT32 u4CLPFCoefSet	:	1;
			UINT32 					:	2;
			#else
			UINT32 					:	6;
			#endif
			UINT32 u4Rst			:	2;	/* Reset TVE */
		} rEncSyn;

	} uEncSyn;
	
	/* 0x04 => TVE mode */
	union
	{
		UINT32 u4EncMode;
		
		struct 
		{
			UINT32 fgEncOff			:	1;	/* disable tv encoder */
			UINT32 fgCbOn			:	1;	/* color bar on */
			UINT32 fgSetup			:	1;	/* setup: 7.5 IRE for Composite/S-video */
			UINT32 fgSetup2			:	1;	/* setup2: 7.5 IRE for Component */
			UINT32 					:	1;	/* CbCr low pass filter on */
			UINT32 fgCLPOn			:	1;	/* UV low pass filter on */
			UINT32 					:	1;	/* use programmable burst freq setting */
			UINT32 fgYLPOn			:	1;	/* Y low pass filter on */

			UINT32 u4YDelay			:	2;	/* delay of input Y */
			UINT32 u4SYDelay		:	2;	/* delay of Y in Composite/S-video */
			UINT32 					:	2;	/* delay of Y in Component */
			UINT32 fgUvUpOn			:	1;	/* UV up-sampling on */
			UINT32 					:	1;	/* CBCR up-sampling on */

			UINT32 u4OutMode0		:	2;	/* output mode (Composite/S-Video) */
			//UINT32 u4OutMode1		:	2;	/* output mode (Component, YPbPr/RGB) */
			UINT32 					:	2;
			UINT32 fgGSync			:	1;	/* sync on Y/G channel on */
			UINT32 fgVGA			:	1;	/* MT5351 BA: composite burst advanced */
			UINT32 					:	1;	
			UINT32			 		:	1;	

			UINT32 					:	1;	
			UINT32 fgSlewOff		:	1;	/* SLEW off */
			UINT32 					:	1;	
			UINT32 fgFulW			:	1;	/* full width output on */
			UINT32 fgCbType			:	1;	/* 0: normal color bar (8 colors) */
			UINT32 					:	1;	
			UINT32 fgBlker			:	1;	/* blacker-than-black mode on */
			UINT32 fgTTRst			:	1;	/* rest teltext function */
		} rEncMode;
		
	} uEncMode;

	/* 0x08 => Closed Caption */
	union
	{
		UINT32 u4Cc;
			
		struct 
		{
			UINT32 u4CcData0		:	8;
			UINT32 u4CcData1		:	8;
			UINT32 u4CcMode			:	2;
			UINT32					:	14;
		} rCc;
		
	} uCc;

	/* 0x0C => Y Scale */
	union
	{
		UINT32 u4YScale;
		
		struct
		{
			UINT32 u4YScale			:	8;	/* Gain of Y [7:0] */
			UINT32 u4ColorWidth		:	8;	/* Color bar width */
			UINT32 u4Blank			:	4;	/* Y data at this level will be blank */
			UINT32 u4Bright2			:	4;	/* brightness adjustment */
			UINT32 fgYScale8			:	1;	/* Gain of Y [8] */
			UINT32					:	1;
			UINT32 u4CbDelay		:	2;	/* cb delay */
			UINT32 u4CrDelay		:	2;	/* cr delay */
			UINT32 fgMixUVOffset		:	1;
			UINT32					:	1;
		} rYScale;
		
	} uYScale;

	/* 0x10 => Cb Scale -->TTX test data */
	union
	{
		UINT32 u4TTXTestData;

		struct 
		{
			UINT32 u4TTXTestData0		:	8;
			UINT32 u4TTXTestData1		:	8;
			UINT32 u4TTXTestData2		:	8;
			UINT32 u4TTXTestData3		:	8;
		} rTTXTestData;
		
	} uTTXTestData;
	
	/* 0x14 => Cr Scale remove*/
		
	/* 0x18 => U Scale */
	union
	{
		UINT32 u4UGain;

		struct 
		{
			UINT32 u4UGain0			:	8;
			#if defined(CC_MT5365) || defined(CC_MT5395)
			UINT32			:	24;		
			#else
			UINT32 u4UGain1			:	8;
			UINT32 u4UGain2			:	8;
			UINT32 u4UGain3			:	8;
			#endif
		} rUGain;
		
	} uUGain;
	
	/* 0x1C => V Scale */
	union
	{
		UINT32 u4VGain;

		struct 
		{
			UINT32 u4VGain0			:	8;
			#if defined(CC_MT5365) || defined(CC_MT5395)
			UINT32			:	24;		
			#else
			UINT32 u4VGain1			:	8;
			UINT32 u4VGain2			:	8;
			UINT32 u4VGain3			:	8;
			#endif
		} rVGain;
		
	} uVGain;
	
	/* 0x20 => Gamma Coeff 1 */
	union
	{
		UINT32 u4GammaCoeff1;

		struct 
		{
			UINT32 u4Gamma0			:	12;
			UINT32 					:	12;
			UINT32 u4Bright			:	8;
		} rGammaCoeff1;
		
	} uGammaCoeff1;

	/* 0x24 => Gamma Coeff 2 */
	union
	{
		UINT32 u4GammaCoeff2;

		struct 
		{
			UINT32 u4Gamma1			:	12;
			UINT32 					:	4;
			UINT32 u4Gamma2			:	12;
			UINT32 					:	4;			
		} rGammaCoeff2;
		
	} uGammaCoeff2;

	/* 0x28 => Gamma Coeff 3 */
	union
	{
		UINT32 u4GammaCoeff3;

		struct 
		{
			UINT32 u4Gamma3			:	12;
			UINT32 					:	4;
			UINT32 u4Gamma4			:	12;
			UINT32 					:	4;			
		} rGammaCoeff3;
		
	} uGammaCoeff3;

	/* 0x2C => Gamma Coeff 4 */
	union
	{
		UINT32 u4GammaCoeff4;

		struct 
		{
			UINT32 u4Gamma5			:	12;
			UINT32 					:	4;
			UINT32 u4Gamma6			:	12;
			UINT32 					:	4;			
		} rGammaCoeff4;
		
	} uGammaCoeff4;

	/* 0x30 => Gamma Coeff 5 */
	union
	{
		UINT32 u4GammaCoeff5;

		struct 
		{
			UINT32 u4Gamma7			:	12;
			UINT32 					:	4;
			UINT32 u4Gamma8			:	12;
			UINT32 					:	4;			
		} rGammaCoeff5;
		
	} uGammaCoeff5;

	/* 0x34 => DAC Configuration */
	union
	{
		UINT32 u4DacCtrl;
		
		struct 
		{
			UINT32 u4DAC1			:	2;	
			UINT32 u4DAC2			:	2;	
			UINT32 u4VCOD			:	2;	
			UINT32 u4PlugDetect		:	3;	
			UINT32 fgVref			:	1;	
			UINT32 fgBGRef_off		:	1;	
			UINT32 					:	5;
			UINT32 u4PdBcd			:	2;
			UINT32 u4PdCd			:	2;
			UINT32 u4Bias			:	2;
			UINT32 					:	2;
			UINT32 u4DaTrim			:	5;
			UINT32 					:	3;
		} rDacControl;
		
	} uDacControl;

	/* 0x38 => Color Burst Configuration 1 */
	union
	{
		UINT32 u4Burst1;

		struct 
		{
			UINT32 u4BurstLvl		:	8;
			UINT32 u4Sch			:	7;
			UINT32 fgBf443			:	1;
			UINT32 					:	8;
			UINT32					:	1;
			UINT32 fgExtGainEn		:	1;
			#if defined(CC_MT5365) || defined(CC_MT5395)
			UINT32 u4DemodGain		:	6;
			#else
			UINT32					:	6;
			#endif
		} rBurst1;
		
	} uBurst1;

	/* 0x3C => Chroma Gain for SC */
	union
	{
		UINT32 u4ChromaGain;

		struct 
		{
			UINT32 u4ChromaGain	:	8;
			UINT32 fgChromaGainEn	:	1;
			UINT32				 	:	1;
			UINT32 u4VdoinGain		:	5;	//MT5363, MT5365 or MT5395
			UINT32 fgVdoinGainOff	:	1;	//MT5363
			UINT32 u4Y2ActiveEnd	:	6;	//MT5363
			UINT32				 	:	4;
			UINT32 u4C2ActiveEnd	:	6;	//MT5363
		} rChromaGain;

	} uChromaGain;
		
	
	/* 0x44 => WSS in Interlace Mode (480i) */
	union
	{
		UINT32 u4Wss480i;

		/* for 480i */
		struct 
		{
		    UINT32 u4Word2_Crc	    :	6; 
		    UINT32 	                :	3;  /* b11 ~ b13 */
		    UINT32 u4Word2_b10		:	1;
		    UINT32 u4Word2_b8b9		:	2;
		    UINT32 u4Word2_b6b7		:	2;
		    UINT32 u4Word1		    :	4;
			UINT32 u4Word0		    :	2;  /* MSB go first */
			UINT32 fgWssOn			:	1;	/* Wss enable */
			UINT32 fgFixWssShoot	:	1;	//MT5363
			UINT32					:	2;
			UINT32 u4WssLvl			:	8;	/* Wss Level */
		} rWss480i;
		
	} uWss480i;

	/* 0x44 => WSS in Interlace Mode (576i) */
	union
	{
		UINT32 u4Wss576i;

		/* for 576i */
		struct 
		{
		    UINT32 u4Cgmsa		    :	2;
		    UINT32 		            :	8;
		    UINT32 u4Wss		    :	4;
			UINT32 		            :	6;  /* MSB go first */
			UINT32 fgWssOn			:	1;	/* Wss enable */
			UINT32 fgFixWssShoot	:	1;	//MT5363
			UINT32					:	2;
			UINT32 u4WssLvl			:	8;	/* Wss Level */
		} rWss576i;
		
	} uWss576i;
	
	/* 0x4C => TT_ctrl */
	union
	{
		UINT32 u4TT_ctrl;

		struct 
		{
			UINT32 fgTeltxtEn			:	1;
			UINT32 fgTeltxtTest		:	1;
			UINT32 u4TeltxtLevel		:	3;
			UINT32 		:	27;
		} rTT_ctrl;
		
	} uTT_ctrl;
	
	/* 0x4C => Teltxt1 */
	union
	{
		UINT32 u4Teltxt1;

		struct 
		{
			UINT32 u4TeltxtAddr		:	28;
			UINT32 					:	4;
		} rTeltxt1;
		
	} uTeltxt1;

	/* 0x50 => Teltxt2 */
	union
	{
		UINT32 u4Teltxt2;

		struct 
		{
			UINT32 u4TTEndHcnt		:	11;
			UINT32 					:	4;
			UINT32 fgBusy			:	1;
			UINT32 u4TTBgnHcnt		:	11;
			UINT32 					:	5;
		} rTeltxt2;
		
	} uTeltxt2;

	/* 0x54 => Teltxt3 */
	union
	{
		UINT32 u4Teltxt3;

		struct 
		{
			UINT32 u4TTEndLine		:	5;
			UINT32 					:	3;
			UINT32 u4TTBgnLine		:	5;
			UINT32 					:	3;
			UINT32 u4TTBufrWidth		:	9;
			UINT32 					:	3;
			UINT32 u4TTAddrNum	:	4;
		} rTeltxt3;
		
	} uTeltxt3;

	/* 0x58 => Teltxt4 */
	union
	{
		UINT32 u4Teltxt4;

		struct 
		{
			UINT32 u4TTP1			:	9;
			UINT32 					:	7;
			UINT32 u4TTP2			:	11;
			UINT32 					:	5;
		} rTeltxt4;
		
	} uTeltxt4;

	/* 0x5C => Teltxt5 */
	union
	{
		UINT32 u4Teltxt5;

		struct 
		{
			UINT32 u4TTSyn			:	8;
			UINT32 					:	8;
			UINT32 u4TTCodeEnd		:	9;
			UINT32 					:	7;
		} rTeltxt5;
		
	} uTeltxt5;

	/* 0x68 => UV Scale */
	union
	{
		UINT32 u4UVGain45;

		struct 
		{
			UINT32 u4UGain4			:	8;
			UINT32 u4UGain5			:	8;
			UINT32 u4VGain4			:	8;
			UINT32 u4VGain5			:	8;
		} rUVGain45;
		
	} uUVGain45;
	
	/* 0x6C => Y/G Limit */
	union
	{
		UINT32 u4YGLimit;

		struct 
		{
			UINT32 u4TopThrd		:	8;
			UINT32 u4B0tThrd		:	9;
			UINT32  					:	15;
		} rYGLimit;
		
	} uYGLimit;

	/* 0x70 => DSIP_AREA */
	union
	{
		UINT32 u4DispArea;

		struct 
		{
			UINT32 u4DispEndPxl		:	12;
			UINT32 					:	3;
			UINT32 fgNewDinSel		:	1;
			UINT32 u4DispBgnPxl		:	12;
			UINT32 					:	4;

		} rDispArea;
		
	} uDispArea;

	/* 0x74 => Burst2 */
	union
	{
		UINT32 u4Burst2;

		struct 
		{
			UINT32 u4BurstEnd		:	8;
			UINT32 u4BurstStart		:	8;
			UINT32 u4MvBurstStart	:	8;	
			UINT32 u4AdvMvBurstStart	:	8;	
		} rBurst2;
		
	} uBurst2;	

	/* 0x78 => Mux control */
	union
	{
		UINT32 u4MuxCtrl;

		struct 
		{
			UINT32 fgCVBSMuxEn		:	1;
			UINT32 fgCVBSP2IEn		:	1;
			UINT32 fgVdoInRndEn		:	1;
			UINT32 fgPipRndEn		:	1;
			UINT32 fgVdacTestEN		:	1;
			UINT32 fgCvbsOsdSel		:	1;		//MT5363
			UINT32 fgCvbsOsdSyncSel		:	1;	//MT5363
			UINT32 					:	1;	
			UINT32 fgPrgsSelfEn		:	1;
			UINT32 fgPrgsOut			:	1;
			UINT32 u4TvType			:	2;	
			UINT32 fgPipSyncOff		:	1;
			UINT32 fgPipMixerOn		:	1;	//MT5363
			UINT32 fgPipMixerBlacker	:	1;	//MT5363
			UINT32					:	1;
			UINT32 fgMixer1Sel		:	1;
			UINT32 fgMixer2Sel		:	1;
			UINT32 fgMixerDac1Sel	:	1;	
			UINT32 fgMixerDac2Sel	:	1;
			UINT32					:	2;
			UINT32 fgSIFMixer1Sel	:	1;
			UINT32 fgSIFMixer2Sel	:	1;
			UINT32 					:	8;
		} rMuxCtrl;

	} uMuxCtrl;

	/* 0x7C => Sync Control  */
	union
	{
		UINT32 u4SyncCtrl;
		
		struct 
		{
			UINT32 fgSyncEn		:	1;
			UINT32					:	1;
			UINT32 fgAdjSyncEn	:	1;
			UINT32 fgAdjForward	:	1;
			UINT32					:	1;
			UINT32 fgFieldPol		:	1;
			UINT32 fgVsyncPol	:	1;
			UINT32 fgHsyncPol	:	1;
			
			UINT32 fgHsyncEdgeSel	:	1;
			UINT32 fgVsyncEdgeSel	:	1;
			UINT32 fgAdjSelfEn		:	1;	
			UINT32 fgAdjTotalEn		:	1;
			UINT32 fgSelfYActiveEn	:	1;
			UINT32 					:	3;	

			UINT32 u4YUVSelInit		:	2;
			UINT32 u4YDelay			:	2;
			UINT32 u4CbDelay		:	2;	
			UINT32 u4CrDelay		:	2;

			UINT32 					:	8;
		} rSyncCtrl;

	} uSyncCtrl;

	/* 0x80 => Sync timing Control 0 */
	union
	{
		UINT32 u4SyncTime0;
		
		struct 
		{
			UINT32 u4AdjHTotal		:	12;
			UINT32					:	4;
			UINT32 u4AdjVTotal		:	11;
			UINT32					:	5;
		} rSyncTime0;

	} uSyncTime0;

	/* 0x84 => Sync timing Control 1 */
	union
	{
		UINT32 u4SyncTime1;
		
		struct 
		{
			UINT32 u4AdjHsync		:	12;
			UINT32					:	4;
			UINT32 u4AdjVsync		:	12;
			UINT32					:	4;
		} rSyncTime1;

	} uSyncTime1;

	/* 0x98 => OSD factory */
	union
	{
		UINT32 u4OsdFactory;
		
		struct 
		{
			UINT32 u4OsdHsyncWidth	:	12;
			UINT32 fgOsdHsyncPol	:	1;
			UINT32 fgOsdVsyncPol	:	1;
			UINT32 fgOsdFldPol		:	1;
			UINT32 					:	1;
			UINT32 u4OsdVsyncWidth	:	11;
			UINT32					:	5;
		} rOsdFactory;

	} uOsdFactory;

	#if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
	/* 0x9C => VPS ctrl1 */
	union
	{
		UINT32 u4VpsCtrl1;
		
		struct 
		{
			UINT32 fgVpsOn			:	1;
			UINT32					:	3;
			UINT32 u4LvlAdj			:	3;
			UINT32 fgWaveformType	:	1;
			UINT32 u4VpsP1			:	9;
			UINT32 					:	3;
			UINT32 u4VpsP2			:	11;
			UINT32					:	1;
		} rVpsCtrl1;

	} uVpsCtrl1;

	/* 0xA0 => VPS data1 */
	union
	{
		UINT32 u4VpsData1;
	} uVpsData1;

	/* 0xA4 => VPS data2 */
	union
	{
		UINT32 u4VpsData2;
	} uVpsData2;

	/* 0xA8 => VPS data3 */
	union
	{
		UINT32 u4VpsData3;
	} uVpsData3;

	/* 0xAC => VPS data4 and VPS ctrl2 */
	union
	{
		UINT32 u4VpsCtrl2;

		struct 
		{
			UINT32 u4VpsBgnPxl		:	8;
			UINT32 u4VpsEndPxl		:	8;
			UINT32 u4VpsBgnLine		:	8;
			UINT32 u4VpsData4		:	8;
		} rVpsCtrl2;
	} uVpsCtrl2;	
	#endif
	
	#if defined(CC_MT5387) || defined(CC_MT5363)	
	/* 0x100 => Y Low Pass Filter Coeff 1 */
	union
	{
		UINT32 u4YCoeff1;
		
		struct 
		{
			UINT32 u4YLpf0			:	6;
			UINT32					:	2;
			UINT32 u4YLpf1			:	6;
			UINT32					:	2;
			UINT32 u4YLpf2			:	6;
			UINT32					:	2;
			UINT32 u4YLpf3			:	6;
			UINT32					:	2;
		} rYCoeff1;
		
	} uYCoeff1;

	/* 0x104 => Y Low Pass Filter Coeff 2 */
	union
	{
		UINT32 u4YCoeff2;
		
		struct 
		{
			UINT32 u4YLpf4			:	6;
			UINT32					:	2;
			UINT32 u4YLpf5			:	6;
			UINT32					:	2;
			UINT32 u4YLpf6			:	6;
			UINT32					:	2;
			UINT32 u4YLpf7			:	6;
			UINT32					:	2;
		} rYCoeff2;
		
	} uYCoeff2;

	/* 0x108 => Y Low Pass Filter Coeff 3 */
	union
	{
		UINT32 u4YCoeff3;
		
		struct 
		{
			UINT32 u4YLpf8			:	7;
			UINT32					:	1;
			UINT32 u4YLpf9			:	7;
			UINT32					:	1;
			UINT32 u4YLpf10			:	7;
			UINT32					:	1;
			UINT32 u4YLpf11			:	7;
			UINT32					:	1;
		} rYCoeff3;
		
	} uYCoeff3;

	/* 0x10C => Y Low Pass Filter Coeff 4 */
	union
	{
		UINT32 u4YCoeff4;
		
		struct 
		{
			UINT32 u4YLpf12			:	7;
			UINT32					:	1;
			UINT32 u4YLpf13			:	7;
			UINT32					:	1;
			UINT32 u4YLpf14			:	7;
			UINT32					:	1;
			UINT32 u4YLpf15			:	7;
			UINT32					:	1;
		} rYCoeff4;
		
	} uYCoeff4;

	/* 0x110 => Y Low Pass Filter Coeff 5 */
	union
	{
		UINT32 u4YCoeff5;
		
		struct 
		{
			UINT32 u4YLpf16			:	8;
			UINT32 u4YLpf17			:	8;
			UINT32 u4YLpf18			:	8;
			UINT32 u4YLpf19			:	8;
		} rYCoeff5;
		
	} uYCoeff5;

	/* 0x114 => UV Low Pass Filter Coeff 0 */
	union
	{
		UINT32 u4CbCrCoeff0;
		
		struct 
		{
			UINT32 u4C1Lpf0			:	7;
			UINT32					:	1;
			UINT32 u4C1Lpf1			:	7;
			UINT32					:	1;
			UINT32 u4C1Lpf2			:	7;
			UINT32					:	1;
			UINT32 u4C1Lpf3			:	7;
			UINT32					:	1;
		} rCbCrCoeff0;
		
	} uCbCrCoeff0;

	/* 0x118 => UV Low Pass Filter Coeff 1 */
	union
	{
		UINT32 u4CbCrCoeff1;
		
		struct 
		{
			UINT32 u4C1Lpf4			:	8;
			UINT32 u4C1Lpf5			:	8;
			UINT32 u4C1Lpf6			:	8;
			UINT32					:	8;
		} rCbCrCoeff1;
		
	} uCbCrCoeff1;
	#endif
} HAL_5391_MAIN_TVE_FIELD_T;

#endif /* _TVE_HW_5391_H_ */
