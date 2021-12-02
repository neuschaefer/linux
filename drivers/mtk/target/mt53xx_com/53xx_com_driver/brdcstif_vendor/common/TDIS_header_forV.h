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
#ifndef _TDIS_HEADER_FORV_H_
#define _TDIS_HEADER_FORV_H_
#include "u_rm.h"
#include "u_handle.h"
#include "x_tuner.h"

//sync from TDIS.h
typedef void* PTD_CTX;
typedef void* PTD_SPECIFIC_CTX;

typedef struct _TUNER_CONFIG_T{
    UINT16              TunerID;            // MT5111_1, MT5111_2, T313_1 
                                            // LSB represents the TS bus ID
                                            // LSB = 0, TS0
                                            // LSB = 1, TS1
    UINT16              i2TunerCapability;  // TER, CAB, OOB_RX 
    UCHAR               ucDemodInbAddr;     // For Demod Inband
    UCHAR               ucRFTunerAddr;      // For RF Tuner
    UCHAR               ucDemodOobAddr;     // For Demod Oob
    UINT16              ConnExclusiveID;
    UCHAR               Reserved[8];        // For future extension.
                                            // Should all be "0"s.
// Note this structure is also referenced by MW,
// remember to sync with MW if it is modified.
} TUNER_CONFIG_T, *PTUNER_CONFIG_T;

#ifndef tuner_break_fct
#define tuner_break_fct
typedef INT32     (*x_break_fct)(void *pArg);
#endif

typedef enum
{
    ALPHA_0 = 0,
    ALPHA_1,
    ALPHA_2,
    ALPHA_4,
}ALPHA_INFO_T;

typedef enum
{
    NATIVE_INTERLEAVE = 0,
    INDEPTH_INTERLEAVE
}INTERLEAVE_INFO_T;

/*********** Signal structure *************************************/
typedef struct
{
    INTERLEAVE_INFO_T  eInDepth;            /* inner interleaver mode   */
    ALPHA_INFO_T       eAlpha;              /* hierarchical information */
} HIER_INFO;

typedef struct
{
    INT32       Frequency;                  /* carrier frequency (in Hz)   */
    UINT32      SymbolRate;                 /* Symbol Rate (in Baud)       */
    INT32       QAMSize;                    /* QAM size (16/32/64/128/256) */
    UINT32      e_bandwidth;                /*EU mw: BW_UNKNOWN = 0, BW_6_MHz, BW_7_MHz, BW_8_MHz*/
    UINT32      e_hierarchy_priority;       /*EU mw: HIR_PRIORITY_UNKNOWN = 0, HIR_PRIORITY_NONE, HIR_PRIORITY_HIGH, HIR_PRIORITY_LOW */
    HIER_INFO   sHierInfo;                  /*Hierarchical information*/
} SIGNAL;

//SIG_POL_T defied in x_tuner.h
/*
typedef enum
{
    POL_HIGH_ACTIVE,
    POL_LOW_ACTIVE
} SIG_POL_T;
*/
typedef struct
{
    BOOL        fgGatedClock;
    BOOL        fgParallelOutput;
    BOOL        fgGatedOutput;
    BOOL        fgHeader;
    SIG_POL_T   eValidPolarity;
    SIG_POL_T   eSyncPolarity;
    SIG_POL_T   eClockPolarity;
    SIG_POL_T   eErrorPolarity;
} MPEG_FMT_T; //for LTDIS_TunerSetMpgFmt_fct

///////////////////////////////////////////////////////////////////////////////
// LTDIS function table declaration
///////////////////////////////////////////////////////////////////////////////
typedef INT32 (*LTDIS_TunerOpen_fct) (PTD_CTX ptTDCtx,
                                    TUNER_CONFIG_T * ptTunerCfgInfo,
                                    PTD_SPECIFIC_CTX* pptTDSpecificCtx);

