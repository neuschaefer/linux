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

#include "pi_8g80_dtmb.h"
#include "PD_Def_dbg.h"  // Note that fcADD_DIAG_INFO defined in PD_Def_dbg.h, This file should be placed in the first position.
#include "pi_def_dtmb.h"
#include "TDIS.h"
#include "pi_demod.h"
#include "pd_tuner_type.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "com_defs.h"

#include "fe_tuner_common_if.h"
#include "eeprom_if.h"
#include "pd_8g80_dtmb.h"
#include "x_stl_lib.h"

//#include "pd_tuner_type.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DTMB_MON_POLL_INTRVL             100  // 100ms
#define DTMB_LOCK_TIME_OUT              24000//24000  // 24000ms
#define DTMB_LOCK_TIME_OUT_CNT         (DTMB_LOCK_TIME_OUT/DTMB_MON_POLL_INTRVL)
#if  fcTUNER_TYPE == cALPS_TDAG4B03A
#define DVBT_TUNER_CONN_TIME_THREHOLD   750                          //add 700ms , TDAG4B03A AGC speed too slow
#else
#define DVBT_TUNER_CONN_TIME_THREHOLD   140
#endif
#define DVBT_DEMOD_CONN_TIME_THREHOLD   250

#ifdef __MODEL_slt__

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

static TUNER_DEVICE_CTX_T*		pDvbtTunerDeviceCtx = NULL;

static x_break_fct     	MTDVBTisBreak;

// for Qtest
static U16	u2ToolxInfoUpdateRate = 1000;
//static U8   ucToolxInfoUpdateLvl  = 1;
//static U8   fgToolxInfoUpdate = FALSE;

#if 0
static U8   fgPec = FALSE;
static U16  u2AccEsr = 0;
static U32  u4AccTotPkt = 0;
static U32  u4AccErrPkt = 0;
static U32  u4AccCorBit = 0;
static U32  u4AccPer = 0;
#endif



//#if (fcTUNER_TYPE == cDVBT_TUNER_ALL)
static UINT8 ucTunerType;
//#endif

// t_escape_mon_suspend move to pdCtx
//static MUTEX_T  t_escape_mon_suspend;
static BOOL     fgMonitorThreadRunning;
static THREAD_T t_monitor_thread;
//static BOOL		fgIsDisconnected = TRUE;
static BOOL     fgBreakMonitor = TRUE;
static BOOL     fg1stMonitorRun = FALSE;
#ifdef EU_AUTO_ATP
 UINT8 gATP_TEST_ITEM_BIT = 0 ;
 UINT8 gATPFailBitMsk =0;
#endif

BOOL fgBreak=FALSE;

STATIC BOOL fgIsAuto = FALSE ;

extern  VOID    DigTunerBypassI2C(BOOL bSwitchOn);


static void PD_DTMB_Register_LTDIS_Fct(PTD_CTX ptTDCtx);

//extern TUNER_COMMONAPI_FCT_TBL_T *pTuner_CommonAPI_Fct;
/*
#if (fcTUNER_TYPE != cDVBT_TUNER_ALL)
extern void Tuner_CommonAPI_Register_Fct(TUNER_COMMONAPI_FCT_TBL_T *pcom_Fct);
#else
extern void Tuner_CommonAPI_Register_Fct(TUNER_COMMONAPI_FCT_TBL_T *pcom_Fct, U8 tunerType);
#endif
*/

/***********************************************************************/
#ifdef fcADD_DIAG_INFO
INT32 DTMB_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
	INT32	retSts = RMR_OK;
	
    mcSHOW_DBG_MSG4(("DTMB_TunerDisc pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerDisc pPdCtx->hHalLock\n"));

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
    mcSHOW_DBG_MSG4(("DTMB_TunerDisc unlock pPdCtx->hHalLock\n"));
    return retSts;
}

#define DRV_CUSTOM_TUNER_DIAG_ITEMS         16

