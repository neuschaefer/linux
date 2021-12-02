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
 * $RCSfile: mkv_midxbuld.c,v $
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
#include "mutil/midxbuld/mkv_idxbuld/mkv_midxbuld.h"
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
#include "mkv_midxbuld.h"
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
/*define mkv parsing buffer length*/
#define MKV_IDXBULD_INIT_PARSER_BUFF_LEN       (10*1024)
#define MKV_IDXBULD_TEMP_PARSER_BUFF_LEN       (100)
    
#define MKV_IDXBUILD_PARSE_KEY_BLOCK           0

#define MKV_SIZE_4G                            0x100000000
    
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

static BOOL _mkv_midxbuld_alloc_range_id ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32*         pui4_range_id,
                                           MKV_IDXBULD_OBJ_T* pt_mkv_handler);
static BOOL _mkv_midxbuld_free_range_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           MKV_IDXBULD_OBJ_T* pt_mkv_handler,
                                           UINT32          ui4_range_id );

static BOOL _mkv_midxbuld_alloc_table_id ( MKV_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                                           UINT32*                  pui4_tbl_id );
static BOOL _mkv_midxbuld_free_table_obj ( MIDXBULD_OBJ_T*  pt_midxbuld_obj,
                                           MKV_IDXBULD_OBJ_T*  pt_mkv_handler,
                                           UINT32           ui4_tbl_id );


/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_copy_bytes
 *
 * Description: 
 *
 * Inputs:  
 *          
 * Outputs: 
 *
 * Returns: 
 *  
 * Note:  
 ----------------------------------------------------------------------------*/
static INT32 _mkv_midxbuld_copy_bytes(VOID* pv_obj, 
                                             VOID* pv_buf, 
                                             SIZE_T z_buf_leng, 
                                             SIZE_T z_copy_size, 
                                             UINT32* pui4_size)
{
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*      pt_mkv_handler;
    setpos_fct              pf_set_pos;    
    copybytes_fct           pf_copybyte;
    UINT8*                  pui1_dest;
    INT32                   i4_ret;
    SIZE_T                  z_copied_sz;

    /*Check argument*/
    if(!pv_obj || !pv_buf || z_copy_size == 0)
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_midxbuld_obj = pv_obj;
    pt_mkv_handler  = pt_midxbuld_obj->pv_handler_obj;
    pf_set_pos      = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte     = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pui1_dest       = pv_buf;
    z_copied_sz     = 0;

    if(!pt_mkv_handler || !pf_set_pos || !pf_copybyte)
    {
        return MIDXBULDR_INV_ARG;
    }
    
    if(z_buf_leng == 0)
    {
        z_buf_leng = pt_mkv_handler->ui4_buf_length;
    }

    /*do*/
    do
    {
        if(pt_mkv_handler->i8_buf_read_pos < 0 || 
           pt_mkv_handler->i8_buf_read_pos >= (INT64)pt_mkv_handler->ui4_buf_write_pos)
        {
            if(pt_mkv_handler->i8_buf_read_pos + (INT64)pt_mkv_handler->ui8_buf_file_pos > (INT64)pt_mkv_handler->ui8_file_size)
            {
                break;
            }
            i4_ret = pf_set_pos(pv_obj, pt_mkv_handler->i8_buf_read_pos + pt_mkv_handler->ui8_buf_file_pos, FM_SEEK_BGN);
            if(i4_ret != 0)
            {
                return i4_ret;
            }
            pt_mkv_handler->ui8_buf_file_pos = (UINT64)(pt_mkv_handler->i8_buf_read_pos + (INT64)pt_mkv_handler->ui8_buf_file_pos);
            pt_mkv_handler->i8_buf_read_pos = 0;
            
            if(z_buf_leng + pt_mkv_handler->ui8_buf_file_pos > pt_mkv_handler->ui8_file_size)
            {
                i4_ret = pf_copybyte(pv_obj, pt_mkv_handler->pui1_header_buf, 
                    (SIZE_T)(pt_mkv_handler->ui8_file_size - pt_mkv_handler->ui8_buf_file_pos), 
                    (SIZE_T)(pt_mkv_handler->ui8_file_size - pt_mkv_handler->ui8_buf_file_pos), 
                    &pt_mkv_handler->ui4_buf_write_pos);
            }
            else
            {
                i4_ret = pf_copybyte(pv_obj, pt_mkv_handler->pui1_header_buf, z_buf_leng, z_buf_leng, 
                    &pt_mkv_handler->ui4_buf_write_pos);
            }
            if(i4_ret != 0)
            {
                return i4_ret;
            }
            if(pt_mkv_handler->ui4_buf_write_pos == 0)
            {
                break;
            }
        }
        
        if(z_copy_size > (SIZE_T)((INT64)pt_mkv_handler->ui4_buf_write_pos - pt_mkv_handler->i8_buf_read_pos))
        {
            _mm_util_mem_copy(pui1_dest + z_copied_sz, 
                pt_mkv_handler->pui1_header_buf + pt_mkv_handler->i8_buf_read_pos,
                (SIZE_T)((INT64)pt_mkv_handler->ui4_buf_write_pos - pt_mkv_handler->i8_buf_read_pos));
            z_copied_sz += pt_mkv_handler->ui4_buf_write_pos - pt_mkv_handler->i8_buf_read_pos;
            z_copy_size -= pt_mkv_handler->ui4_buf_write_pos - pt_mkv_handler->i8_buf_read_pos;

            pt_mkv_handler->i8_buf_read_pos = (INT64)pt_mkv_handler->ui4_buf_write_pos;
        }
        else
        {
            _mm_util_mem_copy(pui1_dest + z_copied_sz, 
                pt_mkv_handler->pui1_header_buf + pt_mkv_handler->i8_buf_read_pos,
                z_copy_size);
            z_copied_sz += z_copy_size;
            z_copy_size -= z_copy_size;
            
            pt_mkv_handler->i8_buf_read_pos += z_copy_size;
        }
    }while(z_copy_size > 0);
    

    /*Set out parameter*/
    if(pui4_size)
    {
        *pui4_size = z_copied_sz;
    }
    
    /*Though there are many pt_mkv_handler but the buffer only one*/
    while(pt_mkv_handler->pt_next)
    {
        pt_mkv_handler->pt_next->i8_buf_read_pos = pt_mkv_handler->i8_buf_read_pos;
        pt_mkv_handler->pt_next->ui8_buf_file_pos = pt_mkv_handler->ui8_buf_file_pos;
        pt_mkv_handler = pt_mkv_handler->pt_next;
    }
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_setpos
 *
 * Description: 
 *
 * Inputs:  
 *          
 * Outputs: 
 *
 * Returns: 
 *  
 * Note:  
 ----------------------------------------------------------------------------*/
static INT32 _mkv_midxbuld_setpos(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*      pt_mkv_handler;
    
    if(!pv_obj || i8_offset < 0)
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_midxbuld_obj = pv_obj;
    pt_mkv_handler  = pt_midxbuld_obj->pv_handler_obj;
    if(!pt_mkv_handler)
    {
        return MIDXBULDR_INV_ARG;
    }
    
    switch(ui1_whence)
    {
    case FM_SEEK_BGN:
        pt_mkv_handler->i8_buf_read_pos = i8_offset - (INT64)pt_mkv_handler->ui8_buf_file_pos;
        break;
    case FM_SEEK_CUR:
        pt_mkv_handler->i8_buf_read_pos += i8_offset;
        break;
    case FM_SEEK_END:
        pt_mkv_handler->i8_buf_read_pos = (INT64)pt_mkv_handler->ui8_file_size - i8_offset 
                                        - (INT64)pt_mkv_handler->ui8_buf_file_pos;
        break;
    default:
        return MIDXBULDR_INV_ARG;
    }
    
    /*Though there are many pt_mkv_handler but the buffer only one*/
    while(pt_mkv_handler->pt_next)
    {
        pt_mkv_handler->pt_next->i8_buf_read_pos = pt_mkv_handler->i8_buf_read_pos;
        pt_mkv_handler = pt_mkv_handler->pt_next;
    }
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_getpos
 *
 * Description: 
 *
 * Inputs:  
 *          
 * Outputs: 
 *
 * Returns: 
 *  
 * Note:  
 ----------------------------------------------------------------------------*/
static INT32 _mkv_midxbuld_getpos(VOID* pv_obj, UINT64* pui8_cur_pos)
{
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*      pt_mkv_handler;
    INT64                   i8_cur_pos;
    
    if(!pv_obj || !pui8_cur_pos)
    {
        return MIDXBULDR_INV_ARG;
    }
    
    pt_midxbuld_obj = pv_obj;
    pt_mkv_handler  = pt_midxbuld_obj->pv_handler_obj;
    if(!pt_mkv_handler)
    {
        return MIDXBULDR_INV_ARG;
    }

    i8_cur_pos = pt_mkv_handler->i8_buf_read_pos + (INT64)pt_mkv_handler->ui8_buf_file_pos;
    if(i8_cur_pos < 0)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    *pui8_cur_pos = (UINT64)i8_cur_pos;
    
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_alloc_range_id
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
static BOOL _mkv_midxbuld_alloc_range_id (MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                                UINT32*         pui4_range_id,
                                                MKV_IDXBULD_OBJ_T* pt_mkv_handler)
{
    UINT8              ui1_key_range_id;
    UINT8              ui1_range_id;

    /*Parameters check*/
    if ((NULL == pt_midxbuld_obj) || (NULL == pui4_range_id) || (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_alloc_range_id:NULL\r\n"));
        return FALSE;
    }

    *pui4_range_id = MIDXBULD_NULL_ID;

    if (pt_mkv_handler->pt_key_range_obj != NULL)
    {
        ui1_key_range_id = (UINT8)((pt_mkv_handler->pt_key_range_obj->ui4_range_id 
                                        & MKV_MIDXBULD_RANGE_ID_MASK)
                                    >> 8);
    }
    /* key range should be allocated first */
    else if(pt_mkv_handler->ui1_range_count > 0)
    {
        DBG_ERROR(("_mkv_midxbuld_alloc_range_id:count=%u\r\n",
                  pt_mkv_handler->ui1_range_count));
        return FALSE;
    }
    else
    {
        ui1_key_range_id = 1;
    }

    if (pt_mkv_handler->ui1_range_count < 0xff)
    {
        UINT8 ui1_i;
        BOOL  b_occupied = TRUE;

        while (b_occupied)
        {
            b_occupied = FALSE;
            if (pt_mkv_handler->ui1_range_count > MKV_MIDXBULD_RANGE_MAX)
            {
                DBG_ERROR(("_mkv_midxbuld_alloc_range_id:error count!!!\r\n"));
                return FALSE;
            }
            for (ui1_i = 0; ui1_i < pt_mkv_handler->ui1_range_count; ++ui1_i)
            {
                ui1_range_id = (UINT8)((pt_mkv_handler->apt_range_obj[ui1_i]->ui4_range_id 
                                            & MKV_MIDXBULD_RANGE_ID_MASK)
                                        >> 8);
                if ((ui1_range_id == pt_mkv_handler->ui1_idx_range_id_last) ||
                    (ui1_key_range_id == pt_mkv_handler->ui1_idx_range_id_last))
                {
                    b_occupied = TRUE;
                    pt_mkv_handler->ui1_idx_range_id_last = (UINT8)
                           ((pt_mkv_handler->ui1_idx_range_id_last == 0xff)
                           ?  1 : (pt_mkv_handler->ui1_idx_range_id_last + 1));
                    break;
                }
            }
        }
    
        *pui4_range_id = (UINT32)pt_mkv_handler->ui1_idx_range_id_last;
    
        pt_mkv_handler->ui1_idx_range_id_last = (UINT8)
              ((pt_mkv_handler->ui1_idx_range_id_last == 0xff)
              ? 1 : (pt_mkv_handler->ui1_idx_range_id_last + 1));
        
        /*midxbuld hande + range id + table id*/
        *pui4_range_id = ((pt_midxbuld_obj->h_midxbuld & 0x0000ffff) << 16) +
                         (*pui4_range_id << 8);
        DBG_INFO(("_mkv_midxbuld_alloc_range_id:range id=%u\r\n",
                  *pui4_range_id));
        
        return TRUE;
    }
    
    DBG_INFO(("_mkv_midxbuld_alloc_range_id:fail\r\n"));
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_free_range_obj
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
static BOOL _mkv_midxbuld_free_range_obj (MIDXBULD_OBJ_T*   pt_midxbuld_obj,
                                                MKV_IDXBULD_OBJ_T* pt_mkv_handler,
                                                UINT32             ui4_range_id)
{
    MKV_IDXBULD_RANGE_OBJ_T*    pt_range_obj;
    UINT8                       ui1_i = 0;
    BOOL                        b_id_match = FALSE;
    MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    UINT32                      ui4_tbl_id;
    BOOL                        b_non_key = TRUE; /*if false it is key range*/

    /*Parameters check*/
    if (NULL == pt_midxbuld_obj || NULL == pt_mkv_handler)
    {
        DBG_ERROR(("_mkv_midxbuld_free_range_obj:NULL\r\n"));
        return FALSE;
    }
    
    /*init NULL*/
    pt_range_obj = NULL;
    
    if (pt_mkv_handler->ui1_range_count > MKV_MIDXBULD_RANGE_MAX)
    {
        DBG_ERROR(("_mkv_midxbuld_free_range_obj:error count!!!\r\n"));
        return FALSE;
    }
    /*If it is key range?*/
    if (pt_mkv_handler->pt_key_range_obj->ui4_range_id == ui4_range_id)
    {
        b_id_match = TRUE;
        b_non_key = FALSE;
        pt_range_obj = pt_mkv_handler->pt_key_range_obj;
    }
    /*non key range*/
    else if (pt_mkv_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_mkv_handler->ui1_range_count; ++ui1_i)
        {
            if (pt_mkv_handler->apt_range_obj[ui1_i]->ui4_range_id == ui4_range_id)
            {
                b_id_match = TRUE;
                b_non_key = TRUE;
                pt_range_obj = pt_mkv_handler->apt_range_obj[ui1_i];
                break;
            }
        }
    }
    else
    {
        DBG_INFO(("_mkv_midxbuld_free_range_obj:did not find range id=%\r\n",
                  ui4_range_id));
        return FALSE;
    }
    
    if (FALSE == b_id_match)
    {
        DBG_INFO(("_mkv_midxbuld_free_range_obj:not find range id=%\r\n",
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
            _mkv_midxbuld_free_table_obj(pt_midxbuld_obj,pt_mkv_handler,ui4_tbl_id);
        }

        /*free the given range*/
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);

        /*merge table array  */
        if (TRUE == b_non_key)
        {
            while ((ui1_i + 1) < pt_mkv_handler->ui1_range_count)
            {
                if (ui1_i + 1 >= MKV_MIDXBULD_RANGE_MAX)
                {
                    DBG_ERROR(("_mkv_midxbuld_free_range_obj:error index!\r\n"));
                    return FALSE;
                }
                pt_mkv_handler->apt_range_obj[ui1_i] = 
                                       pt_mkv_handler->apt_range_obj[ui1_i + 1];
                pt_mkv_handler->apt_range_obj[++ui1_i] = NULL;
            }
            --(pt_mkv_handler->ui1_range_count);
        }
        else
        {
            pt_mkv_handler->pt_key_range_obj = NULL;
        }
        
        DBG_INFO(("_mkv_midxbuld_free_range_obj:free range id=%d OK\r\n",
                  ui4_range_id));
        return TRUE;
    }
    else
    {
        DBG_INFO(("_mkv_midxbuld_free_range_obj:free range id=%d fail\r\n",
                  ui4_range_id));
        return FALSE;
    }
    
}
/*--------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_alloc_table_id
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

static BOOL _mkv_midxbuld_alloc_table_id( MKV_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                                          UINT32*                  pui4_tbl_id )
{

    /*Parameters check*/
    if (NULL == pt_range_obj)
    {
        DBG_ERROR(("_mkv_midxbuld_alloc_table_id:NULL\r\n"));
        return FALSE;
    }
    
    *pui4_tbl_id = MIDXBULD_NULL_ID;

    /*not than 255*/
    if (pt_range_obj->ui1_tbl_total == 0xff)
    {
        DBG_ERROR(("_mkv_midxbuld_alloc_table_id:not more than 255\r\n"));
        return FALSE;
    }

    /*the lowest byte for table id*/
    *pui4_tbl_id = (pt_range_obj->ui4_range_id & 0xffffff00) +
                   (++pt_range_obj->ui1_tbl_total);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_free_table_obj
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

static BOOL _mkv_midxbuld_free_table_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           MKV_IDXBULD_OBJ_T*  pt_mkv_handler,
                                           UINT32          ui4_tbl_id)
{
    MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_last_obj;
    UINT8                       ui1_free_range_id;
    UINT8                       ui1_range_id; 
    UINT8                       ui1_i;


    /*Parameters check*/
    if (NULL == pt_midxbuld_obj || NULL == pt_mkv_handler)
    {
        DBG_ERROR(("_mkv_midxbuld_free_table_obj:NULL\r\n"));
        return FALSE;
    }

    pt_obj = NULL;
    pt_last_obj = NULL;
    if (pt_mkv_handler->ui1_range_count > MKV_MIDXBULD_RANGE_MAX)
    {
        DBG_ERROR(("_mkv_midxbuld_free_table_obj:error count!!!\r\n"));
        return FALSE;
    }
    
    /*search the range id by table id*/
    if (pt_mkv_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == ui4_tbl_id)
    {
        pt_obj = pt_mkv_handler->pt_key_range_obj->pt_tbl_obj;
        pt_mkv_handler->pt_key_range_obj->ui1_tbl_total--;
        pt_mkv_handler->pt_key_range_obj->pt_tbl_obj = pt_obj->pt_next;
    }
    else
    {   
        /*get range id by table id*/
        ui1_free_range_id = (UINT8)
                            ((ui4_tbl_id & MKV_MIDXBULD_RANGE_ID_MASK) >> 8);
        
        for (ui1_i = 0; ui1_i < pt_mkv_handler->ui1_range_count; ui1_i++)
        {
            ui1_range_id = (UINT8)
                            ((pt_mkv_handler->apt_range_obj[ui1_i]->ui4_range_id 
                                  & MKV_MIDXBULD_RANGE_ID_MASK) 
                              >> 8);
            /*whether it is the target range id?*/
            if (ui1_range_id == ui1_free_range_id)
            {
                BOOL b_id_match = FALSE;
            
                pt_obj = pt_mkv_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
                
                /*pt_obj should be not NULL,if the given table is the first in a range*/
                if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_mkv_handler->apt_range_obj[ui1_i]->pt_tbl_obj = pt_obj->pt_next;
                    --(pt_mkv_handler->apt_range_obj[ui1_i]->ui1_tbl_total);
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
                        --(pt_mkv_handler->apt_range_obj[ui1_i]->ui1_tbl_total);
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
        DBG_INFO(("_mkv_midxbuld_free_table_obj:not find table=%u\r\n",
                  ui4_tbl_id));
        return FALSE;
    }

    /*if found free related memory*/
    if (pt_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry)
    {
        _mm_util_mem_free_with_tag(pt_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry,
                                   MM_MEM_MIDX_TAG);
        pt_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
    }
    _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
    
    DBG_INFO(("_mkv_midxbuld_free_table_obj:free table=%u OK\r\n",
              ui4_tbl_id));
    
    return TRUE;
}

INT32 _mkv_midxbuld_parse_simple_block(
                              MIDXBULD_OBJ_T*                  pt_midxbuld_obj, 
                              EBML_ELE_BASIC_T*                pt_ele_head,
                              MKV_IDXBULD_SIMPLE_BLOCK_ELE_T*  pt_simple_block,
                              MKV_IDXBULD_OBJ_T*               pt_mkv_handler)
{
    setpos_fct                        pf_set_pos ;    
    copybytes_fct                     pf_copybyte;
    getpos_fct                        pf_get_pos;
    UINT8*                            pui1_head;       /*read buffer pointer*/
    UINT64                            ui8_cur_pos;
    /*EBML_ELE_BASIC_T                  t_ele_head;*/
    INT32                             i4_ret;
    UINT32                            ui4_read_size;
    /*UINT32                            ui4_id;
    UINT64                            ui8_size;*/
    UINT64                            ui8_idx_total_size;  
    UINT64                            ui8_block_size;
    EBML_VINT_T                       t_stream_id;
    
    if ((NULL == pt_simple_block) 
        || (NULL == pt_ele_head) 
        || (NULL == pt_midxbuld_obj)
        || (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_simple_block:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = MIDXBULDR_OK;
    /*get callback*/
    pf_copybyte = _mkv_midxbuld_copy_bytes;
    pf_set_pos = _mkv_midxbuld_setpos;
    pf_get_pos = _mkv_midxbuld_getpos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_simple_block:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_mkv_handler->pui1_temp_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_simple_block:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get the first index table offset into the mkv file*/
    ui8_cur_pos = _ebml_buf_ele_get_data_offset(pt_ele_head);
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
    ui8_idx_total_size = 0;
    ui8_block_size = _ebml_size_get_value(&(pt_ele_head->t_size));

    pt_simple_block->ui8_position = pt_ele_head->ui8_ele_offset;
    pt_simple_block->ui4_block_size = _ebml_buf_ele_get_ele_size(pt_ele_head);
    
    while (ui8_idx_total_size < ui8_block_size)
    {
        /*get stream id : vint   ; max length: EBML_MAX_VINT_LEN
              timecode  : sint16 ;             2bytes
              flag      : int8   ;             1byte 
        */
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             (SIZE_T)11, /*EBML_MAX_VINT_LEN + 2 + 1*/
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_simple_block:error object "
                       "header! %d \r\n", 
                       i4_ret));
            break;
        }
        
        i4_ret = _ebml_buf_vint(pui1_head, 
                                pt_mkv_handler->ui4_buf_length,
                                &t_stream_id);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_simple_block:"
                       "construct vint error(%d)!\r\n",
                       i4_ret));
            break;
        }
        pt_simple_block->ui2_stream_id = (UINT16)_ebml_vint_get_value(&t_stream_id);
        pui1_head += t_stream_id.ui1_id_len; /*get to next field data*/
        
        /*get timecode*/
        i4_ret = _ebml_buf_int(pui1_head, 
                               pt_mkv_handler->ui4_buf_length,
                               (UINT32)2,
                               &pt_simple_block->i8_timecode);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_simple_block:_ebml_buf_int! %d\r\n", 
                       i4_ret));
            break;
        }
        pui1_head += 2;
        
        /*get flag*/
        
        /*need not to parse frame data so herein break */
        break;
        
    }


    /*free memory*/
    if (MIDXBULDR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    return MIDXBULDR_OK;
}

