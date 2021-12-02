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
 * $RCSfile: codec_mt8291.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "drvcust_if.h"
#include "codec_mt5387.h"
#include "aud_debug.h"
#include "x_pinmux.h"
#include "codec_internal.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_END
//#include "Hw_ckgen.h"
#include "drvcust_if.h"
#ifdef CC_MT5387
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#endif
#include "codec_AD82581.h"
#include "aud_hw.h"
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define MT5387_ADC_VOL_MAX  (42)
#define MT5387_ADC_VOL_MIN  (-42)


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define vWritePWM(addr, data) \
    AADC_WRITE32(REG_INDIRECT0_ADDR, addr);\
    AADC_WRITE32(REG_INDIRECT0_DATA, data);\


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
static void _MT5387_Init_all(UINT8 u1ClkFmt, BOOL fgInit);
static void _MT5387_PWM_SamplingRate_Fsi(UINT8 u1ClkFmt);
#ifdef MT8223_PWM_SETTING
#define PWM_YRAM_TOP_INDX 75
#define PWM_YRAM_END_INDX 128
#else
#define PWM_YRAM_TOP_INDX 70
#define PWM_YRAM_END_INDX 128
#endif
#define DAC0 0
#define DAC1 1
#define DAC2 2

//---------------------------------------------------------------------------
// Static variables a
//---------------------------------------------------------------------------
static const UINT32 YRAM[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x0dc200, 0xfff900, 0xfff000, 0xffed00, 0xfff900, 0x001400, 0x002f00, 0x003400, 0x001400,
  0xffd500, 0xff9800, 0xff8d00, 0xffd200, 0x005300, 0x00c800, 0x00da00, 0x005900, 0xff7000, 0xfea200,
  0xfe8600, 0xff6400, 0x00ea00, 0x023e00, 0x026a00, 0x010300, 0xfe9400, 0xfc7d00, 0xfc3800, 0xfe6400,
  0x022500, 0x055f00, 0x05ce00, 0x028300, 0xfcc900, 0xf7d200, 0xf71000, 0xfc0b00, 0x04ec00, 0x0cdf00,
  0x0e6500, 0x06a000, 0xf7cd00, 0xe95700, 0xe50800, 0xf25f00, 0x124d00, 0x3d8f00, 0x66d700, 0x7fff00
#ifdef MT8223_PWM_SETTING
  ,0x0dc200, 0x0dc200, 0x0dc200, 0x0dc200, 0x0dc200
#endif
};

static const UINT32 YRAM_for_32[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x099900, 0xffff00, 0x000000, 0x000300, 0x000800, 0x000d00, 0x000e00, 0x000600, 0xfff400,
  0xffdb00, 0xffc700, 0xffc500, 0xffe200, 0x001e00, 0x006b00, 0x00a800, 0x00af00, 0x006500, 0xffcc00,
  0xff0d00, 0xfe7600, 0xfe5b00, 0xfef500, 0x003d00, 0x01d700, 0x032400, 0x037700, 0x026000, 0xffee00,
  0xfccd00, 0xfa2800, 0xf94900, 0xfb0c00, 0xff6900, 0x053b00, 0x0a6d00, 0x0ca000, 0x0a0f00, 0x026e00,
  0xf76f00, 0xec9b00, 0xe68600, 0xe96d00, 0xf7c700, 0x112600, 0x31ee00, 0x53f900, 0x701800, 0x7fff00
#ifdef MT8223_PWM_SETTING
  ,0x099900, 0x099900, 0x099900, 0x099900, 0x099900 
#endif  
};


