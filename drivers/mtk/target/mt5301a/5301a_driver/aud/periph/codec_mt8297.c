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
 * $RCSfile: codec_mt8297.c,v $
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
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "aud_if.h"
#include "aud_hw.h"
#include "codec_mt8297.h" // For environment register read & write
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

static HANDLE_T _hMt8297Sema = NULL;

static CODEC_FUNCTBL_T _rCodecFunTbl = 
{
	_MT8297_Init,			//pfnInit
	_MT8297_Mute,			//pfnMute 
	NULL,  							//pfnSetAdcOutFmt;
   	NULL,      						//pfnSetDacInFmt;
   	NULL, 							//pfnSetDacGain;
   	NULL,							// pfnGetDacGain
   	_MT8297_SetAdcGain,	// pfnSetAdcGain
	NULL,							// pfnGetAdcGain   
   	_MT8297_HpVolCtl,				// pfnSetHpGain
   	NULL,							// pfnGetHpGain
   	_MT8297_ChlSel,			// pfnSetAdcMuxSel
   	NULL,							// pfnRegWrite
   	NULL							// pfnRegRead
};


static CODEC_T _rCodecMT8297 = 
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

//---------------------------------------------------------------------------
// Global variables Reference
//---------------------------------------------------------------------------
extern BOOL _fgCodecSCAR2Mute;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

void _MT8297_SemaInit(void)
{
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        fgInit = TRUE;
        VERIFY(x_sema_create(&_hMt8297Sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);         
    }
}

