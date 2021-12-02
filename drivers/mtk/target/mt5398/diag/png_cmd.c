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
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_drv_cli.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_timer.h"
#include "x_assert.h"
#include "fbm_drvif.h"
#include "drvcust_if.h"

#include "jpg_if.h"
#include "img_lib_if.h"

#include "x_dram.h"
#include "srm_drvif.h"
#include "x_dram.h"
#include "x_ckgen.h"
#include "x_gfx.h"

#if  (defined(PNG_VFY_AUTO_TEST) || defined(PNG_DRV_FLOW_TEST))
#define CC_EMULATION
#endif

#if defined (CC_EMULATION) || defined (__MODEL_slt__)

#include "drv_t32.h"

extern UINT8 *_pbPngStrmIn;
extern UINT8 *_pbPngStrmOut;
extern UINT8 *_pbPngGoldenData;
extern UINT32 _u4PngFileListNum;
extern UINT32 _u4CurPngFileListNum;
extern BYTE _arbPngGoldenFile[512];
#ifdef PNG_DRV_FLOW_TEST
UCHAR pPngTestBuffer[24*1024*1024]={0};
#define VERIFY_DATA_ARGB8888 "argb8888.bin"
#define VERIFY_DATA_ARGB4444  "argb4444.bin"
#define VERIFY_DATA_ARGB1555 "argb1555.bin"
#define VERIFY_DATA_ARGB_CLUT8 "argbclut8.bin"
#define VERIFY_DATA_AYCBCR8888 "aycbcr8888.bin"
#endif
#endif

#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"


/******************************************************************************
* Macro            : helper macros to reduce code
******************************************************************************/
#define TRY3(X) \
    /* following do {} while (0) should be safe */ \
    LINT_SUPPRESS_NEXT_STATEMENT(717) \
    do \
    { \
        i4Ret = (X); \
        if (i4Ret) \
        { \
            goto _catch_err; \
        } \
    } while (0)

typedef enum _ENUM_CLIPNG_ERR_CODE_T
{
    E_CLIPNG_OK                = 0,
    E_CLIPNG_ERR_GENERAL    = 1000,
    E_CLIPNG_ERR_PARAM        = 1001,
    E_CLIPNG_ERR_MEM		=1002,
    // add here
    E_CLIPNG_UNKNOWN        = 1999
} ENUM_CLIPNG_ERR_CODE_T;


/******************************************************************************
* Test suit        : cli "jpg.t32test" data
******************************************************************************/
#ifndef PNG_SEMI_HOST
#define PNG_SEMI_HOST
#endif

#define NEW_VERIFY_FLOW 1

#if defined (CC_EMULATION) || defined (__MODEL_slt__)
#ifdef PNG_DRV_FLOW_TEST
UINT32 pAutoRunFileSize[]=
{
#include "png_filesize.h"
};

static UINT32 _u4fileSize = 0, _u4filled_size = 0;

static BOOL _fgPngInstance = FALSE;
static ENUM_IMG_DISP_COLOR_MODE _eDispMode = ARGB8888_DIRECT32;//GFX_COLORMODE_ARGB_D8888;
static GFX_COLORMODE_T _eDecMode = GFX_COLORMODE_ARGB_D8888;
static UINT32 _u4NtyValue = 0;

static HANDLE_T _hPngDecSema;
static UINT8* _pu1Bs = NULL;

static BOOL fgResize = FALSE; //if do resize
static UINT32 _u4PicWidth;
static UINT32 _u4PicHeight;
static UINT32 _u4TgtWidth;
static UINT32 _u4TgtHeight;
static UINT32 _u4PngPixelByte = 4;
// 5391
#endif
static UINT32 _u4SysClock;