static const UINT32 YRAM_for_96[] =
{
  0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
  0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
  0x000000, 0x0c0000, 0x000000, 0x000000, 0x000100, 0x000100, 0xfffa00, 0xfffa00, 0x001600, 0x001700,
  0xffc100, 0xffbd00, 0x009b00, 0x00a700, 0xfeae00, 0xfe8f00, 0x02a000, 0x02ee00, 0xfb1d00, 0xfa5f00,
  0x08d400, 0x0ac800, 0xef4f00, 0xe8a600, 0x2aad00, 0x7fff00, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
#ifdef MT8223_PWM_SETTING
  ,0x0c0000, 0x0c0000, 0x0c0000, 0x0c0000, 0x0c0000 
#endif    
};

static CODEC_FUNCTBL_T _rCodecFunTbl = 
{
	_MT5387_Init,				//pfnInit
	_MT5387_Mute,			//pfnMute 
	_MT5387_AdcFmtCfg,  		//pfnSetAdcOutFmt;
   	_MT5387_DacFmtCfg,      	//pfnSetDacInFmt;
   	NULL, 					//pfnSetDacGain;
   	NULL,					// pfnGetDacGain
   	_MT5387_SetAdcGain,		// pfnSetAdcGain
	NULL,					// pfnGetAdcGain   
   	_MT5387_HpVolCtl,			// pfnSetHpGain
   	NULL,					// pfnGetHpGain
   	_MT5387_ChlSel,			// pfnSetAdcMuxSel
   	_MT5387_WRITE,			// pfnRegWrite
   	_MT5387_READ			// pfnRegRead
};

static CODEC_T _rCodecMT5387 = 
{
	"MT5387",				// szName[32];
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
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _MT5387_WRITE(UINT8 u1Addr, UINT8 u1Data)
{
    UNUSED(u1Addr);
    UNUSED(u1Data);
#if 0
    UINT32 u4WriteCount = 0; 
    
    //u4WriteCount = SIF_Write(SIF_CLK_DIV, MT5387_DEV_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    	;
    }	
    else
    {
    	LOG(5, "Error MT5387 register setting\n");
    }
#endif    
}

UINT8 _MT5387_READ(UINT8 u1Addr) 
{
    UNUSED(u1Addr);
#if 0
    UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;

    //u4ReadCount = SIF_Read(SIF_CLK_DIV, MT5387_DEV_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "MT5387 READ FAIL\n");
    	return FALSE;
    }
