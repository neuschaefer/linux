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
 * $RCSfile: lstm_dbg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains functions and internal variables for dual bank
 *         support of the lst_mngr library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"


#include "lst_mngr/_lstm.h"
#include "lst_mngr/lstm_dual_bank.h"

/*
  internal variable for the dual bank status.
*/
static UINT8 ui1_g_svl_tsl_dual_bank = DUAL_BANK_SVL_TSL_INVALID;
static BOOL  b_g_dual_bank_inited = FALSE;
static CHAR* s_g_main_path = NULL;
static CHAR* s_g_sub_path = NULL;
static HANDLE_T h_g_dir = NULL_HANDLE;
#ifndef LINUX_TURNKEY_SOLUTION
static x_lst_mngr_get_fct pf_g_get_svl_tsl_dual_bank = NULL;
static x_lst_mngr_set_fct pf_g_set_svl_tsl_dual_bank = NULL;
#else
static lst_mngr_get_fct pf_g_get_svl_tsl_dual_bank = NULL;
static lst_mngr_set_fct pf_g_set_svl_tsl_dual_bank = NULL;
static VOID*  pv_g_get_tag = NULL;
static VOID*  pv_g_set_tag = NULL;
#endif

/*----------------------------------------------
  Static Function for dummy get/set function.
------------------------------------------------*/
static INT32 _lstm_dummy_get (UINT8* pui1_result
#ifdef LINUX_TURNKEY_SOLUTION
                              , VOID* pv_tag
#endif
                             )
{
    return -1;
}
static INT32 _lstm_dummy_set (UINT8 ui1_result
#ifdef LINUX_TURNKEY_SOLUTION
                              , VOID* pv_tag
#endif
                             )
{
    return -1;
}

