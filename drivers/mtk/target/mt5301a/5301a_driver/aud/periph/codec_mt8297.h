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
 * $RCSfile: codec_mt8297.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_mt538xinternal.h
 *  Brief of file codec_mt538xinternal.h. \n
 *  Note that MT538X support internal AD and 4-channel interanl DAC. This file define the 
 *  internal AD and ADAC control registers.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"

                 

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


extern CODEC_T * _CODEC_GetMT8297Tbl(void);
extern void _MT8297_SemaUnLock(void);
extern void _MT8297_SemaLock(void);
extern void _MT8297_Init(void);
extern void _MT8297_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT8297_ADC_PowerDown(BOOL fgEnable);

extern void _MT8297_DacBCKSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT8297_ChlSel(UINT8 u1Indx);
extern void _MT8297_Mute(UINT8 u1DacId,BOOL fgEnable);
extern void _MT8297_SetAdcGain(UINT8 u1DecId, INT16 i2Value);
extern void _MT8297_DirectWRITE(UINT8 u1Addr, UINT8 u1Data, UINT8 u1Msk);
extern UINT8 _MT8297_DirectREAD(UINT8 u1Addr);
extern void _MT8297_InDirectWRITE(UINT8 u1Addr, UINT8 u1Data, UINT8 u1Msk);
extern UINT8 _MT8297_InDirectREAD(UINT8 u1Addr);
extern void _MT8297_LRInvert(UINT8 u1DacId,BOOL fgInvert);
extern void _MT8297_DACFmt(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
extern void _MT8297_ADCFmt(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
extern void _MT8297_HpVolCtl(UINT8 u1HpId, INT16 i2Value);


/* ADC Mux Control */
#define INT_ADC_IN0L0R  (0x0  )
#define INT_ADC_IN1L1R  (0x1  )
#define INT_ADC_IN2L2R  (0x2  )
#define INT_ADC_IN3L3R  (0x3  )
#define INT_ADC_IN4L4R  (0x4  )
#define INT_ADC_IN5L5R  (0x5  )
#define INT_ADC_IN6L6R  (0x6  )
//ADC power down control
#define INT_ADC_POWERDOWN  (0x3<<3)
#define INT_ADC_POWERON  (0x0<<3)


#define REG_8297CH_CHCFG    0x208
    #define BCK1_MSK        (0x3 << 18)
    #define BCK0_MSK        (0x3 << 16)
    #define BCK1_SEL_0      (0x0 << 18)
    #define BCK1_SEL_1      (0x3 << 18)
    #define BCK0_SEL_0      (0x0 << 16)
    #define BCK0_SEL_1      (0x3 << 16)
    #define S0_CFG_0_MSK    (0x7 << 0)
    #define S0_CFG_0_LR     (0x0 << 0)
    #define S0_CFG_0_SLR    (0x1 << 0)
    #define S0_CFG_0_CS     (0x2 << 0)
    #define S0_CFG_0_78     (0x3 << 0)
    #define S0_CFG_0_90     (0x4 << 0)
    #define S1_CFG_0_MSK    (0x7 << 3)
    #define S1_CFG_0_LR     (0x0 << 3)
    #define S1_CFG_0_SLR    (0x1 << 3)
    #define S1_CFG_0_CS     (0x2 << 3)
    #define S1_CFG_0_78     (0x3 << 3)
    #define S1_CFG_0_90     (0x4 << 3)
    #define S2_CFG_0_MSK    (0x7 << 6)
    #define S2_CFG_0_LR     (0x0 << 6)
    #define S2_CFG_0_SLR    (0x1 << 6)
    #define S2_CFG_0_CS     (0x2 << 6)
    #define S2_CFG_0_78     (0x3 << 6)
    #define S2_CFG_0_90     (0x4 << 6)
    #define S3_CFG_0_MSK    (0x7 << 9)
    #define S3_CFG_0_LR     (0x0 << 9)
    #define S3_CFG_0_SLR    (0x1 << 9)  
    #define SD0_SEL         (0x1 << 12)
    #define SD1_SEL         (0x1 << 13)
    #define SD2_SEL         (0x1 << 14)

//BANK 00
#define ADAC_18              0x78
    #define DAC0_BCK_SEL_MSK            0x10
    #define DAC1_BCK_SEL_MSK            0x20
    #define DAC2_BCK_SEL_MSK            0x40
    #define DAC0_BCK_SEL_DEC0          0x00
    #define DAC0_BCK_SEL_DEC1          0x10
    #define DAC1_BCK_SEL_DEC0          0x00
    #define DAC1_BCK_SEL_DEC1          0x20
    #define DAC2_BCK_SEL_DEC0          0x00
    #define DAC2_BCK_SEL_DEC1          0x40
    
#define ADAC_1E         0x7E
    #define ADAC_MUTE_SET_MSK   (0x70)
    #define ADAC_MUTE_SET       (0x70)
    #define ADAC_UNMUTE_SET     (0x00)
    #define ADAC0_MUTE_MSK      (0x10)
    #define ADAC1_MUTE_MSK      (0x20)
    #define ADAC2_MUTE_MSK      (0x40)
    #define ADAC0_MUTE          (0x10)
    #define ADAC1_MUTE          (0x20)
    #define ADAC2_MUTE          (0x40)

#define ADC_I2S_CFG0    0x45
    #define ADC_I2S_FMT_MSK     (0x03)
    #define ADC_I2S_FMT_RJ      (0x00)
    #define ADC_I2S_FMT_LJ      (0x01)
    #define ADC_I2S_FMT_I2S     (0x02)
    #define LRCK_INV_MSK        (0x80)
    #define LRCK_INV            (0x80)

//BANK 01
#define ADAC_28         0x68
    #define ANALOG_MUTE_MSK     (0x80)
    #define ANALOG_MUTE         (0x80)
    #define ANALOG_UNMUTE       (0x00)
    #define LR_INVERT_MSK       (0x40)
    #define LR_INVERT           (0x40)

#define ADAC_00         0x60
#define ADAC_08         0x68
#define ADAC_10         0x70
    #define I2S_FMT_MSK         (0x03)
    #define I2S_FMT_RJ          (0x00)
    #define I2S_FMT_LJ          (0x01)
    #define I2S_FMT_I2S         (0x02)
    #define LR_INVERT_MSK       (0x40)
    #define LR_INVERT           (0x40)

#define ADAC_01         0x61
#define ADAC_09         0x69
#define ADAC_11         0x71
    #define LR_DIG_INVERT_MSK   (0x02)
    #define LR_DIG_INVERT       (0x02)

//BANK 02
#define ADAC_45         0x65
    #define HPUTE_MSK           (0x01)
    #define HPMUTE              (0x01)
    #define HPUNMUTE            (0x00)