#endif    
    return FALSE;
}


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _MT5387_Init(void)
{
    AUD_INTER_DAC_CFG_T* prInterDacUsage;
	#if !defined(CC_MT8223)
    UINT32 u4Reg4 = 0;    
	#endif
    Printf("!!!! _MT5387_Init !!!!\n");
    AUD_WRITE32(0x01C8, 0xFA00A000);    //DANIEL_20090717
    //AUD_WRITE32(0x01FC, 0x40000000);    //DANIEL_20090717
    
    //PWM related.
    _MT5387_Init_all(FS_48K, TRUE);

    //AD related.
    AADC_WRITE32(MT5387_REG_INT_SDADC0, 0x01020000);//0x5240
    AADC_WRITE32(MT5387_REG_INT_SDADC1, 0x20dd0000);//0x5244
    AADC_WRITE32(MT5387_REG_INT_SDADC2,0x001804c0); //0x5248


    UNUSED(DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32 *)(void *)&prInterDacUsage));
    if (prInterDacUsage != NULL)
    {
       _MT5387Internal_DacChlSel(AUD_DEC_MAIN,prInterDacUsage->eDac0ChlSel);
        _MT5387Internal_DacChlSel(AUD_DEC_AUX,prInterDacUsage->eDac1ChlSel);
       _MT5387Internal_DacChlSel(AUD_DEC_THIRD,prInterDacUsage->eDac2ChlSel);   
    }

    //----------------Speaker output setting-------------------------
    // power stage ->speaker output setting
    // vWritePWM(A_POE, 0x333333);
    // AADC_WRITE32(REG_PWMIP_BTLMODE_PG1, 0x40000000);
    // CKGEN_WRITE32(0x178,0xC40001C2);//enhance driving current of TP_VPLL pin 
    AADC_WRITE32(REG_PWMIP_BTLMODE_PG1, PWMBTLMODE);//turn on power stage INB and IND function(TP_VPLL pin)
    x_thread_delay(50);    

   #if !defined(CC_MT8223)
   u4Reg4 = IO32ReadFldAlign(CKGEN_PMUX4, FLD_PMUX4);
   u4Reg4 = (u4Reg4 & ~(0x0000007C)); //clear bit[6:4],  bit[3:2]
   #endif
    if (DRVCUST_InitGet(eFlagDDRQfp)) //mt5387 condition
    {
        //CKGEN_WRITE32(0x414,0x04);
#if !defined(CC_MT8223)
#ifdef CC_5387M2V1_AMTEK
        u4Reg4 |= (0x34);
        //vIO32WriteFldAlign(CKGEN_PMUX4,0x34,FLD_PMUX4);
#else
        u4Reg4 |= (0x04);
        //vIO32WriteFldAlign(CKGEN_PMUX4,0x04,FLD_PMUX4);
#endif
#endif
        
        ADAC_GpioCodecMute(TRUE); //power on speaker 
    }
    else
    {  
        #if !defined(CC_MT8223)
        u4Reg4 |= (0x34);    
		#endif
        //vIO32WriteFldAlign(CKGEN_PMUX4,0x34,FLD_PMUX4);//mt5388
        // CKGEN_WRITE32(0x414,0x34);//mt5388
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 1, FLD_RG_T2TTLOE4);  
        vIO32WriteFldAlign(CKGEN_VOPLLCFG2, 1, FLD_RG_T2TTLOE8);
    }
   #if !defined(CC_MT8223)
    vIO32WriteFldAlign(CKGEN_PMUX4,u4Reg4,FLD_PMUX4);
    #endif
    // ADAC_GpioCodecMute(FALSE); //power on speaker 
	//mark for power on pop noise , Amp wiil enable in setoutport function.//by xiaowen 090710
    //ADAC_GpioAmpEnable(TRUE);//xiaoyi for power on popo noise 100312
    _AD82581_Init();

    //-------------------------------------------------------
    AADC_WRITE32(MT5387_REG_INT_SDADC3,0x0);    

    //  CKGEN_WRITE32(0x324,0x30000); //CLK Gen to DAC APLL
    CKGEN_WRITE32(0x324,0x0); //CLK Gen to DAC APLL
    #if !defined(CC_MT8223)
    u4Reg4 = IO32ReadFldAlign(CKGEN_PADDRVCFG2, FLD_PADDRV2);
    u4Reg4 = (u4Reg4 & ~(0x3003)); //clear bit[0:1], bit[16:17]
    vIO32WriteFldAlign(CKGEN_PADDRVCFG2,u4Reg4, FLD_PADDRV2);
    #endif
    
    fgPWMInit = TRUE;
}

void _MT5387_Enable(BOOL fgEnalbe)
{
    if (fgEnalbe)
    {
        //_MT5387_WRITE(REG_MT5387_POW, 0); // Power on all
    }
    else
    {
        //_MT5387_WRITE(REG_MT5387_POW, 0xff); // Power on all
    }
}

void _MT5387_ADCDOWN(BOOL fgEnalbe)
{
    if (fgEnalbe)
    {
            AADC_WRITE32(MT5387_REG_INT_SDADC2, (AADC_READ32(MT5387_REG_INT_SDADC2) |MT5387_ADC_LEFT_PD));    // power down ADC_LEFT channel 
            AADC_WRITE32(MT5387_REG_INT_SDADC2, (AADC_READ32(MT5387_REG_INT_SDADC2) |MT5387_ADC_RIGHT_PD));    //power down ADC_Right channel    

    }
    else
    {
            AADC_CLR_BIT(MT5387_REG_INT_SDADC2, MT5387_ADC_LEFT_PD);    // power on ADC_LEFT channel 
            AADC_CLR_BIT(MT5387_REG_INT_SDADC2, MT5387_ADC_RIGHT_PD);    //power on ADC_Right channel    

    }
}

