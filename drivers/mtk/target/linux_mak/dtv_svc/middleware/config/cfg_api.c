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
 * $RCSfile: cfg_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains exported Configuration Manager (CFG) functions.
 *-------------------------------------------------------------------*/

#include "config/cfg.h"

/*---------------------------------------------------------------------
 * Name: x_cfg_create
 *
 * Description: This API creates configuration database objects
 *
 * Inputs:  -
 *    pt_descr       - pointer to configuration settings descriptor list
 *    ui2_num        - Number of descriptors in list
 *    ps_config_name - Name to associate with this configuration
 *
 * Outputs: -
 *    ph_config
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
INT32 x_cfg_create(
                  CFG_DESCR_T* pt_descr,
                  UINT16       ui2_num,
                  const CHAR*  ps_config_name,
                  HANDLE_T*    ph_config
                  )
{
    INT32           i4_result;
    UINT16          ui2_i;
    HANDLE_T        h_db;
    DB_FIELD_INFO_T t_fields[2];  /* one key, one data */
    UINT16          ui2_rec_id;
    UINT16          ui2_group;
    CFG_LIST_T*     pt_cfg_list;
    CFG_OBJ_T*      pt_cfg_obj;

    DBG_API(("{CFG} x_cfg_create: pt_descr 0x%x ui2_num %d ps_config_name %s\n", pt_descr, ui2_num, ps_config_name));

    if (pt_descr == NULL || ui2_num == 0)
    {
        return CFGR_INV_ARG;
    }

    if (((ph_config != NULL) && (ps_config_name == NULL)) ||
        ((ph_config == NULL) && (ps_config_name != NULL)))
    {
        return CFGR_INV_ARG;
    }

    if (x_strlen(ps_config_name) >= CFG_NAME_LEN)
    {
        return CFGR_INV_NAME;
    }

    /* if stuff is in memory already, return an error */
    if (cfg_find_config(ps_config_name, &pt_cfg_list) == CFGR_OK)
    {
        return CFGR_CONFIG_EXISTS;
    }

    /* sanity check on pt_descr */
    for (ui2_i = 0;ui2_i < ui2_num; ++ui2_i)
    {
        if ((pt_descr+ui2_i)->ui2_id == 0 ||
            (pt_descr+ui2_i)->ui2_num_elem == 0)
        {
            return CFGR_INV_ARG;
        }
        switch ((pt_descr+ui2_i)->e_type)
        {
        case CFG_8BIT_T:
        case CFG_16BIT_T:
        case CFG_32BIT_T:
        case CFG_64BIT_T:
            break;  /* ok */
        case CFG_NONE:
        default:
            return CFGR_INV_ARG;
        }
    }

    if (ph_config != NULL)
    {
        /* allocate a handle and CFG_OBJ_T for caller */
        i4_result = cfg_alloc_handle(ph_config, NULL, &pt_cfg_obj);
        if (i4_result != CFGR_OK)
        {
            return i4_result;
        }
    }

    /* create a CFG_LIST_T */
    i4_result = cfg_create_config(ps_config_name, &pt_cfg_list);
    if (i4_result)
    {
        if (ph_config != NULL)
        {
            x_handle_free(*ph_config);
        }
        return i4_result;
    }

    /* key field is always the same size */
    t_fields[0].ui4_field_type = (DB_FIELD_T) CFG_KEY_FIELD_TYPE;
    t_fields[0].ui2_field_element_count = 1;

    for (ui2_i = 0;ui2_i < ui2_num; ++ui2_i)
    {
        CHAR s_name[30];

        ui2_rec_id = (pt_descr+ui2_i)->ui2_id;

        /* data field varies with the configuration setting */
        t_fields[1].ui4_field_type = (DB_FIELD_T) DB_MAKE_FIELD_TYPE(db_to_cfg_field((pt_descr+ui2_i)->e_type, (pt_descr+ui2_i)->ui2_num_elem), (CFG_KEY_FIELD+1));
        t_fields[1].ui2_field_element_count = (pt_descr+ui2_i)->ui2_num_elem;

        CFG_MAKE_NAME(s_name, pt_cfg_list, ui2_rec_id);

        ui2_group = CFG_GET_GROUP(ui2_rec_id);
        i4_result = db_create(
                             1,
                             DB_MEM_STATIC,
                             s_name,
                             2,
                             t_fields,
                             1,
                             &(t_fields[0].ui4_field_type),
                             &ui2_group,
                             NULL,
                             &h_db
                             );
        if (i4_result != DBR_OK)
        {
            if (ph_config != NULL)
            {
                x_handle_free(*ph_config);
            }
            cfg_del_config(pt_cfg_list);
            return db_to_cfg_err(i4_result);
        }

        i4_result =  cfg_add_setting(pt_cfg_list, ui2_rec_id, h_db,
                                     t_fields[1].ui4_field_type,
                                     t_fields[1].ui2_field_element_count);
        if (i4_result != CFGR_OK)
        {
            if (ph_config != NULL)
            {
                x_handle_free(*ph_config);
            }
            cfg_del_config(pt_cfg_list);
            return i4_result;
        }

        /* send create notifications */
        i4_result =  cfg_dispatch_nfys(pt_cfg_list, CFG_CREATED, ui2_rec_id);
        if (i4_result != CFGR_OK)
        {
            /* should I really do this? */
            if (ph_config != NULL)
            {
                x_handle_free(*ph_config);
            }
            cfg_del_config(pt_cfg_list);
            x_mem_free(pt_cfg_list);
            return i4_result;
        }
    }

    if (ph_config != NULL)
    {
        pt_cfg_obj->pt_config = pt_cfg_list;
    }

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_open
 *
 * Description: This API creates configuration database objects
 *
 * Inputs:  -
 *    ps_config_name - Name of configuration
 *
 * Outputs: -
 *    ph_config      - Handle to opened configuration
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_REC_NOT_FOUND
 *    CFGR_OUT_OF_HANDLE
 *    CFGR_OUT_OF_MEM
 *
 --------------------------------------------------------------------*/
