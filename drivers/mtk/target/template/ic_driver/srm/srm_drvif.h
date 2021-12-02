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
 * $RCSfile: srm_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sm_if.h
 *  Brief of file sm_if.h.
 *  Details of file sm_if.h (optional).
 */

#ifndef SRM_IF_H
#define SRM_IF_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef void (*FN_SRN_CALLBACK) (UINT32, UINT32, UINT32, UINT32);

// Module 
#define SRM_DRV_NS                      6   // MDDI, SCPOS, NR, MPEG, DRAM, OSD

#define SRM_DRV_MDDI                    0
#define SRM_DRV_SCPOS                   1
#define SRM_DRV_NR                      2
#define SRM_DRV_MPEG                    3
#define SRM_DRV_DRAM                    4
#define SRM_DRV_OSD                     5

#define SRM_DRV_AUD                     6
#define SRM_DRV_DMX                     7
#define SRM_DRV_USB                     8
#define SRM_DRV_B2R                     9
#define SRM_DRV_CPU                     10
#define SRM_DRV_GFX                     11
#define SRM_DRV_JPEG                    12
#define SRM_DRV_PSR                     13
#define SRM_DRV_DSP                     14
#define SRM_DRV_TCM                     15
#define SRM_DRV_IDETEST                 16
#define SRM_DRV_UART                    17
#define SRM_DRV_FCI                     18
#define SRM_DRV_MCBAK                   19
#define SRM_DRV_USB2                    20
#define SRM_DRV_TOTAL                   21

// VGA Bandwidth Issue
#define SRM_VGA_BANDWIDTH_OK            0       // Normal Mode
#define SRM_VGA_BANDWIDTH_FAIL          1       // Display Mode

// SRM Control Feature
#define SRM_BOB_CTRL                    0       // BOB Mode Control
#define SRM_NR_CTRL                     1       // NR Mode Control
#define SRM_DISP_CTRL                   2       // Display Mode Control
#define SRM_MUTE_CTRL                   3       // Mute Control
#define SRM_SCALER_WE_CTRL              4       // Scaler Write Enable Control 
#define SRM_MPEG_PIP_CTRL               5       // Deblocking/Half Reference Control
#define SRM_MISC_CTRL                   6       // Misc (Scaler change notify, ...)
#define SRM_FEATURE_CTRL_NS             7

// Operating Mode
#define SRM_MDDI_MODE_FULL              0       // MDDI Full Function
#define SRM_MDDI_MODE_LIGHT             1       // MDDI Light Function
#define SRM_MDDI_MODE_BOB               2       // MDDI BOB
#define SRM_MDDI_MODE_MAX               SRM_MDDI_MODE_BOB

#define SRM_SCPOS_MODE_FULL             0       // Display Mode Off
#define SRM_SCPOS_MODE_OFF              1       // Display Mode On
#define SRM_SCPOS_MODE_MAX              SRM_SCPOS_MODE_OFF

#define SRM_SCPOS_MUTE_OFF              0       // Scaler Mute Off
#define SRM_SCPOS_MUTE_ON               1       // Scaler Mute On
#define SRM_SCPOS_MUTE_MAX              SRM_SCPOS_MUTE_ON

#define SRM_SCPOS_WE_ON                 0       // Normal
#define SRM_SCPOS_WE_OFF                1       // Force Write Enable Off
#define SRM_SCPOS_WE_MAX                SRM_SCPOS_WE_OFF

#define SRM_NR_MODE_3D                  0       // NR 3D Mode (YC Mode)
#define SRM_NR_MODE_2D                  1       // NR 2D Mode
#define SRM_NR_MODE_3D_Y                2       // NR 3D-Y Mode
#define SRM_NR_MODE_MAX                 SRM_NR_MODE_3D_Y

#define SRM_NR_BIT_IGNORE               0       // OLD versino
#define SRM_NR_BIT_9                    1       // 9 bit
#define SRM_NR_BIT_10                   2       // 10 bit

#define SRM_MPEG_MODE_FULL              0       // MPEG Full Function
#define SRM_MPEG_MODE_HALF              1       // MPEG Half Reference
#define SRM_MPEG_MODE_MAX               SRM_MPEG_MODE_HALF

#define SRM_MISC_NULL                   0
#define SRM_MISC_SCALER_CHANGE          1       // Scaler change notify
#define SRM_MISC_MAX                    SRM_MISC_SCALER_CHANGE

// Information Event
#define SRM_SCPOS_EVENT_MASK            (0xFFFF)

#define SRM_SCPOS_EVENT_ONOFF           (1 << 16)
#define SRM_SCPOS_EVENT_SOURCE          (1 << 17)
#define SRM_SCPOS_EVENT_MPEG_SIZE       (1 << 18)
#define SRM_SCPOS_EVENT_SIZE            (1 << 19)
#define SRM_SCPOS_EVENT_OUT_SIZE        (1 << 20)
#define SRM_SCPOS_EVENT_STATUS          (1 << 21)
#define SRM_SCPOS_EVENT_UNMUTE          (1 << 22)
#define SRM_SCPOS_EVENT_UNSTABLE        (1 << 23)

#define SRM_OSD_EVENT_ONOFF             (1 << 16)
#define SRM_OSD_EVENT_SIZE              (1 << 17)

typedef enum
{
    SRM_TV_MODE_TYPE_NORMAL = 0,
    SRM_TV_MODE_TYPE_PIP,
    SRM_TV_MODE_TYPE_POP,
    SRM_TV_MODE_TYPE_LAST_VALID_ENTRY      /* only for counting purpose */
} SRM_TV_MODE_T;

typedef enum
{
    SRM_FREEZE_MODE_SCPOS = 0,
    SRM_FREEZE_MODE_MDDI,
    SRM_FREEZE_MODE_MPEG,
    SRM_FREEZE_MODE_LAST_VALID_ENTRY      /* only for counting purpose */
} SRM_FREEZE_MODE_T;

/** Brief of SRM_VDP_INFO_T
 */
typedef struct 
{
    UINT32 u4Enable;
    UINT32 u4Status;
    UINT32 u4ColorFormat444;
    UINT32 u4VgaTiming;
    UINT32 u4Source;
    UINT32 u4SourceId;
    UINT32 u4MpegResolution;
    UINT32 u4Resolution;
    UINT32 u4Interlace;
    UINT32 u4OutputWidth;
    UINT32 u4OutputHeight;
    UINT32 u4InputFrameRate;

    UINT32 u4NoSignal;
} SRM_VDP_INFO_T;


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

EXTERN void SRM_Init(void);

EXTERN void SRM_Pause(void);

EXTERN void SRM_Resume(void);

EXTERN void SRM_SendEvent(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

EXTERN void SRM_RegisterCB(UINT32 u4Mod, FN_SRN_CALLBACK pfCbFun);

EXTERN UINT32 SRM_DramMonitor(UINT32 u4Mod, UINT32 u4DelayClock, UINT32 u4MonitorDelay);

EXTERN SRM_TV_MODE_T SRM_GetTvMode(void);

EXTERN void SRM_SetTvMode(SRM_TV_MODE_T eMode);

EXTERN SRM_FREEZE_MODE_T SRM_GetFreezeMode(UINT32 u4VdpId);

EXTERN SRM_VDP_INFO_T* SRM_GetVdpInfo(UINT32 u4VdpId);

EXTERN UINT32 SRM_CheckVGABandwidth(UINT32 u4ActiveWidth, UINT32 u4ActiveHeight, UINT32 u4ReflashRate);

EXTERN void SRM_QueryStatus(void);

#endif    // SRM_IF_H

