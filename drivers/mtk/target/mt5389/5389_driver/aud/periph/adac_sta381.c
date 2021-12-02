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

#include "adac_sta381.h"
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

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _STA381_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _STA381_RegRead(UINT8 u1Addr);
static void _STA381_Init(void);
static void _STA381_Mute(BOOL fgEnable);
static void _STA381_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
static void _STA381_RegWriteEx(UINT8 u1Addr, UINT8 u1Data, AUD_AMP_ID_T u1AmpID);
static UINT8 _STA381_RegReadEx(UINT8 u1Addr, AUD_AMP_ID_T u1AmpID);
static void _STA381_DisableEffect (void);

void ADAC_STA381_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static BOOL fgSTA381Mute=FALSE;

static AMP_DRV_CB_T _rAmpSTA381DrvCB = 
{
    _STA381_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpFunTbl = 
{
	_STA381_Init,							//pfnInit
	_STA381_Mute,							//pfnMute 
	_STA381_DacFmtCfg,  			   //pfnSetDacInFmt;
	NULL,						 	     		   //pfnSetDacGain;
	_STA381_RegWrite, 					//pfnWrite;
	_STA381_RegRead,					//pfnRead;
	&_rAmpSTA381DrvCB                          // pfnDrvCB;
};

static AMP_T _rAmpSTA381 = 
{
	"STA381",				// szName[32];
	256,							// i2MaxGain
	-256,						// i2MaxGain
	2,								// u1DacNum
	&_rAmpFunTbl		// rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _STA381_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

    u4WriteCount = SIF_X_Write(AUD_AMP_BUS_ID, SIF_CLK_DIV, STA381BW_ADDR, 1 ,u1Addr, &u1Data, 1);

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount <= 0)
    {
        LOG(0, "STA381 WRITE FAIL (address,data) = (0x %x,0x %x)\n", u1Addr, u1Data);
    }	
}

static UINT8 _STA381_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

    u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, STA381BW_ADDR, 1 , u1Addr, &u1Data, 1);
    LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
        return u1Data;
    }	
    else
    {
        LOG(0, "STA381 READ FAIL address = 0x%x\n", u1Addr);
        return 0;
    }	
}

