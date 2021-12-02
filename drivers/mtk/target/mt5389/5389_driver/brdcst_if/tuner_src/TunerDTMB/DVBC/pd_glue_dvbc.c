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
/************************************************************************* 
* 
* Filename: 
* --------- 
* 
* Description: 
* ------------ 
*    
* Author: 
* ------- 
*    chi zhang
* 
*************************************************************************/

#include "pi_dvbc.h"
#include "PD_Def_dbg.h"  // Note that fcADD_DIAG_INFO defined in PD_Def_dbg.h, This file should be placed in the first position.
#include "pi_def_dvbc.h"
#include "TDIS.h"
#include "pi_demod.h"
#include "pd_tuner_type.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "mt5175c_com_defs.h"

#include "fe_tuner_common_if.h"
#include "eeprom_if.h"
#include "pd_dvbc.h"
#include "x_stl_lib.h"

#include "pd_tuner_type.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "dmx_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
// if you want to enable this patch, remember to add MW and RM codes
// Please refer to BR: DTV_X_IDTV0801_002085_23_001_41_001
// driver changelist: 732538 (JC Wu)
// MW svcctx changelist: 737671 (Weihua Chen)
// RM changelist: 737666 (Ziaoguang Zhang / Jun Zhang)
//#define fcQINGDAO_NO_AUD_VID_DSCRM_PATCH

#define DVBC_MON_POLL_INTRVL             10  // 10ms
#define DVBC_LOCK_TIME_OUT              1200 // 1200ms
#define DVBC_LOCK_TIME_OUT_CNT         (DVBC_LOCK_TIME_OUT/DVBC_MON_POLL_INTRVL)

#define constellation_qam16                        0x00
#define constellation_qam32                        0x01
#define constellation_qam64                        0x02
#define constellation_qam128                       0x03
#define constellation_qam256                       0x04
#define constellation_AutoQam                      0x05

#define fcMONITOR_DEMUX_CONF_COUNT_THRESHOLD 3
#define fcMONITOR_AUDVID_CONF_COUNT_THRESHOLD 3

#if  fcTUNER_TYPE == cALPS_TDAG4B03A
#define DVBC_TUNER_CONN_TIME_THREHOLD   750                          //add 700ms , TDAG4B03A AGC speed too slow
#else
#define DVBC_TUNER_CONN_TIME_THREHOLD   140
#endif
#define DVBC_DEMOD_CONN_TIME_THREHOLD   250

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcSEMA_LOCK_MTFE(sema)                                       \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Lock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__)); \
    x_sema_lock(sema, X_SEMA_OPTION_WAIT);                           \
    mcSHOW_DBG_MSG4((" [ _Excape_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
}
#define mcSEMA_UNLOCK_MTFE(sema)                                     \
{                                                                    \
    mcSHOW_DBG_MSG4((" [ _Unlock_%s-%s-%d ]\r\n", __FUNCTION__, __LINE__));     \
    x_sema_unlock(sema);                                             \
}


DEMOD_CTX_T*    psDvbcDemodCtx;

DVBC_PD_CTX_T *pDVBCPdCtx;

static x_break_fct      MTDVBCisBreak;

// for Qtest
static U16  u2ToolxInfoUpdateRate = 1000;
//static U8   ucToolxInfoUpdateLvl  = 1;
//static U8   fgToolxInfoUpdate = FALSE;
static U8   fgPec = FALSE;
static U8   fgPec_dtd_y = FALSE;
static U16  u2AccEsr = 0;
static U32  u4AccTotPkt = 0;
static U32  u4AccErrPkt = 0;
static U32  u4AccCorBit = 0;
static U32  u4AccPer = 0;

static U32  u4AccPostViterbiBER = 0;
static U32  u4AccPostViterbiBER_Cnt = 0;
static U16  u2KeepRefreshTime = 0;

//#if (fcTUNER_TYPE == cDVBT_TUNER_ALL)
static UINT8 ucTunerType;
//#endif

// t_escape_mon_suspend move to pdCtx
//static MUTEX_T  t_escape_mon_suspend;
static BOOL     fgMonitorThreadRunning;
static THREAD_T t_monitor_thread;
//static BOOL       fgIsDisconnected = TRUE;
static BOOL     fgBreakMonitor = TRUE;
static BOOL     fg1stMonitorRun = FALSE;
#ifdef EU_AUTO_ATP
 UINT8 gATP_TEST_ITEM_BIT = 0 ;
 UINT8 gATPFailBitMsk =0;
#endif
static TUNER_DEVICE_CTX_T*      pDvbcTunerDeviceCtx = NULL;

#ifdef fcQINGDAO_NO_AUD_VID_DSCRM_PATCH
static BOOL fgNoAudVid = FALSE;
#endif

static void PD_DVBC_Register_LTDIS_Fct(PTD_CTX ptTDCtx);

//extern TUNER_COMMONAPI_FCT_TBL_T *pTuner_CommonAPI_Fct;
/*
#if (fcTUNER_TYPE != cDVBT_TUNER_ALL)
extern void Tuner_CommonAPI_Register_Fct(TUNER_COMMONAPI_FCT_TBL_T *pcom_Fct);
#else
extern void Tuner_CommonAPI_Register_Fct(TUNER_COMMONAPI_FCT_TBL_T *pcom_Fct, U8 tunerType);
#endif
*/
#define  DVBC_SYNC_PER_THRESHOLD  99000

/***********************************************************************/
#ifdef fcADD_DIAG_INFO
INT32 DVBC_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
    INT32   retSts = RMR_OK;
    
    mcSHOW_DBG_MSG4(("DVBC_TunerDisc pDVBCPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerDisc pDVBCPdCtx->hHalLock\n"));

    switch(e_tuner_set_type)
    {
        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_OPEN:
            // Do initialization here
            break;

        case DRV_CUSTOM_TUNER_SET_TYPE_DIAG_CLOSE:
            // Do cleanup here
            break;

        default:
            retSts = RMR_INV_SET;
            break;
    }

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerDisc unlock pDVBCPdCtx->hHalLock\n"));
    return retSts;
}

#define DRV_CUSTOM_TUNER_DIAG_ITEMS         16

INT32 DVBC_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                        VOID*                           pv_get_info,
                        SIZE_T*                         pz_size)
{
/*  TUNER_TYPE_DIAG_CONTENT_T *pDiagContent = (TUNER_TYPE_DIAG_CONTENT_T*) pv_get_info;
    INT32   retSts = RMR_OK;
    
    CHAR szFft[8], szQam[8], szCr[8], szGi[8], szHier[8], szAlpha[8], szTmp[8];   
    U8   u1Data[8];  
    //U8   aucData[9];
    U16 u2InrThOnCnt, u2InrTimeOutCnt;
    //S16 s2WndDrift;


    mcSHOW_DBG_MSG4(("DVBT_TunerDisc pDVBCPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerDisc pDVBCPdCtx->hHalLock\n"));

    switch (e_tuner_get_type)
    {
        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTROL:
            if ((*pz_size) < sizeof(TUNER_TYPE_DIAG_CONTROL_T))
            {
                *pz_size = sizeof(TUNER_TYPE_DIAG_CONTROL_T);
                retSts = RMR_DRV_NOT_ENOUGH_SPACE;
                break;
            }
            
            // ToDo - Modify the Polling period for your needed.
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->u2PollPeriodMs = 1000;
            
            // ToDo - Modify the count of DRV_CUSTOM_TUNER_DIAG_ITEMS that to be displayed
            ((TUNER_TYPE_DIAG_CONTROL_T *) pv_get_info)->ucCntOfDiagStruc = DRV_CUSTOM_TUNER_DIAG_ITEMS;
            break;

        case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT:
            if ((*pz_size) < DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T))
            {
                *pz_size = DRV_CUSTOM_TUNER_DIAG_ITEMS * sizeof(TUNER_TYPE_DIAG_CONTENT_T);
                retSts = RMR_DRV_NOT_ENOUGH_SPACE;
                break;
            }

            // To add new items, please remember to modify DRV_CUSTOM_TUNER_DIAG_ITEMS
            // Note that both cDisplayName and cDisplayValue can NOT exceed 15 bytes

            // Justin, move to update update Demod context to thread
            //DVBT_GetInfo(psDvbcDemodCtx);
            //vDVBTGetStatus(psDvbcDemodCtx);
            
            // driver version
            sprintf(pDiagContent->cDisplayName, "%s", "Version");
            sprintf(pDiagContent->cDisplayValue, "%s", DVBT_GetSwVer());
            pDiagContent++;

            // BW
            sprintf(pDiagContent->cDisplayName, "%s", "BW");
            switch (DVBT_GetBW(psDvbcDemodCtx))
            {
                case 0:  sprintf(szTmp, "6M"); break;
                case 1:  sprintf(szTmp, "7M"); break;
                case 2:  sprintf(szTmp, "8M"); break;
                case 3:  sprintf(szTmp, "5M"); break;
                default: sprintf(szTmp, "??"); break;
            }
            sprintf(pDiagContent->cDisplayValue, "%s", szTmp);
            pDiagContent++;

            // DVB-T mode 
            sprintf(pDiagContent->cDisplayName, "%s", "DVB-T mode");
            switch (DVBT_GetFftMode(psDvbcDemodCtx))
            {
                case 0:  sprintf(szFft, "2k"); break;
                case 1:  sprintf(szFft, "8k"); break;
                case 2:  sprintf(szFft, "4k"); break;
                default: sprintf(szFft, "??"); break;
            }
            switch (DVBT_GetConst(psDvbcDemodCtx))
            {
                case 0:  sprintf(szQam, "QPSK"); break;
                case 1:  sprintf(szQam, "16Q" ); break;
                case 2:  sprintf(szQam, "64Q" ); break;
                default: sprintf(szQam, "??"  ); break;
            }            
            switch (DVBT_GetCR(psDvbcDemodCtx))
            {
                case 0:  sprintf(szCr, "1/2"); break;
                case 1:  sprintf(szCr, "2/3"); break;
                case 2:  sprintf(szCr, "3/4"); break;
                case 3:  sprintf(szCr, "5/6"); break;
                case 4:  sprintf(szCr, "7/8"); break;
                default: sprintf(szCr, "??" ); break;
            }
            switch (DVBT_GetGI(psDvbcDemodCtx))
            {
                case 0:  sprintf(szGi, "1/32"); break;
                case 1:  sprintf(szGi, "1/16"); break;
                case 2:  sprintf(szGi, "1/8" ); break;
                case 3:  sprintf(szGi, "1/4" ); break;
                default: sprintf(szGi, "??"  ); break;
            }            
            sprintf(pDiagContent->cDisplayValue, "%s,%s,%s,%s", szFft, szQam, szCr, szGi);
            pDiagContent++;

            // SNR 
            sprintf(pDiagContent->cDisplayName, "%s", "SNR");
            sprintf(pDiagContent->cDisplayValue, "%d dB", (DVBT_GetSignalSNR(psDvbcDemodCtx)>>3) );
            pDiagContent++;
            
            // PostVBER
            sprintf(pDiagContent->cDisplayName, "%s", "BER");
            sprintf(pDiagContent->cDisplayValue, "%d x10^-5", DVBT_GetPostVBer(psDvbcDemodCtx) );
            pDiagContent++;

            // TS PER
            sprintf(pDiagContent->cDisplayName, "%s", "PER");
            sprintf(pDiagContent->cDisplayValue, "%d x10^-5", DVBT_GetPER(psDvbcDemodCtx) );
            pDiagContent++;

            // TPS/TS lock status
            sprintf(pDiagContent->cDisplayName, "%s", "TPS / TS");
            sprintf(pDiagContent->cDisplayValue, "%d / %d", DVBT_GetTpsLock(psDvbcDemodCtx), DVBT_GetTsLock(psDvbcDemodCtx) );
            pDiagContent++;

            // Hierarchy
            sprintf(pDiagContent->cDisplayName, "%s", "Hierarchy");
            switch (DVBT_GetHier(psDvbcDemodCtx))
            {
                case 0:  sprintf(szAlpha, "");      break;
                case 1:  sprintf(szAlpha, ", a=1"); break;
                case 2:  sprintf(szAlpha, ", a=2"); break;
                case 3:  sprintf(szAlpha, ", a=4"); break;
                default: sprintf(szAlpha, "??"  ); break;
            }
            if (DVBT_GetHier(psDvbcDemodCtx) == 0)
            {
                sprintf(szHier,  "non-Hier");                        
            }
            else
            {
                if (DVBT_GetHpLp(psDvbcDemodCtx))
                    sprintf(szHier,  "LP");                          
                else
                    sprintf(szHier,  "HP");
            }
            sprintf(pDiagContent->cDisplayValue, "%s%s", szHier, szAlpha);
            pDiagContent++;

            // IF AGC
            sprintf(pDiagContent->cDisplayName, "%s", "IF AGC");
            sprintf(pDiagContent->cDisplayValue, "%d mV", ((DVBT_GetIfAgc(psDvbcDemodCtx)*3300>>8) + 1650) );
            pDiagContent++;
            
            // Freq offset
            sprintf(pDiagContent->cDisplayName, "%s", "Freq Offset");
            sprintf(pDiagContent->cDisplayValue, "%d kHz", DVBT_GetFreqOffset_kHz(psDvbcDemodCtx)>>8);
            pDiagContent++;

            // Time offset
            sprintf(pDiagContent->cDisplayName, "%s", "Time Offset");
            sprintf(pDiagContent->cDisplayValue, "%d ppm", DVBT_GetTimingOffset(psDvbcDemodCtx)/1000);
            pDiagContent++;

            // AR
            sprintf(pDiagContent->cDisplayName, "%s", "AR");
            DVBT_GetReg(psDvbcDemodCtx, 0x176, u1Data, 1);
            if (u1Data[0] == 0)
                sprintf(pDiagContent->cDisplayValue, "Off");
            else
                sprintf(pDiagContent->cDisplayValue, "On");
            pDiagContent++;
            
            //CCI
            sprintf(pDiagContent->cDisplayName, "%s", "CCI Cnt");
            DVBT_GetReg(psDvbcDemodCtx, 0x1EC, u1Data, 1);
            sprintf(pDiagContent->cDisplayValue, "%d", u1Data[0]);
            pDiagContent++;

            //INR: threshold on
            sprintf(pDiagContent->cDisplayName, "%s", "INR(Thrld On)");
            DVBT_GetReg(psDvbcDemodCtx, 0x1ED, u1Data, 3);
            u2InrThOnCnt = mcUNSIGN_2BYTE(u1Data[2], u1Data[0], 4, 8);
            u2InrTimeOutCnt = mcUNSIGN_2BYTE(u1Data[2]>>4, u1Data[1], 4, 8);
            sprintf(pDiagContent->cDisplayValue, "%d", u2InrThOnCnt);
            pDiagContent++;

            //INR: time out            
            sprintf(pDiagContent->cDisplayName, "%s", "INR(Time Out)");
            sprintf(pDiagContent->cDisplayValue, "%d", u2InrTimeOutCnt);
            pDiagContent++;

            // FDBW
            sprintf(pDiagContent->cDisplayName, "%s", "FDBW");
            DVBT_GetReg(psDvbcDemodCtx, 0x151, u1Data, 1);
            switch ( mcGET_FIELD(u1Data[0], 0x60, 5) )
            {
            case 0: u1Data[0] = 32; break;
            case 1: u1Data[0] = 16; break;
            case 2: u1Data[0] = 8 ; break;
            case 3: u1Data[0] = 4 ; break;
            default:u1Data[0] = 0 ; break;
            }
            sprintf(pDiagContent->cDisplayValue, "1/%d", u1Data[0]);
            pDiagContent++;         

            break; /// End of case DRV_CUSTOM_TUNER_GET_TYPE_DIAG_CONTENT
        default:
            retSts = RMR_DRV_NO_GET_INFO;
            break;
    }

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerDisc unlock pDVBCPdCtx->hHalLock\n"));
    
    return retSts;
*/  return 0;
}
#endif