INT32 DTMB_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                        VOID*                           pv_get_info,
                        SIZE_T*                         pz_size)
{
/*	TUNER_TYPE_DIAG_CONTENT_T *pDiagContent = (TUNER_TYPE_DIAG_CONTENT_T*) pv_get_info;
	INT32	retSts = RMR_OK;
	
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
    SetFreqPara.Modulation = MOD_DTMB;
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
   // U8     aucData[cMAX_READ_NUM + 1];

    //U32    u4AccPer;
   // U8     ucAcqCnt = 0xFF, ucRstCnt = 0xFF, ucArCoef = 0xFF;

    mcSHOW_DBG_MSG(("MonitorProcess start\n"));
    while (fgMonitorThreadRunning)
    {
    	if (fgBreakMonitor){
    		// After mcMUTEX_LOCK(pPdCtx->t_escape_mon_suspend), monitor thread will be suspended until
    		// TunerAcq or TunerClose invoked.
    		mcSHOW_DBG_MSG(("Entering DTMB mon_suspend\n"));
    		mcMUTEX_LOCK(pPdCtx->t_escape_mon_suspend);
    		mcSHOW_DBG_MSG(("Escape DTMB mon_suspend\n"));
    	}
    	else{
		    // mutex lock
            mcSHOW_DBG_MSG4(("MonitorProcess pPdCtx->hHalLock\n"));
            mcMUTEX_LOCK(pPdCtx->hHalLock);
            mcSHOW_DBG_MSG4(("escape MonitorProcess pPdCtx->hHalLock\n"));

            mcSHOW_DBG_MSG4(("Invoke DTMB_Monitor\n"));
            // ***** DVB-T Info Update ***** //
            if (!fg1stMonitorRun)
			
            mcDELAY_MS(100);
            DTMB_UpdateInfo(psDvbtDemodCtx);
			
            fg1stMonitorRun = TRUE;  // Ken, 090202, check Monitor Process actually execute, for best mux
			#if 0

			// ***** Qtest Info Update ***** //
            if (fgPec)
            {
                u4AccTotPkt += psDvbtDemodCtx->u2TotalPkt;
                u4AccErrPkt += psDvbtDemodCtx->u2ErrorPkt;
                u4AccCorBit += psDvbtDemodCtx->u4CorrBit;
                
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
//              mcSHOW_USER_MSG(("start.pec"));
                mcSHOW_USER_MSG(("start.log"));

                mcSHOW_USER_MSG(("%02d%01d",  LOG_TAG_FFT,  psDvbtDemodCtx->u1Fft));
                mcSHOW_USER_MSG(("%02d%01d",  LOG_TAG_GI,   psDvbtDemodCtx->u1GI));
                mcSHOW_USER_MSG(("%02d%01d",  LOG_TAG_CR,   psDvbtDemodCtx->u1CR));
                mcSHOW_USER_MSG(("%02d%02d",  LOG_TAG_SNR,  DVBT_GetSignalSNR(psDvbtDemodCtx)/10));

                mcSHOW_USER_MSG(("%02d%06d",  LOG_TAG_PEC_PER,     u4AccPer));
                mcSHOW_USER_MSG(("%02d%05d",  LOG_TAG_PEC_ESR,     u2AccEsr));
                mcSHOW_USER_MSG(("%02d%010d", LOG_TAG_PEC_PKT_TOT, u4AccTotPkt));
                mcSHOW_USER_MSG(("%02d%010d", LOG_TAG_PEC_PKT_ERR, u4AccErrPkt));
                mcSHOW_USER_MSG(("%02d%010d", LOG_TAG_PEC_COR_BIT, u4AccCorBit));
                mcSHOW_USER_MSG(("%02d%03d",  LOG_TAG_ACQ_CNT,     ucAcqCnt));
                mcSHOW_USER_MSG(("%02d%03d",  LOG_TAG_RST_CNT,     ucRstCnt));
                mcSHOW_USER_MSG(("%02d%03d",  LOG_TAG_AR_COEF,     ucArCoef));

//              mcSHOW_USER_MSG(("stop.pec\r\n"));
                mcSHOW_USER_MSG(("stop.log\r\n"));
#else
                mcSHOW_USER_MSG(("\n************************\n"));
                mcSHOW_USER_MSG(("SNR = %02d\n",  DVBT_GetSignalSNR(psDvbtDemodCtx)/10));
                mcSHOW_USER_MSG(("Total_Pkt = %010d\n",  u4AccTotPkt));
                mcSHOW_USER_MSG(("Error_Pkt = %010d\n",  u4AccErrPkt));
                mcSHOW_USER_MSG(("Reset_Cnt = %03d\n",  ucRstCnt));
                mcSHOW_USER_MSG(("Acc_Per = %06d", u4AccPer));
                mcSHOW_USER_MSG(("\n************************\n"));
#endif
            }

#endif


		    // mutex unlock
            //mcSHOW_DBG_MSG4(("MonitorProcess unlock pPdCtx->hHalLock\n"));
            mcMUTEX_UNLOCK(pPdCtx->hHalLock);
            mcSHOW_DBG_MSG4(("escape  MonitorProcess unlock pPdCtx->hHalLock\n"));

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
    if( (u1TestTunerType > cDVBT_TUNER_TYPE_START) && (u1TestTunerType <= cDVBT_VALID_TYPE_END) ){
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
char *DTMB_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG4(("DTMB_TunerGetVer pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerGetVer pPdCtx->hHalLock\n"));
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG4(("DTMB_TunerGetVer unlock pPdCtx->hHalLock\n"));
    return NULL;
}

/***********************************************************************/
void DTMB_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, 
                            UCHAR ucRegAddr, UCHAR ucRegValue)
{
    mcSHOW_DBG_MSG4(("DTMB_TunerSetRegSetting pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerSetRegSetting pPdCtx->hHalLock\n"));

    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerSetRegSetting unlock pPdCtx->hHalLock\n"));

}

void DTMB_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG4(("DTMB_TunerShowRegSetting pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerShowRegSetting pPdCtx->hHalLock\n"));

    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerShowRegSetting unlock pPdCtx->hHalLock\n"));

    
}

void DTMB_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    mcSHOW_DBG_MSG4(("DTMB_TunerTestI2C pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerTestI2C pPdCtx->hHalLock\n"));
    
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerTestI2C unlock pPdCtx->hHalLock\n"));    
}

/**********************************************************************/

/**********************************************************************/
S32 DTMB_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    S32 i4Ret = 0;
    mcSHOW_DBG_MSG4(("DTMB_TunerNimTest pPdCtx->hHalLock\n"));    
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerNimTest pPdCtx->hHalLock\n"));    

    //add your implementation here
    #ifdef CC_CAB_DEMOD_TUNER_BYPASS
    DigTunerBypassI2C(TRUE);
    #endif

	i4Ret = Tuner_Connect(666000, MT5391_BW_8MHz, FALSE);

    #ifdef CC_CAB_DEMOD_TUNER_BYPASS
    DigTunerBypassI2C(FALSE);
    #endif

    mcSHOW_DRVERR_MSG(("DTMB_TunerNimTest : %d (0:OK, 1:fail)\n", i4Ret));
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerNimTest unlock pPdCtx->hHalLock\n"));  


    return i4Ret;
}


/**********************************************************************/
S32 DTMB_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, 
                     PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
	//UCHAR		test = 0;
	//UINT32 i;

