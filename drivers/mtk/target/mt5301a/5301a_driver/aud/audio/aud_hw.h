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
 * $RCSfile: aud_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_hw.h
 *  Define the audio environment registers and register access macro
 */

#ifndef _AUD_HW_H_
#define _AUD_HW_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

      #define MCLK0                   (0)
      #define MCLK1                   (1)

#define REG_ACK_CFG             0x0000
  #define BCK_DIV_MASK          (0xf  << 0)
    #define BCK_DIV_8                   (0x4  << 0) //512fs/2*8
    #define BCK_DIV_6                   (0x3  << 0) //384fs/2*6
    #define BCK_DIV_4                   (0x2  << 0) //256fs/2*4     default=32fs
    #define BCK_DIV_2                   (0x1  << 0) //128fs/2*2
  #define DEC2_BCK_DIV_MASK     (0xf  << 16)
    #define DEC2_BCK_DIV_8          (0x4  << 16)    //512fs/2*8
    #define DEC2_BCK_DIV_6          (0x3  << 16)    //384fs/2*6
    #define DEC2_BCK_DIV_4          (0x2  << 16)    //256fs/2*4     default=32fs    
    #define DEC2_BCK_DIV_2          (0x1  << 16)    //128fs/2*2
  #define IEC_DIV_MASK          (0x7  << 4)
    #define IEC_DIV_4                   (0x4  << 4) 
    #define IEC_DIV_2                   (0x2  << 4) 
    #define IEC_DIV_1                   (0x1  << 4) 
  #define DEC2_IEC_DIV_MASK     (0x7  << 20)
    #define DEC2_IEC_DIV_4          (0x4  << 20)    
    #define DEC2_IEC_DIV_2          (0x2  << 20)    
    #define DEC2_IEC_DIV_1          (0x1  << 20)
  #define DEC0_MTRI                 (0x1  << 8)
  #define DEC1_MTRI                 (0x1  << 24)
  #define LIN_MS                        (0x1  <<  9)  
#define APLL_SEL_MASK           (0x04000400  << 0)
  #define CLK_SEL_APLL0             (0x0  << 10)
  #define CLK_SEL_APLL1             (0x1  << 10)
  #define DEC2_CLK_SEL_APLL0        (0x0  << 26)
  #define DEC2_CLK_SEL_APLL1        (0x1  << 26)

#define REG_EXTDAC_CFG0         0x0008
#define REG_EXTDAC_CFG1         0x000c
#define AUD_OUT_CFG_MASK        (0x3fff)
  #define LRCK_CYC_MASK             (0x3 << 0)
    #define LRCK_CYC16                      (0x0  << 0)      // 16 cycles
    #define LRCK_CYC24                      (0x1  << 0)      // 24 cycles
    #define LRCK_CYC32                      (0x2  << 0)      // 32 cycles
  #define AOFMT_MASK                (0x0 << 2)              
    #define AOFMT_RJ                    (0x0  << 2)         // Right aligned with LRCK
    #define AOFMT_LJ                    (0x2  << 2)         // Left aligned with LRCK
    #define AOFMT_I2S                       (0x3  << 2)         // I2S interface
  #define INV_BCK                       (0x1  << 4)         // Invert bit clock
  #define INV_LRCK                      (0x1  << 5)         // Invert LRCK
  #define INV_SDATA             (0x1  << 6)     // Invert SDATA
  #define DAC_BNUM_MASK             (0x3F << 8)    // Audio DAC bit number
    #define DAC_16BIT                       (0x10 << 8)    // Audio DAC 16-Bit
    #define DAC_18BIT                       (0x12 << 8)    // Audio DAC 18-Bit
    #define DAC_20BIT                       (0x14 << 8)    // Audio DAC 20-Bit
    #define DAC_24BIT                       (0x18 << 8)    // Audio DAC 24-Bit


