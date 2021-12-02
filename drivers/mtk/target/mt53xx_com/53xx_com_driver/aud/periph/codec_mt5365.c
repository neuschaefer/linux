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
 * $RCSfile: codec_mt8291.c,v $
 * $Revision: #6 $
 *
 *---------------------------------------------------------------------------*/

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "drvcust_if.h"
#include "codec_mt5365.h"
#include "aud_debug.h"
#include "x_pinmux.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_END
#include "drvcust_if.h"
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "aud_hw.h"

//#define LOG_REG_WRITE

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#ifdef LOG_REG_WRITE
#undef AUD_WRITE32
#undef AUD_CLR_BIT

void AUD_WRITE32(UINT32 offset, UINT32 value)
{
    IO_WRITE32(AUDIO_BASE, (offset), (value));
    Printf("[!!!PWM!!!] I2C_EXT 58 00005%03x[31:00] %08x RW\n",
           offset, value);
}

#define AUD_CLR_BIT(Reg, Bit) AUD_WRITE32(Reg, AUD_READ32(Reg) & (~(Bit)))
#endif

#define vWritePWM(addr, data) \
    AUD_WRITE32(REG_INDIRECT0_ADDR, addr);\
    AUD_WRITE32(REG_INDIRECT0_DATA, data);\

#define vWritePWM1(addr, data) \
    AUD_WRITE32(REG_INDIRECT1_ADDR, addr);\
    AUD_WRITE32(REG_INDIRECT1_DATA, data);\

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
static void _MT5365_Init_all(UINT8 u1ClkFmt, BOOL fgInit);
static void _MT5365_PWM_SamplingRate_Fsi(UINT8 u1ClkFmt);

//---------------------------------------------------------------------------
// Static variables a
//---------------------------------------------------------------------------
#ifdef CC_AUD_PWM_SAWLESS_PLL_EN
    ///Aout = 48KHz
    ///PWM Freq = 48*3*4 = 576KHz
static const UINT32 YRAM[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x0d8000, 0xfff900, 0xfff000, 0xffed00, 0xfff900, 0x001400, 0x002f00, 0x003400, 0x001400,
  0xffd500, 0xff9800, 0xff8d00, 0xffd200, 0x005300, 0x00c800, 0x00da00, 0x005900, 0xff7000, 0xfea200,
  0xfe8600, 0xff6400, 0x00ea00, 0x023e00, 0x026a00, 0x010300, 0xfe9400, 0xfc7d00, 0xfc3800, 0xfe6400,
  0x022500, 0x055f00, 0x05ce00, 0x028300, 0xfcc900, 0xf7d200, 0xf71000, 0xfc0b00, 0x04ec00, 0x0cdf00,
  0x0e6500, 0x06a000, 0xf7cd00, 0xe95700, 0xe50800, 0xf25f00, 0x124d00, 0x3d8f00, 0x66d700, 0x7fff00,
  0x0d8000, 0x0d8000, 0x0d8000, 0x0d8000, 0x0d8000  /// Ref Yram[d'21]
};
    ///Aout = 48KHz
    ///PWM Freq = 48*3*3 = 432KHz
static const UINT32 YRAM_432[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x0c0000, 0xfff700, 0xffed00, 0xfff600, 0x001a00, 0x003700, 0x001c00, 0xffc700, 0xff8700,
  0xffc200, 0x006e00, 0x00e700, 0x007800, 0xff4100, 0xfe6d00, 0xff2d00, 0x013800, 0x029200, 0x015b00,
  0xfe1b00, 0xfbf800, 0xfdda00, 0x02de00, 0x062c00, 0x035700, 0xfbb000, 0xf68e00, 0xfac400, 0x06a200,
  0x0f0600, 0x08b100, 0xf4d900, 0xe4c700, 0xee7400, 0x199700, 0x552100, 0x7fff00, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x0c0000, 0x0c0000, 0x0c0000, 0x0c0000, 0x0c0000
};

#else
    ///Aout = 48KHz
    ///PWM Freq = 48*2*4 = 384KHz
static const UINT32 YRAM[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x0f0600, 0xfff900, 0xfff000, 0xffed00, 0xfff900, 0x001400, 0x002f00, 0x003400, 0x001400,
  0xffd500, 0xff9800, 0xff8d00, 0xffd200, 0x005300, 0x00c800, 0x00da00, 0x005900, 0xff7000, 0xfea200,
  0xfe8600, 0xff6400, 0x00ea00, 0x023e00, 0x026a00, 0x010300, 0xfe9400, 0xfc7d00, 0xfc3800, 0xfe6400,
  0x022500, 0x055f00, 0x05ce00, 0x028300, 0xfcc900, 0xf7d200, 0xf71000, 0xfc0b00, 0x04ec00, 0x0cdf00,
  0x0e6500, 0x06a000, 0xf7cd00, 0xe95700, 0xe50800, 0xf25f00, 0x124d00, 0x3d8f00, 0x66d700, 0x7fff00,
  0x0f0600, 0x0f0600, 0x0f0600, 0x0f0600, 0x0f0600  /// Ref Yram[d'21]
};
#endif

#if 0
static const UINT32 YRAM_for_32[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x099900, 0xffff00, 0x000000, 0x000300, 0x000800, 0x000d00, 0x000e00, 0x000600, 0xfff400,
  0xffdb00, 0xffc700, 0xffc500, 0xffe200, 0x001e00, 0x006b00, 0x00a800, 0x00af00, 0x006500, 0xffcc00,
  0xff0d00, 0xfe7600, 0xfe5b00, 0xfef500, 0x003d00, 0x01d700, 0x032400, 0x037700, 0x026000, 0xffee00,
  0xfccd00, 0xfa2800, 0xf94900, 0xfb0c00, 0xff6900, 0x053b00, 0x0a6d00, 0x0ca000, 0x0a0f00, 0x026e00,
  0xf76f00, 0xec9b00, 0xe68600, 0xe96d00, 0xf7c700, 0x112600, 0x31ee00, 0x53f900, 0x701800, 0x7fff00,
  0x099900, 0x099900, 0x099900, 0x099900, 0x099900  /// Ref Yram[d'21]
};


static const UINT32 YRAM_for_96[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x0c0000, 0x000000, 0x000000, 0x000100, 0x000100, 0xfffa00, 0xfffa00, 0x001600, 0x001700,
  0xffc100, 0xffbd00, 0x009b00, 0x00a700, 0xfeae00, 0xfe8f00, 0x02a000, 0x02ee00, 0xfb1d00, 0xfa5f00,
  0x08d400, 0x0ac800, 0xef4f00, 0xe8a600, 0x2aad00, 0x7fff00, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x0c0000, 0x0c0000, 0x0c0000, 0x0c0000, 0x0c0000  /// Ref Yram[d'21]
};
#endif

///#define ABIST_TEST
#ifdef ABIST_TEST
static const UINT32 Risc_1K_0dB_tone[] =
{
0x000000, 0x10B515, 0x2120FC, 0x30FBC5, 0x400000, 0x4DEBE5, 0x5A827A, 0x658C9A, 0x6ED9EC, 0x7641AF,
0x7BA375, 0x7EE7AA, 0x7FFF00, 0x7EE7AA, 0x7BA375, 0x7641AF, 0x6ED9EC, 0x658C9A, 0x5A827A, 0x4DEBE5,
0x400000, 0x30FBC5, 0x2120FC, 0x10B515, 0x000000, 0xEF4AEC, 0xDEDF05, 0xCF043C, 0xC00001, 0xB2141C,
0xA57D87, 0x9A7367, 0x912615, 0x89BE52, 0x845C8C, 0x811857, 0x800001, 0x811857, 0x845c8c, 0x89BE52,
0x912615, 0x9A7367, 0xA57D87, 0xB2141C, 0xC00001, 0xCF043C, 0xDEDF05, 0xEF4AEC
};
static const UINT32 Risc_6K_0dB_tone[] =
{
0x000000, 0x005a81c4, 0x7fffff, 0x5a81c4, 0x000000, 0xffa57e3c, 0xff800100, 0xffa57e3c
};

#endif
static CODEC_FUNCTBL_T _rCodecFunTbl = 
{
	_MT5365_Init,                   //pfnInit
	_MT5365_Mute,                 //pfnMute 
	_MT5365_MuteCLK,              //pfnMuteClk
	_MT5365_AdcFmtCfg,       //pfnSetAdcOutFmt;
   	_MT5365_DacFmtCfg,      	//pfnSetDacInFmt;
   	NULL,                                  //pfnSetDacGain;
   	NULL,                                  //pfnGetDacGain
   	NULL,                                  //pfnSetAdcGain
	NULL,                                  //pfnGetAdcGain   
   	NULL,                                  //pfnSetHpGain
   	NULL,                                  //pfnGetHpGain
   	_MT5365_ChlSel,               //pfnSetAdcMuxSel
   	NULL,                           	//pfnRegWrite
   	NULL                                  //pfnRegRead
};

static CODEC_T _rCodecMT5365 = 
{
	"MT5365",				// szName[32];
	0,							// i2MaxHpGain
	-72,							// i2MinHpGain
	0,							// i2MaxDacGain
	0,							// i2MinDacGain
	7,							// u1AdcMuxNum
	2,							// u1DacNum
	1,							// u1HpNum
	&_rCodecFunTbl			// rCodecFuncTbl
};