void _MT5387_MuteCLK(UINT8 u1DacId,BOOL fgMute)
{
    UNUSED(u1DacId);
    //DAC mute(can mute any clock pop noise, DC level from 3.3V down to 0V)
    if (fgMute) // Mute
    {  
    AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x2D000100);
    AADC_WRITE32(REG_PWMIP_MISC1, 0x00000fff);
    AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x2D00013f);
    vWritePWM(A_PGCTRL0, C_PGCTRL0); 
    }
    else // UnMute
    {
    vWritePWM(A_POE, 0x0);
    vWritePWM(A_PGCTRL0, C_PGCTRL0_PEN); 
    AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x2D00013f);
    vWritePWM(A_POE, 0x030303);
    AADC_WRITE32(REG_PWMIP_MISC1, 0x00003fff);
    AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x2D000100);
    AADC_WRITE32(REG_PWMIP_MISC1, 0x00000fff);
    }
}

void _MT5387_Mute(UINT8 u1DacId,BOOL fgMute)
{
    UNUSED(u1DacId);

    if (!fgPWMInit)
    {
        return;
    }

    //PGA mute
    if (fgMute) // Mute
    {  
    //turn on 0xf04
    vWritePWM(A_POE, 0x333333);
    }
    else // UnMute
    {
    //turn off 0xf04
    vWritePWM(A_POE, 0x030303);
    }
}

void _MT5387_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
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

void _MT5387_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
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

void _MT5387_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg;
    
    switch (u1Indx)
    {
        case 0:
        	u4Reg = MT5387_INT_ADC_IN0L0R;
        	break;
        case 1:
        	u4Reg = MT5387_INT_ADC_IN1L1R;
        	break;
        case 2:
        	u4Reg = MT5387_INT_ADC_IN2L2R;
        	break;
        case 3:
        	u4Reg = MT5387_INT_ADC_IN3L3R;
        	break;
        case 4:
        	u4Reg = MT5387_INT_ADC_IN4L4R;
        	break;	
        case 5:
        	u4Reg = MT5387_INT_ADC_IN5L5R;
        	break;
        case 6:
        	u4Reg = MT5387_INT_ADC_IN6L6R;
        	break;
        case 7:
        	u4Reg = MT5387_INT_ADC_IN7L7R;
        	break;
        default:
        	u4Reg = MT5387_INT_ADC_IN0L0R;
        	break;
    }
    	
    AADC_WRITE32(REG_INT_SDADC2, (AADC_READ32(REG_INT_SDADC2) & ~(0x1fU<<20)) |u4Reg);
}

void _MT5387_SetAdcGain(UINT8 u1DecId, INT16 i2Value)
{
	UNUSED(u1DecId);

	// Check Max and min
	if (i2Value >= MT5387_ADC_VOL_MAX)
	{
		i2Value = MT5387_ADC_VOL_MAX;
	}
	else if (i2Value <= MT5387_ADC_VOL_MIN)
	{
		i2Value = MT5387_ADC_VOL_MIN;
	}

	//u1RegValue = 0xcf +  i2Value;

	//_MT5387_WRITE(REG_MT5387_ADC_L_ATTEN, u1RegValue);
	//_MT5387_WRITE(REG_MT5387_ADC_R_ATTEN, u1RegValue);
}

void _MT5387_HpVolCtl(UINT8 u1HpId, INT16 i2Value)
{
	UNUSED(u1HpId);
    //_MT5387_WRITE(REG_MT5387_HP_L, i2Value);
    //_MT5387_WRITE(REG_MT5387_HP_R, i2Value);
    //_MT5387_WRITE(REG_MT5387_HP_M, i2Value);

    //_MT5387_WRITE(REG_MT5387_HP_UPDATE,0x4);
}

