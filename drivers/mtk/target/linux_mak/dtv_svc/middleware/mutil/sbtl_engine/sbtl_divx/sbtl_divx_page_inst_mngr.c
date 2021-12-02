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
 * $RCSfile: sbtl_divx_page_inst_mngr.c,v $
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
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"

#include "mutil/mm_util.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_widget.h"

#include "mutil/sbtl_engine/sbtl_divx/sbtl_divx_page_inst_mngr.h"

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

#include "sbtl_divx_page_inst_mngr.h"

#endif /*__KERNEL__*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PTS_FREQ        ((UINT64)(90000))
#define TIMER_PERIOD    100
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static UINT8 _get_nibble(UINT8*     aui1_buffer, 
                         INT32      i4_offs);

static VOID _set_nibble(UINT8*      aui1_buffer, 
                        INT32       i4_offs, 
                        UINT8       ui1_n);

static UINT8 _read_uint8(UINT8*     aui1_buffer, 
                         UINT32     ui4_offs);

static UINT16 _read_uint16(UINT8*       aui1_buffer, 
                           UINT32       ui4_offs);

static INT32 _sub_parser_rle2raw(DIVX_SUBPICT_HDR_T*    pt_subpict_hdr,
                                 INT32                  i4_rle_size,
                                 INT32*                 pi4_raw_size, 
                                 UINT8*                 pui1_raw_data);

static VOID _sub_parser_subpacket2rle(SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private,
                                      UINT8*                          aui1_raw_subpacket,
                                      DIVX_SUBPICT_HDR_T*             pt_subpict_hdr);

static DIVX_SUBPICT_HDR_T* _sub_parser_subpacket2rle_alloc(SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private,
                                                           UINT8* aui1_raw_subpacket);



static UINT8* _sub_parser_rle2raw_alloc(DIVX_SUBPICT_HDR_T*     pt_subpict_hdr, 
                                        INT32                   i4_rle_size,
                                        INT32*                  pi4_raw_size);


static INT32 _sbtl_divx_in_range(DIVX_SUBPICT_HDR_T*    pt_subpict_hdr,
                                 PTS_T*                 pt_pts_start,
                                 PTS_T*                 pt_pts_end);

static VOID _sbtl_divx_remove(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

static void _page_divx_scale(SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                             GL_RECT_T*                 pt_sbtl_rc_scrn, 
                             UINT32                     ui4_canvas_w, 
                             UINT32                     ui4_canvas_h);

static void _page_divx_overscan_scale(SBTL_PAGE_INST_MNGR_T*    pt_mngr);


INT32 _sbtl_divx_start(SBTL_PAGE_INST_MNGR_T*   pt_mngr);

INT32 _sbtl_divx_stop(SBTL_PAGE_INST_MNGR_T*    pt_mngr);

static INT32 _sbtl_divx_need_dmx(SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                                 BOOL*                      pb_need);

static INT32 _sbtl_divx_handle_timing(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                                      PTS_T                     t_pts);

static INT32 _sbtl_divx_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                  VOID*                    pv_get_info,
                               UINT32                   ui4_set_type);


static INT32 _sbtl_divx_new_page(SBTL_PAGE_INST_MNGR_T*  pt_mngr);


static SBTL_PAGE_INST_MNGR_T* _divx_page_mngr_constructor (SBTL_ENGINE_T*   pt_engine);

static VOID _divx_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*  pt_mngr);

static INT32 _sbtl_timer_start(SBTL_PAGE_INST_MNGR_T*  pt_mngr);
static VOID _sbtl_timer_stop(SBTL_PAGE_INST_MNGR_T*  pt_mngr);