static BOOL fgPWMInit = FALSE;
static BOOL fgPopMute = FALSE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _MT5365_Init(void)
{
    BOOL    fAdcNegGain = 0;
    AUD_INTER_DAC_CFG_T* prInterDacUsage;
    BOOL fPowerStage = 0;
    BOOL fInternalLDO = 0;
    UNUSED( fPowerStage );
    UNUSED( fInternalLDO );
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_Init\n");
#endif


#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    /// Enable Internal LDO for PWMDAC use.
    if(!fgPWMInit)
    {
        _MT5365_PopMute(TRUE);
        ANA_WRITE32(REG_AADC_CFG0, (0x001844c0|VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
        fInternalLDO = (BOOL)DRVCUST_OptGet(eAudioInternalLDOEnable);
        if( fInternalLDO )
        {
            #if defined(CC_MT5396)
            ANA_WRITE32(REG_PWMDAC_CFG4, 0x3030358A);//0x3030158A
            ANA_WRITE32(REG_PWMDAC_CFG3, 0x30303030);
            ANA_WRITE32(REG_PWMDAC_CFG2, 0x00003030);
            #else //68 89
            ANA_WRITE32(REG_PWMDAC_CFG4, 0x00000102);//(AL3_ILDO_EN|AR3_ILDO_EN|CUR_MOR23_EN|REG_1P2V_EN|CUR_MOR01_EN|V_TO_I_EN)
            ANA_WRITE32(REG_PWMDAC_CFG3, 0x30300000);//(AL1_ILDO_EN|AR1_ILDO_EN|AL2_ILDO_EN|AR2_ILDO_EN)
            ANA_WRITE32(REG_PWMDAC_CFG2, 0x00003C30);//(ANA_READ32(REG_PWMDAC_CFG2)|AL0_ILDO_EN|AR0_ILDO_EN)
            #endif


            #ifdef CC_MT5396     //when open Internal LDO, default use 1kohm mode. For powerstage should use 0k ohm mode
            fPowerStage = (BOOL)DRVCUST_OptGet(eAudioPowerstageUseAL0AR0);  
            if (fPowerStage) //ALAR0
            {
                ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~SMALLGPIO_EN_AL0AR0));                    
            }

            fPowerStage = (BOOL)DRVCUST_OptGet(eAudioPowerstageUseAL1AR1);        
            if (fPowerStage) //ALAR1
            {
                ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~SMALLGPIO_EN_AL1AR1));                    
            }

            fPowerStage = (BOOL)DRVCUST_OptGet(eAudioPowerstageUseAL2AR2);        
            if (fPowerStage) //ALAR2
            {
                ANA_WRITE32(REG_PWMDAC_CFG5, ANA_READ32(REG_PWMDAC_CFG5) & (~SMALLGPIO_EN_AL2AR2));                    
            }

            fPowerStage = (BOOL)DRVCUST_OptGet(eAudioPowerstageUseAL3AR3);        
            if (fPowerStage) //ALAR3
            {
                ANA_WRITE32(REG_PWMDAC_CFG5, ANA_READ32(REG_PWMDAC_CFG5) & (~SMALLGPIO_EN_AL3AR3));                    
            }
            #endif
        }        
    
        //Need change bit16 initial value to 1.
#if defined(CC_MT5389) 
        ANA_WRITE32(REG_PWMDAC_CFG5, (PGA_CLAMP_ON | DEMOD_GROUND_ON));
#elif defined(CC_MT5396)
        ANA_WRITE32(REG_PWMDAC_CFG5, 0x0D630000);
#else
        ANA_WRITE32(REG_PWMDAC_CFG5, PGA_CLAMP_ON);
#endif
    }
#endif

    //PWM related.
    _MT5365_Init_all(FS_48K, TRUE);

    //AD related.
    #if defined(CC_MT5396)
    AUD_WRITE32(REG_INTADC_SDADC0, 0x01020040);//0x5240
    #else
    AUD_WRITE32(REG_INTADC_SDADC0, 0x01020000);//0x5240
    #endif
    //AUD_WRITE32(REG_INTADC_SDADC1, 0x20dd0000);//0x5244

    fAdcNegGain = (BOOL)DRVCUST_OptGet(eAudioADCNegDigitalGain);    ///[Joel]20101026

#if defined(CC_MT5396)
    if(fAdcNegGain)
        AUD_WRITE32(REG_INTADC_SDADC1, 0x230A8000);    ///Bit25 is MSB of adc digital gain.
    else
        AUD_WRITE32(REG_INTADC_SDADC1, 0x20f58000);
#elif defined(CC_MT5368) 
    if(fAdcNegGain)
        AUD_WRITE32(REG_INTADC_SDADC1, 0x23058000);    ///Bit25 is MSB of adc digital gain.
    else
        AUD_WRITE32(REG_INTADC_SDADC1, 0x20FA8000);
#elif defined(CC_MT5389)
		if(fAdcNegGain)
			AUD_WRITE32(REG_INTADC_SDADC1, 0x230C8000);    ///Bit25 is MSB of adc digital gain.
		else
			AUD_WRITE32(REG_INTADC_SDADC1, 0x20F38000);
#elif defined(CC_MT5395)
    if(fAdcNegGain)
        AUD_WRITE32(REG_INTADC_SDADC1, 0x230A8000);    ///Bit25 is MSB of adc digital gain.
    else
        AUD_WRITE32(REG_INTADC_SDADC1, 0x20f58000);
#elif defined(CC_MT5365)
    if(fAdcNegGain)
        AUD_WRITE32(REG_INTADC_SDADC1, 0x23090000);    ///Bit25 is MSB of adc digital gain.
    else
        AUD_WRITE32(REG_INTADC_SDADC1, 0x20f70000);
#endif

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    ANA_WRITE32(REG_AADC_CFG0, (0x001844c0|VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
#elif defined(CC_MT5365) || defined(CC_MT5395)
    AUD_WRITE32(REG_INTADC_SDADC2,0x001804c0); //0x5248
#endif    

    UNUSED(DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32 *)(void *)&prInterDacUsage));

    if (prInterDacUsage != NULL)
    {
        _MT5365_DacChlSel(INTERNAL_DAC_ID_MAIN, prInterDacUsage->eDac0ChlSel);
        _MT5365_DacChlSel(INTERNAL_DAC_ID_AUX, prInterDacUsage->eDac1ChlSel);
        _MT5365_DacChlSel(INTERNAL_DAC_ID_THIRD, prInterDacUsage->eDac2ChlSel);
        //_MT5365_DacChlSel(INTERNAL_DAC_ID_4TH, prInterDacUsage->eDac3ChlSel);
        #ifdef CC_MT5389    /// Frequency Hopping
        /// 5389 AL1/AR1 use PWM GP2 Ch7/6 output 432KHz
        /// Please take care mt5389m1v2_aud.h _armt5389m1v2InterDacUsage
        /// AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00987610);
        /// AL1/AR1 change to 76 of 0x00987610
        //_MT5365_DacChlSel(INTERNAL_DAC_ID_4TH, prInterDacUsage->eDac1ChlSel);
        #elif defined(CC_MT5396)
        //96 AL0/AR0 & AL2/AR2 use PWM GP1 :576kHz
        //    AL1/AR1 & AL3/AR3 use PWM GP2 : 432kHz
        ///AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00329810);
        ///AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0x00FF76FF);
        ///AL1/AR1 change to 98 of 0x00329810
        _MT5365_DacChlSel(INTERNAL_DAC_ID_THIRD, prInterDacUsage->eDac1ChlSel);
        ///AL2/AR2 change to 32 of 0x00329810
        _MT5365_DacChlSel(INTERNAL_DAC_ID_AUX, prInterDacUsage->eDac2ChlSel);
        #endif
    }

    //----------------Speaker output setting-------------------------
    // power stage ->speaker output setting
    // vWritePWM(A_POE, 0x333333);
    // AUD_WRITE32(REG_PWMIP_BTLMODE_PG1, 0x40000000);
    // CKGEN_WRITE32(0x178,0xC40001C2);//enhance driving current of TP_VPLL pin 
    AUD_WRITE32(REG_PWMIP_BTLMODE_PG1, PWMBTLMODE);//turn on power stage INB and IND function(TP_VPLL pin)
    AUD_WRITE32(REG_PWMIP_BTLMODE_PG2, PWMBTLMODE_PG2);//turn on power stage INB and IND function(TP_VPLL pin)


#ifdef CC_AUD_PWM_USE_GPIO
    GPIO_SetOut(39,1);
    GPIO_SetOut(40,1);
    GPIO_SetOut(42,1);
#endif

    fgPWMInit = TRUE;

#ifdef ABIST_TEST
    UINT32 i;
    UINT32 Yi;
    
    AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0xFFFFFF);
    AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0xFFFFFF);

    AUD_WRITE32(REG_PWMCFG0_PG1, 0xFA02A000);

    vWritePWM(A_PIC,        0x8000);

    AUD_WRITE32(REG_INDIRECT0_ADDR, A_PGCTRL0);
    i= AUD_READ32(REG_INDIRECT0_DATA)& 0x7fff9;///Disable 6 channel output
    ///vWritePWM(A_PGCTRL0, (i | 1<<9 | 0x3000006));    ///Disable auto-increment address and enable ch0 only.
    vWritePWM(A_PGCTRL0, (i | (1<<9) | 0x02));    ///Disable auto-increment address and enable ch0 only.

    AUD_WRITE32(REG_INDIRECT1_ADDR, A_PGCTRL0);
    i= AUD_READ32(REG_INDIRECT1_DATA)& 0x7fff9;///Disable 6 channel output, 0x7FF39 enable NS dither.
    vWritePWM1(A_PGCTRL0, (i | 1<<9));    ///Disable auto-increment address and enable ch0 only.

    ///Enable Channel 0
    //vWritePWM(A_POE, 0x0303);
    //vWritePWM(A_POE, 0x01);
    ///Disable Channel 0~5
    //vWritePWM1(A_POE, 0x00);

///#define PWM_USE_APLL1_294M
#ifdef PWM_USE_APLL1_294M
    vIO32WriteFldAlign(CKGEN_PWM_CKCFG, APLL_D1, FLD_PWM_CK_SEL);  //CLK Gen to DAC APLL                    
    vIO32WriteFldAlign(CKGEN_PWM2_CKCFG, APLL_D1, FLD_PWM2_CK_SEL);  //CLK Gen to DAC APLL                    
    #ifdef LOG_REG_WRITE
    Printf("[!!!PWM!!!] I2C_EXT 58 0000d244[3:0] %08x RW\n",APLL_D1);
    Printf("[!!!PWM!!!] I2C_EXT 58 0000d248[3:0] %08x RW\n",APLL_D1);
    #endif

  #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    ANA_WRITE32(REG_PWMDAC_CFG0, 0x000000FF); //APLL1 + Sel APLL + APLL non-inverse + Clock Relatch 
  #elif defined(CC_MT5365) || defined(CC_MT5395)
    AUD_WRITE32(REG_PWMIP_PWMPAD_CFG,0xFF);   ///w 0xf00051ec 0x000000FF    
  #endif
    vWritePWM(0xf02, 0x36FF9c);
    vWritePWM1(0xf02, 0x36FF9c);
    vWritePWM(0xf08, 0x115);
    vWritePWM(0xf09, 0xf0600);
    vWritePWM(0xf08, 0x146);
    vWritePWM(0xf09, 0xf0600);
