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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: cb_nptv.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file cb_nptv.c
 *  This file implements callback interface of mpv for resolution change.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "cb_low.h"
#include "nptv_if.h"
#include "video_timing.h"
#include <linux/module.h>

#if 0
typedef void (*NPTV_PFN_SIG_NFY_FCT)(
    void*               pvNfyTag,
    UINT32     		u4SigStatus,
    UINT32              u4SigSrc
);

typedef struct _NPTV_SIG_NFY_INFO_T_
{
    void*                        		pvTag;        /* pull mode only */
    NPTV_PFN_SIG_NFY_FCT   		pfSigNfy; /* pull mode only */

}NPTV_SIG_NFY_INFO_T;
#endif

EXTERN UINT16 wDrvVideoInputWidth(UINT8 bPath);
EXTERN UINT16 wDrvVideoInputHeight(UINT8 bPath);
EXTERN UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifdef CC_DRIVER_PROGRAM
//-----------------------------------------------------------------------------
/** NPTV callback function.
 *  @param  pvNfyTag
 *  @param  u4SigStatus 
 *  @param  u4SigSrc     
 */
//-----------------------------------------------------------------------------

static void _NPTVCbFct(void* pvNfyTag, UINT32 u4SigStatus, UINT32 u4SigSrc)
{
    UINT32 u4VdpId = (UINT32)pvNfyTag;
    NPTV_CB_T rNPTVCb;

    rNPTVCb.ucVdpId = (UINT8)pvNfyTag;
    rNPTVCb.u4SigStatus = u4SigStatus;
    if(fgIsOversampleTiming())
    {
        rNPTVCb.u2W = wDrvVideoInputWidth(u4VdpId) / 2;
    }
    else
    {
        rNPTVCb.u2W = wDrvVideoInputWidth(u4VdpId);
    }
    rNPTVCb.u2H = wDrvVideoInputHeight(u4VdpId);
    rNPTVCb.ucProgressive = !(unsigned char)bDrvVideoIsSrcInterlace(u4VdpId);
         
    _CB_PutEvent(CB_NPTV_TRIGGER, sizeof(NPTV_CB_T), &rNPTVCb);

}
#endif /* CC_DRIVER_PROGRAM */

//-----------------------------------------------------------------------------
/** To register/enable NPTV callback function.
 *  @param  i4SubId     ID to identify subcomponent, or other parameter.
 *  @retval CBR_OK      Success.
 */
//-----------------------------------------------------------------------------

INT32 _NPTV_RegTrigger(INT32 i4SubId)
{
#ifdef CC_DRIVER_PROGRAM
    int idx;
    NPTV_PFN_SIG_NFY_FCT pfFct;

    pfFct = _NPTVCbFct;

    NPTV_SetSigChgNfyFct(pfFct);
#endif
    return CBR_OK;
}
