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
 * $RCSfile: codec_mt536xinternal.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_mt536xinternal.h
 *  Brief of file codec_mt536xinternal.h. \n
 *  Note that MT536X support internal AD and 4-channel interanl DAC. This file define the 
 *  internal AD and ADAC control registers.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define REG_INT_DACCTL0         0x01c0
    #define SDATA_SEL_S0_MASK   (0x3U << 0)
    #define SDATA_SEL_S0_DAC0   (0x0U << 0)
    #define FIX_EN_0            (0x1U << 4)
    #define OSR_0_MASK          (0x3U << 12)
    #define OSR_0_192K          (0x0U << 12)
    #define OSR_0_96K           (0x01U << 12)
    #define OSR_0_OTHER         (0x02U << 12)
    #define DWA_EN_0            (0x1U << 15)
    #define PREE_SEL_0_MASK     (0x7U << 16)
    #define PREE_SEL_0_cfg2     (0x2U << 16)    
    #define GAIN_3OVER4_0       (0x1U << 23)

#define REG_INT_DACCTL1         0x01c4
    #define DITH_MODE_0_MASK    (0x3U << 20)
    #define DITH_MODE_0_EN      (0x1U << 20)
    #define MODOUT_INV_0        (0x1U << 22)
    
#define REG_INT_DACCTL2         0x01c8
    #define SDATA_SEL_S1_MASK   (0x3U << 0)
    #define SDATA_SEL_S1_DAC1   (0x1U << 0)
    #define FIX_EN_1            (0x1U << 4)
    #define OSR_1_MASK          (0x3U << 12)
    #define OSR_1_192K          (0x0U << 12)
    #define OSR_1_96K           (0x01U << 12)
    #define OSR_1_OTHER         (0x02U << 12)	
    #define DWA_EN_1            (0x1U << 15)    
    #define PREE_SEL_1_MASK     (0x7U << 16)
    #define PREE_SEL_1_cfg2     (0x2U << 16)
    #define GAIN_3OVER4_1       (0x1U << 23)    

#define REG_INT_DACCTL3         0x01cc	
    #define DITH_MODE_1_MASK    (0x3U << 20)
    #define DITH_MODE_1_EN      (0x1U << 20)
    #define MODOUT_INV_1        (0x1U << 22)	

#define REG_INT_DACCTL4         0x01d0
    #define SDATA_SEL_S2_MASK   (0x3U << 0)
    #define SDATA_SEL_S2_DAC2   (0x2U << 0)
    #define FIX_EN_2            (0x1U << 4)
    #define OSR_2_MASK          (0x3U << 12)
    #define OSR_2_192K          (0x0U << 12)
    #define OSR_2_96K           (0x01U << 12)
    #define OSR_2_OTHER         (0x02U << 12)
    #define DWA_EN_2            (0x1U << 15)
    #define PREE_SEL_2_MASK     (0x7U << 16)
    #define PREE_SEL_2_cfg2     (0x2U << 16)
    #define GAIN_3OVER4_2       (0x1U << 23)     

#define REG_INT_DACCTL5         0x01d4	
    #define DITH_MODE_2_MASK    (0x3U << 20)
    #define DITH_MODE_2_EN      (0x1U << 20)
    #define MODOUT_INV_2        (0x1U << 22)	    

#define REG_INT_DACCTL6         0x01d8
    #define DECODER_SEL_0       (0x1U << 4)
    #define DECODER_SEL_1       (0x1U << 5)
    #define DECODER_SEL_2       (0x1U << 6)    
    #define ADAC_MUTE_RAMP      (0x1U << 16)
    #define DIS_UNM_RAMP        (0x1U << 18)
    #define TOTAL_COEFF_BITS_MASK   (0x0fU << 20)
    #define TOTAL_COEFF_BITS_1024_SAMPLE (0x0aU << 20)
    
