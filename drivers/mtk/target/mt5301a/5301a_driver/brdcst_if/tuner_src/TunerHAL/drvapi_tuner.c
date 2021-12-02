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

#ifndef _X_DRVAPI_TUNER_C_
#define _X_DRVAPI_TUNER_C_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "PI_Def_dbg.h"
#include "t_rm.h"
#include "drvapi_tuner.h"
#include "x_rm_dev_types.h"
#include "x_drvapi_tuner.h" /* Antonio 02/14/05 */
#include "x_sys_name.h"
#include "UtilAPI.h"
#include "x_pinmux.h"
#include "nptv_drvif.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    variables definitions
 ----------------------------------------------------------------------------*/
GENERIC_LIST_ENTRY      TunerCtxList;
BOOL                    TunerInited = 0;

/*---------------
*   Removing global variable bDetachMW & bTunerOpenFailure to TDMgr.c
*   to solve involving x_rm_reg_comp() in drvapi_tuner.c when doing driver build.
*   reference CR: DTV00009782
-----------------*/
//BOOL                    bDetachMW = 0;  /*remove to TDMgr.c, solve drvapi_tuner.c cal*/
//BOOL                    bTunerOpenFailure = 0; /*remove to TDMgr.c*/
extern BOOL             bDetachMW;
extern BOOL             bTunerOpenFailure;

THREAD_T                t_init_thread = (THREAD_T)NULL;
char                    init_thread_name[16];
SEMA_T                  t_init_sync = (SEMA_T)NULL;
BOOL                    TDD_Ready = FALSE;

UINT16                  u2AcqTime;

static UCHAR            fStayInConnectedState=0;
UINT32                  gInit_Start_Time;

#if TUNER_DTV_NOTIFY_ENABLE
DRV_COMP_ID_T*  gPt_comp_id = NULL;/* Add the value for nofity MW when DTV Fine Tuner Freq */
#endif


static INT32 TunerConnect(DRV_COMP_ID_T*   pt_comp_id,
                          DRV_CONN_TYPE_T  e_conn_type,
                          const VOID*      pv_conn_info,
                          SIZE_T           z_conn_info_len,
                          VOID*            pv_tag,
                          x_rm_nfy_fct     pf_nfy);

static INT32 TunerDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                             DRV_DISC_TYPE_T  e_disc_type,
                             const VOID*      pv_disc_info,
                             SIZE_T           z_disc_info_len);

static INT32 TunerGet(DRV_COMP_ID_T*  pt_comp_id,
                      DRV_GET_TYPE_T  e_get_type,
                      VOID*           pv_get_info,
                      SIZE_T*         pz_get_info_len);

static INT32 TunerSet(DRV_COMP_ID_T*  pt_comp_id,
                      DRV_SET_TYPE_T  e_set_type,
                      const VOID*     pv_set_info,
                      SIZE_T          z_set_info_len);

DRV_COMP_FCT_TBL_T t_tuner_fct_tbl =
{
    TunerConnect,
    TunerDisconnect,
    TunerGet,
    TunerSet
};

/*-----------------------------------------------------------------------------
                    functions definitions
 ----------------------------------------------------------------------------*/
S32 GetTunerCtx(DRV_COMP_ID_T* pt_comp_id, TUNER_CTX_T** pptTunerCtx,
                STATE_INFO_T** pptStateInfo)
{
    TUNER_CTX_T*    ptTunerCtx;
    STATE_INFO_T*   ptStateInfo=NULL;

    ptTunerCtx = (TUNER_CTX_T*)QueueGetHead(&TunerCtxList);
    // Find the corresponding TunerCtx
    while (ptTunerCtx)
    {
        if (ptTunerCtx->TunerID == pt_comp_id->ui2_id)
        {
            // The corresponding TunerCtx exists.
            break;
        }
        ptTunerCtx = (TUNER_CTX_T*)QueueGetNext(&TunerCtxList, &ptTunerCtx->Link);
    }
    *pptTunerCtx = ptTunerCtx;

    // Find the corresponding StateInfo
    if (ptTunerCtx)
    {
        ptStateInfo = (STATE_INFO_T*)QueueGetHead(&ptTunerCtx->StateInfoList);
        while(ptStateInfo)
        {
            if (ptStateInfo->t_tuner_comp_id.e_type == pt_comp_id->e_type)
            {
                // The corresponding StateInfo exists.
                break;
            }
            ptStateInfo = (STATE_INFO_T*)QueueGetNext(&ptTunerCtx->StateInfoList, &ptStateInfo->Link);
        }
    }
    *pptStateInfo = ptStateInfo;

    if (ptTunerCtx && ptStateInfo)
    {
        return (DRVAPI_TUNER_OK);
    }
    else
    {
        return (DRVAPI_TUNER_ERROR);
    }
}

void TunerStayInConnectedState(UCHAR sw)
{
    fStayInConnectedState = sw;
}

#ifdef CC_MT5371
//---------------------------------------------------------------------
/** _SetResetPeripheralPin()
 *  @param i4Val 0 to reset, 1 to back.
 */
//---------------------------------------------------------------------
static void _SetResetPeripheralPin(INT32 i4Val)
{
    GPIO_Output(0, &i4Val);
}

//-----------------------------------------------------------------------------
/** _BspSerIsMT5371() to check it's MT5371 UART or not.
 *  @retval TRUE    It's running on MT5371 UART.
 *  @retval FALSE   It's not running on MT5371 UART, assume it's MT5351.
 */
//-----------------------------------------------------------------------------
static BOOL _fgIsMT5371 = TRUE;
#define TEST_REGEST 0x2000c110
static BOOL _BspSerIsMT5371()
{
    UINT32 u4BufCtlReg;
    static BOOL   fgInit = FALSE;
    volatile UINT32* pu4TestReg = (volatile UINT32*)TEST_REGEST;

    if (!fgInit) // Do the MT5371 register test and update result to _fgIsMT5371
    {
        u4BufCtlReg = *pu4TestReg;

        // Write pattern to the register
        *pu4TestReg = (*pu4TestReg & 0xffff00ff) | 0xAA00;

        // Read back and do checking
        if ((*pu4TestReg & 0x0000ff00) == 0xAA00)
        {
            _fgIsMT5371 = FALSE;
        }

        // Restore the original setting
        *pu4TestReg = u4BufCtlReg;

        fgInit = TRUE;
    }

    return _fgIsMT5371;
}

void vHwResetDemod(void)
{
    if (_BspSerIsMT5371())
    {
        Printf("#### Reset Tuner ####\n");
        // Start the peripheral reseting.
        _SetResetPeripheralPin(0);
        // delay 100 ms.
        mcDELAY_MS(100);
        // Finish the peripheral reseting.
        _SetResetPeripheralPin(1);
    }
}
#endif

