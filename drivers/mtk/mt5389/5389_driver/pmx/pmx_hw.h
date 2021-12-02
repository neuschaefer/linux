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
 * $RCSfile: pmx_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pmx_hw.h
 *  Brief of file pmx_hw.h.
 *  Details of file pmx_hw.h (optional).
 */

#ifndef PMX_HW_H
#define PMX_HW_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
//LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_hal_5381.h"
//LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define REG_OS_ALPHA_MIX1 0x5c0
#define REG_OS_ALPHA_MIX2 0x5c4
#define REG_OS_BGCOLOR 0x5c8
    #define REG_OS_BGCOLOR_MSK (0xffffff)
#define REG_OS_PATTERN 0x4a4
    #define REG_OS_FASTBLANK_MSK (1 << 2)
    #define REG_OS_PATTERN_MSK (1 << 3)
#define REG_NUM 2
#define PMX_PLANE_NUMBER 5

typedef enum _PMX_MIX_AL_VIDEO_T
{
    PMX_MIX_AL_VIDEO_BOT,
    PMX_MIX_AL_VIDEO_1_OSD_BELOW,
    PMX_MIX_AL_VIDEO_2_OSD_BELOW,
    PMX_MIX_AL_VIDEO_TOP,
    PMX_MIX_AL_VIDEO_MAX,
}PMX_MIX_AL_VIDEO_T;

typedef enum _PMX_OSD_MIX_AL_T
{
    OSD_MIX_AL_11,
    OSD_MIX_AL_12,
    OSD_MIX_AL_2,
    OSD_MIX_AL_MAX,
}PMX_OSD_MIX_AL_T;


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

EXTERN void _PMX_Init(void);
EXTERN void _PMX_SetPlaneOrder(UCHAR ucPlaneId, UCHAR ucPlaneOrder);
EXTERN void _PMX_SetPlaneOrderArray(const UINT32* pu4PlaneOrder);
EXTERN void _PMX_SetAlpha(UCHAR ucPlaneId, UCHAR ucValue);
EXTERN void _PMX_SetBgColor(UINT32 u4Value, BOOL fgYuvMode, BOOL fgInVsync);
EXTERN void _PMX_Set_ALPHA_MIX_GRP1_0(BOOL fgEnable);

EXTERN void _PMX_Set_ALPHA_MIX_VIDEO_BOT(const UINT32* pu4Alpha);
EXTERN void _PMX_Set_ALPHA_MIX_VIDEO_1_OSD_BELOW(const UINT32* pu4Alpha);
EXTERN void _PMX_Set_ALPHA_MIX_VIDEO_2_OSD_BELOW(const UINT32* pu4Alpha);
EXTERN void _PMX_Set_ALPHA_MIX_VIDEO_TOP(const UINT32* pu4Alpha);

#endif // PMX_HW_H
