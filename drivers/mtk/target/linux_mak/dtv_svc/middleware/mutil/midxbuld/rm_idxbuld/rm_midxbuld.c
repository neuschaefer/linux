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
 * $RCSfile: rm_midxbuld.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media index builder public APIs  
 *---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/rm_idxbuld/rm_midxbuld.h"
#include "dbg/x_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"

#include "mutil/mm_common/rfc_ebml/rfc_ebml.h"
#include "mutil/mm_util_input.h"
#include "mutil/mm_util_memory.h"
#include "mutil/mm_util.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_midxbuld.h"
#include "rm_midxbuld.h"
#include "x_dbg.h"

#include "../../mm_util.h"


#include "x_lnk_list.h"
#include "../../mm_common/rfc_ebml/rfc_ebml.h"
#include "../../mm_util_input.h"
#include "../../mm_util_memory.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*define rm parsing buffer length*/
#define RM_IDXBULD_PARSER_BUFF_LEN       10240
#define RM_INDEX_RECORD_SIZE             14
#define RM_INDEX_HEADER_SIZE             20

#define dwBigFourCC(a,b,c,d)  ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
#define RM_BOX_INDX                      dwBigFourCC('I', 'N', 'D', 'X')
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*static UINT8        ui1_idx_range_id_last = 1;*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static BOOL _rm_midxbuld_alloc_range_id ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32*         pui4_range_id );
static BOOL _rm_midxbuld_free_range_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32          ui4_range_id );
static BOOL _rm_midxbuld_alloc_table_id( 
                                          RM_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                                          UINT32* pui4_tbl_id );
static BOOL _rm_midxbuld_free_table_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32          ui4_tbl_id);

static INT32 _rm_idx_set_pos(MIDXBULD_OBJ_T *pt_midxbuld_obj, 
                                 UINT64 ui8_file_offset, 
                                 UINT8 ui1_whence)
{
    setpos_fct      pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos; 
    MEDIA_FEEDER_T  t_feeder;
    /*feeder*/
    if(pt_midxbuld_obj->h_src_feeder != NULL_HANDLE)
    {
        t_feeder.h_feeder = pt_midxbuld_obj->h_src_feeder;
        return pf_set_pos(&t_feeder, (INT64)ui8_file_offset, ui1_whence);
    }
    /*file*/
    else
    {
        return pf_set_pos(pt_midxbuld_obj, (INT64)ui8_file_offset, ui1_whence);
    }
}
#if 0
static INT32 _rm_idx_get_pos(MIDXBULD_OBJ_T *pt_midxbuld_obj, 
                                 UINT64 *pui8_cur_pos)
{
    getpos_fct      pf_get_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos; 
    MEDIA_FEEDER_T  t_feeder;
    /*feeder*/
    if(pt_midxbuld_obj->h_src_feeder != NULL_HANDLE)
    {
        t_feeder.h_feeder = pt_midxbuld_obj->h_src_feeder;
        return pf_get_pos(&t_feeder, pui8_cur_pos);
    }
    /*file*/
    else
    {
        return pf_get_pos(pt_midxbuld_obj, pui8_cur_pos);
    }
}
#endif
static INT32 _rm_idx_copy_bytes(MIDXBULD_OBJ_T *pt_midxbuld_obj, 
                                    void* pv_buf, 
                                    SIZE_T z_buf_leng, 
                                    SIZE_T z_copy_size, 
                                    UINT32* pui4_size)
{
    copybytes_fct      pf_copybyte  = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes; 
    MEDIA_FEEDER_T     t_feeder;
    /*feeder*/
    if(pt_midxbuld_obj->h_src_feeder != NULL_HANDLE)
    {
        t_feeder.h_feeder = pt_midxbuld_obj->h_src_feeder;
        return pf_copybyte(&t_feeder, pv_buf, z_buf_leng, z_copy_size, pui4_size);
    }
    /*file*/
    else
    {
        return pf_copybyte(pt_midxbuld_obj, pv_buf, z_buf_leng, z_copy_size, pui4_size);
    }
}

static INT32 _rm_idxbuild_load_buf(MIDXBULD_OBJ_T *pt_midxbuld_obj,
                              UINT32 ui4_buf_offset,
                              UINT64 ui8_file_offset)
{
    UINT32 ui4_size = 0;
    UINT32 ui4_read_sz = 0;
    INT32  i4_ret = 0;
    RM_IDXBULD_OBJ_T* pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    if(ui4_buf_offset == 0)
    {
        ui4_size = pt_rm_handler->ui4_buf_length;
    }
    else 
    {
        ui4_size = ui4_buf_offset;
        x_memcpy(pt_rm_handler->pui1_header_buf, 
            pt_rm_handler->pui1_header_buf + ui4_buf_offset,
            pt_rm_handler->ui4_buf_length - ui4_buf_offset);
        ui4_buf_offset = pt_rm_handler->ui4_buf_length - ui4_buf_offset;
    }
    
    if((UINT64)ui4_size > pt_midxbuld_obj->z_buff_leng - ui8_file_offset)
    {
        ui4_size = (UINT32)(pt_midxbuld_obj->z_buff_leng - ui8_file_offset);
    }

    i4_ret = _rm_idx_set_pos(pt_midxbuld_obj, ui8_file_offset, FM_SEEK_BGN);
    i4_ret = _rm_idx_copy_bytes(pt_midxbuld_obj, 
                        pt_rm_handler->pui1_header_buf + ui4_buf_offset, 
                        (SIZE_T)pt_midxbuld_obj->z_buff_leng, 
                        ui4_size, 
                        &ui4_read_sz);
    if ((i4_ret != INPUTR_OK) && (ui4_read_sz < ui4_size) && (i4_ret != INPUTR_EOF))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    
    return (INT32)ui4_read_sz;
}

static RM_IDXBULD_ELMT_TBL_OBJ_T* _rm_idxbuild_create_tab(RM_IDXBULD_RANGE_OBJ_T* pt_range_obj)
{
    RM_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    
    pt_tbl_obj = (RM_IDXBULD_ELMT_TBL_OBJ_T*) 
                 _mm_util_mem_alloc_with_tag(sizeof(RM_IDXBULD_ELMT_TBL_OBJ_T),
                 MM_MEM_MIDX_TAG);
    if(pt_tbl_obj)
    {
        x_memset(pt_tbl_obj, 0, sizeof(RM_IDXBULD_ELMT_TBL_OBJ_T));
        if (TRUE != _rm_midxbuld_alloc_table_id(pt_range_obj, 
                                                &pt_tbl_obj->ui4_tbl_id))
        {
            _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
            pt_tbl_obj = NULL;
        }  
    }
    if (NULL == pt_tbl_obj)
    {
        DBG_ERROR(("rm_indx_parse: fail to alloc memory for pt_tbl_obj\r\n"));
    }
    return pt_tbl_obj;
}

/*--------------------------------------------------------------------------
 * Name:  _rm_midxbuld_alloc_table_id
 *
 * Description: This function allocate table id
 *
 * Inputs:  
 *          pt_range_obj ,  specifies range object
 *
 * Outputs: pui4_tbl_id , allocated table id 
 *
 * Returns: TRUE    Success. The routine was successful and the operation has
 *                  been completed.
 *          FALSE   Fail
 * Note:  
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/

static BOOL _rm_midxbuld_alloc_table_id( 
                                        RM_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                                        UINT32* pui4_tbl_id )
{

    /*Parameters check*/
    if (NULL == pt_range_obj)
    {
        DBG_ERROR(("_rm_midxbuld_alloc_table_id:NULL\r\n"));
        return FALSE;
    }
    
    *pui4_tbl_id = MIDXBULD_NULL_ID;

    /*not than 255*/
    if (pt_range_obj->ui1_tbl_total == 0xff)
    {
        DBG_ERROR(("_rm_midxbuld_alloc_table_id:not more than 255\r\n"));
        return FALSE;
    }

    /*the lowest byte for table id*/
    *pui4_tbl_id = (pt_range_obj->ui4_range_id & 0xffffff00) +
                   (++pt_range_obj->ui1_tbl_total);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_midxbuld_free_table_obj
 *
 * Description: This function free table id
 *
 * Inputs:  
 *          pt_midxbuld_obj ,   specifies index builder object
 *          ui4_tbl_id ,        specifies table id
 *
 * Outputs: NULL 
 *
 * Returns: TRUE    Success. The routine was successful and the operation has
 *                  been completed.
 *          FALSE   Fail
 * Note:  
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/

