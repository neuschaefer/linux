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
#ifndef _HW_HDMI_H_
#define _HW_HDMI_H_

//---------------------------PDWNC ---------------------------//
#ifndef CC_UP8032_ATV
#define HDMI_DDC_PAD		0x8008
	#define HDMI_DDC_MUX_MASK 	Fld(2, 24, AC_MSKB3) 
#endif

//---------------------------HDCP----------------------------------//
#define HDCP_DEV0 (IO_VIRT + 0x29300)
  #define HDCP_DEV0_0 (IO_VIRT + 0x29300)
  #define HDCP_DEV0_1 (IO_VIRT + 0x29301)
  #define HDCP_DEV0_2 (IO_VIRT + 0x29302)
  #define HDCP_DEV0_3 (IO_VIRT + 0x29303)
    #define OUT_OP0 Fld(1, 9, AC_MSKB1) //9
    #define HDCP_EN0 Fld(1, 8, AC_MSKB1) //8
    #define DEV_ADDR0 Fld(6, 2, AC_MSKB0) // 7:2
#define HDCP_DEV1 (IO_VIRT + 0x29304)
  #define HDCP_DEV1_0 (IO_VIRT + 0x29304)
  #define HDCP_DEV1_1 (IO_VIRT + 0x29305)
  #define HDCP_DEV1_2 (IO_VIRT + 0x29306)
  #define HDCP_DEV1_3 (IO_VIRT + 0x29307)
    #define OUT_OP1 Fld(1, 9, AC_MSKB1) //9
    #define HDCP_EN1 Fld(1, 8, AC_MSKB1) //8
    #define DEV_ADDR1 Fld(6, 2, AC_MSKB0) // 7:2
#define HDCP_DEV2 (IO_VIRT + 0x29308)
  #define HDCP_DEV2_0 (IO_VIRT + 0x29308)
  #define HDCP_DEV2_1 (IO_VIRT + 0x29309)
  #define HDCP_DEV2_2 (IO_VIRT + 0x2930a)
  #define HDCP_DEV2_3 (IO_VIRT + 0x2930b)
    #define OUT_OP2 Fld(1, 9, AC_MSKB1) //9
    #define HDCP_EN2 Fld(1, 8, AC_MSKB1) //8
    #define DEV_ADDR2 Fld(6, 2, AC_MSKB0) // 7:2
#define HDCP_DEV3 (IO_VIRT + 0x2930c)
  #define HDCP_DEV3_0 (IO_VIRT + 0x2930c)
  #define HDCP_DEV3_1 (IO_VIRT + 0x2930d)
  #define HDCP_DEV3_2 (IO_VIRT + 0x2930e)
  #define HDCP_DEV3_3 (IO_VIRT + 0x2930f)
    #define OUT_OP3 Fld(1, 9, AC_MSKB1) //9
    #define HDCP_EN3 Fld(1, 8, AC_MSKB1) //8
    #define DEV_ADDR3 Fld(6, 2, AC_MSKB0) // 7:2
#define HDCP_ADDR (IO_VIRT + 0x29310)
  #define HDCP_ADDR_0 (IO_VIRT + 0x29310)
  #define HDCP_ADDR_1 (IO_VIRT + 0x29311)
  #define HDCP_ADDR_2 (IO_VIRT + 0x29312)
  #define HDCP_ADDR_3 (IO_VIRT + 0x29313)
    #define REG_HDCP_ADDR Fld(7, 0, AC_MSKB0) //6:0
#define HDCP_DATA (IO_VIRT + 0x29314)
  #define HDCP_DATA_0 (IO_VIRT + 0x29314)
  #define HDCP_DATA_1 (IO_VIRT + 0x29315)
  #define HDCP_DATA_2 (IO_VIRT + 0x29316)
  #define HDCP_DATA_3 (IO_VIRT + 0x29317)
    #define REG_HDCP_DATA Fld(32,0,AC_FULLDW)//[31:0]

//---------------------------HDMI Rx PHY ---------------------------//
#define REG_HDMI_RX_Normal_0_CFG0 (IO_VIRT + 0x62000)
  #define REG_HDMI_RX_Normal_0_CFG0_0 (IO_VIRT + 0x62000)
  #define REG_HDMI_RX_Normal_0_CFG0_1 (IO_VIRT + 0x62001)
  #define REG_HDMI_RX_Normal_0_CFG0_2 (IO_VIRT + 0x62002)
  #define REG_HDMI_RX_Normal_0_CFG0_3 (IO_VIRT + 0x62003)
    #define RG_HDMI_0_CDR_RST Fld(1, 31, AC_MSKB3) //31
    #define RG_HDMI_0_CDR_STOP Fld(1, 30, AC_MSKB3) //30
    #define RG_HDMI_0_ATEST_EN Fld(1, 28, AC_MSKB3) //28
    #define RG_HDMI_0_ANAMON Fld(3, 24, AC_MSKB3) //26:24
    #define RG_HDMI_0_CH0_CDRLPI Fld(7, 16, AC_MSKB2) //22:16
    #define RG_HDMI_0_CH1_CDRLPI Fld(7, 8, AC_MSKB1) //14:8
    #define RG_HDMI_0_CH2_CDRLPI Fld(7, 0, AC_MSKB0) //6:0
#define REG_HDMI_RX_Normal_0_CFG1 (IO_VIRT + 0x62004)
  #define REG_HDMI_RX_Normal_0_CFG1_0 (IO_VIRT + 0x62004)
  #define REG_HDMI_RX_Normal_0_CFG1_1 (IO_VIRT + 0x62005)
  #define REG_HDMI_RX_Normal_0_CFG1_2 (IO_VIRT + 0x62006)
  #define REG_HDMI_RX_Normal_0_CFG1_3 (IO_VIRT + 0x62007)
    #define RG_HDMI_0_BAND_SEL_SW Fld(1, 31, AC_MSKB3) //31
    #define RG_HDMI_0_BAND_SEL_SW_EN Fld(1, 30, AC_MSKB3) //30
    #define RG_HDMI_0_CHAN12_EN Fld(1, 29, AC_MSKB3) //29
    #define RG_HDMI_0_CKDT_AEN Fld(1, 28, AC_MSKB3) //28
    #define RG_HDMI_0_CKDT_SET Fld(1, 27, AC_MSKB3) //27
    #define RG_HDMI_0_HYBIAS Fld(2, 25, AC_MSKB3) //26:25
    #define RG_HDMI_0_CLKBUF_EN Fld(1, 24, AC_MSKB3) //24
    #define RG_HDMI_0_DATA_RST Fld(1, 23, AC_MSKB2) //23
    #define RG_HDMI_0_EQ_PD Fld(1, 22, AC_MSKB2) //22
    #define RG_HDMI_0_D2SBIAS Fld(2, 20, AC_MSKB2) //21:20
    #define RG_HDMI_0_EQ_FILT Fld(4, 16, AC_MSKB2) //19:16
    #define RG_HDMI_0_EQ_FIL_SEL Fld(3, 13, AC_MSKB1) //15:13
    #define RG_HDMI_0_EQ_PRBSCK_SEL Fld(1, 12, AC_MSKB1) //12
    #define RG_HDMI_0_EQ_SWRSTSEL Fld(1, 11, AC_MSKB1) //11
    #define RG_HDMI_0_EQ_RST Fld(1, 10, AC_MSKB1) //10
    #define RG_HDMI_0_HY Fld(2, 8, AC_MSKB1) //9:8
    #define RG_HDMI_0_EQ_PATTERN_SEL Fld(1, 7, AC_MSKB0) //7
    #define RG_HDMI_0_EQBIAS Fld(2, 5, AC_MSKB0) //6:5
    #define RG_HDMI_0_FULL_EN Fld(1, 3, AC_MSKB0) //3
    #define RG_HDMI_0_VREF Fld(3, 0, AC_MSKB0) //2:0
#define REG_HDMI_RX_Normal_0_CFG2 (IO_VIRT + 0x62008)
  #define REG_HDMI_RX_Normal_0_CFG2_0 (IO_VIRT + 0x62008)
  #define REG_HDMI_RX_Normal_0_CFG2_1 (IO_VIRT + 0x62009)
  #define REG_HDMI_RX_Normal_0_CFG2_2 (IO_VIRT + 0x6200a)
  #define REG_HDMI_RX_Normal_0_CFG2_3 (IO_VIRT + 0x6200b)
    #define RG_HDMI_0_SEL Fld(3, 29, AC_MSKB3) //29  
    #define RG_HDMI_0_PI_EN Fld(1, 28, AC_MSKB3) //28
    #define RG_HDMI_0_KPGAIN Fld(3, 24, AC_MSKB3) //26:24
    #define RG_HDMI_0_PLLCK_EN Fld(1, 21, AC_MSKB2) //21
    #define RG_HDMI_0_RXAFE_EN_SW Fld(1, 20, AC_MSKB2) //20
    #define RG_HDMI_0_RXAFECH0_EN Fld(1, 19, AC_MSKB2) //19
    #define RG_HDMI_0_RXAFECH1_EN Fld(1, 18, AC_MSKB2) //18
    #define RG_HDMI_0_RXAFECH2_EN Fld(1, 17, AC_MSKB2) //17
    #define RG_HDMI_0_TERM_EN Fld(1, 16, AC_MSKB2) //16
    #define RG_HDMI_0_ZSEL Fld(2, 14, AC_MSKB1) //15:14
    #define RG_HDMI_0_RXMODE Fld(2, 12, AC_MSKB1) //13:12
    #define RG_HDMI_0_TEST_SEL Fld(1, 8, AC_MSKB1) //8
    #define RG_HDMI_0_EQ_GAIN Fld(2, 4, AC_MSKB0) //5:4
    #define RG_HDMI_0_DEEPCLRCLK_PDB Fld(1, 1, AC_MSKB0) //1
    #define RG_HDMI_0_DEEPCLRCLK_RSTN Fld(1, 0, AC_MSKB0) //0
#define REG_HDMI_RX_Normal_0_CFG3 (IO_VIRT + 0x6200c)
  #define REG_HDMI_RX_Normal_0_CFG3_0 (IO_VIRT + 0x6200c)
  #define REG_HDMI_RX_Normal_0_CFG3_1 (IO_VIRT + 0x6200d)
  #define REG_HDMI_RX_Normal_0_CFG3_2 (IO_VIRT + 0x6200e)
  #define REG_HDMI_RX_Normal_0_CFG3_3 (IO_VIRT + 0x6200f)
    #define RG_HDMI_0_PLL_BAND Fld(6, 24, AC_MSKB3) //29:24
    #define RG_HDMI_0_PLL_BC Fld(2, 20, AC_MSKB2) //21:20
    #define RG_HDMI_0_PLL_BIC Fld(3, 16, AC_MSKB2) //18:16
    #define RG_HDMI_0_PLL_BIR Fld(4, 12, AC_MSKB1) //15:12
    #define RG_HDMI_0_PLL_BP Fld(4, 8, AC_MSKB1) //11:8
    #define RG_HDMI_0_PLL_BR Fld(3, 4, AC_MSKB0) //6:4
#define REG_HDMI_RX_Normal_0_CFG4 (IO_VIRT + 0x62010)
  #define REG_HDMI_RX_Normal_0_CFG4_0 (IO_VIRT + 0x62010)
  #define REG_HDMI_RX_Normal_0_CFG4_1 (IO_VIRT + 0x62011)
  #define REG_HDMI_RX_Normal_0_CFG4_2 (IO_VIRT + 0x62012)
  #define REG_HDMI_RX_Normal_0_CFG4_3 (IO_VIRT + 0x62013)
    #define RG_HDMI_0_PLL_DIVEN Fld(6, 24, AC_MSKB3) //29:24
    #define RG_HDMI_0_PLL_FBSEL Fld(2, 22, AC_MSKB2) //23:22
    #define RG_HDMI_0_PLL_BYPASS Fld(1, 21, AC_MSKB2) //21
    #define RG_HDMI_0_PLL_MONEN Fld(1, 20, AC_MSKB2) //20
    #define RG_HDMI_0_PLL_POSDIV Fld(2, 18, AC_MSKB2) //19:18
    #define RG_HDMI_0_PLL_PREDIV Fld(2, 16, AC_MSKB2) //17:16
    #define RG_HDMI_0_PLL_PWD Fld(1, 15, AC_MSKB1) //15
    #define RG_HDMI_0_PLL_VODEN Fld(1, 14, AC_MSKB1) //14
    #define RG_HDMI_0_PLL_FMEN Fld(1, 13, AC_MSKB1) //13
    #define RG_HDMI_0_TESTE Fld(1, 0, AC_MSKB0) //0
#define REG_HDMI_RX_Normal_0_CFG5 (IO_VIRT + 0x62014)
  #define REG_HDMI_RX_Normal_0_CFG5_0 (IO_VIRT + 0x62014)
  #define REG_HDMI_RX_Normal_0_CFG5_1 (IO_VIRT + 0x62015)
  #define REG_HDMI_RX_Normal_0_CFG5_2 (IO_VIRT + 0x62016)
  #define REG_HDMI_RX_Normal_0_CFG5_3 (IO_VIRT + 0x62017)
    #define RG_HDMI_0_REV Fld(32, 0, AC_FULLDW) //31:0
    #define RG_HDMI_0_REV_PLLAUTORST Fld(1, 8, AC_MSKB1) //8    
#define RGS_HDMI_RX_Normal_0_CFG0 (IO_VIRT + 0x62018)
  #define RGS_HDMI_RX_Normal_0_CFG0_0 (IO_VIRT + 0x62018)
  #define RGS_HDMI_RX_Normal_0_CFG0_1 (IO_VIRT + 0x62019)
  #define RGS_HDMI_RX_Normal_0_CFG0_2 (IO_VIRT + 0x6201a)
  #define RGS_HDMI_RX_Normal_0_CFG0_3 (IO_VIRT + 0x6201b)
    #define RGS_HDMI_0_CH0_CDRLPO Fld(7, 24, AC_MSKB3) //30:24
    #define RGS_HDMI_0_CH0_ABIST_FLAG1 Fld(1, 21, AC_MSKB2) //21
    #define RGS_HDMI_0_CH0_ABIST_FLAG2 Fld(1, 20, AC_MSKB2) //20
    #define RGS_HDMI_0_CH0_EQERR Fld(4, 16, AC_MSKB2) //19:16
    #define RGS_HDMI_0_CH0_STATUS Fld(16, 0, AC_FULLW10) //15:0
    #define RGS_HDMI_0_CH0_LOCK Fld(1, 8, AC_MSKB1) //8    