#if (fcTUNER_TYPE == cDVBT_TUNER_ALL) ||(fcTUNER_TYPE == cDVBT_2K10_TUNERLST) || (fcTUNER_TYPE == cTTE_I2CALLLST)
	UINT8 tunerType;
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
	PD_DTMB_Register_LTDIS_Fct(ptTDCtx);
        
    psDvbtDemodCtx = DVBT_DemodCtxCreate();
	if (psDvbtDemodCtx == NULL)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    psDvbtDemodCtx->psTunerCtx = ITunerGetCtx();

    /*link demod ctx to pd ctx*/
    pPdCtx->pDemodCtx = psDvbtDemodCtx;	
	
    // do tuner init before demod init, for demod init need tuner parameters
 #if (fcTUNER_TYPE != cDVBT_TUNER_ALL) &&(fcTUNER_TYPE != cDVBT_2K10_TUNERLST) && (fcTUNER_TYPE != cTTE_I2CALLLST)
    ITunerRegistration(cITUNER_DONT_CARE);
 #else
    tunerType = checkTunerTypeFromEeprom();
    ITunerRegistration(tunerType);
    mcSHOW_USER_MSG(("Register DVBT Tunerall type = %d\n",tunerType));		
 #endif
    ITuner_Init(ITunerGetCtx());

          
    // do demod init
	if (DTMB_Initialize(psDvbtDemodCtx, DEFAULT_I2C_ADDR, TRUE)){
		mcSHOW_USER_MSG(("DTMB_Initialize failed\n"));
		return (DRVAPI_TUNER_ERROR);
	}

	psDvbtDemodCtx->u1MWIntf = pDvbtTunerDeviceCtx->u1MWIntf;

    mcSHOW_DBG_MSG(("DVBT Disconnect @ TunerOpen!!\n"));
	
    DTMB_DisConnect(psDvbtDemodCtx);   // add by Ken, default disconnect demod for TS_CLK EMI issue, 090114

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
S32 DTMB_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
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
S16 DTMB_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        DRV_CONN_TYPE_T e_conn_type)
{
    S16 s2_ret = FALSE;
    mcMUTEX_LOCK(pPdCtx->hHalLock);


#if 1
    // report tuner unsync if TS unlocked or high PER 
    vDTMBGetStatus(psDvbtDemodCtx);
   // if (DVBT_GetTsLock(psDvbtDemodCtx) == TRUE)
    {
	if (pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	     if (DVBT_GetPER(psDvbtDemodCtx) <= DTMB_SYNC_PER_THRESHOLD)
		 {
		    //mcSHOW_DBG_MSG2(("TS lock!!!!!!!!!"));
            s2_ret = TRUE;
	     }
    }
#else
	if(DTMB_GetSync(psDvbtDemodCtx)==DVBT_DEMOD_STATE_LOCK)
		s2_ret = TRUE;
#endif


	
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

void DTMB_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach);

BOOL DTMB_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                    DRV_CONN_TYPE_T e_conn_type, 
                    void* pv_conn_info, 
                    SIZE_T z_conn_info_len, 
                    x_break_fct _BreakFct,
                    void* pvArg)
{
   	TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = 
			    	(TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;
    U32     freq;
    UINT8   bw=MT5391_BW_8MHz;
    U16     ii = 0;
    UINT8   fgChannelScan = FALSE;
	BOOL    fgRetSts = FALSE;
    BOOL    fgConnSts = TRUE;
    UINT8   u1DvbtDemodState;
   // UCHAR   ucData = 0x3C;   // for best mux
    UINT32  u4TickCnt, u4ConnTime;
   UINT8 mode, GuardInterval, TimDeInter, CarrierMode, PnNumber, FecRate;

    mcSHOW_DBG_MSG4((" DTMB_TunerAcq pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG4((" escape DTMB_TunerAcq pPdCtx->hHalLock\n"));

    MTDVBTisBreak = _BreakFct;

    u4TickCnt = os_get_sys_tick(); 


#if 0
    mcSHOW_DBG_MSG4((" DVBT_TunerAcq pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG4((" escape DVBT_TunerAcq pPdCtx->hHalLock\n"));

    MTDVBTisBreak = _BreakFct;

    u4TickCnt = os_get_sys_tick(); 

    // 090724, Power up AD in the beginning, Menghau
    DVBT_ChipInit();

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

        #ifdef CC_CAB_DEMOD_TUNER_BYPASS
        DigTunerBypassI2C(TRUE);
        #endif
	    if (Tuner_Connect(freq, bw, fgChannelScan)){
	        mcSHOW_DBG_MSG(("Failed on Tuner_SetFreq(%d) \n", freq));
	        fgConnSts = FALSE;
	    }
        #ifdef CC_CAB_DEMOD_TUNER_BYPASS
        DigTunerBypassI2C(FALSE);
        #endif
	    
	    u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
        mcSHOW_DBG_MSG(("DVBT tuner connection time = %dms\n", u4ConnTime));
	    if ((u4ConnTime > DVBT_TUNER_CONN_TIME_THREHOLD) && (fgChannelScan)){
	        mcSHOW_DBG_MSG(("EU Driver ATP Fail: DVBT tuner connection time = %dms, too long!!\n", u4ConnTime));
	    }
#ifdef  EU_AUTO_ATP
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
        // set BW
        DVBT_SetBW(psDvbtDemodCtx, bw);
        
        // set hier
        if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
            DVBT_SetHpLp(psDvbtDemodCtx, EN_LP_DECODE); // set to LP 
        else
            DVBT_SetHpLp(psDvbtDemodCtx, EN_HP_DECODE); // set to HP
                
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
                
        	    if ((u4ConnTime > DVBT_DEMOD_CONN_TIME_THREHOLD) && (fgChannelScan)){
                    mcSHOW_DBG_MSG(("EU Driver ATP Fail: DVBT demod connection time = %dms, too long!!\n", u4ConnTime));
                }
            }
	    }
    }

    DVBT_UpdateInfo(psDvbtDemodCtx);   // Ken, for context update, best mux, 081217

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DVBT_TunerAcq unlock pPdCtx->hHalLock\n"));
    

    if (fgRetSts) { // print info after update
        mcSHOW_DBG_MSG(("TPS lock, freq = %d kHz, lock time = %d ms, SNR = %d/10 dB, Hier = %d (0: unknown, 1: none, 2:hp, 3:lp)\n", 
           freq, ii*DVBT_MON_POLL_INTRVL, DVBT_GetSignalSNR(psDvbtDemodCtx), DVBT_GetHier(psDvbtDemodCtx)));
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
#endif
// 090724, Power up AD in the beginning, Menghau

    //DVBT_ChipInit();

    // 090326, Split DVBT_Connect into SetNormalMode and SetBw for IFAGC startup in advance, JC
    
#if 0
    if (psDvbtDemodCtx->u1TunerAcqType != TUNE_FREQ_ONLY)   // Do not set Demod when do TUNE_FREQ_ONLY 
    {
    	DVBT_DriverStop(psDvbtDemodCtx);
    	
    	//mcDELAY_MS(10);   
    	
        if (DVBT_SetNormalMode(psDvbtDemodCtx)){
	        mcSHOW_USER_MSG(("Failed on DVBT_SetNormalMode()\n"));
	        fgConnSts = FALSE;
        }
    }
#endif

    if(pt_tuner_info->e_mod == MOD_UNKNOWN){  //channel scan, LC 080327
        //ucData = 0;
        fgChannelScan = TRUE;
    }
    // force to turn off AR when channel scan for BestMux selection
    //fgSetVars(pMt5133Ctx, 0x507, &ucData, 1);
    
    //DVBT_SetArCoef(psDvbtDemodCtx, ucData);


    fgBreakMonitor = TRUE;  //if monitor thread is running, pause it.

	freq = pt_tuner_info->ui4_freq/1000;

	psDvbtDemodCtx->fgAutoScan=TRUE;
	//if(pt_tuner_info->e_conn_mode == CONN_MODE_SURF)
	if( (pt_tuner_info->e_mod != MOD_UNKNOWN) && (!fgIsAuto))
	{
	    psDvbtDemodCtx->fgAutoScan=FALSE;
		//get para from mw: QAMSize, GuardInterval,TimDeInter,CarrierMode,PnNumber, FecRate
    	switch(pt_tuner_info->e_mod) 
    	{
    		case MOD_QPSK:
        		mode = constellation_qam4;
    			break;
    		case MOD_QAM_4_NR:
        		mode = constellation_qam4NR;
        		break;
    		case MOD_QAM_16:
				mode = constellation_qam16;
				break;
			case MOD_QAM_32:
				mode = constellation_qam32;
				break;
			case MOD_QAM_64:
				mode = constellation_qam64;
				break;
    		default:
        		mode = constellation_qam64;
				mcSHOW_DRVAPI_MSG(("Invalid input modulation(%d) to DTMB_TunerAcq!\n", pt_tuner_info->e_mod));
        		break;
    	}
    	psDvbtDemodCtx->u1Mod = mode;

   		switch(pt_tuner_info->e_guard_int) 
    	{
    		case GUARD_INT_420:
        		GuardInterval = GuardIntvl_PN420;
    			break;
    		case GUARD_INT_595:
        		GuardInterval = GuardIntvl_PN595;
        		break;
    		case GUARD_INT_945:
				GuardInterval = GuardIntvl_PN945;
				break;
    		default:
        		GuardInterval = GuardIntvl_PN420;
				mcSHOW_DRVAPI_MSG(("Invalid input GuardIntvl(%d) to DTMB_TunerAcq!\n", pt_tuner_info->e_guard_int));
        		break;
   		}
	 	psDvbtDemodCtx->u1GI = GuardInterval;

    	switch(pt_tuner_info->e_timer_int) 
    	{
    		case TIMER_INT_240:
        		TimDeInter = TimDeInterl_240;
    			break;
    		case TIMER_INT_720:
        		TimDeInter = TimDeInterl_720;
       		    break;
    		default:
        		TimDeInter = TimDeInterl_240;
				mcSHOW_DRVAPI_MSG(("Invalid input TimDeInter(%d) to DTMB_TunerAcq!\n", pt_tuner_info->e_timer_int));
        		break;
    	}
	 	psDvbtDemodCtx->u1TimDeInterl = TimDeInter;

    	switch(pt_tuner_info->e_CarrierMode) 
    	{
    		case MUTI_CARRIER:
        		CarrierMode = CarrierMode_MC;
    			break;
    		case SINGAL_CARRIER:
        		CarrierMode = CarrierMode_SC;
        	break;
    			default:
        		CarrierMode = CarrierMode_MC;
				mcSHOW_DRVAPI_MSG(("Invalid input CarrierMode(%d) to DTMB_TunerAcq!\n", pt_tuner_info->e_CarrierMode));
        		break;
    	}
		psDvbtDemodCtx->u1CarrierMode = CarrierMode;

    	switch(pt_tuner_info->e_PnNumber) 
    	{
    		case VAR_PN_SEQ:
        		PnNumber = PnNumber_Variable;
    			break;
    		case CON_PN_SEQ:
        		PnNumber = PnNumber_Constant;
        		break;
    		default:
        		PnNumber = PnNumber_Variable;
				mcSHOW_DRVAPI_MSG(("Invalid input PnNumber(%d) to DTMB_TunerAcq!\n", pt_tuner_info->e_PnNumber));
        		break;
    	}
		psDvbtDemodCtx->u1PnNumber = PnNumber;

    	switch(pt_tuner_info->e_FecRate) 
    	{
    		case CODE_RATE04:
        		FecRate = FecRateLow;
    			break;
   		 	case CODE_RATE06:
        		FecRate = FecRateMiddle;
        		break;
    		case CODE_RATE08:
				FecRate = FecRateHigh;
				break;
    		default:
        		FecRate = FecRateLow;
				mcSHOW_DRVAPI_MSG(("Invalid input FecRate(%d) to DTMB_TunerAcq!\n", pt_tuner_info->e_FecRate));
       			break;
    	}
		psDvbtDemodCtx->u1FecRate = FecRate;

		mcSHOW_DBG_MSG3(("DTMB_TunerAcq: QAMSize = %d \n", psDvbtDemodCtx->u1Mod));
		mcSHOW_DBG_MSG3(("DTMB_TunerAcq: Guard Interval = %d \n", psDvbtDemodCtx->u1GI));
		mcSHOW_DBG_MSG3(("DTMB_TunerAcq: TimDeInterl = %d \n", psDvbtDemodCtx->u1TimDeInterl));
		mcSHOW_DBG_MSG3(("DTMB_TunerAcq: FecRate = %d \n", psDvbtDemodCtx->u1FecRate));
		mcSHOW_DBG_MSG3(("DTMB_TunerAcq: CarrierMode = %d \n", psDvbtDemodCtx->u1CarrierMode));
		mcSHOW_DBG_MSG3(("DTMB_TunerAcq: PnNumber = %d \n",  psDvbtDemodCtx->u1PnNumber));
	}
              
    if (psDvbtDemodCtx->u1TunerAcqType != ACQ_DEMOD_ONLY)   // Do not SetFreq when do demod-only acq

    {              
        //mcSHOW_DBG_MSG(("_Freq1 = %d kHz, set hier = %d, \n", freq, pt_tuner_info->e_hierarchy_priority));	
        mcSHOW_DBG_MSG(("_Freq1 = %d kHz\n", freq));	
       // #ifdef CC_CAB_DEMOD_TUNER_BYPASS
       DigTunerBypassI2C(TRUE);
		//DVBT_TunerDetachI2C(psDvbtDemodCtx, TRUE);
       // #endif
		
	    if (Tuner_Connect(freq, bw, fgChannelScan)){
	        mcSHOW_DBG_MSG(("Failed on Tuner_SetFreq(%d) \n", freq));
	        fgConnSts = FALSE;
	    }
		
        //#ifdef CC_CAB_DEMOD_TUNER_BYPASS
        DigTunerBypassI2C(FALSE);
		//DVBT_TunerDetachI2C(psDvbtDemodCtx, FALSE);
        //#endif

	    if (DTMB_Connect(psDvbtDemodCtx, 8))
		{
	    	mcSHOW_USER_MSG(("Failed on Tuner_Connect(%d)", bw));
	        fgConnSts = FALSE;
	    }
		
	    u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
        mcSHOW_DBG_MSG(("DTMB tuner connection time = %dms\n", u4ConnTime));

    }

    if (psDvbtDemodCtx->u1TunerAcqType != TUNE_FREQ_ONLY)   // Do not set Demod when do TUNE_FREQ_ONLY 
    {
        u4TickCnt = os_get_sys_tick(); 
        
        // set hier
        #if 0
        if (pt_tuner_info->e_hierarchy_priority == DVBT_HIR_PRIORITY_LOW)
            DVBT_SetHpLp(psDvbtDemodCtx, EN_LP_DECODE); // set to LP 
        else
            DVBT_SetHpLp(psDvbtDemodCtx, EN_HP_DECODE); // set to HP
        #endif   
		
        // driver start
        //DVBT_DriverStart(psDvbtDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);

	    if (fgConnSts) {
	        // polling lock loop
	        if(psDvbtDemodCtx->fgAutoScan==TRUE)
	        {
		        for (ii = 0; ii < DTMB_LOCK_TIME_OUT_CNT; ii++)
		        {   // get demod sync state
		            u1DvbtDemodState =DTMB_GetSync(psDvbtDemodCtx);
		            if (u1DvbtDemodState == DTMB_DEMOD_STATE_UNLOCK) {
		                fgRetSts = FALSE;
		                //break;
		            }
		            else if (u1DvbtDemodState == DTMB_DEMOD_STATE_LOCK) {
		                fgRetSts = TRUE;
		                break;
		            }
				    else if (u1DvbtDemodState == DTMB_DEMOD_STATE_UNKNOWN) {
		                fgRetSts = FALSE;
		                break;
		            }
		            //break function
		            if (MTDVBTisBreak(pvArg)){
		                mcSHOW_USER_MSG(("break DTMB_TunerAcq()!"));
		                fgRetSts = FALSE;
						fgBreak = TRUE;
		                break;
		            }
		            // delay for a while
		            mcDELAY_MS(DTMB_MON_POLL_INTRVL);
		        }
	        }
			else
			{
				for (ii = 0; ii < (DTMB_LOCK_TIME_OUT_CNT/10); ii++)//when manual switch need less time
		        {   
		            // delay for a while
		            mcDELAY_MS((DTMB_MON_POLL_INTRVL*2));//when manual switch, interval bigger is better
		            
		            // get demod sync state
		            u1DvbtDemodState =DTMB_ManualGetSync(psDvbtDemodCtx);
		            if (u1DvbtDemodState == DTMB_DEMOD_STATE_UNLOCK) {
		                fgRetSts = FALSE;
		                //break;
		            }
		            else if (u1DvbtDemodState == DTMB_DEMOD_STATE_LOCK) {
		                fgRetSts = TRUE;
						mcDELAY_MS((DTMB_MON_POLL_INTRVL));//when manual switch,more delay 100ms for TS stable
		                break;
		            }
				    else if (u1DvbtDemodState == DTMB_DEMOD_STATE_UNKNOWN) {
		                fgRetSts = FALSE;
		                break;
		            }
		            //break function
	            if (MTDVBTisBreak(pvArg)){
	                mcSHOW_USER_MSG(("break DTMB_TunerAcq()!"));
	                fgRetSts = FALSE;
	                break;
	            }
	          
		        }
			}
	        // if time-out
	        if(u1DvbtDemodState != DTMB_DEMOD_STATE_LOCK)
	        {
	        	mcSHOW_DBG_MSG(("After %d times get sync loop!DTMB not exist! time-out!\n",ii+1));
				u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
                mcSHOW_DBG_MSG(("DTMB Demod connection time = %dms\n", u4ConnTime));
	        }
            else // check Demod lock time only in non-time-out case
            {
            	mcSHOW_DBG_MSG((" After %d times get sync loop, Demod lock!!!!!!!!\n", ii+1));
                u4ConnTime = (os_get_sys_tick()-u4TickCnt) * x_os_drv_get_tick_period();
                mcSHOW_DBG_MSG(("DTMB Demod connection time = %dms\n", u4ConnTime));
                
        	   // if ((u4ConnTime > DVBT_DEMOD_CONN_TIME_THREHOLD) && (fgChannelScan)){
                  //  mcSHOW_DBG_MSG(("EU Driver ATP Fail: DTMB demod connection time = %dms, too long!!\n", u4ConnTime));
                //}
            }
	    }
    }

    DTMB_UpdateInfo(psDvbtDemodCtx);   // Ken, for context update, best mux, 081217

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4((" DTMB_TunerAcq unlock pPdCtx->hHalLock\n"));
    
#if 0
    if (fgRetSts) { // print info after update
        mcSHOW_DBG_MSG(("TPS lock, freq = %d kHz, lock time = %d ms, SNR = %d/10 dB, Hier = %d (0: unknown, 1: none, 2:hp, 3:lp)\n", 
           freq, ii*DTMB_MON_POLL_INTRVL, DTMB_GetSignalSNR(psDvbtDemodCtx), DVBT_GetHier(psDvbtDemodCtx)));
    }
#endif
    fg1stMonitorRun = FALSE;   // Ken, 090202, check Monitor Process actually execute, for best mux
    
	
	if(pDvbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
	{
	    //++ Escape monitor suspend
	    fgBreakMonitor = FALSE;
	    mcMUTEX_UNLOCK(pPdCtx->t_escape_mon_suspend);
	}


	/* Init Disconnect Status */
	pPdCtx->fgDisStatus = FALSE;

    return fgRetSts;


}

/**********************************************************************/
void DTMB_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                    DRV_CONN_TYPE_T e_conn_type, 
                    SIGNAL *_pSignal)
{
    //UINT8 ret;

    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignal pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerGetSignal pPdCtx->hHalLock\n"));                

    _pSignal->Frequency  = (S32)(psDvbtDemodCtx->psTunerCtx->u4RF_Freq - (DVBT_GetFreqOffset_kHz(psDvbtDemodCtx)>>8));
    // removed, tuner HAL already did the conversion
    //_pSignal->Frequency *= 1000; // convert from kHz to Hz 
    _pSignal->SymbolRate = 0;
    _pSignal->QAMSize = 0;

    vDTMBGetPara(psDvbtDemodCtx); // update Hier, Hp/Lp, BW

	switch (DVBT_GetBW(psDvbtDemodCtx))
	{
		case 0:  _pSignal->e_bandwidth = BW_6_MHz;   break;
		case 1:  _pSignal->e_bandwidth = BW_7_MHz;   break;
		case 2:  _pSignal->e_bandwidth = BW_8_MHz;   break;
		case 3:
		default: _pSignal->e_bandwidth = BW_UNKNOWN; break;
	}

	switch (psDvbtDemodCtx->u1Mod)
	{
		case constellation_qam4:    _pSignal->QAMSize = 4;   break;
		case constellation_qam4NR:  _pSignal->QAMSize = 5;   break;
		case constellation_qam16:   _pSignal->QAMSize = 16;   break;
		case constellation_qam32:   _pSignal->QAMSize = 32;   break;
		case constellation_qam64:   _pSignal->QAMSize = 64;   break;
		default: _pSignal->QAMSize = Para_UNKONW; break;
	}
	
	switch (psDvbtDemodCtx->u1GI)
	{
		case GuardIntvl_PN420:    _pSignal->u1GI = GUARD_INT_420;   break;
		case GuardIntvl_PN595:    _pSignal->u1GI = GUARD_INT_595;   break;
		case GuardIntvl_PN945:    _pSignal->u1GI = GUARD_INT_945;   break;
		default: _pSignal->u1GI = GUARD_INT_UNKNOWN; break;
	}
	
	switch (psDvbtDemodCtx->u1TimDeInterl)
	{
		case TimDeInterl_240:    _pSignal->u1TimDeInterl = TIMER_INT_240;   break;
		case TimDeInterl_720:    _pSignal->u1TimDeInterl = TIMER_INT_720;   break;
		default: _pSignal->u1TimDeInterl = TIMER_INT_UNKNOWN; break;
	}
	
    switch (psDvbtDemodCtx->u1FecRate)
	{
		case FecRateLow:    _pSignal->u1FecRate = CODE_RATE04;   break;
		case FecRateMiddle:    _pSignal->u1FecRate = CODE_RATE06;   break;
		case FecRateHigh:    _pSignal->u1FecRate = CODE_RATE08;   break;
		default: _pSignal->u1FecRate = Para_UNKONW; break;
	}
	
	switch (psDvbtDemodCtx->u1CarrierMode)
	{
		case CarrierMode_MC:    _pSignal->u1CarrierMode = MUTI_CARRIER;   break;
		case CarrierMode_SC:    _pSignal->u1CarrierMode = SINGAL_CARRIER;   break;
		default: _pSignal->u1CarrierMode = Para_UNKONW; break;
	}
	
	switch (psDvbtDemodCtx->u1PnNumber)
	{
		case PnNumber_Variable:    _pSignal->u1PnNumber = VAR_PN_SEQ;   break;
		case PnNumber_Constant:    _pSignal->u1PnNumber = CON_PN_SEQ;   break;
		default: _pSignal->u1PnNumber = Para_UNKONW; break;
	}

#if 0
    _pSignal->e_hierarchy_priority = DVBT_GetHier(psDvbtDemodCtx);
    _pSignal->sHierInfo.eAlpha = (ALPHA_INFO_T)DVBT_GetHierMode(psDvbtDemodCtx);
    _pSignal->sHierInfo.eInDepth = (INTERLEAVE_INFO_T)DVBT_GetIndepth(psDvbtDemodCtx);
    mcSHOW_DBG_MSG(("DVBT_TunerGetSignal: PRIORITY = %d (1:None, 2:HP, 3:LP)\n", _pSignal->e_hierarchy_priority));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Alpha = %d (3: a=4)\n", _pSignal->sHierInfo.eAlpha));
    mcSHOW_DBG_MSG4(("DVBT_TunerGetSignal: Indepth = %d \n", _pSignal->sHierInfo.eInDepth));
#endif
	mcSHOW_DBG_MSG3(("DTMB_TunerGetSignal: QAMSize = %d \n", _pSignal->QAMSize));
	mcSHOW_DBG_MSG3(("DTMB_TunerGetSignal: Guard Interval = %d \n", _pSignal->u1GI-5));
	mcSHOW_DBG_MSG3(("DTMB_TunerGetSignal: TimDeInterl = %d \n", _pSignal->u1TimDeInterl-1));
	mcSHOW_DBG_MSG3(("DTMB_TunerGetSignal: FecRate = %d \n", _pSignal->u1FecRate));
	mcSHOW_DBG_MSG3(("DTMB_TunerGetSignal: CarrierMode = %d \n", _pSignal->u1CarrierMode));
	mcSHOW_DBG_MSG3(("DTMB_TunerGetSignal: PnNumber = %d \n",  _pSignal->u1PnNumber));

   
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignal unlock pPdCtx->hHalLock\n"));                    
}

/**********************************************************************/
U8   DTMB_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                DRV_CONN_TYPE_T e_conn_type)
{
    U8 u1_ret;

    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalLevel pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerGetSignalLevel pPdCtx->hHalLock\n"));                
    
    vDTMBGetStatus(psDvbtDemodCtx);
    u1_ret = DTMB_GetSignalLevel(psDvbtDemodCtx);

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalLevel unlock pPdCtx->hHalLock\n"));
    return u1_ret;
}

/**********************************************************************/
S16 DTMB_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)
{


    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalLeveldBm pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerGetSignalLeveldBm pPdCtx->hHalLock\n"));                



    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalLeveldBm unlock pPdCtx->hHalLock\n"));
    return 0;
}

/**********************************************************************/
UINT16  DTMB_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)
{
    
    UINT8 SNR;
    
    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalSNR pPdCtx->hHalLock\n"));                   
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerGetSignalSNR pPdCtx->hHalLock\n"));                    
    
    SNR = DTMB_GetSnrInfo(psDvbtDemodCtx);
    
    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalSNR unlock pPdCtx->hHalLock\n"));

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

UINT16  DTMB_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                                    DRV_CONN_TYPE_T e_conn_type)  // return value = PER*5*10^4
{
    
    INT32 u4_ret;
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
    mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalPER pPdCtx->hHalLock\n"));                
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerGetSignalPER pPdCtx->hHalLock\n"));                

	// Justin, move to update update Demod context to thread
    //DVBT_GetInfo(psDvbtDemodCtx);
    u4_ret = DVBT_GetPER(psDvbtDemodCtx);    

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG4(("DTMB_TunerGetSignalPER unlock pPdCtx->hHalLock\n")); 
    
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
void DTMB_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                            MPEG_FMT_T  *pt_mpeg_fmt)
{
    mcSHOW_DBG_MSG4(("DTMB_TunerSetMpgFmt pPdCtx->hHalLock\n"));                    
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerSetMpgFmt pPdCtx->hHalLock\n"));                        

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerSetMpgFmt unlock pPdCtx->hHalLock\n")); 

}