#define REG_INT_DACCTL8         0x01e0
    #define CLK_RESET0          (0x1U << 0)
    #define CLK_RESET1          (0x1U << 1)
    #define CLK_RESET2          (0x1U << 2)
    #define PD_CTL_MASK         (0x77U << 4)
    #define PD_ON               (0x0U << 4)
    #define PD_DOWN             (0x77 << 4)
    #define PD_CHR_0            (0x1U << 4)
    #define PD_CHR_1            (0x1U << 5)  
    #define PD_CHR_2            (0x1U << 6)    
    #define PD_CHL_0            (0x1U << 8)	
    #define PD_CHL_1            (0x1U << 9)    
    #define PD_CHL_2            (0x1U << 10)
    #define PD_TEST             (0x1U << 11)   
    #define PD_REF0             (0x1U << 12)
    #define PD_REF1             (0x1U << 13)
    #define PD_REF2             (0x1U << 14)     
    #define PD_ISOURCE          (0x1U << 15)

#define REG_INT_DACCTL9         0x01e4
    #define VTESTEN             (0x1U << 11)
    #define VREFRAIL            (0x1U << 15)
    #define ADAC_MUTE           (0x1U << 23)    

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
	
#define REG_INT_SDADC0          0x0240
    #define EN	                (0x1U << 24)
    #define CHPD                (0x1U << 20)
    #define APLL_SEL            (0x1U << 1)
    #define RST                 (0x1U << 0)
#define REG_INT_SDADC1	        0x0244
    #define SHIFT_MASK          (0x7U << 28)
    #define SHIFT_HALF          (0x0U << 28)
    #define SHIFT_NO            (0x1U << 28)
    #define SHIFT_1BIT          (0x2U << 28)
    #define SHIFT_2BIT          (0x3U << 28)
    #define SHIFT_3BIT          (0x4U << 28)
    #define SHIFT_4BIT          (0x5U << 28)
    #define SHIFT_5BIT          (0x6U << 28)
    #define SHIFT_6BIT          (0x7U << 28)
    #define FACTOR_MASK         (0x3ffffffU << 0)
    #define FACTOR              (0x0a30000U << 0)    // MT5360A/MT530B 
    #define FACTOR1            (0x2B80000U << 0)    // MT5360A/MT530B 
    
	
/* ADC Mux Control */
#define REG_INT_SDADC2          0x0248
    #define ADC_ENVRN           (0x1U << 6)
    #define ADC_ENVRP           (0x1U << 7)
    #define ADC_GAIN_MASK       (0x7U << 8)
    #define ADC_GAIN_N6DB       (0x6U << 8)
    /* ADC left channel input mixer control bit */
    #define  INT_ADC_INPUT_MUX_MASK (0x7U  << 23)
    #define INT_ADC_IN0L0R      (0x0U  << 23)
    #define INT_ADC_IN1L1R      (0x1U  << 23)
    #define INT_ADC_IN2L2R      (0x2U  << 23)
    #define INT_ADC_IN3L3R      (0x3U  << 23)
    #define INT_ADC_IN4L4R      (0x4U  << 23)
    #define INT_ADC_IN5L5R      (0x5U  << 23)
    #define INT_ADC_IN6L6R      (0x6U  << 23)
    #define INT_ADC_IN7L7R      (0x7U  << 23)
    #define ADC_L_PD            (0x1U << 26)        
    #define ADC_R_PD            (0x1U << 27)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern CODEC_T * _CODEC_GetMT536xInternalTbl(void);
extern void _MT536xInternal_Init(void);
extern void _MT536xInternal_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT536xInternal_DacBCKSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT536xInternal_ChlSel(UINT8 u1Indx);
extern void _MT536xInternal_Mute(UINT8 u1DacId,BOOL fgEnable);
extern void _MT536xInternal_SetAdcGain(UINT8 u1DecId, INT16 i2Value);
#ifdef CC_DYNAMIC_POWER_ONOFF
extern void _MT536xInternalAdcPowerDown(BOOL fgEnable);
#endif

