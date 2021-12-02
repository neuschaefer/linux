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
 * $RCSfile: codec_AD8356.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file codec_ad8356.c
 *  Brief of file codec_ad8356.c.
 *  Details of file codec_ad8356.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "codec_ad8356.h"
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
#define SAMPLE_RATE_NUM         3
#define SOURCE_NUM              3
    #define SOURCE_FROM_AV      0
    #define SOURCE_FROM_ATV     1
    #define SOURCE_FROM_DTV     2
#define EQ_BIQUAD_NUM           4
#define HL_BIQUAD_NUM           2

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
extern INT32 GPIO_Output(INT32 i4GpioNum, const INT32 *pfgSet);
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

void _AD8356_RegWrite(UINT8 u1Addr, UINT8 u1Data);
UINT8 _AD8356_RegRead(UINT8 u1Addr);
void _AD8356_Init(void);
void _AD8356_Mute(BOOL fgEnable);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static BOOL fgAD8356Mute=FALSE;
static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_AD8356_Init,							//pfnInit
	_AD8356_Mute,							//pfnMute 
	NULL,  			   //pfnSetDacInFmt;
	NULL,						 	     		   //pfnSetDacGain;
	_AD8356_RegWrite, 					//pfnWrite;
	_AD8356_RegRead					//pfnRead;
};

static AMP_T _rAmpAD8356 = 
{
	"AD8356",				// szName[32];
	0,							// i2MaxGain
	0,						// i2MaxGain
	0,								// u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _AD8356_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 
    u4WriteCount = SIF_X_Write(CHannelID,SIF_SLOW_CLK_DIV, AD8356_DEV_ADDR,1, u1Addr, &u1Data, 1);
    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    }	
    else
    {
        LOG(5, "AD8356 WRITE FAIL\n");
    }	
}

UINT8 _AD8356_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;
    u4ReadCount = SIF_X_Read(CHannelID,SIF_SLOW_CLK_DIV, AD8356_DEV_ADDR, 1,u1Addr, &u1Data, 1);
    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "AD8356 READ FAIL\n");
    	return 0;
    }	
}

void _AD8356_Init(void)
{
    // Make sure I2c is init
    SIF_Init(); 
    Printf("_AD8356_Init.\n");
     _AD8356_RegWrite(REG_AD8356_02,0);
}

void _AD8356_Mute(BOOL fgEnable)
{
    fgAD8356Mute = fgEnable;

    UNUSED(fgAD8356Mute);
    Printf("_AD8356_Mute.\n");
    
    if (fgEnable)
    {
        _AD8356_RegWrite(REG_AD8356_02,MUTE);
    }
    else
    {
        _AD8356_RegWrite(REG_AD8356_02,UNMUTE);
    }
}
AMP_T* _AMP_GetAmpAD8356FunTbl(void)
{
	return &_rAmpAD8356;
}


