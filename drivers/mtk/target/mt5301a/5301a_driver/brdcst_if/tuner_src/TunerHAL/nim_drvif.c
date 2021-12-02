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
 * $RCSfile: nim_drvif.c,v $
 *
 *---------------------------------------------------------------------------*/

/** @file nim_drvif.c
 *  This C file implements the NIM interface functions. It will
 *  call the tuner exported APIs.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "nim_drvif.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "drv_common.h"

#include "TDIS.h"
#include "tuner_if.h"

#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_timer.h"
#include "x_tuner.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define DEFINE_IS_LOG       DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define D_NIM_MORNITOR_GPIO         18   // GPIO_18

#define D_NIM_ERR_REPORT_PERIOD      1000

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern UINT32 os_get_sys_tick(void);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
/* global frontend lib variable, set to 1 will dump debug information */

UCHAR u1DbgLevel = 0;
#if PFXMSG_SUPPORT
UCHAR u1DbgPfxMsgEnable=(PFXMSG_OFF_BY_DEFAULT?FALSE:TRUE);
#endif
static BOOL _fgNimTaskInited = FALSE;
static UINT32 _u4NimTsErrCount = 0;
static UINT16 _u1TunerId = MTK_MT5111_0;
static DRV_CONN_TYPE_T _e_conn_type;
static TUNER_MODULATION_T _Mod;

// Added by rocky_huang 2006/06/19
static TUNER_BANDWIDTH_T _eBandwidth = BW_8_MHz;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
LINT_EXT_HEADER_BEGIN
static void _nim_FormatConnectInfo(DRV_CONN_TYPE_T e_conn_type, void *ptConnInfo,
    UINT32 u4FreqK, UINT32 u4SymbolK, INT32 i4QamSize, BOOL fgSpectrumInversion)
{
    switch (e_conn_type)
    {
    case TUNER_CONN_TYPE_TER_DIG:
        {
            TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info;

            pt_tuner_info = (TUNER_TER_DIG_TUNE_INFO_T *)ptConnInfo;
            pt_tuner_info->ui4_freq = u4FreqK * 1000;
            pt_tuner_info->e_mod = _Mod;
            break;
        }

    case TUNER_CONN_TYPE_CAB_DIG:
        {
            TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info;

            pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *)ptConnInfo;
            pt_tuner_info->ui4_freq = u4FreqK * 1000;
            pt_tuner_info->ui4_sym_rate = u4SymbolK * 1000;
            pt_tuner_info->e_mod = _Mod;
            break;
        }

	case TUNER_CONN_TYPE_SAT_DIG:
        {
            TUNER_SAT_DIG_TUNE_INFO_T *pt_tuner_info;

            pt_tuner_info = (TUNER_SAT_DIG_TUNE_INFO_T *)ptConnInfo;
            pt_tuner_info->ui4_freq = u4FreqK * 1000;
            //pt_tuner_info->ui4_sym_rate = u4SymbolK * 1000;
            //pt_tuner_info->e_mod = _Mod;
            break;
        }

    case TUNER_CONN_TYPE_TER_ANA:
        {
            TUNER_TER_ANA_TUNE_INFO_T *pt_tuner_info;

            pt_tuner_info = (TUNER_TER_ANA_TUNE_INFO_T *)ptConnInfo;
            pt_tuner_info->ui4_freq = u4FreqK * 1000;
            break;
        }

    case TUNER_CONN_TYPE_CAB_ANA:
        {
            TUNER_CAB_ANA_TUNE_INFO_T *pt_tuner_info;

            pt_tuner_info = (TUNER_CAB_ANA_TUNE_INFO_T *)ptConnInfo;
            pt_tuner_info->ui4_freq = u4FreqK * 1000;
            break;
        }

	case TUNER_CONN_TYPE_SAT_ANA:
        {
            TUNER_SAT_ANA_TUNE_INFO_T *pt_tuner_info;

            pt_tuner_info = (TUNER_SAT_ANA_TUNE_INFO_T *)ptConnInfo;
            pt_tuner_info->ui4_freq = u4FreqK * 1000;
            break;
        }
	

    default:
        Printf("Invalid CONN_TYPE!\n");
        return;
    }

    return;
}
LINT_EXT_HEADER_END

