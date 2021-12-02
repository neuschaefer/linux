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
 * $RCSfile: codec_wm8776.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_WM8776.c
 *  Brief of file codec_wm8776.c.
 *  Details of file codec_wm8776.c (optional).
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
#include "codec_wm8776.h"
#include "aud_debug.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// In steps of 0.5 dB
#define WM8776_HP_VOL_MAX  12
#define WM8776_HP_VOL_MIN   (-242)

#define WM8776_ADC_VOL_MAX  (48)
#define WM8776_ADC_VOL_MIN  (-42)

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

static void _WM8776_WRITE16(UINT8 u1Addr, UINT16 u2Data);
static void _WM8776_Init(void);
//static void _WM8776_Enable(BOOL fgEnalbe);
static void _WM8776_Mute(UINT8 u1DacId,BOOL fgMute);
static void _WM8776_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
static void _WM8776_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
static void _WM8776_ChlSel(UINT8 u1Indx);
static void _WM8776_SetAdcGain(UINT8 u1DecId, INT16 i2Value);
static void _WM8776_HpVolCtl(UINT8 u1HpId, INT16 i2Value);
static void _WM8776_RegWrite(UINT8 u1Addr, UINT8 u1Data);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

#if 0
{
    CODEC_INIT                         pfnInit;
    CODEC_MUTE                       pfnMute;
    CODEC_SET_DAC_IN_FMT     pfnSetDacInFmt;
    CODEC_SET_ADC_OUT_FMT  pfnSetAdcOutFmt;
    CODEC_SET_DAC_GAIN        pfnSetDacGain;
    CODEC_GET_DAC_GAIN        pfnGetDacGain;
    CODEC_SET_ADC_GAIN        pfnSetDacGain;
    CODEC_GET_ADC_GAIN        pfnGetDacGain;
    CODEC_SET_HP_GAIN           pfnSetHpGain;
    CODEC_GET_HP_GAIN           pfnGetHpGain;
    CODEC_SET_ADC_MUX_SEL  pfnSetAdcMuxSel;
    CODEC_REG_WRITE              pfnRegWrite;
    CODEC_REG_READ				  pfnRegRead;
}
#endif

static CODEC_FUNCTBL_T _rCodecFunTbl = 
{
	_WM8776_Init,							//pfnInit
	_WM8776_Mute,							//pfnMute 
	_WM8776_DacFmtCfg,      		   //pfnSetDacInFmt;
	_WM8776_AdcFmtCfg,  			   //pfnSetAdcOutFmt;
   NULL, 									        //pfnSetDacGain;
   NULL,											// pfnGetDacGain
	_WM8776_SetAdcGain,				// pfnSetAdcGain
	NULL,											// pfnGetAdcGain   
   _WM8776_HpVolCtl,					// pfnSetHpGain
   NULL,											// pfnGetHpGain
   _WM8776_ChlSel,						// pfnSetAdcMuxSel
   _WM8776_RegWrite,					// pfnRegWrite
   NULL 											// pfnRegRead
};