#define REG_EXTADC_CFG      0x0010
#define AUD_IN_CFG_MASK         (0x1fff)
  #define SPDIF_CYC_MASK            (0x3  << 0)      // mask of cycles
    #define SPDIF_CYC16                     (0x0  << 0)      // 16 cycles
    #define SPDIF_CYC24                     (0x1  << 0)      // 24 cycles
    #define SPDIF_CYC32                     (0x2  << 0)      // 32 cycles
  #define SPLIN_FMT_MASK            (0x3  << 2)
    #define SPLIN_RJ                        (0x0  << 2)      // Right aligned with LRCK
    #define SPLIN_LJ                        (0x1  << 2)      // Left aligned with LRCK
    #define SPLIN_I2S                       (0x3  << 2)      // I2S interface
  #define SPLIN_BCK_INVT            (0x1  << 4)      // Inver LR
  #define SPLIN_LR_INVT             (0x1  << 5)      // Inver LR
  #define SPLIN_BNUM_MASK       (0x1f  << 8)   // (number of bits - 1) for line in ADC or SPDIF
    #define ADC_16BIT                       (0xf << 8)    // ADC 16-Bit
    #define ADC_18BIT                       (0x11 << 8)    // ADC 18-Bit
    #define ADC_20BIT                       (0x13 << 8)    // ADC 20-Bit
    #define ADC_24BIT                       (0x17 << 8)    // ADC 24-Bit


#define REG_LIN_CFG0            0x0018
  #define SPDIF_FMT_MASK            (0x8fe<< 0)
  #define SPDIF_LINE_IN_ENABLE          (0x1  << 0)  // Enable SPDIF/Line-In buffering data to DRAM
  #define SPDIF_LINE_IN_DISABLE         (0x0  << 0)  // Disable SPDIF/Line-In buffering data to DRAM
  #define DATA_RAW_16_BITS              (0x0 << 1)  // Store 16 bits data per sample, RAW data
  #define DATA_PCM_24_BITS              (0x1 << 1)  // Store 24 bits data per sample, PCM data 
  #define DATA_LSB_8_BITS           (0x1  << 2)  // LSB 8 bits nonzero
  #define DATA_LSB_SYNC             (0x0  << 2)  //PCM data desided when sync pattern not found
  #define DATA_SWAP                         (0x1  << 3)  // Swap data
  #define DATA_NO_SWAP                  (0x0  << 3)  // No swap data
  #define PCM_CH_NUM_MASK           (0x3  << 4)
    #define PCM_CH_NUM_2_CH             (0x0  << 4)
    #define PCM_CH_NUM_4_CH             (0x1  << 4)
    #define PCM_CH_NUM_6_CH             (0x2  << 4)
    #define PCM_CH_NUM_8_CH             (0x3  << 4)
  #define RISC_INT_PERIOD_MASK  (0x3  << 6)
    #define RISC_INT_PERIOD_32          (0x0  << 6)  // RISC interrupt generating period per 32*4 bytes
    #define RISC_INT_PERIOD_64          (0x1  << 6)  // RISC interrupt generating period per 64*4 bytes
    #define RISC_INT_PERIOD_128     (0x2  << 6)  // RISC interrupt generating period per 128*4 bytes
    #define RISC_INT_PERIOD_256     (0x3  << 6)  // RISC interrupt generating period per 256*4 bytes
  #define PSU_BS_DETEC              (0x0  << 8)
  #define PSU_BS_NO_DETEC           (0x1  << 8)
  #define ADDR_CFG_SL_ALE           (0x0  << 9)
  #define ADDR_CFG_SL_INPUT         (0x1  << 9)
  #define SR_CFG_MUTE_EN            (0x1  << 10)
  #define SR_CFG_MUTE_UNEN          (0x0  << 10)
  #define INT_ADC_EXTLIN                (0x0  << 11)
  #define INT_ADC_INTLIN                (0x1  << 11)
  #define LIN_CLK_SEL_MASK      (0x3  << 12)
    #define AOUT_DEC0               (0x0  << 12)
    #define AOUT_DEC2               (0x1  << 12)
    #define AOUT_HDMI               (0x2  << 12)

#define REG_LINBLK  0x0020

