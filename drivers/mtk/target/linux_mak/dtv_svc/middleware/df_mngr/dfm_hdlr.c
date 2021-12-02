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
 * $RCSfile: dfm_hdlr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all Download Function Manager handler related API
 *         implementations.
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
---------------------------------------------------------------------------*/

#include "dfm_dbg.h"
#include "dfm_hdlr.h"
#include "dfm_eng.h"
#include "handle/handle.h"
#include "inc/x_common.h"

#include "cl/zip_eng/zlib.h"
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "tbl_mngr/x_tm.h"
#include "tbl_mngr/isdb_si_eng/x_isdb_si_eng.h"
#include "img/x_img.h"
#include "wgl/x_wgl_image.h"
#include "graphic/x_gl.h"

typedef struct _LOGO_INFO_T
{
    DLIST_ENTRY_T(_LOGO_INFO_T)  t_link;

    BOOL        b_exist;
    UINT8       ui1_logo_trans_type;
    UINT16      ui2_onid;
    UINT16      ui2_svc_id;
    UINT16      ui2_scan_idx;

    UINT16      ui2_logo_id;    
    UINT16      ui2_dd_data_id;
    UINT16      ui2_logo_ver;
    UINT8       aui1_data[SIMP_LOGO_LEN];
}   LOGO_INFO_T;

typedef DLIST_T(_LOGO_INFO_T)  LOGO_INFO_LIST_T;

#define PNG_CHUNK_HEADER_LEN    (   8 )
#define PNG_TRNS_DATA_3_LEN     ( 128 )
#define PNG_PLTE_DATA_3_LEN     ( 384 )

extern UINT8    g_aui1_png_trns_header_3[];
extern UINT8    g_aui1_png_trns_data_3[];

extern UINT8    g_aui1_png_plte_header_3[];
extern UINT8    g_aui1_png_plte_data_3[];

UINT8   g_aui1_png_trns_header_3[] =
{
    0x00, 0x00, 0x00, 0x80, 0x74, 0x52, 0x4E, 0x53
};

UINT8   g_aui1_png_trns_data_3[] = 
{
    0x00, 0x00, 0x00, 0x80, 0x74, 0x52, 0x4E, 0x53,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0x00 ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0xFF,
    0xFF ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80,
    0x80 ,0x80  
};

UINT8   g_aui1_png_plte_header_3[] =
{
    0x00, 0x00, 0x01, 0x80, 0x50, 0x4C, 0x54, 0x45
};

UINT8   g_aui1_png_plte_data_3[] = 
{
    0x00, 0x00, 0x01, 0x80, 0x50, 0x4C, 0x54, 0x45,  
    0x00 ,0x00 ,0x00 ,0xFF ,0x00 ,0x00,
    0x00 ,0xFF ,0x00 ,0xFF ,0xFF ,0x00,
    0x00 ,0x00 ,0xFF ,0xFF ,0x00 ,0xFF,
    0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF,
    0x00 ,0x00 ,0x00 ,0xAA ,0x00 ,0x00,
    0x00 ,0xAA ,0x00 ,0xAA ,0xAA ,0x00,
    0x00 ,0x00 ,0xAA ,0xAA ,0x00 ,0xAA,
    0x00 ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
    0x00 ,0x00 ,0x55 ,0x00 ,0x55 ,0x00,
    0x00 ,0x55 ,0x55 ,0x00 ,0x55 ,0xAA,
    0x00 ,0x55 ,0xFF ,0x00 ,0xAA ,0x55,
    0x00 ,0xAA ,0xFF ,0x00 ,0xFF ,0x55,
    0x00 ,0xFF ,0xAA ,0x55 ,0x00 ,0x00,
    0x55 ,0x00 ,0x55 ,0x55 ,0x00 ,0xAA,
    0x55 ,0x00 ,0xFF ,0x55 ,0x55 ,0x00,
    0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0xAA,
    0x55 ,0x55 ,0xFF ,0x55 ,0xAA ,0x00,
    0x55 ,0xAA ,0x55 ,0x55 ,0xAA ,0xAA,
    0x55 ,0xAA ,0xFF ,0x55 ,0xFF ,0x00,
    0x55 ,0xFF ,0x55 ,0x55 ,0xFF ,0xAA,
    0x55 ,0xFF ,0xFF ,0xAA ,0x00 ,0x55,
    0xAA ,0x00 ,0xFF ,0xAA ,0x55 ,0x00,
    0xAA ,0x55 ,0x55 ,0xAA ,0x55 ,0xAA,
    0xAA ,0x55 ,0xFF ,0xAA ,0xAA ,0x55,
    0xAA ,0xAA ,0xFF ,0xAA ,0xFF ,0x00,
    0xAA ,0xFF ,0x55 ,0xAA ,0xFF ,0xAA,
    0xAA ,0xFF ,0xFF ,0xFF ,0x00 ,0x55,
    0xFF ,0x00 ,0xAA ,0xFF ,0x55 ,0x00,
    0xFF ,0x55 ,0x55 ,0xFF ,0x55 ,0xAA,
    0xFF ,0x55 ,0xFF ,0xFF ,0xAA ,0x00,
    0xFF ,0xAA ,0x55 ,0xFF ,0xAA ,0xAA,
    0xFF ,0xAA ,0xFF ,0xFF ,0xFF ,0x55,
    0xFF ,0xFF ,0xAA ,0x00 ,0x00 ,0x00,
    0xFF ,0x00 ,0x00 ,0x00 ,0xFF ,0x00,
    0xFF ,0xFF ,0x00 ,0x00 ,0x00 ,0xFF,
    0xFF ,0x00 ,0xFF ,0x00 ,0xFF ,0xFF,
    0xFF ,0xFF ,0xFF ,0xAA ,0x00 ,0x00,
    0x00 ,0xAA ,0x00 ,0xAA ,0xAA ,0x00,
    0x00 ,0x00 ,0xAA ,0xAA ,0x00 ,0xAA,
    0x00 ,0xAA ,0xAA ,0xAA ,0xAA ,0xAA,
    0x00 ,0x00 ,0x55 ,0x00 ,0x55 ,0x00,
    0x00 ,0x55 ,0x55 ,0x00 ,0x55 ,0xAA,
    0x00 ,0x55 ,0xFF ,0x00 ,0xAA ,0x55,
    0x00 ,0xAA ,0xFF ,0x00 ,0xFF ,0x55,
    0x00 ,0xFF ,0xAA ,0x55 ,0x00 ,0x00,
    0x55 ,0x00 ,0x55 ,0x55 ,0x00 ,0xAA,
    0x55 ,0x00 ,0xFF ,0x55 ,0x55 ,0x00,
    0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0xAA,
    0x55 ,0x55 ,0xFF ,0x55 ,0xAA ,0x00,
    0x55 ,0xAA ,0x55 ,0x55 ,0xAA ,0xAA,
    0x55 ,0xAA ,0xFF ,0x55 ,0xFF ,0x00,
    0x55 ,0xFF ,0x55 ,0x55 ,0xFF ,0xAA,
    0x55 ,0xFF ,0xFF ,0xAA ,0x00 ,0x55,
    0xAA ,0x00 ,0xFF ,0xAA ,0x55 ,0x00,
    0xAA ,0x55 ,0x55 ,0xAA ,0x55 ,0xAA,
    0xAA ,0x55 ,0xFF ,0xAA ,0xAA ,0x55,
    0xAA ,0xAA ,0xFF ,0xAA ,0xFF ,0x00,
    0xAA ,0xFF ,0x55 ,0xAA ,0xFF ,0xAA,
    0xAA ,0xFF ,0xFF ,0xFF ,0x00 ,0x55,
    0xFF ,0x00 ,0xAA ,0xFF ,0x55 ,0x00,
    0xFF ,0x55 ,0x55 ,0xFF ,0x55 ,0xAA,
    0xFF ,0x55 ,0xFF ,0xFF ,0xAA ,0x00,
    0xFF ,0xAA ,0x55 ,0xFF ,0xAA ,0xAA,
    0xFF ,0xAA ,0xFF ,0xFF ,0xFF ,0x55  
};

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
static BOOL  b_init = FALSE;                  /* Indicates if initialized. */

static LOGO_CTRL_LIST_T  t_head_logo_req;        /* The head of the logo requeset array */
static LOGO_CTRL_LIST_T  t_free_logo_req;        /* Links the free logo requests. */

static LOGO_DATA_LIST_T  t_head_logo_data;       /* The head of the logo data array */
static LOGO_DATA_LIST_T  t_free_logo_data;       /* Links the free logo data. */

static HANDLE_T          h_current_sdt;
static UINT16            g_ui2_onid;
static UINT16            g_ui2_scan_idx;

/*---------------------------------------------------------------------------
                   static function
---------------------------------------------------------------------------*/
static VOID     _dfm_hdlr_free_logo_data(
                    LOGO_DATA_T*            pt_logo_data
                    );
static VOID     _dfm_hdlr_free_logo(
                    LOGO_CTRL_T*            pt_logo
                    );
static VOID     _dfm_hdlr_free_logo_entry_list(
                    LOGO_TYPE_ENTRY_T*      pt_logo_type_list
                    );

static INT32 _dfm_hdlr_load_logo_data_by_key(
                    HANDLE_T         h_root,
                    UINT16   ui2_onid,
                    UINT16   ui2_dd_data_id,
                    UINT16   ui2_logo_id,
                    UINT16   ui2_logo_ver,
                    UINT16   ui2_scan_idx,
                    LOGO_DATA_T**  ppt_logo_data
                    );
