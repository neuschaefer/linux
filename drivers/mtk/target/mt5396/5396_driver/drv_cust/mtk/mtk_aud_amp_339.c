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
 * $RCSfile: mtk_aud_amp_339.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mtk_aud_amp_339.c
 *  Brief of file mtk_aud_amp_339.c.
 *  Details of file mtk_aud_amp_339.c (optional).
 */

#define AUD_EXTERN_FILE

//=======================
// [**CAUTION**] 
// Please exclude Loader
//=======================
#if ((!defined(CC_MTK_PRELOADER)) && (!defined(CC_MTK_LOADER))) 

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/* =======================================================
 [**CAUTION**] 
 You can only include header files in 
 \target\mtxxxx\xxxx_driver\drv_inc\  
 or files in \vm_linux\output\sysbuild\xxxx\rel or debug\inc\drv_inc\
 or files in the same directory.
========================================================= */
#include "mtk_aud_amp_339.h"    
#include "sif_if.h"
#include "aud_if.h"
#include "drvcust_if.h"

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

static void _MTK_STA339_RegWrite(UINT8 u1Addr, UINT8 u1Data);
static UINT8 _MTK_STA339_RegRead(UINT8 u1Addr);
static void _MTK_STA339_Init(void);
static void _MTK_STA339_Mute(BOOL fgEnable);
static void _MTK_STA339_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk);
static void _MTK_STA339_DisableEffect (void);
#ifdef CC_AUD_AMP_STA333BW
static void _MTK_STA339_RegWriteEx(UINT8 u1Addr, UINT8 u1Data, AUD_AMP_ID_T u1AmpID);
static UINT8 _MTK_STA339_RegReadEx(UINT8 u1Addr, AUD_AMP_ID_T u1AmpID);
#endif

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static BOOL fgMtk_Sta339Mute=FALSE;

static AMP_DRV_CB_T _rAmpMtk_Sta339DrvCB = 
{
    _MTK_STA339_DisableEffect // pfnDisableEffect, disable sound effect   
};

static AMP_FUNCTBL_T _rAmpFunTbl = 
{
    _MTK_STA339_Init,                           //pfnInit
    _MTK_STA339_Mute,                           //pfnMute 
    _MTK_STA339_DacFmtCfg,                      //pfnSetDacInFmt;
    NULL,                                       //pfnSetDacGain;
    _MTK_STA339_RegWrite,                       //pfnWrite;
    _MTK_STA339_RegRead,                        //pfnRead;
    &_rAmpMtk_Sta339DrvCB,                      //pfnDrvCB;    
};

static AMP_T _rAmpMtk_Sta339 = 
{
    "MTK_STA339",               // szName[32];
    256,                        // i2MaxGain
    -256,                       // i2MaxGain
    2,                          // u1DacNum
    &_rAmpFunTbl                // rCodecFuncTbl
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

// =================================================
// [**CAUTION**] 
// 1. Use printf, not LOG
// 2. You can only call public functions in xxx_if.c
//==================================================

static void _MTK_STA339_RegWrite(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

#ifdef CC_AUD_AMP_STA333BW
  #ifndef CC_AUD_AMP_STA333BW_SA_1
    u4WriteCount = SIF_X_Write(2, SIF_CLK_DIV, MTK_STA339_STA333BW_ADDR, 1 ,u1Addr, &u1Data, 1);
  #else
    // BUS0: 0x38 0x3A , BUS2 : 3A
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, MTK_STA339_STA333BW_ADDR, 1 ,u1Addr, &u1Data, 1);
    u4WriteCount = SIF_X_Write(0, SIF_CLK_DIV, MTK_STA339_STA333BW_ADDR_1, 1 ,u1Addr, &u1Data, 1);
  #endif
#else
    u4WriteCount = SIF_Write(SIF_CLK_DIV, MTK_STA339_DDXI2051_ADDR, u1Addr, &u1Data, 1);
#endif

    //printf("I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    }    
    else
    {
        //printf("MTK_STA339 WRITE FAIL\n");
    }    
}

