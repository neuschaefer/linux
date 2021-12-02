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
 * $Author: xiongfeng.wen $
 * $Date: 2012/08/10 $
 * $RCSfile: pd_atdeu_glue.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file pd_atdeu_glue.c
 *  Analig Tuner LTDIS Interface
 */

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/

#include "PI_Def_dbg.h"
#include "x_rm_dev_types.h"
#include "TDIS.h"
#include "queue.h"
#include "TDMgr.h"
#include "util.h"
#include "drvapi_tuner.h"
#include "pd_tuner_type.h"
#include "source_table.h"
#include "UtilAPI.h"
#include "pd_atdeu_glue.h"
//#include "pd_atdeu_if.h"
#include "atd_feature.h"
#include "pi_demod_atd.h"
#include "nptv_drvif.h"
#include "drv_tvd.h"
#include "aud_drvif.h"
#include "pi_demod_atdeu.h"
#include "x_gpio.h"
//#include "ctrl_bus.h"

// Justin TODO, removed in 5363
#if defined (CC_MT5360)
#include "pi_atdeu_tvsys_info.h"
#include "pi_atdeu_tvsys_info.c"
#define fgGetTVDSts(para1_skip,x)  AtdPIEU_fgDrvTunerCheckTVDLock(x, psAtdPdCtx->u1SubSysIdx)
#endif

// ZY, 091222
#include "api_srch_if.h"

/***********************************************************************/
/*              Conditional Compiling                                  */
/***********************************************************************/

//#if !defined(CC_MTAL) && !defined(CC_SDAL) //linux platform
//#define ANAEU_ENABLE_MONITOR
//#endif

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define cMONITOR_PERIOD_MS      500
#define fcADD_TVD_SYNC          1           // Pause TVD before TunerAcq complete, or will show "No Signal"
#define fcADD_AV_SYNC           1
#define fcADD_AV_NOTIFY         1

//for new scan step
#define cNEXT_STEP_TVD_UNLOCK           1500000 //Hz, original value
#define cNEXT_STEP_TVD_LOCK_NOISE_HIGH  2000000 //Hz
#define cNEXT_STEP_TVD_LOCK_PAL_BG      3000000 //Hz, 5.5M + 1.5M
#define cNEXT_STEP_TVD_LOCK_PAL_I       3000000 //Hz, 6.0M + 2.0M  
#define cNEXT_STEP_TVD_LOCK_PAL_DK      3000000 //Hz, 6.5M + 1.5M  
#define cNEXT_STEP_TVD_LOCK_SECAM_L     3000000 //Hz, 6.5M + 1.5M  
#define cNEXT_STEP_TVD_LOCK_SECAM_L1    3000000 //Hz, for auto scan is scan-up process, SIF < VIF
#define cNEXT_STEP_TVD_LOCK_NTSC_M      3000000 //Hz, 4.5M + 1.5M
#define cNEXT_STEP_TVD_UNLOCK_OLD       2000000 //Hz, original value
#define cNEXT_STEP_TVD_LOCK_OLD         3000000 //Hz, original value

#define cRISC_TVD_BASE           (IO_VIRT + 0x22000)
//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
UINT16 DeScrambleBoxDelay = 0;
BOOL DeScrambleBox = FALSE;
ATD_CTX_T*    psAtdeuDemodCtx=NULL;
#if fcTIMING_MEASURE
UINT32  u2TickStart_eu;//jackson: todo, move to a control block
#endif
UINT16 g_CPR_delay=0;
UINT16 g_CPR_cnt=0;
UINT16 g_pra_cnt=0;
UINT16 g_CR_BW_AutoConfig_Delay_cnt=3;
UINT8 g_bypass_pilotsearch;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static TUNER_DEVICE_CTX_T*		pAtdTunerDeviceCtx = NULL;
static BOOL     fgMonitorThreadRunning = FALSE;
static THREAD_T t_monitor_thread;
static MUTEX_T  t_escape_atdeu_mon_suspend;
//static UINT8 u1MonitorLoopCnt = 0;
static UINT8 fgAtdMonitorRet = FALSE;
#if defined(ATD_SNOWSCREEN_MODE)
static UINT8 fgAtdAFTFailCnt = 0;
#endif
#if defined(ATD_NTSC443_DETECT)
static UINT32 u4PreFreq = 0;
static BOOL fgSameFreq = FALSE;
#endif
static ITUNER_CTX_T *pTunerCtx;
static U8    fgNewScanStep = TRUE;
static BOOL   fgTvdLock=FALSE;
//static BOOL   fgAudScan = FALSE;   
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM 
static U8     u1ChChangeInitSys = MOD_PAL_BG; 
#endif
static UINT16 u2DroTarLvlInitial =0;
//static U8    LevelSwitchTogCnt = 0;

STATIC TUNER_ANA_ATTRIBUTE_T sTunerAnaAttribute =
{
    43250000,
    873225000,
    (90 / 2) * 50000
};

STATIC UINT32  au4TvSysGroup[] =
{
    (TV_SYS_MASK_B | TV_SYS_MASK_G),
    (TV_SYS_MASK_D | TV_SYS_MASK_K),
    (TV_SYS_MASK_I),
    (TV_SYS_MASK_L),
    (TV_SYS_MASK_L_PRIME),
    (TV_SYS_MASK_M | TV_SYS_MASK_N)
};

/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
#ifdef EU_AUTO_ATP
extern UINT8 gATP_TEST_ITEM_BIT ;
extern UINT8 gATPFailBitMsk ;
#endif

extern UINT8 bGetSignalType(UINT8 bPath);
extern int  snprintf(char *s, size_t n, const char * format, ...);

//#if defined(CC_DRIVER_DEMO) || defined(CC_LINUX_KERNEL)
//#define _Tvd_NotifyTVSig(u1Path, u1SigState)  // for those system has no MW
//#else
//extern VOID _Tvd_NotifyTVSig(UINT8 u1Path, UINT8 u1SigState);
//#endif
#define AtdPd_TvdNotify(u1Path, u1SigState)  vApiNotifySigChg(u1Path, u1SigState, 1)

#if fcADD_AV_SYNC
// Henry Added for AV synchronization
EXTERN VOID AUD_WaitDspFlush(UINT8 u1DecId);
EXTERN VOID AUD_TriggerDspPlay(UINT8 u1DecId);
EXTERN VOID Aud_SetTvSysMask(UINT8 u1DecId, UINT32 u4Mask);
#endif

#if fcADD_AV_NOTIFY
EXTERN VOID AUD_AtunerSigStableNotify(BOOL fgSatble);
#endif

#if fcADD_TVD_SYNC
EXTERN VOID vTvdHoldMode(UINT8 bHoldOnOff);
#endif
// Add it for tunerall to choose a tuner type by I2C read,jack mei,09-11-25
#ifdef I2C_TUNER_ALL
EXTERN UINT8 ICtrlBus_I2cTunerRead(UINT16 u2ClkDiv,UINT8 i2cAddr,UINT8 *data, UINT16 len);
#endif

/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
VOID Tuner_AtdEU_Register_LTDIS_Fct(PTD_CTX ptTDCtx);
VOID AtdEU_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv);


/*move this API to correct location*/
static INT32 TunerNullBreak(VOID* pArg)
{
    UNUSED(pArg);

    return (FALSE);
}

/***********************************************************************/
/*              Static Functions                                       */
/***********************************************************************/
/************************************************************************
*  AtdPd_CheckTVDLock
*  Check if TV decoder is lock or not at current frequency.
*  @param  psAtdPdCtx : Pointer use to point to struct ATD_PD_CTX_T.
*  @param  u4Freq : Current frequency value in KHz.
*  @param  cMsgInd : Message ID.
*  @retval  TRUE : TV decoder lock.
*  @retval  FALSE : TV decoder unlock.
************************************************************************/
// Justin TODO, why CID implement TVD lock function?
static BOOL AtdPd_CheckTVDLock(ATD_PD_CTX_T *psAtdPdCtx, UINT32 u4Freq, CHAR cMsgInd)
{
BOOL    fgLockSts = TRUE;
    #if fcTIMING_MEASURE
HAL_TIME_T  rCurTime={0,0};
	
mcDELAY_MS(DeScrambleBoxDelay);

//UINT8 bVpressChkTime;

//UINT8 ii=0, jj=0;
    HAL_GetTime(&rCurTime);
    mcSHOW_DBG_MSG(("%%-->%c atv-scan ChkTVD[%d  %d]\n", rCurTime.u4Seconds, rCurTime.u4Micros, cMsgInd));
    #endif

	if ((bGetSignalType(SV_VP_MAIN) != (UINT8) SV_ST_TV)&&(bGetSignalType(SV_VP_PIP) != (UINT8) SV_ST_TV))//NOT tv source ,aways return TVD lock,liuyuan,091222
		{fgLockSts = TRUE;
	    mcSHOW_DBG_MSG(("NOT TV SOURCE,NOT TO DO TVD CHECK!"));
	    return fgLockSts;
		}

/* do TVD check when auto search find VIF, LC Chien 070302*/
    //if (AtdPIEU_fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME, psAtdPdCtx->u1SubSysIdx))
    if (fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME))
    {
	#if TVD_SUPPORT_SNOW_MODE
	ATD_SetSnowMode(psAtdeuDemodCtx,FALSE);//d20110718_Haibo:Disable TVD snow mode after TVD lock
	#endif
    #if defined(ATD_NTSC443_DETECT)
    //if ((psAtdeuDemodCtx->u1TunerType == cALPS_TEQE3) || (psAtdeuDemodCtx->u1TunerType == cALPS_TEQE3_SUNBOARD))
    {
        //UINT32 u4Temp;
        UINT8 ucBurstFreq, ucBurstFreq2, ucData;

        if(psAtdPdCtx->u1SubSysIdx >= MOD_NTSC_M) //first detect TVD lock
        {
            mcDELAY_MS(50);
            ucBurstFreq = bDrvTvdCheckBurstFreq();

            if((ucBurstFreq == TVD_BLOCK_IS358) && fgHwTvd525())//d111107_Haibo:Add line num check
            {
                fgLockSts = TRUE;
                mcSHOW_DBG_MSG(("Burst 358 is detected and line num is 525!\n"));
            }
            else if (fgSameFreq)
            {
                fgLockSts = TRUE;
                mcSHOW_DBG_MSG(("Burst unknow but retry M!\n"));
            }
            else //if(ucBurstFreq == TVD_BLOCK_IS358)
            {

		  psAtdPdCtx->u1SubSysIdx = MOD_PAL_BG;
		  psAtdeuDemodCtx->u1SubSysIdx = MOD_PAL_BG;// add by liuqu for update the sys idx!
                ATD_SetFreq(psAtdeuDemodCtx, u4Freq, TRUE);
                ATD_SetSystem(psAtdeuDemodCtx, MOD_PAL_BG);
                ATD_StartAcq(psAtdeuDemodCtx, TRUE);
                vTvd3dReset();
                mcDELAY_MS(200);
				mcDELAY_MS(DeScrambleBoxDelay);
                mcSHOW_DBG_MSG(("Current DeScrambleBoxDelay = %d \n",DeScrambleBoxDelay));
                ucBurstFreq2 = bDrvTvdCheckBurstFreq();

                if(ucBurstFreq2 == TVD_BLOCK_IS443)
                {
                    fgLockSts = TRUE;
                    mcSHOW_DBG_MSG(("Burst 443 is detected!\n"));
                }
                else if(bTvd3dGetColorSystem()==SV_CS_SECAM)
                {
                    fgLockSts = TRUE;
                    mcSHOW_DBG_MSG(("SECAM is detected!\n"));
                }    
                else
                {
                    ATD_GetReg(psAtdeuDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
                    ucData = ucData & 0xF7;
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
                    fgLockSts = TRUE;
                    mcSHOW_DBG_MSG(("Burst unknown!\n"));
                }
            }            
        }
        /*else if(psAtdPdCtx->u1SubSysIdx <= MOD_PAL_I)
        {
            if(ucBurstFreq == TVD_BLOCK_IS443)
            {
                fgLockSts = TRUE;
                mcSHOW_DBG_MSG(("Burst 443 is detected!\n"));
            }
            else if(bTvd3dGetColorSystem()==SV_CS_SECAM)
            {
                fgLockSts = TRUE;
                mcSHOW_DBG_MSG(("SECAM is detected!\n"));
            }    
            else
            {
                 ATD_GetReg(psAtdeuDemodCtx, 0x9C1, &ucData, 1);
                 ucData = ucData & 0xF7;
                 ATD_SetReg(psAtdeuDemodCtx, 0x9C1, &ucData, 1);
                 fgLockSts = TRUE;
                 mcSHOW_DBG_MSG(("Burst unknown!\n"));
            }
        }*/
        else
        {
            fgLockSts = TRUE;
        } 
    }
    #else
    //else
    {
            fgLockSts = TRUE;               //move to Thomson/tuner.c
    }
    #endif
    #if fcTIMING_MEASURE
        HAL_GetTime(&rCurTime);
        mcSHOW_DBG_MSG(("%%<--%c atv-scan ChkTVD[%d  %d], TVD lock!\n", rCurTime.u4Seconds, rCurTime.u4Micros, cMsgInd));
    #endif
    }
    else
    {
    #if fcTIMING_MEASURE
        HAL_GetTime(&rCurTime);
        mcSHOW_DBG_MSG(("%%<--%c atv-scan ChkTVD[%d  %d], TVD unlock!\n", rCurTime.u4Seconds, rCurTime.u4Micros, cMsgInd));
    #endif
        fgLockSts = FALSE;
    }

    return fgLockSts;
}

/************************************************************************
*  AtdPd_ChangeChannel
*  Flush audio DSP once channel is changed.
*  @param  u4CurFreqency : Current frequency value in Hz.
*  @retval  void.
************************************************************************/
STATIC VOID AtdPd_ChangeChannel(UINT32 u4CurFreqency)
{
#ifndef CC_MT5391_AUD_3_DECODER
#if fcADD_AV_SYNC
STATIC UINT32 u4PreFreqency = 0;
BOOL fgSameChannel;

    fgSameChannel = (u4PreFreqency == u4CurFreqency);
    u4PreFreqency = u4CurFreqency;

    //AUD_DualDecAtvContinuousPlay(fgSameChannel);
    if (!fgSameChannel)
        AUD_WaitDspFlush(1);
#endif
#endif
}

/************************************************************************
*  AtdPd_ExitTunerAcq
*  Notify audio DSP once tuner acquisition is finished.
*  @param  fgLocked : Current tuner lock status.
*  @retval  void.
************************************************************************/
STATIC VOID AtdPd_ExitTunerAcq(BOOL fgLocked)
{
#if fcADD_AV_NOTIFY
    AUD_AtunerSigStableNotify(fgLocked);
#endif
#if fcADD_TVD_SYNC
    vTvdHoldMode(SV_OFF);
#endif
}


/************************************************************************
*  AtdEU_AudSetSystem
*  AUD set demod system to detect system type
*  @param  u1SubSysIdx: system type
*  @param  u1ChMode: 0:scan mode,   1:normal mode
*  @retval none 
************************************************************************/
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
void AtdEU_AudSetSystem(UINT8 u1SubSysIdx, UINT8 u1ChMode)
{
    //ATD_SetSystem(psAtdeuDemodCtx, u1SubSysIdx);
	
	if(u1ChMode==0)  // Scan mode
	{		
		psAtdeuDemodCtx->fgAudScan = TRUE;
		//ATD_SetSystem(psAtdeuDemodCtx, u1SubSysIdx);
		ATD_SetSystemAudio(psAtdeuDemodCtx, u1SubSysIdx);
		//ATD_AudSetSystemAudio(psAtdeuDemodCtx, u1SubSysIdx);		
	    ATD_SetScanMode(psAtdeuDemodCtx);
	    //fgAudScan = TRUE;
	}
	else // normal mode
	{
		psAtdeuDemodCtx->fgAudScan = FALSE; 
		if (u1ChChangeInitSys !=  u1SubSysIdx)
		{
		  ATD_SetSystem(psAtdeuDemodCtx, u1SubSysIdx);
		  /*
		  psAtdeuDemodCtx->u1SubSysIdx = u1SubSysIdx;    
          ATD_SetSystemAudio(psAtdeuDemodCtx, u1SubSysIdx);
          ATD_SetSystemVideo(psAtdeuDemodCtx, u1SubSysIdx);
          ATD_SetEQ(psAtdeuDemodCtx, itGetEqScriptNormal);
          ATD_SetIF(psAtdeuDemodCtx, u1SubSysIdx);   
          ATD_SetAgc(psAtdeuDemodCtx);			  
          */
		}  
		else
		{	
		  ATD_SetSystemAudio(psAtdeuDemodCtx, u1SubSysIdx);	
		}  
		ATD_SetNormalMode(psAtdeuDemodCtx);
		u1ChChangeInitSys = u1SubSysIdx;
		//fgAudScan = FALSE; 		
	}				
	
	if(psAtdeuDemodCtx->fgAudScan==FALSE)
		mcSHOW_DBG_MSG(("AUD Sys Detect Done, u1SubSysIdx = %\n", u1SubSysIdx));
	
}
#endif
/************************************************************************
*  GetAtdConnMode
*  Get current analog connection mode.
*  @param  eMod : Connection mode parameter.
*  @retval  ATV_CONN_MODE_CHAN_SCAN : Analog channel scan.
*  @retval  ATV_CONN_MODE_FINE_TUNE : Analog frequency fine-tune.
*  @retval  ATV_CONN_MODE_CHAN_CHANGE : Analog channel change.
************************************************************************/
/***********************************************************************/
/*
    eMod:
    MOD_PSK_8 --> Channel Change; _fgAutoSearch = FALSE
    MOD_UNKNOWN --> Scan; _fgAutoSearch = TRUE
    MOD_QAM_896 --> First in Scan; _fgAutoSearch = TRUE
    MOD_QAM_1024 --> Fine-tune (EU mode); _fgAutoSearch = TRUE
    
    u4AutoSearchType:
    0 --> Channel Change;
    1 --> Scan;     
    2 --> Fine-tune (EU mode);     
*/