static BOOL _rm_midxbuld_free_table_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32          ui4_tbl_id)
{
    RM_IDXBULD_OBJ_T*          pt_rm_handler;
    RM_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    RM_IDXBULD_ELMT_TBL_OBJ_T* pt_last_obj;
    MIDXBULD_ELMT_TBL_OBJ_T*   pt_ele_tbl_obj   = NULL;
    UINT8                       ui1_free_range_id;
    UINT8                       ui1_range_id; 
    UINT8                       ui1_i;


    /*Parameters check*/
    if (NULL == pt_midxbuld_obj)
    {
        DBG_ERROR(("_rm_midxbuld_free_table_obj:NULL\r\n"));
        return FALSE;
    }

    /*get rm index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("_rm_midxbuld_free_table_obj:pt_rm_handler NULL\r\n"));
        return FALSE;
    }
    
    pt_obj = NULL;
    pt_last_obj = NULL;
    if (pt_rm_handler->ui1_range_count > RM_MIDXBULD_RANGE_MAX)
    {
        DBG_ERROR(("_rm_midxbuld_free_table_obj:error count!!!\r\n"));
        return FALSE;
    }
    
    /*search the range id by table id*/
    if (pt_rm_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == ui4_tbl_id)
    {
        pt_obj = pt_rm_handler->pt_key_range_obj->pt_tbl_obj;
        pt_rm_handler->pt_key_range_obj->ui1_tbl_total--;
        pt_rm_handler->pt_key_range_obj->pt_tbl_obj = pt_obj->pt_next;
    }
    else
    {   
        /*get range id by table id*/
        ui1_free_range_id = (UINT8)
                            ((ui4_tbl_id & RM_MIDXBULD_RANGE_ID_MASK) >> 8);
        
        for (ui1_i = 0; ui1_i < pt_rm_handler->ui1_range_count; ui1_i++)
        {
            ui1_range_id = (UINT8)
                            ((pt_rm_handler->apt_range_obj[ui1_i]->ui4_range_id 
                                  & RM_MIDXBULD_RANGE_ID_MASK) 
                              >> 8);
            /*whether it is the target range id?*/
            if (ui1_range_id == ui1_free_range_id)
            {
                BOOL b_id_match = FALSE;
            
                pt_obj = pt_rm_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
                
                /*pt_obj should be not NULL,if the given table is the first in a range*/
                if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_rm_handler->apt_range_obj[ui1_i]->pt_tbl_obj 
                        = pt_obj->pt_next;
                    --(pt_rm_handler->apt_range_obj[ui1_i]->ui1_tbl_total);
                }
                else
                {
                    pt_last_obj = pt_obj;
                    pt_obj = pt_obj->pt_next;
                }
                
                /*search the rest table nodes in given range*/
                while ((pt_obj != NULL ) && (FALSE == b_id_match))
                {
                    if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                    {
                        b_id_match = TRUE;
                        pt_last_obj->pt_next = pt_obj->pt_next;
                        --(pt_rm_handler->apt_range_obj[ui1_i]->ui1_tbl_total);
                    }
                    else
                    {
                        pt_last_obj = pt_obj;
                        pt_obj = pt_obj->pt_next;
                    }
                }
                
                break;
            }
        }
    }
    
    if (NULL == pt_obj)
    {
        DBG_INFO(("_rm_midxbuld_free_table_obj:not find table=%u\r\n",
                  ui4_tbl_id));
        return FALSE;
    }

    /*if found free related memory*/
    pt_ele_tbl_obj = &pt_obj->t_elmt_tbl;
    while(pt_ele_tbl_obj)
    {
        if(pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry)
        {
            _mm_util_mem_free_with_tag(
                pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry, 
                MM_MEM_MIDX_TAG);
            pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry = NULL;
        }
        pt_obj->t_elmt_tbl.pt_next = pt_ele_tbl_obj->pt_next;
        pt_ele_tbl_obj = pt_obj->t_elmt_tbl.pt_next;
    }
    _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
    
    DBG_INFO(("_rm_midxbuld_free_table_obj:free table=%u OK\r\n",
              ui4_tbl_id));
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_midxbuld_alloc_range_id
 *
 * Description: This function free index table resource, callback!
 *
 * Inputs:  
 *          pt_midxbuld_obj ,  specifies the index builder object
 *          
 * Outputs: pui4_range_id : allocated range id 
 *
 * Returns: TRUE    Success. The routine was successful and the operation has
 *                  been completed.
 *          FALSE   Fail
 * Note:  
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-27 : initial
 ----------------------------------------------------------------------------*/
static BOOL _rm_midxbuld_alloc_range_id ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32*         pui4_range_id )
{
    RM_IDXBULD_OBJ_T* pt_rm_handler;
    UINT8              ui1_key_range_id;
    UINT8              ui1_range_id;

    /*Parameters check*/
    if ((NULL == pt_midxbuld_obj) || (NULL == pui4_range_id))
    {
        DBG_ERROR(("_rm_midxbuld_alloc_range_id:NULL\r\n"));
        return FALSE;
    }

    /*get rm index builder*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    { 
        DBG_ERROR(("_rm_midxbuld_alloc_range_id:pt_rm_handler NULL\r\n"));
        return FALSE;
    }

    *pui4_range_id = MIDXBULD_NULL_ID;

    if (pt_rm_handler->pt_key_range_obj != NULL)
    {
        ui1_key_range_id = (UINT8)((
            pt_rm_handler->pt_key_range_obj->ui4_range_id 
            & RM_MIDXBULD_RANGE_ID_MASK)>> 8);
    }
    /* key range should be allocated first */
    else if(pt_rm_handler->ui1_range_count > 0)
    {
        DBG_ERROR(("_rm_midxbuld_alloc_range_id:count=%u\r\n",
                  pt_rm_handler->ui1_range_count));
        return FALSE;
    }
    else
    {
        ui1_key_range_id = 1;
    }

    if (pt_rm_handler->ui1_range_count < 0xff)
    {
        UINT8 ui1_i;
        BOOL  b_occupied = TRUE;

        while (b_occupied)
        {
            b_occupied = FALSE;
            if (pt_rm_handler->ui1_range_count > RM_MIDXBULD_RANGE_MAX)
            {
                DBG_ERROR(("_rm_midxbuld_alloc_range_id:error count!!!\r\n"));
                return FALSE;
            }
            for (ui1_i = 0; ui1_i < pt_rm_handler->ui1_range_count; ++ui1_i)
            {
                ui1_range_id = (UINT8)
                    ((pt_rm_handler->apt_range_obj[ui1_i]->ui4_range_id 
                     & RM_MIDXBULD_RANGE_ID_MASK)>> 8);
                if ((ui1_range_id == pt_rm_handler->ui1_idx_range_id_last) ||
                    (ui1_key_range_id == pt_rm_handler->ui1_idx_range_id_last))
                {
                    b_occupied = TRUE;
                    pt_rm_handler->ui1_idx_range_id_last = (UINT8)
                           ((pt_rm_handler->ui1_idx_range_id_last == 0xff)
                           ?  1 : (pt_rm_handler->ui1_idx_range_id_last + 1));
                    break;
                }
            }
        }
    
        *pui4_range_id = (UINT32)pt_rm_handler->ui1_idx_range_id_last;
    
        pt_rm_handler->ui1_idx_range_id_last = (UINT8)
              ((pt_rm_handler->ui1_idx_range_id_last == 0xff)
              ? 1 : (pt_rm_handler->ui1_idx_range_id_last + 1));
        
        /*midxbuld hande + range id + table id*/
        *pui4_range_id = ((pt_midxbuld_obj->h_midxbuld & 0x0000ffff) << 16) +
                         (*pui4_range_id << 8);
        DBG_INFO(("_rm_midxbuld_alloc_range_id:range id=%u\r\n",
                  *pui4_range_id));
        
        return TRUE;
    }
    
    DBG_INFO(("_rm_midxbuld_alloc_range_id:fail\r\n"));
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_midxbuld_free_range_obj
 *
 * Description: This function free range resource.
 *
 * Inputs:  
 *          pt_midxbuld_obj ,  specifies the index builder object
 *          ui4_range_id ,    specifies range id
 *
 * Outputs: NULL 
 *
 * Returns: TRUE    Success. The routine was successful and the operation has 
 *                  been completed.
 *          FALSE   Fail
 * Note:  
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/
static BOOL _rm_midxbuld_free_range_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32          ui4_range_id)
{
    RM_IDXBULD_OBJ_T*          pt_rm_handler;
    RM_IDXBULD_RANGE_OBJ_T*    pt_range_obj;
    UINT8                       ui1_i = 0;
    BOOL                        b_id_match = FALSE;
    RM_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    UINT32                      ui4_tbl_id;
    BOOL                        b_non_key = TRUE; /*if false it is key range*/

    /*Parameters check*/
    if (NULL == pt_midxbuld_obj)
    {
        DBG_ERROR(("_rm_midxbuld_free_range_obj:NULL\r\n"));
        return FALSE;
    }
    
    /*init NULL*/
    pt_range_obj = NULL;
    
    /*get rm index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("_rm_midxbuld_free_range_obj:pt_rm_handler NULL\r\n"));
        return FALSE;
    }
    if (pt_rm_handler->ui1_range_count > RM_MIDXBULD_RANGE_MAX)
    {
        DBG_ERROR(("_rm_midxbuld_free_range_obj:error count!!!\r\n"));
        return FALSE;
    }
    /*If it is key range?*/
    if (pt_rm_handler->pt_key_range_obj->ui4_range_id == ui4_range_id)
    {
        b_id_match = TRUE;
        b_non_key = FALSE;
        pt_range_obj = pt_rm_handler->pt_key_range_obj;
    }
    /*non key range*/
    else if (pt_rm_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_rm_handler->ui1_range_count; ++ui1_i)
        {
            if (pt_rm_handler->apt_range_obj[ui1_i]->ui4_range_id == 
                ui4_range_id)
            {
                b_id_match = TRUE;
                b_non_key = TRUE;
                pt_range_obj = pt_rm_handler->apt_range_obj[ui1_i];
                break;
            }
        }
    }
    else
    {
        DBG_INFO(("_rm_midxbuld_free_range_obj:did not find range id=%\r\n",
                  ui4_range_id));
        return FALSE;
    }
    
    if (FALSE == b_id_match)
    {
        DBG_INFO(("_rm_midxbuld_free_range_obj:not find range id=%\r\n",
                  ui4_range_id));
        return FALSE;
    }
    
    /*the given range exists go on...*/
    if (pt_range_obj)
    {
        while (pt_range_obj->pt_tbl_obj != NULL)
        {
            pt_tbl_obj = pt_range_obj->pt_tbl_obj;
            ui4_tbl_id = pt_tbl_obj->ui4_tbl_id;
            /*the following function will adjust pt_tbl_obj of pt_range_obj*/
            _rm_midxbuld_free_table_obj(pt_midxbuld_obj,
                                               ui4_tbl_id);
        }

        /*free the given range*/
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);

        /*merge table array  */
        if (TRUE == b_non_key)
        {
            while ((ui1_i + 1) < pt_rm_handler->ui1_range_count)
            {
                if (ui1_i + 1 >= RM_MIDXBULD_RANGE_MAX)
                {
                    DBG_ERROR(("_rm_midxbuld_free_range_obj:error index!\r\n"));
                    return FALSE;
                }
                pt_rm_handler->apt_range_obj[ui1_i] = 
                                       pt_rm_handler->apt_range_obj[ui1_i + 1];
                pt_rm_handler->apt_range_obj[++ui1_i] = NULL;
            }
            --(pt_rm_handler->ui1_range_count);
        }
        else
        {
            pt_rm_handler->pt_key_range_obj = NULL;
        }
        
        DBG_INFO(("_rm_midxbuld_free_range_obj:free range id=%d OK\r\n",
                  ui4_range_id));
        return TRUE;
    }
    else
    {
        DBG_INFO(("_rm_midxbuld_free_range_obj:free range id=%d fail\r\n",
                  ui4_range_id));
        return FALSE;
    }
    
}

