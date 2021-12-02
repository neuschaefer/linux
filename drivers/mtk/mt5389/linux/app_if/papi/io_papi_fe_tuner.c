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
/*-----------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $RCSfile: io_papi_fe_tuner.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------*/

/** @file io_papi_fe_tuner.c
 *  PAPI ioctl interface of MT538X tuner driver.
 */

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/

#include "x_linux.h"
#include <linux/papi_ioctl.h>

#include "tuner_if.h"
#include "x_tuner.h"
#include "u_tuner.h"
#include "cb_low.h"
#include "papi_src.h"

#include "pi_isdbt_if.h"
#include "pd_isdbt_if.h"
#include "pd_anaeu_if.h"
#include "tuner_interface_wrapper_if.h"
#include "io_papi_fe_chantbl.h"

#include "x_debug.h"
#include "x_timer.h"
#include "x_timeprofile.h"

#include "aud_if.h"


/***********************************************************************/
/* 				Defines                                                */
/***********************************************************************/
//-----------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------
#define ANALOG_FE				0
#define DIGITAL_FE				1

#define MIN_FREQ_KHZ 			54000
#define MAX_FREQ_KHZ 			864000

#define BRAZIL_CHTAB_BANDWIDTH         6000
#define BRAZIL_CHTAB_VHF_LOW_START    54000
#define BRAZIL_CHTAB_VHF_LOW_END      88000
#define BRAZIL_CHTAB_VHF_HIGH_START  174000
#define BRAZIL_CHTAB_VHF_HIGH_END    216000
#define BRAZIL_CHTAB_UHF_START       470000
#define BRAZIL_CHTAB_UHF_END         806000
#define BRAZIL_CHTAB_VHF_LOW_START_CH_NUM    2
#define BRAZIL_CHTAB_VHF_HIGH_START_CH_NUM   7
#define BRAZIL_CHTAB_UHF_START_CH_NUM        14

#define TV_FINE_TUNE_THRESHOLD_KHZ   4500

#define MIN_ASSM				0
#define MAX_ASSM				100
#define MIN_QSSM				0
#define MAX_QSSM				100

#define TUNER_CMD_QUEUE_SIZE    32
#define TUNER_EVENT_TIMEOUT     (UINT32)(1000)		// In milliseconds

#define PAPI_LayerA  1 
#define PAPI_LayerB  2 
#define PAPI_LayerC  4 

#define mcSLEEP_US(x)          	x_thread_delay((UINT32) ((x) / 1000 + 1))
#define mcDELAY_MS(x)          	mcSLEEP_US((UINT32) (x) * 1000)

#define AUD_STOP                0
#define AUD_PLAY                1

/// Common error code
#define TM_FAIL                 -0x800

//-----------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------

// Digital
typedef struct
{
	tmFe_Constellation_t    Constellation;
	tmFe_IqMode_t           IQMode;
	UINT32                  Threshold;
	tmFe_ChanBandwidth_t    Bandwidth;
	tmFe_CodeRate_t         CodeRate_hp;
	tmFe_CodeRate_t         CodeRate_lp;
	INT32                   TimingOffset;
	INT32                   FreqError;
	INT32                   SymbolError;
	tmFe_GuardInterval_t    GuardInterval;
	tmFe_HierarchyMode_t    Hierarchy;
	tmFe_TxMode_t           TxMode;
	UINT16                  ChannelInfo;
	tmFe_FrequencyOffset_t  FreqOffset;
	UINT32                  Freq;
	tmFe_SearchMode_t       SearchMode;
} SST_SYSTEM_PARAM_INFO_T;

// Analog
typedef struct
{
	UINT32                  Freq;
	tmFe_TvSystem_t         TvSystem;
	tmFe_SearchMode_t       SearchMode;
	Bool                    AfcEnabled;
	UCHAR                   TunerSetSys;
} ANA_PARAM_INFO_T;

// Command Queue
typedef enum
{
	TUNER_CMD_SETFREQ = 0,
	TUNER_CMD_STARTSEARCH, 
	TUNER_CMD_END
} TUNER_CMD_ENUM_T;

typedef struct
{
	TUNER_CMD_ENUM_T		CmdId;
	union
	{
		struct
		{
			INT32	freq;
			BOOL	finetune;
		} SetFreqInfo;
		
		struct
		{
			tmFe_SearchMode_t	SearchMode;
			INT32				endFreq;
		} StartSearchInfo;
		
	} CmdInfo;
} TUNER_CMD_QUEUE_T;


//-----------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------
#define KPRINT(fmt...)		\
	printk("[io_tuner]");	\
	printk(fmt);
//#define mcPRINTF(fmt...)        printk(fmt)
//#define mcPRINT_DBG(fmt...)     mcPRINTF(fmt)
//#define mcPRINT_DBG(fmt...)     LOG_I(fmt)
//#define mcPRINT_ERR(fmt...)     LOG_E(fmt)
#define mcPRINT_DBG(fmt...)     KPRINT(fmt)
#define mcPRINT_ERR(fmt...)     KPRINT(fmt)

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size)                          \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size)))  \
    {                                                           \
        return TM_FAIL;                                         \
    }                                                           \
    if (copy_from_user((dst), (void __user *)(src), (size)))    \
    {                                                           \
        return TM_FAIL;                                         \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size)                            \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    {                                                           \
        return TM_FAIL;                                         \
    }                                                           \
    if (copy_to_user((void __user *)(dst), (src), (size)))      \
    {                                                           \
        return TM_FAIL;                                         \
    }

/***********************************************************************/
/* 				Static variables                                       */
/***********************************************************************/
// Digital
static SST_SYSTEM_PARAM_INFO_T		tMT5133SysParam;
static ISDBT_PD_CTX_T*		        pDtvPdCtx;
static DEMOD_CTX_T*					pDemodCtx;
static RF_TUNER_CTX_T*				pTunerCtx;
static HANDLE_T						t_dtv_semlock;
    // Justin TODO
static DEMOD_CTX_T                  DemodCtxTemp;

// Analog
static ANA_PARAM_INFO_T				rAnaPara;
static ATV_PI_CTX_T*				pAtvPiCtx;
static ATV_PD_CTX_T*                pAtvPdCtx;

static HANDLE_T						t_atv_semlock;

static BOOL							_fe_tun_inited = FALSE;
static BOOL							_digital_mode = TRUE;
static BOOL             			fgMonitorThreadRunning;
static UINT32         				t_monitor_thread;
static BOOL							PreDigLockSts = FALSE;
static HANDLE_T 					_hTunerCmdQueue;
static HANDLE_T                     t_atv_TunerQueue_lock = NULL_HANDLE;
static UINT8                        tunerQueueCmdCount = 0;
static UINT8						lastMd = 0;
static UINT8						lastCr = 1;
static UINT8						lastCons = 2;
static UINT8						lastHier =0;
static INT8							lastFreqBias = 0;
static BOOL							_fgStopSearch = FALSE;

static BOOL							_fgPreDetachMWSts = FALSE;
static BOOL							_IsIoctl = FALSE;
static BOOL							_IsLockForIoctl = FALSE;

UINT32 u4FeDivMaxFreq = MAX_FREQ_KHZ;
UINT32 u4FeDivMinFreq = MIN_FREQ_KHZ;

// Channel Table
static PIO_PAPI_FE_CHANTBL_PAIR_T   currChanTblPair = NULL; // default = NULL

/***********************************************************************/
/* 				Interfile variables                                    */
/***********************************************************************/
EXTERN BOOL 						_fgAutoSearch;//Yulia 2005.10.24
extern BOOL              			bDetachMW;

EXTERN void vDTVMute(UINT8 bPath,UINT8 bOnOff);
EXTERN  BOOL  VA1G5BF_AnaTunerSetFreq(ATV_PI_CTX_T *psAtvPiCtx, UINT32 wFreqInKHz, UINT8 u1AnaMode);
EXTERN UINT8 VA1G5BF_bDrvTunerCheckVIFLock(ATV_PI_CTX_T *psAtvPiCtx, UINT32 wFreqInKHz);
EXTERN BOOL fgDrvTunerCheckTVDLock(UINT8 bTryLoopCnt);
EXTERN UINT8 AnalogPINA_TunerSearchNearbyFreq(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
EXTERN UINT8 AnalogPINA_TunerMonitor(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, VOID *pvArg);
EXTERN BOOL ISDBT_FEInitialize();//FE initialization,please refer HSI
EXTERN BOOL ISDBT_IsDemodSleep();//FE initialization,please refer HSI
/***********************************************************************/
/* 				Static Functions                                       */
/***********************************************************************/

/* Function Declarations */
static tmFe_TvSystem_t _AnalogDetectTvSystem(void);

/* Function Definitions */
static void _SEMA_LOCK(void)
{
	x_sema_lock(t_dtv_semlock, X_SEMA_OPTION_WAIT);
	x_sema_lock(t_atv_semlock, X_SEMA_OPTION_WAIT);
	//KPRINT("_SEMA_LOCK\n");
	if (_IsIoctl){
		_IsLockForIoctl = TRUE;
	}
}

static void _SEMA_UNLOCK(void)
{
	//KPRINT("_SEMA_UNLOCK\n");
	x_sema_unlock(t_dtv_semlock);
	x_sema_unlock(t_atv_semlock);
	if (_IsIoctl){
		_IsLockForIoctl = FALSE;		
	}
}

static void _SleepMs_Interruptible(UINT32 TimeMs)
{
	_SEMA_UNLOCK();
	mcDELAY_MS(TimeMs);
	_SEMA_LOCK();	
}

static INT32 _AnalogSearchBreak(void* pArg)
{
    return (_fgStopSearch);
}

static INT32 _papi_fe_tuner_SetFreq(INT32 u4Freq, BOOL fgFinetune)
{
    // Send MsgQ to thread
    TUNER_CMD_QUEUE_T 	rCmdQ;

    _fgStopSearch = TRUE;

    rCmdQ.CmdId = TUNER_CMD_SETFREQ;
    rCmdQ.CmdInfo.SetFreqInfo.freq = u4Freq;
    rCmdQ.CmdInfo.SetFreqInfo.finetune = fgFinetune;

    x_sema_lock(t_atv_TunerQueue_lock, X_SEMA_OPTION_WAIT);
    if ((x_msg_q_send(_hTunerCmdQueue, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
        KPRINT("Error: x_msg_q_send failed\n");
    }
    else
    {
        tunerQueueCmdCount ++;
        KPRINT("++tunerQueueCmdCount (SetFreq)\n");
    }
    x_sema_unlock(t_atv_TunerQueue_lock);
	
    return 0;
}


static Nat32 ChtabFreq2Ch(Nat32 u4FreqInKhz)
{
    Nat32 retValue = 0;  

    if ((u4FreqInKhz >= BRAZIL_CHTAB_VHF_LOW_START) && (u4FreqInKhz <= BRAZIL_CHTAB_VHF_LOW_END))
    {
        retValue = BRAZIL_CHTAB_VHF_LOW_START_CH_NUM + 
            (u4FreqInKhz - BRAZIL_CHTAB_VHF_LOW_START)/BRAZIL_CHTAB_BANDWIDTH;
    }
    else if ((u4FreqInKhz >= BRAZIL_CHTAB_VHF_HIGH_START) && (u4FreqInKhz <= BRAZIL_CHTAB_VHF_HIGH_END)) {
        retValue = BRAZIL_CHTAB_VHF_HIGH_START_CH_NUM + 
            (u4FreqInKhz - BRAZIL_CHTAB_VHF_HIGH_START)/BRAZIL_CHTAB_BANDWIDTH;
    }
    else if ((u4FreqInKhz >= BRAZIL_CHTAB_UHF_START) && (u4FreqInKhz <= BRAZIL_CHTAB_UHF_END)) {
        retValue = BRAZIL_CHTAB_UHF_START_CH_NUM + 
            (u4FreqInKhz - BRAZIL_CHTAB_UHF_START)/BRAZIL_CHTAB_BANDWIDTH;
    }

    KPRINT("Freq = %dKHz => ChNum = %d\n", u4FreqInKhz, retValue); 
    return retValue;
}

static Nat32 ChtabCh2Freq(Nat32 u4ChanNum)
{
    Nat32 u4Freq;

    if (u4ChanNum < BRAZIL_CHTAB_VHF_LOW_START_CH_NUM ) {
        // out of band
        u4Freq = 0;
    }
    else{ 
        if (u4ChanNum < BRAZIL_CHTAB_VHF_HIGH_START_CH_NUM) {
            // in VHF LOW
            u4Freq = (u4ChanNum - BRAZIL_CHTAB_VHF_LOW_START_CH_NUM)*BRAZIL_CHTAB_BANDWIDTH
                + BRAZIL_CHTAB_VHF_LOW_START;
        }
        else if (u4ChanNum < BRAZIL_CHTAB_UHF_START_CH_NUM) {
            // in VHF HIGH
            u4Freq = (u4ChanNum - BRAZIL_CHTAB_VHF_HIGH_START_CH_NUM)*BRAZIL_CHTAB_BANDWIDTH
                + BRAZIL_CHTAB_VHF_HIGH_START;
        }
        else {
            // in UHF
            u4Freq = (u4ChanNum - BRAZIL_CHTAB_UHF_START_CH_NUM)*BRAZIL_CHTAB_BANDWIDTH
                + BRAZIL_CHTAB_UHF_START;
        }
        // change to center freq
        u4Freq += BRAZIL_CHTAB_BANDWIDTH/2;
        
        if ((u4Freq > u4FeDivMaxFreq) || (u4Freq > BRAZIL_CHTAB_UHF_END)) {
            u4Freq = 0;  // out of band
        }        
        else {
            if (!_digital_mode) {
                u4Freq -= 1750; //convert to video freq in analog TV
            }
        }
    }

    KPRINT("ChNum = %d => Freq = %dKHz\n", u4ChanNum, u4Freq); 
    return u4Freq;
}

static void _IndicateCarrierPresentChgIfNecessary(Bool CarrierPresent)
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;
        Nat32		designator;
        int 		source;

        _papi_src_src_GetCurrentVideoSelection(&source, &designator);
	
	if (CarrierPresent){
		if (!PreDigLockSts){	
			PreDigLockSts = TRUE;

                        if (source == papi_src_srcid_SrcTuner &&
			    designator==papi_src_dsgid_DesignatorDigital) 
                        {
       		            // UnMute video if unlock
       	                    vDTVMute(0, 0);	
                        }
	
			
			KPRINT("DigLockSts change:  Unlock --> Lock\n");
			rCbInfo.CbId = PAPI_FE_CALLBACK_CARRIER_PRESENT;
			rCbInfo.CbIdInfo.CarrierPresent = TRUE;
			_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
		}
	}
	else{
		if (PreDigLockSts){
			PreDigLockSts = FALSE;

                        if (source == papi_src_srcid_SrcTuner &&
			    designator==papi_src_dsgid_DesignatorDigital) 
                        {
       		            // Mute video if unlock
       	                    vDTVMute(0, 1);		
                        }
			
			KPRINT("DigLockSts change:  Lock --> Unlock\n");
			rCbInfo.CbId = PAPI_FE_CALLBACK_CARRIER_PRESENT;
			rCbInfo.CbIdInfo.CarrierPresent = FALSE;
			_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
		}
	}	
}
static void _AnalogAcq(Nat32 Freq){

    //rAnaPara.Freq = Freq*62500/1000;
    rAnaPara.Freq = Freq;
    VA1G5BF_AnaTunerSetFreq(pAtvPiCtx, rAnaPara.Freq, 0);
    KPRINT("_AnalogAcq(), Freq = %dKHz\n", Freq); 

#if 0//TODO:PI
	MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
	AnalogPI_TunerSetFreq(pAtvPiCtx, rAnaPara.Freq, MOD_ANA_CABLE);
	MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);
#endif//TODO:PI
}							

static Bool _AnaGetStationPresent(void){
	UINT8   u1Value, u2Value;     
	Bool	retSts;

    // return lock if VIF = 2
    if (VA1G5BF_bDrvTunerCheckVIFLock(pAtvPiCtx, rAnaPara.Freq) == 2) {
        retSts = TRUE;
    }
    else {
        retSts = FALSE;
    }

    if(retSts && !fgDrvTunerCheckTVDLock(0x30))
    {
       retSts = FALSE;
    }    

#if 0//TODO:PI
    MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
	u2Value = SIF_Read_Only(0xf00, 0x86, &u1Value, 1);
    MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);
             
    //KPRINT("AFC value = 0x%02X\n", u1Value); 
           
    if(u2Value == 0)
	{
    	retSts = FALSE;
	}
	else if((u1Value & 0x80)==0x00) //AFCWIN unlock
    {
        retSts = FALSE;
    }
	else if( ((u1Value & 0x80)==0x80) && ((u1Value & 0x10)==0x00) )  // Corrected by RK
	{
    	if( ((u1Value & 0x0e) >> 1) <= 0x02 )
        	retSts = TRUE;
        else
        	retSts = FALSE;
    }
    else if( ((u1Value & 0x80)==0x80) && ((u1Value & 0x10)==0x10) ) // Corrected by RK
	{
        if( (0x07 - ((u1Value & 0x0e) >> 1)) <= 0x02 )
        	retSts = TRUE;
        else
            retSts = FALSE;
    }
    else
    {    
        retSts = FALSE;
	}
#endif//TODO:PI
	return (retSts);
}

