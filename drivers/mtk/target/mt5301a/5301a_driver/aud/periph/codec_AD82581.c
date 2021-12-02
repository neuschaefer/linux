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
 * $RCSfile: codec_mt8292.c,v $
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
#include "codec_mt8292.h"
#include "aud_debug.h"
#include "x_pinmux.h"
#include "codec_AD82581.h"
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
static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_AD82581_Init,      // pfnInit
	_AD82581_Mute,      // pfnMute 
	NULL,               // pfnSetDacInFmt;
   	NULL,               // pfnSetDacGain;
   	_AD82581_WRITE,		// pfnRegWrite
   	_AD82581_READ		// pfnRegRead
};

static AMP_T _rAmpAD82581 = 
{
	"AD82581",				// szName[32];
	0,						// i2MaxGain
	0,						// i2MaxGain
	0,						// u1DacNum
	&_rAmpFunTbl		    // rCodecFuncTbl
};

#if 0
static CODEC_FUNCTBL_T _rCodecFunTbl = 
{
	_MT8292_Init,				//pfnInit
	_MT8292_Mute,			//pfnMute 
	NULL,  		//pfnSetAdcOutFmt;
   	NULL,      	//pfnSetDacInFmt;
   	NULL, 		//pfnSetDacGain;
   	NULL,					// pfnGetDacGain
   	NULL,		// pfnSetAdcGain
	NULL,					// pfnGetAdcGain   
   	NULL,			// pfnSetHpGain
   	NULL,					// pfnGetHpGain
   	_MT8292_ChlSel,			// pfnSetAdcMuxSel
   	_MT8292_WRITE,			// pfnRegWrite
   	_MT8292_READ			// pfnRegRead
};

static CODEC_T _rCodecMT8292 = 
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

#endif 
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _AD82581_WRITE(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 
    
    u4WriteCount = SIF_Write(SIF_CLK_DIV, AD82581_DEV_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    	;
    }	
    else
    {
    	LOG(5, "Error AD82581 register setting\n");
    }	
}

UINT8 _AD82581_READ(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;

    u4ReadCount = SIF_Read(SIF_CLK_DIV, AD82581_DEV_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "AD82581 READ FAIL\n");
    	return FALSE;
    }	
}



#if 0
void _MT8292_Mute(UINT8 u1DacId, BOOL fgMute)
{
    UNUSED(u1DacId);
    
    //PGA mute
    if (fgMute) // Mute
    {  
        _MT8292_WRITE(REG_OUTPUT_PGA_MUTE, 0xff); 
    }
    else // UnMute
    {
        _MT8292_WRITE(REG_OUTPUT_PGA_MUTE, 0x00); 
        _MT8292_WRITE(REG_MT8292_SPEED_MUTE, 0x00); 
        _MT8292_WRITE(REG_MT8292_RAMPMUTE, 0x14); 
        _MT8292_WRITE(REG_MT8292_MUX_MUTE, 0x00); 
    }
}

void _MT8292_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg;
    
    switch (u1Indx)
    {
        case 0:
        	u4Reg = MT8292_ADC_IN1L1R;
        	break;
        case 1:
        	u4Reg = MT8292_ADC_IN2L2R;
        	break;
        case 2:
        	u4Reg = MT8292_ADC_IN3L3R;
        	break;
        case 3:
        	u4Reg = MT8292_ADC_IN4L4R;
        	break;
        case 4:
        	u4Reg = MT8292_ADC_IN5L5R;
        	break;	
        case 5:
        	u4Reg = MT8292_ADC_IN6L6R;
        	break;
        case 6:
        	u4Reg = MT8292_ADC_IN7L7R;
        	break;
        case 7:
        	u4Reg = MT8292_ADC_IN8L8R;
        	break;
        default:
        	u4Reg = MT8292_ADC_CLOSE;
        	LOG(3, "Close all input source\n");
        	break;
    	}
    _MT8292_WRITE(REG_MT8292_INPUT_MUX, u4Reg);  

}

void _MT8292_SetPGAGain(UINT8 u1DacId, UINT8 u1Data)
{
    if (u1DacId == 0)
    {
        _MT8292_WRITE(REG_MT8292_PGA0L_GAIN, u1Data); 
        _MT8292_WRITE(REG_MT8292_PGA0R_GAIN, u1Data); 
    }
    else if (u1DacId == 1)
    {
        _MT8292_WRITE(REG_MT8292_PGA1L_GAIN, u1Data); 
        _MT8292_WRITE(REG_MT8292_PGA1R_GAIN, u1Data);         
    }
    else
    {
        _MT8292_WRITE(REG_MT8292_PGA2L_GAIN, u1Data); 
        _MT8292_WRITE(REG_MT8292_PGA2R_GAIN, u1Data);     
    }
}

void _MT8292_SetHpGain(UINT8 u1Data)
{
    _MT8292_WRITE(REG_MT8292_HP_GAIN, u1Data); 
}

CODEC_T* _CODEC_GetMT8292Tbl(void)
{
	return &_rCodecMT8292;
}

#endif
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
void _AD82581_Init(void)
{    
    // Mute Speed
    _AD82581_WRITE(REG_AD82581_02, 0x0);
    
    _AD82581_WRITE(REG_AD82581_03, MASTER_VOLUME_n_25dB);     
}

void _AD82581_Mute(BOOL fgMute)
{  
    if (fgMute) // Mute
    {  
        _AD82581_WRITE(REG_AD82581_02, MUTE); 
    }
    else // UnMute
    {
        _AD82581_WRITE(REG_AD82581_02, UNMUTE); 
    }
}

AMP_T* _AMP_GetDigiAmpFunTbl(void)
{
	return &_rAmpAD82581;
}


