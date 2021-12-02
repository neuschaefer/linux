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
 * $RCSfile: tuner_interface.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tuner_interface.c
 *  Implement primitive tuner driver interface.
 */

#include "tuner_interface_if.h"
#include "PI_Def_dbg.h"
#include "ctrl_bus.h"

//-----------------------------------------------------------------------------
// PD related stuffs
//-----------------------------------------------------------------------------
//! unique global PI tuner context
static ITUNER_CTX_T piTunerCtx;
static ITUNER_FCT_T piTunerFct = {
    .Init = NULL,
    .SetFreq    = NULL,
    .OP         =NULL
};

//! Primitive tuner function definitions
ITUNER_CTX_T *ITunerGetCtx(VOID)
{
    return &piTunerCtx;
}

BOOL IPhysicalTunerRegistration(ITUNER_FCT_T* tunerFct)
{
    if (!tunerFct)
    {
        mcSHOW_USER_MSG(("[fatal] Null tuner registration! (%s)\n", __FILE__));
        return FALSE;
    }
    
    x_memcpy(&piTunerFct, tunerFct, sizeof(ITUNER_FCT_T));
    return TRUE;
}

static VOID InitializeTunerCtx(ITUNER_CTX_T *pTCtx)
{
	if (pTCtx)
	{	
        x_memset(pTCtx, 0, sizeof(ITUNER_CTX_T));
    }
	else
	{
         mcSHOW_DRVERR_MSG(("Initialize TunerCtx is NULL!\n"));
	}
}

VOID ITuner_Init(
    ITUNER_CTX_T *pTCtx
    )
{
    I2cPinmuxInit();
    InitializeTunerCtx(pTCtx);
   
    if (!piTunerFct.Init)
    {
        mcSHOW_DRVERR_MSG(("ITuner_Init() is NULL!\n"));
    }
    else
        piTunerFct.Init(pTCtx);
}
INT16 ITuner_SetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T * param
    )
{
    if (!piTunerFct.SetFreq)
    {
        mcSHOW_DRVERR_MSG(("ITuner_SetFreq() is NULL!\n"));
        return (-1);
    }
    else
        return piTunerFct.SetFreq(pTCtx,param);
}
INT16 ITuner_OP (ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{
    INT16 ret;
      if (!piTunerFct.OP)
    {
        mcSHOW_DRVERR_MSG(("ITuner_OP() is NULL!\n"));
        return (-1);
    }
    else{
         ret=piTunerFct.OP(pTCtx,eOperation,SetVal,pInOutVal);
         if(ITUNER_NOT_SUPPORTED==ret)//d110327_Haibo:Do not show "ITuner_OP fail" when it is just not support to avoid confuse
         {
         	mcSHOW_DBG_MSG3(("ITuner_OP(eOperation=%d) Not Support\n",eOperation));
         }
		 else if(ITUNER_NOT_OK==ret)
		 {
		    mcSHOW_DRVERR_MSG(("ITuner_OP(eOperation=%d) FAIL! \n",eOperation));
		 }
         return ret;
    }

}

