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

#include "PI_Def_dbg.h"
#include "drvapi_tuner.h"
#include "TDIS.h"
#include "UtilAPI.h"
#include "tuner_if.h"
#include "TDMgr.h"
#include "aud_drvif.h"
#include "nptv_drvif.h"
#include "pd_tuner_type.h"
#include "ctrl_bus.h"

#if (fcTUNER_TYPE >= cTUNER_ALL_START)
STATIC UINT8 CheckTunerTypeFromEeprom(VOID);
#endif

GENERIC_LIST_ENTRY      TDCtxList;
BOOL                    TDMgrInited = 0;
/*---------------
*   Removing global variable bDetachMW & bTunerOpenFailure from drvapi_tuner.c to TDMgr.c
*   to solve involving x_rm_reg_comp() in drvapi_tuner.c when doing driver build.
*   reference CR: DTV00009782
-----------------*/
//extern BOOL             bDetachMW;
//extern BOOL             bTunerOpenFailure;
BOOL                    bDetachMW = 0;
BOOL                    bTunerOpenFailure = 0;

#if 0
BOOL                    LastConn = 0; /*For AUD DSP, switch from analog to digital*/
#endif

static UINT8  u1MWIntf = cTURNKEY_IF;

static SEMA_T                  uniTDCtxLock;

S32 GetTDCtx(UINT16 TunerID, TUNER_DEVICE_CTX_T** pptTDCtx)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    ptTDCtx = (TUNER_DEVICE_CTX_T*)QueueGetHead(&TDCtxList);
    // Find the corresponding TDCtx
    while (ptTDCtx){
        if (ptTDCtx->TunerID == TunerID){
            // The corresponding TDCtx exists.
            break;
        }
        ptTDCtx = (TUNER_DEVICE_CTX_T*)QueueGetNext(&TDCtxList, &ptTDCtx->Link);
    }
    *pptTDCtx = ptTDCtx;

    if (ptTDCtx){
        return (DRVAPI_TUNER_OK);
    }
    else{
        mcSHOW_DRVERR_MSG(("TDMgr can't find Tuner Device (ID=%d)\n", TunerID));
        mcSHOW_DRVERR_MSG(("Please make sure you have initialized Tuner Device (ID=%d)\n", TunerID));
        return (DRVAPI_TUNER_ERROR);
    }
}

///////////////////////////////////////////////////////////////////////////
// TunerSetMwIntf
// The API for setting u1MWIntface type
// 
///////////////////////////////////////////////////////////////////////////
void TunerSetMwIntf(UINT8 u1Value)
{
	u1MWIntf = u1Value;	
	return;
}