STATIC INT8 Tuner_Connect(U32 Freq, U8 sawbw, U8 AutoSearch)
{
    PARAM_SETFREQ_T SetFreqPara;    
    INT8 fgRet = 0;

    ITuner_OP(ITunerGetCtx(), itSetSawBw, sawbw, NULL);
   
    SetFreqPara.Freq = Freq;
    SetFreqPara.Modulation = MOD_DVBC;
    SetFreqPara.fgAutoSearch = AutoSearch;   
    if((ITuner_SetFreq(ITunerGetCtx(), &SetFreqPara)) != 0) {
        mcSHOW_USER_MSG(("Failed on Tuner_SetFreq(%d)", Freq));
        fgRet = 1;
    }

    return fgRet;
}


static void MonitorProcess(VOID *pvArg)
{
    U32    loop = 0;
    U8     aucData[cMAX_READ_NUM + 1];

    //U32    u4AccPer;
    U8     ucAcqCnt = 0xFF, ucRstCnt = 0xFF, ucArCoef = 0xFF;
    U8    ucASRFailCnt = 0xFF, ucSignalNotDct = 0xFF, ucReAcqCnt = 0xFF;

#ifdef fcQINGDAO_NO_AUD_VID_DSCRM_PATCH
    // monitor dmux and MW (no audio and video) indicators to reset DSCRM
    STATIC UINT16 u2rec_pkt_last = 0;
    STATIC UINT16 u2drop_pkt_last = 0;
    STATIC INT16 s2monitor_demux_conf_count = -1;
    STATIC INT16 s2monitor_audvid_conf_count = 0;
    STATIC BOOL fgNoAudVidReset1= FALSE;
    STATIC BOOL fgNoAudVidReset2 = FALSE;
    STATIC INT16 s2reset_timer1= 0;
    STATIC INT16 s2reset_timer2 =0;
    UINT16 u2rec_pkt_diff = 0, u2drop_pkt_diff = 0;
    
    UINT16 u2temp;
    INT32 s4dmx_err_rate;
    //UINT8 ucData;
    UINT32 u4temp;
#endif

    mcSHOW_DBG_MSG(("MonitorProcess start\n"));
    while (fgMonitorThreadRunning)
    {
        if (fgBreakMonitor){
            // After mcMUTEX_LOCK(pDVBCPdCtx->t_escape_mon_suspend), monitor thread will be suspended until
            // TunerAcq or TunerClose invoked.
            mcSHOW_DBG_MSG(("Entering DVBC mon_suspend\n"));
            mcMUTEX_LOCK(pDVBCPdCtx->t_escape_mon_suspend);
            mcSHOW_DBG_MSG(("Escape DVBC mon_suspend\n"));
        }
        else{
            // mutex lock
            mcSHOW_DBG_MSG4(("MonitorProcess pDVBCPdCtx->hHalLock\n"));
            mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
            mcSHOW_DBG_MSG4(("escape MonitorProcess pDVBCPdCtx->hHalLock\n"));

            mcSHOW_DBG_MSG4(("Invoke DVBC_Monitor\n"));
            // ***** DVB-C Info Update ***** //
            if (!fg1stMonitorRun)
            {
            #ifdef fcQINGDAO_NO_AUD_VID_DSCRM_PATCH
                mcSHOW_USER_MSG(("MonitorProcess: Monitor Demux Packets: 1st Monitor Run & Reset Variables!!\n"));
                s2monitor_demux_conf_count = -1;
                s2monitor_audvid_conf_count = 0;
                //u2rec_pkt_last = 0;
                //u2drop_pkt_last = 0;
                fgNoAudVidReset1 = FALSE;
                fgNoAudVidReset2 = FALSE;
                s2reset_timer1 = 0;
                s2reset_timer2 = 0;
            #endif
                mcDELAY_MS(100);
            }
            DVBC_UpdateInfo(psDvbcDemodCtx);
        DVBC_UpdateStatus(psDvbcDemodCtx);
            fg1stMonitorRun = TRUE;  // Ken, 090202, check Monitor Process actually execute, for best mux

#ifdef fcQINGDAO_NO_AUD_VID_DSCRM_PATCH
            // monitor dmux and MW (no audio and video) indicators to reset DSCRM
            if (s2monitor_demux_conf_count == -1)
            {
                s2monitor_demux_conf_count = 0;
                u2rec_pkt_last = (UINT16)DMX_GetReceivedPacketNumber();
                u2drop_pkt_last = (UINT16)DMX_GetDroppedPacketNumber();
            }
            else
            {
                mcSHOW_DBG_MSG(("MonitorProcess: Monitor Demux packets!!\n"));
                
                u2temp = (UINT16)DMX_GetReceivedPacketNumber();
                if (u2temp >= u2rec_pkt_last)
                {
                    u2rec_pkt_diff = u2temp - u2rec_pkt_last;
                }
                else
                {
                    u2rec_pkt_diff = 65535 - u2rec_pkt_last + u2temp;
                }
                u2rec_pkt_last = u2temp;
                mcSHOW_DBG_MSG(("MonitorProcess: Monitor Demux Receive Pkt = %d\n", u2rec_pkt_diff));

                u2temp = (UINT16)DMX_GetDroppedPacketNumber();
                if (u2temp >= u2drop_pkt_last)
                {
                    u2drop_pkt_diff = u2temp - u2drop_pkt_last;
                }
                else
                {
                    u2drop_pkt_diff = 65535 - u2drop_pkt_last + u2temp;
                }
                u2drop_pkt_last = u2temp;
                mcSHOW_DBG_MSG(("MonitorProcess: Monitor Demux Drop Pkt = %d\n", u2drop_pkt_diff));
            }

            if ((u2drop_pkt_diff==0) && (u2rec_pkt_diff==0))
                s4dmx_err_rate = -1;
            else
                s4dmx_err_rate = (((INT32)u2drop_pkt_diff * 100000) / ((INT32)u2drop_pkt_diff+(INT32)u2rec_pkt_diff ));
#if 1
            // demux drop packet check
            // "drop_pkt/(drop_pkt+rcv_pkt) > 1/10" case
            u4temp = (UINT32)u2drop_pkt_diff*9;
            if ((u2drop_pkt_diff > 0) && (u4temp>u2rec_pkt_diff))
            {
                if (DVBC_GetPER(psDvbcDemodCtx)==0)
                {
                    s2monitor_demux_conf_count++;
                    mcSHOW_USER_MSG(("MonitorProcess: Demux Drop Most Pkt & Demod Is Good. Conf_Count=%d\n", s2monitor_demux_conf_count));
                }
                else if (5*DVBC_GetPER(psDvbcDemodCtx)<s4dmx_err_rate)
                {
                    s2monitor_demux_conf_count++;
                    mcSHOW_USER_MSG(("MonitorProcess: Demux PER > 5xDemod PER. Conf_Count=%d\n", s2monitor_demux_conf_count));
                }
                else
                {
                    mcSHOW_USER_MSG(("MonitorProcess: Demux packet check. Not increase cofidence count. Conf_Count=%d\n", s2monitor_demux_conf_count));
                }
                
                if (s2monitor_demux_conf_count >= fcMONITOR_DEMUX_CONF_COUNT_THRESHOLD)
                {
                    mcSHOW_USER_MSG(("MonitorProcess: Demux Drop Most Pkt & Demod Is Good. Need Reset Demod\n"));
                    s2monitor_demux_conf_count = -1;
                    fgNoAudVid = FALSE;
                    s2monitor_audvid_conf_count = 0;
                    
                    //DVBC_AcqStart(psDvbcDemodCtx);
                    // Restart Descrambler
                    DVBC_SetDescramblerLock(psDvbcDemodCtx, TRUE);
                    fgNoAudVidReset1 = TRUE;
                    s2reset_timer1 = 0;
                }                             
            }            
            else
            {
                if (fgNoAudVidReset1==TRUE)
                {
                    s2reset_timer1++;

                    if (s2reset_timer1 >= 30)
                    {
                        s2reset_timer1 = 0;
                        DVBC_SetDescramblerLock(psDvbcDemodCtx, FALSE);
                        fgNoAudVidReset1 = FALSE;
                        mcSHOW_USER_MSG(("MonitorProcess: After reset, recover descrambler setting!!\n"));
                    }
                }                    
            }
#endif
            // MW "No Audio and Video" indicator check
            if (fgNoAudVid == TRUE)
            {
                if (DVBC_GetPER(psDvbcDemodCtx)<=20)  // TOV
                {
                    s2monitor_audvid_conf_count++;
                    mcSHOW_USER_MSG(("MonitorProcess: No Audio/Video & Demod Is Good. Conf_Count=%d\n", s2monitor_audvid_conf_count));
                } 
                else
                {
                    //s2monitor_audvid_conf_count = 0;
                    mcSHOW_USER_MSG(("MonitorProcess: No Audio/Video . Not increase cofidence count. Conf_Count=%d\n", s2monitor_audvid_conf_count));
                }

                if (s2monitor_audvid_conf_count >= fcMONITOR_AUDVID_CONF_COUNT_THRESHOLD)
                {
                    mcSHOW_USER_MSG(("MonitorProcess: No Audio/Video & Demod Is Good. Need Reset Demod\n"));
                    s2monitor_audvid_conf_count = 0;
                    s2monitor_demux_conf_count = -1;
                    fgNoAudVid = FALSE;
                    //DVBC_AcqStart(psDvbcDemodCtx);
                    // Restart Descrambler
                    DVBC_SetDescramblerLock(psDvbcDemodCtx, TRUE);
                    fgNoAudVidReset2 = TRUE;
                    s2reset_timer2 = 0;
                } 
            }            
            else
            {
                if (fgNoAudVidReset2==TRUE)
                {
                    s2reset_timer2++;

                    if (s2reset_timer2 >= 30)
                    {
                        s2reset_timer2 = 0;
                        DVBC_SetDescramblerLock(psDvbcDemodCtx, FALSE);
                        fgNoAudVidReset2 = FALSE;
                        mcSHOW_USER_MSG(("MonitorProcess: After reset, recover descrambler setting!!\n"));
                    }
                }                    
            }
#endif

            // ***** Qtest Info Update ***** //
            if ((fgPec)||(fgPec_dtd_y))
            {
                u4AccTotPkt += psDvbcDemodCtx->u2TotalPkt;
                u4AccErrPkt += psDvbcDemodCtx->u2ErrorPkt;
                u4AccCorBit += psDvbcDemodCtx->u4CorrBit;

                u4AccPostViterbiBER += DVBC_GetPostVBer(psDvbcDemodCtx);
                u4AccPostViterbiBER_Cnt = u4AccPostViterbiBER_Cnt + 1;                
                
                if ((psDvbcDemodCtx->u2TotalPkt==0) || (psDvbcDemodCtx->u2ErrorPkt!=0))
                    u2AccEsr++;
                
                if (u4AccTotPkt == 0)
                    u4AccPer = 200000;
                else    
                {
                    if (u4AccErrPkt < 42950) // (0xFFFFFFFF/100000)             
                        u4AccPer = (u4AccErrPkt*100000/u4AccTotPkt);
                    else if (u4AccErrPkt < 429490)
                        u4AccPer = (u4AccErrPkt*10000/u4AccTotPkt)*10;
                    else if (u4AccErrPkt < 4294900)
                        u4AccPer = (u4AccErrPkt*1000/u4AccTotPkt)*100;
                    else if (u4AccErrPkt < 42949000)
                        u4AccPer = (u4AccErrPkt*100/u4AccTotPkt)*1000;
                    else 
                        u4AccPer = (u4AccErrPkt*10/u4AccTotPkt)*10000;                  
                }
                
                // get rst & acq count
                mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_STATUS0;                
                if (mcHOST_CMD(psDvbcDemodCtx, aucData, ccOFST_DVBC_STATUS0_SIZE, 1))
                {
                    ucReAcqCnt = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_REACQ_CNT);
                    ucASRFailCnt = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_ASR_FAIL_CNT);
                    ucSignalNotDct = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_QCN_FAIL_CNT);
                }
 
                // get AR coef
                DVBC_GetReg(psDvbcDemodCtx, 0x176, &ucArCoef, 1);
#ifndef __MODEL_slt__
                // print info to Toolx
                if(fgPec) 
                {
//                    mcSHOW_USER_MSG(("start.pec"));
                    mcSHOW_USER_MSG(("start.log"));

                    //mcSHOW_USER_MSG(("%02d%01d",  LOG_TAG_FFT,  psDvbcDemodCtx->u1Fft));
                    //mcSHOW_USER_MSG(("%02d%01d",  LOG_TAG_GI,   psDvbcDemodCtx->u1GI));
                    //mcSHOW_USER_MSG(("%02d%01d",  LOG_TAG_CR,   psDvbcDemodCtx->u1CR));
                    mcSHOW_USER_MSG(("%02d%02d",  LOG_TAG_SNR,  DVBC_GetSignalSNR(psDvbcDemodCtx)/10));

                    mcSHOW_USER_MSG(("%02d%06d",  LOG_TAG_PEC_PER,     u4AccPer));
                    mcSHOW_USER_MSG(("%02d%05d",  LOG_TAG_PEC_ESR,     u2AccEsr));
                    mcSHOW_USER_MSG(("%02d%010d", LOG_TAG_PEC_PKT_TOT, u4AccTotPkt));
                    mcSHOW_USER_MSG(("%02d%010d", LOG_TAG_PEC_PKT_ERR, u4AccErrPkt));
                    mcSHOW_USER_MSG(("%02d%010d", LOG_TAG_PEC_COR_BIT, u4AccCorBit));
                    mcSHOW_USER_MSG(("%02d%03d",  LOG_TAG_ACQ_CNT,     ucAcqCnt));
                    mcSHOW_USER_MSG(("%02d%03d",  LOG_TAG_RST_CNT,     ucRstCnt));
                    mcSHOW_USER_MSG(("%02d%03d",  LOG_TAG_AR_COEF,     ucArCoef));

//                mcSHOW_USER_MSG(("stop.pec\r\n"));
                    mcSHOW_USER_MSG(("stop.log\r\n"));
                }
                else
                {
                    psDvbcDemodCtx->u2PecRefreshTime = psDvbcDemodCtx->u2PecRefreshTime - 1; 
                    psDvbcDemodCtx->u4PecObserveTime = psDvbcDemodCtx->u4PecObserveTime - 1;
                    
                    if(psDvbcDemodCtx->u2PecRefreshTime == 0)
                    {
                        psDvbcDemodCtx->u2PecRefreshTime = u2KeepRefreshTime;
//                        mcSHOW_USER_MSG(("\n************************\n"));
                        mcSHOW_USER_MSG(("SNR = %03d/10  ",  DVBC_GetSignalSNR(psDvbcDemodCtx)));
                        mcSHOW_USER_MSG(("Total_Pkt = %010d  ",  u4AccTotPkt));
                        mcSHOW_USER_MSG(("Error_Pkt = %010d  ",  u4AccErrPkt));
                        mcSHOW_USER_MSG(("Acc_Per = %06d  ", u4AccPer));
                        mcSHOW_USER_MSG(("Acc_B4RSBER = %06dE-5  ", (u4AccPostViterbiBER/u4AccPostViterbiBER_Cnt)));
                        mcSHOW_USER_MSG(("ReAcq_Cnt = %03d  ",  ucReAcqCnt));
                        mcSHOW_USER_MSG(("ASRFail_Cnt = %03d  ",  ucASRFailCnt));
                        mcSHOW_USER_MSG(("SgNotDct_Cnt = %03d  ",  ucSignalNotDct));                        
                        mcSHOW_USER_MSG(("PEC_Sec = %04d\n",  psDvbcDemodCtx->u4PecObserveTime));                        
//                        mcSHOW_USER_MSG(("\n************************\n"));
                    }

                    if(psDvbcDemodCtx->u4PecObserveTime == 0)
                    {
                        fgPec_dtd_y = FALSE;
                    }

                }
            
#else
                mcSHOW_USER_MSG(("\n************************\n"));
                mcSHOW_USER_MSG(("SNR = %03d/10  ",  DVBC_GetSignalSNR(psDvbcDemodCtx)));
                mcSHOW_USER_MSG(("Total_Pkt = %010d  ",  u4AccTotPkt));
                mcSHOW_USER_MSG(("Error_Pkt = %010d  ",  u4AccErrPkt));
                mcSHOW_USER_MSG(("Acc_Per = %06d  ", u4AccPer));
                mcSHOW_USER_MSG(("Acc_B4RSBER = %06dE-5  ", (u4AccPostViterbiBER/u4AccPostViterbiBER_Cnt)));
                mcSHOW_USER_MSG(("ReAcq_Cnt = %03d  ",  ucReAcqCnt));
                mcSHOW_USER_MSG(("ASRFail_Cnt = %03d  ",  ucASRFailCnt));
                mcSHOW_USER_MSG(("SgNotDct_Cnt = %03d  ",  ucSignalNotDct));                        
                mcSHOW_USER_MSG(("PEC_Sec = %04d\n",  psDvbcDemodCtx->u4PecObserveTime));    
                mcSHOW_USER_MSG(("\n************************\n"));
				//lei_code_dB110601,add for SLT building warning;
				mcSHOW_DBG_MSG4(("Reset_Cnt = %03d\n",  ucRstCnt));
				mcSHOW_DBG_MSG4(("Acq_Cnt = %03d\n",  ucAcqCnt));
				mcSHOW_DBG_MSG4(("u2KeepRefreshTime = %03d\n",  u2KeepRefreshTime));
#endif
            }

            // mutex unlock
            //mcSHOW_DBG_MSG4(("MonitorProcess unlock pDVBCPdCtx->hHalLock\n"));
            mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
            mcSHOW_DBG_MSG4(("escape  MonitorProcess unlock pDVBCPdCtx->hHalLock\n"));

            // Sleep 
            mcDELAY_MS(u2ToolxInfoUpdateRate); 
            loop ++;
        }
    }
    
    t_monitor_thread = NULL;
    mcSHOW_DBG_MSG(("MonitorProcess exit!\n"));
    mcTHREAD_DESTROY();
}

