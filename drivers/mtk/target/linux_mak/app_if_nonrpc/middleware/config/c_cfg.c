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
 * $RCSfile: c_cfg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains exported Configuration Manager (CFG) functions.
 *-------------------------------------------------------------------*/

#include "handle/handle.h"
#include "aee/aee.h"
#include "config/x_cfg.h"

/*---------------------------------------------------------------------
 * Name: c_cfg_create
 *
 * Description: This API creates configuration database objects
 *
 * Inputs:  -
 *    pt_descr       - structure describing configuration settings
 *    ui2_num        - number of entries in pt_descr
 *    ps_config_name - Name to associate with this configuration
 *
 * Outputs: -
 *    ph_config    - Pointer to variable for configuration handle
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV_ARG
 *    CFGR_INV_NAME - ? User doesn't pass name, so shouldn't pass back 
 *    CFGR_OUT_OF_MEM
 *    CFGR_OUT_OF_HANDLE
 *    CFGR_CONFIG_EXISTS
 *    CFGR_LIMIT_EXCEEDED
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_create(
    CFG_DESCR_T* pt_descr,
    UINT16       ui2_num,
    const CHAR*  ps_config_name,
    HANDLE_T*    ph_config
)
{
    INT32    i4_result;
    HANDLE_T h_aux;

    if (ph_config == NULL)
    {
        i4_result = x_cfg_create(pt_descr, ui2_num, ps_config_name,
                                 ph_config);
    }
    else
    {
        i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);

        switch(i4_result)
        {
        case AEER_OK:
            i4_result = x_cfg_create(pt_descr, ui2_num, ps_config_name,
                                     ph_config);
            if (i4_result == CFGR_OK)
            {
                handle_link_to_aux(h_aux, *ph_config);
            }
            else
            {
                aee_release_handle_resource(1);
            }
            break;
        case AEER_OUT_OF_RESOURCES:
            i4_result = CFGR_AEE_OUT_OF_RESOURCES;
            break;
        default:
            i4_result = CFGR_AEE_NO_RIGHTS;
            break;
        }
    }
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: c_cfg_open
 *
 * Description: This API opens a configuration database
 *
 * Inputs:  -
 *    ps_config_name - Name to associate with this configuration
 *
 * Outputs: -
 *    ph_config    - Pointer to variable for configuration handle
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV_ARG
 *    CFGR_INV_NAME - ? User doesn't pass name, so shouldn't pass back 
 *    CFGR_OUT_OF_MEM
 *    CFGR_OUT_OF_HANDLE
 *    CFGR_CONFIG_EXISTS
 *    CFGR_LIMIT_EXCEEDED
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_open(
    const CHAR*  ps_config_name,
    HANDLE_T*    ph_config
)
{
    INT32    i4_result;
    HANDLE_T h_aux;

    i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);

    switch(i4_result)
    {
    case AEER_OK:
        i4_result = x_cfg_open(ps_config_name, ph_config);
        if (i4_result == CFGR_OK)
        {
            handle_link_to_aux(h_aux, *ph_config);
        }
        else
        {
            aee_release_handle_resource(1);
        }
        break;
    case AEER_OUT_OF_RESOURCES:
        i4_result = CFGR_AEE_OUT_OF_RESOURCES;
        break;
    default:
        i4_result = CFGR_AEE_NO_RIGHTS;
        break;
    }
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: c_cfg_get
 *
 * Description: This API returns the specified configuration setting
 *
 * Inputs:  -
 *    h_config    - handle to configuration
 *    ui2_id      - configuration setting to get
 *
 * Outputs: -
 *    pv_variable - Pointer to variable to hold setting
 *                  Allowed to be NULL if caller just wants length.
 *    pz_length   - Pointer to variable holding size of setting in bytes
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV_ARG
 *    CFGR_NO_WRITE_LOCK
 *    CFGR_REC_NOT_FOUND
 *    CFGR_CANT_UNLOCK
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_get(
    HANDLE_T h_config,
    UINT16   ui2_id,
    VOID*    pv_variable,
    SIZE_T*  pz_length
)
{
    return x_cfg_get(h_config, ui2_id, pv_variable, pz_length);
}

/*---------------------------------------------------------------------
 * Name: c_cfg_set
 *
 * Description: This API sets the specified configuration setting
 *
 * Inputs:  -
 *    h_config    - handle to configuration
 *    ui2_id      - configuration setting to set
 *    pv_variable - Pointer to variable holding new setting value(s)
 *    z_length    - size of setting in bytes
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV_ARG
 *    CFGR_NO_WRITE_LOCK
 *    CFGR_REC_NOT_FOUND
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_set(
    HANDLE_T h_config,
    UINT16   ui2_id,
    VOID*    pv_variable,
    SIZE_T   z_length
)
{
    return x_cfg_set(h_config, ui2_id, pv_variable, z_length);
}

/*---------------------------------------------------------------------
 * Name: c_cfg_notify_reg
 *
 * Description: This API returns a handle to register notification
 *              of the changing of the specified configuration
 *              group.
 *
 * Inputs:  -
 *    h_config       - handle to configuration
 *    ui2_group      - configuration group to watch
 *    pf_notify_func - Pointer to notification function
 *    pv_tag         - Parameter to be passed to notification function
 *
 * Outputs: -
 *    ph_notify      - Handle to notification object
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV_ARG
 *    CFGR_OUT_OF_MEM
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_notify_reg(
    HANDLE_T        h_config,
    UINT16          ui2_group,
    VOID*           pv_tag,
    pf_cfg_nfy_func pf_notify_func,
    HANDLE_T*       ph_notify
)
{
    INT32    i4_result;
    HANDLE_T h_aux;

    i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);

    switch(i4_result)
    {
    case AEER_OK:
        i4_result = x_cfg_notify_reg(h_config, ui2_group,
                           pv_tag, pf_notify_func, ph_notify);
        if (i4_result == CFGR_OK)
        {
            handle_link_to_aux(h_aux, *ph_notify);
        }
        else
        {
            aee_release_handle_resource(1);
        }
        break;
    case AEER_OUT_OF_RESOURCES:
        i4_result = CFGR_AEE_OUT_OF_RESOURCES;
        break;
    default:
        i4_result = CFGR_AEE_NO_RIGHTS;
        break;
    }

    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: c_cfg_notify_reg_by_name
 *
 * Description: This API returns a handle to register notification
 *              of the changing of the specified configuration
 *              group.
 *
 * Inputs:  -
 *    ps_config_name - Name of configuration
 *    ui2_group      - configuration group to watch
 *    pf_notify_func - Pointer to notification function
 *    pv_tag         - Parameter to be passed to notification function
 *
 * Outputs: -
 *    ph_notify      - Handle to notification object
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV_ARG
 *    CFGR_OUT_OF_MEM
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_notify_reg_by_name(
    const CHAR*     ps_config_name,
    UINT16          ui2_group,
    VOID*           pv_tag,
    pf_cfg_nfy_func pf_notify_func,
    HANDLE_T*       ph_notify
)
{
    INT32    i4_result;
    HANDLE_T h_aux;

    i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);

    switch(i4_result)
    {
    case AEER_OK:
        i4_result = x_cfg_notify_reg_by_name(ps_config_name, ui2_group,
                           pv_tag, pf_notify_func, ph_notify);
        if (i4_result == CFGR_OK)
        {
            handle_link_to_aux(h_aux, *ph_notify);
        }
        else
        {
            aee_release_handle_resource(1);
        }
        break;
    case AEER_OUT_OF_RESOURCES:
        i4_result = CFGR_AEE_OUT_OF_RESOURCES;
        break;
    default:
        i4_result = CFGR_AEE_NO_RIGHTS;
        break;
    }

    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: c_cfg_lock
 *
 * Description: This API locks CFG access
 *
 * Inputs:
 *    h_config - handle to configuration
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_NO_WRITE_LOCK
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_lock(
    HANDLE_T h_config
)
{
    return x_cfg_lock(h_config);
}

/*---------------------------------------------------------------------
 * Name: c_cfg_unlock
 *
 * Description: This API unlocks CFG access
 *
 * Inputs:
 *    h_config - handle to configuration
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_CANT_UNLOCK
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_unlock(
    HANDLE_T h_config
)
{
    return x_cfg_unlock(h_config);
}

/*---------------------------------------------------------------------
 * Name: c_cfg_fs_load
 *
 * Description: This API loads all configuration groups from persistent
 *              storage.
 *
 * Inputs:  -
 *    h_dir          - directory to load settings from
 *    ps_pathname    - name of file to load settings from
 *    ps_config_name - Name of configuration to load
 *
 * Outputs
 *    ph_config    - Pointer to handle variable 
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_NO_SUCH_FILE
 *    CFGR_CANT_OPEN_FILE
 *    CFGR_OUT_OF_MEM
 *    CFGR_INV_ARG
 *    CFGR_INV_HANDLE
 *    CFGR_NOT_DIR
 *    CFGR_CORE
 *    CFGR_NAME_TOO_LONG
 *    CFGR_DEVICE_ERROR
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_fs_load(
    HANDLE_T    h_dir,
    CHAR*       ps_pathname,
    const CHAR* ps_config_name,
    HANDLE_T*   ph_config
)
{
    INT32    i4_result;
    HANDLE_T h_aux;

    if (ph_config == NULL)
    {
        i4_result = x_cfg_fs_load(h_dir, ps_pathname,
                           ps_config_name, ph_config);
    }
    else
    {
        i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);
    
        switch(i4_result)
        {
        case AEER_OK:
            i4_result = x_cfg_fs_load(h_dir, ps_pathname,
                               ps_config_name, ph_config);
            if (i4_result == CFGR_OK)
            {
                handle_link_to_aux(h_aux, *ph_config);
            }
            else
            {
                aee_release_handle_resource(1);
            }
            break;
        case AEER_OUT_OF_RESOURCES:
            i4_result = CFGR_AEE_OUT_OF_RESOURCES;
            break;
        default:
            i4_result = CFGR_AEE_NO_RIGHTS;
            break;
        }
    }
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: c_cfg_fs_store
 *
 * Description: This API stores all configuration groups to persistent
 *              storage.
 *
 * Inputs:  -
 *    h_dir        - directory to store settings to
 *    ps_pathname  - name of file to store settings to
 *    h_config     - handle to configuration to store
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_CANT_CREATE_FILE
 *    CFGR_NO_SUCH_FILE
 *    CFGR_INV_ARG
 *    CFGR_INV_HANDLE
 *    CFGR_NOT_DIR
 *    CFGR_CORE
 *    CFGR_NAME_TOO_LONG
 *    CFGR_EXIST
 *    CFGR_DEVICE_ERROR
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_fs_store(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    HANDLE_T h_config
)
{
    return x_cfg_fs_store(h_dir, ps_pathname, h_config);
}

/*---------------------------------------------------------------------
 * Name: c_cfg_fs_qry
 *
 * Description: This API queries if a configuration set exists on
 *              the specified persistent storage.
 *
 * Inputs:
 *    h_dir          - directory to query
 *    ps_pathname    - pathname of device to query
 *    ps_config_name - Name of configuration to query for
 *
 * Output:
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV
 *
 * Exceptions:
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_fs_qry(
    HANDLE_T    h_dir,
    CHAR*       ps_pathname,
    const CHAR* ps_config_name
)
{
    return x_cfg_fs_qry(h_dir, ps_pathname, ps_config_name);
}

/*---------------------------------------------------------------------
 * Name: c_cfg_fs_del
 *
 * Description: This API deletes a configuration set from
 *              the specified persistent storage.
 *
 * Inputs:  -
 *    h_dir          - directory to delete from
 *    ps_pathname    - pathname of device to delete from
 *    ps_config_name - Name of configuration to delete
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_INV
 *
 * Exceptions:
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_fs_del(
    HANDLE_T    h_dir,
    CHAR*       ps_pathname,
    const CHAR* ps_config_name
)
{
    return x_cfg_fs_del(h_dir, ps_pathname, ps_config_name);
}

/*---------------------------------------------------------------------
 * Name: c_cfg_fs_reg_fcts
 *
 * Description:
 *    This API allow application to register their own version of file
 *    IO API to load/save/query configuration set from the specified
 *    persistent storage.
 *
 * Inputs:  -
 *    ps_config_name - Name of configuration to which the application
 *                     specified IO API will be used to access persistent
 *                     storage.
 *
 *    pt_fct_tbl       Specified the IO function table.  The configuration
 *                     manager will make copy of this table.  If NULL,
 *                     then all previous registered functions are cleared. 
 * Returns: -
 *    CFGR_OK          success.
 *    CFGR_INV_ARG     pt_fct_tbl contain null function pointer(s).
 *
 * Exceptions:
 *
 --------------------------------------------------------------------*/
INT32 c_cfg_fs_reg_fcts
(
    HANDLE_T        h_config,
    CFG_FILE_API_T* pt_fct_tbl
)
{
    return x_cfg_fs_reg_fcts(h_config, pt_fct_tbl);
}



    
