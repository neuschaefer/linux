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
 * $RCSfile: dsp_reg.h,v $
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
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "rc2d_int.h"
#include "d2rc_int.h"
#include "dsp_common.h"

//lint -e717 do ... while(0);

//-----------------------------------------------------------------------------
// Register Definition
//-----------------------------------------------------------------------------
#define REG_BUF_MIX_CFG                 (0x2c8)
#define REG_BUF_PNT                     (0x20b)
#define REG_BUF_PNT_DEC2                (0x20e)
 #ifndef CC_MT5391_AUD_SUPPORT
#ifndef CC_MT5360B
#define SRAM_IEC_FLAG                   (0x1550)
#define SRAM_IEC_FLAG_DEC2              (0x2DD0)    ///[Joel](0x2AD0)
#define SRAM_EFFECT_CONTROL             (0x150F)
#define SRAM_SYSTEM_CTRL                (0x1401)
#else  //5360B
#define SRAM_IEC_FLAG                        (0x9d0)
#define SRAM_IEC_FLAG_DEC2              (0x36d0)
#define SRAM_EFFECT_CONTROL            (0x98f)
#define SRAM_SYSTEM_CTRL                  (0x801)
#endif
#else
#define SRAM_IEC_FLAG                   (0x9d0)
#define SRAM_IEC_FLAG_DEC2              (0x36d0)
#define SRAM_EFFECT_CONTROL             (0x98f)
#define SRAM_SYSTEM_CTRL                (0x801)
#endif
#define LOG_DRAM_REFRESH                (0x1U<<4)

//-----------------------------------------------------------------------------
// AUDIO/DSP Register in target
//-----------------------------------------------------------------------------
#define AUD_REG_OFST                   (0x00)
#define ADSP_REG_OFST                    (0x40)

//#define REG_ACK_CFG                     (AUD_REG_OFST + (0x0000>>2))
//#define REG_EXTDAC_CFG0                 (AUD_REG_OFST + (0x0008>>2))
//#define REG_EXTDAC_CFG1                 (AUD_REG_OFST + (0x000C>>2))
//#define REG_EXTADC_CFG0                 (AUD_REG_OFST + (0x0010>>2))

#define REG_SPLIN_CTL                   (AUD_REG_OFST + (0x0018>>2))
    #define SPLIN_ENABLE                (0x01)
    #define SPLIN_24BIT                 (0x01 << 1)
    #define SPLIN_RAW_MSB16BIT          (0x01 << 2)
    #define SPLIN_SWAP_BYTES            (0x01 << 3)
    
    #define SPLIN_CHNUM_2CH             (0x00 << 4)
    #define SPLIN_CHNUM_4CH             (0x01 << 4)
    #define SPLIN_CHNUM_6CH             (0x02 << 4)
    #define SPLIN_CHNUM_8CH             (0x03 << 4)
    
    #define SPLIN_INT_PERIOD_MASK       (0x03 << 6)
    #define SPLIN_INT_PERIOD_032DWRD    (0x00 << 6)
    #define SPLIN_INT_PERIOD_064DWRD    (0x01 << 6)
    #define SPLIN_INT_PERIOD_128DWRD    (0x02 << 6)
    #define SPLIN_INT_PERIOD_256DWRD    (0x03 << 6)

    #define SPLIN_PSEUDO_BS             (0x01 << 8)

#define REG_SPLIN_BLK                   (AUD_REG_OFST + (0x0020>>2))
#define REG_SPLIN_TYPE                  (AUD_REG_OFST + (0x0028>>2))
//#define REG_SPLIN_FS                    (AUD_REG_OFST + (0x002C>>2))

#define REG_SPOUT_CFG                   (AUD_REG_OFST + (0x0030>>2))
    #define SPOUT_BASIC_MASK            (0x000001ff)
    #define SPOUT_BIT_NUM_POS          (0)                  // Bit[1:0]
    #define SPOUT_BIT_NUM_24            (0x0)
    #define SPOUT_BIT_NUM_20            (0x2)
    #define SPOUT_BIT_NUM_16            (0x3)
    #define SPOUT_CHSEL_POS             (2)                 // Bit[4:2]
    #define SPOUT_MUTE_POS               (5)                 // Bit[5]
    #define SPOUT_MUTE                   (0x01 << SPOUT_MUTE_POS)
    #define SPOUT_DOWNSAMPLE_POS        (6)                 // Bit[7:6]        
    #define SPOUT_DOWNSAMPLE_MASK       (0x03 << SPOUT_DOWNSAMPLE_POS)
    #define SPOUT_DOWNSAMPLE_0          (0x0)
    #define SPOUT_DOWNSAMPLE_2          (0x1)
    #define SPOUT_DOWNSAMPLE_4          (0x3)
    #define SPOUT_SPDF_SEL_POS          (8)
    #define SPOUT_SPDF_SEL_MASK         (0x01 << SPOUT_SPDF_SEL_POS)
    #define SPOUT_SPDF_SEL_MAIN         (0)
    #define SPOUT_SPDF_SEL_AUX          (1)


