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
 * $Author: jiangbo.wang $
 * $Date: 2012/12/12 $
 * $RCSfile: fbm_pool_config.c,v $
 * $Revision: #10 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_pool_config.c
 *  Brief of file fbm_pool_config.c.
 *  FBM Pool Management
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "fbm_pool.h"
#include "fbm_pool_config.h"
#include "fbm_debug.h"
#include "../tve/tve_if.h"

#ifdef CC_SRM_ON
#include "srm_drvif.h"
#endif

LINT_EXT_HEADER_BEGIN
#include "source_table.h"
#include "vdp_drvif.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "drvcust_if.h"
#include "x_mid.h"
LINT_EXT_HEADER_END

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396)|| defined(CC_MT5368)||defined(CC_MT5389)
#include "drv_tdtv_drvif.h"
#endif


extern UINT32 SRM_GetScposMode(UINT32 u4VdpId);
extern UINT32 SRM_GetMddiMode(UINT32 u4VdpId);
extern UINT32 SRM_GetTDCMode(UINT32 u4VdpId);
extern UINT32 SRM_GetScposHPDSize(UINT32 u4VdpId);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static FBM_POOL_TYPE_T _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_MAIN;
static FBM_POOL_TYPE_T _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_PIP;
static FBM_POOL_TYPE_T _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_MAIN;
static FBM_POOL_TYPE_T _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_PIP;
static FBM_POOL_TYPE_T _eCurrentMjc = FBM_POOL_TYPE_MJC_1;
static FBM_POOL_TYPE_T _eCurrentTdc = FBM_POOL_TYPE_TDC;
static FBM_POOL_TYPE_T _eCurrentVbi = FBM_POOL_TYPE_VBI;


#if 0
static void _FbmUpdateResolution(UCHAR ucPoolType, FBM_POOL_T* prPool,
                                 const FBM_POOL_MODULE_INFO_T* prInfo)
{
    if ((prPool != NULL) && (prInfo != NULL))
    {
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
#if 0
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5360_VERF_64MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360B_VERF_64MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5361B_64MB))
#endif
                // cyj.porting.2009.03.12
                if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_64MB) ||
                        (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_64MB_ATV_WXGA))
                {
                    prPool->u4Width = 1536;
                }
                else
                {
#if 1 // defined(CC_MT5360B) || defined(CC_MT5392B)
                    if (prInfo->u4PipPop == SRM_TV_MODE_TYPE_POP)
                    {
                        prPool->u4Width = 1024;
                    }
                    else
                    {
                        prPool->u4Width = 2048;
                    }
#else
                    if (prInfo->u4PipPop == SRM_TV_MODE_TYPE_POP)
                    {
                        prPool->u4Width = 960;
                    }
                    else
                    {
                        prPool->u4Width = 1920;
                    }
#endif
                }
            return;
        }

        if (prInfo->u4Resolution > SRM_VDP_720HD_RESOLUTION)
        {
            if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MJC)
            {
                prPool->u4Height = 1088;
            }
            else
            {
                prPool->u4Height = 1080;
            }

            if (prInfo->u4PipPop == SRM_TV_MODE_TYPE_POP)
            {
                prPool->u4Width = 960;
                if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MJC)
                {
                    prPool->u4Height = 600;
                }
            }
            else
            {
                prPool->u4Width = 1920;
            }
        }
        else if (prInfo->u4Resolution == SRM_VDP_720HD_RESOLUTION)
        {
            prPool->u4Height = 720;
            if (prInfo->u4PipPop == SRM_TV_MODE_TYPE_POP)
            {
                prPool->u4Width = 960;
                if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MJC)
                {
                    prPool->u4Height = 600;
                }
            }
            else
            {
                prPool->u4Width = 1280;
            }
        }
        else
        {
            prPool->u4Width = 1440;
            prPool->u4Height = 576;
        }
    }
}
#endif

// NPTV dynamically-allocated FBM
static UINT32 _fbm_u4NptvBeAddr;
static UINT32 _fbm_u4NptvSubPipAddr;
static UINT32 _fbm_u4NptvSubPopAddr;

typedef enum
{
    FBM_MODE_1080I,
    FBM_MODE_576I,
    FBM_MODE_1080P_422,
    FBM_MODE_1080P_444
} FBM_POOL_SCENARIO_T;

typedef struct
{
    UINT8 u1fgPIP;
    UINT8 u1fgPOP;
    UINT8 u1fgSingle;
    UINT8 u1mode[2];
    UINT8 u1Interlace[2];
    UINT8 u1IsMPEG[2];
    UINT8 u1IsTDC[2];
    UINT32 u4Resolution[2];
    UINT32 u4InputWidth[2];
    UINT32 u4InputHeight[2];
} FBM_AUTO_INC_ENV_T;

typedef struct
{
    UINT32 u4MainPipMaxSize;
    UINT32 u4MainPopMaxSize;
    UINT32 u4SubPipMaxSize;
    UINT32 u4SubPopMaxSize;
    UINT32 u4MainPipMaxEnd;
    UINT32 u4MainPopMaxEnd;
    UINT32 u4SubPipMaxEnd;
    UINT32 u4SubPopMaxEnd;
    UINT8 u1MainPipMaxMode;
    UINT8 u1MainPopMaxMode;
    UINT8 u1SubPipMaxMode;
    UINT8 u1SubPopMaxMode;
} FBM_VIDEO_MAX_T;

FBM_POOL_T _FBM_NPTV_VIDEO_CONF[FBM_TYPE_ENV_MAX][FBM_VIDEO_AGENT_MAX];
/* get the max range of main and sub video agents */
FBM_VIDEO_MAX_T _FBM_NPTV_VideoRange;

// for Pscan testing
BOOL fgFBM_PscanTest=0;

void FBM_SetPscanTestMode(BOOL fgIsTestMode)
{
    //Printf("---------FBM_SetPscanTestMode: %x----------\n",fgIsTestMode);
    fgFBM_PscanTest=fgIsTestMode;
}

static FBM_AUTO_INC_ENV_T* _FbmGetEnv(FBM_AUTO_INC_ENV_T *env)
{
    UINT8 u1fgPIP = SRM_IsPipVideo();
    UINT8 u1fgPOP = SRM_IsPopVideo();
    UINT8 u1fgSingle = !(u1fgPIP || u1fgPOP);
    UINT8 u1mode[2];
    UINT8 u1Interlace[2];
    UINT8 u1IsMPEG[2] = {0, 0};
    UINT8 u1IsTDC[2] = {0, 0};
    SRM_VDP_INFO_T* prVdpInfo[2];
    UINT32 u4Resolution[2];

    prVdpInfo[VDP_1] = SRM_GetVdpInfo(VDP_1);
    prVdpInfo[VDP_2] = SRM_GetVdpInfo(VDP_2);

    u4Resolution[VDP_1] = prVdpInfo[VDP_1]->u4Resolution;
    if (prVdpInfo[VDP_1]->u4Interlace == 1)
    {
        u1Interlace[VDP_1] = 1;
        if (prVdpInfo[VDP_1]->u4Resolution < SRM_VDP_720HD_RESOLUTION)
        {
            u1mode[VDP_1] = FBM_MODE_576I;
        }
        else
        {
            u1mode[VDP_1] = FBM_MODE_1080I;
        }
    }
    else
    {
        u1Interlace[VDP_1] = 0;
        if (prVdpInfo[VDP_1]->u4VgaTiming == 1)
        {
            u1mode[VDP_1] = FBM_MODE_1080P_444;
        }
        else
        {
            u1mode[VDP_1] = FBM_MODE_1080P_422;
        }
    }

    u4Resolution[VDP_2] = prVdpInfo[VDP_2]->u4Resolution;
    if (prVdpInfo[VDP_2]->u4Interlace == 1)
    {
        u1Interlace[VDP_2] = 1;
        if (prVdpInfo[VDP_2]->u4Resolution < SRM_VDP_720HD_RESOLUTION)
        {
            u1mode[VDP_2] = FBM_MODE_576I;
        }
        else
        {
            u1mode[VDP_2] = FBM_MODE_1080I;
        }
    }
    else
    {
        u1Interlace[VDP_2] = 0;
        if (prVdpInfo[VDP_2]->u4VgaTiming == 1)
        {
            u1mode[VDP_2] = FBM_MODE_1080P_444;
        }
        else
        {
            u1mode[VDP_2] = FBM_MODE_1080P_422;
        }
    }

    /**
     * manual scan mode workaround: during manual scan, these scpos events
     * will confuse the fbm configuration.
     */
    if (SRM_IsScanMode(VDP_1))
    {
        u1mode[VDP_1] = FBM_MODE_576I;
    }
    if (SRM_IsScanMode(VDP_2))
    {
        u1mode[VDP_2] = FBM_MODE_576I;
    }

    if (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_DTV)
    {
        u1IsMPEG[VDP_1] = 1;
    }
    if (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_DTV)
    {
        u1IsMPEG[VDP_2] = 1;
    }

    if ((prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_ATV) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_ATD) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_SV) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_CVBS) ||
            (prVdpInfo[VDP_1]->u4Source == (UINT32)VSS_SCART))
    {
        u1IsTDC[VDP_1] = 1;
    }
    if ((prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_ATV) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_ATD) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_SV) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_CVBS) ||
            (prVdpInfo[VDP_2]->u4Source == (UINT32)VSS_SCART))
    {
        u1IsTDC[VDP_2] = 1;
    }

    env->u1fgPIP = u1fgPIP;
    env->u1fgPOP = u1fgPOP;
    env->u1fgSingle = u1fgSingle;
    env->u1mode[0] = u1mode[0];
    env->u1mode[1] = u1mode[1];
    env->u1Interlace[0] = u1Interlace[0];
    env->u1Interlace[1] = u1Interlace[1];
    env->u1IsMPEG[0] = u1IsMPEG[0];
    env->u1IsMPEG[1] = u1IsMPEG[1];
    env->u1IsTDC[0] = u1IsTDC[0];
    env->u1IsTDC[1] = u1IsTDC[1];
    env->u4Resolution[0] = u4Resolution[0];
    env->u4Resolution[1] = u4Resolution[1];
    env->u4InputWidth[0]  = prVdpInfo[0]->u4InputWidth;
    env->u4InputWidth[1]  = prVdpInfo[1]->u4InputWidth;
    env->u4InputHeight[0] = prVdpInfo[0]->u4InputHeight;
    env->u4InputHeight[1] = prVdpInfo[1]->u4InputHeight;
    return env;
}


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#if defined(CC_DYNAMIC_FBMSRM_CONF)
static UINT32 _srmfbm_conf_value = FBM_MEM_CFG_MT5395_NOMJC;
//static UINT32 _srmfbm_conf_value = FBM_MEM_CFG_MT5395_3DTV;
//static UINT32 _srmfbm_conf_value = FBM_MEM_CFG_MT5395_DDR3x2_EU;
void SRMFBM_SetConf(UINT32 conf)
{
    _srmfbm_conf_value = conf;
}
UINT32 SRMFBM_GetConf(void)
{
    return _srmfbm_conf_value;
}
#else
UINT32 SRMFBM_GetConf(void)
{
    return DRVCUST_OptGet(eFbmMemSize);
}
#endif

/** _FbmConfigurePool_MT5388_VERF_128MB
 *  Initialize this FBM pool of model MT5388_VERF_128MB
 *  @retval NONE
 */
static VOID _FbmConfigurePool_MT5388_VERF_128MB(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    // arrange root pool ---------------------------------------------------

    // DMX | MPEG | PIP | MDDI | DSP

    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
    {
#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((0x1000000), FBM_DMX_SIZE_ALIGMENT);
#else
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#endif
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT);
    }
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;

    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;

    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_SUPPORT_SCART_OUT_IN_MMP)
    // allocate 7MB for sub modules, no consider flip/mirror
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2F00000;
#elif defined(CC_FLIP_MIRROR_SUPPORT)
#ifdef CC_MIRROR_SW_WA
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x3400000;
#else
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
#endif
#else
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2500000;
#endif
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_MPEG;

    // dsp
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
    {
#if defined(CC_SUPPORT_5_SEC_PTS_PCR_OFFSET)
        prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DUAL_DSP_POOL_SIZE + 0x400000;
#elif defined(CC_SUPPORT_2_SEC_PTS_PCR_OFFSET)
        prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DUAL_DSP_POOL_SIZE + 0x1A0000;
#else
        prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DUAL_DSP_POOL_SIZE;
#endif
    }

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = FBM_POOL_TYPE_DSP;

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_PQ_TOOL;

    // cyj.todo: check if feeder is overwritten
    // feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DSP;

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
    //
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;

    // JPEG (Between Start & OSD1)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_JPEG_OSD4;

#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)||defined(CC_MT5389)
static VOID _FbmConfigurePool_MT5395_MJC_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)||defined(CC_MT5389)
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
#endif
    // arrange root pool ---------------------------------------------------

    // DMX | MPEG | PIP | MDDI | DSP

    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2
#if defined(CC_MT5365)
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB
#elif defined(CC_MT5395)  || defined(CC_MT5396)
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ
#endif
#if defined(CC_MT5396)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
#endif
#if defined(CC_MT5368)||defined(CC_MT5389)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP
#if defined(CC_MT5365)
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV
#endif
#endif
       )
    {
#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((0x1000000), FBM_DMX_SIZE_ALIGMENT);
#else
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x100000;
#endif
#endif

#if defined(CC_FBM_INT_TWO_FBP)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize * 2;
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x200000;
#endif
#endif
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT);
    }
	#if defined(CC_FBM_SUPPORT_PVR)
	   prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
	   prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;
	
	   prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_INHERIT;
	   prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
	   prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
	   prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;
    #else
	   prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
	   prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;
    #endif
   
    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;

	#ifdef CC_FBM_3D_OSD
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPE_VDP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = FBM_POOL_TYPE_TOTAL;
	#endif
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_SUPPORT_SCART_OUT_IN_MMP)
    // allocate 7MB for sub modules, no consider flip/mirror
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2F00000;
#elif defined(CC_FLIP_MIRROR_SUPPORT) && !defined(CC_MT5395) && !defined(CC_MT5396)
#if defined(CC_MT5365)
    if((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB))
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1c42000;//0x1780000;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2900000;
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1E00000; // for MT5366:FBM_MEM_CFG_MT5363_64MBx2
    }
#elif defined(CC_MT5368) || defined(CC_MT5389)
    if((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1))
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1480000;//0x1780000;
         #if defined(MT5366_SUPPORT_3D_PR)
       if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
     	  {
     		  prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize += FBM_ADD_PQ_BUFFER_SIZE;
     	  }
	   #endif
	   #if defined(CC_FLIP_MIRROR_SUPPORT)
	   
       prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize += 0x400000;
	   #endif
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1E00000;
    }

#else // mt5363, mt5387
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
#endif
#else
#if defined(CC_MT5395) || defined(CC_MT5396)
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
       )
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1500000;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1E00000;
    }

    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
    {
#if defined(CC_FBM_NO_SUBPATH)
        //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2E80000;
#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2480000;
#else
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2280000;
#endif
#else
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2900000 + 0xA00000;
#endif
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2900000;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2900000;
    }
#if defined(CC_MT5365)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2900000 + 0xA00000;
    }
#endif
    else // for ic-verification
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2400000 + 0x80000; /* add extra 500K to avoid sub buffer overlapped with dsp in SLT environment */
    }
#elif defined(CC_MT5365)
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB))
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1500000;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2900000;
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2900000 + 0xA00000;
    }
    else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1600000;
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1E00000; // 0x1300000
    }
#elif defined(CC_MT5396)
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1500000;
    }
#elif defined(CC_MT5368)||defined(CC_MT5389)
    if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
       )
    {
        //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1500000;
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2400000;
        //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2c00000;
    }
  else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1200000;
#if defined(MT5366_SUPPORT_3D_PR)
	    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize += FBM_ADD_PQ_BUFFER_SIZE;
#endif
  	}
#else // MT5363, MT5387
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2500000;
#endif
#endif
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_MPEG;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;
#if defined(CC_IC_VERIFY_FBM_MODE)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
#endif

#if (defined(CC_MT5395) || defined(CC_MT5396)) && defined(CC_FBM_2ND_CHANNEL)
    if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
#if defined(CC_MT5396)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
#endif
       )
    {
        // 2nd channel
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_INHERIT;
        prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_TOTAL2;
#if defined(CC_FBM_MT5395_DDR2x2_NOMJC)
        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0; // 46MB
        u4AheadType = FBM_POOL_TYPE_MPEG;
#else
        // 1st channel
        prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_MT5396)
        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x7800000; // 120MB
#else
        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x2E00000; // 46MB
#endif
        prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
        prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = FBM_POOL_TYPE_MPEG;

        u4AheadType = FBM_POOL_TYPE_MJC_1;
#endif
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
    {
        // 2nd channel
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_INHERIT;
        prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_TOTAL2;

        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0;
        u4AheadType = FBM_POOL_TYPE_MPEG;
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET))
    {
        // 2nd channel
        prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_INHERIT;
#if defined(CC_MT5396)
        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x7800000; // 120MB
#else
        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x2E00000; // 46MB
#endif
        prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
        prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
        prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = FBM_POOL_TYPE_TOTAL2;

        u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
#if defined(CC_MT5365)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
#endif
            )
    {
        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x0;
    }
#endif

#if defined(SUPPORT_DIIVA)
    prPoolList[FBM_POOL_TYPE_DIVA].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DIVA].u4PoolSize = FBM_SCPOS_8BIT_SIZE_FLIP(1920, 1080, 4);
    prPoolList[FBM_POOL_TYPE_DIVA].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_DIVA].u4Height = 1080;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DIVA;
#endif

#if !defined(CC_MT5395) && !defined(CC_MT5396) && !defined(CC_MT5368) && !defined(CC_MT5389)
    // dsp
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
    {
#if defined(CC_SUPPORT_5_SEC_PTS_PCR_OFFSET)
        prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x400000;
#elif defined(CC_SUPPORT_2_SEC_PTS_PCR_OFFSET)
        prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x1A0000;
#else
        prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
#endif
    }
    u4AheadType = FBM_POOL_TYPE_DSP;
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_DSP;
    }
#endif

    //#if defined(CC_MT5395) && defined(CC_FBM_2ND_CHANNEL)
    //if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
    //{
    //prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4AheadType;
    //}
    //#endif

#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_DMX2;
    }
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2
#if defined(CC_MT5365)
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1
#elif defined(CC_MT5395) || defined(CC_MT5396)
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET
            || SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ
#if defined(CC_MT5396)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
#endif
#endif
#if defined(CC_MT5368)||defined(CC_MT5389)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
#endif
       )
    {
#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((0x1000000), FBM_DMX_SIZE_ALIGMENT);
#else
        prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x100000;
#endif
#endif
    }
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;

    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x1780000; // Jason rewrite
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = FBM_POOL_TYPE_DMX2;
    u4AheadType = FBM_POOL_TYPE_MPEG2;

    prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = FBM_POOL_TYPE_MPEG2;
    u4AheadType = FBM_POOL_TYPE_FEEDER2;
#endif
    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_PQ_TOOL;
    }

#if defined(CC_53XX_FACE_DETECTION)
    prPoolList[FBM_POOL_TYPE_FACEDET].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FACEDET].u4PoolSize = FBM_FD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FACEDET;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_SWDMX)
    prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif

#if 0//defined(CC_FBM_SUPPORT_PVR)
    prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PVR;
#endif

    prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMXPID;

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)||defined(CC_MT5389)
#if (defined(CC_MT5395) || defined(CC_MT5396)) && defined(CC_SUPPORT_VENC)
    prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VENC;
#endif

    u4BehindType = FBM_POOL_TYPE_NS;
#if  0//def CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
    prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4BehindType;
    u4BehindType = FBM_POOL_TYPE_OD;
#endif

#if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) 
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
    {
        prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
        prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x1200000; // 18MB
        prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_FRC_Y08_C08_444;
        prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_BEFORE;
        prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = u4BehindType;

        u4BehindType = FBM_POOL_TYPE_MJC_1;

    }

    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4BehindType;
#endif
#endif

    // cyj.todo: check if feeder is overwritten
    // feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4VdoBehindType;

#if defined(CC_FBM_NO_SUBPATH)
#if defined(CC_DYNAMIC_MPEG_SIZE)
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DSP;
#else
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_MPEG;
#endif
#endif

#if defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize * 2;
#endif

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
    //
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    #if defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize * 2;
    #endif
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4VdoBehindType;
    #ifndef CC_FBM_3D_OSD
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
    #if !defined(CC_REMOVE_JPEG_OSD34)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
	
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
    #else
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
    #endif
	#else
	u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
	#endif
    // JPEG (Between Start & OSD1)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;

#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

#ifdef CC_MT5396
static VOID _FbmConfigurePool_MT5396_MJC_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMX | MPEG | PIP | MDDI | DSP

    //----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
       )
    {
#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((0x1000000), FBM_DMX_SIZE_ALIGMENT);
#else
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x100000;
#endif
#endif

#if defined(CC_FBM_INT_TWO_FBP)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize * 2;
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x200000;
#endif
#endif
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT);
    }
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;


    //----- MPEG-----
    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;

#ifdef CC_FBM_3D_OSD
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPE_VDP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = FBM_POOL_TYPE_TOTAL;
#endif
    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_SUPPORT_SCART_OUT_IN_MMP)
    // allocate 7MB for sub modules, no consider flip/mirror
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2F00000;
#endif

    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
            )
    {
        //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1500000;
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000+0xaf2000;
    }
    else if((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC))
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2A00000; 
    }
    else // for ic-verification
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2400000 + 0x80000; /* add extra 500K to avoid sub buffer overlapped with dsp in SLT environment */
    }

    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_MPEG;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;
#if defined(CC_IC_VERIFY_FBM_MODE)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
#endif

#if defined(CC_SUPPORT_VSS)
    prPoolList[FBM_POOL_TYPE_VSS].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VSS].u4PoolSize = FBM_VSS_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VSS].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VSS].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VSS;
#endif

#if defined(SUPPORT_DIIVA)
    prPoolList[FBM_POOL_TYPE_DIVA].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DIVA].u4PoolSize = FBM_SCPOS_8BIT_SIZE_FLIP(1920, 1080, 4);
    prPoolList[FBM_POOL_TYPE_DIVA].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_DIVA].u4Height = 1080;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DIVA;
#endif


    #if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST) 
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_DMX2;
    }
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
       )
    {
#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((0x1000000), FBM_DMX_SIZE_ALIGMENT);
#else
        prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#if defined(CC_SSIF_SUPPORT)
	   #if defined(CC_VOMX_TV_COEXIST)
	   prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x500000;
	   #else
	   prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize += 0x500000;
#endif
#endif
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x100000;
#endif
#endif
    }
    	if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
		{
			    prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize += 0xA00000;
		}
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMX2;

    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
    //prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    #if defined(CC_VOMX_TV_COEXIST)
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
    //prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x2F00000; // 23.5Mx2
    prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x1780000; // Jason rewrite
    #else
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x1780000; // Jason rewrite
    #endif
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG2;

    #if defined(CC_VOMX_TV_COEXIST)
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG3].u4Mode = (FBM_POOL_MODE_MPEG_DBK);
    prPoolList[FBM_POOL_TYPE_MPEG3].u4PoolSize = 0x1780000;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG3].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MPEG3;
    #endif

    prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FEEDER2;
#endif
    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_PQ_TOOL;
    }
    //----- MJC-----
