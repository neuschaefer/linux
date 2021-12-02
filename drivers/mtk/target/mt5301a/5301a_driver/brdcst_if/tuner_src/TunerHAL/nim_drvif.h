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
 *
 *---------------------------------------------------------------------------*/

/** @file nim_drvif.h
 *  nim_drvif.c function prototype declaration.
 */

#ifndef NIM_DRVIF_H
#define NIM_DRVIF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"
#include "tuner_if.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "u_rm.h"
#include "u_tuner.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define QAM16    0
#define QAM32    1
#define QAM64    4
#define QAM128   2
#define QAM256   3

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
// nim break call back

typedef INT32 (*PF_NIM_BREAK_CB_T)(void);

#if 0
/** CABLE_CHANNEL_SIGNAL_T: cable channel parameter.
 *
 */
typedef struct _CABLE_CHANNEL_SIGNAL_T
{
    INT32 i4FrequencyK;   // in KHz
    UINT32 u4SymbolRateK; // in KSymbol
    INT32 i4QamSize;      // 16/32/64/128
//    UINT32 e_bandwidth;                /*EU mw: BW_UNKNOWN = 0, BW_6_MHz, BW_7_MHz, BW_8_MHz*/
//    UINT32 e_hierarchy_priority;       /*EU mw: HIR_PRIORITY_UNKNOWN = 0, HIR_PRIORITY_NONE, HIR_PRIORITY_HIGH, HIR_PRIORITY_LOW */

} CABLE_CHANNEL_SIGNAL_T;
#endif

/** TUNER_ALL_INFO_T
 *
 */
typedef union
{
    TUNER_CAB_DIG_TUNE_INFO_T t_cab_tuner_info;
    TUNER_TER_DIG_TUNE_INFO_T t_ter_tuner_info;
    TUNER_CAB_ANA_TUNE_INFO_T			t_cana_tuner_info;
    TUNER_TER_ANA_TUNE_INFO_T           t_tana_tuner_info;    
    TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T t_orx_tuner_info;
    TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T t_otx_tuner_info;
} TUNER_ALL_INFO_T;

typedef void (*PFN_NIM_ERR_NOTIFY)(UINT32 u4ErrCnt, UINT32 u4MicroSec);

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
extern INT32 TunerNimTest(UINT16 TunerID);
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

/** Tuner Primitive API: stop monitor.
 *
 *
 *  @return INT32.\n
 *    always return 0
 */
INT32 NIM_MonitorStop(void);

/** Tuner Primitive API: start monitor.
 *
 *
 *  @return INT32.\n
 *      always return 0
 */
INT32 NIM_MonitorStart(void);

/** Tuner Primitive API: Open tuner.
 *
 *
 *  @return INT32.\n
 *          If success, return DRVAPI_TUNER_OK( = 0).
 *          If fail, return error code ( < 0).
 */
INT32 NIM_Open(void);

/** Tuner Primitive API: close tuner.
 *
 *
 *  @return INT32.\n
 *          If success, return DRVAPI_TUNER_OK( = 0).
 *          If fail, return error code ( < 0).
 */
INT32 NIM_Close(void);

/** Tuner Primitive API: tuner set cable parameter.
 *
 *  @param[in]     u4FreqK      frequency khz.
 *  @param[in]     u4SymbolK    k_symbol_sec.
 *  @param[in]     i4QamSize    QAM size.
 *  @param[in]     fgSpectrumInversion    SpectrumInversion.
 *  @param[in]     pfNimBreak    break function.
 *  @param[in]     uAcqType      acq. type.
 *  @param[in]     uLoopCounts   re-try acq. times.
 *  @param[in]     u4DelayMs     delay n ms for every re-try acq.
 *
 *  @return INT32.\n
 *          If success, return DRVAPI_TUNER_OK( = 0).
 *          If fail, return error code ( < 0).
 */
//void NIM_SetCable(UINT32 u4FreqK, UINT32 u4SymbolK, INT32 i4QamSize, BOOL fgSpectrumInversion,
//                  PF_NIM_BREAK_CB_T pfNimBreak);
//void NIM_SetCable(UINT32 u4FreqK, UINT32 u4SymbolK, INT32 i4QamSize,
//    BOOL fgSpectrumInversion, PF_NIM_BREAK_CB_T pfNimBreak, TUNER_ACQ_ACTION_T uAcqType, UINT32 uLoopCounts);
BOOL NIM_SetCable(UINT32 u4FreqK, UINT32 u4SymbolK, INT32 i4QamSize,
    BOOL fgSpectrumInversion, PF_NIM_BREAK_CB_T pfNimBreak, TUNER_ACQ_ACTION_T uAcqType, UINT32 uLoopCounts, UINT32 u4DelayMs);

/** Tuner Primitive API: Set Bandwidth.
 *
 *
 *  @param[in]     u4Bandwidth   bandwidth.
 *
 *  @return void.\n
 *
 */
void NIM_SetBW(UINT32 u4Bandwidth);