static VOID     _dfm_hdlr_update_logo_png_data( 
                    const UINT8*            pui1_png,
                    UINT16                  ui2_png_len,
                    UINT8**                 ppui1_update_png,
                    UINT16*                 pui2_update_png_len
                    );
static DFM_LOGO_TYPE_T _dfm_hdlr_convert_logo_type(
                    UINT8                   ui1_logo_type
                    );

static VOID _dfm_free_temp_logo_tran_info(
                    LOGO_INFO_LIST_T* pt_tmp_logo_tran_info
                    );
/*---------------------------------------------------------------------------
                    function
---------------------------------------------------------------------------*/
static NFY_RET_T _dfm_hdlr_sdt_nfy_fct
(
    HANDLE_T                    h_obj,
    HANDLE_TYPE_T               e_obj_type,
    TM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_data
)
{
    DFM_MSG_T                   t_msg;  

    DBG_INFO((_DFM_INFO" load SDT condition[%d]\n",e_nfy_cond));
    if (pv_nfy_tag == NULL)
    {
        return NFY_RET_PROCESSED;
    }
    
    if (e_nfy_cond != TM_COND_UPDATE &&
        e_nfy_cond != TM_COND_AVAILABLE &&
        e_nfy_cond != TM_COND_UNAVAILABLE)
    {
        
        return NFY_RET_PROCESSED;
    }   
    if ( (e_nfy_cond == TM_COND_UPDATE) ||
         (e_nfy_cond == TM_COND_AVAILABLE))
    {
        t_msg.e_msg_type = MSG_TYPE_SDT_OBJ_AVAILABLE;      
    }
    else
    {
        t_msg.e_msg_type = MSG_TYPE_SDT_OBJ_UNAVAILABLE;
    }
    t_msg.pv_data = pv_nfy_tag;
    dfm_msg_q_send(&t_msg);

    return NFY_RET_PROCESSED;
}

static NFY_RET_T _dfm_hdlr_cdt_nfy_fct
(
    HANDLE_T                    h_obj,
    HANDLE_TYPE_T               e_obj_type,
    TM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_data
)
{
    DFM_MSG_T                   t_msg;
    
    DBG_INFO((_DFM_INFO" CDT Notify condition[%d]\n",e_nfy_cond));
    if (pv_nfy_tag == NULL)
    {
        return NFY_RET_PROCESSED;
    }
    
    if (e_nfy_cond != TM_COND_UPDATE &&
        e_nfy_cond != TM_COND_AVAILABLE &&
        e_nfy_cond != TM_COND_UNAVAILABLE)
    {
        
        return NFY_RET_PROCESSED;
    }

    if ( (e_nfy_cond == TM_COND_UPDATE) ||
         (e_nfy_cond == TM_COND_AVAILABLE))
    {
        t_msg.e_msg_type = MSG_TYPE_CDT_OBJ_AVAILABLE;      
    }
    else
    {
        t_msg.e_msg_type = MSG_TYPE_CDT_OBJ_UNAVAILABLE;
    }
    t_msg.pv_data = pv_nfy_tag;
    dfm_msg_q_send(&t_msg);
    
    
    return NFY_RET_PROCESSED;
}

static PARSE_RET_T _dfm_hdlr_parse_service_logo_desc_fct
(
    HANDLE_T                    h_obj,
    const UINT8*                pui1_obj_data,
    SIZE_T                      z_obj_data_len,
    UINT16                      ui2_count,
    VOID*                       pv_data,
    SIZE_T                      z_data_len
)
{
    SIZE_T                      z_descr_len = 0;
    INT32                       i4_ret = TMR_OK;
    LOGO_INFO_T*                pt_logo_info = NULL;

    if ((pui1_obj_data    == NULL) ||
        (pui1_obj_data[0] != 0xCF)||
        (pv_data == NULL))
    {
        return PARSE_RET_ERROR;
    }

    z_descr_len = (SIZE_T) pui1_obj_data[1];
    if (z_descr_len != z_obj_data_len - 2)
    {
        return PARSE_RET_ERROR;
    }

    pt_logo_info = (LOGO_INFO_T*)pv_data;
    
    if (pui1_obj_data[0] == 0xCF)   /* Logo_Transmission_Descriptor */
    {
        pt_logo_info->b_exist = TRUE;
        pt_logo_info->ui1_logo_trans_type = pui1_obj_data[2];
        /*Printf("#########type[%d]\n", pt_logo_info->ui1_logo_trans_type);*/
        if (pt_logo_info->ui1_logo_trans_type >= 0x03)
        {            
            pt_logo_info->ui2_logo_id    = 0;
            pt_logo_info->ui2_dd_data_id = 0;
            if ((z_descr_len-1) <= SIMP_LOGO_LEN)
            {                
                /*Printf("##########copy z_descr_len \n");*/
                x_memcpy(pt_logo_info->aui1_data, pui1_obj_data+3, z_descr_len-1);
            }
            else
            {
                /*Printf("##########copy SIMP_LOGO_LEN\n");*/
                x_memcpy(pt_logo_info->aui1_data, pui1_obj_data+3, SIMP_LOGO_LEN);
            }
        }
        else
        {
            pt_logo_info->ui2_logo_id = pui1_obj_data[3];
            pt_logo_info->ui2_logo_id 
                = pt_logo_info->ui2_logo_id << 8 | pui1_obj_data[4];
            pt_logo_info->ui2_logo_id = pt_logo_info->ui2_logo_id & 0x01FF;

            if (pt_logo_info->ui1_logo_trans_type == 0x01)
            {
                pt_logo_info->ui2_logo_ver= pui1_obj_data[5];
                pt_logo_info->ui2_logo_ver 
                    = pt_logo_info->ui2_logo_ver << 8 | pui1_obj_data[6];
                pt_logo_info->ui2_logo_ver 
                    = pt_logo_info->ui2_logo_ver & 0x0FFF;

                pt_logo_info->ui2_dd_data_id = pui1_obj_data[7];
                pt_logo_info->ui2_dd_data_id 
                    = pt_logo_info->ui2_dd_data_id << 8 | pui1_obj_data[8];
                
            }
        }
    }
    if (i4_ret != TMR_OK)
    {
        return PARSE_RET_ERROR;
    }

    return PARSE_RET_NEXT;
}

static VOID  _dfm_hdlr_update_logo_png_data(
    const UINT8* pui1_png,
    UINT16       ui2_png_len,
    UINT8**      ppui1_update_png,
    UINT16*      pui2_update_png_len)
{
    UINT32 ui4_crc32;
    UINT8* pui1_update_png = NULL;
    INT32  i4_pos = 0;
    
    if (pui1_png[25] == 0x03)
    {
        pui1_update_png = (UINT8*)x_mem_alloc((SIZE_T)(ui2_png_len+PNG_PLTE_DATA_3_LEN+PNG_CHUNK_HEADER_LEN+4
                                              + PNG_TRNS_DATA_3_LEN + PNG_CHUNK_HEADER_LEN + 4));
        if (pui1_update_png == NULL)
        {
            DBG_ERROR((_DFM_ERROR"out of memory!\r\n"));
            return;
        }
        x_memcpy(pui1_update_png, pui1_png, 33);
        i4_pos+=33;
    
        /*ui4_crc32 = 0xA71FFB91;*/

        ui4_crc32 = crc32(0, &(g_aui1_png_plte_data_3[4]), PNG_PLTE_DATA_3_LEN+PNG_CHUNK_HEADER_LEN-4);
    
        x_memcpy(pui1_update_png+i4_pos, g_aui1_png_plte_data_3,PNG_PLTE_DATA_3_LEN+PNG_CHUNK_HEADER_LEN);
        i4_pos+=PNG_PLTE_DATA_3_LEN+PNG_CHUNK_HEADER_LEN;

        pui1_update_png[i4_pos]     = (UINT8)((ui4_crc32 >> 24) & 0xFF);
        pui1_update_png[i4_pos + 1] = (UINT8)((ui4_crc32 >> 16) & 0xFF);
        pui1_update_png[i4_pos + 2] = (UINT8)((ui4_crc32 >>  8) & 0xFF);
        pui1_update_png[i4_pos + 3] = (UINT8)((ui4_crc32)       & 0xFF);
        i4_pos+=4;

        /* dyin added tRNS chunk for alpha values */
        ui4_crc32 = crc32(0, &(g_aui1_png_trns_data_3[4]), PNG_TRNS_DATA_3_LEN+PNG_CHUNK_HEADER_LEN-4);
    
        x_memcpy(pui1_update_png+i4_pos, g_aui1_png_trns_data_3,PNG_TRNS_DATA_3_LEN+PNG_CHUNK_HEADER_LEN);
        i4_pos+=PNG_TRNS_DATA_3_LEN+PNG_CHUNK_HEADER_LEN;

        pui1_update_png[i4_pos]     = (UINT8)((ui4_crc32 >> 24) & 0xFF);
        pui1_update_png[i4_pos + 1] = (UINT8)((ui4_crc32 >> 16) & 0xFF);
        pui1_update_png[i4_pos + 2] = (UINT8)((ui4_crc32 >>  8) & 0xFF);
        pui1_update_png[i4_pos + 3] = (UINT8)((ui4_crc32)       & 0xFF);
        i4_pos+=4;

        x_memcpy((VOID*)(pui1_update_png+i4_pos), (VOID*)(pui1_png+33), (SIZE_T)(ui2_png_len-33));
        i4_pos+=ui2_png_len-33;
    }
        
    *ppui1_update_png = pui1_update_png;
    *pui2_update_png_len = (UINT16)(i4_pos);
    
    return;
    
}