#ifdef __MODEL_slt__
#include "x_fm.h"
static UINT32 _u4PngSlt = 0;
static CHAR _acPngFileName[50];
extern UINT32 _CalcPngCheckSum(void* pvData, UINT32 u4ByteLen);
#endif
/******************************************************************************
* Prototype        : cli "png" commands
******************************************************************************/
#ifdef PNG_VFY_AUTO_TEST
static INT32 _PngCmdPcAutoTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdTransSet(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdTransAlphaTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdClockTest(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _PngCmdInit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdAutoLoadData(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdSetParam(INT32 i4Argc, const CHAR ** szArgv);

#ifdef PNG_DRV_FLOW_TEST
static INT32 _PngCmdAutoSaveData(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdAutoDecode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdDecFlowAutoTest(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _PngDecodePicture(BOOL fgResize,UINT8* pu1Addr, UINT32 u4Size);
static INT32 _PngCmdAutoGetAlign(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _PngCmdSetTgWH(INT32 i4Argc, const CHAR ** szArgv);

void _PngFillBufCB(
    UINT32 u4ImgID,
    void *pv_tag,
    void *pv_data,
    UINT32 u4State);
static BOOL fgPngGoldenComp(UINT8 *pu1StrmOut, UINT8* pu1Golden,  UINT32 u4RealReadSize);
#endif

#ifdef __MODEL_slt__
static INT32 _CLI_CMD(const CHAR* szCmd, ...);
static INT32 _PngCmdSlt(INT32 i4Argc, const CHAR ** szArgv);
#endif
#endif//ifdef cc emulation
/******************************************************************************
* Variable        : cli "png" command table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(PNG)

static CLI_EXEC_T _arPngCmdTbl[] =
{
#if defined (CC_EMULATION) || defined (__MODEL_slt__)

    {"init",        "i",    _PngCmdInit, NULL,            "Png init,(1:OSD ARGB8888)", CLI_SUPERVISOR
    },
    {"AutoLoad",        "load",    _PngCmdAutoLoadData, NULL,        "Load file data from D:\\png_test\\default", CLI_SUPERVISOR},
    {"SetParam",      "set",	  _PngCmdSetParam, NULL, "Set Param ", CLI_SUPERVISOR},
#ifdef PNG_DRV_FLOW_TEST
    {"AutoSave",		"save",    _PngCmdAutoSaveData, NULL,		 "Auto save data output data to D:\\png_test\\dump\\upload", CLI_SUPERVISOR},
    {"decode",       "de",    _PngCmdAutoDecode, NULL,        "Decode a png file", CLI_SUPERVISOR},
    {"DecFlowAutoTest",        "run",      _PngCmdDecFlowAutoTest, NULL,         "Decode flow Auto Burning Test ", CLI_SUPERVISOR},
    {"cal",		NULL,	 _PngCmdAutoGetAlign, NULL, 	   "Calculate Align", CLI_SUPERVISOR},
    {"wh",		  NULL,    _PngCmdSetTgWH, NULL,		"set target width & height for imgrz output", CLI_SUPERVISOR},
#endif
#ifdef PNG_VFY_AUTO_TEST
    {"VerifyAutoTest",        "vfy",      _PngCmdPcAutoTest, NULL,        "Auto Burning Test ", CLI_SUPERVISOR},
    {"TransAlphaSet",      "tset",	  _PngCmdTransSet, NULL,		  "Set Transparant Alpha value ", CLI_SUPERVISOR},
    {"TransAlphaTest",      "tatest",	  _PngCmdTransAlphaTest, NULL, "Transparant Alpha test ", CLI_SUPERVISOR},
    {"ClockTest",      "clocktest",	  _PngCmdClockTest, NULL, "clock test ", CLI_SUPERVISOR},
#endif

#ifdef __MODEL_slt__
    {"slt",        NULL,    _PngCmdSlt, NULL,        "slt", CLI_SUPERVISOR},
#endif
#endif
    CLIMOD_DEBUG_CLIENTRY(PNG),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Png)
{
    "png",
    NULL,
    NULL,
    _arPngCmdTbl,
    "Png command",
    CLI_SUPERVISOR
};

/******************************************************************************
* Variable        : cli "png" enums
******************************************************************************/


/******************************************************************************
* Flag(s)        :
******************************************************************************/

/******************************************************************************
* Implementation        : helper functions for commands parsing
******************************************************************************/
#if defined (CC_EMULATION) || defined (__MODEL_slt__)
extern void PngAllocateMemory(UINT32 u4FbmId1,UINT32 u4FbmId2,UINT32 u4SrcBuf);

static INT32 _PngCmdAutoLoadData(INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR *szFileName=NULL;
    UINT32 u4FileSize;
    if (szArgv == NULL)
    {
        return 0;
    }

    if (i4Argc < 3)
    {
        Printf("arg: filename | filesize\n");
        return 0;
    }

    szFileName=(UCHAR *)(szArgv[1]);
    u4FileSize = (INT32)StrToInt(szArgv[2]);
#ifdef PNG_SEMI_HOST
    Printf("_PngCmdAutoLoadData\n");
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/%s\" 0x%08x /ny", "D:/png_test/default",szFileName,(UINT32)(_pbPngStrmIn)));
#endif
    HalFlushInvalidateDCacheMultipleLine((UINT32)(_pbPngStrmIn), (UINT32)(u4FileSize));

#ifdef PNG_DRV_FLOW_TEST
    _u4fileSize = u4FileSize;
#endif
    return 0;
}

#ifdef PNG_DRV_FLOW_TEST
static UCHAR*
_arOutFileName[]={VERIFY_DATA_ARGB8888,VERIFY_DATA_ARGB4444,VERIFY_DATA_ARGB1555,VERIFY_DATA_ARGB_CLUT8,VERIFY_DATA_AYCBCR8888};
static BOOL _fgIsEOF = FALSE, _fgFillBufEnd = FALSE;
static UINT32 _u4FillLen[] = {0, 0};
extern void vInitPngFileList(void);
extern INT32 i4LoadPngFileList(void);
extern void vParsePngFileList(UINT32 u4BufSizeLimit);
extern INT32 i4LoadPngFile(void);
extern void vParsePngGoldenFile(void);

static INT32 _PngCmdAutoSaveData(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4TgSize,i;
    UCHAR *prefix=NULL;

    if (i4Argc>1 && szArgv[1][0]=='-' && szArgv[1][1]=='p')
    {
        prefix=(UCHAR *)(szArgv[2]);
    }
    if ( fgResize)
    {
        u4TgSize = _u4TgtWidth*_u4TgtHeight;
        switch (_eDispMode)
        {
        case AYCBCR8888_DIRECT32:
            u4TgSize = u4TgSize<<2;
            i = 4;
            break;
        case ARGB8888_DIRECT32:
            u4TgSize = u4TgSize<<2;
            i = 0;
            break;
        case ARGB4444_DIRECT16:
            u4TgSize = u4TgSize<<1;
            i = 1;
            break;
        default :
            u4TgSize = u4TgSize<<2;
            i = 0;
            break;
        }
    }
    else
    {
        u4TgSize = _u4PicWidth*_u4PicHeight;
        switch (_eDecMode)
        {
        case GFX_COLORMODE_ARGB_D8888:
            u4TgSize = u4TgSize<<2;
            i = 0;
            break;
        case GFX_COLORMODE_ARGB_D4444:
            u4TgSize = u4TgSize<<1;
            i = 1;
            break;
        case GFX_COLORMODE_ARGB_D1555:
            u4TgSize = u4TgSize<<1;
            i = 2;
            break;
        case GFX_COLORMODE_ARGB_CLUT8:
            i = 3;
            break;
        default :
            u4TgSize = u4TgSize<<2;
            i = 0;
            break;
        }
    }

    HalFlushInvalidateDCacheMultipleLine((UINT32)(_pbPngStrmOut), u4TgSize);

    if (prefix)
    {
        Printf("Save File:%s/%s_%s\n","D:/png_test/dump/upload",prefix,_arOutFileName[i]);
#ifdef PNG_SEMI_HOST
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s_%s\" 0x%08x--0x%08x","D:/png_test/dump/upload",prefix,
                            _arOutFileName[i],(UINT32)(_pbPngStrmOut),(UINT32)(_pbPngStrmOut) + u4TgSize -1));
#endif
    }
    else
    {
        Printf("Save File:%s/%s\n","D:/png_test/dump/upload",_arOutFileName[i]);
#ifdef PNG_SEMI_HOST
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s\" 0x%08x--0x%08x","D:/png_test/dump/upload",
                            _arOutFileName[i],(UINT32)(_pbPngStrmOut),(UINT32)(_pbPngStrmOut) + u4TgSize -1));
#endif
    }

    return 0;
}

