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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dsp_reg_71.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_reg.h
 *  Brief of file dsp_reg.h.
 *  Details of file dsp_reg.h (optional).
 */
 
#ifndef DSP_REG_H
#define DSP_REG_H

#include "x_typedef.h"
#include "x_hal_5371.h"
#include "x_hal_926.h"
#include "rc2d_int.h"
#include "d2rc_int.h"
#include "dsp_intf.h"

//lint -e717 do ... while(0);

#define SRAM_EFFECT_CONTROL			(0x150F)

// **********************************************************************
// Register definition
// **********************************************************************

#define REG_BUF_MIX_CFG     (0x2c8)
#define REG_BUF_PNT         (0x20b)
#define REG_BUF_PNT_DEC2    (0x20e)
#define SRAM_IEC_FLAG       (0x10d0)
#define SRAM_IEC_FLAG_DEC2  (0x1150)
#define SRAM_SYSTEM_CTRL    (0x1001)
#define LOG_DRAM_REFRESH      (0x1<<4)


// **********************************************************************
// AUDIO/DSP Register in target
// **********************************************************************
#define AUD_REG_OFST                   (0x00)
#define ADSP_REG_OFST                    (0x40)

#ifndef EMU_MT8105         // MT5351
#define RW_AUD_CFG                       (AUD_REG_OFST  + (0x000C >> 2))
// RREG_AUDIO_CFG
    // first decoder
    #define  IEC_CHSEL_POS               (10)               // bit 10~12
    #define  IEC_OFF                     (0x1 << 13)        // bit 13
    #define  IEC_DOWNSAMPLE_MASK         (0x3 << 14)        // bit 14, 15
    // second decoder
    #define  IEC_CHSEL_POS_DEC2          (18)               // bit 18~20
    #define  IEC_OFF_DEC2                (0x1 << 21)        // bit 21
    #define  IEC_DOWNSAMPLE_MASK_DEC2    (0x3 << 22)        // bit 22, 23
	// 0: SPDF output is dec1 PCM data, 1: dec2 PCM data
	#define  SPDIFOUT_SW_SPDF_DEC2_POS	 (25)				// bit 25
	#define  SPDIFOUT_SW_SPDF_DEC2		 (0x1 << SPDIFOUT_SW_SPDF_DEC2_POS)
	
	
#define RW_SPLIN_BLK                     (AUD_REG_OFST +  (0x0010 >> 2))
//#define RW_SPLIN_BLK_DEC2              (AUD_REG_OFST +  (0x0014 >> 2))
//#define RW_SPLIN_TYPE_DEC2             (AUD_REG_OFST +  (0x0020 >> 2))

#define RW_ADSP_CFG                      (AUD_REG_OFST  + (0x0028 >> 2))
// REG_ADSP_CFG
    #define  GBH_DI                      (0x1)
    #define  NO_HOLD_GBS                 (0x1 << 1)
    #define  ADSP_CFG_ICACHE_FLUSH       (0x01 << 2)
    #define  ADSP_CFG_RST_B              (0x01 << 4)
    #define  ADSP_CFG_CLK_ENA            (0x01 << 5)
    #define  ADSP_BOOT_FROM_RAM          (0x01 << 6)
    #define  ADSP_CACHE_SETTING_POS      (10)               // bit 10~12
    #define  ADSP_CACHE_SETTING_MASK     (0x07)
    #define  ADSP_CACHE_SETTING          (ADSP_CACHE_SETTING_MASK << ADSP_CACHE_SETTING_POS)
        #define  CACHE_512                   0x04
        #define  CACHE_1K                    0x02
        #define  CACHE_256                   0x01
    #define  ADSP_CACHE_INST_SIZE_POS    (16)               // bit 16, 17
    #define  ADSP_CACHE_INST_SIZE_MASK   (0x03)
    #define  ADSP_CACHE_INST_SIZE        (ADSP_CACHE_INST_SIZE_MASK << ADSP_CACHE_INST_SIZE_POS)
        #define  SIZE32X64                   0x0
        #define  SIZE16X64                   0x1
        #define  SIZE08X64                   0x2
        #define  SIZE04X64                   0x3
   #define  ADSP_CMPTRD_THRES_POS        (18)               // bit 18, 19
   #define  ADSP_CMPTRD_THRES_MASK       (0x03)
   #define  ADSP_CMPTRD_THRES            (ADSP_CMPTRD_THRES_MASK << ADSP_CMPTRD_THRES_POS)
       #define  THRES_UNFULL                 0x0
       #define  THRES_HALF                   0x1
       #define  THRES_3_4                    0x2
   #define  ADSP_AUD_DRAM_CLK_OFF        (0x01 << 31)       // bit 31

#define RW_SD_ADR                        (AUD_REG_OFST  + (0x0040 >> 2))
// RREG_SD_ADR
#define RW_SD_ADR_INTR                   (AUD_REG_OFST  + (0x0044 >> 2))
// RREG_SD_ADR_INTR
#define RW_LD                            (AUD_REG_OFST  + (0x0048 >> 2))
// RREG_LD

#define RO_R2D_STA                       (AUD_REG_OFST  + (0x004c >> 2))
    #define  DSP_B                          (0x1)

#define RW_R2D_RD                        (AUD_REG_OFST  + (0x0050 >> 2))

#define RW_DSP_SBLK01                    (AUD_REG_OFST  + (0x0054 >> 2))
// RREG_RW_DRAM_SBLK01
#define RW_DSP_SBLK23                    (AUD_REG_OFST  + (0x0058 >> 2))
// RREG_RW_DRAM_SBLK23
#define RW_DRAM_CMPT_SBLK01              (AUD_REG_OFST  + (0x005c >> 2))
// RREG_DRAM_CMPT_SBLK01
#define RW_DRAM_CMPT_SBLK23              (AUD_REG_OFST  + (0x0060 >> 2))
// RREG_DRAM_CMPT_SBLK23
#define RW_DSP_IC_DRAM_SBLK01            (AUD_REG_OFST  + (0x0074 >> 2))
// RREG_IC_DRAM_SBLK01
#define RW_DSP_IC_DRAM_SBLK23            (AUD_REG_OFST  + (0x0078 >> 2))
// RREG_IC_DRAM_SBLK23
#define RW_DSP_IC_DRAM_SBLK45            (AUD_REG_OFST  + (0x007c >> 2))
// RREG_IC_DRAM_SBLK45