static VOID _dfm_hdlr_free_logo_entry_list(LOGO_TYPE_ENTRY_T* pt_logo_type_list)
{
    LOGO_TYPE_ENTRY_T* pt_node;
    LOGO_TYPE_ENTRY_T* pt_temp_node;

    pt_node = pt_logo_type_list;
    while (pt_node != NULL)
    {
        pt_temp_node = pt_node->pt_next;
        if (pt_node->ui1_logo_type == DFM_LOGO_TYPE_SIMP)
        {
            x_mem_free(pt_node->pui1_data);
        }
        else
        {
            /*x_img_png_free((VOID*)(pt_temp_node->pui1_data));*/
            x_wgl_img_tpl_destroy(pt_node->t_wgl_himg);
        }
        x_mem_free(pt_node);

        pt_node = pt_temp_node;
    }
}

static VOID _dfm_hdlr_free_logo_data(LOGO_DATA_T* pt_logo_data)
{
    DBG_INFO((_DFM_INFO" free logo data onid[0x%x],dd_data_id[0x%x], logo_id[0x%x], cdt[0x%x]\n",
                        pt_logo_data->ui2_onid, 
                        pt_logo_data->ui2_dd_data_id,
                        pt_logo_data->ui2_logo_id,
                        pt_logo_data->h_cdt));
    if (pt_logo_data->pt_logo_type_entry_list != NULL)
    {
        _dfm_hdlr_free_logo_entry_list(pt_logo_data->pt_logo_type_entry_list);
        pt_logo_data->pt_logo_type_entry_list = NULL;
    }
    x_tm_free(pt_logo_data->h_cdt); 
    pt_logo_data->h_cdt  = NULL_HANDLE;
    pt_logo_data->h_root = NULL_HANDLE;
}


static VOID _dfm_hdlr_free_logo(LOGO_CTRL_T* pt_logo)
{
    HANDLE_INFO_LINK_T*  pt_hdlr_info = NULL;
    
    /*free the handle list*/
    while (!DLIST_IS_EMPTY(&(pt_logo->t_hdl_list)))
    {
        pt_hdlr_info = DLIST_HEAD(&(pt_logo->t_hdl_list));

        if ( (!DLIST_IS_EMPTY( &(pt_logo->t_hdl_list))) && (pt_hdlr_info != NULL) )
        {
            DLIST_REMOVE(pt_hdlr_info, &(pt_logo->t_hdl_list), t_link);

            handle_free(pt_hdlr_info->h_logo, FALSE);

            x_mem_free((VOID*) pt_hdlr_info);
        }
    }    
    x_tm_free(pt_logo->h_sdt);
    pt_logo->h_sdt = NULL_HANDLE;
    
}

static DFM_LOGO_TYPE_T _dfm_hdlr_convert_logo_type(UINT8  ui1_logo_type)
{
    DFM_LOGO_TYPE_T t_logo_type;
    switch (ui1_logo_type)
    {
    case 0x00:
        t_logo_type = DFM_LOGO_TYPE_SD4_3_SMAlL;
        break;
    case 0x01:
        t_logo_type = DFM_LOGO_TYPE_SD16_9_SMALL;
        break;
    case 0x02:
        t_logo_type = DFM_LOGO_TYPE_HD_SMALL;
        break;
    case 0x03:
        t_logo_type = DFM_LOGO_TYPE_SD4_3_LARGE;
        break;
    case 0x04:
        t_logo_type = DFM_LOGO_TYPE_SD16_9_LARGE;
        break;
    case 0x05:
        t_logo_type = DFM_LOGO_TYPE_HD_LARGE;
        break;
    default:
        t_logo_type = DFM_LOGO_TYPE_UNKNOWN;
        break;
    }
    return t_logo_type;
}

static INT32 _dfm_hdlr_load_logo_data_by_key(HANDLE_T         h_root,
                                       UINT16   ui2_onid,
                                       UINT16   ui2_dd_data_id,
                                       UINT16   ui2_logo_id,
                                       UINT16   ui2_logo_ver,
                                       UINT16   ui2_scan_idx,
                                       LOGO_DATA_T**  ppt_logo_data)
{
    INT32 i4_ret;
    
    LOGO_DATA_T*  pt_data = NULL;
    TM_COND_T     e_cond;
    
    pt_data = DLIST_HEAD(&t_head_logo_data);    
    DBG_INFO((_DFM_INFO" get logo data onid[0x%x],dd_data_id[0x%x],logo_id[0x%x], scan_idx[0x%x].\n",
        ui2_onid, ui2_dd_data_id, ui2_logo_id,ui2_scan_idx));
    while (pt_data != NULL)
    {
        if ((pt_data->ui2_onid       == ui2_onid) &&
            (pt_data->ui2_dd_data_id == ui2_dd_data_id) &&
            (pt_data->ui2_logo_id    == ui2_logo_id) &&
            (pt_data->ui2_logo_ver   == ui2_logo_ver)&&
            (pt_data->ui2_scan_idx   == ui2_scan_idx))
        {
            (*ppt_logo_data) = pt_data;
            DBG_INFO((_DFM_INFO"  find logo data onid[0x%x],dd_data_id[0x%x],logo_id[0x%x], scan_idx[0x%x].\n",
                ui2_onid, ui2_dd_data_id, ui2_logo_id,ui2_scan_idx));

            /*if current CDT HANDLE is unavailable, reload again*/
            i4_ret = x_tm_get_cond(pt_data->h_cdt, &e_cond);
            if (i4_ret != TMR_OK)
            {
                DBG_ERROR((_DFM_ERROR"Get current CDT handle error.\r\n"));
                x_tm_free(pt_data->h_cdt);
                return DFMR_FAIL;
            }
            i4_ret = x_tm_get_cond(h_root, &e_cond);
            if ((i4_ret != TMR_OK) || (e_cond != TM_COND_AVAILABLE))
            {
                DBG_ERROR((_DFM_ERROR"DFM's root handle unavailable.\r\n"));
                return DFMR_FAIL;
            }
            if (pt_data->h_cdt != NULL_HANDLE)
            {
                x_tm_free(pt_data->h_cdt);
                pt_data->h_cdt = NULL_HANDLE;
            }
            i4_ret = x_isdb_si_load_cdt_by_key_id(h_root,
                                        ui2_onid, 
                                        ui2_dd_data_id, 
                                        (VOID*)(*ppt_logo_data),
                                        _dfm_hdlr_cdt_nfy_fct, 
                                        &((*ppt_logo_data)->h_cdt), 
                                        &e_cond);

            if (i4_ret != TMR_OK)
            {
                DBG_ERROR((_DFM_ERROR"load cdt failed, i4_ret=%d\r\n",  i4_ret));
                
                /* Oops, something went wrong. Clean-up. */
                DLIST_INSERT_HEAD(*ppt_logo_data, &t_free_logo_data, t_link);
                
                return DFMR_FAIL;
            }
            
            return DFMR_OK;
        }
        pt_data = DLIST_NEXT(pt_data, t_link);
    }

    i4_ret = DFMR_OUT_OF_RESOURCES;
    
    /*alloc logo_ctrl from _dfm_hdlr_free_logo_data*/
    if (!(DLIST_IS_EMPTY(&t_free_logo_data)))
    {
        *ppt_logo_data = DLIST_HEAD(&t_free_logo_data);
        DLIST_REMOVE(*ppt_logo_data, &t_free_logo_data, t_link);

        x_memset (((VOID*) (*ppt_logo_data)), 0, sizeof (LOGO_DATA_T));

        (*ppt_logo_data)->h_cdt              = NULL_HANDLE;
        (*ppt_logo_data)->h_root             = h_root;
        
        (*ppt_logo_data)->ui2_dd_data_id     = ui2_dd_data_id;
        (*ppt_logo_data)->ui2_logo_data_len  = 0;
        (*ppt_logo_data)->ui2_logo_id        = ui2_logo_id;
        (*ppt_logo_data)->ui2_logo_ver       = ui2_logo_ver;
        (*ppt_logo_data)->ui2_onid           = ui2_onid;
        (*ppt_logo_data)->ui2_scan_idx       = ui2_scan_idx;
        DLIST_INIT ( &((*ppt_logo_data)->t_logo_ctrl_list) );


        /*load cdt table from table manager*/
        i4_ret = x_tm_get_cond(h_root, &e_cond);
        if ((i4_ret != TMR_OK) || (e_cond != TM_COND_AVAILABLE))
        {
            DBG_ERROR((_DFM_ERROR"DFM's root handle unavailable.\r\n"));
            return DFMR_FAIL;
        }
        i4_ret = x_isdb_si_load_cdt_by_key_id(h_root,
                                    ui2_onid, 
                                    ui2_dd_data_id, 
                                    (VOID*)(*ppt_logo_data),
                                    _dfm_hdlr_cdt_nfy_fct, 
                                    &((*ppt_logo_data)->h_cdt), 
                                    &e_cond);

        if (i4_ret != TMR_OK)
        {
            DBG_ERROR((_DFM_ERROR"load cdt failed, i4_ret=%d\r\n",  i4_ret));
            
            /* Oops, something went wrong. Clean-up. */
            DLIST_INSERT_HEAD(*ppt_logo_data, &t_free_logo_data, t_link);
            
            return DFMR_FAIL;
        }
        DBG_INFO((_DFM_INFO" alloc free logo data onid[0x%x],dd_data_id[0x%x],logo_id[0x%x], cdt[0x%x].\n",
            ui2_onid, ui2_dd_data_id,ui2_logo_id,(*ppt_logo_data)->h_cdt));

        /*insert the logo_data to pt_head_logo_data*/
        DLIST_INSERT_TAIL(*ppt_logo_data, &t_head_logo_data, t_link);
        
        if (e_cond == TM_COND_AVAILABLE || e_cond == TM_COND_UPDATE)
        {
            DFM_MSG_T                   t_msg;  

            DBG_INFO((_DFM_INFO" Load CDT condition available\n"));
            
            t_msg.e_msg_type = MSG_TYPE_CDT_OBJ_AVAILABLE;      
            
            t_msg.pv_data = (VOID*)(*ppt_logo_data);
            dfm_msg_q_send(&t_msg);
        }
        
    }
        
    return i4_ret;     
        
}