typedef INT32 (*LTDIS_TunerClose_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef BOOL (*LTDIS_TunerAcq_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type,
                                    void* pv_conn_info,
                                    SIZE_T z_conn_info_len,
                                    x_break_fct _BreakFct,
                                    void* pvArg);

typedef void (*LTDIS_TunerDisc_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CONN_TYPE_T e_conn_type);

typedef INT16 (*LTDIS_TunerGetSync_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CONN_TYPE_T e_conn_type);

typedef void (*LTDIS_TunerGetSignal_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                            DRV_CONN_TYPE_T e_conn_type,
                                            SIGNAL *_pSignal);

typedef UINT8 (*LTDIS_TunerGetSignalLevel_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef INT16 (*LTDIS_TunerGetSignalLeveldBm_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef UINT32 (*LTDIS_TunerGetSignalPER_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef UINT16 (*LTDIS_TunerGetSignalSNR_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                                DRV_CONN_TYPE_T e_conn_type);

typedef void (*LTDIS_TunerSetMpgFmt_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                            MPEG_FMT_T *pt_mpeg_fmt);

typedef char* (*LTDIS_TunerGetVer_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

typedef INT32 (*LTDIS_TunerNimTest_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx);

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

typedef INT32 (*LTDIS_TunerRegRead_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                        DRV_CONN_TYPE_T e_conn_type,
                                        UCHAR ucRegAddr,
                                        UCHAR *pucBuffer,
                                        UINT16 ucByteCount);

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

typedef void (*LTDIS_TunerBypassI2C_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn);

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
    LTDIS_TunerDtdCommand_fct		      	pf_TunerDtdCommand;
    LTDIS_TunerAtdCommand_fct		      	pf_TunerAtdCommand;
    LTDIS_TunerSetRegSetting_fct        pf_TunerSetRegSetting;
    LTDIS_TunerShowRegSetting_fct       pf_TunerShowRegSetting;
    LTDIS_TunerTestI2C_fct              pf_TunerTestI2C;
    LTDIS_TunerRegRead_fct              pf_TunerRegRead;
    LTDIS_TunerDetachI2C_fct            pf_TunerDetachI2C;
    LTDIS_TunerSetAttribute_fct         pf_TunerSetAttribute;
    LTDIS_TunerGetAttribute_fct         pf_TunerGetAttribute;
    LTDIS_TunerBypassI2C_fct            pf_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    LTDIS_TunerSetDiagCmd_fct          	pf_TunerSetDiagCmd;
    LTDIS_TunerGetDiagInfo_fct          pf_TunerGetDiagInfo;
#endif
} LTDIS_FCT_TBL_T;



//sync from tuner_if.h
#define DRVAPI_TUNER_OK                 ((INT32) 0)
#define DRVAPI_TUNER_NOT_INIT           ((INT32) -1)
#define DRVAPI_TUNER_ALREDAY_INIT       ((INT32) -2)
#define DRVAPI_TUNER_ERROR              ((INT32) -1024)


//sync from TDMgr.h queue.h 
//------------------------------------------------------------------------
// Data Structure Declaration
//------------------------------------------------------------------------
typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _GENERIC_LIST_ENTRY {

    LIST_ENTRY  	Link;
	unsigned int	Count;

} GENERIC_LIST_ENTRY, *PGENERIC_LIST_ENTRY;

typedef	struct	_TUNER_DEVICE_CTX_T
{
	//++Important: Link should be put in the first place of this data structure.
	GENERIC_LIST_ENTRY			Link;
	//--
	
	UINT16					TunerID;
	TUNER_CONFIG_T				tTunerCfg;
	LTDIS_FCT_TBL_T				tLTDIS_Fct_Tbl;
	PTD_SPECIFIC_CTX			pTDSpecificCtx;
	HANDLE_T				RefSema;//sema
	long					RefCnt;
	UCHAR					I2C_Detached;
} TUNER_DEVICE_CTX_T, *PTUNER_DEVICE_CTX_T;



#endif /*_TDIS_HEADER_FORV_H_*/
