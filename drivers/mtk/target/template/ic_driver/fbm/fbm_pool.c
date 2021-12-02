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
 * $RCSfile: fbm_pool.c,v $
 * $Revision: #1 $
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
#ifdef CC_SCPOS_EN
#include "source_table.h"
#endif
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

// Frame Buffer Pool arrangement type
typedef enum
{
    FBM_POOL_ARRANGE_AFTER,             // pool arrange after ePool1, need to specify pool size
    FBM_POOL_ARRANGE_BEFORE,            // pool arrange before ePool1, need to specify pool size
    FBM_POOL_ARRANGE_BETWEEN,           // pool arrange after ePool1 & before ePool2, no need to specify pool size
    FBM_POOL_ARRANGE_SHARE,             // pool arrange from ePool1 start address plus offset, need to specify pool size
    FBM_POOL_ARRANGE_SHARE_ALL          // no need to specify pool size (1) pool arrange form ePool 1 start address plus offset to end of ePool 1, (2) pool arrange form ePool 2 start address to end of ePool 1
} POOL_ARRANGE_TYPE_T;

// Frame Buffer Pool arrangement info
typedef struct
{
    POOL_ARRANGE_TYPE_T eMode;
    FBM_POOL_TYPE_T ePool1;
    FBM_POOL_TYPE_T ePool2;
    UINT32 u4Offset;
    UINT32 u4Ready;
} POOL_ARRANGE_INFO_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define FBM_MEMSET(addr, value, len)                    \
    (VERIFY(x_memset((addr), value, len) == (addr)))


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
static void _FBM_SrmCtrlCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
#endif

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