void DTMB_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        INT32 i4Argc, const CHAR** aszArgv)
{
    UCHAR   ucCmdId = 0, ucPar, ucData[256];
    U16    ii, jj, kk, u2RegAddr;
	UINT8   i4CurTop = 0;
	UINT8   ucData1,u1I2cAddr;
    //UINT8 temp[2];
    //UINT8 RegistWord[5]; 
    
    if (i4Argc < 1)
    {
        mcSHOW_USER_MSG(("Cmd: \n"));
		//mcSHOW_USER_MSG(("\t e [Register (UINT8)]: Set or Get 5 Register,The CLI input formats are: nim.atd e or nim.atd e ##-##-##-##-##\n"));
		mcSHOW_USER_MSG(("\t t                    [Top]: SetTOP\n"));
        mcSHOW_USER_MSG(("\t d:                   Dump DVBT registers\n"));
        mcSHOW_USER_MSG(("\t s:                   show signal/ts/tps status\n"));
//        mcSHOW_USER_MSG(("\t n  Normal/Standby mode switch (0: Normal; 1: Standby)\n"));
        mcSHOW_USER_MSG(("\t r [RegAddr] [Num]:   read  Demod register\n"));
        mcSHOW_USER_MSG(("\t w [RegAddr] [Value]: write Demod register\n"));        
        mcSHOW_USER_MSG(("\t v [1/0]:             Driver on/off\n"));   
		mcSHOW_USER_MSG(("\t n [1/0]:             Set Auto detect or Manual switch when channel change\n"));  
        //mcSHOW_USER_MSG(("\t t [TopValue]:        Set DTV TOP \n"));
//#if !defined(CC_MTAL) && !defined(CC_SDAL)
		if(pDvbtTunerDeviceCtx->u1MWIntf != cMTAL_IF && pDvbtTunerDeviceCtx->u1MWIntf != cSDAL_IF)
		{
        	mcSHOW_USER_MSG(("\t mp:                  Pause monitor thread\n"));
        	mcSHOW_USER_MSG(("\t mr:                  Resume monitor thread\n"));
		}
//#endif
    }	
	
    mcSHOW_DBG_MSG4(("DTMB_TunerDtdCommand pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerDtdCommand pPdCtx->hHalLock\n"));
    
    
    if (i4Argc > 0)
        ucCmdId = *((CHAR *) aszArgv[0]);
        
    switch (ucCmdId)
    {
     	case 's':
        // no monitor thread in MTAL/SDAL, do info update in nim.dtd s
        //#if (defined(CC_MTAL) || defined(CC_SDAL))
		if (pDvbtTunerDeviceCtx->u1MWIntf == cMTAL_IF || pDvbtTunerDeviceCtx->u1MWIntf == cSDAL_IF)
		{
            DTMB_UpdateInfo(psDvbtDemodCtx);
		}
        //#endif
                                   
//            mcSHOW_USER_MSG(("=================================\n"));
//            mcSHOW_USER_MSG(("SW Ver = %s\n", DVBT_GetSwVer()));
            mcSHOW_USER_MSG(("=================================\n"));
//            mcSHOW_USER_MSG(("IfAgc = 0x%02X\n", DVBT_GetIfAgc(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("Strength = %3d\n", DTMB_GetSignalLevel(psDvbtDemodCtx)));
            mcSHOW_USER_MSG(("SNR = %d/10 dB\n", DTMB_GetSignalSNR(psDvbtDemodCtx) ));
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
#ifdef EU_AUTO_ATP
         if (gATP_TEST_ITEM_BIT & 0x01)
         	{
               mcSHOW_DBG_MSG(("##Start\n##Strength=%d\n##BER = %d\n##PER = %d\n##TPSLOCK = %d\n##TSLOCK = %d\n##End\n",
		 DTMB_GetSignalLevel(psDvbtDemodCtx),	   	
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
       
        case 'n':
           if (i4Argc > 1)
            {
                ucPar = StrToInt((CHAR *) aszArgv[1]);
				if(ucPar)
				{
				    fgIsAuto = TRUE;
					mcSHOW_USER_MSG(("Set Auto Detect when scan!\n"));
				}
				else
				{
					fgIsAuto = FALSE;
					mcSHOW_USER_MSG(("Set Manual switch when scan!\n"));
				}
            }
         
            break;
        
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
                            mcSHOW_USER_MSG(("%02x ", ucData[ii*16+jj]));
                        mcSHOW_USER_MSG(("\n"));
                    }
                }            
            }               	
                    
            break;
#if 0
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
                             temp[jj] = ((CHAR *) aszArgv[1])[ii];
					         ii++;
						     jj++;
						   
                     	  }
                         if(jj >= 3)
						   {
                            mcSHOW_USER_MSG(("error!One word's format is ## \n")); 
							break;
						   }
					
						  RegistWord[kk] = (U16)StrToHex((CHAR *)&temp,jj);
						  mcSHOW_USER_MSG(("the input word is 0x%x\n",RegistWord[kk])); 
						  kk++;
					      jj = 0;
						  ii++;
					    }

				      //ITuner_OP(psDvbtDemodCtx->psTunerCtx, itSetReg,0, &RegistWord); 
				     
				      ITuner_OP( ITunerGetCtx(), itSetReg,0, &RegistWord); 
			          mcSHOW_USER_MSG(("Set Register complete!\n"));
                }
				
                 else if (i4Argc == 1)
                   	{     
                   	      for (ii = 0; ii < 5; ii++)
						  	RegistWord[ii] = 0;
                          //ITuner_OP(psDvbtDemodCtx->psTunerCtx, itGetReg,0, RegistWord); 
						  ITuner_OP(ITunerGetCtx(), itGetReg,0, RegistWord); 
                          for (ii = 0; ii < 5; ii++)
						  mcSHOW_USER_MSG(("the output word is 0x%x\n",RegistWord[ii])); 	
				    }
                   
			    else
			  	   mcSHOW_USER_MSG(("The CLI input formats are: nim.atd e or nim.atd e ##-##-##-##-## \n"));
			 }
                break;