static INT32 _PngCmdAutoDecode(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc >= 2)
    {
        fgResize =(BOOL)StrToInt(szArgv[1]);
    }
    _PngDecodePicture(fgResize,_pbPngStrmIn,_u4fileSize);
    return E_CLIPNG_OK;
}
static INT32 _PngCmdDecFlowAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4FileSize;
    INT32 _i4PngFileSize ;
    INT32 u4NotCompareGolden;

    u4NotCompareGolden = (INT32)StrToInt(szArgv[1]);
    _u4SysClock    = (INT32)StrToInt(szArgv[2]);
    vInitPngFileList();

    i4FileSize = i4LoadPngFileList();
    vParsePngFileList(i4FileSize);
    Printf("_u4PngFileListNum = %d\n",_u4PngFileListNum);

    for (_u4CurPngFileListNum=0; _u4CurPngFileListNum<_u4PngFileListNum; _u4CurPngFileListNum++)
    {
        i4LoadPngFile();
        _i4PngFileSize = pAutoRunFileSize[_u4CurPngFileListNum];
        _PngDecodePicture(FALSE,_pbPngStrmIn,_i4PngFileSize);

        if (u4NotCompareGolden == 0)
        {
            vParsePngGoldenFile();

            Printf("Load Golden data %s\n",_arbPngGoldenFile);
            //compare golden file
#ifdef PNG_SEMI_HOST
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", _arbPngGoldenFile,(UINT32)(_pbPngGoldenData)));
#endif
            HalFlushInvalidateDCacheMultipleLine((UINT32)(_pbPngStrmOut),  (UINT32)(_u4PicWidth*_u4PicHeight*4));

            Printf( "[PNG_EMU]Read Golden file OK\n");
            if (fgPngGoldenComp((UINT8*)_pbPngStrmOut, (UINT8*)_pbPngGoldenData, 0))
            {
                Printf("[PNG_EMU]Comparegolden OK");
            }
        }
    }
    return 0;
}

static INT32 _PngCmdAutoGetAlign(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Number=0,i4Align=0;
    if (i4Argc<3)
    {
        Printf("arg:Num Align\n");
        return 0;
    }
    i4Number=(INT32)StrToInt(szArgv[1]);
    i4Align=(INT32)StrToInt(szArgv[2]);

    if (i4Align==0)
        return 0;

    Printf("Align=%d\n",i4Align*((i4Number+(i4Align-1))/i4Align));
    return 0;
}

void _PNG_SetYTgW(UINT32 u4Width)
{
    _u4TgtWidth =  u4Width;
}
void _PNG_SetYTgH(UINT32 u4Height)
{
    _u4TgtHeight = u4Height;
}

static INT32 _PngCmdSetTgWH(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
    if (szArgv == NULL)
    {
        return 0;
    }

    if (i4Argc < 3)
    {
        Printf("arg: width height");
    }

    u4Width  = StrToInt(szArgv[1]);
    u4Height = StrToInt(szArgv[2]);

    _PNG_SetYTgW(u4Width);
    _PNG_SetYTgH(u4Height);

    Printf("target width = %u, height = %u\n", u4Width, u4Height);
    return 0;
}

