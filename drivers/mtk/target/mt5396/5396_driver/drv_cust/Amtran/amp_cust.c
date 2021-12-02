/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: adac_cust.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_cust.c
 *  Brief of file amp_cust.c.
 *  Details of file amp_cust.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "amp_cust.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
//#include "aud_debug.h"
#include "drvcust_if.h"
//#include "aud_cfg.h"

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
extern INT32 GPIO_Output(INT32 i4GpioNum, INT32 *pfgSet);
extern INT32 GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val);
extern INT32 GPIO_GetIn(INT32 i4GpioNum);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void  _Cust_Init(void);
static void  _Cust_Mute(BOOL fgEnable);
static void _Cust_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
static void _Cust_SetDacGain(UINT8 u1HpId, INT16 i2Gain);
static void  _Cust_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _Cust_RegRead(UINT8 u1Addr);
static void _Cust_DisableEffect (void);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static AMP_DRV_CB_T _rAmpCustDrvCB = 
{
    _Cust_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_Cust_Init,					//pfnInit
	_Cust_Mute,					//pfnMute 
	_Cust_DacFmtCfg,  			//pfnSetDacInFmt;
	_Cust_SetDacGain,           //pfnSetDacGain;
	_Cust_RegWrite, 			//pfnWrite;
	_Cust_RegRead,				//pfnRead;
	&_rAmpCustDrvCB              // pfnDrvCB;	
};

static AMP_T _rAmpCust = 
{
	"Cust AMP",			// szName[32];
	256,				// i2MaxGain
	-256,				// i2MaxGain
	1,					// u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _Cust_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{

    /* ---  sample code
    UINT32 u4WriteCount = 0; 

    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, CUST_ADDR, 1 ,u1Addr, &u1Data, 1);
    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    }	
    else
    {
        LOG(5, "AMP WRITE FAIL\n");
    }	
    */
}

static UINT8 _Cust_RegRead(UINT8 u1Addr) 
{
    /* --- sample code
    
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, CUST_ADDR, 1 , u1Addr, &u1Data, 1);

    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "AMP READ FAIL\n");
    	return 0;
    }	
    */
    return 0;
}

static void _Cust_Init(void)
{
    //Do amp init here
    
    /* --- sample code
    AOUT_CFG_T*		prAudOutFmt;
    UINT32 u4reset,u4polarity;
	UINT32 AmpEnable,AmpEnablepolarity;

    // Make sure I2c is init
    SIF_Init(); 

    DRVCUST_OptQuery(eAudioAmpEnableGpioNum, &AmpEnable);	//sam 100714
    DRVCUST_OptQuery(eAudioAmpGpioPolarity, &AmpEnablepolarity);
    if (AmpEnable != 0xffffffff)
    {        
        GPIO_Output((INT32)AmpEnable,(INT32 *)&AmpEnablepolarity); 
    }

    DRVCUST_OptQuery(eAudioDigiAMPResetGpio, &u4reset);
    DRVCUST_OptQuery(eAudioDigiAMPResetGpioPolarity, &u4polarity);
    if (u4reset != 0xffffffff)
    {        
        GPIO_Output((INT32)u4reset,(INT32 *)&u4polarity);     
        u4polarity = !u4polarity;
        GPIO_Output((INT32)u4reset,(INT32 *)&u4polarity); 
    }
    
    // Format config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    _Cust_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
    */
}

static void _Cust_Mute(BOOL fgEnable)
{
    /* --- sample code
    if (fgEnable)  //Mute
    {
        // Do mute here 
    }
    else  //Unmute
    {      
        // Do unmute here
	} 
	*/   
}

static void _Cust_SetDacGain(UINT8 u1HpId, INT16 i2Gain)
{
    // set amp gain here    
}

static void _Cust_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    /* --- sample code    
    switch (eFormat)
    {
        case FORMAT_LJ:
        	// LJ setting
        	break;
        case FORMAT_I2S:
        	// I2S setting
        	break;
        case FORMAT_RJ:
        	// RJ setting
        	break;
        default:
        	// default setting
        	break;
    }
    */
}

static void _Cust_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

AMP_T* _AMP_GetAmpCustFunTbl(void)
{
	return &_rAmpCust;
}