STATIC UINT8 GetAtdConnMode(UINT8 eMod)
{
UINT8   u1ConnMode = 0, ucData1;
UINT32	u4AutoSearchType = 0;
    
    mcSHOW_DBG_MSG(("eMod = %d\n", eMod));   
    
    u4AutoSearchType = u4GetAutoSearchType();
    mcSHOW_DBG_MSG(("u4AutoSearchType = %d\n", u4AutoSearchType));   
    
    // ZY, 091222, u1ConnMode = u4AutoSearchType, except FM radio mode
    switch (u4AutoSearchType)
    {           	
        case ATV_CONN_MODE_CHAN_CHANGE:      
            u1ConnMode = ATV_CONN_MODE_CHAN_CHANGE;
            ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1); 
            mcSHOW_DBG_MSG(( "Connection_type = channel change!\n"));
            break;
                	
        case ATV_CONN_MODE_CHAN_SCAN:                       
            u1ConnMode = ATV_CONN_MODE_CHAN_SCAN;
            ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1);            
            mcSHOW_DBG_MSG(( "Connection_type = channel scan!\n"));
            break;
        
        case ATV_CONN_MODE_FINE_TUNE:                    
            u1ConnMode = ATV_CONN_MODE_FINE_TUNE;
            ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1);             
            mcSHOW_DBG_MSG(( "Connection_type = fine tune!\n"));
            break; 
        default:
            break;
    }                    
    
    // check eMod for FM radio type
    switch (eMod)
    {
/*    	
        case MOD_UNKNOWN:
        case MOD_QAM_896:
            //_fgAutoSearch = TRUE;
            u1ConnMode = ATV_CONN_MODE_CHAN_SCAN;
            ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1);            
            mcSHOW_DBG_MSG(( "Mod_type = channel scan!\n"));
            break;
 
        case MOD_QAM_1024:
            //_fgAutoSearch = TRUE;
            u1ConnMode = ATV_CONN_MODE_FINE_TUNE;
            ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1);             
            mcSHOW_DBG_MSG(( "Mod_type = fine tune!\n"));
            break;
        
        case MOD_PSK_8:
           // _fgAutoSearch = FALSE;
            u1ConnMode = ATV_CONN_MODE_CHAN_CHANGE;
            ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1); 
            mcSHOW_DBG_MSG(( "Mod_type = channel change!\n"));
            break;
*/            
#ifndef CC_MT5360
        case MOD_FM_RADIO:
            //_fgAutoSearch = FALSE;
            u1ConnMode = ATV_CONN_MODE_FM_RADIO;
            ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1);  
            mcSHOW_DBG_MSG(( "Mod_type = FM radio!\n"));
            break;
#endif        
        default:
            break;
    }
        
    return u1ConnMode;
}


//#ifdef ANAEU_ENABLE_MONITOR
static BOOL SetBreakMonitor(ATD_PD_CTX_T *psAtdPdCtx)
{
    UINT8 u1Cnt=0;
    if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        psAtdPdCtx->fgBreakMonitor = TRUE; //set to break monitor
    
        if(psAtdPdCtx->fgPIMoniStatus){  //PI really doing PI Monitor
            while(psAtdPdCtx->fgPIMoniStatus){
                //wait PI Monitor really finish break
                if(u1Cnt>250){  //in case of deadlock
                    break;
                }
                u1Cnt++;
                mcSHOW_DBG_MSG(("ATD EU PD wait break PI monitor finish. u1Cnt=%d\r\n",u1Cnt));
                mcDELAY_MS(10);
            }
        }
        mcSHOW_DBG_MSG(("Disable monitor process!!\n"));
    }
    return TRUE;
}
static void ResumeMonitor(ATD_PD_CTX_T *psAtdPdCtx)
{
    if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        #if defined(ATD_SNOWSCREEN_MODE)
        fgAtdAFTFailCnt = 0;
        fgAtdMonitorRet = FALSE;
        #endif
        psAtdPdCtx->fgBreakMonitor = FALSE;
        mcMUTEX_UNLOCK(t_escape_atdeu_mon_suspend);
    }
}

/************************************************************************
*  i4AtdEUMonitorBreak
*  Break function of monitor thread.
*  @param  *pvArg : A pointer to ATD_PD_CTX_T structure.
*  @retval  Break flag of break function.
************************************************************************/
STATIC INT32 i4AtdEUMonitorBreak(VOID *pvArg)
{
    ATD_PD_CTX_T *psPdCtx = (ATD_PD_CTX_T *) pvArg;

    if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        if (psPdCtx->fgBreakMonitor)
        {
            mcSHOW_DBG_MSG(("ATD_EU_Monitor break function triggered!!\n"));
        }
    }
    return (psPdCtx->fgBreakMonitor);
}

/************************************************************************
*  MonitorProcess
*  Monitor thread process function.
*  @param  void.
*  @retval  void.
************************************************************************/
STATIC void MonitorProcess(VOID *pvArg)
{

    ATD_PD_CTX_T *psPdCtx = *(ATD_PD_CTX_T **) pvArg;

    if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {

    mcSHOW_DBG_MSG(("MonitorProcess start!\n"));

    #if defined(ATD_SNOWSCREEN_MODE)
    UINT8 ucData1, ucVpress;
    #endif 
    
    while (fgMonitorThreadRunning)
    {
    	if (psPdCtx->fgBreakMonitor)
    	{
    		// After mcMUTEX_LOCK(t_escape_anaeu_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
            mcSHOW_DBG_MSG(("Entering ATD_EU mon_suspend\n"));
    		mcMUTEX_LOCK(t_escape_atdeu_mon_suspend);
    		mcSHOW_DBG_MSG(("Escape ATD_EU mon_suspend\n"));
    	}
    	else
    	{
		    // mutex lock for I2C access
            mcSEMA_LOCK_HAL(psPdCtx->hHalLock);
	        mcSHOW_DBG_MSG4(("Invoke  ATD_EU_Monitor\n"));  //liuqu change debug level

            //add PI monitor API here
			psPdCtx->fgPIMoniStatus = TRUE; //doing PI monitor
            #if defined(ATD_SNOWSCREEN_MODE)
            
            if(fgAtdAFTFailCnt == 1)
            {                                
                /*psAtdeuDemodCtx->fgCheckCR = TRUE;
                if(ATD_GetCRLock(psAtdeuDemodCtx, TRUE) == TRUE)               
                {                    
                    mcSHOW_DBG_MSG(("CR lock one time, retry again!!!!!!!!!!!!!!!!!!!!\n"));
                    ATD_GetReg(psAtdeuDemodCtx, 0x935, &ucData1, 1);
                    ucData1 = ucData1 & 0xFE;
                    ATD_SetReg(psAtdeuDemodCtx, 0x935, &ucData1, 1);
                    ucData1 = ucData1 | 0x01;
                    ATD_SetReg(psAtdeuDemodCtx, 0x935, &ucData1, 1);
                    
                    mcDELAY_MS(100);
                    if(ATD_GetCRLock(psAtdeuDemodCtx, TRUE))
                    {
                        mcSHOW_DBG_MSG(("Find new possible VIF!\n"));
                        ATD_SetSystem(psAtdeuDemodCtx, psPdCtx->u1SubSysIdx);
                        ATD_SetPF(psAtdeuDemodCtx, 0x01);
                        ATD_StartAcq(psAtdeuDemodCtx, FALSE);
                        fgAtdAFTFailCnt = 0;	
                    }
                }*/
                ucVpress = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4;
                if(ucVpress == 0x01)
                {
                    ucData1 = 0x08;
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_POA_CFG, &ucData1, 1);
                    ucData1 = 0x04;                    
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_PE_CFOK, &ucData1, 1);
                    ucData1 = 0x21;                    
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_PS_CFG, &ucData1, 1);
                    //ucData1 = 0x8C;                    
                    //ATD_SetReg(psAtdeuDemodCtx, 0x999, &ucData1, 1);
                    //ucData1 = 0x27;                    
                    //ATD_SetReg(psAtdeuDemodCtx, 0x9B4, &ucData1, 1);                    
                    psAtdeuDemodCtx->u1PF = 0x01;                    
                    ATD_SetNormalMode(psAtdeuDemodCtx);
                    mcSHOW_DBG_MSG(("VPress lock, restart AFT!!!!!!!!!!!!!!!!!!!!\n"));
                    fgAtdAFTFailCnt = 0;	
                }
				else//d20110719_Haibo:reset CR when no signal
				{
				    ucData1 = 0xFE;
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_CTL_RST2, &ucData1, 1);
                    ucData1 = 0xFF;                    
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_CTL_RST2, &ucData1, 1); 
				}
            }

            if(fgAtdAFTFailCnt == 0){
                fgAtdMonitorRet = ATDEU_Monitor(psAtdeuDemodCtx, &psPdCtx->Frequency, i4AtdEUMonitorBreak, psPdCtx);          		
            }

            if (fgAtdMonitorRet) 
            {
                ATD_Patch(psAtdeuDemodCtx);
 #if !ATD_SUPPORT_DRO_SYNC_MODE && !ATD_SUPPORT_H_BLANK_MODE
					if (fgHwTvdVPresTVD3D())
					{
					ATD_DroTargetLevelAdjust(psAtdeuDemodCtx,u2DroTarLvlInitial);
					}
 #else
                    if(IC_VER_5396_AA==BSP_GetIcVersion())//MT5396 before ECO)
					{
					if (fgHwTvdVPresTVD3D())
					   {
					     ATD_DroTargetLevelAdjust(psAtdeuDemodCtx,u2DroTarLvlInitial);
					   } 
					}
#endif				
            }            
            else
            {
                if(fgAtdAFTFailCnt == 0)
                {                     
                    mcSHOW_DBG_MSG(("1st time monitor fail!\n"));                    
                    //ATD_SetSystem(psAtdeuDemodCtx, psPdCtx->u1SubSysIdx);                    
                    //ATD_SetFreq(psAtdeuDemodCtx, psPdCtx->Frequency, _fgAutoSearch);

                    //ATD_SetPF(psAtdeuDemodCtx, 0x02);
                    //ATD_StartAcq(psAtdeuDemodCtx, FALSE);                    
                    ucData1 = 0x00;
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_POA_CFG, &ucData1, 1);
                    ucData1 = 0x09;                    
                    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_ACR_CFG, &ucData1, 1);
                    //ucData1 = 0x26;                    
                    //ATD_SetReg(psAtdeuDemodCtx, 0x9B4, &ucData1, 1);
                    //ucData1 = 0xCC;                    
                    //ATD_SetReg(psAtdeuDemodCtx, 0x999, &ucData1, 1);
                    fgAtdAFTFailCnt++;
                }
            }    

            #else            
            // do ATDEU_Monitor every 500ms
            //u1MonitorLoopCnt++;
            //if (u1MonitorLoopCnt >= 5){
            //    u1MonitorLoopCnt = 0;
                fgAtdMonitorRet = 
                    ATDEU_Monitor(psAtdeuDemodCtx, &psPdCtx->Frequency, i4AtdEUMonitorBreak, psPdCtx);
            //}

            // do ATD_Patch every 100ms            
          #ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
            if (fgAtdMonitorRet==1 && psAtdeuDemodCtx->fgAudScan==FALSE) 
          #else	
            if (fgAtdMonitorRet) 
          #endif
            {  	
			 #if defined(TVD_SNOWSCREEN_MODE)  
			  ATD_SetSnowMode(psAtdeuDemodCtx,FALSE);//d20110719_Haibo:When AFT ok.Disable snow mode
            #endif			 

                    ATD_Patch(psAtdeuDemodCtx);
					 #if !ATD_SUPPORT_DRO_SYNC_MODE && !ATD_SUPPORT_H_BLANK_MODE
					if (fgHwTvdVPresTVD3D())
					{
					ATD_DroTargetLevelAdjust(psAtdeuDemodCtx,u2DroTarLvlInitial);	
					}
					#else
					if(IC_VER_5396_AA==BSP_GetIcVersion())//MT5396 before ECO)
					{
					if (fgHwTvdVPresTVD3D())
					   {
					     ATD_DroTargetLevelAdjust(psAtdeuDemodCtx,u2DroTarLvlInitial);
					   } 
					}
					#endif
            }    
            #endif
			psPdCtx->fgPIMoniStatus = FALSE; //finish doing PI monitor or breaked
            mcSEMA_UNLOCK_HAL(psPdCtx->hHalLock);
	        mcDELAY_MS(cMONITOR_PERIOD_MS);
        }
    }

    t_monitor_thread = (THREAD_T)NULL;
    mcSHOW_DBG_MSG(("MonitorProcess exit!\n"));
    mcTHREAD_DESTROY();

    }
}

