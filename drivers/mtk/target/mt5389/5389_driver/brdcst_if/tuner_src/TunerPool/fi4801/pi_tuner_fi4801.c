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
 * $RCSfile: pi_tuner_fi4801.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_fi4801.c
 *  Tuner control for NuTune fi4801
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "pi_demod_atd.h"
#include "fe_tuner_common_if.h"
#include "eq_script_fi4801.h"
#include "sif_if.h"
#include "drvcust_if.h"   //Get Tuner/Demod I2C bus id

//-----------------------------------------------------------------------------
// EXTERN variable
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define NUTUNE_FI4801_BASE                   ((UINT8) 0xC2)
#define NUTUNE_FI4801_STR                     "NUTUNE_FI4801 2010-01-01\n 2011-04-07\n v1.0"
#define NUTUNE_FI4801_IF_FREQ             ((UINT16) 44000)   // KHz 
#define NUTUNE_FI4801_IF_FREQ_ANA    ((UINT16) 45750)   // KHz 
#define NUTUNE_FI4801_LO_STEP             ((UINT16) 14286)  // 10Hz, Dig 
#define NUTUNE_FI4801_ANA_LO_STEP    ((UINT16) 6250)    // 10Hz, Ana 

#define DEMOD_I2C_ADDR            ((UINT8) 0x30)

//PLL lock check parameters
#define NUTUNE_FI4801_PLL_POLL_INTERVAL   10 //ms
#define NUTUNE_FI4801_PLL_POLL_TIMETOUT  100 //ms
#define NUTUNE_FI4801_PLL_POLL_CNT            NUTUNE_FI4801_PLL_POLL_TIMETOUT/NUTUNE_FI4801_PLL_POLL_INTERVAL //counter

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//static  variable
//-----------------------------------------------------------------------------
BOOL fgCable = TRUE;    // Analog signal input, 0:Air input, 1:Cable input

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------

STATIC UINT8* pFI4801EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pFI4801EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

