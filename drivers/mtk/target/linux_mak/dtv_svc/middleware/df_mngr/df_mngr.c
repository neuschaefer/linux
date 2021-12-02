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
 * $RCSfile: df_mngr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all Download Function Manager API related 
 *      implementations.
 *------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                    include files
--------------------------------------------------------------------------*/
#include "x_df_mngr.h"
#include "df_mngr.h"
#include "_dfm_eng_data.h"
#include "dfm_hdlr.h"
#include "dfm_lock.h"

#include "tbl_mngr/x_tm.h"
#include "inc/u_sys_name.h"

#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "dfm_dbg.h"
#include "dfm_cli.h"
#include "dfm_eng.h"

/*--------------------------------------------------------------------------
       Globe Variable Declaration                                                
---------------------------------------------------------------------------*/
static BOOL                 b_g_dfm_inited = FALSE;
static DFM_T                t_g_sys_dfm;

/*--------------------------------------------------------------------------
       Static Functions Declaration                                              
---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name
 *      dfm_free_object
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static VOID dfm_free_object
(               
    DFM_T*                      pt_dfm
)
{
    if (pt_dfm == NULL) 
    {
        return;
    }   
    
    pt_dfm->s_name[0] = '\0';

    /*save the download data to file system*/

    return ;
}

/*-----------------------------------------------------------------------------
 * Name
 *      _svctx_handle_free_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static BOOL _dfm_handle_free_fct
(
    HANDLE_T                    h_handle,
    HANDLE_TYPE_T               e_type,
    VOID*                       pv_obj,
    VOID*                       pv_tag,
    BOOL                        b_req_handle
)
{
    DFM_T*                      pt_dfm = (DFM_T*)pv_obj;
    INT32                       i4_ret;

    DBG_INFO((_DFM_INFO"%s()\r\n", __func__));

    if ( b_req_handle == TRUE) 
    {
        return TRUE;
    }

    if ( pt_dfm == NULL || e_type != HT_DF_MNGR )
    {
        return TRUE;
    }

    i4_ret = handle_delink(&pt_dfm->t_hdl_link, h_handle);
    if(i4_ret != HR_OK)
    {
        return FALSE;
    }

    if (pv_tag != NULL)
    { /* DFM_LSTNR_T */
        x_mem_free(pv_tag);
    }

    if (pt_dfm->h_sys_dfm == h_handle) 
    {
        /* free the h_sys_dfm, so free everyone associated handle */
        DBG_INFO((_DFM_INFO"%s(), free the h_sys_dfm, so free everyone associated handle\r\n", __func__));
        handle_free_all(&(pt_dfm->t_hdl_link));
        dfm_free_object(pt_dfm);
        pt_dfm->h_sys_dfm = NULL_HANDLE;
    } 
    
    return TRUE;
}

static UINT8 dfm_find_logo_types
(
    LOGO_TYPE_ENTRY_T*          pt_list
)
{
    UINT8  ui1_type = 0x00;
    /*ui1_type = 0x3F;
    return ui1_type;*/
    
    while (pt_list != NULL)
    {
        ui1_type |= pt_list->ui1_logo_type;
        pt_list = pt_list->pt_next;
    }
    return ui1_type;
}

static LOGO_TYPE_ENTRY_T* dfm_find_entry_by_logo_type
(
    LOGO_TYPE_ENTRY_T*          pt_list, 
    DFM_LOGO_TYPE_T             t_logo_type
)
{
    /*if (pt_list != NULL)
    {
        return pt_list;
    }*/
    while (pt_list != NULL)
    {
        if (pt_list->ui1_logo_type == t_logo_type)
        {
            return pt_list;
        }
        pt_list = pt_list->pt_next;
    }
    
    return NULL;
}