/*----------------------------------------------
  Public Function for dual bank support.
------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_get_svl_tsl_dual_bank
 * Description:
 *      get the svl tsl dual bank id.
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
UINT8 lstm_get_svl_tsl_dual_bank(VOID)
{
    INT32  i4_ret;

    if (!b_g_dual_bank_inited)
    {
        DBG_ERROR(("{LSTM} dual bank not inited!\r\n"));
        return DUAL_BANK_SVL_TSL_MAIN;
    }
    /*  Notice:
            1. Carefully using pf_g_set_svl_tsl_dual_bank() & pf_g_get_svl_tsl_dual_bank()
            2. They shall be used in the dual bank customized get/lock/unlock functions only.
            3. The application shall use lstm_get_svl_tsl_dual_bank to determine Main/Sub only.
    */
    if (ui1_g_svl_tsl_dual_bank == DUAL_BANK_SVL_TSL_INVALID)
    {
        /* 1. only get Main/Sub once when TV is powering on.
           2. cache Main/Sub and return Main/Sub directly by cached value after power on.
        */
#ifndef LINUX_TURNKEY_SOLUTION
        i4_ret = pf_g_get_svl_tsl_dual_bank(& ui1_g_svl_tsl_dual_bank);
#else
        i4_ret = pf_g_get_svl_tsl_dual_bank(& ui1_g_svl_tsl_dual_bank, pv_g_get_tag);
#endif
        if ((i4_ret != 0 /* OK */)
            ||
            (DUAL_BANK_GET_SVL_TSL_MAIN_SUB(ui1_g_svl_tsl_dual_bank) != DUAL_BANK_SVL_TSL_MAIN &&
             DUAL_BANK_GET_SVL_TSL_MAIN_SUB(ui1_g_svl_tsl_dual_bank) != DUAL_BANK_SVL_TSL_SUB))
        {
            ui1_g_svl_tsl_dual_bank = DUAL_BANK_SVL_TSL_MAIN;
        }

        if (DUAL_BANK_IS_SVL_TSL_FLUSHING(ui1_g_svl_tsl_dual_bank))
        {
            /* Notice:
               The flushing precess is broken. The SVL/TSL is not stored successfully.
               Use the other bank to resotre the previous SVL/TSL.
            */
            if (DUAL_BANK_GET_SVL_TSL_MAIN_SUB(ui1_g_svl_tsl_dual_bank) == DUAL_BANK_SVL_TSL_MAIN)
            {
                ui1_g_svl_tsl_dual_bank = DUAL_BANK_SVL_TSL_SUB;
            }
            else
            {
                ui1_g_svl_tsl_dual_bank = DUAL_BANK_SVL_TSL_MAIN;
            }
#ifndef LINUX_TURNKEY_SOLUTION
            pf_g_set_svl_tsl_dual_bank(ui1_g_svl_tsl_dual_bank);
#else
            pf_g_set_svl_tsl_dual_bank(ui1_g_svl_tsl_dual_bank, pv_g_set_tag);
#endif
        }
    }

    if (ui1_g_svl_tsl_dual_bank == DUAL_BANK_SVL_TSL_MAIN)
    {
        return DUAL_BANK_SVL_TSL_MAIN;
    }
    else
    {
        /* Notice:
                Make sure the values will be expected. For example: the value got in above
                codes is 255 (same as DUAL_BANK_SVL_TSL_INVALID) and successfully return.
                In this case, the flow of getting values will enter twice instead of once. <- Exception
        */
        ui1_g_svl_tsl_dual_bank = DUAL_BANK_SVL_TSL_SUB;

        return DUAL_BANK_SVL_TSL_SUB;
    }
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_lock_svl_tsl_dual_bank
 * Description:
 *      turn on dirty flag and store with the svl tsl dual bank id.
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID lstm_lock_svl_tsl_dual_bank(VOID)
{
    if (!b_g_dual_bank_inited)
    {
        DBG_ERROR(("{LSTM} dual bank not inited!\r\n"));
        return;
    }
    /*  1. Toggle Main/Sub in Config for next power-on
        2. Actually toggle Main/Sub when the SVL/TSL is storing and stored.
    */
    if (ui1_g_svl_tsl_dual_bank == DUAL_BANK_SVL_TSL_INVALID)
    {
        /* 1. Main/Sub ID is reset by lstm_reset_svl_tsl_dual_bank ()
           2. Get current Main/Sub ID.
        */
        lstm_get_svl_tsl_dual_bank();
    }

    if (ui1_g_svl_tsl_dual_bank == DUAL_BANK_SVL_TSL_MAIN)
    {
#ifndef LINUX_TURNKEY_SOLUTION
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_SUB | DUAL_BANK_SVL_TSL_FLUSHING);
#else
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_SUB | DUAL_BANK_SVL_TSL_FLUSHING, pv_g_set_tag);
#endif
    }
    else
    {
#ifndef LINUX_TURNKEY_SOLUTION
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_MAIN | DUAL_BANK_SVL_TSL_FLUSHING);
#else
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_MAIN | DUAL_BANK_SVL_TSL_FLUSHING, pv_g_set_tag);
#endif
    }
    /* immediately store dual bank status */
    /*a_cfg_store();*/
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_unlock_svl_tsl_dual_bank
 * Description:
 *      turn off dirty flag and store with the svl tsl dual bank id.
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID lstm_unlock_svl_tsl_dual_bank(VOID)
{
    if (!b_g_dual_bank_inited)
    {
        DBG_ERROR(("{LSTM} dual bank not inited!\r\n"));
        return;
    }
    /*
        1. Toggle Main/Sub in Config for next power-on
        2. Actually toggle Main/Sub when the SVL/TSL is storing and stored.
    */
    if (ui1_g_svl_tsl_dual_bank == DUAL_BANK_SVL_TSL_INVALID)
    {
        /* 1. Main/Sub ID is reset by lstm_reset_svl_tsl_dual_bank ()
           2. Get current Main/Sub ID.
        */
        lstm_get_svl_tsl_dual_bank();
    }

    if (ui1_g_svl_tsl_dual_bank == DUAL_BANK_SVL_TSL_MAIN)
    {
#ifndef LINUX_TURNKEY_SOLUTION
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_SUB);
#else
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_SUB, pv_g_set_tag);
#endif
    }
    else
    {
#ifndef LINUX_TURNKEY_SOLUTION
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_MAIN);
#else
        pf_g_set_svl_tsl_dual_bank(DUAL_BANK_SVL_TSL_MAIN, pv_g_set_tag);
