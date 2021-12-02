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
#include "os/inc/x_os.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "edb/edb_hash_api.h"
#include "svl/x_svl.h"
#include "tsl/x_tsl.h"
#include "tbl_mngr/u_tm.h"
#include "tbl_mngr/tm_api.h"

#include "edb/edb_eng.h"
#include "edb/edb_slctr.h"
#include "edb/edb_pool.h"



HANDLE_T    g_edb_handle_hash            =   NULL_HANDLE;/*EDB_T hash table         key:ps_name,obj HANDLE 0f EDB_T**/
HANDLE_T    g_edb_client_hash            =   NULL_HANDLE;/*BASE_CTRL_T hash table   key:handle, obj BASE_CTRL_T* */


INT32 edb_handle_init(SIZE_T z_size,SIZE_T z_client_num)
{
    INT32   i4_ret = 0 ;
    i4_ret = edb_hash_create(z_size, &g_edb_handle_hash);
    EDB_CHECK_RET( 
            (g_edb_handle_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Create EDB list fail\r\n"))
            );
    DBG_INFO((_EDB_INFO"Create EDB hash success\r\n"));

    i4_ret = edb_hash_create(z_client_num, &g_edb_client_hash);
    EDB_CHECK_RET( 
            (g_edb_client_hash == NULL_HANDLE),
            (EDBR_INTERNAL_ERROR),
            (("Create EDB client list fail\r\n"))
            );
    DBG_INFO((_EDB_INFO"Create EDB client request list success\r\n"));

    return i4_ret;
}

INT32 edb_handle_add_edb(const CHAR* ps_name,EDB_T* pt_edb)
{
    INT32   i4_ret = 0 ;
    EDB_CHECK_RET( 
            (g_edb_handle_hash == NULL_HANDLE || ps_name == NULL || pt_edb == NULL),
            (EDBR_INTERNAL_ERROR),
            (("edb handle not init\r\n"))
            );

    i4_ret = edb_hash_add_obj(g_edb_handle_hash, 
                              (const INT8 *) ps_name, x_strlen(ps_name), (VOID *) pt_edb, NULL);
    return i4_ret;
}

INT32 edb_handle_del_edb(const CHAR* ps_name)
{
    INT32       i4_ret = 0 ;
    EDB_T*      pt_edb = NULL;
    i4_ret = edb_handle_get_edb_by_name(ps_name,&pt_edb);
    EDB_CHECK_RET( 
            (g_edb_handle_hash == NULL_HANDLE || pt_edb == NULL),
            (EDBR_INFO_NOT_FOUND),
            (("Can not find EDB handle by name %s\r\n",ps_name))
            );

    i4_ret = edb_hash_remove_obj(g_edb_handle_hash, (const INT8 *)ps_name, x_strlen(ps_name),NULL);

    return i4_ret;
}


INT32 edb_handle_get_edb_by_name(const CHAR* ps_name,EDB_T**  ppt_edb)
{
    INT32   i4_ret = 0 ;

    EDB_CHECK_RET( 
            (g_edb_handle_hash == NULL_HANDLE || ps_name == NULL || ppt_edb== NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = edb_hash_get_obj(g_edb_handle_hash, 
                              (const INT8 *) ps_name, x_strlen(ps_name), (VOID**) ppt_edb);


    return i4_ret;
}





INT32 edb_handle_add_client(HANDLE_T h_client,VOID* pv_obj)
{
    INT32   i4_ret = 0 ;
    EDB_CHECK_RET( 
            (g_edb_client_hash == NULL_HANDLE || h_client == NULL_HANDLE || pv_obj==NULL),
            (EDBR_INTERNAL_ERROR),
            (("Wrong arguments\r\n"))
            );

    i4_ret = edb_hash_add_obj(g_edb_client_hash, 
                              (const INT8 *) (&h_client), sizeof(HANDLE_T), (VOID *) pv_obj, NULL);
    return i4_ret;
}

INT32 edb_handle_del_client(HANDLE_T h_client)
{
    INT32       i4_ret = 0 ;
    VOID*       pv_obj = NULL;

    i4_ret = edb_handle_get_client_obj(h_client,&pv_obj);
    EDB_CHECK_RET( 
            (g_edb_client_hash == NULL_HANDLE),
            (EDBR_INFO_NOT_FOUND),
            (("Can not find EDB client handle\r\n"))
            );

    i4_ret = edb_hash_remove_obj(g_edb_client_hash, (const INT8 *)(&h_client), sizeof(HANDLE_T),NULL);
    x_handle_free(h_client);
    return i4_ret;
}


INT32 edb_handle_get_client_obj(HANDLE_T h_client,VOID** ppb_obj)
{
    INT32   i4_ret = 0 ;
    VOID*   pv_obj = NULL;

    EDB_CHECK_RET( 
            (g_edb_client_hash == NULL_HANDLE || h_client == NULL_HANDLE),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    if (ppb_obj != NULL)
    {
        i4_ret = edb_hash_get_obj(g_edb_client_hash, 
                                  (const INT8 *) (&h_client), sizeof(HANDLE_T), (VOID**) ppb_obj);
    }else
    {
        i4_ret = edb_hash_get_obj(g_edb_client_hash, 
                                  (const INT8 *) (&h_client), sizeof(HANDLE_T), (VOID**)&pv_obj);
    }

    if (pv_obj != NULL)
    {
        i4_ret = EDBR_OK;
    }

    EDB_CHECK_RET( 
            (i4_ret != EDBR_OK),
            (EDBR_INTERNAL_ERROR),
            (("Can not get edb client obj by hanele 0x%x\r\n",h_client))
            );

    return i4_ret;
}








BOOL edb_edb_handle_free ( HANDLE_T       h_handle,
                           HANDLE_TYPE_T  e_type,
                           VOID*          pv_obj,
                           VOID*          pv_tag,
                           BOOL           b_req_handle)
{
    if (e_type == EDBT_EDB)
    {
        EDB_T*                   pt_edb  = NULL;
        EDB_EVENT_POOL_T*        pt_pool = NULL;
        EDB_ENG_CTRL_ENTRY_T*    pt_eng_ctrl_entry = NULL;

        DBG_INFO(("edb_edb_handle_free type:EDBT_EDB\r\n"));
        pt_edb = pv_obj;
        if (pt_edb == NULL){return FALSE;}
        /*free pool memory*/
        pt_pool = &(pt_edb->t_edb_event_pool);
        if (pt_pool != NULL)
        {
            edb_pool_delete(pt_pool);
        }



        DLIST_FOR_EACH(pt_eng_ctrl_entry,&( pt_edb->t_eng_ctrl_list), t_link)
        {
            DLIST_REMOVE(pt_eng_ctrl_entry, &( pt_edb->t_eng_ctrl_list), t_link);
            if (pt_eng_ctrl_entry->pt_eng_data != NULL)
            {
                EDB_OS_FREE(pt_eng_ctrl_entry->pt_eng_data);
            }

            EDB_OS_FREE(pt_eng_ctrl_entry);
        }


        /*TODO free eng ctrl & pv_data*/

        /*free edb object memory*/
        if (pt_edb != NULL)
        {
            EDB_OS_FREE(pt_edb);
        }
    }
    else if(e_type == EDBT_EDB_CLIENT)
    {
        EDB_REQUEST_BASE_CTRL_T* pt_edb_client    = NULL;
        pt_edb_client = pv_obj;

        if (pt_edb_client != NULL)
        {
            EDB_OS_FREE(pt_edb_client);
        }
    }

    return TRUE;
}


INT32  edb_handle_get_edb_client_obj(HANDLE_T h_edb_client,EDB_REQUEST_BASE_CTRL_T** ppt_edb_client)
{
    INT32                   i4_ret = 0;
    HANDLE_TYPE_T           e_type;

    EDB_CHECK_RET( 
            (h_edb_client == NULL_HANDLE || ppt_edb_client == NULL),
            (EDBR_INV_HANDLE),
            (("EDB client handle is null\r\n"))
            );

    i4_ret = handle_get_type_obj(h_edb_client, &e_type, (VOID**) (ppt_edb_client));
    EDB_CHECK_RET( 
            (i4_ret != HR_OK ||e_type != EDBT_EDB_CLIENT),
            (EDBR_INTERNAL_ERROR),
            (("Invalid handle\r\n"))
            );

    return i4_ret;
}

INT32  edb_handle_get_edb_obj(HANDLE_T h_edb,EDB_T** ppt_edb)
{
    INT32                   i4_ret = EDBR_OK;
    HANDLE_TYPE_T           e_type;

    EDB_CHECK_RET( 
            (h_edb == NULL_HANDLE || ppt_edb == NULL),
            (EDBR_INV_HANDLE),
            (("EDB  handle is null\r\n"))
            );

    i4_ret = handle_get_type_obj(h_edb, &e_type, (VOID**) (ppt_edb));
    EDB_CHECK_RET( 
            (i4_ret != HR_OK ||e_type != EDBT_EDB),
            (EDBR_INTERNAL_ERROR),
            (("Invalid handle\r\n"))
            );

    return i4_ret;
}


INT32  edb_handle_get_edb_obj_by_ctrl(HANDLE_T h_edb_eng,EDB_T** ppt_edb)
{
    INT32                   i4_ret = EDBR_OK;
    EDB_ENG_CTRL_ENTRY_T*   pt_eng_ctrl_entry = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE || ppt_edb == NULL),
            (EDBR_INV_HANDLE),
            (("EDB client handle is null\r\n"))
            );


    i4_ret = edb_handle_get_ctrl_obj(h_edb_eng, &pt_eng_ctrl_entry);
    EDB_CHECK_RET( 
            (pt_eng_ctrl_entry == NULL || pt_eng_ctrl_entry->pt_edb == NULL),
            (i4_ret),
            ((_EDB_INFO"EDB ctrl obj is null\r\n"))
            );

    *ppt_edb = pt_eng_ctrl_entry->pt_edb;

    return i4_ret;
}



INT32 edb_handle_get_ctrl_obj(
        HANDLE_T                 h_edb_eng,
        EDB_ENG_CTRL_ENTRY_T**   ppt_eng_ctrl_entry
        )
{
    INT32                   i4_ret = EDBR_OK;
    HANDLE_TYPE_T           e_type;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE || ppt_eng_ctrl_entry == NULL),
            (EDBR_INV_HANDLE),
            (("h_edb_eng  is null\r\n"))
            );

    i4_ret = handle_get_type_obj(h_edb_eng, &e_type, (VOID**) (ppt_eng_ctrl_entry));
    EDB_CHECK_RET( 
            (i4_ret != HR_OK ||e_type != EDBT_EDB_ENG),
            (EDBR_INTERNAL_ERROR),
            (("Invalid handle\r\n"))
            );

    return i4_ret;
}



