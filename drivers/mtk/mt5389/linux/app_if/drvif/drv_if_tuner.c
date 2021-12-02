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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_if_tuner.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tuner.c
 *  Driver interface: TUNER part implementation
 */

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_tuner.h"
#include "u_tuner.h"

//#include "tvsys_info.h"
//#include "TDIS.h"
#include "PI_Tuner.h"
#include "tuner_if.h"
#include "TDMgr.h"

//#include "TDIS.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
#include "drv_if_config.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MAX_TUNER_NUM	2

#define TUNER_ADC_WORKAROUND  0    

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#ifdef USE_5371
typedef union
{
    TUNER_CAB_DIG_TUNE_INFO_T t_cab_tuner_info;
    TUNER_TER_DIG_TUNE_INFO_T t_ter_tuner_info;
    TUNER_CAB_ANA_TUNE_INFO_T t_acab_tuner_info;
    TUNER_TER_ANA_TUNE_INFO_T t_ater_tuner_info;    
    TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T t_orx_tuner_info;
    TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T t_otx_tuner_info;
} TUNER_ALL_INFO_T;

typedef	struct _TUNER_CONFIG_T{
	UINT16				TunerID;			// MT5111_1, MT5111_2, T313_1 ¡K
											// LSB represents the TS bus ID
											// LSB = 0, TS0
											// LSB = 1, TS1
	UINT16				i2TunerCapability;	// TER, CAB, OOB_RX ¡K
	UCHAR				ucDemodInbAddr;		// For Demod Inband
	UCHAR				ucRFTunerAddr;		// For RF Tuner
	UCHAR				ucDemodOobAddr;		// For Demod Oob
	UINT16				ConnExclusiveID;			
	UCHAR				Reserved[8];		// For future extension. 
											// Should all be "0"s.
// Note this structure is also referenced by MW,
// remember to sync with MW if it is modified.											
} TUNER_CONFIG_T, *PTUNER_CONFIG_T;

typedef	struct _TUNER_CONFIG_TBL_T
{
	UINT16				TunerNum;
	UCHAR				MutualExcl;
	TUNER_CONFIG_T		tTunerCfg[MAX_TUNER_NUM];	
} TUNER_CONFIG_TBL_T, *PTUNER_CONFIG_TBL_T;
#endif

typedef enum
{
    DRV_TUNER_MSG_CONNECT,
    DRV_TUNER_MSG_DISCONNECT,    
    DRV_TUNER_MSG_KILL,     
    DRV_TUNER_MSG_UNKNOWN
} DRV_TUNER_MSG_TYPE_T;

typedef struct 
{
    DRV_TUNER_MSG_TYPE_T rType;
    DRV_TUNER_CONN_TYPE_T rConn;
    void* pvTag;
    TUNER_ALL_INFO_T rInfo;   
} DRV_TUNER_MSG_T;

typedef enum
{
    DRV_TUNER_STATE_NOT_INIT,
    DRV_TUNER_STATE_ANA_CONNECTED,
    DRV_TUNER_STATE_DIG_CONNECTED,
    DRV_TUNER_STATE_DISCONNECTED,    
    DRV_TUNER_STATE_UNKNOWN
} DRV_TUNER_STATE_T;

typedef enum
{
    DRV_TUNER_EVENT_ANA_LOCKED,
    DRV_TUNER_EVENT_DIG_LOCKED,
    DRV_TUNER_EVENT_UNKNOWN
} DRV_TUNER_EVENT_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hTunerOpMutex;
static HANDLE_T _hTunerMsgq;
static HANDLE_T _hTunerThread;

static DRV_TUNER_PARAM_T _rTunerParam;
static DRV_TUNER_PARAM_T _rTunerInterParam;
static TUNER_CONFIG_TBL_T _rTunerCfgTbl;
static DRV_TUNER_CONN_TYPE_T _rCurrConnType = DRV_TUNER_CT_MAX;
//static DRV_COMP_FCT_TBL_T _rFuncTbl;
static DRV_TUNER_STATE_T _rTunerState = DRV_TUNER_STATE_NOT_INIT;
static BOOL _fgTunerLockStatus = FALSE;
static BOOL _fgEnableMonitor = TRUE;