/************************************************************************
*  PD_StartMonitorEngine
*  Function which starts monitor thread engine.
*  @param  *psAtdPdCtx : A pointer to ATD_PD_CTX_T structure.
*  @retval  DRVAPI_TUNER_OK : Monitor thread create successfully.
*  @retval  DRVAPI_TUNER_ERROR : Monitor thread create fail.
************************************************************************/
static S32 PD_StartMonitorEngine(ATD_PD_CTX_T *psAtdPdCtx)
{
    //ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;

    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not
    // directly use pv_arg. Therefore, if we want thread to share the same
    // context as caller, we need set pv_arg as "pointer of context".
   	if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{

    fgMonitorThreadRunning = TRUE;
    if (mcTHREAD_CREATE(&t_monitor_thread,
            "EU_ATD_MonThread",
            cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
            MonitorProcess, sizeof(void*), (VOID *) &psAtdPdCtx) != OSR_OK)
    {
        mcSHOW_DBG_MSG(("PD_StartMonitorEngine (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSHOW_DBG_MSG(("PD_StartMonitorEngine success\n"));

    }
    return (DRVAPI_TUNER_OK);
}

/************************************************************************
*  PD_StopMonitorEngine
*  Function which stops monitor thread engine.
*  @param  void.
*  @retval  DRVAPI_TUNER_OK : Stop monitor thread successfully.
************************************************************************/
static S32 PD_StopMonitorEngine(void)
{
    if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	  {
        //fgMonitorThreadRunning = FALSE;
        while (t_monitor_thread){
            mcDELAY_MS(10); // in ms
        }
        mcSHOW_DBG_MSG(("ATD EU PD_StopMonitorEngine success\n"));
    }
    return (DRVAPI_TUNER_OK);
}
//#endif //ANAEU_ENABLE_MONITOR

/************************************************************************
*  vSetTvSubSysIdx
*  Set specified TV system mask into TV system index of PD context.
*  @param  *psAtdPdCtx : A pointer to ATD_PD_CTX_T structure.
*  @param  u4TvSubSysMask : TV system mask.
*  @retval  TRUE : Current TV system is changed.
*  @retval  FALSE : Current TV system is not changed.
************************************************************************/
STATIC UINT8 vGetTvSubSysIdx(ATD_PD_CTX_T *psAtdPdCtx, UINT32  u4TvSubSysMask)
{
UINT8   u1SubSysIdx = MOD_NTSC_M;

    mcSHOW_DBG_MSG(("%s: u4TvSubSysMask = %04X\n", __FUNCTION__, u4TvSubSysMask));

    if (u4TvSubSysMask & (TV_SYS_MASK_B | TV_SYS_MASK_G))
        //u1SubSysIdx = SYS_SUB_PAL_BG;
        u1SubSysIdx = MOD_PAL_BG;
    else if (u4TvSubSysMask & (TV_SYS_MASK_D | TV_SYS_MASK_K))
        //u1SubSysIdx = SYS_SUB_PAL_DK;
        u1SubSysIdx = MOD_PAL_DK;
    else if (u4TvSubSysMask & (TV_SYS_MASK_I))
        //u1SubSysIdx = SYS_SUB_PAL_I;
        u1SubSysIdx = MOD_PAL_I;
    else if (u4TvSubSysMask & (TV_SYS_MASK_L))
        //u1SubSysIdx = SYS_SUB_SECAM_L;
        u1SubSysIdx = MOD_SECAM_L;
    else if (u4TvSubSysMask & (TV_SYS_MASK_L_PRIME))
        //u1SubSysIdx = SYS_SUB_SECAM_L1;
        u1SubSysIdx = MOD_SECAM_L1;

    mcSHOW_DBG_MSG(("%s: u1SubSysIdx = %d\n", __FUNCTION__, u1SubSysIdx - MOD_ANA_TYPE_BEGIN));

    return (u1SubSysIdx);
}

/*STATIC void SetFMRadio(UINT32  i4CurFreq)
{
    UINT8 ucData1;
    //UINT32 u4Temp;
    
    //u4Temp = 0xffffffff;
    //IO_WRITE32(cRISC_TVD_BASE, 0x40C, u4Temp);
    //ATD_ChipInit(psAtdeuDemodCtx);
    //ATD_SetSystem(psAtdeuDemodCtx, MOD_PAL_BG);
    psAtdeuDemodCtx->u1PF = 0x00;
    ucData1 = 0x6D;
    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_PJ_CFG, &ucData1, 1);
    ucData1 = 0xC9;
    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_PFLL_CFG, &ucData1, 1);
    ucData1 = 0x00;
    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_CPR_CFO_CFG, &ucData1, 1);
    ucData1 = 0x00;
    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_AVS_AOH, &ucData1, 1);
    ucData1 = 0x20;
    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_AVS_AOL, &ucData1, 1);
    ucData1 = 0x81;
    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_AUC_CFG1, &ucData1, 1);
    ucData1 = 0xA1;
    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_AUC_CFG2, &ucData1, 1);                                
    
    //SetBreakMonitor(psAtdPdCtx);
    //ATD_SetLNA(psAtdeuDemodCtx, TRUE);
    ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, 0, &ucData1);
    //ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
    //ATD_StartAcq(psAtdeuDemodCtx, FALSE);    
    mcSHOW_DBG_MSG((" Enable ATD CR bypass mode for FM radio\n")); 

}
*/
/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/

UINT32 AtdEU_GetNoiseLevel(void)
{
    return ATDPI_GetNoiseLevel(psAtdeuDemodCtx);
}
UINT16 AtdEU_GetIFAGC(void)
{
    return ATDPI_GetIFAGC(psAtdeuDemodCtx);
}
BOOL AtdEU_SetLNA(BOOL fgLNA)
{
    return ATDPI_SetLNA(psAtdeuDemodCtx, fgLNA);
}

BOOL AtdEU_SetAntIn(UINT8 fgAntIn)
{
    return ATDPI_SetAntIn(psAtdeuDemodCtx, fgAntIn);
}
/*********************************************************
AtdEU_SetDeScrambleBox
This API is for descramble box setting
BOOL fgScramble : TRUE --enable descaramble fuction;FALSE --disable descaramble fuction;
UINT16 DelayForDescramble  When enable descaramble fuction. The delay time(MS) add before check TVD 

*********************************************************/
BOOL AtdEU_SetDeScrambleBox(BOOL fgScramble, UINT16 DelayForDescramble )
{
    if(fgScramble)
		{DeScrambleBox = TRUE;
		DeScrambleBoxDelay = DelayForDescramble;}
    else
		{DeScrambleBox = FALSE;
		DeScrambleBoxDelay = 0;}
#if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368)  || defined(CC_MT5389))
	ATD_SetDelayLine(psAtdeuDemodCtx,fgScramble); 
#endif
	return TRUE;
}


#if (fcTUNER_TYPE == cDVBT_TUNER_ALL)||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST) || (fcTUNER_TYPE == cTTE_I2CALLLST)||(fcTUNER_TYPE == cTPV2K11EU_TUNERLST)

#ifndef I2C_TUNER_ALL
static void setTunerTypeToEeprom(UINT8 u1TunerType){
    UINT8 u1Val1 = 0;
    UINT8 u1Val2 = 0;
    UINT8 u1Val3 = 0;
    UINT32 u4Ofst0,u4Ofst1,u4Ofst2;

    u4Ofst0 = DRVCUST_OptGet(eTunerTypeOfst0);  //read TUNER EEPROM drvcust offset address
    u4Ofst1 = DRVCUST_OptGet(eTunerTypeOfst1);
    u4Ofst2 = DRVCUST_OptGet(eTunerTypeOfst2);

    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst0, (UINT32)(void *)&u1Val1, 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst1, (UINT32)(void *)&u1Val2, 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst2, (UINT32)(void *)&u1Val3, 1));
    mcSHOW_USER_MSG(("read TunerType from EEPROM, addr0x%x=0x%x, addr0x%x=0x%x, addr0x%x=0x%x\r\n",u4Ofst0,u1Val1,u4Ofst1,u1Val2,u4Ofst2,u1Val3));

    VERIFY(0 == EEPROM_Write((UINT64)u4Ofst0, (UINT32) &u1TunerType, 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst0, (UINT32)(void *)&u1Val1, 1));

    VERIFY(0 == EEPROM_Write((UINT64)u4Ofst1, (UINT32) &u1TunerType, 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst1, (UINT32)(void *)&u1Val2, 1));

    VERIFY(0 == EEPROM_Write((UINT64)u4Ofst2, (UINT32) &u1TunerType, 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst2, (UINT32)(void *)&u1Val3, 1));
    mcSHOW_USER_MSG(("Modify TunerType in EEPROM, addr0x%x=0x%x, addr0x%x=0x%x, addr0x%x=0x%x\r\n",u4Ofst0,u1Val1,u4Ofst1,u1Val2,u4Ofst2,u1Val3));

    //VERIFY(0 == EEPROM_Write((UINT64)0x145, (UINT32)(void *)&u1Val, 1));
    //VERIFY(0 == EEPROM_Write((UINT64)0x4de, (UINT32)(void *)&u1Val, 1));
    //VERIFY(0 == EEPROM_Write((UINT64)0x7f6, (UINT32)(void *)&u1Val, 1));
}
//------------------------------------------------------------------------- 
/*
 *  checkValidTunerType
 *  check whether tuner Type is valid
 *  @param  u1TestTunerType   TunerType for test
 *  @retval TRUE    valid tuner type   
 *  @retval FALSE    invalid tuner type
 */
//-------------------------------------------------------------------------

static BOOL checkValidTunerType(UINT8 u1TestTunerType){
//todo: need refine the list
    if( (u1TestTunerType > cDVBT_TUNER_TYPE_START) && (u1TestTunerType <= cITUNER_TYPE_END) ){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

#endif
//------------------------------------------------------------------------- 
/*
 *  checkTunerTypeFromEeprom
 *  1. Use current 5133_All_P_analog configuration
 *  2. tuner id exist in three additional EEPROM address for check
 *    0x145
 *    0x4DE
 *    0x7F6
 *  3. Judgement:
 *  Tuner ID invalid, ex. 0xff -> Use the default Tuner, 
 *  3 address are with the same valid Tuner ID -> Trust the tuner ID
 *  2 address are with the same valid Tuner ID -> Trust the tuner ID
 *  All address are with different Tuner ID -> Use the default Tuner
 *  @param  void
 *  @retval tunerid    return tuner id depend on the matched judgement rule
 */
//-------------------------------------------------------------------------
static UINT8 checkTunerTypeFromEeprom(void)
{

#ifdef I2C_TUNER_ALL
	
  UINT8 *pucBuffer =0;
  UINT16 ucByteCount =1;
	
   if(ICtrlBus_I2cTunerRead(270, 0xc0, pucBuffer, ucByteCount) == 0)
		  
	 {
	     mcSHOW_USER_MSG(("--tuner I2C Addr = C0->Open tuner DT70WI------------\n"));
		 return 0x62;
	 }
    else if(ICtrlBus_I2cTunerRead(270, 0xc2, pucBuffer, ucByteCount) == 0)
     {
	     mcSHOW_USER_MSG(("--tuner I2C Addr = C2->Open tuner ENV57S------------\n"));
	     return 0x5d;
	 }
    else
     {
	     mcSHOW_USER_MSG(("--default I2C Addr = C2->Open tuner ENV57S------------\n"));
	     return 0x5d;
	 }	
#else

	UINT8 u1Val1 = 0;
    UINT8 u1Val2 = 0;
    UINT8 u1Val3 = 0;
    UINT8 u1DeftType;
    UINT32 u4Ofst0,u4Ofst1,u4Ofst2,u4Default;

    u4Ofst0 = DRVCUST_OptGet(eTunerTypeOfst0);
    u4Ofst1 = DRVCUST_OptGet(eTunerTypeOfst1);
    u4Ofst2 = DRVCUST_OptGet(eTunerTypeOfst2);
    u4Default = DRVCUST_OptGet(eTunerTypeDefault);
    mcSHOW_USER_MSG(("ofst0=0x%x, ofst1=0x%x, ofst2=0x%x, def=0x%x\r\n",u4Ofst0, u4Ofst1, u4Ofst2, u4Default));

    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst0, (UINT32)(void *)&u1Val1, 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst1, (UINT32)(void *)&u1Val2, 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst2, (UINT32)(void *)&u1Val3, 1));
    mcSHOW_USER_MSG(("read TunerType from EEPROM, addr0x%x=0x%x, addr0x%x=0x%x, addr0x%x=0x%x\r\n",u4Ofst0,u1Val1,u4Ofst1,u1Val2,u4Ofst2,u1Val3));

    // Ken, 081111
    if((u4Default < cDVBT_TUNER_TYPE_START) || (u4Default > cDVBT_VALID_TYPE_END))
    	  u1DeftType = cPANASONIC_ENV57S; //liuqu,20090716
    else
    	  u1DeftType = (U8) u4Default;

    /*if two of tuner types in eeprom are the same and are valid tuner type, then trust this vaule, other condition return default type*/
    if ((checkValidTunerType(u1Val1) == TRUE) && (u1Val1==u1Val2) ){
        return u1Val1;
    }
    else if((checkValidTunerType(u1Val1) == TRUE) && (u1Val1==u1Val3)){
        return u1Val1;
    }
    else if((checkValidTunerType(u1Val2) == TRUE) && (u1Val2==u1Val3)){
        return u1Val2;
    }
    else{
        setTunerTypeToEeprom(u1DeftType);   // Ken, recommend from CS/Terrence
        //return default tuner type
        return u1DeftType;
    }
#endif
}
#endif

/************************************************************************
*  AtdEU_TunerOpen
*  Analog glue layer initiliazation function.
*  @param  ptTDCtx : A pointer use to point to struct TUNER_DEVICE_CTX_T.
*  @param  ptTunerCfgInfo : A pointer use to point to struct TUNER_CONFIG_T.
*  @param  pptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @retval  DRVAPI_TUNER_OK : Analog glue initialization successfully.
*  @retval  DRVAPI_TUNER_ERROR : Analog glue initialization fail.
************************************************************************/
INT32 AtdEU_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo,
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
#if (ATD_NOT_USE==FALSE)

ATD_PD_CTX_T   *psAtdPdCtx;
#if (fcTUNER_TYPE == cDVBT_TUNER_ALL)||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST) || (fcTUNER_TYPE == cTTE_I2CALLLST)||(fcTUNER_TYPE == cTPV2K11EU_TUNERLST)
	UINT8 tunerType;
#endif

    Tuner_AtdEU_Register_LTDIS_Fct(ptTDCtx);

    /*create ATD PD CTX*/
    pAtdTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    psAtdPdCtx = (ATD_PD_CTX_T *) mcALLOC_MEM(sizeof(ATD_PD_CTX_T));
    if (psAtdPdCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    
    /* Init Disconnect Status */
    psAtdPdCtx->fgDisStatus = FALSE;
    *pptTDSpecificCtx = psAtdPdCtx;

    psAtdPdCtx->ucTvSysGroupNum = sizeof(au4TvSysGroup) / sizeof(UINT32);
    psAtdPdCtx->pu4TvSysGroup = au4TvSysGroup;
    psAtdPdCtx->psTunerAnaAttr = &sTunerAnaAttribute;

    psAtdeuDemodCtx = ATD_DemodCtxCreate();
    if (psAtdeuDemodCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }   
    /*link demod ctx to pd ctx*/
    psAtdPdCtx->sAtdPiCtx = psAtdeuDemodCtx;

 //090720, LC, move ITuner init b4 ATD init
#if 0
    ITunerRegistration(cITUNER_DONT_CARE);
#else
#if (fcTUNER_TYPE != cDVBT_TUNER_ALL) &&(fcTUNER_TYPE != cDVBT_2K10_TUNERLST) && (fcTUNER_TYPE != cTTE_I2CALLLST) &&(fcTUNER_TYPE != cTPV2K11EU_TUNERLST)
    psAtdeuDemodCtx->u1TunerType =fcTUNER_TYPE;//Tuner type get from HAL is not correct.Temprary fix.Haibo Li 100603
    ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
 #else
    tunerType = checkTunerTypeFromEeprom();
	psAtdeuDemodCtx->u1TunerType =tunerType;//Tuner type get from HAL is not correct.Temprary fix.Haibo Li 100603
    ITunerRegistration(tunerType);
    mcSHOW_USER_MSG(("Register ATD Tunerall type = %d\n",tunerType));	
 #endif
#endif

    pTunerCtx = ITunerGetCtx();
    if (pTunerCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }   
    ITuner_Init(pTunerCtx); //todo: modify tuneri2caddr

    psAtdeuDemodCtx->pTCtx = pTunerCtx; 
   // psAtdeuDemodCtx->u1TunerType =fcTUNER_TYPE;//((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType;//Tuner type get from HAL is not correct.Temprary fix.Haibo Li 100603

//    ATD_ChipInit(psAtdeuDemodCtx); // move to ATD_Initialize()

    // Initialize ATD context & download FW
    if (ATD_Initialize(psAtdeuDemodCtx, 0x92, TRUE)){
        mcSHOW_USER_MSG(("ATD_Initialize failed\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    else {
    	mcSHOW_USER_MSG(("ATD_Initialize successful !\n"));
    }

#if 0
#if 0
    ITunerRegistration(cITUNER_DONT_CARE);
#else
 #if (fcTUNER_TYPE != cDVBT_TUNER_ALL)   //liuqu,20090716,add for DVBT TunerAll structure
    ITunerRegistration(((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType);
 #else
    tunerType = checkTunerTypeFromEeprom();
    ITunerRegistration(tunerType);
    mcSHOW_USER_MSG(("Register ATD Tunerall type = %d\n",tunerType));	
 #endif
#endif
    
    pTunerCtx = ITunerGetCtx();
    ITuner_Init(pTunerCtx); //todo: modify tuneri2caddr

    psAtdeuDemodCtx->pTCtx = pTunerCtx; 
    psAtdeuDemodCtx->u1TunerType =((TUNER_DEVICE_CTX_T*)ptTDCtx)->tunerType;
#endif
    
	ATD_DisConnect(psAtdeuDemodCtx);//20110503_Haibo:Discconnect ATD after tuner open
	
#if fcADD_HAL_LOCK
    if (mcSEMA_CREATE(&psAtdPdCtx->hHalLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("%s: mcSEMA_CREATE(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
#endif

#if fcTUNE_NOT_START
    psAtdPdCtx->u1TunerAcqType = ACQ_ACTION_DEFAULT;
#endif


//#ifdef ANAEU_ENABLE_MONITOR
    if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        if (mcSEMA_CREATE(&t_escape_atdeu_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            mcSHOW_DBG_MSG(("EU_ATD_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
            return (DRVAPI_TUNER_ERROR);
        }
    
        psAtdPdCtx->fgBreakMonitor = TRUE;//init
        psAtdPdCtx->fgPIMoniStatus =FALSE; //init
        PD_StartMonitorEngine(psAtdPdCtx);  //start monitor engine
    }
//#endif

#endif

    return (DRVAPI_TUNER_OK);
}

/************************************************************************
*  AtdEU_TunerClose
*  Analog glue layer close function.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @retval  DRVAPI_TUNER_OK : Analog glue close successfully.
*  @retval  DRVAPI_TUNER_ERROR : Analog glue close fail.
************************************************************************/
INT32 AtdEU_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
#if (ATD_NOT_USE==FALSE)

#if fcADD_HAL_LOCK
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
#endif

#if fcADD_HAL_LOCK
    if (mcSEMA_DESTROY(psAtdPdCtx->hHalLock) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("%s: mcSEMA_DESTROY(hHalLock) Fail!\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif

//#ifdef ANAEU_ENABLE_MONITOR
    if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        psAtdPdCtx->fgBreakMonitor = TRUE; //disable and breadk monitor thread
        fgMonitorThreadRunning = FALSE; //need set to false before mcMUTEX_UNLOCK(t_escape_mon_suspend);
        mcMUTEX_UNLOCK(t_escape_atdeu_mon_suspend);
        PD_StopMonitorEngine();  // stop monitor engine
        mcMUTEX_DESTROY(t_escape_atdeu_mon_suspend);  // destory monitor thread
        //PD_StopMonitorEngine();  // stop monitor engine
    }
//#endif
    
    ATD_DemodCtxDestroy(psAtdeuDemodCtx);

    mcFREE_MEM(ptTDSpecificCtx);

#endif

    return (DRVAPI_TUNER_OK);
}

/************************************************************************
*  AtdEU_TunerAcq
*  Analog tuner acquisition function.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @param  pv_conn_info : Connection information.
*  @param  z_conn_info_len : Length of connection information.
*  @param  _BreakFct : Funxtion pointer of break function.
*  @param  pvArg : Break function parameter.
*  @retval  TRUE : Analog tuner acquisition successfully.
*  @retval  FALSE : Analog tuner acquisition fail.
************************************************************************/
//get the time cost tag of ATD_ACQ
#define SUPPORT_OUTPUT_CHANNELSCAN_TIME      0   

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
     extern UINT32 os_get_sys_tick(void);
#endif


BOOL AtdEU_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    VOID* pv_conn_info,
                    SIZE_T z_conn_info_len,
                    x_break_fct _BreakFct,
                    VOID* pvArg)
{
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
UINT32  i4CurFreq = cTUNER_DEFAULT_FREQ;
UINT32  u4Freq, u4FreqBound;
INT32   s4FreqOffset;
INT16   s2FreqOffset;
UINT8   u1Path = SV_VP_MAX;
UINT8   u1ATVStatus = 0;
UINT8   ucData1;
UINT8   u1ConnMode;
TUNER_MODULATION_T emod;
BOOL    fgLockSts = TRUE;
UINT8  u1AirOrCable=0;
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
	  UINT16          u2AcqTime_Test;
	  UINT32          u4TickCnt_Start;
	  UINT32          u4TickCnt_Test; 
	  u4TickCnt_Start = os_get_sys_tick();
#endif

//#ifdef ANAEU_ENABLE_MONITOR
    SetBreakMonitor(psAtdPdCtx);
//#endif
    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry


    if ((e_conn_type == TUNER_CONN_TYPE_TER_ANA)
        || (/*COUNTRY_EU == GetTargetCountry() &&*/ (e_conn_type == TUNER_CONN_TYPE_TER_ANA_SCART_OUT))
        )
    {
        u4Freq = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq;
        u4FreqBound = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq_bound;
        psAtdPdCtx->u4TvSubSysMask = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_tv_sys_mask;
        psAtdeuDemodCtx->fgAFT = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune;
        s2FreqOffset = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->i2_fine_tune_offset;
        AtdPd_ChangeChannel(u4Freq);

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
	    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
	    mcSHOW_DBG_MSG(("ATD_Acq Connection time,after AtdPd_ChangeChannel = %d ms\n", u2AcqTime_Test));
#endif		
        emod=((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->e_mod;	
        u1ConnMode = GetAtdConnMode(emod);
        psAtdPdCtx->u1SubSysIdx = vGetTvSubSysIdx(psAtdPdCtx, psAtdPdCtx->u4TvSubSysMask);        
        u1AirOrCable=0;        
    }
    else if (e_conn_type == TUNER_CONN_TYPE_CAB_ANA) 
    {
        u4Freq = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq;
        u4FreqBound = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_freq_bound;
        psAtdPdCtx->u4TvSubSysMask = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_tv_sys_mask;
        psAtdeuDemodCtx->fgAFT = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune;
        s2FreqOffset = ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->i2_fine_tune_offset;
        AtdPd_ChangeChannel(u4Freq);

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
	    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
	    mcSHOW_DBG_MSG(("ATD_Acq Connection time,after AtdPd_ChangeChannel = %d ms\n", u2AcqTime_Test));
#endif
        emod=((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->e_mod;	
        u1ConnMode = GetAtdConnMode(emod);
        psAtdPdCtx->u1SubSysIdx = vGetTvSubSysIdx(psAtdPdCtx, psAtdPdCtx->u4TvSubSysMask); 
		u1AirOrCable=1;
    }
    else
    {
        mcSHOW_DRVERR_MSG(("%s: Wrong e_conn_type (%d)!\n", __FUNCTION__, e_conn_type));
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        return FALSE;
    }
    ITuner_OP(psAtdeuDemodCtx->pTCtx,itSetAntIn,u1AirOrCable,NULL); ;//Switch tuner input  0: Air 1:Cable
    // update psAtdeuDemodCtx->u1SubSysIdx for SetFreq needs TV system too
    psAtdeuDemodCtx->u1SubSysIdx = psAtdPdCtx->u1SubSysIdx;

   mcSHOW_DBG_MSG((" b_fine_tune=%d\n", psAtdeuDemodCtx->fgAFT));
#if fcADD_TVD_SYNC
      if (u1ConnMode != ATV_CONN_MODE_CHAN_SCAN)     // liuqu,20100315, do not do this job in channel scan mode.
    	{
       vTvdHoldMode(SV_ON);
    	}
#endif 

    if (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE) {
        vTvd3dChannelChange();
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
        psAtdeuDemodCtx->fgAudScan = FALSE;
#endif        
    }

    mcSHOW_DBG_MSG(("%s (Freq=%uHz) (Freq_bound=%uHz)\n", __FUNCTION__, u4Freq, u4FreqBound));

#if fcTIMING_MEASURE
    u2TickStart_eu = mcGET_SYS_TICK();
#endif

    u4Freq = u4Freq / 1000;  // transfer from Hz to KHz
    u4FreqBound = u4FreqBound / 1000;  // transfer from Hz to KHz

#if defined(ATD_NTSC443_DETECT)
    fgSameFreq = (u4PreFreq == u4Freq)?TRUE:FALSE;
    u4PreFreq = u4Freq;
#endif
    
    if (u1ConnMode == ATV_CONN_MODE_FM_RADIO)
    {
//EC_code_d100825: For MT5365/95 FM Radio flow modification: 
        psAtdPdCtx->u1SubSysIdx = MOD_PAL_BG;
         psAtdeuDemodCtx->u1SubSysIdx = MOD_PAL_BG;// add by liuqu for update the sys idx!
         
        ATD_SetFreq(psAtdeuDemodCtx, u4Freq, _fgAutoSearch);
        mcSHOW_DBG_MSG(("Set ATDEU freq. of FM Radio = %dKHz\n",u4Freq));
        ATD_ChipInit(psAtdeuDemodCtx);
        ATD_SetSystem(psAtdeuDemodCtx, psAtdPdCtx->u1SubSysIdx);        
        mcSHOW_DBG_MSG(("Set ATDEU system of FM Radio = %d\n",psAtdPdCtx->u1SubSysIdx));  
        SetFMRadio(psAtdeuDemodCtx, u4Freq);
        ATD_StartAcq(psAtdeuDemodCtx, FALSE);
        AtdPd_ExitTunerAcq(TRUE);
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        psAtdPdCtx->u1VIFLock = 2;
        /* Init Disconnect Status */
        psAtdPdCtx->fgDisStatus = FALSE;
        return TRUE;	
    }	    

    if ((u1ConnMode != ATV_CONN_MODE_CHAN_SCAN)&&(psAtdeuDemodCtx->fgAFT == FALSE) && (s2FreqOffset > -80) && (s2FreqOffset < 80))
    {        
        ucData1 = 0xF9;
        ATD_SetReg(psAtdeuDemodCtx, REG_ATD_ACR_CFG, &ucData1, 1); //EC_code_d9A28: Enable CR auto-reacq for Fine tune mode
#if defined(ATD_FINETUNE_STEPSIZE)  
        s4FreqOffset = s2FreqOffset * ATD_FINETUNE_STEPSIZE; //EC_code_dA111: For SY project, ATD_FINETUNE_STEPSIZE should be define as "31250" equal to 31.25KHz for each step
#else
        s4FreqOffset = s2FreqOffset * 0;
#endif
        mcSHOW_DBG_MSG(("Original freq = %dKHz, Offset freq = %dKHz\n",u4Freq, s4FreqOffset/1000));
        u4Freq = u4Freq + s4FreqOffset/1000;
        mcSHOW_DBG_MSG(("New freq = %dKHz\n",u4Freq));
    }

    // Move to the beginning of TunerAcq due to AD power on, and ATD_ChipInit won't use any
    // argument of psAtdeuDemodCtx
    // Justin TODO, do init every time??
    // ATD_ChipInit(psAtdeuDemodCtx);

    //if (1){  // liuqu changed,20090722,for fine-tune mode debug
    if (ATD_SetFreq(psAtdeuDemodCtx, u4Freq, _fgAutoSearch) != TRUE) /*jackon: why need use SV_TRUE?*/
    {
        mcSHOW_USER_MSG(("Freq %dKHz setting fail!\n", u4Freq));
        AtdPd_ExitTunerAcq(FALSE);
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        //return FALSE; //EC_code_dA507
    }
    
    if (u1ConnMode != ATV_CONN_MODE_FINE_TUNE)
    {
        ATD_ChipInit(psAtdeuDemodCtx);
		
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
	    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
	    mcSHOW_DBG_MSG(("ATD_Acq Connection time,after ATD_ChipInit = %d ms\n", u2AcqTime_Test));
#endif

        ATD_SetSystem(psAtdeuDemodCtx, psAtdPdCtx->u1SubSysIdx);
        if ((psAtdPdCtx->u1SubSysIdx==MOD_SECAM_L) ||(psAtdPdCtx->u1SubSysIdx==MOD_SECAM_L1))// to get the initial DRO target level
        {
            ATD_GetReg(psAtdeuDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1); 
            u2DroTarLvlInitial = 0x0000;
		    u2DroTarLvlInitial |= (UINT16)ucData1;
    		u2DroTarLvlInitial = (u2DroTarLvlInitial << 8);
	    	ATD_GetReg(psAtdeuDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData1, 1);
		    u2DroTarLvlInitial |= (UINT16)ucData1;
			u2DroTarLvlInitial &= 0x7FFF;
    		mcSHOW_DBG_MSG1(("The SECAML/L1 DRO target level(initial value) = 0x%04X \n", u2DroTarLvlInitial));
        }
			
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
		u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
		mcSHOW_DBG_MSG(("ATD_Acq Connection time,after ATD_SetSystem = %d ms\n", u2AcqTime_Test));
#endif
    }

    if (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE)   
    {
        //Reset CR module before SetFreq when channel change ,for CR DTV00144511
		ucData1 = 0xFE; 
	    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_CTL_RST2, &ucData1, 1);
		ucData1 = 0xFF; 
	    ATD_SetReg(psAtdeuDemodCtx, REG_ATD_CTL_RST2, &ucData1, 1);
		
		// Release PRA for TDTWS810D Analog channel change LNA 
		if (psAtdeuDemodCtx->u1TunerType == cLG_TDTWS810D)
		{
		   ucData1 = 0xFF; 
	       ATD_SetReg(psAtdeuDemodCtx, REG_ATD_CTL_RST1, &ucData1, 1); //realese data path register,for TDTWS810D tuner LNA
		}
    }
	
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
		u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
		mcSHOW_DBG_MSG(("ATD_Acq Connection time,before ATD_StartAcq = %d ms\n", u2AcqTime_Test));
#endif

     // Justin TODO, fgScanMode & psAtdeuDemodCtx->u1PF necessary??
    if (u1ConnMode == ATV_CONN_MODE_CHAN_SCAN)
    {
    	if(psAtdeuDemodCtx->fgAFT == TRUE ) 
        {
    	    //psAtdeuDemodCtx->u1PF = 0x01;  //enable pilot filter when initial scan
    	    if(g_bypass_pilotsearch)
            {
                psAtdeuDemodCtx->u1PF = 0x00;  //disable pilot searcher when tvd unlock
                g_bypass_pilotsearch=0x00;
                mcSHOW_DBG_MSG(("Last status is VIF full lock,TVD unlock,need bypass pilot search \n"));
            }
            else{
    	    psAtdeuDemodCtx->u1PF = 0x01;  //enable pilot filter when initial scan
            }
    	}
    	else {
    	    psAtdeuDemodCtx->u1PF = 0x00;  //disable pilot filter when retry during scan
    	}    
    	ATD_StartAcq(psAtdeuDemodCtx, TRUE);
    	//u1ChChangeInitSys = psAtdPdCtx->u1SubSysIdx; //100412, for sys detect at change change
    }
    else if(u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE)
    {
        //psAtdeuDemodCtx->u1PF = 0x01;  //enable pilot filter when channel change
        psAtdeuDemodCtx->u1PF = 0x00;  //enable bypass pilot searcher when channel change
        ATD_StartAcq(psAtdeuDemodCtx, FALSE);
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM         
        u1ChChangeInitSys = psAtdPdCtx->u1SubSysIdx; //100412, for sys detect at change change
#endif        
    }
#if (fcTUNER_TYPE ==cLG_TDTKG931D || fcTUNER_TYPE ==cTCL_DT91WT)
	if(u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE)
		{
	mcSHOW_DBG_MSG(("set LNA for analog TV \n"));
	ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNABYADC, 0, 0);
		}
#endif	
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
    u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
    mcSHOW_DBG_MSG(("ATD_Acq Connection time,after ATD_StartAcq = %d ms\n", u2AcqTime_Test));
#endif

#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG((" (1.%3u ms)\n", (mcGET_SYS_TICK() - u2TickStart_eu) * mcGET_TICK_PERIOD()));
#endif
#if fcTUNE_NOT_START
    if (psAtdPdCtx->u1TunerAcqType == TUNE_FREQ_ONLY){
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
        /* Init Disconnect Status */
        psAtdPdCtx->fgDisStatus = FALSE;
        return TRUE;
    }
#endif

    /* Wait for TV-decoder to connect to tuner */
    if (u1ConnMode != ATV_CONN_MODE_CHAN_CHANGE)
    {
    INT32 i4_i = 0;

        for (i4_i = 0; i4_i < 30; i4_i++)
        {
            if (bGetSignalType(SV_VP_PIP) == (UINT8) SV_ST_TV)
            {
                u1Path = SV_VP_PIP;
                mcSHOW_DBG_MSG(("[ATuner] u1Path = SV_VP_PIP\n"));
                break;
            }
            else if (bGetSignalType(SV_VP_MAIN) == (UINT8) SV_ST_TV)
            {
                u1Path = SV_VP_MAIN;
                mcSHOW_DBG_MSG(("[ATuner] u1Path = SV_VP_MAIN\n"));
                break;
            }
            else if (bGetSignalType(SV_VP_PIP) == (UINT8) SV_ST_AV)
            {
                u1Path = SV_VP_PIP;
                mcSHOW_DBG_MSG(("[ATuner] u1Path = SV_VP_PIP, AV mode\n"));
                break;
            }
            else if (bGetSignalType(SV_VP_MAIN) == (UINT8) SV_ST_AV)
            {
                u1Path = SV_VP_MAIN;
                mcSHOW_DBG_MSG(("[ATuner] u1Path = SV_VP_MAIN, AV mode\n"));
                break;
            }
            else
            {
                if (_BreakFct(pvArg))
                {
                    mcSHOW_DBG_MSG(("[ATuner] BreakFct\n"));
                    break;
                }
                mcDELAY_MS(50);
                continue;
            }
        }
        if ((i4_i >= 30) || (u1Path == SV_VP_MAX))
        {
            mcSHOW_DRVERR_MSG(("[ATuner] ERROR! without TV-decoder!\n"));
            AtdPd_ExitTunerAcq(FALSE);          
            mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);

            return FALSE;

        }
    }

    i4CurFreq = u4Freq;

    // LC Chien, check fine-tune flag instead of _fgAutoSearch for EU-mode, 070326
    if ( (u1ConnMode == ATV_CONN_MODE_CHAN_SCAN) && (psAtdeuDemodCtx->fgAFT))
    {
        UINT32 u2FFF = u4Freq;
        UINT32 u2FFFBD = u4FreqBound;

        if(u2FFF== u2FFFBD)
        	u2FFFBD += 100;
        
        psAtdeuDemodCtx->fgCheckCR = TRUE;

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
		u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
		mcSHOW_DBG_MSG(("ATD_Acq Connection time,before SearchNearbyFreq = %d ms\n", u2AcqTime_Test));
#endif
#if TVD_SUPPORT_SNOW_MODE
         if(fgTvdLock)
		 {
		   mcSHOW_DBG_MSG(("Reset TVD status\n"));
		   vTvd3dReset();//d20110719_Haibo:Reset TVD staye if previous VPress status is ON 
		 }
#endif		 
         ATD_SetSnowMode(psAtdeuDemodCtx, TRUE);//d20110719_Haibo:Enable snow mode before searchNearBy
        u1ATVStatus = ATDEU_SearchNearbyFreq(psAtdeuDemodCtx, &u2FFF, _BreakFct, pvArg, u2FFFBD);

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
		u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
		mcSHOW_DBG_MSG(("ATD_Acq Connection time,after SearchNearbyFreq = %d ms\n", u2AcqTime_Test));
#endif       
        if (mcTEST_BIT(u1ATVStatus, cpANA_LOCK_STS_VIF))
        {
        //    i4CurFreq = u2FFF;
            fgLockSts = AtdPd_CheckTVDLock(psAtdPdCtx, u2FFF, 'V');
            psAtdPdCtx->u1VIFLock = 2;
        }
        else
        {
            fgLockSts = FALSE;
            psAtdPdCtx->u1VIFLock = 0;
        }

        if (fgLockSts)
        {
            fgTvdLock = TRUE;
            mcSHOW_DBG_MSG(("[ATuner] A_N_S\n"));
            AtdPd_TvdNotify(u1Path, SV_VDO_STABLE);
        }
        else
        {
            fgTvdLock = FALSE;
            mcSHOW_DBG_MSG(("[ATuner] A_N_N\n"));
            AtdPd_TvdNotify(u1Path, SV_VDO_NOSIGNAL);
        }
        i4CurFreq = u2FFF;
    } //end of SearchNearBy
    else
    {
    	
        UINT8   ii;
      if(u1ConnMode!=ATV_CONN_MODE_CHAN_SCAN)       
	  {
        mcSHOW_DBG_MSG3(("Snow Mode Off: When not do search\n"));
        ATD_SetSnowMode(psAtdeuDemodCtx, FALSE);
	  }
        //check both VIF status & CFO when 1st in scan, LC 090813
        if(emod== MOD_QAM_896)        
            psAtdeuDemodCtx->fgCheckCR = TRUE;
        else
            psAtdeuDemodCtx->fgCheckCR = FALSE;

        switch(u1ConnMode)
        {
            case ATV_CONN_MODE_CHAN_CHANGE:
            /* Not do fine-tune, just wait VIF lock */
                for (ii = 0; ii < (200 / (cCHECK_VIF_DELAY * 10)); ii ++)
                {
                    mcDELAY_MS(10 * cCHECK_VIF_DELAY);
                    u1ATVStatus = ATD_GetVIFLock(psAtdeuDemodCtx, TRUE);
                    // Check VIF really lock during channel change, LC Chien 070504
                    if (u1ATVStatus >= 1)
                        break;
                }
                psAtdPdCtx->u1VIFLock = u1ATVStatus;
                break;

            case ATV_CONN_MODE_FINE_TUNE:
            //*** ZY 091222, ATD not notify at fine tune mode. TVD will notify
            // always notify signal stable when fine-tune, LC 071002 CR[DTV00106617]
                //_Tvd_NotifyTVSig(u1Path, SV_VDO_STABLE);
                psAtdPdCtx->u1VIFLock = 3;
                break;

            case ATV_CONN_MODE_CHAN_SCAN:
                mcDELAY_MS(20);
                u1ATVStatus = ATD_GetVIFLock(psAtdeuDemodCtx, TRUE);
                //psAtdPdCtx->u1VIFLock = u1ATVStatus;
                // Do TVD check when auto search enable but fine-tune disable, LC Chien 070326
                // When VIFLock=1 at 1st scan, do not do TVD check, LC 090813
                // When VIFLock=1 at retry, do TVD check, LC 090813
                //if ( (u1ATVStatus == 2) || ((u1ATVStatus == 1) && (emod == MOD_UNKNOWN)) )
                if (u1ATVStatus >= 1)//d111107_Haibo:Only check CR lock when do system retry.For channel loss caused by FN(CFO variation)
                {
                    if (AtdPd_CheckTVDLock(psAtdPdCtx, u4Freq, 'X'))
                    {
                        fgTvdLock = TRUE;
                        mcSHOW_DBG_MSG(("[ATuner] A_N_S\n"));    
                        AtdPd_TvdNotify(u1Path, SV_VDO_STABLE);    //move to Thomson/tuner.c
                    }
                    else
                    {
                        fgTvdLock = FALSE;
                        mcSHOW_DBG_MSG(("[ATuner] A_N_N\n"));
                        AtdPd_TvdNotify(u1Path, SV_VDO_NOSIGNAL);
                    }
                    psAtdPdCtx->u1VIFLock = 2;
                }
                else
                {
                    fgTvdLock = FALSE;
                    psAtdPdCtx->u1VIFLock = 0;
                    mcSHOW_DBG_MSG(("[ATuner] A_N_N\n"));
                    AtdPd_TvdNotify(u1Path, SV_VDO_NOSIGNAL);
                }
                break;

            default:
                break;
        }

    } //end of non-SearchNearBy

    mcSHOW_DBG_MSG(("VIFLock: %d (%dKHz)\n",
                    psAtdPdCtx->u1VIFLock,
                    i4CurFreq));

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
		u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
		mcSHOW_DBG_MSG(("ATD_Acq Connection time,after check VIF and TVD = %d ms\n", u2AcqTime_Test));
#endif 

#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG((" (2.%3u ms)\n", (mcGET_SYS_TICK() - u2TickStart_eu) * mcGET_TICK_PERIOD()));
#endif

    mcSHOW_DBG_MSG(("[ATuner] %s fine-tune: %dKHz\n", __FUNCTION__, i4CurFreq));
    psAtdPdCtx->Frequency = i4CurFreq;


//#ifdef ANAEU_ENABLE_MONITOR
    if(!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE))
    {    	
    	//LevelSwitchTogCnt = 0;
    	//psAtdeuDemodCtx->fgAFT = ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune;
        ResumeMonitor(psAtdPdCtx);
        //mcSHOW_USER_MSG(("Enable monitor process!!, fgAFT = %d\n", psAtdeuDemodCtx->fgAFT));
        //mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
    }
    
    /*if (!_BreakFct(pvArg) && (u1ConnMode == ATV_CONN_MODE_CHAN_CHANGE) && (((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->b_fine_tune))
    {
        ResumeMonitor(psAtdPdCtx);   // Enable monitor thread and Disable break function flag,  Unlock monitor thread
        mcSHOW_DBG_MSG(("Enable monitor process!!\n"));
    }*/
//#endif
    if (psAtdPdCtx->u1VIFLock < 1)
    {
        AtdPd_ExitTunerAcq(FALSE);       
        mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
		psAtdPdCtx->fgDisStatus = FALSE;
        return FALSE;
    }

    AtdPd_ExitTunerAcq(TRUE);

#ifndef CC_MT5391_AUD_3_DECODER
    /*if equal MOD_UNKNOWN, this acq is under channel scan, don't play under scan mode, request my ML*/
    //if((((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->e_mod) != MOD_UNKNOWN)
    if(u1ConnMode != ATV_CONN_MODE_CHAN_SCAN) //LC Chien, 20090813
    {
	   if(e_conn_type == TUNER_CONN_TYPE_CAB_ANA)
        {
		  Aud_SetTvSysMask(0, ((TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_tv_sys_mask);
        }
        else
		{
		   Aud_SetTvSysMask(0, ((TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info)->ui4_tv_sys_mask);
		}
        AUD_TriggerDspPlay(1); /*to start audio*/
        //mcSHOW_USER_MSG(("Tuner_DRV call AUD_TriggerDspPlay(AUX_DEC)!\n\n"));
    }
#endif

    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);

  
    /* Init Disconnect Status */
    psAtdPdCtx->fgDisStatus = FALSE;
    return TRUE;

}

/************************************************************************
*  AtdEU_TunerGetSync
*  Get current analog tuner connection status.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @retval  1 : Analog tuner lock.
*  @retval  0 : Analog tuner unlock.
************************************************************************/
INT16 AtdEU_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                          DRV_CONN_TYPE_T e_conn_type)
{

ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;

INT16   i2RetValue = 1;                     // Always in sync state


    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    #ifdef CC_FOR_SDAL
//    psAtdPdCtx->u1VIFLock = AnalogPI_TunerCheckVIFLock(cATUNER_PI_CTX(psAtdPdCtx), i4CurFreq);
    #endif

    //if (!mcTEST_BIT(psAtdPdCtx->u1VIFLock, cpANA_LOCK_STS_VIF))
    if(psAtdPdCtx->u1VIFLock == 0)
    {
        i2RetValue = 0;
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);


    return i2RetValue;
}

/************************************************************************
*  AtdEU_TunerGetSignal
*  Get current analog signal status.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @param  *_pSignal : Output data content pointer.
*  @retval  void.
************************************************************************/
VOID AtdEU_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type,
                    SIGNAL *_pSignal)
{
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
// TUNER_POLARIZATION_T  e_pol;         /* Polarization. */
// TUNER_MODULATION_T    e_mod;         /* Modulation. */
// UINT16                ui2_dir;       /* Direction in 0.1 degrees. Valid value range '0..3599' (inclusive). */
// UINT16                ui2_gain;      /* Gain in 0.1 steps. Valid range '0..1000' (inclusive). */
UINT32 i4CurFreq;
UINT32  u4Freq;

    i4CurFreq = psAtdPdCtx->Frequency;

    u4Freq = i4CurFreq * 1000;

    mcSHOW_DBG_MSG(("[ATuner] %s: %04X (%d)", __FUNCTION__, i4CurFreq, u4Freq));
    if ((e_conn_type == TUNER_CONN_TYPE_CAB_ANA)
//#ifdef TUNER_SUPPORT_SCART_OUT
//        || (e_conn_type == TUNER_CONN_TYPE_CAB_ANA)
//        || (COUNTRY_EU == GetTargetCountry() && (e_conn_type == TUNER_CONN_TYPE_CAB_ANA))
//#endif
    )
    {
        TUNER_CAB_ANA_TUNE_INFO_T * pt_cab_ana = (TUNER_CAB_ANA_TUNE_INFO_T *)_pSignal;
        pt_cab_ana->ui4_freq = u4Freq;
        pt_cab_ana->e_mod = MOD_PSK_8;
        mcSHOW_DBG_MSG((" (%d)\n", pt_cab_ana->ui4_freq));
    }
    else if ((e_conn_type == TUNER_CONN_TYPE_TER_ANA)
//#ifdef TUNER_SUPPORT_SCART_OUT
//        || (e_conn_type == TUNER_CONN_TYPE_TER_ANA)
//        || (COUNTRY_EU == GetTargetCountry() && (e_conn_type == TUNER_CONN_TYPE_TER_ANA))
//#endif
    )
    {
        TUNER_TER_ANA_TUNE_INFO_T * pt_ter_ana = (TUNER_TER_ANA_TUNE_INFO_T *)_pSignal;
        pt_ter_ana->ui4_freq = u4Freq;
        pt_ter_ana->e_mod = MOD_PSK_8;
        mcSHOW_DBG_MSG((" (%d)\n", pt_ter_ana->ui4_freq));
    }
}

/************************************************************************
*  AtdEU_TunerGetSignalLevel
*  Get current analog signal level.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @retval  Always 100.
************************************************************************/
UINT8  AtdEU_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Value;
    /* Provided by TVD/KY lin
    > 60dB: 99
      55dB: 97
      50dB: 91
      45dB: 84
      40dB: 72
      35dB: 55
      30dB: 43
      25dB: 23
      signal off: 0
    */
    u1Value = bTvdSignalStrengthLevel(0); // bMode = 0, return 0~100
    return (u1Value);
}

void ATD_PowerDown(UINT8 fg)
{
if(fg)
	ATD_DisConnect(psAtdeuDemodCtx);
else
	ATD_ChipInit(psAtdeuDemodCtx);

}
/************************************************************************
*  AtdEU_TunerGetSignalLeveldBm
*  Get current analog signal level in dBm.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @retval  Always 0.
************************************************************************/
INT16 AtdEU_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    UINT8 u1Value;
    u1Value = bTvdSignalStrengthLevel(1); // bMode = 1, return in dB
    return (u1Value);
}

/************************************************************************
*  AtdEU_TunerGetSignalPER
*  Get current analog signal packet error rate.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @retval  Always 0.
************************************************************************/
UINT16  AtdEU_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return (0);
}

/************************************************************************
*  AtdEU_TunerGetSignalSNR
*  Get current analog signal SNR.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @retval  Always 0.
************************************************************************/
UINT16 AtdEU_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                    DRV_CONN_TYPE_T e_conn_type)
{
    return (0);                             // return 0 first
}

/************************************************************************
*  AtdEU_TunerSetMpgFmt
*  Get current analog signal mpeg format.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  *pt_mpeg_fmt : Output data content pointer.
*  @retval  void.
************************************************************************/
VOID AtdEU_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                            MPEG_FMT_T *pt_mpeg_fmt)
{
    return;
}

/************************************************************************
*  AtdEU_TunerGetVer
*  Get current analog tuner driver version.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @retval  Current analog tuner driver version.
************************************************************************/
CHAR *AtdEU_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
	char *pVer;
	 ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
	
		mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);	// Avoid re-entry
	
		//mcSHOW_USER_MSG(("[ATuner] %s\n", AtdPIEU_TunerGetVer(cATUNER_PI_CTX(psAtdPdCtx))));
	
		mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
		ITuner_OP(psAtdeuDemodCtx->pTCtx, itGetVer, 0, &pVer);
		return ((char*)pVer);
	   // return "Analog_Tuner";

}


/************************************************************************
*  AtdEU_TunerNimTest
*  Test function of tuner driver.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @retval  Always 0.
************************************************************************/
INT32 AtdEU_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));

    return 0;
}

