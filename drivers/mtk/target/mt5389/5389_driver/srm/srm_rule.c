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
 * $Author: pan.huang $
 * $Date: 2012/10/15 $
 * $RCSfile: srm_rule.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file srm_rule.c
 *  Brief of file srm_rule.c
 *  Details of file srm_rule.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "srm_drvif.h"
#include "fbm_drvif.h"
#include "vdp_drvif.h"
#include "srm_debug.h"
#include "panel.h"
#include "drvcust_if.h"
#include "video_def.h"
#include "x_assert.h"

#ifdef CC_SCPOS_EN
#include "source_table.h"  // nptv
#endif

#include "x_hal_5381.h"

#if defined(CC_SCPOS_3DTV_SUPPORT) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#include "vdo_misc.h"
#include "drv_tdtv.h"
#include "drv_tdtv_drvif.h"
#endif

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SRM_DISP_MIN_FRAME_RATE 48

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define SRM_CHECK_INFO                                                         \
    do                                                                         \
    {                                                                          \
        if ((prSrmVdpInfo == NULL) || (prSrmVdpCtrl == NULL))                  \
        {                                                                      \
            return 0;                                                          \
        }                                                                      \
    }                                                                          \
    while (0)


#define SRM_UPDATE_BOB(vdp, mode, arg1, arg2, arg3)                            \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_BOB_CTRL] < mode))                   \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_BOB_CTRL] = mode;                     \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg1 = arg1;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg2 = arg2;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg3 = arg3;                 \
        LOG(6, "[SRM] BOB DB vdp%d mode%d 0x%x %d %d\n"                        \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         \

#if 0
#define SRM_UPDATE_NR(vdp, mode, arg1, arg2, arg3)                             \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_NR_CTRL] < mode))                    \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_NR_CTRL] = mode;                      \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_NR_CTRL].u4Arg1 = arg1;                  \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_NR_CTRL].u4Arg2 = arg2;                  \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_NR_CTRL].u4Arg3 = arg3;                  \
        LOG(6, "[SRM] NR DB vdp%d mode%d 0x%x %d %d\n"                         \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#define SRM_UPDATE_DISP(vdp, mode, arg1, arg2, arg3)                           \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_DISP_CTRL] < mode))                  \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_DISP_CTRL] = mode;                    \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg1 = arg1;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg2 = arg2;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg3 = arg3;                \
        LOG(6, "[SRM] DISP DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         \

#define SRM_UPDATE_TDC(vdp, mode, arg1, arg2, arg3)                           \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_TDC_CTRL] < mode))                  \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_TDC_CTRL] = mode;                    \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_TDC_CTRL].u4Arg1 = arg1;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_TDC_CTRL].u4Arg2 = arg2;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_TDC_CTRL].u4Arg3 = arg3;                \
        LOG(6, "[SRM] TDC DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }    
	
#if 0
#define SRM_UPDATE_MUTE(vdp, mode, arg1, arg2, arg3)                           \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MUTE_CTRL] < mode))                  \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MUTE_CTRL] = mode;                    \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MUTE_CTRL].u4Arg1 = arg1;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MUTE_CTRL].u4Arg2 = arg2;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MUTE_CTRL].u4Arg3 = arg3;                \
        LOG(6, "[SRM] MUTE DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#if 0
#define SRM_UPDATE_SCALER_WE(vdp, mode, arg1, arg2, arg3)                      \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_SCALER_WE_CTRL] < mode))             \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_SCALER_WE_CTRL] = mode;               \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_SCALER_WE_CTRL].u4Arg1 = arg1;           \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_SCALER_WE_CTRL].u4Arg2 = arg2;           \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_SCALER_WE_CTRL].u4Arg3 = arg3;           \
        LOG(6, "[SRM] WE DB vdp%d mode%d 0x%x %d %d\n"                         \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#if 0
#define SRM_UPDATE_MPEG_PIP(vdp, mode, arg1, arg2, arg3)                       \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MPEG_PIP_CTRL] < mode))              \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MPEG_PIP_CTRL] = mode;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg1 = arg1;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg2 = arg2;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg3 = arg3;            \
        LOG(6, "[SRM] MPEG DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#if 0
#define SRM_UPDATE_MISC_CTRL(vdp, mode, arg1, arg2, arg3)                      \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MISC_CTRL] < mode))                  \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MISC_CTRL] = mode;                    \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MISC_CTRL].u4Arg1 = arg1;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MISC_CTRL].u4Arg2 = arg2;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MISC_CTRL].u4Arg3 = arg3;                \
        LOG(6, "[SRM] MISC DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#if defined(CC_MT5395) || defined(CC_MT5396)
#define SRM_UPDATE_MJC(vdp, mode, arg1, arg2, arg3)                            \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MJC_CTRL] < mode))                   \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MJC_CTRL] = mode;                     \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MJC_CTRL].u4Arg1 = arg1;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MJC_CTRL].u4Arg2 = arg2;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MJC_CTRL].u4Arg3 = arg3;                 \
        LOG(6, "[SRM] MJC DB vdp%d mode%d 0x%x %d %d\n"                        \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#define SRM_UPDATE_MPEG_SIZE(vdp, mode, arg1, arg2, arg3)                      \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MMBUFCHG_CTRL] < mode))              \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MMBUFCHG_CTRL] = mode;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MMBUFCHG_CTRL].u4Arg1 = arg1;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MMBUFCHG_CTRL].u4Arg2 = arg2;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MMBUFCHG_CTRL].u4Arg3 = arg3;            \
        LOG(6, "[SRM] MPEG_SIZE DB vdp%d mode%d 0x%x %d %d\n"                  \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         \

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
/** Used to notify SRM/FBM changes. Used internally. */
#define SET_RESOLUTION(RES, MODE)                                              \
    do                                                                         \
    {                                                                          \
        if ((RES) >= SRM_VDP_1080HD_RESOLUTION)                                \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_1080HD;                                    \
        }                                                                      \
        else if ((RES) >= SRM_VDP_720HD_RESOLUTION)                            \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_720HD;                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_SD;                                        \
        }                                                                      \
        if (SRM_IsEpgMode())                                                   \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_EPG;                                       \
        }                                                                      \
    }                                                                          \
    while (0)

#if defined(CC_MT5363)
#define CC_SRM_RULE_BY_TABLE
#endif
#if defined(CC_SRM_RULE_BY_TABLE)
#define SV_ON 1
#define SV_OFF 0

typedef enum
{
    SRM_SCENE_SINGLE,
    SRM_SCENE_PIP,
    SRM_SCENE_POP,
    SRM_SCENE_MAX
} SRM_SCENARIO_T;

typedef enum
{
    SRM_VDO_1080I,
    SRM_VDO_576I,
    SRM_VDO_1080P422,
    SRM_VDO_1080P444,
    SRM_VDO_720P422,
    SRM_VDO_720P444,
    SRM_VDO_576P422,
    SRM_VDO_576P444,
    SRM_VDO_DTV1080I,
    SRM_VDO_DTV576I,
    SRM_VDO_DTV1080P,
    SRM_VDO_DTV720P,
    SRM_VDO_DTV576P,
    SRM_VDO_MM1080I,
    SRM_VDO_MM576I,
    SRM_VDO_MM1080P,
    SRM_VDO_MM720P,
    SRM_VDO_MM576P,
    SRM_VDO_MAX,
    SRM_VDO_UNSTABLE,
    SRM_VDO_SCAN_MODE
} SRM_VDO_TIMING_T;

typedef struct
{
    UINT32 u4IsValid;
    UINT32 u4SceneIdx;
    UINT32 u4VdpIdx;
    UINT32 u4VdoIdx;
    UINT32 u4ScalerMode;
    UINT32 u4PSCANNRMode;
} SRM_MODE_SETTING;

SRM_MODE_SETTING SRM_CONF_TABLE[SRM_SCENE_MAX][VDP_NS][SRM_VDO_MAX];

void vBuildSRMTable_MT5387_64MBx2(void)
{
    UINT32 u4i;

    // clear srm table to 0
    x_memset(SRM_CONF_TABLE, 0, sizeof(SRM_CONF_TABLE));

    // BUILD SINGLE
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080I].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080I].u4ScalerMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_PSCAN_DISP;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080I].u4PSCANNRMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_MDDI_NR_Y_C | FBM_POOL_MODE_MDDI_FULL | FBM_POOL_MODE_MDDI_DISP;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576I].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576I].u4ScalerMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576I].u4PSCANNRMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_MDDI_NR_Y_C | FBM_POOL_MODE_MDDI_FULL;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080P422].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080P422].u4ScalerMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080P422].u4PSCANNRMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_MDDI_NR_Y_C | FBM_POOL_MODE_MDDI_BOB;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080P444].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080P444].u4ScalerMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_1080P444].u4PSCANNRMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF | FBM_POOL_MODE_MDDI_BOB;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P422].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P422].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P422].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P422].u4VdoIdx = SRM_VDO_1080P422;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P444].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P444].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P444].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_720P444].u4VdoIdx = SRM_VDO_1080P444;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P422].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P422].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P422].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P422].u4VdoIdx = SRM_VDO_720P422;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P444].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P444].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P444].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_576P444].u4VdoIdx = SRM_VDO_720P444;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV1080I].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV1080I].u4ScalerMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_PSCAN_DISP;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV1080I].u4PSCANNRMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_MDDI_NR_Y_C | FBM_POOL_MODE_MDDI_FULL | FBM_POOL_MODE_MDDI_DISP;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV576I].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV576I].u4ScalerMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV576I].u4PSCANNRMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_MDDI_NR_Y_C | FBM_POOL_MODE_MDDI_FULL;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV1080P].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV1080P].u4ScalerMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV1080P].u4PSCANNRMode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_MDDI_NR_Y_C | FBM_POOL_MODE_MDDI_BOB;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV720P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV720P].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV720P].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV720P].u4VdoIdx = SRM_VDO_DTV1080P;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV576P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV576P].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV576P].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_DTV576P].u4VdoIdx = SRM_VDO_DTV720P;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080I].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080I].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080I].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080I].u4VdoIdx = SRM_VDO_DTV1080I;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576I].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576I].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576I].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576I].u4VdoIdx = SRM_VDO_DTV576I;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080P].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080P].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM1080P].u4VdoIdx = SRM_VDO_DTV1080P;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM720P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM720P].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM720P].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM720P].u4VdoIdx = SRM_VDO_DTV720P;

    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576P].u4SceneIdx = SRM_SCENE_SINGLE;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576P].u4VdpIdx = VDP_1;
    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][SRM_VDO_MM576P].u4VdoIdx = SRM_VDO_DTV576P;

    // dereference
    do 
    {
        UINT32 u4Done;

        u4Done = TRUE;
        for (u4i = SRM_VDO_1080I; u4i < SRM_VDO_MAX; u4i++)
        {
            UINT32 u4Scene;
            UINT32 u4Vdp;
            UINT32 u4Vdo;

            if (SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4IsValid == SV_OFF)
            {
                u4Scene = SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4SceneIdx;
                u4Vdp = SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4VdpIdx;
                u4Vdo = SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4VdoIdx;
                if (SRM_CONF_TABLE[u4Scene][u4Vdp][u4Vdo].u4IsValid == SV_ON)
                {
                    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4ScalerMode = SRM_CONF_TABLE[u4Scene][u4Vdp][u4Vdo].u4ScalerMode;
                    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4PSCANNRMode = SRM_CONF_TABLE[u4Scene][u4Vdp][u4Vdo].u4PSCANNRMode;
                    SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4IsValid = SV_ON;
                }
                else
                {
                    u4Done = FALSE;
                }
            }
        }

        if (u4Done == TRUE)
        {
            break;
        }
    } while (1);

    // BUILD PIP - Begin
    // main path in PIP
    // m.1. rules of main path are indexed to SINGLE.
    for (u4i = SRM_VDO_1080I; u4i < SRM_VDO_MAX; u4i++)
    {
        // SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][u4i].u4IsValid = SV_OFF;
        // SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][u4i].u4SceneIdx = SRM_SCENE_SINGLE;
        // SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][u4i].u4VdpIdx = VDP_1;
        // SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][u4i].u4VdoIdx = u4i;
        SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][u4i].u4IsValid = SV_ON;
        SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][u4i].u4ScalerMode = SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4ScalerMode;
        SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][u4i].u4PSCANNRMode = SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4PSCANNRMode;
    }
    // dereference
    // m.2. make the exceptional setting
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_1][SRM_VDO_1080P422].u4PSCANNRMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_Y_C | FBM_POOL_MODE_MDDI_BOB;
    // sub path in PIP
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080I].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080I].u4ScalerMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080I].u4PSCANNRMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF | FBM_POOL_MODE_MDDI_BOB;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576I].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576I].u4ScalerMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576I].u4PSCANNRMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF | FBM_POOL_MODE_MDDI_BOB;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080P422].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080P422].u4ScalerMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080P422].u4PSCANNRMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF | FBM_POOL_MODE_MDDI_BOB;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080P444].u4IsValid = SV_ON;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080P444].u4ScalerMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB | FBM_POOL_MODE_SCPOS_FULL;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_1080P444].u4PSCANNRMode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF | FBM_POOL_MODE_MDDI_BOB;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P422].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P422].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P422].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P422].u4VdoIdx = SRM_VDO_1080P422;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P422].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P422].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P422].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P422].u4VdoIdx = SRM_VDO_720P422;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P444].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P444].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P444].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_720P444].u4VdoIdx = SRM_VDO_1080P444;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P444].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P444].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P444].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_576P444].u4VdoIdx = SRM_VDO_720P444;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080I].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080I].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080I].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080I].u4VdoIdx = SRM_VDO_1080I;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576I].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576I].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576I].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576I].u4VdoIdx = SRM_VDO_576I;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080P].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080P].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV1080P].u4VdoIdx = SRM_VDO_1080P422;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV720P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV720P].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV720P].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV720P].u4VdoIdx = SRM_VDO_720P422;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576P].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576P].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_DTV576P].u4VdoIdx = SRM_VDO_576P422;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080I].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080I].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080I].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080I].u4VdoIdx = SRM_VDO_DTV1080I;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576I].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576I].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576I].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576I].u4VdoIdx = SRM_VDO_DTV576I;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080P].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080P].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM1080P].u4VdoIdx = SRM_VDO_DTV1080P;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM720P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM720P].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM720P].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM720P].u4VdoIdx = SRM_VDO_DTV720P;

    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576P].u4IsValid = SV_OFF;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576P].u4SceneIdx = SRM_SCENE_PIP;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576P].u4VdpIdx = VDP_2;
    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][SRM_VDO_MM576P].u4VdoIdx = SRM_VDO_DTV576P;
    // dereference
    do 
    {
        UINT32 u4Done;

        u4Done = TRUE;
        for (u4i = SRM_VDO_1080I; u4i < SRM_VDO_MAX; u4i++)
        {
            UINT32 u4Scene;
            UINT32 u4Vdp;
            UINT32 u4Vdo;

            if (SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4IsValid == SV_OFF)
            {
                u4Scene = SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4SceneIdx;
                u4Vdp = SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4VdpIdx;
                u4Vdo = SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4VdoIdx;
                if (SRM_CONF_TABLE[u4Scene][u4Vdp][u4Vdo].u4IsValid == SV_ON)
                {
                    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4ScalerMode = SRM_CONF_TABLE[u4Scene][u4Vdp][u4Vdo].u4ScalerMode;
                    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4PSCANNRMode = SRM_CONF_TABLE[u4Scene][u4Vdp][u4Vdo].u4PSCANNRMode;
                    SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4IsValid = SV_ON;
                }
                else
                {
                    u4Done = FALSE;
                }
            }
        }

        if (u4Done == TRUE)
        {
            break;
        }
    } while (1);
    // BUILD PIP - End

    // BUILD POP - Begin
    // main path
    // m.1. rules of main path are indexed to SINGLE.
    for (u4i = SRM_VDO_1080I; u4i < SRM_VDO_MAX; u4i++)
    {
        //SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4IsValid = SV_OFF;
        //SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4SceneIdx = SRM_SCENE_PIP;
        //SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4VdpIdx = VDP_1;
        //SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4VdoIdx = u4i;
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4IsValid = SV_ON;
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4ScalerMode = SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4ScalerMode;
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4PSCANNRMode = SRM_CONF_TABLE[SRM_SCENE_SINGLE][VDP_1][u4i].u4PSCANNRMode;
    }
    // dereference
    // m.2. make the exceptional setting
    for (u4i = SRM_VDO_1080I; u4i < SRM_VDO_MAX; u4i++)
    {
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4ScalerMode &= ~(FBM_POOL_MODE_SCPOS_MODE_MASK);
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4ScalerMode |= FBM_POOL_MODE_SCPOS_FULL;

        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_1][u4i].u4PSCANNRMode &= ~(FBM_POOL_MODE_MDDI_DISP);
    }
    // sub path
    // s.1. rules of sub path are indexed to PIP.
    for (u4i = SRM_VDO_1080I; u4i < SRM_VDO_MAX; u4i++)
    {
        // SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][u4i].u4IsValid = SV_OFF;
        // SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][u4i].u4SceneIdx = SRM_SCENE_PIP;
        // SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][u4i].u4VdpIdx = VDP_2;
        // SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][u4i].u4VdoIdx = u4i;
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][u4i].u4IsValid = SV_ON;
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][u4i].u4ScalerMode = SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4ScalerMode;
        SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][u4i].u4PSCANNRMode = SRM_CONF_TABLE[SRM_SCENE_PIP][VDP_2][u4i].u4PSCANNRMode;
    }
    // dereference
    // 2. make the exceptional setting
    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_1080I].u4PSCANNRMode &= ~(FBM_POOL_MODE_MDDI_MODEMSK);
    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_1080I].u4PSCANNRMode |= FBM_POOL_MODE_MDDI_FULL;

    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_576I].u4PSCANNRMode &= ~(FBM_POOL_MODE_MDDI_MODEMSK);
    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_576I].u4PSCANNRMode |= FBM_POOL_MODE_MDDI_FULL;

    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_DTV1080I].u4PSCANNRMode &= ~(FBM_POOL_MODE_MDDI_MODEMSK);
    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_DTV1080I].u4PSCANNRMode |= FBM_POOL_MODE_MDDI_FULL;

    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_DTV576I].u4PSCANNRMode &= ~(FBM_POOL_MODE_MDDI_MODEMSK);
    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_DTV576I].u4PSCANNRMode |= FBM_POOL_MODE_MDDI_FULL;

    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_MM1080I].u4PSCANNRMode &= ~(FBM_POOL_MODE_MDDI_MODEMSK);
    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_MM1080I].u4PSCANNRMode |= FBM_POOL_MODE_MDDI_FULL;

    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_MM576I].u4PSCANNRMode &= ~(FBM_POOL_MODE_MDDI_MODEMSK);
    SRM_CONF_TABLE[SRM_SCENE_POP][VDP_2][SRM_VDO_MM576I].u4PSCANNRMode |= FBM_POOL_MODE_MDDI_FULL;
    // BUILD POP - End

    // unstable case - move to query functions
    // handle scan mode - move to query function
    // handle game mode - move to query function
    // handle epg mode - move to query function
}