INT32 _mkv_midxbuld_parse_block_group(
                              MIDXBULD_OBJ_T*                  pt_midxbuld_obj, 
                              EBML_ELE_BASIC_T*                pt_ele_head,
                              MKV_IDXBULD_BLOCK_GROUP_ELE_T*   pt_block_group,
                              MKV_IDXBULD_OBJ_T*               pt_mkv_handler)
{
    setpos_fct                        pf_set_pos ;    
    copybytes_fct                     pf_copybyte;
    getpos_fct                        pf_get_pos;
    UINT8*                            pui1_head;       /*read buffer pointer*/
    UINT64                            ui8_cur_pos;
    UINT32                            ui4_head_size;
    EBML_ELE_BASIC_T                  t_ele_head;
    INT32                             i4_ret;
    UINT32                            ui4_read_size;
    UINT32                            ui4_id;
    UINT64                            ui8_size;
    UINT64                            ui8_idx_total_size;  
    UINT64                            ui8_block_group_size;
    UINT32                            ui4_copy_bytes;
    
    if ((NULL == pt_block_group) 
        || (NULL == pt_ele_head) 
        || (NULL == pt_midxbuld_obj)
        || (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_block_group:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = MIDXBULDR_OK;
    /*get callback*/
    pf_copybyte = _mkv_midxbuld_copy_bytes;
    pf_set_pos = _mkv_midxbuld_setpos;
    pf_get_pos = _mkv_midxbuld_getpos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_block_group:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_mkv_handler->pui1_temp_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_block_group:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get the first index table offset into the mkv file*/
    ui8_cur_pos = _ebml_buf_ele_get_data_offset(pt_ele_head);
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
    ui8_idx_total_size = 0;
    ui8_block_group_size = _ebml_size_get_value(&(pt_ele_head->t_size));

    while (ui8_idx_total_size < ui8_block_group_size)
    {
        ui4_copy_bytes = (UINT32)EBML_BASE_MAX_HEADER_LEN;
        if ((ui8_cur_pos + ui4_copy_bytes) > (pt_mkv_handler->ui8_file_size))
        {
            DBG_INFO(("_mkv_midxbuld_parse_block_group: sub elment head size < 12!\r\n"));
            ui4_copy_bytes = (UINT32)(pt_mkv_handler->ui8_file_size - ui8_cur_pos);
        }
        /*get ID and size, load object header from file*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             ui4_copy_bytes, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_block_group:error object "
                       "header! %d \r\n", 
                       i4_ret));
            break;
        }
        
        i4_ret = _ebml_buf_ele_base(pui1_head, 
                                    pt_mkv_handler->ui4_buf_length,
                                    ui8_cur_pos, 
                                    &t_ele_head);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_block_group:error object header(%d)!\r\n",
                       i4_ret));
            break;
        }

        ui4_id = _ebml_id_get_value(&(t_ele_head.t_id));
        ui8_size = _ebml_size_get_value(&(t_ele_head.t_size));

        ui4_head_size = _ebml_buf_ele_get_head_size(&t_ele_head);
        ui8_idx_total_size += ui4_head_size;
        ui8_cur_pos += ui4_head_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_block_group:%d \r\n", i4_ret));
            break;
        }
        
        /*Parse block ID: A1*/
        if (0xA1 == ui4_id)
        {
            if ((ui8_size + ui4_head_size) < ui8_block_group_size)
            {
                /*add this to improve perfermance!!!*/
                DBG_INFO(("_mkv_midxbuld_parse_block_group:this is not key block,size %llu !!!\r\n", ui8_block_group_size));
            }
            else
            {
                i4_ret = _mkv_midxbuld_parse_simple_block(pt_midxbuld_obj,
                                                          &t_ele_head,
                                                          &(pt_block_group->t_block),
                                                          pt_mkv_handler);
                if (INPUTR_OK != i4_ret)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_block_group:"
                               "parse_simple_block err=%d \r\n", i4_ret));
                    break;
                }
            }
        }
        /*Parse reference block: Fb*/
        else if (0xFB == ui4_id)
        {
            /*do not need parse*/
        }
        /*Parse block duration: 9B*/
        else if (0x9B == ui4_id)
        {
            /*do not need parse*/
        }
        else
        {
            /*This may be CRC32, just skip it*/
        }
        
        ui8_idx_total_size += ui8_size;
        ui8_cur_pos += ui8_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_block_group:%d \r\n", i4_ret));
            break;
        }
        
    }

    /*free memory*/
    if (MIDXBULDR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    return MIDXBULDR_OK;
}