/************************************************************************
*  AtdEU_TunerSetRegSetting
*  Set tuner register.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  ucRegSet : Number of setting regsiters.
*  @param  ucRegAddr : Register address.
*  @param  ucRegValue : Register value.
*  @retval  void.
************************************************************************/
VOID AtdEU_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet,
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

/************************************************************************
*  AtdEU_TunerShowRegSetting
*  Show tuner register.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @retval  void.
************************************************************************/
VOID AtdEU_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

/************************************************************************
*  AtdEU_TunerTestI2C
*  Test tuner I2C bus.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @retval  void.
************************************************************************/
VOID AtdEU_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG(("%s\n", __FUNCTION__));
}

/************************************************************************
*  AtdEU_TunerRegRead
*  Read tuner register.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @param  ucRegAddr : Register address.
*  @param  *pucBuffer : Output data content pointer.
*  @param  ucByteCount : Number of setting regsiters.
*  @retval  Always 0.
************************************************************************/
INT32 AtdEU_TunerRegRead(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type,
                    UCHAR ucRegAddr, UCHAR *pucBuffer, U16 ucByteCount)
{
    return 0;
}

/************************************************************************
*  AtdEU_TunerDisc
*  Disconnect analog tuner driver.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_conn_type : Connection type.
*  @retval  void.
************************************************************************/
VOID  AtdEU_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                                DRV_CONN_TYPE_T e_conn_type)
{
    //#if fcENABLE_MONITOR
    ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;    
    //#endif
    if (psAtdPdCtx->fgDisStatus == TRUE)
        {
               /* Has Disconnected, return */
		mcSHOW_DBG_MSG(("ATD has Disconnect!\n"));
		return; 
        }
    mcSHOW_DBG_MSG4(("ATD_TunerDisc pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(psAtdPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape ATD_TunerDisc pPdCtx->hHalLock\n"));

    mcSHOW_DBG_MSG(("[Atuner] %s()\n", __FUNCTION__));

//    psAtdPdCtx->fgBreakMonitor = TRUE;  // break current monitor process
//#ifdef ANAEU_ENABLE_MONITOR
    SetBreakMonitor(psAtdPdCtx);
//#endif
    ATD_DisConnect(psAtdeuDemodCtx);

    
    mcMUTEX_UNLOCK(psAtdPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("ATD_TunerDisc unlock pPdCtx->hHalLock\n"));

	/* set disconnect status */
	psAtdPdCtx->fgDisStatus = TRUE;
	
    //fgIsDisconnected = TRUE;

//#ifdef TUNER_SUPPORT_SCART_OUT
    // Please do not do anything for SCART-feature enabled.
    // When input source change between TV and SCART, it required SCART-Out ATV to be not discontinuity.
    return;
//#endif
}
/************************************************************************
*  AtdEU_TunerCommand
*  Analog tuner nim command.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  i4Argc : Nim command parameter number.
*  @param  aszArgv : Nim command parameter.
*  @retval  void.
************************************************************************/
void AtdEU_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                      INT32 i4Argc, const CHAR** aszArgv)
{
#if fcADD_HAL_LOCK
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
#endif
UCHAR   ucCmdId = 0;
UINT16 u2Data;
UINT8 u1fgMask=1;
    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    if (i4Argc < 1)
    {
	    mcSHOW_USER_MSG(("eq:read out current EQ coefficient \n"));//d110327
		mcSHOW_USER_MSG(("d:Dump ATD register \n"));//d110330_Haibo:Add command to dump ATD register
    }

    if (i4Argc > 0)
    {
        ucCmdId = *((CHAR *) aszArgv[0]);

        switch (ucCmdId)
        {
		case 'e':
		if('q'==((CHAR *) aszArgv[0])[1])
		{
		   ATD_GetEQ(psAtdeuDemodCtx);
		}
		break;
		case 'd':
		{
		    u2Data=0;
		   if (i4Argc > 2)
		   {
		      u1fgMask=(UINT8)StrToInt(aszArgv[2]);
		   }
		   else if (i4Argc > 1)
		   {
		      u2Data=(UINT16)StrToInt(aszArgv[1]);
		   }
		   if(u1fgMask)
		   {
		     u2Data|=0x8000;
		   }
		     ATD_DumpRegister(psAtdeuDemodCtx,u2Data); 
		}
		break;
/*            case 'o':
                if (i4Argc > 1)
                {
                //    i4FreqOffset = axtoi((CHAR *) aszArgv[1]);
                    i4FreqOffset = StrToInt(aszArgv[1]);
                    if (i4Argc > 2)
                        i4FreqOffset = -i4FreqOffset;
                }
                mcSHOW_USER_MSG(("i4FreqOffset = %d\n", i4FreqOffset));
                break;
*/
            default:
                mcSHOW_USER_MSG(("Invalid parameter!\n"));
                break;
        }
    }

    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
    //AtdEU_TunerAtdCommand(ptTDSpecificCtx, i4Argc, aszArgv);//d110327_Haibo:Seperate TunerATDCommand  from TunerCommand
}