UINT32 SRM_GetTableMode(SRM_VDP_INFO_T* prSrmVdpInfo, UINT32 u4VdpId)
{
    // SRM_VDP_INFO_T* prSrmVdpInfo;
    UINT32 u4Mode;

    // prSrmVdpInfo = SRM_GetVdpInfo(VDP_1);

    // get env
    if (SRM_IsScanMode(u4VdpId))
    {
        u4Mode = SRM_VDO_SCAN_MODE;
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[u4VdpId])) &&
             ((u4VdpId == VDP_2) || (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG)) &&
             ((u4VdpId == VDP_2) || (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL)))
    {
        if (prSrmVdpInfo[u4VdpId].u4Source == (UINT32)VSS_DTV)
        {
            if (prSrmVdpInfo[u4VdpId].u4Interlace == 1)
            {
                if ((prSrmVdpInfo[u4VdpId].u4Resolution > SRM_VDP_SD_RESOLUTION))
                {
                    // dtv-1080i
                    u4Mode = SRM_VDO_DTV1080I;
                }
                else
                {
                    // dtv-576i
                    u4Mode = SRM_VDO_DTV576I;
                }
            }
            else
            {
                if ((prSrmVdpInfo[u4VdpId].u4Resolution >= SRM_VDP_1080HD_RESOLUTION))
                {
                    // dtv-1080p
                    u4Mode = SRM_VDO_DTV1080P;
                }
                else if ((prSrmVdpInfo[u4VdpId].u4Resolution >= SRM_VDP_720HD_RESOLUTION))
                {
                    // dtv-720p
                    u4Mode = SRM_VDO_DTV720P;
                }
                else
                {
                    // dtv-576p
                    u4Mode = SRM_VDO_DTV576P;
                }
            }
        }
        else if (SRM_GetMmMode() == SRM_MM_MODE_TYPE_VIDEO)
        {
            if (prSrmVdpInfo[u4VdpId].u4Interlace == 1)
            {
                if ((prSrmVdpInfo[u4VdpId].u4Resolution > SRM_VDP_SD_RESOLUTION))
                {
                    u4Mode = SRM_VDO_MM1080I;
                }
                else
                {
                    u4Mode = SRM_VDO_MM576I;
                }
            }
            else
            {
                if ((prSrmVdpInfo[u4VdpId].u4Resolution >= SRM_VDP_1080HD_RESOLUTION))
                {
                    u4Mode = SRM_VDO_MM1080P;
                }
                else if ((prSrmVdpInfo[u4VdpId].u4Resolution >= SRM_VDP_720HD_RESOLUTION))
                {
                    u4Mode = SRM_VDO_MM720P;
                }
                else
                {
                    u4Mode = SRM_VDO_MM576P;
                }
            }
        }
        else
        {
            if (prSrmVdpInfo[u4VdpId].u4Interlace == 1)
            {
                if ((prSrmVdpInfo[u4VdpId].u4Resolution > SRM_VDP_SD_RESOLUTION))
                {
                    u4Mode = SRM_VDO_1080I;
                }
                else
                {
                    u4Mode = SRM_VDO_576I;
                }
            }
            else
            {
                if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
                {
                    if ((prSrmVdpInfo[u4VdpId].u4Resolution > SRM_VDP_720HD_RESOLUTION))
                    {
                        u4Mode = SRM_VDO_1080P444;
                    }
                    else if ((prSrmVdpInfo[u4VdpId].u4Resolution > SRM_VDP_SD_RESOLUTION))
                    {
                        u4Mode = SRM_VDO_720P444;
                    }
                    else
                    {
                        u4Mode = SRM_VDO_576P444;
                    }
                }
                else
                {
                    if ((prSrmVdpInfo[u4VdpId].u4Resolution >= SRM_VDP_1080HD_RESOLUTION))
                    {
                        u4Mode = SRM_VDO_1080P422;
                    }
                    else if ((prSrmVdpInfo[u4VdpId].u4Resolution >= SRM_VDP_720HD_RESOLUTION))
                    {
                        u4Mode = SRM_VDO_720P422;
                    }
                    else
                    {
                        u4Mode = SRM_VDO_576P422;
                    }
                }
            }
        }

    }
    else
    {
        u4Mode = SRM_VDO_UNSTABLE;
    }

    return u4Mode;
}

