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
 * $Author: cosh.hsu $
 * $Date: 2012/10/29 $
 * $RCSfile: fbm_pool.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_pool.c
 *  Brief of file fbm_pool.c.
 *  FBM Pool Management
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "fbm_pool.h"
#include "fbm_pool_config.h"
#include "fbm_debug.h"

#ifdef CC_SRM_ON
#include "srm_drvif.h"
#endif

LINT_EXT_HEADER_BEGIN
#include "source_table.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "drvcust_if.h"
#include "x_mid.h"
LINT_EXT_HEADER_END


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//#define FBM_MPEG_POOL_MID_EN

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// FBP Status
#define FBP_STATUS_UNKNOWN                          0xFF
#define FBP_STATUS_EMPTY                            0
#define FBP_STATUS_USED                             1
#define FBP_STATUS_OCCUPY                           2

// FBP Maximum Number
#define FBP_MAX_DISASSEMBLY                         4
#define FBP_MAX_NS                                  (FBG_MAX_NS * 3)

// Maximun residual pool number
#define FBM_RESIDUAL_POOL_NS                        4


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// Frame Buffer Pool
typedef struct
{
    UCHAR ucType;
    UCHAR ucStatus;
    UCHAR ucInheritance;

    UCHAR aucOccupyList[FBP_MAX_DISASSEMBLY];

    UINT32 u4Size;
    UINT32 u4Addr;
} FBP_T;

// Frame Buffer Pool Configuration
typedef struct
{
    UCHAR ucType;
    UINT32 u4Addr;
} FBP_CONF_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define FBM_MEMSET(addr, value, len)                    \
    (VERIFY(x_memset((addr), value, len) == (addr)))

#define FBM_POOL_MUTEX_LOCK()                              \
    (VERIFY(x_sema_lock(_hFbmPoolMutex, X_SEMA_OPTION_WAIT) == OSR_OK))

#define FBM_POOL_MUTEX_UNLOCK()                            \
    (VERIFY(x_sema_unlock(_hFbmPoolMutex) == OSR_OK))

#define FBM_CHECK_POOL_INDEX(idx)                                              \
    do                                                                         \
    {                                                                          \
        if ((idx) >= FBM_POOL_TYPE_NS)                                         \
        {                                                                      \
            ASSERT(0);                                                         \
            return NULL;                                                       \
        }                                                                      \
    }                                                                          \
    while (0)

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

#ifdef CC_SRM_ON
static VOID _FBM_SrmCtrlCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
#endif
enum
{
   FBM_MULTI_TV,
   FBM_MULIT_MM,
   FBM_MULIT_NETWORK,
};
//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static POOL_ARRANGE_INFO_T arPoolArrangeInfo[FBM_POOL_TYPE_NS];