static CODEC_T _rCodecWM8776 = 
{
	"WM8776",				// szName[32];
	12,							// i2MaxHpGain
	-242,						// i2MinHpGain
	0,								// i2MaxDacGain
	0,								// i2MinDacGain
	5,								// u1AdcMuxNum
	1,								// u1DacNum
	1,								// u1HpNum
	&_rCodecFunTbl			// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _WM8776_WRITE16(UINT8 u1Addr, UINT16 u2Data)
{
    UINT32 u4WriteCount = 0; 
    UINT8  u1DevAddr;
    UINT8  u1Data;

    /* 7-bits reg + 9-bits data */
    u1DevAddr =  (UINT8)((u1Addr << 1) + (((u2Data & 0x100) == 0x100) ? 1 : 0));
    u1Data = (UINT8)(u2Data & 0xff);
    
    u4WriteCount = SIF_Write(SIF_CLK_DIV, WM8776_DEV_ADDR, u1DevAddr, &u1Data, 1);

    if (u4WriteCount > 0)
    {
    	;
    }	
    else
    {
    	//LOG(1, "Error WM8776 register setting\n");
    }	
}

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _WM8776_Init(void)
{
	AOUT_CFG_T*		prAudOutFmt;
	AOUT_CFG_T*		prAudInFmt;

	UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *) &prAudOutFmt));
	UNUSED(DRVCUST_OptQuery(eAudioInFmtTable, (UINT32 *)(void *) &prAudInFmt));
	
  // Writing any value to register 0x17 will cause a register reset,
  // resetting all register bits to their default values.
  _WM8776_WRITE16(WM8776_REG17, 1);

  // ADC interface control
  //_WM8776_WRITE16(WM8776_REG0B, (D_ADDA_I2S|D_ADDA_WL_24)); 
  _WM8776_AdcFmtCfg(prAudInFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk)); // DAC interface control

  // MCLK setup
  _WM8776_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk)); // DAC interface control

  _WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_256FS | D_ADC_OSR_128 |
        		                 D_DAC_RATE_256FS | D_DAC_SLAVE_MODE |
        		                 D_ADC_SLAVE_MODE));

  //_WM8776_HpVolCtl(0, 0x7F);//Enhance HP output to 6dB 

  
  _WM8776_WRITE16(WM8776_REG0D, 0); // Power on all

}

/*
static void _WM8776_Enable(BOOL fgEnalbe)
{
    if (fgEnalbe)
    {
        _WM8776_WRITE16(WM8776_REG0D, 0); // Power on all
    }
    else
    {
        _WM8776_WRITE16(WM8776_REG0D, 0xff); // Power on all
    }
}
*/

static void _WM8776_Mute(UINT8 u1DacId,BOOL fgMute)
{
    UNUSED(u1DacId);

    if (fgMute) // Mute
    {
        _WM8776_WRITE16(WM8776_REG07, D_DAC_PL_M_M);
    }
    else // UnMute
    {
        _WM8776_WRITE16(WM8776_REG07, D_DAC_PL_L_R);
    }
}

static void _WM8776_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    switch (eFormat)
    {
        case FORMAT_LJ:
        	_WM8776_WRITE16(WM8776_REG0B, (D_ADDA_L_JUST|D_ADDA_WL_24 ));
        	break;
        case FORMAT_I2S:
        	_WM8776_WRITE16(WM8776_REG0B, (D_ADDA_I2S|D_ADDA_WL_24 ));
        	break;
        case FORMAT_RJ:
        	_WM8776_WRITE16(WM8776_REG0B, (D_ADDA_R_JUST|D_ADDA_WL_24 ));
        	break;
        default:
        	_WM8776_WRITE16(WM8776_REG0B, (D_ADDA_I2S|D_ADDA_WL_24 ));
        	break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_256FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_256FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;
        case MCLK_256FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_256FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_256FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;
        case MCLK_384FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_384FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_384FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;

        case MCLK_512FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_512FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_512FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
            break;
        default:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_256FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_256FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;
    }
    
}

static void _WM8776_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    switch (eFormat)
    {
        case FORMAT_LJ:
        	_WM8776_WRITE16(WM8776_REG0A, (D_ADDA_L_JUST|D_ADDA_WL_24 ));
        	break;
        case FORMAT_I2S:
        	_WM8776_WRITE16(WM8776_REG0A, (D_ADDA_I2S|D_ADDA_WL_24 ));
        	break;
        case FORMAT_RJ:
        	_WM8776_WRITE16(WM8776_REG0A, (D_ADDA_R_JUST|D_ADDA_WL_24 ));
        	break;
        default:
        	_WM8776_WRITE16(WM8776_REG0A, (D_ADDA_I2S|D_ADDA_WL_24 ));
        	break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_256FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_256FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;
        case MCLK_256FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_256FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_256FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;
        case MCLK_384FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_384FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_384FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;

        case MCLK_512FS:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_512FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_512FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
            break;
        default:
        	_WM8776_WRITE16(WM8776_REG0C, (D_ADC_RATE_256FS|D_ADC_OSR_128|
        		                           D_DAC_RATE_256FS|D_DAC_SLAVE_MODE|
        		                           D_ADC_MASTER_MODE));
        	break;
    }
    
}