#else
    vIO32WriteFldAlign(CKGEN_PWM_CKCFG, SAWLESSPLL_D1, FLD_PWM_CK_SEL);  //CLK Gen to DAC APLL                    
    vIO32WriteFldAlign(CKGEN_PWM2_CKCFG, SAWLESSPLL_D1, FLD_PWM2_CK_SEL);  //CLK Gen to DAC APLL                    
    #ifdef LOG_REG_WRITE
    Printf("[!!!PWM!!!] I2C_EXT 58 0000d244[3:0] %08x RW\n",SAWLESSPLL_D1);
    Printf("[!!!PWM!!!] I2C_EXT 58 0000d248[3:0] %08x RW\n",SAWLESSPLL_D1);
    #endif    
  #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    ANA_WRITE32(REG_PWMDAC_CFG0, 0x00FF00FF);    //APLL1 + Sel 432MHz + APLL non-inverse + Clock Relatch 
  #elif defined(CC_MT5365) || defined(CC_MT5395)
    AUD_WRITE32(REG_PWMIP_PWMPAD_CFG,0xFF00FF);   ///w 0xf00051ec 0x000000FF
  #endif  
    vWritePWM(0xf02, 0x36EDAC);
    vWritePWM1(0xf02, 0x36EDAC);
    vWritePWM(0xf08, 0x115);
    vWritePWM(0xf09, 0xD8000);
    vWritePWM(0xf08, 0x146);
    vWritePWM(0xf09, 0xD8000);
#endif

    do
    {
        /// Check interrupt!
        for(Yi=0;Yi<48;Yi++)
        ///for(Yi=0;Yi<8;Yi++)
        {
wait_int:
            AUD_WRITE32(REG_INDIRECT0_ADDR, A_PSTAT);
            i=AUD_READ32(REG_INDIRECT0_DATA);
            if((i&0x8000) != 0)    /// Every 48KHz interrupt one time.
            {
                //vWritePWM(A_PDATA, 0x4FFF00);   /// Set data from RISC
                //vWritePWM(A_PDATA, 0x5FFF00);   /// Set data from RISC
                //vWritePWM(A_PDATA, 0x6FFF00);   /// Set data from RISC
                ///vWritePWM(A_PDATA, 0x00);   /// Set data from RISC
                vWritePWM(A_PDATA, Risc_1K_0dB_tone[Yi]);   /// Set data from RISC
                //vWritePWM(A_PDATA, Risc_6K_0dB_tone[Yi]);   /// Set data from RISC
                vWritePWM(A_PSTAT, 0x8000);     /// Clean interrupt
            }
            else
            {
                goto wait_int;
            }
        }
    }while(1);
#endif

}

#ifdef XY_RAM_TEST
void _MT5365_XYRamTest(BOOL xy)
{
	UINT32 i, data;
	if(xy==0)
	{
		vWritePWM(0xf01,0x200);
		vWritePWM1(0xf01,0x200);
		Printf("******************************PG1 X Ram********************************\n");
		for(i=0;i<256;i++)
		{
			vWritePWM(0xf08, i);
			vWritePWM(0xf09, i+1);
        	data=AUD_READ32(REG_INDIRECT0_DATA);
			Printf("xram %d=",i);
			Printf("%d,  ",data);
			if((i+1)%4==0)
			{
				Printf("\n");	
			}	
		}
		Printf("******************************PG2 X Ram********************************\n");
		for(i=0;i<256;i++)
		{
			vWritePWM1(0xf08, i);
			vWritePWM1(0xf09, i+1);
        	data=AUD_READ32(REG_INDIRECT1_DATA);
			Printf("xram %d=",i);
			Printf("%d,  ",data);
			if((i+1)%4==0)
			{
				Printf("\n");	
			}	
		}
		vWritePWM(0xf01,0xf542ff);
		vWritePWM1(0xf01,0xf542ff);
	}
	else if(xy==1)
	{
		vWritePWM(0xf01,0x200);
		vWritePWM1(0xf01,0x200);
		Printf("******************************PG1 Y Ram********************************\n");
		for(i=0;i<75;i++)
		{
			vWritePWM(0xf08, i+256);
			vWritePWM(0xf09, YRAM[i]);
        	data=AUD_READ32(REG_INDIRECT0_DATA);
			Printf("yram %d=",i);
			Printf("0x%x,  ",data);
			if((i+1)%4==0)
			{
				Printf("\n");	
			}	
		}
		Printf("******************************PG2 Y Ram********************************\n");
		for(i=0;i<75;i++)
		{
			vWritePWM1(0xf08, i+256);
			vWritePWM1(0xf09, YRAM[i]);
        	data=AUD_READ32(REG_INDIRECT1_DATA);
			Printf("yram %d=",i);
			Printf("0x%x,  ",data);
			if((i+1)%4==0)
			{
				Printf("\n");	
			}	
		}
		vWritePWM(0xf01,0xf542ff);
		vWritePWM1(0xf01,0xf542ff);	
	}
}
#endif
#ifndef CC_AUD_ANALOG_INPUT_SWITCH_PROTECT
void _MT5365_ADCDOWN(BOOL fgEnalbe)
{
    BOOL fInternalLDO = 0;
    UNUSED( fInternalLDO );
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_ADCDOWN: %d\n", fgEnalbe);
#endif
  #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    if (fgEnalbe)
    {
        ANA_SET_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD));
        fInternalLDO = (BOOL)DRVCUST_OptGet(eAudioInternalLDOEnable);
        if( fInternalLDO )
        {
            ///Internal LDO will refer VMID voltage.
            ANA_SET_BIT(REG_PWMDAC_CFG5, ADC_VCMGEN_1M_R_ON);   ///Enable VCMGEN 1M Resister, before power on.
        }
        else
        {
            ANA_CLR_BIT(REG_PWMDAC_CFG5, ADC_VCMGEN_1M_R_ON);   ///Disable VCMGEN 1M Resister, after power down.
        }
    }
    else
    {
        ANA_SET_BIT(REG_PWMDAC_CFG5, ADC_VCMGEN_1M_R_ON);   ///Enable VCMGEN 1M Resister, before power on.
        ANA_CLR_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD)); // power on
    }
  #elif defined(CC_MT5365) || defined(CC_MT5395)
    if (fgEnalbe)
    {
        AUD_SET_BIT(REG_INTADC_SDADC2, (ADC_L_PD | ADC_R_PD | LEFT_PD | RIGHT_PD));
    }
    else
    {
        AUD_CLR_BIT(REG_INTADC_SDADC2, (ADC_L_PD | ADC_R_PD | LEFT_PD | RIGHT_PD)); // power on
    }
  #endif    
}
#else
void _MT5365_ADCDOWN(BOOL fgEnalbe)
{
    UINT32 u4Reg, u4Reg1, u4Gpio_no;

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_ADCDOWN: %d\n", fgEnalbe);
#endif
    
    if (fgEnalbe)
    {
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) | ADC_L_PD | ADC_R_PD | ADC_GPIO_EN));    // power down
        ANA_CLR_BIT(REG_PWMDAC_CFG5, ADC_VCMGEN_1M_R_ON);   ///Disable VCMGEN 1M Resister.
    #elif defined(CC_MT5365) || defined(CC_MT5395)    
        AUD_WRITE32(REG_INTADC_SDADC2, (AUD_READ32(REG_INTADC_SDADC2) | ADC_L_PD | ADC_R_PD | LEFT_PD | RIGHT_PD | ADC_GPIO_EN ));    // power down
    #endif

    #if !defined(CC_MT5389)
        BSP_PinSet(PIN_AIN0_L, 1);
        GPIO_SetOut(GPIO_AIN0_L,0);
        GPIO_SetOut(GPIO_AIN0_R,0);
    #endif        
        BSP_PinSet(PIN_AIN1_L, 1);
        GPIO_SetOut(GPIO_AIN1_L,0);
        GPIO_SetOut(GPIO_AIN1_R,0);
        BSP_PinSet(PIN_AIN2_L, 1);
        GPIO_SetOut(GPIO_AIN2_L,0);
        GPIO_SetOut(GPIO_AIN2_R,0);
    #if !defined(CC_MT5389)        
        BSP_PinSet(PIN_AIN3_L, 1);
        GPIO_SetOut(GPIO_AIN3_L,0);
        GPIO_SetOut(GPIO_AIN3_R,0);
        BSP_PinSet(PIN_AIN4_L, 1);
        GPIO_SetOut(GPIO_AIN4_L,0);
        GPIO_SetOut(GPIO_AIN4_R,0);
        BSP_PinSet(PIN_AIN5_L, 1);
        GPIO_SetOut(GPIO_AIN5_L,0);
        GPIO_SetOut(GPIO_AIN5_R,0);
        BSP_PinSet(PIN_AIN6_L, 1);
        GPIO_SetOut(GPIO_AIN6_L,0);
        GPIO_SetOut(GPIO_AIN6_R,0);
    #endif        
        LOG(0, "ADC POWERDOWN!\n");
    }
    else
    {
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        ANA_CLR_BIT(REG_AADC_CFG0, ADC_L_PD);     // power on
        ANA_CLR_BIT(REG_AADC_CFG0, ADC_R_PD);
        u4Reg = ANA_READ32(REG_AADC_CFG0)&ADC_INPUT_MUX_MASK;                
    #elif defined(CC_MT5365) || defined(CC_MT5395)
        AUD_CLR_BIT(REG_INTADC_SDADC2, ADC_L_PD);     // power on
        AUD_CLR_BIT(REG_INTADC_SDADC2, ADC_R_PD);
        AUD_CLR_BIT(REG_INTADC_SDADC2, LEFT_PD);
        AUD_CLR_BIT(REG_INTADC_SDADC2, RIGHT_PD);
        u4Reg = AUD_READ32(REG_INTADC_SDADC2)&ADC_INPUT_MUX_MASK;        
    #endif        

        u4Reg = u4Reg>> 20;
        switch (u4Reg)
        {
            case 0:
        	  u4Reg1 = ADC_GPIO0;
        	  u4Gpio_no = PIN_AIN0_L;
        	  break;
            case 1:
        	  u4Reg1 = ADC_GPIO1;
        	  u4Gpio_no = PIN_AIN1_L;
        	  break;
            case 2:
              u4Reg1 = ADC_GPIO2;
              u4Gpio_no = PIN_AIN2_L;
        	  break;
            case 3:
        	  u4Reg1 = ADC_GPIO3;
        	  u4Gpio_no = PIN_AIN3_L;
        	  break;
            case 4:
        	  u4Reg1 = ADC_GPIO4;
        	  u4Gpio_no = PIN_AIN4_L;
        	  break;	
            case 5:
        	  u4Reg1 = ADC_GPIO5;
        	  u4Gpio_no = PIN_AIN5_L;
        	  break;
            case 6:
        	  u4Reg1 = ADC_GPIO6;
        	  u4Gpio_no = PIN_AIN6_L;
        	  break;
            default:
        	  u4Reg1 = ADC_GPIO0;
        	  u4Gpio_no = PIN_AIN0_L;
        	  break;
        }
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        ANA_CLR_BIT(REG_AADC_CFG0, u4Reg1);
    #elif defined(CC_MT5365) || defined(CC_MT5395)
        AUD_CLR_BIT(REG_INTADC_SDADC2, u4Reg1);
    #endif    
        if (u4Reg!=7) BSP_PinSet(u4Gpio_no, 0);
                
        LOG(0, "ADC POWERON!\n");
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        ANA_SET_BIT(REG_PWMDAC_CFG5, ADC_VCMGEN_1M_R_ON);   ///Enable VCMGEN 1M Resister.
        //ANA_CLR_BIT(REG_AADC_CFG0, ADC_GPIO_EN);
    #elif defined(CC_MT5365) || defined(CC_MT5395)
        //AUD_WRITE32(REG_INTADC_SDADC2, (AUD_READ32(REG_INTADC_SDADC2) & 0x807fffff));
    #endif    
    }
}
#endif