UINT32 SRM_GETScalerMode(SRM_VDP_INFO_T* prSrmVdpInfo, UINT32 u4VdpId)
{
    UINT32 u4Mode;
    UINT32 u4Ret;
    UINT32 u4Scene;

    u4Mode = SRM_GetTableMode(prSrmVdpInfo, u4VdpId);

    // query
    if (u4Mode < SRM_VDO_MAX)
    {
        if (SRM_IsPipVideo())
        {
            u4Scene = SRM_SCENE_PIP;
        }
        else if (SRM_IsPopVideo())
        {
            u4Scene = SRM_SCENE_POP;
        }
        else
        {
            u4Scene = SRM_SCENE_SINGLE;
        }
        u4Ret = SRM_CONF_TABLE[u4Scene][u4VdpId][u4Mode].u4ScalerMode; 

        #if 0
        // exception - game mode
        if ((u4VdpId == VDP_1) && SRM_IsGameMode(VDP_1))
        {
            u4Ret &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Ret |= FBM_POOL_MODE_SCPOS_2FB;
        }
        #endif
        // exception - epg mode
        if ((u4VdpId == VDP_1) && SRM_IsEpgMode())
        {
            u4Ret &= ~FBM_POOL_MODE_SCPOS_MODE_MASK;
            u4Ret |= FBM_POOL_MODE_SCPOS_FULL;
        }

        SET_RESOLUTION(prSrmVdpInfo[u4VdpId].u4Resolution, u4Ret);
    }
    else if (u4Mode == SRM_VDO_SCAN_MODE)
    {
        // exception - scan mode
        u4Ret = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT | FBM_POOL_MODE_SCPOS_3FB;
    }
    else // if (u4Mode == SRM_VDO_UNSTABLE)
    {
        // exception - unstable
        u4Ret = FBM_POOL_MODE_UNSTABLE;
    }

    // exception - zoom mode

    return u4Ret;
}

UINT32 SRM_GETPsanNRMode(SRM_VDP_INFO_T* prSrmVdpInfo, UINT32 u4VdpId)
{
    UINT32 u4Mode;
    UINT32 u4Ret;
    UINT32 u4Scene;

    u4Mode = SRM_GetTableMode(prSrmVdpInfo, u4VdpId);

    // query
    if (u4Mode < SRM_VDO_MAX)
    {
        if (SRM_IsPipVideo())
        {
            u4Scene = SRM_SCENE_PIP;
        }
        else if (SRM_IsPopVideo())
        {
            u4Scene = SRM_SCENE_POP;
        }
        else
        {
            u4Scene = SRM_SCENE_SINGLE;
        }
        u4Ret = SRM_CONF_TABLE[u4Scene][u4VdpId][u4Mode].u4PSCANNRMode; 

        // exception - game mode
        if ((u4VdpId == VDP_1) && SRM_IsGameMode(VDP_1))
        {
            u4Ret &= ~FBM_POOL_MODE_MDDI_NR_MODEMSK;
            u4Ret |= FBM_POOL_MODE_MDDI_NR_OFF;
            u4Ret &= ~FBM_POOL_MODE_MDDI_MODEMSK;
            u4Ret |= FBM_POOL_MODE_MDDI_BOB;
        }
        // exception - epg mode

        SET_RESOLUTION(prSrmVdpInfo[u4VdpId].u4Resolution, u4Ret);
    }
    else if (u4Mode == SRM_VDO_SCAN_MODE)
    {
        // exception - scan mode
        u4Ret = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF | FBM_POOL_MODE_MDDI_BOB;
    }
    else // if (u4Mode == SRM_VDO_UNSTABLE)
    {
        // exception - unstable
        u4Ret = FBM_POOL_MODE_MDDI_NR_OFF | FBM_POOL_MODE_MDDI_BOB;
    }

    // exception - zoom mode

    return u4Ret;
}
#endif
static UINT32 _SrmGetScposMode(SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    return prSrmVdpCtrl[VDP_1].rNewPrm[SRM_DISP_CTRL].u4Arg1;
}

#if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
#define DONOT_USE_DISPMODE()\
    do {\
        if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && (prSrmVdpInfo[VDP_1].u4OutputHeight < 5000) && (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION))\
        {\
            u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;\
            u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;\
            u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;\
            u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;\
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;\
        }\
    } while (0)
#define GIVEUP_PSCANNR_DRAM()\
    do {\
        if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && (prSrmVdpInfo[VDP_1].u4OutputHeight < 5000) && (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION))\
        {\
            u4Mode &= ~FBM_POOL_MODE_10BIT;\
            u4Mode |= FBM_POOL_MODE_8BIT;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;\
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;\
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;\
        }\
    } while (0)
#endif


#if defined(CC_DOWN_BW_WITH_MENU)
#define JUDEG_TO_DOWN_SCALER_PQ()\
    do {\
        if (SRM_IsDownBWMode())\
        {\
            u4Mode &= ~FBM_POOL_MODE_10BIT;\
            u4Mode |= FBM_POOL_MODE_8BIT;\
        }\
    } while (0)
#define JUDGE_TO_DOWN_PSCANNR_PQ()\
    do {\
        if (SRM_IsDownBWMode())\
        {\
            u4Mode &= ~FBM_POOL_MODE_10BIT;\
            u4Mode |= FBM_POOL_MODE_8BIT;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;\
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;\
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;\
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;\
        }\
    } while (0)
#endif

/** Only used in main scaler path with frame track. Incompatible with MT8282!? */
// based on _SrmScposRule5362b
static UINT32 _SrmScposRule5387_64Mx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        // not consider game mode
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

#ifndef __MODEL_slt__  // always non-dispmode for SLT
        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1) ||
            SRM_IsPopVideo() ||
            SRM_IsEpgMode())
        {
            #if defined(CC_MT5365) || defined(CC_MT5395)
            if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION) && !SRM_IsEpgMode())
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            }
            #else
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            #endif
        }
        #ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
        else if (prSrmVdpInfo[VDP_1].u4Resolution > (1440 * 576))
        #else
        else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
        #endif
        {
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            else
            {
                #if defined(CC_MT5365) || defined(CC_MT5395)
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                #else
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
                #endif
            }
        }
        else
