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
 * $RCSfile: vdp_vsync.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_vsync.h
 *  Brief of file vdp_vsync.h.
 *  Details of file vdp_vsync.h (optional).
 */

#ifndef VDP_VSYNC_H
#define VDP_VSYNC_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdp_drvif.h"
#include "x_lint.h"
#include "x_typedef.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// Video Plane Event (Max 32)
#define VDP_EVENT_RECONFIG          (UINT32)(1 << 0)
#define VDP_EVENT_MASTER_CHG        (UINT32)(1 << 1)
#define VDP_EVENT_INPUT_CHG         (UINT32)(1 << 2)
#define VDP_EVENT_OUTPUT_CHG        (UINT32)(1 << 3)
#define VDP_EVENT_MODE_CHG          (UINT32)(1 << 4)
#define VDP_EVENT_SEQ_CHG           (UINT32)(1 << 5)
#define VDP_EVENT_ENABLE_CHG        (UINT32)(1 << 6)
#define VDP_EVENT_OPT_SEQ_CHG       (UINT32)(1 << 7)

// Video Call Back Message Q
typedef struct
{
    UINT32 u4Type;
    UINT32 u4Arg1;
    UINT32 u4Arg2;
    UINT32 u4Arg3;
} VDP_CB_MSG_T;

// Video Call Back Message Type
#define VDP_MSG_NOTIFY              0
#define VDP_MSG_CC_CB               1
#define VDP_MSG_AFD_CB              2
#define VDP_MSG_UNMUTE_CB           3
#define VDP_MSG_MM_CB               4
#define VDP_MSG_LIPSYNC_CB          5
#define VDP_MSG_MM_STEP_FIN_CB      6
#define VDP_MSG_MM_SEEK_FIN_CB      7
#define VDP_MSG_REPEAT_DROP_CB      8
#define VDP_MSG_LIPSYNC_OK_TO_NG_CB 9
#define VDP_MSG_LIPSYNC_NG_TO_OK_CB 10
#define VDP_MSG_GET_PIC_TYPE_CB     11

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

EXTERN void _VDP_VsyncInit(void);
EXTERN void _VDP_VsyncReset(UCHAR ucVdpId);
EXTERN void _VDP_VsyncOnOff(UCHAR ucVdpId, UINT32 u4OnOff);
EXTERN void _VDP_VsyncSendCmd(UCHAR ucVdpId, UCHAR ucCmdId);
EXTERN void _VDP_VsyncNotify(UINT32 u4Type, UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3);
EXTERN void _VDP_VsyncTick(UCHAR ucVdpId, UINT32 u4Bottom, UINT32 u4RightView,UINT32 u4IsrType);

EXTERN void _VDP_FrcSetPtsCb(UCHAR ucVdpId, UCHAR ucPort, UINT32 u4CbPts, UINT32 u4CbArg);
EXTERN void _VDP_VsyncMutexLock(void);
EXTERN void _VDP_VsyncMutexUnlock(void);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif    // VDP_VSYNC_H

