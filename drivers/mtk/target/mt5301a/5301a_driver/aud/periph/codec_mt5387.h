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
 * $RCSfile: codec_mt8291.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_MT8291.h
 *  Brief of file codec_MT8291.h. \n
 */

#ifndef CODEC_MT5387_H
#define CODEC_MT5387_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"
#include "drvcust_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
//#define PWM_DAC  // David

#define FADE_IN       0
#define FADE_OUT    1
#define PWM_XRAM   0
#define PWM_YRAM   1

#define MT8223_PWM_SETTING

#define IO_BASE_ADDRESS    0x70000000

// -+-+- Memory Address Map -+-+-
#define REG_INDIRECT0_ADDR 		0x9C0
#define REG_INDIRECT0_DATA 		0x9C4
#define REG_INDIRECT1_ADDR 		0xDC0
#define REG_INDIRECT1_DATA 		0xDC4

#define REG_PWMCFG0_PG1                   0x1C8
#define REG_PWMCFG0_PG2                   0x1CC
  #define UPDATE_NUM_0                      (0x00<< 16) // The timing for latching audio data input 
  #define UPDATE_NUM_1                      (0x01<< 16) // The timing for latching audio data input 
  #define UPDATE_NUM_2                      (0x02<< 16) // The timing for latching audio data input 
  #define GHRESETZ                               (0x01<< 15) // Softreset bit (active low) for PWM G1/G2
  #define WRITE_DMAP_DATA_EN         (0x01<< 12) //Feed audio sample data by ADSP
  #define PWM_ADAC_MUTE                  (0x01<< 8)
  

#define REG_PWMIP_DELSEL                  0x1D0
  #define XRAM_DELSEL_10                    (0x02<< 2)
  #define YRAM_DELSEL_01                    (0x01<< 0)
  
#define REG_PWMIP_PINMUX_PG1         0x1D4
#define REG_PWMIP_PINMUX_PG2         0x1D8
#define REG_PWMIP_MON_PG1               0x1DC
#define REG_PWMIP_MON_PG2               0x1E0
#define REG_PWMIP_GETIDA_PG1          0x1E4
#define REG_PWMIP_GETIDD_PG1          0x1E8
#define REG_PWMIP_PWMPAD_CFG        0x1EC
#define REG_PWMIP_BTLMODE_PG1       0x1F0
#define PWMBTLMODE                             0x49820000
#define REG_PWMIP_MISC0_PG1            0x1F4 // Ramping for G1
#define REG_PWMIP_MISC0_PG2            0x1F8 // Ramping for G2
  #define RAMP_SPEED_MIN                    ((UINT32)(0x3F))<<26  // Suggested
  #define RAMP_SPEED_MAX                   (0x01)<<26  // Fastest, not suggested
  #define RAMP_STEP_MIN                      (0x01<<8)  // Suggested (smallest)
  #define RAMP_EN_ALL                          (0x3F<<0)  // Enable all cheannels ramp mode 
  #define RAMP_DIS_ALL                         (0x00<<0)  // Enable all cheannels ramp mode 
  #define RAMP_EN_CH0                          (0x01<<0)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH1                          (0x01<<1)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH2                          (0x01<<2)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH3                          (0x01<<3)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH4                          (0x01<<4)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH5                          (0x01<<5)   // Enable  cheannel 0 ramp mode 

  #define REG_PWMIP_MISC1                     0x1FC
  #define PWMIP_RAMP_OFF                           0x00003fff
  #define PWMIP_RAMP_ON                             0x00000fff
  #define RAMP_UP_AUTO_CLOSE            (0x01<<13)   
  #define RAMP_UP_EN                            (0x01<<12)   
  #define RAMP_COEFF_RELOAD              (0xFF<<0)   

#define A_BASE 		0x000F00

#define A_P11PIN    A_BASE+0x1B
#define A_P10PIN    A_BASE+0x1A
#define A_P9PIN 	A_BASE+0x19
#define A_P8PIN 	A_BASE+0x18
#define A_P7PIN 	A_BASE+0x17
#define A_P6PIN 	A_BASE+0x16
#define A_P5PIN 	A_BASE+0x15
#define A_P4PIN 	A_BASE+0x14

#define A_P1CTRL 	A_BASE+0x0F
#define A_P0CTRL 	A_BASE+0x0E

#define A_P3PIN 	A_BASE+0x0D
#define A_P2PIN 	A_BASE+0x0C
#define A_P1PIN 	A_BASE+0x0B
#define A_P0PIN 	A_BASE+0x0A
#define A_PCDATA 	A_BASE+0x09
#define A_PCADDR 	A_BASE+0x08
#define A_PGDR 		A_BASE+0x07
#define A_PSTAT 	A_BASE+0x06
#define A_PIC 		A_BASE+0x05
	#define DACCIE 	(0x01<<0)