void _MT5387_DacGainCtrl(UINT8 u1dacId, INT32 i4Value)
{
	
	if(u1dacId == DAC0)
	{
		vWritePWM(A_PCADDR, 0x115);
	    vWritePWM(A_PCDATA, i4Value);
	    vWritePWM(A_PCADDR, 0x146);
	    vWritePWM(A_PCDATA, i4Value);
	}
	else if(u1dacId == DAC1)
	{
		vWritePWM(A_PCADDR, 0x147);
	    vWritePWM(A_PCDATA, i4Value);
	    vWritePWM(A_PCADDR, 0x148);
	    vWritePWM(A_PCDATA, i4Value);		
	}
	else if(u1dacId == DAC2)
	{
		vWritePWM(A_PCADDR, 0x149);
	    vWritePWM(A_PCDATA, i4Value);
	    vWritePWM(A_PCADDR, 0x14A);
	    vWritePWM(A_PCDATA, i4Value);
	}
	
}
void _MT5387_PLLReset(void)
{
	// Reset PLL power down
	//_MT5387_WRITE(REG_MT5387_POW2, 0x30);
    // Reset PLL power ON	
	//_MT5387_WRITE(REG_MT5387_POW2, 0x0); 
}

void _MT5387Internal_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl)
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
    else if (u1DacId == AUD_DEC_AUX)
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
    else
    {
    		switch (eChl)
        {
        	case AUD_CHL_L_R:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC2_SEL_MASK))) |DAC2_SEL_0_LR );
                break;
        	case AUD_CHL_LS_RS:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC2_SEL_MASK))) |DAC2_SEL_0_SLR );
                break;
        	case AUD_CHL_C_SW:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC2_SEL_MASK))) |DAC2_SEL_0_CLFE );
                break;
        	case AUD_CHL_DOWNMIX:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC2_SEL_MASK))) |DAC2_SEL_0_CH90 );
                break;
        	case AUD_CHL_BYPASS:
        		AUD_WRITE32(REG_IDACHCG,( AUD_READ32(REG_IDACHCG) &(~ (DAC2_SEL_MASK))) |DAC2_SEL_0_CH78 );
                break;
        	case AUD_CHL_AUX:
        		AUD_WRITE32(REG_IDACHCG, (AUD_READ32(REG_IDACHCG) &(~ (DAC2_SEL_MASK))) |DAC2_SEL_1_LR );
                break;
            	default:
                break;
        }
    }
}

CODEC_T* _CODEC_GetMT5387Tbl(void)
{
	return &_rCodecMT5387;
}

void _MT5387_PWM_SetSamplingRate(UINT8 u1ClkFmt)
{
    _MT5387_Init_all(u1ClkFmt, FALSE);
}

static void _MT5387_Init_all(UINT8 u1ClkFmt, BOOL fgInit)
{
    UINT32 i;
    UINT32 Yi;
    
    //AUD_INTER_DAC_CFG_T* prInterDacUsage;
    //CKGEN_WRITE32(0x270,0x1); //CLK Gen to DAC APLL
    if (fgInit)
    {
     vIO32WriteFldAlign(CKGEN_PWM_CKCFG,2,FLD_PWM_CK_SEL);

        //AADC_WRITE32(REG_PWMCFG0_PG1,0); 
        AADC_WRITE32(REG_PWMIP_PINMUX_PG1, 0xFFFFFF);
        AADC_WRITE32(REG_PWMIP_PINMUX_PG2, 0xFFFFFF);
        AADC_WRITE32(REG_PWMIP_MON_PG1, 0); // Debug used only

        AADC_WRITE32(REG_PWMIP_PWMPAD_CFG, 0);
        AADC_WRITE32(REG_PWMIP_BTLMODE_PG1, 0);
        AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0);
        AADC_WRITE32(REG_PWMIP_MISC1, 0);
#ifdef MT8223_PWM_SETTING    
    AADC_WRITE32(REG_PWMCFG0_PG1,0xfa829200); //   change  REG_PWMCFG0_PG1 from 0xfa029000  to  0xfa829200  fix L and R 7 degree phase difference
#else
    AADC_WRITE32(REG_PWMCFG0_PG1,0xfa02903f);
#endif

        AADC_WRITE32(REG_INDIRECT0_ADDR, A_P0PIN);
        AADC_WRITE32(REG_INDIRECT0_DATA, C_P0PIN0);

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
        
        vWritePWM(A_POE, P5EN|P4EN|P3EN|P2EN|P1EN|P0EN);

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
        
        vWritePWM(A_POE, P5EN|P4EN|P3EN|P2EN|P1EN|P0EN);
    }

