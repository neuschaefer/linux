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
 * $RCSfile: _sm_rcrd_hdlr_pat_pmt.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Lianming Lin $
 * $MD5HEX: 9651a12385e0d96d6be213500a0475f8 $
 *
 * Description: This file handle message come from table Manager moduel 
 *              within record stream handler 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"

#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_pat_pmt.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_pvr.h"

#include "tbl_mngr/x_tm.h"
#include "tbl_mngr/psi_eng/x_psi_eng.h"
#include "util/x_crc32.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_RCRD_TBL_ROOT_PAT_STRM_NAME               "record pat stream"
#define SM_RCRD_TBL_ROOT_PMT_STRM_NAME               "record pmt stream"

/*PAT:4(id, setc len,) + 4(ts_id, ver, ...) * PMT(4) + CRC(4)*/
#define SM_RCRD_PAT_SECTION_LEN             16 
#define SM_RCRD_SECTION_LEN                 1024 
#define SM_RCRD_PMT_SECTION_LEN             SM_RCRD_SECTION_LEN 


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
static VOID _sm_dbg_dump_tbl_info(const UINT8* pui1_buff, SIZE_T z_size)
{
    UINT32 ui4_i = 0;
    
    for (ui4_i = 0; ui4_i < z_size; ++ui4_i)
    {
        SM_DBG_INFO(SM_DBG_MOD_RSS, ("%.2X ", pui1_buff[ui4_i]));
    }
}


