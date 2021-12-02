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
 * Copyright (c) 2004, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: sbtl_mp4_vobsub_page_inst_mngr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 * History:
 *
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
#include "os/inc/os.h"
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"

#include "mutil/mm_util.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_widget.h"
#include "mutil/sbtl_engine/sbtl_mp4/sbtl_mp4_vobsub_page_inst_mngr.h"

#include "inc/x_mm_common.h"


#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"

#include "../../mm_util.h"

#include "../sbtl_dbg.h"
#include "../sbtl_struct.h"
#include "../sbtl_utils.h"
#include "../sbtl_register.h"
#include "../sbtl_state.h"
#include "../sbtl_widget.h"

#include "sbtl_mp4_vobsub_page_inst_mngr.h"

#endif /*__KERNEL__*/

#ifndef __KERNEL__
    #define x_dbg_stmt_ex x_dbg_stmt
    #else 
    #define x_dbg_stmt_ex(fmt...)
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    500

#define CARRIAGE_RETURN             0x0d
#define LINE_FEED                   0x0a
#define CHAR_TAB                    0x09
#define CHAR_SPACE                  0x20


#define SUBTITLEPARSER_SUCCESS 0
#define SUBTITLEPARSER_BUFFER_TOO_SMALL 1
#define SUBTITLEPARSER_INVALID_DATA 2


typedef enum
{
    SBTL_SEARCH_BEGIN,
    SBTL_SEARCH_MIDDLE
} SBTL_SEARCH_TYPE_T;

typedef enum
{
    NUMBER_TYPE_DECIMAL,
    NUMBER_TYPE_HEX,
    NUMBER_TYPE_MAX
} NUMBER_TYPE_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static VOID _sbtl_mp4_vobsub_remove(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static INT32 _sbtl_mp4_vobsub_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static INT32 _sbtl_mp4_vobsub_stop(SBTL_PAGE_INST_MNGR_T*   pt_mngr);

static INT32 _sbtl_mp4_vobsub_need_dmx(SBTL_PAGE_INST_MNGR_T*   pt_mngr, 
                                   BOOL*                    pb_need);

static INT32 _sbtl_mp4_vobsub_handle_timing(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        PTS_T                   t_pts);

static INT32 _sbtl_mp4_vobsub_config(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                            VOID*                      pv_get_info,
                                 UINT32                     ui4_set_type);


static INT32 _sbtl_mp4_vobsub_new_page(SBTL_PAGE_INST_MNGR_T*   pt_mngr);


static SBTL_PAGE_INST_MNGR_T* _mp4_vobsub_page_mngr_constructor (SBTL_ENGINE_T* pt_engine);

static VOID _mp4_vobsub_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*    pt_mngr);

static INT32 _sbtl_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr);
static VOID _sbtl_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/


static SBTL_MNGR_FCT_TBL_T t_mp4_vobsub_fnct_tbl = 
{
    FALSE,
    _mp4_vobsub_page_mngr_constructor,
    _mp4_vobsub_page_mngr_destructor,
    NULL
};