static INT32 _PngDecodePicture(BOOL fgResize,UINT8 *pu1PngAddr, UINT32 u4PngSize)
{
    UINT32 i4Ret = 0;
    HAL_RAW_TIME_T        rRawTime1;
    HAL_RAW_TIME_T        rRawTime2;
    HAL_TIME_T            rHalTime1;
    HAL_TIME_T            rHalTime2;
    HAL_TIME_T            rHalTimeDuration;
    UINT8             *pu1PngStream = NULL;
    IMG_PARAM_T rInfo;
    IMG_REGION_T rDecOut;
    // IMG_BUF_PARAM_T rCmd;
    UINT32 u4ImageSize;
    static ENUM_IMG_ROTATE_PHASE eRotatePhase = CLOCKWISE_0;
    static BOOL bKeepRatio = FALSE;

    //---------- prepare memory buffer for input image and data structure
    pu1PngStream = (pu1PngAddr);//pu1JpegAddr;
    u4ImageSize = u4PngSize;
    HAL_GetRawTime(&rRawTime1);

    //----------start decoding
    _u4PicWidth = 0;
    _u4PicHeight = 0;
    _u4PicWidth += ((UINT32)(pu1PngStream[16])) << 24;
    _u4PicWidth += ((UINT32)(pu1PngStream[17])) << 16;
    _u4PicWidth += ((UINT32)(pu1PngStream[18])) << 8;
    _u4PicWidth += ((UINT32)(pu1PngStream[19]));
    _u4PicHeight += ((UINT32)(pu1PngStream[20])) << 24;
    _u4PicHeight += ((UINT32)(pu1PngStream[21])) << 16;
    _u4PicHeight += ((UINT32)(pu1PngStream[22])) << 8;
    _u4PicHeight += ((UINT32)(pu1PngStream[23]));
    //image library API

    //Printf("Png width = %d,height = %d\n",_u4PicWidth,_u4PicHeight);
    rInfo.u8BufSize = u4ImageSize;
    if (fgResize)
    {
        rInfo.rDstRegion.u4Width = _u4TgtWidth;//_prOsd->u4Width;
        rInfo.rDstRegion.u4Height = _u4TgtHeight;//_prOsd->u4Height;
    }
    else
    {
        rInfo.rDstRegion.u4Width = _u4PicWidth;//_prOsd->u4Width;
        rInfo.rDstRegion.u4Height = _u4PicHeight;//_prOsd->u4Height;
    }
    rInfo.rDstRegion.u4StartX = 0;
    rInfo.rDstRegion.u4StartY = 0;
    if (fgResize)
    {
        if (_eDispMode == ARGB4444_DIRECT16)
        {
            rInfo.rDstRegion.u4Pitch = (_u4TgtWidth<<1);
        }
        else
        {
            rInfo.rDstRegion.u4Pitch = (_u4TgtWidth<<2);
        }
    }
    else
    {
        if (_eDispMode == ARGB4444_DIRECT16)
        {
            rInfo.rDstRegion.u4Pitch = (_u4PicWidth<<1);
        }
        else
        {
            rInfo.rDstRegion.u4Pitch = (_u4PicWidth<<2);
        }
    }
    rInfo.rDstRegion.u4CbCrOffset = 0;
    rInfo.rSrcRegion.u4SrcStartX =0;
    rInfo.rSrcRegion.u4SrcStartY = 0;
    rInfo.rSrcRegion.u4SrcWidth = _u4PicWidth;
    rInfo.rSrcRegion.u4SrcHeight = _u4PicHeight;
    rInfo.eDispColorMode = _eDispMode;
    rInfo.u4DecColorMode = _eDecMode;
    rInfo.eRotateDegree = eRotatePhase;
    rInfo.bKeepRatio = bKeepRatio;

    rInfo.prDecOut = &rDecOut;

    _pu1Bs = pu1PngStream;
    _u4fileSize = u4ImageSize;
    _u4filled_size = 0;
    rInfo.pu1InBuffer = NULL; //use internal ring buffer
    rInfo.rDstRegion.pu1Addr = (UINT8 *)_pbPngStrmOut;

    TRY3((INT32)IMG_Init(RING_IN_SINGLE_OUT, NULL, 0, _PngFillBufCB));
    IMG_Set_ImgSize(u4ImageSize);
    TRY3((INT32)IMG_Decode(rInfo));
    //VERIFY (x_sema_lock(_hPngDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    do
    {
        VERIFY (x_sema_lock(_hPngDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        if (_fgFillBufEnd)
        {
            if (_u4FillLen[0]==0)
            {
                IMG_SetFillBsBufEnd(_u4FillLen[1], 0, _fgIsEOF);
            }
            else
            {
                IMG_SetFillBsBufEnd(_u4FillLen[0], _u4FillLen[1], _fgIsEOF);
            }
            _u4FillLen[0] = 0;
            _u4FillLen[1] = 0;
            _fgFillBufEnd = FALSE;
            _fgIsEOF = FALSE;
        }
    }
    while (_u4NtyValue == JPG_HW_NFY_FILL_BUF);

    bKeepRatio = ~bKeepRatio;

    HAL_GetRawTime(&rRawTime2);
    HAL_RawToTime(&rRawTime1, &rHalTime1);
    HAL_RawToTime(&rRawTime2, &rHalTime2);
    HAL_GetDeltaTime(&rHalTimeDuration, &rHalTime1, &rHalTime2);

    Printf("Decode duration : %u.%06u seconds\n",
           rHalTimeDuration.u4Seconds,
           rHalTimeDuration.u4Micros);
    //-----------finish decoding
    if (_u4NtyValue == JPG_HW_NFY_FINISHED)
    {
        Printf("Decoded successully.\n");
    }
    else
    {
        Printf("Decoded error.\n");
    }
    Printf("done.\n");
_catch_err:

    IMG_Release();
    return i4Ret;
}

void _PngFillBufCB(
    UINT32 u4ImgID,
    void *pv_tag,
    void *pv_data,
    UINT32 u4State)
{
    IMG_FILL_BUF_MW_T *prBufInfo = (IMG_FILL_BUF_MW_T *)pv_data;
    UINT32 u4Addr1;
    UINT32 u4Required_len1;
    // UINT32 u4TrsnId;
    UINT32 u4curFilledSize1=0;
    _u4NtyValue = u4State;
    if (u4State == JPG_HW_NFY_PROGRESS)
    {
        return;	//ignore the progress notify
    }
    else if ((u4State == JPG_HW_NFY_FINISHED) ||(u4State == JPG_HW_NFY_ERROR))
    {
        VERIFY (x_sema_unlock(_hPngDecSema) == OSR_OK);
        return;
    }
    if (prBufInfo==NULL)
    {
        return;
    }

    //u4TrsnId = prBufInfo->ui4_trsn_id;
    u4Addr1 = (UINT32)prBufInfo->pv_start_add;
    u4Required_len1 = prBufInfo->ui4_required_len;
    if (prBufInfo->b_reset_pos)
    {
        _u4filled_size = prBufInfo->ui4_position;
        _pu1Bs = _pbPngStrmIn + prBufInfo->ui4_position;
    }

    if ((_u4filled_size + u4Required_len1) > _u4fileSize)
    {
        //Printf("_u4fileSize = 0x%08x,_u4filled_size = 0x%08x\n",_u4fileSize,_u4filled_size);
        u4curFilledSize1 = _u4fileSize - _u4filled_size;
        _fgIsEOF = TRUE;
    }
    else
    {
        u4curFilledSize1 = u4Required_len1;
    }

    x_memcpy((void *)u4Addr1, (void *)_pu1Bs, u4curFilledSize1);

    HalFlushInvalidateDCacheMultipleLine((UINT32)(u4Addr1), (UINT32)(u4curFilledSize1));

    _u4filled_size += u4curFilledSize1;
    _pu1Bs += u4curFilledSize1;
    //Printf("_u4filled_size = 0x%08x\n",_u4filled_size);
    _u4FillLen[prBufInfo->ui4_trsn_id] = u4curFilledSize1;
    if (prBufInfo->ui4_trsn_id == 1)
    {
        _fgFillBufEnd = TRUE;
    }
    //prBufInfo->bEOF = isEOF;
    // IMG_SetFillBsBufEnd(u4curFilledSize1, 0, isEOF);
    VERIFY (x_sema_unlock(_hPngDecSema) == OSR_OK);
    // clean lint warning
    UNUSED(u4ImgID);
    if (pv_tag == NULL)
    {
        return;
    }
    else
    {
        *((UINT8 *)pv_tag) = 0;
    }
}

BOOL fgPngGoldenComp(UINT8 *pu1StrmOut, UINT8* pu1Golden,  UINT32 u4RealReadSize)
{
    INT32 i = 0;
    INT32 j = 0;
    //UINT32 u4CurrentX, u4CurrentY;
    UINT32 u4CompDataPixNum = 0;

    if (0)
    {
        for (i=0; i < u4RealReadSize; i++)
        {
            if (pu1StrmOut[i] != pu1Golden[i])
            {
                Printf( "[PNG_EMU] compare fail at %d\n", i);
                return FALSE;
            }
        }
    }
    else
    {
        for (i=0; i<(_u4PicHeight*_u4PicWidth); i++)
        {
            //u4CurrentX = (i) % _u4PicWidth;
            //u4CurrentY = (i) / _u4PicWidth;

            for (j=0; j<_u4PngPixelByte; j++)
            {
                if (pu1StrmOut[i*_u4PngPixelByte+j] != pu1Golden[i*_u4PngPixelByte+j])
                {
                    Printf( "[PNG_EMU] compare fail at %d\n", i);
                    return FALSE;
                }
            }
        }
    }
    Printf( "[PNG_EMU] compare ok! at %d, %d, Compared Pix Total Number = %d \n",  i,j,u4CompDataPixNum);
    return TRUE;
}
#endif

typedef enum
{
    PNG_VERIFY_PARAM_MULTI_SET=0,
    PNG_VERIFY_PARAM_DEC_MODE, // 1
    PNG_VERIFY_PARAM_DISP_MODE, // 2
    PNG_VERIFY_PARAM_CLK_SET,  // 3
    PNG_VERIFY_PARAM_FILELIST_SIZE, // 4
    PNG_VERIFY_PARAM_FG_CHECK_SUM, //5
    PNG_VERIFY_PARAM_NORMAL_TEST, //6
    PNG_VERIFY_PARAM_RESUME_TEST, //7
    PNG_VERIFY_PARAM_COLOR_FMT_TEST, //8
    PNG_VERIFY_PARAM_CROP_TEST, //9
    PNG_VERIFY_PARAM_FG_RESUME_ENABLE, // 10
    PNG_VERIFY_PARAM_MULTI_INIT=100,
    PNG_VERIFY_PARAM_SHOW_HELP=200
}E_PNG_VERIFY_PARAM;

extern UINT32 u4PngFileListSize;

#ifdef PNG_VFY_AUTO_TEST
static BOOL fgCropTest = TRUE ;
static BOOL fgColorfmtTest = TRUE ;
static BOOL fgSrcDstResumeTest = TRUE;
static BOOL fgNormalTest = TRUE;
extern BOOL fgResumeEnable ;
extern BOOL  fgCompareChecksum;
#endif
static INT32 _PngCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4FbmId=FBM_POOL_TYPE_B2R,u4FbmId2=FBM_POOL_TYPE_SCPOS_MAIN;

    if (szArgv == NULL)
    {
        return 0;
    }
    if (i4Argc > 1)
    {
        u4PngFileListSize = StrToInt(szArgv[1]);
        Printf("u4PngFileListSize = %d\n",u4PngFileListSize);
        if (u4PngFileListSize <= 0)
        {
            return (INT32)E_CLIPNG_ERR_GENERAL;
        }

        if (i4Argc > 2)
        {
            u4FbmId=StrToInt(szArgv[2]);
        }
        else
        {
            UINT32 u4FbmAddr, u4FbmSize;
            u4FbmAddr = DRVCUST_OptGet(eFbmMemAddr);
            u4FbmSize = DRVCUST_OptGet(eFbmMemSize);
            Printf("u4FbmAddr = 0x%08x,u4FbmSize = 0x%08x\n",u4FbmAddr,u4FbmSize);
#ifdef PNG_DRV_FLOW_TEST
            u4FbmAddr = u4FbmAddr + u4FbmSize - 8*1024*1024 - 8*1024*1024;
            u4FbmId = u4FbmAddr;
            u4FbmId2 = u4FbmId + 8*1024*1024;
#else
            u4FbmAddr = u4FbmAddr + u4FbmSize -64*1024*1024;
            u4FbmId2 = u4FbmAddr;
#endif
        }
    }
    else
    {
        LOG(0, "usage:\n\t png.i {filelist size,TagFbmId}\n");
        return (INT32)E_CLIPNG_OK;
    }

#ifdef PNG_DRV_FLOW_TEST
    if (!_fgPngInstance)
    {
        _fgPngInstance = TRUE;
        IMG_Config(IMAGE_PNG);
        _eDispMode = ARGB8888_DIRECT32;//GFX_COLORMODE_ARGB_D8888;
        _eDecMode = GFX_COLORMODE_ARGB_D8888;
        /*
            _prOsd = (DRV_OSD_T *)IMG_OSD_create(14); //OSD_CM_ARGB8888_DIRECT32
            if (_prOsd == NULL)
            {
                LOG(0, "OSD create failed\n");
                return (INT32)E_CLIPNG_ERR_GENERAL;
            }*/
        if (IMG_Lib_Init()!= E_IMG_OK)
        {
            return (INT32)E_CLIPNG_ERR_GENERAL;
        }
        VERIFY (x_sema_create(&_hPngDecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }
    UNUSED(szArgv);

    _PNG_SetYTgW(320);
    _PNG_SetYTgH(320);
    PngAllocateMemory(u4FbmId,u4FbmId2,(UINT32)pPngTestBuffer);
#else
    PngAllocateMemory(u4FbmId,u4FbmId2,(UINT32)NULL);
#endif
    return (INT32)E_CLIPNG_OK;
}

INT32 _PngVerifyCmdGetParam(INT32 i4ParamType)
{
    switch (i4ParamType)
    {
#ifdef PNG_DRV_FLOW_TEST
    case PNG_VERIFY_PARAM_DEC_MODE:
        return (INT32)_eDecMode;
    case PNG_VERIFY_PARAM_DISP_MODE:
        return (INT32)_eDispMode;
#endif
    case  PNG_VERIFY_PARAM_CLK_SET:
        return (INT32)_u4SysClock;
    case PNG_VERIFY_PARAM_FILELIST_SIZE:
        return (INT32)u4PngFileListSize;
#ifdef PNG_VFY_AUTO_TEST
    case PNG_VERIFY_PARAM_FG_CHECK_SUM:
        return (INT32)fgCompareChecksum;
    case PNG_VERIFY_PARAM_NORMAL_TEST:
        return (INT32)fgNormalTest;
    case    PNG_VERIFY_PARAM_RESUME_TEST:
        return (INT32)fgSrcDstResumeTest;
    case    PNG_VERIFY_PARAM_COLOR_FMT_TEST:
        return (INT32)fgColorfmtTest;
    case   PNG_VERIFY_PARAM_CROP_TEST:
        return (INT32)fgCropTest;
    case PNG_VERIFY_PARAM_FG_RESUME_ENABLE:
        return (INT32)fgResumeEnable;
#endif
    default :
        break;
    }
    return 0;
}

static INT32 _PngCmdSetParam(INT32 i4Argc, const CHAR ** szArgv)
{
    E_PNG_VERIFY_PARAM e_PngParam=PNG_VERIFY_PARAM_MULTI_INIT;
    if (i4Argc>1)
        e_PngParam=(E_PNG_VERIFY_PARAM)StrToInt(szArgv[1]);
    else
        e_PngParam=PNG_VERIFY_PARAM_SHOW_HELP;
    switch (e_PngParam)
    {
#ifdef PNG_DRV_FLOW_TEST
    case PNG_VERIFY_PARAM_DEC_MODE:
    {
        switch (StrToInt(szArgv[2]))
        {
        case 0:
            _eDecMode = GFX_COLORMODE_ARGB_D8888;
            break;
        case 1:
            _eDecMode = GFX_COLORMODE_ARGB_D4444;
            break;
        case 2:
            _eDecMode = GFX_COLORMODE_ARGB_D1555;
            break;
        case 3:
            _eDecMode = GFX_COLORMODE_ARGB_CLUT8;
            break;
        default:
            break;
        }
    }
    break;
    case PNG_VERIFY_PARAM_DISP_MODE:
    {
        switch (StrToInt(szArgv[2]))
        {
        case 0:
            _eDispMode = AYCBCR8888_DIRECT32;
            break;
        case 1:
            _eDispMode = ARGB8888_DIRECT32;
            break;
        case 2:
            _eDispMode = ARGB4444_DIRECT16;
            break;
        default:
            break;
        }
    }
    break;
#endif
    case  PNG_VERIFY_PARAM_CLK_SET:
        _u4SysClock = (INT32)StrToInt(szArgv[2]);
        break;
    case  PNG_VERIFY_PARAM_FILELIST_SIZE:
        u4PngFileListSize = (INT32)StrToInt(szArgv[2]);
        break;
#ifdef PNG_VFY_AUTO_TEST
    case PNG_VERIFY_PARAM_FG_CHECK_SUM:
        fgCompareChecksum = (INT32)StrToInt(szArgv[2]);
        break;
    case PNG_VERIFY_PARAM_NORMAL_TEST:
        fgNormalTest =  (INT32)StrToInt(szArgv[2]);
        break;
    case    PNG_VERIFY_PARAM_RESUME_TEST:
        fgSrcDstResumeTest =  (INT32)StrToInt(szArgv[2]);
        break;
    case    PNG_VERIFY_PARAM_COLOR_FMT_TEST:
        fgColorfmtTest =  (INT32)StrToInt(szArgv[2]);
        break;
    case   PNG_VERIFY_PARAM_CROP_TEST:
        fgCropTest =  (INT32)StrToInt(szArgv[2]);
        break;
    case PNG_VERIFY_PARAM_FG_RESUME_ENABLE:
        fgResumeEnable = (INT32)StrToInt(szArgv[2]);
        break;
#endif
    case PNG_VERIFY_PARAM_SHOW_HELP:
#ifdef PNG_DRV_FLOW_TEST
        Printf(" 1-->SetDecMode(%d)(0:ARGB8888 1:ARGB4444 2:ARGB15555 3:ARGBCLUT \n ",_eDecMode);
        Printf(" 2-->SetDisplayMode(%d)(0:AYCbCr8888 1:ARGB8888 2:ARGB4444 \n ",_eDispMode);
#endif
        Printf("3 -->Set Clock (%d) ({27,0},{288,1},{324,2},{240,3},{259,4},{337,5},{320,6}, {225,7})\n",_u4SysClock);
        Printf("4 -->Set PngFileList (%d)\n",u4PngFileListSize);
#ifdef PNG_VFY_AUTO_TEST
        Printf("5 --> Set fgCompareChecksum(%d)\n",fgCompareChecksum);
        Printf("6 --> Set fgNormalTest (%d)\n",fgNormalTest);
        Printf("7 --> Set fgSrcDstResumeTest (%d)d\n",fgSrcDstResumeTest);
        Printf("8 --> Set fgColorfmtTest (%d)\n",fgColorfmtTest);
        Printf("9 --> Set fgCropTest (%d)\n",fgCropTest);
        Printf("10 --> Set fgResumeEnable  (%d)d\n",fgResumeEnable);
#endif
        Printf("100-->Init\n200->Help\n");
        Printf("-------------------------------------------------------\n");
        break;
    default:
        break;
    }
    return 0;
}

#ifdef PNG_VFY_AUTO_TEST
extern void vPngVerify(const CHAR** aszArgv);
extern void vPngTransAlphaTest(void);

static INT32 _PngCmdPcAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
    vPngVerify(szArgv);
    return 0;
}

UINT8 u1AlphaMatch,u1AlphaUnMatch = 0xFF;
UINT16 u2RKey,u2GKey,u2BKey,u2GrayKey;
UINT8 u1BGR,u1BGG,u1BGB;
BOOL fgTransEn,fgTransOutEn,fgTrans16BitEn,fgTransOrgAlphaEn;
static INT32 _PngCmdTransSet(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc > 4)
    {
        u2RKey = (UINT16)StrToInt(szArgv[1]);
        u2GKey = (UINT16)StrToInt(szArgv[2]);
        u2BKey = (UINT16)StrToInt(szArgv[3]);
        u2GrayKey = (UINT16)StrToInt(szArgv[4]);
        Printf("New Match color(rgbg) = (0x%x, 0x%x, 0x%x, 0x%x)\n", u2RKey, u2GKey, u2BKey, u2GrayKey);
        if (i4Argc > 7)
        {
            u1BGR = (UINT8)StrToInt(szArgv[5]);
            u1BGG = (UINT8)StrToInt(szArgv[6]);
            u1BGB = (UINT8)StrToInt(szArgv[7]);
            Printf("New background color(rgb) = (0x%x, 0x%x, 0x%x)\n", u1BGR, u1BGG, u1BGB);
        }
        if (i4Argc > 9)
        {
            u1AlphaMatch = (UINT8)StrToInt(szArgv[8]);
            u1AlphaUnMatch = (UINT8)StrToInt(szArgv[9]);
            Printf("New alpha(match, unmatch) = (0x%x, 0x%x)\n", u1AlphaMatch, u1AlphaUnMatch);
        }
    }
    else
    {
        LOG(0, "usage:\n\t png.set {Rkey,Gkey,Bkey,Graykey,BG_R,BG_G,BG_B,Alpha_match,Alpha_unmatch}\n");
        return (INT32)E_CLIPNG_OK;
    }
    return 0;
}

static INT32 _PngCmdTransAlphaTest(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc > 4)
    {
        fgTransEn = (BOOL)StrToInt(szArgv[1]); // 1: match key RGB, output BG_R,BG_G,BG_B,ALPHA_MATCH, unmatch output orig RGB
        fgTransOutEn = (BOOL)StrToInt(szArgv[2]); // 1: match key RGB, do not output anything
        fgTransOrgAlphaEn = (BOOL)StrToInt(szArgv[3]); // only effect on alpha file1: output orig alpha whether match key
        fgTrans16BitEn = (BOOL)StrToInt(szArgv[4]); // only effect on 16bit file 1: match 16 bit,  0:match 8 bit
        fgResumeEnable  = (BOOL)StrToInt(szArgv[5]);
        fgCompareChecksum =  (BOOL)StrToInt(szArgv[6]);
    }
    else
    {
        LOG(0, "usage:\n\t png.test {fgTransEn,fgTransOutEn,fgTrans16BitEn,fgTransOrgAlphaEn}\n");
        return (INT32)E_CLIPNG_OK;
    }
    vPngTransAlphaTest();
    return 0;
}

