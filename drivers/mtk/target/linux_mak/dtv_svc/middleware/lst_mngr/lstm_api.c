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
 * $RCSfile: lstm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/18 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: eeeee16d218b478d12d6bd39898cde8d $
 *
 * Description:
 *         This file contain implementation of Large Object List API's,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/


#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "cdb/cdb_api.h"

#include "lst_mngr/_lstm.h"
#include "lst_mngr/lstm_dbg.h"
#include "lst_mngr/x_lst_mngr.h"
#include "lst_mngr/lstm_dual_bank.h"
#include "lst_mngr/lstm_def_db.h"

#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "nwl/x_nwl.h"
#include "lol/x_lol.h"
#include "satl/x_satl.h"

#include "file_mngr/x_fm.h"
#include "util/fs_raw.h"

/*-----------------------------------------------------------------------------
 * Name: x_lst_mngr_init_list_id_by_type
 *
 * Description: 
 *
 * Inputs: 
 *   ui2_lst_id     List ID.
 *   e_type         List type.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           List is initialized successfully.
 *   LSTMR_INV_ARG      'ui2_lst_id' is 0.
 *   LSTMR_FAIL         The specified 'ui2_lst_id' is already initialized,
 *                      or it failed opening list handle.
 * 
 ----------------------------------------------------------------------------*/