VOID dfm_logo_unavailable_process(LOGO_DATA_T* pt_logo_data)
{
    LOGO_CTRL_T* pt_logo_req = NULL;
    HANDLE_INFO_LINK_T* pt_handle_info = NULL;
    
    
    DLIST_FOR_EACH(pt_logo_req, &(pt_logo_data->t_logo_ctrl_list), t_link_2)
    {
        pt_logo_req->e_cond = DFM_COND_UNAVAILABLE;

        DLIST_FOR_EACH(pt_handle_info, &(pt_logo_req->t_hdl_list), t_link)
        {
            /* Callback user the condition */
            pt_handle_info->pv_nfy( pt_handle_info->h_logo,
                                    DFM_COND_UNAVAILABLE,
                                    pt_handle_info->pv_tag,
                                    0);
        }
        
    }
    return;
}


typedef struct _DFM_ISDB_RENDER_BITMAP_STATE_T
{
    /*HANDLE_T            h_flash_surface;*/    /* it is effective only when fg_is_flashing is TURE in RENDER_PROPERTY_T */
    UINT8*              pui1_ori_bmp;
    UINT32              ui4_ori_bmp_len;
}   DFM_RENDER_BITMAP_STATE_T;

static  VOID    _dfm_img_nfy_fct(
        HANDLE_T                    h_handle,
        const VOID*                 pv_tag,
        IMG_NFY_EVENT_TYPE_T        e_event,
        const VOID*                 pv_data )
{
    return;
}

static  INT32   _dfm_show_bmp_render(
        UINT8*              pui1_ori_png,
        UINT32              ui4_ori_png_len,
        WGL_HIMG_TPL_T*     pt_wgl_img )
{
    HANDLE_T                    h_img = NULL_HANDLE;
    INT32                       i4_ret;
    UINT32                      ui4_ori_width;
    UINT32                      ui4_ori_height;
    IMG_ROTATE_OP_TYPE_T        e_rotate_type = IMG_ROTATE_CLOCKWS_0_OP_TYPE;
    GL_SURFACE_DESC_T           t_desc;
    HANDLE_T                    h_surface = NULL_HANDLE;

    /* get picture from image library */
    i4_ret = x_img_mopen ( 
        pui1_ori_png,
        ui4_ori_png_len,
        "PNG",
        _dfm_img_nfy_fct,
        NULL,
        &h_img );
    if (i4_ret < IMGR_OK)
    {
        goto FAIL;
    }

    i4_ret = x_img_get_dimension(
        h_img,
        INTRA_IMG_PRIMARY_IMG_TYPE,
        (SIZE_T *)(&ui4_ori_width),
        (SIZE_T *)(&ui4_ori_height) );
    if (i4_ret < IMGR_OK)
    {
        goto FAIL;
    }


    t_desc.e_clrmode  = (GL_COLORMODE_T)(GL_COLORMODE_ARGB_D8888);
    t_desc.h_clipper  = NULL_HANDLE;
    t_desc.h_palette  = NULL_HANDLE;
    t_desc.ui4_width  = ui4_ori_width;/*64;*/
    t_desc.ui4_height = ui4_ori_height;/*36;*/
    i4_ret = x_gl_surface_create(&t_desc, &h_surface);
    if (i4_ret < GLR_OK)
    {
        goto FAIL;
    }

    i4_ret = x_img_sync_load(
        h_img,
        INTRA_IMG_PRIMARY_IMG_TYPE,
        NULL,
        e_rotate_type,
        0,
        0,
        ui4_ori_width,
        ui4_ori_height,
        0,
        0,
        t_desc.ui4_width,
        t_desc.ui4_height,
        h_surface );
    if (i4_ret < IMGR_OK)
    {
        goto FAIL;
    }

    /*convert the surface to wgl img*/
    i4_ret = x_wgl_img_tpl_create_by_surf(
        WGL_IMG_ST_NO_SEGMENT,
        h_surface,
        pt_wgl_img);
    if (i4_ret < IMGR_OK)
    {
        goto FAIL;
    }
    /*free surface*/
    x_gl_obj_free( h_surface );
    /* free img handle */
    x_img_close( h_img );

    return DFMR_OK;

FAIL:
    if (NULL_HANDLE != h_surface)
    {
        x_gl_obj_free( h_surface );
    }
    if (NULL_HANDLE != h_img)
    {
        x_img_close( h_img );
    }    
    return DFMR_FAIL;
}



VOID dfm_logo_available_process(LOGO_DATA_T* pt_logo_data)
{
    INT32 i4_ret;
    TM_COND_T e_cond;
    UINT16    ui2_logo_cnt;
    LOGO_TYPE_ENTRY_T* pt_logo_type_node = NULL;
    WGL_HIMG_TPL_T         t_wgl_himg;
    
    LOGO_CTRL_T*  pt_logo_req = NULL;
    HANDLE_INFO_LINK_T* pt_handle_info = NULL;

    UINT8*    pui1_png_update_data = NULL;
    UINT16    ui2_png_update_data_len = 0;
    UINT16     ui2_idx;
    BOOL      b_notify = FALSE;

    i4_ret = x_isdb_si_get_num_logo_data_entries(pt_logo_data->h_cdt,&ui2_logo_cnt, &e_cond);
    if (i4_ret != TMR_OK)
    {       
        return;
    }
    DBG_INFO((_DFM_INFO" CDT notify success logo count[%d]!\r\t", ui2_logo_cnt));
    ui2_idx = 0;
    while (ui2_idx < ui2_logo_cnt)
    {
        ISDB_SI_LOGO_INFO_T t_logo_info;
        UINT8*    pu1_data = NULL;
        
        i4_ret = x_isdb_si_get_logo_entry_by_idx(pt_logo_data->h_cdt, ui2_idx, &t_logo_info, &e_cond);
        if (i4_ret != TMR_OK)
        {
            ui2_idx++;
            continue;
        }
        
        if ( (pt_logo_data->ui2_logo_id != t_logo_info.ui2_logo_id) ||
             (pt_logo_data->ui2_logo_ver!= t_logo_info.ui2_logo_version))
        {
            ui2_idx++;
            continue;
        }

        t_logo_info.ui1_logo_type = _dfm_hdlr_convert_logo_type(t_logo_info.ui1_logo_type);
        /*check old logo data list*/    
        {
            pt_logo_type_node = pt_logo_data->pt_logo_type_entry_list;
            while (pt_logo_type_node != NULL)
            {
                if (pt_logo_type_node->ui1_logo_type == t_logo_info.ui1_logo_type)
                {
                    break;
                }
                pt_logo_type_node = pt_logo_type_node->pt_next;
            }   
        }
        if (pt_logo_type_node != NULL)
        {
            ui2_idx++;
            continue;
        }
                
        pu1_data = (UINT8*)x_mem_alloc(t_logo_info.ui2_data_len);
        if (pu1_data == NULL)
        {
            DBG_ERROR((_DFM_ERROR"Out of Memory\r\n"));
            return ;
        }
        i4_ret = x_isdb_si_get_logo_data_by_idx(pt_logo_data->h_cdt, 
                                                ui2_idx,
                                                t_logo_info.ui2_data_len,
                                                pu1_data,
                                                &e_cond);
        if (i4_ret != TMR_OK)
        {
            x_mem_free(pu1_data);
            pu1_data = NULL;
                        
            break;
        }

        _dfm_hdlr_update_logo_png_data(pu1_data,
            t_logo_info.ui2_data_len, 
            &pui1_png_update_data,
            &ui2_png_update_data_len);
        
        x_mem_free(pu1_data);
        pu1_data = NULL;
        
        DBG_INFO((_DFM_INFO" start PNG_decode\r\t"));   
        /*decode the PNG_data*/
        #if 0
        pui1_png_data = (UINT8*) x_img_png_decode( pui1_png_update_data, 
                                                   (INT32)(ui2_png_update_data_len),
                                                   IMG_PNG_CM_ARGB_4444);
        #else
        i4_ret = _dfm_show_bmp_render(pui1_png_update_data,
                            (UINT32)(ui2_png_update_data_len),
                            &t_wgl_himg);
        if (i4_ret != DFMR_OK)
        {
            DBG_ERROR((_DFM_ERROR" PNG_decode error, logo_type [%d]\n", t_logo_info.ui1_logo_type)); 
            x_mem_free(pui1_png_update_data);
            pui1_png_update_data = NULL;
            ui2_idx ++;
            continue;
        }
        #endif
        DBG_INFO((_DFM_INFO" end PNG_decode\r\t"));     
        DBG_INFO((_DFM_INFO" logo_type [%d]\n", t_logo_info.ui1_logo_type));
            
        x_mem_free(pui1_png_update_data);
        pui1_png_update_data = NULL;

        pt_logo_type_node = NULL;
        {
            if (t_logo_info.ui2_logo_id== pt_logo_data->ui2_logo_id)                
            {
                if ((t_logo_info.ui2_logo_version != pt_logo_data->ui2_logo_ver)&&
                    (pt_logo_data->pt_logo_type_entry_list !=NULL ))
                {
                    _dfm_hdlr_free_logo_entry_list(pt_logo_data->pt_logo_type_entry_list);
                }
                        
                pt_logo_type_node = (LOGO_TYPE_ENTRY_T*)x_mem_alloc(sizeof(LOGO_TYPE_ENTRY_T));
                if (NULL == pt_logo_type_node)
                {
                    #if 0
                    x_img_png_free((VOID*) pui1_png_data);
                    #else
                    x_wgl_img_tpl_destroy(t_wgl_himg);
                    #endif
                    DBG_ERROR((_DFM_ERROR"Out of Memory\r\n"));
                    return ;            
                }
                pt_logo_type_node->ui1_logo_type = t_logo_info.ui1_logo_type;
                
                pt_logo_type_node->ui2_logo_data_len = t_logo_info.ui2_data_len;
                /*pt_logo_type_node->pui1_data = pui1_png_data;*/
                pt_logo_type_node->t_wgl_himg = t_wgl_himg;
                
                pt_logo_type_node->pt_next = pt_logo_data->pt_logo_type_entry_list;
                pt_logo_data->pt_logo_type_entry_list = pt_logo_type_node;

                pt_logo_type_node = NULL;
                b_notify = TRUE;
                
            }
        }       
        
        ui2_idx++;
    }

    if (b_notify == TRUE)
    {
        DLIST_FOR_EACH(pt_logo_req, &(pt_logo_data->t_logo_ctrl_list), t_link_2)
        {
            pt_logo_req->e_cond = DFM_COND_AVAILABLE;
            DLIST_FOR_EACH(pt_handle_info, &(pt_logo_req->t_hdl_list), t_link)
            {
                /* Callback user the condition */
                pt_handle_info->pv_nfy( pt_handle_info->h_logo,
                                    DFM_COND_AVAILABLE,
                                    pt_handle_info->pv_tag,
                                    0);
            }
        
        }
    }

    return;
}