///////////////////////////////////////////////////////////////////////////
// TunerOpen
// If this function return failure, please call TunerClose to release all
// resources.
///////////////////////////////////////////////////////////////////////////
S32 TunerOpen(void)
{
    INT32                   res;
    TUNER_CONFIG_TBL_T      tTunerCfgTbl;
    INT32                   i;
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    UINT8                   _u1TunerType;
    
#if fcINTERNAL_TEST || 1
//    u1DbgLevel = 2 | mcBIT(cpMSG_MONI);
#endif

    if (TDMgrInited){
        mcSHOW_DRVERR_MSG(("TDMgr had been initialized!\n"));
        return (TDMGR_INITIALIZED);
    }
    // Retrieve Tuner Configuration Info from EEPROM/FLASH
    res = TDIS_RetrieveTunerCfgTbl(&tTunerCfgTbl);
    if ((res != DRVAPI_TUNER_OK) ||(tTunerCfgTbl.TunerNum>MAX_TUNER_NUM)) {
        Printf("Fail to retrieve tuner config table.\n");
        return (DRVAPI_TUNER_ERROR);
    }

    // Create a semaphore for protecting TDCtxes exclusively accessing the same H/W (e.g. TunerAcq)
    if (x_sema_create(&uniTDCtxLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        Printf("%s: x_sema_create(&uniTDCtxLock) Fail!\n", __FUNCTION__);
        return (DRVAPI_TUNER_ERROR);
    }
    x_sema_unlock(uniTDCtxLock);

    // Initialize TDCtxList
    QueueInitList(&TDCtxList);

    // Initialize ICtrlBus
    res = ICtrlBus_Init();
    if (res == DRVAPI_TUNER_ERROR)
    {
        return (DRVAPI_TUNER_ERROR);
    }

    #if (fcTUNER_TYPE >= cTUNER_ALL_START)
        _u1TunerType = CheckTunerTypeFromEeprom();
    #else
        _u1TunerType = fcTUNER_TYPE;
    #endif    	
    	
    // Allocate/Initialize resources for each Tuner Device
    for (i=0;i<tTunerCfgTbl.TunerNum;i++){
        /**************************************************************/
        /*  Allocate TDCtx and Link to TDCtxList                      */
        /**************************************************************/
        ptTDCtx = (TUNER_DEVICE_CTX_T*)x_mem_calloc(1, sizeof(TUNER_DEVICE_CTX_T));
        if (ptTDCtx == NULL){
            goto ERROR_EXIT;
        }

        /*************************************************************/
        /*  Initialize TDCtx                                         */
        /*************************************************************/
        // Save TunerID in TunerCtx
        ptTDCtx->TunerID = tTunerCfgTbl.tTunerCfg[i].TunerID;
        // Save Tuner's CfgInfo in TunerCtx
        x_memcpy(&ptTDCtx->tTunerCfg, &tTunerCfgTbl.tTunerCfg[i],
                sizeof(TUNER_CONFIG_T));

        ptTDCtx->tunerType = _u1TunerType;
		
        // Get LTDIS open function and save in TDCtx
        res = LTDIS_GetOpenFct(ptTDCtx->TunerID, &ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerOpen);
        if (res != DRVAPI_TUNER_OK){
            x_mem_free(ptTDCtx);
            goto ERROR_EXIT;
        }

        // Call LTDIS's TunerOpen, TDD will register other LTDIS functions.
        ptTDCtx->u1MWIntf = u1MWIntf;
        res = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerOpen(ptTDCtx, &ptTDCtx->tTunerCfg,
                                                &ptTDCtx->pTDSpecificCtx);
        if (res != DRVAPI_TUNER_OK){
            x_mem_free(ptTDCtx);
            goto ERROR_EXIT;
        }

        // Show the version of tuner device driver
        if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer != NULL){
            mcSHOW_DRVAPI_MSG(("%s\n", ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer(ptTDCtx->pTDSpecificCtx)));
        }

        // Initialize RefSema
        if (mcSEMA_CREATE(&ptTDCtx->RefSema, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            mcSHOW_DRVERR_MSG(("TunerOpen (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
            x_mem_free(ptTDCtx);
            goto ERROR_EXIT;
        }
        mcSEMA_UNLOCK(ptTDCtx->RefSema);
        
        // Initialize pUniTDCtxLock
        ptTDCtx->pUniTDCtxLock = &uniTDCtxLock;

        // All done, Link TunerCtx to TunerCtxList
        QueuePutTail(&TDCtxList, &ptTDCtx->Link);
    }

    TDMgrInited = 1;
    return (DRVAPI_TUNER_OK);

ERROR_EXIT:
    while (QueueGetHead(&TDCtxList)){
        // Remove from TDCtxList to prevent future possible calls of
        // exported APIs.
        ptTDCtx = (TUNER_DEVICE_CTX_T*)QueuePopHead(&TDCtxList);

        // Make sure no exported API is under-process
        if (ptTDCtx->RefSema){
            while (ptTDCtx->RefCnt){
                mcSHOW_DRVERR_MSG(("Wait ptTDCtx->RefCnt to be zero\n"));
                mcDELAY_MS(10);    // in ms
            }
        }

        // Destroy RefSema
        if (ptTDCtx->RefSema){
            mcSEMA_DESTROY(ptTDCtx->RefSema);
        }

        // Call LTDIS's TunerClose
        if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerClose != NULL){
            ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerClose(ptTDCtx->pTDSpecificCtx);
        }

        // Free TDCtx
        x_mem_free(ptTDCtx);
    } // End of while(QueueGetHead(&TDCtxList))

    return (DRVAPI_TUNER_ERROR);

}

S32 TunerClose(void)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (!TDMgrInited){
        mcSHOW_DRVERR_MSG(("TDMgr had NOT been initialized!\n"));
        return(DRVAPI_TUNER_ERROR);
    }

    while (QueueGetHead(&TDCtxList)){
        // Remove from TDCtxList to prevent future possible calls of
        // exported APIs.
        ptTDCtx = (TUNER_DEVICE_CTX_T*)QueuePopHead(&TDCtxList);

        // Make sure no exported API is under-process
        if (ptTDCtx->RefSema){
            while (ptTDCtx->RefCnt){
                mcSHOW_DRVERR_MSG(("Wait ptTDCtx->RefCnt to be zero\n"));
                mcDELAY_MS(10);    // in ms
            }
        }

        // Destroy RefSema
        if (ptTDCtx->RefSema){
            mcSEMA_DESTROY(ptTDCtx->RefSema);
        }

        // Call LTDIS's TunerClose
        if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerClose != NULL){
            ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerClose(ptTDCtx->pTDSpecificCtx);
        }

        // Free TDCtx
        x_mem_free(ptTDCtx);
    } // End of while(QueueGetHead(&TDCtxList))

    TDMgrInited = 0;
    return (DRVAPI_TUNER_OK);
}