//
/** FI4801_TunerInit
 *  Tuner initialzation forFI4801.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID FI4801_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = NUTUNE_FI4801_BASE;
    psTunerCtx->u4RF_Freq = 0;

    psTunerCtx->fgRFAGC = TRUE;  //external RF_AGC

    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = NUTUNE_FI4801_IF_FREQ;
    psTunerCtx->u2IF_Freq_A=NUTUNE_FI4801_IF_FREQ_ANA;
	
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);

    psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x3D;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;  
     
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2F;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xD5;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script
    pFI4801EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQFI4801MN;
    pFI4801EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQFI4801Weak;
   psTunerSpecificCtx->u2LO_Step=NUTUNE_FI4801_ANA_LO_STEP;
   
    mcSHOW_USER_MSG(("Init " NUTUNE_FI4801_STR "\n"));
}

//----------------------------------------------------------------------------- 
/**  FI4801_TunerGetStatus
 *  Set Tuner PLL staus for FI4801
 *  @param  pTCtx    Pointer of pi tuner driver context.
 *  @retval   TRUE     PLL lock
 *               FALSE     PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL FI4801_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    UINT32 u4WordAddr;
    UINT8 ucData;
    UINT32 u4DemodBusId;
    
    u4DemodBusId = SIF_BUS_TUNER_I2C;//default	
    DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);

    u4WordAddr=((UINT32)0xFE<<8) | (pTCtx->I2cAddress | 0x01);	
    if(!SIF_X_Read((UINT8)(u4DemodBusId), 0x100, DEMOD_I2C_ADDR, 2, u4WordAddr, &ucData, 1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    mcSHOW_DBG_MSG3(("Tuner PLL Status = 0x%2x\n", ucData));
	
    if (mcTEST_BIT(ucData, 6))
    {
        return TRUE;
    }
	
    return FALSE;
}

//----------------------------------------------------------------------------- 
/** FI4801_TunerSetFreq
 *  Set Tuner PLL forFI4801/FI4801 to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 FI4801_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
UINT32 u4LO, u4RF, u4Temp;
UINT16 Ndivider;
UINT8   CB1,CB2,CB1E;
UINT8   TunerS[7], TunerDataCnt;
UINT8   u1Mod, ii;
BOOL    fgAnalogSignal = FALSE;

    u1Mod=param->Modulation;

    mcSHOW_DBG_MSG(("%s, Freq = %d KHz, Mode=%d \n",__FUNCTION__, param->Freq,u1Mod));
	
    if ( u1Mod>= MOD_ANA_TYPE_BEGIN )
    {
        fgAnalogSignal = TRUE;
    }
   	
    TunerDataCnt = 7;
    u4RF = param->Freq/1000;   // MHz
	
    if (u4RF < 53 || u4RF > 864)
    {
        mcSHOW_HW_MSG(("Out of range for LO!\n"));
        return (+1);
    }

    if(fgAnalogSignal)
    {  // Analog signal
        u4LO = param->Freq + NUTUNE_FI4801_IF_FREQ_ANA;    
        Ndivider = (UINT16)((u4LO*100 + NUTUNE_FI4801_ANA_LO_STEP/2) / NUTUNE_FI4801_ANA_LO_STEP);
        CB1 = 0xC0;
    }
    else
    {  // ISDB-T signal 
        u4LO = param->Freq + psTunerCtx->u2IF_Freq;   
        Ndivider = (UINT16)((u4LO*100 + NUTUNE_FI4801_LO_STEP/2) / NUTUNE_FI4801_LO_STEP);
        CB1 = 0xC1;
    }
  
    u4Temp = u4LO/1000;  // (LO=RF+IF)MHz
    // Cable reception
    if(u4Temp <= 126)
    {
        CB2 = 0x41;
    }
    else if(u4Temp <= 151)
    {
        CB2 = 0x61;
    }
    else if(u4Temp <= 176)
    {
        CB2 = 0x81;
    }
    else if(u4Temp <= 194)
    {  // VHF-L
        CB2 = 0xA1;
    }
    else if(u4Temp <= 336)
    {
        CB2 = 0x42;
    }
    else if(u4Temp <= 394)
    {
        CB2 = 0x62;
    }
    else if(u4Temp <= 446)
    {
        CB2 = 0x82;
    }
    else if(u4Temp <= 464)
    {  // VHF-H
        CB2 = 0xA2;
    }
    else if(u4Temp <= 691)
    {
        CB2 = 0x64;
    }
    else if(u4Temp <= 811)
    {
        CB2 = 0x84;
    }
    else
    {
        CB2 = 0xA4;
    }
    if((!fgCable)||(u1Mod == MOD_ISDBT))
    {   // Air Input, BS4 = 1
        CB2 |= 0x08; 
    }

    CB1E = 0x82;    // T/A=0, TOP


    TunerS[0]=0xFE;
    TunerS[1]= psTunerCtx->I2cAddress;//Tuner address
    TunerS[2] =(UINT8) 0x7F & (Ndivider >> 8);
    TunerS[3] =(UINT8) 0xFF & Ndivider;
    TunerS[4] = CB1;
    TunerS[5] = CB2;
    TunerS[6] = CB1E;  

    if (ICtrlBus_I2cTunerWrite(270,DEMOD_I2C_ADDR, TunerS, TunerDataCnt))
    {  //I2C bus  connect fail
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else
    {
        //Polling PLL lock status	
        for(ii=0;ii<NUTUNE_FI4801_PLL_POLL_CNT;ii++)
        {
            mcDELAY_MS(NUTUNE_FI4801_PLL_POLL_INTERVAL);
            if(FI4801_TunerGetStatus(psTunerCtx))
            {
                break;
            }
        }
	
        if(ii<NUTUNE_FI4801_PLL_POLL_CNT)
        {
            mcSHOW_DBG_MSG(("PLL lock at %d th time\n",ii));
        }
        else
        {
            mcSHOW_HW_MSG(("PLL unlock !(%d)\n",ii));
        }
	
        if(fgAnalogSignal)
        {
            psTunerCtx->u4RF_Freq = (Ndivider*NUTUNE_FI4801_ANA_LO_STEP)/100 - NUTUNE_FI4801_IF_FREQ_ANA;
        }
        else
        {
            psTunerCtx->u4RF_Freq = (Ndivider*NUTUNE_FI4801_LO_STEP)/100 - psTunerCtx->u2IF_Freq;
        }

        mcSHOW_DBG_MSG(("Addr:[0x%02X:0x%02X-0x%02X] 0x%02X-0x%02X-0x%02X-0x%02X-0x%02X\n",DEMOD_I2C_ADDR,TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4],TunerS[5],TunerS[6]));
    }
	    
    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/** FI4801_TunerGetVer
 *  Set Tuner PLL staus for FI4801
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *FI4801_TunerGetVer(VOID)
{
    return (NUTUNE_FI4801_STR);
}

//----------------------------------------------------------------------------- 
/** FI4801_SetInMode
 *  Set Tuner for Cable or Air input mode
 *  @param  pTunerCtx       Pointer of pi tuner driver context.
 *  @param  bMode       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
STATIC void  FI4801_SetInMode(ITUNER_CTX_T * pTunerCtx, BOOL bMode)
{
    fgCable = bMode;
    mcSHOW_DBG_MSG3(("%s, mode = %d (0: Air, 1:Cable)\n", __FUNCTION__, fgCable));  
}

//------------------------------------------------------- 
/** FI4801_TunerOP
 *  Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 FI4801_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{         
    switch(eOperation)
    {
    case itGetStatus:
        {
            BOOL * Retval=(BOOL *)pInOutVal;
            *Retval=FI4801_TunerGetStatus(pTCtx);
        }
        break;
    case itGetVer:
        {
            CHAR ** RetStr=(CHAR **)pInOutVal;
            *RetStr=FI4801_TunerGetVer();
        }
        break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pFI4801EqNormal[SetVal];
        break;

    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pFI4801EqWeak[SetVal];
        break;
    	
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
		
    case itSetInCableMode: 
        FI4801_SetInMode(pTCtx,SetVal);
        break;		

    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}