static void _STA381_Init(void)
{
    AOUT_CFG_T*		prAudOutFmt;
    UINT32 u4reset,u4polarity;
    AUD_AQ_AMP_DRC_T eDrcData;

    LOG(0, " Start _STA381_Init...\n");
    // Make sure I2c is init
    SIF_Init(); 
    
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpio, &u4reset));
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpioPolarity, &u4polarity));
    if (u4reset != 0xffffffff)
    {
        u4polarity = !u4polarity;
        AUD_GPIOOutput(u4reset, (INT32)u4polarity);    //digital ampliferT
        x_thread_delay(50);
        u4polarity = !u4polarity;
        AUD_GPIOOutput(u4reset, (INT32)u4polarity);    //digital ampliferT
        LOG(0, " GPIO(%d) = %d\n", u4reset, u4polarity);
    }
    
    //Set as ST map(STA339BWS compatible map)
    _STA381_RegWrite(STA381BW_MISC4, 0x00);            // 00-> ST map, 80->SEC map

    //Initial flow for power on sequence 
    _STA381_RegWrite(STA381BW_MASTER_VOL_CRL, 0xFE);//0x00 for hard mute 
    _STA381_RegWrite(STA381BW_CH1CONFIG, 0x00); 
    _STA381_RegWrite(STA381BW_CH2CONFIG, 0x40); 
    _STA381_RegWrite(STA381BW_CH3CONFIG, 0x80); 

    _STA381_RegWrite(STA381BW_CONFD, 0x4c); 
    //support 2 channel	
    _STA381_RegWrite(STA381BW_CONFF, 0xdc);
    _STA381_RegWrite(STA381BW_MASMUTE, 0x40); 

    // Format config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    _STA381_RegWrite(STA381BW_CONFA, 0x63); // 48K and 256fs
    _STA381_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
    _STA381_RegWrite(STA381BW_CONFC, 0xc2); //change default for HDMI & DTV 48Khz& 44.1Khz sasa background noise
			
			
   // Volume setup
    _STA381_RegWrite(STA381BW_MASTER_VOL_CRL, 0x00);	//Oliver 0x20 -> 0x00 /*MASTER_VOL_0DB=0x00*/

    _STA381_RegWrite(STA381BW_CH1_VOL_CRL, 0x3f);
    _STA381_RegWrite(STA381BW_CH2_VOL_CRL, 0x3f);
    _STA381_RegWrite(STA381BW_CH3_VOL_CRL, 0x22);     
    _STA381_RegWrite(STA381BW_CH1CONFIG, 0x11); 
    _STA381_RegWrite(STA381BW_CH2CONFIG, 0x61); 
    
    _STA381_RegWrite(STA381BW_LIMITER, 0x6f); 
    _STA381_RegWrite(STA381BW_LIMITER_THRESHOLD, 0x6f); 
    
    //HP/Line-out related
    _STA381_RegWrite(STA381BW_HPCFG, 0x00);
    _STA381_RegWrite(STA381BW_F3XCFG2, 0x6e);

    //DualBand DRC Init for Wistron
    eDrcData.u1Cutoff = 0;
    eDrcData.u1LfeGain = 0x22;
    eDrcData.u1AttackHi = 0xb8;
    eDrcData.u1ReleaseHi = 0xb8;
    eDrcData.u1RateHi = 0x8;
    eDrcData.u1AttackLo = 0xb8;
    eDrcData.u1ReleaseLo = 0xb8;
    eDrcData.u1RateLo = 0x8;
    ADAC_STA381_SetDualDRC(AUD_AMP_ID_LR, eDrcData); 
    
    
}

static void _STA381_Mute(BOOL fgEnable)
{
    fgSTA381Mute = fgEnable;

    UNUSED(fgSTA381Mute);
    
if (fgEnable)
    {
       //_STA381_RegWrite(STA381BW_MASTER_VOL_CRL, 0x80);
       //x_thread_delay(15);
       //_STA381_RegWrite(STA381BW_MASTER_VOL_CRL, 0x00);//<----- Workaround!! for Speaker is muted by unknown, Need to debug.
           _STA381_RegWrite(STA381BW_CH1_VOL_CRL, 0xFF);
           _STA381_RegWrite(STA381BW_CH2_VOL_CRL, 0xFF);
	LOG(0, " _STA381_Hard_Mute!!\n\n");
    }
    else
    {
        //_STA381_RegWrite(STA381BW_MASTER_VOL_CRL, 0xFF); 
        _STA381_RegWrite(STA381BW_CH1_VOL_CRL, 0x3f);
        _STA381_RegWrite(STA381BW_CH2_VOL_CRL, 0x3f);
    }
}

static void _STA381_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);
    
    switch (eFormat)
    {
        case FORMAT_LJ:
        	_STA381_RegWrite(STA381BW_CONFB, 0x81);
        	break;
        case FORMAT_I2S:
        	_STA381_RegWrite(STA381BW_CONFB, 0x80);
        	break;
        case FORMAT_RJ:
        	_STA381_RegWrite(STA381BW_CONFB, 0x82);
        	break;
        default:
        	_STA381_RegWrite(STA381BW_CONFB, 0x81);
        	break;
    }
}

static void _STA381_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

AMP_T* _AMP_GetAmpSTA381FunTbl(void)
{
	return &_rAmpSTA381;
}

static void _STA381_RegWriteEx(UINT8 u1Addr, UINT8 u1Data, AUD_AMP_ID_T u1AmpID)
{
    UINT32 u4WriteCount = 0;
    UINT8 u1BusID;
    UINT8 u1DevID;

    UNUSED(u4WriteCount);
    UNUSED(u1BusID);
    UNUSED(u1DevID);    
    // BUS2: 0x3A
    // BUS0: 0x3A Ls/Rs
    // BUS0: 0x38 Center:8, Sub:9    
//#ifdef CC_AUD_AMP_STA333BW
#if 1
    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 0;
        u1DevID = STA381BW_ADDR;             
        u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);          
        break;        
