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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifdef __MODEL_slt__
//#if 1

//#include "slt_cmd.h"
#include "slt_if.h"
#include "vdp_if.h"
#include "dmx_if.h"
#include "musb_if.h"
#include "video_def.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_rand.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_pdwnc.h"
#include "vdp_drvif.h"
#include "nim_drvif.h" //EC_code_2007/6/22
#include "hw_vdoin.h"
#include "hw_scpos.h"
#include "hw_ckgen.h"
#include "drv_vdoclk.h"
#include "drv_ycproc.h"
#include "hw_ycproc.h"
#include "drv_di.h"
#include "drv_nr.h"
#include "util.h"//kal
#define DEFINE_IS_LOG CLI_IsLog

#include "x_debug.h"
#include "x_timer.h"
#include "x_drv_cli.h"
#include "drv_pqstress.h"
#include "aud_if.h"
#include "hw_mlvds.h"
#include "hw_vbyone.h"
#include "panel.h"
//#include "g3d_emu_if.h"

#define VDOIN_READ32(offset)          IO_READ32(IO_VIRT + 0x20000, (offset))
#define VDOIN_WRITE32(offset, value)  IO_WRITE32(IO_VIRT + 0x20000, (offset), (value))

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#ifndef SLT_MT5371
#define SLT_MT5371 0
#endif
#define _MODE_NOT_INIT 0
#define _MODE_INIT 1
#define _MODE_PLAY_ATV 2
#define _MODE_PLAY_MPEG 3
#define _MODE_PLAY_POP 4
#define _MODE_PLAY_OSD 5
#define _MODE_PLAY_MISC 6

#define MAX_CAL_NS 10

#define DTD_SLT_TIMEOUT 35000

#define DTD_PARALLEL_TEST  0
#define DVBT_PARALLEL_TEST 0

#ifndef CC_SLT_PANEL_ID
//#define CC_SLT_PANEL_ID 3
#define CC_SLT_PANEL_ID 95
#endif

#define cDTD_TIME_ITEM  4
#define cDVBT_TIME_ITEM  4

//#define PARALLEL_AUD

// define CLK_BOUND for PLL test
#define CLK_BOUND     3000000    //60000000/256
#define DMPLL_DFT_FREQ       (351000000)
#define CPUCLK_DFT_FREQ      (729000000)
#define SYSPLL_DFT_FREQ      (648000000)
#define SAWLESSPLL_DFT_FREQ  (432000000)
#define USBPLL_DFT_FREQ      (480000000)
#define ETHNETPLL_DFT_FREQ   (675000000)
#define PIXPLL_DFT_FREQ      (247508000)//50Hz
//#define PIXPLL_DFT_FREQ      (296460000)//60Hz

#define CC_ECO_IC            (0)
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define ACK \
do \
{ \
    if (delay > 0) \
    { \
        x_thread_delay(delay); \
    } \
    Printf("OK\n"); \
} \
while (0)

#define WAIT_SIGNAL_STABLE \
while (SLT_IsSignalStable(0) == FALSE) \
{ \
    x_thread_delay(50); \
}

#define WAIT_SIGNAL_STABLE_PIP \
while (SLT_IsSignalStable(1) == FALSE) \
{ \
    x_thread_delay(50); \
}


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

extern volatile UINT8 bSLT_test_start;
extern volatile UINT32 port1pass, port2pass;
extern UINT8 _bAutoColorState0;
//extern UINT32 dwSI_DISPLAY_DCLK_TYPICAL;
//extern UINT32 wDISPLAY_WIDTH;
//extern UINT32 wDISPLAY_HEIGHT;

#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT
extern UINT8 TVE_SLT_SRCREMAP_EN;
#endif
extern BOOL fgCVBSFail;
extern UINT8 fgCvbsCheck(void);
extern UINT8  _bDviModeChged;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

EXTERN void vWriteVDOINMsk(UINT32 u4Reg, UINT8 bVal, UINT8 bMsk, UINT8 bByte);
EXTERN INT32 SLT_AudGetStressTestResult(void);
EXTERN INT32 SLT_AudStressTestInit(void);
EXTERN INT32 NAV_SwapSrcMode(void);
EXTERN UINT8 vVgaSlicerSLT_Report(void);
EXTERN UINT8 vVgaSlicerSLT_Report_verC(void);
EXTERN UINT8 vVgaSlicerSLT_Report_cust(void);
EXTERN BOOL fgHDMICRC(INT16 ntry);
EXTERN INT32 _SetVGAAutoColor(UINT32 u4VdpId);
EXTERN void SLT_BypassPQFunc(UINT8 u1VdpId, INT32 bypassMask);
EXTERN INT32 _SetVGAAuto(UINT32 u4VdpId);
EXTERN INT32 NAV_VQ_Reset(UINT32 u4VdpId);
EXTERN INT32 SLT_IntAdSorting(void);
EXTERN INT32 SLT_DtvSorting(void);
EXTERN INT32 SLT_AtdSorting(void);
EXTERN INT32 SLT_AtdEuSorting(void);
//EXTERN BOOL DRV_TUNER_IsCamConnected(void);