/*--------------------------------------------------------------------------
     The implement of Functions                                            
---------------------------------------------------------------------------*/
INT32 x_dfm_open
(
    const CHAR*                 ps_dfm_name,
    HANDLE_T*                   ph_dfm
)
{
    DFM_T*                      pt_dfm     = NULL;
    INT32                       i4_ret;
    SIZE_T                      z_size;

    DBG_API((_DFM_API"%s("
                    "ps_dfm_name = %s,"
                    "ph_dfm = %d"
                    ")\r\n",
                    __func__,
                    ps_dfm_name,
                    ph_dfm
                    ));

    if (b_g_dfm_inited == FALSE) 
    {
        return DFMR_NOT_INIT;
    }

    z_size = x_strlen(ps_dfm_name);
    if (z_size == 0 || z_size > DFM_NAME_MAX_SIZE || ph_dfm == NULL)
    {
        return DFMR_INV_ARG;
    }

    pt_dfm = &t_g_sys_dfm;
    
    dfm_class_lock();
    if (x_strcmp(pt_dfm->s_name, ps_dfm_name) == 0)
    { 
        i4_ret = handle_alloc(
                            HT_DF_MNGR,
                            (VOID*)pt_dfm,
                            NULL,
                            _dfm_handle_free_fct,
                            ph_dfm
                            );
        if ( i4_ret != HR_OK) {
            DBG_ERROR((_DFM_ERROR"handle_alloc(%s) failed, i4_ret=%d\r\n", ps_dfm_name, i4_ret));
            i4_ret = DFMR_OUT_OF_RESOURCES;
            
            goto DFM_EXIT;
        }

        i4_ret = handle_link( &pt_dfm->t_hdl_link, *ph_dfm );
        if (i4_ret != HR_OK)
        {
            DBG_ERROR((_DFM_ERROR"handle_link(%s) failed, i4_ret=%d\r\n", ps_dfm_name, i4_ret));
            i4_ret = DFMR_FAIL;
            
            handle_free(*ph_dfm, FALSE);
            
            goto DFM_EXIT;
        }

        i4_ret = DFMR_OK;
    }
    else
    {
        i4_ret = DFMR_FAIL;
    }
    
DFM_EXIT:   
    dfm_class_unlock();

    return i4_ret;
}

INT32 x_dfm_close
(
    HANDLE_T                    h_dfm 
)
{
    INT32                       i4_ret;

    DBG_API((_DFM_API"%s("
                    "h_dfm = %d"
                    ")\r\n",
                    __func__,
                    h_dfm
                    ));

    if (b_g_dfm_inited == FALSE)
    {
        return DFMR_NOT_INIT;
    } 

    dfm_class_lock();

    i4_ret = handle_free(h_dfm, FALSE);
    if (i4_ret != HR_OK) 
    {
        dfm_class_unlock();
        return DFMR_FAIL;
    }
    dfm_class_unlock();

    return DFMR_OK;
}

INT32 x_dfm_load_logo
(
    HANDLE_T                    h_dfm,
    UINT16                      ui2_on_id,
    UINT16                      ui2_svc_id,
    UINT16                      ui2_scan_idx,
    VOID*                       pv_tag,
    x_dfm_nfy_fct               pt_nfy,
    HANDLE_T*                   ph_logo,
    DFM_COND_T*                 pe_cond
)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_obj_type;
    DFM_T*                      pt_dfm = NULL;  

    LOGO_CTRL_T*  pt_logo_ctrl = NULL;

    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    
    i4_ret = dfm_load_logo_ctrl_by_key(pt_dfm->h_root,ui2_on_id,ui2_svc_id,ui2_scan_idx,&pt_logo_ctrl);
    if (i4_ret != DFMR_OK)
    {
        dfm_class_unlock();
        return i4_ret;
    }
        
    i4_ret = handle_alloc(
                        HT_DFM_LOGO,
                        (VOID*)pt_logo_ctrl,
                        NULL,
                        logo_free_cb,
                        ph_logo
                        );
    if ( i4_ret != HR_OK) {
        DBG_ERROR((_DFM_ERROR"handle_alloc() failed, i4_ret=%d\r\n",  i4_ret));
        dfm_class_unlock();     
        return DFMR_OUT_OF_RESOURCES;
        
    }

    i4_ret = dfm_link_logo_req(  pt_logo_ctrl,
                        pv_tag,
                        pt_nfy,
                        *ph_logo
                        );
    if (i4_ret != DFMR_OK)
    {
        DBG_ERROR((_DFM_ERROR"dfm_link_logo_req() failed, i4_ret=%d\r\n",  i4_ret));
        dfm_class_unlock();
        return i4_ret;
        
    }

    *pe_cond = pt_logo_ctrl->e_cond;

    dfm_class_unlock();

    return DFMR_OK;
    
}

INT32 x_dfm_get_cond
(
    HANDLE_T                    h_dfm,
    HANDLE_T                    h_logo,
    DFM_COND_T*                 pe_cond
)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_obj_type;
    DFM_T*                      pt_dfm = NULL;
    LOGO_CTRL_T*                pt_logo_ctrl = NULL;

    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    
    if (((handle_get_type_obj (h_logo, &e_obj_type, (VOID**)(&pt_logo_ctrl))) != HR_OK) || 
        (e_obj_type != HT_DFM_LOGO))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }

    *pe_cond = pt_logo_ctrl->e_cond;

    dfm_class_unlock();

    i4_ret = DFMR_OK;
    return i4_ret;
    
}

