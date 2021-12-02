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
 * $RCSfile: asf_midxbuld.c,v $
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
#include "mutil/midxbuld/asf_idxbuld/asf_midxbuld.h"
#include "dbg/x_dbg.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"
#include "util/x_sort_search.h"

#include "mutil/minfo/asf_info/asf_parser_common.h"
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
#include "asf_midxbuld.h"
#include "x_dbg.h"

#include "../../mm_util.h"


#include "x_lnk_list.h"
#include "x_sort_search.h"
#include "../../minfo/asf_info/asf_parser_common.h"
#include "../../mm_util_input.h"
#include "../../mm_util_memory.h"

#endif
#define ASSERT(x)  DBG_ASSERT( x, DBG_MOD_MSVCTX + 100 )
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*define asf parsing buffer length*/
#define ASF_IDXBULD_INIT_PARSER_BUFF_LEN       500
#define ASF_OBJ_PKG_HDR_SIZE                   15

#ifndef __KERNEL__  
#define ASF_IDXBULD_ENTRY_BIG                  (1)
#define ASF_IDXBULD_ENTRY_EQUAL                (0)
#define ASF_IDXBULD_ENTRY_SMALL                (-1)
#endif
    
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__  
static INT32 _asf_midxbuld_entry_compare( const VOID*  pv_elem_a,
                                          const VOID*  pv_elem_b,
                                          VOID*        pv_opt_param);
#endif
static BOOL _asf_midxbuld_alloc_range_id ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32*         pui4_range_id );
static BOOL _asf_midxbuld_free_range_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32          ui4_range_id );

static BOOL _asf_midxbuld_alloc_table_id ( ASF_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                                           UINT32*                  pui4_tbl_id );
static BOOL _asf_midxbuld_free_table_obj ( MIDXBULD_OBJ_T*  pt_midxbuld_obj,
                                           UINT32           ui4_tbl_id );
static VOID _asf_midxbuld_free_hdlr_info ( MIDXBULD_OBJ_T* pt_midxbuld_obj );
#ifdef CLI_LVL_ALL
static VOID _asf_midxbuld_dump_hdlr_info ( MIDXBULD_OBJ_T* pt_midxbuld_obj );
#endif
static VOID _asf_midxbuld_free_simple_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_SIMPLE_INDEX_OBJECT_T* pt_simple_index_object );
static INT32 _asf_midxbuld_load_simple_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_SIMPLE_INDEX_OBJECT_T* pt_simple_index_object );

static INT32  _asf_midxbuld_gen_key_frame_by_simple_index_object( 
                           MIDXBULD_KEY_INFO_T*        pt_key_info, 
                           MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                           ASF_SIMPLE_INDEX_OBJECT_T*  pt_simple_index_object );

static VOID _asf_midxbuld_free_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_INDEX_OBJECT_T*        pt_index_object );
static INT32 _asf_midxbuld_load_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_INDEX_OBJECT_T*        pt_index_object );
static INT32  _asf_midxbuld_index_object_indexing( 
                                           MIDXBULD_KEY_INFO_T* pt_key_info, 
                                           MIDXBULD_OBJ_T*      pt_midxbuld_obj );
static INT32  _asf_midxbuld_simple_index_object_indexing( 
                                           MIDXBULD_KEY_INFO_T* pt_key_info, 
                                           MIDXBULD_OBJ_T*      pt_midxbuld_obj );
static INT32  _asf_midxbuld_wma_indexing( 
                                                 MIDXBULD_KEY_INFO_T* pt_key_info, 
                                                 MIDXBULD_OBJ_T*      pt_midxbuld_obj );
static INT32 _asf_midxbuld_indexing( MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                             UINT32*         pui4_range_id );

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
static INT32 _asf_midxbuld_get_start_info(MIDXBULD_OBJ_T*                  pt_midxbuld_obj,
                                         MIDXBULD_ELMT_TBL_TYPE_T         e_tbl_type,
                                         VOID*                            pt_start_info,
                                         SIZE_T                           z_info_size);
#endif