#define A_POE 		A_BASE+0x04
	#define P5EN 	(0x01<<17)
	#define P4EN 	(0x01<<16)
	#define P3EN 	(0x01<<9)
	#define P2EN 	(0x01<<8)
	#define P1EN 	(0x01<<1)
	#define P0EN 	(0x01<<0)
	
#define A_PGCTRL1 	A_BASE+0x02
	#define NUMLEVELS_768 	(0x2FF<<8)
	#define NUMLEVELS_576 	(0x23F<<8)
	#define NUMLEVELS_384	  (0x17F<<8)
	#define INTPBR_1X	(0<<4)
	#define INTPBR_2X	(1<<4)
	#define INTPBR_3X	(2<<4)
	#define INTPBR_4X	(3<<4)
	#define INTPFR_1X	(0<<2)
	#define INTPFR_2X	(1<<2)
	#define INTPFR_3X	(2<<2)
	#define INTPFR_4X	(3<<2)
#define A_PGCTRL0 	A_BASE+0x01
#define A_PDATA 	A_BASE+0x00

// -+-+- Configuration Data -+-+-
#define C_P1CTRL 	0x000000
#define C_P0CTRL 	0x000000


#define C_P11PIN0 	0x000600
#define C_P10PIN0 	0x000600
#define C_P9PIN0 	0x000600
#define C_P8PIN0 	0x000600
#define C_P7PIN0 	0x000600
#define C_P6PIN0 	0x000600
#define C_P5PIN0 	0x000600
#define C_P4PIN0 	0x000600
#define C_P3PIN0 	0x000600
#define C_P2PIN0 	0x000600
#define C_P1PIN0 	0x000600
#define C_P0PIN0 	0x000600

#define C_POE 		0x000003 //if out pure duty 50-50	0x000033
//#define C_PGCTRL1 	0x36ff9c // 0x357f9c
#define C_PGCTRL1 	0x340080 //TEST
//#define C_PGCTRL0 	0x0506fe // 0x0540fe // LR output cross LRCK Freq.
#define C_PGCTRL0 	0xf542fe // David
#define C_PGCTRL0_PEN 	0xf542ff // David

//#define FSI		   48000             //Hz, input sample rate. Valid Range: 32000, 44100, 48000, 64000, 88200, 96000 
#define FSPWM		   384000            //Hz, Valid Range: 352800 for FSI
//#define NUMLEVELS        768   	     //768,576, 384
#define CLOCKING           0

/*
// Sampling rate
#define FS_16K                   0x00
#define FS_22K                   0x01
#define FS_24K                   0x02
#define FS_32K                   0x03
#define FS_44K                   0x04
#define FS_48K                   0x05
#define FS_64K                   0x06
#define FS_88K                   0x07
#define FS_96K                   0x08
#define FS_176K                 0x09
#define FS_192K                 0x0A
*/

/* ADC Mux Control */

#define MT5387_REG_INT_SDADC0        0x0240
#define MT5387_REG_INT_SDADC1	      0x0244

#define MT5387_REG_INT_SDADC2          0x0248
  //  #define ADC_ENVRN           (0x1 << 6)
    #define MT5387_ADC_ENVRP           (0x1 << 7)
    #define MT5387_ADC_GAIN_MASK       (0x7 << 8)
    #define MT5387_ADC_GAIN_N6DB       (0x6 << 8)
    #define MT5387_ADC_LEFT_PD          (0x1 << 11)
    #define MT5387_ADC_RIGHT_PD          (0x1 << 17)

    /* ADC left channel input mixer control bit */
    #define  MT5387_INT_ADC_INPUT_MUX_MASK (0x7  << 20)
    #define MT5387_INT_ADC_IN0L0R      (0x0  << 20)
    #define MT5387_INT_ADC_IN1L1R      (0x1  << 20)
    #define MT5387_INT_ADC_IN2L2R      (0x2  << 20)
    #define MT5387_INT_ADC_IN3L3R      (0x3  << 20)
    #define MT5387_INT_ADC_IN4L4R      (0x4  << 20)
    #define MT5387_INT_ADC_IN5L5R      (0x5  << 20)
    #define MT5387_INT_ADC_IN6L6R      (0x6  << 20)
    #define MT5387_INT_ADC_IN7L7R      (0x7  << 20)
    #define MT5387_ADC_L_PD            (0x1 << 12)        
    #define MT5387_ADC_R_PD            (0x1 << 13)

#define MT5387_REG_INT_SDADC3                  0x024c
   #define MT5387_GPIO_EN                (0xff << 0)

