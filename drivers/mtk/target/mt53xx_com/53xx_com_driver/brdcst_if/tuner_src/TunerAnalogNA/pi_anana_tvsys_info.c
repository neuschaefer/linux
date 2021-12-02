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
 * $RCSfile: pi_anana_tvsys_info.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tvsys_info.c
 *  tv system info.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "PD_Def.h"
#include "drv_video.h"
#include "hw_vdoin.h"
#include "drv_tvd.h"
#include "pi_anana_tvsys_info.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define fcNOT_CHECK_TVD         0           // Not check TVD for Europe System

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
/***********************************************************************/
//#ifndef TUNER_SUPPORT_MULTI_SYS_TV
//#define fgIsTvdLock()           fgIsTvdVLock()   //US
//#else
//#define fgIsTvdLock()           fgHwTvdVLock()   //EU
//#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//EXTERN S16 MTTX_SetFreq(GEN_TUNER_CTX_T *pTunerCtx, U32 Freq);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
extern VOID x_thread_delay (UINT32  ui4_delay);
//----------------------------------------------------------------------------- 
/*
 *  fgDrvTunerCheckTVDLock
 *  FCheck if TVD can lock under current TV signal.
 *  @wTryLoopCnt  TVD check retry times
 *  @retval   TRUE :  TVD can lock..   
 *  @retval   FALSE:  TVD can't lock.
 */
//-----------------------------------------------------------------------------
#if !fcNOT_CHECK_TVD    // [
#if CHN_LOCK_VPRES

BOOL fgDrvTunerCheckTVDLock( UINT16 bTryLoopCnt )
{
    // update interval * max value = 2048*128 = 2^18
    // 2^18 / 27 MHz = 0.0097 sec  = about 10 ms
    // tatal: 10Times x 2ms =20ms to do safe check
    vTvd3dReset(); 
    //wTryLoopCnt = wVpresCheckTime;
    
    while(bTryLoopCnt>0)
    {
        if (fgHwTvdVPres()) // tvd vpres
        {
//    #if SUPPORT_MULTI_SYSTEM  //EU
        // don't check Vlock when setting channel, Yulia,2005.10.24
//            if(_fgAutoSearch == TRUE)
//            {
//                return fgDrvTunerCheckVLock(VLOCK_CHECK_TIME);
//            }
//            else
//            {
//                return TRUE;
//            }
//    #else
            return TRUE;    //US
//    #endif
        }
        mcDELAY_MS(2);
        bTryLoopCnt --;
    }
    //LogS("Lock fail");
    //LogS("FALSE");
    return FALSE;
}
#endif

#endif  // ]