#if 0
static void _nim_TsErrGpio(INT32 i4Gpio, BOOL fgStatus)
{
    UNUSED(fgStatus);
    if (i4Gpio == D_NIM_MORNITOR_GPIO)
    {
        _u4NimTsErrCount++;
    }
}
#endif
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// NIM_SetConnType
//
/** Tuner Primitive API: Set connection type.
 *
 *
 *  @param[in]     e_conn_type   connection type.
 *
 *  @return void.\n
 *
 */
//-----------------------------------------------------------------------------
void NIM_SetConnType(DRV_CONN_TYPE_T e_conn_type)
{
    _e_conn_type = e_conn_type;
}

DRV_CONN_TYPE_T NIM_GetConnType(void)
{
    return _e_conn_type;
}

//-----------------------------------------------------------------------------
// NIM_SetMod
//
/** Tuner Primitive API: Set modulation type.
 *
 *
 *  @param[in]     Mod   modulation type.
 *
 *  @return void.\n
 *
 */
//-----------------------------------------------------------------------------
void NIM_SetMod(TUNER_MODULATION_T Mod)
{
    _Mod = Mod;
}

//-----------------------------------------------------------------------------
// NIM_SetTunerID
//
/** Tuner Primitive API: Set Tuner ID.
 *
 *
 *  @param[in]     TunerID   Tuner ID.
 *
 *  @return void.\n
 *
 */
//-----------------------------------------------------------------------------
void NIM_SetTunerID(UINT16 TunerID)
{
    _u1TunerId = TunerID;
}

//-----------------------------------------------------------------------------
// NIM_SetBW
//
/** Tuner Primitive API: Set Bandwidth.
 *
 *
 *  @param[in]     u4Bandwidth   bandwidth.
 *
 *  @return void.\n
 *
 */
//-----------------------------------------------------------------------------
// Added by rocky_huang 2006/06/19
void NIM_SetBW(UINT32 u4Bandwidth)
{
    _eBandwidth = (TUNER_BANDWIDTH_T)u4Bandwidth;
}

//-----------------------------------------------------------------------------
// NIM_CmdVer
//
/** Tuner Primitive API: Get version.
 *
 *
 *  @return void.\n
 *
 */
//-----------------------------------------------------------------------------
void NIM_CmdVer(void)
{
    Printf("%s\n", TunerGetVer(_u1TunerId));
}

LINT_EXT_HEADER_BEGIN
void NIM_CmdHostCmd(INT32 i4Argc, const CHAR **szArgv)
{
    TunerCommand(_u1TunerId, i4Argc - 1, szArgv + 1);
}

void NIM_CmdDtdCmd(INT32 i4Argc, const CHAR **szArgv)
{
    if(_u1TunerId != MTK_DIGITAL_0){
        Printf("TunerId=%d, do \"nim.id 1\" first.\n",_u1TunerId);
        return;
    }
    TunerDtdCommand(_u1TunerId, i4Argc - 1, szArgv + 1);
}

void NIM_CmdAtdCmd(INT32 i4Argc, const CHAR **szArgv)
{
    if(_u1TunerId != MTK_ANALOG_0){
        Printf("TunerId=%d, do \"nim.id 0\" first.\n",_u1TunerId);
        return;
    }
    TunerAtdCommand(_u1TunerId, i4Argc - 1, szArgv + 1);
}
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// NIM_GetTunerID
//
/** Tuner Primitive API: Get Tuner ID.
 *
 *
 *
 *  @return UINT16.\n
 *
 */
//-----------------------------------------------------------------------------
UINT16 NIM_GetTunerID(void)
{
    return (_u1TunerId);
}