// Frame Buffer Memory Pool Type List
static POOL_LIST_T _arPoolList[] =
{
    //MPEG
    {
        "MPEG",
        FBM_POOL_TYPE_MPEG,
        FBM_MPEG_ADDR_ALIGMENT,   0,
        0,
        0,
        1920, 1080, 0
    },

    //MDDi (Main)
    {
        "MDDI-M",
        FBM_POOL_TYPE_MDDI_MAIN,
        FBM_MDDI_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 0
    },

    //SCPOS (Main)
    {
        "SCPOS-M",
        FBM_POOL_TYPE_SCPOS_MAIN,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 0
    },

    //NR
    {
        "NR",
        FBM_POOL_TYPE_NR,
        FBM_NR_ADDR_ALIGMENT, 0,
        0,
        FBM_NR_POOL_SIZE,
        1920, 1080, 0
    },

    //TDC
    {
        "TDC",
        FBM_POOL_TYPE_TDC,
        FBM_TDC_ADDR_ALIGMENT,  0,
        0,
        FBM_TDC_POOL_SIZE,
        720, 576, 0
    },

    //VBI
    {
        "VBI",
        FBM_POOL_TYPE_VBI,
        FBM_VBI_ADDR_ALIGMENT,  0,
        0,
        FBM_VBI_POOL_SIZE,
        0, 0, 0
    },

    //DMX
    {
        "DMX",
        FBM_POOL_TYPE_DMX,
        FBM_DMX_ADDR_ALIGMENT,   FBM_DMX_SIZE_ALIGMENT,
        0,
        FBM_DMX_POOL_SIZE,
        0, 0, 0
    },

    //MDDi (Pip)
    {
        "MDDI-P",
        FBM_POOL_TYPE_MDDI_PIP,
        FBM_MDDI_ADDR_ALIGMENT,  0,
        0,
        0,
        0, 0, 1
    },

    //SCPOS (Pip)
    {
        "SCPOS-P",
        FBM_POOL_TYPE_SCPOS_PIP,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //SCPOS (VGA)
    {
        "SCPOS-V",
        FBM_POOL_TYPE_SCPOS_VGA,
        FBM_SCPOS_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //DSP
    {
        "DSP",
        FBM_POOL_TYPE_DSP,
        FBM_DSP_ADDR_ALIGMENT,  0,
        0,
        FBM_DSP_POOL_SIZE,
        0, 0, 0
    },

    //GFXSC
    {
        "GFXSC",
        FBM_POOL_TYPE_GFXSC,
        FBM_GFXSC_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //B2R
    {
        "B2R",
        FBM_POOL_TYPE_B2R,
        FBM_B2R_ADDR_ALIGMENT, 0,
        0,
        0,
        1920, 1088, 1
    },

    //JPEG
    {
        "JPEG",
        FBM_POOL_TYPE_JPEG,
        FBM_JPEG_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //OSD
    {
        "JPEG-OSD1",
        FBM_POOL_TYPE_JPEG_OSD1,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //OSD
    {
        "JPEG-OSD2",
        FBM_POOL_TYPE_JPEG_OSD2,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //OSD
    {
        "JPEG-OSD3",
        FBM_POOL_TYPE_JPEG_OSD3,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //OSD
    {
        "JPEG-OSD4",
        FBM_POOL_TYPE_JPEG_OSD4,
        FBM_OSD_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //Feeder
    {
        "Feeder",
        FBM_POOL_TYPE_FEEDER,
        FBM_FEEDER_ADDR_ALIGMENT, 0,
        0,
        0,
        0, 0, 1
    },

    //RSV1
    {
        "RSV1",
        FBM_POOL_TYPE_RSV_01,
        0, 0,
        0,
        0,
        0, 0, 1
    },

    //RSV2
    {
        "RSV2",
        FBM_POOL_TYPE_RSV_02,
        0, 0,
        0,
        0,
        0, 0, 1
    },

    //RSV3
    {
        "RSV3",
        FBM_POOL_TYPE_RSV_03,
        0, 0,
        0,
        0,
        0, 0, 1
    },

    //RSV4
    {
        "RSV4",
        FBM_POOL_TYPE_RSV_04,
        0, 0,
        0,
        0,
        0, 0, 1
    },

    //TOTAL
    {
        "TOTAL",
        FBM_POOL_TYPE_TOTAL,
        0, 0,
        0,
        0,
        0, 0, 1
    },
};

static const UINT32 _u4PoolListNs = (UINT32)FBM_POOL_TYPE_NS;

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

static const UCHAR _ucFbpCqamListNs = 3;

#if 0
// MPEG DBK Frame Buffer Group Type List, size of _arFbpDbkList[n] > size of _arFbpDbkList[n + 1]
static FBP_LIST_T _arFbpCqamDbkList[] =
{
    {
        FBM_FBG_TYPE_1080HD_DBK,
        FBM_FBG_TYPE_1080HD_DBK_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_720HD_DBK,
        FBM_FBG_TYPE_720HD_DBK_SIZE,
        FBM_FBG_TYPE_720HD_Y_SIZE,
        FBM_FBG_TYPE_720HD_C_SIZE,
        FBM_FBG_720HD_H,
        FBM_FBG_720HD_V
    },

    {
        FBM_FBG_TYPE_PAL_DBK,
        FBM_FBG_TYPE_PAL_DBK_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    }
};

static const UCHAR _ucFbpCqamDbkListNs = 3;

#else

//mt539x Frame Buffer Group Type List, size of _arFbpList[n] > size of _arFbpList[n + 1]
static FBP_LIST_T _arFbpCqamDbkList[] =
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

static const UCHAR _ucFbpCqamDbkListNs = 3;
#endif

#ifdef CC_SRM_ON
static const UCHAR _ucFbp5382pPipListNs = 3;

// MPEG pool type at Pip Path for 5382p
static FBP_LIST_T _arFbp5382pPipList[] =
{
    {
        FBM_FBG_TYPE_1080HD_2_FB,
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

static FBP_LIST_T _arFbpCqam16MbList[] =
{
    {
        FBM_FBG_TYPE_1080HD_1_5FB,
        FBM_FBG_TYPE_1080HD_1_5FB_SIZE,
        FBM_FBG_TYPE_1080HD_Y_SIZE,
        FBM_FBG_TYPE_1080HD_C_SIZE,
        FBM_FBG_1080HD_H,
        FBM_FBG_1080HD_V
    },

    {
        FBM_FBG_TYPE_720HD_3FB,
        FBM_FBG_TYPE_720HD_3FB_SIZE,
        FBM_FBG_TYPE_720HD_Y_SIZE,
        FBM_FBG_TYPE_720HD_C_SIZE,
        FBM_FBG_720HD_H,
        FBM_FBG_720HD_V
    },

    {
        FBM_FBG_TYPE_PAL_3FB,
        FBM_FBG_TYPE_PAL_3FB_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    },

    {
        FBM_FBG_TYPE_NTSC,
        FBM_FBG_TYPE_NTSC_SIZE,
        FBM_FBG_TYPE_NTSC_Y_SIZE,
        FBM_FBG_TYPE_NTSC_C_SIZE,
        FBM_FBG_NTSC_H,
        FBM_FBG_NTSC_V
    },
};

static const UCHAR _ucFbpCqam16MbListNs = 4;

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

static const UCHAR _ucFbpDvbtDbkListNs = 1;

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

// MPEG DVBT PAL (Racing)
static FBP_LIST_T _arFbpDvbtRacingList[] =
{
    {
        FBM_FBG_TYPE_PAL_3FB,
        FBM_FBG_TYPE_PAL_3FB_SIZE,
        FBM_FBG_TYPE_PAL_Y_SIZE,
        FBM_FBG_TYPE_PAL_C_SIZE,
        FBM_FBG_PAL_H,
        FBM_FBG_PAL_V
    },
};

static const UCHAR _ucFbpDvbtRacingListNs = 1;

static FBP_LIST_T* _arFbpList = NULL;

static UCHAR _ucFbpListNs = 0;

// Frame Buffer Pool Configuration
// 1 HD System
static FBP_CONF_T _arFbpConf[] =
{
    {FBM_FBG_TYPE_1080HD,   NULL},
};

static UCHAR _ucFbpConfNs = 1;

// frame buffer pool structure
static FBP_T *_prFbp[FBP_MAX_NS];

// frame buffer pool number
static UCHAR _ucFbpNs = 0;

static FBM_POOL_T _arPool[FBM_POOL_TYPE_NS];

//  frame buffer pool memory address
static UINT32 u4FbMallocAddr = NULL;

// FBM call back function
static FBM_CB_FUNC_T* _prFbmCbFunc = NULL;

// Current Frame Buffer Pool
static FBM_POOL_TYPE_T _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_MAIN;
static FBM_POOL_TYPE_T _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_MAIN;
static FBM_POOL_T _rCurrentMpeg;

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
static void _EnableMpegPoolMemoryIntrusionDetection(UINT32 u4Addr, UINT32 u4Size)
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
static void _DisableMpegPoolMemoryIntrusionDetection(void)
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
static void _FbgPoolDisassemble(UCHAR ucDisassembleIdx)
{
    UINT32 u4PoolIdx;
    UINT32 u4TargetIdx;
    UINT32 u4TargetType = FBM_FBG_TYPE_UNKNOWN;
    UINT32 u4TargetSize = 0;

    if ((_prFbp[ucDisassembleIdx] != NULL) &&
        (_prFbp[ucDisassembleIdx]->ucStatus == FBP_STATUS_EMPTY))
    {
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

                    FBM_MEMSET((void *)_prFbp[u4TargetIdx]->aucOccupyList, FBP_MAX_NS, FBP_MAX_DISASSEMBLY);

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
static UCHAR _FbgPoolAssemble(void)
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
                FBM_MEMSET((void *)_prFbp[u4Idx]->aucOccupyList, FBP_MAX_NS, FBP_MAX_DISASSEMBLY);

                return u4Idx;
            }
        }
    }

    return FBP_MAX_NS;
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _FBM_PoolInit
 *  Initialize this FBM pool
 *  @retval NONE
 */
//-------------------------------------------------------------------------
FBM_POOL_T* _FBM_PoolInit(void)
{
    UINT32 u4PoolIdx;
    UINT32 u4ListIdx;
    UINT32 u4MpegSize;
    UINT32 u4PoolTpIdx;

    UINT32 u4Size = 0;
    UINT32 u4LastPoolIdx = 0;
    UINT32 u4LastPoolSize = 0;

    UINT32 u4FbmStartAddr;

    POOL_ARRANGE_INFO_T arPoolArrangeInfo[FBM_POOL_TYPE_NS];

    _prFbmCbFunc = FBM_GetCbFuncTbl();

    u4FbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);

#ifdef CC_SRM_ON
    // Reg SRM Callback Function
    SRM_Init();
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
    } while (0);
#endif

    // double check
    for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
    {
        if (u4ListIdx != (UINT32)_arPoolList[u4ListIdx].eType)
        {
            ASSERT(0);
        }
    }

    if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5391x128MB)
    {
        _arFbpList = _arFbpCqamDbkList;
        _ucFbpListNs = _ucFbpCqamDbkListNs;

        _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD_DBK;
        _ucFbpConfNs = 1;
    }
    else
    {
        _arFbpList = _arFbpCqamList;
        _ucFbpListNs = _ucFbpCqamListNs;

        _arFbpConf[0].ucType = FBM_FBG_TYPE_1080HD;
        _ucFbpConfNs = 1;
    }

    // init pool arrange info
    for (u4ListIdx = 0; u4ListIdx < (UINT32) FBM_POOL_TYPE_NS; u4ListIdx++)
    {
        arPoolArrangeInfo[u4ListIdx].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[u4ListIdx].ePool1 = FBM_POOL_TYPE_TOTAL;
        arPoolArrangeInfo[u4ListIdx].ePool2 = FBM_POOL_TYPE_TOTAL;
        arPoolArrangeInfo[u4ListIdx].u4Offset = 0;
        arPoolArrangeInfo[u4ListIdx].u4Ready = 0;
    }

    if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5391x128MB)
    {
        // 3DEMUX | MPEG | MDDI-420 8BIT | SCPOS 8BIT | SCPOS-PIP | DSP
        arPoolArrangeInfo[FBM_POOL_TYPE_DMX].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_DMX].ePool1 = FBM_POOL_TYPE_TOTAL;

        _arPoolList[FBM_POOL_TYPE_MPEG].u4Mode = (FBM_POOL_MODE_MPEG_DBK | FBM_POOL_MODE_SHARE);
        _arPoolList[FBM_POOL_TYPE_MPEG].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_MPEG].u4Height = 1080;
        arPoolArrangeInfo[FBM_POOL_TYPE_MPEG].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_MPEG].ePool1 = FBM_POOL_TYPE_DMX;

        _arPoolList[FBM_POOL_TYPE_MDDI_MAIN].u4PoolSize = FBM_MDDI_420_FHDPANEL_POOL_SIZE;
        _arPoolList[FBM_POOL_TYPE_MDDI_MAIN].u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_420_FB;
        _arPoolList[FBM_POOL_TYPE_MDDI_MAIN].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_MDDI_MAIN].u4Height = 1080;
        arPoolArrangeInfo[FBM_POOL_TYPE_MDDI_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_MDDI_MAIN].ePool1 = FBM_POOL_TYPE_MPEG;

        _arPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4PoolSize = FBM_SCPOS_FHDPANEL_POOL_SIZE;
        _arPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_3FB;
        _arPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_SCPOS_MAIN].u4Height = 1080;
        arPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_MAIN].ePool1 = FBM_POOL_TYPE_MDDI_MAIN;

        _arPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Inherit = 0;
        _arPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4PoolSize = FBM_SCPOS_POOL_SIZE(FBM_SCPOS_SIZE_ALIGMENT, 720, 576);
        _arPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_3FB;
        _arPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Width = 720;
        _arPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4Height = 576;
        arPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_PIP].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

        arPoolArrangeInfo[FBM_POOL_TYPE_DSP].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_DSP].ePool1 = FBM_POOL_TYPE_SCPOS_PIP;

        // 3D Comb | VBI (Share DMX)
        _arPoolList[FBM_POOL_TYPE_TDC].u4Inherit = 1;
        _arPoolList[FBM_POOL_TYPE_TDC].u4Mode = FBM_POOL_MODE_10BIT;
        arPoolArrangeInfo[FBM_POOL_TYPE_TDC].eMode = FBM_POOL_ARRANGE_SHARE;
        arPoolArrangeInfo[FBM_POOL_TYPE_TDC].ePool1 = FBM_POOL_TYPE_DMX;

        _arPoolList[FBM_POOL_TYPE_VBI].u4Inherit = 1;
        arPoolArrangeInfo[FBM_POOL_TYPE_VBI].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_VBI].ePool1 = FBM_POOL_TYPE_TDC;

        // MDDI-422 10BIT | NR | RSV SCPOS 10BIT (Share MPEG Offset 576P)
        _arPoolList[FBM_POOL_TYPE_MDDI_PIP].u4PoolSize = FBM_MDDI_FHDPANEL_WA_10BIT_POOL_SIZE;
        _arPoolList[FBM_POOL_TYPE_MDDI_PIP].u4Mode = FBM_POOL_MODE_10BIT;
        _arPoolList[FBM_POOL_TYPE_MDDI_PIP].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_MDDI_PIP].u4Height = 1082;
        _arPoolList[FBM_POOL_TYPE_MDDI_PIP].u4Inherit = 1;
        arPoolArrangeInfo[FBM_POOL_TYPE_MDDI_PIP].eMode = FBM_POOL_ARRANGE_SHARE;
        arPoolArrangeInfo[FBM_POOL_TYPE_MDDI_PIP].ePool1 = FBM_POOL_TYPE_MPEG;
        arPoolArrangeInfo[FBM_POOL_TYPE_MDDI_PIP].u4Offset = FBM_FBG_TYPE_1080HD_2_FB_SIZE;