typedef struct _NOTIFY_DATA_T
{
    EDB_NFY_REASON_T        e_reason;
    UINT32                  ui4_data1;
    UINT32                  ui4_data2;
}NOTIFY_DATA_T;

static INT32 _edb_cli_iterator_edb_client_hash(
        HANDLE_T          h_hash,
        UINT32            ui4_counter,
        VOID*             pv_obj,
        VOID*             pv_parse_tag )
{
    EDB_REQUEST_BASE_CTRL_T*    pt_edb_client   = NULL; 
    NOTIFY_DATA_T*              pt_notify_data = NULL;

    EDB_CHECK_RET( 
            (pv_obj == NULL || pv_parse_tag == NULL),
            (EDBR_INTERNAL_ERROR),
            (("Arguments is wrong\r\n"))
            );

    pt_edb_client = (EDB_REQUEST_BASE_CTRL_T*)pv_obj;
    pt_notify_data= (NOTIFY_DATA_T*)pv_parse_tag;
    /*x_edb_nfy_fct*/
    if (pt_edb_client->pf_nfy != NULL)
    {
        pt_edb_client->pf_nfy(
                pt_edb_client->h_edb_client,
                pt_notify_data->e_reason,
                pt_edb_client->pv_nfy_tag,
                pt_notify_data->ui4_data1,
                pt_notify_data->ui4_data2
                );
        DBG_INFO((_EDB_INFO"Notify caller reason=[%d]\r\n",
                  pt_notify_data->e_reason));
    }
    return EDBR_OK;
}

VOID edb_handle_notify(
        EDB_NFY_REASON_T        e_reason,
        //    VOID*                   pv_nfy_tag,
        UINT32                  ui4_data1,
        UINT32                  ui4_data2)
{
    NOTIFY_DATA_T   t_notify_data = {0};

    t_notify_data.e_reason = e_reason;
    t_notify_data.ui4_data1 = ui4_data1;
    t_notify_data.ui4_data2 = ui4_data2;


    if (g_edb_client_hash != NULL_HANDLE)
    {
        edb_hash_parse(g_edb_client_hash,_edb_cli_iterator_edb_client_hash,&t_notify_data);
    }

}