//#define REG_PERI_INT                    (AUD_REG_OFST + (0x0038>>2))
//    #define SPLIN_INT_PERIOD_DISABLE    (0x00)
//    #define SPLIN_INT_PERIOD_ENABLE     (0x01)

#define RW_BS_CFG                       (AUD_REG_OFST + (0x0040>>2))
    #define SPLIN_WRITEPTR_AS_AUDPTR0   (0x01)
    #define SPLIN_WRITEPTR_AS_AUDPTR1   (0x01 << 2)
    #define GBH_DI                      (0x01 << 8)
    #define NO_HOLD_GBS                 (0x01 << 7)

//#define REG_ABUF_PNT                    (AUD_REG_OFST + (0x0044>>2))
//#define REG_ABUF_PNT_DEC2               (AUD_REG_OFST + (0x0048>>2))
#ifdef DSP_TEST_PUTBITS  // Putbits test
#define RW_DMX_WRITE_PNT               (AUD_REG_OFST + (0x004C>>2))
#define RW_DMX_START                   (AUD_REG_OFST + (0x0050>>2))
#define RW_DMX_END                     (AUD_REG_OFST + (0x0054>>2))
#define REG_PUTBS_BLK0_RISC             (AUD_REG_OFST + (0x0078>>2))
#define REG_PUTBS_BLK1_RISC             (AUD_REG_OFST + (0x007C>>2))
#endif

#define REG_ADSP_CFG                    (AUD_REG_OFST + (0x0080>>2))
    #define ADSP_CFG_ICACHE_FLUSH       (0x01 << 2)
    #define ADSP_CFG_RST_B              (0x01 << 4)
    #define ADSP_CFG_CLK_ENA            (0x01 << 5)
    #define ADSP_BOOT_FROM_RAM          (0x01 << 6)
    #define ADSP_CACHE_SIZE_POS         (10)                    // Bit[12:10]
    #define ADSP_CACHE_SIZE_MASK        (0x07 << ADSP_CACHE_SIZE_POS)
#ifdef CC_MT5391_AUD_SUPPORT
        #define CACHE_SIZE_2K           (0x01) //modify by ling
#else
        #define CACHE_SIZE_256          (0x01)
#endif
        #define CACHE_SIZE_1K           (0x02)
        #define CACHE_SIZE_512          (0x04)
    #define ADSP_CACHE_INST_SIZE_POS    (16)                    // Bit[17:16]
    #define ADSP_CACHE_INST_SIZE_MASK   (0x03 << ADSP_CACHE_INST_SIZE_POS)
        #define CBUF_SIZE_16X128               (0x0)
        #define CBUF_SIZE_8X128               (0x01)
        #define CBUF_SIZE_4X128               (0x02)
   #define ADSP_CMPTRD_THRES_POS        (18)                    // Bit[19:18]
   #define ADSP_CMPTRD_THRES_MASK       (0x03 << ADSP_CMPTRD_THRES_POS)
       #define THRES_UNFULL             (0x0)
       #define THRES_HALF               (0x01)
       #define THRES_3_4                (0x02)
   #define ADSP_AUDIO_BLOCK_POS         (20)
   #define ADSP_AUDIO_BLOCK_MASK        (0x0F << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_0_16MB        (0x0 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_16_32MB       (0x01 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_32_48MB       (0x02 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_48_64MB       (0x03 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_64_80MB       (0x04 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_80_96MB       (0x05 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_96_112MB      (0x06 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_112_128MB     (0x07 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_128_144MB     (0x08 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_144_160MB     (0x09 << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_160_176MB     (0x0A << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_176_192MB     (0x0B << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_192_208MB     (0x0C << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_208_224MB     (0x0D << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_224_240MB     (0x0E << ADSP_AUDIO_BLOCK_POS)
       #define DRAM_BLOCK_240_256MB     (0x0F << ADSP_AUDIO_BLOCK_POS)
   #define ADSP_AUD_DRAM_CLK_OFF        (0x01 << 24)       // Bit[24]
   #define ADSP_DRAM_TIMEOUT_RESET      (0x01 << 28)
   #define ADSP_DRAM_TIMEOUT_FLUSH      (0x01 << 29)

#define ADSP_REG_OFST_DDSP          (0x2c0 >> 2)  //(0xb0)
//#define ADSP_REG_OFST                    (0x >> 2)  //(0x1b0)
#define AUD_DSPB_REG_OFST            (0x0400>>2)