#define RO_D2R_STA                       (AUD_REG_OFST  + (0x0080 >> 2))
// RREG_DSP2RC_STA
    #define  DREQ                        (0x1)
    #define  D2RDAV                      (0x1 << 1)
#define RO_D2R_SDA                (AUD_REG_OFST  + (0x0084 >> 2))
// RREG_DSP2RC_SD_ADR
#define RO_D2R_RD                 (AUD_REG_OFST  + (0x0088 >> 2))
// RREG_DSP2RC_RDATA

// Light: MT5371 adds the following registers
#define RW_ADSP_ENV_BAK           (AUD_REG_OFST  + (0x0064 >> 2))
    #define APGA_MASK                    (0x3FFF)
    #define APGA_GAIN_POS                (9)                // ADSP_ENV_BAK[13:9] APGAG
    #define APGA_GAIN_MASK               (0x1F)
    #define APGA_GAIN                    (APGA_GAIN_MASK << APGA_GAIN_POS)
        #define APGA_GAIN_0DB                (0 << APGA_GAIN_POS)
    #define APGA_BIAS3_POS               (7)
    #define APGA_BIAS3_MASK              (0x3)
    #define APGA_BIAS3                   (APGA_BIAS3_MASK << APGA_BIAS3_POS)
        #define APGA_BIAS3_0UA               (0 << APGA_BIAS3_POS)
        #define APGA_BIAS3_10UA              (1 << APGA_BIAS3_POS)
        #define APGA_BIAS3_20UA              (2 << APGA_BIAS3_POS)      // default
        #define APGA_BIAS3_30UA              (3 << APGA_BIAS3_POS)
    #define APGA_BIAS2_POS               (5)
    #define APGA_BIAS2_MASK              (0x3)
    #define APGA_BIAS2                   (APGA_BIAS2_MASK << APGA_BIAS2_POS)
        #define APGA_BIAS2_0UA               (0 << APGA_BIAS2_POS)
        #define APGA_BIAS2_10UA              (1 << APGA_BIAS2_POS)
        #define APGA_BIAS2_20UA              (2 << APGA_BIAS2_POS)      // default
        #define APGA_BIAS2_30UA              (3 << APGA_BIAS2_POS)
    #define APGA_BIAS1_POS               (3)
    #define APGA_BIAS1_MASK              (0x3)
    #define APGA_BIAS1                   (APGA_BIAS1_MASK << APGA_BIAS1_POS)
        #define APGA_BIAS1_0UA               (0 << APGA_BIAS1_POS)
        #define APGA_BIAS1_10UA              (1 << APGA_BIAS1_POS)
        #define APGA_BIAS1_20UA              (2 << APGA_BIAS1_POS)      // default
        #define APGA_BIAS1_30UA              (3 << APGA_BIAS1_POS)
    #define APGA_LPF15MEN_POS            (2)
    #define APGA_LPF15MEN_MASK           (0x01)
    #define APGA_LPF15MEN                (APGA_LPF15MEN_MASK << APGA_LPF15MEN_POS)
        #define APGA_LPF15MEN_300KHZ         (0 << APGA_LPF15MEN_POS)
        #define APGA_LPF15MEN_15MHZ          (1 << APGA_LPF15MEN_POS)   // default
    #define APGA_FLREN_POS               (1)
        #define APGA_FLREN_DIS               (0 << APGA_FLREN_POS)
        #define APGA_FLREN_EN                (1 << APGA_FLREN_POS)      // default
    // APGA power down
    #define APGA_PD_POS                  (0)
    #define APGA_PD_MASK                 (0x01)
    #define APGA_PD                      (APGA_PD_MASK << APGA_PD_POS)
        #define APGA_PD_POWER_ON             (0 << APGA_PD_POS)
        #define APGA_PD_POWER_DOWN           (1 << APGA_PD_POS) // default
#define RW_AOUT_DRAM_CFG          (AUD_REG_OFST  + (0x008C >> 2))
    #define AOUT_WAIT_CYCLE_16           (0 << 0)
    #define AOUT_WAIT_CYCLE_32           (1 << 0)
    #define AOUT_WAIT_CYCLE_64           (2 << 0)
    #define AOUT_WAIT_CYCLE_128          (3 << 0)
    #define AOUT_WAIT_PERIOD_NO          (0 << 2)
    #define AOUT_WAIT_PERIOD_1CH         (1 << 2)
    #define AOUT_WAIT_PERIOD_2CH         (2 << 2)
    #define AOUT_WAIT_PERIOD_4CH         (3 << 2)
    #define AOUT_32BIT_MODE              (0 << 4)
    #define AOUT_128BIT_MODE             (1 << 4)
    