////////////////////////////////////////////////////////////////
    //mute before turn of F01
    if (fgInit)
    { 
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_4X));//for 48 and 44
        AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x15000100);
        AADC_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_ON);
        AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x1500013f);
        x_thread_delay(70);
        vWritePWM(A_PGCTRL0, C_PGCTRL0); // D2audio 20080829
    }
    else
    {
        _MT5387_Mute(0, TRUE);
    }
//////////////////////////////////////////////////////////////////
    
    
    vWritePWM(A_PIC, DACCIE);  // Enable Interrupt for DACC

    // -+-+- Clear X-Ram -+-+-
    vWritePWM(A_PCADDR, 0x000); // X RAM
    //for(i = 0; i < 37*2; i++)
    for(i = 0; i < 0xdf; i++)
    {
        vWritePWM(A_PCADDR, i); //XRAM initial
        vWritePWM(A_PCDATA, 0x0);  // Clear X RAM as 0
    }

    vWritePWM(A_PSTAT, 0xff00);  // Enable Interrupt for DACC

    //RS232LogS("Yram error");
    vWritePWM(A_PCADDR, 0x100);
    for(i = 0; i < PWM_YRAM_TOP_INDX; i++)
    {
        Yi = 256 + i;
        vWritePWM(A_PCADDR, Yi);
        vWritePWM(A_PCDATA, YRAM[i]);
        //RS232LogB(0, BYTE2(YRAM[i]), BYTE1(YRAM[i]), BYTE0(YRAM[i]));
    }

    for(i = PWM_YRAM_TOP_INDX; i < PWM_YRAM_END_INDX; i++)
    {
        Yi = 256 + i;
        vWritePWM(A_PCADDR, Yi);
        vWritePWM(A_PCDATA, 0x0);
    }

    _MT5387_PWM_SamplingRate_Fsi(u1ClkFmt);
    //Change PWM output from 1Vrms to 1.1Vrms , For PWM SNR
    vWritePWM(A_PCADDR, 0x115);
    vWritePWM(A_PCDATA, 0xf0600);
    vWritePWM(A_PCADDR, 0x146);
    vWritePWM(A_PCDATA, 0xf0600);
    vWritePWM(A_PCADDR, 0x147);
    vWritePWM(A_PCDATA, 0x15e000);
    vWritePWM(A_PCADDR, 0x148);
    vWritePWM(A_PCDATA, 0x15e000);
    vWritePWM(A_PCADDR, 0x149);
    vWritePWM(A_PCDATA, 0xf0600);
    vWritePWM(A_PCADDR, 0x14A);
    vWritePWM(A_PCDATA, 0xf0600);
    //PWM Clock shift , For PWM SNR
    vWritePWM(A_P6PIN, C_P6PIN0 | 0x0A);
    vWritePWM(A_P10PIN, C_P10PIN0 |0x0A);

        
    // vWritePWM(A_PGCTRL0, C_PGCTRL0_PEN); Test power on/off pop noise
    //------------------mute function--------------------------
    //F04 GPIO 
    vWritePWM(A_POE, 0x0);
    //turn on 0xf01
    