#define REG_ADSP_CFG_DSPB                    (AUD_REG_OFST + AUD_DSPB_REG_OFST +(0x0080>>2))
#define REG_SD_ADR                      (AUD_REG_OFST + (0x0084>>2))
#define REG_SD_ADR_INTR                 (AUD_REG_OFST + (0x0088>>2))
#define REG_LD                          (AUD_REG_OFST + (0x008C>>2))

#define REG_R2D_STA                     (AUD_REG_OFST + (0x0090>>2))
    #define  DSP_B                      (0x01)

#define REG_R2D_RD                      (AUD_REG_OFST + (0x0094>>2))

#define REG_D2R_STA                     (AUD_REG_OFST + (0x0098>>2))
    #define  DREQ                       (0x01)
    #define  D2RDAV                     (0x01 << 1)

#define REG_D2R_SDA                     (AUD_REG_OFST + (0x009C>>2))
#define REG_D2R_RD                      (AUD_REG_OFST + (0x00A0>>2))
#define REG_DSP_DRAM_SBLK01             (AUD_REG_OFST + (0x00B0>>2))
#define REG_DSP_DRAM_SBLK23             (AUD_REG_OFST + (0x00B4>>2))
#define REG_DSP_CMPT_DRAM_SBLK01        (AUD_REG_OFST + (0x00C0>>2))
#define REG_DSP_CMPT_DRAM_SBLK23        (AUD_REG_OFST + (0x00C4>>2))
#define REG_DSP_IC_DRAM_SBLK01          (AUD_REG_OFST + (0x00C8>>2))
#define REG_DSP_IC_DRAM_SBLK23          (AUD_REG_OFST + (0x00CC>>2))
#define REG_DSP_IC_DRAM_SBLK45          (AUD_REG_OFST + (0x00D0>>2))
#ifdef CC_MT5391_AUD_SUPPORT
#define REG_DSP_IC_DRAM_SBLK6          (AUD_REG_OFST + (0x024C>>2))
#endif

//#define REG_DRAMWR_BLK                  (AUD_REG_OFST + (0x00DC>>2))
//#define REG_APC_LHCNT                   (AUD_REG_OFST + (0x00E0>>2))
//#define REG_APC_LMCNT                   (AUD_REG_OFST + (0x00E4>>2))
//#define REG_APC_WCYC                    (AUD_REG_OFST + (0x00E8>>2))
//#define REG_APC_LWCYC                   (AUD_REG_OFST + (0x00EC>>2))
//#define REG_APC_TCYC                    (AUD_REG_OFST + (0x00F0>>2))
//#define REG_CH_CFG                      (AUD_REG_OFST + (0x0200>>2))
//#define REG_I_DAC_CH_CFG                (AUD_REG_OFST + (0x0204>>2))

//#define REG_AOUT_DRAM_CFG               (AUD_REG_OFST + (0x0210>>2))
//    #define AOUT_WAIT_CYCLE_16          (0x0 << 0)
//    #define AOUT_WAIT_CYCLE_32          (0x01 << 0)
//    #define AOUT_WAIT_CYCLE_64          (0x02 << 0)
//    #define AOUT_WAIT_CYCLE_128         (0x03 << 0)
    
//    #define AOUT_WAIT_PERIOD_NO         (0x0 << 2)
//    #define AOUT_WAIT_PERIOD_1CH        (0x01 << 2)
//    #define AOUT_WAIT_PERIOD_2CH        (0x02 << 2)
//    #define AOUT_WAIT_PERIOD_4CH        (0x03 << 2)
    
//    #define AOUT_32BIT_MODE             (0x0 << 4)
//    #define AOUT_128BIT_MODE            (0x01 << 4)

//-----------------------------------------------------------------------------
// Ain Module Register Map
//-----------------------------------------------------------------------------

#ifndef CC_MT5391_AUD_SUPPORT
#define REG_AUDIN_CFG                           (AUD_REG_OFST + (0x0220>>2))
#define REG_AUDIN_CFG_MASK                      (0x097003F1F)
    #define APGA_GAIN_POS                       (0)                 // Bit[4:0]
    #define APGA_GAIN_MASK                      (0x1F << APGA_GAIN_POS)
        #define APGA_GAIN_0DB                   (0x00 << APGA_GAIN_POS)
    #define APGA_OFFSET_POS                     (8)                  // Bit[13:8]
    #define APGA_OFFSET_MASK                    (0x3F << APGA_OFFSET_POS)
        #define APGA_OFFSET_0V                  (0x00 << APGA_OFFSET_POS)
    #define APGA_GAIN_BY_DSP                (0x00 << 24)
    #define APGA_GAIN_BY_RISC               (0x01 << 24)
    #define ADC_SIF_CLR_DSP                 (0x00 << 25)
    #define ADC_SIF_CLR_RISC                (0x01 << 25)
    #define AUCLR_OFF                           (0x00 << 26)
    #define AUCLR_ON                            (0x01 << 26)
    #define ADCK_INV_RISING                     (0x00 << 28)
    #define ADCK_INV_FALLING                    (0x01 << 28)
    #define ADC_SEL_SIF                         (0x00 << 31)
    #define ADC_SEL_DEMOD                       (0x01U << 31)

