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
 * $RCSfile: adac_apogee.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_apogee.c
 *  Brief of file adac_apogee.c.
 *  Details of file adac_apogee.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "adac_ntp7400l.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "aud_cfg.h"

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
extern INT32 GPIO_Output(INT32 i4GpioNum, const INT32 *pfgSet);
extern INT32 GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val);
extern INT32 GPIO_GetIn(INT32 i4GpioNum);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
static void _NTP_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _NTP_RegRead(UINT8 u1Addr);

#ifdef CC_AUD_NTP7400_AMP_CTRL
static void _NTP_Init(void);
static void _NTP_Mute(BOOL fgEnable);
static void _NTP_DisableEffect (void);
#endif

//static void _NTP_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
//static BOOL fgSTMute=FALSE;
#ifdef CC_AUD_NTP7400_AMP_CTRL
static AMP_DRV_CB_T _rAmpNTPDrvCB = 
{
    _NTP_DisableEffect // pfnDisableEffect, disable sound effect   
};
#else
static AMP_DRV_CB_T _rAmpNTPDrvCB = 
{
    NULL // pfnDisableEffect, disable sound effect   
};
#endif

#ifdef CC_AUD_NTP7400_AMP_CTRL
static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_NTP_Init,					//pfnInit
	_NTP_Mute,					//pfnMute 
	NULL,  			                     //pfnSetDacInFmt;
	NULL,						//pfnSetDacGain;
	_NTP_RegWrite, 				//pfnWrite;
	_NTP_RegRead				//pfnRead;
};
#else
static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	NULL,					//pfnInit
	NULL,					//pfnMute 
	NULL,  			                     //pfnSetDacInFmt;
	NULL,						//pfnSetDacGain;
	_NTP_RegWrite, 				//pfnWrite;
	_NTP_RegRead,				//pfnRead;
	&_rAmpNTPDrvCB                          // pfnDrvCB;	
};
#endif

static AMP_T _rAmpNTP = 
{
	"NTP-7400L",				// szName[32];
	256,					// i2MaxGain
	-256,				// i2MaxGain
	2,					// u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _NTP_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7400L_ADDR, 1 ,u1Addr, &u1Data, 1);
    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount <= 0)
    {
        LOG(5, "NTP WRITE FAIL\n");
    }	

    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7400L_ADDR_SW, 1 ,u1Addr, &u1Data, 1);
    LOG( 9,"I2C write SW (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount <= 0)
    {
        LOG(5, "NTP WRITE SW FAIL\n");
    }	    
}

static UINT8 _NTP_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7400L_ADDR, 1 , u1Addr, &u1Data, 1);

    LOG( 5,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	//return u1Data;
    }	
    else
    {
        LOG(5, "NTP READ FAIL\n");
    	return 0;
    }	

    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, NTP7400L_ADDR_SW, 1 , u1Addr, &u1Data, 1);

    LOG( 5,"I2C read SW (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "NTP READ SW FAIL\n");
    	return 0;
    }	    
}

#ifdef CC_AUD_NTP7400_AMP_CTRL
static void _NTP_Init(void)
{
    //AOUT_CFG_T*		prAudOutFmt;
    // Make sure I2c is init
    
    SIF_Init(); 
    //UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    //_NTP_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
    _NTP_RegWrite(NTP_SM , SM_INC);  
    _NTP_RegWrite(NTP_POF, POF_ON);  
    _NTP_RegWrite(NTP_PWM_MASK0, PWMM_H);  
    
    _NTP_Mute(0);
}

static void _NTP_Mute(BOOL fgEnable)
{

	if (fgEnable)  //Mute
    {
        _NTP_RegWrite(NTP_MASTER_VOL, MASTER_VOL_MUTE);       
    }
    else  //Unmute
    {      
       _NTP_RegWrite(NTP_MASTER_VOL, MASTER_VOL_0DB); 	   
    }

}
#endif

#ifdef CC_AUD_NTP7400_AMP_CTRL
static void _NTP_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}
#endif

#if 0
static void _NTP_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);
    
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
}
#endif

AMP_T* _AMP_GetAmpNTPFunTbl(void)
{
	return &_rAmpNTP;
}