void _MT8297_SemaLock(void)
{
    if (_hMt8297Sema)
    {
        VERIFY(x_sema_lock(_hMt8297Sema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
    else
    {
        _MT8297_SemaInit();
        VERIFY(x_sema_lock(_hMt8297Sema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
}

void _MT8297_SemaUnLock(void)
{
    VERIFY(x_sema_unlock(_hMt8297Sema) == OSR_OK);
}

static void _MT8297_SwitchBank(UINT8 u1BankNum)
{
    switch (u1BankNum)
    {
    case 0:
        _MT8297_DirectWRITE(0x7F, 0x00, 0xff);    
        break;
    case 1:
        _MT8297_DirectWRITE(0x7F, 0x01, 0xff);    
        break;
    case 2:
        _MT8297_DirectWRITE(0x7F, 0x02, 0xff);    
        break;
    default:
        _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    } 
}

void _MT8297_DirectWRITE(UINT8 u1Addr, UINT8 u1Data, UINT8 u1Msk)
{
    UINT32 u4WriteCount = 0; 
    
#ifdef CC_MT5391    
    //u4WriteCount = SIF_Write(SIF_CLK_DIV, MT8297_DEV_ADDR, u1Addr, &u1Data, 1);
    u4WriteCount = SIF_MT8297_DirectWrite8Msk(u1Addr, u1Data, u1Msk);


    LOG( 9,"8297 I2C write (address,data, mask) = (0x %x,0x %x, 0x %x)\n",u1Addr, u1Data, u1Msk);

    if (u4WriteCount == 0)
    {
    	;
    }	
    else
    {
    	LOG(5, "Error MT8297 direct register setting\n");
    }	
#else
    UNUSED(u1Addr);
    UNUSED(u1Data);
    UNUSED(u1Msk);
    UNUSED(u4WriteCount);  
#endif    
}

UINT8 _MT8297_DirectREAD(UINT8 u1Addr) 
{
    //UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;
#ifdef CC_MT5391    
    //u4ReadCount = SIF_Read(SIF_CLK_DIV, MT8297_DEV_ADDR, u1Addr, &u1Data, 1);
    u1Data = SIF_MT8297_DirectRead8(u1Addr);

    LOG( 9,"8297 I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u1Data > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "MT8297 Direct Register Read Fail\n");
    	return FALSE;
    }
#else
    UNUSED(u1Addr);
    UNUSED(u1Data);    
    return FALSE;        
#endif    
}

void _MT8297_InDirectWRITE(UINT8 u1Addr, UINT8 u1Data, UINT8 u1Msk)
{
    UINT32 u4WriteCount = 0; 
  #ifdef CC_MT5391    
    //u4WriteCount = SIF_Write(SIF_CLK_DIV, MT8297_DEV_ADDR, u1Addr, &u1Data, 1);
    u4WriteCount = SIF_MT8297_InDirectWrite8Msk(u1Addr, u1Data, u1Msk);

    LOG( 9,"8297 I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount == 0)
    {
    	;
    }	
    else
    {
    	LOG(5, "Error MT8297 in-direct register setting\n");
    }	
  #else
    UNUSED(u1Addr);
    UNUSED(u1Data);
    UNUSED(u1Msk);
    UNUSED(u4WriteCount);
  #endif      
}

UINT8 _MT8297_InDirectREAD(UINT8 u1Addr) 
{
    //UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;
#ifdef CC_MT5391    
    //u4ReadCount = SIF_Read(SIF_CLK_DIV, MT8297_DEV_ADDR, u1Addr, &u1Data, 1);
    u1Data = SIF_MT8297_InDirectRead8(u1Addr);

    LOG( 9,"8297 I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u1Data > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "MT8297 in-direct register read fail\n");
    	return FALSE;
    }
#else
    UNUSED(u1Addr);
    UNUSED(u1Data);
    return FALSE;    
#endif    
}



//-----------------------------------------------------------------------------
/** _ADAC_InterDacInit
 *  Intial internal ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT8297_Init(void)
{
    AUD_INTER_DAC_CFG_T* prInterDacUsage;

    _MT8297_SemaInit();
   
    //AD Init
    _MT8297_InDirectWRITE(0x03, 0x03, 0xff);
    
    _MT8297_InDirectWRITE(0x05, 0x03, 0xff);  
    
    _MT8297_DirectWRITE(0x40, 0x23, 0xff);
    
    //AD gain
    _MT8297_DirectWRITE(0x41, 0x33, 0xff);
    _MT8297_DirectWRITE(0x42, 0xff, 0xff);
    _MT8297_DirectWRITE(0x43, 0xff, 0xff);
    _MT8297_DirectWRITE(0x44, 0x1a, 0xff);
    
    _MT8297_DirectWRITE(0x45, 0x36, 0xff);
    _MT8297_DirectWRITE(0x46, 0x05, 0xff);//LRCK inverse for phase diff
    _MT8297_DirectWRITE(0x47, 0x00, 0xff);
    _MT8297_DirectWRITE(0x48, 0xE0, 0xff);//For internal reference, change from 0x20 to 0xE0
    _MT8297_DirectWRITE(0x49, 0x00, 0xff);
    _MT8297_DirectWRITE(0x4a, 0x14, 0xff);//For internal reference, change gain from 0x17 to 0x14
    _MT8297_DirectWRITE(0x4b, 0x02, 0xff);
    _MT8297_DirectWRITE(0x4c, 0x06, 0xff); //ain_6


    //===================================================================================
    _MT8297_SemaLock();
    //DAC Init

    //DAC clock source
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x78, 0x80, 0xff);

    // DAC0 format & DAC0 data source
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x60, 0x1A, 0xff);
    //_MT8297_DirectWRITE(0x61, 0x00, 0xff);

    //DAC 1, 2 format & DAC1,2 data source
    _MT8297_DirectWRITE(0x68, 0x1A, 0xff);
    //_MT8297_DirectWRITE(0x69, 0x00, 0xff);
    _MT8297_DirectWRITE(0x70, 0x1A, 0xff);
    //_MT8297_DirectWRITE(0x71, 0x00, 0xff);

    _MT8297_SemaUnLock();
    //===================================================================================
    
	//Set  DAC channel 
    UNUSED(DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32 *)(void *)&prInterDacUsage));
    if (prInterDacUsage != NULL)
    {
        _MT8297_DacChlSel(INTERNAL_DAC_ID_MAIN, prInterDacUsage->eDac0ChlSel);
        _MT8297_DacChlSel(INTERNAL_DAC_ID_AUX,  prInterDacUsage->eDac1ChlSel);
        _MT8297_DacChlSel(INTERNAL_DAC_ID_THIRD,prInterDacUsage->eDac2ChlSel);        
    }	

    //Set 5391 BCK/LRCK type, just need to change 8297 reference bellow 
	AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (BCK0_MSK)) )|BCK0_SEL_0 );
	AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (BCK1_MSK)) )|BCK1_SEL_1 );

    //Set 8297 DAC BCK/LRCK source
    if (prInterDacUsage != NULL)
    {
        _MT8297_DacBCKSel(INTERNAL_DAC_ID_MAIN, prInterDacUsage->eDac0ChlSel);
        _MT8297_DacBCKSel(INTERNAL_DAC_ID_AUX,  prInterDacUsage->eDac1ChlSel);
        _MT8297_DacBCKSel(INTERNAL_DAC_ID_THIRD,prInterDacUsage->eDac2ChlSel);        
    }	

    //===================================================================================
    _MT8297_SemaLock();

    //DAC mute
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x7E, 0x00, 0xff);

    //mute rampenable
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x7A, 0x81, 0xff);

    //channel power on & channel clock reset(digital) 
    _MT8297_DirectWRITE(0x7F, 0x01, 0xff);
    _MT8297_DirectWRITE(0x60, 0x0F, 0xff);
    _MT8297_DirectWRITE(0x61, 0x00, 0xff);

    //channel clock enable (analog)
    _MT8297_DirectWRITE(0x7F, 0x01, 0xff);
    _MT8297_DirectWRITE(0x67, 0x77, 0xff);
    //channel clcok & mute (analog)
    _MT8297_DirectWRITE(0x7F, 0x01, 0xff);
    _MT8297_DirectWRITE(0x68, 0x77, 0xff);


    //DWA & OSR             
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x63, 0x0A, 0xff);
    _MT8297_DirectWRITE(0x6B, 0x0A, 0xff);
    _MT8297_DirectWRITE(0x73, 0x0A, 0xff);
    //PAD driving max                 
    _MT8297_InDirectWRITE(0x60, 0xFF, 0xff);

    //MOD coefficient                
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x64, 0x1A, 0xff);
    _MT8297_DirectWRITE(0x6C, 0x1A, 0xff);
    _MT8297_DirectWRITE(0x74, 0x1A, 0xff);

    //MOD pipe
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x79, 0x30, 0xff);

    //Rampe zero coumt
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x7B, 0x10, 0xff);

    //sample_new_cnt
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x7C, 0x10, 0xff);

    //sample rate change
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    _MT8297_DirectWRITE(0x7D, 0x03, 0xff);

    //latch
    _MT8297_DirectWRITE(0x7F, 0x01, 0xff);
    _MT8297_DirectWRITE(0x69, 0x00, 0xff);
    _MT8297_DirectWRITE(0x6A, 0x00, 0xff);
    _MT8297_DirectWRITE(0x6B, 0x00, 0xff);
    _MT8297_DirectWRITE(0x6C, 0x00, 0xff);

    //VICM/VOCM adjust
    _MT8297_DirectWRITE(0x7F, 0x01, 0xff);
    _MT8297_DirectWRITE(0x62, 0x06, 0xff);
    _MT8297_DirectWRITE(0x63, 0x40, 0xff);
    _MT8297_DirectWRITE(0x64, 0x44, 0xff);
    _MT8297_DirectWRITE(0x65, 0x44, 0xff);
    _MT8297_DirectWRITE(0x66, 0x04, 0xff);

    //PGA mute & HP data
    _MT8297_DirectWRITE(0x7F, 0x02, 0xff);
    _MT8297_DirectWRITE(0x65, 0x90, 0xff);
    //PGA gain
    _MT8297_DirectWRITE(0x66, 0x17, 0xff);
    _MT8297_DirectWRITE(0x67, 0x1D, 0xff);
    _MT8297_DirectWRITE(0x68, 0x1D, 0xff);
    _MT8297_DirectWRITE(0x69, 0x1D, 0xff);
    //HP decoder select
    _MT8297_DirectWRITE(0x60, 0x84, 0xff);

    _MT8297_SemaUnLock();
    //===================================================================================
   
    

/*
    	//Enable AD
	AUD_SET_BIT(REG_INT_SDADC0, EN);
    	//Light: Bypass high pass filter to prevent from "po po" noises
    	//when analog input is floating. MT538x IC might have design defect.
    	AUD_SET_BIT(REG_INT_SDADC0, CHPD);
    	AUD_CLR_BIT(REG_INT_SDADC0, RST);
    	AUD_CLR_BIT(REG_INT_SDADC0,APLL_SEL);

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
	AUD_CLR_BIT(REG_INT_DACCTL0,MUTE_0);		// 1st
	AUD_CLR_BIT(REG_INT_DACCTL2,MUTE_1);		// 2ed
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
#ifdef CC_AUD_DVBT_SUPPORT       
       AUD_SET_BIT(REG_DACCTL6,ADAC_AUX_APLL1_SEL);
#endif

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


*/    
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
void _MT8297_ChlSel(UINT8 u1Indx)
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
        default:
        	u4Reg = INT_ADC_IN0L0R;
        	LOG(1, "Error _ADC_ChlSel\n");
        	break;
    }

    _MT8297_DirectWRITE(0x4c, u4Reg, 0x07);
    
}

//-----------------------------------------------------------------------------
/**
 *  AD power down
 *  
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eChl        Channel selection
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT8297_ADC_PowerDown(BOOL fgEnable)
{

    UINT32 u4Reg;
    if (fgEnable)
    {
        u4Reg = INT_ADC_POWERDOWN;
    }
    else
    {
        u4Reg = INT_ADC_POWERON;
    }	
	    
    _MT8297_DirectWRITE(0x4c, u4Reg, 0x18);
    
}


//-----------------------------------------------------------------------------
/** _MT538xInternal_Mute
 *  Intial internal ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT8297_Mute(UINT8 u1DacId,BOOL fgEnable)
{
    _MT8297_SemaLock();
    
    if (fgEnable)
    {
        if (u1DacId == INTERNAL_DAC_ID_MAIN)
        {
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC0_MUTE, ADAC0_MUTE_MSK);
        }
        else if (u1DacId == INTERNAL_DAC_ID_AUX)
        {
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC1_MUTE, ADAC1_MUTE_MSK);
        }        
        else if (u1DacId == INTERNAL_DAC_ID_THIRD)
        {
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC2_MUTE, ADAC2_MUTE_MSK);
        }        
        else
        {
            if(!_fgCodecSCAR2Mute)
            {    
                _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
                _MT8297_DirectWRITE(ADAC_1E, ADAC2_MUTE, ADAC2_MUTE_MSK);

                //_MT8297_DirectWRITE(0x7F, 0x01, 0xff);
                //_MT8297_DirectWRITE(ADAC_28, ANALOG_MUTE, ANALOG_MUTE_MSK);
            }
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC0_MUTE, ADAC0_MUTE_MSK);
            
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC1_MUTE, ADAC1_MUTE_MSK);
        }
    }
    else
    {
        if (u1DacId == INTERNAL_DAC_ID_MAIN)
        {
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC_UNMUTE_SET, ADAC0_MUTE_MSK);
        }
        else if (u1DacId == INTERNAL_DAC_ID_AUX)
        {
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC_UNMUTE_SET, ADAC1_MUTE_MSK);
        }        
        else if (u1DacId == INTERNAL_DAC_ID_THIRD)
        {
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC_UNMUTE_SET, ADAC2_MUTE_MSK);
        }        
        else
        {   
            if(!_fgCodecSCAR2Mute)
            {    
    	        _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
                _MT8297_DirectWRITE(ADAC_1E, ADAC_UNMUTE_SET, ADAC2_MUTE_MSK);

                //_MT8297_DirectWRITE(0x7F, 0x01, 0xff);
                //_MT8297_DirectWRITE(ADAC_28, ANALOG_UNMUTE, ANALOG_MUTE_MSK);
            }
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC_UNMUTE_SET, ADAC0_MUTE_MSK);
            
            _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
            _MT8297_DirectWRITE(ADAC_1E, ADAC_UNMUTE_SET, ADAC1_MUTE_MSK);
        }
    }       	

    _MT8297_SemaUnLock();
}


//-----------------------------------------------------------------------------
/** _MT538xInternal_SetAdcGain
 *  
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _MT8297_SetAdcGain(UINT8 u1DecId, INT16 i2Value)
{
/*
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
*/
}

void _MT8297_InterDacPGAGain(UINT8 u1DacId,UINT8 u1Volume)
{
    _MT8297_SemaLock();
    
    _MT8297_DirectWRITE(0x7f, 0x02, 0xff);

    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        _MT8297_DirectWRITE(0x67, u1Volume, 0x3f);
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        _MT8297_DirectWRITE(0x68, u1Volume, 0x3f);
    }
    else
    {
       _MT8297_DirectWRITE(0x69, u1Volume, 0x3f);
    }

    _MT8297_SemaUnLock();
}

void _MT8297_InterHPPGAGain(UINT8 u1Volume)
{
    _MT8297_SemaLock();
    
    _MT8297_DirectWRITE(0x7f, 0x02, 0xff);

    _MT8297_DirectWRITE(0x66, u1Volume, 0x3f);

    _MT8297_SemaUnLock();
}


void _MT8297_DacBCKSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl)
{
    _MT8297_SemaLock();

    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);
    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        switch (eChl)
        {
        	case AUD_CHL_L_R:
        	case AUD_CHL_LS_RS:
        	case AUD_CHL_C_SW:
        	case AUD_CHL_BYPASS:
        	case AUD_CHL_DOWNMIX:
                _MT8297_DirectWRITE(ADAC_18, DAC0_BCK_SEL_DEC0, DAC0_BCK_SEL_MSK);
                break;
        	case AUD_CHL_AUX:
                _MT8297_DirectWRITE(ADAC_18, DAC0_BCK_SEL_DEC1, DAC0_BCK_SEL_MSK);
                break;
        }
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        switch (eChl)
        {
        	case AUD_CHL_L_R:
        	case AUD_CHL_LS_RS:
        	case AUD_CHL_C_SW:
        	case AUD_CHL_BYPASS:
        	case AUD_CHL_DOWNMIX:
                _MT8297_DirectWRITE(ADAC_18, DAC1_BCK_SEL_DEC0, DAC1_BCK_SEL_MSK);
                break;
        	case AUD_CHL_AUX:
                _MT8297_DirectWRITE(ADAC_18, DAC1_BCK_SEL_DEC1, DAC1_BCK_SEL_MSK);
                break;
        }
    }
    else
    {
        switch (eChl)
        {
        	case AUD_CHL_L_R:
        	case AUD_CHL_LS_RS:
        	case AUD_CHL_C_SW:
        	case AUD_CHL_BYPASS:
        	case AUD_CHL_DOWNMIX:
                _MT8297_DirectWRITE(ADAC_18, DAC2_BCK_SEL_DEC0, DAC2_BCK_SEL_MSK);
                break;
        	case AUD_CHL_AUX:
                _MT8297_DirectWRITE(ADAC_18, DAC2_BCK_SEL_DEC1, DAC2_BCK_SEL_MSK);
                break;
        }
    }      

    _MT8297_SemaUnLock();
}