/************************************************************************
*  AtdEU_TunerAtdCommand
*  Analog tuner ATD command.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  i4Argc : ATD command parameter number.
*  @param  aszArgv : ATD command parameter.
*  @retval  void.
************************************************************************/
VOID AtdEU_TunerAtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        INT32 i4Argc, const CHAR** aszArgv)
{
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
//GEN_TUNER_CTX_T *pDigiTunerCtx;
UCHAR   ucCmdId = 0, ucPar, ucData[256];
UINT32  i4CurFreq, i4BndFreq;
UINT32  u4Freq = 0;
UINT8   u1SubSysIdx, ii, jj, kk;
U16     u2RegAddr, u2Period;
UINT8   ucData1 = 0;
UINT8   ucData2 = 0;
UINT8   u1Count;
S32     s2Data;
U32     u2Data;
#ifndef CC_MT5360
UINT32  u4Temp;
BOOL    fgFMstate;
UINT8   ucFMdir;
U16     u2FMstep;
#endif
UINT16  u2State;
UINT8   i4CurTop =0;
UINT8 temp[2];
UINT8 RegistWord[5];
UINT8 i4CurAntIn;
INT8 i1Top;


    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
		#if defined(CC_MT5389) 
		mcSHOW_USER_MSG(("\t k: Adjust the DRO target level adaptivily\n"));
		#endif
        mcSHOW_USER_MSG(("\t c:              Show Current Freq (kHz)\n"));    //CR 00012779, for Hisense's request, LC Chien 070514
        mcSHOW_USER_MSG(("\t e [Register (UINT8)]: Set or Get 5 Register,The CLI input formats are: nim.atd e or nim.atd e ##-##-##-##-##\n"));
		mcSHOW_USER_MSG(("\t t [Top]: SetTOP\n"));
        mcSHOW_USER_MSG(("\t f [Freq (kHz)]: AnaTunerSetFreq\n"));
        mcSHOW_USER_MSG(("\t s [SubSysIdx]:  Color / Sound sub-system\n"));
        mcSHOW_USER_MSG(("\t v [Freq (kHz)]: Check Lock Status\n"));
        mcSHOW_USER_MSG(("\t h [Start Freq (kHz)][End Freq (kHz)][Count]: TunerSearchNearbyFreq\n"));
        mcSHOW_USER_MSG(("\t r [RegAddr] [Num]:   read  ATD register\n"));
        mcSHOW_USER_MSG(("\t w [RegAddr] [Value]: write ATD register\n"));
        //mcSHOW_USER_MSG(("\t t [Patch timing]:   Set FW patch timing \n"));
        mcSHOW_USER_MSG(("\t g [Count] [Period (ms)]: Get current ATD status \n"));
        mcSHOW_USER_MSG(("\t p [CR loop bandwidth]: Set CR loop bandwidth \n"));
		mcSHOW_USER_MSG(("\t pd Set TVD re-check interval \n"));
        mcSHOW_USER_MSG(("\t pe g_CPR_cnt\n"));
        mcSHOW_USER_MSG(("\t pg g_pra_cnt\n"));
        mcSHOW_USER_MSG(("\t pf g_CPR_delay\n"));
        mcSHOW_USER_MSG(("\t ph g_CR_BW_AutoConfig_delay_cnt\n"));
		mcSHOW_USER_MSG(("\t pm patch mask setting \n"));
        mcSHOW_USER_MSG(("\t n [0/1]: Disable/enable new scan step\n"));
        //mcSHOW_USER_MSG(("\t e [Start Freq (kHz)][End Freq (kHz)][Count]: Channel scan uni-test\n"));
        mcSHOW_USER_MSG(("\t a [Freq (kHz)]: FM radio test \n"));
        mcSHOW_USER_MSG(("\t j [Start Freq (kHz)][0/1]: FM radio scan up/down\n"));
        mcSHOW_USER_MSG(("\t l [0/1]: Set LNA on/off \n"));
        mcSHOW_USER_MSG(("\t x:             SLT test    \n"));
		mcSHOW_USER_MSG(("\t i Set Antenna [1/2] in Antenna 1/2 int\n"));
		mcSHOW_USER_MSG(("\t b Bypass SIF to aud\n"));
		mcSHOW_USER_MSG(("\t u Reset Tuner\n"));
//#ifdef ANAEU_ENABLE_MONITOR
        if(pAtdTunerDeviceCtx->u1MWIntf != cMTAL_IF && pAtdTunerDeviceCtx->u1MWIntf != cSDAL_IF)
        {
        mcSHOW_USER_MSG(("\t mp:             Pause monitor thread\n"));
        mcSHOW_USER_MSG(("\t mr:             Resume monitor thread\n"));
        }
//#endif
    }

    if (i4Argc > 0)
    {
        ucCmdId = *((CHAR *) aszArgv[0]);

        switch (ucCmdId)
        {
            #if defined(CC_MT5389) 
            case 'k':
			{	if ((StrToInt(aszArgv[1])<=1) || (StrToInt(aszArgv[2])<= 2)||(StrToInt(aszArgv[3]) <= 0)||(StrToInt(aszArgv[4]) > 0))
				{   
			        ATD_DroTargetLevelModifySyncH(psAtdeuDemodCtx, StrToInt(aszArgv[1]), StrToInt(aszArgv[2]), StrToInt(aszArgv[3]), StrToInt(aszArgv[4]), u2DroTarLvlInitial);	
		    		//StrToInt(aszArgv[1]:fgsel=1:DRO target; else==>Fref2
			    	//StrToInt(aszArgv[2]:ACC_MON_Count=0==>1, 1==>2, 2==>4; else==>0
    				//StrToInt(aszArgv[3]:Reg_autoSet=1==>set;else==>non-set				                
	    			////StrToInt(aszArgv[4]: target sync height (cal value)
				    
				}
			    else
				{
                    mcSHOW_USER_MSG(("\t nim.atd k X1 X2 X3 X4!! \n"));   
    				mcSHOW_USER_MSG(("\t  X1: 1/0==>Using the DRO_target_level/ FREF2!! \n"));
	    			mcSHOW_USER_MSG(("\t  X2: ACC-avg:0/1/2==>1/2/4 times!! \n"));
		    		mcSHOW_USER_MSG(("\t  X3: Autocgahge the DRO targer level:0/1==>En/Disable!! \n"));
			    	mcSHOW_USER_MSG(("\t  X4: fine-tune the target weight-syncH(Suggest value is 3400)!! \n"));
				}
			}
			break;
			#endif
		    case 'i':
            {
				i4CurFreq = psAtdPdCtx->Frequency;
			    i4CurAntIn= (U8)StrToInt((CHAR *) aszArgv[1]);
				ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetAntIn, i4CurAntIn, &ucData1); 
				ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
				mcSHOW_USER_MSG(("Set Antenna %d in \n", i4CurAntIn));
            }
            break;

            case 'q':
            {
            	UINT32 ucPar1 = (U32)StrToInt(aszArgv[1]);	
            	UINT32 ucPar2 = (U32)StrToInt(aszArgv[2]);
            	UINT32 ucPar3 = (U32)StrToInt(aszArgv[3]);
            	Qlab_SetMonIndicator(ucPar1,ucPar2,ucPar3);
            }
            break;

			
			case 'd':
							
							if(StrToInt(aszArgv[1])==1)				 
							{					 
								DeScrambleBox=TRUE;
								DeScrambleBoxDelay = 1000;
			#if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
							   ATD_SetDelayLine(psAtdeuDemodCtx,TRUE); 
			#endif
								mcSHOW_USER_MSG(("SET DeScramble box on!!!!")); 			   
							}				 
							else if (StrToInt(aszArgv[1])==0 )
							{
							   DeScrambleBox=FALSE;
							  
							   DeScrambleBoxDelay = 0;
			#if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
							   ATD_SetDelayLine(psAtdeuDemodCtx,FALSE); 
			#endif
							   mcSHOW_USER_MSG(("SET DeScramble box off!!!!")); 
							}
							else
							mcSHOW_USER_MSG(("\t d Set descrambox [0/1] [off/on]\n"));	
			break;

            case 'c':
                i4CurFreq = psAtdPdCtx->Frequency;
                mcSHOW_USER_MSG(("Current Freq = %d kHz\n", i4CurFreq));
                break;

            /*case 't':
                if (i4Argc > 1)
                {
                    psAtdeuDemodCtx->fgMonitor_500ms = (U8)StrToInt(aszArgv[1]);
                }
                else
                {
                    mcSHOW_USER_MSG(("Current FW patch timing = %d (0: 2ms, 1: 500ms)\n", psAtdeuDemodCtx->fgMonitor_500ms));
                }

                break;*/

            case 'f':
                i4CurFreq = psAtdPdCtx->Frequency;
                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    psAtdPdCtx->Frequency = i4CurFreq;
                }
                ATD_ChipInit(psAtdeuDemodCtx);
                ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                ATD_StartAcq(psAtdeuDemodCtx, FALSE);
                //AnalogPIEU_TunerSetFreq(cATUNER_PI_CTX(psAtdPdCtx), i4CurFreq, ATV_CONN_MODE_CHAN_CHANGE);
                mcSHOW_USER_MSG(("Set Frequency = %d KHz\n", i4CurFreq));
                break;

			case 'e':

			 {
                 
                if (i4Argc == 2)
			      {
                      //mcSHOW_USER_MSG(("the input argc is %d \n ",i4Argc));
				      //mcSHOW_USER_MSG(("the input string argv is %s \n ",(CHAR *) aszArgv[1]));
				      ii = 0;
				      jj = 0;
				      kk = 0;
				      while ( (((CHAR *) aszArgv[1])[ii] != '\0') &&( kk < 5))
                   	   {
                         while((((CHAR *) aszArgv[1])[ii] != '-') && (((CHAR *) aszArgv[1])[ii] != '\0')&& (jj < 2))
                          { 
                             temp[jj] = ((CHAR *) aszArgv[1])[ii];
					         ii++;
						     jj++;
						   
                     	  }
                         if(jj > 2)
						   {
                            mcSHOW_USER_MSG(("error!One word's format is ## \n")); 
							break;
						   }
					
                          if (kk >= 5)
                          	{
						       mcSHOW_USER_MSG(("error! the format is ##-##-##-##-##,just 5 words \n")); 
							   break;
						    } 
						  RegistWord[kk] = (U16)StrToHex((CHAR *)&temp,jj);
						  mcSHOW_USER_MSG(("the input word is 0x%x\n",RegistWord[kk])); 
						  kk++;
					      jj = 0;
						  ii++;
					    }
                      if ((jj <= 2) &&( kk <= 5))
                      	{
				          ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetReg,0, &RegistWord); 
			              mcSHOW_USER_MSG(("Set Register complete!\n"));
                      	}
                }
				
                 else if (i4Argc == 1)
                   	{     
                   	      for (ii = 0; ii < 5; ii++)
						  	RegistWord[ii] = 0;
                          ITuner_OP(psAtdeuDemodCtx->pTCtx, itGetReg,0, RegistWord); 
                          for (ii = 0; ii < 5; ii++)
						  mcSHOW_USER_MSG(("the output word is 0x%x\n",RegistWord[ii])); 	
				    }
                   
			    else
			    {
			  	   mcSHOW_USER_MSG(("The CLI input formats are: nim.atd e or nim.atd e ##-##-##-##-## \n"));
			 }
			}
                break;

			
			case 't':
 
			    if (i4Argc ==2)
				  {
				     i4CurFreq = psAtdPdCtx->Frequency;
					 i4CurTop= (U8)StrToInt((CHAR *) aszArgv[1]);
					  if(psAtdeuDemodCtx->u1TunerType==cCUST_TUNER)//Customized tuner   
                     {
                      i1Top	= (INT8)StrToInt((CHAR *) aszArgv[1]);				 
                      if(ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetTop, psAtdPdCtx->u1SubSysIdx,&i1Top)==ITUNER_OK) 
                      {
					   ATD_SetSystem(psAtdeuDemodCtx, psAtdPdCtx->u1SubSysIdx);					 
                      }
                     }
					 else
					 {
					 ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetTop, i4CurTop, &(psAtdPdCtx->u1SubSysIdx)); 
					 ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
					  mcSHOW_USER_MSG(("Set Top = 0x%x\n", i4CurTop));
					 }	
				  }
				 else
				 {
				  if(ITuner_OP(psAtdeuDemodCtx->pTCtx, itGetTop, psAtdPdCtx->u1SubSysIdx, &i1Top)==ITUNER_OK)
				  {
				   mcSHOW_USER_MSG(("Current Top = 0x%x sys_index=%d\n", i1Top,psAtdPdCtx->u1SubSysIdx));
				  }
				 }
				break;
				case 'b':
				{
				 if (i4Argc > 1)
                {
                //    _SubSysIdx = StrToInt(aszArgv[1]);
                    u1SubSysIdx = StrToInt(aszArgv[1]);
					ATD_BypassSIF2Aud(psAtdeuDemodCtx,u1SubSysIdx);
				}
				else
				{
				     mcSHOW_USER_MSG(("\t %d: PAL_BG  \n", MOD_PAL_BG));
                    mcSHOW_USER_MSG(("\t %d: PAL_DK  \n", MOD_PAL_DK));
                    mcSHOW_USER_MSG(("\t %d: PAL_I   \n", MOD_PAL_I));
                    mcSHOW_USER_MSG(("\t %d: SECAM_L \n", MOD_SECAM_L));
                    mcSHOW_USER_MSG(("\t %d: SECAM_L1\n", MOD_SECAM_L1));
                    mcSHOW_USER_MSG(("\t %d: NTSC_M  \n", MOD_NTSC_M));
					mcSHOW_USER_MSG(("\t %d: Unkown  \n", MOD_TYPE_UNKNOWN));
				}
				}
				break;
			   case 'u':
				{
                      ATD_TunerReset(psAtdeuDemodCtx);
					  mcSHOW_DBG_MSG(("ATV Reset Tuner OK!"));
				}
				break;
			   case 'y':
				{
				      UINT8   ucData1;
					  u2RegAddr = (U8)StrToInt(aszArgv[1]);
                      ucPar = (U8)StrToInt(aszArgv[2]);
                      ITuner_OP(psAtdeuDemodCtx->pTCtx, itTunerRead,u2RegAddr, &ucData1); 
					  mcSHOW_USER_MSG(("Register 0x%02X = 0x%02X\n",u2RegAddr, ucData1));
				}
				break;	

            case 's':

                u1SubSysIdx = psAtdPdCtx->u1SubSysIdx;
                if (i4Argc > 1)
                {
                //    _SubSysIdx = StrToInt(aszArgv[1]);
                    u1SubSysIdx = StrToInt(aszArgv[1]);
                    //if (u1SubSysIdx < SYS_SUB_TOTAL)
                    //{
                        psAtdeuDemodCtx->u1SubSysIdx = u1SubSysIdx + MOD_ANA_TYPE_BEGIN;	//modified for get the correct   s4DsmOutFreq
                    	i4CurFreq = psAtdPdCtx->Frequency;
						ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
						
                        psAtdPdCtx->u1SubSysIdx = u1SubSysIdx + MOD_ANA_TYPE_BEGIN;
                        ATD_ChipInit(psAtdeuDemodCtx);   // make sure ATD in normal mode after nim.go dvbt
                        ATD_SetSystem(psAtdeuDemodCtx, psAtdPdCtx->u1SubSysIdx);
						if ((psAtdPdCtx->u1SubSysIdx==MOD_SECAM_L) ||(psAtdPdCtx->u1SubSysIdx==MOD_SECAM_L1))// to get the initial DRO target level
                        {
                            ATD_GetReg(psAtdeuDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1); 
                            u2DroTarLvlInitial = 0x0000;
                		    u2DroTarLvlInitial |= (UINT16)ucData1;
                    		u2DroTarLvlInitial = (u2DroTarLvlInitial << 8);
                	    	ATD_GetReg(psAtdeuDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData1, 1);
                		    u2DroTarLvlInitial |= (UINT16)ucData1;
                			u2DroTarLvlInitial &= 0x7FFF;
                    		mcSHOW_DBG_MSG1(("The SECAML/L1 DRO target level(initial value) = 0x%04X\n", u2DroTarLvlInitial));
                        }
                        //ATD_StartAcq(psAtdeuDemodCtx);
                        //AnalogPIEU_TunerSetSystem(cATUNER_PI_CTX(psAtdPdCtx), u1SubSysIdx);
                    //}
                }
                else
                {
                    mcSHOW_USER_MSG(("\t %d: PAL_BG  \n", MOD_PAL_BG - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: PAL_DK  \n", MOD_PAL_DK - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: PAL_I   \n", MOD_PAL_I - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: SECAM_L \n", MOD_SECAM_L - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: SECAM_L1\n", MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN));
                    mcSHOW_USER_MSG(("\t %d: NTSC_M  \n", MOD_NTSC_M - MOD_ANA_TYPE_BEGIN));
                    //mcSHOW_USER_MSG(("\t 7: NTSC_54M  \n"));
                }
                mcSHOW_USER_MSG(("_SubSysIdx = %d\n", psAtdPdCtx->u1SubSysIdx - MOD_ANA_TYPE_BEGIN));
