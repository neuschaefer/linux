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
#include "drv_ycproc.h"
#include "hw_ycproc.h"
#include "drv_di.h"
#include "drv_nr.h"
#include "util.h"//kal
#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"
#include "x_timer.h"
#include "x_drv_cli.h"

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
       
//#define PARALLEL_AUD

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
extern UINT32 dwSI_DISPLAY_DCLK_TYPICAL;
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
EXTERN INT32 SLT_AtvSorting(void);
EXTERN INT32 SLT_DtvSorting(void);
EXTERN INT32 SLT_AtdSorting(void);
//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static INT32 _SltCmdInit(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _SltCmdFlashingAud(INT32 i4Argc, const CHAR ** szArgv);
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

CLIMOD_DEBUG_FUNCTIONS(CLI)

static void _AudProc(void* pvArg);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static CLI_EXEC_T _arSltCmdTbl[] =
{
    {"init", NULL, _SltCmdInit, NULL, "init", CLI_SUPERVISOR},
    {"audflash", NULL, _SltCmdFlashingAud, NULL, "audflash", CLI_SUPERVISOR},
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
    {"pll", NULL, _SltCmdPll, NULL, "pll", CLI_SUPERVISOR},
    {"showpll", NULL, _SltCmdShowPll, NULL, "show PLL value", CLI_SUPERVISOR},
    {"dtd", NULL, _SltCmdDtd, NULL, "atsc demod test", CLI_SUPERVISOR},
    {"atd", NULL, _SltCmdAtd, NULL, "analog demod test", CLI_SUPERVISOR},
    //{"qdtd", NULL, _SltCmdQDtd, NULL, "get atsc demod test result", CLI_SUPERVISOR},    
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
UINT16 u2SignalPER_dvbt[2];
UINT16 u2SignalPER2[2];//EC_code_2007/6/22 
static HAL_TIME_T DTD_ProcessTime[2][10];
//static UINT32 u4DTDRetryRecord[2]; //useless
	
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

static void _SltAuto(UINT32 u4VdpId)
{    
    _SetVGAAuto(u4VdpId);
    x_sema_lock(_hSltAuto, X_SEMA_OPTION_WAIT); 
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _SltAutoColor(UINT32 u4VdpId)
{    
    _SetVGAAutoColor(u4VdpId);
    x_sema_lock(_hSltAutoColor, X_SEMA_OPTION_WAIT); 
}

#if 0
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltTestYPbPrSlicer(UINT32 u4Mode)
{
    BOOL fgPass;
    if(fgPass)
    {
        Printf("=>PASS\n");
        return 0;
    }
    else
    {
        Printf("=>FAIL\n");
        return 1;
    }	
}
#endif

#if 1  
    #define CLK_BOUND     1000000    //60000000/256
#else
#ifdef CC_XTAL_60MHz
    #define CLK_BOUND     234375    //60000000/256
#else  /* CC_XTAL_60MHz */
#ifdef CC_XTAL_27MHz
     #define CLK_BOUND    105469    //27000000/256
#else /* CC_XTAL_27MHz */
     #define CLK_BOUND    210938    //54000000/256
#endif /* CC_XTAL_27MHz */
#endif  /* CC_XTAL_60MHz */
#endif
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
typedef enum
{
    FS_16K = 0x00,
    FS_22K,
    FS_24K,
    FS_32K,
    FS_44K,
    FS_48K,
    FS_64K,
    FS_88K,
    FS_96K,
    FS_176K,
    FS_192K,
    FS_8K, // appended since 09/10/2007, don't change the order
    FS_11K, // appended since 09/10/2007, don't change the order
    FS_12K// appended since 09/10/2007, don't change the order
}   SAMPLE_FREQ_T;

#define AUD_DEC_MAIN            ((UINT8)0)
#define AUD_DEC_AUX             ((UINT8)1)

extern void AUD_SetSampleFreq(UINT8 u1DecId, SAMPLE_FREQ_T eSmpFreq);

static INT32 _SltTestPll(CAL_SRC_T eSrc, UINT32 u4Target, UINT32 u4Bound)
{
    UINT32 u4CalNs = 0, u4Clock;
    INT32 i4Fail = 0;
    UINT8 ulBand;
    UINT16 u2MS, u2NS, u2Counter;

    switch (eSrc)
    {
    case CAL_SRC_DMPLL:
        Printf("test DMPLL: ");
        break;
    case CAL_SRC_SYSPLL:
        Printf("test SYSPLL: ");
        break;
    //case CAL_SRC_CPUPLL:
        //Printf("test CPUPLL: ");
        //break;
    case CAL_SRC_VOPLL:
        Printf("test VOPLL: ");
        break;
    #if !defined(CC_MT8223)
    case CAL_SRC_TVDPLL:
        Printf("test TVDPLL: ");
        break;
    case CAL_SRC_DTDPLL:
        Printf("test DTDPLL: ");
        break;  
    #endif
    case CAL_SRC_APLL1:
        Printf("test APLL1: ");
        break;
    case CAL_SRC_APLL2:
        Printf("test APLL2: ");
        break;
    default:
        return -1;
    }
    if (eSrc == CAL_SRC_APLL1)
    {
        if(u4Target == CLK_APLL270MHZ)
        {
            AUD_SetSampleFreq(AUD_DEC_MAIN, FS_44K);
        }       
        else if (u4Target == CLK_APLL294MHZ)
        {
            AUD_SetSampleFreq(AUD_DEC_MAIN, FS_48K);
        }       
    }
    else if (eSrc == CAL_SRC_APLL2)
    {
        if(u4Target == CLK_APLL270MHZ)
        {
            AUD_SetSampleFreq(AUD_DEC_AUX, FS_44K);
        }       
        else if (u4Target == CLK_APLL294MHZ)
        {
            AUD_SetSampleFreq(AUD_DEC_AUX, FS_48K);
        }  
    }

    
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

#if 0
        if (eSrc == CAL_SRC_CPUPLL || eSrc == CAL_SRC_APLL0 ||
            eSrc == CAL_SRC_APLL1)
        if (eSrc == CAL_SRC_APLL1 || eSrc == CAL_SRC_APLL2)
        {
            VERIFY(BSP_Calibrate(eSrc, u4Target));
            ++u4CalNs;
        }
        else
#endif
        {
            u4CalNs = MAX_CAL_NS;
        }
    } while (u4CalNs < MAX_CAL_NS);

    if (i4Fail == 0)
    {
        Printf("PASS\n");
    }
    return i4Fail;
}

#if 0
//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _SltReset(void)
{
    if (_u4Mode == _MODE_PLAY_OSD)
    {
        // reset osd
        //CLI_Parser("osd.init");
    }
    else
    {
        // un-freeze vdp plane
        CLI_Parser("vdp.f 0 0");
        CLI_Parser("vdp.f 1 0");
        CLI_Parser("pmx.p 0");
    }
}
#endif

//-----------------------------------------------------------------------------
/** Brief
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

	
    // clear demo program's osd
    CLI_Parser("osd.init");

    NAV_VQ_Reset(0);

    CLI_Parser("pmx.s.d 15");
	
    CLI_Parser("n.d_l 0"); 
    CLI_Parser("n.di.d_l 0");
    CLI_Parser("n.scl.d_l 0");
    CLI_Parser("n.nr.d_l 0");
    CLI_Parser("n.mute.d_l 0");

    CLI_Parser("jpg.d_l 0"); 

    //CLI_Parser("vdp.s.bob 0 0");    
    
    SLT_Init();

    //_u4Mode = _MODE_INIT;
    ACK;
    return 0;
}



//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdFlashingAud(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    //SLT_FlashingAudInit();

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief
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
/** Brief
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
/** Brief
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
/** Brief
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
	        CLI_Parser("nav.play m hdmi1");        
	    }
	    else if(port == 2)
	    {
	        CLI_Parser("nav.play m hdmi2");        
	    }
	    else if(port == 3)
	    {
	        CLI_Parser("nav.play m hdmi3");        
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

    do
    { 
        x_thread_delay(100);
    } while (_bDviModeChged);

    x_thread_delay(100);

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
	vDrvNRBypass(SV_ON);
    vDrvPostSharpOnOff(SV_OFF);
    CLI_Parser("slt.bypassmlc 1"); // bypass MLC
	
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
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdTve(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
    INT32 port = (i4Argc)>2?StrToInt(szArgv[2]):1;  
    INT32 delay2 = (i4Argc)>3?StrToInt(szArgv[3]):1000; 
    INT32 delay3 = (i4Argc)>4?StrToInt(szArgv[4]):1000; 
    
    CLI_Parser("vdp.f 0 1");    
    CLI_Parser("vdp.f 0 0"); 
    
    CLI_Parser("vdp.f 1 1");    
    CLI_Parser("vdp.f 1 0"); 
    CLI_Parser("n.ms off");
    //CLI_Parser("n.ps off");
    CLI_Parser("nav.play m av1"); 	
    //CLI_Parser("n.ms av1"); 	
    CLI_Parser("vdp.s.overscan 0 25 250 80 100");

    CLI_Parser("tve.ce 1");
    CLI_Parser("tve.s.f 0 0");//NTSC
//    CLI_Parser("tve.s.f 0 1");//PAL
//*******OSD pattern gen setting***********
    CLI_Parser("osdt.pt 2 5 720 480 1440 480 32");//NTSC
    IO_WRITE32MSK(OSD_BASE, 0x8, 0x00040000, 0xffffffff);
    IO_WRITE32MSK(OSD_BASE, 0x1c, 0x06b4020d, 0xffffffff);
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x350b0110, 0xffffffff);
    IO_WRITE32MSK(OSD_BASE, 0x404, 0x02d001e0, 0xffffffff);
    IO_WRITE32MSK(OSD_BASE, 0x408, 0x05a0020d, 0xffffffff);
    CLI_Parser("osd.p.fa 2 0");
 //*******End of OSD pattern gen setting***********
  
    CLI_Parser("tve.po 0 1");
 CLI_Parser("tve.s.osd 1");//osd path enable
    
    //CLI_Parser("slt.bypassmlc 0");
#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT  
    TVE_SLT_SRCREMAP_EN = 0;
#endif
    WAIT_SIGNAL_STABLE;
/*    
    do 
	{
		x_thread_delay(100);
	} while (_bDviModeChged);
	x_thread_delay(delay2);
*/	
#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT
     TVE_SLT_SRCREMAP_EN = 1;
#endif     
	vDrvPostSharpOnOff(SV_OFF);
    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("TVEOK\n"); \
    } \
    while (0);

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
	vVideoAllPQItemOnOff((UINT8)i4OnOff);
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdVga(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):2000;
    INT32 delay2 = (i4Argc)>2?StrToInt(szArgv[2]):0;
    INT32 delay3 = (i4Argc)>2?StrToInt(szArgv[3]):0;

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
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdOsd(INT32 i4Argc, const CHAR ** szArgv)
{
    //INT32 mode = StrToInt(szArgv[1]);
    INT32 delay = (i4Argc)>1?StrToInt(szArgv[1]):0;
    //_SltReset();
    
    CLI_Parser("osdt.pt 1 4 1920 1080");
    CLI_Parser("osdt.pt 2 5 720 480 1920 1080 32 0 0");
    CLI_Parser("osd.p.fa 2 150");

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
/** Brief
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
    INT32 bypassMask = (i4Argc)>4?StrToInt(szArgv[4]):0;
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
	vDrvNRBypass(SV_ON);
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
/** Brief
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
	    
	    
	    CLI_Parser("nav.play m vga");   
        WAIT_SIGNAL_STABLE;
	    //CLI_Parser("vdp.s.overscan 0 25 25 25 25");
	    CLI_Parser("nav.play s sv1");    
		WAIT_SIGNAL_STABLE_PIP;
	    CLI_Parser("vdp.s.overscan 1 50 50 600 100");
	    
	    _SltAutoColor(VDP_1);
        _SltAuto(VDP_1);           
            
	    x_thread_delay(delay2);       
	    //SLT_FreezeHdmiFrame(VDP_2, 0);
	    //SLT_WaitFreezeDone();
	    //_u4Mode = _MODE_PLAY_ATV;
		// end cvbs
	}
	else if(mode == 1) // Cvbs+Ypbpr
	{
	    CLI_Parser("nav.sm pip");           
        if (DRVCUST_InitGet(eFlagDDRQfp)) //mt5387 condition
        {
            CLI_Parser("nav.play m sv1");  // use sv1 for MT5387 (av1 is for tve test)
        }
        else
        {
    	    CLI_Parser("nav.play m av2");  // use av2 for MT5388 (av1 is for tve test)
        }
        WAIT_SIGNAL_STABLE;
	    CLI_Parser("vdp.s.overscan 0 50 50 600 100");
	    CLI_Parser("nav.play s ypbpr1");   
        WAIT_SIGNAL_STABLE_PIP;
	    CLI_Parser("vdp.s.overscan 1 50 50 570 50");
	    
	    _SltAutoColor(VDP_2);

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
    SLT_AtvSorting();
    x_thread_delay(delay);
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
    INT32 dramclk = (i4Argc)>1?StrToInt(szArgv[1]):432000000;

	_SltTestPll(CAL_SRC_DMPLL, dramclk, ((dramclk / 1000) * 7));
    _SltTestPll(CAL_SRC_SYSPLL, 324000000, CLK_BOUND);
    //_SltTestPll(CAL_SRC_CPUPLL, 594000000, CLK_BOUND);  // to confirm CPUPLL?
    _SltTestPll(CAL_SRC_VOPLL, dwSI_DISPLAY_DCLK_TYPICAL, CLK_BOUND);
    #if !defined(CC_MT8223)
    _SltTestPll(CAL_SRC_TVDPLL, 540000000, CLK_BOUND);
    _SltTestPll(CAL_SRC_DTDPLL, 600000000, CLK_BOUND);
    #endif
    _SltTestPll(CAL_SRC_APLL1, CLK_APLL294MHZ, CLK_BOUND);
    _SltTestPll(CAL_SRC_APLL1, CLK_APLL270MHZ, CLK_BOUND);
    _SltTestPll(CAL_SRC_APLL2, CLK_APLL270MHZ, CLK_BOUND);
    _SltTestPll(CAL_SRC_APLL2, CLK_APLL294MHZ, CLK_BOUND);

    return 0;  
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdMisc(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 skipMask = (i4Argc)>1?StrToInt(szArgv[1]):0x0;
    INT32 dramclk = (i4Argc)>2?StrToInt(szArgv[2]):405000000;
    INT32 parallel_aud = (i4Argc)>3?StrToInt(szArgv[3]):1;
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
        _SltTestPll(CAL_SRC_DMPLL, dramclk, ((dramclk / 1000) * 7))) // DMPLL spread spectrum 0.7%
    {
        ucCode = '1';
    }    
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(CAL_SRC_SYSPLL, 324000000, CLK_BOUND))
    {
        ucCode = '2';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(CAL_SRC_VOPLL, dwSI_DISPLAY_DCLK_TYPICAL, CLK_BOUND))
    {
        ucCode = '3';
    }
    #if !defined(CC_MT8223)
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(CAL_SRC_TVDPLL, 540000000, CLK_BOUND))
    {
        ucCode = '4';
    }
    else if (((skipMask & 0x1) == 0) &&
             _SltTestPll(CAL_SRC_DTDPLL, 600000000, CLK_BOUND))
    {
        ucCode = '5';
    }    
    #endif
    else if (((skipMask & 0x1) == 0) && 
        _SltTestPll(CAL_SRC_APLL1, CLK_APLL270MHZ, CLK_BOUND))
    {
        ucCode = '6';
    }
    else if (((skipMask & 0x1) == 0) && 
        _SltTestPll(CAL_SRC_APLL1, CLK_APLL294MHZ, CLK_BOUND))
    {
        ucCode = '7';
    }
    
    else if (((skipMask & 0x1) == 0) && 
        _SltTestPll(CAL_SRC_APLL2, CLK_APLL270MHZ, CLK_BOUND))
    {
        ucCode = '8';
    }        
    else if (((skipMask & 0x1) == 0) && 
        _SltTestPll(CAL_SRC_APLL2, CLK_APLL294MHZ, CLK_BOUND))
    {
        ucCode = '9';
    }    
    else if ((skipMask & 0x10) == 0 && CLI_Parser("sif.diag") != 0)
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
    else if ((skipMask & 0x100) == 0 && CLI_Parser("pmx.diag.gt") != 0) // Gamma table
    {
        ucCode = 'm';
    }
    //else if((skipMask & 0x200) == 0 && MUSB_CheckSLT() != 0) // USB combine with jpeg test
    //{
    //    ucCode = 'u';
    //}
    else if((skipMask & 0x400) == 0 && CLI_Parser("jpg.slt") != 0) // JPEG
    {
        ucCode = 'j';
    }	
    else if((skipMask & 0x800) == 0 &&  CLI_Parser("slt.t8032") != 0)  // up alive test
    {
        ucCode = 't';
    }    
	else if((skipMask & 0x2) == 0 || (skipMask & 0x4) == 0 || (skipMask & 0x8) == 0)
	{
		if(!parallel_aud)
		{
			if (((skipMask & 0x8) == 0) && (SLT_IntAdSorting() != 0))
			{
			    Printf("IAD Soring result (fail)\n");        
			    ucCode = 'c';
			}    
			// digital audio test
			else if (((skipMask & 0x4) == 0) && (SLT_DtvSorting() != 0))
			{
			    Printf("DTV Soring result (fail)\n");    
			    ucCode = 'b';
			}    
			// analog audio test
			else if (((skipMask & 0x2) == 0) && (SLT_AtvSorting() != 0))
			{
			    Printf("ATV Soring result (fail)\n");
			    ucCode = 'a';
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
    UINT32 u4Clock;
    UNUSED(i4Argc);
    UNUSED(szArgv);

    #if !defined(CC_MT8223)
    u4Clock = BSP_GetClock(CAL_SRC_TVDPLL, NULL, NULL, NULL, NULL);
    Printf("TVDPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_DTDPLL, NULL, NULL, NULL, NULL);
    Printf("DTDPLL=%d, ", u4Clock);    
    #endif
    u4Clock = BSP_GetClock(CAL_SRC_DMPLL, NULL, NULL, NULL, NULL);
    Printf("DMPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_SYSPLL, NULL, NULL, NULL, NULL);
    Printf("SYSPLL=%d, ", u4Clock);
    u4Clock = BSP_GetClock(CAL_SRC_VOPLL, NULL, NULL, NULL, NULL);
    Printf("VOPLL=%d, ", u4Clock);
#if 0    
    u4Clock = BSP_GetClock(CAL_SRC_CPUPLL, NULL, NULL, NULL, NULL);
    Printf("CPUPLL=%d, ", u4Clock);    
    u4Clock = BSP_GetClock(CAL_SRC_APLL0, NULL, NULL, NULL, NULL);
    Printf("APLL0=%d, ", u4Clock);
#endif    
    u4Clock = BSP_GetClock(CAL_SRC_APLL1, NULL, NULL, NULL, NULL);
    Printf("APLL1=%d\n", u4Clock);

// 5381
    u4Clock = BSP_GetClock(CAL_SRC_APLL2, NULL, NULL, NULL, NULL);
    Printf("APLL0=%d, ", u4Clock);
    
    return 0;
}

static void _AudProc(void* pvArg)
{	
    if (((_i4SkipMask & 0x8) == 0) && (SLT_IntAdSorting() != 0))
    {
        Printf("IAD Soring result (fail)\n");        
        _ucAudErrorCode = 'c';
    }    
    else if (((_i4SkipMask & 0x4) == 0) && (SLT_DtvSorting() != 0))
    {
        Printf("DTV Soring result (fail)\n");    
        _ucAudErrorCode = 'b';
    }  
    else if (((_i4SkipMask & 0x2) == 0) && (SLT_AtvSorting() != 0))
    {
        Printf("ATV Soring result (fail)\n");
        _ucAudErrorCode = 'a';
    }
    // digital audio test
   	x_sema_unlock(_hAudFinSema);
}

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
		Printf("FAIL:d\n");
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
	UINT8 _aDemodSampFreq[3] = {0, 1, 1}; // Sampling rate = 50, 60, 60MHz.	
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
     		// Set sampling rate: VSB fix to 50MHz. QAM to 60MHz.
     		CLI_Parser_Arg("nim.hc h 0x3c 3 0 %d 0", _aDemodSampFreq[(rDtd.type1) % 4]);
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
	
	////static CHAR _azDemodType[3][8] = {"atsc", "calbe64", "cable256"};//EC_code_2007/6/22: Modulatio type: ATSC0x00, 64QAM0x01, 256QAM0x02
	static CHAR _azDemodType[3][8] = {"6M", "7M", "8M"};
	UINT32 _azDemodFreq[3] = {rDvbt.atsc_freq, 0, rDvbt.qam_freq};	
	int i;
	//CHAR mode_type;
	//UINT8 u1SignalLevel;
	UINT16 u2SignalSNR;
	INT16 i2Sync;
	static UINT32 u4DvbtPatternCounter = 0;

    i = u4DvbtPatternCounter;
	
	_fgDvbtUnsync = FALSE; //EC_code_2007/6/25: For initial state
				
	CLI_Parser_Arg("nim.id 1");
	CLI_Parser_Arg("nim.c");            // free context
	
	//for (i = 0; i < rDvbt.num; i ++)
	{	
		UINT32 u4Retry = 10;
		CLI_Parser_Arg("nim.go dvbt");
		CLI_Parser_Arg("nim.hc ar 1");   // force AR ON
		CLI_Parser_Arg("nim.sbw %d", rDvbt.type1+1);   // 1: 6M, 2: 7M, 3: 8M
		CLI_Parser_Arg("nim.freq %d", _azDemodFreq[(rDvbt.type1 + i) % 3]); // Set modulation type & channel frequency
		
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
		
		Printf("SLT Sync status of %s : %d, flagUnsync : %d\n", _azDemodType[(rDvbt.type1 + i) % 3], (INT32)i2Sync, (INT32)_fgDvbtUnsync); 
														
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
				
		if (i2Sync == 0)
		{
			Printf("Dvbt_PER:1/ Unsync: %s", _azDemodType[(rDvbt.type1 + i) % 3]);
			_fgDvbtUnsync = TRUE;
			//break;
		}								
	}
			
	// Should call this to notify DTD ATSC and QAM tests are both finish
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
	INT32 dtd_type2 = StrToInt(szArgv[3]);
	INT32 dtd_type3 = StrToInt(szArgv[4]);
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
static INT32 _SltCmdDvbt(INT32 i4Argc, const CHAR ** szArgv)
{

	INT32 dvbt_num = StrToInt(szArgv[1]);
	INT32 dvbt_type1 = StrToInt(szArgv[2]);
	INT32 dvbt_type2 = StrToInt(szArgv[3]);
	INT32 dvbt_type3 = StrToInt(szArgv[4]);
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
	dvbt_type1 =  dvbt_type1 & 0x0000000f;   // 0x80: 6M, 0x81: 7M, 0x82: 8M 
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
    if (_hDtdFinSema == NULL)
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
        ////x_sema_lock(_hDtdAtscSema, X_SEMA_OPTION_WAIT);
        x_sema_lock(_hDtdFinSema, X_SEMA_OPTION_WAIT);
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
    INT32 overscan = (i4Argc)>3?StrToInt(szArgv[3]):100;
    CLI_Parser("n.ms atd1");
    CLI_Parser("nim.go c_ana");
    CLI_Parser("nim.atd s 6");
    CLI_Parser_Arg("nim.atd f %d",atd_freq);  
    //CLI_Parser("n.ms atd1");
    

#if 1
      x_thread_delay(1000);

   	if (SLT_AtdSorting() == 0)
	{
		CLI_Parser("nim.atd l");
		//Printf("ATDOK\n");	       
	}
	else
	{
		Printf("ATDFAIL\n");
		Printf("ATD SLT audio fail\n");
	}
  
#endif
   
    //CLI_Parser_Arg("vdp.s.overscan 0 %d %d %d %d", overscan, overscan, overscan, overscan);      
#if 0 // Maggie
    do \
    { \
        if (delay > 0) \
        { \
            x_thread_delay(delay); \
        } \
        Printf("ATDOK\n"); \
    } \
    while (0);
#endif

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
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static INT32 _SltCmdQDvbt(void)
{
#if DVBT_PARALLEL_TEST
    if(x_sema_lock_timeout(_hDtdFinSema, DTD_SLT_TIMEOUT) == OSR_OK)
    {
#endif
        if ((_fgDvbtUnsync == FALSE) && (u2SignalPER_dvbt[0]<20) )
        	Printf("Dvbt_PER:0");
        else
        	Printf("Dvbt_PER:1/ PEC > THR");
#if DVBT_PARALLEL_TEST
    }    	
    else // Some error has occurred
    {
        Printf("Dvbt_PER:1/ Some Error in Sema");
    }
    x_sema_unlock(_hDtdFinSema);
#endif

#if !DVBT_PARALLEL_TEST
    x_sema_unlock(_hDvbtFinSema); // by hotkey to prevent dtv test starting while demod test thread is still running
#endif	

    return 0;
}

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
    vDrvMainOnOff(!_fgBypassMLC);
    //vDrvMain422(!_fgBypassMLC);
    vDrvPipOnOff(!_fgBypassMLC);
    //vDrvPip422(!_fgBypassMLC);
    return 0;
    
}

//-----------------------------------------------------------------------------
/** Brief
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

static INT32 _SltCmdReboot(INT32 i4Argc, const CHAR ** szArgv)
{
    CLI_Parser("b.reboot");
	return 0;
}

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

static INT32 _SltCmdT8032(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 ret, i4Retry = 5;
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ALIVE_CHECK;
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
#if 0 // for 5393 only
	INT32 u4LvdsCtrlAEven = (i4Argc) > 1 ? StrToInt(szArgv[1]) : 0; 
	INT32 u4LvdsCtrlAOdd = (i4Argc) > 2 ? StrToInt(szArgv[2]) : 1;
	INT32 u4LvdsCtrlBEven = (i4Argc) > 3 ? StrToInt(szArgv[3]) : 2;
	INT32 u4LvdsCtrlBOdd = (i4Argc) > 4 ? StrToInt(szArgv[4]) : 3;
	 
	Printf("LVDS Port Assign A=%d, B=%d, C=%d, D=%d\n", u4LvdsCtrlAEven, u4LvdsCtrlAOdd, u4LvdsCtrlBEven, u4LvdsCtrlBOdd);
	vRegWriteFldAlign(SCPOS_OS_81, u4LvdsCtrlAEven, SCPOS_LVDS_A_CTRL);
	vRegWriteFldAlign(SCPOS_OS_81, u4LvdsCtrlAOdd, SCPOS_LDVS_B_CTRL);
   	vRegWriteFldAlign(SCPOS_OS_81, u4LvdsCtrlBEven, SCPOS_LVDS_C_CTRL);
	vRegWriteFldAlign(SCPOS_OS_81, u4LvdsCtrlBOdd, SCPOS_LVDS_D_CTRL);
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
static INT32 _SltCmdDtvCrcCheck(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
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
#endif
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
#include "x_debug.h"
#include "x_drv_cli.h"
static CLI_EXEC_T _arSltCmdTbl[] = {};
#endif

CLI_MAIN_COMMAND_ITEM(SLT)
{

    "slt", "slt", NULL, _arSltCmdTbl, "SLT commands", CLI_SUPERVISOR

};