INT32 x_dfm_get_logo_type
(
    HANDLE_T                    h_dfm,
    HANDLE_T                    h_logo,
    UINT8*                      pui1_logo_types,
    DFM_COND_T*                 pe_cond
)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_obj_type;
    DFM_T*                      pt_dfm          = NULL;
    LOGO_CTRL_T*                pt_logo_ctrl    = NULL;

    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif        
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    
    if (((handle_get_type_obj (h_logo, &e_obj_type, (VOID**)(&pt_logo_ctrl))) != HR_OK) || 
        (e_obj_type != HT_DFM_LOGO))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif        
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    if ( (pt_logo_ctrl->e_cond == DFM_COND_AVAILABLE) ||
        (pt_logo_ctrl->e_cond == DFM_COND_UPDATE) )
    {
        if (pt_logo_ctrl->b_simp_logo == TRUE)
        {
            *pui1_logo_types = DFM_LOGO_TYPE_SIMP;
        }
        else
        {
            *pui1_logo_types = 
                dfm_find_logo_types(pt_logo_ctrl->pt_logo_data->pt_logo_type_entry_list);       
        }
    }
        
    *pe_cond = pt_logo_ctrl->e_cond;

    dfm_class_unlock();

    i4_ret = DFMR_OK;
    return i4_ret;
    
}

INT32 x_dfm_get_logo_data
(
    HANDLE_T                    h_dfm,
    HANDLE_T                    h_logo,
    DFM_LOGO_TYPE_T             t_logo_type,
    WGL_HIMG_TPL_T*             pt_img,
    DFM_COND_T*                 pe_cond
)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_obj_type;
    DFM_T*                      pt_dfm              = NULL;
    LOGO_TYPE_ENTRY_T*          pt_logo_type_entry  = NULL;
    LOGO_CTRL_T*                pt_logo_ctrl        = NULL;

    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif  
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    
    if (((handle_get_type_obj (h_logo, &e_obj_type, (VOID**)(&pt_logo_ctrl))) != HR_OK) || 
        (e_obj_type != HT_DFM_LOGO))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif        
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    if ( (pt_logo_ctrl->e_cond == DFM_COND_AVAILABLE) ||
        (pt_logo_ctrl->e_cond == DFM_COND_UPDATE) )
    {
        if (pt_logo_ctrl->b_simp_logo == TRUE)
        {
            pt_logo_type_entry = pt_logo_ctrl->pt_simp_logo_entry;
        }
        else
        {
            pt_logo_type_entry = dfm_find_entry_by_logo_type(
                        pt_logo_ctrl->pt_logo_data->pt_logo_type_entry_list,
                        t_logo_type);
            if (NULL == pt_logo_type_entry)
            {
                DBG_ERROR((_DFM_ERROR"FAIL!!! in getting logo data by type!\n"));;
                *pt_img = NULL_HANDLE;
                *pe_cond = DFM_COND_TRANSITION;

                dfm_class_unlock();
                return DFMR_OK;            
            }
        }
        *pt_img = (WGL_HIMG_TPL_T)(pt_logo_type_entry->t_wgl_himg);
        
    }

    *pe_cond = pt_logo_ctrl->e_cond;

    dfm_class_unlock();

    i4_ret = DFMR_OK;

    return i4_ret;  
}

INT32 x_dfm_free_logo
(
    HANDLE_T                    h_dfm,
    HANDLE_T                    h_logo
)
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_obj_type;
    DFM_T*                      pt_dfm          = NULL;
    LOGO_CTRL_T*                pt_logo_ctrl    = NULL;

    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif        
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    
    if (((handle_get_type_obj (h_logo, &e_obj_type,(VOID**)(&pt_logo_ctrl))) != HR_OK) || 
        (e_obj_type != HT_DFM_LOGO))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif        
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }
    
    i4_ret = handle_free(h_logo, FALSE);
    if (HR_OK != i4_ret)
    {
        dfm_class_unlock();
        return DFMR_FAIL;
    }
    
    dfm_class_unlock();

    return DFMR_OK; 
}

