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

#ifndef _DRV_STC_H_
#define _DRV_STC_H_

#include "x_typedef.h"

/* STC ID */
#define STC_SRC_A1                          0
#define STC_SRC_A2                          1
#define STC_SRC_NS                          2

/* MT5351 STC ID */
#define STC_SRC_V1                          2
#define STC_SRC_V2                          3

/* AV SYNC Mode */
#define AV_SYNC_MODE_NONE                   0
#define AV_SYNC_MODE_AUDIO_MASTER           1
#define AV_SYNC_MODE_VIDEO_MASTER           2
#define AV_SYNC_MODE_SYSTEM_MASTER          3
#define AV_SYNC_MODE_NS                     4

/* Video SYNC Mode */
#define VID_SYNC_MODE_NONE                  0
#define VID_SYNC_MODE_SLAVE                 1
#define VID_SYNC_MODE_MASTER                2
#define VID_SYNC_MODE_SLAVE_MM              3
#define VID_SYNC_MODE_NS                    4

/* AV Sync Target */
#define AV_SYNC_TARGET_AUD                  0
#define AV_SYNC_TARGET_VDO                  1
#define AV_SYNC_TARGET_NS                   2

/* MAX Device ID */
#define MAX_DEVICE_ID                       0xFF

/* STC API Return Value */
#define STC_SET_OK                          0
#define STC_SET_FAIL                        -1
#define STC_VALID                           0
#define STC_INVALID                         -1

/* STC Time Structure */
typedef struct _STC_CLOCK
{
    UCHAR           ucBaseHi;
    UINT16          u2Ext;
    UINT32          u4Base;
} STC_CLOCK;


typedef enum
{
    STC_SPEED_TYPE_FORWARD_1X,
    STC_SPEED_TYPE_FORWARD_2X,
    STC_SPEED_TYPE_FORWARD_4X,
    STC_SPEED_TYPE_FORWARD_8X,
    STC_SPEED_TYPE_FORWARD_16X,
    STC_SPEED_TYPE_FORWARD_32X,
    STC_SPEED_TYPE_FORWARD_64X,
    STC_SPEED_TYPE_FORWARD_128X,
    STC_SPEED_TYPE_FORWARD_256X,
    STC_SPEED_TYPE_FORWARD_512X,
    STC_SPEED_TYPE_FORWARD_1024X,
    STC_SPEED_TYPE_FORWARD_1_DIV_2X,
    STC_SPEED_TYPE_FORWARD_1_DIV_4X,
    STC_SPEED_TYPE_FORWARD_1_DIV_8X,
    STC_SPEED_TYPE_FORWARD_1_DIV_16X,
    STC_SPEED_TYPE_FORWARD_1_DIV_32X,
    STC_SPEED_TYPE_FORWARD_0_DOT_6X,    /* Slow Forward 0.6X */
    STC_SPEED_TYPE_FORWARD_0_DOT_7X,    /* Slow Forward 0.7X */
    STC_SPEED_TYPE_FORWARD_0_DOT_8X,    /* Slow Forward 0.8X */
    STC_SPEED_TYPE_FORWARD_0_DOT_9X,    /* Slow Forward 0.9X */
    STC_SPEED_TYPE_FORWARD_1_DOT_1X,    /* Fast Forward 1.1X */
    STC_SPEED_TYPE_FORWARD_1_DOT_2X,    /* Fast Forward 1.2X */
    STC_SPEED_TYPE_FORWARD_1_DOT_3X,    /* Fast Forward 1.3X */
    STC_SPEED_TYPE_FORWARD_1_DOT_4X,    /* Fast Forward 1.4X */
    STC_SPEED_TYPE_FORWARD_1_DOT_5X,    /* Fast Forward 1.5X */
    STC_SPEED_TYPE_REWIND_1X,
    STC_SPEED_TYPE_REWIND_2X,
    STC_SPEED_TYPE_REWIND_4X,
    STC_SPEED_TYPE_REWIND_8X,
    STC_SPEED_TYPE_REWIND_16X,
    STC_SPEED_TYPE_REWIND_32X,
    STC_SPEED_TYPE_REWIND_64X,
    STC_SPEED_TYPE_REWIND_128X,
    STC_SPEED_TYPE_REWIND_256X,
    STC_SPEED_TYPE_REWIND_512X,
    STC_SPEED_TYPE_REWIND_1024X,
    STC_SPEED_TYPE_NS
}STC_SPEED_TYPE_T;


/******************************************************************************
* STC API
******************************************************************************/
extern INT32 DMX_SetStcSrc(UCHAR ucPidIdx, UCHAR ucStcId);
extern INT32 STC_GetSrc(UCHAR ucStcId, STC_CLOCK* prStc);
extern INT32 STC_SetSrc(UCHAR ucStcId, STC_CLOCK* prStc);

extern INT32 STC_SetSyncTarget(UCHAR ucType, UCHAR ucStcId, UCHAR ucTargetId);
extern UCHAR STC_GetSyncTarget(UCHAR ucType, UCHAR ucStcId);
extern INT32 STC_SetStartPts(UCHAR ucType, UCHAR ucStcId, UINT32 u4Pts);

extern INT32 STC_SetPtsDrift(UCHAR ucStcId, INT32 i4Drift);
extern INT32 STC_GetPtsDrift(UCHAR ucStcId);

extern INT32 STC_GetPtsDriftByAudDeviceId(UCHAR ucDevId);
extern INT32 STC_GetPtsDriftByVdoDeviceId(UCHAR ucDevId);

extern void STC_RxEsPkt(UCHAR ucType, UCHAR ucDevId, UINT32 u4Pts);
extern UINT32 STC_GetFastChangeChangeAhead(UCHAR ucStcId);

extern void STC_SetStcValue(UINT32 u4Stc);
extern UINT32 STC_GetStcValue(void);

extern void STC_StopStc(void);
extern void STC_StartStc(void);

extern void STC_SetSyncMode(UINT32 u4SyncMode);

#define STC_SetAudDeviceId(x, y)        ((void)0)
#define STC_SetVdoDeviceId(x, y)        ((void)0)
#define STC_GetVdoDeviceId(x)           STC_GetSyncTarget(AV_SYNC_TARGET_VDO, x)

#ifdef CC_GET_PCR_PTS_INFO
EXTERN void STC_GetPcr(STC_CLOCK* prPcr);
EXTERN void STC_GetPcrStcDelta(INT64* pi8Delta);
EXTERN void STC_GetAudioPts(STC_CLOCK* prAudioPts);
EXTERN void STC_GetAudioPtsStcDelta(INT64* pi8Delta);
EXTERN BOOL STC_GetAudioSync(void);
EXTERN void STC_GetVideoPts(STC_CLOCK* prVideoPts);
EXTERN void STC_GetVideoPtsStcDelta(INT64* pi8Delta);
EXTERN BOOL STC_GetVideoSync(void);
#endif // CC_GET_PCR_PTS_INFO

#endif /* _DRV_STC_H_ */

