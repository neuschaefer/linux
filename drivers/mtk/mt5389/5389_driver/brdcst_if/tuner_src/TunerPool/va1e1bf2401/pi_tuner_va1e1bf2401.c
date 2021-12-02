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
 * $RCSfile: pi_tuner_va1e1bf2401.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_va1e1bf2401.c
 *  Tuner control for sahrp va1e1bf2401
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "pi_demod_atd.h"
#include "fe_tuner_common_if.h"
#include "eq_script_va1e1bf2401.h"
#include "sif_if.h"
#include "drvcust_if.h"  //Get Tuner/Demod I2C bus id
#include "atd_feature.h"   
//#include "pd_tuner_type.h"
//-----------------------------------------------------------------------------
// EXTERN variable
//-----------------------------------------------------------------------------
//EXTERN BOOL fgEnableATP;
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SHARP_VA1E1BF2401_BASE                            ((UINT8) 0xC2)
#define SHARP_VA1E1BF2401_STR                              "SHARP_VA1E1BF2401 2010-01-01\n 2011-04-09\n v1.0"

#define SHARP_VA1E1BF2401_IF_FREQUENCY            ((UINT16) 44000)   /* KHz */
#define SHARP_VA1E1BF2401_IF_FREQUENCY_ANA   ((UINT16) 45750)   /* KHz */
//#define SHARP_VA1E1BF2401_LO_STEP         ((UINT16) 14286)  /* 10Hz */
#define SHARP_VA1E1BF2401_ANA_LO_STEP             ((UINT16) 5000)  /* 10Hz */

#define TOSHIBA_DEMOD_BASE                  ((UINT8) 0x30)
//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//static  variable
//-----------------------------------------------------------------------------
STATIC UINT8 u1Top=0x04;   //bit[7:4]=000, Digital TOP; bit[3:0]=100, Analog TOP 

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
STATIC UINT8* pVA1E1BF2401EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pVA1E1BF2401EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

#ifdef CC_DALI2K10 
STATIC UINT8 AirChTabFreq2Ch(UINT32 u4FreqInKhz)
{
    UINT8 retValue = 0;  

    if ((u4FreqInKhz >= 54000) && (u4FreqInKhz <72000))
    {
        retValue = 2+(u4FreqInKhz -54000)/6000;
    }
    else if ((u4FreqInKhz >=76000) && (u4FreqInKhz <88000)) 
    {
        retValue = 5+(u4FreqInKhz -76000)/6000;
    }
    else if ((u4FreqInKhz >=174000) && (u4FreqInKhz <216000)) 
    {
        retValue = 7 + (u4FreqInKhz -174000)/6000;
    }
    else if ((u4FreqInKhz >=470000) && (u4FreqInKhz<806000)) 
    {
        retValue = 14 +(u4FreqInKhz -470000)/6000;
    }
    else
    {
        retValue=0;
    }
    mcSHOW_DBG_MSG4(("Freq = %dKHz => ChNum = %d\n", u4FreqInKhz, retValue)); 
    return retValue;
}

STATIC UINT8 CableChTabFreq2Ch(UINT32 u4FreqInKhz)
{
    UINT8 retValue = 0;  
    if((u4FreqInKhz >=72000) && (u4FreqInKhz <78000))
    {
       retValue = 1;
    }
    else if((u4FreqInKhz >=54000) && (u4FreqInKhz <72000))
    {
        retValue=2+(u4FreqInKhz-54000)/6000;
    }
    else if ((u4FreqInKhz >=76000) && (u4FreqInKhz <88000)) 
    {
        retValue =5+(u4FreqInKhz -76000)/6000;
    }
    else if ((u4FreqInKhz >=90000) && (u4FreqInKhz <120000)) 
    {
        retValue =95+(u4FreqInKhz -90000)/6000;
    }
    else if ((u4FreqInKhz >=120000) && (u4FreqInKhz <174000)) 
    {
        retValue =14+(u4FreqInKhz -120000)/6000;
    }
    else if ((u4FreqInKhz >=174000) && (u4FreqInKhz <216000)) 
    {
        retValue =7+(u4FreqInKhz -174000)/6000;
    }
    else if ((u4FreqInKhz >=216000) && (u4FreqInKhz <648000)) 
    {
        retValue =23+(u4FreqInKhz -216000)/6000;
    }
    else if((u4FreqInKhz >=648000) && (u4FreqInKhz <864000))
    {
        retValue=100+(u4FreqInKhz-648000)/6000;
    }
    else
    {
        retValue=0;
    }
	
    mcSHOW_DBG_MSG4(("Freq = %dKHz => ChNum = %d\n", u4FreqInKhz, retValue)); 
	
    return retValue;   
}