#ifdef  EU_AUTO_ATP
               
			   
             if( gATP_TEST_ITEM_BIT &0x08)
             	{
             	UINT8 bVIFLock,bTVDLock;
		i4CurFreq = psAtdPdCtx->Frequency;
		
               bVIFLock =  ATD_GetVIFLock(psAtdeuDemodCtx, TRUE);
		 bTVDLock =   fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME);
		 
              mcSHOW_DBG_MSG(("##Start\n##CurFreq : %d\n##VIFLOCK : %d\n##TVDLOCK : %d\n##SubSysIdx : %d\n",i4CurFreq,bVIFLock,bTVDLock,psAtdPdCtx->u1SubSysIdx - MOD_ANA_TYPE_BEGIN));

		     if (gATP_TEST_ITEM_BIT & 0x10)
     	           {
                     if (!gATPFailBitMsk)
         	       {
                      mcSHOW_DBG_MSG(("##ATPFAIL : 0\n##End\n"));
         	       }
	             else
	  	      {
                      mcSHOW_DRVAPI_MSG(("##ATPFAIL : 1 , Msk =%x\n##End\n",gATPFailBitMsk));
		         //reset to 0
		        gATPFailBitMsk = 0;
	  	      }
     	           }
		    else
		    	{
                     mcSHOW_DRVAPI_MSG(("##End\n"));
		    	}
             	}
			 			 
		 else if(gATP_TEST_ITEM_BIT & 0x01)
		{
                mcSHOW_DBG_MSG(("##Start\n##SubSysIdx : %d\n##End\n",psAtdPdCtx->u1SubSysIdx - MOD_ANA_TYPE_BEGIN));
		}
		 
#endif
                break;

            case 'v':
                i4CurFreq = psAtdPdCtx->Frequency;

                //ATD_ChipInit(psAtdeuDemodCtx);

                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    psAtdPdCtx->Frequency = i4CurFreq;
                    ATD_ChipInit(psAtdeuDemodCtx);
                    ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                    ATD_StartAcq(psAtdeuDemodCtx, FALSE);
                    //mcDELAY_MS(50);
                    //ATD_SetScanMode(psAtdeuDemodCtx);
                }
                //ATD_StartAcq(psAtdeuDemodCtx);
                //for(ii=1;ii<10;ii++)
                //{
               psAtdeuDemodCtx->fgCheckCR=TRUE;//to avoid VIf lock indicator error
                mcSHOW_USER_MSG(("VIF Lock: %d (Freq = %dKHz)\n",
                                 ATD_GetVIFLock(psAtdeuDemodCtx, TRUE),
                                 //AnalogPIEU_TunerCheckVIFLock(cATUNER_PI_CTX(psAtdPdCtx)),
                                 i4CurFreq));
                //mcDELAY_MS(10);
                //}
                mcSHOW_USER_MSG(("TVD Lock: %d\n", fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME)));
#ifdef  EU_AUTO_ATP	
               UINT8 bVIFLock,bTVDLock;
               bVIFLock =  ATD_GetVIFLock(psAtdeuDemodCtx, TRUE);
		 bTVDLock =   fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME);

		  if (gATP_TEST_ITEM_BIT & 0x04)
		 {   //only available in nim.atd p 4(bit 2 =1)
		    if ((bVIFLock ==2) && (bTVDLock ==1))
		   {
                   mcSHOW_DBG_MSG(("##Start\n##Video : ON\n##End\n"));  
		    }
		    else
		   {
                  mcSHOW_DBG_MSG(("##Start\n##Video : OFF\n##End\n"));  
		    }
		} 
		  else if (gATP_TEST_ITEM_BIT & 0x01)
		  {
		      mcSHOW_DBG_MSG(("##Start\n##VIFLOCK : %d\n##TVDLOCK : %d\n##End\n", bVIFLock,  bTVDLock));	
		  }