///PWMDAC digital mute.
void _MT5365_MuteCLK(UINT8 u1DacId,BOOL fgMute)
{
    UNUSED(u1DacId);

    LOG(0, "[!!!PWM_FUNC!!!] _MT5365_MuteCLK: AUD(%d) fgMute = %d\n", u1DacId, fgMute);

    //DAC mute(can mute any clock pop noise, DC level from 3.3V down to 0V)
    if (fgMute) // Mute, fadeout
    {  
        /// A_POE = 0x333333 can't do fade-in / out. 
        /// Need change back to 0x030303.
		vWritePWM(A_POE, 0x030303);
		vWritePWM1(A_POE, 0x030303);

        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_ON);
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x1F<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x1F<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));
    }
    else // UnMute, fadein (Already fade-out)
    {
        ///For pwm async fifo enable and write/read pointer margin 
        ///before A_PGCTRL0 0xF01[0]=1
        AUD_WRITE32(REG_PWMIP_MON_PG1, 0x00);
        AUD_WRITE32(REG_PWMIP_MON_PG1, PWMIP_MON_PG1_INIT);

        // 1. A_POE pwm channel disable.
		vWritePWM(A_POE, 0x000000);
		vWritePWM1(A_POE, 0x000000);

		// 2. Set A_PGTRL0 0xf01 bit0 = 1 enable PWM 
        vWritePWM(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWM(A_PDATA, 0);
        vWritePWM1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWM1(A_PDATA, 0);

		// 3. Fade-out first - fast speed
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00<<26)|(0x100<<16)|(0x01<<8)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00<<26)|(0x100<<16)|(0x01<<8)));
        AUD_WRITE32(REG_PWMIP_MISC1, 0);    ///[11:0]=0 start from 0
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));

		// 4. Delay time for fade-out using.
		x_thread_delay(1);

		// 5. A_POE pwm channel enable.
		vWritePWM(A_POE, 0x030303);
		vWritePWM1(A_POE, 0x030303);

		// 6. Fade-in
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x05<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x05<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF);
    }
}

#ifdef CC_MUTE_BY_ALRx
void _MT5365_Mute(UINT8 u1ALRx,BOOL fgMute)
{
    UINT8 temp_ch = 0;    
    UNUSED(temp_ch);
    
    if ((!fgPWMInit)||fgPopMute)
    {
        return;
    }   
            
    switch(u1ALRx)
    {
        case 0:
            temp_ch = (UINT8)( AUD_READ32(REG_PWMIP_PINMUX_PG1)&0xF);
            break;
        case 1:
            temp_ch = (UINT8)(( AUD_READ32(REG_PWMIP_PINMUX_PG1)>>8 )&0xF );
            break;
        case 2:
            temp_ch = (UINT8)(( AUD_READ32(REG_PWMIP_PINMUX_PG1)>>16 )&0xF );
            break;
        case 3:
            temp_ch = (UINT8)(( AUD_READ32(REG_PWMIP_PINMUX_PG2)>>8 )&0xF);
            break;
        default:
            break;
    } 
    if(fgMute)
    {
        switch(temp_ch)
        {
            case 0:
            case 1:
               AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE); 
                vWritePWM( A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) | 0x000030) ); //f04 = 0x030333
                break;
            case 2:
            case 3:
                AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE);
                vWritePWM(A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) | 0x003000) ); //f04 = 0x033303
                break;
            case 6:
            case 7:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1(A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) | 0x000030) ); //f04 = 0x030333
                break;
            case 8:
            case 9:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1( A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) | 0x003000 )); //f04 = 0x033303
                break;
           default:
                break;
        }
    }
    else
    {
        switch(temp_ch)
        {
            case 0:
            case 1:
                AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE);
                vWritePWM( A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) & 0xffff0f) ); //f04 = 0x030303
                break;
            case 2:
            case 3:
                AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE);
                vWritePWM(A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) & 0xff0fff) ); //f04 = 0x030303
                break;
            case 6:
            case 7:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1( A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) & 0xffff0f )); //f04 = 0x030303
                break;
            case 8:
            case 9:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1( A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) & 0xff0fff) ); //f04 = 0x030303
                break;
            default:
                break;
        }    
    }
}


#else
void _MT5365_Mute(UINT8 u1DacId,BOOL fgMute)
{
    UNUSED(u1DacId);

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_Mute: %d %d\n", u1DacId, fgMute);
#endif

    if ((!fgPWMInit)||fgPopMute)
    {
        return;
    }

    //PGA mute
    if (fgMute) // Mute
    {  
        //turn on 0xf04
        vWritePWM(A_POE, 0x333333);
        vWritePWM1(A_POE, 0x333333);
    }
    else // UnMute
    {
        //turn off 0xf04
        vWritePWM(A_POE, 0x030303);
        vWritePWM1(A_POE, 0x030303);
    }
}
#endif

void _MT5365_PopMute(BOOL fgMute)
{

    fgPopMute = fgMute;
    //PGA mute
    if (fgMute) // Mute
    {  
        //turn on 0xf04
        vWritePWM(A_POE, 0x333333);
        vWritePWM1(A_POE, 0x333333);
    }
    else // UnMute
    {
        //turn off 0xf04
        vWritePWM(A_POE, 0x030303);
        vWritePWM1(A_POE, 0x030303);
    }
}


void _MT5365_MuteALRx(UINT8 u1ALRx,BOOL fgMute)
{
    UINT8 temp_ch = 0;    
    UNUSED(temp_ch);
    
    if ((!fgPWMInit)||fgPopMute)
    {
        return;
    }   
            
    switch(u1ALRx)
    {
        case 0:
            temp_ch = (UINT8)( AUD_READ32(REG_PWMIP_PINMUX_PG1)&0xF);
            break;
        case 1:
            temp_ch = (UINT8)(( AUD_READ32(REG_PWMIP_PINMUX_PG1)>>8 )&0xF );
            break;
        case 2:
            temp_ch = (UINT8)(( AUD_READ32(REG_PWMIP_PINMUX_PG1)>>16 )&0xF );
            break;
        case 3:
            temp_ch = (UINT8)(( AUD_READ32(REG_PWMIP_PINMUX_PG2)>>8 )&0xF);
            break;
        default:
            break;
    } 
    if(fgMute)
    {
        switch(temp_ch)
        {
            case 0:
            case 1:
               AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE); 
                vWritePWM( A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) | 0x000030) ); //f04 = 0x030333
                break;
            case 2:
            case 3:
                AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE);
                vWritePWM(A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) | 0x003000) ); //f04 = 0x033303
                break;
            case 6:
            case 7:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1(A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) | 0x000030) ); //f04 = 0x030333
                break;
            case 8:
            case 9:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1( A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) | 0x003000 )); //f04 = 0x033303
                break;
           default:
                break;
        }
    }
    else
    {
        switch(temp_ch)
        {
            case 0:
            case 1:
                AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE);
                vWritePWM( A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) & 0xffff0f) ); //f04 = 0x030303
                break;
            case 2:
            case 3:
                AUD_WRITE32(REG_INDIRECT0_ADDR, A_POE);
                vWritePWM(A_POE, ((AUD_READ32(REG_INDIRECT0_DATA)) & 0xff0fff) ); //f04 = 0x030303
                break;
            case 6:
            case 7:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1( A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) & 0xffff0f )); //f04 = 0x030303
                break;
            case 8:
            case 9:
                AUD_WRITE32(REG_INDIRECT1_ADDR, A_POE);
                vWritePWM1( A_POE, ((AUD_READ32(REG_INDIRECT1_DATA)) & 0xff0fff) ); //f04 = 0x030303
                break;
            default:
                break;
        }    
    }
}