static void _WM8776_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg;
    
    switch (u1Indx)
    {
        case 0:
        	u4Reg = D_ADC_IN1L1R;
        	break;
        case 1:
        	u4Reg = D_ADC_IN2L2R;
        	break;
        case 2:
        	u4Reg = D_ADC_IN3L3R;
        	break;
        case 3:
        	u4Reg = D_ADC_IN4L4R;
        	break;
        case 4:
        	u4Reg = D_ADC_IN5L5R;
        	break;	
        default:
        	u4Reg = D_ADC_IN1L1R;
        	//LOG(1, "Error _ADC_ChlSel\n");
        	break;
    }
    	
    _WM8776_WRITE16(WM8776_REG15, u4Reg);

}

static void _WM8776_SetAdcGain(UINT8 u1DecId, INT16 i2Value)
{
	UINT8 u1RegValue = 0xcf;
	UNUSED(u1DecId);

	// Check Max and min
	if (i2Value >= WM8776_ADC_VOL_MAX)
	{
		i2Value = WM8776_ADC_VOL_MAX;
	}
	else if (i2Value <= WM8776_ADC_VOL_MIN)
	{
		i2Value = WM8776_ADC_VOL_MIN;
	}

	u1RegValue = 0xcf +  i2Value;

	_WM8776_WRITE16(WM8776_REG0E, ((UINT16)u1RegValue | 0x100));
	_WM8776_WRITE16(WM8776_REG0F, ((UINT16)u1RegValue | 0x100));
}

static void _WM8776_HpVolCtl(UINT8 u1HpId, INT16 i2Value)
{
	UINT8 u1RegValue = 0x79;
      UNUSED(u1HpId);
	// Check Max and min
	if (i2Value >= WM8776_HP_VOL_MAX)
	{
		i2Value = WM8776_HP_VOL_MAX;
	}
	else if (i2Value <= WM8776_HP_VOL_MIN)
	{
		i2Value = WM8776_HP_VOL_MIN;
	}
	
	if (i2Value == 0)
	{
		u1RegValue = 0x79;
	}
	else if ((i2Value == 1) || (i2Value == 2))
	{
		u1RegValue = 0x7a;
	}
	else if ((i2Value == 3) || (i2Value == 4))
	{
		u1RegValue = 0x7b;
	}
	else if ((i2Value == 5) || (i2Value == 6))
	{
		u1RegValue = 0x7c;
	}
	else if ((i2Value == 7) || (i2Value == 8))
	{
		u1RegValue = 0x7d;
	}
	else if ((i2Value == 9) || (i2Value == 10))
	{
		u1RegValue = 0x7e;
	}
	else if ((i2Value == 11) || (i2Value >= 12))
	{
		u1RegValue = 0x7f;
	}
	else if (i2Value < 0)
	{
		u1RegValue = 0x79 +  (i2Value/2);
	}
		
    _WM8776_WRITE16(WM8776_REG00, ((UINT16)u1RegValue | 0x100));
    _WM8776_WRITE16(WM8776_REG01, ((UINT16)u1RegValue | 0x100));
    _WM8776_WRITE16(WM8776_REG02, ((UINT16)u1RegValue | 0x100));
}

static void _WM8776_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
	_WM8776_WRITE16(u1Addr, (UINT16)u1Data | 0x100);
}

CODEC_T* _CODEC_GetWM8776Tbl(void)
{
	return &_rCodecWM8776;
}