#define RGS_HDMI_RX_Normal_0_CFG1 (IO_VIRT + 0x6201c)
  #define RGS_HDMI_RX_Normal_0_CFG1_0 (IO_VIRT + 0x6201c)
  #define RGS_HDMI_RX_Normal_0_CFG1_1 (IO_VIRT + 0x6201d)
  #define RGS_HDMI_RX_Normal_0_CFG1_2 (IO_VIRT + 0x6201e)
  #define RGS_HDMI_RX_Normal_0_CFG1_3 (IO_VIRT + 0x6201f)
    #define RGS_HDMI_0_CH1_CDRLPO Fld(7, 24, AC_MSKB3) //30:24
    #define RGS_HDMI_0_CH1_ABIST_FLAG1 Fld(1, 21, AC_MSKB2) //21
    #define RGS_HDMI_0_CH1_ABIST_FLAG2 Fld(1, 20, AC_MSKB2) //20
    #define RGS_HDMI_0_CH1_EQERR Fld(4, 16, AC_MSKB2) //19:16
    #define RGS_HDMI_0_CH1_STATUS Fld(16, 0, AC_FULLW10) //15:0
    #define RGS_HDMI_0_CH1_LOCK Fld(1, 8, AC_MSKB1) //8    
#define RGS_HDMI_RX_Normal_0_CFG2 (IO_VIRT + 0x62020)
  #define RGS_HDMI_RX_Normal_0_CFG2_0 (IO_VIRT + 0x62020)
  #define RGS_HDMI_RX_Normal_0_CFG2_1 (IO_VIRT + 0x62021)
  #define RGS_HDMI_RX_Normal_0_CFG2_2 (IO_VIRT + 0x62022)
  #define RGS_HDMI_RX_Normal_0_CFG2_3 (IO_VIRT + 0x62023)
    #define RGS_HDMI_0_CH2_CDRLPO Fld(7, 24, AC_MSKB3) //30:24
    #define RGS_HDMI_0_CH2_ABIST_FLAG1 Fld(1, 21, AC_MSKB2) //21
    #define RGS_HDMI_0_CH2_ABIST_FLAG2 Fld(1, 20, AC_MSKB2) //20
    #define RGS_HDMI_0_CH2_EQERR Fld(4, 16, AC_MSKB2) //19:16
    #define RGS_HDMI_0_CH2_STATUS Fld(16, 0, AC_FULLW10) //15:0
    #define RGS_HDMI_0_CH2_LOCK Fld(1, 8, AC_MSKB1) //8        
#define RGS_HDMI_RX_Normal_0_CFG3 (IO_VIRT + 0x62024)
  #define RGS_HDMI_RX_Normal_0_CFG3_0 (IO_VIRT + 0x62024)
  #define RGS_HDMI_RX_Normal_0_CFG3_1 (IO_VIRT + 0x62025)
  #define RGS_HDMI_RX_Normal_0_CFG3_2 (IO_VIRT + 0x62026)
  #define RGS_HDMI_RX_Normal_0_CFG3_3 (IO_VIRT + 0x62027)
    #define RGS_HDMI_0_CH0_EQMM Fld(1, 31, AC_MSKB3) //31
    #define RGS_HDMI_0_CH1_EQMM Fld(1, 30, AC_MSKB3) //30
    #define RGS_HDMI_0_CH2_EQMM Fld(1, 29, AC_MSKB3) //29


//-----------------------------HDMI---------------------------//
#ifndef CC_UP8032_ATV
#define IO_VIRT_HDMI (0)
#define IO_HDMI_BASE (0xc000)
#else
#define IO_VIRT_HDMI (0)
#define IO_HDMI_BASE (0x4000)
#endif

// hdmi port select
#if defined(CC_MT5396)
#define HDMI_SEL (IO_VIRT_HDMI+(0xc000+0xfc4))
  #define HDMI_SEL_0 (IO_VIRT_HDMI+(0xc000+0xfc4))
  #define HDMI_SEL_1 (IO_VIRT_HDMI+(0xc000+0xfc5))
  #define HDMI_SEL_2 (IO_VIRT_HDMI+(0xc000+0xfc6))
  #define HDMI_SEL_3 (IO_VIRT_HDMI+(0xc000+0xfc7))
    #define REG_INT_OUT_RACHEL_SEL Fld(1, 23, AC_MSKB2) //23
    #define REG_INT_OUT_MAVIS_SEL2 Fld(1, 22, AC_MSKB2) //22
    #define REG_INT_OUT_MAVIS_SEL1 Fld(1, 21, AC_MSKB2) //21
    #define REG_ACR_OUT_RACHEL_SEL Fld(1, 20, AC_MSKB2) //20
    #define REG_ACR_OUT_MAVIS_SEL2 Fld(1, 19, AC_MSKB2) //19
    #define REG_ACR_OUT_MAVIS_SEL1 Fld(1, 18, AC_MSKB2) //18
    #define REG_REGISTER_OUT_RACHEL_SEL Fld(1, 17, AC_MSKB2) //17
    #define REG_REGISTER_OUT_MAVIS_SEL2 Fld(1, 16, AC_MSKB2) //16
    #define REG_REGISTER_OUT_MAVIS_SEL1 Fld(1, 15, AC_MSKB1) //15
    #define REG_AUDIO_PATH_OUT_RACHEL_SEL Fld(1, 14, AC_MSKB1) //14
    #define REG_AUDIO_PATH_OUT_MAVIS_SEL2 Fld(1, 13, AC_MSKB1) //13
    #define REG_AUDIO_PATH_OUT_MAVIS_SEL1 Fld(1, 12, AC_MSKB1) //12
    #define REG_VIDEO_PATH_OUT_RACHEL_SEL Fld(1, 11, AC_MSKB1) //11
    #define REG_VIDEO_PATH_OUT_MAVIS_SEL2 Fld(1, 10, AC_MSKB1) //10
    #define REG_VIDEO_PATH_OUT_MAVIS_SEL1 Fld(1, 9, AC_MSKB1) //9
    #define REG_CLK_IN_RACHEL_SEL Fld(1, 8, AC_MSKB1) //8
    #define REG_CLK_IN_MAVIS_SEL2 Fld(1, 7, AC_MSKB0) //7
    #define REG_CLK_IN_MAVIS_SEL1 Fld(1, 6, AC_MSKB0) //6
    #define REG_AUDIO_PATH_IN_RACHEL_SEL Fld(1, 5, AC_MSKB0) //5
    #define REG_AUDIO_PATH_IN_MAVIS_SEL2 Fld(1, 4, AC_MSKB0)  // 4
    #define REG_AUDIO_PATH_IN_MAVIS_SEL1 Fld(1, 3, AC_MSKB0)  // 3
    #define REG_VIDEO_PATH_IN_RACHEL_SEL Fld(1, 2, AC_MSKB0)  // 2
    #define REG_VIDEO_PATH_IN_MAVIS_SEL2 Fld(1, 1, AC_MSKB0)  // 1
    #define REG_VIDEO_PATH_IN_MAVIS_SEL1 Fld(1, 0, AC_MSKB0)  // 0
#elif  defined(CC_MT5368) || defined(CC_MT5389)
#define HDMI_SEL (IO_VIRT_HDMI+(0xc000+0xfc0))
  #define HDMI_SEL_0 (IO_VIRT_HDMI+(0xc000+0xfc0))
  #define HDMI_SEL_1 (IO_VIRT_HDMI+(0xc000+0xfc1))
  #define HDMI_SEL_2 (IO_VIRT_HDMI+(0xc000+0xfc2))
  #define HDMI_SEL_3 (IO_VIRT_HDMI+(0xc000+0xfc3))
    #define REG_INT_OUT_RACHEL_SEL  Fld(1, 25, AC_MSKB3) //25
    #define REG_INT_OUT_MAVIS_SEL  Fld(1, 24, AC_MSKB3) //24
    #define REG_ACR_OUT_RACHEL_SEL Fld(1, 23, AC_MSKB2) //23
    #define REG_ACR_OUT_MAVIS_SEL Fld(1, 22, AC_MSKB2) //22
    #define REG_REGISTER_OUT_RACHEL_SEL Fld(1, 21, AC_MSKB2) //21
    #define REG_REGISTER_OUT_MAVIS_SEL Fld(1, 20, AC_MSKB2) //20
    #define REG_AUDIO_PATH_OUT_RACHEL_SEL Fld(1, 19, AC_MSKB2) //19
    #define REG_AUDIO_PATH_OUT_MAVIS_SEL  Fld(1, 18, AC_MSKB2) //18
    #define REG_VIDEO_PATH_OUT_RACHEL_SEL  Fld(1, 17, AC_MSKB2) //17
    #define REG_VIDEO_PATH_OUT_MAVIS_SEL  Fld(1, 16, AC_MSKB2) //16
    #define REG_ACR_IN_RACHEL_SEL Fld(1, 15, AC_MSKB1) //15
    #define REG_ACR_IN_MAVIS_SEL Fld(1, 14, AC_MSKB1) //14
    #define REG_CLK_IN_RACHEL_SEL Fld(1, 13, AC_MSKB1) //13
    #define REG_CLK_IN_MAVIS_SEL Fld(1, 12, AC_MSKB1) //12
    #define REG_AUDIO_PATH_IN_RACHEL_SEL Fld(1, 11, AC_MSKB1) //11 
    #define REG_AUDIO_PATH_IN_MAVIS_SEL Fld(1, 10, AC_MSKB1) //10
    #define REG_VIDEO_PATH_IN_RACHEL_SEL Fld(1, 9, AC_MSKB1) //9
    #define REG_VIDEO_PATH_IN_MAVIS_SEL Fld(1, 8, AC_MSKB1) //8
#endif

#define HDMI_ID (IO_VIRT_HDMI+(IO_HDMI_BASE+0x000))
  #define HDMI_ID_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x000))
  #define HDMI_ID_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x001))
  #define HDMI_ID_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x002))
  #define HDMI_ID_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x003))
    #define DEV_ID Fld(16, 16, AC_FULLW32) //31:16
    #define VHDL_ID Fld(16, 0, AC_FULLW10) //15:0
#define SRST (IO_VIRT_HDMI+(IO_HDMI_BASE+0x004))
  #define SRST_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x004))
  #define SRST_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x005))
  #define SRST_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x006))
  #define SRST_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x007))
    #define DEEP_RST Fld(1, 31, AC_MSKB3) //31
    #define BYPASS_DP Fld(1, 30, AC_MSKB3) //30
    #define DEEP_STA Fld(2, 28, AC_MSKB3) //29:28
    #define BYPASS_VPROC_AUTO Fld(1, 27, AC_MSKB3) //27
    #define BYPASS_VPROC Fld(1, 26, AC_MSKB3) //26
    #define PHASE_OFFSET Fld(2, 24, AC_MSKB3) //25:24
    #define PWR5V_RX2 Fld(1, 23, AC_MSKB2) //23
    #define PWR5V_RX1 Fld(1, 22, AC_MSKB2) //22
    #define CKDTIN_HDMI Fld(1, 21, AC_MSKB2) //21
    #define BAND_SEL_HW Fld(1, 20, AC_MSKB2) //20
    #define PWR5V_RX0 Fld(1, 19, AC_MSKB2) //19
    #define VSYNC_CAP Fld(1, 18, AC_MSKB2) //18
    #define CKDT Fld(1, 17, AC_MSKB2) //17
    #define SCDT Fld(1, 16, AC_MSKB2) //16
    #define HDCP_RST_AUDO Fld(1, 15, AC_MSKB1) //15
    #define ACR_RST_AUTO Fld(1, 14, AC_MSKB1) //14
    #define AAC_RST Fld(1, 13, AC_MSKB1) //13
    #define SW_RST_AUTO Fld(1, 12, AC_MSKB1) //12
    #define HDCP_RST Fld(1, 11, AC_MSKB1) //11
    #define ACR_RST Fld(1, 10, AC_MSKB1) //10
    #define FIFO_RST Fld(1, 9, AC_MSKB1) //9
    #define SW_RST Fld(1, 8, AC_MSKB1) //8
    #define REV_ID Fld(8, 0, AC_FULLB0) //7:0
#define SYS_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x008))
  #define SYS_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x008))
  #define SYS_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x009))
  #define SYS_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x00a))
  #define SYS_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x00b))
    #define DDC_DEL_EN Fld(1, 15, AC_MSKB1) //15
    #define LONG_STA_HOLD Fld(1, 13, AC_MSKB1) //13
    #define DDC_EN Fld(1, 12, AC_MSKB1) //12
    #define SYS_CTRL_1T_PULSE_EN Fld(1, 11, AC_MSKB1) //11
    #define PLL_COREID Fld(1, 10, AC_MSKB1) //10
    #define SYS_RST Fld(1, 9, AC_MSKB1) //9
    #define RX_EN Fld(1, 8, AC_MSKB1) //8
    #define OCLKDIV Fld(2, 6, AC_MSKB0) //7:6
    #define IDCK Fld(2, 4, AC_MSKB0) //5:4
    #define PIXS Fld(1, 3, AC_MSKB0) //3
    #define BSEL_HDMI Fld(1, 2, AC_MSKB0) //2
    #define EDGE Fld(1, 1, AC_MSKB0) //1
    #define PD_ALL Fld(1, 0, AC_MSKB0) //0