#if defined(CC_FBM_2ND_CHANNEL) // 2 Channel
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV))
    {
    prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x7800000; // 120MB
    prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
    #if 1
    prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_BEFORE;
    #else
    prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
    #endif
    prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = FBM_POOL_TYPE_TOTAL2;
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC))
    {
    	//prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x0;
    	prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
    	prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x1680000; // 21MB
    	prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_FRC_Y10_C10_444;
    	prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
    	prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = u4AheadType;
    	u4AheadType = FBM_POOL_TYPE_MJC_1;
    }
#else // 1 channel
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV))
    {
    prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x7800000; // 120MB
    prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
    prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_MJC_1;
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC))
    {
    	//prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x0;
    	prPoolList[FBM_POOL_TYPE_MJC_1].u4Inherit = FBM_POOL_ROOT;
    	prPoolList[FBM_POOL_TYPE_MJC_1].u4PoolSize = 0x1680000; // 21MB
    	prPoolList[FBM_POOL_TYPE_MJC_1].u4Mode= FBM_POOL_MODE_FRC_Y10_C10_444;
    	prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].eMode = FBM_POOL_ARRANGE_AFTER;
    	prPoolArrangeInfo[FBM_POOL_TYPE_MJC_1].ePool1 = u4AheadType;
    	u4AheadType = FBM_POOL_TYPE_MJC_1;
    }
#endif
#if defined(CC_53XX_FACE_DETECTION)
    prPoolList[FBM_POOL_TYPE_FACEDET].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FACEDET].u4PoolSize = FBM_FD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FACEDET;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_SWDMX)
    prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif

#if FBM_DMX_PID_BUF!=0
    prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = FBM_DMX_PID_BUF;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMXPID;
#endif

#if defined(CC_FBM_SUPPORT_PVR)
    prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PVR;
#endif

    prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMXPID;
    
    prPoolList[FBM_POOL_TYPE_DRM].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DRM].u4PoolSize = FBM_DRM_BUF_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DRM].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DRM].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DRM;

#if defined(CC_SUPPORT_VENC)
    prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VENC;
#endif

    u4BehindType = FBM_POOL_TYPE_NS;
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
    prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4BehindType;
    u4BehindType = FBM_POOL_TYPE_OD;
#endif

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4BehindType;
#if defined(CC_SUPPORT_5_SEC_PTS_PCR_OFFSET)
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x400000;
#elif defined(CC_SUPPORT_2_SEC_PTS_PCR_OFFSET)
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x1A0000;
#else
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE;
#endif


    // cyj.todo: check if feeder is overwritten
    // feeder
    #if defined(CC_VOMX_TV_COEXIST)
    prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FEEDER;
    #else
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4VdoBehindType;
    #endif

#if defined(CC_FBM_NO_SUBPATH)
#if defined(CC_DYNAMIC_MPEG_SIZE)
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DSP;
#else
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_MPEG;
#endif
#endif

#if defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize * 2;
#endif

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
    //
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
#if defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize * 2;
    #endif
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4VdoBehindType;

#ifndef CC_FBM_3D_OSD
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
	
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
	u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif

    // JPEG (Between Start & OSD1)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;

#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

#endif

#if defined(CC_MT5368)|| defined(CC_MT5389)
static VOID  _FbmConfigurePool_MT5368_SCPOS_MAIN(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMX | MPEG | PIP | MDDI | DSP

    //----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
            ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
            ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
       )
    {
#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((0x1000000), FBM_DMX_SIZE_ALIGMENT);
#else
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x100000;
#endif
#endif

#if defined(CC_FBM_INT_TWO_FBP)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize * 2;
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x200000;
#endif
#endif
    }
    else if((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK))
    {
    	#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE)+0x640000, FBM_DMX_SIZE_ALIGMENT); // new spec, video need to queue 6.24M
    	#else
    	prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
    	#endif
    }
    else
    {
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT);
    }
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;


    //----- MPEG-----
    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;

#ifdef CC_FBM_3D_OSD
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPE_VDP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = FBM_POOL_TYPE_TOTAL;
#endif
    //----- SCPOS MAIN-----
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_SUPPORT_SCART_OUT_IN_MMP)
    // allocate 7MB for sub modules, no consider flip/mirror
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2F00000;
#endif

    if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
            ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
       )
    {
        //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1500000;
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
    }
    else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
    {
        //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2200000; // 34M
        #if defined(CC_FBM_3DTV_NOSUB)
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1B00000+0x600000;
        #else
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2800000+0x600000;
        #endif
    }
    else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1500000; // main 12M, sub 9M
    }
    else // for ic-verification
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2400000 + 0x80000; /* add extra 500K to avoid sub buffer overlapped with dsp in SLT environment */
    }

    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_MPEG;
    u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;
#if defined(CC_IC_VERIFY_FBM_MODE)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2b00000;
#endif

#if defined(SUPPORT_DIIVA)
    prPoolList[FBM_POOL_TYPE_DIVA].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DIVA].u4PoolSize = FBM_SCPOS_8BIT_SIZE_FLIP(1920, 1080, 4);
    prPoolList[FBM_POOL_TYPE_DIVA].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_DIVA].u4Height = 1080;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVA].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DIVA;
#endif


#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_DMX2;
    }
    prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
    if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
            ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
            ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
       )
    {
#ifdef CC_SUPPORT_5_SEC_PTS_PCR_OFFSET
        prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((0x1000000), FBM_DMX_SIZE_ALIGMENT);
#else
        prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#if defined(CC_3D_MM_DMX_ADD_2MB)
        prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x100000;
#endif
#endif
    }
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;

    prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x1780000; // Jason rewrite
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = FBM_POOL_TYPE_DMX2;
    u4AheadType = FBM_POOL_TYPE_MPEG2;

    prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = FBM_POOL_TYPE_MPEG2;
    u4AheadType = FBM_POOL_TYPE_FEEDER2;
#endif
    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_TOOL;
    if (u4VdoBehindType == FBM_POOL_TYPE_NS)
    {
        u4VdoBehindType = FBM_POOL_TYPE_PQ_TOOL;
    }

#if defined(CC_53XX_FACE_DETECTION)
    prPoolList[FBM_POOL_TYPE_FACEDET].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FACEDET].u4PoolSize = FBM_FD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FACEDET].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_FACEDET;
#endif

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_SWDMX)
    prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif
    prPoolList[FBM_POOL_TYPE_DMXPID].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_DMXPID].u4PoolSize = 0x400000;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMXPID].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMXPID;

#if defined(CC_FBM_SUPPORT_PVR)
    prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PVR;
#endif

#if defined(CC_SUPPORT_VENC)
    prPoolList[FBM_POOL_TYPE_VENC].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_VENC].u4PoolSize = FBM_VENC_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VENC].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_VENC;
#endif

    u4BehindType = FBM_POOL_TYPE_NS;
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
    prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4BehindType;
    u4BehindType = FBM_POOL_TYPE_OD;
#endif

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4BehindType;
    #if defined(CC_SUPPORT_5_SEC_PTS_PCR_OFFSET)
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DSP_POOL_SIZE + 0x28000;
    #endif


    // cyj.todo: check if feeder is overwritten
    // feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4VdoBehindType;

    // DIVX use sub path since it's only for MM
    #if defined(DIVX_PLUS_CER) || defined(CC_NETFLIX_CE3_SUPPORT)
     prPoolList[FBM_POOL_TYPE_DIVX].u4PoolSize = FBM_DIVX_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVX].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DIVX].ePool1 = FBM_POOL_TYPE_FEEDER;
    #endif

#if defined(CC_FBM_NO_SUBPATH)
#if defined(CC_DYNAMIC_MPEG_SIZE)
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DSP;
#else
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_MPEG;
#endif
#endif

#if defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize * 2;
#endif

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
    //
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    #if defined(CC_FBM_INT_TWO_FBP)
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize * 2;
    #endif
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4VdoBehindType;

#ifndef CC_FBM_3D_OSD
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
	
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
	u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif    

    // JPEG (Between Start & OSD1)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;

#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

static VOID _FbmConfigurePool_MT5501_Buffer(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN, u4BehindType, u4VdoBehindType = FBM_POOL_TYPE_NS;
    UNUSED(u4AheadType);
    UNUSED(u4BehindType);
    UNUSED(u4VdoBehindType);
    // arrange root pool ---------------------------------------------------

    // DMX | MPEG | PIP | MDDI | DSP
    
   //MM mode
	
    //----- MPEG-----
    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_TOTAL;
    u4AheadType = FBM_POOL_TYPE_MPEG;

	//----- DMX-----
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;	 
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);	 
#if defined(CC_SSIF_SUPPORT)
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize += 0x500000;
#endif
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_DMX;
	
    //----- PQ MM-----
    prPoolList[FBM_POOL_TYPE_PQ_MM].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PQ_MM].u4PoolSize = 0xD00000;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_MM].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_MM].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PQ_MM;

#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)
    
#ifdef CC_FBM_3D_OSD
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4Inherit = FBM_POOL_INHERIT;
	prPoolList[FBM_POOL_TYPE_JPG_VDP].u4PoolSize = FBM_JPE_VDP_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_JPG_VDP].ePool1 = u4AheadType;
#endif	

	prPoolList[FBM_POOL_TYPE_DMX2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
#if defined(CC_SSIF_SUPPORT)
    prPoolList[FBM_POOL_TYPE_DMX2].u4PoolSize += 0xD00000;
#endif
	prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_DMX2].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_DMX2;
	
	prPoolList[FBM_POOL_TYPE_MPEG2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_MPEG2].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
	prPoolList[FBM_POOL_TYPE_MPEG2].u4PoolSize = 0x1780000; // Jason rewrite
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_MPEG2].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_MPEG2;
	
	prPoolList[FBM_POOL_TYPE_FEEDER2].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_FEEDER2].u4PoolSize = FBM_FEEDER_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].eMode = FBM_POOL_ARRANGE_AFTER;
	prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER2].ePool1 = u4AheadType;
	u4AheadType = FBM_POOL_TYPE_FEEDER2;
#endif

    u4BehindType = FBM_POOL_TYPE_NS;
	
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == TRUE)
	prPoolList[FBM_POOL_TYPE_OD].u4Inherit = FBM_POOL_ROOT;
	prPoolList[FBM_POOL_TYPE_OD].u4PoolSize = FBM_OD_POOL_SIZE;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_OD].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_OD;
#endif

#if defined(CC_FBM_SUPPORT_SWDMX)
		prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
		prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
		prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_BEFORE;
		prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4BehindType;
		u4BehindType = FBM_POOL_TYPE_SWDMX;
#endif
	
#if defined(CC_FBM_SUPPORT_PVR)
	   prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
	   prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
	   prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_BEFORE;
	   prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4BehindType;
	   u4BehindType = FBM_POOL_TYPE_PVR;
#endif

    // pq tool
	prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_PQ_TOOL;
	
	// vbi
	prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
	prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_VBI;

    //-----DSP-----
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_DSP;

    //----- PQ TV-----
	prPoolList[FBM_POOL_TYPE_PQ_TV].u4Inherit = FBM_POOL_INHERIT;
	prPoolList[FBM_POOL_TYPE_PQ_TV].u4PoolSize = 0x2800000;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TV].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TV].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_PQ_TV;
#if 0
	//----- DMX TV-----
	prPoolList[FBM_POOL_TYPE_DMX_TV].u4Inherit = FBM_POOL_INHERIT;
	prPoolList[FBM_POOL_TYPE_DMX_TV].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
	prPoolArrangeInfo[FBM_POOL_TYPE_DMX_TV].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DMX_TV].ePool1 = u4BehindType;
#endif
	u4BehindType = FBM_POOL_TYPE_DSP;

   	// feeder_NET
    prPoolList[FBM_POOL_TYPE_FEEDER_NET].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_NET].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_NET].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_FEEDER_NET;
	
    //----- PQ NET-----
	prPoolList[FBM_POOL_TYPE_PQ_NET].u4Inherit = FBM_POOL_INHERIT;
	prPoolList[FBM_POOL_TYPE_PQ_NET].u4PoolSize = 0xD00000;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_NET].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_PQ_NET].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_PQ_NET;
#if 0
		//----- DMX NET-----
	prPoolList[FBM_POOL_TYPE_DMX_NET].u4Inherit = FBM_POOL_INHERIT;
	prPoolList[FBM_POOL_TYPE_DMX_NET].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_H264_SIZE), FBM_DMX_SIZE_ALIGMENT);
	prPoolArrangeInfo[FBM_POOL_TYPE_DMX_NET].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_DMX_NET].ePool1 = u4BehindType;
#endif	
	//----- NET SWAP-----
	prPoolList[FBM_POOL_TYPE_NET_SWAP].u4Inherit = FBM_POOL_INHERIT;
	prPoolList[FBM_POOL_TYPE_NET_SWAP].u4PoolSize = FBM_ALIGN_MASK((FBM_MEM_CFG_MT5501_3DNET - 0x4540000 +
#if defined(DIVX_PLUS_CER) || defined(CC_NETFLIX_CE3_SUPPORT)
    0
#else
    (0x600000)
#endif
    +
#if defined(CC_FBM_SUPPORT_PVR)
    0
#else
    (0x380000)
#endif
    ), 0x100000);
    prPoolArrangeInfo[FBM_POOL_TYPE_NET_SWAP].eMode = FBM_POOL_ARRANGE_BEFORE;
	prPoolArrangeInfo[FBM_POOL_TYPE_NET_SWAP].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_NET_SWAP;


	u4BehindType = FBM_POOL_TYPE_DSP;
// feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = u4BehindType;
	u4BehindType = FBM_POOL_TYPE_DSP;
    // handle Image Viewer Pool
    #ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
    //
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = u4BehindType;

#ifndef CC_FBM_3D_OSD
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_FEEDER_MMP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;
#if !defined(CC_REMOVE_JPEG_OSD34)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;
	
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD4;
#else
    u4AheadType = FBM_POOL_TYPE_JPEG_OSD2;
#endif
#else
	u4AheadType = FBM_POOL_TYPE_FEEDER_MMP;
#endif

    // JPEG (Between Start & OSD1)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = u4AheadType;

    #else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
    #endif
}
static VOID _FbmConfigurePool_MT5368_64MBx1(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    FBM_POOL_TYPE_T u4AheadType = FBM_POOL_TYPE_SCPOS_MAIN;
    UNUSED(u4AheadType);
    // dmx
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT);
    // mpeg
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;
    // feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
#if defined(CC_DYNAMIC_MPEG_SIZE)
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DSP;
#else
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_MPEG;
#endif

    // arrange root pool ---------------------------------------------------
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;

    //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x22c0000; // worst case: DTV

    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x22c0000; // worst case: DTV
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264)
    {
        //#if defined(CC_FBM_SHRINK_64Mx1_NOH264)
        //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1880000; // worst case: DTV
        //#else
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1A80000; // worst case: DTV
        //#endif
    }

    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_TOTAL;

    // dsp
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = FBM_POOL_TYPE_DSP;

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_PQ_TOOL;
    u4AheadType = FBM_POOL_TYPE_VBI;

#if defined(CC_FBM_SUPPORT_SWDMX)
    prPoolList[FBM_POOL_TYPE_SWDMX].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SWDMX].u4PoolSize = FBM_SWDMX_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SWDMX].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_SWDMX;
#endif

#if defined(CC_FBM_SUPPORT_PVR)
    prPoolList[FBM_POOL_TYPE_PVR].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_PVR].u4PoolSize = FBM_PVR_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PVR].ePool1 = u4AheadType;
    u4AheadType = FBM_POOL_TYPE_PVR;
#endif

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2  (Share all except DSP)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;


    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_MMP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_JPEG;
#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

#endif


#endif

/** _FbmConfigurePool_MT5363_VERF_64MB
 *  Initialize this FBM pool of model MT5362B_VERF_64MB
 *  @retval NONE
 */
static VOID _FbmConfigurePool_MT5363_VERF_64MB(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    // arrange root pool ---------------------------------------------------

    // PIP | SCPOS | NR | MDDI_PIP | DSP
    // mpeg
    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#ifdef CC_FLIP_MIRROR_SUPPORT
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2200000;
#else
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1100000;
#endif
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_DMX;

    // feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DSP;

    // dsp
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MB)
    {
        prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DUAL_DSP_POOL_SIZE;
    }
    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = FBM_POOL_TYPE_DSP;

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_PQ_TOOL;

    // dmx
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_MPEG;

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2  (Share all except DSP)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;

    // JPEG (Between Start & OSD1)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_MMP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_JPEG;
#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

static VOID _FbmConfigurePool_MT5365_ATV_64MB(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    // arrange root pool ---------------------------------------------------
// scpos-m/ di-m/scpos-p/di-p/
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1A00000;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_TOTAL;


    // dsp
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;
    prPoolList[FBM_POOL_TYPE_DSP].u4PoolSize = FBM_DUAL_DSP_POOL_SIZE;

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = FBM_POOL_TYPE_DSP;

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_PQ_TOOL;

    //MPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DSP;

    // dmx
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_MPEG;

    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DMX;

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2  (Share all except DSP)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;

    // JPEG (Between Start & OSD1)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_MMP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_JPEG;
#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

/** _FbmConfigurePool_MT5387_VERF_64MB
 *  Initialize this FBM pool of model MT5362B_VERF_64MB
 *  @retval NONE
 */
static VOID _FbmConfigurePool_MT5387_VERF_64MB(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    // dmx
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;
    // mpeg
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;
    // feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_MPEG;

    // arrange root pool ---------------------------------------------------
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
#if defined(CC_FLIP_MIRROR_SUPPORT)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2600000;
#elif defined(CC_VIDEO_THUMB_NAIL)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2500000;
    //#elif defined(CC_FLIP_MIRROR_SUPPORT)
    //prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x2200000;
#else
#if defined(CC_MT8223)
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1C00000;
#else
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1E00000 + 0x100000;
#endif
#endif
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_TOTAL;

    // dsp
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = FBM_POOL_TYPE_DSP;

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_PQ_TOOL;

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
#if defined(CC_MT8223)
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_DSP;
    prPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize = 0x200000;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_JPEG;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;

    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_MMP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;
#else
    // JPEG | OSD1 | OSD2  (Share all except DSP)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;

    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_MMP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}

#if defined(CC_MT5365)
static VOID _FbmConfigurePool_MT5365_64MBx1(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    // dmx
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;
    prPoolList[FBM_POOL_TYPE_DMX].u4PoolSize = FBM_ALIGN_MASK((FBM_DMX_SIZE), FBM_DMX_SIZE_ALIGMENT);
    // mpeg
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;
    // feeder
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
#if defined(CC_DYNAMIC_MPEG_SIZE)
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DSP;
#else
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_MPEG;
#endif

    // arrange root pool ---------------------------------------------------
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
    {
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x22c0000; // worst case: DTV
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1_NOH264)
    {
#if defined(CC_FBM_SHRINK_64Mx1_NOH264)
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1880000; // worst case: DTV
#else
        prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1A80000; // worst case: DTV
#endif
    }
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_TOTAL;

    // dsp
    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = FBM_POOL_TYPE_DSP;

    // vbi
    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_PQ_TOOL;

    // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
    // JPEG | OSD1 | OSD2  (Share all except DSP)
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1088, 1, 4); // FHD, 32bpp, x1
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1088;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;


    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;

    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_MMP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_JPEG;
#else
    // JPEG + B2R share DMX + MPEG
    // JPEG
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_TOTAL;

    // B2R
    prPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 2, 2); // FHD, 422, x2
    prPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_2FB;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_JPEG;
#endif
}
#endif

static VOID _FbmConfigurePool_MT5388_64MB_WXGA(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    prPoolList[FBM_POOL_TYPE_DMX].u4Inherit = FBM_POOL_INHERIT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;

    prPoolList[FBM_POOL_TYPE_FEEDER].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_FEEDER].u4PoolSize = FBM_FEEDER_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER].ePool1 = FBM_POOL_TYPE_DMX;

    prPoolList[FBM_POOL_TYPE_MPEG].u4Inherit = FBM_POOL_INHERIT;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_FEEDER;

    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Inherit = FBM_POOL_ROOT;
    prPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = 0x1B00000;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_TOTAL;

    prPoolList[FBM_POOL_TYPE_DSP].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

    // pq tool
    prPoolList[FBM_POOL_TYPE_PQ_TOOL].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_PQ_TOOL].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_VBI].u4Inherit = FBM_POOL_ROOT;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
    prPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_PQ_TOOL;

#ifdef CC_OSD_USE_FBM
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1366, 780, 1, 4);
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1366;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 780;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_DSP;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1366, 780, 1, 4);
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1366;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 780;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD1;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1366, 780, 1, 4);
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1366;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 780;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1366, 780, 1, 4);
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1366;
    prPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 780;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;

    prPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = FBM_POOL_INHERIT;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_JPEG_OSD4;

    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4Inherit = FBM_POOL_INHERIT;
    prPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize = FBM_FEEDER_MMP_POOL_SIZE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].eMode = FBM_POOL_ARRANGE_BEFORE;
    prPoolArrangeInfo[FBM_POOL_TYPE_FEEDER_MMP].ePool1 = FBM_POOL_TYPE_JPEG;
#else
#endif
}

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#define UPDATE_PRPOOL(type, module) \
    do {\
        u4NextDramAddr = FBM_ALIGN_MASK(u4NextDramAddr, FBM_ ## module ## _ADDR_ALIGMENT);\
        prPool = FBM_GetPoolInfo(type);\
        prPool->u4Addr = u4NextDramAddr;\
        prPool->u4Size = u4Size;\
        prPool->u4Width = u4Width;\
        prPool->u4Height = u4Height;\
        prPool->u4Mode = u4Mode;\
        prPool->u4Inherit = FBM_POOL_INHERIT;\
        u4NextDramAddr = prPool->u4Addr + prPool->u4Size;\
    } while (0)
/* consider panel width ? */
#define UPDATE_POP_WIDTH() \
    do {\
        if (env->u1fgPOP)\
        {\
            u4Width /= 2;\
        }\
    } while (0)
#define UPDATE_HPD_WIDTH() \
    do {\
        if (u4VdpId == VDP_1)\
        {\
            if ((u4HPDSize != 0) && (u4HPDSize < u4Width))\
            {\
                u4Width = u4HPDSize;\
            }\
        }\
    } while (0)
#define CALCULATE_PROG_DISP_444_SIZE() \
    do {\
        u1NRNum = 2;\
        if (u4FlipModule & FLIP_BY_PSCAN)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_MDDI_BI_NR_10BIT_444_SIZE(u4Width, u4Height, u1NRNum);\
        }\
        else\
        {\
            u4Size = FBM_MDDI_BI_NR_8BIT_444_SIZE(u4Width, u4Height, u1NRNum);\
        }\
    } while (0)
//mt5365.mt5395 MM>720p need used can't 420->422
#define CALCULATE_PSCAN_NR_YC_SIZE() \
    do {\
        u1NRNum = 2;\
        if (u4FlipModule & FLIP_BY_PSCAN)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(u4Width, u4Height, u1NRNum);\
        }\
        else\
        {\
            u4Size = FBM_MDDI_BI_NR_8BIT_SIZE(u4Width, u4Height, u1NRNum);\
        }\
    } while (0)
#define CALCULATE_PSCAN_NR_Yonly_SIZE() \
    do {\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_MDDI_BI_NR_Y_Only_10BIT_SIZE(u4Width, u4Height);\
        }\
        else\
        {\
            u4Size = FBM_MDDI_BI_NR_Y_Only_8BIT_SIZE(u4Width, u4Height);\
        }\
    } while (0)