/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
static SBTL_MNGR_FCT_TBL_T t_divx_fnct_tbl = 
{
    FALSE,
    _divx_page_mngr_constructor,
    _divx_page_mngr_destructor,
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
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

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
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private))
    {
        return;
    }

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

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


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: set a nibble inside aui1_buffer
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
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
 * Description: read a single uint8_t from aui1_buffer
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT8 _read_uint8(UINT8*     aui1_buffer, 
                         UINT32     ui4_offs)
{
    UINT8* pui1_uint8 = (UINT8*)((UINT32)aui1_buffer + ui4_offs);

    return (*pui1_uint8);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: read a single uint16_t from aui1_buffer
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT16 _read_uint16(UINT8*       aui1_buffer, 
                           UINT32       ui4_offs)
{
    UINT16 ui2_val16;

    ui2_val16= *(aui1_buffer + ui4_offs) + (*(aui1_buffer + ui4_offs + 1) << 8);

    return ui2_val16;
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
static INT32 _sub_parser_rle2raw(DIVX_SUBPICT_HDR_T*    pt_subpict_hdr,
                                 INT32                  i4_rle_size,
                                 INT32*                 pi4_raw_size, 
                                 UINT8*                 pui1_raw_data)
{
    INT32 i4_width;
    INT32 i4_height;
    INT32 i4_field_offset;
    INT32 i4_calcSize;

    /* encoded top&bottom fields */
    UINT8*  aui1_encoded[2];

    /* write&read cursors for top&bottom fields */
    INT32 ai4_limit[2];
    INT32 ai4_w[2];
    INT32 ai4_r[2]={0,0};

    /* read stop offsets */
    INT32 ai4_rstop[2];

    /* cache metrics */
    i4_width = (INT32)pt_subpict_hdr->ui2_width;
    i4_height = (INT32)pt_subpict_hdr->ui2_height;
    i4_field_offset = (INT32)pt_subpict_hdr->ui2_field_offset;

    i4_calcSize = (i4_width*i4_height)/2;

    if ((i4_width*i4_height)%2 == 1)
    {
        i4_calcSize++;
    }

    /* Test that size of memory was sufficient */
    if (*pi4_raw_size < i4_calcSize)
    {
        return SUBTITLEPARSER_BUFFER_TOO_SMALL;
    }
    
    /* set to actual size of data */
    *pi4_raw_size = i4_calcSize;

    /* read stop offsets */
    ai4_rstop[0] = i4_field_offset*2;
    ai4_rstop[1] = (i4_rle_size - i4_field_offset)*2;


    /* encode field (top) */
    aui1_encoded[0] = (UINT8*)((UINT32)pt_subpict_hdr->pui1_rle_data);

    /* encode field (bottom) */
    aui1_encoded[1] = (UINT8*)((UINT32)pt_subpict_hdr->pui1_rle_data + i4_field_offset);

    /* write cursors */
    ai4_limit[1] = i4_width*(i4_height-0);
    ai4_limit[0] = i4_width*(i4_height-1);
    ai4_w[1] = i4_width;
    ai4_w[0] = 0;

    /* continuously decode lines until height is reached */
    while (ai4_w[0] < ai4_limit[0] || ai4_w[1] < ai4_limit[1])
    {
        INT32 i4_v;

        /* iterate through both top and bottom fields */
        for (i4_v=0;i4_v<2;i4_v++)
        {
            INT32 i4_col;
            INT32 i4_len;
            INT32 i4_i;

            if(ai4_w[i4_v] >= ai4_limit[i4_v])
                continue;

            if(ai4_r[i4_v] < ai4_rstop[i4_v])
            {
                /* grab next input nibble */
                INT32 i4_rle = _get_nibble(aui1_encoded[i4_v], ai4_r[i4_v]++);

                if(i4_rle < 0x04)
                {
                    i4_rle = (i4_rle << 4) | _get_nibble(aui1_encoded[i4_v], ai4_r[i4_v]++);

                    if(i4_rle < 0x10)
                    {
                        i4_rle = (i4_rle << 4) | _get_nibble(aui1_encoded[i4_v], ai4_r[i4_v]++);

                        if(i4_rle < 0x040)
                        {
                            i4_rle = (i4_rle << 4) | _get_nibble(aui1_encoded[i4_v], ai4_r[i4_v]++);

                            if(i4_rle < 0x0004)
                            {
                                i4_rle |= (i4_width - ai4_w[i4_v]%i4_width) << 2;
                            }
                        }
                    }
                }

                i4_col = i4_rle & 0x03;
                i4_len = i4_rle >> 2;

                if(i4_len > (i4_width - ai4_w[i4_v]%i4_width) || i4_len == 0)
                {
                    i4_len = i4_width - ai4_w[i4_v]%i4_width;
                }
            }
            else
            {
                i4_col = 0;
                i4_len = i4_width - ai4_w[i4_v]%i4_width;
            }

            for(i4_i=0;i4_i<i4_len;i4_i++)
            {
                _set_nibble(pui1_raw_data, ai4_w[i4_v]++, (UINT8)i4_col);
            }

            if(ai4_w[i4_v]%i4_width == 0)
            {
                ai4_w[i4_v] += i4_width;

                if(ai4_r[i4_v]%2 == 1)
                {
                    ai4_r[i4_v]++;
                }
            }
        }
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
static VOID _sub_parser_subpacket2rle(SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private,
                                      UINT8*                          aui1_raw_subpacket,
                                      DIVX_SUBPICT_HDR_T*             pt_subpict_hdr)
{
    if((pt_private->b_hd && pt_private->ui4_pes_buf_size < 57) || ((!pt_private->b_hd) && pt_private->ui4_pes_buf_size < 53))
    {
        return;
    }

    x_memcpy(pt_subpict_hdr->aui1_duration, aui1_raw_subpacket, 27);

    pt_subpict_hdr->ui2_width = _read_uint16(aui1_raw_subpacket, 0x1B);
    pt_subpict_hdr->ui2_height = _read_uint16(aui1_raw_subpacket, 0x1D);
    pt_subpict_hdr->ui2_left = _read_uint16(aui1_raw_subpacket, 0x1F);
    pt_subpict_hdr->ui2_top = _read_uint16(aui1_raw_subpacket, 0x21);
    pt_subpict_hdr->ui2_right = _read_uint16(aui1_raw_subpacket, 0x23);
    pt_subpict_hdr->ui2_bottom = _read_uint16(aui1_raw_subpacket, 0x25);
    pt_subpict_hdr->ui2_field_offset = _read_uint16(aui1_raw_subpacket, 0x27);

    pt_subpict_hdr->t_background.ui1_red = _read_uint8(aui1_raw_subpacket, 0x29);
    pt_subpict_hdr->t_background.ui1_green = _read_uint8(aui1_raw_subpacket, 0x2A);
    pt_subpict_hdr->t_background.ui1_blue = _read_uint8(aui1_raw_subpacket, 0x2B);

    pt_subpict_hdr->t_pattern.ui1_red = _read_uint8(aui1_raw_subpacket, 0x2C);
    pt_subpict_hdr->t_pattern.ui1_green = _read_uint8(aui1_raw_subpacket, 0x2D);
    pt_subpict_hdr->t_pattern.ui1_blue = _read_uint8(aui1_raw_subpacket, 0x2E);

    pt_subpict_hdr->t_emphasis1.ui1_red = _read_uint8(aui1_raw_subpacket, 0x2F);
    pt_subpict_hdr->t_emphasis1.ui1_green = _read_uint8(aui1_raw_subpacket, 0x30);
    pt_subpict_hdr->t_emphasis1.ui1_blue = _read_uint8(aui1_raw_subpacket, 0x31);

    pt_subpict_hdr->t_emphasis2.ui1_red = _read_uint8(aui1_raw_subpacket, 0x32);
    pt_subpict_hdr->t_emphasis2.ui1_green = _read_uint8(aui1_raw_subpacket, 0x33);
    pt_subpict_hdr->t_emphasis2.ui1_blue = _read_uint8(aui1_raw_subpacket, 0x34);

    pt_subpict_hdr->pui1_rle_data = aui1_raw_subpacket + SUBPICT_PACKHDR_SIZE;

    if (pt_private->b_hd)
    {
        pt_private->t_subalpha_hdr.ui1_background_transparency = _read_uint8(aui1_raw_subpacket, 0x35);
        pt_private->t_subalpha_hdr.ui1_pattern_transparency = _read_uint8(aui1_raw_subpacket, 0x36);
        pt_private->t_subalpha_hdr.ui1_emphasis1_transparency = _read_uint8(aui1_raw_subpacket, 0x37);
        pt_private->t_subalpha_hdr.ui1_emphasis2_transparency = _read_uint8(aui1_raw_subpacket, 0x38);
        pt_subpict_hdr->pui1_rle_data = aui1_raw_subpacket + SUBPICT_PACKHDR_SIZE + 4;
    }
    else
    {
        pt_private->t_subalpha_hdr.ui1_background_transparency = 0;
        pt_private->t_subalpha_hdr.ui1_pattern_transparency = 255;
        pt_private->t_subalpha_hdr.ui1_emphasis1_transparency = 255;
        pt_private->t_subalpha_hdr.ui1_emphasis2_transparency = 255;
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
static DIVX_SUBPICT_HDR_T* _sub_parser_subpacket2rle_alloc(SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private,
                                                           UINT8* aui1_raw_subpacket)
{
    DIVX_SUBPICT_HDR_T* pt_subpict_hdr = (DIVX_SUBPICT_HDR_T*)x_mem_alloc(sizeof(DIVX_SUBPICT_HDR_T));
    if(NULL == pt_subpict_hdr)
    {
        return NULL;
    }

    _sub_parser_subpacket2rle(pt_private, aui1_raw_subpacket, pt_subpict_hdr);

    return pt_subpict_hdr;
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
static UINT8* _sub_parser_rle2raw_alloc(DIVX_SUBPICT_HDR_T*     pt_subpict_hdr, 
                                        INT32                   i4_rle_size,
                                        INT32*                  pi4_raw_size)
{
    UINT8*  pui1_decoded = NULL;

    /* ensure valid pointer */
    if(pt_subpict_hdr == NULL)
    {
        return NULL;
    }


    *pi4_raw_size = (pt_subpict_hdr->ui2_width*pt_subpict_hdr->ui2_height)/2;
    
    if((pt_subpict_hdr->ui2_width*pt_subpict_hdr->ui2_height)%2 == 1) 
    {
        (*pi4_raw_size)++;
    }

    if(*pi4_raw_size > 0)
    {
        /* decode buffer */
        pui1_decoded = (UINT8*)x_mem_alloc((SIZE_T)*pi4_raw_size);

        /* ensure the data was allocated */
        if(pui1_decoded == NULL) 
        {
            return NULL;
        }

        x_memset(pui1_decoded, 0, (SIZE_T)*pi4_raw_size);

        if (_sub_parser_rle2raw(pt_subpict_hdr, i4_rle_size, pi4_raw_size, pui1_decoded)==SUBTITLEPARSER_SUCCESS) 
        {
            return pui1_decoded;
        }

        x_mem_free(pui1_decoded);

        return NULL;
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
static INT32 _sbtl_divx_lock_pib_fct(HANDLE_T   h_page_inst,
                                     VOID*      pv_tag)
{
    return (MMSBTLR_OK);
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
static INT32 _sbtl_divx_unlock_pib_fct(HANDLE_T     h_page_inst,
                                       VOID*        pv_tag)
{
    SBTL_PAGE_INST_MNGR_T*    pt_mngr;
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pv_tag, ("_sbtl_divx_unlock_pib_fct pv_tag=NULL\n"));
    if (NULL==pv_tag)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_divx_unlock_pib_fct pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return MMSBTLR_INV_ARG;
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
static INT32 _sbtl_divx_get_pib_rgn_fct(HANDLE_T                    h_page_inst,
                                        VOID*                       pv_tag,
                                        UINT16                      ui2_rgn_idx,
                                        SBTL_WGL_SBTL_REGION_T*     pt_rgn)
{
    SBTL_PAGE_INST_MNGR_T*    pt_mngr;
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pv_tag, ("_sbtl_divx_get_pib_rgn_fct pv_tag=NULL\n"));
    ASSERT(pt_rgn, ("_sbtl_divx_get_pib_rgn_fct pt_rgn=NULL\n"));
    if ((NULL==pv_tag) || (NULL==pt_rgn))
    {
        return MMSBTLR_INV_ARG;
    }
    
    if (ui2_rgn_idx>0)
    {
        return MMSBTLR_NOT_FOUND;
    }

    pt_mngr = (SBTL_PAGE_INST_MNGR_T*)pv_tag;

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_divx_get_pib_rgn_fct pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return MMSBTLR_INV_ARG;
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
static VOID _sbtl_divx_remove(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;
    
    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    if (!pt_private->b_page_exist)
    {
        return;
    }
    
    if (pt_private->e_state == DIVX_STATE_DISPLAY)
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

    pt_private->e_state = DIVX_STATE_NONE;
    pt_private->b_page_exist = FALSE;

    x_mem_free(pt_private->pt_subpict_hdr);
    pt_private->pt_subpict_hdr = NULL;
    
    x_mem_free(pt_private->t_rgn.pv_bitmap);
    pt_private->t_rgn.pv_bitmap = NULL;
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
static void _page_divx_scale(SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                             GL_RECT_T*                 pt_sbtl_rc_scrn, 
                             UINT32                     ui4_canvas_w, 
                             UINT32                     ui4_canvas_h)
{
    INT32                           i4_w;
    INT32                           i4_h;
    INT32                           i4_w_new;
    INT32                           i4_h_new;
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if (MM_DIVX_SBTL_TITLE_TYPE == pt_mngr->pt_engine->t_cfg.e_divx_sbtl_type)
    {
        i4_w = pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_width;
        i4_h = pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_height;

        if ((i4_w <= 720) && (i4_h <= 576))
        {
            i4_w = 640;
            i4_h = 480;
        }
        else if ((i4_w <= 1280) && (i4_h <= 720))
        {
            i4_w = 960;
            i4_h = 720;
        }
        else
        {
            if (pt_private->b_hd)
            {
                i4_w = 1440;
                i4_h = 1080;
            }
            else
            {
                i4_w = 960;
                i4_h = 720;
            }
        }
        
        i4_w_new = (i4_w*(INT32)ui4_canvas_h)/i4_h;
        i4_h_new = (INT32)ui4_canvas_h;

        pt_sbtl_rc_scrn->i4_bottom = i4_h_new-1;
        pt_sbtl_rc_scrn->i4_top = 0;

        pt_sbtl_rc_scrn->i4_left = ((INT32)ui4_canvas_w - i4_w_new)/2;
        pt_sbtl_rc_scrn->i4_right = (INT32)ui4_canvas_w - pt_sbtl_rc_scrn->i4_left;
    }
    else
    {
        INT32   i4_top;
        INT32   i4_bottom;
        INT32   i4_left;
        INT32   i4_right;
        
        i4_w_new  = pt_mngr->pt_engine->t_cfg.t_rect_display.i4_right
                  - pt_mngr->pt_engine->t_cfg.t_rect_display.i4_left;

        i4_h_new  = pt_mngr->pt_engine->t_cfg.t_rect_display.i4_bottom
                  - pt_mngr->pt_engine->t_cfg.t_rect_display.i4_top;

        i4_top = pt_mngr->pt_engine->t_cfg.t_rect_video.i4_top;
        i4_bottom = pt_mngr->pt_engine->t_cfg.t_rect_video.i4_bottom;
        i4_left = pt_mngr->pt_engine->t_cfg.t_rect_video.i4_left;
        i4_right = pt_mngr->pt_engine->t_cfg.t_rect_video.i4_right;

        pt_sbtl_rc_scrn->i4_top = (i4_top*(INT32)ui4_canvas_h)/i4_h_new;
        pt_sbtl_rc_scrn->i4_bottom = (i4_bottom*(INT32)ui4_canvas_h)/i4_h_new;
        
        pt_sbtl_rc_scrn->i4_left = (i4_left*(INT32)ui4_canvas_w)/i4_w_new;
        pt_sbtl_rc_scrn->i4_right = (i4_right*(INT32)ui4_canvas_w)/i4_w_new;
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
static void _page_divx_overscan_scale(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;

    pt_private->ui4_logical_h = (UINT32)((pt_mngr->pt_engine->t_cfg.ui1_ovs_left 
                               + pt_mngr->pt_engine->t_cfg.ui1_ovs_right)
                               * pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_width
                               / pt_mngr->pt_engine->t_cfg.t_video_resolution.ui4_width);
    
    pt_private->ui4_logical_w = (UINT32)((pt_mngr->pt_engine->t_cfg.ui1_ovs_top
                               + pt_mngr->pt_engine->t_cfg.ui1_ovs_bottom)
                               * pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_height
                               / pt_mngr->pt_engine->t_cfg.t_video_resolution.ui4_height);

    pt_private->ui1_ovs_left = (UINT8)(pt_mngr->pt_engine->t_cfg.ui1_ovs_left
                               * pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_width
                               / pt_mngr->pt_engine->t_cfg.t_video_resolution.ui4_width);

    pt_private->ui1_ovs_right = (UINT8)(pt_mngr->pt_engine->t_cfg.ui1_ovs_right
                               * pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_width
                               / pt_mngr->pt_engine->t_cfg.t_video_resolution.ui4_width);

    pt_private->ui1_ovs_top   = (UINT8)(pt_mngr->pt_engine->t_cfg.ui1_ovs_right
                               * pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_height
                               / pt_mngr->pt_engine->t_cfg.t_video_resolution.ui4_height);

    pt_private->ui1_ovs_bottom = (UINT8)(pt_mngr->pt_engine->t_cfg.ui1_ovs_bottom
                                       * pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_height
                                       / pt_mngr->pt_engine->t_cfg.t_video_resolution.ui4_height);
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
INT32 _sbtl_divx_start(SBTL_PAGE_INST_MNGR_T*   pt_mngr)
{
    INT32                           i4_ret;
    GL_RECT_T                       t_sbtl_rc_scrn;
    SBTL_WGL_SBTL_INIT_T            t_sbtl_init;
    UINT32                          ui4_canvas_w = 0;
    UINT32                          ui4_canvas_h = 0;
    HANDLE_T                        h_canvas;
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("_sbtl_divx_start pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_divx_start pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return MMSBTLR_INV_ARG;
    }
    
    if (pt_private->h_sbtl_wgl)
        return MMSBTLR_EXIST;
    
    /* Get canvas */
    h_canvas = pt_mngr->pt_engine->t_cfg.h_gl_plane;
    
    
    i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);
    if (MMSBTLR_OK != i4_ret)
    {
        return (MMSBTLR_WGL_FAIL);
    }
    
    _page_divx_scale(pt_mngr, 
                     &t_sbtl_rc_scrn, 
                     ui4_canvas_w, 
                     ui4_canvas_h);

    if (MM_DIVX_SBTL_TITLE_TYPE == pt_mngr->pt_engine->t_cfg.e_divx_sbtl_type)
    {
        pt_private->ui4_logical_w = (UINT32)pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_width;
        pt_private->ui4_logical_h = (UINT32)pt_mngr->pt_engine->t_cfg.t_subtitle_avi.i4_height;

        if ((pt_private->ui4_logical_w <= 720) && (pt_private->ui4_logical_h <= 576))
        {
            pt_private->ui4_logical_w = 640;
            pt_private->ui4_logical_h = 480;
        }
        else if ((pt_private->ui4_logical_w <= 1280) && (pt_private->ui4_logical_h <= 720))
        {
            pt_private->ui4_logical_w = 960;
            pt_private->ui4_logical_h = 720;
        }
        else
        {
            if (pt_private->b_hd)
            {
                pt_private->ui4_logical_w = 1440;
                pt_private->ui4_logical_h = 1080;
            }
            else
            {
                pt_private->ui4_logical_w = 960;
                pt_private->ui4_logical_h = 720;
            }
        }
    }
    else
    {
        _page_divx_overscan_scale(pt_mngr);
    }
    
    t_sbtl_init.ui4_page_width       = pt_private->ui4_logical_w;
    t_sbtl_init.ui4_page_height      = pt_private->ui4_logical_h;

    t_sbtl_init.pf_lock_pib          = _sbtl_divx_lock_pib_fct;
    t_sbtl_init.pv_tag_lock_pib      = (VOID*) pt_mngr;
    
    t_sbtl_init.pf_unlock_pib        = _sbtl_divx_unlock_pib_fct;
    t_sbtl_init.pv_tag_unlock_pib    = (VOID*) pt_mngr;
    
    t_sbtl_init.pf_get_page_inst_rgn = _sbtl_divx_get_pib_rgn_fct;
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

    if (MM_DIVX_SBTL_TITLE_TYPE == pt_mngr->pt_engine->t_cfg.e_divx_sbtl_type)
    {
        _sbtl_timer_start(pt_mngr);
    }

    pt_mngr->b_started = TRUE;
    
    _sbtl_divx_new_page(pt_mngr);
    
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
INT32 _sbtl_divx_stop(SBTL_PAGE_INST_MNGR_T*    pt_mngr)
{
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_mngr, ("_sbtl_divx_stop pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_divx_stop pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return MMSBTLR_INV_ARG;
    }

    if (MM_DIVX_SBTL_TITLE_TYPE == pt_mngr->pt_engine->t_cfg.e_divx_sbtl_type)
    {
        _sbtl_timer_stop(pt_mngr);
    }
   
    _sbtl_divx_remove(pt_mngr);
    /* The widget had already been closed */
    if (pt_private->h_sbtl_wgl)
    {
        sbtl_wgl_destroy_widget(pt_private->h_sbtl_wgl);
        pt_private->h_sbtl_wgl = NULL_HANDLE;
    }

    x_sema_delete(pt_private->h_sem);
    pt_private->h_sem = NULL_HANDLE;

    pt_mngr->b_started = FALSE;
    pt_private->e_state = DIVX_STATE_NONE; 
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
static INT32 _sbtl_divx_need_dmx(SBTL_PAGE_INST_MNGR_T*     pt_mngr, 
                                 BOOL*                      pb_need)
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
static INT32 _sbtl_divx_handle_timing(SBTL_PAGE_INST_MNGR_T*    pt_mngr,
                                      PTS_T                     t_pts)
{
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;
    INT32                           i4_ret;

    ASSERT(pt_mngr, ("_sbtl_divx_handle_timing pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_divx_handle_timing pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return MMSBTLR_INV_ARG;
    }

    DBG_INFO((_INFO_HEADER"divx timer %d\n", (UINT32)t_pts));
    if (pt_private->b_page_exist)
    {
        /*_sbtl_divx_in_range(pt_private->pt_subpict_hdr,
                            &t_pts_start,
                            &t_pts_end);

        if (t_pts < t_pts_start)*/
        if (t_pts<pt_private->t_pts_s)
        {
            pt_private->e_state = DIVX_STATE_DATA_RDY;
            return MMSBTLR_OK;
        }
        else if (t_pts >= pt_private->t_pts_s && t_pts <= pt_private->t_pts_e)
        {
            if (pt_private->e_state != DIVX_STATE_DISPLAY)
            {   
                pt_private->e_state = DIVX_STATE_DISPLAY;
                if(pt_mngr->pt_engine->t_cfg.b_hide == FALSE)
                {
                i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                      SBTL_WGL_CMD_SBTL_FLIP,
                                      (VOID*)pt_mngr,
                                      (VOID*)&pt_private->t_pts_s);
            
                if (i4_ret != MMSBTLR_OK)
                {
                    ASSERT(0, ("wgl flip error!!\n"));
                }

                DBG_INFO((_INFO_HEADER"divx flip1 %d %d %d\n",
                         (UINT32)pt_private->t_append_stc, 
                         (UINT32)pt_private->t_pts_s, 
                         (UINT32)(t_pts-pt_private->t_pts_s)));
            }
            }
            return MMSBTLR_OK;
        }
        else
        {
            _sbtl_divx_remove(pt_mngr);
            _sbtl_divx_new_page(pt_mngr);
        }
    }
    else
    {
        _sbtl_divx_new_page(pt_mngr);
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
static INT32 _sbtl_divx_config(SBTL_PAGE_INST_MNGR_T*   pt_mngr,
                                  VOID*                    pv_get_info,
                               UINT32                   ui4_set_type)
{
    UINT32                          ui4_canvas_w = 0;
    UINT32                          ui4_canvas_h = 0;
    INT32                           i4_ret;
    HANDLE_T                        h_canvas;
    GL_RECT_T                       t_sbtl_rc_scrn;
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;
    BOOL                            b_flag;
    PTS_T                           t_pts_current;
    

    ASSERT(pt_mngr, ("_sbtl_divx_config pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_sbtl_divx_config pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return MMSBTLR_INV_ARG;
    }

    switch (ui4_set_type)
    {
    case EVN_MM_SBTL_SET_VIDEO_RECT:
        h_canvas = pt_mngr->pt_engine->t_cfg.h_gl_plane;
        i4_ret = sbtl_wgl_get_canvas_size(h_canvas, &ui4_canvas_w, &ui4_canvas_h);
        if (MMSBTLR_OK != i4_ret)
        {
            return MMSBTLR_WGL_FAIL;
        }

        _page_divx_scale(pt_mngr, 
                         &t_sbtl_rc_scrn, 
                         ui4_canvas_w, 
                         ui4_canvas_h);
        
        sbtl_wgl_move(pt_private->h_sbtl_wgl,
                   &t_sbtl_rc_scrn,
                   TRUE);
        break;

    case EVN_MM_SBTL_SET_DIVX_MENU_ITEM:
        if (pt_private->ui2_menu_item!= pt_mngr->pt_engine->t_cfg.ui2_menu_item)
        {
            _sbtl_divx_remove(pt_mngr);
            pt_private->ui2_menu_item = pt_mngr->pt_engine->t_cfg.ui2_menu_item;
            _sbtl_divx_new_page(pt_mngr);
        }
        break;

    case EVN_MM_SBTL_SET_SPEED:
        _sbtl_timer_start(pt_mngr);
        break;

    case EVN_MM_SBTL_SET_SHOW_HIDE:
        
        b_flag = *(BOOL*)pv_get_info;
        
        i4_ret = sbtl_stc_pts(pt_mngr->pt_engine,
                               &t_pts_current);

        if (i4_ret != MMSBTLR_OK)
        {
            ASSERT(0, ("Get STC Error!!\n"));
            break;
        }
                
        if(b_flag == TRUE)
        {
            pt_mngr->pt_engine->t_cfg.b_hide = FALSE;         
        }
        else
        {
            pt_mngr->pt_engine->t_cfg.b_hide = TRUE;
            if (pt_private->e_state == DIVX_STATE_DISPLAY)
            {        
                i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                  SBTL_WGL_CMD_SBTL_CLEAR_ALL,
                                  NULL,
                                  SBTL_WGL_PACK(t_pts_current));
            
                if (MMSBTLR_OK != i4_ret)
                {
                    return (MMSBTLR_WGL_FAIL);
                }            
            }               
        }
        
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
static UINT32 _sbtl_divx_asci2decimal(UINT8*   aui1_ary,
                                      UINT8    ui1_round)
{
    UINT32  ui4_result = 0;
    UINT32  ui4_multiply = 1;
    while(ui1_round)
    {
        ui1_round -= 1;
        ui4_result += (aui1_ary[ui1_round]-'0')*ui4_multiply;
        ui4_multiply *= 10;
    }

    return ui4_result;
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
static INT32 _sbtl_divx_in_range(DIVX_SUBPICT_HDR_T*    pt_subpict_hdr,
                                 PTS_T*                 pt_pts_start,
                                 PTS_T*                 pt_pts_end)
{
    UINT32  ui4_hour;
    UINT32  ui4_min;
    UINT32  ui4_sec;
    UINT32  ui4_ms;

    ui4_hour = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[1], 2);
    ui4_min = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[4], 2);
    ui4_sec = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[7], 2);
    ui4_ms = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[10], 3);

    *pt_pts_start = (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                    + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                    + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                    + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*3600);

    ui4_hour = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[14], 2);
    ui4_min = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[17], 2);
    ui4_sec = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[20], 2);
    ui4_ms = _sbtl_divx_asci2decimal(&pt_subpict_hdr->aui1_duration[23], 3);

    *pt_pts_end =   (UINT64)_mm_div64(PTS_FREQ*(UINT64)ui4_ms, 1000, NULL)
                    + (UINT64)(PTS_FREQ*(UINT64)ui4_sec)
                    + (UINT64)(PTS_FREQ*(UINT64)ui4_min*60)
                    + (UINT64)(PTS_FREQ*(UINT64)ui4_hour*3600);


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
static INT32 _sbtl_divx_new_page(SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    BOOL                            b_found;
    UINT32                          ui4_data_1;
    UINT32                          ui4_data_2;
    UINT32                          ui4_pts;
    UINT32                          ui4_duration;
    INT32                           i4_ret;
    INT32                           i4_raw_size;
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;
    PTS_T                           t_pts_start;
    PTS_T                           t_pts_end;
    PTS_T                           t_pts_current;

    ASSERT(pt_mngr, ("pt_mngr=NULL\n"));
    ASSERT(pt_mngr->pv_private, ("pt_mngr->pv_private=NULL\n"));
    ASSERT(pt_mngr->pt_engine, ("pt_mngr->pt_engine=NULL\n"));
    if ((NULL==pt_mngr) || (NULL==pt_mngr->pv_private) || (NULL==pt_mngr->pt_engine))
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    
    if (MM_DIVX_SBTL_TITLE_TYPE == pt_mngr->pt_engine->t_cfg.e_divx_sbtl_type)
    {
        if (!pt_mngr->b_started)
        {
            return MMSBTLR_OK;
        }
        
        if (pt_private->b_page_exist)
        {
            return MMSBTLR_OK;
        }
        
        pt_private->e_state = DIVX_STATE_NONE;
        
        do
        {
            b_found = sbtl_dmx_data_que_retrieve(pt_mngr->pt_engine,
                                                 &ui4_data_1,
                                                 &ui4_data_2,
                                                 &ui4_pts,
                                                 &ui4_duration,
                                                 &pt_private->t_append_stc);

            if (b_found)
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
                        return MMSBTLR_CORE;
                    }
                }
                
                pt_private->ui4_pes_buf_size = ui4_data_2;

                i4_ret = sbtl_dmx_retrive_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2,
                                               pt_private->aui1_pes_buf);
                if (MMSBTLR_OK != i4_ret)
                {
                    ASSERT(0, ("DEMUX_GET_TYPE_READ_BUFFER fail\n"));
                }

                
                i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2);
                if (MMSBTLR_OK != i4_ret)
                {
                    ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
                }
                
                if (pt_private->pt_subpict_hdr)
                {
                    x_mem_free(pt_private->pt_subpict_hdr);
                    pt_private->pt_subpict_hdr = NULL;
                }
                if((pt_private->b_hd && pt_private->ui4_pes_buf_size >= 57) || ((!pt_private->b_hd) && pt_private->ui4_pes_buf_size >= 53))
                {
                    pt_private->pt_subpict_hdr = _sub_parser_subpacket2rle_alloc(pt_private, pt_private->aui1_pes_buf);
                }
                else
                {
                    break;
                }
                ASSERT(pt_private->pt_subpict_hdr, ("pt_private->pt_subpict_hdr=NULL\n"));
                if (NULL == pt_private->pt_subpict_hdr)
                {
                    break;
                }

                if ((pt_private->pt_subpict_hdr->ui2_width  > 1440) ||  
                    (pt_private->pt_subpict_hdr->ui2_height > 1080))
                {
                    DBG_INFO((_INFO_HEADER"wrong size %d %d\n", pt_private->pt_subpict_hdr->ui2_width, pt_private->pt_subpict_hdr->ui2_height));
                    break;
                }
                
                pt_private->at_clut[0].a = pt_private->t_subalpha_hdr.ui1_background_transparency;
                pt_private->at_clut[0].u1.r = pt_private->pt_subpict_hdr->t_background.ui1_red;
                pt_private->at_clut[0].u2.g = pt_private->pt_subpict_hdr->t_background.ui1_green;
                pt_private->at_clut[0].u3.b = pt_private->pt_subpict_hdr->t_background.ui1_blue;

                pt_private->at_clut[1].a = pt_private->t_subalpha_hdr.ui1_pattern_transparency;
                pt_private->at_clut[1].u1.r = pt_private->pt_subpict_hdr->t_pattern.ui1_red;
                pt_private->at_clut[1].u2.g = pt_private->pt_subpict_hdr->t_pattern.ui1_green;
                pt_private->at_clut[1].u3.b = pt_private->pt_subpict_hdr->t_pattern.ui1_blue;

                pt_private->at_clut[2].a = pt_private->t_subalpha_hdr.ui1_emphasis1_transparency;
                pt_private->at_clut[2].u1.r = pt_private->pt_subpict_hdr->t_emphasis1.ui1_red;
                pt_private->at_clut[2].u2.g = pt_private->pt_subpict_hdr->t_emphasis1.ui1_green;
                pt_private->at_clut[2].u3.b = pt_private->pt_subpict_hdr->t_emphasis1.ui1_blue;

                pt_private->at_clut[3].a = pt_private->t_subalpha_hdr.ui1_emphasis2_transparency;
                pt_private->at_clut[3].u1.r = pt_private->pt_subpict_hdr->t_emphasis2.ui1_red;
                pt_private->at_clut[3].u2.g = pt_private->pt_subpict_hdr->t_emphasis2.ui1_green;
                pt_private->at_clut[3].u3.b = pt_private->pt_subpict_hdr->t_emphasis2.ui1_blue;

                pt_private->t_rgn.at_clut = pt_private->at_clut;

                pt_private->t_rgn.b_dirty = TRUE;
                pt_private->t_rgn.e_clr_mode = SBTL_WGL_SBTL_COLOR_MODE_CLUT_4_BIT;
                
                pt_private->t_rgn.i4_top = (INT32)pt_private->pt_subpict_hdr->ui2_top;
                pt_private->t_rgn.i4_left = (INT32)pt_private->pt_subpict_hdr->ui2_left;
                pt_private->t_rgn.ui4_width = (UINT32)(pt_private->pt_subpict_hdr->ui2_right
                                                     - pt_private->pt_subpict_hdr->ui2_left
                                                     + 1);
                
                pt_private->t_rgn.ui4_height = (UINT32)(pt_private->pt_subpict_hdr->ui2_bottom
                                                      - pt_private->pt_subpict_hdr->ui2_top
                                                      + 1);

                pt_private->t_rgn.t_rc_dirty.i4_bottom = (INT32)pt_private->t_rgn.ui4_height;
                pt_private->t_rgn.t_rc_dirty.i4_top = 0;
                pt_private->t_rgn.t_rc_dirty.i4_left = 0;
                pt_private->t_rgn.t_rc_dirty.i4_right = (INT32)pt_private->t_rgn.ui4_width;
                
                if (pt_private->t_rgn.pv_bitmap)
                {
                    x_mem_free(pt_private->t_rgn.pv_bitmap);
                    pt_private->t_rgn.pv_bitmap = NULL;
                }
                pt_private->t_rgn.pv_bitmap = _sub_parser_rle2raw_alloc(pt_private->pt_subpict_hdr,
                                                                        (INT32)(ui4_data_2 - SUBPICT_PACKHDR_SIZE),
                                                                        &i4_raw_size);

                ASSERT(pt_private->t_rgn.pv_bitmap, ("pt_private->t_rgn.pv_bitmap=NULL\n"));
                if (NULL == pt_private->t_rgn.pv_bitmap)
                {
                    break;;
                }

                _sbtl_divx_in_range(pt_private->pt_subpict_hdr,
                                   &t_pts_start,
                                   &t_pts_end);

                DBG_INFO((_INFO_HEADER"divx pts_start %d\n",(UINT32)t_pts_start));
                
                i4_ret = sbtl_stc_pts(pt_mngr->pt_engine,
                                       &t_pts_current);

                if (i4_ret != MMSBTLR_OK)
                {
                    ASSERT(0, ("Get STC Error!!\n"));
                    break;
                }


                pt_private->t_pts_s = t_pts_start;
                pt_private->t_pts_e = t_pts_end;
                
                if (t_pts_current < t_pts_start)
                {
                    pt_private->e_state = DIVX_STATE_DATA_RDY;
                    pt_private->b_page_exist = TRUE;
                    return MMSBTLR_OK;
                }
                else if (t_pts_current >= t_pts_start && t_pts_current <= t_pts_end)
                {
                    pt_private->e_state = DIVX_STATE_DISPLAY;
                    /*display*/
                    if(pt_mngr->pt_engine->t_cfg.b_hide == FALSE)
                    {
                    i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                          SBTL_WGL_CMD_SBTL_FLIP,
                                          (VOID*)pt_mngr,
                                          (VOID*)&pt_private->t_pts_s);

                    if (i4_ret != MMSBTLR_OK)
                    {
                        ASSERT(0, ("wgl flip error!!\n"));
                    }
                    }
                    
                    DBG_INFO((_INFO_HEADER"divx flip0 %d %d %d\n",
                             (UINT32)pt_private->t_append_stc, 
                             (UINT32)pt_private->t_pts_s, 
                             (UINT32)(t_pts_current-pt_private->t_pts_s)));

                    pt_private->b_page_exist = TRUE;
                    return MMSBTLR_OK;
                }
                else
                {
                    if (pt_private->pt_subpict_hdr)
                    {
                        x_mem_free(pt_private->pt_subpict_hdr);
                        pt_private->pt_subpict_hdr = NULL;
                    }
                    if (pt_private->t_rgn.pv_bitmap)
                    {
                        x_mem_free(pt_private->t_rgn.pv_bitmap);
                        pt_private->t_rgn.pv_bitmap = NULL;
                    }
                    continue;
                }
            }
            else
            {
                return MMSBTLR_NOT_FOUND;
            }
            
        }while(1);
        
        if (pt_private->pt_subpict_hdr)
        {
            x_mem_free(pt_private->pt_subpict_hdr);
            pt_private->pt_subpict_hdr = NULL;
        }
        if (pt_private->t_rgn.pv_bitmap)
        {
            x_mem_free(pt_private->t_rgn.pv_bitmap);
            pt_private->t_rgn.pv_bitmap = NULL;
        }
    }
    else
    {
        SBTL_DIVX_MENU_PAGE_T*  pt_menu_page;
        BOOL                    b_found_page = FALSE;
        UINT16                  ui2_list_idx = 0;
        
        /*divx menu*/
        b_found = sbtl_dmx_data_que_retrieve(pt_mngr->pt_engine,
                                             &ui4_data_1,
                                             &ui4_data_2,
                                             &ui4_pts,
                                             &ui4_duration,
                                             &pt_private->t_append_stc);

        if (b_found)
        {
            do
            {
                i4_ret = MMSBTLR_OK;
                pt_menu_page = NULL;
                
                pt_menu_page = x_mem_alloc(sizeof(SBTL_DIVX_MENU_PAGE_T));
                ASSERT(pt_menu_page, ("pt_menu_page=NULL\n"));
                
                if (NULL == pt_menu_page)
                {
                    i4_ret = MMSBTLR_CORE;
                    break;
                }

                pt_menu_page->aui1_pes_buf = NULL;
                pt_menu_page->aui1_pes_buf = x_mem_alloc(ui4_data_2);
                ASSERT(pt_menu_page->aui1_pes_buf, ("pt_menu_page->aui1_pes_buf=NULL\n"));
                
                if (NULL == pt_menu_page->aui1_pes_buf)
                {
                    i4_ret = MMSBTLR_CORE;
                    break;
                }
                
                pt_menu_page->ui4_pes_buf_size = ui4_data_2;
                
                i4_ret = sbtl_dmx_retrive_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2,
                                               pt_private->aui1_pes_buf);
                if (MMSBTLR_OK != i4_ret)
                {
                    ASSERT(0, ("DEMUX_GET_TYPE_READ_BUFFER fail\n"));
                    break;
                }
                
                
                i4_ret = sbtl_dmx_release_data(pt_mngr->pt_engine,
                                               ui4_data_1,
                                               ui4_data_2);
                if (MMSBTLR_OK != i4_ret)
                {
                    ASSERT(0, ("DEMUX_SET_TYPE_UNLOCK_BUFFER fail\n"));
                    break;
                }
                
                DLIST_INSERT_TAIL(pt_menu_page, &pt_private->t_menu_page_list, t_link);
                
                if (!pt_mngr->b_started)
                {
                    return MMSBTLR_OK;
                }
            }while(0);

            if (MMSBTLR_OK != i4_ret)
            {
                if (pt_menu_page)
                {
                    if (pt_menu_page->aui1_pes_buf)
                    {
                        x_mem_free(pt_menu_page->aui1_pes_buf);
                    }
                    x_mem_free(pt_menu_page);
                }
                return i4_ret;
            }
            
            pt_menu_page = NULL;
            
            DLIST_FOR_EACH(pt_menu_page, &pt_private->t_menu_page_list, t_link)
            {
                if (ui2_list_idx == pt_private->ui2_menu_item)
                {
                    b_found_page = TRUE;
                }
                ui2_list_idx++;
            }

            if (b_found_page)
            {
                do
                {
                    i4_ret = MMSBTLR_OK;
                    _sbtl_divx_remove(pt_mngr);

                    if(NULL == pt_menu_page)
                    {
                        i4_ret = MMSBTLR_CORE;
                        break;
                    }
                    pt_private->pt_subpict_hdr = _sub_parser_subpacket2rle_alloc(pt_private, pt_menu_page->aui1_pes_buf);
                    
                    ASSERT(pt_private->pt_subpict_hdr, ("pt_private->pt_subpict_hdr=NULL\n"));
                    if(NULL == pt_private->pt_subpict_hdr)
                    {
                        i4_ret = MMSBTLR_CORE;
                        break;
                    }
                    
                    pt_private->at_clut[0].a = pt_private->t_subalpha_hdr.ui1_background_transparency;
                    pt_private->at_clut[0].u1.r = pt_private->pt_subpict_hdr->t_background.ui1_red;
                    pt_private->at_clut[0].u2.g = pt_private->pt_subpict_hdr->t_background.ui1_green;
                    pt_private->at_clut[0].u3.b = pt_private->pt_subpict_hdr->t_background.ui1_blue;
                    
                    pt_private->at_clut[1].a = pt_private->t_subalpha_hdr.ui1_pattern_transparency;
                    pt_private->at_clut[1].u1.r = pt_private->pt_subpict_hdr->t_pattern.ui1_red;
                    pt_private->at_clut[1].u2.g = pt_private->pt_subpict_hdr->t_pattern.ui1_green;
                    pt_private->at_clut[1].u3.b = pt_private->pt_subpict_hdr->t_pattern.ui1_blue;
                    
                    pt_private->at_clut[2].a = pt_private->t_subalpha_hdr.ui1_emphasis1_transparency;
                    pt_private->at_clut[2].u1.r = pt_private->pt_subpict_hdr->t_emphasis1.ui1_red;
                    pt_private->at_clut[2].u2.g = pt_private->pt_subpict_hdr->t_emphasis1.ui1_green;
                    pt_private->at_clut[2].u3.b = pt_private->pt_subpict_hdr->t_emphasis1.ui1_blue;
                    
                    pt_private->at_clut[3].a = pt_private->t_subalpha_hdr.ui1_emphasis2_transparency;
                    pt_private->at_clut[3].u1.r = pt_private->pt_subpict_hdr->t_emphasis2.ui1_red;
                    pt_private->at_clut[3].u2.g = pt_private->pt_subpict_hdr->t_emphasis2.ui1_green;
                    pt_private->at_clut[3].u3.b = pt_private->pt_subpict_hdr->t_emphasis2.ui1_blue;
                    
                    pt_private->t_rgn.at_clut = pt_private->at_clut;
                    
                    pt_private->t_rgn.b_dirty = TRUE;
                    pt_private->t_rgn.e_clr_mode = SBTL_WGL_SBTL_COLOR_MODE_CLUT_4_BIT;
                    
                    pt_private->t_rgn.i4_top =   (INT32)((INT32)pt_private->pt_subpict_hdr->ui2_top
                                                       - (INT32)pt_private->ui1_ovs_top);
                    
                    pt_private->t_rgn.i4_left =  (INT32)((INT32)pt_private->pt_subpict_hdr->ui2_left
                                                       - (INT32)pt_private->ui1_ovs_left);
                    
                    pt_private->t_rgn.ui4_width = (UINT32)(pt_private->pt_subpict_hdr->ui2_left
                                                         - pt_private->pt_subpict_hdr->ui2_right
                                                         + 1);
                    
                    pt_private->t_rgn.ui4_height = (UINT32)(pt_private->pt_subpict_hdr->ui2_top
                                                          - pt_private->pt_subpict_hdr->ui2_bottom
                                                          + 1);
                    
                    pt_private->t_rgn.t_rc_dirty.i4_bottom = (INT32)pt_private->pt_subpict_hdr->ui2_bottom;
                    pt_private->t_rgn.t_rc_dirty.i4_top = (INT32)pt_private->pt_subpict_hdr->ui2_top;
                    pt_private->t_rgn.t_rc_dirty.i4_left = (INT32)pt_private->pt_subpict_hdr->ui2_left;
                    pt_private->t_rgn.t_rc_dirty.i4_right = (INT32)pt_private->pt_subpict_hdr->ui2_right;
                    
                    
                    pt_private->t_rgn.pv_bitmap = _sub_parser_rle2raw_alloc(pt_private->pt_subpict_hdr,
                                                                            (INT32)(ui4_data_2 - SUBPICT_PACKHDR_SIZE),
                                                                            &i4_raw_size);
                    
                    ASSERT(pt_private->t_rgn.pv_bitmap, ("pt_private->t_rgn.pv_bitmap=NULL\n"));
                    if(NULL == pt_private->t_rgn.pv_bitmap)
                    {
                        i4_ret = MMSBTLR_CORE;
                        break;
                    }
                    
                    pt_private->e_state = DIVX_STATE_DISPLAY;
                    /*display*/
                    i4_ret = sbtl_wgl_do_cmd(pt_private->h_sbtl_wgl,
                                          SBTL_WGL_CMD_SBTL_FLIP,
                                          (VOID*)pt_mngr,
                                          (VOID*)&pt_private->t_pts_s);
                    
                    if (i4_ret != MMSBTLR_OK)
                    {
                        ASSERT(0, ("wgl flip error!!\n"));
                        break;
                    }
                    
                    return MMSBTLR_OK;

                }while(0);
                if (MMSBTLR_OK != i4_ret)
                {
                    if (pt_private->pt_subpict_hdr)
                    {
                        x_mem_free(pt_private->pt_subpict_hdr);
                        pt_private->pt_subpict_hdr = NULL;
                    }
                    if (pt_private->t_rgn.pv_bitmap)
                    {
                        x_mem_free(pt_private->t_rgn.pv_bitmap);
                        pt_private->t_rgn.pv_bitmap = NULL;
                    }
                    return i4_ret;
                }
            }
        }

    }
    
    /*decode divx subtitle*/
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
static SBTL_PAGE_INST_MNGR_T* _divx_page_mngr_constructor (SBTL_ENGINE_T*   pt_engine)
{
    SBTL_PAGE_INST_MNGR_T*          pt_mngr;
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;

    ASSERT(pt_engine, ("_divx_page_mngr_constructor pt_engine=NULL\n"));
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

        pt_mngr->pf_start = _sbtl_divx_start;
        pt_mngr->pf_stop = _sbtl_divx_stop;
        pt_mngr->pf_cfg = _sbtl_divx_config;
        pt_mngr->pf_handle_timing = _sbtl_divx_handle_timing;
        pt_mngr->pf_need_dmx = _sbtl_divx_need_dmx;
        pt_mngr->pf_new_page = _sbtl_divx_new_page;
        
        pt_mngr->pt_engine = pt_engine;
        
        pt_mngr->pv_private = x_mem_alloc(sizeof(SBTL_DIVX_PAGE_MNGR_PRIVATE_T));

        if (NULL == pt_mngr->pv_private)
        {
            break;
        }
        x_memset(pt_mngr->pv_private, 0, sizeof(SBTL_DIVX_PAGE_MNGR_PRIVATE_T));

        pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
        pt_private->b_hd = pt_engine->t_cfg.b_divx_hd;
        pt_private->ui2_menu_item = pt_engine->t_cfg.ui2_menu_item;
        
        DLIST_INIT(&pt_private->t_menu_page_list);
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
static VOID _divx_page_mngr_destructor (SBTL_PAGE_INST_MNGR_T*  pt_mngr)
{
    SBTL_DIVX_PAGE_MNGR_PRIVATE_T*  pt_private;
    SBTL_DIVX_MENU_PAGE_T*          pt_menu_page;

    ASSERT(pt_mngr, ("_divx_page_mngr_destructor pt_mngr=NULL\n"));
    if (NULL==pt_mngr)
    {
        return;
    }
    
    pt_private = (SBTL_DIVX_PAGE_MNGR_PRIVATE_T*)pt_mngr->pv_private;
    ASSERT(pt_private, ("_divx_page_mngr_destructor pt_private=NULL\n"));
    if (NULL==pt_private)
    {
        return;
    }

    if (pt_private->pt_subpict_hdr)
    {
        x_mem_free(pt_private->pt_subpict_hdr);
        pt_private->pt_subpict_hdr = NULL;
    }

    if (pt_private->aui1_pes_buf)
    {
        x_mem_free(pt_private->aui1_pes_buf);
        pt_private->aui1_pes_buf = NULL;
    }

    while((pt_menu_page = DLIST_HEAD(&pt_private->t_menu_page_list)) != NULL)
    {
        DLIST_REMOVE(pt_menu_page, &pt_private->t_menu_page_list, t_link);
        if (pt_menu_page)
        {
            x_mem_free(pt_menu_page->aui1_pes_buf);
            x_mem_free(pt_menu_page);
        }
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
VOID x_sbtl_divx_init(VOID)
{
    sbtl_register(MM_SBTL_TYPE_DIVX, 
                  &t_divx_fnct_tbl);
}