#endif // slt
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else if (SRM_GetMmMode() == SRM_MM_MODE_TYPE_VIDEO)
        {
            #if defined(CC_SRM_RULE_BY_TABLE)
            u4Mode |= FBM_POOL_MODE_10BIT;
            #else
            u4Mode |= FBM_POOL_MODE_8BIT;
            #endif
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            #endif
        }

#ifdef CC_MIRROR_SW_WA
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
			    u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
				u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode &= ~FBM_POOL_MODE_9BIT;
                u4Mode &= ~FBM_POOL_MODE_10BIT;
                u4Mode |= FBM_POOL_MODE_8BIT;
				u4Mode &= ~FBM_POOL_MODE_SCPOS_1FB;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_2FB;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
				u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
				u4Mode &= ~FBM_POOL_MODE_420_FB;
                u4Mode &= ~FBM_POOL_MODE_422_FB;
                u4Mode &= ~FBM_POOL_MODE_444_FB;
				u4Mode |= FBM_POOL_MODE_444_FB;
#endif

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP1: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    #if defined(CC_SRM_RULE_BY_TABLE)
    if (u4Mode != SRM_GETScalerMode(prSrmVdpInfo, VDP_1))
    {
        LOG(1, "VDP_1 scaler mode mismatch\n");
        LOG(1, "table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_1));
        LOG(1, "rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETScalerMode(prSrmVdpInfo, VDP_1));

        //Printf("VDP_1 scaler mode mismatch\n");
        //Printf("table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_1));
        //Printf("rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETScalerMode(prSrmVdpInfo, VDP_1));

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    #endif

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }
        #ifdef CC_MIRROR_SW_WA
        u4Mode &= ~FBM_POOL_MODE_420_FB;
        u4Mode &= ~FBM_POOL_MODE_422_FB;
        u4Mode &= ~FBM_POOL_MODE_444_FB;
        u4Mode |= FBM_POOL_MODE_444_FB;
        #endif

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }

        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_2)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP2: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
	
    #if defined(CC_SRM_RULE_BY_TABLE)
    if (u4Mode != SRM_GETScalerMode(prSrmVdpInfo, VDP_2))
    {
        LOG(1, "VDP_2 scaler mode mismatch\n");
        LOG(1, "table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_2));
        LOG(1, "rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETScalerMode(prSrmVdpInfo, VDP_2));

        //Printf("VDP_2 scaler mode mismatch\n");
        //Printf("table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_2));
        //Printf("rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETScalerMode(prSrmVdpInfo, VDP_2));

        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    #endif

    return u4Mode;
}

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368)||defined(CC_MT5389)
static UINT32 _SrmScposRule5395_DDRXx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();
    UINT32 u4ScalerModeEx=0;

    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_1);
    #endif

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        // not consider game mode
        //#ifdef CC_MT5396
        //u4Mode = FBM_POOL_MODE_SCPOS_LINESYNC_DISP | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        //#else
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        //#endif
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif

        //#ifdef CC_MT5368
        //u4Mode = FBM_POOL_MODE_SCPOS_3FB|FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        //#endif
        //SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

    	#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) 
    	//if (SRM_IsPopVideo()||SRM_IsEpgMode())  
    	if (SRM_IsPopVideo()
            ||SRM_IsMainVerticalFull()
            ||(E_TDTV_DRAM_3FB==u4DrvTDTVForceDramModeQuery())
            )
    	#else
        if (SRM_IsEpgMode())  
        #endif
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        #if defined(CC_MT5368) || defined(CC_MT5389) || defined(CC_MT5396)
        else if((prSrmVdpInfo[VDP_1].u4VgaTiming == 1))
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        #endif
        else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
        {
            #if defined(CC_MT5395) || defined(CC_MT5396)
            if (
                (((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
                  || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
                  || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)                  
                  || (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
                  )
                #ifdef CC_SCPOS_3DTV_SUPPORT
                    &&(u4DrvTDTVForceDramModeQuery())
                #endif
                 )
                ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)             
                #ifdef CC_SCPOS_3DTV_SUPPORT
                ||((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
                &&(u4DrvTDTVMJCModeQuery()!=0)) 
                #endif
                )
            {
                #if 0
                if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
                {
                    u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                }
                else
                #endif
                {
                    u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
                }
            }
            else
            #endif
            #if defined(CC_MT5365)
            if (((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)||(SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP))
                #ifdef CC_SCPOS_3DTV_SUPPORT
                &&(u4DrvTDTVForceDramModeQuery())
                #endif
               )
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            }
			else
            #endif
            {
            	#ifdef CC_MT5396
            	//if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
            	//{
            	//	u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            	//}
            	//else
            	//{
            	  u4Mode |= FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
            	//}
            	#else
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            	#endif
            }
        }
        else
        {
        	//#ifdef CC_MT5396
            	//u4Mode |= FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
            	//#else
            	u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            	//#endif
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }
        #if defined(CC_MT5395) || defined(CC_MT5396)
        #ifdef CC_SCPOS_3DTV_SUPPORT 
        if((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
                &&(u4DrvTDTVMJCModeQuery()!=0)&&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION))
        {
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;     
        }
        #endif
        #endif
        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode &= ~FBM_POOL_MODE_10BIT;

                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                #ifdef CC_MT5396
                u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
                #else
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                #endif
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                #ifdef CC_MT5396
                u4Mode |= FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
                #else
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                #endif
            }
            #endif
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif
        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        DONOT_USE_DISPMODE();
        #endif

        //#ifdef CC_MT5396
        //u4Mode = FBM_POOL_MODE_SCPOS_3FB|FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_10BIT;
        //if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        //{
        //    u4Mode |= FBM_POOL_MODE_444_FB;
        //}
        //else
        //{
        //    u4Mode |= FBM_POOL_MODE_422_FB;
        //}
        //#endif
        //SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
        
        #if (defined(CC_MT5368)||defined(CC_MT5389) ) && defined(__MODEL_slt__) // for SLT, always DRAM mode
        u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        #endif
        
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        #ifdef CC_MT5396
        //u4Mode = FBM_POOL_MODE_SCPOS_2FB|FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        #endif
        //SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }

    u4Mode = 0;
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_2);
    #endif
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif
        //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }
        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif
        /*
	#ifdef CC_MT5396
        u4Mode = FBM_POOL_MODE_SCPOS_3FB|FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_10BIT;
	if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }
        #endif
        */
        //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
	#ifdef CC_MT5396
	//u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
    //u4Mode |= FBM_POOL_MODE_SCPOS_2FB;;	
    #endif
    return u4Mode;
}
#endif

#if defined(CC_MT5389)||defined(CC_MT5368)
static UINT32 _SrmScposRule5501_net(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();
    UINT32 u4ScalerModeEx=0;

    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_1);
    #endif

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
		
     #if defined(CC_MT5368) || defined(CC_MT5389) 
    	if((SRM_IsPopVideo()||SRM_IsMainVerticalFull())||((SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
            &&(u4DrvTDTVForceDramModeQuery())))
    #else
        if (SRM_IsEpgMode())  
    #endif
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        #if defined(CC_MT5368) || defined(CC_MT5389) 
        else if((prSrmVdpInfo[VDP_1].u4VgaTiming == 1))
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        #endif
        else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
        {
        	#ifdef CC_MT5396
			u4Mode |= FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
        	#else
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        	#endif
        }
        else
        {
        	#ifdef CC_MT5396
        	if (BSP_GetIcVersion() == IC_VER_5396_AA)
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
			}
			else
			#endif
			{
        	    u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
			}
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }
        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode &= ~FBM_POOL_MODE_10BIT;

                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

		if ((SRM_GetCurrFBMMode() == FBM_MMMODE)||(SRM_GetCurrFBMMode() == FBM_NETMODE))
		{
			if (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION)
				{
					u4Mode &= ~FBM_POOL_MODE_10BIT;	
					u4Mode |= FBM_POOL_MODE_8BIT;
				}
			else
				{
					u4Mode &= ~FBM_POOL_MODE_8BIT;	
					u4Mode |= FBM_POOL_MODE_10BIT;
				}
	    }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                #ifdef CC_MT5396
                u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
                #else
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                #endif
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                #ifdef CC_MT5396
                u4Mode |= FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
                #else
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                #endif
            }
            #endif
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif
        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        DONOT_USE_DISPMODE();
        #endif

        //#ifdef CC_MT5396
        //u4Mode = FBM_POOL_MODE_SCPOS_3FB|FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_10BIT;
        //if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        //{
        //    u4Mode |= FBM_POOL_MODE_444_FB;
        //}
        //else
        //{
        //    u4Mode |= FBM_POOL_MODE_422_FB;
        //}
        //#endif
        //SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
        
        #if (defined(CC_MT5368)||defined(CC_MT5389) ) && defined(__MODEL_slt__) // for SLT, always DRAM mode
        u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        #endif
        
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        #ifdef CC_MT5396
        //u4Mode = FBM_POOL_MODE_SCPOS_2FB|FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        #endif
        //SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }

    u4Mode = 0;
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_2);
    #endif
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif
        //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }
        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDEG_TO_DOWN_SCALER_PQ();
        #endif
        /*
	#ifdef CC_MT5396
        u4Mode = FBM_POOL_MODE_SCPOS_3FB|FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_10BIT;
	if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }
        #endif
        */
        //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);
    }
	#ifdef CC_MT5396
	//u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
    //u4Mode |= FBM_POOL_MODE_SCPOS_2FB;;	
    #endif
    return u4Mode;
}
#endif