#if defined(CC_MT5396) ||defined(CC_MT5368)||defined(CC_MT5389)
#define CALCULATE_PROG_DISP_444_SIZE_EX() \
    do {\
        u1NRNum = 3;\
        if (u4FlipModule & FLIP_BY_PSCAN)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_MDDI_BI_NR_10BIT_444_SIZE(u4Width, u4Height, u1NRNum);\
        }\
        else\
        {\
            u4Size = FBM_MDDI_BI_NR_8BIT_444_SIZE(u4Width, u4Height, u1NRNum);\
        }\
    } while (0)
#define CALCULATE_PSCAN_NR_YC_SIZE_EX() \
    do {\
        u1NRNum = 3;\
        if (u4FlipModule & FLIP_BY_PSCAN)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            if ((env->u1IsMPEG[u4VdpId]) && (env->u4Resolution[u4VdpId] < SRM_VDP_720HD_RESOLUTION))\
            {\
                u4Size = FBM_MDDI_BI_NR_10BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
            }\
            else\
            {\
                u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(u4Width, u4Height, u1NRNum);\
            }\
        }\
        else\
        {\
            if ((env->u1IsMPEG[u4VdpId]) && (env->u4Resolution[u4VdpId] < SRM_VDP_720HD_RESOLUTION))\
            {\
                u4Size = FBM_MDDI_BI_NR_8BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
            }\
            else\
            {\
                u4Size = FBM_MDDI_BI_NR_8BIT_SIZE(u4Width, u4Height, u1NRNum);\
            }\
        }\
    } while (0)

// new rule: 20110817 by Allen
#define CALCULATE_PSCAN_NR_YC_SIZE_BEFORE_ECO() \
    do {\
    	  if(u4PscanModeEx==E_TDTV_DI_DRAM_DOUBLE)\
	  {\
	  	u1NRNum = 2;\
	  }\
	  else if(u4PscanModeEx==E_TDTV_DI_DRAM_0FB)\
	  {\
	     	u1NRNum = 0;\
	  }\
	  else\
	  {\
	        u1NRNum = 1;\
	  }\
        if (u4FlipModule & FLIP_BY_PSCAN)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_MDDI_DISP)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(u4Width, u4Height, u1NRNum);\
        }\
        else\
        {\
            u4Size = FBM_MDDI_BI_NR_8BIT_SIZE(u4Width, u4Height, u1NRNum);\
        }\
    } while (0)

#define CALCULATE_PSCAN_NR_YC_SIZE_3D() \
	do {\
    	  if(u4PscanModeEx==E_TDTV_DI_DRAM_DOUBLE)\
	  {\
	  	u1NRNum = 2;\
	  }\
	  else if(u4PscanModeEx==E_TDTV_DI_DRAM_0FB)\
	  {\
	     	u1NRNum = 0;\
	  }\
	  else\
	  {\
	        u1NRNum = 1;\
	  }\
        if (u4FlipModule & FLIP_BY_PSCAN)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_MDDI_DISP)\
        {\
            u1NRNum += 1;\
        }\
        if (u4MddiMode & FBM_POOL_MODE_10BIT)\
        {\
            if (env->u1IsMPEG[u4VdpId])\
            {\
                u4Size = FBM_MDDI_BI_NR_10BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
            }\
            else\
            {\
                u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(u4Width, u4Height, u1NRNum);\
            }\
        }\
        else\
        {\
            if (env->u1IsMPEG[u4VdpId])\
            {\
                u4Size = FBM_MDDI_BI_NR_8BIT_420_SIZE(u4Width, u4Height, u1NRNum);\
            }\
            else\
            {\
                u4Size = FBM_MDDI_BI_NR_8BIT_SIZE(u4Width, u4Height, u1NRNum);\
            }\
        }\
    } while (0)

#endif

void CALCULATE_PSCAN_YC_NUM(UINT32 u4VdpId, UINT32 u4MddiMode, UINT32 u4flip, UINT8* u1Ynum, UINT8* u1Cnum)
{
    if (u4MddiMode & FBM_POOL_MODE_MDDI_FULL)
    {
        *u1Ynum = 4;
        *u1Cnum = 4;
    }
    else if (u4MddiMode & FBM_POOL_MODE_MDDI_CBOB)
    {
        *u1Ynum = 4;
        *u1Cnum = 3;
    }
    else /* if (u4MddiMode & FBM_POOL_MODE_MDDI_BOB) */
    {
        *u1Ynum = 2;
        *u1Cnum = 2;
    }
    if ((u4VdpId == VDP_1) && (u4MddiMode & FBM_POOL_MODE_MDDI_DISP))
    {
        *u1Ynum += 1;
        *u1Cnum += 1;
        if (u4flip & FLIP_BY_PSCAN)
        {
            *u1Ynum += 1;
            *u1Cnum += 1;
        }
    }
}
#define CALCULATE_SCALER_444_PARAMETERS() \
    do {\
        u1ScalerNum = 3;\
        if (u4FlipModule & FLIP_BY_SCALER)\
        {\
            u1ScalerNum += 1;\
        }\
        if (u4FlipModule & FLIP_BY_3D)\
        {\
            u1ScalerNum += 3;\
        }\
        u4Size = FBM_SCPOS_444_3FB_POOL_SIZE_FLIP(u4Width, u4Height, u1ScalerNum);\
        u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;\
    } while (0)
#define CALCULATE_SCALER_422_PARAMETERS() \
    do {\
        u1ScalerNum = 3;\
        if (u4FlipModule & FLIP_BY_SCALER)\
        {\
            u1ScalerNum += 1;\
        }\
        if (u4FlipModule & FLIP_BY_3D)\
        {\
            u1ScalerNum += 3;\
        }\
        if (u4ScalerMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Size = FBM_SCPOS_10BIT_SIZE_FLIP(u4Width, u4Height, u1ScalerNum);\
        }\
        else\
        {\
            u4Size = FBM_SCPOS_8BIT_SIZE_FLIP(u4Width, u4Height, u1ScalerNum);\
        }\
        u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;\
        if (u4ScalerMode & FBM_POOL_MODE_10BIT)\
        {\
            u4Mode |= FBM_POOL_MODE_10BIT;\
        }\
        else\
        {\
            u4Mode |= FBM_POOL_MODE_8BIT;\
        }\
    } while (0)
#define UPDATE_EPG_WxH(u4VdpId, width, height)\
    do {\
        if (u4VdpId == VDP_1)\
        {\
            if (SRM_IsEpgMode())\
            {\
                u4Width = width;\
                u4Height = height;\
            }\
        }\
    } while (0)
#define UPDATE_WxH_BY_RESOLUTION(u4VdpId) \
    do {\
	   if (env->u4Resolution[u4VdpId] == 0)\
        {\
            u4Width = 740;\
            u4Height = 596;\
        }\
         else\
          if (env->u4Resolution[u4VdpId] <= SRM_VDP_SD_RESOLUTION)\
        {\
            u4Width = env->u4InputWidth[u4VdpId] + 20;\
            u4Height = env->u4InputHeight[u4VdpId] + 20;\
        }\
        else\
          if (env->u4Resolution[u4VdpId] <= SRM_VDP_720HD_RESOLUTION)\
        {\
            u4Width = env->u4InputWidth[u4VdpId] + 20;\
            UPDATE_POP_WIDTH();\
            u4Height = env->u4InputHeight[u4VdpId] + 20;\
        }\
        else\
       {\
            u4Width = env->u4InputWidth[u4VdpId] + 20;\
            UPDATE_HPD_WIDTH();\
            UPDATE_POP_WIDTH();\
            u4Height = env->u4InputHeight[u4VdpId] + 20;\
        }\
    } while (0)
#define UPDATE_TDC_POOL()\
    do {\
        if (env->u1mode[u4VdpId] == FBM_MODE_576I)\
        {\
            u4Width = 780;\
            u4Height = 578;\
            u4Mode = FBM_POOL_MODE_10BIT;\
            u4Size = FBM_TDC_POOL_SIZE;\
            UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);\
        }\
    } while (0)

#define UPDATE_TDC_POOL_BY_MODE()\
    do {\
        if (env->u1mode[u4VdpId] == FBM_MODE_576I)\
        {\
            u4Width = 780;\
            u4Height = 578;\
            u4Mode = FBM_POOL_MODE_10BIT;\
            if(u4TDCMode&FBM_POOL_MODE_TDC_GAME)\
            	u4Size = FBM_TDC_GAME_SIZE;\
            else\
            	u4Size = FBM_TDC_POOL_SIZE;\
            UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);\
        }\
    } while (0)

#define CHECK_PSCAN_SIZE_TO_0()\
    do {\
        if (((u4MddiMode & FBM_POOL_MODE_MDDI_DISP) == 0) && (u4MddiMode & FBM_POOL_MODE_MDDI_BOB) && (u4MddiMode & FBM_POOL_MODE_MDDI_NR_OFF))\
        {\
            u4Size = 0;\
        }\
    } while (0)
#define CHECK_SCALER_SIZE_TO_0()\
    do {\
        if ((u4ScalerMode & FBM_POOL_MODE_SCPOS_PSCAN_DISP) || (u4ScalerMode & FBM_POOL_MODE_SCPOS_DISP)|| (u4ScalerMode & FBM_POOL_MODE_SCPOS_LINESYNC_DISP))\
        {\
            u4Size = 0;\
        }\
    } while (0)

static UINT32 FBM_FlipModule[VDP_NS];

UINT32 u4QueryFlipModule(UINT32 u4VdpId)
{
    return FBM_FlipModule[u4VdpId];
}

UINT32 u4DecideFlipModule(UINT32 u4VdpId, UINT32 u4ScalerMode, UINT8 u1IsMPEG, FBM_AUTO_INC_ENV_T* env)
{
    UINT32 u4FlipModule = FLIP_NONE;
    UINT8 ucTVEEnable;

    TVE_GetEnable(TVE_1, &ucTVEEnable);

#if defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5389)
    // main: dtv, sub: tve : (FLIP_BY_B2R, FLIP_BY_SCALER)
    // main: other, sub: dtv : (FLIP_BY_xxx, FLIP_BY_SCALER)
    // main: hdmi, sub: tve : (FLIP_BY_xxx, FLIP_NONE)
    // main: other, sub: hdmi : (FLIP_BY_xxx, FLIP_BY_SCALER)
    if((u1IsMPEG) && (u4VdpId == VDP_1))
    {
        u4FlipModule = FLIP_BY_B2R;
    }
    else if (u4VdpId == VDP_2)
    {
        u4FlipModule = FLIP_BY_SCALER;
    }
    else  // main: other
    {
        if (u4ScalerMode & FBM_POOL_MODE_SCPOS_FULL)
        {
            u4FlipModule = FLIP_BY_SCALER;
        }
        else if ( (u4ScalerMode & FBM_POOL_MODE_SCPOS_PSCAN_DISP)||(u4ScalerMode & FBM_POOL_MODE_SCPOS_LINESYNC_DISP))
        {
            u4FlipModule = FLIP_BY_PSCAN;
        }
    }
    if((ucTVEEnable)&&(!env->u1IsMPEG[VDP_1])&&(!env->u1IsMPEG[VDP_2])&&(u4VdpId==VDP_2))
    {
        u4FlipModule = FLIP_NONE;
    }

#elif defined(CC_MT5395) || defined(CC_MT5396)
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
            //#if defined(CC_MT5396)
            //|| (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
            //#endif
            )
    {
        if (ucTVEEnable && (u4VdpId == VDP_2))
        {
            u4FlipModule = FLIP_NONE;
        }
        else
        {
            u4FlipModule = FLIP_BY_SCALER;
        }
    }
    else
    {
        u4FlipModule = FLIP_BY_MJC;
    }
#endif

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)||defined(CC_MT5389)
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
    {
        // adam.help.to.check
        // u4FlipModule = FLIP_BY_MJC;
    }
#if defined(CC_MT5365)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
    {
        // todo
    }
#endif
#endif


#if !defined(CC_FLIP_MIRROR_SUPPORT)
    u4FlipModule = FLIP_NONE;
#endif

    FBM_FlipModule[u4VdpId] = u4FlipModule;

    return u4FlipModule;
}

UINT32 u4SizeOfCurrent[2];

void ExpandFBM4SinglexPOP(UINT32 u4VdpId, FBM_AUTO_INC_ENV_T* env, UINT32 u4BaseDramAddr)
{
    UINT32 u4NextDramAddr, u4MddiMode, u4ScalerMode, u4TDCMode,u4Width, u4Height, u4Size, u4Mode, u4HPDSize;
    UINT8 u1Ynum, u1Cnum, u1ScalerNum, u1NRNum;
    UINT32 u4FlipModule;
    FBM_POOL_T* prPool;
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    UINT32 u4PscanModeEx=0;

    
    //u4PscanModeEx=u4DrvTDTVForceDIDramModeQueryEx(VDP_1);
    u4PscanModeEx=u4DrvTDTVForceDIDramModeQueryEx(u4VdpId);
    #endif

    u4NextDramAddr = u4BaseDramAddr;
    u4MddiMode = SRM_GetMddiMode(u4VdpId);
    u4ScalerMode = SRM_GetScposMode(u4VdpId);
    u4TDCMode = SRM_GetTDCMode(u4VdpId);
    u4HPDSize = SRM_GetScposHPDSize(u4VdpId);

    u4FlipModule = u4DecideFlipModule(u4VdpId, u4ScalerMode, env->u1IsMPEG[u4VdpId], env);

    // reset value
    {
        u4Size = 0;
        u4Width = 720;
        u4Height = 576;
        u4Mode = 0xA5A5A5A5;
        if (u4VdpId == VDP_1)
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
            UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
        }
        else
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
            UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
        }
        if (env->u1IsTDC[u4VdpId])
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);
        }
    }

    if ((env->u1mode[u4VdpId] == FBM_MODE_1080I) || (env->u1mode[u4VdpId] == FBM_MODE_576I))
    {

	/* move TDC config pool at the beginning */
    	// tdc
    	/*
        if (env->u1IsTDC[u4VdpId])
        {
	#if defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
            UPDATE_TDC_POOL_BY_MODE();
	#else
            UPDATE_TDC_POOL();
	#endif
        }
        */
        
        UPDATE_WxH_BY_RESOLUTION(u4VdpId);

        if(u4VdpId ==VDP_1 )
        {
            if (SRM_IsScanMode(VDP_1))
            {
                u4Width = 740;
                u4Height = 596;
            }
        }
        // pscan
        if (env->u1mode[u4VdpId] == FBM_MODE_1080I)
        {
            //u4Width = 2048;
            u4Width = env->u4InputWidth[u4VdpId] + 20;
            UPDATE_HPD_WIDTH();
            UPDATE_POP_WIDTH();
            u4Height = env->u4InputHeight[u4VdpId] + 20;

            UPDATE_EPG_WxH(u4VdpId, 720, 1082);
        }
        else // if (env->u1mode[u4VdpId] == FBM_MODE_576I)
        {
#if defined(CC_SUPPORT_OVERSAMPLE_1440_ENABLE)
            if (u4VdpId == VDP_1)
            {
                u4Width = 1440;
                UPDATE_POP_WIDTH();
            }
#endif
        }

        #if defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
        if(u4PscanModeEx==E_TDTV_DI_DRAM_DOUBLE)
        {
        	u1Ynum=9;
        	u1Cnum=9;
        }
        else if(u4PscanModeEx==E_TDTV_DI_DRAM_0FB)
        {
        	u1Ynum=0;
        	u1Cnum=0;
        }
        else // E_TDTV_DI_DRAM_NORMAL
        {
        CALCULATE_PSCAN_YC_NUM(u4VdpId, u4MddiMode, u4FlipModule, &u1Ynum, &u1Cnum);
        }
        #else
        CALCULATE_PSCAN_YC_NUM(u4VdpId, u4MddiMode, u4FlipModule, &u1Ynum, &u1Cnum);
        #endif

        if (u4MddiMode & FBM_POOL_MODE_10BIT)
        {
            if (env->u1IsMPEG[u4VdpId])
            {
#if defined(CC_MT5396) || defined(CC_MT5368)
                if ( (BSP_GetIcVersion() == IC_VER_5396_AA) ||(BSP_GetIcVersion() == IC_VER_5368_AA))
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
                else
#endif
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_420_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
            }
            else
            {
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
            }
        }
        else // if (u4MddiMode & FBM_POOL_MODE_8BIT)
        {
            if (env->u1IsMPEG[u4VdpId])
            {
#if defined(CC_MT5396) || defined(CC_MT5368)
                if ( (BSP_GetIcVersion() == IC_VER_5396_AA) ||(BSP_GetIcVersion() == IC_VER_5368_AA))
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
                else
#endif
                {
                    u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
                }
            }
            else
            {
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
            }
        }
#if defined(CC_SUPPORT_OVERSAMPLE_1440_ENABLE)
        if ((u4VdpId == VDP_1) && (env->u1mode[u4VdpId] == FBM_MODE_576I))
        {
            if (u4MddiMode & FBM_POOL_MODE_10BIT)
            {
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_1440_BI_10BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
            }
            else // if (u4MddiMode & FBM_POOL_MODE_8BIT)
            {
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_1440_BI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
            }
        }
#endif
        u4Mode = 0;

        CHECK_PSCAN_SIZE_TO_0();

        if(fgFBM_PscanTest)
        {
            u4Width = 1920;
            u4Height = 1080;
            u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, 10, 10), FBM_MDDI_SIZE_ALIGMENT);
            //Printf("---------Allen Test: size: 0x%x----------\n",u4Size);
        }

        if (u4VdpId == VDP_1)
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
        }
        else
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
        }

        if (u4VdpId == VDP_2)
        {
            u4Width = 720;
            u4Height = 756;
        }
        else
        {
            if (env->u1mode[u4VdpId] == FBM_MODE_576I)
            {
#if defined(CC_SUPPORT_OVERSAMPLE_1440_ENABLE)
                u4Width = 1440;
                UPDATE_POP_WIDTH();
#endif
            }
            else // 1080i
            {
                if ((u4ScalerMode & FBM_POOL_MODE_SCPOS_PSCAN_DISP) || (u4ScalerMode & FBM_POOL_MODE_SCPOS_DISP)|| (u4ScalerMode & FBM_POOL_MODE_SCPOS_LINESYNC_DISP))
                {
                    u4Width = 0;
                    u4Height = 0;
                }
                else
                {
                    u4Width = 1920;
                    UPDATE_HPD_WIDTH();
                    UPDATE_POP_WIDTH();
                    u4Height = 1080;

                    UPDATE_EPG_WxH(u4VdpId, 720, 1082);
                }
            }
#if defined(CC_IC_VERIFY_FBM_MODE)
            u4Width = 1920;
            u4Height = 1080;
#endif
            UPDATE_EPG_WxH(u4VdpId, 740, 596);
        }
#if (defined(CC_MT5395) && !defined(CC_MT5368) && !defined(CC_MT5396))
        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
        {
            //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
            if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
            {
                u4FlipModule = FLIP_BY_3D;
            }
        }
        else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
        {
            u4FlipModule = FLIP_BY_SCALER;
        }
        else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
        {
#ifdef DRV_SUPPORT_MT8283
            //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
            if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
            {
                u4FlipModule = FLIP_BY_3D;
            }
#endif
            // adam.help.to.check
            // u4FlipModule = FLIP_BY_SCALER;
        }
#endif
#if defined(CC_MT5365)
        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)||(SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP))
        {
            //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
            if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
            {
                u4FlipModule = FLIP_BY_3D;
            }
        }
#endif
#ifdef CC_MT5396
        if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
                ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
           )
        {
            //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
            if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
            {
                u4FlipModule = FLIP_BY_3D;
            }
            #if defined(SUPPORT_3D_OVSN_BY_SCL)
            else if(u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_3FB)
            {
                u4Width = 1920;
                u4Height = 1080;
                LOG(5,"**5396 FBM DRAM_3FB**\n");
            }
            #endif
        }
#endif
#if defined(CC_MT5368) || defined(CC_MT5389)
        if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
                ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
                ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
                ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
           )
        {
            //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
            if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
            {
                u4FlipModule = FLIP_BY_3D;
               LOG(5,"**5368/89 FBM DRAM_6FB**\n");
            }
            //else if(u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_3FB)
            else if(u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_3FB)
            {
                u4Height = 1080;
                LOG(5,"**5368/89 FBM DRAM_3FB**\n");
            }
        }
#endif
        CALCULATE_SCALER_422_PARAMETERS();
        CHECK_SCALER_SIZE_TO_0();

        if (u4VdpId == VDP_1)
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
        }
        else
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
        }

	/* move TDC config pool at the beginning */
        // tdc
        
        if (env->u1IsTDC[u4VdpId])
        {
#if defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
            UPDATE_TDC_POOL_BY_MODE();
#else
            UPDATE_TDC_POOL();
#endif
        }
        
    }

    else if (env->u1mode[u4VdpId] == FBM_MODE_1080P_422)
    {
        UPDATE_WxH_BY_RESOLUTION(u4VdpId);
      if((env->u4InputHeight[u4VdpId]>1080)&&(u4HPDSize<1920))
        {
            u4Height = env->u4InputHeight[u4VdpId]+20;//for 1600*1200 on no-fhd panel.
        }
        // pscan-prog-disp-mode: not consider the onoff of NR, because it does
        // not affect the size in prog disp mode
        if (u4MddiMode & FBM_POOL_MODE_MDDI_DISP)
        {

#if defined(CC_MT5365)
            if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
            {
                UPDATE_EPG_WxH(u4VdpId, 720, 576);
            }
            else
#endif

#if defined(CC_MT5368) || defined(CC_MT5389)
                if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
                {
                    UPDATE_EPG_WxH(u4VdpId, 720, 576);
                }
                else
#endif
                {
                    UPDATE_EPG_WxH(u4VdpId, 720, 1080);
                }

#if defined(CC_MT5396) || defined(CC_MT5368)||defined(CC_MT5389)
             #if defined(CC_MT5396) || defined(CC_MT5368)
                if ( (BSP_GetIcVersion() == IC_VER_5396_AA) ||(BSP_GetIcVersion() == IC_VER_5368_AA))
                {
                    CALCULATE_PSCAN_NR_YC_SIZE_BEFORE_ECO();
                }
                else
                {
                	CALCULATE_PSCAN_NR_YC_SIZE_3D();
                }
             #else  // defined(CC_MT5389)
                CALCULATE_PSCAN_NR_YC_SIZE_3D();
	      #endif
 
#else
            CALCULATE_PSCAN_NR_YC_SIZE();
#endif
            u4Mode = 0;
            if(fgFBM_PscanTest)
            {
                u4Width = 1920;
                u4Height = 1080;
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, 10, 10), FBM_MDDI_SIZE_ALIGMENT);
                //Printf("---------Allen Test: size: 0x%x----------\n",u4Size);
            }
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
            }
        }
#ifdef CC_MT5396
        else if (u4ScalerMode & (FBM_POOL_MODE_SCPOS_FULL|FBM_POOL_MODE_SCPOS_DISP|FBM_POOL_MODE_SCPOS_LINESYNC_DISP))
#else
        else if (u4ScalerMode & (FBM_POOL_MODE_SCPOS_FULL|FBM_POOL_MODE_SCPOS_DISP))