static INT32 _asf_midxbuld_get_idx_elmt_tbl( 
                                      MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                                      MIDXBULD_ELMT_TBL_INFO_T* pt_elmt_tbl );

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_entry_compare
 *
 * Description: Compare the entry by packet number. This compare is to register for qsort.
 *
 * Inputs:  pt_a_entry : the raw entry A
 *          pt_b_entry : the raw entry B
 *       
 * Outputs: NULL
 *
 * Returns: ASF_IDXBULD_ENTRY_BIG: a is bigger than b.
 *          ASF_IDXBULD_ENTRY_SMALL: b is bigger than a.
 *          ASF_IDXBULD_ENTRY_EQUAL: a equal with b.
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__  
INT32 _asf_midxbuld_entry_compare(const VOID*  pv_elem_a,
                                  const VOID*  pv_elem_b,
                                  VOID*        pv_opt_param)
{
    ASF_SIMP_IDX_ENTRY_T* pt_a_entry = (ASF_SIMP_IDX_ENTRY_T*)pv_elem_a;
    ASF_SIMP_IDX_ENTRY_T* pt_b_entry = (ASF_SIMP_IDX_ENTRY_T*)pv_elem_b;
    
    if (pt_a_entry != NULL && pt_b_entry != NULL)
    {
        return (pt_a_entry->ui4_packet_number >= pt_b_entry->ui4_packet_number)?ASF_IDXBULD_ENTRY_BIG : ASF_IDXBULD_ENTRY_SMALL;
    }
    else if (pt_a_entry == NULL && pt_b_entry != NULL)
    {
        return ASF_IDXBULD_ENTRY_BIG;
    }
    else if (pt_a_entry != NULL && pt_b_entry == NULL)
    {
        return ASF_IDXBULD_ENTRY_SMALL;
    }
    else 
    {
        return ASF_IDXBULD_ENTRY_EQUAL;
    }
    
}
#endif
/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_alloc_range_id
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
static BOOL _asf_midxbuld_alloc_range_id ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32*         pui4_range_id )
{
    ASF_IDXBULD_OBJ_T* pt_asf_handler;
    UINT8              ui1_key_range_id;
    UINT8              ui1_range_id;
    UINT8                    ui1_idx_range_id_last;
    UINT8                    ui1_i;
    BOOL                     b_occupied = TRUE;

    /*Parameters check*/
    if ((NULL == pt_midxbuld_obj) || (NULL == pui4_range_id))
    {
        DBG_ERROR( ( "%s#Line%d: parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return FALSE;
    }

    /*get asf index builder*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler)
    { 
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler NULL\r\n", __FUNCTION__, __LINE__ ) );
        return FALSE;
    }

    *pui4_range_id = MIDXBULD_NULL_ID;

    if ( pt_asf_handler->pt_key_range_obj )
    {
        ui1_key_range_id = (UINT8)((pt_asf_handler->pt_key_range_obj->ui4_range_id 
                                        & ASF_MIDXBULD_RANGE_ID_MASK)
                                      >> ASF_MIDXBULD_RANGE_ID_SHFIT );
    }
    /* key range should be allocated first */
    else if(pt_asf_handler->ui1_range_count > 0)
    {
        DBG_ERROR( ( "%s#Line%d:key range should be allocated first count=%u\r\n",
                     __FUNCTION__, __LINE__ , pt_asf_handler->ui1_range_count ) );
        return FALSE;
    }
    else
    {
        ui1_key_range_id = MIDXBULD_NULL_ID;
    }

    ui1_idx_range_id_last = pt_asf_handler->ui1_idx_range_id_last;
    /*key range should be allocated firstsly*/
    if ( ui1_key_range_id == MIDXBULD_NULL_ID )
    {
        if ( ( ui1_idx_range_id_last == 0xff ) 
                || ( ui1_idx_range_id_last == MIDXBULD_NULL_ID ) )
        {
            ui1_idx_range_id_last = ASF_MIDXBULD_START_RANGE_ID;
        }
        *pui4_range_id = ( UINT32 )ui1_idx_range_id_last;  
    }
    else
    {
        /*the following will allocate non-key range id, so  ui1_key_range_id  is valid */
    if (pt_asf_handler->ui1_range_count < ASF_MIDXBULD_RANGE_MAX)
    {
            /*get a exclusive range ID*/
        while (b_occupied)
        {
            b_occupied = FALSE;
                for ( ui1_i = 0; ui1_i < pt_asf_handler->ui1_range_count; ++ui1_i )
            {
                ui1_range_id = (UINT8)((pt_asf_handler->apt_range_obj[ui1_i]->ui4_range_id 
                                            & ASF_MIDXBULD_RANGE_ID_MASK)
                                             >> ASF_MIDXBULD_RANGE_ID_SHFIT );
                    if ( ( ui1_range_id == ui1_idx_range_id_last ) 
                         || ( ui1_key_range_id == ui1_idx_range_id_last ) )
                {
                    b_occupied = TRUE;   
                    break;
                }
            }

                /*try next range id*/
                if ( b_occupied )
                {
                    ui1_idx_range_id_last = ( UINT8 )( ( ui1_idx_range_id_last == 0xff )
                          ? ASF_MIDXBULD_START_RANGE_ID : ( ui1_idx_range_id_last + 1 ) );
                }
        }
    
        *pui4_range_id = (UINT32)ui1_idx_range_id_last;
    
        }
        else
        {
            DBG_ERROR( ( "%s#Line%d: allocate range fail, too allocate range count(%u)\r\n",
                       __FUNCTION__, __LINE__, pt_asf_handler->ui1_range_count ) );
            return FALSE;
        }
    }
        
   ++ui1_idx_range_id_last;
    if ( ( ui1_idx_range_id_last == 0xff ) 
            || ( ui1_idx_range_id_last == MIDXBULD_NULL_ID ) )
    {
        ui1_idx_range_id_last = ASF_MIDXBULD_START_RANGE_ID;
    }
    pt_asf_handler->ui1_idx_range_id_last = ui1_idx_range_id_last;
    
    /*index builder  handle + range id + table id*/
    *pui4_range_id = ( ( pt_midxbuld_obj->h_midxbuld & 0x0000ffff ) << ASF_MIDXBULD_HANDLE_ID_SHFIT )
                      + ( *pui4_range_id << ASF_MIDXBULD_RANGE_ID_SHFIT );
    DBG_INFO( ( "%s#Line%d:range id=%u\r\n", __FUNCTION__, __LINE__, *pui4_range_id ) );
    return TRUE;

}

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_free_range_obj
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
static BOOL _asf_midxbuld_free_range_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32          ui4_range_id)
{
    ASF_IDXBULD_OBJ_T*          pt_asf_handler;
    ASF_IDXBULD_RANGE_OBJ_T*    pt_range_obj;
    UINT8                       ui1_i = 0;
    BOOL                        b_id_match = FALSE;
    ASF_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    UINT32                      ui4_tbl_id;
    BOOL                        b_non_key = TRUE;

    /*Parameters check*/
    if (NULL == pt_midxbuld_obj)
    {
        DBG_ERROR( ( "%s#Line%d: is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return FALSE;
    }
    
    /*init NULL*/
    pt_range_obj = NULL;
    
    /*get asf index builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler)
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return FALSE;
    }
    /*If it is key range?*/
    if (pt_asf_handler->pt_key_range_obj->ui4_range_id == ui4_range_id)
    {
        b_id_match = TRUE;
        b_non_key = FALSE;
        pt_range_obj = pt_asf_handler->pt_key_range_obj;
    }
    /*non key range*/
    else if ((pt_asf_handler->ui1_range_count > 0)
              && (pt_asf_handler->ui1_range_count < ASF_MIDXBULD_RANGE_MAX))
    {
        for (ui1_i = 0; ui1_i < pt_asf_handler->ui1_range_count; ++ui1_i)
        {
            if (pt_asf_handler->apt_range_obj[ui1_i]->ui4_range_id == ui4_range_id)
            {
                b_id_match = TRUE;
                b_non_key = TRUE;
                pt_range_obj = pt_asf_handler->apt_range_obj[ui1_i];
                break;
            }
        }
    }
    else
    {
        DBG_INFO( ( "%s#Line%d:did not find range id = 0x%08x\r\n",
                    __FUNCTION__, __LINE__, ui4_range_id));
        return FALSE;
    }
    
    if (FALSE == b_id_match)
    {
        DBG_INFO( ( "%s#Line%d:not find range id = 0x%08x\r\n",
                    __FUNCTION__, __LINE__, ui4_range_id ) );
        return FALSE;
    }
    
    /*the given range exists go on...*/
    if (pt_range_obj)
    {
        while (pt_range_obj->pt_tbl_obj != NULL)
        {
            pt_tbl_obj = pt_range_obj->pt_tbl_obj;
            ui4_tbl_id = pt_tbl_obj->ui4_tbl_id;
             (VOID)_asf_midxbuld_free_table_obj( pt_midxbuld_obj, ui4_tbl_id );
        }

        /*free the given range*/
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);

        /*merge table array  */
        if (TRUE == b_non_key)
        {
            if (pt_asf_handler->pt_key_range_obj->ui4_range_id != ui4_range_id)
            {
                while ((ui1_i + 1) < pt_asf_handler->ui1_range_count)
                {
                    if ((ui1_i + 1) >= ASF_MIDXBULD_RANGE_MAX)
                    {
                        pt_asf_handler->ui1_range_count = ASF_MIDXBULD_RANGE_MAX;
                        break;
                    }
                    pt_asf_handler->apt_range_obj[ui1_i] = 
                                           pt_asf_handler->apt_range_obj[ui1_i + 1];
                    pt_asf_handler->apt_range_obj[++ui1_i] = NULL;
                }
                pt_asf_handler->ui1_range_count--;
            }
        }
        else
        {
            pt_asf_handler->pt_key_range_obj = NULL;
        }
        
        DBG_INFO( (" %s#Line%d:free range id = 0x%08x OK\r\n",
                    __FUNCTION__, __LINE__, ui4_range_id ) );
        return TRUE;
    }
    DBG_INFO( ( "%s#Line%d:free range id = 0x%08x fail\r\n",
                __FUNCTION__, __LINE__, ui4_range_id ) );
    
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_alloc_table_id
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/

static BOOL _asf_midxbuld_alloc_table_id( ASF_IDXBULD_RANGE_OBJ_T* pt_range_obj,
                                          UINT32*                  pui4_tbl_id )
{

    /*Parameters check*/
    if (NULL == pt_range_obj)
    {
        DBG_ERROR( ( "%s#Line%d: pt_range_obj is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return FALSE;
    }
    
    *pui4_tbl_id = MIDXBULD_NULL_ID;

    /*not more than 255*/
    if (pt_range_obj->ui1_tbl_total == 0xff)
    {
        DBG_ERROR( ( "%s#Line%d:not more than 255\r\n", __FUNCTION__, __LINE__ ) );
        return FALSE;
    }

    /*the lowest byte for table id*/
    *pui4_tbl_id = ( pt_range_obj->ui4_range_id & 0xffffff00 ) 
                      + ( ++( pt_range_obj->ui1_tbl_total ) );

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_free_table_obj
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/

static BOOL _asf_midxbuld_free_table_obj ( MIDXBULD_OBJ_T* pt_midxbuld_obj,
                                           UINT32          ui4_tbl_id)
{
    ASF_IDXBULD_OBJ_T*          pt_asf_handler;
    ASF_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    ASF_IDXBULD_ELMT_TBL_OBJ_T* pt_last_obj;
    UINT8                       ui1_free_range_id;
    UINT8                       ui1_range_id; 
    UINT8                       ui1_i;
    BOOL                         b_id_match = FALSE;
    MIDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_partial_next;
    MIDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_partial_cur;

    /*Parameters check*/
    if (NULL == pt_midxbuld_obj)
    {
        DBG_ERROR( ( "%s#Line%d: pt_midxbuld_obj is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return FALSE;
    }

    /*get asf index builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if ( !pt_asf_handler )
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler NULL\r\n", __FUNCTION__, __LINE__) );
        return FALSE;
    }
    
    pt_obj = NULL;
    pt_last_obj = NULL;
    
    /*search the range id by table id*/
    if (pt_asf_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == ui4_tbl_id)
    {
        pt_obj = pt_asf_handler->pt_key_range_obj->pt_tbl_obj;
        --( pt_asf_handler->pt_key_range_obj->ui1_tbl_total );
        pt_asf_handler->pt_key_range_obj->pt_tbl_obj = pt_obj->pt_next;
    }
    else
    {   
        /*get range id by table id*/
        ui1_free_range_id = ( UINT8 )( ( ui4_tbl_id & ASF_MIDXBULD_RANGE_ID_MASK ) 
                            >> ASF_MIDXBULD_RANGE_ID_SHFIT );
        for ( ui1_i = 0; ui1_i < pt_asf_handler->ui1_range_count; ++ui1_i )
        {
            pt_obj = NULL;
            
            if (ui1_i >=  ASF_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            ui1_range_id = (UINT8)((pt_asf_handler->apt_range_obj[ui1_i]->ui4_range_id 
                                        & ASF_MIDXBULD_RANGE_ID_MASK) 
                                     >> ASF_MIDXBULD_RANGE_ID_SHFIT );
            /*whether it is the target range id*/
            if (ui1_range_id == ui1_free_range_id)
            {
            
                pt_obj = pt_asf_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
                /*pt_obj should be not NULL,if the given table is the first in a range*/
                if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_asf_handler->apt_range_obj[ui1_i]->pt_tbl_obj = pt_obj->pt_next;
                    --( pt_asf_handler->apt_range_obj[ui1_i]->ui1_tbl_total );
                }
                else
                {
                    pt_last_obj = pt_obj;
                    pt_obj = pt_obj->pt_next;
                }
                
                /*search the rest table nodes in given range*/
                while ( ( pt_obj != NULL ) && ( b_id_match == FALSE ) )
                {
                    if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                    {
                        b_id_match = TRUE;
                        pt_last_obj->pt_next = pt_obj->pt_next;
                        --( pt_asf_handler->apt_range_obj[ui1_i]->ui1_tbl_total );
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
        DBG_INFO( ( "%s#Line%d:not find table 0x%08x\r\n", __FUNCTION__, __LINE__, ui4_tbl_id ) );
        return FALSE;
    }

    /* Free related memory*/
    pt_tbl_partial_cur = &( pt_obj->t_elmt_tbl );
    if ( pt_tbl_partial_cur->u_fmt.t_es.pt_idx_entry )
    {
        _mm_util_mem_free_with_tag( pt_tbl_partial_cur->u_fmt.t_es.pt_idx_entry,
                                    MM_MEM_MIDX_TAG );
        pt_tbl_partial_cur->u_fmt.t_es.pt_idx_entry = NULL;
    }
    pt_tbl_partial_next = pt_tbl_partial_cur->pt_next;
    while ( pt_tbl_partial_next )
    {
        if ( pt_tbl_partial_next->u_fmt.t_es.pt_idx_entry )
        {
            _mm_util_mem_free_with_tag( pt_tbl_partial_next->u_fmt.t_es.pt_idx_entry,
                                   MM_MEM_MIDX_TAG);
            pt_tbl_partial_next->u_fmt.t_es.pt_idx_entry = NULL;
        }
        pt_tbl_partial_cur  = pt_tbl_partial_next;
        pt_tbl_partial_next = pt_tbl_partial_next->pt_next;
        _mm_util_mem_free_with_tag( pt_tbl_partial_cur, MM_MEM_MIDX_TAG );
    }
    _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
    
    DBG_INFO( ( "%s#Line%d:free table 0x%08x OK\r\n",__FUNCTION__, __LINE__, ui4_tbl_id ) );
    return TRUE;
}

static VOID _asf_midxbuld_free_simple_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_SIMPLE_INDEX_OBJECT_T* pt_simple_index_object )
{
    if ( ( NULL == pt_midxbuld_obj ) || ( NULL == pt_simple_index_object ) )
    {
        DBG_ERROR( ( "%s#Line%d: parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return;
    }
    
    /*free resource*/
    if ( pt_simple_index_object->pt_index_entry )
    {
        _mm_util_mem_free_with_tag( pt_simple_index_object->pt_index_entry, 
                                    MM_MEM_MIDX_TAG );
        pt_simple_index_object->pt_index_entry = NULL;
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_load_simple_index_object
 *
 * Description: This function load simple index object
 *
 * Inputs:  pt_minfo_obj: specifies the minfo object 
 *           
 *
 * Outputs: pt_simple_index_object: point to a simple object 
 *
 * Returns: MIDXBULDR_OK :         successful
 *          Other non-zero values: Fail
 * NOTE:
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-3-17 : initial
----------------------------------------------------------------------------*/
static INT32 _asf_midxbuld_load_simple_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_SIMPLE_INDEX_OBJECT_T* pt_simple_index_object)
{
    setpos_fct         pf_set_pos;    
    copybytes_fct      pf_copybyte;
    ASF_IDXBULD_OBJ_T* pt_asf_instance_obj; /*store asf parser related info*/
    UINT8*             pui1_head;           /*read buffer pointer*/
    UINT32             ui4_read_size;
    UINT32             ui4_idx;
    INT32              i4_ret;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pt_midxbuld_obj) || (NULL == pt_simple_index_object))
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos))
    {
        DBG_ERROR( ( "%s#Line%d:callback NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    
    /*get asf instance handler*/
    pt_asf_instance_obj = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_instance_obj)
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_instance_obj is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get buffer*/
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR( ( "%s#Line%d:pui1_header_buf is NULL:r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /*init local variables*/
    ui4_read_size = 0;

    /*load object header from file*/
    i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                         (SIZE_T)ASF_OBJ_COMMON_LEN, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR( ( "%s#Line%d:error object header!\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
        
    /*get object guid and size*/
    _asf_get_guid(&(pt_simple_index_object->t_guid), pui1_head);
    /*GUID is 16 bytes long*/ 
    ASF_GET_QWORD(pt_simple_index_object->ui8_size, pui1_head + 16);   
    if ( ASFR_GUID_EQUAL  != _asf_cmp_guid( &( pt_simple_index_object->t_guid ), 
                         &gt_asf_object_simple_index_guid))
    {
        DBG_ERROR( ( "%s#Line%d:this is not simple index object, abnormal path!\r\n",
                     __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*Header Object is at least 56 bytes*/
    if (pt_simple_index_object->ui8_size < 56)
    {
        DBG_ERROR( ( "%s#Line%d: less than smallest object header 56 bytes, len=%llu\r\n",
                     __FUNCTION__, __LINE__,
                   pt_simple_index_object->ui8_size));
        return MIDXBULDR_INITIAL_ERR;
    }

    /*whether the temporary buffer is enough?*/
    if (pt_asf_instance_obj->ui4_buf_length < (UINT32)pt_simple_index_object->ui8_size)
    {
        _mm_util_mem_free_with_tag( pt_asf_instance_obj->pui1_header_buf, MM_MEM_MIDX_TAG );
        pt_asf_instance_obj->pui1_header_buf = 
            _mm_util_mem_alloc_with_tag((UINT32)pt_simple_index_object->ui8_size,
                                        MM_MEM_MIDX_TAG);
        if (NULL != pt_asf_instance_obj->pui1_header_buf)
        {
            pt_asf_instance_obj->ui4_buf_length = ( UINT32 )pt_simple_index_object->ui8_size;
            pui1_head = pt_asf_instance_obj->pui1_header_buf;
            x_memset(pui1_head, 0,(SIZE_T)pt_simple_index_object->ui8_size);
        }
        else
        {
            DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pui1_header_buf\r\n",
                         __FUNCTION__, __LINE__ ) );
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        
    }

    /*get object fields data other than guid and size*/
    i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                         pt_asf_instance_obj->ui4_buf_length , 
                          ( SIZE_T )( pt_simple_index_object->ui8_size - ASF_OBJ_COMMON_LEN ), 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR( ( "%s#Line%d: pf_copybyte returnt %d\r\n", ui4_read_size,
                     __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /*get File guid ,GUID is 16 bytes long*/
    _asf_get_guid(&(pt_simple_index_object->t_file_id), pui1_head);
    pui1_head += 16;  
    
    /*Get time interval, 8 bytes*/
    ASF_GET_QWORD((pt_simple_index_object->ui8_time_interval), pui1_head);
    pui1_head += 8; 

    /*Getmax_packet_count, 4 bytes*/
    ASF_GET_DWORD((pt_simple_index_object->ui4_max_packet_count), pui1_head);
    pui1_head += 4; 

    /*Get index entry count, 4 bytes*/
    ASF_GET_DWORD((pt_simple_index_object->ui4_index_entry_count), pui1_head);
    pui1_head += 4; 

    if (0 != (pt_simple_index_object->ui4_index_entry_count))
    {
        pt_simple_index_object->pt_index_entry = (ASF_SIMP_IDX_ENTRY_T* ) 
                  _mm_util_mem_alloc_with_tag(
                              sizeof(ASF_SIMP_IDX_ENTRY_T)
                              * (pt_simple_index_object->ui4_index_entry_count),
                              MM_MEM_MIDX_TAG);
        if (NULL == pt_simple_index_object->pt_index_entry)
        {
            DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pt_index_entry\r\n",
                         __FUNCTION__, __LINE__ ) );
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        
        for (ui4_idx = 0; 
             ui4_idx < (pt_simple_index_object->ui4_index_entry_count); 
             ++ui4_idx)
        {
            /*get ui4_packet_number*/
             ASF_GET_DWORD( ( pt_simple_index_object->pt_index_entry[ui4_idx].ui4_packet_number ), 
                            pui1_head);
            pui1_head += 4; 
           
            /*get ui2_packet_count*/
            ASF_GET_WORD( ( pt_simple_index_object->pt_index_entry[ui4_idx].ui2_packet_count ), 
                          pui1_head);
            pui1_head += 2; 
        }
        
    }
    
    return MIDXBULDR_OK;
}

static VOID _asf_midxbuld_free_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_INDEX_OBJECT_T*        pt_index_object )
{
    
    UINT32                      ui4_idx;
    UINT32                      ui4_idx_strm;
    
    ASF_IDX_BLOCK_T*            pt_blk_entry;
    ASF_IDX_EACH_STRM_BLOCK_T*  pt_strm_blk; 

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ( NULL == pt_midxbuld_obj ) || ( NULL == pt_index_object ) )
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return ;
    }

    /************************************************************************/
    /*   do                                                                   */
    /************************************************************************/
    
    /*free  blocks*/
    if ( pt_index_object->pt_blk_entry )
    {
        pt_blk_entry = pt_index_object->pt_blk_entry;
        
        for ( ui4_idx = 0; ui4_idx < ( pt_index_object->ui4_blk_cnt ); 
             ++ui4_idx )
        {
            pt_strm_blk = pt_blk_entry[ui4_idx].pt_strm_blk;
            if ( pt_strm_blk )
            {
                /*free index entry */
                for ( ui4_idx_strm = 0; ui4_idx_strm < ( pt_blk_entry[ui4_idx].ui4_strm_blk_cnt ); 
                     ++ui4_idx_strm )
                {
                    if ( pt_strm_blk[ui4_idx_strm].pui4_entry_offset )
                    {
                        _mm_util_mem_free_with_tag( pt_strm_blk[ui4_idx_strm].pui4_entry_offset, 
                                                    MM_MEM_MIDX_TAG );
                        pt_strm_blk[ui4_idx_strm].pui4_entry_offset = NULL;
                    }
                }
                
                /*free each stream block*/
                _mm_util_mem_free_with_tag( pt_strm_blk, MM_MEM_MIDX_TAG );
                pt_blk_entry[ui4_idx].pt_strm_blk = NULL;
            }

         }
             
         /*free  block*/
         _mm_util_mem_free_with_tag( pt_blk_entry, MM_MEM_MIDX_TAG );
         pt_index_object->pt_blk_entry = NULL;
    }

    /*free specifiers*/
    if ( pt_index_object->pv_specifier_obj )
    {
        _mm_util_mem_free_with_tag( pt_index_object->pv_specifier_obj, MM_MEM_MIDX_TAG );
        pt_index_object->pv_specifier_obj = NULL;

    }
    return ;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_load_index_object
 *
 * Description: This function load  index object
 *
 * Inputs:  pt_minfo_obj: specifies the minfo object 
 *           
 *
 * Outputs: pt_index_object: point to a index object 
 *
 * Returns: MIDXBULDR_OK :         successful
 *               Other non-zero values: Fail
 * NOTE:
 *
 * Author : lianming lin(mtk40234)
 * 
 * History:
 *   (1)2010-11-17 : initial
----------------------------------------------------------------------------*/
static INT32 _asf_midxbuld_load_index_object( 
                            MIDXBULD_OBJ_T*            pt_midxbuld_obj, 
                            ASF_INDEX_OBJECT_T*        pt_index_object )
{
    setpos_fct                  pf_set_pos;    
    copybytes_fct               pf_copybyte;
    ASF_IDXBULD_OBJ_T*          pt_asf_instance_obj; /*store asf parser related info*/
    UINT8*                      pui1_head;           /*read buffer pointer*/
    UINT32                      ui4_read_size;
    INT32                       i4_ret;
    
    UINT32                      ui4_idx;
    UINT32                      ui4_idx_strm;
    UINT32                      ui4_idx_entry;
    UINT32                      ui4_entry_cnt;
    
    ASF_IDX_SPECIFERS_T*        pt_idx_specifier;  
    ASF_IDX_BLOCK_T*            pt_blk_entry;
    ASF_IDX_EACH_STRM_BLOCK_T*  pt_strm_blk; 
    UINT32*                     pui4_entry_offset;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ( NULL == pt_midxbuld_obj ) || ( NULL == pt_index_object ) )
    {
        DBG_ERROR( ( "%s#Line%d:NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    if ( ( !pf_copybyte) || ( !pf_set_pos ) )
    {
        DBG_ERROR( ( "%s#Line%d:callback NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /************************************************************************/
    /*   do                                                                   */
    /************************************************************************/
    
    /*get asf instance handler*/
    pt_asf_instance_obj =  (ASF_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    if ( !pt_asf_instance_obj )
    {
        DBG_ERROR( ( "%%s#Line%d:pt_asf_instance_obj is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get buffer*/
    pui1_head = pt_asf_instance_obj->pui1_header_buf;
    if ( !pui1_head )
    {
        DBG_ERROR( ( "%s#Line%d:pui1_header_buf is NULL:r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /*init local variables*/
    ui4_read_size = 0;

    /*load object header from file*/
    i4_ret = pf_copybyte( pt_midxbuld_obj, pui1_head, 
                          pt_asf_instance_obj->ui4_buf_length , 
                          ( SIZE_T )ASF_OBJ_COMMON_LEN, 
                          &ui4_read_size );
    if ( i4_ret != INPUTR_OK )
    {
        DBG_ERROR( ( "%s#Line%d:error object header!\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
        
    /*get object guid and size*/
    _asf_get_guid( &( pt_index_object->t_guid ), pui1_head );
    /*GUID is 16 bytes long*/ 
    ASF_GET_QWORD( pt_index_object->ui8_size, pui1_head + 16 );   
    if ( ASFR_GUID_EQUAL  != _asf_cmp_guid( ( const ASF_GUID_T* )&( pt_index_object->t_guid ), 
                                            &gt_asf_object_index_guid ) )
    {
        DBG_ERROR(("%s#Line%d:is not simple index object, abnormal path!\r\n", 
                   __FUNCTION__, __LINE__  ));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /* Object size should be more than 34 bytes*/
    if ( pt_index_object->ui8_size <= 34 )
    {
        DBG_ERROR(( "%s#Line%d:object length is wrong and length is %llu\r\n",
                    __FUNCTION__, __LINE__, pt_index_object->ui8_size ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /*whether the temporary buffer is enough?*/
    if ( pt_asf_instance_obj->ui4_buf_length < ( UINT32 )pt_index_object->ui8_size )
    {
        _mm_util_mem_free_with_tag( pt_asf_instance_obj->pui1_header_buf, MM_MEM_MIDX_TAG );
        pt_asf_instance_obj->pui1_header_buf = 
            _mm_util_mem_alloc_with_tag( ( UINT32 )pt_index_object->ui8_size,
                                        MM_MEM_MIDX_TAG );
        if ( NULL != pt_asf_instance_obj->pui1_header_buf )
        {
            pt_asf_instance_obj->ui4_buf_length = ( UINT32 )pt_index_object->ui8_size;
            pui1_head = pt_asf_instance_obj->pui1_header_buf;
            x_memset( pui1_head, 0,(SIZE_T)pt_index_object->ui8_size );
        }
        else
        {
            DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pui1_header_buf\r\n",
                         __FUNCTION__, __LINE__ ) );
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        
    }

    /*get object fields data other than guid and size*/
    i4_ret = pf_copybyte( pt_midxbuld_obj, pui1_head, 
                          pt_asf_instance_obj->ui4_buf_length , 
                          ( SIZE_T )( pt_index_object->ui8_size - ASF_OBJ_COMMON_LEN ), 
                          &ui4_read_size );
    if ( i4_ret != INPUTR_OK )
    {
        DBG_ERROR( ( "%s#Line%d: pf_copybyte returnt %d\r\n",
                    __FUNCTION__, __LINE__, ui4_read_size ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /*Get entry time interval, 4 bytes*/
    ASF_GET_DWORD( ( pt_index_object->ui4_time_interval ), pui1_head );
    pui1_head += 4;  
    
    /*Get specifier count, 2 bytes*/
    ASF_GET_WORD( ( pt_index_object->ui2_specifier_cnt ), pui1_head );
    pui1_head += 2; 

    /*Get block count, 4 bytes*/
    ASF_GET_DWORD( ( pt_index_object->ui4_blk_cnt ), pui1_head );
    pui1_head += 4; 

    /*Get specifiers*/
    if ( pt_index_object->ui2_specifier_cnt > 0 )
    {
        pt_index_object->pv_specifier_obj =  
                  _mm_util_mem_alloc_with_tag(
                  sizeof( ASF_IDX_SPECIFERS_T ) * ( pt_index_object->ui2_specifier_cnt ),
                  MM_MEM_MIDX_TAG );
        if ( NULL == pt_index_object->pv_specifier_obj )
        {
            DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pv_specifier_obj\r\n",
                         __FUNCTION__, __LINE__ ) );
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        pt_idx_specifier = ( ASF_IDX_SPECIFERS_T* )pt_index_object->pv_specifier_obj;

        for ( ui4_idx = 0; ui4_idx < ( pt_index_object->ui2_specifier_cnt ); 
             ++ui4_idx )
        {
             /*Get stream number, 2 bytes*/
             ASF_GET_WORD( ( pt_idx_specifier[ui4_idx].ui2_stream_num ), pui1_head );
             pui1_head += 2; 
             /*Get spcifier count, 2 bytes*/
             ASF_GET_WORD( ( pt_idx_specifier[ui4_idx].ui2_idx_type ), pui1_head );
             pui1_head += 2; 
        }
    }
    
    /*Get blocks*/
    if ( pt_index_object->ui4_blk_cnt > 0 )
    {
        pt_index_object->pt_blk_entry = ( ASF_IDX_BLOCK_T* ) 
                  _mm_util_mem_alloc_with_tag(
                  sizeof( ASF_IDX_BLOCK_T ) * ( pt_index_object->ui4_blk_cnt ),
                  MM_MEM_MIDX_TAG );
        if ( NULL == pt_index_object->pt_blk_entry )
        {
            DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pt_blk_entry\r\n",
                         __FUNCTION__, __LINE__ ) );
            _asf_midxbuld_free_index_object( pt_midxbuld_obj, pt_index_object );
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        
        pt_blk_entry = pt_index_object->pt_blk_entry;
        
        for ( ui4_idx = 0; ui4_idx < ( pt_index_object->ui4_blk_cnt ); 
             ++ui4_idx )
        {
            pt_blk_entry[ui4_idx].ui4_strm_blk_cnt = ( UINT32 )pt_index_object->ui2_specifier_cnt;

            /*apply memory for  every stream block*/
            pt_blk_entry[ui4_idx].pt_strm_blk = ( ASF_IDX_EACH_STRM_BLOCK_T* ) 
                      _mm_util_mem_alloc_with_tag(
                      sizeof( ASF_IDX_EACH_STRM_BLOCK_T ) * ( pt_index_object->ui2_specifier_cnt ),
                      MM_MEM_MIDX_TAG );
            if ( NULL == pt_blk_entry[ui4_idx].pt_strm_blk )
            {
                DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pt_strm_blk\r\n",
                             __FUNCTION__, __LINE__ ) );
                _asf_midxbuld_free_index_object( pt_midxbuld_obj, pt_index_object );
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            pt_strm_blk = pt_blk_entry[ui4_idx].pt_strm_blk;

            /*Get index entry count*/
            ASF_GET_DWORD( ui4_entry_cnt, pui1_head );
            pui1_head += 4;  

            /*Get bolck  position offset relative to the first payload , e.g.  data object + 50*/
            for ( ui4_idx_strm = 0; ui4_idx_strm < ( pt_index_object->ui2_specifier_cnt ); 
                 ++ui4_idx_strm )
            {
                pt_strm_blk[ui4_idx_strm].ui4_entry_cnt = ui4_entry_cnt; 
                ASF_GET_QWORD( ( pt_strm_blk[ui4_idx_strm].ui8_blk_pos ), pui1_head );
                pui1_head += 8; 

                /*apply memory for every index entry*/
                pt_strm_blk[ui4_idx_strm].pui4_entry_offset = ( UINT32* ) 
                          _mm_util_mem_alloc_with_tag(
                          sizeof( UINT32 ) * ( ui4_entry_cnt ), MM_MEM_MIDX_TAG );
                if ( NULL == pt_strm_blk[ui4_idx_strm].pui4_entry_offset )
                {
                    DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pui4_entry_offset\r\n",
                                 __FUNCTION__, __LINE__ ) );
                    _asf_midxbuld_free_index_object( pt_midxbuld_obj, pt_index_object );
                    return MIDXBULDR_OUT_OF_MEMORY;
                }

                
            }

            /*Get each stream block's entry*/
            for ( ui4_idx_entry = 0; ui4_idx_entry < ui4_entry_cnt; ++ui4_idx_entry )
            {
                for ( ui4_idx_strm = 0; ui4_idx_strm < ( pt_index_object->ui2_specifier_cnt ); 
                      ++ui4_idx_strm )
                {
                    pui4_entry_offset = pt_strm_blk[ui4_idx_strm].pui4_entry_offset;
                    ASF_GET_DWORD( ( pui4_entry_offset[ui4_idx_entry] ), pui1_head );
                    pui1_head += 4; 
                }
            }

        }
    }


    
    return MIDXBULDR_OK;
}
static INT32 _asf_midxbuld_gen_key_frame_by_simple_index_object( 
                           MIDXBULD_KEY_INFO_T*        pt_key_info, 
                           MIDXBULD_OBJ_T*             pt_midxbuld_obj,
                           ASF_SIMPLE_INDEX_OBJECT_T*  pt_simple_index_object )
{
    ASF_IDXBULD_OBJ_T*                     pt_asf_handler;
    MIDXBULD_ELMT_STRM_TBL_INFO_T          t_describ;
    ASF_IDXBULD_RANGE_OBJ_T*               pt_range_obj;
    ASF_IDXBULD_ELMT_TBL_OBJ_T*            pt_tbl_obj;
    ASF_IDXBULD_ELMT_TBL_OBJ_T*            pt_tbl_obj_tmp;
    MIDXBULD_ELMT_STRM_ENTRY_T*            pt_idx_entry;
    ASF_SIMP_IDX_ENTRY_T*                  pt_index_entry;
    UINT32                                 ui4_interval;
    UINT32                                 ui4_per_packe_size;
    UINT32                                 ui4_idx;
    UINT32                                 ui4_loops_count;
    MIDXBULD_KEY_TBL_EXT_T*                pt_key_tbl_list;
    UINT32                                 ui4_es_idx;
    MIDXBULD_SOURCE_INFO_T*                pt_src_info;
    MIDXBULD_ELMT_STRM_TBL_INFO_T*         pt_es_map;
    BOOL                                   b_found_src = FALSE;
    INT32                                  i4_ret      = MIDXBULDR_OK;
    MIDXBULD_KEY_TBL_EXT_T*                pt_key_tbl_ext;
    
    if ( !pt_key_info || !pt_midxbuld_obj || !pt_simple_index_object )
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__  ) );
        return MIDXBULDR_INV_ARG;
    }

    x_memset(&t_describ, 0, sizeof(MIDXBULD_ELMT_STRM_TBL_INFO_T));
    
    /*get index builder handler*/
    pt_asf_handler = ( ASF_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    if ( !pt_asf_handler )
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /*source  info comes from minfo via mm_hdlr*/
    pt_src_info = &( pt_midxbuld_obj->t_src_info );
    pt_es_map   = &( pt_src_info->t_es_map[0] );
    
    if (pt_asf_handler->ui2_cur_idx >= ASF_STREAM_NUM_MAX)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    for ( ui4_es_idx = 0; ui4_es_idx < MIDXBULD_ELMT_TBL_MAX; ++ui4_es_idx )
    { 
        if ( pt_es_map[ui4_es_idx].ui4_strm_id == 
            ( UINT32 )pt_asf_handler->ui2_simple_key_strm_id[pt_asf_handler->ui2_cur_idx] )
        {
            b_found_src = TRUE;
            break;
        }
    }
    
    if ( !b_found_src )
    {
        DBG_INFO( ( "%s#Line%d: no stream %u source information but index info exsit!\r\n",
                     __FUNCTION__, __LINE__, 
                   ( UINT32 )pt_asf_handler->ui2_simple_key_strm_id[pt_asf_handler->ui2_cur_idx]) ); 
        return  MIDXBULDR_OK;
    }

    /*!Note:simple index one-one map video stream properties object : ordered by stream number*/
    ++( pt_asf_handler->ui2_cur_idx );
    pt_es_map = &( pt_src_info->t_es_map[ui4_es_idx] );
    
    /*1.  allocate range and its table objects*/
    do
    {
        /* allocate range obj memory */
        if ( NULL == pt_asf_handler->pt_key_range_obj )
        {
            pt_range_obj = ( ASF_IDXBULD_RANGE_OBJ_T* )
                            _mm_util_mem_alloc_with_tag( sizeof( ASF_IDXBULD_RANGE_OBJ_T ),
                            MM_MEM_MIDX_TAG );
            if ( NULL == pt_range_obj )
            {
                DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pt_range_obj\r\n",
                             __FUNCTION__, __LINE__ ) );
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break ;
            }
            x_memset( pt_range_obj, 0, sizeof( ASF_IDXBULD_RANGE_OBJ_T ) );
            
            /*allocate id for pt_range_obj*/
            if ( TRUE != _asf_midxbuld_alloc_range_id( pt_midxbuld_obj, 
                                                       &( pt_key_info->ui4_range_id ) ) )
            {
                _mm_util_mem_free_with_tag( pt_range_obj, MM_MEM_MIDX_TAG );
                pt_range_obj = NULL;
                DBG_ERROR( ( "%s#Line%d: fail to alloc range id\r\n", __FUNCTION__, __LINE__ ) );
                i4_ret = MIDXBULDR_OUT_OF_HANDLE;
                break ;
            }
            pt_range_obj->e_tbl_type                 = MIDXBULD_TBL_TYPE_ELMT_STRM;
            pt_range_obj->u_range.t_es.ui4_start_pts = 0;
            pt_range_obj->u_range.t_es.ui4_end_pts   = MIDXBULD_IDX_RANGE_FULL;
            pt_range_obj->ui4_range_id               = pt_key_info->ui4_range_id;
            pt_range_obj->b_is_key_frame             = TRUE;
            pt_range_obj->b_index_is_valid           = FALSE;

            
            pt_asf_handler->pt_curr_range_obj  = pt_range_obj;
            pt_asf_handler->pt_key_range_obj   = pt_range_obj;
            
        }
        else
        {
            pt_range_obj = pt_asf_handler->pt_key_range_obj;
        }
    
        /* allocate elmt tbl obj memory */
        pt_tbl_obj = ( ASF_IDXBULD_ELMT_TBL_OBJ_T* ) 
                      _mm_util_mem_alloc_with_tag( sizeof( ASF_IDXBULD_ELMT_TBL_OBJ_T ),
                      MM_MEM_MIDX_TAG );
        if ( NULL == pt_tbl_obj )
        {
            ( VOID )_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                                  pt_key_info->ui4_range_id );
            pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
            DBG_ERROR( ( "%s#Line%d: failed to allocate memory for pt_tbl_obj\r\n",
                         __FUNCTION__, __LINE__ ) );
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break ;
        }
        x_memset( pt_tbl_obj, 0, sizeof( ASF_IDXBULD_ELMT_TBL_OBJ_T ) );
        
        if ( TRUE != _asf_midxbuld_alloc_table_id( pt_range_obj, 
                                                   &( pt_tbl_obj->ui4_tbl_id ) ) )
        {
            _mm_util_mem_free_with_tag( pt_tbl_obj, MM_MEM_MIDX_TAG );
            pt_tbl_obj = NULL;
            ( VOID )_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                                  pt_key_info->ui4_range_id );
            pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
            DBG_ERROR( ( "%s#Line%d: fail to alloc table id\r\n", __FUNCTION__, __LINE__ ) );
            i4_ret = MIDXBULDR_OUT_OF_HANDLE;
            break ;
        }
        /*add table info to handler*/
        if (pt_asf_handler->ui1_tbl_count < ASF_STREAM_NUM_MAX)
        {
            pt_asf_handler->apt_tbl_obj_map[pt_asf_handler->ui1_tbl_count++] = pt_tbl_obj;
        }
        else
        {
            DBG_ERROR( ( "%s#Line%d: apt_tbl_obj_map is overflow\r\n", __FUNCTION__, __LINE__ ) );
            i4_ret = MIDXBULDR_INTERNAL_ERR;
            break ;
        }
        /*set key  table id*/
        if ( MIDXBULD_NULL_ID == pt_key_info->ui4_tbl_id )
        {
            pt_key_info->ui4_tbl_id = pt_tbl_obj->ui4_tbl_id;
        }
        
        /*set the range related table pointer*/
        if ( NULL == pt_range_obj->pt_tbl_obj )
        {
            pt_range_obj->pt_tbl_obj = pt_tbl_obj;
        }
        else
        {
            pt_tbl_obj_tmp = pt_range_obj->pt_tbl_obj;
            while ( pt_tbl_obj_tmp->pt_next )
            {
                pt_tbl_obj_tmp = pt_tbl_obj_tmp->pt_next;
            }
            pt_tbl_obj_tmp->pt_next = pt_tbl_obj;
        }

        ASSERT( pt_es_map->e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO );
        t_describ.e_es_type     = MIDXBULD_ELEM_STRM_TYPE_KEY;
        t_describ.ui4_strm_id  = pt_es_map->ui4_strm_id;
        t_describ.ui4_scale    = pt_es_map->ui4_scale;
        t_describ.ui4_rate     = pt_es_map->ui4_rate;
        t_describ.ui4_sample_sz = pt_es_map->ui4_sample_sz;
        t_describ.ui4_avg_bytes_per_sec = pt_es_map->ui4_avg_bytes_per_sec;
        /*stream descritption*/
        x_memcpy( ( VOID* )&( pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info ), 
                  ( VOID* )&t_describ, sizeof( MIDXBULD_ELMT_STRM_TBL_INFO_T ) );
    }while  ( 0 );

    /*if  error break*/
    if ( i4_ret != MIDXBULDR_OK )
    {
        DBG_ERROR( ( "%s#Line%d: fail to alloc range or table failed!\r\n", __FUNCTION__, __LINE__ ) );
        return i4_ret;
    }
    
    /*2  physcical file offset = ui8_base_offset + ui4_relative_offset
              the packet begin = the object size + fixed length(50 bytes)*/
    do 
    {
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 
                   pt_midxbuld_obj->t_src_info.ui8_data_offset + 50;
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry =
                   pt_simple_index_object->ui4_index_entry_count;
        pt_tbl_obj->ui4_tbl_size = 
                   pt_simple_index_object->ui4_index_entry_count;
        
        pt_idx_entry = ( MIDXBULD_ELMT_STRM_ENTRY_T* )
                        _mm_util_mem_alloc_with_tag( sizeof(MIDXBULD_ELMT_STRM_ENTRY_T ) 
                        * pt_tbl_obj->ui4_tbl_size,
                        MM_MEM_MIDX_TAG );
        if ( NULL == pt_idx_entry )
        {
            
            DBG_ERROR( ( "%s#Line%d: memory for pt_idx_entry\r\n", __FUNCTION__, __LINE__ ) );
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break ;
        }
        /*set allocated memoty 0*/
        x_memset( pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size );
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;
    
        /*get simple index object entry*/
        pt_index_entry = pt_simple_index_object->pt_index_entry;
        
        /*calculate the interval according to the system clock*/
        ui4_interval = ( UINT32 ) _mm_div64( ( pt_simple_index_object->ui8_time_interval
                                             * MIDXBULD_SYSTEM_CLOCK_FREQUENCY ),
                                             ( 10000 * 1000 ),
                                              NULL );
        /*get packet size from source info*/
        ui4_per_packe_size = pt_midxbuld_obj->t_src_info.ui4_packet_sz;

        /*
        DTV00348914
        sort the raw index data by packet number when table size is less than 500
        */
        if (pt_tbl_obj->ui4_tbl_size < 500)
        {
#ifndef __KERNEL__        
            x_qsort(pt_index_entry, 
                    pt_tbl_obj->ui4_tbl_size, 
                    sizeof(ASF_SIMP_IDX_ENTRY_T), 
                    _asf_midxbuld_entry_compare,
                    NULL);
#endif
        }
        /*
        DTV00348904 
        the first seek pointer is not 0, so index table for every entry's pts will delay a interval time
        */
        ui4_idx = (pt_index_entry->ui4_packet_number == 0)? 0:1;
        ui4_loops_count = (pt_index_entry->ui4_packet_number == 0)?
                           pt_tbl_obj->ui4_tbl_size : pt_tbl_obj->ui4_tbl_size+1;
        /*translate<pts, packet num, packet count> to <pts,offset,size> */
        for (; ui4_idx < ui4_loops_count; ++ui4_idx )
        {
            /*Do:between t_simple_index_object and pt_idx_entry */
            pt_idx_entry->ui4_pts = ui4_interval * ui4_idx;
            pt_idx_entry->ui4_relative_offset = pt_index_entry->ui4_packet_number 
                                                * ui4_per_packe_size;
            pt_idx_entry->ui4_size = pt_index_entry->ui2_packet_count * ui4_per_packe_size;
            ++pt_index_entry;   /*t_simple_index_object*/
            ++pt_idx_entry;     /*pt_idx_entry*/
            if ( ui4_idx > pt_tbl_obj->ui4_tbl_size / 2 )
            {
                pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_MID;
            }
        }
    
        /*3. set key table info even if its key exists inserted  by other index object, up to driver*/
        if ( MIDXBULD_NULL_ID == pt_asf_handler->t_key_table_obj.ui4_key_tbl_id )
        {
            pt_asf_handler->t_key_table_obj.ui4_key_tbl_id = pt_key_info->ui4_tbl_id;
        }
        pt_key_tbl_ext = ( MIDXBULD_KEY_TBL_EXT_T* )
                        _mm_util_mem_alloc_with_tag( sizeof( MIDXBULD_KEY_TBL_EXT_T ) ,
                        MM_MEM_MIDX_TAG );
        if ( NULL == pt_key_tbl_ext )
        {
            DBG_ERROR( ( "%s#Line%d: memory for pt_key_tbl_ext\r\n",__FUNCTION__, __LINE__ ) );
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break ;
        }
        x_memset( pt_key_tbl_ext, 0, sizeof( MIDXBULD_KEY_TBL_EXT_T ) );
        pt_key_tbl_ext->t_elmt_tbl = pt_tbl_obj->t_elmt_tbl;
       
        pt_key_tbl_list = pt_asf_handler->t_key_table_obj.pt_key_tbl_ext;
        if ( !pt_key_tbl_list )
        {
            pt_asf_handler->t_key_table_obj.pt_key_tbl_ext = pt_key_tbl_ext;
            pt_asf_handler->b_key_tbl_ok = TRUE;
        }
        else
        {
            while ( pt_key_tbl_list->pt_next )
            {
                pt_key_tbl_list = pt_key_tbl_list->pt_next;
            }
            pt_key_tbl_list->pt_next = pt_key_tbl_ext;
            
        }
        
    }while(0);
    
    
    if ( i4_ret != MIDXBULDR_OK )
    {
        ( VOID )_asf_midxbuld_free_table_obj( pt_midxbuld_obj,
                                              pt_key_info->ui4_tbl_id );
        ( VOID )_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                              pt_key_info->ui4_range_id );
        pt_key_info->ui4_tbl_id   = MIDXBULD_NULL_ID;
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;

        while ( NULL != pt_asf_handler->t_key_table_obj.pt_key_tbl_ext )
        {
            pt_key_tbl_list = pt_asf_handler->t_key_table_obj.pt_key_tbl_ext;
            pt_asf_handler->t_key_table_obj.pt_key_tbl_ext = pt_key_tbl_list->pt_next;
            _mm_util_mem_free_with_tag( pt_key_tbl_list, MM_MEM_MIDX_TAG );
        }
        pt_asf_handler->t_key_table_obj.pt_key_tbl_ext = NULL;
    }
    
    return i4_ret;
    
}

INT32 _asf_midxbuld_gen_key_frame_by_index_object( 
                                   MIDXBULD_KEY_INFO_T* pt_key_info, 
                                   MIDXBULD_OBJ_T*      pt_midxbuld_obj,
                                   ASF_INDEX_OBJECT_T*  pt_index_object )
{
    ASF_IDXBULD_OBJ_T*                     pt_asf_handler;
    MIDXBULD_ELMT_STRM_TBL_INFO_T          t_describ;
    ASF_IDXBULD_RANGE_OBJ_T*               pt_range_obj;
    ASF_IDXBULD_ELMT_TBL_OBJ_T*            pt_tbl_obj;
    UINT32                                 ui4_idx;
    UINT32                                 ui4_es_idx;
    ASF_IDXBULD_ELMT_TBL_OBJ_T*            pt_last_tbl_obj = NULL;
    INT32                                  i4_ret          = MIDXBULDR_OK;
    BOOL                                   b_found_src     = FALSE;
    UINT32                                 ui4_blk_idx;
    UINT32                                 ui4_blk_idx_temp;
    UINT32                                 ui4_entry_idx;
    UINT32                                 ui4_time_interval;
    ASF_IDX_BLOCK_T*                       pt_blk_entry;
    ASF_IDX_EACH_STRM_BLOCK_T*             pt_strm_blk;
    ASF_IDX_SPECIFERS_T*                   pt_idx_specifier;
    UINT16                                 ui2_stream_num;
    UINT16                                 ui2_idx_type;
    MIDXBULD_SOURCE_INFO_T*                pt_src_info;
    MIDXBULD_ELMT_STRM_TBL_INFO_T*         pt_es_map;
    MIDXBULD_ELMT_TBL_OBJ_T*               pt_elmt_tbl;
    MIDXBULD_ELMT_TBL_OBJ_T*               pt_last_elmt_tbl;
    MIDXBULD_ELMT_STRM_ENTRY_T*            pt_idx_entry;
    MIDXBULD_KEY_TBL_EXT_T*                pt_last_key_tbl_obj;
    ASF_IDXBULD_ELMT_TBL_OBJ_T*            pt_next_tbl_obj;
    MIDXBULD_KEY_TBL_EXT_T*                pt_this_key_tbl_obj;
    
    if ( !pt_key_info || !pt_midxbuld_obj || !pt_index_object )
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__  ) );
        return MIDXBULDR_INV_ARG;
    }

    x_memset(&t_describ, 0, sizeof(MIDXBULD_ELMT_STRM_TBL_INFO_T));
    
    /*get index builder handler*/
    pt_asf_handler = ( ASF_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    if ( !pt_asf_handler )
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*It assume not than 127 streams */
    pt_blk_entry = pt_index_object->pt_blk_entry;
    if ( !pt_blk_entry )
    {
        DBG_ERROR( ( "%s#Line%d:pt_blk_entry is NULL\r\n",
                     __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INTERNAL_ERR;
    }
    if ( !( pt_blk_entry
             && ( pt_blk_entry->ui4_strm_blk_cnt > 0 ) 
             && ( pt_blk_entry->ui4_strm_blk_cnt < ASF_STREAM_NUM_MAX ) ) )
    {
        DBG_ERROR( ( "%s#Line%d:ui4_strm_blk_cnt is %u\r\n",
                     __FUNCTION__, __LINE__,
                     pt_blk_entry->ui4_strm_blk_cnt ) );
        return MIDXBULDR_INTERNAL_ERR;
    }

    /*Get specifiers that determines which stream has index information*/
    pt_idx_specifier = ( ASF_IDX_SPECIFERS_T* )pt_index_object->pv_specifier_obj;

    /*1. allocate range and its table objects*/
    do
    {
        /* allocate range obj memory */
        pt_range_obj = ( ASF_IDXBULD_RANGE_OBJ_T* )
                        _mm_util_mem_alloc_with_tag( sizeof( ASF_IDXBULD_RANGE_OBJ_T ),
                        MM_MEM_MIDX_TAG );
        if ( !pt_range_obj )
        {
            DBG_ERROR( ( "%s#Line%d:failed to allocate memory for pt_range_obj\r\n",
                         __FUNCTION__, __LINE__ ) );
            i4_ret = MIDXBULDR_OUT_OF_MEMORY;
            break ;
        }
        x_memset( pt_range_obj, 0, sizeof( ASF_IDXBULD_RANGE_OBJ_T ) );
    
        /*allocate id for pt_range_obj*/
        if ( TRUE != _asf_midxbuld_alloc_range_id( pt_midxbuld_obj, 
                                                   &pt_key_info->ui4_range_id ) )
        {
            _mm_util_mem_free_with_tag( pt_range_obj, MM_MEM_MIDX_TAG );
            pt_range_obj = NULL;
            DBG_ERROR( ( "%s#Line%d: fail to alloc range id\r\n", __FUNCTION__, __LINE__ ) );
            i4_ret = MIDXBULDR_OUT_OF_HANDLE;
            break ;
        }
        pt_range_obj->e_tbl_type                  = MIDXBULD_TBL_TYPE_ELMT_STRM;
        pt_range_obj->u_range.t_es.ui4_start_pts  = 0;
        pt_range_obj->u_range.t_es.ui4_end_pts    = MIDXBULD_IDX_RANGE_FULL;
        pt_range_obj->ui4_range_id                = pt_key_info->ui4_range_id;
        pt_range_obj->b_is_key_frame              = TRUE;
        pt_range_obj->b_index_is_valid            = FALSE;
        
        pt_asf_handler->pt_curr_range_obj = pt_range_obj;
        pt_asf_handler->pt_key_range_obj  = pt_range_obj;

        /*get block entry head*/
        pt_blk_entry = pt_index_object->pt_blk_entry;

        /*source  info comes from minfo via mm_hdlr*/
        pt_src_info  = &( pt_midxbuld_obj->t_src_info );

        /*every stream should has a  index table object*/
        for ( ui4_idx = 0; ui4_idx < pt_blk_entry->ui4_strm_blk_cnt; ++ui4_idx )
        {
            b_found_src = FALSE;
            pt_strm_blk = pt_blk_entry->pt_strm_blk + ui4_idx;

            ui2_stream_num = pt_idx_specifier[ui4_idx].ui2_stream_num;

            pt_es_map = ( MIDXBULD_ELMT_STRM_TBL_INFO_T* )&( pt_src_info->t_es_map[0] );
            for ( ui4_es_idx = 0; ui4_es_idx < MIDXBULD_ELMT_TBL_MAX; ++ui4_es_idx )
            {
                if ( pt_es_map[ui4_es_idx].ui4_strm_id == ( UINT32 )ui2_stream_num )
                {
                    b_found_src = TRUE;
                    break;
                }
            }
            if ( !b_found_src )
            {
                DBG_INFO( ( "%s#Line%d: no stream %u source information but index info exsit!\r\n",
                             __FUNCTION__, __LINE__, ui2_stream_num) ); 
            }
            else
            {
            pt_es_map = &( pt_src_info->t_es_map[ui4_es_idx] );
            }
            
            /* allocate elmt tbl obj memory */
            pt_tbl_obj = ( ASF_IDXBULD_ELMT_TBL_OBJ_T* ) 
                          _mm_util_mem_alloc_with_tag( sizeof( ASF_IDXBULD_ELMT_TBL_OBJ_T ),
                          MM_MEM_MIDX_TAG );
            if ( !pt_tbl_obj )
            {
                (VOID)_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                                    pt_key_info->ui4_range_id );
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                DBG_ERROR( ( "%s#Line%d: failed to allocate memory for pt_tbl_obj\r\n",
                             __FUNCTION__, __LINE__ ) );
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break ;
            }
            x_memset( pt_tbl_obj, 0, sizeof( ASF_IDXBULD_ELMT_TBL_OBJ_T ) );

            pt_tbl_obj->ui4_tbl_size = pt_strm_blk->ui4_entry_cnt;
            
            if ( TRUE != _asf_midxbuld_alloc_table_id( pt_range_obj, 
                                                       &( pt_tbl_obj->ui4_tbl_id ) ) )
            {
                _mm_util_mem_free_with_tag( pt_tbl_obj, MM_MEM_MIDX_TAG );
                pt_tbl_obj = NULL;
                (VOID)_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                                    pt_key_info->ui4_range_id );
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                DBG_ERROR( ( "%s#Line%d: fail to alloc table id\r\n", __FUNCTION__, __LINE__ ) );
                i4_ret = MIDXBULDR_OUT_OF_HANDLE;
                break ;
            }

            /*First stream is inseted at the first of table table list, FIFO*/
            if ( NULL == pt_range_obj->pt_tbl_obj )
            {
                /*set table id*/
                pt_key_info->ui4_tbl_id = pt_tbl_obj->ui4_tbl_id;
                
                /*set the range related table pointer*/
                pt_range_obj->pt_tbl_obj    = pt_tbl_obj;
            }
            else
            {
                if (pt_last_tbl_obj != NULL)
                {
                    pt_last_tbl_obj->pt_next = pt_tbl_obj;
                }
                else
                {
                    DBG_ERROR( ( "%s#Line%d: pt_last_tbl_obj is null\r\n", __FUNCTION__, __LINE__ ) );
                    i4_ret = MIDXBULDR_INTERNAL_ERR;
                    break ;
                }
            }
            pt_last_tbl_obj = pt_tbl_obj;
            
            if (pt_asf_handler->ui1_tbl_count < ASF_STREAM_NUM_MAX)
            {
                pt_asf_handler->apt_tbl_obj_map[pt_asf_handler->ui1_tbl_count++] = pt_tbl_obj;
            }
            else
            {
                DBG_ERROR( ( "%s#Line%d: apt_tbl_obj_map is overflow\r\n", __FUNCTION__, __LINE__ ) );
                i4_ret = MIDXBULDR_INTERNAL_ERR;
                break ;
            }

            if ( b_found_src )
            {
            t_describ.e_es_type     = pt_es_map->e_es_type;
            t_describ.ui4_strm_id   = pt_es_map->ui4_strm_id;
            t_describ.ui4_scale     = pt_es_map->ui4_scale;
            t_describ.ui4_rate      = pt_es_map->ui4_rate;
            t_describ.ui4_sample_sz = pt_es_map->ui4_sample_sz;
            t_describ.ui4_avg_bytes_per_sec = pt_es_map->ui4_avg_bytes_per_sec;
            }
            else
            {
                x_memset( ( VOID* )&( t_describ ), 0, sizeof( MIDXBULD_ELMT_STRM_TBL_INFO_T ) );
                t_describ.ui4_strm_id = ui2_stream_num;
            }
            /*stream descritption*/
            x_memcpy( ( VOID* )&( pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info ), 
                      ( VOID* )&( t_describ ),
                      sizeof( MIDXBULD_ELMT_STRM_TBL_INFO_T ) );
        }

    }while (0);

    if ( i4_ret != MIDXBULDR_OK )
    {
        DBG_ERROR( ( "%s#Line%d: error return (%d)\r\n", __FUNCTION__, __LINE__, i4_ret) );
        return i4_ret;
    }

    /*2. fill table entry,all block within a stream are in a list */
    ui4_time_interval = pt_index_object->ui4_time_interval
                        * ( MIDXBULD_SYSTEM_CLOCK_FREQUENCY / 1000 ); /*ms--->clock*/
    for ( ui4_blk_idx = 0; ui4_blk_idx < pt_index_object->ui4_blk_cnt; ++ui4_blk_idx )
    {
        pt_tbl_obj   = pt_range_obj->pt_tbl_obj;
        pt_blk_entry = pt_index_object->pt_blk_entry + ui4_blk_idx;
        for ( ui4_idx = 0; ui4_idx < pt_blk_entry->ui4_strm_blk_cnt; ++ui4_idx )
        {
            /*Just translate  the index  info of  a stream we need*/
            if ( !pt_tbl_obj )
            {
                break;
            }
            pt_strm_blk   = pt_blk_entry->pt_strm_blk + ui4_idx;
            if ( pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id 
                 != ( UINT32 )( pt_idx_specifier[ui4_idx].ui2_stream_num ) )
            {
                /*this stream do not build index info*/
                continue;
            }
            ASSERT( pt_strm_blk->pui4_entry_offset );
            
            pt_elmt_tbl   = &( pt_tbl_obj->t_elmt_tbl );
            ui2_idx_type  = pt_idx_specifier[ui4_idx].ui2_idx_type;
            
            ui4_blk_idx_temp = ui4_blk_idx;
            pt_last_elmt_tbl = pt_elmt_tbl;
            while ( ui4_blk_idx_temp ) /*ignore prior blocks related tables*/
            {
                ASSERT( pt_elmt_tbl );
                pt_last_elmt_tbl = pt_elmt_tbl;
                pt_elmt_tbl      = pt_elmt_tbl->pt_next;
                --ui4_blk_idx_temp;
            }
            if ( !pt_elmt_tbl )
            {
                pt_elmt_tbl = ( MIDXBULD_ELMT_TBL_OBJ_T* ) 
                              _mm_util_mem_alloc_with_tag( sizeof( MIDXBULD_ELMT_TBL_OBJ_T ),
                              MM_MEM_MIDX_TAG );
                if ( !pt_elmt_tbl )
                {
                    (VOID)_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                                        pt_key_info->ui4_range_id );
                    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                    DBG_ERROR( ( "%s#Line%d: failed to allocate memory for pt_elmt_tbl\r\n",
                                 __FUNCTION__, __LINE__ ) );
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset( pt_elmt_tbl, 0, sizeof( MIDXBULD_ELMT_TBL_OBJ_T ) );
                
                ASSERT( pt_last_elmt_tbl );
                pt_last_elmt_tbl->pt_next = pt_elmt_tbl;
            }
            pt_elmt_tbl->e_tbl_type          = MIDXBULD_TBL_TYPE_ELMT_STRM;
            pt_elmt_tbl->ui4_number_of_entry = pt_strm_blk->ui4_entry_cnt;
            pt_elmt_tbl->ui8_base_offset     = pt_strm_blk->ui8_blk_pos 
                        +  ( pt_midxbuld_obj->t_src_info.ui8_data_offset + 50 );
            pt_elmt_tbl->u_fmt.t_es.t_info   = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
            pt_elmt_tbl->u_fmt.t_es.pt_idx_entry = ( MIDXBULD_ELMT_STRM_ENTRY_T* ) 
                _mm_util_mem_alloc_with_tag( sizeof( MIDXBULD_ELMT_STRM_ENTRY_T ) 
                                            * ( pt_strm_blk->ui4_entry_cnt ),
                                              MM_MEM_MIDX_TAG );
            if ( !( pt_elmt_tbl->u_fmt.t_es.pt_idx_entry ) )
            {
                (VOID)_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                                    pt_key_info->ui4_range_id );
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                DBG_ERROR( ( "%s#Line%d: failed to allocate memory for pt_elmt_tbl->pt_next\r\n",
                             __FUNCTION__, __LINE__ ) );
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset( pt_elmt_tbl->u_fmt.t_es.pt_idx_entry, 0, 
                      sizeof( MIDXBULD_ELMT_STRM_ENTRY_T ) * ( pt_strm_blk->ui4_entry_cnt ) );
            
            for ( ui4_entry_idx = 0; ui4_entry_idx < pt_strm_blk->ui4_entry_cnt; ++ui4_entry_idx )
            {
                pt_idx_entry = pt_elmt_tbl->u_fmt.t_es.pt_idx_entry + ui4_entry_idx;
                pt_idx_entry->ui4_pts = ui4_time_interval * ui4_entry_idx;
                pt_idx_entry->ui4_relative_offset = ( pt_strm_blk->pui4_entry_offset )[ui4_entry_idx];
                pt_idx_entry->ui4_size            =  ui2_idx_type; /*not size but index type*/
                
            }
            /*next stream related table*/
            pt_tbl_obj   = pt_tbl_obj->pt_next;
            
        }
    }

    /*3.  set key table  info*/
    pt_asf_handler->b_key_tbl_ok      = TRUE;
    pt_asf_handler->t_key_table_obj.ui4_key_tbl_id = pt_key_info->ui4_tbl_id;
    pt_asf_handler->t_key_table_obj.pt_key_tbl_ext = ( MIDXBULD_KEY_TBL_EXT_T* )
                    _mm_util_mem_alloc_with_tag( sizeof( MIDXBULD_KEY_TBL_EXT_T ),
                    MM_MEM_MIDX_TAG );
    if ( NULL == pt_asf_handler->t_key_table_obj.pt_key_tbl_ext )
    {
        (VOID)_asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                            pt_key_info->ui4_range_id );
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        DBG_ERROR( ( "%s#Line%d: memory for pt_key_tbl_list\r\n", __FUNCTION__, __LINE__  ) );
        return  MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset( pt_asf_handler->t_key_table_obj.pt_key_tbl_ext, 0, sizeof( MIDXBULD_KEY_TBL_EXT_T ) );

    pt_this_key_tbl_obj = pt_asf_handler->t_key_table_obj.pt_key_tbl_ext;
    pt_last_key_tbl_obj = NULL;
    
    pt_next_tbl_obj     = pt_range_obj->pt_tbl_obj;
    while ( pt_next_tbl_obj )
    {
        /*now only add video index to key table*/
        if ( ( pt_next_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_KEY )
            || ( pt_next_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO ))
        {
            if ( !pt_this_key_tbl_obj  )
            {
                pt_this_key_tbl_obj = ( MIDXBULD_KEY_TBL_EXT_T* )
                                    _mm_util_mem_alloc_with_tag( sizeof( MIDXBULD_KEY_TBL_EXT_T ) ,
                                    MM_MEM_MIDX_TAG );
                if ( !pt_this_key_tbl_obj )
                {
                    _asf_midxbuld_free_hdlr_info(pt_midxbuld_obj);
                    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                    DBG_ERROR( ( "%s#Line%d: memory for pt_this_key_tbl_obj\r\n", __FUNCTION__, __LINE__ ) );
                    return MIDXBULDR_OUT_OF_MEMORY;
                }
                x_memset( pt_this_key_tbl_obj, 0, sizeof( MIDXBULD_KEY_TBL_EXT_T ) );
                
                
            }
            pt_this_key_tbl_obj->t_elmt_tbl  = pt_next_tbl_obj->t_elmt_tbl;
            
            if ( pt_next_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type == MIDXBULD_ELEM_STRM_TYPE_VIDEO )
            {
                pt_this_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.e_es_type = MIDXBULD_ELEM_STRM_TYPE_KEY;
            }
            
            if ( pt_last_key_tbl_obj )
            {
                pt_last_key_tbl_obj->pt_next = pt_this_key_tbl_obj;
            }
            pt_last_key_tbl_obj              = pt_this_key_tbl_obj;
            pt_this_key_tbl_obj              = NULL;
        }
        
        pt_next_tbl_obj = pt_next_tbl_obj->pt_next;
    }

    return i4_ret;
    
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_index_object
 *
 * Description: This function paser  index objects (index object. media object index, timecode index)
 *
 * Inputs:  
 *         pt_key_info:     specifies which stream should build key frame table
 *         pt_midxbuld_obj: specify the media info object  
 *
 * Outputs: NULL 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the 
 *                           operation has been completed.
 *          Other non-zero values: Fail
 * Note:  
 * Author : lianming lin(mtk40234)
 * 
 * History:
 *   (1)2010-11-17 : initial
 ----------------------------------------------------------------------------*/
static INT32 _asf_midxbuld_index_object_indexing( 
                                           MIDXBULD_KEY_INFO_T* pt_key_info, 
                                           MIDXBULD_OBJ_T*      pt_midxbuld_obj )
{
    setpos_fct                  pf_set_pos;    
    copybytes_fct               pf_copybyte;
    getpos_fct                  pf_get_pos;
    ASF_IDXBULD_OBJ_T*          pt_asf_handler;
    UINT8*                      pui1_head;    /*read buffer pointer*/
    
    UINT32                      ui4_read_size ;
    INT32                       i4_ret; 
    UINT64                      ui8_cur_pos;
    
    ASF_INDEX_OBJECT_T          t_index_object;
    BOOL                        b_found_idx_obj = FALSE;
    
#ifdef CLI_LVL_ALL
    CHAR       ac_temp[40];      /*GUID str temp buffer*/
    UINT32     ui4_len;          /*GUID str temp buffer length*/
#endif  

    /**************************************************************************
                        Check parameters
       **************************************************************************/
    if ( ( !pt_key_info ) || ( ! pt_midxbuld_obj ) )
    {
        DBG_ERROR( ( "%s#Line%d: parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INV_ARG;
    }

    /*init local variables*/
    x_memset( ( VOID* )&t_index_object, 0, sizeof( ASF_INDEX_OBJECT_T ) );
   
    /*get callback*/
    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
    if ( ( !pf_copybyte ) || ( !pf_set_pos ) || ( !pf_get_pos ) )
    {
        DBG_ERROR( ( "%s#Line%d:callback is NULL\r\n", __FUNCTION__, __LINE__  ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get index builder handler*/
    pt_asf_handler = ( ASF_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;
    if ( !pt_asf_handler )
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_asf_handler->pui1_header_buf;
    if ( !pui1_head )
    {
        DBG_ERROR( ( "%s#Line%d:pui1_header_buf is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    /* 1. paser object */
    ui8_cur_pos = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
    pf_set_pos( pt_midxbuld_obj, ( INT64 )ui8_cur_pos, FM_SEEK_BGN );

    /*load object header from file*/ 
    while ( ui8_cur_pos < pt_midxbuld_obj->z_buff_leng ) 
    {
        /*get GUID and size*/
        i4_ret = pf_copybyte( pt_midxbuld_obj, pui1_head, 
                              pt_asf_handler->ui4_buf_length , 
                              ASF_OBJ_COMMON_LEN, 
                              &ui4_read_size );
        if ( i4_ret != INPUTR_OK )
        {
            DBG_ERROR( ( "%s#Line%d:pf_copybyte return %d\r\n",
                         __FUNCTION__, __LINE__, ui4_read_size ) );
            b_found_idx_obj = FALSE;
            break;
        }
     
        /*get object guid and size*/
        _asf_get_guid( &( t_index_object.t_guid ), pui1_head );
         /*GUID is 16 bytes long*/ 
        ASF_GET_QWORD( t_index_object.ui8_size, ( pui1_head + 16 ) );  
        
        /*if not  index object just pass this is abnormal path*/
        if ( ASFR_GUID_EQUAL != _asf_cmp_guid( &( t_index_object.t_guid ), 
                                               &gt_asf_object_index_guid ) )
        {
            #ifdef CLI_LVL_ALL
            /*dispay the object */
            x_memset( ac_temp, 0, sizeof( ac_temp ) );
            _asf_guid_to_str( &( t_index_object.t_guid ), ac_temp, &ui4_len );
            x_dbg_stmt( "Object ID:%s\r\n", ac_temp );
            
            x_dbg_stmt( "Object Size:\t%llu\r\n",  t_index_object.ui8_size );
            x_dbg_stmt( "File Position:\t%llu\r\n",  ui8_cur_pos );
            #endif
            ui8_cur_pos += t_index_object.ui8_size;
            pf_set_pos( pt_midxbuld_obj, ( INT64 )ui8_cur_pos, FM_SEEK_BGN );
            DBG_INFO( ( "%s#Line%d: objects are passed and its information:\r\n",
                         __FUNCTION__, __LINE__ ) );
            continue;
        }

        /*restore current file position*/
        pf_set_pos( pt_midxbuld_obj, ( INT64 )ui8_cur_pos, FM_SEEK_BGN );
        pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_LOAD_IDX_START;
        i4_ret = _asf_midxbuld_load_index_object( pt_midxbuld_obj, 
                                                  &t_index_object );
        if ( MIDXBULDR_OK == i4_ret )
        {   
            b_found_idx_obj = TRUE;    
            
            pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_LOAD_IDX_END;
            break;
        }
        else
        {
            return MIDXBULDR_INITIAL_ERR;
        }
        /*not go here ,do not set position*/
        
    }

    /*If there doesn't exist  index object,  return not ready*/
    if ( FALSE == b_found_idx_obj )
    {
        _asf_midxbuld_free_index_object( pt_midxbuld_obj, &t_index_object );
        return MIDXBULDR_KEY_TBL_NOT_READY;
    }

    /*2.  translate<pts, packet num, packet count> to <pts,offset,size>*/
    pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_START;
    i4_ret = _asf_midxbuld_gen_key_frame_by_index_object( pt_key_info,
                                                          pt_midxbuld_obj, 
                                                          &t_index_object );
    if ( MIDXBULDR_OK != i4_ret )
    {
        DBG_ERROR( ( "%s#Line%d:generate key frame error(%d)\r\n",
                     __FUNCTION__, __LINE__, i4_ret ) );
    }
    pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_END;

    /*3. free resource*/
    _asf_midxbuld_free_index_object( pt_midxbuld_obj, &t_index_object );

    return i4_ret;
}

static INT32  _asf_midxbuld_wma_indexing( 
                                                 MIDXBULD_KEY_INFO_T* pt_key_info, 
                                                 MIDXBULD_OBJ_T*      pt_midxbuld_obj )
{
    setpos_fct                  pf_set_pos ;    
    copybytes_fct               pf_copybyte;
    getpos_fct                  pf_get_pos;
    ASF_IDXBULD_OBJ_T*          pt_asf_handler = NULL;
    ASF_IDXBULD_RANGE_OBJ_T*    pt_range_obj = NULL;
    ASF_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj   = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry = NULL;
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_list = NULL; 
    UINT8*                      pui1_head    = NULL; /*read buffer pointer*/
    
    /*how many bytes are read from file*/
    UINT32                      ui4_read_size ;
    INT32                       i4_ret; /*return code*/
    UINT64                      ui8_cur_pos;

    UINT32                      ui4_cur_pkt_cnt;
    UINT32                      ui4_entry_pkt_cnt = 0;
    UINT32                      ui4_count; 
    UINT32                      ui4_per_packe_size;  /*per packe size*/
    UINT64                      ui8_pkg_count;
    UINT64                      ui8_data_obj_size;
    
    MIDXBULD_ELMT_TBL_DESCRIB_T t_describ;
    UCHAR                       ui1_hdr_flag;
    UCHAR                       ui1_padding_flag;
    UINT64                      ui8_cur_offset;
    UINT32                      ui4_time_code;
    
    if ((NULL == pt_key_info) || (NULL == pt_midxbuld_obj))
    {
        DBG_ERROR(("_asf_midxbuld_key_frame_table_alloc:NULL\r\n"));
        return MIDXBULDR_INV_ARG;
    }

    /*get callback*/
    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR(("_asf_midxbuld_wma_indexing:callback NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get index builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler)
    {
        DBG_ERROR(("_asf_midxbuld_wma_indexing:pt_asf_handler NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_asf_handler->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR(("_asf_midxbuld_wma_indexing:pui1_header_buf NULL\r\n"));
        return MIDXBULDR_INITIAL_ERR;
    }

    
    /*******************************paser object*******************************/
    /*get the first index table offset into the asf file*/
    ui8_cur_pos = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);


    /*get GUID and size*/
    i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                         pt_asf_handler->ui4_buf_length , 
                         ASF_OBJ_COMMON_LEN + 16 + 8 + 2, 
                         &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        DBG_ERROR(("_asf_midxbuld_wma_indexing:pf_copybyte "
                   "return %d\r\n",
                   ui4_read_size));
        return MIDXBULDR_INITIAL_ERR;
    }
 
    /*skip data object guid*/
    pui1_head += 16;
    
    /*Get total data object size, 8 bytes*/
    ASF_GET_QWORD(ui8_data_obj_size, pui1_head);
    pui1_head += 8;
    
    /*skip sub guid*/
    pui1_head += 16;
    
    /*Get total packet counts, 8 bytes*/
    ASF_GET_QWORD(ui8_pkg_count, pui1_head);
    pui1_head += 8;
   
    /*skip reserved, 2 bytes*/
    pui1_head += 2; 

    /*calculate packet size ,the fixed fields of Data Object are 50 bytes long*/
    if (0 != ui8_pkg_count)
    {
        ui4_per_packe_size = (UINT32)_mm_div64(ui8_data_obj_size - 50,
                               ui8_pkg_count, NULL);
        if (((UINT64)ui4_per_packe_size*ui8_pkg_count) < (ui8_data_obj_size - 50))  
        {
            ui4_per_packe_size++;
        }                     
    }
    else
    {
        DBG_INFO(("_asf_midxbuld_wma_indexing:warning:ui8_total_packets is zero\r\n"));
        ui4_per_packe_size = 0;
    }


    /*2.  translate<pts, packet num, packet count> to <pts,offset,size>*/
    i4_ret = MIDXBULDR_OK;
    do 
    {
        /*(1) allocate range and its table objects*/
        do
        {
            /* allocate range obj memory */
            pt_range_obj = (ASF_IDXBULD_RANGE_OBJ_T*)
                            _mm_util_mem_alloc_with_tag(sizeof(ASF_IDXBULD_RANGE_OBJ_T),
                            MM_MEM_MIDX_TAG);
            if (NULL == pt_range_obj)
            {
                DBG_ERROR(("_asf_midxbuld_wma_indexing:failed to allocate "
                           "memory for pt_range_obj\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break ;
            }

            /*allocate id for pt_range_obj*/
            if (TRUE != _asf_midxbuld_alloc_range_id(pt_midxbuld_obj, 
                                                     &pt_key_info->ui4_range_id))
            {
                DBG_ERROR(("_asf_midxbuld_wma_indexing: fail to "
                           "alloc range id\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_HANDLE;
                break ;
            }
            x_memset(pt_range_obj, 0, sizeof(ASF_IDXBULD_RANGE_OBJ_T));
            pt_range_obj->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
            pt_range_obj->u_range.t_es.ui4_start_pts = 0;
            pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
            pt_range_obj->ui4_range_id = pt_key_info->ui4_range_id;
            pt_range_obj->b_is_key_frame = TRUE;
            pt_range_obj->b_index_is_valid = FALSE;

            /* allocate elmt tbl obj memory */
            pt_tbl_obj = (ASF_IDXBULD_ELMT_TBL_OBJ_T*) 
                          _mm_util_mem_alloc_with_tag(sizeof(ASF_IDXBULD_ELMT_TBL_OBJ_T),
                          MM_MEM_MIDX_TAG);
            if (NULL == pt_tbl_obj)
            {
                DBG_ERROR(("_asf_midxbuld_wma_indexing: failed to allocate "
                           "memory for pt_tbl_obj\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break ;
            }
            if (TRUE != _asf_midxbuld_alloc_table_id(pt_range_obj, 
                                                     &pt_key_info->ui4_tbl_id))
            {
                DBG_ERROR(("_asf_midxbuld_wma_indexing: fail to "
                           "alloc table id\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_HANDLE;
                break ;
            }
            x_memset(pt_tbl_obj, 0, sizeof(ASF_IDXBULD_ELMT_TBL_OBJ_T));
            /*set table id*/
            pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
            
            /*set the range related table pointer*/
            pt_range_obj->pt_tbl_obj = pt_tbl_obj;
            /*pt_range_obj->ui1_tbl_total = 1;*/

            t_describ.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_KEY;
            t_describ.u_info.t_es.ui4_strm_id = pt_key_info->ui4_strm_id;
            t_describ.u_info.t_es.ui4_scale = pt_key_info->ui4_scale;
            t_describ.u_info.t_es.ui4_rate = pt_key_info->ui4_rate;
            t_describ.u_info.t_es.ui4_sample_sz = pt_key_info->ui4_sample_sz;
            t_describ.u_info.t_es.ui4_avg_bytes_per_sec = 
                                            pt_key_info->ui4_avg_bytes_per_sec;
            /*stream descritption*/
            x_memcpy((VOID*)&(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info), 
                     (VOID*)&(t_describ.u_info.t_es),
                     sizeof(MIDXBULD_ELMT_STRM_TBL_INFO_T));
        }while (0);

        /*if error break*/
        if (i4_ret != MIDXBULDR_OK)
        {
            break;
        }
        if(pt_tbl_obj == NULL)
        {
            break;
        }

        pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_START;
        
        /*(2) physcical file offset = ui8_base_offset + ui4_relative_offset
         the packet begin = the object size + fixed length(50 bytes)*/
        do 
        {
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 
                       pt_midxbuld_obj->t_src_info.ui8_data_offset + 50;
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            pt_tbl_obj->ui4_tbl_size = ASF_WMA_KEY_TBL_ENTRY_MAX_CNT;

            ui8_cur_offset = pt_tbl_obj->t_elmt_tbl.ui8_base_offset;
            
            pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*)
                            _mm_util_mem_alloc_with_tag(
                            sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) 
                            * pt_tbl_obj->ui4_tbl_size,
                            MM_MEM_MIDX_TAG);
            if (NULL == pt_idx_entry)
            {             
                DBG_ERROR(("_asf_midxbuld_wma_indexing: memory"
                           "for pt_idx_entry\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break ;
            }
            /*set allocated memoty 0*/
            x_memset(pt_idx_entry, 0, 
                     sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) 
                         * pt_tbl_obj->ui4_tbl_size);
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;

            ui4_cur_pkt_cnt = 0;

            /*translate<pts, packet num, packet count> to <pts,offset,size> */
            for (ui4_count = 0; ui4_count < pt_tbl_obj->ui4_tbl_size; )
            {
                pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_offset, FM_SEEK_BGN);
                pui1_head = pt_asf_handler->pui1_header_buf;

                /*get packet header*/
                i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                                     pt_asf_handler->ui4_buf_length , 
                                     ASF_OBJ_PKG_HDR_SIZE, 
                                     &ui4_read_size);
                if (i4_ret != INPUTR_OK)
                {                    
                    DBG_ERROR(("_asf_midxbuld_wma_indexing:pf_copybyte "
                               "return %d\r\n",
                               ui4_read_size));
                    if(i4_ret != INPUTR_EOF)
                    {
                        i4_ret = MIDXBULDR_INITIAL_ERR;
                    }
                    else /*look eos as normal case*/
                    {
                        i4_ret = MIDXBULDR_OK;
                    }
                    break;
                }
             
                /*get packet header flag*/
                ui1_hdr_flag = *pui1_head;
                pui1_head += 1;
                if(0x82 != ui1_hdr_flag)
                {
                    DBG_ERROR(("wma packet header flag is error: != 0x82, key table partial completed, entry count:%d\n",
                        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry));
                    //return MIDXBULDR_INITIAL_ERR;
                    break;
                }
                
                /*skip 2 bytes*/
                pui1_head += 2;
                
                /*get padding flag*/
                ui1_padding_flag = *pui1_head;
                pui1_head += 1;

                /*skip segment type ID*/
                pui1_head += 1;

                /*skip packet size field*/
                if(0x40 == ui1_padding_flag)
                {
                    pui1_head += 2;
                }

                /*skip padding size field*/
                if(0x10 == ui1_padding_flag)
                {
                    pui1_head += 2;
                }
                else if(0x08 == ui1_padding_flag)
                {
                    pui1_head += 1;
                }
                
                ui4_time_code = pui1_head[3] << 24 |
                                pui1_head[2] << 16 |
                                pui1_head[1] << 8  |
                                pui1_head[0]; /*ms*/

                if(((ui4_time_code / 1000 ) >= ui4_count) &&
                   ((ui4_time_code / 1000 ) < (ui4_count + 5))/*filter wrong pts of multi stream in one wma file*/)
                {                   
                    pt_idx_entry->ui4_pts = ui4_time_code * 90;
                    pt_idx_entry->ui4_relative_offset = 
                        (UINT32)(ui8_cur_offset - pt_tbl_obj->t_elmt_tbl.ui8_base_offset);
                    if(ui4_count > 0)
                    {
                        (pt_idx_entry - 1)->ui4_size = 
                            ui4_entry_pkt_cnt * ui4_per_packe_size;                        
                    }
                    ui4_entry_pkt_cnt = 0;
                    
                    pt_idx_entry ++;   /*pt_idx_entry*/
                    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry ++;
                    if(pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry >= ASF_WMA_KEY_TBL_ENTRY_MAX_CNT)
                    {
                        break;
                    }
                    
                    if (ui4_count > pt_tbl_obj->ui4_tbl_size / 2)
                    {
                        pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_MID;
                    }

                    ui4_count ++;
                }
                ui4_entry_pkt_cnt ++;
                ui4_cur_pkt_cnt ++;
                if(ui4_cur_pkt_cnt >= (UINT32)ui8_pkg_count)
                {
                    break;
                }
                ui8_cur_offset += ui4_per_packe_size;
                if(ui8_cur_offset > pt_tbl_obj->t_elmt_tbl.ui8_base_offset + ui8_data_obj_size)
                {
                    break;
                }
            }  
            if (i4_ret != MIDXBULDR_OK)
            {
                break;
            }
            pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_END;
            pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
            /*set the size of last entry*/
            if(pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry >= 1)
            {
                pt_idx_entry[pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry - 1].ui4_size = ui4_entry_pkt_cnt * ui4_per_packe_size;
            }
            

            /*(3) set key table info*/
            pt_asf_handler->t_key_table_obj.ui4_key_tbl_id = pt_key_info->ui4_tbl_id;
            pt_asf_handler->t_key_table_obj.pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)
                            _mm_util_mem_alloc_with_tag(
                            sizeof(MIDXBULD_KEY_TBL_EXT_T) ,
                            MM_MEM_MIDX_TAG);
            if (NULL == pt_asf_handler->t_key_table_obj.pt_key_tbl_ext)
            {
                DBG_ERROR(("_asf_midxbuld_key_frame_table_alloc: memory"
                           "for pt_key_tbl_list\r\n"));
                i4_ret = MIDXBULDR_OUT_OF_MEMORY;
                break ;
            }
            pt_key_tbl_list = pt_asf_handler->t_key_table_obj.pt_key_tbl_ext;
            x_memset(pt_key_tbl_list, 0, sizeof(MIDXBULD_KEY_TBL_EXT_T));
            pt_key_tbl_list->t_elmt_tbl = pt_tbl_obj->t_elmt_tbl;
            pt_key_tbl_list->pt_next = NULL;
            pt_asf_handler->b_key_tbl_ok = TRUE;
            
            /*(4) set range*/
            pt_asf_handler->apt_tbl_obj_map[0] = pt_tbl_obj;
            pt_asf_handler->pt_curr_range_obj  = pt_range_obj;
            pt_asf_handler->pt_key_range_obj   = pt_range_obj;
                
        }while(0);

        if (i4_ret != MIDXBULDR_OK)
        {
            if(pt_tbl_obj)
            {
                if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry)
                {
                    _mm_util_mem_free_with_tag(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, MM_MEM_MIDX_TAG);
                    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
                }
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                pt_tbl_obj = NULL;
            }
            
            if(pt_range_obj)
            {
                _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
                pt_range_obj = NULL;
            }           

            #if 0
            (VOID)_asf_midxbuld_free_table_obj(pt_midxbuld_obj,
                                               pt_key_info->ui4_tbl_id);
            (VOID)_asf_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                               pt_key_info->ui4_range_id);
            #endif
            pt_key_info->ui4_tbl_id   = MIDXBULD_NULL_ID;
            pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
            
            break;
        }
        else
        {
            for (ui4_count = 0; ui4_count < pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry; ui4_count ++)
            {
                DBG_ERROR(("count is %d: pts is %d, offset is %d, size is %d\n", 
                    ui4_count, pt_idx_entry->ui4_pts, pt_idx_entry->ui4_relative_offset, pt_idx_entry->ui4_size));

                pt_idx_entry ++;
            }  
        }
        pt_range_obj->b_index_is_valid = TRUE;

        /*normal, but still need free pt_index_entry so break*/

    }while(0);
        

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_simple_index_object_indexing
 *
 * Description: This function paser simple index objects 
 *
 * Inputs:  
 *         pt_key_info:     specifies which stream should build key frame table
 *         pt_midxbuld_obj: specify the media info object  
 *
 * Outputs: NULL 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the 
 *                           operation has been completed.
 *          Other non-zero values: Fail
 * Note:  
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
static INT32 _asf_midxbuld_simple_index_object_indexing( 
                                           MIDXBULD_KEY_INFO_T* pt_key_info, 
                                           MIDXBULD_OBJ_T*      pt_midxbuld_obj )
{
    setpos_fct                  pf_set_pos ;    
    copybytes_fct               pf_copybyte;
    getpos_fct                  pf_get_pos;
    ASF_IDXBULD_OBJ_T*          pt_asf_handler;
    UINT8*                      pui1_head; /*read buffer pointer*/
    UINT32                      ui4_read_size ;
    INT32                       i4_ret; /*return code*/
    UINT64                      ui8_cur_pos;
    /*UINT64                      ui8_idx_total_size;  */
    
    ASF_SIMPLE_INDEX_OBJECT_T   t_simple_index_object;
    BOOL                        b_found_simple_idx = FALSE;
#ifdef CLI_LVL_ALL
    CHAR                        ac_temp[40];      /*GUID str temp buffer*/
    UINT32                      ui4_len;          /*GUID str temp buffer length*/
#endif  
    
    /**************************************************************************
                        Check parameters
    **************************************************************************/
    if ((NULL == pt_key_info) || (NULL == pt_midxbuld_obj))
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INV_ARG;
    }

    /*init local variables*/
    x_memset( ( VOID * )&t_simple_index_object, 0, sizeof( ASF_SIMPLE_INDEX_OBJECT_T ) );
    /*ui4_simp_idx_obj_num = 1;*/  /*1 is the first valid stream number*/
    /*ui8_idx_total_size   = 0; index total size*/
    
    /*get callback*/
    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos  = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;
    if ((NULL == pf_copybyte) || (NULL == pf_set_pos) || (NULL == pf_get_pos))
    {
        DBG_ERROR( ("%s#Line%d:callback is  NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get index builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler)
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler NULL\r\n",__FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*get parser buffer*/
    pui1_head = pt_asf_handler->pui1_header_buf;
    if (NULL == pui1_head)
    {
        DBG_ERROR( ( "%s#Line%d:pui1_header_buf NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }

    
    /*******************************paser object*******************************/
    /*get the first index table offset into the asf file*/
    ui8_cur_pos = pt_midxbuld_obj->t_src_info.ui8_idx_offset;
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);

    /*load object header from file*/
    while ((ui8_cur_pos < pt_midxbuld_obj->z_buff_leng)
            /*&& ( ui8_idx_total_size < pt_midxbuld_obj->t_src_info.ui8_idx_size ) */) 
    {
        /*get GUID and size*/
        i4_ret = pf_copybyte(pt_midxbuld_obj, pui1_head, 
                             pt_asf_handler->ui4_buf_length , 
                             ASF_OBJ_COMMON_LEN, 
                             &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            DBG_ERROR( ( "%s#Line%d:pf_copybyte return %d\r\n",
                         __FUNCTION__, __LINE__, ui4_read_size ) );
            b_found_simple_idx = FALSE;
            break;
        }
     
        /*get object guid and size*/
        _asf_get_guid(&(t_simple_index_object.t_guid), pui1_head);
         /*GUID is 16 bytes long*/ 
        ASF_GET_QWORD(t_simple_index_object.ui8_size, pui1_head + 16);  
        
        /*if not simple index object just pass this is abnormal path*/
        if ( ASFR_GUID_EQUAL != _asf_cmp_guid( &( t_simple_index_object.t_guid ), 
                             &gt_asf_object_simple_index_guid))
        {
            #ifdef CLI_LVL_ALL
            /*dispay the object */
            x_memset( ac_temp, 0, sizeof( ac_temp ) );
            _asf_guid_to_str( &( t_simple_index_object.t_guid ), ac_temp, &ui4_len );
            x_dbg_stmt( "Object ID:%s\r\n", ac_temp );
            
            x_dbg_stmt( "Object Size:\t%llu\r\n",  t_simple_index_object.ui8_size );
            x_dbg_stmt( "File Position:\t%llu\r\n",  ui8_cur_pos );
            #endif
            ui8_cur_pos += t_simple_index_object.ui8_size;
            pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
            DBG_INFO( ( "%s#Line%d: may be index object followed by simple index object\r\n", 
                        __FUNCTION__, __LINE__ ) );
            
            continue;
        }

        /* Now only parse the first video index object,others are skipped
          ui4_simp_idx_obj_num is not used now......
        */
        
        /*restore current file position*/
        pf_set_pos(pt_midxbuld_obj, (INT64)ui8_cur_pos, FM_SEEK_BGN);
        pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_LOAD_SIMPLE_IDX_START;
        /*load simple index object, Now only supports one video situation*/
        i4_ret = _asf_midxbuld_load_simple_index_object(pt_midxbuld_obj, 
                                                        &t_simple_index_object);
        if (MIDXBULDR_OK == i4_ret)
        {   
            b_found_simple_idx = TRUE;    
            
            pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_LOAD_SIMPLE_IDX_END;
            break;
        }
        else
        {
            return MIDXBULDR_INITIAL_ERR;
        }
        /*not go here ,do not set position*/
        
    }

    /*If there exist simple index object, No return not ready*/
    if (FALSE == b_found_simple_idx)
    {
        /*There is no any key table*/
        _asf_midxbuld_free_simple_index_object( pt_midxbuld_obj, &t_simple_index_object );
        return MIDXBULDR_KEY_TBL_NOT_READY;
    }

    /*2.  translate<pts, packet num, packet count> to <pts,offset,size>*/
        pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_START;
    i4_ret = _asf_midxbuld_gen_key_frame_by_simple_index_object(
            pt_key_info, pt_midxbuld_obj, &t_simple_index_object );
    if ( MIDXBULDR_OK != i4_ret )
    {
        DBG_ERROR( ( "%s#Line%d:generate key frame error(%d)\r\n",
                     __FUNCTION__, __LINE__, i4_ret ) );
    }
    pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_CALC_ENTRY_END;

    /*3. free resource*/
    _asf_midxbuld_free_simple_index_object( pt_midxbuld_obj, &t_simple_index_object );

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_indexing
 *
 * Description: This function builds the index table actually
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *          pui4_range_id       specify which range
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
static INT32 _asf_midxbuld_indexing( MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                             UINT32*         pui4_range_id)
{
    return MIDXBULDR_OK;
}

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_get_start_info
 * Description: This function get index info
 * Inputs: 
 * Outputs:
 * Returns: MINFOR_OK    Success. The routine was successful and the operation 
 *                       has been completed.
 *          Other non-zero values: Fail
 ----------------------------------------------------------------------------*/
static INT32 _asf_midxbuld_get_start_info(MIDXBULD_OBJ_T*                  pt_midxbuld_obj,
                                         MIDXBULD_ELMT_TBL_TYPE_T         e_tbl_type,
                                         VOID*                            pt_start_info,
                                         SIZE_T                           z_info_size)
{
    ASF_IDXBULD_OBJ_T*          pt_asf_handler;
    ASF_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
    UINT32                      ui4_count;
    MIXBULD_START_INFO_T*       pt_strt_info        = (MIXBULD_START_INFO_T*)pt_start_info;

    if (pt_midxbuld_obj == NULL || pt_strt_info == NULL) 
    {
        return MIDXBULDR_INV_ARG;
    }
    
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_asf_handler == NULL)
    {
        return MIDXBULDR_INITIAL_ERR;
    }

    pt_tbl_obj = pt_asf_handler->pt_key_range_obj->pt_tbl_obj;

    while(pt_tbl_obj != NULL)
    {
        if (pt_tbl_obj->t_elmt_tbl.e_tbl_type != e_tbl_type)
        {
            pt_tbl_obj = pt_tbl_obj->pt_next;
            continue;
        }
        switch (e_tbl_type)
        {
        case MIDXBULD_TBL_TYPE_ELMT_STRM:
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry != NULL)
            {
                ui4_count = 0;
                while (ui4_count < pt_tbl_obj->ui4_tbl_size)
                {
                    if ((pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_count)->ui4_relative_offset == 0xFFFFFFFF)
                    {
                        ui4_count++;
                    }
                    else
                    {
                        break;
                    }
                }
                pt_strt_info->ui4_offset     = (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_count)->ui4_relative_offset;
                pt_strt_info->ui4_1st_pkgnum =
                    ((pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_count)->ui4_relative_offset)/pt_midxbuld_obj->t_src_info.ui4_packet_sz;
            }
            else
            {
                return MIDXBULDR_INITIAL_ERR;
            }
            return MIDXBULDR_OK;
        case MIDXBULD_TBL_TYPE_AUDIO_KEY:
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry != NULL)
            {
                ui4_count = 0;
                while (ui4_count < pt_tbl_obj->ui4_tbl_size)
                {
                    if ((pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_count)->ui4_relative_offset == 0xFFFFFFFF)
                    {
                        ui4_count++;
                    }
                    else
                    {
                        break;
                    }
                }
                pt_strt_info->ui4_offset     = (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_count)->ui4_relative_offset;
                pt_strt_info->ui4_1st_pkgnum =
                    ((pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + ui4_count)->ui4_relative_offset)/pt_midxbuld_obj->t_src_info.ui4_packet_sz;
            }
            else
            {
                return MIDXBULDR_INITIAL_ERR;
            }
            
            return MIDXBULDR_OK;
        default:
            return MIDXBULDR_INITIAL_ERR;
        }
    }
    
    if (pt_tbl_obj == NULL)
    {
        return MIDXBULDR_KEY_TBL_NOT_READY;
    }
    else
    {
        return MIDXBULDR_OK;
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  _asf_midxbuld_get_idx_elmt_tbl
 *
 * Description: This function get index info
 *
 * Inputs:  pt_midxbuld_obj, specifies asf index builder object
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
static INT32 _asf_midxbuld_get_idx_elmt_tbl( 
                                      MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                                     MIDXBULD_ELMT_TBL_INFO_T* pt_elmt_tbl)
{
    ASF_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    ASF_IDXBULD_OBJ_T*          pt_asf_handler;
    ASF_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    UINT8                       ui1_i;
    BOOL                        b_id_match = FALSE;
    
    /*check parameters*/
    if (NULL == pt_elmt_tbl)
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n",__FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INV_ARG;
    }

    /*get asf index builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler)
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INTERNAL_ERR;
    }

    /*if it in key range?*/
    if ( pt_asf_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == 
         pt_elmt_tbl->ui4_tbl_id )
    {
        b_id_match = TRUE;
        pt_tbl_obj = pt_asf_handler->pt_key_range_obj->pt_tbl_obj;
    }
    /*non key range array*/
    else if ((pt_asf_handler->ui1_range_count > 0)
              && (pt_asf_handler->ui1_range_count < ASF_MIDXBULD_RANGE_MAX))
    {
        for (ui1_i = 0; ui1_i < pt_asf_handler->ui1_range_count; ui1_i++)
        {
            pt_obj = pt_asf_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
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
    else
    {
        /*What ot do*/
    }

    /*not found table id*/
    if (FALSE == b_id_match)
    {
        DBG_INFO( ( "%s#Line%d: not found table id\r\n", __FUNCTION__, __LINE__ ) );
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
    pt_elmt_tbl->t_elmt_tbl.pt_next  = pt_tbl_obj->t_elmt_tbl.pt_next;
    
    DBG_INFO( ( "%s#Line%d: get table index done\r\n", __FUNCTION__, __LINE__ ) );
    return MIDXBULDR_OK;
}

#ifdef CLI_LVL_ALL
extern BOOL  _midxbuld_cli_asf_get_dump_flag(VOID);
static VOID _asf_midxbuld_dump_strm_info( MIDXBULD_ELMT_STRM_TBL_INFO_T* pt_strm_info )
{
    if ( !_midxbuld_cli_asf_get_dump_flag() )
    {
        return;
    }

    if ( !pt_strm_info )
    {
        x_dbg_stmt( "%s#Line%d:pt_strm_info is NULL\r\n", __FUNCTION__, __LINE__ );
        return;
    }
    
    x_dbg_stmt( "e_es_type               = %u\r\n", pt_strm_info->e_es_type );
    x_dbg_stmt( "ui4_strm_id             = %u\r\n", pt_strm_info->ui4_strm_id );
    x_dbg_stmt( "ui4_scale               = %u\r\n", pt_strm_info->ui4_scale );
    x_dbg_stmt( "ui4_sample_sz           = %u\r\n", pt_strm_info->ui4_sample_sz );
    x_dbg_stmt( "ui4_avg_bytes_per_sec   = %u\r\n", pt_strm_info->ui4_avg_bytes_per_sec );
    x_dbg_stmt( "ui4_indx_offset         = 0x%08x\r\n", pt_strm_info->ui4_indx_offset );
    x_dbg_stmt( "ui4_indx_size           = 0x%08x\r\n", pt_strm_info->ui4_indx_size );
    x_dbg_stmt( "ui8_timeunit            = %llu\r\n", pt_strm_info->ui8_timeunit );
    x_dbg_stmt( "ui8_granule_pos         = %llu\r\n", pt_strm_info->ui8_granule_pos );
    x_dbg_stmt( "ui4_avg_frame_num       = %u\r\n", pt_strm_info->ui4_avg_frame_num );
    x_dbg_stmt( "ui4_avg_packet_size     = %u\r\n", pt_strm_info->ui4_avg_packet_size );
    x_dbg_stmt( "b_is_cur_strm_codec_spt = %u\r\n", pt_strm_info->b_is_cur_strm_codec_spt );
    
    return;
}
static VOID _asf_midxbuld_dump_index_info( 
                                MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry,
                                UINT32 ui4_number_of_entry )
{
    UINT32           ui4_idx; 
    
    if ( !_midxbuld_cli_asf_get_dump_flag() )
    {
        return;
    }
    
    if ( !pt_idx_entry )
    {
        x_dbg_stmt( "%s#Line%d:pt_idx_entry is NULL\r\n", __FUNCTION__, __LINE__ );
        return;
    }

    for ( ui4_idx = 0; ui4_idx < ui4_number_of_entry; ++ui4_idx )
    {
        x_dbg_stmt( "Entry[%u]: PTS = %u;  Offset = 0x%08x ;  Size = 0x%08x\r\n", 
                    ui4_idx, 
                    pt_idx_entry[ui4_idx].ui4_pts,
                    pt_idx_entry[ui4_idx].ui4_relative_offset,
                    pt_idx_entry[ui4_idx].ui4_size );
    }
    
    return;
}

static VOID _asf_midxbuld_dump_tbl_info( MIDXBULD_ELMT_TBL_OBJ_T* pt_elmt_tbl )

{
    MIDXBULD_ELMT_STRM_TBL_INFO_T*         pt_strm_info;
    MIDXBULD_ELMT_TBL_OBJ_T*               pt_elmt_tbl_iter;
    MIDXBULD_ELMT_STRM_ENTRY_T*            pt_idx_entry;
    
    if ( !_midxbuld_cli_asf_get_dump_flag() )
    {
        return;
    }

    if ( !pt_elmt_tbl )
    {
        x_dbg_stmt( "%s#Line%d:pt_elmt_tbl is NULL\r\n", __FUNCTION__, __LINE__ );
        return;
    }
    pt_elmt_tbl_iter = pt_elmt_tbl;

    while ( pt_elmt_tbl_iter )
    {
        x_dbg_stmt( "ui8_base_offset     = 0x%08llx\r\n", pt_elmt_tbl_iter->ui8_base_offset );
        x_dbg_stmt( "ui4_number_of_entry = %u\r\n", pt_elmt_tbl_iter->ui4_number_of_entry );
        x_dbg_stmt( "e_tbl_type          = %s\r\n",
                    ( pt_elmt_tbl_iter->e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM )
                    ? "MIDXBULD_TBL_TYPE_ELMT_STRM" : "MIDXBULD_TBL_TYPE_AUDIO_KEY" );
        
        pt_strm_info = &( pt_elmt_tbl_iter->u_fmt.t_es.t_info );
        _asf_midxbuld_dump_strm_info( pt_strm_info );

        pt_idx_entry = pt_elmt_tbl_iter->u_fmt.t_es.pt_idx_entry;
        _asf_midxbuld_dump_index_info( pt_idx_entry, pt_elmt_tbl_iter->ui4_number_of_entry );
        
        pt_elmt_tbl_iter = pt_elmt_tbl_iter->pt_next;
    }

    return;
}

static VOID _asf_midxbuld_dump_range( ASF_IDXBULD_RANGE_OBJ_T* pt_range_obj )
{
    ASF_IDXBULD_ELMT_TBL_OBJ_T*        pt_tbl_obj;
    
    if ( !_midxbuld_cli_asf_get_dump_flag() )
    {
        return;
    }

    if ( !pt_range_obj )
    {
        x_dbg_stmt( "%s#Line%d:pt_range_obj is NULL\r\n", __FUNCTION__, __LINE__ );
        return;
    }
    
    x_dbg_stmt( "e_tbl_type     = %u\r\n", pt_range_obj->e_tbl_type );
    
    x_dbg_stmt( "Start PTS      = %u\r\n", pt_range_obj->u_range.t_es.ui4_start_pts );
    x_dbg_stmt( "End   PTS      = %u\r\n", pt_range_obj->u_range.t_es.ui4_end_pts );
    
    x_dbg_stmt( "ui4_range_id   = 0x%08x\r\n", pt_range_obj->ui4_range_id );
    
    x_dbg_stmt( "b_is_key_frame = %s\r\n",
                ( pt_range_obj->b_is_key_frame )? "TRUE" : "FALSE" );
    
    x_dbg_stmt( "ui1_tbl_total   = %u\r\n", pt_range_obj->ui1_tbl_total );

    pt_tbl_obj = pt_range_obj->pt_tbl_obj;
    while ( pt_tbl_obj )
    {
        x_dbg_stmt( "ui4_tbl_id   = 0x%08x\r\n", pt_tbl_obj->ui4_tbl_id );
        _asf_midxbuld_dump_tbl_info( &( pt_tbl_obj->t_elmt_tbl ) ); 
        pt_tbl_obj = pt_tbl_obj->pt_next;
    }
    
    return;
}

static VOID _asf_midxbuld_dump_hdlr_info( MIDXBULD_OBJ_T* pt_midxbuld_obj )
{
    ASF_IDXBULD_OBJ_T*                     pt_asf_handler;
    MIDXBULD_KEY_TBL_EXT_T*                pt_key_tbl_obj;
    MIDXBULD_ELMT_TBL_OBJ_T*               pt_elmt_tbl;
    UINT32                                 ui4_idx;

    if ( !_midxbuld_cli_asf_get_dump_flag() )
    {
        return;
    }
    
    /*get index builder handler*/
    pt_asf_handler = ( ASF_IDXBULD_OBJ_T* )( pt_midxbuld_obj->pv_handler_obj );
    if ( !pt_asf_handler )
    {
        x_dbg_stmt( "%s#Line%d:pt_asf_handler is NULL\r\n", __FUNCTION__, __LINE__ );
        return;
    }
    
    /*range info*/
    _asf_midxbuld_dump_range( pt_asf_handler->pt_key_range_obj );
    for ( ui4_idx = 0; ui4_idx < pt_asf_handler->ui1_range_count ; ++ui4_idx )
    {
        _asf_midxbuld_dump_range( pt_asf_handler->apt_range_obj[ui4_idx] );
    }

    /*key table info*/
    x_dbg_stmt( "First ui4_key_tbl_id = 0x%08x\r\n",
                pt_asf_handler->t_key_table_obj.ui4_key_tbl_id );
    pt_key_tbl_obj = pt_asf_handler->t_key_table_obj.pt_key_tbl_ext;
    while ( pt_key_tbl_obj )
    {
        pt_elmt_tbl = &( pt_key_tbl_obj->t_elmt_tbl );
        _asf_midxbuld_dump_tbl_info( pt_elmt_tbl );
        /*next stream key table*/
        pt_key_tbl_obj = pt_key_tbl_obj->pt_next;
    }

    return;
}

#endif
static VOID _asf_midxbuld_free_hdlr_info ( MIDXBULD_OBJ_T* pt_midxbuld_obj )
{
    ASF_IDXBULD_OBJ_T*                     pt_asf_handler;
    MIDXBULD_KEY_TBL_EXT_T*                pt_key_tbl_obj;
    MIDXBULD_KEY_TBL_EXT_T*                pt_next_key_tbl_obj;
    
    if ( !pt_midxbuld_obj )
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__  ) );
        return;
    }
    
    /*get index builder handler*/
    pt_asf_handler = ( ASF_IDXBULD_OBJ_T* )( pt_midxbuld_obj->pv_handler_obj );
    if ( !pt_asf_handler )
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return;
    }
    
    /*free temporary buffer*/
    if ( pt_asf_handler->pui1_header_buf )
    {
        _mm_util_mem_free_with_tag( pt_asf_handler->pui1_header_buf, MM_MEM_MIDX_TAG );
        pt_asf_handler->pui1_header_buf = NULL;
    }

    /* free all range objects */
    if ( pt_asf_handler->pt_key_range_obj )
    {
        _asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                      pt_asf_handler->pt_key_range_obj->ui4_range_id );
    }
    while ( pt_asf_handler->ui1_range_count > 0 )
    {
        _asf_midxbuld_free_range_obj( pt_midxbuld_obj, 
                                      pt_asf_handler->apt_range_obj[0]->ui4_range_id );
    }

    /*free key info*/
    pt_key_tbl_obj = pt_asf_handler->t_key_table_obj.pt_key_tbl_ext;
    while ( pt_key_tbl_obj )
    {
        pt_next_key_tbl_obj = pt_key_tbl_obj->pt_next;
        _mm_util_mem_free_with_tag( pt_key_tbl_obj, MM_MEM_MIDX_TAG );
        pt_key_tbl_obj = pt_next_key_tbl_obj;
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  asf_midxbuld_fopen
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
INT32 asf_midxbuld_fopen(
                          CHAR*                ps_path_name, 
                          MIDXBULD_KEY_INFO_T* pt_key_info, 
                          VOID*                pv_obj )
{

#ifndef __NO_FM__

    INT32               i4_ret;
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    ASF_IDXBULD_OBJ_T*  pt_asf_handler;
    FM_FILE_INFO_T      t_fm_info;
    UINT32              ui4_idx;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == ps_path_name) || (NULL == pt_key_info))
    {
        DBG_ERROR( ( "%s#Line%d :parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*init local variables*/
    
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*set input functions*/
    pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_l_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_l_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;


    /*asf handler for index builder*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)
                   _mm_util_mem_alloc_with_tag(sizeof(ASF_IDXBULD_OBJ_T),
                                               MM_MEM_MIDX_TAG);
    if ( NULL == pt_asf_handler )
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_asf_handler, 0, sizeof(ASF_IDXBULD_OBJ_T));
    /*set asf handler*/
    pt_midxbuld_obj->pv_handler_obj = pt_asf_handler;

    /*allocate parser buffer*/
    pt_asf_handler->pui1_header_buf = (UINT8*)_mm_util_mem_alloc_with_tag(
                                       ASF_IDXBULD_INIT_PARSER_BUFF_LEN,
                                       MM_MEM_MIDX_TAG);
    if (NULL == pt_asf_handler->pui1_header_buf)
    {
        DBG_ERROR( ( "%s#Line%d:pui1_header_buf is NULL\r\n", __FUNCTION__, __LINE__ ) );
        _mm_util_mem_free_with_tag(pt_asf_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset( pt_asf_handler->pui1_header_buf, 0, ASF_IDXBULD_INIT_PARSER_BUFF_LEN );
    pt_asf_handler->ui4_buf_length         = ASF_IDXBULD_INIT_PARSER_BUFF_LEN;
    pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_BULD_IDX_START;

    pt_asf_handler->ui1_idx_range_id_last = ASF_MIDXBULD_START_RANGE_ID;

    i4_ret = x_fm_open( FM_ROOT_HANDLE, ps_path_name, FM_READ_ONLY, 
                        0777, FALSE, &pt_midxbuld_obj->h_file );
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR( ( "%s#Line%d:x_fm_open return %d\r\n", __FUNCTION__, __LINE__,
                   i4_ret));
        _mm_util_mem_free_with_tag( pt_asf_handler->pui1_header_buf, MM_MEM_MIDX_TAG );
        pt_asf_handler->pui1_header_buf = NULL;
        _mm_util_mem_free_with_tag(pt_asf_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
        return MIDXBULDR_FILE_ERR;
    }

    x_fm_get_info_by_handle(pt_midxbuld_obj->h_file, &t_fm_info);
    pt_midxbuld_obj->z_buff_leng = (SIZE_T)t_fm_info.ui8_size;

    /*index info:*/
    if ( _asf_bit_map_is_set( pt_key_info->aui1_idx_strm_map, 16, 128 ) )
    {
        pt_asf_handler->b_index_obj_exsit = TRUE;
    }
    for ( ui4_idx = 1; ui4_idx <= ASF_STREAM_NUM_MAX ; ++ui4_idx )
    {
        if ( _asf_bit_map_is_set( pt_key_info->aui1_idx_strm_map, 16, ui4_idx ) )
        {
            if (pt_asf_handler->ui2_simple_key_strm_cnt < ASF_STREAM_NUM_MAX)
            {
                pt_asf_handler->ui2_simple_key_strm_id[pt_asf_handler->ui2_simple_key_strm_cnt++]
                            = ( UINT16 )ui4_idx;
            }
            else
            {
                DBG_ERROR( ( "%s#Line%d:ui2_simple_key_strm_id is overflow\r\n", __FUNCTION__, __LINE__ ) );
                return MIDXBULDR_INTERNAL_ERR;
            }
        }
    }

    if ( ( pt_midxbuld_obj->t_media_format.e_media_type == MEDIA_TYPE_AUDIO_ES )
         && ( pt_midxbuld_obj->t_media_format.t_media_subtype.u.e_aud_subtype
              == MEDIA_AUD_SUBTYPE_WMA ) ) /*it's wma*/
    {
        i4_ret = _asf_midxbuld_wma_indexing(pt_key_info, pv_obj);
    }
    else
    {
        if ( pt_asf_handler->b_index_obj_exsit )
        {
            i4_ret = _asf_midxbuld_index_object_indexing( pt_key_info, pv_obj );
        }
        /*build key frame table,generally for video*/
        i4_ret = _asf_midxbuld_simple_index_object_indexing( pt_key_info, pv_obj );
    }
    
    if (i4_ret != MIDXBULDR_OK)
    {
        _mm_util_mem_free_with_tag( pt_asf_handler->pui1_header_buf, MM_MEM_MIDX_TAG );
        pt_asf_handler->pui1_header_buf = NULL;
        _mm_util_mem_free_with_tag(pt_asf_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;

        /*close file*/
        if (pt_midxbuld_obj->h_file)
        {
#ifndef __NO_FM__        
            x_fm_close(pt_midxbuld_obj->h_file);
#endif
            pt_midxbuld_obj->h_file = NULL_HANDLE;
        }
            return i4_ret;
    }
    else
    {
        #ifdef CLI_LVL_ALL
        _asf_midxbuld_dump_hdlr_info ( pt_midxbuld_obj );
        #endif
        pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_BULD_IDX_END;
        return MIDXBULDR_OK;
    }

#else

    return MIDXBULDR_INITIAL_ERR;

#endif
}

/*-----------------------------------------------------------------------------
 * Name:  asf_midxbuld_mopen
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
 *   (1)2008-4-01 : initial

 ----------------------------------------------------------------------------*/
INT32 asf_midxbuld_mopen(
                         VOID*                pv_mem_data,
                         SIZE_T               z_mem_leng,
                         MIDXBULD_KEY_INFO_T* pt_key_info,
                         VOID*                pv_obj)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    ASF_IDXBULD_OBJ_T*  pt_asf_handler;
    MEDIA_FEEDER_T*     pt_src_feeder;
    INT32               i4_ret;
    UINT64              ui8_clip_pos;
    UINT32              ui4_idx;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ( pv_mem_data == NULL ) || ( pv_obj == NULL ) )
    {
        DBG_ERROR( ( "%s#Line%d :parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*)pv_obj;

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
        i4_ret = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos( 
                    pt_src_feeder, 0, MEDIA_SEEK_END );
        if (MIDXBULDR_OK != i4_ret)
        {
            DBG_ERROR( ( "%s#Line%d:set error\r\n", __FUNCTION__, __LINE__ ) );
            return MIDXBULDR_INITIAL_ERR;
        }
        
        /*get the end position*/
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos(pt_src_feeder, &ui8_clip_pos);
        pt_midxbuld_obj->z_buff_leng = (SIZE_T)ui8_clip_pos;

        
    }
    else
    {
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1    = mm_util_minput1bytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3    = mm_util_minput3bytes_l_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4    = mm_util_minput4bytes_l_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos   = mm_util_msetpos_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos   = mm_util_mgetpos_fct;
    }

    pt_midxbuld_obj->i8_buff_pos = 0;

    /*allocate memory for idex builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)_mm_util_mem_alloc_with_tag(
                     sizeof( ASF_IDXBULD_OBJ_T ), MM_MEM_MIDX_TAG );
    if (NULL == pt_asf_handler)
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_asf_handler, 0, sizeof(ASF_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_asf_handler;

    /*allocate init memory for index builder*/
    pt_asf_handler->pui1_header_buf = (UINT8*) _mm_util_mem_alloc_with_tag(
                  ASF_IDXBULD_INIT_PARSER_BUFF_LEN, MM_MEM_MIDX_TAG );
    if (NULL == pt_asf_handler->pui1_header_buf)
    {
        DBG_ERROR( ( "%s#Line%d:pui1_header_buf is NULL\r\n", __FUNCTION__, __LINE__ ) );
        _mm_util_mem_free_with_tag(pt_asf_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset (pt_asf_handler->pui1_header_buf, 0, ASF_IDXBULD_INIT_PARSER_BUFF_LEN );
    pt_asf_handler->ui4_buf_length         = ASF_IDXBULD_INIT_PARSER_BUFF_LEN;
    pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_BULD_IDX_START;
    
    pt_asf_handler->ui1_idx_range_id_last = ASF_MIDXBULD_START_RANGE_ID;

    /*index info:*/
    if ( _asf_bit_map_is_set( pt_key_info->aui1_idx_strm_map, 16, 128 ) )
    {
        pt_asf_handler->b_index_obj_exsit = TRUE;
    }
    for ( ui4_idx = 1; ui4_idx <= ASF_STREAM_NUM_MAX ; ++ui4_idx )
    {
        if ( _asf_bit_map_is_set( pt_key_info->aui1_idx_strm_map, 16, ui4_idx ) )
        {
            if (pt_asf_handler->ui2_simple_key_strm_cnt < ASF_STREAM_NUM_MAX)
            {
                pt_asf_handler->ui2_simple_key_strm_id[pt_asf_handler->ui2_simple_key_strm_cnt++]
                                          = ( UINT16 )ui4_idx;
            }
            else
            {
                DBG_ERROR( ( "%s#Line%d:ui2_simple_key_strm_id is overflow\r\n", __FUNCTION__, __LINE__ ) );
                return MIDXBULDR_INTERNAL_ERR;
            }        
        }
    }

    if ( ( pt_midxbuld_obj->t_media_format.e_media_type == MEDIA_TYPE_AUDIO_ES )
         && ( pt_midxbuld_obj->t_media_format.t_media_subtype.u.e_aud_subtype 
              == MEDIA_AUD_SUBTYPE_WMA ) ) /*it's wma*/
    {
        i4_ret = _asf_midxbuld_wma_indexing(pt_key_info, pv_obj);
    }
    else
    {
        if ( pt_asf_handler->b_index_obj_exsit )
        {
            i4_ret = _asf_midxbuld_index_object_indexing( pt_key_info, pv_obj );
        }
        /*build key frame table,generally for video*/
        i4_ret = _asf_midxbuld_simple_index_object_indexing( pt_key_info, pv_obj );
    }
    
    if (MIDXBULDR_OK != i4_ret)
    {
        _mm_util_mem_free_with_tag( pt_asf_handler->pui1_header_buf, MM_MEM_MIDX_TAG );
        pt_asf_handler->pui1_header_buf = NULL;
        _mm_util_mem_free_with_tag(pt_asf_handler, MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
        return i4_ret;
    }
    
    #ifdef CLI_LVL_ALL
    _asf_midxbuld_dump_hdlr_info ( pt_midxbuld_obj );
    #endif
    pt_asf_handler->ui1_curr_buld_progress = ASF_MIDXBULD_BULD_IDX_END;
    return MIDXBULDR_OK;    
}    

/*-----------------------------------------------------------------------------
 * Name:  asf_midxbuld_close
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
INT32 asf_midxbuld_close(VOID* pv_obj)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    ASF_IDXBULD_OBJ_T*  pt_asf_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (NULL == pv_obj)  
    {
        DBG_ERROR( ( "%s#Line%d :parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*get index object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*get asf inde handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler)
    {
        DBG_ERROR( ( "%s#Line%d: pt_asf_handler is  NULL\r\n", __FUNCTION__, __LINE__ ) );
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
    
    _asf_midxbuld_free_hdlr_info( pt_midxbuld_obj );
    
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
 * Name:  asf_midxbuld_start_indexing
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
INT32 asf_midxbuld_start_indexing(VOID* pv_obj, UINT32* pui4_range_id)
{
    MIDXBULD_OBJ_T* pt_midxbuld_obj = NULL;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((NULL == pv_obj) || (NULL == pui4_range_id))
    {
        DBG_ERROR( ( "%s#Line%d:parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*build index array*/
    return _asf_midxbuld_indexing(pt_midxbuld_obj, pui4_range_id);
} 

/*-----------------------------------------------------------------------------
 * Name:  asf_midxbuld_get_info
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
 * Note:  NULL
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-4-01 : initial

 ----------------------------------------------------------------------------*/
INT32 asf_midxbuld_get_info(
                            VOID*                    pv_obj,   
                            MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                            VOID*                    pv_buf,     
                            SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    ASF_IDXBULD_OBJ_T*  pt_asf_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ( pv_obj == NULL ) || ( pz_buf_leng== NULL ) )
    {
        DBG_ERROR( ( "%s#Line%d :parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* Do                                                                   */
    /************************************************************************/
    /*get index builder object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*get asf index builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler) 
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    switch(e_info_type)
    {
        case MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL:
            if (*pz_buf_leng < sizeof(MIDXBULD_ELMT_TBL_INFO_T))
            {
                DBG_ERROR( ("%s#Line%d:less length(%ld)\r\n",__FUNCTION__, __LINE__,
                           *pz_buf_leng));
                return MIDXBULDR_INITIAL_ERR;
            }
            *pz_buf_leng = sizeof(MIDXBULD_ELMT_TBL_INFO_T);
            return _asf_midxbuld_get_idx_elmt_tbl(pt_midxbuld_obj,
                         (MIDXBULD_ELMT_TBL_INFO_T*)pv_buf);
            
        case MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST:
            if (FALSE != pt_asf_handler->b_key_tbl_ok)
            {
                *pz_buf_leng = sizeof(MIDXBULD_KEY_TBL_INFO_T);
                x_memcpy((VOID*)pv_buf, 
                         (VOID*)&(pt_asf_handler->t_key_table_obj),
                         *pz_buf_leng);
                return MIDXBULDR_OK;   
            }
            else
            {
                return MIDXBULDR_KEY_TBL_NOT_READY;
            }
            
        case MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS:
            *(UINT8*)pv_buf = pt_asf_handler->ui1_curr_buld_progress;
            return MIDXBULDR_OK;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT            
        case MIDXBULD_GET_INFO_TYPE_AUDIO_START_INFO:
        {
            MIXBULD_START_INFO_T       t_aud_strt_info;
            x_memset(&t_aud_strt_info, 0, sizeof(MIXBULD_START_INFO_T));
            
            _asf_midxbuld_get_start_info( pv_obj, 
                                          MIDXBULD_TBL_TYPE_ELMT_STRM,
                                          &t_aud_strt_info,
                                          sizeof(MIXBULD_START_INFO_T));
            
            _asf_midxbuld_get_start_info( pv_obj, 
                                          MIDXBULD_TBL_TYPE_ELMT_STRM,
                                          pv_buf,
                                          *pz_buf_leng); 
            if (t_aud_strt_info.ui4_1st_pkgnum == 0/*no index table*/)
            {
                ((MIXBULD_START_INFO_T*)pv_buf)->ui4_1st_pkgnum = pt_midxbuld_obj->t_src_info.ui4_1st_pkgnum;
            }
            else
            {
                ((MIXBULD_START_INFO_T*)pv_buf)->ui4_1st_pkgnum = t_aud_strt_info.ui4_1st_pkgnum;
            }

            if (((MIXBULD_START_INFO_T*)pv_buf)->ui4_offset == 0/*no index table and simple index table*/)
            {
                ((MIXBULD_START_INFO_T*)pv_buf)->ui4_offset = pt_midxbuld_obj->t_src_info.ui8_data_offset;
            }

            return MIDXBULDR_OK;
        }

        case MIDXBULD_GET_INFO_TYPE_VIDEO_START_INFO:
            return _asf_midxbuld_get_start_info( pv_obj, 
                                                 MIDXBULD_TBL_TYPE_ELMT_STRM,
                                                 pv_buf,
                                                 *pz_buf_leng);   
#endif            
        default:
             return MIDXBULDR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  asf_midxbuld_set_info
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
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/
INT32 asf_midxbuld_set_info(
                            VOID*                    pv_obj,   
                            MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                            VOID*                    pv_buf,     
                            SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    ASF_IDXBULD_OBJ_T*  pt_asf_handler;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( ( pv_obj == NULL ) || ( pz_buf_leng == NULL ) )
    {
        DBG_ERROR( ( "%s#Line%d :parameter is NULL\r\n", __FUNCTION__, __LINE__ ) );
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    /*get index builder object*/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    /*get asf index builder handler*/
    pt_asf_handler = (ASF_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (NULL == pt_asf_handler) 
    {
        DBG_ERROR( ( "%s#Line%d:pt_asf_handler NULL\r\n", __FUNCTION__, __LINE__ ) );
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