#define INFO_CI_CONN_TYPE
#define SUPPORT_OUTPUT_CHANNELSCAN_TIME      0
#include "pcmcia_if.h"

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
extern UINT32 os_get_sys_tick(void);
extern UINT32 HalCriticalStart(void);
extern void HalCriticalEnd(UINT32 u4Flags);
#endif

BOOL TunerAcq(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type,
            void* pv_conn_info, SIZE_T z_conn_info_len,
            x_break_fct _BreakFct, void* pvArg)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    TUNER_DEVICE_CTX_T*     ptExclTDCtx;
    BOOL                    fgRetSts = FALSE;
    static UINT16           prev_TunerID = 0xffff;
    static DRV_CONN_TYPE_T  prev_e_conn_type = 0xffff;
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
	  UINT16          u2AcqTime_Test;
	  UINT32          u4TickCnt_Start;
	  UINT32          u4TickCnt_Test; 
	  UINT32          u4Flag;
	  u4TickCnt_Start = os_get_sys_tick();
#endif

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
    u4Flag=HalCriticalStart();
#endif
    if (bTunerOpenFailure)
    { // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return fgRetSts;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK)
    {
        return fgRetSts;
    }
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
    u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
    mcSHOW_DRVAPI_MSG(("Tuner Acq GetTDCtx time = %d ms\n", u2AcqTime_Test));
	  HalCriticalEnd(u4Flag);
#endif

#if 0 /*2009.6.25 Maggie: Discussed with AUD/Shawn, remove this after new TargetCountry format*/
    if (COUNTRY_EU == GetTargetCountry())
    {
#ifndef CC_MT5391_AUD_3_DECODER    	
        /*For aud dsp, analog switch to digital*/
        if(LastConn == MTK_ANALOG_0 && TunerID == MTK_MT5111_0){
    		AUD_WaitDspFlush(1);	
        }
#endif
        LastConn  = TunerID;
    }
#endif

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
    u4Flag=HalCriticalStart();
#endif
    if ((ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN) &&
        (e_conn_type != prev_e_conn_type))
    {
    #if 1   // To fix bug: DTV00141978
        mcSHOW_DRVAPI_MSG(("[Func TunerAcq] Connect type changed, disconnect previous connect type\r\n"));
        TunerDisc(prev_TunerID, prev_e_conn_type);
    #else
        if(TunerID != MTK_ANALOG_0)
        { /*acq digital, disconnect analog*/
            TunerDisc(MTK_ANALOG_0, TUNER_CONN_TYPE_TER_ANA);
            mcSHOW_DRVAPI_MSG(("[Func TunerAcq] Acq digital, disconnect analog\r\n"));
        }
        else
        {       /*acq analog, disconnect digital*/
            TunerDisc(MTK_MT5111_0, TUNER_CONN_TYPE_TER_DIG);
            TunerDisc(MTK_MT5111_0, TUNER_CONN_TYPE_CAB_DIG);
            mcSHOW_DRVAPI_MSG(("[Func TunerAcq] Acq analog, disconnect digital\r\n"));
        }
    #endif
    }
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
    mcSHOW_DRVAPI_MSG(("Tuner Acq TunerDisc time = %d ms\n", u2AcqTime_Test));
	  HalCriticalEnd(u4Flag);
#endif
      
    Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME    
		u4Flag=HalCriticalStart();
#endif
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN)
    {
        // wait semaphore
        VERIFY(x_sema_lock(*ptTDCtx->pUniTDCtxLock, X_SEMA_OPTION_WAIT) == OSR_OK);

        // Connection Exclusive
        if (GetTDCtx(ptTDCtx->tTunerCfg.ConnExclusiveID, &ptExclTDCtx)==DRVAPI_TUNER_OK)
        {
            // Set ptExclTDCtx->I2C_Detached first, and then invoke TunerDetachI2C
            ptExclTDCtx->I2C_Detached = TRUE;
            TunerDetachI2C(ptTDCtx->tTunerCfg.ConnExclusiveID, 1);
            ptTDCtx->I2C_Detached = FALSE;
        }
        else
        {
            mcSHOW_DRVERR_MSG(("ERROR: Can't get TDCtx of TunerID(0x%x)\n",
                    ptTDCtx->tTunerCfg.ConnExclusiveID));
        }
    }
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME 
		u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
    mcSHOW_DRVAPI_MSG(("Tuner Acq Connection Exclusive = %d ms\n", u2AcqTime_Test));
	  HalCriticalEnd(u4Flag);
