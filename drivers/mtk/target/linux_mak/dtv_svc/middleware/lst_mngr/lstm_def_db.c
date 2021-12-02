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
 *         This file contains functions and internal variables for default
 *         database support of the lst_mngr library.
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"


#include "lst_mngr/_lstm.h"
#include "lst_mngr/lstm_def_db.h"

/*
  internal variable for the default database status.
*/
static UINT16   ui2_g_max_num_of_lst_id = 0;
static DEF_DB_ID_MAP_T* pt_g_def_db_id_map[LST_MNGR_NUM_LIST_TYPES] = {NULL};


/*----------------------------------------------
  Public Function for default database support.
------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: lstm_default_db_init
 *
 * Description: This function initializes the default database submodule.
 *
 * Inputs: 
 *    ui2_max_num_of_lst_id:    Maximum number of list ID's that can be
 *                              associated with default database.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           default database module is initialized successfully.
 *   LSTMR_INV_ARG      'ui2_max_num_of_lst_id' is 0.
 *
 ----------------------------------------------------------------------------*/
INT32 lstm_default_db_init(UINT16   ui2_max_num_of_lst_id)
{
    if ( ui2_max_num_of_lst_id > 0 )
    {
        INT32           ui4_i;
        for (ui4_i = 0; ui4_i < LST_MNGR_NUM_LIST_TYPES; ui4_i++)
        {
            pt_g_def_db_id_map[ui4_i] = (DEF_DB_ID_MAP_T *) x_mem_calloc(
                    ui2_max_num_of_lst_id,
                    sizeof(DEF_DB_ID_MAP_T));
            if (pt_g_def_db_id_map[ui4_i] == NULL)
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
            }
            x_memset(pt_g_def_db_id_map[ui4_i], 0, 
                     sizeof(DEF_DB_ID_MAP_T) * ui2_max_num_of_lst_id);
        }

        ui2_g_max_num_of_lst_id = ui2_max_num_of_lst_id;
    }
    else
    {
        return LSTMR_INV_ARG ;
    }
    
    return LSTMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_get_def_db_path
 * Description:
 *      get default database path and list ID for specified list type/id
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 lstm_get_def_db_path(UINT16               ui2_lst_id,
                           LST_MNGR_LST_TYPE_T  e_type,
                           HANDLE_T*            ph_dir,
                           const CHAR**         pps_def_db_path,
                           UINT16*              pui2_def_db_lst_id)
{
    UINT16      ui2_i;

    if (ui2_lst_id == LST_NULL_ID || e_type >= LST_MNGR_NUM_LIST_TYPES ||
        ph_dir == NULL || pps_def_db_path == NULL || pui2_def_db_lst_id == NULL)
    {
        return LSTMR_INV_ARG;
    }

    for (ui2_i = 0; ui2_i < ui2_g_max_num_of_lst_id; ui2_i++)
    {
        if (pt_g_def_db_id_map[e_type][ui2_i].ui2_lst_id == ui2_lst_id)
        {
            *ph_dir             = pt_g_def_db_id_map[e_type][ui2_i].h_dir;
            *pps_def_db_path    = pt_g_def_db_id_map[e_type][ui2_i].ps_lst_path;
            *pui2_def_db_lst_id = pt_g_def_db_id_map[e_type][ui2_i].ui2_def_db_lst_id;
            return LSTMR_OK;
        }
    }

    *ph_dir             = NULL_HANDLE;
    *pps_def_db_path    = NULL;
    *pui2_def_db_lst_id = LST_NULL_ID;
    return LSTMR_NOT_FOUND;
}

/*-----------------------------------------------------------------------------
 * Name:
 *      lstm_reg_def_db_path_map
 * Description:
 *      register default database path and list ID for specified list type/id
 * Inputs:
 * Outputs:
 * Returns:
 *-----------------------------------------------------------------------------*/
INT32 lstm_reg_def_db_path_map(UINT16               ui2_lst_id,
                               LST_MNGR_LST_TYPE_T  e_type,
                               HANDLE_T             h_dir,
                               const CHAR*          ps_def_db_path,
                               UINT16               ui2_def_db_lst_id)
{
    UINT16      ui2_i;
    CHAR*       ps_lst_path = NULL;
    BOOL        b_found = FALSE;

    if (ui2_lst_id == LST_NULL_ID || e_type >= LST_MNGR_NUM_LIST_TYPES ||
        ui2_def_db_lst_id == LST_NULL_ID || ps_def_db_path == NULL)
    {
        return LSTMR_INV_ARG;
    }

    for (ui2_i = 0; ui2_i < ui2_g_max_num_of_lst_id; ui2_i++)
    {
        if (pt_g_def_db_id_map[e_type][ui2_i].ui2_lst_id == ui2_lst_id)
        {
            b_found = TRUE;
            break;
        }
    }

    if (b_found)
    {
        if (pt_g_def_db_id_map[e_type][ui2_i].h_dir == h_dir &&
            pt_g_def_db_id_map[e_type][ui2_i].ui2_def_db_lst_id == ui2_def_db_lst_id &&
            x_strcmp(pt_g_def_db_id_map[e_type][ui2_i].ps_lst_path, ps_def_db_path))
        {
            /* already registerd */
            return LSTMR_OK;
        }
    }
    else
    {
        for (ui2_i = 0; ui2_i < ui2_g_max_num_of_lst_id; ui2_i++)
        {
            if (pt_g_def_db_id_map[e_type][ui2_i].ui2_lst_id == LST_NULL_ID)
            {
                /* empty slot found */
                break;
            }
        }
    }

    if (ui2_i >= ui2_g_max_num_of_lst_id)
    {
        return LSTMR_OUT_OF_SPACE;
    }
    ps_lst_path = x_strdup(ps_def_db_path);
    if (ps_lst_path == NULL)
    {
        return LSTMR_OUT_OF_MEM;
    }

    pt_g_def_db_id_map[e_type][ui2_i].ui2_lst_id    = ui2_lst_id;
    pt_g_def_db_id_map[e_type][ui2_i].ui2_def_db_lst_id = ui2_def_db_lst_id;
    pt_g_def_db_id_map[e_type][ui2_i].h_dir         = h_dir;
    if (pt_g_def_db_id_map[e_type][ui2_i].ps_lst_path != NULL)
    {
        x_mem_free((VOID *)pt_g_def_db_id_map[e_type][ui2_i].ps_lst_path);
    }
    pt_g_def_db_id_map[e_type][ui2_i].ps_lst_path   = ps_lst_path;

    return LSTMR_OK;
}
