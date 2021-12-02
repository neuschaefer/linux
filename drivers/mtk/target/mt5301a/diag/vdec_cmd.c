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

#ifdef CC_MT5363
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif

#define DEFINE_IS_LOG VDEC_IsLog
#include "x_debug.h"
#include "x_mid.h"
#include "x_bim.h"

//#define CC_EMULATION
//#define CC_64x32BLK_CMP
//for emulation
#ifdef CC_EMULATION
  #ifdef CC_DRIVER_DEMO
  #include "drv_t32.h"
  #endif
  //#include "vdec_ide.h"
  #include <string.h>

#endif

/******************************************************************************
* Macro            : helper macros to reduce code
******************************************************************************/
#define STRESS_TESTING
//#define RR_TEST
//#define IDE_READ_SUPPORT
//#define IDE_WRITE_SUPPORT

//#define RV_TEST

//wmv spork
//#define MEM_TEST
#define WMV_PIC_HEADER_LEN (4)
#define DO_COMPARE_FRAME
#define DUMP_MEM
#define START_COMPARE_FRAME (0)//440//532
#define END_COMPARE_FRAME (9999)
#define WMV_PROFILING

#define FILE_INFO_SIZE 0x40000
#define CHKSUM_INFO_SIZE 0x100000
#ifdef RV_TEST
#define GOLDEN_SIZE (1920 * 1088 + 1920 * 1088 / 2)
#else
#define GOLDEN_SIZE (1920 * 1088)
#endif
#define MAX_FILE_NUM 2000
#define MAX_FILENAME_LEN 256

#ifdef MEM_TEST
#define VFIFO_S 0x179c000    //0x2e24000    //should get from dmx fbm
#define VFIFO_E 0x679c000    //80M
#else
#ifdef RV_TEST
#define VFIFO_S 0x5134000
#define VFIFO_E (0x7FF0000 - 1048576 * 2)
#else
//#define VFIFO_S 0x2ff0000    //0x2e80000    //0x2e24000    //should get from dmx fbm
//#define VFIFO_E 0x7ff0000    //0x7ffc000    //81.48M
#define VFIFO_S 0x5134000
#define VFIFO_E 0x7FF0000
//#define VFIFO_S 0x3ff0000      //539x
//#define VFIFO_E 0x7ff0000      //539x
#endif
#define FRMINFO_S VFIFO_E
#endif

#define NO_PIC 0
#define TOP_FIELD 1
#define BOTTOM_FIELD 2
#define FRAME 3
#define CS_NUM 4096    //frame number
#define CS_PER_FRM 4 //Y1, Y2, C1, C2

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
    TIME_PROFILE3
}   H264_PARAM_T;

//for emulation checksum value
typedef struct
{  
    UINT32 u4ShftAddY1[CS_NUM];
    UINT32 u4ShftAddY2[CS_NUM];
    UINT32 u4ShftAddC1[CS_NUM];
    UINT32 u4ShftAddC2[CS_NUM];
}VDEC_CHKSUM_INFO_T;

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

#ifdef CC_EMULATION

UINT32 _VdecGetChkSum(UCHAR* pucPtr, VDEC_CHKSUM_INFO_T* prCSInfo);

void _VdecNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

void _VdecMPEG4Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

void _VdecWMVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

void _VdecRVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);

static BOOL _LoadFile(const char* pcFileName, UINT32 u4Addr);

#ifdef IDE_WRITE_SUPPORT
static BOOL _IDEComp(const char* pcFileName, UINT32 u4Addr, UINT32 u4Len);
#endif

static BOOL _VdecVerifyDecData(const VDEC_DISP_PIC_INFO_T* prDispInfo, BOOL bIsYcomp);

#ifdef CC_64x32BLK_CMP
static void _VdecBlkTransform(UINT8* pu1DstBlk16x32, const VDEC_DISP_PIC_INFO_T* prDispInfo, BOOL bIsYcomp);
#endif

static void _Vdec1389B2R(UINT8* pu1RasterData, VDEC_DISP_PIC_INFO_T rDispInfo, BOOL bIsYcomp);

static void _VdecSendH264Pattern(void* pvArg);

static void _VdecSendMPEG4Pattern(void* pvArg);

static void _VdecSendWMVPattern(void* pvArg);

static void _VdecSendRVPattern(void* pvArg);