#endif

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME		
		u4Flag=HalCriticalStart();
#endif
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerAcq != NULL)
    {
        fgRetSts = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerAcq(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type,
                                            pv_conn_info,
                                            z_conn_info_len,
                                            _BreakFct,
                                            pvArg);
    }
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
    u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
    mcSHOW_DRVAPI_MSG(("Tuner Acq TunerAcq time = %d ms\n", u2AcqTime_Test));
	  HalCriticalEnd(u4Flag);
#endif

#ifdef INFO_CI_CONN_TYPE
	INT32 is_dvbc = (e_conn_type==TUNER_CONN_TYPE_CAB_DIG) ? 1 : 0;
	PCMCIA_SetTsType(is_dvbc);
#endif

#if SUPPORT_OUTPUT_CHANNELSCAN_TIME		
		u4Flag=HalCriticalStart();
#endif		        
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN)
    {
        // release semaphore
        VERIFY(x_sema_unlock(*ptTDCtx->pUniTDCtxLock) == OSR_OK);
    }
    
    Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
#if SUPPORT_OUTPUT_CHANNELSCAN_TIME
    u4TickCnt_Test = os_get_sys_tick()-u4TickCnt_Start;
    u2AcqTime_Test = u4TickCnt_Test * x_os_drv_get_tick_period();
    mcSHOW_DRVAPI_MSG(("Tuner Acq Release Semaphore time  = %d ms\n", u2AcqTime_Test));
		HalCriticalEnd(u4Flag);
#endif

    prev_TunerID = TunerID;
    prev_e_conn_type = e_conn_type;
    return fgRetSts;
}

INT16 TunerGetSync(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    S16                     ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        //Printf("## TunerOpenFailure ##\n");
        return 1;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return 0;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DBG_MSG4(("TunerGetSync TunerID(0x%x) I2C_Detached\n", TunerID));
            return 0;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return 0;
}

void TunerGetSignal(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type,
                    SIGNAL *_pSignal)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return ;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerGetSignal TunerID(0x%x) I2C_Detached\n", TunerID));
            return;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type,
                                            _pSignal);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
    return;
}

void TunerDisc(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return ;
    }
/* //don't do software I2C_Detached check when do TunerDisc
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerDisc TunerID(0x%x) I2C_Detached\n", TunerID));
            return;
        }
    }
*/
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDisc != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDisc(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
    return;
}


void TunerDiscAllInb(void)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (!TDMgrInited){
        mcSHOW_DRVERR_MSG(("TDMgr has NOT been initialized! Skip TunerDiscAllInb\n"));
        return;
    }

    // TunerDiscAllInb is for DCR, we must remark the following, so that MT5112 can be disc-ed
    // successfully.
    /*
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerID(0x%x) I2C_Detached\n", TunerID));
            return;
        }
    }
    */

    mcSHOW_DRVAPI_MSG(("TunerDiscAllInb invokd, this means POD driver want to switch TS to cablecardn"));
    ptTDCtx = (TUNER_DEVICE_CTX_T*)QueueGetHead(&TDCtxList);
    // Find the corresponding TDCtx
    while (ptTDCtx){
        if (ptTDCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_DIG){
            if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDisc != NULL){
                Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
                // Disconnect TUNER_CONN_TYPE_CAB_DIG
                ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDisc(ptTDCtx->pTDSpecificCtx,
                                                    TUNER_CONN_TYPE_CAB_DIG);
                // Disconnect TUNER_CONN_TYPE_TER_DIG
                ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDisc(ptTDCtx->pTDSpecificCtx,
                                                    TUNER_CONN_TYPE_TER_DIG);
                Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
            }
        }
        // Get next ptTDCtx
        ptTDCtx = (TUNER_DEVICE_CTX_T*)QueueGetNext(&TDCtxList, &ptTDCtx->Link);
    }
}

U8 TunerGetSignalLevel(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    U8                      ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return 0;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return 0;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerGetSignalLevel TunerID(0x%x) I2C_Detached\n", TunerID));
            return 0;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return 0;
}

INT16 TunerGetSignalLeveldBm(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    S16                     ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return 0;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return 0;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerGetSignalLeveldBm TunerID(0x%x) I2C_Detached\n", TunerID));
            return 0;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return 0;
}

INT32 TunerSetAttribute(UINT16 TunerID,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    INT32                     ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return RMR_DRV_ERROR;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return RMR_DRV_ERROR;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            // Skip I2C_Detached check for SDAL to set TUNE_FREQ_ONLY before TunerAcq.
            // I2C access may fail in this function.
            mcSHOW_DRVERR_MSG(("TunerSetAttribute TunerID(0x%x) I2C_Detached, Still pass-through\n", TunerID));
        //    return NULL;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute(ptTDCtx->pTDSpecificCtx,
                                            e_set_type, pvAnaAttribute, zAnaAttributeLen);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return RMR_DRV_ERROR;
}