void _MT5365_InterPWMEnable(BOOL fgEnable)
{
    if (!fgPWMInit)
    {
        return;
    } 
    if(fgEnable)
    {
        ///For pwm async fifo enable and write/read pointer margin 
        ///before A_PGCTRL0 0xF01[0]=1
        AUD_WRITE32(REG_PWMIP_MON_PG1, 0x00);
        AUD_WRITE32(REG_PWMIP_MON_PG1, PWMIP_MON_PG1_INIT);
        
        AUD_WRITE32(REG_INDIRECT0_ADDR, A_PGCTRL0);
        vWritePWM( A_PGCTRL0, ((AUD_READ32(REG_INDIRECT0_DATA)) | 0x1 )); //f542ff
        AUD_WRITE32(REG_INDIRECT1_ADDR, A_PGCTRL0);
        vWritePWM1( A_PGCTRL0, ((AUD_READ32(REG_INDIRECT1_DATA)) | 0x1 )); //f542ff
    }
    else
    {
        AUD_WRITE32(REG_INDIRECT0_ADDR, A_PGCTRL0);
        vWritePWM( A_PGCTRL0, ((AUD_READ32(REG_INDIRECT0_DATA)) & 0xfffffe )); //f542fe
        AUD_WRITE32(REG_INDIRECT1_ADDR, A_PGCTRL0);
        vWritePWM1( A_PGCTRL0, ((AUD_READ32(REG_INDIRECT1_DATA)) & 0xfffffe )); //f542fe
    }
}

UINT32 _MT5365_InterPWMGainGet()
{
    UINT32 pgain;
    if (!fgPWMInit)
    {
        return FALSE;
    } 
    _MT5365_InterPWMEnable(FALSE);
    vWritePWM(A_PCADDR, 0x115);             ///AL0
    AUD_WRITE32(REG_INDIRECT0_ADDR, A_PCDATA);
    pgain = (UINT32)(AUD_READ32(REG_INDIRECT0_DATA));
    _MT5365_InterPWMEnable(TRUE);

    return pgain;
}
void _MT5365_InterPWMGainSet(INT32 pgain)
{
    if (!fgPWMInit)
    {
        return;
    } 

    _MT5365_InterPWMEnable(FALSE);
    ///PWMGroup1
    vWritePWM(A_PCADDR, 0x115);             ///AL0
    vWritePWM(A_PCDATA, pgain);   
    vWritePWM(A_PCADDR, 0x146);             ///AR0
    vWritePWM(A_PCDATA, pgain);
    vWritePWM(A_PCADDR, 0x147);             ///AL1
    vWritePWM(A_PCDATA, pgain);
    vWritePWM(A_PCADDR, 0x148);             ///AR1
    vWritePWM(A_PCDATA, pgain);
    vWritePWM(A_PCADDR, 0x149);             ///
    vWritePWM(A_PCDATA, pgain);
    vWritePWM(A_PCADDR, 0x14A);             ///
    vWritePWM(A_PCDATA, pgain);
    ///PWM Group2
    vWritePWM1(A_PCADDR, 0x115);            ///AL2
    vWritePWM1(A_PCDATA, pgain);   
    vWritePWM1(A_PCADDR, 0x146);            ///AR2
    vWritePWM1(A_PCDATA, pgain);
    vWritePWM1(A_PCADDR, 0x147);            ///AL3
    vWritePWM1(A_PCDATA, pgain);
    vWritePWM1(A_PCADDR, 0x148);            ///AR3
    vWritePWM1(A_PCDATA, pgain);
    vWritePWM1(A_PCADDR, 0x149);            ///
    vWritePWM1(A_PCDATA, pgain);
    vWritePWM1(A_PCADDR, 0x14A);            ///
    vWritePWM1(A_PCDATA, pgain);
    
    _MT5365_InterPWMEnable(TRUE);
}
void _MT5365_InterDacEnable(BOOL fgEnable)
{

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_InterDacEnable: %d\n", fgEnable);
#endif

    if (!fgPWMInit)
    {
        return;
    }

    //PGA mute
    if (!fgEnable) // Mute
    {  
    #if defined(CC_MT5396)
		vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL0AR0);
		vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL1AR1);
		vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL2AR2);
		vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL3AR3);
    #elif defined(CC_MT5368) || defined(CC_MT5389)
        //can't switch to GPIO function
    #elif defined(CC_MT5395)
		vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL0AR0);
		vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL1AR1);
		vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL2AR2);
		vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL3AR3);
    #elif defined(CC_MT5365)
		vIO32WriteFldAlign(CKGEN_PMUX1, 3, FLD_PAD_AL0AR0);
		vIO32WriteFldAlign(CKGEN_PMUX1, 3, FLD_PAD_AL1AR1);
		vIO32WriteFldAlign(CKGEN_PMUX1, 1, FLD_PAD_AL2AR2);
		vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL3AR3);		
    #endif
    }
    else // UnMute
    {
    #if defined(CC_MT5396)
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL0AR0);
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL2AR2);
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL3AR3);
    #elif defined(CC_MT5368) || defined(CC_MT5389)
        //can't switch to GPIO function    
    #elif defined(CC_MT5395)
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_AL0AR0);
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_AL1AR1);
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_AL2AR2);
		vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_AL3AR3);
    #elif defined(CC_MT5365)
		vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AL0AR0);
		vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AL1AR1);
		vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AL2AR2);
		vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL3AR3);	
    #endif
    }
}


void _MT5365_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_AdcFmtCfg: %d %d\n", eFormat, eMclk);
#endif

    switch (eFormat)
    {
        case FORMAT_LJ:
        	break;
        case FORMAT_I2S:
        	break;
        case FORMAT_RJ:
        	break;
        default:
        	break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
        	break;
        case MCLK_256FS:
        	break;
        case MCLK_384FS:
        	break;
        case MCLK_512FS:
            break;
        default:
        	break;
    }
    
}

void _MT5365_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_DacFmtCfg: %d %d\n", eFormat, eMclk);
#endif

    switch (eFormat)
    {
        case FORMAT_LJ:
        	break;
        case FORMAT_I2S:
        	break;
        case FORMAT_RJ:
        	break;
        default:
        	break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
        	break;
        case MCLK_256FS:
        	break;
        case MCLK_384FS:
        	break;
        case MCLK_512FS:
            break;
        default:
        	break;
    }
    
}
#ifndef CC_AUD_ANALOG_INPUT_SWITCH_PROTECT
void _MT5365_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg;

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_ChlSel: %d\n", u1Indx);
#endif
    
    switch (u1Indx)
    {
        case 0:
        	u4Reg = ADC_IN0L0R;
        	break;
        case 1:
        	u4Reg = ADC_IN1L1R;
        	break;
        case 2:
        	u4Reg = ADC_IN2L2R;
        	break;
        case 3:
        	u4Reg = ADC_IN3L3R;
        	break;
        case 4:
        	u4Reg = ADC_IN4L4R;
        	break;	
        case 5:
        	u4Reg = ADC_IN5L5R;
        	break;
        case 6:
        	u4Reg = ADC_IN6L6R;
        	break;
        default:
        	u4Reg = ADC_IN0L0R;
        	break;
    }
    // there is corsstalk mux output to AV1. 
	//Solution: when audio input not used, set it as gpio, and pull low
	#ifdef CC_MT5389
	if(u4Reg == ADC_IN2L2R)//AV , set another pin as gpio out, and pull low
	{
		//another pin as GPO
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 1, Fld(1,16,AC_MSKB2)); //d524[16] = 1
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 1, Fld(1,14,AC_MSKB1)); //d524[14] = 1
	   vIO32WriteFldAlign(CKGEN_AGPIO_CTRL, 1, Fld(1,9,AC_MSKB1)); //d1d4[9] = 1

		//AV as audio input
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 0, Fld(1,17,AC_MSKB2)); //d524[17] = 0
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 0, Fld(1,15,AC_MSKB1)); //d524[15] = 0
	   vIO32WriteFldAlign(CKGEN_AGPIO_CTRL, 0, Fld(1,10,AC_MSKB1)); //d1d4[10] = 0
	}
	else if(u4Reg == ADC_IN1L1R)  //another input, set AV as gpio, and pull low
	{
		//another pin as audio input
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 0, Fld(1,16,AC_MSKB2)); //d524[16] = 0
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 0, Fld(1,14,AC_MSKB1)); //d524[14] = 0
	   vIO32WriteFldAlign(CKGEN_AGPIO_CTRL, 0, Fld(1,9,AC_MSKB1)); //d1d4[9] = 0

		//AV as audio GPO
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 1, Fld(1,17,AC_MSKB2)); //d524[17] = 1
	   vIO32WriteFldAlign(CKGEN_GPIOEN1, 1, Fld(1,15,AC_MSKB1)); //d524[15] = 1
	   vIO32WriteFldAlign(CKGEN_AGPIO_CTRL, 1, Fld(1,10,AC_MSKB1)); //d1d4[10] = 1
	}
	#endif
	
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~(ADC_INPUT_MUX_MASK)) | u4Reg);
    #elif defined(CC_MT5365) || defined(CC_MT5395)
    AUD_WRITE32(REG_INTADC_SDADC2, (AUD_READ32(REG_INTADC_SDADC2) & ~(ADC_INPUT_MUX_MASK)) |u4Reg);
    #endif
}
#else
void _MT5365_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg, u4Reg1, u4Gpio_no;

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_ChlSel: %d\n", u1Indx);
#endif
    
    switch (u1Indx)
    {
        case 0:
        	u4Reg = ADC_IN0L0R;
        	u4Reg1 = ADC_GPIO0;
        	u4Gpio_no = PIN_AIN0_L;
        	break;
        case 1:
        	u4Reg = ADC_IN1L1R;
        	u4Reg1 = ADC_GPIO1;
        	u4Gpio_no = PIN_AIN1_L;
        	break;
        case 2:
        	u4Reg = ADC_IN2L2R;
        	u4Reg1 = ADC_GPIO2;
        	u4Gpio_no = PIN_AIN2_L;
        	break;
        case 3:
        	u4Reg = ADC_IN3L3R;
        	u4Reg1 = ADC_GPIO3;
        	u4Gpio_no = PIN_AIN3_L;
        	break;
        case 4:
        	u4Reg = ADC_IN4L4R;
        	u4Reg1 = ADC_GPIO4;
        	u4Gpio_no = PIN_AIN4_L;
        	break;	
        case 5:
        	u4Reg = ADC_IN5L5R;
       	    u4Reg1 = ADC_GPIO5;
       	    u4Gpio_no = PIN_AIN5_L;
        	break;
        case 6:
        	u4Reg = ADC_IN6L6R;
        	u4Reg1 = ADC_GPIO6;
        	u4Gpio_no = PIN_AIN6_L;
        	break;
        default:
        	u4Reg = ADC_IN0L0R;
        	u4Reg1 = ADC_GPIO0;
        	u4Gpio_no = PIN_AIN0_L;
        	break;
    }

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    if (ANA_READ32(REG_AADC_CFG0)&0x3000)
    {
        //ADC is power-down, set swtich only
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~(ADC_INPUT_MUX_MASK)) |u4Reg);
    }
    else
    {
        //ADC is not power-down, set swtich and gpio at the same time
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) | ADC_GPIO_EN ));    // power down GPIO   
        if (u1Indx!=7) BSP_PinSet(u4Gpio_no, 0);
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~(ADC_INPUT_MUX_MASK)) |u4Reg);
        ANA_CLR_BIT(REG_AADC_CFG0, u4Reg1);
    }