INT32 x_dfm_change_svc(
        HANDLE_T         h_dfm,
        UINT16           ui2_onid,
        UINT16           ui2_service_ID,
        UINT16           ui2_scan_idx
                         )
{
    INT32                       i4_ret;
    HANDLE_TYPE_T               e_obj_type;
    DFM_T*                      pt_dfm          = NULL;

    dfm_class_lock();
    if (((handle_get_type_obj (h_dfm, &e_obj_type, (VOID**)(&pt_dfm))) != HR_OK) || 
        (e_obj_type != HT_DF_MNGR))
    {
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INV_ARG);
#endif        
        dfm_class_unlock();
        return DFMR_INV_HANDLE;
    }

    i4_ret = dfm_logo_chg_svc(ui2_onid, ui2_service_ID, ui2_scan_idx);
    if (i4_ret != DFMR_OK)
    {
        dfm_class_unlock();
        return i4_ret;
    }
    
    dfm_class_unlock();

    return DFMR_OK; 
}

/*-----------------------------------------------------------------------------
 * Name
 *      dfm_init
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
INT32 dfm_init 
(
    UINT16                      ui2_max_num_svc_obj,
    UINT16                      ui2_max_num_logo_obj
)
{
    INT32                       i4_ret;
    DFM_T*                      pt_dfm = NULL;
    BOOL                        b_flag_init = FALSE;
    TM_COND_T                   e_root_cond;
    
    /*check the flag of init*/
    if (b_g_dfm_inited == TRUE)
    {
        return DFMR_ALREADY_INIT;
    }
    
    /*create the lock*/
    i4_ret = dfm_init_lock_module();
    if (i4_ret != DFMR_OK)
    {
        return i4_ret;
    }
    
    /* Init Download Function Manager */
    dfm_class_lock();

    pt_dfm = &(t_g_sys_dfm);

    i4_ret = DFMR_OK;
    x_memset(pt_dfm, 0, sizeof(DFM_T));
    pt_dfm->s_name[0] = '\0';

    x_strcpy(pt_dfm->s_name, SN_MAIN_DFM);

    i4_ret = handle_link_init(&(pt_dfm->t_hdl_link));
    if(i4_ret != HR_OK) 
    {
        DBG_ERROR((_DFM_ERROR"DFM init failed, because of handle_link_init() failed\r\n"));
        dfm_class_unlock();
        return DFMR_FAIL;       
    }

    /* create the one - system dfm handle */
    i4_ret = handle_alloc( HT_DF_MNGR,
                    pt_dfm,
                    NULL,
                    _dfm_handle_free_fct,
                    &(pt_dfm->h_sys_dfm) );
    if(i4_ret != HR_OK) 
    {
        DBG_ERROR((_DFM_ERROR" DFM init failed, because of handle_alloc failed\r\n"));
        dfm_class_unlock();
        return DFMR_FAIL;
    }
    
    /*open table manager's root handle*/
    i4_ret = x_tm_open_root("DFM_ROOT",
                    TM_SRC_TYPE_MPEG_2_BRDCST,
                    (VOID*)SN_TUNER_TER_DIG_0,
                    NULL,
                    NULL,
                    &(pt_dfm->h_root), /* BRDCST type */
                    &(e_root_cond));
    if ((i4_ret != TMR_OK) ||
        (i4_ret == TMR_OK && (e_root_cond != TM_COND_AVAILABLE && 
                              e_root_cond != TM_COND_UPDATE)))
    {
        DBG_ERROR((_DFM_ERROR" DFM init failed, because of root handle open!\r\n"));
        dfm_class_unlock(); 
        return DFMR_FAIL;
    }
    
    /*Init the logo handler*/
    b_flag_init = dfm_logo_hdlr_init(ui2_max_num_svc_obj,ui2_max_num_logo_obj); 
    if (b_flag_init != TRUE)
    {
        DBG_ERROR((_DFM_ERROR" DFM init failed, because of logo handler inited!\r\n"));
        x_tm_free(pt_dfm->h_root);
        dfm_class_unlock();
        return DFMR_FAIL;
    }

    /*Init the download manager engine*/
    i4_ret = dfm_eng_init();
    if (i4_ret != DFMR_OK)
    {
        DBG_ERROR((_DFM_ERROR" DFM init failed, because of engine inited!\r\n"));
        dfm_logo_hdlr_deinit();
        x_tm_free(pt_dfm->h_root);
        dfm_class_unlock();
        return DFMR_FAIL;
    }
    
    b_g_dfm_inited = TRUE;

    dfm_class_unlock();
    
    if (b_g_dfm_inited == TRUE)
    {
        dfm_cli_init();
    }
    
    return DFMR_OK;
}


INT32 dfm_deinit ( VOID )
{
    dfm_class_lock();

    dfm_logo_hdlr_deinit();
    
    /*free Root handle*/
    x_tm_free(t_g_sys_dfm.h_root);
    
    dfm_class_unlock();
    return DFMR_OK;
}

