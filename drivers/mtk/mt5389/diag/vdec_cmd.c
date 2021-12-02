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
#include "x_assert.h"
#include "x_ckgen.h"

#include "vdec_if.h"
#include "dmx_if.h"
#include "vdec_drvif.h"
#include "mpv_if.h"
#include "drv_dram.h"
#include "x_timer.h"

#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif

#define DEFINE_IS_LOG VDEC_IsLog
#include "x_debug.h"
#include "x_mid.h"
#include "x_bim.h"

//#define USB_RW_SUPPORT
//#define CC_EMULATION
//#define CC_VP8_EMULATION
#ifdef CC_VP8_EMULATION
#include "vdec_vp8webp_if.h"
#endif // CC_VP8_EMULATION

//#define CC_EMULATION_ONCE
//#define CC_SUPPORT_LOG_INFO
//#define CC_WMV_EMULATION

#ifdef CC_EMULATION_CRC
#define CC_EMULATION_COM_CRC
#ifndef CC_EMULATION_COM_CRC
//#define CC_EMULATION_GEN_CRC
#endif

//#define CC_VP8_EMULATION
//#define CC_VP8_BIGWEBM_EMULATION
#ifdef CC_VP8_EMULATION
#include "vdec_vp8webp_if.h"
#endif // CC_VP8_EMULATION
#endif
#define VDEC_FMT_WEBP (30)

//#define CC_64x32BLK_CMP //Enable if DDR3

//for emulation
#ifdef CC_EMULATION
#ifdef CC_DRIVER_DEMO
#include "drv_t32.h"
#endif
//#include "vdec_ide.h"
#include <string.h>

//#define MEMRW_TEST

#define CC_RV_EMULATION
#ifdef CC_RV_EMULATION
#if defined(CC_MT5389)
#define RV_TRACKING
#endif
#endif

#endif

#ifdef VDEC_TIME_PROFILE
#include "x_util.h"
#endif
/******************************************************************************
* Macro            : helper macros to reduce code
******************************************************************************/
//#define VP6_MID_GOLDEN
//#define VP6_MID_VFIFO

#define STRESS_TESTING
//#define RR_TEST
//#define IDE_READ_SUPPORT
//#define IDE_WRITE_SUPPORT

//wmv spork
//#define MEM_TEST
#define WMV_PIC_HEADER_LEN (4)
#define DO_COMPARE_FRAME
#define DUMP_MEM
#define START_COMPARE_FRAME (0)//440//532
#define END_COMPARE_FRAME (9999)
#define WMV_PROFILING
//#define CC_MPEG4_VERIFICATION
#if defined(CC_MPEG4_VERIFICATION)
#define FILE_INFO_SIZE 0x100000
#elif defined(CC_RV_EMULATION)
#define FILE_INFO_SIZE 0x500000
#else
#define FILE_INFO_SIZE 0x40000
#endif

#if defined(CC_RV_EMULATION)
#define CHKSUM_INFO_SIZE 0x900000
#else
#define CHKSUM_INFO_SIZE 0x100000
#endif
#define REGDUMP_SIZE     (100*1024)
#if 1
#if defined(CC_VP8_BIGWEBM_EMULATION)
#define GOLDEN_SIZE  0x1100000
#elif defined(CC_RV_EMULATION)
#define GOLDEN_SIZE (2048 * 1152 * 3 / 2)
#else
#define GOLDEN_SIZE (1920 * 1088)
#endif
#else //high resolution
#define GOLDEN_SIZE (3840 * 1088)
#endif
#define MAX_FILE_NUM 2000
#define MAX_FILENAME_LEN 256

#ifdef MEM_TEST
#define VFIFO_S 0x179c000    //0x2e24000    //should get from dmx fbm
#define VFIFO_E 0x679c000    //80M
#else
//#define VFIFO_S 0x2ff0000    //0x2e80000    //0x2e24000    //should get from dmx fbm
//#define VFIFO_E 0x7ff0000    //0x7ffc000    //81.48M

#define VFIFO_S 0xE000000
#define VFIFO_E (VFIFO_S+0x1E00000)

// normal webm
//#define VFIFO_S 0x0C000000//0xE000000
//#define VFIFO_E (VFIFO_S+0x03E00000)

// High resolution webm & webp
//#define VFIFO_S 0xF000000
//#define VFIFO_E (VFIFO_S+0x00E00000)

//#define VFIFO_S 0x3ff0000      //539x
//#define VFIFO_E 0x7ff0000      //539x
#endif

#define VFIFO_SZ 0x200000

#define NO_PIC 0
#define TOP_FIELD 1
#define BOTTOM_FIELD 2
#define FRAME 3
#define CS_NUM 4096    //frame number
#define CS_PER_FRM 4 //Y1, Y2, C1, C2
#define VDEC_VERIFY_SET_NUM (16)
typedef enum
{
    DISP_IDX = 0,
    BS_TYPE,
    RRMODE,
    OUTPUT_DISP_ORDER,
    REPEAT_FRM,
    ECC_MODE,
    ECC_TYPE,
    DRAM_BANDWITH_SIMU,
    LOG_SIMU_PAT,
    WAIT_FB_TIME,
    AV_SYNC_INFO,
    TIME_PROFILE,
    TIME_PROFILE2,
    TIME_PROFILE3,
    MM_PARAM,
    SEQ_HDR_INFO,
    RECORD_CRC,
    CRC_CHK
}   H264_PARAM_T;

//for emulation checksum value
typedef struct
{
    UINT32 u4ShftAddY1[CS_NUM];
    UINT32 u4ShftAddY2[CS_NUM];
    UINT32 u4ShftAddC1[CS_NUM];
    UINT32 u4ShftAddC2[CS_NUM];
} VDEC_CHKSUM_INFO_T;

typedef struct
{
  UCHAR rPatternFile[MAX_FILENAME_LEN];
  UCHAR rGoldenFile[MAX_FILENAME_LEN];
  UINT32 u4StartFile;
  UINT32 u4Enable;
}VDEC_VERIFY_SET;
typedef struct
{
  UINT32 u4ColorType;
  UINT32 u4DecMode;
  UINT32 u4TargW;
  UINT32 u4TargH;
  UINT32 u4TarBuffer;
  UINT32 u4SrcBuffer;
  UINT32 u4DecOutYAddr;
  UINT32 u4DecOutCAddr;
  UINT32 u4GldAddr;
  UINT32 u4WorkBufSize;
}WEBP_CFG_T;
/******************************************************************************
* Prototype        : cli "vdec" commands
******************************************************************************/

//low level commands
CLI_EXEC_T* GetVdecCmdTbl(void);

static INT32 _VdecCmdInit(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdPlay(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdStop(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSetRRMode(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSyncStc (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdQuery (INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdECMode(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdRstProfile(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdDramUsage(INT32 i4Argc, const CHAR ** szArgv);

void _VdecCapNotify(UINT32 u4CompId, UINT32 u4Cond, UINT32 u4Data1, UINT32 u4Data2);

static INT32  _VdecCmdCapture(INT32 i4Argc, const CHAR ** szArgv);

static INT32  _VdecCmdBigB(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdDramBWSimu(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdMemProtect(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdAVSyncTest(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdFixFHDDisplay(INT32 i4Argc, const CHAR ** szArgv);

#ifdef CC_EMULATION
UINT32 _VdecGetChkSum(UCHAR* pucPtr, VDEC_CHKSUM_INFO_T* prCSInfo);
#ifdef CC_TESTFLOW
void _VdecNotifyCRC(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4); //for h264
#endif
void _VdecNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static void _VdecVP6Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
static void _VdecVP8Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

void _VdecMPEG4Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

void _VdecAVSNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

void _VdecWMVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

void _VdecRVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static BOOL _LoadFile(const char* pcFileName, UINT32 u4Addr);

static BOOL _SaveFile(const char* pcFileName, UINT32 u4Addr, UINT32 u4Size);

#ifdef IDE_WRITE_SUPPORT
static BOOL _IDEComp(const char* pcFileName, UINT32 u4Addr, UINT32 u4Len);
#endif

static BOOL _VdecVerifyDecData(const VDEC_DISP_PIC_INFO_T* prDispInfo, BOOL bIsYcomp);

#ifdef CC_64x32BLK_CMP
static void _VdecBlkTransform(UINT8* pu1DstBlk16x32, const VDEC_DISP_PIC_INFO_T* prDispInfo, BOOL bIsYcomp);
#endif

static void _Vdec1389B2R(UINT8* pu1RasterData, VDEC_DISP_PIC_INFO_T rDispInfo, BOOL bIsYcomp);

static void _VdecSendH264Pattern(void* pvArg);

static void _VdecSendAVSPattern(void* pvArg);

static void _VdecSendMPEG4Pattern(void* pvArg);

static void _VdecSendWMVPattern(void* pvArg);

static void _VdecSendVP6Pattern(void* pvArg);

static void _VdecSendVP8Pattern(void* pvArg);

#ifdef CC_RV_EMULATION
static INT32 _VdecRvEmu(INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _VdecCmdSendPattern(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSaveDecOut(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSaveSimPat(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSetTestMode(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSetRepeatFrm(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdChkSumEn(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdecCmdGenCrcEn(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdecCmdVerifySetInfo(INT32 i4Argc, const CHAR ** szArgv);



#ifdef CC_WMV_EMULATION
static INT32 _VdecCmdLoadResult(INT32 i4Argc, const CHAR ** szArgv);
#endif


static INT32 _VdecCmdVp8DumpReg(INT32 i4Argc, const CHAR ** szArgv);

static UINT32 _VdecWebPCallBack(UINT32 u4Tag,void *pvData,UINT32 u4CbType);
static INT32 _VdecCmdWebPCfg(INT32 i4Argc, const CHAR ** szArgv);
//wmv
void Dump_Mem(VDEC_DISP_PIC_INFO_T*);
void print_checksum(BOOL fgRead,UINT32 u4FrmIdx);
UINT32 u4CalculatePixelAddress_Y(
    UINT32 u4YBase,                           ///< [IN] frame buffer Y component address
    UINT32 u4XPos,                             ///< [IN] x position of pixel in frame buffer
    UINT32 u4YPos,                             ///< [IN] y position of pixel in frame buffer
    UINT32 u4FrameWidth,                 ///< [IN] Frame buffer width
    BOOL fgBlock,                               ///< [IN] MTK block / raster scan
    UCHAR bBlockW
);
UINT32 u4CalculatePixelAddress_C(
    UINT32 u4CBase,                           ///< [IN] frame buffer CbCr component address
    UINT32 u4XPos,                             ///< [IN] x position of pixel in frame buffer
    UINT32 u4YPos,                             ///< [IN] y position of pixel in frame buffer
    UINT32 u4FrameWidth,                 ///< [IN] Frame buffer width
    BOOL fgBlock,                               ///< [IN] MTK block / raster scan
    BOOL fg420,                                   ///< [IN] 420 / 422
    UCHAR bBlockW
);
#endif

#if defined(CC_STRESS_TEST_CRC_CHK) || defined(CC_EMULATION)
static INT32 _VdecCmdRecordCRC(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _VdecCmdEnCRC(INT32 i4Argc, const CHAR ** szArgv);
#endif

//wmv end

/******************************************************************************
* Variable        : cli "vdec" command table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(VDEC)

static CLI_EXEC_T _arVdecCmdTbl[] =
{
    {"init",                "i",      _VdecCmdInit,            NULL,    "Vdec init",            CLI_SUPERVISOR},
    {"play",               "p",     _VdecCmdPlay,            NULL,    "Vdec play",            CLI_SUPERVISOR},
    {"stop",              "s",      _VdecCmdStop,          NULL,   "Vdec stop",    CLI_SUPERVISOR},
    {"setRRmode",      "srr",    _VdecCmdSetRRMode,    NULL,    "Vdec set RR mode",    CLI_SUPERVISOR},
    {"setECMode",       "secm", _VdecCmdECMode,        NULL,    "Vdec set EC mode",    CLI_SUPERVISOR},
    {"setSyncStc",      "ss",    _VdecCmdSyncStc,    NULL,   "Vdec set sync stc",    CLI_SUPERVISOR},
    {"query",              "q",     _VdecCmdQuery,          NULL,   "Vdec query info",    CLI_SUPERVISOR},
    {"rstTimeProfile",    "r",      _VdecCmdRstProfile,  NULL,   "Vdec rst time profile",    CLI_SUPERVISOR},
    {"setDramUsage",   "d",      _VdecCmdDramUsage,  NULL,   "Vdec dram usage",    CLI_SUPERVISOR},
    {"capture",   "c",      _VdecCmdCapture,  NULL,   "Vdec capture",    CLI_SUPERVISOR},
    {"bigb",   "bb",      _VdecCmdBigB,  NULL,   "Vdec set B parameter",    CLI_SUPERVISOR},
    {"setDramBWSimu", "ds",   _VdecCmdDramBWSimu,    NULL,    "Vdec set dram bandwidth simulation",    CLI_SUPERVISOR},
    {"setMid", "mid",   _VdecCmdMemProtect,    NULL,    "Vdec mid protection",    CLI_SUPERVISOR},
    {"setAVSyncTest", "st",   _VdecCmdAVSyncTest,    NULL,    "Vdec av sync test",    CLI_SUPERVISOR},
    {"FixFHDDisplay",   "fd",    _VdecCmdFixFHDDisplay,    NULL,    "Set FixHDDisplay For avs part",    CLI_SUPERVISOR},
#if defined(CC_STRESS_TEST_CRC_CHK) || defined(CC_EMULATION)
    {"crcRec","crcRec",   _VdecCmdRecordCRC,    NULL,    "Vdec record CRC",    CLI_SUPERVISOR},    
    {"crc","crc",   _VdecCmdEnCRC,    NULL,    "Vdec enable CRC check",    CLI_SUPERVISOR},        
#endif
#ifdef CC_EMULATION
    {"sendPattern",    "sp",    _VdecCmdSendPattern,    NULL,    "Vdec send pattern",    CLI_SUPERVISOR},
    {"saveDecOut",    "so",    _VdecCmdSaveDecOut,    NULL,    "Vdec save dec out data",    CLI_SUPERVISOR},
    {"saveSimPat",     "ssp",   _VdecCmdSaveSimPat,    NULL,    "Vdec save sim pat",    CLI_SUPERVISOR},
    {"setTestMode",   "stm",   _VdecCmdSetTestMode,    NULL,    "Vdec set test mode",    CLI_SUPERVISOR},
    {"setRepeatFrm",   "srf",    _VdecCmdSetRepeatFrm,    NULL,    "Vdec set repeat frame",    CLI_SUPERVISOR},
    {"checksumEn",   "cs",    _VdecCmdChkSumEn,    NULL,    "Vdec checksum enable",    CLI_SUPERVISOR},
    {"GenCrcEn",   "gcrc",    _VdecCmdGenCrcEn,    NULL,    "Vdec generate crc enable",    CLI_SUPERVISOR},
    {"SetVerifySet",   "set",    _VdecCmdVerifySetInfo,    NULL,    "Set verify pattten",    CLI_SUPERVISOR},
    #ifdef CC_VP8_EMULATION
    {"DumpVp8Register", "dr", _VdecCmdVp8DumpReg, NULL, "Dump VP8 decoder register", CLI_SUPERVISOR},
    {"WebPConfig", "webp", _VdecCmdWebPCfg, NULL, "WebP config", CLI_SUPERVISOR},
#ifdef CC_WMV_EMULATION
    {"LoadDecodeResult",   "lr",    _VdecCmdLoadResult,    NULL,    "Load Decode result",    CLI_SUPERVISOR},
#endif
    #endif // CC_VP8_EMULATION
#endif
    CLIMOD_DEBUG_CLIENTRY(VDEC),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Vdec)
{
    "vdec",
    NULL,
    NULL,
    _arVdecCmdTbl,
    "Vdec command",
    CLI_SUPERVISOR
};

/******************************************************************************
* Variable        : cli "vdec" enums
******************************************************************************/
static ENUM_VDEC_FMT_T _eFmt; // Record the current file format
static UINT32 _u4YRREn = 0, _u4CRREn = 0, _u4YRRRatio = 0, _u4CRRRatio = 0;
static UINT8 _u1DramMoniter = 0;
static HANDLE_T _hVdecCapSema;
static UINT8* pu1CapBuf = NULL;
//memory protection
static UINT8 _u1MidMode = 0;
static BOOL _fgDBKDisable = FALSE;
static UINT32 _u4VStart, _u4VEnd;
static UINT8 _u1AVSyncTestMode = 0;
#if defined(CC_STRESS_TEST_CRC_CHK) || defined(CC_EMULATION)
static BOOL _fgEnableCRC = FALSE;
static UINT32 _u4ChkPTS = 0;
#endif
#ifdef CC_EMULATION
static UINT8* _pu1RegDumpBuf = NULL;
static UINT8 _uGenCrcEn=0;
static BOOL _bIsFirstFrm = TRUE;
static BOOL _bFileLoadSuccess = FALSE;
static UINT32 _u4CrcIndex=0;
static char _acTargetDir[MAX_FILE_NUM][MAX_FILENAME_LEN];
static char _acFileDir[MAX_FILE_NUM][MAX_FILENAME_LEN];
static char _acGoldPrefix[MAX_FILE_NUM][MAX_FILENAME_LEN];
static UINT32 _u4CurFileIdx = 0;
static UINT32 _u4CurFrmIdx = 0,_u4StartFameIndex=0,u4EndFrameIndex=0,u4CurFrmIdx22222222 = 0;
static UINT32 _au4FrameNumber[MAX_FILE_NUM];
static UINT32 _au4FrmFldNumberTotal;
static UINT32 _au4FrameNumberCurValidTotal;
static INT32 _i4SSimPatFrmNo = -1;
static BOOL _fgDisableGlolden = FALSE;
static UINT8* _pu1GoldenData = NULL;
#ifdef CC_TESTFLOW
static UINT32 u4CrcSize = 0;
#endif
static UINT8* _pu1GoldenDatacrc = NULL;
static UINT8* _pu1GoldenDatamvc = NULL;
static UINT32 _u4BstmSa = 0;
static UINT32 _u4BstmSz = 0;
#ifdef CC_SUPPORT_LOG_INFO
UINT32 _u4LogBufSa = 0;
UINT32 _u4LogBufSz = 0;
UINT32 _u4LogSz = 0;
#endif

#ifdef CC_64x32BLK_CMP
static UINT8* _pu1DecData16x32 = NULL;
#endif
#ifdef RR_TEST
static UINT8* _pu1GoldenDataC = NULL;
#endif
static BOOL _bSaveOutput = FALSE, _bCmpEn = FALSE, _bIsPatSending = FALSE;
static HANDLE_T _hVdecSema;
static UINT8 _u1TestSet = 0;    //0:other, 1:Allegro, 2:Allegro2
static char* _pcFileInfo = NULL;
static char* _pu1VfifoSa = NULL;
static char* _pcChkSum = NULL;
static INT32 _i4LoopCnt = 0;
//emulation, compare switch, repeat frame
static BOOL _bCmpFrmEn[1000];
static BOOL _bRptFrmEn;
static UINT32 _u4RptFrmNo;
static UINT32 u4DumpIndex=(UINT32)-1;
UINT8 gu1VdecLogSimPat = 0;

static char _acTestBitstream[MAX_FILE_NUM][MAX_FILENAME_LEN];
char _acLoadedBitstream[MAX_FILENAME_LEN];
static BOOL _afgLoadCompData[MAX_FILE_NUM];
static BOOL _afgLoadGoldenData[MAX_FILE_NUM];
static BOOL _fgLoadFail = FALSE;
static UINT32 _au4StartCompareFrameNum[MAX_FILE_NUM];
static UINT32 _au4PicWidth[MAX_FILE_NUM];
static UINT32 _au4PicHeight[MAX_FILE_NUM];
static UINT32 _u4FileCount = 0, _u4DecFrmIdx = 0;
static UINT32 _u4DecCycles = 0;
static char _acFilePrefix[MAX_FILE_NUM][MAX_FILENAME_LEN];

//wmv
static UINT32 bitstream_sz[MAX_FILE_NUM];
static CHAR* _pcStartCodeFile = NULL;
static char golden_path[MAX_FILE_NUM][1024];
int _vlcreg[250];
int _mcreg[250];
UINT32 m_dwBitsFSz;
VDEC_VERIFY_SET rVerifySet[VDEC_VERIFY_SET_NUM];
BOOL _fgAbtCurItem = FALSE;
#ifdef CC_WMV_EMULATION
#define WMV_FILE_NG 1
#define WMV_FILE_SKIP 2
extern UINT32 u4DumpWmvReg(CHAR *pu1Buf);
extern void vDumpWmvMcReg(UCHAR ucEsId);

volatile BOOL _fgHalt = 1;
static UINT8 _auNGFile[MAX_FILE_NUM];
static BOOL fgDumpChecksum = TRUE;
static UINT32 WMV_REPEAT_FRAME = 0;
static UINT32 WMV_REPEAT_FRAME_NO = 0;
static UINT32 WMV_REPEAT_COMP_FRAME_NO = 0;
static char* check_sum = NULL;
static char* tmp = NULL;
static BOOL _fgDownLoadResult=FALSE;
static UINT32 _u4DownLoadFileId=0,_u4DownLoadFrameId=0;
#endif
//end wmv

// VP6
UINT32 _u4StartFrame = 0, _u4WithDbk = FALSE;
UINT32 _u4VP6DecodeMode = 1;
UINT32 _u4VP6EnableAlpha = FALSE;

char _acFileName[MAX_FILENAME_LEN];
char _acChkSumFile[MAX_FILENAME_LEN];
#ifdef CC_TESTFLOW
char _acFileNamecrc[MAX_FILENAME_LEN];
static BOOL fgSaveCrc = TRUE;//whether save crc or compare with crc TRUE:save crc FALSE:compare with crc
#endif
static BOOL _fgEnChkSum = FALSE;
static VDEC_CHKSUM_INFO_T _rCSInfo;

#ifdef CC_VP8_EMULATION
static WEBP_CFG_T _rWebCfg=
{
  WEBP_COLOR_AYCBCR8888,
  WEBP_DEC_MODE_MCUROW,
  480,//720,
  320,//480,
  0xD000000,
  0,
  0x00000000,
  0x00000000,
  0x00000000,
  0
};

static BOOL _LoadSaveFile(const char* pcFileName, BOOL fgLoad,UINT32 u4Addr,UINT32 u4Size);
#endif // CC_VP8_EMULATION

#ifdef WMV_PROFILING
static HAL_TIME_T rTimeRstS;
static HAL_TIME_T rTimeRstE;
static HAL_TIME_T rTimeRstDt;
#endif
static UINT8* _pu1RasterData = NULL;
static UINT32 _au4FrameNumber[MAX_FILE_NUM];
UINT32 _u4Dx311PicWidth;
UINT32 _u4Dx311PicHeight;
// for RV
BOOL _fgRVEmuEnd = FALSE;
UINT32 _u4RVEmuEndPicNum = 0xFFFFFFFF;

#endif

/******************************************************************************
* Flag(s)        :
******************************************************************************/

/******************************************************************************
* Implementation        : helper functions for commands parsing
******************************************************************************/
//-----------------------------------------------------------------------------
#ifdef CC_EMULATION
static BOOL _AVSVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    switch (prPes->u4FrameType)
    {
        case AVS_PIC_TYPE_SEQ_START:
            rMpvPes.ucPicType = 0xB0;
            break;
        case AVS_PIC_TYPE_PIC_I:
            rMpvPes.ucPicType = 0xB3;
            break;
        case AVS_PIC_TYPE_PIC_PB:
            rMpvPes.ucPicType = 0xB6;
            break;
        default:
            //ASSERT(0);
            break;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;
    rMpvPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}

//-----------------------------------------------------------------------------
static BOOL _VideoCallback(const DMX_PES_MSG_T* prPes)
{
#ifndef CC_EMULATION
    switch (DMX_GetVideoType())
    {
        case DMX_VIDEO_AVS:
            return _AVSVideoCallback(prPes);
            break;
        default:
            Printf("Unknown video type\n");
            break;
    }
#endif
    return TRUE;
}
#endif


static INT32 _VdecCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef MEMRW_TEST
    HANDLE_T hThread;
    INT32 i4Ret;
#endif

    UNUSED(i4Argc);
    UNUSED(szArgv);

#ifdef CC_EMULATION
    if (_pcFileInfo == NULL)
    {
        _pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(FILE_INFO_SIZE, 1024);
    }
    if (_pcChkSum == NULL)
    {
        _pcChkSum = (char*)BSP_AllocAlignedDmaMemory(CHKSUM_INFO_SIZE, 1024);
		ASSERT(_pcChkSum!=NULL);
    }
    if (_pu1RegDumpBuf == NULL)
    {
        _pu1RegDumpBuf = (char*)BSP_AllocAlignedDmaMemory(REGDUMP_SIZE, 1024);
		     ASSERT(_pu1RegDumpBuf != NULL);
    }
    if (_pu1GoldenData == NULL)
    {
        _pu1GoldenData = (UINT8*)BSP_AllocAlignedDmaMemory(GOLDEN_SIZE, 1024);
		_pu1GoldenDatamvc= (UINT8*)BSP_AllocAlignedDmaMemory(0x7500000, 1024);
		_pu1GoldenDatacrc = (UINT8*)BSP_AllocAlignedDmaMemory(0x800000, 1024);
#ifdef RR_TEST
        _pu1GoldenDataC = (UINT8*)BSP_AllocAlignedDmaMemory(GOLDEN_SIZE, 1024);
#endif
    }
#ifdef CC_MPEG4_VERIFiCATION
    if (_pu1RasterData == NULL)
    {
        _pu1RasterData = (UINT8*)BSP_AllocAlignedDmaMemory(GOLDEN_SIZE, 1024);
    }
#endif
#ifdef CC_64x32BLK_CMP
    if (_pu1DecData16x32 == NULL)
    {
        _pu1DecData16x32 = (UINT8*)BSP_AllocAlignedDmaMemory(GOLDEN_SIZE, 1024);
    }
#endif
    if (_pcStartCodeFile == NULL)
    {
        _pcStartCodeFile = (char*)BSP_AllocAlignedDmaMemory(FILE_INFO_SIZE, 1024);
    }

    if (_pu1VfifoSa==NULL)
    {
        _pu1VfifoSa = (char*)BSP_AllocAlignedDmaMemory(VFIFO_SZ, 1024);
        ASSERT(_pu1VfifoSa);
    }

    x_memset(rVerifySet,0,sizeof(VDEC_VERIFY_SET)*VDEC_VERIFY_SET_NUM);
#ifdef IDE_READ_SUPPORT
    _DmxIdeReset();
#endif
    VERIFY(x_sema_create(&_hVdecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif

    VERIFY(x_sema_create(&_hVdecCapSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    VDEC_Init();

#ifdef MEMRW_TEST
#if MEMORY_RW_TEST
    // Create Task
    i4Ret = x_thread_create(&hThread,
                            "memrw_thread",
                            MLVDO_STACK_SIZE,
                            MLVDO_THREAD_PRIORITY,
                            memrw,
                            0,
                            NULL);

    VERIFY(i4Ret == OSR_OK);
#endif
#endif

    return 0;
}


static INT32 _VdecCmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if (i4Argc > 2)
    {
        _eFmt = (ENUM_VDEC_FMT_T)StrToInt(szArgv[2]);   
        if(_eFmt >= VDEC_FMT_MAX && _eFmt!=VDEC_FMT_WEBP)
        {
            LOG(0, "vdec.p esid {0} fmt {0:MPV, 1:MPEG4, 2:H.264, 3:WMV, 4:H.264Verification, 5:MJPG, 6:RV, 7:AVS, 8:VP6, 9:VP8}\n");
            return -1;
        }
    }
    else
    {
        LOG(0, "vdec.p esid {0} fmt {0:MPV, 1:MPEG4, 2:H.264, 3:WMV, 4:H.264Verification,5:MJPG, 6:RV, 7:AVS, 8:VP6, 9:VP8}\n");
        return 0;
    }

#ifdef CC_EMULATION
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
    switch (_eFmt)
    {
        case VDEC_FMT_H264:
        case VDEC_FMT_H264VER:
            VDEC_RegEventNotifyFunc(_VdecNotify);
            break;
        case VDEC_FMT_MP4:
            VDEC_RegEventNotifyFunc(_VdecMPEG4Notify);
            break;
        case VDEC_FMT_WMV:
            VDEC_RegEventNotifyFunc(_VdecWMVNotify);
            break;
        case VDEC_FMT_AVS:
            VDEC_RegEventNotifyFunc(_VdecAVSNotify);
            // For dmx + vdec + b2r testing
            DMX_SetToDecoder(TRUE);

            rDecoderCallbacks.pfnSendVideoPes = _VideoCallback;
            rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;

            DMX_SetDecoderCallbacks(&rDecoderCallbacks);

            DMX_SetVideoType(DMX_VIDEO_AVS);
            break;

#ifdef CC_RV_EMULATION
        case VDEC_FMT_RV:
            VDEC_RegEventNotifyFunc(_VdecRVNotify);
            break;
#endif

        case VDEC_FMT_VP6:
                {
                    UINT32 u4BufSa = 0;
                    UINT32 u4BufSz = 90*1024*1024;

                    while(TRUE)
                    {
                        u4BufSa = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSz, 1024);
                        if (u4BufSa)
                        {
                            break;
                        }
                        Printf("BSP allocate memory for VP6 fail\n");
                        ASSERT(0);
                        u4BufSz >>= 1;
                        if (u4BufSz <= (VFIFO_E - VFIFO_S))
                        {
                            break;
                        }
                    }
					/*
                    if (u4BufSz < (VFIFO_E - VFIFO_S))
                    {
					   Printf("BSP allocate memory too small\n");
                        BSP_FreeAlignedDmaMemory((void*)u4BufSa);
                        _u4BstmSa = VFIFO_S;
                        _u4BstmSz = (VFIFO_E - VFIFO_S);
                    }
                    else
					*/
                    {
                        _u4BstmSa = u4BufSa;
                        _u4BstmSz = u4BufSz;
                    }
#ifdef CC_SUPPORT_LOG_INFO
                    _u4LogBufSz = 10*1024*1024;
                    _u4LogBufSa = BSP_AllocAlignedDmaMemory(_u4LogBufSz, 1024);
                    ASSERT(_u4LogBufSa != 0);
                    _u4LogSz = 0;
                    x_memset((void*)_u4LogBufSa, 0, _u4LogBufSz);
#endif
                }
               VDEC_RegEventNotifyFunc(_VdecVP6Notify);
                break;
                
#ifdef CC_VP8_EMULATION
            case VDEC_FMT_VP8:
                VDEC_RegEventNotifyFunc(_VdecVP8Notify);
#ifdef CC_SUPPORT_LOG_INFO
                _u4LogBufSz = 10*1024*1024;
                _u4LogBufSa = BSP_AllocAlignedDmaMemory(_u4LogBufSz, 1024);
                ASSERT(_u4LogBufSa != 0);
                _u4LogSz = 0;
                x_memset((void*)_u4LogBufSa, 0, _u4LogBufSz);
#endif
                break;

            case VDEC_FMT_WEBP:
			    {
				   FBM_POOL_T* pFbmPool = NULL;
				   pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG);
				   if(pFbmPool==NULL)
				   { 
                      Printf("Can't get jpeg fbm type \n");
				   }
				   else
				   { 
                      Printf("WorkingAdd=0x%x,Size=0x%x,ChangeSize=0x%x\n",pFbmPool->u4Addr,pFbmPool->u4Size,_rWebCfg.u4WorkBufSize);
                      if(_rWebCfg.u4WorkBufSize)
                      {
                         pFbmPool->u4Size=_rWebCfg.u4WorkBufSize;
                      }
					  _VDEC_Vp8WebPInit(0,_VdecWebPCallBack,pFbmPool->u4Addr,pFbmPool->u4Size);
				   }
				   return 0;
            	}
            	break;
#endif // CC_VP8_EMULATION
        default: //VDEC_FMT_MPV
            break;
    }
#endif

    if (VDEC_Play(0, _eFmt))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static INT32 _VdecCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if (i4Argc > 1)
    {
        VDEC_Stop(StrToInt(szArgv[1]));
    }
    else
    {
        LOG(0, "vdec.s esid {0}");
    }
    return 0;
}

static INT32 _VdecCmdSetRRMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Param1 = 0, u4Param2 = 0, u4Param3 = 0;

    ASSERT(szArgv != NULL);
    if (i4Argc < 4)
    {
        LOG(0, "vdec.srr [enable 1, enable without wrapper 2][Y:0/C:1/MaxHband:2][Ratio (1/2):0, (3/4):1, bypass:2]\n");
        if (i4Argc > 2)
        {
            u4Param2 = StrToInt(szArgv[2]);
        }
        VDEC_GetParam(ES0, (UINT32)RRMODE, &u4Param1, &u4Param2, &u4Param3);
        LOG(0, "vdec.srr %d %d %d\n", u4Param1, u4Param2, u4Param3);
        return 0;
    }
    VDEC_SetParam(ES0, (UINT32)RRMODE, StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]));

    if (StrToInt(szArgv[2]) == 0)         //Y
    {
        _u4YRREn = StrToInt(szArgv[1]);
        if (_u4YRREn > 0)
        {
            _u4YRRRatio = StrToInt(szArgv[3]);
        }
    }
    else if (StrToInt(szArgv[2]) == 1)     //C
    {
        _u4CRREn = StrToInt(szArgv[1]);
        if (_u4CRREn > 0)
        {
            _u4CRRRatio = StrToInt(szArgv[3]);
        }
    }

    UNUSED(_u4YRRRatio);
    UNUSED(_u4CRRRatio);
    return 0;
}


//u4Param1: Enable error re-concealment, u4Param2: Mode, u4Param3: Dedicate error
static INT32 _VdecCmdECMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Param1 = 0, u4Param2 = 0, u4Param3 = 0, u4Param4 = 0, u4Param5 = 0, u4Param6 = 0;

    ASSERT(szArgv != NULL);
    if (i4Argc < 4)
    {
        LOG(0, "vdec.secm [ERC Enable][ERC mode][ERC errDed][DedType]\n");
        VDEC_GetParam(ES0, (UINT32)ECC_MODE, &u4Param1, &u4Param2, &u4Param3);
        VDEC_GetParam(ES0, (UINT32)ECC_TYPE, &u4Param4, &u4Param5, &u4Param6);
        LOG(0, "vdec.secm %d %d %d 0x%x\n", u4Param1, u4Param2, u4Param3, u4Param4);
        return 0;
    }

    VDEC_SetParam(ES0, (UINT32)ECC_MODE, StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]));
    VDEC_SetParam(ES0, (UINT32)ECC_TYPE, StrToInt(szArgv[4]), 0, 0);
    return 0;
}


static INT32 _VdecCmdSyncStc (INT32 i4Argc, const CHAR ** szArgv)
{
    UCHAR ucEsId;
    UCHAR ucMode;
    UCHAR ucStcId;

    if ((i4Argc < 4) || (szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) || (szArgv[3] == NULL))
    {
        LOG(0, "vdec.ss [ucEsId] [ucMode, 0:none, 1:slave, 2:master] [ucStcId]\n");
        return 0;
    }

    ucEsId = (UCHAR)StrToInt(szArgv[1]);
    ucMode = (UCHAR)StrToInt(szArgv[2]);
    ucStcId = (UCHAR)StrToInt(szArgv[3]);

    VDEC_SyncStc(ucEsId, ucMode, ucStcId);

    return 0;
}


static INT32 _VdecCmdQuery (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT64 au8DramUsage[5]={0}, u4DramClockCycle;
    INT32 idx;
    UINT8 u1VdecId;
    VDEC_HDR_INFO_T rHdrInfo;
    UINT64 u8Remainder1, u8Remainder2;

    for (u1VdecId = 0;u1VdecId < VDEC_MAX_ES;u1VdecId++)
    {
        if (!VDEC_QueryInfo((UCHAR)u1VdecId, &rHdrInfo))
        {
            //return 1;
            continue;
        }

        //x_memset(au8DramUsage, 0, sizeof(au8DramUsage));

        if (!rHdrInfo.fgHdrInvalid)
        {
            LOG(0, "Video decoder: ");
            switch (rHdrInfo.eCodecType)
            {
                case VDEC_FMT_H264:
                case VDEC_FMT_H264VER:
                    LOG(0, "H.264, ");
                    if (rHdrInfo.u4Profile == 66)
                    {
                        LOG(0, "BaseLine ");
                    }
                    else if (rHdrInfo.u4Profile == 77)
                    {
                        LOG(0, "Main ");
                    }
                    else if (rHdrInfo.u4Profile == 88)
                    {
                        LOG(0, "Extended ");
                    }
                    else if (rHdrInfo.u4Profile == 100)
                    {
                        LOG(0, "High ");
                    }
                    LOG(0, "Profile, Level %d.%d, ", rHdrInfo.u4Level / 10, rHdrInfo.u4Level % 10);
                    if (rHdrInfo.u4IsCABAC > 0)
                    {
                        LOG(0, "CABAC\n");
                    }
                    else
                    {
                        LOG(0, "CAVLC\n");
                    }
                    break;
                case VDEC_FMT_WMV:
                    LOG(0, "WMV\n");
                    break;
                case VDEC_FMT_MPV:
                    LOG(0, "MPEG2\n");
                    break;
                case VDEC_FMT_MP4:
                    LOG(0, "MPEG4\n");
                    break;
                case VDEC_FMT_VP8:
                    LOG(0, "VP8\n");
                    break;
                case VDEC_FMT_VP6:
                    LOG(0, "VP6\n");
                    break;
                case VDEC_FMT_RV:
                    LOG(0, "RV\n");
                    break;
                default:
                    LOG(0, "UnKnown\n");
                    break;
            }

            LOG(0, "State: ");
            switch (rHdrInfo.u1CurState)
            {
                case VDEC_ST_STOP:
                    LOG(0, "Stop\n");
                    break;
                case VDEC_ST_PLAY:
                case VDEC_ST_PLAY_I_FRAME:
                    LOG(0, "Play\n");
                    break;
                case VDEC_ST_IDLE:
                    LOG(0, "Idle\n");
                    break;
                default:
                    LOG(0, "UnKnown\n");
                    break;
            }

            LOG(0, "Format: %dx%d", rHdrInfo.u2Width, rHdrInfo.u2Height);
            if (rHdrInfo.fgProgressiveSeq)
            {
                LOG(0, "p ");
            }
            else
            {
                LOG(0, "i ");
            }
            LOG(0, "%d.%dfps\n", (rHdrInfo.u2FrmRate / 100), (rHdrInfo.u2FrmRate % 100));

            //es fifo related
            LOG(0, "EsFifoCnt: (Cur,Max) = (%d, %d)\n", rHdrInfo.u4CurEsCnt, rHdrInfo.u4MaxEsCnt);
            //fbm related
            LOG(0, "FbNum: %d\n", rHdrInfo.ucFbNum);

            rHdrInfo.au4DecTime[0][3] = (rHdrInfo.au4DecTime[0][3] == 0) ? 1 : rHdrInfo.au4DecTime[0][3];
            rHdrInfo.au4DecTime[1][3] = (rHdrInfo.au4DecTime[1][3] == 0) ? 1 : rHdrInfo.au4DecTime[1][3];
            rHdrInfo.au4DecTime[2][3] = (rHdrInfo.au4DecTime[2][3] == 0) ? 1 : rHdrInfo.au4DecTime[2][3];
            if (_u1DramMoniter == 0)
            {
                for (idx = 2; idx >= 0; idx--)
                {
                    au8DramUsage[idx] = 0;
                }
                LOG(0, "Time Profile:\n\tPicType(MinHwDec, AvgHwDec, MaxHwDec, SkipFrm/TotalFrm, MaxSwDec, MaxDramCycle(-))");
            }
            else if (_u1DramMoniter == 1) //total percentage
            {
#ifdef VDEC_TIME_PROFILE
#ifdef CC_MT5396
                u4DramClockCycle = (BSP_GetDomainClock(SRC_MEM_CLK) >> 1) ; //dram cycle /sec
#else
                u4DramClockCycle = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
#endif
                for (idx = 2; idx >= 0; idx--)
                {
                    if ((UINT64)rHdrInfo.au4DecTime[idx][0] > 0)
                    {
                        au8DramUsage[idx] =
                            u8Div6432(u8Div6432(((UINT64)rHdrInfo.au4DecTime[idx][8] * 100000),
                                                (UINT64)rHdrInfo.au4DecTime[idx][0], &u8Remainder1),
                                      (UINT64)u4DramClockCycle, &u8Remainder2);
                    }
                    else
                    {
                        au8DramUsage[idx] = 0;
                    }
                }
#else
                UNUSED(u4DramClockCycle);
#endif

                LOG(0, "Time Profile:\n\tPicType(MinHwDec, AvgHwDec, MaxHwDec, SkipFrm/TotalFrm, MaxSwDec, MaxDramCycle(Total %))");
            }
            else if (_u1DramMoniter == 2) //MPEG Mbyte
            {
#ifdef VDEC_TIME_PROFILE
                for (idx = 2; idx >= 0; idx--)
                {
                    au8DramUsage[idx] = u8Div6432((((UINT64)rHdrInfo.au4DecTime[idx][8] * 16) * (UINT64)rHdrInfo.u2FrmRate),
                                                  100, &u8Remainder1);
                }
#endif
                LOG(0, "Time Profile:\n\tPicType(MinHwDec, AvgHwDec, MaxHwDec, SkipFrm/TotalFrm, MaxSwDec, MaxDramCycle(Mpeg Byte))");
            }
            UNUSED(u8Remainder1);
            UNUSED(u8Remainder2);
            LOG(0, "\n\tI(0.%03us, 0.%03us/%d, 0.%03us/%d, %d/%d, 0.%06us, %d)",
                rHdrInfo.au4DecTime[2][1], rHdrInfo.au4DecTime[2][2] / rHdrInfo.au4DecTime[2][3], (rHdrInfo.au4DecTime[2][6] / rHdrInfo.au4DecTime[2][3])*10000,
                rHdrInfo.au4DecTime[2][0], rHdrInfo.au4DecTime[2][7],
                rHdrInfo.au4DecTime[2][4], rHdrInfo.au4DecTime[2][3], rHdrInfo.au4DecTime[2][5],
                au8DramUsage[2]);
            LOG(0, "\n\tP(0.%03us, 0.%03us/%d, 0.%03us/%d, %d/%d, 0.%06us, %d)",
                rHdrInfo.au4DecTime[0][1], rHdrInfo.au4DecTime[0][2] / rHdrInfo.au4DecTime[0][3], (rHdrInfo.au4DecTime[0][6] / rHdrInfo.au4DecTime[0][3])*10000,
                rHdrInfo.au4DecTime[0][0], rHdrInfo.au4DecTime[0][7],
                rHdrInfo.au4DecTime[0][4], rHdrInfo.au4DecTime[0][3], rHdrInfo.au4DecTime[2][5],
                au8DramUsage[0]);
            LOG(0, "\n\tB(0.%03us, 0.%03us/%d, 0.%03us/%d, %d/%d, 0.%06us, %d)\n",
                rHdrInfo.au4DecTime[1][1], rHdrInfo.au4DecTime[1][2] / rHdrInfo.au4DecTime[1][3], (rHdrInfo.au4DecTime[1][6] / rHdrInfo.au4DecTime[1][3])*10000,
                rHdrInfo.au4DecTime[1][0], rHdrInfo.au4DecTime[1][7],
                rHdrInfo.au4DecTime[1][4], rHdrInfo.au4DecTime[1][3], rHdrInfo.au4DecTime[1][5],
                au8DramUsage[1]);
        }
    }
    UNUSED(i4Argc);
    UNUSED(szArgv);
    return 0;
}


static INT32 _VdecCmdRstProfile(INT32 i4Argc, const CHAR ** szArgv)
{
    VDEC_SetParam(ES0, (UINT32)TIME_PROFILE, 0, 0, 0);

    UNUSED(i4Argc);
    UNUSED(szArgv);
    return 0;
}


static INT32 _VdecCmdDramUsage(INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "vdec.d [DramUsage Mode, 0: none, 1: total, 2: mpeg]\n");
        return 0;
    }
    _u1DramMoniter = StrToInt(szArgv[1]);
    VDEC_SetParam(ES0, (UINT32)TIME_PROFILE2, _u1DramMoniter, 0, 0);

    UNUSED(i4Argc);
    UNUSED(szArgv);
    return 0;
}

void _VdecCapNotify(UINT32 u4CompId, UINT32 u4Cond, UINT32 u4Data1, UINT32 u4Data2)
{
    if (VDP_SetFreeze(0, 0) == 0)
    {
        LOG(1, "Video freeze Fail!");
    }

    if (u4Cond == (UINT32)VDEC_COND_THUMBNAIL_DONE)
    {
        VERIFY(x_sema_unlock(_hVdecCapSema) == OSR_OK);
    }

    UNUSED(u4CompId);
    UNUSED(u4Data1);
    UNUSED(u4Data2);
}

static INT32  _VdecCmdCapture(INT32 i4Argc, const CHAR ** szArgv)
{
    VDP_THUMBNAIL_INFO_T rCapInfo;
    VDP_CAPTURE_INTO_T rVdpCapInfo;
    BOOL bIsOnePass = FALSE;
    HAL_TIME_T rTimeS, rTimeE, rTimeDt;
    UINT32 u4ColorMode = 0;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "vdec.c [Color Mode, 0: AYUV_8888, 1: ARGB_8888, 2: ARGB_4444]\n");
        return 0;
    }

    if (pu1CapBuf == NULL)
    {
        pu1CapBuf = (UINT8*)BSP_AllocAlignedDmaMemory(200 * 200 * 4, 1024);
    }
    rCapInfo.pucCanvasBuffer = pu1CapBuf;

    u4ColorMode = StrToInt(szArgv[1]);
    if (u4ColorMode == 0)
    {
        rCapInfo.eCanvasColormode = VDP_COLORMODE_AYUV_D8888;
    }
    else if (u4ColorMode == 1)
    {
        rCapInfo.eCanvasColormode = VDP_COLORMODE_ARGB_D8888;
    }
    else if (u4ColorMode == 2)
    {
        rCapInfo.eCanvasColormode = VDP_COLORMODE_ARGB_D4444;
    }
    else
    {
        LOG(0, "vdec.c [Color Mode, 0: AYUV_8888, 1: ARGB_8888, 2: ARGB_4444]\n");
        return 0;
    }
    rCapInfo.u4CanvasWidth = 200;
    rCapInfo.u4CanvasHeight = 200;
    rCapInfo.u4ThumbnailWidth = 200;
    rCapInfo.u4ThumbnailHeight = 200;
    rCapInfo.u4ThumbnailX = 0;
    rCapInfo.u4ThumbnailY = 0;

    VDEC_RegEventNotifyFunc(_VdecCapNotify);
    if (VDP_SetFreeze(0, 1) == 0)   //use vdpID 0 arbitrary
    {
        LOG(1, "Video freeze Fail!");
        return 0;
    }
    UNUSED(VDP_GetLockFrameBufferAddr(0, &rVdpCapInfo));
    if ((rVdpCapInfo.u4AddrY == 0) || (rVdpCapInfo.u4AddrC == 0) || (rVdpCapInfo.u4RRMode > 1))
    {
        LOG(1, "Video Capture Fail!");
        return 0;
    }

    bIsOnePass = (rCapInfo.eCanvasColormode == VDP_COLORMODE_AYUV_D8888) ? TRUE : FALSE;

    HAL_GetTime(&rTimeS);
	
#if defined(CC_MT5368) || defined(CC_MT5395)

    UNUSED(VDEC_GetVideoThumbnail(ES0, 0, bIsOnePass, &rCapInfo, &rVdpCapInfo,FALSE));
#else
    UNUSED(VDEC_GetVideoThumbnail(ES0, 0, bIsOnePass, &rCapInfo, &rVdpCapInfo,FALSE));
#endif 

    VERIFY(x_sema_lock(_hVdecCapSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait capture done

    HAL_GetTime(&rTimeE);
    HAL_GetDeltaTime(&rTimeDt, &rTimeS, &rTimeE);

    LOG(1, "Video Capture Frm 0x%x--0x%x, %u.%6u sec\n", (UINT32)rCapInfo.pucCanvasBuffer,
        (UINT32)rCapInfo.pucCanvasBuffer + (200*200*4), rTimeDt.u4Seconds, rTimeDt.u4Micros);

    return 0;
}

static INT32 _VdecCmdBigB(INT32 i4Argc, const CHAR ** szArgv)
{
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        LOG(0, "vdec.bigb fgForceEnable u4BigBLevel1 u4BigBLevel2 u4BigBLevel3 u4BigBBaseCnt\n");
        LOG(0, "vdec.bigb 1 33000 35000 37000 30\n");
        return 0;
    }
    VDEC_SetBigBParameter(0, (BOOL)StrToInt(szArgv[1]),
                          StrToInt(szArgv[2]), StrToInt(szArgv[3]),
                          StrToInt(szArgv[4]), StrToInt(szArgv[5]));
    return 0;
}

#ifdef CC_EMULATION
UINT32 _VdecGetChkSum(UCHAR* pucPtr, VDEC_CHKSUM_INFO_T* prCSInfo)
{
    UCHAR* pcToken;
    UINT32 u4FrmNum = 0, u4CntPerFrm  = 0, u4Val = 0;

    pcToken = strtok(pucPtr, "\r\n");
    while (pcToken != NULL)
    {
        u4Val = StrToInt(pcToken);
        switch (u4CntPerFrm)
        {
            case 0:
                prCSInfo->u4ShftAddY1[u4FrmNum] = u4Val;
                break;
            case 1:
                prCSInfo->u4ShftAddY2[u4FrmNum] = u4Val;
                break;
            case 2:
                prCSInfo->u4ShftAddC1[u4FrmNum] = u4Val;
                break;
            case 3:
                prCSInfo->u4ShftAddC2[u4FrmNum] = u4Val;
                break;
            default:
                break;
        }
        u4CntPerFrm++;
        if (u4CntPerFrm == CS_PER_FRM)
        {
            u4FrmNum++;
            u4CntPerFrm = 0;
        }
        pcToken = strtok(NULL, "\r\n");
    }

    return u4FrmNum;
}

#ifdef CC_TESTFLOW
static BOOL _VdecVerifyDecDataCRC(const VDEC_DISP_PIC_INFO_T* prDispInfo, UINT32 i)
{ int k,j;
	UINT32* pu4GoldData = _pu1GoldenDatacrc+i*32;
    //LOG(0, "now frame=%d \n", i);

	if(prDispInfo!=NULL)
		{
			if(fgSaveCrc)
			{
		     	for(j=0;j<4;j++)
		      	{
					pu4GoldData[j] = prDispInfo->u4Ycrc[j];
		      	}
				pu4GoldData+=4;
			 	for(k=0;k<4;k++)
			    {
				    pu4GoldData[k] = prDispInfo->u4Ccrc[k];
			    }
			}
			else
			{
				
	     		for(j=0;j<4;j++)
		        {
	            	if(prDispInfo->u4Ycrc[j]!=pu4GoldData[j])
	            	{
	                   LOG(0, "//Error!Y %s Data Mismatch at frame=%d, index= %x, decode= 0x%.2x, Golden = 0x%.2x !!! \n", 
	                         _acFileName,i,j, prDispInfo->u4Ycrc[j], pu4GoldData[j]);
					   return FALSE;
				    }
		        }

		 		pu4GoldData+=4;
		 		for(k=0;k<4;k++)
			    {
				 	if(prDispInfo->u4Ccrc[k]!=pu4GoldData[k])
					{
						LOG(0, "//Error!c %s Data Mismatch at frame=%d, index= %x, decode = 0x%.2x, Golden = 0x%.2x !!! \n", 
							  _acFileName, i,k,prDispInfo->u4Ccrc[k], pu4GoldData[k]);
						return FALSE;
					}
				 
			    }
		
			}

		 	
		}
	return TRUE;


}

void _VdecNotifyCRC(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4) //for h264
{
    VDEC_DISP_PIC_INFO_T* prDispInfo;
	BOOL bIsCmpPass = TRUE, bFileLoad = FALSE;
	prDispInfo = (VDEC_DISP_PIC_INFO_T*)u4Arg2;
    if(u4Arg1 == (UINT32)VDEC_EVT_DISP_READY)
    {
	if(_bIsFirstFrm)
	{
		Printf("_bFileLoadSuccess");
        sprintf(_acFileName, "%s/%s.CRC", 
                        _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx]);
		if(!fgSaveCrc)
		{
	   		bFileLoad = _LoadFile(_acFileName, _pu1GoldenDatacrc);
		}
		else
		{
			u4CrcSize = 0;
			bFileLoad = TRUE;
		}
	    Printf("fileadd=%x\n",_pu1GoldenDatacrc);
	    Printf("bFileLoad=%d",bFileLoad);
	    if(bFileLoad)
	   	{ 
		   _bFileLoadSuccess = TRUE;
           _bIsFirstFrm = FALSE;
		   Printf("_bFileLoadSuccess=%d",_bFileLoadSuccess);
		   Printf("_bIsFirstFrm=%d",_bIsFirstFrm);
	    }
	}
	else
	{
          _u4CurFrmIdx++;
		  //Printf("\n_u4CurFrmIdx=%d\n",_u4CurFrmIdx);
	}
    if(_bFileLoadSuccess)
    {
		bIsCmpPass = _VdecVerifyDecDataCRC(prDispInfo, _u4CurFrmIdx);
		if(fgSaveCrc)
		{
			u4CrcSize += 32;
		}
    }
	if(!bIsCmpPass)
    {
        //_VDEC_DumpReg();
		Printf("ERROR ERROR ERROR\n");

		
        #if 1//def CC_DRIVER_DEMO
        LOG(0, "//d.save.binary \"%s/Err%d_Y_decDump.out\" 0x%08x--0x%08x\n",
        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx, 
        (UINT32)prDispInfo->pvYAddr, (UINT32)prDispInfo->pvYAddr + prDispInfo->u4YSize);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/Err%d_Y_decDump.out\" 0x%08x--0x%08x", 
        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx,
        (UINT32)prDispInfo->pvYAddr, (UINT32)prDispInfo->pvYAddr + prDispInfo->u4YSize));
        
        LOG(0, "//d.save.binary \"%s/Err%d_CbCr_decDump.out\" 0x%08x--0x%08x\n",
        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx, 
        (UINT32)prDispInfo->pvCAddr, (UINT32)prDispInfo->pvCAddr + prDispInfo->u4CSize);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/Err%d_CbCr_decDump.out\" 0x%08x--0x%08x", 
        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx,
        (UINT32)prDispInfo->pvCAddr, (UINT32)prDispInfo->pvCAddr + prDispInfo->u4CSize));
        #endif
    }
   }
   else if(u4Arg1 == (UINT32)VDEC_EVT_DEC_BS_END)
   {
       Printf("\nVDEC_EVT_DEC_BS_END\n");
       VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
   }
}

#endif
void _VdecNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4) //for h264
{
	#ifdef CC_TESTFLOW
	_VdecNotifyCRC(u4Arg1, u4Arg2, u4Arg3, u4Arg4);
	#else
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    static UINT8 u1PicStruct = 0;
    static UINT32 u4YAddr = 0;
    BOOL bIsCmpPass = TRUE, bFileLoad = FALSE;

#ifdef IDE_READ_SUPPORT
    UINT32 u4IDERetryCnt = 0;
#endif

    prDispInfo = (VDEC_DISP_PIC_INFO_T*)u4Arg2;
    ASSERT(prDispInfo != NULL);

    if (!_bSaveOutput)   //normal comparing verification
    {
        if (u4Arg1 == (UINT32)VDEC_EVT_DISP_READY)
        {
#ifndef RR_TEST
            if (((UINT32)prDispInfo->pvYAddr != u4YAddr) && (!_bIsFirstFrm))
            {
                _u4CurFrmIdx++;
            }
            _bIsFirstFrm = FALSE;
            u4YAddr = (UINT32)prDispInfo->pvYAddr;

            if (_bCmpEn && _bCmpFrmEn[_u4CurFrmIdx])
            {
                //compare decoded golden data: Y
                if ((_u4YRREn > 0) || (_u4CRREn > 0))
                {
                    sprintf(_acFileName, "%s/%s.26l_%05d.bitY",
                            _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                }
                else
                {
                    sprintf(_acFileName, "%s/%s_%d_Y.out",
                            _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                }
#ifdef IDE_WRITE_SUPPORT
                bIsCmpPass = _IDEComp(_acFileName, (UINT32)prDispInfo->pvYAddr, prDispInfo->u4YSize);
#else
#ifdef  IDE_READ_SUPPORT
                LOG(0, "//%s_%d_Y.out\n", _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
#endif
                if (!_fgEnChkSum)
                {
                    bFileLoad = _LoadFile(_acFileName, (UINT32)_pu1GoldenData);
                }
                if (_fgEnChkSum || bFileLoad)
                {
#ifdef CC_64x32BLK_CMP
                    _VdecBlkTransform(_pu1DecData16x32, prDispInfo, TRUE);
#endif
                    bIsCmpPass = _VdecVerifyDecData(prDispInfo, TRUE);
                }
#endif

                //compare decoded golden data: C
                if (prDispInfo->u4ChromaFormatIdc > 0)
                {
                    if ((_u4YRREn > 0) || (_u4CRREn > 0))
                    {
                        sprintf(_acFileName, "%s/%s.26l_%05d.bitC",
                                _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                    }
                    else
                    {
                        sprintf(_acFileName, "%s/%s_%d_CbCr.out",
                                _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                    }

#ifdef IDE_WRITE_SUPPORT
                    bIsCmpPass &= _IDEComp(_acFileName, (UINT32)prDispInfo->pvCAddr, prDispInfo->u4CSize);
#else
#ifdef  IDE_READ_SUPPORT
                    LOG(0, "//%s_%d_CbCr.out\n", _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
#endif
                    if (!_fgEnChkSum)
                    {
                        bFileLoad = _LoadFile(_acFileName, (UINT32)_pu1GoldenData);
                    }
                    if (_fgEnChkSum || bFileLoad)
                    {
#ifdef CC_64x32BLK_CMP
                        _VdecBlkTransform(_pu1DecData16x32, prDispInfo, FALSE);
#endif
                        bIsCmpPass &= _VdecVerifyDecData(prDispInfo, FALSE);
                    }
#endif
                }

#ifdef MEMRW_TEST
                x_thread_delay(500);
#endif

                if (!bIsCmpPass)
                {
                    //_VDEC_DumpReg();
#ifdef CC_DRIVER_DEMO
                    LOG(0, "//d.save.binary \"%s/Err%d_Y_decDump.out\" 0x%08x--0x%08x\n",
                        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx,
                        (UINT32)prDispInfo->pvYAddr, (UINT32)prDispInfo->pvYAddr + prDispInfo->u4YSize);
                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/Err%d_Y_decDump.out\" 0x%08x--0x%08x",
                                        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx,
                                        (UINT32)prDispInfo->pvYAddr, (UINT32)prDispInfo->pvYAddr + prDispInfo->u4YSize));

                    LOG(0, "//d.save.binary \"%s/Err%d_CbCr_decDump.out\" 0x%08x--0x%08x\n",
                        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx,
                        (UINT32)prDispInfo->pvCAddr, (UINT32)prDispInfo->pvCAddr + prDispInfo->u4CSize);
                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/Err%d_CbCr_decDump.out\" 0x%08x--0x%08x",
                                        _acFileDir[_u4CurFileIdx], _u4CurFrmIdx,
                                        (UINT32)prDispInfo->pvCAddr, (UINT32)prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
                }
            }
#else
            _VdecVerifyDecData(prDispInfo, TRUE);
            _VdecVerifyDecData(prDispInfo, FALSE);
#endif

            u1PicStruct |= prDispInfo->u1PicStruct;
            if (u1PicStruct == FRAME)           //wait for both top and bottom field
            {
#ifndef RR_TEST
                //if(!((_bRptFrmEn) && (_u4RptFrmNo==_i4CurFrmIdx)))
                //{
                //    _i4CurFrmIdx++;
                //}
#endif
                u1PicStruct = 0;
            }

            if ((_bRptFrmEn) && (_u4RptFrmNo == _u4CurFrmIdx))
            {
                VDEC_SetParam(ES0, REPEAT_FRM, 1, 0, 0);
            }
            else
            {
                VDEC_SetParam(ES0, REPEAT_FRM, 0, 0, 0);    //repeat frame disable
            }
        }
        else if (u4Arg1 == (UINT32)VDEC_EVT_DEC_BS_END)
        {
            VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
        }
    }
    else if (_bSaveOutput)                    //save to PC
    {
        UINT32 u4StartAddr;
        UINT32 u4EndAddr;

        //Y
        if (u4Arg3 == 0)
        {
            _Vdec1389B2R(_pu1GoldenData, *prDispInfo, TRUE);
            u4StartAddr = (UINT32)_pu1GoldenData;
            u4EndAddr = (UINT32)_pu1GoldenData + prDispInfo->u4YSize - 1;
        }
        else
        {
            u4StartAddr = (UINT32)prDispInfo->pvYAddr;
            if (u4Arg4 == 0)   // 1/2 RR mode
            {
                u4EndAddr = (UINT32)prDispInfo->pvYAddr + (prDispInfo->u4YSize >> 1) - 1;
            }
            else
            {
                u4EndAddr = (UINT32)prDispInfo->pvYAddr + ((prDispInfo->u4YSize * 3) >> 2) - 1;
            }
        }
#ifdef CC_DRIVER_DEMO
        LOG(6, "//d.save.binary \"%s/%s_%d_decY.out\" 0x%x--0x%x\n",
            _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx,
            u4StartAddr, u4EndAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s_%d_decY.out\" 0x%x--0x%x",
                            _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx,
                            u4StartAddr, u4EndAddr));
#endif

        //C
        if (u4Arg3 == 0)
        {
            _Vdec1389B2R(_pu1GoldenData, *prDispInfo, FALSE);
            u4StartAddr = (UINT32)_pu1GoldenData;
            u4EndAddr = (UINT32)_pu1GoldenData + prDispInfo->u4CSize - 1;
        }
        else
        {
            u4StartAddr = (UINT32)prDispInfo->pvCAddr;
            if (u4Arg4 == 0)   // 1/2 RR mode
            {
                u4EndAddr = (UINT32)prDispInfo->pvCAddr + (prDispInfo->u4CSize >> 1) - 1;
            }
            else
            {
                u4EndAddr = (UINT32)prDispInfo->pvCAddr + ((prDispInfo->u4CSize * 3) >> 2) - 1;
            }
        }
#ifdef CC_DRIVER_DEMO
        LOG(6, "//d.save.binary \"%s/%s_%d_decC.out\" 0x%x--0x%x\n",
            _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx,
            u4StartAddr, u4EndAddr);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s_%d_decC.out\" 0x%x--0x%x",
                            _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx,
                            u4StartAddr, u4EndAddr));
#endif

        u1PicStruct |= prDispInfo->u1PicStruct;
        if (u1PicStruct == FRAME)   //wait for both top and bottom field
        {
            _u4CurFrmIdx++;
            u1PicStruct = 0;
        }
    }

    if (_i4SSimPatFrmNo == _u4CurFrmIdx)
    {
        gu1VdecLogSimPat = 1;
        VDEC_SetParam(ES0, LOG_SIMU_PAT, 1, 0, 0);
    }
    else
    {
        gu1VdecLogSimPat = 0;
        VDEC_SetParam(ES0, LOG_SIMU_PAT, 0, 0, 0);
    }
    #endif
}

void _VdecMPEG4Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    char acPath[MAX_FILENAME_LEN];
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    static UINT8 u1PicStruct = 0;
    UINT32* pu4YRetVal, *pu4CbCrRetVal;
    BOOL fgCompPass = TRUE;
    
    x_memset(acPath, 0, MAX_FILENAME_LEN);
    
    if(u4Arg1 == (UINT32)VDEC_EVT_DISP_READY)
    {
        /*prDispInfo = (VDEC_DISP_PIC_INFO_T*)u4Arg2;
        ASSERT(prDispInfo!=NULL);

        if (_u4CurFrmIdx <= _au4FrameNumber[_u4CurFileIdx])
        {
            x_memset(acPath, 0, MAX_FILENAME_LEN);

            if(!_fgEnChkSum)
            {
                sprintf(acPath, "%s_%d_Y.out",
                    _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);

#if 0
                Printf("d.load.binary %s 0x%08x--0x%08x /ny\n", acPath, (UINT32)_pu1GoldenData, 
                    (UINT32)(_pu1GoldenData + GOLDEN_SIZE));
#endif
                x_memset(_pu1GoldenData, 0, GOLDEN_SIZE);   
                if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                {
			#ifdef CC_64x32BLK_CMP
			_VdecBlkTransform(_pu1DecData16x32, prDispInfo, TRUE);
			#endif
                    fgCompPass = _VdecVerifyMpeg4DecData(_pu1GoldenData, *prDispInfo, TRUE);
                }
                else
                {
                    if (!_fgLoadFail)
                    {
                          #ifdef CC_DRIVER_DEMO
                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y_Fail_Load.out\" 0x%08x--0x%08x", 
                            "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
                                    #else
                                    LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
                                    #endif
                        _fgLoadFail = TRUE;
                    }
                }
            }
            else
            {
                fgCompPass = _VdecVerifyMpeg4DecData(_pu1GoldenData, *prDispInfo, TRUE);
            }

            if (fgCompPass == FALSE)
            {
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x", 
                "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, 
                (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
            }

            if(!_fgEnChkSum)
            {
                sprintf(acPath, "%s_%d_CbCr.out",
                    _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
#if 0
                Printf("d.load.binary %s to 0x%08x--0x%08x /ny \n", acPath, (UINT32)_pu1GoldenData, 
                    (UINT32)(_pu1GoldenData + GOLDEN_SIZE));
#endif
                if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                {
#ifdef CC_64x32BLK_CMP
			_VdecBlkTransform(_pu1DecData16x32, prDispInfo, FALSE);
#endif
                    fgCompPass = _VdecVerifyMpeg4DecData(_pu1GoldenData, *prDispInfo, FALSE);
                }
                else
                {
                    if (!_fgLoadFail)
                    {
                          #ifdef CC_DRIVER_DEMO
                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr_Fail_Load.out\" 0x%08x--0x%08x", 
                                            "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
                                    #else
                                    LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
                                    #endif
                        _fgLoadFail = TRUE;
                    }
                }
            }
            else
            {
                fgCompPass = _VdecVerifyMpeg4DecData(_pu1GoldenData, *prDispInfo, FALSE);
            }

            if (fgCompPass == FALSE)
            {
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x", 
                "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, 
                (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
            }
        }

        u1PicStruct |= prDispInfo->u1PicStruct;
        //if(prDispInfo->u1PicStruct == FRAME)
        {
            _u4CurFrmIdx++;
            //_u4DecFrmIdx++;
            u1PicStruct = 0;
        }*/
    }
    else if(u4Arg1 == (UINT32)VDEC_EVT_DEC_BS_END)
    {
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_MP4);
    }
}
void _VdecAVSNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    #define DUMP_REGISTER_WHEN_ERROR
    //#define DISABLE_ASSERT
    //#define DISABLE_GOLDEN_FOR_TEMP
//#define TEST_UNSTABLE   //do not compare golden or crc
#ifdef TEST_UNSTABLE
//#define DUMP_YUV_ALWAYS
#define START_DUMP_IDX (0)
#endif

    char acPath[MAX_FILENAME_LEN];
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    static UINT8 u1PicStruct = 0;
    UINT32* pu4YRetVal, *pu4CbCrRetVal;
    BOOL fgVfyFail = FALSE;

    x_memset(acPath, 0, MAX_FILENAME_LEN);

    LOG(0, "######VDEC AVS Notify from [_u4CurFileIdx = %d] file [_u4CurFrmIdx = %d] Frame \n", _u4CurFileIdx, _u4CurFrmIdx);
    LOG(0, "######The While Loop Times = %d \n", (_u4DecCycles + 1));
    LOG(0, "######Total file count = %d \n", _u4FileCount);
    LOG(0, "######Total frame/field count = %d \n", _au4FrmFldNumberTotal);
    LOG(0, "######Current valid frame/field count = %d \n", _au4FrameNumberCurValidTotal);
    LOG(0, "######Total frame count  = %d \n", _au4FrameNumber[_u4CurFileIdx]);
    LOG(0, "######To compare golded = %d \n", _afgLoadGoldenData[_u4CurFileIdx]);
#ifdef CC_64x32BLK_CMP
    LOG(0, "$$$$$$ Remind: Now run on DDR3 $$$$$$\n");
#endif    

    if (u4Arg1 == (UINT32)VDEC_EVT_DISP_READY)
    {

        prDispInfo = (VDEC_DISP_PIC_INFO_T*)u4Arg2;
        ASSERT(prDispInfo != NULL);
#ifdef TEST_UNSTABLE
        if ( FALSE )
#else
#ifdef DISABLE_GOLDEN_FOR_TEMP
        if (_afgLoadGoldenData[_u4CurFileIdx] && _fgDisableGlolden == FALSE )
#else
        if (_afgLoadGoldenData[_u4CurFileIdx])
#endif
#endif            
        {
            if ((_u4CurFrmIdx <= _au4FrameNumber[_u4CurFileIdx]))
            {
#ifndef CC_EMULATION_COM_CRC

                x_memset(acPath, 0, MAX_FILENAME_LEN);
                sprintf(acPath, "%s_%d_Y.out",
                        _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                LOG(0, "Loading file name is  =  \n\"%s\"\n", acPath);

                fgVfyFail = FALSE;

#if 0
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\mv.buf\" 0x%08x--0x%08x",
                                    (UINT32)(0x0BA4B000), (UINT32)(0x0BA46000 + 0x87000)));
                LOG(0, "d.load.binary %s 0x%08x--0x%08x /ny\n", acPath, (UINT32)_pu1GoldenData,
                    (UINT32)(_pu1GoldenData + GOLDEN_SIZE));
#endif

#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
                if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                {
#if 0
#if defined(CC_MT5395)
                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                    HalFlushInvalidateDCache();
#endif
#else
                    HalFlushInvalidateDCache();
#endif
#ifdef CC_64x32BLK_CMP
                    _VdecBlkTransform(_pu1DecData16x32, prDispInfo, TRUE);
#endif
                    if (!_VdecVerifyDecData(prDispInfo, TRUE))
                    {
#if 0//def CC_DUMP_RASTER
                        _Vdec1389B2R(_pu1GoldenData, *prDispInfo, TRUE);
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif
                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                            "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                            (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4YSize)));
#else
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif
#ifdef DUMP_REGISTER_WHEN_ERROR
                        LOG(0, "~~~~~~Y ERROR, Dump Register Start~~~~~~ \n");
                        _VDEC_DumpAvsReg();
                        LOG(0, "!!!!!!Y ERROR, Dump Register End!!!!!! \n");
#endif

                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                            "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                            (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
#endif
                        fgVfyFail = TRUE;
                        LOG(0, "!!!!!!VERIFY Y ERROR!!!!!!\nVDEC AVS Notify from %d file %d Frame \n!!!!!!VERIFY Y ERROR!!!!!!\n", _u4CurFileIdx, _u4CurFrmIdx);
                        LOG(0, "The dump Y addr = 0x%X \n The dump Y size = 0x%X \n", (UINT32)prDispInfo->pvYAddr, prDispInfo->u4YSize);
                    }
                }
                else
                {
                    if (!_fgLoadFail)
                    {

#ifdef CC_DRIVER_DEMO
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif

                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y_Fail_Load.out\" 0x%08x--0x%08x",
                                            "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
#else
                        LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif

                        _fgLoadFail = TRUE;
                    }
                }

                x_memset(acPath, 0, MAX_FILENAME_LEN);
                sprintf(acPath, "%s_%d_CbCr.out",
                        _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                LOG(0, "Loading file name is  =  \n\"%s\"\n", acPath);

#if 0
                LOG(0, "d.load.binary %s to 0x%08x--0x%08x /ny \n", acPath, (UINT32)_pu1GoldenData,
                    (UINT32)(_pu1GoldenData + GOLDEN_SIZE));
#endif

#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
                if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                {
#if 0
#if defined(CC_MT5395)
                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                    HalFlushInvalidateDCache();
#endif
#else
                    HalFlushInvalidateDCache();
#endif
#ifdef CC_64x32BLK_CMP
                    _VdecBlkTransform(_pu1DecData16x32, prDispInfo, FALSE);
#endif
                    if (!_VdecVerifyDecData(prDispInfo, FALSE))
                    {
#if 0//def CC_DUMP_RASTER
                        _Vdec1389B2R(_pu1GoldenData, *prDispInfo, FALSE);
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif
                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                            "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                            (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4CSize)));
#else
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif

#ifdef DUMP_REGISTER_WHEN_ERROR
                        LOG(0, "~~~~~~CBCR ERROR, Dump Register Start~~~~~~ \n");
                        _VDEC_DumpAvsReg();
                        LOG(0, "!!!!!!CBCR ERROR, Dump Register End!!!!!! \n");
#endif

                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                            "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                            (UINT32)prDispInfo->pvCAddr, (UINT32)(prDispInfo->pvCAddr + prDispInfo->u4CSize)));
#endif
                        fgVfyFail = TRUE;
                        LOG(0, "!!!!!!VERIFY CBCR ERROR!!!!!!\nVDEC AVS Notify from %d file %d Frame \n!!!!!!VERIFY CBCR ERROR!!!!!!\n", _u4CurFileIdx, _u4CurFrmIdx);
                        LOG(0, "The dump CBCR addr = 0x%X \n The dump CBCR size = 0x%X \n", (UINT32)prDispInfo->pvCAddr, prDispInfo->u4CSize);
                    }
                }
                else
                {
                    if (!_fgLoadFail)
                    {
#ifdef CC_DRIVER_DEMO
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif
                        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr_Fail_Load.out\" 0x%08x--0x%08x",
                                            "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
#else
                        LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif
                        _fgLoadFail = TRUE;
                    }
                }

#ifndef DISABLE_ASSERT
                ASSERT(fgVfyFail == FALSE);
#endif

#ifdef DISABLE_GOLDEN_FOR_TEMP
                if ( fgVfyFail == TRUE )
                {
                    _fgDisableGlolden = TRUE;
                }
#endif

#endif //#ifndef CC_EMULATION_COM_CRC

                _au4FrameNumberCurValidTotal++;

#if  defined(CC_EMULATION_COM_CRC)

                if ( prDispInfo->rPicCRC.u1PicCRCCnt == 0 )
                {
                    ASSERT(FALSE);
                }
                if ( prDispInfo->rPicCRC.u4PicCRC_0[0] == 0 )
                {
                    ASSERT(FALSE);
                }
                LOG(0, "^^^^^^^^^^^^^^^^^^^^\n");
                LOG(0, "CRC Count = 0x%X\n", prDispInfo->rPicCRC.u1PicCRCCnt);
                LOG(0, "CRC_0 RW_CRC_Y_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[0]);
                LOG(0, "CRC_0 RW_CRC_Y_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[1]);
                LOG(0, "CRC_0 RW_CRC_Y_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[2]);
                LOG(0, "CRC_0 RW_CRC_Y_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[3]);
                LOG(0, "CRC_0 RW_CRC_C_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[4]);
                LOG(0, "CRC_0 RW_CRC_C_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[5]);
                LOG(0, "CRC_0 RW_CRC_C_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[6]);
                LOG(0, "CRC_0 RW_CRC_C_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[7]);
                if ( prDispInfo->rPicCRC.u1PicCRCCnt == 2 )
                {
                    if ( prDispInfo->rPicCRC.u4PicCRC_1[0] == 0 )
                    {
                        ASSERT(FALSE);
                    }
                    LOG(0, "CRC_1 RW_CRC_Y_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[0]);
                    LOG(0, "CRC_1 RW_CRC_Y_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[1]);
                    LOG(0, "CRC_1 RW_CRC_Y_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[2]);
                    LOG(0, "CRC_1 RW_CRC_Y_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[3]);
                    LOG(0, "CRC_1 RW_CRC_C_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[4]);
                    LOG(0, "CRC_1 RW_CRC_C_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[5]);
                    LOG(0, "CRC_1 RW_CRC_C_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[6]);
                    LOG(0, "CRC_1 RW_CRC_C_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[7]);
                }
                LOG(0, "^^^^^^^^^^^^^^^^^^^^\n");

                {
                    //Compare CRC
                    UINT32* pu4GoldData = _pu1GoldenData;

                    x_memset(acPath, 0, MAX_FILENAME_LEN);
                    sprintf(acPath, "%s%s%s_%d_CRC_[0].out",
                            _acTargetDir[_u4CurFileIdx], "\\CRC_INFO\\CRC\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx);
                    LOG(0, "Loading CRC_0 file name is  =  \n\"%s\"\n", acPath);

                    x_memset(_pu1GoldenData, 0, GOLDEN_SIZE);
#if 0
#if defined(CC_MT5395)
                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                    HalFlushInvalidateDCache();
#endif
#else
                    HalFlushInvalidateDCache();
#endif
                    if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                    {
                        LOG(0, "Load CRC_0  file name is  OK!!!It is  \n\"%s\"\n", acPath);
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif
                        LOG(0, "~~~~~~~~~~~~~~~~~~~~\n");
                        LOG(0, "pu4GoldData[0] = 0x%X\n", pu4GoldData[0]);
                        LOG(0, "pu4GoldData[1] = 0x%X\n", pu4GoldData[1]);
                        LOG(0, "pu4GoldData[2] = 0x%X\n", pu4GoldData[2]);
                        LOG(0, "pu4GoldData[3] = 0x%X\n", pu4GoldData[3]);
                        LOG(0, "pu4GoldData[4] = 0x%X\n", pu4GoldData[4]);
                        LOG(0, "pu4GoldData[5] = 0x%X\n", pu4GoldData[5]);
                        LOG(0, "pu4GoldData[6] = 0x%X\n", pu4GoldData[6]);
                        LOG(0, "pu4GoldData[7] = 0x%X\n", pu4GoldData[7]);
                        LOG(0, "~~~~~~~~~~~~~~~~~~~~\n");

                        if ( pu4GoldData[0] != prDispInfo->rPicCRC.u4PicCRC_0[0]
                                || pu4GoldData[1] != prDispInfo->rPicCRC.u4PicCRC_0[1]
                                || pu4GoldData[2] != prDispInfo->rPicCRC.u4PicCRC_0[2]
                                || pu4GoldData[3] != prDispInfo->rPicCRC.u4PicCRC_0[3]
                                || pu4GoldData[4] != prDispInfo->rPicCRC.u4PicCRC_0[4]
                                || pu4GoldData[5] != prDispInfo->rPicCRC.u4PicCRC_0[5]
                                || pu4GoldData[6] != prDispInfo->rPicCRC.u4PicCRC_0[6]
                                || pu4GoldData[7] != prDispInfo->rPicCRC.u4PicCRC_0[7]
                           )
                        {
                            fgVfyFail = TRUE;

#if 0
#if 0
#if defined(CC_MT5395)
                            HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
                            HalFlushInvalidateDCache();
#endif
#else
                            HalFlushInvalidateDCache();
#endif

                            UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                                "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
#if 0
#if defined(CC_MT5395)
                            HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                            HalFlushInvalidateDCache();
#endif
#else
                            HalFlushInvalidateDCache();
#endif

                            UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                                "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                (UINT32)prDispInfo->pvCAddr, (UINT32)(prDispInfo->pvCAddr + prDispInfo->u4CSize)));
#endif
                        }
                    }
                    else
                    {
                        if (!_fgLoadFail)
                        {
                            _fgLoadFail = TRUE;
                        }
                    }

                    ASSERT(_fgLoadFail == FALSE);

                    if ( fgVfyFail == TRUE ) //Compare golden file, as some case that golden file same but CRC different
                    {
                        LOG(0, "CRC_0  is  not matched !!!Need to compare golden to confirm!\n");

                        x_memset(acPath, 0, MAX_FILENAME_LEN);
                        sprintf(acPath, "%s_%d_Y.out",
                                _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                        LOG(0, "Loading golden file name is  =  \n\"%s\"\n", acPath);

                        fgVfyFail = FALSE;

#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif

                        if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                        {
#if 0
#if defined(CC_MT5395)
                            HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                            HalFlushInvalidateDCache();
#endif
#else
                            HalFlushInvalidateDCache();
#endif
#ifdef CC_64x32BLK_CMP
                            _VdecBlkTransform(_pu1DecData16x32, prDispInfo, TRUE);
#endif
                            if (!_VdecVerifyDecData(prDispInfo, TRUE))
                            {
#if 0//def CC_DUMP_RASTER
                                _Vdec1389B2R(_pu1GoldenData, *prDispInfo, TRUE);
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif
                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                    (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4YSize)));
#else
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif

                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                    (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
#endif

#ifdef DUMP_REGISTER_WHEN_ERROR
                                LOG(0, "~~~~~~Y ERROR, Dump Register Start~~~~~~ \n");
                                _VDEC_DumpAvsReg();
                                LOG(0, "!!!!!!Y ERROR, Dump Register End!!!!!! \n");
#endif

                                fgVfyFail = TRUE;
                                LOG(0, "!!!!!!VERIFY Y ERROR!!!!!!\nVDEC AVS Notify from %d file %d Frame \n!!!!!!VERIFY Y ERROR!!!!!!\n", _u4CurFileIdx, _u4CurFrmIdx);
                            }
                        }
                        else
                        {
                            if (!_fgLoadFail)
                            {

#ifdef CC_DRIVER_DEMO
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif

                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y_Fail_Load.out\" 0x%08x--0x%08x",
                                                    "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
#else
                                LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif

                                _fgLoadFail = TRUE;
                            }
                        }

                        ASSERT(_fgLoadFail == FALSE);
                        ASSERT(fgVfyFail == FALSE);

                        x_memset(acPath, 0, MAX_FILENAME_LEN);
                        sprintf(acPath, "%s_%d_CbCr.out",
                                _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                        LOG(0, "Loading golden file name is  =  \n\"%s\"\n", acPath);

#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif
                        if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                        {
#if 0
#if defined(CC_MT5395)
                            HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                            HalFlushInvalidateDCache();
#endif
#else
                            HalFlushInvalidateDCache();
#endif
#ifdef CC_64x32BLK_CMP
                            _VdecBlkTransform(_pu1DecData16x32, prDispInfo, FALSE);
#endif
                            if (!_VdecVerifyDecData(prDispInfo, FALSE))
                            {
#if 0//def CC_DUMP_RASTER
                                _Vdec1389B2R(_pu1GoldenData, *prDispInfo, FALSE);
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif
                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                    (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4CSize)));
#else
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif

                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                    (UINT32)prDispInfo->pvCAddr, (UINT32)(prDispInfo->pvCAddr + prDispInfo->u4CSize)));
#endif
#ifdef DUMP_REGISTER_WHEN_ERROR
                                LOG(0, "~~~~~~CBCR ERROR, Dump Register Start~~~~~~ \n");
                                _VDEC_DumpAvsReg();
                                LOG(0, "!!!!!!CBCR ERROR, Dump Register End!!!!!! \n");
#endif
                                fgVfyFail = TRUE;
                                LOG(0, "!!!!!!VERIFY CBCR ERROR!!!!!!\nVDEC AVS Notify from %d file %d Frame \n!!!!!!VERIFY CBCR ERROR!!!!!!\n", _u4CurFileIdx, _u4CurFrmIdx);
                            }
                        }
                        else
                        {
                            if (!_fgLoadFail)
                            {
#ifdef CC_DRIVER_DEMO
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif

                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr_Fail_Load.out\" 0x%08x--0x%08x",
                                                    "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
#else
                                LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif
                                _fgLoadFail = TRUE;
                            }
                        }

                        ASSERT(_fgLoadFail == FALSE);
                        ASSERT(fgVfyFail == FALSE);

                    }

                    if ( prDispInfo->rPicCRC.u1PicCRCCnt == 2 )
                    {
                        x_memset(acPath, 0, MAX_FILENAME_LEN);
                        sprintf(acPath, "%s%s%s_%d_CRC_[1].out",
                                _acTargetDir[_u4CurFileIdx], "\\CRC_INFO\\CRC\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx);
                        LOG(0, "Loading CRC_1 file name is  =  \n\"%s\"\n", acPath);

                        x_memset(_pu1GoldenData, 0, GOLDEN_SIZE);
#if 0
#if defined(CC_MT5395)
                        HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                        HalFlushInvalidateDCache();
#endif
#else
                        HalFlushInvalidateDCache();
#endif
                        if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                        {
                            LOG(0, "Load CRC_1  file name is  OK!!!It is  \n\"%s\"\n", acPath);
#if 0
#if defined(CC_MT5395)
                            HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                            HalFlushInvalidateDCache();
#endif
#else
                            HalFlushInvalidateDCache();
#endif
                            LOG(0, "~~~~~~~~~~~~~~~~~~~~\n");
                            LOG(0, "pu4GoldData[0] = 0x%X\n", pu4GoldData[0]);
                            LOG(0, "pu4GoldData[1] = 0x%X\n", pu4GoldData[1]);
                            LOG(0, "pu4GoldData[2] = 0x%X\n", pu4GoldData[2]);
                            LOG(0, "pu4GoldData[3] = 0x%X\n", pu4GoldData[3]);
                            LOG(0, "pu4GoldData[4] = 0x%X\n", pu4GoldData[4]);
                            LOG(0, "pu4GoldData[5] = 0x%X\n", pu4GoldData[5]);
                            LOG(0, "pu4GoldData[6] = 0x%X\n", pu4GoldData[6]);
                            LOG(0, "pu4GoldData[7] = 0x%X\n", pu4GoldData[7]);
                            LOG(0, "~~~~~~~~~~~~~~~~~~~~\n");

                            if ( pu4GoldData[0] != prDispInfo->rPicCRC.u4PicCRC_1[0]
                                    || pu4GoldData[1] != prDispInfo->rPicCRC.u4PicCRC_1[1]
                                    || pu4GoldData[2] != prDispInfo->rPicCRC.u4PicCRC_1[2]
                                    || pu4GoldData[3] != prDispInfo->rPicCRC.u4PicCRC_1[3]
                                    || pu4GoldData[4] != prDispInfo->rPicCRC.u4PicCRC_1[4]
                                    || pu4GoldData[5] != prDispInfo->rPicCRC.u4PicCRC_1[5]
                                    || pu4GoldData[6] != prDispInfo->rPicCRC.u4PicCRC_1[6]
                                    || pu4GoldData[7] != prDispInfo->rPicCRC.u4PicCRC_1[7]
                               )
                            {
                                fgVfyFail = TRUE;

#if 0
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif

                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                    (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif

                                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                    (UINT32)prDispInfo->pvCAddr, (UINT32)(prDispInfo->pvCAddr + prDispInfo->u4CSize)));
#endif
                            }
                        }
                        else
                        {
                            if (!_fgLoadFail)
                            {
                                _fgLoadFail = TRUE;
                            }
                        }

                        ASSERT(_fgLoadFail == FALSE);

                        if ( fgVfyFail == TRUE ) //Compare golden file, as some case that golden file same but CRC different
                        {
                            LOG(0, "CRC_1  is  not matched !!!Need to compare golden to confirm!\n");

                            x_memset(acPath, 0, MAX_FILENAME_LEN);
                            sprintf(acPath, "%s_%d_Y.out",
                                    _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                            LOG(0, "Loading golden file name is  =  \n\"%s\"\n", acPath);

                            fgVfyFail = FALSE;

#if 0
#if defined(CC_MT5395)
                            HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                            HalFlushInvalidateDCache();
#endif
#else
                            HalFlushInvalidateDCache();
#endif

                            if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                            {
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif
#ifdef CC_64x32BLK_CMP
                                _VdecBlkTransform(_pu1DecData16x32, prDispInfo, TRUE);
#endif
                                if (!_VdecVerifyDecData(prDispInfo, TRUE))
                                {
#if 0//def CC_DUMP_RASTER
                                    _Vdec1389B2R(_pu1GoldenData, *prDispInfo, TRUE);
#if 0
#if defined(CC_MT5395)
                                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                    HalFlushInvalidateDCache();
#endif
#else
                                    HalFlushInvalidateDCache();
#endif
                                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                                        "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                        (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4YSize)));
#else
#if 0
#if defined(CC_MT5395)
                                    HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
                                    HalFlushInvalidateDCache();
#endif
#else
                                    HalFlushInvalidateDCache();
#endif

                                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                                        "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                        (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
#endif
#ifdef DUMP_REGISTER_WHEN_ERROR
                                    LOG(0, "~~~~~~Y ERROR, Dump Register Start~~~~~~ \n");
                                    _VDEC_DumpAvsReg();
                                    LOG(0, "!!!!!!Y ERROR, Dump Register End!!!!!! \n");
#endif
                                    fgVfyFail = TRUE;
                                    LOG(0, "!!!!!!VERIFY Y ERROR!!!!!!\nVDEC AVS Notify from %d file %d Frame \n!!!!!!VERIFY Y ERROR!!!!!!\n", _u4CurFileIdx, _u4CurFrmIdx);
                                }
                            }
                            else
                            {
                                if (!_fgLoadFail)
                                {

#ifdef CC_DRIVER_DEMO
#if 0
#if defined(CC_MT5395)
                                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                    HalFlushInvalidateDCache();
#endif
#else
                                    HalFlushInvalidateDCache();
#endif
                                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y_Fail_Load.out\" 0x%08x--0x%08x",
                                                        "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
#else
                                    LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif

                                    _fgLoadFail = TRUE;
                                }
                            }

                            ASSERT(_fgLoadFail == FALSE);
                            ASSERT(fgVfyFail == FALSE);

                            x_memset(acPath, 0, MAX_FILENAME_LEN);
                            sprintf(acPath, "%s_%d_CbCr.out",
                                    _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                            LOG(0, "Loading golden file name is  =  \n\"%s\"\n", acPath);

#if 0
#if defined(CC_MT5395)
                            HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                            HalFlushInvalidateDCache();
#endif
#else
                            HalFlushInvalidateDCache();
#endif
                            if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
                            {
#if 0
#if defined(CC_MT5395)
                                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                HalFlushInvalidateDCache();
#endif
#else
                                HalFlushInvalidateDCache();
#endif
#ifdef CC_64x32BLK_CMP
                                _VdecBlkTransform(_pu1DecData16x32, prDispInfo, FALSE);
#endif
                                if (!_VdecVerifyDecData(prDispInfo, FALSE))
                                {
#if 0//def CC_DUMP_RASTER
                                    _Vdec1389B2R(_pu1GoldenData, *prDispInfo, FALSE);
#if 0
#if defined(CC_MT5395)
                                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                    HalFlushInvalidateDCache();
#endif
#else
                                    HalFlushInvalidateDCache();
#endif
                                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                                        "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                        (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4CSize)));
#else
#if 0
#if defined(CC_MT5395)
                                    HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                                    HalFlushInvalidateDCache();
#endif
#else
                                    HalFlushInvalidateDCache();
#endif

                                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                                        "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                                        (UINT32)prDispInfo->pvCAddr, (UINT32)(prDispInfo->pvCAddr + prDispInfo->u4CSize)));
#endif
#ifdef DUMP_REGISTER_WHEN_ERROR
                                    LOG(0, "~~~~~~CBCR ERROR, Dump Register Start~~~~~~ \n");
                                    _VDEC_DumpAvsReg();
                                    LOG(0, "!!!!!!CBCR ERROR, Dump Register End!!!!!! \n");
#endif
                                    fgVfyFail = TRUE;
                                    LOG(0, "!!!!!!VERIFY CBCR ERROR!!!!!!\nVDEC AVS Notify from %d file %d Frame \n!!!!!!VERIFY CBCR ERROR!!!!!!\n", _u4CurFileIdx, _u4CurFrmIdx);
                                }
                            }
                            else
                            {
                                if (!_fgLoadFail)
                                {
#ifdef CC_DRIVER_DEMO
#if 0
#if defined(CC_MT5395)
                                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                                    HalFlushInvalidateDCache();
#endif
#else
                                    HalFlushInvalidateDCache();
#endif
                                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr_Fail_Load.out\" 0x%08x--0x%08x",
                                                        "c:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
#else
                                    LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif
                                    _fgLoadFail = TRUE;
                                }
                            }

                            ASSERT(_fgLoadFail == FALSE);
                            ASSERT(fgVfyFail == FALSE);

                        }

                    }
                }

#endif //#if  defined(CC_EMULATION_COM_CRC)

            }
            else
            {
                LOG(0, "!******!\nDo nothing because _u4CurFrmIdx@%d@ > _au4FrameNumber[_u4CurFileIdx]@%d@ \n!******!\n", _u4CurFrmIdx, _au4FrameNumber[_u4CurFileIdx]);
#if 0//defined(CC_EMULATION_GEN_CRC)    
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif

                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                    (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif

                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                    (UINT32)prDispInfo->pvCAddr, (UINT32)(prDispInfo->pvCAddr + prDispInfo->u4CSize)));
#endif
            }
        }
        else
        {
#ifdef CC_EMULATION_COM_CRC
#ifndef TEST_UNSTABLE
            LOG(0, "!!!ERROR!!!\nMust compare golden when CC_EMULATION_COM_CRC \n");
            ASSERT(FALSE);
#endif
#else
            LOG(0, "!!!CAUTION!!!\nDo not compare golden now\n");
#endif

#ifdef DUMP_YUV_ALWAYS
            if ( _u4CurFrmIdx > START_DUMP_IDX )
            {
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvYAddr, (prDispInfo->pvYAddr + prDispInfo->u4YSize));
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
#if 0//def DUMP_REGISTER_WHEN_ERROR
                LOG(0, "~~~~~~Y ERROR, Dump Register Start~~~~~~ \n");
                _VDEC_DumpAvsReg();
                LOG(0, "!!!!!!Y ERROR, Dump Register End!!!!!! \n");
#endif
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                    "d:\\unstable\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                    (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
#if 0//def DUMP_REGISTER_WHEN_ERROR
                LOG(0, "~~~~~~CBCR ERROR, Dump Register Start~~~~~~ \n");
                _VDEC_DumpAvsReg();
                LOG(0, "!!!!!!CBCR ERROR, Dump Register End!!!!!! \n");
#endif
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                    "d:\\unstable\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                    (UINT32)prDispInfo->pvCAddr, (UINT32)(prDispInfo->pvCAddr + prDispInfo->u4CSize)));
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)prDispInfo->pvCAddr, (prDispInfo->pvCAddr + prDispInfo->u4CSize));
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
            }
#endif


#if 0//def CC_DUMP_RASTER
            char acFileName[MAX_FILENAME_LEN];
            //if (_u4CurFrmIdx > 890)
            {
                LOG(5, "_u4CurFrmIdx = %d\n", _u4CurFrmIdx);
                _Vdec1389B2R(_pu1GoldenData, *prDispInfo, TRUE);
#ifndef IDE_WRITE_SUPPORT
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif

                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x",
                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                    (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4YSize)));
#else
                sprintf(acFileName, "d:\\%s_%d_Y.out", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx);
                _IDEComp(acFileName, _pu1GoldenData, prDispInfo->u4YSize);
#endif
                _Vdec1389B2R(_pu1GoldenData, *prDispInfo, FALSE);
#ifndef IDE_WRITE_SUPPORT
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CbCr.out\" 0x%08x--0x%08x",
                                    "d:\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                    (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4CSize)));
#else
                sprintf(acFileName, "d:\\%s_%d_CbCr.out", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx);
                _IDEComp(acFileName, _pu1GoldenData, prDispInfo->u4CSize);
#endif
            }
#endif
        }

        u1PicStruct |= prDispInfo->u1PicStruct;

#if defined(CC_EMULATION_GEN_CRC)

        if (_fgLoadFail == FALSE)
        {
            if ( prDispInfo->rPicCRC.u1PicCRCCnt == 0 )
            {
                ASSERT(FALSE);
            }
            if ( prDispInfo->rPicCRC.u4PicCRC_0[0] == 0 )
            {
                ASSERT(FALSE);
            }
            LOG(0, "^^^^^^^^^^^^^^^^^^^^\n");
            LOG(0, "GEN CRC Count = 0x%X\n", prDispInfo->rPicCRC.u1PicCRCCnt);
            LOG(0, "GEN CRC_0 RW_CRC_Y_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[0]);
            LOG(0, "GEN CRC_0 RW_CRC_Y_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[1]);
            LOG(0, "GEN CRC_0 RW_CRC_Y_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[2]);
            LOG(0, "GEN CRC_0 RW_CRC_Y_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[3]);
            LOG(0, "GEN CRC_0 RW_CRC_C_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[4]);
            LOG(0, "GEN CRC_0 RW_CRC_C_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[5]);
            LOG(0, "GEN CRC_0 RW_CRC_C_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[6]);
            LOG(0, "GEN CRC_0 RW_CRC_C_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_0[7]);
            if ( prDispInfo->rPicCRC.u1PicCRCCnt == 2 )
            {
                if ( prDispInfo->rPicCRC.u4PicCRC_1[0] == 0 )
                {
                    ASSERT(FALSE);
                }
                LOG(0, "GEN CRC_1 RW_CRC_Y_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[0]);
                LOG(0, "GEN CRC_1 RW_CRC_Y_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[1]);
                LOG(0, "GEN CRC_1 RW_CRC_Y_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[2]);
                LOG(0, "GEN CRC_1 RW_CRC_Y_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[3]);
                LOG(0, "GEN CRC_1 RW_CRC_C_0 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[4]);
                LOG(0, "GEN CRC_1 RW_CRC_C_1 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[5]);
                LOG(0, "GEN CRC_1 RW_CRC_C_2 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[6]);
                LOG(0, "GEN CRC_1 RW_CRC_C_3 = 0x%X\n", prDispInfo->rPicCRC.u4PicCRC_1[7]);
            }
            LOG(0, "^^^^^^^^^^^^^^^^^^^^\n");

#if 0
#if defined(CC_MT5395)
            HalFlushInvalidateDCacheMultipleLine((UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_0[0])), ((UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_0[0])) + 0x20 - 1));
#endif
#if defined(CC_MT5365)
            HalFlushInvalidateDCache();
#endif
#else
            HalFlushInvalidateDCache();
#endif

            UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CRC_[0].out\" 0x%08x--0x%08x",
                                "d:\\CRC\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                (UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_0[0])), (UINT32)((UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_0[0])) + 0x20 - 1)));
            if ( prDispInfo->rPicCRC.u1PicCRCCnt == 2 )
            {
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine((UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_1[0])), ((UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_1[0])) + 0x20 - 1));
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
                UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_CRC_[1].out\" 0x%08x--0x%08x",
                                    "d:\\CRC\\", _acFilePrefix[_u4CurFileIdx], _u4CurFrmIdx,
                                    (UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_1[0])), (UINT32)((UINT32)(&(prDispInfo->rPicCRC.u4PicCRC_1[0])) + 0x20 - 1)));
            }
        }
#endif //#if defined(CC_EMULATION_GEN_CRC)
        {
            _u4CurFrmIdx++;
            u1PicStruct = 0;
        }
    }
    else if (u4Arg1 == (UINT32)VDEC_EVT_DEC_ERROR)
    {
#ifdef CC_EMULATION
        LOG(0, "@@@@@@E_VDEC_SKIPFRAME, so that VDEC_EVT_DEC_ERROR\n");
#endif
        _u4CurFrmIdx++;
    }
    else if (u4Arg1 == (UINT32)VDEC_EVT_DEC_BS_END)
    {
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_AVS);

#ifdef CC_EMULATION
        LOG(0, "@@@@@@Play the file to the end\n");
#endif

    }
}

#ifdef CC_VP8_EMULATION
static UINT32 _VdecWebPCallBack(UINT32 u4Tag,void *pvData,UINT32 u4CbType)
{
   WEBP_CFG_T *prWebCfg=(WEBP_CFG_T *)u4Tag;

   if(u4CbType==WEBP_NOTIFY_TYPE_REQDATA)
   {
      WEBP_DATA_REQ_INFO *prDataReqInfo=(WEBP_DATA_REQ_INFO *)pvData;
      x_memcpy((void *)prDataReqInfo->u4WritePosition,(void *)(prWebCfg->u4SrcBuffer+prDataReqInfo->u4FileOffset),prDataReqInfo->u4Size);
	  HalFlushInvalidateDCache();
	  return prDataReqInfo->u4Size;
   }
}

static void _VdecVP8Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
  #define VP8_RETRY_TIMES       3
  #define HW_BLK_W_LOG2         6   // 2^6=64
  #define GOLDEN_BLK_W_LOG2     4   // 2^4=16
  #define HW_BLK_W              ((1ul)<<HW_BLK_W_LOG2)
  #define GOLDEN_BLK_W          ((1ul)<<GOLDEN_BLK_W_LOG2)
  
  UINT32 u4Retry = 0;
  UINT32 dwWidth, dwHeight;
  UINT32 u4HwWidth, u4GoldenWidth;
  VDEC_DISP_PIC_INFO_T* prDispInfo;
  UINT8* golden_dataY = _pu1GoldenData;
  UINT8* golden_dataC = _pu1GoldenData;
  UINT8* hw_output;
  UINT32 i=0;
  UINT32 u4X, u4Y;
  UINT8* pu1GoldenAddr;
  UINT8* pu1HWAddr;
  INT8 fgY;
  BOOL fgSame=TRUE;
  UINT32 CurFrame;

 
  prDispInfo = (VDEC_DISP_PIC_INFO_T*)(u4Arg2);
  CurFrame = _u4CurFrmIdx+prDispInfo->u4CurrentPicNo;
  
  if (CurFrame < _u4StartFameIndex)
  {
    return;
     }
    
  if(u4Arg1 == VDEC_EVT_DEC_BS_END) 
  {

#ifdef CC_SUPPORT_LOG_INFO
      {
          sprintf(_acFileName, "d:/vp8_test/%s.log", _acFileDir[_u4CurFileIdx]);
          _SaveFile(_acFileName, _u4LogBufSa, _u4LogSz);

          _u4LogSz = 0;
          x_memset((void*)_u4LogBufSa, 0, _u4LogBufSz);
      }
#endif

     VDEC_Stop(0);
     VDEC_Init();
     VDEC_Play(0, VDEC_FMT_VP8);
     //VDEC_SetParam(ES0, (UINT32)DEC_MODE, _u4VP6DecodeMode, 0, 0);
     VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
  }
  else if(u4Arg1 == VDEC_EVT_DISP_READY) 
  {

    if(_fgEnableCRC)
    {
       if((CurFrame >= START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
       {
       UINT32 *u4CrcRptr=(UINT32 *)(_pcChkSum+32*_u4CrcIndex);
       BOOL fgFail=FALSE;
               
           Printf("[%d]\n", _u4CrcIndex);
       if(prDispInfo->u4Ycrc[0]==u4CrcRptr[0] && prDispInfo->u4Ycrc[1]==u4CrcRptr[1]
                  &&prDispInfo->u4Ycrc[2]==u4CrcRptr[2] && prDispInfo->u4Ycrc[3]==u4CrcRptr[3]
                  &&prDispInfo->u4Ccrc[0]==u4CrcRptr[4]&&prDispInfo->u4Ccrc[1]==u4CrcRptr[5]
                  &&prDispInfo->u4Ccrc[2]==u4CrcRptr[6]&&prDispInfo->u4Ccrc[3]==u4CrcRptr[7])
       {
                        //LOG(0, "Compare frame %d in CRC ok!!\n",CurFrame);
       }

       else
       {
              LOG(0, "%s\\%s CRC check Fail in frame (%d,%d)\n",_acFileDir[0],_acFileDir[_u4CurFileIdx],CurFrame,_u4CrcIndex);
              LOG(0, "GoldenCrc:\n0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[0],u4CrcRptr[1],u4CrcRptr[2],u4CrcRptr[3]);
              LOG(0, "0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[4],u4CrcRptr[5],u4CrcRptr[6],u4CrcRptr[7]);
              LOG(0, "FrameCrc:\n0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ycrc[0],prDispInfo->u4Ycrc[1],prDispInfo->u4Ycrc[2],prDispInfo->u4Ycrc[3]);
              LOG(0, "0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ccrc[0],prDispInfo->u4Ccrc[1],prDispInfo->u4Ccrc[2],prDispInfo->u4Ccrc[3]);
              _VdecCmdVp8DumpReg(1,NULL);
              x_thread_delay(20000);
              ASSERT(0);
          fgFail=TRUE;
       }
       _u4CrcIndex++;
       
       if(fgFail)
       {
          _fgAbtCurItem=TRUE;
       }
           //_u4CurFrmIdx++;
       }
       return ;
    }
    else if(_uGenCrcEn==1)
    {
       UCHAR *uCrcWptr=(CHAR *)(_pcChkSum+_u4CrcIndex);
       _pcChkSum[_u4CrcIndex]=prDispInfo->u1PicStruct+'0';
       //*u4CrcWptr++=prDispInfo->u4Ycrc[0];
       //*u4CrcWptr++=prDispInfo->u4Ycrc[1];
       //*u4CrcWptr++=prDispInfo->u4Ycrc[2];
       //*u4CrcWptr++=prDispInfo->u4Ycrc[3];
       //*u4CrcWptr++=prDispInfo->u4Ccrc[0];
       //*u4CrcWptr++=prDispInfo->u4Ccrc[1];
       //*u4CrcWptr++=prDispInfo->u4Ccrc[2];
       //*u4CrcWptr++=prDispInfo->u4Ccrc[3];
       _u4CrcIndex++;
       return;
    }

    for(fgY =1 ; fgY >=0 ; fgY--) 
    {
      if(fgY) 
      {        
         dwWidth = prDispInfo->u4W;
         dwHeight = prDispInfo->u4H;
         hw_output = prDispInfo->pvYAddr;

         u4HwWidth = dwWidth;
         if(dwWidth % HW_BLK_W) 
         {
            u4HwWidth = ((dwWidth / HW_BLK_W) + 1) * HW_BLK_W; //ming modify@2006/4/11
         }

         u4GoldenWidth = dwWidth;
         if(dwWidth % GOLDEN_BLK_W) 
         {
            u4GoldenWidth = ((dwWidth / GOLDEN_BLK_W) + 1) * GOLDEN_BLK_W; //ming modify@2006/4/11
         }
         
         if(dwHeight % 32) 
         {
            dwHeight = ((dwHeight / 32) + 1) * 32;
         }

#ifdef DO_COMPARE_FRAME
         if((CurFrame >= START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
         {
    VP8_LOAD_Y_GOLDEN:
            sprintf(_acFileName, "%s/%s/post_%d.y.dat", _acFileDir[1],_acFileDir[_u4CurFileIdx],CurFrame/*_u4CurFrmIdx*/);
            _LoadSaveFile(_acFileName,TRUE, golden_dataY,0);
            
            for(u4Y = 0; u4Y < prDispInfo->u4H_Cmp; u4Y++)
            {
               for(u4X = 0; u4X < prDispInfo->u4W_Cmp;)
               {                      
                  pu1HWAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)hw_output, u4X, u4Y, u4HwWidth, TRUE, HW_BLK_W_LOG2);
                  pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)golden_dataY, u4X, u4Y, u4GoldenWidth/*720*/, TRUE, GOLDEN_BLK_W_LOG2);
                  if((u4X+4<=prDispInfo->u4W_Cmp)&& ((((UINT32)pu1GoldenAddr)&0x3)==0) && ((((UINT32)pu1HWAddr)&0x3)==0))
                  {
                     fgSame= (*((UINT32 *)pu1GoldenAddr) == *((UINT32 *)pu1HWAddr)) ? TRUE : FALSE;
                     u4X+=4;
                  }
                  else
                  {
                     fgSame= (*pu1GoldenAddr== *pu1HWAddr) ? TRUE : FALSE;
                     u4X++;
                  }
                  
                  if(!fgSame)
               {                      
                     if (u4Retry < VP8_RETRY_TIMES)
                  {
                        u4Retry++;
                        goto VP8_LOAD_Y_GOLDEN;
                     }
                     LOG(0, "=== %s\\%s dif in Y(x%d,y%d), golden(@0x%x=0x%x),hw (@0x%x=0x%x)\n",
                          _acFileDir[1],_acFileDir[_u4CurFileIdx], u4X, u4Y, (UINT32)pu1GoldenAddr, *pu1GoldenAddr, 
                          (UINT32)pu1HWAddr, *pu1HWAddr);
                     sprintf(_acFileName, "D:/vp8_test/Dump/%s_%d.Y.dat",_acFileDir[_u4CurFileIdx],CurFrame);
                     _SaveFile(_acFileName,hw_output,u4HwWidth*dwHeight);
                     sprintf(_acFileName, "D:/vp8_test/Dump/goldendump_%s_%d.y.dat",_acFileDir[_u4CurFileIdx],CurFrame);
                     _SaveFile(_acFileName,golden_dataY,u4HwWidth*dwHeight);
                     _VdecCmdVp8DumpReg(1,NULL);
                     x_thread_delay(20000);
                     ASSERT(0);
                     return;
                  }

                  
                }
             }
            
             Printf("Cmp Y size %d\n", u4X * u4Y);
           }
#endif
        }
        else 
        {
           UINT32 u4HwWidth, u4GoldenWidth;
           
           dwWidth = prDispInfo->u4W;
           dwHeight = prDispInfo->u4H/2;
           hw_output = prDispInfo->pvCAddr;

           u4HwWidth = dwWidth;
           if(dwWidth % HW_BLK_W) 
           {
              u4HwWidth = ((dwWidth / HW_BLK_W) + 1) * HW_BLK_W; //ming modify@2006/4/11
           }

           u4GoldenWidth = dwWidth;
           if(dwWidth % GOLDEN_BLK_W) 
           {
              u4GoldenWidth = ((dwWidth / GOLDEN_BLK_W) + 1) * GOLDEN_BLK_W; //ming modify@2006/4/11
           }

                    
           if(dwHeight % 16)
           {
             dwHeight = ((dwHeight / 16) + 1) * 16;           
           }
    
#ifdef DO_COMPARE_FRAME
           if(CurFrame >= (START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
           {
    VP8_LOAD_C_GOLDEN:
              sprintf(_acFileName, "%s/%s/post_%d.c.dat", _acFileDir[1],_acFileDir[_u4CurFileIdx],CurFrame/*_u4CurFrmIdx*/);
              _LoadSaveFile(_acFileName, TRUE,golden_dataC,0);
 
              for(u4Y = 0; u4Y < (prDispInfo->u4H_Cmp>>1); u4Y++)
              {
                for(u4X = 0; u4X < prDispInfo->u4W_Cmp;)
                {                        
                  pu1HWAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)hw_output, u4X, u4Y, u4HwWidth, TRUE, TRUE, HW_BLK_W_LOG2);
                  pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)golden_dataC, u4X, u4Y, u4GoldenWidth/*720*/, TRUE, TRUE, GOLDEN_BLK_W_LOG2);
                  if((u4X+4<=prDispInfo->u4W_Cmp)&&((((UINT32)pu1GoldenAddr)&0x3)==0) && ((((UINT32)pu1HWAddr)&0x3)==0))
                  {
                     fgSame= (*((UINT32 *)pu1GoldenAddr) == *((UINT32 *)pu1HWAddr)) ? TRUE : FALSE;
                     u4X+=4;
                  }
                  else
              {
                     fgSame= (*pu1GoldenAddr== *pu1HWAddr) ? TRUE : FALSE;
                     u4X++;
                  }
                  
                  if(!fgSame)
                {                        
                      if (u4Retry < VP8_RETRY_TIMES)
                  {
                         u4Retry++;
                         goto VP8_LOAD_C_GOLDEN;
                      }

                      LOG(0, "=== %s\\%s dif in C(x%d,y%d), golden(@0x%x=0x%x), hw(@0x%x=0x%x)\n",
                      _acFileDir[1],_acFileDir[_u4CurFileIdx], u4X, u4Y, (UINT32)pu1GoldenAddr, *pu1GoldenAddr, 
                      (UINT32)pu1HWAddr, *pu1HWAddr);
                      sprintf(_acFileName, "D:/vp8_test/Dump/%s_%d.c.dat",_acFileDir[_u4CurFileIdx],CurFrame);
                      _SaveFile(_acFileName,hw_output,u4HwWidth*dwHeight);
                      sprintf(_acFileName, "D:/vp8_test/Dump/goldendump_%s_%d.c.dat",_acFileDir[_u4CurFileIdx],CurFrame);
                      _SaveFile(_acFileName,golden_dataC,u4HwWidth*dwHeight);
                      _VdecCmdVp8DumpReg(1,NULL);
                      x_thread_delay(20000);
                      ASSERT(0);
                      return;                
                  }
                }
              }
              Printf("Cmp CbCr size %d\n", u4X * u4Y);
             }
#endif
          }
        
       }
       //_u4CurFrmIdx++;
       if(u4DumpIndex==CurFrame)
       {
           _VdecCmdVp8DumpReg(1,NULL);
           x_thread_delay(20000);
       }
   }
  
   if(_uGenCrcEn==2)
   {
       UINT32 *u4CrcWptr=(UINT32 *)(_pcChkSum+32*_u4CrcIndex);
       *u4CrcWptr++=prDispInfo->u4Ycrc[0];
       *u4CrcWptr++=prDispInfo->u4Ycrc[1];
       *u4CrcWptr++=prDispInfo->u4Ycrc[2];
       *u4CrcWptr++=prDispInfo->u4Ycrc[3];
       *u4CrcWptr++=prDispInfo->u4Ccrc[0];
       *u4CrcWptr++=prDispInfo->u4Ccrc[1];
       *u4CrcWptr++=prDispInfo->u4Ccrc[2];
       *u4CrcWptr++=prDispInfo->u4Ccrc[3];
       _u4CrcIndex++;
   }
   return;
}
#endif // CC_VP8_EMULATION

typedef enum
{
    VP6_DISP_IDX = 0,
    VP6_PARAM_BLK_TYPE = 0x100,
    VP6_PARAM_DBK_SUPPORT,
    VP6_SET_DEC_MODE,
}   VP6_PARAM_T;

static void _VdecVP6SaveFrame(VDEC_DISP_PIC_INFO_T* prDispInfo, UINT32 u4FrmNum, UINT32 BlkW, UINT32 u4Dbk, BOOL fgAlpha)
{
    UINT32 dwWidth, dwHeight;
    
    dwWidth = prDispInfo->u4W;
    dwHeight = prDispInfo->u4H;
    
    if(dwWidth % BlkW) 
    {
        dwWidth = ((dwWidth / BlkW) + 1) * BlkW;
    }
    
    if(dwHeight % 32) 
    {
        dwHeight = ((dwHeight / 32) + 1) * 32;
    }
    
    if (fgAlpha)
    {
        sprintf(_acFileName, "d:/vp6_data/%s_mc_out_%d_A.out", _acFileDir[2], u4FrmNum);
        printf("SaveAFrame %s 0x%x 0x%x\n", _acFileName, prDispInfo->pvYAddr, dwWidth * dwHeight);
        _SaveFile(_acFileName, (UINT8*)prDispInfo->pvYAddr, dwWidth * dwHeight);
        if (u4Dbk)
        {
            sprintf(_acFileName, "d:/vp6_data/%s_dbk_out_%d_A.out", _acFileDir[2], u4FrmNum);
            _SaveFile(_acFileName, (UINT8*)prDispInfo->pvYDbkAddr, dwWidth * dwHeight);
        }
    }
    else
    {
    sprintf(_acFileName, "d:/vp6_data/%s_mc_out_%d_Y.out", _acFileDir[2], u4FrmNum);
    printf("SaveYFrame %s 0x%x 0x%x\n", _acFileName, prDispInfo->pvYAddr, dwWidth * dwHeight);
    _SaveFile(_acFileName, (UINT8*)prDispInfo->pvYAddr, dwWidth * dwHeight);
    if (u4Dbk)
    {
        sprintf(_acFileName, "d:/vp6_data/%s_dbk_out_%d_Y.out", _acFileDir[2], u4FrmNum);
        _SaveFile(_acFileName, (UINT8*)prDispInfo->pvYDbkAddr, dwWidth * dwHeight);
    }
    
    // Save Frame Buffer CbCr
    dwHeight = prDispInfo->u4H/2;
    
    if(dwHeight % 16)
    {
        dwHeight = ((dwHeight / 16) + 1) * 16;           
    }
    
    sprintf(_acFileName, "d:/vp6_data/%s_mc_out_%d_CbCr.out", _acFileDir[2], u4FrmNum);
    printf("SaveCFrame %s 0x%x 0x%x\n", _acFileName, prDispInfo->pvCAddr, dwWidth * dwHeight);
    _SaveFile(_acFileName, (UINT8*)prDispInfo->pvCAddr, dwWidth * dwHeight);
    
    if (u4Dbk)
    {
        sprintf(_acFileName, "d:/vp6_data/%s_dbk_out_%d_CbCr.out", _acFileDir[2], u4FrmNum);
        _SaveFile(_acFileName, (UINT8*)prDispInfo->pvCDbkAddr, dwWidth * dwHeight);
    }
}


}

static void _VdecVP6DumpReg(UINT32 u4FrmNum)
{

#ifdef	CC_VP8_EMULATION

	//extern void _VDEC_VP6DumpChksumReg(void);
    
    sprintf(_acFileName, "d:/vp6_data/%s_vp6_vld(33~74)_%d.bin", _acFileDir[2], u4FrmNum);
    _SaveFile(_acFileName, (UINT8*)0xF000B084, 0xA8);

    sprintf(_acFileName, "d:/vp6_data/%s_mc(0~511)_%d.bin", _acFileDir[2], u4FrmNum);
    _SaveFile(_acFileName, (UINT8*)0xF000A000, 0x800);
    
    //_VDEC_VP6DumpChksumReg();
#endif
}

static void _VP6_IntrudeHandler(UINT32 u4Region, MID_AGENT_ID_T eAgentId, UINT32 u4Addr)
{
    // Handling the intrusion event
    Printf("System halted\n");
    BSP_HaltSystem();
}

static void _VdecVP6Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    #define RETRY_TIMES 3
    UINT32 u4Retry;
    UINT32 dwWidth, dwHeight;
    UINT32 u4HwWidth, u4GoldenWidth, u4MinWidth;
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    UINT8* golden_dataY = _pu1GoldenData;
    UINT8* golden_dataC = _pu1GoldenData;
    UINT8* hw_output;
    UINT32 i=0;
    UINT32 u4X, u4Y, u4Z;
    UINT8* pu1GoldenAddr;
    UINT8* pu1HWAddr;
    INT8 fgY;
    UINT32 CurFrame;

    UINT32 VP6_HW_BLK_W_LOG2        = 6;   // 2^6=32
    UINT32 VP6_GOLDEN_BLK_W_LOG2    = 4;   // 2^4=16
    UINT32 VP6_HW_BLK_W;
    UINT32 VP6_GOLDEN_BLK_W;
    UINT32 VP6_MIN_W;
    UINT32 u4DblkSupport            = FALSE;
    UINT32 u4Tmp;

    u4Retry = 0;
    VDEC_GetParam(ES0, (UINT32)VP6_PARAM_BLK_TYPE, &VP6_HW_BLK_W_LOG2, &u4Tmp, &u4Tmp);
    VDEC_GetParam(ES0, (UINT32)VP6_PARAM_DBK_SUPPORT, &u4DblkSupport, &u4Tmp, &u4Tmp);
    VP6_HW_BLK_W             = ((1ul)<<VP6_HW_BLK_W_LOG2);
	VP6_GOLDEN_BLK_W         = ((1ul)<<VP6_GOLDEN_BLK_W_LOG2);
	VP6_MIN_W                = ((VP6_HW_BLK_W<VP6_GOLDEN_BLK_W) ? VP6_HW_BLK_W : VP6_GOLDEN_BLK_W);
	
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)(u4Arg2);
    CurFrame = _u4CurFrmIdx++;

    if (prDispInfo->fgAlphaFrame)
    {
        CurFrame--;
        _u4CurFrmIdx--;
    }

    if (CurFrame < _u4StartFrame)
    {
        printf("CurFrame %d\n", CurFrame);
        return;
    }

    if(u4Arg1 == VDEC_EVT_DEC_BS_END) 
    {
        if((!_fgEnableCRC)&&_uGenCrcEn)
        {
            printf("//d.save.binary %s\\%s_crc.bin,frame:%d/ny\n",_acFileDir[1],_acFileDir[2],_u4CrcIndex);
            HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum,CHKSUM_INFO_SIZE);
            UNUSED(T32_HostExec(1,"d.save.binary \"%s/%s_crc.bin\" 0x%08x--0x%08x",_acFileDir[1],_acFileDir[2],_pcChkSum,(_pcChkSum+(32*_u4CrcIndex)-1)));
        }
        else if(_fgEnableCRC)
        {
            if(_fgAbtCurItem)
            {
                printf("Crc Check Fail.\n");
            }
            else
            {
                printf("Crc Check Pass.\n");
            }
        }

#ifdef CC_SUPPORT_LOG_INFO
        {
            if (_u4VP6DecodeMode)
            {
                sprintf(_acFileName, "d:/vp6_data/adobe_%s.log", _acFileDir[2]);
            }
            else
            {
                sprintf(_acFileName, "d:/vp6_data/open_%s.log", _acFileDir[2]);
            }
            
            _SaveFile(_acFileName, _u4LogBufSa, _u4LogSz);

            _u4LogSz = 0;
            x_memset((void*)_u4LogBufSa, 0, _u4LogBufSz);
        }
#endif

        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_VP6);
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
    }
    else if(u4Arg1 == VDEC_EVT_DISP_READY) 
    {
        if (_bSaveOutput)
        {
            _VdecVP6SaveFrame(prDispInfo, CurFrame, VP6_HW_BLK_W, (u4DblkSupport && _u4WithDbk), prDispInfo->fgAlphaFrame);
        }

        if(_fgEnableCRC)
        {
            UINT32 *u4CrcRptr=(UINT32 *)(_pcChkSum);
            BOOL fgFail=FALSE;
            UINT32 u4CrcRetry = 0;
CRC_RETRY:

           x_memset(_acChkSumFile,0,MAX_FILENAME_LEN);
            if (prDispInfo->fgAlphaFrame)
            {
                Printf("VP6 loading CRC file: %s\\%s\\alpha\\mc_out_%d_CRC.out\n",_acFileDir[1],_acFileDir[2],CurFrame);
                sprintf(_acChkSumFile, "%s/%s/alpha/mc_out_%d_CRC.out", _acFileDir[1],_acFileDir[2],CurFrame);
            }
            else
            {
                Printf("VP6 loading CRC file: %s\\%s\\mc_out_%d_CRC.out\n",_acFileDir[1],_acFileDir[2],CurFrame);                
                sprintf(_acChkSumFile, "%s/%s/mc_out_%d_CRC.out", _acFileDir[1],_acFileDir[2],CurFrame);
            }
           _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);

            if(
            (prDispInfo->u4Ycrc[0]==u4CrcRptr[0] && prDispInfo->u4Ycrc[1]==u4CrcRptr[1]
            &&prDispInfo->u4Ycrc[2]==u4CrcRptr[2] && prDispInfo->u4Ycrc[3]==u4CrcRptr[3]) &&
            (prDispInfo->fgAlphaFrame ||
            (prDispInfo->u4Ccrc[0]==u4CrcRptr[4]&&prDispInfo->u4Ccrc[1]==u4CrcRptr[5]
            &&prDispInfo->u4Ccrc[2]==u4CrcRptr[6]&&prDispInfo->u4Ccrc[3]==u4CrcRptr[7])))
            {
            //printf("Compare frame %d in CRC ok!!\n",CurFrame);
            }
            else
            {
                if (u4CrcRetry < RETRY_TIMES)
                {
                    u4CrcRetry++;
                    goto CRC_RETRY;
                }
                printf("E*E*E*E:%s\\%s CRC check Fail in frame (0,%d) ===\n",_acFileDir[0],_acFileDir[2],CurFrame);
                printf("GoldenCrc:\n0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[0],u4CrcRptr[1],u4CrcRptr[2],u4CrcRptr[3]);
                printf("0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[4],u4CrcRptr[5],u4CrcRptr[6],u4CrcRptr[7]);
                printf("FrameCrc:\n0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ycrc[0],prDispInfo->u4Ycrc[1],prDispInfo->u4Ycrc[2],prDispInfo->u4Ycrc[3]);
                printf("0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ccrc[0],prDispInfo->u4Ccrc[1],prDispInfo->u4Ccrc[2],prDispInfo->u4Ccrc[3]);
                fgFail=TRUE;
                _VdecVP6DumpReg(CurFrame);
                if (!_bSaveOutput)
                {
                    _VdecVP6SaveFrame(prDispInfo, CurFrame, VP6_HW_BLK_W, FALSE, prDispInfo->fgAlphaFrame);
                }
                ASSERT(0);
            }

            if(fgFail)
            {
                _fgAbtCurItem=TRUE;
            }
            return ;
        }
        else if(_uGenCrcEn)
        {
            UINT32 *u4CrcWptr=(UINT32 *)(_pcChkSum+32*_u4CrcIndex);
            *u4CrcWptr++=prDispInfo->u4Ycrc[0];
            *u4CrcWptr++=prDispInfo->u4Ycrc[1];
            *u4CrcWptr++=prDispInfo->u4Ycrc[2];
            *u4CrcWptr++=prDispInfo->u4Ycrc[3];
            *u4CrcWptr++=prDispInfo->u4Ccrc[0];
            *u4CrcWptr++=prDispInfo->u4Ccrc[1];
            *u4CrcWptr++=prDispInfo->u4Ccrc[2];
            *u4CrcWptr++=prDispInfo->u4Ccrc[3];
            _u4CrcIndex++;
            return;
        }

        for(fgY =1 ; fgY >=0 ; fgY--) 
        {
            if(fgY) 
            {
                dwWidth = prDispInfo->u4W;
                dwHeight = prDispInfo->u4H;
                hw_output = prDispInfo->pvYAddr;

                if(dwWidth % VP6_HW_BLK_W) 
                {
                    u4HwWidth = ((dwWidth / VP6_HW_BLK_W) + 1) * VP6_HW_BLK_W;
                }
                else
                {
                    u4HwWidth = dwWidth;
                }

                if(dwWidth % VP6_GOLDEN_BLK_W) 
                {
                    u4GoldenWidth = ((dwWidth / VP6_GOLDEN_BLK_W) + 1) * VP6_GOLDEN_BLK_W;
                }
                else
                {
                    u4GoldenWidth = dwWidth;
                }

                if(dwHeight % 32) 
                {
                    dwHeight = ((dwHeight / 32) + 1) * 32;
                }

#ifdef DO_COMPARE_FRAME
                u4MinWidth = VP6_MIN_W;

                if((CurFrame >= START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
                {
    LOAD_Y_GOLDEN:
                    if (prDispInfo->fgAlphaFrame)
                    {
                        sprintf(_acFileName, "%s/%s/alpha/mc_out_%d_Y.out", _acFileDir[1],_acFileDir[2],CurFrame);
                    }
                    else
                    {
                        sprintf(_acFileName, "%s/%s/mc_out_%d_Y.out", _acFileDir[1],_acFileDir[2],CurFrame);
                    }
#ifdef VP6_MID_GOLDEN
                    MID_RegisterHandler(_VP6_IntrudeHandler);               
                    MID_ResetRegion(2);
                    MID_SetRegionEx2(2, MID_AGENT_CPU, golden_dataY, golden_dataY + GOLDEN_SIZE, 0, 0);
                    MID_EnableRegionProtect(2);
#endif
                    _LoadFile(_acFileName, golden_dataY);

#ifdef VP6_MID_GOLDEN
                    MID_ResetRegion(2);
                    MID_SetRegionEx2(2, MID_AGENT_NONE, golden_dataY, golden_dataY + GOLDEN_SIZE, 0, 0);
                    MID_EnableRegionProtect(2);
#endif
                    for(u4Y = 0; u4Y < prDispInfo->u4H_Cmp; u4Y++)
                    {
                        for(u4X = 0; u4X < prDispInfo->u4W_Cmp; u4X += u4MinWidth)
                        {
                            pu1HWAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)hw_output, u4X, u4Y, u4HwWidth, TRUE, VP6_HW_BLK_W_LOG2);
                            pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)golden_dataY, u4X, u4Y, u4GoldenWidth, TRUE, VP6_GOLDEN_BLK_W_LOG2);
                    
                            for (u4Z = 0; u4Z < u4MinWidth; u4Z += 4, pu1HWAddr += 4, pu1GoldenAddr += 4)
                            {
                                UINT32 u4GoldData, u4HwData;
                                u4HwData = *((UINT32*)(pu1HWAddr));
                                u4GoldData = *((UINT32*)(pu1GoldenAddr));
                                if(u4GoldData != u4HwData)
                                {
                                    if (u4Retry < RETRY_TIMES)
                                    {
                                        u4Retry++;
                                        goto LOAD_Y_GOLDEN;
                                    }
                                    
                                    printf("=== %s\\%s dif in Y(x%d,y%d), golden(@0x%x=0x%x),hw (@0x%x=0x%x)\n",
                                    _acFileDir[1],_acFileDir[2], u4X + u4Z, u4Y, (UINT32)pu1GoldenAddr, u4GoldData, 
                                    (UINT32)pu1HWAddr, u4HwData);
                                    _VdecVP6DumpReg(CurFrame);
                                    if (!_bSaveOutput)
                                    {
                                        _VdecVP6SaveFrame(prDispInfo, CurFrame, VP6_HW_BLK_W, FALSE, prDispInfo->fgAlphaFrame);
                                    }
                                    ASSERT(0);
                                    return;
                                }
                            }
                        }
                    }
                    printf("Cmp Y size %d\n", u4X * u4Y);
                }

                if (u4DblkSupport && _u4WithDbk)
                {
                    if((CurFrame >= START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
                    {
                        sprintf(_acFileName, "%s/%s_dbk/dbk_out_%d_Y.out", _acFileDir[1],_acFileDir[2],CurFrame);
                        _LoadFile(_acFileName, golden_dataY);

                        for(u4Y = 0; u4Y < prDispInfo->u4H_Cmp; u4Y++)
                        {
                            for(u4X = 0; u4X < prDispInfo->u4W_Cmp; u4X += u4MinWidth)
                            {
                                pu1HWAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)prDispInfo->pvYDbkAddr, u4X, u4Y, u4HwWidth, TRUE, VP6_HW_BLK_W_LOG2);
                                pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)golden_dataY, u4X, u4Y, u4GoldenWidth, TRUE, VP6_GOLDEN_BLK_W_LOG2);

                                for (u4Z = 0; u4Z < u4MinWidth; u4Z += 4, pu1HWAddr += 4, pu1GoldenAddr += 4)
                                {
                                    UINT32 u4GoldData, u4HwData;
                                    u4HwData = *((UINT32*)(pu1HWAddr));
                                    u4GoldData = *((UINT32*)(pu1GoldenAddr));
                                    if(u4GoldData != u4HwData)
                                    {
                                        printf("=== %s\\%s dif in Y(x%d,y%d), golden(@0x%x=0x%x),hw (@0x%x=0x%x)\n",
                                        _acFileDir[1],_acFileDir[2], u4X + u4Z, u4Y, (UINT32)pu1GoldenAddr, u4GoldData, 
                                        (UINT32)pu1HWAddr, u4HwData);
                                        if (!_bSaveOutput)
                                        {
                                            _VdecVP6SaveFrame(prDispInfo, CurFrame, VP6_HW_BLK_W, TRUE, prDispInfo->fgAlphaFrame);
                                        }
                                        ASSERT(0);
                                        return;
                                    }
                                }
                            }
                        }            
                        printf("Cmp Dbk Y size %d\n", u4X * u4Y);
                    }
                }
#endif
                if (prDispInfo->fgAlphaFrame)
                {   // because alpha frame only contains Y, no CbCr
                    break;
                }
            }
            else 
            {
                dwWidth = prDispInfo->u4W;
                dwHeight = prDispInfo->u4H/2;
                hw_output = prDispInfo->pvCAddr;

                if(dwWidth % VP6_HW_BLK_W)
                {
                    u4HwWidth = ((dwWidth / VP6_HW_BLK_W) + 1) * VP6_HW_BLK_W;
                }
                else
                {
                    u4HwWidth = dwWidth;
                }

                if(dwWidth % VP6_GOLDEN_BLK_W) 
                {
                    u4GoldenWidth = ((dwWidth / VP6_GOLDEN_BLK_W) + 1) * VP6_GOLDEN_BLK_W;
                }
                else
                {
                    u4GoldenWidth = dwWidth;
                }

                if(dwHeight % 16)
                {
                    dwHeight = ((dwHeight / 16) + 1) * 16;           
                }

#ifdef DO_COMPARE_FRAME
    #ifdef SAVE_RAW_DATA

    #endif
                u4MinWidth = VP6_MIN_W;

                if(CurFrame >= (START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
                {
    LOAD_C_GOLDEN:
#ifdef VP6_MID_GOLDEN
                    MID_RegisterHandler(_VP6_IntrudeHandler);               
                    MID_ResetRegion(2);
                    MID_SetRegionEx2(2, MID_AGENT_CPU, golden_dataC, golden_dataC + GOLDEN_SIZE, 0, 0);
                    MID_EnableRegionProtect(2);
#endif
                    sprintf(_acFileName, "%s/%s/mc_out_%d_CbCr.out", _acFileDir[1],_acFileDir[2],CurFrame);
                    _LoadFile(_acFileName, golden_dataC);

#ifdef VP6_MID_GOLDEN
                    MID_ResetRegion(2);
                    MID_SetRegionEx2(2, MID_AGENT_NONE, golden_dataC, golden_dataC + GOLDEN_SIZE, 0, 0);
                    MID_EnableRegionProtect(2);
#endif
                    for(u4Y = 0; u4Y < (prDispInfo->u4H_Cmp>>1); u4Y++)
                    {
                        for(u4X = 0; u4X < prDispInfo->u4W_Cmp; u4X += u4MinWidth)
                        {
                            pu1HWAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)hw_output, u4X, u4Y, u4HwWidth, TRUE, TRUE, VP6_HW_BLK_W_LOG2);
                            pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)golden_dataC, u4X, u4Y, u4GoldenWidth, TRUE, TRUE, VP6_GOLDEN_BLK_W_LOG2);

                            for (u4Z = 0; u4Z < u4MinWidth; u4Z += 4, pu1HWAddr += 4, pu1GoldenAddr += 4)
                            {
                                UINT32 u4GoldData, u4HwData;
                                u4HwData = *((UINT32*)(pu1HWAddr));
                                u4GoldData = *((UINT32*)(pu1GoldenAddr));
                                if(u4GoldData != u4HwData)
                                {
                                    if (u4Retry < RETRY_TIMES)
                                    {
                                        u4Retry++;
                                        goto LOAD_C_GOLDEN;
                                    }

                                    printf("=== %s\\%s dif in C(x%d,y%d), golden(@0x%x=0x%x),hw (@0x%x=0x%x)\n",
                                    _acFileDir[1],_acFileDir[2], u4X + u4Z, u4Y, (UINT32)pu1GoldenAddr, u4GoldData, 
                                    (UINT32)pu1HWAddr, u4HwData);
                                    _VdecVP6DumpReg(CurFrame);
                                    if (!_bSaveOutput)
                                    {
                                        _VdecVP6SaveFrame(prDispInfo, CurFrame, VP6_HW_BLK_W, FALSE, prDispInfo->fgAlphaFrame);
                                    }
                                    ASSERT(0);
                                    return;
                                }
                            }
                        }
                    }            
                    printf("Cmp CbCr size %d\n", u4X * u4Y);
                }

                if (u4DblkSupport && _u4WithDbk)
                {
                    if(CurFrame >= (START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
                    {
                        sprintf(_acFileName, "%s/%s_dbk/dbk_out_%d_CbCr.out", _acFileDir[1],_acFileDir[2],CurFrame);
                        _LoadFile(_acFileName, golden_dataC);

                        for(u4Y = 0; u4Y < (prDispInfo->u4H_Cmp>>1); u4Y++)
                        {
                            for(u4X = 0; u4X < prDispInfo->u4W_Cmp; u4X += u4MinWidth)
                            {
                                pu1HWAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)prDispInfo->pvCDbkAddr, u4X, u4Y, u4HwWidth, TRUE, TRUE, VP6_HW_BLK_W_LOG2);
                                pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)golden_dataC, u4X, u4Y, u4GoldenWidth, TRUE, TRUE, VP6_GOLDEN_BLK_W_LOG2);

                                for (u4Z = 0; u4Z < u4MinWidth; u4Z += 4, pu1HWAddr += 4, pu1GoldenAddr += 4)
                                {
                                    UINT32 u4GoldData, u4HwData;
                                    u4HwData = *((UINT32*)(pu1HWAddr));
                                    u4GoldData = *((UINT32*)(pu1GoldenAddr));
                                    if(u4GoldData != u4HwData)
                                    {
                                        printf("=== %s\\%s dif in C(x%d,y%d), golden(@0x%x=0x%x),hw (@0x%x=0x%x)\n",
                                        _acFileDir[1],_acFileDir[2], u4X + u4Z, u4Y, (UINT32)pu1GoldenAddr, u4GoldData, 
                                        (UINT32)pu1HWAddr, u4HwData);
                                        if (!_bSaveOutput)
                                        {
                                            _VdecVP6SaveFrame(prDispInfo, CurFrame, VP6_HW_BLK_W, TRUE, prDispInfo->fgAlphaFrame);
                                        }
                                        ASSERT(0);
                                        return;
                                    }
                                }
                            }
                        }            
                        printf("Cmp Dbk CbCr size %d\n", u4X * u4Y);
                    }
                }
#endif
            }

        }
    }

    return;
}

void _VdecWMVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UINT32 dwWidth, dwHeight;
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    UINT8* golden_dataY = _pu1GoldenData;
    UINT8* golden_dataC = _pu1GoldenData;
    UINT8* hw_output;
    UINT32 i = 0;
    UINT32 u4X, u4Y;
    UINT8* pu1GoldenAddr;
    UINT8* pu1HWAddr;
    INT8 fgY;
    UINT32 CurFrame;
    UINT32 u4CS1 = 0;
    UINT32 u4CS2 = 0;

    prDispInfo = (VDEC_DISP_PIC_INFO_T*)(u4Arg2);
    //LOG(0,"entering my_save_pic\n");
    CurFrame = prDispInfo->u4CurrentPicNo - 1;
    //LOG(0,"frame idx : %d %d\n",CurFrame,_u4CurFrmIdx);

    //Printf("src decode resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthDec,rWMVDecInfo.rSeqHdrR.u4PicHeightDec);
    //Printf("src cmp resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthCmp,rWMVDecInfo.rSeqHdrR.u4PicHeightCmp);
    //Printf("src resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthSrc,rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);
    //Printf("rWMVDecInfo.rOther.rPicInfo src resolution : %dx%d\n",rWMVDecInfo.rOther.rPicInfo.dwW,rWMVDecInfo.rOther.rPicInfo.dwH);

    if (u4Arg1 == VDEC_EVT_DEC_BS_END)
    {
#ifdef CC_WMV_EMULATION
        if((!_fgEnableCRC)&&_uGenCrcEn)
        {
             printf("//d.save.binary %s\\%s_crc.bin,frame:%d/ny\n",golden_path[_u4CurFileIdx],_acGoldPrefix[_u4CurFileIdx],_u4CrcIndex);
             HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum,CHKSUM_INFO_SIZE);
             UNUSED(T32_HostExec(1,"d.save.binary \"%s/%s_crc.bin\" 0x%08x--0x%08x",golden_path[_u4CurFileIdx],_acGoldPrefix[_u4CurFileIdx],_pcChkSum,(_pcChkSum+(32*_u4CrcIndex)-1)));
        }
        else if(_fgEnableCRC)
        {
           if(_fgAbtCurItem)
           {
               printf("Crc Check Fail.\n");
           }
           else
           {
               printf("Crc Check Pass.\n");
           }
        }
#endif
        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_WMV);
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
    }

    else if (u4Arg1 == VDEC_EVT_DISP_READY)
    {
#ifdef CC_WMV_EMULATION
       // if(prDispInfo->u4FrameAddr<prDispInfo->pvCAddr)
        //{
        //    ASSERT(0);
        //}
            
        if(_fgEnableCRC)
        {
           UINT32 *u4CrcRptr=(UINT32 *)(_pcChkSum+32*_u4CrcIndex);
           BOOL fgFail=FALSE;
           
           if(prDispInfo->u4Ycrc[0]==u4CrcRptr[0] && prDispInfo->u4Ycrc[1]==u4CrcRptr[1]
                 &&prDispInfo->u4Ycrc[2]==u4CrcRptr[2] && prDispInfo->u4Ycrc[3]==u4CrcRptr[3]
                 &&prDispInfo->u4Ccrc[0]==u4CrcRptr[4]&&prDispInfo->u4Ccrc[1]==u4CrcRptr[5]
                 &&prDispInfo->u4Ccrc[2]==u4CrcRptr[6]&&prDispInfo->u4Ccrc[3]==u4CrcRptr[7])
           {
                //printf("Compare frame %d in CRC ok!!\n",CurFrame);
           }
           else
           {
               printf("E*E*E*E:%s CRC check Fail in frame (0,%d,%d) ===\n",_acFileDir[_u4CurFileIdx],CurFrame,_u4CrcIndex);
               printf("GoldenCrc:\n0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[0],u4CrcRptr[1],u4CrcRptr[2],u4CrcRptr[3]);
               printf("0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[4],u4CrcRptr[5],u4CrcRptr[6],u4CrcRptr[7]);
               printf("FrameCrc:\n0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ycrc[0],prDispInfo->u4Ycrc[1],prDispInfo->u4Ycrc[2],prDispInfo->u4Ycrc[3]);
               printf("0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ccrc[0],prDispInfo->u4Ccrc[1],prDispInfo->u4Ccrc[2],prDispInfo->u4Ccrc[3]);
               fgFail=TRUE;
           }
           
           _u4CrcIndex++;
           
           if(prDispInfo->u1PicStruct==2)
           {
               u4CrcRptr=(UINT32 *)(_pcChkSum+32*_u4CrcIndex);
               if(prDispInfo->u4Ycrc[4]==u4CrcRptr[0] && prDispInfo->u4Ycrc[5]==u4CrcRptr[1]
                  &&prDispInfo->u4Ycrc[6]==u4CrcRptr[2] && prDispInfo->u4Ycrc[7]==u4CrcRptr[3]
                  &&prDispInfo->u4Ccrc[4]==u4CrcRptr[4]&&prDispInfo->u4Ccrc[5]==u4CrcRptr[5]
                  &&prDispInfo->u4Ccrc[6]==u4CrcRptr[6]&&prDispInfo->u4Ccrc[7]==u4CrcRptr[7])
               {
                    //printf("Compare frame %d in CRC ok!!\n",CurFrame);
               }
               else
               {
                   printf("E*E*E*E:%s CRC check Fail in crc (1,%d,%d) ===\n",_acFileDir[_u4CurFileIdx],CurFrame,_u4CrcIndex);
                   printf("GoldenCrc:\n0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[0],u4CrcRptr[1],u4CrcRptr[2],u4CrcRptr[3]);
                   printf("0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[4],u4CrcRptr[5],u4CrcRptr[6],u4CrcRptr[7]);
                   printf("FrameCrc:\n0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ycrc[4],prDispInfo->u4Ycrc[5],prDispInfo->u4Ycrc[6],prDispInfo->u4Ycrc[7]);
                   printf("0x%x,0x%x,0x%x,0x%x\n",prDispInfo->u4Ccrc[4],prDispInfo->u4Ccrc[5],prDispInfo->u4Ccrc[6],prDispInfo->u4Ccrc[7]);
                   fgFail=TRUE;
               }
               _u4CrcIndex++;
           }

           if(fgFail)
           {
              print_checksum(0, CurFrame);
              Dump_Mem(prDispInfo);
              vDumpWmvMcReg(0);
              u4DumpWmvReg(NULL);
              _auNGFile[_u4CurFileIdx]=WMV_FILE_NG;
              _fgAbtCurItem=TRUE;
           }
           else if(_fgDownLoadResult&&_u4CurFileIdx==_u4DownLoadFileId&&_u4DownLoadFrameId==CurFrame)
           {
               Dump_Mem(prDispInfo);
               vDumpWmvMcReg(0);
               u4DumpWmvReg(NULL);
           }
           _u4CurFrmIdx++;
           return ;
           
        }
        else if(_uGenCrcEn)
        {
            UINT32 *u4CrcWptr=(UINT32 *)(_pcChkSum+32*_u4CrcIndex);
            *u4CrcWptr++=prDispInfo->u4Ycrc[0];
            *u4CrcWptr++=prDispInfo->u4Ycrc[1];
            *u4CrcWptr++=prDispInfo->u4Ycrc[2];
            *u4CrcWptr++=prDispInfo->u4Ycrc[3];
            *u4CrcWptr++=prDispInfo->u4Ccrc[0];
            *u4CrcWptr++=prDispInfo->u4Ccrc[1];
            *u4CrcWptr++=prDispInfo->u4Ccrc[2];
            *u4CrcWptr++=prDispInfo->u4Ccrc[3];
            _u4CrcIndex++;
            if(prDispInfo->u1PicStruct==2)
            {
               u4CrcWptr=(UINT32 *)(_pcChkSum+32*_u4CrcIndex);
               *u4CrcWptr++=prDispInfo->u4Ycrc[4];
               *u4CrcWptr++=prDispInfo->u4Ycrc[5];
               *u4CrcWptr++=prDispInfo->u4Ycrc[6];
               *u4CrcWptr++=prDispInfo->u4Ycrc[7];
               *u4CrcWptr++=prDispInfo->u4Ccrc[4];
               *u4CrcWptr++=prDispInfo->u4Ccrc[5];
               *u4CrcWptr++=prDispInfo->u4Ccrc[6];
               *u4CrcWptr++=prDispInfo->u4Ccrc[7];
               _u4CrcIndex++;
            }
            _u4CurFrmIdx++;
            return;
        }
#endif
        for (fgY = 1 ; fgY >= 0 ; fgY--)
        {
            if (fgY)
            {

                dwWidth = prDispInfo->u4W;
                dwHeight = prDispInfo->u4H;
                hw_output = prDispInfo->pvYAddr;

                if (dwWidth % 16)
                {
                    dwWidth = ((dwWidth / 16) + 1) * 16; //ming modify@2006/4/11
                }
                if (dwHeight % 32)
                {
                    dwHeight = ((dwHeight / 32) + 1) * 32;
                }

                //dwSz = dwWidth * dwHeight;
                //LOG(0,"hw y address : %x\n",hw_output);
                //LOG(0,"Y %d size :%d, %dx%d\n", CurFrame/*_u4CurFrmIdx*/, prDispInfo->u4YSize, prDispInfo->u4W, prDispInfo->u4H);
#ifdef DO_COMPARE_FRAME
                if ((CurFrame >= START_COMPARE_FRAME) && (CurFrame <= END_COMPARE_FRAME))
                {
                    if (!_fgEnChkSum)
                    {
                        sprintf(_acFileName, "%s/%s_%d_Y.out", golden_path[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], CurFrame/*_u4CurFrmIdx*/);
                        _LoadFile(_acFileName, golden_dataY);
                    }
                    else
                    {
                        u4CS1 = 0;
                        u4CS2 = 0;
                        //printf("compare frame %d Y in checksum\n",CurFrame);
                    }
                    for (u4Y = 0; u4Y < prDispInfo->u4H_Cmp; u4Y++)
                    {
                        for (u4X = 0; u4X < prDispInfo->u4W_Cmp; u4X++)
                        {
                            pu1HWAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)hw_output, u4X, u4Y, dwWidth, TRUE, 4);
                            if (_fgEnChkSum)
                            {
                                u4CS1 += (*pu1HWAddr);
                                u4CS2 += 2 * (*pu1HWAddr);
                            }
                            else
                            {
                                pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)golden_dataY, u4X, u4Y, dwWidth/*720*/, TRUE, 4);
                                if (*pu1GoldenAddr != *pu1HWAddr)
                                {
                                    printf("=== %s dif in Y(x%d,y%d), golden(@0x%x=0x%x),hw (@0x%x=0x%x)\n",
                                           _acGoldPrefix[_u4CurFileIdx], u4X, u4Y, (UINT32)pu1GoldenAddr, *pu1GoldenAddr,
                                           (UINT32)pu1HWAddr, *pu1HWAddr);
                                    print_checksum(0, CurFrame);
                                    Dump_Mem(prDispInfo);
                                    _u4CurFrmIdx++;
                                    return FALSE;
                                }
                            }
                        }
                    }
                    if (_fgEnChkSum)
                    {
                        //compare with checksum
                        if (u4CS1 != _rCSInfo.u4ShftAddY1[CurFrame] || u4CS2 != _rCSInfo.u4ShftAddY2[CurFrame])
                        {
                            printf("=== %s checksum dif in frame %d Y\n", _acGoldPrefix[_u4CurFileIdx], CurFrame);
                            ASSERT(0);
                            return FALSE;
                        }
                        else
                        {
                            printf("compare frame %d Y in checksum ok!!\n", CurFrame);
                        }
                    }
                    printf("Cmp Y size %d\n", u4X * u4Y);
                }
#endif
            }

            else
            {

                dwWidth = prDispInfo->u4W;
                dwHeight = prDispInfo->u4H / 2;
                hw_output = prDispInfo->pvCAddr;
                if (dwWidth % 16)
                {
                    dwWidth = ((dwWidth / 16) + 1) * 16; //ming modify@2006/4/11
                }

                if (dwHeight % 16)
                {
                    dwHeight = ((dwHeight / 16) + 1) * 16;
                }

                //printf("hw Cbcr address : %x\n",hw_output);
                //printf("CbCr %d size :%d\n", CurFrame/*_u4CurFrmIdx*/, prDispInfo->u4CSize);
#ifdef DO_COMPARE_FRAME
                if (CurFrame >= (START_COMPARE_FRAME) && (CurFrame <= END_COMPARE_FRAME))
                {
                    if (!_fgEnChkSum)
                    {
                        sprintf(_acFileName, "%s/%s_%d_CbCr.out", golden_path[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], CurFrame/*_u4CurFrmIdx*/);
                        _LoadFile(_acFileName, golden_dataC);
                    }
                    else
                    {
                        u4CS1 = 0;
                        u4CS2 = 0;
                    }

                    for (u4Y = 0; u4Y < (prDispInfo->u4H_Cmp >> 1)/*(rWMVDecInfo.rOther.rPicInfo.dwH>>1)*/; u4Y++)
                    {
                        for (u4X = 0; u4X < prDispInfo->u4W_Cmp/*rWMVDecInfo.rOther.rPicInfo.dwW*/; u4X++)
                        {
                            pu1HWAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)hw_output, u4X, u4Y, dwWidth, TRUE, TRUE, 4);
                            if (_fgEnChkSum)
                            {
                                u4CS1 += (*pu1HWAddr);
                                u4CS2 += 2 * (*pu1HWAddr);
                            }
                            else
                            {
                                pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)golden_dataC, u4X, u4Y, dwWidth/*720*/, TRUE, TRUE, 4);
                                if (*pu1GoldenAddr != *pu1HWAddr)
                                {
                                    printf("=== %s dif in C(x%d,y%d), golden(@0x%x=0x%x), hw(@0x%x=0x%x)\n",
                                           _acGoldPrefix[_u4CurFileIdx], u4X, u4Y, (UINT32)pu1GoldenAddr, *pu1GoldenAddr,
                                           (UINT32)pu1HWAddr, *pu1HWAddr);
                                    print_checksum(0, CurFrame);
                                    Dump_Mem(prDispInfo);
                                    _u4CurFrmIdx++;
                                    return FALSE;
                                }
                            }
                        }
                    }
                    if (_fgEnChkSum)
                    {
                        //compare checksum for vc-1 decoder
                        if (u4CS1 != _rCSInfo.u4ShftAddC1[CurFrame] || u4CS2 != _rCSInfo.u4ShftAddC2[CurFrame])
                        {
                            printf("=== %s checksum dif in frame %d CbCr\n", _acGoldPrefix[_u4CurFileIdx], CurFrame);
                            ASSERT(0);
                            return FALSE;
                        }
                        else
                        {
                            printf("compare frame %d CbCr in checsum ok!!\n", CurFrame);
                        }

                    }
                    printf("Cmp CbCr size %d\n", u4X * u4Y);
                }
#endif
            }
        }
        _u4CurFrmIdx++;
    }
    return;
}


#ifdef CC_RV_EMULATION
#ifdef CC_EMULATION_CRC
static BOOL _VdecRvEmuCompareCRC(VDEC_DISP_PIC_INFO_T *prDispInfo);
#endif
static BOOL _fgRvEmuCompGolden;
BOOL _fgRvTracking = 1;
BOOL _fgRvShrinkFbm = 1;
static BOOL fgCompareGold(VDEC_DISP_PIC_INFO_T *prDispInfo);
void _VdecRVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    if (u4Arg1 == VDEC_EVT_DEC_BS_END)
    {
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
    }
    else if (u4Arg1 == VDEC_EVT_DISP_READY)
    {
        VDEC_DISP_PIC_INFO_T *prDispInfo = (VDEC_DISP_PIC_INFO_T *)u4Arg2;
        if (prDispInfo == NULL)
        {
            return;
        }

#ifdef CC_EMULATION_CRC
        VERIFY(_VdecRvEmuCompareCRC(prDispInfo));
#endif

        VERIFY(!_fgRvEmuCompGolden || fgCompareGold(prDispInfo));
    }
}
#endif

#ifdef USB_RW_SUPPORT
#include "x_fm.h"
static BOOL _fgLoadUsbFile(CHAR *pcFileName, UINT32 u4Addr)
{
    UINT64 u8FileSize, u8Tmp;
    UINT32 u4Request, u4Read;
    HANDLE_T  h_file;

    VERIFY(FMR_OK == x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777, FALSE, &h_file));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_END, &u8FileSize));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_BGN, &u8Tmp));

    while (u8FileSize > 0)
    {
        if (u8FileSize >> 32)
        {
            u4Request = 0xffffffff;
        }
        else
        {
            u4Request = (UINT32)u8FileSize;
        }
        x_fm_read(h_file, (VOID *)u4Addr, u4Request, &u4Read);
        ASSERT(u4Request == u4Read);
        u4Addr += u4Read;
        u8FileSize -= u4Read;
    }

    VERIFY(FMR_OK == x_fm_close(h_file));

    return TRUE;
}

static BOOL _fgSaveUsbFile(CHAR *pcFileName, UINT32 u4Addr, UINT32 u4Size)
{
    HANDLE_T    h_file;
    UINT32      u4WriteByte;

    VERIFY(FMR_OK == x_fm_open(FM_ROOT_HANDLE, pcFileName, (FM_OPEN_CREATE|FM_OPEN_TRUNC), 0777, FALSE, &h_file));
    VERIFY(FMR_OK == x_fm_write(h_file, (VOID*)u4Addr, u4Size, &u4WriteByte));
    VERIFY(u4Size == u4WriteByte);
    VERIFY(FMR_OK == x_fm_close(h_file));

    return TRUE;
}
#endif

static BOOL _SaveFile(const char* pcFileName, UINT32 u4Addr, UINT32 u4Size)
{
    HalFlushInvalidateDCache();

    #ifdef  IDE_READ_SUPPORT
    _DmxIdeSendName(pcFileName);
    if(!_DmxIdeGetFile(u4Addr, 0))
    {
        LOG(0, "--File load failed\n");
        return FALSE;
    }
    #else
        #ifdef CC_DRIVER_DEMO    
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s\" 0x%08x++0x%x", pcFileName, u4Addr, u4Size-1));
        #else
        LOG(0, "CC_DRIVER_DEMO !defined, File load fail~");
        #endif
    #endif

    return TRUE;
}

#ifdef CC_VP8_EMULATION
static BOOL _LoadSaveFile(const char* pcFileName, BOOL fgLoad,UINT32 u4Addr,UINT32 u4Size)
{

    HalFlushInvalidateDCache();
    #ifdef  IDE_READ_SUPPORT
    _DmxIdeSendName(pcFileName);
    if(!_DmxIdeGetFile(u4Addr, 0))
    {
        LOG(0, "--File load failed\n");
        return FALSE;
    }
    #elif defined(USB_RW_SUPPORT)
    if(fgLoad)
    {
        Printf("LoadUsbFile [%s]\n", pcFileName);
        _fgLoadUsbFile(pcFileName, u4Addr);
    }
    else
    {
        Printf("d.load.save \"%s\" 0x%08x++0x%x\n", pcFileName, u4Addr,u4Size-1);
        #ifdef CC_DRIVER_DEMO    
          UNUSED(T32_HostExec2(7,pcFileName,u4Addr,u4Size-1));
        #else
          LOG(0, "CC_DRIVER_DEMO !defined, File load fail~");
        #endif
    }
    #else
    if(fgLoad)
    {
        Printf("d.load.binary \"%s\" 0x%08x /ny\n", pcFileName, u4Addr);
        #ifdef CC_DRIVER_DEMO    
          UNUSED(T32_HostExec2(6,pcFileName,u4Addr,0));
        #else
          Printf( "CC_DRIVER_DEMO !defined, File load fail~");
        #endif
    }
    else
    {
        Printf("d.load.save \"%s\" 0x%08x++0x%x\n", pcFileName, u4Addr,u4Size-1);
        #ifdef CC_DRIVER_DEMO    
          UNUSED(T32_HostExec2(7,pcFileName,u4Addr,u4Size-1));
        #else
          LOG(0, "CC_DRIVER_DEMO !defined, File load fail~");
        #endif
    }
    #endif

    HalFlushInvalidateDCache();
    return TRUE;
}
#endif // CC_VP8_EMULATION

static BOOL _LoadFile(const char* pcFileName, UINT32 u4Addr)
{
    HalFlushInvalidateDCache();
    
    #ifdef  IDE_READ_SUPPORT
    _DmxIdeSendName(pcFileName);
    if (!_DmxIdeGetFile(u4Addr, 0))
    {
        LOG(0, "--File load failed\n");
        return FALSE;
    }
    #elif defined(USB_RW_SUPPORT)
    Printf("LoadUsbFile [%s]\n", pcFileName);
    _fgLoadUsbFile(pcFileName, u4Addr);
    #else
    Printf("//d.load.binary \"%s\" 0x%08x /ny\n", pcFileName, u4Addr);
        #ifdef CC_DRIVER_DEMO
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pcFileName, u4Addr));
        #else
    LOG(0, "CC_DRIVER_DEMO !defined, File load fail~");
        #endif
    #endif

    HalFlushInvalidateDCache();
    return TRUE;
}

#ifdef IDE_WRITE_SUPPORT
static BOOL _IDEComp(const char* pcFileName, UINT32 u4Addr, UINT32 u4Len)
{
    _DmxIdeSendName(pcFileName);
    _DmxIdeSendDataLen(u4Len);
    _DmxIdeSendData(u4Addr, u4Len);

    return _DmxIdeGetCmpResult();
}
#endif

static BOOL _VdecVerifyDecData(const VDEC_DISP_PIC_INFO_T* prDispInfo, BOOL bIsYcomp)
{
    UINT32* pu4DecData;
    UINT32* pu4GoldData = _pu1GoldenData;
    UINT32 u4DecSize, u4CmpWord;
    UINT32 u4Residual, u4Pic32XSize;
    UINT32 u4BlkH, u4BlkFrmSize, u4PicH;    //Y: 16x32, CbCr: 16x16
    UINT32 u4CS1 = 0, u4CS2 = 0, u4GoldenCS1 = 0, u4GoldenCS2 = 0;

    u4BlkH = (bIsYcomp) ? 32 : 16;
    u4BlkFrmSize = u4BlkH << 2;        //size in word
    u4PicH = (bIsYcomp) ? prDispInfo->u4H : (prDispInfo->u4H >> 1);

#ifdef CC_64x32BLK_CMP
    //64x32 block mode
    pu4DecData = _pu1DecData16x32;
#else
    //16x32 block mode
    pu4DecData = (UINT32*)((bIsYcomp) ? prDispInfo->pvYAddr : prDispInfo->pvCAddr);
#endif

    u4Residual = u4PicH % u4BlkH; // 16 or 0
    u4Pic32XSize = prDispInfo->u4W * (u4PicH - u4Residual);

    if (bIsYcomp && (_u4YRREn > 0))
    {
        if (_u4YRRRatio > 0)
        {
            u4DecSize = ((prDispInfo->u4W * u4PicH) * 3) >> 2;
            u4Pic32XSize = (u4Pic32XSize * 3) >> 2;
        }
        else
        {
            u4DecSize = (prDispInfo->u4W * u4PicH) >> 1;
            u4Pic32XSize = u4Pic32XSize >> 1;
        }
    }
    else if ((!bIsYcomp) && (_u4CRREn > 0))
    {
        if (_u4CRRRatio > 0)
        {
            u4DecSize = ((prDispInfo->u4W * u4PicH) * 3) >> 2;
            u4Pic32XSize = (u4Pic32XSize * 3) >> 2;
        }
        else
        {
            u4DecSize = (prDispInfo->u4W * u4PicH) >> 1;
            u4Pic32XSize = u4Pic32XSize >> 1;
        }
    }
    else
    {
        u4DecSize = (bIsYcomp) ? prDispInfo->u4YSize : prDispInfo->u4CSize;
    }
    u4DecSize >>= 2;    //size in word
    u4Pic32XSize >>= 2;

    if ((_u4YRREn > 0) || (_u4CRREn > 0))
    {
        for (u4CmpWord = 0; u4CmpWord < u4Pic32XSize; u4CmpWord++)
        {
            //compare data
            if (_fgEnChkSum)
            {
                u4CS1 += pu4DecData[u4CmpWord];
                u4CS2 += pu4DecData[u4CmpWord];
                u4CS1 <<= 1;
                u4CS2 <<= 2;
            }
            else
            {
                if ((pu4GoldData[u4CmpWord] != pu4DecData[u4CmpWord]))
                {
                    LOG(0, "//Error! %c Data Mismatch at [0x%.8x] = 0x%.2x, Golden = 0x%.2x !!! \n",
                        (bIsYcomp) ? 'Y' : 'C', u4CmpWord, pu4DecData[u4CmpWord], pu4GoldData[u4CmpWord]);
#ifndef STRESS_TESTING
                    while (1);
#else
                    return FALSE;
#endif
                }
            }
        }
    }
    else
    {
        for (u4CmpWord = 0; u4CmpWord < u4DecSize; u4CmpWord++)
        {
            if ((prDispInfo->u1PicStruct == BOTTOM_FIELD) && ((u4CmpWord % 4) == 0) && (!((u4CmpWord >> 2) % 2)))
            {
                // Skip Top lines, 16x16 Block mode
                u4CmpWord += 4;    // 4*4 byte
            }
            //compare data
            if (_fgEnChkSum)
            {
                u4CS1 += pu4DecData[u4CmpWord];
                u4CS2 += pu4DecData[u4CmpWord];
                u4CS1 <<= 1;
                u4CS2 <<= 2;
            }
            else
            {
                if ((pu4GoldData[u4CmpWord] != pu4DecData[u4CmpWord]))
                {
                    LOG(0, "//Error! %c Data Mismatch at [0x%.8x] = 0x%.2x, Golden = 0x%.2x !!! \n",
                        (bIsYcomp) ? 'Y' : 'C', u4CmpWord, pu4DecData[u4CmpWord], pu4GoldData[u4CmpWord]);
#ifndef STRESS_TESTING
                    while (1);
#else
                    return FALSE;
#endif
                }
            }
            //ignore partial data of the last row if u4Residual > 0
            if (u4Residual && (u4CmpWord > u4Pic32XSize) &&
                    (((u4CmpWord - u4Pic32XSize) % u4BlkFrmSize) == ((u4Residual << 2) - 1)) &&
                    (prDispInfo->u1PicStruct == FRAME))
            {
                u4CmpWord += ((u4BlkH - u4Residual) << 2);
            }
            /*else if(u4Residual && (u4CmpWord > u4Pic32XSize) &&
                (u4CmpWord % u4BlkFldSize == (u4BlkFrmSize - 1)) &&
                (rDispInfo.u1PicStruct != FRAME))
            {
                u4CmpWord += u4BlkFrmSize;
            }*/
            // odd lines for top, even lines for bottom
            if ((prDispInfo->u1PicStruct == TOP_FIELD) && ((u4CmpWord % 4) == 3) && (!((u4CmpWord >> 2) % 2)))
            {
                // Skip bottom lines, 16x16 Block mode
                u4CmpWord += 4;    // 4*4 = 16 byte
            }
        }
    }

    if (_fgEnChkSum)
    {
        if (bIsYcomp)
        {
            u4GoldenCS1 = _rCSInfo.u4ShftAddY1[_u4CurFrmIdx];
            u4GoldenCS2 = _rCSInfo.u4ShftAddY2[_u4CurFrmIdx];
        }
        else
        {
            u4GoldenCS1 = _rCSInfo.u4ShftAddC1[_u4CurFrmIdx];
            u4GoldenCS2 = _rCSInfo.u4ShftAddC2[_u4CurFrmIdx];
        }
        if ((u4GoldenCS1 != u4CS1) ||
                (u4GoldenCS2 != u4CS2))
        {
            LOG(0, "//Error! Frm %d %c ChkSum Mismatch (0x%x, golden1 0x%x), (0x%x, golden2 0x%x)\n",
                _u4CurFrmIdx,
                (bIsYcomp) ? 'Y' : 'C',
                u4CS1, u4GoldenCS1,
                u4CS2, u4GoldenCS2);
            return FALSE;
        }
        else
        {
            LOG(0, "//Frm %d %c CS pass\n", _u4CurFrmIdx, (bIsYcomp) ? 'Y' : 'C');
        }
    }
    return TRUE;
}


#ifdef CC_64x32BLK_CMP
//block mode from 64x32 to 16x32
static void _VdecBlkTransform(UINT8* pu1DstBlk16x32,
                              const VDEC_DISP_PIC_INFO_T* prDispInfo, BOOL bIsYcomp)
{
    UINT8* pu1DecData;
    UINT8* apu1Dst[3]; //each entry represent block with width 16
    UINT32 u4CvWByte, u4CvHByte, u4PixIdx, u4BlkW16Idx;
    UINT32 u4BlkH, u4PicH;    //Y: 16x32, CbCr: 16x16

    u4BlkH = (bIsYcomp) ? 32 : 16;
    u4PicH = (bIsYcomp) ? prDispInfo->u4H : (prDispInfo->u4H >> 1);

    //initial
    pu1DecData = (UINT8*)((bIsYcomp) ? prDispInfo->pvYAddr : prDispInfo->pvCAddr);
    apu1Dst[0] = pu1DstBlk16x32;
    for (u4BlkW16Idx = 1; u4BlkW16Idx < 4; u4BlkW16Idx++)
    {
        apu1Dst[u4BlkW16Idx] = apu1Dst[u4BlkW16Idx - 1] + (u4BlkH << 4);  //*16
    }

    //transform
    for (u4CvHByte = 0; u4CvHByte < u4PicH; u4CvHByte += u4BlkH)
    {
        for (u4CvWByte = 0; u4CvWByte < prDispInfo->u4W; u4CvWByte += 64)
        {
            for (u4PixIdx = 0; u4PixIdx < u4BlkH; u4PixIdx++)
            {
                for (u4BlkW16Idx = 0; u4BlkW16Idx < 4; u4BlkW16Idx++)
                {
                    x_memcpy(apu1Dst[u4BlkW16Idx], pu1DecData, 16);
                    pu1DecData += 16;
                    apu1Dst[u4BlkW16Idx] += 16;
                }
            }
            //advance pointer
            for (u4BlkW16Idx = 0; u4BlkW16Idx < 4; u4BlkW16Idx++)
            {
                apu1Dst[u4BlkW16Idx] += (((u4BlkH << 4) << 1) + (u4BlkH << 4));
            }
        }
    }

    HalFlushInvalidateDCache();
}
#endif


static void _Vdec1389B2R(UINT8* pu1RasterData, VDEC_DISP_PIC_INFO_T rDispInfo, BOOL bIsYcomp)
{
    UINT8* pu1DecData;
    UINT32 u4CvWByte, u4CvHByte, u4PixIdx;
    UINT32 u4BlkH, u4PicH;    //Y: 16x32, CbCr: 16x16

    u4BlkH = (bIsYcomp) ? 32 : 16;
    u4PicH = (bIsYcomp) ? rDispInfo.u4H : (rDispInfo.u4H >> 1);

    pu1DecData = (UINT8*)((bIsYcomp) ? rDispInfo.pvYAddr : rDispInfo.pvCAddr);

    for (u4CvHByte = 0; u4CvHByte < u4PicH; u4CvHByte += u4BlkH)
    {
        for (u4CvWByte = 0; u4CvWByte < rDispInfo.u4W; u4CvWByte += 16)
        {
            for (u4PixIdx = 0; u4PixIdx < u4BlkH; u4PixIdx++)
            {
                x_memcpy(pu1RasterData, pu1DecData, 16);
                pu1DecData += 16;
                pu1RasterData += rDispInfo.u4W;
            }
            pu1RasterData -= (u4BlkH * rDispInfo.u4W);
            pu1RasterData += 16;
        }
        pu1RasterData += ((u4BlkH - 1) * rDispInfo.u4W);
    }
}


static INT32 _VdecCmdSendPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    HANDLE_T hThread;
    ASSERT(szArgv != NULL);

    if (!_bIsPatSending)
    {
        switch (_eFmt)
        {
            case VDEC_FMT_H264:
            case VDEC_FMT_H264VER:
                if (i4Argc < 3)
                {
                    LOG(0, "vdec.sp FileInfoDir LoopCnt {Location of PatternFiles.txt, loop times(0=stress test)}");
                    return 0;
                }
                //prepare memory
                x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);

                sprintf(_acFileName, "%s/PatternFiles.txt", szArgv[1]);
#ifdef  IDE_READ_SUPPORT
                LOG(0, "//PatternFiles.txt\n");
#endif
                _LoadFile(_acFileName, (UINT32)_pcFileInfo);

                _i4LoopCnt = StrToInt(szArgv[2]);

                VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
                                       255,  _VdecSendH264Pattern, 0, NULL) == OSR_OK);
                break;
            case VDEC_FMT_MP4:
                VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
                                       255,  _VdecSendMPEG4Pattern, 0, NULL) == OSR_OK);
                break;
            case VDEC_FMT_WMV:
                _i4LoopCnt = StrToInt(szArgv[1]);
                VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
                                       255,  _VdecSendWMVPattern, 0, NULL) == OSR_OK);
                break;
            case VDEC_FMT_AVS:
                VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
                                       255,  _VdecSendAVSPattern, 0, NULL) == OSR_OK);
                break;

#ifdef CC_RV_EMULATION
            case VDEC_FMT_RV:
                return _VdecRvEmu(i4Argc, szArgv);
#endif

            case VDEC_FMT_VP6:
                _i4LoopCnt = StrToInt(szArgv[1]);
                VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
                255,  _VdecSendVP6Pattern, 0, NULL) == OSR_OK);
                break;
#ifdef CC_VP8_EMULATION
            case VDEC_FMT_VP8:
            case VDEC_FMT_WEBP:
                _i4LoopCnt = StrToInt(szArgv[1]);
                VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
                255,  _VdecSendVP8Pattern, 0, NULL) == OSR_OK);
                break;
#endif
            default:
                LOG(0, "Unknown format type\n");
                break;
        }
    }

    return 0;
}


static void _VdecSendAVSPattern(void* pvArg)
{
    INT32 i = 0, j = 0, i4Temp = 0;
    char acPath[MAX_FILENAME_LEN];
    char* pcToken;
    UCHAR *pucCorruptByte = NULL;
    UINT32 u4FileNum = 0, u4Idx = 0;
    INT32 i4LoopCnt = 0;
    HANDLE_T hThread;
    INT32 i4Pos = 0;
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Temp, u4FileSize = 0xFFFFFFFF, u4CorruptTimes;

    LOG(0, "Enter _VdecCmdSendPattern");

    x_memset(acPath, 0, MAX_FILENAME_LEN);
    sprintf(acPath, "d:\\AvsBitstreamList.txt");
    x_memset(_pcFileInfo, 0, FILE_INFO_SIZE);

    x_memset(_acLoadedBitstream, 0, MAX_FILENAME_LEN);

    LOG(0, "Start  to load %s\n", acPath);

#if 0
#if defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo, FILE_INFO_SIZE);
#endif
#if defined(CC_MT5365)
    HalFlushInvalidateDCache();
#endif
#else
    HalFlushInvalidateDCache();
#endif
    _LoadFile(acPath, (UINT32)_pcFileInfo);
#if 0
#if defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo, FILE_INFO_SIZE);
#endif
#if defined(CC_MT5365)
    HalFlushInvalidateDCache();
#endif
#else
    HalFlushInvalidateDCache();
#endif
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny",
    //            acPath, (UINT32)_pcFileInfo));
    //_LoadFile(acPath, pcFileInfo);
    LOG(0, "d:\\AvsBitstreamList.txt 0x%08x /ny", _pcFileInfo);


    pcToken = strtok(_pcFileInfo, ",\n");
    if (pcToken != NULL)
    {
        _u4FileCount = StrToInt(pcToken);
        LOG(0, "Total File Count = %ld\n", _u4FileCount);
    }

#if 0 //move to each line
    pcToken = strtok(NULL, ",\n");
    if (pcToken != NULL)
    {
        strcpy(_acTargetDir, pcToken);
        LOG(0, "The Target Directory is :  \n\"%s\"\n", _acTargetDir);
    }
#endif

    pcToken = strtok(NULL, ",");

    _au4FrmFldNumberTotal = 0x0;
    
    while (i < _u4FileCount)
    {
        strcpy(_acFileDir[i], pcToken + 1);
        LOG(0, "File : %s\n", _acFileDir[i]);
        pcToken = strtok(NULL, ",");
        strcpy(_acTargetDir[i], pcToken);
        //LOG(0, "The Target Directory is : %s\n", _acTargetDir[i]);
        pcToken = strtok(NULL, ",");
        strcpy(_acTestBitstream[i], pcToken);
        pcToken = strtok(NULL, ",");
        strcpy(_acGoldPrefix[i], pcToken);
        pcToken = strtok(NULL, ",");
        strcpy(_acFilePrefix[i], pcToken);
        pcToken = strtok(NULL, ",");
        _afgLoadCompData[i] = StrToInt(pcToken);
        
        pcToken = strtok(NULL, ",");
        _au4FrameNumber[i] = StrToInt(pcToken);
        _au4FrmFldNumberTotal += (_au4FrameNumber[i] + 1);
        
        pcToken = strtok(NULL, ",");
        _au4PicWidth[i] = StrToInt(pcToken);
        pcToken = strtok(NULL, ",");
        _au4PicHeight[i] = StrToInt(pcToken);
        pcToken = strtok(NULL, ",\n");
        _afgLoadGoldenData[i++] = StrToInt(pcToken);
        pcToken = strtok(NULL, ",");
    }

    _u4DecCycles = 0;

#ifndef CC_EMULATION_ONCE
    while (1)
#endif
    {
        LOG(0, "******New While Loop to Scan All For %d Time(s)******\n", (_u4DecCycles + 1));        
#ifdef CC_64x32BLK_CMP
        LOG(0, "$$$$$$ Remind: Now run on DDR3 $$$$$$\n");
#endif
        for (_u4CurFileIdx = 0, _au4FrameNumberCurValidTotal = 0x0; _u4CurFileIdx < _u4FileCount; _u4CurFileIdx++)
        {
            LOG(0, "******New File Start  to load: Total File Cnt = %ld, Total Frm/Fld Cnt = %ld \n", _u4FileCount, _au4FrmFldNumberTotal);
            LOG(0, "Current FIle is %ld\n", _u4CurFileIdx);

            //loop all files
            x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
            x_memset(_pcFileInfo, 0, FILE_INFO_SIZE);

            rPesInfo.u4VldReadPtr = VFIFO_S;
            rPesInfo.u4FifoStart = VFIFO_S;    //should get from dmx fbm
            rPesInfo.u4FifoEnd = VFIFO_E;
            rPesInfo.fgNonBuffer = TRUE;

            LOG(0, "u4VldReadPtr is 0x%X\n", rPesInfo.u4VldReadPtr);
            LOG(0, "u4FifoStart is 0x%X\n", rPesInfo.u4FifoStart);
            LOG(0, "u4FifoEnd is 0x%X\n", rPesInfo.u4FifoEnd);

            //reset frame idx
            _u4CurFrmIdx = 0;

            _fgLoadFail = FALSE;

            _fgDisableGlolden = FALSE;

            x_memset(acPath, 0, MAX_FILENAME_LEN);
            sprintf(acPath, "%sBsInfo.txt", _acFileDir[_u4CurFileIdx]);

            LOG(0, "Start  to load %s\n", acPath);

            sprintf(_acFileName, "%sBsInfo.txt", _acFileDir[_u4CurFileIdx]);
#if 0
#if defined(CC_MT5395)
            HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo, FILE_INFO_SIZE);
#endif
#if defined(CC_MT5365)
            HalFlushInvalidateDCache();
#endif
#else
            HalFlushInvalidateDCache();
#endif
            _LoadFile(_acFileName, (UINT32)_pcFileInfo);
#if 0
#if defined(CC_MT5395)
            HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo, FILE_INFO_SIZE);
#endif
#if defined(CC_MT5365)
            HalFlushInvalidateDCache();
#endif
#else
            HalFlushInvalidateDCache();
#endif
            //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%sBsInfo.txt\" 0x%08x /ny",
            //            _acFileDir[_u4CurFileIdx], (UINT32)_pcFileInfo));
            //_LoadFile(acPath, pcFileInfo);
            LOG(0, "d.load.binary \"%sBsInfo.txt\" 0x%08x /ny \n", _acFileDir[_u4CurFileIdx], (UINT32)_pcFileInfo);


            x_memset(acPath, 0, MAX_FILENAME_LEN);
            sprintf(acPath, "%s.avs", _acTestBitstream[_u4CurFileIdx]);

#if 0
            if ((_afgLoadCompData[_u4CurFileIdx]) &&
                    (x_strcmp(acPath, _acLoadedBitstream)))
#endif
            {
                /*UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s.avs\" 0x27a4000 /ny",
                            _acFileDir[_u4CurFileIdx]));*/

#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine(rPesInfo.u4FifoStart, rPesInfo.u4FifoEnd - rPesInfo.u4FifoStart);
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif
                _LoadFile(acPath, rPesInfo.u4FifoStart);
#if 0
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine(rPesInfo.u4FifoStart, rPesInfo.u4FifoEnd - rPesInfo.u4FifoStart);
#endif
#if defined(CC_MT5365)
                HalFlushInvalidateDCache();
#endif
#else
                HalFlushInvalidateDCache();
#endif

                LOG(0, "d.load.binary \"%s.avs\" 0x27a4000 /ny \n", _acTestBitstream[_u4CurFileIdx]);
                x_memset(_acLoadedBitstream, 0, MAX_FILENAME_LEN);
                sprintf(_acLoadedBitstream, "%s.avs", _acTestBitstream[_u4CurFileIdx]);
            }
#if 0
            else
            {
                LOG(0, "We don't need to load file\n");
            }
#endif

            //parsing bitstream info, offset/type of nalu, and send pes packet
            pcToken = strtok(_pcFileInfo, ",");
            while ( pcToken != NULL )
            {
                if (pcToken != NULL)
                {
                    if (u4FileSize == 0xFFFFFFFF)
                    {
                        u4FileSize = (UINT32)StrToInt(pcToken);
                        pcToken = strtok(NULL, ",");
                        u4CorruptTimes = u4FileSize / 500;
#ifndef CC_EMULATION
                        LOG(3, "CorruptTimes = %d\n", u4CorruptTimes);
                        for (i = 1; i < u4CorruptTimes; i++)
                        {
                            pucCorruptByte = (UCHAR*)(VFIFO_S + i * 500);
                            for (j = 0; j < 10; j++)
                            {
                                u4Temp = (UINT32)rand();
                                pucCorruptByte[j] = (UCHAR)u4Temp;
                            }
                        }
#endif
                        continue;
                    }


                    i4Pos = StrToInt(pcToken);
                    if (i4Pos == 0 && _u4CurFrmIdx != 0)
                    {
                        break;
                    }
#ifndef CC_EMULATION
                    if ((i4Pos == 0 || i4Pos == 1) ||
                            (((i4Pos % 500) > 10) && ((i4Pos % 500) < 494)))
#endif
                    {
                        rPesInfo.fgNonBuffer = TRUE;
                        rPesInfo.u4VldReadPtr = (rPesInfo.u4FifoStart + i4Pos);
                        rPesInfo.ucPicType = *((UCHAR*)(rPesInfo.u4FifoStart + i4Pos + 3));
                        x_thread_delay(30);
                        //rPesInfo.u4Width = _au4PicWidth[_u4CurFileIdx];
                        //rPesInfo.u4Height = _au4PicHeight[_u4CurFileIdx];
                        VDEC_SendEs((void*)&rPesInfo);
                    }
                    //LOG(0, "_VdecSendAVSPattern read ptr pos = %d\n", i4Pos);

                }
                pcToken = strtok(NULL, ",");
            }

            /*while(1)
            {
             if (_u4DecFrmIdx == 0)
                 i4Pos = 0;
             else
                 i4Pos = 47;

             _u4DecFrmIdx++;
             LOG(0, "_VdecCmdSendPattern File Position = %ld\n", i4Pos);
             rPesInfo.u4VldReadPtr = (rPesInfo.u4FifoStart + i4Pos);
             LOG(0, "_VdecCmdSendPattern rPesInfo.u4VldReadPtr = %08x\n", rPesInfo.u4VldReadPtr);
             VDEC_SendEs((void*)&rPesInfo);
            }*/

            rPesInfo.u4VldReadPtr = 0x00000000;
            rPesInfo.fgNonBuffer = TRUE;
            //Printf("Send Null Pointer\n");
            rPesInfo.fgEos = TRUE;
            VDEC_SendEs((void*)&rPesInfo);
            //VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
            while (x_sema_lock_timeout(_hVdecSema, 100000) != OSR_OK)
            {
                rPesInfo.u4VldReadPtr = 0x00000000;
                rPesInfo.fgNonBuffer = TRUE;
                //Printf("Send Null Pointer\n");
                rPesInfo.fgEos = TRUE;
                VDEC_SendEs((void*)&rPesInfo);
            }
            u4FileSize = 0xFFFFFFFF;
        }
        _u4DecCycles++;

#ifdef CC_DRIVER_DEMO
#if 0
#if defined(CC_MT5395)
        HalFlushInvalidateDCacheMultipleLine(_pu1GoldenData, GOLDEN_SIZE);
#endif
#if defined(CC_MT5365)
        HalFlushInvalidateDCache();
#endif
#else
        HalFlushInvalidateDCache();
#endif
#if 0
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary c:\\decodingcycles%ld.txt 0x%08x--0x%08x",
                            _u4DecCycles, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
#endif
#else
        LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif

#ifdef CC_EMULATION_ONCE
        LOG(0, "@@@@@@Scannig all the video bitstreams once\n");
#endif

    }
}


static void _VdecSendH264Pattern(void* pvArg)
{
    char* pcToken;
    UINT32 u4FileNum = 0, u4Idx = 0, u4FrmNum = 0;
    VDEC_PES_INFO_T rPesInfo;
	#ifdef CC_TESTFLOW
	char NameLen = 0;
	char i111 = 0;
	#endif

    _bIsPatSending = TRUE;
    //parsing file name
    pcToken = strtok(_pcFileInfo, "\r\n");
    while ( pcToken != NULL )
    {
        if (pcToken[0] == '#')   //skip the test case
        {
            pcToken = strtok(NULL, "\r\n");
        }
        else
        {
            strcpy(_acFileDir[u4Idx], pcToken);
            pcToken = strtok(NULL, "\r\n");
            if (pcToken != NULL)
            {
                strcpy(_acGoldPrefix[u4Idx++], pcToken);
                pcToken = strtok(NULL, "\r\n");
            }
        }
    }
    u4FileNum = u4Idx;
    LOG(6, "///FileNum %d\n", u4FileNum);
    //loop all files
    x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    rPesInfo.u4FifoStart = _pu1GoldenDatamvc;    //should get from dmx fbm
    rPesInfo.u4FifoEnd = _pu1GoldenDatamvc + 0x7500000;
    do
    {
	LOG(0, "444_i4LoopCnt= %d\n", _i4LoopCnt);
        for (_u4CurFileIdx = 0; _u4CurFileIdx < u4FileNum; _u4CurFileIdx++)
        {
            //reset frame idx
            _u4CurFrmIdx = 0;
            _bIsFirstFrm = TRUE;
            rPesInfo.ucMpvId = VLD0;
#ifdef RR_TEST
            if (_u4YRREn)
            {
                sprintf(_acFileName, "%s/%s.26l_%05d.bitY",
                        _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                _LoadFile(_acFileName, (UINT32)_pu1GoldenData);
                sprintf(_acFileName, "%s/%s.26l_%05d.bitC",
                        _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                _LoadFile(_acFileName, (UINT32)_pu1GoldenDataC);
            }
            else
            {
                sprintf(_acFileName, "%s/%s_0_Y.out",
                        _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                _LoadFile(_acFileName, (UINT32)_pu1GoldenData);
                sprintf(_acFileName, "%s/%s_0_CbCr.out",
                        _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
                _LoadFile(_acFileName, (UINT32)_pu1GoldenDataC);
            }
#endif

            //load BsInfo.txt
            if (_u1TestSet == 1)       //SarnOff
            {
                sprintf(_acFileName, "R:/264TS/Sarnoff/BsInfo/%s.264.info", _acGoldPrefix[_u4CurFileIdx]);
            }
            else if (_u1TestSet == 2)   //Allegro2
            {
                sprintf(_acFileName, "D:/Project/539xVDEC/264_For_DTV/BsInfo2/%s/BsInfo.txt", _acGoldPrefix[_u4CurFileIdx]);
            }
            else
            {
                sprintf(_acFileName, "%s/BsInfo.txt", _acFileDir[_u4CurFileIdx]);
                sprintf(_acChkSumFile, "%s/ChkSum.txt", _acFileDir[_u4CurFileIdx]);
            }
#ifdef  IDE_READ_SUPPORT
            LOG(0, "//BsInfo.txt\n");
#endif
            _LoadFile(_acFileName, (UINT32)_pcFileInfo);
            if (_fgEnChkSum)
            {
                x_memset((void*)&_rCSInfo, 0,  sizeof(VDEC_CHKSUM_INFO_T));
                _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);
                u4FrmNum = _VdecGetChkSum(_pcChkSum, &_rCSInfo);
                LOG(1, "Chksum FrmNum %d\n", u4FrmNum);
            }
            //parsing bitstream file name
            pcToken = strtok(_pcFileInfo, "\r\n");
            if (pcToken != NULL)
            {
                sprintf(_acFileName, "%s/%s", _acFileDir[_u4CurFileIdx], pcToken);
				#ifdef CC_TESTFLOW
				if(fgSaveCrc)
				{
					NameLen = strlen(pcToken);
					printf("namelen = %d\n",NameLen);
					for(i111 = 0; i111<NameLen - 4; i111++)
					{
						_acFileNamecrc[i111] = pcToken[i111];
					}
					_acFileNamecrc[i111] = pcToken[NameLen];
					_acFileNamecrc[i111+1] = pcToken[NameLen+1];
					printf("name = %s\n",_acFileNamecrc);
				}
				#endif
#ifdef  IDE_READ_SUPPORT
                LOG(0, "//%s\n", pcToken);
#endif
                _LoadFile(_acFileName, rPesInfo.u4FifoStart);
            }
            //parsing bitstream info, offset/type of nalu, and send pes packet
            u4CurFrmIdx22222222 = 0;
            pcToken = strtok(NULL, "\r\n");
            while ( pcToken != NULL )
            {
                rPesInfo.ucPicType = StrToInt(pcToken);
                pcToken = strtok(NULL, "\r\n");
                if (pcToken != NULL)
                {
                    rPesInfo.u4VldReadPtr = (rPesInfo.u4FifoStart + StrToInt(pcToken)) + 1; // +1 eliminate ucPicType
                    VDEC_SendEs((void*)&rPesInfo);
					u4CurFrmIdx22222222++;
#ifdef RR_TEST
                    if (_u4YRREn)
                    {
                        while (rPesInfo.ucPicType == 0x65)
                        {
                            VDEC_SendEs((void*)&rPesInfo);
                        }
                    }
                    else
                    {
                        while (rPesInfo.ucPicType == 0x25)
                        {
                            VDEC_SendEs((void*)&rPesInfo);
                        }
                    }
#endif
                    pcToken = strtok(NULL, "\r\n");
                }
            }
            rPesInfo.ucPicType = 0;
            VDEC_SendEs((void*)&rPesInfo);
            VDEC_SendEs((void*)&rPesInfo);  //for remain 1 Q entry in sendES
            rPesInfo.ucMpvId = VDEC_MAX_VLD;
            VDEC_SendEs((void*)&rPesInfo);
            LOG(0, "//File %d Finish ES Sending\n", _u4CurFileIdx);
            VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
            LOG(0, "//File %d Finish ES Decoding\n", _u4CurFileIdx);
			#ifdef CC_TESTFLOW
			if(fgSaveCrc)
			{
			//back up crc
			LOG(0, "//d.save.binary \"%s/%s.crc\" 0x%08x--0x%08x\n",
                    _acFileDir[_u4CurFileIdx], _acFileNamecrc,
                    (UINT32)_pu1GoldenDatacrc, (UINT32)_pu1GoldenDatacrc + u4CrcSize);
                    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s.CRC\" 0x%08x--0x%08x", 
                    _acFileDir[_u4CurFileIdx],_acFileNamecrc,
                    (UINT32)_pu1GoldenDatacrc, (UINT32)_pu1GoldenDatacrc + u4CrcSize -1));
			}
			#endif
            //re-play, re-init
            VDEC_Play(0, VDEC_FMT_H264VER);
            VDEC_SetParam(ES0, OUTPUT_DISP_ORDER, _bCmpEn, 0, 0);
            VDEC_SetParam(ES0, RRMODE, _u4YRREn, 0, _u4YRRRatio);
            VDEC_SetParam(ES0, RRMODE, _u4CRREn, 1, _u4CRRRatio);
        }
		LOG(0, "111_i4LoopCnt= %d\n", _i4LoopCnt);

        if (_i4LoopCnt > 0)
        {
            _i4LoopCnt--;
        }
        else
        {
            _i4LoopCnt = -1;    //forever loop
        }
		LOG(0, "222_i4LoopCnt= %d\n", _i4LoopCnt);
        if (_i4LoopCnt == 0)
        {
            break;
        }
    }
    while (1);

    _bIsPatSending = FALSE;
    LOG(0, "//Finish All ES Decoding\n");

}


static void _VdecSendMPEG4Pattern(void* pvArg)
{
	INT32 i = 0, i4Temp = 0;
	char acPath[MAX_FILENAME_LEN];
	char* pcToken;
	UINT32 u4FileNum = 0,u4Idx = 0;
	INT32 i4LoopCnt = 0;
	HANDLE_T hThread;
	INT32 i4Pos = 0; 
	VDEC_PES_INFO_T rPesInfo;
	UINT32 u4SentFrames = 0;

	char* pcBitstream;

	Printf( "Enter _VdecCmdSendPattern\n");
	Printf( "Start MT5363 verification\n");

	x_memset(acPath, 0, MAX_FILENAME_LEN);
	//sprintf(acPath, "c:\\BitstreamListConformance.txt");
	sprintf(acPath, "c:\\BitstreamListDivX.txt");
	x_memset(_pcFileInfo, 0, FILE_INFO_SIZE);	

	Printf( "Start  to load %s\n", acPath);
       //_LoadFile(acPath, (UINT32)_pcFileInfo);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", 
				acPath, (UINT32)_pcFileInfo));
	//_LoadFile(acPath, pcFileInfo);
	Printf( "d.load.binary c:\\BitstreamListDivX.txt 0x%08x /ny", _pcFileInfo);

				
	pcToken = strtok(_pcFileInfo, ",\n");
	if(pcToken != NULL)
	{
		_u4FileCount = StrToInt(pcToken);
		Printf( "Total File Count = %ld\n", _u4FileCount);
	}

	pcToken = strtok(NULL, ",");
	while(i < _u4FileCount)
	{
		strcpy(_acFileDir[i], pcToken + 1);
		Printf( "File : %s\n", _acFileDir[i]);
		pcToken = strtok(NULL, ",");
		strcpy(_acTestBitstream[i], pcToken);
		pcToken = strtok(NULL, ",");
		/*strcpy(_acGoldPrefix[i], pcToken);
		pcToken = strtok(NULL, ",");*/
		strcpy(_acFilePrefix[i], pcToken);
		pcToken = strtok(NULL, ",");
		_afgLoadCompData[i] = StrToInt(pcToken);
		pcToken = strtok(NULL, ",");
		_au4FrameNumber[i] = StrToInt(pcToken);
		pcToken = strtok(NULL, ",");
		_au4PicWidth[i] = StrToInt(pcToken);
		pcToken = strtok(NULL, ",\n");
		_au4PicHeight[i++] = StrToInt(pcToken);
		pcToken = strtok(NULL, ",");
	} 

       UINT32 u4FrmNum;
	//pcBitstream = BSP_AllocAlignedDmaMemory(3 * 1024 * 1024, 1024);

	while (1)
	{
		for(_u4CurFileIdx = 0; _u4CurFileIdx < _u4FileCount; _u4CurFileIdx++)
		{
			//loop all files
			x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
			x_memset(_pcFileInfo, 0, FILE_INFO_SIZE);
			
			rPesInfo.u4FifoStart = VFIFO_S;    //should get from dmx fbm
			rPesInfo.u4FifoEnd = VFIFO_E;
			//rPesInfo.u4VldReadPtr = pcBitstream;
			//rPesInfo.u4FifoStart = pcBitstream;	//should get from dmx fbm
			//rPesInfo.u4FifoEnd = pcBitstream + 3 * 1024 * 1024;

			_u4Dx311PicWidth = _au4PicWidth[_u4CurFileIdx];
			_u4Dx311PicHeight = _au4PicHeight[_u4CurFileIdx];

#ifdef IC_MPEG4_VERIFICATION
            VDEC_MPEG4SetFilenameAndFrameNum(_acFilePrefix[_u4CurFileIdx], 0);
#endif
			//reset frame idx
			_u4CurFrmIdx = 0; 
                
			_fgLoadFail = FALSE;

			u4SentFrames = 0;
			x_memset(acPath, 0, MAX_FILENAME_LEN);
			sprintf(acPath, "%sBsInfo.txt", _acFileDir[_u4CurFileIdx]);
			
		       Printf("Start  to load %s\n", acPath);

                    sprintf(_acFileName, "%sBsInfo.txt", _acFileDir[_u4CurFileIdx]);
                    //_LoadFile(_acFileName, (UINT32)_pcFileInfo);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%sBsInfo.txt\" 0x%08x /ny", 
						_acFileDir[_u4CurFileIdx], (UINT32)_pcFileInfo));		
			//_LoadFile(acPath, pcFileInfo);
			Printf("d.load.binary \"%sBsInfo.txt\" 0x%08x /ny \n", _acFileDir[_u4CurFileIdx], (UINT32)_pcFileInfo);


			x_memset(acPath, 0, MAX_FILENAME_LEN);
			//sprintf(acPath, "%s.m4v", _acTestBitstream[_u4CurFileIdx]);
                     sprintf(acPath, "%s.m4v", _acTestBitstream[_u4CurFileIdx]);
			if (_afgLoadCompData[_u4CurFileIdx])
			{
				UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s.m4v\" 0x5134000 /ny", 
							_acTestBitstream[_u4CurFileIdx]));
				//_afgLoadCompData[_u4CurFileIdx] = 0;

				//_LoadFile(acPath, rPesInfo.u4FifoStart);

				Printf( "d.load.binary \"%s.m4v\" 0x5134000 /ny \n", _acTestBitstream[_u4CurFileIdx]);
			}
			else
			{
				Printf("We don't need to load file\n");
			}

                    sprintf(_acChkSumFile, "%sChkSum.txt", _acGoldPrefix[_u4CurFileIdx]);
                    if(_fgEnChkSum)
                    {
                        x_memset((void*)&_rCSInfo, 0,  sizeof(VDEC_CHKSUM_INFO_T));
                        _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);
                        u4FrmNum = _VdecGetChkSum(_pcChkSum, &_rCSInfo);
                        LOG(1, "Chksum FrmNum %d\n", u4FrmNum);
                    }            


			//parsing bitstream info, offset/type of nalu, and send pes packet	 
			pcToken = strtok(_pcFileInfo, ",");
			while( pcToken != NULL )
			{
				if(pcToken != NULL)
				{
					i4Pos = StrToInt(pcToken);
					if ((i4Pos == 0 && _u4CurFrmIdx != 0) ||
					    (u4SentFrames > _au4FrameNumber[_u4CurFileIdx]))
					{
						break;
					}
				       //Printf("bitstream pos = %ld\n", i4Pos);
					rPesInfo.fgNonBuffer = TRUE;
                                   rPesInfo.u4VldReadPtr = (rPesInfo.u4FifoStart + i4Pos);
                                   //Printf("VDec Send Mpeg4 Pattern Read ptr = 0x%08x\n", rPesInfo.u4VldReadPtr);
					//rPesInfo.u4Width = _au4PicWidth[_u4CurFileIdx];
					//rPesInfo.u4Height = _au4PicHeight[_u4CurFileIdx];
					VDEC_SendEs((void*)&rPesInfo);
					u4SentFrames++;
				}
				pcToken = strtok(NULL, ",");
			}

            /*while(1)
            {
                if (_u4DecFrmIdx == 0)
                    i4Pos = 0;
                else
                    i4Pos = 47;

                _u4DecFrmIdx++;
                LOG(0, "_VdecCmdSendPattern File Position = %ld\n", i4Pos);
                rPesInfo.u4VldReadPtr = (rPesInfo.u4FifoStart + i4Pos);
                LOG(0, "_VdecCmdSendPattern rPesInfo.u4VldReadPtr = %08x\n", rPesInfo.u4VldReadPtr);
                VDEC_SendEs((void*)&rPesInfo);
            }*/

			rPesInfo.u4VldReadPtr = 0x00000000;
		       //rPesInfo.fgNonBuffer = TRUE;
		       Printf("Send Null Pointer\n");
			rPesInfo.u4VldReadPtr = 0x00000000;
		       //rPesInfo.fgNonBuffer = TRUE;
		       Printf("Send Null Pointer\n");
		       rPesInfo.fgEos = TRUE;
			VDEC_SendEs((void*)&rPesInfo);
			VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);	//wait decode done

			/*UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_.out\" 0x%08x--0x%08x", 
				"c:\\", _acFilePrefix[_u4CurFileIdx], (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));*/
		}

              Printf("Verification finishes\n");
             
		_u4DecCycles++;
	/*	#ifdef CC_DRIVER_DEMO
		UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary c:\\decodingcycles%ld.txt 0x%08x--0x%08x", 
		_u4DecCycles, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
		UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary c:\\decodingcycles%ld.m4v 0x%08x--0x%08x", 
                     _u4DecCycles, (UINT32)pcBitstream, (UINT32)pcBitstream + 3 * 1024 * 1024));
           #else
            Printf( "CC_DRIVER_DEMO !defined, File save fail~");
            #endif
    */
    }
}

BOOL _VdecWMVIsX8Intra(UINT32 u4addr, UINT32 u4addr2)
{
    return FALSE;
}

void _VdecVParserWMV(VDEC_PES_INFO_T* pVDec_Pes_Info)
{
#ifdef CC_DRIVER_DEMO

    VDEC_PES_INFO_T* rPesInfo = pVDec_Pes_Info;

    UINT32 u4RCVVersion = 0;
	UINT32 u4CodecVersion = 0;
    UINT32 u4RcvNumFrames = 0;
	UINT32 u4SetPos = 0;
    UINT32 u4PicHeightSrc = 0;
	UINT32 u4PicWidthSrc = 0;
    UINT32 u4HeaderLen =0;
    UINT32 u4SeqHdrData1 = 0;
	UINT32 u4SeqHdrDataLen = 0;
    BOOL bSkipFrame = FALSE;
    UINT32 hdrext = 0;
    UINT32 u4FrameIndex = 0;

    u4SetPos = 0;

    if ( rPesInfo->u4VideoType == DMX_VIDEO_VC1) //.vc1
    {
        // vc-1 video type
    }
    else if ( rPesInfo->u4VideoType != DMX_VIDEO_VC1 ) //.rcv
    {
        x_memcpy(&u4RcvNumFrames, ((rPesInfo->u4FifoStart) + u4SetPos), 4);
        u4SetPos += 4;
        u4RCVVersion = (u4RcvNumFrames >> 30) & 0x1; //[30]
        u4CodecVersion = u4RcvNumFrames >> 24; //[29:24]
        u4RcvNumFrames &= 0xffffff; //[23:0]
        hdrext = (u4CodecVersion >> 7) & 0x1;

        if (u4RCVVersion == 0)
        {
            u4CodecVersion &= 0x7f;
        }
        else
        {
            u4CodecVersion &= 0x3f;
        }

        if (u4CodecVersion == 0) //wmv7
        {
            rPesInfo->u4VideoType = DMX_VIDEO_WMV7;//WMV1;
        }
        else if (u4CodecVersion == 2) //WMV8
        {
            rPesInfo->u4VideoType = DMX_VIDEO_WMV8;//WMV2;
        }
        else if (u4CodecVersion == 5) //Simple & Main Profile
        {
            rPesInfo->u4VideoType = DMX_VIDEO_WMV9;//WMV3;
        }
        else if (u4CodecVersion == 6) //Advanced Profile
        {
            rPesInfo->u4VideoType = DMX_VIDEO_VC1;//WMVA;
        }
        else if (u4CodecVersion == 8) //Advanced Profile
        {
            rPesInfo->u4VideoType = DMX_VIDEO_VC1;//WMVA;
        }
        else
        {
            rPesInfo->u4VideoType = 0xff; //Uunknow type
            LOG(0, "_VdecVParserWMV : unknow type\n");
        }
    }

    if (rPesInfo->u4VideoType != DMX_VIDEO_VC1) //for others , no start code
    {
        if (hdrext != 0)
        {
            //parsing SeqHdrDataLen
            x_memcpy(&u4SeqHdrDataLen, ((rPesInfo->u4FifoStart) + u4SetPos), 4);
            u4SetPos += 4;

            //parsing SeqHdrData1
            //headerData( headerLen * 8 bits )
            x_memcpy(&u4SeqHdrData1, ((rPesInfo->u4FifoStart) + u4SetPos), 4);
            u4SetPos += 4;
            u4SeqHdrData1 = ((u4SeqHdrData1 & 0x000000FF) << 24) | ((u4SeqHdrData1 & 0x0000FF00) << 8) | ((u4SeqHdrData1 & 0x00FF0000) >> 8) | ((u4SeqHdrData1 & 0xFF000000) >> 24);
            if (u4SeqHdrDataLen == 5)
            {
                u4SetPos += 1;
            }
        }
        //parsing picture height
        x_memcpy(&u4PicHeightSrc, ((rPesInfo->u4FifoStart) + u4SetPos), 4);
        u4SetPos += 4;
        //parsing picture width
        x_memcpy(&u4PicWidthSrc, ((rPesInfo->u4FifoStart) + u4SetPos), 4);
        u4SetPos += 4;

        LOG(0, "_VdecVParserWMV : Source Resolution = %d x %d\n", u4PicWidthSrc, u4PicHeightSrc);
        /*
        if( rcv_version == 1 )
        {
          rcv_add_hdr_size(32 bits)
          level(2 bits)
          res(1 bit)
          bCBR(1 bit)
          res(28 bits)
          bitrate(32 bits)
          framerate(32 bits)
          total bit = 128bits = 16 bytes
        }
        */
        //parsing additional header information
        if (u4RCVVersion == 1)
        {
            u4SetPos += 16;
        }
        x_memcpy(&u4HeaderLen, ((rPesInfo->u4FifoStart + u4SetPos)), 4);
        //u4HeaderLen += 4;
        u4SetPos += 4;
        u4HeaderLen &= 0x0fffffff;

        if (u4HeaderLen == 1) //it indicates Picture Length == 1, means a skipped frame
        {
            //bPreviousSkip = TRUE;
            bSkipFrame = TRUE;
            LOG(0, "Frame %d : Skip Frame \n", u4FrameIndex);
        }
        else
        {
            //bPreviousSkip = FALSE;
            bSkipFrame = FALSE;
        }

        if (u4RCVVersion == 1)
        {
            //parsing time_stamp syntax element
            u4SetPos += 4;
            //u4HeaderLen += 4;
        }


        //rPesInfo->fgGop = bSkipFrame;
        rPesInfo->u4VldReadPtr = u4SetPos + rPesInfo->u4FifoStart;
        VDEC_SetParam(0, 0, u4PicWidthSrc, u4PicHeightSrc, rPesInfo->ucPicType);
        VDEC_SetParam(0, 1, u4SeqHdrDataLen, u4SeqHdrData1, bSkipFrame);
#ifdef CC_WMV_EMULATION
        rPesInfo->fgSkipFrame = bSkipFrame;
#endif
        if (rPesInfo->u4VideoType == DMX_VIDEO_WMV8 && _VdecWMVIsX8Intra(rPesInfo->u4VldReadPtr, rPesInfo->u4FifoStart))
        {
            return;
        }
        else
        {
            VDEC_SendEs((void*)rPesInfo);
            u4FrameIndex++;
        }
        do
        {
            u4SetPos += u4HeaderLen;
            rPesInfo->u4VldReadPtr = u4SetPos;
            x_memcpy(&u4HeaderLen, ((rPesInfo->u4VldReadPtr + rPesInfo->u4FifoStart)), 4);
            u4SetPos += 4;
            u4HeaderLen &= 0x0fffffff;

            if (u4HeaderLen == 1) //it indicates Picture Length == 1, means a skipped frame
            {
                bSkipFrame = TRUE;
                LOG(0, "Frame %d : Skip Frame \n", u4FrameIndex);
            }
            else
            {
                bSkipFrame = FALSE;
            }

            if (u4RCVVersion == 1)
            {
                //parsing time_stamp syntax element
                u4SetPos += 4;
            }
            rPesInfo->u4VldReadPtr = u4SetPos + rPesInfo->u4FifoStart;
            //rPesInfo->fgGop = bSkipFrame;
            //VDEC_SetParam(0,1,u4SeqHdrDataLen,u4SeqHdrData1,bSkipFrame);
#ifdef CC_WMV_EMULATION
            rPesInfo->fgSkipFrame = bSkipFrame;
#endif
            if ((rPesInfo->u4VldReadPtr - rPesInfo->u4FifoStart  + WMV_PIC_HEADER_LEN) < bitstream_sz[_u4CurFileIdx])
            {
                if (rPesInfo->u4VideoType == DMX_VIDEO_WMV8 && _VdecWMVIsX8Intra(rPesInfo->u4VldReadPtr, rPesInfo->u4FifoStart))
                {
                    return;
                }
                else
                {
                    VDEC_SendEs((void*)rPesInfo);
                    u4FrameIndex++;
                }
            }
            else
            {
                break;
            }
        }
        while ((rPesInfo->u4VldReadPtr - rPesInfo->u4FifoStart + WMV_PIC_HEADER_LEN) < bitstream_sz[_u4CurFileIdx]);
    }
    else //start code version
    {
        CHAR* pcToken;
        //VDEC_SetParam(0,0,rPesInfo->ucPicType,0,0);
        x_memset((void*)_pcStartCodeFile, 0, FILE_INFO_SIZE);
        HalFlushInvalidateDCacheMultipleLine((UINT32)_pcStartCodeFile, FILE_INFO_SIZE);
        LOG(0, "d.load.binary \"%s\\StartCode_%s.txt\" 0x%08x /ny\n", "D:\\project\\wmv\\vc1_start_code"/*golden_path[_u4CurFileIdx]*/, _acGoldPrefix[_u4CurFileIdx], _pcStartCodeFile);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\StartCode_%s.txt\" 0x%08x /ny", "D:\\project\\wmv\\vc1_start_code"/*golden_path[_u4CurFileIdx]*/, _acGoldPrefix[_u4CurFileIdx], _pcStartCodeFile));
        HalFlushInvalidateDCacheMultipleLine((UINT32)_pcStartCodeFile, FILE_INFO_SIZE);
        pcToken = strtok(_pcStartCodeFile, "\r\n");
        while ( pcToken != NULL)
        {
            rPesInfo->u4VldReadPtr = StrToInt(pcToken) + rPesInfo->u4FifoStart;
            VDEC_SendEs((void*)rPesInfo);
            pcToken = strtok(NULL, "\r\n");
        }
    }
#endif
}

char *get_wmv_cur_filename(VOID)
{
  return _acFileDir[_u4CurFileIdx];
}

void _VdecSendWMVPattern(void* pvArg)
{
    VDEC_PES_INFO_T rPesInfo;
    CHAR* pcToken;
    UINT32 iLen,u4BitStreamStartPos;
    UINT16 u2file_num=0;
    UINT16 index=0,u2SetIndex=0;    
    char acPath[MAX_FILENAME_LEN];            
    char *pattern_path=NULL;//[512]= "d:\\wmv_test\\Pattern.txt";
    char *golden_path_arg=NULL;//[512] ="d:\\wmv_test\\Golden.txt";
    do
    {
       for(u2SetIndex=0;u2SetIndex<VDEC_VERIFY_SET_NUM;u2SetIndex++)
       {
          if(!rVerifySet[u2SetIndex].u4Enable)
          {
            continue;
          }
          u2file_num=0;	
		  index=0;
		  pattern_path=rVerifySet[u2SetIndex].rPatternFile;
		  golden_path_arg=rVerifySet[u2SetIndex].rGoldenFile;
          x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);
#ifdef CC_WMV_EMULATION
          x_memset((void*)_auNGFile, 0, MAX_FILE_NUM);
#endif
		  HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo,FILE_INFO_SIZE);
          //load pattern file to dram
          LOG(0,"====StartSet %d=====\n",u2SetIndex);
          LOG(0,"d.load.binary \"%s\" 0x%08x /ny\n",pattern_path, _pcFileInfo);
          UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pattern_path, _pcFileInfo));
          
          pcToken = strtok(_pcFileInfo, "\r\n");
          while( pcToken != NULL) 
          { 
            strcpy(_acFileDir[u2file_num], pcToken);
            pcToken = strtok(NULL, "\r\n");
            bitstream_sz[u2file_num++] = StrToInt(pcToken);
            pcToken = strtok(NULL, "\r\n");
          }

          x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);
	      HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo,FILE_INFO_SIZE);
          //load golden pattern to dram
          LOG(0,"d.load.binary \"%s\" 0x%08x /ny\n",golden_path_arg, _pcFileInfo);
          UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", golden_path_arg, _pcFileInfo));

          pcToken = strtok(_pcFileInfo, "\r\n");
          while( pcToken != NULL) 
          {
            strcpy(golden_path[index], pcToken);
            pcToken = strtok(NULL, "\r\n");
            strcpy(_acGoldPrefix[index++], pcToken);
            pcToken = strtok(NULL, "\r\n");
          }

		  //wmv crc
	      if(rVerifySet[u2SetIndex].u4StartFile<u2file_num)
	      {
             _u4CurFileIdx=rVerifySet[u2SetIndex].u4StartFile;
	      }
		  else
		  {
             _u4CurFileIdx=0;
		  }
		  
          for(; _u4CurFileIdx < u2file_num ; _u4CurFileIdx++)
          {
#ifdef CC_WMV_EMULATION
		     _fgAbtCurItem = FALSE;
             _u4CrcIndex=0;
#endif
             _u4CurFrmIdx =0;
             x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
            
             if(_acFileDir[_u4CurFileIdx][0] == '#')
             {
                  continue;    //skip the pattern
             }
#ifdef CC_WMV_EMULATION
			 if(bitstream_sz[_u4CurFileIdx]>0x3C00000)
			 {
                Printf("Skip file %d:%s\n",_u4CurFileIdx,_acFileDir[_u4CurFileIdx]);
				_auNGFile[_u4CurFileIdx]=WMV_FILE_SKIP;
				continue;
			 }
#endif			 
			 //wmv crc
			 u4BitStreamStartPos=VFIFO_E-bitstream_sz[_u4CurFileIdx];
             /*
			 if(u4BitStreamStartPos>=VFIFO_S)
			 {
                        u4BitStreamStartPos=VFIFO_S;
			 }
			 else
			 */
			 {
                u4BitStreamStartPos=u4BitStreamStartPos-(u4BitStreamStartPos&(0x10000-1))-0x100000;
			 }			 
			 HalFlushInvalidateDCacheMultipleLine((UINT32)u4BitStreamStartPos, bitstream_sz[_u4CurFileIdx]+2048);
             Printf("Progress:(%d/%d)\n",_u4CurFileIdx,u2file_num);
			 
			// ASSERT((VFIFO_E-VFIFO_S)> bitstream_sz[_u4CurFileIdx]);
			 
             //get input_bitstream to vFIFO start point
             x_memset(acPath, 0, MAX_FILENAME_LEN);
             sprintf(acPath, "%s", _acFileDir[_u4CurFileIdx]);
#ifdef WMV_PROFILING
             HAL_GetTime(&rTimeRstS);
#endif
             _LoadFile(acPath,u4BitStreamStartPos);
#ifdef WMV_PROFILING
             HAL_GetTime(&rTimeRstE);
             HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
             Printf("WMV loading bitstream time = (%ld) \n", rTimeRstDt.u4Seconds);
#endif
			 HalFlushInvalidateDCacheMultipleLine((UINT32)u4BitStreamStartPos, bitstream_sz[_u4CurFileIdx]+1024);
             //load checksum
             if(_fgEnChkSum)
             {
                UINT32 u4FrmNum =0;
                sprintf(_acChkSumFile, "%s/ChkSum.txt", golden_path[_u4CurFileIdx]);
                x_memset((void*)&_rCSInfo, 0,  sizeof(VDEC_CHKSUM_INFO_T));
#ifdef WMV_PROFILING
                HAL_GetTime(&rTimeRstS);
#endif
                _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);
#ifdef WMV_PROFILING
                HAL_GetTime(&rTimeRstE);
                HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
                Printf("WMV loading checksum time = (%ld) \n", rTimeRstDt.u4Micros);
#endif

                u4FrmNum = _VdecGetChkSum(_pcChkSum, &_rCSInfo);
                LOG(1, "Chksum FrmNum %d\n", u4FrmNum);
             }
             //wmv crc
			 if(_fgEnableCRC)
			 {
                x_memset(_acChkSumFile,0,MAX_FILENAME_LEN);
				Printf("WMV loading CRC file:%s\\%s_crc.bin\n",golden_path[_u4CurFileIdx],_acGoldPrefix[_u4CurFileIdx]);
                sprintf(_acChkSumFile, "%s/%s_crc.bin", golden_path[_u4CurFileIdx],_acGoldPrefix[_u4CurFileIdx]);
#ifdef WMV_PROFILING
				HAL_GetTime(&rTimeRstS);
#endif
			   HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
			   _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);
			   HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
#ifdef WMV_PROFILING
				HAL_GetTime(&rTimeRstE);
				HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
				Printf("WMV loading CRC time = (%ld) \n", rTimeRstDt.u4Micros);
#endif
			 }
			
             iLen = x_strlen(_acFileDir[_u4CurFileIdx]);

             if((_acFileDir[_u4CurFileIdx][iLen-4] == '.') && (_acFileDir[_u4CurFileIdx][iLen-3] == 'v') &&
                 (_acFileDir[_u4CurFileIdx][iLen-2] == 'c') && (_acFileDir[_u4CurFileIdx][iLen-1] == '1') )
             {
                rPesInfo.u4VideoType = DMX_VIDEO_VC1;
             }
             else if( (_acFileDir[_u4CurFileIdx][iLen-4] == '.') && (_acFileDir[_u4CurFileIdx][iLen-3] == 'r') &&
                (_acFileDir[_u4CurFileIdx][iLen-2] == 'c') && (_acFileDir[_u4CurFileIdx][iLen-1] == 'v') )
             {
                rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
             }

#ifdef WMV_PROFILING
            HAL_GetTime(&rTimeRstS);
#endif

             rPesInfo.u4VldReadPtr = u4BitStreamStartPos;
            rPesInfo.u4FifoStart = u4BitStreamStartPos;
            rPesInfo.u4FifoEnd = VFIFO_E;
            _VdecVParserWMV(&rPesInfo);

            //indicate last frame
            rPesInfo.ucPicType = 255;
            VDEC_SendEs((void*)&rPesInfo);

            //send eos to push last frame to decoder
            rPesInfo.fgEos = TRUE;
            VDEC_SendEs((void*)&rPesInfo);
            VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
#ifdef WMV_PROFILING
            HAL_GetTime(&rTimeRstE);
            HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
            Printf("WMV decoding and compare time = (%ld) \n", rTimeRstDt.u4Seconds);
#endif
            //printf("_VdecSendWMVPattern : end of file %d\n",_u4CurFileIdx);
            //re-play, re-init
            VDEC_Play(0, VDEC_FMT_WMV);
        }
#ifdef CC_WMV_EMULATION
		  Printf("Check fail files in %s\n",pattern_path);
          for(_u4CurFileIdx=0; _u4CurFileIdx < u2file_num ; _u4CurFileIdx++)
          {
             if(_auNGFile[_u4CurFileIdx]==WMV_FILE_NG)
             {
                Printf("%s\n",_acFileDir[_u4CurFileIdx]);
				Printf("%d\n",bitstream_sz[_u4CurFileIdx]);                
             }
          }

		  Printf("Skip files in %s\n",pattern_path);
          for(_u4CurFileIdx=0; _u4CurFileIdx < u2file_num ; _u4CurFileIdx++)
          {
             if(_auNGFile[_u4CurFileIdx]==WMV_FILE_SKIP)
             {
                Printf("%s\n",_acFileDir[_u4CurFileIdx]);
				Printf("%d\n",bitstream_sz[_u4CurFileIdx]);                
             }
          }
#endif
       }

       if(_i4LoopCnt > 0)
       {
          _i4LoopCnt--;
       }
       else
       {
          _i4LoopCnt = -1;    //forever loop
       }
       if(_i4LoopCnt == 0)
       {
          break;
       }
    }while(1);
}

void _VdecVParserVP6(VDEC_PES_INFO_T* pVDec_Pes_Info)
{
    VDEC_HDR_INFO_T rHdrInfo;
    UINT32 u4SizeAddr=0,u4FrameAddr=0,u4FrameIndex=0;
    UINT32 u4FrameSize=0, u4NextFrameSize;
    CHAR* pcToken;
    UINT32 u4FrameCnt;

    // get pic_cnt.txt 
    x_memset(_acChkSumFile, 0, MAX_FILENAME_LEN);
    x_memset(_pcChkSum, 0, 128);
    sprintf(_acChkSumFile, "%s/%s/pic_cnt.txt", _acFileDir[1],_acFileDir[2]);
    _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);
    pcToken = strtok(_pcChkSum, "\r\n");
    u4FrameCnt = StrToInt(pcToken);
    printf("FrameCnt %d\n", u4FrameCnt);

    if(pVDec_Pes_Info->u4VldReadPtr==pVDec_Pes_Info->u4FifoStart)
    {
        x_memset((void*)_pcStartCodeFile, 0, FILE_INFO_SIZE);
        sprintf(_acChkSumFile, "%s/%s.vdo.size", _acFileDir[0],_acFileDir[2]);
        _LoadFile(_acChkSumFile, (UINT32)_pcStartCodeFile);
    }

    u4SizeAddr=(UINT32)(_pcStartCodeFile+4);
    u4FrameAddr=pVDec_Pes_Info->u4FifoStart;

    while (u4FrameIndex < u4FrameCnt - 1)
    {
        memset(&rHdrInfo, 0, sizeof(VDEC_HDR_INFO_T));
        u4FrameSize=*((UINT32 *)u4SizeAddr);
        u4NextFrameSize=*((UINT32 *)(u4SizeAddr+4));
        if(u4FrameSize == 0 || u4NextFrameSize == 0)
        {
            break;
        }

        pVDec_Pes_Info->u4VldReadPtr = u4FrameAddr;
        pVDec_Pes_Info->u4TickNum = u4FrameSize;
        pVDec_Pes_Info->u4PTS=u4FrameIndex*9000;
        pVDec_Pes_Info->ucPicType=1;
        pVDec_Pes_Info->u1AlphaFlag     = 0;

        if (_u4VP6EnableAlpha)
        {
            UINT8* pData = (UINT8*)u4FrameAddr;
            UINT32 u4YCbCrOfst = u4FrameAddr + 3;
            UINT32 u4AlphaOfst = ((((UINT32)pData[0])<<16) | (((UINT32)pData[1])<<8) | (((UINT32)pData[2])<<0)) + u4YCbCrOfst;

            pVDec_Pes_Info->u4VldReadPtr    = u4YCbCrOfst;
            pVDec_Pes_Info->u1AlphaFlag     = VP6_ALPHA_ENABLE;
            VDEC_SendEs(pVDec_Pes_Info);

            pVDec_Pes_Info->u4VldReadPtr    = u4AlphaOfst;
            pVDec_Pes_Info->u1AlphaFlag     = VP6_ALPHA_ENABLE | VP6_ALPHA_FRAME;
            VDEC_SendEs(pVDec_Pes_Info);
        }
        else
        {
            VDEC_SendEs(pVDec_Pes_Info);
        }


        while(1)
        {
            VDEC_QueryInfo(pVDec_Pes_Info->ucEsId, &rHdrInfo);
            if (rHdrInfo.u4CurEsCnt > (MPV_MAX_PES_NS_PER_QUEUE / 2))
            {
                x_thread_delay(30);
            }
            else
            {
                break;
            }
        }

        u4FrameAddr+=u4FrameSize;
        u4SizeAddr+=4;
        u4FrameIndex++;
    }

    Printf("Vp6 send frame %d\n",u4FrameIndex);
    return;
}

#ifdef CC_VP8_EMULATION
static INT32 _VdecCmdWebPCfg(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value;

    if(i4Argc<5)
    {
      Printf("ColorMode:0-->AYCbCr, 1-->Y/CbCr\n");
      Printf("DecodeMode: 0-->McuRow, 1-->PicMode, 2-->Auto\n");
      Printf("TargetWidht,TargetHeight\n");
      Printf("TargetBuf[Optional]\n");
      Printf("WorkBufSize[Optional]\n");
      Printf("DecoutY[Optional]\n");
      Printf("DecoutC[Optional]\n");
      Printf("GldBuf[Optional]\n");
      return 0;
    }
    _rWebCfg.u4ColorType=StrToInt(szArgv[1]);
    _rWebCfg.u4DecMode=StrToInt(szArgv[2]);

    u4Value=StrToInt(szArgv[3]);
    if (u4Value)
    {
        _rWebCfg.u4TargW=u4Value;
    }

    u4Value=StrToInt(szArgv[4]);
    if (u4Value)
    {
        _rWebCfg.u4TargH=u4Value;
    }
        
    if(i4Argc>5)
    {
        u4Value=StrToInt(szArgv[5]);
        if (u4Value)
        {
            _rWebCfg.u4TarBuffer=u4Value;
        }
    }

    if(i4Argc>6)
    {
        u4Value=StrToInt(szArgv[6]);
        if (u4Value)
        {
            _rWebCfg.u4WorkBufSize=u4Value;
        }
    }

    if(i4Argc>7)
    {
        u4Value=StrToInt(szArgv[7]);
        if (u4Value)
        {
            _rWebCfg.u4DecOutYAddr=u4Value;
        }
    }

    if(i4Argc>8)
    {
        u4Value=StrToInt(szArgv[8]);
        if (u4Value)
        {
            _rWebCfg.u4DecOutCAddr=u4Value;
        }
    }

    if(i4Argc>9)
    {
        u4Value=StrToInt(szArgv[9]);
        if (u4Value)
        {
            _rWebCfg.u4GldAddr=u4Value;
        }
    }

    Printf("Color=%d,Decode=%d,TW=%d,TH=%d,TargBuf=0x%x,WorkbufSize=0x%x\n",_rWebCfg.u4ColorType, \
        _rWebCfg.u4DecMode,_rWebCfg.u4TargW,_rWebCfg.u4TargH,_rWebCfg.u4TarBuffer,_rWebCfg.u4WorkBufSize);
    Printf("DecoutY=0%x,DecoutC=0x%x,GoldBuf=0x%x\n",_rWebCfg.u4DecOutYAddr,_rWebCfg.u4DecOutCAddr,_rWebCfg.u4GldAddr);
    return 0;
}


static BOOL _VdecWebpCompareData(UINT32 u4GldAddr,UINT32 u4DataAddr,UINT32 u4Width, UINT32 u4Height,UINT32 u4Type)
{
#if 0
    UINT32 u4DataSize=0;
    UINT32 *puGldAddr=(UINT32 *)u4GldAddr;
    UINT32 *puDataAddr=(UINT32 *)u4DataAddr;
    UINT32 *puEndAddr=NULL;
    if(u4Type==0) // Y
    {
      u4DataSize= (((u4Width+15)/16)*((u4Height+15)/16))*256;     
    }
    else if(u4Type==1) //C
    {
        u4DataSize= (((u4Width+15)/16)*((u4Height+15)/16))*128;     
    }
    else if(u4Type==2)
    {
        u4DataSize= u4Width*u4Height*4;     
    }

    puEndAddr=u4GldAddr+u4DataSize;

    while(puGldAddr<puEndAddr)
    {
       if((*puGldAddr)!=(*puDataAddr))
       {
          Printf("Webp data compare %d error \n",u4Type);
          ASSERT(0);
          return FALSE;
       }
       puGldAddr++;
       puDataAddr++;
    }
    return TRUE;
#else

    if (u4Type == 0 || u4Type == 1)
    {
        #define WEBP_HW_BLK_W               16
        #define WEBP_GD_BLK_W               16
        #define WEBP_HW_BLK_W_LOG2          4
        #define WEBP_GD_BLK_W_LOG2          4

        UINT32 u4HwWidth, u4GdWidth;
        UINT32 x, y;
        UINT8 *pu1HwAddr, *pu1GdAddr;
        BOOL  fgSame;

        u4HwWidth = u4Width;
        if(u4Width % WEBP_HW_BLK_W) 
        {
            u4HwWidth = ((u4Width / WEBP_HW_BLK_W) + 1) * WEBP_HW_BLK_W;
        }

        u4GdWidth = u4Width;
        if(u4Width % WEBP_GD_BLK_W) 
        {
            u4GdWidth = ((u4Width / WEBP_GD_BLK_W) + 1) * WEBP_GD_BLK_W;
        }

        if (u4Type == 1)
        {
            u4Height /= 2;
        }

        for (y = 0; y < u4Height; y++)
        {
            for (x = 0; x < u4Width;)
            {
                if (u4Type == 0)
                {
                    pu1HwAddr = (UINT8*)u4CalculatePixelAddress_Y(u4DataAddr, x, y, u4HwWidth, TRUE, WEBP_HW_BLK_W_LOG2);
                    pu1GdAddr = (UINT8*)u4CalculatePixelAddress_Y(u4GldAddr, x, y, u4GdWidth, TRUE, WEBP_GD_BLK_W_LOG2);
                }
                else
                {
                    pu1HwAddr = (UINT8*)u4CalculatePixelAddress_C(u4DataAddr, x, y, u4HwWidth, TRUE, TRUE, WEBP_HW_BLK_W_LOG2);
                    pu1GdAddr = (UINT8*)u4CalculatePixelAddress_C(u4GldAddr, x, y, u4GdWidth, TRUE, TRUE, WEBP_GD_BLK_W_LOG2);
                }
                
                if((x+4<=u4Width)&& ((((UINT32)pu1GdAddr)&0x3)==0) && ((((UINT32)pu1HwAddr)&0x3)==0))
                {
                    fgSame= (*((UINT32 *)pu1GdAddr) == *((UINT32 *)pu1HwAddr)) ? TRUE : FALSE;
                    x+=4;
                }
                else
                {
                    fgSame= (*pu1GdAddr== *pu1HwAddr) ? TRUE : FALSE;
                    x++;
                }

                if(!fgSame)
                {                      
                    Printf("Webp data compare %d error \n",u4Type);
                    ASSERT(0);
                    return FALSE;
                }
            }
        }
    }
    else
    {
        UINT32 u4DataSize=0;
        UINT32 *puGldAddr=(UINT32 *)u4GldAddr;
        UINT32 *puDataAddr=(UINT32 *)u4DataAddr;
        UINT32 *puEndAddr=NULL;
        if(u4Type==0) // Y
        {
          u4DataSize= (((u4Width+15)/16)*((u4Height+15)/16))*256;     
        }
        else if(u4Type==1) //C
        {
            u4DataSize= (((u4Width+15)/16)*((u4Height+15)/16))*128;     
        }
        else if(u4Type==2)
        {
            u4DataSize= u4Width*u4Height*4;     
        }
        
        puEndAddr=u4GldAddr+u4DataSize;
        
        while(puGldAddr<puEndAddr)
        {
           if((*puGldAddr)!=(*puDataAddr))
           {
              Printf("Webp data compare %d error \n",u4Type);
              ASSERT(0);
              return FALSE;
           }
           puGldAddr++;
           puDataAddr++;
        }
    }
    
    return TRUE;
#endif
}


void _VdecWebPDecode(UINT32 u4StartAddr)
{
   WEBP_DECIN_PARAM_T rInParam;
   WEBP_DECOUT_PARAM_T rOutParam;
   WEBP_CFG_T *prWebCfg=&_rWebCfg;
   UINT32 u4FrameSize=0,u4SaveSize=0;
   static UINT32 u4LoadIndex=0;
   UCHAR *puData=(UCHAR *)u4StartAddr;
   prWebCfg->u4SrcBuffer=u4StartAddr;
   u4FrameSize =  puData[32]+(puData[33]<<8)+(puData[34]<<16)+(puData[35]<<24);
   x_memset(&rInParam,0,sizeof(WEBP_DECIN_PARAM_T));
   x_memset(&rOutParam,0,sizeof(WEBP_DECOUT_PARAM_T));
   
   rInParam.u4Tag=(UINT32)prWebCfg;
   rInParam.u4DestBuffer=prWebCfg->u4TarBuffer;
   rInParam.eColorMode=prWebCfg->u4ColorType;
   rInParam.u4StartX=0;
   rInParam.u4StartY=0;
   rInParam.u4Width=prWebCfg->u4TargW;
   rInParam.u4Height=prWebCfg->u4TargH;
   rInParam.u4FileOffset=0x2C;
   rInParam.u4FileSize=u4FrameSize+rInParam.u4FileOffset;
   rInParam.u4Pitch=((rInParam.u4Width+15)/16)*16;
   rInParam.u4NotifyInfo=0;
#ifdef CC_WEBP_EMULATION
   rInParam.u4DecOutY=prWebCfg->u4DecOutYAddr;
   rInParam.u4DecOutC=prWebCfg->u4DecOutCAddr;
#endif

   if(prWebCfg->u4ColorType==WEBP_COLOR_AYCBCR8888 || prWebCfg->u4ColorType==WEBP_COLOR_ARGB8888)
   {
     rInParam.u4Pitch=rInParam.u4Pitch<<2;
   }
   
   //_IMGRZ_MMU_int();

   {
     FBM_POOL_T* pFbmPool = NULL;
     pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG);
     if(pFbmPool==NULL)
     { 
        Printf("Can't get jpeg fbm type \n");
     }
     else
     { 
        Printf("WorkingAdd=0x%x,Size=0x%x,ChangeSize=0x%x\n",pFbmPool->u4Addr,pFbmPool->u4Size,_rWebCfg.u4WorkBufSize);
        if(_rWebCfg.u4WorkBufSize)
        {
           pFbmPool->u4Size=_rWebCfg.u4WorkBufSize;
        }
        _VDEC_Vp8WebPInit(0,_VdecWebPCallBack,pFbmPool->u4Addr,pFbmPool->u4Size);
     }
   }

   if(_VDEC_Vp8WebPDecStart(0,prWebCfg->u4DecMode,&rInParam,&rOutParam))
   {
      u4SaveSize=rInParam.u4Pitch*rOutParam.u4Height;
      if(prWebCfg->u4ColorType==WEBP_COLOR_YCSEP420 ||prWebCfg->u4ColorType== WEBP_COLOR_YCSEP422)
      {
	      sprintf(_acFileName, "D:/vp8_test/Webp/%s_%dx%d_%d.y.bin",_acFileDir[_u4CurFileIdx],prWebCfg->u4TargW,prWebCfg->u4TargH,u4LoadIndex);
          Printf("d.save.binary %s 0x%x++0x%x\n",_acFileName,prWebCfg->u4TarBuffer,u4SaveSize-1);
          _SaveFile(_acFileName,prWebCfg->u4TarBuffer,u4SaveSize);
	      sprintf(_acFileName, "D:/vp8_test/Webp/%s_%dx%d_%d.c.bin",_acFileDir[_u4CurFileIdx],prWebCfg->u4TargW,prWebCfg->u4TargH,u4LoadIndex);
		  if(prWebCfg->u4ColorType==WEBP_COLOR_YCSEP420)
		  {
			  Printf("d.save.binary %s 0x%x++0x%x\n",_acFileName,prWebCfg->u4TarBuffer+rOutParam.u4CbCrOffset,(u4SaveSize>>1)-1);
		  }
		  else
		  {
			  Printf("d.save.binary %s 0x%x++0x%x\n",_acFileName,prWebCfg->u4TarBuffer+rOutParam.u4CbCrOffset,u4SaveSize-1);
		  }
          _SaveFile(_acFileName,prWebCfg->u4TarBuffer+rOutParam.u4CbCrOffset,u4SaveSize>>1);
          u4LoadIndex++;
      }
	  else
	  {
#ifdef CC_WEBP_EMULATION

          if(_fgEnableCRC)
          {
              UINT32 *u4CrcRptr=(UINT32 *)(_pcChkSum);
              x_memset(_acChkSumFile,0,MAX_FILENAME_LEN);
              Printf("VP8 loading CRC file:%s\\%s\crc.bin\n",_acFileDir[1],_acFileDir[_u4CurFileIdx]);
              sprintf(_acFileName, "%s/%s/crc.bin", _acFileDir[1],_acFileDir[_u4CurFileIdx]);
              HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
              _LoadSaveFile(_acFileName,TRUE,(UINT32)_pcChkSum,0);
              HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);

            if(rOutParam.u4Ycrc[0]==u4CrcRptr[0] && rOutParam.u4Ycrc[1]==u4CrcRptr[1]
                       &&rOutParam.u4Ycrc[2]==u4CrcRptr[2] && rOutParam.u4Ycrc[3]==u4CrcRptr[3]
                       &&rOutParam.u4Ccrc[0]==u4CrcRptr[4]&&rOutParam.u4Ccrc[1]==u4CrcRptr[5]
                       &&rOutParam.u4Ccrc[2]==u4CrcRptr[6]&&rOutParam.u4Ccrc[3]==u4CrcRptr[7])
            {
                    Printf("Compare frame in CRC ok!!\n");
            }
            else
            {
               LOG(0, "%s\\%s CRC check Fail in frame\n",_acFileDir[0],_acFileDir[_u4CurFileIdx]);
               LOG(0, "GoldenCrc:\n0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[0],u4CrcRptr[1],u4CrcRptr[2],u4CrcRptr[3]);
               LOG(0, "0x%x,0x%x,0x%x,0x%x\n",u4CrcRptr[4],u4CrcRptr[5],u4CrcRptr[6],u4CrcRptr[7]);
               LOG(0, "FrameCrc:\n0x%x,0x%x,0x%x,0x%x\n",rOutParam.u4Ycrc[0],rOutParam.u4Ycrc[1],rOutParam.u4Ycrc[2],rOutParam.u4Ycrc[3]);
               LOG(0, "0x%x,0x%x,0x%x,0x%x\n",rOutParam.u4Ccrc[0],rOutParam.u4Ccrc[1],rOutParam.u4Ccrc[2],rOutParam.u4Ccrc[3]);
               ASSERT(0);
            }
/*
            if(prWebCfg->u4GldAddr)
            {
                UINT32 u4DatSize=0;
                sprintf(_acFileName, "%s/%s/golden%dX%d_8888", _acFileDir[1],_acFileDir[_u4CurFileIdx],rInParam.u4Width,rInParam.u4Height);
                _LoadSaveFile(_acFileName,TRUE,prWebCfg->u4GldAddr,0);
                if(_VdecWebpCompareData(prWebCfg->u4GldAddr,rInParam.u4DestBuffer,rInParam.u4Width,rInParam.u4Height,2)==FALSE)
                {    
                    Printf("Resize data compile fail!!!\n");
                    u4DatSize=rInParam.u4Pitch*rInParam.u4Height;
                    sprintf(_acFileName, "D:/vp8_test/Webp/ErrDump/%s_golden%dX%d_8888.dat",_acFileDir[_u4CurFileIdx],rInParam.u4Width,rInParam.u4Height);
                    _SaveFile(_acFileName,rInParam.u4DestBuffer,u4DatSize);
                    sprintf(_acFileName, "D:/vp8_test/Webp/ErrDump/%s_regester.bin",_acFileDir[_u4CurFileIdx]);
                    _SaveFile(_acFileName,0xf0020000,0x200);
                    ASSERT(0);
                }
            }
            else
*/
            {
                UINT32 u4DatSize=0;
                Printf("VP8 loading CRC file:%s\\%s\rz_crc.bin\n",_acFileDir[1],_acFileDir[_u4CurFileIdx]);
                sprintf(_acFileName, "%s/%s/rz_crc.bin", _acFileDir[1],_acFileDir[_u4CurFileIdx]);
                HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
                _LoadSaveFile(_acFileName,TRUE,(UINT32)_pcChkSum,0);
                HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
                if(u4CrcRptr[0]!=rOutParam.u4RzCrc)
                {
                   Printf("Resize data crc compare faile!!\n");
                   Printf("Goden:0x%x,RzOut=0x%x\n",u4CrcRptr[0],rOutParam.u4RzCrc);
                   u4DatSize=rInParam.u4Pitch*rInParam.u4Height;
                   sprintf(_acFileName, "D:/vp8_test/Webp/ErrDump/%s_golden%dX%d_8888.dat",_acFileDir[_u4CurFileIdx],rInParam.u4Width,rInParam.u4Height);
                   _SaveFile(_acFileName,rInParam.u4DestBuffer,u4DatSize);
                   sprintf(_acFileName, "D:/vp8_test/Webp/ErrDump/%s_regester.bin",_acFileDir[_u4CurFileIdx]);
                   _SaveFile(_acFileName,0xf0020000,0x200);
                   ASSERT(0);
                }
            }
            return;
          }
          else if(_uGenCrcEn)
          {
             UINT32 u4RzAddr=0,u4RzSize=0;
             IMGRZ_RegInfo(&u4RzAddr,&u4RzSize);
             sprintf(_acChkSumFile, "%s/%s/rz_param.bin", _acFileDir[1],_acFileDir[_u4CurFileIdx]);
             _LoadSaveFile(_acChkSumFile,FALSE,(UINT32)u4RzAddr,u4RzSize);
             _pcChkSum[0]=rOutParam.u4PPType+'0';
             sprintf(_acFileName, "%s/%s/pp_type.txt", _acFileDir[1],_acFileDir[_u4CurFileIdx]);
             _LoadSaveFile(_acFileName,FALSE,_pcChkSum,1);
             *((UINT32 *)_pcChkSum)=rOutParam.u4RzCrc;
             sprintf(_acFileName, "%s/%s/rz_crc.bin", _acFileDir[1],_acFileDir[_u4CurFileIdx]);
             HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
             _LoadSaveFile(_acFileName,FALSE,_pcChkSum,4);
             return ;
          }
          else
          {
              if((rInParam.u4DecOutY)&&(rInParam.u4DecOutC)&&(rOutParam.eDecMode==WEBP_DEC_MODE_MCUROW))
              {
                  UINT32 u4MBWidht=0,u4MBHeight=0,u4DatSize=0;
                  u4MBWidht=((rOutParam.u4SrcWidht+15)/16);
                  u4MBHeight=((rOutParam.u4SrcHeight+15)/16);

                  if(prWebCfg->u4GldAddr)
                  {
                      sprintf(_acFileName, "%s/%s/post_0.y.dat", _acFileDir[1],_acFileDir[_u4CurFileIdx]);
                      _LoadSaveFile(_acFileName,TRUE,prWebCfg->u4GldAddr,0);
                      
                      if(_VdecWebpCompareData(prWebCfg->u4GldAddr,rInParam.u4DecOutY,rOutParam.u4SrcWidht,rOutParam.u4SrcHeight,0)==FALSE)
                      {    
                          u4DatSize=u4MBWidht*u4MBHeight*256;
                          sprintf(_acFileName, "D:/vp8_test/Webp/ErrDump/%s.y.dat",_acFileDir[_u4CurFileIdx]);
                          _SaveFile(_acFileName,rInParam.u4DecOutY,u4DatSize);
                          ASSERT(0);
                      }
                      
                      sprintf(_acFileName, "%s/%s/post_0.c.dat", _acFileDir[1],_acFileDir[_u4CurFileIdx]);
                      _LoadSaveFile(_acFileName,TRUE,prWebCfg->u4GldAddr,0);
                      if(_VdecWebpCompareData(prWebCfg->u4GldAddr,rInParam.u4DecOutC,rOutParam.u4SrcWidht,rOutParam.u4SrcHeight,1)==FALSE)
                      {    
                         u4DatSize=u4MBWidht*u4MBHeight*128;
                         sprintf(_acFileName, "D:/vp8_test/Webp/ErrDump/%s.c.dat",_acFileDir[_u4CurFileIdx]);
                         _SaveFile(_acFileName,rInParam.u4DecOutC,u4DatSize);
                         ASSERT(0);
                      }
                  }
                  else
                  {
                      u4DatSize=u4MBWidht*u4MBHeight*256;
                      sprintf(_acFileName, "D:/vp8_test/Webp/Dump/%s.y.dat",_acFileDir[_u4CurFileIdx]);
                      _SaveFile(_acFileName,rInParam.u4DecOutY,u4DatSize);
                      
                      u4DatSize=u4MBWidht*u4MBHeight*128;
                      sprintf(_acFileName, "D:/vp8_test/Webp/Dump/%s.c.dat",_acFileDir[_u4CurFileIdx]);
                      _SaveFile(_acFileName,rInParam.u4DecOutC,u4DatSize);
                  }
              }

              if(prWebCfg->u4GldAddr)
              {
                  UINT32 u4DatSize=0;
                  sprintf(_acFileName, "%s/%s/golden%dX%d_8888", _acFileDir[1],_acFileDir[_u4CurFileIdx],rInParam.u4Width,rInParam.u4Height);
                  _LoadSaveFile(_acFileName,TRUE,prWebCfg->u4GldAddr,0);
                  if(_VdecWebpCompareData(prWebCfg->u4GldAddr,rInParam.u4DestBuffer,rInParam.u4Width,rInParam.u4Height,2)==FALSE)
                  {    
                      u4DatSize=rInParam.u4Pitch*rInParam.u4Height;
                      sprintf(_acFileName, "D:/vp8_test/Webp/ErrDump/%s_golden%dX%d_8888.dat",_acFileDir[_u4CurFileIdx],rInParam.u4Width,rInParam.u4Height);
                      _SaveFile(_acFileName,rInParam.u4DestBuffer,u4DatSize);
                      ASSERT(0);
                  }
              }
          }
         
#else
          sprintf(_acFileName, "D:/vp8_test/Webp/%s_%dx%d.aycbcr.bin",_acFileDir[_u4CurFileIdx],prWebCfg->u4TargW,prWebCfg->u4TargH);
          Printf("d.save.binary %s 0x%x++0x%x\n",_acFileName,prWebCfg->u4TarBuffer,u4SaveSize-1);
          _SaveFile(_acFileName,prWebCfg->u4TarBuffer,u4SaveSize);
#endif
      }
   }
   else
   {
      Printf("#####################WebP decode fail############## \n");
      Printf("#####################WebP decode fail############## \n");
      Printf("#####################WebP decode fail############## \n");
      //ASSERT(0);
   }
   return;
}

void _VdecVParserVP8(UINT32 u4StartAddr)
{
    VDEC_PES_INFO_T rPesInfo;
    VDEC_HDR_INFO_T rHdrInfo;
    UINT32 u4FrameNo = 0;//*((UINT32*)(u4StartAddr + 24));
    UINT32 u4FrameAddr = 0,u4FrameIndex = 0, u4FrameSize = 0;
    UCHAR *puData=(UCHAR *)u4StartAddr;

    x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.u4VideoType  = DMX_VIDEO_VP8;
    rPesInfo.u4FifoStart  = u4StartAddr;
    rPesInfo.u4FifoEnd    = VFIFO_E;
    rPesInfo.fgDtsValid   = TRUE;
    rPesInfo.ucPicType    = 1;
    u4FrameNo=puData[24]+ (puData[25]<<8)+(puData[26]<<16)+(puData[27]<<24);
    u4FrameAddr = (UINT32)(puData + 32);
    do
    {
        puData=(UCHAR *)u4FrameAddr;
        u4FrameSize =  puData[0]+(puData[1]<<8)+(puData[2]<<16)+(puData[3]<<24);
        if(u4FrameSize == 0)
        {
            break;
        }

        u4FrameAddr += 12;

        if(u4FrameIndex>u4EndFrameIndex)
        {
            break;
        }
        else if(u4FrameIndex+_u4CurFrmIdx>END_COMPARE_FRAME)
        {
            break;
        }
        else //if (u4FrameIndex >= _u4StartFameIndex)
        {
            rPesInfo.u4VldReadPtr   = u4FrameAddr;
            rPesInfo.u4PTS          = u4FrameIndex * 9000;
            rPesInfo.u4DmxFrameType = u4FrameSize;
            
            VDEC_SendEs(&rPesInfo);

            VDEC_QueryInfo(rPesInfo.ucEsId, &rHdrInfo);
            if (rHdrInfo.u4CurEsCnt > (MPV_MAX_PES_NS_PER_QUEUE / 2))
            {
                x_thread_delay(100);
            }
        }

        u4FrameAddr += u4FrameSize;
        u4FrameIndex++;
    } while( u4FrameIndex < u4FrameNo);

    Printf("Vp8 send frame %d\n",u4FrameIndex);
}


static void _VdecSendVP8Pattern(void* pvArg)
{
    VDEC_PES_INFO_T rPesInfo;
    CHAR* pcToken;
    UINT32 u4BitStreamStartPos,u4StreamNum=0,u4Suffix=(UINT32)-1;
    UINT16 u2file_num=0;
    UINT16 index=0,u2SetIndex=0,u4StreamIndex=0,u4LastFileIndex=0;    
    UCHAR acPath[MAX_FILENAME_LEN];            
    UCHAR acNum[16];
    UCHAR *pattern_path=NULL;
    UCHAR *golden_path_arg=NULL;
    BOOL fgStarted=FALSE;

    //extern void _IMGRZ_MMU_int(void);
    do
    {
        for(u2SetIndex=0;u2SetIndex<VDEC_VERIFY_SET_NUM;u2SetIndex++)
        {
            if(!rVerifySet[u2SetIndex].u4Enable)
            {
                continue;
            }
            
            _u4CrcIndex=0;
            u2file_num=0; 
            index=0;
            pattern_path=rVerifySet[u2SetIndex].rPatternFile;
            golden_path_arg=rVerifySet[u2SetIndex].rGoldenFile;
            x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);
            x_memset((void*)_acFileDir, 0, sizeof(_acFileDir));
            HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo,FILE_INFO_SIZE);
            
            //load pattern file to dram
            Printf("====StartSet %d=====\n",u2SetIndex);
            Printf("d.load.binary \"%s\" 0x%08x /ny\n",pattern_path, _pcFileInfo);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pattern_path, _pcFileInfo));

            pcToken = strtok(_pcFileInfo, "\r\n");
            while( pcToken != NULL) 
            { 
                strcpy(_acFileDir[u2file_num], pcToken);
                pcToken = strtok(NULL, "\r\n");
                u2file_num++;
            }
            ASSERT(u2file_num>2);

            if(rVerifySet[u2SetIndex].u4StartFile<u2file_num)
            {
                _u4CurFileIdx=rVerifySet[u2SetIndex].u4StartFile+2;
            }
            else
            {
                _u4CurFileIdx=2;
            }

            fgStarted=TRUE;
            for(index=2;index<u2file_num;index++)
            {
              if(_acFileDir[index][0]=='#'&&_acFileDir[index][1]=='@'&&_acFileDir[index][2]=='#')
              {
                  fgStarted=TRUE;
              }

              if(_acFileDir[index][0]=='#'&&_acFileDir[index][1]=='&'&&_acFileDir[index][2]=='#')
              {
                  fgStarted=FALSE;
              }
              if(fgStarted&&_acFileDir[index][0]!='#')
              {
                 u4StreamNum++;
              }
            }

            fgStarted=TRUE;
            for(; _u4CurFileIdx < u2file_num ; _u4CurFileIdx++)
            {
                _u4CurFrmIdx =0;
                _u4StartFameIndex=0;
                u4EndFrameIndex=(UINT32)-1;
                u4Suffix=(UINT32)-1;
                x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

                // Start
                if(_acFileDir[_u4CurFileIdx][0] == '#' 
                    && _acFileDir[_u4CurFileIdx][1] == '@'
                    &&_acFileDir[_u4CurFileIdx][2] == '#')
                {
                   
                   fgStarted=TRUE;
                   continue;
                }

                if(_acFileDir[_u4CurFileIdx][0] == '#' 
                    && _acFileDir[_u4CurFileIdx][1] == '&'
                    &&_acFileDir[_u4CurFileIdx][2] == '#')
                {
                   fgStarted=FALSE;
                   continue;
                }
                
                if(!fgStarted)
                {
                   continue;
                }
                
                if(_acFileDir[_u4CurFileIdx][0] == '#' && _acFileDir[_u4CurFileIdx][1] != '$')
                {
                   continue;    //skip the pattern
                }

                if(_acFileDir[_u4CurFileIdx][0] == '#' && _acFileDir[_u4CurFileIdx][1]=='$'
                   && _acFileDir[_u4CurFileIdx][2]=='0' )
                {
                    x_memset(_acFileDir[0],0,MAX_FILENAME_LEN);
                    x_memcpy(_acFileDir[0],_acFileDir[_u4CurFileIdx]+3,strlen(_acFileDir[_u4CurFileIdx]+3));
                    continue;    //skip the pattern
                }

                if(_acFileDir[_u4CurFileIdx][0] == '#' && _acFileDir[_u4CurFileIdx][1]=='$'
                   && _acFileDir[_u4CurFileIdx][2]=='1' )
                {
                    x_memset(_acFileDir[1],0,MAX_FILENAME_LEN);
                    x_memcpy(_acFileDir[1],_acFileDir[_u4CurFileIdx]+3,strlen(_acFileDir[_u4CurFileIdx]+3));
                    continue;    //skip the pattern
                }
				
                if(_acFileDir[_u4CurFileIdx][0] == '#' && _acFileDir[_u4CurFileIdx][1]=='$'
                   && _acFileDir[_u4CurFileIdx][2]=='>' ) //cli
                {
                    Printf("CLI:%s\n",_acFileDir[_u4CurFileIdx]+3);
					CLI_Parser(_acFileDir[_u4CurFileIdx]+3);
					x_thread_delay(1000);
                    continue;    //skip the pattern
                }

                u4BitStreamStartPos=VFIFO_S;
                //u4BitStreamStartPos=u4BitStreamStartPos-(u4BitStreamStartPos&(0x10000-1))-0x100000;
                //HalFlushInvalidateDCacheMultipleLine((UINT32)u4BitStreamStartPos,VFIFO_E-VFIFO_S);
                u4StreamIndex++;
                Printf("Progress:(%d/%d)\n",u4StreamIndex,u4StreamNum);
                x_memset(acPath, 0, MAX_FILENAME_LEN);
                x_memset(acNum, 48, 16);
                pcToken=_acFileDir[_u4CurFileIdx];
                
                while(*pcToken)
                {

                   if(pcToken[0]=='/'&& pcToken[1]=='/')
                   {
                       pcToken[0]=0;
                       pcToken[1]=0;
                       break;
                   }
                   
                   if(pcToken[0]=='#'&& pcToken[1]=='%')
                   {
                      UINT32 u4NumIndex=0;
                      pcToken[0]=0;
                      pcToken[1]=0;
                      pcToken+=2;
                      while(*pcToken>47 && *pcToken<58)
                      {
                         acNum[u4NumIndex++]=*pcToken++;
                      }
                      acNum[u4NumIndex]=0;
                      u4Suffix=StrToInt(acNum);
                      x_memset(acNum, 48, 16);
                   }
                   
                   if(pcToken[0]=='#'&& pcToken[1]=='@')
                   {
                      UINT32 u4NumIndex=0;
                      pcToken[0]=0;
                      pcToken[1]=0;
                      pcToken+=2;
                      while(*pcToken>47 && *pcToken<58)
                      {
                         acNum[u4NumIndex++]=*pcToken++;
                      }
                      acNum[u4NumIndex]=0;
                      _u4CurFrmIdx=StrToInt(acNum);
                      x_memset(acNum, 48, 16);
                   }

                   if(pcToken[0]=='#'&& pcToken[1]=='&')
                   {
                      UINT32 u4NumIndex=0;
                      pcToken[0]=0;
                      pcToken[1]=0;
                      pcToken+=2;
                      while(*pcToken>47 && *pcToken<58)
                      {
                         acNum[u4NumIndex++]=*pcToken++;
                      }
                      acNum[u4NumIndex]=0;
                      _u4StartFameIndex=StrToInt(acNum);
                      x_memset(acNum, 48, 16);
                   }

                   if(pcToken[0]=='#'&& pcToken[1]=='*')
                   {
                      UINT32 u4NumIndex=0;
                      pcToken[0]=0;
                      pcToken[1]=0;
                      pcToken+=2;
                      while(*pcToken>47 && *pcToken<58)
                      {
                         acNum[u4NumIndex++]=*pcToken++;
                      }
                      acNum[u4NumIndex]=0;
                      u4EndFrameIndex=StrToInt(acNum);
                      x_memset(acNum, 48, 16);
                   }
                   
                   pcToken++;
                }
                
                pcToken=_acFileDir[_u4CurFileIdx];
                while(*pcToken)pcToken++;
                while(*(--pcToken)==32) *pcToken=0;
                
                _u4CurFrmIdx+=_u4StartFameIndex;
                if(_u4CurFrmIdx>END_COMPARE_FRAME)
                {
                   continue;
                }
                
                Printf("StartFrameIndex=%d,EndIndex=%d,GoldenIndex=%d\n",_u4StartFameIndex,u4EndFrameIndex,_u4CurFrmIdx);

                if(_eFmt!=VDEC_FMT_WEBP)
                {
                    if(_uGenCrcEn==1)
                    {
                        if((!_fgEnableCRC)&& u4StreamIndex>1)
                        {
                            if(x_strcmp(_acFileDir[u4LastFileIndex],_acFileDir[_u4CurFileIdx])!=0)
                            {
                               sprintf(_acFileName, "%s/%s/pp_type.txt", _acFileDir[1],_acFileDir[u4LastFileIndex]);
                               _LoadSaveFile(_acFileName,FALSE,_pcChkSum,_u4CrcIndex);
                               _u4CrcIndex=0;             
                            }
                        }

                    }
                    else if(_uGenCrcEn==2)
                    {
                        if((!_fgEnableCRC)&& u4StreamIndex>1)
                        {
                            if(x_strcmp(_acFileDir[u4LastFileIndex],_acFileDir[_u4CurFileIdx])!=0)
                            {
                               sprintf(_acFileName, "%s/%s/crc.bin", _acFileDir[1],_acFileDir[u4LastFileIndex]);
                               _LoadSaveFile(_acFileName,FALSE,_pcChkSum,_u4CrcIndex*32);
                               _u4CrcIndex=0;             
                            }
                        }
                    }
                }
                
                if(u4Suffix==(UINT32)-1)
                {
                sprintf(acPath, "%s/%s.ivf", _acFileDir[0],_acFileDir[_u4CurFileIdx]);
                    
                }
                else
                {
                    sprintf(acPath, "%s/%s %d.ivf", _acFileDir[0],_acFileDir[_u4CurFileIdx],u4Suffix);
                }
                
                _LoadSaveFile(acPath,TRUE,u4BitStreamStartPos,0);
                HalFlushInvalidateDCacheMultipleLine((UINT32)u4BitStreamStartPos,VFIFO_E-VFIFO_S);
                if(_eFmt!=VDEC_FMT_WEBP)
                {

                if(_fgEnableCRC)
                {
                     if(x_strcmp(_acFileDir[u4LastFileIndex],_acFileDir[_u4CurFileIdx])!=0)
                     {
                        _u4CrcIndex=0;
                    x_memset(_acChkSumFile,0,MAX_FILENAME_LEN);
                        Printf("VP8 loading CRC file:%s\\%s\crc.bin\n",_acFileDir[1],_acFileDir[_u4CurFileIdx]);
                        sprintf(_acChkSumFile, "%s/%s/crc.bin", _acFileDir[1],_acFileDir[_u4CurFileIdx]);

                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
                        _LoadSaveFile(_acChkSumFile,TRUE,(UINT32)_pcChkSum,0);
                    HalFlushInvalidateDCacheMultipleLine((UINT32)_pcChkSum, CHKSUM_INFO_SIZE);
                }
                  }
                  u4LastFileIndex=_u4CurFileIdx;


                _VdecVParserVP8(u4BitStreamStartPos);

                rPesInfo.u4VideoType=DMX_VIDEO_VP8;
                rPesInfo.u4VldReadPtr = u4BitStreamStartPos;
                rPesInfo.u4FifoStart = u4BitStreamStartPos;
                rPesInfo.u4FifoEnd = VFIFO_E;
                rPesInfo.fgDtsValid=TRUE;
                rPesInfo.ucPicType = 255;
                VDEC_SendEs((void*)&rPesInfo);
                  u4DumpIndex=(UINT32)-1;
                //send eos to push last frame to decoder
                rPesInfo.fgEos = TRUE;
                VDEC_SendEs((void*)&rPesInfo);
                VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
                VDEC_Play(0, VDEC_FMT_VP8);
            }
		      else
		      {
                  _VdecWebPDecode((UCHAR *)u4BitStreamStartPos);
			  }
				
            }

        }

        if(_i4LoopCnt > 0)
        {
            _i4LoopCnt--;
        }
        else
        {
            _i4LoopCnt = -1; 
        }
        if(_i4LoopCnt == 0)
        {
            break;
        }
    }while(1);
}

static INT32 _VdecCmdVp8DumpReg(INT32 i4Argc, const CHAR ** szArgv)
{
    extern void _VDEC_Vp8DumpReg(void);
    
    _VDEC_Vp8DumpReg();
}
#endif // CC_VP8_EMULATION

static UINT32 _VdecVP6ReadLine(char* pIn, char* pOut)
{
    UINT32 u4Cnt = 0;

    while (pIn[u4Cnt])
    {
        pOut[u4Cnt] = pIn[u4Cnt];
        if (pIn[u4Cnt++] == '\n')
        {
            break;
        }
    }

    pOut[u4Cnt] = 0;
    return u4Cnt;
}

static void _VdecSendVP6Pattern(void* pvArg)
{
    VDEC_PES_INFO_T rPesInfo;
    CHAR* pcToken;
    UINT32 u4BitStreamStartPos;
    UINT16 u2SetIndex=0;
    UCHAR *pattern_path = NULL;
    UINT32 i, u4Ofst, u4Cnt;
    char  acLine[256];

#if defined(VP6_MID_GOLDEN) || defined(VP6_MID_VFIFO)
    MID_Init();
    MID_Reset();
#endif
    do
    {
        for(u2SetIndex=0;u2SetIndex<VDEC_VERIFY_SET_NUM;u2SetIndex++)
        {
            if(!rVerifySet[u2SetIndex].u4Enable)
            {
                continue;
            }
            u4Ofst = 0;
            pattern_path=rVerifySet[u2SetIndex].rPatternFile;

            x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);
            HalFlushInvalidateDCacheMultipleLine((UINT32)_pcFileInfo,FILE_INFO_SIZE);

            LOG(0,"====StartSet %d=====\n",u2SetIndex);
            LOG(0,"d.load.binary \"%s\" 0x%08x /ny\n",pattern_path, _pcFileInfo);
            UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pattern_path, _pcFileInfo));

            while (TRUE)
            {
                BOOL fgFileExist = FALSE;

                _u4StartFrame = 0;
                _u4WithDbk = FALSE;
                _u4VP6EnableAlpha = FALSE;
                _fgAbtCurItem = FALSE;
                _u4CurFrmIdx = 0;

                u4Cnt = _VdecVP6ReadLine(_pcFileInfo + u4Ofst, acLine);
                if (u4Cnt == 0)
                {
                    break;
                }
                u4Ofst += u4Cnt;

                pcToken = strtok(acLine, " \t\r\n");
                if (pcToken[0] == '#')
                {
                    continue;
                }

                while (pcToken != NULL)
                {
                    if (strncmp(pcToken, "[fmt]", 5) == 0)
                    {
                        if (strncmp(pcToken + 5, "adobe", 5) == 0)
                        {
                            _u4VP6DecodeMode = 1;
                        }
                        else
                        {
                            _u4VP6DecodeMode = 0;
                        }
                    }
                    else if (strncmp(pcToken, "[bs]", 4) == 0)
                    {
                        strcpy(_acFileDir[0], pcToken + 4);
                    }
                    else if (strncmp(pcToken, "[gp]", 4) == 0)
                    {
                        strcpy(_acFileDir[1], pcToken + 4);
                    }
                    else if (strncmp(pcToken, "[frm]", 5) == 0)
                    {
                        _u4StartFrame = StrToInt(pcToken + 5);
                    }
                    else if (strncmp(pcToken, "[dbk]", 5) == 0)
                    {
                        _u4WithDbk = TRUE;
                    }
                    else if (strncmp(pcToken, "[alpha]", 7) == 0)
                    {
                        _u4VP6EnableAlpha = TRUE;
                    }
                    else if (pcToken[0] == '#')
                    {
                        break;
                    }
                    else
                    {
                        fgFileExist = TRUE;
                        strcpy(_acFileDir[2], pcToken);
                    }

                    pcToken = strtok(NULL, " \t\r\n");
                }

                if (!fgFileExist)
                {
                    continue;
                }

                VDEC_SetParam(ES0, VP6_SET_DEC_MODE, _u4VP6DecodeMode, 0, 0);
                
                // start process this file
                sprintf(_acFileName, "%s/%s.vdo", _acFileDir[0], _acFileDir[2]);
                Printf("Progress:%s\n", _acFileName);

                u4BitStreamStartPos=_u4BstmSa;

#ifdef VP6_MID_VFIFO
                MID_RegisterHandler(_VP6_IntrudeHandler);               
                MID_ResetRegion(1);
                MID_SetRegionEx2(1, MID_AGENT_CPU, _u4BstmSa, _u4BstmSa + _u4BstmSz, 0, 0);
                MID_EnableRegionProtect(1);
#endif
                _LoadFile(_acFileName,u4BitStreamStartPos);
                HalFlushInvalidateDCacheMultipleLine((UINT32)u4BitStreamStartPos,_u4BstmSz);

#ifdef VP6_MID_VFIFO
                MID_ResetRegion(1);
                MID_SetRegionEx2(1, MID_AGENT_NONE, _u4BstmSa, _u4BstmSa + _u4BstmSz, 0, 0);
                MID_EnableRegionProtect(1);
#endif
                x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
                rPesInfo.u4VideoType  = DMX_VIDEO_VP6;
                rPesInfo.u4VldReadPtr = u4BitStreamStartPos;
                rPesInfo.u4FifoStart  = u4BitStreamStartPos;
                rPesInfo.u4FifoEnd    = _u4BstmSa + _u4BstmSz;
                rPesInfo.fgDtsValid   = TRUE;
                _VdecVParserVP6(&rPesInfo);
                rPesInfo.ucPicType = 255;
                VDEC_SendEs((void*)&rPesInfo);

                //send eos to push last frame to decoder
                rPesInfo.fgEos = TRUE;
                VDEC_SendEs((void*)&rPesInfo);
                VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
                VDEC_Play(0, VDEC_FMT_VP6);
            }
        }

        if(_i4LoopCnt > 0)
        {
            _i4LoopCnt--;
        }
        else
        {
            _i4LoopCnt = -1; 
        }
        if(_i4LoopCnt == 0)
        {
            break;
        }
    }while(1);

    return;    
}


#if defined(CC_RV_EMULATION)

// =========== RV Emulation ===============

#define FRMINFO_BASE_DIR "r:/rm_emu/frminf"
#define GOLDEN_BASE_DIR  "r:/rm_emu/golden"
//#define EXELIST_ADDR _pcStartCodeFile
//#define FRMINFO_ADDR _pcFileInfo
//#define GOLD_ADDR _pu1GoldenData
//#define CRC_ADDR _pcChkSum
//#define VFIFO_ADDR _pu1VfifoSa
#define CRC_BASE                           (IO_VIRT + 0x03000)
#define RISCRead(addr)         (*(volatile UINT32 *)(addr))
#define PIXEL_ERR_LOG_CNT     10       // max number of log lines printed when pixel error
#define GOLD_SZ GOLDEN_SIZE

typedef enum
{
	RV_INTRAPIC,        /* 0 (00) */
	RV_FORCED_INTRAPIC, /* 1 (01) */
	RV_INTERPIC,        /* 2 (10) */
	RV_TRUEBPIC         /* 3 (11) */
} EnumRVPicCodType;

typedef struct
{
  BOOL   fgRV9;  // TRUE -> RV9  FALSE -> RV8
  BOOL   fgBwdIsI;
  EnumRVPicCodType ePtype;
  UINT32 u4OrgWidth;
  UINT32 u4OrgHeight;
  UINT32 u4Width;
  UINT32 u4Height;
  UINT32 u4LineSize;
  UINT32 u4PctszSize;
  UINT32 u4Pctsz;
  UINT32 u4Pquant;
  UINT32 u4Oquant;
  UINT32 u4DFP;
  UINT32 u4Tr;
  UINT32 u4MbaSize;
  UINT32 u4Mba;
  UINT32 u4Rtype;
  UINT32 u4Iratio;
  UINT32 u4HdrSkip;
  UINT32 u4NumSlice;
  UINT32 au4SliceSize[128];
  UINT32 u4BstLength;
} RvPicAttr;

static int _iCurEmuItem = 0;
static int _iTotEmuItem = 0;
static char *_aaRmEmuItem[1000];
char *_pcLoadedBitstream;
#ifdef CC_EMULATION_CRC
static UINT32 _u4CRCPtr;
#endif
static UINT32 _u4FrmInfoPtr;
static UINT32 _u4NumFrmToTest = (UINT32)-1;
static UINT32 _u4ErrPos = 0xFFFFFFFF;
static BOOL _fgShowDiff = TRUE;

static BOOL fgGetNextItem(char **ppcItem)
{
  if (_iCurEmuItem == _iTotEmuItem)
    return FALSE;
    
  *ppcItem = _aaRmEmuItem[_iCurEmuItem++];
  printf("Item to test: %s\n\n", *ppcItem);
  
  return TRUE;
}


static void vLoadExeList(char *szExelist)
{
  char *pcPtr, *pcStart;
  
  _iCurEmuItem = _iTotEmuItem = 0;
    
  UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08X /ny", szExelist, _pcStartCodeFile));
  
  for (pcPtr = pcStart = (char *)_pcStartCodeFile; *pcPtr != ';'; pcPtr++)
  {
    if (*pcPtr == 0x0D && *(pcPtr + 1) == 0x0A)
    {
      _aaRmEmuItem[_iTotEmuItem++] = pcStart;
      *pcPtr = 0;
      pcStart = pcPtr + 2;
      pcPtr++;
    }
    else if (*pcPtr == 0x0A)
    {
      _aaRmEmuItem[_iTotEmuItem++] = pcStart;
      *pcPtr = 0;
      pcStart = pcPtr + 1;
    }
  }
  
  printf("Execution list loaded.  Total %d items.\n\n", _iTotEmuItem);
}


static void vShowExeList(void)
{
  int i;
  
  for (i = 0; i < _iTotEmuItem; i++)
    printf("%s\n", _aaRmEmuItem[i]);
  
  _iCurEmuItem = 0;
  
  printf("\n");
}


static void vLoadFrmInfo(char *szDir)
{
    printf("d.load.binary \"%s/%s/frminfo.bin\" 0x%08X /ny\n", FRMINFO_BASE_DIR, szDir, _pcFileInfo);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/%s/frminfo.bin\" 0x%08X /ny", FRMINFO_BASE_DIR, szDir, _pcFileInfo));
}


#ifdef CC_EMULATION_CRC
static void vLoadCRC(char *szDir)
{
    printf("d.load.binary \"%s/%s/CrcResult.crc\" 0x%08X /ny\n", GOLDEN_BASE_DIR, szDir, _pcChkSum);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/%s/CrcResult.crc\" 0x%08X /ny", GOLDEN_BASE_DIR, szDir, _pcChkSum));
}

static BOOL _VdecRvEmuCompareCRC(VDEC_DISP_PIC_INFO_T *prDispInfo)
{
    UINT32 u4Val, u4CRC, i;

    u4Val = *(UINT32 *)(_pcChkSum + _u4CRCPtr) + 1;  // frame no
    _u4CRCPtr += 4;

    if (u4Val != prDispInfo->u4CurrentPicNo)
    {
        printf("!!! CRC frame number mismatch !!!\n");
        return FALSE;
    }

    for (i = 0; i < 4; i++)
    {
        u4CRC= RISCRead(CRC_BASE + (2+i)*4);
        u4Val = *(UINT32 *)(_pcChkSum + _u4CRCPtr);
        _u4CRCPtr += 4;
        if (u4Val != u4CRC)
        {
            printf("!!! CRC Y%d mismatch, %08X v.s %08X !!!\n", i, u4Val, u4CRC);
            ASSERT(0);
            return FALSE;
        }
    }

    for (i = 0; i < 4; i++)
    {
        u4CRC= RISCRead(CRC_BASE + (6+i)*4);
        u4Val = *(UINT32 *)(_pcChkSum + _u4CRCPtr);
        _u4CRCPtr += 4;
        if (u4Val != u4CRC)
        {
            printf("!!! CRC C%d mismatch, %08X v.s %08X !!!\n", i, u4Val, u4CRC);
            ASSERT(0);
            return FALSE;
        }
    }

    return TRUE;
}
#endif

static UINT32 mb_to_line_y(UINT32 buf, int width, int x, int y)
{
    UINT32 val = 0;
    int mb_x;
    int mb_y;
    int offset_x;
    int offset_y;
    int offset;

#ifdef DDR3_TEST
    mb_x = x >> 6;
#else
    mb_x = x >> 4;
#endif
    mb_y = y >> 4;

#ifdef DDR3_TEST
    offset_x = x % 64;
#else
    offset_x = x % 16;
#endif
    offset_y = y % 32;

#ifdef DDR3_TEST
    offset = offset_x + 
           (offset_y << 6) +
           (mb_x << 11) +
           (((mb_y >> 1) * width) << 5);
#else
    offset = offset_x + 
           (offset_y << 4) +
           (mb_x << 9) +
           (((mb_y >> 1) * width) << 5);
#endif

    val = *(UINT32 *)(buf + offset);
    _u4ErrPos = (UINT32)buf + offset;

    return val;
}

static void mb_to_line_c(UINT32 *pu4Val1, UINT32 *pu4Val2, UINT32 buf, int width, int x, int y)
{
    int mb_x;
    int mb_y;
    int offset_x;
    int offset_y;
    int offset;

#ifdef DDR3_TEST
    mb_x = x >> 5;
#else
    mb_x = x >> 3;
#endif
    mb_y = y >> 3;

#ifdef DDR3_TEST
    offset_x = x % 32;
#else
    offset_x = x % 8;
#endif
    offset_y = y % 16;

#ifdef DDR3_TEST
    offset = (offset_x << 1) + 
           (offset_y << 6) +
           (mb_x << 10) +
           (((mb_y >> 1) * width) << 4);
#else
    offset = (offset_x << 1) + 
           (offset_y << 4) +
           (mb_x << 8) +
           (((mb_y >> 1) * width) << 4);
#endif

    *pu4Val1 = *(UINT32 *)(buf + offset);
    *pu4Val2 = *(UINT32 *)(buf + offset + 4);
    _u4ErrPos = (UINT32)buf + offset;
}


static BOOL fgCompareGold(VDEC_DISP_PIC_INFO_T *prDispInfo)
{
    UINT32 n = prDispInfo->u4CurrentPicNo - 1;
    UINT32 u4DecWidth = prDispInfo->u4W_Cmp;
    UINT32 u4DecHeight = prDispInfo->u4H_Cmp;
    UINT32 u4Width = prDispInfo->u4W;
    UINT32 u4Height = prDispInfo->u4H;
    UINT32 u4OutBufY = prDispInfo->pvYAddr;
    UINT32 u4OutBufC = prDispInfo->pvCAddr;
    BOOL fgRet = TRUE;
    BOOL fgYOK = TRUE;
    BOOL fgUOK = TRUE;
    BOOL fgVOK = TRUE;
    int x, y;
    UINT32 u4ErrVal, u4ErrVal2;
    UINT32 u4GolVal, u4GolVal2;
    UINT32 u4UVal, u4VVal;
    UINT32 u4ErrCntY, u4ErrCntC;
    UINT32 u4Total;
    UINT32 u4GoldYSz, u4GoldYUSz;
#ifdef DDR3_TEST
    UINT32 u4DecWidthAlign;
    UINT32 u4WMBSize = u4DecWidth / 16;

    if (u4WMBSize % 4)
    {
        u4WMBSize = u4WMBSize + (4 - (u4WMBSize % 4));
    }

    u4DecWidthAlign = u4WMBSize * 16;
#endif

#ifdef VDEC_L2_CACHE
    HalFlushInvalidateDCacheMultipleLine(_pu1GoldenData, GOLD_SZ);
#else
    HalFlushInvalidateDCache();
#endif
    u4GoldYSz = u4DecWidth * u4DecHeight;
    u4GoldYUSz = u4GoldYSz + u4GoldYSz / 4;

CMP_BEGIN:
    fgYOK = fgUOK = fgVOK = TRUE;

    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/%s/g%07d.yuv\" 0x%08X /ny", GOLDEN_BASE_DIR, _pcLoadedBitstream, n, _pu1GoldenData));

    u4ErrCntY = u4Total = 0;
    for (y = 0; y < u4Height; y++)
    {
        for (x = 0; x < u4Width; x += 4)
        {
#ifdef DDR3_TEST
            u4ErrVal = mb_to_line_y(u4OutBufY, u4DecWidthAlign, x, y);
#else
            u4ErrVal = mb_to_line_y(u4OutBufY, u4DecWidth, x, y);
#endif

            u4GolVal = *(UINT32 *)(_pu1GoldenData + y * u4DecWidth + x);
            if (u4ErrVal != u4GolVal)
            {
                u4ErrCntY++;
                if (_fgShowDiff)
                {
                    if (u4ErrCntY == PIXEL_ERR_LOG_CNT + 1)
                    {
                        printf("!!! too many errors !!!\n");
                    }
                    else if (u4ErrCntY <= PIXEL_ERR_LOG_CNT)
                    {
                        printf("y(%4d,%4d) (m,p)(0x%08X,0x%08X) ", x, y, _u4ErrPos, y * u4DecWidth + x);
                        printf("(0x%08X,0x%08X)\n", u4ErrVal, u4GolVal);
                    }
                }
                fgYOK = FALSE;
            }
            u4Total += 4;
        }
    }

    if (!fgYOK)
        printf("Error count y: %d/%d\n", u4ErrCntY, u4Total);

    u4ErrCntC = 0;
    for (y = 0; y < u4Height/2; y++)
    {
        for (x = 0; x < u4Width/2; x += 4)
        {
            u4UVal = *(UINT32 *)(_pu1GoldenData + u4GoldYSz  + y * u4DecWidth / 2 + x);
            u4VVal = *(UINT32 *)(_pu1GoldenData + u4GoldYUSz + y * u4DecWidth / 2 + x);

#ifdef DDR3_TEST
            mb_to_line_c(&u4ErrVal, &u4ErrVal2, u4OutBufC, u4DecWidthAlign, x, y);
#else
            mb_to_line_c(&u4ErrVal, &u4ErrVal2, u4OutBufC, u4DecWidth, x, y);
#endif
            u4GolVal = ((u4VVal << 16) & 0xFF000000) | ((u4UVal << 8) & 0x00FF0000) |
            ((u4VVal <<  8) & 0x0000FF00) | ((u4UVal) & 0x000000FF);

            if (u4ErrVal != u4GolVal)
            {
                u4ErrCntC++;
                if (_fgShowDiff)
                {
                    if (u4ErrCntC == PIXEL_ERR_LOG_CNT + 1)
                    {
                        printf("!!! too many errors !!!\n");
                    }
                    else if (u4ErrCntC <= PIXEL_ERR_LOG_CNT)
                    {
                        printf("c(%4d,%4d) (m,u,v)(0x%08X,0x%08X,0x%08X) ", x, y, _u4ErrPos, u4GoldYSz + y * u4DecWidth / 2 + x, u4GoldYUSz + y * u4DecWidth / 2 + x);
                        printf("(0x%08X,0x%08X)\n", u4ErrVal, u4GolVal);
                    }
                }
                fgUOK = fgVOK = FALSE;
            }

            u4GolVal2 = ((u4VVal) & 0xFF000000) | ((u4UVal >>  8) & 0x00FF0000) | ((u4VVal >> 8) & 0x0000FF00) | ((u4UVal >> 16) & 0x000000FF);

            if (u4ErrVal2 != u4GolVal2)
            {
                u4ErrCntC++;
                if (_fgShowDiff)
                {
                    if (u4ErrCntC == PIXEL_ERR_LOG_CNT + 1)
                    {
                        printf("!!! too many errors !!!\n");
                    }
                    else if (u4ErrCntC <= PIXEL_ERR_LOG_CNT)
                    {
                        printf("c(%4d,%4d) (m,u,v)(0x%08X,0x%08X,0x%08X) ", x+2, y, _u4ErrPos + 4, u4GoldYSz + y * u4DecWidth / 2 + x+2, u4GoldYUSz + y * u4DecWidth / 2 + x+2);
                        printf("(0x%08X,0x%08X)\n", u4ErrVal2, u4GolVal2);
                    }
                }
                fgUOK = fgVOK = FALSE;
            }
        }
    }

    if (!fgUOK || !fgVOK)
      printf("Error count c: %d/%d\n", u4ErrCntC, u4Total/2);

    fgRet = fgYOK && fgUOK && fgVOK;

    return (fgRet);
}


VOID _RVLoadBitstream(UINT32 n)
{
    //printf("d.load.binary \"%s/%s/f%07d.bin\" 0x%08X /ny\n", FRMINFO_BASE_DIR, _pcLoadedBitstream, n, _pu1VfifoSa);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/%s/f%07d.bin\" 0x%08X /ny", FRMINFO_BASE_DIR, _pcLoadedBitstream, n, _pu1VfifoSa));
    HalFlushInvalidateDCache();
}


BOOL _VdecRvEmuIsItemEnd(VOID)
{
    UINT32 u4Parm = *(UINT32 *)(_pcFileInfo + _u4FrmInfoPtr);
    return (u4Parm == 0xFFFFFFFF)? TRUE : FALSE;
}


static UINT32 u4GetField(UINT32 u4Val, int b0, int b1)
{
    int i;
    UINT32 u4Mask = 0;
    
    for (i = b0; i <= b1; i++)
      u4Mask |= (1 << i);
    
    u4Val &= u4Mask;
    u4Val >>= b0;
    
    return u4Val;
}


BOOL fgGetRvPic(RvPicAttr *prPic)
{
    UINT32 u4Parm;
    int i;

    u4Parm = *(UINT32 *)(_pcFileInfo + _u4FrmInfoPtr);
    _u4FrmInfoPtr += 4;

    if (u4Parm == 0xFFFFFFFF)  // hit file end
      return FALSE;
    
    prPic->ePtype = (EnumRVPicCodType)u4GetField(u4Parm, 0, 1);
    prPic->u4OrgWidth = u4GetField(u4Parm, 2, 12);
    if (prPic->u4OrgWidth == 0)
    {
        prPic->u4OrgWidth = 2048;
    }
    prPic->u4OrgHeight = u4GetField(u4Parm, 13, 23);
    prPic->u4Width = (prPic->u4OrgWidth + 15) / 16 * 16;
    prPic->u4Height = (prPic->u4OrgHeight + 15) / 16 * 16;
    prPic->u4PctszSize = u4GetField(u4Parm, 24, 25);
    prPic->u4Pctsz = u4GetField(u4Parm, 26, 28);
    prPic->u4Oquant = u4GetField(u4Parm, 29, 30);
    prPic->u4DFP = u4GetField(u4Parm, 31, 31);

    u4Parm = *(UINT32 *)(_pcFileInfo + _u4FrmInfoPtr);
    _u4FrmInfoPtr += 4;

    prPic->u4Tr = u4GetField(u4Parm, 0, 12);
    prPic->u4Mba = u4GetField(u4Parm, 13, 26);
    prPic->u4Rtype = u4GetField(u4Parm, 27, 27);
    prPic->fgRV9 = u4GetField(u4Parm, 28, 28);

    u4Parm = *(UINT32 *)(_pcFileInfo + _u4FrmInfoPtr);
    _u4FrmInfoPtr += 4;

    prPic->u4NumSlice = u4GetField(u4Parm, 0, 7);
    prPic->u4Pquant = u4GetField(u4Parm, 8, 12);
    prPic->u4MbaSize = u4GetField(u4Parm, 13, 16);
    prPic->u4HdrSkip = u4GetField(u4Parm, 17, 24);

    u4Parm = *(UINT32 *)(_pcFileInfo + _u4FrmInfoPtr);
    _u4FrmInfoPtr += 4;

    prPic->u4Iratio = u4Parm;

    prPic->u4BstLength = 0;
    for (i = 0; i < prPic->u4NumSlice; i++)
    {
        u4Parm = *(UINT32 *)(_pcFileInfo + _u4FrmInfoPtr);
        _u4FrmInfoPtr += 4;
        prPic->au4SliceSize[i] = u4Parm;
        prPic->u4BstLength += u4Parm;
    }
    
    return TRUE;
}


VOID _VdecRvEmuGetWidthHeight(UINT32 *pu4OrgWidth, UINT32 *pu4OrgHeight)
{
    UINT32 u4Parm, u4OrgWidth;

    u4Parm = *(UINT32 *)(_pcFileInfo + _u4FrmInfoPtr);
    u4OrgWidth = u4GetField(u4Parm, 2, 12);
    if (u4OrgWidth == 0)
    {
        u4OrgWidth = 2048;
    }

    *pu4OrgWidth = u4OrgWidth;
    *pu4OrgHeight = u4GetField(u4Parm, 13, 23);
}


static UINT32 _VdecRvEmuCalFrmCnt(VOID)
{
    UINT32 u4Parm, u4FrmCnt, u4NumSlice;
    UINT32 u4FrmInfoPtr = _u4FrmInfoPtr;

    u4FrmCnt= 0;
    while (1)
    {
        u4Parm = *(UINT32 *)(_pcFileInfo + u4FrmInfoPtr);

        if (u4Parm == 0xFFFFFFFF)  // hit file end
          break;

        u4FrmInfoPtr += 8;

        u4Parm = *(UINT32 *)(_pcFileInfo + u4FrmInfoPtr);
        u4NumSlice = u4GetField(u4Parm, 0, 7);
        u4FrmInfoPtr += 8 + 4 * u4NumSlice;
        u4FrmCnt++;
    }

    printf("frm cnt = %d\n", u4FrmCnt);
    return u4FrmCnt;
}


static void _VdecSendRVPattern(void* pvArg)
{
    VDEC_PES_INFO_T rPesInfo;
    CHAR *pcItem;
    UINT32 u4FrmCnt;
    int i;

    _bIsPatSending = TRUE;

    x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    rPesInfo.u4FifoStart = _pu1VfifoSa;
    rPesInfo.u4VldReadPtr = _pu1VfifoSa;
    rPesInfo.u4FifoEnd = _pu1VfifoSa + VFIFO_SZ;
    rPesInfo.ucMpvId = VLD0;
    while (_iCurEmuItem < _iTotEmuItem)
    {
        if (!fgGetNextItem(&pcItem))
        {
            _iCurEmuItem = 0;
            break;
        }

        vLoadFrmInfo((char *)pcItem);

#ifdef CC_EMULATION_CRC
        _u4CRCPtr = 0;
        vLoadCRC((char *)pcItem);
#endif

        _pcLoadedBitstream = pcItem;
        _u4FrmInfoPtr = 0;

        u4FrmCnt = 1024;//_VdecRvEmuCalFrmCnt();
        if (u4FrmCnt > _u4NumFrmToTest)
        {
            u4FrmCnt = _u4NumFrmToTest;
        }
        printf("%d frm to test\n", u4FrmCnt);

        for (i = u4FrmCnt + 1; i; --i)
        {
            VDEC_SendEs((void*)&rPesInfo);
            x_thread_delay(1);
        }

        printf("//File %d Finish ES Sending\n", _iCurEmuItem - 1);
        VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
        printf("//File %d Finish ES Decoding\n", _iCurEmuItem - 1);
        x_thread_delay(500);
        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_RV);
    }

    _iCurEmuItem = 0;
    printf("RV Verification Done\n");
    _bIsPatSending = FALSE;
}


#ifdef RV_TRACKING
extern UINT32 _u4B2rCrcInx;
extern UINT32 _u4DispSuccessIdx;
extern UINT32 _u4CheckDispSuccessIdx;
extern BOOL _bSaveB2rCrc;
extern UINT32 _au4B2rCrcSave[];
extern UINT32 _au4B2rCrcArray[];
extern UINT32 _au4B2rCrcArray2[];
extern UINT32 _au4B2rCrcArray3[];

VOID ClrB2rVar(VOID)
{
    _u4B2rCrcInx = 0;
    _u4DispSuccessIdx = 0;
    _u4CheckDispSuccessIdx = 0;
    _bSaveB2rCrc = 0;
}

VOID ShowB2rAry(UINT32 u4Start, UINT32 u4End)
{
    printf("Show B2R CRC from %d to %d\n", u4Start, u4End);

    if (u4End >= 4096/*sizeof(_au4B2rCrcSave)*/)
    {
        u4End = 4095;
    }

    if (u4Start > u4End)
    {
        printf("u4Start > u4End\n");
        return;
    }

    for ( ; u4Start <= u4End; ++u4Start)
    {
        printf("%2d: 0x%08x(%2d)\t0x%08x(%2d)\n", 
            u4Start, 
            _au4B2rCrcSave[u4Start], _au4B2rCrcArray2[u4Start],
            _au4B2rCrcArray[u4Start], _au4B2rCrcArray3[u4Start]);
    }
}
#endif


static INT32 _VdecRvEmu(INT32 i4Argc, const CHAR ** szArgv)
{
    HANDLE_T hThread;
    UINT32 u4Case;

    if (i4Argc < 2)
    {
        goto SHOW_RV_EMU_USAGE;
    }

    u4Case = StrToInt(szArgv[1]);
    switch (u4Case)
    {
    default:
        goto SHOW_RV_EMU_USAGE;

    case 100:
        if (i4Argc < 3)
        {
            goto SHOW_RV_EMU_USAGE;
        }
        _aaRmEmuItem[0] = x_strcpy(_pcStartCodeFile, szArgv[2]);
        _iTotEmuItem = 1;
        _iCurEmuItem = 0;
    case 101:
        VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
        255,  _VdecSendRVPattern, 0, NULL) == OSR_OK);
        break;

    case 300:
        _fgRvEmuCompGolden = !_fgRvEmuCompGolden;
        printf("Toggle golden compare %s\n\n", _fgRvEmuCompGolden ? "ON" : "OFF");
        break;

#ifdef RV_TRACKING
/*
bitstream:
0B => RV8_004
1B => RV8_001
2B => RV10_001
3B => RV9_001

CLI:
mid.r
vdec.i
fbm.i
vdec.d_l 1
vdec.p 0 6

vdp.i
vdp.e 0 1
vdp.s.outr 0 1
vdp.s.srcr 0 1
vdp.s.mode 0 0
vdp.s.i 0 0 0
vdp.d_l 5

vdec.sp 996 100

vdec.sp 301
vdec.sp 302
vdec.sp 100 WCP/RV40/RV9_001

vdec.sp 301
vdec.sp 100 WCP/RV40/RV9_001
*/
    case 301:
        ClrB2rVar();
        printf("Clear B2R CRC\n");
        break;

    case 302:
        _bSaveB2rCrc = !_bSaveB2rCrc;
        printf("Toggle B2R Save CRC %s\n\n", _bSaveB2rCrc ? "ON" : "OFF");
        break;

    case 303:
        _fgRvTracking = !_fgRvTracking;
        printf("Toggle RV Tracking %s\n\n", _fgRvTracking ? "ON" : "OFF");
        break;

    case 305:
        _fgRvShrinkFbm = !_fgRvShrinkFbm;
        printf("Toggle RV Shrink FBM %s\n\n", _fgRvShrinkFbm ? "ON" : "OFF");
        break;

    case 320:
        if (i4Argc < 4)
        {
            goto SHOW_RV_EMU_USAGE;
        }
        ShowB2rAry(StrToInt(szArgv[2]), StrToInt(szArgv[3]));
        break;
#endif

    case 992:
        if (i4Argc >= 3)
        {
            vLoadExeList((char *)(szArgv[2]));
        }
        vShowExeList();
        break;

    case 996:
        if (i4Argc >= 3)
        {
            _u4NumFrmToTest = StrToInt(szArgv[2]);
            if (_u4NumFrmToTest == 0)
            {
                _u4NumFrmToTest = (UINT32)-1;
            }
            _u4RVEmuEndPicNum = _u4NumFrmToTest - 1;
        }
        printf("Number of frames to test is %d\n", _u4NumFrmToTest);
        break;
    }

    return 0;

SHOW_RV_EMU_USAGE:
    printf("Usage: %s <case> [<arg> ...]\n", szArgv[0]);
    printf("case {100: Test whole file, %s 100 filename}\n", szArgv[0]);
    printf("case {101: Test whole file by execution list}\n");
    printf("case {300: Toggle golden compare ON/OFF}\n");
#ifdef RV_TRACKING
    printf("case {301: Clear B2R CRC}\n");
    printf("case {302: Toggle Save CRC ON/OFF}\n");
    printf("case {303: Toggle RV Tracking ON/OFF}\n");
    printf("case {305: Toggle RV Shrink FBM ON/OFF}\n");
    printf("case {320: Print B2R CRC from s to e, %s 305 s e}\n", szArgv[0]);
#endif
    printf("case {992: Load execution list, %s 992 exe_list_path}\n", szArgv[0]);
    printf("case {996: Change number of frames to test, %s 996 n}\n", szArgv[0]);
    return -1;
}
#endif


static INT32 _VdecCmdSaveDecOut(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if (i4Argc < 2)
    {
        LOG(0, "vdec.so enable(1)");
        return 0;
    }
    _bSaveOutput = (BOOL)StrToInt(szArgv[1]);
    return 0;
}

void Dump_Mem(VDEC_DISP_PIC_INFO_T* prDispInfo)
{
#ifndef DUMP_MEM
    return;
#endif

    static int i = 0;
    UINT8* pu1HWAddr;
    UINT8* hw_output;

    UINT32 dwWidth = prDispInfo->u4W;
    UINT32 dwHeight = prDispInfo->u4H;
    //UINT32 m_dwCurrPicNo = _u4CurFrmIdx;

    //UINT32 dwWidth = rWMVDecInfo.rSeqHdrR.u4PicWidthCmp;
    //UINT32 dwHeight = rWMVDecInfo.rSeqHdrR.u4PicHeightCmp;

    hw_output = prDispInfo->pvYAddr;

    if (dwWidth % 16)
        dwWidth = ((dwWidth / 16) + 1) * 16; //ming modify@2006/4/11
    if (dwHeight % 32)
        dwHeight = ((dwHeight / 32) + 1) * 32;


    pu1HWAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)hw_output, 0, 0, dwWidth, TRUE, 4);

#ifdef CC_DRIVER_DEMO
    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y_%d.raw\" 0x%08x--0x%08x", _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx, pu1HWAddr, pu1HWAddr + dwWidth*dwHeight - 1));
#else
    LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y0_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,rWMVDecInfo.rDram.pu1Pic0Y, rWMVDecInfo.rDram.pu1Pic0Y+dwWidth*dwHeight-1));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y1_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,rWMVDecInfo.rDram.pu1Pic1Y, rWMVDecInfo.rDram.pu1Pic1Y+dwWidth*dwHeight-1));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y2_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,rWMVDecInfo.rDram.pu1Pic2Y, rWMVDecInfo.rDram.pu1Pic2Y+dwWidth*dwHeight-1));

    hw_output = prDispInfo->pvCAddr;
    pu1HWAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)hw_output, 0, 0, dwWidth, TRUE, TRUE, 4);

#ifdef CC_DRIVER_DEMO
    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_C_%d.raw\" 0x%08x--0x%08x", _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx, pu1HWAddr, (pu1HWAddr + dwWidth*dwHeight / 2) - 1));
#else
    LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
#endif
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_C0_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],rWMVDecInfo.rOther.u4CurrPicNo,rWMVDecInfo.rDram.pu1Pic0C, (rWMVDecInfo.rDram.pu1Pic0C+dwWidth*dwHeight/2)-1));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_C1_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],rWMVDecInfo.rOther.u4CurrPicNo,rWMVDecInfo.rDram.pu1Pic1C, (rWMVDecInfo.rDram.pu1Pic1C+dwWidth*dwHeight/2)-1));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_C2_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],rWMVDecInfo.rOther.u4CurrPicNo,rWMVDecInfo.rDram.pu1Pic2C, (rWMVDecInfo.rDram.pu1Pic2C+dwWidth*dwHeight/2)-1));


    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_BP1_%d.dat\" 0x%08x--0x%08x",_u4CurFileIdx[_u4CurFileIdx],rWMVDecInfo.rOther.u4CurrPicNo,Bp_1, Bp_2));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_BP2_%d.dat\" 0x%08x--0x%08x",_u4CurFileIdx[_u4CurFileIdx],rWMVDecInfo.rOther.u4CurrPicNo,Bp_2, Bp_3));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_BP3_%d.dat\" 0x%08x--0x%08x",_u4CurFileIdx[_u4CurFileIdx],rWMVDecInfo.rOther.u4CurrPicNo,Bp_3, Bp_4));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_BP4_%d.dat\" 0x%08x--0x%08x",_u4CurFileIdx[_u4CurFileIdx],rWMVDecInfo.rOther.u4CurrPicNo,Bp_4, Mv_3));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_MV1_%d.dat\" 0x%08x--0x%08x",_fiInName,rWMVDecInfo.rOther.u4CurrPicNo,Mv_1, Mv_2));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_MV12_%d.dat\" 0x%08x--0x%08x",_fiInName,rWMVDecInfo.rOther.u4CurrPicNo,Mv_1_2, DCAC_2));


    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_stream_%d_%d.dat\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],rWMVDecInfo.rSeqHdrR.u4PicWidthDec, rWMVDecInfo.rSeqHdrR.u4PicHeightDec, V_FIFO_SA, V_FIFO_SA+rWMVDecInfo.rOther.u4FileLen));

    i++;
}

 
void print_checksum(BOOL fgRead, UINT32 u4FrmIdx)
{
    UINT32 u4Len;
#ifdef CC_WMV_EMULATION 
    u4Len = u4DumpWmvReg(_pu1RegDumpBuf);
#endif
    #ifdef CC_DRIVER_DEMO
    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_%d.txt\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],u4FrmIdx,(UINT32)_pu1RegDumpBuf, (UINT32)_pu1RegDumpBuf + u4Len - 1));
    #endif
}

    /*    Printf("===WMV VLD status===\n");
        Printf("Decode complete, 240 : %d\n",ReadREG(RO_VLD_PIC_DEC_END));
    for(u4Idx = (VLD_REG_OFST + (0x208)); u4Idx <= (VLD_REG_OFST + (0x258)); u4Idx+=4)
    {
        Printf("VLD status, %d(0x%3x)=0x%x\n", ((u4Idx-VLD_REG_OFST)>>2), u4Idx, ReadREG(u4Idx));
    }
    Printf("===WMV CheckSum===\n");
        Printf("_DCAC_1_CheckSum = 0x%x\n",ReadREG(VLD_REG_OFST+(0x3C4)));
          Printf("_DCAC_2_CheckSum = 0x%x\n",ReadREG(VLD_REG_OFST+(0x3C8)));
          //Printf("_DCAC_237_CheckSum = 0x%x\n",VLD_READ32((0x3B4)));
          //Printf("_DCAC_238_CheckSum = 0x%x\n",VLD_READ32((0x3B8)));
          //Printf("_VLD_189_CheckSum = %x\n",ReadREG(VLD_REG_OFST + (0x2F4)));
          Printf("MV1 = 0x%x\n",ReadREG(VLD_REG_OFST+(0x3D8)));
          Printf("MV2 = 0x%x\n",ReadREG(VLD_REG_OFST+(0x3DC)));
          Printf("MV3 = 0x%x\n",ReadREG(VLD_REG_OFST+(0x3E0)));

          Printf("DP1 = 0x%x\n",ReadREG(VLD_REG_OFST+(0x3EC)));

          Printf("VOP_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x350)));
          Printf("DCSA2_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x354)));
          Printf("MV3SA_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x358)));
          Printf("DCFSM_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x37C)));
          Printf("DCBLK_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x380)));
          Printf("DCDRAM_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x384)));

          Printf("MVST_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x388)));
          Printf("MVDRAM_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x38C)));
          Printf("MBST_REG = 0x%x\n",ReadREG(VLD_REG_OFST+(0x390)));


          Printf("IDCT Input Y0 Y1 Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x5E8)));
          Printf("IDCT Input Y2 Y3 Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x5EC)));
          Printf("IDCT Input Cb Cr Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x5F0)));
          Printf("MC Output Luma Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x5F4)));
          Printf("MC Output Chroma Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x5F8)));
          Printf("Motion Vector Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x5FC)));
          Printf("MC Parameter 1 Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x600)));
          Printf("MC Parameter 2 Checksum Register = 0x%x\n",ReadREG(MC_REG_OFST+(0x604)));

          Printf("WMV parameter 2: %x\n",ReadREG(RW_VLD_WMV_PARA2));

        if(fgRead)
        {
        for(u4Idx = 0; u4Idx <= 231; u4Idx++)
        {
            _mcreg[u4Idx] = ReadREG(MC_REG_OFST + u4Idx*4);
        }

              //VLD from 34 ~ 147,  192~250, except 40, 41 (read will lead some impact of HW)
        for(u4Idx = 34; u4Idx <= 39; u4Idx++)
        {
            _vlcreg[u4Idx] = ReadREG(VLD_REG_OFST + u4Idx*4);
        }
        for(u4Idx = 42; u4Idx <= 147; u4Idx++)
        {
            _vlcreg[u4Idx] = ReadREG(VLD_REG_OFST + u4Idx*4);
        }
        for(u4Idx = 192; u4Idx <= 250; u4Idx++)
        {
            _vlcreg[u4Idx] = ReadREG(VLD_REG_OFST + u4Idx*4);
        }
         }

        Printf("===WMV MC ALL===\n");
        for(u4Idx = (MC_REG_OFST + (0)); u4Idx <= (MC_REG_OFST + (0x39C)); u4Idx+=4)
    {
        Printf("MC status, %d(0x%3x)=0x%x\n", ((u4Idx-MC_REG_OFST)>>2), u4Idx, _mcreg[(u4Idx - MC_REG_OFST)>>2]);
    }

          //VLD from 34 ~ 147,  192~250, except 40, 41 (read will lead some impact of HW)
        Printf("===WMV VLD Other===\n");
    for(u4Idx = (VLD_REG_OFST + (0x88)); u4Idx <= (VLD_REG_OFST + (0x9C)); u4Idx+=4)
    {
        Printf("VLD status, %d(0x%3x)=0x%x\n", ((u4Idx-VLD_REG_OFST)>>2), u4Idx, _vlcreg[(u4Idx - VLD_REG_OFST)>>2]);
    }
    for(u4Idx = (VLD_REG_OFST + (0xA8)); u4Idx <= (VLD_REG_OFST + (0x24C)); u4Idx+=4)
    {
        Printf("VLD status, %d(0x%3x)=0x%x\n", ((u4Idx-VLD_REG_OFST)>>2), u4Idx, _vlcreg[(u4Idx - VLD_REG_OFST)>>2]);
    }
    for(u4Idx = (VLD_REG_OFST + (0x300)); u4Idx <= (VLD_REG_OFST + (0x3e8)); u4Idx+=4)
    {
        Printf("VLD status, %d(0x%3x)=0x%x\n", ((u4Idx-VLD_REG_OFST)>>2), u4Idx, _vlcreg[(u4Idx - VLD_REG_OFST)>>2]);
    }


}
*/


UINT32 u4CalculatePixelAddress_Y(
    UINT32 u4YBase,                           ///< [IN] frame buffer Y component address
    UINT32 u4XPos,                             ///< [IN] x position of pixel in frame buffer
    UINT32 u4YPos,                             ///< [IN] y position of pixel in frame buffer
    UINT32 u4FrameWidth,                 ///< [IN] Frame buffer width
    BOOL fgBlock,                               ///< [IN] MTK block / raster scan
    UCHAR bBlockW
)
{
    UINT32 u4YAdr;
    UINT32 u4X = u4XPos;
    UINT32 u4Y = u4YPos;
    UINT32 u4YBlockNum;
    UINT32 u4YOffsetInBlock;
    UINT32 u4XBlockNum;
    UINT32 u4XOffsetInBlock;

    if(bBlockW == 6) // in DTV, one block width is (64x32)
    {
        u4XOffsetInBlock = u4X & 0x3F;
    }
    else if(bBlockW == 4)  // in MT8520, one block width is (16x32)
    {
        u4XOffsetInBlock = u4X & 0xF;
    }
    else if(bBlockW == 3)  // in MT8108 and MT1389S, one block width is (8x32)
    {
        u4XOffsetInBlock = u4X & 0x7;
    }
    else if(bBlockW == 2)  // in MT8105, one block width is (4x32)
    {
        u4XOffsetInBlock = u4X & 0x3;
    }


    // Y arrangement is the same in 420 and 422 YCbCr Mode.
    u4YBlockNum = u4Y >> 5;
    u4YOffsetInBlock = u4Y & 0x1F;
    u4XBlockNum = u4X >> bBlockW;
    if (fgBlock)
        u4YAdr = u4YBase + ((u4FrameWidth * u4YBlockNum) << 5) + ((u4YOffsetInBlock + (u4XBlockNum << 5) ) << bBlockW) + u4XOffsetInBlock;
    else
        u4YAdr =  u4YBase + (u4Y * u4FrameWidth) + u4X;

    return u4YAdr;
}

UINT32 u4CalculatePixelAddress_C(
    UINT32 u4CBase,                           ///< [IN] frame buffer CbCr component address
    UINT32 u4XPos,                             ///< [IN] x position of pixel in frame buffer
    UINT32 u4YPos,                             ///< [IN] y position of pixel in frame buffer
    UINT32 u4FrameWidth,                 ///< [IN] Frame buffer width
    BOOL fgBlock,                               ///< [IN] MTK block / raster scan
    BOOL fg420,                                   ///< [IN] 420 / 422
    UCHAR bBlockW
)
{
    UINT32 u4CAdr;
    UINT32 u4X = u4XPos;
    UINT32 u4Y = u4YPos;
    UINT32 u4YBlockNum;
    UINT32 u4YOffsetInBlock;
    UINT32 u4XBlockNum;
    UINT32 u4XOffsetInBlock;


    u4YBlockNum = u4Y >> 5;
    u4YOffsetInBlock = u4Y & 0x1F;
    u4XBlockNum = u4X >> bBlockW;

    // Map C (chrominance)
    u4X &= 0xFFFE;
    if(bBlockW == 6)
    {
        u4XOffsetInBlock = u4X & 0x3F;
    }
    else if(bBlockW == 4)
    {
        u4XOffsetInBlock = u4X & 0xF;
    }
    else if(bBlockW == 3)
    {
        u4XOffsetInBlock = u4X & 0x7;
    }
    else if(bBlockW == 2)
    {
        u4XOffsetInBlock = u4X & 0x3;
    }

    u4FrameWidth = ((u4FrameWidth + 1) & 0xFFFE);
    if (fg420)
    {
        u4Y = u4Y >> 1;
        u4YOffsetInBlock = u4Y & 0xF;
        if (fgBlock)
            u4CAdr = u4CBase + ((u4FrameWidth * u4YBlockNum) << 4) + ((u4YOffsetInBlock + (u4XBlockNum << 4) ) << bBlockW) + u4XOffsetInBlock;
        else
            u4CAdr = u4CBase + (u4Y * u4FrameWidth) + (u4X);
    }
    else
    {
        // This code should be emended.
        if (fgBlock)
            u4CAdr = u4CBase + ((u4FrameWidth * u4YBlockNum) << 5) + ((u4YOffsetInBlock + (u4XBlockNum << 4) ) << bBlockW) + u4XOffsetInBlock;
        else
            u4CAdr = u4CBase + (u4Y * u4FrameWidth) + (u4X);
    }

    return u4CAdr;
}

/*
BOOL WMV_save_pic(CHAR* golden_path,CHAR* golden_prefix)
{

  if((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD)||(rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1))
  {
    if(!my_save_pic(golden_path,golden_prefix, TRUE))
    {
        return FALSE;
    }
    if(!my_save_pic(golden_path,golden_prefix, FALSE))
    {
        return FALSE;
    }
  }
  return TRUE;
}*/

static INT32 _VdecCmdSaveSimPat(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if (i4Argc < 3)
    {
        LOG(0, "vdec.ssp [enable 1][Frame number(start from 0)]");
        return 0;
    }
    if (StrToInt(szArgv[1]) == 0)
    {
        _i4SSimPatFrmNo = -1;
    }
    else
    {
        _i4SSimPatFrmNo = (INT32)StrToInt(szArgv[2]);
        if (_i4SSimPatFrmNo == 0)
        {
            gu1VdecLogSimPat = 1;
            VDEC_SetParam(ES0, LOG_SIMU_PAT, 1, 0, 0);
        }
    }
    return 0;
}

static INT32 _VdecCmdSetTestMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4FrmIdx;

    ASSERT(szArgv != NULL);
    if (i4Argc < 3)
    {
        LOG(0, "vdec.stm [TestSet 0:other, 1:Allegro, 2:Allegro2][Comparison Enable]([FrameNo1][FrameNo2]...)");
        return 0;
    }
    VDEC_SetParam(ES0, OUTPUT_DISP_ORDER, StrToInt(szArgv[2]), 0, 0);

    _u1TestSet = (UINT8)StrToInt(szArgv[1]);
    if (StrToInt(szArgv[2]) > 0)
    {
        _bCmpEn = TRUE;
        if (i4Argc > 3)
        {
            x_memset((void*)_bCmpFrmEn, 0, sizeof(_bCmpFrmEn));
            for (u4FrmIdx = 3; u4FrmIdx < i4Argc; u4FrmIdx++)
            {
                _bCmpFrmEn[StrToInt(szArgv[u4FrmIdx])] = TRUE;
            }
        }
        else
        {
            for (u4FrmIdx = 0; u4FrmIdx < 1000; u4FrmIdx++)
            {
                _bCmpFrmEn[u4FrmIdx] = TRUE;
            }
        }
    }
    else
    {
        x_memset((void*)_bCmpFrmEn, 0, sizeof(_bCmpFrmEn));
        _bCmpEn = FALSE;
    }

    return 0;
}

static INT32 _VdecCmdSetRepeatFrm(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if (i4Argc < 3)
    {
        LOG(0, "vdec.srf [Repeat Frame Enable][Repeat FrameNo]\n");
        LOG(0, "vdec.srf %d %d\n", _bRptFrmEn, _u4RptFrmNo);
        return 0;
    }
    if (StrToInt(szArgv[1]) > 0)
    {
        _bRptFrmEn = TRUE;
        _u4RptFrmNo = (UINT32)StrToInt(szArgv[2]);
        if (_u4RptFrmNo == 0)
        {
            VDEC_SetParam(ES0, REPEAT_FRM, 1, 0, 0);
        }
    }
    else
    {
        _bRptFrmEn = FALSE;
        _u4RptFrmNo = 0;
    }

    return 0;
}

static INT32 _VdecCmdChkSumEn(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if (i4Argc < 2)
    {
        LOG(0, "vdec.cs [Checksum Enable]\n");
        LOG(0, "vdec.cs %d\n", _fgEnChkSum);
        return 0;
    }
    if (StrToInt(szArgv[1]) > 0)
    {
        _fgEnChkSum = TRUE;
    }
    else
    {
        _fgEnChkSum = FALSE;
    }

    return 0;
}
static INT32 _VdecCmdGenCrcEn(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if(i4Argc < 2)
    {
        LOG(0, "vdec.gcrc [Generate Crc Enable]\n");
        LOG(0, "vdec.gcrc %d\n", _uGenCrcEn);
        return 0;
    }

    
    if(StrToInt(szArgv[1]) > 0)
    {
        _uGenCrcEn = StrToInt(szArgv[1]);
    }
    else
    {
        _uGenCrcEn = 0;
    }

    return 0;
}

//wmv crc
static INT32 _VdecCmdVerifySetInfo(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT32 u4Index=0,u4Enable,u4StrLen,u4StartIndex;
   if(i4Argc<=2)
   {
      LOG(0,"Enable,Id,PatternPath,GoldenPath,StartFileIndex\n");
      for(u4Index=0;u4Index<VDEC_VERIFY_SET_NUM;u4Index++)
      {  
         LOG(0,"Set %d,Enable=%s\n",u4Index,rVerifySet[u4Index].u4Enable ? "True" : "False");
         
         if(rVerifySet[u4Index].u4Enable || x_strlen(rVerifySet[u4Index].rPatternFile)>1)
         {
           LOG(0,"PatternFile=%s\n",rVerifySet[u4Index].rPatternFile);
           LOG(0,"GoldenFile=%s\n",rVerifySet[u4Index].rGoldenFile);
           LOG(0,"StartFile=%d\n",rVerifySet[u4Index].u4StartFile);
         }
      }

      return 0;
   }
   
   u4Enable=StrToInt(szArgv[1]);
   u4Index=StrToInt(szArgv[2]);
   if(i4Argc==3)
   {
     if(u4Enable&&x_strlen(rVerifySet[u4Index].rPatternFile)>1)
     {
        rVerifySet[u4Index].u4Enable=1;
     } 
     else
     {
        rVerifySet[u4Index].u4Enable=0;
     }

     return 0;
   }

   if(i4Argc==4)
   {
       u4StartIndex=StrToInt(szArgv[2]);

       if(u4Enable&&x_strlen(rVerifySet[u4Index].rPatternFile)>1)
       {
          rVerifySet[u4Index].u4Enable=1;
       } 
       else
       {
          rVerifySet[u4Index].u4Enable=0;
       }

       rVerifySet[u4Index].u4StartFile=u4StartIndex;
       return 0;
   }
   
   x_memset(rVerifySet[u4Index].rPatternFile,0,MAX_FILENAME_LEN);
   x_memset(rVerifySet[u4Index].rGoldenFile,0,MAX_FILENAME_LEN);
   x_memcpy(rVerifySet[u4Index].rPatternFile,szArgv[3],x_strlen(szArgv[3]));
   x_memcpy(rVerifySet[u4Index].rGoldenFile,szArgv[4],x_strlen(szArgv[4]));
   rVerifySet[u4Index].u4Enable=StrToInt(szArgv[1]);
   rVerifySet[u4Index].u4StartFile=StrToInt(szArgv[5]);

   return 0;
}
#ifdef CC_WMV_EMULATION
static INT32 _VdecCmdLoadResult(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc<2)
    {
       LOG(0,"Enable=%d,FileId=%d,Frame=%d\n",_fgDownLoadResult,_u4DownLoadFileId,_u4DownLoadFrameId);
       return 0;
    }

    if(i4Argc>=2)
    {
      _fgDownLoadResult= (BOOL)StrToInt(szArgv[1]);
    }

    if(i4Argc==2)
    {
       return 0;
    }

    if(i4Argc>=4)
    {
      _u4DownLoadFileId= (UINT32)StrToInt(szArgv[2]);
      _u4DownLoadFrameId= (UINT32)StrToInt(szArgv[3]);
    }

    return 0;
}
#endif
#endif

static INT32 _VdecCmdDramBWSimu(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Param1 = 0, u4Param2 = 0, u4Param3 = 0;

    ASSERT(szArgv != NULL);
    if (i4Argc < 4)
    {
        LOG(0, "vdec.ds [MC request max len (1-100), Disable req connect(1:Disable, 0:En), FIFO(0:16Bank, 1:8bank, 2:4bank)]\n");
        VDEC_GetParam(ES0, (UINT32)DRAM_BANDWITH_SIMU, &u4Param1, &u4Param2, &u4Param3);
        LOG(0, "vdec.ds %d %d %d\n", u4Param1, u4Param2, u4Param3);
        return 0;
    }

    VDEC_SetParam(ES0, (UINT32)DRAM_BANDWITH_SIMU, StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]));
    return 0;
}

static INT32 _VdecCmdMemProtect(INT32 i4Argc, const CHAR ** szArgv)
{
    FBM_POOL_T* prFbmPoolDMX;
    BOOL fgRet = TRUE;

    ASSERT(szArgv != NULL);
    if (i4Argc < 3)
    {
        LOG(0, "vdec.mid mode [0:none, 1:OverWriteTest(exclusive), 2:Vfifo/FBM Protect, 3:Vfifo/FBM Protect emu] MPEG2DBKDisable[1:disable]\n");
        LOG(0, "vdec.mid %d %d\n\n", _u1MidMode, _fgDBKDisable);
        return 0;
    }
    prFbmPoolDMX = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    if ((prFbmPoolDMX == NULL) || (prFbmPoolDMX->u4Addr == 0))
    {
        return FALSE;
    }

    fgRet &= MID_Init();
    fgRet &= MID_Reset();

    _u1MidMode = (UINT8)StrToInt(szArgv[1]);
    _fgDBKDisable = (UINT8)StrToInt(szArgv[2]);
    if (_u1MidMode > 3)
    {
        return FALSE;
    }
    else if (_u1MidMode == 3)
    {
        _u4VStart = VFIFO_S;
        _u4VEnd = VFIFO_E;
        fgRet &= MID_SetRegion(0, MID_AGENT_CPU, _u4VStart, _u4VEnd);
        fgRet &= MID_EnableRegionProtect(0);
    }

    if (_u1MidMode == 1)
    {
        //frame-based mid
        fgRet &= VDEC_SetMid(ES0, TRUE);
    }
    else
    {
        fgRet &= VDEC_SetMid(ES0, FALSE);
    }

    if (_u1MidMode == 0)
    {
        VDEC_FbmMidTest(ES0, 0, FALSE);
        fgRet &= BIM_DisableIrq(VECTOR_DRAMC);
    }
    else
    {
        // Disable allocate fbm with gfx clean
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_MEMSET, (UINT32)NULL);
        // call fbm flag, enable MID after FBG create
        VDEC_FbmMidTest(ES0, _u1MidMode, _fgDBKDisable);
        if (_fgDBKDisable)
        {
            //disable mpeg2 deblocking
            MPV_SetDbkInfo(0, FALSE, 0, 0, 0, 0);
        }
        fgRet &= BIM_EnableIrq(VECTOR_DRAMC);
    }

    if (fgRet)
    {
        LOG(0, "Memory Protect Set Done~\n");
        if (_u1MidMode > 1)
        {
            LOG(0, "Enable region after FBG create\n");
        }
    }
    else
    {
        LOG(0, "Memory Protect Set Fail !\n");
    }
    return 0;
}

static INT32 _VdecCmdAVSyncTest(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgRet = TRUE;
    UINT8 u1Mode = 0;

    ASSERT(szArgv != NULL);
    if (i4Argc < 2)
    {
        LOG(0, "vdec.st ModeMask [0:none, 1:Fake 1'st frame PTS, 2:Fake PTS(random), 4:Delete pcr pid]\n");
        LOG(0, "vdec.st %d\n", _u1AVSyncTestMode);
        return 0;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (u1Mode < 8)
    {
        _u1AVSyncTestMode = u1Mode;
        VDEC_AVSyncTest(ES0, u1Mode);
    }
    return fgRet;
}


static INT32 _VdecCmdFixFHDDisplay(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_ITIMING_ALWAYS
    UCHAR  ucEsId;
    BOOL     fgInterlace;    
    
    if (3 == i4Argc)
    {
        ucEsId = (UCHAR)StrToInt(szArgv[1]);
        fgInterlace = (0 < StrToInt(szArgv[2])) ? TRUE : FALSE;
        
        if (ucEsId >= VDEC_MAX_ES)
        {
            Printf("Input ES Id larger than %d\n", VDEC_MAX_ES);
        }
        else
        {
            VDEC_SetFixFHDDisplay(ucEsId, fgInterlace);
            Printf("SetFixFHDisplay: EsId:%d On:%s\n",
                     ucEsId,
                     (TRUE == fgInterlace) ? "true" : "false");
        }
    }
    else
    {
        Printf("Error: invalid parameter,ignored.\n");
    }
#else
        Printf("CC_ITIMING_ALWAYS was not defined, ignored.\n");
#endif

    return 0;
}

#if defined(CC_STRESS_TEST_CRC_CHK) || defined(CC_EMULATION)
static INT32 _VdecCmdRecordCRC(INT32 i4Argc, const CHAR ** szArgv)
{      
    ASSERT(szArgv != NULL);
    if(i4Argc < 2)
    {
        LOG(0, "vdec.crcRec pts_value\n");
        LOG(0, "vdec.crcRec 0x%x \n\n", _u4ChkPTS);
        return 0;
    }
    _u4ChkPTS = (UINT32)StrToInt(szArgv[1]);
    VDEC_SetParam(ES0, (UINT32)RECORD_CRC, _u4ChkPTS, 0, 0);
    return 0;
}

static INT32 _VdecCmdEnCRC(INT32 i4Argc, const CHAR ** szArgv)
{      
    UINT32 u4CrcErrCnt = 0, u4BsErrCnt = 0, u4PassCnt = 0, u4Idx;
    UINT32* pu4ErrCnt;
    
    ASSERT(szArgv != NULL);

    VDEC_GetParam(ES0, (UINT32)CRC_CHK, &u4CrcErrCnt, &u4BsErrCnt, &u4PassCnt);

    if(u4CrcErrCnt == 0)
    {
        LOG(0, "Get status err, u4CrcErrCnt = 0\n");
    }
    pu4ErrCnt = (UINT32*)u4CrcErrCnt;
    if(i4Argc < 2)
    {
        LOG(0, "vdec.crc enable[0:off, 1:on]\n");
        LOG(0, "vdec.crc %d, err cnt(bs, pp_y, pp_c)=(%d, %d, %d), pass cnt %d, checking pts 0x%x\n\n", 
            _fgEnableCRC, u4BsErrCnt, pu4ErrCnt[0], pu4ErrCnt[1], u4PassCnt, _u4ChkPTS);
        if((pu4ErrCnt[0]> 0) || (pu4ErrCnt[1]>0))
        {
            for(u4Idx = 4; u4Idx <= 21; u4Idx++)
            {
                LOG(0, "Idx %d errorCnt %d\n");
            }
        }
        return 0;
    }
    _fgEnableCRC = (BOOL)StrToInt(szArgv[1]);
    VDEC_SetParam(ES0, (UINT32)CRC_CHK, _fgEnableCRC, 0, 0);

    return 0;
}
#endif