INT32 x_lst_mngr_init_list_id_by_type(UINT16            ui2_lst_id,
                                      LST_MNGR_LST_TYPE_T e_type)
{
    HANDLE_T    h_handle = NULL_HANDLE;
    INT32       i4_rc=LSTMR_FAIL;
    
    if ( ui2_lst_id != 0            &&
         e_type < LST_MNGR_NUM_LIST_TYPES )
    {
        lst_mngr_sema_lock();
        /* check if the input list type and id has become invalid. */
        if ( lst_mngr_get_handle_by_type_id(e_type, ui2_lst_id, &h_handle)  &&
             !x_handle_valid(h_handle)
            )
        {
            lst_mngr_del_type_id(e_type, ui2_lst_id);
        }
        if ( lst_mngr_check_lst_type_id(e_type, ui2_lst_id) == TRUE )
        {
            switch(e_type)
            {
                case LST_TYPE_SVL:
                    i4_rc = x_svl_open(ui2_lst_id, NULL, NULL, &h_handle);
                    break;

                case LST_TYPE_TSL:
                    i4_rc = x_tsl_open(ui2_lst_id, NULL, NULL, &h_handle);
                    break;

                case LST_TYPE_NWL:
                    i4_rc = x_nwl_open(ui2_lst_id, NULL, NULL, &h_handle);
                    break;

                case LST_TYPE_LOL:
                    i4_rc = x_lol_open(ui2_lst_id, NULL, NULL, &h_handle);
                    break;

                case LST_TYPE_SATL:
#ifdef MW_DVBS_DEV_ENABLE
                    i4_rc = x_satl_open(ui2_lst_id, NULL, NULL, &h_handle);
#endif
                    break;

                default:
                    i4_rc = LSTMR_INV_ARG;
                    break;
            }
            if ( i4_rc != LSTMR_OK ||
                 lst_mngr_add_lst_type_id_handle(e_type, ui2_lst_id, h_handle)
                   != LSTMR_OK
               )
            {
                DBG_ERROR(("{LSTM} failed adding handle for %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id));
                i4_rc = LSTMR_FAIL;
            }
        }
        else
        {
            DBG_ERROR(("{LSTM} handle for %s %d already inited\r\n",
                       as_lst_name[e_type], ui2_lst_id));
        }
        /* Add the name and id to the internal list. */
        lst_mngr_sema_unlock();
    }
    else
    {
        i4_rc=LSTMR_INV_ARG;
    }

    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_deinit_list_id_by_type
 *
 * Description: This API deletes the specified List handle associated 
 *              with specified type & ID.
 *
 * Inputs: 
 *   ui2_lst_id     List ID.
 *   e_type         List type.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           Specified List is de-initialized successfully.
 *   LSTMR_INV_ARG      'ui2_lst_id' is 0.
 *   LSTMR_FAIL         The specified 'ui2_lst_id' is not initialized,
 *                      or it failed closing list handle.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_deinit_list_id_by_type(UINT16            ui2_lst_id,
                                        LST_MNGR_LST_TYPE_T e_type)
{
    HANDLE_T        h_handle = NULL_HANDLE;
    INT32           i4_rc = LSTMR_OK;

    if ( ui2_lst_id != 0            &&
         e_type < LST_MNGR_NUM_LIST_TYPES )
    {
        lst_mngr_sema_lock();
        /* remove list id/handle pairs. */
        if ( !lst_mngr_get_handle_by_type_id(e_type, ui2_lst_id, &h_handle) )
        {
            DBG_ERROR(("{LSTM} failed getting handle for %s %d\r\n",
                       as_lst_name[e_type], ui2_lst_id));
            i4_rc = LSTMR_FAIL;
        }
        lst_mngr_del_type_id(e_type, ui2_lst_id);
        lst_mngr_sema_unlock();
        
        if (i4_rc == LSTMR_OK)
        {
            switch(e_type)
            {
                case LST_TYPE_SVL:
                    i4_rc = x_svl_close(h_handle);
                    break;

                case LST_TYPE_TSL:
                    i4_rc = x_tsl_close(h_handle);
                    break;

                case LST_TYPE_NWL:
                    i4_rc = x_nwl_close(h_handle);
                    break;

                case LST_TYPE_LOL:
                    i4_rc = x_lol_close(h_handle);
                    break;

                case LST_TYPE_SATL:
#ifdef MW_DVBS_DEV_ENABLE
                    i4_rc = x_satl_close(h_handle);
#endif
                    break;

                default:
                    i4_rc = LSTMR_INV_ARG;
                    break;
            }
            if ( i4_rc != LSTMR_OK )
            {
                DBG_ERROR(("{LSTM} failed closing handle for %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id));
                i4_rc = LSTMR_FAIL;
            }
        }
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_lst_mngr_init_list_id
 *
 * Description: 
 *
 * Inputs: 
 *   ui2_lst_id     List ID.
 *   ui4_lst_mask   Bit mask indicating what type of lists are to be inited.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           List is initialized successfully.
 *   LSTMR_INV_ARG      'ui2_lst_id' is 0.
 *   LSTMR_FAIL         The specified 'ui2_lst_id' is already initialized,
 *                      or it failed opening list handle.
 * 
 ----------------------------------------------------------------------------*/
INT32 x_lst_mngr_init_list_id(UINT16            ui2_lst_id,
                              UINT32            ui4_lst_mask)
{
    LST_MNGR_LST_TYPE_T e_type;
    INT32               i4_i;
    INT32               i4_rc = LSTMR_OK;

    if ( ui2_lst_id != 0            &&
         ui4_lst_mask != 0 )
    {
        for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) i4_i;
            if ( (ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0 )
            {
                continue;
            }

            i4_rc = x_lst_mngr_init_list_id_by_type(ui2_lst_id, e_type);
            if ( i4_rc != LSTMR_OK )
            {
                break;
            }
        }
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_lst_mngr_deinit_list_id
 *
 * Description: 
 *
 * Inputs: 
 *   ui2_lst_id     List ID.
 *   ui4_lst_mask   Bit mask indicating what type of lists are to be deinited.
 *   b_force        Specifies whether to ignore errors and force to proceed.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           List is initialized successfully.
 *   LSTMR_INV_ARG      'ui2_lst_id' is 0.
 *   LSTMR_FAIL         The specified 'ui2_lst_id' is not initialized,
 *                      or it failed closing list handle.
 * 
 ----------------------------------------------------------------------------*/
INT32 x_lst_mngr_deinit_list_id(UINT16            ui2_lst_id,
                                UINT32            ui4_lst_mask,
                                BOOL              b_force)
{
    LST_MNGR_LST_TYPE_T e_type;
    INT32               i4_i;
    INT32               i4_rc = LSTMR_OK;

    if ( ui2_lst_id != 0            &&
         ui4_lst_mask != 0 )
    {
        for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) i4_i;
            if ( (ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0 )
            {
                continue;
            }

            i4_rc = x_lst_mngr_deinit_list_id_by_type(ui2_lst_id, e_type);
            if ( i4_rc != LSTMR_OK && !b_force )
            {
                break;
            }
        }
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }
    return i4_rc;
}


/********************************************
   List data persistent storage related APIs
*********************************************/

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_fs_store
 *
 * Description: This API stores the List objects specified by bitmask to
 *              the file system.
 *
 * Inputs: 
 *   h_dir        Handle to the parent directory of the file specified by
 *                the path 'ps_lst_path'.  If 'ps_lst_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_lst_path  Specifies the file path to the file where the list objects
 *                are stored in the file system.
 *   ui2_lst_id   Specifies the  ID of the list objects to be stored to the
 *                file system.  The limit for list ID is between 1 and 65535.
 *   ui4_lst_mask Bit mask indicating what type of lists are to be stored.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           List objects is stored to the file system. 
 *   LSTMR_NOT_FOUND    The List object specified by the 'ui2_lst_id' does
 *                      not exist.
 *   LSTMR_INV_ARG      'ps_lst_path' is NULL.
 *   LSTMR_FAIL         List objects store failure.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_fs_store(HANDLE_T          h_dir,
                          const CHAR*       ps_lst_path,
                          UINT16            ui2_lst_id,
                          UINT32            ui4_lst_mask)
{
    LST_MNGR_LST_TYPE_T e_type;
    BOOL                b_failed = FALSE;
    HANDLE_T            h_file;
    INT32               i4_i;
    INT32               i4_rc = LSTMR_OK;
    
    if ( ps_lst_path != NULL &&
         ui2_lst_id  != 0    &&
         ui4_lst_mask != 0
        )
    {
        if ( fs_raw_is_inited_ex(h_dir, ps_lst_path, FM_READ_WRITE, &h_file) == FALSE )
        {
            DBG_INFO(("{LSTM} init fs ms=%d\r\n", x_os_get_sys_tick() * x_os_get_sys_tick_period()));
            if ( fs_raw_init(h_dir, ps_lst_path) != FSRAWR_OK )
            {
                return LSTMR_INV_FILE_PATH;
            }

            DBG_INFO(("{LSTM} open file ms=%d\r\n", x_os_get_sys_tick() * x_os_get_sys_tick_period()));
            /*
              Open the RAW file.
            */
            i4_rc=x_fm_open(h_dir,
                            ps_lst_path,
                            FM_OPEN_CREATE | FM_READ_WRITE,
                            0644,
                            FALSE,
                            &h_file);
        }
        else
        {
            i4_rc = FMR_OK;
        }

        if ( i4_rc == FMR_OK )
        {
            lst_mngr_sema_lock();

            for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
            {
                e_type = (LST_MNGR_LST_TYPE_T) i4_i;
                if ( (ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0 )
                {
                    continue;
                }

                DBG_INFO(("{LSTM} storing %s %d ms=%d\r\n", as_lst_name[e_type], ui2_lst_id,
                          x_os_get_sys_tick() * x_os_get_sys_tick_period()));
                switch(e_type)
                {
                    case LST_TYPE_SVL:
                        i4_rc = x_svl_fs_store_given_file_handle(h_file, ui2_lst_id);
                        break;

                    case LST_TYPE_TSL:
                        i4_rc = x_tsl_fs_store_given_file_handle(h_file, ui2_lst_id);
                        break;

                    case LST_TYPE_NWL:
                        i4_rc = x_nwl_fs_store_given_file_handle(h_file, ui2_lst_id);
                        break;

                    case LST_TYPE_LOL:
                        i4_rc = x_lol_fs_store_given_file_handle(h_file, ui2_lst_id);
                        break;

                    case LST_TYPE_SATL:
#ifdef MW_DVBS_DEV_ENABLE
                        i4_rc = x_satl_fs_store_given_file_handle(h_file, ui2_lst_id);
#endif
                        break;

                    default:
                        i4_rc = LSTMR_INV_ARG;
                        break;
                }
                if ( i4_rc != LSTMR_OK )
                {
                    DBG_ERROR(("{LSTM} failed storing data for %s %d, ret=%d\r\n",
                               as_lst_name[e_type], ui2_lst_id, i4_rc));
                    b_failed = TRUE;
                }
            }
            if (b_failed)
            {
                i4_rc = LSTMR_FAIL;
            }
            lst_mngr_sema_unlock();
            DBG_INFO(("{LSTM} closing file ms=%d\r\n", x_os_get_sys_tick() * x_os_get_sys_tick_period()));
            x_fm_close( h_file );
        }
        else
        {
            DBG_ERROR(("{LSTM} failed opening RAW file for list ID %d\r\n",
                       ui2_lst_id));
            i4_rc = LSTMR_FAIL;
        }
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }
    DBG_INFO(("{LSTM} done ms=%d\r\n", x_os_get_sys_tick() * x_os_get_sys_tick_period()));
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_fs_sync
 *
 * Description: This API loads the List objects specified by bitmask from
 *              the file system.
 *
 * Inputs: 
 *   h_dir        Handle to the parent directory of the file specified by
 *                the path 'ps_lst_path'.  If 'ps_lst_path' is an absolute
 *                path, then this argument should be set to 'FM_ROOT_HANDLE'.
 *   ps_lst_path  Specifies the file path to the file where the list objects
 *                are loaded from the file system.
 *   ui2_lst_id   Specifies the  ID of the list objects to be loaded from the
 *                file system.  The limit for list ID is between 1 and 65535.
 *   ui4_lst_mask Bit mask indicating what type of lists are to be loaded.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           List objects is loaded from the file system. 
 *   LSTMR_NOT_FOUND    The List object specified by the 'ui2_lst_id' does
 *                      not exist.
 *   LSTMR_INV_ARG      'ps_lst_path' is NULL.
 *   LSTMR_FAIL         List objects sync failure.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_fs_sync(HANDLE_T          h_dir,
                         const CHAR*       ps_lst_path,
                         UINT16            ui2_lst_id,
                         UINT32            ui4_lst_mask)
{
    LST_MNGR_LST_TYPE_T e_type;
    BOOL                b_failed = FALSE;
    HANDLE_T            h_handle;
    HANDLE_T            h_file;
    INT32               i4_i;
    INT32               i4_rc = LSTMR_OK;
    
    if ( ps_lst_path != NULL &&
         ui2_lst_id  != 0    &&
         ui4_lst_mask != 0
        )
    {
        if ( fs_raw_is_inited_ex(h_dir, ps_lst_path, FM_READ_ONLY, &h_file) == FALSE )
        {
            return LSTMR_FILE_READ_FAIL;
        }

        lst_mngr_sema_lock();
        
        for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) i4_i;
            if ((ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0)
            {
                continue;
            }

            if ( !lst_mngr_get_handle_by_type_id(e_type, ui2_lst_id, &h_handle) )
            {
                DBG_ERROR(("{LSTM} failed getting handle for %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id));
                i4_rc = LSTMR_NOT_FOUND;
                break;
            }

            switch(e_type)
            {
                case LST_TYPE_SVL:
                    i4_rc = x_svl_fs_sync_given_file_handle(h_handle, h_file, ui2_lst_id);
                    break;

                case LST_TYPE_TSL:
                    i4_rc = x_tsl_fs_sync_given_file_handle(h_handle, h_file, ui2_lst_id);
                    break;

                case LST_TYPE_NWL:
                    i4_rc = x_nwl_fs_sync_given_file_handle(h_handle, h_file, ui2_lst_id);
                    break;

                case LST_TYPE_LOL:
                    i4_rc = x_lol_fs_sync_given_file_handle(h_handle, h_file, ui2_lst_id);
                    break;

                case LST_TYPE_SATL:
#ifdef MW_DVBS_DEV_ENABLE
                    i4_rc = x_satl_fs_sync_given_file_handle(h_handle, h_file, ui2_lst_id);
#endif
                    break;

                default:
                    i4_rc = LSTMR_INV_ARG;
                    break;
            }
            if ( i4_rc != LSTMR_OK )
            {
                DBG_ERROR(("{LSTM} failed loading data for %s %d, ret=%d\r\n",
                           as_lst_name[e_type], ui2_lst_id, i4_rc));
                b_failed = TRUE;
            }
        }
        if (b_failed)
        {
            i4_rc = LSTMR_FAIL;
        }
        lst_mngr_sema_unlock();

        x_fm_close( h_file );
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_reg_dual_bank
 *
 * Description: This API initializes dual bank setting by registering the
 *              MAIN/SUB bank path and function callbacks for get/set ACFG
 *              NVM status flag.
 *
 * Inputs: 
 *   h_dir        Handle to the parent directory of the file specified by
 *                the path 'ps_main_path' and 'ps_sub_path'.  If main/sub
 *                path are an absolute paths, then this argument should be
 *                set to 'FM_ROOT_HANDLE'.
 *   ps_main_path Specifies the path of the main bank on NVM to store/load
 *                list data when dual bank support is enabled, or the path
 *                of the single bank on NVM to store/load list data when
 *                dual bank support is disabled.
 *   ps_sub_path  Specifies the path of the sub bank on NVM to store/load
 *                list data when dual bank support is enabled. If dual bank
 *                support is disabled, this field shall be of NULL value.
 *   pf_get       Pointer to the NVM status get function.
 *   pf_set       Pointer to the NVM status set function.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           Dual bank setting is registered.
 *   LSTMR_INV_ARG      'ps_main_path' is NULL, or 'pf_get', 'pf_set' not
 *                      specified.
 *   LSTMR_OUT_OF_MEM   Memory allocation failure.
 -----------------------------------------------------------------------*/
#ifdef LINUX_TURNKEY_SOLUTION
INT32 lst_mngr_reg_dual_bank(HANDLE_T             h_dir,
                             const CHAR*          ps_main_path,
                             const CHAR*          ps_sub_path,
                             lst_mngr_get_fct     pf_get,
                             lst_mngr_set_fct     pf_set,
                             VOID*                pv_get_tag,
                             VOID*                pv_set_tag)
{
    INT32               i4_rc = LSTMR_OK;

    lst_mngr_sema_lock();

    i4_rc = lstm_reg_dual_bank_path_callback_with_tag(
            h_dir, ps_main_path, ps_sub_path,
            pf_get, pf_set, pv_get_tag, pv_set_tag);

    lst_mngr_sema_unlock();

    return i4_rc;
}
static INT32 _wrapped_get_fct(UINT8* pui1_val, VOID* pv_tag)
{
    return ((x_lst_mngr_get_fct)(pv_tag))(pui1_val);
}
static INT32 _wrapped_set_fct(UINT8 ui1_val, VOID* pv_tag)
{
    return ((x_lst_mngr_set_fct)(pv_tag))(ui1_val);
}
#endif
INT32 x_lst_mngr_reg_dual_bank(HANDLE_T             h_dir,
                               const CHAR*          ps_main_path,
                               const CHAR*          ps_sub_path,
                               x_lst_mngr_get_fct   pf_get,
                               x_lst_mngr_set_fct   pf_set)
{
#ifndef LINUX_TURNKEY_SOLUTION
    INT32               i4_rc = LSTMR_OK;

    lst_mngr_sema_lock();

    i4_rc = lstm_reg_dual_bank_path_callback(h_dir, ps_main_path, ps_sub_path,
                                             pf_get, pf_set);

    lst_mngr_sema_unlock();

    return i4_rc;
#else
    return lst_mngr_reg_dual_bank(h_dir, ps_main_path, ps_sub_path,
                                  _wrapped_get_fct, _wrapped_set_fct,
                                  (VOID*)pf_get, (VOID*)pf_set);
#endif
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_dual_bank_fs_store
 *
 * Description: This API stores the List objects specified by bitmask to
 *              the file system, path determined by dual bank status. It
 *              shall be invoked after dual bank initialization.

 *
 * Inputs: 
 *   ui2_lst_id   Specifies the  ID of the list objects to be stored to the
 *                file system.  The limit for list ID is between 1 and 65535.
 *   ui4_lst_mask Bit mask indicating what type of lists are to be stored.
 *   e_as_mode    Specifies where to store the lists to. If the value is
 *                BY_NVM, get/set callbacks for dual bank is invoked to
 *                determine which bank to store. Otherwise, lists are stored
 *                to the corresponding bank as the name suggests, i.e.
 *                TO_MAIN_SUB, TO_MAIN, TO_SUB.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           List objects is stored to the file system. 
 *   LSTMR_INV_ARG      'e_as_mode' is invalid.
 *   LSTMR_FAIL         List objects store failure.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_dual_bank_fs_store(UINT16            ui2_lst_id,
                                    UINT32            ui4_lst_mask,
                                    LST_MNGR_DUAL_BANK_AS_MODE_T e_as_mode)
{
    const CHAR*         ps_lst_path [2];
    INT32               i4_ret = LSTMR_OK;
    
    switch (e_as_mode)
    {
        case LST_MNGR_DUAL_BANK_AS_MODE_BY_NVM:
            if (DUAL_BANK_SVL_TSL_SUB == lstm_get_svl_tsl_dual_bank())
            {
                ps_lst_path[0] = lstm_get_dual_bank_sub_path();
            }
            else
            {
                ps_lst_path[0] = lstm_get_dual_bank_main_path();
            }
            ps_lst_path [1] = NULL;
            break;
        case LST_MNGR_DUAL_BANK_AS_MODE_TO_MAIN_SUB:
            ps_lst_path [0] = lstm_get_dual_bank_main_path();
            ps_lst_path [1] = lstm_get_dual_bank_sub_path();
            break;
        case LST_MNGR_DUAL_BANK_AS_MODE_TO_MAIN:
            ps_lst_path [0] = lstm_get_dual_bank_main_path();
            ps_lst_path [1] = NULL;
            break;
        case LST_MNGR_DUAL_BANK_AS_MODE_TO_SUB:
            ps_lst_path [0] = lstm_get_dual_bank_sub_path();
            ps_lst_path [1] = NULL;
            break;

        default:
            return LSTMR_INV_ARG;
    }
    if (ps_lst_path[0] == NULL)
    {
        DBG_ERROR(("{LSTM} cannot determine path to store %d\r\n", ui2_lst_id));
        return LSTMR_FAIL;
    }

    lstm_lock_svl_tsl_dual_bank();

    if (ps_lst_path [0] != NULL)
    {
        DBG_INFO(("{LSTM} Storing to %s\r\n", ps_lst_path[0]));
        i4_ret = x_lst_mngr_fs_store(lstm_get_dual_bank_dir(),
                                     ps_lst_path [0],
                                     ui2_lst_id,
                                     ui4_lst_mask);
        if(i4_ret != LSTMR_OK)
        {
            DBG_ERROR(("{LSTM} failed storing %d on %s\r\n", ui2_lst_id, ps_lst_path [0]));
        }
    }

    if (ps_lst_path [1] != NULL)
    {
        DBG_INFO(("{LSTM} Storing to %s\r\n", ps_lst_path[1]));
        i4_ret = x_lst_mngr_fs_store(lstm_get_dual_bank_dir(),
                                     ps_lst_path [1],
                                     ui2_lst_id,
                                     ui4_lst_mask);
        if(i4_ret != LSTMR_OK)
        {
            DBG_ERROR(("{LSTM} failed storing %d on %s\r\n", ui2_lst_id, ps_lst_path [1]));
        }
    }
    lstm_unlock_svl_tsl_dual_bank();

    return i4_ret;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_dual_bank_fs_sync
 *
 * Description: This API loads the List objects specified by bitmask from
 *              the file system, path determined by dual bank status. It
 *              shall be invoked after dual bank initialization.
 *
 * Inputs: 
 *   ui2_lst_id   Specifies the  ID of the list objects to be loaded from the
 *                file system.  The limit for list ID is between 1 and 65535.
 *   ui4_lst_mask Bit mask indicating what type of lists are to be loaded.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           List objects is loaded from the file system. 
 *   LSTMR_NOT_FOUND    The List object specified by the 'ui2_lst_id' does
 *                      not exist.
 *   LSTMR_INV_ARG      Main/sub bank path is NULL.
 *   LSTMR_FAIL         List objects sync failure.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_dual_bank_fs_sync(UINT16            ui2_lst_id,
                                   UINT32            ui4_lst_mask)
{
    const CHAR*         ps_lst_path;
    
    if (DUAL_BANK_SVL_TSL_SUB == lstm_get_svl_tsl_dual_bank())
    {
        ps_lst_path = lstm_get_dual_bank_main_path();
    }
    else
    {
        ps_lst_path = lstm_get_dual_bank_sub_path();
        if (ps_lst_path == NULL) /* dual bank not enabled */
        {
            ps_lst_path = lstm_get_dual_bank_main_path();
        }
    }
    
    DBG_INFO(("{LSTM} Loading from %s\r\n", ps_lst_path));
    return x_lst_mngr_fs_sync(lstm_get_dual_bank_dir(),
                              ps_lst_path,
                              ui2_lst_id,
                              ui4_lst_mask);
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_get_dual_bank_status
 *
 * Description: This API returns the dual bank status (MAIN/SUB). It shall
 *              be invoked after dual bank initialization.
 *
 * Inputs: None
 *
 * Outputs: None 
 *
 * Returns: The dual bank status.
 *   0                  Main bank is in use.
 *   1                  Sub bank is in use.
 -----------------------------------------------------------------------*/
UINT8 x_lst_mngr_get_dual_bank_status(VOID)
{
    return lstm_get_svl_tsl_dual_bank();
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_reset_dual_bank_status
 *
 * Description: This API resets the dual bank status, to let lst_mngr
 *              switch dual bank status for the next dual bank operation. It
 *              shall be invoked after dual bank initialization.
 *
 * Inputs: None
 *
 * Outputs: None 
 *
 * Returns: None
 -----------------------------------------------------------------------*/
VOID x_lst_mngr_reset_dual_bank_status(VOID)
{
    lstm_reset_svl_tsl_dual_bank();
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_reg_default_db
 *
 * Description: This API initializes default database setting for the lists
 *              specified by given list ID and mask by registering the
 *              list ID and path for the corresponding default lists.
 *
 * Inputs: 
 *   h_dir        Handle to the parent directory of the file specified by
 *                the path 'ps_def_db_path'.  If default database path is an
 *                absolute paths, then this argument should be set to
 *                'FM_ROOT_HANDLE'.
 *   ps_def_db_path Specifies the file path to the file where the default
 *                data is to be loaded from file system for the list objects
 *                specified.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           Default database setting is registered.
 *   LSTMR_INV_ARG      'ps_def_db_path' is NULL, or invalid list id/mask.
 *   LSTMR_OUT_OF_MEM   Memory allocation failure.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_reg_default_db(UINT16              ui2_lst_id,
                                UINT32              ui4_lst_mask,
                                HANDLE_T            h_dir,
                                const CHAR*         ps_def_db_path,
                                UINT16              ui2_def_db_lst_id)
{
    LST_MNGR_LST_TYPE_T e_type;
    INT32               i4_i;
    INT32               i4_rc = LSTMR_OK;

    if ( ui2_lst_id != 0            &&
         ui4_lst_mask != 0 )
    {
        lst_mngr_sema_lock();
        for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) i4_i;
            if ( (ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0 )
            {
                continue;
            }

            i4_rc = lstm_reg_def_db_path_map(ui2_lst_id, e_type,
                                             h_dir, ps_def_db_path,
                                             ui2_def_db_lst_id);
            if ( i4_rc != LSTMR_OK )
            {
                DBG_ERROR(("{LSTM} failed registering default database for %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id));
                break;
            }
        }
        lst_mngr_sema_unlock();
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }

    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_load_default_db
 *
 * Description: This API loads the default data for the List objects
 *              specified by bitmask from the file system.
 *              This API shall be called after default database setting for
 *              the specified list id/mask has been registered.
 *
 * Inputs: 
 *   ui2_lst_id   Specifies the  ID of the list objects to be loaded default
 *                data for from the file system.  The limit for list ID is
 *                between 1 and 65535.
 *   ui4_lst_mask Bit mask indicating what type of lists are to be loaded.
 *   b_clean_list If TRUE, lists specified by ui4_lst_mask whose default
 *                databases are not found will be cleaned. Otherwise only
 *                lists found to have default databases will be replaced by
 *                the default content, leaving other lists untouched.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           List objects is loaded from the file system. 
 *   LSTMR_NOT_FOUND    The List object specified by the 'ui2_lst_id' does
 *                      not exist.
 *   LSTMR_INV_ARG      'ps_lst_path' is NULL.
 *   LSTMR_FAIL         List objects sync failure.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_load_default_db(UINT16            ui2_lst_id,
                                 UINT32            ui4_lst_mask,
                                 BOOL              b_clean_list)
{
    LST_MNGR_LST_TYPE_T e_type;
    BOOL                b_failed = FALSE;
    HANDLE_T            h_handle;
    HANDLE_T            h_dir;
    const CHAR*         ps_def_db_lst_path;
    UINT16              ui2_def_db_lst_id;
    INT32               i4_i;
    INT32               i4_rc = LSTMR_OK;
    
    if ( ui2_lst_id  != 0    &&
         ui4_lst_mask != 0 )
    {
        lst_mngr_sema_lock();
        
        for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) i4_i;
            if ((ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0)
            {
                continue;
            }

            if ( !lst_mngr_get_handle_by_type_id(e_type, ui2_lst_id, &h_handle) )
            {
                DBG_ERROR(("{LSTM} failed getting handle for %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id));
                i4_rc = LSTMR_NOT_FOUND;
                break;
            }

            i4_rc = lstm_get_def_db_path(ui2_lst_id, e_type,
                                         &h_dir, &ps_def_db_lst_path,
                                         &ui2_def_db_lst_id);
            if (i4_rc != LSTMR_OK && !b_clean_list)
            {
                DBG_ERROR(("{LSTM} failed getting default database path for %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id));
                break;
            }
        }

        if (i4_rc == LSTMR_OK)
        {
            for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
            {
                e_type = (LST_MNGR_LST_TYPE_T) i4_i;
                if ((ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0)
                {
                    continue;
                }

                lst_mngr_get_handle_by_type_id(e_type, ui2_lst_id, &h_handle);

                i4_rc = lstm_get_def_db_path(ui2_lst_id, e_type,
                                             &h_dir, &ps_def_db_lst_path,
                                             &ui2_def_db_lst_id);
                if (i4_rc == LSTMR_OK)
                {
                    switch(e_type)
                    {
                        case LST_TYPE_SVL:
                            i4_rc = x_svl_fs_sync(h_handle, h_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                            break;

                        case LST_TYPE_TSL:
                            i4_rc = x_tsl_fs_sync(h_handle, h_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                            break;

                        case LST_TYPE_NWL:
                            i4_rc = x_nwl_fs_sync(h_handle, h_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                            break;

                        case LST_TYPE_LOL:
                            i4_rc = x_lol_fs_sync(h_handle, h_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                            break;

                        case LST_TYPE_SATL:
#ifdef MW_DVBS_DEV_ENABLE
                            i4_rc = x_satl_fs_sync(h_handle, h_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
#endif
                            break;

                        default:
                            i4_rc = LSTMR_INV_ARG;
                            break;
                    }
                }

                if (i4_rc != LSTMR_OK && b_clean_list)
                {
                    DBG_INFO(("{LSTM} loading default database failed with %d for %s %d. clean list instead\r\n",
                              i4_rc, as_lst_name[e_type], ui2_lst_id));
                    switch(e_type)
                    {
                        case LST_TYPE_SVL:
                            i4_rc = x_svl_clean(h_handle);
                            break;

                        case LST_TYPE_TSL:
                            i4_rc = x_tsl_clean(h_handle);
                            break;

                        case LST_TYPE_NWL:
                            i4_rc = x_nwl_clean(h_handle);
                            break;

                        case LST_TYPE_LOL:
                            i4_rc = x_lol_clean(h_handle);
                            break;

                        case LST_TYPE_SATL:
#ifdef MW_DVBS_DEV_ENABLE
                            i4_rc = x_satl_clean(h_handle);
#endif
                            break;

                        default:
                            i4_rc = LSTMR_INV_ARG;
                            break;
                    }
                }

                if ( i4_rc != LSTMR_OK )
                {
                    DBG_ERROR(("{LSTM} failed loading default data for %s %d, ret=%d\r\n",
                               as_lst_name[e_type], ui2_lst_id, i4_rc));
                    b_failed = TRUE;
                }
            }
            if (b_failed)
            {
                i4_rc = LSTMR_FAIL;
            }
        }
        lst_mngr_sema_unlock();
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_dual_bank_fs_sync_with_fallback
 *
 * Description: This API loads the List objects specified by bitmask from
 *              the file system, path determined by dual bank status.  If it
 *              failed to load from dual bank path, try to load from the
 *              default database.  This API shall be invoked after dual bank
 *              and default database settings have been initialized.
 *
 * Inputs: 
 *   ui2_lst_id   Specifies the  ID of the list objects to be loaded from the
 *                file system.  The limit for list ID is between 1 and 65535.
 *   ui4_lst_mask Bit mask indicating what type of lists are to be loaded.
 *
 * Outputs: None 
 *
 * Returns: 
 *   LSTMR_OK           List objects is loaded from the file system. 
 *   LSTMR_NOT_FOUND    The List object specified by the 'ui2_lst_id' does
 *                      not exist.
 *   LSTMR_INV_ARG      Main/sub bank path is NULL.
 *   LSTMR_FAIL         List objects sync failure.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_dual_bank_fs_sync_with_fallback(UINT16            ui2_lst_id,
                                                 UINT32            ui4_lst_mask)
{
    LST_MNGR_LST_TYPE_T e_type;
    BOOL                b_failed = FALSE;
    HANDLE_T            h_handle;
    HANDLE_T            h_dir;
    const CHAR*         ps_lst_path;
    HANDLE_T            h_def_db_dir;
    const CHAR*         ps_def_db_lst_path;
    UINT16              ui2_def_db_lst_id;
    INT32               i4_i;
    INT32               i4_rc = LSTMR_OK;
    
    h_dir = lstm_get_dual_bank_dir();
    if (DUAL_BANK_SVL_TSL_SUB == lstm_get_svl_tsl_dual_bank())
    {
        ps_lst_path = lstm_get_dual_bank_main_path();
    }
    else
    {
        ps_lst_path = lstm_get_dual_bank_sub_path();
        if (ps_lst_path == NULL) /* dual bank not enabled */
        {
            ps_lst_path = lstm_get_dual_bank_main_path();
        }
    }
    
    if ( ps_lst_path != NULL &&
         ui2_lst_id  != 0    &&
         ui4_lst_mask != 0
        )
    {
        lst_mngr_sema_lock();
        
        for (i4_i = 0; i4_i < LST_MNGR_NUM_LIST_TYPES; i4_i++)
        {
            e_type = (LST_MNGR_LST_TYPE_T) i4_i;
            if ((ui4_lst_mask & MAKE_BIT_MASK_32 (e_type)) == 0)
            {
                continue;
            }

            if ( !lst_mngr_get_handle_by_type_id(e_type, ui2_lst_id, &h_handle) )
            {
                DBG_ERROR(("{LSTM} failed getting handle for %s %d\r\n",
                           as_lst_name[e_type], ui2_lst_id));
                i4_rc = LSTMR_NOT_FOUND;
                break;
            }

            lstm_get_def_db_path(ui2_lst_id, e_type,
                                 &h_def_db_dir, &ps_def_db_lst_path,
                                 &ui2_def_db_lst_id);

            switch(e_type)
            {
                case LST_TYPE_SVL:
                    i4_rc = x_svl_fs_sync(h_handle, h_dir, ps_lst_path, ui2_lst_id);
                    if (i4_rc != SVLR_OK)
                    {
                        i4_rc = x_svl_fs_sync(h_handle, h_def_db_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                    }
                    break;

                case LST_TYPE_TSL:
                    i4_rc = x_tsl_fs_sync(h_handle, h_dir, ps_lst_path, ui2_lst_id);
                    if (i4_rc != TSLR_OK)
                    {
                        i4_rc = x_tsl_fs_sync(h_handle, h_def_db_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                    }
                    break;

                case LST_TYPE_NWL:
                    i4_rc = x_nwl_fs_sync(h_handle, h_dir, ps_lst_path, ui2_lst_id);
                    if (i4_rc != NWLR_OK)
                    {
                        i4_rc = x_nwl_fs_sync(h_handle, h_def_db_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                    }
                    break;

                case LST_TYPE_LOL:
                    i4_rc = x_lol_fs_sync(h_handle, h_dir, ps_lst_path, ui2_lst_id);
                    if (i4_rc != LOLR_OK)
                    {
                        i4_rc = x_lol_fs_sync(h_handle, h_def_db_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                    }
                    break;

                case LST_TYPE_SATL:
#ifdef MW_DVBS_DEV_ENABLE
                    i4_rc = x_satl_fs_sync(h_handle, h_dir, ps_lst_path, ui2_lst_id);
                    if (i4_rc != SATLR_OK)
                    {
                        i4_rc = x_satl_fs_sync(h_handle, h_def_db_dir, ps_def_db_lst_path, ui2_def_db_lst_id);
                    }
#endif
                    break;

                default:
                    i4_rc = LSTMR_INV_ARG;
                    break;
            }
            if ( i4_rc != LSTMR_OK )
            {
                DBG_ERROR(("{LSTM} failed loading data for %s %d, ret=%d\r\n",
                           as_lst_name[e_type], ui2_lst_id, i4_rc));
                b_failed = TRUE;
            }
        }
        if (b_failed)
        {
            i4_rc = LSTMR_FAIL;
        }
        lst_mngr_sema_unlock();
    }
    else
    {
        i4_rc = LSTMR_INV_ARG;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_swap_svl_rec
 *
 * Description: This function swaps two service records in the SVL 
 *              database object.  All contents in the two records are
 *              exchanged except the record ID and channel ID.  The
 *              record ID specified must exist in the SVL database.
 *              The caller must ensure that a write lock has been
 *              acquired before calling this API.
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   ui2_svl_rec_id_A   One record ID of the service record to swap.
 *   ui2_svl_rec_id_B   Other record ID of the service record to swap.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_REC_NOT_FOUND The specified record is not found.
 *   SVLR_INV_ARG      The SVL record id is invalid.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_swap_svl_rec(HANDLE_T          h_svl,
                              UINT16            ui2_svl_rec_id_A,
                              UINT16            ui2_svl_rec_id_B)
{
    INT32           i4_rc;
    UINT32          ui4_ver_id = SVL_NULL_VER_ID;
    SVL_REC_T       t_svl_rec_A;
    SVL_REC_T       t_svl_rec_B;
    SVL_REC_T       t_tmp_svl_rec;
    UINT32          ui4_tmp_ch_id;
    
     x_memset(&t_svl_rec_A,0,sizeof(SVL_REC_T));
     x_memset(&t_svl_rec_B,0,sizeof(SVL_REC_T));

    do
    {
        i4_rc = x_svl_get_rec(h_svl, ui2_svl_rec_id_A, &t_svl_rec_A,
                              &ui4_ver_id);
        if (i4_rc != SVLR_OK)
        {
            break;
        }

        i4_rc = x_svl_get_rec(h_svl, ui2_svl_rec_id_B, &t_svl_rec_B,
                              &ui4_ver_id);
        if (i4_rc != SVLR_OK)
        {
            break;
        }

        x_memcpy(&t_tmp_svl_rec, &t_svl_rec_A, sizeof(SVL_REC_T));
        x_memcpy(&t_svl_rec_A, &t_svl_rec_B,   sizeof(SVL_REC_T));
        x_memcpy(&t_svl_rec_B, &t_tmp_svl_rec, sizeof(SVL_REC_T));

        ui4_tmp_ch_id = t_svl_rec_A.uheader.t_rec_hdr.ui4_channel_id;
        t_svl_rec_A.uheader.t_rec_hdr.ui4_channel_id =
            t_svl_rec_B.uheader.t_rec_hdr.ui4_channel_id;
        t_svl_rec_B.uheader.t_rec_hdr.ui4_channel_id = ui4_tmp_ch_id;

        t_svl_rec_A.ui2_svl_rec_id = ui2_svl_rec_id_A;
        t_svl_rec_B.ui2_svl_rec_id = ui2_svl_rec_id_B;

        i4_rc = x_svl_update_rec(h_svl, &t_svl_rec_A, TRUE);
        if (i4_rc != SVLR_OK)
        {
            break;
        }

        i4_rc = x_svl_update_rec(h_svl, &t_svl_rec_B, TRUE);
        if (i4_rc != SVLR_OK)
        {
            break;
        }
    } while (0);

    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_copy_svl_rec
 *
 * Description: This function copies specified service record to a target
 *              service record with different record ID and channel ID in
 *              the SVL database object.  The original record ID specified
 *              must exist in the SVL database.  If the a service record
 *              with the same record ID as the specified target exists,
 *              then that service record is overwritten; otherwise a new
 *              service record is added. 
 *              The caller must ensure that a write lock has been acquired
 *              before calling this API.
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   ui2_svl_rec_id     Record ID of the service record to copy from.
 *   ui2_new_rec_id     New record ID of the service record to swap.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_OUT_OF_MEM   Not enough memory to create a new SVL record.
 *   SVLR_REC_NOT_FOUND The specified record is not found.
 *   SVLR_INV_ARG      The SVL record id is invalid.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_copy_svl_rec(HANDLE_T          h_svl,
                              UINT16            ui2_svl_rec_id,
                              UINT16            ui2_new_rec_id,
                              UINT32            ui4_new_channel_id)
{
    INT32           i4_rc;
    UINT32          ui4_ver_id = SVL_NULL_VER_ID;
    SVL_REC_T       t_svl_rec;
    do
    {
        i4_rc = x_svl_get_rec(h_svl, ui2_svl_rec_id, &t_svl_rec, &ui4_ver_id);
        if (i4_rc != SVLR_OK)
        {
            break;
        }

        t_svl_rec.ui2_svl_rec_id = ui2_new_rec_id;
        t_svl_rec.uheader.t_rec_hdr.ui4_channel_id = ui4_new_channel_id;

        i4_rc = x_svl_update_rec(h_svl, &t_svl_rec, FALSE);
        if (i4_rc != SVLR_OK)
        {
            break;
        }
    } while (0);

    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_clean_svl
 *
 * Description: This function cleans the whole / partial SVL by deleting
 *              records with matching broadcast type.
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_clean   Clean parameter.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Operation successful. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_OUT_OF_MEM   Not enough memory to store rec ID list.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_clean_svl(HANDLE_T             h_svl,
                           LST_MNGR_CLEAN_T*    pt_clean)
{
    INT32               i4_ret;
    UINT32              ui4_ver_id = SVL_NULL_VER_ID;
    LST_MNGR_CLEAN_T    t_clean;
    UINT16              ui2_num_recs;
    UINT16*             pui2_svl_rec_ids = NULL;
    SVL_REC_T           t_svl_rec = {0};
    UINT16              ui2_idx;

    if (pt_clean == NULL)
    {
        pt_clean = &t_clean;
        pt_clean->e_brdcst_type = BRDCST_TYPE_UNKNOWN;
    }

    if (pt_clean->e_brdcst_type == BRDCST_TYPE_UNKNOWN)
        /* other conditions could be added */
    {
        return x_svl_clean(h_svl);
    }

    x_svl_lock(h_svl);
    do {
        i4_ret = x_svl_get_num_rec_by_brdcst_type (
                        h_svl,
                        pt_clean->e_brdcst_type,
                        0xFFFFFFFF,
                        &ui2_num_recs,
                        &ui4_ver_id
                        );
        if (i4_ret != SVLR_OK)
        {
            break;
        }

        if (ui2_num_recs == 0)
        {
            i4_ret = SVLR_OK;
            break;
        }
        
        pui2_svl_rec_ids = (UINT16*)x_mem_alloc(ui2_num_recs * sizeof(UINT16));
        if (pui2_svl_rec_ids == NULL)
        {
            i4_ret = SVLR_OUT_OF_MEM;
            break;
        }

        i4_ret = SVLR_OK;
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            i4_ret = x_svl_get_rec_by_brdcst_type(
                        h_svl,
                        pt_clean->e_brdcst_type,
                        0xFFFFFFFF,
                        ui2_idx,
                        &t_svl_rec,
                        &ui4_ver_id
                        );
            if (i4_ret != SVLR_OK)
            {
                break;
            }            

            if (TRUE)
                /* other conditions could be added */
            {
                pui2_svl_rec_ids[ui2_idx] = t_svl_rec.ui2_svl_rec_id;
            }
            else
            {
                pui2_svl_rec_ids[ui2_idx] = SVL_NULL_REC_ID;
            }
        }

        if (i4_ret != SVLR_OK)
        {
            break;
        } 
 
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            if (pui2_svl_rec_ids[ui2_idx] != SVL_NULL_REC_ID)
            {
                i4_ret = x_svl_del_rec(h_svl, pui2_svl_rec_ids[ui2_idx]);
                if (i4_ret != SVLR_OK)
                {
                    break;
                }
            }
        }
    } while (0);
    x_svl_unlock(h_svl);
    
    if (pui2_svl_rec_ids != NULL)
    {
        x_mem_free(pui2_svl_rec_ids);
    }
    return i4_ret;
}

/*----------------------------------------------------------------------
 * Name: x_lst_mngr_clean_tsl
 *
 * Description: This function cleans the whole / partial TSL by deleting
 *              records with matching broadcast type.
 *
 * Inputs: 
 *   h_tsl      Handle to the TSL object.
 *   pt_clean   Clean parameter.
 *
 * Outputs: None
 *
 * Returns: 
 *   TSLR_OK           Operation successful. 
 *   TSLR_INV_HANDLE   Bad TSL handle.
 *   TSLR_OUT_OF_MEM   Not enough memory to store rec ID list.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_clean_tsl(HANDLE_T             h_tsl,
                           LST_MNGR_CLEAN_T*    pt_clean)
{
    INT32               i4_ret;
    UINT32              ui4_ver_id = TSL_NULL_VER_ID;
    LST_MNGR_CLEAN_T    t_clean;
    UINT16              ui2_num_recs;
    UINT16*             pui2_tsl_rec_ids = NULL;
    TSL_REC_T           t_tsl_rec = {0};
    UINT16              ui2_idx;

    if (pt_clean == NULL)
    {
        pt_clean = &t_clean;
        pt_clean->e_brdcst_type = BRDCST_TYPE_UNKNOWN;
    }

    if (pt_clean->e_brdcst_type == BRDCST_TYPE_UNKNOWN)
        /* other conditions could be added */
    {
        return x_tsl_clean(h_tsl);
    }

    x_tsl_lock(h_tsl);
    do {
        i4_ret = x_tsl_get_num_recs(
                        h_tsl,
                        &ui2_num_recs,
                        &ui4_ver_id
                        );
        if (i4_ret != TSLR_OK)
        {
            break;
        }

        if (ui2_num_recs == 0)
        {
            i4_ret = TSLR_OK;
            break;
        }
        
        pui2_tsl_rec_ids = (UINT16*)x_mem_alloc(ui2_num_recs * sizeof(UINT16));
        if (pui2_tsl_rec_ids == NULL)
        {
            i4_ret = TSLR_OUT_OF_MEM;
            break;
        }

        i4_ret = TSLR_OK;
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            i4_ret = x_tsl_get_rec_by_rec_idx(
                        h_tsl,
                        ui2_idx,
                        &t_tsl_rec,
                        &ui4_ver_id
                        );
            if (i4_ret != TSLR_OK)
            {
                break;
            }            

            if (pt_clean->e_brdcst_type == BRDCST_TYPE_UNKNOWN ||
                t_tsl_rec.uheader.t_desc.e_bcst_type == pt_clean->e_brdcst_type)
                /* other conditions could be added */
            {
                pui2_tsl_rec_ids[ui2_idx] = t_tsl_rec.ui2_tsl_rec_id;
            }
            else
            {
                pui2_tsl_rec_ids[ui2_idx] = TSL_NULL_REC_ID;
            }
        }

        if (i4_ret != TSLR_OK)
        {
            break;
        } 
 
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            if (pui2_tsl_rec_ids[ui2_idx] != TSL_NULL_REC_ID)
            {
                i4_ret = x_tsl_del_rec(h_tsl, pui2_tsl_rec_ids[ui2_idx]);
                if (i4_ret != TSLR_OK)
                {
                    break;
                }
            }
        }
    } while (0);
    x_tsl_unlock(h_tsl);
    
    if (pui2_tsl_rec_ids != NULL)
    {
        x_mem_free(pui2_tsl_rec_ids);
    }
    return i4_ret;
}

#ifdef MW_DVBS_DEV_ENABLE
static INT32 _lst_mngr_clean_svl_by_satl(
              UINT16  ui2_satl_id,
              UINT16  ui2_satl_rec_id)
{
    INT32               i4_ret = LSTMR_OK;
    UINT32              ui4_ver_id = SVL_NULL_VER_ID;
    UINT16              ui2_num_recs = 0;
    UINT16*             pui2_svl_rec_ids = NULL;
    SVL_REC_T           t_svl_rec = {0};
    UINT16              ui2_idx = 0;
    HANDLE_T            h_svl = NULL_HANDLE;
    BOOL                b_new_handle = FALSE;

    
    if (!lst_mngr_get_handle_by_type_id(LST_TYPE_SVL, ui2_satl_id, &h_svl) 
        || !x_handle_valid(h_svl))
    {
        i4_ret = x_svl_open(ui2_satl_id, NULL, NULL, &h_svl);
        if (SVLR_OK != i4_ret)
        {
            return i4_ret;
        }
        b_new_handle = TRUE;
    }

    x_svl_lock(h_svl);
    do {
        i4_ret = x_svl_get_num_rec_by_sat (
                        h_svl,
                        ui2_satl_id,
                        ui2_satl_rec_id,
                        0xFFFFFFFF,
                        &ui2_num_recs,
                        &ui4_ver_id);
        if (i4_ret != SVLR_OK)
        {
            break;
        }

        if (ui2_num_recs == 0)
        {
            break;
        }
        
        pui2_svl_rec_ids = (UINT16*)x_mem_alloc(ui2_num_recs * sizeof(UINT16));
        if (pui2_svl_rec_ids == NULL)
        {
            i4_ret = LSTMR_OUT_OF_MEM;
            break;
        }

        i4_ret = SVLR_OK;
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            i4_ret = x_svl_get_rec_by_sat(
                        h_svl,
                        ui2_satl_id,
                        ui2_satl_rec_id,
                        0xFFFFFFFF,
                        ui2_idx,
                        &t_svl_rec,
                        &ui4_ver_id);
            if (i4_ret != SVLR_OK)
            {
                break;
            }            

            pui2_svl_rec_ids[ui2_idx] = t_svl_rec.ui2_svl_rec_id;
        }

        if (i4_ret != SVLR_OK)
        {
            break;
        } 
 
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            if (pui2_svl_rec_ids[ui2_idx] != SVL_NULL_REC_ID)
            {
                i4_ret = x_svl_del_rec(h_svl, pui2_svl_rec_ids[ui2_idx]);
                if (i4_ret != SVLR_OK)
                {
                    break;
                }
            }
        }
    } while (0);
    x_svl_unlock(h_svl);
    
    if (pui2_svl_rec_ids != NULL)
    {
        x_mem_free(pui2_svl_rec_ids);
    }
    if (b_new_handle)
    {
        x_svl_close(h_svl);
    }
    return i4_ret;
}

static INT32 _lst_mngr_clean_nwl_by_satl(
              UINT16  ui2_satl_id,
              UINT16  ui2_satl_rec_id)
{
    INT32               i4_ret = LSTMR_OK;
    UINT32              ui4_ver_id = NWL_NULL_VER_ID;
    UINT16              ui2_num_recs = 0;
    UINT16*             pui2_nwl_rec_ids = NULL;
    NWL_REC_T           t_nwl_rec = {0};
    UINT16              ui2_idx = 0;
    HANDLE_T            h_nwl = NULL_HANDLE;
    BOOL                b_new_handle = FALSE;
    
    if (!lst_mngr_get_handle_by_type_id(LST_TYPE_NWL, ui2_satl_id, &h_nwl) 
        || !x_handle_valid(h_nwl))
    {
        i4_ret = x_nwl_open(ui2_satl_id, NULL, NULL, &h_nwl);
        if (NWLR_OK != i4_ret)
        {
            return i4_ret;
        }
        b_new_handle = TRUE;
    }

    x_nwl_lock(h_nwl);
    do {
        i4_ret = x_nwl_get_num_rec_by_sat (
                        h_nwl,
                        ui2_satl_id,
                        ui2_satl_rec_id,
                        /* 0xFFFFFFFF,  No NWL mask parameter for nwl api*/
                        &ui2_num_recs,
                        &ui4_ver_id);
        if (i4_ret != NWLR_OK)
        {
            break;
        }

        if (ui2_num_recs == 0)
        {
            break;
        }
        
        pui2_nwl_rec_ids = (UINT16*)x_mem_alloc(ui2_num_recs * sizeof(UINT16));
        if (pui2_nwl_rec_ids == NULL)
        {
            i4_ret = LSTMR_OUT_OF_MEM;
            break;
        }

        i4_ret = NWLR_OK;
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            i4_ret = x_nwl_get_rec_by_sat(
                        h_nwl,
                        ui2_satl_id,
                        ui2_satl_rec_id,
                        /* 0xFFFFFFFF,  No NWL mask parameter for nwl api*/
                        ui2_idx,
                        &t_nwl_rec,
                        &ui4_ver_id);
            if (i4_ret != NWLR_OK)
            {
                break;
            }            

            pui2_nwl_rec_ids[ui2_idx] = t_nwl_rec.ui2_nwl_rec_id;
        }

        if (i4_ret != NWLR_OK)
        {
            break;
        } 
 
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            if (pui2_nwl_rec_ids[ui2_idx] != NWL_NULL_REC_ID)
            {
                i4_ret = x_nwl_del_rec(h_nwl, pui2_nwl_rec_ids[ui2_idx]);
                if (i4_ret != NWLR_OK)
                {
                    break;
                }
            }
        }
    } while (0);
    x_nwl_unlock(h_nwl);
    
    if (pui2_nwl_rec_ids != NULL)
    {
        x_mem_free(pui2_nwl_rec_ids);
    }
    if (b_new_handle)
    {
        x_nwl_close(h_nwl);
    }
    return i4_ret;
}

static INT32 _lst_mngr_clean_tsl_by_satl(
              UINT16  ui2_satl_id,
              UINT16  ui2_satl_rec_id)
{
    INT32               i4_ret = LSTMR_OK;
    UINT32              ui4_ver_id = TSL_NULL_VER_ID;
    UINT16              ui2_num_recs = 0;
    UINT16*             pui2_tsl_rec_ids = NULL;
    TSL_REC_T           t_tsl_rec = {0};
    UINT16              ui2_idx = 0;
    HANDLE_T            h_tsl = NULL_HANDLE;
    BOOL                b_new_handle = FALSE;

    if (!lst_mngr_get_handle_by_type_id(LST_TYPE_TSL, ui2_satl_id, &h_tsl) 
        || !x_handle_valid(h_tsl))
    {
        i4_ret = x_tsl_open(ui2_satl_id, NULL, NULL, &h_tsl);
        if (TSLR_OK != i4_ret)
        {
            return i4_ret;
        }
        b_new_handle = TRUE;
    }

    x_tsl_lock(h_tsl);
    do {
        i4_ret = x_tsl_get_num_rec_by_sat (
                        h_tsl,
                        0xFFFFFFFF,
                        ui2_satl_id,
                        ui2_satl_rec_id,
                        &ui2_num_recs,
                        &ui4_ver_id);
        if (i4_ret != TSLR_OK)
        {
            break;
        }

        if (ui2_num_recs == 0)
        {
            break;
        }
        
        pui2_tsl_rec_ids = (UINT16*)x_mem_alloc(ui2_num_recs * sizeof(UINT16));
        if (pui2_tsl_rec_ids == NULL)
        {
            i4_ret = LSTMR_OUT_OF_MEM;
            break;
        }

        i4_ret = TSLR_OK;
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            i4_ret = x_tsl_get_rec_by_sat(
                        h_tsl,
                        0xFFFFFFFF,
                        ui2_satl_id,
                        ui2_satl_rec_id,
                        ui2_idx,
                        &t_tsl_rec,
                        &ui4_ver_id);
            if (i4_ret != TSLR_OK)
            {
                break;
            }            

            pui2_tsl_rec_ids[ui2_idx] = t_tsl_rec.ui2_tsl_rec_id;
        }

        if (i4_ret != TSLR_OK)
        {
            break;
        } 
 
        for (ui2_idx = 0; ui2_idx < ui2_num_recs; ui2_idx++)
        {
            if (pui2_tsl_rec_ids[ui2_idx] != TSL_NULL_REC_ID)
            {
                i4_ret = x_tsl_del_rec(h_tsl, pui2_tsl_rec_ids[ui2_idx]);
                if (i4_ret != TSLR_OK)
                {
                    break;
                }
            }
        }
    } while (0);
    x_tsl_unlock(h_tsl);
    
    if (pui2_tsl_rec_ids != NULL)
    {
        x_mem_free(pui2_tsl_rec_ids);
    }
    if (b_new_handle)
    {
        x_tsl_close(h_tsl);
    }
    return i4_ret;
}
#endif /* MW_DVBS_DEV_ENABLE */


/*----------------------------------------------------------------------
 * Name:  x_lst_mngr_clean_satl_ch
 *
 * Description: This function clean satellite's children.
 *
 * Inputs: 
 *       ui2_satl_id        satl list id
 *       ui2_satl_rec_id    satl record id
 *       b_clean_tsl        clean related tsl records or not
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           Operation successful. 
 *   LSTMR_INV_ARG      Invalid input arguments.
 -----------------------------------------------------------------------*/
INT32 x_lst_mngr_clean_satl_ch(
              UINT16  ui2_satl_id,
              UINT16  ui2_satl_rec_id,
              BOOL    b_clean_tsl)
{
    INT32               i4_ret = LSTMR_OK;

#ifdef MW_DVBS_DEV_ENABLE

    /* Check the input parameters. */
    if (ui2_satl_id == SATL_NULL_ID && ui2_satl_rec_id == SATL_NULL_REC_ID)
    {
        return LSTMR_INV_ARG;
    }

    /* Remove the SVL records those pertain to given satl record. */
    i4_ret = _lst_mngr_clean_svl_by_satl(ui2_satl_id, ui2_satl_rec_id);
    if (i4_ret != SVLR_OK)
    {
        DBG_ERROR(("Clean svl by stal failed!"
                     " i4_ret=%d, satl_id=%d, satl_rec_id=%d\r\n",
                     i4_ret, ui2_satl_id, ui2_satl_rec_id));
        return i4_ret;
    }

    /* Remove the NWL records those pertain to given satl record. */
    i4_ret = _lst_mngr_clean_nwl_by_satl(ui2_satl_id, ui2_satl_rec_id);
    if (i4_ret != NWLR_OK)
    {
        DBG_ERROR(("Clean nwl by stal failed!"
                     " i4_ret=%d, satl_id=%d, satl_rec_id=%d\r\n",
                     i4_ret, ui2_satl_id, ui2_satl_rec_id));
        return i4_ret;
    }

    /* Remove the related TSL records as per b_clean_tsl */
    if (b_clean_tsl)
    {
        i4_ret = _lst_mngr_clean_tsl_by_satl(ui2_satl_id, ui2_satl_rec_id);
        if (i4_ret != NWLR_OK)
        {
            DBG_ERROR(("Clean tsl by stal failed!"
                         " i4_ret=%d, satl_id=%d, satl_rec_id=%d\r\n",
                         i4_ret, ui2_satl_id, ui2_satl_rec_id));
            return i4_ret;
        }
    }

#endif /* MW_DVBS_DEV_ENABLE */

    return i4_ret;
}
