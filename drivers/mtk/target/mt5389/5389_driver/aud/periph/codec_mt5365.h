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
 * $Author: kai.wang $
 * $Date: 2012/09/18 $
 * $RCSfile: codec_mt5365.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_mt5365.h
 *  Brief of file codec_mt5365.h. \n
 */

#ifndef CODEC_MT5365_H
#define CODEC_MT5365_H

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

// -+-+- Memory Address Map -+-+-
#define REG_INDIRECT0_ADDR 		0x800
#define REG_INDIRECT0_DATA 		0x804
#define REG_INDIRECT1_ADDR 		0x840
#define REG_INDIRECT1_DATA 		0x844

#define REG_PWMCFG0_PG1                     0x1C8
#define REG_PWMCFG0_PG2                     0x1CC
  #define SWAP_LR                               (0x01<< 23) // This bit is used to swap Left/Right channel before pwmIP uses it
  #define UPDATE_NUM_0                          (0x00<< 16) // The timing for latching audio data input 
  #define UPDATE_NUM_1                          (0x01<< 16) // The timing for latching audio data input 
  #define UPDATE_NUM_2                          (0x02<< 16) // The timing for latching audio data input 
  #define GHRESETZ                              (0x01<< 15) // Softreset bit (active low) for PWM G1/G2
  #define WRITE_DMAP_DATA_EN                    (0x01<< 12) // Feed audio sample data by ADSP
  #define PWM_ADAC_MUTE                         (0x01<< 8)
  #define ADAC_SWAP_LR                          (0x01<< 7)  // input data swapping
  #define CR_MODE                               (0x01<< 1)  // Clock Ratio Mode in top (test only), PWM controller assumes that LJCLK = DCLK
  #define LINE_IN_BYPASS                        (0x01<< 0)  // input data from line-in mode

#define REG_PWMIP_PINMUX_PG1                0x1D4
#define REG_PWMIP_PINMUX_PG2                0x1D8
#define REG_PWMIP_MON_PG1                   0x1DC
#define REG_PWMIP_BTLMODE_PG2               0x1E0
  #define EN_GROUP_APLL2                        (0x01<< 30)
#define REG_PWMIP_GETIDA_PG1                0x1E4
#define REG_PWMIP_GETIDD_PG1                0x1E8
#if defined(CC_MT5365) || defined(CC_MT5395)
#define REG_PWMIP_PWMPAD_CFG                0x1EC
#endif
#define REG_PWMIP_BTLMODE_PG1               0x1F0
  #define PWMBTLMODE                            0x44C00003  // by bigfox class-d related
  #define PWMBTLMODE_PG2                        0x04C00003  // bit30 = 0, follow apll1

#define REG_PWMIP_MISC0_PG1                 0x1F4 // Ramping for G1
#define REG_PWMIP_MISC0_PG2                 0x1F8 // Ramping for G2
  #define RAMP_SPEED_MIN                        ((UINT32)(0x3F))<<26  // Suggested
  #define RAMP_SPEED_MAX                        (0x01)<<26  // Fastest, not suggested
  #define RAMP_STEP_MIN                         (0x01<<8)  // Suggested (smallest)
  #define RAMP_EN_ALL                           (0x3F<<0)  // Enable all cheannels ramp mode 
  #define RAMP_DIS_ALL                          (0x00<<0)  // Enable all cheannels ramp mode 
  #define RAMP_EN_CH0                           (0x01<<0)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH1                           (0x01<<1)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH2                           (0x01<<2)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH3                           (0x01<<3)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH4                           (0x01<<4)  // Enable  cheannel 0 ramp mode 
  #define RAMP_EN_CH5                           (0x01<<5)   // Enable  cheannel 0 ramp mode 

#define REG_PWMIP_MISC1                   0x1FC
  #define PWMIP_RAMP_OFF                         0x00003fff
  #define PWMIP_RAMP_ON                          0x00000fff
  #define RAMP_UP_AUTO_CLOSE                    (0x01<<13)   
  #define RAMP_UP_EN                            (0x01<<12)   

#define A_P11PIN    0xF1B
#define A_P10PIN    0xF1A
#define A_P9PIN 	0xF19
#define A_P8PIN 	0xF18
#define A_P7PIN 	0xF17
#define A_P6PIN 	0xF16
#define A_P5PIN 	0xF15
#define A_P4PIN 	0xF14

#define A_P1CTRL 	0xF0F
#define A_P0CTRL 	0xF0E