#define REG_SPDFTYP 0x0028
  #define TYPE_DETAIL           (0x1f)
    #define TYPE_AC3              (0x01)
    #define TYPE_MPEG1_L1         (0x04)
    #define TYPE_MPEG1_L2L3       (0x05)
    #define TYPE_MPEG2_EXT        (0x06)
    #define TYPE_AAC              (0x07)
    #define TYPE_MPEG2_L1LSF      (0x08)
    #define TYPE_MPEG2_L2LSF      (0x09)
    #define TYPE_MPEG2_L3LSF      (0x0a)
    #define TYPE_DTS1             (0x0b)
    #define TYPE_DTS2             (0x0c)
    #define TYPE_DTS3             (0x0d)
  #define TYPE_BSNUM            (0x7 << 5)
  #define TYPE_ROUGH            (0x3 << 8)
    #define TYPE_PCM              (0 << 8)
    #define TYPE_RAW              (1 << 8)
    #define TYPE_DTS_16BITS       (2 << 8)
  #define TYPE_DTS_24BITS       (3 << 8)
  #define TYPE_DETECT           (0x1 << 10)

#ifndef CC_MT5391_AUD_SUPPORT
#define SPOCFG0         0x0030
  #define WORD_LENGTH_MASK       (0x3 <<0)
  #define WORD_LENGTH_24BIT        (0 << 0)
  #define WORD_LENGTH_20BIT        (2 << 0)
  #define WORD_LENGTH_16BIT        (3 << 0)
  #define IEC_MUTE                    (1 << 5)
  #define HDMI_RX_SPDIF               (1 << 12)   // write "1" will output spdif data from HDMI-RX to SPDF pin
#else
#define SPOCFG0         0x0030
  #define WORD_LENGTH_MASK       (0x3 <<0)
  #define WORD_LENGTH_24BIT        (0 << 0)
  #define WORD_LENGTH_20BIT        (2 << 0)
  #define WORD_LENGTH_16BIT        (3 << 0)
  #define IEC_MUTE                    (1 << 5)

#define REG_AU_PAD              0x03fc
    #define SPDIF_OUT_SEL_MSK           (0x3 << 4)
    #define AUDIO_SPDIF                 (0 << 4)
    #define GPIO_SPDIF                  (1 << 4)    //5391: limit in GPIO_13 and GPIO_26
    #define HDMI_RX_SPDIF               (2 << 4)   // write "1" will output spdif data from HDMI-RX to SPDF pin
    #define MCLK1_EN_8297           (0x1 << 20)
    #define MCLK0_EN_8297           (0x1 << 16)
#endif

#define REG_BS_CFG              0x0040
  #define PTR_SEL_BANK0_MASK    (0x3 << 0)
  #define PTR_SEL_BANK0_PSR             (0 << 0) 
  #define PTR_SEL_BANK0_SPDIF_LINE_IN   (1 << 0) 
  #define PTR_SEL_BANK0_DMX             (2 << 0)
  #define PTR_SEL_BANK0_VFIFO           (3 << 0)    ///[Joel]
  #define PTR_SEL_BANK1_MASK    (0x3 << 2)
  #define PTR_SEL_BANK1_PSR             (0 << 2) 
  #define PTR_SEL_BANK1_SPDIF_LINE_IN   (1 << 2) 
  #define PTR_SEL_BANK1_DMX             (2 << 2)
  #define PTR_SEL_BANK1_VFIFO           (3 << 2)    ///[Joel]
  #define CLR_BS                        (1 << 4)  
  #define GBCD                                  (1 << 9)   
  #define PAR_SEL                       (1<<10)
  #define PAR_SEL2                      (0<<13)     ///[Joel]
  #define PAR_SEL2_MASK                 (1<<13)     ///[Joel]

#define REG_DMX_WRITE_PNT   0x004c
#define REG_DMX_STR             0x0050
#define REG_DMX_END             0x0054

/*
#define REG_ADSP_CFG            0x0080
#define DRAM_BLOCK_MASK             (0x6 << 20)    
  #define DRAM_BLOCK_0_16MB     (0 << 20)      // audio dram buffer will locate in 0~16M Bytes
  #define DRAM_BLOCK_16_32MB    (1 << 20)      // audio dram buffer will locate in 16~32M Bytes
  #define DRAM_BLOCK_32_48MB    (2 << 20)      // audio dram buffer will locate in 32~48M Bytes
  #define DRAM_BLOCK_48_64MB    (3 << 20)      // audio dram buffer will locate in 48~64M Bytes
*/