#define REG_IDACHCG             0x0204
    #define DAC0_SEL_POS        (0)
    #define DAC0_SEL_MASK       (0x7U << DAC0_SEL_POS)
    #define DAC0_SEL_0_LR       (0x0U << DAC0_SEL_POS)
    #define DAC0_SEL_0_SLR      (0x1U << DAC0_SEL_POS)
    #define DAC0_SEL_0_CLFE     (0x2U << DAC0_SEL_POS)
    #define DAC0_SEL_0_CH78     (0x3U << DAC0_SEL_POS)
    #define DAC0_SEL_0_CH90     (0x4U << DAC0_SEL_POS)
    #define DAC0_SEL_1_LR       (0x5U << DAC0_SEL_POS)
    #define DAC0_SEL_1_SLR      (0x6U << DAC0_SEL_POS)
    #define DAC0_SEL_1_CLFE     (0x7U << DAC0_SEL_POS)
    #define DAC1_SEL_POS        (3)
    #define DAC1_SEL_MASK       (0x7U << DAC1_SEL_POS)
    #define DAC1_SEL_0_LR       (0x0U << DAC1_SEL_POS)
    #define DAC1_SEL_0_SLR	    (0x1U << DAC1_SEL_POS)
    #define DAC1_SEL_0_CLFE     (0x2U << DAC1_SEL_POS)
    #define DAC1_SEL_0_CH78     (0x3U << DAC1_SEL_POS)
    #define DAC1_SEL_0_CH90     (0x4U << DAC1_SEL_POS)
    #define DAC1_SEL_1_LR       (0x5U << DAC1_SEL_POS)
    #define DAC1_SEL_1_SLR	    (0x6U << DAC1_SEL_POS)
    #define DAC1_SEL_1_CLFE     (0x7U << DAC1_SEL_POS)
    #define DAC2_SEL_POS        (6)     
    #define DAC2_SEL_MASK       (0x7U << DAC2_SEL_POS)
    #define DAC2_SEL_0_LR       (0x0U << DAC2_SEL_POS)
    #define DAC2_SEL_0_SLR      (0x1U << DAC2_SEL_POS)
    #define DAC2_SEL_0_CLFE     (0x2U << DAC2_SEL_POS)
    #define DAC2_SEL_0_CH78     (0x3U << DAC2_SEL_POS)
    #define DAC2_SEL_0_CH90     (0x4U << DAC2_SEL_POS)
    #define DAC2_SEL_1_LR       (0x5U << DAC2_SEL_POS)
    #define DAC2_SEL_1_SLR      (0x6U << DAC2_SEL_POS)
    #define DAC2_SEL_1_CLFE     (0x7U << DAC2_SEL_POS)	
   
//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#if 0
#define dWritePWMAddr(dAddr)  *(volatile DWRD *)(IO_BASE_ADDRESS + REG_INDIRECT0_ADDR) = dAddr;\
	                            *(volatile DWRD *)(IO_BASE_ADDRESS + REG_INDIRECT1_ADDR) = dAddr;

#define dReadPWMAddr(dAddr) *(volatile DWRD *)(IO_BASE_ADDRESS + REG_INDIRECT0_ADDR)

#define dWritePWMDAC( dVal)  *(volatile DWRD *)(IO_BASE_ADDRESS + REG_INDIRECT0_DATA) = dVal;\
                             *(volatile DWRD *)(IO_BASE_ADDRESS + REG_INDIRECT1_DATA) = dVal;


#define dReadPWMDAC() *(volatile DWRD *)(IO_BASE_ADDRESS + REG_INDIRECT0_DATA)

#define PWMDAC_WRITE32(offset, value) IO_WRITE32(IO_BASE_ADDRESS, (offset), (value))
#endif
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern CODEC_T* _CODEC_GetMT5387Tbl(void);

void _MT5387_WRITE(UINT8 u1Addr, UINT8 u1Data);
UINT8 _MT5387_READ(UINT8 u1Addr) ;
void _MT5387_Init(void);
void _MT5387_Enable(BOOL fgEnalbe);
void _MT5387_Mute(UINT8 u1DacId,BOOL fgMute);
void _MT5387_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT5387_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT5387_ChlSel(UINT8 u1Indx);
void _MT5387_HpVolCtl(UINT8 u1HpId, INT16 i2Value);
void _MT5387_SetAdcGain(UINT8 u1DecId, INT16 i2Value);
void _MT5387_PLLReset(void);
extern void _MT5387_ADCDOWN(BOOL fgEnalbe);
extern void _MT5387_PWM_SetSamplingRate(UINT8 u1ClkFmt);
void _MT5387_MuteCLK(UINT8 u1DacId,BOOL fgMute);
void _MT5387Internal_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);

#endif /* CODEC_MT5387_H */