//////////////////////////////////////////////////////////////////
    if (fgInit)
    {
        vWritePWM(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWM(A_PDATA, 0); // Clear MT5387/88 DAC Audio data as 0 ---
    
        //-------------------un-mute function------------------------
        AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x1500013f);
        //vWritePWM(A_POE, 0x030303);   //GPIO ?
        AADC_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF);
        x_thread_delay(70);    
        AADC_WRITE32(REG_PWMIP_MISC0_PG1, 0x15000100);
        AADC_WRITE32(REG_PWMIP_MISC1, 0x00000fff);
    }
    else
    {
        _MT5387_Mute(0, FALSE);
    }
//////////////////////////////////////////////////////////////////    
    
    //set I2S mode output and DAC output no phase shift and LR inverse 
    AADC_CLR_BIT(REG_EXTDAC_CFG0,INV_LRCK);
    //AADC_WRITE32(REG_EXTDAC_CFG0, AADC_READ32(REG_EXTDAC_CFG0)|AOFMT_I2S);
#ifndef MT8223_PWM_SETTING
    AADC_WRITE32(REG_PWMCFG0_PG1,0xfa02923f);//make I2S mode and itnernal DAC  LR output normal
    AADC_WRITE32(REG_PWMIP_PINMUX_PG1,0xff2301);//make DAC I2S mode and internal DAC LR output normal  
#endif    
}

static void _MT5387_PWM_SamplingRate_Fsi(UINT8 u1ClkFmt)
{
    static BOOL _fgPWMRsted = FALSE;

    UINT32 u4PGCCtrlData = 0;

    UNUSED(_fgPWMRsted);

    int i, Yi, FSI;

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

    AADC_WRITE32(REG_INDIRECT0_ADDR,A_PGCTRL1);
    u4PGCCtrlData = AADC_READ32(REG_INDIRECT0_DATA);
    u4PGCCtrlData = u4PGCCtrlData & ~(0x3C);
#if 0       
    //if ((FSI == 32000) && (FSPWM/FSI)/3 == 4)
    if ((FSI == 16000) ||(FSI == 32000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FFAC);
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_3X | INTPFR_4X));
    }
    //else if ((FSPWM/FSI)/2 == 2)
    else if ((FSI == 44100) || (FSI == 48000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF9c);
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_4X));
    }
    else if ((FSI == 64000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF9c);
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_576 | INTPBR_2X | INTPFR_4X));
    }
    else if ((FSI == 96000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF94);
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_2X));
    }
    else if ((FSI == 88200) || (FSI == 176400) || (FSI == 192000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF84);
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_1X | INTPFR_2X));
    }
    else if ((FSI == 22050) || (FSI == 24000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FFb4);
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_4X | INTPFR_4X));
    }
    else
    {
        vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_4X));//for 48 and 44
    }
#else
    //if ((FSI == 32000) && (FSPWM/FSI)/3 == 4)
    if ((FSI == 16000) ||(FSI == 32000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FFAC);
        //vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_3X | INTPFR_4X));
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_3X | INTPFR_4X);
    }
    //else if ((FSPWM/FSI)/2 == 2)
    else if ((FSI == 44100) || (FSI == 48000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF9c);
        //vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_4X));
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_1X | INTPFR_4X);
    }
    else if ((FSI == 64000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF9c);
        //vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_576 | INTPBR_2X | INTPFR_4X));
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_2X | INTPFR_4X);        
    }
    else if ((FSI == 96000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF94);
        //vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_2X));
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_2X | INTPFR_2X);        
    }
    else if ((FSI == 88200) || (FSI == 176400) || (FSI == 192000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FF84);
        //vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_1X | INTPFR_2X));
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_1X | INTPFR_2X);        
    }
    else if ((FSI == 22050) || (FSI == 24000))
    {
        //vWritePWM(A_PGCTRL1, 0x36FFb4);
        //vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_4X | INTPFR_4X));
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_4X | INTPFR_4X);        
    }
    else
    {
        //vWritePWM(A_PGCTRL1, (C_PGCTRL1 | NUMLEVELS_768 | INTPBR_2X | INTPFR_4X));//for 48 and 44    
        u4PGCCtrlData = u4PGCCtrlData | (INTPBR_2X | INTPFR_4X);

    }
    vWritePWM(A_PGCTRL1, u4PGCCtrlData);
    
