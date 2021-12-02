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

#ifndef _TDIS_H_
#define _TDIS_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "PI_Def.h"
#include "x_rm.h"
#include "tuner_if.h"

#ifndef fcADD_DIAG_INFO
#define fcADD_DIAG_INFO
#endif

/***********************************************************************/
#ifdef fcADD_DIAG_INFO

#if 0
#if defined (CC_MT5387)
#include "d_mtk_cust.h"
#else
#include "d_5381_cust.h"
#endif
#else
#include "u_drv_cust.h"
#endif

#endif

/***********************************************************************/
#define TUNER_SOURCE_LABEL  "TUNER_SOURCE_LABEL 2.99.0.0\n"
///////////////////////////////////////////////////////////////////////////////
#define MAX_TUNER_NUM   2

/********** Tuner ID definition ****************************/
// ID definition must follow the rule:
// LSB should be the indicator of TS-BUS-ID
// LSB = 0, indicates this tuner stays in TS0
// LSB = 1, indicates this tuner stays in TS1
#define MTK_MT5111_0            0x0001
#define MTK_MT5111_1            0x0100
#define SAM_T313_0              0x0001
#define SAM_T313_1              0x0100
#define SAM_GEN2004_0           0x0001
#define MTK_MT5112_0            0x0001
#define MTK_MT5112_1            0x0100
#define DUMMY_0                 0x0001
#define DUMMY_1                 0x0000
#define MTK_MT5131_0            0x0001
#define MTK_MT5132_0            0x0001
#define MTK_MT5133_0            0x0001
#define MTK_DTD_0               0x0001
#define MTK_DVBT_0              0x0001
#define MTK_DVBC_0              0x0001
#define MTK_DVBS_0              0x0001 //LIUYUAN,20100202
#define MTK_DVBCDVBT_0          0x0001
#define MTK_ISDBT_0             0x0001
#define MTK_DTMB_0              0x0001
#define MTK_DIGITAL_0           0x0001 /*For DTDDVBT_DTOS40A_P_analog*/
#define OTHER_DEMOD_0           0x0001
#define MTK_ANALOG_0            0x0000
#define VENDOR_0                0x0001
#define UNDEFINED_ID            0xffff

/*********** Tuner Capability *******************************/
#define cWITH_DRVT_TUNER_TER_DIG                0x0001  // BIT_0
#define cWITH_DRVT_TUNER_CAB_DIG                0x0002  // BIT_1
#define cWITH_DRVT_TUNER_CAB_DIG_OOB_RX         0x0004  // BIT_2
#define cWITH_DRVT_TUNER_TER_ANA                0x0008  // BIT_3
#define cWITH_DRVT_TUNER_CAB_ANA                0x0010  // BIT_4
#define cWITH_DRVT_TUNER_TER_ANA_SCART_OUT      0x0020  // BIT_5
#define cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT      0x0040  // BIT_6
#define cWITH_DRVT_TUNER_SAT_DIG                0x0080  // BIT_7    //LIUYUAN,20100202
#define cWITH_DRVT_TUNER_SAT_ANA                0x0100  // BIT_8    //ChiZhang,20100210
#define cWITH_DRVT_TUNER_SAT_ANA_SCART_OUT      0x0200  // BIT_9    //ChiZhang,20100210

#define cCONN_EXCLUSIVE_EN                      0x1000  // BIT_15

/////////////////////////////////////////////////////////////////
typedef void* PTD_CTX;
typedef void* PTD_SPECIFIC_CTX;

typedef struct _TUNER_CONFIG_T{
    UINT16              TunerID;            // MT5111_1, MT5111_2, T313_1 ¡K
                                            // LSB represents the TS bus ID
                                            // LSB = 0, TS0
                                            // LSB = 1, TS1
    UINT16              i2TunerCapability;  // TER, CAB, OOB_RX ¡K
    UCHAR               ucDemodInbAddr;     // For Demod Inband
    UCHAR               ucRFTunerAddr;      // For RF Tuner
    UCHAR               ucDemodOobAddr;     // For Demod Oob
    UINT16              ConnExclusiveID;
    UCHAR               Reserved[8];        // For future extension.
                                            // Should all be "0"s.
// Note this structure is also referenced by MW,
// remember to sync with MW if it is modified.
} TUNER_CONFIG_T, *PTUNER_CONFIG_T;

typedef struct _TUNER_CONFIG_TBL_T
{
    UINT16              TunerNum;
    UCHAR               MutualExcl;
    TUNER_CONFIG_T      tTunerCfg[MAX_TUNER_NUM];
} TUNER_CONFIG_TBL_T, *PTUNER_CONFIG_TBL_T;