static UINT8 _MTK_STA339_RegRead(UINT8 u1Addr) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;

#ifdef CC_AUD_AMP_STA333BW
    u4ReadCount = SIF_X_Read(2, SIF_CLK_DIV, MTK_STA339_STA333BW_ADDR, 1 , u1Addr, &u1Data, 1);
#else
    u4ReadCount = SIF_Read(SIF_CLK_DIV, MTK_STA339_DDXI2051_ADDR, u1Addr, &u1Data, 1);
#endif

    //printf("I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, u1Data);

    if (u4ReadCount > 0)
    {
        return u1Data;
    }    
    else
    {
        //printf("MTK_STA339 READ FAIL\n");
        return 0;
    }    
}

static void _MTK_STA339_Init(void)
{
    AOUT_CFG_T*        prAudOutFmt;
    UINT32 u4reset,u4polarity;

    // Make sure I2c is init
    SIF_Init(); 
    
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpio, &u4reset));
    UNUSED(DRVCUST_OptQuery(eAudioDigiAMPResetGpioPolarity, &u4polarity));
    if (u4reset != 0xffffffff)
    {
        u4polarity = !u4polarity;
        GPIO_Output(u4reset, (INT32*)&u4polarity);    //digital ampliferT
        x_thread_delay(50);
        u4polarity = !u4polarity;
        GPIO_Output(u4reset, (INT32*)&u4polarity);    //digital ampliferT
    }

    //Initial flow for power on sequence 
    _MTK_STA339_RegWrite(REG_MTK_STA339_MASTER_VOL_CRL, 0xFE); 
    _MTK_STA339_RegWrite(STA232W_CH1CONFIG, 0x00); 
    _MTK_STA339_RegWrite(STA232W_CH2CONFIG, 0x40); 
    _MTK_STA339_RegWrite(STA232W_CH3CONFIG, 0x80); 

    _MTK_STA339_RegWrite(STA232W_CONFD, 0x4c); 
    //support 2 channel    
    _MTK_STA339_RegWrite(STA232W_CONFF, 0xdc);
    _MTK_STA339_RegWrite(STA232W_MASMUTE, 0x40); 

    // Format config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *)&prAudOutFmt));
    _MTK_STA339_RegWrite(STA232W_CONFA, 0x63); // 48K and 256fs
    _MTK_STA339_DacFmtCfg(prAudOutFmt->eFormat, (MCLK_FREQUENCY_T)DRVCUST_OptGet(eAudioDacMclk));
    _MTK_STA339_RegWrite(STA232W_CONFC, 0xc2); //change default for HDMI & DTV 48Khz& 44.1Khz sasa background noise
            
    // Volume setup
    _MTK_STA339_RegWrite(REG_MTK_STA339_CHL_1_VOL_CRL, 0x41);
    _MTK_STA339_RegWrite(REG_MTK_STA339_CHL_2_VOL_CRL, 0x41);
    _MTK_STA339_RegWrite(REG_MTK_STA339_CHL_3_VOL_CRL, 0x22); 
    _MTK_STA339_RegWrite(STA232W_CH1CONFIG, 0x10); 
    _MTK_STA339_RegWrite(STA232W_CH2CONFIG, 0x50); 
    
    _MTK_STA339_RegWrite(STA232W_LIMITER, 0x6f); 
    _MTK_STA339_RegWrite(STA232W_LIMITER_THRESHOLD, 0x6f); 

}

static void _MTK_STA339_Mute(BOOL fgEnable)
{
    fgMtk_Sta339Mute = fgEnable;

    UNUSED(fgMtk_Sta339Mute);
    
    if (fgEnable)
    {
       _MTK_STA339_RegWrite(REG_MTK_STA339_MASTER_VOL_CRL, 0x80);
       x_thread_delay(15);
       _MTK_STA339_RegWrite(REG_MTK_STA339_MASTER_VOL_CRL, 0xFF);//<----- Workaround!! for Speaker is muted by unknown, Need to debug.
    }
    else
    {
       _MTK_STA339_RegWrite(REG_MTK_STA339_MASTER_VOL_CRL, 0x00); 
    }
}