#ifdef CC_AUD_AMP_STA333BW_SA_1
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR;
        u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR_1;
        u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);
        break;        
#endif        
    default:
        break;
    };
    
#else
    UNUSED(_STA381_RegWriteEx);
    return;
#endif //CC_AUD_AMP_STA333BW
}

static UINT8 _STA381_RegReadEx(UINT8 u1Addr, AUD_AMP_ID_T u1AmpID) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;
    UINT8 u1BusID;
    UINT8 u1DevID;

    UNUSED(u1BusID);
    UNUSED(u1DevID);
    UNUSED(u4ReadCount);
    UNUSED(u1Data);
    
//#ifdef CC_AUD_AMP_STA333BW
#if 1
    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 0;
        u1DevID = STA381BW_ADDR;          
        break;        
#ifdef CC_AUD_AMP_STA333BW_SA_1
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR;
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = APOGEE_STA333BW_ADDR_1;
        break;        
#endif
    default:
        u1BusID = 0;
        u1DevID = STA381BW_ADDR;  
        break;
    };

    u4ReadCount = SIF_X_Read(u1BusID, SIF_CLK_DIV, u1DevID, 1 , u1Addr, &u1Data, 1);
    if (u4ReadCount > 0)
    {
    	return u1Data;
    }	
    else
    {
        LOG(5, "STA381 READ FAIL\n");
    	return 0;
    }
#else
    UNUSED(_APOGEE_RegReadEx);
    return 0;
#endif // CC_AUD_AMP_STA333BW
}


void ADAC_STA381_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT8 u1Value)
{
//#ifdef CC_AUD_AMP_STA333BW
#if 1
    switch (u1ChId)
{
    case 0: //Master volume
        _STA381_RegWriteEx(STA381BW_MASTER_VOL_CRL, u1Value, u1AmpID);
        break;
    case 1: //Channel 1
        _STA381_RegWriteEx(STA381BW_CH1_VOL_CRL, u1Value, u1AmpID);    
        break;        
    case 2: //Channel 2
        _STA381_RegWriteEx(STA381BW_CH2_VOL_CRL, u1Value, u1AmpID);
        break;
    default:
        LOG(0,"Incorrect Channel volume control of STA381, u1ChId = %d!!\n", u1ChId);
        break;    
    };
#else
    return;
#endif // CC_AUD_AMP_STA333BW   
}

void ADAC_STA381_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData)    
{
//#ifdef CC_AUD_AMP_STA333BW
#if 1
    UINT8 u1Temp;

    //CufOff Reg: 0xC,  Bit[7:4]
    u1Temp = _STA381_RegReadEx(STA381BW_AUTOMODE,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1Cutoff << 4);
    _STA381_RegWriteEx(STA381BW_AUTOMODE, u1Temp, u1AmpID); 

    //Low Freq Gain Reg: 0xA,  Bit[7:0]
    _STA381_RegWriteEx(STA381BW_CH3_VOL_CRL, (eDrcData.u1LfeGain), u1AmpID);

    //Attack Hi Reg: 0x32,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackHi);
    _STA381_RegWriteEx(STA339_ATTACK1_THRESHOLD, u1Temp, u1AmpID); 
    //Attack Lo Reg: 0x34,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackLo);
    _STA381_RegWriteEx(STA339_ATTACK2_THRESHOLD, u1Temp, u1AmpID);

    //Release Hi Reg: 0x33,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_RELEASE1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseHi);
    _STA381_RegWriteEx(STA339_RELEASE1_THRESHOLD, u1Temp, u1AmpID);
    //Release Lo Reg: 0x35,  Bit[6:0]
    u1Temp = _STA381_RegReadEx(STA339_RELEASE2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseLo);
    _STA381_RegWriteEx(STA339_RELEASE2_THRESHOLD, u1Temp, u1AmpID);

    //Rate Hi Reg: 0x12,  Bit[7:4]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK_RATE1,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1RateHi << 4);
    _STA381_RegWriteEx(STA339_ATTACK_RATE1, u1Temp, u1AmpID);
    //Rate Lo Reg: 0x14,  Bit[7:4]
    u1Temp = _STA381_RegReadEx(STA339_ATTACK_RATE2,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1RateLo << 4);
    _STA381_RegWriteEx(STA339_ATTACK_RATE2, u1Temp, u1AmpID);    
        
#else
    return;
#endif // CC_AUD_AMP_STA333BW
}

