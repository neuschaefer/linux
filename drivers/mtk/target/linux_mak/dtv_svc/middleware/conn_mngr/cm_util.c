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
 * $RCSfile: cm_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the Connection Manager util functions, which can
 *         be used by other components (e.g. connection handlers).
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "conn_mngr/_cm.h"
#include "scdb/scdb_api.h"
#include "tbl_mngr/x_tm.h"
#include "tbl_mngr/psi_eng/x_psi_eng.h"
#include "dbg/x_dbg.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_aud_dec.h"

#ifdef MW_DVBS_DEV_ENABLE
#include "res_mngr/drv/x_tuner.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define VALID_RD3D_FORMAT_IDENTIFIER(p) ((p[2]=='R') && (p[3]=='D') && (p[4]=='3') && (p[5]=='D'))

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static UINT32  g_ui4_audio_formats = 0;
#ifdef MW_DVBS_DEV_ENABLE
static BOOL    g_b_is_microcontroller_reset = FALSE;
#endif

/*-----------------------------------------------------------------------------
 * Name: tm_parse_get_mvc_ext_info
 *
 * Description: This API is the MVC extension descriptor parse function.
 *              All fields will be copied into the provided buffer
 *              (passed as pv_data).
 *
 * Inputs:  h_pmt           Contains the PMT handle.
 *          pui1_obj_data   References the descriptor data.
 *          z_obj_data_len  Contains the descriptor length.
 *          ui2_count       Contains the descriptor index.
 *          pv_data         References some data.
 *          z_data_len      Contains the length of the data.
 *
 * Outputs: -
 *
 * Returns: PARSE_RET_NEXT
 ----------------------------------------------------------------------------*/
static PARSE_RET_T tm_parse_get_mvc_ext_info (HANDLE_T      h_pmt,
                                                 const UINT8*  pui1_obj_data,
                                                 SIZE_T        z_obj_data_len,
                                                 UINT16        ui2_count,
                                                 VOID*         pv_data,
                                                 SIZE_T        z_data_len)
{
    /* minimum size = 6 bytes (i.e. at least one language in the loop) */

    if (VALID_DESCRIPTOR(0x31, 10))
    {
        MVC_EXTENSION_DESC_T*   pt_info;

        pt_info = (MVC_EXTENSION_DESC_T *) pv_data;

        pt_info->b_mvc_video             = TRUE;
        pt_info->b_mvc_ext_desc          = TRUE;    /* describe MVC extension descriptor exist or not */
        pt_info->ui2_avg_bit_rate        = ((UINT16)pui1_obj_data[2]<<8) + pui1_obj_data[3];
        pt_info->ui2_max_bit_rate        = ((UINT16)pui1_obj_data[4]<<8) + pui1_obj_data[5];
        pt_info->ui2_view_ord_idx_min    = ((UINT16)(pui1_obj_data[6]&0xF)<<6) + (pui1_obj_data[7]>>2);
        pt_info->ui2_view_ord_idx_max    = ((UINT16)(pui1_obj_data[7]&0x3)<<8) + pui1_obj_data[8];
        pt_info->ui1_temporal_id_start   = pui1_obj_data[9]>>5;
        pt_info->ui1_temporal_id_end     = (pui1_obj_data[9]>>2)&0x7;
        pt_info->b_no_sei_nal_present    = ((pui1_obj_data[9]&0x2)==0)?FALSE:TRUE;
        pt_info->b_no_prefix_nal_present = ((pui1_obj_data[9]&0x1)==0)?FALSE:TRUE;
        pt_info->e_mvc_src               = MVC_SRC_TYPE_STRM_TAG;
    }

    return PARSE_RET_NEXT;
}


/*-----------------------------------------------------------------------------
 * Name: cm_get_mvc_ext_info
 *
 * Description: This API retrieves the MVC extension descriptor in a PMT.
 *
 * Inputs:  h_pmt    Contains the PMT handle.
 *          ui2_idx  Contains the loop index in the PMT.
 *
 * Outputs: pt_info  Contains the MVC extension information.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_get_mvc_ext_info(HANDLE_T                h_pmt,
                                UINT16                  ui2_index,
                                MVC_EXTENSION_DESC_T*   pt_info)
{
    TM_FIRST_LOOP_DESCR_SEL_T  t_descr;
    UINT8                      ui1_descriptor_tag;

    if(NULL == pt_info)
    {
        DBG_ERROR((DBG_PREFIX"pt_info is NULL\n"));
        return;
    }

    ui1_descriptor_tag = 0x31; /* MVC extension descriptor tag */

    t_descr.ui2_loop_idx        = ui2_index;
    t_descr.pui1_descr_tag_list = & ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;


    x_tm_parse_obj(h_pmt,
                   TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                   & t_descr,
                   pt_info,
                   sizeof(MVC_EXTENSION_DESC_T),
                   tm_parse_get_mvc_ext_info,
                   NULL);
}

/*-----------------------------------------------------------------------------
 * Name: tm_parse_get_rd3d_reg_info
 *
 * Description: This API is the RD3D registration descriptor parse function.
 *              The layer type and coding method will be
 *              copied into the provided buffer (passed as pv_data).
 *
 * Inputs:  h_pmt           Contains the PMT handle.
 *          pui1_obj_data   References the descriptor data.
 *          z_obj_data_len  Contains the descriptor length.
 *          ui2_count       Contains the descriptor index.
 *          pv_data         References some data.
 *          z_data_len      Contains the length of the data.
 *
 * Outputs: -
 *
 * Returns: PARSE_RET_NEXT
 ----------------------------------------------------------------------------*/
static PARSE_RET_T tm_parse_get_rd3d_reg_info (HANDLE_T      h_pmt,
                                                  const UINT8*  pui1_obj_data,
                                                  SIZE_T        z_obj_data_len,
                                                  UINT16        ui2_count,
                                                  VOID*         pv_data,
                                                  SIZE_T        z_data_len)
{
    /* minimum size = 6 bytes (i.e. at least one language in the loop) */

    if ((VALID_DESCRIPTOR(0x05, 7))
        && VALID_RD3D_FORMAT_IDENTIFIER(pui1_obj_data))
    {
        RD3D_REG_DESC_T*  pt_info;

        pt_info = (RD3D_REG_DESC_T *) pv_data;

        pt_info->b_rd3d = TRUE;
        pt_info->e_layer_type = (REALD_LAYER_TYPE_T)pui1_obj_data[6];
        if(pt_info->e_layer_type == 0)
        {
            pt_info->e_coding_method = (REALD_CODING_METHOD_T)pui1_obj_data[7];
        }
    }

    return PARSE_RET_NEXT;
}

