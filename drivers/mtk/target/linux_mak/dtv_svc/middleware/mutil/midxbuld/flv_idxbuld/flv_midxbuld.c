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
 * $RCSfile: flv_midxbuld.c,v $
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
#include "mutil/midxbuld/flv_idxbuld/flv_midxbuld.h"
#include "dbg/x_dbg.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "util/x_lnk_list.h"
#include "mutil/mm_util.h"
#include "svctx/svctx_dbg.h"

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
#include "flv_midxbuld.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_util.h"
#include "../../mm_util.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

static UINT8        ui1_idx_range_id_last = 1;

#define FLV_MIDXBULD_RANGE_ID_MASK                          ((UINT32)0xff00)

#define FLV_MIDXBULD_ESTIMATE_KEY_SIZE_START                ((UINT8)5)
#define FLV_MIDXBULD_ESTIMATE_KEY_SIZE_MID                  ((UINT8)10)
#define FLV_MIDXBULD_ESTIMATE_KEY_SIZE_END                  ((UINT8)15)
#define FLV_MIDXBULD_INDEXING_START                         ((UINT8)20)
#define FLV_MIDXBULD_INDEXING_VIDEO_START                   ((UINT8)25)
#define FLV_MIDXBULD_INDEXING_VID_DUR                       ((UINT8)60)
#define FLV_MIDXBULD_INDEXING_VIDEO_END                     ((UINT8)85)
#define FLV_MIDXBULD_INDEXING_END                           ((UINT8)100)

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static UINT32 flv_midxbuld_key_tbl_size_estimate(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_key_id);
                                   
                              
static UINT32 flv_midxbuld_elmt_tbl_size_estimate(
                        MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                        FLV_IDXBULD_RANGE_OBJ_T*     pt_range_obj,
                        MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ);

static INT32 flv_midxbuld_get_idx_elmt_tbl(
                        MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                        MIDXBULD_ELMT_TBL_INFO_T*    pt_elmt_tbl);

static INT32 flv_midxbuld_get_key_idx_tbl_list(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_KEY_TBL_INFO_T*  pt_key_tbl);

static INT32 flv_midxbuld_alloc_range(
                        MIDXBULD_OBJ_T*                 pt_midxbuld_obj, 
                        MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc);
                              
static INT32 flv_midxbuld_free_range(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id);