/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _mp4_vobsub_parse_dcsqt(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                         UINT32                  ui4_packetsize,
                         UINT32                  ui4_datasize)
{
    BOOL                                b_break;
    UINT32                              ui4_next_dcsq;
    UINT32                              ui4_i;
    UINT8*                              aui1_spu;
    UINT16                              ui2_pal;
    UINT32                              ui4_dcsq_stm;
    UINT32                              ui4_len;
    UINT8                               ui1_pallete_idx;
    SBTL_MP4_VOBSUB_SUB_INFO_T*             pt_sub_info;
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*    pt_private;

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    pt_sub_info = &pt_private->t_sub_info;
    
    pt_sub_info->aui1_spu_data = pt_private->aui1_pes_buf;
    pt_sub_info->ui4_packetsize = ui4_packetsize;
    pt_sub_info->ui4_datasize = ui4_datasize;

    aui1_spu = pt_private->aui1_pes_buf;
    ui4_next_dcsq = ui4_datasize;
    ui2_pal=0;

    do
    {
        ui4_i = ui4_next_dcsq;

        ui4_dcsq_stm = (aui1_spu[ui4_i] << 8) | aui1_spu[ui4_i+1];
        ui4_i += 2;

        ui4_next_dcsq = (aui1_spu[ui4_i] << 8) | aui1_spu[ui4_i+1];
        ui4_i += 2;

        b_break = FALSE;

        while(!b_break)
        {
            switch(aui1_spu[ui4_i])
            {
            case 0x00: 
                ui4_len = 0; 
                break;
                
            case 0x01: 
                ui4_len = 0; 
                break;
                
            case 0x02: 
                ui4_len = 0; 
                break;
                
            case 0x03: 
                ui4_len = 2; 
                break;
                
            case 0x04: 
                ui4_len = 2; 
                break;
                
            case 0x05: 
                ui4_len = 6; 
                break;
                
            case 0x06: 
                ui4_len = 4; 
                break;
                
            default: 
                ui4_len = 0; 
                break;
            }

            if(ui4_i+ui4_len >= ui4_packetsize)
            {
                DBG_INFO(("Warning: Wrong subpicture parameter block ending\n"));
                break;
            }

            switch(aui1_spu[ui4_i++])
            {
            case 0x00: /* forced start displaying */
                pt_sub_info->b_forced = TRUE;
                break;

            case 0x01: /* start displaying */
                pt_sub_info->b_start_pts_offset = TRUE;
                
                pt_sub_info->b_forced = FALSE;
                pt_sub_info->t_start_pts_offset = (ui4_dcsq_stm<<10);
                break;

            case 0x02: /* stop displaying */
                pt_sub_info->b_end_pts_offset = TRUE;
                
                pt_sub_info->t_end_pts_offset = (ui4_dcsq_stm<<10);
                break;

            case 0x03:
                pt_sub_info->b_pallete = TRUE;
                
                ui2_pal = (aui1_spu[ui4_i] << 8) | aui1_spu[ui4_i+1];
                ui4_i += 2;
                break;

            case 0x04:
                pt_sub_info->b_contrast = TRUE;
                
                pt_sub_info->ui2_contrast = (aui1_spu[ui4_i] << 8) | aui1_spu[ui4_i+1];
                ui4_i += 2;
                break;

            case 0x05:
                pt_sub_info->b_display_area = TRUE;
                
                pt_sub_info->i4_left = (INT32)((aui1_spu[ui4_i] << 4) + (aui1_spu[ui4_i+1] >> 4));
                pt_sub_info->i4_top = (INT32)((aui1_spu[ui4_i+3] << 4) + (aui1_spu[ui4_i+4] >> 4));
                pt_sub_info->i4_right = (INT32)(((aui1_spu[ui4_i+1] & 0x0f) << 8) + aui1_spu[ui4_i+2]);
                pt_sub_info->i4_bottom = (INT32)(((aui1_spu[ui4_i+4] & 0x0f) << 8) + aui1_spu[ui4_i+5]);

                pt_sub_info->ui2_width = (UINT16)(pt_sub_info->i4_right - pt_sub_info->i4_left + 1);
                pt_sub_info->ui2_height= (UINT16)(pt_sub_info->i4_bottom - pt_sub_info->i4_top + 1);
                
                ui4_i += 6;
                break;

            case 0x06:
                pt_sub_info->b_data_offset = TRUE;
                
                pt_sub_info->aui2_data_offset[0] = (aui1_spu[ui4_i] << 8) + aui1_spu[ui4_i+1];
                ui4_i += 2;

                pt_sub_info->aui2_data_offset[1] = (aui1_spu[ui4_i] << 8) + aui1_spu[ui4_i+1];
                ui4_i += 2;

                pt_sub_info->i4_field_offset = (INT32)pt_sub_info->aui2_data_offset[1]
                                             - (INT32)pt_sub_info->aui2_data_offset[0];

                pt_sub_info->aui1_rle_data = pt_private->aui1_pes_buf 
                                            + pt_sub_info->aui2_data_offset[0];

                pt_sub_info->i4_rle_size = (INT32)(pt_sub_info->ui4_datasize
                                                 - pt_sub_info->aui2_data_offset[0]);
                break;

            case 0xff: /* end of dcsq */
                b_break = TRUE;
                continue;

            default: /* skip this dcsq */
                b_break = TRUE;
                break;
            }
        }
    }while(ui4_i <= ui4_next_dcsq && ui4_i < ui4_packetsize && ui4_next_dcsq < ui4_packetsize-4);

    if(ui4_next_dcsq > ui4_packetsize-4)
    {
        DBG_INFO(("Warning: Wrong subpicture parameter block ending\n"));
    }

    for(ui4_i = 0; ui4_i < 4; ui4_i++) 
    {
        if (pt_sub_info->b_pallete)
        {
            ui1_pallete_idx = (UINT8)((ui2_pal >> (ui4_i << 2)) & 0xf);
            pt_private->at_clut_sub[ui4_i] = pt_private->at_clut_idx[ui1_pallete_idx];
        }
        else
        {
            pt_private->at_clut_sub[ui4_i] = pt_private->at_clut_idx[ui4_i];
        }
    }
}