#elif defined(CC_MT5365) || defined(CC_MT5395)
    if (AUD_READ32(REG_INTADC_SDADC2)&0xf000)
    {
        //ADC is power-down, set swtich only
        AUD_WRITE32(REG_INTADC_SDADC2, (AUD_READ32(REG_INTADC_SDADC2) & ~(ADC_INPUT_MUX_MASK)) |u4Reg);
    }
    else
    {
        //ADC is not power-down, set swtich and gpio at the same time
        AUD_WRITE32(REG_INTADC_SDADC2, (AUD_READ32(REG_INTADC_SDADC2) | ADC_GPIO_EN ));    // power down GPIO	
        if (u1Indx!=7) BSP_PinSet(u4Gpio_no, 0);
        AUD_WRITE32(REG_INTADC_SDADC2, (AUD_READ32(REG_INTADC_SDADC2) & ~(ADC_INPUT_MUX_MASK)) |u4Reg);
        AUD_CLR_BIT(REG_INTADC_SDADC2, u4Reg1);
    }
#endif    
}

#endif

void _MT5365_DacChlSel(UINT8 u1DacId, AUD_CHL_DEF_T eChl)
{
    UINT32 u4DacPos, u4DacSel;

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_DacChlSel: %d %d\n", u1DacId, eChl);
#endif

    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        u4DacPos = DAC0_SEL_POS;
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        u4DacPos = DAC1_SEL_POS;
    } 
    else if (u1DacId == INTERNAL_DAC_ID_THIRD)
    {
        u4DacPos = DAC2_SEL_POS;
    }
    else
    {
        u4DacPos = DAC3_SEL_POS;
    }

    if (eChl == AUD_CHL_L_R)
    {
        u4DacSel = DAC_SEL_0_LR;
    }
    else if (eChl == AUD_CHL_LS_RS)
    {
        u4DacSel = DAC_SEL_0_SLR;
    }
    else if (eChl == AUD_CHL_C_SW)
    {
        u4DacSel = DAC_SEL_0_CLFE;
    }
    else if (eChl == AUD_CHL_BYPASS)
    {
        u4DacSel = DAC_SEL_0_CH78;
    }
    else if (eChl == AUD_CHL_DOWNMIX)
    {
        u4DacSel = DAC_SEL_0_CH90;
    }
    else
    {
        u4DacSel = DAC_SEL_1_LR;
    }

    u4DacSel = u4DacSel << u4DacPos;

    AUD_WRITE32(REG_IDACHCG,
        ((AUD_READ32(REG_IDACHCG) & (~(DAC_SEL_MASK << u4DacPos))) | u4DacSel));

#ifdef CC_AUDIO_EU_PWM_OUTPUT_AUXDEC
    if(u1DacId == INTERNAL_DAC_ID_4TH)
    {
        if(eChl == AUD_CHL_AUX)
            AUD_WRITE32(REG_PWMCFG0_PG2, (AUD_READ32(REG_PWMCFG0_PG2)|(0x01<<14))); ///(0x01<<14) PWMG2 refer Aout1 clock (not Aout0)
        else
            AUD_WRITE32(REG_PWMCFG0_PG2, (AUD_READ32(REG_PWMCFG0_PG2)&0xFFFFBFFF)); ///(0x00<<14) PWMG2 refer Aout0 clock
    }
#endif
}

static CHAR* _aszDacCh[] = {"LR","SLR","CLFE","CH78","CH910","DEC2LR"};

void _MT5365_DacChlSelQuery(void)
{
    UINT8 u1Index;
    UINT32 u4RegVal = AUD_READ32(REG_IDACHCG);

    u1Index = (u4RegVal>>DAC0_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 6)
        Printf("DAC0: %s\n", _aszDacCh[u1Index]);
    u1Index = (u4RegVal>>DAC1_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 6)    
        Printf("DAC1: %s\n", _aszDacCh[u1Index]);
    u1Index = (u4RegVal>>DAC2_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 6)    
        Printf("DAC2: %s\n", _aszDacCh[u1Index]);
    u1Index = (u4RegVal>>DAC3_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 6)    
        Printf("DAC3: %s\n", _aszDacCh[u1Index]);
}

CODEC_T* _CODEC_GetMT5365Tbl(void)
{
	return &_rCodecMT5365;
}

void _MT5365_PWM_SetSamplingRate(UINT8 u1ClkFmt)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_PWM_SetSamplingRate: %d\n", u1ClkFmt);
#endif
    _MT5365_Init_all(u1ClkFmt, FALSE);
}

static void _MT5365_Init_all(UINT8 u1ClkFmt, BOOL fgInit)
{
    UINT32 i;
    UINT32 Yi;
    BOOL fPhaseInverse = 0;
    BOOL fInternalLDO = 0;
    UINT32 PWMGain = 0;
    UINT32 PWMGain_N = 0;
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    AOUT_CFG_T *prAudOutFmt;
#endif

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_Init_all: %d %d\n", u1ClkFmt, fgInit);
#endif
    
    if (fgInit)
    {
        vIO32WriteFldAlign(CKGEN_PWM_CKCFG, PWM_CK_SEL, FLD_PWM_CK_SEL);  //CLK Gen to DAC APLL                    
        vIO32WriteFldAlign(CKGEN_PWM2_CKCFG, PWM_CK_SEL, FLD_PWM2_CK_SEL);  //CLK Gen to DAC APLL                    
#ifdef LOG_REG_WRITE
        Printf("[!!!PWM!!!] I2C_EXT 58 0000d244[3:0] %08x RW\n",PWM_CK_SEL);
        Printf("[!!!PWM!!!] I2C_EXT 58 0000d248[3:0] %08x RW\n",PWM_CK_SEL);
#endif

        AUD_WRITE32(REG_PWMIP_PINMUX_PG1, 0xFFFFFF);
        AUD_WRITE32(REG_PWMIP_PINMUX_PG2, 0xFFFFFF);
        ///For pwm async fifo enable and write/read pointer margin 
        ///before A_PGCTRL0 0xF01[0]=1
        AUD_WRITE32(REG_PWMIP_MON_PG1, 0x00);
        AUD_WRITE32(REG_PWMIP_MON_PG1, PWMIP_MON_PG1_INIT);

    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        ANA_WRITE32(REG_PWMDAC_CFG0, PWMPAD_CFG_INIT);      /// Define by CC_AUD_PWM_SAWLESS_PLL_EN
    #elif defined(CC_MT5365) || defined(CC_MT5395)
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWMPAD_CFG_INIT);
    #endif    
        AUD_WRITE32(REG_PWMIP_BTLMODE_PG1, 0);
        AUD_WRITE32(REG_PWMIP_BTLMODE_PG2, 0);
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, 0);
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, 0);
        AUD_WRITE32(REG_PWMIP_MISC1, 0);

    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
        if (prAudOutFmt->eFormat == FORMAT_I2S)
        {
            //I2S Mode for 0 degree phase error.
            //51C8/51CC: FA029200
            //5008:181E
            AUD_WRITE32(REG_PWMCFG0_PG1,0xfa029200);
            AUD_WRITE32(REG_PWMCFG0_PG2,0xfa029200);    
        }
        else
        {
            //LJ/RJ Mode for 0 degree phase error.
            //51C8/51CC: FA029000
            //5008:181A / 1812
            AUD_WRITE32(REG_PWMCFG0_PG1,0xfa029000);
            AUD_WRITE32(REG_PWMCFG0_PG2,0xfa029000);
        }
    #else
        AUD_WRITE32(REG_PWMCFG0_PG1,0xfa829200);
        AUD_WRITE32(REG_PWMCFG0_PG2,0xfa829200);
    #endif

        AUD_WRITE32(REG_INDIRECT0_ADDR, A_P0PIN);
        AUD_WRITE32(REG_INDIRECT0_DATA, C_P0PIN0);

        vWritePWM(A_P0PIN, C_P0PIN0);
        vWritePWM(A_P1PIN, C_P1PIN0);
        vWritePWM(A_P2PIN, C_P2PIN0);
        vWritePWM(A_P3PIN, C_P3PIN0);
        vWritePWM(A_P4PIN, C_P4PIN0);
        vWritePWM(A_P5PIN, C_P5PIN0);
        vWritePWM(A_P6PIN, C_P6PIN0);
        vWritePWM(A_P7PIN, C_P7PIN0);
        vWritePWM(A_P8PIN, C_P8PIN0);
        vWritePWM(A_P9PIN, C_P9PIN0);
        vWritePWM(A_P10PIN, C_P10PIN0);
        vWritePWM(A_P11PIN, C_P11PIN0);
        
        //vWritePWM(A_POE, P5EN|P4EN|P3EN|P2EN|P1EN|P0EN);

        vWritePWM(A_P1PIN, C_P1PIN0);
        vWritePWM(A_P2PIN, C_P2PIN0);
        vWritePWM(A_P3PIN, C_P3PIN0);
        vWritePWM(A_P4PIN, C_P4PIN0);
        vWritePWM(A_P5PIN, C_P5PIN0);
        vWritePWM(A_P6PIN, C_P6PIN0);
        vWritePWM(A_P7PIN, C_P7PIN0);
        vWritePWM(A_P8PIN, C_P8PIN0);
        vWritePWM(A_P9PIN, C_P9PIN0);
        vWritePWM(A_P10PIN, C_P10PIN0);
        vWritePWM(A_P11PIN, C_P11PIN0);
        
        //vWritePWM(A_POE, P5EN|P4EN|P3EN|P2EN|P1EN|P0EN);

        vWritePWM1(A_P0PIN, C_P0PIN0);
        vWritePWM1(A_P1PIN, C_P1PIN0);
        vWritePWM1(A_P2PIN, C_P2PIN0);
        vWritePWM1(A_P3PIN, C_P3PIN0);
        vWritePWM1(A_P4PIN, C_P4PIN0);
        vWritePWM1(A_P5PIN, C_P5PIN0);
        vWritePWM1(A_P6PIN, C_P6PIN0);
        vWritePWM1(A_P7PIN, C_P7PIN0);
        vWritePWM1(A_P8PIN, C_P8PIN0);
        vWritePWM1(A_P9PIN, C_P9PIN0);
        vWritePWM1(A_P10PIN, C_P10PIN0);
        vWritePWM1(A_P11PIN, C_P11PIN0);
        
        //vWritePWM1(A_POE, P5EN|P4EN|P3EN|P2EN|P1EN|P0EN);

        vWritePWM1(A_P1PIN, C_P1PIN0);
        vWritePWM1(A_P2PIN, C_P2PIN0);
        vWritePWM1(A_P3PIN, C_P3PIN0);
        vWritePWM1(A_P4PIN, C_P4PIN0);
        vWritePWM1(A_P5PIN, C_P5PIN0);
        vWritePWM1(A_P6PIN, C_P6PIN0);
        vWritePWM1(A_P7PIN, C_P7PIN0);
        vWritePWM1(A_P8PIN, C_P8PIN0);
        vWritePWM1(A_P9PIN, C_P9PIN0);
        vWritePWM1(A_P10PIN, C_P10PIN0);
        vWritePWM1(A_P11PIN, C_P11PIN0);
        
        //vWritePWM1(A_POE, P5EN|P4EN|P3EN|P2EN|P1EN|P0EN);

    }