static BOOL _flv_midxbuld_alloc_table_id (
                        FLV_IDXBULD_RANGE_OBJ_T* pt_range_obj,
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

static BOOL _flv_midxbuld_alloc_range_id (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32*                  pui4_range_id )
{
    FLV_IDXBULD_OBJ_T* pt_flv_handler;
    UINT8              ui1_key_range_id = 0;
    UINT8              ui1_range_id;

    pt_flv_handler = pt_midxbuld_obj->pv_handler_obj;

    *pui4_range_id = MIDXBULD_NULL_ID;

    if (pt_flv_handler->pt_key_range_obj != NULL)
    {
        ui1_key_range_id = (UINT8)((pt_flv_handler->pt_key_range_obj->ui4_range_id & FLV_MIDXBULD_RANGE_ID_MASK) >> 8);
    }
    else if(pt_flv_handler->ui1_range_count > 0) /* key range should be allocated first */
    {
        return FALSE;
    }

    if (pt_flv_handler->ui1_range_count < 0xff)
    {
        UINT8 ui1_i;
        BOOL  b_occupied = TRUE;

        while (b_occupied)
        {
            b_occupied = FALSE;
            for (ui1_i = 0; ui1_i < pt_flv_handler->ui1_range_count; ui1_i++)
            {
                if (ui1_i >= FLV_MIDXBULD_RANGE_MAX)
                {
                    break;
                }
                ui1_range_id = (UINT8)((pt_flv_handler->apt_range_obj[ui1_i]->ui4_range_id & FLV_MIDXBULD_RANGE_ID_MASK) >> 8);
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
    
        ui1_idx_range_id_last = (UINT8)((ui1_idx_range_id_last == 0xff) ? 1 : (ui1_idx_range_id_last + 1));

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

static BOOL _flv_midxbuld_free_table_obj (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_tbl_id)
{
    FLV_IDXBULD_OBJ_T*          pt_flv_handler;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_obj = NULL;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_last_obj = NULL;
    UINT8                       ui1_free_range_id, ui1_range_id, ui1_i;
    MIDXBULD_ELMT_TBL_OBJ_T*    pt_elmt;

    pt_flv_handler = pt_midxbuld_obj->pv_handler_obj;

    if (pt_flv_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == ui4_tbl_id)
    {
        pt_obj = pt_flv_handler->pt_key_range_obj->pt_tbl_obj;
        pt_flv_handler->pt_key_range_obj->ui1_tbl_total--;
        pt_flv_handler->pt_key_range_obj->pt_tbl_obj = pt_obj->pt_next;
    }
    else
    {
        ui1_free_range_id = (UINT8)((ui4_tbl_id & FLV_MIDXBULD_RANGE_ID_MASK) >> 8);
        
        for (ui1_i = 0; ui1_i < pt_flv_handler->ui1_range_count; ui1_i++)
        {
            if (ui1_i >= FLV_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            ui1_range_id = (UINT8)((pt_flv_handler->apt_range_obj[ui1_i]
                            ->ui4_range_id & FLV_MIDXBULD_RANGE_ID_MASK) >> 8);
            if (ui1_range_id == ui1_free_range_id)
            {
                BOOL b_id_match = FALSE;
            
                pt_obj = pt_flv_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
                if (pt_obj->ui4_tbl_id == ui4_tbl_id)
                {
                    b_id_match = TRUE;
                    pt_flv_handler->apt_range_obj[ui1_i]->pt_tbl_obj = pt_obj->pt_next;
                    pt_flv_handler->apt_range_obj[ui1_i]->ui1_tbl_total--;
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
                        pt_flv_handler->apt_range_obj[ui1_i]->ui1_tbl_total--;
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

    pt_elmt = &(pt_obj->t_elmt_tbl);
    while (pt_elmt != NULL)
    {
        if (pt_elmt->e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
        {
            if (pt_elmt->u_fmt.t_es.pt_idx_entry != NULL)
            {
                _mm_util_mem_free_with_tag(pt_elmt->u_fmt.t_es.pt_idx_entry,
                                       MM_MEM_MIDX_TAG);
            }
        }
        else /* (pt_elmt->e_tbl_type == MIDXBULD_TBL_TYPE_AUDIO_KEY) */
        {
            if (pt_elmt->u_fmt.t_as_key.pui4_pts != NULL)
            {
                _mm_util_mem_free_with_tag(pt_elmt->u_fmt.t_as_key.pui4_pts,
                                       MM_MEM_MIDX_TAG);
            }
        }
        pt_elmt = pt_elmt->pt_next;
    }
    _mm_util_mem_free_with_tag(pt_obj, MM_MEM_MIDX_TAG);
            
    return TRUE;
}

static BOOL _flv_midxbuld_free_range_obj (
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id)
{
    FLV_IDXBULD_OBJ_T*          pt_flv_handler;
    FLV_IDXBULD_RANGE_OBJ_T*    pt_range_obj = NULL;
    UINT8                       ui1_i = 0;
    BOOL                        b_id_match = FALSE;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    UINT32                      ui4_tbl_id = MIDXBULD_NULL_ID;

    pt_flv_handler = pt_midxbuld_obj->pv_handler_obj;

    if (pt_flv_handler->pt_key_range_obj->ui4_range_id == ui4_range_id)
    {
        b_id_match = TRUE;
        pt_range_obj = pt_flv_handler->pt_key_range_obj;
    }
    else if (pt_flv_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_flv_handler->ui1_range_count; ui1_i++)
        {
            if (ui1_i >= FLV_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            if (pt_flv_handler->apt_range_obj[ui1_i]->ui4_range_id == ui4_range_id)
            {
                b_id_match = TRUE;
                pt_range_obj = pt_flv_handler->apt_range_obj[ui1_i];

                break;
            }
        }
    }
    
    if (FALSE == b_id_match)
    {
        return FALSE;
    }

    if (pt_range_obj != NULL)
    {
        while (pt_range_obj->pt_tbl_obj != NULL)
        {
            pt_tbl_obj = pt_range_obj->pt_tbl_obj;
            ui4_tbl_id = pt_tbl_obj->ui4_tbl_id;
            _flv_midxbuld_free_table_obj(pt_midxbuld_obj, ui4_tbl_id);
        }
        
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);

        if (pt_flv_handler->pt_key_range_obj->ui4_range_id != ui4_range_id)
        {
            while ((ui1_i + 1) < pt_flv_handler->ui1_range_count)
            {
                if ((ui1_i + 1) >= FLV_MIDXBULD_RANGE_MAX)
                {
                    break;
                }
                pt_flv_handler->apt_range_obj[ui1_i] = pt_flv_handler->apt_range_obj[ui1_i + 1];
                pt_flv_handler->apt_range_obj[++ui1_i] = NULL;
            }
            pt_flv_handler->ui1_range_count--;
        }

        return TRUE;
    }

    return FALSE;
}

typedef enum
{
    VALUE_NUM,
    VALUE_BOOL,
    VALUE_STR,
    VALUE_OBJ,
    VALUE_CLIP,
    VALUE_NULL,
    VALUE_UNDEF,
    VALUE_REFER,
    VALUE_ECMA,
    VALUE_STRICT=10,
    VALUE_DATE,
    VALUE_LSTR
}E_VALUE_TYPE;

#define FLV_DATAOBJ_ENDCODE 0x000009
#define FLV_SAVE_DATA_BUF_LEN 1024

#define DOUBLE_PRECISION                30 /*the count of get number after "." of double*/
#define DBL_SIG_BITS                    (52 - DOUBLE_PRECISION)
#define FLV_KEY_TBL_PROC_FINISH              (0xFFFFFFFF)

#define FLV_TIMES_STR       "times"
#define FLV_FILEPOS_STR     "filepositions"

static BOOL  g_b_has_times  = FALSE;
static BOOL  g_b_has_filepos = FALSE;
static UINT32 g_ui4_idx_entry_cnt = 0;
static CHAR*  g_pac_save_buf = NULL;

#define GET_UINT64_VALUE(pui1_buf, ui8_value)  \
    ui8_value = (UINT64)pui1_buf[0] << 56 |    \
                (UINT64)pui1_buf[1] << 48 |    \
                (UINT64)pui1_buf[2] << 40 |    \
                (UINT64)pui1_buf[3] << 32 |    \
                (UINT64)pui1_buf[4] << 24 |    \
                (UINT64)pui1_buf[5] << 16 |    \
                (UINT64)pui1_buf[6] << 8  |    \
                (UINT64)pui1_buf[7];           \
    pui1_buf += 8;                             \
    ui4_cur_pos += 8;

#define GET_UINT32_VALUE(pui1_buf, ui4_value)  \
    ui4_value = (UINT32)pui1_buf[0] << 24 |    \
                (UINT32)pui1_buf[1] << 16 |    \
                (UINT32)pui1_buf[2] << 8  |    \
                (UINT32)pui1_buf[3];           \
    pui1_buf += 4;                             \
    ui4_cur_pos += 4;

#define GET_UINT24_VALUE(pui1_buf, ui4_value)  \
    ui4_value = (UINT32)pui1_buf[0] << 16 |    \
                (UINT32)pui1_buf[1] << 8  |    \
                (UINT32)pui1_buf[2];           \
    pui1_buf += 3;                             \
    ui4_cur_pos += 3;

#define GET_UINT16_VALUE(pui1_buf, ui2_value) \
    ui2_value = (UINT16)pui1_buf[0] << 8 |    \
                (UINT16)pui1_buf[1];          \
    pui1_buf += 2;                            \
    ui4_cur_pos += 2;

#define GET_STR_VALUE(pui1_buf, psz_str)      \
{                                             \
   UINT16 ui2_strlen;                         \
   GET_UINT16_VALUE(pui1_buf, ui2_strlen);    \
   if(ui2_strlen < FLV_SAVE_DATA_BUF_LEN)     \
   {                                          \
      x_memcpy(psz_str, pui1_buf, ui2_strlen);   \
      psz_str[ui2_strlen]=0;                  \
   }                                          \
   else                                       \
   {                                          \
      x_memcpy(psz_str, pui1_buf, FLV_SAVE_DATA_BUF_LEN);   \
      psz_str[FLV_SAVE_DATA_BUF_LEN]=0;       \
   }                                          \
   pui1_buf += (ui2_strlen);                  \
   ui4_cur_pos += (ui2_strlen);               \
}

#define GET_LSTR_VALUE(pui1_buf, psz_lstr)    \
{                                             \
   UINT32 ui4_strlen;                         \
   GET_UINT32_VALUE(pui1_buf, ui4_strlen);    \
   if(ui4_strlen < FLV_SAVE_DATA_BUF_LEN)     \
   {                                          \
      x_memcpy(psz_lstr, pui1_buf, ui4_strlen);  \
      psz_lstr[ui4_strlen]=0;                 \
   }                                          \
   else                                       \
   {                                          \
      x_memcpy(psz_lstr, pui1_buf, FLV_SAVE_DATA_BUF_LEN);   \
      psz_lstr[FLV_SAVE_DATA_BUF_LEN]=0;      \
   }                                          \
   pui1_buf += (ui4_strlen);                  \
   ui4_cur_pos += (ui4_strlen);               \
}

#define FLV_ALLOC_BUF()                 \
    if(g_pac_save_buf == NULL)          \
    {                                   \
        g_pac_save_buf = (CHAR*)x_mem_alloc(FLV_SAVE_DATA_BUF_LEN+1); \
        if(g_pac_save_buf == NULL)      \
        {                               \
            return MIDXBULDR_INTERNAL_ERR; \
        }                               \
    }

#define FLV_FREE_BUF()                  \
    if(g_pac_save_buf != NULL)          \
    {                                   \
        x_mem_free(g_pac_save_buf);     \
        g_pac_save_buf = NULL;          \
    }


static INT32 flv_midxbuld_script_dataobjects(UINT8* pui1_buffer, 
                   BOOL b_has_endcode, UINT32 ui4_obj_count,
                   MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry,
                   UINT32 ui4_data_size);
static INT32 flv_midxbuld_objectvalue(UINT8* pui1_buffer,
                   MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry,
                   UINT32 ui4_data_size);

static INT32 flv_midxbuld_dbl_2_int64(UINT8* aui1_db_value, INT64* pi8_out_val, UINT32 ui4_scale)
{
    UINT64 ui8_tail;
    INT32 i4_e_val;
    BOOL  b_sign = 0;

    if(NULL == aui1_db_value || NULL == pi8_out_val)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    if((UINT64)ui4_scale >= ((UINT64)1 << (64 - DOUBLE_PRECISION)))
    {   /*will over flow*/
        *pi8_out_val = 0;
        return MIDXBULDR_INTERNAL_ERR;
    }
    
    b_sign = (BOOL)(aui1_db_value[7] >> 7);

    i4_e_val = (INT32)(((((INT32)(aui1_db_value[7] & 0x7F)) << 4) & 0x7FF) | 
                        ((aui1_db_value[6] >> 4) & 0x0F));

    if(i4_e_val != 0x7FF)
    {
        i4_e_val -= (INT32)0x3FF;
        if((i4_e_val > 32) || (i4_e_val < (-5)))
        { /*will over flow*/
            *pi8_out_val = 0;
            return MIDXBULDR_INTERNAL_ERR;
        }
    }
    else
    {
        if(b_sign)
        {   /* - INF*/
            *pi8_out_val = (INT64)0xFFFFFFFFFFFFFFFF;
            return MIDXBULDR_OK;
        }
        else
        {
            /* + INF*/
            *pi8_out_val = (INT64)0x7FFFFFFFFFFFFFFF;
            return MIDXBULDR_OK;
        }
    }

    ui8_tail = (UINT64)1 << ((52 - DBL_SIG_BITS));
    if(DBL_SIG_BITS <= 48)
    {
        ui8_tail |= (((UINT64)aui1_db_value[6] & 0x0F) << ((48 - DBL_SIG_BITS) & 0x3F));
        if((48 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[5] >> 
                ((8 - (48 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 40)
    {
        ui8_tail |= (((UINT64)aui1_db_value[5]) << ((40 - DBL_SIG_BITS) & 0x3F));
        if((40 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[4] >> 
                ((8 - (40 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 32)
    {
        ui8_tail |= ((UINT64)aui1_db_value[4] << ((32 - DBL_SIG_BITS) & 0x1F));
        if((32 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[3] >> 
                ((8 - (32 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 24)
    {
        ui8_tail |= ((UINT64)aui1_db_value[3] << ((24 - DBL_SIG_BITS) & 0x1F));
        if((24 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[2] >> 
                ((8 - (24 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 16)
    {
        ui8_tail |= ((UINT64)aui1_db_value[2] << ((16 - DBL_SIG_BITS) & 0x0F));
        if((16 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[1] >> 
                ((8 - (16 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 8)
    {
        ui8_tail |= ((UINT64)aui1_db_value[1] << ((8 - DBL_SIG_BITS) & 0x07));
        if((8 - DBL_SIG_BITS) < 8)
        {
            ui8_tail |= (((UINT64)aui1_db_value[0] >> 
                ((8 - (8 - DBL_SIG_BITS)) & 0x07)) & 0xFF);
        }
    }
    if(DBL_SIG_BITS <= 0)
    {
        ui8_tail |= (UINT64)(aui1_db_value[0] & 0xFF);
    }

    if(i4_e_val >= 0)
    {
        ui8_tail <<= i4_e_val;
    }
    else
    {
        ui8_tail >>= (-i4_e_val);
    }
    ui8_tail *= ui4_scale;    
    ui8_tail += ((UINT64)1 << (DOUBLE_PRECISION - 1));
    ui8_tail >>= DOUBLE_PRECISION;
    if(b_sign)
    {
        *pi8_out_val = - (INT64)ui8_tail;
    }
    else
    {
        *pi8_out_val = (INT64)ui8_tail;
    }

    return MIDXBULDR_OK;

}

static INT32 flv_midxbuld_objectvalue(UINT8* pui1_buffer,
                 MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry,
                 UINT32 ui4_data_size)
{
    UCHAR*  pui1_data = pui1_buffer;
    UINT8   ui1_val_type;
	UINT64  ui8_fd_value;
	UINT32  ui4_array_cnt=0;
	INT32   i4_process_size=0;
    UINT32  ui4_cur_pos = 0;
	
    ui1_val_type=*pui1_data++;
    ui4_cur_pos ++;

    if(ui1_val_type==VALUE_NUM)
    {
        GET_UINT64_VALUE(pui1_data,ui8_fd_value);

        if(pt_key_entry)
        {
           if(g_b_has_times)
           {
                flv_midxbuld_dbl_2_int64((UINT8*)&ui8_fd_value, (INT64*)&ui8_fd_value, 90000);
                pt_key_entry->ui4_pts = (UINT32)ui8_fd_value;            
                pt_key_entry->ui4_size = 0;
           }
           else if(g_b_has_filepos)
           {
                flv_midxbuld_dbl_2_int64((UINT8*)&ui8_fd_value, (INT64*)&ui8_fd_value, 1);
                pt_key_entry->ui4_relative_offset = (UINT32)ui8_fd_value;
           }
        }
	}
	else if(ui1_val_type==VALUE_BOOL)
	{
        pui1_data ++;
        ui4_cur_pos ++;
	}
	else if(ui1_val_type==VALUE_STR || ui1_val_type==VALUE_CLIP)
	{
        GET_STR_VALUE(pui1_data, g_pac_save_buf);
	}
	else if(ui1_val_type==VALUE_OBJ || ui1_val_type==VALUE_ECMA || ui1_val_type==VALUE_STRICT)
	{

	     BOOL bHasEndCode=TRUE;
       
	     if(ui1_val_type==VALUE_ECMA ||ui1_val_type==VALUE_STRICT )
	     {
             GET_UINT32_VALUE(pui1_data, ui4_array_cnt);
             if(g_b_has_times)
             {
                g_ui4_idx_entry_cnt = ui4_array_cnt;
                if(NULL == pt_key_entry)
                {   /*only get key entry count*/
                    return (INT32)FLV_KEY_TBL_PROC_FINISH;
                }
             }
	     }

	     if(ui1_val_type==VALUE_STRICT)
	     {
             bHasEndCode=FALSE;
	     }
   
         i4_process_size=flv_midxbuld_script_dataobjects(pui1_data,bHasEndCode,
            ui4_array_cnt, pt_key_entry, ui4_data_size - ui4_cur_pos);
         if(i4_process_size == (INT32)FLV_KEY_TBL_PROC_FINISH)
         {
             return (INT32)FLV_KEY_TBL_PROC_FINISH;
         }
         if(i4_process_size > 0)
         {
            pui1_data += i4_process_size;
            ui4_cur_pos += i4_process_size;
         }
	}
	else if(ui1_val_type==VALUE_NULL)
	{
        pui1_data ++;
        ui4_cur_pos ++;
	}
    else if(ui1_val_type==VALUE_UNDEF)
    {
        //ui4_cur_pos = (UINT32)-1;
    }
	else if(ui1_val_type==VALUE_REFER)
	{
        pui1_data += sizeof(UINT16);
        ui4_cur_pos += sizeof(UINT16);
	}
	else if(ui1_val_type==VALUE_DATE)
	{
        /*GET_UINT64_VALUE(pui1_data,ui8_fd_value);
            pd8_value = (DOUBLE*)(&ui8_fd_value);
            d8_value = *pd8_value;
            GET_UINT16_VALUE(pui1_data, ui2_value1);*/

        pui1_data += (sizeof(UINT64) + sizeof(UINT16));
        ui4_cur_pos += (sizeof(UINT64) + sizeof(UINT16));
	}
	else if(ui1_val_type==VALUE_LSTR)
	{
        GET_LSTR_VALUE(pui1_data, g_pac_save_buf);
	}    

    return (INT32)ui4_cur_pos;
}

static INT32 flv_midxbuld_script_dataobjects(UINT8*  pui1_buffer,BOOL b_has_endcode,
       UINT32 ui4_obj_cnt, MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry, UINT32 ui4_data_size)
{
   UINT32  ui4_obj_end=0;
   INT32   i4_process_size=0;
   UCHAR*  pui1_data = pui1_buffer;
   UINT32  ui4_cur_pos = 0;
   MIDXBULD_ELMT_STRM_ENTRY_T* pt_cur_key_entry = NULL;
   
   if(b_has_endcode)
   {
       GET_UINT24_VALUE(pui1_data, ui4_obj_end);
       pui1_data -= 3; /*restore pointer*/
       ui4_cur_pos -= 3;
   }
   if(!b_has_endcode && !ui4_obj_cnt)
   {
       return 0;
   }
   while((i4_process_size >= 0) && 
         (ui4_obj_end != FLV_DATAOBJ_ENDCODE) && 
         (ui4_cur_pos < ui4_data_size))
   {
       if(b_has_endcode)
       {
           GET_STR_VALUE(pui1_data, g_pac_save_buf);

           if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_TIMES_STR))
           {
                g_b_has_times = TRUE;                
           }
           else if(!x_strcmp((const CHAR*)g_pac_save_buf, FLV_FILEPOS_STR))
           {
                g_b_has_filepos = TRUE;
           }
           pt_cur_key_entry = pt_key_entry;
       }
       else
       {
           if(pt_cur_key_entry == NULL)
           {
                pt_cur_key_entry = pt_key_entry;
           }           
       }
       
       i4_process_size = flv_midxbuld_objectvalue(pui1_data, 
                  pt_cur_key_entry, ui4_data_size - ui4_cur_pos);
       if(i4_process_size == (INT32)FLV_KEY_TBL_PROC_FINISH)
       {
           g_b_has_times = FALSE;
           g_b_has_filepos = FALSE;
           return (INT32)FLV_KEY_TBL_PROC_FINISH;
       }
       if(i4_process_size > 0)
       {
           pui1_data += i4_process_size;
           ui4_cur_pos += i4_process_size;
       }
       
       if(b_has_endcode)
       {
           GET_UINT24_VALUE(pui1_data, ui4_obj_end);
           pui1_data -= 3; /*restore pointer*/
           ui4_cur_pos -= 3;
       }
       else 
       {
          ui4_obj_cnt--;          
          if(ui4_obj_cnt == 0)
          {
             break;
          }

          if(g_b_has_times || g_b_has_filepos)
          {
             if(pt_cur_key_entry)
             {
                pt_cur_key_entry ++;
             }
          }
       }
   }

   g_b_has_times = FALSE;
   g_b_has_filepos = FALSE;
   
   if(b_has_endcode)
   {
       GET_UINT24_VALUE(pui1_data, ui4_obj_end);
   }
   return (INT32)ui4_cur_pos;
}


/* initialize key frame table obj for indexing */
static INT32 flv_midxbuld_key_frame_table_alloc(
                        MIDXBULD_KEY_INFO_T*     pt_key_info, 
                        VOID*                    pv_obj)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    FLV_IDXBULD_OBJ_T*          pt_flv_handler;
    FLV_IDXBULD_RANGE_OBJ_T*    pt_range_obj;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj_last;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj;
    MIDXBULD_ELMT_TBL_DESCRIB_T t_describ;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry;
    UINT8                       ui1_i;
    UINT8                       ui1_strm_num;
    UINT32                      ui4_key_num;
    UINT32                      ui4_tbl_id;
    /*UINT64                      ui8_temp;*/

    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;
    pt_flv_handler = (FLV_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;

    pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
    pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;

    /* allocate elmt tbl range obj memory */
    pt_range_obj = (FLV_IDXBULD_RANGE_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_RANGE_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_range_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (TRUE != _flv_midxbuld_alloc_range_id(pt_midxbuld_obj, &pt_key_info->ui4_range_id))
    {
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_flv_handler->pt_key_range_obj = pt_range_obj;
    
    x_memset(pt_range_obj, 0, sizeof(FLV_IDXBULD_RANGE_OBJ_T));
    pt_range_obj->e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->ui4_range_id = pt_key_info->ui4_range_id;
    pt_range_obj->b_is_key_frame = TRUE;
    pt_range_obj->b_index_is_valid = FALSE;
    pt_range_obj->ui1_tbl_total = 0;

    t_describ.u_info.t_es.e_es_type = MIDXBULD_ELEM_STRM_TYPE_KEY;
    t_describ.u_info.t_es.ui4_strm_id = pt_key_info->ui4_strm_id;
    t_describ.u_info.t_es.ui4_scale = pt_key_info->ui4_scale;
    t_describ.u_info.t_es.ui4_rate = pt_key_info->ui4_rate;
    t_describ.u_info.t_es.ui4_sample_sz = pt_key_info->ui4_sample_sz;
    t_describ.u_info.t_es.ui4_avg_bytes_per_sec = pt_key_info->ui4_avg_bytes_per_sec;

    /* allocate elmt tbl obj memory */
    pt_tbl_obj = (FLV_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_tbl_obj == NULL)
    {
        _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    if (TRUE != _flv_midxbuld_alloc_table_id(pt_range_obj, &pt_key_info->ui4_tbl_id))
    {
        _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
        _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_HANDLE;
    }

    pt_range_obj->pt_tbl_obj = pt_tbl_obj;
    pt_tbl_obj_last = pt_tbl_obj;

    x_memset(pt_tbl_obj, 0, sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T));
    pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
    pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
    ui4_key_num = flv_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, pt_range_obj, &t_describ);
    pt_tbl_obj->ui4_tbl_size = ui4_key_num;
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = t_describ.u_info.t_es;
    /*pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_start_frame_num = 0;
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_end_frame_num = 0;*/
    /* key num is 0, no need alloc memory */
    if(ui4_key_num == 0)
    {
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
        pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
        pt_flv_handler->pt_curr_range_obj = pt_range_obj;
        return MIDXBULDR_OK;
    }
    pt_idx_entry = 
        (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
            sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, 
            MM_MEM_MIDX_TAG);
    if (pt_idx_entry == NULL)
    {
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
        _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;

    pt_tbl_obj->ui8_last_chunk_num = 0;
    pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
    pt_tbl_obj->pt_next = NULL;

    ui1_strm_num = 0;
    if (ui1_strm_num >= FLV_MIDXBULD_IDX_STRM_NUM_MAX)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    
    /*pt_flv_handler->aui1_idx_strm_num_map[ui1_strm_num] = 0;
           pt_flv_handler->apt_tbl_obj_map[0] = pt_tbl_obj;*/
#if 1
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {        
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_tbl_obj = (FLV_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            if (TRUE != _flv_midxbuld_alloc_table_id(pt_range_obj, &ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }

            pt_tbl_obj_last->pt_next = pt_tbl_obj;
            pt_tbl_obj_last = pt_tbl_obj;
            x_memset(pt_tbl_obj, 0, sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T));
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = ui4_key_num;
            pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_AUDIO_KEY;
            pt_tbl_obj->ui4_tbl_size = ui4_key_num;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.e_es_type 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_strm_id 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_strm_id;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_scale 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_scale;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_rate 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_rate;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_sample_sz 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_sample_sz;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info.ui4_avg_bytes_per_sec 
                = pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].ui4_avg_bytes_per_sec;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts 
                = (UINT32*) _mm_util_mem_alloc_with_tag(sizeof(UINT32) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts == NULL)
            {
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts = NULL;
                _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
                pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
                pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts, 
                     0, 
                     sizeof(UINT32) * pt_tbl_obj->ui4_tbl_size);

            pt_tbl_obj->ui8_last_chunk_num = 0;
            pt_tbl_obj->ui4_tbl_id = ui4_tbl_id;
            pt_tbl_obj->pt_next = NULL;
        }
    }
#endif
    
    pt_flv_handler->pt_curr_range_obj = pt_range_obj;

    pt_flv_handler->ui8_resume_pos = 0;
    pt_flv_handler->ui8_resume_size = 0;

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  flv_midxbuld_fopen
 *
 * Description: This function open a handler for a file
 *
 * Inputs:  ps_path_name specify the path name of the file
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 flv_midxbuld_fopen(
                        CHAR*                    ps_path_name, 
                        MIDXBULD_KEY_INFO_T*     pt_key_info, 
                        VOID*                    pv_obj)
{

#ifndef __NO_FM__
    UINT8                   ui1_i;
    INT32                   i4_ret;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    FLV_IDXBULD_OBJ_T*      pt_flv_handler;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext_last;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_obj == NULL)
        || (ps_path_name == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    if (pt_key_info->ui4_scale == 0 ||
        pt_key_info->ui4_rate == 0 ||
        (pt_key_info->ui4_sample_sz == 1 && pt_key_info->ui4_avg_bytes_per_sec == 0))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_fcopybytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_finput1bytes_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_finput3bytes_l_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_l_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_fsetpos_fct;
    pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_fgetpos_fct;

    pt_flv_handler = (FLV_IDXBULD_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_flv_handler == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_flv_handler, 0, sizeof(FLV_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_flv_handler;
    
    i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_path_name, FM_READ_ONLY, 0777, FALSE, &pt_midxbuld_obj->h_file);
    if (i4_ret != FMR_OK)
    {
        pt_midxbuld_obj->pv_handler_obj = NULL;
        _mm_util_mem_free_with_tag(pt_flv_handler, MM_MEM_MIDX_TAG);
        return MIDXBULDR_INITIAL_ERR;
    }

    pt_flv_handler->ui1_range_count = 0;
    pt_flv_handler->b_is_abs_offset = FALSE;
    pt_flv_handler->ui4_invalid_entry = 0;
    ui1_idx_range_id_last = 1;

    pt_flv_handler->pui1_idx_buf = 
        (UINT8*)_mm_util_mem_alloc_with_tag(sizeof(UINT8)*FLV_IDX_BUF_LENGTH, 
                                            MM_MEM_MIDX_TAG);
    if(pt_flv_handler->pui1_idx_buf == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_flv_handler->pui1_idx_buf, 0, sizeof(UINT8)*FLV_IDX_BUF_LENGTH);
#if 0
    x_memset(pt_flv_handler->aui1_idx_strm_num_map, 
             0xff, 
             FLV_MIDXBULD_IDX_STRM_NUM_MAX); /* reset ab_idx_strm_num_list */
    x_memset(pt_flv_handler->apt_tbl_obj_map, 
             0, 
             sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T*) * MIDXBULD_ELMT_TBL_MAX);
#endif
    /* key table extern aduio */
    pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                            (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
    if(pt_key_tbl_ext == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_tbl_ext, 
             0, 
             sizeof(MIDXBULD_KEY_TBL_EXT_T));
    pt_flv_handler->pt_key_tbl_ext = pt_key_tbl_ext;
    pt_key_tbl_ext_last = pt_key_tbl_ext;
    pt_key_tbl_ext->pt_next = NULL;

    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                                    (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
            if(pt_key_tbl_ext == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_key_tbl_ext, 
                     0, 
                     sizeof(MIDXBULD_KEY_TBL_EXT_T));
            pt_key_tbl_ext_last->pt_next = pt_key_tbl_ext;
            pt_key_tbl_ext_last = pt_key_tbl_ext;
            pt_key_tbl_ext->pt_next = NULL;
        }
    }
#if 0
    /* FLV2.0 multi-riff */
    x_memset(pt_flv_handler->apt_base_indx_info,
             0, 
             sizeof(FLV_BASE_INDX_INFO_T*) * MIDXBULD_ELMT_TBL_MAX);
#endif

    FLV_ALLOC_BUF();
    /* common index type */
    return flv_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj);
    
#else
    return MIDXBULDR_INITIAL_ERR;
#endif
}

/*-----------------------------------------------------------------------------
 * Name:  flv_midxbuld_mopen
 *
 * Description: This function open a handler for a memory buffer
 *
 * Inputs:  pv_mem_data  specify the address of the memory buffer
 *          z_mem_leng   specify the length of the buffer
 *          pv_obj       specify the media info object  
 *
 * Outputs: pv_obj       modify data in the media info object  
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 flv_midxbuld_mopen(
                        VOID*                    pv_mem_data,
                        SIZE_T                   z_mem_leng,
                        MIDXBULD_KEY_INFO_T*     pt_key_info,
                        VOID*                    pv_obj)
{
    UINT8                   ui1_i;
    //INT32                   i4_ret;
    MIDXBULD_OBJ_T*         pt_midxbuld_obj;
    FLV_IDXBULD_OBJ_T*      pt_flv_handler;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext;
    MIDXBULD_KEY_TBL_EXT_T* pt_key_tbl_ext_last;
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ((pv_mem_data == NULL)
        || (pv_obj == NULL))
    {
        return MIDXBULDR_INV_ARG;
    }
    if (pt_key_info->ui4_scale == 0 ||
        pt_key_info->ui4_rate == 0 ||
        (pt_key_info->ui4_sample_sz == 1 && pt_key_info->ui4_avg_bytes_per_sec == 0))
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    pt_midxbuld_obj = (MIDXBULD_OBJ_T*) pv_obj;

    if (z_mem_leng == 0)
    {
        MEDIA_FEEDER_T* pt_src_feeder = (MEDIA_FEEDER_T*)pv_mem_data;

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
    }
    else
    {
        pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes = mm_util_mcopybytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input1 = mm_util_minput1bytes_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input3 = mm_util_minput3bytes_l_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_minput4bytes_l_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos = mm_util_msetpos_fct;
        pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos = mm_util_mgetpos_fct;
    }

    pt_midxbuld_obj->i8_buff_pos = 0;

    pt_flv_handler = (FLV_IDXBULD_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_OBJ_T), MM_MEM_MIDX_TAG);
    if (pt_flv_handler == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_flv_handler, 0, sizeof(FLV_IDXBULD_OBJ_T));
    pt_midxbuld_obj->pv_handler_obj = pt_flv_handler;

    pt_flv_handler->ui1_range_count = 0;
    pt_flv_handler->b_is_abs_offset = FALSE;
    pt_flv_handler->ui4_invalid_entry = 0;
    ui1_idx_range_id_last = 1;

    pt_flv_handler->pui1_idx_buf = 
        (UINT8*)_mm_util_mem_alloc_with_tag(sizeof(UINT8)*FLV_IDX_BUF_LENGTH, 
                                            MM_MEM_MIDX_TAG);
    if(pt_flv_handler->pui1_idx_buf == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_flv_handler->pui1_idx_buf, 0, sizeof(UINT8)*FLV_IDX_BUF_LENGTH);

#if 0
    x_memset(pt_flv_handler->aui1_idx_strm_num_map, 
             0xff, 
             FLV_MIDXBULD_IDX_STRM_NUM_MAX); /* reset ab_idx_strm_num_list */
    x_memset(pt_flv_handler->apt_tbl_obj_map, 
             0, 
             sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T*) * MIDXBULD_ELMT_TBL_MAX);
#endif
    /* key table extern aduio */
    pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                            (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
    if(pt_key_tbl_ext == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_tbl_ext, 
             0, 
             sizeof(MIDXBULD_KEY_TBL_EXT_T));
    pt_flv_handler->pt_key_tbl_ext = pt_key_tbl_ext;
    pt_key_tbl_ext_last = pt_key_tbl_ext;
    pt_key_tbl_ext->pt_next = NULL;
    for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
    {
        if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
        {
            break;
        }
        if(pt_midxbuld_obj->t_src_info.t_es_map[ui1_i].e_es_type == MIDXBULD_ELEM_STRM_TYPE_AUDIO)
        {
            pt_key_tbl_ext = (MIDXBULD_KEY_TBL_EXT_T*)_mm_util_mem_alloc_with_tag
                                    (sizeof(MIDXBULD_KEY_TBL_EXT_T), MM_MEM_MIDX_TAG);
            if(pt_key_tbl_ext == NULL)
            {
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_key_tbl_ext, 
                     0, 
                     sizeof(MIDXBULD_KEY_TBL_EXT_T));
            pt_key_tbl_ext_last->pt_next = pt_key_tbl_ext;
            pt_key_tbl_ext_last = pt_key_tbl_ext;
            pt_key_tbl_ext->pt_next = NULL;
        }
    }
#if 0
    /* FLV2.0 multi-riff */
    x_memset(pt_flv_handler->apt_base_indx_info,
             0, 
             sizeof(FLV_BASE_INDX_INFO_T*) * MIDXBULD_ELMT_TBL_MAX);
#endif
    /* common index type */
    FLV_ALLOC_BUF();
    return flv_midxbuld_key_frame_table_alloc(pt_key_info, pv_obj);

}    

/*-----------------------------------------------------------------------------
 * Name:  flv_midxbuld_close
 *
 * Description: This function close a handler
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 flv_midxbuld_close(VOID* pv_obj)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_ext;
    //FLV_MULTI_RIFF_INFO_T*      pt_multi_riff;
    
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
    if (pt_midxbuld_obj->pv_handler_obj != NULL)
    {
        FLV_IDXBULD_OBJ_T*  pt_flv_handler;
        //UINT8               ui1_i;

        /* free all range objects */
        pt_flv_handler = (FLV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
        while (pt_flv_handler->ui1_range_count > 0)
        {
            _flv_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                         pt_flv_handler->apt_range_obj[0]->ui4_range_id);
        }
        _flv_midxbuld_free_range_obj(pt_midxbuld_obj, 
                                     pt_flv_handler->pt_key_range_obj->ui4_range_id);
#if 0
        for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
        {
            if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            _mm_util_mem_free_with_tag(pt_flv_handler->apui8_last_chunk_num_map[ui1_i], 
                                       MM_MEM_MIDX_TAG);
        }

        _mm_util_mem_free_with_tag(pt_flv_handler->pui8_key_frame_pos_map, 
                                   MM_MEM_MIDX_TAG);
#endif
        _mm_util_mem_free_with_tag(pt_flv_handler->pui1_idx_buf, 
                                   MM_MEM_MIDX_TAG);

        while(pt_flv_handler->pt_key_tbl_ext != NULL)
        {
            pt_key_tbl_ext = pt_flv_handler->pt_key_tbl_ext;
            pt_flv_handler->pt_key_tbl_ext = pt_flv_handler->pt_key_tbl_ext->pt_next;
            _mm_util_mem_free_with_tag(pt_key_tbl_ext,
                                       MM_MEM_MIDX_TAG);
        }
#if 0
        /* FLV2.0 multi-riff */
        for (ui1_i = 0; ui1_i < pt_midxbuld_obj->t_src_info.ui1_total_strm_num; ui1_i++)
        {
            if (ui1_i >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            if(pt_flv_handler->apt_base_indx_info[ui1_i] != NULL)
            {
                _mm_util_mem_free_with_tag(
                    pt_flv_handler->apt_base_indx_info[ui1_i]->pt_base_indx_entry, 
                    MM_MEM_MIDX_TAG);
                _mm_util_mem_free_with_tag(
                    pt_flv_handler->apt_base_indx_info[ui1_i], 
                    MM_MEM_MIDX_TAG);
            }
        }
        while(pt_flv_handler->pt_multi_riff_info != NULL)
        {
            pt_multi_riff = pt_flv_handler->pt_multi_riff_info;
            pt_flv_handler->pt_multi_riff_info = 
                pt_flv_handler->pt_multi_riff_info->pt_next;
            _mm_util_mem_free_with_tag(pt_multi_riff, MM_MEM_MIDX_TAG);
        }
#endif
        _mm_util_mem_free_with_tag(pt_midxbuld_obj->pv_handler_obj, 
                                   MM_MEM_MIDX_TAG);
        pt_midxbuld_obj->pv_handler_obj = NULL;
    }
    return MIDXBULDR_OK;
}    

/*-----------------------------------------------------------------------------
 * Name:  flv_midxbuld_start_indexing
 *
 * Description: This function start indexing
 *
 * Inputs:  pv_obj       specify the media info object  
 *
 * Outputs: 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 flv_midxbuld_start_indexing(VOID* pv_obj, UINT32* pui4_range_id)
{
    MIDXBULD_OBJ_T*             pt_midxbuld_obj;
    FLV_IDXBULD_OBJ_T*          pt_flv_handler;
    copybytes_fct               pf_copybyte;
    setpos_fct                  pf_set_pos;
    UINT64                      ui8_pos, ui8_size;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    UINT32                      ui4_size;
    UINT32                      ui4_read_size;
    UINT32                      ui4_curr_size;
    UINT8*                      pui1_index_buf;
    UINT32                      ui4_read_pos = 0;
    UINT32                      ui4_cur_parse_size;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_key_entry;
    
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
    /*build index array*/    
    
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    pt_flv_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_flv_handler == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    if(pt_flv_handler->pt_curr_range_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* do                                                                   */
    /************************************************************************/
    *pui4_range_id = pt_flv_handler->pt_curr_range_obj->ui4_range_id;
    
    if (pt_flv_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
    {
        return MIDXBULDR_OK;
    }
    if (pt_flv_handler->pt_key_range_obj->b_index_is_valid == TRUE &&
        pt_flv_handler->ui1_range_count == 0)
    {
        return MIDXBULDR_OK;
    }
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    }

    FLV_ALLOC_BUF();
    
    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;

    /* set position to the source begin */
    pf_set_pos(pt_midxbuld_obj, (INT64)pt_midxbuld_obj->t_src_info.ui8_idx_offset, FM_SEEK_BGN);

    ui4_size = (UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_size;
    ui4_curr_size = ui4_size;

    pf_copybyte(pt_midxbuld_obj, 
                    pt_flv_handler->pui1_idx_buf, 
                    FLV_IDX_BUF_LENGTH,
                    ui4_curr_size, 
                    &ui4_read_size);
    if(ui4_size > ui4_read_size)
    {
        ui4_size -= ui4_read_size;
    }
    else
    {
        ui4_size = 0;
    }

    pui1_index_buf = pt_flv_handler->pui1_idx_buf;

    pt_flv_handler->ui1_curr_buld_progress = FLV_MIDXBULD_INDEXING_START;
    //ui1_estimate_buld_start_percent = FLV_MIDXBULD_INDEXING_VIDEO_START;
    
    if (pt_flv_handler->ui8_resume_pos != 0)
    {
        ui8_pos = pt_flv_handler->ui8_resume_pos;
        ui8_size = pt_flv_handler->ui8_resume_size;
        pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_CUR);
    }
    else
    {
        ui8_pos = 0;
        ui8_size = pt_midxbuld_obj->t_src_info.ui8_idx_size;
        /* consider the partial range case */
        pt_tbl_obj   = pt_flv_handler->pt_key_range_obj->pt_tbl_obj;
        pt_key_entry = 
            pt_flv_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;

        while(ui4_read_pos < (UINT32)ui8_size)
        {
           ui4_cur_parse_size = (UINT32)flv_midxbuld_objectvalue(pui1_index_buf, pt_key_entry, 
                         (UINT32)ui8_size - ui4_read_pos);
           if(ui4_cur_parse_size == (UINT32)FLV_KEY_TBL_PROC_FINISH)
           {
               break;
           }
           ui4_read_pos += ui4_cur_parse_size;
           pui1_index_buf += ui4_cur_parse_size;       
        }            
        
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->ui4_tbl_size;
    }
    
    pt_flv_handler->ui1_curr_buld_progress = FLV_MIDXBULD_INDEXING_VIDEO_END;    

    pt_flv_handler->ui1_curr_buld_progress = FLV_MIDXBULD_INDEXING_END;
    
    pt_flv_handler->pt_curr_range_obj->b_index_is_valid = TRUE;

    FLV_FREE_BUF();
    
    return MIDXBULDR_OK;
} 

/*-----------------------------------------------------------------------------
 * Name:  flv_midxbuld_get_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 flv_midxbuld_get_info(
                        VOID*                    pv_obj,   
                        MIDXBULD_GET_INFO_TYPE_T e_info_type,  
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    FLV_IDXBULD_OBJ_T*  pt_flv_handler;

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
    pt_flv_handler = (FLV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_flv_handler == NULL) 
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    switch (e_info_type)
    {
    case MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL:
        return flv_midxbuld_get_idx_elmt_tbl(pt_midxbuld_obj, (MIDXBULD_ELMT_TBL_INFO_T*) pv_buf);

    case MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST:
        return flv_midxbuld_get_key_idx_tbl_list(pt_midxbuld_obj, (MIDXBULD_KEY_TBL_INFO_T*)pv_buf);
        
    case MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS:
        *(UINT8*)pv_buf = pt_flv_handler->ui1_curr_buld_progress;
        return MIDXBULDR_OK;
        
    default:
        return MIDXBULDR_INV_ARG;
    }
}

static INT32 flv_midxbuld_get_key_idx_tbl_list(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_KEY_TBL_INFO_T*  pt_key_tbl)
{
    FLV_IDXBULD_OBJ_T*          pt_flv_handler;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_obj = NULL;
    
    pt_flv_handler = (FLV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_flv_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == pt_key_tbl->ui4_key_tbl_id)
    {
        pt_tbl_obj = pt_flv_handler->pt_key_range_obj->pt_tbl_obj;
        pt_key_tbl->pt_key_tbl_ext = pt_flv_handler->pt_key_tbl_ext;
    }
    else
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_key_tbl_obj = pt_flv_handler->pt_key_tbl_ext;
    while(pt_tbl_obj != NULL)
    {
        pt_key_tbl_obj->t_elmt_tbl = pt_tbl_obj->t_elmt_tbl;
        if(pt_tbl_obj->ui4_tbl_id == pt_key_tbl->ui4_key_tbl_id)
        {
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
        }
        else
        {
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info = pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.t_info;
            pt_key_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts = pt_tbl_obj->t_elmt_tbl.u_fmt.t_as_key.pui4_pts;
        }
        pt_tbl_obj = pt_tbl_obj->pt_next;
        pt_key_tbl_obj = pt_key_tbl_obj->pt_next;
    }

    return MIDXBULDR_OK;
}

static INT32 flv_midxbuld_get_idx_elmt_tbl(
                        MIDXBULD_OBJ_T*           pt_midxbuld_obj,
                        MIDXBULD_ELMT_TBL_INFO_T* pt_elmt_tbl)
{
    FLV_IDXBULD_OBJ_T*          pt_flv_handler;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj = NULL;
    FLV_IDXBULD_ELMT_TBL_OBJ_T* pt_obj;
    UINT8                       ui1_i;
    BOOL                        b_id_match = FALSE;

    pt_flv_handler = (FLV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    if (pt_flv_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_id == pt_elmt_tbl->ui4_tbl_id)
    {
        b_id_match = TRUE;
        pt_tbl_obj = pt_flv_handler->pt_key_range_obj->pt_tbl_obj;
    }
    else if (pt_flv_handler->ui1_range_count > 0)
    {
        for (ui1_i = 0; ui1_i < pt_flv_handler->ui1_range_count; ui1_i++)
        {
            if (ui1_i >= FLV_MIDXBULD_RANGE_MAX)
            {
                break;
            }
            pt_obj = pt_flv_handler->apt_range_obj[ui1_i]->pt_tbl_obj;
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
    if ((FALSE == b_id_match) || (NULL == pt_tbl_obj))
    {
        return MIDXBULDR_INV_ARG;
    }

    pt_elmt_tbl->t_elmt_tbl.ui8_base_offset = pt_tbl_obj->t_elmt_tbl.ui8_base_offset;
    pt_elmt_tbl->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.t_info = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info;
    pt_elmt_tbl->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;
    
    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  flv_midxbuld_set_info
 *
 * Description: This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete.
 *
 * Inputs:  pv_obj       specify the media info object  
 *          e_info_type  specify the information type of the media.
 *          pv_buf       specify the memory buffer which will contain the media information.
 *          pz_buf_leng  specify the length of the buffer.
 *
 * Outputs: pv_buf       specify the memory buffer which will contain the media information
 *          pz_buf_leng  If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length.
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 flv_midxbuld_set_info(
                        VOID*                    pv_obj,   
                        MIDXBULD_SET_INFO_TYPE_T e_info_type,  
                        VOID*                    pv_buf,     
                        SIZE_T*                  pz_buf_leng)
{
    MIDXBULD_OBJ_T*     pt_midxbuld_obj;
    FLV_IDXBULD_OBJ_T*  pt_flv_handler;
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
    pt_flv_handler = (FLV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;
    if (pt_flv_handler == NULL) 
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    switch (e_info_type)
    {
    case MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE:
        return flv_midxbuld_alloc_range(pt_midxbuld_obj, (MIDXBULD_ELMT_TBL_ALLOC_INFO_T*)pv_buf);

    case MIDXBULD_SET_INFO_TYPE_FREE_RANGE:
        return flv_midxbuld_free_range(pt_midxbuld_obj, (UINT32)pv_buf);

    default:
        return MIDXBULDR_INV_ARG;
    }
}

static UINT32 flv_midxbuld_key_tbl_size_estimate(
                              MIDXBULD_OBJ_T* pt_midxbuld_obj,
                              UINT32          ui4_key_id)
{
    FLV_IDXBULD_OBJ_T*  pt_flv_handler;
    copybytes_fct       pf_copybyte;
    setpos_fct          pf_set_pos;
    getpos_fct          pf_get_pos;
    UINT64              ui8_pos;
    UINT64              ui8_i = 0;
    UINT32              ui4_size = 0;
    UINT32              ui4_read_size;
    UINT32              ui4_curr_size;
    UINT8*              pui1_index_buf;
    UINT32              ui4_read_pos  = 0;
    UINT32              ui4_cur_parse_size = 0;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_midxbuld_obj == NULL)
    {
        return 0;
    }    
    pt_flv_handler = pt_midxbuld_obj->pv_handler_obj;
    if (pt_flv_handler == NULL)
    {
        return 0;
    }    

    pf_copybyte = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_get_pos = pt_midxbuld_obj->t_input_fct_tbl.pf_get_pos;

    pf_get_pos(pt_midxbuld_obj, &ui8_pos);
    
    pf_set_pos(pt_midxbuld_obj, (INT64)(pt_midxbuld_obj->t_src_info.ui8_idx_offset), FM_SEEK_BGN);

    pt_flv_handler->ui1_curr_buld_progress = FLV_MIDXBULD_ESTIMATE_KEY_SIZE_START;
      
    while (ui8_i < pt_midxbuld_obj->t_src_info.ui8_idx_size)
    {
        ui4_curr_size = (UINT32)(pt_midxbuld_obj->t_src_info.ui8_idx_size - ui8_i);
        if(ui4_curr_size >= FLV_IDX_BUF_LENGTH)
        {
            ui4_curr_size = FLV_IDX_BUF_LENGTH;
        }
        pf_copybyte(pt_midxbuld_obj, 
                    pt_flv_handler->pui1_idx_buf, 
                    FLV_IDX_BUF_LENGTH,
                    ui4_curr_size, 
                    &ui4_read_size);        
        
        pui1_index_buf = pt_flv_handler->pui1_idx_buf;
            
        while(ui4_read_pos < (UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_size)
        {
           ui4_cur_parse_size = (UINT32)flv_midxbuld_objectvalue(pui1_index_buf, NULL, 
                   (UINT32)pt_midxbuld_obj->t_src_info.ui8_idx_size - ui4_read_pos);
           if(ui4_cur_parse_size == (UINT32)FLV_KEY_TBL_PROC_FINISH)
           {
               break;
           }
           ui4_read_pos += ui4_cur_parse_size;
           pui1_index_buf += ui4_cur_parse_size;       
        }
        
        pt_flv_handler->b_is_abs_offset = TRUE;
        pt_flv_handler->ui4_invalid_entry = g_ui4_idx_entry_cnt;
        ui4_size = g_ui4_idx_entry_cnt;
        g_ui4_idx_entry_cnt = 0;

        if ((ui4_read_size < ui4_curr_size) || 
            (ui4_cur_parse_size == (UINT32)FLV_KEY_TBL_PROC_FINISH))
        {
            break;
        }
        pt_flv_handler->ui1_curr_buld_progress = FLV_MIDXBULD_ESTIMATE_KEY_SIZE_MID;
        
        ui8_i += ui4_curr_size;
    }
    
#if 0
    pt_flv_handler->pui8_key_frame_pos_map = 
        (UINT64*) _mm_util_mem_alloc_with_tag(sizeof(UINT64) * ui4_size, MM_MEM_MIDX_TAG);
#endif
    pt_flv_handler->ui1_curr_buld_progress = FLV_MIDXBULD_ESTIMATE_KEY_SIZE_END;
    
    pf_set_pos(pt_midxbuld_obj, (INT64)ui8_pos, FM_SEEK_BGN);

    return ui4_size;
}

static UINT32 flv_midxbuld_elmt_tbl_size_estimate(
                        MIDXBULD_OBJ_T*              pt_midxbuld_obj,
                        FLV_IDXBULD_RANGE_OBJ_T*     pt_range_obj,
                        MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ)
{
    UINT32 ui4_tbl_size = 0;

    if (pt_range_obj->u_range.t_es.ui4_start_pts >= pt_range_obj->u_range.t_es.ui4_end_pts)
    {
        return 0;
    }
    
    switch (pt_describ->u_info.t_es.e_es_type)
    {
    case MIDXBULD_ELEM_STRM_TYPE_AUDIO:      
    case MIDXBULD_ELEM_STRM_TYPE_VIDEO:
    case MIDXBULD_ELEM_STRM_TYPE_SUBTITLE:
    case MIDXBULD_ELEM_STRM_TYPE_KEY:
        ui4_tbl_size = flv_midxbuld_key_tbl_size_estimate(pt_midxbuld_obj, pt_describ->u_info.t_es.ui4_strm_id);

        break;

    default:
        ui4_tbl_size = 0;
        break;
    }

    return ui4_tbl_size;
}

static INT32 flv_midxbuld_alloc_range(
                        MIDXBULD_OBJ_T*                 pt_midxbuld_obj, 
                        MIDXBULD_ELMT_TBL_ALLOC_INFO_T* pt_elmt_tbl_alloc)
{
    FLV_IDXBULD_OBJ_T*           pt_flv_handler;
    FLV_IDXBULD_RANGE_OBJ_T*     pt_range_obj;
    FLV_IDXBULD_ELMT_TBL_OBJ_T*  pt_tbl_obj;
    FLV_IDXBULD_ELMT_TBL_OBJ_T*  pt_tbl_obj_last;
    MIDXBULD_ELMT_TBL_DESCRIB_T* pt_describ = NULL;
    UINT8                        ui1_tbl_count = 0;
    UINT8                        ui1_strm_num;

    pt_flv_handler = (FLV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;

    if (pt_flv_handler->ui1_range_count >= FLV_MIDXBULD_RANGE_MAX)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    if (pt_elmt_tbl_alloc->u_range.t_es.ui4_start_pts >= pt_elmt_tbl_alloc->u_range.t_es.ui4_end_pts)
    {
        return MIDXBULDR_INV_ARG;
    }

    if (pt_flv_handler->pt_key_range_obj != NULL &&
        pt_flv_handler->pt_key_range_obj->b_index_is_valid == FALSE)
    {
        return MIDXBULDR_KEY_TBL_NOT_READY;
    }
#if 0
    x_memset(pt_flv_handler->aui1_idx_strm_num_map, 0xff, FLV_MIDXBULD_IDX_STRM_NUM_MAX); /* reset ab_idx_strm_num_list */
    x_memset(pt_flv_handler->apt_tbl_obj_map, 0, sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T*) * MIDXBULD_ELMT_TBL_MAX);
#endif
    if (pt_elmt_tbl_alloc->e_tbl_type == MIDXBULD_TBL_TYPE_ELMT_STRM)
    {
        /* allocate elmt tbl range obj memory */
        pt_range_obj = (FLV_IDXBULD_RANGE_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_RANGE_OBJ_T), MM_MEM_MIDX_TAG);
        if (pt_range_obj == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }

        if (TRUE != _flv_midxbuld_alloc_range_id(pt_midxbuld_obj, &pt_elmt_tbl_alloc->ui4_range_id))
        {
            _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
            return MIDXBULDR_OUT_OF_HANDLE;
        }

        pt_flv_handler->apt_range_obj[pt_flv_handler->ui1_range_count++] = pt_range_obj;

        x_memset(pt_range_obj, 0, sizeof(FLV_IDXBULD_RANGE_OBJ_T));
        pt_range_obj->e_tbl_type = pt_elmt_tbl_alloc->e_tbl_type;
        pt_range_obj->u_range.t_es = pt_elmt_tbl_alloc->u_range.t_es;
        pt_range_obj->ui4_range_id = pt_elmt_tbl_alloc->ui4_range_id;
        pt_range_obj->b_is_key_frame = FALSE;
        pt_range_obj->b_index_is_valid = FALSE;

        pt_describ = pt_elmt_tbl_alloc->pt_tbl_describ;

        /* allocate elmt tbl obj memory */
        pt_tbl_obj = (FLV_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
        if (pt_tbl_obj == NULL)
        {
            _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        if (TRUE != _flv_midxbuld_alloc_table_id(pt_range_obj, &pt_describ->ui4_tbl_id))
        {
            _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
            _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_HANDLE;
        }

        pt_range_obj->pt_tbl_obj = pt_tbl_obj;
        pt_tbl_obj_last = pt_tbl_obj;

        x_memset(pt_tbl_obj, 0, sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T));
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
        pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
        pt_tbl_obj->ui4_tbl_size = flv_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, pt_range_obj, pt_describ);
        
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_describ->u_info.t_es;
        /*pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_start_frame_num = 0;
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_end_frame_num = 0;*/
        pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = 
            (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
        if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry == NULL)
        {
            _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
            pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
            pt_describ->ui4_tbl_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);

        pt_tbl_obj->ui8_last_chunk_num = 0;
        /*ui8_temp = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        pt_tbl_obj->ui4_pts_per_frame = ui8_temp / pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;*/
        pt_tbl_obj->ui4_tbl_id = pt_describ->ui4_tbl_id;
        pt_tbl_obj->pt_next = NULL;

        ui1_strm_num = 0;
        if (ui1_strm_num >= FLV_MIDXBULD_IDX_STRM_NUM_MAX)
        {
            return MIDXBULDR_INITIAL_ERR;
        }
#if 0
        pt_flv_handler->aui1_idx_strm_num_map[ui1_strm_num] = ui1_tbl_count;
        pt_flv_handler->apt_tbl_obj_map[ui1_tbl_count++] = pt_tbl_obj;
#endif
#if 1
        while (pt_describ->pt_next != NULL)
        {
            pt_describ = pt_describ->pt_next;

            /* allocate elmt tbl obj memory */
            pt_tbl_obj = (FLV_IDXBULD_ELMT_TBL_OBJ_T*) _mm_util_mem_alloc_with_tag(sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T), MM_MEM_MIDX_TAG);
            if (pt_tbl_obj == NULL)
            {
                _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            if (TRUE != _flv_midxbuld_alloc_table_id(pt_range_obj, &pt_describ->ui4_tbl_id))
            {
                _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
                _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_HANDLE;
            }
            
            pt_tbl_obj_last->pt_next = pt_tbl_obj;
            pt_tbl_obj_last = pt_tbl_obj;

            x_memset(pt_tbl_obj, 0, sizeof(FLV_IDXBULD_ELMT_TBL_OBJ_T));
            pt_tbl_obj->t_elmt_tbl.ui8_base_offset = 0;            
            pt_tbl_obj->t_elmt_tbl.e_tbl_type = MIDXBULD_TBL_TYPE_ELMT_STRM;
            pt_tbl_obj->ui4_tbl_size = flv_midxbuld_elmt_tbl_size_estimate(pt_midxbuld_obj, pt_range_obj, pt_describ);
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->ui4_tbl_size;
            
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = pt_describ->u_info.t_es;
            /*pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_start_frame_num = 0;
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.ui4_end_frame_num = 0;*/
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size, MM_MEM_MIDX_TAG);
            if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry == NULL)
            {
                _flv_midxbuld_free_range_obj(pt_midxbuld_obj, pt_elmt_tbl_alloc->ui4_range_id);
                pt_elmt_tbl_alloc->ui4_range_id = MIDXBULD_NULL_ID;
                pt_describ->ui4_tbl_id = MIDXBULD_NULL_ID;
                return MIDXBULDR_OUT_OF_MEMORY;
            }
            x_memset(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);

            pt_tbl_obj->ui8_last_chunk_num = 0;
            /*ui8_temp = (UINT64)pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
            pt_tbl_obj->ui4_pts_per_frame = ui8_temp / pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_rate;*/
            pt_tbl_obj->ui4_tbl_id = pt_describ->ui4_tbl_id;
            pt_tbl_obj->pt_next = NULL;

            ui1_strm_num = 0;
            if (ui1_strm_num >= FLV_MIDXBULD_IDX_STRM_NUM_MAX)
            {
                return MIDXBULDR_INITIAL_ERR;
            }
            /*pt_flv_handler->aui1_idx_strm_num_map[ui1_strm_num] = ui1_tbl_count;*/
            ui1_tbl_count++;
            if (ui1_tbl_count >= MIDXBULD_ELMT_TBL_MAX)
            {
                break;
            }
            /*pt_flv_handler->apt_tbl_obj_map[ui1_tbl_count] = pt_tbl_obj;*/
        }
#endif

        pt_flv_handler->pt_curr_range_obj = pt_range_obj;

        pt_flv_handler->ui8_resume_pos = 0;
        pt_flv_handler->ui8_resume_size = 0;
    }
    else
    {
        return MIDXBULDR_INV_ARG;
    }

    return MIDXBULDR_OK;
}


static INT32 flv_midxbuld_free_range(
                        MIDXBULD_OBJ_T*          pt_midxbuld_obj,
                        UINT32                   ui4_range_id)
{
    FLV_IDXBULD_OBJ_T*  pt_flv_handler;
    
    pt_flv_handler = (FLV_IDXBULD_OBJ_T*)pt_midxbuld_obj->pv_handler_obj;

    if (pt_flv_handler->ui1_range_count == 0 ||
        pt_flv_handler->pt_key_range_obj->ui4_range_id == ui4_range_id) /* can NOT free key range */
    {
        return MIDXBULDR_INV_HANDLE;
    }

    if (TRUE == _flv_midxbuld_free_range_obj(pt_midxbuld_obj, ui4_range_id))
    {
        return MIDXBULDR_OK;
    }
    
    return MIDXBULDR_INV_HANDLE;
}