VOID dfm_sdt_unavailable_process(LOGO_CTRL_T* pt_logo_ctrl)
{

    HANDLE_INFO_LINK_T* pt_handle_info = NULL;
    INT32 i4_ret;
    ISDB_SI_ON_TS_ID_INFO_T  t_ts_info;
    TM_COND_T   e_cond; 

    /*check current SDT handle*/
    i4_ret = x_isdb_si_get_ts_ids(pt_logo_ctrl->h_sdt, &t_ts_info, &e_cond);
    if (i4_ret != TMR_OK)
    {
        return ;
    }
    if (t_ts_info.ui2_on_id != pt_logo_ctrl->ui2_onid)
    {
        DBG_INFO((_DFM_INFO" [Unavailable]This isn't current SDT(on_id[0x%x],not on_id[0x%x])\n",t_ts_info.ui2_on_id,pt_logo_ctrl->ui2_onid));
        return; 
    }
    DBG_INFO((_DFM_INFO" [Unavailable]This is current SDT(on_id[0x%x])\n",t_ts_info.ui2_on_id));
    /*this is current TS*/
    pt_logo_ctrl->e_cond = DFM_COND_UNAVAILABLE;

    DLIST_FOR_EACH(pt_handle_info, &(pt_logo_ctrl->t_hdl_list), t_link)
    {
        /* Callback user the condition */
        pt_handle_info->pv_nfy( pt_handle_info->h_logo,
                                DFM_COND_UNAVAILABLE,
                                pt_handle_info->pv_tag,
                                0);
    }
        

    return;
}
static VOID _dfm_free_temp_logo_tran_info(LOGO_INFO_LIST_T* pt_tmp_logo_tran_info)
{
    LOGO_INFO_T*      pt_info = NULL;

    if (pt_tmp_logo_tran_info == NULL)
    {
        return;
    }

    while (!DLIST_IS_EMPTY(pt_tmp_logo_tran_info))
    {
        pt_info = DLIST_HEAD(pt_tmp_logo_tran_info);
        DLIST_REMOVE(pt_info, pt_tmp_logo_tran_info, t_link);
        x_mem_free((VOID*) pt_info);
    }
}
VOID dfm_sdt_available_process(LOGO_CTRL_T* pt_logo_ctrl)
{
    INT32 i4_ret;
    TM_COND_T e_cond;
    UINT16    ui2_svc_cnt;
    UINT16                      ui2_idx;

    LOGO_DATA_T*                pt_logo_data = NULL;
    LOGO_CTRL_T*                pt_logo_ctrl_node = NULL;
    
    LOGO_INFO_LIST_T   t_logo_info_list;
    LOGO_INFO_T*       pt_logo_info = NULL;
    LOGO_INFO_T*       pt_logo_temp_info = NULL;

    LOGO_TYPE_ENTRY_T* pt_temp_type_entry = NULL;
    LOGO_CTRL_T*       pt_logo_ctrl_temp = NULL;

    ISDB_SI_ON_TS_ID_INFO_T  t_ts_info;
    TM_FIRST_LOOP_DESCR_SEL_T   t_first_loop_descr_sel;
    UINT8                       ui1_descr_tag;


    /*check current SDT handle*/
    i4_ret = x_isdb_si_get_ts_ids(pt_logo_ctrl->h_sdt, &t_ts_info, &e_cond);
    if (i4_ret != TMR_OK)
    {
        return ;
    }
    if (t_ts_info.ui2_on_id != pt_logo_ctrl->ui2_onid)
    {       
        DBG_INFO((_DFM_INFO" [Available]This isn't current SDT(on_id[0x%x],not on_id[0x%x])\n",
            t_ts_info.ui2_on_id,pt_logo_ctrl->ui2_onid));
        return; 
    }
    DBG_INFO((_DFM_INFO" [Available]This is current SDT(on_id[0x%x])\n",
        t_ts_info.ui2_on_id));
    DLIST_INIT(&t_logo_info_list);

    i4_ret = x_isdb_si_get_num_svc_entries(pt_logo_ctrl->h_sdt,&ui2_svc_cnt, &e_cond);
    if (i4_ret != TMR_OK)
    {       
        return;
    }
    
    ui1_descr_tag                              = 0xCF; /* Logo_Transmission_Descriptor, defined in ARIB TR-B14 Vol.1 */
    t_first_loop_descr_sel.pui1_descr_tag_list = &ui1_descr_tag;
    t_first_loop_descr_sel.z_num_descr_tags    = 1;
    
    for (ui2_idx = 0; ui2_idx<ui2_svc_cnt; ui2_idx++)
    {   
        ISDB_SI_SVC_ENTRY_INFO_T t_svc_info;

        pt_logo_info = (LOGO_INFO_T*)x_mem_alloc(sizeof(LOGO_INFO_T));
        if (pt_logo_info == NULL)
        {
            DBG_ERROR((_DFM_ERROR"Out of Memory"));
            return ;
        }
        x_memset(pt_logo_info, 0, sizeof(LOGO_INFO_T));
        pt_logo_info->b_exist = FALSE;
        
        i4_ret = x_isdb_si_get_svc_entry(pt_logo_ctrl->h_sdt, ui2_idx, &t_svc_info, &e_cond);
        if ((i4_ret != TMR_OK) && (i4_ret == TMR_INV_GET_INFO))
        {
            x_mem_free(pt_logo_info);
            pt_logo_info = NULL;   
            continue;
        }
        if ((i4_ret != TMR_OK) ||
            (i4_ret == TMR_OK && (e_cond != TM_COND_AVAILABLE && 
                                  e_cond != TM_COND_UPDATE)))
        {
            i4_ret = DFMR_INV_HANDLE;
            x_mem_free(pt_logo_info);
            pt_logo_info = NULL;            
            _dfm_free_temp_logo_tran_info(&t_logo_info_list);
            return ;
        }

        pt_logo_info->ui2_onid = pt_logo_ctrl->ui2_onid;
        pt_logo_info->ui2_svc_id = t_svc_info.ui2_svc_id;
        pt_logo_info->ui2_scan_idx = pt_logo_ctrl->ui2_scan_idx;
        
        t_first_loop_descr_sel.ui2_loop_idx        = ui2_idx;

        i4_ret = x_tm_parse_obj(pt_logo_ctrl->h_sdt,
                                TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                                &t_first_loop_descr_sel,
                                pt_logo_info,
                                sizeof(LOGO_INFO_T),
                                _dfm_hdlr_parse_service_logo_desc_fct,
                                &e_cond);
        if ((i4_ret != TMR_OK) ||
            (i4_ret == TMR_OK && (e_cond != TM_COND_AVAILABLE && 
                                  e_cond != TM_COND_UPDATE)))
        {
            /*Printf("********ERROR!\n");*/
            i4_ret = DFMR_INV_HANDLE;
            x_mem_free(pt_logo_info);
            pt_logo_info = NULL;            
            _dfm_free_temp_logo_tran_info(&t_logo_info_list);
            return;
        }
        if (pt_logo_info->b_exist == FALSE)
        {
            x_mem_free(pt_logo_info);
            pt_logo_info = NULL;
            continue;
        }
        
        DLIST_INSERT_TAIL(pt_logo_info, &t_logo_info_list, t_link);
    }

    if (DLIST_IS_EMPTY(&t_logo_info_list))
    {
        DBG_INFO((_DFM_INFO"No logo_transition_descriptor in SDT, onid[0x%x]\n", pt_logo_ctrl->ui2_onid));
        return;
    }
    DLIST_FOR_EACH(pt_logo_info, &t_logo_info_list, t_link)
    {
        if (pt_logo_info->ui1_logo_trans_type == 0x02)
        {
            /*Find logo data type == 0x01, in current SDT.*/
            DLIST_FOR_EACH(pt_logo_temp_info, &t_logo_info_list, t_link)
            {
                if (pt_logo_temp_info->ui2_logo_id == pt_logo_info->ui2_logo_id)
                {
                    pt_logo_info->ui2_dd_data_id = pt_logo_temp_info->ui2_dd_data_id;
                    pt_logo_info->ui2_logo_id    = pt_logo_temp_info->ui2_logo_id;
                    pt_logo_info->ui2_logo_ver   = pt_logo_temp_info->ui2_logo_ver;
                    break;
                }
            }
        }
    }    
    pt_logo_info = NULL;
    DLIST_FOR_EACH(pt_logo_info, &t_logo_info_list, t_link)
    {
        /*Look up the logo ctrl data in request list for all services in current TS*/
        DLIST_FOR_EACH(pt_logo_ctrl_node, &t_head_logo_req, t_link_1)
        {
            if ((pt_logo_info->ui2_onid == pt_logo_ctrl_node->ui2_onid)&&
                (pt_logo_info->ui2_svc_id == pt_logo_ctrl_node->ui2_svc_id)&&
                (pt_logo_info->ui2_scan_idx == pt_logo_ctrl_node->ui2_scan_idx) )
            {
                if (pt_logo_info->ui1_logo_trans_type == 0x3)
                {
                    pt_temp_type_entry = (LOGO_TYPE_ENTRY_T*)x_mem_alloc(sizeof(LOGO_TYPE_ENTRY_T));
                    if (pt_temp_type_entry == NULL)
                    {
                        DBG_ERROR((_DFM_ERROR"Out of Memory\r\n"));
                        return;
                    }
                    pt_temp_type_entry->ui1_logo_type = DFM_LOGO_TYPE_SIMP;
                    pt_temp_type_entry->ui2_logo_data_len = SIMP_LOGO_LEN;
        
                    pt_temp_type_entry->pui1_data = (UINT8*)x_mem_alloc(SIMP_LOGO_LEN);
                    if (pt_temp_type_entry->pui1_data == NULL)
                    {
                        x_mem_free(pt_temp_type_entry);
                        DBG_ERROR((_DFM_ERROR"Out of Memory\r\n"));
                        return;
                    }
                    x_memcpy(pt_temp_type_entry->pui1_data, pt_logo_info->aui1_data, SIMP_LOGO_LEN );

                    pt_logo_ctrl->b_simp_logo = TRUE;
                    pt_logo_ctrl->pt_simp_logo_entry = pt_temp_type_entry;
                    pt_logo_ctrl->e_cond = DFM_COND_AVAILABLE;
        
                }
                else if ((pt_logo_info->ui1_logo_trans_type == 0x01)||
                    (pt_logo_info->ui1_logo_trans_type == 0x02))
                {     
                    /*Load CDT from transport stream */
                    i4_ret = _dfm_hdlr_load_logo_data_by_key( pt_logo_ctrl->h_root,
                                                        pt_logo_info->ui2_onid,
                                                        pt_logo_info->ui2_dd_data_id,
                                                        pt_logo_info->ui2_logo_id,
                                                        pt_logo_info->ui2_logo_ver,
                                                                    pt_logo_info->ui2_scan_idx,
                                                        &pt_logo_data
                                                        );
                    if (i4_ret != DFMR_OK)
                    {               
                        _dfm_free_temp_logo_tran_info(&t_logo_info_list);
                        return ;
                    }
                    /*set the point of logo_data_t to logo_ctrl_t*/
                    pt_logo_ctrl_node->pt_logo_data = pt_logo_data;

                    /*insert the logo request to logo_data_t! Check whether exist or not at before*/
                    pt_logo_ctrl_temp = DLIST_HEAD(&(pt_logo_data->t_logo_ctrl_list));
                    while (pt_logo_ctrl_temp != NULL)
                    {
                        if (pt_logo_ctrl_temp == pt_logo_ctrl_node)
                        {
                            break;
                        }
                        pt_logo_ctrl_temp = DLIST_NEXT(pt_logo_ctrl_temp, t_link_2);
                    }
                        
                    if (pt_logo_ctrl_temp == NULL)
                    {
                        DLIST_INSERT_TAIL(pt_logo_ctrl_node, &(pt_logo_data->t_logo_ctrl_list), t_link_2);
                    }

                    /*Check the logo request list and find logo request by onid and serviceid*/

                    if (pt_logo_data->pt_logo_type_entry_list != NULL)
                    {
                        pt_logo_ctrl_node->e_cond = DFM_COND_AVAILABLE;
                    }
                }
                else
                {
                    DBG_ERROR((_DFM_ERROR" ERROR rec\r\n"));
                }
                
                pt_logo_ctrl_node->b_sdt_ready_flag = TRUE; 
                
                /*logo transimission type is simple logo or logo type is 2*/
                if (pt_logo_ctrl_node->e_cond == DFM_COND_AVAILABLE)
                {       
                    /*Notify use*/
                    HANDLE_INFO_LINK_T* pt_handle_info = NULL;  

                    DLIST_FOR_EACH(pt_handle_info, &(pt_logo_ctrl_node->t_hdl_list), t_link)
                    {
                        /* Callback user the condition */
                        pt_handle_info->pv_nfy( pt_handle_info->h_logo,
                                                DFM_COND_AVAILABLE,
                                                pt_handle_info->pv_tag,
                                                0);
                    }       
                }
                            
                break;
            }
        }
        if (pt_logo_ctrl_node == NULL)
        {
            DBG_INFO((_DFM_INFO"No logo information in SDT loop, onid[0x%x],service_id[0x%x]\n", 
                pt_logo_info->ui2_onid,pt_logo_info->ui2_svc_id));
            return;
        }
    }
    /*Free temp logo transition information data list*/
    _dfm_free_temp_logo_tran_info(&t_logo_info_list);
                
    return;
}


