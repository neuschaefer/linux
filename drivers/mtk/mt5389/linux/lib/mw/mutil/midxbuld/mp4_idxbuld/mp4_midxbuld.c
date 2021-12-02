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
 * $RCSfile: mp4_midxbuld.c,v $
 * $Revision: #3 $ 1.0
 * $Date: 2012/06/20 $ 2004/11/26
 * $Author: weihua.chen $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media index builder public APIs  
 *---------------------------------------------------------------------------*/

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
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/mm_util_input.h"
#include "mutil/mm_util_memory.h"
#include "dbg/x_dbg.h"
#include "mutil/mm_util.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"
#include "mp4_midxbuld.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "../../mm_util_input.h"
#include "../../mm_util_memory.h"
#include "x_dbg.h"

#include "../../mm_util.h"

#include "x_lnk_list.h"
#include "mp4_midxbuld.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
const static UINT32 cui4_moov = 0x6D6F6F76; /* "moov" */
static UINT8        ui1_idx_range_id_last = 1;

#define MP4_MIDXBULD_RANGE_ID_MASK                          ((UINT32)0xff00)
#define MP4_MIDXBULD_TABLE_ID_MASK                          ((UINT32)0xff)

#define MP4_KEY_LEN_OF_SAMPLE_NUM                           100
#define MP4_SINGLE_RANGE_DURATION    (20*90000)

#if (OPTIMAZE_CODE == 1)
#define LITTLE_TO_BIG_DWORD(dwValue)   (((dwValue&0x000000FF)<<24)|     \
                                        ((dwValue&0x0000FF00)<< 8)|     \
                                        ((dwValue&0x00FF0000)>> 8)|     \
                                        ((dwValue&0xFF000000)>>24))
#define MP4_MIDXBULD_BUF_SIZE        (100*1024)      
#endif
#define ENABLE_CTTS 1
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#if (OPTIMAZE_CODE == 1)
/*-----------------------------------------------------------------------------
 * Name:  _mp4_midxbuld_mem_alloc
 *
 * Description: This function allocate a buffer
 *
 * Inputs:  
 *          pt_midxbuld_obj ,  specifies midxbuld object
 *          ui4_size ,         specifies buffer size
 *
 * Outputs: pv_buf , buffer address 
 *
 * Returns: TRUE    Success. The routine was successful and 
 *                           the operation has been completed.
 *          FALSE   Fail
 ----------------------------------------------------------------------------*/