//-----------------------------------------------------------------------------
// NIM_GetDbgLvl
//
/** Tuner Primitive API: Get debug level.
 *
 *
 *  @return UCHAR.\n
 *
 */
//-----------------------------------------------------------------------------
UCHAR NIM_GetDbgLvl(void)
{
    return u1DbgLevel;
}

//-----------------------------------------------------------------------------
// NIM_SetDbgLvl
//
/** Tuner Primitive API: Set debug level.
 *
 *
 *  @return void.\n
 *
 */
//-----------------------------------------------------------------------------
void NIM_SetDbgLvl(UCHAR dbg_lvl)
{
    u1DbgLevel = dbg_lvl;
}

//-----------------------------------------------------------------------------
// NIM_MonitorStart
//
/** Tuner Primitive API: start monitor.
 *
 *
 *  @return INT32.\n
 *      always return 0
 */
//-----------------------------------------------------------------------------
INT32 NIM_MonitorStart(void)
{
    if (!_fgNimTaskInited)
    {
        _fgNimTaskInited = TRUE;

        // monitor TSERR pin status.
        //VERIFY(0 <= GPIO_Reg(D_NIM_MORNITOR_GPIO, GPIO_TYPE_INTR_RISE, _nim_TsErrGpio));
    }

    return 0;
}

//-----------------------------------------------------------------------------
// NIM_MonitorStop
//
/** Tuner Primitive API: stop monitor.
 *
 *
 *  @return INT32.\n
 *    always return 0
 */
//-----------------------------------------------------------------------------
INT32 NIM_MonitorStop(void)
{
    return 0;
}

//-----------------------------------------------------------------------------
// NIM_Open
//
/** Tuner Primitive API: Open tuner.
 *
 *
 *  @return INT32.\n
 *          If success, return DRVAPI_TUNER_OK( = 0).
 *          If fail, return error code ( < 0).
 */
//-----------------------------------------------------------------------------
INT32 NIM_Open(void)
{
    INT32 i4Ret;

    i4Ret = TunerOpen();
    Printf("TunerOpen() return %d\n", (INT16)i4Ret);
    return i4Ret;
}

//-----------------------------------------------------------------------------
// NIM_Close
//
/** Tuner Primitive API: close tuner.
 *
 *
 *  @return INT32.\n
 *          If success, return DRVAPI_TUNER_OK( = 0).
 *          If fail, return error code ( < 0).
 */
//-----------------------------------------------------------------------------
INT32 NIM_Close(void)
{
    return TunerClose();
}

//-----------------------------------------------------------------------------
// NIM_SetCable
//
/** Tuner Primitive API: tuner set cable parameter.
 *
 *  @param[in]     u4FreqK      frequency khz.
 *  @param[in]     u4SymbolK    k_symbol_sec.
 *  @param[in]     i4QamSize    QAM size.
 *  @param[in]     fgSpectrumInversion    SpectrumInversion.
 *  @param[in]     pfNimBreak    break function.
 *
 *  @return INT32.\n
 *          If success, return DRVAPI_TUNER_OK( = 0).
 *          If fail, return error code ( < 0).
 */