static int	_AnalogGetAfc(void)
{    
	UINT8   u1Value, u2Value;     
	int		AFCValue;
#if 0//TODO:PI
    MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
    u2Value = SIF_Read_Only(0xf00, 0x86, &u1Value, 1);
    MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);
    
    //KPRINT("AFC value = 0x%02X\n", u1Value); 

    if(u2Value == 0)
    {
       AFCValue = tmFe_FreqUnknown;
    }
    else if((u1Value & 0x80)==0x00) //AFCWIN unlock
    {
       AFCValue = tmFe_FreqUnknown;
    }
    else if( ((u1Value & 0x10)==0x10) && ((u1Value & 0x80)==0x80) ) //AFCWIN lock, AFC4=1 (above IF)
    {
        if((u1Value & 0x0e)==0x00)
            AFCValue = tmFe_FreqFarAboveIf;
        else
            AFCValue = tmFe_FreqAboveIf;
    }
    else if( ((u1Value & 0x10)==0x00) && ((u1Value & 0x80)==0x80) )
    {
        if((u1Value & 0x0e)==0x0e)
           AFCValue = tmFe_FreqFarBelowIf;
        else
           AFCValue = tmFe_FreqBelowIf;
    }
    else
    {
       AFCValue = tmFe_FreqUnknown;
    }
#endif//TODO:PI
    return    AFCValue;
}

static void _NotifyStationPresent(Bool Lock)
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;

	KPRINT("_NotifyStationPresent: Lock ?  %d\n", Lock);
	rCbInfo.CbId = PAPI_FE_CALLBACK_STATION_PRESENT;
	rCbInfo.CbIdInfo.StationPresent = Lock;
	_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
}


static void _NotifySearchStateChanged(tmFe_SearchState_t state)
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;

	KPRINT("_NotifySearchStateChanged: State?  %d\n", state);
	rCbInfo.CbId = PAPI_FE_CALLBACK_SEARCH_STATE_CHANGED;
	rCbInfo.CbIdInfo.SearchState = state;
	_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
}	        			

static void _NotifySearchInProgress(tmFe_SearchState_t state)
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;

	KPRINT("_NotifySearchInProgress: State?  %d\n", state);
	rCbInfo.CbId = PAPI_FE_CALLBACK_SEARCH_IN_PROGRESS;
	rCbInfo.CbIdInfo.SearchState = state;
	_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
}	        			

static void _NotifyTvSysDetected(tmFe_TvSystem_t tvsys)
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;

	KPRINT("_NotifyTvSysDetected: tvsys?  %d\n", tvsys);
	rCbInfo.CbId = PAPI_FE_CALLBACK_TVSYSTEM_DETECTED;
	rCbInfo.CbIdInfo.TvSystem = tvsys;
	_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
}

static void _NotifyAfcFreqChanged(INT32 freq)
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;

	KPRINT("_NotifyAfcFreqChanged: current frequency = %d\n", freq);
	rCbInfo.CbId = PAPI_FE_CALLBACK_AFC_FREQ_CHANGED;
	rCbInfo.CbIdInfo.CurrentFreq = freq;
	_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
}

static void _NotifyAfcLimitReached(INT32 freq)
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;

	KPRINT("_NotifyAfcLimitReached: current frequency = %d\n", freq);
	rCbInfo.CbId = PAPI_FE_CALLBACK_AFC_LIMIT_REACHED;
	rCbInfo.CbIdInfo.CurrentFreq = freq;
	_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
}

static void _NotifyAfcModeChanged()
{
	PAPI_FE_CALLBACK_INFO_T		rCbInfo;

	KPRINT("_NotifyAfcModeChanged: AFC temporarily disable\n");
	rCbInfo.CbId = PAPI_FE_CALLBACK_AFC_MODE_CHANGED;
	_CB_PutEvent(CB_PAPI_TUNER_TRIGGER, sizeof(PAPI_FE_CALLBACK_INFO_T), &rCbInfo);
}

static void _AnalogTunerSetSys(UCHAR	sys)
{
	rAnaPara.TunerSetSys = sys;
#if 0//TODO:PI
	MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
	AnalogPI_TunerSetSystem(pAtvPiCtx, sys-1);	// -1 is necessary
	MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);	
#endif//TODO:PI
}

static void _AnalogSearchNearbyAccordingSearchMode(INT32 startFreqKHz, INT32 endFreqKHz)
{
    UINT8 status = 0;
	rAnaPara.Freq = startFreqKHz;
	
	KPRINT("_AnalogSearchNearbyAccordingSearchMode(start=%dKHz, end=%dKHz)\n", startFreqKHz, endFreqKHz);
#if 0//TODO:PI
	if (rAnaPara.SearchMode&tmFe_SmPositiveModulation){ // Positive modulation
		if (startFreqKHz<100000 && endFreqKHz<100000){
			_AnalogTunerSetSys(MOD_SECAM_L1);
			MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
			AnalogPI_TunerSearchNearbyFreq(pAtvPiCtx, eTUNER_UNI_AUTO_SCAN, &rAnaPara.Freq, _AnalogSearchBreak, NULL, 
					endFreqKHz);
			MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);	
		}
		else if (startFreqKHz>=100000 && endFreqKHz>=100000){
			_AnalogTunerSetSys(MOD_SECAM_L);
			MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
			AnalogPI_TunerSearchNearbyFreq(pAtvPiCtx, eTUNER_UNI_AUTO_SCAN, &rAnaPara.Freq, _AnalogSearchBreak, NULL, 
					endFreqKHz);
			MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);	
		}
		else if (startFreqKHz<100000 && endFreqKHz>100000){
			_AnalogTunerSetSys(MOD_SECAM_L1);
			MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
			AnalogPI_TunerSearchNearbyFreq(pAtvPiCtx, eTUNER_UNI_AUTO_SCAN, &rAnaPara.Freq, _AnalogSearchBreak, NULL, 
					99999);
			MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);	
			
			if (!_AnaGetStationPresent()){
				_AnalogTunerSetSys(MOD_SECAM_L);
				rAnaPara.Freq = 100000;
				MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
				AnalogPI_TunerSearchNearbyFreq(pAtvPiCtx, eTUNER_UNI_AUTO_SCAN, &rAnaPara.Freq, _AnalogSearchBreak, NULL, 
						endFreqKHz);
				MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);					
			}
		}
		else{
			KPRINT("ERROR: Unknown range\n");
		}
	}
	else{ // Negative modulation
		_AnalogTunerSetSys(MOD_PAL_BG);
		MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
		status = AnalogPI_TunerSearchNearbyFreq(pAtvPiCtx, eTUNER_UNI_AUTO_SCAN, &rAnaPara.Freq, _AnalogSearchBreak, NULL, 
				endFreqKHz);
		if (status)
		{
		    KPRINT("VIF lock! (%d KHz)\n", rAnaPara.Freq);
		}
		MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);	
	}
#endif//TODO:PI
}			

static Bool _AnalogSearchNearbyWithNotifyInProgress(INT32 startFreqKHz, INT32 endFreqKHz, BOOL *bInterrupt)
{
	INT32		nextFreq;   // KHz
	Bool		Lock = FALSE;
    UINT16 		u2MsgIdx;
    SIZE_T 		zMsgSize = sizeof(UINT32);
    UINT32      u4ChNum;
	
	///+++
    // convert to nominal freq 
   // u4ChNum = ChtabFreq2Ch(startFreqKHz);
	//rAnaPara.Freq = ChtabCh2Freq(u4ChNum);
      /* modify for startsearch() */
      rAnaPara.Freq = startFreqKHz;//fix CR  [DTV00054767] 

	while ((rAnaPara.Freq<=endFreqKHz) && (rAnaPara.Freq!=0)){
	    Lock = FALSE;

        if (_fgStopSearch){
            break;
        }

        //++ Interrupt current cmd if further cmd received
        if( tunerQueueCmdCount > 0 )
        {
            *bInterrupt = TRUE;
            KPRINT("Skipping _AnalogSearchNearbyWithNotifyInProgress Command\n");
            break;
        }
        //--

        _NotifySearchInProgress(tmFe_SsSearching);

       // _AnalogAcq(rAnaPara.Freq);
        KPRINT("Tuner current freq search at %d kHz\n", rAnaPara.Freq);
        // Release sema awhile to let other ioctl has opportunity in.
        _SleepMs_Interruptible(100);

         //  Lock = _AnaGetStationPresent();
       // KPRINT("SearchNearby at %d kHz\n",rAnaPara.Freq); 
 if(3==AnalogPINA_TunerSearchNearbyFreq(pAtvPiCtx, &(rAnaPara.Freq),_AnalogSearchBreak, pAtvPdCtx)){          
 	Lock=TRUE; 
 }
        if (Lock) {
            KPRINT("TVD Lock, Freq = %dkHz\n", rAnaPara.Freq);
            //Check TV system if VIF is locked in analog mode
            if (tmFe_TvSystemNone == _AnalogDetectTvSystem())
                Lock = FALSE;
        }

        if (Lock)
            break;

        rAnaPara.Freq += BRAZIL_CHTAB_BANDWIDTH;//fix CR  [DTV00054767] 
        //u4ChNum++;
       // rAnaPara.Freq = ChtabCh2Freq(u4ChNum);
        /* modify for startsearch() */
      //  rAnaPara.Freq += 62; /* 62(.5)KHz for one-step */
		
#if 0//TODO:PI
        nextFreq = rAnaPara.Freq + 1000;
        /*! Set tuner with the starting frequency after VIF lock.
         *  For convenience and safty, do it everytime before _AnalogSearchNearbyAccordingSearchMode()
         */
        //++
    	MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
    	AnalogPI_TunerSetFreq(pAtvPiCtx, rAnaPara.Freq, MOD_ANA_CABLE);
    	MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);
    	//--

		if (rAnaPara.Freq < endFreqKHz){
			_NotifySearchInProgress(tmFe_SsSearching);

			//_AnalogSearchNearbyAccordingSearchMode(rAnaPara.Freq, endFreqKHz);
			_AnalogSearchNearbyAccordingSearchMode(rAnaPara.Freq, nextFreq);

			Lock = _AnaGetStationPresent();
			if (Lock)
			{
                //Check TV system if VIF is locked in analog mode
                if (tmFe_TvSystemNone == _AnalogDetectTvSystem())
                    Lock = FALSE;
			}
			
			if (Lock)
				break;
		}
		else{
			_NotifySearchInProgress(tmFe_SsSearching);
			
			//_AnalogSearchNearbyAccordingSearchMode(rAnaPara.Freq, endFreqKHz);
			_AnalogSearchNearbyAccordingSearchMode(rAnaPara.Freq, nextFreq);

			Lock = _AnaGetStationPresent();
			if (Lock)
			{
                //Check TV system if VIF is locked in analog mode
                if (tmFe_TvSystemNone == _AnalogDetectTvSystem())
                    Lock = FALSE;
			}
			break;
		}	
		rAnaPara.Freq += 1000;
#endif//TODO:PI			
		// Release sema awhile to let other ioctl has opportunity in.
		//_SleepMs_Interruptible(20);
	}
	///---
    /// 
    if (!Lock)
    {
// if finally no ch found and ends, freq still update to next freq => roll back
//        tMT5133SysParam.Freq -= 500;
        //u4ChNum--;
        //rAnaPara.Freq = ChtabCh2Freq(u4ChNum);

        // set the freq at the end of search to endFreqKHz
        tMT5133SysParam.Freq = endFreqKHz;
    }

	return Lock;
}

static Bool _DigitalGetCarrierPresent(void)
{
    Bool status = 0;

    status = ISDBT_GetSync(pDemodCtx);
//    KPRINT("[FE]ISDBT_GetSync() = %d\n", status);

#if 0//TODO:PI
	MT5133_UpdateInfo(pDemodCtx);
	status = MT5133_GetTsLock(pDemodCtx));
#endif//TODO:PI
    return status;
}

static void _DigitalAcq(Nat32 Freq){

//    Freq = Freq*62500/1000; // Translate Freq from 1/16MHz to KHz

    tMT5133SysParam.Freq = Freq;
    KPRINT("_DigitalAcq(), Freq = %dKHz\n", Freq); 

    // set freq
    // Justin TODO
    if (IWrapper_Tuner_SetFreq(pTunerCtx, NULL, tMT5133SysParam.Freq, 0, 0, 0)){
        KPRINT(("Failed on ISDB-T Tuner_SetFreq(%d) \n", tMT5133SysParam.Freq));
    }

/*
    pTCtx->dtdTunerCtx = pTunerCtx;
    if (VA1G5BF_TunerSetFreq(pTCtx, tMT5133SysParam.Freq, 0, 0, 0)) {
        KPRINT(("Failed on ISDB-T Tuner_SetFreq(%d) \n", tMT5133SysParam.Freq));
    }
*/
    // set Demod
    if (ISDBT_Connect(pDemodCtx)){
        KPRINT(("Failed on ISDBT_Connect()!! \n"));
    }
#if 0// for DVB-T only
	switch(tMT5133SysParam.Bandwidth)
	{
		case tmFe_ChanBandwidth6MHz:
			DTOS40A_SetSawBw(pTunerCtx, SAW_BW_7M);
			break;
		case tmFe_ChanBandwidth7MHz:
			DTOS40A_SetSawBw(pTunerCtx, SAW_BW_7M);
			break;
		case tmFe_ChanBandwidth8MHz:
			DTOS40A_SetSawBw(pTunerCtx, SAW_BW_8M);
			break;
		default:
			DTOS40A_SetSawBw(pTunerCtx, SAW_BW_8M);
			break;
	}
	MT5133_NormalMode(DEFAULT_I2C_ADDR);        
	MT5133_FSM_Stop(pDemodCtx);
	MT5133_TunerI2c(pDemodCtx, TRUE);
	DTOS40A_SetFreq(pTunerCtx, Freq, MOD_DVBT, FALSE);
	MT5133_TunerI2c(pDemodCtx, FALSE);
	
	MT5133_SetSearchParam(pDemodCtx, CH_SEARCH_SCAN, lastMd, lastCr, lastCons, lastHier, lastFreqBias);

	switch(tMT5133SysParam.Bandwidth)
	{
		case tmFe_ChanBandwidth6MHz:
			MT5133_SetBW(pDemodCtx, MT5133_BW_6MHz);
			break;
		case tmFe_ChanBandwidth7MHz:
			MT5133_SetBW(pDemodCtx, MT5133_BW_7MHz);
			break;
		case tmFe_ChanBandwidth8MHz:
			MT5133_SetBW(pDemodCtx, MT5133_BW_8MHz);
			break;
		default:
			MT5133_SetBW(pDemodCtx, MT5133_BW_8MHz);
			break;
	}
					
	MT5133_FSM_Start(pDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);				
#endif//TODO:PI
}							