INT32 _mp4_midxbuld_mem_alloc(MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                                    VOID**              pv_buf,
                                    UINT32             ui4_size)
{
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler = NULL;
    
    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    if(ui4_size <= MP4_MIDXBULD_BUF_SIZE)
    {
        *pv_buf = pt_mp4_handler->t_mem_buf.pv_mem_buffer;
    }
    else
    {
        _mm_util_mem_free_with_tag(pt_mp4_handler->t_mem_buf.pv_mem_buffer, 
                                   MM_MEM_MIDX_TAG);
        *pv_buf = _mm_util_mem_alloc_with_tag(ui4_size, MM_MEM_MIDX_TAG);
        if (*pv_buf == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        pt_mp4_handler->t_mem_buf.pv_mem_buffer = *pv_buf;
        pt_mp4_handler->t_mem_buf.ui4_mem_buf_size = ui4_size;
    }
    x_memset(pt_mp4_handler->t_mem_buf.pv_mem_buffer, 
            0, 
            pt_mp4_handler->t_mem_buf.ui4_mem_buf_size);
    return MIDXBULDR_OK;
}

INT32 _mp4_midxbuld_mem_free(MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                                    VOID*             pv_buf)
{
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler;
    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    
    x_memset(pt_mp4_handler->t_mem_buf.pv_mem_buffer, 
            0, 
            pt_mp4_handler->t_mem_buf.ui4_mem_buf_size);
    return MIDXBULDR_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name:  _mp4_midxbuld_alloc_table_id
 *
 * Description: This function allocate table id
 *
 * Inputs:  
 *          pt_range_obj ,  specifies range object
 *
 * Outputs: pui4_tbl_id , allocated table id 
 *
 * Returns: TRUE    Success. The routine was successful and 
 *                           the operation has been completed.
 *          FALSE   Fail
 ----------------------------------------------------------------------------*/
BOOL _mp4_midxbuld_alloc_table_id (
                        MP4_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                        UINT32*                  pui4_tbl_id )
{
    *pui4_tbl_id = MIDXBULD_NULL_ID;
    
    if (pt_range_obj->ui1_tbl_total == 0xff)
    {
        return FALSE;
    }
    
    *pui4_tbl_id = (pt_range_obj->ui4_range_id & 0xffffff00) +
                   (++pt_range_obj->ui1_tbl_total);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_midxbuld_alloc_range_id
 *
 * Description: This function alloc range id
 *
 * Inputs:  
 *          pt_midxbuld_obj ,  specifies the index builder object
 *          
 * Outputs: pui4_range_id : allocated range id 
 *
 * Returns: TRUE    Success. The routine was successful and 
 *                           the operation has been completed.
 *          FALSE   Fail
 ----------------------------------------------------------------------------*/
BOOL _mp4_midxbuld_alloc_range_id (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32*                  pui4_range_id )
{
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler = NULL;
    UINT8               ui1_key_range_id, ui1_range_id;
    UINT8               ui1_idx;
    BOOL                b_occupied;

    ui1_key_range_id = 0;
    ui1_range_id = 0;
    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    b_occupied = TRUE;

    *pui4_range_id = MIDXBULD_NULL_ID;

    if (pt_mp4_handler->pt_key_range_obj != NULL)
    {
        ui1_key_range_id = (UINT8)((pt_mp4_handler->pt_key_range_obj->ui4_range_id 
                            & MP4_MIDXBULD_RANGE_ID_MASK) >> 8);
    }
    else if (pt_mp4_handler->ui1_range_count > 0) 
    {
        /* key range should be allocated first */
        return FALSE;
    }

    if (pt_mp4_handler->ui1_range_count < 0xff)
    {
        while (b_occupied)
        {
            b_occupied = FALSE;
            for (ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_range_count; ui1_idx++)
            {
                if(ui1_idx >= MP4_MIDXBULD_RANGE_MAX)
                {
                    break;
                }
                ui1_range_id = (UINT8)((pt_mp4_handler->apt_range_obj[ui1_idx]
                            ->ui4_range_id & MP4_MIDXBULD_RANGE_ID_MASK) >> 8);
                if ((ui1_range_id == ui1_idx_range_id_last) ||
                    (ui1_key_range_id == ui1_idx_range_id_last))
                {
                    b_occupied = TRUE;
                    ui1_idx_range_id_last++;
                    break;
                }
            }
        }
    
        *pui4_range_id = (UINT32)ui1_idx_range_id_last;
    
        ui1_idx_range_id_last = (UINT8)((ui1_idx_range_id_last == 0xff) ? 
                                    1 : (ui1_idx_range_id_last + 1));
        
        if (ui1_key_range_id == ui1_idx_range_id_last)
        {
            ui1_idx_range_id_last = (UINT8)((ui1_idx_range_id_last == 0xff) ? 
                                        1 : (ui1_idx_range_id_last + 1));
        }
        
        *pui4_range_id = ((pt_midxbuld_obj->h_midxbuld & 0x0000ffff) << 16) +
                         (*pui4_range_id << 8);
    
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_midxbuld_free_table_obj
 *
 * Description: This function free table object
 *
 * Inputs:  
 *          pt_midxbuld_obj ,   specifies index builder object
 *          ui4_tbl_id ,        specifies table id
 *
 * Outputs: NULL 
 *
 * Returns: TRUE    Success. The routine was successful  and 
 *                           the operation has been completed.
 *          FALSE   Fail
 ----------------------------------------------------------------------------*/
static BOOL _mp4_midxbuld_free_table_obj (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_tbl_id)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_obj = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_last_obj = NULL;
    MIDXBULD_ELMT_TBL_OBJ_T*    pt_elmt_obj     = NULL;
    UINT8                       ui1_free_range_id, ui1_range_id;
    UINT8                       ui1_idx;
    BOOL                        b_id_match;

    /*Parameters check*/
    if (pt_midxbuld_obj == NULL)
    {
        return FALSE;
    }
    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL)
    {
        return FALSE;
    }

    /* initial local variable */
    pt_obj = NULL;
    pt_last_obj = NULL;
    b_id_match = FALSE;

    if ((pt_mp4_handler->pt_key_range_obj != NULL) && 
        (pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == ui4_tbl_id))
    {
        pt_obj = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj;
        pt_mp4_handler->pt_key_range_obj->ui1_tbl_total--;
        pt_mp4_handler->pt_key_range_obj->pt_tbl_obj = pt_obj->pt_next;
    }
    else
    {
        ui1_free_range_id = (UINT8)((ui4_tbl_id & MP4_MIDXBULD_RANGE_ID_MASK) >> 8);
        
        for (ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_range_count; ui1_idx++)
        {
            if(ui1_idx >= MP4_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            ui1_range_id = (UINT8)((pt_mp4_handler->apt_range_obj[ui1_idx]
                            ->ui4_range_id & MP4_MIDXBULD_RANGE_ID_MASK) >> 8);
            if (ui1_range_id == ui1_free_range_id)
            {
                pt_obj = pt_mp4_handler->apt_range_obj[ui1_idx]->pt_tbl_obj;
                if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_mp4_handler->apt_range_obj[ui1_idx]->pt_tbl_obj
                                                            = pt_obj->pt_next;
                    pt_mp4_handler->apt_range_obj[ui1_idx]->ui1_tbl_total--;
                }
                else
                {
                    pt_last_obj = pt_obj;
                    pt_obj = pt_obj->pt_next;
                }

                while (pt_obj != NULL && 
                       b_id_match == FALSE)
                {
                    if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                    {
                        b_id_match = TRUE;
                        pt_last_obj->pt_next = pt_obj->pt_next;
                        pt_mp4_handler->apt_range_obj[ui1_idx]->ui1_tbl_total--;
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

    if (pt_obj == NULL)
    {
        return FALSE;
    }

    pt_elmt_obj = pt_obj->t_elmt_tbl.pt_next;
    while(pt_elmt_obj != NULL)
    {
        pt_obj->t_elmt_tbl.pt_next = pt_elmt_obj->pt_next;
        _mm_util_mem_free_with_tag(pt_elmt_obj, MM_MEM_MIDX_TAG); 
        pt_elmt_obj = pt_obj->t_elmt_tbl.pt_next;       
    }
    if (pt_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry)
    {
        _mm_util_mem_free_with_tag(pt_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 
                                    MM_MEM_MIDX_TAG);
    }
    _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
            
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_midxbuld_free_range_obj
 *
 * Description: This function free range object resource.
 *
 * Inputs:  
 *          pt_midxbuld_obj ,  specifies the index builder object
 *          ui4_range_id ,     specifies range id
 *
 * Outputs: NULL 
 *
 * Returns: TRUE    Success. The routine was successful and 
 *                           the operation has been completed.
 *          FALSE   Fail
 ----------------------------------------------------------------------------*/
BOOL _mp4_midxbuld_free_range_obj (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id)
{
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler = NULL;
    MP4_IDXBULD_RANGE_OBJ_T*        pt_range_obj = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj = NULL;
    PIFF_BOX_INET_INFO_T*           pt_moof_info    = NULL;
    UINT32                          ui4_tbl_id;
    UINT8                           ui1_idx;
    BOOL                            b_id_match;

    /*Parameters check*/
    if (pt_midxbuld_obj == NULL)
    {
        return FALSE;
    }
    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    pt_moof_info = pt_mp4_handler->piff_inet_moof_info;

    /* initial local variable */
    pt_range_obj = NULL;
    pt_tbl_obj = NULL;
    ui4_tbl_id = MIDXBULD_NULL_ID;
    ui1_idx = 0;
    b_id_match = FALSE;

    if ((pt_mp4_handler->pt_key_range_obj != NULL) && 
        (pt_mp4_handler->pt_key_range_obj->ui4_range_id == ui4_range_id))
    {
        b_id_match = TRUE;
        pt_range_obj = pt_mp4_handler->pt_key_range_obj;
    }
    else if (pt_mp4_handler->ui1_range_count > 0)
    {
        for (ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_range_count; ui1_idx++)
        {
            if(ui1_idx >= MP4_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            if (pt_mp4_handler->apt_range_obj[ui1_idx]->ui4_range_id
                == ui4_range_id)
            {
                b_id_match = TRUE;
                pt_range_obj = pt_mp4_handler->apt_range_obj[ui1_idx];
                break;
            }
        }
    }
    
    if (FALSE == b_id_match)
    {
        return FALSE;
    }

    if (pt_range_obj)
    {
        while (pt_range_obj->pt_tbl_obj != NULL)
        {
            pt_tbl_obj = pt_range_obj->pt_tbl_obj;
            ui4_tbl_id = pt_tbl_obj->ui4_tbl_id;
            if(!_mp4_midxbuld_free_table_obj(pt_midxbuld_obj, ui4_tbl_id))
            {
                break;
            }
        }

        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);

        if (((pt_mp4_handler->pt_key_range_obj != NULL) && 
                (pt_mp4_handler->pt_key_range_obj->ui4_range_id != ui4_range_id))
           ||(  (pt_moof_info != NULL) && 
                (pt_moof_info->b_is_inet_data)))
        {
            while ((ui1_idx + 1) < pt_mp4_handler->ui1_range_count)
            {
                if(ui1_idx+1 >= MP4_MIDXBULD_RANGE_MAX)
                {
                    break;
                }
                pt_mp4_handler->apt_range_obj[ui1_idx] = 
                    pt_mp4_handler->apt_range_obj[ui1_idx + 1];
                pt_mp4_handler->apt_range_obj[++ui1_idx] = NULL;
            }
            pt_mp4_handler->ui1_range_count--;
        }

        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_free_key_ext_tbl
 *
 * Description: This function free key extension table
 *
 * Inputs:  
 *          pt_key_ext_head ,  specifies the key ext table head
 *          
 *
 * Outputs: NULL 
 *
 * Returns: 
 *          
 ----------------------------------------------------------------------------*/
VOID _mp4_free_key_ext_tbl(MP4_IDXBULD_KEY_EXT_TBL_T* pt_key_ext_head)
{
    MP4_IDXBULD_KEY_EXT_TBL_T* pt_key_ext_tbl = NULL;
        
    while (pt_key_ext_head != NULL)
    {
        pt_key_ext_tbl = pt_key_ext_head;
        pt_key_ext_head = pt_key_ext_head->pt_next;
#if (ENABLE_CTTS == 1)
        if(pt_key_ext_tbl->t_ctts_buf.pt_ctts_item_buf != NULL)
        {
            _mm_util_mem_free_with_tag(pt_key_ext_tbl->t_ctts_buf.pt_ctts_item_buf, 
                                        MM_MEM_MIDX_TAG);
            pt_key_ext_tbl->t_ctts_buf.pt_ctts_item_buf = NULL;
        }
#endif
        if (pt_key_ext_tbl->pt_key_entry != NULL)
        {
            _mm_util_mem_free_with_tag(pt_key_ext_tbl->pt_key_entry, 
                                        MM_MEM_MIDX_TAG);
        }
        if (pt_key_ext_tbl->pt_key_ext != NULL)
        {
            _mm_util_mem_free_with_tag(pt_key_ext_tbl->pt_key_ext, 
                                        MM_MEM_MIDX_TAG);
        }
        if( pt_key_ext_tbl != NULL)
        {
            _mm_util_mem_free_with_tag(pt_key_ext_tbl, MM_MEM_MIDX_TAG);
        }
    }
}


INT32 _mp4_reorder_key_tbl(MP4_IDXBULD_ELMT_TBL_OBJ_T*  pt_tbl_obj, 
                           MP4_IDXBULD_KEY_EXT_TBL_T*   pt_key_ext_t)
{
    UINT32                              ui4_i       = 0;
    UINT32                              ui4_j       = 0;
    UINT32                              ui4_key_num = 0;
    UINT32                              ui4_4G_start= 0;
    UINT32                              ui4_4G_end  = 0;    
    MIDXBULD_ELMT_STRM_ENTRY_T*         pDst        = NULL;
    MP4_MIDXBULD_ELMT_STRM_ENTRY_T*     pSrc        = NULL;
    MIDXBULD_ELMT_TBL_OBJ_T*            pObj        = NULL;
    UINT32                              ui4_cur_4G_idx = 0; 
    
    
    if((pt_tbl_obj == NULL) || (pt_key_ext_t == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    
    ui4_key_num = pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
    ui4_4G_end  = ui4_key_num - 1;
    pSrc        = pt_key_ext_t->pt_key_entry;
    pDst        = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    pObj        = &(pt_tbl_obj->t_elmt_tbl);

    if(ui4_key_num == 0)
    {
        pObj->ui4_number_of_entry = 0;
        return MIDXBULDR_OK;
    }
    
    pObj->ui8_base_offset = 0;
    while(pObj != NULL)
    {
        for(; ui4_i<ui4_key_num; ui4_i++)
        {
            if(ui4_cur_4G_idx != (UINT32)(pSrc[ui4_i].ui8_relative_offset>>32))
            {
                ui4_cur_4G_idx = (UINT32)(pSrc[ui4_i].ui8_relative_offset>>32);
                ui4_4G_end  = ui4_i - 1;
                break;
            }
        }
        for(ui4_j=ui4_4G_start; ui4_j<=ui4_4G_end; ui4_j++)
        {
            pDst[ui4_j].ui4_pts               = pSrc[ui4_j].ui4_pts;
            pDst[ui4_j].ui4_relative_offset   = (UINT32)pSrc[ui4_j].ui8_relative_offset;
            pDst[ui4_j].ui4_size              = pSrc[ui4_j].ui4_size;
            #if 0//ndef __KERNEL__
            if((ui4_j <= ui4_4G_start + 5) || (ui4_j + 5 > ui4_4G_end))
            {
                x_dbg_stmt("=====key baseofst:%#llx, pts: 0x%x, ofst:0x%x, size:0x%x=====\r\n", 
                pObj->ui8_base_offset, 
                pDst[ui4_j].ui4_pts,
                pDst[ui4_j].ui4_relative_offset,
                pDst[ui4_j].ui4_size);
            }
            #endif
        }
        pObj->ui4_number_of_entry = ui4_4G_end - ui4_4G_start + 1;       
        if(ui4_4G_end == ui4_key_num - 1)
        {
            break;
        } 
        
        ui4_4G_start = ui4_4G_end + 1;
        ui4_4G_end   = ui4_key_num - 1;
        pObj->pt_next = (MIDXBULD_ELMT_TBL_OBJ_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
        if (pObj->pt_next == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pObj->pt_next, 0, sizeof(MIDXBULD_ELMT_TBL_OBJ_T));
        
        pObj->pt_next->e_tbl_type = pt_tbl_obj->t_elmt_tbl.e_tbl_type;
        pObj->pt_next->ui8_base_offset = ((UINT64)ui4_cur_4G_idx<<32);
        pObj->pt_next->u_fmt.t_es.pt_idx_entry = &pDst[ui4_4G_start];
        
        pObj = pObj->pt_next;
    }
    return MIDXBULDR_OK;
}

INT32 mp4_midxbuld_pcm_audio_sample_table_build(
                            MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                            MP4_IDXBULD_TRACK_T*    pt_cur_track)
{
    INT32                       i4_ret              = 0;
    UINT32                      ui4_tmp             = 0;
    UINT32                      ui4_entry_count     = 0;
    UINT32                      ui4_sample_count    = 0;
    UINT32                      ui4_cur_sample_num  = 0;
    UINT32                      ui4_read_size       = 0;
    UINT32                      ui4_i               = 0;
    input4bytes_fct             pf_input4           = NULL;
    setpos_fct                  pf_set_pos          = NULL;
    copybytes_fct               pf_copybytes        = NULL; 
    MP4_RAW_STSC_TBL_T*         pr_stsc_buf         = NULL;
    MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T* pt_tmp_size_dur = NULL;
     
    if((pt_midxbuld_obj == NULL) || (pt_cur_track == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    
    pf_input4       = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos      = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybytes    = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;

    /* Check PCM data attribute first, only support sample_size==1 && sample_dur==1 case*/
    /* seek to STTS  box to get sample duration */
    pf_set_pos(pt_midxbuld_obj, 
                (INT64)(pt_cur_track->t_stts.ui8_box_offset + 20), 
                FM_SEEK_BGN);
    pf_input4(pt_midxbuld_obj, &ui4_tmp);
    if(ui4_tmp != 1)/* Not PCM ??? */
    {
        return MIDXBULDR_INTERNAL_ERR;
    }    

    /* seek to STSZ  box to get sample size */
    pf_set_pos(pt_midxbuld_obj, 
                (INT64)(pt_cur_track->t_stsz.ui8_box_offset + 12), 
                FM_SEEK_BGN);
    pf_input4(pt_midxbuld_obj, &ui4_tmp);
    if(ui4_tmp != 1)/* Not PCM ??? */
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pf_input4(pt_midxbuld_obj, &ui4_sample_count);

    do
    {
        /* seek to STSC  box to get entry count */
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(pt_cur_track->t_stsc.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
        pf_input4(pt_midxbuld_obj, &ui4_entry_count);


        /* Allocate temp buffer and read data to buffer*/
        pr_stsc_buf = (MP4_RAW_STSC_TBL_T*)_mm_util_mem_alloc_with_tag(
                                                sizeof(MP4_RAW_STSC_TBL_T) * ui4_entry_count, 
                                                MM_MEM_MIDX_TAG);
        if (pr_stsc_buf == NULL)
        {
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pr_stsc_buf, 0, sizeof(MP4_RAW_STSC_TBL_T) * ui4_entry_count);

        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                              pr_stsc_buf, 
                              sizeof(MP4_RAW_STSC_TBL_T)*ui4_entry_count, 
                              sizeof(MP4_RAW_STSC_TBL_T)*ui4_entry_count, 
                              &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            i4_ret = MIDXBULDR_INTERNAL_ERR;
            break;
        }

        /* Reorder buffer data */
        pt_cur_track->t_pcm_sam_inf.ui4_size_dur_num = ui4_entry_count;
        pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur = 
            (MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T*)_mm_util_mem_alloc_with_tag(
                                                sizeof(MP4_PCM_AUDIO_SAMPLE_SIZE_DUR_INFO_T) * ui4_entry_count, 
                                                MM_MEM_MIDX_TAG);
        if (pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur == NULL)
        {
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break;
        }

        pt_tmp_size_dur = pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur;
        pt_tmp_size_dur[0].ui4_sample_start_idx     
            = LITTLE_TO_BIG_DWORD(pr_stsc_buf[0].ui4_first_chunk);
        pt_tmp_size_dur[0].ui4_sample_size_duration 
            = LITTLE_TO_BIG_DWORD(pr_stsc_buf[0].ui4_samples_per_chunk);
        for(ui4_i=1; ui4_i<ui4_entry_count; ui4_i++)
        {
            pt_tmp_size_dur[ui4_i].ui4_sample_start_idx     
                = LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_i].ui4_first_chunk);
            pt_tmp_size_dur[ui4_i-1].ui4_sample_count       
                = pt_tmp_size_dur[ui4_i].ui4_sample_start_idx - LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_i-1].ui4_first_chunk);
            pt_tmp_size_dur[ui4_i].ui4_sample_size_duration 
                = LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_i].ui4_samples_per_chunk);
            ui4_cur_sample_num += pt_tmp_size_dur[ui4_i-1].ui4_sample_count
                                    *pt_tmp_size_dur[ui4_i-1].ui4_sample_size_duration;
        }        
        pt_tmp_size_dur[ui4_entry_count-1].ui4_sample_count
                = (ui4_sample_count - ui4_cur_sample_num)/pt_tmp_size_dur[ui4_entry_count-1].ui4_sample_size_duration;

        
        /* Get audio sample offset from STCO */        
        /* seek to STCO  box to get entry count */
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(pt_cur_track->t_stco.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
        pf_input4(pt_midxbuld_obj, &ui4_entry_count);
        pt_cur_track->t_pcm_sam_inf.ui4_total_sample_num= ui4_entry_count;
        pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf      = 
            (UINT32*)_mm_util_mem_alloc_with_tag(sizeof(UINT32) * ui4_entry_count, 
                                                 MM_MEM_MIDX_TAG);
        if (pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf == NULL)
        {
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        
        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                              pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf, 
                              sizeof(UINT32)*ui4_entry_count, 
                              sizeof(UINT32)*ui4_entry_count, 
                              &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        for(ui4_i=0; ui4_i<ui4_entry_count; ui4_i++)
        {
            pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf[ui4_i]
                = LITTLE_TO_BIG_DWORD(pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf[ui4_i]);
        }        
    }while(0);

    if(pr_stsc_buf != NULL)
    {
        _mm_util_mem_free_with_tag(pr_stsc_buf, MM_MEM_MIDX_TAG);
        pr_stsc_buf = NULL;
    }
    if(i4_ret != MIDXBULDR_OK)
    {
        if(pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur != NULL)
        {
            _mm_util_mem_free_with_tag(pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur, 
                                       MM_MEM_MIDX_TAG);
            pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur = NULL;
        }
        
        if(pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf != NULL)
        {
            _mm_util_mem_free_with_tag(pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf, 
                                       MM_MEM_MIDX_TAG);
            pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf = NULL;
        }
        return i4_ret;
    }
    
    return MIDXBULDR_OK;
}


INT32 mp4_midxbuld_pcm_audio_sample_indexing(
                            MIDXBULD_OBJ_T*                 pt_midxbuld_obj,
                            MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj,
                            MP4_IDXBULD_TRACK_T*            pt_cur_track)
{
    UINT32                      ui4_i                   = 0;
    UINT32                      ui4_j                   = 0;
    UINT64                      ui8_curr_duration_sum   = 0;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry            = NULL;

    if((pt_midxbuld_obj == NULL) ||
       (pt_tbl_obj      == NULL) ||
       (pt_cur_track    == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }

    for(ui4_i = 0; ui4_i < pt_cur_track->t_pcm_sam_inf.ui4_size_dur_num; ui4_i++)
    {
        if(pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx
                < pt_tbl_obj->ui4_start_sample)
        {
            if(pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx + 
                        pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count
                > pt_tbl_obj->ui4_start_sample)
            {
                ui8_curr_duration_sum += (pt_tbl_obj->ui4_start_sample
                                            - pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx)
                                        * pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration;
                break;
            }
            ui8_curr_duration_sum += pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count
                                      * pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration;
        }
        else
        {
            break;
        }
    }
                
    pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    if(pt_idx_entry == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    for(ui4_j=pt_tbl_obj->ui4_start_sample; ui4_j<=pt_tbl_obj->ui4_end_sample; ui4_j++)
    {
        if(ui4_j > pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx
                    + pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count)
        {
            ui4_i++;
        }
        pt_idx_entry->ui4_relative_offset = pt_cur_track->t_pcm_sam_inf.paui4_ofst_buf[ui4_j - 1];
        pt_idx_entry->ui4_size  = pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration *
                                  pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_sample_sz;
        pt_idx_entry->ui4_pts   = (UINT32)_mm_div64(ui8_curr_duration_sum * MIDXBULD_SYSTEM_CLOCK_FREQUENCY,
                                                    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale, 
                                                    NULL);  
#if 0//ndef __KERNEL__
        x_dbg_stmt("====pts = %d , ofst=0x%x, size=%d \r\n", 
                  pt_idx_entry->ui4_pts,
                  pt_idx_entry->ui4_relative_offset,
                  pt_idx_entry->ui4_size);    
#endif
        pt_idx_entry++;
        ui8_curr_duration_sum += pt_cur_track->t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration;
    }
    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->ui4_tbl_size;  
    
#ifndef __KERNEL__
        x_dbg_stmt("PCM audio indexing ui4_number_of_entry = %d \r\n", 
                  pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry);
        x_dbg_stmt("***************** range info start*********************\r\n");
        x_dbg_stmt("start    pts: 0x%8x\r\n", pt_tbl_obj->ui4_start_pts);
        x_dbg_stmt("end      pts: 0x%8x\r\n", pt_tbl_obj->ui4_end_pts);
        x_dbg_stmt("start tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[0].ui4_pts);
        x_dbg_stmt("end  tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_pts);
        x_dbg_stmt("start offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[0].ui4_relative_offset);
        x_dbg_stmt("start sample: 0x%8x\r\n", pt_tbl_obj->ui4_start_sample);
        x_dbg_stmt("end   sample: 0x%8x\r\n", pt_tbl_obj->ui4_end_sample);
        x_dbg_stmt("end   offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_relative_offset);
        x_dbg_stmt("***************** range info end*********************\r\n");
#endif  

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_indexing
 *
 * Description: This function builds the index table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_indexing(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                            UINT32*         pui4_range_id)
{
    INT32                       i4_ret;
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler = NULL;
    input4bytes_fct             pf_input4;
    setpos_fct                  pf_set_pos;
    getpos_fct                  pf_get_pos;
    UINT8                       ui1_idx;
    UINT32                      ui4_i, ui4_j;
    MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_pre_entry = NULL;
    
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_t = NULL;
    MP4_IDXBULD_TRACK_T         t_cur_track = {0};

    UINT32 ui4_range_start_pts, ui4_range_end_pts;
    UINT32 ui4_range_start_sample, ui4_range_end_sample;
    UINT32 ui4_key_num, ui4_start_key_idx;
    
    UINT32 ui4_count_init, ui4_curr_pts;
    UINT32 ui4_sample_count, ui4_sample_duration;
    UINT64 ui8_time, ui8_pos, ui8_curr_duration_sum;
    
    UINT32 ui4_chunk_count, ui4_entry_count;
    UINT32 ui4_curr_entry, ui4_curr_sample, ui4_curr_chunk, ui4_curr_chunk_sample;
    UINT32 ui4_first_chunk, ui4_next_first_chunk;
    UINT32 ui4_samples_per_chunk;
    
#if (ENABLE_CTTS == 1)
    UINT32              ui4_cur_sample_idx = 0;
    MP4_RAW_CTTS_TBL_T* pt_ctts_item_buf = NULL;
#endif

#if (OPTIMAZE_CODE == 1)
    UINT32              ui4_read_size;
    MP4_RAW_STTS_TBL_T  t_stts_item;
    MP4_RAW_STSC_TBL_T  t_stsc_item;
    UINT32*             paui4_stsz_buf = NULL;
    copybytes_fct       pf_copybytes;
#else
    UINT32              ui4_sample_size;
#endif
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }

    /* initial local variable */
    pt_tbl_obj = NULL;
    ui4_start_key_idx = 0;
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    *pui4_range_id = pt_mp4_handler->pt_curr_range_obj->ui4_range_id;
    
    if (pt_mp4_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
    {
        return MIDXBULDR_OK;
    }
    if (pt_mp4_handler->pt_key_range_obj->b_index_is_valid == TRUE &&
        pt_mp4_handler->ui1_range_count == 0)
    {
        return MIDXBULDR_OK;
    }
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    }

    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
#if (OPTIMAZE_CODE == 1)
    pf_copybytes = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
#endif

    /* set position to the source begin */
    pf_set_pos(pt_midxbuld_obj, 
                (INT64)pt_mp4_handler->t_cur_file_info.ui8_file_moov_pos, 
                FM_SEEK_BGN);

    /* check the source size */
    i4_ret = pf_input4(pt_midxbuld_obj, &ui4_i);
    if(i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_FILE_ERR;
    }
    if ((UINT32)pt_mp4_handler->t_cur_file_info.ui8_file_moov_size != ui4_i)
    {
        return MIDXBULDR_INV_ARG;
    }

    /* check the source start code */
    i4_ret = pf_input4(pt_midxbuld_obj, &ui4_i);
    if(i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_FILE_ERR;
    }
    if (cui4_moov != ui4_i)
    {
        return MIDXBULDR_FMT_NOT_SUPPORT;
    }

    ui4_key_num = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size;

    /* key table indexing */
    if (pt_mp4_handler->pt_curr_range_obj->b_is_key_frame)
    {
        pt_tbl_obj = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj;
        /* get related key ext table for key table */
        pt_key_ext_t = pt_mp4_handler->pt_key_ext_tbl;
        while (pt_key_ext_t != NULL)
        {
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id
                    == pt_key_ext_t->ui4_track_id)
            {
                break;
            }
            pt_key_ext_t = pt_key_ext_t->pt_next;
        }
        if((pt_key_ext_t == NULL)
          || !pt_tbl_obj->ui1_is_key_exist)
        {
            if((ui4_key_num == 0)/*no key table exist*/
                || !pt_tbl_obj->ui1_is_key_exist)
            {
                pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
                pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
                pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
                return MIDXBULDR_OK;
            }
            else
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
        }
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = ui4_key_num;  
        DBG_INFO(("indexing key number = %d \r\n", ui4_key_num));
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset = pt_key_ext_t->ui8_base_offset;
        pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
#if 0
        x_memcpy(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 
                 pt_key_ext_t->pt_key_entry, 
                 sizeof(MIDXBULD_ELMT_STRM_ENTRY_T)*ui4_key_num);
#else
        _mp4_reorder_key_tbl(pt_tbl_obj, pt_key_ext_t);
#endif
        
        /* todo: need memory copy the entry to current range object?? */           
        return MIDXBULDR_OK;
    }
 
    /* non-key table indexing */
    pt_tbl_obj = pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj;
    while (pt_tbl_obj != NULL)
    {
        UINT32  ui4_ctts_i       = 0;   
        if (pt_midxbuld_obj->b_abort_flag == TRUE)
        {
            return MIDXBULDR_ABORT_INDEXING;
        }
        if(pt_tbl_obj->ui4_tbl_size == 0)
        {
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj = pt_tbl_obj->pt_next;
            continue;
        }
        /* get range start / end pts */
        ui4_range_start_pts = pt_tbl_obj->ui4_start_pts;
        ui4_range_end_pts  =  pt_tbl_obj->ui4_end_pts;
        /* get range start / end frame(sample) */
        ui4_range_start_sample = pt_tbl_obj->ui4_start_sample;
        ui4_range_end_sample  = pt_tbl_obj->ui4_end_sample;
        /* get curr track info by pt_describ stream id */
        for(ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; ui1_idx++)
        {
            if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
            {
                return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
            }
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id == 
                pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
            {
                 t_cur_track = pt_mp4_handler->t_track_info_box[ui1_idx];
                 break;
            }
        }
        if(ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        if((pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) && 
                     pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.b_is_pcm_aud)
        {
            mp4_midxbuld_pcm_audio_sample_indexing( pt_midxbuld_obj,
                                                    pt_tbl_obj,
                                                    &t_cur_track);                      
            pt_tbl_obj = pt_tbl_obj->pt_next;
            continue;
        }
        
        /* get related key ext table */
        pt_key_ext_t = pt_mp4_handler->pt_key_ext_tbl;
        while (pt_key_ext_t != NULL)
        {
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id
                    == pt_key_ext_t->ui4_track_id)
            {
                break;
            }
            pt_key_ext_t = pt_key_ext_t->pt_next;
        }
        if (pt_key_ext_t == NULL)
        {
            if(ui4_key_num != 0)
            {
                break;
            }
            return MIDXBULDR_INTERNAL_ERR;
        }
           
        if(!pt_mp4_handler->t_mln_moov_info.b_is_moov_empty && 
            pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)
        {
            ui4_key_num = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_key_cnt;
        }
        else
        {
            ui4_key_num = pt_key_ext_t->ui4_key_num;
        }
           
        /* cal start key ext index by start pts */
        for(ui4_i = ui4_key_num; ui4_i > 0; ui4_i--)
        {
            if (pt_key_ext_t->pt_key_entry[ui4_i-1].ui4_pts <= ui4_range_start_pts)
            {
                ui4_start_key_idx = ui4_i - 1;
                break;
            }
        }

        /* cal pts */
        if(ui4_key_num == 0)/*no key table exist*/
        {
            ui4_count_init = 0;
            ui4_curr_pts = 0;
            ui8_curr_duration_sum = 0;
            /* seek to 'time to sample'  box to cal pts */
            pf_set_pos(pt_midxbuld_obj, 
                       (INT64)(t_cur_track.t_stts.ui8_box_offset + 16), 
                       FM_SEEK_BGN);
        }
        else
        {
            ui4_count_init
                = pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_key_ext_sample
                    - pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                        .ui4_stts_entry_sample;
            ui4_curr_pts = pt_key_ext_t->pt_key_entry[ui4_start_key_idx].ui4_pts;
            ui8_curr_duration_sum = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                    .ui8_key_ext_duration_sum;
            /* seek to 'time to sample'  box to cal pts */
            pf_set_pos(pt_midxbuld_obj, 
                       (INT64)(t_cur_track.t_stts.ui8_box_offset + 16
                            + 8*pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                .ui4_stts_entry), 
                       FM_SEEK_BGN);            
        }
               
        /* init current entry */
        pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        
#if (ENABLE_CTTS == 1)
        if(t_cur_track.t_ctts.b_is_box_exist)
        {
            pt_ctts_item_buf = pt_key_ext_t->t_ctts_buf.pt_ctts_item_buf;
            ui4_cur_sample_idx = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                            .ui4_key_ext_sample;
        }
#endif

        /* check the dividend can't equal zero */
        if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale == 0)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        while (ui4_curr_pts <= ui4_range_end_pts)
        {
            pf_get_pos(pt_midxbuld_obj, &ui8_pos);
            if(ui8_pos >= t_cur_track.t_stts.ui8_box_offset
                                + t_cur_track.t_stts.ui4_box_size)
            {/*Reach to STTS end  */
                break;
            }
#if (OPTIMAZE_CODE == 1)
            i4_ret = pf_copybytes(pt_midxbuld_obj, 
                        &t_stts_item, 
                        sizeof(MP4_RAW_STTS_TBL_T), 
                        sizeof(MP4_RAW_STTS_TBL_T), 
                        &ui4_read_size);
            if(i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
            ui4_sample_count = LITTLE_TO_BIG_DWORD(t_stts_item.ui4_sample_count);
            ui4_sample_duration
                        = LITTLE_TO_BIG_DWORD(t_stts_item.ui4_sample_duration);
#else
            i4_ret = pf_input4(pt_midxbuld_obj, &ui4_sample_count);
            if(i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
            i4_ret = pf_input4(pt_midxbuld_obj, &ui4_sample_duration);
            if(i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
#endif
            for(ui4_i = ui4_count_init; ui4_i < ui4_sample_count; ui4_i++)
            {
                ui8_time = ui8_curr_duration_sum * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
#if (ENABLE_CTTS == 1)
                if(t_cur_track.t_ctts.b_is_box_exist)
                {                                    
                    INT64   i8_ctts_ofst    = 0;
                    INT64   i8_cur_dur_sum  = 0;
                    for(;ui4_ctts_i < pt_key_ext_t->t_ctts_buf.ui4_ctts_entry_num; ui4_ctts_i ++)
                    {
                        if( ui4_cur_sample_idx <= pt_ctts_item_buf[ui4_ctts_i].ui4_sample_count)
                        {                    
                            break;                    
                        }
                    }                    
                    ui4_cur_sample_idx++;
                    
                    i8_ctts_ofst    = (INT64)((INT32)pt_ctts_item_buf[ui4_ctts_i].ui4_sample_offset); 
                    i8_cur_dur_sum  = (INT64)ui8_curr_duration_sum;

                    if((i8_ctts_ofst + i8_cur_dur_sum) < 0)
                    {
                        ui8_time = (UINT64)i8_cur_dur_sum;
                    }
                    else
                    {
                        ui8_time = (UINT64)(i8_ctts_ofst + i8_cur_dur_sum);
                    }
                        
                    ui8_time = ui8_time * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
                }
#endif
                ui4_curr_pts = (UINT32)_mm_div64(
                            ui8_time,
                            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale, 
                            NULL);                            
                                
#if (ENABLE_CTTS == 1)/*To avoid last-N entry pts > end pts issue*/
                if(t_cur_track.t_ctts.b_is_box_exist == FALSE)
#endif
                {
                    if (ui4_curr_pts >= ui4_range_start_pts)
                    {
                        /* get current entry pts */
                        pt_idx_entry->ui4_pts = ui4_curr_pts;
                        pt_idx_entry++;
                        if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                            pt_tbl_obj->ui4_tbl_size))
                        {
                            break;
                        }
                    }
                }
                else
                {
                    /* get current entry ptes */
                    pt_idx_entry->ui4_pts = ui4_curr_pts;
                    pt_idx_entry++;
                    if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                        pt_tbl_obj->ui4_tbl_size))
                    {
                        break;
                    }
                }
                ui8_curr_duration_sum += (UINT64)ui4_sample_duration;
            }
            if (ui4_i < ui4_sample_count)
            {
                break;
            }
            ui4_count_init = 0;
        }
        pt_mp4_handler->ui1_curr_buld_progress
            += 70/3/pt_mp4_handler->ui1_valid_track_cnt;/*-- 30%+ 70/(3*cnt)--*/

        /* ============= caculate sample size ==================== */
        if (t_cur_track.ui4_default_sample_size == 0)
        {
            pf_set_pos(pt_midxbuld_obj, 
                       (INT64)(t_cur_track.t_stsz.ui8_box_offset + 20
                            + 4*(ui4_range_start_sample-1)), 
                       FM_SEEK_BGN);
            if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(pt_midxbuld_obj,
                                       (VOID*)&paui4_stsz_buf,
                                       sizeof(UINT32)*(ui4_range_end_sample
                                                        - ui4_range_start_sample + 1)))
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            i4_ret = pf_copybytes(pt_midxbuld_obj, 
                            paui4_stsz_buf, 
                            sizeof(UINT32)*(ui4_range_end_sample
                                                     - ui4_range_start_sample + 1), 
                            sizeof(UINT32)*(ui4_range_end_sample
                                                     - ui4_range_start_sample + 1), 
                            &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
        }
        
        for(ui4_i = 0; 
            ui4_i < (ui4_range_end_sample - ui4_range_start_sample + 1); 
            ui4_i++)
        {
            pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_i;
            if(pt_idx_entry == NULL)/*no key table exist*/
            {
                break;
            }
            if (t_cur_track.ui4_default_sample_size == 0)
            {
                pt_idx_entry->ui4_size = LITTLE_TO_BIG_DWORD(paui4_stsz_buf[ui4_i]);
            }
            else
            {
                pt_idx_entry->ui4_size = t_cur_track.ui4_default_sample_size;
            }       
        }
        if (t_cur_track.ui4_default_sample_size == 0)
        {
            _mp4_midxbuld_mem_free(pt_midxbuld_obj, paui4_stsz_buf);
        }

        pt_mp4_handler->ui1_curr_buld_progress
            += 70/3/pt_mp4_handler->ui1_valid_track_cnt;/*-- 30%+ 70/(3*cnt)--*/

        /* =============== cal sample offset ================ */ 
        if(t_cur_track.t_stco.b_is_box_exist)
        {
            pf_set_pos(pt_midxbuld_obj, 
                        (INT64)(t_cur_track.t_stco.ui8_box_offset + 12), 
                        FM_SEEK_BGN);
        }
        else if(t_cur_track.t_co64.b_is_box_exist)
        {
            pf_set_pos(pt_midxbuld_obj, 
                        (INT64)(t_cur_track.t_co64.ui8_box_offset + 12), 
                        FM_SEEK_BGN);
        }
        /* get chunk count */  
        i4_ret = pf_input4(pt_midxbuld_obj, &ui4_chunk_count);
        if(i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_FILE_ERR;
        }
        
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_stsc.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
        /* get sample to chunk entry count */
        i4_ret = pf_input4(pt_midxbuld_obj, &ui4_entry_count);
        if(i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_FILE_ERR;
        }
        if(ui4_key_num == 0)/*no key table exist*/
        {
            ui4_curr_entry = 0;
            ui4_curr_sample = 0;
        }
        else
        {
            ui4_curr_entry = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                        .ui4_stsc_entry;
            ui4_curr_sample = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                        .ui4_stsc_entry_sample;
        }
        ui4_curr_chunk_sample = ui4_curr_sample;
    
        while (ui4_curr_sample <= ui4_range_end_sample)
        {
            pf_set_pos(pt_midxbuld_obj, 
                       (INT64)(t_cur_track.t_stsc.ui8_box_offset + 16
                                + 12 * ui4_curr_entry), 
                       FM_SEEK_BGN);
#if (OPTIMAZE_CODE == 1)
            i4_ret = pf_copybytes(pt_midxbuld_obj, 
                        &t_stsc_item, 
                        sizeof(MP4_RAW_STSC_TBL_T), 
                        sizeof(MP4_RAW_STSC_TBL_T), 
                        &ui4_read_size);
            if(i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
            ui4_first_chunk = LITTLE_TO_BIG_DWORD(t_stsc_item.ui4_first_chunk);
            ui4_samples_per_chunk = LITTLE_TO_BIG_DWORD(t_stsc_item.ui4_samples_per_chunk);
#else
            i4_ret = pf_input4(pt_midxbuld_obj, &ui4_first_chunk);
            if(i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
            i4_ret = pf_input4(pt_midxbuld_obj, &ui4_samples_per_chunk);
            if(i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
            /*Just for skip 4 bytes*/
            i4_ret = pf_input4(pt_midxbuld_obj, &ui4_i);
            if(i4_ret != INPUTR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
#endif

            if (ui4_curr_entry < ui4_entry_count - 1)
            {
                i4_ret = pf_input4(pt_midxbuld_obj, &ui4_next_first_chunk);
                if(i4_ret != INPUTR_OK)
                {
                    return MIDXBULDR_FILE_ERR;
                }
            }
            else
            {
                ui4_next_first_chunk = ui4_chunk_count + 1;
            }

            for(ui4_curr_chunk = ui4_first_chunk; 
                ui4_curr_chunk < ui4_next_first_chunk; 
                ui4_curr_chunk++)
            {
                /*navy*/
                for(ui4_i = 0; ui4_i < ui4_samples_per_chunk; ui4_i++)
                {
                    if (pt_midxbuld_obj->b_abort_flag == TRUE)
                    {
                        return MIDXBULDR_ABORT_INDEXING;
                    }
                    if (ui4_curr_sample >= ui4_range_start_sample)
                    {
                        pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
                        if(pt_idx_entry == NULL)/*no key table exist*/
                        {
                            break;
                        }
                        pt_idx_entry += (ui4_curr_sample - ui4_range_start_sample);
                        /* seek to chunk offset box */
                        if ((ui4_i == 0) || (ui4_curr_sample == ui4_range_start_sample))
                        {
                            UINT32  ui4_high = 0;
                            UINT32  ui4_low  = 0;
                            UINT64  ui8_tmp  = 0;
                            if(t_cur_track.t_stco.b_is_box_exist)
                            {
                                pf_set_pos(pt_midxbuld_obj, 
                                       (INT64)(t_cur_track.t_stco.ui8_box_offset + 16
                                                + 4*(ui4_curr_chunk - 1)), 
                                       FM_SEEK_BGN);
                                i4_ret = pf_input4(pt_midxbuld_obj, 
                                                &ui4_low);
                                if(i4_ret != INPUTR_OK)
                                {
                                    return MIDXBULDR_FILE_ERR;
                                }
                            }
                            else if(t_cur_track.t_co64.b_is_box_exist)
                            {
                                pf_set_pos(pt_midxbuld_obj, 
                                       (INT64)(t_cur_track.t_co64.ui8_box_offset + 16
                                                + 8*(ui4_curr_chunk - 1) + 0), 
                                       FM_SEEK_BGN);
                                i4_ret = pf_input4(pt_midxbuld_obj, &ui4_high);
                                i4_ret = pf_input4(pt_midxbuld_obj, &ui4_low);
                            }
                            ui8_tmp = (((UINT64)ui4_high)<<32) | ui4_low;
                            if(((ui8_tmp & MP4_SUPER_BIG_MASK_1G_H) != 0) && 
                               (ui4_curr_sample == ui4_range_start_sample))
                            {
                                pt_tbl_obj->t_elmt_tbl.ui8_base_offset  = ui8_tmp&MP4_SUPER_BIG_MASK_1G_H;
                            }
                            pt_idx_entry->ui4_relative_offset = (UINT32)(ui8_tmp - pt_tbl_obj->t_elmt_tbl.ui8_base_offset);

                        }
                        if (ui4_i == 0)
                        {
                        }
                        else if (ui4_curr_sample == ui4_range_start_sample)
                        {
                            
#if (OPTIMAZE_CODE == 1)
                            if (t_cur_track.ui4_default_sample_size == 0)
                            {
                                pf_set_pos(
                                    pt_midxbuld_obj, 
                                    (INT64)(t_cur_track.t_stsz.ui8_box_offset + 20
                                                + 4*(ui4_curr_chunk_sample - 1)), 
                                    FM_SEEK_BGN);
                                
                                if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                                        pt_midxbuld_obj,
                                                        (VOID*)&paui4_stsz_buf,
                                                        sizeof(UINT32)*ui4_i))
                                {
                                    return MIDXBULDR_OUT_OF_MEMORY;
                                }
                                i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                                paui4_stsz_buf, 
                                                sizeof(UINT32)*ui4_i, 
                                                sizeof(UINT32)*ui4_i, 
                                                &ui4_read_size);
                                if (i4_ret != INPUTR_OK)
                                {
                                    return MIDXBULDR_INTERNAL_ERR;
                                }
                            }
                            for(ui4_j = 0; ui4_j < ui4_i; ui4_j++)
                            {
                                if (t_cur_track.ui4_default_sample_size == 0)
                                {
                                    pt_idx_entry->ui4_relative_offset += 
                                        LITTLE_TO_BIG_DWORD(paui4_stsz_buf[ui4_j]); 
                                }
                                else
                                {
                                    pt_idx_entry->ui4_relative_offset += 
                                        t_cur_track.ui4_default_sample_size;
                                }
                            }
                            _mp4_midxbuld_mem_free(pt_midxbuld_obj, paui4_stsz_buf);
#else
                            if (t_cur_track.ui4_default_sample_size == 0)
                            {
                                pf_set_pos(
                                    pt_midxbuld_obj, 
                                    (t_cur_track.t_stsz.ui8_box_offset + 20
                                                + 4*(ui4_curr_chunk_sample - 1)), 
                                    FM_SEEK_BGN);
                            }
                            for(ui4_j = 0; ui4_j < ui4_i; ui4_j++)
                            {
                                if (t_cur_track.ui4_default_sample_size == 0)
                                {
                                    i4_ret = pf_input4(pt_midxbuld_obj, &ui4_sample_size);
                                    pt_idx_entry->ui4_relative_offset += ui4_sample_size; 
                                }
                                else
                                {
                                    pt_idx_entry->ui4_relative_offset += 
                                            t_cur_track.ui4_default_sample_size;
                                }
                            }
#endif
                        }
                        else
                        {
                            pt_idx_pre_entry = pt_idx_entry - 1;
                            pt_idx_entry->ui4_relative_offset = 
                                        pt_idx_pre_entry->ui4_relative_offset
                                        + pt_idx_pre_entry->ui4_size;
                        }
                    }
                    ui4_curr_sample++;
                    if (ui4_curr_sample > ui4_range_end_sample)
                    {
                        ui4_curr_chunk = ui4_next_first_chunk;
                        break;
                    }
                }
                ui4_curr_chunk_sample += ui4_samples_per_chunk;
            }

            ui4_curr_entry++;
            if (ui4_curr_entry >= ui4_entry_count)
            {
                break;
            }
        }
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->ui4_tbl_size;

        /*if(pt_mp4_handler->ui1_curr_file_idx > 1)*/
        {
            pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
            for(ui4_i=0; ui4_i<pt_tbl_obj->ui4_tbl_size; ui4_i++)
            {
                pt_idx_entry->ui4_pts += pt_mp4_handler->t_cur_file_info.ui8_file_start_pts;
                pt_idx_entry->ui4_relative_offset += pt_mp4_handler->t_cur_file_info.ui8_file_start_pos;
                pt_idx_entry++;            
            }
        }
#if 0//ndef __KERNEL__
        {
            UINT32 ui4_n = 0;
            for(ui4_n =0; ui4_n < pt_tbl_obj->ui4_tbl_size; ui4_n++)
            {
                x_dbg_stmt("==normal===baseofst:%#llx, pts: 0x%x, ofst:0x%x, size:0x%x=====\r\n", 
                pt_tbl_obj->t_elmt_tbl.ui8_base_offset, 
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry[ui4_n].ui4_pts,
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry[ui4_n].ui4_relative_offset,
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry[ui4_n].ui4_size);
            }
        }
#endif
#ifndef __KERNEL__
        if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry == NULL)
        {
            x_dbg_stmt("############# ERROR! pt_idx_entry == NULL ############\r\n");
            break;
        }
        x_dbg_stmt("indexing ui4_number_of_entry = %d \r\n", 
                  pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry);
        x_dbg_stmt("***************** range info start*********************\r\n");
        x_dbg_stmt("start    pts: 0x%8x\r\n", pt_tbl_obj->ui4_start_pts);
        x_dbg_stmt("end      pts: 0x%8x\r\n", pt_tbl_obj->ui4_end_pts);
        x_dbg_stmt("start tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[0].ui4_pts);
        x_dbg_stmt("end  tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_pts);
        x_dbg_stmt("start offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[0].ui4_relative_offset);
        x_dbg_stmt("start sample: 0x%8x\r\n", pt_tbl_obj->ui4_start_sample);
        x_dbg_stmt("end   sample: 0x%8x\r\n", pt_tbl_obj->ui4_end_sample);
        x_dbg_stmt("end   offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                    .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_relative_offset);
        x_dbg_stmt("***************** range info end*********************\r\n");
#endif       
        pt_mp4_handler->ui1_curr_buld_progress
            += 70/3/pt_mp4_handler->ui1_valid_track_cnt;/*-- 30%+ 70/(3*cnt)--*/
        
        pt_tbl_obj = pt_tbl_obj->pt_next;
    }

    pt_mp4_handler->pt_curr_range_obj->b_index_is_valid = TRUE;

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_key_frame_table_alloc
 *
 * Description: This function initialize key frame table obj for indexing
 *
 * Inputs:  
 *         pt_key_info:     specifies key frame table
 *         pv_obj:          specify the media info object  
 *
 * Outputs: NULL 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and 
 *                                   the operation has been completed.
 *          Non-zero values: Fail
 ----------------------------------------------------------------------------*/
static INT32 mp4_midxbuld_key_frame_table_alloc(
                        MIDXBULD_KEY_INFO_T* pt_key_info, 
                        VOID*                pv_obj)
{
    MIDXBULD_OBJ_T*                 pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler = NULL;
    MP4_IDXBULD_RANGE_OBJ_T*        pt_range_obj = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_idx_entry = NULL;
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*            pt_cmov_info = NULL;
#endif

    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;
#if (ENABLE_CMOV == 1)
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
#endif

    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
    pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;

    /* allocate elmt tbl range obj memory */
    pt_range_obj = (MP4_IDXBULD_RANGE_OBJ_T*) 
            _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_RANGE_OBJ_T), 
                                        MM_MEM_MIDX_TAG);
    if (pt_range_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (TRUE != _mp4_midxbuld_alloc_range_id(pt_midxbuld_obj, 
                                            &pt_key_info->ui4_range_id))
    {
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_mp4_handler->pt_key_range_obj = pt_range_obj;
    
    x_memset(pt_range_obj, 0, sizeof(MP4_IDXBULD_RANGE_OBJ_T));
    pt_range_obj->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->ui4_range_id = pt_key_info->ui4_range_id;
    pt_range_obj->b_is_key_frame = TRUE;
    pt_range_obj->b_index_is_valid = FALSE;
    pt_range_obj->ui1_tbl_total = 0;

    t_describ.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_KEY;
    t_describ.u_info.t_es.ui4_strm_id = pt_key_info->ui4_strm_id;
    /* here scale and rate are used to caculate frame rate*/
    t_describ.u_info.t_es.ui4_scale = pt_key_info->ui4_scale;
    t_describ.u_info.t_es.ui4_rate = pt_key_info->ui4_rate;
    t_describ.u_info.t_es.ui4_sample_sz = pt_key_info->ui4_sample_sz;
    t_describ.u_info.t_es.ui4_avg_bytes_per_sec = pt_key_info->ui4_avg_bytes_per_sec;

    /* allocate elmt tbl obj memory */
    pt_tbl_obj = (MP4_IDXBULD_ELMT_TBL_OBJ_T*) 
                _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T), 
                                            MM_MEM_MIDX_TAG);
    if (pt_tbl_obj == NULL)
    {
        _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    if (TRUE != _mp4_midxbuld_alloc_table_id(pt_range_obj, &pt_key_info->ui4_tbl_id))
    {
        _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
        _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_range_obj->pt_tbl_obj = pt_tbl_obj;
    
    x_memset(pt_tbl_obj, 0, sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T));
    pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
    pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
#if (ENABLE_CMOV == 1)
    if(pt_cmov_info->b_is_cmov_box_exist)
    {
        pt_tbl_obj->ui4_tbl_size = 
            mp4_cmov_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                pt_range_obj, 
                                                pt_tbl_obj, 
                                                &t_describ);
    }
    else
#endif
    {
        pt_tbl_obj->ui4_tbl_size = 
            mp4_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                pt_range_obj, 
                                                pt_tbl_obj, 
                                                &t_describ);
    }
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = t_describ.u_info.t_es;

    if(pt_tbl_obj->ui4_tbl_size == 0)/*no key table exist*/
    {
        pt_tbl_obj->ui1_is_key_exist = 0;
        pt_idx_entry = NULL;
    }
    else
    {
        pt_tbl_obj->ui1_is_key_exist = 1;
        pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) 
                _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_STRM_ENTRY_T)
                        * pt_tbl_obj->ui4_tbl_size, 
                        MM_MEM_MIDX_TAG);
        if (pt_idx_entry == NULL)
        {
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
            pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
            pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_idx_entry,
                0, 
                sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
    }
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;

    pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
    pt_tbl_obj->pt_next = NULL;
    
    pt_mp4_handler->pt_curr_range_obj = pt_range_obj;

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_key_ext_cal_size
 *
 * Description: This function calculate key ext frame size
 *
 * Inputs:  pt_midxbuld_obj,     specify the media index builder object  
 *          pt_key_ext_tbl,      specifies key ext table
 *          t_cur_track,         specifies current track
 *
 * Outputs: pt_key_ext_tbl,      specifies key ext table
 *
 * Returns: 
 *
 ----------------------------------------------------------------------------*/
static INT32 mp4_key_ext_cal_size(
                        MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                        MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl,
                        MP4_IDXBULD_TRACK_T         t_cur_track)
{
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler;
    input4bytes_fct     pf_input4;
    setpos_fct          pf_set_pos;
    UINT32              ui4_key_num;
    UINT32              ui4_key_sample_idx;
    UINT32              ui4_tmp;

    INT32               i4_ret;    
    UINT32              ui4_read_size;    
    UINT32              ui4_stsz_sample_count;    
    UINT32              ui4_stsz_real_buf_count = 0;     
    UINT32              ui4_cur_stsz_buf_last_sample_idx = 0;
    UINT32              ui4_stsz_buf_max = MP4_MIDXBULD_BUF_SIZE/sizeof(UINT32);   
    UINT32*             paui4_stsz_buf = NULL;    
    copybytes_fct       pf_copybytes;    

    pf_copybytes = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    /*ui4_key_num = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size;*/
    /*navy 080610 modified for VideoLen>AudioLen*/
    if(!pt_mp4_handler->t_mln_moov_info.b_is_moov_empty && 
        pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)
    {
        ui4_key_num = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_key_cnt;
    }
    else
    {
        ui4_key_num = pt_key_ext_tbl->ui4_key_num;
    }
    
    pf_set_pos(pt_midxbuld_obj, 
                (INT64)t_cur_track.t_stsz.ui8_box_offset + 16, 
                FM_SEEK_BGN);
    if (t_cur_track.ui4_default_sample_size == 0)
    {
        pf_input4(pt_midxbuld_obj, &ui4_stsz_sample_count);
        if(ui4_stsz_sample_count <= ui4_stsz_buf_max)
        {
            ui4_stsz_real_buf_count = ui4_stsz_sample_count;
        }
        else
        {
            ui4_stsz_real_buf_count = ui4_stsz_buf_max;
        }
        if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                         pt_midxbuld_obj,
                                         (VOID*)&paui4_stsz_buf,
                                         sizeof(UINT32)*ui4_stsz_real_buf_count))
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                            paui4_stsz_buf, 
                                            sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                            sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                            &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
    }
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        if (t_cur_track.ui4_default_sample_size == 0)
        {
            ui4_key_sample_idx
                        = pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
            if(ui4_key_sample_idx - ui4_cur_stsz_buf_last_sample_idx 
                                                            > ui4_stsz_buf_max)
            {
                _mp4_midxbuld_mem_free(pt_midxbuld_obj, paui4_stsz_buf);
                ui4_cur_stsz_buf_last_sample_idx += ui4_stsz_real_buf_count;    
                if(ui4_stsz_sample_count - ui4_cur_stsz_buf_last_sample_idx
                                <= ui4_stsz_buf_max)
                {
                    ui4_stsz_real_buf_count = ui4_stsz_sample_count - ui4_cur_stsz_buf_last_sample_idx;
                }
                else
                {
                    ui4_stsz_real_buf_count = ui4_stsz_buf_max;
                }
                if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                        pt_midxbuld_obj,
                                        (VOID*)&paui4_stsz_buf,
                                        sizeof(UINT32)*ui4_stsz_real_buf_count))
                {
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                pf_set_pos(pt_midxbuld_obj,
                                (INT64)(t_cur_track.t_stsz.ui8_box_offset + 20 +
                                        4 * ui4_cur_stsz_buf_last_sample_idx),
                                FM_SEEK_BGN);
                i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                        paui4_stsz_buf, 
                                        sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                        sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                        &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {
                    return MIDXBULDR_INTERNAL_ERR;
                }
            }
            pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_size
                     = LITTLE_TO_BIG_DWORD(paui4_stsz_buf[ui4_key_sample_idx - 1
                                            - ui4_cur_stsz_buf_last_sample_idx]);
        }
        else
        {
            pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_size
                                        = t_cur_track.ui4_default_sample_size;
        }
    }
    if (t_cur_track.ui4_default_sample_size == 0)
    {
        _mp4_midxbuld_mem_free(pt_midxbuld_obj, paui4_stsz_buf);
    }
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_key_ext_cal_offset
 *
 * Description: This function calculate key ext frame offset
 *
 * Inputs:  pt_midxbuld_obj,     specify the media index builder object  
 *          pt_key_ext_tbl,      specifies key ext table
 *          t_cur_track,         specifies current track
 *
 * Outputs: pt_key_ext_tbl,      specifies key ext table
 *
 * Returns: 
 *
 ----------------------------------------------------------------------------*/
#if (OPTIMAZE_CODE == 1)
static INT32 mp4_key_ext_cal_offset(
                        MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                        MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl,
                        MP4_IDXBULD_TRACK_T         t_cur_track)
{
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler;
    input4bytes_fct     pf_input4;
    setpos_fct          pf_set_pos;
    MP4_STSC_TBL_T*     pt_stsc_tbl = NULL;
    MP4_STSC_TBL_T*     pt_stsc_curr = NULL;
    MP4_STSC_TBL_T*     pt_stsc_prev = NULL;
    UINT32 ui4_key_num;
    UINT32 ui4_key_sample_idx;
    UINT32 ui4_entry_count;
    UINT32 ui4_chunk_count = 0;
    UINT32 ui4_key_ext_chunk;
    UINT32 ui4_sample_offset_in_chunk;
    UINT32 ui4_next_first_chunk;
    UINT32 ui4_tmp, ui4_k, ui4_t;
    
    UINT32              ui4_read_size;
    UINT32              ui4_stsz_sample_count;
    UINT32              ui4_stsz_real_buf_count = 0;
    UINT32              ui4_stsz_buf_max;
    UINT32              ui4_cur_stsz_buf_last_sample_idx;
    INT32               i4_ret = MIDXBULDR_OK;
    MP4_RAW_STSC_TBL_T* pr_stsc_buf = NULL;
    UINT32*             paui4_stco_buf = NULL;
    UINT32*             paui4_stsz_buf = NULL;
    copybytes_fct       pf_copybytes;

    ui4_cur_stsz_buf_last_sample_idx = 0;
    ui4_stsz_buf_max = MP4_MIDXBULD_BUF_SIZE/sizeof(UINT32);
    pf_copybytes = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;

    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    
    /*navy 080610 modified for VideoLen>AudioLen*/
    if(!pt_mp4_handler->t_mln_moov_info.b_is_moov_empty && 
        pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)
    {
        ui4_key_num = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_key_cnt;
    }
    else
    {
        ui4_key_num = pt_key_ext_tbl->ui4_key_num;
    }

    /* seek to 'sample to chunk' and 'chunk offset'  box to cal offset */
    if(t_cur_track.t_stco.b_is_box_exist)
    {
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_stco.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
        /* get chunk count */
        pf_input4(pt_midxbuld_obj, &ui4_chunk_count);
        paui4_stco_buf = (UINT32*)_mm_util_mem_alloc_with_tag(
                                                sizeof(UINT32) * ui4_chunk_count, 
                                                MM_MEM_MIDX_TAG);
        if(paui4_stco_buf == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                              paui4_stco_buf, 
                              sizeof(UINT32)*ui4_chunk_count, 
                              sizeof(UINT32)*ui4_chunk_count, 
                              &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
    }
    else if(t_cur_track.t_co64.b_is_box_exist)
    {
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_co64.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
        /* get chunk count */
        pf_input4(pt_midxbuld_obj, &ui4_chunk_count);
        paui4_stco_buf = (UINT32*)_mm_util_mem_alloc_with_tag(
                                            sizeof(UINT32) * ui4_chunk_count*2, 
                                            MM_MEM_MIDX_TAG);
        if(paui4_stco_buf == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                              paui4_stco_buf, 
                              sizeof(UINT32)*ui4_chunk_count*2, 
                              sizeof(UINT32)*ui4_chunk_count*2, 
                              &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
    }
    else
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    pf_set_pos(pt_midxbuld_obj, 
                (INT64)(t_cur_track.t_stsc.ui8_box_offset + 12), 
                FM_SEEK_BGN);
    /* get sample to chunk entry count */
    pf_input4(pt_midxbuld_obj, &ui4_entry_count);
    pt_stsc_tbl = (MP4_STSC_TBL_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_STSC_TBL_T) * ui4_entry_count, 
                                        MM_MEM_MIDX_TAG);
    if (pt_stsc_tbl == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_stsc_tbl, 0, sizeof(MP4_STSC_TBL_T) * ui4_entry_count);

    if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                    pt_midxbuld_obj,
                                    (VOID*)&pr_stsc_buf,
                                    sizeof(MP4_RAW_STSC_TBL_T)*ui4_entry_count))
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    i4_ret = pf_copybytes(pt_midxbuld_obj, 
                        pr_stsc_buf, 
                        sizeof(MP4_RAW_STSC_TBL_T)*ui4_entry_count, 
                        sizeof(MP4_RAW_STSC_TBL_T)*ui4_entry_count, 
                        &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pt_stsc_curr = pt_stsc_tbl;
    pt_stsc_prev = pt_stsc_curr;
    for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        pt_stsc_curr->ui4_first_chunk
              = LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_tmp].ui4_first_chunk);
        pt_stsc_curr->ui4_samples_per_chunk
              = LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_tmp].ui4_samples_per_chunk);
        pt_stsc_curr->ui4_sample_description_index
              = LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_tmp].ui4_sample_description_index);
        if (ui4_tmp < (ui4_entry_count - 1))
        {
            ui4_next_first_chunk = 
                    LITTLE_TO_BIG_DWORD(pr_stsc_buf[ui4_tmp+1].ui4_first_chunk);
        }
        else
        {
            ui4_next_first_chunk = ui4_chunk_count + 1;
        }
        pt_stsc_curr->ui4_repeat_count
                        = ui4_next_first_chunk - pt_stsc_curr->ui4_first_chunk;
        if (ui4_tmp == 0)
        {
            pt_stsc_curr->ui4_first_sample = 1;
        }
        else
        {
            pt_stsc_curr->ui4_first_sample = pt_stsc_prev->ui4_first_sample + 
                pt_stsc_prev->ui4_samples_per_chunk * pt_stsc_prev->ui4_repeat_count;
        }
        if (ui4_tmp < (ui4_entry_count - 1))
        {
            pt_stsc_prev = pt_stsc_curr;
            pt_stsc_curr = pt_stsc_tbl + ui4_tmp + 1;
            pt_stsc_curr->ui4_first_chunk = ui4_next_first_chunk;
        }
    }    
    _mp4_midxbuld_mem_free(pt_midxbuld_obj, pr_stsc_buf);
    
    pf_set_pos(pt_midxbuld_obj, 
                (INT64)t_cur_track.t_stsz.ui8_box_offset + 16, 
                FM_SEEK_BGN);
    pf_input4(pt_midxbuld_obj, &ui4_stsz_sample_count);

    if (t_cur_track.ui4_default_sample_size == 0)
    {
        if(ui4_stsz_sample_count <= ui4_stsz_buf_max)
        {
            ui4_stsz_real_buf_count = ui4_stsz_sample_count;
        }
        else
        {
            ui4_stsz_real_buf_count = ui4_stsz_buf_max;
        }
        if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                         pt_midxbuld_obj,
                                         (VOID*)&paui4_stsz_buf,
                                         sizeof(UINT32)*ui4_stsz_real_buf_count))
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                            paui4_stsz_buf, 
                                            sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                            sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                            &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
    }

    ui4_k = 0;
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        ui4_key_sample_idx = pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
        for(; ui4_k < ui4_entry_count; ui4_k++)
        {
            if ((ui4_key_sample_idx >= pt_stsc_tbl[ui4_k].ui4_first_sample) &&
                (ui4_key_sample_idx < (pt_stsc_tbl[ui4_k].ui4_first_sample + 
                        pt_stsc_tbl[ui4_k].ui4_repeat_count * 
                            pt_stsc_tbl[ui4_k].ui4_samples_per_chunk)))
            {
                break;
            }
        }
        if(ui4_k >= ui4_entry_count)
        {
            ui4_k = ui4_entry_count - 1;
        }
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stsc_entry = ui4_k;
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stsc_entry_sample
                                        = pt_stsc_tbl[ui4_k].ui4_first_sample;
        /* check the dividend can't equal zero */
        if(pt_stsc_tbl[ui4_k].ui4_samples_per_chunk == 0)
        {
            _mm_util_mem_free_with_tag(pt_stsc_tbl, MM_MEM_MIDX_TAG);
            return MIDXBULDR_INITIAL_ERR;
        }
        ui4_key_ext_chunk = pt_stsc_tbl[ui4_k].ui4_first_chunk + 
                  (ui4_key_sample_idx - pt_stsc_tbl[ui4_k].ui4_first_sample)
                        / pt_stsc_tbl[ui4_k].ui4_samples_per_chunk;
        ui4_sample_offset_in_chunk
                    = (ui4_key_sample_idx -pt_stsc_tbl[ui4_k].ui4_first_sample)
                        % pt_stsc_tbl[ui4_k].ui4_samples_per_chunk;
        /* seek to chunk offset box */
        if(t_cur_track.t_stco.b_is_box_exist)
        {
            pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset
                = LITTLE_TO_BIG_DWORD(paui4_stco_buf[ui4_key_ext_chunk - 1]);
        }
        else if(t_cur_track.t_co64.b_is_box_exist)
        {
            UINT64      ui8_tmp = 0;
            ui8_tmp = LITTLE_TO_BIG_DWORD(paui4_stco_buf[ui4_key_ext_chunk*2 - 2]);
            ui8_tmp = ui8_tmp<<32;
            ui8_tmp = ui8_tmp | (LITTLE_TO_BIG_DWORD(paui4_stco_buf[ui4_key_ext_chunk*2 - 1]));
            pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset = ui8_tmp;
        }
        /* seek to sample size box */
        if (ui4_sample_offset_in_chunk > 0)
        {
            if (t_cur_track.ui4_default_sample_size == 0)
            {
                for(ui4_t = 0; ui4_t < ui4_sample_offset_in_chunk; ui4_t++)
                {
                    if(ui4_key_sample_idx - ui4_sample_offset_in_chunk + ui4_t
                            - ui4_cur_stsz_buf_last_sample_idx > ui4_stsz_buf_max)
                    {
                        _mp4_midxbuld_mem_free(pt_midxbuld_obj, paui4_stsz_buf);
                        ui4_cur_stsz_buf_last_sample_idx += ui4_stsz_real_buf_count;
                        if(ui4_stsz_sample_count-ui4_cur_stsz_buf_last_sample_idx
                                            <= ui4_stsz_buf_max)
                        {
                            ui4_stsz_real_buf_count = ui4_stsz_sample_count
                                                - ui4_cur_stsz_buf_last_sample_idx;
                        }
                        else
                        {
                            ui4_stsz_real_buf_count = ui4_stsz_buf_max;
                        }
                        pf_set_pos(pt_midxbuld_obj, 
                           (INT64)(t_cur_track.t_stsz.ui8_box_offset + 20 + 
                                    4 * ui4_cur_stsz_buf_last_sample_idx), 
                                    FM_SEEK_BGN);
                        if(MIDXBULDR_OK !=  _mp4_midxbuld_mem_alloc(
                                        pt_midxbuld_obj,
                                        (VOID*)&paui4_stsz_buf,
                                        sizeof(UINT32)*ui4_stsz_real_buf_count))
                        {
                            return MIDXBULDR_OUT_OF_MEMORY;
                        }
                        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                            paui4_stsz_buf, 
                                            sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                            sizeof(UINT32)*ui4_stsz_real_buf_count, 
                                            &ui4_read_size);
                        if (i4_ret != INPUTR_OK)
                        {
                            return MIDXBULDR_INTERNAL_ERR;
                        }
                    }
                    pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset
                            += LITTLE_TO_BIG_DWORD(paui4_stsz_buf[ui4_key_sample_idx
                                        - ui4_sample_offset_in_chunk
                                        + ui4_t - 1
                                        - ui4_cur_stsz_buf_last_sample_idx]);
                }
            }
            else
            {
                pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset 
                           += t_cur_track.ui4_default_sample_size
                                        * ui4_sample_offset_in_chunk;
            }
        }
    }
    if (t_cur_track.ui4_default_sample_size == 0)
    {
        _mp4_midxbuld_mem_free(pt_midxbuld_obj, paui4_stsz_buf);
    }
    _mm_util_mem_free_with_tag(pt_stsc_tbl, MM_MEM_MIDX_TAG);
    _mm_util_mem_free_with_tag(paui4_stco_buf, MM_MEM_MIDX_TAG);
    return MIDXBULDR_OK;
}
#else
static INT32 mp4_key_ext_cal_offset(
                        MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                        MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl,
                        MP4_IDXBULD_TRACK_T         t_cur_track)
{
    /*MP4_IDXBULD_OBJ_T*  pt_mp4_handler;*/
    input4bytes_fct     pf_input4;
    setpos_fct          pf_set_pos;
    MP4_STSC_TBL_T*     pt_stsc_tbl = NULL;
    MP4_STSC_TBL_T*     pt_stsc_curr = NULL;
    MP4_STSC_TBL_T*     pt_stsc_prev = NULL;
    UINT32 ui4_key_num;
    UINT32 ui4_key_sample_idx;
    UINT32 ui4_entry_count;
    UINT32 ui4_chunk_count;
    UINT32 ui4_key_ext_chunk;
    UINT32 ui4_sample_offset_in_chunk;
    UINT32 ui4_sample_size;
    UINT32 ui4_next_first_chunk;
    UINT32 ui4_tmp, ui4_k, ui4_t;

    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    
    /*pt_mp4_handler = (MP4_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    ui4_key_num = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size; */
    /*navy 080610 modified for VideoLen>AudioLen*/
    ui4_key_num = pt_key_ext_tbl->ui4_key_num;
    /* seek to 'sample to chunk' and 'chunk offset'  box to cal offset */
    if(t_cur_track.t_stco.b_is_box_exist)
    {
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_stco.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
    }
    else if(t_cur_track.t_co64.b_is_box_exist)
    {
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_co64.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
    }
    /* get chunk count */
    pf_input4(pt_midxbuld_obj, &ui4_chunk_count);

    pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_stsc.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
    /* get sample to chunk entry count */
    pf_input4(pt_midxbuld_obj, &ui4_entry_count);
    pt_stsc_tbl = (MP4_STSC_TBL_T*)_mm_util_mem_alloc_with_tag(
                                        sizeof(MP4_STSC_TBL_T) * ui4_entry_count, 
                                        MM_MEM_MIDX_TAG);
    if (pt_stsc_tbl == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    pt_stsc_curr = pt_stsc_tbl;
    pt_stsc_prev = pt_stsc_curr;
    pf_input4(pt_midxbuld_obj, &(pt_stsc_curr->ui4_first_chunk));
    for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        pf_input4(pt_midxbuld_obj, &(pt_stsc_curr->ui4_samples_per_chunk));
        pf_input4(pt_midxbuld_obj, &(pt_stsc_curr->ui4_sample_description_index));
        if (ui4_tmp < (ui4_entry_count - 1))
        {
            pf_input4(pt_midxbuld_obj, &ui4_next_first_chunk);
        }
        else
        {
            ui4_next_first_chunk = ui4_chunk_count + 1;
        }
        pt_stsc_curr->ui4_repeat_count
                        = ui4_next_first_chunk - pt_stsc_curr->ui4_first_chunk;
        if (ui4_tmp == 0)
        {
            pt_stsc_curr->ui4_first_sample = 1;
        }
        else
        {
            pt_stsc_curr->ui4_first_sample
                        = pt_stsc_prev->ui4_first_sample + 
                            pt_stsc_prev->ui4_samples_per_chunk
                                * pt_stsc_prev->ui4_repeat_count;
        }
        if (ui4_tmp < (ui4_entry_count - 1))
        {
            pt_stsc_prev = pt_stsc_curr;
            pt_stsc_curr = pt_stsc_tbl + ui4_tmp + 1;
            pt_stsc_curr->ui4_first_chunk = ui4_next_first_chunk;
        }
    }
    ui4_k = 0;
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        ui4_key_sample_idx = pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
        for(; ui4_k < ui4_entry_count; ui4_k++)
        {
            if ((ui4_key_sample_idx >= pt_stsc_tbl[ui4_k].ui4_first_sample) &&
                (ui4_key_sample_idx < (pt_stsc_tbl[ui4_k].ui4_first_sample
                    + pt_stsc_tbl[ui4_k].ui4_repeat_count
                        * pt_stsc_tbl[ui4_k].ui4_samples_per_chunk)))
            {
                break;
            }
        }
        if(ui4_k >= ui4_entry_count)
        {
            ui4_k = ui4_entry_count - 1;
        }
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stsc_entry = ui4_k;
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stsc_entry_sample
                                    = pt_stsc_tbl[ui4_k].ui4_first_sample;
        /* check the dividend can't equal zero */
        if(pt_stsc_tbl[ui4_k].ui4_samples_per_chunk == 0)
        {
            _mm_util_mem_free_with_tag(pt_stsc_tbl, MM_MEM_MIDX_TAG);
            return MIDXBULDR_INITIAL_ERR;
        }
        ui4_key_ext_chunk = pt_stsc_tbl[ui4_k].ui4_first_chunk + 
                  (ui4_key_sample_idx -pt_stsc_tbl[ui4_k].ui4_first_sample)
                        / pt_stsc_tbl[ui4_k].ui4_samples_per_chunk;
        ui4_sample_offset_in_chunk
                    = (ui4_key_sample_idx -pt_stsc_tbl[ui4_k].ui4_first_sample)
                            % pt_stsc_tbl[ui4_k].ui4_samples_per_chunk;
        /* seek to chunk offset box */
        if(t_cur_track.t_stco.b_is_box_exist)
        {
            pf_set_pos(pt_midxbuld_obj, 
                           (INT64)(t_cur_track.t_stco.ui8_box_offset + 16
                                    + 4*(ui4_key_ext_chunk - 1)), 
                           FM_SEEK_BGN);
        }
        else if(t_cur_track.t_co64.b_is_box_exist)
        {
              pf_set_pos(pt_midxbuld_obj, 
                           (INT64)(t_cur_track.t_co64.ui8_box_offset + 16
                                    + 8*(ui4_key_ext_chunk - 1) + 4), 
                           FM_SEEK_BGN);
        }
        pf_input4(pt_midxbuld_obj, 
                    &(pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset));
        /* seek to sample size box */
        if (ui4_sample_offset_in_chunk > 0)
        {
            if (t_cur_track.ui4_default_sample_size == 0)
            {
                pf_set_pos(pt_midxbuld_obj, 
                           (INT64)(t_cur_track.t_stsz.ui8_box_offset + 20
                                + 4*(ui4_key_sample_idx
                                     - ui4_sample_offset_in_chunk - 1)), 
                           FM_SEEK_BGN);
            }
            if (t_cur_track.ui4_default_sample_size == 0)
            {
                for(ui4_t = 0; ui4_t < ui4_sample_offset_in_chunk; ui4_t++)
                {
                    pf_input4(pt_midxbuld_obj, &ui4_sample_size);
                    pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset
                                                            += ui4_sample_size; 
                }
            }
            else
            {
                pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui8_relative_offset
                            += t_cur_track.ui4_default_sample_size
                                            * ui4_sample_offset_in_chunk;
            }
        }
    }
    _mm_util_mem_free_with_tag(pt_stsc_tbl, MM_MEM_MIDX_TAG);
    return MIDXBULDR_OK;
}
#endif

#if (ENABLE_CTTS == 1)
/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_init_ctts_tbl
 *
 * Description: This function builds key ext table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *          pt_key_info ,       specifies key frame info
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the key ext table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
static INT32 mp4_midxbuld_init_ctts_tbl(
                            MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                            MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl,
                            MP4_IDXBULD_TRACK_T*        t_cur_track)
{
    INT32                   i4_ret = 0;
    UINT32                  ui4_tmp = 0;
    UINT32                  ui4_read_size = 0;
    UINT32                  ui4_entry_count = 0;
    setpos_fct              pf_set_pos;
    input4bytes_fct         pf_input4;
    copybytes_fct           pf_copybytes;
    MP4_RAW_CTTS_TBL_T*     pt_ctts_item_buf = NULL;

    if((NULL == pt_midxbuld_obj) |(NULL == pt_key_ext_tbl)||(NULL == t_cur_track))
    {
        return MIDXBULDR_INV_ARG;
    }
    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_copybytes = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    
    pf_set_pos(pt_midxbuld_obj, 
               (INT64)(t_cur_track->t_ctts.ui8_box_offset + 12), 
               FM_SEEK_BGN);
    /*get ctts entry count*/
    pf_input4(pt_midxbuld_obj, &ui4_entry_count);
    pt_ctts_item_buf = (MP4_RAW_CTTS_TBL_T*)_mm_util_mem_alloc_with_tag(
                                sizeof(MP4_RAW_CTTS_TBL_T) * (ui4_entry_count + 1), 
                                MM_MEM_MIDX_TAG);
    if(pt_ctts_item_buf == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_ctts_item_buf, 0, sizeof(MP4_RAW_CTTS_TBL_T) * (ui4_entry_count+1));
    pt_key_ext_tbl->t_ctts_buf.ui4_ctts_entry_num = ui4_entry_count;
    pt_key_ext_tbl->t_ctts_buf.pt_ctts_item_buf = pt_ctts_item_buf;
    i4_ret = pf_copybytes(pt_midxbuld_obj, 
                            pt_ctts_item_buf, 
                            sizeof(MP4_RAW_CTTS_TBL_T) * ui4_entry_count, 
                            sizeof(MP4_RAW_CTTS_TBL_T) * ui4_entry_count, 
                            &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    pt_ctts_item_buf[0].ui4_sample_count
                = LITTLE_TO_BIG_DWORD(pt_ctts_item_buf[0].ui4_sample_count);
    pt_ctts_item_buf[0].ui4_sample_offset
                = LITTLE_TO_BIG_DWORD(pt_ctts_item_buf[0].ui4_sample_offset);
    for(ui4_tmp = 1; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        pt_ctts_item_buf[ui4_tmp].ui4_sample_count
            = LITTLE_TO_BIG_DWORD(pt_ctts_item_buf[ui4_tmp].ui4_sample_count)
                                + pt_ctts_item_buf[ui4_tmp-1].ui4_sample_count;
        pt_ctts_item_buf[ui4_tmp].ui4_sample_offset
                = LITTLE_TO_BIG_DWORD(pt_ctts_item_buf[ui4_tmp].ui4_sample_offset);
    }    
    if(ui4_tmp == ui4_entry_count)
    {
        pt_ctts_item_buf[ui4_tmp].ui4_sample_offset = 0;
    }
    return MIDXBULDR_OK;
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_key_ext_table_build
 *
 * Description: This function builds key ext table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *          pt_key_info ,       specifies key frame info
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the key ext table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_key_ext_table_build(
                            MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                            MIDXBULD_KEY_INFO_T*    pt_key_info)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler      = NULL;
    MP4_IDXBULD_TRACK_T         t_cur_track         = {0};
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl      = NULL;
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl2     = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry        = NULL;
    MP4_STTS_TBL_T*             pt_stts_tbl         = NULL;    
    UINT64 ui8_tmp_duration                         = 0;
    UINT64 ui8_time                                 = 0;
    UINT32 ui4_key_num                              = 0;
    UINT32 ui4_total_key_num                        = 0;
    UINT32 ui4_key_sample_idx                       = 0;
    UINT32 ui4_key_sample_pts                       = 0;
    UINT32 ui4_entry_count                          = 0;    
    UINT32 ui4_tmp                                  = 0;
    UINT32 ui4_k                                    = 0;
    UINT32 ui4_i                                    = 0;
    UINT32 ui4_last_ctts_idx                        = 0;

    INT32  i4_ret                                   = 0;
    UINT8  ui1_idx                                  = 0;
    UINT8  ui1_j                                    = 0;
    input4bytes_fct             pf_input4;
    setpos_fct                  pf_set_pos;
#if (ENABLE_CTTS == 1)
    MP4_RAW_CTTS_TBL_T* pt_ctts_item_buf            = NULL;
#endif
#if (OPTIMAZE_CODE == 1)
    UINT32                      ui4_read_size       = 0;
    UINT32*                     paui4_stss_buf      = NULL;
    MP4_RAW_STTS_TBL_T*         pr_stts_buf         = NULL;
    copybytes_fct               pf_copybytes;    
    pf_copybytes = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
#endif
    
    pf_input4       = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos      = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;    
    pt_mp4_handler  = (MP4_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    

    /* key table first */    
    if(!pt_mp4_handler->t_mln_moov_info.b_is_moov_empty &&
        pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)
    {
        ui4_key_num = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_key_cnt;
    }
    else
    {
        ui4_key_num = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size;
    }
    for(ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; ui1_idx++)
    {
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
        }
        if (pt_key_info->ui4_strm_id
                    == pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
        {
            t_cur_track = pt_mp4_handler->t_track_info_box[ui1_idx];
            break;
        }
    }
    if(ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    if(ui4_key_num == 0)/*no key table exist*/
    {
        if((pt_midxbuld_obj->t_src_info.ui1_total_strm_num ==1) &&
            !pt_midxbuld_obj->t_src_info.t_es_map[0].b_is_cur_strm_codec_spt)
        {
            return MIDXBULDR_FMT_NOT_SUPPORT;
        }
        /* seek to 'time to sample'  box to cal pts */
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_stsz.ui8_box_offset + 16), 
                    FM_SEEK_BGN);
        /* get Time to Sample entry count */
        pf_input4(pt_midxbuld_obj, &ui4_entry_count);  
        ui4_key_num = ui4_entry_count / MP4_KEY_LEN_OF_SAMPLE_NUM;
        if((ui4_entry_count % MP4_KEY_LEN_OF_SAMPLE_NUM) != 0)
        {
            ui4_key_num += 1;
        }
        pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size = ui4_key_num;

        pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui1_is_key_exist = 0;
        pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                    sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num, 
                    MM_MEM_MIDX_TAG);
        if (pt_idx_entry == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num);
        pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl
                                        .u_fmt.t_es.pt_idx_entry = pt_idx_entry;
    
    }
    
    ui4_total_key_num = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size;
    
    if (ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    if (pt_mp4_handler->pt_key_ext_tbl != NULL)
    {
        _mm_util_mem_free_with_tag(pt_mp4_handler->pt_key_ext_tbl, MM_MEM_MIDX_TAG);
    }
    pt_mp4_handler->pt_key_ext_tbl = (MP4_IDXBULD_KEY_EXT_TBL_T*)
                _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_TBL_T), 
                                            MM_MEM_MIDX_TAG);
    if (pt_mp4_handler->pt_key_ext_tbl == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_handler->pt_key_ext_tbl, 
             0, 
             sizeof(MP4_IDXBULD_KEY_EXT_TBL_T));
    pt_key_ext_tbl = pt_mp4_handler->pt_key_ext_tbl;
    pt_key_ext_tbl->ui4_track_id = pt_key_info->ui4_strm_id;
    pt_key_ext_tbl->ui8_base_offset = 0;
    pt_key_ext_tbl->pt_next = NULL;
    /*navy 080610 modified for VideoLen>AudioLen*/
    pt_key_ext_tbl->ui4_key_num = ui4_total_key_num;
    pt_key_ext_tbl->pt_key_entry = (MP4_MIDXBULD_ELMT_STRM_ENTRY_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T)
                                            *ui4_total_key_num, 
                                        MM_MEM_MIDX_TAG);
    pt_key_ext_tbl->pt_key_ext = (MP4_IDXBULD_KEY_EXT_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_T)
                                            *ui4_total_key_num, 
                                        MM_MEM_MIDX_TAG);
    if ((pt_key_ext_tbl->pt_key_entry == NULL) 
        || (pt_key_ext_tbl->pt_key_ext == NULL))
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_ext_tbl->pt_key_entry, 
             0, 
             sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_total_key_num);
    x_memset(pt_key_ext_tbl->pt_key_ext, 
             0, 
             sizeof(MP4_IDXBULD_KEY_EXT_T) * ui4_total_key_num);
    /* seek to stss box to get key sample */
    if (t_cur_track.t_stss.b_is_box_exist)
    {
        pf_set_pos(pt_midxbuld_obj, 
                   (INT64)(t_cur_track.t_stss.ui8_box_offset + 16), 
                   FM_SEEK_BGN);
    }
    