INT32 x_cfg_open(
                const CHAR* ps_config_name,
                HANDLE_T*   ph_config
                )
{
    INT32       i4_result;
    CFG_LIST_T* pt_list;
    CFG_OBJ_T*  pt_cfg_obj;

    DBG_API(("{CFG} x_cfg_open: ps_config_name %s ph_config 0x%x\n",
             ps_config_name, ph_config));

    if (ph_config == NULL || ps_config_name == NULL)
    {
        return CFGR_INV_ARG;
    }

    i4_result = cfg_find_config(ps_config_name, &pt_list);
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    /* allocate a handle and CFG_OBJ_T for caller */
    i4_result = cfg_alloc_handle(ph_config, NULL, &pt_cfg_obj);
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    pt_cfg_obj->pt_config = pt_list;

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_get
 *
 * Description: This API returns the specified configuration setting
 *
 * Inputs:  -
 *    h_config    - Handle to configuration
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
INT32 x_cfg_get(
               HANDLE_T h_config,
               UINT16   ui2_id,
               VOID*    pv_variable,
               SIZE_T*  pz_length
               )
{
    INT32           i4_result;
    UINT32          ui4_key_field_type;
    UINT32          ui4_data_field_type;
    UINT8           ui1_key = 0;
    UINT32          ui4_version;
    CFG_REC_LIST_T* pt_rec_info;
    VOID*           apv_keyfield_addr[1];
    CFG_LIST_T*     pt_list;
    CFG_OBJ_T*      pt_obj;
    HANDLE_TYPE_T   e_type;

    DBG_API(("{CFG} x_cfg_get: h_config %d ui2_id %d pv_variable 0x%x pz_length 0x%x\n", h_config, ui2_id, pv_variable, pz_length));

    if (pz_length == NULL)
    {
        return CFGR_INV_ARG;
    }

    i4_result = cfg_get_read_lock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    if (h_config != NULL_HANDLE)
    {
        i4_result = handle_get_type_obj(h_config, &e_type, (VOID**) &pt_obj);
        if (i4_result != HR_OK || (e_type != CFG_HANDLE) || (pt_obj == NULL))
        {
            cfg_unlock();
#ifdef DEBUG
            if(x_handle_valid(h_config))
            {
                CFG_ABORT(DBG_CAT_HANDLE, 1);
            }
#endif
            return CFGR_INV_HANDLE;
        }

        pt_list = pt_obj->pt_config;
    }
    else
    {
        pt_list = NULL;
    }

    i4_result = cfg_get_rec_obj(pt_list, ui2_id, &pt_rec_info);
    if (i4_result != CFGR_OK)
    {
        cfg_unlock();
        return i4_result;
    }

    ui4_key_field_type = (UINT32) CFG_KEY_FIELD_TYPE;
    ui4_data_field_type = pt_rec_info->ui4_field_type;
    i4_result = cfg_get_field_size(pt_rec_info->ui4_field_type,
                                   pz_length);
    if (i4_result != CFGR_OK)
    {
        cfg_unlock();
        return i4_result;
    }
    *pz_length *= pt_rec_info->ui2_num;

    /* if user just wanted size, return now */
    if (pv_variable == NULL)
    {
        cfg_unlock();
        return CFGR_OK;
    }
    apv_keyfield_addr[0] = &ui1_key;
    ui4_version = CDB_NULL_VER_ID;
    i4_result = db_read_rec(
                           pt_rec_info->h_db,
                           1,
                           &ui4_key_field_type,
                           (const VOID**) apv_keyfield_addr,
                           NULL,
                           1,
                           &ui4_data_field_type,
                           &pv_variable,
                           0,
                           &ui4_version
                           );
    if (i4_result != DBR_OK)
    {
        cfg_unlock();
        return db_to_cfg_err(i4_result);
    }

    return cfg_unlock();
}

/*---------------------------------------------------------------------
 * Name: x_cfg_set
 *
 * Description: This API sets the specified configuration setting
 *
 * Inputs:  -
 *    h_config    - Handle to configuration
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
INT32 x_cfg_set(
               HANDLE_T h_config,
               UINT16   ui2_id,
               VOID*    pv_variable,
               SIZE_T   z_length
               )
{
    INT32             i4_result;
    CFG_REC_LIST_T*   pt_reclist;
    UINT8             ui1_key = 0;
    VOID*             apv_keyfield_addr[1];
    VOID*             apv_field_addrs[2];
    UINT16            aui2_field_len[2];
    CFG_LIST_T*       pt_list;
    CFG_OBJ_T*        pt_obj;
    HANDLE_TYPE_T     e_type;
    SIZE_T            z_rec_len;

    DBG_API(("{CFG} x_cfg_set: h_config %d ui2_id %d pv_variable 0x%x z_length %d\n", h_config, ui2_id, pv_variable, z_length));

    if (pv_variable == NULL || z_length == 0)
    {
        return CFGR_INV_ARG;
    }

    i4_result = cfg_get_write_lock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    if (h_config != NULL_HANDLE)
    {
        i4_result = handle_get_type_obj(h_config, &e_type, (VOID**) &pt_obj);
        if (i4_result != HR_OK || (e_type != CFG_HANDLE) || (pt_obj == NULL))
        {
            cfg_unlock();
#ifdef DEBUG
            if(x_handle_valid(h_config))
            {
                CFG_ABORT(DBG_CAT_HANDLE, 1);
            }
#endif
            return CFGR_INV_HANDLE;
        }

        pt_list = pt_obj->pt_config;
    }
    else
    {
        pt_list = NULL;
    }

    i4_result = cfg_get_rec_obj(pt_list, ui2_id, &pt_reclist);
    if (i4_result != CFGR_OK)
    {
        cfg_unlock();
        return i4_result;
    }

    i4_result = cfg_get_field_size(pt_reclist->ui4_field_type, &z_rec_len);
    if (i4_result != CFGR_OK)
    {
        cfg_unlock();
        return i4_result;
    }

    if (z_length > (z_rec_len *pt_reclist->ui2_num))
    {
        cfg_unlock();
        return CFGR_INV_ARG;
    }

    i4_result = db_write_lock(pt_reclist->h_db);
    if (i4_result != DBR_OK)
    {
        cfg_unlock();
        return db_to_cfg_err(i4_result);
    }

    apv_keyfield_addr[0] = &ui1_key;
    apv_field_addrs[0] = &ui1_key;
    apv_field_addrs[1] = pv_variable;
    aui2_field_len[0]  = (UINT16) sizeof(ui1_key);
    aui2_field_len[1]  = (UINT16) z_length;
    /*
    i4_result = db_write_rec(
                            pt_reclist->h_db,
                            (VOID*) apv_keyfield_addr,
                            (VOID*) &apv_field_addrs,
                            DB_UPDATE_RECORD
                            );
    */
    i4_result = db_write_rec_v2(
                            pt_reclist->h_db,
                            (const VOID**) apv_keyfield_addr,
                            (const VOID**) &apv_field_addrs,
                            aui2_field_len,
                            DB_UPDATE_RECORD
                            );
    
    if (i4_result != DBR_OK)
    {
        cfg_unlock();
        db_write_unlock(pt_reclist->h_db);
        return db_to_cfg_err(i4_result);
    }

    /* unlock the structure */
    i4_result = db_write_unlock(pt_reclist->h_db);
    if (i4_result != DBR_OK)
    {
        /*** abort? ***/
        cfg_unlock();
        return db_to_cfg_err(i4_result);
    }

    i4_result = cfg_unlock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    /* send notification right away */
    return cfg_dispatch_nfys(pt_list, CFG_UPDATED, ui2_id);
}