void InitThreadProc(VOID *pvArg)
{
    mcSHOW_DRVAPI_MSG(("InitThreadProc is running!\n"));

    // Check if Tuner Device Drivers are ready
    TunerSetMwIntf(cTURNKEY_IF);
    if (TunerOpen() < DRVAPI_TUNER_OK)
    {
#ifdef CC_MT5371
        Printf("#### TunerOpen failed, please check power module and I2C bus! ####\n");
        vHwResetDemod();
        if (_fgIsMT5371)
        {
            Printf("#### Re-open Again ####\n");
            if (TunerOpen() < (S32)DRVAPI_TUNER_OK)
            {
                Printf("#### TunerOpen failed, please check power module and I2C bus! ####\n");
                Printf("#### Enter Dummy mode ####\n");
                bTunerOpenFailure = TRUE;
            }
        }
        else
        {
            Printf("#### Enter Dummy mode ####\n");
            bTunerOpenFailure = TRUE;
        }
#else
        Printf("#### Enter Dummy mode ####\n");
        bTunerOpenFailure = TRUE;
#endif
    }

    TDD_Ready = TRUE;
    mcSEMA_UNLOCK(t_init_sync);
    t_init_thread = (THREAD_T)NULL;
    mcSHOW_DRVAPI_MSG(("InitThreadProc exit!\n"));
    mcSHOW_DRVAPI_MSG(("InitThreadProc time = %d ms\n",
            (INT16)((os_get_sys_tick()-gInit_Start_Time)*x_os_drv_get_tick_period())));

    mcTHREAD_DESTROY();
}

/*
This function convert modulation type.
*/
TUNER_MODULATION_T TunerMod(S32 qam_size)
{
    TUNER_MODULATION_T tuner_mod;

    switch (qam_size)
    {
        case 4:
            tuner_mod = MOD_QPSK;
            break;
        case 8:
            tuner_mod = MOD_VSB_8;
            break;
        case 16:
            tuner_mod = MOD_QAM_16;
            break;
        case 32:
            tuner_mod = MOD_QAM_32;
            break;
        case 64:
            tuner_mod = MOD_QAM_64;
            break;
        case 128:
            tuner_mod = MOD_QAM_128;
            break;
        case 256:
            tuner_mod = MOD_QAM_256;
            break;
        case 999:
            tuner_mod = MOD_UNKNOWN;/*auto QAM*/
            break;
        default:
            tuner_mod = MOD_QAM_64;
            break;
    }
    return (tuner_mod);
}

/***********************************************************************/
/*
This function provide tuner break condition for acquisition.
*/
static S32 TunerBreak(void* pArg)
{
    DRV_COMP_ID_T*      pt_comp_id;
    TUNER_CTX_T*        ptTunerCtx;
    STATE_INFO_T*       ptStateInfo;
    
    //TunerAcq in TUNER_STATE_CONNECTED state. - mtk40109 2009-07-21
    //Return from NIM to MW.
    if (!pArg)
    {
        return (FALSE);
    }
    
    pt_comp_id = (DRV_COMP_ID_T*)pArg;
    // Get corresponding TunerCtx and StateInfo
    if (GetTunerCtx(pt_comp_id, &ptTunerCtx, &ptStateInfo)!= DRVAPI_TUNER_OK)
    {
        return (TRUE);
    }
    
    if (ptStateInfo->e_tuner_state != TUNER_STATE_CONNECTING)
    {
        mcSHOW_DRVAPI_MSG(("\nTunerBreak: @state(%d)\n", ptStateInfo->e_tuner_state));
        return (TRUE);
    }
    
    return (FALSE);
}

/*
This function set tuner state unconditionally.
*/
static void TunerStateSet(STATE_INFO_T* ptStateInfo, TUNER_STATE_T x)
{
    mcMUTEX_LOCK(ptStateInfo->t_tuner_state_lock);
    ptStateInfo->e_tuner_state = x;
    mcMUTEX_UNLOCK(ptStateInfo->t_tuner_state_lock);
    mcSHOW_DRVAPI_MSG(("%s TunerStateSet: %d\n", ptStateInfo->comp_name, x));
}

/*
This function change tuner state conditionally.
*/
static void TunerStateChange(STATE_INFO_T* ptStateInfo, TUNER_STATE_T os, TUNER_STATE_T ns)
{
    mcMUTEX_LOCK(ptStateInfo->t_tuner_state_lock);
    if (ptStateInfo->e_tuner_state == os)
    {
        ptStateInfo->e_tuner_state = ns;
        mcMUTEX_UNLOCK(ptStateInfo->t_tuner_state_lock);
        mcSHOW_DRVAPI_MSG(("%s TunerStateChang: %d -> %d\n",ptStateInfo->comp_name, os, ns));
    }
    else
    {
        mcMUTEX_UNLOCK(ptStateInfo->t_tuner_state_lock);
    }
}

VOID pf_dummy_nfy (DRV_COMP_ID_T*  pt_comp_id,
                   DRV_COND_T      e_nfy_cond,
                   VOID*           pv_tag,
                   UINT32          ui4_data)
{
    mcSHOW_DRVAPI_MSG(("pf_dummy_nfy is called\n"));
    return;
}