#if (OPTIMAZE_CODE == 1)
    if(pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui1_is_key_exist != 0)
    {
        if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                     pt_midxbuld_obj,
                                     (VOID*)&paui4_stss_buf,
                                     sizeof(UINT32)*ui4_key_num))
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                paui4_stss_buf, 
                                sizeof(UINT32)*ui4_key_num, 
                                sizeof(UINT32)*ui4_key_num, 
                                &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
    }
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        if(pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui1_is_key_exist != 0)
        {
            pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample
                            = LITTLE_TO_BIG_DWORD(paui4_stss_buf[ui4_tmp]);
        }
        else
        {
            pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample
                            = ui4_tmp * MP4_KEY_LEN_OF_SAMPLE_NUM + 1;
        }
    }
    if(pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui1_is_key_exist != 0)
    {
        _mp4_midxbuld_mem_free(pt_midxbuld_obj, paui4_stss_buf);
    }
#else
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        if(pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui1_is_key_exist != 0)
        {
            pf_input4(pt_midxbuld_obj, 
                    &(pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample));
        }
        else
        {
            pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample
                                    = ui4_tmp * MP4_KEY_LEN_OF_SAMPLE_NUM + 1;
        }
    }
#endif

    /* seek to 'time to sample'  box to cal pts */
    pf_set_pos(pt_midxbuld_obj, 
                (INT64)(t_cur_track.t_stts.ui8_box_offset + 12), 
                FM_SEEK_BGN);
    /* get Time to Sample entry count */
    pf_input4(pt_midxbuld_obj, &ui4_entry_count);
    pt_stts_tbl = (MP4_STTS_TBL_T*)_mm_util_mem_alloc_with_tag(
                                    sizeof(MP4_STTS_TBL_T) * ui4_entry_count, 
                                    MM_MEM_MIDX_TAG);
    if (pt_stts_tbl == NULL)
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_stts_tbl, 0, sizeof(MP4_STTS_TBL_T) * ui4_entry_count);