#define RW_AADC_CFG0              (AUD_REG_OFST  + (0x0090 >> 2))
    #define ADCCKSEL_POS                 (20)
        #define ADCCKSEL_RISING_EDGE         (0 << ADCCKSEL_POS)
        #define ADCCKSEL_FALLING_EDGE        (1 << ADCCKSEL_POS)
    #define NCK_CTRL_POS                 (12)
        #define NCK_CTRL_300PS               (0 << NCK_CTRL_POS)
        #define NCK_CTRL_350PS               (1 << NCK_CTRL_POS)// default
        #define NCK_CTRL_400PS               (3 << NCK_CTRL_POS)
    // external/internal clock (108 MHz) selection
    #define CKSEL_POS                    (11)
    #define CKSEL_MASK                   (0x01)
    #define CKSEL                        (CKSEL_MASK << CKSEL_POS)
        #define CKSEL_EXTERN_CLK             (0 << CKSEL_POS)
        #define CKSEL_INTERN_CLK             (1 << CKSEL_POS)
    #define CKDIV_POS                    (10)
        #define CKDIV_FS                     (0 << CKDIV_POS)
        #define CKDIV_FS_2                   (1 << CKDIV_POS)
    #define TRGSEL_POS                   (9)
        #define TRGSEL_FALLING_EDGE          (0 << TRGSEL_POS)
        #define TRGSEL_RISING_EDGE           (1 << TRGSEL_POS)
    #define CLR_POS                      (8)
    #define CLR_MASK                     (0x01)
    #define CLR                          (CLR_MASK << CLR_POS)
        #define CLR_DIS                      (0 << CLR_POS)
        #define CLR_EN                       (1 << CLR_POS)
    #define VREFTEST_POS                 (7)
        #define VREFTEST_DIS                 (0 << VREFTEST_POS)
        #define VREFTEST_EN                  (1 << VREFTEST_POS)
    #define ADCTEST_POS                  (6)
        #define ADCTEST_DIS                  (0 << ADCTEST_POS)
        #define ADCTEST_EN                   (1 << ADCTEST_POS)

#define RW_AADC_CFG0_ECO              (AUD_REG_OFST  + (0x0090 >> 2))
	#define RISC_SEL_ECO                      (1 << 15)
    #define ADCCKSEL_POS_ECO                  (20)
        #define ADCCKSEL_RISING_EDGE_ECO          (0 << ADCCKSEL_POS_ECO)
        #define ADCCKSEL_FALLING_EDGE_ECO         (1 << ADCCKSEL_POS_ECO)
    #define NCK_CTRL_POS_ECO                  (28)
        #define NCK_CTRL_300PS_ECO                (0 << NCK_CTRL_POS_ECO)
        #define NCK_CTRL_350PS_ECO                (1 << NCK_CTRL_POS_ECO)// default
        #define NCK_CTRL_400PS_ECO                (3 << NCK_CTRL_POS_ECO)
    // external/internal clock (108 MHz) selection
    #define CKSEL_POS_ECO                     (27)
    #define CKSEL_MASK_ECO                    (0x01)
    #define CKSEL_ECO                         (CKSEL_MASK << CKSEL_POS_ECO)
        #define CKSEL_EXTERN_CLK_ECO              (0 << CKSEL_POS_ECO)
        #define CKSEL_INTERN_CLK_ECO              (1 << CKSEL_POS_ECO)
    #define CKDIV_POS_ECO                     (26)
        #define CKDIV_FS_ECO                      (0 << CKDIV_POS_ECO)
        #define CKDIV_FS_2_ECO                    (1 << CKDIV_POS_ECO)
    #define TRGSEL_POS_ECO                    (25)
        #define TRGSEL_FALLING_EDGE_ECO           (0 << TRGSEL_POS_ECO)
        #define TRGSEL_RISING_EDGE_ECO            (1 << TRGSEL_POS_ECO)
    #define CLR_POS_ECO                       (24)
    #define CLR_MASK_ECO                      (0x01)
    #define CLR_ECO                           (CLR_MASK_ECO << CLR_POS_ECO)
        #define CLR_DIS_ECO                       (0 << CLR_POS_ECO)
        #define CLR_EN_ECO                        (1 << CLR_POS_ECO)        
    #define VREFTEST_POS_ECO                  (7)
        #define VREFTEST_DIS_ECO                  (0 << VREFTEST_POS_ECO)
        #define VREFTEST_EN_ECO                   (1 << VREFTEST_POS_ECO)
    #define ADCTEST_POS_ECO                   (6)
        #define ADCTEST_DIS_ECO                   (0 << ADCTEST_POS_ECO)
        #define ADCTEST_EN_ECO                    (1 << ADCTEST_POS_ECO)

    #define APGA_GAIN_POS_ECO                 (0)                // AADC_CFG0[4:0] APGAG    
    #define APGA_GAIN_MASK_ECO               (0x1F)    
    #define APGA_GAIN_ECO                    (APGA_GAIN_MASK_ECO << APGA_GAIN_POS_ECO)    
        #define APGA_GAIN_0DB_ECO                (0 << APGA_GAIN_POS_ECO)
    #define APGA_OFFSET_POS_ECO              (0x8)                  // APGA_CFG0[13:8] OFFSET   
    #define APGA_OFFSET_MASK_ECO             (0x3F)   
    #define APGA_OFFSET_ECO                  (APGA_OFFSET_MASK_ECO << APGA_OFFSET_POS_ECO)       
        #define APGA_OFFSET_0V_ECO               (0 << APGA_OFFSET_POS_ECO)    
    