BOOL logo_free_cb
(
    HANDLE_T                    h_obj,
    HANDLE_TYPE_T               e_type,
    VOID*                       pv_obj,
    VOID*                       pv_tag,
    BOOL                        b_req_handle
)
{
    BOOL                        b_return;
    LOGO_CTRL_T*                pt_logo;
    LOGO_CTRL_T*                pt_logo_node_temp = NULL;
    LOGO_DATA_T*                pt_logo_data;
    LOGO_DATA_T*                pt_logo_temp_data = NULL;
    HANDLE_INFO_LINK_T*         pt_hdlr_info = NULL;
    HANDLE_INFO_LINK_T*         pt_hdlr_info_node = NULL;
    LOGO_CTRL_T*                pt_logo_node = NULL;

    b_return = FALSE;

    /* Only do the following if it is a valid handle type. */
    if (e_type == HT_DFM_LOGO)
    {
        /* Only free the handle if this callback is */
        /* called via API "rm_close".               */  
        DBG_INFO((_DFM_INFO" free logo handle [0x%x]\n",h_obj));
        if ((pt_logo = ((LOGO_CTRL_T*) pv_obj)) != NULL)
        {
            b_return = TRUE;
            
            DLIST_FOR_EACH(pt_hdlr_info, &(pt_logo->t_hdl_list), t_link)
            {
                if (pt_hdlr_info->h_logo == h_obj) 
                {
                    DBG_INFO((_DFM_INFO" find free logo handle [0x%x]\n",h_obj));
                    break;
                }
            }
            /*found nothing!*/
            if (pt_hdlr_info == NULL)
            {
                return b_return;
            }

            DLIST_FOR_EACH(pt_hdlr_info_node,&(pt_logo->t_hdl_list),t_link)
            {
                if (pt_hdlr_info_node->h_logo == pt_hdlr_info->h_logo)
                {
                    DLIST_REMOVE(pt_hdlr_info_node,&(pt_logo->t_hdl_list),t_link);
                    break;
                }
            }
            
            if (DLIST_IS_EMPTY( &(pt_logo->t_hdl_list)) )
            {               
                if ((pt_logo_data = pt_logo->pt_logo_data) != NULL)
                {
                    DBG_INFO((_DFM_INFO" empty logo ctrl onid[0x%x],serviceid[0x%x]\n",
                        pt_logo->ui2_onid,pt_logo->ui2_svc_id));
                
                    /*Remove the logo_ctrl from logo_data's logo_ctrl_list*/
                    DLIST_FOR_EACH(pt_logo_node, &(pt_logo_data->t_logo_ctrl_list), t_link_2)
                    {
                        if (pt_logo_node == pt_logo) 
                        {
                            DBG_INFO((_DFM_INFO" find free logo ctrl onid[0x%x],serviceid[0x%x]\n",
                                pt_logo->ui2_onid,pt_logo->ui2_svc_id));
                            break;
                        }
                    }
                
                    if (pt_logo_node != NULL)
                    {
                        DLIST_FOR_EACH(pt_logo_node_temp, &(pt_logo_data->t_logo_ctrl_list), t_link_2)
                        {
                            if ((pt_logo_node_temp->ui2_onid ==  pt_logo->ui2_onid)&&
                                (pt_logo_node_temp->ui2_svc_id ==  pt_logo->ui2_svc_id))
                            {
                                DLIST_REMOVE(pt_logo_node_temp, &(pt_logo_data->t_logo_ctrl_list), t_link_2);
                                break;
                            }
                        }
                        DBG_INFO((_DFM_INFO" free logo ctrl onid[0x%x],serviceid[0x%x]\n",
                            pt_logo->ui2_onid,pt_logo->ui2_svc_id));

                        if (DLIST_IS_EMPTY( &(pt_logo_data->t_logo_ctrl_list)))
                        {
                            DBG_INFO((_DFM_INFO" empty logo data onid[0x%x],dd_data_id[0x%x], logo_id[%x]\n",
                                pt_logo_data->ui2_onid, 
                                pt_logo_data->ui2_dd_data_id,
                                pt_logo_data->ui2_logo_id));
                        
                            _dfm_hdlr_free_logo_data(pt_logo_data);

                            /*remove from t_head_logo_data*/
                            DLIST_FOR_EACH(pt_logo_temp_data, &t_head_logo_data, t_link)
                            {
                                if ((pt_logo_temp_data->ui2_onid == pt_logo_data->ui2_onid)&&
                                    (pt_logo_temp_data->ui2_logo_id == pt_logo_data->ui2_logo_id)&&
                                    (pt_logo_temp_data->ui2_dd_data_id == pt_logo_data->ui2_dd_data_id)&&
                                    (pt_logo_temp_data->ui2_logo_ver == pt_logo_data->ui2_logo_ver))
                                {
                                    DLIST_REMOVE(pt_logo_data, &t_head_logo_data, t_link);
                                    break;
                                }
                            }                            
                            /*add to pt_free_logo_data*/
                            DLIST_INSERT_TAIL(pt_logo_data, &t_free_logo_data, t_link);
                        }
                    }
                }
                _dfm_hdlr_free_logo(pt_logo);
                
                /*remove from the head_logo_req */
                DLIST_FOR_EACH(pt_logo_node, &t_head_logo_req, t_link_1)
                {
                    if ((pt_logo_node->ui2_onid ==  pt_logo->ui2_onid)&&
                        (pt_logo_node->ui2_svc_id ==  pt_logo->ui2_svc_id))
                    {
                        DLIST_REMOVE(pt_logo_node, &t_head_logo_req, t_link_1);
                        break;
                    }
                }
                /*insert to the  free_logo_req */
                DLIST_INSERT_TAIL(pt_logo, &t_free_logo_req, t_link_1);
            }
        }
    }
    else
    {
    }

    return (b_return);
}