#if (OPTIMAZE_CODE == 1)
    if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                     pt_midxbuld_obj,
                                     (VOID*)&pr_stts_buf,
                                     sizeof(MP4_RAW_STTS_TBL_T)*ui4_entry_count))
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                pr_stts_buf, 
                                sizeof(MP4_RAW_STTS_TBL_T)*ui4_entry_count, 
                                sizeof(MP4_RAW_STTS_TBL_T)*ui4_entry_count, 
                                &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        pt_stts_tbl[ui4_tmp].ui4_sample_count
                   = LITTLE_TO_BIG_DWORD(pr_stts_buf[ui4_tmp].ui4_sample_count);
        pt_stts_tbl[ui4_tmp].ui4_sample_duration
                   = LITTLE_TO_BIG_DWORD(pr_stts_buf[ui4_tmp].ui4_sample_duration);
        if (ui4_tmp == 0)
        {
            pt_stts_tbl[ui4_tmp].ui4_starting_sample = 1;
            pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 0;
        }
        else
        {
            pt_stts_tbl[ui4_tmp].ui4_starting_sample = 
                                    pt_stts_tbl[ui4_tmp-1].ui4_starting_sample
                                    + pt_stts_tbl[ui4_tmp-1].ui4_sample_count;
            pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 
                            pt_stts_tbl[ui4_tmp-1].ui8_starting_total_duration + 
                            (UINT64)pt_stts_tbl[ui4_tmp-1].ui4_sample_count
                                * pt_stts_tbl[ui4_tmp-1].ui4_sample_duration;
        }
    }
    _mp4_midxbuld_mem_free(pt_midxbuld_obj, pr_stts_buf);