#define REG_RWD_BLK01           0x00b0
#define REG_RWD_BCLK23          0x00b4
#define REG_CMPTD_BLK01         0x00c0
#define REG_CMPTD_BLK23         0x00c4

#define REG_DSP_PC                 0x015c+0x40

#if 1 // PROBE_RELATED_API
#define REG_DSP_CTRL_R             0x0100
#define REG_DSP_RB_BASE            0x0114
#define REG_DSP_PB_CMSK            (REG_DSP_RB_BASE)
#define REG_DSP_PB_CTAR            (REG_DSP_RB_BASE+0x04)
#define REG_DSP_PB_TAR             (REG_DSP_RB_BASE+0x08)
#define REG_DSP_PB_CTRL            (REG_DSP_RB_BASE+0x0c)
#define REG_DSP_PB_INDEX           (REG_DSP_RB_BASE+0x10)
#define REG_DSP_PB_DATA            (REG_DSP_RB_BASE+0x14)
#define REG_DSP_PB_STOP            (REG_DSP_RB_BASE+0x18)
#define REG_DSP_PB_STP0            (REG_DSP_RB_BASE+0x1c)
#define REG_DSP_PB_STP1            (REG_DSP_RB_BASE+0x20)
#define REG_DSP_PB_STP2            (REG_DSP_RB_BASE+0x24)
#define REG_DSP_PB_STP3            (REG_DSP_RB_BASE+0x28)
#define REG_DSP_PB_WRDX            (REG_DSP_RB_BASE+0x34+0x40) 
#define REG_DSP_PB_WR              (REG_DSP_RB_BASE+0x38+0x40) 
#define REG_DSP_PB_HIDX            (REG_DSP_RB_BASE+0x3C+0x40) 
#endif

#define REG_CH_CFG          0x0200
  #define AOUTDATA0_MAIN          (0)
  #define AOUTDATA1_MAIN          (1)
  #define AOUTDATA2_MAIN          (2)
  #define AOUTDATA3_MAIN          (3)
  #define AOUTDATA4_MAIN          (4)
  #define AOUTDATA0_AUX            (5)
  #define CH0_SEL_POS               (0)
  #define CH_SEL_MASK           (0x3FFFF)
    #define CH_SEL                  (((UINT32)CH_SEL_MASK) << CH0_SEL_POS)
    #define CH0_AOUTDATA0       (0 << CH0_SEL_POS)
    #define CH0_AOUTDATA1       (1 << CH0_SEL_POS)
    #define CH0_AOUTDATA2       (2 << CH0_SEL_POS)
    #define CH0_AOUTDATA3       (3 << CH0_SEL_POS)
    #define CH0_AOUTDATA4       (4 << CH0_SEL_POS)
  #define CH1_SEL_POS               (3)
    #define CH1_AOUTDATA0       (0 << CH1_SEL_POS)
    #define CH1_AOUTDATA1       (1 << CH1_SEL_POS)
    #define CH1_AOUTDATA2       (2 << CH1_SEL_POS)
    #define CH1_AOUTDATA3       (3 << CH1_SEL_POS)
    #define CH1_AOUTDATA4       (4 << CH1_SEL_POS)
  #define CH2_SEL_POS               (6)
    #define CH2_AOUTDATA0       (0 << CH2_SEL_POS)
    #define CH2_AOUTDATA1       (1 << CH2_SEL_POS)
    #define CH2_AOUTDATA2       (2 << CH2_SEL_POS)
    #define CH2_AOUTDATA3       (3 << CH2_SEL_POS)
    #define CH2_AOUTDATA4       (4 << CH2_SEL_POS)
  #define CH3_SEL_POS               (9)
    #define CH3_AOUTDATA0       (0 << CH3_SEL_POS)
    #define CH3_AOUTDATA1       (1 << CH3_SEL_POS)
    #define CH3_AOUTDATA2       (2 << CH3_SEL_POS)
    #define CH3_AOUTDATA3       (3 << CH3_SEL_POS)
    #define CH3_AOUTDATA4       (4 << CH3_SEL_POS)
  #define CH4_SEL_POS               (12)
    #define CH4_AOUTDATA0       (0 << CH4_SEL_POS)
    #define CH4_AOUTDATA1       (1 << CH4_SEL_POS)
    #define CH4_AOUTDATA2       (2 << CH4_SEL_POS)
    #define CH4_AOUTDATA3       (3 << CH4_SEL_POS)
    #define CH4_AOUTDATA4       (4 << CH4_SEL_POS)
  #define DEC2_CH0_SEL_POS              (15)
    #define DEC2_CH0_AOUTDATA0      (0 << DEC2_CH0_SEL_POS)
    #define DEC2_CH0_AOUTDATA1      (1 << DEC2_CH0_SEL_POS)
    #define DEC2_CH0_AOUTDATA2      (2 << DEC2_CH0_SEL_POS)
  #define DEC2_CH1_SEL_POS              (18)
    #define DEC2_CH1_AOUTDATA0      (0 << DEC2_CH1_SEL_POS)
    #define DEC2_CH1_AOUTDATA1      (1 << DEC2_CH1_SEL_POS)
    #define DEC2_CH1_AOUTDATA2      (2 << DEC2_CH1_SEL_POS)
  #define SDATA_MCLK_SYNC_POS     (24)
  //#define SDATA_MCLK_SYNC_MASK      (0x3F)
  #define SDATA_MCLK_SYNC_MASK      (0x1F)
  #define SDATA_MCLK_SYNC           (SDATA_MCLK_SYNC_MASK << SDATA_MCLK_SYNC_POS)
  #define AOUTDATA0_AUX_TO_SDATA0           (1 << 24)
  #define AOUTDATA0_AUX_TO_SDATA1           (1 << 25)
  #define AOUTDATA0_AUX_TO_SDATA2           (1 << 26)
  #define AOUTDATA0_AUX_TO_SDATA3           (1 << 27)
  #define AOUTDATA0_AUX_TO_SDATA4           (1 << 28)
  #define AOUTDATA_MCLK_MAIN_TO_SDATA5  (1 << 29)  
  #define BCK_LRCK_SEL (1 << 30) 
  #define CKSEL1       ((UINT32)1 << 31)  //pwh for audio
  #define CKSEL2       ((UINT32)1 << 30)  //pwh for audio