INT32 dfm_link_logo_req(LOGO_CTRL_T*   pt_logo,
                    VOID*          pv_tag,
                    x_dfm_nfy_fct  pt_nfy,
                    HANDLE_T       h_logo)
{
    HANDLE_INFO_LINK_T* pt_hdr_info = NULL;

    if (pt_logo == NULL)
    {
        return DFMR_INV_ARG;
    }

    pt_hdr_info = (HANDLE_INFO_LINK_T*)x_mem_alloc(sizeof(HANDLE_INFO_LINK_T));
    if (pt_hdr_info == NULL)
    {
        return DFMR_OUT_OF_RESOURCES;
    }

    pt_hdr_info->h_logo = h_logo;
    pt_hdr_info->pv_tag = pv_tag;
    pt_hdr_info->pv_nfy = pt_nfy;

    DLIST_INSERT_TAIL(pt_hdr_info, &(pt_logo->t_hdl_list), t_link);

    return DFMR_OK;
    
}

INT32 dfm_logo_chg_svc(UINT16           ui2_onid,
                         UINT16           ui2_svc_id,
                         UINT16           ui2_scan_idx)
{
    INT32 i4_ret = 0;
    
    LOGO_CTRL_T*  pt_logo = NULL;
    LOGO_CTRL_T*  pt_logo_ctrl = NULL;

    TM_COND_T         e_cond;

    /*search from load logo ctrl list by onid and service_id*/
    pt_logo = DLIST_HEAD(&t_head_logo_req);
    DBG_INFO((_DFM_INFO" change service and look for logo_ctrl_t onid[0x%x],serviceid[0x%x],scan_idx[0x%x].\n",
            ui2_onid,ui2_svc_id,ui2_scan_idx));
    while (pt_logo != NULL)
    {
        if ((pt_logo->ui2_onid == ui2_onid)&&
            (pt_logo->ui2_svc_id == ui2_svc_id)&&
            (pt_logo->ui2_scan_idx == ui2_scan_idx))
        {
            pt_logo_ctrl = pt_logo;
            DBG_INFO((_DFM_INFO" find change service logo ctrl onid[0x%x],serviceid[0x%x],scan_idx[0x%x].\n",
            ui2_onid,ui2_svc_id,ui2_scan_idx));
        }
        pt_logo = DLIST_NEXT(pt_logo, t_link_1);
    }   
    g_ui2_onid     = ui2_onid;
    g_ui2_scan_idx = ui2_scan_idx;

    if (pt_logo_ctrl != NULL)
    {
        if (pt_logo_ctrl->h_sdt == h_current_sdt)
        {
            if (pt_logo_ctrl->h_sdt != NULL_HANDLE)
            {
                x_tm_free(pt_logo_ctrl->h_sdt);
                pt_logo_ctrl->h_sdt = NULL_HANDLE;
            }
            h_current_sdt = NULL_HANDLE;
        }
        else
        {
            if (pt_logo_ctrl->h_sdt != NULL_HANDLE)
            {
                x_tm_free(pt_logo_ctrl->h_sdt);
                pt_logo_ctrl->h_sdt = NULL_HANDLE;
            }
            if (h_current_sdt != NULL_HANDLE)
            {
                x_tm_free(h_current_sdt);
                h_current_sdt = NULL_HANDLE;
            }
        }
        DBG_INFO((_DFM_INFO" load current sdt onid[0x%x],serviceid[0x%x],scan_idx[0x%x].\n",
            ui2_onid,ui2_svc_id,ui2_scan_idx));
        i4_ret = x_isdb_si_load_sdt(pt_logo_ctrl->h_root,
                            ui2_onid,
                            ui2_onid,
                            (VOID*)(pt_logo_ctrl),
                            _dfm_hdlr_sdt_nfy_fct, 
                            &(pt_logo_ctrl->h_sdt), 
                            &e_cond);

        if (i4_ret != TMR_OK)
        {
            DBG_ERROR((_DFM_ERROR" load sdt failed, i4_ret=%d\r\n",  i4_ret));
            /* Oops, something went wrong. Clean-up. */
                        
            return DFMR_FAIL;
        }                   

        h_current_sdt = pt_logo_ctrl->h_sdt;
        
        if (e_cond == TM_COND_AVAILABLE || e_cond == TM_COND_UPDATE)
        {
            DFM_MSG_T                   t_msg;  

            DBG_INFO((_DFM_INFO" Load SDT condition available!\n"));
            
            t_msg.e_msg_type = MSG_TYPE_SDT_OBJ_AVAILABLE;      
            
            t_msg.pv_data = pt_logo_ctrl;
            dfm_msg_q_send(&t_msg);
        }       
        
        return DFMR_OK;
            
    }

    if (h_current_sdt != NULL_HANDLE)
    {
        x_tm_free(h_current_sdt);
        h_current_sdt = NULL_HANDLE;
    }    
    return i4_ret;
}