// Frame Buffer Memory Pool Type List
static POOL_LIST_T _arPoolList[] =
{
    //DMX
    {
        "DMX",
        FBM_POOL_TYPE_DMX,
        FBM_DMX_ADDR_ALIGMENT, FBM_DMX_SIZE_ALIGMENT,
        0,
        FBM_DMX_POOL_SIZE,
        0, 0,
        FBM_POOL_INHERIT
    },

    //TDC
    {
        "TDC",
        FBM_POOL_TYPE_TDC,
        FBM_TDC_ADDR_ALIGMENT, 0,
        0,
        FBM_TDC_POOL_SIZE,
        720, 576,
        FBM_POOL_INHERIT
    },

    //TDC2
    {
        "TDC_2",
        FBM_POOL_TYPE_TDC_2,
        FBM_TDC_ADDR_ALIGMENT, 0,
        0,
        0,
        720, 576,
        FBM_POOL_INHERIT
    },

    //VBI
    {
        "VBI",
        FBM_POOL_TYPE_VBI,
        FBM_VBI_ADDR_ALIGMENT, 0,
        0,
        FBM_VBI_POOL_SIZE,
        0, 0,
        FBM_POOL_INHERIT
    },

    //VBI_2
    {
        "VBI_2",
        FBM_POOL_TYPE_VBI_2,
        FBM_VBI_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //MPEG
    {
        "MPEG",
        FBM_POOL_TYPE_MPEG,
        FBM_MPEG_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //MJC_1
    {
        "MJC_1",
        FBM_POOL_TYPE_MJC_1,
        FBM_MJC_ADDR_ALIGMENT, FBM_MJC_SIZE_ALIGMENT,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //MPEG_TV
    {
        "MPEG_TV",
        FBM_POOL_TYPE_MPEG_TV,
        FBM_MPEG_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //MPEG_NET 
    {
        "MPEG_NET",
        FBM_POOL_TYPE_MPEG_NET,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //SCPOS (Main)
    {
        "SCPOS_MAIN",
        FBM_POOL_TYPE_SCPOS_MAIN,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        1440, 576,
        FBM_POOL_INHERIT
    },

    //DMX_TV
    {
        "DMX_TV",
        FBM_POOL_TYPE_DMX_TV,
        FBM_DMX_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //DMX_NET
    {
        "DMX_NET",
        FBM_POOL_TYPE_DMX_NET,
        FBM_DMX_ADDR_ALIGMENT, 0,
        0,
        0,
        720, 576,
        FBM_POOL_INHERIT
    },

    //FEEDER_NET
    {
        "FEEDER_NET",
        FBM_POOL_TYPE_FEEDER_NET,
        FBM_DMX_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //SCPOS (Pip)
    {
        "SCPOS_PIP",
        FBM_POOL_TYPE_SCPOS_PIP,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        960, 540,
        FBM_POOL_INHERIT
    },

    //Feeder
    {
        "Feeder",
        FBM_POOL_TYPE_FEEDER,
        FBM_FEEDER_ADDR_ALIGMENT, FBM_FEEDER_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //Feeder MMP (for turnkey MMP)
    {
        "Feeder_MMP",
        FBM_POOL_TYPE_FEEDER_MMP,
        FBM_FEEDER_ADDR_ALIGMENT, FBM_FEEDER_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //MDDI_1
    {
        "MDDI_1",
        FBM_POOL_TYPE_MDDI_1,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //MDDI_2
    {
        "PQ_MM",
        FBM_POOL_TYPE_PQ_MM,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //PQ_NET
    {
        "PQ_NET",
        FBM_POOL_TYPE_PQ_NET,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //PQ_TV
    {
	   "PQ_TV",
	   FBM_POOL_TYPE_PQ_TV,
	   FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //NET SWAP
    {
        "NET_SWAP",
        FBM_POOL_TYPE_NET_SWAP,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //TDC_DYN
    {
        "TDC_DYN",
        FBM_POOL_TYPE_TDC_DYN,
        FBM_TDC_ADDR_ALIGMENT, 0,
        0,
        FBM_TDC_POOL_SIZE,
        720, 576,
        FBM_POOL_INHERIT
    },

    //SCPOS_DYN_MAIN
    {
        "SCPOS_DYN_MAIN",
        FBM_POOL_TYPE_SCPOS_DYN_MAIN,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //MDDI_DYN_MAIN
    {
        "MDDI_DYN_MAIN",
        FBM_POOL_TYPE_MDDI_DYN_MAIN,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //SCPOS_DYN_SUB
    {
        "SCPOS_DYN_SUB",
        FBM_POOL_TYPE_SCPOS_DYN_SUB,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //MDDI_DYN_SUB
    {
        "MDDI_DYN_SUB",
        FBM_POOL_TYPE_MDDI_DYN_SUB,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //MDDI (Pip)
    {
        "MDDI_P",
        FBM_POOL_TYPE_MDDI_PIP,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        960, 1080,
        FBM_POOL_INHERIT
    },

    //NR_1
    {
        "NR_1",
        FBM_POOL_TYPE_NR_1,
        FBM_NR_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //DSP
    {
        "DSP",
        FBM_POOL_TYPE_DSP,
        FBM_DSP_ADDR_ALIGMENT, 0,
        0,
        FBM_DSP_POOL_SIZE,
        0, 0,
        FBM_POOL_INHERIT
    },

    //B2R
    {
        "B2R",
        FBM_POOL_TYPE_B2R,
        FBM_B2R_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1088,
        FBM_POOL_INHERIT
    },

    //JPEG
    {
        "JPEG",
        FBM_POOL_TYPE_JPEG,
        FBM_JPEG_ADDR_ALIGMENT, 0,
        0,
        FBM_JPEG_POOL_SIZE,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //OSD
    {
        "JPEG-OSD1",
        FBM_POOL_TYPE_JPEG_OSD1,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //OSD
    {
        "JPEG-OSD2",
        FBM_POOL_TYPE_JPEG_OSD2,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //OSD
    {
        "JPEG-OSD3",
        FBM_POOL_TYPE_JPEG_OSD3,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //OSD
    {
        "JPEG-OSD4",
        FBM_POOL_TYPE_JPEG_OSD4,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //PQ_TOOL
    {
        "PQ-TOOL",
        FBM_POOL_TYPE_PQ_TOOL,
        FBM_PQ_TOOL_ADDR_ALIGMENT, FBM_PQ_TOOL_SIZE_ALIGMENT,
        0,
        FBM_PQ_TOOL_POOL_SIZE,
        0, 0,
        FBM_POOL_INHERIT
    },

    //OD
    {
        "OD",
        FBM_POOL_TYPE_OD,
        FBM_OD_ADDR_ALIGMENT, FBM_OD_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //VENC
    {
        "VENC",
        FBM_POOL_TYPE_VENC,
        FBM_VENC_ADDR_ALIGMENT, FBM_VENC_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //SWDMX
    {
        "SWDMX",
        FBM_POOL_TYPE_SWDMX,
        FBM_SWDMX_ADDR_ALIGMENT, FBM_SWDMX_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //FACEDET
    {
        "FACEDET",
        FBM_POOL_TYPE_FACEDET,
        FBM_FD_ADDR_ALIGMENT, FBM_FD_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //FBM_POOL_TYPE_MPEG_FREE
    {
        "FBM_POOL_TYPE_MPEG_FREE",
        FBM_POOL_TYPE_MPEG_FREE,
        0, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //FBM_POOL_TYPE_DIVA
    {
        "FBM_POOL_TYPE_DIVA",
        FBM_POOL_TYPE_DIVA,
        0, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //Feeder2
    {
        "Feeder2",
        FBM_POOL_TYPE_FEEDER2,
        FBM_FEEDER_ADDR_ALIGMENT, FBM_FEEDER_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //DMX2
    {
        "DMX2",
        FBM_POOL_TYPE_DMX2,
        FBM_DMX_ADDR_ALIGMENT, FBM_DMX_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //MPEG2
    {
        "MPEG2",
        FBM_POOL_TYPE_MPEG2,
        FBM_MPEG_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //PVR
    {
        "PVR",
        FBM_POOL_TYPE_PVR,
        FBM_PVR_ADDR_ALIGMENT, FBM_PVR_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //PVR2
    {
        "PVR2",
        FBM_POOL_TYPE_PVR2,
        FBM_PVR2_ADDR_ALIGMENT, FBM_PVR2_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //JPG_VDP
    {
        "JPG_VDP",
        FBM_POOL_TYPE_JPG_VDP,
        FBM_JPG_VDP_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    // DIVX
    {
        "DIVX",
        FBM_POOL_TYPE_DIVX,
        FBM_DIVX_ADDR_ALIGMENT, FBM_DIVX_SIZE_ALIGMENT,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },

    //MPEG3
    {
        "MPEG3",
        FBM_POOL_TYPE_MPEG3,
        FBM_MPEG_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1080,
        FBM_POOL_INHERIT
    },
    //Video screen Shot
    {
        "VSS",
        FBM_POOL_TYPE_VSS,
        FBM_VSS_ADDR_ALIGMENT, FBM_VSS_SIZE_ALIGMENT,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //DMXPID
    {
        "DMXPID",
        FBM_POOL_TYPE_DMXPID,
        64, 64,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //FBM_POOL_TYPE_DRM
    {
        "DRM",
        FBM_POOL_TYPE_DRM,
        4*1024, 4*1024,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //TOTAL-2
    {
        "TOTAL2",
        FBM_POOL_TYPE_TOTAL2,
        0, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },

    //TOTAL
    {
        "TOTAL",
        FBM_POOL_TYPE_TOTAL,
        0, 0,
        0,
        0,
        0, 0,
        FBM_POOL_INHERIT
    },
};

static const UINT32 _u4PoolListNs = (UINT32)FBM_POOL_TYPE_NS;

#if defined(CC_MT8223)
static FBP_LIST_T _arFbpCqamList[] =
{
    {
        FBM_FBG_TYPE_1080HD,
        0x0,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
#else
// MPEG Frame Buffer Group Type List, size of _arFbpList[n] > size of _arFbpList[n + 1]
static FBP_LIST_T _arFbpCqamList[] =
{
    {
        FBM_FBG_TYPE_1080HD,
        FBM_FBG_TYPE_1080HD_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_720HD,
        FBM_FBG_TYPE_720HD_SIZE,
        FBM_FBG_TYPE_720HD_Y_SIZE,
        FBM_FBG_TYPE_720HD_C_SIZE,
        FBM_FBG_720HD_H,
        FBM_FBG_720HD_V
    },

    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
#endif

#if defined(CC_MT8223)
static const UCHAR _ucFbpCqamListNs = 1;
#else
static const UCHAR _ucFbpCqamListNs = 3;
#endif

#if defined(CC_FBM_INT_TWO_FBP) || defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST)
static FBP_LIST_T _arFbpTwoLists[] =
{
    {
        FBM_FBG_TYPE_SBSHD,
        FBM_FBG_TYPE_1080HD_H264_SIZE*2,
        FBM_FBG_TYPE_SBSHD_Y_SIZE,
        FBM_FBG_TYPE_SBSHD_C_SIZE,
        FBM_FBG_SBSHD_H,
        FBM_FBG_SBSHD_V
    },
    {
        FBM_FBG_TYPE_TNBHD,
        FBM_FBG_TYPE_1080HD_H264_SIZE*2,
        FBM_FBG_TYPE_TNBHD_Y_SIZE,
        FBM_FBG_TYPE_TNBHD_C_SIZE,
        FBM_FBG_TNBHD_H,
        FBM_FBG_TNBHD_V
    },
    {
        FBM_FBG_TYPE_1080HD,
        FBM_FBG_TYPE_1080HD_H264_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },
    {
        FBM_FBG_TYPE_720HD,
        FBM_FBG_TYPE_720HD_H264_SIZE,
        FBM_FBG_TYPE_720HD_Y_SIZE,
        FBM_FBG_TYPE_720HD_C_SIZE,
        FBM_FBG_720HD_H,
        FBM_FBG_720HD_V
    },
    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_H264_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
static const UCHAR _ucFbpTwoListNs = 5;
#endif

#if 1
//mt539x Frame Buffer Group Type List, size of _arFbpList[n] > size of _arFbpList[n + 1]
static FBP_LIST_T _arFbpH264List[] =
{
    {
        FBM_FBG_TYPE_1080HD,
        FBM_FBG_TYPE_1080HD_H264_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },
    {
        FBM_FBG_TYPE_720HD,
        FBM_FBG_TYPE_720HD_H264_SIZE,
        FBM_FBG_TYPE_720HD_Y_SIZE,
        FBM_FBG_TYPE_720HD_C_SIZE,
        FBM_FBG_720HD_H,
        FBM_FBG_720HD_V
    },

    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_H264_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
static const UCHAR _ucFbpH264ListNs = 3;
#else
static FBP_LIST_T _arFbpH264List[] =
{
    {
        FBM_FBG_TYPE_1080HD,
        FBM_FBG_TYPE_1080HD_H264_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    }
};
static const UCHAR _ucFbpH264ListNs = 1;
#endif


static FBP_LIST_T _arFbpH264List64M[] =
{
    {
        FBM_FBG_TYPE_1080HD,
        FBM_FBG_TYPE_1080HD_H264_64M_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_H264_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};

static const UCHAR _ucFbpH264List64MNs = 2;


#if 0
// MPEG DBK Frame Buffer Group Type List, size of _arFbpDbkList[n] > size of _arFbpDbkList[n + 1]
static FBP_LIST_T _arFbpCqamDbkList[] =
{
    {
        FBM_FBG_TYPE_1080HD,
        FBM_FBG_TYPE_1080HD_DBK_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_720HD,
        FBM_FBG_TYPE_720HD_DBK_SIZE,
        FBM_FBG_TYPE_720HD_Y_SIZE,
        FBM_FBG_TYPE_720HD_C_SIZE,
        FBM_FBG_720HD_H,
        FBM_FBG_720HD_V
    },

    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_DBK_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
#endif

//static const UCHAR _ucFbpCqamDbkListNs = 3;


#if 0
static FBP_LIST_T _arFbpH264PipList[] =
{
    {
        FBM_FBG_TYPE_1080HD_RR,
        FBM_FBG_TYPE_1080HD_H264_RR_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_H264_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
#endif

//static const UCHAR _ucFbpH264PipListNs = 2;

static FBP_LIST_T _arFbpH264PipList64M[] =
{
    {
        FBM_FBG_TYPE_1080HD_RR,
        FBM_FBG_TYPE_1080HD_H264_RR_64M_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_H264_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};

static const UCHAR _ucFbpH264PipList64MNs = 2;

#if 0
// MPEG pool type at Pip Path for 5391
static FBP_LIST_T _arFbpMPEG2PipList[] =
{
    {
        FBM_FBG_TYPE_1080HD_RR,
        FBM_FBG_TYPE_1080HD_2_FB_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_720HD,
        FBM_FBG_TYPE_720HD_SIZE,
        FBM_FBG_TYPE_720HD_Y_SIZE,
        FBM_FBG_TYPE_720HD_C_SIZE,
        FBM_FBG_720HD_H,
        FBM_FBG_720HD_V
    },

    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
#endif

//static const UCHAR _ucFbpMPEG2PipListNs = 3;

#if 0
// MPEG DVBT PAL
static FBP_LIST_T _arFbpDvbtDbkList[] =
{
    {
        FBM_FBG_TYPE_PAL_DBK,
        FBM_FBG_TYPE_STD_PAL_DBK_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};
#endif

//static const UCHAR _ucFbpDvbtDbkListNs = 1;

// MPEG DVBT PAL
static FBP_LIST_T _arFbpDvbtList[] =
{
    {
        FBM_FBG_TYPE_PAL,
        FBM_FBG_TYPE_PAL_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};

static const UCHAR _ucFbpDvbtListNs = 1;


static FBP_LIST_T* _arFbpList = NULL;

static UCHAR _ucFbpListNs = 0;

// Frame Buffer Pool Configuration
// 1 HD System
#if defined(CC_FBM_INT_TWO_FBP) || defined(CC_FBM_TWO_FBP) || defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST)
static FBP_CONF_T _arFbpConf[2] =
#else
static FBP_CONF_T _arFbpConf[] =
#endif
{
    {FBM_FBG_TYPE_1080HD,   (UINT32)NULL},
};

static UCHAR _ucFbpConfNs = 1;

// frame buffer pool structure
static FBP_T *_prFbp[FBP_MAX_NS];

// frame buffer pool number
static UCHAR _ucFbpNs = 0;

static FBM_POOL_T _arPool[FBM_POOL_TYPE_NS];

//  frame buffer pool memory address
static UINT32 u4FbMallocAddr = (UINT32)NULL;

// FBM call back function
static FBM_CB_FUNC_T* _prFbmCbFunc = NULL;

// Current Frame Buffer Pool
static FBM_POOL_T _rCurrentMpeg;

static HANDLE_T _hFbmPoolMutex;

static UINT32 _u4SumOfRoot;
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *      Enable Memory Intrusion Detection for RO region
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static VOID _EnableMpegPoolMemoryIntrusionDetection(UINT32 u4Addr, UINT32 u4Size)
{
#ifdef FBM_MPEG_POOL_MID_EN
    UINT32 u4LowAddr, u4HighAddr, u4Region;

    u4Region = 2;
    u4LowAddr = (u4Addr + MID_ADDR_ALIGNMENT - 1) & ~(MID_ADDR_ALIGNMENT - 1);
    u4HighAddr = ((u4Addr + u4Size) - MID_ADDR_ALIGNMENT + 1) & ~(MID_ADDR_ALIGNMENT - 1);

    // Disable the region detection
    if (!MID_DisableRegionProtect(u4Region))
    {
    }

    // Set RO section to read-only
    if (!MID_SetRegionEx(u4Region, MID_AGENT_MPEG2, u4LowAddr, u4HighAddr, MID_FLAG_CPUACCESS))
    {
        ASSERT(0);
    }

    // Enable the region detection
    if (!MID_EnableRegionProtect(u4Region))
    {
        ASSERT(0);
    }
#else
    UNUSED(u4Addr);
    UNUSED(u4Size);
#endif
}

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *      Disable Memory Intrusion Detection for RO region
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static VOID _DisableMpegPoolMemoryIntrusionDetection(VOID)
{
#ifdef FBM_MPEG_POOL_MID_EN

    // Disable the region detection
    if (!MID_DisableRegionProtect(2))
    {
    }

#endif
}

//-------------------------------------------------------------------------
/** _FbgPoolDisassemble
 *  Disassemble FBM pool
 *  @param  ucDisassembleIdx   Pool index.
 *  @return NONE
 */
//-------------------------------------------------------------------------
static VOID _FbgPoolDisassemble(UCHAR ucDisassembleIdx)
{
    UINT32 u4PoolIdx;
    UINT32 u4TargetIdx;
    UINT32 u4TargetType = FBM_FBG_TYPE_UNKNOWN;
    UINT32 u4TargetSize = 0;

    if ((_prFbp[ucDisassembleIdx] != NULL) &&
            (_prFbp[ucDisassembleIdx]->ucStatus == FBP_STATUS_EMPTY))
    {
        FBM_POOL_MUTEX_LOCK();
        // search for pool type to disassemble, largest
        for (u4PoolIdx = 0; u4PoolIdx < _ucFbpListNs; u4PoolIdx++)
        {
            if (_prFbp[ucDisassembleIdx]->u4Size > _arFbpList[u4PoolIdx].u4Size)
            {
                if (_arFbpList[u4PoolIdx].u4Size > u4TargetSize)
                {
                    u4TargetSize = _arFbpList[u4PoolIdx].u4Size;
                    u4TargetType = _arFbpList[u4PoolIdx].ucType;
                }
            }
        }
        FBM_POOL_MUTEX_UNLOCK();

        if ((u4TargetType != FBM_FBG_TYPE_UNKNOWN) &&
                (u4TargetSize != 0))
        {
            UINT32 u4DisassembleNs;

            u4DisassembleNs = (UCHAR)(_prFbp[ucDisassembleIdx]->u4Size / u4TargetSize);

            if (u4DisassembleNs > (FBP_MAX_NS - (UINT32)_ucFbpNs))
            {
                u4DisassembleNs = (FBP_MAX_NS - (UINT32)_ucFbpNs);
            }

            if (u4DisassembleNs > FBP_MAX_DISASSEMBLY)
            {
                u4DisassembleNs = FBP_MAX_DISASSEMBLY;
            }

            for (u4PoolIdx = 0; u4PoolIdx < u4DisassembleNs; u4PoolIdx++)
            {
                u4TargetIdx = 0;
                while (u4TargetIdx < FBP_MAX_NS)
                {
                    if (_prFbp[u4TargetIdx] == NULL)
                    {
                        break;
                    }
                    else
                    {
                        u4TargetIdx++;
                    }
                }

                if (u4TargetIdx < FBP_MAX_NS)
                {
                    _prFbp[u4TargetIdx] = (FBP_T *)x_mem_alloc(sizeof(FBP_T));
                    ASSERT(_prFbp[u4TargetIdx] != NULL);

                    _ucFbpNs++;
                    _prFbp[u4TargetIdx]->ucStatus = FBP_STATUS_EMPTY;
                    _prFbp[u4TargetIdx]->ucType = u4TargetType;
                    _prFbp[u4TargetIdx]->u4Size = u4TargetSize;
                    _prFbp[u4TargetIdx]->u4Addr = _prFbp[ucDisassembleIdx]->u4Addr + (u4PoolIdx * u4TargetSize);
                    _prFbp[u4TargetIdx]->ucInheritance = ucDisassembleIdx;

                    FBM_MEMSET((VOID *)_prFbp[u4TargetIdx]->aucOccupyList, FBP_MAX_NS, FBP_MAX_DISASSEMBLY);

                    _prFbp[ucDisassembleIdx]->aucOccupyList[u4PoolIdx] = u4TargetIdx;
                }
                else
                {
                    ASSERT(0);
                }
            }

            _prFbp[ucDisassembleIdx]->ucStatus = FBP_STATUS_OCCUPY;
        }
        else
        {
            ASSERT(0);
        }
    }
}


//-------------------------------------------------------------------------
/** _FbgPoolAssemble
 *  Assemble FBM pool
 *  @return NONE
 */
//-------------------------------------------------------------------------
static UCHAR _FbgPoolAssemble(VOID)
{
    UINT32 u4Idx;
    UINT32 u4OccupyIdx;
    UINT32 u4CanAssemable;

    for (u4Idx = 0; u4Idx < FBP_MAX_NS; u4Idx++)
    {
        if ((_prFbp[u4Idx] != NULL) &&
                (_prFbp[u4Idx]->ucStatus == FBP_STATUS_OCCUPY))
        {

            u4CanAssemable = 1;
            for (u4OccupyIdx = 0; u4OccupyIdx < FBP_MAX_DISASSEMBLY; u4OccupyIdx++)
            {
                if ((_prFbp[u4Idx]->aucOccupyList[u4OccupyIdx] != FBP_MAX_NS) &&
                        (_prFbp[_prFbp[u4Idx]->aucOccupyList[u4OccupyIdx]]->ucStatus != FBP_STATUS_EMPTY))
                {
                    u4CanAssemable = 0;
                }
            }

            if (u4CanAssemable == 1)
            {
                for (u4OccupyIdx = 0; u4OccupyIdx < FBP_MAX_DISASSEMBLY; u4OccupyIdx++)
                {
                    if (_prFbp[u4Idx]->aucOccupyList[u4OccupyIdx] != FBP_MAX_NS)
                    {
                        x_mem_free(_prFbp[_prFbp[u4Idx]->aucOccupyList[u4OccupyIdx]]);
                        _prFbp[_prFbp[u4Idx]->aucOccupyList[u4OccupyIdx]] = NULL;
                        _ucFbpNs--;
                    }
                }

                _prFbp[u4Idx]->ucStatus = FBP_STATUS_EMPTY;
                FBM_MEMSET((VOID *)_prFbp[u4Idx]->aucOccupyList, FBP_MAX_NS, FBP_MAX_DISASSEMBLY);

                return u4Idx;
            }
        }
    }

    return FBP_MAX_NS;
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

#if defined(CC_FBM_2ND_CHANNEL)
#if defined(CC_MT5395)
#if defined(CC_CHA_CHB_NO_GAP)
#define DRAM_CHB_START_ADDR (DRVCUST_OptGet(eTotalMemSize))
#else
#define DRAM_CHB_START_ADDR (0x20000000)
#endif
#else
#define DRAM_CHB_START_ADDR (DRVCUST_OptGet(eTotalMemSize))
#endif

UINT8 FBM_IsAddrInChB(UINT32 u4Addr)
{
#if defined(CC_MT5395) || defined(CC_MT5396)
    if (u4Addr >= DRAM_CHB_START_ADDR)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}
#endif

//-------------------------------------------------------------------------
/** _FBM_PoolInit
 *  Initialize this FBM pool
 *  @retval NONE
 */
//-------------------------------------------------------------------------
FBM_POOL_T* _FBM_PoolInit(VOID)
{
    UINT32 u4PoolIdx;
    UINT32 u4ListIdx;
    UINT32 u4MpegSize;
    UINT32 u4PoolTpIdx;

    UINT32 u4Size = 0;
    //UINT32 u4LastPoolIdx = 0;
    //UINT32 u4LastPoolSize = 0;

    UINT32 u4FbmStartAddr;
#if defined(CC_FBM_2ND_CHANNEL)
    UINT32 u4FbmStartAddr2;
#endif


    // create fbm semaphore
    VERIFY(x_sema_create(&_hFbmPoolMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

#ifndef CC_MEM_TOOL
    _prFbmCbFunc = FBM_GetCbFuncTbl();
#endif
    u4FbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);
#if defined(CC_FBM_2ND_CHANNEL)
    u4FbmStartAddr2 = DRAM_CHB_START_ADDR;
#endif

#ifdef CC_SRM_ON
    // Reg SRM Callback Function
    SRM_Init();
    SRM_Pause();
    SRM_RegisterCB(SRM_MPEG_PIP_CTRL, _FBM_SrmCtrlCb);
#endif

#if 0
    do
    {

        LOG(1, "MDDI 8Bit\n");
        LOG(1, "SD = 0x%x\n", FBM_MDDI_SD_POOL_SIZE);
        LOG(1, "1440 = 0x%x\n", FBM_MDDI_1440PANEL_POOL_SIZE);
        LOG(1, "FHD = 0x%x\n", FBM_MDDI_FHDPANEL_POOL_SIZE);
        LOG(1, "MDDI 10Bit\n");
        LOG(1, "SD = 0x%x\n", FBM_MDDI_SD_10BIT_POOL_SIZE);
        LOG(1, "1440 = 0x%x\n", FBM_MDDI_1440PANEL_10BIT_POOL_SIZE);
        LOG(1, "FHD = 0x%x\n", FBM_MDDI_FHDPANEL_10BIT_POOL_SIZE);

        LOG(1, "SCPOS 8Bit\n");
        LOG(1, "SD = 0x%x\n", FBM_SCPOS_SD_POOL_SIZE);
        LOG(1, "1440 = 0x%x\n", FBM_SCPOS_1440PANEL_POOL_SIZE);
        LOG(1, "FHD = 0x%x\n", FBM_SCPOS_FHDPANEL_POOL_SIZE);
        LOG(1, "SCPOS 10Bit\n");
        LOG(1, "SD = 0x%x\n", FBM_SCPOS_SD_10BIT_POOL_SIZE);
        LOG(1, "1440 = 0x%x\n", FBM_SCPOS_1440PANEL_10BIT_POOL_SIZE);
        LOG(1, "FHD = 0x%x\n", FBM_SCPOS_FHDPANEL_10BIT_POOL_SIZE);
        LOG(1, "SCPOS 8Bit 2x422\n");
        LOG(1, "SD = 0x%x\n", FBM_SCPOS_2FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576));
        LOG(1, "1440 = 0x%x\n", FBM_SCPOS_2FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1440, 900));
        LOG(1, "FHD = 0x%x\n", FBM_SCPOS_2FB_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 1920, 1080));

        LOG(1, "MJC_Y8=%d, ", FBM_MJC_Y8_SIZE(4));
        LOG(1, "MJC_Y2=%d, ", FBM_MJC_Y2_SIZE(0, 0));
        LOG(1, "MJC_C8=%d, ", FBM_MJC_C8_SIZE(0));
        LOG(1, "MJC_C2=%d, ", FBM_MJC_C2_SIZE(0, 0));
        LOG(1, "MJC_MV=%d\n", FBM_MJC_MV_SIZE);
    } while (0);
#endif

    // double check
    for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
    {
        if (u4ListIdx != (UINT32)_arPoolList[u4ListIdx].eType)
        {
            VERIFY(0);
        }
    }
#if 0
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392B_EU_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392H_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391H_FHD60_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391_VERF) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_SH_MT5391F) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_SH_MT5391BF) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_SPACE) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_ROW))
    {
        _arFbpList = _arFbpH264List;
        _ucFbpListNs = _ucFbpH264ListNs;

        _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
        _ucFbpConfNs = 1;
    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392B_US_128MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392F_128MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391F_FHD60_128MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_SH_MT5391BF_128MB))
    {
        _arFbpList = _arFbpCqamDbkList;
        _ucFbpListNs = _ucFbpCqamDbkListNs;

        _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
        _ucFbpConfNs = 1;

    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360_VERF_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360x64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360B_VERF_64MB))
    {
        _arFbpList = _arFbpH264List64M;
        _ucFbpListNs = _ucFbpH264List64MNs;

        _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
        _ucFbpConfNs = 1;

    }
    else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_64MB) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_64MB_MOUT) ||
             (SRMFBM_GetConf() == FBM_MEM_CFG_MT5361B_64MB))
    {
        _arFbpList = _arFbpDvbtDbkList;
        _ucFbpListNs = _ucFbpDvbtDbkListNs;

        _arFbpConf[0].ucType = FBM_FBG_TYPE_PAL_DBK;
        _ucFbpConfNs = 1;

    }
    else
#endif
        if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5363_64MBx2)
#if defined(CC_MT5365)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_128Mx1_NOSUB)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_64Mx1)
#endif
#if defined(CC_MT5395) || defined(CC_MT5396)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR2x2)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTV)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHA_NOMJC)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_3DTVCHB_NOMJC)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_NOMJC)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5395_DDR3x2_EU_3DTV_60HZ)
#endif
#if defined(CC_MT5396)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_DDR3x4)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5396_3DTV_CHA_NOMJC)
#endif
#if defined(CC_MT5368)||defined(CC_MT5389)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x1)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_DDR3x2)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_3DTV)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_128Mx1_SHRINK)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5368_64Mx1)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT6595_COMP)
#if defined(CC_MT5365)
                || (SRMFBM_GetConf() == FBM_MEM_CFG_MT5365_3DTV)
#endif
#endif
           )
        {
            _arFbpList = _arFbpH264List;
            _ucFbpListNs = _ucFbpH264ListNs;

            _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
            _ucFbpConfNs = 1;
        }
        else
        {
            _arFbpList = _arFbpCqamList;
            _ucFbpListNs = _ucFbpCqamListNs;

            _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
            _ucFbpConfNs = 1;
        }


#ifdef CC_FBM_TWO_FBP
    _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
    _arFbpConf[1].ucType = FBM_FBG_TYPE_1080HD;
    _ucFbpConfNs = 2;
#endif

#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST)
    _arFbpList = _arFbpTwoLists;
    _ucFbpListNs = _ucFbpTwoListNs;

    _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
    _arFbpConf[1].ucType = FBM_FBG_TYPE_1080HD;
    _ucFbpConfNs = 2;
#endif

#if defined(CC_FBM_INT_TWO_FBP)
    _arFbpList = _arFbpTwoLists;
    _ucFbpListNs = _ucFbpTwoListNs;

    _arFbpConf[0].ucType = FBM_FBG_TYPE_SBSHD;
    _arFbpConf[1].ucType = FBM_FBG_TYPE_UNKNOWN;
    _ucFbpConfNs = 2;
#endif

    UNUSED(_arFbpDvbtList);
    UNUSED(_ucFbpDvbtListNs);

    // init pool arrange info
    for (u4ListIdx = 0; u4ListIdx < (UINT32) FBM_POOL_TYPE_NS; u4ListIdx++)
    {
        arPoolArrangeInfo[u4ListIdx].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[u4ListIdx].ePool1 = FBM_POOL_TYPE_TOTAL;
        arPoolArrangeInfo[u4ListIdx].ePool2 = FBM_POOL_TYPE_TOTAL;
        arPoolArrangeInfo[u4ListIdx].u4Offset = 0;
        arPoolArrangeInfo[u4ListIdx].u1Ready = 0;
    }

//    FBM_ConfigurePool(&_arPoolList, &arPoolArrangeInfo);
    FBM_ConfigurePool(_arPoolList, arPoolArrangeInfo);

    // Calculate mpeg total frame buffer size
    u4MpegSize = 0;

#if defined(CC_FBM_INT_TWO_FBP) || defined(CC_FBM_TWO_FBP) || defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST)
    for (u4PoolIdx = 0; u4PoolIdx < 1; u4PoolIdx++)
#else
    for (u4PoolIdx = 0; u4PoolIdx < _ucFbpConfNs; u4PoolIdx++)
#endif
    {
        for (u4ListIdx = 0; u4ListIdx < _ucFbpListNs; u4ListIdx++)
        {
            if (_arFbpConf[u4PoolIdx].ucType == _arFbpList[u4ListIdx].ucType)
            {
                u4MpegSize += _arFbpList[u4ListIdx].u4Size;
            }
        }
    }


#if defined(CC_FBM_INT_TWO_FBP) && defined(CC_FBM_INT_VOMX)
    _arPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = u4MpegSize + (0x2900000 - u4MpegSize/2) + FBM_EXTRA_FOR_SEAMLESS;
#else
    _arPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = u4MpegSize + FBM_EXTRA_FOR_SEAMLESS;
#endif

    _arPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize += FBM_MPEG_POOL_PAD;  // Protection

    if (u4FbMallocAddr == (UINT32)NULL)
    {
        UINT32 u4Start;
        UINT32 u4End;
        UINT32 u4MaxAddrAlign;
        UINT32 u4ReservedSize;
        UINT32 u4Ready;

        // Calculate Total Size
        u4Size = 0;
        for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
        {
            if ((_arPoolList[u4ListIdx].u4Inherit == FBM_POOL_ROOT) &&
                    (_arPoolList[u4ListIdx].eType != FBM_POOL_TYPE_TOTAL))
            {
                u4Size += _arPoolList[u4ListIdx].u4PoolSize;
            }
        }
        _u4SumOfRoot = u4Size;

        u4ReservedSize = DRVCUST_OptGet(eFbmMemSize);

        if (u4ReservedSize < u4Size)
        {
            Printf("FBM size is not enough before allocate, reserved=0x%x, required=0x%x\n", u4ReservedSize, u4Size);
            VERIFY(0);
        }
#ifdef CC_FBM_3D_OSD
#if defined(CC_MT5389)
		if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
		{
		if (_arPoolList[FBM_POOL_TYPE_PQ_TV].u4PoolSize <(_arPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize+0x1000000))
        {
            Printf("FBM_POOL_TYPE_PQ_MM size is not enough for support 3D Photo, reserved=0x%x, required=0x%x\n", _arPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize, 
				_arPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize+0x1000000);
            VERIFY(0);
        }
		}
		else
#endif
        if (_arPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize <(_arPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize+_arPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize+0x1000000) )
        {
            Printf("FBM_POOL_TYPE_SCPOS_MAIN size is not enough for support 3D Photo, reserved=0x%x, required=0x%x\n", _arPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize,
                   _arPoolList[FBM_POOL_TYPE_FEEDER_MMP].u4PoolSize+_arPoolList[FBM_POOL_TYPE_JPEG].u4PoolSize+0x1000000);
            VERIFY(0);
        }
#endif
        // handle total pool
        u4MaxAddrAlign = 0;
        for (u4PoolTpIdx = 0; u4PoolTpIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolTpIdx++)
        {
            if (_arPoolList[u4PoolTpIdx].u4AddrAlign > u4MaxAddrAlign)
            {
                u4MaxAddrAlign = _arPoolList[u4PoolTpIdx].u4AddrAlign;
            }
        }
        // _arPool[FBM_POOL_TYPE_TOTAL].u4Addr = FBM_ALIGN_MASK((UINT32)(u4FbmStartAddr)-u4MaxAddrAlign, u4MaxAddrAlign); // keep it for history...
        _arPool[FBM_POOL_TYPE_TOTAL].u4Addr = FBM_ALIGN_MASK((UINT32)(u4FbmStartAddr), u4MaxAddrAlign);
        _arPool[FBM_POOL_TYPE_TOTAL].u4Size = (UINT32)DRVCUST_OptGet(eFbmMemSize);
        arPoolArrangeInfo[FBM_POOL_TYPE_TOTAL].u1Ready = 1;

#if defined(CC_FBM_2ND_CHANNEL)
        _arPool[FBM_POOL_TYPE_TOTAL2].u4Addr = u4FbmStartAddr2;;
        _arPool[FBM_POOL_TYPE_TOTAL2].u4Size = BSP_GetChBMemSize();
        arPoolArrangeInfo[FBM_POOL_TYPE_TOTAL2].u1Ready = 1;
#endif

        // handle non-inherit pool
        do
        {
            for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
            {
                if ((_arPoolList[u4ListIdx].u4Inherit == FBM_POOL_ROOT) &&
                        (arPoolArrangeInfo[u4ListIdx].u1Ready == 0))
                {
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_AFTER)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;
                        if ((UINT32)eAheadPool >= FBM_POOL_TYPE_NS)
                        {
                            ASSERT(TRUE);
                            continue;
                        }

                        if (eAheadPool == FBM_POOL_TYPE_TOTAL)
                        {
                            // start from the begining
                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4FbmStartAddr, _arPoolList[u4ListIdx].u4AddrAlign);
                            _arPool[FBM_POOL_TYPE_TOTAL].u4Addr = _arPool[u4ListIdx].u4Addr;
                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
#if defined(CC_FBM_2ND_CHANNEL)
                        else if (eAheadPool == FBM_POOL_TYPE_TOTAL2)
                        {
                            // start from the begining
                            _arPool[u4ListIdx].u4Addr = u4FbmStartAddr2;;

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
#endif
                        else
                        {
                            // after another pool
                            // check 'ahead pool' ready or not
                            if (arPoolArrangeInfo[eAheadPool].u1Ready != 0)
                            {
                                u4Start = _arPool[eAheadPool].u4Addr;
                                u4Start += _arPool[eAheadPool].u4Size;
                                u4Start += FBM_POOL_PADDING_SIZE;

                                _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                                arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                                _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                                _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                                _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                                _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                                _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                            }

                            ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                        }
                    }
                    else if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_BEFORE)
                    {
                        FBM_POOL_TYPE_T eBehindPool;
                        eBehindPool = arPoolArrangeInfo[u4ListIdx].ePool1;

                        // check 'behind pool' ready or not
                        if (eBehindPool == FBM_POOL_TYPE_NS)
                        {
                            u4Start = u4FbmStartAddr + u4ReservedSize;
                            u4Start -= _arPoolList[u4ListIdx].u4PoolSize;
                            u4Start -= _arPoolList[u4ListIdx].u4AddrAlign;
                            u4Start -= FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
#if 0 // defined(CC_FBM_2ND_CHANNEL)
                        else if (eBehindPool == FBM_POOL_TYPE_TOTAL2)
                        {
                            _arPool[u4ListIdx].u4Addr = u4FbmStartAddr2 + BSP_GetChBMemSize() - _arPoolList[u4ListIdx].u4PoolSize;

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
#endif
                        else if (arPoolArrangeInfo[eBehindPool].u1Ready != 0)
                        {
                            u4Start = _arPool[eBehindPool].u4Addr;
                            u4Start -= _arPoolList[u4ListIdx].u4PoolSize;
                            u4Start -= _arPoolList[u4ListIdx].u4AddrAlign;
                            u4Start -= FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eBehindPool);
                    }
                    else
                    {
                        // only after mode for non-inherit pool
                        ASSERT(0);
                    }
                }
            }

            // check all non-inherit pool ready
            u4Ready = 1;
            for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
            {
                if ((_arPoolList[u4ListIdx].u4Inherit == FBM_POOL_ROOT) &&
                        (arPoolArrangeInfo[u4ListIdx].u1Ready == 0))
                {
                    u4Ready = 0;
                }
            }
        } while (u4Ready == 0);

        // handle inherit pool
        do
        {
            for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
            {
                if ((_arPoolList[u4ListIdx].u4Inherit == FBM_POOL_INHERIT) &&
                        (arPoolArrangeInfo[u4ListIdx].u1Ready == 0))
                {
                    if ((_arPoolList[u4ListIdx].u4PoolSize == 0) &&
                            (arPoolArrangeInfo[u4ListIdx].eMode != FBM_POOL_ARRANGE_BETWEEN) &&
                            (arPoolArrangeInfo[u4ListIdx].eMode != FBM_POOL_ARRANGE_SHARE_ALL))
                    {
                        arPoolArrangeInfo[u4ListIdx].u1Ready = 1;
                    }
                    else if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_SHARE)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;

                        FBM_CHECK_POOL_INDEX(eAheadPool);

                        // check 'ahead pool' ready or not
                        if (arPoolArrangeInfo[eAheadPool].u1Ready != 0)
                        {
                            ASSERT(_arPool[eAheadPool].u4Size > _arPoolList[u4ListIdx].u4PoolSize);

                            u4Start = _arPool[eAheadPool].u4Addr;

                            if (arPoolArrangeInfo[u4ListIdx].u4Offset != 0)
                            {
                                u4Start += arPoolArrangeInfo[u4ListIdx].u4Offset;
                                u4Start += FBM_POOL_PADDING_SIZE;
                            }

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                    }
                    else if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_SHARE_ALL)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        FBM_POOL_TYPE_T eAheadPool2;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;
                        eAheadPool2 = arPoolArrangeInfo[u4ListIdx].ePool2;

                        FBM_CHECK_POOL_INDEX(eAheadPool);
                        FBM_CHECK_POOL_INDEX(eAheadPool2);

                        // check 'ahead pool' ready or not
                        if (arPoolArrangeInfo[eAheadPool].u1Ready != 0)
                        {
                            if ((arPoolArrangeInfo[u4ListIdx].u4Offset == 0) &&
                                    (eAheadPool != eAheadPool2))
                            {
                                // check 'ahead pool' ready or not
                                if (arPoolArrangeInfo[eAheadPool2].u1Ready != 0)
                                {
                                    u4Start = _arPool[eAheadPool].u4Addr;
                                    u4Start += _arPool[eAheadPool2].u4Size;
                                    u4Start += FBM_POOL_PADDING_SIZE;

                                    _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                                    arPoolArrangeInfo[u4ListIdx].u1Ready = 1;
                                }

                                ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool2);
                            }
                            else
                            {
                                u4Start = _arPool[eAheadPool].u4Addr;

                                if (arPoolArrangeInfo[u4ListIdx].u4Offset != 0)
                                {
                                    u4Start += arPoolArrangeInfo[u4ListIdx].u4Offset;
                                    u4Start += FBM_POOL_PADDING_SIZE;
                                }

                                _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                                arPoolArrangeInfo[u4ListIdx].u1Ready = 1;
                            }

                            if (arPoolArrangeInfo[u4ListIdx].u1Ready != 0)
                            {
                                if ((_arPool[eAheadPool].u4Addr > _arPool[u4ListIdx].u4Addr) ||
                                        ((_arPool[u4ListIdx].u4Addr - _arPool[eAheadPool].u4Addr) > _arPoolList[eAheadPool].u4PoolSize))
                                {
                                    _arPool[u4ListIdx].u4Size = 0;
                                }
                                else
                                {
                                    _arPool[u4ListIdx].u4Size = _arPoolList[eAheadPool].u4PoolSize -
                                                                (_arPool[u4ListIdx].u4Addr - _arPool[eAheadPool].u4Addr);
                                }

                                // Size Alignment
                                _arPool[u4ListIdx].u4Size = (_arPool[u4ListIdx].u4Size & (~(_arPoolList[u4ListIdx].u4SizeAlign)));

                                _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                                _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                                _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                                _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                            }
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                    }
                    else if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_AFTER)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;

                        FBM_CHECK_POOL_INDEX(eAheadPool);

                        // check 'ahead pool' ready or not
                        if (arPoolArrangeInfo[eAheadPool].u1Ready != 0)
                        {
                            if (eAheadPool == FBM_POOL_TYPE_TOTAL)
                            {
                                _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4FbmStartAddr, _arPoolList[u4ListIdx].u4AddrAlign);
                            }
#if defined(CC_FBM_2ND_CHANNEL)
                            else if (eAheadPool == FBM_POOL_TYPE_TOTAL2)
                            {
                                _arPool[u4ListIdx].u4Addr = u4FbmStartAddr2;;
                            }
#endif
                            else
                            {
                                u4Start = _arPool[eAheadPool].u4Addr;
                                u4Start += _arPool[eAheadPool].u4Size;
                                u4Start += FBM_POOL_PADDING_SIZE;

                                _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);
                            }

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                    }
                    else if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_BEFORE)
                    {
                        FBM_POOL_TYPE_T eBehindPool;
                        eBehindPool = arPoolArrangeInfo[u4ListIdx].ePool1;

                        // check 'behind pool' ready or not
                        if (eBehindPool == FBM_POOL_TYPE_NS)
                        {
                            u4Start = u4FbmStartAddr + u4ReservedSize;
                            u4Start -= _arPoolList[u4ListIdx].u4PoolSize;
                            u4Start -= _arPoolList[u4ListIdx].u4AddrAlign;
                            u4Start -= FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
#if defined(CC_FBM_2ND_CHANNEL)
                        else if (eBehindPool == FBM_POOL_TYPE_TOTAL2)
                        {
                            u4Start = u4FbmStartAddr2 + BSP_GetChBMemSize() - _arPoolList[u4ListIdx].u4PoolSize;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
#endif
                        else if (arPoolArrangeInfo[eBehindPool].u1Ready != 0)
                        {
                            u4Start = _arPool[eBehindPool].u4Addr;
                            u4Start -= _arPoolList[u4ListIdx].u4PoolSize;
                            u4Start -= _arPoolList[u4ListIdx].u4AddrAlign;
                            u4Start -= FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eBehindPool);
                    }
                    else if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_BETWEEN)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        FBM_POOL_TYPE_T eBehindPool;

                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;
                        eBehindPool = arPoolArrangeInfo[u4ListIdx].ePool2;

                        // check 'ahead pool' & 'behind pool' ready or not

                        if ((eAheadPool == FBM_POOL_TYPE_NS) &&
                                (eBehindPool < FBM_POOL_TYPE_NS) &&
                                (arPoolArrangeInfo[eBehindPool].u1Ready != 0))
                        {
                            // share between u4FbmStartAddr

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4FbmStartAddr, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            if (_arPool[eBehindPool].u4Addr > _arPool[u4ListIdx].u4Addr)
                            {
                                _arPool[u4ListIdx].u4Size = _arPool[eBehindPool].u4Addr - _arPool[u4ListIdx].u4Addr;
                            }
                            else
                            {
                                _arPool[u4ListIdx].u4Size = 0;
                            }

                            // Size Alignment
                            _arPool[u4ListIdx].u4Size = (_arPool[u4ListIdx].u4Size & (~(_arPoolList[u4ListIdx].u4SizeAlign)));

                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
                        else if ((eAheadPool < FBM_POOL_TYPE_NS) &&
                                 (eBehindPool < FBM_POOL_TYPE_NS) &&
                                 (arPoolArrangeInfo[eAheadPool].u1Ready != 0) &&
                                 (arPoolArrangeInfo[eBehindPool].u1Ready != 0))
                        {
                            u4Start = _arPool[eAheadPool].u4Addr;
                            u4Start += _arPool[eAheadPool].u4Size;
                            u4Start += FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u1Ready = 1;

                            if (_arPool[eBehindPool].u4Addr > _arPool[u4ListIdx].u4Addr)
                            {
                                _arPool[u4ListIdx].u4Size = _arPool[eBehindPool].u4Addr - _arPool[u4ListIdx].u4Addr;
                            }
                            else
                            {
                                _arPool[u4ListIdx].u4Size = 0;
                            }

                            // Size Alignment
                            _arPool[u4ListIdx].u4Size = (_arPool[u4ListIdx].u4Size & (~(_arPoolList[u4ListIdx].u4SizeAlign)));

                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eBehindPool);
                        ASSERT(eAheadPool != eBehindPool);
                    }
                    else
                    {
                        ASSERT(0);
                    }

                    _arPool[u4ListIdx].u4Mode |= FBM_POOL_MODE_SHARE;
                }
            }

            // check all non-inherit pool ready
            u4Ready = 1;
            for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
            {
                if ((_arPoolList[u4ListIdx].u4Inherit != 0) &&
                        (arPoolArrangeInfo[u4ListIdx].u1Ready == 0))
                {
                    u4Ready = 0;
                }
            }
        } while (u4Ready == 0);

        // check DSP 16MB Boundary Issue
        if (_arPool[FBM_POOL_TYPE_DSP].u4Size != 0)
        {
            u4Start = (_arPool[FBM_POOL_TYPE_DSP].u4Addr & 0xFF000000);
            u4End = ((_arPool[FBM_POOL_TYPE_DSP].u4Addr + _arPool[FBM_POOL_TYPE_DSP].u4Size) & 0xFF000000);

            if (u4Start != u4End)
            {
                VERIFY(u4Start == u4End);
            }
        }

        u4Size = _arPool[FBM_POOL_TYPE_DSP].u4Addr + _arPool[FBM_POOL_TYPE_DSP].u4Size - u4FbmStartAddr;
        if (u4ReservedSize < u4Size)
        {
            Printf("FBM size is not enough after allocate, reserved=0x%x, required=0x%x\n", u4ReservedSize, u4Size);
            VERIFY(0);
        }

        // handle null pool
        for (u4PoolTpIdx = 0; u4PoolTpIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolTpIdx++)
        {
            if (_arPool[u4PoolTpIdx].u4Addr == (UINT32)NULL)
            {
                _arPool[u4PoolTpIdx].u4Addr = FBM_ALIGN_MASK((UINT32)(u4FbmStartAddr),
                                              _arPoolList[u4PoolTpIdx].u4AddrAlign);
            }
        }
    }

    // handle MPEG pool
    if (_arPool[FBM_POOL_TYPE_MPEG].u4Addr != (UINT32)NULL)
    {
        _arFbpConf[0].u4Addr = FBM_ALIGN_MASK(_arPool[FBM_POOL_TYPE_MPEG].u4Addr, FBM_FMG_Y_ALIGMENT);

#ifdef CC_FBM_TWO_FBP
        _arFbpConf[1].u4Addr = FBM_ALIGN_MASK(_arPool[FBM_POOL_TYPE_MPEG].u4Addr + (FBM_FBG_TYPE_1080HD_H264_SIZE>>1), FBM_FMG_Y_ALIGMENT);
#endif
#ifdef CC_FBM_INT_TWO_FBP
        _arFbpConf[1].u4Addr = FBM_ALIGN_MASK(_arPool[FBM_POOL_TYPE_MPEG].u4Addr + FBM_FBG_TYPE_1080HD_H264_SIZE, FBM_FMG_Y_ALIGMENT);
#endif
#if defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB) || defined(CC_VOMX_TV_COEXIST)
        _arFbpConf[1].u4Addr = FBM_ALIGN_MASK(_arPool[FBM_POOL_TYPE_MPEG2].u4Addr, FBM_FMG_Y_ALIGMENT);
#endif

#if 0 //refine mpeg memory layout
        // Set Address to _arFbpConf
        for (u4PoolIdx = 0; u4PoolIdx < _ucFbpConfNs; u4PoolIdx++)
        {
            for (u4ListIdx = 0; u4ListIdx < _ucFbpListNs; u4ListIdx++)
            {
                if (_arFbpConf[u4PoolIdx].ucType == _arFbpList[u4ListIdx].ucType)
                {
                    if (_arFbpConf[u4PoolIdx].u4Addr == NULL)
                    {
                        _arFbpConf[u4PoolIdx].u4Addr = _arFbpConf[u4LastPoolIdx].u4Addr + u4LastPoolSize;
                    }
                    u4LastPoolSize = _arFbpList[u4ListIdx].u4Size;
                    u4LastPoolIdx = u4PoolIdx;
                }
            }
        }
#endif

        for (u4PoolIdx = 0; u4PoolIdx < _ucFbpConfNs; u4PoolIdx++)
        {
            LOG(7, "FBP(%d) %d 0x%08x\n", u4PoolIdx, _arFbpConf[u4PoolIdx].ucType, _arFbpConf[u4PoolIdx].u4Addr);
        }

        // Configure frame buffer pool (Type, Addr, Status, ...)
        _ucFbpNs = _ucFbpConfNs;

        for (u4PoolIdx = 0; u4PoolIdx < _ucFbpConfNs; u4PoolIdx++)
        {
            _prFbp[u4PoolIdx] = (FBP_T *)x_mem_alloc(sizeof(FBP_T));
            if (_prFbp[u4PoolIdx] != NULL)
            {
                _prFbp[u4PoolIdx]->ucType = _arFbpConf[u4PoolIdx].ucType;
                _prFbp[u4PoolIdx]->u4Addr = _arFbpConf[u4PoolIdx].u4Addr;
                _prFbp[u4PoolIdx]->ucStatus = FBP_STATUS_EMPTY;
                _prFbp[u4PoolIdx]->ucInheritance = FBP_MAX_NS;
                FBM_MEMSET((VOID *)_prFbp[u4PoolIdx]->aucOccupyList, FBP_MAX_NS, FBP_MAX_DISASSEMBLY);
            }
			
        }

        // Configure MPEG frame buffer pool (Size)
        for (u4PoolIdx = 0; u4PoolIdx < _ucFbpNs; u4PoolIdx++)
        {
            for (u4ListIdx = 0; u4ListIdx < _ucFbpListNs; u4ListIdx++)
            {
                if (_prFbp[u4PoolIdx]->ucType == _arFbpList[u4ListIdx].ucType)
                {
                    _prFbp[u4PoolIdx]->u4Size = _arFbpList[u4ListIdx].u4Size;
                }
            }
        }
    }
#ifndef CC_MEM_TOOL
//#ifndef __KERNEL__
#if !defined(CC_FBM_2ND_CHANNEL) && !defined(__KERNEL__)
    // Pad the interval between pools
    for (u4PoolTpIdx = 0; u4PoolTpIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolTpIdx++)
    {
        UINT32 u4PadAddr;
        UINT32 u4PadLen;

        if ((_arPool[u4PoolTpIdx].u4Inherit == FBM_POOL_ROOT) &&
                (_arPool[u4PoolTpIdx].u4Addr > u4FbmStartAddr))
        {
            u4PadAddr = _arPool[u4PoolTpIdx].u4Addr - FBM_POOL_PADDING_SIZE;
            u4PadLen = FBM_POOL_PADDING_SIZE;

            FBM_MEMSET((VOID*)u4PadAddr, FBM_POOL_PADDING, u4PadLen);
        }
    }
#endif
#endif
    return (_arPool);
}


//-------------------------------------------------------------------------
/** _FBM_GetPoolList
 *  Get pool list
 *  @param  pprFbpList   FBP pool list point to pointer.
 *  @param  pucFbpNs   FBP pool list pointer number.
 *  @return NONE.
 */
//-------------------------------------------------------------------------
VOID _FBM_GetPoolList(FBP_LIST_T** pprFbpList, UCHAR* pucFbpNs)
{
    ASSERT(pprFbpList != NULL);
    ASSERT(pucFbpNs != NULL);

    *pucFbpNs = _ucFbpListNs;
    *pprFbpList = _arFbpList;
}


//-------------------------------------------------------------------------
/** _FBM_PoolAllocate
 *  Allocate FBM pool
 *  @param  ucFbgType   Pool type.
 *  @param  pu4FbMemoryPool   Pool address pointer.
 *  @return Pool size.
 *          If fail, return 0.
 */
//-------------------------------------------------------------------------
UINT32 _FBM_PoolAllocate(UCHAR ucFbgType, UINT32* pu4FbMemoryPool)
{
    UINT32 u4PoolIdx;
    UINT32 u4TargetPoolIdx;
    UINT32 ucTargetPoolSize;
    UINT32 u4RequestPoolSize;

    if (pu4FbMemoryPool == NULL)
    {
        return 0;
    }

    // search pool list for empty pool
    for (u4PoolIdx = 0; u4PoolIdx < FBP_MAX_NS; u4PoolIdx++)
    {
        if ((_prFbp[u4PoolIdx] != NULL) &&
                (_prFbp[u4PoolIdx]->ucType == ucFbgType) &&
                (_prFbp[u4PoolIdx]->ucStatus == FBP_STATUS_EMPTY))
        {
            _prFbp[u4PoolIdx]->ucStatus = FBP_STATUS_USED;
            *pu4FbMemoryPool = _prFbp[u4PoolIdx]->u4Addr;

            _EnableMpegPoolMemoryIntrusionDetection(_prFbp[u4PoolIdx]->u4Addr, _prFbp[u4PoolIdx]->u4Size);

            return _prFbp[u4PoolIdx]->u4Size;
        }
    }

    FBM_POOL_MUTEX_LOCK();

    // check reqeust pool size
    u4RequestPoolSize = 0;
    for (u4PoolIdx = 0; u4PoolIdx < _ucFbpListNs; u4PoolIdx++)
    {
        if (ucFbgType == _arFbpList[u4PoolIdx].ucType)
        {
            u4RequestPoolSize = _arFbpList[u4PoolIdx].u4Size;
            u4PoolIdx = _ucFbpListNs;
        }
    }

    FBM_POOL_MUTEX_UNLOCK();

    if (u4RequestPoolSize == 0)
    {
        return 0;
    }

    // search for pool to disassemble, smallest
    u4TargetPoolIdx = FBP_MAX_NS;
    ucTargetPoolSize = ~0;
    for (u4PoolIdx = 0; u4PoolIdx < FBP_MAX_NS; u4PoolIdx++)
    {
        if ((_prFbp[u4PoolIdx] != NULL) &&
                (_prFbp[u4PoolIdx]->ucStatus == FBP_STATUS_EMPTY) &&
                (_prFbp[u4PoolIdx]->u4Size >= u4RequestPoolSize))
        {
            if (_prFbp[u4PoolIdx]->u4Size < ucTargetPoolSize)
            {
                ucTargetPoolSize = _prFbp[u4PoolIdx]->u4Size;
                u4TargetPoolIdx = u4PoolIdx;
            }
        }
    }

    if (u4TargetPoolIdx != FBP_MAX_NS)
    {
        _FbgPoolDisassemble(u4TargetPoolIdx);
        return _FBM_PoolAllocate(ucFbgType, pu4FbMemoryPool);
    }

    return 0;
}


//-------------------------------------------------------------------------
/** _FBM_PoolAllocate
 *  Release FBM pool
 *  @param  u4FbMemoryPool   Pool address.
 *  @return NONE
 */
//-------------------------------------------------------------------------
VOID _FBM_PoolRelease(UINT32 u4FbMemoryPool)
{
    UINT32 u4PoolIdx;

    // search pool list for target pool
    for (u4PoolIdx = 0; u4PoolIdx < FBP_MAX_NS; u4PoolIdx++)
    {
        if ((_prFbp[u4PoolIdx] != NULL) &&
                (_prFbp[u4PoolIdx]->u4Addr == u4FbMemoryPool) &&
                (_prFbp[u4PoolIdx]->ucStatus == FBP_STATUS_USED))
        {
            _prFbp[u4PoolIdx]->ucStatus = FBP_STATUS_EMPTY;
            u4PoolIdx = FBP_MAX_NS;
        }
    }

    // assemble pool
    while (_FbgPoolAssemble() != FBP_MAX_NS)
        {}

    _DisableMpegPoolMemoryIntrusionDetection();
}


//-------------------------------------------------------------------------
/** _FBM_PoolAllocate
 *  Print FBM pool status
 *  @return NONE
 */
//-------------------------------------------------------------------------
VOID _FBM_PoolStatus(VOID)
{
    UCHAR ucIdx;

    LOG(1, "\n");
    LOG(1, "Sum of Root: 0x%x\n", _u4SumOfRoot);

//    FBM_PrintPoolInfo(&_arPool, &_arPoolList);
    FBM_PrintPoolInfo(_arPool, _arPoolList);

    for (ucIdx = 0; ucIdx < FBP_MAX_NS; ucIdx++)
    {
        if (_prFbp[ucIdx] != NULL)
        {
            LOG(1, "MPEG Pool(%d) T(%d) S(%d) Addr(0x%08x, 0x%08x)",
                ucIdx,
                _prFbp[ucIdx]->ucType,
                _prFbp[ucIdx]->ucStatus,
                _prFbp[ucIdx]->u4Addr,
                _prFbp[ucIdx]->u4Size);

            if (_prFbp[ucIdx]->ucInheritance != FBP_MAX_NS)
            {
                LOG(1, " Inherit(%d)", _prFbp[ucIdx]->ucInheritance);
            }

            if (_prFbp[ucIdx]->ucStatus == FBP_STATUS_OCCUPY)
            {
                LOG(1, " Occupy(%d,%d,%d,%d)\n",
                    _prFbp[ucIdx]->aucOccupyList[0],
                    _prFbp[ucIdx]->aucOccupyList[1],
                    _prFbp[ucIdx]->aucOccupyList[2],
                    _prFbp[ucIdx]->aucOccupyList[3]);
            }
            else
            {
                LOG(1, "\n");
            }
        }
    }
    SRM_QueryStatus();
}

/**
 * FBM SRM control call back
 *
 * @param VOID
 * @return VOID
 */
#ifdef CC_SRM_ON
static VOID _FBM_SrmCtrlCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UINT32 u4Idx;
    // cyj.porting
#if 0
    if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392F_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391F_FHD60_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392H_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391H_FHD60_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_64MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360_VERF_64MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360x64MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_128MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_64MB) ||
            (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360B_VERF_64MB))
#endif
    {
        static UINT32 u4CurrentMpegMode = FBM_POOL_MODE_MPEG_NO_COMP;

        LOG(3, "[SRM] MPEG CTRL vdp%d 0x%x %d %d\n", u4Arg1, u4Arg2, u4Arg3, u4Arg4);

        if ((u4CurrentMpegMode &  FBM_POOL_MODE_MPEG_NO_COMP) !=
                (u4Arg2 & FBM_POOL_MODE_MPEG_NO_COMP))
        {
            UINT32 u4Notify = 0;
            UINT32 u4PoolIdx;

            u4CurrentMpegMode = u4Arg2;

            for (u4PoolIdx = 0; u4PoolIdx < FBP_MAX_NS; u4PoolIdx++)
            {
                if ((_prFbp[u4PoolIdx] != NULL) &&
                        (_prFbp[u4PoolIdx]->ucStatus == FBP_STATUS_USED) &&
                        (_prFbp[u4PoolIdx]->ucType == FBM_FBG_TYPE_1080HD))
                {
                    u4Notify = 1;
                }
            }

            if (u4Notify != 0)
            {
                // Mode Change, Notify MPEG to reallocate FBM
                for (u4Idx = 0; u4Idx < FBG_MAX_NS; u4Idx++)
                {
                    FBM_SetFrameBufferFlag(u4Idx, FBM_FLAG_RELEASE_FBG);
                }
            }
        }

        FBM_POOL_MUTEX_LOCK();

        if (u4Arg2 & FBM_POOL_MODE_MPEG_NO_COMP)
        {
            // cyj.porting
#if 0
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392B_EU_128MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392H_128MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391H_FHD60_128MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391_VERF) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_128MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_128MB))
            {
                _arFbpList = _arFbpH264List;
                _ucFbpListNs = _ucFbpH264ListNs;
            }
            else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392B_US_128MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5392F_128MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391F_FHD60_128MB))
            {
                _arFbpList = _arFbpCqamDbkList;
                _ucFbpListNs = _ucFbpCqamDbkListNs;
            }
            else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360_VERF_64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360x64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360B_VERF_64MB))
#endif
            {
                _arFbpList = _arFbpH264List64M;
                _ucFbpListNs = _ucFbpH264List64MNs;
            }
        }
        else if (u4Arg2 & FBM_POOL_MODE_MPEG_COMP_50)
        {
            // cyj.porting
#if 0
            if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392H_128MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391H_FHD60_128MB) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391_VERF) ||
                    (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_128MB))
            {
                _arFbpList = _arFbpH264PipList;
                _ucFbpListNs = _ucFbpH264PipListNs;
            }
            else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5392F_128MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5391F_FHD60_128MB))
            {
                _arFbpList = _arFbpMPEG2PipList;
                _ucFbpListNs = _ucFbpMPEG2PipListNs;
            }
            else if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5362_VERF_64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360_VERF_64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360x64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5362B_VERF_64MB) ||
                     (SRMFBM_GetConf() == FBM_MEM_CFG_MT5360B_VERF_64MB))
#endif
            {
                _arFbpList = _arFbpH264PipList64M;
                _ucFbpListNs = _ucFbpH264PipList64MNs;

            }
        }
        else
        {
            //ASSERT(0);
        }
        FBM_POOL_MUTEX_UNLOCK();
    }
}
#endif

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** FBM_SelectGroupType
 *  Choice FBM Type according to size
 *  @param  u4Width   width.
 *  @param  u4Height   height.
 *  @return FBG type.
 */
//-------------------------------------------------------------------------
UCHAR FBM_SelectGroupType(UINT32 u4Width, UINT32 u4Height)
{
    // 1) select the smallest width/height pool type fit required width/height
    // 2) select the biggest pool type

    UINT32 u4Idx;
    UINT32 u4BiggestEmptyPoolSize;

    UINT32 u4TargetWidth;
    UINT32 u4TargetHeight;
    UCHAR ucTargetType;

#ifdef CC_RESIZE_SMALL_IMAGE
    {
       UINT32 u4Align=0;
	   
	   u4Align=(u4Width+31)&~31;
       if(u4Width<FBM_IMG_RESIZE_LIMITATION)
       {
          u4Width=MAX(u4Align,FBM_IMG_RESIZE_LIMITATION)*2;
       }

	   u4Align=(u4Height+31)&~31;
       if(u4Height<FBM_IMG_RESIZE_LIMITATION)
       {
          u4Height=MAX(u4Align,FBM_IMG_RESIZE_LIMITATION)*2;
       }
    }
#endif

    // Check the biggest pool size
    u4BiggestEmptyPoolSize = 0;
    for (u4Idx = 0; u4Idx < FBP_MAX_NS; u4Idx++)
    {
        if (_prFbp[u4Idx] != NULL)
        {
            if (_prFbp[u4Idx]->u4Size > u4BiggestEmptyPoolSize)
            {
                u4BiggestEmptyPoolSize = _prFbp[u4Idx]->u4Size;
            }
        }
    }

    // Check pool type according to width/height & pool size smaller than biggest empty pool size
    ucTargetType = FBM_FBG_TYPE_UNKNOWN;
    u4TargetWidth = 0xFFFFFFFF;
    u4TargetHeight = 0xFFFFFFFF;

    FBM_POOL_MUTEX_LOCK();
    for (u4Idx = 0; u4Idx < _ucFbpListNs; u4Idx++)
    {
        if (((_arFbpList[u4Idx].u4Width >= u4Width) &&
                (_arFbpList[u4Idx].u4Height >= u4Height) &&
                (_arFbpList[u4Idx].u4Width <= u4TargetWidth) &&
                (_arFbpList[u4Idx].u4Height <= u4TargetHeight) &&
                (_arFbpList[u4Idx].u4Size <= u4BiggestEmptyPoolSize))
                || _arFbpList[u4Idx].u4Width * _arFbpList[u4Idx].u4Height >=
                FBM_ALIGN_MASK(u4Width, FBM_B2R_H_PITCH) *
                FBM_ALIGN_MASK(u4Height, FBM_MPV_V_ALIGMENT))
        {
            ucTargetType = _arFbpList[u4Idx].ucType;

            u4TargetWidth = _arFbpList[u4Idx].u4Width;
            u4TargetHeight = _arFbpList[u4Idx].u4Height;
        }
    }
    FBM_POOL_MUTEX_UNLOCK();

    LOG(7, "Select(%d, %d) Type(%d) Big(0x%x)\n", u4Width, u4Height, ucTargetType, u4BiggestEmptyPoolSize);

    return ucTargetType;
}

UCHAR FBM_ChangePoolInfo(UCHAR ucPoolType)
{
    switch(ucPoolType)
    {
	 case FBM_POOL_TYPE_SCPOS_MAIN:
	if(SRM_GetCurrFBMMode()==FBM_MMMODE)
		{
		ucPoolType = FBM_POOL_TYPE_PQ_MM;
		}
	else if(SRM_GetCurrFBMMode()==FBM_NETMODE)
		{
		ucPoolType = FBM_POOL_TYPE_PQ_NET;
		}
	else if(SRM_GetCurrFBMMode()==FBM_TVMODE)
		{
		ucPoolType = FBM_POOL_TYPE_PQ_TV;
    	}
     break;
	 case FBM_POOL_TYPE_FEEDER:
	if(SRM_GetCurrFBMMode()==FBM_MMMODE)
		{
		ucPoolType = FBM_POOL_TYPE_FEEDER;
		}
	else if(SRM_GetCurrFBMMode()==FBM_NETMODE)
		{
		ucPoolType = FBM_POOL_TYPE_FEEDER_NET;
		}
     break;
	default:
	 break;
    }
    return ucPoolType;
}
//-------------------------------------------------------------------------
/** FBM_GetPoolInfo
 *  Get Pool Info
 *  @param  ucPoolType
 *  @return NONE.
 */
//-------------------------------------------------------------------------

FBM_POOL_T* FBM_GetPoolInfo(UCHAR ucPoolType)
{
#ifndef CC_MEM_TOOL
    if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_POOL_CHANGE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_POOL_CHANGE]))
    {
        ((FBM_CB_POOL_CHANGE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_POOL_CHANGE])((FBM_POOL_TYPE_T)ucPoolType);
    }
    else
    {
        ASSERT(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_POOL_CHANGE] == 0);
    }
#endif
#if defined(CC_MT5389)
if(SRMFBM_GetConf() == FBM_MEM_CFG_MT5501_3DNET)
{
    ucPoolType = FBM_ChangePoolInfo(ucPoolType);
}
#endif
    return (&_arPool[ucPoolType]);
}

//-------------------------------------------------------------------------
/** FBM_GetPoolMpeg
 *  Get MPEG Pool Info
 *  @param NONE
 *  @return NONE.
 */
//-------------------------------------------------------------------------

FBM_POOL_T* FBM_GetPoolMpeg(VOID)
{
    UCHAR ucIdx;

    for (ucIdx = 0; ucIdx < FBP_MAX_NS; ucIdx++)
    {
        if (_prFbp[ucIdx] != NULL)
        {
            if (_prFbp[ucIdx]->ucStatus == FBP_STATUS_USED)
            {
                _rCurrentMpeg = _arPool[(UCHAR)FBM_POOL_TYPE_MPEG];

                _rCurrentMpeg.u4Addr = _prFbp[ucIdx]->u4Addr;
                _rCurrentMpeg.u4Size = _prFbp[ucIdx]->u4Size;

                return &_rCurrentMpeg;
            }
        }
    }

    return FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_MPEG);
}

CHAR* FBM_GetPoolName(UCHAR ucPoolType)
{
    return _arPoolList[ucPoolType].szName;
}

UINT32 FBM_GetPoolSize(UCHAR ucPoolType)
{
    return _arPoolList[ucPoolType].u4PoolSize;
}