#endif				
         case 't': 
				if (i4Argc ==2)
			    {
                   i4CurTop= (U8)StrToInt((CHAR *) aszArgv[1]);
				   //ITuner_OP(psDvbtDemodCtx->psTunerCtx, itSetTop, i4CurTop, &ucData1); 
				   PARAM_SETFREQ_T SetFreqPara;
				   SetFreqPara.Freq = (psDvbtDemodCtx->Frequency)/1000;
                   SetFreqPara.Modulation = MOD_DTMB;
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
#if 0
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
#endif

		 case 'r':
            if (i4Argc < 3)
                break;
            u1I2cAddr = (U8)StrToInt(aszArgv[1]);    
            u2RegAddr = (U16)StrToInt(aszArgv[2]);
            if (DTMB_ExDemodGetReg(psDvbtDemodCtx, u1I2cAddr, u2RegAddr, &ucPar, 1))
            {        
                mcSHOW_USER_MSG(("External Demod Get Register 0x%03X Fail!\n", u2RegAddr));
                break;
            }  
			else
			{
				mcSHOW_USER_MSG(("Register 0x%03X = 0x%02X\n", u2RegAddr, ucPar));
			}
            break;
#if 0
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
#endif

        case 'w':
            if (i4Argc < 4)
                break;
			
            u1I2cAddr = (U8)StrToInt(aszArgv[1]);
            u2RegAddr = (U16)StrToInt(aszArgv[2]);
            ucPar = (U8)StrToInt(aszArgv[3]);
            if (DTMB_ExDemodSetReg(psDvbtDemodCtx, u1I2cAddr, u2RegAddr, &ucPar, 1))
            {        
                mcSHOW_USER_MSG(("External Demod Set Register 0x%03X Fail!\n", u2RegAddr));
                break;
            }          
            mcSHOW_USER_MSG(("Set Register 0x%03X = 0x%02X, OK!\n", u2RegAddr, ucPar));
            break;
		  case 'g':
		  	vDTMBGetInfo(psDvbtDemodCtx);
		  	DVBT_GetPER(psDvbtDemodCtx);
			DTMB_GetSnrInfo(psDvbtDemodCtx);
			 
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
                DTMB_Connect(psDvbtDemodCtx, MT5391_BW_8MHz);
                mcSHOW_USER_MSG(("DVBT Connect!\n"));
            }          
            else
            {
                DTMB_DisConnect(psDvbtDemodCtx);
                mcSHOW_USER_MSG(("DVBT Disconnect!\n"));
            }
            break;
        default:
            break;
    }    

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG4(("DTMB_TunerDtdCommand unlock pPdCtx->hHalLock\n"));
    
	
}
/**********************************************************************/
void DTMB_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, 
                        INT32 i4Argc, const CHAR** aszArgv)
{
    //UCHAR   ucCmdId = 0;
    UCHAR   ucArg1;
    UCHAR   ucCmdSize = 0;
    UINT8 tunerType;
    UINT8 ucData[256];

	mcSHOW_DBG_MSG4(("DTMB_TunerCommand pPdCtx->hHalLock\n"));
    mcMUTEX_LOCK(pPdCtx->hHalLock);
	mcSHOW_DBG_MSG4(("escape DTMB_TunerCommand pPdCtx->hHalLock\n"));
    	    
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
            vDTMBGetPara(psDvbtDemodCtx);
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

#if 0    	
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
        break;
#endif

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

        mcSHOW_USER_MSG(("\r\n"));
        mcSHOW_USER_MSG(("\r\n"));
        break;
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerCommand unlock pPdCtx->hHalLock\n")); 

}