#define VID_SET (IO_VIRT_HDMI+(IO_HDMI_BASE+0x010))
  #define VID_SET_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x010))
  #define VID_SET_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x011))
  #define VID_SET_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x012))
  #define VID_SET_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x013))
    #define CP_PKT_EN Fld(1, 31, AC_MSKB3) //31
    #define AV_MUTE_CLR Fld(1, 30, AC_MSKB3) //30
    #define BYPASS_T4_PKT_ERR Fld(1, 29, AC_MSKB3) //29
    #define HDCP_DEC_CAP Fld(1, 28, AC_MSKB3) //28
    #define CEA_NEW_UNREC_SEL Fld(1, 27, AC_MSKB3) //27
    #define CLR_SUM_TH Fld(1, 26, AC_MSKB3) //26
    #define S3C3_SET_MUTE Fld(1, 25, AC_MSKB3) //25
    #define S3C3_CLR_MUTE Fld(1, 24, AC_MSKB3) //24
    #define H_VDO_GEN Fld(1, 23, AC_MSKB2) //23
    #define V_VDO_GEN Fld(1, 22, AC_MSKB2) //22
    #define C_INT_BYP Fld(1, 21, AC_MSKB2) //21
    #define E_VDO_GEN Fld(1, 19, AC_MSKB2) //19
    #define R_VDO_GEN Fld(1, 18, AC_MSKB2) //18
    #define G_VDO_GEN Fld(1, 17, AC_MSKB2) //17
    #define B_VDO_GEN Fld(1, 16, AC_MSKB2) //16
    #define EERPOM_ID Fld(6, 10, AC_MSKB1) //15:10
    #define EEPROM_ID_SW Fld(2, 8, AC_MSKB1) //9:8
    #define KSCL_H Fld(1, 7, AC_MSKB0) //7
    #define MSIF_WIGLE_EN Fld(1, 6, AC_MSKB0) //6
    #define B7_INV Fld(1, 5, AC_MSKB0) //5
    #define B6_INV Fld(1, 4, AC_MSKB0) //4
    #define H_V_DE_SWAP Fld(1, 3, AC_MSKB0) //3
    #define DE_INV Fld(1, 2, AC_MSKB0) //2
    #define AUTO_OCLKDIV_EN Fld(1, 1, AC_MSKB0) //1
    #define MUTE_CKDT Fld(1, 0, AC_MSKB0) //0
#define MUTE_SET (IO_VIRT_HDMI+(IO_HDMI_BASE+0x014))
  #define MUTE_SET_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x014))
  #define MUTE_SET_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x015))
  #define MUTE_SET_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x016))
  #define MUTE_SET_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x017))
    #define DDC_DEGLITCH_EN Fld(1, 31, AC_MSKB3) //31
    #define BYPASS_CTL3_FSM Fld(1, 30, AC_MSKB3) //30
    #define BYPASS_CNTL3 Fld(1, 29, AC_MSKB3) //29
    #define BYPASS_CNTL2 Fld(1, 28, AC_MSKB3) //28
    #define BYPASS_CNTL0 Fld(1, 27, AC_MSKB3) //27
    #define BYPASS_DI_GB Fld(1, 26, AC_MSKB3) //26
    #define BYPASS_DEC_HV Fld(1, 25, AC_MSKB3) //25
    #define AUTO_DEC_HV Fld(1, 24, AC_MSKB3) //24
    #define OP_WINDOW_WIDE Fld(1, 23, AC_MSKB2) //23
    #define BYPASS_CTL3_FSM_DVI Fld(1, 22, AC_MSKB2) //22
    #define ZERO_SEL Fld(1, 21, AC_MSKB2) //21
    #define TDFIFO_SYNC_EN Fld(1, 20, AC_MSKB2) //20
		#define NO_VSYNC_EN Fld(1, 19, AC_MSKB2)//19
    #define AUTO_MUTE_VIDEO Fld(1, 7, AC_MSKB0) //7
    #define AUTO_MUTE_AUDIO Fld(1, 6, AC_MSKB0) //6
    #define AUTO_MUTE_CTL3 Fld(1, 5, AC_MSKB0) //5
#define SHD_BKSV0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x018))
  #define SHD_BKSV0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x018))
  #define SHD_BKSV0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x019))
  #define SHD_BKSV0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x01a))
  #define SHD_BKSV0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x01b))
    #define HDCP_BKSV2 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_BKSV1 Fld(8, 16, AC_FULLB2) //23:16
#define SHD_BKSV1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x01c))
  #define SHD_BKSV1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x01c))
  #define SHD_BKSV1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x01d))
  #define SHD_BKSV1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x01e))
  #define SHD_BKSV1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x01f))
	#define HDCP_RI0_15_8 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_BKSV5 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_BKSV4 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_BKSV3 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_AKSV (IO_VIRT_HDMI+(IO_HDMI_BASE+0x020))
  #define SHD_AKSV_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x020))
  #define SHD_AKSV_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x021))
  #define SHD_AKSV_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x022))
  #define SHD_AKSV_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x023))
    #define HDCP_AKSV3 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_AKSV2 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_AKSV1 Fld(8, 8, AC_FULLB1) //15:8
	#define HDCP_RI0_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_AN0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x024))
  #define SHD_AN0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x024))
  #define SHD_AN0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x025))
  #define SHD_AN0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x026))
  #define SHD_AN0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x027))
    #define HDCP_AN2 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_AN1 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_AKSV5 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_AKSV4 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_AN1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x028))
  #define SHD_AN1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x028))
  #define SHD_AN1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x029))
  #define SHD_AN1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x02a))
  #define SHD_AN1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x02b))
    #define HDCP_AN6 Fld(8, 24, AC_FULLB3) //31:24
    #define HDCP_AN5 Fld(8, 16, AC_FULLB2) //23:16
    #define HDCP_AN4 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_AN3 Fld(8, 0, AC_FULLB0) //7:0
#define SHD_BSTATUS (IO_VIRT_HDMI+(IO_HDMI_BASE+0x02c))
  #define SHD_BSTATUS_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x02c))
  #define SHD_BSTATUS_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x02d))
  #define SHD_BSTATUS_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x02e))
  #define SHD_BSTATUS_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x02f))
    #define DEV_EXCEED Fld(1, 31, AC_MSKB3) //31
    #define DEV_CNT Fld(7, 24, AC_MSKB3) //30:24
    #define HDMI_CAPABLE Fld(1, 23, AC_MSKB2) //23
    #define REPEATER Fld(1, 22, AC_MSKB2) //22
    #define FIFO_RDY Fld(1, 21, AC_MSKB2) //21
    #define FAST Fld(1, 20, AC_MSKB2) //20
    #define FIFO_RDY_WP Fld(1, 16, AC_MSKB2) //16
    #define HDCP_AN8 Fld(8, 8, AC_FULLB1) //15:8
    #define HDCP_AN7 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_STAT (IO_VIRT_HDMI+(IO_HDMI_BASE+0x030))
  #define HDCP_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x030))
  #define HDCP_STAT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x031))
  #define HDCP_STAT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x032))
  #define HDCP_STAT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x033))
    #define HDCP_DECRYPT Fld(1, 21, AC_MSKB2) //21
    #define HDCP_AUTH Fld(1, 20, AC_MSKB2) //20
    #define HDCP_SPHST Fld(4, 16, AC_MSKB2) //19:16
    #define HDCP_DEBUG_WP Fld(1, 15, AC_MSKB1) //15
    #define HDCP_DEBUG Fld(7, 8, AC_MSKB1) //14:8
    #define BSTATUS Fld(3, 5, AC_MSKB0) //7:5
    #define RHDMI_MODE_EN Fld(1, 4, AC_MSKB0) //4
    #define RPT_CASE_EXCEED Fld(1, 3, AC_MSKB0) //3
    #define DEPTH Fld(3, 0, AC_MSKB0) //2:0
#define SHA_LENGTH (IO_VIRT_HDMI+(IO_HDMI_BASE+0x034))
  #define SHA_LENGTH_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x034))
  #define SHA_LENGTH_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x035))
  #define SHA_LENGTH_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x036))
  #define SHA_LENGTH_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x037))
    #define SHA_MODE Fld(1, 26, AC_MSKB3) //26
    #define RPT_V_RDY Fld(1, 24, AC_MSKB3) //24
	#define RPT_START_ADDR_9_8 Fld(2, 18, AC_MSKB2) //19:18
	#define SHA_LENGTH_9_8 Fld(2, 16, AC_MSKB2) //17:16
	#define SHA_LENGTH_7_0 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_START_ADDR_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define VID_HRES (IO_VIRT_HDMI+(IO_HDMI_BASE+0x038))
  #define VID_HRES_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x038))
  #define VID_HRES_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x039))
  #define VID_HRES_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x03a))
  #define VID_HRES_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x03b))
    #define VID_HRES_STB Fld(1, 31, AC_MSKB3) //31
    #define VRES_CHG_DET_SEL Fld(1,30,AC_MSKB3)//[30:30]
    #define INTERLACE_DET_SEL Fld(1,29,AC_MSKB3)//[29:29]
    #define VID_HRES_12_0 Fld(13, 16, AC_MSKW32) //28:16
    #define HCHG_CNT_THR Fld(4, 12, AC_MSKB1) //15:12
    #define HSTB_CNT_THR Fld(4, 8, AC_MSKB1) //11:8
    #define FLD_DET_SEL Fld(1, 7, AC_MSKB0) //7
    #define FLD_DET_INI Fld(1, 6, AC_MSKB0) //6
    #define HRES_THR Fld(6, 0, AC_MSKB0) //5:0
#define VID_VRES (IO_VIRT_HDMI+(IO_HDMI_BASE+0x03c))
  #define VID_VRES_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x03c))
  #define VID_VRES_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x03d))
  #define VID_VRES_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x03e))
  #define VID_VRES_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x03f))
    #define VCHG_CNT_THR Fld(4, 28, AC_MSKB3) //31:28
    #define VSTB_CNT_THR Fld(4, 24, AC_MSKB3) //27:24
    #define VRES_MUTE_AUTO_CLR Fld(1, 23, AC_MSKB2) //23
    #define VRES_MUTE_CLR Fld(1, 22, AC_MSKB2) //22
    #define VRES_THR Fld(6, 16, AC_MSKB2) //21:16
    #define VID_VRES_STB Fld(1, 15, AC_MSKB1) //15
    #define VID_VRES_MUTE Fld(1, 14, AC_MSKB1) //14
    #define VID_VRES_11_8 Fld(4, 8, AC_MSKB1) //11:8
    #define VID_VRES_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define VID_MODE (IO_VIRT_HDMI+(IO_HDMI_BASE+0x048))
  #define VID_MODE_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x048))
  #define VID_MODE_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x049))
  #define VID_MODE_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x04a))
  #define VID_MODE_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x04b))
    #define BLANKDATA1 Fld(8, 24, AC_FULLB3) //31:24
    #define ENSYNCCODES Fld(1, 23, AC_MSKB2) //23
    #define ENMUXYC Fld(1, 22, AC_MSKB2) //22
    #define ENDITHER Fld(1, 21, AC_MSKB2) //21
    #define ENRGB2YCBCRRANGE Fld(1, 20, AC_MSKB2) //20
    #define ENUPSAMPLE Fld(1, 18, AC_MSKB2) //18
    #define ENDOWNSAMPLE Fld(1, 17, AC_MSKB2) //17
    #define ENYCBCR2RGBRANGE Fld(1, 11, AC_MSKB1) //11
    #define INVERTVSYNC Fld(1, 7, AC_MSKB0) //7
    #define INVERTHSYNC Fld(1, 6, AC_MSKB0) //6
    #define YCBCR2RGBMODE Fld(1, 2, AC_MSKB0) //2
    #define EXTBITMODE Fld(1, 1, AC_MSKB0) //1
    #define RGB2YCBCRMODE Fld(1, 0, AC_MSKB0) //0
#define VID_BLANK (IO_VIRT_HDMI+(IO_HDMI_BASE+0x04c))
  #define VID_BLANK_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x04c))
  #define VID_BLANK_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x04d))
  #define VID_BLANK_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x04e))
  #define VID_BLANK_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x04f))
    #define INTERLACE_LINE_DIFF_TH Fld(4,28,AC_MSKB3)//[31:28]
	#define VID_DEPIXELS_11_8 Fld(4, 24, AC_MSKB3) //27:24
	#define VID_DEPIXELS_7_0 Fld(8, 16, AC_FULLB2) //23:16
	#define VID_DEPIXELS_11_0 Fld(12, 16, AC_MSKW32) //27:16	
    #define BLANKDATA3 Fld(8, 8, AC_FULLB1) //15:8
    #define BLANKDATA2 Fld(8, 0, AC_FULLB0) //7:0
#define VID_STAT (IO_VIRT_HDMI+(IO_HDMI_BASE+0x050))
  #define VID_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x050))
  #define VID_STAT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x051))
  #define VID_STAT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x052))
  #define VID_STAT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x053))
    #define VFRONTPORCH Fld(6, 24, AC_MSKB3) //29:24
    #define V2ACTIVELINES Fld(6, 16, AC_MSKB2) //21:16
	#define VID_DELINES_11_8 Fld(4, 8, AC_MSKB1) //11:8
	#define VID_DELINES_7_0 Fld(8, 0, AC_FULLB0) //7:0
	#define VID_DELINES_11_0 Fld(12, 0, AC_MSKW10) //11:0	
#define VID_CH_MAP (IO_VIRT_HDMI+(IO_HDMI_BASE+0x054))
  #define VID_CH_MAP_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x054))
  #define VID_CH_MAP_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x055))
  #define VID_CH_MAP_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x056))
  #define VID_CH_MAP_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x057))
    #define CHANNEL_MAP Fld(3, 16, AC_MSKB2) //18:16
    #define INTERLACEDOUT Fld(1, 10, AC_MSKB1) //10
    #define VSYNCPOL Fld(1, 9, AC_MSKB1) //9
    #define F2BACKPORCH Fld(1, 0, AC_MSKB0) //0
#define VID_HFP (IO_VIRT_HDMI+(IO_HDMI_BASE+0x058))
  #define VID_HFP_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x058))
  #define VID_HFP_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x059))
  #define VID_HFP_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x05a))
  #define VID_HFP_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x05b))
	#define VID_HSACTIVEWIDTH_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define VID_HFRONTPORCH Fld(8, 8, AC_FULLB1) //15:8
#define VID_AOF (IO_VIRT_HDMI+(IO_HDMI_BASE+0x05c))
  #define VID_AOF_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x05c))
  #define VID_AOF_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x05d))
  #define VID_AOF_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x05e))
  #define VID_AOF_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x05f))
    #define VIDOUTMODE Fld(8, 24, AC_FULLB3) //31:24
	#define VID_HSACTIVEWIDTH_9_8 Fld(2, 0, AC_MSKB0) //1:0