INT32 _sm_rcrd_strm_free_pvr_swap_info(PVR_SWAP_INFO_T*         pt_swap_info)
{
    PVR_SWAP_SECT_LIST_T*      pt_sect_list = NULL;
    
    if (NULL == pt_swap_info)
    {
        return SMR_INV_ARG;
    }

    pt_sect_list = &(pt_swap_info->u.t_sect_list);
    if (pt_sect_list->at_sects != NULL)
    {   
        while (pt_sect_list->ui2_num_sect != 0)
        {
            if (pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data != NULL)
            {
                x_mem_free(pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data);
                pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data = NULL;
            }
            --(pt_sect_list->ui2_num_sect);
            
        }

        x_mem_free(pt_sect_list->at_sects);
        pt_sect_list->at_sects = NULL;
    }
    
    return SMR_OK;
    
}
/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_generate_pat
 *
 * Description: This function generates a new PAT.Generate a new PAT section 
 *              accoring to the PMT syntax in ISO-13818-1
 *
 * Inputs:  
 *       pt_rs_obj :      record stream object
 *       pt_pid_svc_info: PMT service info
 *       
 * Outputs: 
 *       pt_pat_info: pat section info
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
INT32 _sm_rcrd_strm_generate_pat(SM_RCRD_STREAM_T*        pt_rs_obj, 
                                 PSI_PID_SVC_ID_INFO_T*   pt_pid_svc_info,
                                 PVR_SWAP_INFO_T*         pt_pat_info)
{
    INT32                      i4_ret;
    UINT8*                     pui1_section;
    UINT32                     ui4_crc;
    UINT16                     ui2_ts_id;
    TM_COND_T                  e_cond;
    SIZE_T                     t_len;
    UINT8                      ui1_version;
    UINT16                     ui2_pmt_pid;
    PVR_SWAP_SECT_LIST_T*      pt_sect_list = NULL;
    UINT8*                     pui1_sect_buff = NULL;
    SIZE_T                     z_len_sect_buf;
    TM_COND_T                  e_cond_pmt;
    
    i4_ret = SMR_OK;
    
    /*1.Check arguments and Assert PAT is availabl or update*/
    if ((NULL == pt_rs_obj) 
        || (NULL == pt_pid_svc_info) 
        || (NULL == pt_pat_info))
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pat:NULL arguments\r\n"));
        return SMR_INV_ARG;
    }

    /*2. Get raw PAT for debug*/
    pui1_sect_buff = (UINT8*)x_mem_alloc(SM_RCRD_SECTION_LEN);
    if (NULL == pui1_sect_buff)
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pat:fail to allocate mem\r\n"));
        return SMR_INSUFFICIENT_MEMORY;
    }
    x_memset(pui1_sect_buff, 0, SM_RCRD_SECTION_LEN);
    z_len_sect_buf = (SIZE_T)SM_RCRD_SECTION_LEN;

    /*Get raw PAT data and Print it*/
    i4_ret = x_psi_get_raw_section(pt_rs_obj->h_pat,
                                   0,
                                   pui1_sect_buff,
                                   &z_len_sect_buf,
                                   &e_cond_pmt);
    
    if (TMR_OK == i4_ret)
    {
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pat:The raw PAT is as follow:>>>>>>>>>>>>"
                         ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n"));
        _sm_dbg_dump_tbl_info(pui1_sect_buff, z_len_sect_buf);
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pat:The raw PAT is as above:<<<<<<<<<<<<<"
                         "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\r\n"));
    }
    else
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pat:fail to get raw section\r\n"));
    }
    x_mem_free(pui1_sect_buff);
    pui1_sect_buff = NULL;
   
    /*3.Get ts id*/
    i4_ret = x_psi_get_ts_id(pt_rs_obj->h_pat, &ui2_ts_id, &e_cond);
    if (i4_ret != TMR_OK)
    {
        /* dump error code and do error handling */
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pat:get ts id err=%d\r\n", 
                        i4_ret));
        return SMR_INTERNAL_ERROR;
    }

    /*4.Get version*/
    t_len = sizeof(UINT8);
    i4_ret = x_tm_get_obj(pt_rs_obj->h_pat,
                          TM_GET_TYPE_VERSION,
                          0,
                          &ui1_version,
                          &t_len,
                          &e_cond);
    if (i4_ret != TMR_OK)
    {
        /* dump error code and do error handling */
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pat:get pat ver ret=%d\r\n", 
                        i4_ret));
        return SMR_INTERNAL_ERROR;
    }
  
    /* 5.allocate memory for an empty PAT section only one pmt*/
    pui1_section = (UINT8 *) x_mem_alloc(SM_RCRD_PAT_SECTION_LEN);
    if (NULL == pui1_section)
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pat:alloc mem failed\r\n"));
        return SMR_INSUFFICIENT_MEMORY;
    }
    x_memset(pui1_section, 0, SM_RCRD_PAT_SECTION_LEN);

    /* 6. fill in section */
    /*6.1 table id*/
    *pui1_section = 0x00;   
    /*6.2 section sytax indicator, 0, reserverd*/
    *(pui1_section + 1) = 0xb0;
    /*6.3 section length: ui2_sect_len - 3*/
    *(pui1_section + 2) = 0x0d;
    PUT_UINT16_TO_PTR_BIG_END(ui2_ts_id, (UINT16*)(pui1_section + 3));

    /*6.4 version*/
    *(pui1_section + 5) = ((ui1_version & 0x1f) << 1) | 0xc1;
    /*only one section: section num = max section num = 0*/
    *(pui1_section + 6) = 0;
    *(pui1_section + 7) = 0;
  
    /*6.5 PMT info, should add Network info???*/
    PUT_UINT16_TO_PTR_BIG_END(pt_rs_obj->ui2_program_num, (UINT16*)(pui1_section + 8));
    ui2_pmt_pid = (UINT16)(0xE000) | ((UINT16)(pt_pid_svc_info->t_pid));
    PUT_UINT16_TO_PTR_BIG_END(ui2_pmt_pid, (UINT16*)(pui1_section + 10));
    
    /*6.6 recalculate CRC for this section */
    ui4_crc = x_crc32_gen(pui1_section,
                          SM_RCRD_PAT_SECTION_LEN - 4,
                          0xffffffff);
    PUT_UINT32_TO_PTR_BIG_END(ui4_crc, (pui1_section + SM_RCRD_PAT_SECTION_LEN - 4));

    SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pat: \r\n"
                     "ui2_ts_id   = %u  \r\n"
                     "ui1_version = %u  \r\n"
                     "ui2_pmt_pid = %u\r\n",
                     ui2_ts_id,
                     ui1_version,
                     ui2_pmt_pid));

    pt_pat_info->e_swap_type = PVR_SWAP_TYPE_SECT_LIST;
    pt_sect_list = &(pt_pat_info->u.t_sect_list);
    pt_sect_list->ui2_num_sect = 1;
    pt_sect_list->at_sects = (PVR_SECT_DATA_T*)x_mem_alloc(sizeof(PVR_SECT_DATA_T));
    if (NULL == pt_sect_list->at_sects)
    {
        x_mem_free(pui1_section);
        pui1_section = NULL;
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pat:fail to alloc memory!!!\r\n"));
        return SMR_INTERNAL_ERROR;
    }
    pt_sect_list->at_sects[0].z_sect_len = SM_RCRD_PAT_SECTION_LEN;
    pt_sect_list->at_sects[0].pv_sect_data = (VOID*)pui1_section;

    SM_RSH_DBG_INFO(("_sm_rcrd_strm_generate_pat:generate PAT OK!!!\r\n"));

    /*Print New PAT info*/
    SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pat:The new PAT is as follow:>>>>>>>>>>>>>>"
                     ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n"));
    _sm_dbg_dump_tbl_info(pui1_section, (SIZE_T)SM_RCRD_PAT_SECTION_LEN);
    SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pat:The new PAT is as above:<<<<<<<<<<<<<<<<"
                     "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\r\n"));
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_swap_pat
 *
 * Description: This function generates a new PAT and swaps the old one 
 *
 * Inputs:  
 *       pt_rs_obj : record stream object
 *       
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_swap_pat(SM_RCRD_STREAM_T*    pt_rs_obj)
{
    INT32                      i4_ret;

    PSI_PID_SVC_ID_INFO_T      t_pid_svc_info;
    BOOL                       b_pmt_entry_found;
    TM_COND_T                  e_cond;
    SIZE_T                     z_len;
    UINT16                     ui2_num_entries;
    UINT16                     ui2_idx;
    PVR_SWAP_INFO_T            t_pat_info;

    b_pmt_entry_found = FALSE;
    
    /*1. Get PMT-PID by Program-Number(ui2_prg_num) */
    if ((NULL == pt_rs_obj) 
        || (NULL_HANDLE == pt_rs_obj->h_pat)
        || ((pt_rs_obj->e_pat_cond != TM_COND_AVAILABLE)
             && (pt_rs_obj->e_pat_cond != TM_COND_UPDATE)))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_swap_pat:not allow\r\n"));
        return SMR_NOT_ALLOW;
    }

    z_len = sizeof(UINT16);
    i4_ret = x_tm_get_obj(pt_rs_obj->h_pat,
                          TM_GET_TYPE_NUM_FIRST_LOOP_ENTRIES,
                          NULL,
                          (VOID*)&ui2_num_entries,
                          &z_len,
                          &e_cond);
    
    if (TMR_OK == i4_ret)
    {
        for (ui2_idx = 0; ui2_idx < ui2_num_entries; ++ui2_idx)
        {
            i4_ret = x_psi_get_svc_entry(pt_rs_obj->h_pat,
                                         ui2_idx ,
                                         &t_pid_svc_info,
                                         &e_cond);
            if (TMR_OK == i4_ret)
            {
                if (t_pid_svc_info.ui2_svc_id == pt_rs_obj->ui2_program_num)
                {
                    b_pmt_entry_found = TRUE;
                    break;
                }
            }
            else if (TMR_INV_HANDLE_STATE == i4_ret)
            {
                   /* The current PAT is not in TM_COND_AVAILABLE or TM_COND_UPDATE state */
                   /* break here and wait for PAT's notification about 
                      TM_COND_AVAILABLE or TM_COND_UPDATE state */
                   SM_RSH_DBG_INFO(("sm_rcrd_strm_swap_pat:pat is not available or update\r\n"));
                   return SMR_OK;
            }
            else
            {
                /* dump error code and do error handling */
                SM_RSH_DBG_ERR(("sm_rcrd_strm_swap_pat:get service error=%d\r\n", i4_ret));
                return SMR_INTERNAL_ERROR;
            }
        }
    }
    else if (TMR_INV_HANDLE_STATE == i4_ret)
    {
        /* The current PAT is not in TM_COND_AVAILABLE or TM_COND_UPDATE state */
        /* break here and wait for PAT's notification about TM_COND_AVAILABLE 
           or TM_COND_UPDATE state */
            SM_RSH_DBG_INFO(("sm_rcrd_strm_swap_pat:pa may be not available or update\r\n"));
           return SMR_OK;
    }
    else
    {
        /* dump error code and do error handling */
        SM_RSH_DBG_ERR(("sm_rcrd_strm_swap_pat:get service num err=%d\r\n", i4_ret));
        return SMR_INTERNAL_ERROR;
    }


    /* 2.
       Generate a new PAT section accoring to t_pid_svc_info
       Please refer to the PAT syntax in ISO-13818-1 
    */
    if (TRUE == b_pmt_entry_found)
    {
        /*Generate PAT table how to use t_pid_svc_info*/
        x_memset(&t_pat_info, 0, sizeof(PVR_SWAP_INFO_T));
        i4_ret = _sm_rcrd_strm_generate_pat(pt_rs_obj, &t_pid_svc_info, &t_pat_info);
        if (SMR_OK == i4_ret)
        {
            /*4.Set command to PVR STRM*/
            i4_ret =  sm_rcrd_strm_pvr_set(pt_rs_obj->t_base.h_stream,
                                           PVR_SET_TYPE_SWAP,
                                           (VOID*)&t_pat_info,
                                           (SIZE_T)sizeof(PVR_SWAP_INFO_T));
            if (SMR_OK == i4_ret)
            {
                sm_rs_send_msg_do_event(SM_EVN_RSH_TM_PAT_SWAPED,
                                        pt_rs_obj->t_base.h_stream,
                                        0);
            }
        }

        /*free memory*/
        _sm_rcrd_strm_free_pvr_swap_info(&t_pat_info);
        return i4_ret;
    }
    else
    {
        return SMR_INV_SET_INFO;
    }
}