STATIC UINT8 u1AirPatchArr[]=
{
//             0       1        2       3       4       5      6        7       8        9 
/*0*/     0x00, 0x00, 0x17, 0x17, 0x17, 0x21, 0x13, 0x21, 0x21, 0x19, 
/*1*/	0x16, 0x16,  0x21, 0x16, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
/*2*/	0x21, 0x21,  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 
/*3*/	0x21, 0x21,  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
/*4*/	0x21, 0x21,  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 
/*5*/	0x21, 0x21,  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
/*6*/	0x21, 0x21,  0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21
};
//Spur check table v2 091217 
//CH78 0x21==>0x17
//CH79 0x10==>0x17
//CH81  0x11==>0x17
//CH104 0x16==>0x21
//CH110 0x11==>0x21
STATIC UINT8 u1CablePatchArr[]=
{
//         0         1        2       3      4        5        6       7       8       9 
/*00*/0x00,   0x21, 0x17, 0x17, 0x17, 0x21, 0x13, 0x21, 0x21, 0x19, 
/*01*/0x16,   0x16, 0x21, 0x16, 0x16, 0x21, 0x21, 0x21, 0x21, 0x16, 
/*02*/0x19,   0x19, 0x21, 0x21, 0x21, 0x23, 0x23, 0x16, 0x21, 0x15,
/*03*/0x23,   0x31, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 
/*04*/0x21,   0x21, 0x19, 0x19, 0x21, 0x21, 0x21, 0x21, 0x21, 0x23, 
/*05*/0x23,   0x21, 0x21, 0x21, 0x21, 0x21, 0x19, 0x19, 0x19, 0x17,
/*06*/0x17,   0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x17, 0x19,
/*07*/0x19,   0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x21, 0x17, 0x17, 
/*08*/0x21,   0x17, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 
/*09*/0x19,   0x19, 0x19, 0x19, 0x19, 0x19, 0x21, 0x19, 0x19, 0x19, 
/*10*/0x19,   0x19, 0x19, 0x19, 0x21, 0x19, 0x19, 0x19, 0x21, 0x19, 
/*11*/0x21,   0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x21, 0x19, 0x19,
/*12*/0x19,   0x16, 0x16, 0x19, 0x19, 0x19
};

STATIC BOOL AnaSpurPatch(ITUNER_CTX_T *pTCtx)
{
    UINT8 u1ChNum=0;
    UINT8 u1Ret=0;
    UINT8 u1RegVal;
   
    if(pTCtx->specific_member.us_ctx.u1InputSrc==MOD_ANA_CABLE)
    {
        mcSHOW_DBG_MSG4(("Cable Mode\n"));
        u1ChNum=CableChTabFreq2Ch(pTCtx->u4RF_Freq);
        if((u1ChNum<1) || (u1ChNum>=(sizeof(u1CablePatchArr)/sizeof(u1CablePatchArr[0]))))
        {
            mcSHOW_USER_MSG(("Invalid channel number %d\n",u1ChNum));
            return FALSE;
        }
        u1RegVal=u1CablePatchArr[u1ChNum];
    }
    else
    {
        mcSHOW_DBG_MSG4(("Antenna Mode\n"));
        u1ChNum=AirChTabFreq2Ch(pTCtx->u4RF_Freq);
        if((u1ChNum<2) || (u1ChNum>=(sizeof(u1AirPatchArr)/sizeof(u1AirPatchArr[0]))))
        {
            mcSHOW_USER_MSG(("Invalid channel number %d\n",u1ChNum));
            return FALSE;
        }
	u1RegVal=u1AirPatchArr[u1ChNum];
    }
	
    mcSHOW_DBG_MSG(("[%d]=0x%02X\n",u1ChNum,u1RegVal));
    u1Ret|=ICtrlBus_I2cDemodWrite(270,TOSHIBA_DEMOD_BASE,0x11,&u1RegVal,1);
	
    return ((u1Ret)?FALSE:TRUE);
}

