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
 * $RCSfile: codec_mt538xinternal.h,v $
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
// Constant definitions
//---------------------------------------------------------------------------

#define REG_INT_DACCTL0	0x01c0
	#define DIG_MUTE_0		(0x1 << 24)
	#define OSR_0_MASK	(0x3 << 22)
	#define OSR_0		(0x0 << 22)
	#define DITH_MODE_0_MASK (0x3 << 20)
	#define DITH_MODE_0_EN   (0x1 << 20)
#define REG_INT_DACCTL1	0x01c4
	#define DWA_EN_0	(0x1 << 0)
	#define PREE_SEL_0_MASK	(0x7 << 4)
	#define PREE_SEL_0_cfg2		(0x2 << 4)
	#define MODOUT_INV_0    (0x1 << 14)
#define REG_INT_DACCTL2	0x01c8
	#define FIX_EN_1		(0x1 << 30)
	#define FIX_EN_0		(0x1 << 28)
	#define DIG_MUTE_1		(0x1 << 24)
	#define OSR_1_MASK	(0x3 << 22)
	#define OSR_1		(0x0 << 22)	
	#define DITH_MODE_1_MASK (0x3 << 20)
	#define DITH_MODE_1_EN   (0x1 << 20)
#define REG_INT_DACCTL3	0x01cc	
	#define DWA_EN_1	(0x1 << 0)
	#define PREE_SEL_1_MASK	(0x7 << 4)
	#define PREE_SEL_1_cfg2		(0x2 << 4)
	#define MODOUT_INV_1    (0x1 << 14)	
#define REG_INT_DACCTL4	0x01d0
	#define PD_CTL_MASK (0xf << 4)
	#define PD_ON		(0x0 << 4)
	#define PD_DOWN	(0xf << 4)
	#define PD_CHR_2	(0x1 << 7)
	#define PD_CHL_2	(0x1 << 6)
	#define PD_CHR_1	(0x1 << 5)
	#define PD_CHL_1	(0x1 << 4)
	#define PD_ISOURCE	(0x1 << 3)
	#define PD_TEST		(0x1 << 2)
	#define PD_REF2		(0x1 << 1)
	#define PD_REF1		(0x1 << 0)
    #define CLK_RESET1  (0x1 << 10)
	#define CLK_RESET2  (0x1 << 11)
#define REG_DACCTL5			0x01d4
	#define VREFRAIL		(0x1 << 0)
    #define ADAC_MUTE       (0x1 << 14)
#define REG_DACCTL6			0x01d8
       #define ADAC_AUX_APLL1_SEL      (0x1U<< 31)    
	#define GAIN_3OVER4_1		(0x1 << 13)
	#define GAIN_3OVER4_0		(0x1 << 12)
#define REG_DACCTL8         0x01e0
//    #define MOD_RST_STG_MASK    (0xf << 12)
//    #define MOD_RST_STG         (0x3 << 12)

#define REG_IDACHCG		0x0204
	#define DAC0_SEL_MASK	(0x7 << 0)
	#define DAC0_SEL_0_LR		(0x0 << 0)
	#define DAC0_SEL_0_SLR		(0x1 << 0)
	#define DAC0_SEL_0_CLFE	(0x2 << 0)
	#define DAC0_SEL_0_CH78	(0x3 << 0)
	#define DAC0_SEL_0_CH90	(0x4 << 0)
	#define DAC0_SEL_1_LR		(0x5 << 0)
	#define DAC0_SEL_1_SLR		(0x6 << 0)
	#define DAC0_SEL_1_CLFE	(0x7 << 0)
	#define DAC1_SEL_MASK	(0x7 << 3)
	#define DAC1_SEL_0_LR		(0x0 << 3)
	#define DAC1_SEL_0_SLR		(0x1 << 3)
	#define DAC1_SEL_0_CLFE	(0x2 << 3)
	#define DAC1_SEL_0_CH78	(0x3 << 3)
	#define DAC1_SEL_0_CH90	(0x4 << 3)
	#define DAC1_SEL_1_LR		(0x5 << 3)
	#define DAC1_SEL_1_SLR		(0x6 << 3)
	#define DAC1_SEL_1_CLFE	(0x7 << 3)

	
#define REG_INT_SDADC0        0x0240
	#define EN			(0x1 << 24)
	#define CHPD			(0x1 << 20)
	#define APLL_SEL		(0x1 << 1)
	#define RST			(0x1 << 0)
#define REG_INT_SDADC1	      0x0244
	#define SHIFT_MASK	(0x7 << 28)
	#define SHIFT_HALF	(0x0 << 28)
	#define SHIFT_NO	(0x1 << 28)
	#define SHIFT_1BIT	(0x2 << 28)
	#define SHIFT_2BIT	(0x3 << 28)
	#define SHIFT_3BIT	(0x4 << 28)
	#define SHIFT_4BIT	(0x5 << 28)
	#define SHIFT_5BIT	(0x6 << 28)
	#define SHIFT_6BIT	(0x7 << 28)
	#define FACTOR_MASK (0x3ffffff << 0)
	#define FACTOR		(0x1a30000 << 0)
	#define FACTOR1		(0x06effff << 0)
#ifdef CC_AUD_APGA_2_5V	
	#define FACTOR2		(0x1630000 << 0)            // Update to 2.5Vrms
#endif
	
/* ADC Mux Control */
#define REG_INT_SDADC2        0x0248
	#define ADC_R_PD			(0x1 << 27)
	#define ADC_L_PD			(0x1 << 26)
        /* ADC left channel input mixer control bit */
        #define  INT_ADC_INPUT_MUX_MASK	(0x7  << 23)
                 #define INT_ADC_IN0L0R              	(0x0  << 23)
                 #define INT_ADC_IN1L1R              	(0x1  << 23)
                 #define INT_ADC_IN2L2R                 	(0x2  << 23)
                 #define INT_ADC_IN3L3R                 	(0x3  << 23)
                 #define INT_ADC_IN4L4R                 	(0x4  << 23)
                 #define INT_ADC_IN5L5R                 	(0x5  << 23)
                 #define INT_ADC_IN6L6R                 	(0x6  << 23)
                 #define INT_ADC_IN7L7R                 	(0x7  << 23)
                 

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern CODEC_T * _CODEC_GetMT538xInternalTbl(void);
extern void _MT538xInternal_Init(void);
extern void _MT538xInternal_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT538xInternal_ChlSel(UINT8 u1Indx);
extern void _MT538xInternal_Mute(UINT8 u1DacId,BOOL fgEnable);
extern void _MT538xInternal_SetAdcGain(UINT8 u1DecId, INT16 i2Value);