static DRV_TUNER_LOCK_STATUS_T _arTunerStatusMap[2] = 
{
    DRV_TUNER_NOT_LOCKED,
    DRV_TUNER_LOCKED
};
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DRV_TUNER_DIGC_MAX_FREQ     ((UINT32) 865000000)
#define DRV_TUNER_DIGC_MIN_FREQ     ((UINT32)  53000000)

#define DRV_TUNER_DIGT_MAX_FREQ     ((UINT32) 865000000)
#define DRV_TUNER_DIGT_MIN_FREQ     ((UINT32)  53000000)

#define DRV_TUNER_MUTEX_LOCK x_sema_lock(_hTunerOpMutex,X_SEMA_OPTION_WAIT);
#define DRV_TUNER_MUTEX_UNLOCK x_sema_unlock(_hTunerOpMutex);
#define DRV_TUNER_MUTEX_INIT x_sema_create(&_hTunerOpMutex,X_SEMA_TYPE_MUTEX,X_SEMA_STATE_UNLOCK);

#define DRV_TUNER_POLLING_DELAY 200

//-----------------------------------------------------------------------------
// Extern functions
//-----------------------------------------------------------------------------
//extern void TunerDisc(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);
//extern void TunerAcq(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, 
//            void* pv_conn_info, SIZE_T z_conn_info_len, 
//            x_break_fct _BreakFct, void* pvArg);
//extern INT16 TunerGetSync(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);
//extern INT32 TunerOpen(void);
//extern INT32 TDIS_RetrieveTunerCfgTbl(TUNER_CONFIG_TBL_T* ptTunerCfgTbl);
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

/*
static INT32 _ANA_SigNotify(UINT8 u1Status, void* pvTag)
{
    if(u1Status)
    {
        printf("[TUNER] Calling tuner lock done callback from ATuner_mw, DETECTED\n");   
        *((DRV_TUNER_SIGNAL_STATUS_T*)pvTag) = DRV_TUNER_SIGNAL_DETECTED;
    }
    else
    {
        printf("[TUNER] Calling tuner lock done callback from ATuner_mw, NOT DETECTED\n");   
        *((DRV_TUNER_SIGNAL_STATUS_T*)pvTag) = DRV_TUNER_SIGNAL_NOT_DETECTED;
    }
}
*/

static INT32 _TunerBreaker(void* pArg)
{
    return (INT32)FALSE;
}

static BOOL _GetLockStatus(DRV_TUNER_MSG_T* prMsg, UINT32 u4Iter)
{
    UINT16 u2Locked = 0;
    UINT32 i;
    UINT16 u2TunerId;
    if(prMsg->rConn == DRV_TUNER_CT_DIGC ||prMsg->rConn == DRV_TUNER_CT_DIGT)
    {
        u2TunerId = _rTunerCfgTbl.tTunerCfg[0].TunerID;
    }
    else
    {
        u2TunerId = _rTunerCfgTbl.tTunerCfg[1].TunerID;
    }
    u2Locked = TunerGetSync(u2TunerId, prMsg->rConn);                             
    for(i = 0; i < u4Iter && u2Locked == 0; i++)
    {                                 
        x_thread_delay(100);
        u2Locked = TunerGetSync(u2TunerId, prMsg->rConn);
    }
    if(u2Locked == 0)
    {
        return FALSE;
    }
    return TRUE;
}