#define REG_INTERDACHCG  0x204
#define REG_AOUT_DRAM_CFG   0x0210
  #define DRAM_ACCESS_128_BITS          (1  << 4)

#define REG_SDADC0_DRV		0x240
  #define RST_FILTER		(0x1 << 0)
  
#define REG_TEST_CFG	0x0250
	#define HDMI_SPDIF_SEL		(0x1 << 0)
	#define DTVSLT		(0x03 << 1)
/*
// redefined in codec_mt538xinternal.h
#define REG_DACCTL8 0x01E0
    #define DEC0_CLK_SEL        (0x1 < 8)
    #define DEC1_CLK_SEL        (0x1 < 9)
*/
#define REG_APGA_CFG0           0x035c
#define REG_APGA_CFG1           0x0360
#define REG_APGA_CFG2           0x0364
#define REG_APGA_CFG3           0x0368
#define REG_APGA_CFG4           0x0370
#define REG_APGA_CFG5           0x0374


#define REG_DSPROM_CFG      0x16C       //FIXME !! dsp rom access timing issue
#define REG_AINROM_CFG      0x288

#define REG_AUD_X_CFG       0x24C
    #define LR_DETECT_ON       (0x1 << 30)
#define REG_RISC_INT                    0x0068
    #define DSPB2RC_INTR                (0x01 << 9)
    #define DSPA2RC_INTR                (0x01 << 8)

//---------------------------------------------------------------------------
// Internal dac definitions
//---------------------------------------------------------------------------
#define REG_INT_DACCTL6         0x01d8
    #define APLL_CLK_SEL_0      (0x1U << 0)
    #define APLL_CLK_SEL_1      (0x1U << 1)
    #define APLL_CLK_SEL_2      (0x1U << 2)
    #define MOD_RST_STG_MASK    (0xfU << 12)
    #define MOD_RST_STG         (0x3U << 12)
    #define MUTE_0              (0x1U << 28)
    #define MUTE_1              (0x1U << 29)    
    #define MUTE_2              (0x1U << 30)      