/*
This function implements component connect function.
*/
static INT32 TunerConnect(DRV_COMP_ID_T*   pt_comp_id,
                          DRV_CONN_TYPE_T  e_conn_type,
                          const VOID*      pv_conn_info,
                          SIZE_T           z_conn_info_len,
                          VOID*            pv_tag,
                          x_rm_nfy_fct     pf_nfy)
{
    TUNER_CTX_T*            ptTunerCtx;
    STATE_INFO_T*           ptStateInfo;
    UINT32                  Freq=0;
    UINT32                  SymbolRate=0;
    TUNER_MODULATION_T      e_mod=MOD_UNKNOWN;
    BOOL                    SpecInv=0;

    mcSHOW_DRVAPI_MSG(("[MWCmd]TunerConnect()\n"));
    if (bDetachMW)
    {
        mcSHOW_DRVAPI_MSG(("TunerDriver Detach MW\n"));
        return (RMR_OK);
    }

    // Service will be provided ONLY when tuner is opened successfully.
    if (!TDD_Ready)
    {
        mcSEMA_LOCK(t_init_sync);
        mcSEMA_UNLOCK(t_init_sync);
    }

    // Get corresponding TunerCtx and StateInfo
    if (GetTunerCtx(pt_comp_id, &ptTunerCtx, &ptStateInfo)!= DRVAPI_TUNER_OK)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    
    if (ptStateInfo->e_conn_type != e_conn_type)
    {
        mcSHOW_DRVERR_MSG(("TunerConnect: RMR_DRV_INV_CONN_INFO!\n"));
        return (RMR_DRV_INV_CONN_INFO);
    }
    
    /* verify the request */
    if ((pt_comp_id->e_type != DRVT_TUNER_CAB_DIG) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG_OOB_RX) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_DIG) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA))
    {
        mcSHOW_DRVERR_MSG(("TunerConnect: RMR_DRV_INV_CONN_INFO!\n"));
        return (RMR_DRV_INV_CONN_INFO);
    }
    
    if ((pf_nfy == NULL) && (!ptStateInfo->b_disable_nfy))
    {
        mcSHOW_DRVERR_MSG(("TunerConnect: RMR_DRV_INV_CONN_INFO!\n"));
        return (RMR_DRV_INV_CONN_INFO);
    }
    
    /* verify connect info */
    if (pv_conn_info == NULL)
    {
        mcSHOW_DRVERR_MSG(("TunerConnect: RMR_DRV_INV_CONN_INFO!\n"));
        return (RMR_DRV_INV_CONN_INFO);
    }

    switch (e_conn_type)
    {
        case TUNER_CONN_TYPE_TER_DIG:
            ptStateInfo->b_disable_nfy = ((TUNER_TER_DIG_TUNE_INFO_T*)pv_conn_info)->b_no_disc_nfy;
            break;
        case TUNER_CONN_TYPE_CAB_DIG:
            ptStateInfo->b_disable_nfy = ((TUNER_CAB_DIG_TUNE_INFO_T*)pv_conn_info)->b_no_disc_nfy;
            break;
        case TUNER_CONN_TYPE_CAB_DIG_OOB_RX:
            ptStateInfo->b_disable_nfy = ((TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T*)pv_conn_info)->b_no_disc_nfy;
            break;
        case TUNER_CONN_TYPE_TER_ANA:
            ptStateInfo->b_disable_nfy = ((TUNER_TER_ANA_TUNE_INFO_T*)pv_conn_info)->b_no_disc_nfy;
            break;
        case TUNER_CONN_TYPE_CAB_ANA:
            ptStateInfo->b_disable_nfy = ((TUNER_CAB_ANA_TUNE_INFO_T*)pv_conn_info)->b_no_disc_nfy;
            break;
        case TUNER_CONN_TYPE_CAB_ANA_SCART_OUT:
            ptStateInfo->b_disable_nfy = ((TUNER_CAB_ANA_TUNE_INFO_T*)pv_conn_info)->b_no_disc_nfy;
            break;
        case TUNER_CONN_TYPE_TER_ANA_SCART_OUT:
            ptStateInfo->b_disable_nfy = ((TUNER_CAB_ANA_TUNE_INFO_T*)pv_conn_info)->b_no_disc_nfy;
            break;
        default:
            mcSHOW_DRVERR_MSG(("ERROR: Unknown e_conn_type\n"));
            break;
    }

    if ((pt_comp_id->e_type != DRVT_TUNER_TER_ANA) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA))
    {
        if (Util_TranslateConnectInfo(e_conn_type, (VOID*)pv_conn_info, z_conn_info_len,
                    &Freq, &SymbolRate, &e_mod, &SpecInv)!=RMR_OK)
        {
            return (RMR_DRV_INV_CONN_INFO);
        }

        /* Don't set tuner if RF freq not changed */
        if ((ptStateInfo->e_tuner_state == TUNER_STATE_CONNECTED) &&
            (TunerGetSync(ptTunerCtx->TunerID, e_conn_type)) &&
            (ptStateInfo->t_tuner_info.ui4_freq == Freq) &&
            (ptStateInfo->t_tuner_info.ui4_sym_rate == SymbolRate) &&
            (ptStateInfo->t_tuner_info.e_mod == e_mod) &&
            (ptStateInfo->e_conn_type == TUNER_CONN_TYPE_CAB_DIG_OOB_RX) &&
            (ptStateInfo->OobSpecInv == SpecInv))
        {
            ptStateInfo->pf_tuner_nfy = pf_nfy;
            ptStateInfo->pv_tuner_nfy_tag = pv_tag;
            //++ Save conn_info
            x_memcpy(&ptStateInfo->t_conn_info, pv_conn_info, z_conn_info_len);
            ptStateInfo->z_conn_info_len = z_conn_info_len;
            //--
            TunerStateSet(ptStateInfo, TUNER_STATE_CONNECTED);
            /* always report connected */
            if (pf_nfy != NULL) // Klocwork error (NPD.CHECK.MIGHT)
            {
                pf_nfy(&ptStateInfo->t_tuner_comp_id,
                    DRV_COND_CONNECTED,
                    pv_tag,
                    RM_CONN_REASON_AS_REQUESTED);
                pf_nfy(&ptStateInfo->t_tuner_comp_id,
                    DRV_COND_STATUS,
                    pv_tag,
                    TUNER_COND_STATUS_GOT_SIGNAL);
            }
            return (RMR_OK);
        }
    }
    
    if (ptStateInfo->e_tuner_state == TUNER_STATE_CONNECTING)
    {
        // According to Driver-Tuner document,
        // (Sec: 2.3.2  Multiple connect requests)
        // "It is important to note that when connection requests are
        // discarded the tuner driver does not have to notify the
        // Middleware layer."
        ptStateInfo->pf_tuner_nfy = (x_rm_nfy_fct)pf_dummy_nfy;
    }

    /* signal TunerProcess to be TUNER_STATE_DISCONNECTED */
    TunerStateSet(ptStateInfo, TUNER_STATE_DISCONNECTED);
    mcSEMA_UNLOCK(ptStateInfo->t_tuner_sync);
    mcSHOW_DRVLOCK_MSG(("\t====== .LOCK_disconnected\n"));
    mcSEMA_LOCK(ptStateInfo->t_tuner_disconnected);
    mcSHOW_DRVLOCK_MSG(("\t====== .LOCK2_disconnected\n"));

    // TunerProcess is ready to process this new request
    ptStateInfo->pf_tuner_nfy = pf_nfy;
    ptStateInfo->pv_tuner_nfy_tag = pv_tag;
    //++ Save conn_info
    x_memcpy(&ptStateInfo->t_conn_info, pv_conn_info, z_conn_info_len);
    ptStateInfo->z_conn_info_len = z_conn_info_len;
    //--

    if ((pt_comp_id->e_type != DRVT_TUNER_TER_ANA) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA))
    {
        /* change tuner parameters */
        ptStateInfo->t_tuner_info.ui4_freq     = Freq;
        ptStateInfo->t_tuner_info.ui4_sym_rate = SymbolRate;
        ptStateInfo->t_tuner_info.e_mod        = e_mod;
        ptStateInfo->OobSpecInv = SpecInv;
    }

    /* signal tuner to start a new acquisition */
    TunerStateSet(ptStateInfo, TUNER_STATE_CONNECTING);
    mcSHOW_DRVLOCK_MSG(("\t====== .UNLOCK_connecting\n"));
    mcSEMA_UNLOCK(ptStateInfo->t_tuner_connecting);

    mcSHOW_DRVAPIOK_MSG(("TunerConnect: RMR_OK!\n"));
    return (RMR_ASYNC_NFY);
}

