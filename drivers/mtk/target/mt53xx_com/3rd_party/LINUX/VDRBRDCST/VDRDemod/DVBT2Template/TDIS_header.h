/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
#ifndef _TDIS_HEADER_FORV_H_
#define _TDIS_HEADER_FORV_H_
#include "u_rm.h"
#include "u_handle.h"
#include "x_tuner.h"
#include "tuner_if.h"

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

//#ifndef tuner_break_fct
//#define tuner_break_fct
//typedef INT32     (*x_break_fct)(void *pArg);
//#endif
/* define in tuner_if.h
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
*/
/*********** Signal structure *************************************/
//Use define in tuner_if.h"
//typedef struct
//{
//    INTERLEAVE_INFO_T  eInDepth;            /* inner interleaver mode   */
//    ALPHA_INFO_T       eAlpha;              /* hierarchical information */
//} HIER_INFO;

//typedef struct
//{
//    INT32       Frequency;                  /* carrier frequency (in Hz)   */
//    UINT32      SymbolRate;                 /* Symbol Rate (in Baud)       */
//    INT32       QAMSize;                    /* QAM size (16/32/64/128/256) */
//    UINT32      e_bandwidth;                /*EU mw: BW_UNKNOWN = 0, BW_6_MHz, BW_7_MHz, BW_8_MHz*/
//    UINT32      e_hierarchy_priority;       /*EU mw: HIR_PRIORITY_UNKNOWN = 0, HIR_PRIORITY_NONE, HIR_PRIORITY_HIGH, HIR_PRIORITY_LOW */
//    HIER_INFO   sHierInfo;                  /*Hierarchical information*/
//} SIGNAL;

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

typedef  BOOL (*LTDIS_TunerGetSync_fct) (PTD_SPECIFIC_CTX ptTDSpecificCtx,
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
//#define DRVAPI_TUNER_OK                 ((INT32) 0)
//#define DRVAPI_TUNER_NOT_INIT           ((INT32) -1)
//#define DRVAPI_TUNER_ALREDAY_INIT       ((INT32) -2)
//#define DRVAPI_TUNER_ERROR              ((INT32) -1024)


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
	HANDLE_T                      *pUniTDCtxLock;
	UINT8                       tunerType;      // to store tuner type for querying
	UINT8						u1MWIntf;       //to store current MWIntface type. 
} TUNER_DEVICE_CTX_T, *PTUNER_DEVICE_CTX_T;

#ifndef cTHREAD_STACK_SIZE
#define cTHREAD_STACK_SIZE      2048
#endif
#ifndef cTHREAD_PRIORITY
#define cTHREAD_PRIORITY        101
#endif

#endif /*_TDIS_HEADER_FORV_H_*/