static INT32 _VdecCmdSendPattern(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSaveDecOut(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSaveSimPat(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSetTestMode(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdSetRepeatFrm(INT32 i4Argc, const CHAR ** szArgv);

static INT32 _VdecCmdChkSumEn(INT32 i4Argc, const CHAR ** szArgv);

//wmv
void Dump_Mem(VDEC_DISP_PIC_INFO_T*);
void print_checksum(BOOL fgRead);
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
    {"setECMode",       "secm",_VdecCmdECMode,        NULL,    "Vdec set EC mode",    CLI_SUPERVISOR},
    {"setSyncStc",      "ss",    _VdecCmdSyncStc,    NULL,   "Vdec set sync stc",    CLI_SUPERVISOR},
    {"query",              "q",     _VdecCmdQuery,          NULL,   "Vdec query info",    CLI_SUPERVISOR},
    {"rstTimeProfile",    "r",      _VdecCmdRstProfile,  NULL,   "Vdec rst time profile",    CLI_SUPERVISOR},
    {"setDramUsage",   "d",      _VdecCmdDramUsage,  NULL,   "Vdec dram usage",    CLI_SUPERVISOR},    
    {"capture",   "c",      _VdecCmdCapture,  NULL,   "Vdec capture",    CLI_SUPERVISOR},  
    {"bigb",   "bb",      _VdecCmdBigB,  NULL,   "Vdec set B parameter",    CLI_SUPERVISOR},      
    {"setDramBWSimu","ds",   _VdecCmdDramBWSimu,    NULL,    "Vdec set dram bandwidth simulation",    CLI_SUPERVISOR},
    {"setMid","mid",   _VdecCmdMemProtect,    NULL,    "Vdec mid protection",    CLI_SUPERVISOR},    
#ifdef CC_EMULATION
    {"sendPattern",    "sp",    _VdecCmdSendPattern,    NULL,    "Vdec send pattern",    CLI_SUPERVISOR},
    {"saveDecOut",    "so",    _VdecCmdSaveDecOut,    NULL,    "Vdec save dec out data",    CLI_SUPERVISOR},
    {"saveSimPat",     "ssp",   _VdecCmdSaveSimPat,    NULL,    "Vdec save sim pat",    CLI_SUPERVISOR},
    {"setTestMode",   "stm",   _VdecCmdSetTestMode,    NULL,    "Vdec set test mode",    CLI_SUPERVISOR},
    {"setRepeatFrm",   "srf",    _VdecCmdSetRepeatFrm,    NULL,    "Vdec set repeat frame",    CLI_SUPERVISOR},
    {"checksumEn",   "cs",    _VdecCmdChkSumEn,    NULL,    "Vdec checksum enable",    CLI_SUPERVISOR},
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
#ifdef CC_EMULATION
static BOOL _bIsFirstFrm = TRUE;

static char _acFileDir[MAX_FILE_NUM][MAX_FILENAME_LEN];
static char _acGoldPrefix[MAX_FILE_NUM][MAX_FILENAME_LEN];
static UINT32 _u4CurFileIdx = 0, _u4CurFrmIdx = 0;
static INT32 _i4SSimPatFrmNo = -1;
static UINT8* _pu1GoldenData = NULL;
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
static char* _pcChkSum = NULL;
static INT32 _i4LoopCnt = 0;
//emulation, compare switch, repeat frame
static BOOL _bCmpFrmEn[1000];
static BOOL _bRptFrmEn;
static UINT32 _u4RptFrmNo;

UINT8 gu1VdecLogSimPat = 0;

static char _acTestBitstream[MAX_FILE_NUM][MAX_FILENAME_LEN];
static BOOL _afgLoadCompData[MAX_FILE_NUM];
static BOOL _fgLoadFail = FALSE;
static UINT32 _au4StartCompareFrameNum[MAX_FILE_NUM];
static UINT32 _au4PicWidth[MAX_FILE_NUM];
static UINT32 _au4PicHeight[MAX_FILE_NUM];
static UINT32 _u4FileCount = 0, _u4DecFrmIdx = 0;
static UINT32 _u4DecCycles = 0;
static char _acFilePrefix[MAX_FILE_NUM][MAX_FILENAME_LEN];

//wmv
static UINT32 bitstream_sz[MAX_FILE_NUM];
static CHAR* _pcStartCodeFile=NULL;
static char golden_path[MAX_FILE_NUM][1024];
int _vlcreg[250];
int _mcreg[250];
UINT32 m_dwBitsFSz;
//end wmv

char _acFileName[MAX_FILENAME_LEN];
char _acChkSumFile[MAX_FILENAME_LEN];
static BOOL _fgEnChkSum = FALSE;
static VDEC_CHKSUM_INFO_T _rCSInfo;

#ifdef WMV_PROFILING
static HAL_TIME_T rTimeRstS;
static HAL_TIME_T rTimeRstE;
static HAL_TIME_T rTimeRstDt; 
#endif

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

static INT32 _VdecCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{   
    UNUSED(i4Argc);
    UNUSED(szArgv);

    #ifdef CC_EMULATION
        if(_pcFileInfo == NULL)
        {
            _pcFileInfo = (char*)BSP_AllocAlignedDmaMemory(FILE_INFO_SIZE, 1024);
        }
        if(_pcChkSum == NULL)
        {
            _pcChkSum = (char*)BSP_AllocAlignedDmaMemory(CHKSUM_INFO_SIZE, 1024);
        }
        if(_pu1GoldenData == NULL)
        {
            _pu1GoldenData = (UINT8*)BSP_AllocAlignedDmaMemory(GOLDEN_SIZE, 1024);
            #ifdef RR_TEST
            _pu1GoldenDataC = (UINT8*)BSP_AllocAlignedDmaMemory(GOLDEN_SIZE, 1024);
            #endif
        }
        #ifdef CC_64x32BLK_CMP
        if(_pu1DecData16x32 == NULL)
        {
            _pu1DecData16x32 = (UINT8*)BSP_AllocAlignedDmaMemory(GOLDEN_SIZE, 1024);
        }
        #endif
        if(_pcStartCodeFile==NULL)
        {
            _pcStartCodeFile = (char*)BSP_AllocAlignedDmaMemory(FILE_INFO_SIZE, 1024);
        }
        #ifdef IDE_READ_SUPPORT
        _DmxIdeReset();
        #endif
        VERIFY(x_sema_create(&_hVdecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    #endif

    VERIFY(x_sema_create(&_hVdecCapSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    VDEC_Init();
    
    return 0;
}


static INT32 _VdecCmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);
    if (i4Argc > 2)
    {
        _eFmt = (ENUM_VDEC_FMT_T)StrToInt(szArgv[2]);   
        if(_eFmt >= VDEC_FMT_MAX)
        {
            LOG(0, "vdec.p esid {0} fmt {0:MPV, 1:MPEG4, 2:H.264, 3:WMV, 4:H.264Verification, 5:MJPG, 6:RV}");
            return -1;
        }
    }
    else
    {
        LOG(0, "vdec.p esid {0} fmt {0:MPV, 1:MPEG4, 2:H.264, 3:WMV, 4:H.264Verification, 5:MJPG, 6:RV}");
        return 0;
    }

    #ifdef CC_EMULATION
        switch(_eFmt)
        {
            case VDEC_FMT_H264: case VDEC_FMT_H264VER:
                VDEC_RegEventNotifyFunc(_VdecNotify);
                break;
            case VDEC_FMT_MP4:
                VDEC_RegEventNotifyFunc(_VdecMPEG4Notify);
                break;
            case VDEC_FMT_WMV:
                VDEC_RegEventNotifyFunc(_VdecWMVNotify);
                break;
            case VDEC_FMT_RV:
                VDEC_RegEventNotifyFunc(_VdecRVNotify);
                break;
            default: //VDEC_FMT_MPV
                break;
        }
    #endif
    
    if(VDEC_Play(0, _eFmt))
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
    UINT32 u4Param1=0, u4Param2=0, u4Param3=0;
    
    ASSERT(szArgv != NULL);    
    if(i4Argc < 4)
    {
        LOG(0, "vdec.srr [enable 1, enable without wrapper 2][Y:0/C:1/MaxHband:2][Ratio (1/2):0, (3/4):1, bypass:2]\n");
        if(i4Argc > 2)
        {
            u4Param2 = StrToInt(szArgv[2]);
        }
        VDEC_GetParam(ES0, (UINT32)RRMODE, &u4Param1, &u4Param2, &u4Param3);
        LOG(0, "vdec.srr %d %d %d\n", u4Param1, u4Param2, u4Param3);
        return 0;
    }
    VDEC_SetParam(ES0, (UINT32)RRMODE, StrToInt(szArgv[1]), StrToInt(szArgv[2]), StrToInt(szArgv[3]));

    if(StrToInt(szArgv[2])==0)            //Y
    {
        _u4YRREn = StrToInt(szArgv[1]);
        if(_u4YRREn > 0)
        {
            _u4YRRRatio = StrToInt(szArgv[3]);
        }
    }
    else if(StrToInt(szArgv[2])==1)        //C
    {
        _u4CRREn = StrToInt(szArgv[1]);
        if(_u4CRREn > 0)
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
    UINT32 u4Param1=0, u4Param2=0, u4Param3=0, u4Param4=0, u4Param5=0, u4Param6=0;
    
    ASSERT(szArgv != NULL);
    if(i4Argc < 4)
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
    UINT64 au8DramUsage[5], u4DramClockCycle;
    INT32 idx;
    VDEC_HDR_INFO_T rHdrInfo;

    if(!VDEC_QueryInfo((UCHAR)ES0, &rHdrInfo))
    {
        return 1;
    }

    x_memset(au8DramUsage, 0, sizeof(au8DramUsage));
    
    if(!rHdrInfo.fgHdrInvalid)
    {
        LOG(0, "Video decoder: ");
        switch(rHdrInfo.eCodecType)
        {
            case VDEC_FMT_H264:
            case VDEC_FMT_H264VER:
                LOG(0, "H.264, ");
                if(rHdrInfo.u4Profile == 66)
                {
                    LOG(0, "BaseLine ");
                }
                else if(rHdrInfo.u4Profile == 77)
                {
                    LOG(0, "Main ");
                }
                else if(rHdrInfo.u4Profile == 88)
                {
                    LOG(0, "Extended ");
                }
                else if(rHdrInfo.u4Profile == 100)
                {
                    LOG(0, "High ");
                }
                LOG(0, "Profile, Level %d.%d, ", rHdrInfo.u4Level/10, rHdrInfo.u4Level%10);
                if(rHdrInfo.u4IsCABAC > 0)
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
            default:
                LOG(0, "UnKnown\n");
                break;
        }

        LOG(0, "State: ");
        switch(rHdrInfo.u1CurState)
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
        if(rHdrInfo.fgProgressiveSeq)
        {
            LOG(0, "p ");
        }
        else
        {
            LOG(0, "i ");
        }
        LOG(0, "%d.%dfps\n", (rHdrInfo.u2FrmRate/100), (rHdrInfo.u2FrmRate%100));

        //es fifo related
        LOG(0, "EsFifoCnt: (Cur,Max) = (%d, %d)\n", rHdrInfo.u4CurEsCnt, rHdrInfo.u4MaxEsCnt);
        //fbm related
        LOG(0, "FbNum: %d\n", rHdrInfo.ucFbNum);
        
        rHdrInfo.au4DecTime[0][3] = (rHdrInfo.au4DecTime[0][3] == 0)?1:rHdrInfo.au4DecTime[0][3];
        rHdrInfo.au4DecTime[1][3] = (rHdrInfo.au4DecTime[1][3] == 0)?1:rHdrInfo.au4DecTime[1][3];
        rHdrInfo.au4DecTime[2][3] = (rHdrInfo.au4DecTime[2][3] == 0)?1:rHdrInfo.au4DecTime[2][3];
        u4DramClockCycle = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
        if(_u1DramMoniter == 0)
        {
            for(idx = 2; idx >=0; idx--)
            {
                au8DramUsage[idx] = 0;
            }
            LOG(0, "Time Profile:\n\tPicType(MinHwDec, AvgHwDec, MaxHwDec, SkipFrm/TotalFrm, MaxSwDec, MaxDramCycle(-))");
        }
        else if(_u1DramMoniter == 1) //total percentage
        {
            #ifdef VDEC_TIME_PROFILE
            for(idx = 2; idx >=0; idx--)
            {
                if((UINT64)rHdrInfo.au4DecTime[idx][0] > 0)
                {
                    au8DramUsage[idx] = (((UINT64)rHdrInfo.au4DecTime[idx][8] * 100000) / (UINT64)rHdrInfo.au4DecTime[idx][0])/(UINT64)u4DramClockCycle;
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
        else if(_u1DramMoniter == 2) //MPEG Mbyte
        {
            #ifdef VDEC_TIME_PROFILE
            for(idx = 2; idx >=0; idx--)
            {
                au8DramUsage[idx] = (((UINT64)rHdrInfo.au4DecTime[idx][8] * 16) * (UINT64)rHdrInfo.u2FrmRate)/100;
            }
            #endif
            LOG(0, "Time Profile:\n\tPicType(MinHwDec, AvgHwDec, MaxHwDec, SkipFrm/TotalFrm, MaxSwDec, MaxDramCycle(Mpeg Byte))");
        }
        LOG(0, "\n\tI(0.%03us, 0.%03us/%d, 0.%03us/%d, %d/%d, 0.%06us, %d)",
            rHdrInfo.au4DecTime[2][1], rHdrInfo.au4DecTime[2][2]/rHdrInfo.au4DecTime[2][3], (rHdrInfo.au4DecTime[2][6]/rHdrInfo.au4DecTime[2][3])*10000,
            rHdrInfo.au4DecTime[2][0], rHdrInfo.au4DecTime[2][7], 
            rHdrInfo.au4DecTime[2][4], rHdrInfo.au4DecTime[2][3], rHdrInfo.au4DecTime[2][5],
            au8DramUsage[2]);
        LOG(0, "\n\tP(0.%03us, 0.%03us/%d, 0.%03us/%d, %d/%d, 0.%06us, %d)",
            rHdrInfo.au4DecTime[0][1], rHdrInfo.au4DecTime[0][2]/rHdrInfo.au4DecTime[0][3], (rHdrInfo.au4DecTime[0][6]/rHdrInfo.au4DecTime[0][3])*10000,
            rHdrInfo.au4DecTime[0][0], rHdrInfo.au4DecTime[0][7], 
            rHdrInfo.au4DecTime[0][4], rHdrInfo.au4DecTime[0][3], rHdrInfo.au4DecTime[2][5],
            au8DramUsage[0]);
        LOG(0, "\n\tB(0.%03us, 0.%03us/%d, 0.%03us/%d, %d/%d, 0.%06us, %d)\n", 
            rHdrInfo.au4DecTime[1][1], rHdrInfo.au4DecTime[1][2]/rHdrInfo.au4DecTime[1][3], (rHdrInfo.au4DecTime[1][6]/rHdrInfo.au4DecTime[1][3])*10000,
            rHdrInfo.au4DecTime[1][0], rHdrInfo.au4DecTime[1][7], 
            rHdrInfo.au4DecTime[1][4], rHdrInfo.au4DecTime[1][3], rHdrInfo.au4DecTime[1][5],
            au8DramUsage[1]);
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
    if(VDP_SetFreeze(0, 0) == 0)
    {
        LOG(1, "Video freeze Fail!");
    }
    
    if(u4Cond == (UINT32)VDEC_COND_THUMBNAIL_DONE)
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

    if(pu1CapBuf == NULL)
    {
        pu1CapBuf = (UINT8*)BSP_AllocAlignedDmaMemory(200*200*4, 1024);
    }
    rCapInfo.pucCanvasBuffer = pu1CapBuf;

    u4ColorMode = StrToInt(szArgv[1]);
    if(u4ColorMode == 0)
    {    
        rCapInfo.eCanvasColormode = VDP_COLORMODE_AYUV_D8888;
    }
    else if(u4ColorMode == 1)
    {
        rCapInfo.eCanvasColormode = VDP_COLORMODE_ARGB_D8888;
    }
    else if(u4ColorMode == 2)
    {
        rCapInfo.eCanvasColormode = VDP_COLORMODE_ARGB_D4444;
    }
    else
    {
        LOG(0, "vdec.c [Color Mode, 0: AYUV_8888, 1: ARGB_8888, 2: ARGB_4444]\n");    
        return 0;
    }
    rCapInfo.u4CanvasWidth = 200;
    rCapInfo.u4CanvasHeight= 200;
    rCapInfo.u4ThumbnailWidth = 200;
    rCapInfo.u4ThumbnailHeight= 200;
    rCapInfo.u4ThumbnailX = 0;
    rCapInfo.u4ThumbnailY= 0;
    
    VDEC_RegEventNotifyFunc(_VdecCapNotify);
    if(VDP_SetFreeze(0, 1) == 0)    //use vdpID 0 arbitrary
    {
        LOG(1, "Video freeze Fail!");
        return 0;
    }
    UNUSED(VDP_GetLockFrameBufferAddr(0, &rVdpCapInfo));
    if((rVdpCapInfo.u4AddrY == 0) || (rVdpCapInfo.u4AddrC== 0) || (rVdpCapInfo.u4RRMode > 1))
    {
        LOG(1, "Video Capture Fail!");
        return 0;
    }

    bIsOnePass = (rCapInfo.eCanvasColormode == VDP_COLORMODE_AYUV_D8888) ? TRUE : FALSE;

    HAL_GetTime(&rTimeS);

    UNUSED(VDEC_GetVideoThumbnail(ES0, 0, bIsOnePass, &rCapInfo, &rVdpCapInfo));
    
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
    while(pcToken!= NULL)
    {
        u4Val = StrToInt(pcToken);
        switch(u4CntPerFrm)
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
        if(u4CntPerFrm == CS_PER_FRM)
        {
            u4FrmNum++;
            u4CntPerFrm = 0;
        }
        pcToken = strtok(NULL, "\r\n");
    }

    return u4FrmNum;
}

void _VdecNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4) //for h264
{
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    static UINT8 u1PicStruct = 0;
    static UINT32 u4YAddr = 0;
    BOOL bIsCmpPass = TRUE, bFileLoad = FALSE;

    #ifdef IDE_READ_SUPPORT
    UINT32 u4IDERetryCnt = 0;
    #endif      
    
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)u4Arg2;
    ASSERT(prDispInfo!=NULL);

    if(!_bSaveOutput)    //normal comparing verification
    {
        if(u4Arg1 == (UINT32)VDEC_EVT_DISP_READY)
        {
            #ifndef RR_TEST
            if(((UINT32)prDispInfo->pvYAddr != u4YAddr) && (!_bIsFirstFrm))
            {
                _u4CurFrmIdx++;
            }
            _bIsFirstFrm = FALSE;
            u4YAddr = (UINT32)prDispInfo->pvYAddr;

            if(_bCmpEn && _bCmpFrmEn[_u4CurFrmIdx])
            {  
                //compare decoded golden data: Y
                if((_u4YRREn>0) || (_u4CRREn>0))
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
                    if(!_fgEnChkSum)
                    {
                        bFileLoad = _LoadFile(_acFileName, (UINT32)_pu1GoldenData);
                    }
                    if(_fgEnChkSum || bFileLoad)
                    {
                        #ifdef CC_64x32BLK_CMP
                        _VdecBlkTransform(_pu1DecData16x32, prDispInfo, TRUE);
                        #endif
                        bIsCmpPass = _VdecVerifyDecData(prDispInfo, TRUE);
                    }
                #endif
                   
                //compare decoded golden data: C
                if(prDispInfo->u4ChromaFormatIdc > 0)
                {
                    if((_u4YRREn>0) || (_u4CRREn>0))
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
                        if(!_fgEnChkSum)
                        {
                            bFileLoad = _LoadFile(_acFileName, (UINT32)_pu1GoldenData);
                        }                        
                        if(_fgEnChkSum || bFileLoad)
                        {
                            #ifdef CC_64x32BLK_CMP
                            _VdecBlkTransform(_pu1DecData16x32, prDispInfo, FALSE);
                            #endif
                            bIsCmpPass &= _VdecVerifyDecData(prDispInfo, FALSE);
                        }
                     #endif
                }
               
                if(!bIsCmpPass)
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
            if(u1PicStruct == FRAME)            //wait for both top and bottom field
            {
                #ifndef RR_TEST
                //if(!((_bRptFrmEn) && (_u4RptFrmNo==_i4CurFrmIdx)))
                //{
                //    _i4CurFrmIdx++;
                //}
                #endif
                u1PicStruct = 0;        
            }

            if((_bRptFrmEn) && (_u4RptFrmNo==_u4CurFrmIdx))
            {
                VDEC_SetParam(ES0, REPEAT_FRM, 1, 0, 0);
            }
            else
            {
                VDEC_SetParam(ES0, REPEAT_FRM, 0, 0, 0);    //repeat frame disable
            }                    
        }
        else if(u4Arg1 == (UINT32)VDEC_EVT_DEC_BS_END)
        {
            VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
        }
    }
    else if (_bSaveOutput)                    //save to PC
    {
        UINT32 u4StartAddr;
        UINT32 u4EndAddr;
        
        //Y
        if(u4Arg3 == 0)
        {
            _Vdec1389B2R(_pu1GoldenData, *prDispInfo, TRUE);
            u4StartAddr = (UINT32)_pu1GoldenData;
            u4EndAddr = (UINT32)_pu1GoldenData + prDispInfo->u4YSize - 1; 
        }
        else
        {
            u4StartAddr = (UINT32)prDispInfo->pvYAddr;        
            if(u4Arg4 == 0)    // 1/2 RR mode
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
        if(u4Arg3 == 0)
        {
            _Vdec1389B2R(_pu1GoldenData, *prDispInfo, FALSE);
            u4StartAddr = (UINT32)_pu1GoldenData;
            u4EndAddr = (UINT32)_pu1GoldenData + prDispInfo->u4CSize - 1; 
        }
        else
        {
            u4StartAddr = (UINT32)prDispInfo->pvCAddr;        
            if(u4Arg4 == 0)    // 1/2 RR mode
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
        if(u1PicStruct == FRAME)    //wait for both top and bottom field
        {
            _u4CurFrmIdx++;
            u1PicStruct = 0;
        }            
    }

    if(_i4SSimPatFrmNo == _u4CurFrmIdx)
    {
        gu1VdecLogSimPat = 1;
        VDEC_SetParam(ES0, LOG_SIMU_PAT, 1, 0, 0);
    }
    else
    {
        gu1VdecLogSimPat = 0;
        VDEC_SetParam(ES0, LOG_SIMU_PAT, 0, 0, 0);
    }
}


void _VdecMPEG4Notify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    char acPath[MAX_FILENAME_LEN];
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    static UINT8 u1PicStruct = 0;
    UINT32* pu4YRetVal, *pu4CbCrRetVal;

    x_memset(acPath, 0, MAX_FILENAME_LEN);
    
    if(u4Arg1 == (UINT32)VDEC_EVT_DISP_READY)
    {

        prDispInfo = (VDEC_DISP_PIC_INFO_T*)u4Arg2;
        ASSERT(prDispInfo!=NULL);
        /*pu4YRetVal = (UINT32*)u4Arg3;
        ASSERT(pu4YRetVal!=NULL);
        pu4CbCrRetVal = (UINT32*)u4Arg4;
        ASSERT(pu4CbCrRetVal!=NULL);

        *pu4YRetVal = 1;
        *pu4CbCrRetVal = 1;*/

        if (_u4CurFrmIdx >= _au4StartCompareFrameNum[_u4CurFileIdx]
            /*&& _u4CurFrmIdx <= _au4FrameNumber[_u4CurFileIdx]*/)
        {
            x_memset(acPath, 0, MAX_FILENAME_LEN);
            sprintf(acPath, "%s_%d_Y.out",
                _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);

            //compare decoded golden data: Y
            /*UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s_%d_Y.out\" 0x%08x /ny", 
                _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData));*/
            LOG(0, "d.load.binary %s 0x%08x--0x%08x /ny\n", acPath, (UINT32)_pu1GoldenData, 
                (UINT32)(_pu1GoldenData + GOLDEN_SIZE));

            if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
            {
                //*pu4YRetVal = _VdecVerifyDecData(prDispInfo, TRUE);
                _VdecVerifyDecData(prDispInfo, TRUE);
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
            
            sprintf(acPath, "%s_%d_CbCr.out",
                _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx);
            //compare decoded golden data: C
            /*UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s_%d_CbCr.out\" 0x%08x /ny", 
                _acFileDir[_u4CurFileIdx], _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)_pu1GoldenData));*/
            LOG(0, "d.load.binary %s to 0x%08x--0x%08x /ny \n", acPath, (UINT32)_pu1GoldenData, 
                (UINT32)(_pu1GoldenData + GOLDEN_SIZE));

            if (_LoadFile(acPath, (UINT32)_pu1GoldenData))
            {
                //*pu4CbCrRetVal = _VdecVerifyDecData(prDispInfo, FALSE);
                _VdecVerifyDecData(prDispInfo, FALSE);
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


        /*UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s%s_%d_Y.out\" 0x%08x--0x%08x", 
        "c:\\", _acGoldPrefix[_u4CurFileIdx], _u4CurFrmIdx, (UINT32)prDispInfo->pvYAddr, (UINT32)(prDispInfo->pvYAddr + prDispInfo->u4YSize)));
        LOG(0, "prDispInfo->u4YSize = %ld\n", prDispInfo->u4YSize);*/
        
        u1PicStruct |= prDispInfo->u1PicStruct;
        //if(prDispInfo->u1PicStruct == FRAME)
        {
            _u4CurFrmIdx++;
            //_u4DecFrmIdx++;
            u1PicStruct = 0;
        }
    }
    else if(u4Arg1 == (UINT32)VDEC_EVT_DEC_BS_END)
    {
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_MP4);
    }
}


void _VdecWMVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UINT32 dwWidth, dwHeight;
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    UINT8* golden_dataY = _pu1GoldenData;
    UINT8* golden_dataC = _pu1GoldenData;
    UINT8* hw_output;
    UINT32 i=0;
    UINT32 u4X, u4Y;
    UINT8* pu1GoldenAddr;
    UINT8* pu1HWAddr;
    INT8 fgY;
    UINT32 CurFrame;
    UINT32 u4CS1 = 0;
    UINT32 u4CS2 = 0;
    
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)(u4Arg2);
    //LOG(0,"entering my_save_pic\n");
    CurFrame = prDispInfo->u4CurrentPicNo-1;
    //LOG(0,"frame idx : %d %d\n",CurFrame,_u4CurFrmIdx);

    //Printf("src decode resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthDec,rWMVDecInfo.rSeqHdrR.u4PicHeightDec);
    //Printf("src cmp resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthCmp,rWMVDecInfo.rSeqHdrR.u4PicHeightCmp);
    //Printf("src resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthSrc,rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);
    //Printf("rWMVDecInfo.rOther.rPicInfo src resolution : %dx%d\n",rWMVDecInfo.rOther.rPicInfo.dwW,rWMVDecInfo.rOther.rPicInfo.dwH);

    if(u4Arg1 == VDEC_EVT_DEC_BS_END) 
    {
        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_WMV);
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
    }

    else if(u4Arg1 == VDEC_EVT_DISP_READY) 
    {
        for(fgY =1 ; fgY >=0 ; fgY--) 
        {
        if(fgY) 
        {
        
            dwWidth = prDispInfo->u4W;
            dwHeight = prDispInfo->u4H;
            hw_output = prDispInfo->pvYAddr;
            
            if(dwWidth%16) 
            {
                dwWidth = ((dwWidth / 16) + 1) * 16; //ming modify@2006/4/11
            }
            if(dwHeight % 32) 
            {
                dwHeight = ((dwHeight / 32) + 1) * 32;
            }

            //dwSz = dwWidth * dwHeight;
            //LOG(0,"hw y address : %x\n",hw_output);
            //LOG(0,"Y %d size :%d, %dx%d\n", CurFrame/*_u4CurFrmIdx*/, prDispInfo->u4YSize, prDispInfo->u4W, prDispInfo->u4H);
#ifdef DO_COMPARE_FRAME
            if((CurFrame >= START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
            {
                if(!_fgEnChkSum)
                {
                    sprintf(_acFileName, "%s/%s_%d_Y.out", golden_path[_u4CurFileIdx],_acGoldPrefix[_u4CurFileIdx],CurFrame/*_u4CurFrmIdx*/);
                    _LoadFile(_acFileName, golden_dataY);
                }
                else
                {
                    u4CS1 = 0;
                    u4CS2 = 0;
                    //printf("compare frame %d Y in checksum\n",CurFrame);
                }
                for(u4Y = 0; u4Y < prDispInfo->u4H_Cmp; u4Y++)
                {
                    for(u4X = 0; u4X < prDispInfo->u4W_Cmp; u4X++)
                    {                      
                        pu1HWAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)hw_output, u4X, u4Y, dwWidth, TRUE, 4);
                        if(_fgEnChkSum)
                        {
                            u4CS1 += (*pu1HWAddr);
        	                u4CS2 += 2*(*pu1HWAddr);    
                        }
                        else
                        {
                            pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)golden_dataY, u4X, u4Y, dwWidth/*720*/, TRUE, 3);
                            if(*pu1GoldenAddr != *pu1HWAddr)
                            {
                                printf("=== %s dif in Y(x%d,y%d), golden(@0x%x=0x%x),hw (@0x%x=0x%x)\n",
                                    _acGoldPrefix[_u4CurFileIdx], u4X, u4Y, (UINT32)pu1GoldenAddr, *pu1GoldenAddr, 
                                    (UINT32)pu1HWAddr, *pu1HWAddr);
                                //print_checksum(1);
                                //Dump_Mem(prDispInfo);
                                return FALSE;
                            }
                        }
                    }
                }
                if(_fgEnChkSum)
                {
                    //compare with checksum
                    if(u4CS1 !=_rCSInfo.u4ShftAddY1[CurFrame] || u4CS2 !=_rCSInfo.u4ShftAddY2[CurFrame])
                    {
                        printf("=== %s checksum dif in frame %d Y\n",_acGoldPrefix[_u4CurFileIdx],CurFrame);
                        ASSERT(0);
                        return FALSE;
                    }
                    else
                    {
                        printf("compare frame %d Y in checksum ok!!\n",CurFrame);
                    }
                }
                printf("Cmp Y size %d\n", u4X * u4Y);
            }
#endif
        }
    
        else 
        {

            dwWidth = prDispInfo->u4W;
            dwHeight = prDispInfo->u4H/2;
            hw_output = prDispInfo->pvCAddr;
            if(dwWidth%16)
            {
                dwWidth = ((dwWidth / 16) + 1) * 16; //ming modify@2006/4/11
            }
                
            if(dwHeight % 16)
            {
                dwHeight = ((dwHeight / 16) + 1) * 16;           
            }

            //printf("hw Cbcr address : %x\n",hw_output);
            //printf("CbCr %d size :%d\n", CurFrame/*_u4CurFrmIdx*/, prDispInfo->u4CSize);        
#ifdef DO_COMPARE_FRAME
            if(CurFrame >= (START_COMPARE_FRAME) && (CurFrame<=END_COMPARE_FRAME))
            {
                if(!_fgEnChkSum)
                {
                    sprintf(_acFileName, "%s/%s_%d_CbCr.out", golden_path[_u4CurFileIdx],_acGoldPrefix[_u4CurFileIdx],CurFrame/*_u4CurFrmIdx*/);
                    _LoadFile(_acFileName, golden_dataC);
                }
                else
                {    
                    u4CS1 = 0;
                    u4CS2 = 0;
                }
                
                for(u4Y = 0; u4Y < (prDispInfo->u4H_Cmp>>1)/*(rWMVDecInfo.rOther.rPicInfo.dwH>>1)*/; u4Y++)
                {
                    for(u4X = 0; u4X < prDispInfo->u4W_Cmp/*rWMVDecInfo.rOther.rPicInfo.dwW*/; u4X++)
                    {                        
                        pu1HWAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)hw_output, u4X, u4Y, dwWidth, TRUE, TRUE, 4);
                        if(_fgEnChkSum)
                        {
                            u4CS1 += (*pu1HWAddr);
        	                u4CS2 += 2*(*pu1HWAddr);
                        }
                        else
                        {
                            pu1GoldenAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)golden_dataC, u4X, u4Y, dwWidth/*720*/, TRUE, TRUE, 3);
                            if(*pu1GoldenAddr != *pu1HWAddr)
                            {
                                printf("=== %s dif in C(x%d,y%d), golden(@0x%x=0x%x), hw(@0x%x=0x%x)\n",
                                    _acGoldPrefix[_u4CurFileIdx], u4X, u4Y, (UINT32)pu1GoldenAddr, *pu1GoldenAddr, 
                                    (UINT32)pu1HWAddr, *pu1HWAddr);
                                //print_checksum(1);
                                //Dump_Mem(prDispInfo);
                                return FALSE;                
                            }
                        }
                    }
                }
                if(_fgEnChkSum)
                {
                    //compare checksum for vc-1 decoder
                    if(u4CS1 !=_rCSInfo.u4ShftAddC1[CurFrame] || u4CS2 !=_rCSInfo.u4ShftAddC2[CurFrame])
                    {
                        printf("=== %s checksum dif in frame %d CbCr\n",_acGoldPrefix[_u4CurFileIdx],CurFrame);
                        ASSERT(0);
                        return FALSE;
                    }
                    else
                    {
                        printf("compare frame %d CbCr in checsum ok!!\n",CurFrame);
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


#ifdef RV_TEST

#define GOLDEN_BASE_DIR  "r:\\rm_emu\\golden"
#define PIXEL_ERR_LOG_CNT     10       // max number of log lines printed when pixel error
#define DRAM_NONCACHE_OFFSET 0//0x30000000

static UINT32 _u4ErrPos;
static BOOL _fgShowDiff = TRUE;
static UINT32 _u4SumTotal = 0;
static UINT32 _u4SumErrCnt = 0;

static UINT32 mb_to_line_y(UINT32 buf, int width, int x, int y)
{
  UINT32 val = 0;
  int mb_x;
  int mb_y;
  int offset_x;
  int offset_y;
  int offset;

  mb_x = x >> 4;
  mb_y = y >> 4;

  offset_x = x % 16;
  offset_y = y % 32;

/*
  offset = offset_x + 
           offset_y * 16 +
           mb_x * 16 * 32 +
           (mb_y >> 1) * width * 32;
*/
  offset = offset_x + 
           (offset_y << 4) +
           (mb_x << 9) +
           (((mb_y >> 1) * width) << 5);
           
  val = *(UINT32 *)(DRAM_NONCACHE_OFFSET + buf + offset);
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

  mb_x = x >> 3;
  mb_y = y >> 3;

  offset_x = x % 8;
  offset_y = y % 16;

/*
  offset = offset_x * 2 + 
           offset_y * 16 +
           mb_x * 16 * 16 +
           (mb_y >> 1) * width * 16;
*/
  offset = (offset_x << 1) + 
           (offset_y << 4) +
           (mb_x << 8) +
           (((mb_y >> 1) * width) << 4);

  *pu4Val1 = *(UINT32 *)(DRAM_NONCACHE_OFFSET + buf + offset);
  *pu4Val2 = *(UINT32 *)(DRAM_NONCACHE_OFFSET + buf + offset + 4);
  _u4ErrPos = (UINT32)buf + offset;
}

static BOOL fgCompareGold(UINT32 n, char *szDir, UINT32 u4DecWidth, UINT32 u4DecHeight, UINT32 u4Width, UINT32 u4Height, BOOL fgFirstTime, UINT32 u4OutBufY, UINT32 u4OutBufC)
{
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
  int ideRetryCnt;
  UINT32 GOLD_ADDR = (UINT32)_pu1GoldenData;

#if 0
  if (_fgCmpOnPC)
  {
    UINT32 u4NewHeight = u4DecHeight / 16;
    UINT32 u4Sz;
    BOOL fgRet;
    
    if (u4NewHeight % 2)
      u4NewHeight = u4NewHeight * 16 + 16;
    else
      u4NewHeight = u4NewHeight * 16;
    u4Sz = u4DecWidth * u4NewHeight;
    
    _DmxIdeSendDataLen(u4Sz);
    _DmxIdeSendData(u4OutBufY, u4Sz);
    _DmxIdeSendData(u4OutBufC, u4Sz >> 1);
    
    fgRet = _DmxIdeGetCmpResult();
    
    return fgRet;
  }
#endif
  
  u4GoldYSz = u4DecWidth * u4DecHeight;
  u4GoldYUSz = u4GoldYSz + u4GoldYSz / 4;
  
  ideRetryCnt = 0;

CMP_BEGIN:
  fgYOK = fgUOK = fgVOK = TRUE;
  
  if (fgFirstTime || ideRetryCnt > 0)
  {
#if 0
    if (_fgFileFromIDE)
    {
#ifndef MB_GOLDEN
      _DmxIdeGetFile(GOLD_ADDR, 6);
#else
      _DmxIdeGetFile(GOLD_ADDR, 4);
      _DmxIdeGetFile(GOLD_ADDR + u4GoldYSz, 5);
#endif
    }
    else
#endif
    {
      char szCmd[150];

      sprintf(szCmd, "d.load.binary \"%s\\g%07d.yuv\" 0x%08X /ny", szDir, n, GOLD_ADDR);
      printf("%s\n", szCmd);
      UNUSED(T32_HostExec(E_T32_EXEC, szCmd));
    }
  }
  
  u4ErrCntY = u4Total = 0;
  for (y = 0; y < u4Height; y++)
  {
    for (x = 0; x < u4Width; x += 4)
    {
#ifndef MB_GOLDEN
      u4ErrVal = mb_to_line_y(u4OutBufY, u4DecWidth, x, y);
#else
      _u4ErrPos = u4OutBufY + y * u4DecWidth + x;
      u4ErrVal = *(UINT32 *)(DRAM_NONCACHE_OFFSET + _u4ErrPos);
#endif
      u4GolVal = *(UINT32 *)(GOLD_ADDR + y * u4DecWidth + x);
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
#ifndef MB_GOLDEN
  for (y = 0; y < u4Height/2; y++)
  {
    for (x = 0; x < u4Width/2; x += 4)
    {
/*      
      u4ErrVal = mb_to_line_c(u4OutBufC, u4DecWidth, x, y);
      u2UVal = *(UINT16 *)(GOLD_ADDR + 1920*1088 + y * u4DecWidth / 2 + x);
      u2VVal = *(UINT16 *)(GOLD_ADDR + 1920*1088 + 1920*1088/4 + y * u4DecWidth / 2 + x);
      u4GolVal = ((u2VVal << 16) & 0xFF000000) | ((u2UVal << 8) & 0x00FF0000) |
                 ((u2VVal << 8) & 0x0000FF00) | ((u2UVal) & 0x000000FF);
*/
      u4UVal = *(UINT32 *)(GOLD_ADDR + u4GoldYSz  + y * u4DecWidth / 2 + x);
      u4VVal = *(UINT32 *)(GOLD_ADDR + u4GoldYUSz + y * u4DecWidth / 2 + x);

      mb_to_line_c(&u4ErrVal, &u4ErrVal2, u4OutBufC, u4DecWidth, x, y);
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

      u4GolVal2 = ((u4VVal) & 0xFF000000)      | ((u4UVal >>  8) & 0x00FF0000) |
                  ((u4VVal >> 8) & 0x0000FF00) | ((u4UVal >> 16) & 0x000000FF);
                 
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
#else
  for (y = 0; y < u4Height/2; y++)
  {
    for (x = 0; x < u4Width; x += 4)
    {
      _u4ErrPos = u4OutBufC + y * u4DecWidth + x;
      u4ErrVal = *(UINT32 *)(DRAM_NONCACHE_OFFSET + _u4ErrPos);
      u4GolVal = *(UINT32 *)(GOLD_ADDR + u4GoldYSz + y * u4DecWidth + x);

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
            printf("c(%4d,%4d) (m,p)(0x%08X,0x%08X) ", x, y, _u4ErrPos, u4OutBufC + y * u4DecWidth + x);
            printf("(0x%08X,0x%08X)\n", u4ErrVal, u4GolVal);
          }
        }
        fgUOK = fgVOK = FALSE;
      }
    }
  }
#endif

  if (!fgUOK || !fgVOK)
    printf("Error count c: %d/%d\n", u4ErrCntC, u4Total/2);

  fgRet = fgYOK && fgUOK && fgVOK;
  
#if 0
  if (_fgFileFromIDE && !fgRet && ideRetryCnt++ < IDE_RETRY)
  {
    printf("%s #%d possible IDE error, retry golden comparison...\n", szDir, n);
    goto CMP_BEGIN;
  }
#endif
  
  _u4SumTotal += u4Total;
  _u4SumErrCnt += u4ErrCntY;
  
  //if (n == 186) fgRet = FALSE;

  return (fgRet);
}
#endif

void _VdecRVNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    VDEC_DISP_PIC_INFO_T* prDispInfo;
    UINT32 CurFrame;
    UINT8* golden_data = _pu1GoldenData;
    UINT32 u4DecWidth, u4DecHeight;
    UINT32 u4Width, u4Height;
    
    prDispInfo = (VDEC_DISP_PIC_INFO_T*)(u4Arg2);
    CurFrame = prDispInfo->u4CurrentPicNo-1;
    LOG(0,"frame idx : %d %d\n",CurFrame,_u4CurFrmIdx);

    if(u4Arg1 == VDEC_EVT_DEC_BS_END) 
    {
        VDEC_Stop(0);
        VDEC_Init();
        VDEC_Play(0, VDEC_FMT_RV);
        VERIFY(x_sema_unlock(_hVdecSema) == OSR_OK);
    }
    else if(u4Arg1 == VDEC_EVT_DISP_READY) 
    {
#if defined(DO_COMPARE_FRAME) && defined(RV_TEST)
        u4DecWidth = prDispInfo->u4W_Cmp;
        u4DecHeight = prDispInfo->u4H_Cmp;
        u4Width = prDispInfo->u4W;
        u4Height = prDispInfo->u4H;
        if (1/*(CurFrame >= START_COMPARE_FRAME) && (CurFrame<END_COMPARE_FRAME)*/)
        {
            if (!_fgEnChkSum)
            {
                ASSERT(fgCompareGold(CurFrame, golden_path[_u4CurFileIdx], u4DecWidth, u4DecHeight, u4Width, u4Height, TRUE, (UINT32)prDispInfo->pvYAddr, (UINT32)prDispInfo->pvCAddr));
            }
            else
            {
            }
        }
#endif
        _u4CurFrmIdx++;
    }

    return;
}


static BOOL _LoadFile(const char* pcFileName, UINT32 u4Addr)
{
    #ifdef  IDE_READ_SUPPORT
    _DmxIdeSendName(pcFileName);
    if(!_DmxIdeGetFile(u4Addr, 0))
    {
        LOG(0, "--File load failed\n");
        return FALSE;
    }
    #else
    printf("//d.load.binary \"%s\" 0x%08x /ny\n", pcFileName, u4Addr);
        #ifdef CC_DRIVER_DEMO    
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pcFileName, u4Addr));
        #else
        LOG(0, "CC_DRIVER_DEMO !defined, File load fail~");
        #endif
    #endif

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
    u4PicH = (bIsYcomp) ? prDispInfo->u4H : (prDispInfo->u4H>>1);

    #ifdef CC_64x32BLK_CMP
    //64x32 block mode
    pu4DecData = _pu1DecData16x32;
    #else
    //16x32 block mode
    pu4DecData = (UINT32*)((bIsYcomp) ? prDispInfo->pvYAddr : prDispInfo->pvCAddr);
    #endif
    
    u4Residual = u4PicH % u4BlkH; // 16 or 0
    u4Pic32XSize = prDispInfo->u4W * (u4PicH - u4Residual);
    
    if(bIsYcomp && (_u4YRREn > 0))
    {
        if(_u4YRRRatio > 0)
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
    else if((!bIsYcomp) && (_u4CRREn > 0))
    {
        if(_u4CRRRatio > 0)
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
        u4DecSize = (bIsYcomp) ? prDispInfo->u4YSize: prDispInfo->u4CSize;
    }
    u4DecSize >>= 2;    //size in word
    u4Pic32XSize >>= 2;

    if((_u4YRREn > 0) || (_u4CRREn > 0))
    {        
        for(u4CmpWord = 0; u4CmpWord < u4Pic32XSize; u4CmpWord++)
        {
            //compare data
            if(_fgEnChkSum)
            {
                u4CS1 += pu4DecData[u4CmpWord];
                u4CS2 += pu4DecData[u4CmpWord];
                u4CS1 <<= 1;
                u4CS2 <<= 2;
            }
            else
            {
                if((pu4GoldData[u4CmpWord] != pu4DecData[u4CmpWord]))
                {
                    LOG(0, "//Error! %c Data Mismatch at [0x%.8x] = 0x%.2x, Golden = 0x%.2x !!! \n", 
                        (bIsYcomp)?'Y':'C', u4CmpWord, pu4DecData[u4CmpWord], pu4GoldData[u4CmpWord]);
                    #ifndef STRESS_TESTING
                    while(1);
                    #else
                    return FALSE;
                    #endif
                }
            }
        }
    }
    else
    {
        for(u4CmpWord = 0; u4CmpWord < u4DecSize; u4CmpWord++)
        {
            if((prDispInfo->u1PicStruct == BOTTOM_FIELD) && ((u4CmpWord%4) == 0) && (!((u4CmpWord>>2)%2)))
            {
                // Skip Top lines, 16x16 Block mode
                u4CmpWord += 4;    // 4*4 byte 
            }
            //compare data
            if(_fgEnChkSum)
            {
                u4CS1 += pu4DecData[u4CmpWord];
                u4CS2 += pu4DecData[u4CmpWord];
                u4CS1 <<= 1;
                u4CS2 <<= 2;
            }
            else
            {
                if((pu4GoldData[u4CmpWord] != pu4DecData[u4CmpWord]))
                {
                    LOG(0, "//Error! %c Data Mismatch at [0x%.8x] = 0x%.2x, Golden = 0x%.2x !!! \n", 
                        (bIsYcomp)?'Y':'C', u4CmpWord, pu4DecData[u4CmpWord], pu4GoldData[u4CmpWord]);
                    #ifndef STRESS_TESTING
                    while(1);
                    #else
                    return FALSE;
                    #endif
                }
            }            
            //ignore partial data of the last row if u4Residual > 0
            if(u4Residual && (u4CmpWord > u4Pic32XSize) && 
                (((u4CmpWord - u4Pic32XSize) % u4BlkFrmSize) == ((u4Residual << 2)-1)) && 
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
            if((prDispInfo->u1PicStruct == TOP_FIELD) && ((u4CmpWord%4) == 3) && (!((u4CmpWord>>2)%2)))
            {      
                // Skip bottom lines, 16x16 Block mode
                u4CmpWord += 4;    // 4*4 = 16 byte
            }
        }
    }

    if(_fgEnChkSum)
    {
        if(bIsYcomp)
        {
            u4GoldenCS1 = _rCSInfo.u4ShftAddY1[_u4CurFrmIdx];
            u4GoldenCS2 = _rCSInfo.u4ShftAddY2[_u4CurFrmIdx];
        }
        else
        {
            u4GoldenCS1 = _rCSInfo.u4ShftAddC1[_u4CurFrmIdx];
            u4GoldenCS2 = _rCSInfo.u4ShftAddC2[_u4CurFrmIdx];
        }
        if((u4GoldenCS1!= u4CS1) ||
            (u4GoldenCS2 != u4CS2))
        {
            LOG(0, "//Error! Frm %d %c ChkSum Mismatch (0x%x, golden1 0x%x), (0x%x, golden2 0x%x)\n",
                _u4CurFrmIdx,
                (bIsYcomp)?'Y':'C', 
                u4CS1, u4GoldenCS1,
                u4CS2, u4GoldenCS2);
            return FALSE;
        }
        else
        {
            LOG(0, "//Frm %d %c CS pass\n", _u4CurFrmIdx, (bIsYcomp)?'Y':'C');
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
    u4PicH = (bIsYcomp) ? prDispInfo->u4H : (prDispInfo->u4H>>1);

    //initial
    pu1DecData = (UINT8*)((bIsYcomp) ? prDispInfo->pvYAddr : prDispInfo->pvCAddr);
    apu1Dst[0] = pu1DstBlk16x32;
    for(u4BlkW16Idx = 1; u4BlkW16Idx < 4; u4BlkW16Idx++)
    {
        apu1Dst[u4BlkW16Idx] = apu1Dst[u4BlkW16Idx - 1] + (u4BlkH <<4);   //*16
    }

    //transform
    for(u4CvHByte = 0; u4CvHByte < u4PicH; u4CvHByte += u4BlkH)
    {
        for(u4CvWByte = 0; u4CvWByte < prDispInfo->u4W; u4CvWByte += 64)
        {            
            for(u4PixIdx = 0; u4PixIdx < u4BlkH; u4PixIdx++)
            {
                for(u4BlkW16Idx = 0; u4BlkW16Idx < 4; u4BlkW16Idx++)
                {
                    x_memcpy(apu1Dst[u4BlkW16Idx], pu1DecData, 16);
                    pu1DecData += 16;
                    apu1Dst[u4BlkW16Idx] += 16;
                }
            }
            //advance pointer
            for(u4BlkW16Idx = 0; u4BlkW16Idx < 4; u4BlkW16Idx++)
            {
                apu1Dst[u4BlkW16Idx] += (((u4BlkH <<4) <<1) + (u4BlkH <<4));
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
    u4PicH = (bIsYcomp) ? rDispInfo.u4H : (rDispInfo.u4H>>1);
        
    pu1DecData = (UINT8*)((bIsYcomp) ? rDispInfo.pvYAddr : rDispInfo.pvCAddr);

    for(u4CvHByte = 0; u4CvHByte < u4PicH; u4CvHByte += u4BlkH)
    {
        for(u4CvWByte = 0; u4CvWByte < rDispInfo.u4W; u4CvWByte += 16)
        {
            for(u4PixIdx = 0; u4PixIdx < u4BlkH; u4PixIdx++)
            {
                x_memcpy(pu1RasterData, pu1DecData, 16);
                pu1DecData += 16;
                pu1RasterData += rDispInfo.u4W;
            }
            pu1RasterData -= (u4BlkH * rDispInfo.u4W);
            pu1RasterData += 16;
        }
        pu1RasterData += ((u4BlkH-1) * rDispInfo.u4W);
    }
}

static INT32 _VdecCmdSendPattern(INT32 i4Argc, const CHAR ** szArgv)
{
    HANDLE_T hThread;
    ASSERT(szArgv != NULL);    
    
    if(!_bIsPatSending)
    {
        switch (_eFmt)
        {
            case VDEC_FMT_H264: case VDEC_FMT_H264VER:
                if(i4Argc < 3)
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
            case VDEC_FMT_RV:
                _i4LoopCnt = StrToInt(szArgv[1]);
                VERIFY(x_thread_create(&hThread, "PseudoDMX", 4096,
                255,  _VdecSendRVPattern, 0, NULL) == OSR_OK);
                break;
            default:
                LOG(0, "Unknown format type\n");
                break;
        }
    }

    return 0;
}


static void _VdecSendH264Pattern(void* pvArg)
{
    char* pcToken;
    UINT32 u4FileNum = 0,u4Idx = 0, u4FrmNum = 0;
    VDEC_PES_INFO_T rPesInfo;

    _bIsPatSending = TRUE;
    //parsing file name
    pcToken = strtok(_pcFileInfo, "\r\n");
    while( pcToken != NULL )
    {
        if(pcToken[0] == '#')    //skip the test case
        {
            pcToken = strtok(NULL, "\r\n");
        }
        else
        {
            strcpy(_acFileDir[u4Idx], pcToken);
            pcToken = strtok(NULL, "\r\n");
            if(pcToken != NULL)
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
    rPesInfo.u4FifoStart = VFIFO_S;    //should get from dmx fbm
    rPesInfo.u4FifoEnd = VFIFO_E;
    do
    {
        for(_u4CurFileIdx = 0; _u4CurFileIdx < u4FileNum; _u4CurFileIdx++)
        {            
            //reset frame idx
            _u4CurFrmIdx = 0;
            _bIsFirstFrm = TRUE;
            rPesInfo.ucMpvId = VLD0;
            #ifdef RR_TEST
            if(_u4YRREn)
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
            if(_u1TestSet == 1)        //SarnOff
            {
                    sprintf(_acFileName, "R:/264TS/Sarnoff/BsInfo/%s.264.info", _acGoldPrefix[_u4CurFileIdx]);
            }
            else if(_u1TestSet == 2)    //Allegro2
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
            if(_fgEnChkSum)
            {
                x_memset((void*)&_rCSInfo, 0,  sizeof(VDEC_CHKSUM_INFO_T));
                _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);
                u4FrmNum = _VdecGetChkSum(_pcChkSum, &_rCSInfo);
                LOG(1, "Chksum FrmNum %d\n", u4FrmNum);
            }            
            //parsing bitstream file name
            pcToken = strtok(_pcFileInfo, "\r\n");
            if(pcToken != NULL)
            {
                sprintf(_acFileName, "%s/%s", _acFileDir[_u4CurFileIdx], pcToken);
                #ifdef  IDE_READ_SUPPORT
                LOG(0, "//%s\n", pcToken);
                #endif    
                _LoadFile(_acFileName, rPesInfo.u4FifoStart);
            }
            //parsing bitstream info, offset/type of nalu, and send pes packet    
            pcToken = strtok(NULL, "\r\n");
            while( pcToken != NULL )
            {
                rPesInfo.ucPicType = StrToInt(pcToken);
                pcToken = strtok(NULL, "\r\n");
                if(pcToken != NULL)
                {
                    rPesInfo.u4VldReadPtr = (rPesInfo.u4FifoStart + StrToInt(pcToken)) + 1; // +1 eliminate ucPicType
                    VDEC_SendEs((void*)&rPesInfo);
                    #ifdef RR_TEST
                    if(_u4YRREn)
                    {
                        while(rPesInfo.ucPicType == 0x65)
                        {
                            VDEC_SendEs((void*)&rPesInfo);
                        }
                    }
                    else
                    {
                        while(rPesInfo.ucPicType == 0x25)
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
            LOG(6, "//File %d Finish ES Sending\n", _u4CurFileIdx);
            VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
            LOG(6, "//File %d Finish ES Decoding\n", _u4CurFileIdx);
            //re-play, re-init
            VDEC_Play(0, VDEC_FMT_H264VER);
            VDEC_SetParam(ES0, OUTPUT_DISP_ORDER, _bCmpEn, 0, 0);
            VDEC_SetParam(ES0, RRMODE, _u4YRREn, 0, _u4YRRRatio);
            VDEC_SetParam(ES0, RRMODE, _u4CRREn, 1, _u4CRRRatio);
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

    LOG(0, "Enter _VdecCmdSendPattern");

    x_memset(acPath, 0, MAX_FILENAME_LEN);
    sprintf(acPath, "c:\\BitstreamList.txt");
    x_memset(_pcFileInfo, 0, FILE_INFO_SIZE);    

    LOG(0, "Start  to load %s\n", acPath);
    _LoadFile(acPath, (UINT32)_pcFileInfo);
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", 
    //            acPath, (UINT32)_pcFileInfo));
    //_LoadFile(acPath, pcFileInfo);
    LOG(0, "d.load.binary c:\\BitstreamList.txt 0x%08x /ny", _pcFileInfo);

                
    pcToken = strtok(_pcFileInfo, ",\n");
    if(pcToken != NULL)
    {
        _u4FileCount = StrToInt(pcToken);
        LOG(0, "Total File Count = %ld\n", _u4FileCount);
    }

    pcToken = strtok(NULL, ",");
    while(i < _u4FileCount)
    {
        strcpy(_acFileDir[i], pcToken + 1);
        LOG(0, "File : %s\n", _acFileDir[i]);
        pcToken = strtok(NULL, ",");
        strcpy(_acTestBitstream[i], pcToken);
        pcToken = strtok(NULL, ",");
        strcpy(_acGoldPrefix[i], pcToken);
        pcToken = strtok(NULL, ",");
        strcpy(_acFilePrefix[i], pcToken);
        pcToken = strtok(NULL, ",");
        _afgLoadCompData[i] = StrToInt(pcToken);
        pcToken = strtok(NULL, ",");
        _au4StartCompareFrameNum[i] = StrToInt(pcToken);
        pcToken = strtok(NULL, ",");
        _au4PicWidth[i] = StrToInt(pcToken);
        pcToken = strtok(NULL, ",\n");
        _au4PicHeight[i++] = StrToInt(pcToken);
        pcToken = strtok(NULL, ",");
    } 

    while (1)
    {
        for(_u4CurFileIdx = 0; _u4CurFileIdx < _u4FileCount; _u4CurFileIdx++)
        {
            //loop all files
            x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
            x_memset(_pcFileInfo, 0, FILE_INFO_SIZE);
            
            rPesInfo.u4VldReadPtr = VFIFO_S;
            rPesInfo.u4FifoStart = VFIFO_S;    //should get from dmx fbm
            rPesInfo.u4FifoEnd = VFIFO_E;

            //reset frame idx
            _u4CurFrmIdx = 0; 

            _fgLoadFail = FALSE;
            
            x_memset(acPath, 0, MAX_FILENAME_LEN);
            sprintf(acPath, "%sBsInfo.txt", _acFileDir[_u4CurFileIdx]);
            
            LOG(0, "Start  to load %s\n", acPath);

                    sprintf(_acFileName, "%sBsInfo.txt", _acFileDir[_u4CurFileIdx]);
                    _LoadFile(_acFileName, (UINT32)_pcFileInfo);
            //UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%sBsInfo.txt\" 0x%08x /ny", 
            //            _acFileDir[_u4CurFileIdx], (UINT32)_pcFileInfo));        
            //_LoadFile(acPath, pcFileInfo);
            LOG(0, "d.load.binary \"%sBsInfo.txt\" 0x%08x /ny \n", _acFileDir[_u4CurFileIdx], (UINT32)_pcFileInfo);


            x_memset(acPath, 0, MAX_FILENAME_LEN);
            sprintf(acPath, "%s.m4v", _acTestBitstream[_u4CurFileIdx]);

            if (_afgLoadCompData[_u4CurFileIdx])
            {
                /*UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s.m4v\" 0x27a4000 /ny", 
                            _acFileDir[_u4CurFileIdx]));*/
                _LoadFile(acPath, rPesInfo.u4FifoStart);
                LOG(0, "d.load.binary \"%s.m4v\" 0x27a4000 /ny \n", _acTestBitstream[_u4CurFileIdx]);
            }
            else
            {
                LOG(0, "We don't need to load file\n");
            }

            //parsing bitstream info, offset/type of nalu, and send pes packet     
            pcToken = strtok(_pcFileInfo, ",");
            while( pcToken != NULL )
            {
                if(pcToken != NULL)
                {
                    i4Pos = StrToInt(pcToken);
                    if (i4Pos == 0 && _u4CurFrmIdx != 0)
                    {
                        break;
                    }
                    rPesInfo.u4VldReadPtr = (rPesInfo.u4FifoStart + i4Pos);
                    //rPesInfo.u4Width = _au4PicWidth[_u4CurFileIdx];
                    //rPesInfo.u4Height = _au4PicHeight[_u4CurFileIdx];
                    VDEC_SendEs((void*)&rPesInfo);
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
            VDEC_SendEs((void*)&rPesInfo);
            VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done

        }
        _u4DecCycles++;
        #ifdef CC_DRIVER_DEMO
        UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary c:\\decodingcycles%ld.txt 0x%08x--0x%08x", 
        _u4DecCycles, (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + 1)));
            #else
            LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
            #endif
    }
}

BOOL _VdecWMVIsX8Intra(UINT32 u4addr,UINT32 u4addr2)
{
    return FALSE;
}

void _VdecVParserWMV(VDEC_PES_INFO_T* pVDec_Pes_Info)
{
#ifdef CC_DRIVER_DEMO

    VDEC_PES_INFO_T* rPesInfo = pVDec_Pes_Info;
    
    UINT32 u4RCVVersion,u4CodecVersion;
    UINT32 u4RcvNumFrames,u4SetPos;
    UINT32 u4PicHeightSrc,u4PicWidthSrc;
    UINT32 u4HeaderLen =0;
    UINT32 u4SeqHdrData1,u4SeqHdrDataLen;
    BOOL bSkipFrame = FALSE;
    UINT32 hdrext;
    UINT32 u4FrameIndex = 0;

    u4SetPos = 0;

    if( rPesInfo->u4VideoType == DMX_VIDEO_VC1)  //.vc1
    { 
        // vc-1 video type
    }
    else if( rPesInfo->u4VideoType != DMX_VIDEO_VC1 ) //.rcv
    {
        x_memcpy(&u4RcvNumFrames,((rPesInfo->u4FifoStart)+u4SetPos),4);
        u4SetPos +=4;
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
        
        if(u4CodecVersion==0) //wmv7
        { 
            rPesInfo->u4VideoType = DMX_VIDEO_WMV7;//WMV1;
        }
        else if(u4CodecVersion == 2) //WMV8 
        {
            rPesInfo->u4VideoType = DMX_VIDEO_WMV8;//WMV2;
        }
        else if(u4CodecVersion == 5) //Simple & Main Profile
        { 
            rPesInfo->u4VideoType = DMX_VIDEO_WMV9;//WMV3;
        }
        else if(u4CodecVersion == 6) //Advanced Profile
        { 
            rPesInfo->u4VideoType = DMX_VIDEO_VC1;//WMVA;
        }
        else if(u4CodecVersion == 8) //Advanced Profile
        { 
            rPesInfo->u4VideoType = DMX_VIDEO_VC1;//WMVA;
        }
        else 
        {
            rPesInfo->u4VideoType = 0xff; //Uunknow type
            LOG(0,"_VdecVParserWMV : unknow type\n");
        }
    }
    
    if(rPesInfo->u4VideoType != DMX_VIDEO_VC1) //for others , no start code
    {
        if(hdrext != 0) 
        {
            //parsing SeqHdrDataLen
            x_memcpy(&u4SeqHdrDataLen,((rPesInfo->u4FifoStart)+u4SetPos),4);
            u4SetPos += 4;

            //parsing SeqHdrData1
            //headerData( headerLen * 8 bits )
            x_memcpy(&u4SeqHdrData1,((rPesInfo->u4FifoStart)+u4SetPos),4);
            u4SetPos += 4; 
            u4SeqHdrData1 = ((u4SeqHdrData1&0x000000FF)<<24) | ((u4SeqHdrData1&0x0000FF00)<<8) | ((u4SeqHdrData1&0x00FF0000)>>8) | ((u4SeqHdrData1&0xFF000000)>>24);
            if(u4SeqHdrDataLen == 5) 
            {
                u4SetPos +=1;
            }
        }
        //parsing picture height
        x_memcpy(&u4PicHeightSrc,((rPesInfo->u4FifoStart)+u4SetPos),4);
        u4SetPos += 4;
        //parsing picture width
        x_memcpy(&u4PicWidthSrc,((rPesInfo->u4FifoStart)+u4SetPos),4);
        u4SetPos += 4; 
        
        LOG(0,"_VdecVParserWMV : Source Resolution = %d x %d\n",u4PicWidthSrc, u4PicHeightSrc);
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
            u4SetPos +=16;
        }
        x_memcpy(&u4HeaderLen,((rPesInfo->u4FifoStart + u4SetPos)),4);
        //u4HeaderLen += 4;
        u4SetPos +=4;
        u4HeaderLen &= 0x0fffffff;
        
        if(u4HeaderLen == 1) //it indicates Picture Length == 1, means a skipped frame
        {
            //bPreviousSkip = TRUE;
            bSkipFrame = TRUE;
            LOG(0,"Frame %d : Skip Frame \n",u4FrameIndex);
        }
        else
        {
            //bPreviousSkip = FALSE;
            bSkipFrame = FALSE;
        }
            
        if(u4RCVVersion == 1)
        {
            //parsing time_stamp syntax element
            u4SetPos +=4;
            //u4HeaderLen += 4;
        }


        //rPesInfo->fgGop = bSkipFrame;
        rPesInfo->u4VldReadPtr = u4SetPos + rPesInfo->u4FifoStart;
        VDEC_SetParam(0,0,u4PicWidthSrc,u4PicHeightSrc,rPesInfo->ucPicType);
        VDEC_SetParam(0,1,u4SeqHdrDataLen,u4SeqHdrData1,bSkipFrame);
        
        if(rPesInfo->u4VideoType==DMX_VIDEO_WMV8 && _VdecWMVIsX8Intra(rPesInfo->u4VldReadPtr,rPesInfo->u4FifoStart))
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
            x_memcpy(&u4HeaderLen,((rPesInfo->u4VldReadPtr + rPesInfo->u4FifoStart)),4);
            u4SetPos +=4;
            u4HeaderLen &= 0x0fffffff;
        
            if(u4HeaderLen == 1) //it indicates Picture Length == 1, means a skipped frame
            {
                bSkipFrame = TRUE;
                LOG(0,"Frame %d : Skip Frame \n",u4FrameIndex);
            }
            else
            {
                bSkipFrame = FALSE;
            }
                
            if(u4RCVVersion == 1)
            {
                //parsing time_stamp syntax element
                u4SetPos +=4;
            }
            rPesInfo->u4VldReadPtr = u4SetPos + rPesInfo->u4FifoStart;
            //rPesInfo->fgGop = bSkipFrame;
            //VDEC_SetParam(0,1,u4SeqHdrDataLen,u4SeqHdrData1,bSkipFrame);
            if((rPesInfo->u4VldReadPtr -rPesInfo->u4FifoStart  + WMV_PIC_HEADER_LEN)< bitstream_sz[_u4CurFileIdx]) 
            {
                if(rPesInfo->u4VideoType==DMX_VIDEO_WMV8 && _VdecWMVIsX8Intra(rPesInfo->u4VldReadPtr,rPesInfo->u4FifoStart))
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
        }while((rPesInfo->u4VldReadPtr-rPesInfo->u4FifoStart + WMV_PIC_HEADER_LEN)< bitstream_sz[_u4CurFileIdx]);
    }
    else //start code version
    {
        CHAR* pcToken;
        //VDEC_SetParam(0,0,rPesInfo->ucPicType,0,0);
        x_memset((void*)_pcStartCodeFile, 0, FILE_INFO_SIZE);
        LOG(0,"d.load.binary \"%s\\StartCode_%s.txt\" 0x%08x /ny\n","D:\\project\\wmv\\vc1_start_code"/*golden_path[_u4CurFileIdx]*/,_acGoldPrefix[_u4CurFileIdx], _pcStartCodeFile);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\StartCode_%s.txt\" 0x%08x /ny","D:\\project\\wmv\\vc1_start_code"/*golden_path[_u4CurFileIdx]*/,_acGoldPrefix[_u4CurFileIdx], _pcStartCodeFile));
        pcToken = strtok(_pcStartCodeFile, "\r\n");
        while( pcToken != NULL) 
        {
            rPesInfo->u4VldReadPtr = StrToInt(pcToken) + rPesInfo->u4FifoStart;
            VDEC_SendEs((void*)rPesInfo);
            pcToken = strtok(NULL, "\r\n");
        } 
    }
#endif
}


void _RVLoadBitstream(UINT32 u4PicNo, UINT32 u4FifoStart)
{
    printf("d.load.binary \"%s\\f%07d.bin\" 0x%08X /ny\n", _acFileDir[_u4CurFileIdx], u4PicNo, u4FifoStart);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\f%07d.bin\" 0x%08X /ny", _acFileDir[_u4CurFileIdx], u4PicNo, u4FifoStart));
}

void _VdecVParserRV(VDEC_PES_INFO_T* pVDec_Pes_Info)
{
#ifdef CC_DRIVER_DEMO

    VDEC_PES_INFO_T* rPesInfo = pVDec_Pes_Info;
    
    UINT32 u4RCVVersion,u4CodecVersion;
    UINT32 u4RcvNumFrames,u4SetPos;
    UINT32 u4PicHeightSrc,u4PicWidthSrc;
    UINT32 u4HeaderLen =0;
    UINT32 u4SeqHdrData1,u4SeqHdrDataLen;
    BOOL bSkipFrame = FALSE;
    UINT32 hdrext;
    UINT32 u4FrameIndex = 0;

    u4SetPos = 0;

    if (rPesInfo->u4VideoType == DMX_VIDEO_RV) // !!!
    {
        _fgRVEmuEnd = FALSE;
        rPesInfo->u4VldReadPtr = rPesInfo->u4FifoStart;  // reset VLD read pointer to FIFO start
        rPesInfo->u4RenderPts = FRMINFO_S;  // !!! tmp use this to serve as frame info read pointer
        //VDEC_SetParam(0,0,u4PicWidthSrc,u4PicHeightSrc,rPesInfo->ucPicType);
        //VDEC_SetParam(0,1,u4SeqHdrDataLen,u4SeqHdrData1,bSkipFrame);
        
        VDEC_SendEs((void*)rPesInfo);
        u4FrameIndex++;

        do 
        {
            rPesInfo->u4VldReadPtr = rPesInfo->u4FifoStart;  // !!!
            //VDEC_SetParam(0,1,u4SeqHdrDataLen,u4SeqHdrData1,bSkipFrame);
            //if(*(UINT32 *)(rPesInfo->u4RenderPts) != 0xFFFFFFFF)   //fixme!!!
            if (!_fgRVEmuEnd)  // !!! tmp solution
            {
                VDEC_SendEs((void*)rPesInfo);
                u4FrameIndex++;
            } 
            else 
            {
                break;
            }
        }while(1);
    }
    else //start code version
    {
        CHAR* pcToken;
        //VDEC_SetParam(0,0,rPesInfo->ucPicType,0,0);
        x_memset((void*)_pcStartCodeFile, 0, FILE_INFO_SIZE);
        LOG(0,"d.load.binary \"%s\\StartCode_%s.txt\" 0x%08x /ny\n","D:\\Project\\decoder\\vc-1\\wmv\\vc1_start_code"/*golden_path[_u4CurFileIdx]*/,_acGoldPrefix[_u4CurFileIdx], _pcStartCodeFile);
        UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\StartCode_%s.txt\" 0x%08x /ny","D:\\Project\\decoder\\vc-1\\wmv\\vc1_start_code"/*golden_path[_u4CurFileIdx]*/,_acGoldPrefix[_u4CurFileIdx], _pcStartCodeFile));
        pcToken = strtok(_pcStartCodeFile, "\r\n");
        while( pcToken != NULL) 
        {
            rPesInfo->u4VldReadPtr = StrToInt(pcToken) + rPesInfo->u4FifoStart;
            VDEC_SendEs((void*)rPesInfo);
            pcToken = strtok(NULL, "\r\n");
        } 
    }
#endif
}


void _VdecSendWMVPattern(void* pvArg)
{
    VDEC_PES_INFO_T rPesInfo;
    CHAR* pcToken;
    UINT32 iLen;
    UINT16 u2file_num=0;
    UINT16 index=0;    
    char acPath[MAX_FILENAME_LEN];            
    char pattern_path[512]= "c:\\PatternFiles.txt";
    char golden_path_arg[512] ="c:\\Golden.txt";

    x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);

    //load pattern file to dram
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

    do
    {
        for(_u4CurFileIdx =0 ; _u4CurFileIdx < u2file_num ; _u4CurFileIdx++)
        {
            _u4CurFrmIdx =0;
            x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
            
            if(_acFileDir[_u4CurFileIdx][0] == '#')
            {
                  continue;    //skip the pattern
            }
            
            //get input_bitstream to vFIFO start point
            x_memset(acPath, 0, MAX_FILENAME_LEN);
            sprintf(acPath, "%s", _acFileDir[_u4CurFileIdx]);
#ifdef WMV_PROFILING
            HAL_GetTime(&rTimeRstS);
#endif
            _LoadFile(acPath,VFIFO_S);
#ifdef WMV_PROFILING
            HAL_GetTime(&rTimeRstE);
            HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
            Printf("WMV loading bitstream time = (%ld) \n", rTimeRstDt.u4Seconds);
#endif
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
            
            iLen = x_strlen(_acFileDir[_u4CurFileIdx]);

            if( (_acFileDir[_u4CurFileIdx][iLen-4] == '.') && (_acFileDir[_u4CurFileIdx][iLen-3] == 'v') &&
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

            rPesInfo.u4VldReadPtr = VFIFO_S;
            rPesInfo.u4FifoStart = VFIFO_S;
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


void _VdecSendRVPattern(void* pvArg)
{
    VDEC_PES_INFO_T rPesInfo;
    CHAR* pcToken;
    UINT32 iLen;
    UINT16 u2file_num=0;
    UINT16 index=0;    
    char acPath[MAX_FILENAME_LEN];            

    char pattern_path[512]= "c:\\PatternFilesRV.txt";
    char golden_path_arg[512] ="c:\\GoldenRV.txt";

    x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);

    //load pattern file to dram
    LOG(0,"d.load.binary \"%s\" 0x%08x /ny\n",pattern_path, _pcFileInfo);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pattern_path, _pcFileInfo));

    pcToken = strtok(_pcFileInfo, "\r\n");
#if 1
    while( pcToken != NULL) 
    { 
        strcpy(_acFileDir[u2file_num++], pcToken);
        pcToken = strtok(NULL, "\r\n");
    }
#else
    {
        strcpy(_acFileDir[u2file_num++], "R:\\rm_emu\\frminf\\wcp\\rv30\\rv8_008");
    }
#endif

    x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);

    //load golden pattern to dram
    LOG(0,"d.load.binary \"%s\" 0x%08x /ny\n",golden_path_arg, _pcFileInfo);
    UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", golden_path_arg, _pcFileInfo));

#if 1
    pcToken = strtok(_pcFileInfo, "\r\n");
    while( pcToken != NULL) 
    {
        strcpy(golden_path[index++], pcToken);
        pcToken = strtok(NULL, "\r\n");
    } 
#else
    strcpy(golden_path[0], "R:\\rm_emu\\golden\\wcp\\rv30\\rv8_008");
#endif

    do
    {
        for(_u4CurFileIdx =0 ; _u4CurFileIdx < u2file_num ; _u4CurFileIdx++)
        {
            _u4CurFrmIdx =0;
            x_memset((void*)&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
            
              if(_acFileDir[_u4CurFileIdx][0] == '#')
              {
                  continue;    //skip the pattern
              }

            //get input_bitstream to vFIFO start point
            x_memset(acPath, 0, MAX_FILENAME_LEN);
            sprintf(acPath, "%s\\frminfo.bin", _acFileDir[_u4CurFileIdx]);
#ifdef RV_PROFILING
            HAL_GetTime(&rTimeRstS);
#endif
            _LoadFile(acPath,FRMINFO_S);
#ifdef RV_PROFILING
            HAL_GetTime(&rTimeRstE);
            HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
            Printf("RV loading bitstream time = (%ld) \n", rTimeRstDt.u4Seconds);
#endif
            //load checksum
            if(_fgEnChkSum)
            {
                UINT32 u4FrmNum =0;
                //sprintf(_acChkSumFile, "%s/ChkSum.txt", golden_path[_u4CurFileIdx]);
                sprintf(_acChkSumFile, "D:\\vc1_emulation\\ChkSum\\ChkSum_%s.txt", _acGoldPrefix[_u4CurFileIdx]);
                x_memset((void*)&_rCSInfo, 0,  sizeof(VDEC_CHKSUM_INFO_T));
#ifdef RV_PROFILING
                HAL_GetTime(&rTimeRstS);
#endif
                _LoadFile(_acChkSumFile, (UINT32)_pcChkSum);
#ifdef RV_PROFILING
                HAL_GetTime(&rTimeRstE);
                HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
                Printf("WMV loading checksum time = (%ld) \n", rTimeRstDt.u4Micros);
#endif

                u4FrmNum = _VdecGetChkSum(_pcChkSum, &_rCSInfo);
                LOG(0, "Chksum FrmNum %d\n", u4FrmNum);
            }
            //load frame count
            #if 0
            {
                x_memset((void*)_pcFileInfo, 0, FILE_INFO_SIZE);
                sprintf(_acFrameCountFile,"D:\\vc1_emulation\\FrameCount\\FrameCount_%s.txt",
                    _acGoldPrefix[_u4CurFileIdx]);
                _LoadFile(_acFrameCountFile, (UINT32)_pcFileInfo);
                //sscanf(_pcFileInfo,"%d",&_u4GoldneFrameCount);
                pcToken = strtok(_pcFileInfo, "\r\n");
                printf("totla string : %s\n",_pcFileInfo);
                while( pcToken != NULL) 
                {
                    printf("token : %s\n",pcToken);
                    _u4GoldneFrameCount = StrToInt(pcToken);
                    pcToken = strtok(NULL, "\r\n");
                    //strcpy(golden_path[index], pcToken);
                } 
                LOG(0,"frame count : %d\n",_u4GoldneFrameCount);
            }
            #endif
            
            iLen = x_strlen(_acFileDir[_u4CurFileIdx]);


            rPesInfo.u4VideoType = DMX_VIDEO_RV;

#ifdef RV_PROFILING
            HAL_GetTime(&rTimeRstS);
#endif

            rPesInfo.u4VldReadPtr = VFIFO_S;
            rPesInfo.u4FifoStart = VFIFO_S;
            rPesInfo.u4FifoEnd = VFIFO_E;
            _VdecVParserRV(&rPesInfo);

#if 0
            //indicate last frame
            rPesInfo.ucPicType = 255;
            VDEC_SendEs((void*)&rPesInfo);
#endif
            
            //send eos to push last frame to decoder
            rPesInfo.fgEos = TRUE;
            VDEC_SendEs((void*)&rPesInfo);
            VERIFY(x_sema_lock(_hVdecSema, X_SEMA_OPTION_WAIT) == OSR_OK);    //wait decode done
#ifdef RV_PROFILING
            HAL_GetTime(&rTimeRstE);
            HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
            Printf("WMV decoding and compare time = (%ld) \n", rTimeRstDt.u4Seconds);
#endif
            //printf("_VdecSendWMVPattern : end of file %d\n",_u4CurFileIdx);
            //re-play, re-init
            VDEC_Stop(0);
            VDEC_Play(0, VDEC_FMT_RV);
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


static INT32 _VdecCmdSaveDecOut(INT32 i4Argc, const CHAR ** szArgv)
{
    ASSERT(szArgv != NULL);    
    if(i4Argc < 2)
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
        
    if(dwWidth%16)
        dwWidth = ((dwWidth / 16) + 1) * 16; //ming modify@2006/4/11
    if(dwHeight % 32) 
        dwHeight = ((dwHeight / 32) + 1) * 32;
        

    pu1HWAddr = (UINT8*)u4CalculatePixelAddress_Y((UINT32)hw_output, 0, 0, dwWidth, TRUE, 4);

      #ifdef CC_DRIVER_DEMO
    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,pu1HWAddr, pu1HWAddr+dwWidth*dwHeight-1));
      #else
      LOG(0, "CC_DRIVER_DEMO !defined, File save fail~");
      #endif
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y0_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,rWMVDecInfo.rDram.pu1Pic0Y, rWMVDecInfo.rDram.pu1Pic0Y+dwWidth*dwHeight-1));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y1_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,rWMVDecInfo.rDram.pu1Pic1Y, rWMVDecInfo.rDram.pu1Pic1Y+dwWidth*dwHeight-1));
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_Y2_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,rWMVDecInfo.rDram.pu1Pic2Y, rWMVDecInfo.rDram.pu1Pic2Y+dwWidth*dwHeight-1));

    hw_output = prDispInfo->pvCAddr;
    pu1HWAddr = (UINT8*)u4CalculatePixelAddress_C((UINT32)hw_output, 0, 0, dwWidth, TRUE, TRUE, 4);

      #ifdef CC_DRIVER_DEMO
    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_C_%d.raw\" 0x%08x--0x%08x",_acGoldPrefix[_u4CurFileIdx],_u4CurFrmIdx,pu1HWAddr, (pu1HWAddr+dwWidth*dwHeight/2)-1));
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

/*
void print_checksum(BOOL fgRead) {
    UINT32 u4Idx;
    
        //Printf("===Dump VLD mem ===cc\n");    
    //UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"d:\\dump\\tmp\\%s_VLD_90_%d.raw\" 0x%08x--0x%08x",_fiInName,rWMVDecInfo.rOther.u4CurrPicNo,RW_VLD_PICSZ, RW_VLD_MV3_ADDR));

        Printf("===WMV MC status===\n");    
        Printf("MC status, 37(0x94)=0x%x\n",ReadREG(RO_MC_STATUS));
        Printf("MC QIU status, 245(0x3d4)=0x%x\n",ReadREG(RO_MC_QIU_STATUS));
        Printf("MC HIU status, 246(0x3d8)=0x%x\n",ReadREG(RO_MC_HIU_STATUS));
    Printf("MC CBP, 27(0x6C)=0x%x\n", ReadREG(RO_MC_CBP));
    
        Printf("===WMV VLD status===\n");
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

  if(bBlockW == 4)  // in MT8520, one block width is (16x32)
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
  if(bBlockW == 4)
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
    if(i4Argc < 3)
    {
        LOG(0, "vdec.ssp [enable 1][Frame number(start from 0)]");
        return 0;
    }
    if(StrToInt(szArgv[1]) == 0)
    {
        _i4SSimPatFrmNo = -1;
    }
    else
    {
        _i4SSimPatFrmNo = (INT32)StrToInt(szArgv[2]);
        if(_i4SSimPatFrmNo==0)
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
    if(i4Argc < 3)
    {
        LOG(0, "vdec.stm [TestSet 0:other, 1:Allegro, 2:Allegro2][Comparison Enable]([FrameNo1][FrameNo2]...)");
        return 0;
    }
    VDEC_SetParam(ES0, OUTPUT_DISP_ORDER, StrToInt(szArgv[2]), 0, 0);

    _u1TestSet = (UINT8)StrToInt(szArgv[1]);
    if(StrToInt(szArgv[2]) > 0)
    {
        _bCmpEn = TRUE;
        if(i4Argc > 3)
        {
            x_memset((void*)_bCmpFrmEn, 0, sizeof(_bCmpFrmEn));    
            for(u4FrmIdx = 3; u4FrmIdx < i4Argc; u4FrmIdx++)
            {
                _bCmpFrmEn[StrToInt(szArgv[u4FrmIdx])] = TRUE;
            }
        }
        else
        {
            for(u4FrmIdx = 0; u4FrmIdx < 1000; u4FrmIdx++)
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
    if(i4Argc < 3)
    {
        LOG(0, "vdec.srf [Repeat Frame Enable][Repeat FrameNo]\n");
        LOG(0, "vdec.srf %d %d\n", _bRptFrmEn, _u4RptFrmNo);
        return 0;
    }
    if(StrToInt(szArgv[1]) > 0)
    {
        _bRptFrmEn = TRUE;
        _u4RptFrmNo = (UINT32)StrToInt(szArgv[2]);
        if(_u4RptFrmNo == 0)
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
    if(i4Argc < 2)
    {
        LOG(0, "vdec.cs [Checksum Enable]\n");
        LOG(0, "vdec.cs %d\n", _fgEnChkSum);
        return 0;
    }
    if(StrToInt(szArgv[1]) > 0)
    {
        _fgEnChkSum = TRUE;
    }
    else
    {
        _fgEnChkSum = FALSE;
    }

    return 0;
}
#endif

static INT32 _VdecCmdDramBWSimu(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Param1=0, u4Param2=0, u4Param3=0;
    
    ASSERT(szArgv != NULL);
    if(i4Argc < 4)
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
    if(i4Argc < 3)
    {
        LOG(0, "vdec.mid mode [0:none, 1:OverWriteTest(exclusive), 2:Vfifo/FBM Protect, 3:Vfifo/FBM Protect emu] MPEG2DBKDisable[1:disable]\n");
        LOG(0, "vdec.mid %d %d\n\n", _u1MidMode, _fgDBKDisable);
        return 0;
    }
    prFbmPoolDMX = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    if((prFbmPoolDMX == NULL) || (prFbmPoolDMX->u4Addr == 0))
    {
        return FALSE;
    }

    fgRet &= MID_Init();
    fgRet &= MID_Reset();
    
    _u1MidMode = (UINT8)StrToInt(szArgv[1]);
    _fgDBKDisable = (UINT8)StrToInt(szArgv[2]);
    if(_u1MidMode > 3)
    {
        return FALSE;
    }
    else if(_u1MidMode == 3)
    {
        _u4VStart = VFIFO_S;
        _u4VEnd = VFIFO_E;
        fgRet &= MID_SetRegion(0, MID_AGENT_CPU, _u4VStart, _u4VEnd);
        fgRet &= MID_EnableRegionProtect(0);
    }

    if(_u1MidMode == 1)
    {
        //frame-based mid
        fgRet &= VDEC_SetMid(ES0, TRUE);
    }
    else
    {
        fgRet &= VDEC_SetMid(ES0, FALSE);
    }

    if(_u1MidMode == 0)
    {
        VDEC_FbmMidTest(ES0, 0, FALSE);
        fgRet &= BIM_DisableIrq(VECTOR_DRAMC);
    }
    else
    {
        // Disable allocate fbm with gfx clean
        FBM_RegCbFunc(FBM_CB_FUNC_GFX_MEMSET, NULL);
        // call fbm flag, enable MID after FBG create
        VDEC_FbmMidTest(ES0, _u1MidMode, _fgDBKDisable);
        if(_fgDBKDisable)
        {
            //disable mpeg2 deblocking
            MPV_SetDbkInfo(0, FALSE, 0, 0, 0, 0);
        }
        fgRet &= BIM_EnableIrq(VECTOR_DRAMC);
    }

    if(fgRet)
    {
        LOG(0, "Memory Protect Set Done~\n");
        if(_u1MidMode > 1)
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


