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
 * $RCSfile: digiamp_others.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_apogee.c
 *  Brief of file adac_apogee.c.
 *  Details of file adac_apogee.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "digiamp_others.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_END

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

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _OTHERS_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _OTHERS_RegRead(UINT8 u1Addr);
static void _OTHERS_Init(void);
static void _OTHERS_Mute(BOOL fgEnable);
static void _OTHERS_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_OTHERS_Init,							//pfnInit
	_OTHERS_Mute,							//pfnMute 
	_OTHERS_DacFmtCfg,  			   //pfnSetDacInFmt;
	NULL,						 	     		   //pfnSetDacGain;
	_OTHERS_RegWrite, 					//pfnWrite;
	_OTHERS_RegRead					//pfnRead;
};

static AMP_T _rAmpOthers = 
{
	"AD82571A",				// szName[32];
	NULL,							// i2MaxGain
	NULL,						// i2MaxGain
	NULL,								// u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _OTHERS_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

	u4WriteCount = SIF_Write(SIF_CLK_DIV, OTHERS_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    }	
    else
    {
        LOG(5, "%c WRITE FAIL\n",_rAmpOthers.szName);
    }	
}

static UINT8 _OTHERS_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

    u4ReadCount = SIF_Read(SIF_CLK_DIV, OTHERS_ADDR, u1Addr, &u1Data, 1);

    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "%c READ FAIL\n",_rAmpOthers.szName);
    	return 0;
    }	
}

static void _OTHERS_Init(void)
{
	 AOUT_CFG_T*		prAudOutFmt;

    	// Make sure I2c is init
    	SIF_Init(); 

	//Power on GPIO
	ADAC_GpioAmpEnable(TRUE);
	x_thread_delay(10);

	// Unmute all
   	_OTHERS_RegWrite(OTHERS_CONF2, 0x00); 
    
    	// Format config, mark for use default value
	UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    	//_OTHERS_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
			
    	// Volume setup
	_OTHERS_RegWrite(REG_OTHERS_MASTER_VOL_CRL, OTHERS_MASTER_VOL_POS_12DB);

}

static void _OTHERS_Mute(BOOL fgEnable)
{
    if (fgEnable)
    {
        _OTHERS_RegWrite(REG_OTHERS_MASTER_VOL_CRL, OTHERS_MUTE_ALL_CHL);
    }
    else
    {
        _OTHERS_RegWrite(REG_OTHERS_MASTER_VOL_CRL, OTHERS_UNMUTE_ALL_CHL);
    }
}

static void _OTHERS_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);
    
    switch (eFormat)
    {
        case FORMAT_LJ:
        	_OTHERS_RegWrite(OTHERS_CONF0, _OTHERS_RegRead(OTHERS_CONF0)& ~OTHERS_CONF0_IF_MASK|0x1 <<5);
        	break;
        case FORMAT_I2S:
        	_OTHERS_RegWrite(OTHERS_CONF0, _OTHERS_RegRead(OTHERS_CONF0)& ~OTHERS_CONF0_IF_MASK|0x0 <<5);
        	break;
        case FORMAT_RJ:
        	_OTHERS_RegWrite(OTHERS_CONF0, _OTHERS_RegRead(OTHERS_CONF0)& ~OTHERS_CONF0_IF_MASK|0x5 <<5);
        	break;
        default:
        	_OTHERS_RegWrite(OTHERS_CONF0, _OTHERS_RegRead(OTHERS_CONF0)& ~OTHERS_CONF0_IF_MASK|0x0 <<5);
        	break;
    }
}

AMP_T* _AMP_GetAmpOthersFunTbl(void)
{
	return &_rAmpOthers;
}

/*
void test()
{
//powr on sequence 1
	vHWADDACRegWriteByte(STA232W_CONFF, 0x5C);
	vHWADDACRegWriteByte(STA232W_MASMUTE, 0x01);
//initializing	
	vHWADDACRegWriteByte(STA232W_CONFA, 0x63); //256fps
	vHWADDACRegWriteByte(STA232W_CONFD, 0x10);
	//vHWADDACRegWrite(STA232W_CONFF, 0xDC); --> overlap, disable
	vHWADDACRegWriteByte(STA232W_MASTER,0x00);
	vHWADDACRegWriteByte(STA232W_MASVOL1,0x44);
	vHWADDACRegWriteByte(STA232W_MASVOL2,0x44);
	vHWADDACRegWriteByte(STA232W_AUTOMODE,0x80);
	vHWADDACRegWriteByte(STA232W_CH1CONFIG,0x00);
	vHWADDACRegWriteByte(STA232W_CH2CONFIG,0x40);
	vHWADDACRegWriteByte(STA232W_LIMITER,0x7F);
//POWER ON SEQUENCE 2
	vHWADDACRegWriteByte(STA232W_CONFF,0xDC);
	vHWADDACRegWriteByte(STA232W_MASMUTE, 0x00);
}
*/