/*
This function implements component disconnect function.
*/
static INT32 TunerDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                             DRV_DISC_TYPE_T  e_disc_type,
                             const VOID*      pv_disc_info,
                             SIZE_T           z_disc_info_len)
{
    TUNER_CTX_T*        ptTunerCtx;
    STATE_INFO_T*       ptStateInfo;

    mcSHOW_DRVAPI_MSG(("[MWCmd]TunerDisconnect()\n"));
    if (bDetachMW)
    {
        mcSHOW_DRVAPI_MSG(("TunerDriver Detach MW\n"));
        return (RMR_OK);
    }
    
    // Service will be provided ONLY when tuner is opened successfully.
    if (!TDD_Ready)
    {
        mcSEMA_LOCK(t_init_sync);
        mcSEMA_UNLOCK(t_init_sync);
    }

    // Get corresponding TunerCtx and StateInfo
    if (GetTunerCtx(pt_comp_id, &ptTunerCtx, &ptStateInfo)!= DRVAPI_TUNER_OK)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    
    /* verify the request */
    if (((pt_comp_id->e_type != DRVT_TUNER_CAB_DIG) &&
         (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG_OOB_RX) &&
         (pt_comp_id->e_type != DRVT_TUNER_TER_DIG) &&
         (pt_comp_id->e_type != DRVT_TUNER_TER_ANA) &&
         (pt_comp_id->e_type != DRVT_TUNER_TER_ANA_SCART_OUT) &&
         (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA_SCART_OUT) &&     
         (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA)))
    {
        mcSHOW_DRVERR_MSG(("TunerDisconnect: RMR_DRV_INV_DISC_INFO!\n"));
        return (RMR_DRV_INV_DISC_INFO);
    }

    if (ptStateInfo->e_tuner_state != TUNER_STATE_DISCONNECTED)
    { /* change tuner state */
        TunerStateSet(ptStateInfo, TUNER_STATE_DISCONNECTING);
        mcSEMA_LOCK(ptStateInfo->t_tuner_disconnecting); /*bug fix CR: DTV00139714*/
        mcSEMA_UNLOCK(ptStateInfo->t_tuner_sync);
    }
    else
    { /*MW wanna do TunerDisconnect even tuner sate is disconnect*/
        TunerDisc(ptTunerCtx->TunerID, ptStateInfo->e_conn_type);
    }

    mcSHOW_DRVAPIOK_MSG(("TunerDisconnect: RMR_OK!\n"));
    return (RMR_OK); /*bug fix CR: DTV00139714*/
}