static S32 PD_StartMonitorEngine(void)
{
    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not 
    // directly use pv_arg. Therefore, if we want thread to share the same 
    // context as caller, we need set pv_arg as "pointer of context".
    fgMonitorThreadRunning = TRUE;
    if (mcTHREAD_CREATE(&t_monitor_thread,
            "DVBC_MonThread",
            cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
            MonitorProcess, sizeof(void*), NULL) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("PD_StartMonitorEngine (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSHOW_DBG_MSG(("PD_StartMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}

static S32 PD_StopMonitorEngine(void)
{
//    fgMonitorThreadRunning = FALSE;  //must set to FALSE before unlock sema t_escape_mon_suspend
    while (t_monitor_thread){
        mcDELAY_MS(10); // in ms
    }
    mcSHOW_DBG_MSG(("DVBC PD_StopMonitorEngine success\n"));
    return (DRVAPI_TUNER_OK);
}
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
    else
    {
        return FALSE;
    }
}
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
#if 0
    if(u4Default ==0 ){ //u4Default value is get from drv_cust setting. drv_cust no need to know exactly tunerType value
        u1DeftType = cPANASONIC_ENG37E; 
    }
    else{ //drv_cust can change EFAULT_TUNER_TYPE_DEFAULT from 0 to other value, here can mapping default tunertype to others
        u1DeftType = cSAMSUNG_DTOS40A;
    }
#endif

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
}
//this API leverage from 5112, it export to analog pd code to get tuner id from EEPROM
//Jackson todo: arch. need refine
//UINT8 u1GetTunerType(DEMOD_CTX_T *psDemodCtx, UINT8 u1TunerType, BOOL fgAnaTunerType)
UINT8 DVBC_GetTunerType(VOID)
{
    //return checkTunerTypeFromEeprom();
    mcSHOW_DBG_MSG(("TunerType(0x%x) from digital PD\r\n", ucTunerType));
    return (ucTunerType);
}