#define A_P3PIN 	0xF0D
#define A_P2PIN 	0xF0C
#define A_P1PIN 	0xF0B
#define A_P0PIN 	0xF0A
#define A_PCDATA 	0xF09
#define A_PCADDR 	0xF08
#define A_PGDR 		0xF07
#define A_PSTAT 	0xF06
#define A_PIC 		0xF05
	#define DACCIE 	(0x01<<0)
#define A_POE 		0xF04
	#define P5EN 	(0x01<<17)
	#define P4EN 	(0x01<<16)
	#define P3EN 	(0x01<<9)
	#define P2EN 	(0x01<<8)
	#define P1EN 	(0x01<<1)
	#define P0EN 	(0x01<<0)
	
#define A_PGCTRL1 	0xF02
        #define NUMLEVELS_1000  (0x3E7<<8)
        #define NUMLEVELS_750 	(0x2ED<<8)
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
#define A_PGCTRL0 	0xF01
#define A_PDATA 	0xF00

// -+-+- Configuration Data -+-+-
#define C_P1CTRL 	0x000000
#define C_P0CTRL 	0x000000

///PWM PG2
#define C_P11PIN0 	0x000500
#define C_P10PIN0 	0x000600
#define C_P9PIN0 	0x000500
#define C_P8PIN0 	0x00060A    //For Ch4&5 SNR
#define C_P7PIN0 	0x000500
#define C_P6PIN0 	0x000600    //For SNR ///0x000500  //Line out phase is inversed.
///PWM PG1
#define C_P5PIN0 	0x000500
#define C_P4PIN0 	0x00060A    //For Ch2&3 SNR
#define C_P3PIN0 	0x000500
#define C_P2PIN0 	0x000600
#define C_P1PIN0 	0x000500
#define C_P0PIN0 	0x00060A    //For Ch0&1 SNR ///0x000500

#define C_POE 		0x000003 //if out pure duty 50-50	0x000033
//#define C_PGCTRL1 	0x36ff9c // 0x357f9c
#define C_PGCTRL1 	0x340080 //TEST
//#define C_PGCTRL0 	0x0506fe // 0x0540fe // LR output cross LRCK Freq.

#define C_PGCTRL0       0xF542FE
#define C_PGCTRL0_PEN 	0xF542FF

//#define FSI		   48000             //Hz, input sample rate. Valid Range: 32000, 44100, 48000, 64000, 88200, 96000 
#define FSPWM		   384000            //Hz, Valid Range: 352800 for FSI
//#define NUMLEVELS        768   	     //768,576, 384
#define CLOCKING           0

///////// PWM constant definitions////////   
/// 0xD244[3:0], 0xD248[3:0]
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#define APLL_D1             1
#define APLL2_D1            4
#define SAWLESSPLL_D1       7
#elif defined(CC_MT5365) || defined(CC_MT5395)
#define APLL_D1             1
#define APLL2_D1            6
#define SAWLESSPLL_D1       11
#endif

#ifdef CC_AUD_PWM_SAWLESS_PLL_EN
#define PWMPAD_CFG_INIT         0xFF00FF        ///Sawless + Clock Relatch 

#define PGCTRL1_INIT            (C_PGCTRL1 | NUMLEVELS_750 | INTPBR_3X | INTPFR_4X) ///PWM Reg 0xF02, Bigfox suggest
#define PGCTRL1_INIT_432        (C_PGCTRL1 | NUMLEVELS_1000 | INTPBR_3X | INTPFR_3X)
    
#define PWMIP_MON_PG1_INIT      (0x3FF << 16)   /// 0x51DC

#define INTPBR_48K              INTPBR_3X       ///PWM Reg 0xF02[5:4]
#define PWM_CK_SEL              SAWLESSPLL_D1   ///0xD244[3:0]
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    ///Reduce pwm digital gain for external gain 2.11X (for internal LDO use)
    #define PWM_DIGITAL_INTERNALLDO_GAIN        0x0A8000        ///Check with Kevin Lo  // for 68/96/89 open internal LDO
    #define PWM_DIGITAL_INTERNALLDO_GAIN_N      0xF58000

    #define PWM_DIGITAL_GAIN        0x0B8000        //for 68/96/89 close internal LDO
    #define PWM_DIGITAL_GAIN_N      0xF48000

    #else
    #define PWM_DIGITAL_GAIN        0x0D8000        ///PWM YRAM 0x115, 0x146~14A  //for 65/66/95 without internal LDO
    #define PWM_DIGITAL_GAIN_N      0xF28000
    #endif