/*-----------------------------------------------------------------------------
 * Name: cm_get_rd3d_layer_type
 *
 * Description: This API retrieves the RD3D layer type in PMT ES loop.
 *
 * Inputs:  h_pmt    Contains the PMT handle.
 *          ui2_idx  Contains the loop index in the PMT.
 *
 * Outputs: pe_layer  Contains the RD3D layer type.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_get_rd3d_layer_type(HANDLE_T                h_pmt,
                                   UINT16                  ui2_index,
                                   REALD_LAYER_TYPE_T*     pe_layer)
{
    TM_FIRST_LOOP_DESCR_SEL_T   t_descr;
    UINT8                       ui1_descriptor_tag;
    RD3D_REG_DESC_T             t_info;
    INT32                       i4_ret;

    x_memset(&t_info, 0, sizeof(RD3D_REG_DESC_T));

    ui1_descriptor_tag = 0x05; /* Registration descriptor tag */

    t_descr.ui2_loop_idx        = ui2_index;
    t_descr.pui1_descr_tag_list = & ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;

    *pe_layer = REALD_UNKNOWN_LAYER;

    i4_ret = x_tm_parse_obj(h_pmt,
                           TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                           & t_descr,
                           & t_info,
                           sizeof(RD3D_REG_DESC_T),
                           tm_parse_get_rd3d_reg_info,
                           NULL);

    if((i4_ret==TMR_OK) && (t_info.b_rd3d==TRUE))
    {
        *pe_layer = t_info.e_layer_type;
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_get_rd3d_reg_info
 *
 * Description: This API retrieves the RD3D info in PMT main body loop.
 *
 * Inputs:  h_pmt    Contains the PMT handle.
 *
 * Outputs: pt_info  Contains the RD3D information.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_get_rd3d_reg_info(HANDLE_T               h_pmt,
                                UINT16                  ui2_index,
                                RD3D_REG_DESC_T*        pt_info)
{
    TM_MAIN_BODY_DESCR_SEL_T    t_descr;
    UINT8                       ui1_descriptor_tag;
    INT32 i4_ret;

    ui1_descriptor_tag = 0x05; /* Registration descriptor tag */

    t_descr.pui1_descr_tag_list = & ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;

    pt_info->b_rd3d = FALSE;

    i4_ret = x_tm_parse_obj(h_pmt,
                           TM_PARSE_TYPE_MAIN_BODY_DESCRS,
                           & t_descr,
                           pt_info,
                           sizeof(RD3D_REG_DESC_T),
                           tm_parse_get_rd3d_reg_info,
                           NULL);

    if((i4_ret==TMR_OK) && (pt_info->b_rd3d==TRUE))
    {
        cm_get_rd3d_layer_type(h_pmt, ui2_index, &pt_info->e_layer_type);
    }
}


/*-----------------------------------------------------------------------------
 * Name: cm_is_valid_connection
 *
 * Description: This API checks whether a handle is a valid connection handle.
 *
 * Inputs:  h_obj  Contains the handle to check.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle is a handle to a connection, FALSE otherwise.
 ----------------------------------------------------------------------------*/
BOOL cm_is_valid_connection (HANDLE_T  h_obj)
{
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;

    i4_res = x_handle_get_type(h_obj, & e_type);

    return (BOOL) ((i4_res == HR_OK) && (e_type == CMT_CONNECTION));
}

/*-----------------------------------------------------------------------------
 * Name: cm_comp_rm_select_init
 *
 * Description: This API initializes the select structure.
 *
 * Inputs:  pt_select  References the structure to initialize.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_comp_rm_select_init (CM_COMP_RM_SELECT_T*  pt_select)
{
    x_memset(pt_select, 0, sizeof(CM_COMP_RM_SELECT_T));
}

/*-----------------------------------------------------------------------------
 * Name: cm_comp_rm_select
 *
 * Description: This API is called when a component is being opened. This API
 *              will find a component to open (if any). See comments below in
 *              code for selection policy.
 *
 * Inputs:  e_type            Contains the driver type.
 *          ui2_id            Contains the driver component id.
 *          pv_comp_sel_data  References private data passed in rm_open_comp.
 *          pv_comp_data      References the component's private data.
 *          z_comp_data_len   Contains the length of *pv_comp_data.
 *          ui2_num_active    Contains the number of handles to this component
 *                            in OPENED state.
 *          ui2_num_wait      Contains the number of handles to this component
 *                            in OPENING state.
 *          e_ctrl            Contains the control type of the component.
 *          b_would_conflict  Specifies if a conflict could occur.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
SELECT_T cm_comp_rm_select (DRV_TYPE_T   e_type,
                            UINT16       ui2_id,
                            VOID*        pv_comp_sel_data,
                            const VOID*  pv_comp_data,
                            SIZE_T       z_comp_data_len,
                            UINT16       ui2_num_active,
                            UINT16       ui2_num_wait,
                            CTRL_TYPE_T  e_ctrl,
                            BOOL         b_would_conflict)
{
    CM_COMP_RM_SELECT_T*  pt_select;
    SELECT_T              e_select;

    pt_select = (CM_COMP_RM_SELECT_T *) pv_comp_sel_data;

    pt_select->b_would_conflict = b_would_conflict;

    if (! b_would_conflict)
    {
        if ((ui2_num_active == 0) && (ui2_num_wait == 0))
        {
            /*
              component is free and no conflict -> search is over
            */

            DBG_INFO((DBG_PREFIX
                      "Component free and no conflict (type=%d, id=%d)\n",
                      e_type, ui2_id));

            e_select = SEL_COMP_BREAK;
        }
        else
        {
            /*
              component is not free, but no conflict -> component is selected
              but the search continues (a free component could still be found)
            */

            DBG_INFO((DBG_PREFIX
                      "Component not free but no conflict (type=%d, id=%d)\n",
                      e_type, ui2_id));

            e_select = SEL_COMP_NEXT;
        }
    }
    else
    {
        if (ui2_num_active == 0)
        {
            /*
              conflict exists -> depending on b_select_if_conflict the
              component could be simply skipped (and not selected) or selected.
              Either way, the search for a 'better' component continues
            */

            DBG_INFO((DBG_PREFIX
                      "Component free but conflict (type=%d, id=%d) (%s)\n",
                      e_type, ui2_id,
                      pt_select->b_select_if_conflict ? "select" : "ignore"));

            e_select =
                pt_select->b_select_if_conflict ? SEL_COMP_NEXT : SEL_NEXT;
        }
        else
        {
            /*
              a conflict when ui2_num_active is not 0 means the component is
              already opened on a different pipe with a higher priority.
            */

            DBG_INFO((DBG_PREFIX
                      "Component already opened on higher priority pipe "
                      "(type=%d, id=%d)\n",
                      e_type, ui2_id));

            e_select = pt_select->b_select_if_lower_priority ?
                SEL_COMP_NEXT : SEL_NEXT;
        }
    }

    return e_select;
}

/*-----------------------------------------------------------------------------
 * Name: cm_remove_stream_type_from_scdb
 *
 * Description: This API removes all the streams of a given stream type in a
 *              given SCDB. The SCDB must be locked.
 *
 * Inputs:  h_scdb         Contains a handle to an SCDB.
 *          e_stream_type  Contains the stream type.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    All streams were removed successfully.
 *          CMR_FAIL  Not all streams were removed. Some may have been removed
 *                    but at least one stream could not be removed.
 ----------------------------------------------------------------------------*/