/**********************************************************************/
void DTMB_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
	//DTMB_TunerI2c(psDvbtDemodCtx, bDetach);
}

void *GetDigiDvbtTunerCtx(void)
{
    return psDvbtDemodCtx->psTunerCtx;
}

/**********************************************************************/
void  DTMB_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
	if(pPdCtx->fgDisStatus == TRUE)
	{
		/* Has Disconnected, return */
		mcSHOW_DBG_MSG(("DVBT Has Disconnect!\n"));
		return;
	}
    mcSHOW_DBG_MSG4(("DTMB_TunerDisc pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerDisc pPdCtx->hHalLock\n"));

/*    if (DVBT_StandbyMode(DEFAULT_I2C_ADDR))
    	mcSHOW_USER_MSG(("MT5133 standby mode FAIL!!\n"));
    else
    	mcSHOW_DRVAPI_MSG(("MT5133 enter standby mode\n"));
*/
    fgBreakMonitor = TRUE;
    DTMB_DisConnect(psDvbtDemodCtx);

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerDisc unlock pPdCtx->hHalLock\n"));

	/* set disconnect status */
	pPdCtx->fgDisStatus = TRUE;
	
    //fgIsDisconnected = TRUE;
    
}


INT16 DTMB_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type, VOID *pAnazAttribute, SIZE_T* pzAnaAttributeLen)
{
    TUNER_DIG_ATTRIBUTE_T *pAttri;
    INT32 temp;
    UINT16 u2Temp;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDvbtDemodCtx->psTunerCtx->specific_member.eu_ctx);

    mcSHOW_DBG_MSG4(("DTMB_TunerGetAttribute pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerGetAttribute pPdCtx->hHalLock\n")); 

    switch (e_get_type)
    {
    case TUNER_GET_TYPE_BER:
        //DVBT_UpdateInfo(psDvbtDemodCtx);   // Ken, for context update, best mux, 081217
        // JC, 090202, if context not updated, update here, 090202
        if (!fg1stMonitorRun)
        {
        	vDTMBGetInfo(psDvbtDemodCtx);
        	mcSHOW_DBG_MSG(("DTMB_TunerGetAttribute: TUNER_GET_TYPE_BER not enough time to update\n"));
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
          vDTMBGetInfo(psDvbtDemodCtx);
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
        default:
        {
            mcSHOW_DBG_MSG4(("DTMB_TunerGetAttribute: RMR_DRV_INV_GET_INFO!, type=%d\n", e_get_type));
            //return (RMR_DRV_INV_GET_INFO);
          	break;
        }
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerGetAttribute unlock pPdCtx->hHalLock\n")); 

    return (RMR_OK);
}


/***********************************************************************/
INT32 DTMB_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
    UCHAR ucData = *(UCHAR *)pvAnaAttribute;
    UCHAR *pucData = (UCHAR *)pvAnaAttribute;


    mcSHOW_DBG_MSG4(("DTMB_TunerSetAttribute pPdCtx->hHalLock\n")); 
    mcMUTEX_LOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("escape DTMB_TunerSetAttribute pPdCtx->hHalLock\n")); 


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
        mcSHOW_DBG_MSG(("DTMB_TunerSetAttribute: TUNER_SET_TYPE_CI_CARD_TYPE\n"));
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

        default:
            mcSHOW_DBG_MSG(("DTMB_TunerSetAttribute: RMR_DRV_INV_SET_INFO!\n"));