#endif 				
                break;

            case 'h':
              //  _fgAutoSearch = TRUE;
                jj = 0;
                kk = 0;
                i4CurFreq = psAtdPdCtx->Frequency;

                //ATD_ChipInit(psAtdeuDemodCtx);

                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    i4BndFreq = StrToInt(aszArgv[2]);
                    u1Count = StrToInt(aszArgv[3]);
                    ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                    //AnalogPIEU_TunerSetFreq(cATUNER_PI_CTX(psAtdPdCtx), i4CurFreq, ATV_CONN_MODE_CHAN_CHANGE);
                }
                else
                {
                    i4BndFreq = i4CurFreq;
                    u1Count = 1;
                }
                //u4Freq = i4CurFreq;
                ATD_StartAcq(psAtdeuDemodCtx, TRUE);
                //mcDELAY_MS(20);
                //ATD_SetScanMode(psAtdeuDemodCtx);
                //psAtdeuDemodCtx->fgCheckCR = TRUE;
                for(ii=0; ii<u1Count; ii++)
                {
                    u4Freq = i4CurFreq;
                    mcSHOW_USER_MSG(("the %d th time SearchNearFreq.\n",ii));
                    psAtdeuDemodCtx->fgCheckCR = TRUE;
                    if(ATDEU_SearchNearbyFreq(psAtdeuDemodCtx, &u4Freq, TunerNullBreak, NULL, i4BndFreq))
                    {
                        //if(AtdPIEU_fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME, psAtdPdCtx->u1SubSysIdx))
                        if (fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME)) 
                        {
                            jj++;  //VIF lock, TVD lock
                        }
                        else
                        {
                            kk++;  //VIF lock, but TVD unlock
                        }
                    }                        

                    if(ii <(u1Count -1))
                    {
                        ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                        ATD_StartAcq(psAtdeuDemodCtx, TRUE); 
                    }     
                
                }
                //AnalogPIEU_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtdPdCtx), &u4Freq, TunerNullBreak, NULL, u4Freq);
                mcSHOW_USER_MSG(("Search NearBy Before %d After %d KHz\n", i4CurFreq, u4Freq));
                mcSHOW_USER_MSG(("Search NearBy %d times from %d KHz to %d KHz\n", u1Count, i4CurFreq, i4BndFreq));
                mcSHOW_USER_MSG(("VIF lock times = %d, TVD lock times = %d\n", jj+kk, jj));                
                
                psAtdPdCtx->Frequency = u4Freq;
               // _fgAutoSearch = FALSE;
                break;
            #if 0    
            case 'e':
                //_fgAutoSearch = TRUE;
                i4CurFreq = psAtdPdCtx->Frequency;
                                
                //ATD_ChipInit(psAtdeuDemodCtx);

                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    i4BndFreq = StrToInt(aszArgv[2]);
                    u1Count = StrToInt(aszArgv[3]);
                    ATD_SetSystem(psAtdeuDemodCtx, SYS_SUB_PAL_BG);
                    ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                }
                else
                {
                    i4BndFreq = i4CurFreq;
                    u1Count = 1;
                }
                //u4Freq = i4CurFreq;
                ATD_StartAcq(psAtdeuDemodCtx, TRUE);
                //mcDELAY_MS(20);
                //ATD_SetScanMode(psAtdeuDemodCtx);
                //psAtdeuDemodCtx->fgCheckCR = TRUE;
                for(ii=0; ii<u1Count; ii++)
                {
                    u4Freq = i4CurFreq;
                    mcSHOW_USER_MSG(("the %d th time SearchNearFreq.\n",ii));
                    psAtdeuDemodCtx->fgCheckCR = TRUE;
                    if(ATDEU_SearchNearbyFreq(psAtdeuDemodCtx, &u4Freq, TunerNullBreak, NULL, i4BndFreq))
                    {
                        mcSHOW_USER_MSG(("SearchNearBy success at %dKHz!\n", u4Freq));
                        mcSHOW_USER_MSG(("TV system = B/G, check TVD\n"));
                        //if(AtdPIEU_fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME, SYS_SUB_PAL_BG))
                        if (fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME)) 
                        {                            
                            mcDELAY_MS(500);
                            u4Temp = mcRISC_Reg(cRISC_TVD_BASE, 0x080);
                            mcSHOW_USER_MSG(("525/625 line = 0x%08x\n",u4Temp));
                            if((u4Temp & 0x04000000) == 0x04000000)
                            {
                                mcSHOW_USER_MSG(("TV system = PAL, but signal is NTSC!\n"));
                            }
                        }
                        ATD_SetSystem(psAtdeuDemodCtx, SYS_SUB_NTSC_M);
                        ATD_StartAcq(psAtdeuDemodCtx, TRUE);
                        mcSHOW_USER_MSG(("TV system = M, check TVD\n"));
                        //if(AtdPIEU_fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME, SYS_SUB_NTSC_M))
                        if (fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME))
                        {                            
                            mcDELAY_MS(500);
                            u4Temp = mcRISC_Reg(cRISC_TVD_BASE, 0x080);
                            mcSHOW_USER_MSG(("525/625 line = 0x%08x\n",u4Temp));
                            if((u4Temp & 0x04000000) != 0x04000000)
                            {
                                mcSHOW_USER_MSG(("TV system = NTSC, but signal is PAL!\n"));
                            }
                        }     
                        ATD_SetSystem(psAtdeuDemodCtx, SYS_SUB_PAL_DK);
                        ATD_StartAcq(psAtdeuDemodCtx, TRUE);
                        mcSHOW_USER_MSG(("TV system = D/K, check TVD\n"));
                        //if(AtdPIEU_fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME, SYS_SUB_PAL_DK))
                        if (fgGetTVDSts(psAtdeuDemodCtx,CHN_LOCK_CHECK_TIME))
                        {                            
                            mcDELAY_MS(500);
                            u4Temp = mcRISC_Reg(cRISC_TVD_BASE, 0x080);
                            mcSHOW_USER_MSG(("525/625 line = 0x%08x\n",u4Temp));
                            if((u4Temp & 0x04000000) == 0x04000000)
                            {
                                mcSHOW_USER_MSG(("TV system = PAL, but signal is NTSC!\n"));
                            }
                        }    
                            
                    }                        

                    if(ii <(u1Count -1))
                    {
                        ATD_SetSystem(psAtdeuDemodCtx, SYS_SUB_PAL_BG);
                        ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                        ATD_StartAcq(psAtdeuDemodCtx, TRUE); 
                    }     
                
                }
                //AnalogPIEU_TunerSearchNearbyFreq(cATUNER_PI_CTX(psAtdPdCtx), &u4Freq, TunerNullBreak, NULL, u4Freq);
                //mcSHOW_USER_MSG(("Search NearBy Before %d After %d KHz\n", i4CurFreq, u4Freq));
                //mcSHOW_USER_MSG(("Search NearBy %d times from %d KHz to %d KHz\n", u1Count, i4CurFreq, i4BndFreq));
                //mcSHOW_USER_MSG(("VIF lock times = %d, TVD lock times = %d\n", jj+kk, jj));                
                
                psAtdPdCtx->Frequency = u4Freq;
              //  _fgAutoSearch = FALSE;
                break;
            #endif        
            case 'r':
                if (i4Argc < 3)
                    break;

                u2RegAddr = (U16)StrToInt(aszArgv[1]);
                ucPar = (U8)StrToInt(aszArgv[2]);
                if (ATD_GetReg(psAtdeuDemodCtx, u2RegAddr, ucData, ucPar))
                {
                    mcSHOW_USER_MSG(("MTATD Get Register 0x%03X Fail!\n", u2RegAddr));
                    break;
                }
                for (ii=0; ii<ucPar; ii++)
                {
                    mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X\n", u2RegAddr+ii, ucData[ii]));
                }
                break;

            case 'w':
                if (i4Argc < 3)
                    break;

                u2RegAddr = (U16)StrToInt(aszArgv[1]);
                ucPar = (U8)StrToInt(aszArgv[2]);
                if (ATD_SetReg(psAtdeuDemodCtx, u2RegAddr, &ucPar, 1))
                {
                    mcSHOW_USER_MSG(("MTATD Set Register 0x%03X Fail!\n", u2RegAddr));
                    break;
                }
                mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X, OK!\n", u2RegAddr, ucPar));
                break;

            case 'g':
                if (i4Argc == 1)
                {
                    u1Count = 1;
                    u2Period = 0;     	
                }
                else if (i4Argc == 2)
                {
                    u1Count = StrToInt(aszArgv[1]);
                    u2Period = 1000;                     
                }
                else if (i4Argc == 3)
                {
                    u1Count = StrToInt(aszArgv[1]);
                    u2Period = StrToInt(aszArgv[2]);                       
                }
                else
                {
                    break;
                }
                
                for (ii=0; ii<u1Count; ii++)
                {
                 ATD_GetInfo(psAtdeuDemodCtx);                           
                if( ii < u1Count-1 )
                {
                    mcDELAY_MS(u2Period);
                } 
                }
                                        
                break;
            case 'p':
                if('d'==((CHAR *) aszArgv[0])[1])
                {
                    if (i4Argc > 1)
                    {
                        psAtdeuDemodCtx->pTCtx->u2RetryTVDTm = (UINT16)StrToInt(aszArgv[1]);
                    }
                    mcSHOW_USER_MSG(("Re-try TVD delay = %d ms\n",psAtdeuDemodCtx->pTCtx->u2RetryTVDTm)); 
                }
                else if('e'==((CHAR *) aszArgv[0])[1])
                {
                    if (i4Argc > 1)
                    {
                        g_CPR_cnt = (UINT16)StrToInt(aszArgv[1]);
                    }
                    mcSHOW_USER_MSG(("reset g_CPR_cnt = %d\n",g_CPR_cnt)); 
                }
                else if('g'==((CHAR *) aszArgv[0])[1])
                {
                    if (i4Argc > 1)
                    {
                        g_pra_cnt = (UINT16)StrToInt(aszArgv[1]);
                    }
                    mcSHOW_USER_MSG(("reset g_pra_cnt = %d \n",g_pra_cnt)); 
                }
                else if('f'==((CHAR *) aszArgv[0])[1])
                {
                    if (i4Argc > 1)
                    {
                        g_CPR_delay = (UINT16)StrToInt(aszArgv[1]);
                    }
                    mcSHOW_USER_MSG(("reset g_CPR_delay = %d\n",g_CPR_delay)); 
                }
                else if('h'==((CHAR *) aszArgv[0])[1])
                {
                    if (i4Argc > 1)
                    {
                        g_CR_BW_AutoConfig_Delay_cnt = (UINT16)StrToInt(aszArgv[1]);
                    }
                    mcSHOW_USER_MSG(("reset g_CR_BW_AutoConfig_Delay_cnt = %d\n",g_CR_BW_AutoConfig_Delay_cnt)); 
                }
				else if('m'==((CHAR *) aszArgv[0])[1])
				{
				 if (i4Argc > 1)
                    {
                        psAtdeuDemodCtx->pTCtx->u1AtdPatchSwitch = (UINT8)StrToInt(aszArgv[1]);
                    }
				else	
					{
					mcSHOW_USER_MSG(("patch for CR:0x%02X\n",MASK_PATCH_CR));
					mcSHOW_USER_MSG(("patch for DRO:0x%02X\n",MASK_PATCH_DRO));
					mcSHOW_USER_MSG(("patch for CCI:0x%02X\n",MASK_PATCH_CCI));
					mcSHOW_USER_MSG(("patch for Sensitivity:0x%02X\n",MASK_PATCH_SENS));
					mcSHOW_USER_MSG(("patch for StrongChroma:0x%02X\n",MASK_PATCH_STCHR));
					mcSHOW_USER_MSG(("patch for SoundBeat:0x%02X\n",MASK_PATCH_SBEAT));
                   	}
                    mcSHOW_USER_MSG(("Current Patch mask = 0x%02X \n",psAtdeuDemodCtx->pTCtx->u1AtdPatchSwitch));  
				}
                else
                {
                    if (i4Argc > 1)
                    {
                        ucData1 = 0x00;
                        ATD_SetReg(psAtdeuDemodCtx, 0x00A, &ucData1, 1);

                        ucData2 = StrToInt(aszArgv[1]);
                        ATD_SetAutoConfig(psAtdeuDemodCtx, 0); //turn-off PN & FN auto-config            
                        ATD_SetCrBw(psAtdeuDemodCtx, (ucData2 & 0xFF));
                        mcSHOW_USER_MSG(("Set CR loop bandwidth as %d\n", ucData2)); 
                    }
                } 
                break;    
#ifndef CC_MT5360
            case 'a':
                if (i4Argc > 1)
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                }
                else
                {
                    i4CurFreq = psAtdPdCtx->Frequency;
                } 
                u4Temp = 0xffffffff;
                IO_WRITE32(cRISC_TVD_BASE, 0x40C, u4Temp);                
//    #ifdef ANAEU_ENABLE_MONITOR                                   
                SetBreakMonitor(psAtdPdCtx);
//    #endif                
                SetFMRadio(psAtdeuDemodCtx, u4Freq);
                ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                ATD_StartAcq(psAtdeuDemodCtx, FALSE);
                fgFMstate = _AudFMRadioDetection();
                mcSHOW_USER_MSG(("Set Frequency = %d KHz, FM status = %d\n", i4CurFreq, fgFMstate));                                            
                if (fgFMstate)
                {
                    _AudPlayFMRadio(); 
                    mcSHOW_USER_MSG(("Play FM radio channel by tuner driver...\n"));   
                }                                                                        
            break;
            
            case 'j':
                if (i4Argc == 2 )
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    ucFMdir = 0;
                }
                else if(i4Argc == 3 )
                {
                    i4CurFreq = StrToInt(aszArgv[1]);
                    ucFMdir = StrToInt(aszArgv[2]);                    
                }
                else
                {
                    break;
                }     
                
                if(ucFMdir)
                {
                    u2FMstep = (i4CurFreq - 88000)/50;
                    i4CurFreq = i4CurFreq - 100;
                }
                else
                {
                    u2FMstep = (108000 - i4CurFreq)/50;
                    i4CurFreq = i4CurFreq + 100;
                }
                
                u4Temp = 0xffffffff;
                IO_WRITE32(cRISC_TVD_BASE, 0x40C, u4Temp);   
//            #ifdef ANAEU_ENABLE_MONITOR                                                             
                SetBreakMonitor(psAtdPdCtx);
//            #endif
                SetFMRadio(psAtdeuDemodCtx, u4Freq);
                
                for(ii=0; ii<=u2FMstep; ii++)
                {
                    ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                    ATD_StartAcq(psAtdeuDemodCtx, FALSE);
                    
                    fgFMstate = _AudFMRadioDetection();
                    
                    mcSHOW_USER_MSG(("Set Frequency = %d KHz, FM status = %d\n", i4CurFreq, fgFMstate)); 
                    if(fgFMstate)
                    {
                        if((i4CurFreq - (i4CurFreq/100)*100) != 0 )  //found FM radio at xxx.xMHz
                        {                            
                            if(ucFMdir)
                            {
                                ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq-50, FALSE);
                            }
                            else
                            {
                                ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq+50, FALSE);
                            }                                
                            ATD_StartAcq(psAtdeuDemodCtx, FALSE);
                            
                            if(_AudFMRadioDetection())
                            {
                                if(ucFMdir)
                                {
                                    i4CurFreq = i4CurFreq - 50;
                                    mcSHOW_USER_MSG(("Jump -50 KHz!!\n"));
                                }
                                else
                                {
                                    i4CurFreq = i4CurFreq + 50;
                                    mcSHOW_USER_MSG(("Jump +50 KHz!!\n"));    
                                }
                            }                                                
                        }        
                        mcSHOW_USER_MSG(("Found FM radio at %d KHz!!\n", i4CurFreq));
                        _AudPlayFMRadio(); 
                        mcSHOW_USER_MSG(("Play FM radio channel by tuner driver...\n"));  
                        break;
                    }
                    else
                    {
                        if(ucFMdir)
                        {
                            i4CurFreq = i4CurFreq - 50;
                            if(i4CurFreq < 88000)
                            {
                                mcSHOW_USER_MSG(("Can not found FM radio!!\n"));
                        	break;
                            }
                        }
                        else
                        {
                            i4CurFreq = i4CurFreq + 50;
                            if(i4CurFreq > 108000)
                            {
                                mcSHOW_USER_MSG(("Can not found FM radio!!\n"));
                        	break;
                            }
                        }
                    }
                }     
                //AnalogPIEU_TunerSetFreq(cATUNER_PI_CTX(psAtdPdCtx), i4CurFreq, ATV_CONN_MODE_CHAN_CHANGE);
                //mcSHOW_USER_MSG(("Set Frequency = %d KHz, FM status = %d\n", i4CurFreq, fgFMstate));                                            
            break;
#endif            
            case 'n':
                if (i4Argc > 1)
                {
                    fgNewScanStep = StrToInt(aszArgv[1]);
                }
                mcSHOW_USER_MSG(("fgNewScanStep = %d\n", fgNewScanStep));
                break;                

            case 'l':
                i4CurFreq = psAtdPdCtx->Frequency;
                
                if(i4Argc==1)                
                {                    
                    ITuner_OP(psAtdeuDemodCtx->pTCtx, itGetLNA, 0, &ucData1);                    
                    mcSHOW_USER_MSG(("LNA status = %d\n", ucData1));                
                }                
                else if (i4Argc > 1)
                {
                    //ITuner_OP(psAtdeuDemodCtx->pTCtx, itSetLNA, StrToInt(aszArgv[1]), &ucData1);
                    //ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
                    AtdEU_SetLNA(StrToInt(aszArgv[1]));
                    mcSHOW_USER_MSG(("Set LNA = %d\n", StrToInt(aszArgv[1])));
                }
                break;                

//#ifdef ANAEU_ENABLE_MONITOR
            case 'm':
                {
                    UINT8   ucCmdExt;

                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'p')
                    {
						SetBreakMonitor(psAtdPdCtx);
                    }
                    else if (ucCmdExt == 'r')
                    {
						ResumeMonitor(psAtdPdCtx);
                    }
                }
                break;
//#endif

            case 'x':
            {   
	            S32 Record[10] = {0,0,0,0,0,0,0,0,0,0};
                //U8  u1Data1, u1Data2;
                U16  u2Data1, u2Data2;
                UINT32 RegTemp = 0;
	    	  UINT32 dRegTemp = 0;	
                
                // reset TVD
                /*RegTemp = IO_READ32(0x20022000, 0x400);
                RegTemp |= 0x00000004; //Bit 2
	    	    IO_WRITE32(0x20022000, 0x400, RegTemp);
	    	    RegTemp &= 0xfffffffb; //Bit 2
	    	    IO_WRITE32(0x20022000, 0x400, RegTemp);*/
	    	       
	    	    //Get ATD TVD Indicator
                for (ii=0; ii<3; ii++)
                {
                    //PRA
#if (defined (CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
                    ATD_GetReg(psAtdeuDemodCtx, REG_ATD_SLD_PRA_34, &ucData1, 1);
                   mcSHOW_USER_MSG(("REG_ATD_SLD_PRA_34(0x2e4) = 0x%02X\n", ucData1));
#else
                    ATD_GetReg(psAtdeuDemodCtx, REG_ATD_PRA_ST, &ucData1, 1);
                    mcSHOW_USER_MSG(("REG_ATD_PRA_ST(0x15E) = 0x%02X\n", ucData1));
#endif
                    Record[0] += (ucData1 & 0x40) >> 6;
                    
                    // CR
                    s2Data = ATD_GetCFOKHz(psAtdeuDemodCtx, FALSE);
                    mcSHOW_USER_MSG(("CFO = %d KHz\n", s2Data));
                    if (s2Data < 0)
                    	s2Data = -s2Data;
                    Record[1] += s2Data;               	                    
                    ATD_GetReg(psAtdeuDemodCtx, REG_ATD_CR_STATE0, &ucData1, 1);
                    mcSHOW_USER_MSG(("REG_ATD_CR_STATE0(0x1a0) = 0x%02X\n", ucData1));
                    Record[2] += ucData1 & 0x3;
                    ATD_GetReg(psAtdeuDemodCtx, REG_ATD_CR_STATE1, &ucData1, 1);
                    mcSHOW_USER_MSG(("REG_ATD_CR_STATE1(0x1a1) = 0x%02X\n", ucData1));
                    Record[3] += (ucData1 & 0x20) >> 5;
                           
                    //TVD
                    ucData1 = IO_READ8(VIDEO_IN0_BASE, 0x80);
                    mcSHOW_USER_MSG(("TVD noise level = %d\n", ucData1));           	
                    Record[4] += ucData1;
                    ucData1 = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4;
                    mcSHOW_USER_MSG(("Vpress = %d\n", ucData1)); 		
                    Record[5] += ucData1;
                    u2Data = IO_READ32(VIDEO_IN0_BASE, 0x44);
                    u2Data1= (u2Data& 0x3FF0000) >> 16; //Sync Level
                    u2Data2 = u2Data & 0x000003FF; //Blank Level
                    mcSHOW_USER_MSG(("Sync (0x%3x) and Blank (0x%3x)\n",u2Data1,u2Data2));          	
                    Record[6] += u2Data1;
                    Record[7] += u2Data2;
	                
                    {
	    	        
	    	        //UINT16 Values[6];
	    	        //1 1.Enable Catch Sel
	    	        RegTemp = IO_READ32(VIDEO_IN0_BASE, 0x5EC);
	    	        RegTemp |= 0x00008000; //Bit 15
	    	        IO_WRITE32(VIDEO_IN0_BASE, 0x5EC, RegTemp);
                    
	    	        //1 2.Select Probe Signal
	    	        RegTemp = IO_READ32(VIDEO_IN0_BASE, 0x61C);
	    	        RegTemp &= 0x0FFFFFFF; //Bit 31 ~28
	    	        RegTemp |= 0xB0000000; //Bit 31 ~28
	    	        IO_WRITE32(VIDEO_IN0_BASE, 0x61C, RegTemp);
	    	        
	    	        IO_WRITE32(VIDEO_IN0_BASE, 0x0A8,0x0);   // clear indicator, Pluto	    	        	    	        
	    	        mcDELAY_MS(600);
	                
	    	        //1 3. Get Indicator
	    	        dRegTemp = IO_READ32(VIDEO_IN0_BASE, 0x0A8);
	    	        mcSHOW_USER_MSG(("V Sync Loss sum during 64 fields %d\n", dRegTemp & 0x3F));
                                           Record[8] += dRegTemp & 0x3F;
	    	        mcSHOW_USER_MSG(("H Sync Loss sum during 64 fields %d\n", (dRegTemp >> 6) & 0x3FFF));
                                           Record[9] += ((dRegTemp >> 6) & 0x3FFF);
	                }
 	                 //mcDELAY_MS(500);
                } //End for(ii=0; ii<5; ii++)
        
                //Average ATD TVD Indicator                
                for(ii=0;ii<10;ii++)
                {
	              Record[ii] /= 3;
	              mcSHOW_USER_MSG(("%d ", Record[ii]));			
                }
                mcSHOW_USER_MSG(("\n"));			
        
                //Check ATD TVD Indicator
                u2State = 0;
                if (Record[0] == 0x01)  // PrA should be in the tracking state
                    u2State += mcBIT(0);
                if (Record[1] < 50) // CFO < 50KHz.
                    u2State += mcBIT(1); 
                if (Record[2] == 0x03) // CR should be int the tracking state 
                	u2State += mcBIT(2); 
                if (Record[3] == 0x01) // Pilot good stability
                	u2State += mcBIT(3); 
                if (Record[4] < 0x15) // TvD noise level => Need to tune.
                	u2State += mcBIT(4); 
                if (Record[5] == 1) // TvD VPRES
                	u2State += mcBIT(5);
                if ((Record[6] > 33) || (Record[6] < 36)) // TVD sync level
                	u2State += mcBIT(6); 
                if ((Record[7] > 250) || (Record[7] < 262)) // Blank Level ~ 0x100.
                	u2State += mcBIT(7); 
                if (Record[8] == 0) // V Sync Loss sum during 64 fields?
                	u2State += mcBIT(8); 
                if ((psAtdPdCtx->u1SubSysIdx==MOD_NTSC_M) && (Record[9] <= 576)) // H Sync Loss sum during 64 fields, NTSC
                	u2State += mcBIT(9);
                else if ((psAtdPdCtx->u1SubSysIdx==MOD_SECAM_L) && (Record[9] <= 480)) // H Sync Loss sum during 64 fields, SECAM
                	u2State += mcBIT(9);
                
                if(u2State == 0x3FF)
                //if ((u2State&0x00ff) == 0x00ff)
                {    
                	mcSHOW_USER_MSG(("VAGood\n"));
                }
                else
                {                	
                      mcSHOW_USER_MSG(("ATD SLT video fail(0x%03X)\n", u2State));
        	          mcSHOW_USER_MSG(("ATDFAIL\n"));
                }                                
            }                
                break;