#define MCM_REPDT (IO_VIRT_HDMI+(IO_HDMI_BASE+0x064))
  #define MCM_REPDT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x064))
  #define MCM_REPDT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x065))
  #define MCM_REPDT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x066))
  #define MCM_REPDT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x067))
	#define GPIO_EN_15_8 Fld(8, 24, AC_FULLB3) //31:24
	#define GPIO_EN_7_0 Fld(8, 16, AC_FULLB2) //23:16
    #define STA_EPDT Fld(8, 8, AC_FULLB1) //15:8
#define VID_CRC_CHK (IO_VIRT_HDMI+(IO_HDMI_BASE+0x068))
  #define VID_CRC_CHK_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x068))
  #define VID_CRC_CHK_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x069))
  #define VID_CRC_CHK_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x06a))
  #define VID_CRC_CHK_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x06b))
    #define HDMI_STA_CRC_OUT_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define STA_CRC_RDY Fld(1, 23, AC_MSKB2) //23
    #define STA_CRC_FAIL Fld(1, 22, AC_MSKB2) //22
    #define PLLCLK_DIV Fld(2, 20, AC_MSKB2) //21:20
    #define CRC_MODE Fld(2, 18, AC_MSKB2) //19:18
    #define TEST_DIN_SEL Fld(1, 17, AC_MSKB2) //17
    #define CRC_START_HDMI Fld(1, 16, AC_MSKB2) //16
    #define HDMI_STA_CRC_OUT_23_16 Fld(8, 8, AC_FULLB1) //15:8
    #define HDMI_STA_CRC_OUT_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define VID_CRC_OUT (IO_VIRT_HDMI+(IO_HDMI_BASE+0x06c))
  #define VID_CRC_OUT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x06c))
  #define VID_CRC_OUT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x06d))
  #define VID_CRC_OUT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x06e))
  #define VID_CRC_OUT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x06f))

    #define AAC_XCLK_IN_PCLK_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define XCLK_IN_PCLK_SEL Fld(1, 23, AC_MSKB2) //23
    #define XCLK_IN_DPCLK Fld(1, 22, AC_MSKB2) //22
    #define CCIR_MTK Fld(1, 21, AC_MSKB2) //21
    #define RD_BU_KS_EN Fld(1, 20, AC_MSKB2) //20
    #define AAC_XCLK_IN_PCLK_10_8 Fld(3, 16, AC_MSKB2) //18:16
		#define ABC_XCLK_DIFF_TH Fld(8, 0, AC_FULLB0)//7:0
#define INTR_STATE0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x070))
  #define INTR_STATE0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x070))
  #define INTR_STATE0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x071))
  #define INTR_STATE0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x072))
  #define INTR_STATE0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x073))
    #define INTR3_CEA_NEW_CP Fld(1, 31, AC_MSKB3) //31
    #define INTR3_CP_SET_MUTE Fld(1, 30, AC_MSKB3) //30
    #define INTR3_P_ERR Fld(1, 29, AC_MSKB3) //29
    #define INTR3_NEW_UNREC Fld(1, 28, AC_MSKB3) //28
    #define INTR3_NEW_MPEG Fld(1, 27, AC_MSKB3) //27
    #define INTR3_NEW_AUD Fld(1, 26, AC_MSKB3) //26
    #define INTR3_NEW_SPD Fld(1, 25, AC_MSKB3) //25
    #define INTR3_NEW_AVI Fld(1, 24, AC_MSKB3) //24
    #define INTR2_HDMI_MODE Fld(1, 23, AC_MSKB2) //23
    #define INTR2_VSYNC Fld(1, 22, AC_MSKB2) //22
    #define INTR2_SOFT_INTR_EN Fld(1, 21, AC_MSKB2) //21
    #define INTR2_CKDT Fld(1, 20, AC_MSKB2) //20
    #define INTR2_SCDT Fld(1, 19, AC_MSKB2) //19
    #define INTR2_GOT_CTS Fld(1, 18, AC_MSKB2) //18
    #define INTR2_NEW_AUD_PKT Fld(1, 17, AC_MSKB2) //17
    #define INTR2_CLK_CHG Fld(1, 16, AC_MSKB2) //16
    #define INTR1_HW_CTS_CHG Fld(1, 15, AC_MSKB1) //15
    #define INTR1_HW_N_CHG Fld(1, 14, AC_MSKB1) //14
    #define INTR1_PKT_ERR Fld(1, 13, AC_MSKB1) //13
    #define INTR1_PLL_UNLOCKED Fld(1, 12, AC_MSKB1) //12
    #define INTR1_FIFO_ERR Fld(1, 11, AC_MSKB1) //11
    #define INTR1_BCH_PKT_ERR_ALERT Fld(1, 10, AC_MSKB1) //10
    #define INTR1_AUTH_START Fld(1, 9, AC_MSKB1) //9
    #define INTR1_AUTH_DONE Fld(1, 8, AC_MSKB1) //8
    #define SOFT_INTR_EN Fld(1, 3, AC_MSKB0) //3
    #define INTR_OD Fld(1, 2, AC_MSKB0) //2
    #define INTR_POLARITY Fld(1, 1, AC_MSKB0) //1
    #define INTR_STATE Fld(1, 0, AC_MSKB0) //0
#define INTR_MASK0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x074))
  #define INTR_MASK0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x074))
  #define INTR_MASK0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x075))
  #define INTR_MASK0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x076))
  #define INTR_MASK0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x077))
    #define INTR3_MASK Fld(8, 24, AC_FULLB3) //31:24
    #define INTR2_MASK Fld(8, 16, AC_FULLB2) //23:16
    #define INTR1_MASK Fld(8, 8, AC_FULLB1) //15:8
    #define INTR_AUD_PACKET Fld(1, 26, AC_MSKB3) //26 
    #define NEW_ACP_ONLY Fld(1, 5, AC_MSKB0) //5
    #define NEW_UNREC_ONLY Fld(1, 4, AC_MSKB0) //4
    #define NEW_MPEG_ONLY Fld(1, 3, AC_MSKB0) //3
    #define NEW_AUD_ONLY Fld(1, 2, AC_MSKB0) //2
    #define NEW_SPD_ONLY Fld(1, 1, AC_MSKB0) //1
    #define NEW_AVI_ONLY Fld(1, 0, AC_MSKB0) //0
#define INTR_STATE1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x078))
  #define INTR_STATE1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x078))
  #define INTR_STATE1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x079))
  #define INTR_STATE1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x07a))
  #define INTR_STATE1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x07b))
		#define INTR7_RATIO_ERROR Fld(1, 29, AC_MSKB3)//29
    #define INTR7_AUD_CH_STAT Fld(1, 28, AC_MSKB3) //28
    #define INTR7_GCP_CD_CHG Fld(1, 27, AC_MSKB3) //27
    #define INTR7_GAMUT Fld(1, 26, AC_MSKB3) //26
    #define INTR7_HBR Fld(1, 25, AC_MSKB3) //25
    #define INTR7_SACD Fld(1, 24, AC_MSKB3) //24
    #define INTR6_PRE_UNDERUN Fld(1, 23, AC_MSKB2) //23
    #define INTR6_PRE_OVERUN Fld(1, 22, AC_MSKB2) //22
    #define INTR6_PWR5V_RX2 Fld(1, 20, AC_MSKB2) //20
    #define INTR6_PWR5V_RX1 Fld(1, 19, AC_MSKB2) //19
    #define INTR6_NEW_ACP Fld(1, 18, AC_MSKB2) //18
    #define INTR6_P_ERR2 Fld(1, 17, AC_MSKB2) //17
    #define INTR6_PWR5V_RX0 Fld(1, 16, AC_MSKB2) //16
    #define INTR5_FN_CHG Fld(1, 15, AC_MSKB1) //15
    #define INTR5_AUDIO_MUTE Fld(1, 14, AC_MSKB1) //14
    #define INTR5_BCH_AUDIO_ALERT Fld(1, 13, AC_MSKB1) //13
    #define INTR5_VRESCHG Fld(1, 12, AC_MSKB1) //12
    #define INTR5_HRESCHG Fld(1, 11, AC_MSKB1) //11
    #define INTR5_POLCHG Fld(1, 10, AC_MSKB1) //10
    #define INTR5_INTERLACEOUT Fld(1, 9, AC_MSKB1) //9
    #define INTR5_AUD_SAMPLE_F Fld(1, 8, AC_MSKB1) //8
    #define INTR4_PKT_RECEIVED_ALERT Fld(1, 7, AC_MSKB0) //7
    #define INTR4_HDCP_PKT_ERR_ALERT Fld(1, 6, AC_MSKB0) //6
    #define INTR4_T4_PKT_ERR_ALERT Fld(1, 5, AC_MSKB0) //5
    #define INTR4_NO_AVI Fld(1, 4, AC_MSKB0) //4
    #define INTR4_CTS_DROPPED_ERR Fld(1, 3, AC_MSKB0) //3
    #define INTR4_CTS_REUSED_ERR Fld(1, 2, AC_MSKB0) //2
    #define INTR4_OVERRUN Fld(1, 1, AC_MSKB0) //1
    #define INTR4_UNDERRUN Fld(1, 0, AC_MSKB0) //0
#define INTR_MASK1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x07c))
  #define INTR_MASK1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x07c))
  #define INTR_MASK1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x07d))
  #define INTR_MASK1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x07e))
  #define INTR_MASK1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x07f))
    #define INTR7_MASK Fld(5, 24, AC_MSKB3) //28:24
    #define INTR6_MASK7_2 Fld(6, 18, AC_MSKB2) //23:18
    #define INTR6_MASK0 Fld(1, 16, AC_MSKB2) //16
    #define INTR5_MASK Fld(8, 8, AC_FULLB1) //15:8
    #define INTR4_MASK Fld(8, 0, AC_FULLB0) //7:0
#define TMDS_ECTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x080))
  #define TMDS_ECTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x080))
  #define TMDS_ECTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x081))
  #define TMDS_ECTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x082))
  #define TMDS_ECTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x083))
    #define PLLCKENB Fld(1, 3, AC_MSKB0) //3
#define N_HDMI_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x084))
  #define N_HDMI_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x084))
  #define N_HDMI_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x085))
  #define N_HDMI_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x086))
  #define N_HDMI_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x087))
		#define TT2_EXT_PKT_EN Fld(1, 31, AC_MSKB3)//31
    #define TT2_GCP_ERR_CLR_EN Fld(1, 30, AC_MSKB3) //30
    #define TT2_CTSMUL4_EN Fld(1, 29, AC_MSKB3) //29
    #define TT2_NMUL4_EN Fld(1, 28, AC_MSKB3) //28
    #define TT2_CTS_DIV4_CARRY_EN Fld(1, 27, AC_MSKB3) //27
    #define TT2_CTSDIV4_EN Fld(1, 26, AC_MSKB3) //26
    #define TT2_NDIV4_CARRY_EN Fld(1, 25, AC_MSKB3) //25
    #define TT2_NDIV4_EN Fld(1, 24, AC_MSKB3) //24
    #define TT1_SPDIF_IGN_CH Fld(1, 23, AC_MSKB2) //23
    #define TT1_SPDIF_PICK_8CH Fld(1, 22, AC_MSKB2) //22
    #define TT1_SPDIF_BYP_OVERUN Fld(1, 21, AC_MSKB2) //21
    #define TT1_SPDIF_B_8CH Fld(1, 20, AC_MSKB2) //20
    #define TT1_SPDIF_NO_FLAT Fld(1, 19, AC_MSKB2) //19
    #define TT1_SPDIF_VUCP_8CH Fld(1, 18, AC_MSKB2) //18
    #define TT1_SPDIF_REQ_8CH Fld(1, 17, AC_MSKB2) //17
    #define TT1_SPDIF_8CH Fld(1, 16, AC_MSKB2) //16
    #define TT0_CD_NO_CLR Fld(1, 15, AC_MSKB1) //15
    #define TT0_PHASE_SW_SEL Fld(1, 14, AC_MSKB1) //14
    #define TT0_PP_SW_SEL Fld(1, 13, AC_MSKB1) //13
    #define TT0_CD_SW_SEL Fld(1, 12, AC_MSKB1) //12
    #define TT0_HBR_8CH Fld(1, 11, AC_MSKB1) //11
    #define TT0_GAMUT_EN Fld(1, 10, AC_MSKB1) //10
    #define TT0_HBR_EN Fld(1, 9, AC_MSKB1) //9
    #define TT0_SACD_EN Fld(1, 8, AC_MSKB1) //8
#define APLL0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x088))
  #define APLL0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x088))
  #define APLL0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x089))
  #define APLL0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x08a))
  #define APLL0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x08b))
    #define USOVRVAL Fld(3, 24, AC_MSKB3) //26:24
    #define SDINOVRVAL Fld(8, 16, AC_FULLB2) //23:16
    #define WRAP Fld(1, 15, AC_MSKB1) //15
    #define SDREF Fld(3, 12, AC_MSKB1) //14:12
    #define CLIP Fld(4, 8, AC_MSKB1) //11:8
    #define MODOVRVAL Fld(2, 6, AC_MSKB0) //7:6
    #define ZERO Fld(3, 3, AC_MSKB0) //5:3
    #define POLE Fld(3, 0, AC_MSKB0) //2:0
#define APLL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x08c))
  #define APLL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x08c))
  #define APLL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x08d))
  #define APLL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x08e))
  #define APLL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x08f))
    #define RG_HAPLL_REV Fld(8, 24, AC_FULLB3) //31:24
    #define ACR_DPLL_LOCK Fld(1, 8, AC_MSKB1) //8
    #define PRESDIVSELOVR Fld(1, 7, AC_MSKB0) //7
    #define MODOVR Fld(1, 6, AC_MSKB0) //6
    #define SDINOVR Fld(1, 5, AC_MSKB0) //5
    #define FS32OVR Fld(1, 4, AC_MSKB0) //4
    #define USOVR Fld(1, 3, AC_MSKB0) //3
    #define PREDIVSELOVRSEL Fld(1, 2, AC_MSKB0) //2
    #define FS32OVRVAL Fld(1, 1, AC_MSKB0) //1
    #define NCLKSEL Fld(1, 0, AC_MSKB0) //0