#define REG_INT_DACCTL10         0x01e8
    #define DAC_CLK_SEL_0       (0x1 << 0)
    #define DAC_CLK_SEL_1       (0x1 << 1)
    #define DAC_CLK_SEL_2       (0x1 << 2)

//---------------------------------------------------------------------------
// DSP CLK Select and PAD Select
//---------------------------------------------------------------------------
#ifdef CC_MT5391_AUD_SUPPORT
// MT5390
#define REG_SYSPLL_CFG2         0x0164
#define REG_AIN_CKCFG           0x023c
#define REG_AUD_CKCFG_MAIN      0x0240
#define REG_AUD_CKCFG_AUX       0x0258 
    #define AUD_CLK_SEL_MASK       (0x7U << 0)
    #define AUD_CLK_SEL_APLL       (0x1  << 0)
    #define AUD_CLK_SEL_AOMCLK     (0x2  << 0)
    #define AUD_CLK_SEL_HDMI       (0x3  << 0)
    #define AUD_CLK_SEL_SPDIF      (0x4  << 0)
    #define AUD_CLK_SEL_APLL_K1    (0x5  << 0)
    #define AUD_CLK_SEL_LDMCLK0    (0x6  << 0)//8297
    #define AUD_CLK_SEL_LDMCLK1    (0x7  << 0)//8297    
#define REG_DSP_CLK_SEL         0x0228
  #define PDN_DSP                  (1 << 7)  //turn off audio dsp clock
  #define ADSP_CLK_SEL_MASK        (0x0f << 0)


#define REG_PINMUX_SEL_0    0x400
#define REG_PINMUX_SEL_1    0x404
#define REG_PINMUX_SEL_2    0x408
#define REG_PINMUX_SEL_3    0x40c     

#else
// MT5360
#define REG_SYSPLL_CFG1         0x0164
    #define REG_SYSPLL_CFG1_AUD_MASK    (0x003fbc00U)
    #define SYSPLL_DIG_SEL_FROM_SYSPLL  (0x0U << 21)
    #define SYSPLL_DIGI_DIV_SEL_BY_2    (0x01U << 16)
    #define SYSPLL_DIGI_DIV_INIT_INITIAL    (0x01U << 15)
    #define SYSPLL_AUADC_SEL_FROM_54MHZ     (0x01U << 12)
    #define SYSPLL_APLL_SEL_FROM_SYSPLL     (0x03U << 10)
#define REG_AIN_CKCFG           0x0250
    #define REG_AIN_CKCFG_MASK          (0x0f7U)
    #define AIN_CK_PD_NORMAL            (0x0U << 7)
    #define AIN_CK_TST_NORMAL           (0x0U << 4)
    #define AIN_CK_SEL_TVDPLL_D5_CK     (0x01U << 0)
#define REG_AUD_CKCFG_MAIN      0x0254
#define REG_AUD_CKCFG_AUX       0x0268 
    #define AUD_CLK_SEL_MASK       (0x7U << 0)
    #define AUD_CLK_SEL_APLL       (0x1  << 0)
    #define AUD_CLK_SEL_AOMCLK     (0x2  << 0)
    #define AUD_CLK_SEL_HDMI       (0x3  << 0)
    #define AUD_CLK_SEL_SPDIF      (0x4  << 0)
    #define AUD_CLK_SEL_APLL_K1    (0x5  << 0)
    #define AUD_CLK_SEL_LDMCLK0    (0x6  << 0)//8297
    #define AUD_CLK_SEL_LDMCLK1    (0x7  << 0)//8297    
#define REG_DSP_CLK_SEL         0x0234
  #define PDN_DSP                  (1 << 7)  //turn off audio dsp clock
  #define ADSP_CLK_SEL_MASK        (0x0f << 0)
  #define ADSP_CLK_270MHZ           (0x01)
  #define ADSP_CLK_dtd_2           (0x03)
#endif
  
//---------------------------------------------------------------------------
// Parser definitions
//---------------------------------------------------------------------------