#define REG_ATVCFG0                             (AUD_REG_OFST + (0x0228>>2))
#define REG_ATVCFG0_MASK                        (0x7771F3FF)
    #define RG_SIF_SEL_POS               (28)          // Bit[8:0]
    #define RG_SIF_SEL_MASK              (0x3 << RG_PGA_SIF_PWD_POS)
        #define SIF_SEL_OFF                             (0x00 << RG_SIF_SEL_POS)    // default
        #define SIF_SEL_AF                               (0x01 << RG_SIF_SEL_POS)
        #define SIF_SEL_SIF                              (0x02 << RG_SIF_SEL_POS)
    #define RG_PGA_SIF_PWD_POS               (0)          // Bit[8:0]
    #define RG_PGA_SIF_PWD_MASK              (0x17F << RG_PGA_SIF_PWD_POS)
        #define RG_PGA_SIF_PWD_ON                         (0x080 << RG_PGA_SIF_PWD_POS)
        #define RG_PGA_SIF_PWD_DOWN                       (0x1FF << RG_PGA_SIF_PWD_POS)    // default

#define REG_ATVCFG1                             (AUD_REG_OFST + (0x022C>>2))
#define REG_ATVCFG1_MASK                        (0xFF333F73)
    #define RG_SIF_CHSEL_POS               (20)          // Bit[21:20]
    #define RG_SIF_CHSEL_MASK              (0x3 << RG_SIF_CHSEL_POS)
        #define RG_SIF_CHSEL_TEST                         (0x00 << RG_SIF_CHSEL_POS)
        #define RG_SIF_CHSEL_SIF                         (0x01 << RG_SIF_CHSEL_POS)    // default
        #define RG_SIF_CHSEL_T_IN                         (0x10 << RG_SIF_CHSEL_POS)
        #define RG_SIF_CHSEL_NA                         (0x11 << RG_SIF_CHSEL_POS)
    #define RG_SIF_ADC_PWD_POS               (17)          // Bit[17]
    #define RG_SIF_ADC_PWD_MASK              (0x1<< RG_SIF_ADC_PWD_POS)
        #define RG_SIF_ADC_PWD_ON                         (0x00 << RG_SIF_ADC_PWD_POS)
        #define RG_SIF_ADC_PWD_DOWN                       (0x1 << RG_SIF_ADC_PWD_POS)    // default
    #define RG_SIF_SIF_EN_POS               (16)          // Bit[17]
    #define RG_SIF_SIF_EN_MASK              (0x1<< RG_SIF_SIF_EN_POS)
        #define RG_SIF_SIF_EN_ON                         (0x01 << RG_SIF_SIF_EN_POS)     // default
        #define RG_SIF_SIF_EN_DOWN                       (0x0 << RG_SIF_SIF_EN_POS)   
    #define RG_BIAS_SIF_PWD_POS               (6)          // Bit[6]
    #define RG_BIAS_SIF_PWD_MASK              (0x1<< RG_BIAS_SIF_PWD_POS)
        #define RG_BIAS_SIF_PWD_ON                         (0x00 << RG_BIAS_SIF_PWD_POS)
        #define RG_BIAS_SIF_PWD_DOWN                       (0x1 << RG_BIAS_SIF_PWD_POS)    // default
   #else
    #define REG_AUDIN_CFG                           (AUD_REG_OFST + (0x0220>>2))
    #define REG_AUDIN_CFG_MASK                      (0x090000000)

	 #define ADC_SEL_CVBS                         (0x00 << 31)
	 #define ADC_SEL_DEMOD                       (0x01U << 31)
	 #define ADCK_RISING_EDGE                   (0x00 << 28)
       #define ADCK_FALLING_EDGE                 (0x01 << 28)	   
   #endif
		