#if defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5389)
static UINT32 _SrmScposRule5365_128Mx1(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

	#if defined(CC_MT5368) || defined(CC_MT5389)
        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1) ||
            SRM_IsPopVideo() ||
            SRM_IsMainVerticalFull() ||
            (u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB))
        {
        	if((u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB) || (prSrmVdpInfo[VDP_1].u4VgaTiming == 1))
    		{
				if(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
				{
					u4PDSize = 1440;
				}
				u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
    		}
            else if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION) &&( !SRM_IsPopVideo()) && (!SRM_IsMainVerticalFull()))
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            else
            {    
                if(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
                {
                u4PDSize = 1440;
                }
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            }
        }
        #else
        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1) ||
            SRM_IsPopVideo() ||
            SRM_IsEpgMode())
        {
            if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION) && !SRM_IsEpgMode())
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            }
        }
        #endif
        else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
            {
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_10BIT;
            }
        }

	#if defined(CC_128Mx1_WITHSUB_OVERSET)
        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
	{
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
	    {
		u4PDSize = 1440;
	    }
	}
	#endif

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            #endif
        }

        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        DONOT_USE_DISPMODE();
        #endif

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
	
    return u4Mode;
}
#endif

#if defined(CC_MT5365)
static UINT32 _SrmScposRule5365_128Mx1_NOSUB(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1) ||
            SRM_IsPopVideo() ||
            SRM_IsEpgMode())
        {
            if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION) && !SRM_IsEpgMode())
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            }
        }
        else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
                && (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
            {
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_10BIT;
            }
        }

        if ((!SRM_IsPopVideo())
            && ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION) && (prSrmVdpInfo[VDP_1].u4Interlace == 0)))
        {
            // DTV-1080P
            u4PDSize = 1440;
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            #endif
        }

        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        DONOT_USE_DISPMODE();
        #endif
		
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    u4Mode = FBM_POOL_MODE_UNSTABLE;
    SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
	
    return u4Mode;
}
#endif

static UINT32 _SrmScposRule5365_64Mx1_ATV_WXGA(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        // not consider game mode

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (SRM_IsPopVideo() ||
            SRM_IsEpgMode() 
            // || (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF) // check MM 1080i mode
            )
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else if ((prSrmVdpInfo[VDP_1].u4Interlace == 1) &&
                (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION))
        {
            // 1080i
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            #endif
        }

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP1: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;

        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }

        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_2)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP2: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    return u4Mode;
}


static UINT32 _SrmScposRule5387_64Mx1_ATV_WXGA(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        // not consider game mode

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (SRM_IsPopVideo() ||
            SRM_IsEpgMode() 
            // || (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF) // check MM 1080i mode
            )
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else if ((prSrmVdpInfo[VDP_1].u4Interlace == 1) &&
                (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION))
        {
            // 1080i
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            #endif
        }

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP1: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;

        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }

        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_2)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP2: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    return u4Mode;
}
static UINT32 _SrmScposRule5363_64Mx1_Single(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        // not consider game mode

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) &&
                 (prSrmVdpInfo[VDP_1].u4Interlace == 0) &&
                 (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
        {
            // 1080p
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (SRM_IsEpgMode() 
            // || SRM_IsPopVideo()
            // || (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF)
           )
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION) &&
                 (prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            // 1080i
           u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
			// 1080i
        //    u4Mode |= FBM_POOL_MODE_SCPOS_DISP;
        }
        else if(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_DISP;
			
        }
        else
        {
	      u4Mode |= FBM_POOL_MODE_SCPOS_FULL;	 
        }
        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }
        

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP1: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
#if 1
       u4Mode = 0;
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        u4Mode |= FBM_POOL_MODE_422_FB;     
        SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    
#endif
    return u4Mode;
}
static UINT32 _SrmScposRule5387_64Mx1_Single(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        // not consider game mode
        
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) &&
                 (prSrmVdpInfo[VDP_1].u4Interlace == 0) &&
                 (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
        {
            // 1080p
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (SRM_IsEpgMode() 
            // || SRM_IsPopVideo()
            // || (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF)
           )
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        #if defined(CC_DTV_HD_DOT_BY_DOT)
        else if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && (prSrmVdpInfo[VDP_1].u4Resolution == SRM_VDP_1080HD_RESOLUTION))
        {
            #if 0
            if (SRM_IsForce2DotByDot())
            {
                LOG(5, "SRM_IsForce2DotByDot()\n");
                SRM_ClrZoomDBDChg();
                u4Mode &= ~(FBM_POOL_MODE_SCPOS_DISP | FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_SCPOS_PSCAN_DISP | FBM_POOL_MODE_SCPOS_MJC_DISP);
                // dtv 1080i or dtv 1080p
                u4Mode |= FBM_POOL_MODE_SCPOS_DISP;
                if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
                {
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Width = 1920;
                }
            }
            else 
            #endif
            if (SRM_IsForce2Zoom())
            {
                LOG(5, "SRM_IsForce2Zoom()\n");
                SRM_ClrZoomDBDChg();
                u4Mode &= ~(FBM_POOL_MODE_SCPOS_DISP | FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_SCPOS_PSCAN_DISP | FBM_POOL_MODE_SCPOS_MJC_DISP);
                if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
                {
                    // 1080i
                    u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Width = 1440;
                }
                else
                {
                    // 1080p
                    u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
                }
            }
            else
            {
                if ((prSrmVdpInfo[VDP_1].u4InputSrcWidthRatio == 10000) &&
                    (prSrmVdpInfo[VDP_1].u4InputSrcHeightRadio == 10000) &&
                    (prSrmVdpInfo[VDP_1].u4OutputWidth == 10000) &&
                    (prSrmVdpInfo[VDP_1].u4OutputHeight == 10000) &&
                    (wDISPLAY_WIDTH == 1920) && 
                    (wDISPLAY_HEIGHT == 1080))
                {
                    LOG(5, "SRM_IsForce2DotByDot()\n");
                    // dtv 1080i or dtv 1080p
                    u4Mode &= ~(FBM_POOL_MODE_SCPOS_DISP | FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_SCPOS_PSCAN_DISP | FBM_POOL_MODE_SCPOS_MJC_DISP);
                    if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
                    {
                    	u4Mode |= FBM_POOL_MODE_SCPOS_DISP;
                        _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Width = 1920;
                    }
		    else
		    {
                        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
		    }
                }
                else
                {
                    LOG(5, "SRM_IsForce2Zoom()\n");
                    u4Mode &= ~(FBM_POOL_MODE_SCPOS_DISP | FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_SCPOS_PSCAN_DISP | FBM_POOL_MODE_SCPOS_MJC_DISP);
                    if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
                    {
                        // 1080i
                        u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                        _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Width = 1440;
                    }
                    else
                    {
                        // 1080p
                        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
                    }
                }
            }
        }
        #endif
        else if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION) &&
                 (prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            // 1080i
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        #if defined(CC_DTV_HD_DOT_BY_DOT)
        SRM_ClrZoomDBDChg();
        #endif

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: SCALER: VDP1: MM Buffer Change\n");
            u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_MM_BUFCHG;
        }
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    return u4Mode;
}
#if defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5389)
static UINT32 _SrmScposRule5365_64Mx1_Single(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if (SRM_IsEpgMode())
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else if ((prSrmVdpInfo[VDP_1].u4Interlace == 1) && (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION))
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            #if defined(CC_SCALER_DISP_FALLBACK)
            if((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && 
                    (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
            {
                if (SRM_IsForce2Zoom())
                {
                    LOG(5, "SRM_IsForce2Zoom()\n");
                    SRM_ClrZoomDBDChg();
                    u4PDSize =720;
                }
                else
                {
                    LOG(5, "SRM_TRY_SCALER_DISP()\n");
                    SRM_ClrZoomDBDChg();
                    u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                    u4Mode |= FBM_POOL_MODE_SCPOS_DISP;
                }
            }
            #else
            if((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && 
                    (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
            {
                u4PDSize =720;
            }
            #endif
        }
        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION)
            {
                u4Mode |= FBM_POOL_MODE_10BIT;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            #endif
        }

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    u4Mode = FBM_POOL_MODE_UNSTABLE;
    SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);

    return u4Mode;
}
//#endif

//#if defined(CC_MT5365)
static UINT32 _SrmScposRule5365_64Mx1_Single_NOH264(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if (SRM_IsEpgMode())
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else if ((prSrmVdpInfo[VDP_1].u4Interlace == 1) && (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION))
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }

        // use 444 in vga input source
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
            {
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_10BIT;
            }
        }

	#if defined(CC_FBM_SHRINK_64Mx1_NOH264)
	if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
	{
            // DTV-1080I, DTV-1080P
            u4PDSize = 1440;
	}
	#endif

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
                u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
            #endif
        }

        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, 0);
    }

    u4Mode = FBM_POOL_MODE_UNSTABLE;
    SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);

    return u4Mode;
}
#endif
static UINT32 _SrmMddiRule5387_64Mx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        // not consider game mode
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) 
        #if !(defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396))
        && (prSrmVdpInfo[VDP_1].u4VgaTiming == 0)
        #endif
        )
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);
        u4Mode |= FBM_POOL_MODE_10BIT;

        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            #ifndef __MODEL_slt__
            if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_DISP;
            }
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            #else // turn on bob mode in SLT
            //if (prSrmVdpInfo[VDP_1].u4Source != (UINT32)VSS_DTV)
            //{
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            //}
            //else
            //{
            //    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            //}
            if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
            {
                u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            }
            #endif
        }
        else
        {
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
            if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_DISP;
            }
            #endif
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            #if defined(CC_MT5365)
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            #elif !defined(__MODEL_slt__)
            // 422 1080P, pip case
            if ((prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_1080HD_RESOLUTION) && SRM_IsPipVideo())
            {
                if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                }
                else if (DRVCUST_OptGet(eDdrClock) > 800000000)
                {
                    // yc, 10b
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    #if 0
                    // yc, 8b
                    u4Mode &= ~FBM_POOL_MODE_10BIT;
                    u4Mode |= FBM_POOL_MODE_8BIT;
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    #endif
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            }
            #else
            if(prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) 
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            }
            #endif
        }

        #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }
        #endif

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            #endif
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP1: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    #if defined(CC_SRM_RULE_BY_TABLE)
    if (u4Mode != SRM_GETPsanNRMode(prSrmVdpInfo, VDP_1))
    {
        LOG(1, "VDP_1 pscan.nr mode mismatch\n");
        LOG(1, "table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_1));
        LOG(1, "rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETPsanNRMode(prSrmVdpInfo, VDP_1));

        //Printf("VDP_1 pscan.nr mode mismatch\n");
        //Printf("table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_1));
        //Printf("rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETPsanNRMode(prSrmVdpInfo, VDP_1));

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    #endif

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        if (SRM_IsPopVideo())
        {
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
            if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
            }
            else
            #endif
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
        }


        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]) &&
        (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);
        u4Mode |= FBM_POOL_MODE_8BIT;

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {
            if (SRM_IsPopVideo())
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

                #ifndef __MODEL_slt__
                    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
                    if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                    }
                    else
                    #endif
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                    }
                #else
                if (prSrmVdpInfo[VDP_2].u4Source != (UINT32)VSS_DTV)
                {
                    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
                    if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                    }
                    else
                    #endif
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                    }
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                }
                #endif
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB; // in sub pip, pscan DONOT support full mode
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB | FBM_POOL_MODE_MDDI_NR_OFF;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_2)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP2: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    #if defined(CC_SRM_RULE_BY_TABLE)
    if (u4Mode != SRM_GETPsanNRMode(prSrmVdpInfo, VDP_2))
    {
        LOG(1, "VDP_2 pscan.nr mode mismatch\n");
        LOG(1, "table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_2));
        LOG(1, "rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETPsanNRMode(prSrmVdpInfo, VDP_2));

        //Printf("VDP_2 pscan.nr mode mismatch\n");
        //Printf("table idx: 0x%x\n", SRM_GetTableMode(prSrmVdpInfo, VDP_2));
        //Printf("rule u4Mode: 0x%x, table u4Mode: 0x%x\n", u4Mode, SRM_GETPsanNRMode(prSrmVdpInfo, VDP_2));

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    #endif

    return u4Mode;
}

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368)||defined(CC_MT5389)
static UINT32 _SrmMddiRule5395_DDRXx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PscanModeEx=0;

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    u4PscanModeEx=u4DrvTDTVDIModeQuery();
    #endif

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        // not consider game mode
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif

        //SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) 
        )
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        u4Mode |= FBM_POOL_MODE_10BIT;
        #if defined(CC_MT5395) || defined(CC_MT5396)
        #ifdef CC_SCPOS_3DTV_SUPPORT 
        if((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)&&
                (((u4DrvTDTVMJCModeQuery()==0) &&(!((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)&&(prSrmVdpInfo[VDP_1].u4Interlace==1)))
                  &&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))||
                 ((u4DrvTDTVMJCModeQuery()!=0)&&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION))))
        {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }
        else
        #endif
        #endif
        {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        }

        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
        {
            #if defined(CC_MT5395) || defined(CC_MT5396)
            if(((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU) ||
		(SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET) ||
		#ifdef CC_SCPOS_3DTV_SUPPORT
		((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)&&(u4DrvTDTVMJCModeQuery()==0)) ||
		#endif
		(SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)) &&
                (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION) &&
                SRM_IsPipVideo())
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
            }
            #ifdef CC_SCPOS_3DTV_SUPPORT
            else if((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)&&(u4DrvTDTVMJCModeQuery()!=0)&&
                    (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            }
            #endif
            else
            #endif
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }


        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode &= ~FBM_POOL_MODE_9BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            #endif
        }

        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode &= ~FBM_POOL_MODE_10BIT;

                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif
        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        GIVEUP_PSCANNR_DRAM();
        #endif

        // hw limitation: frame sequential interlaced timing, pscan use bob mode
        if (u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }

        //SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        //SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        if (SRM_IsPopVideo())
        {
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
            if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
            }
            else
            #endif
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif

        //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]) &&
        (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);
        u4Mode |= FBM_POOL_MODE_8BIT;

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {
            if (SRM_IsPopVideo())
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

                #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
                if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                }
                else
                #endif
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                #if defined(CC_MT5395) || defined(CC_MT5396)
                if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET) ||
		    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC))
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                }
                else
                #endif
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB; // in sub pip, pscan DONOT support full mode
                }

                if (DRVCUST_OptGet(eDdrClock) < 800000000)
                {
                    u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;
                    u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                }
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB | FBM_POOL_MODE_MDDI_NR_OFF;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif

        //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    #ifdef CC_MT5396
        if (prSrmVdpInfo[VDP_1].u4Interlace == 0)
    	{
		u4Mode = FBM_POOL_MODE_MDDI_NR_OFF;
		u4Mode |= FBM_POOL_MODE_10BIT;
	}
    #endif

    return u4Mode;
}
#endif