#define RW_AADC_CFG1              (AUD_REG_OFST  + (0x0094 >> 2))
    #define ADC_REFPADJ_POS              (28)
        #define ADC_REFPADJ_1_800V           ((UINT32)0 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_1_825V           ((UINT32)1 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_1_850V           ((UINT32)2 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_1_875V           ((UINT32)3 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_1_900V           ((UINT32)4 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_1_925V           ((UINT32)5 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_1_950V           ((UINT32)6 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_1_975V           ((UINT32)7 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_000V           ((UINT32)8 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_025V           ((UINT32)9 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_050V           ((UINT32)10 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_075V           ((UINT32)11 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_100V           ((UINT32)12 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_125V           ((UINT32)13 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_150V           ((UINT32)14 << ADC_REFPADJ_POS)
        #define ADC_REFPADJ_2_175V           ((UINT32)15 << ADC_REFPADJ_POS)
    #define ADC_REFNADJ_POS              (24)
        #define ADC_REFNADJ_0_800V           ((UINT32)0 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_0_825V           ((UINT32)1 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_0_850V           ((UINT32)2 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_0_875V           ((UINT32)3 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_0_900V           ((UINT32)4 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_0_925V           ((UINT32)5 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_0_950V           ((UINT32)6 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_0_975V           ((UINT32)7 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_000V           ((UINT32)8 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_025V           ((UINT32)9 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_050V           ((UINT32)10 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_075V           ((UINT32)11 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_100V           ((UINT32)12 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_125V           ((UINT32)13 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_150V           ((UINT32)14 << ADC_REFNADJ_POS)
        #define ADC_REFNADJ_1_175V           ((UINT32)15 << ADC_REFNADJ_POS)
    #define ADC_VICMADJ_POS              (20)
        #define ADC_VICMADJ_1_300V           ((UINT32)0 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_325V           ((UINT32)1 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_350V           ((UINT32)2 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_375V           ((UINT32)3 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_400V           ((UINT32)4 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_425V           ((UINT32)5 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_450V           ((UINT32)6 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_475V           ((UINT32)7 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_500V           ((UINT32)8 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_525V           ((UINT32)9 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_550V           ((UINT32)10 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_575V           ((UINT32)11 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_600V           ((UINT32)12 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_625V           ((UINT32)13 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_650V           ((UINT32)14 << ADC_VICMADJ_POS)
        #define ADC_VICMADJ_1_675V           ((UINT32)15 << ADC_VICMADJ_POS)
    #define ADC_VOCMADJ_POS              (16)
        #define ADC_VOCMADJ_1_300V           ((UINT32)0 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_325V           ((UINT32)1 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_350V           ((UINT32)2 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_375V           ((UINT32)3 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_400V           ((UINT32)4 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_425V           ((UINT32)5 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_450V           ((UINT32)6 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_475V           ((UINT32)7 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_500V           ((UINT32)8 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_525V           ((UINT32)9 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_550V           ((UINT32)10 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_575V           ((UINT32)11 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_600V           ((UINT32)12 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_625V           ((UINT32)13 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_650V           ((UINT32)14 << ADC_VOCMADJ_POS)
        #define ADC_VOCMADJ_1_675V           ((UINT32)15 << ADC_VOCMADJ_POS)
    #define IFLASH_POS                   (14)
        #define IFLASH_10UA                  ((UINT32)0 << IFLASH_POS)
        #define IFLASH_15UA                  ((UINT32)1 << IFLASH_POS)
        #define IFLASH_20UA                  ((UINT32)2 << IFLASH_POS)
        #define IFLASH_30UA                  ((UINT32)3 << IFLASH_POS)
    #define IVREF_POS                    (12)
        #define IVREF_10UA                   ((UINT32)0 << IVREF_POS)
        #define IVREF_15UA                   ((UINT32)1 << IVREF_POS)
        #define IVREF_20UA                   ((UINT32)2 << IVREF_POS)
        #define IVREF_30UA                   ((UINT32)3 << IVREF_POS)
    #define ISH_POS                      (10)
        #define ISH_40UA                     ((UINT32)0 << ISH_POS)
        #define ISH_50UA                     ((UINT32)1 << ISH_POS)
        #define ISH_60UA                     ((UINT32)2 << ISH_POS)
        #define ISH_80UA                     ((UINT32)3 << ISH_POS)
    #define IGBIAS_POS                   (8)
        #define IGBIAS_11UA                  ((UINT32)0 << IGBIAS_POS)
        #define IGBIAS_16UA                  ((UINT32)1 << IGBIAS_POS)
        #define IGBIAS_20UA                  ((UINT32)2 << IGBIAS_POS)
        #define IGBIAS_26UA                  ((UINT32)3 << IGBIAS_POS)
    #define ISUB4_POS                    (6)
        #define ISUB4_10UA                   ((UINT32)0 << ISUB4_POS)
        #define ISUB4_15UA                   ((UINT32)1 << ISUB4_POS)
        #define ISUB4_20UA                   ((UINT32)2 << ISUB4_POS)
        #define ISUB4_30UA                   ((UINT32)3 << ISUB4_POS)
    #define ISUB3_POS                    (4)
        #define ISUB3_10UA                   ((UINT32)0 << ISUB3_POS)
        #define ISUB3_15UA                   ((UINT32)1 << ISUB3_POS)
        #define ISUB3_20UA                   ((UINT32)2 << ISUB3_POS)
        #define ISUB3_30UA                   ((UINT32)3 << ISUB3_POS)
    #define ISUB2_POS                    (2)
        #define ISUB2_10UA                   ((UINT32)0 << ISUB2_POS)
        #define ISUB2_15UA                   ((UINT32)1 << ISUB2_POS)
        #define ISUB2_20UA                   ((UINT32)2 << ISUB2_POS)
        #define ISUB2_30UA                   ((UINT32)3 << ISUB2_POS)
    #define ISUB1_POS                    (0)
        #define ISUB1_10UA                   ((UINT32)0 << ISUB1_POS)
        #define ISUB1_15UA                   ((UINT32)1 << ISUB1_POS)
        #define ISUB1_20UA                   ((UINT32)2 << ISUB1_POS)
        #define ISUB1_30UA                   ((UINT32)3 << ISUB1_POS)