//        arPoolArrangeInfo[FBM_POOL_TYPE_MDDI_PIP].u4Offset = FBM_FBG_TYPE_PAL_DBK_SIZE;

        _arPoolList[FBM_POOL_TYPE_NR].u4Mode = FBM_POOL_MODE_10BIT;
        _arPoolList[FBM_POOL_TYPE_NR].u4Inherit = 1;
        arPoolArrangeInfo[FBM_POOL_TYPE_NR].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_NR].ePool1 = FBM_POOL_TYPE_MDDI_PIP;

        _arPoolList[FBM_POOL_TYPE_RSV_01].u4PoolSize = FBM_SCPOS_FHDPANEL_10BIT_POOL_SIZE;
        _arPoolList[FBM_POOL_TYPE_RSV_01].u4Mode = FBM_POOL_MODE_10BIT | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_3FB;
        _arPoolList[FBM_POOL_TYPE_RSV_01].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_RSV_01].u4Height = 1080;
        _arPoolList[FBM_POOL_TYPE_RSV_01].u4Inherit = 1;
        _arPoolList[FBM_POOL_TYPE_RSV_01].u4AddrAlign = _arPoolList[FBM_POOL_TYPE_SCPOS_PIP].u4AddrAlign;
        arPoolArrangeInfo[FBM_POOL_TYPE_RSV_01].eMode = FBM_POOL_ARRANGE_AFTER;
        arPoolArrangeInfo[FBM_POOL_TYPE_RSV_01].ePool1 = FBM_POOL_TYPE_NR;

        // SCPOS-VGA (Share MDDI)
        _arPoolList[FBM_POOL_TYPE_SCPOS_VGA].u4PoolSize = FBM_SCPOS_FHDPANEL_444_3FB_POOL_SIZE;
        _arPoolList[FBM_POOL_TYPE_SCPOS_VGA].u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_444_FB | FBM_POOL_MODE_3FB;
        _arPoolList[FBM_POOL_TYPE_SCPOS_VGA].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_SCPOS_VGA].u4Height = 1080;
        _arPoolList[FBM_POOL_TYPE_SCPOS_VGA].u4Inherit = 1;
        arPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_VGA].eMode = FBM_POOL_ARRANGE_SHARE;
        arPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_VGA].ePool1 = FBM_POOL_TYPE_MPEG;
        arPoolArrangeInfo[FBM_POOL_TYPE_SCPOS_VGA].u4Offset = FBM_FBG_TYPE_1080HD_2_FB_SIZE;

        // handle Image Viewer Pool