/* Notify function. */
NFY_RET_T _sm_load_pat_nfy(HANDLE_T       h_obj,
                           HANDLE_TYPE_T  e_obj_type,
                           TM_COND_T      e_nfy_cond,
                           VOID*          pv_nfy_tag,
                           UINT32         ui4_data)
{
    SM_RCRD_STRM_HDLR_MSG_T     t_msg;
    
    if (NULL == pv_nfy_tag)
    {
        return NFY_RET_INVALID;
    }
    
    t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_TM; 
    t_msg.h_stream   = (HANDLE_T)pv_nfy_tag;
    
    t_msg.u.t_tm_msg.e_nfy_cond = e_nfy_cond;
    t_msg.u.t_tm_msg.ui4_data   = ui4_data;
    t_msg.u.t_tm_msg.ui4_table  = SM_RCRD_TM_PAT;
    t_msg.u.t_tm_msg.h_table    = h_obj;
    sm_rs_send_msg(&t_msg);

    return NFY_RET_PROCESSED;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_load_pat
 *
 * Description: This function loads pat and generate a new PAT and swap the old one 
 *
 * Inputs:  
 *       pt_rs_obj : record stream object
 *       b_update:   If update pat
 *       
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_load_pat(SM_RCRD_STREAM_T*      pt_rs_obj)
{
    INT32                      i4_ret;
    CHAR                       aui1_root_name[SM_RCRD_UTIL_NAME_LEN] = {0};
    SM_RCRD_STRM_HDLR_MSG_T    t_msg;
    TM_COND_T                  e_nfy_cond;
    
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pat:NULL arguments\r\n"));
        return SMR_INV_ARG;
    }

    if (TRUE == pt_rs_obj->b_pat_loaded)
    {
        SM_RSH_DBG_INFO(("sm_rcrd_strm_load_pat:PAT has been loaded......\r\n"));
        SM_ABORT(SM_DBG_ABRT_INV_CASE_STMT);
        return SMR_OK;
    }

    i4_ret = SMR_OK;
    
    /*1.Open a tm root */
    if (NULL_HANDLE == pt_rs_obj->h_tm_root)
    {
        if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
             && (RSH_TBL_GRP_PSI_PAT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            x_sprintf(aui1_root_name, "sm_pat%x", pt_rs_obj->t_base.h_stream);
        }
        else if ((RSH_RCRD_TYPE_SECT == pt_rs_obj->t_rcrd_type)
                  && (RSH_TBL_GRP_PSI_PMT == pt_rs_obj->t_sect_info.e_tbl_grp))
        {
            x_sprintf(aui1_root_name, "sm_pmt%x", pt_rs_obj->t_base.h_stream);
        }
        else
        {
            SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pat:not support\r\n"));
            return SMR_NOT_SUPPORT;
        }
        
        i4_ret = x_tm_open_root(aui1_root_name,  
                                TM_SRC_TYPE_CONN_HANDLE,
                                (VOID*)pt_rs_obj->t_base.h_connection,
                                NULL,
                                NULL,
                                &(pt_rs_obj->h_tm_root),
                                NULL);
    }
    /*Check i4_ret */                                 
    if ((TMR_OK == i4_ret) && (pt_rs_obj->h_tm_root != NULL_HANDLE))
    {
        /*2. load PAT */
        i4_ret = x_psi_load_pat(pt_rs_obj->h_tm_root,
                                TM_SRC_TYPE_SAME_AS_ROOT,
                                NULL,
                                (VOID*)(pt_rs_obj->t_base.h_stream), 
                                _sm_load_pat_nfy,
                                &(pt_rs_obj->h_pat),
                                /*&(pt_rs_obj->e_pat_cond));*/
                                 &e_nfy_cond);
    
        /* check i4_ret */                                 
        if (TMR_OK == i4_ret)
        {
            pt_rs_obj->b_pat_loaded = TRUE;

            t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_TM; 
            t_msg.h_stream   = (HANDLE_T)pt_rs_obj->t_base.h_stream;
            
            t_msg.u.t_tm_msg.e_nfy_cond = e_nfy_cond;
            t_msg.u.t_tm_msg.ui4_data   = 0;
            t_msg.u.t_tm_msg.ui4_table  = SM_RCRD_TM_PAT;
            t_msg.u.t_tm_msg.h_table    = pt_rs_obj->h_pat;
            sm_rs_send_msg(&t_msg);
            
            return i4_ret;
        }
        else
        {
            /* Dump error code and do error handling */
            SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pat:load pat err=%d\r\n", i4_ret));
            if (pt_rs_obj->h_tm_root != NULL_HANDLE)
            {
                i4_ret = sm_rcrd_strm_free_tm_obj(pt_rs_obj->h_tm_root);
                if (i4_ret != SMR_OK)
                {
                    SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pat:PAT(0x%.8X) error=%d!\r\n",
                                    (UINT32)pt_rs_obj->h_tm_root,
                                    i4_ret));
                }
                pt_rs_obj->h_tm_root = NULL_HANDLE;
            }
            return SMR_INTERNAL_ERROR;
        }
    }
    else
    {
        /* Dump error code and do error handling */
        SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pat:open root err=%d, root=%d\r\n", 
                        i4_ret,
                        pt_rs_obj->h_tm_root));
        return SMR_INTERNAL_ERROR;
    }
}