//-----------------------------------------------------------------------------
BOOL NIM_SetCable(UINT32 u4FreqK, UINT32 u4SymbolK, INT32 i4QamSize,
    BOOL fgSpectrumInversion, PF_NIM_BREAK_CB_T pfNimBreak, TUNER_ACQ_ACTION_T uAcqType, UINT32 uLoopCounts, UINT32 u4DelayMs)
{
//UINT32  fFreqM;
TUNER_ACQ_ACTION_T  eActionType;
TUNER_ALL_INFO_T    tConnInfo;
UINT32  u4TickCnt, i=0;
BOOL    fgRetSts = FALSE;

    tConnInfo.t_ter_tuner_info.e_bandwidth = _eBandwidth;

//    fFreqM = (UINT32)u4FreqK / 1000; // convert to MHz
//    _nim_FormatConnectInfo(_e_conn_type, &tConnInfo, fFreqM, u4SymbolK, i4QamSize,
//        fgSpectrumInversion);
    _nim_FormatConnectInfo(_e_conn_type, &tConnInfo, u4FreqK, u4SymbolK, i4QamSize,
        fgSpectrumInversion);

#if 1 // Temp removed due to other driver not ready at drv_build initial stage
    if(_u1TunerId != MTK_ANALOG_0){ /*acq digital, disconnect analog*/
        TunerDisc(MTK_ANALOG_0, TUNER_CONN_TYPE_TER_ANA);
//        CLI_Parser_Arg("nptv.ms dtv1");
    }
    else{       /*acq analog, disconnect digital*/
           TunerDisc(MTK_MT5111_0, TUNER_CONN_TYPE_TER_DIG);
//           CLI_Parser_Arg("nptv.ms tv");
    }
#endif

    u4TickCnt = os_get_sys_tick();

    eActionType = uAcqType;
    TunerSetAttribute(_u1TunerId, TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS, (void *) &eActionType, sizeof(TUNER_ACQ_ACTION_T));

    for(i=0; i<uLoopCounts; i++)
    {
        u4TickCnt = os_get_sys_tick();

        Printf("NIM_SetCable(): Start TunerAcq uloopCounts=%d\n",(INT16)i);

        _nim_SetTimeout(20000); // timeout after 20 seconds
        fgRetSts = TunerAcq(_u1TunerId, _e_conn_type, &tConnInfo, sizeof(TUNER_ALL_INFO_T), (x_break_fct)pfNimBreak, NULL);
        u4TickCnt = os_get_sys_tick()-u4TickCnt;
        Printf(" NIM_SetCable():End TunerAcq, Connection time = %d ms, delayMs=%d ms\n", (INT16)(u4TickCnt*x_os_drv_get_tick_period()), (INT16)u4DelayMs);
        mcDELAY_MS(u4DelayMs);
    }
    return fgRetSts;
}

//-----------------------------------------------------------------------------
// NIM_DetachMW
//
/** Tuner Primitive API: tuner detach middleware.
 *
 *  @param[in]     bDetach     1: detach , 0: not detach.
 *
 *  @return void.\n
 */
//-----------------------------------------------------------------------------
void    NIM_DetachMW(BOOL bDetach)
{
    TunerDetachMW(_u1TunerId, bDetach);
}

LINT_EXT_HEADER_BEGIN
//-----------------------------------------------------------------------------
// NIM_GetCableSignal
//
/** Tuner Primitive API: tuner get cable signal parameter.
 *
 *  @param[out]     prCableSignal     pointer to cable signal parameter structure.
 *
 *  @return void.\n
 */
//-----------------------------------------------------------------------------
void NIM_GetCableSignal(SIGNAL *prCableSignal)
{
    TunerGetSignal(_u1TunerId, _e_conn_type, prCableSignal);
}
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// NIM_GetCableSignalLevel
//
/** Tuner Primitive API: tuner get cable signal level.
 *
 *
 *  @return UINT8.\n
 *          return signal level
 */
//-----------------------------------------------------------------------------
UINT8 NIM_GetCableSignalLevel(void)
{
    return TunerGetSignalLevel(_u1TunerId, _e_conn_type);
}

//-----------------------------------------------------------------------------
// NIM_GetCableSync
//
/** Tuner Primitive API: tuner get cable sync.
 *
 *
 *  @return UINT16.\n
 *      success =1, fail <=0
 */
//-----------------------------------------------------------------------------
INT16 NIM_GetCableSync(void)
{
    return TunerGetSync(_u1TunerId, _e_conn_type);
}

//-----------------------------------------------------------------------------
// NIM_GetErrorCnt
//
/** Tuner Primitive API: tuner get error count.
 *
 *
 *  @return UINT32.\n
 *      return error count
 */
//-----------------------------------------------------------------------------
UINT32 NIM_GetErrorCnt(void)
{
    return _u4NimTsErrCount;
}