#else
    for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
    {
        pf_input4(pt_midxbuld_obj, &(pt_stts_tbl[ui4_tmp].ui4_sample_count));
        pf_input4(pt_midxbuld_obj, &(pt_stts_tbl[ui4_tmp].ui4_sample_duration));
        if (ui4_tmp == 0)
        {
            pt_stts_tbl[ui4_tmp].ui4_starting_sample = 1;
            pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 0;
        }
        else
        {
            pt_stts_tbl[ui4_tmp].ui4_starting_sample = 
                                    pt_stts_tbl[ui4_tmp-1].ui4_starting_sample
                                    + pt_stts_tbl[ui4_tmp-1].ui4_sample_count;
            pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 
                            pt_stts_tbl[ui4_tmp-1].ui8_starting_total_duration + 
                            (UINT64)pt_stts_tbl[ui4_tmp-1].ui4_sample_count
                                * pt_stts_tbl[ui4_tmp-1].ui4_sample_duration;
        }
    }
#endif
    for(ui1_idx = 0; 
        ui1_idx < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; 
        ui1_idx++)
    {
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
        }
        if (pt_key_ext_tbl->ui4_track_id
                    == pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_strm_id)
        {
            break; /* get related source index */
        } 
    }
        
    /*caculate ctts start*/   
#if (ENABLE_CTTS == 1)
    if(t_cur_track.t_ctts.b_is_box_exist)
    {
        if(mp4_midxbuld_init_ctts_tbl(pt_midxbuld_obj, 
                                      pt_key_ext_tbl, 
                                      &t_cur_track) == MIDXBULDR_OK)
        {
            pt_ctts_item_buf = pt_key_ext_tbl->t_ctts_buf.pt_ctts_item_buf;
        }
        else
        {
            return MIDXBULDR_INITIAL_ERR;
        }
    }
#endif
    ui4_k = 0;
    if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
    {
        return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
    }
    /* check the dividend can't equal zero */
    if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale == 0)
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return MIDXBULDR_INITIAL_ERR;
    }
    for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
    {
        ui4_key_sample_idx = pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
        for(; ui4_k < ui4_entry_count; ui4_k++)
        {
            if ((ui4_key_sample_idx >= pt_stts_tbl[ui4_k].ui4_starting_sample) &&
                (ui4_key_sample_idx < (pt_stts_tbl[ui4_k].ui4_starting_sample
                                        + pt_stts_tbl[ui4_k].ui4_sample_count)))
                {
                    break;
                }
        }
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stts_entry = ui4_k;
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui4_stts_entry_sample
                                = pt_stts_tbl[ui4_k].ui4_starting_sample;
        pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum
                  = pt_stts_tbl[ui4_k].ui8_starting_total_duration + 
                    (UINT64)(ui4_key_sample_idx - pt_stts_tbl[ui4_k].ui4_starting_sample)
                        *pt_stts_tbl[ui4_k].ui4_sample_duration;
        ui8_time = (UINT64)MIDXBULD_SYSTEM_CLOCK_FREQUENCY
                    * pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;
#if (ENABLE_CTTS == 1)      
        if(t_cur_track.t_ctts.b_is_box_exist)
        {            
            INT64   i8_ctts_ofst    = 0;
            INT64   i8_cur_dur_sum  = 0;
            for(ui4_i=ui4_last_ctts_idx; ui4_i < pt_key_ext_tbl->t_ctts_buf.ui4_ctts_entry_num; ui4_i ++)
            {
                if( ui4_key_sample_idx <= pt_ctts_item_buf[ui4_i].ui4_sample_count)
                {                    
                    ui4_last_ctts_idx = ui4_i;
                    break;                    
                }
            } 
            i8_ctts_ofst    = (INT64)((INT32)pt_ctts_item_buf[ui4_i].ui4_sample_offset); 
            i8_cur_dur_sum  = (INT64)pt_key_ext_tbl->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;
            
            if((i8_ctts_ofst + i8_cur_dur_sum) < 0)
            {
                ui8_time = (UINT64)i8_cur_dur_sum;
            }
            else
            {
                ui8_time = (UINT64)(i8_ctts_ofst + i8_cur_dur_sum);
            }
                
            ui8_time = ui8_time * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        }
#endif
        pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_pts = 
                        (UINT32)_mm_div64(ui8_time,
                                          pt_midxbuld_obj->t_src_info
                                                    .t_es_map[ui1_idx].ui4_scale, 
                                          NULL);  

    }
    _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);
    
    /* cal sample size */
    mp4_key_ext_cal_size(pt_midxbuld_obj, pt_key_ext_tbl, t_cur_track);

    /* cal offset */
    i4_ret = mp4_key_ext_cal_offset( pt_midxbuld_obj, pt_key_ext_tbl, t_cur_track);
    if (i4_ret != MIDXBULDR_OK)
    {
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        return i4_ret;
    }
    
    /* other key extention table audio, sbtl;*/
    for(ui1_j = 0; ui1_j < pt_mp4_handler->ui1_valid_track_cnt; ui1_j++)
    {
        if(ui1_j >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
        }
        if (pt_key_info->ui4_strm_id
                        == pt_mp4_handler->t_track_info_box[ui1_j].ui4_track_id)
        {
            continue;
        }
        else
        {
            t_cur_track = pt_mp4_handler->t_track_info_box[ui1_j];
        }
        
        for(ui4_k=0; ui4_k<pt_mp4_handler->ui1_valid_track_cnt; ui4_k++)
        {
            if(pt_midxbuld_obj->t_src_info.t_es_map[ui4_k].ui4_strm_id == t_cur_track.ui4_track_id)
            {
                break;
            }
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui4_k].b_is_pcm_aud)
        {
            i4_ret = mp4_midxbuld_pcm_audio_sample_table_build( pt_midxbuld_obj,
                                                                &pt_mp4_handler->t_track_info_box[ui1_j]);
            if(i4_ret != MIDXBULDR_OK)
            {
            /* Fail allowed ??? */
            }
            continue;
        }
        
        pt_key_ext_tbl2 = (MP4_IDXBULD_KEY_EXT_TBL_T*)
                    _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_TBL_T), 
                                                MM_MEM_MIDX_TAG);
        if (pt_key_ext_tbl2 == NULL)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_key_ext_tbl2, 0, sizeof(MP4_IDXBULD_KEY_EXT_TBL_T));
        pt_key_ext_tbl2->ui4_track_id = t_cur_track.ui4_track_id;
        pt_key_ext_tbl2->ui8_base_offset = 0;
        /*navy 080610 modified for VideoLen>AudioLen*/
        pt_key_ext_tbl2->ui4_key_num = ui4_total_key_num;
        pt_key_ext_tbl2->pt_next = NULL;
        pt_key_ext_tbl2->pt_key_entry = (MP4_MIDXBULD_ELMT_STRM_ENTRY_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T)*ui4_total_key_num, 
                                        MM_MEM_MIDX_TAG);
        pt_key_ext_tbl2->pt_key_ext = (MP4_IDXBULD_KEY_EXT_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_T)*ui4_key_num, 
                                        MM_MEM_MIDX_TAG);
        if ((pt_key_ext_tbl2->pt_key_entry == NULL)
            || (pt_key_ext_tbl2->pt_key_ext == NULL))
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_key_ext_tbl2->pt_key_entry, 
                 0, 
                 sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_total_key_num);
        x_memset(pt_key_ext_tbl2->pt_key_ext, 
                 0, 
                 sizeof(MP4_IDXBULD_KEY_EXT_T) * ui4_key_num);
    /* TBD: add here ??? */
#if (ENABLE_CTTS == 1)
    if(t_cur_track.t_ctts.b_is_box_exist)
    {
        if(mp4_midxbuld_init_ctts_tbl(pt_midxbuld_obj, 
                                      pt_key_ext_tbl2, 
                                      &t_cur_track) == MIDXBULDR_OK)
        {
            pt_ctts_item_buf = pt_key_ext_tbl2->t_ctts_buf.pt_ctts_item_buf;
        }
        else
        {
            return MIDXBULDR_INITIAL_ERR;
        }
    }
#endif
        
        /* seek to 'time to sample'  box to cal pts */
        pf_set_pos(pt_midxbuld_obj, 
                    (INT64)(t_cur_track.t_stts.ui8_box_offset + 12), 
                    FM_SEEK_BGN);
        /* get Time to Sample entry count */
        pf_input4(pt_midxbuld_obj, &ui4_entry_count);
        pt_stts_tbl = (MP4_STTS_TBL_T*)_mm_util_mem_alloc_with_tag(
                                    sizeof(MP4_STTS_TBL_T) * ui4_entry_count, 
                                    MM_MEM_MIDX_TAG);
        if (pt_stts_tbl == NULL)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return MIDXBULDR_OUT_OF_MEMORY;
        }

#if (OPTIMAZE_CODE == 1)
        if(MIDXBULDR_OK != _mp4_midxbuld_mem_alloc(
                                     pt_midxbuld_obj,
                                     (VOID*)&pr_stts_buf,
                                     sizeof(MP4_RAW_STTS_TBL_T)*ui4_entry_count))
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                pr_stts_buf, 
                                sizeof(MP4_RAW_STTS_TBL_T)*ui4_entry_count, 
                                sizeof(MP4_RAW_STTS_TBL_T)*ui4_entry_count, 
                                &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
        {
            pt_stts_tbl[ui4_tmp].ui4_sample_count
                     = LITTLE_TO_BIG_DWORD(pr_stts_buf[ui4_tmp].ui4_sample_count);
            pt_stts_tbl[ui4_tmp].ui4_sample_duration
                     = LITTLE_TO_BIG_DWORD(pr_stts_buf[ui4_tmp].ui4_sample_duration);
            if (ui4_tmp == 0)
            {
                pt_stts_tbl[ui4_tmp].ui4_starting_sample = 1;
                pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 0;
            }
            else
            {
                pt_stts_tbl[ui4_tmp].ui4_starting_sample = 
                                pt_stts_tbl[ui4_tmp-1].ui4_starting_sample 
                                    + pt_stts_tbl[ui4_tmp-1].ui4_sample_count;
                pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 
                    pt_stts_tbl[ui4_tmp-1].ui8_starting_total_duration + 
                    (UINT64)pt_stts_tbl[ui4_tmp-1].ui4_sample_count
                        * pt_stts_tbl[ui4_tmp-1].ui4_sample_duration;
            }
        }
        _mp4_midxbuld_mem_free(pt_midxbuld_obj, pr_stts_buf);
#else
        for(ui4_tmp = 0; ui4_tmp < ui4_entry_count; ui4_tmp++)
        {
            pf_input4(pt_midxbuld_obj, &(pt_stts_tbl[ui4_tmp].ui4_sample_count));
            pf_input4(pt_midxbuld_obj, &(pt_stts_tbl[ui4_tmp].ui4_sample_duration));
            if (ui4_tmp == 0)
            {
                pt_stts_tbl[ui4_tmp].ui4_starting_sample = 1;
                pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 0;
            }
            else
            {
                pt_stts_tbl[ui4_tmp].ui4_starting_sample = 
                                    pt_stts_tbl[ui4_tmp-1].ui4_starting_sample
                                        + pt_stts_tbl[ui4_tmp-1].ui4_sample_count;
                pt_stts_tbl[ui4_tmp].ui8_starting_total_duration = 
                            pt_stts_tbl[ui4_tmp-1].ui8_starting_total_duration + 
                            (UINT64)pt_stts_tbl[ui4_tmp-1].ui4_sample_count
                                * pt_stts_tbl[ui4_tmp-1].ui4_sample_duration;
            }
        }
#endif
        for(ui1_idx = 0; 
            ui1_idx < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; 
            ui1_idx++)
        {
            if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
            {
                return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
            }
            if (pt_key_ext_tbl2->ui4_track_id
                    == pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_strm_id)
            {
                break; /* get related source index */
            } 
        }
        ui4_k = 0;
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return MIDXBULDR_INTERNAL_ERR;/*----TBD----*/
        }
        /* check the dividend can't equal zero */
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale == 0)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);
            return MIDXBULDR_INITIAL_ERR;
        }
        for(ui4_tmp = 0; ui4_tmp < ui4_key_num; ui4_tmp++)
        {
            /* get key pts first */
            ui4_key_sample_pts
                = pt_mp4_handler->pt_key_ext_tbl->pt_key_entry[ui4_tmp].ui4_pts;
            ui8_time = (UINT64)ui4_key_sample_pts
                        * pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale;
            ui8_tmp_duration = _mm_div64(ui8_time,
                                                MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                                NULL);                            
            
            for(; ui4_k < ui4_entry_count; ui4_k++)
            {
                if ((ui8_tmp_duration >= pt_stts_tbl[ui4_k].ui8_starting_total_duration) 
                    && (ui8_tmp_duration < (pt_stts_tbl[ui4_k].ui8_starting_total_duration
                                            + (UINT64)pt_stts_tbl[ui4_k].ui4_sample_count
                                            * pt_stts_tbl[ui4_k].ui4_sample_duration)))
                    {
                        break;
                    }
            }
            if(ui4_k >= ui4_entry_count)
            {
                /*ui4_k = ui4_entry_count - 1;*/
                /*navy 080610 modified for VideoLen>AudioLen*/
                pt_key_ext_tbl2->ui4_key_num = ui4_tmp;
                break;
            }
            
            /* check the dividend can't equal zero */
            if(pt_stts_tbl[ui4_k].ui4_sample_duration == 0)
            {
                _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
                _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);
                return MIDXBULDR_INITIAL_ERR;
            }
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_key_ext_sample
                    = pt_stts_tbl[ui4_k].ui4_starting_sample
                        + (UINT32)_mm_div64(ui8_tmp_duration - pt_stts_tbl[ui4_k].ui8_starting_total_duration,
                                            pt_stts_tbl[ui4_k].ui4_sample_duration, 
                                            NULL);
            ui4_key_sample_idx
                        = pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_key_ext_sample;
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_stts_entry = ui4_k;
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui4_stts_entry_sample
                        = pt_stts_tbl[ui4_k].ui4_starting_sample;
            pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum
                        = pt_stts_tbl[ui4_k].ui8_starting_total_duration + 
                            (UINT64)(ui4_key_sample_idx-pt_stts_tbl[ui4_k].ui4_starting_sample)
                                *pt_stts_tbl[ui4_k].ui4_sample_duration;
            ui8_time = (UINT64)MIDXBULD_SYSTEM_CLOCK_FREQUENCY
                    * pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;

#if (ENABLE_CTTS == 1)      
        if(t_cur_track.t_ctts.b_is_box_exist)
        {  
            UINT32  ui4_i       = 0;
            INT64   i8_ctts_ofst    = 0;
            INT64   i8_cur_dur_sum  = 0;
            
            for(ui4_i = 0; ui4_i < pt_key_ext_tbl2->t_ctts_buf.ui4_ctts_entry_num; ui4_i ++)
            {
                if( ui4_key_sample_idx <= pt_ctts_item_buf[ui4_i].ui4_sample_count)
                {                    
                    break;                    
                }
            } 
            i8_ctts_ofst    = (INT64)((INT32)pt_ctts_item_buf[ui4_i].ui4_sample_offset); 
            i8_cur_dur_sum  = (INT64)pt_key_ext_tbl2->pt_key_ext[ui4_tmp].ui8_key_ext_duration_sum;
            if((i8_ctts_ofst + i8_cur_dur_sum) < 0)
            {
                ui8_time = (UINT64)i8_cur_dur_sum;
            }
            else
            {
                ui8_time = (UINT64)(i8_ctts_ofst + i8_cur_dur_sum);
            }
                
            ui8_time = ui8_time * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        }
#endif
            pt_key_ext_tbl2->pt_key_entry[ui4_tmp].ui4_pts = 
                (UINT32)_mm_div64(ui8_time,
                                  pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_scale, 
                                  NULL);                            
            
            
        }
        _mm_util_mem_free_with_tag(pt_stts_tbl, MM_MEM_MIDX_TAG);

        /* cal sample size */
        mp4_key_ext_cal_size(pt_midxbuld_obj, pt_key_ext_tbl2, t_cur_track);
        /* cal sample offset */
        i4_ret = mp4_key_ext_cal_offset(pt_midxbuld_obj, 
                                        pt_key_ext_tbl2, 
                                        t_cur_track);
        if (i4_ret != MIDXBULDR_OK)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
            return i4_ret;
        }
        pt_key_ext_tbl->pt_next = pt_key_ext_tbl2;
        pt_key_ext_tbl = pt_key_ext_tbl->pt_next;
    }
    
    return MIDXBULDR_OK;
}


#define dwBigFourCC(a,b,c,d)    ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
#define MP4_BOX_MVHD       dwBigFourCC('m', 'v', 'h', 'd')
#define MP4_BOX_MOOV       dwBigFourCC('m', 'o', 'o', 'v')

UINT64 _ui8_get_single_file_duration(MIDXBULD_OBJ_T*    pt_midxbuld_obj)
{
    setpos_fct                      pf_set_pos;  
    input4bytes_fct                 pf_inp4byte;  
    copybytes_fct                   pf_copybyte;
    INT32                           i4_ret;
    UINT32                          ui4_box_type;
    UINT32                          ui4_read_size;
    UINT32                          ui4_high_word;
    UINT32                          ui4_low_word;
    UINT64                          ui8_cur_pos  = 0;
    UINT32                          ui4_timescale= 0;
    UINT64                          ui8_duration = 0;
    UINT32                          ui4_box_size = 0;
    
    
    /* check parameter */
    if (pt_midxbuld_obj == NULL)
    {
        return 0;
    }
    if ((pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos == NULL)
        || (pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes == NULL))
    {
        return 0;
    }
    
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_inp4byte = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    
    ui8_cur_pos = pt_midxbuld_obj->t_src_info.ui8_idx_offset + 8;
    i4_ret = pf_set_pos(pt_midxbuld_obj , ui8_cur_pos, FM_SEEK_BGN);
    if(FMR_OK != i4_ret)
    {
        return 0;
    }
    
    while(1)
    {
        i4_ret = pf_inp4byte(pt_midxbuld_obj, &ui4_box_size);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        i4_ret = pf_inp4byte(pt_midxbuld_obj, &ui4_box_type);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }

        if (ui4_box_type == MP4_BOX_MVHD)
        {
            UINT8 aui1_box_buf[128] = {0};
            i4_ret = pf_copybyte(pt_midxbuld_obj, 
                                 aui1_box_buf, 
                                 128, 
                                 ui4_box_size - 8, 
                                 &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                return 0;
            }
            if (aui1_box_buf[0] == 1)/*Version*/
            {
                LOADB_DWRD(aui1_box_buf + 20, ui4_timescale);
                LOADB_DWRD(aui1_box_buf + 24, ui4_high_word);
                LOADB_DWRD(aui1_box_buf + 28, ui4_low_word);
                ui8_duration = ((UINT64)ui4_high_word<<32) | ui4_low_word;
            }
            else
            {
                LOADB_DWRD(aui1_box_buf + 12, ui4_timescale);
                LOADB_DWRD(aui1_box_buf + 16, ui4_high_word);
                ui8_duration = ui4_high_word;
            }
            
            ui8_duration = _mm_div64(ui8_duration * MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                     ui4_timescale, 
                                     NULL);
            break;
        }
        pf_set_pos(pt_midxbuld_obj, ui4_box_size - 8, FM_SEEK_CUR);
        
        ui8_cur_pos += ui4_box_size;
        
        if(ui8_cur_pos >= pt_midxbuld_obj->t_src_info.ui8_idx_offset + pt_midxbuld_obj->t_src_info.ui8_idx_size)
        {
            break;
        }
    }
    
    i4_ret = pf_set_pos(pt_midxbuld_obj , pt_midxbuld_obj->t_src_info.ui8_idx_offset, FM_SEEK_BGN);

    return ui8_duration;
}