INT32 _mkv_midxbuld_parse_key_block(MIDXBULD_OBJ_T*          pt_midxbuld_obj, 
                                    EBML_UINT                        ui8_timecode,
                                    MKV_IDXBULD_CUE_TRACK_POS_ELE_T* pt_cue_pos,
                                    MKV_IDXBULD_OBJ_T*               pt_mkv_handler)
{
    setpos_fct                        pf_set_pos ;    
    copybytes_fct                     pf_copybyte;
    getpos_fct                        pf_get_pos;
    UINT8*                            pui1_head;       /*read buffer pointer*/
    UINT64                            ui8_cur_pos;
    UINT32                            ui4_head_size;
    EBML_ELE_BASIC_T                  t_ele_head;
    INT32                             i4_ret;
    UINT32                            ui4_read_size;
    UINT32                            ui4_id;
    UINT64                            ui8_size;
    UINT64                            ui8_idx_total_size;  
    UINT64                            ui8_cluster_data_size;
    EBML_UINT                         ui8_block_timecode;
    MKV_IDXBULD_CLUSTER_ELE_T         t_cluster;
    BOOL                              b_cluster_tm_found;
    BOOL                              b_key_found;
    MKV_IDXBULD_SIMPLE_BLOCK_ELE_T*   pt_simple_block;    
    MKV_IDXBULD_BLOCK_GROUP_ELE_T*    pt_block_group; 
    UINT32                            ui4_copy_bytes;

    
    if ((NULL == pt_cue_pos) 
        || (NULL == pt_midxbuld_obj)
        || (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_key_block:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /*get callback*/
    pf_copybyte = _mkv_midxbuld_copy_bytes;
    pf_set_pos = _mkv_midxbuld_setpos;
    pf_get_pos = _mkv_midxbuld_getpos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_key_block:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_mkv_handler->pui1_temp_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_key_block:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
   
    /*get the cluster offset into the mkv file*/
    ui8_cur_pos = pt_mkv_handler->ui8_seg_data_offset + pt_cue_pos->ui8_pos;
    pt_cue_pos->ui8_cluster_pos = ui8_cur_pos;
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
    ui4_copy_bytes = (UINT32)EBML_BASE_MAX_HEADER_LEN;
    if ((ui8_cur_pos + ui4_copy_bytes) > (pt_mkv_handler->ui8_file_size))
    {
        DBG_INFO(("_mkv_midxbuld_parse_key_block: elment head size < 12!\r\n"));
        ui4_copy_bytes = (UINT32)(pt_mkv_handler->ui8_file_size - ui8_cur_pos);
    }
    i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                         pt_mkv_handler->ui4_buf_length , 
                         ui4_copy_bytes, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_key_block:error object "
                   "header! %d \r\n", 
                   i4_ret));
        return i4_ret;
    }
    
    i4_ret = _ebml_buf_ele_base(pui1_head, 
                                pt_mkv_handler->ui4_buf_length,
                                ui8_cur_pos, 
                                &t_ele_head);
    if (EBMLR_OK != i4_ret)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_key_block:error object header(%d)!\r\n",
                   i4_ret));
        return i4_ret;
    }

    ui4_id = _ebml_id_get_value(&(t_ele_head.t_id));
    ui8_size = _ebml_size_get_value(&(t_ele_head.t_size));
    ui8_cur_pos += _ebml_buf_ele_get_head_size(&t_ele_head);

    /*assert: ID should be  1F 43 b6 75*/
    if (0x1f43b675 != ui4_id)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_key_block:error ID(%d)!\r\n",
                   ui4_id));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
    if (INPUTR_OK != i4_ret)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:%d \r\n", i4_ret));
        return i4_ret;
    }

    /*begin to get data*/
    ui8_cluster_data_size = ui8_size;
    ui8_idx_total_size = 0;
    ui8_block_timecode = 0;
    x_memset(&t_cluster, 0, sizeof(t_cluster));
    b_cluster_tm_found = FALSE;
    b_key_found = FALSE;
    
    while (ui8_idx_total_size < ui8_cluster_data_size)
    {
        ui4_copy_bytes = (UINT32)EBML_BASE_MAX_HEADER_LEN;
        if ((ui8_cur_pos + ui4_copy_bytes) > (pt_mkv_handler->ui8_file_size))
        {
            DBG_INFO(("_mkv_midxbuld_parse_key_block: sub elment head size < 12!\r\n"));
            ui4_copy_bytes = (UINT32)(pt_mkv_handler->ui8_file_size - ui8_cur_pos);
        }
        /*get ID and size, load object header from file*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             ui4_copy_bytes, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_key_block:error object "
                       "header! %d \r\n", 
                       i4_ret));
            break;
        }
        
        i4_ret = _ebml_buf_ele_base(pui1_head, 
                                    pt_mkv_handler->ui4_buf_length,
                                    ui8_cur_pos, 
                                    &t_ele_head);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_key_block:error object header(%d)!\r\n",
                       i4_ret));
            break;
        }

        ui4_id = _ebml_id_get_value(&(t_ele_head.t_id));
        ui8_size = _ebml_size_get_value(&(t_ele_head.t_size));

        ui4_head_size = _ebml_buf_ele_get_head_size(&t_ele_head);
        ui8_idx_total_size += ui4_head_size;
        ui8_cur_pos += ui4_head_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_key_block:%d \r\n", i4_ret));
            break;
        }

        switch (ui4_id)
        {
            /*Cluster timecode*/
            case 0xE7:
                /*get value*/
                i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                                     pt_mkv_handler->ui4_buf_length , 
                                     (SIZE_T)ui8_size, 
                                     &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_key_block:pf_copybyte! %d \r\n", 
                               i4_ret));
                    break;
                }
                i4_ret = _ebml_buf_uint(pui1_head, 
                                        pt_mkv_handler->ui4_buf_length,
                                        (UINT32)ui8_size,
                                        &(t_cluster.ui8_base_timecode));
                if (EBMLR_OK != i4_ret)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_key_block:_ebml_buf_uint! %d \r\n", 
                               i4_ret));
                    break;
                }
                b_cluster_tm_found = TRUE;
                break;
            /*Cluster's parent position relative to its grand parent*/
            case 0xA7:
                break;
            /*Preceding cluster size*/
            case 0xAB:
                break;
            /*Block group*/
            case 0xA0:
                pt_block_group = (MKV_IDXBULD_BLOCK_GROUP_ELE_T*)
                     _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_BLOCK_GROUP_ELE_T),
                                                 MM_MEM_MIDX_TAG);
                if (NULL == pt_block_group)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_key_block:fail to alloc memory \r\n"));
                    i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                    break;
                }
                x_memset(pt_block_group, 0, sizeof(MKV_IDXBULD_BLOCK_GROUP_ELE_T));
                if (NULL != t_cluster.pt_block_group_head)
                {
                    pt_block_group->pt_next = t_cluster.pt_block_group_head;
                    t_cluster.pt_block_group_head = pt_block_group;
                }
                else
                {
                    t_cluster.pt_block_group_head = pt_block_group;
                }
                ++t_cluster.ui4_block_group_count;
                
                i4_ret = _mkv_midxbuld_parse_block_group(pt_midxbuld_obj, 
                                                         &t_ele_head,
                                                         pt_block_group,
                                                         pt_mkv_handler);
                if (MIDXBULDR_OK != i4_ret)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_key_block:parse_block_group \r\n"));
                    break;
                }
                /*compare block timecode with the cue point timecode*/
                if (TRUE == b_cluster_tm_found)
                {
                    /*Get block timecode */
                    ui8_block_timecode = t_cluster.ui8_base_timecode
                                         + pt_block_group->t_block.i8_timecode;
                    /*find out key block*/
                    if ((pt_cue_pos->ui2_stream_id 
                            == pt_block_group->t_block.ui2_stream_id) 
                         && (ui8_block_timecode == ui8_timecode))
                    {
                        b_key_found = TRUE;
                        /*set offset and block size*/
                        pt_cue_pos->ui8_pos = 
                                  pt_block_group->t_block.ui8_position;
                        pt_cue_pos->ui4_block_size = 
                                  pt_block_group->t_block.ui4_block_size;
                    }
                    
                }
                break;
            /*Simple block*/
            case 0xA3:
                pt_simple_block = (MKV_IDXBULD_SIMPLE_BLOCK_ELE_T*)
                     _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_SIMPLE_BLOCK_ELE_T),
                                                 MM_MEM_MIDX_TAG);
                if (NULL == pt_simple_block)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_key_block:fail to alloc memory \r\n"));
                    i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                    break;
                }
                x_memset(pt_simple_block, 0, sizeof(MKV_IDXBULD_SIMPLE_BLOCK_ELE_T));
                if (NULL != t_cluster.pt_block_head)
                {
                    pt_simple_block->pt_next = t_cluster.pt_block_head;
                    t_cluster.pt_block_head = pt_simple_block;
                }
                else
                {
                    t_cluster.pt_block_head = pt_simple_block;
                }
                ++t_cluster.ui4_simple_block_count;
                
                i4_ret = _mkv_midxbuld_parse_simple_block(pt_midxbuld_obj, 
                                                          &t_ele_head,
                                                          pt_simple_block,
                                                          pt_mkv_handler);
                if (MIDXBULDR_OK != i4_ret)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_key_block:parse_simple_block \r\n"));
                    break;
                }

                /*compare block timecode with the cue point timecode*/
                if (TRUE == b_cluster_tm_found)
                {
                    /*Get block timecode */
                    ui8_block_timecode = t_cluster.ui8_base_timecode
                                         + pt_simple_block->i8_timecode ;
                    /*find out key block*/
                    if ((pt_cue_pos->ui2_stream_id == pt_simple_block->ui2_stream_id)
                         && (ui8_block_timecode == ui8_timecode))
                    {
                        b_key_found = TRUE;
                        /*set offset and block size*/
                        pt_cue_pos->ui8_pos = 
                                  pt_simple_block->ui8_position;
                        pt_cue_pos->ui4_block_size = 
                                  pt_simple_block->ui4_block_size;
                    }
                }
                break;
            default:
                /*Just skip*/
                ;
                
        }
        
        if (MIDXBULDR_OK != i4_ret)
        {
            break;
        }

        /*find out key block*/
        if (TRUE == b_key_found)
        {
            break;
        }
        
        ui8_idx_total_size += ui8_size;
        ui8_cur_pos += ui8_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:%d \r\n", i4_ret));
            break;
        }
    }

    if ((MIDXBULDR_OK == i4_ret) && (TRUE != b_key_found))
    {
        /*seach block group */
        pt_block_group = t_cluster.pt_block_group_head;
        while (NULL != pt_block_group)
        {
            ui8_block_timecode = t_cluster.ui8_base_timecode
                                 + pt_block_group->t_block.i8_timecode;
            /*find out key block*/
            if ((pt_cue_pos->ui2_stream_id == pt_block_group->t_block.ui2_stream_id) 
                 && (ui8_block_timecode == ui8_timecode))
            {
                b_key_found = TRUE;
                /*set offset and block size*/
                pt_cue_pos->ui8_pos = 
                          pt_block_group->t_block.ui8_position;
                pt_cue_pos->ui4_block_size = 
                          pt_block_group->t_block.ui4_block_size;
                break;
            }
            pt_block_group = pt_block_group->pt_next;
        }
        
        /*seach simple block */
        if (TRUE != b_key_found)
        {
            pt_simple_block = t_cluster.pt_block_head;
            while (NULL != pt_simple_block)
            {

                ui8_block_timecode = t_cluster.ui8_base_timecode
                                     + pt_simple_block->i8_timecode ;
                /*find out key block*/
                if ((pt_cue_pos->ui2_stream_id == pt_simple_block->ui2_stream_id)
                    && (ui8_block_timecode == ui8_timecode))
                {
                    b_key_found = TRUE;
                    /*set offset and block size*/
                    pt_cue_pos->ui8_pos = 
                              pt_simple_block->ui8_position;
                    pt_cue_pos->ui4_block_size = 
                              pt_simple_block->ui4_block_size;
                    break;
                }
                pt_simple_block = pt_simple_block->pt_next;
            }
        }
    }

 
    /*******free memory no matter failed ******/
    /*delete block group */
    while (NULL != t_cluster.pt_block_group_head)
    {   
        pt_block_group = t_cluster.pt_block_group_head;
        t_cluster.pt_block_group_head = pt_block_group->pt_next;
        _mm_util_mem_free_with_tag(pt_block_group, MM_MEM_MIDX_TAG);
    }
    t_cluster.pt_block_group_head = NULL;

    /*delete simple block*/
    while (NULL != t_cluster.pt_block_head)
    {   
        pt_simple_block = t_cluster.pt_block_head;
        t_cluster.pt_block_head = pt_simple_block->pt_next;
        _mm_util_mem_free_with_tag(pt_simple_block, MM_MEM_MIDX_TAG);
    }
    t_cluster.pt_block_head = NULL;

    /*do ....*/
    if (MIDXBULDR_OK != i4_ret)
    {   
        return i4_ret;
    }
    
    /*if do not find key block it is abnormal Assert....*/
    if (TRUE != b_key_found)
    {
        pt_cue_pos->ui8_pos = (UINT64)0xFFFFFFFFFFFFFFFFll;
        pt_cue_pos->ui4_block_size = 0xFFFFFFFF;
        DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:abnormal no key block "
                   "found out!!!\r\n"));
    }
   
    return MIDXBULDR_OK;
}