#if defined(CC_MT5389)||defined(CC_MT5368)
static UINT32 _SrmMddiRule5501_net(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PscanModeEx=0;

    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    u4PscanModeEx=u4DrvTDTVDIModeQuery();
    #endif

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        // not consider game mode
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif

        //SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) 
        )
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        u4Mode |= FBM_POOL_MODE_10BIT;
		 if ((_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_FULL)&&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
         {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
		 }
		 else
		 {
		    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
		 }

        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
        {
			if(((SRM_GetCurrFBMMode()== FBM_MMMODE)||(SRM_GetCurrFBMMode()== FBM_NETMODE))&&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
			{
			   if(_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_FULL)
			   	{
			    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
			   	}
			    else
			    {
			    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
			    }
				u4Mode &= ~FBM_POOL_MODE_10BIT;
				u4Mode |= FBM_POOL_MODE_8BIT;
				u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
				u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
				u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
			}
			else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
             }
      
            // hw limitation: frame sequential interlaced timing, pscan use bob mode
            #ifdef CC_SCPOS_3DTV_SUPPORT
            if ((u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB)||((u4DrvTDTVForceDramModeQuery()!=0)&&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)))
            {
                u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
                u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            }
            //Printf("u4DrvTDTVForceDIDramModeQuery: 0x%x\n", u4DrvTDTVForceDIDramModeQuery());
            #endif
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
			if(((SRM_GetCurrFBMMode()== FBM_MMMODE)||(SRM_GetCurrFBMMode()== FBM_NETMODE))&&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
			{
				u4Mode &= ~FBM_POOL_MODE_10BIT;
				u4Mode |= FBM_POOL_MODE_8BIT;
			}
        }

        #ifdef CC_SCPOS_3DTV_SUPPORT
        if ((u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB)||((u4DrvTDTVForceDramModeQuery()!=0)&&(prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION)))
        {
			u4Mode &= ~FBM_POOL_MODE_10BIT;
			u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }
        //Printf("u4DrvTDTVForceDIDramModeQuery: 0x%x\n", u4DrvTDTVForceDIDramModeQuery());
        #endif


        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode &= ~FBM_POOL_MODE_9BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;
        }

        if (SRM_IsGameMode(VDP_1))
        {
            #ifdef CC_GAME_MODE_BETTER_Q
            if(prSrmVdpInfo[VDP_1].u4Interlace == 0)
            {
            	u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            	u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            	u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            }
            #else
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            #endif

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            #endif
        }

        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode &= ~FBM_POOL_MODE_10BIT;

                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif
        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        GIVEUP_PSCANNR_DRAM();
        #endif


        //SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        //SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        if (SRM_IsPopVideo())
        {
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
            if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
            }
            else
            #endif
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif

        //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]) &&
        (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);
        u4Mode |= FBM_POOL_MODE_8BIT;

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {
            if (SRM_IsPopVideo())
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

                #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
                if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                }
                else
                #endif
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB; // in sub pip, pscan DONOT support full mode

                if (DRVCUST_OptGet(eDdrClock) < 800000000)
                {
                    u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;
                    u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                }
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB | FBM_POOL_MODE_MDDI_NR_OFF;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        #if defined(CC_DOWN_BW_WITH_MENU)
        JUDGE_TO_DOWN_PSCANNR_PQ();
        #endif

        //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, u4PscanModeEx);
    }
    #ifdef CC_MT5396
        if (prSrmVdpInfo[VDP_1].u4Interlace == 0)
    	{
		u4Mode = FBM_POOL_MODE_MDDI_NR_OFF;
		u4Mode |= FBM_POOL_MODE_10BIT;
	}
    #endif

    return u4Mode;
}
#endif

#if defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5389)
static UINT32 _SrmMddiRule5365_128Mx1(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) 
        )
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);
        if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)

        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
        {
            if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_DISP;
            }
            if(0)//SRM_IsMainVerticalFull()&&(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
            {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
            }
            else
            {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
         #if defined(CC_MT5368) || defined(CC_MT5389)
         	 u4Mode |= FBM_POOL_MODE_MDDI_FULL;
         #else
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
            {
		#if defined(CC_128Mx1_WITHSUB_OVERSET)
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
		#else
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
		#endif
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
          #endif
            }
        }
        else
        {
            if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_DISP;
            }
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;

            if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        GIVEUP_PSCANNR_DRAM();
        #endif
		if((u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB))
		{
			u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
			u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
			u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

			u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
			u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
			u4Mode |= FBM_POOL_MODE_MDDI_BOB;
		}
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        if (SRM_IsPopVideo())
        {
            #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
            if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
            }
            else
            #endif
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]) &&
        (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);
        u4Mode |= FBM_POOL_MODE_8BIT;

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            if (SRM_IsPopVideo())
            {
                if (prSrmVdpInfo[VDP_2].u4Resolution > SRM_VDP_720HD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                }
                else
                {
                    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)
                    if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                    }
                    else
                    #endif
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                    }
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB | FBM_POOL_MODE_MDDI_NR_OFF;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }

    return u4Mode;
}
#endif