INT32 cm_remove_stream_type_from_scdb (HANDLE_T       h_scdb,
                                       STREAM_TYPE_T  e_stream_type)
{
    STREAM_COMP_ID_T  t_stream_desc;
    SCDB_REC_T        t_scdb_rec;
    UINT32            ui4_version;
    INT32             i4_res;

    ui4_version = SCDB_NULL_VER_ID;

    while ((i4_res = x_scdb_get_rec_by_idx(h_scdb,
                                           e_stream_type,
                                           0,
                                           & t_stream_desc,
                                           & t_scdb_rec,
                                           & ui4_version))
           == SCDBR_OK)
    {
        i4_res = scdb_del_rec(h_scdb, & t_stream_desc);

        if (i4_res != SCDBR_OK)
        {
            i4_res = SCDBR_FAIL;
            break;
        }

        ui4_version = SCDB_NULL_VER_ID;
    }

    i4_res = (i4_res == SCDBR_REC_NOT_FOUND) ? CMR_OK : CMR_FAIL;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_scdb_has_stream_type
 *
 * Description: This API checks if a stream with a given stream type exists in
 *              a given SCDB..
 *
 * Inputs:  h_scdb         Contains a handle to an SCDB.
 *          e_stream_type  Contains the stream type.
 *
 * Outputs: -
 *
 * Returns: TRUE or FALSE
 ----------------------------------------------------------------------------*/
BOOL cm_scdb_has_stream_type (HANDLE_T       h_scdb,
                              STREAM_TYPE_T  e_stream_type)
{
    STREAM_COMP_ID_T  t_stream_desc;
    SCDB_REC_T        t_scdb_rec;
    UINT32            ui4_version;
    INT32             i4_res;

    ui4_version = SCDB_NULL_VER_ID;

    i4_res = x_scdb_get_rec_by_idx(h_scdb,
                                   e_stream_type,
                                   0,
                                   & t_stream_desc,
                                   & t_scdb_rec,
                                   & ui4_version);

    return (i4_res == SCDBR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: cm_conn_show_scdb
 *
 * Description: This API lists the content of a connection's SCDB.
 *
 * Inputs:  h_conn     Contains a handle to a connection.
 *          ps_prefix  References a prefix string to display. Can be NULL.
 *
 * Outputs: -
 *
 * Returns: CMR_OK
 *          CMR_FAIL
 ----------------------------------------------------------------------------*/
VOID cm_conn_show_scdb (HANDLE_T     h_conn,
                        const CHAR*  ps_prefix)
{
#ifdef DEBUG
    HANDLE_T  h_scdb;
    INT32     i4_res;

    i4_res = x_scdb_open(h_conn, & h_scdb, NULL, NULL);

    if (i4_res == SCDBR_OK)
    {
        SCDB_COMMON_T     t_scdb_common;
        STREAM_COMP_ID_T  t_stream_comp_id;
        SCDB_REC_T        t_scdb_rec;
        UINT32            ui4_version;
        UINT16            ui2_index;

        if (ps_prefix == NULL)
        {
            ps_prefix = "";
        }

        /* common part */
        i4_res = x_scdb_get_common(h_scdb, & t_scdb_common);

        if (i4_res == SCDBR_OK)
        {
            x_dbg_stmt("%sSCDB data format %d (feed mode: %d)\n",
                       ps_prefix,
                       t_scdb_common.e_data_fmt,
                       t_scdb_common.e_feed_mode);
        }
        else
        {
            x_dbg_stmt("%sCould not get SCDB common part (%d)\n",
                       ps_prefix, i4_res);
        }

        ui4_version = SCDB_NULL_VER_ID;
        ui2_index   = 0;

        while ((i4_res = x_scdb_get_rec_by_idx(h_scdb,
                                               ST_UNKNOWN,
                                               ui2_index,
                                               & t_stream_comp_id,
                                               & t_scdb_rec,
                                               & ui4_version))
               == SCDBR_OK)
        {
            switch (t_scdb_rec.e_rec_type)
            {
                case SCDB_REC_TYPE_VIDEO_MPEG:
                {
                    SCDB_VIDEO_MPEG_T*  pt_video_mpeg;

                    pt_video_mpeg = & t_scdb_rec.u.t_video_mpeg;

                    x_dbg_stmt("%sMPEG Video (pid=%d, tag=%p)\n",
                               ps_prefix,
                               pt_video_mpeg->ui2_pid,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_VIDEO_ANALOG:
                {
                    x_dbg_stmt("%sAnalog video (tag=%p)\n",
                               ps_prefix,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_MPEG:
                {
                    SCDB_AUDIO_MPEG_T*  pt_audio_mpeg;

                    pt_audio_mpeg = & t_scdb_rec.u.t_audio_mpeg;

                    x_dbg_stmt("%sMPEG Audio \"%s\" (pid=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_mpeg->s_lang,
                               pt_audio_mpeg->ui2_pid,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_ANALOG:
                {
                    x_dbg_stmt("%sAnalog audio (tag=%p)\n",
                               ps_prefix,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_CLOSED_CAPTION:
                {
                    SCDB_CLOSED_CAPTION_T*  pt_atsc_cc;

                    pt_atsc_cc = & t_scdb_rec.u.t_atsc_cc;

                    if (pt_atsc_cc->e_type == CC_ATSC_LINE_21)
                    {
                        CC_ATSC_LINE_21_T*  pt_cc_atsc_line_21;

                        pt_cc_atsc_line_21 =
                            & pt_atsc_cc->udata.t_cc_atsc_line_21;

                        x_dbg_stmt
                            ("%sCC%d \"%s\"  E%d W%d %s (tag=%p)\n",
                             ps_prefix,
                             pt_cc_atsc_line_21->ui1_cc_id,
                             pt_atsc_cc->s_lang,
                             pt_cc_atsc_line_21->b_easy_reader,
                             pt_cc_atsc_line_21->b_wide_aspect,
                             pt_cc_atsc_line_21->b_det_in_descr?
                             "DETECTED" : "UNDETECTED",
                             t_stream_comp_id.pv_stream_tag);
                    }
                    else
                    {
                        CC_ATSC_DTV_T*  pt_cc_atsc_dtv;

                        pt_cc_atsc_dtv = & pt_atsc_cc->udata.t_cc_atsc_dtv;

                        x_dbg_stmt("%sAdv CC svc%d \"%s\"  E%d W%d (tag=%p)\n",
                                   ps_prefix,
                                   pt_cc_atsc_dtv->ui1_caption_svc_num,
                                   pt_atsc_cc->s_lang,
                                   pt_cc_atsc_dtv->b_easy_reader,
                                   pt_cc_atsc_dtv->b_wide_aspect,
                                   t_stream_comp_id.pv_stream_tag);
                    }
                }
                break;

                case SCDB_REC_TYPE_SUBTITLE_DVB:
                {
                    SCDB_SUBTITLE_DVB_T*  pt_dvb_sbtl;

                    pt_dvb_sbtl = & t_scdb_rec.u.t_dvb_sbtl;

                    x_dbg_stmt("%sDVB-ST \"%s\" "
                               "composition=%d ancilary=%d (pid=%d, tag=%p)\n",
                               ps_prefix,
                               pt_dvb_sbtl->s_lang,
                               pt_dvb_sbtl->ui2_comp_pg_id,
                               pt_dvb_sbtl->ui2_anci_pg_id,
                               pt_dvb_sbtl->ui2_pid,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_DIGITAL:
                {
                    SCDB_AUDIO_DIGITAL_T*  pt_audio_digital;

                    pt_audio_digital = & t_scdb_rec.u.t_audio_digital;

                    x_dbg_stmt("%sDigital audio "
                               "(encoding=%d, layer=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_digital->e_enc,
                               pt_audio_digital->e_layer,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_AVI:
                {
                    SCDB_AUDIO_AVI_T*  pt_audio_avi;

                    pt_audio_avi = & t_scdb_rec.u.t_audio_avi;

                    x_dbg_stmt("%sAVI audio (encoding=%d, layer=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_avi->e_enc,
                               pt_audio_avi->e_layer,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_VIDEO_AVI:
                {
                    SCDB_VIDEO_AVI_T*  pt_video_avi;

                    pt_video_avi = & t_scdb_rec.u.t_video_avi;

                    x_dbg_stmt("%sAVI video (%dx%d, encoding=%d, tag=%p)\n",
                               ps_prefix,
                               pt_video_avi->i4_width,
                               pt_video_avi->i4_height,
                               pt_video_avi->e_enc,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_SUBTITLE_DIVX:
                {
                    SCDB_SUBTITLE_DIVX_T*  pt_subtitle_avi;

                    pt_subtitle_avi = & t_scdb_rec.u.t_subtitle_avi;

                    x_dbg_stmt("%sDIVX subtitles \"%s\" (%dx%d, tag=%p)\n",
                               ps_prefix,
                               pt_subtitle_avi->s_lang,
                               pt_subtitle_avi->i4_width,
                               pt_subtitle_avi->i4_height,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_MP4:
                {
                    SCDB_AUDIO_MP4_T*  pt_audio_mp4;

                    pt_audio_mp4 = & t_scdb_rec.u.t_audio_mp4;

                    x_dbg_stmt("%sMP4 audio (encoding=%d, layer=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_mp4->e_enc,
                               pt_audio_mp4->e_layer,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_VIDEO_MP4:
                {
                    SCDB_VIDEO_MP4_T*  pt_video_mp4;

                    pt_video_mp4 = & t_scdb_rec.u.t_video_mp4;

                    x_dbg_stmt("%sMP4 video (%dx%d, encoding=%d, tag=%p)\n",
                               ps_prefix,
                               pt_video_mp4->i4_width,
                               pt_video_mp4->i4_height,
                               pt_video_mp4->e_enc,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_ASF:
                {
                    SCDB_AUDIO_ASF_T*  pt_audio_asf;

                    pt_audio_asf = & t_scdb_rec.u.t_audio_asf;

                    x_dbg_stmt("%sASF audio (encoding=%d, layer=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_asf->e_enc,
                               pt_audio_asf->e_layer,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_VIDEO_ASF:
                {
                    SCDB_VIDEO_ASF_T*  pt_video_asf;

                    pt_video_asf = & t_scdb_rec.u.t_video_asf;

                    x_dbg_stmt("%sASF video (%dx%d, encoding=%d, tag=%p)\n",
                               ps_prefix,
                               pt_video_asf->i4_width,
                               pt_video_asf->i4_height,
                               pt_video_asf->e_enc,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_PS:
                {
                    SCDB_AUDIO_PS_T*  pt_audio_ps;

                    pt_audio_ps = & t_scdb_rec.u.t_audio_ps;

                    x_dbg_stmt("%sPS audio (encoding=%d, layer=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_ps->e_enc,
                               pt_audio_ps->e_layer,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_VIDEO_PS:
                {
                    SCDB_VIDEO_PS_T*  pt_video_ps;

                    pt_video_ps = & t_scdb_rec.u.t_video_ps;

                    x_dbg_stmt("%sPS video (%dx%d, encoding=%d, tag=%p)\n",
                               ps_prefix,
                               pt_video_ps->i4_width,
                               pt_video_ps->i4_height,
                               pt_video_ps->e_enc,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_MKV:
                {
                    SCDB_AUDIO_MKV_T*  pt_audio_mkv;

                    pt_audio_mkv = & t_scdb_rec.u.t_audio_mkv;

                    x_dbg_stmt("%sMKV audio (encoding=%d, layer=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_mkv->e_enc,
                               pt_audio_mkv->e_layer,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_VIDEO_MKV:
                {
                    SCDB_VIDEO_MKV_T*  pt_video_mkv;

                    pt_video_mkv = & t_scdb_rec.u.t_video_mkv;

                    x_dbg_stmt("%sMKV video (%dx%d, encoding=%d, tag=%p)\n",
                               ps_prefix,
                               pt_video_mkv->i4_width,
                               pt_video_mkv->i4_height,
                               pt_video_mkv->e_enc,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_AUDIO_RM:
                {
                    SCDB_AUDIO_RM_T*  pt_audio_rm;

                    pt_audio_rm = & t_scdb_rec.u.t_audio_rm;

                    x_dbg_stmt("%sRM audio (encoding=%d, tag=%p)\n",
                               ps_prefix,
                               pt_audio_rm->e_enc,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_VIDEO_RM:
                {
                    SCDB_VIDEO_RM_T*  pt_video_rm;

                    pt_video_rm = & t_scdb_rec.u.t_video_rm;

                    x_dbg_stmt("%sRM video (%dx%d, encoding=%d, tag=%p)\n",
                               ps_prefix,
                               pt_video_rm->ui2_frm_width,
                               pt_video_rm->ui2_frm_height,
                               pt_video_rm->e_enc,
                               t_stream_comp_id.pv_stream_tag);
                }
                break;

                case SCDB_REC_TYPE_UNKNOWN:
                default:
                    x_dbg_stmt("%sUnknown stream type %d (tag=%p)\n",
                               ps_prefix,
                               t_scdb_rec.e_rec_type,
                               t_stream_comp_id.pv_stream_tag);
                    break;
            }

            ui2_index++;
        }

        x_scdb_close(h_scdb);

        x_dbg_stmt("%s(%d stream%s)\n",
                   ps_prefix, ui2_index, (ui2_index > 1) ? "s" : "");

        if (i4_res == SCDBR_DB_MODIFIED)
        {
            x_dbg_stmt("%sSCDB modified, try again\n", ps_prefix);
        }
    }
#endif
}

#ifdef DEBUG
/*-----------------------------------------------------------------------------
 * Name: cm_show_descriptor
 *
 * Description: This API shows a descriptor as a byte array.
 *
 * Inputs:  pui1_data   References the descriptor data.
 *          z_data_len  Contains the number of bytes to display.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID cm_show_descriptor (const UINT8*  pui1_data,
                                SIZE_T        z_data_len)
{
    UINT32  ui4_i;

    for (ui4_i = 0; ui4_i < z_data_len; ui4_i++)
    {
        x_dbg_stmt("%02x ", pui1_data[ui4_i]);
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_show_iso_639_language_descriptor
 *
 * Description: This API shows an ISO 639 language descriptor.
 *
 * Inputs:  pui1_data   References the descriptor data.
 *          z_data_len  Contains the number of bytes to display.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID cm_show_iso_639_language_descriptor (const UINT8*  pui1_data,
                                                 SIZE_T        z_data_len)
{
    UINT32  ui4_n;
    UINT32  ui4_i;

    x_dbg_stmt("ISO_639_language_descriptor");

    ui4_n = pui1_data[1] / 4;

    pui1_data += 2;

    for (ui4_i = 0; ui4_i < ui4_n; ui4_i++)
    {
        CHAR  s_language[4];

        s_language[0] = pui1_data[0];
        s_language[1] = pui1_data[1];
        s_language[2] = pui1_data[2];
        s_language[3] = '\0';

        x_dbg_stmt(", \"%s\" %02x", s_language, pui1_data[3]);

        pui1_data += 4;
    }
}

/*-----------------------------------------------------------------------------
 * Name: tm_parse_show_descriptors
 *
 * Description: This API is the "show descriptor" descriptor parse function.
 *
 * Inputs:  h_obj           Contains the table handle.
 *          pui1_obj_data   References the descriptor data.
 *          z_obj_data_len  Contains the descriptor length.
 *          ui2_count       Contains the descriptor index.
 *          pv_data         References the prefix to display.
 *          z_data_len      0 for no space after prefix, else space is added.
 *
 * Outputs: -
 *
 * Returns: PARSE_RET_NEXT
 ----------------------------------------------------------------------------*/
static PARSE_RET_T tm_parse_show_descriptors (HANDLE_T      h_obj,
                                              const UINT8*  pui1_obj_data,
                                              SIZE_T        z_obj_data_len,
                                              UINT16        ui2_count,
                                              VOID*         pv_data,
                                              SIZE_T        z_data_len)
{
    const CHAR*  ps_prefix;

    ps_prefix = (const CHAR*) pv_data;

    x_dbg_stmt("%s%s", ps_prefix, (z_data_len > 0) ? " " : "");

    switch (pui1_obj_data[0])
    {
        case 0x0A: /* ISO 639 language descriptor */
        {
            cm_show_iso_639_language_descriptor(pui1_obj_data, z_obj_data_len);
        }
        break;

        default:
        {
            cm_show_descriptor(pui1_obj_data, z_obj_data_len);
        }
        break;
    }

    x_dbg_stmt("\n");

    return PARSE_RET_NEXT;
}

/*-----------------------------------------------------------------------------
 * Name: cm_show_main_body_descriptors
 *
 * Description: This API shows the main body descriptors of a table object.
 *
 * Inputs:  h_obj      Contains the table handle.
 *          ps_prefix  References a prefix string to display. Can be NULL.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID cm_show_main_body_descriptors (HANDLE_T     h_obj,
                                           const CHAR*  ps_prefix)
{
    INT32      i4_res;
    TM_COND_T  e_cond;

    i4_res = x_tm_parse_obj(h_obj,
                            TM_PARSE_TYPE_MAIN_BODY_ALL_DESCRS,
                            NULL, (VOID*) ps_prefix, 0,
                            tm_parse_show_descriptors,
                            & e_cond);

    if (i4_res != TMR_OK)
    {
        x_dbg_stmt("%sCould not show main body's descriptors (%d)\n",
                   ps_prefix, i4_res);
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_show_first_loop_descriptors
 *
 * Description: This API shows the first loop descriptors of a table object.
 *
 * Inputs:  h_obj      Contains the table handle.
 *          ui2_index  Contains the loop index.
 *          ps_prefix  References a prefix string to display. Can be NULL.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID cm_show_first_loop_descriptors (HANDLE_T     h_obj,
                                            UINT16       ui2_index,
                                            const CHAR*  ps_prefix)
{
    INT32      i4_res;
    TM_COND_T  e_cond;

    i4_res = x_tm_parse_obj(h_obj,
                            TM_PARSE_TYPE_FIRST_LOOP_ALL_DESCRS,
                            (VOID*) ((UINT32) ui2_index),
                            (VOID*) ps_prefix, 1,
                            tm_parse_show_descriptors,
                            & e_cond);

    if (i4_res != TMR_OK)
    {
        x_dbg_stmt("%sCould not show main body's descriptors (%d)\n",
                   ps_prefix, i4_res);
    }
}
#endif /* DEBUG */

/*-----------------------------------------------------------------------------
 * Name: cm_show_pat
 *
 * Description: This API shows informations about a PAT.
 *
 * Inputs:  h_pat      Contains the PAT handle.
 *          ps_prefix  References a prefix string to display. Can be NULL.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_show_pat (HANDLE_T     h_pat,
                  const CHAR*  ps_prefix)
{
#ifdef DEBUG
    PSI_PID_SVC_ID_INFO_T  t_service_info;
    UINT16                 ui2_index;

    if (ps_prefix == NULL)
    {
        ps_prefix = "";
    }

    ui2_index = 0;

    while (x_psi_get_svc_entry(h_pat, ui2_index, & t_service_info, NULL)
           ==
           TMR_OK)
    {
        x_dbg_stmt("%sProgram %d (PID %d/0x%x)\n",
                   ps_prefix,
                   t_service_info.ui2_svc_id,
                   t_service_info.t_pid, t_service_info.t_pid);

        ui2_index++;
    }

    if (ui2_index == 0)
    {
        x_dbg_stmt("%sNo program in PAT\n", ps_prefix);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: cm_show_pmt
 *
 * Description: This API shows informations about the all streams in a PMT.
 *
 * Inputs:  h_pmt      Contains the PMT handle.
 *          ps_prefix  References a prefix string to display. Can be NULL.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_show_pmt (HANDLE_T     h_pmt,
                  const CHAR*  ps_prefix)
{
#ifdef DEBUG
    PSI_STREAM_INFO_T  t_stream_info;
    UINT16             ui2_index;

    if (ps_prefix == NULL)
    {
        ps_prefix = "";
    }

    ui2_index = 0;

    cm_show_main_body_descriptors(h_pmt, ps_prefix);

    while (x_psi_get_stream_entry(h_pmt,
                                  ui2_index,
                                  & t_stream_info,
                                  NULL)
           == TMR_OK)
    {
        x_dbg_stmt("%sStream type %d (on PID %d/0x%x)\n",
                   ps_prefix,
                   t_stream_info.ui1_stream_type,
                   t_stream_info.t_pid, t_stream_info.t_pid);

        cm_show_first_loop_descriptors(h_pmt, ui2_index, ps_prefix);

        ui2_index++;
    }

    if (ui2_index == 0)
    {
        x_dbg_stmt("%sNo stream in PMT\n", ps_prefix);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: cm_send_msg_retry
 *
 * Description: This API sends a message of type CM_MSG_T to the specified
 *              message queue. If sending the message fails the API will wait
 *              a few milliseconds and then retry.
 *
 * Inputs:  h_msgq      Contains a handle to the message queue to send the
 *                      message to.
 *          ui4_code    Contains the message code.
 *          ui4_data_1  Contains some additional data.
 *          ui4_data_2  Contains some additional data.
 *          h_obj       Some object handle.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The message was sent (possibly after multiple tries).
 *          CMR_FAIL  The message could not be sent.
 ----------------------------------------------------------------------------*/
INT32 cm_send_msg_retry (HANDLE_T  h_msgq,
                         UINT32    ui4_code,
                         UINT32    ui4_data_1,
                         UINT32    ui4_data_2,
                         HANDLE_T  h_obj)
{
    CM_MSG_T  t_msg;
    UINT32    ui4_count;
    INT32     i4_res;

    t_msg.ui4_code   = ui4_code;
    t_msg.ui4_data_1 = ui4_data_1;
    t_msg.ui4_data_2 = ui4_data_2;
    t_msg.h_obj      = h_obj;

    ui4_count = 0;

    i4_res = CMR_OK;

    while (x_msg_q_send(h_msgq,
                        & t_msg,
                        sizeof(CM_MSG_T),
                        127)
           != OSR_OK)
    {
        ui4_count++;

        if (ui4_count >= 10)
        {
            DBG_ERROR((DBG_PREFIX"Could not send msg after %d tries\n",
                       ui4_count));

            i4_res = CMR_FAIL;

            break;
        }

        x_thread_delay(50);
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_send_msg_no_retry
 *
 * Description: This API sends a message of type CM_MSG_T to the specified
 *              message queue. This can be called from an interrupt. A common
 *              priority is used.
 *
 * Inputs:  h_msgq      Contains a handle to the message queue to send the
 *                      message to.
 *          ui4_code    Contains the message code.
 *          ui4_data_1  Contains some additional data.
 *          ui4_data_2  Contains some additional data.
 *          h_obj       Some object handle.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The message was sent.
 *          CMR_FAIL  The message could not be sent.
 ----------------------------------------------------------------------------*/
INT32 cm_send_msg_no_retry (HANDLE_T  h_msgq,
                            UINT32    ui4_code,
                            UINT32    ui4_data_1,
                            UINT32    ui4_data_2,
                            HANDLE_T  h_obj)
{
    CM_MSG_T  t_msg;
    INT32     i4_res;

    t_msg.ui4_code   = ui4_code;
    t_msg.ui4_data_1 = ui4_data_1;
    t_msg.ui4_data_2 = ui4_data_2;
    t_msg.h_obj      = h_obj;

    i4_res = x_msg_q_send(h_msgq,
                          & t_msg,
                          sizeof(CM_MSG_T),
                          127);

    i4_res = (i4_res == OSR_OK) ? CMR_OK : CMR_FAIL;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_send_msg_no_retry_high
 *
 * Description: This API sends a message of type CM_MSG_T to the specified
 *              message queue. This can be called from an interrupt. A high
 *              priority is used.
 *
 * Inputs:  h_msgq      Contains a handle to the message queue to send the
 *                      message to.
 *          ui4_code    Contains the message code.
 *          ui4_data_1  Contains some additional data.
 *          ui4_data_2  Contains some additional data.
 *          h_obj       Some object handle.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The message was sent.
 *          CMR_FAIL  The message could not be sent.
 ----------------------------------------------------------------------------*/
INT32 cm_send_msg_no_retry_high (HANDLE_T  h_msgq,
                                 UINT32    ui4_code,
                                 UINT32    ui4_data_1,
                                 UINT32    ui4_data_2,
                                 HANDLE_T  h_obj)
{
    CM_MSG_T  t_msg;
    INT32     i4_res;

    t_msg.ui4_code   = ui4_code;
    t_msg.ui4_data_1 = ui4_data_1;
    t_msg.ui4_data_2 = ui4_data_2;
    t_msg.h_obj      = h_obj;

    i4_res = x_msg_q_send(h_msgq,
                          & t_msg,
                          sizeof(CM_MSG_T),
                          1);

    i4_res = (i4_res == OSR_OK) ? CMR_OK : CMR_FAIL;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_drv_color_sys_to_color_sys
 *
 * Description: This API converts a driver (TV decoder) color system to a color
 *              system.
 *
 * Inputs:  e_drv_color_sys  Contains the driver color system.
 *
 * Outputs: -
 *
 * Returns: a color system
 ----------------------------------------------------------------------------*/
COLOR_SYS_T cm_drv_color_sys_to_color_sys (TV_DEC_COLOR_SYS_T  e_drv_color_sys)
{
    COLOR_SYS_T  e_color_sys;

    switch (e_drv_color_sys)
    {
        case TV_DEC_COLOR_SYS_NTSC:
            e_color_sys = COLOR_SYS_NTSC;
            break;

        case TV_DEC_COLOR_SYS_NTSC_443:
            e_color_sys = COLOR_SYS_NTSC_443;
            break;

        case TV_DEC_COLOR_SYS_PAL:
            e_color_sys = COLOR_SYS_PAL;
            break;

        case TV_DEC_COLOR_SYS_PAL_N:
            e_color_sys = COLOR_SYS_PAL_N;
            break;

        case TV_DEC_COLOR_SYS_PAL_M:
            e_color_sys = COLOR_SYS_PAL_M;
            break;

        case TV_DEC_COLOR_SYS_PAL_60:
            e_color_sys = COLOR_SYS_PAL_60;
            break;

        case TV_DEC_COLOR_SYS_SECAM:
            e_color_sys = COLOR_SYS_SECAM;
            break;

        default:
            e_color_sys = COLOR_SYS_UNKNOWN;
    }

    return e_color_sys;
}

/*-----------------------------------------------------------------------------
 * Name: cm_audio_type
 *
 * Description: This API converts an audio type (as defined in the specs) to
 *              its matching enumeration value.
 *
 * Inputs:  ui1_type  Contains the audio type as defined in ISO 13818-1.
 *
 * Outputs: -
 *
 * Returns: matching enumeration value.
 ----------------------------------------------------------------------------*/
AUD_TYPE_T cm_audio_type (UINT8  ui1_type)
{
    AUD_TYPE_T  e_type;

    switch (ui1_type)
    {
        case 0x01:
            e_type = AUD_TYPE_CLEAN;
            break;

        case 0x02:
            e_type = AUD_TYPE_HEARING_IMPAIRED;
            break;

        case 0x03:
            e_type = AUD_TYPE_VISUAL_IMPAIRED;
            break;

        default:
            e_type = AUD_TYPE_UNKNOWN;
            break;
    }

    return e_type;
}

/*-----------------------------------------------------------------------------
 * Name: tm_parse_get_audio_layer
 *
 * Description: This API is the PMT audio stream descriptor parse function. It
 *              retrieves the layer.
 *
 * Inputs:  h_pmt           Contains the PMT handle.
 *          pui1_obj_data   References the descriptor data.
 *          z_obj_data_len  Contains the descriptor length.
 *          ui2_count       Contains the descriptor index.
 *          pv_data         References some data.
 *          z_data_len      Contains the length of the data.
 *
 * Outputs: -
 *
 * Returns: PARSE_RET_END
 ----------------------------------------------------------------------------*/
static PARSE_RET_T tm_parse_get_audio_layer (HANDLE_T      h_pmt,
                                             const UINT8*  pui1_obj_data,
                                             SIZE_T        z_obj_data_len,
                                             UINT16        ui2_count,
                                             VOID*         pv_data,
                                             SIZE_T        z_data_len)
{
    PARSE_RET_T  e_parse_ret = PARSE_RET_NEXT;

    if ((pui1_obj_data[0] == 0x03) && (z_obj_data_len >= 3))
    {
        AUD_LAYER_T  e_layer;

        switch (pui1_obj_data[2] & 0x30)
        {
            case 0x30:
                e_layer = AUD_LAYER_1;
                break;

            case 0x20:
                e_layer = AUD_LAYER_2;
                break;

            case 0x10:
                e_layer = AUD_LAYER_3;
                break;

            default:
                e_layer = AUD_LAYER_UNKNOWN;
                break;
        }

        *((AUD_LAYER_T*) pv_data) = e_layer;

        e_parse_ret = PARSE_RET_END;
    }

    return e_parse_ret;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pmt_get_audio_layer
 *
 * Description: This API retrieves audio layer in a PMT.
 *
 * Inputs:  h_pmt    Contains the PMT handle.
 *          ui2_idx  Contains the loop index in the PMT.
 *
 * Outputs: pe_layer  Contains the layer.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_pmt_get_audio_layer(HANDLE_T      h_pmt,
                            UINT16        ui2_index,
                            AUD_LAYER_T*  pe_layer)
{
    TM_FIRST_LOOP_DESCR_SEL_T  t_descr;
    UINT8                      ui1_descriptor_tag;

    ui1_descriptor_tag = 0x03; /* audio stream descriptor tag */

    t_descr.ui2_loop_idx        = ui2_index;
    t_descr.pui1_descr_tag_list = & ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;

    *pe_layer = AUD_LAYER_UNKNOWN;

    x_tm_parse_obj(h_pmt,
                   TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                   & t_descr,
                   pe_layer,
                   sizeof(AUD_LAYER_T),
                   tm_parse_get_audio_layer,
                   NULL);
}

/*-----------------------------------------------------------------------------
 * Name: tm_parse_get_iso_639_info
 *
 * Description: This API is the PMT ISO 639 language descriptor parse function.
 *              The language code (3+1 characters) and the audio type will be
 *              copied into the provided buffer (passed as pv_data).
 *
 * Inputs:  h_pmt           Contains the PMT handle.
 *          pui1_obj_data   References the descriptor data.
 *          z_obj_data_len  Contains the descriptor length.
 *          ui2_count       Contains the descriptor index.
 *          pv_data         References some data.
 *          z_data_len      Contains the length of the data.
 *
 * Outputs: -
 *
 * Returns: PARSE_RET_NEXT
 ----------------------------------------------------------------------------*/
static PARSE_RET_T tm_parse_get_iso_639_info (HANDLE_T      h_pmt,
                                              const UINT8*  pui1_obj_data,
                                              SIZE_T        z_obj_data_len,
                                              UINT16        ui2_count,
                                              VOID*         pv_data,
                                              SIZE_T        z_data_len)
{
    /* minimum size = 6 bytes (i.e. at least one language in the loop) */

    if ((pui1_obj_data[0] == 0x0A) && (z_obj_data_len >= 6))
    {
        CM_ISO_639_INFO_T*  pt_info;
        UINT32              ui4_nb_entries;

        ui4_nb_entries = ((z_obj_data_len - 2) / 4);

        pt_info = (CM_ISO_639_INFO_T *) pv_data;

        pt_info->ui4_nb_languages += ui4_nb_entries;
    }

    return PARSE_RET_NEXT;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pmt_get_iso_639_info
 *
 * Description: This API retrieves the ISO 639 language information in a PMT.
 *
 * Inputs:  h_pmt    Contains the PMT handle.
 *          ui2_idx  Contains the loop index in the PMT.
 *
 * Outputs: pt_info  Contains the ISO 639 language information.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_pmt_get_iso_639_info(HANDLE_T            h_pmt,
                             UINT16              ui2_index,
                             CM_ISO_639_INFO_T*  pt_info)
{
    TM_FIRST_LOOP_DESCR_SEL_T  t_descr;
    UINT8                      ui1_descriptor_tag;

    ui1_descriptor_tag = 0x0A; /* ISO 639 language descriptor tag */

    t_descr.ui2_loop_idx        = ui2_index;
    t_descr.pui1_descr_tag_list = & ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;

    pt_info->ui4_nb_languages = 0;

    x_tm_parse_obj(h_pmt,
                   TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                   & t_descr,
                   pt_info,
                   sizeof(CM_ISO_639_INFO_T),
                   tm_parse_get_iso_639_info,
                   NULL);
}

/*-----------------------------------------------------------------------------
 * Name: tm_parse_get_component_tag
 *
 * Description: This API is the PMT stream identifier descriptor parse
 *              function. The component tag will be copied into the provided
 *              buffer (passed as pv_data).
 *
 * Inputs:  h_pmt           Contains the PMT handle.
 *          pui1_obj_data   References the descriptor data.
 *          z_obj_data_len  Contains the descriptor length.
 *          ui2_count       Contains the descriptor index.
 *          pv_data         References some data.
 *          z_data_len      Contains the length of the data.
 *
 * Outputs: -
 *
 * Returns: PARSE_RET_NEXT
 ----------------------------------------------------------------------------*/
static PARSE_RET_T tm_parse_get_component_tag (HANDLE_T      h_pmt,
                                               const UINT8*  pui1_obj_data,
                                               SIZE_T        z_obj_data_len,
                                               UINT16        ui2_count,
                                               VOID*         pv_data,
                                               SIZE_T        z_data_len)
{
    /* minimum size = 3 bytes */

    if ((pui1_obj_data[0] == 0x52) && (z_obj_data_len >= 3))
    {
        *((UINT8*) pv_data) = pui1_obj_data[2];
    }

    return PARSE_RET_END;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pmt_get_component_tag
 *
 * Description: This API retrieves the component tag (DVB) in a PMT.
 *
 * Inputs:  h_pmt    Contains the PMT handle.
 *          ui2_idx  Contains the loop index in the PMT.
 *
 * Outputs: pui1_component_tag  Contains the component tag.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 cm_pmt_get_component_tag (HANDLE_T  h_pmt,
                                UINT16    ui2_index,
                                UINT8*    pui1_component_tag)
{
    TM_FIRST_LOOP_DESCR_SEL_T  t_descr;
    INT32                      i4_res;
    UINT8                      ui1_descriptor_tag;

    ui1_descriptor_tag = 0x52; /* stream identifier descriptor tag */

    t_descr.ui2_loop_idx        = ui2_index;
    t_descr.pui1_descr_tag_list = & ui1_descriptor_tag;
    t_descr.z_num_descr_tags    = 1;

    i4_res = x_tm_parse_obj(h_pmt,
                            TM_PARSE_TYPE_FIRST_LOOP_DESCRS,
                            & t_descr,
                            pui1_component_tag,
                            sizeof(UINT8),
                            tm_parse_get_component_tag,
                            NULL);

    i4_res = (i4_res == TMR_OK) ? CMR_OK : CMR_FAIL;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_are_streams_equal
 *
 * Description: This API determines if a stream (defined by STREAM_COMP_ID_T
 *              and SCDB_REC_T) equals another stream (defined by a PID, a
 *              stream type and/or a descriptor in the table).
 *
 * Inputs:  h_pmt            Contains the PMT handle.
 *          pt_stream_comp   References the stream.
 *          pt_scdb_rec      References the SCDB record (that contains the PID
 *                           of the stream).
 *          ui2_index        Contains the index in the PMT loop.
 *          ui2_pid          Contains the PID.
 *          ui1_stream_type  Contains the stream type.
 *
 * Outputs: -
 *
 * Returns: TRUE or FALSE.
 ----------------------------------------------------------------------------*/
BOOL cm_are_streams_equal (HANDLE_T                 h_pmt,
                           const STREAM_COMP_ID_T*  pt_stream_comp,
                           const SCDB_REC_T*        pt_scdb_rec,
                           UINT16                   ui2_index,
                           UINT16                   ui2_pid,
                           UINT8                    ui1_stream_type,
                           UINT32                   ui4_flags)
{
    BOOL  b_equal=FALSE;

    switch (ui1_stream_type)
    {
        case STREAM_TYPE_AUDIO_11172_3:
        case STREAM_TYPE_AUDIO_13818_3:
        case STREAM_TYPE_AUDIO_14496_3:
        case STREAM_TYPE_AUDIO_ATSC_A53:
        {
            b_equal =
                (pt_stream_comp->e_type == ST_AUDIO)
                &&
                (ui2_pid == pt_scdb_rec->u.t_audio_mpeg.ui2_pid);
#if 0
                /*stream type 0x81 is used for ac3 audio stream in dvb nowadays, 
                  if we mark dvb , it's will lead to frequently scdb update in some test ts */
                &&
                ((ui1_stream_type != STREAM_TYPE_AUDIO_ATSC_A53)
                 ||
                 CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags));
#endif
        }
        break;

        case STREAM_TYPE_VIDEO_11172_2:
        case STREAM_TYPE_VIDEO_13818_2:
        case STREAM_TYPE_VIDEO_14496_2:
        case STREAM_TYPE_VIDEO_14496_10:
        case STREAM_TYPE_VIDEO_DIGICIPHER_2:
        case STREAM_TYPE_VIDEO_SMPTE_421M:
        case STREAM_TYPE_VIDEO_MVC:
        {
            b_equal =
                ((pt_stream_comp->e_type == ST_VIDEO)
                 &&
                 (ui2_pid == pt_scdb_rec->u.t_video_mpeg.ui2_pid)
                 &&
                 ((ui1_stream_type != STREAM_TYPE_VIDEO_DIGICIPHER_2)
                  ||
                  CM_IS_ATSC_SCTE_UNKNOWN(ui4_flags)))
                ||
                ((pt_stream_comp->e_type == ST_TELETEXT)
                 &&
                 (CM_IS_DVB(ui4_flags))
                 &&
                 (cm_pmt_entry_has_teletext_stream(h_pmt,
                                                   ui2_index,
                                                   ui2_pid,
                                                   pt_scdb_rec)));
        }
        break;

        case STREAM_TYPE_PES_PRIVATE_13818_1:
        {
            b_equal =
                ((CM_IS_DVB(ui4_flags))
                 &&
                 (((pt_stream_comp->e_type == ST_SUBTITLE)
                   &&
                   (cm_pmt_entry_has_dvbst_stream(h_pmt,
                                                  ui2_index,
                                                  ui2_pid,
                                                  pt_scdb_rec)))
                  ||
                  ((pt_stream_comp->e_type == ST_TELETEXT)
                   &&
                   (cm_pmt_entry_has_teletext_stream(h_pmt,
                                                     ui2_index,
                                                     ui2_pid,
                                                     pt_scdb_rec)))
                  ||
                  ((pt_stream_comp->e_type == ST_DATA)
                   &&
                   (cm_pmt_entry_has_vbi_data_stream(h_pmt,
                                                     ui2_index,
                                                     ui2_pid,
                                                     pt_scdb_rec)))
                  ||
                  ((pt_stream_comp->e_type == ST_AUDIO)
                   &&
                   (cm_pmt_entry_has_ac3_dts_aac_stream(h_pmt,
                                                        ui2_index,
                                                        ui2_pid,
                                                        pt_scdb_rec)))));
        }
        break;

        case STREAM_TYPE_PRIVATE_SECTIONS_13818_1:
        {
            b_equal =
                (pt_stream_comp->e_type == ST_DATA)
                &&
                (pt_scdb_rec->e_rec_type == SCDB_REC_TYPE_PRIVATE_MPEG)
                &&
                (ui2_pid == pt_scdb_rec->u.t_private_mpeg.ui2_pid);
        }
        break;

        default:
        {
            if (CM_IS_DVB(ui4_flags))
            {
                b_equal =
                    (pt_stream_comp->e_type == ST_AUDIO)
                    &&
                    (cm_pmt_entry_has_ac3_dts_aac_stream(h_pmt,
                                                         ui2_index,
                                                         ui2_pid,
                                                         pt_scdb_rec));
            }
            else
            {
                b_equal = FALSE;
            }
        }
        break;
    }

    return b_equal;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pmt_has_stream
 *
 * Description: This API determines whether a PMT contains a specific stream.
 *              The stream is defined by a STREAM_COMP_ID_T structure and a
 *              SCDB_REC_T structure.
 *
 * Inputs:  h_pmt           Contains the PMT handle.
 *          pt_stream_comp  References the stream.
 *          pt_scdb_rec     References the SCDB record (that contains the PID
 *                          of the stream).
 *
 * Outputs: -
 *
 * Returns: TRUE or FALSE.
 ----------------------------------------------------------------------------*/
BOOL cm_pmt_has_stream (HANDLE_T                 h_pmt,
                        const STREAM_COMP_ID_T*  pt_stream_comp,
                        const SCDB_REC_T*        pt_scdb_rec,
                        UINT32                   ui4_flags)
{
    PSI_STREAM_INFO_T  t_stream_info;
    TM_COND_T          e_stream_cond;
    UINT16             ui2_index;
    BOOL               b_found;

    b_found   = FALSE;
    ui2_index = 0;

    while ((! b_found)
           &&
           (x_psi_get_stream_entry(h_pmt,
                                   ui2_index,
                                   & t_stream_info,
                                   & e_stream_cond) == TMR_OK))
    {
        b_found = cm_are_streams_equal(h_pmt,
                                       pt_stream_comp,
                                       pt_scdb_rec,
                                       ui2_index,
                                       t_stream_info.t_pid,
                                       t_stream_info.ui1_stream_type,
                                       ui4_flags);

        if (b_found && (pt_stream_comp->e_type == ST_AUDIO))
        {
            AUD_FMT_T  e_audio_format;

            /*
              in that case, we check if the PMT stream is also DUAL_MONO. If it
              is not, then we set b_found to FALSE if the index is greater than
              0 (i.e. we only keep the very first SCDB record with the same PID
              and disregard all the other ones).
            */

            if (ui4_flags & CM_FLAG_ATSC_SCTE)
            {
                e_audio_format = AUD_FMT_TYPE_5_1;
            }
            else if (ui4_flags & CM_FLAG_DVB)
            {
                CM_ISO_639_INFO_T  t_iso_639_info;

                cm_pmt_get_iso_639_info(h_pmt, ui2_index, & t_iso_639_info);

                if (t_iso_639_info.ui4_nb_languages == 2)
                {
                    e_audio_format = AUD_FMT_DUAL_MONO;
                }
                else
                {
                    e_audio_format = AUD_FMT_UNKNOWN;
                }
            }
            else
            {
                e_audio_format = AUD_FMT_UNKNOWN;
            }

            if (e_audio_format != AUD_FMT_DUAL_MONO)
            {
                b_found = (pt_scdb_rec->u.t_audio_mpeg.ui1_index == 0);
            }
        }

        ui2_index++;
    }

    return b_found;
}

/*-----------------------------------------------------------------------------
 * Name: cm_query_audio_decoders_formats
 *
 * Description: This API query audio decoder format by getting an static value
 *              that already query during booting
 *
 * Inputs:  -
 *
 * Outputs:
 *
 * Returns: bitmask of all the supported formats
 ----------------------------------------------------------------------------*/
UINT32
cm_query_audio_decoders_formats (VOID)
{
    return g_ui4_audio_formats;
}

/*-----------------------------------------------------------------------------
 * Name: cm_set_audio_format
 *
 * Description: This API changes the audio format of a specific stream (defined
 *              by its PID). For example, this is needed when two dual mono
 *              streams happen to be one stereo stream.
 *
 * Inputs:  ui2_pid         Contains the PID of the stream.
 *          ui1_index       Specifies the stream index.
 *          e_audio_format  Contains the new audio format.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_set_audio_format (UINT16                ui2_pid,
                          UINT8                 ui1_index,
                          AUD_FMT_T             e_audio_format,
                          HANDLE_T              h_scdb,
                          BOOL                  b_dual_mono_fake_stereo,
                          BOOL                  b_dual_mono_mixed_stereo,
                          cm_get_stream_tag_fct pf_get_tag)
{
    STREAM_COMP_ID_T  t_stream_comp_id;
    SCDB_REC_T        t_scdb_rec;
    UINT32            ui4_ver_id;
    INT32             i4_res;
    UINT16            ui2_index;

    DBG_INFO((DBG_PREFIX"Set audio format (pid %u, index %u, format %u)\n",
              ui2_pid, ui1_index, e_audio_format));

    i4_res = scdb_lock(h_scdb);

    if (i4_res == SCDBR_OK)
    {
        ui4_ver_id = SCDB_NULL_VER_ID;
        ui2_index  = 0;

        switch (e_audio_format)
        {
            case AUD_FMT_DUAL_MONO:
            {
                #ifndef MW_CM_DISABLE_HANDLING_DRIVER_REPORTED_DUAL_MONO
                while (x_scdb_get_rec_by_idx(h_scdb,
                                             ST_AUDIO,
                                             ui2_index,
                                             & t_stream_comp_id,
                                             & t_scdb_rec,
                                             & ui4_ver_id) == SCDBR_OK)
                {
                    ui4_ver_id = SCDB_NULL_VER_ID;
                    ui2_index++;

                    if (t_scdb_rec.u.t_audio_mpeg.ui2_pid == ui2_pid)
                    {
                        SCDB_AUDIO_MPEG_T*  pt_audio;

                        pt_audio = & t_scdb_rec.u.t_audio_mpeg;

                        if (pt_audio->e_aud_fmt != AUD_FMT_DUAL_MONO)
                        {
                            /*
                              From non-DUAL_MONO to DUAL_MONO: the stream is
                              updated to become DUAL_MONO (index 0) and a new
                              DUAL_MONO stream is also added (index 1).
                            */

                            pt_audio->ui1_index = AUDIO_INDEX_DUAL_0;
                            pt_audio->e_aud_fmt = AUD_FMT_DUAL_MONO;

                            scdb_update_rec(h_scdb,
                                            & t_stream_comp_id, & t_scdb_rec);

                            pt_audio->ui1_index = AUDIO_INDEX_DUAL_1;

                            t_stream_comp_id.pv_stream_tag = pf_get_tag();

                            scdb_add_rec(h_scdb,
                                         & t_stream_comp_id, & t_scdb_rec);

                            DBG_INFO((DBG_PREFIX
                                      "Splitted audio stream in 2 (tag=%p)\n",
                                      t_stream_comp_id.pv_stream_tag));

                            /* Check if we enable fake stereo record flag
                             * Then we would add one record for L+R with no language codes
                             * No language code is used because L and R may have different languages
                             */
                            if (b_dual_mono_fake_stereo)
                            {
                                pt_audio->ui1_index = AUDIO_INDEX_DUAL_MONO_FAKE_STEREO;
                                pt_audio->s_lang[0] = '\0';
                                pt_audio->s_lang[1] = '\0';
                                pt_audio->s_lang[2] = '\0';
                                pt_audio->s_lang[3] = '\0';

                                t_stream_comp_id.pv_stream_tag = pf_get_tag();

                                scdb_add_rec(h_scdb,
                                             & t_stream_comp_id, & t_scdb_rec);

                                DBG_INFO((DBG_PREFIX
                                          "Fake audio stream in 3 (tag=%p)\n",
                                          t_stream_comp_id.pv_stream_tag));
                            }

                            /* Check if we enable mixed stereo record flag
                             * Then we would add one record for L+R with no language codes
                             * No language code is used because L and R may have different languages
                             */
                            if (b_dual_mono_mixed_stereo)
                            {
                                pt_audio->ui1_index = AUDIO_INDEX_DUAL_MONO_MIXED_MIXING;
                                pt_audio->s_lang[0] = '\0';
                                pt_audio->s_lang[1] = '\0';
                                pt_audio->s_lang[2] = '\0';
                                pt_audio->s_lang[3] = '\0';

                                t_stream_comp_id.pv_stream_tag = pf_get_tag();

                                scdb_add_rec(h_scdb,
                                             & t_stream_comp_id, & t_scdb_rec);

                                DBG_INFO((DBG_PREFIX
                                          "Mixed stereo audio stream in 4 (tag=%p)\n",
                                          t_stream_comp_id.pv_stream_tag));
                            }
                        }

                        break;
                    }
                }
                #endif /* #ifndef MW_CM_DISABLE_HANDLING_DRIVER_REPORTED_DUAL_MONO */
            }
            break;

            default:
            {
                while (x_scdb_get_rec_by_idx(h_scdb,
                                             ST_AUDIO,
                                             ui2_index,
                                             & t_stream_comp_id,
                                             & t_scdb_rec,
                                             & ui4_ver_id) == SCDBR_OK)
                {
                    ui4_ver_id = SCDB_NULL_VER_ID;
                    ui2_index++;

                    if (t_scdb_rec.u.t_audio_mpeg.ui2_pid == ui2_pid)
                    {
                        SCDB_AUDIO_MPEG_T*  pt_audio;

                        pt_audio = & t_scdb_rec.u.t_audio_mpeg;

                        if (pt_audio->e_aud_fmt == AUD_FMT_DUAL_MONO)
                        {
                            if (pt_audio->ui1_index == ui1_index)
                            {
                                /*
                                  From DUAL_MONO to non-DUAL_MONO: the stream
                                  is simply updated if it has the index of the
                                  stream to keep.
                                */

                                DBG_INFO((DBG_PREFIX
                                          "Change audio format: %u (tag=%p)\n",
                                          e_audio_format,
                                          t_stream_comp_id.pv_stream_tag));

                                pt_audio->e_aud_fmt = e_audio_format;

                                scdb_update_rec(h_scdb,
                                                & t_stream_comp_id,
                                                & t_scdb_rec);
                            }
                            else
                            {
                                /*
                                  From DUAL_MONO to non-DUAL_MONO: the stream
                                  is deleted if it does not have the index of
                                  the stream to keep.
                                */

                                DBG_INFO((DBG_PREFIX"Delete tag %p\n",
                                          t_stream_comp_id.pv_stream_tag));

                                scdb_del_rec(h_scdb,
                                             & t_stream_comp_id);

                                ui2_index = 0;
                            }
                        }
                        else
                        {
                            /*
                              From non-DUAL_MONO to non-DUAL_MONO: the stream
                              is simply updated with the new audio format and
                              we can stop right away.
                            */

                            pt_audio->e_aud_fmt = e_audio_format;

                            scdb_update_rec(h_scdb,
                                            & t_stream_comp_id, & t_scdb_rec);

                            break;
                        }
                    }
                }
            }
            break;
        }

        scdb_unlock(h_scdb);
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_audio_dec_rm_nfy
 *
 * Description: This API is called by the Resource Manager to notify the
 *              handler of changes in the state or condition of the audio
 *              decoder.
 *
 * Inputs:  h_comp      Contains a handle to the driver component.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the tag value specified in rm_open_comp.
 *          ui4_data    Contains additional data (depends on e_nfy_cond).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID cm_audio_dec_rm_nfy (HANDLE_T   h_comp,
                                 UINT8      ui1_port,
                                 RM_COND_T  e_nfy_cond,
                                 VOID*      pv_tag,
                                 UINT32     ui4_data)
{
    /* nothing */
}

/*-----------------------------------------------------------------------------
 * Name: cm_get_audio_decoders_formats
 *
 * Description: This API goes through the list of registered audio decoders and
 *              finds all the supported audio formats.
 *
 * Inputs:  -
 *
 * Outputs:
 *
 * Returns: bitmask of all the supported formats
 ----------------------------------------------------------------------------*/
static UINT32 cm_get_audio_decoders_formats (VOID)
{
    UINT32  ui4_formats;
    INT32   i4_res;
    UINT16  ui2_index;
    UINT16  ui2_id;

    ui4_formats = 0;
    ui2_index   = 0;

    while ((i4_res = rm_get_comp_info(DRVT_AUD_DEC,
                                      ui2_index,
                                      & ui2_id,
                                      NULL, NULL, NULL)) == RMR_OK)
    {
        HANDLE_T   h_rm_aud_dec;
        RM_COND_T  e_rm_cond;

        i4_res = rm_open_comp(NULL_HANDLE,
                              OBJ_COMP_REL_IGNORE,
                              DRVT_AUD_DEC,
                              ui2_id,
                              NULL,
                              CTRL_TYPE_MONITOR,
                              NULL, /* pv_comp_sel_data */
                              NULL, /* pf_comp_sel */
                              NULL, /* pv_tag */
                              cm_audio_dec_rm_nfy,
                              & h_rm_aud_dec,
                              & e_rm_cond);

        if (i4_res == RMR_OK)
        {
            AUD_DEC_CAPABILITY_INFO_T  t_aud_dec_cap;
            SIZE_T                     z_size;

            z_size = sizeof(AUD_DEC_CAPABILITY_INFO_T);

            i4_res = rm_get(h_rm_aud_dec,
                            FALSE,
                            ANY_PORT_NUM,
                            AUD_DEC_GET_TYPE_CAPABILITY,
                            & t_aud_dec_cap,
                            & z_size,
                            & e_rm_cond);

            if (i4_res == RMR_OK)
            {
                DBG_INFO((DBG_PREFIX
                          "Audio decoder (id %u) supports formats 0x%08x\n",
                          ui2_id, t_aud_dec_cap.ui4_fmt));

                ui4_formats |= t_aud_dec_cap.ui4_fmt;
            }
            else
            {
                DBG_ERROR((DBG_PREFIX
                           "Could not get capabilities of audio decoder %u"
                           " (res=%d)\n",
                           ui2_id, i4_res));
            }

            rm_close(h_rm_aud_dec, FALSE, NULL);
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not open audio decoder %u (res=%d)\n",
                       ui2_id, i4_res));
        }

        ui2_index++;
    }

    return ui4_formats;
}

#ifdef MW_DVBS_DEV_ENABLE
static VOID cm_tuner_rm_nfy (HANDLE_T   h_comp,
                                 UINT8      ui1_port,
                                 RM_COND_T  e_nfy_cond,
                                 VOID*      pv_tag,
                                 UINT32     ui4_data)
{
    /* nothing */
}

static INT32 cm_reset_sat_micro_controller (VOID)
{
    INT32   i4_res;
    UINT16  ui2_index;
    UINT16  ui2_id;

    ui2_index   = 0;

    while ((i4_res = rm_get_comp_info(DRVT_TUNER_SAT_DIG,
                                      ui2_index,
                                      & ui2_id,
                                      NULL, NULL, NULL)) == RMR_OK)
    {
        HANDLE_T   h_rm_sat_tuner;
        RM_COND_T  e_rm_cond;

        i4_res = rm_open_comp(NULL_HANDLE,
                              OBJ_COMP_REL_IGNORE,
                              DRVT_TUNER_SAT_DIG,
                              ui2_id,
                              NULL,
                              CTRL_TYPE_MONITOR,
                              NULL, /* pv_comp_sel_data */
                              NULL, /* pf_comp_sel */
                              NULL, /* pv_tag */
                              cm_tuner_rm_nfy,
                              & h_rm_sat_tuner,
                              & e_rm_cond);

        if (i4_res == RMR_OK)
        {
            DISEQC_BUS_CMD_T diseqc_cmd;

            x_memset(&diseqc_cmd, 0, sizeof(DISEQC_BUS_CMD_T));

            diseqc_cmd.aui1_cmd[0] = DISEQC_FRAMING_RPT_TRANSMIT_NO_REPLY; // Question: repeat? replay?
            diseqc_cmd.aui1_cmd[1] = DISEQC_ADDR_ANY_LNB_SWITCHER_SMATV;
            diseqc_cmd.aui1_cmd[2] = DISEQC_10_CMD_RESET_MICROCONTROLLER;
            diseqc_cmd.ui1_cmd_len = 3;
            diseqc_cmd.ui1_control_mask = DISEQC_CMD_CTRL_FULL_CMD_MASK;

            i4_res = rm_set(h_rm_sat_tuner,
                            FALSE,
                            ANY_PORT_NUM,
                            TUNER_SET_TYPE_DISEQC_CMD,
                            (VOID*) DISEQC_10_CMD_RESET_MICROCONTROLLER,
                            sizeof(AUD_DEC_CTRL_T),
                            NULL);

            if(i4_res != RMR_OK)
            {
                // To do;
            }
            else
            {
                DBG_ERROR((DBG_PREFIX"Reset microcontroller fail  (res=%d)\n", i4_res));
                return CMR_FAIL;
            }

            rm_close(h_rm_sat_tuner, FALSE, NULL);
        }
        else
        {
            DBG_ERROR((DBG_PREFIX"Could not found satellite tuner (res=%d)\n", i4_res));
            return CMR_NO_COMP_FOUND;
        }

        ui2_index++;
    }

    return CMR_OK;
}

BOOL cm_microctrler_is_enable(VOID)
{
    return g_b_is_microcontroller_reset;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: cm_util_init
 *
 * Description: This API initialize shared utility functions
 *
 * Inputs:  -
 *
 * Outputs:
 *
 * Returns: bitmask of all the supported formats
 ----------------------------------------------------------------------------*/
VOID
cm_util_init (VOID)
{
    /* get audio decoder capacity */
    g_ui4_audio_formats = cm_get_audio_decoders_formats();
#ifdef MW_DVBS_DEV_ENABLE
    {
        INT32 i4_res = CMR_FAIL;

        i4_res = cm_reset_sat_micro_controller();
        if(CMR_OK != i4_res)
        {
            if(CMR_NO_COMP_FOUND == i4_res)
            {
                x_dbg_stmt("!!! [CONN_MNGR] Error: DiSEqC micro controller not found!\n");
                g_b_is_microcontroller_reset = FALSE;
            }
            else
            {
                x_dbg_stmt("!!! [CONN_MNGR] Error: reset DiSEqC micro controller fail!\n");
                DBG_ASSERT(0, DBG_ABRT_CANNOT_GET_HANDLE_OBJ);
            }

            return;
        }
        g_b_is_microcontroller_reset = TRUE;
    }
#endif

}