static void _MTK_STA339_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(eMclk);
    
    switch (eFormat)
    {
        case FORMAT_LJ:
            _MTK_STA339_RegWrite(STA232W_CONFB, 0x81);
            break;
        case FORMAT_I2S:
            _MTK_STA339_RegWrite(STA232W_CONFB, 0x80);
            break;
        case FORMAT_RJ:
            _MTK_STA339_RegWrite(STA232W_CONFB, 0x82);
            break;
        default:
            _MTK_STA339_RegWrite(STA232W_CONFB, 0x81);
            break;
    }
}

static void _MTK_STA339_DisableEffect (void)
{
    // FIX ME!!!
    // Please add disable sound effect commands in this function.
    // Let the freq. response the same after amp.
}

// ===============================================================
// [**CAUTION**] 
//  MUST provide this API with this naming "AUD_AMP_GetExtFunTbl."
// ===============================================================
AMP_T* AUD_AMP_GetDrvCustFunTbl(void)
{
    return &_rAmpMtk_Sta339;
}


#ifdef CC_AUD_AMP_STA333BW
static void _MTK_STA339_RegWriteEx(UINT8 u1Addr, UINT8 u1Data, AUD_AMP_ID_T u1AmpID)
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
#ifdef CC_AUD_AMP_STA333BW
    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 2;
        u1DevID = MTK_STA339_STA333BW_ADDR;             
        break;        
#ifdef CC_AUD_AMP_STA333BW_SA_1
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = MTK_STA339_STA333BW_ADDR;
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = MTK_STA339_STA333BW_ADDR_1;
        break;        
#endif        
    default:
        u1BusID = 2;
        u1DevID = MTK_STA339_STA333BW_ADDR;
        break;
    };
    u4WriteCount = SIF_X_Write(u1BusID, SIF_CLK_DIV, u1DevID, 1 ,u1Addr, &u1Data, 1);
#else
    UNUSED(_MTK_STA339_RegWriteEx);
    return;
#endif //CC_AUD_AMP_STA333BW
}

static UINT8 _MTK_STA339_RegReadEx(UINT8 u1Addr, AUD_AMP_ID_T u1AmpID) 
{
    UINT32 u4ReadCount = 0;
    UINT8 u1Data;
    UINT8 u1BusID;
    UINT8 u1DevID;

    UNUSED(u1BusID);
    UNUSED(u1DevID);
    UNUSED(u4ReadCount);
    UNUSED(u1Data);
    
#ifdef CC_AUD_AMP_STA333BW
    switch (u1AmpID)
    {
    case AUD_AMP_ID_LR:
        u1BusID = 2;
        u1DevID = MTK_STA339_STA333BW_ADDR;             
        break;        
#ifdef CC_AUD_AMP_STA333BW_SA_1
    case AUD_AMP_ID_LsRs:
        u1BusID = 0;
        u1DevID = MTK_STA339_STA333BW_ADDR;
        break;        
    case AUD_AMP_ID_CSw:
        u1BusID = 0;
        u1DevID = MTK_STA339_STA333BW_ADDR_1;
        break;        
#endif
    default:
        u1BusID = 2;
        u1DevID = MTK_STA339_STA333BW_ADDR;
        break;
    };

    u4ReadCount = SIF_X_Read(u1BusID, SIF_CLK_DIV, u1DevID, 1 , u1Addr, &u1Data, 1);
    if (u4ReadCount > 0)
    {
        return u1Data;
    }    
    else
    {
        //LOG(5, "MTK_STA339 READ FAIL\n");
        return 0;
    }
#else
    UNUSED(_MTK_STA339_RegReadEx);
    return 0;
#endif // CC_AUD_AMP_STA333BW
}
#endif

