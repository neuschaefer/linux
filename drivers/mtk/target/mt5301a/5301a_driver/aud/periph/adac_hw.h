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
 * $RCSfile: adac_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_hw.h
 *  Brief of file adac_hw.h. \n
 *  Note that MT537X support 4-channel interanl DAC. This file define the 
 *  internal ADAC control registers.
 */

#ifndef _ADAC_HW_H_
#define _ADAC_HW_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define REG_IDACHCG        0x0204
/*
  #define INT_DAC1_SEL_MASK  	(7 << 0)
  #define INT_DAC1_SEL_LR_PAIR 		(0 << 0)      // Interal dac sample form L/R channel  
  #define INT_DAC1_SEL_SLR_PAIR 	(1 << 0)     // Interal dac sample form LS/RS channel  
  #define INT_DAC1_SEL_C_PAIR  		(2 << 0)      // Interal dac sample form C/LFE channel  
  #define INT_DAC1_SEL_CH78_PAIR 	(3 << 0)    // Interal dac sample form 7/8 channel  
  #define INT_DAC1_SEL_CH910_PAIR 	(4 << 0)   // Interal dac sample form 9/10 channel
  #define INT_DAC2_SEL_MASK    	(7 << 3)
  #define INT_DAC2_SEL_LR_PAIR 		(0 << 3)      // Interal dac sample form L/R channel  
  #define INT_DAC2_SEL_SLR_PAIR 	(1 << 3)     // Interal dac sample form LS/RS channel  
  #define INT_DAC2_SEL_C_PAIR  		(2 << 3)      // Interal dac sample form C/LFE channel  
  #define INT_DAC2_SEL_CH78_PAIR 	(3 << 3)    // Interal dac sample form 7/8 channel  
  #define INT_DAC2_SEL_CH910_PAIR 	(4 << 3)   // Interal dac sample form 9/10 channel  

#define REG_DACCTL0       0x01C0
	#define DITH_INIT_0		(0x7ffff << 0)
	#define MUTE_0			(1 << 24)
	#define MONITOR_SEL		(3 << 26)
	#define TEST_CLK_SEL	(3 << 28)
	
#define REG_DACCTL1       0x01C4
	#define DWA_EN_0		(1 << 0)
	#define DIS_SETZ_0		(1 << 1)
	#define BYP_FIR_0		(1 << 2)
	#define REV_LR			(1 << 3)
	#define PRSS_SEL_0		(7 << 4)
	#define MODOUT_DLY_0	(15 << 8)
	#define MODOUT_INT_0	(1 << 14)
	#define MOD_COEF_0		(3 << 16)
	#define OSR_0			(2 << 22)
	#define BCK_INV_0		(1 << 24)
	#define LPF_INV_0		(1 << 25)
	#define MOD_INV_0		(1 << 26)
	#define SCF_INV_0		(1 << 27)

#define REG_DACCTL2       0x01C8
	#define MUTE_1			(1 << 24)
	
#define REG_DACCTL3       0x01CC


#define REG_DACCTL4       0x01D0
  #define PD_REF 				(3 << 0)      
  #define PD_TEST  			(1 << 2)     
  #define PD_ISOURCE 		(1 << 3)      
  #define PD_CH  				(15 << 4)     


#define REG_DACCTL5       0x01D4
	#define CLKEN			(15 << 8) 		
#define REG_DACCTL6       0x01D8
#define REG_DACCTL8       0x01E0
  #define MAIN_DEC_CLK_SEL_APLL0     (0 << 8)
  #define MAIN_DEC_CLK_SEL_APLL1   	(1 << 8)
  #define AUX_DEC_CLK_SEL_APLL0	(0 << 9)
  #define AUX_DEC_CLK_SEL_APLL1	(1 << 9)
*/