#ifdef CC_OSD_USE_FBM
        // JPEG | OSD1 | OSD2 | OSD3 | OSD4 (Share all except DSP)
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Inherit = 1;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1080, 1, 4);   // FHD, 32bpp, x1
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD4].u4Height = 1080;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].eMode = FBM_POOL_ARRANGE_BEFORE;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD4].ePool1 = FBM_POOL_TYPE_DSP;

        _arPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Inherit = 1;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1080, 1, 4);   // FHD, 32bpp, x1
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD3].u4Height = 1080;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].eMode = FBM_POOL_ARRANGE_BEFORE;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD3].ePool1 = FBM_POOL_TYPE_JPEG_OSD4;

        _arPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Inherit = 1;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1080, 1, 4);   // FHD, 32bpp, x1
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD2].u4Height = 1080;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].eMode = FBM_POOL_ARRANGE_BEFORE;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD2].ePool1 = FBM_POOL_TYPE_JPEG_OSD3;

        _arPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Inherit = 1;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4PoolSize = FBM_OSD_POOL_SIZE(1920, 1080, 1, 4);   // FHD, 32bpp, x1
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_JPEG_OSD1].u4Height = 1080;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].eMode = FBM_POOL_ARRANGE_BEFORE;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG_OSD1].ePool1 = FBM_POOL_TYPE_JPEG_OSD2;

        // JPEG (Between Start & OSD1)
        _arPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = 1;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BETWEEN;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_NS;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool2 = FBM_POOL_TYPE_JPEG_OSD1;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].u4Offset = 0;