#if 0
/* read a nibble from aui1_buffer */
static UINT8 _get_nibble(UINT8*     aui1_buffer, 
                         INT32      i4_offs)
{
    INT32 i4_b = i4_offs/2;
    INT32 i4_h = (i4_offs%2 == 1);

    UINT8 ui1_ret;

    if(i4_h == 0)
    {
        ui1_ret = (aui1_buffer[i4_b] & 0xF0) >> 4;
    }
    else
    {
        ui1_ret = (aui1_buffer[i4_b] & 0x0F) >> 0;
    }

    return ui1_ret;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
/* set a nibble inside aui1_buffer */
static VOID _set_nibble(UINT8*      aui1_buffer, 
                        INT32       i4_offs, 
                        UINT8       ui1_n)
{
    INT32 i4_b = i4_offs/2;
    INT32 i4_h = (i4_offs%2 == 1);

    if(i4_h == 0)
    {
        aui1_buffer[i4_b] = (aui1_buffer[i4_b] & 0x0F) | (ui1_n << 4);
    }
    else
    {
        aui1_buffer[i4_b] = (aui1_buffer[i4_b] & 0xF0) | (ui1_n << 0);
    }

    return;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
UINT8 _mp4_vobsub_get_nibble(UINT8*     aui1_spu,
                         BOOL*      pb_aligned,
                         UINT32*    pui4_offset)
{
    UINT8   ui1_ret;
    
    ui1_ret = (aui1_spu[*pui4_offset] >> ((*pb_aligned) << 2)) & 0x0f;

    *pb_aligned = !(*pb_aligned);

    (*pui4_offset) += (*pb_aligned);

    return ui1_ret;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mp4_vobsub_rle2raw(SBTL_MP4_VOBSUB_SUB_INFO_T*    pt_sub_info,
                             INT32                      i4_rle_size,
                             UINT32*                    pui4_raw_size, 
                             UINT8*                     pui1_raw_data)
{
    UINT8       ui1_plane;
    BOOL        b_aligned;
    UINT32      ui4_end0;
    UINT32      ui4_end1;
    UINT32      aui4_data_offset[2];
    UINT32      ui4_code;

    UINT32      ui4_x;
    UINT32      ui4_y;
    UINT32      ui4_right;
    UINT32      ui4_tmp;
    UINT32      ui4_i;

    ui4_x = 0;
    ui4_y = 0;
    ui4_right = pt_sub_info->ui2_width;
    
    ui1_plane = 0;
    b_aligned = TRUE;

    aui4_data_offset[0] = pt_sub_info->aui2_data_offset[0];
    aui4_data_offset[1] = pt_sub_info->aui2_data_offset[1];
    
    ui4_end0 = aui4_data_offset[1];
    ui4_end1 = pt_sub_info->ui4_datasize;

    while((ui1_plane == 0 && aui4_data_offset[0] < ui4_end0) || (ui1_plane == 1 && aui4_data_offset[1] < ui4_end1))
    {
        if((ui4_code = _mp4_vobsub_get_nibble(pt_sub_info->aui1_spu_data, &b_aligned, &aui4_data_offset[ui1_plane])) >= 0x4
        || (ui4_code = (ui4_code << 4) | _mp4_vobsub_get_nibble(pt_sub_info->aui1_spu_data, &b_aligned, &aui4_data_offset[ui1_plane])) >= 0x10
        || (ui4_code = (ui4_code << 4) | _mp4_vobsub_get_nibble(pt_sub_info->aui1_spu_data, &b_aligned, &aui4_data_offset[ui1_plane])) >= 0x40
        || (ui4_code = (ui4_code << 4) | _mp4_vobsub_get_nibble(pt_sub_info->aui1_spu_data, &b_aligned, &aui4_data_offset[ui1_plane])) >= 0x100)
        {
            for (ui4_i=0; ui4_i<(ui4_code >> 2); ui4_i++)
            {
                if (((ui4_right*ui4_y+ui4_x+ui4_i)/2)<=(*pui4_raw_size))
                {
                    _set_nibble(pui1_raw_data, (INT32)(ui4_right*ui4_y+ui4_x+ui4_i), (UINT8)(ui4_code & 3));
                }
            }
            
            if((ui4_x += (ui4_code >> 2)) < ui4_right)
            {
                continue;
            }
            else
            {
                ui4_x = ui4_right;
            }
        }

        ui4_tmp = ui4_right - ui4_x;
        for (ui4_i=0; ui4_i<ui4_tmp; ui4_i++, ui4_x++)
        {
            if (((ui4_right*ui4_y+ui4_x)/2)<(*pui4_raw_size))
            {
                _set_nibble(pui1_raw_data, (INT32)(ui4_right*ui4_y+ui4_x), (UINT8)(ui4_code & 3));
            }
        }
        
        if(!b_aligned)
        {
            /* align to byte */
            _mp4_vobsub_get_nibble(pt_sub_info->aui1_spu_data, &b_aligned, &aui4_data_offset[ui1_plane]);
        }

        ui4_x = 0;
        ui4_y++;
        ui1_plane = (UINT8)(1 - ui1_plane);
    }

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT8* _mp4_vobsubsub_rle2raw_alloc(SBTL_MP4_VOBSUB_SUB_INFO_T* pt_sub_info,
                                       UINT32*                 pui4_raw_size)
{
    UINT8*  aui1_decoded;
    
    *pui4_raw_size = (UINT32)((pt_sub_info->ui2_width*pt_sub_info->ui2_height)/2);

    if((pt_sub_info->ui2_width*pt_sub_info->ui2_height)%2 == 1) 
    {
        (*pui4_raw_size)++;
    }

    if(*pui4_raw_size > 0)
    {
        /* decode buffer */
        aui1_decoded = (UINT8*)x_mem_alloc(*pui4_raw_size);

        /* ensure the data was allocated */
        if(aui1_decoded == NULL) 
        {
            return NULL;
        }

        x_memset(aui1_decoded, 0, *pui4_raw_size);

        if (_mp4_vobsub_rle2raw(pt_sub_info, pt_sub_info->i4_rle_size, pui4_raw_size, aui1_decoded)==SUBTITLEPARSER_SUCCESS) 
        {
            return aui1_decoded;
        }

        x_mem_free(aui1_decoded);
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static BOOL _mp4_vobsub_parse_sub(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                  PTS_T                    t_pts_current,
                                  PTS_T*                   pt_pts_start,
                                  PTS_T*                   pt_pts_end)
{
    BOOL                                    b_found;
    UINT32                                  ui4_data_1;
    UINT32                                  ui4_data_2;
    UINT32                                  ui4_data_3;
    UINT32                                  ui4_data_4;
    UINT32                                  ui4_pts;
    UINT32                                  ui4_pts_1;
    UINT32                                  ui4_duration;
    INT32                                   i4_ret;
    UINT32                                  ui4_packetsize;
    UINT32                                  ui4_datasize;
    UINT32                                  ui4_raw_size;
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*    pt_private;
        
    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    while (1)
    {
        b_found = sbtl_dmx_data_que_retrieve(pt_mngr->pt_engine,
                                             &ui4_data_1,
                                             &ui4_data_2,
                                             &ui4_pts,
                                             &ui4_duration,
                                             &pt_private->t_append_stc);
        if (!b_found)
        {
            return FALSE;
        }

        b_found = sbtl_dmx_data_que_query(pt_mngr->pt_engine,
                                          &ui4_data_3,
                                          &ui4_data_4,
                                          &ui4_pts_1,
                                          &ui4_duration);
        if (!b_found || ui4_pts_1 > t_pts_current)
        {
            if (ui4_data_2 > pt_private->ui4_pes_buf_size)
            {
                if (pt_private->aui1_pes_buf)
                {
                    x_mem_free(pt_private->aui1_pes_buf);
                    pt_private->aui1_pes_buf = NULL;
                }
                pt_private->ui4_pes_buf_size = 0;
                pt_private->aui1_pes_buf = x_mem_alloc(ui4_data_2);
                
                ASSERT(pt_private->aui1_pes_buf, ("pt_private->aui1_pes_buf=NULL\n"));
                if (NULL == pt_private->aui1_pes_buf)
                {
                    return FALSE;
                }
                pt_private->ui4_pes_buf_size = ui4_data_2;
            }
             
            i4_ret = sbtl_dmx_retrive_data(pt_mngr->pt_engine,
                                                   ui4_data_1,
                                                   ui4_data_2,
                                                   pt_private->aui1_pes_buf);
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("DEMUX_GET_TYPE_READ_BUFFER fail\n"));
                return FALSE;
            }

             i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2);
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
                return FALSE;
            } 
            break;
        }
        else
        {
            i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2);
            if (MMSBTLR_OK != i4_ret)
            {
                ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
                return FALSE;
            } 
            continue;
        }    
    }

    ui4_packetsize = (pt_private->aui1_pes_buf[0] << 8) + pt_private->aui1_pes_buf[1];
    ui4_datasize = (pt_private->aui1_pes_buf[2] << 8) + pt_private->aui1_pes_buf[3];
    
    if(ui4_datasize >= ui4_packetsize || ui4_packetsize > 53220)
    {
        return FALSE;
    }
    
    _mp4_vobsub_parse_dcsqt(pt_mngr, ui4_packetsize, ui4_datasize);

    if (pt_private->t_sub_info.ui2_width == 0 || pt_private->t_sub_info.ui2_height == 0)
    {
        return FALSE;
    }

    if (!pt_private->t_sub_info.b_pallete
    ||  !pt_private->t_sub_info.b_data_offset
    ||  !pt_private->t_sub_info.b_display_area)
    {
        return FALSE;
    }

    *pt_pts_start = pt_private->t_sub_info.t_start_pts_offset + ui4_pts;
    *pt_pts_end = pt_private->t_sub_info.t_end_pts_offset + ui4_pts;
    
    if (pt_private->t_sub_info.b_end_pts_offset)
    {
        
    }
    else
    {
        *pt_pts_end = pt_private->t_pts + 2*PTS_FREQ;
    }

    if (*pt_pts_start > *pt_pts_end)
    {
        *pt_pts_start = 0;
        *pt_pts_end = 0;
        return FALSE;
    }
    
    if (t_pts_current >= *pt_pts_end)
    {
        *pt_pts_start = 0;
        *pt_pts_end = 0;
        return FALSE;
    }

    pt_private->t_sub_info.aui1_decoded = _mp4_vobsubsub_rle2raw_alloc(&pt_private->t_sub_info, &ui4_raw_size);
    pt_private->ui4_pes_buf_size = 0;
    pt_private->t_pts = 0;
    if (pt_private->t_sub_info.aui1_decoded)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mp4_vobsub_seek_time_stamp(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                         PTS_T*                     pt_pts_start,
                                         PTS_T*                     pt_pts_end)
{
    BOOL                                b_select;
    INT32                               i4_ret;
    PTS_T                               t_pts_current;

    i4_ret = sbtl_stc_pts(pt_mngr->pt_engine, &t_pts_current);
            
    if (i4_ret != MMSBTLR_OK)
    {
        return i4_ret;
    }

    /*Get candidate BLOCK*/
    b_select = _mp4_vobsub_parse_sub(pt_mngr,
                                     t_pts_current,
                                     pt_pts_start,
                                     pt_pts_end);

    if (b_select)
    {
        return MMSBTLR_OK;
    }
    else
    {
        return MMSBTLR_NOT_FOUND;
    }
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT32 _sbtl_timer_period(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    ASSERT(pt_mngr->pt_engine->t_cfg.i4_speed>0, ("pt_mngr->pt_engine.t_cfg.i4_speed=%d\n", pt_mngr->pt_engine->t_cfg.i4_speed));
    if (pt_mngr->pt_engine->t_cfg.i4_speed <= 0)
    {
        return 500;
    }
    
    return (UINT32)((TIMER_PERIOD * 100)/pt_mngr->pt_engine->t_cfg.i4_speed);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_timer_cb_fct(HANDLE_T     pt_tm_handle,
                               VOID*        pv_tag)
{
    SBTL_PAGE_INST_MNGR_T*  pt_mngr;
    MM_SBTL_HDLR_MSG_T      t_msg;

    ASSERT(pv_tag, ("pv_tag=NULL\n"));
    if (NULL==pv_tag)
    {
        return;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;
    mm_sbtl_hdlr_send_timer_msg(t_msg, pt_mngr);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                             i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }

    i4_ret = x_timer_create(&pt_private->h_timer);
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }

    i4_ret = x_timer_start(pt_private->h_timer,
                           _sbtl_timer_period(pt_mngr),
                           X_TIMER_FLAG_REPEAT,
                           _sbtl_timer_cb_fct,
                           pt_mngr);
                    
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return;
    }

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (pt_private->h_timer)
    {
        x_timer_delete(pt_private->h_timer);
        pt_private->h_timer = NULL_HANDLE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_lock_pib_fct(HANDLE_T   h_page_inst,
                                     VOID*      pv_tag)
{
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_unlock_pib_fct(HANDLE_T     h_page_inst,
                                         VOID*        pv_tag)
{
    SBTL_PAGE_INST_MNGR_T*    pt_mngr;
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pv_tag, ("_sbtl_mp4_vobsub_unlock_pib_fct pv_tag=NULL\n"));
    if (NULL==pv_tag)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_mp4_vobsub_unlock_pib_fct pt_private=NULL\n"));

    if (NULL==pt_private)
    {
        return MMSBTLR_NOT_FOUND;
    }

    x_sema_unlock(pt_private->h_sem);

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_get_pib_rgn_fct(HANDLE_T                  h_page_inst,
                                          VOID*                     pv_tag,
                                          UINT16                    ui2_rgn_idx,
                                          SBTL_WGL_SBTL_REGION_T*   pt_rgn)
{
    SBTL_PAGE_INST_MNGR_T*    pt_mngr;
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pv_tag, ("_sbtl_mp4_vobsub_get_pib_rgn_fct pv_tag=NULL\n"));
    ASSERT(pt_rgn, ("_sbtl_mp4_vobsub_get_pib_rgn_fct pt_rgn=NULL\n"));
    if ((NULL==pv_tag) || (NULL==pt_rgn))
    {
        return MMSBTLR_INV_ARG;
    }

    if (ui2_rgn_idx>0)
    {
        return MMSBTLR_NOT_FOUND;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_mp4_vobsub_get_pib_rgn_fct pt_private=NULL\n"));

    if (NULL==pt_private)
    {
        return MMSBTLR_NOT_FOUND;
    }
    
    *pt_rgn = pt_private->t_rgn;
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _sbtl_mp4_vobsub_remove(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_private->b_page_exist)
    {
        return;
    }
    
    if (pt_private->e_state == MP4_VOBSUB_STATE_DISPLAY)
    {
        i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                 SBTL_WGL_CMD_SBTL_CLEAR_ALL,
                                 (VOID*)&pt_private->t_pts_s,
                                 NULL);

        if (MMSBTLR_OK != i4_ret)
        {
            ASSERT(0, ("clear widget fail!!/n"));
        }
        #if 0
        i4_ret = x_sema_lock(pt_private->h_sem, X_SEMA_OPTION_WAIT);

        if (i4_ret != OSR_OK)
        {
            ASSERT(0, ("x_sema_lock fail %d\r\n", i4_ret));
        }
        #endif
    }

    pt_private->e_state = MP4_VOBSUB_STATE_NONE;
    pt_private->b_page_exist = FALSE;
    
    x_mem_free(pt_private->t_rgn.pv_bitmap);
    x_memset(&pt_private->t_rgn, 0, sizeof(SBTL_WGL_SBTL_REGION_T));
    
    return;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    INT32                               i4_ret;
    GL_RECT_T                           t_sbtl_rc_scrn;
    SBTL_WGL_SBTL_INIT_T                t_sbtl_init;
    UINT32                              ui4_canvas_w = 0;
    UINT32                              ui4_canvas_h = 0;
    HANDLE_T                            h_canvas;
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*    pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if (pt_private->h_sbtl_wgl)
        return MMSBTLR_EXIST;
    
    /* Get canvas */
    h_canvas = pt_mngr->pt_engine->t_cfg.h_gl_plane;
    
    
    i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);
    if (MMSBTLR_OK != i4_ret)
    {
        return (MMSBTLR_WGL_FAIL);
    }


    t_sbtl_rc_scrn.i4_left = 0;
    t_sbtl_rc_scrn.i4_top = 0;
    t_sbtl_rc_scrn.i4_bottom = (INT32)ui4_canvas_h;
    t_sbtl_rc_scrn.i4_right = (INT32)ui4_canvas_w;


    pt_private->ui4_logical_w = (UINT32)pt_mngr->pt_engine->t_cfg.t_subtitle_mp4.i4_width;
    pt_private->ui4_logical_h = (UINT32)pt_mngr->pt_engine->t_cfg.t_subtitle_mp4.i4_height;
    
    t_sbtl_init.ui4_page_width       = pt_private->ui4_logical_w;
    t_sbtl_init.ui4_page_height      = pt_private->ui4_logical_h;

    t_sbtl_init.pf_lock_pib          = _sbtl_mp4_vobsub_lock_pib_fct;
    t_sbtl_init.pv_tag_lock_pib      = (VOID*) pt_mngr;
    
    t_sbtl_init.pf_unlock_pib        = _sbtl_mp4_vobsub_unlock_pib_fct;
    t_sbtl_init.pv_tag_unlock_pib    = (VOID*) pt_mngr;
    
    t_sbtl_init.pf_get_page_inst_rgn = _sbtl_mp4_vobsub_get_pib_rgn_fct;
    t_sbtl_init.pv_tag_get_rgn       = (VOID*) pt_mngr;
    
    i4_ret = sbtl_wgl_create_widget(h_canvas,
                                    SBTL_WGL_WIDGET_SBTL,
                                    SBTL_WGL_BORDER_NULL,
                                    &t_sbtl_rc_scrn,
                                    255,
                                    &t_sbtl_init,
                                    NULL,
                                    &(pt_private->h_sbtl_wgl));
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = sbtl_wgl_show(pt_private->h_sbtl_wgl, TRUE);
    
    if (MMSBTLR_OK != i4_ret)
    {
       return (MMSBTLR_WGL_FAIL);
    }

    i4_ret = x_sema_create(&pt_private->h_sem, 
                           X_SEMA_TYPE_BINARY, 
                           X_SEMA_STATE_LOCK);
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }

    _sbtl_timer_start(pt_mngr);

    pt_mngr->b_started = TRUE;
    
    _sbtl_mp4_vobsub_new_page(pt_mngr);
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_stop(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    _sbtl_timer_stop(pt_mngr);

    _sbtl_mp4_vobsub_remove(pt_mngr);
    
    /* The widget had already been closed */
    if (pt_private->h_sbtl_wgl)
    {
        sbtl_wgl_destroy_widget(pt_private->h_sbtl_wgl);
        pt_private->h_sbtl_wgl = NULL_HANDLE;
    }

    x_sema_delete(pt_private->h_sem);
    pt_private->h_sem = NULL_HANDLE;

    pt_mngr->b_started = FALSE;
    pt_private->e_state = MP4_VOBSUB_STATE_NONE;
    pt_private->b_page_exist = FALSE;
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_need_dmx(SBTL_PAGE_INST_MNGR_T*   pt_mngr, 
                                   BOOL*                    pb_need)
{
    *pb_need = TRUE;
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_handle_timing(SBTL_PAGE_INST_MNGR_T*  pt_mngr,
                                        PTS_T                   t_pts)
{
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*    pt_private;
    INT32                               i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if (pt_private->b_page_exist)
    {
        if (t_pts<pt_private->t_pts_s)
        {
            pt_private->e_state = MP4_VOBSUB_STATE_DATA_RDY;
            return MMSBTLR_OK;
        }
        else if (t_pts >= pt_private->t_pts_s && t_pts <= pt_private->t_pts_e)
        {
            if (pt_private->e_state != MP4_VOBSUB_STATE_DISPLAY)
            {
                pt_private->e_state = MP4_VOBSUB_STATE_DISPLAY;
                i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                      SBTL_WGL_CMD_SBTL_FLIP,
                                      (VOID*)pt_mngr,
                                      (VOID*)&pt_private->t_pts_s);
            
                if (i4_ret != MMSBTLR_OK)
                {
                    ASSERT(0, ("wgl flip error!!\n"));
                }
            }
            return MMSBTLR_OK;
        }
        else
        {
            _sbtl_mp4_vobsub_remove(pt_mngr);
            _sbtl_mp4_vobsub_new_page(pt_mngr);
        }
    }
    else
    {
        _sbtl_mp4_vobsub_new_page(pt_mngr);
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_config(SBTL_PAGE_INST_MNGR_T*     pt_mngr,
                                            VOID*                      pv_get_info,
                                 UINT32                     ui4_set_type)
{
    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return MMSBTLR_INV_ARG;
    }

    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_SPEED:
        _sbtl_timer_start(pt_mngr);
        break;

    default:
        break;
    }

    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _sbtl_mp4_vobsub_new_page(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    INT32                               i4_ret;
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*    pt_private;
    PTS_T                               t_pts_start;
    PTS_T                               t_pts_end;
    PTS_T                               t_pts_current;


    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    ASSERT(pt_mngr->pt_engine, ("pt_mngr->pt_engine=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private) || (NULL==pt_mngr->pt_engine))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if (!pt_mngr->b_started)
    {
        return MMSBTLR_OK;
    }
    
    if (pt_private->b_page_exist)
    {
        return MMSBTLR_OK;
    }
    
    pt_private->e_state = MP4_VOBSUB_STATE_NONE;
    
    do
    {

        i4_ret = sbtl_stc_pts(pt_mngr->pt_engine, &t_pts_current);

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("Get STC Error!!\n"));
        }

        i4_ret = _mp4_vobsub_seek_time_stamp(pt_mngr,
                                             &t_pts_start,
                                             &t_pts_end);

        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }
            
        pt_private->t_pts_s = t_pts_start;
        pt_private->t_pts_e = t_pts_end;

        pt_private->at_clut_sub[0].a = 0;
        pt_private->at_clut_sub[1].a = 255;
        pt_private->at_clut_sub[2].a = 255;
        pt_private->at_clut_sub[3].a = 255;

        
        pt_private->t_rgn.at_clut = pt_private->at_clut_sub;
        
        pt_private->t_rgn.b_dirty = TRUE;
        pt_private->t_rgn.e_clr_mode = SBTL_WGL_SBTL_COLOR_MODE_CLUT_4_BIT;
        
        pt_private->t_rgn.i4_top = pt_private->t_sub_info.i4_top;
        pt_private->t_rgn.i4_left = pt_private->t_sub_info.i4_left;
        pt_private->t_rgn.ui4_width = pt_private->t_sub_info.ui2_width;
        pt_private->t_rgn.ui4_height = pt_private->t_sub_info.ui2_height;
        
        pt_private->t_rgn.t_rc_dirty.i4_bottom = (INT32)(pt_private->t_rgn.ui4_height);
        pt_private->t_rgn.t_rc_dirty.i4_top = 0;
        pt_private->t_rgn.t_rc_dirty.i4_left = 0;
        pt_private->t_rgn.t_rc_dirty.i4_right = (INT32)(pt_private->t_rgn.ui4_width);
        
        pt_private->t_rgn.pv_bitmap = pt_private->t_sub_info.aui1_decoded;
        
        if (t_pts_current < t_pts_start)
        {
            pt_private->e_state = MP4_VOBSUB_STATE_DATA_RDY;
            pt_private->b_page_exist = TRUE;
            
            return MMSBTLR_OK;
        }
        else if (t_pts_current >= t_pts_start && t_pts_current <= t_pts_end)
        {
            pt_private->e_state = MP4_VOBSUB_STATE_DISPLAY;
            /*display*/
            
            i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                     SBTL_WGL_CMD_SBTL_FLIP,
                                     (VOID*)pt_mngr,
                                     (VOID*)&pt_private->t_pts_s);

            if (i4_ret != MMSBTLR_OK)
            {
                ASSERT(0, ("wgl flip error!!\n"));
            }
            pt_private->b_page_exist = TRUE;
            return MMSBTLR_OK;
        }
        else
        {
            if (pt_private->t_rgn.pv_bitmap)
            {
                x_mem_free(pt_private->t_rgn.pv_bitmap);
                x_memset(&pt_private->t_rgn, 0, sizeof(SBTL_WGL_SBTL_REGION_T));
            }            
            continue;
        }
        
    }while(1);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static SBTL_PAGE_INST_MNGR_T* _mp4_vobsub_page_mngr_constructor (SBTL_ENGINE_T*   pt_engine)
{
    SBTL_PAGE_INST_MNGR_T*                  pt_mngr;
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*    pt_private;
    UINT8                                   ui1_i = 0;

    ASSERT(pt_engine, ("_mp4_vobsub_page_mngr_constructor pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return NULL;
    }

    do
    {
        pt_mngr = (SBTL_PAGE_INST_MNGR_T*)x_mem_alloc(sizeof(SBTL_PAGE_INST_MNGR_T));
        if (NULL == pt_mngr)
        {
            break;
        }
        x_memset(pt_mngr, 0, sizeof(SBTL_PAGE_INST_MNGR_T));

        pt_mngr->pf_start = _sbtl_mp4_vobsub_start;
        pt_mngr->pf_stop = _sbtl_mp4_vobsub_stop;
        pt_mngr->pf_cfg = _sbtl_mp4_vobsub_config;
        pt_mngr->pf_handle_timing = _sbtl_mp4_vobsub_handle_timing;
        pt_mngr->pf_need_dmx = _sbtl_mp4_vobsub_need_dmx;
        pt_mngr->pf_new_page = _sbtl_mp4_vobsub_new_page;
        
        pt_mngr->pt_engine = pt_engine;
        
        pt_mngr->pv_private = x_mem_alloc(sizeof(SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T));

        if (NULL == pt_mngr->pv_private)
        {
            break;
        }

        pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
        x_memset(pt_mngr->pv_private, 0, sizeof(SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T));

        // set default frame size
        pt_private->ui4_frame_width = 720;
        pt_private->ui4_frame_height = 480;

        // set default palette
        pt_private->at_clut_idx[0].a = 0;
        pt_private->at_clut_idx[0].u1.r = 0x00;
        pt_private->at_clut_idx[0].u2.g = 0x00;
        pt_private->at_clut_idx[0].u3.b = 0x00;

        pt_private->at_clut_idx[1].a = 255;
        pt_private->at_clut_idx[1].u1.r = 0xff;
        pt_private->at_clut_idx[1].u2.g = 0xff;
        pt_private->at_clut_idx[1].u3.b = 0xff;


        pt_private->at_clut_idx[2].a = 255;
        pt_private->at_clut_idx[2].u1.r = 0x00;
        pt_private->at_clut_idx[2].u2.g = 0x00;
        pt_private->at_clut_idx[2].u3.b = 0x00;

        pt_private->at_clut_idx[3].a = 255;
        pt_private->at_clut_idx[3].u1.r = 0xff;
        pt_private->at_clut_idx[3].u2.g = 0xff;
        pt_private->at_clut_idx[3].u3.b = 0xff;

        for(ui1_i = 4; ui1_i<16; ui1_i++)
        {
            pt_private->at_clut_idx[ui1_i].a = 255;
            pt_private->at_clut_idx[ui1_i].u1.r = 0xff;
            pt_private->at_clut_idx[ui1_i].u2.g = 0xff;
            pt_private->at_clut_idx[ui1_i].u3.b = 0xff;
        }

        return pt_mngr;
    }while(0);

    if (pt_mngr)
    {
        if (pt_mngr->pv_private)
        {
            x_mem_free(pt_mngr->pv_private);
        }
        x_mem_free(pt_mngr);
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _mp4_vobsub_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("_mp4_vobsub_page_mngr_destructor pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return;
    }
    
    pt_private = (SBTL_MP4_VOBSUB_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_mp4_vobsub_page_mngr_destructor pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return;
    }
    
    if (pt_private->aui1_pes_buf)
    {
        x_mem_free(pt_private->aui1_pes_buf);
    }
   
    x_mem_free(pt_private);
    pt_mngr->pt_engine->pt_page_mngr = NULL;
    x_mem_free(pt_mngr);
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID x_sbtl_mp4_vobsub_init(VOID)
{
    sbtl_register(MM_SBTL_TYPE_MP4_VOBSUB, 
                  &t_mp4_vobsub_fnct_tbl);
}