/*
This function implements component get function.
*/
static INT32 TunerGet(DRV_COMP_ID_T*  pt_comp_id,
                      DRV_GET_TYPE_T  e_get_type,
                      VOID*           pv_get_info,
                      SIZE_T*         pz_get_info_len)
{
    TUNER_CTX_T*        ptTunerCtx;
    STATE_INFO_T*       ptStateInfo;

    mcSHOW_DRVAPI_MSG(("[MWCmd]TunerGet()\n"));
    if (bDetachMW)
    {
        mcSHOW_DRVAPI_MSG(("TunerDriver Detach MW\n"));
        return (RMR_OK);
    }

    // Service will be provided ONLY when tuner is opened successfully.
    if (!TDD_Ready)
    {
        mcSEMA_LOCK(t_init_sync);
        mcSEMA_UNLOCK(t_init_sync);
    }

    // Get corresponding TunerCtx and StateInfo
    if (GetTunerCtx(pt_comp_id, &ptTunerCtx, &ptStateInfo)!= DRVAPI_TUNER_OK)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    /* verify the request */
    if ((pt_comp_id->e_type != DRVT_TUNER_CAB_DIG) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG_OOB_RX) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_DIG) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA))
    {
        mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_INV_GET_INFO!\n"));
        return (RMR_DRV_INV_GET_INFO);
    }

    /* supported types */
    switch (e_get_type)
    {
        case TUNER_GET_TYPE_TER_ANA:
        {
            if (ptStateInfo->e_tuner_state == TUNER_STATE_CONNECTED)
            { /* if connected */
                if (*pz_get_info_len < sizeof(TUNER_TER_ANA_TUNE_INFO_T))
                { /* not enough space */
                    *pz_get_info_len = sizeof(TUNER_TER_ANA_TUNE_INFO_T);
                    mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                    return (RMR_DRV_NOT_ENOUGH_SPACE);
                }
                else
                { /* get info */
                    TunerGetSignal(ptTunerCtx->TunerID, TUNER_CONN_TYPE_TER_ANA,
                                    (SIGNAL*)pv_get_info);
                }
            }
            else
            { /* if not connected */
                mcSHOW_DRVAPI_MSG(("TunerGet: RMR_DRV_NO_GET_INFO!\n"));
                return (RMR_DRV_NO_GET_INFO);
            }
            break;
        }
        case TUNER_GET_TYPE_CAB_ANA:
        {
            if (ptStateInfo->e_tuner_state == TUNER_STATE_CONNECTED)
            { /* if connected */
                if (*pz_get_info_len < sizeof(TUNER_CAB_ANA_TUNE_INFO_T))
                { /* not enough space */
                    *pz_get_info_len = sizeof(TUNER_CAB_ANA_TUNE_INFO_T);
                    mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                    return (RMR_DRV_NOT_ENOUGH_SPACE);
                }
                else
                { /* get info */
                    TunerGetSignal(ptTunerCtx->TunerID, TUNER_CONN_TYPE_CAB_ANA, (SIGNAL*)pv_get_info);
                }
            }
            else
            { /* if not connected */
                mcSHOW_DRVAPI_MSG(("TunerGet: RMR_DRV_NO_GET_INFO!\n"));
                return (RMR_DRV_NO_GET_INFO);
            }
            break;
        }
        case TUNER_GET_TYPE_TER_DIG:
        {
            if (ptStateInfo->e_tuner_state == TUNER_STATE_CONNECTED)
            { /* if connected */
                if (*pz_get_info_len < sizeof(TUNER_TER_DIG_TUNE_INFO_T))
                { /* not enough space */
                    *pz_get_info_len = sizeof(TUNER_TER_DIG_TUNE_INFO_T);
                    mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                    return (RMR_DRV_NOT_ENOUGH_SPACE);
                }
                else
                { /* get info */
                    SIGNAL  t_signal;
                    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = (TUNER_TER_DIG_TUNE_INFO_T *) pv_get_info;

                    TunerGetSignal(ptTunerCtx->TunerID, TUNER_CONN_TYPE_TER_DIG, &t_signal);
                    pt_tuner_info->ui4_freq     = t_signal.Frequency * 1000;    /* Hz */
                    pt_tuner_info->e_mod        = TunerMod(t_signal.QAMSize);   /* TUNER_MODULATION_T */
                    pt_tuner_info->e_bandwidth  = (TUNER_BANDWIDTH_T) t_signal.e_bandwidth;
                    pt_tuner_info->e_hierarchy_priority = (TUNER_HIERARCHY_PRIORITY_T) t_signal.e_hierarchy_priority;
                }
            }
            else
            { /* if not connected */
                mcSHOW_DRVAPI_MSG(("TunerGet: RMR_DRV_NO_GET_INFO!\n"));
                return (RMR_DRV_NO_GET_INFO);
            }
            break;
        }
        case TUNER_GET_TYPE_CAB_DIG:
        {
            if (ptStateInfo->e_tuner_state == TUNER_STATE_CONNECTED)
            { /* if connected */
                if (*pz_get_info_len < sizeof(TUNER_CAB_DIG_TUNE_INFO_T))
                { /* not enough space */
                    *pz_get_info_len = sizeof(TUNER_CAB_DIG_TUNE_INFO_T);
                    mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                    return (RMR_DRV_NOT_ENOUGH_SPACE);
                }
                else
                { /* get info */
                    SIGNAL  t_signal;
                    TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *) pv_get_info;

                    TunerGetSignal(ptTunerCtx->TunerID, TUNER_CONN_TYPE_CAB_DIG, &t_signal);
                    pt_tuner_info->ui4_freq     = t_signal.Frequency * 1000;    /* Hz */
                    pt_tuner_info->ui4_sym_rate = t_signal.SymbolRate * 1000;   /* sps */
                    pt_tuner_info->e_mod        = TunerMod(t_signal.QAMSize);   /* TUNER_MODULATION_T */
                }
            }
            else
            { /* if not connected */
                mcSHOW_DRVAPI_MSG(("TunerGet: RMR_DRV_NO_GET_INFO!\n"));
                return (RMR_DRV_NO_GET_INFO);
            }
            break;
        }
        case TUNER_GET_TYPE_CAB_DIG_OOB_RX:
        {
            if (ptStateInfo->e_tuner_state == TUNER_STATE_CONNECTED)
            { /* if connected */
                if (*pz_get_info_len < sizeof(TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T))
                { /* not enough space */
                    *pz_get_info_len = sizeof(TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T);
                    mcSHOW_DRVERR_MSG(("TunerGetOob: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                    return (RMR_DRV_NOT_ENOUGH_SPACE);
                }
                else
                { /* get info */
                    SIGNAL  t_signal;
                    TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T *pt_tuner_info = (TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T *) pv_get_info;

                    TunerGetSignal(ptTunerCtx->TunerID, TUNER_CONN_TYPE_CAB_DIG_OOB_RX, &t_signal);
                    pt_tuner_info->ui4_freq      = t_signal.Frequency * 1000;    /* Hz */
                    pt_tuner_info->ui4_data_rate = t_signal.SymbolRate * 1000;
                    pt_tuner_info->e_mod         = TunerMod(t_signal.QAMSize);   /* TUNER_MODULATION_T */
                }
            }
            else
            { /* if not connected */
                mcSHOW_DRVAPI_MSG(("TunerGetOob: RMR_DRV_NO_GET_INFO!\n"));
                return (RMR_DRV_NO_GET_INFO);
            }
            break;
        }
        case TUNER_GET_TYPE_SIGNAL_LEVEL:
        {
            if (*pz_get_info_len < sizeof(UINT8))
            { /* not enough space */
                *pz_get_info_len = sizeof(UINT8);
                mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            { /* get info */
                *(UINT8 *)pv_get_info = TunerGetSignalLevel(ptTunerCtx->TunerID, ptStateInfo->e_conn_type);
            }
            mcSHOW_DRVAPI_MSG(("%s Signal level: (%d/100)\n", ptStateInfo->comp_name, *(UINT8 *)pv_get_info));
            break;
        }
        case TUNER_GET_TYPE_STATE:
        {
            if (*pz_get_info_len < sizeof(TUNER_STATE_INFO_T))
            { /* not enough space */
                *pz_get_info_len = sizeof(TUNER_STATE_INFO_T);
                mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            { /* get info */
                TUNER_STATE_INFO_T *pt_tuner_state;

                pt_tuner_state = (TUNER_STATE_INFO_T *)pv_get_info;
                pt_tuner_state->e_state = ptStateInfo->e_tuner_state;
                if ((pt_comp_id->e_type == DRVT_TUNER_CAB_ANA) ||
                    (pt_comp_id->e_type == DRVT_TUNER_TER_ANA))
                {
                    pt_tuner_state->b_dig_signal = (FALSE);
                }
                else
                {
                    pt_tuner_state->b_dig_signal = (TRUE);
                }
            }
            mcSHOW_DRVAPI_MSG(("%s Tuner state: (%d)\n",ptStateInfo->comp_name, ptStateInfo->e_tuner_state));
            mcSHOW_DRVAPI_MSG(("%s Current HW sync state: (%d)\n",ptStateInfo->comp_name,
                        TunerGetSync(ptTunerCtx->TunerID, ptStateInfo->e_conn_type)));
            break;
        }
        case TUNER_GET_TYPE_NO_DISC_NFY:
        {
            if (*pz_get_info_len < sizeof(BOOL))
            { /* not enough space */
                *pz_get_info_len = sizeof(BOOL);
                mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            { /* get info */
                *(BOOL *)pv_get_info = ptStateInfo->b_disable_nfy;
            }
            break;
        }
        case TUNER_GET_TYPE_DBM_SIGNAL_LEVEL:
        {
            if (*pz_get_info_len < sizeof(INT16))
            { /* not enough space */
                *pz_get_info_len = sizeof(INT16);
                mcSHOW_DRVERR_MSG(("TunerGet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            { /* get info */
                *(INT16 *)pv_get_info = TunerGetSignalLeveldBm(ptTunerCtx->TunerID, ptStateInfo->e_conn_type);
            }
            mcSHOW_DRVAPI_MSG(("%s (%d dBm)\n",ptStateInfo->comp_name, *(INT16 *) pv_get_info / 10));
            break;
        }

        case TUNER_GET_TYPE_ANA_ATTRIBUTE:
        case TUNER_GET_TYPE_TV_SYS_GROUP:
        case TUNER_GET_TYPE_TV_SYS_MASK:
        {
            if ((pt_comp_id->e_type==DRVT_TUNER_TER_ANA) || (pt_comp_id->e_type==DRVT_TUNER_CAB_ANA))
            {
                return (TunerGetAttribute(ptTunerCtx->TunerID, e_get_type, pv_get_info, pz_get_info_len));
            }
            break;
        }

        case TUNER_GET_TYPE_BER:
        case TUNER_GET_TYPE_DIG_ATTRIBUTE:
        case TUNER_GET_TYPE_RAW_SIGNAL_LEVEL:
        case TUNER_GET_TYPE_SIGNAL_LEVEL_TOLERANCE:
        case TUNER_GET_TYPE_BER_TOLERANCE:
        {
            return (TunerGetAttribute(ptTunerCtx->TunerID, e_get_type, pv_get_info, pz_get_info_len));
        }

        default:
        {
            // It is more reasonable for pd_glue to handle this.
            return (TunerGetAttribute(ptTunerCtx->TunerID, e_get_type, pv_get_info, pz_get_info_len));
        }
    }

    mcSHOW_DRVAPIOK_MSG(("TunerGet: RMR_OK!\n"));
    return (RMR_OK);
}

/*
This function implements component set function.
*/
static INT32 TunerSet(DRV_COMP_ID_T*  pt_comp_id,
                      DRV_SET_TYPE_T  e_set_type,
                      const VOID*     pv_set_info,
                      SIZE_T          z_set_info_len)
{
    TUNER_CTX_T*        ptTunerCtx;
    STATE_INFO_T*       ptStateInfo;
    INT32               retStatus;

    mcSHOW_DRVAPI_MSG(("[MWCmd]TunerSet()\n"));
    if (bDetachMW)
    {
        mcSHOW_DRVAPI_MSG(("TunerDriver Detach MW\n"));
        return (RMR_OK);
    }

    // Service will be provided ONLY when tuner is opened successfully.
    if (!TDD_Ready)
    {
        mcSEMA_LOCK(t_init_sync);
        mcSEMA_UNLOCK(t_init_sync);
    }

    // Get corresponding TunerCtx and StateInfo
    if (GetTunerCtx(pt_comp_id, &ptTunerCtx, &ptStateInfo) != DRVAPI_TUNER_OK)
    {
        return (DRVAPI_TUNER_ERROR);
    }
    
    /* verify the request */
    if ((pt_comp_id->e_type != DRVT_TUNER_CAB_DIG) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG_OOB_RX) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_DIG) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA) &&
        (pt_comp_id->e_type != DRVT_TUNER_TER_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA_SCART_OUT) &&
        (pt_comp_id->e_type != DRVT_TUNER_CAB_ANA))
    {
        mcSHOW_DRVERR_MSG(("TunerSet: RMR_DRV_INV_SET_INFO!\n"));
        return (RMR_DRV_INV_SET_INFO);
    }

    /* supported types */
    switch (e_set_type)
    {
        case TUNER_SET_TYPE_NO_DISC_NFY:
        {
            if (z_set_info_len < sizeof(BOOL))
            { /* not enough space */
                z_set_info_len = sizeof(BOOL);
                mcSHOW_DRVERR_MSG(("TunerSet: RMR_DRV_NOT_ENOUGH_SPACE!\n"));
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            { /* set info */
                ptStateInfo->b_disable_nfy = (BOOL)(S32)pv_set_info;
            }
            break;
        }
        case TUNER_SET_TYPE_CI_CARD_TYPE:
        {
            retStatus = TunerSetAttribute(ptTunerCtx->TunerID, e_set_type, pv_set_info, z_set_info_len);
            if (retStatus != RMR_OK)
            {
                return retStatus;
            }
            break;
        }

        default:
        {
        	// Ii is more reasonable for pd_glue to handle this
        	return (TunerSetAttribute(ptTunerCtx->TunerID, e_set_type, pv_set_info, z_set_info_len));
        }
    }

    /* there is nothing to be set with digital cable tuner */
    mcSHOW_DRVAPIOK_MSG(("TunerSet: RMR_OK!\n"));
    return (RMR_OK);
}

/*
This function implements the tuner state machine process and never return.
*/
VOID TunerProcess(VOID *pvArg)
{
    STATE_INFO_T*   ptStateInfo = *(PSTATE_INFO_T*)pvArg;
    TUNER_CTX_T*    ptTunerCtx = (TUNER_CTX_T*)ptStateInfo->ptParentTunerCtx;
    UCHAR           uc_unluck;
    TIME_TICK_T     t0_unlock, t1_unlock;
    TIME_DIFF_T     t_diff;
    S16             sync_sts;
    UINT32          u4TickCnt;
    BOOL            fgRetSts = FALSE;
    BOOL            b_notified = FALSE;
    EWS_INFO        ewsInfo = {FALSE, 0};
    SIZE_T          ewsInfoLen;
    BOOL            bPrevDetachMW = 0;

    mcSHOW_DRVAPI_MSG(("TunerProcess %s is running!\n", ptStateInfo->thread_name));
    ptStateInfo->e_tuner_state = TUNER_STATE_DISCONNECTED;
    uc_unluck = 0;

    while (ptStateInfo->fgTunerThreadRunning)
    {
        //While Detach MW,  pause the TunerProcess. - mtk40109 2009-07-15
        if(bDetachMW)
        {
            mcDELAY_MS(cMAX_RESET_TIME);
            bPrevDetachMW = 1;
            continue;
        }
        
        switch (ptStateInfo->e_tuner_state)
        {
            case TUNER_STATE_DISCONNECTED:
            {
                b_notified = FALSE;

                mcSHOW_DRVLOCK_MSG(("\t****** .UNLOCK_disconnected\n"));
                mcSEMA_UNLOCK(ptStateInfo->t_tuner_disconnected);
                mcSHOW_DRVLOCK_MSG(("\t****** LOCK_connecting\n"));

                // Waiting for t_tuner_connecting
                mcSEMA_LOCK(ptStateInfo->t_tuner_connecting);
                mcSHOW_DRVLOCK_MSG(("\t****** LOCK2_connecting\n"));
                break;
            }
            
            case TUNER_STATE_CONNECTING:
            {
                b_notified = FALSE;
                mcSHOW_DRVAPI_MSG(("TunerAcq\n"));
                
                /* open a new acquisition */
                u4TickCnt = os_get_sys_tick();

		#if TUNER_DTV_NOTIFY_ENABLE
		gPt_comp_id = &ptStateInfo->t_tuner_comp_id;/* Add the value for nofity MW when DTV Fine Tuner Freq */
		#endif
                fgRetSts = TunerAcq(ptTunerCtx->TunerID, ptStateInfo->e_conn_type,
                                    (void*)&ptStateInfo->t_conn_info,
                                    ptStateInfo->z_conn_info_len,
                                    TunerBreak, &ptStateInfo->t_tuner_comp_id);

                if (fgRetSts) 
                {/* lock */
                    mcSHOW_DRVAPI_MSG(("%s: Connected in TUNER_STATE_CONNECTING\n", ptStateInfo->comp_name));
                    uc_unluck = 0;
                    TunerStateChange(ptStateInfo, TUNER_STATE_CONNECTING, TUNER_STATE_CONNECTED);

                    /* always report connected */
                    ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                            DRV_COND_CONNECTED,
                            ptStateInfo->pv_tuner_nfy_tag,
                            RM_CONN_REASON_AS_REQUESTED);
                    ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                            DRV_COND_STATUS,
                            ptStateInfo->pv_tuner_nfy_tag,
                            TUNER_COND_STATUS_GOT_SIGNAL);

                    //! EWS notification
                    TunerGetAttribute(ptTunerCtx->TunerID, TUNER_GET_SPECIAL_TYPE_EWS_STATUS, (void*)&ewsInfo, (SIZE_T*)&ewsInfoLen);
                    if (ewsInfo.isNotify)
                    {//! EWS triggerred
                        if (ewsInfo.val)
                        {//! EWS ON
                            ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                    DRV_COND_STATUS,
                                    ptStateInfo->pv_tuner_nfy_tag,
                                    TUNER_COND_STATUS_EWS_ON);
                        }
                        else
                        {//! EWS OFF
                            ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                    DRV_COND_STATUS,
                                    ptStateInfo->pv_tuner_nfy_tag,
                                    TUNER_COND_STATUS_EWS_OFF);
                        }
                    }
                }
                else
                {/* out of lock */  
                    mcSHOW_DRVAPI_MSG(("%s: Can't sync in TUNER_STATE_CONNECTING\n", ptStateInfo->comp_name));
                    if (!fStayInConnectedState)
                    {
                        if (!ptStateInfo->b_disable_nfy)
                        {
                            mcSHOW_DRVERR_MSG(("[TUNER_STATE_CONNECTING] b_disable_nfy = FALSE\n"));
                            TunerStateChange(ptStateInfo, TUNER_STATE_CONNECTING, TUNER_STATE_DISCONNECTED);
                            
                            ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                    DRV_COND_DISCONNECTED,
                                    ptStateInfo->pv_tuner_nfy_tag,
                                    TUNER_DISC_REASON_SIGNAL_LOSS);
                        }
                        else
                        {
                            mcSHOW_DRVERR_MSG(("[TUNER_STATE_CONNECTING] b_disable_nfy = TRUE\n"));
                            TunerStateChange(ptStateInfo, TUNER_STATE_CONNECTING, TUNER_STATE_CONNECTED);
                            
                            /* always report connected */
                            ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                    DRV_COND_CONNECTED,
                                    ptStateInfo->pv_tuner_nfy_tag,
                                    RM_CONN_REASON_AS_REQUESTED);
                            ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                    DRV_COND_STATUS,
                                    ptStateInfo->pv_tuner_nfy_tag,
                                    TUNER_COND_STATUS_LOST_SIGNAL);

                            b_notified = TRUE;
                        }
                    }
                }

                u4TickCnt = os_get_sys_tick()-u4TickCnt;
                u2AcqTime = u4TickCnt * x_os_drv_get_tick_period();
                mcSHOW_DRVAPI_MSG(("TunerID%d Connection time = %d ms\n",ptTunerCtx->TunerID, u2AcqTime));
                break;
            }
            
            case TUNER_STATE_CONNECTED:
            {
                //While nim.dm 0,  resume the TunerProcess. - mtk40109 2009-07-21
                //Sync the PI state with HAL state by calling TunerAcq.
                // The last parameter must be NULL to guarantee TunerBreak return FALSE.
                if(bDetachMW == 0 && bPrevDetachMW == 1)
                {
                    TunerAcq(ptTunerCtx->TunerID, ptStateInfo->e_conn_type,
                            (void*)&ptStateInfo->t_conn_info,
                            ptStateInfo->z_conn_info_len,
                            TunerBreak, NULL);
                }
                
                sync_sts = TunerGetSync(ptTunerCtx->TunerID, ptStateInfo->e_conn_type);
                if (sync_sts)
                { /* lock & minitor */
                    if (TRUE == b_notified)
                    {
                        ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                DRV_COND_STATUS,
                                ptStateInfo->pv_tuner_nfy_tag,
                                TUNER_COND_STATUS_GOT_SIGNAL);
                            
                        b_notified = FALSE;
                    }

                    //! EWS notification
                    TunerGetAttribute(ptTunerCtx->TunerID, TUNER_GET_SPECIAL_TYPE_EWS_STATUS, (void*)&ewsInfo, (SIZE_T*)&ewsInfoLen);
                    if (ewsInfo.isNotify)
                    {//! EWS triggerred
                        if (ewsInfo.val)
                        {//! EWS ON
                            ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                    DRV_COND_STATUS,
                                    ptStateInfo->pv_tuner_nfy_tag,
                                    TUNER_COND_STATUS_EWS_ON);
                        }
                        else
                        {//! EWS OFF
                            ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                    DRV_COND_STATUS,
                                    ptStateInfo->pv_tuner_nfy_tag,
                                    TUNER_COND_STATUS_EWS_OFF);
                        }
                    }

                    uc_unluck = 0;
                    mcSEMA_LOCK_TIMEOUT(ptStateInfo->t_tuner_sync, cUNCLOCK_POLLING_TIME);
                }
                else
                { /* out of lock */
                    if (uc_unluck == 0)
                    {
                        mcGET_SYS_TIME(t0_unlock);
                    }                    
                    uc_unluck++;

                    mcGET_SYS_TIME(t1_unlock);
                    mcGET_DIFF_TIME(t_diff, t0_unlock, t1_unlock);
                    
                    if (mcCONV_SYS_TIME(t_diff) >= cUNLOCK_TIME_THRD)  // To fix bug:DTV00213128
                    //if (uc_unluck >= cUNLOCK_CNT_THRD)
                    {
                        mcSHOW_DRVAPI_MSG(("%s: Loss sync in TUNER_STATE_CONNECTED, try to sync %d times\n", ptStateInfo->comp_name, uc_unluck));
                        uc_unluck = 0;

                        if (fStayInConnectedState)
                        {
                            TunerStateChange(ptStateInfo, TUNER_STATE_CONNECTED, TUNER_STATE_CONNECTING);
                        }
                        else
                        {
                            if (!ptStateInfo->b_disable_nfy)
                            {
                                mcSHOW_DRVERR_MSG(("[TUNER_STATE_CONNECTED] b_disable_nfy = FALSE\n"));
                                TunerStateChange(ptStateInfo, TUNER_STATE_CONNECTED, TUNER_STATE_DISCONNECTED);
                                ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                        DRV_COND_DISCONNECTED,
                                        ptStateInfo->pv_tuner_nfy_tag,
                                        TUNER_DISC_REASON_SIGNAL_LOSS);
                            }
                            else
                            {
                                if (FALSE == b_notified)
                                {
                                    mcSHOW_DRVERR_MSG(("[TUNER_STATE_CONNECTED] b_disable_nfy = TRUE\n"));
                                    ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                                            DRV_COND_STATUS,
                                            ptStateInfo->pv_tuner_nfy_tag,
                                            TUNER_COND_STATUS_LOST_SIGNAL);

                                    b_notified = TRUE;
                                }
                            }
                        }
                    }
                    mcDELAY_MS(cLOCK_POLLING_TIME);
                }
                break;
            }
            
            case TUNER_STATE_DISCONNECTING:
            {
                mcSHOW_DRVAPI_MSG(("%s: Got disconnection command\n", ptStateInfo->comp_name));
                
                TunerStateChange(ptStateInfo, TUNER_STATE_DISCONNECTING, TUNER_STATE_DISCONNECTED);
                /*Modify for LTDIS protection: Move TunerDisc() from TunerDisconnect() to here. Let tunerbreak callback function can work correctly*/
                TunerDisc(ptTunerCtx->TunerID, ptStateInfo->e_conn_type);
                mcSEMA_UNLOCK(ptStateInfo->t_tuner_disconnecting);/*bug fix CR: DTV00139714*/
                
                if (!ptStateInfo->b_disable_nfy)
                {
                    mcSHOW_DRVERR_MSG(("[TUNER_STATE_DISCONNECTING] b_disable_nfy = FALSE\n"));

                    ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                            DRV_COND_DISCONNECTED,
                            ptStateInfo->pv_tuner_nfy_tag,
                            RM_DISC_REASON_AS_REQUESTED);
                }
                else
                {
                    mcSHOW_DRVERR_MSG(("[TUNER_STATE_DISCONNECTING] b_disable_nfy = TRUE\n"));
                    
                    ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                            DRV_COND_STATUS,
                            ptStateInfo->pv_tuner_nfy_tag,
                            TUNER_COND_STATUS_LOST_SIGNAL);
                }
                break;
            }
        } /* switch */
        
        bPrevDetachMW = 0;
        mcCHK_STATE();
    } /* while */

    ptStateInfo->t_tuner_thread = (THREAD_T)NULL;
    mcSHOW_DRVAPI_MSG(("TunerProcess %s exit!\n", ptStateInfo->thread_name));
    mcTHREAD_DESTROY();
}