EXTERN void _AudPllInitial(UINT8 u1ApllId, SAMPLE_FREQ_T eFs);
EXTERN void AudPllInit(void);
EXTERN INT32 _i4EtherSltTest(INT32 i4Argc, const CHAR ** szArgv);
EXTERN void vDrvNRBypass(UINT8 bOnOff);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static INT32 _SltCmdInit(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _SltCmdFlashingAud(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdCvbs(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdQCvbs(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _SltCmdCvbsCheck(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdSVideo(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdYpbpr(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdHdmi(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdVga(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdOsd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdMpeg(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdMisc(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdAud(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdQAud(INT32 i4Argc, const CHAR * * szArgv); // by hotkey 2007/12/25
static INT32 _SltCmdQMisc(INT32 i4Argc, const CHAR * * szArgv);
static INT32 _SltCmdPll(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdShowPll(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdDtd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdDvbt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdAtd(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdQDtd(void);
static INT32 _SltCmdQDvbt(void);
static INT32 _SltCmdUsb(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdOK(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdEcho(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdBypassMLC(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _SltCmdEnableHDMI2ndTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdDelay(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdMpegUnlock(INT32 i4Argc, const CHAR * * szArgv);
static INT32 _SltCmdModeChgDone(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdPip(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdPop(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdPopA(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _SltCmdLvds(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _SltCmdHdmiColorTransform(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdReboot(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdTime(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdBypassPQ(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdFreezePts(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdUnFreezePts(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdTve(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdT8032(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdLvdsPortAssign(INT32 i4Argc, const CHAR **szArgv);
static INT32 _SltCmdEthTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _SltCmdFreezeField(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdForkThread(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdQDtdLock(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdQDvbtLock(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdDtvCrcCheck(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdMMSLT(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdMBIST(INT32 i4Argc, const CHAR **szArgv);
static INT32 _SltCmdQMBIST(INT32 i4Argc, const CHAR **szArgv);
static INT32 _SltCmdPQTorture(INT32 i4Argc, const CHAR **szArgv);
static INT32 _SltCmdVersion(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _SltCmdGfx3d(INT32 i4Argc, const CHAR ** szArgv);

CLIMOD_DEBUG_FUNCTIONS(CLI)

static void _AudProc(void* pvArg);
//static INT32 _SltTestVPllSS(UINT32 u4SSpermillage, UINT32 u4DelayUs, UINT32 u4Loop);
static INT32 _SltPqStress(INT32 i4RoundTimes);
// for ethernet & usb
static void _MiscProc(void *pvArg);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static CLI_EXEC_T _arSltCmdTbl[] =
{
    {"init", NULL, _SltCmdInit, NULL, "init", CLI_SUPERVISOR},
//    {"audflash", NULL, _SltCmdFlashingAud, NULL, "audflash", CLI_SUPERVISOR},
    {"cvbs", NULL, _SltCmdCvbs, NULL, "cvbs", CLI_SUPERVISOR},
    {"qcvbs", NULL, _SltCmdQCvbs, NULL, "qcvbs", CLI_SUPERVISOR},
    //{"cvbsc", NULL, _SltCmdCvbsCheck, NULL, "cvbsc", CLI_SUPERVISOR},
    {"svideo", NULL, _SltCmdSVideo, NULL, "svideo", CLI_SUPERVISOR},
    {"ypbpr", NULL, _SltCmdYpbpr, NULL, "ypbpr", CLI_SUPERVISOR},
    {"hdmi", NULL, _SltCmdHdmi, NULL, "hdmi", CLI_SUPERVISOR},
    {"vga", NULL, _SltCmdVga, NULL, "vga", CLI_SUPERVISOR},
    {"osd", NULL, _SltCmdOsd, NULL, "osd", CLI_SUPERVISOR},
    {"mpeg", NULL, _SltCmdMpeg, NULL, "mpeg", CLI_SUPERVISOR},
    {"misc", NULL, _SltCmdMisc, NULL, "misc", CLI_SUPERVISOR},
    {"aud", NULL, _SltCmdAud, NULL, "aud", CLI_SUPERVISOR},
    {"qaud", NULL, _SltCmdQAud, NULL, "query audio self test result", CLI_SUPERVISOR},
    {"qmisc", NULL, _SltCmdQMisc, NULL, "query misc test result", CLI_SUPERVISOR},
    {"pll", NULL, _SltCmdPll, NULL, "pll", CLI_SUPERVISOR},
    {"showpll", NULL, _SltCmdShowPll, NULL, "show PLL value", CLI_SUPERVISOR},
    {"dtd", NULL, _SltCmdDtd, NULL, "atsc demod test", CLI_SUPERVISOR},
    {"atd", NULL, _SltCmdAtd, NULL, "analog demod test", CLI_SUPERVISOR},
    {"qdtd", NULL, _SltCmdQDtd, NULL, "get atsc demod test result", CLI_SUPERVISOR},
    {"bypassmlc", NULL, _SltCmdBypassMLC, NULL, "bypass MLC", CLI_SUPERVISOR},
    {"usb", NULL, _SltCmdUsb, NULL, "USB slt", CLI_SUPERVISOR},
    {"delay", NULL, _SltCmdDelay, NULL, "Delay specified time", CLI_SUPERVISOR},
    {"ok", NULL, _SltCmdOK, NULL, "ok", CLI_SUPERVISOR},
	{"echo", NULL, _SltCmdEcho, NULL, "echo", CLI_SUPERVISOR},
    {"mpegu", NULL, _SltCmdMpegUnlock, NULL, "mpeg unlock", CLI_SUPERVISOR},
    {"modechgdone", NULL, _SltCmdModeChgDone, NULL, "mcd", CLI_SUPERVISOR},
    {"pip", NULL, _SltCmdPip, NULL, "pip", CLI_SUPERVISOR},
    {"pop", NULL, _SltCmdPop, NULL, "pop", CLI_SUPERVISOR},
    {"popa", NULL, _SltCmdPopA, NULL, "popa", CLI_SUPERVISOR},
    //{"lvds", NULL, _SltCmdLvds, NULL, "lvds", CLI_SUPERVISOR},
    //{"hdmict", NULL, _SltCmdHdmiColorTransform, NULL, "hdmi color transform", CLI_SUPERVISOR},
    {"reboot", NULL, _SltCmdReboot, NULL, "Reboot system", CLI_SUPERVISOR},
    {"t", NULL, _SltCmdTime, NULL, "Show time spent", CLI_SUPERVISOR},
    {"bypasspq", NULL, _SltCmdBypassPQ, NULL, "{vdpId, bypassMask} 0x1:bob, 0x2:DI, 0x4: NR, 0x8:Sharpness, 0x10:CTI", CLI_SUPERVISOR},
    {"fpts", NULL, _SltCmdFreezePts, NULL, "freeze at pts", CLI_SUPERVISOR},
    {"ufpts", NULL, _SltCmdUnFreezePts, NULL, "unfreeze", CLI_SUPERVISOR},
    {"tve", NULL, _SltCmdTve, NULL, "tve", CLI_SUPERVISOR},
    {"t8032", NULL, _SltCmdT8032, NULL, "t8032", CLI_SUPERVISOR},
    {"dvbt", NULL, _SltCmdDvbt, NULL, "dvbt demod test", CLI_SUPERVISOR},
//    {"qdvbt", NULL, _SltCmdQDvbt, NULL, "get dvbt demod test result", CLI_SUPERVISOR},
    {"lvdspa", NULL, _SltCmdLvdsPortAssign, NULL, "lvds port assign", CLI_SUPERVISOR},
    {"eth", NULL, _SltCmdEthTest, NULL, "ethernet slt", CLI_SUPERVISOR},
    {"ff", NULL, _SltCmdFreezeField, NULL, "freeze field {field, path}", CLI_SUPERVISOR},
    {"fork", NULL, _SltCmdForkThread, NULL, "fork thread {thread number} {command}", CLI_SUPERVISOR},
	{"qdtdlock", NULL, _SltCmdQDtdLock, NULL, "qdtdlock {timeout}", CLI_SUPERVISOR},
    {"qdvbtlock", NULL, _SltCmdQDvbtLock, NULL, "qdvbtlock {timeout}", CLI_SUPERVISOR},
    {"dtvcrc", NULL, _SltCmdDtvCrcCheck, NULL, "usage: pts b2rdramcrc b2rpixelcrc pscancrc", CLI_SUPERVISOR},
    {"mmslt", NULL, _SltCmdMMSLT, NULL, "usage: mp4 (or rm)", CLI_SUPERVISOR},
    {"mbist", NULL, _SltCmdMBIST, NULL, "0:full test, 1: partial test", CLI_SUPERVISOR},
    {"qmt",   NULL, _SltCmdQMBIST, NULL, "Query mbist result", CLI_SUPERVISOR},
    {"pqtort", NULL, _SltCmdPQTorture, NULL, "Torture PQ memory", CLI_SUPERVISOR},
    {"ver", NULL, _SltCmdVersion, NULL, "fw version", CLI_SUPERVISOR},
//    {"gfx3d", NULL,    _SltCmdGfx3d, NULL,    "3D GFX test", CLI_SUPERVISOR},

    CLIMOD_DEBUG_CLIENTRY(CLI),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _rSltRootCmd =
{
    "slt", NULL, NULL, _arSltCmdTbl,
    "SLT command", CLI_SUPERVISOR
};

//static UINT32 _u4Mode = _MODE_NOT_INIT;

BOOL _fgBypassMLC = FALSE;

UINT16 u2PERRecord[2][20];
UINT16 u2SignalPER[2];//EC_code_2007/6/22
UINT16 u2PERRecord_dvbt[2][20];
UINT16 u2SignalPER_dvbt[2];
UINT16 u2SignalPER2[2];//EC_code_2007/6/22
static HAL_TIME_T DTD_ProcessTime[2][10];
//static UINT32 u4DTDRetryRecord[2]; //useless
static HAL_TIME_T DVBT_ProcessTime[2][10];
//static UINT32 u4DVBTRetryRecord[2];

typedef struct {
BOOL fgAtscFirst;
INT32 num;
INT32 type1;
INT32 delay;
INT32 duration;
INT32 atsc_freq;
INT32 qam_freq;
INT32 PEC_delay;
INT32 pattern_delay;
INT32 selftest;
 } SLT_DTD_T;
BOOL _fgDtdUnsync = TRUE;
BOOL _fgDvbtUnsync = TRUE;
BOOL _fgDtdUnsync2 = TRUE;

UINT32 _u4OverscanRatio = 0;

HANDLE_T _hSltAutoColor = NULL;
HANDLE_T _hSltAuto = NULL;


UINT8 _u1SltDTV = 0;

//INT32 _i4PanelId = CC_SLT_PANEL_ID;
INT32 _i4PanelId = 100;
//UINT8 _u1SltHdmiColorTransform = 4;

// demod test
#if DTD_PARALLEL_TEST
static HANDLE_T _hDtdThread = NULL;
#endif
#if DVBT_PARALLEL_TEST
static HANDLE_T _hDvbtThread = NULL;
#endif
static HANDLE_T _hDtdAtscSema = NULL;
static HANDLE_T _hDtdFinSema = NULL;
static HANDLE_T _hDvbtFinSema = NULL;

// for Audio test
static HANDLE_T _hAudFinSema = NULL;
static HANDLE_T _hAudThread	= NULL;
static UCHAR _ucAudErrorCode = 0;
static HANDLE_T _hWorkerThread0	= NULL;
static HANDLE_T _hWorkerThread1	= NULL;
static INT32 _i4SkipMask = 0;

BOOL _fgSltInit = FALSE;

// for misc test (usb & ethernet)
static HANDLE_T _hMiscFinSema = NULL;
static HANDLE_T _hMiscTrigSema = NULL;
static HANDLE_T _hMiscThread = NULL;
static UCHAR _ucMiscErrorCode = 0;

// for vdec stop
BOOL fgSltWatchPts = FALSE;
BOOL fgSltNotSendPic = FALSE;
UINT32 u4SltPts;

typedef void (*NPTV_PFN_VGA_AUTOCOLOR_NFY)(
    UINT16 ui2_id,
    BOOL fgSuccess
);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
/*static INT32 _SltCmdGfx3d(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    G3D_EMU_CONF_T rConf = {0, 0, 0, "Z:\\g3d_emulation\\g3d_emu_list.txt"};
    i4Ret = i4Graphics3D_Drv_Init(rConf);

    i4Ret = i4Graphics3D_Display();

    //reset
    IO_WRITE32(0xF0040000, 0x20, 0x20);
    IO_WRITE32(0xF0048000, 0x100c, 0x20);
    x_thread_delay(100);
    i4Ret = i4Graphics3D_Drv_Emu();
    //retry
    IO_WRITE32(0xF0040000, 0x20, 0x20);
    IO_WRITE32(0xF0048000, 0x100c, 0x20);
    x_thread_delay(100);
    i4Ret = i4Graphics3D_Drv_Emu();
    if(i4Ret == 0)
    {
        Printf("GFX3DOK\n");
    }
    else
    {
        Printf("GFX3DFAIL\n");
    }
    return 0;
}
*/
//-----------------------------------------------------------------------------
/** Brief   	invoke vga auto position
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _SltAuto(UINT32 u4VdpId)
{
    _SetVGAAuto(u4VdpId);
    x_sema_lock(_hSltAuto, X_SEMA_OPTION_WAIT);
}


//-----------------------------------------------------------------------------
/** Brief		invoke vga auto color
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _SltAutoColor(UINT32 u4VdpId)
{
    _SetVGAAutoColor(u4VdpId);
    x_sema_lock(_hSltAutoColor, X_SEMA_OPTION_WAIT);
}

//-----------------------------------------------------------------------------
/** Brief		PLL test function
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------


static INT32 _SltTestPll(CAL_SRC_T eSrc, UINT32 u4Target, UINT32 u4Bound)
{
    UINT32 u4CalNs = 0, u4Clock;
    INT32 i4Fail = 0;
    UINT8 ulBand;
    UINT16 u2MS, u2NS, u2Counter;

    switch (eSrc)
    {
    case SRC_ETHPLL_CLK:
        Printf("test Ethernet pll: ");
        break;
    case SRC_SYSPLL_CLK:
        Printf("test SYSPLL: ");
        break;
    case CAL_SRC_AD_VOPLL_DPIX_CK:
        Printf("test VOPLL: ");
        break;
    case SRC_CPU_CLK:
        Printf("test CPUPLL: ");
        break;
    case CAL_SRC_SAWLESSPLL:
        Printf("test SAWLESSPLL: ");
        break;
    case CAL_SRC_APLL1:
        Printf("test APLL1: ");
        break;
    case CAL_SRC_APLL2:
        Printf("test APLL2: ");
        break;
    case CAL_SRC_USBPLL:
        Printf("test USBPLL: ");
        break;
    case SRC_MEM_CLK:
        Printf("test MEMCLK: ");
        break;

    case CAL_SRC_ADCPLL:
        Printf("test ADCPLL: ");
        break;
    case CAL_SRC_PIXPLL:
        Printf("test PIXPLL: ");
        break;
    //case CAL_SRC_HDMI_PLL340M:
    //    Printf("test HDMIPLL: ");
    //    break;

    default:
        return -1;
    }

    if(eSrc == CAL_SRC_APLL2)
    {
        if(u4Target == CLK_APLL270MHZ)
        {
            _AudPllInitial(1, FS_44K);
        }
        else if(u4Target == CLK_APLL294MHZ)
        {
            _AudPllInitial(1, FS_48K);
        }
    }
    //Printf(" %d\n", BSP_GetDomainClock(eSrc) );
    do
    {
        x_thread_delay(5);
        u4Clock = BSP_GetClock(eSrc, &ulBand, &u2MS, &u2NS, &u2Counter);

        if ((u4Clock > u4Target + u4Bound) ||
            (u4Clock < u4Target - u4Bound))
        {
            Printf("FAIL: u4CalNs=%d, u4Target=%d, u4Clock=%d, ulBand=%d, "
                   "u2MS=%d, u2NS=%d, u2Counter=%d)\n",
                   u4CalNs, u4Target, u4Clock, ulBand, u2MS, u2NS, u2Counter);
            i4Fail++;
        }

            u4CalNs = MAX_CAL_NS;
    } while (u4CalNs < MAX_CAL_NS);

    if (i4Fail == 0)
    {
        Printf("PASS\n");
    }
    return i4Fail;


    //return 0;
}



//-----------------------------------------------------------------------------
/** Brief		Init data structures & semaphores for SLT test
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;

    static BOOL fgInited = FALSE;

    while(!_fgSltInit)
    {
        x_thread_delay(100);
    }

    if(fgInited)
    {
        SLT_Init();
        AudPllInit();

        //_u4Mode = _MODE_INIT;
        ACK;
        return 0;
    }
    fgInited = TRUE;

    if(_hSltAutoColor == NULL)
    {
        x_sema_create(&_hSltAutoColor, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    }

    if(_hSltAuto == NULL)
    {
        x_sema_create(&_hSltAuto, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    }
    //CLI_Parser("nav.i 3");

	if(_hDtdFinSema == NULL)
	{
		x_sema_create(&_hDtdFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
	}

	if(_hDvbtFinSema == NULL)
	{
		x_sema_create(&_hDvbtFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
	}

    // initialize ethernet
    CLI_Parser("net.eth.i");

    // clear demo program's osd
    CLI_Parser("osd.i");

    NAV_VQ_Reset(0);

    CLI_Parser("pmx.s.d 15");


    //CLI_Parser("n.MuteMaskForceOff 0 1"); // force unmute for stop DTV test flashing
    CLI_Parser("jpg.d_l 0");
    CLI_Parser("n.scl.d_l 0");
    CLI_Parser("n.di.d_l 0");
    CLI_Parser("n.nr.d_l 0");
    CLI_Parser("n.mute.d_l 0");
    CLI_Parser("n.d_l 0");

    //CLI_Parser("vdp.s.bob 0 0");

    SLT_Init();

    //_u4Mode = _MODE_INIT;
    ACK;
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief		play av1 and freeze
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdCvbs(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 overscan = (i4Argc)>2?StrToInt(szArgv[2]):30;
    INT32 delay2 = (i4Argc)>1?StrToInt(szArgv[3]):500;

    CLI_Parser("slt.bypassmlc 0");
    CLI_Parser("vdp.f 0 1");
    CLI_Parser("vdp.f 0 0");
    _u1SltDTV = 1;
    CLI_Parser("nav.play m av1");
    CLI_Parser("vdp.s.bob 0 0");
    CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);
    WAIT_SIGNAL_STABLE;

    x_thread_delay(delay2);
    //SLT_FreezeTopField();
    SLT_FreezeHdmiFrame(VDP_1, 0);
    SLT_WaitFreezeDone();

    //_u4Mode = _MODE_PLAY_ATV;

    ACK;

    return 0;
}

static INT32 _SltCmdQCvbs(INT32 i4Argc, const CHAR ** szArgv)
{
	if(fgCVBSFail)
	{
		Printf("CVBSFAIL\n");
	}
	else
	{
		Printf("OK\n");
	}

	return 0;
}

#if 0
#define abs(x)                  (((x) >= 0) ? (x) : -(x))

static INT32 _SltCmdCvbsCheck(INT32 i4Argc, const CHAR ** szArgv)
{
    #define BLANK_THRESHOLD 5
    #define SYNCHEIGHT_THRESHOLD 5

    UINT32 u4BlankLvl=0, u4SyncBottomLvl=0, u4ExpectedSyncHeight=0;
    UINT32 u4BlankDiff, u4SyncHeight, u4SyncHeightDiff ;
    UINT8 i = 0, bStatus = 0;
    BOOL fgVPRES = FALSE;
    // 1. Switch input source to cvbs
    CLI_Parser("nav.play m av1");
    WAIT_SIGNAL_STABLE;
    // 2. read registers
    u4ExpectedSyncHeight = (IO_READ32(VIDEO_IN0_BASE, 0x4CC) )& 0x7ff; // 0x200224CC[11:0]
    for(i=0;i<10;i++)//average 10 times, total is 7mS
    {
       u4BlankLvl += IO_READ32(VIDEO_IN0_BASE, 0x044) & 0x3ff; // 0x20022044[9:0]
       u4SyncBottomLvl += (IO_READ32(VIDEO_IN0_BASE, 0x44) & 0x3ff0000) >> 16; // 0x20022044[25:16]
       vUtDelay2us(350);//delay 700uS(about 10 lines) for stable,
    }
    u4ExpectedSyncHeight >>= 2;//12bits->10bits
    u4BlankLvl = u4BlankLvl/10;//average 10 times
    u4SyncBottomLvl = u4SyncBottomLvl/10;//average 10 times
    u4BlankDiff = u4BlankLvl - 256;
    u4SyncHeight = u4BlankLvl -u4SyncBottomLvl;
    u4SyncHeightDiff = u4SyncHeight - u4ExpectedSyncHeight;
    fgVPRES = IO_READ32MSK_BOOL(VIDEO_IN0_BASE, 0x80, 0x1000); // 0x20022080[12]

    if( abs(u4BlankDiff) > BLANK_THRESHOLD )//if blank level is over threshold
        bStatus = bStatus | 0x01;
    if( abs(u4SyncHeightDiff) > SYNCHEIGHT_THRESHOLD )//if sync level is over threshold
        bStatus = bStatus | 0x02;
    if( fgVPRES == 0 ) //if no Video present
        bStatus = bStatus | 0x04;
    if(fgCVBSFail)
        bStatus = bStatus | 0x08;

    Printf("blank level = %d\n", u4BlankLvl);
    Printf("sync bottom level = %d\n", u4SyncBottomLvl);
    Printf("expected sync height = %d\n", u4ExpectedSyncHeight);
    Printf("real sync height = %d\n", u4SyncHeight);
    Printf("blank diff = %5d, sync diff = %5d\n", u4BlankDiff, u4SyncHeightDiff);
    Printf("video present = %u\n", fgVPRES);

    if( bStatus == 0 )
    {
        Printf("CVBS SLT OK\n");
        return 0;
    }
    else
    {
        Printf("CVBS SLT NG status =0x%x\n", bStatus);
        return -1;
    }

}
#endif
//-----------------------------------------------------------------------------
/** Brief		play sv1 and freeze
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdSVideo(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 overscan = (i4Argc)>2?StrToInt(szArgv[2]):30;
    CLI_Parser("slt.bypassmlc 0");
    _u1SltDTV = 1;
#if SLT_MT5371
    _SltReset();
    CLI_Parser("nvgt.sm single");
    CLI_Parser("nvgt.play m sv1");

    WAIT_SIGNAL_STABLE;
    //SLT_FreezeTopField();
    SLT_FreezeHdmiFrame(VDP_1, 1);
    SLT_WaitFreezeDone();

    //_u4Mode = _MODE_PLAY_ATV;
#else
    CLI_Parser("vdp.f 0 1");
    CLI_Parser("vdp.f 0 0");

    CLI_Parser("nav.play m sv1");

    CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);

    WAIT_SIGNAL_STABLE;
    //SLT_FreezeTopField();
    SLT_FreezeHdmiFrame(VDP_1, 1);
    SLT_WaitFreezeDone();

    //_u4Mode = _MODE_PLAY_ATV;
    x_thread_delay(500);
#endif


    ACK;

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief		play ypbpr and freeze
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdYpbpr(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):100;
    INT32 delay2 = (i4Argc)>2?StrToInt(szArgv[2]):0;
    INT32 overscan = (i4Argc)>3?StrToInt(szArgv[3]):25;
    INT32 delay3 = (i4Argc)>4?StrToInt(szArgv[4]):100;
    //INT32 fgMjcOn = (i4Argc)>8?StrToInt(szArgv[8]):0;

    CLI_Parser("slt.bypassmlc 0");
    CLI_Parser("vdp.f 0 1");
    CLI_Parser("vdp.f 0 0");

    CLI_Parser("nav.play m ypbpr1");

    CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);
    WAIT_SIGNAL_STABLE;

    x_thread_delay(delay2);

    _SltAutoColor(VDP_1);
    //SLT_FreezeTopField();

    // Stupid delay
    x_thread_delay(delay3);

    SLT_FreezeHdmiFrame(VDP_1, 0);
    SLT_WaitFreezeDone();
    //_u4Mode = _MODE_PLAY_ATV;

    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("YBROK\n"); \
    } \
    while (0);

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief		play hdmi and check CRC if flag is enable
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdHdmi(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 bypass = (i4Argc)>2?StrToInt(szArgv[2]):0;
    INT32 port = (i4Argc)>3?StrToInt(szArgv[3]):1;
    INT32 crc = (i4Argc)>4?StrToInt(szArgv[4]):0;
    INT32 delay2 = (i4Argc)>5?StrToInt(szArgv[5]):1000;
    INT32 delay3 = (i4Argc)>6?StrToInt(szArgv[6]):1000;
    INT32 overscan = (i4Argc)>7?StrToInt(szArgv[7]):250;

    CLI_Parser("vdp.f 0 1");
    CLI_Parser("vdp.f 0 0");

    if(!bypass)
	{
	    // HDMI3 has better impedence
	    if(port == 1)
	    {
            CLI_Parser("n.ms hdmi1");
	    }
	    else if(port == 2)
	    {
            CLI_Parser("n.ms hdmi2");
	    }
	    else if(port == 3)
	    {
            CLI_Parser("n.ms hdmi3");
	    }
	    else if(port == 4)
	    {
            CLI_Parser("n.ms hdmi4");
	    }
		else
		{
		    Printf("[SLT] error! hdmi port >4");
		}
	}
    // why this???
    //CLI_Parser("nav.sm pip");
    //CLI_Parser("n.ps off");

    x_thread_delay(delay2);
    //CLI_Parser("w 0x20023880 0x20000082"); // bypass NR
    //vApiVideoNRTIIROnOff(SV_OFF);
    //vDrvPreSharpMainOnOff(SV_OFF);

    CLI_Parser("vdp.s.srcr 0 1");
    CLI_Parser("vdp.s.outr 0 1");

    CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);
    //CLI_Parser("vdp.s.121 0 1");

    //CLI_Parser_Arg("vdp.s.bob 0 %d", bob);

    x_thread_delay(delay3);

    while (_bDviModeChged)
    {
        x_thread_delay(100);
    }

    if(crc)
    {
        if (fgHDMICRC(10) == FALSE)
        {
            Printf("HDMI CRC check fail!\n");
            CLI_Parser("pmx.p 1");
            Printf("HDMIFAIL\n");
            return -1;
        }
    }
    //SLT_BypassPQFunc(0, bypassMask);
    //x_thread_delay(100);

    WAIT_SIGNAL_STABLE;
	//vDrvNRBypass(SV_ON);
    vDrvPreSharpOnOff(SV_VP_MAIN, SV_OFF);
    vDrvPostSharpOnOff(SV_OFF);

    //CLI_Parser("slt.bypassmlc 1"); // bypass MLC

    //SLT_FreezeHdmiFrame(VDP_1, 0);
    //SLT_WaitFreezeDone();
    //_u4Mode = _MODE_PLAY_ATV;


    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("HDMIOK\n"); \
    } \
    while (0);

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief		play tve
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdTve(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 port = (i4Argc)>2?StrToInt(szArgv[2]):1;
    INT32 delay2 = (i4Argc)>3?StrToInt(szArgv[3]):1000;
    //INT32 delay3 = (i4Argc)>4?StrToInt(szArgv[4]):1000;

    switch(port)
    {
        case 1:
            if(_bSrcMainNew == SV_VS_HDMI1 && _bSrcSubNew == SV_VS_HDMI1)
              goto TVE_READY_HDMI;
            break;
        case 2:
            if(_bSrcMainNew == SV_VS_HDMI2 && _bSrcSubNew == SV_VS_HDMI2)
              goto TVE_READY_HDMI;
            break;
        case 3:
            if(_bSrcMainNew == SV_VS_HDMI3 && _bSrcSubNew == SV_VS_HDMI3)
              goto TVE_READY_HDMI;
            break;
        default:
            break;
    }

    CLI_Parser("vdp.f 0 1");
    CLI_Parser("vdp.f 0 0");

    CLI_Parser("vdp.f 1 1");
    CLI_Parser("vdp.f 1 0");


    // HDMI3 has better impedence
    if(port == 1)
    {
        CLI_Parser("tve.cs 1 hdmi1");
    }
    else if(port == 2)
    {
        CLI_Parser("tve.cs 1 hdmi2");
    }
    else if(port == 3)
    {
        CLI_Parser("tve.cs 1 hdmi3");
    }
    // why this???
    //CLI_Parser("nav.sm pip");
    //CLI_Parser("n.ps off");

    //x_thread_delay(delay2);
    //CLI_Parser("w 0x20023880 0x20000082"); // bypass NR
    //vApiVideoNRTIIROnOff(SV_OFF);
    //vDrvPreSharpMainOnOff(SV_OFF);

    //CLI_Parser("vdp.s.srcr 0 1");
    //CLI_Parser("vdp.s.outr 0 1");

    //CLI_Parser("vdp.s.overscan 0 25 25 25 25");
    //CLI_Parser("vdp.s.121 0 1");

    //CLI_Parser_Arg("vdp.s.bob 0 %d", bob);

    //_SltBypassPQFunc(0, bypassMask);

    //x_thread_delay(delay3);
    do
    {
        x_thread_delay(100);
    } while (_bDviModeChged);
/*

    if(crc)
    {
        if (fgHDMICRC(10) == FALSE)
        {
            Printf("HDMI CRC check fail!\n");
            CLI_Parser("pmx.p 1");
        }
    }
*/
    x_thread_delay(delay2);

TVE_READY_HDMI:
    //SLT_FreezeHdmiFrame(VDP_1, 0);
    //SLT_WaitFreezeDone();

    //_u4Mode = _MODE_PLAY_ATV;
#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT
    TVE_SLT_SRCREMAP_EN = 1;
#endif
    CLI_Parser("n.ms off");
    //CLI_Parser("n.ps off");
    CLI_Parser("n.ms scart1");
    CLI_Parser("vdp.s.overscan 0 1000 1000 1000 1000");

    //osd not support tve scart out in 5396 A Ver.
    //osd related cli
    //CLI_Parser("osdt.pt 1 4 720 576 720 576 32");
    //CLI_Parser("osd.tve 1");//osd not support tve scart out in 5396 A Ver.
    CLI_Parser("tve.pm 0 1");
    CLI_Parser("osd.p.fa 1 50");

     // HDMI3 has better impedence
/*     if(port == 1)
     {
         CLI_Parser("n.ps hdmi1");
     }
     else if(port == 2)
     {
         CLI_Parser("n.ps hdmi2");
     }
     else if(port == 3)
     {
         CLI_Parser("n.ps hdmi3");
     }          */

    //x_thread_delay(delay);

    WAIT_SIGNAL_STABLE;

    do
    {
        if (delay > 0)
        {
            x_thread_delay(delay);
        }
        Printf("TVEOK\n");
    }
    while (0);

    //IO_WRITE32(IO_VIRT, 0x23800, 0x60100070); // bypass NR to prevent noise in main path

    //TVE_SLT_SRCREMAP_EN = 0;

    return 0;

}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdBypassPQ(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4OnOff = (i4Argc)>1?StrToInt(szArgv[1]):0;

	if(i4OnOff)
	{
		Printf("all PQ on\n");
	}
	else
	{
		Printf("all PQ bypass\n");
	}
	vApiVideoAllPQItemOnOff((UINT8)i4OnOff);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief		play vga and do auto position & auto color
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdVga(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):2000;
    //INT32 delay2 = (i4Argc)>2?StrToInt(szArgv[2]):0;
    //INT32 delay3 = (i4Argc)>2?StrToInt(szArgv[3]):0;

    CLI_Parser("vdp.f 0 1");
    CLI_Parser("vdp.f 0 0");

    CLI_Parser("slt.bypassmlc 1");

    CLI_Parser("nav.play m vga");

    CLI_Parser("vdp.s.overscan 0 0 0 0 0");

    WAIT_SIGNAL_STABLE;


    _SltAutoColor(VDP_1);
    _SltAuto(VDP_1);

    //_u4Mode = _MODE_PLAY_ATV;

    //x_thread_delay(delay2);
    //VDP_SetFreeze(0, 1);

    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("VGAOK\n"); \
    } \
    while (0);

    return 0;
}



//-----------------------------------------------------------------------------
/** Brief		show pre-defined osd pattern
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdOsd(INT32 i4Argc, const CHAR ** szArgv)
{
    //INT32 mode = StrToInt(szArgv[1]);
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
    //_SltReset();

    //test OSD plane 1 2 3
    CLI_Parser("osdt.pt 3 4 960 320 1920 1080");
    //CLI_Parser("osdt.pt 2 5 720 480 1920 1080 32 0 0");
    CLI_Parser("osd.p.fa 3 150");

    //_u4Mode = _MODE_PLAY_OSD;

    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("OSDOK\n"); \
    } \
    while (0);

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdMpegUnlock(INT32 i4Argc, const CHAR * * szArgv)
{
    if(_hDtdAtscSema == NULL)
    {
        if(x_sema_create(&_hDtdAtscSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            return -1;
        }
    }
    else
    {
       x_sema_unlock(_hDtdAtscSema);
    }
    return 0;
}
//-----------------------------------------------------------------------------
/** Brief		dtv test command to play dtv and freeze at certain pts
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
extern VDP_CONF_T _arVdpConf[];
#define SLT_DTV_FREEZE_PTS
static INT32 _SltCmdMpeg(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 pts = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 vpid = (i4Argc)>2?StrToInt(szArgv[2]):33;
    INT32 delay = (i4Argc)>3?StrToInt(szArgv[3]):500;
#ifndef SLT_DTV_FREEZE_PTS
    INT32 bypassMask = (i4Argc)>4?StrToInt(szArgv[4]):0;
#endif
    INT32 freq = (i4Argc)>5?StrToInt(szArgv[5]):50;
    INT32 vtype = (i4Argc)>6?StrToInt(szArgv[6]):2;
    INT32 overscan = (i4Argc)>7?StrToInt(szArgv[7]):0;

    CLI_Parser("vdp.f 0 1");
    CLI_Parser("vdp.f 0 0");
#ifdef SLT_DTV_FREEZE_PTS
    CLI_Parser("slt.ufpts");
#endif
    CLI_Parser("slt.bypassmlc 0");

	//CLI_Parser("mpv.dbk 0 1");
	CLI_Parser_Arg("nav.play m dtv %d 0 %d atsc %d %d", vpid, vpid, freq, vtype);
    x_thread_delay(500);
    CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);

#ifndef SLT_DTV_FREEZE_PTS
    SLT_FreezeMpegFrame(VDP_1, (pts == 0)?198111:pts, bypassMask);
    SLT_WaitFreezeDone();

#else
	CLI_Parser_Arg("slt.fpts %d", pts);
	//polling flag to see if it's stopped
	while(!fgSltNotSendPic)
	{
		x_thread_delay(20);
	}
#endif
	vDrvPreSharpOnOff(VDP_1, SV_OFF);// turn off post sharpness
	vDrvPostSharpOnOff(SV_OFF); // turn off post sharpness
	//vDrvNRBypass(SV_ON);
    //_u4Mode = _MODE_PLAY_MPEG;

    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("SLTOK\n"); \
    } \
    while (0);

    _u1SltDTV = 0;

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdPip(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 pts = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 vpid = (i4Argc)>2?StrToInt(szArgv[2]):33;
    INT32 port = (i4Argc)>3?StrToInt(szArgv[3]):1;
    INT32 bob = (i4Argc)>4?StrToInt(szArgv[4]):0;
    INT32 hdmi_bypass = (i4Argc)>5?StrToInt(szArgv[5]):0;
    INT32 overscan = (i4Argc)>6?StrToInt(szArgv[6]):0;
    INT32 delay = (i4Argc)>7?StrToInt(szArgv[7]):500;
    INT32 delay2 = (i4Argc)>8?StrToInt(szArgv[8]):5000;
    INT32 delay3 = (i4Argc)>9?StrToInt(szArgv[9]):500;

    _u4OverscanRatio = 0;

    CLI_Parser("slt.bypassmlc 0");

    _u1SltDTV = bob;

    if (bob)
    {
        CLI_Parser("vdp.s.bob 0 1");
    }

    if(!hdmi_bypass)
    {

        // HDMI3 has better impedence
        if(port == 1)
        {
            CLI_Parser("nav.play m hdmi1");
        }
        else if(port == 2)
        {
            CLI_Parser("nav.play m hdmi2");
        }
        else
        {
            CLI_Parser("nav.play m hdmi3");
        }

        CLI_Parser("nav.sm pip");

        x_thread_delay(delay2);

        CLI_Parser("vdp.s.srcr 0 1");
        CLI_Parser("vdp.s.outr 0 1");
        CLI_Parser("vdp.s.overscan 0 25 25 25 25");

        x_thread_delay(delay3);

        SLT_FreezeHdmiFrame(VDP_1, 0);
        SLT_WaitFreezeDone();
        //_u4Mode = _MODE_PLAY_ATV;

    }
    else
    {
        CLI_Parser("vdp.f 0 0");
        CLI_Parser("nav.sm pip");
    }

    CLI_Parser("mpv.dbk 0 0");

    CLI_Parser_Arg("nav.play s dtv %d 0 %d dvbt %d", vpid, vpid, 50);
    CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);
    SLT_FreezeMpegFrame(VDP_2, (pts == 0)?72000:pts, 0);
    SLT_WaitFreezeDone();
    //_u4Mode = _MODE_PLAY_MPEG;


    if(hdmi_bypass)
    {

        x_thread_delay(delay3);

        SLT_FreezeHdmiFrame(VDP_1, 0);
        SLT_WaitFreezeDone();
        //_u4Mode = _MODE_PLAY_ATV;
    }

    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("SLTOK\n"); \
    } \
    while (0);

    _u1SltDTV = 0;

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdPop(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 pts = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 vpid = (i4Argc)>2?StrToInt(szArgv[2]):33;
    INT32 port = (i4Argc)>3?StrToInt(szArgv[3]):1;
    INT32 bob = (i4Argc)>4?StrToInt(szArgv[4]):1;
    INT32 mpeg_bypass = (i4Argc)>5?StrToInt(szArgv[5]):0;
    INT32 delay = (i4Argc)>6?StrToInt(szArgv[6]):500;
    INT32 delay2 = (i4Argc)>6?StrToInt(szArgv[7]):100;


    //_u4OverscanRatio = (i4Argc)>6?StrToInt(szArgv[6]):0;
    _u4OverscanRatio = 0;


    CLI_Parser("slt.bypassmlc 0");

    _u1SltDTV = bob;

    if (bob)
    {
        CLI_Parser("vdp.s.bob 0 1");
    }


    if(!mpeg_bypass)
    {
        CLI_Parser("mpv.dbk 0 0");
        CLI_Parser("nav.sm pop");

        CLI_Parser_Arg("nav.play m dtv %d 0 %d atsc %d", vpid, vpid, 50);

        SLT_FreezeMpegFrame(VDP_1, (pts == 0)?126039:pts, 0);
        SLT_WaitFreezeDone();
        //_u4Mode = _MODE_PLAY_MPEG;
    }
    else
    {
        CLI_Parser("vdp.f 0 0");
        CLI_Parser("nav.sm pop");
    }

    // HDMI3 has better impedence
    if(port == 1)
    {
        CLI_Parser("nav.play s hdmi1");
    }
    else if(port == 2)
    {
        CLI_Parser("nav.play s hdmi2");
    }
    else
    {
        CLI_Parser("nav.play s hdmi3");
    }


    CLI_Parser("vdp.s.srcr 1 1");
    CLI_Parser("vdp.s.overscan 1 25 25 25 25");

    do
    {
        x_thread_delay(100);
    } while (_bDviModeChged);

    x_thread_delay(delay2);

    SLT_FreezeHdmiFrame(VDP_2, 0);
    SLT_WaitFreezeDone();
    //_u4Mode = _MODE_PLAY_ATV;

    //CLI_Parser("n.ps off");
    //x_thread_delay(5000);


    if(mpeg_bypass)
    {
        SLT_FreezeMpegFrame(VDP_1, (pts == 0)?126039:pts, 0);
        SLT_WaitFreezeDone();
        //_u4Mode = _MODE_PLAY_MPEG;
    }


    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("SLTOK\n"); \
    } \
    while (0);

    _u1SltDTV = 0;

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief		test pop mode, combination is pre-defined
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdPopA(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 mode = (i4Argc)>1?StrToInt(szArgv[1]):0; // mode 0: CVBS+VGA, mode 1: Svideo+Ypbpr
	INT32 delay = (i4Argc)>2?StrToInt(szArgv[2]):500;
	INT32 delay2 = (i4Argc)>2?StrToInt(szArgv[3]):500;

	//CLI_Parser("slt.bypassmlc 0");
	CLI_Parser("vdp.f 0 1");
	CLI_Parser("vdp.f 0 0");
	CLI_Parser("vdp.f 1 1");
	CLI_Parser("vdp.f 1 0");
    //_u1SltDTV = 1;

	if(mode == 0) // SV+VGA
	{
	    //CLI_Parser("slt.bypassmlc 1");

	    CLI_Parser("nav.sm pop");

#if CC_ECO_IC
	    CLI_Parser("nav.play m vga");
	    //CLI_Parser("vdp.s.overscan 0 25 25 25 25");
	    CLI_Parser("nav.play s sv1");
            WAIT_SIGNAL_STABLE;
            WAIT_SIGNAL_STABLE_PIP;
	    CLI_Parser("vdp.s.overscan 1 50 50 600 100");
#else  //pscan can't do 3dYC in sub path, swap the two source
        CLI_Parser("nav.play m sv1");
        CLI_Parser("nav.play s vga");
        WAIT_SIGNAL_STABLE;
        WAIT_SIGNAL_STABLE_PIP;
        CLI_Parser("vdp.s.overscan 0 50 50 600 600");
#endif
        if (!DRVCUST_OptGet(eEFuseAutocolor))
        {
#if CC_ECO_IC
	        _SltAutoColor(VDP_1);
#else  //pscan can't do 3dYC in sub path, swap the two source
            _SltAutoColor(VDP_2);
#endif

        }
#if CC_ECO_IC
        _SltAuto(VDP_1);
#else  //pscan can't do 3dYC in sub path, swap the two source
            _SltAuto(VDP_2);
#endif
        x_thread_delay(delay2);
	    //SLT_FreezeHdmiFrame(VDP_2, 0);
	    //SLT_WaitFreezeDone();
	    //_u4Mode = _MODE_PLAY_ATV;
		// end cvbs
	}
	else if(mode == 1) // Cvbs+Ypbpr
	{
	    CLI_Parser("nav.sm pip");

	    CLI_Parser("nav.play m av1");
        WAIT_SIGNAL_STABLE;
	    CLI_Parser("vdp.s.overscan 0 50 50 600 100");
	    CLI_Parser("nav.play s ypbpr1");
        WAIT_SIGNAL_STABLE_PIP;
        if (!DRVCUST_OptGet(eEFuseAutocolor))
        {
#if 0 //YPBPR signal is not standard, can't cal the gain/offset
	        _SltAutoColor(VDP_2);
#endif
        }
        _SltAuto(VDP_2);
        CLI_Parser("vdp.s.overscan 1 50 50 600 50");
	    x_thread_delay(delay2);
	    //SLT_FreezeHdmiFrame(VDP_1, 0);
	    //SLT_WaitFreezeDone();
	    //SLT_FreezeHdmiFrame(VDP_2, 0);
	    //SLT_WaitFreezeDone();


	    //_u4Mode = _MODE_PLAY_ATV;
		// End Svideo

	}

    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("SLTOK\n"); \
    } \
    while (0);

	return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdAud(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
#if 0 // by hotkey 0729 for compile
    CLI_Parser("pmx.p 1");
#if SLT_MT5371
    if (SLT_AudGetStressTestResult() == 1)
    {
        Printf("audio stress test pass\n");
    }
    else
    {
        CLI_Parser("pmx.p 0");
        Printf("audio stress test fail\n");
    }
#else
    SLT_DtvSorting();
    x_thread_delay(delay);
    SLT_IntAdSorting() ;
#endif
#endif
    ACK;
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdPll(INT32 i4Argc, const CHAR ** szArgv)
{

    INT32 dramclk = (i4Argc)>1?StrToInt(szArgv[1]):(DRVCUST_InitGet(eDdrClock));
	INT32 PixelClk = PANEL_GetPixelClk50Hz();//PANEL_GetPixelClk60Hz();

	Printf("dramclk=%d\n", dramclk);
	Printf("IS_LVDS_HIGHSPEED=%d\n", IS_LVDS_HIGHSPEED);

	if ( IS_LVDS_HIGHSPEED )
	{
	    PixelClk /= 2;
	}
    Printf("PANEL_GetPixelClk60Hz=%d\n", PixelClk);

    _SltTestPll(SRC_ETHPLL_CLK, ETHNETPLL_DFT_FREQ, CLK_BOUND);
    _SltTestPll(SRC_SYSPLL_CLK, SYSPLL_DFT_FREQ, CLK_BOUND);
    _SltTestPll(CAL_SRC_AD_VOPLL_DPIX_CK, PixelClk, CLK_BOUND);//vopll
    _SltTestPll(SRC_CPU_CLK, CPUCLK_DFT_FREQ, CLK_BOUND);
    _SltTestPll(CAL_SRC_SAWLESSPLL, SAWLESSPLL_DFT_FREQ, CLK_BOUND);//IFPLL
    _SltTestPll(CAL_SRC_APLL1, CLK_APLL294MHZ, CLK_BOUND);
    _SltTestPll(CAL_SRC_APLL2, CLK_APLL270MHZ, CLK_BOUND);
    _SltTestPll(CAL_SRC_APLL2, CLK_APLL294MHZ, CLK_BOUND);
    _SltTestPll(CAL_SRC_USBPLL, USBPLL_DFT_FREQ, CLK_BOUND);
    _SltTestPll(SRC_MEM_CLK, dramclk/4, CLK_BOUND);

    //_SltTestPll(CAL_SRC_PIXPLL, PIXPLL_DFT_FREQ, CLK_BOUND); //96 new mLVDS/VB1/EPI

    //CAL_SRC_VDECPLL

    //SRC_VDOIN_MPCLK
    //SRC_BUS_CLK
    //CAL_SRC_ADCPLL



    return 0;


}


//-----------------------------------------------------------------------------
/** Brief		misc self-test
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdMisc(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 skipMask = (i4Argc)>1?StrToInt(szArgv[1]):0x0;
    INT32 dramclk = (i4Argc)>2?StrToInt(szArgv[2]):(DRVCUST_InitGet(eDdrClock));
    INT32 parallel_aud = (i4Argc)>3?StrToInt(szArgv[3]):1;
    INT32 parallel_misc = (i4Argc)>4?StrToInt(szArgv[4]):1;
    //UINT32 u4RingOscClock, u4RingOscTick, u4SystemClock;
    UCHAR ucCode;// = '0';

    ucCode = '0';
	_i4SkipMask = skipMask; // to keep skip mask for audio test
    if (skipMask == 0xffff)
    {
        Printf("<Selftest:0/00000/0000/0000/123456789\n");
        //_u4Mode = _MODE_PLAY_MISC;
        return 0;
    }

    if (((skipMask & 0x1) == 0) &&
        _SltTestPll(SRC_ETHPLL_CLK, ETHNETPLL_DFT_FREQ, CLK_BOUND))
    {
        ucCode = '1';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(SRC_SYSPLL_CLK, SYSPLL_DFT_FREQ, CLK_BOUND))
    {
        ucCode = '2';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(CAL_SRC_AD_VOPLL_DPIX_CK, PANEL_GetPixelClk60Hz(), CLK_BOUND))//vopll
    {
        ucCode = '3';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(SRC_CPU_CLK, CPUCLK_DFT_FREQ, CLK_BOUND))
    {
        ucCode = '4';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(CAL_SRC_SAWLESSPLL, SAWLESSPLL_DFT_FREQ, CLK_BOUND))
    {
        ucCode = '5';
    }
    else if (((skipMask & 0x1) == 0) &&
       // ( _SltTestPll(CAL_SRC_APLL1, CLK_APLL270MHZ, CLK_BOUND) ||
          _SltTestPll(CAL_SRC_APLL1, CLK_APLL294MHZ, CLK_BOUND)) //)
    {
        ucCode = '6';
    }
    else if (((skipMask & 0x1) == 0) &&
         ( _SltTestPll(CAL_SRC_APLL2, CLK_APLL270MHZ, CLK_BOUND) ||
             _SltTestPll(CAL_SRC_APLL2, CLK_APLL294MHZ, CLK_BOUND)) )

    {
        ucCode = '7';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(CAL_SRC_USBPLL, USBPLL_DFT_FREQ, CLK_BOUND))
    {
        ucCode = '8';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(SRC_MEM_CLK, dramclk/4, CLK_BOUND))
    {
        ucCode = '9';
    }
    /*else if((skipMask & 0x2000) == 0 && _SltTestVPllSS(20, 100, 10) != 0)
    {
        ucCode = 'v';
    } */
   else  if ((skipMask & 0x10) == 0 && CLI_Parser("sif.diag") != 0)
    {
        ucCode = 's';
    }
    else if ((skipMask & 0x20) == 0 && CLI_Parser("gfx.slt") != 0)
    {
        ucCode = 'g';
    }
    else if ((skipMask & 0x40) == 0 && CLI_Parser("imgrz.slt") != 0)
    {
        ucCode = 'h';
    }
    else if ((skipMask & 0x80) == 0 && DMX_Diag() != 0) //
    {
        ucCode = 'd';
    }
#if 0
    else if ((skipMask & 0x100) == 0 && CLI_Parser("pmx.diag.gt") != 0) // Gamma table
    {
        ucCode = 'm';
    }
    else if((skipMask & 0x400) == 0 && CLI_Parser("jpg.slt") != 0) // JPEG
    {
        ucCode = 'j';
    }
#endif
    else if((skipMask & 0x800) == 0 &&  CLI_Parser("slt.t8032") != 0)  // up alive test
    {
        ucCode = 't';
    }   
    else if(parallel_misc == 0 && (skipMask & 0x1000) == 0 && CLI_Parser("slt.eth") != 0)
    {
        ucCode = 'e';
    }
    else if(parallel_misc == 0 && (skipMask & 0x200) == 0 && MUSB_CheckSLT() != 0)
    {
        ucCode = 'u';
    }
    else if((skipMask & 0x2000) == 0 &&  CLI_Parser("gdma.slt") != 1)//gdma
    {
        ucCode = 'a';
    }

    if(parallel_misc != 0 && ((skipMask & 0x1000) == 0 /*ethernet*/|| (skipMask & 0x200) == 0 /*USB*/))
    {
        if(_hMiscFinSema == NULL)
        {
            if(x_sema_create(&_hMiscFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
            {
                ucCode ='y';
            }
        }

        if(_hMiscTrigSema == NULL)
        {
            if(x_sema_create(&_hMiscTrigSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
            {
                ucCode ='y';
            }
        }

        if(_hMiscThread == NULL)
        {
            if(x_thread_create(&_hMiscThread, "SLT_MISC_PARA", 8192, 99, _MiscProc, 0, NULL) != OSR_OK)
            {
                ucCode ='y';
            }
        }
        x_sema_unlock(_hMiscTrigSema);
    }

    if((skipMask & 0x2) == 0 || (skipMask & 0x4) == 0 || (skipMask & 0x8) == 0)
    {
        if(!parallel_aud)
        {
            if (((skipMask & 0x8) == 0) && (SLT_IntAdSorting() != 0))
            {
                Printf("IAD Soring result (fail)\n");
                ucCode = 'c';
            }
            // analog audio test
            else if (((skipMask & 0x2) == 0) && (SLT_DtvSorting() != 0))
            {
                Printf("DTV Soring result (fail)\n");
                ucCode = 'd';
            }
        }
        else
        {
            if(x_sema_create(&_hAudFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
                return -1;
             if (x_thread_create(&_hAudThread, "SLT_AUD_PARA", 8192, 99, _AudProc, 0, NULL) != OSR_OK)
             {
                 _ucAudErrorCode = 'f';
             }
        }
    }
    //Printf("<Selftest:%c/%05d/%04x/%04x/123456789\n", ucCode, u4RingOscClock/1000, port1pass, port2pass);
    Printf("<Selftest:%c/%05d/%04x/%04x/123456789\n", ucCode, 0, 0, 0);

    //_u4Mode = _MODE_PLAY_MISC;

    return 0;
}




//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdShowPll(INT32 i4Argc, const CHAR ** szArgv)
{
 /*   UINT32 u4Clock;
    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4Clock = BSP_GetClock(SRC_SYSPLL_CLK, NULL, NULL, NULL, NULL);
    Printf("SYSPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_LVDS_DPIX_CK, NULL, NULL, NULL, NULL);
    Printf("VOPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_PIXPLL, NULL, NULL, NULL, NULL);
    Printf("PIXPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_HDMI_PLL340M, NULL, NULL, NULL, NULL);
    Printf("HDMIPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_USBPLL, NULL, NULL, NULL, NULL);
    Printf("USBPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_SAWLESSPLL, NULL, NULL, NULL, NULL);
    Printf("SAWLESSPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(SRC_CPU_CLK, NULL, NULL, NULL, NULL);
    Printf("CPUPLL=%d, ", u4Clock);
#if 0
    u4Clock = BSP_GetClock(CAL_SRC_APLL0, NULL, NULL, NULL, NULL);
    Printf("APLL0=%d, ", u4Clock);
#endif
    u4Clock = BSP_GetClock(CAL_SRC_APLL1, NULL, NULL, NULL, NULL);
    Printf("APLL1=%d\n", u4Clock);

    u4Clock = BSP_GetClock(CAL_SRC_APLL2, NULL, NULL, NULL, NULL);
    Printf("APLL2=%d, ", u4Clock);
   */
    return 0;
}


// this is audio test thread content
static void _AudProc(void* pvArg)
{
	x_thread_delay(2000);

    if (((_i4SkipMask & 0x8) == 0) && (SLT_IntAdSorting() != 0))
    {
        Printf("IAD Soring result (fail)\n");
        _ucAudErrorCode = 'c';
    }
    else if (((_i4SkipMask & 0x2) == 0) && (SLT_DtvSorting() != 0))
    {
        Printf("DTV Soring result (fail)\n");
        _ucAudErrorCode = 'd';
    }
    // digital audio test
   	x_sema_unlock(_hAudFinSema);
}

// query audio test result
static INT32 _SltCmdQAud(INT32 i4Argc, const CHAR ** szArgv)
{
	if(x_sema_lock_timeout(_hAudFinSema, DTD_SLT_TIMEOUT) == OSR_OK)
	{
		if(_ucAudErrorCode != 0)
			Printf("FAIL:%c\n", _ucAudErrorCode);
		else
			Printf("AUDOK\n");
	}
	else
            Printf("FAIL:x\n");
	return 0;
}


static void _MiscProc(void *pvArg)
{
    const char *nullstr = "";
    do {
        // wait a while to make sure the usb/ethernet is connected
        x_sema_lock(_hMiscTrigSema, X_SEMA_OPTION_WAIT);

        x_thread_delay(3000);

        if((_i4SkipMask & 0x1000) == 0 && _i4EtherSltTest(1, &nullstr) != 0)
        {
            _ucMiscErrorCode = 'e';
        }
        else if((_i4SkipMask & 0x200) == 0 && MUSB_CheckSLT() != 0)
        {
            _ucMiscErrorCode = 'u';
        }
        x_sema_unlock(_hMiscFinSema);
    }while(1);
}

static INT32 _SltCmdQMisc(INT32 i4Argc, const CHAR ** szArgv)
{
    if(_hMiscFinSema == NULL)
    {
        Printf("MISCOK (ignored)\n");
    }
    else if(x_sema_lock_timeout(_hMiscFinSema, DTD_SLT_TIMEOUT) == OSR_OK)
    {
        if(_ucMiscErrorCode == 0)
        {
            Printf("MISCOK\n");
        }
        else
        {
            Printf("FAIL:%c\n",_ucMiscErrorCode);
        }
        x_sema_unlock(_hMiscFinSema);
    }
    else
    {
        Printf("FAIL:%c\n",_ucMiscErrorCode);
    }
    return 0;
}
//-----------------------------------------------------------------------------
/** _GetDTDTime
 *  @param  u1DemodNum
 *  @param  u1Element    Valid range:0~19
 *  @return void
 */
//-----------------------------------------------------------------------------

static void _GetDTDTime(UINT8 u1DemodNum, UINT8 u1Element)
{
HAL_TIME_T rCurrent;

	HAL_GetTime(&rCurrent);

    DTD_ProcessTime[u1DemodNum][u1Element] = rCurrent;
	Printf("Time(%d, %d): %ds:%dus\n", u1DemodNum, u1Element, DTD_ProcessTime[u1DemodNum][u1Element].u4Seconds, DTD_ProcessTime[u1DemodNum][u1Element].u4Micros);

}


static void _RstDTDTime(UINT8 u1DemodNum, UINT8 u1Element)
{
    DTD_ProcessTime[u1DemodNum][u1Element].u4Seconds = 0;
    DTD_ProcessTime[u1DemodNum][u1Element].u4Micros = 0;
}


static void _WaitSysTimeMs(HAL_TIME_T rStart, UINT16 u2WaitTime_ms)
{
HAL_TIME_T rCurrent2, rDelta;

    while(1)
    {
        x_thread_delay(100); // 100 ms
        HAL_GetTime(&rCurrent2);

        HAL_GetDeltaTime(&rDelta, &rStart, &rCurrent2);

	    //Printf("_WaitSysTimeMs: %d.%6d, %d.%6d\n", rCurrent2.u4Seconds, rCurrent2.u4Micros, rDelta.u4Seconds, rDelta.u4Micros);

        if ( (rDelta.u4Seconds*1000*10 + rDelta.u4Micros/100) >= u2WaitTime_ms*10) // (base: 100us)
        	break;

    }
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
#if DTD_PARALLEL_TEST // 2009.2.23
static void _DtdProc(void* pvArg)
{
	SLT_DTD_T rDtd = *((SLT_DTD_T*)pvArg);

	static CHAR _azDemodType[3][8] = {"atsc", "calbe64", "cable256"};//EC_code_2007/6/22: Modulatio type: ATSC0x00, 64QAM0x01, 256QAM0x02
	UINT32 _azDemodFreq[3] = {rDtd.atsc_freq, 0, rDtd.qam_freq};
	int i, jj;
	//CHAR mode_type;
	//UINT8 u1SignalLevel;
	UINT16 u2SignalSNR;
	INT16 i2Sync;


       //bApiVideoSetVideoSrc(0, SV_VS_DT1)
       // Initialize
       for (i = 0; i < 2; i++)
       {
	       u2SignalPER[i] = 0xFFFF;
           for (jj = 0; jj < 20 ; jj++)
	       {
	         u2PERRecord[i][jj] = 0xFFFF;
	       }

           for (jj = 0; jj < 10; jj++)
           {
              _RstDTDTime(i, jj);
           }

           //u4DTDRetryRecord[i] = 10;
       }

     _GetDTDTime(0, 0);

	_fgDtdUnsync = FALSE; //EC_code_2007/6/25: For initial state
	{
                     CLI_Parser_Arg("nim.id 1");
			//CLI_Parser_Arg("nim.go dvbt");   // default run DVBT
			CLI_Parser_Arg("nim.c");            // free context
			for (i = 0; i < rDtd.num; i ++)
			{
				UINT32 u4Retry = 10;
				CLI_Parser_Arg("nim.go %s", _azDemodType[(rDtd.type1 + i*2) % 4]);
				CLI_Parser_Arg("nim.freq %d", _azDemodFreq[(rDtd.type1 + i*2) % 4]); // Set modulation type & channel frequency: ATSC @ 201MHz, QAM256 @ 213MHz
				if(rDtd.fgAtscFirst)// Should call this to notify MPEG SLT thread to start;
			       {
        				//x_sema_unlock(_hDtdAtscSema); //by hotkey to continue main thread
				}
				else
				{
				       rDtd.fgAtscFirst = TRUE;
				}
				//x_sema_unlock(_hDtdAtscSema); // by hotkey to continue main thread

				x_thread_delay(1000);

  	            _GetDTDTime(i, 1);

				//Check Demod lock status
				while( (i2Sync= NIM_GetCableSync()) == 0 )
				{
				    if(u4Retry-- == 0)
				    {
				        break;
				    }
				    x_thread_delay(500);
				}

  	            _GetDTDTime(i, 2);
          	    Printf("Check_Demod_Retry_%d\n", u4Retry);
                //u4DTDRetryRecord[i]= u4Retry;

				Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", _azDemodType[(rDtd.type1 + i*2) % 4], (INT32)i2Sync, (INT32)_fgDtdUnsync);

#if 0
				if (i2Sync)
#endif
					{
						x_thread_delay((rDtd.delay-1)*1000);

    	                _GetDTDTime(i, 3);

						Printf("========PEC START=======(Timer: %d sec)\n", rDtd.delay);
						Printf("duration is %d, delay is %d, and PEC_time is %d\n", rDtd.duration, rDtd.delay, (rDtd.duration-rDtd.delay-rDtd.PEC_delay));

						u2SignalSNR  = NIM_GetCableSignalSNR();
						Printf("%s cable SNR : %d\n", _azDemodType[i], u2SignalSNR);
						CLI_Parser_Arg("nim.dl 1");
						Printf("nim.dtd y %d 1\n", (rDtd.duration-rDtd.delay-rDtd.PEC_delay)); //EC_code_2007/7/19: For RF playback with single channel
						CLI_Parser_Arg("nim.dtd y %d 1", (rDtd.duration-rDtd.delay-rDtd.PEC_delay));
						//x_thread_delay((rDtd.duration-rDtd.delay-rDtd.PEC_delay)*1000);
						for (jj = 0; jj < (rDtd.duration-rDtd.delay-rDtd.PEC_delay); jj++)
						{
						    x_thread_delay(1000);
						    u2PERRecord[i][jj] = NIM_GetCableSignalPER();
						}
						Printf("========PEC END=======(Timer: %d sec)\n", (rDtd.duration-rDtd.PEC_delay));
    	                _GetDTDTime(i, 4);

						//CLI_Parser_Arg("nim.dtd z");
						//x_thread_delay(1000);
						u2SignalPER[i] = NIM_GetCableSignalPER();
						Printf("%s cable PER end: %d\n", _azDemodType[(rDtd.type1 + i*2) % 4], u2SignalPER[i]); //Need to reply by accumulated PER start function
						u2SignalSNR  = NIM_GetCableSignalSNR();
						Printf("%s cable SNR : %d\n", _azDemodType[(rDtd.type1 + i*2) % 4], u2SignalSNR);
						x_thread_delay(rDtd.pattern_delay*1000);

    	                _GetDTDTime(i, 5);
					}
#if 0
				else
#endif
					{

					    if( i2Sync == 0)
					    {
						    Printf("Dtd_PER:1/ Unsync: %s", _azDemodType[(rDtd.type1 + i*2) % 4]);
   						    _fgDtdUnsync = TRUE;
						     // break; // test
					    }
					}
			}

	// Should call this to notify DTD ATSC and QAM tests are both finish
    #if DTD_PARALLEL_TEST // 2009.2.23
        x_sema_unlock(_hDtdFinSema);
	#else
        _SltCmdQDtd();
	#endif
            /*
			if (!_fgDtdUnsync)
				{
					//Printf("Enter _SltCmdQDtd\n");
					_SltCmdQDtd();
				}
			*/
		}

       ////x_thread_exit();

}
#endif

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------


#if !DTD_PARALLEL_TEST // 2009.2.23
static void _DtdSeqProc(void* pvArg)
{
	SLT_DTD_T rDtd = *((SLT_DTD_T*)pvArg);

	static CHAR _azDemodType[3][8] = {"atsc", "calbe64", "cable256"};//EC_code_2007/6/22: Modulatio type: ATSC0x00, 64QAM0x01, 256QAM0x02
	UINT32 _azDemodFreq[3] = {rDtd.atsc_freq, 0, rDtd.qam_freq};
	int i, jj;
	//CHAR mode_type;
	//UINT8 u1SignalLevel;
	UINT16 u2SignalSNR;
	INT16 i2Sync;
	static UINT32 u4PatternCounter = 0;
	//UINT32 u4Retry = 10;

       i = u4PatternCounter;

       // Initialize
       u2SignalPER[i] = 0xFFFF;
       for (jj = 0; jj < 20 ; jj++)
	   {
	       u2PERRecord[i][jj] = 0xFFFF;
	   }

       for (jj = 0; jj < cDTD_TIME_ITEM; jj++)
       {
           _RstDTDTime(i, jj);
       }

       //u4DTDRetryRecord[i] = 10;

       _GetDTDTime(i, 0);

	   _fgDtdUnsync = FALSE; //EC_code_2007/6/25: For initial state

       CLI_Parser_Arg("nim.id 1");
	   //CLI_Parser_Arg("nim.go dvbt");   // default run DVBT
	   CLI_Parser_Arg("nim.c");            // free context
	   //for (i = 0; i < rDtd.num; i ++)
	   {
     		//u4Retry = 10;
     		CLI_Parser_Arg("nim.go %s", _azDemodType[(rDtd.type1) % 4]);
     		CLI_Parser_Arg("nim.freq %d", _azDemodFreq[(rDtd.type1) % 4]);
     		//CLI_Parser_Arg("nim.dtd f %d", _azDemodFreq[(rDtd.type1) % 4]);
     		if(rDtd.fgAtscFirst)// Should call this to notify MPEG SLT thread to start;
     	    {
           				//x_sema_unlock(_hDtdAtscSema); //by hotkey to continue main thread
     		}
     		else
     		{
     		       rDtd.fgAtscFirst = TRUE;
     		}
     		//x_sema_unlock(_hDtdAtscSema); // by hotkey to continue main thread

     		//x_thread_delay(1000);

            _GetDTDTime(i, 1);

     		_WaitSysTimeMs(DTD_ProcessTime[i][0], rDtd.delay*1000);

     		//Check Demod lock status
     		i2Sync= NIM_GetCableSync();
     		Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", _azDemodType[(rDtd.type1) % 4], (INT32)i2Sync, (INT32)_fgDtdUnsync);

		    u2SignalSNR  = NIM_GetCableSignalSNR();
		    Printf("%s cable SNR : %d\n", _azDemodType[i], u2SignalSNR);

            if (i2Sync != 0)
            {

		    //x_thread_delay((rDtd.delay-1)*1000);

   	        _GetDTDTime(i, 2);

		    Printf("duration is %d, delay is %d, and PEC_time is %d\n", rDtd.duration, rDtd.delay, rDtd.PEC_delay);
		    Printf("========PEC START=======\n");

		    //CLI_Parser_Arg("nim.dl 1");
		    Printf("nim.dtd y %d 1\n", rDtd.PEC_delay); //EC_code_2007/7/19: For RF playback with single channel
		    CLI_Parser_Arg("nim.dtd y %d 1", rDtd.PEC_delay);
		    #if 1
		    for (jj = 0; jj < rDtd.PEC_delay; jj++)
		    {
		        //x_thread_delay(1000);
                //HAL_GetTime(&rStartTime);
          		_WaitSysTimeMs(DTD_ProcessTime[i][2], 1000*(jj+1));
		        u2SignalPER[i] = NIM_GetCableSignalPER();
		        u2PERRecord[i][jj] = u2SignalPER[i];
		    }
		    #else
          	_WaitSysTimeMs(rStartTime, rDtd.PEC_delay*1000);
		    #endif

		    Printf("========PEC END=======\n");

   	        //_GetDTDTime(i, 3);

		    //CLI_Parser_Arg("nim.dtd z");
		    //x_thread_delay(1000);
		    //u2SignalPER[i] = NIM_GetCableSignalPER();
		    //Printf("%s cable PER end: %d\n", _azDemodType[(rDtd.type1) % 4], u2SignalPER[i]); //Need to reply by accumulated PER start function
		    //u2SignalSNR  = NIM_GetCableSignalSNR();
		    //Printf("%s cable SNR : %d\n", _azDemodType[(rDtd.type1) % 4], u2SignalSNR);
		    //x_thread_delay(rDtd.pattern_delay*1000);

            }
            else
			{
			    Printf("Dtd_PER:1/ Unsync: %s", _azDemodType[(rDtd.type1) % 4]);
  				_fgDtdUnsync = TRUE;
			}
       }

	_GetDTDTime(i, 3);

	// Should call this to notify DTD ATSC and QAM tests are both finish
	u4PatternCounter++;
    if (u4PatternCounter == rDtd.num)
    {
       _SltCmdQDtd();
       u4PatternCounter = 0;
    }

}
#endif

//-----------------------------------------------------------------------------
/** _GetDVBTTime
 *  @param  u1DemodNum
 *  @param  u1Element    Valid range:0~19
 *  @return void
 */
//-----------------------------------------------------------------------------
static void _GetDVBTTime(UINT8 u1DemodNum, UINT8 u1Element)
{
HAL_TIME_T rCurrent;

	HAL_GetTime(&rCurrent);

    DVBT_ProcessTime[u1DemodNum][u1Element] = rCurrent;
	Printf("Time(%d, %d): %ds:%dus\n", u1DemodNum, u1Element, DVBT_ProcessTime[u1DemodNum][u1Element].u4Seconds, DVBT_ProcessTime[u1DemodNum][u1Element].u4Micros);

}


static void _RstDVBTTime(UINT8 u1DemodNum, UINT8 u1Element)
{
    DVBT_ProcessTime[u1DemodNum][u1Element].u4Seconds = 0;
    DVBT_ProcessTime[u1DemodNum][u1Element].u4Micros = 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
#if DVBT_PARALLEL_TEST // 2009.2.26
static void _DvbtProc(void* pvArg)
{
	SLT_DTD_T rDvbt = *((SLT_DTD_T*)pvArg);

	////static CHAR _azDemodType[3][8] = {"atsc", "calbe64", "cable256"};//EC_code_2007/6/22: Modulatio type: ATSC0x00, 64QAM0x01, 256QAM0x02
	static CHAR _azDemodType[3][8] = {"6M", "7M", "8M"};
	UINT32 _azDemodFreq[3] = {rDvbt.atsc_freq, 0, rDvbt.qam_freq};
	int i;
	//CHAR mode_type;
	//UINT8 u1SignalLevel;
	UINT16 u2SignalSNR;
	INT16 i2Sync;

       //bApiVideoSetVideoSrc(0, SV_VS_DT1);

	_fgDvbtUnsync = FALSE; //EC_code_2007/6/25: For initial state
	{
			CLI_Parser_Arg("nim.id 1");
			CLI_Parser_Arg("nim.c");            // free context
		/*	// DTDPLL settings
			CLI_Parser_Arg("w 0x2000d180 0xF0030E60");
			CLI_Parser_Arg("w 0x2000d184 0x00000040");
			CLI_Parser_Arg("w 0x2000d184 0x00000041");
			// TVDPLL settings
			CLI_Parser_Arg("w 0x2000d170 0xF00312E0");
			CLI_Parser_Arg("w 0x2000d174 0x00000041");
			// SRAM switch to DVBT uP
			CLI_Parser_Arg("w 0x2002b4b4 0x000f0000");
                     CLI_Parser_Arg("w 0x2002b4a4 0x09c87800");
                     CLI_Parser_Arg("w 0x2002b0dc 0x00013407");   // 0xDD, b'4 = 1          */
			for (i = 0; i < rDvbt.num; i ++)
			{
				UINT32 u4Retry = 10;
				CLI_Parser_Arg("nim.go dvbt");
				CLI_Parser_Arg("nim.hc ar 1");   // force AR ON
				CLI_Parser_Arg("nim.sbw %d", rDvbt.type1+1);   // 1: 6M, 2: 7M, 3: 8M
				CLI_Parser_Arg("nim.freq %d", _azDemodFreq[(rDvbt.type1 + i) % 3]); // Set modulation type & channel frequency
				/*if(rDvbt.fgAtscFirst)// Should call this to notify MPEG SLT thread to start;
			       {
        				//x_sema_unlock(_hDtdAtscSema); //by hotkey to continue main thread
				}
				else
				{
				       rDvbt.fgAtscFirst = TRUE;
				}
				////x_sema_unlock(_hDtdAtscSema); // by hotkey to continue main thread*/

				x_thread_delay(1000);

				//Check Demod lock status
				while( (i2Sync= NIM_GetCableSync()) == 0 )
				{
				    if(u4Retry-- == 0)
				    {
				        break;
				    }
				    x_thread_delay(500);
				}

				////Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", _azDemodType[(rDtd.type1 + i*2) % 4], (INT32)i2Sync, (INT32)_fgDvbtUnsync);
				Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", _azDemodType[(rDvbt.type1 + i) % 3], (INT32)i2Sync, (INT32)_fgDvbtUnsync);
		#if 0
				if (i2Sync)
		#endif
					{
						x_thread_delay((rDvbt.delay-1)*1000);

						Printf("========PEC START=======(Timer: %d sec)\n", rDvbt.delay);
						Printf("duration is %d, delay is %d, and PEC_time is %d\n", rDvbt.duration, rDvbt.delay, (rDvbt.duration-rDvbt.delay-rDvbt.PEC_delay));

						u2SignalSNR  = NIM_GetCableSignalSNR();
						Printf("%s cable SNR : %d\n", _azDemodType[(rDvbt.type1 + i) % 3], u2SignalSNR);
						CLI_Parser_Arg("nim.dl 1");
						Printf("nim.hc monon (%d seconds)\n", (rDvbt.duration-rDvbt.delay-rDvbt.PEC_delay));
						CLI_Parser_Arg("nim.hc monon");
						x_thread_delay((rDvbt.duration-rDvbt.delay-rDvbt.PEC_delay)*1000);
						u2SignalSNR  = NIM_GetCableSignalSNR();
						Printf("========PEC END=======(Timer: %d sec)\n", (rDvbt.duration-rDvbt.PEC_delay));
						CLI_Parser_Arg("nim.hc monoff");
						x_thread_delay(1000);
						u2SignalPER_dvbt[i] = NIM_GetCableSignalPER();
						Printf("%s cable PER end: %d\n", _azDemodType[(rDvbt.type1 + i) % 3], u2SignalPER_dvbt[i]); //Need to reply by accumulated PER start function
						//u2SignalSNR  = NIM_GetCableSignalSNR();
						Printf("%s cable SNR : %d\n", _azDemodType[(rDvbt.type1 + i) % 3], u2SignalSNR);
						x_thread_delay(rDvbt.pattern_delay*1000);
					}
		#if 0
				else
		#endif
					{
						if (i2Sync == 0)
						{
						    Printf("Dvbt_PER:1/ Unsync: %s", _azDemodType[(rDvbt.type1 + i) % 3]);
						    _fgDvbtUnsync = TRUE;
						    //break;
						}
					}
			}

	// Should call this to notify DTD ATSC and QAM tests are both finish
	#if DVBT_PARALLEL_TEST
    	x_sema_unlock(_hDtdFinSema);
    #else
    	_SltCmdQDvbt();
    #endif
            /*
			if (!_fgDvbtUnsync)
				{
					//Printf("Enter _SltCmdQDtd\n");
					_SltCmdQDtd();
				}
			*/
		}

       ////x_thread_exit();

}
#endif

#if !DVBT_PARALLEL_TEST // 2009.2.26
static void _DvbtSeqProc(void* pvArg)
{
    SLT_DTD_T rDvbt = *((SLT_DTD_T*)pvArg);

    int i, jj;
    UINT16 u2SignalSNR;
    INT16 i2Sync;
    static UINT32 u4DvbtPatternCounter = 0;
    BOOL bfgDVBC=FALSE;

    i = u4DvbtPatternCounter;

    // Initialize
    u2SignalPER_dvbt[i] = 0xFFFF;
    for (jj = 0; jj < 20 ; jj++)
    {
        u2PERRecord_dvbt[i][jj] = 0xFFFF;
}

    for (jj = 0; jj < cDVBT_TIME_ITEM; jj++)
    {
        _RstDVBTTime(i, jj);
    }

    _GetDVBTTime(i, 0);

    _fgDvbtUnsync = FALSE; //EC_code_2007/6/25: For initial state

    if((UCHAR)(rDvbt.type1 & 0xD0) == 0xD0)
    {
        bfgDVBC = TRUE;
    }
    else
    {
        bfgDVBC = FALSE;
    }

    CLI_Parser_Arg("nim.id 1");
	//lei_code_dB_110512;
    //CLI_Parser_Arg("nim.c");            // free context

    //for (i = 0; i < rDvbt.num; i ++)
    {
        //UINT32 u4Retry = 10;
        if(bfgDVBC)
        {
            CLI_Parser_Arg("nim.go cable64");
            CLI_Parser_Arg("nim.sc %d 0 64", rDvbt.qam_freq); // Set modulation type & channel frequency
        }
        else
        {
            CLI_Parser_Arg("nim.go dvbt");
            CLI_Parser_Arg("nim.hc ar 1");   // force AR ON
            CLI_Parser_Arg("nim.sbw 3");   // 1: 6M, 2: 7M, 3: 8M. Force 8M
            CLI_Parser_Arg("nim.freq %d", rDvbt.atsc_freq); // Set modulation type & channel frequency
        }

        //x_thread_delay(1000);
        _GetDVBTTime(i, 1);
        _WaitSysTimeMs(DVBT_ProcessTime[i][0], rDvbt.delay*1000);

        //Check Demod lock status
        /*while( (i2Sync= NIM_GetCableSync()) == 0 )
        {
            if(u4Retry-- == 0)
            {
                break;
            }
            x_thread_delay(500);
        }*/

        //Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", _azDemodType[(rDvbt.type1 + i) % 3], (INT32)i2Sync, (INT32)_fgDvbtUnsync);

        i2Sync= NIM_GetCableSync();
        u2SignalSNR  = NIM_GetCableSignalSNR();
        if(bfgDVBC)
        {
            Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", rDvbt.qam_freq, (INT32)i2Sync, (INT32)_fgDvbtUnsync);
            Printf("DVB-C cable SNR : %d\n", u2SignalSNR);
        }
        else
        {
            Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", rDvbt.atsc_freq, (INT32)i2Sync, (INT32)_fgDvbtUnsync);
            Printf("DVB-T cable SNR : %d\n", u2SignalSNR);
        }

        if (i2Sync != 0)
        {
            _fgDvbtUnsync = FALSE;

            //x_thread_delay((rDvbt.delay-1)*1000);
            _GetDVBTTime(i, 2);

            //Printf("========PEC START=======(Timer: %d sec)\n", rDvbt.delay);
            //Printf("duration is %d, delay is %d, and PEC_time is %d\n", rDvbt.duration, rDvbt.delay, (rDvbt.duration-rDvbt.delay-rDvbt.PEC_delay));
            Printf("duration is %d, delay is %d, and PEC_time is %d\n", rDvbt.duration, rDvbt.delay, rDvbt.PEC_delay);
            Printf("========PEC START=======\n");

            /*u2SignalSNR  = NIM_GetCableSignalSNR();
                    Printf("%s cable SNR : %d\n", _azDemodType[(rDvbt.type1 + i) % 3], u2SignalSNR);
                    CLI_Parser_Arg("nim.dl 1");
                    Printf("nim.hc monon (%d seconds)\n", (rDvbt.duration-rDvbt.delay-rDvbt.PEC_delay));
                    CLI_Parser_Arg("nim.hc monon");
                    x_thread_delay((rDvbt.duration-rDvbt.delay-rDvbt.PEC_delay)*1000);
                    u2SignalSNR  = NIM_GetCableSignalSNR();
                    Printf("========PEC END=======(Timer: %d sec)\n", (rDvbt.duration-rDvbt.PEC_delay));
                    CLI_Parser_Arg("nim.hc monoff");*/

            CLI_Parser_Arg("nim.dl 1");
            Printf("nim.hc monon (%d seconds)\n", rDvbt.PEC_delay);
            CLI_Parser_Arg("nim.hc monon");

        #if 1
            for (jj = 0; jj < rDvbt.PEC_delay; jj++)
            {
                //x_thread_delay(1000);
                //HAL_GetTime(&rStartTime);
                _WaitSysTimeMs(DVBT_ProcessTime[i][2], 1000*(jj+1));
                u2SignalPER_dvbt[i] = NIM_GetCableSignalPER();
                u2PERRecord_dvbt[i][jj] = u2SignalPER_dvbt[i];
}
        #else
                _WaitSysTimeMs(rStartTime, rDvbt.PEC_delay*1000);
#endif

            CLI_Parser_Arg("nim.hc monoff");
            Printf("========PEC END=======\n");
        }
        else
        {
            if(bfgDVBC)
            {
                Printf("Dvbt_PER:1/ Unsync: %s", rDvbt.qam_freq);
            }
            else
            {
                Printf("Dvbt_PER:1/ Unsync: %s", rDvbt.atsc_freq);
            }
            _fgDvbtUnsync = TRUE;
            u4DvbtPatternCounter = 0; // If unsync, Early break.
        }
        /*
        x_thread_delay(1000);
        u2SignalPER_dvbt[i] = NIM_GetCableSignalPER();
        Printf("%s cable PER end: %d\n", _azDemodType[(rDvbt.type1 + i) % 3], u2SignalPER_dvbt[i]); //Need to reply by accumulated PER start function
        //u2SignalSNR  = NIM_GetCableSignalSNR();
        Printf("%s cable SNR : %d\n", _azDemodType[(rDvbt.type1 + i) % 3], u2SignalSNR);
        x_thread_delay(rDvbt.pattern_delay*1000);

        if (i2Sync == 0)
        {
            Printf("Dvbt_PER:1/ Unsync: %s", _azDemodType[(rDvbt.type1 + i) % 3]);
            _fgDvbtUnsync = TRUE;
            //break;
        }*/
    }

	//lei_code_dB110512;
    //CLI_Parser_Arg("nim.c"); //Hui add for free context
    _GetDVBTTime(i, 3);

    u4DvbtPatternCounter++;
    if (u4DvbtPatternCounter == rDvbt.num)
    {
       _SltCmdQDvbt();
       u4DvbtPatternCounter = 0;
    }
}
#endif

//-----------------------------------------------------------------------------
/** Run VSB/QAM SLT test
 *  @param    dtd_num             The number of patterns. Default: 2 (One for VSB and one for Q256)
 *  @param    dtd_type1           To decide if ATSC test is performed prior to QAM test. 0: VSB -> Q256 (Default)
 *  @param    dtd_type2           No use now
 *  @param    dtd_type3           No use now
 *  @param    dtd_delay           The delay to start PEC. (sec) ==> t1
 *  @param    dtd_duration        The duration of one test pattern (sec)
 *  @param    dtd_atsc_freq       ATSC freq (in KHz)
 *  @param    dtd_qam_freq        QAM freq (in KHz)
 *  @param    dtd_PEC_delay       PEC count time (sec) ==> t2
 *  @param    dtd_pattern_delay   No use now
 *  @param    dtd_selftest        No use now
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdDtd(INT32 i4Argc, const CHAR ** szArgv)
{

	INT32 dtd_num = StrToInt(szArgv[1]);
	INT32 dtd_type1 = StrToInt(szArgv[2]);
	//INT32 dtd_type2 = StrToInt(szArgv[3]);
	//INT32 dtd_type3 = StrToInt(szArgv[4]);
	INT32 dtd_delay = StrToInt(szArgv[5]);
	INT32 dtd_duration = StrToInt(szArgv[6]);
	INT32 dtd_atsc_freq = StrToInt(szArgv[7]);
	INT32 dtd_qam_freq = StrToInt(szArgv[8]);
	INT32 dtd_PEC_delay = StrToInt(szArgv[9]);
	INT32 dtd_pattern_delay = StrToInt(szArgv[10]);
	INT32 dtd_selftest = StrToInt(szArgv[11]);//EC_code_2007/6/26: For Self test


	//static CHAR _azDemodType[3][8] = {"atsc", "calbe64", "cable256"};//EC_code_2007/6/22: Modulatio type: ATSC0x00, 64QAM0x01, 256QAM0x02
	//UINT32 _azDemodFreq[3] = {dtd_atsc_freq, 0, dtd_qam_freq};

	//int i;
	//CHAR mode_type;
	//UINT8 u1SignalLevel;
	//UINT16 u2SignalSNR;
	//INT16 i2Sync;
	_fgDtdUnsync = FALSE; //EC_code_2007/6/25: For initial state


SLT_DTD_T rDtd;
      //INT32 i4Ret;

    // time print for debug
	CLI_Parser("slt.t");

	rDtd.num = dtd_num;
	rDtd.type1 = dtd_type1;
	rDtd.delay = dtd_delay;
	rDtd.duration = dtd_duration;
	rDtd.atsc_freq = dtd_atsc_freq;
	rDtd.qam_freq = dtd_qam_freq;
	rDtd.PEC_delay = dtd_PEC_delay;
	rDtd.pattern_delay = dtd_pattern_delay;
	rDtd.selftest = dtd_selftest;

    // To decide if ATSC test is performed prior to QAM test according to UDP command
//    BOOL fgAtscFirst;
    if (dtd_type1==0)
    		rDtd.fgAtscFirst = TRUE;
    else
    		rDtd.fgAtscFirst = FALSE;

#if DTD_PARALLEL_TEST

    // Create semaphore for notify MPEG SLT to start and
    //if(_hDtdAtscSema == NULL || _hDtdFinSema == NULL)
    if(_hDtdFinSema == NULL)
    {
        /*if(x_sema_create(&_hDtdAtscSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            return -1;
        }*/
        if(x_sema_create(&_hDtdFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            return -1;
        }
    }
    else
    {
        //x_sema_lock(_hDtdAtscSema, X_SEMA_OPTION_WAIT);
        x_sema_lock(_hDtdFinSema, X_SEMA_OPTION_WAIT);
    }

    // Create thread where demod test really performs
   // if(x_thread_create(&_hDtdThread, "SLT_DTD", 2048, 200, _DtdProc, sizeof(BOOL), (void*)fgAtscFirst) == OSR_OK)
//   if (x_thread_create(&_hDtdThread, "SLT_DTD", 4096, 200, _DtdProc, sizeof(rDtd), (void*)&rDtd) == OSR_OK)
   if (x_thread_create(&_hDtdThread, "SLT_DTD", 4096, 99, _DtdProc, sizeof(rDtd), (void*)&rDtd) == OSR_OK)
    {
       Printf("DTDOK\n");
       return 0;
    }
    else
    {
        Printf("DTDFAIL\n");
        return 0;
    }
/*
    if(x_sema_lock_timeout(_hDtdAtscSema, DTD_SLT_TIMEOUT) == OSR_OK)
    {
        Printf("[SLTDTD] lock ok\n");
        Printf("DTDOK\n");
        return 0;
    }
    else
    {
        Printf("DTDFAIL\n");
        return 0;
    }*/

#else
    // time print for debug
	CLI_Parser("slt.t");
    _DtdSeqProc((void*)&rDtd);
	return 0;
#endif
}

//-----------------------------------------------------------------------------
/** Run DVBT SLT test
 *  @param    dvbt_num             The number of patterns. Default: 1
 *  @param    dvbt_type1           To decide which pattern to play (0x82: 8M)
 *  @param    dvbt_type2           No use now
 *  @param    dvbt_type3           No use now
 *  @param    dvbt_delay           The delay to start PEC. (sec) ==> t1
 *  @param    dvbt_duration        The duration of one test pattern (sec)
 *  @param    dvbt_atsc_freq       freq (in KHz)
 *  @param    dvbt_qam_freq        QAM freq (in KHz), not use in DVBT
 *  @param    dvbt_PEC_delay       PEC count time (sec) ==> t2
 *  @param    dvbt_pattern_delay   No use now
 *  @param    dvbt_selftest        No use now
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdDvbt(INT32 i4Argc, const CHAR ** szArgv)
{

    INT32 dvbt_num = StrToInt(szArgv[1]);
    INT32 dvbt_type1 = StrToInt(szArgv[2]);
    //INT32 dvbt_type2 = StrToInt(szArgv[3]);
    //INT32 dvbt_type3 = StrToInt(szArgv[4]);
    INT32 dvbt_delay = StrToInt(szArgv[5]);
    INT32 dvbt_duration = StrToInt(szArgv[6]);
    INT32 dvbt_freq1 = StrToInt(szArgv[7]);
    INT32 dvbt_freq2 = StrToInt(szArgv[8]);
    INT32 dvbt_PEC_delay = StrToInt(szArgv[9]);
    INT32 dvbt_pattern_delay = StrToInt(szArgv[10]);
    INT32 dvbt_selftest = StrToInt(szArgv[11]);//EC_code_2007/6/26: For Self test

    //static CHAR _azDemodType[3][8] = {"dvbt", "dvbt", "dvbt"};
    //UINT32 _azDemodFreq[3] = {dvbt_freq1, 666000, dvbt_freq2};

    //int i;
    //CHAR mode_type;
    //UINT8 u1SignalLevel;
    //UINT16 u2SignalSNR;
    //INT16 i2Sync;
    _fgDvbtUnsync = FALSE; //EC_code_2007/6/25: For initial state

    SLT_DTD_T rDvbt;
    rDvbt.num = dvbt_num;
    dvbt_type1 =  dvbt_type1 & 0x000000f0;   // 0xDx:DVBC, 0x8x: DVBT-8M
    rDvbt.type1 = dvbt_type1;
    rDvbt.delay = dvbt_delay;
    rDvbt.duration = dvbt_duration;
    rDvbt.atsc_freq = dvbt_freq1;
    rDvbt.qam_freq = dvbt_freq2;
    rDvbt.PEC_delay = dvbt_PEC_delay;
    rDvbt.pattern_delay = dvbt_pattern_delay;
    rDvbt.selftest = dvbt_selftest;

#if DVBT_PARALLEL_TEST

    // Create semaphore for notify MPEG SLT to start and

    ////if(_hDtdAtscSema == NULL || _hDtdFinSema == NULL)
    if (_hDvbtFinSema == NULL)
    {
        /*if(x_sema_create(&_hDtdAtscSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            return -1;
        }*/
        if(x_sema_create(&_hDvbtFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            return -1;
        }
    }
    else
    {
        ////x_sema_lock(_hDtdAtscSema, X_SEMA_OPTION_WAIT);
        x_sema_lock(_hDvbtFinSema, X_SEMA_OPTION_WAIT);
    }

    // Create thread where demod test really performs
   // if(x_thread_create(&_hDtdThread, "SLT_DTD", 2048, 200, _DtdProc, sizeof(BOOL), (void*)fgAtscFirst) == OSR_OK)
   if (x_thread_create(&_hDvbtThread, "SLT_DVBT", 4096, 200, _DvbtProc, sizeof(rDvbt), (void*)&rDvbt) == OSR_OK)
    {
       Printf("DVBTOK\n");
	return 0;
}
    else
    {
        Printf("DVBTFAIL\n");
        return 0;
    }
/*
    if(x_sema_lock_timeout(_hDtdAtscSema, DTD_SLT_TIMEOUT) == OSR_OK)
    {
        Printf("[SLTDTD] lock ok\n");
        Printf("DTDOK\n");
        return 0;
    }
    else
    {
        Printf("DTDFAIL\n");
        return 0;
    }*/


#else
    _DvbtSeqProc((void*)&rDvbt);

	return 0;
#endif
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdAtd(INT32 i4Argc, const CHAR ** szArgv)
{

    INT32 atd_freq = (i4Argc)>1?StrToInt(szArgv[1]):0;
	INT32 bypass_audio = (i4Argc)>2?StrToInt(szArgv[2]):0;
    INT32 delay = (i4Argc)>3?StrToInt(szArgv[3]):100;
    INT32 atd_system = (i4Argc)>4?StrToInt(szArgv[4]):0;   // 3: SECAM, 5: NTSC

    //CLI_Parser("nav.play m atv");
    CLI_Parser("n.ms atd1");
    //CLI_Parser("nim.go dvbt");
    CLI_Parser("nim.id 1");
    CLI_Parser("nim.dtd mp");
    CLI_Parser("nim.dtd c 0");
    CLI_Parser("nim.id 0");
    CLI_Parser_Arg("nim.atd s %d",atd_system);
    CLI_Parser_Arg("nim.atd f %d",atd_freq);
    //CLI_Parser("n.ms atd1");

    if (delay < 1000)
    	delay = 1000;
    x_thread_delay(delay);


    if(atd_system == 5)
    {
   	    if (bypass_audio || SLT_AtdSorting() == 0)
	    {
		    CLI_Parser("nim.atd x");
	        //Printf("VAGood");
	    }
	    else
	    {
    	 	Printf("ATD SLT audio fail\n");
    	 	Printf("ATDFAIL");
        }
    }
    else if(atd_system == 3)
    {
	    if (bypass_audio || SLT_AtdEuSorting() == 0)
	    {
		    CLI_Parser("nim.atd x");
	        //Printf("VAGood");
    	}
       else
       {
    	 	Printf("ATD EU SLT audio fail\n");
    	 	Printf("ATDFAIL");
       }
	}

    //CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------


static INT32 _SltCmdQDtd(void)
{
int ii, jj;
HAL_TIME_T rDelta;

    #if DTD_PARALLEL_TEST // 2009.2.23
    if(x_sema_lock_timeout(_hDtdFinSema, DTD_SLT_TIMEOUT) == OSR_OK)
    {
    #endif
        if ((_fgDtdUnsync == FALSE) && (u2SignalPER[0]<20) &&(u2SignalPER[1]<20))
        	Printf("Dtd_PER:0");
        else
          {
     	    Printf("Dtd_PER:1/ PEC > THR, PER[0]=%d, PER[1]=%d ", u2SignalPER[0], u2SignalPER[1]);
        	Printf("_fgDtdUnsync=%d", _fgDtdUnsync);
         }

        // Show PER
        for (ii = 0; ii < 2; ii++)
        {
            Printf("\n%d:", ii);
            for (jj = 0; jj < 20; jj++)
                 Printf("%d ", u2PERRecord[ii][jj]);
        }

        // Show system time
        for (ii = 0; ii < 2; ii++)
        {
            Printf("\nTime%d:", ii);
            for (jj = 0; jj < cDTD_TIME_ITEM; jj++)
                Printf("%d: %ds:%6dus\n", jj, DTD_ProcessTime[ii][jj].u4Seconds, DTD_ProcessTime[ii][jj].u4Micros);

            HAL_GetDeltaTime(&rDelta, &(DTD_ProcessTime[ii][0]), &(DTD_ProcessTime[ii][cDTD_TIME_ITEM-1]));
            Printf("DTDProc time: %ds:%6dus\n", rDelta.u4Seconds, rDelta.u4Micros);
        }

        // Show retry count
        #if 0
        for (ii = 0; ii < 2; ii++)
        {
            Printf("Retry(%d)=%d ", ii, u4DTDRetryRecord[ii]);
        }
        #endif
        Printf("\n");

    #if DTD_PARALLEL_TEST // 2009.2.23
    }
    else // Some error has occurred
    {
        Printf("Dtd_PER:1/ Some Error in Sema");
    }
    x_sema_unlock(_hDtdFinSema);
    #endif

	#if !DTD_PARALLEL_TEST
    x_sema_unlock(_hDtdFinSema); // by hotkey to prevent dtv test starting while demod test thread is still running
	#endif
    return 0;

}



//-----------------------------------------------------------------------------
/** Brief  	query if dvbt test is finished
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdQDvbt(void)
{
#if DVBT_PARALLEL_TEST
    if(x_sema_lock_timeout(_hDvbtFinSema, DTD_SLT_TIMEOUT) == OSR_OK)
    {
#endif
        if ((_fgDvbtUnsync == FALSE) && (u2SignalPER_dvbt[0]<20) && (u2SignalPER_dvbt[1]<20))
//        if (_fgDvbtUnsync == FALSE)
        	Printf("Dvbt_PER:0");
        else
            Printf("Dvbt_PER:1/ PEC > THR PER[0]=%d, PER[1]=%d ", u2SignalPER_dvbt[0], u2SignalPER_dvbt[1]);
#if DVBT_PARALLEL_TEST
    }
    else // Some error has occurred
    {
        Printf("Dvbt_PER:1/ Some Error in Sema");
    }
    x_sema_unlock(_hDvbtFinSema);
#endif

#if !DVBT_PARALLEL_TEST
    x_sema_unlock(_hDvbtFinSema); // by hotkey to prevent dtv test starting while demod test thread is still running
#endif

    return 0;
}

// query if dtd test is finished
static INT32 _SltCmdQDtdLock(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Timeout = (i4Argc < 2) ? (0) : (StrToInt(szArgv[1]));

	if(u4Timeout == 0)
	{
		if(x_sema_lock(_hDtdFinSema, X_SEMA_OPTION_WAIT) == OSR_OK)
		{
			Printf("QLOCKOK\n");
		}
		else
		{
			Printf("QLOCKFAIL\n");
		}
	}
	else
	{
		if(x_sema_lock_timeout(_hDtdFinSema, u4Timeout) == OSR_OK)
		{
			Printf("QLOCKOK\n");
		}
		else
		{
			Printf("QLOCKFAIL\n");
		}
	}

	return 0;

}

static INT32 _SltCmdQDvbtLock(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Timeout = (i4Argc < 2) ? (0) : (StrToInt(szArgv[1]));

	if(u4Timeout == 0)
	{
		if(x_sema_lock(_hDvbtFinSema, X_SEMA_OPTION_WAIT) == OSR_OK)
		{
			Printf("QLOCKOK\n");
		}
		else
		{
			Printf("QLOCKFAIL\n");
		}
	}
	else
	{
		if(x_sema_lock_timeout(_hDvbtFinSema, u4Timeout) == OSR_OK)
		{
			Printf("QLOCKOK\n");
		}
		else
		{
			Printf("QLOCKFAIL\n");
		}
	}

	return 0;

}

// test usb
static INT32 _SltCmdUsb(INT32 i4Argc, const CHAR ** szArgv)
{
    if(MUSB_CheckSLT() == 0)
    {
        Printf("OK");
    }
    else
    {
        Printf("Fail");
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** Print "OK" to CLI
 *  @param argument count
 *  @param array of arguments represented is char string
 *  @return useless return value
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdOK(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    Printf("OK\n");
    return 0;
}

// echo the input string
static INT32 _SltCmdEcho(INT32 i4Argc, const CHAR ** szArgv)
{
	if(i4Argc < 2)
	{
		return -1024;
	}

    Printf("%s\n", szArgv[1]);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdBypassMLC(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc < 2)
    {
        return -1024;
    }

    _fgBypassMLC = StrToInt(szArgv[1]);
    //vDrvMainOnOff(!_fgBypassMLC);
    vDrvSCEAllOnOff(!_fgBypassMLC);
    //vDrvMain422(!_fgBypassMLC);
    //vDrvPipOnOff(!_fgBypassMLC);
    //vDrvPip422(!_fgBypassMLC);
    return 0;

}

//-----------------------------------------------------------------------------
/** Brief   	delay execution for some time (unit is milliseconds)
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdDelay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Delay;
    if(i4Argc < 2)
    {
        return -1024;
    }
    u4Delay = StrToInt(szArgv[1]);

    x_thread_delay(u4Delay);

    return 0;

}
//extern void vVdoModeChgDone(UINT8 bPath); // by hotkey 0729 for compile
static INT32 _SltCmdModeChgDone(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc < 2)
    {
        return -1024;
    }
    //vVdoModeChgDone(StrToInt(szArgv[1])); // by hotkey 0729 for compile

    return 0;
}

#if 0
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdLvds(INT32 i4Argc, const CHAR ** szArgv)
{
    static BOOL fgIsRetry = FALSE;
    INT32 lvds10bit = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 pattern = (i4Argc)>2?StrToInt(szArgv[2]):1;
    INT32 delay;
    INT32 ack = (i4Argc)>4?StrToInt(szArgv[4]):1; ;

    if(pattern)
    {
        if(fgIsRetry)
        {
            delay = (i4Argc)>3?StrToInt(szArgv[3]):1000;
            CLI_Parser("pmx.s.p 52");
            CLI_Parser("pmx.p 1");
        }
        else
        {
            CLI_Parser("pmx.p 1");
            delay = 100;
            fgIsRetry = TRUE;
        }
    }

// by hotkey for compile
    if(lvds10bit == 1) // Normal 10-bit
    {

            // BGA with 10-bit LVDS (change pin mux)
        vRegWriteFldAlign(SCPOS_OS_12, 1, SCPOS_10B_EN);
        vRegWriteFldAlign(SCPOS_OS_05, 0, SCPOS_REG_8BIT_DUAL);
        vRegWriteFldAlign(SCPOS_OS_05, 0, SCPOS_REG_BGA_8BIT_DUAL);
        VDOIN_WRITE32(SCPOS_OS_27, 0x0123ff56);
        VDOIN_WRITE32(SCPOS_OS_26, 0xb8a7);
    }
    else if(lvds10bit == 2) // Switched 10-bit
    {
            // BGA with 10-bit LVDS (change pin mux)
        vRegWriteFldAlign(SCPOS_OS_12, 1, SCPOS_10B_EN);
        vRegWriteFldAlign(SCPOS_OS_05, 0, SCPOS_REG_8BIT_DUAL);
        vRegWriteFldAlign(SCPOS_OS_05, 0, SCPOS_REG_BGA_8BIT_DUAL);
        VDOIN_WRITE32(SCPOS_OS_27, 0x0124ff56);
        VDOIN_WRITE32(SCPOS_OS_26, 0xb9a7);
    }
    else
    {
                    // BGA with 8-bit LVDS (change pin mux)
        vRegWriteFldAlign(SCPOS_OS_12, 0, SCPOS_10B_EN);
        vRegWriteFldAlign(SCPOS_OS_05, 0, SCPOS_REG_8BIT_DUAL);
        vRegWriteFldAlign(SCPOS_OS_05, 1, SCPOS_REG_BGA_8BIT_DUAL);
        VDOIN_WRITE32(SCPOS_OS_27, 0xffffffff);
        VDOIN_WRITE32(SCPOS_OS_26, 0xffff);
    }
    if(ack)
    {
        _u4Mode = _MODE_INIT;

        ACK;
    }
}

static INT32 _SltCmdHdmiColorTransform(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 colorTransform;

    if(i4Argc < 2)
    {
        return -1024;
    }
    colorTransform = (i4Argc)>1?StrToInt(szArgv[1]):0;
    _u1SltHdmiColorTransform = colorTransform;
    return 0;
}
#endif

// an alias for b.reboot command
static INT32 _SltCmdReboot(INT32 i4Argc, const CHAR ** szArgv)
{
    //magic # for doing mbist when looping
    IO_WRITE32(IO_VIRT, 0x28140, 0xaaaaff00);

    CLI_Parser("b.reboot");
	return 0;
}

// time command will show difference between last call to this command and current time
static INT32 _SltCmdTime(INT32 i4Argc, const CHAR ** szArgv)
{
	static HAL_TIME_T rLast;
	static BOOL bFirst = TRUE;
	HAL_TIME_T rCurrent, rDiff;

	if(!bFirst)
	{
		bFirst = FALSE;
		HAL_GetTime(&rLast);
	}
	HAL_GetTime(&rCurrent);
	HAL_GetDeltaTime(&rDiff, &rCurrent, &rLast);
	rLast = rCurrent;
	Printf("Time: %ds:%dus\n", rDiff.u4Seconds, rDiff.u4Micros);

	return 0;
}

static INT32 _SltCmdFreezePts(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Pts = (i4Argc)>1?StrToInt(szArgv[1]):0;

    if(i4Argc == 1)
    {
        Printf("Enter PTS to freeze\n");
        return -1;
    }

    fgSltWatchPts = TRUE; // enable watch
    u4SltPts = i4Pts;
    return 0;
}

static INT32 _SltCmdUnFreezePts(INT32 i4Argc, const CHAR ** szArgv)
{
    fgSltWatchPts = FALSE;
    fgSltNotSendPic = FALSE;
    return 0;
}

EXTERN INT32 PDWNC_T8032Cmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse);
// uP test command
static INT32 _SltCmdT8032(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 ret, i4Retry = 5;
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_QUERY_SW_VERSION; //PDWNC_CMD_ARMT8032_ALIVE_CHECK;
    rCmd.u1SubCmd = 0;
    //ret=PDWNC_T8032Cmd(&rCmd, NULL);

    while((ret=PDWNC_T8032Cmd(&rCmd, NULL)) != 0)
    {
        if(i4Retry-- == 0)
            break;
    }

    if(ret==0)
    {
       Printf("T8032 OK!\n");
    }
    else if(ret==-1)
    {
       Printf("T8032FAIL: ACK mismach!\n");
    }
    else if(ret==-2)
    {
       Printf("T8032FAIL: Time out!\n");
    }
    else
    {
       Printf("T8032FAIL: strange!!\n");
    }
    return ret;
}


static INT32 _SltCmdLvdsPortAssign(INT32 i4Argc, const CHAR **szArgv)
{
#ifdef CC_MT5396
    	INT32 u4LvdsCtrlAEven = (i4Argc) > 1 ? StrToInt(szArgv[1]) : 0;
		INT32 u4LvdsCtrlAOdd = (i4Argc) > 2 ? StrToInt(szArgv[2]) : 1;
		INT32 u4LvdsCtrlBEven = (i4Argc) > 3 ? StrToInt(szArgv[3]) : 2;
		INT32 u4LvdsCtrlBOdd = (i4Argc) > 4 ? StrToInt(szArgv[4]) : 3;

		Printf("LVDS Port Assign A=%d, B=%d, C=%d, D=%d\n", u4LvdsCtrlAEven, u4LvdsCtrlAOdd, u4LvdsCtrlBEven, u4LvdsCtrlBOdd);

		vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAEven, RG_A_SW);
		vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBEven, RG_B_SW);
		vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlAOdd,  RG_C_SW);
		vIO32WriteFldAlign(MLVDS_SO2, u4LvdsCtrlBOdd,  RG_D_SW);

		Printf("SLTOK\n\n");
#endif

	return 0;
}


static INT32 _SltCmdEthTest(INT32 i4Argc, const CHAR **szArgv)
{
   INT32 i4Ret;
   UNUSED(i4Argc);
   UNUSED(szArgv);

   i4Ret = CLI_Parser("net.eth.slt");

   return i4Ret;
}


static INT32 _SltCmdFreezeField(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Field = (i4Argc)>1?StrToInt(szArgv[1]):0;
    UINT32 u4Path = (i4Argc)>2?StrToInt(szArgv[2]):0;

    CLI_Parser_Arg("vdp.f %d 1", u4Path);
    CLI_Parser_Arg("vdp.f %d 0", u4Path);
    x_thread_delay(200);
    SLT_FreezeHdmiFrame(u4Path, u4Field);
    SLT_WaitFreezeDone();

    return 0;
}

static CHAR _auCmd[2][256]; // maximum 2 worker threads at the same time

static void _WorkerThread(void* pvArg)
{
	INT32 i4Ret;
	Printf("\nSLT worker thread [%d] start to execute...\n", *((UINT32*)pvArg));
	i4Ret = CLI_Parser(_auCmd[*((UINT32*)pvArg)]);
	Printf("\n... SLT worker thread [%d] finished, return value (%d)\n", *((UINT32*)pvArg), i4Ret);
}

// This function forks a thread to do the CLI command specified
// without locking the COM port
static INT32 _SltCmdForkThread(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Thread;
    INT32 i;


    if(i4Argc < 3)
    {
        Printf("Usage: slt.fork {u4Thread} {command}\n");
        return 0;
    }

    u4Thread = StrToInt(szArgv[1]);

	// time print for debug
	if(u4Thread)
		CLI_Parser("slt.t");

	if(u4Thread > 1)
	{
        Printf("Usage: slt.fork {u4Thread} {command}\n");
		Printf("u4Thread = 0~1\n");
        return 0;
	}

	_auCmd[u4Thread][0] = '\0'; // reset the string for strcat

    // save parameters to global data structure for worker thread
    for(i = 2; i < i4Argc; i++)
    {
    	x_strcat(_auCmd[u4Thread], szArgv[i]);
		x_strcat(_auCmd[u4Thread], " "); // add space between arguments
    }

    // create new thread for embedded CLI command

 	if (x_thread_create((u4Thread)? &_hWorkerThread1: &_hWorkerThread0,
						(u4Thread)? "SLT_WORKER_1" : "SLT_WORKER_0" ,
						8192, 200, _WorkerThread, sizeof(UINT32), &u4Thread) != OSR_OK)
	{
		Printf("Fail to create SLT worker thread [%d]. It's possibly running.\n", u4Thread);
		return -1;
	}

    return 0;
}

static INT32 _SltCmdMMSLT(INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc < 2)
    {
        Printf("please give media type. (rm/mp4/vc1/avs/vp6/vp8)\n");
        return -1;
    }
    //CLI_Parser("n.di.scrc");
    //CLI_Parser("playmgr.sltstop");

    if(x_strcmp("rm", szArgv[1]) == 0)
    {
        CLI_Parser("playmgr.sltstop");
        CLI_Parser("playmgr.sltplay /mnt/usb_0/slt.rm 4 5");
    }
    else if (x_strcmp("mp4", szArgv[1]) == 0)
    {
        CLI_Parser("playmgr.sltstop");
        CLI_Parser("playmgr.sltplay /mnt/usb_0/b05.avi 4 1");
    }
    else if (x_strcmp("vc1", szArgv[1]) == 0)
    {
        CLI_Parser("playmgr.sltstop");
        CLI_Parser("playmgr.sltplay /mnt/usb_0/slt.wmv 4 2");
    }
    else if (x_strcmp("avs", szArgv[1]) == 0)
    {
        CLI_Parser("playmgr.sltstop");
        CLI_Parser("playmgr.sltplay /mnt/usb_0/slt.avs 2 6");
    }
    else if (x_strcmp("vp6", szArgv[1]) == 0)
    {
        CLI_Parser("playmgr.sltstop");
        CLI_Parser("playmgr.sltplay /mnt/usb_0/slt.flv 4 6");
    }
    else if (x_strcmp("vp8", szArgv[1]) == 0)
    {
        CLI_Parser("playmgr.sltstop");
        CLI_Parser("playmgr.sltplay /mnt/usb_0/slt.webm 4 4");
    }
    else
    {
        Printf("unknown media type\n");
    }

	return 0;
}

#if 0
static INT32 _SltCmdDtvCrcCheck(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 pts, u4B2rDramCrc, u4B2rPixelCrc, u4PscanCrc;
	if(i4Argc < 5)
	{
		Printf("usage: pts, u4B2rDramCrc, u4B2rPixelCrc, u4PscanCrc\n");
		return -1;
	}
    pts = (UINT32)StrToInt(szArgv[1]);
	u4B2rDramCrc = (UINT32)StrToInt(szArgv[2]);
	u4B2rPixelCrc = (UINT32)StrToInt(szArgv[3]);
	u4PscanCrc = (UINT32)StrToInt(szArgv[4]);

	CLI_Parser("n.di.scrc");
	CLI_Parser_Args("vdp.s.bpts %d", pts); // must set CRC ready flag to false

	// check CRC ready
	while(!CrcReady)
		x_thread_delay(100);

	if(_rVdpB2rCrc.u4DramChkSum == u4B2rDramCrc
	   && _rVdpB2rCrc.u4PixelChkSum == u4B2rPixelCrc
	   && _rVdpB2rCrc.u4PSCANCrc == u4PscanCrc )
	{
		Printf("DTVCRCOK\n");
	}
	else
	{
		Printf("DTVCRCFAIL:PTS:%d, Bd:%d, Bp:%d, Pc:%d\n", u4B2rDramCrc, u4B2rPixelCrc, u4PscanCrc);
	}
	return 0;
}
#else
static INT32 _SltCmdDtvCrcCheck(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Channel, u4VdoPid, u4Pts, u4VdecVdoType, u4DmxVdoType;
    CHAR strCliCmd[32];

    if(i4Argc < 5)
    {
        Printf("%s [channel] [videopid] [videotype (avs, mpeg2, h264)] [pts]\n", szArgv[0]);
        return -1;
    }
    u4Channel = (UINT32)StrToInt(szArgv[1]);
    u4VdoPid = (UINT32)StrToInt(szArgv[2]);
    u4Pts = (UINT32)StrToInt(szArgv[4]);
    if(x_strcmp("avs",szArgv[3]) == 0)
    {
        u4DmxVdoType = 9;
        u4VdecVdoType = 7;
    }
    else if(x_strcmp("mpeg2", szArgv[3]) == 0)
    {
        u4DmxVdoType = 0;
        u4VdecVdoType = 0;
    }
    else if(x_strcmp("h264", szArgv[3]) == 0)
    {
        u4DmxVdoType = 1;
        u4VdecVdoType = 2;
    }

    x_sprintf(strCliCmd, "nim.channel %d", u4Channel);
    CLI_Parser(strCliCmd);

    CLI_Parser("n.ms dtv1");
    CLI_Parser("vdp.e 0");
    CLI_Parser("vdp.s.i 0 0 0");
    CLI_Parser("vdec.s 0");
    CLI_Parser("d.i");
    CLI_Parser("d.st 0");
    CLI_Parser("d.std on off");
    x_sprintf(strCliCmd, "d.svt %d", u4DmxVdoType);
    CLI_Parser(strCliCmd);
    x_sprintf(strCliCmd, "d.sp 0 %d video", u4VdoPid);
    CLI_Parser(strCliCmd);
    x_sprintf(strCliCmd, "vdec.p 0 %d", u4VdecVdoType);
    CLI_Parser(strCliCmd);
    x_sprintf(strCliCmd, "vdp.s.bpts %d", u4Pts);
    CLI_Parser(strCliCmd);
    return 0;
}
#endif

#define WRITE32MSK(base, offset, value, msk)	\
        IO_WRITE32((base), (offset), \
                   (IO_READ32((base), (offset)) & ~(msk)) | ((value) & (msk)))


// TRUE: MBIST pass
// FALSE: MBIST fail
static BOOL _fgMBIST(BOOL logOn)
{

	BOOL fgResult = TRUE;
	UINT32 u4MBISTStatus;
	CRIT_STATE_T cs;
	cs = x_crit_start();
#if 1
    // arm rom check sum
    IO_WRITE32(IO_VIRT, 0x08138, 0x12fdd9b1);
    IO_WRITE32(IO_VIRT, 0x0813c, 0x00000497);
    IO_WRITE32(IO_VIRT, 0x08140, 0x1c05da0f);
    IO_WRITE32(IO_VIRT, 0x08144, 0x00000495);

    // post proc
    // rom check sum
    IO_WRITE32(IO_VIRT, 0x27580, 0x004d8913);
    IO_WRITE32(IO_VIRT, 0x27584, 0x00000000);
    IO_WRITE32(IO_VIRT, 0x27588, 0x004d9009);
    IO_WRITE32(IO_VIRT, 0x2758c, 0x00000000);
    IO_WRITE32(IO_VIRT, 0x27590, 0x0307e6a2);
    IO_WRITE32(IO_VIRT, 0x27594, 0x00000000);

    // audio
    // rom check sum and config
    IO_WRITE32(IO_VIRT, 0x052e4, 0xaa0ab04a);
    IO_WRITE32(IO_VIRT, 0x052e8, 0x12000005);
    IO_WRITE32(IO_VIRT, 0x052ec, 0x642b6f0c);
    IO_WRITE32(IO_VIRT, 0x052f0, 0x12000004);
    IO_WRITE32(IO_VIRT, 0x052f4, 0x83b9ac27);
    IO_WRITE32(IO_VIRT, 0x052f8, 0x120001bb);
    IO_WRITE32(IO_VIRT, 0x056e4, 0xd8307122);
    IO_WRITE32(IO_VIRT, 0x056e8, 0x12000005);
    IO_WRITE32(IO_VIRT, 0x056f4, 0x2658f92e);
    IO_WRITE32(IO_VIRT, 0x056f8, 0x120001c3);
#else // verify this works
    // arm rom check sum
    IO_WRITE32(IO_VIRT, 0x08138, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x0813c, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x08140, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x08144, 0xffffffff);

    // post proc
    // rom check sum
    IO_WRITE32(IO_VIRT, 0x27580, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x27584, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x27588, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x2758c, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x27590, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x27594, 0xffffffff);

    // audio
    // rom check sum and config
    IO_WRITE32(IO_VIRT, 0x052e4, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x052e8, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x052ec, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x052f0, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x052f4, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x052f8, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x056e4, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x056e8, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x056f4, 0xffffffff);
    IO_WRITE32(IO_VIRT, 0x056f8, 0xffffffff);
#endif

    // clear status register
    IO_WRITE32(IO_VIRT, 0x0e030, 0x00000000);
    IO_WRITE32(IO_VIRT, 0x0e038, 0x00000000);
    IO_WRITE32(IO_VIRT, 0x0e03c, 0x00000000);

    //IO_WRITE32(IO_VIRT, 0x0d234, 0x00000001);  //dsp_ck
    //IO_WRITE32(IO_VIRT, 0x0d208, 0x00000003);  // cpu_clk
    IO_WRITE32(IO_VIRT, 0x0d5e0, 0x0000fff8);   // mbist clock ctrl

    IO_WRITE32(IO_VIRT, 0x0e004, 0x000ff800);       // [19:11] post_proc rom
    IO_WRITE32(IO_VIRT, 0x0e008, 0x00660000);      // [18:17] promA and dromA
    // [22:21] promB and dromB
    IO_WRITE32(IO_VIRT, 0x0e00c, 0x00000c00);      // [74:74] arm rom


    // start mbist
    IO_WRITE32(IO_VIRT, 0x0e000, 0x00000003);      // enable rom mbist

	//HAL_Delay_us(20000); // delay 20ms and then check result

    // check mbist ready
    while((IO_READ32MSK(IO_VIRT, 0x0e054, 0x000ff800) != 0x000ff800)
            || (IO_READ32MSK(IO_VIRT, 0x0e058, 0x00660000) != 0x00660000)
            || (IO_READ32MSK(IO_VIRT, 0x0e05c, 0x00000c00) != 0x00000c00));


	x_crit_end(cs);

    u4MBISTStatus = IO_READ32MSK(IO_VIRT, 0x0e030, 0x00000ff8);
    if(u4MBISTStatus)
    {
        if(logOn)
        {
            Printf("Fail: 0x%x = 0x%08x\n", 0x2000e030, u4MBISTStatus);
        }
        fgResult = FALSE;
    }

    u4MBISTStatus = IO_READ32MSK(IO_VIRT, 0x0e038, 0x0181c000);
    if(u4MBISTStatus)
    {
        if(logOn)
        {
            Printf("Fail: 0x%x = 0x%08x\n", 0x2000e038, u4MBISTStatus);
        }
        fgResult = FALSE;
    }

    u4MBISTStatus = IO_READ32MSK(IO_VIRT, 0x0e03c, 0x00060000);
    if(u4MBISTStatus)
    {
        if(logOn)
        {
            Printf("Fail: 0x%x = 0x%08x\n", 0x2000e03c, u4MBISTStatus);
        }
        fgResult = FALSE;
    }

	return fgResult;
}

static INT32 _SltCmdMBIST(INT32 i4Argc, const CHAR **szArgv)
{
	if(_fgMBIST(TRUE))
	{
		Printf("MBISTOK\n");
	}
	else
	{
		Printf("MBISTFAIL\n");
	}
   return 0;
}
/*
static INT32 _SltTestVPllSS(UINT32 u4SSpermillage, UINT32 u4DelayUs, UINT32 u4Loop)
{
    UINT32 u4VPllSSpermillage = u4SSpermillage;
    UINT32 u4VPllMinClkFreq;
    UINT32 u4VPllMaxClkFreq;
    UINT32 u4TmpCnt;
    UINT32 u4VPllClkFreq;
    UINT32 u4VPllCentClkFreq;

    u4VPllCentClkFreq = BSP_GetDomainClock(CAL_SRC_LVDS_DPIX_CK);
    u4VPllMinClkFreq = u4VPllMaxClkFreq = u4VPllCentClkFreq;

    vDrvSetVOSpread(u4VPllSSpermillage);

    for(u4TmpCnt=0;u4TmpCnt<u4Loop;u4TmpCnt++)
    {
        u4VPllClkFreq = BSP_GetDomainClock(CAL_SRC_LVDS_DPIX_CK);
        if(u4VPllClkFreq < u4VPllMinClkFreq)
        {
            u4VPllMinClkFreq = u4VPllClkFreq;
        }
        else if(u4VPllClkFreq > u4VPllMaxClkFreq)
        {
            u4VPllMaxClkFreq = u4VPllClkFreq;
        }
        HAL_Delay_us(u4DelayUs);
    }

    Printf("VPll ss_range:(%d o/oo)\n",(u4VPllMaxClkFreq-u4VPllMinClkFreq)/(u4VPllCentClkFreq/1000));
    vDrvSetVOSpread(0);

    if((u4VPllMaxClkFreq-u4VPllMinClkFreq)/(u4VPllCentClkFreq/1000) > u4VPllSSpermillage)
    {
 */

 /* out of the expected ss range */
   /*     return SV_TRUE;
    }
    return SV_FALSE;
}
*/

static INT32 _SltCmdQMBIST(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4LoopCnt;
    UINT32 u4TmpAddr;
    UINT32 u4TmpBit;
    if(IO_READ32(IO_VIRT, 0x28140) != 0)
    {
        Printf("MTFAIL\n");

        for(u4LoopCnt=0;u4LoopCnt<4;u4LoopCnt++)
        {
            if(IO_READ32(IO_VIRT, 0x28140+u4LoopCnt*4)!=0)
            {
                u4TmpAddr = ((IO_READ32(IO_VIRT, 0x28140+u4LoopCnt*4)& 0xffff0000)>>16);
                u4TmpBit = (IO_READ32(IO_VIRT, 0x28140+u4LoopCnt*4) & 0xffff);
                Printf("0x%08x = 0x%08x(%d) [%d]\n",
                       u4TmpAddr|IO_VIRT,
                       1 << u4TmpBit,
                       u4TmpBit,
                       (u4TmpAddr >= 0xd650)?((u4TmpAddr-0xd650)/4*16+u4TmpBit):(0) );
            }
        }
    }
    else
    {
        Printf("MTOK\n");
    }
    return 0;
}

static INT32 _SltCmdPQTorture(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4RndTms = (i4Argc)>1?StrToInt(szArgv[1]):10;

    if(_SltPqStress(i4RndTms) != 0)
    {
        Printf("PQFAIL\n");
    }
    else
    {
        Printf("PQPASS\n");
    }

    return 0;
}

static INT32 _SltPqStress(INT32 i4RoundTimes)
{
    CHAR* strMod[] = {"gamma", "sce", "ylev sharpness", "local sharpness", "scaler", "Lc dimming", "OD"};

    INT32 u4mod, u4ptn = 0/*complement data*/;

    for(u4mod=0;u4mod<7;u4mod++)
    {
        if(SV_FALSE == bDrvPeSramStressTest(u4mod, u4ptn, i4RoundTimes))
        {
          LOG(0, "fail @%d (%s)\n", u4mod, strMod[u4mod]);
          /* test fail */
          return 1;
        }
    }

    return 0;
}
static INT32 _SltCmdVersion(INT32 i4Argc, const CHAR **szArgv)
{
    CHAR *szBuildDate;
    CHAR *szDrvCust;
    UINT8 u1TmpCnt;
    char strBuildDate[30];
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    if (DRVCUST_InitQuery(eDrvCust, (UINT32 *)(void *)&szDrvCust)!=0)
    {
        szDrvCust = "??";
    }
    if (DRVCUST_InitQuery(eBuildDate, (UINT32 *)(void *)&szBuildDate)!=0)
    {
        szBuildDate = "??";
    }
    x_strcpy(strBuildDate, szBuildDate);

    strBuildDate[3] = '\0';
    strBuildDate[6] = '\0';

    for(u1TmpCnt=0;u1TmpCnt<12;u1TmpCnt++)
    {
        if(x_strcmp(months[u1TmpCnt], strBuildDate) == 0)
        {
            break;
        }
    }

    if(strBuildDate[4] == ' ') strBuildDate[4] = '0';

    Printf("\nSLT FW_VERSION:\n%s_%02d%02d\n", szDrvCust, u1TmpCnt+1, StrToInt(strBuildDate+4));

    return 0;
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
CLI_EXEC_T* GetSltCmdTbl(void)
{
    return &_rSltRootCmd;
}
#else

#include "slt_if.h"
#include "vdp_if.h"
#include "dmx_if.h"
#include "musb_if.h"
#include "video_def.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_rand.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_pdwnc.h"
#include "vdp_drvif.h"
#include "nim_drvif.h" //EC_code_2007/6/22
#include "hw_vdoin.h"
#include "hw_scpos.h"
#include "hw_ckgen.h"
#include "drv_vdoclk.h"
#include "drv_ycproc.h"
#include "hw_ycproc.h"
#include "drv_di.h"
#include "drv_nr.h"
#include "util.h"//kal
#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"
#include "x_timer.h"
#include "x_drv_cli.h"
#include "drv_pqstress.h"
#include "aud_if.h"
#include "aud_drvif.h"

//extern UINT32 StrToInt(const CHAR* pszStr);


static INT32 _SltCliTest(INT32 i4Argc, const CHAR ** szArgv)
{
    /*INT32 skipMask = (i4Argc)>1?StrToInt(szArgv[1]):0x0;
    INT32 dramclk = (i4Argc)>2?StrToInt(szArgv[2]):(DRVCUST_InitGet(eDdrClock));
    INT32 parallel_aud = (i4Argc)>3?StrToInt(szArgv[3]):1;
    INT32 parallel_misc = (i4Argc)>4?StrToInt(szArgv[4]):1;
    */
    Printf("i4Argc=%d\n", i4Argc);
    Printf("szArgv[0]=%s\n", StrToInt(szArgv[0]));//cmd
    Printf("szArgv[1]=%d\n", StrToInt(szArgv[1]));//arg1
    Printf("szArgv[2]=%d\n", StrToInt(szArgv[2]));
    Printf("szArgv[3]=%d\n", StrToInt(szArgv[3]));

    return 0;
}

static INT32 _SltCmdReboot(INT32 i4Argc, const CHAR ** szArgv)
{
    //magic # for doing mbist when looping
    IO_WRITE32(IO_VIRT, 0x28140, 0xaaaaff00);

    CLI_Parser("b.reboot");
	return 0;
}

static CLI_EXEC_T _arSltCmdTbl[] =
{
    {"test", "t",       _SltCliTest, NULL,      "CLI test", CLI_SUPERVISOR },
    {"reboot", NULL,    _SltCmdReboot, NULL,    "Reboot system", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR},

};

#endif//__MODEL_slt__

CLI_MAIN_COMMAND_ITEM(SLT)
{

    "slt", "slt", NULL, _arSltCmdTbl, "SLT commands", CLI_GUEST

};