static void _RequestHandler(DRV_TUNER_MSG_T* prMsg)
{
    switch (prMsg->rType)
    {
        case DRV_TUNER_MSG_DISCONNECT:                	              
            if(prMsg->rConn == DRV_TUNER_CT_DIGC || prMsg->rConn == DRV_TUNER_CT_DIGT)
            {
                TunerDisc(_rTunerCfgTbl.tTunerCfg[0].TunerID, prMsg->rConn);
            } 
            else if(prMsg->rConn == DRV_TUNER_CT_ANAC || prMsg->rConn == DRV_TUNER_CT_ANAT)
            {            
                TunerDisc(_rTunerCfgTbl.tTunerCfg[1].TunerID, prMsg->rConn);
            }                     
            _rTunerState = DRV_TUNER_STATE_DISCONNECTED;                    
            break;                
        case DRV_TUNER_MSG_CONNECT:
        	if(prMsg->rConn == DRV_TUNER_CT_DIGC || prMsg->rConn == DRV_TUNER_CT_DIGT)
        	{
                printf("[TUNER] Set Freq to %d\n", prMsg->rInfo.t_ter_tuner_info.ui4_freq);
#if 0          
                bApiVideoSetVideoSrc(0, SV_VS_DT1);           
#endif
                TunerAcq(_rTunerCfgTbl.tTunerCfg[0].TunerID, prMsg->rConn,
                              (void*)&(prMsg->rInfo),
                              sizeof(prMsg->rInfo),
                              _TunerBreaker, NULL);                

                _fgTunerLockStatus = _GetLockStatus(prMsg, 10);                         
                if(_rTunerParam.rDigParam.pfnTuningDone != NULL)
                { 
                    if (_fgTunerLockStatus)
                    {                    
                        printf("[TUNER] Lock at freq %d\n", prMsg->rInfo.t_ter_tuner_info.ui4_freq);
                        _rTunerParam.rDigParam.pfnTuningDone(((UINT32)DRV_TUNER_LOCKED << 24) | (UINT32)prMsg->pvTag);
                    }                         
                    else
                    {
                        printf("[TUNER] Not lock at freq %d\n", prMsg->rInfo.t_ter_tuner_info.ui4_freq);
                        _rTunerParam.rDigParam.pfnTuningDone(((UINT32)DRV_TUNER_NOT_LOCKED) << 24  | (UINT32)prMsg->pvTag);
                    }
        	    }
#ifdef CC_DVBT_SUPPORT
              VERIFY(DMX_SelectExternalTuner());
#endif

                _rTunerState = DRV_TUNER_STATE_DIG_CONNECTED;    
        	}
        	else if(prMsg->rConn == DRV_TUNER_CT_ANAC || prMsg->rConn == DRV_TUNER_CT_ANAT)
        	{
                 BOOL fgTunerLock;
#if 0
                bApiVideoSetVideoSrc(0, SV_VS_TUNER1);           
#endif
                fgTunerLock = TunerAcq(_rTunerCfgTbl.tTunerCfg[1].TunerID, prMsg->rConn,
                (void*)&(prMsg->rInfo),
                sizeof(prMsg->rInfo),
                _TunerBreaker, NULL);    

                _rTunerState = DRV_TUNER_STATE_ANA_CONNECTED; 
                
                if(_rTunerParam.rAnaParam.pfnTuningDone != NULL)
                {         
                    BOOL fgLockStatus;
                    //fgLockStatus = _GetLockStatus(prMsg, 10);
                    if(fgTunerLock)
                    {
                        fgLockStatus = fgDrvTunerCheckTVDLock(20);
                    }
                    else
                    {
                        fgLockStatus = FALSE;
                    }
                    if(fgLockStatus)
                    {                        
                  
                        _rTunerParam.rAnaParam.pfnTuningDone(((UINT32)DRV_TUNER_LOCKED << 24) | (UINT32)prMsg->pvTag);
                    }
                    else
                    {
                        _rTunerParam.rAnaParam.pfnTuningDone(((UINT32)DRV_TUNER_NOT_LOCKED) << 24  | (UINT32)prMsg->pvTag);
                    }
                }                                              

        	}
        	else
        	{
        	    
        	}                	
             break;
        case DRV_TUNER_MSG_KILL:
             _rTunerState = DRV_TUNER_STATE_NOT_INIT;
             break;
        default:
        	VERIFY(0);
    }
}