INT16 TunerGetAttribute(UINT16 TunerID,
                           DRV_GET_TYPE_T  e_get_type,
                           void *pvAnaAttribute,
                           SIZE_T* pzAnaAttributeLen)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    INT16                     ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return RMR_DRV_ERROR;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return RMR_DRV_ERROR;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            // Skip I2C_Detached check due to request from SSD/Dainel and CID/SA/James.
            // They need to assure that there will be no I2C access in this get function.
            mcSHOW_DRVERR_MSG(("TunerGetAttribute TunerID(0x%x) I2C_Detached, But still pass for TunerGetAttribute\n", TunerID));
            /*
            mcSHOW_DRVERR_MSG(("TunerID(0x%x) I2C_Detached\n", TunerID));
            return NULL;
            */
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute(ptTDCtx->pTDSpecificCtx,
                                            e_get_type, pvAnaAttribute, pzAnaAttributeLen);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return RMR_DRV_ERROR;
}

#ifdef fcADD_DIAG_INFO
INT32 TunerSetDiagCmd(UINT16 TunerID,
                        DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
                        VOID*                           pv_set_info,
                        SIZE_T                          z_size)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    INT32                   ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return RMR_DRV_ERROR;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return RMR_DRV_ERROR;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            // They need to assure that there will be no I2C access in this get function.
            mcSHOW_DRVERR_MSG(("TunerID(0x%x) I2C_Detached, But still pass for TunerSetDiagCmd\n", TunerID));
            /*
            mcSHOW_DRVERR_MSG(("TunerID(0x%x) I2C_Detached\n", TunerID));
            return NULL;
            */
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd(ptTDCtx->pTDSpecificCtx,
                                            e_tuner_set_type, pv_set_info, z_size);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return RMR_DRV_ERROR;
}

INT32 TunerGetDiagInfo(UINT16 TunerID,
                        DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
                        VOID*                           pv_get_info,
                        SIZE_T*                         pz_size)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    INT32                   ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return RMR_DRV_NO_GET_INFO;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return RMR_DRV_NO_GET_INFO;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            // They need to assure that there will be no I2C access in this get function.
            mcSHOW_DRVERR_MSG(("TunerID(0x%x) I2C_Detached, But still pass for TunerGetDiagInfo\n", TunerID));
            /*
            mcSHOW_DRVERR_MSG(("TunerID(0x%x) I2C_Detached\n", TunerID));
            return NULL;
            */
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo(ptTDCtx->pTDSpecificCtx,
                                            e_tuner_get_type, pv_get_info, pz_size);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return RMR_DRV_NO_GET_INFO;
}
#endif

UINT16 TunerGetSignalPER(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    UINT16                  ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return 0;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return 0;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerGetSignalPER TunerID(0x%x) I2C_Detached\n", TunerID));
            return 0;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return 0;
}

UINT16 TunerGetSignalSNR(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    UINT16                  ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return 0;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return 0;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerGetSignalSNR TunerID(0x%x) I2C_Detached\n", TunerID));
            return 0;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR(ptTDCtx->pTDSpecificCtx,
                                            e_conn_type);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return 0;
}

void TunerSetMpgFmt(UINT16 TunerID, MPEG_FMT_T  *pt_mpeg_fmt)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return ;
    }
    
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerSetMpgFmt TunerID(0x%x) I2C_Detached\n", TunerID));
            return;
        }
    }
    
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt(ptTDCtx->pTDSpecificCtx,
                                            pt_mpeg_fmt);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
}

void TunerGetTsFmt(void  *pvTsFmt)
{
    SIZE_T u4TsFmtLen;
    TS_FMT_T  *pt_ts_fmt = (TS_FMT_T *)pvTsFmt;
    
    if (bTunerOpenFailure)
    {
        Printf("## TunerOpenFailure ##\n");
    return;
}

    // When the PD/PI not implement the TUNER_GET_TYPE_TS_FMT API, return UNKNOWN.
    pt_ts_fmt->SerOrPar = SP_UNKNOWN;
    pt_ts_fmt->ClockPolarity = POL_UNKNOWN_ACTIVE;
    
    TunerGetAttribute(MTK_DIGITAL_0, TUNER_GET_TYPE_TS_FMT, (void *)pt_ts_fmt, &u4TsFmtLen);
}