#define REG_AENV_CFG        0x00a8
  #define GBCD                 (1 << 0)      // 
  #define DRAM_BLOCK_MASK      (0x3 << 1)    
  #define DRAM_BLOCK_0_16MB    (0 << 1)      // audio dram buffer will locate in 0~16M Bytes
  #define DRAM_BLOCK_16_32MB   (1 << 1)      // audio dram buffer will locate in 16~32M Bytes
  #define DRAM_BLOCK_32_48MB   (2 << 1)      // audio dram buffer will locate in 32~48M Bytes
  #define DRAM_BLOCK_48_64MB   (3 << 1)      // audio dram buffer will locate in 48~64M Bytes
  #define INT_DAC1_SEL_MASK    ((7 << 3) | (1 << 9))
  #define INT_DAC1_SEL_LR_PAIR (0 << 3)      // Interal dac sample form L/R channel  
  #define INT_DAC1_SEL_SLR_PAIR (1 << 3)     // Interal dac sample form LS/RS channel  
  #define INT_DAC1_SEL_C_PAIR  (2 << 3)      // Interal dac sample form C/LFE channel  
  #define INT_DAC1_SEL_CH78_PAIR (3 << 3)    // Interal dac sample form 7/8 channel  
  #define INT_DAC1_SEL_CH910_PAIR (4 << 3)   // Interal dac sample form 9/10 channel
  #define INT_DAC2_SEL_MASK    ((7 << 6) | (1 << 9))
  #define INT_DAC2_SEL_LR_PAIR (0 << 5)      // Interal dac sample form L/R channel  
  #define INT_DAC2_SEL_SLR_PAIR (1 << 5)     // Interal dac sample form LS/RS channel  
  #define INT_DAC2_SEL_C_PAIR  (2 << 5)      // Interal dac sample form C/LFE channel  
  #define INT_DAC2_SEL_CH78_PAIR (3 << 5)    // Interal dac sample form 7/8 channel  
  #define INT_DAC2_SEL_CH910_PAIR (4 << 5)   // Interal dac sample form 9/10 channel  
  #define INT_DAC_SEL_MAIN      (0 << 9)     // Select source from main decoder
  #define INT_DAC_SEL_AUX       (1 << 9)     // Select source from aux decoder
  #define LOOP_SEL_EN           (1 << 10)    // 
  #define LOOP_BACK_TEST_EN     (1 << 11)    // Turn on Loop back test , write "1" to this bits will turn on loop back test.
  #define MUTE_MCLK             (1 << 16)
  #define MUTE_MCLK_DEC2        (1 << 17)
      #define MCLK0                   (0)
      #define MCLK1                   (1)
      #define SDATA_MCLK_SYNC_POS     (18)
  #define SDATA_MCLK_SYNC_MASK    (0x7F)
  #define SDATA_MCLK_SYNC         (SDATA_MCLK_SYNC_MASK << SDATA_MCLK_SYNC_POS)
  #define MCLK_AUX_TO_MCLK_MAIN           (1 << 18)
  #define AOUTDATA0_AUX_TO_SDATA0         (1 << 19)
  #define AOUTDATA0_AUX_TO_SDATA1         (1 << 20)
  #define AOUTDATA0_AUX_TO_SDATA2         (1 << 21)
  #define AOUTDATA0_AUX_TO_SDATA3         (1 << 22)
  #define AOUTDATA0_AUX_TO_SDATA4         (1 << 23)
  #define AOUTDATA_MCLK_MAIN_TO_SDATA5    (1 << 24)  
  //#define HDMI_RX_SPDIF               (1 << 25)   // write "1" will output spdif data from HDMI-RX to SPDF pin
  #define APLL_SEL_MASK               ((UINT32)0xf0000000)
  #define INTER_DAC_CLK_SEL_APLL0     (0 << 28)
  #define INTER_DAC_CLK_SEL_APLL1     (1 << 28)
  #define DAC_CLK_SEL_APLL0           (0 << 29)
  #define DAC_CLK_SEL_APLL1           (1 << 29)
  #define MAIN_DEC_CLK_SEL_APLL0      (0 << 31)
  #define MAIN_DEC_CLK_SEL_APLL1      (1 << 31)
  #define AUX_DEC_CLK_SEL_APLL0       ((UINT32)(0 << 30))
  #define AUX_DEC_CLK_SEL_APLL1       ((UINT32)(1 << 30))

#define REG_ADC_CFG0       0x0090
  #define ADC_TEST_EN        (1 << 6)      // Test enable
  #define VREF_TEST_EN       (1 << 7)      // Enable VREFP/VREFN for test
  #define ADC_CLR            (1 << 8)      // Clear ADC internal digital circuit
  #define TRIG_SEL_FALLING   (0 << 9)      // Rising edge / falling edge select, 
  #define TRIG_SEL_RISING    (1 << 9)      // 0: falling edge, 1: rising edge
  #define CLK_DIV_BY_1       (0 << 10)     // Clock divided  1: Fs
  #define CLK_DIV_BY_2       (1 << 10)     // Clock divided  2: 1/2 Fs : default
  #define CLK_SEL_EXT        (0 << 11)     // External clock
  #define CLK_SEL_INT        (1 << 11)     // Internal clock : default
  #define NCLK_CTRL_300_PS   (0 << 12)     // Non-overlap clock timing select 
  #define NCLK_CTRL_350_PS   (1 << 12)     // Default
  #define NCLK_CTRL_400_PS   (2 << 12)    