#define RW_AADC_CFG2              (AUD_REG_OFST  + (0x0098 >> 2))
    #define IMDAC4_POS                   (30)
        #define IMDAC4_10UA                  ((UINT32)0 << IMDAC4_POS)
        #define IMDAC4_15UA                  ((UINT32)1 << IMDAC4_POS)
        #define IMDAC4_20UA                  ((UINT32)2 << IMDAC4_POS)  // default
        #define IMDAC4_30UA                  ((UINT32)3 << IMDAC4_POS)
    #define IMDAC3_POS                   (28)
        #define IMDAC3_10UA                  ((UINT32)0 << IMDAC3_POS)
        #define IMDAC3_15UA                  ((UINT32)1 << IMDAC3_POS)
        #define IMDAC3_20UA                  ((UINT32)2 << IMDAC3_POS)  // default
        #define IMDAC3_30UA                  ((UINT32)3 << IMDAC3_POS)
    #define IMDAC2_POS                   (26)
        #define IMDAC2_10UA                  ((UINT32)0 << IMDAC2_POS)
        #define IMDAC2_15UA                  ((UINT32)1 << IMDAC2_POS)
        #define IMDAC2_20UA                  ((UINT32)2 << IMDAC2_POS)  // default
        #define IMDAC2_30UA                  ((UINT32)3 << IMDAC2_POS)
    #define IMDAC1_POS                   (24)
        #define IMDAC1_10UA                  ((UINT32)0 << IMDAC1_POS)
        #define IMDAC1_15UA                  ((UINT32)1 << IMDAC1_POS)
        #define IMDAC1_20UA                  ((UINT32)2 << IMDAC1_POS)  // default
        #define IMDAC1_30UA                  ((UINT32)3 << IMDAC1_POS)
    // dual/single input selection
    #define CHSEL_POS                    (20)
    #define CHSEL_MASK                   (0x03)
    #define CHSEL                        ((UINT32)CHSEL_MASK << CHSEL_POS)
        #define CHSEL_STEREO                 ((UINT32)0 << CHSEL_POS)
        #define CHSEL_CH0                    ((UINT32)1 << CHSEL_POS)   // default
        #define CHSEL_CH1                    ((UINT32)2 << CHSEL_POS)
    // power down ADC reference
    #define PD_REF_POS                   (17)
    #define PD_REF_MASK                  (0x01)
    #define PD_REF                       ((UINT32)PD_REF_MASK << PD_REF_POS)
        #define PD_REF_POWER_ON              ((UINT32)0 << PD_REF_POS)
        #define PD_REF_POWER_DOWN            ((UINT32)1 << PD_REF_POS)  // default
    // power down ADC core
    #define PD_ADC_POS                   (16)
    #define PD_ADC_MASK                  (0x01)
    #define PD_ADC                       ((UINT32)PD_ADC_MASK << PD_ADC_POS)
        #define PD_ADC_POWER_ON              ((UINT32)0 << PD_ADC_POS)
        #define PD_ADC_POWER_DOWN            ((UINT32)1 << PD_ADC_POS)  // default
#define RW_AUDIN_CFG                     (AUD_REG_OFST  + (0x009C >> 2))
#define RW_AENV_CFG                      (AUD_REG_OFST  + (0x00A8 >> 2))
#define RW_APGA_CFG0                     (AUD_REG_OFST  + (0x035C >> 2))
    #define APGA_OFFSET_POS              (0x1)                  // APGA_CFG0[6:1] OFFSET
    #define APGA_OFFSET_MASK             (0x3F)
    #define APGA_OFFSET                  ((UINT32)APGA_OFFSET_MASK << APGA_OFFSET_POS)
        #define APGA_OFFSET_0V               (0 << APGA_OFFSET_POS)
    #define APGA_GAIN6DB_POS             (0)
        #define APGA_GAIN6DB_DIS             (0 << APGA_GAIN6DB_POS)
        #define APGA_GAIN6DB_EN              ((UINT32)1 << APGA_GAIN6DB_POS)
#define RW_APGA_CFG1                     (AUD_REG_OFST  + (0x0360 >> 2))
    #define APGA_VOCMADJ10_POS           (6)
    #define APGA_VOCMADJ10_MASK          (0x03)
        #define APGA_VOCMADJ10_1_30V         ((UINT32)0 << APGA_VOCMADJ10_POS)
        #define APGA_VOCMADJ10_1_35V         ((UINT32)1 << APGA_VOCMADJ10_POS)
        #define APGA_VOCMADJ10_1_40V         ((UINT32)2 << APGA_VOCMADJ10_POS)
        #define APGA_VOCMADJ10_1_45V         ((UINT32)3 << APGA_VOCMADJ10_POS)
        #define APGA_VOCMADJ10_1_50V         ((UINT32)0 << APGA_VOCMADJ10_POS) // default
        #define APGA_VOCMADJ10_1_55V         ((UINT32)1 << APGA_VOCMADJ10_POS)
        #define APGA_VOCMADJ10_1_60V         ((UINT32)2 << APGA_VOCMADJ10_POS)
        #define APGA_VOCMADJ10_1_65V         ((UINT32)3 << APGA_VOCMADJ10_POS)
    #define APGA_REFPADJ_POS             (3)
    #define APGA_REFPADJ_MASK            (0x07)
        #define APGA_REFPADJ_1_80V           ((UINT32)0 << APGA_REFPADJ_POS)
        #define APGA_REFPADJ_1_85V           ((UINT32)1 << APGA_REFPADJ_POS)
        #define APGA_REFPADJ_1_90V           ((UINT32)2 << APGA_REFPADJ_POS)
        #define APGA_REFPADJ_1_95V           ((UINT32)3 << APGA_REFPADJ_POS)
        #define APGA_REFPADJ_2_00V           ((UINT32)4 << APGA_REFPADJ_POS) // default
    #define APGA_REFNADJ_POS             (0)
    #define APGA_REFNADJ_MASK            (0x07)
        #define APGA_REFNADJ_0_80V           ((UINT32)0 << APGA_REFNADJ_POS)
        #define APGA_REFNADJ_0_85V           ((UINT32)1 << APGA_REFNADJ_POS)
        #define APGA_REFNADJ_0_90V           ((UINT32)2 << APGA_REFNADJ_POS)
        #define APGA_REFNADJ_0_95V           ((UINT32)3 << APGA_REFNADJ_POS)
        #define APGA_REFNADJ_1_00V           ((UINT32)4 << APGA_REFNADJ_POS) // default
        #define APGA_REFNADJ_1_05V           ((UINT32)5 << APGA_REFNADJ_POS)
        #define APGA_REFNADJ_1_10V           ((UINT32)6 << APGA_REFNADJ_POS)
        #define APGA_REFNADJ_1_15V           ((UINT32)7 << APGA_REFNADJ_POS)