void TunerSetTsSP(UCHAR  ucTsSP)
{
    SIG_SP_T  e_ts_sp = (SIG_SP_T)ucTsSP;
    if (bTunerOpenFailure)
    {
        Printf("## TunerOpenFailure ##\n");
        return;
    }
    
    TunerSetAttribute(MTK_DIGITAL_0, TUNER_SET_TYPE_TS_SP, (void *)&e_ts_sp, sizeof(SIG_SP_T));
}

void TunerSetTsTristate(UCHAR  ucTristate)
{
    BOOL_T  e_ts_tristate = (BOOL_T)ucTristate;
    if (bTunerOpenFailure)
    {
        Printf("## TunerOpenFailure ##\n");
        return;
    }
    
    TunerSetAttribute(MTK_DIGITAL_0, TUNER_SET_TYPE_TS_TRISTATE, (void *)&e_ts_tristate, sizeof(BOOL_T));
}

void TunerSetCiCardType(UCHAR *pucAttri)
{
    if (bTunerOpenFailure)
    {
        Printf("## TunerOpenFailure ##\n");
        return;
    }
    
    TunerSetAttribute(MTK_DIGITAL_0, TUNER_SET_TYPE_CI_CARD_TYPE, (void *)pucAttri, 2);
}

char* TunerGetVer(UINT16 TunerID)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    char*                   ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return NULL;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return NULL;
    }

    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN)
    {
        if (ptTDCtx->I2C_Detached)
        {
            mcSHOW_DRVERR_MSG(("TunerGetVer TunerID(0x%x) I2C_Detached\n", TunerID));
        //    return NULL;
        }
    }

    // Print TUNER_SOURCE_LABEL
    Printf(TUNER_SOURCE_LABEL);

    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer(ptTDCtx->pTDSpecificCtx);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return NULL;
}

void TunerDetachI2C(UINT16 TunerID, BOOL bDetach)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return;
    }

    Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C != NULL){
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C(ptTDCtx->pTDSpecificCtx, bDetach);
    }
    Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);

    return;
}

//------------------------------------------------------------------------- 
/*
 *  TunerBypassI2C
 *  To control tunerBypass switch ON or OFF
 *  @param  TunerID      tunerID
 *  @param  bSwitchOn    TRUE(TunerBypass ON) or FALSE(TunerBypass OFF)
 *  @retval void
 */
//-------------------------------------------------------------------------
void TunerBypassI2C(UINT16 TunerID, BOOL bSwitchOn)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return;
    }

    Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C != NULL){
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C(ptTDCtx->pTDSpecificCtx, bSwitchOn);
    }
    Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);

    return;
}

void TunerDetachMW(UINT16 TunerID, BOOL bDetach)
{
    bDetachMW = bDetach;
}


INT32 TunerNimTest(UINT16 TunerID)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    S32                     ret;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return DRVAPI_TUNER_ERROR;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        Printf("## GetTDCtx fail ##\n");
        return DRVAPI_TUNER_ERROR;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            Printf("TunerNimTest TunerID(0x%x) I2C_Detached\n", TunerID);
            return DRVAPI_TUNER_ERROR;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ret = ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest(ptTDCtx->pTDSpecificCtx);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        return ret;
    }
    return DRVAPI_TUNER_ERROR;
}

void TunerCommand(UINT16 TunerID, INT32 i4Argc, const CHAR** aszArgv)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerCommand TunerID(0x%x) I2C_Detached\n", TunerID));
//            return;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerCommand != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerCommand(ptTDCtx->pTDSpecificCtx, i4Argc,
                                                    aszArgv);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
}

void TunerDtdCommand(UINT16 TunerID, INT32 i4Argc, const CHAR** aszArgv)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    
    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerDtdCommand TunerID(0x%x) I2C_Detached\n", TunerID));
//            return;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand(ptTDCtx->pTDSpecificCtx, i4Argc,
                                                    aszArgv);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
}

void TunerAtdCommand(UINT16 TunerID, INT32 i4Argc, const CHAR** aszArgv)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;
    
    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerAtdCommand TunerID(0x%x) I2C_Detached\n", TunerID));
//            return;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerAtdCommand != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerAtdCommand(ptTDCtx->pTDSpecificCtx, i4Argc,
                                                    aszArgv);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
}

void TunerSetRegSetting(UINT16 TunerID, UCHAR ucRegSet, UCHAR ucRegAddr,
                        UCHAR ucRegValue)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return ;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerSetRegSetting TunerID(0x%x) I2C_Detached\n", TunerID));
            return;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting(ptTDCtx->pTDSpecificCtx,
                                            ucRegSet,
                                            ucRegAddr,
                                            ucRegValue);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
    return;
}

void TunerShowRegSetting(UINT16 TunerID)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return ;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerShowRegSetting TunerID(0x%x) I2C_Detached\n", TunerID));
            return;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting(ptTDCtx->pTDSpecificCtx);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
    return;
}

