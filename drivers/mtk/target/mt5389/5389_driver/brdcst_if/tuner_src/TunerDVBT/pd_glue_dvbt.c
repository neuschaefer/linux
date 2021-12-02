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
*    JaShun Wan
* 
*************************************************************************/

#include "pi_dvbt_if.h"
#include "PD_Def_dbg.h"  // Note that fcADD_DIAG_INFO defined in PD_Def_dbg.h, This file should be placed in the first position.
#include "pi_def_dvbt.h"
#include "TDIS.h"
#include "pi_demod.h"
#include "pd_tuner_type.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "com_defs.h"

#include "fe_tuner_common_if.h"
#include "eeprom_if.h"
#include "pd_dvbt_if.h"
#include "x_stl_lib.h"

//#include "pd_tuner_type.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DVBT_MON_POLL_INTRVL             10  // 10ms
#define DVBT_LOCK_TIME_OUT              500  // 500ms
#define DVBT_LOCK_TIME_OUT_CNT         (DVBT_LOCK_TIME_OUT/DVBT_MON_POLL_INTRVL)
#if  fcTUNER_TYPE == cALPS_TDAG4B03A
#define DVBT_TUNER_CONN_TIME_THREHOLD   750                          //add 700ms , TDAG4B03A AGC speed too slow
#else
#define DVBT_TUNER_CONN_TIME_THREHOLD   140
#endif
#define DVBT_DEMOD_CONN_TIME_THREHOLD   300

#ifdef __MODEL_slt__   // iven temp mark

#define cRISC_DTD_BASE           (IO_VIRT + 0x2B000)

#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}\

#endif

DEMOD_CTX_T*    psDvbtDemodCtx;

DVBT_PD_CTX_T *pPdCtx;

static TUNER_DEVICE_CTX_T*    pDvbtTunerDeviceCtx = NULL;

static x_break_fct      MTDVBTisBreak;

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
//static BOOL     fgIsDisconnected = TRUE;
static BOOL     fgBreakMonitor = TRUE;
static BOOL     fg1stMonitorRun = FALSE;
#ifdef EU_AUTO_ATP
 UINT8 gATP_TEST_ITEM_BIT = 0 ;
 UINT8 gATPFailBitMsk =0;
#endif
extern UINT8  u1TStatus;

static void PD_DVBT_Register_LTDIS_Fct(PTD_CTX ptTDCtx);

//extern TUNER_COMMONAPI_FCT_TBL_T *pTuner_CommonAPI_Fct;
/*
#if (fcTUNER_TYPE != cDVBT_TUNER_ALL)
extern void Tuner_CommonAPI_Register_Fct(TUNER_COMMONAPI_FCT_TBL_T *pcom_Fct);
#else
extern void Tuner_CommonAPI_Register_Fct(TUNER_COMMONAPI_FCT_TBL_T *pcom_Fct, U8 tunerType);
#endif
*/
#define  DVBT_SYNC_PER_THRESHOLD  99000

/***********************************************************************/
#ifdef fcADD_DIAG_INFO
INT32 DVBT_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
    INT32   retSts = RMR_OK;

    mcSHOW_DBG_MSG4(("DVBT_TunerDisc pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerDisc pPdCtx->hHalLock\n"));

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

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerDisc unlock pPdCtx->hHalLock\n"));
    return retSts;
}

#define DRV_CUSTOM_TUNER_DIAG_ITEMS         16