#ifdef EU_AUTO_ATP
            case 'z':
    
                if (i4Argc > 1)
                {
                    gATP_TEST_ITEM_BIT = StrToInt(aszArgv[1]);                           
		     if (gATP_TEST_ITEM_BIT & 0x10)//check ATPFAIL BITS
		     	{
                        gATPFailBitMsk = 0;  //reset gATPFailBitMsk
		     	}		
                }  
		break;		
#endif				
                
            default:
                mcSHOW_USER_MSG(("Invalid parameter!\n"));
                break;
        }
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
}

/************************************************************************
*  AtdEU_TunerGetAttribute
*  Get analog tuner driver attribute.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_get_type : Attribute type.
*  @param  *pvAnaAttribute : Output data content pointer.
*  @param  *pzAnaAttributeLen : Output data length pointer.
*  @retval  RMR_OK : Get analog tuner driver attribute Successfully.
*  @retval  RMR_DRV_INV_GET_INFO : Get analog tuner driver attribute fail.
************************************************************************/
INT16 AtdEU_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_GET_TYPE_T  e_get_type,
                           VOID *pvAnaAttribute,
                           SIZE_T *pzAnaAttributeLen)
{

ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;


UINT32  u4TvSubSysMask;
UINT16 u2Value;

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    switch (e_get_type)
    {
        case TUNER_GET_TYPE_RFAGC:
       {
            u2Value = ATD_GetRFAGC(psAtdeuDemodCtx);
            *(UINT16*)pvAnaAttribute = u2Value;
            *pzAnaAttributeLen = sizeof(UINT16);
            break;
        }

        case TUNER_GET_TYPE_TV_SYS_MASK:
        {
            u4TvSubSysMask = psAtdPdCtx->u4TvSubSysMask;
            mcSHOW_DBG_MSG(("\n%s: u1SubSysIdx = %d, u4TvSubSysMask = %04X\n", __FUNCTION__,
                            psAtdPdCtx->u1SubSysIdx - MOD_ANA_TYPE_BEGIN, u4TvSubSysMask));
            *((UINT32 *) pvAnaAttribute) = u4TvSubSysMask;
            break;
        }

        case TUNER_GET_TYPE_ANA_ATTRIBUTE:
        {
        TUNER_ANA_ATTRIBUTE_T   *psTunerAnaAttribute;
   
            psTunerAnaAttribute = (TUNER_ANA_ATTRIBUTE_T *) pvAnaAttribute;
            if(psAtdeuDemodCtx->u1TunerType==cCUST_TUNER)
			{
			 psTunerAnaAttribute->ui4_lower_bound_freq=psAtdeuDemodCtx->pTCtx->specific_member.eu_ctx.m_s4AnaFreqBoundLower;
			 psTunerAnaAttribute->ui4_upper_bound_freq=psAtdeuDemodCtx->pTCtx->specific_member.eu_ctx.m_s4AnaFreqBoundUpper;
			}
			else
			{
			ITuner_OP(pTunerCtx, itGetAnaFreqBound, 0, psAtdPdCtx->psTunerAnaAttr); // update freq bound freom tuner
            *psTunerAnaAttribute = *((TUNER_ANA_ATTRIBUTE_T *) (psAtdPdCtx->psTunerAnaAttr));
			}
           break;
        }

        case TUNER_GET_TYPE_TV_SYS_GROUP:
        {
        UINT8   u1TvSysGroupIdx;
        TUNER_TV_SYS_GROUP_INFO_T   *psTvSysGroup;

            psTvSysGroup = (TUNER_TV_SYS_GROUP_INFO_T *) pvAnaAttribute;
            u1TvSysGroupIdx = psTvSysGroup->ui1_tv_sys_group_idx;
            psTvSysGroup->ui4_tv_sys_mask = TV_SYS_MASK_NONE;
            if (u1TvSysGroupIdx < psAtdPdCtx->ucTvSysGroupNum)
            {
                psTvSysGroup->ui4_tv_sys_mask =
                        psAtdPdCtx->pu4TvSysGroup[u1TvSysGroupIdx];
            }
            break;
        }

        case TUNER_GET_TYPE_NEXT_STEP_SIZE:
        {
            TUNER_STEP_SIZE_T *psTunerStepSize;
            //BOOL   fgTvdLock;
            UINT8  u1TvdState;
            
            psTunerStepSize = (TUNER_STEP_SIZE_T *)pvAnaAttribute;
            //fgTvdLock = fgDrvTvdCheckTVDLock (CHN_LOCK_CHECK_TIME);//AtdPIEU_fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME, psAtdPdCtx->u1SubSysIdx);
            u1TvdState = bTvdNAState4Step();
            
            if (fgTvdLock) {
                // bTvdNAState4Step(): Get TVD noise level
                // 0: reserved
                // 1: Low noise
                // 2: High noise
                if (u1TvdState > 1) { // if TVD noise is high
                    psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_NOISE_HIGH;
                }
                else {
#if 0                    
                    switch (psAtdPdCtx->u1SubSysIdx)
                    {
                    case MOD_PAL_BG: 
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_PAL_BG;            
                        break;
                    case MOD_PAL_DK: 
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_PAL_DK;            
                        break;
                    case MOD_PAL_I: 
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_PAL_I;            
                        break;
                    case MOD_SECAM_L: 
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_SECAM_L;            
                        break;
                    case MOD_SECAM_L1: 
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_SECAM_L1;            
                        break;
                    case MOD_NTSC_M: 
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_NTSC_M;            
                        break;
                    default: // be conservative in unexpected case
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_NOISE_HIGH;            
                        break;
                    }
#else
                    if (psAtdPdCtx->u1SubSysIdx == MOD_PAL_BG)
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_PAL_BG;            
                    else if (psAtdPdCtx->u1SubSysIdx == MOD_PAL_DK)
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_PAL_DK;            
                    else if (psAtdPdCtx->u1SubSysIdx == MOD_PAL_I)
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_PAL_I;            
                    else if (psAtdPdCtx->u1SubSysIdx == MOD_SECAM_L)
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_SECAM_L;            
                    else if (psAtdPdCtx->u1SubSysIdx == MOD_SECAM_L1)
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_SECAM_L1;            
                    else if (psAtdPdCtx->u1SubSysIdx == MOD_NTSC_M)
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_NTSC_M; 
                    else
                        psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_NOISE_HIGH;  
#endif                    
                }
            }
            else {
                psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_UNLOCK;
                g_bypass_pilotsearch=0x01;
                mcSHOW_DBG_MSG(("VIF Full lock,TVD Unlock,next step size is 1.5M.try to bypass pilot searcher\n"));
            }
            
            mcSHOW_DBG_MSG(("TVD noise = %d, TV spec = %d\n", u1TvdState, psAtdPdCtx->u1SubSysIdx - MOD_ANA_TYPE_BEGIN));
            mcSHOW_DBG_MSG(("Next step = %dHz\n", psTunerStepSize->i4_step_size));
            
            if (!fgNewScanStep){
                if (fgTvdLock){
                    psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_LOCK_OLD;
                }
                else {
                    psTunerStepSize->i4_step_size = cNEXT_STEP_TVD_UNLOCK_OLD;
                }
            }
            break;     
        }

        default:
        {
            mcSHOW_DBG_MSG4(("%s: RMR_DRV_INV_GET_INFO!\n", __FUNCTION__));

            mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
            return (RMR_DRV_INV_GET_INFO);
        //    break;
        }
    }

    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
    return (RMR_OK);

}

/************************************************************************
*  AtdEU_TunerSetAttribute
*  Set analog tuner driver attribute.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_set_type : Attribute type.
*  @param  *pvAnaAttribute : Input data content pointer.
*  @param  zAnaAttributeLen : Input data length.
*  @retval  RMR_OK : Set analog tuner driver attribute Successfully.
*  @retval  RMR_DRV_INV_GET_INFO : Set analog tuner driver attribute fail.
************************************************************************/
INT32 AtdEU_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
#if fcTUNE_NOT_START
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
#endif

//    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    switch (e_set_type)
    {


#if fcTUNE_NOT_START
        case TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS:
            if (zAnaAttributeLen)
            {
            TUNER_ACQ_ACTION_T *peActionType = (TUNER_ACQ_ACTION_T *) pvAnaAttribute;

                mcSHOW_DBG_MSG(("%s: (%d, %X) eActionType=%d!\n", __FUNCTION__,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS, *peActionType));
                psAtdPdCtx->u1TunerAcqType = *peActionType;
            }
            break;
#endif

        case TUNER_SET_TYPE_TUNER_LOCAL_DEFINE_RESUME_PI_MONITOR:
//#ifdef ANAEU_ENABLE_MONITOR
            if(psAtdPdCtx->fgBreakMonitor){
                ResumeMonitor(psAtdPdCtx);
            }
//#endif
        break;
     case TUNER_SET_TYPE_FREQ_AUTO_NFY:
     {
          ANALOG_BRDCST_CTRL_FREQ_AUTO_NFY_T* pAutoNFY = (ANALOG_BRDCST_CTRL_FREQ_AUTO_NFY_T *)pvAnaAttribute;
          if (pAutoNFY->b_enable_freq_auto_nfy)
          {
                ATDEU_SetFreqAutoNFY(TRUE);
                mcSHOW_DBG_MSG(("TUNER_SET_TYPE_FREQ_AUTO_NFY = TRUE"));        
          }
          else
          {
               ATDEU_SetFreqAutoNFY(FALSE);
                mcSHOW_DBG_MSG(("TUNER_SET_TYPE_FREQ_AUTO_NFY = FALSE"));           
          }
          break;
     }
        default:
        {
            mcSHOW_DBG_MSG4(("%s: RMR_DRV_INV_GET_INFO!\n", __FUNCTION__));
//            mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
            return (RMR_DRV_INV_GET_INFO);
        //    break;
        }
    }
//    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);

#if fcTUNE_NOT_START
    return (RMR_OK);
#endif
}

/***********************************************************************/
#ifdef fcADD_DIAG_INFO

#define DRV_CUSTOM_TUNER_DIAG_ITEMS         3

/************************************************************************
*  AtdEU_TunerSetDiagCmd
*  Set analog tuner diagnostic information.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_tuner_set_type : Diagnostic information type.
*  @param  *pv_set_info : Input data content pointer.
*  @param  z_size : Input data length.
*  @retval  RMR_OK : Set analog tuner diagnostic information successfully.
*  @retval  RMR_DRV_SET_FAILED : Set analog tuner diagnostic information fail.
*  @retval  RMR_DRV_INV_SET_INFO : Invalid analog tuner diagnostic information.
************************************************************************/
INT32 AtdEU_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
	INT32	retSts = RMR_OK;
//#ifdef TUNER_DVBT_ENABLE_AGC_CTRL /*fix build warning*/
    ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;

  
    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry
//#endif /*TUNER_DVBT_ENABLE_AGC_CTRL*/

    switch(e_tuner_set_type)
    {
        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN:
            // Do initialization here
            break;

        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE:
            // Do cleanup here
            break;
#if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
		case DRV_CUSTOM_TUNER_SET_TYPE_DESCRAMBLE:
             {   
				 BOOL fgScramble ;
				 	fgScramble = *((UINT8*)pv_set_info);
				if(fgScramble)
		         {DeScrambleBox = TRUE;
		          DeScrambleBoxDelay = 1000;}
                else
		         {DeScrambleBox = FALSE;
		          DeScrambleBoxDelay = 0;}
             
	              ATD_SetDelayLine(psAtdeuDemodCtx,fgScramble); 
			} 
            break;
#endif
#if 0
    case DRV_CUSTOM_TUNER_SET_TYPE_AGC:
        {

            GEN_TUNER_CTX_T *pDigiTunerCtx;
            pDigiTunerCtx = psAtdPdCtx->sAtdPiCtx.pDigiTunerCtx;
            pDigiTunerCtx->m_Ana_Top = ((TUNER_TYPE_DIAG_AGC_T *) pv_set_info)->ucAgcValue;
            if ( !AnalogPIEU_TunerSetSystem(cATUNER_PI_CTX(psAtdPdCtx), AnalogPIEU_TunerGetSystem(cATUNER_PI_CTX(psAtdPdCtx))) ){

                //PDWNC_WriteErrorCode(ERR_I2C_DEMOD_ANALOG);
                mcSHOW_USER_MSG(("TunerSetDiagCmd: set ATV TOP fail!!\n"));
            }
	        mcSHOW_DBG_MSG(("TunerSetDiagCmd: set ATV TOP = 0x%02X\n", pDigiTunerCtx->m_Ana_Top));
        }
	        break;
#endif /*TUNER_DVBT_ENABLE_AGC_CTRL*/
        default:
            retSts = RMR_DRV_INV_SET_INFO;
            break;
    }

//#ifdef TUNER_DVBT_ENABLE_AGC_CTRL
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
//#endif /*TUNER_DVBT_ENABLE_AGC_CTRL*/
    return retSts;
}



/************************************************************************
*  AtdEU_TunerGetDiagInfo
*  Get analog tuner diagnostic information.
*  @param  ptTDSpecificCtx : A pointer use to point to struct PTD_SPECIFIC_CTX.
*  @param  e_tuner_get_type : Diagnostic information type.
*  @param  *pv_get_info : Output data content pointer.
*  @param  *pz_size : Output data length.
*  @retval  RMR_OK : Get analog tuner diagnostic information successfully.
*  @retval  RMR_DRV_NO_GET_INFO : Get analog tuner diagnostic information fail.
*  @retval  RMR_DRV_NOT_ENOUGH_SPACE : Get analog tuner diagnostic information fail.
************************************************************************/
INT32 AtdEU_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                              DRV_CUSTOM_TUNER_GET_TYPE_T   e_tuner_get_type,
                              VOID                           *pv_get_info,
                              SIZE_T                        *pz_size)
{
ATD_PD_CTX_T *psAtdPdCtx = (ATD_PD_CTX_T *) ptTDSpecificCtx;
TUNER_TYPE_DIAG_CONTENT_T *pDiagContent = (TUNER_TYPE_DIAG_CONTENT_T *) pv_get_info;

    mcSEMA_LOCK_HAL(psAtdPdCtx->hHalLock);  // Avoid re-entry

    switch (e_tuner_get_type)
    {
        #if 0
        case DRV_CUSTOM_TUNER_GET_TYPE_AGC:
        {
            GEN_TUNER_CTX_T *pDigiTunerCtx;
            pDigiTunerCtx = psAtdPdCtx->sAtvPiCtx.pDigiTunerCtx;
            if ((*pz_size) < sizeof(TUNER_TYPE_DIAG_AGC_T)){
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            ((TUNER_TYPE_DIAG_AGC_T *) pv_get_info)->ucAgcValue = pDigiTunerCtx->m_Ana_Top;
            mcSHOW_DBG_MSG(("TunerGetDiagInfo: Get ATV TOP = 0x%02X\n", pDigiTunerCtx->m_Ana_Top));
        }
            break;
        #endif
        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL:
            if ((*pz_size) < sizeof(TUNER_TYPE_DIAG_CONTROL_T))
            {
                *pz_size = sizeof(TUNER_TYPE_DIAG_CONTROL_T);
                mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->u2PollPeriodMs = 2000;
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->ucCntOfDiagStruc = DRV_CUSTOM_TUNER_DIAG_ITEMS;
            break;

        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT:
            if ((*pz_size) < DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T))
            {
                *pz_size = DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T);
                mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

        // To add new items, please remember to modify DRV_CUSTOM_TUNER_DIAG_ITEMS
        // Note that both cDisplayName and cDisplayValue can NOT exceed 15 bytes

        // Record 1
            snprintf(pDiagContent->cDisplayName,15, "%s", "Version:");
             snprintf(pDiagContent->cDisplayValue, 15, "%s", "MTK_ATD");
            pDiagContent++;

        // Record 2
            snprintf(pDiagContent->cDisplayName,15, "%s", "RF Freq (KHz):");
             snprintf(pDiagContent->cDisplayValue, 15, "%6d", psAtdPdCtx->Frequency);
            pDiagContent++;

        // Record 3
            snprintf(pDiagContent->cDisplayName,15, "%s", "Lock State:");
             snprintf(pDiagContent->cDisplayValue, 15, "%d", psAtdPdCtx->u1VIFLock);
            pDiagContent++;

            break;                          // End of case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT

        default:
            mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
            return RMR_DRV_NO_GET_INFO;
    }
    mcSEMA_UNLOCK_HAL(psAtdPdCtx->hHalLock);
    return RMR_OK;
}
#endif
static void AtdEU_NonImplement_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    mcSHOW_USER_MSG(("AtdEU_NonImplement_TunerDtdCommand\r\n"));
}

static void AtdEU_NonImplement_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
        mcSHOW_DBG_MSG2(("AtdEU_NonImplement_TunerDetachI2C\r\n"));
}
/***********************************************************************/
/*                  Acquisition APIs                                   */
/***********************************************************************/
/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************
*  Tuner_AtdEU_Register_LTDIS_Fct
*  Analog ltdis api mapping.
*  @param  ptTDCtx : A void type pointer use to point to struct TUNER_DEVICE_CTX_T.
*  @retval  void.
***********************************************************************/
VOID Tuner_AtdEU_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
  TUNER_DEVICE_CTX_T*     pTunerDeviceCtx = ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = AtdEU_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = AtdEU_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = AtdEU_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = AtdEU_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = AtdEU_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = AtdEU_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = AtdEU_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = AtdEU_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = AtdEU_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = AtdEU_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = AtdEU_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = AtdEU_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand  = AtdEU_NonImplement_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAtdCommand = AtdEU_TunerAtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = AtdEU_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = AtdEU_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = AtdEU_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerRegRead = AtdEU_TunerRegRead;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = AtdEU_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C   = AtdEU_NonImplement_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = AtdEU_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = AtdEU_TunerGetAttribute;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = AtdEU_TunerGetDiagInfo;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd  = AtdEU_TunerSetDiagCmd;
#endif
}