INT32 _mkv_midxbuld_parse_cue_pos(MIDXBULD_OBJ_T*                  pt_midxbuld_obj, 
                                  EBML_ELE_BASIC_T*                pt_ele_head,
                                  MKV_IDXBULD_CUE_TRACK_POS_ELE_T* pt_cue_pos,
                                  MKV_IDXBULD_OBJ_T*      pt_mkv_handler)
{
    setpos_fct                        pf_set_pos ;    
    copybytes_fct                     pf_copybyte;
    getpos_fct                        pf_get_pos;
    UINT8*                            pui1_head;       /*read buffer pointer*/
    UINT64                            ui8_cur_pos;
    UINT32                            ui4_head_size;
    EBML_ELE_BASIC_T                  t_ele_head;
    INT32                             i4_ret;
    UINT32                            ui4_read_size;
    UINT32                            ui4_id;
    UINT64                            ui8_size;
    UINT64                            ui8_idx_total_size;  
    UINT64                            ui8_cue_pos_size;
    EBML_UINT                         ui8_value;
    UINT32                            ui4_copy_bytes;
    
    if ((NULL == pt_cue_pos) 
        || (NULL == pt_ele_head) 
        || (NULL == pt_midxbuld_obj)
        || (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = MIDXBULDR_OK;
    /*get callback*/
    pf_copybyte = _mkv_midxbuld_copy_bytes;
    pf_set_pos = _mkv_midxbuld_setpos;
    pf_get_pos = _mkv_midxbuld_getpos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_mkv_handler->pui1_temp_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get the first index table offset into the mkv file*/
    ui8_cur_pos = _ebml_buf_ele_get_data_offset(pt_ele_head);
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
    ui8_idx_total_size = 0;
    ui8_cue_pos_size = _ebml_size_get_value(&(pt_ele_head->t_size));

    pt_cue_pos->ui2_stream_id = 0xFFFF;
    pt_cue_pos->ui4_block_size = 0xFFFFFFFF;
    pt_cue_pos->ui8_pos = (UINT64)0xFFFFFFFFFFFFFFFFll;
    pt_cue_pos->pt_next = NULL;
    
    while (ui8_idx_total_size < ui8_cue_pos_size)
    {
        ui4_copy_bytes = (UINT32)EBML_BASE_MAX_HEADER_LEN;
        if ((ui8_cur_pos + ui4_copy_bytes) > (pt_mkv_handler->ui8_file_size))
        {
            DBG_INFO(("_mkv_midxbuld_parse_cue_pos: sub element head size < 12!\r\n"));
            ui4_copy_bytes = (UINT32)(pt_mkv_handler->ui8_file_size - ui8_cur_pos);
        }
        /*get ID and size, load object header from file*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             ui4_copy_bytes, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:error object "
                       "header! %d \r\n", 
                       i4_ret));
            break;
        }
        
        i4_ret = _ebml_buf_ele_base(pui1_head, 
                                    pt_mkv_handler->ui4_buf_length,
                                    ui8_cur_pos, 
                                    &t_ele_head);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:error object header(%d)!\r\n",
                       i4_ret));
            break;
        }

        ui4_id = _ebml_id_get_value(&(t_ele_head.t_id));
        ui8_size = _ebml_size_get_value(&(t_ele_head.t_size));

        ui4_head_size = _ebml_buf_ele_get_head_size(&t_ele_head);
        ui8_idx_total_size += ui4_head_size;
        ui8_cur_pos += ui4_head_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:%d \r\n", i4_ret));
            break;
        }
        
        /*get value*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             (SIZE_T)ui8_size, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:pf_copybyte! %d \r\n", 
                       i4_ret));
            break;
        }
            
        i4_ret = _ebml_buf_uint(pui1_head, 
                                pt_mkv_handler->ui4_buf_length,
                                (UINT32)ui8_size,
                                &ui8_value);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:_ebml_buf_uint! %d \r\n", 
                       i4_ret));
            break;
        }
 
        
        /*Parse stream ID: F7*/
        if (0xF7 == ui4_id)
        {
            pt_cue_pos->ui2_stream_id = (UINT16)ui8_value;
        }
        /*Parse position relative to segment: F1*/
        else if (0xF1 == ui4_id)
        {
            pt_cue_pos->ui8_pos = ui8_value;
        }
        /*Parse key block number: 53 78*/
        else if (0x5378 == ui4_id)
        {
            /*do not need */
        }
        else
        {
            /*This may be CRC32, just skip it*/
        }
        
        ui8_idx_total_size += ui8_size;
        ui8_cur_pos += ui8_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:%d \r\n", i4_ret));
            break;
        }
        
    }

    /*free memory*/
    if (MIDXBULDR_OK != i4_ret)
    {
        return i4_ret;
    }
    
    return MIDXBULDR_OK;
}

INT32 _mkv_midxbuld_parse_cue_point(MIDXBULD_OBJ_T*      pt_midxbuld_obj, 
                                    EBML_ELE_BASIC_T*            pt_ele_head,
                                    MKV_IDXBULD_CUE_POINT_ELE_T* pt_cue_point,
                                    MKV_IDXBULD_OBJ_T*           pt_mkv_handler)
{
    setpos_fct                        pf_set_pos ;    
    copybytes_fct                     pf_copybyte;
    getpos_fct                        pf_get_pos;
    UINT8*                            pui1_head; /*read buffer pointer*/
    UINT64                            ui8_cur_pos;
    UINT32                            ui4_head_size;
    EBML_ELE_BASIC_T                  t_ele_head;
    INT32                             i4_ret;
    UINT32                            ui4_read_size;
    UINT32                            ui4_id;
    UINT64                            ui8_size;
    UINT64                            ui8_idx_total_size;  /*per packe size*/
    UINT64                            ui8_cue_point_size;
    MKV_IDXBULD_CUE_TRACK_POS_ELE_T*  pt_cue_position;
    MKV_IDXBULD_CUE_TRACK_POS_ELE_T*  pt_track_pos_temp;
    UINT32                            ui4_copy_bytes;

    
    if ((NULL == pt_cue_point) 
        || (NULL == pt_ele_head) 
        || (NULL == pt_midxbuld_obj)
        || (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_point:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = MIDXBULDR_OK;
    /*get callback*/
    pf_copybyte = _mkv_midxbuld_copy_bytes;
    pf_set_pos = _mkv_midxbuld_setpos;
    pf_get_pos = _mkv_midxbuld_getpos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_point:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_mkv_handler->pui1_temp_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_point:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get the first index table offset into the mkv file*/
    ui8_cur_pos = _ebml_buf_ele_get_data_offset(pt_ele_head);
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
    ui8_idx_total_size = 0;
    ui8_cue_point_size = _ebml_size_get_value(&(pt_ele_head->t_size));

    pt_cue_point->ui2_pos_num = 0;
    pt_cue_point->pt_track_pos_head = NULL;
    pt_cue_point->ui8_timecode = (UINT64)(-1);
    pt_cue_point->pt_next = NULL;
    
    while (ui8_idx_total_size < ui8_cue_point_size)
    {
        ui4_copy_bytes = (UINT32)EBML_BASE_MAX_HEADER_LEN;
        if ((ui8_cur_pos + ui4_copy_bytes) > (pt_mkv_handler->ui8_file_size))
        {
            DBG_INFO(("_mkv_midxbuld_parse_cue_point: sub element head size < 12!\r\n"));
            ui4_copy_bytes = (UINT32)(pt_mkv_handler->ui8_file_size - ui8_cur_pos);
        }
        /*get ID and size, load object header from file*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             ui4_copy_bytes, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_point:error object "
                       "header! %d \r\n", 
                       i4_ret));
            break;
        }
        
        i4_ret = _ebml_buf_ele_base(pui1_head, 
                                    pt_mkv_handler->ui4_buf_length,
                                    ui8_cur_pos, 
                                    &t_ele_head);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_point:error object header(%d)!\r\n",
                       i4_ret));
            break;
        }

        ui4_id = _ebml_id_get_value(&(t_ele_head.t_id));
        ui8_size = _ebml_size_get_value(&(t_ele_head.t_size));

        ui4_head_size = _ebml_buf_ele_get_head_size(&t_ele_head);
        ui8_idx_total_size += ui4_head_size;
        ui8_cur_pos += ui4_head_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:%d \r\n", i4_ret));
            break;
        }
        
        /*Parse Cue Position : BB*/
        if (0xB7 == ui4_id)
        {
            pt_cue_position = (MKV_IDXBULD_CUE_TRACK_POS_ELE_T*)
                     _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_CUE_TRACK_POS_ELE_T),
                                                 MM_MEM_MIDX_TAG);
            if (NULL == pt_cue_position)
            {
                DBG_ERROR(("_mkv_midxbuld_parse_cue_point:fail to alloc memory \r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break;
            }
            x_memset(pt_cue_position, 0, sizeof(MKV_IDXBULD_CUE_TRACK_POS_ELE_T));
            if (NULL != pt_cue_point->pt_track_pos_head)
            {
                pt_cue_point->pt_track_pos_head->pt_next = pt_cue_position;
                pt_cue_point->pt_track_pos_head = pt_cue_position;
            }
            else
            {
                pt_cue_point->pt_track_pos_head = pt_cue_position;
            }
            /*count cuepoint*/
            ++pt_cue_point->ui2_pos_num;
            i4_ret = _mkv_midxbuld_parse_cue_pos(pt_midxbuld_obj, 
                                                 &t_ele_head, 
                                                 pt_cue_position, 
                                                 pt_mkv_handler);
            if (MIDXBULDR_OK != i4_ret)
            {
                DBG_ERROR(("_mkv_midxbuld_parse_cue_point: "
                           " _mkv_midxbuld_parse_cue_postion %d \r\n", i4_ret));
                break;
            }
        }
        /*Parse Cue timecode : B3*/
        else if (0xB3 == ui4_id)
        {
            /*get value*/
            i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                                 pt_mkv_handler->ui4_buf_length , 
                                 (SIZE_T)ui8_size, 
                                 &ui4_read_size);
            if (i4_ret != INPUTR_OK)
            {
                DBG_ERROR(("_mkv_midxbuld_parse_cue_point:pf_copybyte! %d \r\n", 
                           i4_ret));
                break;
            }
            
            
            i4_ret = _ebml_buf_uint(pui1_head, 
                                    pt_mkv_handler->ui4_buf_length,
                                    (UINT32)ui8_size,
                                    &(pt_cue_point->ui8_timecode));
            if (EBMLR_OK != i4_ret)
            {
                DBG_ERROR(("_mkv_midxbuld_parse_cue_point:_ebml_buf_uint! %d \r\n", 
                           i4_ret));
                break;
            }
        }
        else
        {
            /*This may be CRC32, just skip it*/
        }
        ui8_cur_pos += ui8_size;
        ui8_idx_total_size += ui8_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:%d \r\n", i4_ret));
            break;
        }
        
    }

    /*parse key block info by timecode got from cue point*/
    if (MIDXBULDR_OK == i4_ret)
    {
        pt_track_pos_temp = pt_cue_point->pt_track_pos_head;
        while (NULL != pt_track_pos_temp)
        {
            /*discard abnormal cue position entry*/
            if ((pt_cue_point->ui8_timecode >> 32))
            {
                DBG_INFO(("_mkv_midxbuld_parse_cue_point: this cue point tm is too large %llu!\r\n", pt_cue_point->ui8_timecode));
                pt_track_pos_temp->ui2_stream_id = (UINT16)0xFFFF;
            }
            else
            {
#if MKV_IDXBUILD_PARSE_KEY_BLOCK
                /*seek cluster to get key block offset*/
                i4_ret = _mkv_midxbuld_parse_key_block(pt_midxbuld_obj, 
                                                       pt_cue_point->ui8_timecode,
                                                       pt_track_pos_temp,
                                                       pt_mkv_handler);
                if (MIDXBULDR_OK != i4_ret)
                {
                    DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:key_block %d\r\n", i4_ret));
                    break;
                }
#endif
                /*discard abnormal cue entry*/
                if (pt_track_pos_temp->ui8_pos == ((UINT64)0xFFFFFFFFFFFFFFFFll))
                {
                    DBG_INFO(("_mkv_midxbuld_parse_cue_point: not found this cue point's key block!\r\n"));
                    pt_track_pos_temp->ui2_stream_id = (UINT16)0xFFFF;
                }
#if !MKV_IDXBUILD_PARSE_KEY_BLOCK
                /*only parse cluster position not key block postion*/
                else
                {
                    pt_track_pos_temp->ui8_cluster_pos = pt_track_pos_temp->ui8_pos + 
                                                         pt_mkv_handler->ui8_seg_data_offset;
                }
#endif
            }
            pt_track_pos_temp = pt_track_pos_temp->pt_next;
        }
        
    }
    
    /*free memory*/
    if (MIDXBULDR_OK != i4_ret)
    {
        while (NULL != pt_cue_point->pt_track_pos_head)
        {
            pt_track_pos_temp = pt_cue_point->pt_track_pos_head;
            pt_cue_point->pt_track_pos_head = pt_track_pos_temp->pt_next;
            _mm_util_mem_free_with_tag(pt_track_pos_temp, MM_MEM_MIDX_TAG);
            
        }
        pt_cue_point->pt_track_pos_head = NULL;
        
        return i4_ret;
    }
    
    
    return MIDXBULDR_OK;
    
}

INT32 _mkv_midxbuld_free_cue_ele(MKV_IDXBULD_CUE_ELE_T*  pt_cue_ele)
{
    MKV_IDXBULD_CUE_POINT_ELE_T*     pt_cue_point; 
    MKV_IDXBULD_CUE_TRACK_POS_ELE_T* pt_track_pos_temp;  

    if (NULL == pt_cue_ele)
    {
        DBG_ERROR(("_mkv_midxbuld_free_cue_ele:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    while (NULL != pt_cue_ele->pt_cue_point_head)
    {
        pt_cue_point = pt_cue_ele->pt_cue_point_head;
        pt_cue_ele->pt_cue_point_head = pt_cue_point->pt_next;

        while (NULL != pt_cue_point->pt_track_pos_head)
        {
            pt_track_pos_temp = pt_cue_point->pt_track_pos_head;
            pt_cue_point->pt_track_pos_head = pt_track_pos_temp->pt_next;
            _mm_util_mem_free_with_tag(pt_track_pos_temp, MM_MEM_MIDX_TAG);
        }
        _mm_util_mem_free_with_tag(pt_cue_point, MM_MEM_MIDX_TAG);           
    }
    pt_cue_ele->pt_cue_point_head = NULL;
    pt_cue_ele->pt_cue_point_tail = NULL;
    _mm_util_mem_free_with_tag(pt_cue_ele, MM_MEM_MIDX_TAG);
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_parse_cue_ele
 *
 * Description: This function get cue info
 *
 * Inputs:  pt_midxbuld_obj       specify midxbuld  obj 
 *          i8_offset :cue offset into file
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero values: Fail
 * Output:  pt_cue_elel
 * Note:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-10-29 : initial
 ----------------------------------------------------------------------------*/

INT32 _mkv_midxbuld_parse_cue_ele(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                  EBML_ELE_BASIC_T*       pt_ele_head,
                                  MKV_IDXBULD_CUE_ELE_T*  pt_cue_ele,
                                  MKV_IDXBULD_OBJ_T*      pt_mkv_handler)
{
    setpos_fct                       pf_set_pos ;    
    copybytes_fct                    pf_copybyte;
    getpos_fct                       pf_get_pos;
    UINT8*                           pui1_head; /*read buffer pointer*/
    UINT64                           ui8_cur_pos;
    UINT32                           ui4_head_size;
    EBML_ELE_BASIC_T                 t_ele_head;
    INT32                            i4_ret;
    UINT32                           ui4_read_size;
    UINT32                           ui4_id;
    UINT64                           ui8_size;
    UINT64                           ui8_idx_total_size;  /*per packe size*/
    UINT64                           ui8_cue_size;
    MKV_IDXBULD_CUE_POINT_ELE_T*     pt_cue_point; 
    UINT32                           ui4_copy_bytes;

    if ((NULL == pt_cue_ele) 
        || (NULL == pt_ele_head) 
        || (NULL == pt_midxbuld_obj)
        || (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    i4_ret = MIDXBULDR_OK;
    /*get callback*/
    pf_copybyte = _mkv_midxbuld_copy_bytes;
    pf_set_pos = _mkv_midxbuld_setpos;
    pf_get_pos = _mkv_midxbuld_getpos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_mkv_handler->pui1_temp_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    /******************************1. paser cues*******************************/
    /*get the first index table offset into the mkv file*/
    ui8_cur_pos = _ebml_buf_ele_get_data_offset(pt_ele_head);
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
    ui8_idx_total_size = 0;
    ui8_cue_size = _ebml_size_get_value(&(pt_ele_head->t_size));
    
    pt_cue_ele->ui4_index_count = 0;
    pt_cue_ele->pt_cue_point_head = NULL;
    pt_cue_ele->pt_cue_point_tail = NULL;
    
    /*begin read every cuepoint*/
    while (ui8_idx_total_size < ui8_cue_size)
    {
        ui4_copy_bytes = (UINT32)EBML_BASE_MAX_HEADER_LEN;
        if ((ui8_cur_pos + ui4_copy_bytes) > (pt_mkv_handler->ui8_file_size))
        {
            DBG_INFO(("_mkv_midxbuld_parse_cue_ele: sub elment head size < 12!\r\n"));
            ui4_copy_bytes = (UINT32)(pt_mkv_handler->ui8_file_size - ui8_cur_pos);
        }
        /*get ID and size, load object header from file*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             ui4_copy_bytes, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:error object "
                       "header! %d \r\n", 
                       i4_ret));
            break;
        }
        
        i4_ret = _ebml_buf_ele_base(pui1_head, 
                                    pt_mkv_handler->ui4_buf_length,
                                    ui8_cur_pos, 
                                    &t_ele_head);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:error object header(%d)!\r\n",
                       i4_ret));
            break;
        }

        ui4_id = _ebml_id_get_value(&(t_ele_head.t_id));
        ui8_size = _ebml_size_get_value(&(t_ele_head.t_size));

        ui4_head_size = _ebml_buf_ele_get_head_size(&t_ele_head);
        ui8_idx_total_size += ui4_head_size;
        ui8_cur_pos += ui4_head_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:%d \r\n", i4_ret));
            break;
        }
        
        /*Parse Cue Point : BB*/
        if (0xBB == ui4_id)
        {
            /*Get the index info entry*/
            pt_cue_point = (MKV_IDXBULD_CUE_POINT_ELE_T*)
                     _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_CUE_POINT_ELE_T),
                                                 MM_MEM_MIDX_TAG);
            if (NULL == pt_cue_point)
            {
                DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:fail to alloc memory \r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break;
            }
            x_memset(pt_cue_point, 0, sizeof(MKV_IDXBULD_CUE_POINT_ELE_T));
            if (NULL != pt_cue_ele->pt_cue_point_tail)
            {
                pt_cue_ele->pt_cue_point_tail->pt_next = pt_cue_point;
                pt_cue_ele->pt_cue_point_tail = pt_cue_point;
            }
            else
            {
                pt_cue_ele->pt_cue_point_head = pt_cue_point;
                pt_cue_ele->pt_cue_point_tail = pt_cue_point;
            }
            /*count cuepoint*/
            ++pt_cue_ele->ui4_index_count;
            i4_ret = _mkv_midxbuld_parse_cue_point(pt_midxbuld_obj, 
                                                   &t_ele_head, 
                                                   pt_cue_point,
                                                   pt_mkv_handler);
            if (MIDXBULDR_OK != i4_ret)
            {
                DBG_ERROR(("_mkv_midxbuld_parse_cue_ele: "
                           "_mkv_midxbuld_parse_cue_point %d \r\n", i4_ret));
                break;
            }
        }
        else
        {
            /*This may be CRC32, just skip it*/
        }
        
        ui8_cur_pos += ui8_size;
        ui8_idx_total_size += ui8_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_ele:%d \r\n", i4_ret));
            break;
        }
        
    }

    /*free memory*/
    if (MIDXBULDR_OK != i4_ret)
    {
        /*_mkv_midxbuld_free_cue_ele(pt_cue_ele);lijun*/        
        return i4_ret;
    }
    
    return MIDXBULDR_OK;
}

INT32 _mkv_midxbuld_parse_cues(MIDXBULD_OBJ_T*        pt_midxbuld_obj,
                                      MKV_IDXBULD_CUE_ELE_T*  pt_cue_ele,
                                      MKV_IDXBULD_OBJ_T*      pt_mkv_handler,
                                      MIDXBULD_SOURCE_INFO_T* pt_src_info)
{
    setpos_fct                       pf_set_pos ;    
    copybytes_fct                    pf_copybyte;
    getpos_fct                       pf_get_pos;
    
    BOOL                             b_cue_found;
    INT32                            i4_ret;
    UINT32                           ui4_copy_bytes;
    UINT32                           ui4_id;
    UINT32                           ui4_head_size;
    UINT32                           ui4_read_size;
    UINT64                           ui8_cur_pos;
    UINT64                           ui8_idx_total_size;
    UINT64                           ui8_size;
    
    UINT8*                           pui1_head;
    EBML_ELE_BASIC_T                 t_ele_head;
    
    ui8_idx_total_size  = 0;  /*index total size*/
    b_cue_found         = FALSE;
    i4_ret              = MIDXBULDR_OK;
    pf_copybyte         = _mkv_midxbuld_copy_bytes;
    pf_set_pos          = _mkv_midxbuld_setpos;
    pf_get_pos          = _mkv_midxbuld_getpos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cues:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    if (NULL == pt_mkv_handler)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cues:pt_mkv_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    pui1_head = pt_mkv_handler->pui1_temp_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_mkv_midxbuld_parse_cues:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }

    /*get the first index table offset into the mkv file*/
    ui8_cur_pos = pt_src_info->ui8_idx_offset;
    /*set to cue element position*/
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);

    while ((ui8_cur_pos < pt_midxbuld_obj->z_buff_leng)
           && (ui8_idx_total_size < pt_src_info->ui8_idx_size)) 
    {
        ui4_copy_bytes = (UINT32)EBML_BASE_MAX_HEADER_LEN;
        if ((ui8_cur_pos + ui4_copy_bytes) > (pt_mkv_handler->ui8_file_size))
        {
            DBG_INFO(("_mkv_midxbuld_parse_cues: sub element head size < 12!\r\n"));
            ui4_copy_bytes = (UINT32)(pt_mkv_handler->ui8_file_size - ui8_cur_pos);
        }
        /*get ID and size, load object header from file*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_mkv_handler->ui4_buf_length , 
                             ui4_copy_bytes, 
                             &ui4_read_size);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cues:error object "
                       "header! %d \r\n", 
                       i4_ret));
            break;
        }
        
        i4_ret = _ebml_buf_ele_base(pui1_head, 
                                    pt_mkv_handler->ui4_buf_length,
                                    ui8_cur_pos, 
                                    &t_ele_head);
        if (EBMLR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cues:error object"
                       " header(%d)!\r\n",
                       i4_ret));
            break;
        }

        ui4_id              = _ebml_id_get_value(&(t_ele_head.t_id));
        ui8_size            = _ebml_size_get_value(&(t_ele_head.t_size));
        ui4_head_size       = _ebml_buf_ele_get_head_size(&t_ele_head);
        ui8_idx_total_size  += ui4_head_size;
        ui8_cur_pos         += ui4_head_size;
        
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cue_pos:%d \r\n", i4_ret));
            break;
        }
        
        /*Parse Cues : ID 1C 53 BB 6B*/
        if (0x1C53BB6B == ui4_id)
        {
            i4_ret = _mkv_midxbuld_parse_cue_ele(pt_midxbuld_obj, 
                                                 &t_ele_head,
                                                 pt_cue_ele,
                                                 pt_mkv_handler);
            if (MIDXBULDR_OK != i4_ret)
            {
                DBG_ERROR(("_mkv_midxbuld_parse_cues:"
                           " _mkv_midxbuld_parse_cue_ele(%d)!\r\n",
                           i4_ret));
            }
            b_cue_found = TRUE;
            break;
        }

        /*Assert ,should be Cue info, If not, handle this exception by skipping it */
        ui8_cur_pos         += ui8_size;
        ui8_idx_total_size  += ui8_size;
        i4_ret = pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        if (INPUTR_OK != i4_ret)
        {
            DBG_ERROR(("_mkv_midxbuld_parse_cues:%d \r\n", i4_ret));
            break;
        }
    }

    if (MIDXBULDR_OK != i4_ret || TRUE != b_cue_found)
    {
        /*free memory*/
        DBG_ERROR(("_mkv_midxbuld_parse_cues:get cue error!!!\r\n"));
        return MIDXBULDR_INTERNAL_ERR;
    }
    return MIDXBULDR_OK;
}

UINT32 _mkv_midxbuld_get_key_num_by_stream_id(
                        MKV_IDXBULD_CUE_ELE_T*      pt_cue_ele,
                        UINT16                      ui2_stream_id,
                        UINT64*                     pui8_max_pos)
{
    MKV_IDXBULD_CUE_POINT_ELE_T*      pt_cue_point_head;
    MKV_IDXBULD_CUE_TRACK_POS_ELE_T*  pt_track_pos_head;
    UINT32                            ui4_count = 0;
    
    *pui8_max_pos = 0;
    if (NULL == pt_cue_ele)
    {
        DBG_ERROR(("_mkv_midxbuld_get_key_num_by_stream_id:NULL\r\n"));
        return 0;
    }
    
    pt_cue_point_head = pt_cue_ele->pt_cue_point_head;
    while (NULL != pt_cue_point_head)
    {
        /*search by id*/
        pt_track_pos_head = pt_cue_point_head->pt_track_pos_head;
        while (NULL != pt_track_pos_head)
        {
            if (ui2_stream_id == pt_track_pos_head->ui2_stream_id)
            {
                ++ui4_count;
                if (pt_track_pos_head->ui8_pos > *pui8_max_pos)
                {
                    *pui8_max_pos = pt_track_pos_head->ui8_pos;
                }
                break;
            }
            /*get to next stream */
            pt_track_pos_head = pt_track_pos_head->pt_next;
        }

        /*find out a index point for given stream id*/
        /*get to next index point*/
        pt_cue_point_head = pt_cue_point_head->pt_next;
        
    }
    return ui4_count;
}


INT32 _mkv_midxbuld_get_key_by_stream_id(
                        MKV_IDXBULD_CUE_ELE_T*      pt_cue_ele,
                        UINT16                      ui2_stream_id,
                        MKV_IDXBULD_OBJ_T*          pt_mkv_handler,
                        UINT32                      ui4_rate,
                        UINT32                      ui4_scale,
                        MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj)
{

    MKV_IDXBULD_CUE_POINT_ELE_T*      pt_cue_point_head;
    MIDXBULD_ELMT_STRM_ENTRY_T*       pt_idx_entry;
    MKV_IDXBULD_CUE_TRACK_POS_ELE_T*  pt_track_pos_head;
    MIDXBULD_ELMT_TBL_OBJ_T*          pt_elmt_tbl_obj;
    MIDXBULD_ELMT_STRM_ENTRY_T*       pt_idx_head;
    UINT32                            ui4_entry_num;
    UINT32                            ui4_tbl_num;
    UINT32                            ui4_loop_num;
    UINT64                            ui8_seg_tm_scale;
    
    ui8_seg_tm_scale = pt_mkv_handler->ui8_seg_tm_scale;
    pt_idx_head      = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    ui4_entry_num    = pt_tbl_obj->ui4_tbl_size;
    ui4_tbl_num      = 0;
    /*Parameter check*/
    if ((NULL == pt_cue_ele) || (NULL == pt_idx_head))
    {
        DBG_ERROR(("_mkv_midxbuld_get_key_by_stream_id:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }

    if (ui4_entry_num > pt_cue_ele->ui4_index_count)
    {
        DBG_ERROR(("_mkv_midxbuld_get_key_by_stream_id:INSUFFICIENT_BUFFER\r\n"));
        return MIDXBULDR_INSUFFICIENT_BUFFER;
    }

    if (0 == ui4_scale)
    {
        DBG_ERROR(("_mkv_midxbuld_get_key_by_stream_id:ui4_scale == 0\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    ui4_entry_num = 0;
    pt_elmt_tbl_obj = &pt_tbl_obj->t_elmt_tbl;
    pt_idx_entry = pt_idx_head;
    pt_cue_point_head = pt_cue_ele->pt_cue_point_head;
    ui4_loop_num = 0;
    
    if (pt_cue_point_head != NULL && pt_cue_point_head->pt_track_pos_head != NULL)
    {
        
        ui4_tbl_num = (UINT32)_mm_div64(pt_cue_point_head->pt_track_pos_head->ui8_cluster_pos,
                                        MKV_SIZE_4G,
                                        NULL);
        pt_elmt_tbl_obj->ui8_base_offset = MKV_SIZE_4G * ui4_tbl_num;
    }
    
    while (NULL != pt_cue_point_head)
    {
        /*search by id*/
        pt_track_pos_head = pt_cue_point_head->pt_track_pos_head;
        while (NULL != pt_track_pos_head)
        {
            if (ui2_stream_id == pt_track_pos_head->ui2_stream_id)
            {
                break;
            }
            /*get to next stream */
            pt_track_pos_head = pt_track_pos_head->pt_next;
        }

        /*find out a index point for given stream id*/
        if (NULL != pt_track_pos_head)
        {
            pt_idx_entry->ui4_pts 
                = (UINT32)_mm_div64((pt_cue_point_head->ui8_timecode
                                    * 9 * ui8_seg_tm_scale),
                                    (1000 * 100),
                                    NULL);                
            pt_idx_entry->ui4_size 
                    = (UINT32)(pt_track_pos_head->ui8_cluster_pos 
                    - MKV_SIZE_4G * ui4_tbl_num);
                /*
            pt_idx_entry->ui4_relative_offset 
                = (UINT32)(pt_track_pos_head->ui8_pos 
                    - MKV_SIZE_4G * ui4_tbl_num);
                */
            if (pt_track_pos_head->ui8_cluster_pos >= MKV_SIZE_4G * (ui4_tbl_num + 1))
            {
                ui4_tbl_num++;
                pt_elmt_tbl_obj->ui4_number_of_entry = ui4_entry_num;
                if (pt_elmt_tbl_obj->pt_next != NULL)
                {
                    pt_elmt_tbl_obj = pt_elmt_tbl_obj->pt_next;
                }
                else
                {
                    pt_elmt_tbl_obj->pt_next = (MIDXBULD_ELMT_TBL_OBJ_T*) 
                                        _mm_util_mem_alloc_with_tag(
                                        sizeof(MIDXBULD_ELMT_TBL_OBJ_T), 
                                        MM_MEM_MIDX_TAG);
                    if (pt_elmt_tbl_obj->pt_next == NULL)
                    {
                        return MIDXBULDR_OUT_OF_MEMORY;
                    }
                    x_memset(pt_elmt_tbl_obj->pt_next, 0, sizeof(MIDXBULD_ELMT_TBL_OBJ_T));
                    pt_elmt_tbl_obj = pt_elmt_tbl_obj->pt_next;
                }
                pt_elmt_tbl_obj->ui8_base_offset = MKV_SIZE_4G * ui4_tbl_num;
                pt_elmt_tbl_obj->u_fmt.t_es.pt_idx_entry = pt_idx_entry;
                ui4_entry_num = 0;
            }
            /*have gotten a index point*/
            ++pt_idx_entry;
            ++ui4_entry_num;
        }
        
        /*get to next index point*/
        if(pt_cue_ele->ui4_index_count == 0)
        {
            return MIDXBULDR_INV_ARG;
        }
        pt_mkv_handler->ui1_curr_buld_progress 
            += 40 * ui4_loop_num / pt_cue_ele->ui4_index_count;
        ui4_loop_num++;
        pt_cue_point_head = pt_cue_point_head->pt_next;
        
    }
    pt_elmt_tbl_obj->ui4_number_of_entry = ui4_entry_num;
   
    return MIDXBULDR_OK;
}

INT32 _mkv_midxbuld_indexing(MIDXBULD_OBJ_T*      pt_midxbuld_obj,
                                   MKV_IDXBULD_OBJ_T*   pt_mkv_handler)
{
    MKV_IDXBULD_RANGE_OBJ_T*         pt_range_obj;
    MKV_IDXBULD_ELMT_TBL_OBJ_T*      pt_tbl_obj;
    MIDXBULD_KEY_TBL_EXT_T*          pt_key_tbl_list; 
    MKV_IDXBULD_CUE_ELE_T*           pt_cue_ele;
    MIDXBULD_KEY_TBL_EXT_T*          pt_key_tbl_ext; /* (O) */
    MIDXBULD_KEY_INFO_T*             pt_key_info;
    MIDXBULD_ELMT_TBL_DESCRIB_T      t_describ;
    MIDXBULD_ELMT_TBL_OBJ_T*         pt_elmt_tbl_obj;
    
    INT32                            i4_ret; /*return code*/
    
    /*Check parameters*/
    if (NULL == pt_midxbuld_obj || NULL == pt_mkv_handler)
    {
        DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }

    pt_key_info     = &pt_mkv_handler->t_key_info;
    pt_cue_ele      = pt_mkv_handler->pt_cue_ele;
    pt_tbl_obj      = pt_mkv_handler->apt_tbl_obj_map[0];
    pt_range_obj    = pt_mkv_handler->pt_key_range_obj;
    pt_key_tbl_list = pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext;
    if (NULL == pt_cue_ele 
        || NULL == pt_tbl_obj 
        || NULL == pt_range_obj 
        || NULL == pt_key_tbl_list 
        )
    {
        DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc:pointer NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*init local variables*/
    x_memset((VOID *)&t_describ, 0, sizeof(MIDXBULD_ELMT_TBL_DESCRIB_T));

    /*2.  translate cue info to <pts,key block offset, key block size>*/
    do 
    {
        pt_range_obj->e_tbl_type                 = MIDXBULD_TBL_TYPE_ELMT_STRM;
        pt_range_obj->b_is_key_frame             = TRUE;
        pt_range_obj->b_index_is_valid           = FALSE;
        pt_range_obj->pt_tbl_obj                 = pt_tbl_obj;
        pt_range_obj->ui1_tbl_total              = 1;
        pt_range_obj->u_range.t_es.ui4_start_pts = 0;
        pt_range_obj->u_range.t_es.ui4_end_pts   = MIDXBULD_IDX_RANGE_FULL;
        
        /*stream descritption*/
        t_describ.u_info.t_es.e_es_type     = MIDXBULD_ELEM_STRM_TYPE_KEY;
        t_describ.u_info.t_es.ui4_strm_id   = pt_key_info->ui4_strm_id;
        t_describ.u_info.t_es.ui4_scale     = pt_key_info->ui4_scale;
        t_describ.u_info.t_es.ui4_rate      = pt_key_info->ui4_rate;
        t_describ.u_info.t_es.ui4_sample_sz = pt_key_info->ui4_sample_sz;
        t_describ.u_info.t_es.ui4_avg_bytes_per_sec 
                                            = pt_key_info->ui4_avg_bytes_per_sec;
        pt_elmt_tbl_obj                     = &pt_tbl_obj->t_elmt_tbl;
        while(pt_elmt_tbl_obj)
        {
            x_memcpy((VOID*)&(pt_elmt_tbl_obj->u_fmt.t_es.t_info), 
                     (VOID*)&(t_describ.u_info.t_es),
                     sizeof(MIDXBULD_ELMT_STRM_TBL_INFO_T));
            pt_elmt_tbl_obj->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
            pt_elmt_tbl_obj             = pt_elmt_tbl_obj->pt_next;
        }
        
        pt_mkv_handler->ui1_curr_buld_progress = 60;
        /*translate<pts, packet num, packet count> to <pts,offset,size> */
        i4_ret = _mkv_midxbuld_get_key_by_stream_id(pt_cue_ele,
                                                    (UINT16)pt_key_info->ui4_strm_id,
                                                    pt_mkv_handler,
                                                    pt_key_info->ui4_rate,
                                                    pt_key_info->ui4_scale,
                                                    pt_tbl_obj);
        if (MIDXBULDR_OK != i4_ret)
        {
            /*pt_idx_entry will be freed during pt_tbl_obj free */
            DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc: "
                       "get_key_by_stream_id error %d \r\n", i4_ret));
            _mkv_midxbuld_free_table_obj(pt_midxbuld_obj,pt_mkv_handler,
                                         pt_key_info->ui4_tbl_id);
            _mkv_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                         pt_mkv_handler,
                                         pt_key_info->ui4_range_id);
            if (NULL != pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext)
            {
                pt_key_tbl_ext = pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext;
                pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext = pt_key_tbl_ext->pt_next;
                _mm_util_mem_free_with_tag(pt_key_tbl_ext, 
                                           MM_MEM_MIDX_TAG);
            }
            pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext = NULL;
            pt_key_info->ui4_tbl_id    = MIDXBULD_NULL_ID;
            pt_key_info->ui4_range_id  = MIDXBULD_NULL_ID;
            i4_ret                     = MIDXBULDR_INTERNAL_ERR;
            break;
        }
        
        /*(6)set key table info*/
        pt_key_tbl_list->t_elmt_tbl  = pt_tbl_obj->t_elmt_tbl;
        pt_key_tbl_list->pt_next     = NULL;
        pt_mkv_handler->b_key_tbl_ok = TRUE;
    }while(0);


    /*free resource*/
    _mkv_midxbuld_free_cue_ele(pt_cue_ele); 
    pt_mkv_handler->pt_cue_ele = NULL;
    pt_mkv_handler->ui1_curr_buld_progress = 100;

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_key_frame_table_alloc
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
INT32 _mkv_midxbuld_key_frame_table_alloc( 
                                           MIDXBULD_KEY_INFO_T*    pt_key_info, 
                                           MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                                           MKV_IDXBULD_OBJ_T*      pt_mkv_handler,
                                           MIDXBULD_SOURCE_INFO_T* pt_src_info)
{
    MIDXBULD_ELMT_TBL_DESCRIB_T      t_describ;
    
    MKV_IDXBULD_RANGE_OBJ_T*         pt_range_obj       = NULL;
    MKV_IDXBULD_ELMT_TBL_OBJ_T*      pt_tbl_obj         = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T*      pt_idx_entry       = NULL;
    MIDXBULD_KEY_TBL_EXT_T*          pt_key_tbl_list    = NULL; 
    MKV_IDXBULD_CUE_ELE_T*           pt_cue_ele         = NULL;
    MIDXBULD_ELMT_TBL_OBJ_T*         pt_elmt_tbl_obj    = NULL;

    INT32                            i4_ret; /*return code*/
    UINT64                           ui8_cue_max_track_pos;
    UINT32                           ui4_elmt_tbl_num;
    
    /*Check parameters*/
    if ((NULL == pt_key_info) || (NULL == pt_midxbuld_obj)|| (NULL == pt_mkv_handler))
    {
        DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc:invalid argument!\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /*init local variables*/
    pt_tbl_obj = NULL;
    x_memset((VOID *)&t_describ, 0, sizeof(MIDXBULD_ELMT_TBL_DESCRIB_T));

    
    /****1. parse cues*****/
    pt_cue_ele = (MKV_IDXBULD_CUE_ELE_T*)
                 _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_CUE_ELE_T),
                                             MM_MEM_MIDX_TAG);
    if (pt_cue_ele == NULL)
    {
        DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc:pt_cue_ele NULL\r\n"));
        return MIDXBULDR_INTERNAL_ERR;
    }
    x_memset(pt_cue_ele, 0, sizeof(MKV_IDXBULD_CUE_ELE_T));

    i4_ret = _mkv_midxbuld_parse_cues(pt_midxbuld_obj, pt_cue_ele, pt_mkv_handler, pt_src_info);
    if(MIDXBULDR_OK != i4_ret)
    {
        _mkv_midxbuld_free_cue_ele(pt_cue_ele);        
        return MIDXBULDR_INTERNAL_ERR;
    }

    /*2.  translate cue info to <pts,key block offset, key block size>*/
    i4_ret = MIDXBULDR_OK;
    do 
    {
        /*(1)apply range object*/
        pt_range_obj = (MKV_IDXBULD_RANGE_OBJ_T*)
                        _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_RANGE_OBJ_T),
                        MM_MEM_MIDX_TAG);
        if (NULL == pt_range_obj)
        {
            DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc:failed to allocate "
                       "memory for pt_range_obj\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_range_obj, 0, sizeof(MKV_IDXBULD_RANGE_OBJ_T));
        
        /*(2)allocate id for pt_range_obj*/
        if (TRUE != _mkv_midxbuld_alloc_range_id(pt_midxbuld_obj, 
                                                 &pt_range_obj->ui4_range_id,
                                                 pt_mkv_handler))
        {
            DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc: fail to "
                       "alloc range id for pt_range_obj\r\n"));
            i4_ret = MIDXBULDR_OUT_OF_HANDLE;
            break;
        }
        
        /*(3)allocate elmt tbl obj memory */
        pt_tbl_obj = (MKV_IDXBULD_ELMT_TBL_OBJ_T*) 
                      _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_ELMT_TBL_OBJ_T),
                      MM_MEM_MIDX_TAG);
        if (NULL == pt_tbl_obj)
        {
            DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc: fail to "
                       "alloc memory for pt_range_obj\r\n"));
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_tbl_obj, 0, sizeof(MKV_IDXBULD_ELMT_TBL_OBJ_T));
        
        /*(4)allocate table id for pt_tbl_obj*/
        if (TRUE != _mkv_midxbuld_alloc_table_id(pt_range_obj, 
                                                 &pt_tbl_obj->ui4_tbl_id))
        {
            DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc: fail to "
                       "alloc table id for pt_tbl_obj\r\n"));
            i4_ret = MIDXBULDR_OUT_OF_HANDLE;
            break;
        }  

        /*(5)begin ---------set key frame table info--------------*/
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry =
            _mkv_midxbuld_get_key_num_by_stream_id(pt_cue_ele,
                                                  (UINT16)pt_key_info->ui4_strm_id,
                                                  &ui8_cue_max_track_pos);
        pt_tbl_obj->ui4_tbl_size = pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
        
        /*allocate element table obj*/
        ui4_elmt_tbl_num = (UINT32)(ui8_cue_max_track_pos >> 32);
        pt_elmt_tbl_obj = &pt_tbl_obj->t_elmt_tbl;
        while(ui4_elmt_tbl_num--)
        {
            pt_elmt_tbl_obj->pt_next = (MIDXBULD_ELMT_TBL_OBJ_T*) 
                                        _mm_util_mem_alloc_with_tag(
                                        sizeof(MIDXBULD_ELMT_TBL_OBJ_T), 
                                        MM_MEM_MIDX_TAG);
            if (NULL == pt_elmt_tbl_obj->pt_next)
            {
                i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
                break;
            }
            x_memset(pt_elmt_tbl_obj->pt_next, 0, sizeof(MIDXBULD_ELMT_TBL_OBJ_T));
            pt_elmt_tbl_obj = pt_elmt_tbl_obj->pt_next;
        }
        
        pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*)
                        _mm_util_mem_alloc_with_tag(
                        sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) 
                        * pt_tbl_obj->ui4_tbl_size,
                          MM_MEM_MIDX_TAG);
        if (NULL == pt_idx_entry)
        {
            DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc: memory"
                       "for pt_idx_entry\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        /*set allocated memoty 0*/
        x_memset(pt_idx_entry, 0, 
                 sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
        
        /*(6)set key table info*/
        pt_mkv_handler->t_key_table_obj.ui4_key_tbl_id = pt_tbl_obj->ui4_tbl_id;
        pt_key_tbl_list = (MIDXBULD_KEY_TBL_EXT_T*)
                          _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_KEY_TBL_EXT_T) ,
                          MM_MEM_MIDX_TAG);
        if (NULL == pt_key_tbl_list)
        {
            DBG_ERROR(("_mkv_midxbuld_key_frame_table_alloc: memory"
                       "for pt_key_tbl_list\r\n"));
            i4_ret =  MIDXBULDR_OUT_OF_MEMORY;
            break;
        }
        x_memset(pt_key_tbl_list, 0, sizeof(MIDXBULD_KEY_TBL_EXT_T));
        
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;     
        pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext = pt_key_tbl_list;
        pt_mkv_handler->apt_tbl_obj_map[0] = pt_tbl_obj;
        pt_mkv_handler->pt_curr_range_obj = pt_range_obj;
        pt_mkv_handler->pt_key_range_obj = pt_range_obj;        
    }while(0);
    
    if (i4_ret != MIDXBULDR_OK)
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
        if(pt_idx_entry)
        {
            _mm_util_mem_free_with_tag(pt_idx_entry, MM_MEM_MIDX_TAG);
            pt_idx_entry        = NULL;
        }
        if(pt_key_tbl_list)
        {
            _mm_util_mem_free_with_tag(pt_key_tbl_list, MM_MEM_MIDX_TAG);
            pt_key_tbl_list     = NULL;
        }
        pt_key_info->ui4_tbl_id                        = MIDXBULD_NULL_ID;
        pt_key_info->ui4_range_id                      = MIDXBULD_NULL_ID;
    }
    else
    {
        pt_key_info->ui4_tbl_id   = pt_tbl_obj->ui4_tbl_id;
        pt_key_info->ui4_range_id = pt_range_obj->ui4_range_id;
    }
    
    /*store cues info for indexing*/
    pt_mkv_handler->pt_cue_ele = pt_cue_ele;
    /*store key info for indexing*/
    pt_mkv_handler->t_key_info = *pt_key_info;
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _mkv_midxbuld_get_idx_elmt_tbl
 *
 * Description: This function get index info
 *
 * Inputs:  pt_midxbuld_obj, specifies mkv index builder object
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