static void _TunerThread(VOID *pvArg)
{
    UINT16 u2MsgIdx;
    SIZE_T u4MsgSize;
    DRV_TUNER_MSG_T rMsg;
    INT32 i4Ret;

    
    while (_rTunerState != DRV_TUNER_STATE_NOT_INIT)
    {

        u4MsgSize = sizeof(rMsg);

        /* If there's no message in _hTunerMsgq, polling the sync status of tuner if tuner is in connected state */        
        if(_rTunerState == DRV_TUNER_STATE_DIG_CONNECTED && _rTunerParam.rDigParam.pfnLockStatusChange != NULL)
        {
            if( (i4Ret = x_msg_q_receive(&u2MsgIdx, (void*)&rMsg, &u4MsgSize, &_hTunerMsgq, 1, X_MSGQ_OPTION_NOWAIT)) == OSR_NO_MSG)
            {
    
                BOOL fgLockStatus = _GetLockStatus(&rMsg, 5);
                /* Not provided for the simplicity */           
                rMsg.rConn = _rCurrConnType;
                if(fgLockStatus != _fgTunerLockStatus)
                {
                    _fgTunerLockStatus = fgLockStatus;
                    printf("[TUNER] Monitored lock status change to %d\n", fgLockStatus); 
                    if(_rTunerParam.rDigParam.pfnLockStatusChange != NULL)
                    {
                        _rTunerParam.rDigParam.pfnLockStatusChange((INT32)(_arTunerStatusMap[fgLockStatus] << 24));                      
                    }      
                }                                          	
                else if(_rTunerCfgTbl.TunerNum >= 2 && _rTunerState == DRV_TUNER_STATE_ANA_CONNECTED)
                {
                    /* Not provided due to F/W limitation */           
                }
                else
                {
                    /* Nothing */
                }
                x_thread_delay(DRV_TUNER_POLLING_DELAY);
            }
            else
            {
                _RequestHandler(&rMsg);
            }
        }    
        else if((i4Ret = x_msg_q_receive(&u2MsgIdx, (void*)&rMsg, &u4MsgSize, &_hTunerMsgq, 1, X_MSGQ_OPTION_WAIT)) == OSR_OK)
        {                     
            _RequestHandler(&rMsg);   
        }
    } 

    x_thread_exit();
}

static UINT32 _TvSys2Mask(UINT32  u4TvSys)
{
    UINT32   u4Mask;

    if (u4TvSys != DRV_TV_SYS_UNKNOWN)
    {
        u4Mask = TV_SYS_MASK_B | TV_SYS_MASK_G;
        if (u4TvSys == (DRV_TV_SYS_B_G))
        {
            u4Mask = TV_SYS_MASK_B | TV_SYS_MASK_G;
        }
        else if (u4TvSys == (DRV_TV_SYS_D_K))
        {
            u4Mask = TV_SYS_MASK_D | TV_SYS_MASK_K;
        }
        else if (u4TvSys == (DRV_TV_SYS_I))
        {
            u4Mask = TV_SYS_MASK_I;
        }
        else if (u4TvSys == (DRV_TV_SYS_L))
        {
            u4Mask = TV_SYS_MASK_L;
        }
        else if (u4TvSys == (DRV_TV_SYS_L1))
        {
            u4Mask = TV_SYS_MASK_L_PRIME;
        }
        return u4Mask;
    }
    return TV_SYS_MASK_NONE;
}