INT32 DVBT_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
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


    mcSHOW_DBG_MSG4(("DVBT_TunerDisc pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerDisc pPdCtx->hHalLock\n"));

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
            //DVBT_GetInfo(psDvbtDemodCtx);
            //vDVBTGetStatus(psDvbtDemodCtx);
            
            // driver version
            sprintf(pDiagContent->cDisplayName, "%s", "Version");
            sprintf(pDiagContent->cDisplayValue, "%s", DVBT_GetSwVer());
            pDiagContent++;

            // BW
            sprintf(pDiagContent->cDisplayName, "%s", "BW");
            switch (DVBT_GetBW(psDvbtDemodCtx))
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
            switch (DVBT_GetFftMode(psDvbtDemodCtx))
            {
                case 0:  sprintf(szFft, "2k"); break;
                case 1:  sprintf(szFft, "8k"); break;
                case 2:  sprintf(szFft, "4k"); break;
                default: sprintf(szFft, "??"); break;
            }
            switch (DVBT_GetConst(psDvbtDemodCtx))
            {
                case 0:  sprintf(szQam, "QPSK"); break;
                case 1:  sprintf(szQam, "16Q" ); break;
                case 2:  sprintf(szQam, "64Q" ); break;
                default: sprintf(szQam, "??"  ); break;
            }            
            switch (DVBT_GetCR(psDvbtDemodCtx))
            {
                case 0:  sprintf(szCr, "1/2"); break;
                case 1:  sprintf(szCr, "2/3"); break;
                case 2:  sprintf(szCr, "3/4"); break;
                case 3:  sprintf(szCr, "5/6"); break;
                case 4:  sprintf(szCr, "7/8"); break;
                default: sprintf(szCr, "??" ); break;
            }
            switch (DVBT_GetGI(psDvbtDemodCtx))
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
            sprintf(pDiagContent->cDisplayValue, "%d dB", (DVBT_GetSignalSNR(psDvbtDemodCtx)>>3) );
            pDiagContent++;
            
            // PostVBER
            sprintf(pDiagContent->cDisplayName, "%s", "BER");
            sprintf(pDiagContent->cDisplayValue, "%d x10^-5", DVBT_GetPostVBer(psDvbtDemodCtx) );
            pDiagContent++;

            // TS PER
            sprintf(pDiagContent->cDisplayName, "%s", "PER");
            sprintf(pDiagContent->cDisplayValue, "%d x10^-5", DVBT_GetPER(psDvbtDemodCtx) );
            pDiagContent++;

            // TPS/TS lock status
            sprintf(pDiagContent->cDisplayName, "%s", "TPS / TS");
            sprintf(pDiagContent->cDisplayValue, "%d / %d", DVBT_GetTpsLock(psDvbtDemodCtx), DVBT_GetTsLock(psDvbtDemodCtx) );
            pDiagContent++;

            // Hierarchy
            sprintf(pDiagContent->cDisplayName, "%s", "Hierarchy");
            switch (DVBT_GetHier(psDvbtDemodCtx))
            {
                case 0:  sprintf(szAlpha, "");      break;
                case 1:  sprintf(szAlpha, ", a=1"); break;
                case 2:  sprintf(szAlpha, ", a=2"); break;
                case 3:  sprintf(szAlpha, ", a=4"); break;
                default: sprintf(szAlpha, "??"  ); break;
            }
            if (DVBT_GetHier(psDvbtDemodCtx) == 0)
            {
                sprintf(szHier,  "non-Hier");
            }
            else
            {
                if (DVBT_GetHpLp(psDvbtDemodCtx))
                    sprintf(szHier,  "LP");
                else
                    sprintf(szHier,  "HP");
            }
            sprintf(pDiagContent->cDisplayValue, "%s%s", szHier, szAlpha);
            pDiagContent++;

            // IF AGC
            sprintf(pDiagContent->cDisplayName, "%s", "IF AGC");
            sprintf(pDiagContent->cDisplayValue, "%d mV", ((DVBT_GetIfAgc(psDvbtDemodCtx)*3300>>8) + 1650) );
            pDiagContent++;
            
            // Freq offset
            sprintf(pDiagContent->cDisplayName, "%s", "Freq Offset");
            sprintf(pDiagContent->cDisplayValue, "%d kHz", DVBT_GetFreqOffset_kHz(psDvbtDemodCtx)>>8);
            pDiagContent++;

            // Time offset
            sprintf(pDiagContent->cDisplayName, "%s", "Time Offset");
            sprintf(pDiagContent->cDisplayValue, "%d ppm", DVBT_GetTimingOffset(psDvbtDemodCtx)/1000);
            pDiagContent++;

            // AR
            sprintf(pDiagContent->cDisplayName, "%s", "AR");
            DVBT_GetReg(psDvbtDemodCtx, 0x176, u1Data, 1);
            if (u1Data[0] == 0)
                sprintf(pDiagContent->cDisplayValue, "Off");
            else
                sprintf(pDiagContent->cDisplayValue, "On");
            pDiagContent++;
            
            //CCI
            sprintf(pDiagContent->cDisplayName, "%s", "CCI Cnt");
            DVBT_GetReg(psDvbtDemodCtx, 0x1EC, u1Data, 1);
            sprintf(pDiagContent->cDisplayValue, "%d", u1Data[0]);
            pDiagContent++;

            //INR: threshold on
            sprintf(pDiagContent->cDisplayName, "%s", "INR(Thrld On)");
            DVBT_GetReg(psDvbtDemodCtx, 0x1ED, u1Data, 3);
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
            DVBT_GetReg(psDvbtDemodCtx, 0x151, u1Data, 1);
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

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerDisc unlock pPdCtx->hHalLock\n"));
    
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
    SetFreqPara.Modulation = MOD_DVBT;
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

    mcSHOW_DBG_MSG(("MonitorProcess start\n"));
    while (fgMonitorThreadRunning)
    {
        if (fgBreakMonitor){
            // After mcMUTEX_LOCK(pPdCtx->t_escape_mon_suspend), monitor thread will be suspended until
            // TunerAcq or TunerClose invoked.
            mcSHOW_DBG_MSG(("Entering DVBT mon_suspend\n"));
            mcMUTEX_LOCK(pPdCtx->t_escape_mon_suspend);
            mcSHOW_DBG_MSG(("Escape DVBT mon_suspend\n"));
        }
        else{
            // mutex lock
            mcSHOW_DBG_MSG4(("MonitorProcess pPdCtx->hHalLock\n"));
            mcMUTEX_LOCK(pPdCtx->hHalLock);
            mcSHOW_DBG_MSG4(("escape MonitorProcess pPdCtx->hHalLock\n"));

            mcSHOW_DBG_MSG4(("Invoke DVBT_Monitor\n"));
            // ***** DVB-T Info Update ***** //
            if (!fg1stMonitorRun)
                mcDELAY_MS(100);
            DVBT_UpdateInfo(psDvbtDemodCtx);
            fg1stMonitorRun = TRUE;  // Ken, 090202, check Monitor Process actually execute, for best mux

            // ***** Qtest Info Update ***** //
            if ((fgPec)||(fgPec_dtd_y))
            {
                u4AccTotPkt += psDvbtDemodCtx->u2TotalPkt;
                u4AccErrPkt += psDvbtDemodCtx->u2ErrorPkt;
                u4AccCorBit += psDvbtDemodCtx->u4CorrBit;

                u4AccPostViterbiBER += DVBT_GetPostVBer(psDvbtDemodCtx);
                u4AccPostViterbiBER_Cnt = u4AccPostViterbiBER_Cnt + 1;

                if ((psDvbtDemodCtx->u2TotalPkt==0) || (psDvbtDemodCtx->u2ErrorPkt!=0))
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
                mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_INFO5;
                if (mcHOST_CMD(psDvbtDemodCtx, aucData, ccOFST_DEMOD_INFO5_SIZE, 1))
                {
                    ucAcqCnt = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO5_ACQ_COUNT);
                    ucRstCnt = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO5_RESET_COUNT);
                }
                                
                // get AR coef
                DVBT_GetReg(psDvbtDemodCtx, 0x176, &ucArCoef, 1);
#ifndef __MODEL_slt__
                // print info to Toolx
//              mcPRINTF("start.pec");
                if(fgPec) 
                {
                    mcPRINTF("start.log");

                    mcPRINTF("%02d%01d",  LOG_TAG_FFT,  psDvbtDemodCtx->u1Fft);
                    mcPRINTF("%02d%01d",  LOG_TAG_GI,   psDvbtDemodCtx->u1GI);
                    mcPRINTF("%02d%01d",  LOG_TAG_CR,   psDvbtDemodCtx->u1CR);
                    mcPRINTF("%02d%02d",  LOG_TAG_SNR,  DVBT_GetSignalSNR(psDvbtDemodCtx)/10);
    
                    mcPRINTF("%02d%06d",  LOG_TAG_PEC_PER,     u4AccPer);
                    mcPRINTF("%02d%05d",  LOG_TAG_PEC_ESR,     u2AccEsr);
                    mcPRINTF("%02d%010d", LOG_TAG_PEC_PKT_TOT, u4AccTotPkt);
                    mcPRINTF("%02d%010d", LOG_TAG_PEC_PKT_ERR, u4AccErrPkt);
                    mcPRINTF("%02d%010d", LOG_TAG_PEC_COR_BIT, u4AccCorBit);
                    mcPRINTF("%02d%03d",  LOG_TAG_ACQ_CNT,     ucAcqCnt);
                    mcPRINTF("%02d%03d",  LOG_TAG_RST_CNT,     ucRstCnt);
                    mcPRINTF("%02d%03d",  LOG_TAG_AR_COEF,     ucArCoef);

    //              mcPRINTF("stop.pec\r\n");
                    mcPRINTF("stop.log\r\n");
                 }
                else
                {
                    psDvbtDemodCtx->u2PecRefreshTime = psDvbtDemodCtx->u2PecRefreshTime - 1; 
                    psDvbtDemodCtx->u4PecObserveTime = psDvbtDemodCtx->u4PecObserveTime - 1;
                    
                    if(psDvbtDemodCtx->u2PecRefreshTime == 0)
                    {
                        psDvbtDemodCtx->u2PecRefreshTime = u2KeepRefreshTime;
//                        mcSHOW_USER_MSG(("\n************************\n"));
                        mcSHOW_USER_MSG(("SNR = %03d/10\t",  DVBT_GetSignalSNR(psDvbtDemodCtx)));
						mcSHOW_USER_MSG(("SQI = %03d\t",  DVBT_GetSQI(psDvbtDemodCtx)));
                        mcSHOW_USER_MSG(("Total_Pkt = %010d\t",  u4AccTotPkt));
                        mcSHOW_USER_MSG(("Error_Pkt = %010d\t",  u4AccErrPkt));
                        mcSHOW_USER_MSG(("Reset_Cnt = %03d\t",  ucRstCnt));
						mcSHOW_USER_MSG(("ReAcq_Cnt = %03d\t",  ucAcqCnt));
                        mcSHOW_USER_MSG(("Acc_Per = %06d\t", u4AccPer));
                        mcSHOW_USER_MSG(("Acc_PostVBER = %06dE-5\t", (u4AccPostViterbiBER/u4AccPostViterbiBER_Cnt)));
                        mcSHOW_USER_MSG(("PEC_Sec = %05d\n",  psDvbtDemodCtx->u4PecObserveTime));                        
//                        mcSHOW_USER_MSG(("\n************************\n"));
                    }

                    if(psDvbtDemodCtx->u4PecObserveTime == 0)
                    {
                        fgPec_dtd_y = FALSE;
                    }

                }
#else
                mcSHOW_USER_MSG(("\n************************\n"));
                mcSHOW_USER_MSG(("SNR = %03d/10\n",  DVBT_GetSignalSNR(psDvbtDemodCtx)));
				mcSHOW_USER_MSG(("Total_Pkt = %010d\n",  u4AccTotPkt));
                mcSHOW_USER_MSG(("Error_Pkt = %010d\n",  u4AccErrPkt));
                mcSHOW_USER_MSG(("Reset_Cnt = %03d\n",  ucRstCnt));
				mcSHOW_USER_MSG(("Acc_Per = %06d", u4AccPer));
                mcSHOW_USER_MSG(("\n************************\n"));
				//lei_code_dB110601,add for SLT building warning;
				mcSHOW_DBG_MSG4(("Acq_Cnt = %03d\n",  ucAcqCnt));
				mcSHOW_DBG_MSG4(("u2KeepRefreshTime = %03d\n",  u2KeepRefreshTime));
#endif
            }

            // mutex unlock
            //mcSHOW_DBG_MSG4(("MonitorProcess unlock pPdCtx->hHalLock\n"));
            mcMUTEX_UNLOCK(pPdCtx->hHalLock);
            mcSHOW_DBG_MSG4(("escape  MonitorProcess unlock pPdCtx->hHalLock\n"));

            // Sleep 
            mcDELAY_MS(u2ToolxInfoUpdateRate); 
            loop ++;
        }
    }
    
    t_monitor_thread = (THREAD_T)NULL;
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
            "DVBT_MonThread",
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
    mcSHOW_DBG_MSG(("DVBT PD_StopMonitorEngine success\n"));
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
#ifndef I2C_TUNER_ALL
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
        mcSHOW_USER_MSG(("--defaut I2C Addr = C2->Open tuner ENV57S------------\n"));
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
#endif
}
//this API leverage from 5112, it export to analog pd code to get tuner id from EEPROM
//Jackson todo: arch. need refine
//UINT8 u1GetTunerType(DEMOD_CTX_T *psDemodCtx, UINT8 u1TunerType, BOOL fgAnaTunerType)
UINT8 u1GetTunerType(VOID)
{
    //return checkTunerTypeFromEeprom();
    mcSHOW_DBG_MSG(("TunerType(0x%x) from digital PD\r\n", ucTunerType));
    return (ucTunerType);
}

/***********************************************************************/
char *DVBT_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
#if 0 
    mcSHOW_DBG_MSG4(("DVBT_TunerGetVer pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerGetVer pPdCtx->hHalLock\n"));
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerGetVer unlock pPdCtx->hHalLock\n"));
    return NULL;
#endif

    DVBT_PD_CTX_T *psDVBT_PDCtx = (DVBT_PD_CTX_T *) ptTDSpecificCtx;
    DEMOD_CTX_T *psDemodCtx = psDVBT_PDCtx->pDemodCtx;
    CHAR    *szRetValue;

    mcSEMA_LOCK_HAL(psDVBT_PDCtx->hHalLock);   // Avoid re-entry

    szRetValue = DVBT_GetVer(psDemodCtx);

    mcSEMA_UNLOCK_HAL(psDVBT_PDCtx->hHalLock);
    return (szRetValue);    
    
}

/***********************************************************************/
void DVBT_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, 
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSHOW_DBG_MSG4(("DVBT_TunerSetRegSetting pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerSetRegSetting pPdCtx->hHalLock\n"));

    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerSetRegSetting unlock pPdCtx->hHalLock\n"));

}

void DVBT_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG4(("DVBT_TunerShowRegSetting pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerShowRegSetting pPdCtx->hHalLock\n"));

    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerShowRegSetting unlock pPdCtx->hHalLock\n"));

    
}

void DVBT_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG4(("DVBT_TunerTestI2C pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerTestI2C pPdCtx->hHalLock\n"));
    
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerTestI2C unlock pPdCtx->hHalLock\n"));    
}

/**********************************************************************/

/**********************************************************************/
S32 DVBT_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    S32 i4Ret = 0;
    mcSHOW_DBG_MSG4(("DVBT_TunerNimTest pPdCtx->hHalLock\n"));    
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerNimTest pPdCtx->hHalLock\n"));    

    //add your implementation here
    #ifdef CC_CAB_DEMOD_TUNER_BYPASS
    DigTunerBypassI2C(TRUE);
    #endif

    i4Ret = Tuner_Connect(666000, MT5391_BW_8MHz, FALSE);

    #ifdef CC_CAB_DEMOD_TUNER_BYPASS
    DigTunerBypassI2C(FALSE);
    #endif

    mcSHOW_DRVERR_MSG(("DVBT_TunerNimTest : %d (0:OK, 1:fail)\n", i4Ret));
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerNimTest unlock pPdCtx->hHalLock\n"));  


    return i4Ret;
}

