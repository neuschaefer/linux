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
 * $RCSfile: pi_tuner_TEQH3L01A.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_TEQH3L01A.c
 *  Tuner control for SS TEQH3L01A
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "fe_tuner_common_if.h"
#include "pi_demod_atd.h"

#include "eq_script_teqh3l01a_5388_sun.h"
#include "eq_script_teqh3l01a_5388.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define ALPS_TEQH3L01A_BASE            ((UINT16) 0xC2)
#define ALPS_TEQH3L01A_IF_FREQUENCY    ((UINT16) 44000)   /* KHz */
#define ALPS_TEQH3L01A_ANA_LO_STEP         ((UINT16) 31250)  /* Hz */
#define ALPS_TEQH3L01A_STR             "ALPS_TEQH3L01A 2010-01-01\n 2011-04-09\n v1.0"

STATIC UINT8* pTeqh3l01aEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pTeqh3l01aEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//
/** TEQH3L01A_TunerInit
 *  Tuner initialzation forTEQH3L01A.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void TEQH3L01A_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = ALPS_TEQH3L01A_BASE;
    psTunerCtx->u4RF_Freq = 0;

    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = ALPS_TEQH3L01A_IF_FREQUENCY;
    
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS|MASK_PATCH_SBEAT);
    psTunerCtx->fgLnaOn = FALSE;
    psTunerCtx->fgRFAGC = TRUE;
    psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00; 
    psTunerSpecificCtx->u2LO_Step=ALPS_TEQH3L01A_ANA_LO_STEP;
    
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2A;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xE0;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script
    pTeqh3l01aEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTEQH3L01AMN;
    pTeqh3l01aEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTEQH3L01AMNWeak;

    mcSHOW_USER_MSG(("Init " ALPS_TEQH3L01A_STR " for mtk board\n"));
}

//-----------------------------------------------------------------------------
//
/** TEQH3L01A_SunBoard_TunerInit
 *  Tuner initialzation forTEQH3L01A (for sun board).
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void TEQH3L01A_SunBoard_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = ALPS_TEQH3L01A_BASE;
    psTunerCtx->u4RF_Freq = 0;

    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = ALPS_TEQH3L01A_IF_FREQUENCY;
    
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS|MASK_PATCH_SBEAT);
    psTunerCtx->fgLnaOn = FALSE;
    psTunerCtx->fgRFAGC = TRUE;
    psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00; 
    psTunerSpecificCtx->u2LO_Step=ALPS_TEQH3L01A_ANA_LO_STEP;

    // AGC para for sun board
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2A;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xE0; //0xDF;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; //0x03;

    // init EQ script for sun board
    pTeqh3l01aEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTEQH3L01AMN_SunBoard;
    pTeqh3l01aEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQTEQH3L01AMNWeak_SunBoard;

    mcSHOW_USER_MSG(("Init " ALPS_TEQH3L01A_STR " for sun board\n"));
}


//----------------------------------------------------------------------------- 
/** TEQH3L01A_TunerSetFreq
 *  Set Tuner PLL forTEQH3L01A/TEQH3L01A to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 TEQH3L01A_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
UINT32  Lo;
UINT8   CB,BB;
UINT16  Ndivider;
UINT8   TunerS[4], TunerDataCnt,u1Mod;
UINT32 freq=param->Freq;
BOOL 	 fgAnalogSignal = FALSE;

    mcSHOW_DBG_MSG(("ALPS TEQH3L01A_TunerSetFreq %dKHz\n", freq));
    
    u1Mod=param->Modulation;
    if ( u1Mod>= MOD_ANA_TYPE_BEGIN )
        fgAnalogSignal = TRUE;

    if(fgAnalogSignal) freq += 1750;//analog IF 45.75MHz

    TunerDataCnt = 4;
    
    Lo = freq + psTunerCtx->u2IF_Freq;

    Ndivider = (UINT16) ((Lo*1000 + ALPS_TEQH3L01A_ANA_LO_STEP/2) /ALPS_TEQH3L01A_ANA_LO_STEP);
   
     Lo =Lo /1000;//convert to unit MHZ
     
     mcSHOW_DBG_MSG(("Lo=%dMHz\n",Lo));
     
/* Assign Control word for specific tuner */
    //if (Lo<98 || Lo > 850)//Freq Range 101MHz~847MHz(LO)
    if (Lo<97 || Lo > 908)
    {
        return (+1);
    }
//  LNA=1(OFF),Resolution:31.25KHz LNA Bit 3
    if (pTCtx->fgLnaOn) {
        CB=0xCA;
    }
    else {
        CB=0xC2;
    }

    if(Lo<=176){
       BB=0x01;  
    }
    else if(Lo<=410){
    //else if(Lo<=404){	
      BB=0x02;
    }
    else{
      BB=0x08;
    }
    
    /* LO settings */
    TunerS[0] = (UINT8) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UINT8) 0xFF & Ndivider;
    TunerS[2] = (UINT8) CB;               
    TunerS[3] = (UINT8) BB;
    
 /* Write to tuner device */
    if (ICtrlBus_I2cTunerWrite(270,psTunerCtx->I2cAddress, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else//I2C bus  connect
    {
       if( fgAnalogSignal )
           psTunerCtx->u4RF_Freq=(Ndivider*ALPS_TEQH3L01A_ANA_LO_STEP)/1000-psTunerCtx->u2IF_Freq-1750;
	else
	    psTunerCtx->u4RF_Freq=(Ndivider*ALPS_TEQH3L01A_ANA_LO_STEP)/1000-psTunerCtx->u2IF_Freq;
       
       mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X(%dKHz)\n",psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3],psTunerCtx->u4RF_Freq));
    }

    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/**  TEQH3L01A_TunerGetStatus
 *  Set Tuner PLL staus forTEQH3L01A/TEQH3L01A
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL TEQH3L01A_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;

    UINT8 TunerS[4];

    if (ICtrlBus_I2cTunerRead(270,psTunerCtx->I2cAddress, TunerS, 1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    if (mcTEST_BIT(TunerS[0], 6))
        return TRUE;
    return FALSE;
}

//----------------------------------------------------------------------------- 
/** TEQH3L01A_TunerGetStatus
 *  Set Tuner PLL staus forTEQH3L01A/TEQH3L01A
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *TEQH3L01A_TunerGetVer(VOID)
{
    return (ALPS_TEQH3L01A_STR);
}
/***********************************************************************/
//------------------------------------------------------------------------------------------------------------------------------------- 
/** TEQH3L01A_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TEQH3L01A_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation){
    case itGetStatus:
        {
            BOOL * Retval=(BOOL *)pInOutVal;
            *Retval=TEQH3L01A_TunerGetStatus(pTCtx);
        }
        break;
    case itGetVer:
        {
            CHAR ** RetStr=(CHAR **)pInOutVal;
            *RetStr=TEQH3L01A_TunerGetVer();
        }
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pTeqh3l01aEqNormal[SetVal];
        break;
    
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pTeqh3l01aEqWeak[SetVal];
        break;
         
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
    case itSetLNA:
        pTCtx->fgLnaOn = SetVal;
        break;
    case itGetLNA :
    {
        BOOL * Retval=(BOOL *)pInOutVal;
        *Retval  = pTCtx->fgLnaOn;	
    }
        break;    
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