INT32 dfm_load_logo_ctrl_by_key(HANDLE_T        h_root,
                               UINT16           ui2_onid,
                               UINT16            ui2_svc_id,
                               UINT16            ui2_scan_idx,
                               LOGO_CTRL_T**     ppt_logo)
{
    INT32 i4_ret;
    
    LOGO_CTRL_T*  pt_logo = NULL;

    TM_COND_T         e_cond;

    /*search from load logo ctrl list by onid and service_id*/
    pt_logo = DLIST_HEAD(&t_head_logo_req);
    DBG_INFO((_DFM_INFO" load logo ctrl onid[0x%x],serviceid[0x%x],scan_idx[0x%x].\n",
            ui2_onid,ui2_svc_id,ui2_scan_idx));
    while (pt_logo != NULL)
    {
        if ((pt_logo->ui2_onid == ui2_onid)&&
            (pt_logo->ui2_svc_id == ui2_svc_id)&&
            (pt_logo->ui2_scan_idx == ui2_scan_idx))
        {
            *ppt_logo = pt_logo;
            DBG_INFO((_DFM_INFO" find logo ctrl onid[0x%x],serviceid[0x%x],scan_idx[0x%x].\n",
                    ui2_onid,ui2_svc_id,ui2_scan_idx));
            return DFMR_OK;
        }
        pt_logo = DLIST_NEXT(pt_logo, t_link_1);
    }   

    i4_ret = DFMR_OUT_OF_RESOURCES;
    /*alloc logo_ctrl from free_logo_req*/
    if (!(DLIST_IS_EMPTY(&t_free_logo_req)))
    {
        *ppt_logo = DLIST_HEAD(&t_free_logo_req);
        DLIST_REMOVE(*ppt_logo, &t_free_logo_req, t_link_1);
        
        x_memset (((VOID*) (*ppt_logo)), 0, sizeof (LOGO_CTRL_T));

        (*ppt_logo)->ui2_onid     = ui2_onid;
        (*ppt_logo)->ui2_svc_id   = ui2_svc_id;
        (*ppt_logo)->ui2_scan_idx = ui2_scan_idx;
        
        (*ppt_logo)->pt_logo_data = NULL;
        (*ppt_logo)->b_sdt_ready_flag = FALSE;
        (*ppt_logo)->b_simp_logo = FALSE;
        
        DLIST_INIT( &((*ppt_logo)->t_hdl_list) );
        
        (*ppt_logo)->e_cond       = DFM_COND_TRANSITION;
        (*ppt_logo)->h_root       = h_root;
        
        DBG_INFO((_DFM_INFO" alloc free logo ctrl onid[0x%x],serviceid[0x%x],scan_idx[0x%x].\n",
            ui2_onid,ui2_svc_id,ui2_scan_idx));
        DBG_INFO((_DFM_INFO" load actual sdt onid[0x%x],serviceid[0x%x],scan_idx[0x%x].\n",
            ui2_onid,ui2_svc_id,ui2_scan_idx));

        /*If this request is current TS*/
        if ((g_ui2_onid == ui2_onid)&&
            (g_ui2_scan_idx == ui2_scan_idx))
        {            
            if ((h_current_sdt == NULL_HANDLE)||
                ((i4_ret = x_tm_get_cond(h_current_sdt, &e_cond))!= TMR_OK)||
                ((e_cond != TM_COND_AVAILABLE) && (e_cond != TM_COND_UPDATE)&&((e_cond != TM_COND_TRANSITION))))
            {
                if (h_current_sdt != NULL_HANDLE)
                {
                    x_tm_free(h_current_sdt);
                    h_current_sdt = NULL_HANDLE;
                }
                i4_ret = x_isdb_si_load_sdt(h_root,
                                    ui2_onid,
                                    ui2_onid,
                                    (VOID*)(*ppt_logo),
                                    _dfm_hdlr_sdt_nfy_fct, 
                                    &((*ppt_logo)->h_sdt), 
                                    &e_cond);

                if (i4_ret != TMR_OK)
                {
                    DBG_ERROR((_DFM_ERROR" load sdt failed, i4_ret=%d\r\n",  i4_ret));
                    /* Oops, something went wrong. Clean-up. */
                    DLIST_INSERT_HEAD(*ppt_logo, &t_free_logo_req, t_link_1);
                                
                    return DFMR_FAIL;
                }

                h_current_sdt = (*ppt_logo)->h_sdt;
                /*insert the logo request to t_head_logo_req*/
                DLIST_INSERT_TAIL(*ppt_logo, &t_head_logo_req, t_link_1);
                
                if (e_cond == TM_COND_AVAILABLE || e_cond == TM_COND_UPDATE)
                {
                    DFM_MSG_T                   t_msg;  

                    DBG_INFO((_DFM_INFO" Load SDT condition available!\n"));
                    
                    t_msg.e_msg_type = MSG_TYPE_SDT_OBJ_AVAILABLE;      
                    
                    t_msg.pv_data = (VOID*)(*ppt_logo);
                    dfm_msg_q_send(&t_msg);
                }
            }
            else /*send message process*/
            { 
                (*ppt_logo)->h_sdt = h_current_sdt;
                
                DLIST_INSERT_TAIL(*ppt_logo, &t_head_logo_req, t_link_1);
                if (e_cond == TM_COND_AVAILABLE || e_cond == TM_COND_UPDATE)
                {
                    DFM_MSG_T                   t_msg;  

                    (*ppt_logo)->b_sdt_ready_flag = TRUE;
                    DBG_INFO((_DFM_INFO" Load SDT condition available!\n"));
                    
                    t_msg.e_msg_type = MSG_TYPE_SDT_OBJ_AVAILABLE;      
                    
                    t_msg.pv_data = (VOID*)(*ppt_logo);
                    dfm_msg_q_send(&t_msg);
                }
            }
            return DFMR_OK; 
            
        }

        /*If it is not currrent TS or never call change service*/
        /*insert the logo request to t_head_logo_req*/
        DLIST_INSERT_TAIL(*ppt_logo, &t_head_logo_req, t_link_1);
        return DFMR_OK;
            
    }
        
    return i4_ret;
}

BOOL dfm_logo_hdlr_init (UINT16   ui2_num_logo_req,
                         UINT16   ui2_num_logo_data)
{
    LOGO_CTRL_T*      pt_logo_req = NULL;
    LOGO_DATA_T*      pt_logo_data = NULL;
    UINT16            ui2_num_logo_info;

    if (!b_init)
    {       
        ui2_num_logo_info = ui2_num_logo_data;
        
        if (ui2_num_logo_info > 0) 
        {
            /* Allocate space for the logo . */
            if ((pt_logo_data= (LOGO_DATA_T*) x_mem_alloc (sizeof (LOGO_DATA_T) * ui2_num_logo_data)) != NULL)
            {
                b_init = TRUE;

                DLIST_INIT(&t_head_logo_data);
                DLIST_INIT(&t_free_logo_data);

                /* Initialize the logos. */
                x_memset (((VOID*) pt_logo_data), 0, (sizeof (LOGO_DATA_T) * ui2_num_logo_data));

                while (ui2_num_logo_info > 0)
                {
                    DLIST_INSERT_HEAD(pt_logo_data, &t_free_logo_data, t_link);
                    pt_logo_data++;
                    ui2_num_logo_info--;
                }
            }
            else
            {
                DBG_ERROR((_DFM_ERROR" malloc failed\r\n"));
                return FALSE;
            }
        }   

        if (ui2_num_logo_req > 0)
        {
            /* Allocate space for the logo requests. */
            if ((pt_logo_req = (LOGO_CTRL_T*) x_mem_alloc (sizeof (LOGO_CTRL_T) * ui2_num_logo_req)) != NULL)
            {
               
                b_init = TRUE;

                DLIST_INIT(&t_head_logo_req);
                DLIST_INIT(&t_free_logo_req);

                /* Initialize the logo requests. */
                x_memset (((VOID*) pt_logo_req), 0, (sizeof (LOGO_CTRL_T) * ui2_num_logo_req));

                while (ui2_num_logo_req > 0)
                {
                    DLIST_INSERT_HEAD(pt_logo_req, &t_free_logo_req, t_link_1);
                    pt_logo_req++;
                    ui2_num_logo_req--;
                }
            }
            else
            {
                if (pt_logo_data != NULL)
                {
                /* Oops, out of resources. Clean-up. */
                    x_mem_free ((VOID*) pt_logo_data);
                    pt_logo_data = NULL;
                }
            }            
        }   
        h_current_sdt = NULL_HANDLE;
        g_ui2_onid = 0;
        g_ui2_scan_idx = 0;
    }
    return (b_init);
}

VOID dfm_logo_hdlr_deinit()
{   
    LOGO_DATA_T*    pt_logo_data = NULL;
    LOGO_CTRL_T*    pt_logo_ctrl = NULL;
    
    /*free logo_ctrl_t*/
    while (!DLIST_IS_EMPTY(&(t_head_logo_req)))
    {
        pt_logo_ctrl = DLIST_HEAD(&(t_head_logo_req));
        DLIST_REMOVE(pt_logo_ctrl, &(t_head_logo_req), t_link_1);
        
        _dfm_hdlr_free_logo(pt_logo_ctrl);
        x_mem_free(pt_logo_ctrl);
        pt_logo_ctrl = NULL;
    }
    while (!DLIST_IS_EMPTY(&(t_free_logo_req)))
    {
        pt_logo_ctrl = DLIST_HEAD(&(t_free_logo_req));
        DLIST_REMOVE(pt_logo_ctrl, &(t_free_logo_req), t_link_1);
        
        _dfm_hdlr_free_logo(pt_logo_ctrl);
        x_mem_free(pt_logo_ctrl);
        pt_logo_ctrl = NULL;
    }

    /*free logo_info_t*/
    while (!DLIST_IS_EMPTY(&(t_head_logo_data)))
    {
        pt_logo_data = DLIST_HEAD(&(t_head_logo_data));
        DLIST_REMOVE(pt_logo_data, &(t_head_logo_data), t_link);        
        _dfm_hdlr_free_logo_data(pt_logo_data);
        x_mem_free(pt_logo_data);
        pt_logo_data = NULL;
    }
    while (!DLIST_IS_EMPTY(&(t_free_logo_data)))
    {
        pt_logo_data = DLIST_HEAD(&(t_free_logo_data));
        DLIST_REMOVE(pt_logo_data, &(t_free_logo_data), t_link);
        
        if (pt_logo_data->pt_logo_type_entry_list != NULL)
        {
            _dfm_hdlr_free_logo_entry_list(pt_logo_data->pt_logo_type_entry_list);
            pt_logo_data->pt_logo_type_entry_list = NULL;
        }
        x_mem_free(pt_logo_data);
        pt_logo_data = NULL;
    }
    return;
}

#ifdef DEBUG 
extern LOGO_DATA_LIST_T* dfm_get_logo_data( VOID )
{
    LOGO_DATA_LIST_T* pt_logo_data;

    pt_logo_data = &t_head_logo_data;
    return pt_logo_data;
}

extern LOGO_CTRL_LIST_T* dfm_get_logo_ctrl( VOID )
{
    LOGO_CTRL_LIST_T* pt_logo_ctrl;

    pt_logo_ctrl = &t_head_logo_req;
    return pt_logo_ctrl;
}
#endif


