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

/** @file codec_WM8776.c
 *  Brief of file codec_WM8776.c.
 *  Details of file codec_WM8776.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "drvcust_if.h"
#include "codec_mt8291.h"
#include "aud_debug.h"
#include "x_pinmux.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define MT8291_ADC_VOL_MAX  (42)
#define MT8291_ADC_VOL_MIN  (-42)


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables a
//---------------------------------------------------------------------------

static CODEC_FUNCTBL_T _rCodecFunTbl = 
{
	_MT8291_Init,				//pfnInit
	_MT8291_Mute,			//pfnMute 
	_MT8291_AdcFmtCfg,  		//pfnSetAdcOutFmt;
   	_MT8291_DacFmtCfg,      	//pfnSetDacInFmt;
   	NULL, 					//pfnSetDacGain;
   	NULL,					// pfnGetDacGain
   	_MT8291_SetAdcGain,		// pfnSetAdcGain
	NULL,					// pfnGetAdcGain   
   	_MT8291_HpVolCtl,			// pfnSetHpGain
   	NULL,					// pfnGetHpGain
   	_MT8291_ChlSel,			// pfnSetAdcMuxSel
   	_MT8291_WRITE,			// pfnRegWrite
   	_MT8291_READ			// pfnRegRead
};

static CODEC_T _rCodecMT8291 = 
{
	"MT8291",				// szName[32];
	0,							// i2MaxHpGain
	-72,							// i2MinHpGain
	0,							// i2MaxDacGain
	0,							// i2MinDacGain
	7,							// u1AdcMuxNum
	2,							// u1DacNum
	1,							// u1HpNum
	&_rCodecFunTbl			// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _MT8291_WRITE(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 
    
    u4WriteCount = SIF_Write(SIF_CLK_DIV, MT8291_DEV_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    	;
    }	
    else
    {
    	LOG(5, "Error MT8291 register setting\n");
    }	
}

UINT8 _MT8291_READ(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;

    u4ReadCount = SIF_Read(SIF_CLK_DIV, MT8291_DEV_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "MT8291 READ FAIL\n");
    	return FALSE;
    }	
}



//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _MT8291_Init(void)
{
	AOUT_CFG_T*		prAudOutFmt;
	AOUT_CFG_T*		prAudInFmt;
	//INT32 				i4GpioSwitchHigh;
	//INT32 				i4GpioSwitchLow;
	//UINT32 				u4SpkPath;

	UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *) &prAudOutFmt));
	UNUSED(DRVCUST_OptQuery(eAudioInFmtTable, (UINT32 *)(void *) &prAudInFmt));

	//i4GpioSwitchHigh=1;
	//i4GpioSwitchLow=0;

  // PGA mute
  _MT8291_WRITE(REG_MT8291_DAC_OUPUT_PGA_CTRL, 0xf3); 

  // reset PLL power down
  _MT8291_WRITE(REG_MT8291_POW2, 0x30); 

  // reset PLL power ON
  _MT8291_WRITE(REG_MT8291_POW2, 0x0);  
   
  // DAC software reset
  _MT8291_WRITE(REG_MT8291_DAC_SWRESET, 0x00);  

  // DAC software reset
  _MT8291_WRITE(REG_MT8291_DAC_SWRESET, 0xff);  

  // ADC interface control
  _MT8291_AdcFmtCfg(prAudInFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk)); // ADC interface control

  // MCLK setup
  _MT8291_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk)); // DAC interface control

  // Adjust offset of dac gain, default value is 0x30 --> 2.4 Vrms, 
  //                                                            0x2a --> 2 Vrms
  _MT8291_WRITE(REG_MT8291_DAC_GAIN_OFFSET, 0x2A);	
  
  _MT8291_HpVolCtl(0x0,0x1F);//Enhance HP output to 0dB 
 
  // PGA unmute
  _MT8291_WRITE(REG_MT8291_DAC_OUPUT_PGA_CTRL, 0x0);  

}

void _MT8291_Enable(BOOL fgEnalbe)
{
    if (fgEnalbe)
    {
        _MT8291_WRITE(REG_MT8291_POW, 0); // Power on all
    }
    else
    {
        _MT8291_WRITE(REG_MT8291_POW, 0xff); // Power on all
    }
}

void _MT8291_Mute(UINT8 u1DacId,BOOL fgMute)
{
    UNUSED(u1DacId);

    //PGA mute
    if (fgMute) // Mute
    {  
        _MT8291_WRITE(REG_MT8291_DAC_OUPUT_PGA_CTRL, MT8291_PGA_MUTE);
        x_thread_delay(10); 
    }
    else // UnMute
    {
        _MT8291_WRITE(REG_MT8291_DAC_OUPUT_PGA_CTRL, MT8291_PGA_UNMUTE);
        x_thread_delay(10); 
    }
}

void _MT8291_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
	UINT8 u1Data;
    switch (eFormat)
    {
        case FORMAT_LJ:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_INF_CTRL, ((u1Data & ~MT8291_ADCFMTCFG_MASK)|MT8291_ADCFMT_LJ|MT8291_ADC_WL_24 ));
        	break;
        case FORMAT_I2S:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_INF_CTRL, ((u1Data & ~MT8291_ADCFMTCFG_MASK)|MT8291_ADCFMT_I2S|MT8291_ADC_WL_24));
        	break;
        case FORMAT_RJ:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_INF_CTRL, ((u1Data & ~MT8291_ADCFMTCFG_MASK)|MT8291_ADCFMT_RJ|MT8291_ADC_WL_24 ));
        	break;
        default:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_INF_CTRL, ((u1Data & ~MT8291_ADCFMTCFG_MASK)|MT8291_ADCFMT_I2S|MT8291_ADC_WL_24));
        	break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_256FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		 _MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
        	break;
        case MCLK_256FS:
     	   	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_256FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
        	break;
        case MCLK_384FS:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_384FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_384FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_384FS|MT8291_DAC_S_MODE));
        	break;

        case MCLK_512FS:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_512FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_512FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_512FS|MT8291_DAC_S_MODE));
            break;
        default:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_256FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
        	break;
    }
    
}

void _MT8291_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
	UINT8  u1Data;
    switch (eFormat)
    {
        case FORMAT_LJ:
        	u1Data=_MT8291_READ(REG_MT8291_DAC0_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC0_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_LJ|MT8291_DAC_WL_24 ));
        	u1Data=_MT8291_READ(REG_MT8291_DAC1_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC1_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_LJ|MT8291_DAC_WL_24 ));

        	break;
        case FORMAT_I2S:
        	u1Data=_MT8291_READ(REG_MT8291_DAC0_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC0_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_I2S|MT8291_DAC_WL_24));
        	u1Data=_MT8291_READ(REG_MT8291_DAC1_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC1_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_I2S|MT8291_DAC_WL_24));
        	break;
        case FORMAT_RJ:
        	u1Data=_MT8291_READ(REG_MT8291_DAC0_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC0_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_RJ|MT8291_DAC_WL_24 ));
        	u1Data=_MT8291_READ(REG_MT8291_DAC1_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC1_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_RJ|MT8291_DAC_WL_24 ));
        	break;
        default:
        	u1Data=_MT8291_READ(REG_MT8291_DAC0_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC0_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_I2S|MT8291_DAC_WL_24));
        	u1Data=_MT8291_READ(REG_MT8291_DAC1_INF_CTRL);
        	_MT8291_WRITE(REG_MT8291_DAC1_INF_CTRL, ((u1Data & ~MT8291_DACFMTCFG_MASK)|MT8291_DACFMT_I2S|MT8291_DAC_WL_24));
        	break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_256FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		 _MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL );
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
        	break;
        case MCLK_256FS:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_256FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		 _MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
        	break;
        case MCLK_384FS:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_384FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		 _MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_384FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_384FS|MT8291_DAC_S_MODE));
        	break;

        case MCLK_512FS:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_512FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		 _MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_512FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_512FS|MT8291_DAC_S_MODE));
            break;
        default:
        	u1Data=_MT8291_READ(REG_MT8291_ADC_I2S_CTRL);
        	_MT8291_WRITE(REG_MT8291_ADC_I2S_CTRL, ((u1Data & ~MT8291_ADC_I2S_MASK)|
        						MT8291_ADC_RATE_256FS|MT8291_ADC_OSR_128|MT8291_ADC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC0_I2S_CTRL);
		 _MT8291_WRITE( REG_MT8291_DAC0_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
		 u1Data=_MT8291_READ(REG_MT8291_DAC1_I2S_CTRL);
		_MT8291_WRITE( REG_MT8291_DAC1_I2S_CTRL,((u1Data & ~MT8291_DAC_I2S_MASK)|MT8291_DAC_RATE_256FS|MT8291_DAC_S_MODE));
        	break;
    }
    
}

void _MT8291_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg;
    
    switch (u1Indx)
    {
        case 0:
        	u4Reg = MT8291_ADC_IN1L1R;
        	break;
        case 1:
        	u4Reg = MT8291_ADC_IN2L2R;
        	break;
        case 2:
        	u4Reg = MT8291_ADC_IN3L3R;
        	break;
        case 3:
        	u4Reg = MT8291_ADC_IN4L4R;
        	break;
        case 4:
        	u4Reg = MT8291_ADC_IN5L5R;
        	break;	
        case 5:
        	u4Reg = MT8291_ADC_IN6L6R;
        	break;
        case 6:
        	u4Reg = MT8291_ADC_IN7L7R;
        	break;
        default:
        	u4Reg = MT8291_ADC_IN1L1R;
        	LOG(1, "Error _ADC_ChlSel\n");
        	break;
    }
    	
    _MT8291_WRITE(REG_MT8291_INPUT_MUX, u4Reg);

}

void _MT8291_SetAdcGain(UINT8 u1DecId, INT16 i2Value)
{
	UINT8 u1RegValue = 0xcf;
	UNUSED(u1DecId);

	// Check Max and min
	if (i2Value >= MT8291_ADC_VOL_MAX)
	{
		i2Value = MT8291_ADC_VOL_MAX;
	}
	else if (i2Value <= MT8291_ADC_VOL_MIN)
	{
		i2Value = MT8291_ADC_VOL_MIN;
	}

	u1RegValue = 0xcf +  i2Value;

	_MT8291_WRITE(REG_MT8291_ADC_L_ATTEN, u1RegValue);
	_MT8291_WRITE(REG_MT8291_ADC_R_ATTEN, u1RegValue);
}

void _MT8291_HpVolCtl(UINT8 u1HpId, INT16 i2Value)
{
	UNUSED(u1HpId);
    	_MT8291_WRITE(REG_MT8291_HP_L, i2Value);
    	_MT8291_WRITE(REG_MT8291_HP_R, i2Value);
    	_MT8291_WRITE(REG_MT8291_HP_M, i2Value);

       _MT8291_WRITE(REG_MT8291_HP_UPDATE,0x4);
}

void _MT8291_PLLReset(void)
{
	// Reset PLL power down
	_MT8291_WRITE(REG_MT8291_POW2, 0x30);
    // Reset PLL power ON	
	_MT8291_WRITE(REG_MT8291_POW2, 0x0); 
}

CODEC_T* _CODEC_GetMT8291Tbl(void)
{
	return &_rCodecMT8291;
}



