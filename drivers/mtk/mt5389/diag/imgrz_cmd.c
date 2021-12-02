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
 * $RCSfile: imgrz_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_cmd.c
 *  This file contains implementation of CLI CMD for IMGRZ
 *  
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e960 -e961 */
#include "x_printf.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#define DEFINE_IS_LOG   IMGRZ_IsLog       // for LOG use
#include "x_debug.h"

#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "drvcust_if.h"
#include "gfx_if.h"
/*lint -restore */

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 */
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct _IMGRZCMDTEXT2ENUM_T
{
    CHAR *szText;
    INT32 i4Value;
} IMGRZCMDTEXT2ENUM_T;


typedef enum _ENUM_CLIIMGRZ_ERR_CODE_T
{
    E_CLIIMGRZ_OK          = 0,
    E_CLIIMGRZ_ERR_GENERAL = 1000,
    E_CLIIMGRZ_ERR_PARAM   = 1001,
    // add here
    E_CLIIMGRZ_UNKNOWN     = 1999
} ENUM_CLIIMGRZ_ERR_CODE_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define IMGRZCMD_CHK_NUM_MIN_PARAM(N)           \
    if (i4Argc < ((N)+1))                       \
    {                                           \
        return -(INT32)E_CLIIMGRZ_ERR_PARAM;    \
    }


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

extern INT32 CLI_Parser(const CHAR* szCmdBuf);

// imgrz_debug.h
extern void IMGRZ_DbgDumpInfo(UINT32 u4Which);

// imgrz_hw.h
extern void IMGRZ_HwSetEngClkSrc(UINT32 u4Which, INT32 i4ClkSrc);

// imgrz_cmdque.h
extern void IMGRZ_CmdQueDbgInfo(UINT32 u4Which);
extern void IMGRZ_CmdQueSetNewConfig(UINT32 u4Which, INT32 i4NewConfig);

// imgrz_dif.h
extern void IMGRZ_DifSetMode(UINT32 u4Which, UINT32 u4Mode);;


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

// set

// JPG Op

// VDO Op

// OSD Op
static INT32 _ImgrzOsdOpCmdSetSrcBuf    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzOsdOpCmdSetTgBuf     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzOsdOpCmdScale        (INT32 i4Argc, const CHAR ** szArgv);