#if 0 //remove to tuner_if.h TDMgr.h
/*************** UTDIS Function Declaration *********************************/
S32 TunerOpen(void);
S32 TunerClose(void);
#if fcADD_ACQ_CHK
BOOL TunerAcq(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                SIZE_T z_conn_info_len, x_break_fct _BreakFct,
                void* pvArg);
#else
void TunerAcq(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                SIZE_T z_conn_info_len, x_break_fct _BreakFct,
                void* pvArg);
#endif

void TunerDisc(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);
void TunerDetachI2C(UINT16 TunerID, BOOL bDetach);
void TunerGetSignal(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal);
INT16 TunerGetSync(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);
U8  TunerGetSignalLevel(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type); // (RegGetAGC2()*100L)/0x3ff
INT16 TunerGetSignalLeveldBm(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);
void TunerSetMpgFmt(UINT16 TunerID, MPEG_FMT_T *pt_mpeg_fmt);
UINT16  TunerGetSignalSNR(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);
UINT16  TunerGetSignalPER(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);
char *TunerGetVer(UINT16 TunerID);
S32 TunerRegRead(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, UCHAR ucRegAddr, UCHAR *pucBuffer, U16 ucByteCount);
void TunerSetRegSetting(UINT16 TunerID, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue);
void TunerShowRegSetting(UINT16 TunerID);
INT32 TunerSetAttribute(UINT16 TunerID,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen);
INT16 TunerGetAttribute(UINT16 TunerID,
                           DRV_GET_TYPE_T  e_get_type,
                           void *pvAnaAttribute,
                           SIZE_T* pzAnaAttributeLen);
INT32 TunerSetDiagCmd(UINT16 TunerID,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size);
INT32 TunerGetDiagInfo(UINT16 TunerID,
                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                        VOID*                           pv_get_info,
                        SIZE_T*                         pz_size);

#endif
///////////////////////////////////////////////////////////////////////////////
// LTDIS function table declaration
///////////////////////////////////////////////////////////////////////////////
typedef S32 (*LTDIS_TunerOpen_fct) (PTD_CTX ptTDCtx,
                                    TUNER_CONFIG_T * ptTunerCfgInfo,
                                    PTD_SPECIFIC_CTX* pptTDSpecificCtx);