INT32 _mkv_midxbuld_get_idx_elmt_tbl(MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                                     MKV_IDXBULD_OBJ_T*          pt_mkv_handler,
                                     MIDXBULD_ELMT_TBL_INFO_T* pt_elmt_tbl)
{
    MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    MKV_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    UINT8                       ui1_i;
    BOOL                        b_id_match = FALSE;
    
    /*check parameters*/
    if ((NULL == pt_midxbuld_obj) || (NULL == pt_elmt_tbl) || NULL == pt_mkv_handler)
    {
        DBG_ERROR(("_mkv_midxbuld_get_idx_elmt_tbl:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /*init local variables*/
    if (pt_mkv_handler->ui1_range_count > MKV_MIDXBULD_RANGE_MAX)
    {
        DBG_ERROR(("_mkv_midxbuld_get_idx_elmt_tbl:error count!!!\r\n"));
        return FALSE;
    }
    /*if it in key range?*/
    if (pt_mkv_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id 
        == pt_elmt_tbl->ui4_tbl_id)
    {
        b_id_match = TRUE;
        pt_tbl_obj = pt_mkv_handler->pt_key_range_obj->pt_tbl_obj;
    }
    /*non key range array*/
    else if (pt_mkv_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_mkv_handler->ui1_range_count; ui1_i++)
        {
            pt_obj = pt_mkv_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
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
        DBG_INFO(("_mkv_midxbuld_get_idx_elmt_tbl: not found\r\n"));
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
    
    DBG_INFO(("_mkv_midxbuld_get_idx_elmt_tbl: found\r\n"));
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_midxbuld_fopen
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
INT32 mkv_midxbuld_fopen(
                          CHAR*                ps_path_name, 
                          MIDXBULD_KEY_INFO_T* pt_key_info, 
                          VOID*                pv_obj )
{

#ifndef __NO_FM__

    INT32               i4_ret;
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*  pt_mkv_handler;
    FM_FILE_INFO_T      t_fm_info;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;

    MIDXBULD_SOURCE_INFO_T*     pt_src_info;
    MKV_IDXBULD_OBJ_T*          pt_idx_obj;
    HANDLE_T                    h_temp;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == ps_path_name) || (NULL == pt_key_info))
    {
        DBG_ERROR(("mkv_midxbuld_fopen:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*init local variables*/
    pt_mkv_handler = NULL;
    i4_ret = MIDXBULDR_OK;
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;
    h_temp = pt_midxbuld_obj->h_midxbuld;
    /*set input functions*/
    pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    /*mkv handler for index builder*/
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
    pt_src_info = &pt_midxbuld_obj->t_src_info;
    while(pt_src_info)
    {
        pt_idx_obj = (MKV_IDXBULD_OBJ_T*)
                         _mm_util_mem_alloc_with_tag(sizeof(MKV_IDXBULD_OBJ_T),
                                                     MM_MEM_MIDX_TAG);
        if ( NULL == pt_idx_obj )
        {
            DBG_ERROR(("mkv_midxbuld_fopen:pt_mkv_handler is NULL\r\n"));
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            goto EXIT_FOPEN;
        }
        x_memset(pt_idx_obj, 0, sizeof(MKV_IDXBULD_OBJ_T));
        if(pt_src_info == &pt_midxbuld_obj->t_src_info)
        {
            /*set mkv handler*/
            pt_midxbuld_obj->pv_handler_obj = pt_idx_obj;
            /*allocate parser buffer*/
            pt_idx_obj->pui1_header_buf = (UINT8*)_mm_util_mem_alloc_with_tag(
                                               MKV_IDXBULD_INIT_PARSER_BUFF_LEN,
                                               MM_MEM_MIDX_TAG);
            if (NULL == pt_idx_obj->pui1_header_buf)
            {
                DBG_ERROR(("mkv_midxbuld_fopen:pui1_header_buf is NULL\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                goto EXIT_FOPEN;
            }
            x_memset(pt_idx_obj->pui1_header_buf, 0, MKV_IDXBULD_INIT_PARSER_BUFF_LEN);
            pt_idx_obj->pui1_temp_buf = (UINT8*)_mm_util_mem_alloc_with_tag(
                                               MKV_IDXBULD_TEMP_PARSER_BUFF_LEN,
                                               MM_MEM_MIDX_TAG);
            if (NULL == pt_idx_obj->pui1_temp_buf)
            {
                DBG_ERROR(("mkv_midxbuld_fopen:pui1_temp_buf is NULL\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                goto EXIT_FOPEN;
            }
            x_memset(pt_idx_obj->pui1_temp_buf, 0, MKV_IDXBULD_TEMP_PARSER_BUFF_LEN);
            pt_idx_obj->ui4_buf_length = MKV_IDXBULD_INIT_PARSER_BUFF_LEN;
            pt_idx_obj->i8_buf_read_pos = 0;
            pt_idx_obj->ui4_buf_write_pos = 0;
            pt_idx_obj->ui8_buf_file_pos = 0;
            
            i4_ret = x_fm_open(FM_ROOT_HANDLE, 
                               ps_path_name,
                               FM_READ_ONLY, 
                               0777, FALSE, 
                               &pt_midxbuld_obj->h_file);
            if (i4_ret != FMR_OK)
            {
                DBG_ERROR(("mkv_midxbuld_fopen:x_fm_open return %d\r\n", i4_ret));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                goto EXIT_FOPEN;
            }
            /*get file size*/
            pf_set_pos(pt_midxbuld_obj , 0, FM_SEEK_END);
            pf_get_pos(pt_midxbuld_obj, &(pt_idx_obj->ui8_file_size));
            pf_set_pos(pt_midxbuld_obj , 0, FM_SEEK_BGN);
            
            x_fm_get_info_by_handle(pt_midxbuld_obj->h_file, &t_fm_info);
            pt_midxbuld_obj->z_buff_leng = t_fm_info.ui8_size;
        }
        else
        {
            if(!pt_mkv_handler)
            {
                i4_ret = MIDXBULDR_INV_ARG;
                goto EXIT_FOPEN;
            }
            pt_mkv_handler->pt_next = pt_idx_obj;
            pt_idx_obj->pui1_temp_buf = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->pui1_temp_buf;
            pt_idx_obj->pui1_header_buf = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->pui1_header_buf;
            pt_idx_obj->ui4_buf_length = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui4_buf_length;
            pt_idx_obj->i8_buf_read_pos = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->i8_buf_read_pos;
            pt_idx_obj->ui4_buf_write_pos = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui4_buf_write_pos;
            pt_idx_obj->ui8_buf_file_pos = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui8_buf_file_pos;
            pt_idx_obj->ui8_file_size = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui8_file_size;
        }
        pt_idx_obj->ui1_idx_range_id_last = 1;
        pt_idx_obj->ui8_seg_data_offset = 
                        pt_src_info->ui8_seg_data_offset;
        pt_idx_obj->ui8_seg_tm_scale = 
                        pt_src_info->ui8_seg_tm_scale;
        /*build key frame table,generally for video*/
        i4_ret = _mkv_midxbuld_key_frame_table_alloc(pt_key_info, 
                                                     (MIDXBULD_OBJ_T*)pv_obj,
                                                     pt_idx_obj, pt_src_info);
        if (i4_ret != MIDXBULDR_OK)
        {
            goto EXIT_FOPEN;
        }
        
        pt_mkv_handler = pt_idx_obj;
#if SUPPORT_MULTI_INFO
        pt_src_info = pt_src_info->pt_next;
        pt_key_info = pt_key_info->pt_next;
        pt_midxbuld_obj->h_midxbuld++;
#else
        pt_src_info = NULL;
#endif
    }
    pt_midxbuld_obj->h_midxbuld = h_temp;
    ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui1_curr_buld_progress = 50;
    
EXIT_FOPEN:
    if(MIDXBULDR_OK != i4_ret)
    {
        pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        if(pt_mkv_handler && pt_mkv_handler->pui1_header_buf)
        {
            _mm_util_mem_free_with_tag(pt_mkv_handler->pui1_header_buf,
                                       MM_MEM_MIDX_TAG);
            pt_mkv_handler->pui1_header_buf = NULL;
        }
        if(pt_mkv_handler && pt_mkv_handler->pui1_temp_buf)
        {
            _mm_util_mem_free_with_tag(pt_mkv_handler->pui1_temp_buf,
                                       MM_MEM_MIDX_TAG);
            pt_mkv_handler->pui1_temp_buf = NULL;
        }
        while(pt_mkv_handler)
        {
            pt_midxbuld_obj->pv_handler_obj = pt_mkv_handler->pt_next;
            _mm_util_mem_free_with_tag(pt_mkv_handler, MM_MEM_MIDX_TAG);
            pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        }
        pt_midxbuld_obj->pv_handler_obj = NULL;
        
        /*close file*/
        if (pt_midxbuld_obj->h_file)
        {
            x_fm_close(pt_midxbuld_obj->h_file);
            pt_midxbuld_obj->h_file = NULL_HANDLE;
        }
    }
    else
    {
        ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui1_curr_buld_progress = 100;
    }
    return i4_ret;

#else

    return MIDXBULDR_INITIAL_ERR;

#endif
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_midxbuld_mopen
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
INT32 mkv_midxbuld_mopen(
                         VOID*                pv_mem_data,
                         SIZE_T               z_mem_leng,
                         MIDXBULD_KEY_INFO_T* pt_key_info,
                         VOID*                pv_obj)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*  pt_mkv_handler;
    MEDIA_FEEDER_T*     pt_src_feeder;
    INT32               i4_ret;
    UINT64              ui8_clip_pos;
    
    MIDXBULD_SOURCE_INFO_T*     pt_src_info;
    MKV_IDXBULD_OBJ_T*          pt_idx_obj;
    HANDLE_T                    h_temp;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_mem_data) || (NULL == pv_obj))
    {
        DBG_ERROR(("mkv_midxbuld_mopen:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_mkv_handler = NULL;
    i4_ret = MIDXBULDR_OK;
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;
    h_temp = pt_midxbuld_obj->h_midxbuld;
    if (z_mem_leng == 0)
    {
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
        /*To get clip size*/
        i4_ret = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos(pt_src_feeder, 
                                                             0, 
                                                             MEDIA_SEEK_END);
        if (MIDXBULDR_OK != i4_ret)
        {
            DBG_ERROR(("mkv_midxbuld_mopen:set error\r\n"));
            return MIDXBULDR_INITIAL_ERR;
        }
        /*get the end position*/
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos(pt_src_feeder, &ui8_clip_pos);
        pt_midxbuld_obj->z_buff_leng = ui8_clip_pos;

        
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

    pt_src_info = &pt_midxbuld_obj->t_src_info;
    while(pt_src_info)
    {
        /*allocate memory for idex builder handler*/
        pt_idx_obj = (MKV_IDXBULD_OBJ_T*)_mm_util_mem_alloc_with_tag(
                                              sizeof(MKV_IDXBULD_OBJ_T),
                                              MM_MEM_MIDX_TAG);
        if (NULL == pt_idx_obj)
        {
            DBG_ERROR(("mkv_midxbuld_mopen:pt_mkv_handler NULL\r\n"));
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            goto EXIT_MOPEN;
        }
        x_memset(pt_idx_obj, 0, sizeof(MKV_IDXBULD_OBJ_T));
        if(pt_src_info == &pt_midxbuld_obj->t_src_info)
        {
            pt_midxbuld_obj->pv_handler_obj = pt_idx_obj;
            /*get file size*/
            pt_idx_obj->ui8_file_size = (UINT64)pt_midxbuld_obj->z_buff_leng;
            
            /*allocate init memory for index builder*/
            pt_idx_obj->pui1_header_buf = (UINT8*) _mm_util_mem_alloc_with_tag(
                                               MKV_IDXBULD_INIT_PARSER_BUFF_LEN,
                                               MM_MEM_MIDX_TAG);
            if (NULL == pt_idx_obj->pui1_header_buf)
            {
                DBG_ERROR(("mkv_midxbuld_mopen:pui1_header_buf NULL\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                goto EXIT_MOPEN;
            }
            x_memset(pt_idx_obj->pui1_header_buf, 0, MKV_IDXBULD_INIT_PARSER_BUFF_LEN);
            pt_idx_obj->pui1_temp_buf = (UINT8*)_mm_util_mem_alloc_with_tag(
                                               MKV_IDXBULD_TEMP_PARSER_BUFF_LEN,
                                               MM_MEM_MIDX_TAG);
            if (NULL == pt_idx_obj->pui1_temp_buf)
            {
                DBG_ERROR(("mkv_midxbuld_fopen:pui1_temp_buf is NULL\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                goto EXIT_MOPEN;
            }
            x_memset(pt_idx_obj->pui1_temp_buf, 0, MKV_IDXBULD_TEMP_PARSER_BUFF_LEN);
            pt_idx_obj->ui4_buf_length = MKV_IDXBULD_INIT_PARSER_BUFF_LEN;
            pt_idx_obj->i8_buf_read_pos = 0;
            pt_idx_obj->ui4_buf_write_pos = 0;
            pt_idx_obj->ui8_buf_file_pos = 0;
        }
        else
        {
            if(!pt_mkv_handler)
            {
                i4_ret = MIDXBULDR_INV_ARG;
                goto EXIT_MOPEN;
            }
            pt_mkv_handler->pt_next = pt_idx_obj;
            pt_idx_obj->pui1_temp_buf = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->pui1_temp_buf;
            pt_idx_obj->pui1_header_buf = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->pui1_header_buf;
            pt_idx_obj->ui4_buf_length = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui4_buf_length;
            pt_idx_obj->i8_buf_read_pos = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->i8_buf_read_pos;
            pt_idx_obj->ui4_buf_write_pos = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui4_buf_write_pos;
            pt_idx_obj->ui8_buf_file_pos = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui8_buf_file_pos;
            pt_idx_obj->ui8_file_size = 
                ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui8_file_size;
        }
        pt_idx_obj->ui1_idx_range_id_last = 1;
        pt_idx_obj->ui8_seg_data_offset = 
                        pt_src_info->ui8_seg_data_offset;
        pt_idx_obj->ui8_seg_tm_scale =
                        pt_src_info->ui8_seg_tm_scale;
        
        i4_ret = _mkv_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj, 
                                                     pt_idx_obj, pt_src_info);
        if (MIDXBULDR_OK != i4_ret)
        {
            goto EXIT_MOPEN;
        }
        
        pt_mkv_handler = pt_idx_obj;
#if SUPPORT_MULTI_INFO
        pt_src_info = pt_src_info->pt_next;
        pt_key_info = pt_key_info->pt_next;
        pt_midxbuld_obj->h_midxbuld++;
#else
        pt_src_info = NULL;
#endif
    }
    pt_midxbuld_obj->h_midxbuld = h_temp;
EXIT_MOPEN:
    if(MIDXBULDR_OK != i4_ret)
    {
        pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        if(pt_mkv_handler && pt_mkv_handler->pui1_header_buf)
        {
            _mm_util_mem_free_with_tag(pt_mkv_handler->pui1_header_buf,
                                       MM_MEM_MIDX_TAG);
            pt_mkv_handler->pui1_header_buf = NULL;
        }
        if(pt_mkv_handler && pt_mkv_handler->pui1_temp_buf)
        {
            _mm_util_mem_free_with_tag(pt_mkv_handler->pui1_temp_buf,
                                       MM_MEM_MIDX_TAG);
            pt_mkv_handler->pui1_temp_buf = NULL;
        }
        while(pt_mkv_handler)
        {
            pt_midxbuld_obj->pv_handler_obj = pt_mkv_handler->pt_next;
            _mm_util_mem_free_with_tag(pt_mkv_handler, MM_MEM_MIDX_TAG);
            pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        }
        pt_midxbuld_obj->pv_handler_obj = NULL;
    }
    else
    {
        ((MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj)->ui1_curr_buld_progress = 100;
    }
    return i4_ret;
}    

/*-----------------------------------------------------------------------------
 * Name:  mkv_midxbuld_close
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
INT32 mkv_midxbuld_close(VOID* pv_obj)
{
    MIDXBULD_OBJ_T*          pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*       pt_mkv_handler;
    MIDXBULD_KEY_TBL_EXT_T*  pt_key_tbl_ext; 
#if SUPPORT_MULTI_INFO
    MIDXBULD_SOURCE_INFO_T*  pt_src_info;
#endif
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (NULL == pv_obj)  
    {
        DBG_ERROR(("mkv_midxbuld_close NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*get index object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*get mkv inde handler*/
    pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_mkv_handler)
    {
        DBG_ERROR(("mkv_midxbuld_close pt_mkv_handler NULL\r\n"));
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
    /*free temporary buffer*/
    if (pt_mkv_handler->pui1_header_buf)
    {
        _mm_util_mem_free_with_tag(pt_mkv_handler->pui1_header_buf,
                                   MM_MEM_MIDX_TAG);
        pt_mkv_handler->pui1_header_buf = NULL;
    }
    if (pt_mkv_handler->pui1_temp_buf)
    {
        _mm_util_mem_free_with_tag(pt_mkv_handler->pui1_temp_buf,
                                   MM_MEM_MIDX_TAG);
        pt_mkv_handler->pui1_temp_buf = NULL;
    }

    while(pt_mkv_handler)
    {
        /* free key table info */
        while (NULL != pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext)
        {
            pt_key_tbl_ext = pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext;
            pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext = pt_key_tbl_ext->pt_next;
            _mm_util_mem_free_with_tag(pt_key_tbl_ext, 
                                       MM_MEM_MIDX_TAG);
        }
        pt_mkv_handler->t_key_table_obj.pt_key_tbl_ext = NULL;
        
        /* free all range objects */

        while (pt_mkv_handler->ui1_range_count > 0)
        {
            _mkv_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                         pt_mkv_handler,
                                         pt_mkv_handler->apt_range_obj[0]->ui4_range_id);
        }
        
        if (pt_mkv_handler->pt_key_range_obj)
        {
            _mkv_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                         pt_mkv_handler,
                                         pt_mkv_handler->pt_key_range_obj->ui4_range_id);
        }
        
        /*free handler object*/
        pt_midxbuld_obj->pv_handler_obj = pt_mkv_handler->pt_next;
        _mm_util_mem_free_with_tag(pt_mkv_handler, 
                                   MM_MEM_MIDX_TAG);
        pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    }
    pt_midxbuld_obj->pv_handler_obj = NULL;

    /*free src info*/
#if SUPPORT_MULTI_INFO
    pt_src_info = pt_midxbuld_obj->t_src_info.pt_next;
    while(pt_src_info)
    {
        pt_midxbuld_obj->t_src_info.pt_next = pt_src_info->pt_next;
        x_mem_free(pt_src_info);
        pt_src_info = pt_midxbuld_obj->t_src_info.pt_next;
    }
    pt_midxbuld_obj->t_src_info.pt_next = NULL;
#endif
    
    return MIDXBULDR_OK;
}    




/*-----------------------------------------------------------------------------
 * Name:  mkv_midxbuld_start_indexing
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
INT32 mkv_midxbuld_start_indexing(VOID* pv_obj, UINT32* pui4_range_id)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*          pt_mkv_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pui4_range_id))
    {
        DBG_ERROR(("mkv_midxbuld_start_indexing: NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;
    pt_mkv_handler = pt_midxbuld_obj->pv_handler_obj;
    /*build index array*/
    while(pt_mkv_handler)
    {
        _mkv_midxbuld_indexing(pv_obj, pt_mkv_handler);
        pt_mkv_handler = pt_mkv_handler->pt_next;
    }
    return MIDXBULDR_OK;
} 

/*-----------------------------------------------------------------------------
 * Name:  mkv_midxbuld_get_info
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
INT32 mkv_midxbuld_get_info(
                            VOID*                    pv_obj,   
                            MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                            VOID*                    pv_buf,     
                            SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*  pt_mkv_handler;
    MIDXBULD_KEY_TBL_INFO_T* pt_key_table_obj;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pz_buf_leng))
    {
        DBG_ERROR(("mkv_midxbuld_get_info:NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* Do                                                                   */
    /************************************************************************/
    /*get index builder object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*get mkv index builder handler*/
    pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    pt_key_table_obj = (MIDXBULD_KEY_TBL_INFO_T*)pv_buf;
    if(e_info_type != MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS)
    {
        while(pt_mkv_handler && pt_key_table_obj)
        {
           if(pt_mkv_handler->t_key_table_obj.ui4_key_tbl_id == pt_key_table_obj->ui4_key_tbl_id)
           {
               break;
           }
           pt_mkv_handler = pt_mkv_handler->pt_next;
        }
    }
    if (NULL == pt_mkv_handler) 
    {
        DBG_ERROR(("mkv_midxbuld_get_info:pt_mkv_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    switch(e_info_type)
    {
        case MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL:
            if (*pz_buf_leng < sizeof(MIDXBULD_ELMT_TBL_INFO_T))
            {
                DBG_ERROR(("mkv_midxbuld_get_info:less length(%ld)\r\n",
                           *pz_buf_leng));
                return MIDXBULDR_INITIAL_ERR;
            }
            *pz_buf_leng = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
            return _mkv_midxbuld_get_idx_elmt_tbl(pt_midxbuld_obj,pt_mkv_handler,
                         (MIDXBULD_ELMT_TBL_INFO_T*)pv_buf);
        case MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST:
            if (FALSE != pt_mkv_handler->b_key_tbl_ok)
            {
                *pz_buf_leng = sizeof(MIDXBULD_KEY_TBL_INFO_T);
                x_memcpy((VOID*)pv_buf, 
                         (VOID*)&(pt_mkv_handler->t_key_table_obj),
                         *pz_buf_leng);
                return MIDXBULDR_OK;   
            }
            else
            {
                return MIDXBULDR_KEY_TBL_NOT_READY;
            }

        case MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS:
            *(UINT8*)pv_buf = pt_mkv_handler->ui1_curr_buld_progress;
            *pz_buf_leng = 1;
            return MIDXBULDR_OK;
        default:
             return MIDXBULDR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  mkv_midxbuld_set_info
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
INT32 mkv_midxbuld_set_info(
                            VOID*                    pv_obj,   
                            MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                            VOID*                    pv_buf,     
                            SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    MKV_IDXBULD_OBJ_T*  pt_mkv_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pz_buf_leng))
    {
        DBG_ERROR(("mkv_midxbuld_set_info:NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*get index builder object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    
    /*get mkv index builder handler*/
    pt_mkv_handler = (MKV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_mkv_handler) 
    {
        DBG_ERROR(("mkv_midxbuld_set_info:pt_mkv_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    switch (e_info_type)
    {
        case MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE:
            /*The first stage do not come here,only support simple index object*/
            return MIDXBULDR_OK;

        case MIDXBULD_SET_INFO_TYPE_FREE_RANGE:
            return MIDXBULDR_OK;

        default:
            return MIDXBULDR_INV_ARG;
    }
    
}

#ifdef __cplusplus
}
#endif