#if defined(IMGRZ_ENABLE_SW_MODE)
static INT32 _ImgrzCmdEnCm              (INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _ImgrzCmdInit              (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdReset             (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdFlush             (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdQuery             (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdSetEngClkSrc      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdSetQueConfig      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdSetCrashDetect    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdSlt               (INT32 i4Argc, const CHAR ** szArgv);

//static INT32 _RzCmdGetEnum(const CHAR *szText, const IMGRZCMDTEXT2ENUM_T* prText2Enum);
static INT32 _ImgrzSlt(UINT32 u4Loop, UINT32 u4SwEn);

CLIMOD_DEBUG_FUNCTIONS(IMGRZ)     // for LOG use


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

// set table
static CLI_EXEC_T _arImgrzSetCmdTbl[] =
{

//    {
//        "idx2dirsrc", NULL, _ImgrzSetCmdIdx2DirSrc, NULL, 
//        "Set idx2dir source buffer", CLI_SUPERVISOR
//    },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// osd operation table
static CLI_EXEC_T _arImgrzOsdOpCmdTbl[] =
{
    {
        "src", NULL, _ImgrzOsdOpCmdSetSrcBuf, NULL, 
        "set source buffer", CLI_SUPERVISOR
    },
    {
        "tg", NULL, _ImgrzOsdOpCmdSetTgBuf, NULL, 
        "set target buffer", CLI_SUPERVISOR
    },
    {
        "scale", NULL, _ImgrzOsdOpCmdScale, NULL, 
        "scale up/down", CLI_SUPERVISOR
    },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


// cmd table
static CLI_EXEC_T _arImgrzCmdTbl[] =
{
#if defined(IMGRZ_ENABLE_SW_MODE)
    {
        "encm", NULL, _ImgrzCmdEnCm, NULL, 
        "Enable IMGRZ C-model", CLI_SUPERVISOR
    },
#endif
    {
        "slt", NULL, _ImgrzCmdSlt, NULL, 
        "Imgrz SLT", CLI_SUPERVISOR
    },
    {
        "init", "i", _ImgrzCmdInit, NULL, 
        "Imgrz init", CLI_SUPERVISOR
    },
    {
        "reset", "res", _ImgrzCmdReset, NULL, 
        "Imgrz reset", CLI_SUPERVISOR
    },
    {
        "flush", "f", _ImgrzCmdFlush, NULL, 
        "Imgrz command queue flush (auto on off)", CLI_SUPERVISOR
    },
    {
        "query", "q", _ImgrzCmdQuery, NULL, 
        "Imgrz Query HW status", CLI_SUPERVISOR
    },
    {
        "clksrc", "clk", _ImgrzCmdSetEngClkSrc, NULL, 
        "Imgrz engine clock source", CLI_SUPERVISOR
    },
    {
        "cmdqcfg", "qcfg", _ImgrzCmdSetQueConfig, NULL, 
        "Imgrz engine cmdque configuration", CLI_SUPERVISOR
    },
    {
        "crashd", NULL, _ImgrzCmdSetCrashDetect, NULL, 
        "Imgrz engine crash detection", CLI_SUPERVISOR
    },
    {
        "set", "s", NULL, _arImgrzSetCmdTbl, 
        "Imgrz parameter set", CLI_SUPERVISOR
    },
    {
        "osdop", NULL, NULL, _arImgrzOsdOpCmdTbl, 
        "Imgrz-OSD operations", CLI_SUPERVISOR
    },
    {
        "slt", NULL, _ImgrzCmdSlt, NULL,
        "Imgrz-SLT operations", CLI_SUPERVISOR
    },
//    {
//        "vdo", NULL, NULL, _arImgrzVdoOpCmdTbl, 
//        "Imgrz-Video operations", CLI_SUPERVISOR
//    },
//    {
//        "jpg", NULL, NULL, _arImgrzJpgOpCmdTbl, 
//        "Imgrz-JPG operations", CLI_SUPERVISOR
//    },


    CLIMOD_DEBUG_CLIENTRY(IMGRZ),     // for LOG use

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#if 0
static IMGRZCMDTEXT2ENUM_T _arImgrzOsdCmEnumTbl[] =
{
    {"cbycry",      (INT32)E_RZ_OSD_DIR_CM_CbYCrY},
    {"ycbycr",      (INT32)E_RZ_OSD_DIR_CM_YCbYCr},
    {"argb8888",    (INT32)E_RZ_OSD_DIR_CM_AYCbCr8888},
    {"aycbcr8888",  (INT32)E_RZ_OSD_DIR_CM_ARGB8888},
    {"argb4444",    (INT32)E_RZ_OSD_DIR_CM_ARGB4444},
    {"argb1555",    (INT32)E_RZ_OSD_DIR_CM_ARGB1555},
    {"rgb565",      (INT32)E_RZ_OSD_DIR_CM_RGB565},
    {"8bpp",        (INT32)E_RZ_OSD_IDX_CM_8BPP},
    {"4bpp",        (INT32)E_RZ_OSD_IDX_CM_4BPP},
    {"2bpp",        (INT32)E_RZ_OSD_IDX_CM_2BPP},

    {NULL,          (INT32)E_RZ_OSD_DIR_CM_ARGB8888}
};
#endif

static INT32 _i4ImgrzFlushMode = 0;


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _ImgrzOsdOpCmdSetSrcBuf
 *  imgrz.osdop.src 
 *      {base_addr} 
 *      {direct_color_mode_flag}
 *      {color_mode} 
 *      {pitch_in_bytes}
 *      {x_coordinate}
 *      {y_coordinate}
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzOsdOpCmdSetSrcBuf(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4Base, u4IsDirCM, u4CM, u4Pitch, u4Sx, u4Sy;
    IMGRZCMD_CHK_NUM_MIN_PARAM(6);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
        (szArgv[3] == NULL) || (szArgv[4] == NULL) || 
        (szArgv[5] == NULL) || (szArgv[6] == NULL))
    {
        return -1;
    }
    
    u4Base    = (UINT32)StrToInt(szArgv[1]);
    u4IsDirCM = (UINT32)StrToInt(szArgv[2]);
    u4CM      = (UINT32)_RzCmdGetEnum(szArgv[3], _arImgrzOsdCmEnumTbl);
    u4Pitch   = (UINT32)StrToInt(szArgv[4]);
    u4Sx      = (UINT32)StrToInt(szArgv[5]);
    u4Sy      = (UINT32)StrToInt(szArgv[6]);
#endif
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzOsdOpCmdSetTgBuf
 *  imgrz.osdop.tg 
 *     {base_addr} 
 *     {direct_color_mode} 
 *     {pitch_in_bytes}
 *     {x_coordinate}
 *     {y_coordinate}
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzOsdOpCmdSetTgBuf(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4Base, u4DirCM, u4Pitch, u4Dx, u4Dy;
    IMGRZCMD_CHK_NUM_MIN_PARAM(5);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
        (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL))
    {
        return -1;
    }
    
    u4Base  = (UINT32)StrToInt(szArgv[1]);
    u4DirCM = (UINT32)_RzCmdGetEnum(szArgv[2], _arImgrzOsdCmEnumTbl);
    u4Pitch = (UINT32)StrToInt(szArgv[3]);
    u4Dx    = (UINT32)StrToInt(szArgv[4]);
    u4Dy    = (UINT32)StrToInt(szArgv[5]);
#endif
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzOsdOpCmdScale
 *  imgrz.osdop.scale 
 *      {src_width} 
 *      {src_height}
 *      {tg_width} 
 *      {tg_height}
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzOsdOpCmdScale(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4SrcW, u4SrcH, u4TgW, u4TgH;
    IMGRZCMD_CHK_NUM_MIN_PARAM(4);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL))
    {
        return -1;
    }

    u4SrcW = (UINT32) StrToInt(szArgv[1]);
    u4SrcH = (UINT32) StrToInt(szArgv[2]);
    u4TgW  = (UINT32) StrToInt(szArgv[3]);
    u4TgH  = (UINT32) StrToInt(szArgv[4]);
#endif
//    IMGRZ_OsdScale(u4SrcW, u4SrcH, u4TgW, u4TgH);
    if (_i4ImgrzFlushMode)
    {
        IMGRZ_Flush();
        IMGRZ_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdInit
 *  imgrz.init
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    IMGRZ_Init();
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdEnCm
 *  imgrz.encm
 */
//-------------------------------------------------------------------------
#if defined(IMGRZ_ENABLE_SW_MODE)
static INT32 _ImgrzCmdEnCm(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4EnCm = 0;

    if (szArgv == NULL)
    {
        return -1;
    }
    
    if (i4Argc < 2)
    {
        Printf("arg: EnCm (1:on, 0:off)\n");
        return -1;
    }

    u4EnCm = (UINT32) StrToInt(szArgv[1]);
    
    if (u4EnCm)
    {
        IMGRZ_Reset(E_FIRST_RESIZER);
        IMGRZ_DifSetMode(E_FIRST_RESIZER, (UINT32)E_RZ_SW_MOD);
        Printf("Enable IMGRZ C-model\n");
    }
    else
    {
        IMGRZ_Reset(E_FIRST_RESIZER);
        IMGRZ_DifSetMode(E_FIRST_RESIZER, (UINT32)E_RZ_HW_MOD);
        Printf("Disable IMGRZ C-model\n");
    }

    return 0;
}
#endif


//-------------------------------------------------------------------------
/** _ImgrzCmdReset
 *  imgrz.reset
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdReset(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    // Prevent emulation program from 
    // resetting engine before returning interrupt
    IMGRZ_Wait();

    IMGRZ_Reset(E_FIRST_RESIZER);
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdFlush
 *  imgrz.flush [{auto, delayed|off}]
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdFlush(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    if ((i4Argc >= 2) &&
        ((x_strncmp("auto", szArgv[1], 4) == 0) ||
         (x_strncmp("AUTO", szArgv[1], 4) == 0)))
    {
        _i4ImgrzFlushMode = 1;
    }
    else
    {
        _i4ImgrzFlushMode = 0;
    }

    if (i4Argc == 1)
    {
        IMGRZ_Flush();
        IMGRZ_Wait();
    }
    return (INT32)E_CLIIMGRZ_OK;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdQuery
 *  imgrz.query
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdQuery           (INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("current hw status = %s\n", IMGRZ_QueryHwIdle(E_FIRST_RESIZER) ? "idle" : "busy");

#if defined(IMGRZ_DEBUG_MODE)
    Printf("    << In IMGRZ Debug Mode >>\n");
    Printf("(if imgrz engine status is idle, "    \
        "flush count must be euqal to interrupt count)\n");

    IMGRZ_DbgDumpInfo(E_FIRST_RESIZER);
    IMGRZ_CmdQueDbgInfo(E_FIRST_RESIZER);
#endif // #if defined(IMGRZ_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdSetEngClkSrc
 *  imgrz.clksrc
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSetEngClkSrc    (INT32 i4Argc, const CHAR ** szArgv)
{
#if defined(IMGRZ_DEBUG_MODE)

    INT32 i4ClkSrc = 0;
    INT32 i4ClkSrcMax = 8;

    if (i4Argc < 2)
    {
        Printf("\narg: clk_src_num\n");
        Printf("   0 : xtal clk\n");
        Printf("   1 : syspll / 2  (162 / 216 MHz)\n");
        Printf("   2 : tvdpll / 3  (180   MHz)\n");
        Printf("   3 : tvdpll / 5  (108   MHz)\n");
        Printf("   4 : dtdpll / 3  (200   MHz)\n");
        Printf("   5 : dtdpll / 4  (150   MHz)\n");
        Printf("   6 : pspll  / 1  (148.5 MHz)\n");
        Printf("   7 : mem\n");
        return -1;
    }
    
    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    i4ClkSrc  = (INT32) StrToInt(szArgv[1]);

    if (i4ClkSrc >= i4ClkSrcMax)
    {
        Printf("Error: clk src = %d\n", i4ClkSrc);
        return -1;
    }
    
    IMGRZ_HwSetEngClkSrc(E_FIRST_RESIZER, i4ClkSrc);
    Printf("GRA_CLK_ADDR : 0x2000D224\n");
#endif // #if defined(IMGRZ_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdSetQueConfig
 *  imgrz.cmdqcfg
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSetQueConfig    (INT32 i4Argc, const CHAR ** szArgv)
{
#if defined(IMGRZ_DEBUG_MODE)

    INT32 i4QueCfg = 0;
    INT32 i4QueCfgMax = 8;

    if (i4Argc < 2)
    {
        Printf("\narg: cmdq_cfg\n");
        Printf("   0 : cmdq_002_KB\n");
        Printf("   1 : cmdq_004_KB\n");
        Printf("   2 : cmdq_008_KB\n");
        Printf("   3 : cmdq_016_KB\n");
        Printf("   4 : cmdq_032_KB\n");
        Printf("   5 : cmdq_064_KB\n");
        Printf("   6 : cmdq_128_KB\n");
        Printf("   7 : cmdq_256_KB\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    i4QueCfg  = (INT32) StrToInt(szArgv[1]);

    if (i4QueCfg >= i4QueCfgMax)
    {
        Printf("Error: cmdq_cfg = %d\n", i4QueCfg);
        return -1;
    }
    
    IMGRZ_CmdQueSetNewConfig(E_FIRST_RESIZER, i4QueCfg);
#endif // #if defined(IMGRZ_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdSetCrashDetect
 *  imgrz.crashd
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSetCrashDetect    (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value = 0;

    if (i4Argc < 2)
    {
        Printf("\narg:\n");
        Printf("   0 : disable crash detection\n");
        Printf("   1 : enable  crash detection\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    u4Value = (UINT32) StrToInt(szArgv[1]);
    
    _IMGRZ_SetCrashDetection(E_FIRST_RESIZER, u4Value);

    Printf("IMGRZ engine crash detection = %s \n", 
        _IMGRZ_GetCrashDetection(E_FIRST_RESIZER) ? "Enable" : "Disable");

    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdSlt
 *  imgrz.slt
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4SwEn = 0;   // c-model enabled
    UINT32 u4Loop = 10;

    if (szArgv == NULL)
    {
        return -1;
    }
    
    if (i4Argc == 2)
    {
        u4Loop = (UINT32) StrToInt(szArgv[1]);
    }

    if (i4Argc == 3)
    {
        u4Loop = (UINT32) StrToInt(szArgv[1]);
        u4SwEn = (UINT32) StrToInt(szArgv[2]);
    }

    i4Ret = _ImgrzSlt(u4Loop, u4SwEn);

    if ((INT32)E_CLIIMGRZ_OK == i4Ret)
    {
        Printf("IMGRZ SLT (%u loops)......... SUCCESS\n", u4Loop);
        return 0;
    }
    else
    {
        Printf("IMGRZ SLT (%u loops)......... FAILED\n", u4Loop);
        return -1;
    }
}


//-------------------------------------------------------------------------
/** _SLT_Imgrz
 *
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzSlt(UINT32 u4Loop, UINT32 u4SwEn)
{
    RZ_OSD_SCL_PARAM_SET_T rScaleParam;
    INT32 i4Flag = 0, i4FailCount;
    UINT32 i, j = 0;
    UINT32 u4FbmStartAddr, u4FbmTotalSize;
    UINT32 u4Pitch32bpp;
    UINT32 u4CanvasWidth, u4CanvasHeight;
    UINT32 u4Width, u4Height;
    UINT32 u4BytePerPix, u4Sum, u4SltAns;
    UINT32 u4CanvasBufSize, u4CanvasPitch;
    UINT8 *pu1HwSrc, *pu1HwDst, *pu1HwDst2, *pu1HwDst3;
    UINT8 *pu1Ptr;

	x_memset(&rScaleParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));
	
    u4SltAns = 0x100573fc;
    pu1HwDst3 = pu1HwDst2 = pu1HwDst = pu1HwSrc = NULL;
    u4BytePerPix = 4;   // 32-bpp (argb8888)
    
    u4CanvasWidth = 768;
    u4CanvasHeight = 512;
    u4CanvasPitch = (u4CanvasWidth * u4BytePerPix);
    
    u4Width = 720;
    u4Height = 496;

    u4CanvasBufSize = (u4CanvasWidth * u4CanvasHeight * u4BytePerPix);
    
    u4Pitch32bpp = (u4Width * u4BytePerPix);

    u4FbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);
    u4FbmTotalSize = DRVCUST_OptGet(eFbmMemSize);
    
    // assign 4 buffers (src, dst, dst2, dst3)
    pu1HwSrc = (UINT8 *)(u4FbmStartAddr);
    VERIFY(pu1HwSrc != NULL);
    
    pu1HwDst = (UINT8 *)(u4FbmStartAddr + u4CanvasBufSize);
    VERIFY(pu1HwDst != NULL);

    pu1HwDst2 = (UINT8 *)(u4FbmStartAddr + (2 * u4CanvasBufSize));
    VERIFY(pu1HwDst2 != NULL);

    pu1HwDst3 = (UINT8 *)(u4FbmStartAddr + (3 * u4CanvasBufSize));
    VERIFY(pu1HwDst3 != NULL);

    Printf("------------------- [GFX-SCalar SLT] --------------------\n");
    Printf("FBM start addr = 0x%08x\n", u4FbmStartAddr);
    Printf("FBM total size = %u (MB)\n", (u4FbmTotalSize / 1048576));
    Printf("SRC  start addr = 0x%08x\n", (UINT32)pu1HwSrc);
    Printf("DST  start addr = 0x%08x\n", (UINT32)pu1HwDst);
    Printf("DST2 start addr = 0x%08x\n", (UINT32)pu1HwDst2);
    Printf("DST3 start addr = 0x%08x\n", (UINT32)pu1HwDst3);

    // initialization
    GFX_Init();
    //GFXSC_Init();
    
    // clear 4 buffers
    GFX_SetDst(pu1HwSrc, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwDst2, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwDst3, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);

    GFX_Flush();
    GFX_Wait();

    i4Flag = 0;
    i4FailCount = 0;
    for (j = 0; j < u4Loop; j++)
    {
        // HW Action
        // 1. rect fill (src)
        GFX_SetDst(pu1HwSrc, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetColor(0xfa6359be);
        GFX_Fill(0, 0, u4Width, u4Height);
        GFX_SetColor(0xFFFFEBCD);    //blanchedalmond
        GFX_Fill(20,20,140,120);
        GFX_SetColor(0xFFFAEBD7);    //antiquewhite
        GFX_Fill(200, 20, 140, 120);
        GFX_SetColor(0xFF8A2BE2);    //blueviolet
        GFX_Fill(380, 20, 140, 120);
        GFX_SetColor(0xFF7FFFD4);    //aquamarine
        GFX_Fill(560, 20, 140, 120);
        GFX_SetColor(0xFFF0FFFF);    //azure
        GFX_Fill(20, 180, 140, 120);
        GFX_SetColor(0xFFF5F5DC);    //beige
        GFX_Fill(200, 180, 140, 120);
        GFX_SetColor(0xFFFFE4C4);    //bisque
        GFX_Fill(380, 180, 140, 120);
        GFX_SetColor(0xFFF0F8FF);    //aliceblue
        GFX_Fill(560, 180, 140, 120);
        GFX_SetColor(0xFFDEB887);    //burlywood
        GFX_Fill(20, 340, 140, 120);
        GFX_SetColor(0xFF00FFFF);    //aqua
        GFX_Fill(200, 340, 140, 120);
        GFX_SetColor(0xFFA52A2A);    //brown
        GFX_Fill(380, 340, 140, 120);
        GFX_SetColor(0xFF0000FF);    //blue
        GFX_Fill(560, 340, 140, 120);
        GFX_SetColor(0xFFFFFFFF);
        GFX_Fill(0, 0, 720, 2);
        GFX_Fill(0, 159, 720, 3);
        GFX_Fill(0, 319, 720, 3);
        GFX_Fill(0, 478, 720, 2);
        GFX_Fill(0, 0, 2, 480);
        GFX_Fill(179, 0, 3, 480);
        GFX_Fill(359, 0, 3, 480);
        GFX_Fill(539, 0, 3, 480);
        GFX_Fill(718, 0, 2, 480);
        GFX_Flush();
        GFX_Wait();

        //scale down testing
        IMGRZ_ReInit();
        rScaleParam.u4SrcBase = (UINT32)pu1HwSrc;
        rScaleParam.u4IsSrcDirCM = 1;
        rScaleParam.u4SrcCM = (UINT32)E_RZ_OSD_DIR_CM_AYCbCr8888;
        rScaleParam.u4SrcHOffset = 0;
        rScaleParam.u4SrcVOffset = 0;
        rScaleParam.u4SrcW = u4Width;
        rScaleParam.u4SrcH = u4Height;
        rScaleParam.u4TgBase = (UINT32)pu1HwDst;
        rScaleParam.u4TgCM = rScaleParam.u4SrcCM;
      
        rScaleParam.u4SrcBufLen = u4Width << 2;
        rScaleParam.u4TgBufLen = u4Width << 1;
        rScaleParam.u4TgHOffset = 0;
        rScaleParam.u4TgVOffset = 0;
        rScaleParam.u4TgW = u4Width / 2;
        rScaleParam.u4TgH = u4Height / 2;
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
        IMGRZ_Scale((void *)(&rScaleParam));   
        IMGRZ_Flush();
        IMGRZ_Wait();

        //scale up testing
        IMGRZ_ReInit();
        rScaleParam.u4SrcBase = (UINT32)pu1HwDst;
        rScaleParam.u4IsSrcDirCM = 1;
        rScaleParam.u4SrcCM = (UINT32)E_RZ_OSD_DIR_CM_AYCbCr8888;
        rScaleParam.u4SrcHOffset = 0;
        rScaleParam.u4SrcVOffset = 0;
        rScaleParam.u4SrcW = u4Width / 2;
        rScaleParam.u4SrcH = u4Height / 2;
        rScaleParam.u4TgBase = (UINT32)pu1HwDst2;
        rScaleParam.u4TgCM = rScaleParam.u4SrcCM;
      
        rScaleParam.u4SrcBufLen = u4Width << 1;
        rScaleParam.u4TgBufLen = u4Width << 2;
        rScaleParam.u4TgHOffset = 0;
        rScaleParam.u4TgVOffset = 0;
        rScaleParam.u4TgW = u4Width;
        rScaleParam.u4TgH = u4Height;
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
        IMGRZ_Scale((void *)(&rScaleParam));   
        IMGRZ_Flush();
        IMGRZ_Wait();

        // physical -> virtual address
        pu1Ptr = (UINT8 *)VIRTUAL((UINT32)pu1HwDst2);
        
        // check sum
        u4Sum = 0;
        for (i = 0; i < u4CanvasBufSize; i++)
        {
            u4Sum += ((UINT32)(*(pu1Ptr + i)));
        }

        Printf("[Run %d]\n", j);
        Printf("u4Sum    = 0x%08x\n", u4Sum);
        Printf("u4SltAns = 0x%08x\n", u4SltAns);

        if (u4Sum != u4SltAns)
        {
            i4Flag = 1;
            i4FailCount++;
        }
    } // ~for (j = 0; j < i4Loop; j++)

    if (i4Flag == 1)
    {
        Printf("[IMGRZ] Fail Count = %d\n", i4FailCount);
        return -1;
    }
    else
    {
        return (INT32)E_CLIIMGRZ_OK;
    }
}

#if 0
//-------------------------------------------------------------------------
/** _GetEnum
 *  
 */
//-------------------------------------------------------------------------
static INT32 _RzCmdGetEnum(const CHAR *szText, const IMGRZCMDTEXT2ENUM_T* prText2Enum)
{
    if ((NULL == szText) ||
        (NULL == prText2Enum))
    {
        return -(INT32)E_CLIIMGRZ_ERR_PARAM;
    }

    while (prText2Enum->szText)
    {
        if (x_strncmp(prText2Enum->szText, szText, x_strlen(prText2Enum->szText)) == 0)
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
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

CLI_MAIN_COMMAND_ITEM(IMGRZ)
{
    "imgrz",
    NULL,
    NULL,
    _arImgrzCmdTbl,
    "Imgrz command",
    CLI_SUPERVISOR
};
/*lint -restore */