#endif
    }
    /* immediately store dual bank status */
    /*a_cfg_store();*/
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_reset_svl_tsl_dual_bank
 * Description:
 *      reset dirty flag for next load with the svl tsl dual bank id.
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
VOID lstm_reset_svl_tsl_dual_bank(VOID)
{
    /*
        If there are reload request in software, this API shall be invoked
        after all svl/tsl store are successfully finished.
    */
    ui1_g_svl_tsl_dual_bank = DUAL_BANK_SVL_TSL_INVALID;
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_get_dual_bank_dir
 * Description:
 *      get the dual bank directory handle.
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
HANDLE_T lstm_get_dual_bank_dir(VOID)
{
    return h_g_dir;
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_get_dual_bank_main_path
 * Description:
 *      get the dual bank main path.
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
const CHAR* lstm_get_dual_bank_main_path(VOID)
{
    return s_g_main_path;
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_get_dual_bank_sub_path
 * Description:
 *      get the dual bank sub path.
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
const CHAR* lstm_get_dual_bank_sub_path(VOID)
{
    return s_g_sub_path;
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_reg_dual_bank_path_callback
 * Description:
 *      register dual bank path and get/set callback function
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
#ifndef LINUX_TURNKEY_SOLUTION
INT32 lstm_reg_dual_bank_path_callback(HANDLE_T h_dir,
                                       const CHAR* ps_main_path,
                                       const CHAR* ps_sub_path,
                                       x_lst_mngr_get_fct pf_get,
                                       x_lst_mngr_set_fct pf_set)
#else
INT32 lstm_reg_dual_bank_path_callback_with_tag(HANDLE_T h_dir,
                                                const CHAR* ps_main_path,
                                                const CHAR* ps_sub_path,
                                                lst_mngr_get_fct pf_get,
                                                lst_mngr_set_fct pf_set,
                                                VOID* pv_get_tag,
                                                VOID* pv_set_tag)
#endif
{
    if (b_g_dual_bank_inited)
    {
        if (h_dir == NULL_HANDLE && ps_main_path == NULL && ps_sub_path == NULL &&
            pf_get == NULL && pf_set == NULL) /* uninit */
        {
            h_g_dir = NULL_HANDLE;
            pf_g_get_svl_tsl_dual_bank = NULL;
            pf_g_set_svl_tsl_dual_bank = NULL;
#ifdef LINUX_TURNKEY_SOLUTION
            pv_g_get_tag = NULL;
            pv_g_set_tag = NULL;
#endif
            if (s_g_main_path != NULL)
            {
                x_mem_free(s_g_main_path);
                s_g_main_path = NULL;
            }
            if (s_g_sub_path != NULL)
            {
                x_mem_free(s_g_sub_path);
                s_g_sub_path = NULL;
            }
            b_g_dual_bank_inited = FALSE;
            return LSTMR_OK;
        }
        else
        {
            return LSTMR_ALREADY_INITED;
        }
    }
    if (ps_main_path == NULL)
    {
        return LSTMR_INV_ARG;
    }
    if (ps_sub_path == NULL) /* single bank */
    {
        DBG_INFO(("{LSTM} Register single bank %d %s\r\n", h_dir, ps_main_path));
        s_g_main_path = x_strdup(ps_main_path);
        if (s_g_main_path == NULL)
        {
            return LSTMR_OUT_OF_MEM;
        }
        s_g_sub_path = NULL;
        h_g_dir = h_dir;
        pf_g_get_svl_tsl_dual_bank = _lstm_dummy_get;
        pf_g_set_svl_tsl_dual_bank = _lstm_dummy_set;
#ifdef LINUX_TURNKEY_SOLUTION
        pv_g_get_tag = NULL;
        pv_g_set_tag = NULL;
#endif
        b_g_dual_bank_inited = TRUE;
    }
    else
    {
        DBG_INFO(("{LSTM} Register dual bank %d %s %s\r\n", h_dir, ps_main_path, ps_sub_path));
        if (pf_get == NULL || pf_set == NULL)
        {
            return LSTMR_INV_ARG;
        }

        s_g_main_path = x_strdup(ps_main_path);
        if (s_g_main_path == NULL)
        {
            return LSTMR_OUT_OF_MEM;
        }
        s_g_sub_path = x_strdup(ps_sub_path);
        if (s_g_sub_path == NULL)
        {
            x_mem_free(s_g_main_path);
            s_g_main_path = NULL;
            return LSTMR_OUT_OF_MEM;
        }
        h_g_dir = h_dir;
        pf_g_get_svl_tsl_dual_bank = pf_get;
        pf_g_set_svl_tsl_dual_bank = pf_set;
#ifdef LINUX_TURNKEY_SOLUTION
        pv_g_get_tag = pv_get_tag;
        pv_g_set_tag = pv_set_tag;
#endif
        b_g_dual_bank_inited = TRUE;
    }
    return LSTMR_OK;
}
#ifdef LINUX_TURNKEY_SOLUTION
INT32 lstm_get_dual_bank_cb_tag(lst_mngr_get_fct* ppf_get,
                                lst_mngr_set_fct* ppf_set,
                                VOID**  ppv_get_tag,
                                VOID**  ppv_set_tag)
{
    if (ppf_get == NULL || ppf_set == NULL ||
        ppv_get_tag == NULL || ppv_set_tag == NULL)
    {
        return LSTMR_INV_ARG;
    }
    *ppf_get = pf_g_get_svl_tsl_dual_bank;
    *ppf_set = pf_g_set_svl_tsl_dual_bank;
    *ppv_get_tag = pv_g_get_tag;
    *ppv_set_tag = pv_g_set_tag;
    return LSTMR_OK;
}
#endif