//-----------------------------------------------------------------------------
// Line ADC Module Register Map
//-----------------------------------------------------------------------------
#define REG_SDADC0                              (AUD_REG_OFST + (0x0240>>2))
    #define SDADC_RST_POS                       (0)                 // Bit[0]
    #define SDACC_RST_MASK                      (0x01 << SDADC_RST_POS)
        #define SDADC_RST_NORMAL                (0x00 << SDADC_RST_POS)
        #define SDADC_RST_RESET                 (0x01 << SDADC_RST_POS)
    #define SDADC_APLL_SEL_POS                  (1)                 // Bit[1]
    #define SDADC_APLL_SEL_MASK                 (0x01 << SDADC_APLL_SEL_POS)
        #define SDADC_APLL_SEL_APLL0            (0x00 << SDADC_APLL_SEL_POS)
        #define SDADC_APLL_SEL_APLL1            (0x01 << SDADC_APLL_SEL_POS)
    #define SDADC_CLK1_DIV_POS                  (2)                 // Bit[3:2]
    #define SDADC_CLK1_DIV_MASK                 (0x03 << SDADC_CLK1_DIV_POS)
        #define SDADC_CLK1_DIV_1                (0x00 << SDADC_CLK1_DIV_POS)
        #define SDADC_CLK1_DIV_2                (0x01 << SDADC_CLK1_DIV_POS)
        #define SDADC_CLK1_DIV_4                (0x02 << SDADC_CLK1_DIV_POS)
        #define SDADC_CLK1_DIV_8                (0x03 << SDADC_CLK1_DIV_POS)
    #define SDADC_CLK2_DIV_POS                  (4)                 // Bit[5:4]
    #define SDADC_CLK2_DIV_MASK                 (0x03 << SDADC_CLK2_DIV_POS)
        #define SDADC_CLK2_DIV_1                (0x00 << SDADC_CLK2_DIV_POS)
        #define SDADC_CLK2_DIV_2                (0x01 << SDADC_CLK2_DIV_POS)
        #define SDADC_CLK2_DIV_4                (0x02 << SDADC_CLK2_DIV_POS)
        #define SDADC_CLK2_DIV_8                (0x03 << SDADC_CLK2_DIV_POS)
    #define SDADC_CLK1_INV_POS                  (6)                 // Bit[6]
    #define SDADC_CLK1_INV_MASK                 (0x01 << SDADC_CLK1_INV_POS)
        #define SDADC_CLK1_INV_DIS              (0x00 << SDADC_CLK1_INV_POS)
        #define SDADC_CLK1_INV_EN               (0x01 << SDADC_CLK1_INV_POS)
    #define SDADC_CLK2_INV_POS                  (7)                 // Bit[7]
    #define SDADC_CLK2_INV_MASK                 (0x01 << SDADC_CLK2_INV_POS)
        #define SDADC_CLK2_INV_DIS              (0x00 << SDADC_CLK2_INV_POS)
        #define SDADC_CLK2_INV_EN               (0x01 << SDADC_CLK2_INV_POS) 
    #define SDADC_OSR_POS                       (19)                // Bit[19]
    #define SDADC_OSR_MASK                      (0x01 << SDADC_OSR_POS)
        #define SDADC_OSR_128FS                 (0x00 << SDADC_OSR_POS)
        #define SDADC_OSR_64FS                  (0x01 << SDADC_OSR_POS)
    #define SDADC_CHPD_POS                      (20)                // Bit[20]
    #define SDADC_CHPD_MASK                     (0x01 << SDADC_CHPD_POS)
        #define SDADC_CHPD_HPF_EN               (0x00 << SDADC_CHPD_POS)
        #define SDADC_CHPD_HPF_DIS              (0x01 << SDADC_CHPD_POS)
    #define SDADC_FILTER_POS                    (24)                // Bit[24]
    #define SDADC_FILTER_MASK                   (0x01 << SDADC_FILTER_POS)
        #define SDADC_FILTER_DIS                (0x00 << SDADC_FILTER_POS)
        #define SDADC_FILTER_EN                 (0x01 << SDADC_FILTER_POS)
    #define SDADC_TEST_EN_POS                   (31)                // Bit[31]
    #define SDADC_TEST_EN_MASK                  (0x01 << SDADC_TEST_EN_POS)
        #define SDADC_TEST_EN_ADC               (0x00 << SDADC_TEST_EN_POS)
        #define SDADC_TEST_EN_PAD               (0x01 << SDADC_TEST_EN_POS)

#define REG_SDADC1                              (AUD_REG_OFST + (0x0244>>2)) 
    #define SDADC_FACTOR_POS                    (0)                 // Bit[25:0]
    #define SDADC_FACTOR_MASK                   (0x03FFFFFF << SDADC_FACTOR_POS)
    #define SDADC_SHIFT_POS                     (28)                // Bit[30:28]
    #define SDADC_SHIFT_MASK                    (0x03 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_HALF                (0x00 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_NO                  (0x01 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_1L                  (0x02 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_2L                  (0x03 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_3L                  (0x04 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_4L                  (0x05 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_5L                  (0x06 << SDADC_SHIFT_POS)
        #define SDADC_SHIFT_6L                  (0x07 << SDADC_SHIFT_POS)