#else
        // B2R (Before SCPOS-P)
        _arPoolList[FBM_POOL_TYPE_B2R].u4Inherit = 1;
        _arPoolList[FBM_POOL_TYPE_B2R].u4PoolSize = FBM_B2R_POOL_SIZE(1920, 1088, 3, 2);   // FHD, 422, x2
        _arPoolList[FBM_POOL_TYPE_B2R].u4Mode = FBM_POOL_MODE_422_FB | FBM_POOL_MODE_2FB;
        _arPoolList[FBM_POOL_TYPE_B2R].u4Width = 1920;
        _arPoolList[FBM_POOL_TYPE_B2R].u4Height = 1088;
        arPoolArrangeInfo[FBM_POOL_TYPE_B2R].eMode = FBM_POOL_ARRANGE_BEFORE;
        arPoolArrangeInfo[FBM_POOL_TYPE_B2R].ePool1 = FBM_POOL_TYPE_SCPOS_MAIN;

        // JPEG (Between Start & B2R)
        _arPoolList[FBM_POOL_TYPE_JPEG].u4Inherit = 1;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].eMode = FBM_POOL_ARRANGE_BETWEEN;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool1 = FBM_POOL_TYPE_VBI;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].ePool2 = FBM_POOL_TYPE_B2R;
        arPoolArrangeInfo[FBM_POOL_TYPE_JPEG].u4Offset = 0;