/***********************************************************************/
char *DVBC_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{

#if 0 
    mcSHOW_DBG_MSG4(("DVBC_TunerGetVer pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerGetVer pDVBCPdCtx->hHalLock\n"));
    
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerGetVer unlock pDVBCPdCtx->hHalLock\n"));
    return NULL;
#endif

    DVBC_PD_CTX_T *psDVBC_PDCtx = (DVBC_PD_CTX_T *) ptTDSpecificCtx;
    DEMOD_CTX_T *psDemodCtx = psDVBC_PDCtx->pDemodCtx;
    CHAR    *szRetValue;

    mcSEMA_LOCK_HAL(psDVBC_PDCtx->hHalLock);   // Avoid re-entry

    szRetValue = DVBC_GetVer(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psDVBC_PDCtx->hHalLock);
    return (szRetValue);    
}

/***********************************************************************/
void DVBC_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, 
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSHOW_DBG_MSG4(("DVBC_TunerSetRegSetting pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerSetRegSetting pDVBCPdCtx->hHalLock\n"));

    
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerSetRegSetting unlock pDVBCPdCtx->hHalLock\n"));

}

void DVBC_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG4(("DVBC_TunerShowRegSetting pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerShowRegSetting pDVBCPdCtx->hHalLock\n"));

    
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerShowRegSetting unlock pDVBCPdCtx->hHalLock\n"));

    
}

void DVBC_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG4(("DVBC_TunerTestI2C pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerTestI2C pDVBCPdCtx->hHalLock\n"));
    
    
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerTestI2C unlock pDVBCPdCtx->hHalLock\n"));    
}

/**********************************************************************/

/**********************************************************************/
S32 DVBC_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    S32 i4Ret = 0;
    mcSHOW_DBG_MSG4(("DVBC_TunerNimTest pDVBCPdCtx->hHalLock\n"));    
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerNimTest pDVBCPdCtx->hHalLock\n"));    

    //add your implementation here
    #ifdef CC_CAB_DEMOD_TUNER_BYPASS
    DigTunerBypassI2C(TRUE);
    #endif

      i4Ret = Tuner_Connect(666000, DVBC_BW_8MHz, FALSE);

    #ifdef CC_CAB_DEMOD_TUNER_BYPASS
    DigTunerBypassI2C(FALSE);
    #endif

    mcSHOW_DRVERR_MSG(("DVBC_TunerNimTest : %d (0:OK, 1:fail)\n", i4Ret));
    
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerNimTest unlock pDVBCPdCtx->hHalLock\n"));  


    return i4Ret;
}

/**********************************************************************/
DVBC_PD_CTX_T *DVBC_GetPdCtx(void)
{
    return pDVBCPdCtx;
}

/**********************************************************************/
S32 DVBC_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, 
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    //UCHAR     test = 0;
    //UINT32 i;
    // return (DRVAPI_TUNER_OK);
#if (fcTUNER_TYPE == cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST)||(fcTUNER_TYPE == cTPV2K11EU_TUNERLST)
    UINT8 tunerType;