//-----------------------------------------------------------------------------
// NIM_GetCableSignalSNR
//
/** Tuner Primitive API: tuner get cable signal SNR.
 *
 *
 *  @return UINT16.\n
 *      return signal SNR
 */
//-----------------------------------------------------------------------------
UINT16 NIM_GetCableSignalSNR(void)
{
    return TunerGetSignalSNR(_u1TunerId, _e_conn_type);
}
#if 1
//-----------------------------------------------------------------------------
// NIM_TunerNimTest
//
/** Tuner Primitive API: NIM_TunerNimTest.
 *
 *
 *  @return UINT16.\n
 *      return 0    ok
 *      return !=0  fail
 */
//-----------------------------------------------------------------------------
INT32 NIM_TunerNimTest(void)
{
    INT32 i4Ret;

    i4Ret = TunerNimTest(_u1TunerId);
    if(i4Ret == 0){
        Printf("TunerNimTest(TunerId=%d) ok(return=%d)\r\n",_u1TunerId,i4Ret);
    }
    else{
        Printf("TunerNimTest(TunerId=%d) fail(return=%d)\r\n",_u1TunerId,i4Ret);
    }
    return i4Ret;
}
#endif
//-----------------------------------------------------------------------------
// NIM_GetCableSignalPER
//
/** Tuner Primitive API: tuner get cable signal PER.
 *
 *
 *  @return UINT16.\n
 *      return signal PER
 */
//-----------------------------------------------------------------------------
UINT16 NIM_GetCableSignalPER(void)
{
    return TunerGetSignalPER(_u1TunerId, _e_conn_type);
}

//-----------------------------------------------------------------------------
// NIM_DetachI2C
//
/** Tuner Primitive API: tuner detach I2C.
 *
 *
 *  @return UINT32.\n
 *      always return 0
 */
//-----------------------------------------------------------------------------
UINT32 NIM_DetachI2C(UINT32 u4Param)
{
    TunerDetachI2C(_u1TunerId, u4Param);
    return 0;
}

UINT32 NIM_SetFineTune(BOOL fgFineTune)
{
//static BOOL _fgFineTune = FALSE;

//    _fgFineTune = fgFineTune;
    return 0;
}

void NIM_SetTsSP(UCHAR ucSerorPal)
{
    SIG_SP_T e_ts_sp = (SIG_SP_T)ucSerorPal;

    TunerSetAttribute(_u1TunerId, TUNER_SET_TYPE_TS_SP, (void *)&e_ts_sp, sizeof(SIG_SP_T));
}

void NIM_SetTsTristate(UCHAR ucTristate)
{ 
    BOOL_T e_ts_tristate = (BOOL_T)ucTristate;

    TunerSetAttribute(_u1TunerId, TUNER_SET_TYPE_TS_TRISTATE, (void *)&e_ts_tristate, sizeof(BOOL_T));
}

void NIM_GetTsFmt(UCHAR *pucSerorPal, UCHAR *pucClockPol, UINT32 *pu4TsClock, BOOL *pbGatedClock, BOOL *pbExternalDemod)
{
    TS_FMT_T rTsFmt = {SP_UNKNOWN, POL_UNKNOWN_ACTIVE};
    SIZE_T u4TsFmtLen;

    TunerGetAttribute(_u1TunerId, TUNER_GET_TYPE_TS_FMT, (void *)&rTsFmt, &u4TsFmtLen);
    *pucSerorPal = (UCHAR)rTsFmt.SerOrPar;
    *pucClockPol = (UCHAR)rTsFmt.ClockPolarity;
    *pu4TsClock =  rTsFmt.u4TsClockInKHz;
    *pbGatedClock = rTsFmt.fgGatedClock;
    *pbExternalDemod = rTsFmt.fgExternalDemod;
}

LINT_EXT_HEADER_BEGIN
void NIM_RegErrNotifyFunc(PFN_NIM_ERR_NOTIFY pfFunc)
{
}
LINT_EXT_HEADER_END