#if 0
static INT32 _rm_idxbuild_stream_id_2_ele_strm_type(
                             UINT32                     ui4_strm_id,
                             MIDXBULD_OBJ_T*            pt_midxbuld_obj,
                             MIDXBULD_ELMT_STRM_TYPE_T *pt_ele_strm_type)
{
    MIDXBULD_SOURCE_INFO_T  *pt_src_info = NULL;

    UINT8   ui1_count = 0;
    
    if (NULL == pt_midxbuld_obj)
    {
        return MIDXBULDR_INV_ARG;
    }
    pt_src_info = &pt_midxbuld_obj->t_src_info;

    for(ui1_count = 0; ui1_count < MIDXBULD_ELMT_TBL_MAX; ui1_count++)
    {
        /*if find out break*/
        if(pt_src_info->t_es_map[ui1_count].ui4_strm_id == ui4_strm_id)
        {
            *pt_ele_strm_type = pt_src_info->t_es_map[ui1_count].e_es_type;          
            break;
        }
    }
    
    if(ui1_count == MIDXBULD_ELMT_TBL_MAX)
    {
        return MIDXBULDR_INV_ARG;
    }
    return MIDXBULDR_OK;
}

INT32 _rm_midxbuld_indexing( MIDXBULD_OBJ_T* pt_midxbuld_obj )
{
    INT32                           i4_ret                  = 0; /*return code*/
    INT32                           i4_read_sz              = 0;
    UINT16                          ui2_obj_ver             = 0;
    UINT16                          ui2_strm_index          = 0;
    UINT32                          ui4_id                  = 0;
    UINT32                          ui4_idx_size            = 0;
    UINT32                          ui4_number              = 0;
    UINT32                          ui4_number_of_entry     = 0;
    UINT32                          ui4_buf_read_pos        = 0;
    UINT32                          ui4_buf_write_pos       = 0;
    UINT64                          ui8_curr_parser_offset  = 0;
    UINT64                          ui8_idx_offset          = 0;
    
    RM_IDXBULD_OBJ_T*               pt_rm_handler    = NULL;
    RM_IDXBULD_RANGE_OBJ_T*         pt_range_obj     = NULL;
    RM_IDXBULD_ELMT_TBL_OBJ_T*      pt_tbl_obj       = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_index_records = NULL;
    MIDXBULD_KEY_TBL_EXT_T*         pt_key_tbl_ext   = NULL; /* (O) */
    MIDXBULD_ELMT_TBL_OBJ_T*        pt_elmt_tbl      = NULL;
        
    /*check parameters*/
    if (NULL == pt_midxbuld_obj)
    {
        DBG_ERROR(("RM_MIDXBULD_INDEXING : parameter invalid!\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    /*get index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("RM_MIDXBULD_INDEXING : pt_rm_handler NULL!\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*init local variables*/
    pt_range_obj     = pt_rm_handler->pt_key_range_obj;
    pt_tbl_obj       = pt_rm_handler->pt_key_range_obj->pt_tbl_obj;
    pt_key_tbl_ext   = pt_rm_handler->t_key_table_obj.pt_key_tbl_ext;
    
    /*set pr_range_obj*/
    pt_range_obj->e_tbl_type                 = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts   = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->b_is_key_frame             = TRUE;
    pt_range_obj->b_index_is_valid           = FALSE;
    pt_range_obj->ui1_tbl_total              = 1;

    i4_ret = MIDXBULDR_OK;
    ui8_idx_offset = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
    pt_elmt_tbl = &pt_tbl_obj->t_elmt_tbl;
    while(pt_elmt_tbl)
    {
        /*read index to buffer*/
        x_memset(pt_rm_handler->pui1_header_buf, 0, pt_rm_handler->ui4_buf_length);
        ui8_curr_parser_offset = ui8_idx_offset;
        i4_read_sz = _rm_idxbuild_load_buf(pt_midxbuld_obj, 0, ui8_curr_parser_offset);
        if(i4_read_sz < RM_INDEX_HEADER_SIZE)
        {
            DBG_ERROR(("RM_MIDXBULD_INDEXING: read index block fail!\r\n"));
            i4_ret =  MIDXBULDR_INTERNAL_ERR;
            break;
        }
        ui8_curr_parser_offset += i4_read_sz;
        
        LOADB_DWRD(pt_rm_handler->pui1_header_buf,      ui4_id);
        LOADB_DWRD(pt_rm_handler->pui1_header_buf + 4,  ui4_idx_size);
        LOADB_WORD(pt_rm_handler->pui1_header_buf + 8,  ui2_obj_ver);
        LOADB_DWRD(pt_rm_handler->pui1_header_buf + 10, ui4_number_of_entry);
        LOADB_WORD(pt_rm_handler->pui1_header_buf + 14, ui2_strm_index);
        if (ui4_id != RM_BOX_INDX || ui2_obj_ver != 0)
        {
            DBG_ERROR(("RM_MIDXBULD_INDEXING : index head invalid!\r\n"));
            i4_ret =  MIDXBULDR_INTERNAL_ERR;
            break;
        }
        
        ui4_number += ui4_number_of_entry;
        /*set pt_tbl_obj->t_elmt_tbl*/
        pt_elmt_tbl->ui8_base_offset     = 0;
        pt_elmt_tbl->e_tbl_type          = MIDXBULD_TBL_TYPE_ELMT_STRM;
        pt_elmt_tbl->ui4_number_of_entry = ui4_number_of_entry;
        
        /*set pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info*/
        if(MIDXBULDR_OK != _rm_idxbuild_stream_id_2_ele_strm_type(
            ui2_strm_index,
            pt_midxbuld_obj,
            &pt_elmt_tbl->u_fmt.t_es.t_info.e_es_type))
        {
            DBG_ERROR(("RM_MIDXBULD_INDEXING : stream type unknown!\r\n"));
            i4_ret =  MIDXBULDR_INTERNAL_ERR;
            break;
        }
        pt_elmt_tbl->u_fmt.t_es.t_info.ui4_indx_offset = (UINT32)ui8_idx_offset;
        pt_elmt_tbl->u_fmt.t_es.t_info.ui4_strm_id     = ui2_strm_index;
        pt_elmt_tbl->u_fmt.t_es.t_info.ui4_indx_size   = ui4_idx_size;
        
        pt_index_records = pt_elmt_tbl->u_fmt.t_es.pt_idx_entry;
        if(ui4_number_of_entry > 0)
        {
            ui4_buf_read_pos  = RM_INDEX_HEADER_SIZE;
            ui4_buf_write_pos = (UINT32)i4_read_sz;
            
            if(pt_index_records == NULL)
            {
                DBG_ERROR(("RM_MIDXBULD_INDEXING: pt_index_records null!\r\n"));
                i4_ret = MIDXBULDR_INTERNAL_ERR;
                break;
            }

            for( ; ui4_number_of_entry > 0; ui4_number_of_entry--)
            {
                if ((UINT64)ui4_buf_read_pos + RM_INDEX_RECORD_SIZE > ui4_buf_write_pos)
                {
                    i4_read_sz = _rm_idxbuild_load_buf(pt_midxbuld_obj, 
                                                        ui4_buf_read_pos, 
                                                        ui8_curr_parser_offset);
                    if (i4_read_sz < 0)
                    {
                        _mm_util_mem_free(pt_index_records);
                        pt_elmt_tbl->u_fmt.t_es.pt_idx_entry = NULL;
                        DBG_ERROR(("RM ERR: _rm_idxbuild_parse_record load buf error!\r\n"));
                        return MIDXBULDR_INTERNAL_ERR;
                    }
                    ui8_curr_parser_offset += i4_read_sz;
                    ui4_buf_write_pos += (UINT32)i4_read_sz - ui4_buf_read_pos;
                    ui4_buf_read_pos = 0;
                }
                LOADB_WORD(pt_rm_handler->pui1_header_buf + ui4_buf_read_pos, ui2_obj_ver);
                if (!ui2_obj_ver)
                {
                    LOADB_DWRD(pt_rm_handler->pui1_header_buf + ui4_buf_read_pos + 2, 
                        pt_index_records->ui4_pts);
                    LOADB_DWRD(pt_rm_handler->pui1_header_buf + ui4_buf_read_pos + 6, 
                        pt_index_records->ui4_relative_offset);
                }
                ui4_buf_read_pos += RM_INDEX_RECORD_SIZE;
                pt_index_records++;
            }
        }

        pt_elmt_tbl = pt_elmt_tbl->pt_next;
        ui8_idx_offset += ui4_idx_size;
        if(ui8_idx_offset > pt_midxbuld_obj->z_buff_leng)
        {
            break;
        }
    }
    
    /*set pt_tbl_obj*/
    pt_tbl_obj->ui4_tbl_size = ui4_number;
    
    /*set pt_key_tbl_ext*/
    pt_key_tbl_ext->t_elmt_tbl  = pt_tbl_obj->t_elmt_tbl;
    pt_rm_handler->b_key_tbl_ok = 1;
    pt_rm_handler->t_key_table_obj.ui4_key_tbl_id = pt_tbl_obj->ui4_tbl_id;
    DBG_INFO(("RM_MIDXBULD_INDEXING: rm_indx_parse succeed!\r\n"));
    
    return i4_ret;
}   
#else 
INT32 _rm_midxbuld_indexing( MIDXBULD_OBJ_T* pt_midxbuld_obj )
{
    INT32                           i4_ret                  = 0; /*return code*/
    INT32                           i4_read_sz              = 0;
    UINT16                          ui2_obj_ver             = 0;
    UINT16                          ui2_strm_index          = 0;
    UINT32                          ui4_id                  = 0;
    UINT32                          ui4_idx_size            = 0;
    UINT32                          ui4_number_of_entry     = 0;
    UINT32                          ui4_buf_read_pos        = 0;
    UINT32                          ui4_buf_write_pos       = 0;
    UINT64                          ui8_curr_parser_offset  = 0;
    
    RM_IDXBULD_OBJ_T*               pt_rm_handler    = NULL;
    RM_IDXBULD_RANGE_OBJ_T*         pt_range_obj     = NULL;
    RM_IDXBULD_ELMT_TBL_OBJ_T*      pt_tbl_obj       = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_index_records = NULL;
    MIDXBULD_KEY_TBL_EXT_T*         pt_key_tbl_ext   = NULL; /* (O) */
    MIDXBULD_ELMT_TBL_OBJ_T*        pt_elmt_tbl      = NULL;
        
    /*check parameters*/
    if (NULL == pt_midxbuld_obj)
    {
        DBG_ERROR(("RM_MIDXBULD_INDEXING : parameter invalid!\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    /*get index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("RM_MIDXBULD_INDEXING : pt_rm_handler NULL!\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    if (!pt_rm_handler->b_key_tbl_ok)
    {
        return MIDXBULDR_OK;
    }
    
    /*init local variables*/
    pt_range_obj     = pt_rm_handler->pt_key_range_obj;
    pt_tbl_obj       = pt_rm_handler->pt_key_range_obj->pt_tbl_obj;
    pt_key_tbl_ext   = pt_rm_handler->t_key_table_obj.pt_key_tbl_ext;
    pt_index_records = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    
    /*set pr_range_obj*/
    pt_range_obj->e_tbl_type                 = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts   = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->b_is_key_frame             = TRUE;
    pt_range_obj->b_index_is_valid           = FALSE;
    pt_range_obj->ui1_tbl_total              = 1;

    i4_ret = MIDXBULDR_OK;
    do
    {
        /*read index to buffer*/
        x_memset(pt_rm_handler->pui1_header_buf, 0, pt_rm_handler->ui4_buf_length);
        ui8_curr_parser_offset = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
        i4_read_sz = _rm_idxbuild_load_buf(pt_midxbuld_obj, 0, ui8_curr_parser_offset);
        if(i4_read_sz < RM_INDEX_HEADER_SIZE)
        {
            DBG_ERROR(("RM_MIDXBULD_INDEXING: read index block fail!\r\n"));
            i4_ret =  MIDXBULDR_INTERNAL_ERR;
            break;
        }
        ui8_curr_parser_offset += i4_read_sz;
        
        LOADB_DWRD(pt_rm_handler->pui1_header_buf,      ui4_id);
        LOADB_DWRD(pt_rm_handler->pui1_header_buf + 4,  ui4_idx_size);
        LOADB_WORD(pt_rm_handler->pui1_header_buf + 8,  ui2_obj_ver);
        LOADB_DWRD(pt_rm_handler->pui1_header_buf + 10, ui4_number_of_entry);
        LOADB_WORD(pt_rm_handler->pui1_header_buf + 14, ui2_strm_index);
        if (ui4_id != RM_BOX_INDX || ui2_obj_ver != 0)
        {
            DBG_ERROR(("RM_MIDXBULD_INDEXING : index head invalid!\r\n"));
            i4_ret =  MIDXBULDR_INTERNAL_ERR;
            break;
        }

        /*set pt_tbl_obj*/
        pt_tbl_obj->ui4_tbl_size         = ui4_number_of_entry;
        
        /*set pt_tbl_obj->t_elmt_tbl*/
        pt_elmt_tbl                      = &pt_tbl_obj->t_elmt_tbl;
        pt_elmt_tbl->ui8_base_offset     = 0;
        pt_elmt_tbl->e_tbl_type          = MIDXBULD_TBL_TYPE_ELMT_STRM;
        pt_elmt_tbl->pt_next             = NULL;
        pt_elmt_tbl->ui4_number_of_entry = ui4_number_of_entry;
        
        /*set pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info*/
        pt_elmt_tbl->u_fmt.t_es.t_info.e_es_type       = MIDXBULD_ELEM_STRM_TYPE_KEY;
        pt_elmt_tbl->u_fmt.t_es.t_info.ui4_indx_offset 
            = (UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_offset;
        pt_elmt_tbl->u_fmt.t_es.t_info.ui4_strm_id     = ui2_strm_index;
        pt_elmt_tbl->u_fmt.t_es.t_info.ui4_indx_size   = ui4_idx_size;
        
        if(ui4_number_of_entry > 0)
        {
            ui4_buf_read_pos  = RM_INDEX_HEADER_SIZE;
            ui4_buf_write_pos = (UINT32)i4_read_sz;
            
            if(pt_index_records == NULL)
            {
                DBG_ERROR(("RM_MIDXBULD_INDEXING: pt_index_records null!\r\n"));
                i4_ret = MIDXBULDR_INTERNAL_ERR;
                break;
            }

            for( ; ui4_number_of_entry > 0; ui4_number_of_entry--)
            {
                if ((UINT64)ui4_buf_read_pos + RM_INDEX_RECORD_SIZE > ui4_buf_write_pos)
                {
                    i4_read_sz = _rm_idxbuild_load_buf(pt_midxbuld_obj, 
                                                        ui4_buf_read_pos, 
                                                        ui8_curr_parser_offset);
                    if (i4_read_sz < 0)
                    {
                        _mm_util_mem_free_with_tag(pt_elmt_tbl->u_fmt.t_es.pt_idx_entry, MM_MEM_MIDX_TAG);
                        pt_elmt_tbl->u_fmt.t_es.pt_idx_entry = NULL;
                        DBG_ERROR(("RM ERR: _rm_idxbuild_parse_record load buf error!\r\n"));
                        return MIDXBULDR_INTERNAL_ERR;
                    }
                    ui8_curr_parser_offset += i4_read_sz;
                    ui4_buf_write_pos += (UINT32)i4_read_sz - ui4_buf_read_pos;
                    ui4_buf_read_pos = 0;
                }
                LOADB_WORD(pt_rm_handler->pui1_header_buf + ui4_buf_read_pos, ui2_obj_ver);
                if (!ui2_obj_ver)
                {
                    LOADB_DWRD(pt_rm_handler->pui1_header_buf + ui4_buf_read_pos + 2, 
                        pt_index_records->ui4_pts);
                    LOADB_DWRD(pt_rm_handler->pui1_header_buf + ui4_buf_read_pos + 6, 
                        pt_index_records->ui4_relative_offset);
                }
                ui4_buf_read_pos += RM_INDEX_RECORD_SIZE;
                pt_index_records++;
            }
        }
        
        /*set pt_key_tbl_ext*/
        pt_key_tbl_ext->t_elmt_tbl  = *pt_elmt_tbl;
        pt_rm_handler->b_key_tbl_ok = 1;
        pt_rm_handler->t_key_table_obj.ui4_key_tbl_id = pt_tbl_obj->ui4_tbl_id;
        
        DBG_INFO(("RM_MIDXBULD_INDEXING: rm_indx_parse succeed!\r\n"));
    }while(0);
    
    return i4_ret;
}    
#endif
        
/*-----------------------------------------------------------------------------
 * Name:  _rm_midxbuld_key_packet_table_alloc
 *
 * Description: This function paser cue objects 
 *
 * Inputs:  
 *         pt_key_info:     specifies which stream should build key frame table
 *         pt_midxbuld_obj: specify the media info object  
 *
 * Outputs: NULL 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the 
 *                          operation has been completed.
 *          Other non-zero values: Fail
 * Note:  
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/
#if 0
INT32 _rm_midxbuld_key_packet_table_alloc( 
                                           MIDXBULD_KEY_INFO_T* pt_key_info, 
                                           MIDXBULD_OBJ_T* pt_midxbuld_obj )
{
    INT32                           i4_ret                  = 0; 
    UINT16                          ui2_obj_ver             = 0;
    UINT32                          ui4_idx_size            = 0;
    UINT32                          ui4_id                  = 0;
    UINT32                          ui4_read_sz             = 0;
    UINT32                          ui4_number_of_entry     = 0;
    UINT64                          ui8_idx_offset          = 0;
    
    RM_IDXBULD_OBJ_T*               pt_rm_handler    = NULL;
    RM_IDXBULD_RANGE_OBJ_T*         pt_range_obj     = NULL;
    RM_IDXBULD_ELMT_TBL_OBJ_T*      pt_tbl_obj       = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_index_records = NULL;
    MIDXBULD_KEY_TBL_EXT_T*         pt_key_tbl_ext   = NULL;
    MIDXBULD_ELMT_TBL_OBJ_T*        pt_ele_tbl_obj   = NULL;

    
    /*check parameters*/
    if ((NULL == pt_key_info) || (NULL == pt_midxbuld_obj))
    {
        DBG_ERROR(("RM_IDXBULD_ERR : parameter invalid!\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /*get index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("RM_IDXBULD_ERR : pt_rm_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*init local variables*/
    i4_ret                    = MIDXBULDR_OK;

    /*fill in range*/
    do 
    {
        /*(1)apply range object*/
        pt_range_obj = (RM_IDXBULD_RANGE_OBJ_T*)
                _mm_util_mem_alloc_with_tag(sizeof(RM_IDXBULD_RANGE_OBJ_T),
                MM_MEM_MIDX_TAG);
        if (NULL == pt_range_obj)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : allocate pt_range_obj fail!"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_range_obj, 0, sizeof(RM_IDXBULD_RANGE_OBJ_T));
        
        /*(2)allocate id for pt_range_obj*/
        if (TRUE != _rm_midxbuld_alloc_range_id(pt_midxbuld_obj, 
                                            &pt_range_obj->ui4_range_id))
        {
            DBG_ERROR(("RM_IDXBULD_ERR : alloc id for pt_range_obj fail!\r\n"));
            i4_ret = MIDXBULDR_OUT_OF_HANDLE;
            break;
        }
        
        /*alloc pt_tbl_obj*/
        pt_tbl_obj = _rm_idxbuild_create_tab(pt_range_obj);
        if(!pt_tbl_obj)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : alloc pt_tbl_obj fail!\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        
        i4_ret         = MIDXBULDR_OK;
        ui8_idx_offset = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
        pt_ele_tbl_obj = &pt_tbl_obj->t_elmt_tbl;
        while(ui8_idx_offset < pt_midxbuld_obj->z_buff_leng)
        {
            /*parse index head*/
            _rm_idx_set_pos(pt_midxbuld_obj, ui8_idx_offset, FM_SEEK_BGN);
            i4_ret = _rm_idx_copy_bytes(pt_midxbuld_obj, 
                                pt_rm_handler->pui1_header_buf, 
                                (SIZE_T)pt_midxbuld_obj->z_buff_leng, 
                                RM_INDEX_HEADER_SIZE, 
                                &ui4_read_sz);
            if ((i4_ret != INPUTR_OK) || (ui4_read_sz < RM_INDEX_HEADER_SIZE))
            {
                DBG_ERROR(("RM_IDXBULD_ERR : read index head fail!\r\n"));
                i4_ret =  MIDXBULDR_INTERNAL_ERR;
                break;
            }
            LOADB_DWRD(pt_rm_handler->pui1_header_buf, ui4_id);
            LOADB_DWRD(pt_rm_handler->pui1_header_buf + 4, ui4_idx_size);
            LOADB_WORD(pt_rm_handler->pui1_header_buf + 8, ui2_obj_ver);
            LOADB_DWRD(pt_rm_handler->pui1_header_buf + 10, ui4_number_of_entry);
            
            ui8_idx_offset += ui4_idx_size;
            if (ui4_id != RM_BOX_INDX 
                || ui2_obj_ver != 0 
                || ui8_idx_offset > pt_midxbuld_obj->z_buff_leng)
            {
                DBG_ERROR(("RM_IDXBULD_ERR : index head invalid!\r\n"));
                i4_ret = MIDXBULDR_INTERNAL_ERR;
                break;
            }
            
            /*alloc index record*/
            pt_index_records = (MIDXBULD_ELMT_STRM_ENTRY_T*)_mm_util_mem_alloc(
                                sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_number_of_entry);
            if(!pt_index_records)
            {
                DBG_ERROR(("RM_IDXBULD_ERR : alloc pt_index_records fail!\r\n"));
                i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
                break;
            }
            pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry  = pt_index_records;

            /*alloc t_elmt_tbl*/
            pt_ele_tbl_obj->pt_next = (MIDXBULD_ELMT_TBL_OBJ_T*)_mm_util_mem_alloc(
                                sizeof(MIDXBULD_ELMT_TBL_OBJ_T));
            if(!pt_ele_tbl_obj->pt_next)
            {
                DBG_ERROR(("RM_IDXBULD_ERR : alloc pt_ele_tbl_obj fail!\r\n"));
                i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
                break;
            }
            pt_ele_tbl_obj = pt_ele_tbl_obj->pt_next;
        }
        if(MIDXBULDR_OK != i4_ret)
        {
            break;
        }
        
        /*alloc tbl ext*/
        pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)
                         _mm_util_mem_alloc(sizeof(MIDXBULD_KEY_TBL_EXT_T));
        if(!pt_key_tbl_ext)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : alloc pt_key_tbl_ext fail!\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }

        /*clear structure*/
        x_memset(pt_index_records, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_number_of_entry);
        x_memset(pt_key_tbl_ext, 0, sizeof(MIDXBULD_KEY_TBL_EXT_T));
        
        /*set key range*/
        pt_rm_handler->pt_key_range_obj                 = pt_range_obj;
        pt_rm_handler->pt_key_range_obj->pt_tbl_obj     = pt_tbl_obj;
        pt_rm_handler->apt_tbl_obj_map[0]               = pt_tbl_obj;
        pt_rm_handler->t_key_table_obj.pt_key_tbl_ext   = pt_key_tbl_ext;
    }while(0);
    
    
    if(MIDXBULDR_OK != i4_ret)
    {
        if(pt_range_obj)
        {
            _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
            pt_range_obj        = NULL;
        }
        if(pt_tbl_obj)
        {
            pt_ele_tbl_obj = pt_tbl_obj->t_elmt_tbl.pt_next;
            while(pt_ele_tbl_obj)
            {
                if(pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry)
                {
                    _mm_util_mem_free_with_tag(
                        pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry, 
                        MM_MEM_MIDX_TAG);
                    pt_ele_tbl_obj->u_fmt.t_es.pt_idx_entry = NULL;
                }
                pt_tbl_obj->t_elmt_tbl.pt_next = pt_ele_tbl_obj->pt_next;
                _mm_util_mem_free_with_tag(pt_ele_tbl_obj, MM_MEM_MIDX_TAG);
                pt_ele_tbl_obj = pt_tbl_obj->t_elmt_tbl.pt_next;
            }
            _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
            pt_tbl_obj          = NULL;
        }
        if(pt_key_tbl_ext)
        {
            _mm_util_mem_free_with_tag(pt_key_tbl_ext, MM_MEM_MIDX_TAG);
            pt_key_tbl_ext      = NULL;
        }
        pt_key_info->ui4_tbl_id        = MIDXBULD_NULL_ID;
        pt_key_info->ui4_range_id      = MIDXBULD_NULL_ID;
    }
    else
    {
        pt_key_info->ui4_tbl_id        = pt_tbl_obj->ui4_tbl_id;
        pt_key_info->ui4_range_id      = pt_range_obj->ui4_range_id;
    }

    pt_rm_handler->t_key_info = *pt_key_info;
    return i4_ret;
}
#else
INT32 _rm_midxbuld_key_packet_table_alloc( 
                                           MIDXBULD_KEY_INFO_T* pt_key_info, 
                                           MIDXBULD_OBJ_T* pt_midxbuld_obj )
{
    INT32                           i4_ret                  = 0; 
    UINT16                          ui2_obj_ver             = 0;
    UINT16                          ui2_strm_index          = 0;
    UINT32                          ui4_id                  = 0;
    UINT32                          ui4_read_sz             = 0;
    UINT32                          ui4_number_of_entry     = 0;
    
    RM_IDXBULD_OBJ_T*               pt_rm_handler    = NULL;
    RM_IDXBULD_RANGE_OBJ_T*         pt_range_obj     = NULL;
    RM_IDXBULD_ELMT_TBL_OBJ_T*      pt_tbl_obj       = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_index_records = NULL;
    MIDXBULD_KEY_TBL_EXT_T*         pt_key_tbl_ext   = NULL;

    
    /*check parameters*/
    if ((NULL == pt_key_info) || (NULL == pt_midxbuld_obj))
    {
        DBG_ERROR(("RM_IDXBULD_ERR : parameter invalid!\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /*get index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("RM_IDXBULD_ERR : pt_rm_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*init local variables*/
    i4_ret                    = MIDXBULDR_OK;

    /*fill in range*/
    do 
    {
        /*(1)apply range object*/
        pt_range_obj = (RM_IDXBULD_RANGE_OBJ_T*)
                _mm_util_mem_alloc_with_tag(sizeof(RM_IDXBULD_RANGE_OBJ_T),
                MM_MEM_MIDX_TAG);
        if (NULL == pt_range_obj)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : allocate pt_range_obj fail!"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_range_obj, 0, sizeof(RM_IDXBULD_RANGE_OBJ_T));
        
        /*(2)allocate id for pt_range_obj*/
        if (TRUE != _rm_midxbuld_alloc_range_id(pt_midxbuld_obj, 
                                            &pt_range_obj->ui4_range_id))
        {
            DBG_ERROR(("RM_IDXBULD_ERR : alloc id for pt_range_obj fail!\r\n"));
            i4_ret = MIDXBULDR_OUT_OF_HANDLE;
            break;
        }
        
        /*alloc pt_tbl_obj*/
        pt_tbl_obj = _rm_idxbuild_create_tab(pt_range_obj);
        if(!pt_tbl_obj)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : alloc pt_tbl_obj fail!\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }

        
        /*parse index head*/
        _rm_idx_set_pos(pt_midxbuld_obj, pt_midxbuld_obj->t_src_info.ui8_idx_offset, FM_SEEK_BGN);
        i4_ret = _rm_idx_copy_bytes(pt_midxbuld_obj, 
                            pt_rm_handler->pui1_header_buf, 
                            (SIZE_T)pt_midxbuld_obj->z_buff_leng, 
                            RM_INDEX_HEADER_SIZE, 
                            &ui4_read_sz);
        if ((i4_ret != INPUTR_OK) || (ui4_read_sz < RM_INDEX_HEADER_SIZE))
        {
            DBG_ERROR(("RM_IDXBULD_ERR : read index head fail!\r\n"));
            i4_ret =  MIDXBULDR_KEY_TBL_NOT_READY;
            break;
        }
        LOADB_DWRD(pt_rm_handler->pui1_header_buf, ui4_id);
        LOADB_WORD(pt_rm_handler->pui1_header_buf + 8, ui2_obj_ver);
        LOADB_DWRD(pt_rm_handler->pui1_header_buf + 10, ui4_number_of_entry);
        LOADB_WORD(pt_rm_handler->pui1_header_buf + 14, ui2_strm_index);
        if (ui4_id != RM_BOX_INDX || ui2_obj_ver != 0)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : index head invalid!\r\n"));
            i4_ret =  MIDXBULDR_KEY_TBL_NOT_READY;
            break;
        }

        /*alloc index record*/
        pt_index_records = (MIDXBULD_ELMT_STRM_ENTRY_T*)_mm_util_mem_alloc(
                            sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_number_of_entry);
        if(!pt_index_records)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : alloc pt_index_records fail!\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        
        /*alloc tbl ext*/
        pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)
                         _mm_util_mem_alloc(sizeof(MIDXBULD_KEY_TBL_EXT_T));
        if(!pt_key_tbl_ext)
        {
            DBG_ERROR(("RM_IDXBULD_ERR : alloc pt_key_tbl_ext fail!\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }

        /*clear structure*/
        x_memset(pt_index_records, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_number_of_entry);
        x_memset(pt_key_tbl_ext, 0, sizeof(MIDXBULD_KEY_TBL_EXT_T));
        
        /*set key range*/
        pt_rm_handler->pt_key_range_obj                 = pt_range_obj;
        pt_rm_handler->pt_key_range_obj->pt_tbl_obj     = pt_tbl_obj;
        pt_rm_handler->apt_tbl_obj_map[ui2_strm_index]  = pt_tbl_obj;
        pt_rm_handler->t_key_table_obj.pt_key_tbl_ext   = pt_key_tbl_ext;
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry  = pt_index_records;
    }while(0);
    
    pt_rm_handler->b_key_tbl_ok = TRUE;
    if(MIDXBULDR_OK != i4_ret)
    {
        if(pt_range_obj)
        {
            _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
            pt_range_obj        = NULL;
        }
        if(pt_tbl_obj)
        {
            _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
            pt_tbl_obj          = NULL;
        }
        if(pt_index_records)
        {
            _mm_util_mem_free_with_tag(pt_index_records, MM_MEM_MIDX_TAG);
            pt_index_records    = NULL;
        }
        if(pt_key_tbl_ext)
        {
            _mm_util_mem_free_with_tag(pt_key_tbl_ext, MM_MEM_MIDX_TAG);
            pt_key_tbl_ext      = NULL;
        }
        x_memset(pt_rm_handler, 0, sizeof(RM_IDXBULD_OBJ_T));
        pt_key_info->ui4_tbl_id        = MIDXBULD_NULL_ID;
        pt_key_info->ui4_range_id      = MIDXBULD_NULL_ID;
    }
    else
    {
        pt_key_info->ui4_tbl_id        = pt_tbl_obj->ui4_tbl_id;
        pt_key_info->ui4_range_id      = pt_range_obj->ui4_range_id;
    }
    if(MIDXBULDR_KEY_TBL_NOT_READY == i4_ret)
    {
        i4_ret = MIDXBULDR_OK;
    }

    pt_rm_handler->t_key_info = *pt_key_info;
    return i4_ret;
}
#endif
/*-----------------------------------------------------------------------------
 * Name:  _rm_midxbuld_get_idx_elmt_tbl
 *
 * Description: This function get index info
 *
 * Inputs:  pt_midxbuld_obj, specifies rm index builder object
 *          pt_elmt_tbl    ,   specify what want to retrieve  
 *
 * Outputs: pt_elmt_tbl       specify what want to retrieve 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero values: Fail
 *
 * Note:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/

INT32 _rm_midxbuld_get_idx_elmt_tbl(MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                     MIDXBULD_ELMT_TBL_INFO_T* pt_elmt_tbl)
{
    RM_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    RM_IDXBULD_OBJ_T*          pt_rm_handler;
    RM_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    UINT8                       ui1_i;
    BOOL                        b_id_match = FALSE;
    
    /*check parameters*/
    if ((NULL == pt_midxbuld_obj) || (NULL == pt_elmt_tbl))
    {
        DBG_ERROR(("_rm_midxbuld_get_idx_elmt_tbl:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /*init local variables*/

    /*get rm index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("_rm_midxbuld_get_idx_elmt_tbl:pt_rm_handler NULL\r\n"));
        return MIDXBULDR_INTERNAL_ERR;
    }
    if (pt_rm_handler->ui1_range_count > RM_MIDXBULD_RANGE_MAX)
    {
        DBG_ERROR(("_rm_midxbuld_get_idx_elmt_tbl:error count!!!\r\n"));
        return FALSE;
    }
    /*if it in key range?*/
    if (pt_rm_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id 
        == pt_elmt_tbl->ui4_tbl_id)
    {
        b_id_match = TRUE;
        pt_tbl_obj = pt_rm_handler->pt_key_range_obj->pt_tbl_obj;
    }
    /*non key range array*/
    else if (pt_rm_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_rm_handler->ui1_range_count; ui1_i++)
        {
            pt_obj = pt_rm_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
            while (pt_obj != NULL)
            {
                /*if found break*/
                if (pt_obj->ui4_tbl_id == pt_elmt_tbl->ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_tbl_obj = pt_obj;
                    pt_obj = NULL;
                }
                else
                {
                    pt_obj = pt_obj->pt_next;
                }
            }

            if (b_id_match)
            {
                break;
            }
        }
        
    }

    /*not found*/
    if (FALSE == b_id_match)
    {
        DBG_INFO(("_rm_midxbuld_get_idx_elmt_tbl: not found\r\n"));
        return MIDXBULDR_INTERNAL_ERR;
    }
   
    /*get index builder info*/
    pt_elmt_tbl->t_elmt_tbl.ui8_base_offset
                             = pt_tbl_obj->t_elmt_tbl.ui8_base_offset;
    pt_elmt_tbl->t_elmt_tbl.ui4_number_of_entry 
                             = pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.t_info 
                             = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.pt_idx_entry
                             = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    
    DBG_INFO(("_rm_midxbuld_get_idx_elmt_tbl: found\r\n"));
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_midxbuld_elmt_tbl_alloc
 *
 * Description: This function get index info
 *
 * Inputs:  pt_midxbuld_obj       specify midxbuld  obj 
 *          pt_elmt_tbl_alloc     what range will be indexed
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero values: Fail
 *
 * Note:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/

INT32 _rm_midxbuld_elmt_tbl_alloc(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                              MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc)
{
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _rm_midxbuld_elmt_tbl_free
 *
 * Description: This function get index info
 *
 * Inputs:  pt_midxbuld_obj       specify midxbuld  obj 
            h_elmt_tbl            which index talble be freed
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero values: Fail
 *
 * Note:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/

INT32 _rm_midxbuld_elmt_tbl_free(MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                  HANDLE_T        h_elmt_tbl)
{
    return MIDXBULDR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  rm_midxbuld_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name: specify the path name of the file
 *          pt_key_info : specify the key info object  
 *          pv_obj      : specify the index builder object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and 
 *                           the operation has been completed.
 *          Other non-zero values: Fail
 * Note:
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/
INT32 rm_midxbuld_fopen(
                          CHAR*                ps_path_name, 
                          MIDXBULD_KEY_INFO_T* pt_key_info, 
                          VOID*                pv_obj )
{

#ifndef __NO_FM__

    INT32               i4_ret;
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    RM_IDXBULD_OBJ_T*   pt_rm_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == ps_path_name) || (NULL == pt_key_info))
    {
        DBG_ERROR(("rm_midxbuld_fopen:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*init local variables*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;
    
    /*set input functions*/
    pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input1    = mm_util_finput1bytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input3    = mm_util_finput3bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4    = mm_util_finput4bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos   = mm_util_fsetpos_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos   = mm_util_fgetpos_fct;


    /*rm handler for index builder*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)_mm_util_mem_alloc_with_tag(
                            sizeof(RM_IDXBULD_OBJ_T), MM_MEM_MIDX_TAG);
    if ( NULL == pt_rm_handler )
    {
        DBG_ERROR(("rm_midxbuld_fopen:pt_rm_handler is NULL\r\n"));
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_rm_handler, 0, sizeof(RM_IDXBULD_OBJ_T));
    
    /*set rm handler*/
    pt_midxbuld_obj->pv_handler_obj = pt_rm_handler;

    pt_rm_handler->ui1_idx_range_id_last = 1;
    i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                       ps_path_name,
                       FM_READ_ONLY, 
                       0777, FALSE, 
                       &pt_midxbuld_obj->h_file);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("rm_midxbuld_fopen:x_fm_open return %d\r\n", i4_ret));
        _mm_util_mem_free_with_tag(pt_rm_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
        return MIDXBULDR_FILE_ERR;
    }
    
    pt_midxbuld_obj->z_buff_leng = RM_IDXBULD_PARSER_BUFF_LEN;

    pt_rm_handler->ui4_buf_length = RM_IDXBULD_PARSER_BUFF_LEN;
    pt_rm_handler->pui1_header_buf = (UINT8*) 
                 _mm_util_mem_alloc_with_tag(pt_rm_handler->ui4_buf_length,
                 MM_MEM_MIDX_TAG);
    if (NULL == pt_rm_handler->pui1_header_buf)
    {
        DBG_ERROR(("rm_midxbuld_fopen:fail to alloc memory\r\n"));
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    /*build key frame table,generally for video*/
    i4_ret = _rm_midxbuld_key_packet_table_alloc(pt_key_info, pv_obj);
    if (i4_ret != MIDXBULDR_OK)
    {
        _mm_util_mem_free_with_tag(pt_rm_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;

        /*close file*/
        if (pt_midxbuld_obj->h_file)
        {
            x_fm_close(pt_midxbuld_obj->h_file);
            pt_midxbuld_obj->h_file = NULL_HANDLE;
        }
        return i4_ret;
    }
    else
    {
        return MIDXBULDR_OK;
    }

#else
    return MIDXBULDR_INITIAL_ERR;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  rm_midxbuld_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the 
 *                            operation has been completed.
 *          Other non-zero values: Fail
 * Note:
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial

 ----------------------------------------------------------------------------*/
INT32 rm_midxbuld_mopen(
                         VOID*                pv_mem_data,
                         SIZE_T               z_mem_leng,
                         MIDXBULD_KEY_INFO_T* pt_key_info,
                         VOID*                pv_obj)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    RM_IDXBULD_OBJ_T*   pt_rm_handler;
    MEDIA_FEEDER_T*     pt_src_feeder;
    INT32               i4_ret;    
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_mem_data) || (NULL == pv_obj))
    {
        DBG_ERROR(("rm_midxbuld_mopen:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;

    if (z_mem_leng == 0)
    {
    #if 1
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input1;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input3;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_input4;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_set_pos;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_get_pos;
    #endif
    }
    else
    {
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes 
            = mm_util_mcopybytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1    
            = mm_util_minput1bytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3    
            = mm_util_minput3bytes_b_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4    
            = mm_util_minput4bytes_b_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos   
            = mm_util_msetpos_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos   
            = mm_util_mgetpos_fct;
    }

    pt_midxbuld_obj->i8_buff_pos = 0;

    /*allocate memory for idex builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)_mm_util_mem_alloc_with_tag(
                                          sizeof(RM_IDXBULD_OBJ_T),
                                          MM_MEM_MIDX_TAG);
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("rm_midxbuld_mopen:pt_rm_handler NULL\r\n"));
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_rm_handler, 0, sizeof(RM_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_rm_handler;
    
    pt_midxbuld_obj->z_buff_leng = RM_IDXBULD_PARSER_BUFF_LEN;
    pt_rm_handler->ui4_buf_length = RM_IDXBULD_PARSER_BUFF_LEN;
    pt_rm_handler->pui1_header_buf = (UINT8*) 
                 _mm_util_mem_alloc_with_tag(pt_rm_handler->ui4_buf_length,
                 MM_MEM_MIDX_TAG);
    if (NULL == pt_rm_handler->pui1_header_buf)
    {
        DBG_ERROR(("rm_midxbuld_mopen:fail to alloc memory\r\n"));
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    
    pt_rm_handler->ui1_idx_range_id_last = 1;
    i4_ret = _rm_midxbuld_key_packet_table_alloc(pt_key_info, pv_obj);
    if (MIDXBULDR_OK != i4_ret)
    {
        _mm_util_mem_free_with_tag(pt_rm_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
        return i4_ret;
    }
    
    return MIDXBULDR_OK;    
}    

/*-----------------------------------------------------------------------------
 * Name:  rm_midxbuld_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: NULL
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the 
 *                            operation has been completed.
 *          Other non-zero values: Fail
 * Note:
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/
INT32 rm_midxbuld_close(VOID* pv_obj)
{
    MIDXBULD_OBJ_T*          pt_midxbuld_obj;
    RM_IDXBULD_OBJ_T*       pt_rm_handler;
    MIDXBULD_KEY_TBL_EXT_T*  pt_key_tbl_ext; 
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (NULL == pv_obj)  
    {
        DBG_ERROR(("rm_midxbuld_close NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*get index object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*get rm inde handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler)
    {
        DBG_ERROR(("rm_midxbuld_close pt_rm_handler NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    /*close file*/
    if (pt_midxbuld_obj->h_file)
    {
#ifndef __NO_FM__        
        x_fm_close(pt_midxbuld_obj->h_file);
#endif
        pt_midxbuld_obj->h_file = NULL_HANDLE;
    }
    
    /* free key table info */
    if (NULL != pt_rm_handler->t_key_table_obj.pt_key_tbl_ext)
    {
        pt_key_tbl_ext = pt_rm_handler->t_key_table_obj.pt_key_tbl_ext;
        pt_rm_handler->t_key_table_obj.pt_key_tbl_ext = pt_key_tbl_ext->pt_next;
        _mm_util_mem_free_with_tag(pt_key_tbl_ext, 
                                   MM_MEM_MIDX_TAG);
    }
    pt_rm_handler->t_key_table_obj.pt_key_tbl_ext = NULL;
    
    /* free all range objects */
    if (pt_rm_handler->pt_key_range_obj)
    {
        _rm_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                pt_rm_handler->pt_key_range_obj->ui4_range_id);
    }
    while (pt_rm_handler->ui1_range_count > 0)
    {
        _rm_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                pt_rm_handler->apt_range_obj[0]->ui4_range_id);
    }

    if (pt_rm_handler->pui1_header_buf)
    {
        _mm_util_mem_free_with_tag(pt_rm_handler->pui1_header_buf, 
                                   MM_MEM_MIDX_TAG);
        pt_rm_handler->pui1_header_buf = NULL;
        pt_rm_handler->ui4_buf_length = 0;
    }
	
    /*free handler object*/
    if (pt_midxbuld_obj->pv_handler_obj)
    {
        _mm_util_mem_free_with_tag(pt_midxbuld_obj->pv_handler_obj, 
                                   MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
    }
	
    return MIDXBULDR_OK;
}    




/*-----------------------------------------------------------------------------
 * Name:  rm_midxbuld_start_indexing
 *
 * Description: This function start indexing
 *
 * Inputs:  pv_obj        ,  specify the media info object 
 *           pui4_range_id,  specify which range 
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation
 *                        has been completed.
 *          Other non-zero values: Fail
 *
 * Note:  NULL
 * 
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/
INT32 rm_midxbuld_start_indexing(VOID* pv_obj, UINT32* pui4_range_id)
{
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pui4_range_id))
    {
        DBG_ERROR(("rm_midxbuld_start_indexing: NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    if(MIDXBULDR_OK != _rm_midxbuld_indexing(pv_obj))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    /*build index array*/
    return MIDXBULDR_OK;
} 

/*-----------------------------------------------------------------------------
 * Name:  rm_midxbuld_get_info
 *
 * Description: This API retrieves the information of media idx builder handle. 
 *              It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain 
 *                       the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain 
 *                       the media information
 *          pz_buf_leng  If it is smaller than required, it will return 
 *                       MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain
 *                       required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero values: Fail
 * Note:    NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial

 ----------------------------------------------------------------------------*/
INT32 rm_midxbuld_get_info(
                            VOID*                    pv_obj,   
                            MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                            VOID*                    pv_buf,     
                            SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    RM_IDXBULD_OBJ_T*  pt_rm_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pz_buf_leng))
    {
        DBG_ERROR(("rm_midxbuld_get_info:NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* Do                                                                   */
    /************************************************************************/
    /*get index builder object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*get rm index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler) 
    {
        DBG_ERROR(("rm_midxbuld_get_info:pt_rm_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    switch(e_info_type)
    {
        case MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL:
            if (*pz_buf_leng < sizeof(MIDXBULD_ELMT_TBL_INFO_T))
            {
                DBG_ERROR(("rm_midxbuld_get_info:less length(%ld)\r\n",
                           *pz_buf_leng));
                return MIDXBULDR_INITIAL_ERR;
            }
            *pz_buf_leng = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
            return _rm_midxbuld_get_idx_elmt_tbl(pt_midxbuld_obj,
                         (MIDXBULD_ELMT_TBL_INFO_T*)pv_buf);
        case MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST:
            if (FALSE != pt_rm_handler->b_key_tbl_ok)
            {
                *pz_buf_leng = sizeof(MIDXBULD_KEY_TBL_INFO_T);
                x_memcpy((VOID*)pv_buf, 
                         (VOID*)&(pt_rm_handler->t_key_table_obj),
                         *pz_buf_leng);
                return MIDXBULDR_OK;   
            }
            else
            {
                return MIDXBULDR_KEY_TBL_NOT_READY;
            }

        default:
             return MIDXBULDR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  rm_midxbuld_set_info
 *
 * Description: This API retrieves the information of media idx builder handle. 
 *               It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the 
 *                        media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media 
 *                        information
 *          pz_buf_leng  If it is smaller than required, it will return 
 *                         MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain 
 *                         required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero values: Fail
 *
 * Note:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/
INT32 rm_midxbuld_set_info(
                            VOID*                    pv_obj,   
                            MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                            VOID*                    pv_buf,     
                            SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    RM_IDXBULD_OBJ_T*  pt_rm_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pz_buf_leng))
    {
        DBG_ERROR(("rm_midxbuld_set_info:NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*get index builder object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    
    /*get rm index builder handler*/
    pt_rm_handler = (RM_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_rm_handler) 
    {
        DBG_ERROR(("rm_midxbuld_set_info:pt_rm_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    switch (e_info_type)
    {
        case MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE:
            /*The first stage do not come here,only support simple index object*/
            return _rm_midxbuld_elmt_tbl_alloc(
                    pt_midxbuld_obj, 
                    (MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)pv_buf);

        case MIDXBULD_SET_INFO_TYPE_FREE_RANGE:
            return _rm_midxbuld_elmt_tbl_free(
                    pt_midxbuld_obj,
                    ((MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)pv_buf)->e_tbl_type);

        default:
            return MIDXBULDR_INV_ARG;
    }
    
}

#ifdef __cplusplus
}
#endif