static Bool _DigitalSearchNearbyWithNotifyInProgress(INT32 startFreqKHz, INT32 endFreqKHz, tmFe_SearchState_t SearchMode, BOOL *bInterrupt)
{
    INT32		preFreq;
    //Bool		dvbtCh_detected = FALSE;
    BOOL		isdbtCh_detected = FALSE;
    HAL_TIME_T 	rTime1;
    HAL_TIME_T 	rTime2;
    HAL_TIME_T	rDiffTime;
    UINT32      u4ChNum;

    // convert to nominal freq 
    u4ChNum = ChtabFreq2Ch(startFreqKHz);
	tMT5133SysParam.Freq = ChtabCh2Freq(u4ChNum);
    preFreq = tMT5133SysParam.Freq;
	
    while((tMT5133SysParam.Freq<=endFreqKHz) && (tMT5133SysParam.Freq!=0))
    {
        if (_fgStopSearch){
            break;
        }

        //++ Interrupt current cmd if further cmd received
        if( tunerQueueCmdCount > 0 )
        {
            *bInterrupt = TRUE;
            KPRINT("Skipping _DigitalSearchNearbyWithNotifyInProgress Command\n");
            break;
        }
        //--

		if ((tMT5133SysParam.Freq-preFreq)>=1000){
			_NotifySearchInProgress(tmFe_SsSearching);
			preFreq = tMT5133SysParam.Freq;
		}

        // connect to a channel
        _DigitalAcq(tMT5133SysParam.Freq);		
		KPRINT("Tuner current freq search at %d \n", tMT5133SysParam.Freq);
		
        // wait lock 
        HAL_GetTime(&rTime1);
        isdbtCh_detected = FALSE;
        while(1)
        {
            if (_DigitalGetCarrierPresent()){
                isdbtCh_detected = TRUE;
                break;
            }

            HAL_GetTime(&rTime2);
            HAL_GetDeltaTime(&rDiffTime, &rTime1, &rTime2);
            if (rDiffTime.u4Seconds >= 1){
                break;
            }
		
            _SleepMs_Interruptible(10);			
        }		

        if (isdbtCh_detected)
        {   // it could be wrong in wrap around case, change the solution to adding BW at the beginning 
            /*
            if ((tMT5133SysParam.Freq == startFreqKHz) && (!(SearchMode&tmFe_SmCurrentIncluded)))
            {
                isdbtCh_detected = FALSE;
                //tMT5133SysParam.Freq += BRAZIL_CHTAB_BANDWIDTH; // min BW=5M, step at least 2.5M*2
                u4ChNum++;
                tMT5133SysParam.Freq = ChtabCh2Freq(u4ChNum);
            }
            else
                */
            {
                KPRINT("Tuner search freq locked at freq = %d\n", tMT5133SysParam.Freq);
                break;
            }
        }
        else
        {
            //tMT5133SysParam.Freq += BRAZIL_CHTAB_BANDWIDTH;
            u4ChNum++;
            tMT5133SysParam.Freq = ChtabCh2Freq(u4ChNum);
        
        }
    }

    //! temporary solution
	if (!isdbtCh_detected)
    {
// if finally no ch found and ends, freq still update to next freq => roll back
//        tMT5133SysParam.Freq -= 500;
// 
        //u4ChNum--;
        //tMT5133SysParam.Freq = ChtabCh2Freq(u4ChNum);
        
        // set the freq at the end of search to endFreqKHz
        tMT5133SysParam.Freq = endFreqKHz;
    }

	return isdbtCh_detected;

// for DVB-T only
/*
#if 0//TODO:PI
		MT5133_FSM_Stop(pDemodCtx);
		MT5133_TunerI2c(pDemodCtx, TRUE);
		DTOS40A_SetFreq(pTunerCtx, tMT5133SysParam.Freq, MOD_DVBT, FALSE);
		MT5133_TunerI2c(pDemodCtx, FALSE);
		MT5133_DemodReset(pDemodCtx);
		MT5133_FSM_Start(pDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);
		
		KPRINT("Tuner current freq search at %d \n", tMT5133SysParam.Freq);
		
		dvbtCh_detected = FALSE;

		HAL_GetTime(&rTime1);
		while(1)
		{
			//update demod status before MT5133_GetDvbtNotExist(p) & MT5133_GetTpsLock(p)
			MT5133_GetStatus(pDemodCtx);	
		
			if (MT5133_GetDvbtNotExist(pDemodCtx))
			{	
				//No DVB-T signal exists in this channel. Early break and go to next channel.
				break;
			}
			else if (MT5133_GetTsLock(pDemodCtx))
			{
				dvbtCh_detected = TRUE;
				break;						
			}

			HAL_GetTime(&rTime2);
			HAL_GetDeltaTime(&rDiffTime, &rTime1, &rTime2);
			if ((rDiffTime.u4Seconds>=1)||(rDiffTime.u4Micros>500000)){
				break;
			}
		
			_SleepMs_Interruptible(5);
			
		}		
		if (dvbtCh_detected)
		{
			if (tMT5133SysParam.Freq == startFreqKHz)
			{
				if (!(SearchMode&tmFe_SmCurrentIncluded))
				{
					dvbtCh_detected = FALSE;
					tMT5133SysParam.Freq += 5000; // min BW=5M, step at least 2.5M*2
				}
				else
				{
					KPRINT("Tuner search freq locked at freq = %d\n", tMT5133SysParam.Freq);
					break;
				}									
			}
			else
			{
				KPRINT("Tuner search freq locked at freq = %d\n", tMT5133SysParam.Freq);
				break;
			}
		}
		else
		{
			tMT5133SysParam.Freq += 500;
		}
#endif//TODO:PI

		if (_fgStopSearch){
			break;
		}
	}
*/
}

static void _AudioFlowControl(BOOL fgPlay, BOOL fgDigital)
{
#if 0//TODO:PI
    if (fgPlay)
    {
        if (fgDigital)  //DTV
        {
            printk("\n===== DTV Audio PLAY ========\n");
            AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_MPEG);                     
            AUD_DSPCmdDualDecPlay(AUD_DEC_MAIN);            
        }
        else            //ATV
        {
            printk("\n===== ATV Audio PLAY ========\n");
            AUD_AtunerSigStableNotify(TRUE);
            AUD_TriggerDspPlay(AUD_DEC_AUX);
            AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_PAL);                     
            AUD_DSPCmdDualDecPlay(AUD_DEC_MAIN);
        }
    }
    else
    {
            printk("\n===== TV Audio STOP ========\n");
            AUD_DSPCmdDualDecStop(AUD_DEC_MAIN);
            AUD_WaitDspFlush(AUD_DEC_AUX);
    }
#endif//TODO:PI
}

static void _AnalogSetAudioSysMask()
{
	UINT32 				u4Mask;
#if 0//TODO:PI
	switch(rAnaPara.TunerSetSys){
		case MOD_PAL_BG:
			u4Mask = AUD_TV_SYS_MASK_B;
			break;
		case MOD_PAL_DK:
			u4Mask = AUD_TV_SYS_MASK_D;
			break;
		case MOD_PAL_I:
			u4Mask = AUD_TV_SYS_MASK_I;
			break;
		case MOD_SECAM_L:
			u4Mask = AUD_TV_SYS_MASK_L;
			break;
		case MOD_SECAM_L1:
			u4Mask = AUD_TV_SYS_MASK_L_PRIME;
			break;
		default:
			u4Mask = AUD_TV_SYS_MASK_B;
			break;
	}
	Aud_SetTvSysMask(0, u4Mask);
#endif//TODO:PI
}

static tmFe_TvSystem_t _AnalogInvokeAudioToCheckTvSystem()
{
	tmFe_TvSystem_t		retSys;
#if 0//TODO:PI
	AUD_FMT_T 			eAudFmt;
	TV_AUD_SYS_T 		eTvSys;

	_AnalogSetAudioSysMask();
	 _AudioFlowControl(0, 0)  ;
	AUD_DrvATVFmtDetectSync(&eAudFmt, &eTvSys);
	switch(eTvSys){
		case SV_NONE_DETECTED:
		case SV_MTS:
		case SV_FM_FM:
		case SV_NTSC_M:
		case SV_A2_DK3:
			retSys = tmFe_TvSystemNone;
			break;
		case SV_A2_BG:
		case SV_PAL_BG:
		case SV_SECAM_BG:
			retSys = tmFe_TvSystemBg;
			break;
		case SV_A2_DK1:
			retSys = tmFe_TvSystemDk1;
			break;
		case SV_A2_DK2:
			retSys = tmFe_TvSystemDk2;
			break;
		case SV_PAL_I:
			retSys = tmFe_TvSystemI;
			break;
		case SV_PAL_DK:
		case SV_SECAM_DK:
			retSys = tmFe_TvSystemDk;
			break;
		case SV_SECAM_L:
			retSys = tmFe_TvSystemL;
			break;
		case SV_SECAM_L_PLUM:
			retSys = tmFe_TvSystemL1;
			break;
		default:
			retSys = tmFe_TvSystemNone;
			break;
	}
#endif//TODO:PI
	return retSys;
}

static tmFe_TvSystem_t _AnalogDetectTvSystem(void)
{
	tmFe_TvSystem_t 	TvSys;
#if 0//TODO:PI
	if (fgDrvTunerCheckTVDLock(0x30)){
		if (rAnaPara.TunerSetSys == MOD_PAL_BG){	// BG
			// Check BG
			_AnalogTunerSetSys(MOD_PAL_BG);
			TvSys = _AnalogInvokeAudioToCheckTvSystem();
			if (TvSys != tmFe_TvSystemNone){
				return TvSys;
			}
			// Check DK
			_AnalogTunerSetSys(MOD_PAL_DK);
			TvSys = _AnalogInvokeAudioToCheckTvSystem();
			if (TvSys != tmFe_TvSystemNone){
				return TvSys;
			}
			// Check I
			_AnalogTunerSetSys(MOD_PAL_I);
			TvSys = _AnalogInvokeAudioToCheckTvSystem();
			if (TvSys != tmFe_TvSystemNone){
				return TvSys;
			}
			return tmFe_TvSystemNone;
		}
		else{	// L or L'
			TvSys = _AnalogInvokeAudioToCheckTvSystem();			
			return TvSys;
		}
	}
	KPRINT("TvD not lock, report tmFe_TvSystemNone\n");
	return tmFe_TvSystemNone;	
#else	
	KPRINT("TvD not lock, report tmFe_TvSystemM\n");
	return tmFe_TvSystemM;
#endif//TODO:PI
}

static INT32 i4CheckNoticeBreak(VOID *pvArg)                                                            
{                                                                                                       
    /* FE_MONI_TASK_T *psMoniTask = &(((PD_CTX_T *) pvArg)->sFeMoniTask); */                                
    //FE_MONI_TASK_T *psMoniTask = (FE_MONI_TASK_T *) pvArg;                                                  
    ATV_PD_CTX_T *psPdCtx = (ATV_PD_CTX_T *) pvArg;                                                                                                
    if(psPdCtx->fgBreakMonitor)                                                                      
    {                                                                                                   
        KPRINT(" [%s-%s = 1]\n", __FILE__, __FUNCTION__);                                       
    }                                                                                                   
    return (psPdCtx->fgBreakMonitor);                                                                 
}