#define RW_APGA_CFG2              (AUD_REG_OFST  + (0x0364 >> 2))
    // APGA input path selection
    #define AUCHSEL_POS                  (1)
    #define AUCHSEL_MASK                 (0x03)
    #define AUCHSEL                      ((UINT32)AUCHSEL_MASK << AUCHSEL_POS)
        #define AUCHSEL_NO                   ((UINT32)0 << AUCHSEL_POS)
        #define AUCHSEL_SIF                  ((UINT32)1 << AUCHSEL_POS)
        #define AUCHSEL_AF                   ((UINT32)2 << AUCHSEL_POS)
    #define APGA_VOCMADJ2_POS            (0)
    #define APGA_VOCMADJ2_MASK           (0x01)
        #define APGA_VOCMADJ2_1_30V          ((UINT32)0 << APGA_VOCMADJ2_POS)
        #define APGA_VOCMADJ2_1_35V          ((UINT32)0 << APGA_VOCMADJ2_POS)
        #define APGA_VOCMADJ2_1_40V          ((UINT32)0 << APGA_VOCMADJ2_POS)
        #define APGA_VOCMADJ2_1_45V          ((UINT32)0 << APGA_VOCMADJ2_POS)
        #define APGA_VOCMADJ2_1_50V          ((UINT32)1 << APGA_VOCMADJ2_POS) // default
        #define APGA_VOCMADJ2_1_55V          ((UINT32)1 << APGA_VOCMADJ2_POS)
        #define APGA_VOCMADJ2_1_60V          ((UINT32)1 << APGA_VOCMADJ2_POS)
        #define APGA_VOCMADJ2_1_65V          ((UINT32)1 << APGA_VOCMADJ2_POS)
#define RW_APGA_CFG3              (AUD_REG_OFST  + (0x0368 >> 2))
    #define APGA_TEST_POS                (2)
        #define APGA_TEST_DIS                ((UINT32)0 << APGA_TEST_POS)
        #define APGA_TEST_EN                 ((UINT32)1 << APGA_TEST_POS)
#define RW_APGA_CFG4              (AUD_REG_OFST  + (0x0370 >> 2))
    #define APGA_TEST_MODE_POS           (0)
        #define APGA_TEST_MODE_NONE          ((UINT32)0 << APGA_TEST_MODE_POS)
        #define APGA_TEST_MODE_VREF          ((UINT32)0x80 << APGA_TEST_MODE_POS)
        #define APGA_TEST_MODE_VOCM          ((UINT32)0x40 << APGA_TEST_MODE_POS)
#define RW_APGA_CFG5              (AUD_REG_OFST  + (0x0374 >> 2))
    #define APGA_RESERVE_POS             (0)
        #define APGA_RESERVE_DEFAULT         ((UINT32)6 << APGA_RESERVE_POS)

#define RW_ADSPCR                 (ADSP_REG_OFST)
    #define  PROM_WR                     ((UINT32)0x1 << 1)
    #define  PRAM_WR                     ((UINT32)0x1 << 2)
    #define  DROM_WR                     ((UINT32)0x1 << 3)
    #define  HOT_DNLD                    ((UINT32)0x1 << 6)
#define RW_PROM_DNLD              (ADSP_REG_OFST + (0x0004 >> 2))
// PROM_DOWNLOAD_REG
#define RW_PRAM_DNLD              (ADSP_REG_OFST + (0x0008 >> 2))
// PRAM_DOWNLOAD_REG
#define RW_DROM_DNLD              (ADSP_REG_OFST + (0x0050 >> 2))
// DROM_DOWNLOAD_REG
#define WO_CNTR_CLR               (ADSP_REG_OFST + (0x000C >> 2))
    // COUNTER_CONTROL_REG
    #define  CNTR_CLR                    (0x1 << 1)
#define RW_DNLD_CNTR              (ADSP_REG_OFST + (0x0010 >> 2))
// PRAM_ADDR_REG
#else           // MT8105
#define RW_AUD_CFG                (AUD_REG_OFST  + (0x0C >> 2))
// RREG_AUDIO_CFG
    // first decoder
    #define  IEC_CHSEL_POS               8                  // bit 8~10
    #define  IEC_OFF                     ((UINT32)0x1 << 12)        // bit 12
    #define  IEC_DOWNSAMPLE_MASK         ((UINT32)0x3 << 13)        // bit 13, 13
    // second decoder --> dummy for MT8105 emulation
    #define  IEC_CHSEL_POS_DEC2          0                 // bit 0
    #define  IEC_OFF_DEC2                ((UINT32)0x1 << 0)        // bit 0
    #define  IEC_DOWNSAMPLE_MASK_DEC2    ((UINT32)0x3 << 0)        // bit 0