#define REG_SDADC2                              (AUD_REG_OFST + (0x0248>>2))
    #define SDADC_ADCOSR_POS                    (0)                 // Bit[0]
    #define SDADC_ADCOSR_MASK                   (0x01 << SDADC_ADCOSR_POS)
        #define SDADC_ADCOSR_48K                (0x00 << SDADC_ADCOSR_POS)
        #define SDADC_ADCOSR_96K                (0x01 << SDADC_ADCOSR_POS)
    #define SDADC_CK256EDGE_POS                 (1)                 // Bit[1]
    #define SDADC_CK256EDGE_MASK                (0x01 << SDADC_CK256EDGE_POS)
        #define SDADC_CK256EDGE_FALLING         (0x00 << SDADC_CK256EDGE_POS)
        #define SDADC_CK256EDGE_RISING          (0x01 << SDADC_CK256EDGE_POS)
    #define SDADC_CKDLY_POS                     (2)                 // Bit[3:2]
    #define SDADC_CKDLY_MASK                    (0x03 << SDADC_CKDLY_POS)
        #define SDADC_CKDLY_NO                  (0x00 << SDADC_CKDLY_POS)
        #define SDADC_CKDLY_8N                  (0x01 << SDADC_CKDLY_POS)
        #define SDADC_CKDLY_16N                 (0x02 << SDADC_CKDLY_POS)
        #define SDADC_CKDLY_MAX                 (0x03 << SDADC_CKDLY_POS)
    #define SDADC_ENCHOP_POS                    (4)                 // Bit[4]
    #define SDADC_ENCHOP_MASK                   (0x01 << SDADC_ENCHOP_POS)
        #define SDADC_ENCHOP_DIS                (0x00 << SDADC_ENCHOP_POS)
        #define SDADC_ENCHOP_EN                 (0x01 << SDADC_ENCHOP_POS)
    #define SDADC_ENDWA_POS                     (5)                 // Bit[5]  
    #define SDADC_ENDWA_MASK                    (0x01 << SDADC_ENDWA_POS)
        #define SDADC_ENDWA_DIS                 (0x00 << SDADC_ENDWA_POS)
        #define SDADC_ENDWA_EN                  (0x01 << SDADC_ENDWA_POS)
    #define SDADC_ENVRN_POS                     (6)                 // Bit[6]
    #define SDADC_ENVRN_MASK                    (0x01 << SDADC_ENVRN_POS)
        #define SDADC_ENVRN_VSS                 (0x00 << SDADC_ENVRN_POS)
        #define SDADC_ENVRN_INTREF              (0x01 << SDADC_ENVRN_POS)
    #define SDADC_ENVRP_POS                     (7)                 // Bit[7]
    #define SDADC_ENVRP_MASK                    (0x01 << SDADC_ENVRP_POS)
        #define SDADC_ENVRP_VDD                 (0x00 << SDADC_ENVRP_POS)
        #define SDADC_ENVRP_INTREF              (0x01 << SDADC_ENVRP_POS)
    #define SDADC_GAIN_POS                      (8)                 // Bit[10:8]
    #define SDADC_GAIN_MASK                     (0x07 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_18DB               (0x00 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_16_5DB             (0x01 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_15DB               (0x02 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_13_5DB             (0x03 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_12DB               (0x04 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_10_5DB             (0x05 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_9DB                (0x06 << SDADC_GAIN_POS)
        #define SDADC_GAIN_N_7_5DB              (0x07 << SDADC_GAIN_POS)
    #define SDADC_LEFT_PD_POS                   (11)                // Bit[11]
    #define SDADC_LEFT_PD_MASK                  (0x01 << SDADC_LEFT_PD_POS)
        #define SDADC_LEFT_PD_POWER_ON          (0x00 << SDADC_LEFT_PD_POS)
        #define SDADC_LEFT_PD_POWER_DOWN        (0x01 << SDADC_LEFT_PD_POS)
    #define SDADC_LPF_BIAS_POS                  (12)                // Bit[12]
    #define SDADC_LPF_BIAS_MASK                 (0x01 << SDADC_LPF_BIAS_POS)
        #define SDADC_LPF_BIAS_7_6U             (0x00 << SDADC_LPF_BIAS_POS)
        #define SDADC_LPF_BIAS_20U              (0x01 << SDADC_LPF_BIAS_POS)
    #define SDADC_MUTE_LFT_POS                  (13)                // Bit[13]
    #define SDADC_MUTE_LFT_MASK                 (0x01 << SDADC_MUTE_LFT_POS)
        #define SDADC_MUTE_LFT_DIS              (0x00 << SDADC_MUTE_LFT_POS)
        #define SDADC_MUTE_LFT_EN               (0x01 << SDADC_MUTE_LFT_POS)
    #define SDADC_MUTE_RHT_POS                  (14)                // Bit[14]
    #define SDADC_MUTE_RHT_MASK                 (0x01 << SDADC_MUTE_RHT_POS)
        #define SDADC_MUTE_RHT_DIS              (0x00 << SDADC_MUTE_RHT_POS)
        #define SDADC_MUTE_RHT_EN               (0x01 << SDADC_MUTE_RHT_POS) 
    #define SDADC_OPBIAS_POS                    (15)                // Bit[16:15]
    #define SDADC_OPBIAS_MASK                   (0x03 << SDADC_OPBIAS_POS)
        #define SDADC_OPBIAS_D_1_4              (0x00 << SDADC_OPBIAS_POS)
        #define SDADC_OPBIAS_D_1_2              (0x00 << SDADC_OPBIAS_POS)
        #define SDADC_OPBIAS_1                  (0x00 << SDADC_OPBIAS_POS)
        #define SDADC_OPBIAS_M_1_2              (0x00 << SDADC_OPBIAS_POS)
    #define SDADC_RIGHT_PD_POS                  (17)                // Bit[17]
    #define SDADC_RIGHT_PD_MASK                 (0x01 << SDADC_RIGHT_PD_POS)
        #define SDADC_RIGHT_PD_POWER_ON         (0x00 << SDADC_RIGHT_PD_POS)
        #define SDADC_RIGHT_PD_POWER_DOWN       (0x01 << SDADC_RIGHT_PD_POS)
    #define SDADC_SELCHPCK_POS                  (18)                // Bit[18]
    #define SDADC_SELCHPCK_MASK                 (0x01 << SDADC_SELCHPCK_POS)
        #define SDADC_SELCHPCK_8FS              (0x00 << SDADC_SELCHPCK_POS)
        #define SDADC_SELCHPCK_4FS              (0x01 << SDADC_SELCHPCK_POS)
    #define SDADC_SELCK_POS                     (19)                // Bit[19]
    #define SDADC_SELCK_MASK                    (0x01 << SDADC_SELCK_POS)
        #define SDADC_SELCK_256FS               (0x00 << SDADC_SELCK_POS)
        #define SDADC_SELCK_EXT                 (0x01 << SDADC_SELCK_POS)
    #define SDADC_SELDLY_POS                    (20)                // Bit[20]
    #define SDADC_SELDLY_MASK                   (0x01 << SDADC_SELDLY_POS)
        #define SDADC_SELDLY_3_9N               (0x00 << SDADC_SELDLY_POS)
        #define SDADC_SELDLY_4_85N              (0x01 << SDADC_SELDLY_POS)
    #define SDADC_SELPDCON_POS                  (21)                // Bit[21]
    #define SDADC_SELPDCON_MASK                 (0x01 << SDADC_SELPDCON_POS)
        #define SDADC_SELPDCON_ADC              (0x00 << SDADC_SELPDCON_POS)
        #define SDADC_SELPDCON_PD               (0x01 << SDADC_SELPDCON_POS)
    #define SDADC_ADCINSEL_POS                  (23)                // Bit[25:23]
    #define SDADC_ADCINSEL_MASK                 (0x03 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_0                (0x00 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_1                (0x01 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_2                (0x02 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_3                (0x03 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_4                (0x04 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_5                (0x05 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_6                (0x06 << SDADC_ADCINSEL_POS)
        #define SDADC_ADCINSEL_7                (0x07 << SDADC_ADCINSEL_POS)
    #define SDADC_ADC_L_PD_POS                  (26)                // Bit[26]
    #define SDADC_ADC_L_PD_MASK                 (0x01 << SDADC_ADC_L_PD_POS)
        #define SDADC_ADC_L_PD_POWER_ON         (0x00 << SDADC_ADC_L_PD_POS)
        #define SDADC_ADC_L_PD_POWER_DOWN       (0x01 << SDADC_ADC_L_PD_POS)
    #define SDADC_ADC_R_PD_POS                  (27)                // Bit[27]
    #define SDADC_ADC_R_PD_MASK                 (0x01 << SDADC_ADC_R_PD_POS)
        #define SDADC_ADC_R_PD_POWER_ON         (0x00 << SDADC_ADC_R_PD_POS)
        #define SDADC_ADC_R_PD_POWER_DOWN       (0x01 << SDADC_ADC_R_PD_POS)        
        
