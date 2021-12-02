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
 * $RCSfile: pi_tuner_va4a1fb5010.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_va4a1fb5010.c
 *  Tuner control for sharp va4a1fb5010
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "pi_demod_atd.h"
#include "fe_tuner_common_if.h"
#include "sif_if.h"
#include "drvcust_if.h"   //Get Tuner/Demod I2C bus id
#include "eq_script_va4a1fb5010.h"
#include "pi_tuner_va4a1fb5010.h"

//-----------------------------------------------------------------------------
// EXTERN variable
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SHARP_VA4A1FB5010_BASE                   ((UINT8) 0xC2)
#define SHARP_VA4A1FB5010_STR                     "SHARP_VA4A1FB5010 2010-01-01\n 2011-04-09\n v1.0"
#define SHARP_VA4A1FB5010_IF_FREQ             ((UINT16) 4000)   // KHz 
#define SHARP_VA4A1FB5010_IF_FREQ_ANA_MN    ((UINT16) 5750)   // KHz 
#define SHARP_VA4A1FB5010_ANA_LO_STEP    ((UINT16) 5000)    // 10Hz 



//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//static  variable
//-----------------------------------------------------------------------------
//UINT8 u1PreMode = 0xFF;

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------

STATIC UINT8* pVA4A1FB5010EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pVA4A1FB5010EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

//
/** VA4A1FB5010_TunerInit
 *  Tuner initialzation forVA4A1FB5010.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID VA4A1FB5010_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = SHARP_VA4A1FB5010_BASE;
    psTunerCtx->u4RF_Freq = 0;

    psTunerCtx->fgRFAGC = TRUE;  //external RF_AGC

    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = SHARP_VA4A1FB5010_IF_FREQ;
    psTunerCtx->u2IF_Freq_A=SHARP_VA4A1FB5010_IF_FREQ_ANA_MN;
	
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);

    psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;  
     
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2F;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xD5;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script
    pVA4A1FB5010EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQVA4A1FB5010MN;
    pVA4A1FB5010EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQVA4A1FB5010Weak;
   psTunerSpecificCtx->u2LO_Step=SHARP_VA4A1FB5010_ANA_LO_STEP;

   Tuner_HW_Reset();   // Tuner HardWare Reset
   
   if(Tuner_RegInit())
   {
       mcSHOW_USER_MSG(("Tuner Reg Init Fail !\n"));   
   }
   
    mcSHOW_USER_MSG(("Init " SHARP_VA4A1FB5010_STR "\n"));
}

//----------------------------------------------------------------------------- 
/**  VA4A1FB5010_TunerGetStatus
 *  Set Tuner PLL staus for VA4A1FB5010
 *  @param  pTCtx    Pointer of pi tuner driver context.
 *  @retval   TRUE     PLL lock
 *               FALSE     PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL VA4A1FB5010_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    Tuner_RegDump();	
    return TRUE;
}


//----------------------------------------------------------------------------- 
/** VA4A1FB5010_TunerSetFreq
 *  Set Tuner PLL forVA4A1FB5010/VA4A1FB5010 to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 VA4A1FB5010_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
UINT32 u4RF;
UINT8   u1Mode;
BOOL    fgAnalogSignal = FALSE;

    u1Mode=param->Modulation;

    mcSHOW_DBG_MSG(("%s, Freq = %d KHz, Mode=%d \n",__FUNCTION__, param->Freq,u1Mode));

    if(Tuner_RegInit())
    {
        mcSHOW_USER_MSG(("Tuner Reg Init Fail !\n"));	
    }
	
    u4RF = param->Freq/1000; //MHz	
    if (u4RF < 53 || u4RF > 865)
    {
        mcSHOW_HW_MSG(("Out of range for LO!\n"));
        return (+1);
    }


    mcSHOW_DBG_MSG(("Set the Specific Freq\n"));    
    switch(u1Mode)
    {
    case MOD_ATSC_8VSB:
        if((u4RF>548)&&(u4RF < 554))
        {
            API_SET_ATSC_551MHz();
        }
        else
        {
            API_SET_ATSC_183MHz();
        }
        break;		
    case MOD_J83B_64QAM:
    case MOD_J83B_256QAM:
    case MOD_J83B_AUTO_QAM:
        if((u4RF>108)&&(u4RF < 114))
        {
            API_SET_ATSC_Cable_111MHz();
        }
        else
        {
            API_SET_ATSC_Cable_183MHz();
        }
        break;	
	case MOD_DVBC:
		API_SET_DVBC_666MHz( );
		 psTunerCtx->u2IF_Freq = 5000;
		 break;

	case MOD_DVBT:	
		API_SET_DVBT_666MHz( );
		 psTunerCtx->u2IF_Freq = 5000;
		 break;
    case MOD_NTSC_M:
    default:
            API_SET_NTSC_181_25MHz();
            break;
    }

    psTunerCtx->u4RF_Freq = param->Freq;    // Update the RF Freq
	
    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/** VA4A1FB5010_TunerGetVer
 *  Set Tuner PLL staus for VA4A1FB5010
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *VA4A1FB5010_TunerGetVer(VOID)
{
    return (SHARP_VA4A1FB5010_STR);
}


//------------------------------------------------------- 
/** VA4A1FB5010_TunerOP
 *  Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 VA4A1FB5010_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{         
    switch(eOperation)
    {
    case itGetStatus:
        {
            VA4A1FB5010_TunerGetStatus(pTCtx);
        }
        break;
    case itGetVer:
        {
        CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=VA4A1FB5010_TunerGetVer();
        }
        break;
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pVA4A1FB5010EqNormal[SetVal];
        break;

    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pVA4A1FB5010EqWeak[SetVal];
        break;
    	
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
		
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}