#if defined(CC_MT5365)
static UINT32 _SrmMddiRule5365_128Mx1_NOSUB(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);
        if (((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
            && (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
            ||(prSrmVdpInfo[VDP_1].u4VgaTiming == 1))
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_DISP;
            }

            if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
                && (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION))
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
        }
        else
        {
            if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_DISP;
            }
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;

            if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            }
        }
        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        #if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        GIVEUP_PSCANNR_DRAM();
        #endif

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }

    u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
    SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);

    return u4Mode;
}
#endif
static UINT32 _SrmMddiRule5363_64Mx1_Single(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        // not consider game mode

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) &&
        (prSrmVdpInfo[VDP_1].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_10BIT;
        if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
        {
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION))
                {
                    // dtv-1080i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    u4Mode &= ~FBM_POOL_MODE_10BIT;
                    u4Mode |= FBM_POOL_MODE_8BIT;
                    //u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
                else
                {
                    // dtv-576i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
            else
            {
                // dtv-progressive
                if (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                }
            }
        }
        else
        {
            // 1080i, 576i
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;

                if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
                {
                    // 1080i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY; // buggy
                    //u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C; // test
                }
                else
                {
                    // 576i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
            else
            {
                if (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION)
                {
                    // 422, 1080p
                    //u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
					u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    u4Mode &= ~FBM_POOL_MODE_10BIT;
                    u4Mode |= FBM_POOL_MODE_8BIT;
                }
                else
                {
                    // 422, 576p
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP1: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
#if 1
            u4Mode = 0;
            u4Mode |= FBM_POOL_MODE_10BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL| FBM_POOL_MODE_MDDI_NR_OFF;

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    
#endif
    return u4Mode;
}
static UINT32 _SrmMddiRule5387_64Mx1_Single(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        // not consider game mode

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) &&
        (prSrmVdpInfo[VDP_1].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_10BIT;
        if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
        {
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                if ((prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION))
                {
                    // dtv-1080i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    u4Mode &= ~FBM_POOL_MODE_10BIT;
                    u4Mode |= FBM_POOL_MODE_8BIT;
                    //u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
                else
                {
                    // dtv-576i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
            else
            {
                // dtv-progressive
                if (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                }
            }
        }
        else
        {
            // 1080i, 576i
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;

                if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
                {
                    // 1080i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY; // buggy
                    //u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C; // test
                }
                else
                {
                    // 576i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
            else
            {
                // if (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_1080HD_RESOLUTION)
                if (prSrmVdpInfo[VDP_1].u4Resolution >= 1280*960) // 1280*960*85Hz: for DMT1285A CVT1285H is OK
                {
                    // 422, 1080p
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    u4Mode &= ~FBM_POOL_MODE_10BIT;
                    u4Mode |= FBM_POOL_MODE_8BIT;
                }
                else
                {
                    // 422, 576p
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP1: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }

    return u4Mode;
}
#if defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5389)
static UINT32 _SrmMddiRule5365_64Mx1_Single(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION)
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
        {
	    #if !defined(CC_DYNAMIC_MPEG_SIZE)
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
	    #endif
            #if defined(CC_DYNAMIC_MPEG_SIZE) // to save the dram size in 64MB segment
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                }
                else
                {
                    // dtv-576i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                // dtv-progressive
                if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                }
                else if(!SRM_IsEpgMode())
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                }
            }
            #endif
        }
        else
        {
            // 1080i, 576i
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;

                u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }

    u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
    SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);

    return u4Mode;
}
//#endif

//#if defined(CC_MT5365)
static UINT32 _SrmMddiRule5365_64Mx1_Single_NOH264(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }
        if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
        {
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
                {
                    // dtv-1080i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                }
                else
                {
                    // dtv-576i
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                // dtv-progressive
                if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_720HD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            // 1080i, 576i
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }

    u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
    SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);

    return u4Mode;
}
#endif
static UINT32 _SrmMddiRule5365_64Mx1_ATV_WXGA(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        // not consider game mode

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) &&
        (prSrmVdpInfo[VDP_1].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        {
            // 1080i, 576i
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;

                if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
                {
                    // u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
                    {
                        // 1080i
                        // u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    }
                    else
                    {
                        // 576i
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    }
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
                {
                    // u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    if (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION)
                    {
                        // 576p
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    }
                    else
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    }
                }
                else if (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_1080HD_RESOLUTION)
                {
                    // 422, 1080p
                    if (SRM_IsPipVideo() || SRM_IsPopVideo())
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    }
                    else
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    }
                }
                else if (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION)
                {
                    // 422, 720p
                    if (SRM_IsPipVideo() || SRM_IsPopVideo())
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    }
                    else
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    }
                }
                else
                {
                    // 422, 576p
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            #endif
        }

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP1: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        #if 0 // original scan mode
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        #else // extend scan mode to atv source
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        if (SRM_IsPopVideo())
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        #endif

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_2])) && 
             (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);
        u4Mode |= FBM_POOL_MODE_8BIT;

        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {
            if (SRM_IsPopVideo())
            {
                    #if defined(CC_MT5365)
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                    #else
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                    #endif
            }
            else
            {
                if (prSrmVdpInfo[VDP_2].u4Resolution >= SRM_VDP_1080HD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_2)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP2: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }

    return u4Mode;
}

// based on _SrmMddiRule5360b
static UINT32 _SrmMddiRule5387_64Mx1_ATV_WXGA(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;

    SRM_CHECK_INFO;

    if (SRM_IsScanMode(VDP_1))
    {
        // extend scan mode to atv source
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }
        // not consider game mode

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) &&
        (prSrmVdpInfo[VDP_1].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        {
            // 1080i, 576i
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;

                if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
                {
                    // u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
                    {
                        // 1080i
                        // u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    }
                    else
                    {
                        // 576i
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    }
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                if (prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV)
                {
                    // u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    if (prSrmVdpInfo[VDP_1].u4Resolution <= SRM_VDP_SD_RESOLUTION)
                    {
                        // 576p
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    }
                    else
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
                    }
                }
                else if (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_1080HD_RESOLUTION)
                {
                    // 422, 1080p
                    if (SRM_IsPipVideo() || SRM_IsPopVideo())
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    }
                    else
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    }
                }
                else if (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION)
                {
                    // 422, 720p
                    if (SRM_IsPipVideo() || SRM_IsPopVideo())
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
                    }
                    else
                    {
                        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                    }
                }
                else
                {
                    // 422, 576p
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
                }
            }
        }

        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;

            #ifdef CC_GAME_MODE_BETTER_Q
            if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
            {
                u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            #endif
        }

        if (_SrmGetScposMode(prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_DISP;
        }

        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_1)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP1: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_1, u4CtrlMode, u4Mode, 0, 0);
    }

    u4Mode = 0;
    if (SRM_IsScanMode(VDP_2))
    {
        #if 0 // original scan mode
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        #else // extend scan mode to atv source
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        if (SRM_IsPopVideo())
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        #endif

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_2])) && 
             (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);
        u4Mode |= FBM_POOL_MODE_8BIT;

        u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {
            if (SRM_IsPopVideo())
            {
                    #if defined(CC_MT5365)
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                    #else
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                    #endif
            }
            else
            {
                if (prSrmVdpInfo[VDP_2].u4Resolution >= SRM_VDP_1080HD_RESOLUTION)
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_MDDI_FULL;
                }
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;

            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
        if (SRM_ISMMBufChg(VDP_2)) // check in unstable case
        {
            LOG(3, "SRM RULE: PSCAN: VDP2: MM Buffer Change\n");
            u4Mode |= FBM_POOL_MODE_8BIT;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            u4Mode |= FBM_POOL_MODE_MDDI_MM_BUFCHG;
        }
        SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    }

    return u4Mode;
}

#if defined(CC_MT5395) || defined(CC_MT5396)
extern SRM_TV_MODE_T _eApplicationMode;
static UINT32 _SrmMJCRule5395(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4MJCModeEx = 0;

    #ifdef CC_MT5396
    u4MJCModeEx = u4DrvTDTVMJCModeQuery();
    #endif

    SRM_CHECK_INFO;

    #ifdef CC_SCPOS_3DTV_SUPPORT
    #ifdef CC_MT5395
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
    {
        u4Mode = FBM_POOL_MODE_FRC_Y08_C08_444;
    }
    else if (u4DrvTDTVMJCModeQuery())
    {
        u4Mode = FBM_POOL_MODE_FRC_Y08_C08_444;
    }
    else
    #endif
    #endif
    #ifdef CC_MT5396
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
    {
        u4Mode = FBM_POOL_MODE_FRC_Y10_C10_444;
    }
    else
    #endif
    {
    	SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

    	if (SRM_IsGameMode(VDP_1) || (SRM_IsPipVideo() && (_eApplicationMode == SRM_TV_MODE_TYPE_PIP)) || SRM_IsPopVideo() || (prSrmVdpInfo[VDP_1].u4VgaTiming == 1))
    	{
    	    u4Mode = FBM_POOL_MODE_FRC_Y10_C10_444;
    	}
    	else if (SRM_IsScanMode(VDP_1))
    	{
    	    u4Mode = FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
    	}
    	else
    	{
    	    if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]))
    	    {
                if (DRVCUST_OptGet(eDdrClock) > 1100000000)
                {
                    //use 422 mode to avoid chroma missing when odd vertical oversan.
                    u4Mode = FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
                }
        	    else
        	    {
                    if (prSrmVdpInfo[VDP_1].u4Source != (UINT32)VSS_DTV)
                    {
                        u4Mode = FBM_POOL_MODE_MJC_Y08_C08_422_1RDC_3FRM;
                    }
                    else
                    {
                        u4Mode = FBM_POOL_MODE_MJC_Y10_C08_420_0RDC_3FRM;
                    }
                }
            }
            else
            {
                u4Mode = FBM_POOL_MODE_UNSTABLE;
            }
        }
    }

    SRM_UPDATE_MJC(VDP_1, u4CtrlMode, u4Mode, 0, u4MJCModeEx);

    return u4Mode;
}
#endif