////////////////////////////////////////////////////////////////
    fInternalLDO = (BOOL)DRVCUST_OptGet(eAudioInternalLDOEnable);
    //mute before turn of F01
    if (fgInit)
    {
        vWritePWM(A_PGCTRL1, PGCTRL1_INIT);
        if( fInternalLDO )
        {
            vWritePWM1(A_PGCTRL1, PGCTRL1_INIT_432);
        }
        else
        {
            vWritePWM1(A_PGCTRL1, PGCTRL1_INIT);
        }

        // 1. A_POE pwm channel disable.
		vWritePWM(A_POE, 0x000000);
		vWritePWM1(A_POE, 0x000000);

		///[Joel 20100610] For PWM ECO part - sync ADC & PWMDAC square rise edge.
        //vWritePWM(0xDFF, 0x01);
        //vWritePWM1(0xDFF, 0x01);

		// 2. Set A_PGTRL0 0xf01 bit0 = 1 enable PWM 
        vWritePWM(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWM(A_PDATA, 0);
        vWritePWM1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWM1(A_PDATA, 0);

		// 3. Fade-out first
		///(0x00<<26) : fast speed
		///(0xFF<<8) : Maximum step for short fade-out time
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00<<26)|(0xFF<<8)));///0x15000100);
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00<<26)|(0xFF<<8)));///0x15000100);
        AUD_WRITE32(REG_PWMIP_MISC1, 0x00); ///Initial gain = 0
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00<<26)|(0xFF<<8)|(0x3F<<0)));///0x1500013f);
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00<<26)|(0xFF<<8)|(0x3F<<0)));///0x1500013f);
    }
    else
    {
        _MT5365_Mute(0, TRUE);
    }
//////////////////////////////////////////////////////////////////
    
    
    vWritePWM(A_PIC, DACCIE);  // Enable Interrupt for DACC
    vWritePWM1(A_PIC, DACCIE);  // Enable Interrupt for DACC

    // -+-+- Clear X-Ram -+-+-
    vWritePWM(A_PCADDR, 0x000); // X RAM
    vWritePWM1(A_PCADDR, 0x000); // X RAM
    //for(i = 0; i < 37*2; i++)
    for(i = 0; i < 0xdf; i++)
    {
        vWritePWM(A_PCADDR, i); //XRAM initial
        vWritePWM(A_PCDATA, 0x0);  // Clear X RAM as 0
        vWritePWM1(A_PCADDR, i); //XRAM initial
        vWritePWM1(A_PCDATA, 0x0);  // Clear X RAM as 0
    }

    vWritePWM(A_PSTAT, 0xff00);  // Enable Interrupt for DACC
    vWritePWM1(A_PSTAT, 0xff00);  // Enable Interrupt for DACC

    //RS232LogS("Yram error");
    vWritePWM(A_PCADDR, 0x100);
    vWritePWM1(A_PCADDR, 0x100);
    for(i = 0; i < 75; i++)
    {
        Yi = 256 + i;
        /// for GP1
        vWritePWM(A_PCADDR, Yi);
        vWritePWM(A_PCDATA, YRAM[i]);
        /// for GP2
        if( fInternalLDO )
        {
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM_432[i]);
        }
        else
        {
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM[i]);
        }
        //RS232LogB(0, BYTE2(YRAM[i]), BYTE1(YRAM[i]), BYTE0(YRAM[i]));
    }
    _MT5365_PWM_SamplingRate_Fsi(u1ClkFmt);
    //Change PWM output from 1Vrms to 1.1Vrms , For PWM SNR
    if (u1ClkFmt == FS_32K)
    {
        vWritePWM(A_PCADDR, 0x115);
        vWritePWM(A_PCDATA, 0xaaa00);
        vWritePWM1(A_PCADDR, 0x115);
        vWritePWM1(A_PCDATA, 0xaaa00);
        for(i = 0x146; i < 0x14B; i++)
        {
            /// for GP1
            vWritePWM(A_PCADDR, i);
            vWritePWM(A_PCDATA, 0xAAA00);
            /// for GP2
            vWritePWM1(A_PCADDR, i);
            vWritePWM1(A_PCDATA, 0xAAA00);
        }
    }
    else if ((u1ClkFmt == FS_96K)||(u1ClkFmt == FS_176K)||(u1ClkFmt == FS_192K))
    {
        vWritePWM(A_PCADDR, 0x115);
        vWritePWM(A_PCDATA, 0xe0000);
        vWritePWM1(A_PCADDR, 0x115);
        vWritePWM1(A_PCDATA, 0xe0000);
        for(i = 0x146; i < 0x14B; i++)
        {
            /// for GP1
            vWritePWM(A_PCADDR, i);
            vWritePWM(A_PCDATA, 0xe0000);
            /// for GP2
            vWritePWM1(A_PCADDR, i);
            vWritePWM1(A_PCDATA, 0xe0000);
        }
    }
    else    // default, 44.1, 48k
    {     
        PWMGain = (UINT32)PWM_DIGITAL_GAIN;
        PWMGain_N = (UINT32)PWM_DIGITAL_GAIN_N;
        if( fInternalLDO )
        {
            #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
            PWMGain = (UINT32)PWM_DIGITAL_INTERNALLDO_GAIN;
            PWMGain_N = (UINT32)PWM_DIGITAL_INTERNALLDO_GAIN_N;
            #endif
        }
        ///PWMGroup1
        vWritePWM(A_PCADDR, 0x115);             ///AL0
        vWritePWM(A_PCDATA, PWMGain);   
        vWritePWM(A_PCADDR, 0x146);             ///AR0
        vWritePWM(A_PCDATA, PWMGain);
        vWritePWM(A_PCADDR, 0x147);             ///AL1
        vWritePWM(A_PCDATA, PWMGain);
        vWritePWM(A_PCADDR, 0x148);             ///AR1
        vWritePWM(A_PCDATA, PWMGain);
        vWritePWM(A_PCADDR, 0x149);             ///
        vWritePWM(A_PCDATA, PWMGain);
        vWritePWM(A_PCADDR, 0x14A);             ///
        vWritePWM(A_PCDATA, PWMGain);
        ///PWM Group2
        vWritePWM1(A_PCADDR, 0x115);            ///AL2
        vWritePWM1(A_PCDATA, PWMGain);   
        vWritePWM1(A_PCADDR, 0x146);            ///AR2
        vWritePWM1(A_PCDATA, PWMGain);
        vWritePWM1(A_PCADDR, 0x147);            ///AL3
        vWritePWM1(A_PCDATA, PWMGain);
        vWritePWM1(A_PCADDR, 0x148);            ///AR3
        vWritePWM1(A_PCDATA, PWMGain);
        vWritePWM1(A_PCADDR, 0x149);            ///
        vWritePWM1(A_PCDATA, PWMGain);
        vWritePWM1(A_PCADDR, 0x14A);            ///
        vWritePWM1(A_PCDATA, PWMGain);

        
        fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL1AR1PhaseInverse);
        if (fPhaseInverse)
        {
            vWritePWM(A_PCADDR, 0x115);             ///AL0
            vWritePWM(A_PCDATA, PWMGain_N);   
            vWritePWM(A_PCADDR, 0x146);             ///AR0
            vWritePWM(A_PCDATA, PWMGain_N);
        }

        fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL2AR2PhaseInverse);
        if (fPhaseInverse)
        {
            vWritePWM(A_PCADDR, 0x147);             ///AL1
            vWritePWM(A_PCDATA, PWMGain_N);
            vWritePWM(A_PCADDR, 0x148);             ///AR1
            vWritePWM(A_PCDATA, PWMGain_N);
        }

        fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL3AR3PhaseInverse);
        if (fPhaseInverse)
        {
            vWritePWM1(A_PCADDR, 0x115);            ///AL2
            vWritePWM1(A_PCDATA, PWMGain_N);   
            vWritePWM1(A_PCADDR, 0x146);            ///AR2
            vWritePWM1(A_PCDATA, PWMGain_N);
        }

        fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL4AR4PhaseInverse);
        if (fPhaseInverse)
        {
            vWritePWM1(A_PCADDR, 0x147);            ///AL3
            vWritePWM1(A_PCDATA, PWMGain_N);
            vWritePWM1(A_PCADDR, 0x148);            ///AR3
            vWritePWM1(A_PCDATA, PWMGain_N);
        }
    }