#endif
    }
    else
    {
        ASSERT(0);
    }

    // Calculate mpeg total frame buffer size
    u4MpegSize = 0;

    for (u4PoolIdx = 0; u4PoolIdx < _ucFbpConfNs; u4PoolIdx++)
    {
        for (u4ListIdx = 0; u4ListIdx < _ucFbpListNs; u4ListIdx++)
        {
            if (_arFbpConf[u4PoolIdx].ucType == _arFbpList[u4ListIdx].ucType)
            {
                u4MpegSize += _arFbpList[u4ListIdx].u4Size;
            }
        }
    }

    _arPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize = u4MpegSize;
    _arPoolList[FBM_POOL_TYPE_MPEG].u4PoolSize += FBM_MPEG_POOL_PAD;  // Protection

    if (u4FbMallocAddr == NULL)
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
            if ((_arPoolList[u4ListIdx].u4Inherit == 0) &&
                (_arPoolList[u4ListIdx].eType != FBM_POOL_TYPE_TOTAL))
            {
                u4Size += _arPoolList[u4ListIdx].u4PoolSize;
            }
        }

        u4ReservedSize = DRVCUST_OptGet(eFbmMemSize);

        if (u4ReservedSize < u4Size)
        {
            ASSERT(u4ReservedSize >= u4Size);
        }

        // handle total pool
        u4MaxAddrAlign = 0;
        for (u4PoolTpIdx = 0; u4PoolTpIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolTpIdx++)
        {
            if (_arPoolList[u4PoolTpIdx].u4AddrAlign > u4MaxAddrAlign)
            {
                u4MaxAddrAlign = _arPoolList[u4PoolTpIdx].u4AddrAlign;
            }
        }
        _arPool[FBM_POOL_TYPE_TOTAL].u4Addr = FBM_ALIGN_MASK((UINT32)(u4FbmStartAddr), u4MaxAddrAlign);
        _arPool[FBM_POOL_TYPE_TOTAL].u4Size = (UINT32)DRVCUST_OptGet(eFbmMemSize);
        arPoolArrangeInfo[FBM_POOL_TYPE_TOTAL].u4Ready = 1;

        // handle non-inherit pool
        do
        {
            for (u4ListIdx = 0; u4ListIdx < _u4PoolListNs; u4ListIdx++)
            {
                if ((_arPoolList[u4ListIdx].u4Inherit == 0) &&
                    (arPoolArrangeInfo[u4ListIdx].u4Ready == 0))
                {
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_AFTER)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;

                        if (eAheadPool == FBM_POOL_TYPE_TOTAL)
                        {
                            // start from the begining
                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4FbmStartAddr, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
                        else
                        {
                            // after another pool
                            // check 'ahead pool' ready or not
                            if (arPoolArrangeInfo[eAheadPool].u4Ready != 0)
                            {
                                u4Start = _arPool[eAheadPool].u4Addr;
                                u4Start += _arPool[eAheadPool].u4Size;
                                u4Start += FBM_POOL_PADDING_SIZE;

                                _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                                arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

                                _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                                _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                                _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                                _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                                _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                            }

                            ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                        }
                    }
                    else
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_BEFORE)
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

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
                        else
                        if (arPoolArrangeInfo[eBehindPool].u4Ready != 0)
                        {
                            u4Start = _arPool[eBehindPool].u4Addr;
                            u4Start -= _arPoolList[u4ListIdx].u4PoolSize;
                            u4Start -= _arPoolList[u4ListIdx].u4AddrAlign;
                            u4Start -= FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

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
                if ((_arPoolList[u4ListIdx].u4Inherit == 0) &&
                    (arPoolArrangeInfo[u4ListIdx].u4Ready == 0))
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
                if ((_arPoolList[u4ListIdx].u4Inherit != 0) &&
                    (arPoolArrangeInfo[u4ListIdx].u4Ready == 0))
                {
                    if ((_arPoolList[u4ListIdx].u4PoolSize == 0) &&
                        (arPoolArrangeInfo[u4ListIdx].eMode != FBM_POOL_ARRANGE_BETWEEN) &&
                        (arPoolArrangeInfo[u4ListIdx].eMode != FBM_POOL_ARRANGE_SHARE_ALL))
                    {
                        arPoolArrangeInfo[u4ListIdx].u4Ready = 1;
                    }
                    else
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_SHARE)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;

                        // check 'ahead pool' ready or not
                        if (arPoolArrangeInfo[eAheadPool].u4Ready != 0)
                        {
                            u4Start = _arPool[eAheadPool].u4Addr;

                            if (arPoolArrangeInfo[u4ListIdx].u4Offset != 0)
                            {
                                u4Start += arPoolArrangeInfo[u4ListIdx].u4Offset;
                                u4Start += FBM_POOL_PADDING_SIZE;
                            }

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                    }
                    else
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_SHARE_ALL)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        FBM_POOL_TYPE_T eAheadPool2;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;
                        eAheadPool2 = arPoolArrangeInfo[u4ListIdx].ePool2;

                        // check 'ahead pool' ready or not
                        if (arPoolArrangeInfo[eAheadPool].u4Ready != 0)
                        {
                            if ((arPoolArrangeInfo[u4ListIdx].u4Offset == 0) &&
                                (eAheadPool != eAheadPool2))
                            {
                                // check 'ahead pool' ready or not
                                if (arPoolArrangeInfo[eAheadPool2].u4Ready != 0)
                                {
                                    u4Start = _arPool[eAheadPool].u4Addr;
                                    u4Start += _arPool[eAheadPool2].u4Size;
                                    u4Start += FBM_POOL_PADDING_SIZE;

                                    _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                                    arPoolArrangeInfo[u4ListIdx].u4Ready = 1;
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

                                arPoolArrangeInfo[u4ListIdx].u4Ready = 1;
                            }

                            if (arPoolArrangeInfo[u4ListIdx].u4Ready != 0)
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
                    else
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_AFTER)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;

                        // check 'ahead pool' ready or not
                        if (arPoolArrangeInfo[eAheadPool].u4Ready != 0)
                        {
                            u4Start = _arPool[eAheadPool].u4Addr;
                            u4Start += _arPool[eAheadPool].u4Size;
                            u4Start += FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eAheadPool);
                    }
                    else
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_BEFORE)
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

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }
                        else
                        if (arPoolArrangeInfo[eBehindPool].u4Ready != 0)
                        {
                            u4Start = _arPool[eBehindPool].u4Addr;
                            u4Start -= _arPoolList[u4ListIdx].u4PoolSize;
                            u4Start -= _arPoolList[u4ListIdx].u4AddrAlign;
                            u4Start -= FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

                            _arPool[u4ListIdx].u4Size = _arPoolList[u4ListIdx].u4PoolSize;
                            _arPool[u4ListIdx].u4Mode = _arPoolList[u4ListIdx].u4Mode;
                            _arPool[u4ListIdx].u4Width= _arPoolList[u4ListIdx].u4Width;
                            _arPool[u4ListIdx].u4Height = _arPoolList[u4ListIdx].u4Height;
                            _arPool[u4ListIdx].u4Inherit = _arPoolList[u4ListIdx].u4Inherit;
                        }

                        ASSERT((FBM_POOL_TYPE_T)u4ListIdx != eBehindPool);
                    }
                    else
                    if (arPoolArrangeInfo[u4ListIdx].eMode == FBM_POOL_ARRANGE_BETWEEN)
                    {
                        FBM_POOL_TYPE_T eAheadPool;
                        FBM_POOL_TYPE_T eBehindPool;

                        eAheadPool = arPoolArrangeInfo[u4ListIdx].ePool1;
                        eBehindPool = arPoolArrangeInfo[u4ListIdx].ePool2;

                        // check 'ahead pool' & 'behind pool' ready or not

                        if ((eAheadPool == FBM_POOL_TYPE_NS) &&
                            (eBehindPool < FBM_POOL_TYPE_NS) &&
                            (arPoolArrangeInfo[eBehindPool].u4Ready != 0))
                        {
                            // share between u4FbmStartAddr

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4FbmStartAddr, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

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
                        else
                        if ((eAheadPool < FBM_POOL_TYPE_NS) &&
                            (eBehindPool < FBM_POOL_TYPE_NS) &&
                            (arPoolArrangeInfo[eAheadPool].u4Ready != 0) &&
                            (arPoolArrangeInfo[eBehindPool].u4Ready != 0))
                        {
                            u4Start = _arPool[eAheadPool].u4Addr;
                            u4Start += _arPool[eAheadPool].u4Size;
                            u4Start += FBM_POOL_PADDING_SIZE;

                            _arPool[u4ListIdx].u4Addr = FBM_ALIGN_MASK(u4Start, _arPoolList[u4ListIdx].u4AddrAlign);

                            arPoolArrangeInfo[u4ListIdx].u4Ready = 1;

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
                    (arPoolArrangeInfo[u4ListIdx].u4Ready == 0))
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
                ASSERT(u4Start == u4End);
            }
        }

        // handle null pool
        for (u4PoolTpIdx = 0; u4PoolTpIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolTpIdx++)
        {
            if (_arPool[u4PoolTpIdx].u4Addr == NULL)
            {
                _arPool[u4PoolTpIdx].u4Addr = FBM_ALIGN_MASK((UINT32)(u4FbmStartAddr),
                    _arPoolList[u4PoolTpIdx].u4AddrAlign);
            }
        }
    }

    // handle MPEG pool
    if (_arPool[FBM_POOL_TYPE_MPEG].u4Addr != NULL)
    {
        _arFbpConf[0].u4Addr = FBM_ALIGN_MASK(_arPool[FBM_POOL_TYPE_MPEG].u4Addr, FBM_FMG_Y_ALIGMENT);

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
                FBM_MEMSET((void *)_prFbp[u4PoolIdx]->aucOccupyList, FBP_MAX_NS, FBP_MAX_DISASSEMBLY);
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

#ifndef __KERNEL__
    // Pad the interval between pools
    for (u4PoolTpIdx = 0; u4PoolTpIdx < (UINT32)FBM_POOL_TYPE_NS; u4PoolTpIdx++)
    {
        UINT32 u4PadAddr;
        UINT32 u4PadLen;

        if ((_arPool[u4PoolTpIdx].u4Inherit == 0) &&
            (_arPool[u4PoolTpIdx].u4Addr > u4FbmStartAddr))
        {
            u4PadAddr = _arPool[u4PoolTpIdx].u4Addr - FBM_POOL_PADDING_SIZE;
            u4PadLen = FBM_POOL_PADDING_SIZE;

            FBM_MEMSET((void*)u4PadAddr, FBM_POOL_PADDING, u4PadLen);
        }
    }
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
void _FBM_GetPoolList(FBP_LIST_T** pprFbpList, UCHAR* pucFbpNs)
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
void _FBM_PoolRelease(UINT32 u4FbMemoryPool)
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
void _FBM_PoolStatus(void)
{
//#define FBM_PRINT_STATUS_TOTAL
    UCHAR ucIdx;

    LOG(1, "\n");

    for (ucIdx = 0; ucIdx < (UINT32)FBM_POOL_TYPE_NS; ucIdx++)
    {
#ifdef FBM_PRINT_STATUS_TOTAL
        LOG(1, "Addr(0x%08x) Size(0x%08x) Total (0x%08x) (%s) Pool\n",
            _arPool[ucIdx].u4Addr,
            _arPool[ucIdx].u4Size,
            (_arPool[ucIdx].u4Addr + _arPool[ucIdx].u4Size),
            _arPoolList[ucIdx].szName);
#else
        LOG(1, "Addr(0x%08x) Size(0x%08x) Mode(0x%x, %d, %d) (%s) Pool\n",
            _arPool[ucIdx].u4Addr,
            _arPool[ucIdx].u4Size,
            _arPool[ucIdx].u4Mode,
            _arPool[ucIdx].u4Width,
            _arPool[ucIdx].u4Height,
            _arPoolList[ucIdx].szName);
#endif
    }

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
}

/**
 * FBM SRM control call back
 *
 * @param void
 * @return void
 */
#ifdef CC_SRM_ON
static void _FBM_SrmCtrlCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UINT32 u4Idx;

    if ((DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5382px64MB) ||
        (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5381px32MB))
    {
        static UINT32 u4CurrentMpegMode = SRM_MPEG_MODE_FULL;

        LOG(3, "[SRM] MPEG CTRL %d %d %d %d\n", u4Arg1, u4Arg2, u4Arg3, u4Arg4);

        if (u4CurrentMpegMode != u4Arg2)
        {
            UINT32 u4Notify;
            u4CurrentMpegMode = u4Arg2;

            u4Notify = 1;
            if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5381px32MB)
            {
                // 5381p, Notify at 1080i/1080p only

                UINT32 u4PoolIdx;
                u4Notify = 0;
                for (u4PoolIdx = 0; u4PoolIdx < FBP_MAX_NS; u4PoolIdx++)
                {
                    if ((_prFbp[u4PoolIdx] != NULL) &&
                        (_prFbp[u4PoolIdx]->ucStatus == FBP_STATUS_USED) &&
                        ((_prFbp[u4PoolIdx]->ucType == FBM_FBG_TYPE_1080HD) ||(_prFbp[u4PoolIdx]->ucType == FBM_FBG_TYPE_1080HD_2_FB)))
                        {
                            u4Notify = 1;
                        }
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

        if (u4Arg2 == SRM_MPEG_MODE_FULL)
        {
            if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5382px64MB)
            {
                _arFbpList = _arFbpCqamDbkList;
                _ucFbpListNs = _ucFbpCqamDbkListNs;
            }
            else
            {
                _arFbpList = _arFbpCqamList;
                _ucFbpListNs = _ucFbpCqamListNs;
            }
        }
        else
        if (u4Arg2 == SRM_MPEG_MODE_HALF)
        {
            _arFbpList = _arFbp5382pPipList;
            _ucFbpListNs = _ucFbp5382pPipListNs;
        }
        else
        {
            ASSERT(0);
        }
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
    for (u4Idx = 0; u4Idx < _ucFbpListNs; u4Idx++)
    {
        if ((_arFbpList[u4Idx].u4Width >= u4Width) &&
            (_arFbpList[u4Idx].u4Height >= u4Height) &&
            (_arFbpList[u4Idx].u4Width < u4TargetWidth) &&
            (_arFbpList[u4Idx].u4Height < u4TargetHeight) &&
            (_arFbpList[u4Idx].u4Size <= u4BiggestEmptyPoolSize))
        {
            ucTargetType = _arFbpList[u4Idx].ucType;

            u4TargetWidth = _arFbpList[u4Idx].u4Width;
            u4TargetHeight = _arFbpList[u4Idx].u4Height;
        }
    }

    LOG(7, "Select(%d, %d) Type(%d) Big(0x%x)\n", u4Width, u4Height, ucTargetType, u4BiggestEmptyPoolSize);

    return ucTargetType;
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
    if (FBM_CHECK_CB_FUNC_VERIFY(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_POOL_CHANGE], _prFbmCbFunc->au4CbFuncCRC[FBM_CB_FUNC_POOL_CHANGE]))
    {
        ((FBM_CB_POOL_CHANGE_FUNC)_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_POOL_CHANGE])((FBM_POOL_TYPE_T)ucPoolType);
    }
    else
    {
        ASSERT(_prFbmCbFunc->au4CbFunc[FBM_CB_FUNC_POOL_CHANGE] == 0);
    }

    return (&_arPool[ucPoolType]);
}