/** Tuner Primitive API: tuner get cable signal parameter.
 *
 *  @param[out]     prCableSignal     pointer to cable signal parameter structure.
 *
 *  @return void.\n
 */
void NIM_GetCableSignal(SIGNAL *prCableSignal);

/** Tuner Primitive API: tuner get cable signal level.
 *
 *
 *  @return UINT8.\n
 *          return signal level
 */
UINT8 NIM_GetCableSignalLevel(void); // AGC2 * 100 / 0x3ff

/** Tuner Primitive API: tuner get cable signal SNR.
 *
 *
 *  @return UINT16.\n
 *      return signal SNR
 */
UINT16 NIM_GetCableSignalSNR(void);

/** Tuner Primitive API: tuner get cable signal PER.
 *
 *
 *  @return UINT16.\n
 *      return signal PER
 */
UINT16 NIM_GetCableSignalPER(void);

/** Tuner Primitive API: Nim test
 *
 *
 *  @return UINT16.\n
 *      return 0    ok
 *      return !=0  fail
 */
INT32 NIM_TunerNimTest(void);

/** Tuner Primitive API: tuner get cable sync.
 *
 *
 *  @return UINT16.\n
 *      success =1, fail <=0
 */
INT16 NIM_GetCableSync(void); // INT16 CheckLock()

INT32 _nim_SetTimeout(UINT32 u4TimeoutMs);
INT32 _nim_Breaker(void);

/** Tuner Primitive API: tuner get error count.
 *
 *
 *  @return UINT32.\n
 *      return error count
 */
UINT32 NIM_GetErrorCnt(void);

//void NIM_RegErrNotifyFunc(PFN_NIM_ERR_NOTIFY pfFunc);
BOOL NIM_PollingToggle(void);
UINT32 NIM_SetPollPeriod(UINT32 u4Period);

/** Tuner Primitive API: tuner detach I2C.
 *
 *
 *  @return UINT32.\n
 *      always return 0
 */
UINT32 NIM_DetachI2C(UINT32 u4Param);

void NIM_RegErrNotifyFunc(PFN_NIM_ERR_NOTIFY pfFunc);

// Added by rocky_huang 2006/07/14
EXTERN void TunerDetachI2C(UINT16 TunerID, BOOL bDetach);
EXTERN void TunerDetachMW(UINT16 TunerID, BOOL bDetach);
EXTERN void TunerCommand(UINT16 TunerID, INT32 i4Argc, const CHAR** aszArgv);
EXTERN void TunerDtdCommand(UINT16 TunerID, INT32 i4Argc, const CHAR** aszArgv);
EXTERN void TunerAtdCommand(UINT16 TunerID, INT32 i4Argc, const CHAR** aszArgv);

/** Tuner Primitive API: Set connection type.
 *
 *
 *  @param[in]     e_conn_type   connection type.
 *
 *  @return void.\n
 *
 */
void NIM_SetConnType(DRV_CONN_TYPE_T e_conn_type);

DRV_CONN_TYPE_T NIM_GetConnType(void);
/** Tuner Primitive API: Set modulation type.
 *
 *
 *  @param[in]     Mod   modulation type.
 *
 *  @return void.\n
 *
 */
void NIM_SetMod(TUNER_MODULATION_T Mod);

/** Tuner Primitive API: Set Tuner ID.
 *
 *
 *  @param[in]     TunerID   Tuner ID.
 *
 *  @return void.\n
 *
 */
void NIM_SetTunerID(UINT16 TunerID);

/** Tuner Primitive API: Get version.
 *
 *
 *  @return void.\n
 *
 */
void NIM_CmdVer(void);

void NIM_CmdHostCmd(INT32 i4Argc, const CHAR **szArgv);
void NIM_CmdDtdCmd(INT32 i4Argc, const CHAR **szArgv);
void NIM_CmdAtdCmd(INT32 i4Argc, const CHAR **szArgv);

/** Tuner Primitive API: Get Tuner ID.
 *
 *
 *
 *  @return UINT16.\n
 *
 */
UINT16 NIM_GetTunerID(void);

/** Tuner Primitive API: Get debug level.
 *
 *
 *  @return UCHAR.\n
 *
 */
UCHAR NIM_GetDbgLvl(void);

/** Tuner Primitive API: Set debug level.
 *
 *
 *  @return void.\n
 *
 */
void NIM_SetDbgLvl(UCHAR dbg_lvl);

/** Tuner Primitive API: Get Ts Format.
 *
 *
 *  @return void.\n
 *
 */
void NIM_GetTsFmt(UCHAR *pucSerorPal, UCHAR *pucClockPol, UINT32 *pu4TsClock, BOOL *pbGatedClock, BOOL *pbExternalDemod);

/** Tuner Primitive API: Set Ts SP.
 *
 *
 *  @return void.\n
 *
 */
void NIM_SetTsSP(UCHAR ucSerorPal);

/** Tuner Primitive API: Set Ts tristate.
 *
 *
 *  @return void.\n
 *
 */
void NIM_SetTsTristate(UCHAR ucTristate);

#endif // NIM_DRVIF_H