#endif

//-----------------------------------------------------------------------------
/** VA1E1BF2401_TunerInit
 *  Tuner initialzation forVA1E1BF2401.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID VA1E1BF2401_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = SHARP_VA1E1BF2401_BASE;
    psTunerCtx->u4RF_Freq = 0;

    psTunerCtx->fgRFAGC = TRUE;      //External RF_AGC

    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = SHARP_VA1E1BF2401_IF_FREQUENCY;
    psTunerCtx->u2IF_Freq_A=SHARP_VA1E1BF2401_IF_FREQUENCY_ANA;

#ifndef ATD_SUPPORT_DRO_SYNC_MODE
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
#else  // DRO Sync mode
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI);
#endif	

  //  psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
   // psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x3D;
   // psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;  
     
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2F;     //CJ 091021//0x19;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xD5;      //CJ 091021//0xE8;//Yueqin's test result 090925 0xEF;According to YueQin's advice 090921
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script
    pVA1E1BF2401EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQSHARPVA1E1BF2401MN;
    pVA1E1BF2401EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQSHARPVA1E1BF2401MNWeak;
    //pVA1E1BF2401EqNormal[MOD_NTSC_M_54M_44M]  = EQSHARPVA1E1BF2401MN;
    //pVA1E1BF2401EqNormal[MOD_NTSC_M_54M_36M]  = EQSHARPVA1E1BF2401MN;
    psTunerSpecificCtx->u2LO_Step=SHARP_VA1E1BF2401_ANA_LO_STEP;
   
    mcSHOW_USER_MSG(("Init " SHARP_VA1E1BF2401_STR " External?%d\n",psTunerCtx->fgRFAGC));
}


//----------------------------------------------------------------------------- 
/** VA1E1BF2401_TunerSetFreq
 *  Set Tuner PLL forVA1E1BF2401/VA1E1BF2401 to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 VA1E1BF2401_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
UINT32  Lo;
UINT8   CB1,CB2,Nband;
UINT16  Ndivider;
BOOL 	 fgAnalogSignal = FALSE;
UINT8   TunerS[7], TunerDataCnt,u1Mod;
UINT32 freq;

    u1Mod=param->Modulation;
    mcSHOW_DBG_MSG((SHARP_VA1E1BF2401_STR " SetFreq %dKHz Mode=%d \n",param->Freq,u1Mod));
    if ( u1Mod>= MOD_ANA_TYPE_BEGIN )
    {  
        fgAnalogSignal = TRUE;
    }
   	
    TunerDataCnt = 7;
    freq=param->Freq;
    if(fgAnalogSignal)
    {	
        freq+=1750;//analog IF 45.75MHz
    }
    
    Lo = freq + psTunerCtx->u2IF_Freq;
    
    if(!fgAnalogSignal)
    {//digital  mode use digital LO step
        Ndivider = (UINT16) ((Lo * 7) / 1000);
    }
    else
    {
        Ndivider = (UINT16) ((Lo * 100) /SHARP_VA1E1BF2401_ANA_LO_STEP);
    }
	
    freq =freq /1000;    //convert to unit MHZ
    if (freq < 54 || freq > 864)
    {
       mcSHOW_HW_MSG(("Out of range for LO!\n"));
       return (+1);
    }
	
    /* Assign Control word for specific tuner */
    CB1 = 0x80;
    if (fgAnalogSignal)      
    {   // For NTSC
        // CB1=0xA7; //106dBuv bit3~bit5
        CB1 |= 0x07;      //R[2:0] = 111, Stepsize=50KHz
        CB1 |= ((u1Top&0x0F)<<3);//Set Top
    }
    else
    {
        // CB1=0x81;    //114dBuv bit3~bit5
        CB1 |= 0x01;        //R[2:0] = 001, Stepsize=142.86KHz
        CB1 |= ((u1Top&0xF0)>>1); //Set Top
    }
	
    CB2=0xD1;
    if (freq < 161)
    {  
        Nband = 0x01;
    }
    else if (freq < 426)
    { 
        Nband = 0x02;
    }
    else
    {
        Nband = 0x04;
    }
    
    if((freq<88) && (freq>=82))
    {
        Nband |= 0x08;     //CH6 FM trap ON
    }   

    TunerS[0]=0xFE;
    TunerS[1]= psTunerCtx->I2cAddress;//Tuner address
    TunerS[2] =(UINT8) 0x7F & (Ndivider >> 8);
    TunerS[3] =(UINT8) 0xFF & Ndivider;
    TunerS[4] = CB1;
    TunerS[5] = Nband;
    TunerS[6] = CB2;  
    mcSHOW_DBG_MSG(("Addr:0x%02X:0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n",TOSHIBA_DEMOD_BASE,TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4],TunerS[5],TunerS[6]));

     /* Write to tuner device */
    if (ICtrlBus_I2cTunerWrite(270,TOSHIBA_DEMOD_BASE, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }

    if(fgAnalogSignal)
    {
        psTunerCtx->u4RF_Freq=(Ndivider*SHARP_VA1E1BF2401_ANA_LO_STEP)/100-psTunerCtx->u2IF_Freq-1750;
    }
    else
    {
        psTunerCtx->u4RF_Freq=(Ndivider*1000)/7-psTunerCtx->u2IF_Freq;
    }
   
    mcDELAY_MS(10);
	
#ifdef CC_DALI2K10    
    if(fgAnalogSignal)
    {
        if(!AnaSpurPatch(psTunerCtx))
        {
            mcSHOW_HW_MSG((" Analog TV spur patch fail!\n"));
        }
    }
#endif

    return (0);
}