#endif
        {
#if defined(CC_MT5395)
            // remove this, after ECO
            if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
            {
                // for 720p: scaler frame interleave mode
                if (env->u4Resolution[u4VdpId] == SRM_VDP_720HD_RESOLUTION)
                {
                    u4Height = 1080;
                }
            }
#endif
            // pscan-nr

#if defined(CC_MT5365)
            if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
            {
                UPDATE_EPG_WxH(u4VdpId, 720, 576);
            }
            else
#endif
#if defined(CC_MT5368) || defined(CC_MT5389)
                if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
                {
                    UPDATE_EPG_WxH(u4VdpId, 720, 576);
                }
                else
#endif
                {
                    UPDATE_EPG_WxH(u4VdpId, 720, 1080);
                }

            if (u4MddiMode & FBM_POOL_MODE_MDDI_NR_Y_C)
            {
                CALCULATE_PSCAN_NR_YC_SIZE();
            }
            else if (u4MddiMode & FBM_POOL_MODE_MDDI_NR_Y_ONLY)
            {
                CALCULATE_PSCAN_NR_Yonly_SIZE();
            }
            else // FBM_POOL_MODE_MDDI_NR_OFF
            {
                u4Size = 0;
            }
            u4Mode = 0;
            if(fgFBM_PscanTest)
            {
                u4Width = 1920;
                u4Height = 1080;
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, 10, 10), FBM_MDDI_SIZE_ALIGMENT);
                //Printf("---------Allen Test: size: 0x%x----------\n",u4Size);
            }
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
            }

#if (defined(CC_MT5395) && !defined(CC_MT5368) && !defined(CC_MT5396))
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
                    || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
                    || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
            }
            else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
            {
                u4FlipModule = FLIP_BY_SCALER;
            }
            else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
            {
#ifdef DRV_SUPPORT_MT8283
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
#endif
                // adam.help.to.check
                //u4FlipModule = FLIP_BY_SCALER;
            }
#endif
#if defined(CC_MT5365)
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)||(SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP))
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
            }
#endif
#ifdef CC_MT5396
            if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
               )
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
                #if defined(SUPPORT_3D_OVSN_BY_SCL)
                else if(u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_3FB)
                {
                    u4Width = 1920;
                    u4Height = 1080;
                }
                #endif
                
            }
#endif
#if defined(CC_MT5368) || defined(CC_MT5389)
            if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
               )
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
                //else if(u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_3FB)
                else if(u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_3FB)
                {
                    u4Height = 1080;
                    //Printf("**5368/89 FBM DRAM_3FB**\n");
                }
            }
#endif

            UPDATE_EPG_WxH(u4VdpId, 720, 576);
            CALCULATE_SCALER_422_PARAMETERS();
            CHECK_SCALER_SIZE_TO_0();
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
            }
        }
    }

    else if (env->u1mode[u4VdpId] == FBM_MODE_1080P_444)
    {
        UPDATE_WxH_BY_RESOLUTION(u4VdpId);

        if((env->u4InputHeight[u4VdpId]>1080)&&(u4HPDSize<1920))
        {
            u4Height = env->u4InputHeight[u4VdpId]+20;//for 1600*1200 on no-fhd panel.
        }
        if (u4MddiMode & FBM_POOL_MODE_MDDI_DISP)
        {
            UPDATE_EPG_WxH(u4VdpId, 720, 1082);
#if defined(CC_MT5396) || defined(CC_MT5368)||defined(CC_MT5389)
            CALCULATE_PROG_DISP_444_SIZE_EX();
#else
            CALCULATE_PROG_DISP_444_SIZE();
#endif
            u4Mode = 0;
            if(fgFBM_PscanTest)
            {
                u4Width = 1920;
                u4Height = 1080;
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, 10, 10), FBM_MDDI_SIZE_ALIGMENT);
                //Printf("---------Allen Test: size: 0x%x----------\n",u4Size);
            }
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
            }
        }
        else if (u4ScalerMode & (FBM_POOL_MODE_SCPOS_FULL|FBM_POOL_MODE_SCPOS_DISP))
        {
            UPDATE_EPG_WxH(u4VdpId, 720, 576);
#if (defined(CC_MT5395) && !defined(CC_MT5368) && !defined(CC_MT5396))
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
                    || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
                    || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQuery(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
            }
            else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
            {
                u4FlipModule = FLIP_BY_SCALER;
            }
            else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
            {
                // adam.help.to.check
                //u4FlipModule = FLIP_BY_SCALER;
            }
#endif
#if defined(CC_MT5365)
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)||(SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP))
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
            }
#endif
#ifdef CC_MT5396
            if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
               )
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
                #if defined(SUPPORT_3D_OVSN_BY_SCL)
                else if(u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_3FB)
                {
                    u4Width = 1920;
                    u4Height = 1080;
                }
                #endif
            }
#endif
#if defined(CC_MT5368) ||defined(CC_MT5389)
            if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
                    ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
               )
            {
                //if (u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_6FB)
                if (u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_6FB)
                {
                    u4FlipModule = FLIP_BY_3D;
                }
                //else if(u4DrvTDTVForceDramModeQuery() == E_TDTV_DRAM_3FB)
                else if(u4DrvTDTVForceDramModeQueryEx(u4VdpId) == E_TDTV_DRAM_3FB)
                {
                    u4Height = 1080;
                    //Printf("**5368/89 FBM DRAM_3FB**\n");
                }
            }
#endif
            CALCULATE_SCALER_444_PARAMETERS();
            CHECK_SCALER_SIZE_TO_0();
            if (u4VdpId == VDP_1)
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_MAIN, SCPOS);
            }
            else
            {
                UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
            }

            if(fgFBM_PscanTest)
            {
                u4Width = 1920;
                u4Height = 1080;
                u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(u4Width, u4Height, 10, 10), FBM_MDDI_SIZE_ALIGMENT);
                if (u4VdpId == VDP_1)
                {
                    UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_MAIN, MDDI);
                }
                else
                {
                    UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
                }
            }
        }
    }

    u4SizeOfCurrent[u4VdpId] = u4NextDramAddr-u4BaseDramAddr;
    LOG(3, "vdp:%d, occupied size: 0x%x\n", u4VdpId, u4SizeOfCurrent[u4VdpId]);
}

void ExpandFBM4SubPIP(UINT32 u4VdpId, FBM_AUTO_INC_ENV_T* env, UINT32 u4BaseDramAddr)
{
    UINT32 u4NextDramAddr, u4MddiMode, u4ScalerMode,u4TDCMode, u4Width, u4Height, u4Size, u4Mode;
    UINT8 u1Ynum, u1Cnum, u1ScalerNum;
    UINT32 u4FlipModule;
    FBM_POOL_T* prPool;

    u4NextDramAddr = u4BaseDramAddr;
    u4MddiMode = SRM_GetMddiMode(u4VdpId);
    u4TDCMode = SRM_GetTDCMode(u4VdpId);
    u4ScalerMode = SRM_GetScposMode(u4VdpId);

    u4FlipModule = u4DecideFlipModule(u4VdpId, u4ScalerMode, env->u1IsMPEG[u4VdpId], env);

    // reset value
    {
        u4Size = 0;
        u4Width = 720;
        u4Height = 576;
        u4Mode = 0xA5A5A5A5;
        UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);
        UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
        if (env->u1IsTDC[u4VdpId])
        {
            UPDATE_PRPOOL(FBM_POOL_TYPE_TDC_DYN, TDC);
        }
    }

    if ((env->u1mode[u4VdpId] == FBM_MODE_1080I) || (env->u1mode[u4VdpId] == FBM_MODE_576I))
    {
    	/* move TDC config pool at the beginning */
    	// tdc
    	/*
        if (env->u1IsTDC[u4VdpId])
        {
	#if defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
            UPDATE_TDC_POOL_BY_MODE();
	#else
            UPDATE_TDC_POOL();
	#endif
        }
        */
        
        // pscan
        if (env->u1mode[u4VdpId] == FBM_MODE_1080I)
        {
            u4Width = 720;
            u4Height = 1082;
        }
        else // if (env->u1mode[u4VdpId] == FBM_MODE_576I)
        {
            u4Width = 780;
            u4Height = 576;
        }
        CALCULATE_PSCAN_YC_NUM(u4VdpId, u4MddiMode, u4FlipModule, &u1Ynum, &u1Cnum);
        // 8 bit only
        u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
        u4Mode = 0;
        UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);

        // scaler
        u4Width = 720;
        u4Height = 578;
        CALCULATE_SCALER_422_PARAMETERS();
        UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);

	/* move TDC config pool at the beginning */
        // tdc
        
        if (env->u1IsTDC[u4VdpId])
        {
#if defined(CC_MT5396) ||defined(CC_MT5368) ||defined(CC_MT5389)
            UPDATE_TDC_POOL_BY_MODE();
#else
            UPDATE_TDC_POOL();
#endif
        }
        
    }

    else if ((env->u1mode[u4VdpId] == FBM_MODE_1080P_422) || (env->u1mode[u4VdpId] == FBM_MODE_1080P_444))
    {
        // pscan - dummpy for FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR
        u4NextDramAddr = u4BaseDramAddr;
        u4Width = 780;
        u4Height = 576;
        u1Ynum = 1;
        u1Cnum = 1;
        u4Mode=0;
        u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(u4Width, u4Height, u1Ynum, u1Cnum), FBM_MDDI_SIZE_ALIGMENT);
        //u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;
        UPDATE_PRPOOL(FBM_POOL_TYPE_MDDI_DYN_SUB, MDDI);

        // scaler
        u4NextDramAddr = u4BaseDramAddr;
        u4Width = 780;
        u4Height = 576;
        CALCULATE_SCALER_444_PARAMETERS();
        UPDATE_PRPOOL(FBM_POOL_TYPE_SCPOS_DYN_SUB, SCPOS);
    }

    u4SizeOfCurrent[u4VdpId] = u4NextDramAddr-u4BaseDramAddr;
    LOG(3, "vdp:%d, occupied size: 0x%x\n", u4VdpId, u4SizeOfCurrent[u4VdpId]);
}

#if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
UINT32 _FbmGetDyamicMPEG_NptvMaxSize(void)
{
    return 0x34a000; // 720P
}

UINT32 _FbmGetDyamicMPEG_MPEGMaxSize(void)
{
    return 0x750000; // RMVB 720P
}
#endif

static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5365_128MB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    //FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;
    UINT8 IsNoSub = FALSE;
#if defined(CC_MT5395) || defined(CC_MT5396)
    UINT8 u1CheckDSP = 1;
    UNUSED(u1CheckDSP);
#endif

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;
        //prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        //_fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvBeAddr + (prPool->u4Size)/2;

        /* set _fbm_u4NptvSubPipAddr in the following case */
        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)
#if defined(CC_MT5365)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB)
#endif
           )
        {
#if defined(CC_FLIP_MIRROR_SUPPORT)
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1310000;
#else
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xde0000;
#endif
        }
#if defined(CC_MT5395) || defined(CC_MT5396)
        else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
#if defined(CC_MT5396)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
#endif
#if defined(CC_MT5368)||defined(CC_MT5389)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
#endif
                )
        {
            // the same as MT5365_128Mx2
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xe00000;
        }
        else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
                 || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
        {
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2100000 + 0xA00000;
        }
        else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
        {
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2100000;
        }
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
        {
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2100000;
        }

#if defined(CC_MT5368) ||defined(CC_MT5389)
        else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
        {
            #if defined(CC_FLIP_MIRROR_SUPPORT)
            #if defined(MT5366_SUPPORT_3D_PR)
						_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xE80000 +FBM_ADD_PQ_BUFFER_SIZE;
            #else 
						_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xE80000;
            #endif
                        _fbm_u4NptvSubPipAddr += 0x400000;
            #else
            #if defined(MT5366_SUPPORT_3D_PR)
						_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xd00000 +FBM_ADD_PQ_BUFFER_SIZE;
            #else
						_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xd00000;
			#endif
			#endif
						
        }
#endif

#if defined(CC_MT5365)
        else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
        {
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2100000 + 0xA00000;
        }
        else if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
        {
#if defined(CC_FLIP_MIRROR_SUPPORT)
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1310000;
#else
            // MT5365_128Mx2
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xe80000;
#endif
        }
#endif
#endif
        else
        {
#if defined(CC_FLIP_MIRROR_SUPPORT)
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1310000;
#else
            // MT5365_128Mx2
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1180000;
            //_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1c00000;
#endif
        }

#if defined(CC_IC_VERIFY_FBM_MODE)
        _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2000000;
#endif

        // reset value
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
    }

    _FbmGetEnv(&env);

#if defined(CC_MT5365)
    IsNoSub = ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1) || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1_NOH264));
#endif
#if defined(CC_MT5368) || defined(CC_MT5389)
    IsNoSub = ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)  || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264));
#endif
#if defined(CC_FBM_NO_SUBPATH)
    IsNoSub = TRUE;
#endif

    if (IsNoSub)
    {
        if (env.u1IsMPEG[VDP_1])
        {
#if defined(CC_DYNAMIC_MPEG_SIZE)
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
#else
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
#endif
            _fbm_u4NptvBeAddr = prPool->u4Addr + prPool->u4Size;
#if defined(CC_DYNAMIC_MPEG_SIZE) // unmark
            _fbm_u4NptvBeAddr = prPool->u4Addr + SRM_GetMMSizeMode(VDP_1);
#endif
        }
        else
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
            _fbm_u4NptvBeAddr = prPool->u4Addr;
        }

        if (env.u1fgPIP || env.u1fgPOP)
        {
            Printf("This model does NOT support PIP/POP\n");
            //ASSERT(0);
        }
    }

    ExpandFBM4SinglexPOP(VDP_1, &env, _fbm_u4NptvBeAddr);
#if !defined(CC_FBM_NO_SUBPATH)
#if defined(CC_MT5365)
    if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1)
            && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1_NOH264))
#endif
#if defined(CC_MT5368) || defined(CC_MT5389)
        if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5368_64Mx1)
                && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5368_64Mx1_NOH264))
#endif
        {
            if (env.u1fgPIP || env.u1fgPOP)
            {
                if (env.u1fgPOP)
                {
                    // u4NextDramAddr = _fbm_u4NptvSubPopAddr; // todo
                    ExpandFBM4SinglexPOP(VDP_2, &env, _fbm_u4NptvSubPopAddr);
                }
                else
                {
                    // u4NextDramAddr = _fbm_u4NptvSubPipAddr; // todo
                    ExpandFBM4SubPIP(VDP_2, &env, _fbm_u4NptvSubPipAddr);
                }
            }
        }
#endif
    if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
	        (ucPoolType == (UCHAR) FBM_POOL_TYPE_PVR) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
    {
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
        {
            _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        }
		if (ucPoolType == (UCHAR) FBM_POOL_TYPE_PVR)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_PVR);
            prPool->u4Addr = _fbm_u4NptvSubPipAddr - FBM_PVR_POOL_SIZE;//FBM_POOL_TYPE_PVR;//0x400000;		
        }
        // sub pip error checking
        // sub pop error checking
#if !defined(CC_FBM_NO_SUBPATH)
#if defined(CC_MT5365)
        if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1)
                && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1_NOH264))
#endif
#if defined(CC_MT5368) || defined(CC_MT5389)
            if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5368_64Mx1)
                    && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5368_64Mx1_NOH264))
#endif
            {
                if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                        (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
                {
                    if (env.u1fgPIP)
                    {
                        if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPipAddr)
                        {
                            Printf("FBM main overwrite subpip\n");
                        }
                    }
                    else if (env.u1fgPOP)
                    {
                        if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPopAddr)
                        {
                            Printf("FBM main overwrite subpop\n");
                        }
                    }
                }
            }
#endif

#if defined(CC_MT5395) || defined(CC_MT5396)
        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
#if defined(CC_MT5396)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
#endif
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
        {
            // only SCPOS_MAIN in channel 2, so it need not to check
            u1CheckDSP = 0;
        }
#endif
        // dsp error checking
#if defined(CC_MT5395) || defined(CC_MT5396)
        if (u1CheckDSP)
#endif
        {
            if ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr))
            {
                Printf("FBM nptv overlap with DSP: 0x%x\n", ucPoolType);
                Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
                Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
                VERIFY(0);
            }
        }
        return prPool;
    }

#if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG_FREE);
    prPool->u4Addr = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG)->u4Addr;
    prPool->u4Addr += _FbmGetDyamicMPEG_NptvMaxSize();
    prPool->u4Addr += _FbmGetDyamicMPEG_MPEGMaxSize();
    prPool->u4Size = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr - prPool->u4Addr;
#endif

    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}
#if defined(CC_MT5389)||defined(CC_MT5368)
static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5501_128MB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool;
   // static UINT8 fgInited = FALSE;
    //FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;
    UINT8 IsNoSub = FALSE;
    #if defined(CC_MT5395) || defined(CC_MT5396)
    UINT8 u1CheckDSP = 1;
    UNUSED(u1CheckDSP);
    #endif

  //  if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
       //fgInited = TRUE;
        if(SRM_GetCurrFBMMode()==FBM_MMMODE)
        {
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_PQ_MM);
        }
		else if(SRM_GetCurrFBMMode()==FBM_NETMODE)
		{
		prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_PQ_NET);
		}
		else
		{
		prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_PQ_TV);
		}
        _fbm_u4NptvBeAddr = prPool->u4Addr;
        //prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        //_fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvBeAddr + (prPool->u4Size)/2;

        /* set _fbm_u4NptvSubPipAddr in the following case */
        
		_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1800000;      

        // reset value
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
    }

    _FbmGetEnv(&env);

    #if defined(CC_MT5365)
    IsNoSub = ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1) || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1_NOH264));
    #endif
    #if defined(CC_MT5368) || defined(CC_MT5389) 
    IsNoSub = ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1) );
    #endif
    #if defined(CC_FBM_NO_SUBPATH)
    IsNoSub = TRUE;
    #endif

    if (IsNoSub)
    {
        if (env.u1IsMPEG[VDP_1])
        {
	    #if defined(CC_DYNAMIC_MPEG_SIZE)
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
	    #else
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
	    #endif
            _fbm_u4NptvBeAddr = prPool->u4Addr + prPool->u4Size;
            #if defined(CC_DYNAMIC_MPEG_SIZE) // unmark
            _fbm_u4NptvBeAddr = prPool->u4Addr + SRM_GetMMSizeMode(VDP_1);
            #endif
        }
        else
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
            _fbm_u4NptvBeAddr = prPool->u4Addr;
        }

        if (env.u1fgPIP || env.u1fgPOP)
        {
            Printf("This model does NOT support PIP/POP\n");
            //ASSERT(0);
        }
    }

    ExpandFBM4SinglexPOP(VDP_1, &env, _fbm_u4NptvBeAddr);
    #if !defined(CC_FBM_NO_SUBPATH)
    #if defined(CC_MT5365)
    if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1)
        && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1_NOH264))
    #endif
    #if defined(CC_MT5368) || defined(CC_MT5389) 
    if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5368_64Mx1))
    #endif
    {
        if (env.u1fgPIP || env.u1fgPOP)
        {
            if (env.u1fgPOP)
            {
                // u4NextDramAddr = _fbm_u4NptvSubPopAddr; // todo
                ExpandFBM4SinglexPOP(VDP_2, &env, _fbm_u4NptvSubPopAddr);
            }
            else
            {
                // u4NextDramAddr = _fbm_u4NptvSubPipAddr; // todo
                ExpandFBM4SubPIP(VDP_2, &env, _fbm_u4NptvSubPipAddr);
            }
        }
    }
    #endif
    if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC) ||
        (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
        (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN) ||
        (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
        (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
    {
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
        {
            _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        }
        // sub pip error checking
        // sub pop error checking
        #if !defined(CC_FBM_NO_SUBPATH)
        #if defined(CC_MT5365)
        if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1)
            && (SRMFBM_GetConf() != FBM_MEM_CFG_MT5365_64Mx1_NOH264))
        #endif
        #if defined(CC_MT5368) || defined(CC_MT5389) 
        if ((SRMFBM_GetConf() != FBM_MEM_CFG_MT5368_64Mx1))
        #endif
        {
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                    (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
            {
                if (env.u1fgPIP)
                {
                    if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPipAddr)
                    {
                        Printf("FBM main overwrite subpip\n");
                    }
                }
                else if (env.u1fgPOP)
                {
                    if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPopAddr)
                    {
                        Printf("FBM main overwrite subpop\n");
                    }
                }
            }
        }
        #endif

        #if defined(CC_MT5395) || defined(CC_MT5396)
        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
       #if defined(CC_MT5396)
	     || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
	#endif
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
        {
            // only SCPOS_MAIN in channel 2, so it need not to check
            u1CheckDSP = 0;
        }
        #endif
        // dsp error checking
        #if defined(CC_MT5395) || defined(CC_MT5396)
        if (u1CheckDSP)
        #endif
        {
            if ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr))
            {
                Printf("FBM nptv overlap with DSP: 0x%x\n", ucPoolType);
                Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
                Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
                #if defined(CC_MT5368) || defined(CC_MT5389)
                Printf("------------------------------------\n");
                #else
                VERIFY(0);
                #endif
            }
        }
        return prPool;
    }

    #if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG_FREE);
    prPool->u4Addr = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG)->u4Addr;
    prPool->u4Addr += _FbmGetDyamicMPEG_NptvMaxSize();
    prPool->u4Addr += _FbmGetDyamicMPEG_MPEGMaxSize();
    prPool->u4Size = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr - prPool->u4Addr;
    #endif

    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}
#endif

#ifdef CC_MT5396
static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5396_1GB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    //FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;
    UINT8 IsNoSub = FALSE;
    UINT8 u1CheckDSP = 1;
    UNUSED(u1CheckDSP);

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;
        //prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        //_fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvBeAddr + (prPool->u4Size)/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvSubPopAddr +0x200000;
        /* set _fbm_u4NptvSubPipAddr in the following case */

        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
           )
        {
            // the same as MT5365_128Mx2
            //_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xe00000;
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2000000;
        }
        else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
           )
        {
            // the same as MT5365_128Mx2
            //_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xe00000;
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1b00000;
        }
        else
        {
#if defined(CC_FLIP_MIRROR_SUPPORT)
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1310000;
#else
            // MT5365_128Mx2
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1180000;
            //_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1c00000;
#endif
        }

        //#if defined(CC_IC_VERIFY_FBM_MODE)
#if 0
        _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2000000;
#endif

        // reset value
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
    }

    _FbmGetEnv(&env);

#if defined(CC_FBM_NO_SUBPATH)
    IsNoSub = TRUE;
#endif

    if (IsNoSub)
    {
        if (env.u1IsMPEG[VDP_1])
        {
#if defined(CC_DYNAMIC_MPEG_SIZE)
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
#else
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
#endif
            _fbm_u4NptvBeAddr = prPool->u4Addr + prPool->u4Size;
#if defined(CC_DYNAMIC_MPEG_SIZE) // unmark
            _fbm_u4NptvBeAddr = prPool->u4Addr + SRM_GetMMSizeMode(VDP_1);
#endif
        }
        else
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
            _fbm_u4NptvBeAddr = prPool->u4Addr;
        }

        if (env.u1fgPIP || env.u1fgPOP)
        {
            Printf("This model does NOT support PIP/POP\n");
            //ASSERT(0);
        }
    }

    ExpandFBM4SinglexPOP(VDP_1, &env, _fbm_u4NptvBeAddr);
#if !defined(CC_FBM_NO_SUBPATH)
    if (env.u1fgPIP || env.u1fgPOP)
    {
        if (env.u1fgPOP)
        {
            // u4NextDramAddr = _fbm_u4NptvSubPopAddr; // todo
            ExpandFBM4SinglexPOP(VDP_2, &env, _fbm_u4NptvSubPopAddr);
        }
        else
        {
            // u4NextDramAddr = _fbm_u4NptvSubPipAddr; // todo
            ExpandFBM4SubPIP(VDP_2, &env, _fbm_u4NptvSubPipAddr);
        }
    }