#define TMDS_CTRL0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x090))
  #define TMDS_CTRL0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x090))
  #define TMDS_CTRL0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x091))
  #define TMDS_CTRL0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x092))
  #define TMDS_CTRL0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x093))
    #define C_DATA_SYNC_AUTO Fld(1,31,AC_MSKB3)//31
    #define C_PRBS_EN Fld(1,30,AC_MSKB3)//30
    #define C_PHY_PCLK_INV Fld(1,27,AC_MSKB3)//27
    #define C_PHY_CH2_CLK_INV Fld(1,26,AC_MSKB3)//26
    #define C_PHY_CH1_CLK_INV Fld(1,25,AC_MSKB3)//25
    #define C_PHY_CH0_CLK_INV Fld(1,24,AC_MSKB3)//24
    #define C_PRBS_AMOUNT Fld(16,8,AC_FULLW21)//[23:8]
    #define DPCLK_INV Fld(1,4,AC_MSKB0)//4
    #define CDR_RT_RST_ Fld(1,2,AC_MSKB0)//2
    #define TMDS_PHY_RST_ Fld(1,1,AC_MSKB0)  //1
    #define PCLK_INV Fld(1,0,AC_MSKB0)//0

#define TMDS_CTRL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x094))
  #define TMDS_CTRL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x094))
  #define TMDS_CTRL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x095))
  #define TMDS_CTRL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x096))
  #define TMDS_CTRL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x097))
    #define PHY_C_CRC_CLR Fld(1,14,AC_MSKB1)//14
    #define PHY_C_CRC_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define PHY_C_CRC_START Fld(1,11,AC_MSKB1)//11
    #define PHY_C_D_SPT_EDGE Fld(1,10,AC_MSKB1)//10
    #define PHY_C_D_SPT_DIV Fld(2,8,AC_MSKB1)//[9:8]
    #define PHY_C_PRBS_THCNT Fld(8,0,AC_FULLB0)//[7:0]

   //
   //CKDT 0x09C
   
#define AEC_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b4))
  #define AEC_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b4))
  #define AEC_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b5))
  #define AEC_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b6))
  #define AEC_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b7))
	#define EXP_EN_15_8 Fld(8, 24, AC_FULLB3) //31:24
	#define EXP_EN_7_0 Fld(8, 16, AC_FULLB2) //23:16
    #define AAC_OUT_OFF_EN Fld(1, 13, AC_MSKB1) //13
    #define AUD_DIV_MIN Fld(1, 12, AC_MSKB1) //12
    #define AAC_MUTE_STAT Fld(1, 11, AC_MSKB1) //11
    #define AVC_EN Fld(1, 10, AC_MSKB1) //10
    #define AAC_ALL Fld(1, 9, AC_MSKB1) //9
    #define AAC_EN Fld(1, 8, AC_MSKB1) //8
		#define AAC_SM_SEL_1T Fld(1, 0, AC_MSKB0)//0
#define AEC_EN (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b8))
  #define AEC_EN_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b8))
  #define AEC_EN_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0b9))
  #define AEC_EN_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ba))
  #define AEC_EN_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0bb))
    #define CAPTURE_CNT_WP Fld(1, 24, AC_MSKB3) //24
    #define FORMAT_CHANGE_EXP_EN Fld(1, 8, AC_MSKB1) //8
    #define EXP_EN Fld(8, 0, AC_FULLB0) //7:0
#define ECC_THRES0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0bc))
  #define ECC_THRES0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0bc))
  #define ECC_THRES0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0bd))
  #define ECC_THRES0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0be))
  #define ECC_THRES0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0bf))
    #define T4_UNCORR_THRESH Fld(7, 16, AC_MSKB2) //22:16
    #define T4_CORR_THRESH Fld(7, 8, AC_MSKB1) //14:8
    #define BCH_THRESH Fld(5, 0, AC_MSKB0) //4:0
#define ECC_THRES1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c0))
  #define ECC_THRES1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c0))
  #define ECC_THRES1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c1))
  #define ECC_THRES1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c2))
  #define ECC_THRES1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c3))
    #define T4_PKT_THRESH Fld(16, 16, AC_FULLW32) //31:16
    #define HDMI_PKT_THRESH Fld(16, 0, AC_FULLW10) //15:0
#define ECC_THRES2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c4))
  #define ECC_THRES2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c4))
  #define ECC_THRES2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c5))
  #define ECC_THRES2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c6))
  #define ECC_THRES2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c7))
    #define HDCP_PKT_THRESH Fld(16, 16, AC_FULLW32) //31:16
    #define BCH_PKT_THRESH Fld(16, 0, AC_FULLW10) //15:0
#define ECC_PKT_CNT0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c8))
  #define ECC_PKT_CNT0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c8))
  #define ECC_PKT_CNT0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0c9))
  #define ECC_PKT_CNT0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ca))
  #define ECC_PKT_CNT0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0cb))
    #define RHDMI_T4_ERR_CNT Fld(16, 16, AC_FULLW32) //31:16
    #define RHDMI_PKT_CNT Fld(16, 0, AC_FULLW10) //15:0
#define ECC_PKT_CNT1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0cc))
  #define ECC_PKT_CNT1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0cc))
  #define ECC_PKT_CNT1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0cd))
  #define ECC_PKT_CNT1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ce))
  #define ECC_PKT_CNT1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0cf))
    #define RHDMI_HDCP_ERR_CNT Fld(16, 16, AC_FULLW32) //31:16
    #define RHDMI_BCH_ERR_CNT Fld(16, 0, AC_FULLW10) //15:0
#define HDCP_DS0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d4))
  #define HDCP_DS0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d4))
  #define HDCP_DS0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d5))
  #define HDCP_DS0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d6))
  #define HDCP_DS0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d7))
	#define DS_MO_7_0 Fld(8, 24, AC_FULLB3) //31:24
    #define DS_BSTATUS Fld(3, 21, AC_MSKB2) //23:21
    #define DS_HDMI_MODE Fld(1, 20, AC_MSKB2) //20
    #define DS_CASC_EXCEED Fld(1, 19, AC_MSKB2) //19
    #define DS_DEPTH Fld(3, 16, AC_MSKB2) //18:16
    #define DS_DEV_EXCEED Fld(1, 15, AC_MSKB1) //15
    #define DS_DEV_CNT Fld(7, 8, AC_MSKB1) //14:8
#define HDCP_DS1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d8))
  #define HDCP_DS1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d8))
  #define HDCP_DS1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0d9))
  #define HDCP_DS1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0da))
  #define HDCP_DS1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0db))
	#define DS_MO_39_32 Fld(8, 24, AC_FULLB3) //31:24
	#define DS_MO_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define DS_MO_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define DS_MO_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0dc))
  #define HDCP_DS2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0dc))
  #define HDCP_DS2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0dd))
  #define HDCP_DS2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0de))
  #define HDCP_DS2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0df))
    #define RPT_V_H0 Fld(8, 24, AC_FULLB3) //31:24
	#define DS_MO63_56 Fld(8, 16, AC_FULLB2) //23:16
	#define DS_MO55_48 Fld(8, 8, AC_FULLB1) //15:8
	#define DS_MO47_40 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e0))
  #define HDCP_DS3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e0))
  #define HDCP_DS3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e1))
  #define HDCP_DS3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e2))
  #define HDCP_DS3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e3))
    #define RPT_V_H1 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H0_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H0_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H0_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e4))
  #define HDCP_DS4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e4))
  #define HDCP_DS4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e5))
  #define HDCP_DS4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e6))
  #define HDCP_DS4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e7))
    #define RPT_V_H2 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H1_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H1_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H1_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e8))
  #define HDCP_DS5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e8))
  #define HDCP_DS5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0e9))
  #define HDCP_DS5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ea))
  #define HDCP_DS5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0eb))
    #define RPT_V_H3 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H2_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H2_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H2_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ec))
  #define HDCP_DS6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ec))
  #define HDCP_DS6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ed))
  #define HDCP_DS6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ee))
  #define HDCP_DS6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ef))
    #define RPT_V_H4 Fld(8, 24, AC_FULLB3) //31:24
	#define RPT_V_H3_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H3_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H3_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define HDCP_DS7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f0))
  #define HDCP_DS7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f0))
  #define HDCP_DS7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f1))
  #define HDCP_DS7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f2))
  #define HDCP_DS7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f3))
    #define OUTBOX_FULL_WP Fld(1, 31, AC_MSKB3) //31
    #define DDC_OUTBOX_FULL Fld(1, 25, AC_MSKB3) //25
    #define INBOX_FULL Fld(1, 24, AC_MSKB3) //24
	#define RPT_V_H4_31_24 Fld(8, 16, AC_FULLB2) //23:16
	#define RPT_V_H4_23_16 Fld(8, 8, AC_FULLB1) //15:8
	#define RPT_V_H4_15_8 Fld(8, 0, AC_FULLB0) //7:0
#define DDC_BOX (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f4))
  #define DDC_BOX_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f4))
  #define DDC_BOX_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f5))
  #define DDC_BOX_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f6))
  #define DDC_BOX_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f7))
    #define EPA Fld(8, 24, AC_FULLB3) //31:24
    #define EPDT Fld(8, 16, AC_FULLB2) //23:16
    #define DDC_OUTBOX Fld(8, 8, AC_FULLB1) //15:8
    #define DDC_INBOX Fld(8, 0, AC_FULLB0) //7:0
#define EPST (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f8))
  #define EPST_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f8))
  #define EPST_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0f9))
  #define EPST_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0fa))
  #define EPST_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0fb))
    #define EPST_KS_MASK Fld(8, 24, AC_FULLB3) //31:24
    #define EPCM_WP Fld(1, 23, AC_MSKB2) //23
    #define MSIF_UNLOCK Fld(1, 22, AC_MSKB2) //22
    #define LD_KSV Fld(1, 21, AC_MSKB2) //21
    #define EPCM Fld(5, 16, AC_MSKB2) //20:16
    #define OTP_UNLOCKED Fld(1, 15, AC_MSKB1) //15
    #define OTP_BIST2_ERR Fld(1, 14, AC_MSKB1) //14
    #define OTP_BIST1_ERR Fld(1, 13, AC_MSKB1) //13
    #define OPT_DN_KSV Fld(1, 12, AC_MSKB1) //12
    #define MSIF_ACK_ERR Fld(1, 11, AC_MSKB1) //11
    #define MSIF_SDA_ERR Fld(1, 10, AC_MSKB1) //10
    #define OTP_BIST0_ERR Fld(1, 9, AC_MSKB1) //9
    #define MSIF_CMD_DONE Fld(1, 8, AC_MSKB1) //8
    #define EPA_PAGE Fld(1, 0, AC_MSKB0) //0
#define KS_MASK (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0fc))
  #define KS_MASK_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0fc))
  #define KS_MASK_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0fd))
  #define KS_MASK_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0fe))
  #define KS_MASK_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x0ff))
	#define KS_MASK_b0 Fld(2, 6, AC_MSKB0) //7:6
	#define KS_MASK_b1 Fld(4, 2, AC_MSKB0) //5:2
	#define KS_MASK_b2 Fld(2, 0, AC_MSKB0) //1:0
#define ACR_CTRL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x100))
  #define ACR_CTRL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x100))
  #define ACR_CTRL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x101))
  #define ACR_CTRL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x102))
  #define ACR_CTRL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x103))
    #define N_VAL_SW Fld(8, 24, AC_FULLB3) //31:24
    #define FM_IN_VAL_SW Fld(2, 22, AC_MSKB2) //23:22
    #define FM_VAL_SW Fld(2, 20, AC_MSKB2) //21:20
    #define FS_VAL_SW Fld(4, 16, AC_MSKB2) //19:16
    #define CTS_DROPPED_AUTO_EN Fld(1, 7, AC_MSKB0) //7
    #define POST_HW_SW_SEL Fld(1, 6, AC_MSKB0) //6
    #define UPLL_HW_SW_SEL Fld(1, 5, AC_MSKB0) //5
    #define CTS_HW_SW_SEL Fld(1, 4, AC_MSKB0) //4
    #define N_HW_SW_SEL Fld(1, 3, AC_MSKB0) //3
    #define CTS_REUSED_AUTO_EN Fld(1, 2, AC_MSKB0) //2
    #define FS_HW_SW_SEL Fld(1, 1, AC_MSKB0) //1
    #define ACR_INIT_WP Fld(1, 0, AC_MSKB0) //0
#define N_SVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x104))
  #define N_SVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x104))
  #define N_SVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x105))
  #define N_SVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x106))
  #define N_SVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x107))
    #define N_VAL_HW15_8 Fld(8, 24, AC_FULLB3) //31:24
    #define N_VAL_HW7_0 Fld(8, 16, AC_FULLB2) //23:16
	#define N_VAL_SW19_16 Fld(4, 8, AC_MSKB1) //11:8
	#define N_VAL_SW15_8 Fld(8, 0, AC_FULLB0) //7:0
#define N_HVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x108))
  #define N_HVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x108))
  #define N_HVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x109))
  #define N_HVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x10a))
  #define N_HVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x10b))
	#define CTS_VAL_SW19_16 Fld(4, 24, AC_MSKB3) //27:24
	#define CTS_VAL_SW15_8 Fld(8, 16, AC_FULLB2) //23:16
	#define CTS_VAL_SW7_0 Fld(8, 8, AC_FULLB1) //15:8
    #define N_VAL_HW Fld(4, 0, AC_MSKB0) //3:0
#define CTS_HVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x10c))
  #define CTS_HVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x10c))
  #define CTS_HVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x10d))
  #define CTS_HVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x10e))
  #define CTS_HVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x10f))
    #define UPLL_VAL_SW Fld(7, 24, AC_MSKB3) //30:24
    #define CTS_VAL_HW19_16 Fld(4, 16, AC_MSKB2) //19:16
    #define CTS_VAL_HW15_8 Fld(8, 8, AC_FULLB1) //15:8
    #define CTS_VAL_HW7_0 Fld(8, 0, AC_FULLB0) //7:0
#define UPLL_HVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x110))
  #define UPLL_HVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x110))
  #define UPLL_HVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x111))
  #define UPLL_HVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x112))
  #define UPLL_HVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x113))
    #define LK_WIN_SW Fld(8,24,AC_FULLB3)//[31:24]
    #define POST_VAL_HW Fld(6, 16, AC_MSKB2) //21:16
    #define POST_VAL_SW Fld(6, 8, AC_MSKB1) //13:8
    #define UPLL_VAL_HW Fld(7, 0, AC_MSKB0) //6:0
