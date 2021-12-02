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
 * $RCSfile: codec_mt538xinternal.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_hw.c
 *  Brief of file adac_hw.c.
 *  Note that MT537X support 4-channel interanl DAC
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "x_util.h"
#include "aud_if.h"
#include "aud_hw.h"
#include "codec_mt538xinternal.h" // For environment register read & write
#include "drvcust_if.h"
#include "aud_debug.h"
#include "x_pinmux.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Macro 
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MT538xInternal_ADC_VOL_MAX  (4)
#define MT538xInternal_ADC_VOL_MIN  (-4)

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static CODEC_FUNCTBL_T _rCodecFunTbl = 
{
	_MT538xInternal_Init,			//pfnInit
	_MT538xInternal_Mute,			//pfnMute 
	NULL,  							//pfnSetAdcOutFmt;
   	NULL,      						//pfnSetDacInFmt;
   	NULL, 							//pfnSetDacGain;
   	NULL,							// pfnGetDacGain
   	_MT538xInternal_SetAdcGain,	// pfnSetAdcGain
	NULL,							// pfnGetAdcGain   
   	NULL,							// pfnSetHpGain
   	NULL,							// pfnGetHpGain
   	_MT538xInternal_ChlSel,			// pfnSetAdcMuxSel
   	NULL,							// pfnRegWrite
   	NULL							// pfnRegRead
};