/**********************************************************************/
DVBT_PD_CTX_T *DVBT_GetPdCtx(void)
{
    return pPdCtx;
}

/**********************************************************************/
S32 DVBT_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, 
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
    //UCHAR     test = 0;
    //UINT32 i;
if(u1TStatus==1)
{
   return (DRVAPI_TUNER_ERROR);
}
#if (fcTUNER_TYPE == cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST) || (fcTUNER_TYPE == cTTE_I2CALLLST)||(fcTUNER_TYPE == cTPV2K11EU_TUNERLST)
    UINT8 tunerType;
#endif

#ifdef __MODEL_slt__  // iven temp mark
UINT32 u4RegValue;
#endif

    pDvbtTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    /*create DVBT PD CTX*/
    pPdCtx = (DVBT_PD_CTX_T *) x_mem_calloc(1, sizeof(DVBT_PD_CTX_T));
    if (pPdCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }

    /* Init Disconnect Status */
    pPdCtx->fgDisStatus = FALSE;
    *pptTDSpecificCtx = pPdCtx;

    if (mcSEMA_CREATE(&pPdCtx->hHalLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("DVBT_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);


    // Register to TDMgr
    PD_DVBT_Register_LTDIS_Fct(ptTDCtx);
#ifdef __MODEL_slt__  // iven temp mark
    #if defined(CC_MT5396)

    mcSHOW_DRVERR_MSG(("Reset Demod\n"));

    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFFFFCFFF) | 0x00000000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;    



    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFFFFCFFF) | 0x00003000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;        

    
    #elif defined(CC_MT5368)

    mcSHOW_DRVERR_MSG(("Reset Demod\n"));
    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFF9FFFFF) | 0x00000000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;


    u4RegValue = mcRISC_Reg(0xf0000000, 0xd270);
    u4RegValue = (u4RegValue & 0xFF9FFFFF) | 0x00600000;
    mcRISC_Reg(0xf0000000, 0xd270) = u4RegValue;    

    

    
    #endif