void MTK_STA339_SetChannelVol(AUD_AMP_ID_T u1AmpID, UINT8 u1ChId, UINT8 u1Value)
{
#ifdef CC_AUD_AMP_STA333BW
    switch (u1ChId)
{
    case 0: //Master volume
        _MTK_STA339_RegWriteEx(REG_MTK_STA339_MASTER_VOL_CRL, u1Value, u1AmpID);
        break;
    case 1: //Channel 1
        _MTK_STA339_RegWriteEx(REG_MTK_STA339_CHL_1_VOL_CRL, u1Value, u1AmpID);    
        break;        
    case 2: //Channel 2
        _MTK_STA339_RegWriteEx(REG_MTK_STA339_CHL_2_VOL_CRL, u1Value, u1AmpID);
        break;
    };
#else
    return;
#endif // CC_AUD_AMP_STA333BW   
}

void MTK_STA339_SetDualDRC(AUD_AMP_ID_T u1AmpID, AUD_AQ_AMP_DRC_T eDrcData)    
{
#ifdef CC_AUD_AMP_STA333BW
    UINT8 u1Temp;

    //CufOff Reg: 0xC,  Bit[7:4]
    u1Temp = _MTK_STA339_RegReadEx(REG_MTK_STA339_AUTOMODE_REG2,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1Cutoff << 4);
    _MTK_STA339_RegWriteEx(REG_MTK_STA339_AUTOMODE_REG2, u1Temp, u1AmpID); 

    //Low Freq Gain Reg: 0xA,  Bit[7:0]
    _MTK_STA339_RegWriteEx(REG_MTK_STA339_CHL_3_VOL_CRL, (eDrcData.u1LfeGain), u1AmpID);

    //Attack Hi Reg: 0x32,  Bit[6:0]
    u1Temp = _MTK_STA339_RegReadEx(STA339_ATTACK1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackHi);
    _MTK_STA339_RegWriteEx(STA339_ATTACK1_THRESHOLD, u1Temp, u1AmpID); 
    //Attack Lo Reg: 0x34,  Bit[6:0]
    u1Temp = _MTK_STA339_RegReadEx(STA339_ATTACK2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1AttackLo);
    _MTK_STA339_RegWriteEx(STA339_ATTACK2_THRESHOLD, u1Temp, u1AmpID);

    //Release Hi Reg: 0x33,  Bit[6:0]
    u1Temp = _MTK_STA339_RegReadEx(STA339_RELEASE1_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseHi);
    _MTK_STA339_RegWriteEx(STA339_RELEASE1_THRESHOLD, u1Temp, u1AmpID);
    //Release Lo Reg: 0x35,  Bit[6:0]
    u1Temp = _MTK_STA339_RegReadEx(STA339_RELEASE2_THRESHOLD, u1AmpID);
    u1Temp = (u1Temp & 0x80) | (eDrcData.u1ReleaseLo);
    _MTK_STA339_RegWriteEx(STA339_RELEASE2_THRESHOLD, u1Temp, u1AmpID);

    //Rate Hi Reg: 0x12,  Bit[7:4]
    u1Temp = _MTK_STA339_RegReadEx(STA339_ATTACK_RATE1,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1RateHi << 4);
    _MTK_STA339_RegWriteEx(STA339_ATTACK_RATE1, u1Temp, u1AmpID);
    //Rate Lo Reg: 0x14,  Bit[7:4]
    u1Temp = _MTK_STA339_RegReadEx(STA339_ATTACK_RATE2,u1AmpID);
    u1Temp = (u1Temp & 0xF) | (eDrcData.u1RateLo << 4);
    _MTK_STA339_RegWriteEx(STA339_ATTACK_RATE2, u1Temp, u1AmpID);    
        
#else
    return;
#endif // CC_AUD_AMP_STA333BW
}