typedef S32 (*LTDIS_TunerClose_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

#if fcADD_ACQ_CHK
typedef BOOL (*LTDIS_TunerAcq_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type,
                                    void* pv_conn_info,
                                    SIZE_T z_conn_info_len,
                                    x_break_fct _BreakFct,
                                    void* pvArg);
#else
typedef void (*LTDIS_TunerAcq_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type,
                                    void* pv_conn_info,
                                    SIZE_T z_conn_info_len,
                                    x_break_fct _BreakFct,
                                    void* pvArg);
#endif

typedef void (*LTDIS_TunerDisc_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CONN_TYPE_T e_conn_type);

typedef INT16 (*LTDIS_TunerGetSync_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CONN_TYPE_T e_conn_type);

typedef void (*LTDIS_TunerGetSignal_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                            DRV_CONN_TYPE_T e_conn_type,
                                            SIGNAL *_pSignal);

typedef U8 (*LTDIS_TunerGetSignalLevel_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef INT16 (*LTDIS_TunerGetSignalLeveldBm_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef UINT16 (*LTDIS_TunerGetSignalPER_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef UINT16 (*LTDIS_TunerGetSignalSNR_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef void (*LTDIS_TunerSetMpgFmt_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                            MPEG_FMT_T *pt_mpeg_fmt);

typedef char* (*LTDIS_TunerGetVer_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef S32 (*LTDIS_TunerNimTest_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef void (*LTDIS_TunerCommand_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        INT32 i4Argc,
                                        const CHAR** aszArgv);

typedef void (*LTDIS_TunerDtdCommand_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        INT32 i4Argc,
                                        const CHAR** aszArgv);

typedef void (*LTDIS_TunerAtdCommand_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        INT32 i4Argc,
                                        const CHAR** aszArgv);

typedef void (*LTDIS_TunerSetRegSetting_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                UCHAR ucRegSet,
                                                UCHAR ucRegAddr,
                                                UCHAR ucRegValue);

typedef void (*LTDIS_TunerShowRegSetting_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef void (*LTDIS_TunerDetachI2C_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach);

typedef void (*LTDIS_TunerTestI2C_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef S32 (*LTDIS_TunerRegRead_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CONN_TYPE_T e_conn_type,
                                        UCHAR ucRegAddr,
                                        UCHAR *pucBuffer,
                                        U16 ucByteCount);

typedef INT32 (*LTDIS_TunerSetAttribute_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAnaAttribute,
                                        SIZE_T zAnaAttributeLen);

#ifdef fcADD_DIAG_INFO
typedef INT32(*LTDIS_TunerSetDiagCmd_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                                        VOID*                           pv_set_info,
                                        SIZE_T                          z_size);
typedef INT32(*LTDIS_TunerGetDiagInfo_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                                        VOID*                           pv_get_info,
                                        SIZE_T*                         pz_size);
#endif

typedef INT16 (*LTDIS_TunerGetAttribute_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_GET_TYPE_T  e_get_type,
                                        void *pvAnaAttribute,
                                        SIZE_T* pzAnaAttributeLen);

typedef INT16 (*LTDIS_TunerGetAttribute2_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_GET_TYPE_T  e_get_type,
                                        void *pvAnaAttribute,
                                        SIZE_T* pzAnaAttributeLen,
                                        DRV_CONN_TYPE_T e_conn_type);

typedef void (*LTDIS_TunerBypassI2C_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn);

typedef BOOL (*LTDIS_TunerTunerNotifyEnable_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef struct _LTDIS_FCT_TBL_T
{
    LTDIS_TunerOpen_fct                 pf_TunerOpen;
    LTDIS_TunerClose_fct                pf_TunerClose;
    LTDIS_TunerAcq_fct                  pf_TunerAcq;
    LTDIS_TunerDisc_fct                 pf_TunerDisc;
    LTDIS_TunerGetSync_fct              pf_TunerGerSync;
    LTDIS_TunerGetSignal_fct            pf_TunerGetSignal;
    LTDIS_TunerGetSignalLevel_fct       pf_TunerGetSignalLevel;
    LTDIS_TunerGetSignalLeveldBm_fct    pf_TunerGetSignalLeveldBm;
    LTDIS_TunerGetSignalPER_fct         pf_TunerGetSignalPER;
    LTDIS_TunerGetSignalSNR_fct         pf_TunerGetSignalSNR;
    LTDIS_TunerSetMpgFmt_fct            pf_TunerSetMpgFmt;
    LTDIS_TunerGetVer_fct               pf_TunerGetVer;
    LTDIS_TunerNimTest_fct              pf_TunerNimTest;
    LTDIS_TunerCommand_fct              pf_TunerCommand;
    LTDIS_TunerDtdCommand_fct           pf_TunerDtdCommand;
    LTDIS_TunerAtdCommand_fct           pf_TunerAtdCommand;
    LTDIS_TunerSetRegSetting_fct        pf_TunerSetRegSetting;
    LTDIS_TunerShowRegSetting_fct       pf_TunerShowRegSetting;
    LTDIS_TunerTestI2C_fct              pf_TunerTestI2C;
    LTDIS_TunerRegRead_fct              pf_TunerRegRead;
    LTDIS_TunerDetachI2C_fct            pf_TunerDetachI2C;
    LTDIS_TunerSetAttribute_fct         pf_TunerSetAttribute;
    LTDIS_TunerGetAttribute_fct         pf_TunerGetAttribute;
    LTDIS_TunerBypassI2C_fct            pf_TunerBypassI2C;
    LTDIS_TunerTunerNotifyEnable_fct    pf_TunerNotifyEnable;
#ifdef fcADD_DIAG_INFO
    LTDIS_TunerSetDiagCmd_fct           pf_TunerSetDiagCmd;
    LTDIS_TunerGetDiagInfo_fct          pf_TunerGetDiagInfo;
#endif
    LTDIS_TunerGetAttribute2_fct        pf_TunerGetAttribute2;
} LTDIS_FCT_TBL_T;

//////////////////////////////////////////////////////////////////////////////
typedef struct _LTDIS_TUNERID_OPENFCT_PAIR_T
{
    UINT16                  TunerID;
    LTDIS_TunerOpen_fct     pf_TunerOpen;
} LTDIS_TUNERID_OPENFCT_PAIR_T;

typedef struct _LTDIS_TUNERID_COMPNAMEFORMW_PAIR_T
{
    UINT16              TunerID;
    const char*         cab_dig_name;
    const char*         ter_dig_name;
    const char*         oob_cab_rx_name;
    const char*         ter_ana_name;
    const char*         cab_ana_name;
} LTDIS_TUNERID_COMPNAMEFORMW_PAIR_T;

/************** Function Declaration ****************************************/
INT32 LTDIS_GetOpenFct(UINT16 TunerID, LTDIS_TunerOpen_fct* ppf_TunerOpen);
char* LTDIS_GetCompNameForMW(UINT16 TunerID, DRV_TYPE_T DrvType);
INT32 TDIS_RetrieveTunerCfgTbl(TUNER_CONFIG_TBL_T* ptTunerCfgTbl);

#endif /* _TDIS_H_ */