static void _MonitorProcess(void *pvArg)
{
	TUNER_CMD_QUEUE_T			rCmdQ;

    INT32 						i4Result;
    UINT16 						u2MsgIdx;
    SIZE_T 						zMsgSize = sizeof(UINT32);
    
    BOOL                        bInterrupt = FALSE;
    UINT8 Reg[2];
    //KPRINT("MonitorProcess start\n");

    while (fgMonitorThreadRunning)
    {
        bInterrupt = FALSE;

        i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &rCmdQ, &zMsgSize, 
                           &_hTunerCmdQueue, 1, (UINT32)TUNER_EVENT_TIMEOUT);
        //! decrease the number of cmd in tuner queue
    	x_sema_lock(t_atv_TunerQueue_lock, X_SEMA_OPTION_WAIT);
        if (i4Result == OSR_OK &&
            (rCmdQ.CmdId == TUNER_CMD_SETFREQ || rCmdQ.CmdId == TUNER_CMD_STARTSEARCH))
        {
            tunerQueueCmdCount --;
            KPRINT("--tunerQueueCmdCount (cmd=%d)\n", rCmdQ.CmdId);
        }
        x_sema_unlock(t_atv_TunerQueue_lock);

		// ++ bDetachMW is set by nim command
		if (bDetachMW){
			if (!_fgPreDetachMWSts){
				_fgPreDetachMWSts = TRUE;
				KPRINT("_MonitorProcess: bDetachMW is set, skip _MonitorProcess function\n");
			}
			continue;
		}
		else{
			if (_fgPreDetachMWSts){
				_fgPreDetachMWSts = FALSE;
				KPRINT("_MonitorProcess: bDetachMW is cleared, recover _MonitorProcess function\n");
			}
		}
		// --
        
        _SEMA_LOCK();
          
        if (i4Result == OSR_OK){
            switch(rCmdQ.CmdId){
            case TUNER_CMD_SETFREQ:
                //KPRINT("_hTunerCmdQueue: Get eCmd == TUNER_CMD_SETFREQ\n");
                //KPRINT("rCmdQ.CmdInfo.SetFreqInfo.finetune = %d\n", rCmdQ.CmdInfo.SetFreqInfo.finetune);
                KPRINT("TUNER_CMD_SETFREQ: freq = %d KHz\n", rCmdQ.CmdInfo.SetFreqInfo.freq);
                {                
                //if (!rCmdQ.CmdInfo.SetFreqInfo.finetune){							
                Nat32		designator;
                int 		source;
                HAL_TIME_T 	rTime1;
                HAL_TIME_T 	rTime2;
                HAL_TIME_T	rDiffTime;
                Bool		Lock = FALSE;
                tmFe_TvSystem_t tvSys = tmFe_TvSystemNone;
                    
                // stop audio before set to new freq.
                // Justin TODO
                _AudioFlowControl(AUD_STOP,_digital_mode);
                
                HAL_GetTime(&rTime1);
                
                _papi_src_src_GetCurrentVideoSelection(&source, &designator);
                
                if (designator==papi_src_dsgid_DesignatorDigital ||
                    (designator==papi_src_dsgid_DesignatorNone&&!rCmdQ.CmdInfo.SetFreqInfo.finetune)){
                    KPRINT("papi_src_dsgid_DesignatorDigital\n");
                    _digital_mode = TRUE;
                    _IndicateCarrierPresentChgIfNecessary(0);							
                    _DigitalAcq(rCmdQ.CmdInfo.SetFreqInfo.freq);
                }
                else if (designator==papi_src_dsgid_DesignatorAnalog ||
                         (designator==papi_src_dsgid_DesignatorNone&&rCmdQ.CmdInfo.SetFreqInfo.finetune)){
                    KPRINT("papi_src_dsgid_DesignatorAnalog\n");
                    _digital_mode = FALSE;
                    _fgAutoSearch = FALSE;    

                    // in setfreq, notify AFC is on->off
                    if (rAnaPara.AfcEnabled) {
                        _NotifyAfcModeChanged();
                        rAnaPara.AfcEnabled=FALSE;
                    }

                    _IndicateCarrierPresentChgIfNecessary(0);
                    _AnalogAcq(rCmdQ.CmdInfo.SetFreqInfo.freq);
                    // Justin TODO, do micro search if finetune
                    /*
                    if (rCmdQ.CmdInfo.SetFreqInfo.finetune) {
                        AnalogPINA_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtvPdCtx), &u2FFF, _BreakFct, pvArg);
                    }*/
                }
                        			
                while (1){
                if( tunerQueueCmdCount > 0 )
                {
                    KPRINT("Skipping GetStationPresent Command(SetFreq)\n");
                    break;
                }
                if (_digital_mode){
                    //++ Chek Carrier Present			
                    Lock = _DigitalGetCarrierPresent();
                    //--
                }
                else{
                    //++ Check Station Present
                    Lock = _AnaGetStationPresent();
                    if (Lock) {
                        KPRINT("VIF Lock, _AnaGetStationPresent() = %d\n", Lock);
                    }
                    //--
                }
            
                HAL_GetTime(&rTime2);
                HAL_GetDeltaTime(&rDiffTime, &rTime1, &rTime2);
                if (Lock || (rDiffTime.u4Seconds>=1)){
                    break;
                }
            
                _SleepMs_Interruptible(50);
            }
	        			
            //if VIF lock, check TVD lock
            if (Lock)
            {
                //Check TV system if VIF is locked in analog mode
                if (!_digital_mode && tmFe_TvSystemNone == _AnalogDetectTvSystem())
                    Lock = FALSE;
                else
                {
                    //play audio if new freq. is locked.
                    _AnalogSetAudioSysMask();
                    _AudioFlowControl(AUD_PLAY,_digital_mode);                                
                }
            }

            if (Lock && (rAnaPara.SearchMode&tmFe_SmTvSystemIncluded)){
                tvSys = _AnalogDetectTvSystem();
                if (tvSys == tmFe_TvSystemNone)
                    Lock = FALSE;
            }
            
            if( tunerQueueCmdCount > 0 )
            {
                KPRINT("Skipping NotifyStationPresent Command(SetFreq)\n");
            }
            else
            {
                _NotifyStationPresent(Lock);
                _NotifyTvSysDetected(tvSys);                
            }

            // setfreq done, notify AFC is off->on
            if (!_digital_mode && !rAnaPara.AfcEnabled) {
           //     _NotifyAfcModeChanged();//only afc change from on->off then notify
                  rAnaPara.AfcEnabled==TRUE;
            }
            }
            break;

            case TUNER_CMD_STARTSEARCH:
                KPRINT("TUNER_CMD_STARTSEARCH: SearchMode = 0x%x\n", rCmdQ.CmdInfo.StartSearchInfo.SearchMode);
//#if 0//TODO:PI
                if (_digital_mode){
                    // Digital mode
                    Bool	Lock = FALSE;
                    INT32	endFreqKHz;

                    _NotifySearchStateChanged(tmFe_SsSearching);
#if 1//ignore digital
                    tMT5133SysParam.SearchMode = rCmdQ.CmdInfo.StartSearchInfo.SearchMode;
                    endFreqKHz = rCmdQ.CmdInfo.StartSearchInfo.endFreq*62500/1000;

                    if (!(tMT5133SysParam.SearchMode & tmFe_SmCurrentIncluded)){
                        tMT5133SysParam.Freq += BRAZIL_CHTAB_BANDWIDTH; // Add BW to Skip locking the current channel
                    }
                    //++	
                    // for wrap around case
                    if (tMT5133SysParam.Freq >= endFreqKHz){
                        KPRINT("TUNER_CMD_STARTSEARCH: Start Freq = %dKHz, End Freq = %dKHz\n", tMT5133SysParam.Freq, u4FeDivMaxFreq);
                        Lock = _DigitalSearchNearbyWithNotifyInProgress(tMT5133SysParam.Freq, u4FeDivMaxFreq,
 tMT5133SysParam.SearchMode, &bInterrupt);
                        if (bInterrupt)
                        {
                            _SEMA_UNLOCK();
                            continue;
                        }
                        if (!Lock){
                            tMT5133SysParam.Freq = u4FeDivMinFreq;
                            KPRINT("TUNER_CMD_STARTSEARCH: Start Freq = %dKHz, End Freq = %dKHz\n", tMT5133SysParam.Freq, endFreqKHz);
                            Lock = _DigitalSearchNearbyWithNotifyInProgress(tMT5133SysParam.Freq, endFreqKHz,
                                            tMT5133SysParam.SearchMode, &bInterrupt);
                            if (bInterrupt)
                            {
                                _SEMA_UNLOCK();
                                continue;
                            }
                        }		
                    }
                    else{
                        KPRINT("TUNER_CMD_STARTSEARCH: Start Freq = %dKHz, End Freq = %dKHz\n", tMT5133SysParam.Freq, endFreqKHz);
                        Lock = _DigitalSearchNearbyWithNotifyInProgress(tMT5133SysParam.Freq, endFreqKHz, 
                                        tMT5133SysParam.SearchMode, &bInterrupt);
                        if (bInterrupt)
                        {
                            _SEMA_UNLOCK();
                            continue;
                        }
                    }
                    //--
#endif
                    _NotifySearchStateChanged(tmFe_SsNone);
                    _NotifySearchInProgress(tmFe_SsNone);

                    KPRINT("TUNER_CMD_STARTSEARCH: Stop at Freq = %dKHz\n", tMT5133SysParam.Freq);
                    
                    if( tunerQueueCmdCount > 0 )
                    {
                        KPRINT("Skipping Digital StartSearch Command\n");
                    }
                    else
                    {
                        _NotifyStationPresent(Lock);
                    }
                }
                else{
                    // Analog mode
                    BOOL Lock = FALSE;
                    INT32 endFreqKHz;
                    tmFe_TvSystem_t tvSys = tmFe_TvSystemNone;

                    // in start search, notify AFC is on->off
                    if (rAnaPara.AfcEnabled) {
                        _NotifyAfcModeChanged();
                        rAnaPara.AfcEnabled = FALSE; // disable AFC when StartSearch
                    }

                    _NotifySearchStateChanged(tmFe_SsSearching);
#if 1//ignore analog
                    _fgAutoSearch = TRUE;
                    _fgStopSearch=FALSE;  
                    rAnaPara.SearchMode = rCmdQ.CmdInfo.StartSearchInfo.SearchMode;
                    endFreqKHz = rCmdQ.CmdInfo.StartSearchInfo.endFreq*62500/1000;

                    if (!(rAnaPara.SearchMode & tmFe_SmCurrentIncluded)){
                        rAnaPara.Freq += 300; // Add 300 KHz to Skip locking the current frequency

                        _AnalogAcq(rAnaPara.Freq);
                        /*
                        MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
                        AnalogPI_TunerSetFreq(pAtvPiCtx, rAnaPara.Freq, MOD_ANA_CABLE);
                        MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);	
                        */
                    }

                    //++	
                    if (endFreqKHz <= rAnaPara.Freq){
                        KPRINT("TUNER_CMD_STARTSEARCH: Start Freq = %dKHz, End Freq = %dKHz\n", rAnaPara.Freq, u4FeDivMaxFreq);
                        Lock = _AnalogSearchNearbyWithNotifyInProgress(rAnaPara.Freq, u4FeDivMaxFreq, &bInterrupt);
                        if (bInterrupt)
                        {
                            _SEMA_UNLOCK();
                            continue;
                        }
                        if (!Lock){
                            rAnaPara.Freq = u4FeDivMinFreq;
                            KPRINT("TUNER_CMD_STARTSEARCH: Start Freq = %dKHz, End Freq = %dKHz\n", rAnaPara.Freq, endFreqKHz);
                            Lock = _AnalogSearchNearbyWithNotifyInProgress(rAnaPara.Freq, endFreqKHz, &bInterrupt);
                            if (bInterrupt)
                            {
                                _SEMA_UNLOCK();
                                continue;
                            }
                        }		
                    }
                    else{
                        KPRINT("TUNER_CMD_STARTSEARCH: Start Freq = %dKHz, End Freq = %dKHz\n", rAnaPara.Freq, endFreqKHz);
                        Lock = _AnalogSearchNearbyWithNotifyInProgress(rAnaPara.Freq, endFreqKHz, &bInterrupt);
                        if (bInterrupt)
                        {
                            _SEMA_UNLOCK();
                            continue;
                        }
                    }
                    //--
                    KPRINT("TUNER_CMD_STARTSEARCH: Stop at Freq = %dKHz, VIF Lock = %d\n", rAnaPara.Freq, Lock);

                    if (Lock && (rAnaPara.SearchMode&tmFe_SmTvSystemIncluded)){
                        tvSys = _AnalogDetectTvSystem();
                        if (tvSys == tmFe_TvSystemNone)
                            Lock = FALSE;
                    }
#endif
                    _NotifySearchStateChanged(tmFe_SsNone);
                    _NotifySearchInProgress(tmFe_SsNone);
                    
                    
                    if( tunerQueueCmdCount > 0 )
                    {
                        KPRINT("Skipping Analog StartSearch Command\n");
                    }
                    else
                    {
                        _NotifyStationPresent(Lock);
                        _NotifyTvSysDetected(tvSys);
                    }

                    // start search done, notify AFC off->on
                    /*
                    if (rAnaPara.AfcEnabled) {
                        _NotifyAfcModeChanged();
                       rAnaPara.AfcEnabled=TRUE;
                    }*/
                }
//#endif//TODO:PI
                break;
            default:
                KPRINT("Error: Unknown eCmd\n");
                break;
        	}
        }
		else if (i4Result == OSR_TIMEOUT){
#if 1//TODO:PI  //Justin TODO, what's this ??
			if (_digital_mode){
                if(ISDBT_IsDemodSleep()){
                                
                   KPRINT("Resume Digital Demod\n");
                   if(!ISDBT_FEInitialize()){//FE initialization,please refer HSI)
                     KPRINT("FE Initialize fail\n");
                   }
                   else{
                     KPRINT("Start Digital Tuning %dKhz\n",tMT5133SysParam.Freq);
                     _papi_fe_tuner_SetFreq(tMT5133SysParam.Freq, 0);
                   }
                }
				//++ Check Carrier Present			
				_IndicateCarrierPresentChgIfNecessary(_DigitalGetCarrierPresent());				
				//--
			}
            else{
                // check if ATF enabled?
                if (rAnaPara.AfcEnabled) {
                    UINT32 u4PreFreq = pAtvPdCtx->u4AcqFrequency;

                    // do AFT
                    pAtvPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor
                    AnalogPINA_TunerMonitor(pAtvPiCtx, &(pAtvPdCtx->u4AcqFrequency),  
                                            i4CheckNoticeBreak, pAtvPdCtx);
                    pAtvPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked

                    // check if freq is changed?
                    if (u4PreFreq != pAtvPdCtx->u4AcqFrequency) {
                        _NotifyAfcFreqChanged(pAtvPdCtx->u4AcqFrequency);

                        // check if boundary reached
                        if ((pAtvPdCtx->u4AcqFrequency >= rAnaPara.Freq + TV_FINE_TUNE_THRESHOLD_KHZ/2) ||
                            (pAtvPdCtx->u4AcqFrequency <= rAnaPara.Freq - TV_FINE_TUNE_THRESHOLD_KHZ/2)) {
                            _NotifyAfcLimitReached(pAtvPdCtx->u4AcqFrequency);
                        }
                    }
                }
            }
#endif//TODO:PI
		}
		else{
        	KPRINT("Error: x_msg_q_receive_timeout failed\n");
		}
			
		_SEMA_UNLOCK();
			
    }
    
    t_monitor_thread = NULL;
    KPRINT("MonitorProcess exit!\n");
    x_thread_exit();
}