INT32 _ui4_get_single_file_idx_info(MIDXBULD_OBJ_T*    pt_midxbuld_obj)
{
    copybytes_fct                   pf_copybyte     = NULL;  
    setpos_fct                      pf_set_pos      = NULL;  
    input4bytes_fct                 pf_inp4byte     = NULL;  
    MP4_IDXBULD_OBJ_T*              pt_mp4_hdlr_obj = NULL;
    INT32                           i4_ret          = 0;
    UINT32                          ui4_box_size    = 0;
    UINT64                          ui8_box_size    = 0;
    UINT32                          ui4_box_type    = 0;
    UINT32                          ui4_high_word   = 0;
    UINT32                          ui4_low_word    = 0;
    UINT64                          ui8_cur_pos     = 0;
    UINT8                           ui1_file_idx    = 0;
    MP4_SINGLE_FILE_INFO_T*         pt_file_info    = NULL;
    
    
    /* check parameter */
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    if ((pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos == NULL)
        || (pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    
    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_inp4byte = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    
    pt_mp4_hdlr_obj = pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_hdlr_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }

            
    ui1_file_idx    = pt_mp4_hdlr_obj->ui1_curr_file_idx;
    //pt_midxbuld_obj->t_src_info.ui8_idx_offset = 0x64;//ui8_cur_pos - 8;
    pt_file_info = &(((MP4_MULTI_FILE_INFO_T*)pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_multi_file_info)->at_file_info[ui1_file_idx]);
    if(((MP4_MULTI_FILE_INFO_T*)pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_multi_file_info)->ui1_file_num == 1)
    {
        *pt_file_info = pt_mp4_hdlr_obj->t_cur_file_info;
        
        return MIDXBULDR_OK;
    }

    ui8_cur_pos = pt_file_info->ui8_file_start_pos;
            
    i4_ret = pf_set_pos(pt_midxbuld_obj , ui8_cur_pos, FM_SEEK_BGN);    
    if(FMR_OK != i4_ret)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    while (i4_ret == FMR_OK)
    {
        i4_ret = pf_inp4byte(pt_midxbuld_obj, &ui4_box_size);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }
        i4_ret = pf_inp4byte(pt_midxbuld_obj, &ui4_box_type);
        if (i4_ret != INPUTR_OK)
        {
            break;
        }

        if(ui4_box_type == MP4_BOX_MOOV)
        {
            UINT64  ui8_tmp_dur = 0;
            UINT64  ui8_adjust  = 0;
            pt_midxbuld_obj->t_src_info.ui8_idx_offset = ui8_cur_pos;      
            pt_file_info->ui8_file_moov_pos  = ui8_cur_pos;
            pt_file_info->ui8_file_moov_size = ui4_box_size;
            if(ui1_file_idx == 0)
            {
                pt_file_info->ui8_file_start_pts = 0;
            }
            else
            {
                MP4_SINGLE_FILE_INFO_T* pt_tmp = NULL;
                pt_tmp = &(((MP4_MULTI_FILE_INFO_T*)pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_multi_file_info)->at_file_info[ui1_file_idx - 1]);
                pt_file_info->ui8_file_start_pts
                    = pt_tmp->ui8_file_start_pts + pt_tmp->ui8_file_duration;
            }
            ui8_tmp_dur = _ui8_get_single_file_duration(pt_midxbuld_obj);
            ui8_adjust = ui8_tmp_dur - pt_file_info->ui8_file_duration;
            if(ui8_adjust != 0)
            {
                pt_file_info->ui8_file_duration += ui8_adjust;
                for(ui1_file_idx = pt_mp4_hdlr_obj->ui1_curr_file_idx + 1; 
                    ui1_file_idx<((MP4_MULTI_FILE_INFO_T*)pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_multi_file_info)->ui1_file_num; 
                    ui1_file_idx++)
                {
                    pt_file_info++;
                    pt_file_info->ui8_file_start_pts += ui8_adjust;
                }                
                ui1_file_idx    = pt_mp4_hdlr_obj->ui1_curr_file_idx;
                pt_file_info = &(((MP4_MULTI_FILE_INFO_T*)pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_multi_file_info)->at_file_info[ui1_file_idx]);
            }
            pt_mp4_hdlr_obj->t_cur_file_info.ui8_file_duration = pt_file_info->ui8_file_duration;
            break;            
        }
        
    
        if (ui4_box_size == 1) /* 64-bit big size */
        {
            pf_inp4byte(pt_midxbuld_obj, &ui4_high_word);
            pf_inp4byte(pt_midxbuld_obj, &ui4_low_word);
            
            ui8_box_size = ((UINT64)ui4_high_word<<32) + ui4_low_word;
            i4_ret = pf_set_pos(pt_midxbuld_obj , (INT64)ui8_box_size - 16, FM_SEEK_CUR);            
            ui8_cur_pos += ui8_box_size;
            if(i4_ret != FMR_OK)
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
        }
        else if(ui4_box_size == 0) /* box extends to end of file */
        {
            i4_ret = FMR_EOF;
        }
        else
        {
            i4_ret = pf_set_pos(pt_midxbuld_obj , (INT64)ui4_box_size - 8, FM_SEEK_CUR);           
            ui8_cur_pos += ui4_box_size;
            if(i4_ret != FMR_OK)
            {
                break;
            }
        }
    }
    if(i4_ret != MIDXBULDR_OK)
    {
        return i4_ret;
    }
    do
    {
        MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
        
        pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
        if (pt_cmov_info->b_is_cmov_box_exist && (pt_cmov_info->pui1_decomp_cmov_data != NULL))
        {
            _mm_util_mem_free(pt_cmov_info->pui1_decomp_cmov_data);
            pt_cmov_info->pui1_decomp_cmov_data = NULL;
        }
        if(pt_cmov_info->b_is_cmov_box_exist)
        {
            UINT32 ui4_read_size = 0;
            pt_cmov_info->ui8_cmov_box_pos
                                        = pt_file_info->ui8_file_moov_pos;      
            pt_cmov_info->ui4_cmov_box_size
                                        = (UINT32)pt_file_info->ui8_file_moov_size;    
            pt_cmov_info->pui1_raw_cmov_data = NULL;

            pt_cmov_info->ui4_cmov_box_decomp_size
                                        = (UINT32)pt_file_info->ui8_file_moov_size;
            pt_cmov_info->pui1_decomp_cmov_data
                                        = _mm_util_mem_alloc(sizeof(UINT8)*pt_file_info->ui8_file_moov_size);

            if(pt_cmov_info->pui1_decomp_cmov_data == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            
            i4_ret = pf_set_pos(pt_midxbuld_obj, pt_file_info->ui8_file_moov_pos, FM_SEEK_BGN);
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("ERROR: seek to moov start fail. \r\n"));
                return MIDXBULDR_INTERNAL_ERR;
            }
            
            i4_ret = pf_copybyte(pt_midxbuld_obj,
                                 pt_cmov_info->pui1_decomp_cmov_data, 
                                 pt_cmov_info->ui4_cmov_box_size, 
                                 pt_cmov_info->ui4_cmov_box_size, 
                                 &ui4_read_size);
            if((i4_ret != INPUTR_OK)
                || (ui4_read_size != pt_cmov_info->ui4_cmov_box_size))
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
        }
    }while(0);
        
    return i4_ret;
}
INT32 mp4_single_file_info_alloc(
                         MIDXBULD_KEY_INFO_T* pt_key_info, 
                         VOID*                pv_obj)
{
    INT32                   i4_ret;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler = NULL;    
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext = NULL;
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
#endif
    PIFF_BOX_INET_INFO_T*   pt_moof_info = NULL;
    
    DBG_INFO(("API: mp4_single_file_info_alloc begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }

    i4_ret = _ui4_get_single_file_idx_info(pt_midxbuld_obj);
    if (i4_ret != MIDXBULDR_OK)
    {
#ifndef __KERNEL__
    x_dbg_stmt("============= %s   line:%d  ERROR: i4_ret=%d !!!!!!!!!!!!!! ==============\r\n",__func__, __LINE__,i4_ret);
#endif
        return MIDXBULDR_INTERNAL_ERR;
    }

    
#if (ENABLE_CMOV == 1)
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    pt_moof_info = (PIFF_BOX_INET_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_piff_inet_info);
    pt_mp4_handler->piff_inet_moof_info = pt_moof_info;
#endif


    pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                            (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
    if(pt_key_tbl_ext == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_tbl_ext, 0, sizeof(MIDXBULD_KEY_TBL_EXT_T));
    pt_mp4_handler->pt_key_tbl_ext = pt_key_tbl_ext;
    pt_key_tbl_ext->pt_next = NULL;

    i4_ret = mp4_midx_box_parse(pt_midxbuld_obj);
    if (i4_ret != MIDXBULDR_OK)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    if(pt_moof_info->b_is_piff_file)
    {      
        if(pt_moof_info->b_is_mfra_key_exist == FALSE)
        {
            pt_moof_info->b_is_inet_data = TRUE;        
#ifndef __KERNEL__
            x_dbg_stmt("###### Error: PIFF file without key table not support now ######## \r\n");
#endif
            //return MIDXBULDR_FMT_NOT_SUPPORT;
        }
        if(!pt_moof_info->b_is_inet_data) // from inet not to build key tbl;
        {
            if(!pt_mp4_handler->t_mln_moov_info.b_is_moov_empty &&
                pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)
            {
                i4_ret =  mp4_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj);
            }
            else
            {
                i4_ret = piff_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj);
            }
        }     
    }    
    else    
    {
        i4_ret =  mp4_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj);
    }
    if (i4_ret != MIDXBULDR_OK)
    {
        DBG_INFO(("key frame table alloc fail \r\n" ));
        return i4_ret;
    }
    pt_mp4_handler->ui1_curr_buld_progress = 10;/*--- 10% ---*/

    /* key frame ext table alloc and build */
    if(pt_moof_info->b_is_piff_file)    
    {      
        if(!pt_moof_info->b_is_inet_data)
        {
            if(!pt_mp4_handler->t_mln_moov_info.b_is_moov_empty &&
                pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)
            {
#if (ENABLE_CMOV == 1)
                if(pt_cmov_info->b_is_cmov_box_exist)
                {
                    i4_ret = mp4_cmov_midxbuld_key_ext_table_build(pt_midxbuld_obj, pt_key_info);
                }
                else
#endif
                {
                    i4_ret = mp4_midxbuld_key_ext_table_build(pt_midxbuld_obj, pt_key_info);
                }
                if(pt_mp4_handler->piff_inet_moof_info->b_is_mfra_key_exist)
                {   
                    i4_ret = piff_get_key_from_mfra(pt_midxbuld_obj, pt_key_info);
                }
                else
                {
                    //i4_ret = piff_get_key_info(pt_midxbuld_obj,  pt_key_info);
                }
            }
            else
            {
                i4_ret = piff_midxbuld_key_ext_table_build(pt_midxbuld_obj, pt_key_info);
            }
        }
    }
    else
#if (ENABLE_CMOV == 1)
    if(pt_cmov_info->b_is_cmov_box_exist)
    {
        i4_ret = mp4_cmov_midxbuld_key_ext_table_build(pt_midxbuld_obj, pt_key_info);
    }
    else
#endif
    {
        i4_ret = mp4_midxbuld_key_ext_table_build(pt_midxbuld_obj, pt_key_info);
    }
    pt_mp4_handler->ui1_curr_buld_progress = 30;/*--- 30% ---*/

    return i4_ret;
}