static INT32 _u4PNGCLKTable[8][2]={{27,0},{288,1},{324,2},{240,3},{259,4},{337,5},{320,6}, {225,7}};
static INT32 _PngCmdClockTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i = 0;
    INT32 j = 0;
    INT32 u4start,u4end,u4Num;
    INT32 u4Type;
    INT32 u4Clock1,u4Clock2,u4Clock3;
    if (i4Argc > 2)
    {
        u4Type = (INT32)StrToInt(szArgv[1]);
        if (u4Type == 0)
        {
            u4start = (INT32)StrToInt(szArgv[2]);
            u4end = (INT32)StrToInt(szArgv[3]);
            u4Num = (INT32)StrToInt(szArgv[4]);
            if ((u4end > 8) || (u4start < 0) || (u4start > u4end) )
            {
                Printf("Input parameter error!!");
                return 0;
            }
        }
        else if (u4Type == 1)
        {
            u4Num = (INT32)StrToInt(szArgv[2]);
            u4Clock1 = (INT32)StrToInt(szArgv[3]);
            u4Clock2 = (INT32)StrToInt(szArgv[4]);
            u4Clock3 = (INT32)StrToInt(szArgv[5]);
        }

    }
    else
    {
        u4start = 0;
        u4end = 8;
        u4Num = 2;
    }

    if (u4Type == 0)
    {
        for (j = 0; j < u4Num; j++)
        {
            for (i = u4start; i < u4end; i++)
            {
                //IO_WRITE32(CKGEN_BASE, 0x740, _u4PNGCLKTable[i][1]);
                Printf("png %d clock = %d MHZ",i,_u4PNGCLKTable[i][0]);
                _u4SysClock = i;
                vPngVerify(szArgv);
            }
        }
    }
    else if (u4Type == 1)
    {

        for (j = 0; j < u4Num; j++)
        {
            if (i4Argc >=4)
            {

                Printf("png %d clock = %d MHZ",u4Clock1,_u4PNGCLKTable[u4Clock1][0]);
                _u4SysClock = u4Clock1;
                vPngVerify(szArgv);
            }
            if (i4Argc >=5)
            {

                Printf("png %d clock = %d MHZ",u4Clock2,_u4PNGCLKTable[u4Clock2][0]);
                _u4SysClock = u4Clock2;
                vPngVerify(szArgv);
            }
            if (i4Argc >=6)
            {

                Printf("png %d clock = %d MHZ",u4Clock3,_u4PNGCLKTable[u4Clock3][0]);
                _u4SysClock = u4Clock3;
                vPngVerify(szArgv);
            }
        }
    }
    return 0;
}
#endif
#ifdef __MODEL_slt__
static INT32 _CLI_CMD(const CHAR* szCmd, ...)
{
    CHAR szBuf[256];
    VA_LIST t_ap;

    VA_START(t_ap, szCmd);
    x_vsprintf(szBuf, szCmd, t_ap);
    VA_END(t_ap);

    return CLI_Parser(szBuf);
}
static INT32 _PngCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SltAns;
    UINT32 u4PngSize;
    HANDLE_T hImgFile;
    UINT32 u4Read;

    if (i4Argc < 4)
    {
        Printf("png.slt <filename> <filesize> <checksum>\n");
        x_sprintf(_acPngFileName, "/mnt/usb_0/basi6a08.png");
        u4PngSize = (UINT32)361;
        u4SltAns = (UINT32)0xb59e7ae0;
    }
    else
    {
        x_sprintf(_acPngFileName, "/mnt/usb_0/%s", szArgv[1]);
        u4PngSize = (UINT32)StrToInt(szArgv[2]);
        u4SltAns = (UINT32)StrToInt(szArgv[3]);
    }

    _u4PngSlt = 0;
    _CLI_CMD("fbm.i");
    _CLI_CMD("png.i 1");

    if (_pbPngStrmIn == NULL)
    {
        LOG(0, "Input buffer have not been inited\n");
        return -(INT32)E_CLIPNG_ERR_MEM;
    }
    if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, _acPngFileName, FM_READ_ONLY, 0777, FALSE, &hImgFile))
    {
        LOG(0, "Open file fail!\n");
        return -1;
    }
    if (FMR_OK != x_fm_read(hImgFile, _pbPngStrmIn, u4PngSize, &u4Read))
    {
        LOG(0, "Read file fail!\n");
        return -1;
    }
    x_fm_close(hImgFile);

    _PngDecodePicture(FALSE,_pbPngStrmIn,u4PngSize);

    _u4PngSlt = _CalcPngCheckSum((void*)_pbPngStrmOut,
                                 _u4PicHeight*_u4PicWidth*_u4PngPixelByte);
    if (_u4PngSlt != u4SltAns)
    {
        Printf("[Png] Fail Count = %d\n", (INT32)_u4PngSlt);
        return -1;
    }
    Printf("[Png] %x , SLT PNGOK!\n", (INT32)u4SltAns);
    return 0;
}
#endif

#endif