//-------------------------------------------------------------------------
/** FBM_GetPoolInfoAuto
 *  Get Pool Info
 *  @param  ucPoolType
 *  @param  pInfo
 *  @return NONE.
 */
//-------------------------------------------------------------------------

FBM_POOL_T* FBM_GetPoolInfoAuto(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* pInfo)
{
    if (pInfo == NULL)
    {
        return NULL;
    }

    LOG(3, "FBM Get Pool (%d, %s) Source(%d) Width(%d) Height(%d)\n",
        ucPoolType,
        _arPoolList[ucPoolType].szName,
        pInfo->u4SourceType,
        pInfo->u4Width,
        pInfo->u4Height);

    if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5391x128MB)
    {
#ifdef CC_SCPOS_EN
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_SCPOS_MAIN)
        {
            if (pInfo->u4ColorFormat444 != 0)
            {
                _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_VGA;
                return FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_SCPOS_VGA);
            }
            else
            if ((pInfo->u4SourceType == (UINT32) VSS_DTV) &&
                (pInfo->u4Height >= 1080))
            {
                // 8 Bit at DTV-1080i
                _eCurrentScposMain = FBM_POOL_TYPE_SCPOS_MAIN;
                return FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_SCPOS_MAIN);
            }
            else
            {
                // 10 Bit
                _eCurrentScposMain = FBM_POOL_TYPE_RSV_01;
                return FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_RSV_01);
            }
        }
        else
        if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
        {
            if ((pInfo->u4SourceType == (UINT32) VSS_DTV) &&
                (pInfo->u4Height > 576))
            {
                // 8 Bit at DTV-1080i
                _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_MAIN;
                return FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_MDDI_MAIN);
            }
            else
            {
                // 10 Bit
                _eCurrentMddiMain = FBM_POOL_TYPE_MDDI_PIP;
                return FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_MDDI_PIP);
            }
        }
#endif

        return FBM_GetPoolInfo(ucPoolType);
    }

    return FBM_GetPoolInfo(ucPoolType);
}

//-------------------------------------------------------------------------
/** FBM_GetPoolMpeg
 *  Get MPEG Pool Info
 *  @param NONE
 *  @return NONE.
 */
//-------------------------------------------------------------------------

FBM_POOL_T* FBM_GetPoolMpeg(void)
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
    else
    if (ucPoolType == (UCHAR) FBM_POOL_TYPE_MDDI_MAIN)
    {
        return (UCHAR) _eCurrentMddiMain;
    }

    return ucPoolType;
}

