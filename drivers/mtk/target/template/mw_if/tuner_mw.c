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

#ifndef _TUNER_MW_C_
#define _TUNER_MW_C_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "PD_Def.h"
#include "drvapi_tuner.h"
#include "x_rm_dev_types.h"
/*-----------------------------------------------------------------------------
                    variables definitions
 ----------------------------------------------------------------------------*/
extern BOOL TunerInited;
extern THREAD_T		t_init_thread;
extern char		init_thread_name[];
extern DRV_COMP_FCT_TBL_T t_tuner_fct_tbl;
extern UINT32		gInit_Start_Time;
extern BOOL     	TDD_Ready;
extern SEMA_T		t_init_sync;
extern void InitThreadProc(VOID *pvArg);
extern GENERIC_LIST_ENTRY      TunerCtxList;

#define MAX_MUTUAL_EXCL_COMP_REG_NUM            8
static DRV_COMP_REG_T   MutualExclCompReg_T[MAX_MUTUAL_EXCL_COMP_REG_NUM];


INT32 InitializeStateInfo(TUNER_CTX_T* ptTunerCtx, STATE_INFO_T* ptStateInfo,
                            DRV_TYPE_T DrvType)
{
    INT32   res;

    ptStateInfo->ptParentTunerCtx = (VOID*)ptTunerCtx;
    ptStateInfo->t_tuner_comp_id.e_type = DrvType;
    ptStateInfo->t_tuner_comp_id.ui2_id = ptTunerCtx->TunerID;
    ptStateInfo->t_tuner_comp_reg.e_type = DrvType;
    ptStateInfo->t_tuner_comp_reg.e_id_type = ID_TYPE_IND;
    ptStateInfo->t_tuner_comp_reg.u.t_ind.ui2_id = ptTunerCtx->TunerID;
    switch(DrvType){
        case DRVT_TUNER_CAB_DIG:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_CAB_DIG;
                sprintf(ptStateInfo->comp_name, "Comp_%d_CAB", ptTunerCtx->TunerID);
                sprintf(ptStateInfo->thread_name, "TU%d_CAB_Thread", ptTunerCtx->TunerID);
                break;
        case DRVT_TUNER_TER_DIG:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_TER_DIG;
                sprintf(ptStateInfo->comp_name, "Comp_%d_TER", ptTunerCtx->TunerID);
                sprintf(ptStateInfo->thread_name, "TU%d_TER_Thread", ptTunerCtx->TunerID);
                break;
        case DRVT_TUNER_TER_ANA:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_TER_ANA;
                sprintf(ptStateInfo->comp_name, "Comp_%d_TER_ANA", ptTunerCtx->TunerID);
                sprintf(ptStateInfo->thread_name, "TU%d_TER_ANA_Thread", ptTunerCtx->TunerID);
                break;
        case DRVT_TUNER_CAB_ANA:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_CAB_ANA;
                sprintf(ptStateInfo->comp_name, "Comp_%d_CAB_ANA", ptTunerCtx->TunerID);
                sprintf(ptStateInfo->thread_name, "TU%d_CAB_ANA_Thread", ptTunerCtx->TunerID);
                break;
        case DRVT_TUNER_CAB_DIG_OOB_RX:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_CAB_DIG_OOB_RX;
                sprintf(ptStateInfo->comp_name, "Comp_%d_OOBRX", ptTunerCtx->TunerID);
                sprintf(ptStateInfo->thread_name, "TU%d_ORX_Thread", ptTunerCtx->TunerID);
                break;
#ifdef TUNER_SUPPORT_SCART_OUT
        case DRVT_TUNER_CAB_ANA_SCART_OUT:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_CAB_ANA_SCART_OUT;
                sprintf(ptStateInfo->comp_name, "Comp_%d_CAB_SCRT", ptTunerCtx->TunerID);
                sprintf(ptStateInfo->thread_name, "TU%d_CAB_SCRT_Trd", ptTunerCtx->TunerID);
                break;
        case DRVT_TUNER_TER_ANA_SCART_OUT:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_TER_ANA_SCART_OUT;
                sprintf(ptStateInfo->comp_name, "Comp_%d_TER_SCRT", ptTunerCtx->TunerID);
                sprintf(ptStateInfo->thread_name, "TU%d_TER_SCRT_Trd", ptTunerCtx->TunerID);
                break;
#endif
        default:
                ptStateInfo->e_conn_type = TUNER_CONN_TYPE_TER_DIG;
                break;
    }
    ptStateInfo->fgTunerThreadRunning = TRUE;
    if (mcMUTEX_CREATE(&ptStateInfo->t_tuner_state_lock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit (mcMUTEX_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcMUTEX_UNLOCK(ptStateInfo->t_tuner_state_lock);
    if (mcSEMA_CREATE(&ptStateInfo->t_tuner_disconnected, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    if (mcSEMA_CREATE(&ptStateInfo->t_tuner_disconnecting, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    if (mcSEMA_CREATE(&ptStateInfo->t_tuner_connecting, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    if (mcSEMA_CREATE(&ptStateInfo->t_tuner_sync, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    if (mcSEMA_CREATE(&ptStateInfo->t_sync_tunerprocess_acq_and_mwtask, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    // Ready for creating Thread
    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not
    // directly use pv_arg. Therefore, if we want thread to share the same
    // context as caller, we need set pv_arg as "pointer of context".
    if (mcTHREAD_CREATE(&(ptStateInfo->t_tuner_thread),
            ptStateInfo->thread_name,
            cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
            TunerProcess, sizeof(void*), (void*)&ptStateInfo) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    //--

    // Everything is ready for Registering to RM
    res = x_rm_reg_comp(&ptStateInfo->t_tuner_comp_reg, 1, 1,
                LTDIS_GetCompNameForMW(ptTunerCtx->TunerID, DrvType),
                DRV_FLAG_ASYNC_CONN_OR_DISC,
                &t_tuner_fct_tbl,
                (const VOID *)NULL,
                (SIZE_T) 0);
    if (res != RMR_OK)
    {
        mcSHOW_DRVERR_MSG(("TunerInit TER: DRVAPI_TUNER_ERROR!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    return (DRVAPI_TUNER_OK);
}
/*************************************************************************
This function initializes tuner hardware, create the tuner
state machine thread, and registers to resource manager.

To release the resources allocated by Tuner_Init (no matter success or failure),
please call Tuner_Kill.
*************************************************************************/
INT32 Tuner_Init(void)
{
    INT32               res;
    UCHAR               i;
    TUNER_CONFIG_TBL_T  tTunerCfgTbl;
    TUNER_CTX_T*        ptTunerCtx;
    STATE_INFO_T*       ptStateInfo_TER=NULL;
    STATE_INFO_T*       ptStateInfo_CAB=NULL;
    STATE_INFO_T*       ptStateInfo_OOBRx;
    STATE_INFO_T*       ptStateInfo_TER_ANA=NULL;
    STATE_INFO_T*       ptStateInfo_CAB_ANA=NULL;

#ifdef TUNER_SUPPORT_SCART_OUT
    STATE_INFO_T*       ptStateInfo_TER_ANA_SCART_OUT=NULL;
    STATE_INFO_T*       ptStateInfo_CAB_ANA_SCART_OUT=NULL;
    //UCHAR               index2 = 0;
#endif

    UCHAR               index = 0;

    if (TunerInited){
        mcSHOW_DRVERR_MSG(("Tuner had been initialized \n"));
        return (DRVAPI_TUNER_ERROR);
    }

    gInit_Start_Time = os_get_sys_tick();

    mcSHOW_DRVAPI_MSG(("TunerInit...... \n"));
		printf("TunerInit.....................");

    // Create Thread for TunerOpen to save overall boot time.
    // x_thread_create(HANDLE_T *ph_th_hdl, CHAR *ps_name, SIZE_T z_stack_size,
    //                 UINT8 ui1_pri, x_os_thread_main_fct pf_main_rtn,
    //                  SIZE_T z_arg_size, VOID *pv_arg);
    // Important: x_thread_create will copy pv_arg to its memory while not
    // directly use pv_arg. Therefore, if we want thread to share the same
    // context as caller, we need set pv_arg as "pointer of context".
    TDD_Ready = FALSE;
    if (mcSEMA_CREATE(&t_init_sync, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("t_init_sync (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
        goto TUNER_INIT_ERROR_HANDLER;
    }
    sprintf(init_thread_name, "InitThread");
    if (mcTHREAD_CREATE(&t_init_thread,
            init_thread_name,
            cTHREAD_STACK_SIZE, cTHREAD_PRIORITY,
            InitThreadProc, sizeof(void*), (void*)NULL) != OSR_OK)
    {
        mcSHOW_DRVERR_MSG(("InitThreadProc (mcTHREAD_CREATE): DRVAPI_TUNER_ERROR!\n"));
        goto TUNER_INIT_ERROR_HANDLER;
    }

    // Retrieve Tuner Configuration Info from EEPROM/FLASH
    res = TDIS_RetrieveTunerCfgTbl(&tTunerCfgTbl);
    if (res != DRVAPI_TUNER_OK){
        goto TUNER_INIT_ERROR_HANDLER;
    }

    // Initialize TunerCtxList
    QueueInitList(&TunerCtxList);

    // Allocate/Initialize resources for each Tuner Device
    for (i=0;i<tTunerCfgTbl.TunerNum;i++){
        /**************************************************************/
        /*  Allocate TunerCtx and Link to TunerCtxList                */
        /**************************************************************/
        ptTunerCtx = (TUNER_CTX_T*)x_mem_calloc(1, sizeof(TUNER_CTX_T));
        if (ptTunerCtx == NULL){
            goto TUNER_INIT_ERROR_HANDLER;;
        }
        // Link TunerCtx to TunerCtxList
        QueuePutTail(&TunerCtxList, &ptTunerCtx->Link);

        /*************************************************************/
        /*  Initialize TunerCtx                                      */
        /*************************************************************/
        // Save TunerID in TunerCtx
        ptTunerCtx->TunerID = tTunerCfgTbl.tTunerCfg[i].TunerID;
        // Save Tuner's CfgInfo in TunerCtx
        x_memcpy(&ptTunerCtx->tTunerCfg, &tTunerCfgTbl.tTunerCfg[i],
                sizeof(TUNER_CONFIG_T));
        // Initialize RefSema
        if (mcSEMA_CREATE(&ptTunerCtx->RefSema, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            mcSHOW_DRVERR_MSG(("TunerInit (mcSEMA_CREATE): DRVAPI_TUNER_ERROR!\n"));
            goto TUNER_INIT_ERROR_HANDLER;
        }
        mcSEMA_UNLOCK(ptTunerCtx->RefSema);

        // Initialize StateInfoList
        QueueInitList(&ptTunerCtx->StateInfoList);

        /****************************************************************/
        /*  Allocate all necessary StateInfos, Link to StateInfoList,   */
        /*  Create thread and register to RM.                           */
        /****************************************************************/
        if (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_TER_ANA){
            ptStateInfo_TER_ANA = (STATE_INFO_T*)x_mem_calloc(1, sizeof(STATE_INFO_T));
            if (ptStateInfo_TER_ANA == NULL){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            // Link to StateInfoList
            QueuePutTail(&ptTunerCtx->StateInfoList, &ptStateInfo_TER_ANA->Link);
            // Initialize StateInfo
            if (InitializeStateInfo(ptTunerCtx, ptStateInfo_TER_ANA, DRVT_TUNER_TER_ANA)
                != DRVAPI_TUNER_OK){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            MutualExclCompReg_T[index++] = ptStateInfo_TER_ANA->t_tuner_comp_reg;
        }
        if (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_ANA){
            ptStateInfo_CAB_ANA = (STATE_INFO_T*)x_mem_calloc(1, sizeof(STATE_INFO_T));
            if (ptStateInfo_CAB_ANA == NULL){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            // Link to StateInfoList
            QueuePutTail(&ptTunerCtx->StateInfoList, &ptStateInfo_CAB_ANA->Link);
            // Initialize StateInfo
            if (InitializeStateInfo(ptTunerCtx, ptStateInfo_CAB_ANA, DRVT_TUNER_CAB_ANA)
                != DRVAPI_TUNER_OK){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            MutualExclCompReg_T[index++] = ptStateInfo_CAB_ANA->t_tuner_comp_reg;
        }

        if ((ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_TER_ANA)
            &&
            (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_ANA)
            ){
            res = x_rm_reg_comp_excl_list (&ptStateInfo_TER_ANA->t_tuner_comp_reg,
                                &ptStateInfo_CAB_ANA->t_tuner_comp_reg,
                                1);
            if (res != RMR_OK)
            {
                mcSHOW_DRVERR_MSG(("x_rm_reg_comp_excl_list failure!\n"));
                goto TUNER_INIT_ERROR_HANDLER;
            }
        }
#ifdef TUNER_SUPPORT_SCART_OUT
        if (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_TER_ANA_SCART_OUT){
            ptStateInfo_TER_ANA_SCART_OUT = (STATE_INFO_T*)x_mem_calloc(1, sizeof(STATE_INFO_T));
            if (ptStateInfo_TER_ANA_SCART_OUT == NULL){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            // Link to StateInfoList
            QueuePutTail(&ptTunerCtx->StateInfoList, &ptStateInfo_TER_ANA_SCART_OUT->Link);
            // Initialize StateInfo
            if (InitializeStateInfo(ptTunerCtx, ptStateInfo_TER_ANA_SCART_OUT, DRVT_TUNER_TER_ANA_SCART_OUT)
                != DRVAPI_TUNER_OK){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            
        }
        if (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT){
            ptStateInfo_CAB_ANA_SCART_OUT = (STATE_INFO_T*)x_mem_calloc(1, sizeof(STATE_INFO_T));
            if (ptStateInfo_CAB_ANA_SCART_OUT == NULL){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            // Link to StateInfoList
            QueuePutTail(&ptTunerCtx->StateInfoList, &ptStateInfo_CAB_ANA_SCART_OUT->Link);
            // Initialize StateInfo
            if (InitializeStateInfo(ptTunerCtx, ptStateInfo_CAB_ANA_SCART_OUT, DRVT_TUNER_CAB_ANA_SCART_OUT)
                != DRVAPI_TUNER_OK){
                goto TUNER_INIT_ERROR_HANDLER;
            }
             
        }
        if ((ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_TER_ANA_SCART_OUT)
            &&
            (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT)
            ){
            res = x_rm_reg_comp_excl_list (&ptStateInfo_TER_ANA_SCART_OUT->t_tuner_comp_reg,
                                &ptStateInfo_CAB_ANA_SCART_OUT->t_tuner_comp_reg,
                                1);
            if (res != RMR_OK)
            {
                mcSHOW_DRVERR_MSG(("x_rm_reg_comp_excl_list 2 failure!\n"));
                goto TUNER_INIT_ERROR_HANDLER;
            }
        }
#endif

        if (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_TER_DIG){
            ptStateInfo_TER = (STATE_INFO_T*)x_mem_calloc(1, sizeof(STATE_INFO_T));
            if (ptStateInfo_TER == NULL){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            // Link to StateInfoList
            QueuePutTail(&ptTunerCtx->StateInfoList, &ptStateInfo_TER->Link);
            // Initialize StateInfo
            if (InitializeStateInfo(ptTunerCtx, ptStateInfo_TER, DRVT_TUNER_TER_DIG)
                != DRVAPI_TUNER_OK){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            MutualExclCompReg_T[index++] = ptStateInfo_TER->t_tuner_comp_reg;
        }
        if (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_DIG){
            ptStateInfo_CAB = (STATE_INFO_T*)x_mem_calloc(1, sizeof(STATE_INFO_T));
            if (ptStateInfo_CAB == NULL){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            // Link to StateInfoList
            QueuePutTail(&ptTunerCtx->StateInfoList, &ptStateInfo_CAB->Link);
            // Initialize StateInfo
            if (InitializeStateInfo(ptTunerCtx, ptStateInfo_CAB, DRVT_TUNER_CAB_DIG)
                != DRVAPI_TUNER_OK){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            MutualExclCompReg_T[index++] = ptStateInfo_CAB->t_tuner_comp_reg;
        }
        if (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_DIG_OOB_RX){
            ptStateInfo_OOBRx = (STATE_INFO_T*)x_mem_calloc(1, sizeof(STATE_INFO_T));
            if (ptStateInfo_OOBRx == NULL){
                goto TUNER_INIT_ERROR_HANDLER;
            }
            // Link to StateInfoList
            QueuePutTail(&ptTunerCtx->StateInfoList, &ptStateInfo_OOBRx->Link);
            // Initialize StateInfo
            if (InitializeStateInfo(ptTunerCtx, ptStateInfo_OOBRx, DRVT_TUNER_CAB_DIG_OOB_RX)
                != DRVAPI_TUNER_OK){
                goto TUNER_INIT_ERROR_HANDLER;
            }
        }

        if ((ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_TER_DIG)
            &&
            (ptTunerCtx->tTunerCfg.i2TunerCapability & cWITH_DRVT_TUNER_CAB_DIG)
            ){
            res = x_rm_reg_comp_excl_list (&ptStateInfo_TER->t_tuner_comp_reg,
                                &ptStateInfo_CAB->t_tuner_comp_reg,
                                1);
            if (res != RMR_OK)
            {
                mcSHOW_DRVERR_MSG(("x_rm_reg_comp_excl_list failure!\n"));
                goto TUNER_INIT_ERROR_HANDLER;
            }
        }

    } // end of for-loop

    ASSERT(index <= MAX_MUTUAL_EXCL_COMP_REG_NUM);
    if (tTunerCfgTbl.MutualExcl){
        res = x_rm_reg_comp_excl_list (&MutualExclCompReg_T[0],
                            &MutualExclCompReg_T[1],
                            index-1);
        if (res != RMR_OK)
        {
            mcSHOW_DRVERR_MSG(("x_rm_reg_comp_excl_list failure! - MutualExclCompReg_T\n"));
            ASSERT(0);
            goto TUNER_INIT_ERROR_HANDLER;
        }
    }

    TunerInited = 1;
    mcSHOW_DRVAPI_MSG(("TunerInit OK \n"));
    mcSHOW_DRVAPI_MSG(("Tuner_Init API time = %d ms\n",
            (os_get_sys_tick()-gInit_Start_Time)*x_os_drv_get_tick_period()));
    return (DRVAPI_TUNER_OK);


TUNER_INIT_ERROR_HANDLER:
    mcSHOW_DRVERR_MSG(("TunerInit failure !! \n"));
    return (DRVAPI_TUNER_ERROR);
}
#endif	/* _X_TUNER_MW_C_ */