#endif

    pDvbcTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    /*create DVBC PD CTX*/
    pDVBCPdCtx = (DVBC_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBC_PD_CTX_T));
    if (pDVBCPdCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    /* Init Disconnect Status */
    pDVBCPdCtx->fgDisStatus = FALSE;
    
    *pptTDSpecificCtx = pDVBCPdCtx;

    if (mcSEMA_CREATE(&pDVBCPdCtx->hHalLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("DVBC_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);

    // Register to TDMgr
    PD_DVBC_Register_LTDIS_Fct(ptTDCtx);
        
    psDvbcDemodCtx = DVBC_DemodCtxCreate();
    if (psDvbcDemodCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    psDvbcDemodCtx->psTunerCtx = ITunerGetCtx();

    /*link demod ctx to pd ctx*/
    pDVBCPdCtx->pDemodCtx = psDvbcDemodCtx;

#if defined(CC_MT5135) ||defined (CC_MT5175)
   SPIBus_Init();
#endif

    // do tuner init before demod init, for demod init need tuner parameters
 #if (fcTUNER_TYPE != cDVBT_TUNER_ALL) &&(fcTUNER_TYPE != cDVBT_2K10_TUNERLST)&& (fcTUNER_TYPE != cTPV2K11EU_TUNERLST)  //liuqu,20090716,add for DVBT TunerAll structure
    ITunerRegistration(cITUNER_DONT_CARE);
 #else
    tunerType = checkTunerTypeFromEeprom();
    ITunerRegistration(tunerType);
    mcSHOW_USER_MSG(("Register DVBC Tunerall type = %d\n",tunerType));     
 #endif
    ITuner_Init(ITunerGetCtx());

          
    // do demod init
    if (DVBC_Initialize(psDvbcDemodCtx, DEFAULT_I2C_ADDR, TRUE)){
        mcSHOW_USER_MSG(("DVBC_Initialize failed\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    // Ken, Set TS to Tri-state for CI CAM compatibility after Power on 
    DVBC_SetTsPathMode(psDvbcDemodCtx, FALSE);

    mcSHOW_DBG_MSG(("DVBC Disconnect @ TunerOpen!!\n"));
    DVBC_DisConnect(psDvbcDemodCtx);   // add by Ken, default disconnect demod for TS_CLK EMI issue, 090114

//#if !defined(CC_MTAL) && !defined(CC_SDAL)
    if(pDvbcTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbcTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        if (mcSEMA_CREATE(&pDVBCPdCtx->t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
        {
        mcSHOW_DRVERR_MSG(("DVBC_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
            return (DRVAPI_TUNER_ERROR);
        }

       // Start Monitoring Thread
       fgBreakMonitor = TRUE;
        PD_StartMonitorEngine();
    }
//#endif
    return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
S32 DVBC_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
    if(pDvbcTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbcTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        fgBreakMonitor = FALSE;
        fgMonitorThreadRunning = FALSE;  //must set to FALSE before unlock sema t_escape_mon_suspend
        mcMUTEX_UNLOCK(pDVBCPdCtx->t_escape_mon_suspend);
        PD_StopMonitorEngine();
        mcMUTEX_DESTROY(pDVBCPdCtx->t_escape_mon_suspend);
    }
//#endif
    DVBC_DemodCtxDestroy(psDvbcDemodCtx);


    mcMUTEX_DESTROY(pDVBCPdCtx->hHalLock);
    
    return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
S16 DVBC_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        DRV_CONN_TYPE_T e_conn_type)
{
    S16 s2_ret = FALSE;
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    
    // report tuner unsync if TS unlocked or high PER 
    DVBC_UpdateStatus(psDvbcDemodCtx);
    if (DVBC_GetTsLock(psDvbcDemodCtx) == TRUE)
    {
    // in SDAL, TunerGetSync() is used in ch scan => don't use PER to drop ch
    // and also PER is not updated in SDAL
    //#ifndef CC_SDAL 
        if(pDvbcTunerDeviceCtx->u1MWIntf != cSDAL_IF)
        {
           if (DVBC_GetPER(psDvbcDemodCtx) < DVBC_SYNC_PER_THRESHOLD)
                s2_ret = TRUE;  
        }
        else
        {
            s2_ret = TRUE;
        }
    //#endif
    //        s2_ret = TRUE;
    }

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    return s2_ret;
}

void DVBC_SetTSInterface(BOOL fgParallel)
{
    mcSHOW_DBG_MSG4((" DVBC_SetTSInterface pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" escape DVBC_SetTSInterface pDVBCPdCtx->hHalLock\n"));

    DVBC_SetTsOutputMode(psDvbcDemodCtx, fgParallel);

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DVBC_SetTSInterface unlock pDVBCPdCtx->hHalLock\n"));
}

void DVBC_SetTSPath(BOOL fgEnable)
{
    mcSHOW_DBG_MSG4((" DVBC_SetTSPath pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" escape DVBC_SetTSPath pDVBCPdCtx->hHalLock\n"));

    DVBC_SetTsPathMode(psDvbcDemodCtx, fgEnable);

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DVBC_SetTSPath unlock pDVBCPdCtx->hHalLock\n"));
}

BOOL DVBC_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type, 
                    void* pv_conn_info, 
                    SIZE_T z_conn_info_len, 
                    x_break_fct _BreakFct,
                    void* pvArg)
{
    TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *) pv_conn_info;
    U32     freq;
    UINT16  symbolRate = 0;
    UINT32  mode;
    UINT8   bw = 0;
    U16     ii = 0;
    UINT8   fgChannelScan = FALSE;
    BOOL    fgRetSts = FALSE;
    BOOL    fgConnSts = TRUE;
    UINT8   u1DvbcDemodState;
    UINT32  u4TickCnt, u4ConnTime;
    UINT8   ucUnstableCnt = 0;

    //Set DVB-C scan mode
    pDVBCPdCtx->t_ScanMode = pt_tuner_info->e_conn_mode;   

    mcSHOW_DBG_MSG4((" DVBC_TunerAcq pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" escape DVBC_TunerAcq pDVBCPdCtx->hHalLock\n"));

    MTDVBCisBreak = _BreakFct;

    DVBC_ChipInit(psDvbcDemodCtx);// Power up AD in the beginning,Init Chip Register;
#if defined (CC_MT5135) || defined(CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368) ||defined (CC_MT5175) 
    DVBC_FwSwitch(psDvbcDemodCtx);/* For Remap Use */
#endif

    DVBC_DriverStop(psDvbcDemodCtx);  

    DVBC_SetNormalMode(psDvbcDemodCtx);

    //Convert freq
    freq = pt_tuner_info->ui4_freq/1000;
    mcSHOW_DBG_MSG(("_Freq1 = %d kHz, \n", freq));  

    //Convert symbol rate
    symbolRate = pt_tuner_info->ui4_sym_rate/1000;
    //Convert QAM mode
    switch (pt_tuner_info->e_mod)
    {
        case MOD_QAM_16:
            mode = constellation_qam16;
            break;
        case MOD_QAM_32:
            mode = constellation_qam32;
            break;
        case MOD_QAM_64:
            mode = constellation_qam64;
            break;
        case MOD_QAM_128:
            mode = constellation_qam128;
            break;
        case MOD_QAM_256:
            mode = constellation_qam256;
            break;
        case MOD_UNKNOWN:
            mode = constellation_AutoQam;
            break;
        default:
            mode = constellation_qam64;
            mcSHOW_DBG_MSG(("Invalid input modulation(%d) to DVBC_TunerAcq!\n", pt_tuner_info->e_mod));
            break;
    }

    mcSHOW_DBG_MSG(("symbol rate = %d\n", symbolRate));
    mcSHOW_DBG_MSG(("Mode = %d\n", mode));

    if(pDVBCPdCtx->t_ScanMode == CONN_MODE_SCAN)  //channel scan, LC 080327
    {
        fgChannelScan = TRUE;
    }
    fgBreakMonitor = TRUE;  //if monitor thread is running, pause it.

    //Do Tuner Connect and Cal Cost Time
    u4TickCnt = os_get_sys_tick();

    DigTunerBypassI2C(TRUE);

    if (Tuner_Connect(freq, bw, fgChannelScan))
    {
        mcSHOW_DBG_MSG(("Failed on Tuner_SetFreq(%d) \n", freq));
        fgConnSts = FALSE;
    }

    DigTunerBypassI2C(FALSE);

    u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
    mcSHOW_DBG_MSG4(("DVBC tuner connection time = %dms\n", u4ConnTime));

    if ((u4ConnTime > DVBC_TUNER_CONN_TIME_THREHOLD) && (fgChannelScan))
    {
        mcSHOW_DBG_MSG(("EU Driver ATP Fail: DVBC tuner connection time = %dms, too long!!\n", u4ConnTime));
    }
#ifdef  EU_AUTO_ATP
         mcSHOW_DRVAPI_MSG(("ATPFailBitMskBefore =%d\n",gATPFailBitMsk));
         gATPFailBitMsk |=0X04 ;// DVBC ATP Fail !
         mcSHOW_DRVAPI_MSG(("ATPFailBitMskAfter =%x\n",gATPFailBitMsk));
#endif

        // set QAM
        DVBC_SetQamMode(psDvbcDemodCtx, mode);
        //Set Symbol Rate
        DVBC_SetSymbolRate(psDvbcDemodCtx, symbolRate);
        if(fgChannelScan)  //channel scan
        {
            // disbale Advance AQM/ASR
            DVBC_AdvAQM(psDvbcDemodCtx, FALSE);
            DVBC_AdvASR(psDvbcDemodCtx, FALSE);
            mcSHOW_DBG_MSG(("DVBC TunerAcq: Adv AQM and ASR disabled\n"));
        }
        else
        {
            // Enbale Advance AQM/ASR
            DVBC_AdvAQM(psDvbcDemodCtx, TRUE);
            DVBC_AdvASR(psDvbcDemodCtx, TRUE);
            mcSHOW_DBG_MSG(("DVBC TunerAcq: Adv AQM and ASR enabled\n"));
        }

        DVBC_SetTsOutput(psDvbcDemodCtx, FALSE);
        // driver start
        DVBC_DriverStart(psDvbcDemodCtx);
        //Start Acq Start
        DVBC_AcqStart(psDvbcDemodCtx);

        if (fgConnSts)
        {
            // polling lock loop
            for (ii = 0; ii < DVBC_LOCK_TIME_OUT_CNT; ii++)
            {   // get demod sync state
                u1DvbcDemodState = DVBC_GetSync(psDvbcDemodCtx);
                if (u1DvbcDemodState == DVBC_DEMOD_STATE_UNLOCK) {
                    mcSHOW_DBG_MSG(("TS unlock! DVB-C not exist!\n"));
                    fgRetSts = FALSE;
                    break;
                }
                else if (u1DvbcDemodState == DVBC_DEMOD_STATE_LOCK) {
                    if(DVBC_CheckLocked(psDvbcDemodCtx))
                        fgRetSts = TRUE;
                    else
                    {
                        mcSHOW_DBG_MSG(("DVB-C Locked, but ASR-CFO>500KHz!\n"));
                        fgRetSts = FALSE;
                    }
                    break;
                }
                else if (u1DvbcDemodState == DVBC_DEMOD_STATE_UNSTABLE)
                {
                    ucUnstableCnt++;
                    mcSHOW_DBG_MSG(("Demod state is unstable for %d times\n", ucUnstableCnt));
                }
                else
                {
                    mcSHOW_DBG_MSG2(("Demod state is unknown.\n"));
                }
                //break function
                if (MTDVBCisBreak(pvArg)){
                    mcSHOW_USER_MSG(("break DVBC_TunerAcq()!"));
                    fgRetSts = FALSE;
                    break;
                }
                // delay for a while
                mcDELAY_MS(DVBC_MON_POLL_INTRVL);
            }   
            // if time-out
            if(ii >= DVBC_LOCK_TIME_OUT_CNT)
            {
                if (ucUnstableCnt > 0)   // Unstable and timeout, still return to lock for BE to try to decode
                {
                    fgRetSts = TRUE;
                    mcSHOW_DBG_MSG(("Happend TS lock for %d times, return lock. time-out\n", ucUnstableCnt));
                }
                else
                {
                    mcSHOW_DBG_MSG(("TPS unlock! time-out\n"));
                }
            }
            else // check Demod lock time only in non-time-out case
            {
                u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
                mcSHOW_DBG_MSG4(("DVBC Demod connection time = %dms\n", u4ConnTime));
                
                if ((u4ConnTime > DVBC_DEMOD_CONN_TIME_THREHOLD) && (fgChannelScan)){
                    mcSHOW_DBG_MSG(("EU Driver ATP Fail: DVBC demod connection time = %dms, too long!!\n", u4ConnTime));
                }
            }
        }

    //DVBC_UpdateInfo(psDvbcDemodCtx);   // Ken, for context update, best mux, 081217
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DVBC_TunerAcq unlock pDVBCPdCtx->hHalLock\n"));

    if(fgRetSts) 
    { // print info after update
        mcSHOW_DBG_MSG(("TS lock, freq = %d kHz, lock time = %d ms, SNR = %d/10 dB\n", 
           freq, ii*DVBC_MON_POLL_INTRVL, DVBC_GetSignalSNR(psDvbcDemodCtx)));

        DVBC_SetTsOutput(psDvbcDemodCtx, TRUE);
    }
#ifdef fcQINGDAO_NO_AUD_VID_DSCRM_PATCH
    // after tuneracq, default not always detect for descrambler
    DVBC_SetDescramblerLock(psDvbcDemodCtx, FALSE);
    fgNoAudVid = FALSE;
#endif

    fg1stMonitorRun = FALSE;   // Ken, 090202, check Monitor Process actually execute, for best mux

    //#if !defined(CC_MTAL) && !defined(CC_SDAL)
    if(pDvbcTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbcTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        //++ Escape monitor suspend
        fgBreakMonitor = FALSE;
        mcMUTEX_UNLOCK(pDVBCPdCtx->t_escape_mon_suspend);
    }
    //#endif

    /* Init Disconnect Status */
    pDVBCPdCtx->fgDisStatus = FALSE;

    return fgRetSts;
}

/**********************************************************************/
void DVBC_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                    DRV_CONN_TYPE_T e_conn_type, 
                    SIGNAL *_pSignal)
{
    //UINT8 ret;

    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignal pDVBCPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerGetSignal pDVBCPdCtx->hHalLock\n"));                

    _pSignal->Frequency  = (S32)(psDvbcDemodCtx->psTunerCtx->u4RF_Freq);

    DVBC_UpdateInfo(psDvbcDemodCtx); // Get DVBC Info

    _pSignal->SymbolRate = psDvbcDemodCtx->u2SymbolRate;
    switch (psDvbcDemodCtx->ucQamMode)
    {
        case constellation_qam16:  _pSignal->QAMSize = 16; break;//MOD_QAM_16;   break;
        case constellation_qam32:  _pSignal->QAMSize = 32; break;//MOD_QAM_32;   break;
        case constellation_qam64:  _pSignal->QAMSize = 64; break;//MOD_QAM_64;   break;
        case constellation_qam128: _pSignal->QAMSize = 128; break;//MOD_QAM_128;     break;
        case constellation_qam256: _pSignal->QAMSize = 256; break;//MOD_QAM_256;     break;
        default: _pSignal->QAMSize = BW_UNKNOWN; break;
    }

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignal unlock pDVBCPdCtx->hHalLock\n"));                    
}

/**********************************************************************/
U8   DVBC_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                DRV_CONN_TYPE_T e_conn_type)
{
    U8 u1_ret;

    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalLevel pDVBCPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerGetSignalLevel pDVBCPdCtx->hHalLock\n"));                

    u1_ret = DVBC_GetSignalLevel(psDvbcDemodCtx);

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalLevel unlock pDVBCPdCtx->hHalLock\n"));
    return u1_ret;
}

/**********************************************************************/
S16 DVBC_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)
{
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalLeveldBm pDVBCPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerGetSignalLeveldBm pDVBCPdCtx->hHalLock\n"));         

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalLeveldBm unlock pDVBCPdCtx->hHalLock\n"));
    return 0;
}

/**********************************************************************/
UINT16  DVBC_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)
{    
    UINT16 SNR;
    
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalSNR pDVBCPdCtx->hHalLock\n"));                   
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerGetSignalSNR pDVBCPdCtx->hHalLock\n"));                    

    SNR = DVBC_GetSignalSNR(psDvbcDemodCtx);

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalSNR unlock pDVBCPdCtx->hHalLock\n"));

//#ifdef CC_SDAL
    if(pDvbcTunerDeviceCtx->u1MWIntf == cSDAL_IF)
    {
        return (UINT16)((SNR * 10) >> 3);
    }
//#else
    else
    {
        return (UINT16)(SNR >> 3);
    }
//#endif
}

UINT16  DVBC_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)  // return value = PER*5*10^4
{
    
    //INT32 u4_ret;
#ifdef __MODEL_slt__
    ////mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalPER pDVBCPdCtx->hHalLock\n"));                
    ////mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);

    ////mcSHOW_DBG_MSG4(("escape DVBC_TunerGetSignalPER pDVBCPdCtx->hHalLock\n"));     
    ////mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);

    if (u4AccPer >= 50000)
    {
        return 50000;
    }
    else
    {
        return (UINT16) (u4AccPer);
    }   
#else
    INT32 u4_ret;
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalPER pDVBCPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerGetSignalPER pDVBCPdCtx->hHalLock\n"));                

    // Justin, move to update update Demod context to thread
    //DVBT_GetInfo(psDvbcDemodCtx);
    u4_ret = DVBC_GetPER(psDvbcDemodCtx);    

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerGetSignalPER unlock pDVBCPdCtx->hHalLock\n")); 
    
    if ( u4_ret == -1 )
    {
        return 50000;
    }
    else
    {
        return (UINT16)(u4_ret >> 1);
    }
#endif
}

/**********************************************************************/
void DVBC_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                            MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSHOW_DBG_MSG4(("DVBC_TunerSetMpgFmt pDVBCPdCtx->hHalLock\n"));                    
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerSetMpgFmt pDVBCPdCtx->hHalLock\n"));                        

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerSetMpgFmt unlock pDVBCPdCtx->hHalLock\n")); 

}

void DVBC_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucCmdId = 0, ucPar, ucData[256];
    U16    ii, jj, kk, u2RegAddr;
    UINT8   i4CurTop = 0;
    UINT8   ucData1;
    UINT8 temp[2];
    UINT8 RegistWord[5]; 
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDvbcDemodCtx->psTunerCtx->specific_member.eu_ctx);
    
    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t e [Register (UINT8)]: Set or Get 5 Register,The CLI input formats are: nim.atd e or nim.atd e ##-##-##-##-##\n"));
        mcSHOW_USER_MSG(("\t t                    [Top]: SetTOP\n"));
        mcSHOW_USER_MSG(("\t d:                   Dump DVBC registers\n"));
        mcSHOW_USER_MSG(("\t s:                   show signal/ts/tps status\n"));
//        mcSHOW_USER_MSG(("\t n  Normal/Standby mode switch (0: Normal; 1: Standby)\n"));
        mcSHOW_USER_MSG(("\t r [RegAddr] [Num]:   read  Demod register\n"));
        mcSHOW_USER_MSG(("\t w [RegAddr] [Value]: write Demod register\n"));        
        mcSHOW_USER_MSG(("\t v [1/0]:             Driver on/off\n"));        
        //SHOW_USER_MSG(("\t t [TopValue]:        Set DTV TOP \n"));
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
        if(pDvbcTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbcTunerDeviceCtx->u1MWIntf != cSDAL_IF)
        {
            mcSHOW_USER_MSG(("\t mp:                  Pause monitor thread\n"));
            mcSHOW_USER_MSG(("\t mr:                  Resume monitor thread\n"));
        }
//#endif
    }   
    
        mcSHOW_DBG_MSG4(("DVBC_TunerDtdCommand pDVBCPdCtx->hHalLock\n"));
        mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
        mcSHOW_DBG_MSG4(("escape DVBC_TunerDtdCommand pDVBCPdCtx->hHalLock\n"));
    
    
    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
        case 's':
        // no monitor thread in MTAL/SDAL, do info update in nim.dtd s
        #if (defined(CC_MTAL) || defined(CC_SDAL))
            DVBC_UpdateInfo(psDvbcDemodCtx);
        #endif

            mcSHOW_USER_MSG(("=================================\n"));
            mcSHOW_USER_MSG(("Digital Frequency lower bound=%d Hz\n", pEuTunerCtx->m_s4FreqBoundLower));
            mcSHOW_USER_MSG(("Digital Frequency upper bound=%d Hz\n", pEuTunerCtx->m_s4FreqBoundUpper));
            mcSHOW_USER_MSG(("Analog Frequency lower bound=%d Hz\n", pEuTunerCtx->m_s4AnaFreqBoundLower));
            mcSHOW_USER_MSG(("Analog Frequency upper bound=%d Hz\n", pEuTunerCtx->m_s4AnaFreqBoundUpper));

            switch (psDvbcDemodCtx->ucQamMode)
            {
                case 0: mcSHOW_USER_MSG(("QamMode = 16 QAM\n"));   break;
                case 1: mcSHOW_USER_MSG(("QamMode = 32 QAM\n"));   break;
                case 2: mcSHOW_USER_MSG(("QamMode = 64 QAM\n"));   break;
                case 3: mcSHOW_USER_MSG(("QamMode = 128 QAM\n"));  break;
                case 4: mcSHOW_USER_MSG(("QamMode = 256 QAM\n"));  break;
                default: mcSHOW_USER_MSG(("QamMode = N/A QAM\n")); break;
            }
            mcSHOW_USER_MSG(("SymbolRate = %dKs/S\n", psDvbcDemodCtx->u2SymbolRate));
            mcSHOW_USER_MSG(("CFO = %dkHz\n", DVBC_GetCFO(psDvbcDemodCtx)/100000));
            mcSHOW_USER_MSG(("TFO = %dppm\n", DVBC_GetTFO(psDvbcDemodCtx)));
            mcSHOW_USER_MSG(("SNR = %d/10 dB\n", DVBC_GetSignalSNR(psDvbcDemodCtx) ));
            mcSHOW_USER_MSG(("=================================\n"));         
            mcSHOW_USER_MSG(("TsLock = %x\n", DVBC_GetTsLock(psDvbcDemodCtx)));
            mcSHOW_USER_MSG(("=================================\n"));
            mcSHOW_USER_MSG(("B4 RS BER = %d * 10^-5\n", DVBC_GetPostVBer(psDvbcDemodCtx)));
            mcSHOW_USER_MSG(("PER = %d * 10^-5\n", DVBC_GetPER(psDvbcDemodCtx)));
            mcSHOW_USER_MSG(("=================================\n"));
        #if defined (CC_MT5135) || defined(CC_MT5396)|| defined(CC_MT5368) ||defined (CC_MT5175)//EC_code_dB330: For MT5396DVBTC demod
            mcSHOW_USER_MSG(("SLD RFAGC CW = 0.%d\n", psDvbcDemodCtx->s2SldPraRfagcCtrlWord/41));
            mcSHOW_USER_MSG(("SLD IFAGC CW = 0.%d\n", psDvbcDemodCtx->s2SldPraIfagcCtrlWord/41));
            mcSHOW_USER_MSG(("SLD PRA Gain Idx = 0x%X\n", psDvbcDemodCtx->ucSldPraGainIdx));
            mcSHOW_USER_MSG(("SLD Target LV = %d\n", psDvbcDemodCtx->s2SldPraOut));
        #endif
            mcSHOW_USER_MSG(("=================================\n"));
#ifdef EU_AUTO_ATP
        if (gATP_TEST_ITEM_BIT & 0x01)
        {
            mcSHOW_DBG_MSG(("##BER = %d\n##PER = %d\n##TSLOCK = %d\n##End\n",
            DVBC_GetPostVBer(psDvbcDemodCtx),
            DVBC_GetPER(psDvbcDemodCtx),
            DVBC_GetTsLock(psDvbcDemodCtx)));
        }
        if (gATP_TEST_ITEM_BIT & 0x02) 
        {
            if(DVBC_GetTsLock(psDvbcDemodCtx))
            {
                mcSHOW_DBG_MSG(("##Start\n##Video = ON\n##End\n" ));
            }
            else
            {
                mcSHOW_DBG_MSG(("##Start\n##Video = OFF\n##End\n" ));
            }
        }
        break;

        case 'z':
            if (i4Argc > 1)
            {
                gATP_TEST_ITEM_BIT = StrToInt(aszArgv[1]);
            }
            mcSHOW_USER_MSG(("AUTO ATP debug level:%x\n",gATP_TEST_ITEM_BIT));
#endif
        break;
 
        case 'd':
            for (kk=0; kk<3; kk++)
            {
                if(DVBC_DumpRegisters(psDvbcDemodCtx, ucData, kk, 8))
                {
                    mcSHOW_USER_MSG(("MTDVBC Register Page %d Dump Fail!\n", kk));
                }
                else
                {
                    mcSHOW_USER_MSG(("MTDVBC Register Page %d Dump Result:\n", kk));
                    mcSHOW_USER_MSG(("   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));
                    for (ii=0; ii<16; ii++)
                    {
                        mcSHOW_USER_MSG(("%02X ", ii));
                        for (jj=0; jj<16; jj++)
                        {
                            mcSHOW_USER_MSG(("%02x ", ucData[ii*16+jj]));
                        }
                        mcSHOW_USER_MSG(("\n"));
                    }
                }
            }
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
                    while ( ((CHAR *) aszArgv[1])[ii] != '\0')
                    {
                        while((((CHAR *) aszArgv[1])[ii] != '-') && (((CHAR *) aszArgv[1])[ii] != '\0'))
                        { 
                            if(jj>1) break;
                            temp[jj] = (UINT8)(((CHAR *) aszArgv[1])[ii]);
                            ii++;
                            jj++;
                           
                        }
                        if(jj >= 2)
                        {
                            mcSHOW_USER_MSG(("error!One word's format is ## \n")); 
                            break;
                        }
                        if(kk>4) break;
                        RegistWord[kk] = (UINT8)(StrToHex((CHAR *)&temp,jj));
                        mcSHOW_USER_MSG(("the input word is 0x%x\n",RegistWord[kk])); 
                        kk++;                        
                        jj = 0;
                        ii++;
                    }

                    if(kk >= 5)
                    {
                        mcSHOW_USER_MSG(("error!One word's format is ## \n"));                        
                    }
                    //ITuner_OP(psDvbcDemodCtx->psTunerCtx, itSetReg,0, &RegistWord); 
                     
                    ITuner_OP( ITunerGetCtx(), itSetReg,0, &RegistWord); 
                    mcSHOW_USER_MSG(("Set Register complete!\n"));
                }
                else if (i4Argc == 1)
                {     
                    for (ii = 0; ii < 5; ii++)
                        RegistWord[ii] = 0;
                    //ITuner_OP(psDvbcDemodCtx->psTunerCtx, itGetReg,0, RegistWord); 
                    ITuner_OP(ITunerGetCtx(), itGetReg,0, RegistWord); 
                    for (ii = 0; ii < 5; ii++)
                    {
                        mcSHOW_USER_MSG(("the output word is 0x%x\n",RegistWord[ii]));
                    }
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
            	PARAM_SETFREQ_T SetFreqPara;
                i4CurTop= (U8)StrToInt((CHAR *) aszArgv[1]);
                //ITuner_OP(psDvbcDemodCtx->psTunerCtx, itSetTop, i4CurTop, &ucData1);           
                SetFreqPara.Freq = (psDvbcDemodCtx->Frequency)/1000;
                SetFreqPara.Modulation = MOD_DVBC;
                SetFreqPara.fgAutoSearch =( psDvbcDemodCtx->u1ChSearchMode & 0x00); //no autosearch,just set freq one time
                ITuner_OP(ITunerGetCtx(), itSetTop, i4CurTop, &ucData1);
                if((ITuner_SetFreq(ITunerGetCtx(), &SetFreqPara)) != 0)
                {
                    mcSHOW_USER_MSG(("Failed on Tuner_SetFreq(%d)", SetFreqPara.Freq));
                    break;
                }
                mcSHOW_USER_MSG(("Set Top = 0x%x\n", i4CurTop));
            }
            break;

        case 'r':
            if (i4Argc < 3)
                break;

            u2RegAddr = (U16)StrToInt(aszArgv[1]);
            ucPar = (U8)StrToInt(aszArgv[2]);
            if (DVBC_GetReg(psDvbcDemodCtx, u2RegAddr, ucData, ucPar))
            {
                mcSHOW_USER_MSG(("MTDVBC Get Register 0x%03X Fail!\n", u2RegAddr));
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
            if (DVBC_SetReg(psDvbcDemodCtx, u2RegAddr, &ucPar, 1))
            {
                mcSHOW_USER_MSG(("MTDVBC Set Register 0x%03X Fail!\n", u2RegAddr));
                break;
            }
            mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X, OK!\n", u2RegAddr, ucPar));
            break;

        case 'v':
            if (i4Argc < 2)
                break;

            ucPar = StrToInt((CHAR *) aszArgv[1]);
            if (ucPar)
            {
                //DVBC_DriverStart(psDvbcDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);
                DVBC_DriverStart(psDvbcDemodCtx);
                mcSHOW_USER_MSG(("Driver On!\n"));
            }
            else
            {
                DVBC_DriverStop(psDvbcDemodCtx);
                mcSHOW_USER_MSG(("Driver Off!\n"));
            }
            break;
/*
        case 't':            
            ucData[0] = (U8)StrToInt(aszArgv[1]);
            if (i4Argc < 2)
            {
                mcSHOW_USER_MSG(("DTV TOP = 0x%02X\n", pTunerCtx->m_aucPara[0]));               
            }
            else
            {
                pTunerCtx->m_aucPara[0] = ucData[0];
                Tuner_SetFreq_DVBC(pTunerCtx, pTunerCtx->, MOD_NOT_ATV, FALSE);
                mcSHOW_USER_MSG(("set DTV TOP = 0x%02X\n", ucData[0]));
            }
            break;
*/
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
        case 'm':
            {
                if(pDvbcTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbcTunerDeviceCtx->u1MWIntf != cSDAL_IF)
                {
                    UINT8   ucCmdExt;

                    ucCmdExt = ((CHAR *) aszArgv[0])[1];

                    if (ucCmdExt == 'p')
                    {
                        fgBreakMonitor = TRUE;                        
                    }
                    else if (ucCmdExt == 'r')
                    {
                        fgBreakMonitor = FALSE;
                        mcMUTEX_UNLOCK(pDVBCPdCtx->t_escape_mon_suspend);
                    }
                }
            }
            break;
//#endif
        case 'c':
            if (i4Argc < 2)
                break;

            ucPar = StrToInt((CHAR *) aszArgv[1]);
            if (ucPar)
            {
                DVBC_Connect(psDvbcDemodCtx, psDvbcDemodCtx->ucQamMode, psDvbcDemodCtx->u2SymbolRate); //Chi Zhang Need Modify in the later time   
                mcSHOW_USER_MSG(("DVBC Connect!\n"));
            }
            else
            {
                DVBC_DisConnect(psDvbcDemodCtx);
                mcSHOW_USER_MSG(("DVBC Disconnect!\n"));
            }
            break;

       case 'y':    // stat PEC
            if (i4Argc > 1)
            {
                psDvbcDemodCtx->u4PecObserveTime = StrToInt(aszArgv[1]); // Input1: Observe time(sec)
                if (psDvbcDemodCtx->u4PecObserveTime > 600)
                {
                    psDvbcDemodCtx->u4PecObserveTime = 600;
                    mcSHOW_USER_MSG(("Max observation time is 600 sec!"));
                }

                psDvbcDemodCtx->u2PecRefreshTime = StrToInt(aszArgv[2]); // Input2: Refresh time(sec)
                u2KeepRefreshTime = psDvbcDemodCtx->u2PecRefreshTime;
//                psDvbtDemodCtx->u1PecFlag = 1;              
                DVBC_UpdateInfo(psDvbcDemodCtx);              
//                psDvbtDemodCtx->u2AccReAcqCnt = 0; // clear reacq count for qtest log
                u4AccTotPkt = 0;
                u4AccErrPkt = 0;
                u4AccCorBit = 0;
                u2AccEsr = 0;
                u4AccPer = 0;
                u4AccPostViterbiBER = 0;
                u4AccPostViterbiBER_Cnt = 0;
                fgPec_dtd_y = TRUE;                           
                mcSHOW_USER_MSG(("\n"));
                mcSHOW_USER_MSG(("u2PecRefreshCnt = %04d\n",  psDvbcDemodCtx->u2PecRefreshTime));
                mcSHOW_USER_MSG(("u4PecObserveTime = %05d\n",  psDvbcDemodCtx->u4PecObserveTime));                
            }
            else
            {
                mcSHOW_USER_MSG(("\t y ObserveTime(sec) RefreshTime(sec): Start PEC \n"));
            }
            break;

        case 'z':   // stop PEC
            fgPec_dtd_y = FALSE;
            break;
            
        default:
            break;
    }    

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerDtdCommand unlock pDVBCPdCtx->hHalLock\n"));
}

/**********************************************************************/
void DVBC_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        INT32 i4Argc, const CHAR** aszArgv)
{
    //UCHAR   ucCmdId = 0;
    UCHAR   ucArg1;
    UCHAR   ucCmdSize = 0;
    UINT8   tunerType;
    UINT8   ucData[256];

    mcSHOW_DBG_MSG4(("DVBC_TunerCommand pDVBCPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerCommand pDVBCPdCtx->hHalLock\n"));
 
    if (i4Argc > 0) {
        //ucCmdId = *((CHAR *) aszArgv[0]);
        ucCmdSize = x_strlen((CHAR *) aszArgv[0]);
    }

    switch (ucCmdSize) {
    case 1: // one-word cmd
        if (x_strcmp((CHAR *) aszArgv[0], "s") == 0) 
        {
            if((fcTUNER_TYPE != cDVBT_TUNER_ALL) && (fcTUNER_TYPE != cDVBT_2K10_TUNERLST)){
                mcSHOW_USER_MSG(("Only build tuner all image can use this command\r\n"));           
                break;
            }
            else
            {
                if(i4Argc <= 1 || i4Argc >=3 )
                {
                    mcSHOW_USER_MSG(("\t s [1/2/3/4/5]: Set TunerID to EEPROM (1: ENV57S, 2: FT2101, 3: TDTWS810D,4: FH2607, 5: Query TunerType in EEPROM)\n"));
                    break;
                }
            }
            mcSHOW_USER_MSG(("i4Argc=%d\r\n",i4Argc));
            ucArg1 = (U8)StrToInt(aszArgv[1]);
            mcSHOW_USER_MSG(("i4Argc=%x, ucArg1=%x\r\n",i4Argc,ucArg1));

            switch(ucArg1){
            case 1:
                mcSHOW_USER_MSG(("case 1, cPANASONIC_ENV57S=0x%x\r\n",cPANASONIC_ENV57S));
                setTunerTypeToEeprom(cPANASONIC_ENV57S);
                break;
            case 2:
                mcSHOW_USER_MSG(("case 2, cNXP_FT2101=0x%x\r\n",cNXP_FT2101));
                setTunerTypeToEeprom(cNXP_FT2101);
                break;
            case 3:
                mcSHOW_USER_MSG(("case 3, cLG_TDTWS810D=0x%x\r\n",cLG_TDTWS810D));
                setTunerTypeToEeprom(cLG_TDTWS810D);
                break;
            case 4:
                mcSHOW_USER_MSG(("case 4, cNuTune_FT2607=0x%x\r\n",cNuTune_FT2607));
                setTunerTypeToEeprom(cNuTune_FT2607);
                break;
            default:
                tunerType = checkTunerTypeFromEeprom();
                mcSHOW_USER_MSG(("tunerType decision(after check EEPROM)=0x%x\r\n",tunerType));
                break;
            }     
        }
        break;
    case 2: // two-word cmd
        if (x_strcmp((CHAR *) aszArgv[0], "ts") == 0)
        {
            if (i4Argc > 1)
            {
                if (StrToInt(aszArgv[1]) == 1)   // parallel
                {
                    DVBC_SetTsOutputMode(psDvbcDemodCtx, TRUE);
                    mcSHOW_USER_MSG(("Set TS to parallel mode\n"));
                }
                else if ( StrToInt(aszArgv[1]) == 0)   // serial mode
                {
                    DVBC_SetTsOutputMode(psDvbcDemodCtx, FALSE);
                    mcSHOW_USER_MSG(("Set TS to serial mode\n"));
                }
            }
        }
        break;

    case 4:
        {
            SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDvbcDemodCtx->psTunerCtx->specific_member.eu_ctx);
            if (x_strcmp((CHAR *) aszArgv[0], "stop") == 0) {
                if (i4Argc > 1) {
                    pEuTunerCtx->m_aucPara[0] = (U8)StrToHex((CHAR *) aszArgv[1], 2);
                }
                mcSHOW_USER_MSG(("TOP value = 0x%x\n", pEuTunerCtx->m_aucPara[0]));
            }
        }
        break;
 
    case 5:
        if (x_strcmp((CHAR *) aszArgv[0], "monon") == 0)
        {
            u4AccTotPkt = 0;
            u4AccErrPkt = 0;
            u4AccCorBit = 0;
            u2AccEsr    = 0;
            u4AccPer = 0;
            fgPec = TRUE;
        }
        break;
    case 6:
         if (x_strcmp((CHAR *) aszArgv[0], "monoff") == 0) {
            fgPec = FALSE;
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "tspath") == 0) 
        {
            if (i4Argc > 1) 
            {
                if (StrToInt(aszArgv[1]) == 1)   // pass-through
                {
                    DVBC_SetTsPathMode(psDvbcDemodCtx, TRUE);
                    mcSHOW_USER_MSG(("Set TS path pass-through\n"));
                }
                else if ( StrToInt(aszArgv[1]) == 0)   // tri-state
                {
                    DVBC_SetTsPathMode(psDvbcDemodCtx, FALSE);
                    mcSHOW_USER_MSG(("Set TS path tri-state\n"));
                }
            }          
        }
        break;

    case 8:
        if (x_strcmp((CHAR *) aszArgv[0], "checkreg") == 0) 
        {
            mcSHOW_USER_MSG(("DVBC basic register settings check: \n"));

            mcSHOW_USER_MSG(("DTD PLL CFG (0x2000d134) = 0x%x\tDTD PLL setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x134), (mcRISC_Reg(CKGEN_BASE, 0x134) & 0x00001F70)));
            mcSHOW_USER_MSG(("DTD PLL CFG (0x2000d12c) = 0x%x\tDTD PLL setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x12c), (mcRISC_Reg(CKGEN_BASE, 0x12c) & 0x04000000)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d1d0) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x1d0)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d1d4) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x1d4)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d21c) = 0x%x\tDEMOD ADC setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x21c), (mcRISC_Reg(CKGEN_BASE, 0x21c) & 0x00030003)));
            mcSHOW_USER_MSG(("PINMUX CFG (0x2000d400) = 0x%x\tPINMUX setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x400), (mcRISC_Reg(CKGEN_BASE, 0x400) & 0x20000000)));
            DVBC_GetReg(psDvbcDemodCtx, 0x93e, ucData, 1);
            mcSHOW_USER_MSG(("DVBC MODE (0x2002b93e) = 0x%x\tDVBC MODE setting = 0x%x\n", ucData[0], (ucData[0] & 0x01)));
            DVBC_GetReg(psDvbcDemodCtx, 0x4a0, ucData, 4);
            mcSHOW_USER_MSG(("RISC WAIT TIME (0x2002b4a0) = 0x%x\n", (*((UINT32*)ucData))));

            DVBC_GetReg(psDvbcDemodCtx, 0x00, ucData, 1);
            mcSHOW_USER_MSG(("uP CTRL REG= 0x%02x\n", ucData[0]));

            DVBC_GetReg(psDvbcDemodCtx, 0x0D, ucData, 2);
            mcSHOW_USER_MSG(("uP TOGGLE= 0x%02x\n", ucData[0]));
            mcSHOW_USER_MSG(("uP RUN_TAG= 0x%02x\n", ucData[1]));

            DVBC_GetReg(psDvbcDemodCtx, 0x1A0, ucData, 2);
            mcSHOW_USER_MSG(("IFAGC Target Level Set= 0x%02x%02x\n", ucData[1], ucData[0]));

            DVBC_GetReg(psDvbcDemodCtx, 0x1D7, ucData, 2);
            mcSHOW_USER_MSG(("IFAGC Target Level Out= 0x%02x%02x\n", ucData[1], ucData[0]));
        }
        break;

    default:
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t s [1/2/3/4/5]: Set TunerID to EEPROM (1: ENV57S, 2: FT2101, 3: TDTWS810D,4: FH2607, 5: Query TunerType in EEPROM)\n"));
        mcSHOW_USER_MSG(("\t monon:    (period level) turn on monitor for Toolx info update\n"));
        mcSHOW_USER_MSG(("\t monoff:   turn off monitor for Toolx info update\n"));
        mcSHOW_USER_MSG(("\t pecon:    turn on  PEC\n"));
        mcSHOW_USER_MSG(("\t pecoff:   turn off PEC\n"));
        mcSHOW_USER_MSG(("\t sh:       set hierarchical (1: HP, 0: LP)\n"));
        mcSHOW_USER_MSG(("\t stop:     Set TOP\n"));
        mcSHOW_USER_MSG(("\t checkreg:  Basic register settings check"));
        mcSHOW_USER_MSG(("\t ar [0/1]: AR Coefficient [Off/On]\n"));
        mcSHOW_USER_MSG(("\t tspath [0/1]: TS path [tri-state/pass-through]\n"));

        mcSHOW_USER_MSG(("\r\n"));
        mcSHOW_USER_MSG(("\r\n"));
        break;
    }

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerCommand unlock pDVBCPdCtx->hHalLock\n")); 

}

/**********************************************************************/
void DVBC_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);

    // no implement
    
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
}

void *GetDigiDvbCTunerCtx(void)
{
    return psDvbcDemodCtx->psTunerCtx;
}

/**********************************************************************/
void  DVBC_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    if(pDVBCPdCtx->fgDisStatus == TRUE)
    {
        /* Has Disconnected, return */
        mcSHOW_DBG_MSG(("DVBC Has Disconnect!\n"));
        return;
    }
    mcSHOW_DBG_MSG4(("DVBC_TunerDisc pDVBCPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerDisc pDVBCPdCtx->hHalLock\n"));

/*    if (DVBC_StandbyMode(DEFAULT_I2C_ADDR))
        mcSHOW_USER_MSG(("MT5133 standby mode FAIL!!\n"));
    else
        mcSHOW_DRVAPI_MSG(("MT5133 enter standby mode\n"));
*/
    fgBreakMonitor = TRUE;
    DVBC_DisConnect(psDvbcDemodCtx);   

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerDisc unlock pDVBCPdCtx->hHalLock\n"));
    
    /* set disconnect status */
        pDVBCPdCtx->fgDisStatus = TRUE;
    //fgIsDisconnected = TRUE;
    
}


INT16 DVBC_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type, VOID *pAnazAttribute, SIZE_T* pzAnaAttributeLen)
{
    TUNER_DIG_ATTRIBUTE_T *pAttri;
    INT32 temp;
    //UINT16 u2Temp;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDvbcDemodCtx->psTunerCtx->specific_member.eu_ctx);

    mcSHOW_DBG_MSG4(("DVBC_TunerGetAttribute pDVBCPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerGetAttribute pDVBCPdCtx->hHalLock\n")); 

    switch (e_get_type)
    {
    case TUNER_GET_TYPE_BER:
        //DVBC_UpdateInfo(psDvbcDemodCtx);   // Ken, for context update, best mux, 081217
        // JC, 090202, if context not updated, update here, 090202
        if (!fg1stMonitorRun)
        {
            DVBC_UpdateInfo(psDvbcDemodCtx);
            mcSHOW_DBG_MSG(("DVBC_TunerGetAttribute: TUNER_GET_TYPE_BER not enough time to update\n"));
        }
        *pzAnaAttributeLen = sizeof(INT32);
        temp = DVBC_GetPostVBer(psDvbcDemodCtx);
        x_memcpy(pAnazAttribute, &temp, *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_DIG_ATTRIBUTE:
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pAnazAttribute;
        pAttri->ui4_lower_bound_freq = pEuTunerCtx->m_s4FreqBoundLower;
        pAttri->ui4_upper_bound_freq = pEuTunerCtx->m_s4FreqBoundUpper;
        pAttri->b_auto_qam_capability = TRUE;
        pAttri->b_auto_sr_capability = TRUE;
        /* 5135 DVBC step is 1MHz */
        pAttri->ui4_fine_tune_delta_freq = 500000;
        break;

    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        *pzAnaAttributeLen = sizeof(S8);
        x_memcpy(pAnazAttribute, &(pEuTunerCtx->m_SigLvScan), *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_BER_TOLERANCE:
        *pzAnaAttributeLen = sizeof(INT32);
        x_memcpy(pAnazAttribute, &psDvbcDemodCtx->i4BerTh, *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
        *pzAnaAttributeLen = sizeof(UINT8);
        x_memcpy(pAnazAttribute, &pEuTunerCtx->m_SigLvTh, *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAnaAttributeLen = sizeof(TS_FMT_T);
            tsFmt = DVBC_GetTsFmt(psDvbcDemodCtx);
            x_memcpy(pAnazAttribute, &tsFmt, *pzAnaAttributeLen);
        }
        break;
    case TUNER_GET_TYPE_SQI:
        if (!fg1stMonitorRun)
        {
            DVBC_UpdateInfo(psDvbcDemodCtx);
            mcSHOW_DBG_MSG(("DVBC_TunerGetAttribute: TUNER_GET_TYPE_SQI not enough time to update\n"));
        }
        *pzAnaAttributeLen = sizeof(INT32);
        temp = (INT32)DVBC_GetSQI(psDvbcDemodCtx);
        x_memcpy(pAnazAttribute, &temp, *pzAnaAttributeLen);
        break;

    default:
        {
            mcSHOW_DBG_MSG4(("DVBC_TunerGetAttribute: RMR_DRV_INV_GET_INFO!, type=%d\n", e_get_type));
            //return (RMR_DRV_INV_GET_INFO);
            break;
        }
    }

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerGetAttribute unlock pDVBCPdCtx->hHalLock\n")); 

    return (RMR_OK);
}


/***********************************************************************/
INT32 DVBC_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
    UCHAR ucData = *(UCHAR *)pvAnaAttribute;

    mcSHOW_DBG_MSG4(("DVBC_TunerSetAttribute pDVBCPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_TunerSetAttribute pDVBCPdCtx->hHalLock\n")); 


    switch (e_set_type)
    {
#ifdef fcTUNE_NOT_START
        case TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS:
            if (zAnaAttributeLen)
            {
                TUNER_ACQ_ACTION_T *peActionType = (TUNER_ACQ_ACTION_T *) pvAnaAttribute;

                mcSHOW_DBG_MSG(("%s: (%d, %X) eActionType=%d!\n", __FUNCTION__,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                                TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS, *peActionType));
                psDvbcDemodCtx->u1TunerAcqType = *peActionType;
            }
            break;
#endif

        case TUNER_SET_TYPE_TS_OUTPUT:
            if (ucData == BOOL_TRUE) {
                DVBC_SetTsOutput(psDvbcDemodCtx, TRUE);
            }
            else if (ucData == BOOL_FALSE) {
                DVBC_SetTsOutput(psDvbcDemodCtx, FALSE);
            }
            break;
            
#ifdef fcQINGDAO_NO_AUD_VID_DSCRM_PATCH
        case TUNER_SET_TYPE_NO_AUD_VID:
            mcSHOW_USER_MSG(("DVBC_TunerSetAttribute: MW set NO_AUD_AND_VID.\n"));
            fgNoAudVid = TRUE;
            break;
#endif

        default:
            mcSHOW_DBG_MSG(("DVBC_TunerSetAttribute: RMR_DRV_INV_SET_INFO!\n"));
//            return (RMR_DRV_INV_SET_INFO);
            break;
    }

    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_TunerSetAttribute unlock pDVBCPdCtx->hHalLock\n")); 

    return (RMR_OK);
}


/***********************************************************************/
void DVBC_SetDmxExt(void)
{
    
    mcSHOW_DBG_MSG4(("DVBC_SetDmxExt pDVBCPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBC_SetDmxExt pDVBCPdCtx->hHalLock\n")); 
        
    mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBC_SetDmxExt unlock pDVBCPdCtx->hHalLock\n")); 

}

/***********************************************************************/
void DVBC_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{   
#if defined(CC_MT5135DVBC)
    UCHAR ucValue = 0;
#endif
    
    //mcMUTEX_LOCK(pDVBCPdCtx->hHalLock);   // no lock due to may be deadlock with TunerAcq

#if defined(CC_MT5135DVBC)

    if (bSwitchOn == TRUE) 
    {
        /* ByPass */ 
        ucValue = 0x07;
        DVBC_SetReg(psDvbcDemodCtx, DVBC_REG_I2C_Deglitch, &ucValue , 1);
        ucValue = 0x50;
        DVBC_SetReg(psDvbcDemodCtx, DVBC_REG_I2C_Pullup_Del, &ucValue , 1);
        ucValue = 0x03;
        DVBC_SetReg(psDvbcDemodCtx, DVBC_REG_I2C_Cfg, &ucValue , 1);
    }
    else
    {
        /* No ByPass */
        ucValue = 0x02;
        DVBC_SetReg(psDvbcDemodCtx, DVBC_REG_I2C_Cfg, &ucValue , 1);
    }
    
#endif

    //mcMUTEX_UNLOCK(pDVBCPdCtx->hHalLock);
}


/**********************************************************************/
void PD_DVBC_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*     pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = DVBC_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = DVBC_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = DVBC_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = DVBC_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DVBC_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DVBC_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DVBC_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DVBC_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = DVBC_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = DVBC_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = DVBC_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DVBC_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DVBC_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = DVBC_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = DVBC_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand = DVBC_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C = DVBC_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = DVBC_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = DVBC_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = DVBC_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DVBC_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd = DVBC_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = DVBC_TunerGetDiagInfo;
#endif
}