static DRV_ERR_CODE_T _SetFreq(UINT32* pu4Freq, DRV_TUNER_CONN_TYPE_T rType, DRV_TV_SYS_T rTvSys, BOOL fgAutoFineTune, void* pvTag)
{
    //UINT32 u4InfoSize;
    //DRV_COMP_ID_T rCompId;
    TUNER_ALL_INFO_T rInfo;

    DRV_TUNER_MSG_T rMsg;

    if(pu4Freq == NULL)
    {
        return DRV_E_INV_ARG;
    }

    /* Disconnect currently enabled connection first */ 
    rMsg.rType = DRV_TUNER_MSG_DISCONNECT;
    rMsg.rConn = _rCurrConnType;            
    x_msg_q_send(_hTunerMsgq, (void*)&rMsg, sizeof(rMsg), 253);

    if(rType == DRV_TUNER_CT_DIGC)    
    {                    
        TUNER_CAB_DIG_TUNE_INFO_T *prInfo = (TUNER_CAB_DIG_TUNE_INFO_T *)&rInfo;

        if(*pu4Freq > DRV_TUNER_DIGC_MAX_FREQ || *pu4Freq < DRV_TUNER_DIGC_MIN_FREQ)
        {
            return DRV_E_INV_ARG;
        }
        
        prInfo->e_mod = (TUNER_MODULATION_T)_rTunerParam.rDigParam.rMod;
        prInfo->ui4_freq = *pu4Freq;
        prInfo->ui4_sym_rate = _rTunerParam.rDigParam.u4SymbolRate;
        prInfo->b_no_conn_disc_nfy = FALSE;
        
        //rCompId.ui2_id = _rTunerCfgTbl.tTunerCfg[0].TunerID;        
        //rCompId.e_type = DRVT_TUNER_CAB_DIG;        

        
        //u4InfoSize = sizeof(TUNER_CAB_DIG_TUNE_INFO_T);
    }
    else if(rType == DRV_TUNER_CT_DIGT)
    {
        TUNER_TER_DIG_TUNE_INFO_T *prInfo = (TUNER_TER_DIG_TUNE_INFO_T *)&rInfo;
        
        if(*pu4Freq > DRV_TUNER_DIGT_MAX_FREQ || *pu4Freq < DRV_TUNER_DIGT_MIN_FREQ)
        {
            return DRV_E_INV_ARG;
        }        
        
        prInfo->e_mod = (TUNER_MODULATION_T)_rTunerParam.rDigParam.rMod;
        prInfo->e_bandwidth = (TUNER_BANDWIDTH_T)_rTunerParam.rDigParam.rBandwidth;
        prInfo->ui4_freq = *pu4Freq;
        prInfo->b_no_conn_disc_nfy = FALSE;        

        //rCompId.ui2_id = _rTunerCfgTbl.tTunerCfg[0].TunerID;
        //rCompId.e_type = DRVT_TUNER_TER_DIG;
        
        //u4InfoSize = sizeof(TUNER_TER_DIG_TUNE_INFO_T);        
    }
    else if(rType == DRV_TUNER_CT_ANAC && _rTunerCfgTbl.TunerNum >= 2)
    {
        TUNER_CAB_ANA_TUNE_INFO_T *prInfo = (TUNER_CAB_ANA_TUNE_INFO_T *)&rInfo;
        prInfo->ui4_freq = *pu4Freq;
        prInfo->ui4_freq_bound = *pu4Freq + 100000;        
        prInfo->ui4_tv_sys_mask = _TvSys2Mask(rTvSys);
        prInfo->b_no_conn_disc_nfy = FALSE;
        if(fgAutoFineTune)
        {
            prInfo->e_mod = MOD_UNKNOWN;
        }
        else
        {
            prInfo->e_mod = MOD_PSK_8;
        }        
        //rCompId.ui2_id = _rTunerCfgTbl.tTunerCfg[1].TunerID;        
        //rCompId.e_type = DRVT_TUNER_CAB_ANA;
        
        //u4InfoSize = sizeof(TUNER_CAB_ANA_TUNE_INFO_T);
    }
    else if(rType == DRV_TUNER_CT_ANAT && _rTunerCfgTbl.TunerNum >= 2)
    {
        TUNER_TER_ANA_TUNE_INFO_T *prInfo = (TUNER_TER_ANA_TUNE_INFO_T *)&rInfo;
        prInfo->ui4_freq = *pu4Freq;
        prInfo->ui4_freq_bound = *pu4Freq + 100000;
        prInfo->ui4_tv_sys_mask = _TvSys2Mask(rTvSys);
        prInfo->b_fine_tune = fgAutoFineTune;
        
        prInfo->b_no_conn_disc_nfy = FALSE;
        if(fgAutoFineTune)
        {
            prInfo->e_mod = MOD_UNKNOWN;
        }
        else
        {
            prInfo->e_mod = MOD_PSK_8; 
        }
        //rCompId.ui2_id = _rTunerCfgTbl.tTunerCfg[1].TunerID;
        //rCompId.e_type = DRVT_TUNER_TER_ANA;
        
        //u4InfoSize = sizeof(TUNER_TER_ANA_TUNE_INFO_T);        
    }
    else
    {
        return DRV_E_FAIL;
    }
    rMsg.rType = DRV_TUNER_MSG_CONNECT;
    rMsg.rInfo = rInfo;
    rMsg.rConn = rType;
    rMsg.pvTag = (void*)((UINT32)pvTag & 0x00ffffff);
    _rCurrConnType = rType;     
               
    x_msg_q_send(_hTunerMsgq, (void*)&rMsg, sizeof(rMsg), 253);

    return DRV_E_OK;    
}