BOOL MTK_STA339_ReadRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, UINT8* u1Data, BOOL fgAllSet)
{
#ifdef CC_AUD_AMP_STA333BW
    UINT8 u1Temp;
        
    _MTK_STA339_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _MTK_STA339_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16
    u1Temp = _MTK_STA339_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro R1 bit in reg 0x26
    u1Temp |= 0x4;
    _MTK_STA339_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);

    u1Temp = _MTK_STA339_RegReadEx(STA339_B1CF1, u1AmpID);     //0x17 : Top 8 bits of Data.
    u1Data[0] = u1Temp;
    u1Temp = _MTK_STA339_RegReadEx(STA339_B1CF2, u1AmpID);     //0x18 : Mid 8 bits of Data.
    u1Data[1] = u1Temp;
    u1Temp = _MTK_STA339_RegReadEx(STA339_B1CF3, u1AmpID);     //0x19 : Last 8 bits of Data.
    u1Data[2] = u1Temp;    

    if (fgAllSet)        
    {
        u1Temp = _MTK_STA339_RegReadEx(STA339_B2CF1, u1AmpID);     
        u1Data[3] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_B2CF2, u1AmpID);     
        u1Data[4] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_B2CF3, u1AmpID);     
        u1Data[5] = u1Temp;

        u1Temp = _MTK_STA339_RegReadEx(STA339_A1CF1, u1AmpID);     
        u1Data[6] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_A1CF2, u1AmpID);     
        u1Data[7] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_A1CF3, u1AmpID);     
        u1Data[8] = u1Temp;

        u1Temp = _MTK_STA339_RegReadEx(STA339_A2CF1, u1AmpID);     
        u1Data[9] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_A2CF2, u1AmpID);     
        u1Data[10] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_A2CF3, u1AmpID);     
        u1Data[11] = u1Temp;        

        u1Temp = _MTK_STA339_RegReadEx(STA339_B0CF1, u1AmpID);     
        u1Data[12] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_B0CF2, u1AmpID);     
        u1Data[13] = u1Temp;
        u1Temp = _MTK_STA339_RegReadEx(STA339_B0CF3, u1AmpID);     
        u1Data[14] = u1Temp;                
    }

    return TRUE;
#else    
    UNUSED(MTK_STA339_ReadRam);
    return FALSE;
#endif //CC_AUD_AMP_STA333BW
}

BOOL MTK_STA339_WriteRam(AUD_AMP_ID_T u1AmpID, UINT8 u1Bank, UINT8 u1Addr, AUD_AQ_AMP_RAM_DATA_T rAmpRamData, BOOL fgAllSet)
{
#ifdef CC_AUD_AMP_STA333BW
    UINT8 u1Temp;
        
    _MTK_STA339_RegWriteEx(STA339_EQDRC_CFG, u1Bank, u1AmpID); // Set EQ Cfg, select Bank.
    _MTK_STA339_RegWriteEx(STA339_CFADDR, u1Addr, u1AmpID);    // Write Addr to reg 0x16

    _MTK_STA339_RegWriteEx(STA339_B1CF1, rAmpRamData.u1Data[0], u1AmpID);     //0x17 : Top 8 bits of Data.
    _MTK_STA339_RegWriteEx(STA339_B1CF2, rAmpRamData.u1Data[1], u1AmpID);     //0x18 : Mid 8 bits of Data.
    _MTK_STA339_RegWriteEx(STA339_B1CF3, rAmpRamData.u1Data[2], u1AmpID);     //0x19 : Last 8 bits of Data.

    if (fgAllSet)        
    {
        _MTK_STA339_RegWriteEx(STA339_B2CF1, rAmpRamData.u1Data[3], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_B2CF2, rAmpRamData.u1Data[4], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_B2CF3, rAmpRamData.u1Data[5], u1AmpID); 

        _MTK_STA339_RegWriteEx(STA339_A1CF1, rAmpRamData.u1Data[6], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_A1CF2, rAmpRamData.u1Data[7], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_A1CF3, rAmpRamData.u1Data[8], u1AmpID); 

        _MTK_STA339_RegWriteEx(STA339_A2CF1, rAmpRamData.u1Data[9], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_A2CF2, rAmpRamData.u1Data[10], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_A2CF3, rAmpRamData.u1Data[11], u1AmpID); 

        _MTK_STA339_RegWriteEx(STA339_B0CF1, rAmpRamData.u1Data[12], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_B0CF2, rAmpRamData.u1Data[13], u1AmpID); 
        _MTK_STA339_RegWriteEx(STA339_B0CF3, rAmpRamData.u1Data[14], u1AmpID);              
    }

    u1Temp = _MTK_STA339_RegReadEx(STA339_CFUD, u1AmpID);      // Write 1 ro W1 bit in reg 0x26
    u1Temp |= 0x1;
    _MTK_STA339_RegWriteEx(STA339_CFUD, u1Temp, u1AmpID);

    return TRUE;
#else    
    UNUSED(MTK_STA339_WriteRam);
    return FALSE;
#endif //CC_AUD_AMP_STA333BW
}