#if TUNER_DTV_NOTIFY_ENABLE
/* Add the value for nofity MW when DTV Fine Tuner Freq */
INT32 Tuner_Notify(DRV_COND_T e_nfy_cond, UINT32 u4Msg)
{
    TUNER_CTX_T*        ptTunerCtx;
    STATE_INFO_T*       ptStateInfo;

    mcSHOW_DRVAPI_MSG(("[HALCmd]Tuner_Notify()\n"));    
    // Get corresponding TunerCtx and StateInfo
    if (GetTunerCtx(gPt_comp_id, &ptTunerCtx, &ptStateInfo)!= DRVAPI_TUNER_OK)
    {
        return (DRVAPI_TUNER_ERROR);
    }	
    ptStateInfo->pf_tuner_nfy(&ptStateInfo->t_tuner_comp_id,
                            e_nfy_cond,
                            ptStateInfo->pv_tuner_nfy_tag,
                            u4Msg);
	return (RMR_OK); 
}
#endif

/*
This function cancels tuner thread and close tuner I2C device.
*/
INT32 Tuner_Kill(void)
{
    TUNER_CTX_T*        ptTunerCtx;
    STATE_INFO_T*       ptStateInfo;

    if (!TunerInited)
    {
        mcSHOW_DRVERR_MSG(("Tuner had NOT been initialized.\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    //++ Make sure init_thread has been terminated
    while (t_init_thread)
    {
        mcDELAY_MS(10);    // in ms
    }
    
    if (t_init_sync)
    {
        mcSEMA_DESTROY(t_init_sync);
    }
    //--

    while (QueueGetHead(&TunerCtxList))
    {
        // Remove from TunerCtxList to prevent future possible calls of
        // exported APIs.
        ptTunerCtx = (TUNER_CTX_T*)QueuePopHead(&TunerCtxList);

        // Make sure no exported API is under-process
        if (ptTunerCtx->RefSema)
        {
            while (ptTunerCtx->RefCnt)
            {
                mcDELAY_MS(10);    // in ms
            }
        }

        // Destroy RefSema
        if (ptTunerCtx->RefSema)
        {
            mcSEMA_DESTROY(ptTunerCtx->RefSema);
        }

        // Clean up all StateInfos
        while (QueueGetHead(&ptTunerCtx->StateInfoList))
        {
            ptStateInfo = (STATE_INFO_T*)QueuePopHead(&ptTunerCtx->StateInfoList);

            // Terminate thread
            ptStateInfo->fgTunerThreadRunning = FALSE;
            /* Force Thread leave idle state, therefore can change to exit state */
            mcSEMA_UNLOCK(ptStateInfo->t_tuner_connecting);
            mcSEMA_UNLOCK(ptStateInfo->t_tuner_sync);

            while (ptStateInfo->t_tuner_thread)
            {
                mcDELAY_MS(10);    // in ms
            }

            // Destroy all semaphores in StateInfo
            if (ptStateInfo->t_tuner_state_lock)
            {
                mcMUTEX_DESTROY(ptStateInfo->t_tuner_state_lock);
            }
            if (ptStateInfo->t_tuner_disconnected)
            {
                mcSEMA_DESTROY(ptStateInfo->t_tuner_disconnected);
            }
            if (ptStateInfo->t_tuner_disconnecting)
            {
                mcSEMA_DESTROY(ptStateInfo->t_tuner_disconnecting);
            }
            if (ptStateInfo->t_tuner_connecting)
            {
                mcSEMA_DESTROY(ptStateInfo->t_tuner_connecting);
            }
            if (ptStateInfo->t_tuner_sync)
            {
                mcSEMA_DESTROY(ptStateInfo->t_tuner_sync);
            }

            // Free StateInfo
            x_mem_free(ptStateInfo);
        } // End of while (QueueGetHead(&ptTunerCtx->StateInfoList))

        // Free TunerCtx
        x_mem_free(ptTunerCtx);
    } // End of while(QueueGetHead(&TunerCtxList))

    TunerClose();

    TunerInited = 0;
    mcSHOW_DRVAPI_MSG(("Tuner_Kill: DRVAPI_TUNER_OK!\n"));

    return (DRVAPI_TUNER_OK);
}

#endif /* _X_DRVAPI_TUNER_C_ */