BOOL _sm_rcrd_strm_is_in_es_list(const MPEG_2_PID_T*  at_pids_es,
                                 UINT16               ui2_num_es,
                                 MPEG_2_PID_T         t_pid)
{
    UINT32      ui4_idx;
    
    if (NULL == at_pids_es)
    {
        return FALSE;
    }

    for (ui4_idx = 0; ui4_idx < ui2_num_es; ++ui4_idx)
    {
        if (t_pid == at_pids_es[ui4_idx])
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_generate_pmt
 *
 * Description: This function generates a new PMT.Generate a new PMT section 
 *              is accoring to the raw data in pui1_pmt_buff. New section   
 *              refers to the PMT syntax in ISO-13818-1
 *
 * Inputs:  
 *       pt_rs_obj : record stream object
 *       
 * Outputs: 
 *       pt_pmt_info: PMT section
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
INT32 _sm_rcrd_strm_generate_pmt(SM_RCRD_STREAM_T*   pt_rs_obj, 
                                 PVR_SWAP_INFO_T*    pt_pmt_info)
{
    INT32                      i4_ret;
    TM_COND_T                  e_cond_pmt;
    SIZE_T                     z_len_sect_buf;
    UINT8*                     pui1_sect_buff   = NULL;
    UINT32                     ui4_sec_num;
    UINT16                     ui2_program_info_len;
    UINT16                     ui2_es_desc_len;
    UINT32                     ui4_rest_es_info_len;
    UINT32                     ui4_rmv_es_info_len;
    UINT32                     ui4_each_es_info_len;
    UINT16                     ui2_sect_len;  /*do not include size before sec_len field in PMT*/
    UINT8*                     pui1_es_info     = NULL;
    MPEG_2_PID_T               t_strm_pid;
    UINT32                     ui4_crc;
    PVR_SWAP_SECT_LIST_T*      pt_sect_list     = NULL;
    PVR_SECT_DATA_T*           pt_sects         = NULL;
    
    /*Check arguments*/
    if ((NULL == pt_rs_obj) 
        || ((pt_rs_obj->e_pmt_cond != TM_COND_AVAILABLE)
             && (pt_rs_obj->e_pmt_cond != TM_COND_UPDATE))
        || (NULL == pt_pmt_info))
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pmt:invalid arguments\r\n"));
        return SMR_INV_ARG;
    }
    
    /*allocate section buffer*/
    pui1_sect_buff = (UINT8*)x_mem_alloc(SM_RCRD_PMT_SECTION_LEN);
    if (NULL == pui1_sect_buff)
    {
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pmt:fail to allocate mem\r\n"));
        return SMR_INSUFFICIENT_MEMORY;
    }
    x_memset(pui1_sect_buff, 0, SM_RCRD_PMT_SECTION_LEN);
    
    z_len_sect_buf = (SIZE_T)SM_RCRD_PMT_SECTION_LEN;
    ui4_sec_num = 0;
    x_memset(pt_pmt_info, 0, sizeof(PVR_SWAP_INFO_T));
    i4_ret = SMR_OK;
    
    while (1)
    {
        
        /*1 get raw section, if not get break*/
        i4_ret = x_psi_get_raw_section(pt_rs_obj->h_pmt,
                                       (UINT8)(ui4_sec_num)++,
                                       pui1_sect_buff,
                                       &z_len_sect_buf,
                                       &e_cond_pmt);
        
        if (TMR_OK == i4_ret)
        {
            /*last section singal*/
            if (0 == z_len_sect_buf)
            {
                i4_ret = SMR_OK;
                break;
            }
        
        }
        else
        {
            if (0 == z_len_sect_buf)
            {
                /*last section singal*/
                i4_ret = SMR_OK;
                break;
            }
            
            SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pmt:fail to get raw section\r\n"));
            i4_ret = SMR_INTERNAL_ERROR;
            break;
        }
        
        /*2.Need do according to ES list,remove stream which is not in ES list*/
        /*get to stream loop positin in section: 10 bytes before program_info_len 13818-1*/
        ui2_sect_len = GET_UINT16_FROM_PTR_BIG_END(pui1_sect_buff + 1);
        ui2_sect_len &= 0x03ff;

        ui2_program_info_len = GET_UINT16_FROM_PTR_BIG_END(pui1_sect_buff + 10);
        ui2_program_info_len &= 0x03ff;

        if(12 + ui2_program_info_len < 1023)
        {
            pui1_es_info = pui1_sect_buff + 12 + ui2_program_info_len;
        }
        else
        {
            return SMR_INTERNAL_ERROR;
        }
        /*9 means data size between sec_len and program_description,
          4 means CRC size*/
        ui4_rest_es_info_len = (UINT32)(ui2_sect_len - 9 - ui2_program_info_len - 4);
        ui4_rmv_es_info_len = 0;

        /*Print old PMT in terminal*/
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pmt:The RAW PMT section(%d)"
                         " is as follow:>>>>>>>>\r\n",
                         ui4_sec_num - 1));
        _sm_dbg_dump_tbl_info(pui1_sect_buff, (SIZE_T)(ui2_sect_len + 3));
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pmt:The raw PMT section(%d) "
                         "is as above:<<<<<<<<<\r\n",
                         ui4_sec_num - 1));
        
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pmt:\r\n"
                         "raw ui2_program_info_len = %u\r\n"
                         "raw ui2_sect_len         = %u\r\n",
                         ui2_program_info_len,
                         ui2_sect_len));
        
        while (ui4_rest_es_info_len != 0)
        {
            /*2.1 get stream type/pid, es_desc_info_length*/
            /*ui1_strm_type = *pui1_es_info;*/
            t_strm_pid = GET_UINT16_FROM_PTR_BIG_END(pui1_es_info + 1);
            t_strm_pid = t_strm_pid & 0x1fff;
            
            ui2_es_desc_len =  GET_UINT16_FROM_PTR_BIG_END(pui1_es_info + 3);
            ui2_es_desc_len &= 0x03ff;

            /*get a stream info length including type,pid,descriptor,other such as reserved*/
            ui4_each_es_info_len = (UINT32)(1 + 2 + 2 + ui2_es_desc_len);
            if (ui4_rest_es_info_len > ui4_each_es_info_len)
            {
                ui4_rest_es_info_len -= ui4_each_es_info_len;
            }
            else
            {
                ui4_rest_es_info_len = 0;
            }
            SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pmt:\r\n"
                             "t_strm_pid           = %u\r\n"
                             "ui2_es_desc_len      = %u\r\n"
                             "ui2_rest_es_info_len = %d\r\n",
                             t_strm_pid,
                             ui2_es_desc_len,
                             ui4_rest_es_info_len));

            
            /*2.2 if not in ES list remove*/
            if (!_sm_rcrd_strm_is_in_es_list(pt_rs_obj->t_pmt_info.at_pids_es,
                                             pt_rs_obj->t_pmt_info.ui2_num_es,
                                             t_strm_pid))
            {
                /*memory move:not more than (Max size 0x3fd)1021 - 
                 (before stream info len)9 -(CRC)4 - (at least this stream)5*/
                if (ui4_rest_es_info_len <= 1003)
                {
                    x_memmove(pui1_es_info, 
                              pui1_es_info + ui4_each_es_info_len,
                              ui4_rest_es_info_len);
                }
                else
                {
                    SM_RSH_DBG_ERR(("\r\n_sm_rcrd_strm_generate_pmt:\r\n"
                                    "t_strm_pid           = %u \r\n"
                                    "ui2_es_desc_len      = %u \r\n"
                                    "ui2_rest_es_info_len = %u!!!\r\n",
                                    t_strm_pid,
                                    ui2_es_desc_len,
                                    ui4_rest_es_info_len));
                    i4_ret = SMR_INTERNAL_ERROR;
                    break;
                }
                /*adjust section length*/
                ui4_rmv_es_info_len += ui4_each_es_info_len;
            }
            /*2.3 get to next stream*/
            else
            {
                pui1_es_info += ui4_each_es_info_len;
            }
        }
        
        /*3.adjust section length*/
        ui2_sect_len -= ui4_rmv_es_info_len;
        PUT_UINT16_TO_PTR_BIG_END(((ui2_sect_len & 0x3ff) | 0xb000), pui1_sect_buff + 1);
        
        /*4.recalculate CRC for this section, 3 indicate data size before sec_len ,4 is CRC length*/
        ui4_crc = x_crc32_gen(pui1_sect_buff,
                              (SIZE_T)(ui2_sect_len + 3 - 4),
                              0xffffffff);
        PUT_UINT32_TO_PTR_BIG_END(ui4_crc, pui1_sect_buff + ui2_sect_len + 3 - 4);

        /*5.output new section*/
        /*Need check section if comply with 13818-1? default NULL*/
        pt_pmt_info->e_swap_type = PVR_SWAP_TYPE_SECT_LIST;
        pt_sect_list = &(pt_pmt_info->u.t_sect_list);
        ++(pt_sect_list->ui2_num_sect);

        /*reallocate sects*/
        pt_sects = pt_sect_list->at_sects;
        pt_sect_list->at_sects = (PVR_SECT_DATA_T*)x_mem_alloc(pt_sect_list->ui2_num_sect
                                                               * sizeof(PVR_SECT_DATA_T));
        if (NULL == pt_sect_list->at_sects)
        {
            i4_ret = SMR_INTERNAL_ERROR;
            SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pmt:fail to alloc at_sects!!!\r\n"));
            break;
        }
        x_memset(pt_sect_list->at_sects,
                 0, 
                 pt_sect_list->ui2_num_sect * sizeof(PVR_SECT_DATA_T));

        /*Copy old info to new sects*/
        if (pt_sects != NULL)
        {
            x_memcpy(pt_sect_list->at_sects, 
                     pt_sects, 
                     (pt_sect_list->ui2_num_sect - 1) * sizeof(PVR_SECT_DATA_T));
            x_mem_free(pt_sects);
            pt_sects = NULL;
        }

        /*new section length, 3 indicate data size before sec_len*/
        pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].z_sect_len = 
                               (SIZE_T)(ui2_sect_len + 3);
        /*allocate section buffer*/
        pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data = 
                              x_mem_alloc((SIZE_T)(ui2_sect_len + 3));
        if (NULL == pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data)
        {
            i4_ret = SMR_INTERNAL_ERROR;
            SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pmt:fail to alloc pv_sect_data!!!\r\n"));
            break;
        }
        x_memcpy(pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data,
                 pui1_sect_buff,
                 (SIZE_T)(ui2_sect_len + 3));


        /*Print New PMT in terminal*/
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pmt:The new PMT section(%d) "
                         "is as follow:>>>>>>>>\r\n",
                         ui4_sec_num - 1));
        _sm_dbg_dump_tbl_info(pui1_sect_buff, (SIZE_T)(ui2_sect_len + 3));
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pmt:The  new PMT section(%d) "
                         " is as above:<<<<<<<<<\r\n",
                         ui4_sec_num - 1));
        SM_RSH_DBG_INFO(("\r\n_sm_rcrd_strm_generate_pmt:\r\n"
                         "ui2_rmv_es_info_len    = %u\r\n"
                         "new ui2_sect_len       = %u\r\n",
                         ui4_rmv_es_info_len,
                         ui2_sect_len));
        
    };

    /*free pui1_sect_buff*/
    if (pui1_sect_buff != NULL)
    {
        x_mem_free(pui1_sect_buff);
        pui1_sect_buff = NULL;
    }
    
    /*maybe fail,so need check*/
    if (pt_sects != NULL)
    {
        x_mem_free(pt_sects);
        pt_sects = NULL;
    }
    
    if (SMR_OK == i4_ret)
    {
        SM_RSH_DBG_INFO(("_sm_rcrd_strm_generate_pmt:generate PMT OK!!!\r\n"));
        return SMR_OK;
    }
    else
    {
        /*free all sections*/
        if ((pt_sect_list != NULL) && (pt_sect_list->at_sects != NULL))
        {
            while (pt_sect_list->ui2_num_sect != 0)
            {
                if (pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data != NULL)
                {
                    x_mem_free(pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data);
                    pt_sect_list->at_sects[pt_sect_list->ui2_num_sect - 1].pv_sect_data = NULL;
                    --(pt_sect_list->ui2_num_sect);
                }
                else
                {
                    SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pmt:pt_sect_list data is corrupted!!!\r\n"));
                }
                
            }
            x_mem_free(pt_sect_list->at_sects);
            pt_sect_list->at_sects = NULL;
        }
        
        SM_RSH_DBG_ERR(("_sm_rcrd_strm_generate_pmt:fail to generate PMT !!!\r\n"));
        return SMR_INTERNAL_ERROR;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_swap_pmt
 *
 * Description: This function generates a new PMT and swap the old one 
 *
 * Inputs:  
 *       pt_rs_obj : record stream object
 *       
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_swap_pmt(SM_RCRD_STREAM_T*    pt_rs_obj)
{

    INT32                      i4_ret;
    PVR_SWAP_INFO_T            t_pmt_info;
    PVR_BUFFER_STATUS_T        t_pvr_buf_stat;
    UINT32                     ui4_lba; 
    SIZE_T                     z_len;
    
    /*1.Check arguments*/
    if ((NULL == pt_rs_obj) 
        || (NULL_HANDLE == pt_rs_obj->h_pmt)
        || ((pt_rs_obj->e_pmt_cond != TM_COND_AVAILABLE)
             && (pt_rs_obj->e_pmt_cond != TM_COND_UPDATE)))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_swap_pmt:not allow \r\n"));
        return SMR_NOT_ALLOW;
    }
    
    /*2. Generate PMT table */
    x_memset(&t_pmt_info, 0, sizeof(PVR_SWAP_INFO_T));
    i4_ret = _sm_rcrd_strm_generate_pmt(pt_rs_obj, &t_pmt_info);
    if (SMR_OK == i4_ret)
    {
        /*3.Set command to PVR STRM*/
        i4_ret =  sm_rcrd_strm_pvr_set(pt_rs_obj->t_base.h_stream,
                                       PVR_SET_TYPE_SWAP,
                                       (VOID*)&t_pmt_info,
                                       (SIZE_T)sizeof(PVR_SWAP_INFO_T));
        
        if (SMR_OK == i4_ret)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_TM_PMT_SWAPED,
                                    pt_rs_obj->t_base.h_stream,
                                    0);
        }
    }

    /*free memory*/
    _sm_rcrd_strm_free_pvr_swap_info(&t_pmt_info);

    /*4.Update*/
    if (SMR_OK == i4_ret)
    {
        /*4.1 get buffer status*/
        z_len = (SIZE_T)sizeof(PVR_BUFFER_STATUS_T);
        i4_ret =  sm_rcrd_strm_pvr_get(pt_rs_obj->t_base.h_stream,
                                       PVR_GET_TYPE_BUFFER_STATUS,
                                       (VOID*)&t_pvr_buf_stat,
                                       &z_len);
        if (i4_ret != SMR_OK)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR,
                                    pt_rs_obj->t_base.h_stream,
                                    0);
            SM_RSH_DBG_ERR(("sm_rcrd_strm_swap_pmt:pvr get err=%d \r\n", i4_ret));
            return i4_ret;
        }
        /*4.2 Get lba*/
        i4_ret = rec_util_get_lba_by_pvr_buf_status(pt_rs_obj->t_rutil_info.h_record_util,
                                                    &t_pvr_buf_stat,
                                                    &ui4_lba);
        if (i4_ret != SMR_OK)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR,
                                    pt_rs_obj->t_base.h_stream,
                                    0);
            SM_RSH_DBG_ERR(("sm_rcrd_strm_swap_pmt:get buffer stat err=%d \r\n", i4_ret));
            return i4_ret;
        }
        /*4.3. update scdb in record utility*/
        if (pt_rs_obj->t_pmt_info.ui2_num_recs != 0)
        {
        i4_ret = rec_util_update_scdb_info(pt_rs_obj->t_rutil_info.h_record_util,
                                           ui4_lba,
                                           pt_rs_obj->t_pmt_info.ui2_num_recs,
                                           pt_rs_obj->t_pmt_info.at_scdb_recs);
        
        if (i4_ret != SMR_OK)
        {
            sm_rs_send_msg_do_event(SM_EVN_RSH_COMP_ERR,
                                    pt_rs_obj->t_base.h_stream,
                                    0);
            SM_RSH_DBG_ERR(("sm_rcrd_strm_swap_pmt:update scdb err=%d \r\n", i4_ret));
            return i4_ret;
            }
        }
        
        /*4.4 Send PMT swap event*/
        /*sm_rs_send_msg_do_event(SM_EVN_RSH_TM_PMT_SWAPED, 
                                pt_rs_obj->t_base.h_stream,
                                0);*/
    } 

    if (SMR_OK == i4_ret)
    {
        SM_RSH_DBG_INFO(("sm_rcrd_strm_swap_pmt:pmt swap OK!!!\r\n"));
        /*pt_rs_obj->b_es_list_change = FALSE;*/
    }
    
    return i4_ret;
}