INT32 mp4_single_file_info_free(VOID*   pv_obj)
{
    MIDXBULD_OBJ_T*         pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler = NULL;

    DBG_INFO(("API: mp4_midxbuld_close begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)  
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;


    if (pt_midxbuld_obj->pv_handler_obj)
    {
        /* free all range objects */
        pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        while (pt_mp4_handler->ui1_range_count > 0)
        {
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                pt_mp4_handler->apt_range_obj[0]->ui4_range_id);
        }
        _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
            pt_mp4_handler->pt_key_range_obj->ui4_range_id);


        /* free key extension table */
        _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        pt_mp4_handler->pt_key_ext_tbl = NULL;
        
        _mm_util_mem_free_with_tag(pt_mp4_handler->pt_key_tbl_ext, MM_MEM_MIDX_TAG);
        pt_mp4_handler->pt_key_tbl_ext = NULL;
        
#if (ENABLE_CMOV == 1)
    {
        MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
        pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
        if (pt_cmov_info->b_is_cmov_box_exist && (pt_cmov_info->pui1_decomp_cmov_data != NULL))
        {
            _mm_util_mem_free(pt_cmov_info->pui1_decomp_cmov_data);
            pt_cmov_info->pui1_decomp_cmov_data = NULL;
        }
    }
#endif
    }
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and 
 *                                   the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_fopen(
                         CHAR*                ps_path_name, 
                         MIDXBULD_KEY_INFO_T* pt_key_info, 
                         VOID*                pv_obj)
{

#ifndef __NO_FM__

    INT32                   i4_ret  = 0;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler = NULL;    
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
    PIFF_BOX_INET_INFO_T*   pt_moof_info = NULL;

#endif
    
    DBG_INFO(("API: mp4_midxbuld_fopen begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) 
                        _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_OBJ_T), 
                                                    MM_MEM_MIDX_TAG);
    if (pt_mp4_handler == NULL)
    {
        DBG_INFO(("Alloc midxbuld obj fail \r\n" ));
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_handler, 0, sizeof(MP4_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_mp4_handler;
#if (ENABLE_CMOV == 1)
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    pt_moof_info = (PIFF_BOX_INET_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_piff_inet_info);
    pt_mp4_handler->piff_inet_moof_info = pt_moof_info;
#endif

#if (OPTIMAZE_CODE == 1)
#if (ENABLE_CMOV == 1)
    if(!pt_cmov_info->b_is_cmov_box_exist)
#endif
    {
        pt_mp4_handler->t_mem_buf.pv_mem_buffer
              = _mm_util_mem_alloc_with_tag(MP4_MIDXBULD_BUF_SIZE, 
                                            MM_MEM_MIDX_TAG);
        if (pt_mp4_handler->t_mem_buf.pv_mem_buffer == NULL)
        {
            pt_midxbuld_obj->pv_handler_obj = NULL;
            _mm_util_mem_free_with_tag(pt_mp4_handler, MM_MEM_MIDX_TAG);
            return MIDXBULDR_OUT_OF_MEMORY;
        }
    }
#endif    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                       ps_path_name, 
                       FM_READ_ONLY, 0777, 
                       FALSE, 
                       &(pt_midxbuld_obj->h_file));
    if (i4_ret != FMR_OK)
    {
        pt_midxbuld_obj->pv_handler_obj = NULL;
        _mm_util_mem_free_with_tag(pt_mp4_handler, MM_MEM_MIDX_TAG);
        return MIDXBULDR_INITIAL_ERR;
    }

    pt_mp4_handler->pt_key_info
                = _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_KEY_INFO_T), 
                                              MM_MEM_MIDX_TAG);
    if(pt_mp4_handler->pt_key_info == NULL)
    {
        pt_midxbuld_obj->pv_handler_obj = NULL;
        _mm_util_mem_free_with_tag(pt_mp4_handler, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    pt_mp4_handler->ui1_range_count     = 0;
    pt_mp4_handler->ui1_curr_file_idx   = 0;
    x_memcpy(pt_mp4_handler->pt_key_info, pt_key_info, sizeof(MIDXBULD_KEY_INFO_T));
    
    pt_mp4_handler->t_cur_file_info.ui8_file_start_pos = 0;
    pt_mp4_handler->t_cur_file_info.ui8_file_start_pts = 0;
    pt_mp4_handler->t_cur_file_info.ui8_file_moov_pos
                        = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
    pt_mp4_handler->t_cur_file_info.ui8_file_moov_size
                        = pt_midxbuld_obj->t_src_info.ui8_idx_size;
        
    ui1_idx_range_id_last               = 1;

    i4_ret = mp4_single_file_info_alloc(pt_key_info, pv_obj);

    return i4_ret;
    
#else    

    return MIDXBULDR_INTERNAL_ERR;

#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and 
 *                                   the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_mopen(
                         VOID*                pv_mem_data,
                         SIZE_T               z_mem_leng,
                         MIDXBULD_KEY_INFO_T* pt_key_info,
                         VOID*                pv_obj)
{
    INT32                   i4_ret;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler = NULL;
    MEDIA_FEEDER_T*         pt_src_feeder = NULL;
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
#endif
    PIFF_BOX_INET_INFO_T*   pt_moof_info = NULL;

    DBG_INFO(("API: mp4_midxbuld_mopen begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    if (z_mem_leng == 0)
    {
        pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;

        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes =
            pt_src_feeder->t_feeder_le_fct_tbl.pf_copybytes;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input1;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input3;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_input4;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_set_pos;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos =
            pt_src_feeder->t_feeder_be_fct_tbl.pf_get_pos;
    }
    else
    {
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_minput1bytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_b_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_b_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_msetpos_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_mgetpos_fct;
    }

    pt_midxbuld_obj->i8_buff_pos = 0;

    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) 
                        _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_OBJ_T), 
                                                    MM_MEM_MIDX_TAG);
    if (pt_mp4_handler == NULL)
    {
        DBG_INFO(("Alloc midxbuld obj fail \r\n" ));
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_mp4_handler, 0, sizeof(MP4_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_mp4_handler;
#if (ENABLE_CMOV == 1)
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
#endif
    pt_moof_info = (PIFF_BOX_INET_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_piff_inet_info);    
    pt_mp4_handler->piff_inet_moof_info = pt_moof_info;

#if (OPTIMAZE_CODE == 1)
#if (ENABLE_CMOV == 1)
    if(!pt_cmov_info->b_is_cmov_box_exist)
#endif
    {
        pt_mp4_handler->t_mem_buf.pv_mem_buffer
              = _mm_util_mem_alloc_with_tag(MP4_MIDXBULD_BUF_SIZE, 
                                            MM_MEM_MIDX_TAG);
        if (pt_mp4_handler->t_mem_buf.pv_mem_buffer == NULL)
        {
            pt_midxbuld_obj->pv_handler_obj = NULL;
            _mm_util_mem_free_with_tag(pt_mp4_handler, MM_MEM_MIDX_TAG);
            return MIDXBULDR_OUT_OF_MEMORY;
        }
    }
#endif   

    pt_mp4_handler->pt_key_info
                = _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_KEY_INFO_T), 
                                              MM_MEM_MIDX_TAG);
    if(pt_mp4_handler->pt_key_info == NULL)
    {
        pt_midxbuld_obj->pv_handler_obj = NULL;
        _mm_util_mem_free_with_tag(pt_mp4_handler, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    pt_mp4_handler->ui1_range_count     = 0;
    pt_mp4_handler->ui1_curr_file_idx   = 0;
    x_memcpy(pt_mp4_handler->pt_key_info, pt_key_info, sizeof(MIDXBULD_KEY_INFO_T));
    pt_mp4_handler->t_cur_file_info.ui8_file_start_pos = 0;
    pt_mp4_handler->t_cur_file_info.ui8_file_start_pts = 0;
    pt_mp4_handler->t_cur_file_info.ui8_file_duration  = pt_midxbuld_obj->t_src_info.ui4_total_duration;
    pt_mp4_handler->t_cur_file_info.ui8_file_moov_pos
                        = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
    pt_mp4_handler->t_cur_file_info.ui8_file_moov_size
                        = pt_midxbuld_obj->t_src_info.ui8_idx_size;
        
    ui1_idx_range_id_last               = 1;

    i4_ret = mp4_single_file_info_alloc(pt_key_info, pv_obj);

#ifndef __KERNEL__
    x_dbg_stmt("=======%s line %d end=======\n", __func__, __LINE__);
#endif
    return i4_ret;
}    

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_close(VOID* pv_obj)
{
    UINT8                   ui1_track_cnt   = 0;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler = NULL;
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
#endif

    DBG_INFO(("API: mp4_midxbuld_close begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)  
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    if (pt_midxbuld_obj->h_file)
    {        
#ifndef __NO_FM__        
        x_fm_close(pt_midxbuld_obj->h_file);
#endif
        pt_midxbuld_obj->h_file = NULL_HANDLE;
    }
#if (ENABLE_CMOV == 1)
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
#endif
    if (pt_midxbuld_obj->pv_handler_obj)
    {
        /* free all range objects */
        pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        while (pt_mp4_handler->ui1_range_count > 0)
        {
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                pt_mp4_handler->apt_range_obj[0]->ui4_range_id);
        }
        if(pt_mp4_handler->pt_key_range_obj != NULL)
        {
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                pt_mp4_handler->pt_key_range_obj->ui4_range_id);
        }


        /* free key extension table */
        if( pt_mp4_handler->pt_key_ext_tbl != NULL)
        {
            _mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);
        }
        if( pt_mp4_handler->pt_key_tbl_ext != NULL)
        {          
            _mm_util_mem_free_with_tag(pt_mp4_handler->pt_key_tbl_ext, MM_MEM_MIDX_TAG);
        }
        
#if (ENABLE_CMOV == 1)
        if(!pt_cmov_info->b_is_cmov_box_exist)
#endif
        {
#if (OPTIMAZE_CODE == 1)
            if(pt_mp4_handler->t_mem_buf.pv_mem_buffer != NULL)
            {
                _mm_util_mem_free_with_tag(pt_mp4_handler->t_mem_buf.pv_mem_buffer, 
                                            MM_MEM_MIDX_TAG);
            }
#endif
        }

        for(ui1_track_cnt = 0; ui1_track_cnt < pt_mp4_handler->ui1_valid_track_cnt; ui1_track_cnt++)
        {
            if(pt_mp4_handler->t_track_info_box[ui1_track_cnt].t_pcm_sam_inf.pat_aud_pcm_size_dur != NULL)
            {
                _mm_util_mem_free_with_tag(
                    pt_mp4_handler->t_track_info_box[ui1_track_cnt].t_pcm_sam_inf.pat_aud_pcm_size_dur, 
                    MM_MEM_MIDX_TAG);
                pt_mp4_handler->t_track_info_box[ui1_track_cnt].t_pcm_sam_inf.pat_aud_pcm_size_dur = NULL;
            }
            if(!pt_cmov_info->b_is_cmov_box_exist &&
                (pt_mp4_handler->t_track_info_box[ui1_track_cnt].t_pcm_sam_inf.paui4_ofst_buf != NULL))
            {
                _mm_util_mem_free_with_tag(
                    pt_mp4_handler->t_track_info_box[ui1_track_cnt].t_pcm_sam_inf.paui4_ofst_buf, 
                    MM_MEM_MIDX_TAG);
                pt_mp4_handler->t_track_info_box[ui1_track_cnt].t_pcm_sam_inf.paui4_ofst_buf = NULL;
            }
        }
        if(pt_mp4_handler->pt_key_info != NULL)
        {
            _mm_util_mem_free_with_tag(pt_mp4_handler->pt_key_info, MM_MEM_MIDX_TAG);
        }
        if(pt_midxbuld_obj->pv_handler_obj != NULL )
        {
            _mm_util_mem_free_with_tag(pt_midxbuld_obj->pv_handler_obj, MM_MEM_MIDX_TAG);
            pt_midxbuld_obj->pv_handler_obj = NULL;
        }
    }
    return MIDXBULDR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_start_indexing
 *
 * Description: This function start indexing
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_start_indexing(VOID* pv_obj, UINT32* pui4_range_id)
{
    INT32                   i4_ret          = 0;
    BOOL                    b_piff_use_common_idxing = FALSE;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj = NULL;
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*    pt_cmov_info = NULL;
#endif
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler  = NULL;
    PIFF_BOX_INET_INFO_T*   pt_moof_info    = NULL;
    

    DBG_INFO(("API: mp4_midxbuld_start_indexing  begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pv_obj == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    pt_mp4_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    pt_moof_info = pt_mp4_handler->piff_inet_moof_info;
    
    if(pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj != NULL)
    {
        if(pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file        && 
           !pt_mp4_handler->t_mln_moov_info.b_is_moov_empty           && 
           (pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj->ui4_end_sample 
               <= pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_sample_cnt))
        {
            b_piff_use_common_idxing = TRUE;
        }
    }
        
#if (ENABLE_CMOV == 1)
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
    /*build index array*/
    if(pt_cmov_info->b_is_cmov_box_exist)
    {
        i4_ret = mp4_cmov_midxbuld_indexing(pt_midxbuld_obj, pui4_range_id);
    }
    else
#endif
    if(pt_moof_info->b_is_piff_file)    
    {
        if(pt_moof_info->b_is_inet_data)
        {
            i4_ret = piff_inet_midxbuld_indexing(pt_midxbuld_obj, pui4_range_id);
        }
        else 
        {
            if(b_piff_use_common_idxing)
            {
                i4_ret = mp4_midxbuld_indexing(pt_midxbuld_obj, pui4_range_id);
            }
            else
            {
                i4_ret = piff_midxbuld_indexing(pt_midxbuld_obj, pui4_range_id);
            }
        }
    }
    else
    {
        i4_ret = mp4_midxbuld_indexing(pt_midxbuld_obj, pui4_range_id);
    }

    return i4_ret;
} 

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_get_info
 *
 * Description: This API retrieves the information of media idx builder handle. 
 *              It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, 
 *                          it will return MINFO_INSUFFICENT_MEM, 
 *                          and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_get_info(
                            VOID*                       pv_obj,   
                            MIDXBULD_GET_INFO_TYPE_T    e_info_type,  
                            VOID*                       pv_buf,     
                            SIZE_T*                     pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler = NULL;

    DBG_INFO(("API: mp4_midxbuld_get_info begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) 
        || (pz_buf_leng== NULL))
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL) 
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    switch(e_info_type)
    {
    case MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL:
        return mp4_midxbuld_get_idx_elmt_tbl(pt_midxbuld_obj, 
                                             (MIDXBULD_ELMT_TBL_INFO_T*) pv_buf);
    case MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST:
        return mp4_midxbuld_get_key_idx_tbl_list(pt_midxbuld_obj, 
                                                (MIDXBULD_KEY_TBL_INFO_T*)pv_buf);
    case MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS:
        *(UINT8*)pv_buf = pt_mp4_handler->ui1_curr_buld_progress;
        return MIDXBULDR_OK;
    default:
        return MIDXBULDR_INV_ARG;
    }
}

INT32 mp4_midxbuld_get_key_idx_tbl_list(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_KEY_TBL_INFO_T*  pt_key_tbl)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_obj = NULL;
    PIFF_BOX_INET_INFO_T*           pt_moof_info = NULL;
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    pt_moof_info = pt_mp4_handler->piff_inet_moof_info;
    
    pt_key_tbl->ui4_key_tbl_id = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id;
    if(pt_moof_info->b_is_inet_data)
    {
    
    }
    else
    {
    if (pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id
        == pt_key_tbl->ui4_key_tbl_id)
    {
        pt_tbl_obj = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj;
        pt_key_tbl->pt_key_tbl_ext = pt_mp4_handler->pt_key_tbl_ext;
    }
    else
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_key_tbl_obj = pt_mp4_handler->pt_key_tbl_ext;
    while(pt_tbl_obj != NULL)
    {
        pt_key_tbl_obj->t_elmt_tbl = pt_tbl_obj->t_elmt_tbl;
        if(pt_tbl_obj->ui4_tbl_id == pt_key_tbl->ui4_key_tbl_id)
        {
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info
                                = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry
                                = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        }
        else
        {
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info
                                = pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info;
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts 
                                = pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts;
        }
        pt_tbl_obj = pt_tbl_obj->pt_next;
        pt_key_tbl_obj = pt_key_tbl_obj->pt_next;
    }
    }

    return MIDXBULDR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_get_idx_elmt_tbl
 *
 * Description: This function get index element info
 *
 * Inputs:  pt_midxbuld_obj,  specifies index builder object
 *          pt_elmt_tbl,      specify the element table  
 *
 * Outputs: pt_elmt_tbl       element table info 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and  
 *                                the operationhas been completed.
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_get_idx_elmt_tbl(
                        MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                        MIDXBULD_ELMT_TBL_INFO_T*   pt_elmt_tbl)
{
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_obj = NULL;
    UINT8                           ui1_idx;
    BOOL                            b_id_match;

    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    pt_tbl_obj = NULL;
    b_id_match = FALSE;

    if ( pt_mp4_handler->pt_key_range_obj!=NULL && pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id
        == pt_elmt_tbl->ui4_tbl_id)
    {
        b_id_match = TRUE;
        pt_tbl_obj = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj;
    }
    else if (pt_mp4_handler->ui1_range_count > 0)
    {
        for (ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_range_count; ui1_idx++)
        {
            if(ui1_idx >= MP4_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            pt_obj = pt_mp4_handler->apt_range_obj[ui1_idx]->pt_tbl_obj;
            while (pt_obj != NULL)
            {
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
    if ((FALSE == b_id_match) || (pt_tbl_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_elmt_tbl->t_elmt_tbl.ui8_base_offset
                        = pt_tbl_obj->t_elmt_tbl.ui8_base_offset;
    pt_elmt_tbl->t_elmt_tbl.ui4_number_of_entry
                        = pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
    pt_elmt_tbl->t_elmt_tbl.e_tbl_type
                        = pt_tbl_obj->t_elmt_tbl.e_tbl_type;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.t_info
                        = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.pt_idx_entry 
                        = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_set_info
 *
 * Description: This API retrieves the information of media idx builder handle. 
 *              It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, 
 *                          it will return MINFO_INSUFFICENT_MEM, 
 *                          and pz_buf_leng will contain required length.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and 
 *                                the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_set_info(
                            VOID*                       pv_obj,   
                            MIDXBULD_SET_INFO_TYPE_T    e_info_type,  
                            VOID*                       pv_buf,     
                            SIZE_T*                     pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj = NULL;
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler = NULL;
    INT32                       i4_ret       = 0; 
    PIFF_BOX_INET_INFO_T*       pt_moof_info = NULL;
    DBG_INFO(("API: mp4_midxbuld_set_info begin \r\n" ));
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL) 
        || (pz_buf_leng== NULL))
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL) 
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    pt_moof_info = pt_mp4_handler->piff_inet_moof_info;
    switch(e_info_type)
    {
    case MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE:
        if(pt_moof_info->b_is_inet_data)
        {            
            pt_moof_info->b_key_build = FALSE;
            i4_ret = piff_inet_get_data(pt_midxbuld_obj); 
            if(i4_ret != MINFOR_OK)
            {
                return MIDXBULDR_FILE_ERR;
            }
        }           
        return mp4_midxbuld_alloc_range(pt_midxbuld_obj, 
                                        (MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)pv_buf);

    case MIDXBULD_SET_INFO_TYPE_FREE_RANGE:
        return mp4_midxbuld_free_range(pt_midxbuld_obj, (UINT32)pv_buf);

    default:
        return MIDXBULDR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_key_tbl_size_estimate
 *
 * Description: This function estimate key table size
 *
 * Inputs:  
 *          pt_midxbuld_obj ,   specifies the index builder object
 *          ui4_key_id ,        specifies the key track id
 *
 * Outputs: NULL 
 *
 * Returns: 
 *          value,   key table size
 ----------------------------------------------------------------------------*/
UINT32 mp4_midxbuld_key_tbl_size_estimate(
                              MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                              UINT32            ui4_key_id)
{
    MP4_IDXBULD_OBJ_T*      pt_mp4_handler = NULL;
    MP4_IDXBULD_TRACK_T     t_key_track = {0};
    input4bytes_fct         pf_input4;
    setpos_fct              pf_set_pos;
    UINT32                  ui4_size = 0;
    UINT8                   ui1_idx;


    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return 0;
    }    
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler == NULL)
    {
        return 0;
    }

    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;

    for(ui1_idx = 0; ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; ui1_idx++)
    {
        if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
        {
            return 0;
        }
        if (ui4_key_id == pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
        {
            t_key_track = pt_mp4_handler->t_track_info_box[ui1_idx];
            break;
        }
    }
    if (ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt) 
    {
        return 0;
    }
    if(!t_key_track.t_stss.b_is_box_exist)
    {
        return 0;
    }
    /*4 byte box size + 4 byte type + 4 byte ver&flag = 12 */
    pf_set_pos(pt_midxbuld_obj, 
               (INT64)(t_key_track.t_stss.ui8_box_offset + 12), 
               FM_SEEK_BGN);
    pf_input4(pt_midxbuld_obj, &ui4_size);
    
    if(pt_mp4_handler->piff_inet_moof_info->b_is_mfra_key_exist)
    {
        UINT32  ui4_moof_key_num = 0;
        pf_set_pos(pt_midxbuld_obj, 
                    pt_mp4_handler->piff_inet_moof_info->ui8_mfra_box_start_pos + 20,
                    FM_SEEK_BGN);/*to mfra box*/
        pf_input4(pt_midxbuld_obj, &ui4_moof_key_num);
        ui4_size += ui4_moof_key_num;
    }
    
    return ui4_size;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_elmt_tbl_size_estimate
 *
 * Description: This function estimate range table size
 *
 * Inputs:  
 *          pt_midxbuld_obj ,   specifies the index builder object
 *          pt_range_obj ,      specifies the range object
 *          pt_curr_tbl_obj ,   specifies current element table object
 *          pt_describ ,        specifies the element table description
 *
 * Outputs: NULL 
 *
 * Returns: 
 *          value,   key table size
 ----------------------------------------------------------------------------*/
UINT32 mp4_midxbuld_elmt_tbl_size_estimate(
                              MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                              MP4_IDXBULD_RANGE_OBJ_T*     pt_range_obj,
                              MP4_IDXBULD_ELMT_TBL_OBJ_T*  pt_curr_tbl_obj,
                              MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ)
{
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler = NULL;
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_t = NULL;
    MP4_IDXBULD_TRACK_T         t_cur_track = {0};
    UINT32 ui4_key_num = 0;
    UINT32 ui4_start_sample = (UINT32)0xFFFFFFFF;
    UINT32 ui4_end_sample = 0;
    UINT32 ui4_i = 0;
    UINT32 ui4_tbl_size = 0;
    UINT8  ui1_idx;   
    
    getpos_fct                  pf_get_pos; 
    setpos_fct                  pf_set_pos; 
    input4bytes_fct             pf_input4;
    copybytes_fct               pf_copybytes;
    
    if (pt_range_obj->u_range.t_es.ui4_start_pts
        >= pt_range_obj->u_range.t_es.ui4_end_pts)
    {
        return 0;
    }
    
    pf_input4 = pt_midxbuld_obj->t_input_fct_tbl.pf_input4;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
    pf_copybytes = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;

    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;
    
   
    switch (pt_describ->u_info.t_es.e_es_type)
    {
    case MIDXBULD_ELEM_STRM_TYPE_AUDIO:
    case MIDXBULD_ELEM_STRM_TYPE_VIDEO:
    case MIDXBULD_ELEM_STRM_TYPE_SUBTITLE:
        {
            /* get curr track info by pt_describ stream id */
            for(ui1_idx = 0; 
                ui1_idx < pt_mp4_handler->ui1_valid_track_cnt; 
                ui1_idx++)
            {
                if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
                {
                    return 0;
                }
                if (pt_describ->u_info.t_es.ui4_strm_id 
                    == pt_mp4_handler->t_track_info_box[ui1_idx].ui4_track_id)
                {
                     t_cur_track = pt_mp4_handler->t_track_info_box[ui1_idx];
                     break;
                }
            }
            if(ui1_idx >= pt_mp4_handler->ui1_valid_track_cnt)
            {
                return 0;
            }
            /* get related key ext table */
            pt_key_ext_t = pt_mp4_handler->pt_key_ext_tbl;
            while (pt_key_ext_t != NULL)
            {
                if (pt_describ->u_info.t_es.ui4_strm_id
                                        == pt_key_ext_t->ui4_track_id)
                {
                    break;
                }
                pt_key_ext_t = pt_key_ext_t->pt_next;
            }
            if((pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) && 
                     pt_describ->u_info.t_es.b_is_pcm_aud)
            {
            }
            else
            {
                if (pt_key_ext_t == NULL)
                {
                    return 0;
                }

                /* cal start key ext index by range start pts */
                ui4_key_num = pt_key_ext_t->ui4_key_num;
                if(pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file && 
                    !pt_mp4_handler->t_mln_moov_info.b_is_moov_empty)
                {
                    ui4_key_num = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_key_cnt;
                }
                else
                {
                   ui4_key_num = pt_key_ext_t->ui4_key_num;
                }
            }
        
            if(pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO 
                || pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_SUBTITLE)
            {
                for(ui4_i = 0; ui4_i < ui4_key_num; ui4_i++)
                {
                    if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                                > pt_range_obj->u_range.t_es.ui4_start_pts)
                    {
                        if(ui4_i > 0)
                        {
                            ui4_i -= 1;/* Get previous key idx, if first one, just get first*/
                        }
                    }
                    else if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                                == pt_range_obj->u_range.t_es.ui4_start_pts)
                    {/*do nothing with the ui4_i*/
                    }
                    else
                    {
                        continue;
                    }
                    ui4_start_sample = pt_key_ext_t->pt_key_ext[ui4_i]
                                                        .ui4_key_ext_sample;
                    pt_range_obj->u_range.t_es.ui4_start_pts
                                = pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts;
                    pt_curr_tbl_obj->ui4_start_pts
                                = pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts;
                    break;
                }
                if(((UINT32)0xFFFFFFFF) == ui4_start_sample)
                {
                    ui4_start_sample
                        = pt_key_ext_t->pt_key_ext[ui4_key_num - 1].ui4_key_ext_sample;
                    pt_curr_tbl_obj->ui4_start_pts
                        = pt_key_ext_t->pt_key_entry[ui4_key_num - 1].ui4_pts;
                    pt_range_obj->u_range.t_es.ui4_start_pts
                                = pt_key_ext_t->pt_key_entry[ui4_key_num - 1].ui4_pts;
                }

                if(pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file          && 
                     !pt_mp4_handler->t_mln_moov_info.b_is_moov_empty           && 
                     (pt_range_obj->u_range.t_es.ui4_start_pts 
                            < pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur) &&
                     (pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur 
                            < 60*MIDXBULD_SYSTEM_CLOCK_FREQUENCY))
                {
                    ui4_end_sample = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_sample_cnt;
                    pt_range_obj->u_range.t_es.ui4_end_pts = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur;
                }
                else
                {
                    /* Get end sample index&pts */
                    for(; ui4_i < ui4_key_num; ui4_i++)
                    {
                        if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                            >= pt_range_obj->u_range.t_es.ui4_end_pts)
                        {
                            ui4_end_sample = pt_key_ext_t->pt_key_ext[ui4_i]
                                                            .ui4_key_ext_sample - 1;
                            pt_range_obj->u_range.t_es.ui4_end_pts
                                        = pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts - 1;
                            break;
                        }
                    }
                }
            }
            else if((pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO) && 
                     pt_describ->u_info.t_es.b_is_pcm_aud)
            {
                UINT32  ui4_sample_duration = 0;
                UINT32  ui4_target_pts_idx  = 0;
                UINT64  ui8_time            = 0;

                /* Start sample index */
                ui8_time = pt_range_obj->u_range.t_es.ui4_start_pts;
                ui8_time *= pt_describ->u_info.t_es.ui4_scale;
                ui4_target_pts_idx = (UINT32)_mm_div64(ui8_time,
                                                       MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                                       NULL);
                
                /* cal start key ext index by start pts */
                for(ui4_i = 0; ui4_i < t_cur_track.t_pcm_sam_inf.ui4_size_dur_num; ui4_i++)
                {
                    if(ui4_sample_duration + t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count
                                 >= ui4_target_pts_idx)
                    {
                        ui4_start_sample = t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx
                            + (ui4_target_pts_idx - ui4_sample_duration)/t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration;
                        break;
                    }
                    else
                    {
                        ui4_sample_duration += t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count;
                    }
                }

                /* End sample index */
                if(pt_range_obj->u_range.t_es.ui4_end_pts == MIDXBULD_IDX_RANGE_FULL)
                {
                    ui8_time = pt_midxbuld_obj->t_src_info.ui4_total_duration;
                }
                else
                {
                    ui8_time = pt_range_obj->u_range.t_es.ui4_end_pts;
                }
                ui8_time *= pt_describ->u_info.t_es.ui4_scale;
                ui4_target_pts_idx = (UINT32)_mm_div64(ui8_time,
                                                       MIDXBULD_SYSTEM_CLOCK_FREQUENCY, 
                                                       NULL);
                
                /* cal start key ext index by start pts */
                for(; ui4_i < t_cur_track.t_pcm_sam_inf.ui4_size_dur_num; ui4_i++)
                {
                    if(ui4_sample_duration + t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count
								   >= ui4_target_pts_idx ||
						((ui4_target_pts_idx - ui4_sample_duration - t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
								   t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count)/
								   t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i + 1].ui4_sample_size_duration == 0)) 
                    {
                        ui4_end_sample = t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_start_idx
                            + (ui4_target_pts_idx - ui4_sample_duration)/t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration;
                        if(0 == (ui4_target_pts_idx - ui4_sample_duration)%t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration)
                        {
                            ui4_end_sample -= 1;
                        }
                        break;
                    }
                    else
                    {
                        ui4_sample_duration += t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_size_duration * 
                                           t_cur_track.t_pcm_sam_inf.pat_aud_pcm_size_dur[ui4_i].ui4_sample_count;
                    }
                }
                if((ui4_end_sample == 0) && (ui4_end_sample >= t_cur_track.t_pcm_sam_inf.ui4_total_sample_num))
                {
                    ui4_end_sample = t_cur_track.t_pcm_sam_inf.ui4_total_sample_num;
                }
                pt_curr_tbl_obj->ui4_start_pts  = pt_range_obj->u_range.t_es.ui4_start_pts;
                pt_curr_tbl_obj->ui4_end_pts    = pt_range_obj->u_range.t_es.ui4_end_pts;
            }
            else if(pt_describ->u_info.t_es.e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
            {
                INT32   i4_ret              = 0;
                UINT32  ui4_start_key_idx   = (UINT32)~0;
                UINT32  ui4_count_init      = 0; 
                UINT32  ui4_curr_pts        = 0;
                UINT32  ui4_last_pts        = 0;
                UINT32  ui4_sample_count    = 0;
                UINT32  ui4_sample_duration = 0;
                UINT64  ui8_time            = 0;
                UINT64  ui8_pos             = 0;
                UINT64  ui8_curr_duration_sum = 0;
                UINT32  ui4_read_size       = 0;
                UINT32  ui4_cur_sample_idx  = 0;
                MP4_RAW_STTS_TBL_T  t_stts_item;
                
                /* cal start key ext index by start pts */
                for(ui4_i = 0; ui4_i < ui4_key_num; ui4_i++)
                {
                    if (pt_key_ext_t->pt_key_entry[ui4_i].ui4_pts 
                                >= pt_range_obj->u_range.t_es.ui4_start_pts)
                    {
                        if(ui4_i == 0)
                        {
                            ui4_start_key_idx = 0;
                        }
                        else
                        {
                            ui4_start_key_idx = ui4_i - 1;
                        }
                        break;
                    }
                }

                if(ui4_start_key_idx   == (UINT32)~0)/*Vlen > ALen*/
                {
                    ui4_start_key_idx = ui4_key_num - 1;
                }

                /* cal pts */
                ui4_count_init
                    = pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_key_ext_sample
                        - pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                            .ui4_stts_entry_sample;
                ui4_curr_pts = pt_key_ext_t->pt_key_entry[ui4_start_key_idx].ui4_pts;
                ui8_curr_duration_sum = pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                                        .ui8_key_ext_duration_sum;
                /* seek to 'time to sample'  box to cal pts */
                pf_set_pos(pt_midxbuld_obj, 
                           (INT64)(t_cur_track.t_stts.ui8_box_offset + 16
                                + 8*pt_key_ext_t->pt_key_ext[ui4_start_key_idx]
                                    .ui4_stts_entry), 
                           FM_SEEK_BGN);
                       
                
                /* check the dividend can't equal zero */
                if(pt_describ->u_info.t_es.ui4_scale == 0)
                {
                    return 0;
                }               
                ui4_cur_sample_idx = pt_key_ext_t->pt_key_ext[ui4_start_key_idx].ui4_key_ext_sample;
                while (ui4_curr_pts <= pt_range_obj->u_range.t_es.ui4_end_pts)
                {
                    pf_get_pos(pt_midxbuld_obj, &ui8_pos);
                    if(ui8_pos >= t_cur_track.t_stts.ui8_box_offset
                                        + t_cur_track.t_stts.ui4_box_size)
                    {/*Reach to STTS end  */
                        break;
                    }
                    i4_ret = pf_copybytes(pt_midxbuld_obj, 
                                          &t_stts_item, 
                                          sizeof(MP4_RAW_STTS_TBL_T), 
                                          sizeof(MP4_RAW_STTS_TBL_T), 
                                          &ui4_read_size);
                    if(i4_ret != INPUTR_OK)
                    {
                        return 0;
                    }
                    ui4_sample_count = LITTLE_TO_BIG_DWORD(t_stts_item.ui4_sample_count);
                    ui4_sample_duration
                                = LITTLE_TO_BIG_DWORD(t_stts_item.ui4_sample_duration);


                    for(ui4_i = ui4_count_init; ui4_i < ui4_sample_count; ui4_i++)
                    {
                        ui8_time = ui8_curr_duration_sum * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;

                        ui4_curr_pts = (UINT32)_mm_div64(ui8_time,
                                                         pt_describ->u_info.t_es.ui4_scale, 
                                                         NULL);                            

                        if (ui4_curr_pts > pt_range_obj->u_range.t_es.ui4_end_pts)
                        {
                            ui4_end_sample = ui4_cur_sample_idx - 1;
                            pt_curr_tbl_obj->ui4_end_pts = ui4_curr_pts;
                            break;
                        }
                        if((ui4_curr_pts >= pt_range_obj->u_range.t_es.ui4_start_pts)
                            && (ui4_start_sample == 0xFFFFFFFF))
                        {
                            ui4_start_sample = ui4_cur_sample_idx;
                            pt_curr_tbl_obj->ui4_start_pts = ui4_last_pts;
                        }                       
                        ui4_cur_sample_idx++;           
                        ui4_last_pts = ui4_curr_pts;
                        ui8_curr_duration_sum += (UINT64)ui4_sample_duration;
                    }
                    if (ui4_i < ui4_sample_count)
                    {
                        break;
                    }
                    ui4_count_init = 0;
                }
            }

            
            if(ui4_end_sample == 0)
            {   
                pf_set_pos(pt_midxbuld_obj, 
                            (INT64)(t_cur_track.t_stsz.ui8_box_offset + 16), 
                            FM_SEEK_BGN);
                pf_input4(pt_midxbuld_obj, &ui4_end_sample);
                pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
                
                if(pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file          && 
                     !pt_mp4_handler->t_mln_moov_info.b_is_moov_empty           && 
                     (pt_range_obj->u_range.t_es.ui4_start_pts 
                            < pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur) &&
                     (pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur 
                            < 60*MIDXBULD_SYSTEM_CLOCK_FREQUENCY))
                {
                    pt_range_obj->u_range.t_es.ui4_end_pts = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur;
                }
                else
                {
                    pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
                }
            }
            pt_curr_tbl_obj->ui4_end_pts = pt_range_obj->u_range.t_es.ui4_end_pts;

            /* cal table size */
            pt_curr_tbl_obj->ui4_start_sample = ui4_start_sample;
            pt_curr_tbl_obj->ui4_end_sample = ui4_end_sample;
            
            ui4_tbl_size = ui4_end_sample - ui4_start_sample + 1;
            #ifndef __KERNEL__
            if(ui4_start_sample > ui4_end_sample)
            {
                x_dbg_stmt("!!!! ui4_start_sample > ui4_end_sample !!!!\r\n");
                ui4_tbl_size = 0;
            }
            if(ui4_tbl_size > 2000)
            {
                x_dbg_stmt("!!!! NOTICE: ui4_tbl_size=%d >2000, maybe error !!!!\r\n", ui4_tbl_size);
            }
            #endif
        }
        break;

    case MIDXBULD_ELEM_STRM_TYPE_KEY:
        {
            ui4_tbl_size = mp4_midxbuld_key_tbl_size_estimate(pt_midxbuld_obj, 
                                            pt_describ->u_info.t_es.ui4_strm_id);
        }
        break;

    default:
        {
            ui4_tbl_size = 0;
        }
        break;
    }

    return ui4_tbl_size;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_alloc_range
 *
 * Description: This function allocate range resource.
 *
 * Inputs:  
 *          pt_midxbuld_obj ,   specifies the index builder object
 *          pt_elmt_tbl_alloc , specifies the element table
 *
 * Outputs: NULL 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and 
 *                                   the operation has been completed.
 *          Non-zero value  Fail
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_alloc_range(
                            MIDXBULD_OBJ_T*                 pt_midxbuld_obj, 
                            MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc)
{
    UINT8                           ui1_i           = 0;
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler   = NULL;
    MP4_IDXBULD_RANGE_OBJ_T*        pt_range_obj     = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj       = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj_last  = NULL;
    MIDXBULD_ELMT_TBL_DESCRIB_T*    pt_describ       = NULL;
    PIFF_BOX_INET_INFO_T*           pt_moof_info     = NULL;
    MP4_MULTI_FILE_INFO_T*          pt_file_info     = NULL;
#if (ENABLE_CMOV == 1)
    MP4_BOX_CMOV_INFO_T*            pt_cmov_info     = NULL;
    pt_cmov_info = (MP4_BOX_CMOV_INFO_T*)(pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_cmov_info);
#endif

    pt_file_info = pt_midxbuld_obj->t_src_info.t_mp4_extra_info.pt_mp4_multi_file_info;

    DBG_INFO(("API: mp4_midxbuld_alloc_range begin \r\n" ));

    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    pt_describ = NULL;
    pt_moof_info = pt_mp4_handler->piff_inet_moof_info;
    pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
    pt_mp4_handler->pt_elmt_tbl_alloc = pt_elmt_tbl_alloc;
    if (pt_mp4_handler->ui1_range_count >= MP4_MIDXBULD_RANGE_MAX)
    {
        DBG_INFO(("ERROR: out of range count \r\n" ));
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts >= 
        pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts)
    {
        DBG_INFO(("ERROR: invalid range pts \r\n" ));
        return MIDXBULDR_RANGE_ERROR;
    }

    if((pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts >= 
        pt_midxbuld_obj->t_src_info.ui4_total_duration) &&
        (pt_midxbuld_obj->t_src_info.ui4_total_duration != 0))
    {
        DBG_INFO(("ERROR: invalid range pts \r\n" ));
        return MIDXBULDR_RANGE_ERROR;
    }

    for(ui1_i = 0; ui1_i<pt_file_info->ui1_file_num; ui1_i++)
    {
        if((pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts
                >= pt_file_info->at_file_info[ui1_i].ui8_file_start_pts)
            && (pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts
                <= pt_file_info->at_file_info[ui1_i].ui8_file_start_pts
                    + pt_file_info->at_file_info[ui1_i].ui8_file_duration))
        {
            pt_mp4_handler->ui1_curr_file_idx = ui1_i;
            break;
        }
        else if((pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts
                >= pt_file_info->at_file_info[ui1_i].ui8_file_start_pts)
            && (pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts
                < pt_file_info->at_file_info[ui1_i].ui8_file_start_pts
                    + pt_file_info->at_file_info[ui1_i].ui8_file_duration)
            && (pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts
                > pt_file_info->at_file_info[ui1_i].ui8_file_start_pts
                    + pt_file_info->at_file_info[ui1_i].ui8_file_duration))
        {
            pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts
                = pt_file_info->at_file_info[ui1_i].ui8_file_start_pts
                    + pt_file_info->at_file_info[ui1_i].ui8_file_duration;
            pt_mp4_handler->ui1_curr_file_idx = ui1_i;
            break;
        }
        else if((pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts
                < pt_file_info->at_file_info[ui1_i].ui8_file_start_pts)
            && (pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts + 90000
                >= pt_file_info->at_file_info[ui1_i].ui8_file_start_pts))
        {
#ifndef __KERNEL__
    x_dbg_stmt("===== ########  NOTICE !!! ########## =====\r\n");
#endif
            pt_mp4_handler->ui1_curr_file_idx = ui1_i;
            break;
        }
    }
    if(pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts == MIDXBULD_IDX_RANGE_FULL)
    {
        /*Last range, do nothing*/
    }
    else if(((pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts 
                > pt_mp4_handler->t_cur_file_info.ui8_file_start_pts 
                + pt_mp4_handler->t_cur_file_info.ui8_file_duration)
            || (pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts 
                < pt_mp4_handler->t_cur_file_info.ui8_file_start_pts))
         && (pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts 
                <= pt_midxbuld_obj->t_src_info.ui4_total_duration))
    {
        INT32 i4_ret            = 0;
        UINT32 ui4_tmp_range_id = 0;
#ifndef __KERNEL__
    x_dbg_stmt("===== ########  file switch, current file is %d ########## =====\r\n", pt_mp4_handler->ui1_curr_file_idx);
#endif
        
        mp4_single_file_info_free(pt_midxbuld_obj);
        
        i4_ret = mp4_single_file_info_alloc(pt_mp4_handler->pt_key_info, pt_midxbuld_obj); 
        if((i4_ret != MIDXBULDR_OK) || (pt_mp4_handler->ui1_valid_track_cnt == 0))
        {
            return MIDXBULDR_INTERNAL_ERR;
        }

        pt_mp4_handler->t_cur_file_info = pt_file_info->at_file_info[pt_mp4_handler->ui1_curr_file_idx];
        
        i4_ret = mp4_midxbuld_indexing(pt_midxbuld_obj, &ui4_tmp_range_id);

    }
    else if(pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts + MP4_SINGLE_RANGE_DURATION 
                >= pt_mp4_handler->t_cur_file_info.ui8_file_start_pts
                    + pt_mp4_handler->t_cur_file_info.ui8_file_duration)
    {
        pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts
            = pt_mp4_handler->t_cur_file_info.ui8_file_start_pts
                    + pt_mp4_handler->t_cur_file_info.ui8_file_duration;
    }
    //pt_mp4_handler->t_cur_file_info = pt_file_info->at_file_info[pt_mp4_handler->ui1_curr_file_idx];

    pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts -= pt_mp4_handler->t_cur_file_info.ui8_file_start_pts;
    pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts   -= pt_mp4_handler->t_cur_file_info.ui8_file_start_pts;

#ifndef __KERNEL__
    x_dbg_stmt("=====start pts: %d, end: %d, file_start:%llu , dur: %llu=====\r\n", 
                pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts, 
                pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts,
                pt_mp4_handler->t_cur_file_info.ui8_file_start_pts,
                pt_mp4_handler->t_cur_file_info.ui8_file_duration);
#endif

    if (pt_elmt_tbl_alloc->e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
    {
        /* allocate elmt tbl range obj memory */
        pt_range_obj = (MP4_IDXBULD_RANGE_OBJ_T*) 
                    _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_RANGE_OBJ_T), 
                                                MM_MEM_MIDX_TAG);
        if (pt_range_obj == NULL)
        {
            DBG_INFO(("ERROR: out of range obj memory \r\n" ));
            return MIDXBULDR_OUT_OF_MEMORY;
        }

        if (TRUE != _mp4_midxbuld_alloc_range_id(pt_midxbuld_obj, 
                                                 &pt_elmt_tbl_alloc->ui4_range_id))
        {
            DBG_INFO(("ERROR: out of alloc range id \r\n" ));
            _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
            return MIDXBULDR_OUT_OF_HANDLE;
        }
        
        pt_mp4_handler->apt_range_obj[pt_mp4_handler->ui1_range_count++] = pt_range_obj;

        x_memset(pt_range_obj, 0, sizeof(MP4_IDXBULD_RANGE_OBJ_T));
        pt_range_obj->e_tbl_type = pt_elmt_tbl_alloc->e_tbl_type;
        pt_range_obj->u_range.t_es = pt_elmt_tbl_alloc->u_range.t_es;
        pt_range_obj->ui4_range_id = pt_elmt_tbl_alloc->ui4_range_id;
        pt_range_obj->b_is_key_frame = FALSE;
        pt_range_obj->b_index_is_valid = FALSE;

        pt_describ = pt_elmt_tbl_alloc->pt_tbl_describ;

        /* allocate elmt tbl obj memory */
        pt_tbl_obj = (MP4_IDXBULD_ELMT_TBL_OBJ_T*) 
                _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T), 
                                            MM_MEM_MIDX_TAG);
        if (pt_tbl_obj == NULL)
        {
            DBG_INFO(("ERROR: out of elment table memory \r\n" ));
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                            pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        if (TRUE != _mp4_midxbuld_alloc_table_id(pt_range_obj, 
                                                    &pt_describ->ui4_tbl_id))
        {
            DBG_INFO(("ERROR: out of alloc table id \r\n" ));
            _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                            pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_HANDLE;
        }

        pt_range_obj->pt_tbl_obj = pt_tbl_obj;
        pt_tbl_obj_last = pt_tbl_obj;

        x_memset(pt_tbl_obj, 0, sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T));
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
        pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;

        if(pt_moof_info->b_is_piff_file == TRUE)
        {            
            if(pt_moof_info->b_is_inet_data)
            {
                if(pt_tbl_obj->trun_qseek_tbl != NULL) /*frequent free & alloc, need to modify*/
                {
                     _mm_util_mem_free_with_tag(pt_tbl_obj->trun_qseek_tbl, MM_MEM_MIDX_TAG);
                      pt_tbl_obj->trun_qseek_tbl = NULL;
                }
                if(pt_tbl_obj->tfhd_box_info!= NULL)
                {
                     _mm_util_mem_free_with_tag(pt_tbl_obj->tfhd_box_info, MM_MEM_MIDX_TAG);
                      pt_tbl_obj->tfhd_box_info = NULL;
                }
                pt_tbl_obj->trun_qseek_tbl = (TRUN_QUICK_SEEK_TBL*)
                 _mm_util_mem_alloc_with_tag(sizeof(TRUN_QUICK_SEEK_TBL) * 30, 
                                            MM_MEM_MIDX_TAG); 
                if(pt_tbl_obj->trun_qseek_tbl == NULL)
                {
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset(pt_tbl_obj->trun_qseek_tbl, 0, 
                    sizeof(TRUN_QUICK_SEEK_TBL) * (pt_moof_info->ui4_moof_cnt)); /*30 truns;*/

                pt_tbl_obj->tfhd_box_info = (PIFF_TFHD_TBL_T*)
                            _mm_util_mem_alloc_with_tag(sizeof(PIFF_TFHD_TBL_T) * (pt_moof_info->ui4_moof_cnt), 
                                                        MM_MEM_MIDX_TAG); 
                if(pt_tbl_obj->tfhd_box_info == NULL)
                {
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset(pt_tbl_obj->tfhd_box_info, 0, 
                    sizeof(PIFF_TFHD_TBL_T) * (pt_moof_info->ui4_moof_cnt)); 
                
                pt_tbl_obj->ui4_tbl_size =
                    piff_midxbuld_inet_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                            pt_range_obj, 
                                                            pt_tbl_obj, 
                                                            pt_describ);
                
            }
            else
            {
                if(!pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file         ||
                    (pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file && 
                     !pt_mp4_handler->t_mln_moov_info.b_is_moov_empty    && 
                     (pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts >
                        pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur)))
                {
                    pt_tbl_obj->ui4_tbl_size = 
                        piff_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                            pt_range_obj, 
                                                            pt_tbl_obj, 
                                                            pt_describ);
                }
                else
#if (ENABLE_CMOV == 1)
                if(pt_cmov_info->b_is_cmov_box_exist)
                {
                    pt_tbl_obj->ui4_tbl_size = 
                        mp4_cmov_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                                 pt_range_obj, 
                                                                 pt_tbl_obj, 
                                                                 pt_describ);
                }
                else
#endif
                {
                    pt_tbl_obj->ui4_tbl_size = 
                        mp4_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                            pt_range_obj, 
                                                            pt_tbl_obj, 
                                                            pt_describ);
                }
            }
        }
        else
#if (ENABLE_CMOV == 1)
        if(pt_cmov_info->b_is_cmov_box_exist)
        {
            pt_tbl_obj->ui4_tbl_size = 
                mp4_cmov_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                         pt_range_obj, 
                                                         pt_tbl_obj, 
                                                         pt_describ);
        }
        else
#endif
        {
            pt_tbl_obj->ui4_tbl_size = 
                mp4_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                    pt_range_obj, 
                                                    pt_tbl_obj, 
                                                    pt_describ);
        }
        /* reset range start/end pts*/
        pt_elmt_tbl_alloc->u_range.t_es = pt_range_obj->u_range.t_es;
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_describ->u_info.t_es;

        if(pt_tbl_obj->ui4_tbl_size == 0)
        {
#if 1
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
           /* _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
            pt_range_obj->pt_tbl_obj = NULL;
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                            pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_RANGE_ERROR;*/
#else/*just for fix bug*/
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = 
                (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                    sizeof(MIDXBULD_ELMT_STRM_ENTRY_T), 
                    MM_MEM_MIDX_TAG);/*no need check*/
#endif
        }
        else
        {
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = 
                (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                    sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, 
                    MM_MEM_MIDX_TAG);
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry == NULL)
            {
                DBG_INFO(("ERROR: out of entry memory \r\n" ));
                _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                                pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                pt_describ->ui4_tbl_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 
                     0, 
                     sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
        }
         pt_tbl_obj->ui4_tbl_id = pt_describ->ui4_tbl_id;
        pt_tbl_obj->pt_next = NULL;

        while (pt_describ->pt_next != NULL)
        {
            pt_describ = pt_describ->pt_next;

            /* allocate elmt tbl obj memory */
            pt_tbl_obj = (MP4_IDXBULD_ELMT_TBL_OBJ_T*) 
                    _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T), 
                                                MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                                pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            if (TRUE != _mp4_midxbuld_alloc_table_id(pt_range_obj, 
                                                    &pt_describ->ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                                pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }
            
            pt_tbl_obj_last->pt_next = pt_tbl_obj;
            pt_tbl_obj_last = pt_tbl_obj;

            x_memset(pt_tbl_obj, 0, sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T));
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
            if(pt_describ->u_info.t_es.b_is_cur_strm_codec_spt == FALSE)
            {
                pt_tbl_obj->ui4_tbl_size = 0;
            }
            else if(pt_moof_info->b_is_piff_file == TRUE)
            {            
                if(pt_moof_info->b_is_inet_data)
                {                
                    if(pt_tbl_obj->trun_qseek_tbl != NULL)
                    {
                         _mm_util_mem_free_with_tag(pt_tbl_obj->trun_qseek_tbl, MM_MEM_MIDX_TAG);
                         pt_tbl_obj->trun_qseek_tbl = NULL;
                    }
                    if(pt_tbl_obj->tfhd_box_info!= NULL)
                    {
                         _mm_util_mem_free_with_tag(pt_tbl_obj->tfhd_box_info, MM_MEM_MIDX_TAG);
                         pt_tbl_obj->tfhd_box_info = NULL;
                    }
                    pt_tbl_obj->trun_qseek_tbl = (TRUN_QUICK_SEEK_TBL*)
                     _mm_util_mem_alloc_with_tag(sizeof(TRUN_QUICK_SEEK_TBL) * (pt_moof_info->ui4_moof_cnt), 
                                                MM_MEM_MIDX_TAG); 
                    if(pt_tbl_obj->trun_qseek_tbl == NULL)
                    {
                        return MIDXBULDR_OUT_OF_MEMORY;
                    }
                    x_memset(pt_tbl_obj->trun_qseek_tbl, 0, 
                        sizeof(TRUN_QUICK_SEEK_TBL) * (pt_moof_info->ui4_moof_cnt)); 

                    pt_tbl_obj->tfhd_box_info = (PIFF_TFHD_TBL_T*)
                                _mm_util_mem_alloc_with_tag(sizeof(PIFF_TFHD_TBL_T) * (pt_moof_info->ui4_moof_cnt), 
                                                            MM_MEM_MIDX_TAG); 
                    if(pt_tbl_obj->tfhd_box_info == NULL)
                    {
                        return MIDXBULDR_OUT_OF_MEMORY;
                    }
                    x_memset(pt_tbl_obj->tfhd_box_info, 0, 
                        sizeof(PIFF_TFHD_TBL_T) * (pt_moof_info->ui4_moof_cnt)); 
                    
                    pt_tbl_obj->ui4_tbl_size =
                        piff_midxbuld_inet_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                        pt_range_obj, 
                                                        pt_tbl_obj, 
                                                        pt_describ);                    
                }
                else
                {
                    if(!pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file         ||
                        (pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file && 
                         !pt_mp4_handler->t_mln_moov_info.b_is_moov_empty    && 
                         (pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts >
                            pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_dur)))
                    {
                        pt_tbl_obj->ui4_tbl_size = 
                            piff_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                                pt_range_obj, 
                                                                pt_tbl_obj, 
                                                                pt_describ);
                    }
                    else
#if (ENABLE_CMOV == 1)
                    if(pt_cmov_info->b_is_cmov_box_exist)
                    {
                        pt_tbl_obj->ui4_tbl_size = 
                            mp4_cmov_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                                     pt_range_obj, 
                                                                     pt_tbl_obj, 
                                                                     pt_describ);
                    }
                    else
#endif
                    {
                        pt_tbl_obj->ui4_tbl_size = 
                            mp4_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                                pt_range_obj, 
                                                                pt_tbl_obj, 
                                                                pt_describ);
                    }
                }
            } 
            else           
#if (ENABLE_CMOV == 1)
            if(pt_cmov_info->b_is_cmov_box_exist)
            {
                pt_tbl_obj->ui4_tbl_size = 
                    mp4_cmov_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                            pt_range_obj, 
                                                            pt_tbl_obj, 
                                                            pt_describ);
            }
            else
