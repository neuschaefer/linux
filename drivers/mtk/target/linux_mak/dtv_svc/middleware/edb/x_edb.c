/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2006, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: ,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "handle/u_handle_grp.h"
#include "handle/handle.h"
#include "hash/hash_api.h"
#include "dbg/x_dbg.h"
#include "edb/x_edb.h"
#include "edb/edb_slctr.h"
#include "edb/edb_pool.h"
#include "dt/x_dt.h"
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "edb/edb_hash_api.h"




/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  data declarations
  ----------------------------------------------------------------------------*/

typedef struct _EDB_ENG_KEY_INFO_BASE_T
{
    BRDCST_TYPE_T       e_brdcst_type;
    UINT16              ui2_svl_id;
} EDB_ENG_KEY_INFO_BASE_T;


/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/
extern HANDLE_T    g_edb_handle_hash;
extern HANDLE_T    g_edb_client_hash;
#if 0

static INT32 _get_brdcst_type_frm_cmd (
        EDB_COMMAND_T*         pt_cmd,
        BRDCST_TYPE_T*         pe_brdcst_type)
{
    INT32                   i4_ret           = 0;
    EDB_ENG_OPEN_DATA_T*    pt_eng_open_data = NULL;
    EDB_COMMAND_CODE_T      e_code;

    if (pt_cmd == NULL || pe_brdcst_type == NULL)
    {
        return EDBR_INV_ARG;
    }

    while ((e_code = pt_cmd->e_code) != EDB_CMD_END)
    {
        switch (e_code)
        {
            case EDB_CMD_OPEN_DATA:
                pt_eng_open_data = pt_cmd->u.pt_eng_open_data;
                EDB_CHECK_RET( 
                        (pt_eng_open_data == NULL),
                        (EDBR_INV_ARG),
                        (("Arguments is wrong,pt_cmd is null\r\n"))
                        );
                *pe_brdcst_type  = pt_eng_open_data->e_brdcst_type;

                break;
            default:
                break;
        }
        pt_cmd++;
    }

    return i4_ret;
}