// add for MPEG buffer change case
static UINT32 _SrmStableRule(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    static UINT32 cnt = 0; // force to call the callback
    UINT32 u4Mode=0;
    if (SRM_ISMMBufChg(VDP_1))
    {
        cnt++;
        SRM_CLRMMBufChg(VDP_1);
        SRM_UPDATE_MPEG_SIZE(VDP_1, SRM_CTRL_MODE_NONE, 0, 0, cnt);
    }
    if (SRM_ISMMBufChg(VDP_2))
    {
        cnt++;
        SRM_CLRMMBufChg(VDP_2);
        SRM_UPDATE_MPEG_SIZE(VDP_2, SRM_CTRL_MODE_NONE, 0, 0, cnt);
    }
    if(SRM_CheckStable(&prSrmVdpInfo[VDP_1]))
    {
        u4Mode=FBM_POOL_MODE_10BIT;
        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode |= FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
        else
        {
            u4Mode &= ~FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
        
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)

        #if defined(CC_MT5368) || defined(CC_MT5389)
        /* for 5389, TDC always uses game mode */
       // if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV))
       // {
       // 	u4Mode |= FBM_POOL_MODE_TDC_FULL;
       // }
       // else if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264))
       // {
        	u4Mode |= FBM_POOL_MODE_TDC_GAME;
       // }
        #endif
       #if defined(CC_MT5396)
       if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC))
       {
           u4Mode |= FBM_POOL_MODE_TDC_GAME;
       }       
       #endif
        
    #endif
        
        SRM_UPDATE_TDC(VDP_1, 1, u4Mode, 0, 0);
    }
    else
    {
        u4Mode=FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_TDC(VDP_1, 1, u4Mode, 0, 0);
    }
    if(SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        u4Mode=FBM_POOL_MODE_10BIT;
        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode |= FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
        else
        {
            u4Mode &= ~FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
        
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)

        #if defined(CC_MT5368) || defined(CC_MT5389)
        /* for 5389, TDC always uses game mode */
       // if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
       // 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
       // 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
       // 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264)
       // 	)
        {
        	u4Mode |= FBM_POOL_MODE_TDC_GAME;
        }
        #endif
       #if defined(CC_MT5396)
       if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC))
       {
           u4Mode |= FBM_POOL_MODE_TDC_GAME;
       }       
       #endif
        
    #endif
        SRM_UPDATE_TDC(VDP_2, 1, u4Mode, 0, 0);
    }
    else
    {
        u4Mode=FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_TDC(VDP_2, 1, u4Mode, 0, 0);
    }
    return 0;
}

static UINT32 _SrmDummyRule(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    return 0;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

VOID SRM_LoadRule(FN_SRN_DB_RULE **papfStableRule, UINT32 *pu4StableRuleNs,
                  FN_SRN_DB_RULE **papfNotStableRule, UINT32 *pu4NotStableRuleNs)
{
    #if defined(CC_SRM_RULE_BY_TABLE)
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_VERF) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_128MB) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2))
    {
        vBuildSRMTable_MT5387_64MBx2();
    }
    #endif
    if (!papfStableRule || !pu4StableRuleNs || !papfNotStableRule || !pu4NotStableRuleNs)
    {
        return;
    }

    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_VERF) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_128MB) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
       )
    {
        UINT8 idx = 0;
        #if defined(CC_MT5365) || defined(CC_MT5395)
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5395_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5395_DDRXx2;
        #else
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5387_64Mx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5387_64Mx2;
        #endif
        #if defined(CC_MT5395)
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMJCRule5395;
        #endif
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
    #if defined(CC_MT5365)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5365_128Mx1;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_128Mx1;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB)
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5365_128Mx1_NOSUB;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_128Mx1_NOSUB;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
    {
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5365_64Mx1_Single;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_64Mx1_Single;
        papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = 3;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1_NOH264)
    {
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5365_64Mx1_Single_NOH264;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_64Mx1_Single_NOH264;
        papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = 3;
    }
    #endif
    #if defined(CC_MT5395) || defined(CC_MT5396)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
             #if defined(CC_MT5396)
		|| (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
		|| (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
		|| (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
	      #endif
            )
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5395_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5395_DDRXx2;
	if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5395_NOMJC)
            && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
            && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
            //#if defined(CC_MT5396)
            //&& (SRMFBM_GetConf() != FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
            //#endif
            )
	{
	    papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMJCRule5395;
	}
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
    #endif
    #if defined(CC_MT5365) || defined(CC_MT5395)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
            #if defined(CC_MT5365)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
            #endif
            )
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5395_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5395_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
    #endif
    #if defined(CC_MT5368)||defined(CC_MT5389)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
    	|| (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
    	|| (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
            )
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5395_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5395_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5501_net;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5501_net;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
    	      || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK))
    {
    	UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5365_128Mx1;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_128Mx1;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1))
    {
    	papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5365_64Mx1_Single;
       papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_64Mx1_Single;
       papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
       *pu4StableRuleNs = 3;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264)
    {
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5365_64Mx1_Single_NOH264;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_64Mx1_Single_NOH264;
        papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = 3;
    }
    #endif
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_64MB)
    {
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5387_64Mx1_Single;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5387_64Mx1_Single;
        papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = 3;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MB)
    {
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5363_64Mx1_Single;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5363_64Mx1_Single;
        papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = 3;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_64MB_ATV_WXGA)
    {
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5387_64Mx1_ATV_WXGA;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5387_64Mx1_ATV_WXGA;
        papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = 3;
    }
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_ATV_64MB)
    {
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5365_64Mx1_ATV_WXGA;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5365_64Mx1_ATV_WXGA;
        papfStableRule[2] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = 3;
    }
    else
    {
        *pu4StableRuleNs = 0;
        *pu4NotStableRuleNs = 0;

        // to solve warning
        papfNotStableRule[0] = (FN_SRN_DB_RULE*)_SrmDummyRule;

        VERIFY(0);
        papfStableRule[0] = (FN_SRN_DB_RULE*)_SrmScposRule5387_64Mx2;
        papfStableRule[1] = (FN_SRN_DB_RULE*)_SrmMddiRule5387_64Mx2;
    }
}

UINT32 SRM_CheckDisplayMode(UINT32 u4Source, UINT32 u4Resolution,
                            UINT32 u4InputFrameRate, UINT32 u4VgaTiming, UINT32 u4Interlace)
{
    return SRM_GetVdpCtrl(VDP_1)->rPrm[SRM_DISP_CTRL].u4Arg1;
#if 0
    UINT32 u4Mode;
    SRM_VDP_INFO_T arSrmVdpInfo[2];
    SRM_VDP_CRTL_T arSrmVdpCtrl[2];

    x_memset((VOID*)&arSrmVdpInfo, 0, sizeof(SRM_VDP_INFO_T)*2);
    x_memset((VOID*)&arSrmVdpCtrl, 0, sizeof(SRM_VDP_CRTL_T)*2);

    arSrmVdpInfo[VDP_1].u4Enable = TRUE;
    arSrmVdpInfo[VDP_1].u4Status = TRUE;
    arSrmVdpInfo[VDP_1].u4Source = u4Source;
    arSrmVdpInfo[VDP_1].u4Resolution = u4Resolution;
    arSrmVdpInfo[VDP_1].u4InputFrameRate = u4InputFrameRate;
    arSrmVdpInfo[VDP_1].u4VgaTiming = u4VgaTiming;
    arSrmVdpInfo[VDP_1].u4Interlace = u4Interlace;

    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392F_128MB) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391F_FHD60_128MB) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392H_128MB) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391H_FHD60_128MB) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391_VERF)||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392B_EU_128MB)||
        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392B_US_128MB)||
        (SRMFBM_GetConf() == FBM_MEM_CFG_SH_MT5391F) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_SH_MT5391BF) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_SH_MT5391BF_128MB) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_SPACE) ||
        (SRMFBM_GetConf() == FBM_MEM_CFG_ROW))
    {
        u4Mode = _SrmScposRule5391(&arSrmVdpInfo[0], &arSrmVdpCtrl[0]);
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_128MB)
    {
        u4Mode = _SrmScposRule5362a(&arSrmVdpInfo[0], &arSrmVdpCtrl[0]);
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_128MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_64MB_MOUT) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_64MB))
    {
        u4Mode = _SrmScposRule5362b(&arSrmVdpInfo[0], &arSrmVdpCtrl[0]);
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5360_VERF_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360B_VERF_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5361B_64MB))
    {
        u4Mode = _SrmScposRule5360(&arSrmVdpInfo[0], &arSrmVdpCtrl[0]);
    }
    else
    {
        return FBM_POOL_MODE_SCPOS_FULL;
    }

    return u4Mode;
#else
    // return FBM_POOL_MODE_SCPOS_FULL;
#endif
}

UINT32 SRM_CheckMddiMode(UINT32 u4Source, UINT32 u4Resolution,
                         UINT32 u4InputFrameRate, UINT32 u4Interlace)
{
    // this function is only for SS, and SS will enable MDDi for all interlace input source
    // so don't need to process SRM rules
    if (u4Interlace == TRUE)
    {
        // use mddi
        return TRUE;
    }

    UNUSED(u4Source);
    UNUSED(u4Resolution);
    UNUSED(u4InputFrameRate);

    return FALSE;
}