#define LK_THRS_SVAL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x114))
  #define LK_THRS_SVAL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x114))
  #define LK_THRS_SVAL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x115))
  #define LK_THRS_SVAL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x116))
  #define LK_THRS_SVAL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x117))
    #define FS_FILTER_EN Fld(1, 28, AC_MSKB3) //28
    #define RHDMI_AUD_SAMPLE_F Fld(4, 24, AC_MSKB3) //27:24
		#define AUD_FS_44K           (0x0)
		#define AUD_FS_88K           (0x8)
		#define AUD_FS_176K          (0xc)
		#define AUD_FS_48K           (0x2)
		#define AUD_FS_96K           (0xa)
		#define AUD_FS_192K          (0xe)
		#define AUD_FS_176K          (0xc)
		#define AUD_FS_32K           (0x3)
		#define AUD_FS_UNKNOWN       (0x1) 	
    #define LS_THRS_SVAL Fld(4, 16, AC_MSKB2) //19:16
	#define LK_THRS_SVAL_b0 Fld(8, 8, AC_FULLB1) //15:8
	#define LK_THRS_SVAL_b1 Fld(8, 0, AC_FULLB0) //7:0
#define ACR_CTRL3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x118))
  #define ACR_CTRL3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x118))
  #define ACR_CTRL3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x119))
  #define ACR_CTRL3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x11a))
  #define ACR_CTRL3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x11b))
    #define FBDIV Fld(1, 24, AC_MSKB3) //24
    #define REG_HADDS2_PCW_CHG_SW Fld(1, 9, AC_MSKB1) //9
    #define REG_HADDS2_PCW_SEL_SW Fld(1, 8, AC_MSKB1) //8
    #define CTS_THRESH Fld(4, 3, AC_MSKB0) //6:3
    #define MCLK_LOOPBACK Fld(1, 2, AC_MSKB0) //2
    #define LOG_WIN_ENA Fld(1, 1, AC_MSKB0) //1
    #define POST_DIV2_ENA Fld(1, 0, AC_MSKB0) //0
    #define REG_HADDS2_PCW_CHG_SW Fld(1, 9, AC_MSKB1) //9
#define DDS (IO_VIRT_HDMI+(IO_HDMI_BASE+0x11c))
  #define DDS_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x11c))
  #define DDS_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x11d))
  #define DDS_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x11e))
  #define DDS_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x11f))
    #define REG_HADDS2_PCW_SW Fld(32,0,AC_FULLDW)//[31:0]
    
#define I2S_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x124))
  #define I2S_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x124))
  #define I2S_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x125))
  #define I2S_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x126))
  #define I2S_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x127))
    #define SD3_EN Fld(1, 31, AC_MSKB3) //31
    #define SD2_EN Fld(1, 30, AC_MSKB3) //30
    #define SD1_EN Fld(1, 29, AC_MSKB3) //29
    #define SD0_EN Fld(1, 28, AC_MSKB3) //28
    #define MCLK_EN Fld(1, 27, AC_MSKB3) //27
    #define VUCP Fld(1, 25, AC_MSKB3) //25
    #define PCM Fld(1, 24, AC_MSKB3) //24
    #define INVALID_EN Fld(1, 23, AC_MSKB2) //23
    #define CLK_EDGE Fld(1, 22, AC_MSKB2) //22
    #define SIZE Fld(1, 21, AC_MSKB2) //21
    #define MSB Fld(1, 20, AC_MSKB2) //20
    #define WS Fld(1, 19, AC_MSKB2) //19
    #define JUSTIFY Fld(1, 18, AC_MSKB2) //18
    #define DATA_DIR Fld(1, 17, AC_MSKB2) //17
	#define HDMI_1ST_BIT Fld(1, 16, AC_MSKB2) //16
		#define AAC_ST Fld(3, 0, AC_MSKB0)//2:0
#define AUDRX_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x128))
  #define AUDRX_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x128))
  #define AUDRX_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x129))
  #define AUDRX_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x12a))
  #define AUDRX_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x12b))
    #define AUD_CH_STAT2 Fld(8, 24, AC_FULLB3) //31:24
	#define AUD_CH_STAT1_7_6 Fld(2, 22, AC_MSKB2) //23:22
	#define AUD_CH_STAT1_5_3 Fld(3, 19, AC_MSKB2) //21:19
    #define AUD_CH_STAT1_2 Fld(1, 18, AC_MSKB2) //18
    #define AUD_CH_STAT1_1 Fld(1, 17, AC_MSKB2) //17
    #define AUD_CH_STAT1_0 Fld(1, 16, AC_MSKB2) //16
    #define I2S_LENGTH_EN Fld(1, 15, AC_MSKB1) //15
    #define SWR_EN Fld(1, 14, AC_MSKB1) //14
    #define HW_MUTE_EN Fld(1, 13, AC_MSKB1) //13
    #define PASS_SPDIF_ERR Fld(1, 12, AC_MSKB1) //12
    #define PASS_AUD_ERR Fld(1, 11, AC_MSKB1) //11
    #define I2S_MODE Fld(1, 10, AC_MSKB1) //10
    #define SPDIF_MODE Fld(1, 9, AC_MSKB1) //9
    #define SPDIF_EN Fld(1, 8, AC_MSKB1) //8
    #define SD3_MAP Fld(2, 6, AC_MSKB0) //7:6
    #define SD2_MAP Fld(2, 4, AC_MSKB0) //5:4
    #define SD1_MAP Fld(2, 2, AC_MSKB0) //3:2
    #define SD0_MAP Fld(2, 0, AC_MSKB0) //1:0
#define CHST0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x12c))
  #define CHST0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x12c))
  #define CHST0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x12d))
  #define CHST0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x12e))
  #define CHST0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x12f))
    #define CS_BIT15_8 Fld(8, 24, AC_FULLB3) //31:24
	#define CHST_SWAP Fld(4, 20, AC_MSKB2) //23:20
    #define CS_BIT2 Fld(1, 18, AC_MSKB2) //18
    #define OW_EN Fld(1, 16, AC_MSKB2) //16
    #define DIV_INCR Fld(8, 8, AC_FULLB1) //15:8
    #define CH_NUM1 Fld(4, 4, AC_MSKB0) //7:4
    #define SOURCE1 Fld(4, 0, AC_MSKB0) //3:0
#define CHST1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x130))
  #define CHST1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x130))
  #define CHST1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x131))
  #define CHST1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x132))
  #define CHST1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x133))
    #define HDMI_MODE_SW_VALUE Fld(1, 25, AC_MSKB3) //25
    #define HDMI_MODE_OVERWRITE Fld(1, 24, AC_MSKB3) //24
    #define I2S_LENGTH Fld(4, 20, AC_MSKB2) //23:20
    #define CH3_MUTE Fld(1, 19, AC_MSKB2) //19
    #define CH2_MUTE Fld(1, 18, AC_MSKB2) //18
    #define CH1_MUTE Fld(1, 17, AC_MSKB2) //17
    #define CH0_MUTE Fld(1, 16, AC_MSKB2) //16
    #define AUD_ORG_FS Fld(4, 12, AC_MSKB1) //15:12
    #define AUD_LENGTH Fld(3, 9, AC_MSKB1) //11:9
    #define AUD_LENGTH_MAX Fld(1, 8, AC_MSKB1) //8
    #define AUD_ACCURACY Fld(4, 4, AC_MSKB0) //7:4
    #define AUD_SAMPLE_F Fld(4, 0, AC_MSKB0) //3:0
#define AUDP_STAT (IO_VIRT_HDMI+(IO_HDMI_BASE+0x134))
  #define AUDP_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x134))
  #define AUDP_STAT_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x135))
  #define AUDP_STAT_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x136))
  #define AUDP_STAT_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x137))
    #define MUTE_OUT_POL Fld(1, 26, AC_MSKB3) //26
    #define AUDIO_MUTE Fld(1, 25, AC_MSKB3) //25
    #define AUDP_VIDEO_MUTE Fld(1, 24, AC_MSKB3) //24
    #define STA_HDCP_CTL3_STATE Fld(4, 20, AC_MSKB2) //23:20
    #define BYP_DE_FLT Fld(1, 19, AC_MSKB2) //19
    #define BYP_DVIFILT Fld(1, 18, AC_MSKB2) //18
    #define BYP_SYNC Fld(1, 17, AC_MSKB2) //17
    #define BYP_DALIGN Fld(1, 16, AC_MSKB2) //16
    #define PREAMBLE_CRI Fld(5, 8, AC_MSKB1) //12:8
    #define STA_HDCP_CTL3_DET Fld(1, 7, AC_MSKB0) //7
    #define STA_HDCP_EN_DET Fld(1, 6, AC_MSKB0) //6
    #define STA_DI_PRMBL_DET Fld(1, 5, AC_MSKB0) //5
    #define STA_VID_PRMBL_DET Fld(1, 4, AC_MSKB0) //4
    #define HDMI_LAYOUT Fld(1, 3, AC_MSKB0) //3
    #define HDMI_MUTE Fld(1, 2, AC_MSKB0) //2
    #define HDMI_MODE_EN Fld(1, 1, AC_MSKB0) //1
    #define HDMI_MODE_DET Fld(1, 0, AC_MSKB0) //0
#define TEST_CTRL (IO_VIRT_HDMI+(IO_HDMI_BASE+0x138))
  #define TEST_CTRL_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x138))
  #define TEST_CTRL_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x139))
  #define TEST_CTRL_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x13a))
  #define TEST_CTRL_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x13b))
    #define TST_XCLK Fld(1, 31, AC_MSKB3) //31
    #define TST_CKDT Fld(1, 30, AC_MSKB3) //30
    #define TST_PLLREF Fld(1, 23, AC_MSKB2) //23
    #define TST_PLLCK Fld(1, 22, AC_MSKB2) //22
    #define TST_OSCK Fld(1, 21, AC_MSKB2) //21
    #define TST_APLLCK Fld(1, 20, AC_MSKB2) //20
    #define TST_SIF Fld(1, 19, AC_MSKB2) //19
    #define RBIST_CLK Fld(1, 18, AC_MSKB2) //18
    #define CORE_ISO_EN Fld(1, 16, AC_MSKB2) //16
    #define HDMI_FIFO_DIFF Fld(7, 8, AC_MSKB1) //14:8
    #define HDCP_CRI Fld(5, 0, AC_MSKB0) //4:0
#define PD_SYS (IO_VIRT_HDMI+(IO_HDMI_BASE+0x13c))
  #define PD_SYS_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x13c))
  #define PD_SYS_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x13d))
  #define PD_SYS_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x13e))
  #define PD_SYS_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x13f))
    #define PD_AO Fld(1, 31, AC_MSKB3) //31
    #define PD_VO Fld(1, 30, AC_MSKB3) //30
    #define PD_APLL Fld(1, 29, AC_MSKB3) //29
    #define PD_12CHAN Fld(1, 27, AC_MSKB3) //27
    #define PD_FULL Fld(1, 26, AC_MSKB3) //26
    #define PD_OSC Fld(1, 25, AC_MSKB3) //25
    #define PD_XTAL_APLL Fld(1, 24, AC_MSKB3) //24
    #define PD_PCLK Fld(1, 23, AC_MSKB2) //23
    #define PD_MCLK Fld(1, 22, AC_MSKB2) //22
    #define PD_TERM Fld(1, 20, AC_MSKB2) //20
    #define PD_QO Fld(1, 19, AC_MSKB2) //19
    #define PD_QE Fld(1, 18, AC_MSKB2) //18
    #define PD_VHDE Fld(1, 17, AC_MSKB2) //17
    #define PD_ODCK Fld(1, 16, AC_MSKB2) //16
    #define PD_TOTAL Fld(1, 0, AC_MSKB0) //0
#define AVIRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x140))
  #define AVIRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x140))
  #define AVIRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x141))
  #define AVIRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x142))
  #define AVIRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x143))
    #define CEA_AVI_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_AVI_HEADER_15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_AVI_HEADER_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x144))
  #define AVIRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x144))
  #define AVIRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x145))
  #define AVIRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x146))
  #define AVIRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x147))
    #define CEA_AVI_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x148))
  #define AVIRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x148))
  #define AVIRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x149))
  #define AVIRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x14a))
  #define AVIRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x14b))
    #define CEA_AVI_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x14c))
  #define AVIRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x14c))
  #define AVIRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x14d))
  #define AVIRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x14e))
  #define AVIRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x14f))
    #define CEA_AVI_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AVI_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define AVIRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x150))
  #define AVIRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x150))
  #define AVIRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x151))
  #define AVIRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x152))
  #define AVIRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x153))
    #define CEA_AVI_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AVI_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AVI_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define HDMI_SPH (IO_VIRT_HDMI+(IO_HDMI_BASE+0x158))
  #define HDMI_SPH_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x158))
  #define HDMI_SPH_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x159))
  #define HDMI_SPH_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x15a))
  #define HDMI_SPH_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x15b))
    #define HDMI_PHY_CRC_RDY Fld(1,25,AC_MSKB3)//25
    #define HDMI_PHY_CRC_FAIL Fld(1,24,AC_MSKB3)//24
    #define HDMI_PHY_CRC_OUT Fld(24,0,AC_MSKDW)//[23:0]
#define ANA_STAT_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x15c))
  #define ANA_STAT_0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x15c))
  #define ANA_STAT_0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x15d))
  #define ANA_STAT_0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x15e))
  #define ANA_STAT_0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x15f))
    #define AD_HDMI_ADEQ_CH2 Fld(4, 24, AC_MSKB3) //27:24
    #define AD_HDMI_ADEQ_CH1 Fld(4, 20, AC_MSKB2) //23:20
    #define AD_HDMI_ADEQ_CH0 Fld(4, 16, AC_MSKB2) //19:16