#define REG_SDADC3                              (AUD_REG_OFST + (0x024C>>2))
    #define SDADC_AADC_RES_POS                  (0)                 // Bit[15:0]
    #define SDADC_AADC_RES_MASK                 (0x0FFFF << SDADC_AADC_RES_POS)
        #define SDADC_AADC_RES_BIT0_CLR         (0x00)
        #define SDADC_AADC_RES_BIT0_SET         (0x01)
        #define SDADC_AADC_RES_BIT1_CLR         (0x00 << 1)
        #define SDADC_AADC_RES_BIT1_SET         (0x01 << 1)

#define VDOIN_WRITE32(offset, value)  IO_WRITE32(VIDEO_IN0_BASE, (offset), (value))
#define VFE_0A                                  0x4a8
//-----------------------------------------------------------------------------
// ADSP RISC Register Map
//-----------------------------------------------------------------------------
// DSP Control Reg (5100h)
#define RW_ADSPCR                       (ADSP_REG_OFST)
    #define  PROM_WR                    ((UINT32)0x01 << 1)
    #define  PRAM_WR                    ((UINT32)0x01 << 2)
    #define  DROM_WR                    ((UINT32)0x01 << 3)
    #define  HOT_DNLD                   ((UINT32)0x01 << 6)    
    #define  PMEM_AUTO_INC        ((UINT32)0x01 << 10) 