BOOL ADAC_STA381_ReadRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, UINT8* u1Data, BOOL fgAllSet)
{
//#ifdef CC_AUD_AMP_STA333BW
#if 1
    UINT8 u1Temp;
        
    _STA381_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _STA381_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16
    u1Temp = _STA381_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro R1 bit in reg 0x26
    u1Temp |= 0x4;
    _STA381_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);

    u1Temp = _STA381_RegReadEx(STA339_B1CF1, u1AmpID);     //0x17 : Top 8 bits of Data.
    u1Data[0] = u1Temp;
    u1Temp = _STA381_RegReadEx(STA339_B1CF2, u1AmpID);     //0x18 : Mid 8 bits of Data.
    u1Data[1] = u1Temp;
    u1Temp = _STA381_RegReadEx(STA339_B1CF3, u1AmpID);     //0x19 : Last 8 bits of Data.
    u1Data[2] = u1Temp;    

    if (fgAllSet)        
    {
        u1Temp = _STA381_RegReadEx(STA339_B2CF1, u1AmpID);     
        u1Data[3] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B2CF2, u1AmpID);     
        u1Data[4] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B2CF3, u1AmpID);     
        u1Data[5] = u1Temp;

        u1Temp = _STA381_RegReadEx(STA339_A1CF1, u1AmpID);     
        u1Data[6] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A1CF2, u1AmpID);     
        u1Data[7] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A1CF3, u1AmpID);     
        u1Data[8] = u1Temp;

        u1Temp = _STA381_RegReadEx(STA339_A2CF1, u1AmpID);     
        u1Data[9] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A2CF2, u1AmpID);     
        u1Data[10] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_A2CF3, u1AmpID);     
        u1Data[11] = u1Temp;        

        u1Temp = _STA381_RegReadEx(STA339_B0CF1, u1AmpID);     
        u1Data[12] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B0CF2, u1AmpID);     
        u1Data[13] = u1Temp;
        u1Temp = _STA381_RegReadEx(STA339_B0CF3, u1AmpID);     
        u1Data[14] = u1Temp;                
    }

    return TRUE;
#else    
    UNUSED(ADAC_STA381_ReadRam);
    return FALSE;
#endif //CC_AUD_AMP_STA333BW
}