#define REG_VSTA                0x0C        ///JPEG fifo start addr, CC_AUDIO_DSP_JPG_SUPPORT, ADSP_JPEG_DEC2
#define REG_VEND                0x10        ///JPEG fifo end addr
#define REG_VRP                 0x54        ///JPEG read point
#define REG_VWP                 0x58        ///JPEG write point

#define REG_A0STA				0x14		// Audio buffer 0 start address
#define REG_A0END				0x18		// Audio buffer 0 end address

#define REG_ARP					0x1d4		// Audio buffer read pointer
#define REG_AWP					0x5c		// Audio buffer write pointer


#define REG_PARCTL				0x1f0
  #define PARCTL_RST			(1 << 0)	// Reset parser hardware
  #define PARCTL_PCKE			(1 << 1)	// Parser clock enable

  #define PRS_RST          ((unsigned)0x1 << 0)
  #define PRS_CKE          ((unsigned)0x1 << 1)
  #define PRS_AUTOPD       ((unsigned)0x1 << 2)
  #define WDLE_MODE         ((unsigned)0x1 << 3)

#define vPREnableAll()   PARSER_WRITE32(REG_PARCTL, PRS_RST + PRS_CKE)
#define vPRDisableAll()  PARSER_WRITE32(REG_PARCTL, 0)


#ifdef CC_MT5391_AUD_SUPPORT
//---------------------------------------------------------------------------
// MT8297 definitions
//---------------------------------------------------------------------------
#define REG_MCLK0_CKCFG0        0x09
#define REG_MCLK1_CKCFG0        0x0B
    #define REG_CLK_SRC_SEL_MSK        0x03
    #define MCLK_SEL_APLL0      0x01
    #define MCLK_SEL_APLL1      0x02

#define REG_MCLK0_CKCFG1        0x0A
#define REG_MCLK1_CKCFG1        0x0C
    #define MCLK_PIN_OUTPUT_EN_MSK     0x10

#define REG_APLL0_CFG0          0x1D
#define REG_APLL1_CFG0          0x2A
    #define REG_APLL_FREFSEL_MSK       0xC0
#define REG_APLL0_CFG2          0x1F
#define REG_APLL1_CFG2          0x2C
    #define REG_APLL_VCO_BAND_MSK       0x07
    #define REG_APLL_PREDIV_EN_MSK      0x08
#define REG_APLL0_CFG3          0x20    
#define REG_APLL1_CFG3          0x2D    
    #define REG_APLL_POSTDIV_MSK        0xff
#define REG_APLL0_CFG4          0x21
#define REG_APLL0_CFG5          0x22
#define REG_APLL0_CFG6          0x23
#define REG_APLL1_CFG4          0x2E
#define REG_APLL1_CFG5          0x2F
#define REG_APLL1_CFG6          0x30
    #define REG_APLL_MOD_MSK        0xFF
#define REG_APLL0_CFG7          0x24
#define REG_APLL0_CFG8          0x25
#define REG_APLL1_CFG7          0x31
#define REG_APLL1_CFG8          0x32
    #define RG_APLL_SCFDIV_MASK    0xF0
    #define RG_APLL_LPFDIV_MASK    0xFF
    #define REG_APLL_POSDIVSEL_MSK   0x08
#define REG_APLL0_CFGC          0x29
#define REG_APLL1_CFGC          0x36
    #define APLL_K1K2_MSK             0xFF

#define REG_BANK                0x7F
#define REG_BANK_MSK                        0xff
#define REG_DAC_18              0x78
    #define APLL_DAC0_CLK_SEL_MSK            0x01
    #define APLL_DAC1_CLK_SEL_MSK            0x02
    #define APLL_DAC2_CLK_SEL_MSK            0x04
    #define APLL_DAC0_CLK_SEL_APLL0          0x00
    #define APLL_DAC0_CLK_SEL_APLL1          0x01
    #define APLL_DAC1_CLK_SEL_APLL0          0x00
    #define APLL_DAC1_CLK_SEL_APLL1          0x02
    #define APLL_DAC2_CLK_SEL_APLL0          0x00
    #define APLL_DAC2_CLK_SEL_APLL1          0x04