#endif
    // Do not touch F01 when set sample rate.
    //vWritePWM(A_PGCTRL0, C_PGCTRL0); // D2audio 20080829

    vWritePWM(A_PIC, DACCIE);  // Enable Interrupt for DACC

    // -+-+- Clear X-Ram -+-+-
    vWritePWM(A_PCADDR, 0x000); // X RAM
    //for(i = 0; i < 37*2; i++)
    for(i = 0; i < 0xdf; i++)
    {
        vWritePWM(A_PCADDR, i);
        vWritePWM(A_PCDATA, 0x0);  // Clear X RAM as 0
    }
    // -+-+- Load Y-Ram -+-+-
    //if ((FSI == 32000) && (FSPWM/FSI)/3 == 4)
    if ((FSI == 16000) ||(FSI == 32000) || (FSI == 64000))  //16k, 32K, 64khz
    {
        vWritePWM(A_PCADDR, 0x100); // Y RAM
        //RS232LogS("Yram coeff32k update");
        for(i = 0; i < PWM_YRAM_TOP_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM_for_32[i]);
            //RS232LogB(0, BYTE2( YRAM_for_32[i]), BYTE1( YRAM_for_32[i]), BYTE0( YRAM_for_32[i]));
        }
        for(i = PWM_YRAM_TOP_INDX; i < PWM_YRAM_END_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, 0x0);
        }
    }
    //else if ((FSPWM/FSI)/2 == 4)  // FSI is 48k
    else if ((FSI == 22050) ||(FSI == 24000) || (FSI == 44100) || (FSI == 48000))
    {
        vWritePWM(A_PCADDR, 0x100);
        //RS232LogS("Yram coeff update");
        for(i = 0; i < PWM_YRAM_TOP_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM[i]);
            //RS232LogB(0, BYTE2(YRAM[i]), BYTE1(YRAM[i]), BYTE0(YRAM[i]));
        }
        for(i = PWM_YRAM_TOP_INDX; i < PWM_YRAM_END_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, 0x0);
        }
    }
    //else if ((FSPWM/FSI)/2 == 2)// for 96Khz
    else if ((FSI == 88200) ||(FSI == 96000) || (FSI == 176400) || (FSI == 192000))
    {
        vWritePWM(A_PCADDR, 0x100);
        //RS232LogS("Yram coeff update");
        for(i = 0; i < PWM_YRAM_TOP_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM_for_96[i]);
            //RS232LogB(0, BYTE2(YRAM_for_96[i]), BYTE1(YRAM_for_96[i]), BYTE0(YRAM_for_96[i]));
        }
        for(i = PWM_YRAM_TOP_INDX; i < PWM_YRAM_END_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, 0x0);
        }
    }
    else //Fs=44.1 should follow Fs=48
    {
        //RS232LogS("Yram error");
        vWritePWM(A_PCADDR, 0x100);
        for(i = 0; i < PWM_YRAM_TOP_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, YRAM[i]);
            //RS232LogB(0, BYTE2(YRAM[i]), BYTE1(YRAM[i]), BYTE0(YRAM[i]));
        }
        for(i = PWM_YRAM_TOP_INDX; i < PWM_YRAM_END_INDX; i++)
        {
            Yi = 256 + i;
            vWritePWM(A_PCADDR, Yi);
            vWritePWM(A_PCDATA, 0x0);
        }
    }
  
}