/* Notify function. */
NFY_RET_T _sm_load_pmt_nfy(HANDLE_T       h_obj,
                           HANDLE_TYPE_T  e_obj_type,
                           TM_COND_T      e_nfy_cond,
                           VOID*          pv_nfy_tag,
                           UINT32         ui4_data)
{
    SM_RCRD_STRM_HDLR_MSG_T     t_msg;
    
    if (NULL == pv_nfy_tag)
    {
        return NFY_RET_INVALID;
    }
    
    t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_TM; 
    t_msg.h_stream = (HANDLE_T)pv_nfy_tag;
    
    t_msg.u.t_tm_msg.e_nfy_cond = e_nfy_cond;
    t_msg.u.t_tm_msg.ui4_data   = ui4_data;
    t_msg.u.t_tm_msg.ui4_table  = SM_RCRD_TM_PMT;
    t_msg.u.t_tm_msg.h_table    = h_obj;
    
    sm_rs_send_msg(&t_msg);

    return NFY_RET_PROCESSED;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_load_pmt
 *
 * Description: This function loads PMT and generate a new PMT and swap the old one 
 *
 * Inputs:  
 *       pt_rs_obj : record stream object
 *       
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-12 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_load_pmt(SM_RCRD_STREAM_T*       pt_rs_obj)
{
    INT32                      i4_ret;
    SM_RCRD_STRM_HDLR_MSG_T    t_msg;
    TM_COND_T                  e_nfy_cond;
    
    /*1.check arguments*/
    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pmt:invalid arguments\r\n"));
        return SMR_INV_ARG;
    }
    
    if (TRUE == pt_rs_obj->b_pmt_loaded)
    {
        SM_RSH_DBG_INFO(("sm_rcrd_strm_load_pmt:PAT has been loaded......\r\n"));
        return SMR_OK;
    }
    
    /*2.Load pat*/
    if ((pt_rs_obj->e_pat_cond != TM_COND_AVAILABLE)
        && (pt_rs_obj->e_pat_cond != TM_COND_UPDATE))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pmt:can't hanppen!\r\n"));
        return SMR_CAN_NOT_OP_IN_THIS_STATE;
    }

    /*3.load PMT (after PAT is TM_COND_AVAILABLE || TM_COND_UPDATE) */
    i4_ret = x_psi_load_pmt_by_svc_id(pt_rs_obj->h_pat,
                                      pt_rs_obj->ui2_program_num,
                                      TM_SRC_TYPE_SAME_AS_ROOT,
                                      NULL,
                                      (VOID*)(pt_rs_obj->t_base.h_stream),
                                      _sm_load_pmt_nfy,
                                      &(pt_rs_obj->h_pmt),
                                      /*&(pt_rs_obj->e_pmt_cond));*/
                                      &e_nfy_cond);
    /* check i4_ret */                                 
    if (TMR_OK == i4_ret)
    {
        pt_rs_obj->b_pmt_loaded = TRUE;
        
        t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_TM; 
        t_msg.h_stream   = (HANDLE_T)pt_rs_obj->t_base.h_stream;
        
        t_msg.u.t_tm_msg.e_nfy_cond = e_nfy_cond;
        t_msg.u.t_tm_msg.ui4_data   = 0;
        t_msg.u.t_tm_msg.ui4_table  = SM_RCRD_TM_PMT;
        t_msg.u.t_tm_msg.h_table    = pt_rs_obj->h_pmt;
        
        sm_rs_send_msg(&t_msg);
        
    }
    else
    {
        /* Dump error code and do error handling */
        SM_RSH_DBG_ERR(("sm_rcrd_strm_load_pmt:load service err=%d \r\n", i4_ret));
        return SMR_INTERNAL_ERROR;
    }

    return i4_ret;

}

/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_free_tm_obj
 *
 * Description: This function free PMT or PAT 
 *
 * Inputs:  
 *       h_obj : PAT or PMT handle
 *       
 * Outputs: NULL  
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-04-13 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_free_tm_obj(HANDLE_T  h_obj)
{
    INT32      i4_ret;

    if (!x_handle_valid(h_obj))
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_free_tm_obj:invalid handle!!!\r\n"));
        return SMR_INTERNAL_ERROR;
    }
    
    i4_ret = x_tm_free(h_obj);
    
    if (TMR_OK != i4_ret)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_free_tm_obj:free tm obj err %d\r\n",
                        i4_ret));
        return SMR_INTERNAL_ERROR;
    }
    else
    {
        return SMR_OK;
    }
}