//#define RW_DSP_SBLK               (AUD_REG_OFST  + (0x10 >> 2))
//#define RW_GBS_CFG                (AUD_REG_OFST  + (0x20 >> 2))
#define RW_ADSP_CFG               (AUD_REG_OFST  + (0x20 >> 2))
// REG_ADSP_CFG
   #define  GBH_DI                      (0x1)
   #define  NO_HOLD_GBS                 (0x1 << 1)
   #define  ADSP_CFG_ICACHE_FLUSH       (0x01 << 2)
   #define  ADSP_CFG_RST_B              (0x01 << 4)
   #define  ADSP_CFG_CLK_ENA            (0x01 << 5)
   #define  ADSP_BOOT_FROM_RAM          (0x01 << 6)
   #define  ADSP_CACHE_SETTING_POS      8                   // bit 8~12
   #define  ADSP_CACHE_SETTING_MASK     0x1f
   #define  ADSP_CACHE_SETTING          (ADSP_CACHE_SETTING_MASK << ADSP_CACHE_SETTING_POS)
       #define  CACHE_512                   0x08
       #define  CACHE_1K                    0x04
       #define  CACHE_2K                    0x02
   #define  ADSP_CACHE_INST_SIZE_POS    16                  // bit 16, 17
   #define  ADSP_CACHE_INST_SIZE_MASK   0x03
   #define  ADSP_CACHE_INST_SIZE        (ADSP_CACHE_INST_SIZE_MASK << ADSP_CACHE_INST_SIZE_POS)
       #define  SIZE32X64                   0x0
       #define  SIZE16X64                   0x1
       #define  SIZE08X64                   0x2
       #define  SIZE04X64                   0x3

#define RW_SD_ADR                 (AUD_REG_OFST  + (0x40 >> 2))
// RREG_SD_ADR
#define RW_SD_ADR_INTR            (AUD_REG_OFST  + (0x44 >> 2))
// RREG_SD_ADR_INTR
#define RW_LD                     (AUD_REG_OFST  + (0x48 >> 2))
// RREG_LD

#define RO_R2D_STA                (AUD_REG_OFST  + (0x50 >> 2))
    #define  DSP_B                       (0x1)

//#define RW_R2D_RD                 (AUD_REG_OFST  + (0x54 >> 2))

#define RW_DSP_SBLK01             (AUD_REG_OFST  + (0x58 >> 2))
// RREG_RW_DRAM_SBLK01
#define RW_DSP_SBLK23             (AUD_REG_OFST  + (0x5c >> 2))
// RREG_RW_DRAM_SBLK23
#define RW_DRAM_CMPT_SBLK01       (AUD_REG_OFST  + (0x60 >> 2))
// RREG_DRAM_CMPT_SBLK01
#define RW_DRAM_CMPT_SBLK23       (AUD_REG_OFST  + (0x64 >> 2))
// RREG_DRAM_CMPT_SBLK23
#define RW_DSP_IC_DRAM_SBLK01     (AUD_REG_OFST  + (0x74 >> 2))
// RREG_IC_DRAM_SBLK01
#define RW_DSP_IC_DRAM_SBLK23     (AUD_REG_OFST  + (0x78 >> 2))
// RREG_IC_DRAM_SBLK23
#define RW_DSP_IC_DRAM_SBLK45     (AUD_REG_OFST  + (0x7c >> 2))
// RREG_IC_DRAM_SBLK45

#define RO_D2R_STA                (AUD_REG_OFST  + (0x80 >> 2))
// RREG_DSP2RC_STA
    #define  DREQ                        (0x1)
    #define  D2RDAV                      (0x1 << 1)
#define RO_D2R_SDA                (AUD_REG_OFST  + (0x84 >> 2))
// RREG_DSP2RC_SD_ADR
#define RO_D2R_RD                 (AUD_REG_OFST  + (0x88 >> 2))
// RREG_DSP2RC_RDATA

#define RW_ADSPCR                 (ADSP_REG_OFST)
    #define  PROM_WR                     (0x1 << 1)
    #define  PRAM_WR                     (0x1 << 2)
    #define  DROM_WR                     (0x1 << 3)
    #define  HOT_DNLD                    (0x1 << 6)
#define RW_PROM_DNLD              (ADSP_REG_OFST + (0x04 >> 2))
// PROM_DOWNLOAD_REG
#define RW_PRAM_DNLD              (ADSP_REG_OFST + (0x08 >> 2))
// PRAM_DOWNLOAD_REG
#define RW_DROM_DNLD              (ADSP_REG_OFST + (0x50 >> 2))
// DROM_DOWNLOAD_REG
#define WO_CNTR_CLR               (ADSP_REG_OFST + (0x0C >> 2))
// COUNTER_CONTROL_REG
    #define  CNTR_CLR                    (0x1 << 1)
#define RW_DNLD_CNTR              (ADSP_REG_OFST + (0x10 >> 2))

#define RW_AIN_ACK_CFG            (AUD_REG_OFST  + (0xB4 >> 2))

#define RW_SPLIN_BLK              (AUD_REG_OFST +  (0x14 >> 2))

#endif                 // end of EMU_MT8105

#define RW_SPLIN_CTL              (AUD_REG_OFST +  (0x0018 >> 2))
    #define SPLIN_ENABLE                  (0x01)
    #define SPLIN_24BIT                   (0x01<<1)
    #define SPLIN_RAW_MSB16BIT            (0x01<<2)
    #define SPLIN_SWAP_BYTES              (0x01<<3) 
    #define SPLIN_INT_PERIOD_MASK         (0x03<<4)
    #define SPLIN_INT_PERIOD_DISABLE      (0x00<<4)
    #define SPLIN_INT_PERIOD_064DWRD      (0x01<<4)
    #define SPLIN_INT_PERIOD_128DWRD      (0x02<<4)
    #define SPLIN_INT_PERIOD_256DWRD      (0x03<<4)
    #define SPLIN_CHNUM_2CH               (0x00<<6)
    #define SPLIN_CHNUM_4CH               (0x01<<6)
    #define SPLIN_CHNUM_6CH               (0x02<<6)
    #define SPLIN_CHNUM_8CH               (0x03<<6)
    #define SPLIN_WRITEPTR_AS_AUDPTR      (0x01<<8)
    #define SPLIN_PSEUDO_BS               (0x01<<10)
    #define SPLIN_WRITEPTR_AS_AUDPTR_DEC2 (0x01<<24)

#define RW_SPLIN_TYPE             (AUD_REG_OFST + (0x001C >> 2))