#endif

    psDvbtDemodCtx = DVBT_DemodCtxCreate();
    if (psDvbtDemodCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    psDvbtDemodCtx->psTunerCtx = ITunerGetCtx();

    /*link demod ctx to pd ctx*/
    pPdCtx->pDemodCtx = psDvbtDemodCtx;

#if defined(CC_MT5135)
    SPIBus_Init();
#endif

    // do tuner init before demod init, for demod init need tuner parameters
 #if (fcTUNER_TYPE != cDVBT_TUNER_ALL) &&(fcTUNER_TYPE != cDVBT_2K10_TUNERLST) && (fcTUNER_TYPE != cTTE_I2CALLLST)&& (fcTUNER_TYPE != cTPV2K11EU_TUNERLST)
    ITunerRegistration(cITUNER_DONT_CARE);
 #else
    tunerType = checkTunerTypeFromEeprom();
    ITunerRegistration(tunerType);
    mcSHOW_USER_MSG(("Register DVBT Tunerall type = %d\n",tunerType));
 #endif
    ITuner_Init(ITunerGetCtx());

    // do demod init
    if (DVBT_Initialize(psDvbtDemodCtx, DEFAULT_I2C_ADDR, TRUE)){
        mcSHOW_USER_MSG(("DVBT_Initialize failed\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    // Ken, Set TS to Tri-state for CI CAM compatibility after Power on 
    DVBT_SetTsPathMode(psDvbtDemodCtx, FALSE);

    mcSHOW_DBG_MSG(("DVBT Disconnect @ TunerOpen!!\n"));
    DVBT_DisConnect(psDvbtDemodCtx);   // add by Ken, default disconnect demod for TS_CLK EMI issue, 090114

//#if !defined(CC_MTAL) && !defined(CC_SDAL)
    if(pDvbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        if (mcSEMA_CREATE(&pPdCtx->t_escape_mon_suspend, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            mcSHOW_DRVERR_MSG(("DVBT_TunerOpen (mcSEMA_CREATE): ERROR!\n"));
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
S32 DVBT_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
    if(pDvbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        fgBreakMonitor = FALSE;
        fgMonitorThreadRunning = FALSE;  //must set to FALSE before unlock sema t_escape_mon_suspend
        mcMUTEX_UNLOCK(pPdCtx->t_escape_mon_suspend);
        PD_StopMonitorEngine();
        mcMUTEX_DESTROY(pPdCtx->t_escape_mon_suspend);
    }
//#endif
    DVBT_DemodCtxDestroy(psDvbtDemodCtx);


    mcMUTEX_DESTROY(pPdCtx->hHalLock);
    
    return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
S16 DVBT_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        DRV_CONN_TYPE_T e_conn_type)
{
    S16 s2_ret = FALSE;
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    
    // report tuner unsync if TS unlocked or high PER 
    vDVBTGetStatus(psDvbtDemodCtx);
    if (DVBT_GetTsLock(psDvbtDemodCtx) == TRUE)
    {
    // in SDAL, TunerGetSync() is used in ch scan => don't use PER to drop ch
    // and also PER is not updated in SDAL
    //#ifndef CC_SDAL
        if (pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
            if (DVBT_GetPER(psDvbtDemodCtx) < DVBT_SYNC_PER_THRESHOLD)
    //#endif
            s2_ret = TRUE;
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    return s2_ret;
}

void DVBT_SetTSInterface(BOOL fgParallel)
{
    mcSHOW_DBG_MSG4((" DVBT_SetTSInterface pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" escape DVBT_SetTSInterface pPdCtx->hHalLock\n"));

    DVBT_SetTsOutputMode(psDvbtDemodCtx, fgParallel);
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DVBT_SetTSInterface unlock pPdCtx->hHalLock\n"));
}

UINT8 DVBT_SetTSPath(BOOL fgEnable)
{
    UINT8 u1Ret = 0;

    mcSHOW_DBG_MSG4((" DVBT_SetTSPath pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" escape DVBT_SetTSPath pPdCtx->hHalLock\n"));

    u1Ret = DVBT_SetTsPathMode(psDvbtDemodCtx, fgEnable);
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DVBT_SetTSPath unlock pPdCtx->hHalLock\n"));

    return u1Ret;
}

BOOL DVBT_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type, 
                    void* pv_conn_info, 
                    SIZE_T z_conn_info_len, 
                    x_break_fct _BreakFct,
                    void* pvArg)
{

    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;
    U32     freq;
    UINT8   bw;
    U16     ii = 0;
    UINT8   fgChannelScan = FALSE;
    BOOL    fgRetSts = FALSE;
    BOOL    fgConnSts = TRUE;
    UINT8   u1DvbtDemodState;
    UCHAR   ucData = 0x3C;   // for best mux
    UINT32  u4TickCnt, u4ConnTime;

    mcSHOW_DBG_MSG4((" DVBT_TunerAcq pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" escape DVBT_TunerAcq pPdCtx->hHalLock\n"));

    MTDVBTisBreak = _BreakFct;

    u4TickCnt = os_get_sys_tick(); 

    // 090724, Power up AD in the beginning, Menghau
    DVBT_ChipInit(psDvbtDemodCtx);
#if defined(CC_MT5135) || defined(CC_MT5396)|| defined(CC_MT5368) //EC_code_dB330: For MT5396DVBTC demod
    DVBT_FwSwitch(psDvbtDemodCtx);/* For ReMap Use */
#endif

    // 090326, Split DVBT_Connect into SetNormalMode and SetBw for IFAGC startup in advance, JC
#ifdef fcTUNE_NOT_START
    if (psDvbtDemodCtx->u1TunerAcqType != TUNE_FREQ_ONLY)   // Do not set Demod when do TUNE_FREQ_ONLY 
#endif
    {
        DVBT_DriverStop(psDvbtDemodCtx);

        //mcDELAY_MS(10);   

        if (DVBT_SetNormalMode(psDvbtDemodCtx)){
            mcSHOW_USER_MSG(("Failed on DVBT_SetNormalMode()\n"));
            fgConnSts = FALSE;
        }
    }

    if(pt_tuner_info->e_mod == MOD_UNKNOWN){  //channel scan, LC 080327
        ucData = 0;
        fgChannelScan = TRUE;
    }
    // force to turn off AR when channel scan for BestMux selection
    //fgSetVars(pMt5133Ctx, 0x507, &ucData, 1);
    DVBT_SetArCoef(psDvbtDemodCtx, ucData);


    fgBreakMonitor = TRUE;  //if monitor thread is running, pause it.


    freq = pt_tuner_info->ui4_freq/1000;

    switch(pt_tuner_info->e_bandwidth) 
    {
    case BW_6_MHz:
        mcSHOW_DBG_MSG(("Set BW = 6 MHz\n"));
        bw = MT5391_BW_6MHz;
    break;
    case BW_7_MHz:
        mcSHOW_DBG_MSG(("Set BW = 7 MHz\n"));
        bw = MT5391_BW_7MHz;
        break;
    case BW_8_MHz:
    default:
        mcSHOW_DBG_MSG(("Set BW = 8 MHz\n"));
        bw = MT5391_BW_8MHz;
        break;
    }


#ifdef fcTUNE_NOT_START
    if (psDvbtDemodCtx->u1TunerAcqType != ACQ_DEMOD_ONLY)   // Do not SetFreq when do demod-only acq
#endif
    {              
        mcSHOW_DBG_MSG(("_Freq1 = %d kHz, set hier = %d, \n", freq, pt_tuner_info->e_hierarchy_priority));  
        
        DigTunerBypassI2C(TRUE);

        if (Tuner_Connect(freq, bw, fgChannelScan)){
            mcSHOW_DBG_MSG(("Failed on Tuner_SetFreq(%d) \n", freq));
            fgConnSts = FALSE;
        }

        //if (ITuner_OP(ITunerGetCtx(), itSetIF, NULL, NULL)== ITUNER_OK)
        //{
        //    DVBT_SetIF(psDvbtDemodCtx);
        //}

        DigTunerBypassI2C(FALSE);

        u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
        mcSHOW_DBG_MSG(("DVBT tuner connection time = %dms\n", u4ConnTime));
#ifdef  EU_AUTO_ATP
        if ((u4ConnTime > DVBT_TUNER_CONN_TIME_THREHOLD) && (fgChannelScan)){
            mcSHOW_DBG_MSG(("EU Driver ATP Fail: DVBT tuner connection time = %dms, too long!!\n", u4ConnTime));
        }
//#ifdef  EU_AUTO_ATP
               mcSHOW_DRVAPI_MSG(("ATPFailBitMskBefore =%d\n",gATPFailBitMsk));
         gATPFailBitMsk |=0X04 ;// DVBT ATP Fail !
         mcSHOW_DRVAPI_MSG(("ATPFailBitMskAfter =%x\n",gATPFailBitMsk));
#endif  
    }


#ifdef fcTUNE_NOT_START
    if (psDvbtDemodCtx->u1TunerAcqType != TUNE_FREQ_ONLY)   // Do not set Demod when do TUNE_FREQ_ONLY 
#endif
    {
        u4TickCnt = os_get_sys_tick(); 
        //if (DVBT_Connect(psDvbtDemodCtx, bw)){
        //    mcSHOW_USER_MSG(("Failed on Tuner_Connect(%d)", bw));
        //    fgConnSts = FALSE;
        //}
        //DVBT_DriverStop(psDvbtDemodCtx);   
        
        DVBT_SetIfAgcCurve(psDvbtDemodCtx);
		DVBT_SetIF(psDvbtDemodCtx);
		
        // set BW
        DVBT_SetBW(psDvbtDemodCtx, bw);

        // set hier
        //if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
		if (pt_tuner_info->e_hierarchy_priority == HIR_PRIORITY_LOW)
            DVBT_SetHpLp(psDvbtDemodCtx, EN_LP_DECODE); // set to LP 
        else
            DVBT_SetHpLp(psDvbtDemodCtx, EN_HP_DECODE); // set to HP

        DVBT_SetTsOutput(psDvbtDemodCtx, FALSE);
        // driver start
        DVBT_DriverStart(psDvbtDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);

        if (fgConnSts) {
            // polling lock loop
            for (ii = 0; ii < DVBT_LOCK_TIME_OUT_CNT; ii++)
            {   // get demod sync state
                u1DvbtDemodState = DVBT_GetSync(psDvbtDemodCtx);
                if (u1DvbtDemodState == DVBT_DEMOD_STATE_UNLOCK) {
                    mcSHOW_DBG_MSG(("TPS unlock! DVB-T not exist!\n"));
                    fgRetSts = FALSE;
                    break;
                }
                else if (u1DvbtDemodState == DVBT_DEMOD_STATE_LOCK) {
                    fgRetSts = TRUE;
                    break;
                }
                //break function
                if (MTDVBTisBreak(pvArg)){
                    mcSHOW_USER_MSG(("break DVBT_TunerAcq()!"));
                    fgRetSts = FALSE;
                    break;
                }
                // delay for a while
                mcDELAY_MS(DVBT_MON_POLL_INTRVL);
            }   
            // if time-out
            if(ii >= DVBT_LOCK_TIME_OUT_CNT)
            {
                mcSHOW_DBG_MSG(("TPS unlock! time-out\n"));
            }
            else // check Demod lock time only in non-time-out case
            {
                u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
                mcSHOW_DBG_MSG(("DVBT Demod connection time = %dms\n", u4ConnTime));
     #ifdef  EU_AUTO_ATP           
                if ((u4ConnTime > DVBT_DEMOD_CONN_TIME_THREHOLD) && (fgChannelScan)){
                    mcSHOW_DBG_MSG(("EU Driver ATP Fail: DVBT demod connection time = %dms, too long!!\n", u4ConnTime));
                }
	 #endif
            }
        }
    }

    DVBT_UpdateInfo(psDvbtDemodCtx);   // Ken, for context update, best mux, 081217

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DVBT_TunerAcq unlock pPdCtx->hHalLock\n"));
    

    if (fgRetSts) { // print info after update
        mcSHOW_DBG_MSG(("TPS lock, freq = %d kHz, lock time = %d ms, SNR = %d/10 dB, Hier = %d (0: unknown, 1: none, 2:hp, 3:lp)\n", 
           freq, ii*DVBT_MON_POLL_INTRVL, DVBT_GetSignalSNR(psDvbtDemodCtx), DVBT_GetHier(psDvbtDemodCtx)));

        DVBT_SetTsOutput(psDvbtDemodCtx, TRUE);
    }

    fg1stMonitorRun = FALSE;   // Ken, 090202, check Monitor Process actually execute, for best mux
    
    //#if !defined(CC_MTAL) && !defined(CC_SDAL)
    if(pDvbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
    {
        //++ Escape monitor suspend
        fgBreakMonitor = FALSE;
        mcMUTEX_UNLOCK(pPdCtx->t_escape_mon_suspend);
    }
    //#endif

    /* Init Disconnect Status */
    pPdCtx->fgDisStatus = FALSE;

    return fgRetSts;

}

/**********************************************************************/
void DVBT_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                    DRV_CONN_TYPE_T e_conn_type, 
                    SIGNAL *_pSignal)
{
    //UINT8 ret;

    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerGetSignal pPdCtx->hHalLock\n"));                

    _pSignal->Frequency  = (S32)(psDvbtDemodCtx->psTunerCtx->u4RF_Freq - (DVBT_GetFreqOffset_kHz(psDvbtDemodCtx)>>8));
    // removed, tuner HAL already did the conversion
    //_pSignal->Frequency *= 1000; // convert from kHz to Hz 
    _pSignal->SymbolRate = 0;
    _pSignal->QAMSize = 0;

    vDVBTGetPara(psDvbtDemodCtx); // update Hier, Hp/Lp, BW

    switch (DVBT_GetBW(psDvbtDemodCtx))
    {
        case 0:  _pSignal->e_bandwidth = BW_6_MHz;   break;
        case 1:  _pSignal->e_bandwidth = BW_7_MHz;   break;
        case 2:  _pSignal->e_bandwidth = BW_8_MHz;   break;
        case 3:
        default: _pSignal->e_bandwidth = BW_UNKNOWN; break;
    }

    _pSignal->e_hierarchy_priority = DVBT_GetHier(psDvbtDemodCtx);
    _pSignal->sHierInfo.eAlpha = (ALPHA_INFO_T)DVBT_GetHierMode(psDvbtDemodCtx);
    _pSignal->sHierInfo.eInDepth = (INTERLEAVE_INFO_T)DVBT_GetIndepth(psDvbtDemodCtx);
    mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: PRIORITY = %d (1:None, 2:HP, 3:LP)\n", _pSignal->e_hierarchy_priority));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Alpha = %d (3: a=4)\n", _pSignal->sHierInfo.eAlpha));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Indepth = %d \n", _pSignal->sHierInfo.eInDepth));

    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal unlock pPdCtx->hHalLock\n"));                    
}

/**********************************************************************/
U8   DVBT_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                DRV_CONN_TYPE_T e_conn_type)
{
    U8 u1_ret;
    INT32 s4Temp = 0;
    INT16 s2Temp;

    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalLevel pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerGetSignalLevel pPdCtx->hHalLock\n"));                
    
    s2Temp = ITuner_OP( ITunerGetCtx(), itGetSSIIndex,0, &s4Temp);
    if (s2Temp == ITUNER_OK)   // If SSI is ready, use it for signal level
    {
        u1_ret = (U8) s4Temp;
        mcSHOW_DBG_MSG(("DVBT_TunerGetSignalLevel (SSI): %d\n", s4Temp));
    }
    else
    {
        //vDVBTGetStatus(psDvbtDemodCtx);   // update in MonitorProcess, Ken, 20100608
        u1_ret = DVBT_GetSignalLevel(psDvbtDemodCtx);
        mcSHOW_DBG_MSG(("DVBT_TunerGetSignalLevel (IFAGC): %d\n", u1_ret));
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalLevel unlock pPdCtx->hHalLock\n"));
    return u1_ret;
}

/**********************************************************************/
S16 DVBT_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)
{


    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalLeveldBm pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerGetSignalLeveldBm pPdCtx->hHalLock\n"));                



    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalLeveldBm unlock pPdCtx->hHalLock\n"));
    return 0;
}

/**********************************************************************/
UINT16  DVBT_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)
{
    
    UINT8 SNR;
    
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalSNR pPdCtx->hHalLock\n"));                   
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerGetSignalSNR pPdCtx->hHalLock\n"));                    
    
    SNR = DVBT_GetSnrInfo(psDvbtDemodCtx);
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalSNR unlock pPdCtx->hHalLock\n"));

//#ifdef CC_SDAL
    if(pDvbtTunerDeviceCtx->u1MWIntf == cSDAL_IF)
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

UINT16  DVBT_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)  // return value = PER*5*10^4
{
    
    //INT32 u4_ret;
#ifdef __MODEL_slt__
    ////mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalPER pPdCtx->hHalLock\n"));                
    ////mcMUTEX_LOCK(pPdCtx->hHalLock);

    ////mcSHOW_DBG_MSG4(("escape DVBT_TunerGetSignalPER pPdCtx->hHalLock\n"));     
    ////mcMUTEX_UNLOCK(pPdCtx->hHalLock);

    if (u4AccPer >= 50000)
    {
        return 50000;
    }
    else
    {
        return (UINT16) (u4AccPer);
    }   
#else
    INT32 u4_ret;//lei_code_dB110601,for slt build warning
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalPER pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerGetSignalPER pPdCtx->hHalLock\n"));                

    // Justin, move to update update Demod context to thread
    //DVBT_GetInfo(psDvbtDemodCtx);
    u4_ret = DVBT_GetPER(psDvbtDemodCtx);    

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignalPER unlock pPdCtx->hHalLock\n")); 
    
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
void DVBT_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                            MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSHOW_DBG_MSG4(("DVBT_TunerSetMpgFmt pPdCtx->hHalLock\n"));                    
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerSetMpgFmt pPdCtx->hHalLock\n"));                        

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerSetMpgFmt unlock pPdCtx->hHalLock\n")); 

}

void DVBT_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucCmdId = 0, ucPar, ucData[256];
    U16     ii, jj, kk, u2RegAddr;
    UINT8   i4CurTop = 0;
    UINT8   ucData1;
    UINT8   temp[2];
    UINT32  temp4;
    UINT8   RegistWord[5]; 
	PARAM_SETFREQ_T SetFreqPara;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDvbtDemodCtx->psTunerCtx->specific_member.eu_ctx);
    
    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
        mcSHOW_USER_MSG(("\t e [Register (UINT8)]: Set or Get 5 Register,The CLI input formats are: nim.atd e or nim.atd e ##-##-##-##-##\n"));
        mcSHOW_USER_MSG(("\t t                    [Top]: SetTOP\n"));
        mcSHOW_USER_MSG(("\t d:                   Dump DVBT registers\n"));
        mcSHOW_USER_MSG(("\t s:                   show signal/ts/tps status\n"));
//        mcSHOW_USER_MSG(("\t n  Normal/Standby mode switch (0: Normal; 1: Standby)\n"));
        mcSHOW_USER_MSG(("\t r [RegAddr] [Num]:   read  Demod register\n"));
        mcSHOW_USER_MSG(("\t w [RegAddr] [Value]: write Demod register\n"));        
        mcSHOW_USER_MSG(("\t v [1/0]:             Driver on/off\n"));        
        //mcSHOW_USER_MSG(("\t t [TopValue]:        Set DTV TOP \n"));
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
        if(pDvbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
        {
            mcSHOW_USER_MSG(("\t mp:                  Pause monitor thread\n"));
            mcSHOW_USER_MSG(("\t mr:                  Resume monitor thread\n"));
        }
//#endif
    }

    mcSHOW_DBG_MSG4(("DVBT_TunerDtdCommand pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerDtdCommand pPdCtx->hHalLock\n"));


    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);

    switch (ucCmdId)
    {
        case 's':
        // no monitor thread in MTAL/SDAL, do info update in nim.dtd s
        //#if (defined(CC_MTAL) || defined(CC_SDAL))
            if (pDvbtTunerDeviceCtx->u1MWIntf == cMTAL_IF || pDvbtTunerDeviceCtx->u1MWIntf == cSDAL_IF)
            {
                DVBT_UpdateInfo(psDvbtDemodCtx);
            }
        //#endif

            mcSHOW_USER_MSG(("=================================\n"));
            mcSHOW_USER_MSG(("Digital Frequency lower bound=%d Hz\n", pEuTunerCtx->m_s4FreqBoundLower));
            mcSHOW_USER_MSG(("Digital Frequency upper bound=%d Hz\n", pEuTunerCtx->m_s4FreqBoundUpper));
            mcSHOW_USER_MSG(("Analog Frequency lower bound=%d Hz\n", pEuTunerCtx->m_s4AnaFreqBoundLower));
            mcSHOW_USER_MSG(("Analog Frequency upper bound=%d Hz\n", pEuTunerCtx->m_s4AnaFreqBoundUpper));

            switch (psDvbtDemodCtx->u1Mod)
            {
                case 0: mcSHOW_USER_MSG(("QamMode = QPSK\n"));     break;
                case 1: mcSHOW_USER_MSG(("QamMode = 16QAM\n"));    break;
                case 2: mcSHOW_USER_MSG(("QamMode = 64QAM\n"));    break;
                default: mcSHOW_USER_MSG(("QamMode = N/A QAM\n")); break;
            }
            switch (psDvbtDemodCtx->u1Fft)
            {
                case 0: mcSHOW_USER_MSG(("FFT = 2k\n"));   break;
                case 1: mcSHOW_USER_MSG(("FFT = 8k\n"));   break;
                case 2: mcSHOW_USER_MSG(("FFT = 4k\n"));   break;
                default: mcSHOW_USER_MSG(("FFT = N/A\n")); break;
            }
            switch (psDvbtDemodCtx->u1GI)
            {
                case 0: mcSHOW_USER_MSG(("GI = 1/32\n")); break;
                case 1: mcSHOW_USER_MSG(("GI = 1/16\n")); break;
                case 2: mcSHOW_USER_MSG(("GI = 1/8\n"));  break;
                case 3: mcSHOW_USER_MSG(("GI = 1/4\n"));  break;
                default: mcSHOW_USER_MSG(("GI = N/A\n")); break;
            }
            switch (psDvbtDemodCtx->u1CR)
            {
                case 0: mcSHOW_USER_MSG(("CR = 1/2\n"));  break;
                case 1: mcSHOW_USER_MSG(("CR = 2/3\n"));  break;
                case 2: mcSHOW_USER_MSG(("CR = 3/4\n"));  break;
                case 3: mcSHOW_USER_MSG(("CR = 5/6\n"));  break;
                case 4: mcSHOW_USER_MSG(("CR = 7/8\n"));  break;
                default: mcSHOW_USER_MSG(("CR = N/A\n")); break;
            }
            switch (psDvbtDemodCtx->u1Hier)
            {
                case 0: mcSHOW_USER_MSG(("Alpha = None\n")); break;
                case 1: mcSHOW_USER_MSG(("Alpha = a=1\n"));  break;
                case 2: mcSHOW_USER_MSG(("Alpha = a=2\n"));  break;
                case 3: mcSHOW_USER_MSG(("Alpha = a=4\n"));  break;
                default: mcSHOW_USER_MSG(("Alpha = N/A\n")); break;
            }

            mcSHOW_USER_MSG(("CFO = %dkHz\n", psDvbtDemodCtx->s2Ifo));
			mcSHOW_USER_MSG(("Freq_Offset = %dkHz\n", DVBT_GetFreqOffset_kHz(psDvbtDemodCtx)/256));
            mcSHOW_USER_MSG(("Strength = %3d\n", DVBT_GetSignalLevel(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("SNR = %d/10 dB\n", DVBT_GetSignalSNR(psDvbtDemodCtx) ));
            mcSHOW_USER_MSG(("=================================\n"));         
            mcSHOW_USER_MSG(("TpsLock = %x\n", DVBT_GetTpsLock(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("TsLock = %x\n", DVBT_GetTsLock(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("=================================\n"));
//            mcSHOW_USER_MSG(("ErrorPkt = %d\n", DVBT_GetErrorPkt(psDvbtDemodCtx)));
//            mcSHOW_USER_MSG(("PER = %d\n", DVBT_GetPER(psDvbtDemodCtx)));
//            mcSHOW_USER_MSG(("TotalPkt = %d\n", DVBT_GetTotalPkt(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("PostV BER = %d * 10^-5\n", DVBT_GetPostVBer(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("PER = %d * 10^-5\n", DVBT_GetPER(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("=================================\n"));
        #if defined(CC_MT5135) || defined(CC_MT5396)|| defined(CC_MT5368) //EC_code_dB330: For MT5396DVBTC demod
            mcSHOW_USER_MSG(("SLD RFAGC CW = 0.%d\n", psDvbtDemodCtx->s2SldPraRfAgc/41));
            mcSHOW_USER_MSG(("SLD IFAGC CW = 0.%d\n", psDvbtDemodCtx->s2SldPraIfAgc/41));
            mcSHOW_USER_MSG(("SLD PRA Gain Idx = 0x%X\n", psDvbtDemodCtx->ucSldPraGainIdx));
            mcSHOW_USER_MSG(("SLD Target LV = %d\n", psDvbtDemodCtx->s2SldPraOut));
        #endif
            mcSHOW_USER_MSG(("SQI = %d\n", DVBT_GetSQI(psDvbtDemodCtx)));
            ITuner_OP( ITunerGetCtx(), itGetSSIIndex,0, &temp4);            
            mcSHOW_USER_MSG(("SSI = %d\n", temp4));
            mcSHOW_USER_MSG(("=================================\n"));
#ifdef EU_AUTO_ATP
        if (gATP_TEST_ITEM_BIT & 0x01)
        {
            mcSHOW_DBG_MSG(("##Start\n##Strength=%d\n##BER = %d\n##PER = %d\n##TPSLOCK = %d\n##TSLOCK = %d\n##End\n",
            DVBT_GetSignalLevel(psDvbtDemodCtx),
            DVBT_GetPostVBer(psDvbtDemodCtx),
            DVBT_GetPER(psDvbtDemodCtx),
            DVBT_GetTpsLock(psDvbtDemodCtx),
            DVBT_GetTsLock(psDvbtDemodCtx)));
        }
        if (gATP_TEST_ITEM_BIT & 0x02)
        {
            if(DVBT_GetTsLock(psDvbtDemodCtx))
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
/*        
        case 'n':
           if (i4Argc > 1)
            {
                ucPar = StrToInt((CHAR *) aszArgv[1]);
                switch (ucPar){
                    case 0:
                        DVBT_NormalMode(DEFAULT_I2C_ADDR);
                        mcSHOW_USER_MSG(("MT5133 Normal Mode!\n"));
                        break;
                    case 1:
                        DVBT_StandbyMode(DEFAULT_I2C_ADDR);
                        mcSHOW_USER_MSG(("MT5133 Standby Mode!\n"));
                        break;
                    default:
                        break;
                }
            }
         
            break;
*/        
        case 'd':        
            for (kk=0; kk<3; kk++)
            {
                if(DVBT_DumpRegisters(psDvbtDemodCtx, ucData, kk, 8))
                {
                    mcSHOW_USER_MSG(("MTDVBT Register Page %d Dump Fail!\n", kk));
                }
                else
                {
                    mcSHOW_USER_MSG(("MTDVBT Register Page %d Dump Result:\n", kk));
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

                    //ITuner_OP(psDvbtDemodCtx->psTunerCtx, itSetReg,0, &RegistWord); 
                    if ((jj <= 2) &&( kk <= 5))
                    {
                        ITuner_OP( ITunerGetCtx(), itSetReg,0, &RegistWord); 
                        mcSHOW_USER_MSG(("Set Register complete!\n"));
                    }
                }
                else if (i4Argc == 1)
                {     
                    for (ii = 0; ii < 5; ii++)
                        RegistWord[ii] = 0;
                    //ITuner_OP(psDvbtDemodCtx->psTunerCtx, itGetReg,0, RegistWord); 
                    ITuner_OP(ITunerGetCtx(), itGetReg,0, RegistWord); 
                    for (ii = 0; ii < 5; ii++)
                    {
                        mcSHOW_USER_MSG(("the output word is 0x%x\n",RegistWord[ii]));  
                    }
                }
                else
                    mcSHOW_USER_MSG(("The CLI input formats are: nim.atd e or nim.atd e ##-##-##-##-## \n"));
            }
            break;

        case 't': 
            if (i4Argc ==2)
            {
                i4CurTop= (U8)StrToInt((CHAR *) aszArgv[1]);
                //ITuner_OP(psDvbtDemodCtx->psTunerCtx, itSetTop, i4CurTop, &ucData1); 
                //PARAM_SETFREQ_T SetFreqPara;
                SetFreqPara.Freq = (psDvbtDemodCtx->Frequency)/1000;
                SetFreqPara.Modulation = MOD_DVBT;
                SetFreqPara.fgAutoSearch =( psDvbtDemodCtx->u1ChSearchMode & 0x00); //no autosearch,just set freq one time
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
            if (DVBT_GetReg(psDvbtDemodCtx, u2RegAddr, ucData, ucPar))
            {
                mcSHOW_USER_MSG(("MTDVBT Get Register 0x%03X Fail!\n", u2RegAddr));
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
            if (DVBT_SetReg(psDvbtDemodCtx, u2RegAddr, &ucPar, 1))
            {
                mcSHOW_USER_MSG(("MTDVBT Set Register 0x%03X Fail!\n", u2RegAddr));
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
                DVBT_DriverStart(psDvbtDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);
                mcSHOW_USER_MSG(("Driver On!\n"));
            }
            else
            {
                DVBT_DriverStop(psDvbtDemodCtx);
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
                Tuner_SetFreq_DVBT(pTunerCtx, pTunerCtx->, MOD_NOT_ATV, FALSE);
                mcSHOW_USER_MSG(("set DTV TOP = 0x%02X\n", ucData[0]));
            }
            break;
*/
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
        case 'm':
            if(pDvbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
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
                    mcMUTEX_UNLOCK(pPdCtx->t_escape_mon_suspend);
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
                DVBT_Connect(psDvbtDemodCtx, MT5391_BW_8MHz);
                mcSHOW_USER_MSG(("DVBT Connect!\n"));
            }
            else
            {
                DVBT_DisConnect(psDvbtDemodCtx);
                mcSHOW_USER_MSG(("DVBT Disconnect!\n"));
            }
            break;

        case 'y':    // stat PEC
            if (i4Argc > 1)
            {
                psDvbtDemodCtx->u4PecObserveTime = StrToInt(aszArgv[1]); // Input1: Observe time(sec)
                if (psDvbtDemodCtx->u4PecObserveTime > 600)
                {
                    psDvbtDemodCtx->u4PecObserveTime = 600;
                    mcSHOW_USER_MSG(("Max observation time is 600 sec!"));
                }

                psDvbtDemodCtx->u2PecRefreshTime = StrToInt(aszArgv[2]); // Input2: Refresh time(sec)
                u2KeepRefreshTime = psDvbtDemodCtx->u2PecRefreshTime;
//                psDvbtDemodCtx->u1PecFlag = 1;
                DVBT_UpdateInfo(psDvbtDemodCtx);
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
                mcSHOW_USER_MSG(("u2PecRefreshCnt = %04d\n",  psDvbtDemodCtx->u2PecRefreshTime));
                mcSHOW_USER_MSG(("u4PecObserveTime = %05d\n",  psDvbtDemodCtx->u4PecObserveTime));
            }
            else
            {
                mcSHOW_USER_MSG(("\t y ObserveTime(sec) RefreshTime(sec): Start PEC \n"));
            }
            break;

        case 'z':   // stop PEC
            fgPec_dtd_y = FALSE;
            break;
		case 'b':   // stop PEC
            DVBT_Recover(psDvbtDemodCtx);
            break;

        default:
            break;
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerDtdCommand unlock pPdCtx->hHalLock\n"));
}

/**********************************************************************/
void DVBT_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        INT32 i4Argc, const CHAR** aszArgv)
{
    //UCHAR   ucCmdId = 0;
    UCHAR   ucArg1;
    UCHAR   ucCmdSize = 0;
    UINT8 tunerType;
    UINT8 ucData[256];

    mcSHOW_DBG_MSG4(("DVBT_TunerCommand pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerCommand pPdCtx->hHalLock\n"));

    if (i4Argc > 0) {
        //ucCmdId = *((CHAR *) aszArgv[0]);
        ucCmdSize = x_strlen((CHAR *) aszArgv[0]);
    }

    switch (ucCmdSize) {
    case 1: // one-word cmd
        if (x_strcmp((CHAR *) aszArgv[0], "s") == 0) {
            if((fcTUNER_TYPE != cDVBT_TUNER_ALL) && (fcTUNER_TYPE != cDVBT_2K10_TUNERLST)){
                mcSHOW_USER_MSG(("Only build tuner all image can use this command\r\n"));
                break;
            }
            else{
                if(i4Argc <= 1 || i4Argc >=3 ){
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
        if (x_strcmp((CHAR *) aszArgv[0], "sh") == 0) {
            if (i4Argc > 1) 
            {
                if (StrToInt(aszArgv[1]))
                    DVBT_SetHpLp(psDvbtDemodCtx, EN_HP_DECODE);//HP
                else
                    DVBT_SetHpLp(psDvbtDemodCtx, EN_LP_DECODE);//LP
            }
            DVBT_GetReg(psDvbtDemodCtx, MT5391_REG_TPS_REG_1, ucData, 1);
            mcSHOW_USER_MSG(("set hier = %d (1:HP, 0:LP)\n", !(ucData[0]&0x40) ));
            vDVBTGetPara(psDvbtDemodCtx);
            mcSHOW_USER_MSG(("current hier = %d (1:none, 2:HP, 3:LP)\n", DVBT_GetHier(psDvbtDemodCtx)));
            DVBT_DemodReset(psDvbtDemodCtx); // reset to trigger MT5391_SetTsClkRate() in FW
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "ar") == 0) {
            if (i4Argc > 1) 
            {
                if (StrToInt(aszArgv[1]) == 1)
                {
                    DVBT_SetArCoef(psDvbtDemodCtx, 0x3C);   // Ar coefficiet ON
                    mcSHOW_USER_MSG(("AR Coefficient ON\n"));
                }
                else if ( StrToInt(aszArgv[1]) == 0)
                {
                    DVBT_SetArCoef(psDvbtDemodCtx, 0x00);   // Ar coefficiet OFF
                    mcSHOW_USER_MSG(("AR Coefficient OFF\n"));
                }
            }
            else 
            {
                DVBT_GetArCoef(psDvbtDemodCtx, &(ucData[0]));
                mcSHOW_USER_MSG(("AR Coefficient = 0x%x\n", ucData[0]));
            }
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "tm") == 0)
        {
            if (i4Argc > 1)
            {
                if (StrToInt(aszArgv[1]) == 1)   // parallel
                {
                    DVBT_SetTsOutputMode(psDvbtDemodCtx, TRUE);
                    mcSHOW_USER_MSG(("[DVBT] Set TS to parallel mode\n"));
                }
                else if ( StrToInt(aszArgv[1]) == 0)   // serial mode
                {
                    DVBT_SetTsOutputMode(psDvbtDemodCtx, FALSE);
                    mcSHOW_USER_MSG(("[DVBT] Set TS to serial mode\n"));
                }
            }
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "te") == 0) 
        {
            if (StrToInt(aszArgv[1]) == 1)   // pass-through
            {
                DVBT_SetTsPathMode(psDvbtDemodCtx, TRUE);
                mcSHOW_USER_MSG(("[DVBT] Set TS path pass-through\n"));
            }
            else if ( StrToInt(aszArgv[1]) == 0)   // tri-state
            {
                DVBT_SetTsPathMode(psDvbtDemodCtx, FALSE);
                mcSHOW_USER_MSG(("[DVBT] Set TS path tri-state\n"));
            }
        }
		else if (x_strcmp((CHAR *) aszArgv[0], "sp") == 0)
		{
			if (i4Argc > 1)
			{
				ucArg1 = (U8)StrToInt(aszArgv[1]);
				DVBT_SetPGAGainIndex(psDvbtDemodCtx, ucArg1);
				mcSHOW_USER_MSG(("Set PGA Gain Index ucArg1=%d\r\n",ucArg1));

			}
		}
        break;
	case 3:
		if (x_strcmp((CHAR *) aszArgv[0], "snr") == 0) {
			if (i4Argc > 1)
			{
				psDvbtDemodCtx->s2snr_cc = (U8)StrToInt(aszArgv[1]);
				mcSHOW_USER_MSG(("Set SNR = SNR-0.1*%d\r\n",psDvbtDemodCtx->s2snr_cc));
			}
        }
        break;

    case 4:
        {
            SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDvbtDemodCtx->psTunerCtx->specific_member.eu_ctx);
            if (x_strcmp((CHAR *) aszArgv[0], "stop") == 0) {
                if (i4Argc > 1) {
                    pEuTunerCtx->m_aucPara[0] = (U8)StrToHex((CHAR *) aszArgv[1], 2);
                }
                mcSHOW_USER_MSG(("TOP value = 0x%x\n", pEuTunerCtx->m_aucPara[0]));
            }
        }
        break;

    case 5:
    /*
        if (x_strcmp((CHAR *) aszArgv[0], "monon") == 0) 
        {
            ucToolxInfoUpdateLvl  = (U8) StrToInt(aszArgv[2]);
            if (StrToInt(aszArgv[1]) < 100)
            {   // disable info update if refreah rate is too fast
                u2ToolxInfoUpdateRate = 1000;
                fgToolxInfoUpdate = FALSE;
            }
            else
            {
                u2ToolxInfoUpdateRate = (U16)StrToInt(aszArgv[1]);
                fgToolxInfoUpdate = TRUE;
            }
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "pecon") == 0) 
        */
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
/*        if (x_strcmp((CHAR *) aszArgv[0], "monoff") == 0) {
            fgToolxInfoUpdate = FALSE;
        }
        else if (x_strcmp((CHAR *) aszArgv[0], "pecoff") == 0) {
*/
        if (x_strcmp((CHAR *) aszArgv[0], "monoff") == 0) {
            fgPec = FALSE;
        }
        break;

    case 7:
        if (x_strcmp((CHAR *) aszArgv[0], "dagcmon") == 0)
        {
            if (i4Argc == 4)
            {
                DVBT_SetDagcMon(psDvbtDemodCtx, StrToInt(aszArgv[1]), StrToInt(aszArgv[2]), StrToInt(aszArgv[3]));
            }
        }
        break;

    case 8:
         if (x_strcmp((CHAR *) aszArgv[0], "checkreg") == 0) 
        {
            mcSHOW_USER_MSG(("DVBT basic register settings check: \n"));

            mcSHOW_USER_MSG(("DTD PLL CFG (0x2000d134) = 0x%x\tDTD PLL setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x134), (mcRISC_Reg(CKGEN_BASE, 0x134) & 0x00001F70)));
            mcSHOW_USER_MSG(("DTD PLL CFG (0x2000d12c) = 0x%x\tDTD PLL setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x12c), (mcRISC_Reg(CKGEN_BASE, 0x12c) & 0x04000000)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d1d0) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x1d0)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d1d4) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x1d4)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d21c) = 0x%x\tDEMOD ADC setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x21c), (mcRISC_Reg(CKGEN_BASE, 0x21c) & 0x00030003)));
            mcSHOW_USER_MSG(("PINMUX CFG (0x2000d400) = 0x%x\tPINMUX setting = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x400), (mcRISC_Reg(CKGEN_BASE, 0x400) & 0x20000000)));
            DVBT_GetReg(psDvbtDemodCtx, 0x93e, ucData, 1);
            mcSHOW_USER_MSG(("DVBT MODE (0x2002b93e) = 0x%x\tDVBT MODE setting = 0x%x\n", ucData[0], (ucData[0] & 0x01)));
            DVBT_GetReg(psDvbtDemodCtx, 0x4a0, ucData, 4);
            mcSHOW_USER_MSG(("RISC WAIT TIME (0x2002b4a0) = 0x%x\n", (*((UINT32*)ucData))));
/*            mcSHOW_USER_MSG(("DTD PLL CFG (0x2000d180) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x180)));
            mcSHOW_USER_MSG(("DTD PLL CFG (0x2000d184) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x184)));
            mcSHOW_USER_MSG(("TVD PLL CFG (0x2000d170) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x170)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d1d0) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x1d0)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d1d4) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x1d4)));
            mcSHOW_USER_MSG(("DEMOD ADC (0x2000d1d8) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x1d8)));
            mcSHOW_USER_MSG(("CLKGEN CFG (0x2000d210) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x210)));
            mcSHOW_USER_MSG(("CLKGEN CFG (0x2000d214) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x214)));
            mcSHOW_USER_MSG(("CLKGEN CFG (0x2000d218) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x218)));
            mcSHOW_USER_MSG(("CLKGEN CFG (0x2000d21c) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x21c)));
            mcSHOW_USER_MSG(("CLKGEN CFG (0x2000d204) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x204)));
            mcSHOW_USER_MSG(("TSO PAD DRIVING (0x2000d320) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x320)));
            mcSHOW_USER_MSG(("PINMUX CFG (0x2000d408) = 0x%x\n", mcRISC_Reg(CKGEN_BASE, 0x408))); */

            DVBT_GetReg(psDvbtDemodCtx, 0x00, ucData, 1);
            mcSHOW_USER_MSG(("uP CTRL REG= 0x%02x\n", ucData[0]));

            DVBT_GetReg(psDvbtDemodCtx, 0x0D, ucData, 2);
            //DVBT_GetReg(psDvbtDemodCtx, 0x0D, ucData, 1);
            //DVBT_GetReg(psDvbtDemodCtx, 0x0E, ucData+1, 1);
            mcSHOW_USER_MSG(("uP TOGGLE= 0x%02x\n", ucData[0]));
            mcSHOW_USER_MSG(("uP RUN_TAG= 0x%02x\n", ucData[1]));

            DVBT_GetReg(psDvbtDemodCtx, 0x1A0, ucData, 2);
            //DVBT_GetReg(psDvbtDemodCtx, 0x1A0, ucData, 1);
            //DVBT_GetReg(psDvbtDemodCtx, 0x1A1, ucData+1, 1);
            mcSHOW_USER_MSG(("IFAGC Target Level Set= 0x%02x%02x\n", ucData[1], ucData[0]));

            DVBT_GetReg(psDvbtDemodCtx, 0x1D7, ucData, 2);
            //DVBT_GetReg(psDvbtDemodCtx, 0x1d7, ucData, 1);
            //DVBT_GetReg(psDvbtDemodCtx, 0x1d8, ucData+1, 1);
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
        mcSHOW_USER_MSG(("\t te [0(disable)/1(enable)]      : ts enable\n"));
        mcSHOW_USER_MSG(("\t tm [0(serial)/1(parallel)]     : ts mode\n"));
        mcSHOW_USER_MSG(("\t dagcmon [0(off)/1(on)] [monTime(20)] [monConfTh(10)] : Set DAGC monitor parameters\n"));

        mcSHOW_USER_MSG(("\r\n"));
        mcSHOW_USER_MSG(("\r\n"));
        break;
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerCommand unlock pPdCtx->hHalLock\n")); 
}

/**********************************************************************/
void DVBT_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    // No implement
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
}

void *GetDigiDvbtTunerCtx(void)
{
    return psDvbtDemodCtx->psTunerCtx;
}

/**********************************************************************/
void  DVBT_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    if(pPdCtx->fgDisStatus == TRUE)
    {
        /* Has Disconnected, return */
        mcSHOW_DBG_MSG(("DVBT Has Disconnect!\n"));
        return;
    }
    mcSHOW_DBG_MSG4(("DVBT_TunerDisc pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerDisc pPdCtx->hHalLock\n"));

/*    if (DVBT_StandbyMode(DEFAULT_I2C_ADDR))
        mcSHOW_USER_MSG(("MT5133 standby mode FAIL!!\n"));
    else
        mcSHOW_DRVAPI_MSG(("MT5133 enter standby mode\n"));
*/
    fgBreakMonitor = TRUE;
    DVBT_DisConnect(psDvbtDemodCtx);

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerDisc unlock pPdCtx->hHalLock\n"));

    /* set disconnect status */
    pPdCtx->fgDisStatus = TRUE;

    //fgIsDisconnected = TRUE;
    
}


INT16 DVBT_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type, VOID *pAnazAttribute, SIZE_T* pzAnaAttributeLen)
{
    TUNER_DIG_ATTRIBUTE_T *pAttri;
    INT32 temp;
    UINT16 u2Temp;
    INT16 s2Temp;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDvbtDemodCtx->psTunerCtx->specific_member.eu_ctx);

    mcSHOW_DBG_MSG4(("DVBT_TunerGetAttribute pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerGetAttribute pPdCtx->hHalLock\n")); 

    switch (e_get_type)
    {
    case TUNER_GET_TYPE_BER:
        //DVBT_UpdateInfo(psDvbtDemodCtx);   // Ken, for context update, best mux, 081217
        // JC, 090202, if context not updated, update here, 090202
        if (!fg1stMonitorRun)
        {
            vDVBTGetInfo(psDvbtDemodCtx);
            mcSHOW_DBG_MSG(("DVBT_TunerGetAttribute: TUNER_GET_TYPE_BER not enough time to update\n"));
        }
        *pzAnaAttributeLen = sizeof(INT32);
        temp = DVBT_GetPostVBer(psDvbtDemodCtx);
        x_memcpy(pAnazAttribute, &temp, *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_DIG_ATTRIBUTE:
        pAttri = (TUNER_DIG_ATTRIBUTE_T *) pAnazAttribute;
        pAttri->ui4_lower_bound_freq = pEuTunerCtx->m_s4FreqBoundLower;
        pAttri->ui4_upper_bound_freq = pEuTunerCtx->m_s4FreqBoundUpper;
        // freq offset tolerence range in Hz (for IFS = 4)
        switch (DVBT_GetBW(psDvbtDemodCtx))
        {
            case 0:  pAttri->ui4_fine_tune_delta_freq = 536000; break; //6MHz
            case 1:  pAttri->ui4_fine_tune_delta_freq = 625000; break; //7MHz
            case 2:  pAttri->ui4_fine_tune_delta_freq = 714000; break; //8MHz
            case 3:  pAttri->ui4_fine_tune_delta_freq = 446000; break; //5MHz
            default: pAttri->ui4_fine_tune_delta_freq = 0;      break;
        }
        break;

    case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        // update m_SigLvScan
        temp = 0;
        s2Temp = ITuner_OP( ITunerGetCtx(), itGetSSIIndex,0, &temp);
        if (s2Temp == ITUNER_OK)
        {
            mcSHOW_DBG_MSG(("TUNER_GET_TYPE_RAW_SIGNAL_LEVEL (SSI): %d-%d\n", temp, pEuTunerCtx->m_SigLvScan));
        }
        else
        {
            mcSHOW_DBG_MSG(("TUNER_GET_TYPE_RAW_SIGNAL_LEVEL (IFAGC): %d\n", pEuTunerCtx->m_SigLvScan));
        }
        *pzAnaAttributeLen = sizeof(S8);
        x_memcpy(pAnazAttribute, &(pEuTunerCtx->m_SigLvScan), *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_BER_TOLERANCE:
        *pzAnaAttributeLen = sizeof(INT32);
        x_memcpy(pAnazAttribute, &psDvbtDemodCtx->i4BerTh, *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
        *pzAnaAttributeLen = sizeof(UINT8);
        x_memcpy(pAnazAttribute, &pEuTunerCtx->m_SigLvTh, *pzAnaAttributeLen);
        break;
    case TUNER_GET_TYPE_CELL_ID:
        *pzAnaAttributeLen = sizeof(UINT16);
        vDVBTGetInfo(psDvbtDemodCtx);
        u2Temp = DVBT_GetCellId(psDvbtDemodCtx);
        x_memcpy(pAnazAttribute, &u2Temp, *pzAnaAttributeLen);
        break;

    case TUNER_GET_TYPE_TS_FMT:
        {
            TS_FMT_T tsFmt;
            *pzAnaAttributeLen = sizeof(TS_FMT_T);
            tsFmt = DVBT_GetTsFmt(psDvbtDemodCtx);
            x_memcpy(pAnazAttribute, &tsFmt, *pzAnaAttributeLen);
        }
        break;
    case TUNER_GET_TYPE_SQI:
        if (!fg1stMonitorRun)
        {
            vDVBTGetInfo(psDvbtDemodCtx);
            mcSHOW_DBG_MSG(("DVBT_TunerGetAttribute: TUNER_GET_TYPE_SQI not enough time to update\n"));
        }
        *pzAnaAttributeLen = sizeof(INT32);
        temp = DVBT_GetSQI(psDvbtDemodCtx);
        x_memcpy(pAnazAttribute, &temp, *pzAnaAttributeLen);        
        break;
    case TUNER_GET_TYPE_SSI:
        *pzAnaAttributeLen = sizeof(INT32);      
        ITuner_OP( ITunerGetCtx(), itGetSSIIndex,0, &temp);
        mcSHOW_DBG_MSG(("DVBT SSI: %d\n",temp));
        x_memcpy(pAnazAttribute, &temp, *pzAnaAttributeLen);
        break;

    default:
        {
            mcSHOW_DBG_MSG4(("DVBT_TunerGetAttribute: RMR_DRV_INV_GET_INFO!, type=%d\n", e_get_type));
            //return (RMR_DRV_INV_GET_INFO);
            break;
        }
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerGetAttribute unlock pPdCtx->hHalLock\n")); 

    return (RMR_OK);
}


/***********************************************************************/
INT32 DVBT_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
    UCHAR ucData = *(UCHAR *)pvAnaAttribute;
    UCHAR *pucData = (UCHAR *)pvAnaAttribute;


    mcSHOW_DBG_MSG4(("DVBT_TunerSetAttribute pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_TunerSetAttribute pPdCtx->hHalLock\n")); 


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
                psDvbtDemodCtx->u1TunerAcqType = *peActionType;
            }
            break;
#endif

    case TUNER_SET_TYPE_TS_TRISTATE:
        if (ucData == BOOL_FALSE) {
            DVBT_SetTsPathMode(psDvbtDemodCtx, TRUE);
        }
        else if (ucData == BOOL_TRUE) {
            DVBT_SetTsPathMode(psDvbtDemodCtx, FALSE);
        }
        break;

    case TUNER_SET_TYPE_TS_SP:
        if (ucData == SP_PARALLEL) {
            DVBT_SetTsOutputMode(psDvbtDemodCtx, TRUE);
        }
        else if (ucData == SP_SERIAL) {
            DVBT_SetTsOutputMode(psDvbtDemodCtx, FALSE);
        }
        break;

    case TUNER_SET_TYPE_CI_CARD_TYPE:
        mcSHOW_DBG_MSG(("DVBT_TunerSetAttribute: TUNER_SET_TYPE_CI_CARD_TYPE\n"));
        if (pucData[0] == TUNER_CI_CARD_TYPE_1) {
            mcSHOW_DBG_MSG(("TUNER_SET_TYPE_CI_CARD_TYPE: TUNER_CI_CARD_TYPE_1, para = %d\n", pucData[1]));
            if (pucData[1] == TRUE) {
                vSetFwVarDVBT(psDvbtDemodCtx, 0x6F, 0);
            }
            else if (pucData[1] == FALSE) {
                vSetFwVarDVBT(psDvbtDemodCtx, 0x6F, 1);
            }
        }
        break;

    case TUNER_SET_TYPE_TS_OUTPUT:
        if (ucData == BOOL_TRUE) {
            DVBT_SetTsOutput(psDvbtDemodCtx, TRUE);
        }
        else if (ucData == BOOL_FALSE) {
            DVBT_SetTsOutput(psDvbtDemodCtx, FALSE);
        }
        break;

    default:
            mcSHOW_DBG_MSG(("DVBT_TunerSetAttribute: RMR_DRV_INV_SET_INFO!\n"));
//            return (RMR_DRV_INV_SET_INFO);
            break;
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_TunerSetAttribute unlock pPdCtx->hHalLock\n")); 

    return (RMR_OK);
}


/***********************************************************************/
void DVBT_SetDmxExt(void)
{
    mcSHOW_DBG_MSG4(("DVBT_SetDmxExt pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DVBT_SetDmxExt pPdCtx->hHalLock\n")); 
        
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DVBT_SetDmxExt unlock pPdCtx->hHalLock\n")); 

}

/***********************************************************************/
void DVBT_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{   
    //UCHAR ucValue = 0;
    
    //mcMUTEX_LOCK(pPdCtx->hHalLock);   // no lock due to may be deadlock with TunerAcq

#if defined(CC_MT5135DVBT)
    DVBT_TunerI2c(psDvbtDemodCtx, bSwitchOn);
#endif

    //mcMUTEX_UNLOCK(pPdCtx->hHalLock);
}
#if 0
#if defined (CC_MT5135) || defined (CC_MT5135DVBT)
INT32 DVBT_Recover(DEMOD_CTX_T *psDemodCtx)
{

    U32     freq = 666000;
    UINT8   bw = MT5391_BW_8MHz;
	UINT8   bandwidth = MT5391_BW_8MHz;
	UINT8   u1Hierarchy = DVBT_HIR_PRIORITY_UNKNOWN;
	UINT8   fgChannelScan = FALSE;
    BOOL    fgRetSts = FALSE;
	UINT8   fgRet = 0;

	freq = psDemodCtx->psTunerCtx->u4RF_Freq;
	if(freq == 0) freq = 666000;
	bandwidth= DVBT_GetBW(psDemodCtx);
	u1Hierarchy= DVBT_GetHier(psDemodCtx);

	fgChannelScan = TRUE;

	ITuner_Init(ITunerGetCtx());

	if (DVBT_Recover_Initialize(psDemodCtx, DEFAULT_I2C_ADDR, TRUE))
	{
        mcSHOW_DBG_MSG(("DVBT_Recover_Initialize failed\n"));
        return (DRVAPI_TUNER_ERROR);
    }

	//DVBT_ChipInit(psDemodCtx);
	
	DVBT_FwSwitch(psDemodCtx);

	DVBT_DriverStop(psDemodCtx);
    
    fgRet = DVBT_SetNormalMode(psDemodCtx);

	DVBT_SetArCoef(psDemodCtx, 0x3C);

	switch(bandwidth) 
    {
    case 0:
        bw = MT5391_BW_6MHz;
    break;
    case 1:
        bw = MT5391_BW_7MHz;
        break;
    case 2:
    default:
        bw = MT5391_BW_8MHz;
        break;
    }

	mcSHOW_DBG_MSG(("DVBT_Recover:Current freq = %dKHz\n", freq));
	mcSHOW_DBG_MSG(("DVBT_Recover:Current bw = %dKHz\n", bw));
	mcSHOW_DBG_MSG(("DVBT_Recover:Current u1Hierarchy = %dKHz\n", u1Hierarchy));

	#if 1
	DigTunerBypassI2C(TRUE);
   	if (Tuner_Connect(freq, bw, fgChannelScan))
	{
            mcSHOW_DBG_MSG(("Failed on Tuner_SetFreq(%d) \n", freq));
            fgRetSts = FALSE;
    }
    DigTunerBypassI2C(FALSE);
	#endif
    
    DVBT_SetIfAgcCurve(psDemodCtx);
	DVBT_SetIF(psDemodCtx);
		
    DVBT_SetBW(psDemodCtx, bw);

	#if 1
	if (u1Hierarchy == DVBT_HIR_PRIORITY_LOW)
            DVBT_SetHpLp(psDemodCtx, EN_LP_DECODE); // set to LP 
    else
            DVBT_SetHpLp(psDemodCtx, EN_HP_DECODE); // set to HP
	#endif
    

    // driver start
    DVBT_DriverStart(psDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);
    
    return (DRVAPI_TUNER_OK);
}
#endif
#endif

/**********************************************************************/
void PD_DVBT_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*    pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = DVBT_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = DVBT_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = DVBT_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = DVBT_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DVBT_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DVBT_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DVBT_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DVBT_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = DVBT_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = DVBT_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = DVBT_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DVBT_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DVBT_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = DVBT_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = DVBT_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand = DVBT_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C = DVBT_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = DVBT_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = DVBT_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = DVBT_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DVBT_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd = DVBT_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = DVBT_TunerGetDiagInfo;
#endif
}
