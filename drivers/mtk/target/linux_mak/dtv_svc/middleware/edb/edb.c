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
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"


#include "edb/edb_eng.h"
#include "edb/edb_slctr.h"
#include "edb/edb_pool.h"



VOID edb_delete_edb( VOID* pv_obj )
{
    EDB_T*                   pt_edb  = NULL;
    EDB_EVENT_POOL_T*        pt_pool = NULL;
    EDB_ENG_CTRL_ENTRY_T*    pt_eng_ctrl_entry = NULL;
    EDB_COMMAND_T*           pt_cmd       = NULL;
    UINT16                   ui2_idx      = 0;

    pt_cmd = EDB_OS_ALLOC(sizeof(EDB_COMMAND_T) * 3);
    if (pt_cmd == NULL)
    {
        return;
    }
    ui2_idx = 0;


    pt_cmd[ui2_idx].e_code      = EDB_CMD_DO_ENABLE;
    pt_cmd[ui2_idx].u.b_bool    = FALSE;
    ui2_idx++;

    DBG_INFO(("edb_edb_handle_free type:EDBT_EDB\r\n"));
    pt_edb = pv_obj;

    /*free pool memory*/
    pt_pool = &(pt_edb->t_edb_event_pool);
    if (pt_pool != NULL)
    {
        edb_pool_delete(pt_pool);
    }

    if (pt_edb == NULL)
    {
        if (pt_cmd != NULL) {EDB_OS_FREE(pt_cmd);}
        return; 
    }

    while (!DLIST_IS_EMPTY(&( pt_edb->t_eng_ctrl_list)))
    {
        pt_eng_ctrl_entry = DLIST_HEAD(&( pt_edb->t_eng_ctrl_list));

        if ( (!DLIST_IS_EMPTY( &( pt_edb->t_eng_ctrl_list))) && (pt_eng_ctrl_entry != NULL) )
        {
            DLIST_REMOVE(pt_eng_ctrl_entry, &( pt_edb->t_eng_ctrl_list), t_link);

            if (EDBR_OK != pt_eng_ctrl_entry->pt_eng->t_edb_fct_tbl.pf_set(
                            pt_eng_ctrl_entry->h_eng_ctrl,
                            pt_cmd))
            {
                DBG_INFO((_EDB_INFO"Stop engine fail\r\n"));
            }

            EDB_OS_FREE(pt_eng_ctrl_entry->pt_eng_data);
            pt_eng_ctrl_entry->pt_eng_data = NULL;

            EDB_OS_FREE(pt_eng_ctrl_entry);
        }
    }    

    if (pt_cmd != NULL) {EDB_OS_FREE(pt_cmd);}

   
    /*free edb object memory*/
    if (pt_edb != NULL)
    {
        EDB_OS_FREE(pt_edb);
    }

}


INT32 edb_dvb_get_eng_data_from_ctrl(HANDLE_T h_edb_eng,VOID** ppt_eng_data)
{
    INT32                       i4_ret = 0;
    EDB_ENG_CTRL_ENTRY_T*       pt_eng_ctrl_entry = NULL;

    EDB_CHECK_RET( 
            (h_edb_eng == NULL_HANDLE || ppt_eng_data == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Invalid argument\r\n"))
            );


    i4_ret = edb_handle_get_ctrl_obj(h_edb_eng,&pt_eng_ctrl_entry);
    EDB_CHECK_RET( 
            (pt_eng_ctrl_entry == NULL),
            (EDBR_INV_ARG),
            ((_EDB_INFO"Can not get edb client object\r\n"))
            );

    *ppt_eng_data = pt_eng_ctrl_entry->pt_eng_data;
    return i4_ret;
}



INT32 get_brdcst_type_svl_id_from_cmd (
        EDB_COMMAND_T*         pt_cmd,
        BRDCST_TYPE_T*         pe_brdcst_type,
        UINT16*                pui2_svl_id)
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
                *pui2_svl_id     = pt_eng_open_data->ui2_svl_id;
                break;
            default:
                break;
        }
        pt_cmd++;
    }

    return i4_ret;
}