#if (0) // Putbits test
#define RW_PUTB_CTL              (AUD_REG_OFST +  (0x0068 >> 2))
#define RW_PUTBS_BLK0_DSP    (RW_PUTB_CTL)
#define RW_PUTBS_BLK1_DSP    (RW_PUTB_CTL+1)
#endif

#if 0
#define RW_PB_CMPR_MASK  (ADSP_REG_OFST + (0x14 >> 2))
#define RW_PB_CMPR_TRGE  (ADSP_REG_OFST + (0x18 >> 2))
#define RW_PB_TRGE       (ADSP_REG_OFST + (0x1C >> 2))
#define TRGT_EN          0x1
#define TRIG_RISE        (0x1 << 1)
#define TRGT_LAT         (0x1 << 2)
#define CMPR_EN          (0x1 << 8)

#define RW_PB_CTR        (ADSP_REG_OFST + (0x20 >> 2))
#define PB_HOLD          0x1

#define RW_PB_IDX        (ADSP_REG_OFST + (0x24 >> 2))
#define RW_PB_DATA       (ADSP_REG_OFST + (0x28 >> 2))
#define RW_PB_SOP_EN     (ADSP_REG_OFST + (0x2C >> 2))
#define RW_PB_STOP_PTR0  (ADSP_REG_OFST + (0x30 >> 2))
#define RW_PB_STOP_PTR1  (ADSP_REG_OFST + (0x34 >> 2))
#define RW_PB_STOP_PTR2  (ADSP_REG_OFST + (0x38 >> 2))
#define RW_PB_STOP_PTR3  (ADSP_REG_OFST + (0x3C >> 2))
#define RW_RS232_DATA    (ADSP_REG_OFST + (0x40 >> 2))

#define RW_RS232_STATUS  (ADSP_REG_OFST + (0x44 >> 2))
#define RD_ALLOW         0x1
#define WR_ALLOW         (0x1 << 1)
#define CHK_END_ERR      (0x1 << 4)
#define PARITY_ERR       (0x1 << 5)
#define RW_DSP_SPLIN_BLK (AUD_REG_OFST+ (0x14>>2))
#define RW_DSP_SPLIN_CTL (AUD_REG_OFST+ (0x18>>2))
#define RW_DSP_SPDIF_TYPE (AUD_REG_OFST+(0x1C>>2))
#define RW_DSP_ABUF_PNT (AUD_REG_OFST+(0xA0>>2))
#endif

// **********************************************************************
// Register Access Macro
// **********************************************************************
#define WriteREG(addr,value)   IO_WRITE32(AUDIO_BASE,(addr<<2),value)
#define ReadREG(addr)          	 IO_READ32(AUDIO_BASE,(addr<<2))

#define SendDSPInt(arg1, arg2) WriteREG(RW_SD_ADR_INTR, \
                                 (((UINT32) arg1) + (((UINT32) arg2) << 8)))
#define SendDSPLInt(arg1, arg2) WriteREG(RW_SD_ADR_INTR, \
                                  (((UINT32) arg1) + (((UINT32) arg2) << 16)))

// **********************************************************************
// Memory Access Macro
// **********************************************************************

//#define WriteDspCommDram(addr, val)   (*((UINT32*)(NONCACHE(_u4DspBuf+((addr)<<2)))) = (val))     //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
#define WriteDspCommDram(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = (val);   \
                                      HalFlushDCache();                                         \
                                      }while(0)

//#define dReadDspCommDram(addr)       (*((UINT32*)(NONCACHE(_u4DspBuf+((addr)<<2)))))
#define dReadDspCommDram(addr)       (*((UINT32*)(VIRTUAL(_u4DspBuf+((addr)<<2)))))

//#define WriteDspDecDram(addr, val)   *((UINT32 *)(NONCACHE(_u4DspDramBuf[0]+ ((addr)<<2))) = val
#define WriteDspDecDram(addr, val)   do{                                                            \
                                     *((UINT32 *)(VIRTUAL(_u4DspDramBuf[0]+ ((addr)<<2))) = val;   \
                                     HalFlushDCache();                                              \
                                     }while(0)

#define wReadDspWORD(arg1)           (UINT16)(dReadDspCommDram(arg1) >> 8)
#define vWriteDspWORD(arg1,arg2)     WriteDspCommDram(arg1, (UINT32)((arg2) << 8))


// using cache area
//#define WriteDspDecCacheDram(addr, val) (*((UINT32 *)NONCACHE(_u4DspDramBuf[0] + ((addr)<<2)))) = val
#define WriteDspDecCacheDram(addr, val) do{                                                             \
                                        (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))) = val;  \
                                        HalFlushDCache();                                               \
                                        }while(0)

//#define ReadDspDecCacheDram(addr, data) data = (*((UINT32 *)NONCACHE(_u4DspDramBuf[0] + ((addr)<<2))))
#define ReadDspDecCacheDram(addr, data) do{                                                             \
                                        HalFlushInvalidateDCache();                                     \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2)))); \
                                        }while(0)

//#define DspDecCacheDramData(addr) (*((UINT32 *)NONCACHE(_u4DspDramBuf[0] + ((addr)<<2))))
#define DspDecCacheDramData(addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2))))

#define uReadDspShmBYTE(addr) (*((UINT8*)(_u4ShareMem+addr)))
#define u2ReadDspShmWORD(addr) (*((UINT16*)(_u4ShareMem+addr)))
#define u4ReadDspShmDWRD(addr) (*((UINT32*)(_u4ShareMem+addr)))

#define vWriteDspShmBYTE(addr,value) *((UINT8*)(_u4ShareMem + addr))=(UINT8)(value)
#define vWriteDspShmWORD(addr,value) *((UINT16*)(_u4ShareMem + addr))=(UINT16)(value)
#define vWriteDspShmDWRD(addr,value) *((UINT32*)(_u4ShareMem + addr))=(UINT32)(value)


#endif  // DSP_REG_H