static CODEC_T _rCodecMT538xInternal = 
{
	"MT538xInternal",				// szName[32];
	0,							// i2MaxHpGain
	0,							// i2MinHpGain
	0,							// i2MaxDacGain
	0,							// i2MinDacGain
	0,							// u1AdcMuxNum
	0,							// u1DacNum
	0,							// u1HpNum
	&_rCodecFunTbl				// rAdacFuncTbl
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _ADAC_InterDacInit
 *  Intial internal ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT538xInternal_Init(void)
{
	AUD_INTER_DAC_CFG_T* prInterDacUsage;
    BOOL fPhaseInverse;


    	//Enable AD
	AUD_SET_BIT(REG_INT_SDADC0, EN);
    	//Light: Bypass high pass filter to prevent from "po po" noises
    	//when analog input is floating. MT538x IC might have design defect.
    	AUD_SET_BIT(REG_INT_SDADC0, CHPD);
    	AUD_CLR_BIT(REG_INT_SDADC0, RST);
    	AUD_CLR_BIT(REG_INT_SDADC0,APLL_SEL);

#if 0
	if (BSP_GetIcVersion() >= IC_VER_5382P_AA)
	{
    		//Set AD gain
    		AUD_WRITE32(REG_INT_SDADC1, (AUD_READ32(REG_INT_SDADC1) & ~SHIFT_MASK) |SHIFT_2BIT );
    		AUD_WRITE32(REG_INT_SDADC1, (AUD_READ32(REG_INT_SDADC1) & ~FACTOR_MASK) |FACTOR1 );   	
	}
	else
	{
             //Set AD gain     
    		AUD_WRITE32(REG_INT_SDADC1, (AUD_READ32(REG_INT_SDADC1) & ~SHIFT_MASK) |SHIFT_1BIT );
#ifdef CC_AUD_APGA_2_5V
    		AUD_WRITE32(REG_INT_SDADC1, (AUD_READ32(REG_INT_SDADC1) & ~FACTOR_MASK) |FACTOR2 );
#else
             AUD_WRITE32(REG_INT_SDADC1, (AUD_READ32(REG_INT_SDADC1) & ~FACTOR_MASK) |FACTOR );   	
#endif
	}
#endif

	//Power on channel modulator
	// If loader doesn't power on it, then force power on
	if (AUD_READ32(REG_INT_SDADC2) & ADC_R_PD)
	{
    	AUD_CLR_BIT(REG_INT_SDADC2, ADC_R_PD);
	}
	if (AUD_READ32(REG_INT_SDADC2) & ADC_L_PD)
	{
    	AUD_CLR_BIT(REG_INT_SDADC2, ADC_L_PD);
	}	

    //Light: Reset MOD pipeline to prevent from "sasa" noises on internal DAC1
    //when changing sampling rate. MT538x IC might have design defect.
    AUD_WRITE32(REG_DACCTL8, (AUD_READ32(REG_DACCTL8) & (~MOD_RST_STG_MASK)) | MOD_RST_STG);
    
	//unmute DA...digital
	AUD_CLR_BIT(REG_INT_DACCTL0,DIG_MUTE_0);		// 1st
	AUD_CLR_BIT(REG_INT_DACCTL2,DIG_MUTE_1);		// 2ed
	//unmute DA...analog
       AUD_CLR_BIT(REG_DACCTL5,ADAC_MUTE);

	//Set OSR
	AUD_WRITE32(REG_INT_DACCTL0, (AUD_READ32(REG_INT_DACCTL0) & ~OSR_0_MASK) |OSR_0);	// 1st
	AUD_WRITE32(REG_INT_DACCTL2, (AUD_READ32(REG_INT_DACCTL2) & ~OSR_1_MASK) |OSR_1);	// 2ed	

	//Enable DWA
	AUD_SET_BIT(REG_INT_DACCTL1,DWA_EN_0);	// 1st
	AUD_SET_BIT(REG_INT_DACCTL3,DWA_EN_1);	// 2ed

	//Phase fix channel select and enable	
	AUD_SET_BIT(REG_INT_DACCTL1,MODOUT_INV_0);		// 1st
	AUD_SET_BIT(REG_INT_DACCTL3,MODOUT_INV_1);		// 2ed

    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL1AR1PhaseInverse);
    if (fPhaseInverse)
    {
    	AUD_CLR_BIT(REG_INT_DACCTL1,MODOUT_INV_0);		// 1st
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL2AR2PhaseInverse);
    if (fPhaseInverse)
    {
    	AUD_CLR_BIT(REG_INT_DACCTL3,MODOUT_INV_1);		// 2ed
    }


	//Channel power on
	// If loader doesn't power on it, then force power on
    if (AUD_READ32(REG_INT_DACCTL4) & PD_CTL_MASK)
    {
	    AUD_WRITE32(REG_INT_DACCTL4,(AUD_READ32(REG_INT_DACCTL4) & ~PD_CTL_MASK)|PD_ON);
    }    
    if (AUD_READ32(REG_INT_DACCTL4) & PD_ISOURCE)
    {
	    AUD_CLR_BIT(REG_INT_DACCTL4,PD_ISOURCE);
    }    
	if (AUD_READ32(REG_INT_DACCTL4) & PD_REF2)
	{
	    AUD_CLR_BIT(REG_INT_DACCTL4,PD_REF2);
	}    
	if (AUD_READ32(REG_INT_DACCTL4) & PD_REF1)
	{
	    AUD_CLR_BIT(REG_INT_DACCTL4,PD_REF1);
	}   

    // Reset channel clock 
    AUD_CLR_BIT(REG_INT_DACCTL4, CLK_RESET1);
	AUD_CLR_BIT(REG_INT_DACCTL4, CLK_RESET2);
	AUD_SET_BIT(REG_INT_DACCTL4, CLK_RESET1);
	AUD_SET_BIT(REG_INT_DACCTL4, CLK_RESET2);

	//Move to apll setting
	//ADAC_WRITE32(0x01e0,0x00100084);

	//Set Dither and the Dither operand, this is due to dc noise
	AUD_WRITE32(REG_INT_DACCTL0, (AUD_READ32(REG_INT_DACCTL0) & ~DITH_MODE_0_MASK) |DITH_MODE_0_EN);	// 1st	
	AUD_WRITE32(REG_INT_DACCTL1, (AUD_READ32(REG_INT_DACCTL1) & ~PREE_SEL_0_MASK) |PREE_SEL_0_cfg2);	// 1st	
	AUD_WRITE32(REG_INT_DACCTL2, (AUD_READ32(REG_INT_DACCTL2) & ~DITH_MODE_1_MASK) |DITH_MODE_1_EN);	// 2ed	
	AUD_WRITE32(REG_INT_DACCTL3, (AUD_READ32(REG_INT_DACCTL3) & ~PREE_SEL_1_MASK) |PREE_SEL_0_cfg2);	// 2ed	


	//Reference voltage
	AUD_SET_BIT(REG_DACCTL5,VREFRAIL);
	
       // Init 2nd LR ch APLL clock sel
       if(AUD_GetTargetCountry() == (UINT32)COUNTRY_EU)
           AUD_SET_BIT(REG_DACCTL6,ADAC_AUX_APLL1_SEL);           


	//Channel modulator output level
	AUD_SET_BIT(REG_DACCTL6,GAIN_3OVER4_1);
	AUD_SET_BIT(REG_DACCTL6,GAIN_3OVER4_0);

	//Set  DAC channel 
    	UNUSED(DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32 *)(void *)&prInterDacUsage));
    if (prInterDacUsage != NULL)
    {
       	_MT538xInternal_DacChlSel(AUD_DEC_MAIN,prInterDacUsage->eDac0ChlSel);
        	_MT538xInternal_DacChlSel(AUD_DEC_AUX,prInterDacUsage->eDac1ChlSel);
    }	
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _MT538xInternal_ChlSel
 *  Internal AADC channel select.
 *  Note that the 4 channel must be on the same decoder (cannot be on 
 *  different decoder (MAIN and AUX) simultaneously)
 *
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eChl        Channel selection
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT538xInternal_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg;
    
    switch (u1Indx)
    {
        case 0:
        	u4Reg = INT_ADC_IN0L0R;
        	break;
        case 1:
        	u4Reg = INT_ADC_IN1L1R;
        	break;
        case 2:
        	u4Reg = INT_ADC_IN2L2R;
        	break;
        case 3:
        	u4Reg = INT_ADC_IN3L3R;
        	break;
        case 4:
        	u4Reg = INT_ADC_IN4L4R;
        	break;	
        case 5:
        	u4Reg = INT_ADC_IN5L5R;
        	break;
        case 6:
        	u4Reg = INT_ADC_IN6L6R;
        	break;
        case 7:
        	u4Reg = INT_ADC_IN7L7R;
        	break;
        default:
        	u4Reg = INT_ADC_IN0L0R;
        	LOG(1, "Error _ADC_ChlSel\n");
        	break;
    }

    AUD_WRITE32(REG_INT_SDADC2, (AUD_READ32(REG_INT_SDADC2) & ~INT_ADC_INPUT_MUX_MASK) |u4Reg);
    
}

