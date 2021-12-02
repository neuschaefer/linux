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
 * $RCSfile: osd_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_cmd.c
 *  This header file includes CLI function definitions of OSD driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_cmd.h"
#include "osd_drvif.h"
#ifndef CC_MTK_LOADER
#include "osdc_if.h"
#endif

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"

#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG OSD_IsLog
#endif /* CC_MTK_LOADER */
#include "x_debug.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _TEXT2ENUM_T
{
    CHAR* szText;
    INT32 i4Value;
} TEXT2ENUM_T;


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN INT32 _OSD_BASE_UpdateHwReg(void);
EXTERN INT32 _OSD_BASE_SetScmemCfg(UINT32 u4Value);
EXTERN INT32 _OSD_PLA_GetReg(UINT32 u4Plane, UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_PLA_SetReg(UINT32 u4Plane, const UINT32 *pOsdPlaneReg);
EXTERN INT32 _OSD_SC_SetFormat(UINT32 u4Scaler, UINT32 u4Value);
EXTERN INT32 _OSD_SC_UpdateHwReg(UINT32 u4Scaler);


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static INT32 _OsdCmdInit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdCmdQuery(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdPlaneCmdReset(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdEnable(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdFlip(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdAlpha(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdFading(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdHFilter(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdFifo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdDumpReg(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdPlaneCmdDumpInfo(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdListCmdCreate(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdList(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdDump(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdDelete(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdListCmdDetach(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdRgnCmdHelp(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdNew(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdInsert(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdDelete(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdSet(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdRgnCmdPattern(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _OsdScCmdScale(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _OsdScCmdSetLpf(INT32 i4Argc, const CHAR ** szArgv);

#ifndef CC_MTK_LOADER
CLIMOD_DEBUG_FUNCTIONS(OSD)
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static CLI_EXEC_T _arOsdPlaneCmdTbl[] =
{
    {
        "reset", "res", _OsdPlaneCmdReset, NULL,
        "Reset plane", CLI_SUPERVISOR
    },
    {
        "enable", "en", _OsdPlaneCmdEnable, NULL,
        "Set/get enable flag {plane#, on/off}", CLI_SUPERVISOR
    },
    {
        "flip", "flp", _OsdPlaneCmdFlip, NULL,
        "Flip to region list {plane#, list#}", CLI_SUPERVISOR
    },
    {
        "alpha", "afa", _OsdPlaneCmdAlpha, NULL,
        "Set/get alpha value {0~255}", CLI_SUPERVISOR
    },
    {
        "fading", "fa", _OsdPlaneCmdFading, NULL,
        "Set/get fading value {0~255}", CLI_SUPERVISOR
    },
    {
        "hfilter", "hf", _OsdPlaneCmdHFilter, NULL,
        "Enable HFilter {plane#, on/off}", CLI_SUPERVISOR
    },
    {
        "fifo", "ff", _OsdPlaneCmdFifo, NULL,
        "Configure fifo {plane#, fast-req, ex-vacancy-thr, vacancy-thr, "
        "full-thr}", CLI_SUPERVISOR
    },
    {
        "reg", "reg", _OsdPlaneCmdDumpReg, NULL,
        "Dump plane register", CLI_SUPERVISOR
    },
    {
        "dump", "dump", _OsdPlaneCmdDumpInfo, NULL,
        "Dump plane detail", CLI_SUPERVISOR
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arOsdListCmdTbl[] =
{
    {
        "create", "cr", _OsdListCmdCreate, NULL,
        "Create region list", CLI_SUPERVISOR
    },
    {
        "list", "ls", _OsdListCmdList, NULL,
        "List region list", CLI_SUPERVISOR
    },
    {
        "dump", "dump", _OsdListCmdDump, NULL,
        "Dump all in list {list#}", CLI_SUPERVISOR
    },
    {
        "delete", "del", _OsdListCmdDelete, NULL,
        "Delete region list {list#}", CLI_SUPERVISOR
    },
    {
        "detach", "det", _OsdListCmdDetach, NULL,
        "Detach region(s) from list {list#} {(rgn# | all)}", CLI_SUPERVISOR
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arOsdRegionCmdTbl[] =
{
    {
        "help", "h", _OsdRgnCmdHelp, NULL,
        "Display manual pages", CLI_SUPERVISOR
    },
    {
        "new", "n", _OsdRgnCmdNew, NULL,
        "Allocate new region", CLI_SUPERVISOR
    },
    {
        "insert", "ins", _OsdRgnCmdInsert, NULL,
        "Insert region into list {rgn#, list#}", CLI_SUPERVISOR
    },
    {
        "delete", "del", _OsdRgnCmdDelete, NULL,
        "Delete region from list {rgn#}", CLI_SUPERVISOR
    },
    {
        "set", NULL, _OsdRgnCmdSet, NULL,
        "Set region property {rgn#, attr, value}", CLI_SUPERVISOR
    },
    {
        "pattern", "pt", _OsdRgnCmdPattern, NULL,
        "Draw pattern {region#, mode(0:V line, 1:H line, 2:border, "
        "3:colorbar)}", CLI_SUPERVISOR
    },
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arOsdScalerCmdTbl[] =
{
    {
        "scale", "s", _OsdScCmdScale, NULL,
        "Scale OSD plane {scaler#, src_w, src_h, dst_w, dst_h}", CLI_SUPERVISOR
    },
    {
        "setlpf", "l", _OsdScCmdSetLpf, NULL,
        "Set low pass filter {scaler#, en}", CLI_SUPERVISOR
    },
#ifndef CC_MTK_LOADER
    {
        "osdc", "osdc", OSDC_Main, NULL,
        "c model", CLI_SUPERVISOR
    },
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arOsdCmdTbl[] =
{
    {
        "init", "i", _OsdCmdInit, NULL,
        "Init osd driver", CLI_SUPERVISOR
    },
    {
        "mode", "m", _OsdCmdMode, NULL,
        "Set osd output mode", CLI_SUPERVISOR
    },
    {
        "query", "q", _OsdCmdQuery, NULL,
        "Dump osd info", CLI_GUEST
    },
    {
        "plane", "p", NULL, _arOsdPlaneCmdTbl,
        "Plane commands", CLI_SUPERVISOR
    },
    {
        "list", "l", NULL, _arOsdListCmdTbl,
        "Region list commands", CLI_SUPERVISOR
    },
    {
        "region", "r", NULL, _arOsdRegionCmdTbl,
        "Region commands", CLI_SUPERVISOR
    },
    {
        "scaler", "sc", NULL, _arOsdScalerCmdTbl,
        "Scaler commands", CLI_SUPERVISOR
    },
#ifndef CC_MTK_LOADER
    CLIMOD_DEBUG_CLIENTRY(OSD),
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Osd)
{
    "osd", NULL, NULL, _arOsdCmdTbl,
    "Osd driver", CLI_SUPERVISOR
};

static TEXT2ENUM_T _arRegionFieldEnum[] =
{
    // alloc, prev, next, flags, pos_x, pos_y, src_w, src_h, src_addr, src_pitch
    // disp_w, disp_h, colormode, alpha, clip_v, clip_h, pal_load, pal_addr,
    // pal_len, step_v, step_h, colorkey, colorkey_en, mix_sel
    {"alloc", OSD_RGN_ALLOC},
    {"prev", OSD_RGN_PREV},
    {"next", OSD_RGN_NEXT},
    {"flags", OSD_RGN_FLAGS},
    {"colormode", OSD_RGN_COLORMODE},
    {"pos_x", OSD_RGN_POS_X},
    {"pos_y", OSD_RGN_POS_Y},
    {"out_w", OSD_RGN_OUT_W},
    {"out_h", OSD_RGN_OUT_H},
    {"src_addr", OSD_RGN_BMP_ADDR},
    {"src_pitch", OSD_RGN_BMP_PITCH},
    {"clip_h", OSD_RGN_CLIP_H},
    {"clip_v", OSD_RGN_CLIP_V},
    {"src_w", OSD_RGN_BMP_W},
    {"src_h", OSD_RGN_BMP_H},
    {"disp_w", OSD_RGN_DISP_W},
    {"disp_h", OSD_RGN_DISP_H},
    {"step_h", OSD_RGN_STEP_H},
    {"step_v", OSD_RGN_STEP_V},
    {"alpha", OSD_RGN_ALPHA},
    {"mix_sel", OSD_RGN_MIX_SEL},
    {"pal_load", OSD_RGN_PAL_LOAD},
    {"pal_addr", OSD_RGN_PAL_ADDR},
    {"pal_len", OSD_RGN_PAL_LEN},
    // note the order for strcmp()
    {"colorkey_en", OSD_RGN_COLOR_KEY_EN},
    {"colorkey", OSD_RGN_COLOR_KEY},
    {"big_endian", OSD_RGN_BIG_ENDIAN},
    {"vflip", OSD_RGN_V_FLIP},
    {"hmirror", OSD_RGN_H_MIRROR},
    {NULL, -1}
};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdReset(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.pla.reset {plane (1,2,3)}
    INT32 i4Plane;

    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    return OSD_PLA_Reset(i4Plane);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.enable {plane (1,2,3)} [{enable (1,0)}]
    INT32 i4Plane;
    BOOL fgEnable;

    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    if (i4Argc >= 3)
    {
        // set enable flag
        switch (szArgv[2][0])
        {
        case 'e':
        case 'E':
        case 'y':
        case 'Y':
        case 't':
        case 'T':
        case '1':
            return OSD_PLA_Enable(i4Plane, 1);

        default:
            return OSD_PLA_Enable(i4Plane, 0);
        }
    }
    else
    {
        OSD_PLA_UNION_T rOsdPlaneReg;
        _OSD_PLA_GetReg(i4Plane - 1, rOsdPlaneReg.au4Reg);
        fgEnable = (BOOL)rOsdPlaneReg.rField.fgEn;
        Printf("OSD Plane %d enable : %d\n", i4Plane + 1, fgEnable);
    }
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdFlip(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.flip {plane (1,2,3)} {rgnlist}
    INT32 i4Plane;
    INT32 i4RgnList;

    CHK_NUM_MIN_PARAM(2);
    GET_PLANE(i4Plane);
    GET_RGNLIST(i4RgnList, 2);

    return OSD_PLA_FlipTo(i4Plane, i4RgnList);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdAlpha(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.alpha {plane (1,2,3)} [{alpha}]
    INT32 i4Plane;
    INT32 i4Alpha;

    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    if (i4Argc >= 3)
    {
        // set alpha
        i4Alpha = StrToInt(szArgv[2]);
        if ((i4Alpha < 0) || (i4Alpha > 255))
        {
            return -(INT32)OSD_CLI_RET_ERR_PARAM;
        }
        return OSD_PLA_SetBlendLevel(i4Plane, (UINT8)i4Alpha);
    }
    else
    {
        // get alpha
        UINT8 u1Alpha;
        OSD_PLA_GetBlendLevel(i4Plane, &u1Alpha);
        Printf("OSD Plane %d alpha : %d(0x%02x)\n", i4Plane, (UINT32)u1Alpha);
    }
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdFading(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.alpha {plane (1,2,3)} [{alpha}]
    INT32 i4Plane;
    INT32 i4Alpha;

    CHK_NUM_MIN_PARAM(1);
    GET_PLANE(i4Plane);

    if (i4Argc >= 3)
    {
        // set alpha
        i4Alpha = StrToInt(szArgv[2]);
        if ((i4Alpha < 0) || (i4Alpha > 255))
        {
            return -(INT32)OSD_CLI_RET_ERR_PARAM;
        }
        return OSD_PLA_SetFading(i4Plane, (UINT8)i4Alpha);
    }
    else
    {
        // get alpha
        UINT8 u1Alpha;
        OSD_PLA_GetFading(i4Plane, &u1Alpha);
        Printf("OSD Plane %d fading : %d(0x%02x)\n", i4Plane, (UINT32)u1Alpha);
    }
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdHFilter(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.hfilter {plane (1,2,3)} [{enable}]
    INT32 i4Plane;

    CHK_NUM_MIN_PARAM(2);
    GET_PLANE(i4Plane);
    INT32 i4Enable = StrToInt(szArgv[2]);

    if ((i4Enable < 0) || (i4Enable > 1))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    return OSD_PLA_SetHFilter(i4Plane, (BOOL)i4Enable);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdFifo(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.fifo {plane (1,2,3)}
    // [{fast request, ex vacancy threshold, vacancy threshold, full threshold}]
    INT32 i4Plane;

    CHK_NUM_MIN_PARAM(5);
    GET_PLANE(i4Plane);
    INT32 i4FastReq = StrToInt(szArgv[2]);
    INT32 i4ExVacThr = StrToInt(szArgv[3]);
    INT32 i4VacThr = StrToInt(szArgv[4]);
    INT32 i4FullThr = StrToInt(szArgv[5]);

    if ((i4FastReq < 0) || (i4FastReq > 1))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    else if ((i4ExVacThr < 0) || (i4ExVacThr > 0x3f))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    if ((i4VacThr < 0) || (i4VacThr > 0xf))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    if ((i4FullThr < 0) || (i4FullThr > 0xf))
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }
    return OSD_PLA_SetFifo(i4Plane, (BOOL)i4FastReq, (UINT8)i4ExVacThr,
                           (UINT8)i4VacThr, (UINT8)i4FullThr);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdDumpReg(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.dumpreg
    OSD_PLA_UNION_T rOsdPlaneReg;
    INT32 i4Plane;

    for (i4Plane = 0; i4Plane < OSD_PLANE_MAX_NUM; i4Plane++)
    {
        _OSD_PLA_GetReg(i4Plane, rOsdPlaneReg.au4Reg);
        Printf("OSD Plane %d : 0x%08x 0x%08x\n", i4Plane + 1,
               rOsdPlaneReg.au4Reg[0], rOsdPlaneReg.au4Reg[1]);
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdPlaneCmdDumpInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.plane.dumpinfo
    UINT32 u4Plane;

    for (u4Plane = 0; u4Plane < OSD_PLANE_MAX_NUM; u4Plane++)
    {
        OSD_PLA_Dump(u4Plane);
        Printf("\n");
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdCreate(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.create
    INT32 i4RgnList;
    INT32 i4Ret;
    INT32 i4Count = 1;

    if (i4Argc==2)
    {
        i4Count = StrToInt(szArgv[1]);
    }

    do
    {
        i4Ret = OSD_RGN_LIST_Create((UINT32*)&i4RgnList);

        if (i4Ret == OSD_RET_OK)
        {
            LOG(5, "OSD region list created : %d\n", i4RgnList);
        }
    }
    while (--i4Count != 0);

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdList(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.list
    INT32 i4RgnList = 0;
    INT32 i4Ret = 0;
    INT32 i4ListHead, i4ListTail, i4Count;
    UINT32 fgFlags;
    INT32 i4UsedCount;

    Printf("List  Head    Tail    Count    Flags\n");
    Printf("-----------------------------------------\n");
    for (i4Ret = 0, i4UsedCount = 0; i4Ret == 0; i4RgnList++)
    {
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_HEAD,
                                 (UINT32*)&i4ListHead);
        if (i4Ret)
        {
            continue;
        }
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_TAIL,
                                 (UINT32*)&i4ListTail);
        if (i4Ret)
        {
            continue;
        }
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_COUNT,
                                 (UINT32*)&i4Count);
        if (i4Ret)
        {
            continue;
        }
        i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_LIST_FLAGS, &fgFlags);
        if (i4Ret)
        {
            continue;
        }

        Printf(" %3d  h[%04d] t[%04d] cnt(%3d) fg(0x%04x)\n",
                i4RgnList, i4ListHead, i4ListTail, i4Count, fgFlags);
        if (fgFlags)
        {
            i4UsedCount++;
        }
    }
    Printf("-----------------------------------------\n");
    Printf("Total %d lists are currently used.\n", i4UsedCount);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdDump(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.dump {region_list}
    INT32 i4RgnList;
    INT32 i4Region;
    INT32 i4Ret;
    INT32 i4Count;

    CHK_NUM_MIN_PARAM(1);
    GET_RGNLIST(i4RgnList, 1);

    i4Ret = OSD_RGN_LIST_Get(i4RgnList, OSD_RGN_LIST_COUNT, (UINT32*)&i4Count);
    if (i4Ret)
    {
        return -OSD_CLI_RET_ERR_LIST;
    }

    OSD_RGN_LIST_Get(i4RgnList,OSD_RGN_LIST_HEAD, (UINT32*)&i4Region);
    while (i4Count--)
    {
        UINT32 u4Next;
        if (OSD_RGN_Dump(i4Region) < 0)
        {
            break;
        }
        //*OSD_RGN_Get(OSD_RGN_NEXT, i4Region, &u4Next);
        OSD_RGN_Get(i4Region, OSD_RGN_NEXT, &u4Next);
        i4Region = (INT32)u4Next;
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdDelete(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.delete {region_list}
    INT32 i4RgnList;

    CHK_NUM_MIN_PARAM(1);
    GET_RGNLIST(i4RgnList, 1);

    return OSD_RGN_LIST_Delete(i4RgnList);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdListCmdDetach(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.list.detach {region_list} {(region | all)}
    INT32 i4RgnList;

    CHK_NUM_MIN_PARAM(2);
    GET_RGNLIST(i4RgnList, 1);

    if (x_strncmp("all", szArgv[2], 3) == 0)
    {
        return OSD_RGN_LIST_DetachAll(i4RgnList);
    }

    if ((szArgv[2][0] <= '9') &&
        (szArgv[2][0] >= '0'))
    {
        INT32 i4Region;
        i4Region = StrToInt(szArgv[2]);

        return OSD_RGN_Detach(i4Region, i4RgnList);
    }

    return -(INT32)OSD_CLI_RET_ERR_PARAM;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdHelp(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("osd.region.new {src_w} {src_h} "
           "{src_addr} {color_mode} {src_pitch} "
           "{dst_x} {dst_y} {dst_w} {dst_h}\n"
           "osd.region.append {region} {list}\n"
           "osd.region.insert {region} {list}\n"
           "osd.region.delete {region}\n"
           "osd.region.set {region} {field} {value}\n"
           "\tfields:"
           "\t\talloc, prev, next, flags, pos_x, pos_y,\n"
           "\t\tsrc_w, src_h, src_addr, src_pitch,\n"
           "\t\tout_w, out_h, disp_w, disp_h, colormode, alpha\n"
           "\t\tclip_v, clip_h, pal_load, pal_addr, pal_len\n"
           "\t\tvflip, hmirror\n"
           "\t\tstep_v, step_h, colorkey, colorkey_en, mix_sel\n");

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdNew(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.region.new \
    //          {src_w} {src_h} {src_addr} \
    //          {color_mode} {src_pitch} \
    //          {dst_x} {dst_y} {dst_w} {dst_h}

    INT32 i4Ret;
    INT32 i4Region;
    UINT32 u4BmpWidth, u4BmpHeight;
    void * pvBitmap;
    UINT32 eColorMode, u4BmpPitch = 0;
    UINT32 u4DispX, u4DispY, u4DispW, u4DispH;

    CHK_NUM_MIN_PARAM(4);

    u4BmpWidth = (UINT32)StrToInt(szArgv[1]);
    u4BmpHeight = (UINT32)StrToInt(szArgv[2]);
    pvBitmap = (void *)StrToInt(szArgv[3]);
    eColorMode = (UINT32)StrToInt(szArgv[4]);

    if (i4Argc == 10)
    {
        u4BmpPitch = (UINT32)StrToInt(szArgv[5]);
        u4DispX = (UINT32)StrToInt(szArgv[6]);
        u4DispY = (UINT32)StrToInt(szArgv[7]);
        u4DispW = (UINT32)StrToInt(szArgv[8]);
        u4DispH = (UINT32)StrToInt(szArgv[9]);
    }
    else
    {
        OSD_GET_PITCH_SIZE(eColorMode, u4BmpWidth, u4BmpPitch);
        u4DispX = 0;
        u4DispY = 0;
        u4DispW = u4BmpWidth;
        u4DispH = u4BmpHeight;
    }

    i4Ret = OSD_RGN_Create
        ((UINT32*)&i4Region,
        u4BmpWidth, u4BmpHeight, pvBitmap,
        eColorMode, u4BmpPitch,
        u4DispX, u4DispY, u4DispW, u4DispH);

    if (i4Ret)
    {
        LOG(1, "OSD region create failed : %d\n", i4Ret);
    }
    else
    {
        LOG(5, "OSD region created : i4Region == %d\n", i4Region);

        if (eColorMode<=OSD_CM_RGB_CLUT8 ||
            eColorMode==OSD_CM_ARGB8888_DIRECT32)
        {
            OSD_RGN_SetBigEndian(i4Region, 1);
        }

        OSD_RGN_Dump(i4Region);
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdInsert(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.region.insert {region} {list}
    INT32 i4Region, i4RgnList;

    CHK_NUM_MIN_PARAM(2);
    GET_REGION(i4Region, 1);
    GET_RGNLIST(i4RgnList, 2);

    return OSD_RGN_Insert(i4Region, i4RgnList);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdDelete(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.region.delete {region};
    INT32 i4Region;
    CHK_NUM_MIN_PARAM(1);
    GET_REGION(i4Region, 1);

    return OSD_RGN_Delete(i4Region);
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdSet(INT32 i4Argc, const CHAR ** szArgv)
{
    // osd.region.set {region} {field} {value}
    //
    // where filed in list:
    //      alloc, prev, next, flags, pos_x, pos_y, src_w, src_h, src_addr,
    //      src_pitch, disp_w, disp_h, colormode, alpha, clip_v, clip_h, 
    //      pal_load, pal_addr, pal_len, step_v, step_h, colorkey, colorkey_en,
    //      mix_sel

    INT32 i4Region;
    INT32 eRegionSetCmd;
    CHK_NUM_MIN_PARAM(3);
    GET_REGION(i4Region, 1);

    eRegionSetCmd = OSD_FindField(szArgv[2]);
    if (eRegionSetCmd < 0)
    {
        return eRegionSetCmd;
    }

    return OSD_RGN_Set(i4Region, eRegionSetCmd, StrToInt(szArgv[3]));
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdRgnCmdPattern(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_DEBUG
    INT32 i4Region;
    UINT32 u4Pattern, u4RgnW, u4RgnH, u4ColorMode, u4BmpAddress;
    CHK_NUM_MIN_PARAM(2);
    GET_REGION(i4Region, 1);

    u4Pattern = StrToInt(szArgv[2]);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_ADDR, &u4BmpAddress);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_W, &u4RgnW);
    OSD_RGN_Get(i4Region, OSD_RGN_BMP_H, &u4RgnH);
    OSD_RGN_Get(i4Region, OSD_RGN_COLORMODE, &u4ColorMode);

    switch (u4Pattern)
    {
    case 0:
        OSD_DrawVLine((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 1:
        OSD_DrawHLine((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 2:
        OSD_DrawBorder((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 3:
        OSD_DrawColorbar((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 4:
        OSD_DrawSlt((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    case 5:
        OSD_DrawRamp((UINT8*)u4BmpAddress, u4RgnW, u4RgnH, u4ColorMode);
        break;
    default:
        LOG(1, "no support\n");
    }
    HalFlushInvalidateDCache();
#endif
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdScCmdScale(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(5);
    INT32 sc = StrToInt(szArgv[1]);
    INT32 sw = StrToInt(szArgv[2]);
    INT32 sh = StrToInt(szArgv[3]);
    INT32 dw = StrToInt(szArgv[4]);
    INT32 dh = StrToInt(szArgv[5]);
    OSD_SC_Scale(sc, TRUE, sw, sh, dw, dh);
    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdScCmdSetLpf(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(2);
    INT32 sc = StrToInt(szArgv[1]);
    INT32 en = StrToInt(szArgv[2]);

    if (i4Argc > 3)
    {
        OSD_SC_SetLpfInfo(sc, en, StrToInt(szArgv[3]), StrToInt(szArgv[4]),
                          StrToInt(szArgv[5]), StrToInt(szArgv[6]),
                          StrToInt(szArgv[7]));
    }
    else
    {
        OSD_SC_SetLpf(sc, en);
    }

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    OSD_Reset();

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCmdMode(INT32 i4Argc, const CHAR ** szArgv)
{
    CHK_NUM_MIN_PARAM(1);
    INT32 mode = StrToInt(szArgv[1]);
    OSD_VERIFY_DISPLAY_MODE(mode);
    OSD_BASE_SetDisplayMode(mode);

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _OsdCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Tmp;

    for (u4Tmp = 0; u4Tmp < OSD_PLANE_MAX_NUM; u4Tmp++)
    {
        OSD_PLA_Dump(u4Tmp);
    }
    OSD_BASE_Dump();

    return OSD_CLI_RET_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 OSD_FindField(const CHAR *szText)
{
    if (NULL == szText)
    {
        return -(INT32)OSD_CLI_RET_ERR_PARAM;
    }

    TEXT2ENUM_T* prText2Enum = _arRegionFieldEnum;
    while (prText2Enum->szText)
    {
        if (x_strncmp(prText2Enum->szText, szText,
                      x_strlen(prText2Enum->szText)) == 0)
        {
            break;
        }
        else
        {
            prText2Enum++;
        }
    }

    return prText2Enum->i4Value;
}