#endif
    if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
    {
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
        {
            _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        }
        // sub pip error checking
        // sub pop error checking
#if !defined(CC_FBM_NO_SUBPATH)
        if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
        {
            if (env.u1fgPIP)
            {
                if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPipAddr)
                {
                    Printf("FBM main overwrite subpip\n");
                }
            }
            else if (env.u1fgPOP)
            {
                if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPopAddr)
                {
                    Printf("FBM main overwrite subpop\n");
                }
            }
        }
#endif

        /*
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
            {
                // only SCPOS_MAIN in channel 2, so it need not to check
                u1CheckDSP = 0;
            }
            */
        // dsp error checking

        if (u1CheckDSP)
        {
            if ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr))
            {
                Printf("FBM nptv overlap with DSP: 0x%x\n", ucPoolType);
                Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
                Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
                VERIFY(0);
            }
        }
        return prPool;
    }

#if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG_FREE);
    prPool->u4Addr = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG)->u4Addr;
    prPool->u4Addr += _FbmGetDyamicMPEG_NptvMaxSize();
    prPool->u4Addr += _FbmGetDyamicMPEG_MPEGMaxSize();
    prPool->u4Size = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr - prPool->u4Addr;
#endif

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}

#endif

#if defined(CC_MT5368)||defined(CC_MT5389)
static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5368_256MB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    //FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;
    UINT8 IsNoSub = FALSE;
    UINT8 u1CheckDSP = 1;
    UNUSED(u1CheckDSP);

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;
        //prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        //_fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvBeAddr + (prPool->u4Size)/2;
        _fbm_u4NptvSubPopAddr = _fbm_u4NptvSubPopAddr +0x200000;
        /* set _fbm_u4NptvSubPipAddr in the following case */

        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
           )
        {
            // the same as MT5365_128Mx2
            // _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xe00000;
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2000000;
        }
        else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
                )
        {
            // the same as MT5365_128Mx2
            //_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1500000; // Main path = 21M
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1b00000+0x600000;
        }
        else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK)
                )
        {
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0xc00000; // main 12M, sub 9M
        }
        else
        {
#if defined(CC_FLIP_MIRROR_SUPPORT)
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1310000;
#else
            // MT5365_128Mx2
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1180000;
            //_fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x1c00000;
#endif
        }

        //#if defined(CC_IC_VERIFY_FBM_MODE)
#if 0
        _fbm_u4NptvSubPipAddr = _fbm_u4NptvBeAddr + 0x2000000;
#endif


        // reset value
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC);
        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
    }

    _FbmGetEnv(&env);

#if defined(CC_FBM_NO_SUBPATH)
    IsNoSub = TRUE;
#endif

    if (IsNoSub)
    {
        if (env.u1IsMPEG[VDP_1])
        {
#if defined(CC_DYNAMIC_MPEG_SIZE)
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
#else
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
#endif
            _fbm_u4NptvBeAddr = prPool->u4Addr + prPool->u4Size;
#if defined(CC_DYNAMIC_MPEG_SIZE) // unmark
            _fbm_u4NptvBeAddr = prPool->u4Addr + SRM_GetMMSizeMode(VDP_1);
#endif
        }
        else
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
            _fbm_u4NptvBeAddr = prPool->u4Addr;
        }

        if (env.u1fgPIP || env.u1fgPOP)
        {
            Printf("This model does NOT support PIP/POP\n");
            //ASSERT(0);
        }
    }

    ExpandFBM4SinglexPOP(VDP_1, &env, _fbm_u4NptvBeAddr);
#if !defined(CC_FBM_NO_SUBPATH)
    if (env.u1fgPIP || env.u1fgPOP)
    {
        if (env.u1fgPOP)
        {
            // u4NextDramAddr = _fbm_u4NptvSubPopAddr; // todo
            ExpandFBM4SinglexPOP(VDP_2, &env, _fbm_u4NptvSubPopAddr);
        }
        else
        {
            // u4NextDramAddr = _fbm_u4NptvSubPipAddr; // todo
            ExpandFBM4SubPIP(VDP_2, &env, _fbm_u4NptvSubPipAddr);
        }
    }
#endif
    if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
    {
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
        {
            _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
        }
        // sub pip error checking
        // sub pop error checking
#if !defined(CC_FBM_NO_SUBPATH)
        if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
        {
            if (env.u1fgPIP)
            {
                if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPipAddr)
                {
                    Printf("FBM main overwrite subpip\n");
                }
            }
            else if (env.u1fgPOP)
            {
                if ((prPool->u4Addr + prPool->u4Size) > _fbm_u4NptvSubPopAddr)
                {
                    Printf("FBM main overwrite subpop\n");
                }
            }
        }
#endif

        /*
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC))
            {
                // only SCPOS_MAIN in channel 2, so it need not to check
                u1CheckDSP = 0;
            }
            */
        // dsp error checking

        if (u1CheckDSP)
        {
            if ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr))
            {
                Printf("FBM nptv overlap with DSP: 0x%x\n", ucPoolType);
                Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
                Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
                VERIFY(0);
            }
        }
        return prPool;
    }

#if defined(CC_FBM_SPLIT_MPEG_FREE_PART)
    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG_FREE);
    prPool->u4Addr = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG)->u4Addr;
    prPool->u4Addr += _FbmGetDyamicMPEG_NptvMaxSize();
    prPool->u4Addr += _FbmGetDyamicMPEG_MPEGMaxSize();
    prPool->u4Size = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr - prPool->u4Addr;
#endif

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}

#endif

#endif

static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5388_128MB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        UINT8 u1Mode, u1Type;

        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
        _fbm_u4NptvBeAddr = (prPool->u4Addr + prPool->u4Size);
        prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        _fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        /* set _fbm_u4NptvSubPipAddr in the following case */

        _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
        _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
        _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
        _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
        _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;

        //if (env.u1fgSingle || env.u1fgPIP)
        {
            // main modules
            {
                // if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);

#if defined(__MODEL_slt__)
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
#elif defined(CC_FLIP_MIRROR_SUPPORT)
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
#endif
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    LOG(6, "main di 1080i size %x \n",prPool->u4Size);

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler: for EPG mode
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;
#if defined(__MODEL_slt__)
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
#elif defined(CC_MIRROR_SW_WA)
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
#endif

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    LOG(6, "main scaler 1080i size %x \n",prPool->u4Size);
                    LOG(6, "main scaler 1080i end address %x \n",prPool->u4Addr+prPool->u4Size);
                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

#if defined(__MODEL_slt__)
                    _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
#endif

#if defined(CC_MIRROR_SW_WA)
                    _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
#endif
                }

                //if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr,FBM_MDDI_ADDR_ALIGMENT);
#ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_1440_BI_10BIT_422_SIZE(1440, 578, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 1440;
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(780, 578, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 780;
#endif
                    prPool->u4Height = 578;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
#ifdef CC_MIRROR_SW_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 578);
#else
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 578);
#endif
#else
#ifdef CC_MIRROR_SW_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
#else
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
#endif
#endif
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
#ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
                    prPool->u4Width = 1440;
#else
                    prPool->u4Width = 780;
#endif
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // tdc in main
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPrevPool = prPool;

                    u1Type = FBM_TDC;
                    LOG(6, "main 576i end address %x \n",prPool->u4Addr+prPool->u4Size);
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

                // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(2048, 1082, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_MIRROR_SW_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
#else
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
#endif
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    LOG(6, "main 1080p422 end address %x \n",prPool->u4Addr+prPool->u4Size);
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
#if !defined(CC_MIRROR_SW_WA)
#if !defined(__MODEL_slt__)
                    _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
#endif
#endif
                }

                //if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    LOG(6, "main 1080p444 end address %x \n",prPool->u4Addr+prPool->u4Size);
                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

                    {
                        // dummy allocation
                        // main di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 576;
                        prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }

#if 0
                // MM 1080P cases
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    {
                        // dummy allocation: scaler will get and assert the prPool in vDrvScpipSrmDispCb
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(2048, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 2048;
                        prPool->u4Height = 1082;

                        u1Mode = FBM_TYPE_MM_1080P_MODE;
                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }

                // MM 1080I cases
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(2048, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MM_1080I_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    {
                        // dummy allocation: scaler will get and assert the prPool in vDrvScpipSrmDispCb
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                        prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                        prPool->u4Width = 2048;
                        prPool->u4Height = 1082;

                        u1Mode = FBM_TYPE_MM_1080I_MODE;
                        u1Type = FBM_MAIN_SCALER;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }
#endif

                // error checking
                prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
                if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
                {
                    LOG(1, "main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                    VERIFY(0);
                }
                if (_fbm_u4NptvSubPipAddr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
                {
                    LOG(1, "main overwrite sub pip, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                    VERIFY(0);
                }
            }
            // if (env.u1fgPIP) // sub modules
            {
                //if (env.u1Interlace[VDP_2] == 1) // 1080i
                {
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), FBM_MDDI_SIZE_ALIGMENT); // c bob
                    prPool->u4Width = 720;
                    prPool->u4Height = 1080;
                    prPool->u4Mode = 0;
                    LOG(6, "sub 1080i di size %x \n",prPool->u4Size);

                    // 1080i
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // sub scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(prPrevPool->u4Addr + prPrevPool->u4Size, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    LOG(6, "sub 1080i scaler size %x \n",prPool->u4Size);

                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }
                }
                //if (env.u1Interlace[VDP_2] == 1) // 576i
                {
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT); // yc 3d
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPool->u4Mode = 0;

                    // 576i
                    LOG(6, "sub 576 di size %x \n",prPool->u4Size);
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // sub scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(prPrevPool->u4Addr + prPrevPool->u4Size, FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_TVE_US_IMAGERESIZER_WA
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT) * 2;
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT);
#endif
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    LOG(6, "sub 576i scaler size %x \n",prPool->u4Size);

                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // tdc in sub
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    LOG(6, "sub 576i tdc size %x \n",prPool->u4Size);

                    u1Type = FBM_TDC;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }
                }
                //else
                {
                    {
                        // dummy allocation
                        // sub di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 1080;
                        prPool->u4Mode = 0;

                        u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                        u1Type = FBM_SUB_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }

                    // sub scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_TVE_US_IMAGERESIZER_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 760, 580) * 2;
#else
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 760, 580);
#endif
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 760;
                    prPool->u4Height = 580;

                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }

                    // 1080P422 is the same as 1080P444
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Mode;

                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Mode;
                }

                // error checking
                prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
                if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4SubPipMaxEnd)
                {
                    LOG(1, "sub overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1SubPipMaxMode);
                    VERIFY(0);
                }
            }
        }
        // else // POP: take PAP mode into consideration (1100x900)
        {
            {
                // if (env.u1mode[VDP_1] == FBM_MODE_1080I) // not support pscan-disp mode
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1100, 1082);
                    prPool->u4Width = 1100;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1100, 900);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1100;
                    prPool->u4Height = 900;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }
                }
                // else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr,FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 750, 590);
                    prPool->u4Width = 750;
                    prPool->u4Height = 590;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_576I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 750, 590);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 750;
                    prPool->u4Height = 590;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }
                }
                // else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(1100, 1082, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 1100;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_422;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1100, 900);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1100;
                    prPool->u4Height = 900;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }
                }
                // else if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1100, 900);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1100;
                    prPool->u4Height = 900;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_444;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }

                    {
                        // dummy allocation
                        // main di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 576;
                        prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }
            }

            // error checking
            prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
            if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4MainPopMaxEnd)
            {
                LOG(1, "main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPopMaxMode);
                VERIFY(0);
            }
            if (_fbm_u4NptvSubPopAddr <= _FBM_NPTV_VideoRange.u4MainPopMaxEnd)
            {
                LOG(1, "main overwrite sub pop, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPopMaxMode);
                VERIFY(0);
            }

            // sub path modules (1920/2)x(1080/2)
            // if (env.u1mode[VDP_2] == FBM_MODE_1080I) // not support pscan-disp mode
            {
                // sub di
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_MDDI_ADDR_ALIGMENT);
                prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 2048/2, 1082);
                prPool->u4Width = 2048/2;
                prPool->u4Height = 1082;
                prPool->u4Mode = 0;

                u1Mode = FBM_TYPE_SUB_POP_MODE_1080I;
                u1Type = FBM_SUB_PSCAN;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                // sub scaler
                prPrevPool = prPool;
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048/2, 576);
                prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 2048/2;
                prPool->u4Height = 900;

                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }
            }
            // else if (env.u1mode[VDP_2] == FBM_MODE_576I)
            {
                // sub di
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr,FBM_MDDI_ADDR_ALIGMENT);
                prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 750, 590);
                prPool->u4Width = 750;
                prPool->u4Height = 590;
                prPool->u4Mode = 0;

                u1Mode = FBM_TYPE_SUB_POP_MODE_576I;
                u1Type = FBM_SUB_PSCAN;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                // sub scaler
                prPrevPool = prPool;
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 750, 590);
                prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 750;
                prPool->u4Height = 590;

                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }
            }
            // else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
            {
                // sub di
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_MDDI_ADDR_ALIGMENT);
                prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(2048/2, 1082, 2), FBM_MDDI_SIZE_ALIGMENT);
                prPool->u4Width = 2048/2;
                prPool->u4Height = 1082;
                prPool->u4Mode = 0;

                u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_422;
                u1Type = FBM_SUB_PSCAN;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                // sub scaler
                prPrevPool = prPool;
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048/2, 576);
                prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 2048/2;
                prPool->u4Height = 900;

                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }
            }
            // else if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
            {
                // sub scaler
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048/2, 576);
                prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 2048/2;
                prPool->u4Height = 900;

                u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_444;
                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }

                {
                    // dummy allocation
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                }
            }

            // error checking
            prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
            if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4SubPopMaxEnd)
            {
                LOG(1, "sub overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1SubPopMaxMode);
                VERIFY(0);
            }
        }
    }

    _FbmGetEnv(&env);
    LOG(6, "u1mode(%d %d) u1IsMPEG(%d, %d)\n", env.u1mode[VDP_1], env.u1mode[VDP_2],env.u1IsMPEG[VDP_1], env.u1IsMPEG[VDP_2]);

    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
    {
        UINT8 u1Mode, u1Type;

        u1Type = FBM_TDC;
        if (env.u1IsTDC[VDP_1] == 1)
        {
            u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
        }
        // for tdc in sub pop
        else // if (env.u1IsTDC[VDP_2] == 1)
        {
            u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
        }
        prPool = FBM_GetPoolInfo(_eCurrentTdc);
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate overange\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

#ifdef CC_SUPPORT_SCART_OUT_IN_MMP
        if (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF)
        {
            if (env.u1IsTDC[VDP_2] == 1)
            {
                prPool->u4Addr = _fbm_u4NptvBeAddr + (prPool->u4Addr-_fbm_u4NptvSubPipAddr);
            }
        }
#endif

        return prPool;
#if 0
        prPool = FBM_GetPoolInfo(_eCurrentTdc);
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly

#if 0 // shared with MPEG
        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate overange\n");
            ASSERT(0);
        }
#endif

        return prPool;
#endif
    }
    if (((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)))
    {
        UINT8 u1Mode, u1Type;
#if 0
        if (prInfo->u4SourceType == (UINT32)VSS_DTV)
        {
            // assumption: single
            if (env.u1mode[VDP_1] == FBM_MODE_1080I)
            {
                u1Mode = FBM_TYPE_MM_1080I_MODE;
            }
            else if (env.u1mode[VDP_1] == FBM_MODE_576I)
            {
                u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
            }
            else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
            {
                u1Mode = FBM_TYPE_MM_1080P_MODE;
            }
        }
        else
#endif
            if (env.u1fgSingle || env.u1fgPIP)
            {
                if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                        (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
                {
                    if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                    {
                        u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                    }
                    else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                    {
                        u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                    }
                    else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                    {
                        u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                    }
                    else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                    {
                        u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                    }
                }
                else
                {
                    if (env.u1mode[VDP_2] == FBM_MODE_1080I)
                    {
                        u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                    }
                    else if (env.u1mode[VDP_2] == FBM_MODE_576I)
                    {
                        u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
                    }
                    else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
                    {
                        u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_422;
                    }
                    else // if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
                    {
                        u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                    }
                }
            }
            else // POP
            {
                if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                        (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
                {
                    // main
                    if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                    {
                        u1Mode = FBM_TYPE_MAIN_POP_MODE_1080I;
                    }
                    else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                    {
                        u1Mode = FBM_TYPE_MAIN_POP_MODE_576I;
                    }
                    else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                    {
                        u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_422;
                    }
                    else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                    {
                        u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_444;
                    }
                }
                else
                {
                    // sub
                    if (env.u1mode[VDP_2] == FBM_MODE_1080I)
                    {
                        u1Mode = FBM_TYPE_SUB_POP_MODE_1080I;
                    }
                    else if (env.u1mode[VDP_2] == FBM_MODE_576I)
                    {
                        u1Mode = FBM_TYPE_SUB_POP_MODE_576I;
                    }
                    else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
                    {
                        u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_422;
                    }
                    else // if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
                    {
                        u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_444;
                    }
                }
            }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
            u1Type = FBM_MAIN_PSCAN;
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)
            if ((SRM_GetScposMode(VDP_1) & FBM_POOL_MODE_SCPOS_PSCAN_DISP) &&
                    ((env.u1mode[VDP_1] != FBM_MODE_1080I)))
            {
                u1Type = FBM_MAIN_SCALER;
            }
#endif
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
            u1Type = FBM_MAIN_SCALER;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
            u1Type = FBM_SUB_PSCAN;
        }
        else // if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
            u1Type = FBM_SUB_SCALER;
        }
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly
        LOG(3, "Load Index: (u1Mode, u1Type): (%x, %x)\n", u1Mode, u1Type);

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate over-range\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

#ifdef CC_SUPPORT_SCART_OUT_IN_MMP
        if (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF)
        {
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) || (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
            {
                prPool->u4Addr = _fbm_u4NptvBeAddr + (prPool->u4Addr-_fbm_u4NptvSubPipAddr);
            }
        }
#endif

        return prPool;
    }

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}

#if 0 // defined(CC_MT5395)
#define SAVE_FBM_RESULT(mode, type)\
    do { \
        u1Mode = mode;\
        _FBM_NPTV_VIDEO_CONF[mode][type].u4Addr = prPool->u4Addr;\
        _FBM_NPTV_VIDEO_CONF[mode][type].u4Size = prPool->u4Size;\
        _FBM_NPTV_VIDEO_CONF[mode][type].u4Width = prPool->u4Width;\
        _FBM_NPTV_VIDEO_CONF[mode][type].u4Height = prPool->u4Height;\
        _FBM_NPTV_VIDEO_CONF[mode][type].u4Mode = prPool->u4Mode;\
    } while (0)

#define EXPAND_POOL_BY_TYPE_1(TYPE, PATH, WIDTH, HEIGHT, SIZE, MODE)\
    do { \
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_ ## TYPE ## _DYN_ ##PATH);\
        prPool->u4Addr = FBM_ALIGN_MASK(u4NextDramAddr, FBM_ ## TYPE ## _ADDR_ALIGMENT);\
        prPool->u4Size = FBM_ALIGN_MASK(SIZE, FBM_ ## TYPE ## _SIZE_ALIGMENT); \
        prPool->u4Width = WIDTH; \
        prPool->u4Height = HEIGHT; \
        prPool->u4Mode = MODE; \
        u4NextDramAddr = prPool->u4Addr + prPool->u4Size;\
    } while (0)

#define EXPAND_POOL_BY_TYPE_2(TYPE, WIDTH, HEIGHT, SIZE, MODE)\
    do { \
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_ ## TYPE ## _DYN);\
        prPool->u4Addr = FBM_ALIGN_MASK(u4NextDramAddr, FBM_ ## TYPE ## _ADDR_ALIGMENT);\
        prPool->u4Size = FBM_ALIGN_MASK(SIZE, FBM_ ## TYPE ## _SIZE_ALIGMENT); \
        prPool->u4Width = WIDTH; \
        prPool->u4Height = HEIGHT; \
        prPool->u4Mode = MODE; \
        u4NextDramAddr = prPool->u4Addr + prPool->u4Size;\
    } while (0)

static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5395_128MB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        UINT8 u1Mode, u1Idx;
        UINT32 u4NextDramAddr;

        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
        _fbm_u4NptvBeAddr = (prPool->u4Addr + prPool->u4Size);
        prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        _fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        /* set _fbm_u4NptvSubPipAddr in the following case */

        _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
        _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
        _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
        _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
        _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;

        //if (env.u1fgSingle || env.u1fgPIP)
        {
            // main modules
            {
                u1Idx = 0;
                do {
                    u4NextDramAddr = _fbm_u4NptvBeAddr;
                    if (u1Idx == 0)
                    {
                        // if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                        {
                            // main di
#if defined(__MODEL_slt__)
#define CONFIG_MAIN_PSCAN_1080I_IN_SINGLE_PIP \
                            EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 2048, 1082, FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 4, 4), 0);
#elif defined(CC_FLIP_MIRROR_SUPPORT)
#define CONFIG_MAIN_PSCAN_1080I_IN_SINGLE_PIP \
                            EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 2048, 1082, FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 6, 6), 0);
#else
#define CONFIG_MAIN_PSCAN_1080I_IN_SINGLE_PIP \
                            EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 2048, 1082, FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 5, 5), 0);
#endif
                            CONFIG_MAIN_PSCAN_1080I_IN_SINGLE_PIP;
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_1080I, FBM_MAIN_PSCAN);

                            // main scaler: for EPG mode
#if defined(__MODEL_slt__)
#define CONFIG_MAIN_SCALER_1080I_IN_SINGLE_PIP \
                            EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 2048, 1082, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082),\
                                    FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
#else
#define CONFIG_MAIN_SCALER_1080I_IN_SINGLE_PIP \
                            EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 780, 578, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578),\
                                    FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
#endif
                            CONFIG_MAIN_SCALER_1080I_IN_SINGLE_PIP;
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_1080I, FBM_MAIN_SCALER);
                        }
                    }
                    else if (u1Idx == 1)
                    {
                        //if (env.u1mode[VDP_1] == FBM_MODE_576I)
                        {
                            // main di
#if defined(CC_SUPPORT_OVERSAMPLE_1440_ENABLE)
                            EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 1440, 578, FBM_MDDI_BI_10BIT_422_SIZE(1440, 578, 6, 6), 0);
#else
                            EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 780, 578, FBM_MDDI_BI_10BIT_422_SIZE(780, 578, 4, 4), 0);
#endif
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_576I, FBM_MAIN_PSCAN);

                            // main scaler
#if defined(CC_SUPPORT_OVERSAMPLE_1440_ENABLE)
                            EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 780, 578, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 578),\
                                                  FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
#else
                            EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 780, 578, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578),\
                                                  FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