#define SPDRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x160))
  #define SPDRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x160))
  #define SPDRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x161))
  #define SPDRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x162))
  #define SPDRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x163))
    #define CEA_SPD_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_SPD_HEADER15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_SPD_HEADER7_0 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x164))
  #define SPDRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x164))
  #define SPDRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x165))
  #define SPDRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x166))
  #define SPDRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x167))
    #define CEA_SPD_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x168))
  #define SPDRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x168))
  #define SPDRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x169))
  #define SPDRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x16a))
  #define SPDRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x16b))
    #define CEA_SPD_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x16c))
  #define SPDRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x16c))
  #define SPDRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x16d))
  #define SPDRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x16e))
  #define SPDRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x16f))
    #define CEA_SPD_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x170))
  #define SPDRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x170))
  #define SPDRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x171))
  #define SPDRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x172))
  #define SPDRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x173))
    #define CEA_SPD_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x174))
  #define SPDRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x174))
  #define SPDRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x175))
  #define SPDRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x176))
  #define SPDRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x177))
    #define CEA_SPD_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x178))
  #define SPDRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x178))
  #define SPDRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x179))
  #define SPDRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x17a))
  #define SPDRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x17b))
    #define CEA_SPD_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define SPDRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x17c))
  #define SPDRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x17c))
  #define SPDRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x17d))
  #define SPDRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x17e))
  #define SPDRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x17f))
    #define SPD_DEC Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_SPD_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_SPD_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_SPD_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x180))
  #define AUDRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x180))
  #define AUDRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x181))
  #define AUDRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x182))
  #define AUDRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x183))
    #define CEA_AUD_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AUD_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_AUD_HEADER_15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_AUD_HEADER_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x184))
  #define AUDRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x184))
  #define AUDRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x185))
  #define AUDRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x186))
  #define AUDRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x187))
    #define CEA_AUD_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AUD_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AUD_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AUD_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x188))
  #define AUDRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x188))
  #define AUDRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x189))
  #define AUDRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x18a))
  #define AUDRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x18b))
    #define CEA_AUD_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_AUD_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_AUD_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AUD_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define AUDRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x18c))
  #define AUDRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x18c))
  #define AUDRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x18d))
  #define AUDRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x18e))
  #define AUDRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x18f))
    #define CEA_AUD_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_AUD_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a0))
  #define MPEGRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a0))
  #define MPEGRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a1))
  #define MPEGRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a2))
  #define MPEGRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a3))
    #define CEA_MPEG_CHECKSUM Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_LENGTH Fld(8, 16, AC_FULLB2) //23:16
	#define CEA_MPEG_HEADER_15_8 Fld(8, 8, AC_FULLB1) //15:8
	#define CEA_MPEG_HEADER_7_0 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a4))
  #define MPEGRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a4))
  #define MPEGRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a5))
  #define MPEGRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a6))
  #define MPEGRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a7))
    #define CEA_MPEG_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a8))
  #define MPEGRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a8))
  #define MPEGRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1a9))
  #define MPEGRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1aa))
  #define MPEGRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ab))
    #define CEA_MPEG_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ac))
  #define MPEGRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ac))
  #define MPEGRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ad))
  #define MPEGRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ae))
  #define MPEGRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1af))
    #define CEA_MPEG_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b0))
  #define MPEGRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b0))
  #define MPEGRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b1))
  #define MPEGRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b2))
  #define MPEGRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b3))
    #define CEA_MPEG_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b4))
  #define MPEGRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b4))
  #define MPEGRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b5))
  #define MPEGRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b6))
  #define MPEGRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b7))
    #define CEA_MPEG_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b8))
  #define MPEGRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b8))
  #define MPEGRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1b9))
  #define MPEGRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ba))
  #define MPEGRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1bb))
    #define CEA_MPEG_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define MPEGRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1bc))
  #define MPEGRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1bc))
  #define MPEGRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1bd))
  #define MPEGRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1be))
  #define MPEGRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1bf))
    #define MPEG_DEC Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_MPEG_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_MPEG_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_MPEG_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c0))
  #define UNRECRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c0))
  #define UNRECRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c1))
  #define UNRECRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c2))
  #define UNRECRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c3))
		#define CEA_UNREC_DBYTE4 Fld(8, 24, AC_FULLB3)//31:24
		#define CEA_UNREC_DBYTE3 Fld(8, 16, AC_FULLB2)//23:16
		#define CEA_UNREC_DBYTE2 Fld(8, 8, AC_FULLB1)//15:8
		#define CEA_UNREC_DBYTE1 Fld(8, 0, AC_FULLB0)//7:0
#define UNRECRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c4))
  #define UNRECRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c4))
  #define UNRECRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c5))
  #define UNRECRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c6))
  #define UNRECRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c7))
    #define CEA_UNREC_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c8))
  #define UNRECRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c8))
  #define UNRECRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1c9))
  #define UNRECRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ca))
  #define UNRECRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1cb))
    #define CEA_UNREC_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1cc))
  #define UNRECRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1cc))
  #define UNRECRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1cd))
  #define UNRECRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ce))
  #define UNRECRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1cf))
    #define CEA_UNREC_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d0))
  #define UNRECRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d0))
  #define UNRECRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d1))
  #define UNRECRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d2))
  #define UNRECRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d3))
    #define CEA_UNREC_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d4))
  #define UNRECRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d4))
  #define UNRECRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d5))
  #define UNRECRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d6))
  #define UNRECRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d7))
    #define CEA_UNREC_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d8))
  #define UNRECRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d8))
  #define UNRECRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1d9))
  #define UNRECRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1da))
  #define UNRECRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1db))
    #define CEA_UNREC_DBYTE28 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_UNREC_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define UNRECRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1dc))
  #define UNRECRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1dc))
  #define UNRECRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1dd))
  #define UNRECRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1de))
  #define UNRECRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1df))
    #define CEA_CP_CLR_MUTE Fld(1, 28, AC_MSKB3) //28
    #define CEA_CP_SET_MUTE Fld(1, 24, AC_MSKB3) //24
    #define CEA_UNREC_DBYTE31 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_UNREC_DBYTE30 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_UNREC_DBYTE29 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e0))
  #define ACPRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e0))
  #define ACPRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e1))
  #define ACPRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e2))
  #define ACPRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e3))
		#define CEA_ACP_DBYTE4 Fld(8, 24, AC_FULLB3)//31:24
		#define CEA_ACP_DBYTE3 Fld(8, 16, AC_FULLB2)//23:16
		#define CEA_ACP_DBYTE2 Fld(8, 8, AC_FULLB1)//15:8
		#define CEA_ACP_DBYTE1 Fld(8, 0, AC_FULLB0)//7:0
#define ACPRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e4))
  #define ACPRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e4))
  #define ACPRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e5))
  #define ACPRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e6))
  #define ACPRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e7))
    #define CEA_ACP_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e8))
  #define ACPRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e8))
  #define ACPRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1e9))
  #define ACPRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ea))
  #define ACPRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1eb))
    #define CEA_ACP_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ec))
  #define ACPRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ec))
  #define ACPRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ed))
  #define ACPRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ee))
  #define ACPRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ef))
    #define CEA_ACP_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f0))
  #define ACPRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f0))
  #define ACPRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f1))
  #define ACPRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f2))
  #define ACPRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f3))
    #define CEA_ACP_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f4))
  #define ACPRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f4))
  #define ACPRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f5))
  #define ACPRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f6))
  #define ACPRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f7))
    #define CEA_ACP_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f8))
  #define ACPRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f8))
  #define ACPRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1f9))
  #define ACPRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1fa))
  #define ACPRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1fb))
    #define CEA_ACP_DBYTE28 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define ACPRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1fc))
  #define ACPRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1fc))
  #define ACPRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1fd))
  #define ACPRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1fe))
  #define ACPRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x1ff))
    #define ACP_DEC Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_ACP_DBYTE31 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_ACP_DBYTE30 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_ACP_DBYTE29 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x200))
  #define GAMUTRX0_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x200))
  #define GAMUTRX0_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x201))
  #define GAMUTRX0_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x202))
  #define GAMUTRX0_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x203))
    #define CEA_GAMUT_DBYTE4 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE3 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE2 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE1 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x204))
  #define GAMUTRX1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x204))
  #define GAMUTRX1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x205))
  #define GAMUTRX1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x206))
  #define GAMUTRX1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x207))
    #define CEA_GAMUT_DBYTE8 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE7 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE6 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE5 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x208))
  #define GAMUTRX2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x208))
  #define GAMUTRX2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x209))
  #define GAMUTRX2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x20a))
  #define GAMUTRX2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x20b))
    #define CEA_GAMUT_DBYTE12 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE11 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE10 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE9 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x20c))
  #define GAMUTRX3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x20c))
  #define GAMUTRX3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x20d))
  #define GAMUTRX3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x20e))
  #define GAMUTRX3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x20f))
    #define CEA_GAMUT_DBYTE16 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE15 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE14 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE13 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x210))
  #define GAMUTRX4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x210))
  #define GAMUTRX4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x211))
  #define GAMUTRX4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x212))
  #define GAMUTRX4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x213))
    #define CEA_GAMUT_DBYTE20 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE19 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE18 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE17 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x214))
  #define GAMUTRX5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x214))
  #define GAMUTRX5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x215))
  #define GAMUTRX5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x216))
  #define GAMUTRX5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x217))
    #define CEA_GAMUT_DBYTE24 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE23 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE22 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE21 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX6 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x218))
  #define GAMUTRX6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x218))
  #define GAMUTRX6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x219))
  #define GAMUTRX6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x21a))
  #define GAMUTRX6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x21b))
    #define CEA_GAMUT_DBYTE28 Fld(8, 24, AC_FULLB3) //31:24
    #define CEA_GAMUT_DBYTE27 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE26 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE25 Fld(8, 0, AC_FULLB0) //7:0
#define GAMUTRX7 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x21c))
  #define GAMUTRX7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x21c))
  #define GAMUTRX7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x21d))
  #define GAMUTRX7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x21e))
  #define GAMUTRX7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x21f))
    #define CEA_GAMUT_DBYTE31 Fld(8, 16, AC_FULLB2) //23:16
    #define CEA_GAMUT_DBYTE30 Fld(8, 8, AC_FULLB1) //15:8
    #define CEA_GAMUT_DBYTE29 Fld(8, 0, AC_FULLB0) //7:0
#define N_HDMI_CTRL1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x220))
  #define N_HDMI_CTRL1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x220))
  #define N_HDMI_CTRL1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x221))
  #define N_HDMI_CTRL1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x222))
  #define N_HDMI_CTRL1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x223))
    #define reservedregister_N_HDMI_CTRL1_11 Fld(1, 31, AC_MSKB3) //31
    #define reservedregister_N_HDMI_CTRL1_10 Fld(1, 30, AC_MSKB3) //30
    #define reservedregister_N_HDMI_CTRL1_9 Fld(1, 29, AC_MSKB3) //29
    #define reservedregister_N_HDMI_CTRL1_8 Fld(1, 28, AC_MSKB3) //28
    #define TT88_3_SPDIF_CS_SEL Fld(2, 26, AC_MSKB3) //27:26
    #define TT88_3_SPDIF_CH_SEL Fld(2, 24, AC_MSKB3) //25:24
    #define reservedregister_N_HDMI_CTRL1_7 Fld(1, 23, AC_MSKB2) //23
    #define reservedregister_N_HDMI_CTRL1_6 Fld(1, 22, AC_MSKB2) //22
    #define TT88_2_I2S_BOUT Fld(1, 21, AC_MSKB2) //21
    #define TT88_2_I2S_EXT_OUT Fld(1, 20, AC_MSKB2) //20
    #define TT88_2_CD_CLR_VSYNC_CNT Fld(4, 16, AC_MSKB2) //19:16
    #define TT88_1_PP_SW Fld(4, 12, AC_MSKB1) //15:12
    #define TT88_1_CD_SW Fld(4, 8, AC_MSKB1) //11:8
    #define reservedregister_N_HDMI_CTRL1_5 Fld(1, 7, AC_MSKB0) //7
    #define reservedregister_N_HDMI_CTRL1_4 Fld(1, 6, AC_MSKB0) //6
    #define reservedregister_N_HDMI_CTRL1_3 Fld(1, 5, AC_MSKB0) //5
    #define TT88_0_NEW_GAMUT_ONLY Fld(1, 4, AC_MSKB0) //4
    #define TT88_0_PHASE_SW Fld(1, 3, AC_MSKB0) //3
    #define reservedregister_N_HDMI_CTRL1_2 Fld(1, 2, AC_MSKB0) //2
    #define reservedregister_N_HDMI_CTRL1_1 Fld(1, 1, AC_MSKB0) //1
    #define reservedregister_N_HDMI_CTRL1_0 Fld(1, 0, AC_MSKB0) //0
#define N_HDMI_CTRL2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x224))
  #define N_HDMI_CTRL2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x224))
  #define N_HDMI_CTRL2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x225))
  #define N_HDMI_CTRL2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x226))
  #define N_HDMI_CTRL2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x227))
    #define reservedregister_N_HDMI_CTRL2_7 Fld(1, 31, AC_MSKB3) //31
    #define reservedregister_N_HDMI_CTRL2_6 Fld(1, 30, AC_MSKB3) //30
    #define reservedregister_N_HDMI_CTRL2_5 Fld(1, 29, AC_MSKB3) //29
    #define reservedregister_N_HDMI_CTRL2_4 Fld(1, 28, AC_MSKB3) //28
    #define reservedregister_N_HDMI_CTRL2_3 Fld(1, 27, AC_MSKB3) //27
    #define reservedregister_N_HDMI_CTRL2_2 Fld(1, 26, AC_MSKB3) //26
    #define reservedregister_N_HDMI_CTRL2_1 Fld(1, 25, AC_MSKB3) //25
    #define reservedregister_N_HDMI_CTRL2_0  Fld(1, 24, AC_MSKB3) //24
    #define TT89_STAT2_GCP_PP_OK Fld(4, 20, AC_MSKB2) //23:20
    #define TT89_STAT2_GCP_CD_OK Fld(4, 16, AC_MSKB2) //19:16
    #define TT89_STAT1_CD_OLD Fld(4, 12, AC_MSKB1) //15:12
    #define TT89_STAT1_GCP_CD Fld(4, 8, AC_MSKB1) //11:8
    #define TT89_STAT0_GCP_PHASE_OK Fld(1, 5, AC_MSKB0) //5
    #define TT89_STAT0_GCP_PHASE Fld(1, 4, AC_MSKB0) //4
    #define TT89_STAT0_NO_VALID_NEW_PP Fld(1, 3, AC_MSKB0) //3
    #define TT89_STAT0_NO_VALID_NEW_CD Fld(1, 2, AC_MSKB0) //2
    #define TT89_STAT0_GCP_PP_ERR Fld(1, 1, AC_MSKB0) //1
    #define TT89_STAT0_GCP_CD_ERR Fld(1, 0, AC_MSKB0) //0
