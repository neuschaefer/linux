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
 * $Date: 2012/06/28 $
 * $RCSfile: codec_mt8292.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_WM8776.c
 *  Brief of file codec_WM8776.c.
 *  Details of file codec_WM8776.c (optional).
 */

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
static void _AD82581_WRITE(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _AD82581_READ(UINT8 u1Addr);
static void _AD82581_Init(void);
static void _AD82581_Mute(BOOL fgEnable);
static void _AD82581_DisableEffect(void);

//---------------------------------------------------------------------------
// Static variables 
//---------------------------------------------------------------------------

static AMP_DRV_CB_T _rAmp82581DrvCB = 
{
    _AD82581_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpFunTbl =
{
	_AD82581_Init,          // pfnInit
	_AD82581_Mute,                   // pfnMute 
	NULL,                   // pfnSetDacInFmt;
	NULL,                   // pfnSetDacGain;
	_AD82581_WRITE,         // pfnWrite;
	_AD82581_READ,           // pfnRead;
	&_rAmp82581DrvCB                          // pfnDrvCB;
};

static AMP_T _rAmp82581 = 
{
	"AD82581",          // szName[32];
	256,                // i2MaxGain
	-256,               // i2MaxGain
	2,                  // u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _AD82581_WRITE(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 
    
    //u4WriteCount = SIF_Write(SIF_CLK_DIV, AD82581_DEV_ADDR, u1Addr, &u1Data, 1);
    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, AD82581_DEV_ADDR, 1 ,u1Addr, &u1Data, 1);

    if (u4WriteCount <= 0)
    {
    	LOG(0, "Error AD82581 register setting\n");
    }	
}

static UINT8 _AD82581_READ(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data=0;

    //u4ReadCount = SIF_Read(SIF_CLK_DIV, AD82581_DEV_ADDR, u1Addr, &u1Data, 1);
    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, AD82581_DEV_ADDR, 1 , u1Addr, &u1Data, 1);    

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
static void _AD82581_Init(void)
{
    // Make sure I2c is init
    SIF_Init(); 

    //release A_MUTE and AMP_Enable to let AMP_PD# charge up
    //20100504 [DTV00080261] move to loader
    //A_MUTE pin   
    //ADAC_GpioCodecMute(FALSE);
    //AMP mute GPIO
    //ADAC_GpioAmpEnable(TRUE);
    //AMP_PD# pin charge up time needed
    //x_thread_delay(500);

    _AD82581_WRITE(0x2, 0x0);
    LOG(0, "AD82581_Init finish\n");
}
static void _AD82581_Mute(BOOL fgEnable)
{
   
    if (fgEnable)
    {
        _AD82581_WRITE(REG_AD82581_02,MUTE);
		LOG(8,"_AD82581_Mute.\n");
    }
    else
    {
        _AD82581_WRITE(REG_AD82581_02,UNMUTE);
		LOG(8,"_AD82581_UN-Mute.\n");
    }
}

static void _AD82581_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

AMP_T* _AMP_GetAmp82581FunTbl(void)
{
	return &_rAmp82581;
}

    