void TunerTestI2C(UINT16 TunerID)
{
    TUNER_DEVICE_CTX_T*     ptTDCtx;

    if (bTunerOpenFailure){ // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return;
    }

    if (GetTDCtx(TunerID, &ptTDCtx)!=DRVAPI_TUNER_OK){
        return ;
    }
    if (ptTDCtx->tTunerCfg.i2TunerCapability & cCONN_EXCLUSIVE_EN){
        if (ptTDCtx->I2C_Detached){
            mcSHOW_DRVERR_MSG(("TunerTestI2C TunerID(0x%x) I2C_Detached\n", TunerID));
            return;
        }
    }
    if (ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C != NULL){
        Util_InterlockedIncrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
        ptTDCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C(ptTDCtx->pTDSpecificCtx);
        Util_InterlockedDecrement(ptTDCtx->RefSema, &ptTDCtx->RefCnt);
    }
    return;
}

S32 TunerRegRead(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, UCHAR ucRegAddr, UCHAR *pucBuffer,
                    U16 ucByteCount)
{
    return 0;
}

/**********************************************************************/
void *TunerGetCtx(UINT16 TunerID)
{
TUNER_DEVICE_CTX_T  *ptTDCtx;

    if (bTunerOpenFailure)
    {                                             // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return (void *) NULL;
    }

    if (GetTDCtx(TunerID, &ptTDCtx) != DRVAPI_TUNER_OK)
    {
        return (void *) NULL;
    }
    return (void *) ptTDCtx->pTDSpecificCtx;
}

void *TunerGetAnaCtx(void)
{
    return TunerGetCtx(MTK_ANALOG_0);
}

//------------------------------------------------------------------------- 
/*
 *  DigTunerBypassI2C
 *  To control digital demod tunerBypass switch ON or OFF
 *  @param  bSwitchOn    TRUE(TunerBypass ON) or FALSE(TunerBypass OFF)
 *  @retval void
 */
//-------------------------------------------------------------------------
void DigTunerBypassI2C(BOOL bSwitchOn)
{
    TunerBypassI2C(MTK_DTD_0, bSwitchOn);
}

/**********************************************************************/
#ifdef TUNER_SET_TYPE_SMART_ANTENNA_PARS
INT32 TunerSetSmartAntenna(PSMART_ANTENNA_PARS_T pt_smart_ant)
{
    if (bTunerOpenFailure)
    {                                             // Enter TunerDummy mode
        Printf("## TunerOpenFailure ##\n");
        return NULL;
    }

    TunerSetAttribute(MTK_MT5112_0, TUNER_SET_TYPE_SMART_ANTENNA_PARS,
                      pt_smart_ant, sizeof(SMART_ANTENNA_PARS_T));
    return 0;
}
#endif

#ifdef fcADD_DIAG_INFO
INT32 d_custom_tuner_set (
    DRV_CUSTOM_TUNER_TYPE_T         e_tuner_type,
    DRV_CUSTOM_TUNER_SET_TYPE_T     e_tuner_set_type,
    VOID*                           pv_set_info,
    SIZE_T                          z_size)
{
    INT32           retSts;
    UINT16          tuner_id;

    // Check input parameters
    if(pv_set_info == NULL)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch(e_tuner_type){
        case DRV_CUSTOM_TUNER_TYPE_ANALOG:
            tuner_id = 0;
            break;
        case DRV_CUSTOM_TUNER_TYPE_DIGITAL:
            tuner_id = 1;
            break;
        default:
            mcSHOW_DRVERR_MSG(("Error e_tuner_type in d_custom_tuner_set\n"));
            tuner_id = 1;
            break;
    }

    retSts = TunerSetDiagCmd(tuner_id, e_tuner_set_type, pv_set_info, z_size);
    return (retSts);
}