//----------------------------------------------------------------------------- 
/**  VA1E1BF2401_TunerGetStatus
 *  Set Tuner PLL staus forVA1E1BF2401/VA1E1BF2401
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL VA1E1BF2401_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    //ITUNER_CTX_T *psTunerCtx = pTCtx;
    UINT32 u4WordAddr=((UINT32)0xFE<<8) | (pTCtx->I2cAddress | 0x01);
    UINT8 ucData;
    UINT32 u4DemodBusId = SIF_BUS_TUNER_I2C; //default
	
    DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);
	
    // if (ICtrlBus_I2cTunerRead(270,psTunerCtx->I2cAddress, TunerS, 1))
    if(!SIF_X_Read((UINT8)(u4DemodBusId),0x100,TOSHIBA_DEMOD_BASE,2,u4WordAddr,&ucData,1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    if (mcTEST_BIT(ucData, 6))
    {
        return TRUE;
    }
	
    return FALSE;
}

//----------------------------------------------------------------------------- 
/** VA1E1BF2401_TunerGetStatus
 *  Set Tuner PLL staus forVA1E1BF2401/VA1E1BF2401
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *VA1E1BF2401_TunerGetVer(VOID)
{
    return (SHARP_VA1E1BF2401_STR);
}
//------------------------------------------------------- 
/** VA1E1BF2401_TunerOP
 *  Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 VA1E1BF2401_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{        
    switch(eOperation)
    {
    case itGetStatus:
        {
            BOOL * Retval=(BOOL *)pInOutVal;
            *Retval=VA1E1BF2401_TunerGetStatus(pTCtx);
        }
        break;
		
    case itGetVer:
        {
            CHAR ** RetStr=(CHAR **)pInOutVal;
            *RetStr=VA1E1BF2401_TunerGetVer();
        }
        break;
		
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pVA1E1BF2401EqNormal[SetVal];
        break;

    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pVA1E1BF2401EqWeak[SetVal];
        break;
    	
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;

    case itSetTop:
        if(SetVal&0x08)
        { //Digital TOP			
            u1Top &= 0x0F;
            u1Top |= ((SetVal & 0x07)<<4);			   
        }
        else
        { //Analog TOP
            u1Top &= 0xF0;
            u1Top |= (SetVal & 0x07);
        }
        break;

    case itGetTop:	
        if(SetVal)
        { //Digital TOP
            *(UINT8 *)pInOutVal=((u1Top & 0x70)>>4);		 
        }
        else
        { //Analog TOP
            *(UINT8 *)pInOutVal=(u1Top & 0x07);
        }
        break;

    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}