#endif
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_576I, FBM_MAIN_SCALER);

                            // tdc in main
                            EXPAND_POOL_BY_TYPE_2(TDC, 720, 576, FBM_TDC_POOL_SIZE, FBM_POOL_MODE_10BIT);
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_576I, FBM_TDC);
                        }
                    }
                    else if (u1Idx == 2)
                    {
                        // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                        {
                            // main di
                            EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 2048, 1082, FBM_MDDI_BI_NR_10BIT_SIZE(2048, 1082), 0);
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_1080P_422, FBM_MAIN_PSCAN);

                            // main scaler
                            EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 2048, 1082, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082),\
                                                  FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_1080P_422, FBM_MAIN_SCALER);
                        }
                    }
                    else if (u1Idx == 3)
                    {
                        //if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                        {
                            {
                                // dummy allocation
                                // main di
                                u4NextDramAddr = _fbm_u4NptvBeAddr;
                                EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 720, 576, FBM_MDDI_BI_10BIT_422_SIZE(720, 576, 4, 4), FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR);
                                SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_1080P_444, FBM_MAIN_PSCAN);
                            }
                            u4NextDramAddr = _fbm_u4NptvBeAddr;
                            // main scaler
                            EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 2048, 1082, FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082),\
                                                  FBM_POOL_MODE_8BIT|FBM_POOL_MODE_444_FB|FBM_POOL_MODE_SCPOS_3FB);
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_PIP_MODE_1080P_444, FBM_MAIN_SCALER);
                        }
                    }
                    else
                    {
                        break;
                    }

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

                    u1Idx++;
                } while (1);

                _fbm_u4NptvSubPipAddr = _FBM_NPTV_VideoRange.u4MainPipMaxEnd + 0x10000;
                // error checking
                if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
                {
                    LOG(1, "main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                    ASSERT(0);
                }
            }

            // if (env.u1fgPIP) // sub modules
            {
                u1Idx = 0;
                do {
                    u4NextDramAddr = _fbm_u4NptvSubPipAddr;
                    if (u1Idx == 0)
                    {
                        //if (env.u1Interlace[VDP_2] == 1) // 1080i
                        {
                            // sub di
                            EXPAND_POOL_BY_TYPE_1(MDDI, SUB, 780, 1080, FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), 0); // c bob
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_1080I, FBM_SUB_PSCAN);

                            // sub scaler
                            EXPAND_POOL_BY_TYPE_1(SCPOS, SUB, 720, 576, FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_POOL_MODE_8BIT|FBM_POOL_MODE_444_FB|FBM_POOL_MODE_SCPOS_3FB);
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_1080I, FBM_SUB_SCALER);
                        }
                    }
                    else if (u1Idx == 1)
                    {
                        //if (env.u1Interlace[VDP_2] == 1) // 576i
                        {
                            // sub di
                            EXPAND_POOL_BY_TYPE_1(MDDI, SUB, 720, 576, FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), 0); // yc 3d
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_576I, FBM_SUB_PSCAN);

                            // sub scaler
                            EXPAND_POOL_BY_TYPE_1(SCPOS, SUB, 720, 576, FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT),\
                                                  FBM_POOL_MODE_8BIT|FBM_POOL_MODE_444_FB|FBM_POOL_MODE_SCPOS_3FB);
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_576I, FBM_SUB_SCALER);

                            // tdc in sub
                            EXPAND_POOL_BY_TYPE_2(TDC, 720, 576, FBM_TDC_POOL_SIZE, FBM_POOL_MODE_10BIT);
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_576I, FBM_TDC);
                        }
                    }
                    else if (u1Idx == 2)
                    {
                        // 1080P422, 1080P444
                        {
                            // dummy allocation
                            // sub di
                            u4NextDramAddr = _fbm_u4NptvSubPipAddr;
                            EXPAND_POOL_BY_TYPE_1(MDDI, SUB, 720, 1080, FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), 0);
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_1080P_444, FBM_SUB_PSCAN);
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_1080P_422, FBM_SUB_PSCAN);
                        }

                        // sub scaler
                        u4NextDramAddr = _fbm_u4NptvSubPipAddr;
                        EXPAND_POOL_BY_TYPE_1(SCPOS, SUB, 760, 580, FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 760, 580),\
                                              FBM_POOL_MODE_8BIT|FBM_POOL_MODE_444_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_1080P_444, FBM_SUB_SCALER);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_PIP_MODE_1080P_422, FBM_SUB_SCALER);
                    }
                    else
                    {
                        break;
                    }

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }

                    u1Idx++;
                } while (1);

                // error checking
                if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= _FBM_NPTV_VideoRange.u4SubPipMaxEnd)
                {
                    LOG(1, "sub overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1SubPipMaxMode);
                    ASSERT(0);
                }
            }
        }
        // else // POP: take PAP mode into consideration (1100x900)
        {
            u1Idx = 0;
            do {
                u4NextDramAddr = _fbm_u4NptvBeAddr;
                if (u1Idx == 0)
                {
                    // if (env.u1mode[VDP_1] == FBM_MODE_1080I) // not support pscan-disp mode
                    {
                        // main di
                        EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 1100, 1082, FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1100, 1082), 0);
                        SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_1080I, FBM_MAIN_PSCAN);

                        // main scaler
                        EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 1100, 900, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1100, 900),\
                                              FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_1080I, FBM_MAIN_SCALER);
                    }
                }
                else if (u1Idx == 1)
                {
                    // else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                    {
                        // main di
                        EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 750, 590, FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 750, 590), 0);
                        SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_576I, FBM_MAIN_PSCAN);

                        // main scaler
                        EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 750, 590, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 750, 590),\
                                              FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_576I, FBM_MAIN_SCALER);
                    }
                }
                else if (u1Idx == 2)
                {
                    // else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                    {
                        // main di
                        EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 1100, 1082, FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(1100, 1082), FBM_MDDI_SIZE_ALIGMENT), 0);
                        SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_1080P_422, FBM_MAIN_PSCAN);

                        // main scaler
                        EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 1100, 900, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1100, 900),\
                                              FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_1080P_422, FBM_MAIN_SCALER);
                    }
                }
                else if (u1Idx == 3)
                {
                    // else if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                    {
                        {
                            // dummy allocation
                            u4NextDramAddr = _fbm_u4NptvBeAddr;
                            // main di
                            EXPAND_POOL_BY_TYPE_1(MDDI, MAIN, 720, 576, FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT),\
                                                  FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR);
                            SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_1080P_444, FBM_MAIN_PSCAN);
                        }
                        u4NextDramAddr = _fbm_u4NptvBeAddr;
                        // main scaler
                        EXPAND_POOL_BY_TYPE_1(SCPOS, MAIN, 1100, 900, FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1100, 900),\
                                              FBM_POOL_MODE_8BIT|FBM_POOL_MODE_444_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_MAIN_POP_MODE_1080P_444, FBM_MAIN_SCALER);
                    }
                }
                else
                {
                    break;
                }

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                    _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                }

                u1Idx++;
            } while (1);

            // error checking
            if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= _FBM_NPTV_VideoRange.u4MainPopMaxEnd)
            {
                Printf("main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPopMaxMode);
                ASSERT(0);
            }
            if (_fbm_u4NptvSubPopAddr <= _FBM_NPTV_VideoRange.u4MainPopMaxEnd)
            {
                Printf("main overwrite sub pop, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPopMaxMode);
                ASSERT(0);
            }

            // sub path modules (1920/2)x(1080)
            u1Idx = 0;
            do {
                u4NextDramAddr = _fbm_u4NptvSubPopAddr;
                if (u1Idx == 0)
                {
                    // if (env.u1mode[VDP_2] == FBM_MODE_1080I) // not support pscan-disp mode
                    {
                        // sub di
                        EXPAND_POOL_BY_TYPE_1(MDDI, SUB, 2048/2, 1082, FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 2048/2, 1082), 0);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_1080I, FBM_SUB_PSCAN);

                        // sub scaler
                        EXPAND_POOL_BY_TYPE_1(SCPOS, SUB, 2048/2, 1082, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048/2, 1082),\
                                              FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_1080I, FBM_SUB_SCALER);
                    }
                }
                else if (u1Idx == 1)
                {
                    // else if (env.u1mode[VDP_2] == FBM_MODE_576I)
                    {
                        // sub di
                        EXPAND_POOL_BY_TYPE_1(MDDI, SUB, 750, 590, FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 750, 590), 0);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_576I, FBM_SUB_PSCAN);

                        // sub scaler
                        EXPAND_POOL_BY_TYPE_1(SCPOS, SUB, 750, 590, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 750, 590),\
                                              FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_576I, FBM_SUB_SCALER);
                    }
                }
                else if (u1Idx == 2)
                {
                    // else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
                    {
                        // sub di
                        EXPAND_POOL_BY_TYPE_1(MDDI, SUB, 2048/2, 1082, FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(2048/2, 1082), FBM_MDDI_SIZE_ALIGMENT), 0);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_1080P_422, FBM_SUB_PSCAN);

                        // sub scaler
                        EXPAND_POOL_BY_TYPE_1(SCPOS, SUB, 2048/2, 1082, FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048/2, 1082),\
                                              FBM_POOL_MODE_10BIT|FBM_POOL_MODE_422_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_1080P_422, FBM_SUB_SCALER);
                    }
                }
                else if (u1Idx == 3)
                {
                    // else if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
                    {
                        {
                            // dummy allocation
                            u4NextDramAddr = _fbm_u4NptvSubPopAddr;
                            // sub di
                            EXPAND_POOL_BY_TYPE_1(MDDI, SUB, 720, 576, FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT),\
                                                  FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR);
                            SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_1080P_444, FBM_MAIN_PSCAN);
                        }
                        // sub scaler
                        u4NextDramAddr = _fbm_u4NptvSubPopAddr;
                        EXPAND_POOL_BY_TYPE_1(SCPOS, SUB, 2048/2, 1082, FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048/2, 1082),\
                                              FBM_POOL_MODE_8BIT|FBM_POOL_MODE_444_FB|FBM_POOL_MODE_SCPOS_3FB);
                        SAVE_FBM_RESULT(FBM_TYPE_SUB_POP_MODE_1080P_444, FBM_SUB_SCALER);
                    }
                }
                else
                {
                    break;
                }

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }

                u1Idx++;
            } while (1);

            // error checking
            if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= _FBM_NPTV_VideoRange.u4SubPopMaxEnd)
            {
                LOG(1, "sub overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1SubPopMaxMode);
                ASSERT(0);
            }
        }

        Printf("u4MainPipMaxSize+u4SubPipMaxSize: 0x%x\n", _FBM_NPTV_VideoRange.u4MainPipMaxSize + _FBM_NPTV_VideoRange.u4SubPipMaxSize);
        Printf("u4MainPopMaxSize+u4SubPopMaxSize: 0x%x\n", _FBM_NPTV_VideoRange.u4MainPopMaxSize + _FBM_NPTV_VideoRange.u4SubPopMaxSize);
    }

    _FbmGetEnv(&env);
    LOG(6, "u1mode(%d %d) u1IsMPEG(%d, %d)\n", env.u1mode[VDP_1], env.u1mode[VDP_2],env.u1IsMPEG[VDP_1], env.u1IsMPEG[VDP_2]);

    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
    {
        UINT8 u1Mode, u1Type;

        u1Type = FBM_TDC;
        if (env.u1IsTDC[VDP_1] == 1)
        {
            u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
        }
        // for tdc in sub pop
        else // if (env.u1IsTDC[VDP_2] == 1)
        {
            u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
        }
        prPool = FBM_GetPoolInfo(_eCurrentTdc);
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate overange\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            ASSERT(0);
        }

#ifdef CC_SUPPORT_SCART_OUT_IN_MMP
        if (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF)
        {
            if (env.u1IsTDC[VDP_2] == 1)
            {
                prPool->u4Addr = _fbm_u4NptvBeAddr + (prPool->u4Addr-_fbm_u4NptvSubPipAddr);
            }
        }
#endif

        return prPool;
    }
    if (((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)))
    {
        UINT8 u1Mode, u1Type;
        if (env.u1fgSingle || env.u1fgPIP)
        {
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                    (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
            {
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                }
            }
            else
            {
                if (env.u1mode[VDP_2] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                }
            }
        }
        else // POP
        {
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                    (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
            {
                // main
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_576I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_444;
                }
            }
            else
            {
                // sub
                if (env.u1mode[VDP_2] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_1080I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_576I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_444;
                }
            }
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
            u1Type = FBM_MAIN_PSCAN;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
            u1Type = FBM_MAIN_SCALER;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
            u1Type = FBM_SUB_PSCAN;
#if defined(CC_MT5365) || defined(CC_MT5395)
            if ((SRM_GetScposMode(VDP_1) & FBM_POOL_MODE_SCPOS_PSCAN_DISP) &&
                    ((env.u1mode[VDP_1] != FBM_MODE_1080I)))
            {
                u1Type = FBM_MAIN_SCALER;
            }
#endif
        }
        else // if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
            u1Type = FBM_SUB_SCALER;
        }
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly
        LOG(3, "Load Index: (u1Mode, u1Type): (%x, %x)\n", u1Mode, u1Type);

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate over-range\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            ASSERT(0);
        }

#ifdef CC_SUPPORT_SCART_OUT_IN_MMP
        if (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF)
        {
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) || (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP))
            {
                prPool->u4Addr = _fbm_u4NptvBeAddr + (prPool->u4Addr-_fbm_u4NptvSubPipAddr);
            }
        }
#endif

        return prPool;
    }


		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}
#endif

static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5365_ATV_64MB(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;
    static UINT8 fgInited = FALSE;
    FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        UINT8 u1Mode, u1Type;

        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr =prPool->u4Addr;
        prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        _fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;
        /* set _fbm_u4NptvSubPipAddr in the following case */

        _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
        _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
        _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
        _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
        _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;
        //if (env.u1fgSingle || env.u1fgPIP)
        {
            // main modules
            {
                // if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);

                    LOG(6, "main fbm begin	address %x \n",prPool->u4Addr);
#if defined(__MODEL_slt__)
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
#elif defined(CC_FLIP_MIRROR_SUPPORT)
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
#endif
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    LOG(6, "main di 1080i size %x \n",prPool->u4Size);

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler: for EPG mode
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;
#if defined(__MODEL_slt__)
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
#elif defined(CC_MIRROR_SW_WA)
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
#endif

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    LOG(6, "main scaler 1080i size %x \n",prPool->u4Size);
                    LOG(6, "main scaler 1080i end address %x \n",prPool->u4Addr+prPool->u4Size);
                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

#if defined(__MODEL_slt__)
                    _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
#endif

#if defined(CC_MIRROR_SW_WA)
                    _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
#endif

                }

                //if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr,FBM_MDDI_ADDR_ALIGMENT);
#ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_1440_BI_10BIT_422_SIZE(1440, 578, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 1440;
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(780, 578, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 780;
#endif
                    prPool->u4Height = 578;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
#ifdef CC_MIRROR_SW_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 578);
#else
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 578);
#endif
#else
#ifdef CC_MIRROR_SW_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
#else
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
#endif
#endif
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
#ifdef CC_SUPPORT_OVERSAMPLE_1440_ENABLE
                    prPool->u4Width = 1440;
#else
                    prPool->u4Width = 780;
#endif
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // tdc in main
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPrevPool = prPool;

                    u1Type = FBM_TDC;
                    LOG(6, "main 576i end address %x \n",prPool->u4Addr+prPool->u4Size);
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

                // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(780, 1082, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 780;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_MIRROR_SW_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 1082);
#else
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 1082);
#endif
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1440;
                    prPool->u4Height = 1082;

                    LOG(6, "main 1080p422 end address %x \n",prPool->u4Addr+prPool->u4Size);
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

#if !defined(CC_MIRROR_SW_WA)
#if !defined(__MODEL_slt__)
                _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
#endif
#endif
                //if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 1082);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1440;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    LOG(6, "main 1080p444 end address %x \n",prPool->u4Addr+prPool->u4Size);
                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

                    {
                        // dummy allocation
                        // main di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 576;
                        prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }

#if 1
                // MM 1080P cases
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1024, 1082);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1024;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    LOG(6, "main 1080p MM scaler end address %x \n",prPool->u4Addr+prPool->u4Size);
                    {
                        // dummy allocation: scaler will get and assert the prPool in vDrvScpipSrmDispCb
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(0, 0, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 0;
                        prPool->u4Height = 0;

                        u1Mode = FBM_TYPE_MM_1080P_MODE;
                        u1Type = FBM_MAIN_PSCAN;

                        LOG(6, "main 1080p MM di end address %x \n",prPool->u4Addr+prPool->u4Size);
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }

                // MM 1080I cases
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    //prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(2048, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_422_SIZE(2048, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MM_1080I_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    LOG(6, "main 1080i MM di end address %x \n",prPool->u4Addr+prPool->u4Size);
                    {
                        // dummy allocation: scaler will get and assert the prPool in vDrvScpipSrmDispCb
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 0, 0);
                        prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                        prPool->u4Width = 0;
                        prPool->u4Height = 0;

                        u1Mode = FBM_TYPE_MM_1080I_MODE;
                        u1Type = FBM_MAIN_SCALER;

                        LOG(6, "main 1080i MM scaler end address %x \n",prPool->u4Addr+prPool->u4Size);
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }
                // MM 576I cases
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    //prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(720, 576, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_422_SIZE(780, 580, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 780;
                    prPool->u4Height = 580;

                    u1Mode = FBM_TYPE_MM_576I_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    LOG(6, "main 576i MM di end address %x \n",prPool->u4Addr+prPool->u4Size);
                    {
                        prPrevPool=prPool;
                        // dummy allocation: scaler will get and assert the prPool in vDrvScpipSrmDispCb
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(prPrevPool->u4Addr+prPrevPool->u4Size, FBM_SCPOS_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 580);
                        prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                        prPool->u4Width = 780;
                        prPool->u4Height = 580;

                        u1Mode = FBM_TYPE_MM_576I_MODE;
                        u1Type = FBM_MAIN_SCALER;

                        LOG(6, "main 576i MM scaler end address %x \n",prPool->u4Addr+prPool->u4Size);
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }
#endif

                // error checking
                prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
                if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
                {
                    LOG(1, "main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                    VERIFY(0);
                }
                if (_fbm_u4NptvSubPipAddr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
                {
                    LOG(1, "main overwrite sub pip, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                    VERIFY(0);
                }
            }
            // if (env.u1fgPIP) // sub modules
            {
                //if (env.u1Interlace[VDP_2] == 1) // 1080i
                {
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), FBM_MDDI_SIZE_ALIGMENT); // c bob
                    prPool->u4Width = 720;
                    prPool->u4Height = 1080;
                    prPool->u4Mode = 0;
                    LOG(6, "sub 1080i di size %x \n",prPool->u4Size);

                    // 1080i
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // sub scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(prPrevPool->u4Addr + prPrevPool->u4Size, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    LOG(6, "sub 1080i scaler size %x \n",prPool->u4Size);

                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }
                }
                //if (env.u1Interlace[VDP_2] == 1) // 576i
                {
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT); // yc 3d
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPool->u4Mode = 0;

                    // 576i
                    LOG(6, "sub 576 di size %x \n",prPool->u4Size);
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // sub scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(prPrevPool->u4Addr + prPrevPool->u4Size, FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_TVE_US_IMAGERESIZER_WA
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT) * 2;
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT);
#endif
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    LOG(6, "sub 576i scaler size %x \n",prPool->u4Size);

                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // tdc in sub
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    LOG(6, "sub 576i tdc size %x \n",prPool->u4Size);

                    u1Type = FBM_TDC;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }
                }
                //else
                {
                    {
                        // dummy allocation
                        // sub di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 1080;
                        prPool->u4Mode = 0;

                        u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                        u1Type = FBM_SUB_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }

                    // sub scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_SCPOS_ADDR_ALIGMENT);
#ifdef CC_TVE_US_IMAGERESIZER_WA
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 760, 580) * 2;
#else
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 760, 580);
#endif
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 760;
                    prPool->u4Height = 580;

                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }

                    // 1080P422 is the same as 1080P444
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Mode;

                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Mode;
                }

                // error checking
                prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
                if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4SubPipMaxEnd)
                {
                    LOG(1, "sub overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1SubPipMaxMode);
                    VERIFY(0);
                }
            }
        }

    }

    _FbmGetEnv(&env);
    LOG(6, "u1mode(%d %d) u1IsMPEG(%d, %d)\n", env.u1mode[VDP_1], env.u1mode[VDP_2],env.u1IsMPEG[VDP_1], env.u1IsMPEG[VDP_2]);

    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
    {
        UINT8 u1Mode, u1Type;

        u1Type = FBM_TDC;
        if (env.u1IsTDC[VDP_1] == 1)
        {
            u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
        }
        // for tdc in sub pop
        else // if (env.u1IsTDC[VDP_2] == 1)
        {
            u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
        }
        prPool = FBM_GetPoolInfo(_eCurrentTdc);
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate overange\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

        return prPool;
    }
    if (((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)))
    {
        UINT8 u1Mode, u1Type;
        if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
        {
#if 1
            if (prInfo->u4SourceType == (UINT32)VSS_DTV)
            {
                // assumption: single
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MM_1080I_MODE;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MM_576I_MODE;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                }
            }
            else
#endif
            {
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                }
            }
        }
        else
        {
            if (env.u1mode[VDP_2] == FBM_MODE_1080I)
            {
                u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
            }
            else if (env.u1mode[VDP_2] == FBM_MODE_576I)
            {
                u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
            }
            else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
            {
                u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_422;
            }
            else // if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
            {
                u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
            }
        }


        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
            u1Type = FBM_MAIN_PSCAN;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
            u1Type = FBM_MAIN_SCALER;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
            u1Type = FBM_SUB_PSCAN;
        }
        else // if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
            u1Type = FBM_SUB_SCALER;
        }
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly
        LOG(3, "Load Index: (u1Mode, u1Type): (%x, %x)\n", u1Mode, u1Type);

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate over-range\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

        return prPool;
    }

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}


static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5387_64MB_FHD_Single(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;

    static UINT8 fgInited = FALSE;
    FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        UINT8 u1Mode, u1Type;

        fgInited = TRUE;
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;

        _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
        _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
        //_eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
        //_eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
        _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;

        //if (env.u1fgSingle || env.u1fgPIP)
        {
            // if (env.u1IsMPEG[VDP_1] == 0)
            {
                // if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
#ifdef CC_FLIP_MIRROR_SUPPORT
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(2048, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
#endif
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler: for EPG mode
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

                // if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr,FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(780, 578, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // tdc in main
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPrevPool = prPool;

                    u1Type = FBM_TDC;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

                // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(2048, 1082, 2);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

                //if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 2048, 1082);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

                    {
                        // dummy allocation
                        // main di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 576;
                        prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }

                // MM && interlace
                {
                    prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);

                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size),FBM_MDDI_ADDR_ALIGMENT);
#if defined(CC_FLIP_MIRROR_DOT_BY_DOT)
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1920, 1080, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
#elif defined(CC_FLIP_MIRROR_SUPPORT)
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1440, 1080, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
#elif defined(CC_DTV_HD_DOT_BY_DOT)
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1920, 1080, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1440, 1080, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
#endif
#if defined(CC_DTV_HD_DOT_BY_DOT) && defined(CC_FLIP_MIRROR_DOT_BY_DOT)
                    prPool->u4Width = 1920;
#else
                    prPool->u4Width = 1440; // bandwidth issue
#endif
                    prPool->u4Height = 1080;
                    prPool->u4Mode = 0;

#if defined(CC_DTV_HD_DOT_BY_DOT) && defined(CC_FLIP_MIRROR_DOT_BY_DOT)
#error "CC_DTV_HD_DOT_BY_DOT and CC_FLIP_MIRROR_DOT_BY_DOT are exclusive"
#endif

                    u1Mode = FBM_TYPE_MM_1080I_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size)-(prPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // FBM_TYPE_MM_576I_MODE is the same as FBM_TYPE_MM_1080I_MODE
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Mode;

                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Mode;

                    // error checking
                    if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= (prPool->u4Addr)+(prPool->u4Size))
                    {
                        Printf("mm 1080i mode overwrite DSP\n");
                        Printf("dsp begin: 0x%x\n", FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr);
                        Printf("the end of the last one: 0x%x\n", (prPool->u4Addr)+(prPool->u4Size));
                        VERIFY(0);
                    }
                }

                // MM && progressive
                {
                    prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);

                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