//////////////////////////////////////////////////////////////////
    if (fgInit)
    {
        // PWM Initial Stage Fade-in Setting.
        // If no fade-out time, fade-in from duty 50%.
		// 4. Delay time for fade-out using.
		x_thread_delay(1);

		// 5. A_POE pwm channel enable.
		//vWritePWM(A_POE, 0x030303);
		//vWritePWM1(A_POE, 0x030303);

		// 6. Fade-in
        // (0x0D<<26): 99.5msec
        // (0x05<<26): 42.6msec
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF);
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x0D<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x0D<<26)|(0x100<<16)|(0x01<<8)|(0x3F)));
    }
    else
    {
        _MT5365_Mute(0, FALSE);
    }
//////////////////////////////////////////////////////////////////    

    /// make DAC I2S mode and internal DAC LR output normal
    AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00983210);
    AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0x00FF76FF);

    if( fInternalLDO )
    {
        #ifdef CC_MT5389    /// Frequency Hopping
        /// 5389 AL1/AR1 use PWM GP2 Ch7/6 output 432KHz
        /// Please take care mt5389m1v2_aud.h _armt5389m1v2InterDacUsage
        AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00987610);
        #elif defined(CC_MT5396)
        //96 AL0/AR0 & AL2/AR2 use PWM GP1 :576kHz
        //    AL1/AR1 & AL3/AR3 use PWM GP2 : 432kHz
        AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00329810);
        AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0x00FF76FF);
        #endif
    }
    /// Judge AR/AL0~AR/AL3 enable or not.
    /// Set input high impedance when AR/ALx disable.
#if defined(CC_MT5396)
    if (!DRVCUST_OptGet(eAudioDacAL0AR0Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL0AR0); //GPIO function
    }
    if (!DRVCUST_OptGet(eAudioDacAL1AR1Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL1AR1); //GPIO function
    }
    if (!DRVCUST_OptGet(eAudioDacAL2AR2Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL2AR2); //GPIO function
    }
    if (!DRVCUST_OptGet(eAudioDacAL3AR3Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL3AR3); //GPIO function
    }    
#elif defined(CC_MT5368) || defined(CC_MT5389)
    //can't switch to GPIO function
#elif defined(CC_MT5395)
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL0AR0Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL0AR0);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL1AR1Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL1AR1);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL2AR2Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL2AR2);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL3AR3Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AL3AR3);
    }    
#elif defined(CC_MT5365)
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL0AR0Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX1, 3, FLD_PAD_AL0AR0);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL1AR1Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX1, 3, FLD_PAD_AL1AR1);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL2AR2Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX1, 1, FLD_PAD_AL2AR2);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioDacAL3AR3Enable);
    if (!fPhaseInverse)
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL3AR3);
    }
#endif

#ifdef CC_AUDIO_EU_PWM_OUTPUT_AUXDEC
    ///(0x01<<14) PWMG2 refer Aout1 clock (not Aout0)
    AUD_WRITE32(REG_PWMCFG0_PG2,0xFA829200|(0x01<<14));
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAuxOutAtAL0AR0);
    if (fPhaseInverse)
    {
        AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00321076);
        AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0x00FF98FF);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAuxOutAtAL1AR1);
    if (fPhaseInverse)
    {
        AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00327610);
        AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0x00FF98FF);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAuxOutAtAL2AR2);
    if (fPhaseInverse)
    {
        AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00763210);
        AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0x00FF98FF);
    }
    fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAuxOutAtAL3AR3);
    if (fPhaseInverse)
    {
        AUD_WRITE32(REG_PWMIP_PINMUX_PG1,0x00983210);
        AUD_WRITE32(REG_PWMIP_PINMUX_PG2,0x00FF76FF);
    }
#endif
}

static void _MT5365_PWM_SamplingRate_Fsi(UINT8 u1ClkFmt)
{
    int i, Yi, FSI;
    static BOOL _fgPWMRsted = FALSE;
    UINT32 u4PGCCtrlData = 0;
    BOOL fInternalLDO = 0;
    UNUSED(FSI);
    UNUSED(_fgPWMRsted);
    UNUSED(u4PGCCtrlData);

    #ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5365_PWM_SamplingRate_Fsi: %d\n", u1ClkFmt);
    #endif

#if 1
    ////////////////////////
    /// Only FS_48K case ///
    ////////////////////////

    // Enable Interrupt for DACC
    vWritePWM(A_PIC, DACCIE);  
    vWritePWM1(A_PIC, DACCIE);

    // -+-+- Clear X-Ram -+-+-
    vWritePWM(A_PCADDR, 0x000); // X RAM
    vWritePWM1(A_PCADDR, 0x000); // X RAM
    //for(i = 0; i < 37*2; i++)
    for(i = 0; i < 0xdf; i++)
    {
        /// Set GP1
        vWritePWM(A_PCADDR, i);
        vWritePWM(A_PCDATA, 0x0);  // Clear X RAM as 0
        /// Set GP2
        vWritePWM1(A_PCADDR, i);
        vWritePWM1(A_PCDATA, 0x0);  // Clear X RAM as 0
    }

    // -+-+- Load Y-Ram -+-+-
    vWritePWM(A_PCADDR, 0x100);
    vWritePWM1(A_PCADDR, 0x100);
    fInternalLDO = (BOOL)DRVCUST_OptGet(eAudioInternalLDOEnable);
    for(i = 0; i < 0x46; i++)
    {
        Yi = 256 + i;
        /// for GP1
        vWritePWM(A_PCADDR, Yi);
        vWritePWM(A_PCDATA, YRAM[i]);
        /// for GP2
        if( fInternalLDO )
        {
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM_432[i]);
        }
        else
        {
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM[i]);
        }
    }

#else
    
    if (u1ClkFmt==FS_32K)
    {
        FSI = 32000;
    }
    else if (u1ClkFmt==FS_44K)
    {
        FSI = 44100;
    }
    else if (u1ClkFmt==FS_48K)
    {
        FSI = 48000;
    }
    else if (u1ClkFmt==FS_64K)
    {
        FSI = 64000;
    }
    else if (u1ClkFmt==FS_88K)
    {
        FSI = 88200;
    }
    else if (u1ClkFmt==FS_96K)
    {
        FSI = 96000;
    }
    else if (u1ClkFmt==FS_176K)
    {
        FSI = 176400;
    }
    else if (u1ClkFmt==FS_192K)
    {
        FSI = 192000;
    }
    else if (u1ClkFmt==FS_16K)
    {
        FSI = 16000;
    }
    else if (u1ClkFmt==FS_22K)
    {
        FSI = 22050;
    }
    else if (u1ClkFmt==FS_24K)
    {
        FSI = 24000;
    }
    else
    {
        //FSI = 48000;
        FSI = 44100; // 20080829
    }

    //==============================================

    AUD_WRITE32(REG_INDIRECT0_ADDR,A_PGCTRL1);
    u4PGCCtrlData = AUD_READ32(REG_INDIRECT0_DATA);
    u4PGCCtrlData = u4PGCCtrlData & ~(0x3C);
    if ((FSI == 16000) ||(FSI == 32000))
    {
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_3X | INTPFR_4X);
    }
    else if ((FSI == 44100) || (FSI == 48000))
    {
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_48K | INTPFR_4X);
    }
    else if ((FSI == 64000))
    {
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_2X | INTPFR_4X);        
    }
    else if ((FSI == 96000))
    {
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_2X | INTPFR_2X);        
    }
    else if ((FSI == 88200) || (FSI == 176400) || (FSI == 192000))
    {
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_1X | INTPFR_2X);        
    }
    else if ((FSI == 22050) || (FSI == 24000))
    {
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_4X | INTPFR_4X);        
    }
    else
    {
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_2X | INTPFR_4X);
    }
    vWritePWM(A_PGCTRL1, u4PGCCtrlData);
    vWritePWM1(A_PGCTRL1, u4PGCCtrlData);

    vWritePWM(A_PIC, DACCIE);  // Enable Interrupt for DACC
    vWritePWM1(A_PIC, DACCIE);  // Enable Interrupt for DACC

    // -+-+- Clear X-Ram -+-+-
    vWritePWM(A_PCADDR, 0x000); // X RAM
    vWritePWM1(A_PCADDR, 0x000); // X RAM
    //for(i = 0; i < 37*2; i++)
    for(i = 0; i < 0xdf; i++)
    {
        /// Set GP1
        vWritePWM(A_PCADDR, i);
        vWritePWM(A_PCDATA, 0x0);  // Clear X RAM as 0
        /// Set GP2
        vWritePWM1(A_PCADDR, i);
        vWritePWM1(A_PCDATA, 0x0);  // Clear X RAM as 0
    }
    // -+-+- Load Y-Ram -+-+-
    if ((FSI == 16000) ||(FSI == 32000) || (FSI == 64000))  //16k, 32K, 64khz
    {
        vWritePWM(A_PCADDR, 0x100); // Y RAM
        vWritePWM1(A_PCADDR, 0x100); // Y RAM
        for(i = 0; i < 75; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM_for_32[i]);
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM_for_32[i]);
        }
    }
    else if ((FSI == 22050) ||(FSI == 24000) || (FSI == 44100) || (FSI == 48000))
    {
        vWritePWM(A_PCADDR, 0x100);
        vWritePWM1(A_PCADDR, 0x100);
        for(i = 0; i < 0x46; i++)
        {
            Yi = 256 + i;
            /// for GP1
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM[i]);
            /// for GP2
    #ifdef CC_AUD_ENABLE_INTERNAL_LDO
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM_432[i]);
    #else
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM[i]);
    #endif
        }
      
    }
    else if ((FSI == 88200) ||(FSI == 96000) || (FSI == 176400) || (FSI == 192000))
    {
        vWritePWM(A_PCADDR, 0x100);
        vWritePWM1(A_PCADDR, 0x100);
        for(i = 0; i < 75; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM_for_96[i]);
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM_for_96[i]);
        }
    }
    else //Fs=44.1 should follow Fs=48
    {
        vWritePWM(A_PCADDR, 0x100);
        vWritePWM1(A_PCADDR, 0x100);
        for(i = 0; i < 75; i++)
        {
            Yi = 256 + i;
            /// for GP1
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM[i]);
            /// for GP2
    #ifdef CC_AUD_ENABLE_INTERNAL_LDO
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM_432[i]);
    #else
            vWritePWM1(A_PCADDR, Yi);
            vWritePWM1(A_PCDATA, YRAM[i]);
    #endif
        }
    }
#endif  
}