// PROM Download Reg (5104h)
#define RW_PROM_DNLD                    (ADSP_REG_OFST + (0x0004 >> 2))
// PRAM Download Reg (5108h)
#define RW_PRAM_DNLD                    (ADSP_REG_OFST + (0x0008 >> 2))
// Counter Control Reg (FPGA only)(510Ch)
#define WO_CNTR_CLR                     (ADSP_REG_OFST + (0x000C >> 2))
    #define  CNTR_CLR                   (0x01 << 1)
// PRAM Addr Reg (5110h)
#define RW_DNLD_CNTR                    (ADSP_REG_OFST + (0x0010 >> 2))
// DROM Download Reg (FPGA only) (5158h)
#define RW_DROM_DNLD              (ADSP_REG_OFST + (0x0058 >> 2))

//-----------------------------------------------------------------------------
// Register Access Macro
//-----------------------------------------------------------------------------

#ifndef DSP_REG_MARCO
#undef WriteREG
#undef ReadREG

#define WriteREG(addr,value)   IO_WRITE32(AUDIO_BASE,(addr<<2),value)
#define ReadREG(addr)          	 IO_READ32(AUDIO_BASE,(addr<<2))
#endif

#define SendDSPInt(arg1, arg2) WriteREG(REG_SD_ADR_INTR, \
                                 (((UINT32) arg1) + (((UINT32) arg2) << 8)))
#define SendDSPLInt(arg1, arg2) WriteREG(REG_SD_ADR_INTR, \
                                  (((UINT32) arg1) + (((UINT32) arg2) << 16)))

//-----------------------------------------------------------------------------
// Memory Access Macro
//-----------------------------------------------------------------------------
//#define WriteDspCommDram(addr, val)   (*((UINT32*)(NONCACHE(_u4DspBuf+((addr)<<2)))) = (val))     //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
//lint -emacro({717}, WriteDspCommDram)  to disable "do {...} while (0)" lint warning
#define WriteDspCommDram(addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf+((addr)<<2)))) = (val);   \
                                      HalFlushDCache();                                         \
                                      }while(0)

//#define dReadDspCommDram(addr)       (*((UINT32*)(NONCACHE(_u4DspBuf+((addr)<<2)))))
#define dReadDspCommDram(addr)       (*((UINT32*)(VIRTUAL(_u4DspBuf+((addr)<<2)))))
#define dReadDspCommDramAddr(addr)  (VIRTUAL(_u4DspBuf+((addr)<<2)))

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

#define ReadDspDec2CacheDram(addr, data) do{                                                             \
                                        HalFlushInvalidateDCache();                                     \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[1] + ((addr)<<2)))); \
                                        }while(0)
                                        
//#define DspDecCacheDramData(addr) (*((UINT32 *)NONCACHE(_u4DspDramBuf[0] + ((addr)<<2))))
#define DspDecCacheDramData(addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[0] + ((addr)<<2))))

#define uReadDspShmBYTE(addr) (*((UINT8*)(_u4ShareMem+addr)))
#define u2ReadDspShmWORD(addr) (*((UINT16*)(_u4ShareMem+addr)))
#define u4ReadDspShmDWRD(addr) (*((UINT32*)(_u4ShareMem+addr)))

#define vWriteDspShmBYTE(addr,value) (*((UINT8*)(_u4ShareMem + addr)))=(UINT8)(value);
#define vWriteDspShmWORD(addr,value) (*((UINT16*)(_u4ShareMem + addr))) = (UINT16)(value);
#define vWriteDspShmDWRD(addr,value) (*((UINT32*)(_u4ShareMem + addr))) = (UINT32)(value);


#endif  // DSP_REG_H