#if defined(CC_DTV_HD_DOT_BY_DOT)
                    prPool->u4Size = FBM_SCPOS_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1920, 1080); // 1080p, 8bit
#else
                    prPool->u4Size = FBM_SCPOS_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 1080); // 1080p, 8bit
#endif
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
#if defined(CC_DTV_HD_DOT_BY_DOT)
                    prPool->u4Width = 1920;
#else
                    prPool->u4Width = 1440;
#endif
                    prPool->u4Height = 1080;

                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // error checking
                    if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= (prPool->u4Addr)+(prPool->u4Size))
                    {
                        Printf("mm 1080p mode overwrite DSP\n");
                        Printf("dsp begin: 0x%x\n", FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr);
                        Printf("the end of the last one: 0x%x\n", (prPool->u4Addr)+(prPool->u4Size));
                        VERIFY(0);
                    }

                    // main di: NR is only enabed in SD timing
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(750, 580, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size)-prPool->u4Size-FBM_MDDI_ADDR_ALIGMENT, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Width = 750;
                    prPool->u4Height = 580;
                    prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                }
            }

            // error checking
            prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
            if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
            {
                Printf("main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                Printf("size: 0x%x\n", _FBM_NPTV_VideoRange.u4MainPipMaxSize);
                VERIFY(0);
            }
        }
    }

    _FbmGetEnv(&env);
    LOG(6, "u1mode(%d %d) u1IsMPEG(%d, %d)\n", env.u1mode[VDP_1], env.u1mode[VDP_2],env.u1IsMPEG[VDP_1], env.u1IsMPEG[VDP_2]);

    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
    {
        prPool = FBM_GetPoolInfo(_eCurrentTdc);
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly

        return prPool;
    }

    if (((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)))
    {
        UINT8 u1Mode, u1Type;
        if (prInfo->u4SourceType == (UINT32)VSS_DTV)
        {
            // assumption: single
            if (env.u1mode[VDP_1] == FBM_MODE_1080I)
            {
                u1Mode = FBM_TYPE_MM_1080I_MODE;
            }
            else if (env.u1mode[VDP_1] == FBM_MODE_576I)
            {
                u1Mode = FBM_TYPE_MM_576I_MODE;
            }
            else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
            {
                //u1Mode = FBM_TYPE_MM_1080P_MODE;
                if (prInfo->u4Resolution <= SRM_VDP_SD_RESOLUTION)
                {
                    u1Mode = FBM_TYPE_MM_576I_MODE;
                }
                else
                {
                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                }
            }
        }
        else
        {
#if 0
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                    (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
#endif
            {
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                }
            }
        }

        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
            u1Type = FBM_MAIN_PSCAN;
        }
        else // if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
            u1Type = FBM_MAIN_SCALER;
        }
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly
#if 0
        if ((prInfo->u4SourceType == (UINT32)VSS_DTV) && (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF))
        {
            prPool->u4Addr += FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER)->u4Size;
        }
#endif
        LOG(3, "Load Index: (u1Mode, u1Type): (%x, %x)\n", u1Mode, u1Type);

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate over-range\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

        return prPool;
    }

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}

static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5363_64MB_FHD_Single(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;

    static UINT8 fgInited = FALSE;
    FBM_POOL_T* prPrevPool;
    FBM_AUTO_INC_ENV_T env;

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        UINT8 u1Mode, u1Type;

        fgInited = TRUE;
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;

        _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
        _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
        _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
        _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
        _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;

        //if (env.u1fgSingle || env.u1fgPIP)
        {
            // if (env.u1IsMPEG[VDP_1] == 0)
            {
                // if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
#ifdef CC_FLIP_MIRROR_SUPPORT
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(1376, 1082, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(1376, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
#endif
                    prPool->u4Width = 1376;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler: for EPG mode
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = 0;//FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = 0;//FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }
                //  Printf("FBM_TYPE_MAIN_PIP_MODE_1080I %x\n",prPool->u4Addr+prPool->u4Size+0x10000);
#if 1//for scart out dram buffer
                // ???
                _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
                // sub di
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), FBM_MDDI_SIZE_ALIGMENT); // c bob
                prPool->u4Width = 720;
                prPool->u4Height = 1080;
                prPool->u4Mode = 0;
                LOG(6, "sub DI size %x \n",prPool->u4Size);
                // 576i
                u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
                u1Type = FBM_SUB_PSCAN;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                // sub scaler
                prPrevPool = prPool;
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(prPrevPool->u4Addr + prPrevPool->u4Size, FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT);
                prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 720;
                prPool->u4Height = 576;
                // Printf("sub scaler size %x \n",prPool->u4Size);
                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                    _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                }

#endif

                // if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr,FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(780, 578, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    //	   Printf("FBM_TYPE_MAIN_PIP_MODE_576I_di %x\n",prPool->u4Size);
                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    // Printf("FBM_TYPE_MAIN_PIP_MODE_576I_scaler %x\n",prPool->u4Size);

                    // tdc in main
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPrevPool = prPool;

                    u1Type = FBM_TDC;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }
                // Printf("FBM_TYPE_MAIN_PIP_MODE_576I %x\n",prPool->u4Addr+prPool->u4Size+0x10000);

                // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    // main di
#if 0
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_MDDI_BI_NR_10BIT_SIZE(2048, 1082);
                    prPool->u4Width = 2048;
                    prPool->u4Height = 1082;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
#else
                    // dummy allocation
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;

                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
#endif
                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1280, 722);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_DISP;
                    prPool->u4Width = 1280;
                    prPool->u4Height = 722;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }
                // Printf("FBM_MODE_1080P_422 %x\n",prPool->u4Addr+prPool->u4Size+0x10000);


                //if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1280, 722);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_DISP;
                    prPool->u4Width = 1280;
                    prPool->u4Height = 722;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

                    {
                        // dummy allocation
                        // main di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 576;
                        prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }
                // Printf("FBM_MODE_1080P_444 %x\n",prPool->u4Addr+prPool->u4Size+0x10000);

                // MM && interlace
                {
                    prPrevPool = FBM_GetPoolInfo( _fbm_u4NptvBeAddr);

                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size),FBM_MDDI_ADDR_ALIGMENT);
                    // prPool->u4Size = FBM_MDDI_420_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1920, 1080);
#ifdef CC_FLIP_MIRROR_SUPPORT
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1440, 1080, 6, 6), FBM_MDDI_SIZE_ALIGMENT);
#else
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1440, 1080, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
#endif
                    prPool->u4Width = 1440; // bandwidth issue
                    prPool->u4Height = 1080;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MM_1080I_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size)-(prPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // FBM_TYPE_MM_576I_MODE is the same as FBM_TYPE_MM_1080I_MODE
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Mode;

                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Mode;

                    // error checking
                    if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= (prPool->u4Addr)+(prPool->u4Size))
                    {
                        Printf("mm 1080i mode overwrite DSP\n");
                        Printf("dsp begin: 0x%x\n", FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr);
                        Printf("the end of the last one: 0x%x\n", (prPool->u4Addr)+(prPool->u4Size));
                        VERIFY(0);
                    }
                }
                // Printf("FBM_TYPE_MM_1080I_MODE %x\n",prPool->u4Addr+prPool->u4Size+0x10000);

                // MM && progressive
                {
                    prPrevPool = FBM_GetPoolInfo( _fbm_u4NptvBeAddr);

                    // main di: NR is only enabed in SD timing
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size),FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(750, 580, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 750;
                    prPool->u4Height = 580;
                    prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Size = FBM_SCPOS_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 1080); // 1080p, 8bit
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1440;
                    prPool->u4Height = 1080;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // error checking
                    if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= (prPool->u4Addr)+(prPool->u4Size))
                    {
                        Printf("mm 1080p mode overwrite DSP\n");
                        Printf("dsp begin: 0x%x\n", FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr);
                        Printf("the end of the last one: 0x%x\n", (prPool->u4Addr)+(prPool->u4Size));
                        VERIFY(0);
                    }
                }
            }
            // Printf("FBM_TYPE_MM_1080P_MODE %x\n",prPool->u4Addr+prPool->u4Size+0x10000);

            // error checking
            prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
            if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
            {
                Printf("main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                Printf("size: 0x%x\n", _FBM_NPTV_VideoRange.u4MainPipMaxSize);
                VERIFY(0);
            }
        }

    }

    _FbmGetEnv(&env);
    LOG(6, "u1mode(%d %d) u1IsMPEG(%d, %d)\n", env.u1mode[VDP_1], env.u1mode[VDP_2],env.u1IsMPEG[VDP_1], env.u1IsMPEG[VDP_2]);

    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
    {
        prPool = FBM_GetPoolInfo(_eCurrentTdc);
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly

        return prPool;
    }

    if (((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)))
    {
        UINT8 u1Mode, u1Type;
        if (prInfo->u4SourceType == (UINT32)VSS_DTV)
        {
            // assumption: single
            if (env.u1mode[VDP_1] == FBM_MODE_1080I)
            {
                u1Mode = FBM_TYPE_MM_1080I_MODE;
            }
            else if (env.u1mode[VDP_1] == FBM_MODE_576I)
            {
                u1Mode = FBM_TYPE_MM_576I_MODE;
            }
            else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
            {
                u1Mode = FBM_TYPE_MM_1080P_MODE;
            }
        }
        else
        {
#if 0
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                    (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
#endif
            {
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                }
            }
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            u1Mode =FBM_TYPE_SUB_PIP_MODE_576I;
            u1Type = FBM_SUB_SCALER;

        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
            u1Type = FBM_MAIN_PSCAN;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
            u1Type = FBM_MAIN_SCALER;
        }
#if 1
        else if(ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
            u1Mode =FBM_TYPE_SUB_PIP_MODE_576I;

            u1Type = FBM_SUB_PSCAN;
        }
        else // if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
            u1Type = FBM_SUB_SCALER;
        }
#endif
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly
#if 1
        if ((prInfo->u4SourceType == (UINT32)VSS_DTV) && (SRM_GetMmMode() != SRM_MM_MODE_TYPE_OFF))
        {
            prPool->u4Addr += FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER)->u4Size;
        }
#endif

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate over-range\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

        return prPool;
    }

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}
static FBM_POOL_T* _FbmGetPoolInfoAuto_MT5388_64MB_WXGA(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_T* prPool=NULL;

    static UINT8 fgInited = FALSE;
    FBM_POOL_T* prPrevPool;
    //UINT32 _fbm_u4NptvBeAddr, _fbm_u4NptvSubPipAddr, _fbm_u4NptvSubPopAddr;
    FBM_AUTO_INC_ENV_T env;

    if (!fgInited) // calculate max size of main, sub path in pip/pop
    {
        fgInited = TRUE;

        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
        _fbm_u4NptvBeAddr = prPool->u4Addr;

        _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
        _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;
        _eCurrentMddiPip = FBM_POOL_TYPE_MDDI_DYN_SUB;
        _eCurrentScposPip = FBM_POOL_TYPE_SCPOS_DYN_SUB;
        _eCurrentTdc = FBM_POOL_TYPE_TDC_DYN;

        prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
        _fbm_u4NptvSubPopAddr = (_fbm_u4NptvBeAddr + (prPrevPool->u4Addr))/2;

        //if (env.u1fgSingle || env.u1fgPIP)
        {
            UINT8 u1Mode, u1Type;

            //if (env.u1IsMPEG[VDP_1] == 0)
            {
                // if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(1440, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 1440;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler: for EPG mode
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

                //if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr,FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_10BIT_422_SIZE(780, 578, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 780;
                    prPool->u4Height = 578;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // tdc in main
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPrevPool = prPool;

                    u1Type = FBM_TDC;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }
                }

                // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(1440, 1080, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 1440;
                    prPool->u4Height = 1080;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 780);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1440;
                    prPool->u4Height = 780;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

                    _fbm_u4NptvSubPipAddr = prPool->u4Addr+prPool->u4Size+0x10000;
                }

                //if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 780);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1440;
                    prPool->u4Height = 780;

                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPipMaxMode = u1Mode;
                    }

                    {
                        // dummy allocation
                        // main di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 576;
                        prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }

                // MM 1080P cases
                {
                    prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 780);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 1440;
                    prPool->u4Height = 780;

                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // error checking
                    if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= (prPool->u4Addr)+(prPool->u4Size))
                    {
                        Printf("mm 1080p mode overwrite DSP\n");
                        Printf("DSP begin: 0x%x\n", FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr);
                        Printf("the end of the last one: 0x%x\n", (prPool->u4Addr)+(prPool->u4Size));
                        VERIFY(0);
                    }

                    // consider NR in 576P
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(750, 580, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Addr = FBM_ALIGN_MASK(((prPrevPool->u4Addr+prPrevPool->u4Size)-prPool->u4Size),FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Width = 750;
                    prPool->u4Height = 580;
                    prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

#if 0
                    {
                        // dummy allocation: scaler will get and assert the prPool in vDrvScpipSrmDispCb
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1440, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 1440;
                        prPool->u4Height = 1082;

                        u1Mode = FBM_TYPE_MM_1080P_MODE;
                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
#endif
                }

                // MM 1080I cases
                // 8bit, 420
                {
                    // main di
                    prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MPEG);
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr+prPrevPool->u4Size), FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_8BIT_420_SIZE(1440, 1082, 5, 5), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 1440;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MM_1080I_MODE;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // error checking
                    if (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr <= (prPool->u4Addr)+(prPool->u4Size))
                    {
                        Printf("mm 1080i mode overwrite DSP\n");
                        Printf("DSP begin: 0x%x\n", FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr);
                        Printf("the end of the last one: 0x%x\n", (prPool->u4Addr)+(prPool->u4Size));
                        VERIFY(0);
                    }

                    // consider scaler in 576i
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 780, 578);
                    prPool->u4Addr = FBM_ALIGN_MASK(((prPrevPool->u4Addr+prPrevPool->u4Size)-prPool->u4Size),FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Width = 750;
                    prPool->u4Height = 580;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

#if 0
                    {
                        // dummy allocation: scaler will get and assert the prPool in vDrvScpipSrmDispCb
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 780);
                        prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                        prPool->u4Width = 1440;
                        prPool->u4Height = 780;

                        u1Mode = FBM_TYPE_MM_1080I_MODE;
                        u1Type = FBM_MAIN_SCALER;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
#endif

                    // FBM_TYPE_MM_576I_MODE is the same as FBM_TYPE_MM_1080I_MODE
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_PSCAN].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_PSCAN].u4Mode;

                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_576I_MODE][FBM_MAIN_SCALER].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_MM_1080I_MODE][FBM_MAIN_SCALER].u4Mode;
                }
            }

            // error checking
            prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
            if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
            {
                LOG(1, "main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                VERIFY(0);
            }

            if (_fbm_u4NptvSubPipAddr <= _FBM_NPTV_VideoRange.u4MainPipMaxEnd)
            {
                LOG(1, "main overwrite sub pip, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPipMaxMode);
                VERIFY(0);
            }

            // if (env.u1fgPIP && ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) || (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)))
            {
                // sub path modules
                //if (env.u1Interlace[VDP_2] == 1) // 1080i
                {
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), FBM_MDDI_SIZE_ALIGMENT); // c bob
                    prPool->u4Width = 720;
                    prPool->u4Height = 1080;
                    prPool->u4Mode = 0;

                    // 1080i
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // sub scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;

                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }
                }
                //if (env.u1Interlace[VDP_2] == 1) // 576i
                {
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT); // yc 3d
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPool->u4Mode = 0;

                    // 576i
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // sub scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_SCPOS_SD_444_3FB_POOL_SIZE, FBM_SCPOS_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;

                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // tdc in sub
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_TDC_DYN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_TDC_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_TDC_POOL_SIZE, FBM_TDC_SIZE_ALIGMENT);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT;
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;

                    u1Type = FBM_TDC;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }
                }
                //else
                {
                    {
                        // dummy allocation
                        // sub di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 1080, 4, 3), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 1080;
                        prPool->u4Mode = 0;

                        u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                        u1Type = FBM_SUB_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }

                    // sub scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPipAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 760, 580);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 760;
                    prPool->u4Height = 580;

                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                    u1Type = FBM_SUB_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPipMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4SubPipMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4SubPipMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPipAddr;
                        _FBM_NPTV_VideoRange.u1SubPipMaxMode = u1Mode;
                    }

                    // 1080P422 is the same as 1080P444
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_SCALER].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_SCALER].u4Mode;

                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Addr   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Addr;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Size   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Size;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Width  = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Width;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Height = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Height;
                    _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_422][FBM_SUB_PSCAN].u4Mode   = _FBM_NPTV_VIDEO_CONF[FBM_TYPE_SUB_PIP_MODE_1080P_444][FBM_SUB_PSCAN].u4Mode;
                }

                // error checking
                prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
                if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4SubPipMaxEnd)
                {
                    LOG(1, "sub overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1SubPipMaxMode);
                    VERIFY(0);
                }
            }
        }
        // else // POP: take PAP mode into consideration
        {
            UINT8 u1Mode, u1Type;
            // _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_DYN_MAIN;
            // _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_DYN_MAIN;

            {
                // if (env.u1mode[VDP_1] == FBM_MODE_1080I) // not support pscan-disp mode
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 860, 1082);
                    prPool->u4Width = 860;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 860, 650);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 860;
                    prPool->u4Height = 650;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }
                }
                // else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr,FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 750, 590);
                    prPool->u4Width = 750;
                    prPool->u4Height = 590;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_576I;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 750, 590);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 750;
                    prPool->u4Height = 590;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }
                }
                // else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    // main di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                    prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(860, 1082, 2), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 860;
                    prPool->u4Height = 1082;
                    prPool->u4Mode = 0;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_422;
                    u1Type = FBM_MAIN_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    // main scaler
                    prPrevPool = prPool;
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 860, 650);
                    prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 860;
                    prPool->u4Height = 650;

                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }
                }
                // else if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    // main scaler
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_SCPOS_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 860, 650);
                    prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                    prPool->u4Width = 860;
                    prPool->u4Height = 650;

                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_444;
                    u1Type = FBM_MAIN_SCALER;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                    if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4MainPopMaxEnd))
                    {
                        _FBM_NPTV_VideoRange.u4MainPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                        _FBM_NPTV_VideoRange.u4MainPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvBeAddr;
                        _FBM_NPTV_VideoRange.u1MainPopMaxMode = u1Mode;
                    }

                    {
                        // dummy allocation
                        // main di
                        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
                        prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvBeAddr, FBM_MDDI_ADDR_ALIGMENT);
                        prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                        prPool->u4Width = 720;
                        prPool->u4Height = 576;
                        prPool->u4Mode = FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR;

                        u1Type = FBM_MAIN_PSCAN;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                        _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                    }
                }
            }

            // error checking
            prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
            if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4MainPopMaxEnd)
            {
                LOG(1, "main overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPopMaxMode);
                VERIFY(0);
            }
            if (_fbm_u4NptvSubPopAddr <= _FBM_NPTV_VideoRange.u4MainPopMaxEnd)
            {
                LOG(1, "main overwrite sub pop, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1MainPopMaxMode);
                VERIFY(0);
            }

            // sub path modules
            _fbm_u4NptvSubPipAddr = _fbm_u4NptvSubPopAddr;

            // if (env.u1mode[VDP_2] == FBM_MODE_1080I) // not support pscan-disp mode
            {
                // sub di
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_MDDI_ADDR_ALIGMENT);
                prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1440/2, 1082);
                prPool->u4Width = 1440/2;
                prPool->u4Height = 1082;
                prPool->u4Mode = 0;

                u1Mode = FBM_TYPE_SUB_POP_MODE_1080I;
                u1Type = FBM_SUB_PSCAN;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                // sub scaler
                prPrevPool = prPool;
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440/2, 576);
                prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 1440/2;
                prPool->u4Height = 576;

                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }
            }
            // else if (env.u1mode[VDP_2] == FBM_MODE_576I)
            {
                // sub di
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr,FBM_MDDI_ADDR_ALIGMENT);
                prPool->u4Size = FBM_MDDI_BI_10BIT_POOL_SIZE(FBM_MDDI_SIZE_ALIGMENT, 1440/2, 576);
                prPool->u4Width = 1440/2;
                prPool->u4Height = 576;
                prPool->u4Mode = 0;

                u1Mode = FBM_TYPE_SUB_POP_MODE_576I;
                u1Type = FBM_SUB_PSCAN;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                // sub scaler
                prPrevPool = prPool;
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440/2, 576);
                prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 1440/2;
                prPool->u4Height = 576;

                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }
            }
            // else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
            {
                // sub di
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr =  FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_MDDI_ADDR_ALIGMENT);
                prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_BI_NR_10BIT_SIZE(1440/2, 1080, 2), FBM_MDDI_SIZE_ALIGMENT);
                prPool->u4Width = 1440/2;
                prPool->u4Height = 1080;
                prPool->u4Mode = 0;

                u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_422;
                u1Type = FBM_SUB_PSCAN;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                // sub scaler
                prPrevPool = prPool;
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK((prPrevPool->u4Addr + prPrevPool->u4Size), FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_10BIT_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440/2, 576);
                prPool->u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 1440/2;
                prPool->u4Height = 576;

                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }
            }
            // else if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
            {
                // sub scaler
                prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_SCPOS_ADDR_ALIGMENT);
                prPool->u4Size = FBM_SCPOS_444_3FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440/2, 576);
                prPool->u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_SCPOS_3FB;
                prPool->u4Width = 1440/2;
                prPool->u4Height = 576;

                u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_444;
                u1Type = FBM_SUB_SCALER;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;

                if ((prPool->u4Addr+prPool->u4Size) > (_FBM_NPTV_VideoRange.u4SubPopMaxEnd))
                {
                    _FBM_NPTV_VideoRange.u4SubPopMaxEnd = prPool->u4Addr+prPool->u4Size;
                    _FBM_NPTV_VideoRange.u4SubPopMaxSize = prPool->u4Addr+prPool->u4Size - _fbm_u4NptvSubPopAddr;
                    _FBM_NPTV_VideoRange.u1SubPopMaxMode = u1Mode;
                }

                {
                    // dummy allocation
                    // sub di
                    prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
                    prPool->u4Addr = FBM_ALIGN_MASK(_fbm_u4NptvSubPopAddr, FBM_MDDI_ADDR_ALIGMENT);
                    prPool->u4Size = FBM_ALIGN_MASK(FBM_MDDI_SUB_BI_8BIT_422_SIZE(720, 576, 4, 4), FBM_MDDI_SIZE_ALIGMENT);
                    prPool->u4Width = 720;
                    prPool->u4Height = 576;
                    prPool->u4Mode = 0;

                    u1Type = FBM_SUB_PSCAN;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr = prPool->u4Addr;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size = prPool->u4Size;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width = prPool->u4Width;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height = prPool->u4Height;
                    _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode = prPool->u4Mode;
                }
            }

            // error checking
            prPrevPool = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
            if (prPrevPool->u4Addr <= _FBM_NPTV_VideoRange.u4SubPopMaxEnd)
            {
                LOG(1, "sub overwrite DSP, mode: 0x%x\n", _FBM_NPTV_VideoRange.u1SubPopMaxMode);
                VERIFY(0);
            }
        }
    }

    _FbmGetEnv(&env);
    LOG(6, "u1mode(%d %d) u1IsMPEG(%d, %d)\n", env.u1mode[VDP_1], env.u1mode[VDP_2],env.u1IsMPEG[VDP_1], env.u1IsMPEG[VDP_2]);

    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_TDC)
    {
        UINT8 u1Mode, u1Type;

        u1Type = FBM_TDC;
        if (env.u1IsTDC[VDP_1] == 1)
        {
            u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
        }
        // for tdc in sub pop
        else // if (env.u1IsTDC[VDP_2] == 1)
        {
            u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
        }
        prPool = FBM_GetPoolInfo(_eCurrentTdc);
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate overange\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

        return prPool;
    }

    if (((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
            (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)))
    {
        UINT8 u1Mode, u1Type;
        if (prInfo->u4SourceType == (UINT32)VSS_DTV)
        {
            // assumption: single
            if (env.u1mode[VDP_1] == FBM_MODE_1080I)
            {
                u1Mode = FBM_TYPE_MM_1080I_MODE;
            }
            else if (env.u1mode[VDP_1] == FBM_MODE_576I)
            {
                u1Mode = FBM_TYPE_MM_576I_MODE;
            }
            else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_422) // (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
            {
                if (prInfo->u4Resolution <= SRM_VDP_SD_RESOLUTION)
                {
                    u1Mode = FBM_TYPE_MM_576I_MODE;
                }
                else
                {
                    u1Mode = FBM_TYPE_MM_1080P_MODE;
                }
            }
        }
        else if (env.u1fgSingle || env.u1fgPIP)
        {
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                    (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
            {
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_576I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_MAIN_PIP_MODE_1080P_444;
                }
            }
            else
            {
                if (env.u1mode[VDP_2] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_576I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_SUB_PIP_MODE_1080P_444;
                }
            }
        }
        else // POP
        {
            if ((ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN) ||
                    (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN))
            {
                // main
                if (env.u1mode[VDP_1] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_576I;
                }
                else if (env.u1mode[VDP_1] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_1] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_MAIN_POP_MODE_1080P_444;
                }
            }
            else
            {
                // sub
                if (env.u1mode[VDP_2] == FBM_MODE_1080I)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_1080I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_576I)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_576I;
                }
                else if (env.u1mode[VDP_2] == FBM_MODE_1080P_422)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_422;
                }
                else // if (env.u1mode[VDP_2] == FBM_MODE_1080P_444)
                {
                    u1Mode = FBM_TYPE_SUB_POP_MODE_1080P_444;
                }
            }
        }
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_MAIN);
            u1Type = FBM_MAIN_PSCAN;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_MAIN);
            u1Type = FBM_MAIN_SCALER;
        }
        else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_MDDI_DYN_SUB);
            u1Type = FBM_SUB_PSCAN;
        }
        else // if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_PIP)
        {
            prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_DYN_SUB);
            u1Type = FBM_SUB_SCALER;
        }
        prPool->u4Addr = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Addr;
        prPool->u4Size = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Size;
        prPool->u4Width = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Width;
        prPool->u4Height = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Height;
        prPool->u4Mode = _FBM_NPTV_VIDEO_CONF[u1Mode][u1Type].u4Mode;
        prPool->u4Inherit = FBM_POOL_INHERIT; // need to set FBM_POOL_INHERIT explicitly
        LOG(3, "Load Index: (u1Mode, u1Type): (%x, %x)\n", u1Mode, u1Type);

        // error checking
        if ((prPool->u4Addr < _fbm_u4NptvBeAddr) ||
                ((prPool->u4Addr + prPool->u4Size) > (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr)))
        {
            Printf("FBM nptv allocate overange\n");
            Printf("(u1Mode, u1Type): (0x%x, 0x%x)\n", u1Mode, u1Type);
            Printf("occupied range: 0x%x--0x%x\n", prPool->u4Addr, (prPool->u4Addr + prPool->u4Size));
            Printf("valid range: 0x%x--0x%x\n", _fbm_u4NptvBeAddr, (FBM_GetPoolInfo(FBM_POOL_TYPE_DSP)->u4Addr));
            VERIFY(0);
        }

        return prPool;
    }

		if(ucPoolType >= FBM_POOL_TYPE_NS)
    {
    	Printf("input type error (%d)\n",ucPoolType);
    	return (FBM_GetPoolInfo(FBM_POOL_TYPE_TOTAL));
    }
    prPool = FBM_GetPoolInfo(ucPoolType);

    return prPool;
}