//-----------------------------------------------------------------------------
/** _MT538xInternal_Mute
 *  Intial internal ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT538xInternal_Mute(UINT8 u1DacId,BOOL fgEnable)
{
    UNUSED(u1DacId);

      if (fgEnable)
    	{
        	AUD_SET_BIT(REG_INT_DACCTL0, DIG_MUTE_0);
		    AUD_SET_BIT(REG_INT_DACCTL2, DIG_MUTE_1);
            AUD_SET_BIT(REG_DACCTL5,ADAC_MUTE);        	
    	}
    	else
    	{
        	AUD_CLR_BIT(REG_INT_DACCTL0, DIG_MUTE_0);
        	AUD_CLR_BIT(REG_INT_DACCTL2, DIG_MUTE_1);
            AUD_CLR_BIT(REG_DACCTL5,ADAC_MUTE);
    	}
}


//-----------------------------------------------------------------------------
/** _MT538xInternal_SetAdcGain
 *  
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT538xInternal_SetAdcGain(UINT8 u1DecId, INT16 i2Value)
{
	UINT8 u1RegValue = 0xcf;
	UNUSED(u1DecId);

	// Check Max and min
	if (i2Value >= MT538xInternal_ADC_VOL_MAX)
	{
		i2Value = MT538xInternal_ADC_VOL_MAX;
	}
	else if (i2Value <= MT538xInternal_ADC_VOL_MIN)
	{
		i2Value = MT538xInternal_ADC_VOL_MIN;
	}

	if (i2Value == 0)
	{
		u1RegValue = 0x0;
	}
	if (i2Value == 1)
	{
		u1RegValue = 0x0;
	}


	u1RegValue = 0xcf +  i2Value;

	AUD_WRITE32(REG_INT_SDADC1, u1RegValue);

}

void _MT538xInternal_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl)
{
    if (u1DacId == AUD_DEC_MAIN)
    {
        switch (eChl)
        {
        	case AUD_CHL_L_R:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC0_SEL_MASK)) )|DAC0_SEL_0_LR );
                break;
        	case AUD_CHL_LS_RS:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC0_SEL_MASK))) |DAC0_SEL_0_SLR );
                break;
        	case AUD_CHL_C_SW:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC0_SEL_MASK)))|DAC0_SEL_0_CLFE );
                break;
        	case AUD_CHL_DOWNMIX:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC0_SEL_MASK))) |DAC0_SEL_0_CH90 );
                break;
        	case AUD_CHL_BYPASS:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC0_SEL_MASK))) |DAC0_SEL_0_CH78 );
                break;
        	case AUD_CHL_AUX:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC0_SEL_MASK)))|DAC0_SEL_1_LR );
                break;
            	default:
                break;
        }
    }	
    else
    {
        switch (eChl)
        {
        	case AUD_CHL_L_R:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC1_SEL_MASK))) |DAC1_SEL_0_LR );
                break;
        	case AUD_CHL_LS_RS:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC1_SEL_MASK))) |DAC1_SEL_0_SLR );
                break;
        	case AUD_CHL_C_SW:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC1_SEL_MASK))) |DAC1_SEL_0_CLFE );
                break;
        	case AUD_CHL_DOWNMIX:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC1_SEL_MASK))) |DAC1_SEL_0_CH90 );
                break;
        	case AUD_CHL_BYPASS:
        		AUD_WRITE32(REG_IDACHCG,( AUD_READ32(REG_IDACHCG) &(~ (DAC1_SEL_MASK))) |DAC1_SEL_0_CH78 );
                break;
        	case AUD_CHL_AUX:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC1_SEL_MASK))) |DAC1_SEL_1_LR );
                break;
            	default:
                break;
        }
    }
}

//-----------------------------------------------------------------------------
/** _MT538xInternal_GetInterAdcTbl
 *  Intial internal ADC and ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
CODEC_T*  _CODEC_GetMT538xInternalTbl(void)
{
    return &_rCodecMT538xInternal;
}