void _MT8297_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl)
{
    _MT8297_SemaLock();
    
    _MT8297_DirectWRITE(0x7F, 0x00, 0xff);

    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        _MT8297_DirectWRITE(0x61, 0x00, 0xff);

        switch (eChl)
        {
        	case AUD_CHL_L_R:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD0_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S0_CFG_0_MSK)) )|S0_CFG_0_LR );
                break;
        	case AUD_CHL_LS_RS:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD0_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S0_CFG_0_MSK)) )|S0_CFG_0_SLR );
                break;
        	case AUD_CHL_C_SW:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD0_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S0_CFG_0_MSK)) )|S0_CFG_0_CS );
                break;
        	case AUD_CHL_BYPASS:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD0_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S0_CFG_0_MSK)) )|S0_CFG_0_78 );
                break;
        	case AUD_CHL_DOWNMIX:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD0_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S0_CFG_0_MSK)) )|S0_CFG_0_90 );
                break;
        	case AUD_CHL_AUX:
            	AUD_SET_BIT(REG_8297CH_CHCFG, SD0_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S3_CFG_0_MSK)) )|S3_CFG_0_LR );
                break;
            	default:
                break;
        }
    }	
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        _MT8297_DirectWRITE(0x69, 0x04, 0xff);
        switch (eChl)
        {
        	case AUD_CHL_L_R:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD1_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S1_CFG_0_MSK)) )|S1_CFG_0_LR );
                break;
        	case AUD_CHL_LS_RS:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD1_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S1_CFG_0_MSK)) )|S1_CFG_0_SLR );
                break;
        	case AUD_CHL_C_SW:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD1_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S1_CFG_0_MSK)) )|S1_CFG_0_CS );
                break;
        	case AUD_CHL_BYPASS:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD1_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S1_CFG_0_MSK)) )|S1_CFG_0_78 );
                break;
        	case AUD_CHL_DOWNMIX:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD1_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S1_CFG_0_MSK)) )|S1_CFG_0_90 );
                break;
        	case AUD_CHL_AUX:
            	AUD_SET_BIT(REG_8297CH_CHCFG, SD1_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S3_CFG_0_MSK)) )|S3_CFG_0_LR );
                break;
            	default:
                break;
        }
    }
    else
    {
        _MT8297_DirectWRITE(0x71, 0x08, 0xff);
        switch (eChl)
        {
        	case AUD_CHL_L_R:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD2_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S2_CFG_0_MSK)) )|S2_CFG_0_LR );
                break;
        	case AUD_CHL_LS_RS:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD2_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S2_CFG_0_MSK)) )|S2_CFG_0_SLR );
                break;
        	case AUD_CHL_C_SW:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD2_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S2_CFG_0_MSK)) )|S2_CFG_0_CS );
                break;
        	case AUD_CHL_BYPASS:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD2_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S2_CFG_0_MSK)) )|S2_CFG_0_78 );
                break;
        	case AUD_CHL_DOWNMIX:
            	AUD_CLR_BIT(REG_8297CH_CHCFG, SD2_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S2_CFG_0_MSK)) )|S2_CFG_0_90 );
                break;
        	case AUD_CHL_AUX:
            	AUD_SET_BIT(REG_8297CH_CHCFG, SD2_SEL);       	    
        		AUD_WRITE32(REG_8297CH_CHCFG, (AUD_READ32(REG_8297CH_CHCFG) &(~ (S3_CFG_0_MSK)) )|S3_CFG_0_LR );
                break;
            	default:
                break;
        }
    }
    
    _MT8297_SemaUnLock();
}