INT32 d_custom_tuner_get (
    DRV_CUSTOM_TUNER_TYPE_T         e_tuner_type,
    DRV_CUSTOM_TUNER_GET_TYPE_T     e_tuner_get_type,
    VOID*                           pv_get_info,
    SIZE_T*                         pz_size)
{
    INT32           retSts;
    UINT16          tuner_id;

    // Check input parameters
    if((pv_get_info == NULL) || (pz_size == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    switch(e_tuner_type){
        case DRV_CUSTOM_TUNER_TYPE_ANALOG:
            tuner_id = 0;
            break;
        case DRV_CUSTOM_TUNER_TYPE_DIGITAL:
            tuner_id = 1;
            break;
        default:
            mcSHOW_DRVERR_MSG(("Error e_tuner_type in d_custom_tuner_get\n"));
            tuner_id = 1;
            break;
    }

    retSts = TunerGetDiagInfo(tuner_id, e_tuner_get_type, pv_get_info, pz_size);
    return (retSts);
}
#endif
#if fcTUNER_TYPE >= cTUNER_ALL_START
//------------------------------------------------------------------------- 
/*
 *  CheckValidTunerTypeByE2pVal
 *  translate from EEPROM value to tuner type
 */
//-------------------------------------------------------------------------

STATIC void TransTunerTypeByE2pVal(UINT8 *u1Val)
{

#if defined(MT5387DTD_P_ANALOG)  
/* translate from EEPROM value to tuner type */
    u1Val[3] = cALPS_TDAU4XB02A;
#elif defined(DTDDUMMY_P_UATD)
    u1Val[3] = cALPS_TEQH3L01A;
#endif

}
//------------------------------------------------------------------------- 
/*
 *  CheckValidTunerType
 *  check whether tuner Type is valid
 *  @param  u1TestTunerType   TunerType for test
 *  @retval TRUE    valid tuner type   
 *  @retval FALSE    invalid tuner type
 */
//-------------------------------------------------------------------------
STATIC BOOL CheckValidTunerType(UINT8 data)
{
    BOOL fgRet = FALSE;

//#if defined(MT5387_DTD_P_ANALOG)
    if ((data >=  cITUNER_TYPE_START  ) && (data <=   cITUNER_TYPE_END))
    {  
        fgRet = TRUE;
    }
//#endif

    return fgRet;
}

//------------------------------------------------------------------------- 
/*
 *  CheckTunerTypeFromEeprom
 *  1. Tuner id exist in three additional EEPROM address for check
 *  2. Judgement:
 *  Tuner ID invalid, ex. 0xff -> Use the default Tuner, 
 *  Three address are with the same valid Tuner ID -> Trust the tuner ID
 *  Two address are with the same valid Tuner ID -> Trust the tuner ID
 *  All address are with different Tuner ID -> Use the default Tuner
 *  @param  void
 *  @retval tunerid    return tuner id depend on the matched judgement rule
 */
//-------------------------------------------------------------------------
STATIC UINT8 CheckTunerTypeFromEeprom(VOID)
{
    UINT8  u1Val[4]={0};//initialize u1Val to fix Klocwork warning

    UINT8  u1ResultType;
    UINT32 u4Ofst[3];
    UINT32 u4Default;

    I2cPinmuxInit();

    /* Read three offset value and default tuner type from drv_cust */
    u4Ofst[0] = DRVCUST_OptGet(eTunerTypeOfst0);
    u4Ofst[1] = DRVCUST_OptGet(eTunerTypeOfst1);
    u4Ofst[2] = DRVCUST_OptGet(eTunerTypeOfst2);
    u4Default = DRVCUST_OptGet(eTunerTypeDefault);
    mcSHOW_USER_MSG(("EEPROM(tuner): Ofst0=0x%x,Ofst1=0x%x,Ofst2=0x%x,def=0x%x\r\n",u4Ofst[0], u4Ofst[1], u4Ofst[2], u4Default));

    /* Read three tuner IDs from EEPROM offset */
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[0], (UINT32)(void *)&u1Val[0], 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[1], (UINT32)(void *)&u1Val[1], 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[2], (UINT32)(void *)&u1Val[2], 1));
    mcSHOW_USER_MSG(("Read TunerType from EEPROM: addr(0x%x)=0x%x,addr(0x%x)=0x%x,addr(0x%x)=0x%x\r\n",u4Ofst[0],u1Val[0],u4Ofst[1],u1Val[1],u4Ofst[2],u1Val[2]));

    TransTunerTypeByE2pVal(u1Val);

    if (CheckValidTunerType(u4Default) == TRUE)
        u1Val[3] = u4Default;

    /* If two of tuner types in eeprom are the same and are valid tuner type, then trust this value, other condition return default type */
    if ((CheckValidTunerType(u1Val[0]) == TRUE) && (u1Val[0]==u1Val[1])){
        u1ResultType = u1Val[0];
    }
    else if((CheckValidTunerType(u1Val[0]) == TRUE) && (u1Val[0]==u1Val[2])){
        u1ResultType = u1Val[0];
    }
    else if((CheckValidTunerType(u1Val[1]) == TRUE) && (u1Val[1]==u1Val[2])){
        u1ResultType = u1Val[1];
    }
    else{
        //Return default tuner type
        u1ResultType = u1Val[3];
    }
    mcSHOW_DBG_MSG(("u1ResultType=0x%x\r\n", u1ResultType));

    return u1ResultType;
}

#endif