#define REG_ADAC_03             0x63
#define REG_ADAC_0B             0x6B
#define REG_ADAC_13             0x73
    #define OSR_SET_MSK                     0x02
    #define OSR_512fs                       0x00
    #define OSR_1024fs                      0x01
    #define OSR_2048fs                      0x02

#define REG_ADAC_2C             0x6C
    #define CLKDIV24SEL0_MSK                0x01
    #define CLKDIV24SEL1_MSK                0x02
    #define CLKDIV24SEL2_MSK                0x04
    #define CLKDIV24SEL0_SEL_APLL0          0x00
    #define CLKDIV24SEL0_SEL_APLL1          0x01
    #define CLKDIV24SEL1_SEL_APLL0          0x00
    #define CLKDIV24SEL1_SEL_APLL1          0x02
    #define CLKDIV24SEL2_SEL_APLL0          0x00
    #define CLKDIV24SEL2_SEL_APLL1          0x04
    
    
#endif    

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define CKGEN_READ8(offset)                 IO_READ8(CKGEN_BASE, (offset))
#define CKGEN_READ16(offset)            IO_READ16(CKGEN_BASE, (offset))
#define CKGEN_READ32(offset)            IO_READ32(CKGEN_BASE, (offset))

#define CKGEN_WRITE8(offset, value)         IO_WRITE8(CKGEN_BASE, (offset), (value))
#define CKGEN_WRITE16(offset, value)        IO_WRITE16(CKGEN_BASE, (offset), (value))
#define CKGEN_WRITE32(offset, value)        IO_WRITE32(CKGEN_BASE, (offset), (value))

#define AUD_READ8(offset)                   IO_READ8(AUDIO_BASE, (offset))
#define AUD_READ16(offset)                  IO_READ16(AUDIO_BASE, (offset))
#define AUD_READ32(offset)                  IO_READ32(AUDIO_BASE, (offset))

#define AUD_WRITE8(offset, value)           IO_WRITE8(AUDIO_BASE, (offset), (value))
#define AUD_WRITE16(offset, value)          IO_WRITE16(AUDIO_BASE, (offset), (value))
#define AUD_WRITE32(offset, value)          IO_WRITE32(AUDIO_BASE, (offset), (value))

#define AUD_SET_BIT(Reg, Bit)                   AUD_WRITE32(Reg, AUD_READ32(Reg) | (Bit))
#define AUD_CLR_BIT(Reg, Bit)                   AUD_WRITE32(Reg, AUD_READ32(Reg) & (~(Bit)))

#define CKGEN_SET_BIT(Reg, Bit)                 CKGEN_WRITE32(Reg, CKGEN_READ32(Reg) | (Bit))
#define CKGEN_CLR_BIT(Reg, Bit)                 CKGEN_WRITE32(Reg, CKGEN_READ32(Reg) & (~(Bit)))

#define AUD_READ32_SPECIAL(offset)          ((AUD_READ32(offset) & 0xffff) | ((AUD_READ32(offset) & 0x1800000) << 1))
#define AUD_SET_BIT_SPECIAL(Reg, Bit)       AUD_WRITE32(Reg, AUD_READ32_SPECIAL(Reg) | (Bit))
#define AUD_CLR_BIT_SPECIAL(Reg, Bit)       AUD_WRITE32(Reg, AUD_READ32_SPECIAL(Reg) & (~(Bit)))


#define PARSER_READ8(offset)			IO_READ8(PARSER0_BASE, (offset))
#define PARSER_READ16(offset)			IO_READ16(PARSER0_BASE, (offset))
#define PARSER_READ32(offset)			IO_READ32(PARSER0_BASE, (offset))
#define PARSER_WRITE8(offset, value)	IO_WRITE8(PARSER0_BASE, (offset), (value))
#define PARSER_WRITE16(offset, value)	IO_WRITE16(PARSER0_BASE, (offset), (value))
#define PARSER_WRITE32(offset, value)	IO_WRITE32(PARSER0_BASE, (offset), (value))
#define PARSER_2_WRITE32(offset, value)	IO_WRITE32(AUDIO_BASE + 0x00600, (offset), (value))


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#endif /* _AUD_HW_H_ */