static  INT32 _edb_get_eng_from_command(
        EDB_COMMAND_T*         pt_cmd,
        EDB_ENG_T**            ppt_edb_eng)
{
    INT32               i4_ret = 0;
    BRDCST_TYPE_T       e_brdcst_type;
    EDB_ENG_T*          pt_edb_eng = NULL;

    EDB_CHECK_RET( 
            (pt_cmd == NULL || ppt_edb_eng == NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    i4_ret =_get_brdcst_type_frm_cmd(pt_cmd, &e_brdcst_type);
    if (i4_ret != EDBR_OK)
    {
        return i4_ret;
    }

    /*DBG_INFO((_EDB_INFO"x_edb_create e_brdcst_type=%d\r\n",e_brdcst_type));*/

    if (EDB_DVB_BRDCST_GRP & (MAKE_BIT_MASK_8(e_brdcst_type)))
    {
        e_brdcst_type = EDB_DVB_BRDCST_GRP;
    }


    pt_edb_eng = edb_find_eng_by_brdcst_type(e_brdcst_type);
    if(pt_edb_eng == NULL )
    {
        /*DBG_INFO((_EDB_INFO"edb_find_eng_by_brdcst_type faid\r\n"));*/
        return EDBR_INV_COMMAND;
    }

    *ppt_edb_eng = pt_edb_eng;
    return i4_ret;

}
#endif

extern INT32 x_edb_create (
        const CHAR*          ps_edb_name,
        EDB_COMMAND_T*       pt_cmd,
        VOID*                pv_nfy_tag,
        x_edb_nfy_fct        pf_nfy,
        HANDLE_T*            ph_edb_client)
{
    INT32                    i4_ret        = 0;
    EDB_ENG_T*               pt_eng        = NULL;
    EDB_T*                   pt_edb        = NULL;   
    //HANDLE_T                 h_edb         = NULL_HANDLE;
    EDB_REQUEST_BASE_CTRL_T* pt_edb_client = NULL;
    HANDLE_T                 h_edb_client  = NULL_HANDLE;
    SIZE_T                   z_edb_name_len=0;
    BOOL                     b_new_edb = FALSE;
    EDB_COMMAND_T*           pt_tmp_cmd = NULL;
    EDB_ENG_CTRL_ENTRY_T*    pt_eng_ctrl_entry = NULL;
    HANDLE_T                 h_edb_eng = NULL_HANDLE;
    EDB_COND_T               e_cond;


    BRDCST_TYPE_T            e_brdcst_type = BRDCST_TYPE_UNKNOWN;

    EDB_CHECK_RET( 
            (ps_edb_name == NULL || ph_edb_client == NULL),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    DBG_INFO((_EDB_INFO"ps_edb_name=%s\r\n",ps_edb_name));
    if (!edb_is_inited())
    {
        DBG_INFO((_EDB_INFO"x_edb_create return=%d\n\r",  EDBR_NOT_INIT));
        return EDBR_INTERNAL_ERROR;
    }

    edb_eng_list_lock();

    /*get edb handle by name*/
    i4_ret = edb_handle_get_edb_by_name(ps_edb_name, &pt_edb);

    if (i4_ret == EDBR_OK && pt_edb != NULL)/*EDB have been created*/ 
    {

    }
    else/*Create  new EDB handle */
    {
        pt_edb = (EDB_T*)EDB_OS_ALLOC(sizeof(EDB_T));
        if (pt_edb == NULL)
        {
            DBG_INFO((_EDB_INFO" Alloc error\r\n"));    
            edb_eng_list_unlock();
            return EDBR_OUT_OF_MEM;
        }

        x_memset(pt_edb,0x0,sizeof(EDB_T));


        z_edb_name_len = x_strlen(ps_edb_name);
        if (z_edb_name_len >= EDB_NAME_MAX_SIZE)
        {
            z_edb_name_len = EDB_NAME_MAX_SIZE;
        }
        x_memset(pt_edb->s_name,0x0,EDB_NAME_MAX_SIZE+1);
        x_strncpy(pt_edb->s_name,ps_edb_name,z_edb_name_len);

        pt_edb->e_cond          = EDB_COND_STOPPED;

        i4_ret = edb_pool_create(
                EDB_POOL_HASH_CHANNEL_TABLE_SIZE,
                x_dt_get_brdcst_utc(NULL,NULL),
                &(pt_edb->t_edb_event_pool)
                );
        if (i4_ret != EDBR_OK)
        {
            DBG_INFO(("edb_pool_create Error ui2_total_channels_cap=%d\r\n",EDB_POOL_HASH_CHANNEL_TABLE_SIZE));
            edb_pool_delete(&(pt_edb->t_edb_event_pool));
            edb_eng_list_unlock();
            return EDBR_OUT_OF_RESOURCE;
        }
        DBG_INFO((_EDB_INFO"Create EDB Pool success\r\n"));
        b_new_edb = TRUE;
    }/*Create  new EDB handle */


    pt_edb_client = EDB_OS_ALLOC(sizeof(EDB_REQUEST_BASE_CTRL_T));
    if (pt_edb_client == NULL)
    {
        i4_ret = EDBR_OUT_OF_MEM;
        goto EXIT_CREATE;
    }
    x_memset(pt_edb_client ,0x0,sizeof(EDB_REQUEST_BASE_CTRL_T));
    
    /*alloc handle for use request*/
    if (handle_alloc (EDBT_EDB_CLIENT,
                      ((VOID*) pt_edb_client),
                      NULL,
                      edb_edb_handle_free,
                      &h_edb_client) != HR_OK)
    {   
        i4_ret = EDBR_OUT_OF_HANDLES;;
        goto EXIT_CREATE;
    }


    pt_edb_client->h_edb_client = h_edb_client;
    pt_edb_client->b_creater    = b_new_edb;
    pt_edb_client->pf_nfy       = pf_nfy;
    pt_edb_client->pv_nfy_tag   = pv_nfy_tag;
    pt_edb_client->pt_edb       = pt_edb;

    if (b_new_edb)
    {
        pt_edb->h_creater = h_edb_client;
        //if (EDBR_OK != edb_handle_add_edb(ps_edb_name,pt_edb->h_creater))//FAN
        if (EDBR_OK != edb_handle_add_edb(ps_edb_name,pt_edb))
        {
            i4_ret = EDBR_INTERNAL_ERROR;
            goto EXIT_CREATE;
        }
        /*create engine data*/
        pt_tmp_cmd = pt_cmd;
        while (pt_tmp_cmd->e_code != EDB_CMD_END)
        {
            if (pt_tmp_cmd->e_code >= EDB_CMD_MAX_NUM)
            {
                i4_ret = EDBR_INV_COMMAND;
                goto EXIT_CREATE;
            }

            if (pt_tmp_cmd->e_code == EDB_CMD_OPEN_DATA)
            {
                /*alloc handle for use request*/
                pt_eng_ctrl_entry = (EDB_ENG_CTRL_ENTRY_T*) EDB_OS_ALLOC(sizeof(EDB_ENG_CTRL_ENTRY_T));
                if (pt_eng_ctrl_entry == NULL)
                {
                    i4_ret = EDBR_OUT_OF_MEM;
                    goto EXIT_CREATE;
                }

                x_memset(((VOID*) pt_eng_ctrl_entry), 0, sizeof(EDB_ENG_CTRL_ENTRY_T));


                e_brdcst_type  = pt_tmp_cmd->u.pt_eng_open_data->e_brdcst_type;

                if (EDB_DVB_BRDCST_GRP & (MAKE_BIT_MASK_8(e_brdcst_type)))
                {
                    e_brdcst_type = EDB_DVB_BRDCST_GRP;
                }

                pt_eng = edb_find_eng_by_brdcst_type(e_brdcst_type);

                if (pt_eng == NULL)
                {
                    i4_ret = EDBR_ENG_NOT_FOUND;
                    goto EXIT_CREATE;
                }

                pt_eng_ctrl_entry->pt_eng = pt_eng;
                pt_eng_ctrl_entry->h_eng_ctrl = NULL_HANDLE;
                pt_eng_ctrl_entry->ui2_svl_id = pt_tmp_cmd->u.pt_eng_open_data->ui2_svl_id;
                pt_eng_ctrl_entry->pt_edb = pt_edb;

                x_strncpy(pt_eng_ctrl_entry->ac_tuner_name,pt_tmp_cmd->u.pt_eng_open_data->ps_tuner_name,
                          (SIZE_T)(
                                  x_strlen(pt_tmp_cmd->u.pt_eng_open_data->ps_tuner_name) > EDB_MAX_TUNER_NAME ?
                                  EDB_MAX_TUNER_NAME : x_strlen(pt_tmp_cmd->u.pt_eng_open_data->ps_tuner_name)
                                  )
                         );

                if (handle_alloc (EDBT_EDB_ENG,
                                  ((VOID*) pt_eng_ctrl_entry),
                                  NULL,
                                  edb_edb_handle_free,
                                  &h_edb_eng) != HR_OK)
                {   
                    i4_ret = EDBR_OUT_OF_HANDLES;;
                    goto EXIT_CREATE;
                }
                pt_eng_ctrl_entry->h_eng_ctrl = h_edb_eng;
                if (EDBR_OK != pt_eng_ctrl_entry->pt_eng->t_edb_fct_tbl.pf_create_eng_obj(
                                h_edb_eng,
                                pt_tmp_cmd,
                                &(pt_eng_ctrl_entry->pt_eng_data),
                                &e_cond))
                {
                    i4_ret = EDBR_INTERNAL_ERROR;;
                    goto EXIT_CREATE;
                }

                DLIST_INSERT_TAIL(pt_eng_ctrl_entry,&( pt_edb->t_eng_ctrl_list),t_link);

            }

            pt_tmp_cmd++;
        }
        /*set cmd to each engine ctrl*/
        DLIST_FOR_EACH(pt_eng_ctrl_entry,&( pt_edb->t_eng_ctrl_list), t_link)
        {
            pt_tmp_cmd = pt_cmd;
            if (EDBR_OK != pt_eng_ctrl_entry->pt_eng->t_edb_fct_tbl.pf_set(
                            pt_eng_ctrl_entry->h_eng_ctrl,
                            pt_tmp_cmd))
            {
                /*TODO*/
            }
        }
    }

    if (EDBR_OK != edb_handle_add_client(h_edb_client,pt_edb_client))
    {
        i4_ret = EDBR_INTERNAL_ERROR;
        goto EXIT_CREATE;
    }

    *ph_edb_client = h_edb_client;

    edb_eng_list_unlock();
    return EDBR_OK;


EXIT_CREATE:

    edb_handle_del_client(h_edb_client);

    if (pt_edb_client)
    {
        EDB_OS_FREE(pt_edb_client);
    }

    if (b_new_edb)
    {
        /*clean engine ctrl data, TODO*/

        edb_pool_delete(&(pt_edb->t_edb_event_pool));
        EDB_OS_FREE(pt_edb);
    }

    edb_eng_list_unlock();
    return i4_ret;
}



static VOID _edb_delete_edb( VOID* pv_obj )
{
    edb_delete_edb(pv_obj);
}


extern INT32 x_edb_destroy (
        HANDLE_T    h_edb_client)
{
    INT32       i4_ret = 0;
    VOID*       pv_obj  = NULL;
    SIZE_T      z_client_num = 0;


    EDB_CHECK_RET( 
            (h_edb_client == NULL_HANDLE),
            (EDBR_INV_ARG),
            (("Arguments is wrong\r\n"))
            );

    edb_eng_list_lock();

    i4_ret = edb_handle_get_client_obj(h_edb_client,&pv_obj);
    if (i4_ret == EDBR_OK)
    {
        DBG_INFO(("EDB client handle=0x%x\r\n",h_edb_client));
    }

    i4_ret = edb_handle_del_client(h_edb_client);
    if (i4_ret != EDBR_OK)
    {
        edb_eng_list_unlock();
        return i4_ret;
    }


    if (g_edb_client_hash != NULL_HANDLE)
    {
        edb_hash_get_size(g_edb_client_hash, &z_client_num);
        if (z_client_num == (SIZE_T)0x0)//CLean EDB_T
        {
            if (g_edb_handle_hash != NULL_HANDLE)
            {
                hash_delete(g_edb_handle_hash, _edb_delete_edb );
                g_edb_handle_hash = NULL_HANDLE;
            }

        }
    }

    edb_eng_list_unlock();
    return i4_ret;
}


extern INT32 x_edb_set_listener (
        HANDLE_T             h_edb_client,
        VOID*                pv_nfy_tag,
        x_edb_nfy_fct        pf_nfy)
{
    INT32                       i4_ret        = EDBR_INTERNAL_ERROR;
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client = NULL;

    edb_eng_list_lock();

    i4_ret = edb_handle_get_edb_client_obj(h_edb_client,&pt_edb_client);
    if (pt_edb_client != NULL &&
        i4_ret == EDBR_OK)
    {
        pt_edb_client->pf_nfy       = pf_nfy;
        pt_edb_client->pv_nfy_tag   = pv_nfy_tag;
    }
    edb_eng_list_unlock(); 
    return i4_ret;
}

extern INT32 x_edb_set (
        HANDLE_T            h_edb_client,
        EDB_COMMAND_T*      pt_set_cmd)
{
    INT32                       i4_ret              = EDBR_INTERNAL_ERROR;
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client       = NULL;
    EDB_ENG_CTRL_LIST_T*        pt_eng_ctrl_list    = NULL;
    EDB_ENG_CTRL_ENTRY_T*       pt_eng_ctrl_entry   = NULL;
    EDB_COMMAND_T*              pt_tmp_cmd          = NULL;
    BOOL                        fg_flag             = FALSE;

    EDB_CHECK_RET( 
            (h_edb_client == NULL_HANDLE || pt_set_cmd == NULL),
            (i4_ret),
            ((_EDB_INFO"EDB client handle is null\r\n"))
            );

    edb_eng_list_lock();

    i4_ret = edb_handle_get_edb_client_obj(h_edb_client,&pt_edb_client);
    if (pt_edb_client != NULL && pt_edb_client->pt_edb != NULL &&  i4_ret == EDBR_OK)
    {
        pt_eng_ctrl_list = &(pt_edb_client->pt_edb->t_eng_ctrl_list);
        DLIST_FOR_EACH(pt_eng_ctrl_entry, pt_eng_ctrl_list, t_link)
        {
            pt_tmp_cmd = pt_set_cmd;
            i4_ret = pt_eng_ctrl_entry->pt_eng->t_edb_fct_tbl.pf_set(
                            pt_eng_ctrl_entry->h_eng_ctrl,
                            pt_tmp_cmd);
            
            if (EDBR_OK != i4_ret)
            {
                fg_flag = FALSE;
            }
            else
            {
                fg_flag = TRUE;
            }
        }
    }

    if (fg_flag == FALSE)
    {
        i4_ret = EDBR_ENG_NOT_FOUND;
    }

    edb_eng_list_unlock();    
    return i4_ret;
}

extern INT32 x_edb_get_info_len (
        HANDLE_T            h_edb_client,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len)
{
    INT32                       i4_ret        = EDBR_INTERNAL_ERROR;
    EDB_ENG_T*                  pt_eng        = NULL;
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client = NULL;
    EDB_COND_T                  e_cond;
    EDB_ENG_CTRL_LIST_T*        pt_eng_ctrl_list    = NULL;
    EDB_ENG_CTRL_ENTRY_T*       pt_eng_ctrl_entry   = NULL;    
    EDB_ENG_KEY_INFO_BASE_T*    pt_key_info_base    = NULL;

    EDB_CHECK_RET( 
            (h_edb_client == NULL_HANDLE || pv_key_info == NULL),
            (i4_ret),
            ((_EDB_INFO"EDB client handle is null OR key is null\r\n"))
            );

	edb_pool_lock_rd();
    edb_eng_list_lock();

    i4_ret = edb_handle_get_edb_client_obj(h_edb_client,&pt_edb_client);

    if (i4_ret == EDBR_OK &&
        pt_edb_client != NULL)
    {
        pt_key_info_base = (EDB_ENG_KEY_INFO_BASE_T*) pv_key_info;

        pt_eng_ctrl_list = &(pt_edb_client->pt_edb->t_eng_ctrl_list);
        DLIST_FOR_EACH(pt_eng_ctrl_entry, pt_eng_ctrl_list, t_link)
        {
            if(
                    (pt_eng_ctrl_entry->pt_eng->e_brdcst_type & (MAKE_BIT_MASK_8(pt_key_info_base->e_brdcst_type))) &&
                    (pt_eng_ctrl_entry->ui2_svl_id == pt_key_info_base->ui2_svl_id)
              )
            {
                pt_eng = pt_eng_ctrl_entry->pt_eng;
                break;
            }
        }
    }

    if (pt_eng != NULL)
    {
        i4_ret = pt_eng->t_edb_fct_tbl.pf_get_event_info_len(
                pt_eng_ctrl_entry->h_eng_ctrl,
                e_key_type,
                pv_key_info,
                pz_event_info_len,
                &e_cond
                );
    }
    else
    {
        i4_ret = EDBR_ENG_NOT_FOUND;
    }
		
    edb_eng_list_unlock();
	edb_pool_unlock();
	
    return i4_ret;

}

extern INT32 x_edb_get_info (
        HANDLE_T            h_edb_client,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info)
{
    INT32                       i4_ret              = EDBR_INTERNAL_ERROR;
    EDB_ENG_T*                  pt_eng              = NULL;
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client       = NULL;
    EDB_COND_T                  e_cond;
    EDB_ENG_CTRL_LIST_T*        pt_eng_ctrl_list    = NULL;
    EDB_ENG_CTRL_ENTRY_T*       pt_eng_ctrl_entry   = NULL;  
    EDB_ENG_KEY_INFO_BASE_T*    pt_key_info_base    = NULL; 


    EDB_CHECK_RET( 
            (h_edb_client == NULL_HANDLE),
            (i4_ret),
            ((_EDB_INFO"EDB client handle is null\r\n"))
            );

	edb_pool_lock_rd();
	edb_eng_list_lock();
	
    i4_ret = edb_handle_get_edb_client_obj(h_edb_client,&pt_edb_client);

    if (i4_ret == EDBR_OK &&
        pt_edb_client != NULL)
    {
        pt_key_info_base = (EDB_ENG_KEY_INFO_BASE_T*) pv_key_info;

        pt_eng_ctrl_list = &(pt_edb_client->pt_edb->t_eng_ctrl_list);
        DLIST_FOR_EACH(pt_eng_ctrl_entry, pt_eng_ctrl_list, t_link)
        {
            if(
                    (pt_eng_ctrl_entry->pt_eng->e_brdcst_type & (MAKE_BIT_MASK_8(pt_key_info_base->e_brdcst_type))) &&
                    (pt_eng_ctrl_entry->ui2_svl_id == pt_key_info_base->ui2_svl_id)
              )
            {
                pt_eng = pt_eng_ctrl_entry->pt_eng;
                break;
            }
        }
    }

    if (pt_eng != NULL)
    {
        i4_ret = pt_eng->t_edb_fct_tbl.pf_get_event_info(
                pt_eng_ctrl_entry->h_eng_ctrl,
                e_key_type,
                pv_key_info,
                pz_event_info_len,
                pv_event_info,
                &e_cond
                );
    }
    else
    {
        i4_ret = EDBR_ENG_NOT_FOUND;
    }
	
	edb_eng_list_unlock();
	edb_pool_unlock();   
	
    return i4_ret;
}





VOID x_edb_pool_lock_rd(VOID)
{
    edb_pool_lock_rd();
}


VOID x_edb_pool_unlock_rd(VOID)
{
    edb_pool_unlock();
}








