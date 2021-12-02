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
 *         This file contains functions and internal variables to support
 *         CLI control of the lst_mngr library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"


#include "lst_mngr/_lstm.h"
#include "lst_mngr/lstm_dbg.h"
#include "lst_mngr/lstm_dual_bank.h"
#include "lst_mngr/lstm_def_db.h"

/*
  internal variable for the debug level.
*/
static UINT16  ui2_dbg_level = DBG_INIT_LEVEL_MW_LSTM;

CHAR* as_lst_name[] = {
    "SVL",
    "TSL",
    "NWL",
    "LOL",
    "SATL"
};

/*----------------------------------------------
  Public Function for DBG support.
------------------------------------------------*/
UINT16 lstm_get_dbg_level(VOID)
{
    return ui2_dbg_level;
}

VOID   lstm_set_dbg_level(UINT16  ui2_dbg)
{
    ui2_dbg_level = ui2_dbg;
}

/*-------------------------------------------------------------------------
 * Name: lstm_print_info
 *
 * Description: This API prints information for List Manager.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 --------------------------------------------------------------------------*/
VOID   lstm_print_info(UINT16    ui2_lst_id)
{
    UINT16      ui2_i;
    UINT16      ui2_def_db_lst_id;
    HANDLE_T    h_def_db_dir;
    const CHAR* ps_def_db_path;
    LST_MNGR_LST_TYPE_T e_type;

    if (ui2_lst_id != 0)
    {
        x_dbg_stmt("Initialized lists for ID %d:\r\n", ui2_lst_id);
        for (ui2_i = 0; ui2_i < LST_MNGR_NUM_LIST_TYPES; ui2_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) ui2_i;
            if (lst_mngr_check_lst_type_id(e_type, ui2_lst_id) == FALSE)
            {
                x_dbg_stmt("%s %d\r\n", as_lst_name[e_type], ui2_lst_id);
            }
        }
        x_dbg_stmt("\r\n");
    }

    x_dbg_stmt("Dual bank status: %s\r\n", lstm_get_svl_tsl_dual_bank() == \
               DUAL_BANK_SVL_TSL_SUB ? "SUB" : "MAIN");
    x_dbg_stmt("Dual bank path: h_dir=%d, main path: %s, sub path: %s\r\n",
               lstm_get_dual_bank_dir(), lstm_get_dual_bank_main_path(), 
               lstm_get_dual_bank_sub_path());
    x_dbg_stmt("\r\n");

    if (ui2_lst_id != 0)
    {
        x_dbg_stmt("Registered default database paths for list ID %d:\r\n",
                   ui2_lst_id);
        for (ui2_i = 0; ui2_i < LST_MNGR_NUM_LIST_TYPES; ui2_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) ui2_i;
            if (lstm_get_def_db_path(ui2_lst_id, e_type, &h_def_db_dir,
                                     &ps_def_db_path, &ui2_def_db_lst_id)
                == LSTMR_OK)
            {
                x_dbg_stmt("%s %d => h_dir=%d, path: %s, %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id,
                           h_def_db_dir, ps_def_db_path,
                           as_lst_name[e_type], ui2_def_db_lst_id);
            }
        }
    }
    x_dbg_stmt("====\r\n");
}