//            return (RMR_DRV_INV_SET_INFO);
            break;
    }

    mcMUTEX_UNLOCK(pPdCtx->hHalLock);
    mcSHOW_DBG_MSG4(("DTMB_TunerSetAttribute unlock pPdCtx->hHalLock\n")); 

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
void DTMB_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{	
    DTMB_TunerI2c(psDvbtDemodCtx, bSwitchOn);
}

/**********************************************************************/
void PD_DTMB_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
	TUNER_DEVICE_CTX_T*		pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
	
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose = DTMB_TunerClose;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq = DTMB_TunerAcq;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync = DTMB_TunerGetSync;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal = DTMB_TunerGetSignal;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel = DTMB_TunerGetSignalLevel;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = DTMB_TunerGetSignalLeveldBm;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER = DTMB_TunerGetSignalPER;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR = DTMB_TunerGetSignalSNR;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt = DTMB_TunerSetMpgFmt;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer = DTMB_TunerGetVer;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest = DTMB_TunerNimTest;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting = DTMB_TunerSetRegSetting;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting = DTMB_TunerShowRegSetting;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C = DTMB_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand = DTMB_TunerCommand;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand = DTMB_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C = DTMB_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc = DTMB_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute = DTMB_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute = DTMB_TunerGetAttribute;
	pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C = DTMB_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd = DTMB_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo = DTMB_TunerGetDiagInfo;
#endif
}