BOOL ADAC_STA381_WriteRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, AUD_AQ_AMP_RAM_DATA_T rAmpRamData, BOOL fgAllSet)
{
//#ifdef CC_AUD_AMP_STA333BW
#if 1
    UINT8 u1Temp;
        
    _STA381_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _STA381_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16

    _STA381_RegWriteEx(STA339_B1CF1, rAmpRamData.u1Data[0], u1AmpID);     //0x17 : Top 8 bits of Data.
    _STA381_RegWriteEx(STA339_B1CF2, rAmpRamData.u1Data[1], u1AmpID);     //0x18 : Mid 8 bits of Data.
    _STA381_RegWriteEx(STA339_B1CF3, rAmpRamData.u1Data[2], u1AmpID);     //0x19 : Last 8 bits of Data.

    if (fgAllSet)        
    {
        _STA381_RegWriteEx(STA339_B2CF1, rAmpRamData.u1Data[3], u1AmpID); 
        _STA381_RegWriteEx(STA339_B2CF2, rAmpRamData.u1Data[4], u1AmpID); 
        _STA381_RegWriteEx(STA339_B2CF3, rAmpRamData.u1Data[5], u1AmpID); 

        _STA381_RegWriteEx(STA339_A1CF1, rAmpRamData.u1Data[6], u1AmpID); 
        _STA381_RegWriteEx(STA339_A1CF2, rAmpRamData.u1Data[7], u1AmpID); 
        _STA381_RegWriteEx(STA339_A1CF3, rAmpRamData.u1Data[8], u1AmpID); 

        _STA381_RegWriteEx(STA339_A2CF1, rAmpRamData.u1Data[9], u1AmpID); 
        _STA381_RegWriteEx(STA339_A2CF2, rAmpRamData.u1Data[10], u1AmpID); 
        _STA381_RegWriteEx(STA339_A2CF3, rAmpRamData.u1Data[11], u1AmpID); 

        _STA381_RegWriteEx(STA339_B0CF1, rAmpRamData.u1Data[12], u1AmpID); 
        _STA381_RegWriteEx(STA339_B0CF2, rAmpRamData.u1Data[13], u1AmpID); 
        _STA381_RegWriteEx(STA339_B0CF3, rAmpRamData.u1Data[14], u1AmpID);              
    }

    u1Temp = _STA381_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro W1 bit in reg 0x26
    u1Temp |= 0x1;
    _STA381_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);

    return TRUE;
#else    
    UNUSED(ADAC_STA381_WriteRam);
    return FALSE;
#endif //CC_AUD_AMP_STA333BW
}

void ADAC_STA381_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4PostSclCh1, UINT32 u4PostSclCh2)
{
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
        
    UNUSED(ADAC_STA381_SetPostScale);        
    UNUSED(u4PostSclCh1);
    UNUSED(u4PostSclCh2);    

    rAmpData.u1Data[0] = (u4PostSclCh1 & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh1 & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh1 & 0xFF);    
    ADAC_STA381_WriteRam(u1AmpID,0,0x34,rAmpData, FALSE);

    rAmpData.u1Data[0] = (u4PostSclCh2 & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh2 & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh2 & 0xFF);     
    ADAC_STA381_WriteRam(u1AmpID,0,0x35,rAmpData, FALSE);
    
    return;
}

void ADAC_STA381_SetPEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T rAQExtPeqData)
{ 
    UINT8 u1Temp;
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
    UINT8 i,j;
    UINT8 u1Idx;
    
    UNUSED(u1AmpID);
    UNUSED(rAQExtPeqData);
    UNUSED(ADAC_STA381_SetPEQ);   
    // Set 1 to bit4 in reg 0x3
    u1Temp = _STA381_RegReadEx(STA381BW_CONFD, u1AmpID);     
    u1Temp |= 0x10;
    _STA381_RegWriteEx(STA381BW_CONFD, u1Temp, u1AmpID);

    // Write Coeffs to Ram. (Band 0 ~ 7 , 5 coeffs each.)
    for (i=0; i<7; i++) // 7 Bands
    {
        u1Idx= 0;
        for (j=0; j<15; j+=3) //For one band, there are 5 coeffs, each are 3 byte.
        {
            rAmpData.u1Data[j] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF0000) >> 16;
            rAmpData.u1Data[j+1] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF00)>> 8;
            rAmpData.u1Data[j+2] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF);
            LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j,rAmpData.u1Data[j]);
            LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+1,rAmpData.u1Data[j+1]);
            LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+2,rAmpData.u1Data[j+2]);
            u1Idx += 1;
        }
        ADAC_STA381_WriteRam(u1AmpID, 0, (i*5), rAmpData, TRUE);
        LOG(3,"EXT_PEQ - Write PEQ to band %d, Start Addr %d\n", i, i*5);
    }        
}