//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_TUNER_Init(void)
{
    DRV_TUNER_MUTEX_INIT

    DRV_TUNER_MUTEX_LOCK


    x_memset(&_rTunerParam, 0, sizeof(DRV_TUNER_PARAM_T));
    x_memset(&_rTunerInterParam, 0, sizeof(DRV_TUNER_PARAM_T));
    
    _rTunerState = DRV_TUNER_STATE_DISCONNECTED;
    VERIFY(x_msg_q_create(&_hTunerMsgq, "M_TUNER", sizeof(DRV_TUNER_MSG_T), 64) == OSR_OK);    
    VERIFY(x_thread_create(&_hTunerThread, "T_TUNER", 2048, 253, _TunerThread, 0, NULL) == OSR_OK); 

    //Tuner_SetAnaNfy(_ANA_SigNotify);
    
    VERIFY(DMX_Init());     

#ifdef CC_DVBT_SUPPORT
    VERIFY(DMX_SelectExternalTuner());
#endif

#if !TUNER_ADC_WORKAROUND     
    TunerOpen();            
#endif
    TDIS_RetrieveTunerCfgTbl(&_rTunerCfgTbl);   

    DRV_TUNER_MUTEX_UNLOCK

    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_TUNER_SetParameter(DRV_TUNER_PARAM_T* prParam)
{
    DRV_TUNER_MUTEX_LOCK    

    x_memcpy(&_rTunerInterParam, prParam, sizeof(DRV_TUNER_PARAM_T));

    /*
    _rTunerParam.rAnaParam.fgAfcEnabled = prParam->rAnaParam.fgAfcEnabled;
    _rTunerParam.rAnaParam.pfnTuningDone = prParam->rAnaParam.pfnTuningDone;
    _rTunerParam.rAnaParam.rTvSys = prParam->rAnaParam.rTvSys;

    _rTunerParam.rDigParam.fgSpectrumInversion = prParam->rDigParam.fgSpectrumInversion;
    _rTunerParam.rDigParam.pfnTuningDone = prParam->rDigParam.pfnTuningDone;
    _rTunerParam.rDigParam.pfnLockStatusChange = prParam->rDigParam.pfnLockStatusChange;
    _rTunerParam.rDigParam.rBandwidth = prParam->rDigParam.rBandwidth;
    _rTunerParam.rDigParam.rMod = prParam->rDigParam.rMod;
    _rTunerParam.rDigParam.u4QamSize = prParam->rDigParam.u4QamSize;
    _rTunerParam.rDigParam.u4SymbolRate = prParam->rDigParam.u4SymbolRate;    
    */
    
    DRV_TUNER_MUTEX_UNLOCK        

    return DRV_E_OK;    
}

DRV_ERR_CODE_T DRV_TUNER_SetFreq(UINT32* pu4Freq, DRV_TUNER_CONN_TYPE_T rType, DRV_TV_SYS_T rTvSys, BOOL fgAutoFineTune, void* pvTag)
{
    DRV_ERR_CODE_T rRetCode = DRV_E_FAIL;    
    
    DRV_TUNER_MUTEX_LOCK    

    x_memcpy(&_rTunerParam, &_rTunerInterParam, sizeof(DRV_TUNER_PARAM_T));    
    
    rRetCode = _SetFreq(pu4Freq, rType, rTvSys, fgAutoFineTune, pvTag);
    
    DRV_TUNER_MUTEX_UNLOCK
    
    return rRetCode;
}

DRV_ERR_CODE_T DRV_TUNER_GetFreq(UINT32* pu4Freq, INT32 i4Tag)
{
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_TUNER_GetLockStatus(DRV_TUNER_LOCK_STATUS_T* prStatus, 
	INT32 i4Tag)
{
    *prStatus = (DRV_TUNER_LOCK_STATUS_T)(((UINT32)i4Tag) >> 24);
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_TUNER_GetInfo(DRV_TUNER_INFO_T* prInfo)
{
    return DRV_E_OK;
}