void  MTK_STA339_SetPostScale(AUD_AMP_ID_T u1AmpID, UINT32 u4PostSclCh1, UINT32 u4PostSclCh2)
{
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
        
    UNUSED(MTK_STA339_SetPostScale);        
    UNUSED(u4PostSclCh1);
    UNUSED(u4PostSclCh2);    

    rAmpData.u1Data[0] = (u4PostSclCh1 & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh1 & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh1 & 0xFF);    
    MTK_STA339_WriteRam(u1AmpID,0,0x34,rAmpData, FALSE);

    rAmpData.u1Data[0] = (u4PostSclCh2 & 0xFF0000)>> 16;
    rAmpData.u1Data[1] = (u4PostSclCh2 & 0xFF00)>> 8;
    rAmpData.u1Data[2] = (u4PostSclCh2 & 0xFF);     
    MTK_STA339_WriteRam(u1AmpID,0,0x35,rAmpData, FALSE);
    
    return;
}

void MTK_STA339_SetPEQ(AUD_AMP_ID_T u1AmpID, AUD_AQ_EXTPEQ_DATA_T rAQExtPeqData)
{
    UNUSED(u1AmpID);
    UNUSED(rAQExtPeqData);
    UNUSED(MTK_STA339_SetPEQ);    

#ifdef CC_AUD_AMP_STA333BW
    UINT8 u1Temp;
    AUD_AQ_AMP_RAM_DATA_T rAmpData;
    UINT8 i,j;
    UINT8 u1Idx;

    // Set 1 to bit4 in reg 0x3
    u1Temp = _MTK_STA339_RegReadEx(STA232W_CONFD, u1AmpID);     
    u1Temp |= 0x10;
    _MTK_STA339_RegWriteEx(STA232W_CONFD, u1Temp, u1AmpID);

    // Write Coeffs to Ram. (Band 0 ~ 7 , 5 coeffs each.)
    for (i=0; i<7; i++) // 7 Bands
    {
        u1Idx= 0;
        for (j=0; j<15; j+=3) //For one band, there are 5 coeffs, each are 3 byte.
        {
            rAmpData.u1Data[j] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF0000) >> 16;
            rAmpData.u1Data[j+1] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF00)>> 8;
            rAmpData.u1Data[j+2] = ((rAQExtPeqData.u4Data[u1Idx]) & 0xFF);
            //LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j,rAmpData.u1Data[j]);
            //LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+1,rAmpData.u1Data[j+1]);
            //LOG(3,"EXT_PEQ - rAmpData.u1Data[%d] = 0x%x\n", j+2,rAmpData.u1Data[j+2]);
            u1Idx += 1;
        }
        MTK_STA339_WriteRam(u1AmpID, 0, (i*5), rAmpData, TRUE);
        //LOG(3,"EXT_PEQ - Write PEQ to band %d, Start Addr %d\n", i, i*5);
    }        
#else
    return;
#endif //CC_AUD_AMP_STA333BW
}

#endif //CC_MTK_PRELOADER, CC_MTK_LOADER