//-------------------------------------------------------------------------
/** FBM_GetPoolInfoAuto
 *  Get Pool Info
 *  @param  ucPoolType
 *  @param  prInfo
 *  @return NONE.
 */
//-------------------------------------------------------------------------
FBM_POOL_T* FBM_GetPoolInfoAuto(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* prInfo)
{
    FBM_POOL_MODULE_INFO_T rInfo;
    if (prInfo == NULL)
    {
        SRM_VDP_INFO_T* prVdpInfo;
        prVdpInfo = SRM_GetVdpInfo(VDP_1);
        x_memset((VOID*)&rInfo, 0, sizeof(FBM_POOL_MODULE_INFO_T));
        rInfo.u4SourceType = prVdpInfo->u4Source;
        rInfo.u4Resolution = prVdpInfo->u4Resolution;
        rInfo.u4Interlace = prVdpInfo->u4Interlace;
        rInfo.u4VgaTiming = prVdpInfo->u4VgaTiming;
        rInfo.u4ColorFormat444 = prVdpInfo->u4ColorFormat444;

        if (SRM_IsPopVideo())
        {
            rInfo.u4PipPop = SRM_TV_MODE_TYPE_POP;
        }
        else if (SRM_IsPipVideo())
        {
            rInfo.u4PipPop = SRM_TV_MODE_TYPE_PIP;
        }
        prInfo = &rInfo;
    }
    else
    {
        x_memcpy((VOID*)&rInfo, (VOID*)prInfo, sizeof(FBM_POOL_MODULE_INFO_T));
        rInfo.u4Resolution = prInfo->u4Width * prInfo->u4Height;
        prInfo = &rInfo;
    }

    LOG(3, "FBM Get Pool (%s) s(%d) w(%d) h(%d), p(%d), i(%d), r(%d)\n",
        FBM_GetPoolName(ucPoolType),
        prInfo->u4SourceType,
        prInfo->u4Width,
        prInfo->u4Height,
        prInfo->u4PipPop,
        prInfo->u4Interlace,
        prInfo->u4Resolution);

    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_VERF) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
#if defined(CC_MT5365)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB)
#endif
       )
    {
#if defined(CC_MT5365)
        // FBM_MEM_CFG_MT5365_128Mx1, FBM_MEM_CFG_MT5365_128Mx1_NOSUB
        // FBM_MEM_CFG_MT5363_64MBx2
        return _FbmGetPoolInfoAuto_MT5365_128MB(ucPoolType, prInfo);
#elif defined(CC_MT5395) || defined(CC_MT5396)
        // FBM_MEM_CFG_MT5363_64MBx2, For IC verification
        return _FbmGetPoolInfoAuto_MT5388_128MB(ucPoolType, prInfo);
#else
        // FBM_MEM_CFG_MT5387_VERF
        // FBM_MEM_CFG_MT5388_128MB
        // FBM_MEM_CFG_MT5363_64MBx2
        return _FbmGetPoolInfoAuto_MT5388_128MB(ucPoolType, prInfo);
#endif
    }
#if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
            )
    {
        return _FbmGetPoolInfoAuto_MT5365_128MB(ucPoolType, prInfo);
    }
#if defined(CC_MT5396)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
             ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC))
    {
        return _FbmGetPoolInfoAuto_MT5396_1GB(ucPoolType, prInfo);
    }
#endif
#if defined(CC_MT5368)||defined(CC_MT5389)
    else if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
              || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
              || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
              || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK)
            )
    {
        return _FbmGetPoolInfoAuto_MT5368_256MB(ucPoolType, prInfo);
    }
    else if ( (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
              || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
              || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264))
    {
        return _FbmGetPoolInfoAuto_MT5365_128MB(ucPoolType, prInfo);
    }
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
    {
        return _FbmGetPoolInfoAuto_MT5501_128MB(ucPoolType, prInfo);
    }
#endif
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
    {
        return _FbmGetPoolInfoAuto_MT5365_128MB(ucPoolType, prInfo);
    }
#endif
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_64MB)
    {
        return _FbmGetPoolInfoAuto_MT5387_64MB_FHD_Single(ucPoolType, prInfo);
    }
#if defined(CC_MT5365)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1_NOH264))
    {
        return _FbmGetPoolInfoAuto_MT5365_128MB(ucPoolType, prInfo);
    }
#endif
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MB)
    {
        return _FbmGetPoolInfoAuto_MT5363_64MB_FHD_Single(ucPoolType, prInfo);
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_64MB_ATV_WXGA)
    {
        return _FbmGetPoolInfoAuto_MT5388_64MB_WXGA(ucPoolType, prInfo);
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_ATV_64MB)
    {
        return _FbmGetPoolInfoAuto_MT5365_ATV_64MB(ucPoolType, prInfo);
    }
    else
    {
        VERIFY(0);
        _FbmGetPoolInfoAuto_MT5388_128MB(ucPoolType, prInfo);
    }
    return FBM_GetPoolInfo(ucPoolType);
}


//-------------------------------------------------------------------------
/** FBM_GetPoolCurrent
 *  Get Pool Current Use
 *  @param ucPoolType
 *  @return ucPoolType.
 */
//-------------------------------------------------------------------------

UCHAR FBM_GetPoolCurrent(UCHAR ucPoolType)
{
    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
    {
        return (UCHAR) _eCurrentScposMain;
    }
    else if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MJC_1)
    {
        return (UCHAR) _eCurrentMjc;
    }

    return ucPoolType;
}


//-------------------------------------------------------------------------
/** FBM_PrintPoolInfo
 *  Print FBM pool status
 *  @return NONE
 */
//-------------------------------------------------------------------------
VOID FBM_PrintPoolInfo(FBM_POOL_T* prPool, POOL_LIST_T* prPoolList)
{
    UCHAR ucIdx, ucLoop;

    for (ucLoop = 0; ucLoop < 3; ucLoop++)
    {
        for (ucIdx = 0; ucIdx < (UINT32)FBM_POOL_TYPE_NS; ucIdx++)
        {
            if ((((ucLoop == 0) && (prPool[ucIdx].u4Size > 0))&&(prPool[ucIdx].u4Inherit!= FBM_POOL_ROOT)) ||
                    (((ucLoop == 1) && (prPool[ucIdx].u4Size > 0))&&(prPool[ucIdx].u4Inherit == FBM_POOL_ROOT))||
                    ((ucLoop == 2) && (prPool[ucIdx].u4Size == 0)))
            {
#ifdef FBM_PRINT_STATUS_TOTAL
                LOG(1, "Addr(0x%08x) Size(0x%08x) Total (0x%08x) %s\n",
                    prPool[ucIdx].u4Addr,
                    prPool[ucIdx].u4Size,
                    (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                    prPoolList[ucIdx].szName);
#else
                if (prPool[ucIdx].u4Size > 0)
                {
                    LOG(1, "Addr(0x%08x) Size(0x%08x) End(0x%08x) Mode(%c, 0x%08x, %04d, %04d) %s\n",
                        prPool[ucIdx].u4Addr,
                        prPool[ucIdx].u4Size,
                        (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                        (prPool[ucIdx].u4Inherit == FBM_POOL_ROOT)?'R':'I',
                        prPool[ucIdx].u4Mode,
                        prPool[ucIdx].u4Width,
                        prPool[ucIdx].u4Height,
                        prPoolList[ucIdx].szName);
                }
                else
                {
                    LOG(8, "Addr(0x%08x) Size(0x%08x) End(0x%08x) Mode(%c, 0x%08x, %04d, %04d) %s\n",
                        prPool[ucIdx].u4Addr,
                        prPool[ucIdx].u4Size,
                        (prPool[ucIdx].u4Addr + prPool[ucIdx].u4Size),
                        (prPool[ucIdx].u4Inherit == FBM_POOL_ROOT)?'R':'I',
                        prPool[ucIdx].u4Mode,
                        prPool[ucIdx].u4Width,
                        prPool[ucIdx].u4Height,
                        prPoolList[ucIdx].szName);
                }
#endif
            }
        }
    }

    LOG(1, "Current pool (SCPOS-M=%s, SCPOS-P=%s, MDDI-M=%s, MDDI-P=%s, TDC=%s)\n",
        FBM_GetPoolName((UCHAR)_eCurrentScposMain),
        FBM_GetPoolName((UCHAR)_eCurrentScposPip),
        FBM_GetPoolName((UCHAR)_eCurrentMddiMain),
        FBM_GetPoolName((UCHAR)_eCurrentMddiPip),
        FBM_GetPoolName((UCHAR)_eCurrentTdc));

    LOG(1, "Current pool (MJC=%s, VBI=%s)\n",
        FBM_GetPoolName((UCHAR)_eCurrentMjc),
        FBM_GetPoolName((UCHAR)_eCurrentVbi));

    // cyj.porting.2009.03.12
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_VERF)
    {
        LOG(1, "FBM_MEM_CFG_MT5387_VERF Model\n");
    }
    if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_128MB)
    {
        LOG(1, "FBM_MEM_CFG_MT5388_128MB Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_64MB)
    {
        LOG(1, "FBM_MEM_CFG_MT5387_64MB Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MB)
    {
        LOG(1, "FBM_MEM_CFG_MT5363_64MB Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_64MB_ATV_WXGA)
    {
        LOG(1, "FBM_MEM_CFG_MT5388_64MB_ATV_WXGA Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
    {
        LOG(1, "FBM_MEM_CFG_MT5363_64MBx2 Model\n");
    }
#if defined(CC_MT5395) || defined(CC_MT5396)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_DDR3x2 Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_DDR2x2 Model\n");
    }
#if defined(CC_MT5396)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
    {
        LOG(1, "FBM_MEM_CFG_MT5396_DDR3x4 Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
    {
        LOG(1, "FBM_MEM_CFG_MT5396_3DTV Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
    {
        LOG(1, "FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC Model\n");
    }
#endif
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_3DTV Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_NOMJC Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_DDR3x2_EU Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_DDR3x2_EU_NET Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
    {
        LOG(1, "FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ Model\n");
    }
#endif
#if defined(CC_MT5368)||defined(CC_MT5389)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
    {
        LOG(1, "FBM_MEM_CFG_MT5368_DDR3x1 Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
    {
        LOG(1, "FBM_MEM_CFG_MT5368_DDR3x2 Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
    {
        LOG(1, "FBM_MEM_CFG_MT5368_3DTV Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
    {
        LOG(1, "FBM_MEM_CFG_MT5368_128Mx1 Model\n");
    }
     else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK)
    {
        LOG(1, "FBM_MEM_CFG_MT5368_128Mx1_SHRINK Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
    {
        LOG(1, "FBM_MEM_CFG_MT5368_64Mx1 Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264)
    {
        LOG(1, "FBM_MEM_CFG_MT5368_64Mx1_NOH264 Model\n");
    }
	 else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
    {
        LOG(1, "FBM_MEM_CFG_MT5501_3DNET Model\n");
    }
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
    {
        LOG(1, "FBM_MEM_CFG_MT6595_COMP Model\n");
    }
#if defined(CC_MT5365)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
    {
        LOG(1, "FBM_MEM_CFG_MT5365_3DTV Model\n");
    }
#endif
#endif
#if defined(CC_MT5365)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)
    {
        LOG(1, "FBM_MEM_CFG_MT5365_128Mx1 Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB)
    {
        LOG(1, "FBM_MEM_CFG_MT5365_128Mx1_NOSUB Model\n");
    }
#endif
#if defined(CC_MT5365)
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
    {
        LOG(1, "FBM_MEM_CFG_MT5365_64Mx1 Model\n");
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1_NOH264)
    {
        LOG(1, "FBM_MEM_CFG_MT5365_64Mx1_NOH264 Model\n");
    }
#endif
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_ATV_64MB)
    {
        LOG(1, "FBM_MEM_CFG_MT5365_ATV_64MB Model\n");
    }
    LOG(9, "0x%08x = FBM_POOL_MODE_8BIT\n", FBM_POOL_MODE_8BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_9BIT\n", FBM_POOL_MODE_9BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_10BIT\n", FBM_POOL_MODE_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_C_10BIT\n", FBM_POOL_MODE_C_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_420_FB\n", FBM_POOL_MODE_420_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_422_FB\n", FBM_POOL_MODE_422_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_444_FB\n", FBM_POOL_MODE_444_FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SD\n", FBM_POOL_MODE_SD);
    LOG(9, "0x%08x = FBM_POOL_MODE_720HD\n", FBM_POOL_MODE_720HD);
    LOG(9, "0x%08x = FBM_POOL_MODE_1080HD\n", FBM_POOL_MODE_1080HD);
    LOG(9, "0x%08x = FBM_POOL_MODE_SHARE\n", FBM_POOL_MODE_SHARE);
    LOG(9, "0x%08x = FBM_POOL_MODE_TOGGLE_TRIGGER\n", FBM_POOL_MODE_TOGGLE_TRIGGER);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_NO_COMP\n", FBM_POOL_MODE_MPEG_NO_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_COMP_75\n", FBM_POOL_MODE_MPEG_COMP_75);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_COMP_50\n", FBM_POOL_MODE_MPEG_COMP_50);
    LOG(9, "0x%08x = FBM_POOL_MODE_MPEG_DBK\n", FBM_POOL_MODE_MPEG_DBK);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_DROP\n", FBM_POOL_MODE_MJC_DROP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_NO_DROP\n", FBM_POOL_MODE_MJC_NO_DROP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_NO_COMP\n", FBM_POOL_MODE_MJC_NO_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_Y_COMP4\n", FBM_POOL_MODE_MJC_Y_COMP4);
    LOG(9, "0x%08x = FBM_POOL_MODE_MJC_Y_COMP3\n", FBM_POOL_MODE_MJC_Y_COMP3);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_3D_Y_C\n", FBM_POOL_MODE_NR_3D_Y_C);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_3D_Y_ONLY\n", FBM_POOL_MODE_NR_3D_Y_ONLY);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_2D\n", FBM_POOL_MODE_NR_2D);
    LOG(9, "0x%08x = FBM_POOL_MODE_NR_BYPASS\n", FBM_POOL_MODE_NR_BYPASS);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_1FB\n", FBM_POOL_MODE_SCPOS_1FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_2FB\n", FBM_POOL_MODE_SCPOS_2FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_3FB\n", FBM_POOL_MODE_SCPOS_3FB);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_DISP\n", FBM_POOL_MODE_SCPOS_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_FULL\n", FBM_POOL_MODE_SCPOS_FULL);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_PSCAN_DISP\n", FBM_POOL_MODE_SCPOS_PSCAN_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_MJC_DISP\n", FBM_POOL_MODE_SCPOS_MJC_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_SCPOS_LINESYNC_DISP\n", FBM_POOL_MODE_SCPOS_LINESYNC_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_BOB\n", FBM_POOL_MODE_MDDI_BOB);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_FULL\n", FBM_POOL_MODE_MDDI_FULL);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_CBOB\n", FBM_POOL_MODE_MDDI_CBOB);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_Y_C\n", FBM_POOL_MODE_MDDI_NR_Y_C);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_Y_ONLY\n", FBM_POOL_MODE_MDDI_NR_Y_ONLY);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_OFF\n", FBM_POOL_MODE_MDDI_NR_OFF);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_NR_10BIT\n", FBM_POOL_MODE_MDDI_NR_10BIT);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_COMP\n", FBM_POOL_MODE_MDDI_COMP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_DISP\n", FBM_POOL_MODE_MDDI_DISP);
    LOG(9, "0x%08x = FBM_POOL_MODE_MDDI_P2I\n", FBM_POOL_MODE_MDDI_P2I);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_NORMAL\n", FBM_TV_MODE_TYPE_NORMAL);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_PIP\n", FBM_TV_MODE_TYPE_PIP);
    LOG(9, "0x%08x = FBM_TV_MODE_TYPE_POP\n", FBM_TV_MODE_TYPE_POP);

    LOG(5, "NPTV Main Begin Addr:          0x%08x\n", _fbm_u4NptvBeAddr);
    LOG(5, "NPTV Sub PIP Begin Addr:       0x%08x\n", _fbm_u4NptvSubPipAddr);
    LOG(5, "NPTV Sub POP Begin Addr:       0x%08x\n", _fbm_u4NptvSubPopAddr);
    LOG(5, "Main pip: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4MainPipMaxEnd, _FBM_NPTV_VideoRange.u4MainPipMaxSize, _FBM_NPTV_VideoRange.u1MainPipMaxMode);
    LOG(5, "Sub  pip: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4SubPipMaxEnd, _FBM_NPTV_VideoRange.u4SubPipMaxSize, _FBM_NPTV_VideoRange.u1SubPipMaxMode);
    LOG(5, "Main pop: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4MainPopMaxEnd, _FBM_NPTV_VideoRange.u4MainPopMaxSize, _FBM_NPTV_VideoRange.u1MainPopMaxMode);
    LOG(5, "Sub  pop: (EndAddr,Size,Mode): 0x%08x, 0x%08x, 0x%08x\n", _FBM_NPTV_VideoRange.u4SubPopMaxEnd, _FBM_NPTV_VideoRange.u4SubPopMaxSize, _FBM_NPTV_VideoRange.u1SubPopMaxMode);
}

//-------------------------------------------------------------------------
/** FBM_ConfigurePool
 *  Initialize this FBM pool
 *  @retval NONE
 */
//-------------------------------------------------------------------------
VOID FBM_ConfigurePool(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo)
{
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_VERF) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
#if defined(CC_MT5365)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)
            || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB)
#endif
       )
    {
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)||defined(CC_MT5389)
        _FbmConfigurePool_MT5395_MJC_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
#else
        _FbmConfigurePool_MT5388_VERF_128MB(prPoolList, prPoolArrangeInfo);
#endif
    }
#if defined(CC_MT5395) || defined(CC_MT5396)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
            )
    {
        _FbmConfigurePool_MT5395_MJC_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
    }
#endif
#if defined(CC_MT5396)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
            )
    {
        _FbmConfigurePool_MT5396_MJC_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
    }
#endif
#if defined(CC_MT5368)||defined(CC_MT5389)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
             ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
             ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
             ||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK)
            )
    {
        _FbmConfigurePool_MT5368_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
    }
	else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
    {
        _FbmConfigurePool_MT5501_Buffer(prPoolList, prPoolArrangeInfo);
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1))
    {
        _FbmConfigurePool_MT5395_MJC_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1_NOH264))
    {
        _FbmConfigurePool_MT5368_64MBx1(prPoolList, prPoolArrangeInfo);
    }
#endif
#if defined(CC_MT5365) || defined(CC_MT5395)|| defined(CC_MT5368) || defined(CC_MT5396)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
#if defined(CC_MT5365)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
#endif
            )
    {
        _FbmConfigurePool_MT5395_MJC_SCPOS_MAIN(prPoolList, prPoolArrangeInfo);
    }
#endif
#if defined(CC_MT5365)
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
             || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1_NOH264))
    {
        _FbmConfigurePool_MT5365_64MBx1(prPoolList, prPoolArrangeInfo);
    }
#endif
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5387_64MB)
    {
        _FbmConfigurePool_MT5387_VERF_64MB(prPoolList, prPoolArrangeInfo);
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MB)
    {
        _FbmConfigurePool_MT5363_VERF_64MB(prPoolList, prPoolArrangeInfo);
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5388_64MB_ATV_WXGA)
    {
        _FbmConfigurePool_MT5388_64MB_WXGA(prPoolList, prPoolArrangeInfo);
    }
    else if (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_ATV_64MB)
    {
        _FbmConfigurePool_MT5365_ATV_64MB(prPoolList, prPoolArrangeInfo);
    }
    else
    {
        VERIFY(0);
        _FbmConfigurePool_MT5388_VERF_128MB(prPoolList, prPoolArrangeInfo);
    }
}