#define N_HDMI_CTRL3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x228))
  #define N_HDMI_CTRL3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x228))
  #define N_HDMI_CTRL3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x229))
  #define N_HDMI_CTRL3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x22a))
  #define N_HDMI_CTRL3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x22b))
    #define WS_PERIOD Fld(8, 24, AC_FULLB3) //31:24
    #define SCK_PERIOD Fld(8, 16, AC_FULLB2) //23:16
    #define ACP_VS_SET Fld(2, 10, AC_MSKB1) //11:10
    #define SOFT_ACP_CLR Fld(1, 9, AC_MSKB1) //9
    #define ACP_CLR_EN Fld(1, 8, AC_MSKB1) //8
    #define EXT_PKT_DEC Fld(8, 0, AC_FULLB0) //7:0
#define N_HDMI_CTRL4 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x22c))
  #define N_HDMI_CTRL4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x22c))
  #define N_HDMI_CTRL4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x22d))
  #define N_HDMI_CTRL4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x22e))
  #define N_HDMI_CTRL4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x22f))
		#define TMDS_CK_PERIOD Fld(8, 12, AC_MSKW21)//19:12
		#define RADDR Fld(6, 6, AC_MSKW10)//11:6
		#define WADDR Fld(6, 0, AC_MSKB0)//5:0
#define N_CTRL5 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x230))
	#define N_CTRL5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x230))
	#define N_CTRL5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x231))
	#define N_CTRL5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x232))
	#define N_CTRL5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x233))
		#define PRE_UNDERRUN_TH Fld(5, 8, AC_MSKB1)//12:8
		#define PRE_OVERRUN_TH Fld(5, 0, AC_MSKB0)//4:0

#define HAPLL_REPLACE_APLL2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x234))
	#define HAPLL_REPLACE_APLL2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x234))
	#define HAPLL_REPLACE_APLL2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x235))
 
	#define HAPLL_REPLACE_APLL2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x236))
	#define HAPLL_REPLACE_APLL2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x237))
		#define HAPLL_MODULATOR_INPUT Fld(25, 0, AC_MSKDW)//24:0
#define CTS_HW_ADP1 (IO_HDMI_BASE+0x238)
	#define CTS_HW_ADP1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x238))
	#define CTS_HW_ADP1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x239))
	#define CTS_HW_ADP1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x23a))
	#define CTS_HW_ADP1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x23b))
    #define CTS_LOCK_ALG_SEL Fld(3,29,AC_MSKB3)//[31:29]
    #define CTS_HW_ADP_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define PTR_DIFF_LOW_TH Fld(4,24,AC_MSKB3)//[27:24]
    #define PTR_DIFF_HIGH_TH Fld(6,16,AC_MSKB2)//[21:16]
    #define CTS_ROUGH_STEP Fld(8,8,AC_FULLB1)//[15:8]
    #define CTS_FINE_STEP Fld(8,0,AC_FULLB0)//[7:0]
#define CTS_HW_ADP2 (IO_HDMI_BASE+0x23C)
	#define CTS_HW_ADP2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x23c))
	#define CTS_HW_ADP2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x23d))
	#define CTS_HW_ADP2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x23e))
	#define CTS_HW_ADP2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x23f))
    #define CTS_HW_ADP Fld(20,0,AC_MSKDW)//[19:0]
#define CTS_STATUS (IO_HDMI_BASE+0x240)
	#define CTS_STATUS_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x240))
	#define CTS_STATUS_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x241))
	#define CTS_STATUS_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x242))
	#define CTS_STATUS_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x243))
    #define CTS_STATUS_MONITOR_OUT Fld(32,0,AC_FULLDW)//[31:0]
#define CTS_HW_ADP3 (IO_HDMI_BASE+0x244)
	#define CTS_HW_ADP3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x244))
	#define CTS_HW_ADP3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x245))
	#define CTS_HW_ADP3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x246))
	#define CTS_HW_ADP3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x247))
    #define SDINOVRVAL_31_0 Fld(32,0,AC_FULLDW)//[31:0]
#define CTS_HW_ADP4 (IO_HDMI_BASE+0x248)
	#define CTS_HW_ADP4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x248))
	#define CTS_HW_ADP4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x249))
	#define CTS_HW_ADP4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x24a))
	#define CTS_HW_ADP4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x24b))
    #define CTS_STABLE_CNT_TH Fld(16,16,AC_FULLW32)//[31:16]
    #define NCLK_CNT_TH Fld(8,8,AC_FULLB1)//[15:8]
    #define SDINOVRVAL_39_32 Fld(8,0,AC_FULLB0)//[7:0]
#define CTS_HW_ADP5 (IO_HDMI_BASE+0x24C)
	#define CTS_HW_ADP5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x24c))
	#define CTS_HW_ADP5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x24d))
	#define CTS_HW_ADP5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x24e))
	#define CTS_HW_ADP5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x24f))
    #define CTS_ADU_FREQ_DIFF Fld(16,0,AC_FULLW10)//[15:0]
#define CTS_HW_ADP6 (IO_HDMI_BASE+0x250)
	#define CTS_HW_ADP6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x250))
	#define CTS_HW_ADP6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x251))
	#define CTS_HW_ADP6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x252))
	#define CTS_HW_ADP6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x253))
    #define CTS_REAL_STABLE_CNT_TH Fld(16,8,AC_FULLW21)//[23:8]
    #define CTS_PRE_STABLE_CNT_TH Fld(8,0,AC_FULLB0)//[7:0]
#define CTS_HW_ADP7 (IO_HDMI_BASE+0x25C)
	#define CTS_HW_ADP7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x254))
	#define CTS_HW_ADP7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x255))
	#define CTS_HW_ADP7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x256))
	#define CTS_HW_ADP7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x257))
    #define AUD_FIFO_DIFF Fld(5,16,AC_MSKB2)//[20:16]
    #define FIFO_DIFF_HIGH_TH Fld(5,8,AC_MSKB1)//[12:8]
    #define FIFO_DIFF_LOW_TH Fld(5,0,AC_MSKB0)//[4:0]

#define TMDS_CTRL_STATUS1 (IO_HDMI_BASE+0x260)
	#define TMDS_CTRL_STATUS1_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x260))
	#define TMDS_CTRL_STATUS1_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x261))
	#define TMDS_CTRL_STATUS1_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x262))
	#define TMDS_CTRL_STATUS1_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x263))
    #define HDMI_CH0_STATUS Fld(16,16,AC_FULLW32)//[31:16]
    #define HDMI_CH0_EQERR Fld(4,8,AC_MSKB1)//[11:8]
    #define HDMI_CH0_CDRLPO Fld(7,0,AC_MSKB0)//[6:0]
#define TMDS_CTRL_STATUS2 (IO_HDMI_BASE+0x264)
	#define TMDS_CTRL2_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x264))
	#define TMDS_CTRL2_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x265))
	#define TMDS_CTRL2_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x266))
	#define TMDS_CTRL2_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x267))
    #define HDMI_CH1_STATUS Fld(16,16,AC_FULLW32)//[31:16]
    #define HDMI_CH1_EQERR Fld(4,8,AC_MSKB1)//[11:8]
    #define HDMI_CH1_CDRLPO Fld(7,0,AC_MSKB0)//[6:0]
#define TMDS_CTRL_STATUS3 (IO_HDMI_BASE+0x268)
	#define TMDS_CTRL_STATUS3_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x268))
	#define TMDS_CTRL_STATUS3_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x269))
	#define TMDS_CTRL_STATUS3_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x26a))
	#define TMDS_CTRL_STATUS3_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x26b))
    #define HDMI_CH2_STATUS Fld(16,16,AC_FULLW32)//[31:16]
    #define HDMI_CH2_EQERR Fld(4,8,AC_MSKB1)//[11:8]
    #define HDMI_CH2_CDRLPO Fld(7,0,AC_MSKB0)//[6:0]
#define TMDS_CTRL_STATUS4 (IO_HDMI_BASE+0x26C)
	#define TMDS_CTRL_STATUS4_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x26c))
	#define TMDS_CTRL_STATUS4_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x26d))
	#define TMDS_CTRL_STATUS4_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x26e))
	#define TMDS_CTRL_STATUS4_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x26f))
    #define HDMI_CH2_EQMM Fld(1,2,AC_MSKB0)//[2:2]
    #define HDMI_CH1_EQMM Fld(1,1,AC_MSKB0)//[1:1]
    #define HDMI_CH0_EQMM Fld(1,0,AC_MSKB0)//[0:0]
#define TMDS_CTRL_STATUS5 (IO_HDMI_BASE+0x270)
	#define TMDS_CTRL_STATUS5_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x270))
	#define TMDS_CTRL_STATUS5_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x271))
	#define TMDS_CTRL_STATUS5_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x272))
	#define TMDS_CTRL_STATUS5_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x273))
    #define CS1_PRBS_ERRCNT1 Fld(16,16,AC_FULLW32)//[31:16]
    #define CS0_PRBS_ERRCNT1 Fld(16,0,AC_FULLW10)//[15:0]
#define TMDS_CTRL_STATUS6 (IO_HDMI_BASE+0x274)
	#define TMDS_CTRL_STATUS6_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x274))
	#define TMDS_CTRL_STATUS6_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x275))
	#define TMDS_CTRL_STATUS6_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x276))
	#define TMDS_CTRL_STATUS6_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x277))
    #define PRBS_THPASS Fld(1,29,AC_MSKB3)//[29:29]
    #define PRBS_DONE Fld(1,28,AC_MSKB3)//[28:28]
    #define CS_PRBS_STATUS2 Fld(4,24,AC_MSKB3)//[27:24]
    #define CS_PRBS_STATUS1 Fld(4,20,AC_MSKB2)//[23:20]
    #define CS_PRBS_STATUS0 Fld(4,16,AC_MSKB2)//[19:16]
    #define CS_PRBS_ERRCNT2 Fld(16,0,AC_FULLW10)//[15:0]
#define TMDS_CTRL_STATUS7 (IO_HDMI_BASE+0x278)
	#define TMDS_CTRL_STATUS7_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x278))
	#define TMDS_CTRL_STATUS7_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x279))
	#define TMDS_CTRL_STATUS7_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x27a))
	#define TMDS_CTRL_STATUS7_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x27b))
    #define DEBUG_CDR_DATA Fld(30,0,AC_MSKDW)//[29:0]
#define TMDS_CTRL8 (IO_HDMI_BASE+0x27C)
	#define TMDS_CTRL8_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x27c))
	#define TMDS_CTRL8_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x27d))
	#define TMDS_CTRL8_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x27e))
	#define TMDS_CTRL8_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x27f))
    #define DEBUG_FIFO_DATA Fld(30,0,AC_MSKDW)//[29:0]
#define TMDS_CTRL_STATUS9 (IO_HDMI_BASE+0x280)
	#define TMDS_CTRL_STATUS9_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x280))
	#define TMDS_CTRL_STATUS9_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x281))
	#define TMDS_CTRL_STATUS9_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x282))
	#define TMDS_CTRL_STATUS9_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x283))
    #define DEBUG_DPLL_QD0 Fld(10,20,AC_MSKW32)//[29:20]
    #define DEBUG_DPLL_QD1 Fld(10,10,AC_MSKW21)//[19:10]
    #define DEBUG_DPLL_QD2 Fld(10,0,AC_MSKW10)//[9:0]
#define TMDS_CTRL_STATUS10 (IO_HDMI_BASE+0x284)
	#define TMDS_CTRL_STATUS10_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x284))
	#define TMDS_CTRL_STATUS10_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x285))
	#define TMDS_CTRL_STATUS10_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x286))
	#define TMDS_CTRL_STATUS10_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x287))
    #define DEBUG_DPLL_CTL0 Fld(10,20,AC_MSKW32)//[29:20]
    #define DEBUG_DPLL_CTL1 Fld(10,10,AC_MSKW21)//[19:10]
    #define DEBUG_DPLL_CTL2 Fld(10,0,AC_MSKW10)//[9:0]
#define TMDS_CTRL_STATUS11 (IO_HDMI_BASE+0x288)
	#define TMDS_CTRL_STATUS11_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x288))
	#define TMDS_CTRL_STATUS11_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x289))
	#define TMDS_CTRL_STATUS11_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x28a))
	#define TMDS_CTRL_STATUS11_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x28b))
    #define DEBUG_PSYNC_QP0 Fld(10,20,AC_MSKW32)//[29:20]
    #define DEBUG_PSYNC_QP1 Fld(10,10,AC_MSKW21)//[19:10]
    #define DEBUG_PSYNC_QP2 Fld(10,0,AC_MSKW10)//[9:0]
#define TMDS_CTRL_STATUS12 (IO_HDMI_BASE+0x28C)
	#define TMDS_CTRL_STATUS12_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x28c))
	#define TMDS_CTRL_STATUS12_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x28d))
	#define TMDS_CTRL_STATUS12_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x28e))
	#define TMDS_CTRL_STATUS12_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x28f))
    #define DEBUG_PSYNC_CTL0 Fld(10,20,AC_MSKW32)//[29:20]
    #define DEBUG_PSYNC_CTL1 Fld(10,10,AC_MSKW21)//[19:10]
    #define DEBUG_PSYNC_CTL2 Fld(10,0,AC_MSKW10)//[9:0]
#define TMDS_CTRL_STATUS13 (IO_HDMI_BASE+0x290)
	#define TMDS_CTRL_STATUS13_0 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x290))
	#define TMDS_CTRL_STATUS13_1 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x291))
	#define TMDS_CTRL_STATUS13_2 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x292))
	#define TMDS_CTRL_STATUS13_3 (IO_VIRT_HDMI+(IO_HDMI_BASE+0x293))
    #define DEBUG_DECODE_Q0 Fld(10,20,AC_MSKW32)//[29:20]
    #define DEBUG_DECODE_Q1 Fld(10,10,AC_MSKW21)//[19:10]
    #define DEBUG_DECODE_Q2 Fld(10,0,AC_MSKW10)//[9:0]
#endif