#if 1
void _MT8297_LRInvert(UINT8 u1DacId,BOOL fgInvert)
{
    _MT8297_SemaLock();
    
    _MT8297_SwitchBank(0);

    if (fgInvert)
    {
        if (u1DacId == INTERNAL_DAC_ID_MAIN)
        {
            _MT8297_DirectWRITE(ADAC_00, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_01, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }
        else if (u1DacId == INTERNAL_DAC_ID_AUX)
        {
            _MT8297_DirectWRITE(ADAC_08, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }        
        else if (u1DacId == INTERNAL_DAC_ID_THIRD)
        {
            _MT8297_DirectWRITE(ADAC_10, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }        
        else
        {
            _MT8297_DirectWRITE(ADAC_00, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_08, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_10, LR_INVERT, LR_INVERT_MSK);

            _MT8297_DirectWRITE(ADAC_01, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }
    }
    else
    {
        if (u1DacId == INTERNAL_DAC_ID_MAIN)
        {
            _MT8297_DirectWRITE(ADAC_00, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_01, 0, LR_DIG_INVERT_MSK);
        }
        else if (u1DacId == INTERNAL_DAC_ID_AUX)
        {
            _MT8297_DirectWRITE(ADAC_08, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, 0, LR_DIG_INVERT_MSK);
        }        
        else if (u1DacId == INTERNAL_DAC_ID_THIRD)
        {
            _MT8297_DirectWRITE(ADAC_10, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, 0, LR_DIG_INVERT_MSK);            
        }        
        else
        {
            _MT8297_DirectWRITE(ADAC_00, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_08, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_10, 0, LR_INVERT_MSK);

            _MT8297_DirectWRITE(ADAC_01, 0, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, 0, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, 0, LR_DIG_INVERT_MSK);
        }
    }       	

    _MT8297_SemaUnLock();
}

#else

void _MT8297_LRInvert(UINT8 u1DacId,BOOL fgInvert)
{
    _MT8297_SwitchBank(0);

    if (fgInvert)
    {
        if (u1DacId == INTERNAL_DAC_ID_MAIN)
        {
            //_MT8297_DirectWRITE(ADAC_00, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_01, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }
        else if (u1DacId == INTERNAL_DAC_ID_AUX)
        {
            //_MT8297_DirectWRITE(ADAC_08, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }        
        else if (u1DacId == INTERNAL_DAC_ID_THIRD)
        {
            //_MT8297_DirectWRITE(ADAC_10, LR_INVERT, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }        
        else
        {
            //_MT8297_DirectWRITE(ADAC_00, LR_INVERT, LR_INVERT_MSK);
            //_MT8297_DirectWRITE(ADAC_08, LR_INVERT, LR_INVERT_MSK);
            //_MT8297_DirectWRITE(ADAC_10, LR_INVERT, LR_INVERT_MSK);

            _MT8297_DirectWRITE(ADAC_01, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, LR_DIG_INVERT, LR_DIG_INVERT_MSK);
        }
    }
    else
    {
        if (u1DacId == INTERNAL_DAC_ID_MAIN)
        {
            //_MT8297_DirectWRITE(ADAC_00, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_01, 0, LR_DIG_INVERT_MSK);
        }
        else if (u1DacId == INTERNAL_DAC_ID_AUX)
        {
            //_MT8297_DirectWRITE(ADAC_08, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, 0, LR_DIG_INVERT_MSK);
        }        
        else if (u1DacId == INTERNAL_DAC_ID_THIRD)
        {
            //_MT8297_DirectWRITE(ADAC_10, 0, LR_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, 0, LR_DIG_INVERT_MSK);            
        }        
        else
        {
            //_MT8297_DirectWRITE(ADAC_00, 0, LR_INVERT_MSK);
            //_MT8297_DirectWRITE(ADAC_08, 0, LR_INVERT_MSK);
            //_MT8297_DirectWRITE(ADAC_10, 0, LR_INVERT_MSK);

            _MT8297_DirectWRITE(ADAC_01, 0, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_09, 0, LR_DIG_INVERT_MSK);
            _MT8297_DirectWRITE(ADAC_11, 0, LR_DIG_INVERT_MSK);
        }
    }       	
}

#endif


void _MT8297_DACFmt(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);

    _MT8297_SemaLock();
    
    _MT8297_SwitchBank(0);
    
    switch(eFormat)
    {
    case FORMAT_RJ:
        _MT8297_DirectWRITE(ADAC_00, I2S_FMT_RJ, I2S_FMT_MSK);
        _MT8297_DirectWRITE(ADAC_08, I2S_FMT_RJ, I2S_FMT_MSK);
        _MT8297_DirectWRITE(ADAC_10, I2S_FMT_RJ, I2S_FMT_MSK);
        break;
    case FORMAT_LJ:
        _MT8297_DirectWRITE(ADAC_00, I2S_FMT_LJ, I2S_FMT_MSK);
        _MT8297_DirectWRITE(ADAC_08, I2S_FMT_LJ, I2S_FMT_MSK);
        _MT8297_DirectWRITE(ADAC_10, I2S_FMT_LJ, I2S_FMT_MSK);
        break;
    case FORMAT_I2S:
        _MT8297_DirectWRITE(ADAC_00, I2S_FMT_I2S, I2S_FMT_MSK);
        _MT8297_DirectWRITE(ADAC_08, I2S_FMT_I2S, I2S_FMT_MSK);
        _MT8297_DirectWRITE(ADAC_10, I2S_FMT_I2S, I2S_FMT_MSK);       
        break;
    default:
        break;
    }

    _MT8297_SemaUnLock();
}


void _MT8297_ADCFmt(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);

    _MT8297_SemaLock();
    
    switch(eFormat)
    {
    case FORMAT_RJ:
        _MT8297_DirectWRITE(ADC_I2S_CFG0, ADC_I2S_FMT_RJ, ADC_I2S_FMT_MSK);
        break;
    case FORMAT_LJ:
        _MT8297_DirectWRITE(ADC_I2S_CFG0, ADC_I2S_FMT_LJ, ADC_I2S_FMT_MSK);
        break;
    case FORMAT_I2S:
        _MT8297_DirectWRITE(ADC_I2S_CFG0, ADC_I2S_FMT_I2S, ADC_I2S_FMT_MSK);
        break;
    default:
        break;
    }

    _MT8297_SemaUnLock();
}


void _MT8297_HpVolCtl(UINT8 u1HpId, INT16 i2Value)
{
    UNUSED(u1HpId);
    _MT8297_SemaLock();
    _MT8297_SwitchBank(2);
	
    if (i2Value == 0)
    {
        _MT8297_DirectWRITE(ADAC_45, HPMUTE, HPUTE_MSK);
    }
    else if (i2Value == 100)
    {
        _MT8297_DirectWRITE(ADAC_45, HPUNMUTE, HPUTE_MSK);
    }
    else
    {
        _MT8297_DirectWRITE(ADAC_45, HPUNMUTE, HPUTE_MSK);
    }

    _MT8297_SemaUnLock();
}


//-----------------------------------------------------------------------------
/** _MT538xInternal_GetInterAdcTbl
 *  Intial internal ADC and ADAC
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
CODEC_T*  _CODEC_GetMT8297Tbl(void)
{
    return &_rCodecMT8297;
}