#define REG_ADC_CFG1       0x0094
  #define CORE_POWER_DOWN_EN (1 << 0)      // Power down ADC core
  #define REF_POWER_DOWN_EN  (1 << 1)      // Power down ADC reference
  #define CHL_SEL_MASK       (3 << 4)
  #define CHL_SEL_DUAL       (0 << 4)      // CH0/CH1
  #define CHL_SEL_CH0        (1 << 4)      // CH0 : default
  #define CHL_SEL_CH1        (2 << 4)      // CH1
  #define DAC1_CUR_X0_5      (0 << 8)      // x0.5 DAC mdac1 current select
  #define DAC1_CUR_X0_75     (1 << 8)      // x0.75 DAC mdac1 current select
  #define DAC1_CUR_X1        (2 << 8)      // x1 DAC mdac1 current select
  #define DAC1_CUR_X1_5      (3 << 8)      // x1.5 DAC mdac1 current select
  #define DAC2_CUR_X0_5      (0 << 10)     // x0.5 DAC mdac2 current select
  #define DAC2_CUR_X0_75     (1 << 10)     // x0.75 DAC mdac2 current select
  #define DAC2_CUR_X1        (2 << 10)     // x1 DAC mdac2 current select
  #define DAC2_CUR_X1_5      (3 << 10)     // x1.5 DAC mdac2 current select
  #define DAC3_CUR_X0_5      (0 << 12)     // x0.5 DAC mdac3 current select
  #define DAC3_CUR_X0_75     (1 << 12)     // x0.75 DAC mdac3 current select
  #define DAC3_CUR_X1        (2 << 12)     // x1 DAC mdac3 current select
  #define DAC3_CUR_X1_5      (3 << 12)     // x1.5 DAC mdac3 current select
  #define DAC4_CUR_X0_5      (0 << 14)     // x0.5 DAC mdac4 current select
  #define DAC4_CUR_X0_75     (1 << 14)     // x0.75 DAC mdac4 current select
  #define DAC4_CUR_X1        (2 << 14)     // x1 DAC mdac4 current select
  #define DAC4_CUR_X1_5      (3 << 14)     // x1.5 DAC mdac4 current select
  
#define REG_ADC_CFG2       0x0098
  #define VOL_CUR_ADJUST_DEFAULT (0x8888AAAA)
  
#define REG_ADC_CFG3       0x0308
  #define ADC_CFG3_DEFAULT       (0x27)
#define REG_ADC_CFG4       0x030c
  #define ADC_CFG4_DEFAULT       (0x14)
#define REG_ADC_CFG5       0x0314
  #define ADC_CFG5_DEFAULT       (0x22)

#define REG_ADC_CFG_24      0x0324
#define REG_ADC_CFG_34      0x0334


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// Note that MT537X support 4-channel interanl DAC.
// We assume channel 1&2 as stereo main channel, and channel 3&4 as stereo 
// aux channel.
typedef enum
{
    ADAC_STEREO_CHANNEL_MAIN,   // channel 1&2
    ADAC_STEREO_CHANNEL_AUX     // channel 3&4
}   ADAC_STEREO_CHANNEL_T ;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define ADAC_READ8(offset)			    IO_READ8(AUDIO_BASE, (offset))
#define ADAC_READ16(offset)			    IO_READ16(AUDIO_BASE, (offset))
#define ADAC_READ32(offset)			    IO_READ32(AUDIO_BASE, (offset))

#define ADAC_WRITE8(offset, value)	    IO_WRITE8(AUDIO_BASE, (offset), (value))
#define ADAC_WRITE16(offset, value)	    IO_WRITE16(AUDIO_BASE, (offset), (value))
#define ADAC_WRITE32(offset, value)	    IO_WRITE32(AUDIO_BASE, (offset), (value))

#define ADAC_SET_BIT(Reg, Bit)          ADAC_WRITE32(Reg, ADAC_READ32(Reg) | (Bit))
#define ADAC_CLR_BIT(Reg, Bit)          ADAC_WRITE32(Reg, ADAC_READ32(Reg) & (~(Bit)))

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern ADAC_T * _ADAC_GetInterDacTbl(void);
extern void _ADAC_InterDacInit(void);
extern void _ADAC_InterDacEnable(BOOL fgEnable);
extern void _ADAC_InterDacChlSel(UINT8 u1DacId, AUD_CHL_DEF_T eChl);
extern void _ADAC_InterDacMute(BOOL fgEnable);

#endif /* _ADAC_HW_H_ */