static int _StartMonitorEngine(void)
{
	TUNER_CMD_QUEUE_T			rCmdQ;

    if ((x_msg_q_create(&_hTunerCmdQueue, "TUNER_CMD_Q", sizeof(rCmdQ), TUNER_CMD_QUEUE_SIZE)) != OSR_OK){
    	KPRINT("Fatal error: x_msg_q_create failed\n");
    	return (-1);
    }

    // create mutex to protect cmd counter
    if (x_sema_create(&t_atv_TunerQueue_lock,  X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        KPRINT("_StartMonitorEngine (x_sema_create(t_atv_TunerQueue_lock)): ERROR!\n");
        return (-1);
    }
    x_sema_unlock(t_atv_TunerQueue_lock);
    
    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not 
    // directly use pv_arg. Therefore, if we want thread to share the same 
    // context as caller, we need set pv_arg as "pointer of context".
    fgMonitorThreadRunning = TRUE;
    if (x_thread_create(&t_monitor_thread,
            "_MonThread",
            2048, 90,
            _MonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        KPRINT("_StartMonitorEngine (mcTHREAD_CREATE): ERROR!\n");
        return (-1);
    }
    //KPRINT("_StartMonitorEngine success\n");
    return (0);
}

static int _StopMonitorEngine(void)
{
    fgMonitorThreadRunning = FALSE;
    while (t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    KPRINT("PD_StopMonitorEngine success\n");

    x_sema_delete(t_atv_TunerQueue_lock);

    return (0);
}

static Nat FindChanTblFromId(tmFe_ChanTable_t table_id, PIO_PAPI_FE_CHANTBL_PAIR_T* pPair)
{
    Nat8 i;
    
    if (pPair == NULL)
        return (-1);

    for (i = 0; i < PAPI_IO_FE_CHANTBL_NUM; i ++)
    {
        if (table_id == io_papi_fe_chantbl_pair[i].id)
        {
            *pPair = &io_papi_fe_chantbl_pair[i];
            return (i);
        }
    }
    return (-1);
}

static Nat FindNextValidChan(PIO_PAPI_FE_CHANTBL_T table, Nat32 siz, Nat32 freq)
{
    Nat8 i;

    if (freq > table[siz-1].final_freq || freq < table[0].initial_freq)
        return (-1);
    
    for (i = 0; i < siz; i ++)
    {
        if (table[i].central_freq >= freq)
            return table[i].chan;   // find ceiling or valid channel
    }

    if (i >= siz)   // shouldn't happen
        return (-1);
    return 0;
}

static Nat FindPrevValidChan(PIO_PAPI_FE_CHANTBL_T table, Nat32 siz, Nat32 freq)
{
    Nat8 i;

    if (freq > table[siz-1].final_freq || freq < table[0].initial_freq)
        return (-1);

    for (i = 0; i < siz; i ++)
    {
        if ((table[i].central_freq - freq) == 0)
            return table[i].chan;   // find valid channel
        else if ((table[i].central_freq > freq) && (i > 0))
            return table[i-1].chan;
    }

    if (i >= siz)   // shouldn't happen
        return (-1);
 return 0;
}

static int _papi_tuner_init(void)
{
	int i4Ret = 0;

	if (!_fe_tun_inited){
#ifdef MEASURE_DRV_INIT_TIME
        int id;
        id = TIME_RegisterEntry("_papi_tuner_init");
        TIME_StartMeasure(id);
#endif /* MEASURE_DRV_INIT_TIME */
        
		if (TunerOpen() == DRVAPI_TUNER_OK){
			_StartMonitorEngine();
			_fe_tun_inited = TRUE;

			// 1114

			pDtvPdCtx = (ISDBT_PD_CTX_T*)TunerGetCtx(DIGITAL_FE);
			pDemodCtx = (DEMOD_CTX_T*)pDtvPdCtx->pDemodCtx;

            // Justin temp
//            pDemodCtx = &DemodCtxTemp;
            if (ISDBT_Initialize(pDemodCtx)){
                KPRINT(("ISDBT_Initialize fail!\n"));
            }

			pTunerCtx = (RF_TUNER_CTX_T*)&pDemodCtx->sTunerCtx;
			t_dtv_semlock = pDtvPdCtx->hHalLock;
			//KPRINT("pDemodCtx = 0x%x, pTunerCtx = 0x%x\n ", pDemodCtx, pTunerCtx);
			
			pAtvPdCtx = (ATV_PD_CTX_T*)TunerGetCtx(ANALOG_FE);
			pAtvPiCtx = &pAtvPdCtx->sAtvPiCtx;
            pAtvPiCtx->pDigiTunerCtx = pTunerCtx;
			t_atv_semlock = pAtvPdCtx->hHalLock;
			//KPRINT("pAtvPiCtx = 0x%x", pAtvPiCtx);
		}
		else{
			KPRINT("TunerOpen failed\n");
			i4Ret = -EFAULT;
		}

#ifdef MEASURE_DRV_INIT_TIME
        TIME_StopMeasure(id);
#endif /* MEASURE_DRV_INIT_TIME */
	}
	
	return i4Ret;
}

static int _papi_tuner_Get1Para(unsigned int cmd, unsigned long arg)
{
	int i4Ret = 0;
	int retValue;	

    if (!access_ok(VERIFY_WRITE, (void __user *) arg, _IOC_SIZE(cmd)))    
	{        
		mcPRINT_ERR("papi_ioctl_sdtuner arg access error\n");        
		return -EFAULT;    
	}

	switch(cmd)
	{
    case PAPI_IO_FE_CHANTAB_GETFIRSTTABLE:
        {
            retValue = io_papi_fe_chantbl_pair[0].id;
        }
        break;
    case PAPI_IO_FE_CHANTAB_GETTABLE:
        {
            retValue = currChanTblPair->id;
        }
        break;

    case PAPI_IO_FE_TUN_GETCHANOFFSET:
        {   // Justin TODO
            retValue = 0;
        }
        break;
    case PAPI_IO_FE_TUN_GETTVSYSTEM:	
        {
            retValue = rAnaPara.TvSystem;
        }
        break;
    case PAPI_IO_FE_TUN_GETSEARCHSTATE:					
        {   // Justin TODO
            retValue = tmFe_SsNone;
        }
        break;
    case PAPI_IO_FE_TUN_GETSEARCHMODE:	
        {
            retValue = rAnaPara.SearchMode;
        }
        break;
    case PAPI_IO_FE_TUN_GETSTATIONPRESENT:
        _SEMA_LOCK();
        {
            if (_digital_mode){
                retValue = _DigitalGetCarrierPresent();
            }
            else{
                retValue = _AnaGetStationPresent();
                //retValue = _AnaGetStationPresent();
           }

        }
        _SEMA_UNLOCK();
        break;
    case PAPI_IO_FE_TUN_GETFREQ:
        {
            if (_digital_mode){
                retValue = tMT5133SysParam.Freq*1000/62500;
            }
            else{
                retValue = rAnaPara.Freq*1000/62500;
            }
        }
        break;

    case PAPI_IO_FE_TUN_GETCHAN:
        {
            UINT32 u4FreqInKhz;

            if (_digital_mode){
                u4FreqInKhz = tMT5133SysParam.Freq;
            }
            else{
                u4FreqInKhz = rAnaPara.Freq;
            }

            retValue = ChtabFreq2Ch(u4FreqInKhz);  
        }
        break;

    case PAPI_IO_FE_CHDECISDBT_GETSUPPCODERATES      :
        {
            retValue =  tmFe_CodeRate1_2 | 
                        tmFe_CodeRate2_3 | 
                        tmFe_CodeRate3_4 | 
                        tmFe_CodeRate5_6 | 
                        tmFe_CodeRate7_8 |
                        tmFe_CodeRateInvalid |
                        tmFe_CodeRateUnknown |
                        tmFe_CodeRateAuto;
        }    
        break;
    case PAPI_IO_FE_CHDECISDBT_GETSUPPCONSTELLATIONS :
        {
            retValue =  tmFe_InvalidConstellation | 
                        tmFe_AutoConstellation | 
                        tmFe_Qam64 | 
                        tmFe_Qam16 | 
                        tmFe_Qpsk  | 
                        tmFe_DQpsk |
                        tmFe_UnknownConstellation;
        }
        break;
    case PAPI_IO_FE_CHDECISDBT_GETSUPPGUARDINTERVALS :
        {
            retValue =  tmFe_GuardIntervalInvalid | 
                        tmFe_GuardIntervalUnknown | 
                        tmFe_GuardIntervalAuto | 
                        tmFe_GuardInterval1_32 | 
                        tmFe_GuardInterval1_16 | 
                        tmFe_GuardInterval1_8  | 
                        tmFe_GuardInterval1_4;
        }
        break;
    case PAPI_IO_FE_CHDECISDBT_GETSUPPORTEDLAYERS    :
        {
            retValue =  PAPI_LayerA |
                        PAPI_LayerB |
                        PAPI_LayerC;
        }
        break;
    case PAPI_IO_FE_CHDECISDBT_GETACTUALGUARDINTERVAL:
        _SEMA_LOCK();   // protect HW accessing from race condition
        {
            if (ISDBT_UpdateModPara(pDemodCtx))
            { 
                retValue = (-EIO); // I2C error
                break;
            }
        }
        _SEMA_UNLOCK();
        // remove break to do the following get guard interval case
        //break;
    case PAPI_IO_FE_CHDECISDBT_GETGUARDINTERVAL      :
        {
            switch (ISDBT_GetGiSize()) {
            case ISDBT_GI_SIZE_1_32:     retValue = tmFe_GuardInterval1_32;    break;
            case ISDBT_GI_SIZE_1_16:     retValue = tmFe_GuardInterval1_16;    break;
            case ISDBT_GI_SIZE_1_8:      retValue = tmFe_GuardInterval1_8;     break;
            case ISDBT_GI_SIZE_1_4:      retValue = tmFe_GuardInterval1_4;     break;
            case ISDBT_GI_SIZE_UNKNOWN:  retValue = tmFe_GuardIntervalUnknown; break;
            default:                     retValue = tmFe_GuardIntervalInvalid; break;
            }
        }
        break;
    case PAPI_IO_FE_CHDECISDBT_GETLAYER              :
        {
            UINT8 u1LayerMask = 0;

            retValue = 0;
            u1LayerMask = ISDBT_GetLayer(pDemodCtx);

            if (u1LayerMask & ISDBT_TS_MASK_LAYERA) {
                retValue |= PAPI_LayerA;
            }
            if (u1LayerMask & ISDBT_TS_MASK_LAYERB) {
                retValue |= PAPI_LayerB;
            }
            if (u1LayerMask & ISDBT_TS_MASK_LAYERC) {
                retValue |= PAPI_LayerC;
            }
        }
        break;
    case PAPI_IO_FE_CHDECISDBT_GETCHANNELINFORMATION :
        {
            // Justin TODO FB1
        }
        break;

#if 0//TODO:PI
		// Actual
		case PAPI_IO_FE_CHDEC_GETACTUALIQMODE:
			_SEMA_LOCK();
			{
				UINT8 ucIqMode;
				MT5133_GetPara(pDemodCtx);
				ucIqMode = MT5133_GetSi(pDemodCtx);
				if (ucIqMode == 1)
					retValue = tmFe_IqModeNormal;	// for PAPI definition
				else if (ucIqMode == 2)
					retValue = tmFe_IqModeInverse;	// for PAPI definition
				else
					retValue = tmFe_UnknownIqMode;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDEC_GETACTUALCONSTELLATION:
			_SEMA_LOCK();
			{
				UINT8 ucConst;
				MT5133_GetPara(pDemodCtx);
				ucConst = MT5133_GetConst(pDemodCtx);
				if (ucConst == 0)
					retValue = tmFe_Qpsk;
				else if (ucConst == 1)
					retValue = tmFe_Qam16;
				else if (ucConst == 2)
					retValue = tmFe_Qam64;
				else
					retValue = tmFe_UnknownConstellation;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GetActualHierarchyMode:
			_SEMA_LOCK();
			{	
				UINT8 ucHier;
				MT5133_GetPara(pDemodCtx);
				ucHier = MT5133_GetHier(pDemodCtx);
				if (ucHier == 0)
					retValue = tmFe_HierarchyNone;
				else if (ucHier == 1)
					retValue = tmFe_HierarchyAlpha1;
				else if (ucHier == 2)
					retValue = tmFe_HierarchyAlpha2;
				else if (ucHier == 3)
					retValue = tmFe_HierarchyAlpha4;
				else
					retValue = tmFe_HierarchyModeUnknown;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GETACTUALGUARDINTERVAL:	
			_SEMA_LOCK();
			{	
				UINT8 ucGi;
				MT5133_GetPara(pDemodCtx);
				ucGi = MT5133_GetGI(pDemodCtx);
				if (ucGi == 0)
					retValue = tmFe_GuardInterval1_32;
				else if (ucGi == 1)
					retValue = tmFe_GuardInterval1_16;
				else if (ucGi == 2)
					retValue = tmFe_GuardInterval1_8;
				else if (ucGi == 3)
					retValue = tmFe_GuardInterval1_4;
				else
					retValue = tmFe_GuardIntervalUnknown;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GETACTUALTXMODE:		
			_SEMA_LOCK();
			{	
				UINT8 ucTxMode;
				MT5133_GetPara(pDemodCtx);
				ucTxMode = MT5133_GetFftMode(pDemodCtx);
				if (ucTxMode == 0)
					retValue = tmFe_TxMode2k;
				else if (ucTxMode == 1)
					retValue = tmFe_TxMode8k;
				else
					retValue = tmFe_TxModeUnknown;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GETACTUALCHANBANDWIDTH:	
			_SEMA_LOCK();
			{	
				UINT8 ucBw, ii = 0;
				HAL_TIME_T 	rTime1;
				HAL_TIME_T 	rTime2;
				HAL_TIME_T	rDiffTime;
				/*ucBw = MT5133_GetBW(pDemodCtx);
				if (ucBw == 0)
					retValue = tmFe_ChanBandwidth6MHz;
				else if (ucBw == 1)
					retValue = tmFe_ChanBandwidth7MHz;
				else if (ucBw == 2)
					retValue = tmFe_ChanBandwidth8MHz;
				else
					retValue = tmFe_ChanBandwidthUnknown;*/
				ucBw = 3;
				for (ii = 0; ii < 3; ii++)
				{
					MT5133_FSM_Stop(pDemodCtx);
					MT5133_SetSearchParam(pDemodCtx, CH_SEARCH_SCAN, lastMd, lastCr, lastCons, lastHier, lastFreqBias);
					MT5133_SetBW(pDemodCtx, ii);
					MT5133_FSM_Start(pDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);
					
					HAL_GetTime(&rTime1);
					while(1)
					{
						//update demod status before MT5133_GetDvbtNotExist(p) & MT5133_GetTpsLock(p)
						MT5133_GetStatus(pDemodCtx);	
					
						if (MT5133_GetDvbtNotExist(pDemodCtx))
						{	
							//No DVB-T signal exists in this channel. Early break and go to next channel.
							break;
						}
						else if (MT5133_GetTpsLock(pDemodCtx))
						{
							ucBw = ii;
							ii = 3;
							break;
						}
					
						HAL_GetTime(&rTime2);
						HAL_GetDeltaTime(&rDiffTime, &rTime1, &rTime2);
						if ((rDiffTime.u4Seconds>=1)||(rDiffTime.u4Micros>500000)){
							break;
						}
					
						_SleepMs_Interruptible(5);						
					}
				}
				if (ucBw == 0)
					retValue = tmFe_ChanBandwidth6MHz;
				else if (ucBw == 1)
					retValue = tmFe_ChanBandwidth7MHz;
				else if (ucBw == 2)
					retValue = tmFe_ChanBandwidth8MHz;
				else
					retValue = tmFe_ChanBandwidthUnknown;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GETACTUALFREQUENCYOFFSET:	
			_SEMA_LOCK();
			{	
				INT32	s4FreqOffset;
				s4FreqOffset = MT5133_GetFreqOffset_kHz(pDemodCtx)/256/166;
				if (s4FreqOffset == 0)
					retValue = tmFe_OffsetNone;
				else if (s4FreqOffset == 1)
					retValue = tmFe_OffsetP1d6MHz;
				else if (s4FreqOffset == 2)
					retValue = tmFe_OffsetP2d6MHz;
				else if (s4FreqOffset == 3)
					retValue = tmFe_OffsetP3d6MHz;
				else if (s4FreqOffset == -1)
					retValue = tmFe_OffsetM1d6MHz;
				else if (s4FreqOffset == -2)
					retValue = tmFe_OffsetM2d6MHz;
				else if (s4FreqOffset == -3)
					retValue = tmFe_OffsetM3d6MHz;
				else
					retValue = tmFe_OffsetUnknown;
			}
			_SEMA_UNLOCK();
			break;
			
		// Supported
		case PAPI_IO_FE_CHDEC_GETTIMEERRORSUPP:		
			{
				retValue = FALSE;
			}
			break;
		case PAPI_IO_FE_CHDEC_GETFREQERRORSUPP:			
			{
				retValue = TRUE;
			}
			break;
		case PAPI_IO_FE_CHDEC_GETSYMBOLERRORSUPP:	
			{
				retValue = FALSE;
			}
			break;
		case PAPI_IO_FE_CHDEC_GETSUPPIQMODES:
			{
				retValue = tmFe_IqModeNormal | tmFe_IqModeInverse;	
			}
			break;
		case PAPI_IO_FE_CHDEC_GETSUPPCONSTELLATIONS:
			{
				retValue = tmFe_InvalidConstellation | tmFe_AutoConstellation | tmFe_Qam64 | tmFe_Qam16 | tmFe_Qpsk | 
									tmFe_UnknownConstellation;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETSUPPHierarchyModes:	
			{
				retValue = tmFe_HierarchyNone | tmFe_HierarchyAlpha1 | tmFe_HierarchyAlpha2 | tmFe_HierarchyAlpha4 |
							tmFe_HierarchyModeAuto;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETSUPPCODERATES:		
			{
				retValue = tmFe_CodeRate1_2 | tmFe_CodeRate2_3 | tmFe_CodeRate3_4 | tmFe_CodeRate5_6 | tmFe_CodeRate7_8 |
							tmFe_CodeRateAuto;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETSUPPGUARDINTERVALS:		
			{
				retValue = tmFe_GuardIntervalInvalid | tmFe_GuardIntervalUnknown | tmFe_GuardIntervalAuto | 
							tmFe_GuardInterval1_32 | tmFe_GuardInterval1_16 | tmFe_GuardInterval1_8 | 
							tmFe_GuardInterval1_4;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETSUPPTXMODES:		
			{
				retValue = tmFe_TxModeInvalid | tmFe_TxModeUnknown | tmFe_TxModeAuto | tmFe_TxMode2k | tmFe_TxMode8k;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETSUPPCHANBANDWIDTHS:		
			{
				retValue = tmFe_ChanBandwidth6MHz | tmFe_ChanBandwidth7MHz | tmFe_ChanBandwidth8MHz;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETSUPPFREQUENCYOFFSETS:	
			{
				retValue = tmFe_OffsetP3d6MHz | tmFe_OffsetM3d6MHz;
			}
			break;

		// Get
		case PAPI_IO_FE_CHDEC_GETTIMEERROR:
			_SEMA_LOCK();
			{
				retValue = MT5133_GetTimingOffset(pDemodCtx)/1000;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDEC_GETBERTHRESHOLD:
			{
				retValue = tMT5133SysParam.Threshold;
			}
			break;
		case PAPI_IO_FE_CHDEC_GETFREQERROR:		
			_SEMA_LOCK();
			{
				retValue = MT5133_GetFreqOffset_kHz(pDemodCtx)/256*1000;	
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDEC_GETSYMBOLERROR:			
			_SEMA_LOCK();
			{
				retValue = MT5133_GetErrorPkt(pDemodCtx);
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDEC_GETCARRIERPRESENT:
			_SEMA_LOCK();
			{
				retValue = _DigitalGetCarrierPresent();
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDEC_GETBER:
			_SEMA_LOCK();
			{
				INT32 	s4Ber;
				s4Ber = MT5133_GetPostVBer(pDemodCtx);   // BER * 10^5
				if (s4Ber == -1)
					retValue = 0;
				else
					retValue = (UINT32)s4Ber * 1000;   // unit is 10^-8
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDEC_GETIQMODE:
			{
				retValue = tMT5133SysParam.IQMode;;
			}
			break;
		case PAPI_IO_FE_CHDEC_GETCONSTELLATION:
			{
				retValue = tMT5133SysParam.Constellation;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GetHierarchyMode:
			{
				retValue = tMT5133SysParam.Hierarchy;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETGUARDINTERVAl:	
			{
				retValue = tMT5133SysParam.GuardInterval;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETTXMODE:			
			{
				retValue = tMT5133SysParam.TxMode;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETCHANBANDWIDTH:	
			{
				retValue = tMT5133SysParam.Bandwidth;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETSTREAMPRIORITY:	
			_SEMA_LOCK();
			{
				UINT8 ucHpLp;
				ucHpLp = MT5133_GetHpLp(pDemodCtx);
				if (ucHpLp == 0)
					retValue = tmFe_StreamPriorityHigh;
				else
					retValue = tmFe_StreamPriorityLow;
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GETTPSCELLID:	
			_SEMA_LOCK();
			{
				retValue = MT5133_GetCellId(pDemodCtx);
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GETCHANNELINFORMATION:	
			{
				retValue = tMT5133SysParam.ChannelInfo;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETFREQUENCYOFFSET:	
			{
				retValue = tMT5133SysParam.FreqOffset;			
			}
			break;
		case PAPI_IO_FE_ASSM_GETMEASVALID:
			{
				retValue = TRUE;
			}
			break;
		case PAPI_IO_FE_ASSM_START:
			{
				retValue = TRUE;
			}
			break;
		case PAPI_IO_FE_ASSM_STOP:
			{
				retValue = TRUE;
			}
			break;
		case PAPI_IO_FE_ASSM_GETSIGSTRENGTH:
			_SEMA_LOCK();
	        {
	        	UINT8	u1Value; 
	        	UINT16	u2Value;
	        	
	        	MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, TRUE);
                u2Value = SIF_Read_Only(0xf00, 0xC2, &u1Value, 1);
                MT5133_AtvTunerI2c(DEFAULT_I2C_ADDR, FALSE);
                            
                if (u2Value == 0)
                {
                	retValue = 0;
                }
                else
                {
                	switch (u1Value & 0x07)
                	{
                		case 0:
                			retValue = 100;
                		    break;
                		case 1:
                			retValue = 80;
                		    break;
                        case 2:
                			retValue = 60;
                		    break;
                		case 3:
                			retValue = 40;
                		    break;    
                		default:
                			retValue = 20;
                		    break;
                    }
                }		                                
			}
			_SEMA_UNLOCK();
			break;
#endif//TODO:PI
		case PAPI_IO_FE_QSSM_START:
			{
				retValue = TRUE;
			}
			break;
		case PAPI_IO_FE_QSSM_STOP:
			{
				retValue = TRUE;
			}
			break;
		case PAPI_IO_FE_QSSM_GETMEASVALID:
			{
				retValue = TRUE;
			}
			break;
		case PAPI_IO_FE_QSSM_GETSIGSTRENGTH:
			_SEMA_LOCK();
			{
				retValue = ISDBT_GetSignalLevel(pDemodCtx);
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_TUN_GETAFCVALUE:				
			_SEMA_LOCK();
			{
				retValue = _AnalogGetAfc();
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_TUN_GETAFCENABLED:					
			{
				retValue = rAnaPara.AfcEnabled;
			}
			break;
		case PAPI_IO_FE_TUN_GETAFCSUPP:				
			{
                Nat32		designator;
                int 		source;

                _papi_src_src_GetCurrentVideoSelection(&source, &designator);

                if (designator==papi_src_dsgid_DesignatorDigital){
                    retValue = FALSE;
                }
                else if (designator==papi_src_dsgid_DesignatorAnalog){
                    retValue = TRUE;
                }
                else {
                    retValue = FALSE;
                }
			}
			break;
		default:
			break;
	}

    if (copy_to_user((void __user *)arg, &(retValue), sizeof(retValue)))
    {
        return -EFAULT;
    }				
	
	return i4Ret;
}
static int _papi_tuner_Set1Para(unsigned int cmd, unsigned long arg)
{
	int i4Ret = 0;

	switch(cmd)
	{
    case PAPI_IO_FE_CHANTAB_SETTABLE:
        {
            PIO_PAPI_FE_CHANTBL_PAIR_T pair;

            if (-1 == FindChanTblFromId(arg, &pair))
            {
                mcPRINT_ERR("PAPI_IO_FE_CHANTAB_SETTABLE: no matching table found\n");
                return -EFAULT;
            }
            currChanTblPair = pair;
        }
        break;
    case PAPI_IO_FE_CHDECISDBT_SETGUARDINTERVAL      :
        _SEMA_LOCK();   // protect HW accessing from race condition
        {
            UINT8 u1GiSize;

            switch (arg) {
            case tmFe_GuardInterval1_32:  u1GiSize=ISDBT_GI_SIZE_1_32;    break;
            case tmFe_GuardInterval1_16:  u1GiSize=ISDBT_GI_SIZE_1_16;    break;
            case tmFe_GuardInterval1_8:   u1GiSize=ISDBT_GI_SIZE_1_8;     break;
            case tmFe_GuardInterval1_4:   u1GiSize=ISDBT_GI_SIZE_1_4;     break;
            default:                      u1GiSize=ISDBT_GI_SIZE_UNKNOWN; break;
            }
            
            if (ISDBT_SetGiSize(pDemodCtx, u1GiSize))
            {
                i4Ret = (-EIO); // I2C error
            }
        }
        _SEMA_UNLOCK();
        break;
    case PAPI_IO_FE_CHDECISDBT_SETLAYER              :
        _SEMA_LOCK();   // protect HW accessing from race condition
        {
            UINT8 u1TsMAsk = 0;
            if (arg & PAPI_LayerA) {
                u1TsMAsk |= ISDBT_TS_MASK_LAYERA;
            }
            if (arg & PAPI_LayerB) {
                u1TsMAsk |= ISDBT_TS_MASK_LAYERB;
            }
            if (arg & PAPI_LayerC) {
                u1TsMAsk |= ISDBT_TS_MASK_LAYERC;
            }
            
            if (ISDBT_SetLayer(pDemodCtx, u1TsMAsk))
            {
                i4Ret = (-EIO); // I2C error
            }
        }
        _SEMA_UNLOCK();
        break;		
    case PAPI_IO_FE_CHDECISDBT_SETCHANNELINFORMATION :
        {
            // not implement yet, Justin TODO FB1
        }
        break;
    case PAPI_IO_FE_TUN_STOPSEARCH:	
        {
            KPRINT("PAPI_IO_FE_TUN_STOPSEARCH\n");
            _fgStopSearch = TRUE;
        }
        break;			
    case PAPI_IO_FE_TUN_SETTVSYSTEM:
        {        	 
            // no need to do anything to ATD, support NTSC & PAL M/N only
            // rAnaPara.TunerSetSys = MOD_NTSC_M; // Justin TODO
            //KPRINT("PAPI_IO_FE_TUN_SETTVSYSTEM: %d\n", arg);
            rAnaPara.TvSystem = arg; //(tmFe_TvSystemM | tmFe_TvSystemN);
        }
        break;
/*
        _SEMA_LOCK();
        {
            rAnaPara.TvSystem = arg;
            switch(arg){
                case tmFe_TvSystemBg:
                    rAnaPara.TunerSetSys = MOD_PAL_BG;
                    break;
                case tmFe_TvSystemD:
                case tmFe_TvSystemDk: 
                case tmFe_TvSystemDk1:
                case tmFe_TvSystemDk2:
                case tmFe_TvSystemK1:				    
                    rAnaPara.TunerSetSys = MOD_PAL_DK;
                    break;
                case tmFe_TvSystemI:
                    rAnaPara.TunerSetSys = MOD_PAL_I;   
                    break;                                             
                case tmFe_TvSystemL:
                    rAnaPara.TunerSetSys = MOD_SECAM_L;   
                    break;
                case tmFe_TvSystemL1:
                    rAnaPara.TunerSetSys = MOD_SECAM_L1;   
                    break;    
                default:
                    rAnaPara.TunerSetSys = MOD_PAL_BG;
                    break;
            }
            _AnalogTunerSetSys(rAnaPara.TunerSetSys);
        }
        _SEMA_UNLOCK();
        break;
*/

#if 0//TODO:PI
		case PAPI_IO_FE_CHDEC_DVBT_SETCHANBANDWIDTH:
			{
				tMT5133SysParam.Bandwidth = arg;
			}
			break;		
		case PAPI_IO_FE_CHDEC_SETIQMODE:
			{
				tMT5133SysParam.IQMode = arg;
				
				if (tMT5133SysParam.IQMode == tmFe_IqModeInverse){
					mcSET_FIELD(lastMd, 0x02, 0x03, 0);   // for PAPI funtion mapping
				}
				else if (tMT5133SysParam.IQMode == tmFe_IqModeNormal){
					mcSET_FIELD(lastMd, 0x01, 0x03, 0);   // for PAPI funtion mapping
				}
				else if (tMT5133SysParam.IQMode == tmFe_IqModeAuto){
					mcSET_FIELD(lastMd, 0x00, 0x03, 0);
				}
				else{
					mcSET_FIELD(lastMd, 0x02, 0x03, 0);   // for PAPI funtion mapping
				}
			}
			break;
		case PAPI_IO_FE_CHDEC_SETCONSTELLATION:
			{
				tMT5133SysParam.Constellation = arg;
				
				if (tMT5133SysParam.Constellation == tmFe_Qpsk)
					lastCons = 0x00;
				else if (tMT5133SysParam.Constellation == tmFe_Qam16)
					lastCons = 0x01;
				else if (tMT5133SysParam.Constellation == tmFe_Qam64)
					lastCons = 0x02;
				else
					lastCons = 0x01;
			}
			break;
		case PAPI_IO_FE_CHDEC_SETBERTHRESHOLD:
			{
				tMT5133SysParam.Threshold = arg;
			}
			break;
		case PAPI_IO_FE_CHDEC_SETTIMEERROR:		
			{
				tMT5133SysParam.TimingOffset = arg;
			}
			break;
		case PAPI_IO_FE_CHDEC_SETFREQERROR:		
			{
				tMT5133SysParam.FreqError = arg;
				
				lastFreqBias = tMT5133SysParam.FreqError / 1000 / 40;
			}
			break;
		case PAPI_IO_FE_CHDEC_SETSYMBOLERROR:			
			{
				tMT5133SysParam.SymbolError = arg;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_SETHIERARCHYMODE:
			{
				tMT5133SysParam.Hierarchy = arg;
				
				if (tMT5133SysParam.Hierarchy == tmFe_HierarchyNone)
					lastHier = 0x00;
				else if (tMT5133SysParam.Hierarchy == tmFe_HierarchyAlpha1)
					lastHier = 0x01;
				else if (tMT5133SysParam.Hierarchy == tmFe_HierarchyAlpha2)
					lastHier = 0x02;
				else if (tMT5133SysParam.Hierarchy == tmFe_HierarchyAlpha4)
					lastHier = 0x03;
				else
					lastHier = 0x00;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_SETGUARDINTERVAL:	
			{
				tMT5133SysParam.GuardInterval = arg;
				
				if (tMT5133SysParam.GuardInterval == tmFe_GuardInterval1_32){
					mcSET_FIELD(lastMd, 0x00, 0x1C, 2);
				}
				else if (tMT5133SysParam.GuardInterval == tmFe_GuardInterval1_16){
					mcSET_FIELD(lastMd, 0x01, 0x1C, 2);
				}
				else if (tMT5133SysParam.GuardInterval == tmFe_GuardInterval1_8){
					mcSET_FIELD(lastMd, 0x02, 0x1C, 2);
				}
				else if (tMT5133SysParam.GuardInterval == tmFe_GuardInterval1_4){
					mcSET_FIELD(lastMd, 0x03, 0x1C, 2);
				}
				else if (tMT5133SysParam.GuardInterval == tmFe_GuardIntervalAuto){
					mcSET_FIELD(lastMd, 0x04, 0x1C, 2);
				}
				else{
					mcSET_FIELD(lastMd, 0x03, 0x1C, 2);
				}
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_SETTXMODE:				
			{
				tMT5133SysParam.TxMode = arg;
				
				if (tMT5133SysParam.TxMode == tmFe_TxMode2k){
					lastMd = mcSET_BIT(lastMd, 7);
				}
				else if (tMT5133SysParam.TxMode == tmFe_TxMode8k){
					lastMd = mcSET_BIT(lastMd, 5);
				}
				else{
					lastMd = mcSET_BIT(lastMd, 5);
				}
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_SETSTREAMPRIORITY:			
			_SEMA_LOCK();
			{
				switch (arg)
				{
					case tmFe_StreamPriorityHigh:
						MT5133_SetHpLp(pDemodCtx, EN_HP_DECODE);
						break;
					case tmFe_StreamPriorityLow:
						MT5133_SetHpLp(pDemodCtx, EN_LP_DECODE);
						break;
					default:
						MT5133_SetHpLp(pDemodCtx, EN_HP_DECODE);
						break;
				}		
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_SETCHANNELINFORMATION:		
			{
				tMT5133SysParam.ChannelInfo = arg;
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_SETFREQUENCYOFFSET:
			{
				tMT5133SysParam.FreqOffset = arg;
			}
			break;

#endif//TODO:PI
		case PAPI_IO_FE_TUN_ENABLEAFC:	
			{
				rAnaPara.AfcEnabled = arg;
			}
			break;
		default:
			break;
	}


	return i4Ret;	
}
static int _papi_tuner_GetmParas(unsigned int cmd, unsigned long arg)
{
	int i4Ret = 0;

    if (!access_ok(VERIFY_WRITE, (void __user *) arg, _IOC_SIZE(cmd)))    
	{        
		mcPRINT_ERR("papi_ioctl_sdtuner arg access error\n");        
		return -EFAULT;    
	}

	switch(cmd)
	{
    case PAPI_IO_FE_TUN_GETCHANOFFSETRANGE:
        {
            IOCTL_2ARG_T rArg;
        
            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");        
                return -EFAULT;				
            }				

            // Justin TODO, what's max/min channel offset?
            rArg.au4Arg[0] = tmFe_NoOffset;     // the minimum channel offset. 
            rArg.au4Arg[1] = tmFe_NoOffset;    // the maximum channel offset. 
            
            if (copy_to_user((void __user *)arg, &(rArg), sizeof(IOCTL_2ARG_T)))
            {
                return -EFAULT;
            }		
        }
        break;
    case PAPI_IO_FE_TUN_GETSEARCHMODESUPP:
        {
            IOCTL_2ARG_T rArg;
        
            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");        
                return -EFAULT;				
            }				
            /*
            if ((rUInfo.mode & tmFe_SmDefault) ||
                (rUInfo.mode & tmFe_SmPositiveModulation) ||
                (rUInfo.mode & tmFe_SmCurrentIncluded) ||
                (rUInfo.mode & tmFe_SmTvSystemIncluded))
                */
            // rArg.au4Arg[0] is search mode input
            if ((rArg.au4Arg[0] & tmFe_SmDefault) ||
                (rArg.au4Arg[0] & tmFe_SmCurrentIncluded))
            {
                rArg.au4Arg[1] = TRUE;
            }
            else
            {
                rArg.au4Arg[1] = FALSE;
            }
            
            if (copy_to_user((void __user *)arg, &(rArg), sizeof(IOCTL_2ARG_T)))
            {
                return -EFAULT;
            }				
        }
        break;
    case PAPI_IO_FE_TUN_GETTVSYSTEMSUPP:
        {
            IOCTL_2ARG_T rArg;

            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");        
                return -EFAULT;				
            }				

            // rArg.au4Arg[0] is TV system input
            if ((rArg.au4Arg[0] & tmFe_TvSystemM) ||
                (rArg.au4Arg[0] & tmFe_TvSystemN)  ) {
                rArg.au4Arg[1] = TRUE;
            }
            else {
                rArg.au4Arg[1] = FALSE;
            }
/*
            switch(rUInfo.tvSystem)
            {
                case tmFe_TvSystemBg:
                case tmFe_TvSystemD:
                case tmFe_TvSystemDk: 
                case tmFe_TvSystemDk1:
                case tmFe_TvSystemDk2:
                case tmFe_TvSystemK1:				    
                case tmFe_TvSystemI:
                case tmFe_TvSystemL:				        
                case tmFe_TvSystemL1:
                    rUInfo.Supp = TRUE;   
                    break;    
                default:
                    rUInfo.Supp = FALSE;
                    break;
            }
*/
            if (copy_to_user((void __user *)arg, &(rArg), sizeof(IOCTL_2ARG_T)))
            {
                return -EFAULT;
            }				
        }
        break;


		case PAPI_IO_FE_CHANTAB_GETCARRIERFREQ:
			{
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETCARRIERFREQ: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].chan)
                    {
                        rArg.au4Arg[2] = table[i].central_freq * 16 / 1000;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETCARRIERFREQ: no matching channel found\n");
                    rArg.au4Arg[2] = 0;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
			}
			break;
        case PAPI_IO_FE_CHANTAB_GETFIRSTCHAN:
            {
				IOCTL_2ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[0], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETFIRSTCHAN: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                rArg.au4Arg[1] = table[0].chan;    // the first channel entry in the table stored in element-1
                
                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));
            }
            break;
        case PAPI_IO_FE_CHANTAB_FREQ2CHAN:
            {
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_FREQ2CHAN: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].central_freq)
                    {
                        rArg.au4Arg[2] = table[i].chan;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_FREQ2CHAN: no matching frequency found\n");
                    rArg.au4Arg[2] = 0;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
            }
            break;
        case PAPI_IO_FE_CHANTAB_GETISCHAN:
            {
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETISCHAN: no matching table\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].chan)
                    {
                        rArg.au4Arg[2] = TRUE;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETISCHAN: no matching channel\n");
                    rArg.au4Arg[1] = TRUE;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
            }
            break;
        case PAPI_IO_FE_CHANTAB_GETLASTCHAN:
            {
				IOCTL_2ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[0], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETLASTCHAN: no matching table\n");
					return -EFAULT;
                }
                table = pair->table;

                rArg.au4Arg[1] = table[pair->siz - 1].chan;

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));
            }
            break;
		case PAPI_IO_FE_CHANTAB_GETLOWERFREQ:
			{
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETLOWERFREQ: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].chan)
                    {
                        rArg.au4Arg[2] = table[i].initial_freq * 16 / 1000;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETLOWERFREQ: no matching channel found\n");
                    rArg.au4Arg[2] = 0;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETNEXTCHAN:
			{
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETNEXTCHAN: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].chan)
                    {
                        if ((i + 1) >= pair->siz)
                            rArg.au4Arg[2] = table[0].chan;
                        else
                            rArg.au4Arg[2] = table[i+1].chan;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETNEXTCHAN: no matching channel found\n");
                    rArg.au4Arg[2] = 0;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETNEXTTABLE:
			{
				IOCTL_2ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                Nat tblIdx;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
                
                tblIdx = FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[0], &pair);
                if (-1 == tblIdx)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETNEXTTABLE: no matching table found\n");
					return -EFAULT;
                }

                if ((tblIdx + 1) >= PAPI_IO_FE_CHANTBL_NUM)
                    rArg.au4Arg[1] = io_papi_fe_chantbl_pair[0].id;
                else
                    rArg.au4Arg[1] = io_papi_fe_chantbl_pair[tblIdx + 1].id;

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETNEXTVALIDCHAN:
			{
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                Nat32 chan;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETNEXTVALIDCHAN: no matching table found\n");
					return -EFAULT;
                }

                chan = FindNextValidChan(pair->table, pair->siz, rArg.au4Arg[0]);
                if (-1 == chan)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETNEXTVALIDCHAN: no matching channel found\n");
					return -EFAULT;
                }

                rArg.au4Arg[2] = chan;

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETPREVCHAN:
			{
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETPREVCHAN: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].chan)
                    {
                        if (i == 0) // if 1st entry, then cyclying to the last entry
                            rArg.au4Arg[2] = table[pair->siz].chan;
                        else
                            rArg.au4Arg[2] = table[i-1].chan;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETPREVCHAN: no matching channel found\n");
                    rArg.au4Arg[2] = 0;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETPREVVALIDCHAN:
			{
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                Nat32 chan;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETPREVVALIDCHAN: no matching table found\n");
					return -EFAULT;
                }

                chan = FindPrevValidChan(pair->table, pair->siz, rArg.au4Arg[0]);
                if (-1 == chan)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETPREVVALIDCHAN: no matching channel found\n");
					return -EFAULT;
                }

                rArg.au4Arg[2] = chan;

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETSEGMENTBOUNDARIES:
			{
				IOCTL_4ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_4ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETSEGMENTBOUNDARIES: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].chan)
                    {
                        rArg.au4Arg[2] = table[i].initial_freq* 16 / 1000;
                        rArg.au4Arg[3] = table[i].final_freq* 16 / 1000;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETSEGMENTBOUNDARIES: no matching channel found\n");
                    rArg.au4Arg[2] = 0;
                    rArg.au4Arg[3] = 0;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_4ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETTABLESUPP:
			{
				IOCTL_2ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                Nat tblIdx;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
                
                tblIdx = FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[0], &pair);
                if (-1 == tblIdx)
			        rArg.au4Arg[1] = FALSE;
                else
                    rArg.au4Arg[1] = TRUE;

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));
			}
			break;
		case PAPI_IO_FE_CHANTAB_GETUPPERFREQ:
			{
				IOCTL_3ARG_T rArg;
                PIO_PAPI_FE_CHANTBL_PAIR_T pair;
                PIO_PAPI_FE_CHANTBL_T table;
                Nat8 i;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));

                if (-1 == FindChanTblFromId((tmFe_ChanTable_t)rArg.au4Arg[1], &pair))
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETUPPERFREQ: no matching table found\n");
					return -EFAULT;
                }
                table = pair->table;

                for (i = 0; i < pair->siz; i ++)
                {
                    if (rArg.au4Arg[0] == table[i].chan)
                    {
                        rArg.au4Arg[2] = table[i].final_freq * 16 / 1000;
                        break;
                    }
                }
                if (i >= pair->siz)
                {
			        mcPRINT_ERR("PAPI_IO_FE_CHANTAB_GETUPPERFREQ: no matching channel found\n");
                    rArg.au4Arg[2] = 0;
                    i4Ret = -EFAULT;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_3ARG_T));
			}
			break;
            
        // ISDBT
        case PAPI_IO_FE_CHDECISDBT_GETACTUALCODERATES:
			_SEMA_LOCK();   // protect HW accessing from race condition
			{
                if (ISDBT_UpdateModPara(pDemodCtx))
                { 
                    i4Ret = (-EIO); // I2C error
                    break;
                }
			}
			_SEMA_UNLOCK();
            // remove break to do the following get code rate case
            //break;
        case PAPI_IO_FE_CHDECISDBT_GETCODERATES          :
            {
                IOCTL_2ARG_T rArg;
                UINT8 u1layer = ISDBT_LAYER_A;
                UINT8 fgInvalidLayer = FALSE;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));

                switch (rArg.au4Arg[0])
                {
                case PAPI_LayerA: u1layer = ISDBT_LAYER_A; break;
                case PAPI_LayerB: u1layer = ISDBT_LAYER_B; break;
                case PAPI_LayerC: u1layer = ISDBT_LAYER_C; break;
                default:  fgInvalidLayer = TRUE; break;
                }
                switch (ISDBT_GetCr(u1layer)) {
                case ISDBT_CR_1_2:      rArg.au4Arg[1] = tmFe_CodeRate1_2;     break;
                case ISDBT_CR_2_3:      rArg.au4Arg[1] = tmFe_CodeRate2_3;     break;
                case ISDBT_CR_3_4:      rArg.au4Arg[1] = tmFe_CodeRate3_4;     break;
                case ISDBT_CR_5_6:      rArg.au4Arg[1] = tmFe_CodeRate5_6;     break;
                case ISDBT_CR_7_8:      rArg.au4Arg[1] = tmFe_CodeRate7_8;     break;
                case ISDBT_CR_NOLAYER:  rArg.au4Arg[1] = tmFe_CodeRateInvalid; break;
                case ISDBT_CR_RESERVED: rArg.au4Arg[1] = tmFe_CodeRateUnknown; break;
                default:                rArg.au4Arg[1] = tmFe_CodeRateInvalid; break;
                }

                if (fgInvalidLayer) {
                    rArg.au4Arg[1] = tmFe_CodeRateInvalid;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));
            }
            break;

        case PAPI_IO_FE_CHDECISDBT_GETACTUALCONSTELLATION:
            _SEMA_LOCK();   // protect HW accessing from race condition
            {
                if (ISDBT_UpdateModPara(pDemodCtx))
                { 
                    i4Ret = (-EIO); // I2C error
                    break;
                }
            }
            _SEMA_UNLOCK();
            // remove break to do the following get constellation case
            //break;
        case PAPI_IO_FE_CHDECISDBT_GETCONSTELLATION      :
            {
                IOCTL_2ARG_T rArg;
                UINT8 u1layer = ISDBT_LAYER_A;
                UINT8 fgInvalidLayer = FALSE;

                COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));

                switch (rArg.au4Arg[0])
                {
                case PAPI_LayerA: u1layer = ISDBT_LAYER_A; break;
                case PAPI_LayerB: u1layer = ISDBT_LAYER_B; break;
                case PAPI_LayerC: u1layer = ISDBT_LAYER_C; break;
                default:  fgInvalidLayer = TRUE; break;
                }

                switch (ISDBT_GetMod(u1layer)) {
                case ISDBT_MOD_DQPSK:     rArg.au4Arg[1] = tmFe_DQpsk;     break;
                case ISDBT_MOD_QPSK:      rArg.au4Arg[1] = tmFe_Qpsk;      break;
                case ISDBT_MOD_16QAM:     rArg.au4Arg[1] = tmFe_Qam16;     break;
                case ISDBT_MOD_64QAM:     rArg.au4Arg[1] = tmFe_Qam64;     break;
                case ISDBT_MOD_NOLAYER:   rArg.au4Arg[1] = tmFe_InvalidConstellation; break;
                case ISDBT_MOD_RESERVED:  rArg.au4Arg[1] = tmFe_UnknownConstellation; break;
                default:                  rArg.au4Arg[1] = tmFe_InvalidConstellation; break;
                }

                if (fgInvalidLayer) {
                    rArg.au4Arg[1] = tmFe_InvalidConstellation;
                }

                COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));
            }
            break;

        case PAPI_IO_FE_TUN_GETFREQRANGE:
            {
                IOCTL_2ARG_T rArg;
    
                rArg.au4Arg[0] = u4FeDivMinFreq*1000/62500; //the minimum frequency in units of 1/16th MHz
                rArg.au4Arg[1] = u4FeDivMaxFreq*1000/62500; //the maximum frequency in units of 1/16th MHz
                if (copy_to_user((void __user *)arg, &(rArg), sizeof(IOCTL_2ARG_T)))
                {
                    return -EFAULT;
                }				
            }
            break;

#if 0//TODO:PI
		case PAPI_IO_FE_CHDEC_GETBERTHRESHOLDRANGE:
			{
				PAPI_IO_CHDEC_BERTHRESHOLDRANGE_INFO_T	rUInfo;
				rUInfo.MinThreshold = 1000;     // 10^-5
				rUInfo.MaxThreshold = 10000000; // 0.1
			    if (copy_to_user((void __user *)arg, &(rUInfo), sizeof(rUInfo)))
			    {
			        return -EFAULT;
			    }				
			}
			break;
		case PAPI_IO_FE_CHDECDVBT_GETACTUALCODERATES:
			_SEMA_LOCK();
			{
				PAPI_IO_CHDEC_CODERATE_INFO_T	rUInfo;
				UINT8 ucHpCr, ucLpCr, ucHier;
				MT5133_GetPara(pDemodCtx);
				ucHier = MT5133_GetHier(pDemodCtx);
				ucHpCr = MT5133_GetHpCR(pDemodCtx);
				ucLpCr = MT5133_GetLpCR(pDemodCtx);
				if (ucHpCr == 0)
					rUInfo.HighPrioRate = tmFe_CodeRate1_2;
				else if (ucHpCr == 1)
					rUInfo.HighPrioRate = tmFe_CodeRate2_3;
				else if (ucHpCr == 2)
					rUInfo.HighPrioRate = tmFe_CodeRate3_4;
				else if (ucHpCr == 3)
					rUInfo.HighPrioRate = tmFe_CodeRate5_6;
				else if (ucHpCr == 4)
					rUInfo.HighPrioRate = tmFe_CodeRate7_8;
				else
					rUInfo.HighPrioRate = tmFe_CodeRateUnknown;
					
				if (ucHier == 0)
					rUInfo.LowPrioRate = tmFe_CodeRateNone;
				else
				{					
					if (ucLpCr == 0)
						rUInfo.LowPrioRate = tmFe_CodeRate1_2;
					else if (ucLpCr == 1)
						rUInfo.LowPrioRate = tmFe_CodeRate2_3;
					else if (ucLpCr == 2)
						rUInfo.LowPrioRate = tmFe_CodeRate3_4;
					else if (ucLpCr == 3)
						rUInfo.LowPrioRate = tmFe_CodeRate5_6;
					else if (ucLpCr == 4)
						rUInfo.LowPrioRate = tmFe_CodeRate7_8;
					else
						rUInfo.LowPrioRate = tmFe_CodeRateUnknown;
				}						
			
			    if (copy_to_user((void __user *)arg, &(rUInfo), sizeof(rUInfo)))
			    {
			    	_SEMA_UNLOCK();
			        return -EFAULT;
			    }				
			}
			_SEMA_UNLOCK();
			break;
		case PAPI_IO_FE_CHDECDVBT_GETCODERATES:		
			{
				PAPI_IO_CHDEC_CODERATE_INFO_T	rUInfo;
				rUInfo.HighPrioRate = tMT5133SysParam.CodeRate_hp;
				rUInfo.LowPrioRate = tMT5133SysParam.CodeRate_lp;
			    if (copy_to_user((void __user *)arg, &(rUInfo), sizeof(rUInfo)))
			    {
			        return -EFAULT;
			    }				
			}
			break;

		case PAPI_IO_FE_ASSM_GETSIGSTRENGTHRANGE:
			{
				PAPI_IO_FE_ASSM_SIGNALSTRENGTHRANGE_INFO_T	rUInfo;

				rUInfo.MinStrength = MIN_ASSM;
				rUInfo.MaxStrength = MAX_ASSM;
			    if (copy_to_user((void __user *)arg, &(rUInfo), sizeof(rUInfo)))
			    {
			        return -EFAULT;
			    }				
			}
			break;
#endif//TODO:PI
		case PAPI_IO_FE_QSSM_GETSIGSTRENGTHRANGE:
			{
				IOCTL_2ARG_T rUInfo;

				rUInfo.au4Arg[0] = MIN_QSSM;
				rUInfo.au4Arg[1] = MAX_QSSM;
			    if (copy_to_user((void __user *)arg, &(rUInfo), sizeof(rUInfo)))
			    {
			        return -EFAULT;
			    }				
			}
			break;
		default:
			break;
	}
	
	return i4Ret;
}
static int _papi_tuner_SetmParas(unsigned int cmd, unsigned long arg)
{
	int i4Ret = 0;

    if (!access_ok(VERIFY_READ, (void __user *) arg, _IOC_SIZE(cmd)))    
	{        
        mcPRINT_ERR("papi_ioctl_sdtuner arg access error\n");        
        return -EFAULT;    
	}

	switch(cmd)
	{
    // not important, Justin TODO FB0.5
    case PAPI_IO_FE_CHDECISDBT_SETCODERATES          :  
        _SEMA_LOCK();   // protect HW accessing from race condition
        {            
            IOCTL_2ARG_T rArg;
            UINT8  u1layer;
            UINT8  u1Coderate;

            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");   
                _SEMA_UNLOCK();
                return -EFAULT;				
            }

            switch (rArg.au4Arg[0])
            {
            case PAPI_LayerA: u1layer = ISDBT_LAYER_A; break;
            case PAPI_LayerB: u1layer = ISDBT_LAYER_B; break;
            case PAPI_LayerC: u1layer = ISDBT_LAYER_C; break;
            default:          u1layer = 0;             break;
            }

            switch (rArg.au4Arg[1]) {
            case tmFe_CodeRate1_2:     u1Coderate = ISDBT_CR_1_2;     break;
            case tmFe_CodeRate2_3:     u1Coderate = ISDBT_CR_2_3;     break;
            case tmFe_CodeRate3_4:     u1Coderate = ISDBT_CR_3_4;     break;
            case tmFe_CodeRate5_6:     u1Coderate = ISDBT_CR_5_6;     break;
            case tmFe_CodeRate7_8:     u1Coderate = ISDBT_CR_7_8;     break;
            default:                   u1Coderate = ISDBT_CR_NOLAYER; break;
            }

            if (ISDBT_SetCr(pDemodCtx, u1layer, u1Coderate))
            {
                i4Ret = (-EIO); // I2C error
            }

        }
        _SEMA_UNLOCK();
        break;              
    case PAPI_IO_FE_CHDECISDBT_SETCONSTELLATION      :
        _SEMA_LOCK();   // protect HW accessing from race condition
        {            
            IOCTL_2ARG_T rArg;
            UINT8  u1layer;
            UINT8  u1Constellation;

            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");    
                _SEMA_UNLOCK();
                return -EFAULT;				
            }

            switch (rArg.au4Arg[0])
            {
            case PAPI_LayerA: u1layer = ISDBT_LAYER_A; break;
            case PAPI_LayerB: u1layer = ISDBT_LAYER_B; break;
            case PAPI_LayerC: u1layer = ISDBT_LAYER_C; break;
            default:          u1layer = 0;             break;
            }

            switch (rArg.au4Arg[1]) {
            case tmFe_DQpsk:     u1Constellation = ISDBT_MOD_DQPSK;     break;
            case tmFe_Qpsk:      u1Constellation = ISDBT_MOD_QPSK;      break;
            case tmFe_Qam16:     u1Constellation = ISDBT_MOD_16QAM;     break;
            case tmFe_Qam64:     u1Constellation = ISDBT_MOD_64QAM;     break;
            default:             u1Constellation = ISDBT_MOD_NOLAYER;   break;
            }

            if (ISDBT_SetMod(pDemodCtx, u1layer, u1Constellation))
            {
                i4Ret = (-EIO); // I2C error
            }
        }
        _SEMA_UNLOCK();
        break;

    case PAPI_IO_FE_TUN_SETCHAN:
        {
            IOCTL_2ARG_T rArg;
            UINT32 u4ChanNum, u4ChanOffset;
            UINT32 u4Freq;

            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");        
                return -EFAULT;				
            }				

            u4ChanNum = rArg.au4Arg[0];
            u4ChanOffset = rArg.au4Arg[1];

            u4Freq = ChtabCh2Freq(u4ChanNum);
            u4Freq += u4ChanOffset *1000/16;

            _papi_fe_tuner_SetFreq(u4Freq, TRUE); // Justin TODO, check finetune value
        }
        break;
    case PAPI_IO_FE_TUN_SETFREQ:
        {
            IOCTL_2ARG_T rArg;
            UINT32 u4Freq;
            BOOL fgFinetune;
            
            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");        
                return -EFAULT;				
            }				
            
            u4Freq = rArg.au4Arg[0] *1000/16;
            fgFinetune = (BOOL)rArg.au4Arg[1];            
            _papi_fe_tuner_SetFreq(u4Freq, fgFinetune);
        }
        break;
    case PAPI_IO_FE_TUN_STARTSEARCH:
        {
            IOCTL_2ARG_T rArg;

            if (copy_from_user(&rArg, (void __user *) arg, sizeof(IOCTL_2ARG_T)))
            {
                mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");        
                return -EFAULT;				
            }				

            _fgStopSearch = FALSE;
            // Send MsgQ to thread
            TUNER_CMD_QUEUE_T 	rCmdQ;

            rCmdQ.CmdId = TUNER_CMD_STARTSEARCH;
            rCmdQ.CmdInfo.StartSearchInfo.SearchMode = rArg.au4Arg[0];
            rCmdQ.CmdInfo.StartSearchInfo.endFreq = rArg.au4Arg[1];

            x_sema_lock(t_atv_TunerQueue_lock, X_SEMA_OPTION_WAIT);
            if ((x_msg_q_send(_hTunerCmdQueue, &rCmdQ, sizeof(TUNER_CMD_QUEUE_T), 1)) != OSR_OK){
                KPRINT("Error: x_msg_q_send failed\n");
            }
            else
            {
                tunerQueueCmdCount ++;
                KPRINT("++tunerQueueCmdCount (StartSearch)\n");
            }
            x_sema_unlock(t_atv_TunerQueue_lock);
        }
        break;

#if 0//TODO:PI
		case PAPI_IO_FE_CHDECDVBT_SETCODERATES:
			{
				PAPI_IO_CHDEC_CODERATE_INFO_T	rUInfo;
				
				if (copy_from_user(&rUInfo, (void __user *) arg, sizeof(PAPI_IO_CHDEC_CODERATE_INFO_T)))
				{
			        mcPRINT_ERR("papi_ioctl_sdtuner copy_from_user error\n");        
					return -EFAULT;				
				}				
				
				tMT5133SysParam.CodeRate_hp = rUInfo.HighPrioRate;
				tMT5133SysParam.CodeRate_lp = rUInfo.LowPrioRate;
				
				if (tMT5133SysParam.CodeRate_hp == tmFe_CodeRate1_2)
					lastCr = 0x00;
				else if (tMT5133SysParam.CodeRate_hp == tmFe_CodeRate2_3)
					lastCr = 0x01;
				else if (tMT5133SysParam.CodeRate_hp == tmFe_CodeRate3_4)
					lastCr = 0x02;
				else if (tMT5133SysParam.CodeRate_hp == tmFe_CodeRate5_6)
					lastCr = 0x03;
				else if (tMT5133SysParam.CodeRate_hp == tmFe_CodeRate7_8)
					lastCr = 0x04;
				else
					lastCr = 0x03;
				
				if (tMT5133SysParam.CodeRate_lp == tmFe_CodeRate1_2){
					mcSET_FIELD(lastCr, 0x00, 0x38, 3);
				}
				else if (tMT5133SysParam.CodeRate_lp == tmFe_CodeRate2_3){
					mcSET_FIELD(lastCr, 0x01, 0x38, 3);
				}
				else if (tMT5133SysParam.CodeRate_lp == tmFe_CodeRate3_4){
					mcSET_FIELD(lastCr, 0x02, 0x38, 3);
				}
				else if (tMT5133SysParam.CodeRate_lp == tmFe_CodeRate5_6){
					mcSET_FIELD(lastCr, 0x03, 0x38, 3);
				}
				else if (tMT5133SysParam.CodeRate_lp == tmFe_CodeRate7_8){
					mcSET_FIELD(lastCr, 0x04, 0x38, 3);
				}
				else{
					mcSET_FIELD(lastCr, 0x03, 0x38, 3);
				}
					
				
			}
			break;
#endif//TODO:PI
		default:
			break;
	}
	return i4Ret;
}

/***********************************************************************/
/* Public Functions                                                    */
/***********************************************************************/
int papi_ioctl_fe_tun(struct inode *inode, struct file *file, unsigned int cmd,
                       unsigned long arg)
{
	int i4Ret = 0;

	if (cmd == PAPI_IO_FE_TUN_INIT){
		return _papi_tuner_init();
	}

	if (!_fe_tun_inited){
		KPRINT("papi_ioctl_fe_tun: (!_fe_tun_inited), return -EFAULT\n");
		return -EFAULT;
	}

	// ++ bDetachMW is set by nim command
	if (bDetachMW){
		KPRINT("papi_ioctl_fe_tun: bDetachMW is set, skip papi_ioctl_fe_tun function\n");
		return i4Ret;
	}
	// --

	_IsIoctl = TRUE;		
    switch (cmd)
    {
		// Get 1 parameter
		/** CHDEC
		case PAPI_IO_FE_CHDEC_GETTIMEERROR:	
		case PAPI_IO_FE_CHDEC_GETTIMEERRORSUPP:		
		case PAPI_IO_FE_CHDEC_GETFREQERRORSUPP:			
		case PAPI_IO_FE_CHDEC_GETFREQERROR:		
		case PAPI_IO_FE_CHDEC_GETSYMBOLERROR:			
		case PAPI_IO_FE_CHDEC_GETSYMBOLERRORSUPP:	
		case PAPI_IO_FE_CHDEC_GETBER:
		case PAPI_IO_FE_CHDEC_GETBERTHRESHOLD:
		case PAPI_IO_FE_CHDEC_GETCARRIERPRESENT:
		case PAPI_IO_FE_CHDEC_GETSUPPCONSTELLATIONS:
		case PAPI_IO_FE_CHDEC_GETSUPPIQMODES:
		case PAPI_IO_FE_CHDEC_GETACTUALCONSTELLATION:
		case PAPI_IO_FE_CHDEC_GETCONSTELLATION:
		case PAPI_IO_FE_CHDEC_GETIQMODE:
		case PAPI_IO_FE_CHDEC_GETACTUALIQMODE:
		*/
		/** DVBT
		case PAPI_IO_FE_CHDECDVBT_GETSUPPCODERATES:		
		case PAPI_IO_FE_CHDECDVBT_GETSUPPHierarchyModes:	
		case PAPI_IO_FE_CHDECDVBT_GetActualHierarchyMode:	
		case PAPI_IO_FE_CHDECDVBT_GetHierarchyMode:
		case PAPI_IO_FE_CHDECDVBT_GETGUARDINTERVAl:	
		case PAPI_IO_FE_CHDECDVBT_GETACTUALGUARDINTERVAL:	
		case PAPI_IO_FE_CHDECDVBT_GETSUPPGUARDINTERVALS:		
		case PAPI_IO_FE_CHDECDVBT_GETSUPPTXMODES:		
		case PAPI_IO_FE_CHDECDVBT_GETTXMODE:			
		case PAPI_IO_FE_CHDECDVBT_GETACTUALTXMODE:		
		case PAPI_IO_FE_CHDECDVBT_GETSUPPCHANBANDWIDTHS:		
		case PAPI_IO_FE_CHDECDVBT_GETCHANBANDWIDTH:	
		case PAPI_IO_FE_CHDECDVBT_GETSTREAMPRIORITY:	
		case PAPI_IO_FE_CHDECDVBT_GETACTUALCHANBANDWIDTH:	
		case PAPI_IO_FE_CHDECDVBT_GETTPSCELLID:	
		case PAPI_IO_FE_CHDECDVBT_GETCHANNELINFORMATION:	
		case PAPI_IO_FE_CHDECDVBT_GETSUPPFREQUENCYOFFSETS:	
		case PAPI_IO_FE_CHDECDVBT_GETACTUALFREQUENCYOFFSET:	
		case PAPI_IO_FE_CHDECDVBT_GETFREQUENCYOFFSET:
		*/
    // ATV part	
    case PAPI_IO_FE_TUN_GETTVSYSTEM:	
    case PAPI_IO_FE_TUN_GETSEARCHSTATE:					
    case PAPI_IO_FE_TUN_GETSEARCHMODE:
    case PAPI_IO_FE_TUN_GETFREQ:		
    case PAPI_IO_FE_TUN_GETSTATIONPRESENT:
    case PAPI_IO_FE_TUN_GETCHAN:
    case PAPI_IO_FE_CHANTAB_GETTABLE:
    case PAPI_IO_FE_CHANTAB_GETFIRSTTABLE:
    case PAPI_IO_FE_QSSM_GETMEASVALID:
    case PAPI_IO_FE_QSSM_STOP:
    case PAPI_IO_FE_QSSM_GETSIGSTRENGTH:
    case PAPI_IO_FE_QSSM_START:	
    case PAPI_IO_FE_TUN_GETAFCVALUE:				
    case PAPI_IO_FE_TUN_GETAFCENABLED:					
    case PAPI_IO_FE_TUN_GETAFCSUPP:		
    case PAPI_IO_FE_TUN_GETCHANOFFSET:
    // ISDB-T part
    case PAPI_IO_FE_CHDECISDBT_GETSUPPCODERATES      :
    case PAPI_IO_FE_CHDECISDBT_GETSUPPCONSTELLATIONS :
    case PAPI_IO_FE_CHDECISDBT_GETSUPPGUARDINTERVALS :
    case PAPI_IO_FE_CHDECISDBT_GETSUPPORTEDLAYERS    :
    case PAPI_IO_FE_CHDECISDBT_GETLAYER              :
    case PAPI_IO_FE_CHDECISDBT_GETACTUALGUARDINTERVAL:
    case PAPI_IO_FE_CHDECISDBT_GETGUARDINTERVAL      :
    case PAPI_IO_FE_CHDECISDBT_GETCHANNELINFORMATION :
        i4Ret = _papi_tuner_Get1Para(cmd, arg);
        break;			
			
		// Set 1 parameter
		/** CHDEC
		case PAPI_IO_FE_CHDEC_SETTIMEERROR:			
		case PAPI_IO_FE_CHDEC_SETFREQERROR:		
		case PAPI_IO_FE_CHDEC_SETSYMBOLERROR:			
		case PAPI_IO_FE_CHDEC_SETCONSTELLATION:
		case PAPI_IO_FE_CHDEC_SETIQMODE:
		case PAPI_IO_FE_CHDEC_SETBERTHRESHOLD:
		*/
        /** DVBT
		case PAPI_IO_FE_CHDEC_DVBT_SETCHANBANDWIDTH:
		case PAPI_IO_FE_CHDECDVBT_SETHIERARCHYMODE:
		case PAPI_IO_FE_CHDECDVBT_SETGUARDINTERVAL:	
		case PAPI_IO_FE_CHDECDVBT_SETTXMODE:				
		case PAPI_IO_FE_CHDECDVBT_SETSTREAMPRIORITY:			
		case PAPI_IO_FE_CHDECDVBT_SETCHANNELINFORMATION:		
		case PAPI_IO_FE_CHDECDVBT_SETFREQUENCYOFFSET:
		*/
		case PAPI_IO_FE_TUN_STOPSEARCH:				
		case PAPI_IO_FE_TUN_ENABLEAFC:	
		case PAPI_IO_FE_TUN_SETTVSYSTEM:
        case PAPI_IO_FE_CHANTAB_SETTABLE:
        case PAPI_IO_FE_CHDECISDBT_SETGUARDINTERVAL      :
        case PAPI_IO_FE_CHDECISDBT_SETLAYER              :
			i4Ret = _papi_tuner_Set1Para(cmd, arg);
			break;

		// Get m parameters
		/** DVBT
		case PAPI_IO_FE_CHDECDVBT_GETACTUALCODERATES:
		case PAPI_IO_FE_CHDECDVBT_GETCODERATES:		
		*/
		/** CHDEC
		case PAPI_IO_FE_CHDEC_GETBERTHRESHOLDRANGE:
        */
		case PAPI_IO_FE_TUN_GETFREQRANGE:
		case PAPI_IO_FE_TUN_GETTVSYSTEMSUPP:
        case PAPI_IO_FE_TUN_GETSEARCHMODESUPP:
        case PAPI_IO_FE_TUN_GETCHANOFFSETRANGE:
        case PAPI_IO_FE_CHANTAB_GETCARRIERFREQ:
        case PAPI_IO_FE_CHANTAB_GETFIRSTCHAN:
        case PAPI_IO_FE_CHANTAB_FREQ2CHAN:
        case PAPI_IO_FE_CHANTAB_GETISCHAN:
        case PAPI_IO_FE_CHANTAB_GETLASTCHAN:
        case PAPI_IO_FE_CHANTAB_GETLOWERFREQ:
        case PAPI_IO_FE_CHANTAB_GETNEXTCHAN:
        case PAPI_IO_FE_CHANTAB_GETNEXTTABLE:
        case PAPI_IO_FE_CHANTAB_GETNEXTVALIDCHAN:
        case PAPI_IO_FE_CHANTAB_GETPREVCHAN:
        case PAPI_IO_FE_CHANTAB_GETPREVVALIDCHAN:
        case PAPI_IO_FE_CHANTAB_GETSEGMENTBOUNDARIES:
        case PAPI_IO_FE_CHANTAB_GETTABLESUPP:
        case PAPI_IO_FE_CHANTAB_GETUPPERFREQ:
		case PAPI_IO_FE_QSSM_GETSIGSTRENGTHRANGE:
        case PAPI_IO_FE_CHDECISDBT_GETACTUALCODERATES:
        case PAPI_IO_FE_CHDECISDBT_GETCODERATES          :
        case PAPI_IO_FE_CHDECISDBT_GETACTUALCONSTELLATION:
        case PAPI_IO_FE_CHDECISDBT_GETCONSTELLATION      :
			i4Ret = _papi_tuner_GetmParas(cmd, arg);
			break;
		
		// Set m parameters
		/** DVBT
		case PAPI_IO_FE_CHDECDVBT_SETCODERATES:
		*/
		case PAPI_IO_FE_TUN_STARTSEARCH:
        case PAPI_IO_FE_TUN_SETFREQ:
        case PAPI_IO_FE_TUN_SETCHAN:
        case PAPI_IO_FE_CHDECISDBT_SETCODERATES          :        
        case PAPI_IO_FE_CHDECISDBT_SETCONSTELLATION      :
			i4Ret = _papi_tuner_SetmParas(cmd, arg);
			break;

        default:
            mcPRINT_ERR("PapiTuner: unknown IO command %u\n", cmd);
        	i4Ret = -EFAULT;
        	break;
    }
//    mcPRINTF("%s ret: %d\n", __FUNCTION__, i4Ret);

	if (_IsLockForIoctl){
		//KPRINT("Serious WARNING: Possible semaphore locked but not release correctly in papi_ioctl_fe_tun\n");
	}

	_IsIoctl = FALSE;		

    return i4Ret;
}


/* Exported */
INT32 _papi_fe_tuner_GetFreq(void)
{
    UINT32 u4Ret;
#if 0//TODO:PI  
	if (_digital_mode){
		u4Ret = tMT5133SysParam.Freq*1000/62500;
	}
	else{
		u4Ret = rAnaPara.Freq*1000/62500;
	}
#endif//TODO:PI
	return (INT32)u4Ret;
}

INT32 _papi_fe_tuner_GetTvSystem(void)
{
    return (INT32)(rAnaPara.TvSystem);
}

INT32 _papi_fe_tuner_SetTvSystem(INT32 i4TvSystem)
{   
	_SEMA_LOCK();
    {
#if 0//TODO:PI  
    	rAnaPara.TvSystem = i4TvSystem;
    	switch(i4TvSystem){
    	case tmFe_TvSystemBg:
    	    rAnaPara.TunerSetSys = MOD_PAL_BG;
    	    break;
    	case tmFe_TvSystemD:
    	case tmFe_TvSystemDk: 
    	case tmFe_TvSystemDk1:
    	case tmFe_TvSystemDk2:
    	case tmFe_TvSystemK1:				    
    	    rAnaPara.TunerSetSys = MOD_PAL_DK;
    	    break;
    	case tmFe_TvSystemI:
    	    rAnaPara.TunerSetSys = MOD_PAL_I;   
    	    break;                                             
    	case tmFe_TvSystemL:
    	    rAnaPara.TunerSetSys = MOD_SECAM_L;   
    	    break;
    	case tmFe_TvSystemL1:
    	    rAnaPara.TunerSetSys = MOD_SECAM_L1;   
    	    break;    
    	default:
    	    rAnaPara.TunerSetSys = MOD_PAL_BG;
    	    break;
    	}
    	_AnalogTunerSetSys(rAnaPara.TunerSetSys);
#endif//TODO:PI
    }
    _SEMA_UNLOCK();	
	return 0;
}

INT32 _papi_fe_tuner_SetFeDiversity(MTK_CUST_FE_DIV_T* prFeDiv)
{
    u4FeDivMaxFreq = prFeDiv->maxFreq;
    u4FeDivMinFreq = prFeDiv->minFreq;

    return 0;
}