#else
#define PWMPAD_CFG_INIT         0x0000FF        ///APLL1 + Sel APLL + APLL non-inverse + Clock Relatch 
#define PGCTRL1_INIT            (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_4X)
#define PWMIP_MON_PG1_INIT      (0x3FF << 16)   /// 0x51DC

#define INTPBR_48K              INTPBR_2X       ///PWM Reg 0xF02
#define PWM_CK_SEL              APLL_D1         ///0xD244[3:0]
#define PWM_DIGITAL_GAIN        0x0F0600        ///PWM YRAM 0x115, 0x146~14A
#define PWM_DIGITAL_GAIN_N      0xF0FA00
#endif

///////// GPIO constant definition for AIN0~AIN6////////  
#if defined(CC_MT5396)
#define GPIO_AIN0_L 170
#define GPIO_AIN0_R 169
#define GPIO_AIN1_L 172
#define GPIO_AIN1_R 171
#define GPIO_AIN2_L 174
#define GPIO_AIN2_R 173
#define GPIO_AIN3_L 176
#define GPIO_AIN3_R 175
#define GPIO_AIN4_L 178
#define GPIO_AIN4_R 177
#define GPIO_AIN5_L 180
#define GPIO_AIN5_R 179
#define GPIO_AIN6_L 182
#define GPIO_AIN6_R 181
#elif defined(CC_MT5368)
#define GPIO_AIN0_L 94
#define GPIO_AIN0_R 93
#define GPIO_AIN1_L 97
#define GPIO_AIN1_R 95
#define GPIO_AIN2_L 98
#define GPIO_AIN2_R 96
#define GPIO_AIN3_L 101
#define GPIO_AIN3_R 99
#define GPIO_AIN4_L 102
#define GPIO_AIN4_R 100
#define GPIO_AIN5_L 105
#define GPIO_AIN5_R 103
#define GPIO_AIN6_L 106
#define GPIO_AIN6_R 104
#elif defined(CC_MT5389)
//TODO
#define GPIO_AIN1_L 48
#define GPIO_AIN1_R 46
#define GPIO_AIN2_L 49
#define GPIO_AIN2_R 47
#elif defined(CC_MT5395)
#define GPIO_AIN0_L 108
#define GPIO_AIN0_R 109
#define GPIO_AIN1_L 110
#define GPIO_AIN1_R 111
#define GPIO_AIN2_L 112
#define GPIO_AIN2_R 113
#define GPIO_AIN3_L 114
#define GPIO_AIN3_R 115
#define GPIO_AIN4_L 116
#define GPIO_AIN4_R 117
#define GPIO_AIN5_L 118
#define GPIO_AIN5_R 119
#define GPIO_AIN6_L 120
#define GPIO_AIN6_R 121
#elif defined(CC_MT5365)
#define GPIO_AIN0_L 106
#define GPIO_AIN0_R 107
#define GPIO_AIN1_L 108
#define GPIO_AIN1_R 109
#define GPIO_AIN2_L 110
#define GPIO_AIN2_R 111
#define GPIO_AIN3_L 112
#define GPIO_AIN3_R 113
#define GPIO_AIN4_L 114
#define GPIO_AIN4_R 115
#define GPIO_AIN5_L 116
#define GPIO_AIN5_R 117
#define GPIO_AIN6_L 118
#define GPIO_AIN6_R 119
#endif

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern CODEC_T* _CODEC_GetMT5365Tbl(void);

void _MT5365_Init(void);
void _MT5365_Mute(UINT8 u1DacId,BOOL fgMute);
void _MT5365_MuteALRx(UINT8 u1ALRx,BOOL fgMute);
void _MT5365_InterPWMEnable(BOOL fgEnable);
void _MT5365_InterPWMGainSet(INT32 pgain);
UINT32 _MT5365_InterPWMGainGet(void);
void _MT5365_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT5365_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
void _MT5365_ChlSel(UINT8 u1Indx);
extern void _MT5365_PWM_SetSamplingRate(UINT8 u1ClkFmt);
extern void _MT5365_ADCDOWN(BOOL fgEnalbe);
extern void _MT5365_MuteCLK(UINT8 u1DacId,BOOL fgMute);
void _MT5365_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
void _MT5365_DacChlSelQuery(void);
void _MT5365_PopMute(BOOL fgMute);

#endif /* CODEC_MT5365_H */