/*---------------------------------------------------------------------
 * Name: x_cfg_notify_reg
 *
 * Description: This API returns a handle to register notification
 *              of the changing of the specified configuration
 *              group.
 *
 * Inputs:  -
 *    h_config       - Handle to configuration
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
INT32 x_cfg_notify_reg(
                      HANDLE_T        h_config,
                      UINT16          ui2_group,
                      VOID*           pv_tag,
                      pf_cfg_nfy_func pf_notify_func,
                      HANDLE_T*       ph_notify
                      )
{
    INT32             i4_result;
    CFG_NFY_OBJ_T*    pt_nfy_obj;
    CFG_OBJ_T*        pt_obj;
    CFG_LIST_T*       pt_list;
    HANDLE_TYPE_T     e_type;
    CFG_GROUP_LIST_T* pt_group;

    DBG_API(("{CFG} x_cfg_notify_reg: h_config %d ui2_group %d pv_tag 0x%x pf_notify_func 0x%x ph_notify 0x%x\n",
             h_config, ui2_group, pv_tag, pf_notify_func, ph_notify));

    /* note that a null pv_tag is ok */
    if (pf_notify_func == NULL || ph_notify == NULL)
    {
        return CFGR_INV_ARG;
    }

    i4_result = cfg_get_write_lock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    if (h_config != NULL_HANDLE)
    {
        i4_result = handle_get_type_obj(h_config, &e_type, (VOID**) &pt_obj);
        if (i4_result != HR_OK || (e_type != CFG_HANDLE) || (pt_obj == NULL))
        {
            cfg_unlock();
#ifdef DEBUG
            if(x_handle_valid(h_config))
            {
                CFG_ABORT(DBG_CAT_HANDLE, 1);
            }
#endif
            return CFGR_INV_HANDLE;
        }
        pt_list = pt_obj->pt_config;
    }
    else
    {
        pt_list = NULL;
    }

    /* check to make sure group is valid */
    if (cfg_get_group_obj(pt_list, ui2_group, &pt_group) != CFGR_OK)
    {
        cfg_unlock();
        return CFGR_INV_ARG;
    }

    /* allocate a notification object */
    i4_result = cfg_alloc_nfy_obj(pt_list, ph_notify, &pt_nfy_obj);
    if (i4_result != CFGR_OK)
    {
        cfg_unlock();
        return i4_result;
    }

    pt_nfy_obj->pf_nfy = pf_notify_func;
    pt_nfy_obj->pv_tag = pv_tag;
    pt_nfy_obj->ui2_group = ui2_group;

    i4_result = cfg_unlock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_notify_reg_by_name
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
INT32 x_cfg_notify_reg_by_name(
                      const CHAR*     ps_config_name,
                      UINT16          ui2_group,
                      VOID*           pv_tag,
                      pf_cfg_nfy_func pf_notify_func,
                      HANDLE_T*       ph_notify
                      )
{
    INT32             i4_result;
    CFG_NFY_OBJ_T*    pt_nfy_obj;
    CFG_LIST_T*       pt_list;
    CFG_GROUP_LIST_T* pt_group;

    DBG_API(("{CFG} x_cfg_notify_reg_by_name: ps_config_name %s ui2_group %d pv_tag 0x%x pf_notify_func 0x%x ph_notify 0x%x\n",
             ps_config_name, ui2_group, pv_tag, pf_notify_func, ph_notify));

    /* note that a null pv_tag is ok */
    if (pf_notify_func == NULL || ph_notify == NULL || ps_config_name == NULL)
    {
        return CFGR_INV_ARG;
    }

    i4_result = cfg_get_write_lock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    /* find config handle by name */
    i4_result = cfg_find_config(ps_config_name, &pt_list);
    if (i4_result != CFGR_OK)
    {
        cfg_unlock();
        return i4_result;
    }

    /* check to make sure group is valid */
    if (cfg_get_group_obj(pt_list, ui2_group, &pt_group) != CFGR_OK)
    {
        cfg_unlock();
        return CFGR_INV_ARG;
    }

    /* allocate a notification object */
    i4_result = cfg_alloc_nfy_obj(pt_list, ph_notify, &pt_nfy_obj);
    if (i4_result != CFGR_OK)
    {
        cfg_unlock();
        return i4_result;
    }

    pt_nfy_obj->pf_nfy = pf_notify_func;
    pt_nfy_obj->pv_tag = pv_tag;
    pt_nfy_obj->ui2_group = ui2_group;

    cfg_unlock();

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_lock
 *
 * Description: This API locks CFG access
 *
 * Inputs:
 *    h_config
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_NO_WRITE_LOCK
 *
 --------------------------------------------------------------------*/
INT32 x_cfg_lock(
                HANDLE_T h_config
                )
{
    DBG_API(("{CFG} x_cfg_lock h_config %d\n", h_config));

    return cfg_get_write_lock();
}

/*---------------------------------------------------------------------
 * Name: x_cfg_unlock
 *
 * Description: This API unlocks CFG access
 *
 * Inputs:
 *    h_config
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_CANT_UNLOCK
 *
 --------------------------------------------------------------------*/
INT32 x_cfg_unlock(
                  HANDLE_T h_config
                  )
{
    DBG_API(("{CFG} x_cfg_unlock h_config %d\n", h_config));

    return cfg_unlock();
}

/*---------------------------------------------------------------------
 * Name: x_cfg_fs_load
 *
 * Description: This API loads all configuration groups from persistent
 *              storage.
 *
 * Inputs:  -
 *    h_dir          - directory to load settings from
 *    ps_pathname    - name of file to load settings from
 *    ps_config_name - Name of configuration to load
 *
 * Outputs:
 *    ph_config
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
INT32 x_cfg_fs_load(
                   HANDLE_T    h_dir,
                   CHAR*       ps_pathname,
                   const CHAR* ps_config_name,
                   HANDLE_T*   ph_config
                   )
{
    INT32            i4_result;
    CFG_FILE_API_T   t_fct_tbl;
    
    DBG_API(("{CFG} x_cfg_fs_load: h_dir %d ps_pathname %s ps_config_name %s\n", h_dir, ps_pathname, ps_config_name));

    if (ps_pathname == NULL)
    {
        return CFGR_INV_ARG;
    }

    if ((ph_config == NULL && ps_config_name != NULL) ||
        (ph_config != NULL && ps_config_name == NULL))
    {
        return CFGR_INV_ARG;
    }

    if ( (cfg_get_custom_fct_tbl(ps_config_name,
                                 &t_fct_tbl) == CFGR_OK)          &&
         (t_fct_tbl.pf_load != NULL)
        )
    {
        DBG_API(("{CFG} load config data using registered function.\n"));
        i4_result = cfg_load_settings_by_custom_fct(h_dir,
                                                    ps_pathname,
                                                    ps_config_name,
                                                    &t_fct_tbl,
                                                    ph_config);
    }
    else
    {
        /* load each group from the file */
        i4_result = cfg_load_settings(h_dir, ps_pathname, ps_config_name, ph_config);
    }
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_fs_store
 *
 * Description: This API stores all configuration groups to persistent
 *              storage.
 *
 * Inputs:  -
 *    h_dir        - directory to store settings to
 *    ps_pathname  - name of file to store settings to
 *    h_config     -
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
INT32 x_cfg_fs_store(
    HANDLE_T       h_dir,
    CHAR*          ps_pathname,
    HANDLE_T       h_config
    )
{
    INT32          i4_result;
    CFG_OBJ_T*     pt_obj = NULL;
    HANDLE_TYPE_T  e_type;
    CFG_FILE_API_T t_fct_tbl;
    CHAR*          ps_config_name = NULL;
    CRIT_STATE_T   t_state;
    
    DBG_API(("{CFG} x_cfg_fs_store: h_dir %d ps_pathname %s h_config %d\n", h_dir, ps_pathname, h_config));

    if (ps_pathname == NULL)
    {
        return CFGR_INV_ARG;
    }
    
    /* Get the config object for the handle. */
    if (h_config != NULL_HANDLE)
    {
        t_state=x_crit_start();

        i4_result = handle_get_type_obj(h_config, &e_type, (VOID**) &pt_obj);
        if (i4_result != HR_OK || (e_type != CFG_HANDLE) || (pt_obj == NULL))
        {
            x_crit_end(t_state);
#ifdef DEBUG
            if(x_handle_valid(h_config))
            {
                CFG_ABORT(DBG_CAT_HANDLE, 1);
            }
#endif
            return CFGR_INV_HANDLE;
        }

        ps_config_name = pt_obj->pt_config->as_name;
        x_crit_end(t_state);
    }
    
    if ( (cfg_get_custom_fct_tbl(ps_config_name, &t_fct_tbl)==CFGR_OK)     &&
         (t_fct_tbl.pf_store != NULL)
        )
    {
        DBG_API(("{CFG} store config data using registered function.\n"));
        i4_result = t_fct_tbl.pf_store(h_dir,
                                       ps_pathname,
                                       h_config,
                                       t_fct_tbl.pv_store_tag);
    }
    else
    {
        i4_result = cfg_store_settings(h_dir, ps_pathname, h_config);
    }
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_fs_qry
 *
 * Description: This API queries if a configuration set exist on
 *              the specified persistent storage.
 *
 * Inputs:
 *    h_dir          - directory to query
 *    ps_pathname    - pathname of device to query
 *    ps_config_name - Name of configuration to query for
 *
 * Output:
 *    None
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV
 *    CFGR_INV_FILE_PATH
 *    CFGR_NO_SUCH_FILE
 *
 * Exceptions:
 *
 --------------------------------------------------------------------*/
INT32 x_cfg_fs_qry(
                  HANDLE_T    h_dir,
                  CHAR*       ps_pathname,
                  const CHAR* ps_config_name
                  )
{
    INT32          i4_result;
    HANDLE_T       h_file;
    CFG_FILE_API_T t_fct_tbl;

    DBG_API(("{CFG} x_cfg_fs_qry: h_dir %d ps_pathname %s ps_config_name %s\n", h_dir, ps_pathname, ps_config_name));
    
    if (ps_pathname == NULL)
    {
        return CFGR_INV_ARG;
    }

    if ( (cfg_get_custom_fct_tbl(ps_config_name, &t_fct_tbl)==CFGR_OK)     &&
         (t_fct_tbl.pf_query != NULL)
        )
    {
        DBG_API(("{CFG} query config data using registered function.\n"));
        i4_result = t_fct_tbl.pf_query(h_dir,
                                       ps_pathname,
                                       ps_config_name,
                                       t_fct_tbl.pv_query_tag
            );
    }
    else
    {
        if (! fs_raw_is_inited(h_dir, ps_pathname))
        {
            return CFGR_INV_FILE_PATH;
        }

        i4_result = x_fm_open(
            h_dir,
            ps_pathname,
            FM_OPEN_CREATE|FM_READ_WRITE,
            0644,
            FALSE,
            &h_file
            );
        if (i4_result != FMR_OK)
        {
            return fm_to_cfg_err(i4_result);
        }

        /* do raw search */
        fs_raw_read_lock(h_file);

        i4_result = cfg_fs_find_config(h_file, ps_config_name);

        fs_raw_unlock(h_file);
        x_fm_close(h_file);
    }
    

    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_fs_del
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
INT32 x_cfg_fs_del(
                  HANDLE_T    h_dir,
                  CHAR*       ps_pathname,
                  const CHAR* ps_config_name
                  )
{
    INT32         i4_result;
    HANDLE_T      h_file;

    DBG_API(("{CFG} x_cfg_fs_del: h_dir %d ps_pathname %s ps_config_name %s\n", h_dir, ps_pathname, ps_config_name));

    if (ps_pathname == NULL)
    {
        return CFGR_INV_ARG;
    }

    /* raw file system */
    if (! fs_raw_is_inited(h_dir, ps_pathname))
    {
        return CFGR_INV_FILE_PATH;
    }

    /* the only thing really allowed is to delete ALL */
    i4_result = x_fm_open(
                         h_dir,
                         ps_pathname,
                         FM_OPEN_CREATE|FM_READ_WRITE,
                         0644,
                         FALSE,
                         &h_file
                         );
    if (i4_result != FMR_OK)
    {
        return fm_to_cfg_err(i4_result);
    }

    fs_raw_write_lock(h_file);

    /* search for this configuration */
    i4_result = cfg_fs_del_config(h_file, ps_config_name);

    fs_raw_unlock(h_file);
    x_fm_close(h_file);

    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: x_cfg_fs_reg_fcts
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
 *    CFGR_OK          success
 *    CFGR_INV_ARG     pt_fct_tbl contain null function pointer.
 *
 * Exceptions:
 *
 --------------------------------------------------------------------*/
INT32 x_cfg_fs_reg_fcts
(
    HANDLE_T        h_config,
    CFG_FILE_API_T* pt_fct_tbl
)
{
    INT32           i4_result = CFGR_UNKNOWN;
    CFG_LIST_T*     pt_list   = NULL;

    /*
       Check input argument. If a function table structure is provided, but some
       function is not provided, then retunr an error.
    */
    if ( (pt_fct_tbl           != NULL)              &&
         (  (pt_fct_tbl->pf_load  == NULL)  ||
            (pt_fct_tbl->pf_store == NULL)  ||
            (pt_fct_tbl->pf_query == NULL)     )
        )
    {
        return CFGR_INV_ARG;
    }
            
    i4_result = cfg_get_write_lock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    i4_result = cfg_get_config_from_handle(h_config, &pt_list) ;
    
    if ( (i4_result == CFGR_OK)      &&
         (pt_list   != NULL)         
        )
    {
        if ( pt_fct_tbl != NULL )
        {
            pt_list->t_file_fct_tbl.pf_load     = pt_fct_tbl->pf_load;
            pt_list->t_file_fct_tbl.pv_load_tag = pt_fct_tbl->pv_load_tag;

            pt_list->t_file_fct_tbl.pf_store     = pt_fct_tbl->pf_store;
            pt_list->t_file_fct_tbl.pv_store_tag = pt_fct_tbl->pv_store_tag;

            pt_list->t_file_fct_tbl.pf_query     = pt_fct_tbl->pf_query;
            pt_list->t_file_fct_tbl.pv_query_tag = pt_fct_tbl->pv_query_tag;
        }
        else
        {
            /*
               If NULL function table, then un-register all custom function
               (e.g. set all fct pointers to NULL.
            */
            x_memset(&(pt_list->t_file_fct_tbl), 0, sizeof(CFG_FILE_API_T));
        }
    }

    i4_result = cfg_unlock();
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }
    
    return i4_result;
}


    