#endif
            {
                pt_tbl_obj->ui4_tbl_size = 
                    mp4_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, 
                                                    pt_range_obj, 
                                                    pt_tbl_obj, 
                                                    pt_describ);
            }
            
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_describ->u_info.t_es;
            if(pt_tbl_obj->ui4_tbl_size != 0)
            {
                /*pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_start_frame_num = 0;
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_end_frame_num = 0;*/
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = 
                    (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                        sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, 
                        MM_MEM_MIDX_TAG);
                if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry == NULL)
                {
                    _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                                 pt_elmt_tbl_alloc->ui4_range_id);
                    pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                    pt_describ->ui4_tbl_id = MIDXBULD_NULL_ID;
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 
                         0, 
                         sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
            }
            else
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
            }
 
            pt_tbl_obj->ui4_tbl_id = pt_describ->ui4_tbl_id;
            pt_tbl_obj->pt_next = NULL;
        }

        pt_mp4_handler->pt_curr_range_obj = pt_range_obj;

        
        pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts += pt_mp4_handler->t_cur_file_info.ui8_file_start_pts;
        if(pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts != MIDXBULD_IDX_RANGE_FULL)
        {
            pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts   += pt_mp4_handler->t_cur_file_info.ui8_file_start_pts;
        }
    }
    else
    {
        DBG_INFO(("ERROR: invalid arg - table type \r\n" ));
        return MIDXBULDR_INV_ARG;
    }

    DBG_INFO(("API: mp4_midxbuld_alloc_range OK \r\n" ));
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_free_range
 *
 * Description: This function free range resource.
 *
 * Inputs:  
 *          pt_midxbuld_obj ,  specifies the index builder object
 *          ui4_range_id ,     specifies range id
 *
 * Outputs: NULL 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and 
 *                                   the operation has been completed.
 *          Non-zero value  Fail
 ----------------------------------------------------------------------------*/
INT32 mp4_midxbuld_free_range(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id)
{
    MP4_IDXBULD_OBJ_T*  pt_mp4_handler = NULL;
    
    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    /* can NOT free key range */
    if ((pt_mp4_handler->ui1_range_count == 0) ||
       ((pt_mp4_handler->pt_key_range_obj != NULL) && (pt_mp4_handler->pt_key_range_obj->ui4_range_id == ui4_range_id)))
    {
        return MIDXBULDR_INV_HANDLE;
    }

    if (TRUE == _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, ui4_range_id))
    {
        return MIDXBULDR_OK;
    }
    
    return MIDXBULDR_INV_HANDLE;
}

