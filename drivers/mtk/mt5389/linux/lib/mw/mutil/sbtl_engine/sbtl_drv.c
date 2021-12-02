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
 * $RCSfile: sbtl_drv.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 *         
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
    
#include "res_mngr/drv/x_stc_ctrl.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/drv/x_demux.h" 
#include "mutil/sbtl_engine/u_mm_sbtl_engine.h"
#include "sbtl_drv.h"

#else /*__KERNEL__*/

#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"

#include "x_demux.h"
#include "u_mm_sbtl_engine.h"
#include "sbtl_drv.h"
#endif /*__KERNEL__*/


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
#ifndef __KERNEL__
static VOID _sbtl_rm_nfy_fct(HANDLE_T       h_obj,
                             UINT8          ui1_port,
                             RM_COND_T      e_nfy_cond,
                             VOID*          pv_tag,
                             UINT32         ui4_data)
{
    /*no use*/
    return;
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
#ifndef __KERNEL__
static VOID _sbtl_dmx_nfy_fct(HANDLE_T      h_handle,
                              UINT8         ui1_port,
                              RM_COND_T     e_nfy_cond,
                              VOID*         pv_nfy_tag,
                              UINT32        ui4_data)
{
}
#endif

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
INT32 sbtl_stc_open_drv(HANDLE_T*       ph_stc, 
                        CHAR*           pc_stc_grp_name)
{
#ifndef __KERNEL__
    INT32           i4_ret;
    RM_COND_T       t_cond;
    
    i4_ret = rm_open_comp(NULL_HANDLE,
                          OBJ_COMP_REL_IGNORE,
                          DRVT_STC_CTRL,
                          ANY_COMP_ID,
#ifdef MW_SUPPORT_2STC
                          pc_stc_grp_name,
#else
                          "snk_MainDisp",
#endif
                          CTRL_TYPE_SHARED,
                          NULL,
                          NULL,
                          NULL,
                          _sbtl_rm_nfy_fct,
                          ph_stc,
                          &t_cond);
    if (i4_ret != RMR_OK)
    {
        return MMSBTLR_DEVICE_ERROR;
    }

    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/

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
INT32 sbtl_stc_close_drv(HANDLE_T       h_stc)
{
#ifndef __KERNEL__

    RM_COND_T       t_cond;
    
    rm_close(h_stc, FALSE, &t_cond);

    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/

}

INT32 sbtl_stc_pts_drv(HANDLE_T         h_stc,
                       PTS_T*           pt_pts)
{
#ifndef __KERNEL__
    INT32           i4_ret;
    RM_COND_T       t_cond;
    SIZE_T          z_size;

    *pt_pts = 0;
    z_size = 4;
    i4_ret = rm_get(h_stc,
                    FALSE,
                    0,
                    STC_CTRL_GET_TYPE_STC,
                    (VOID *)pt_pts,
                    &z_size,
                    &t_cond);
    
    if (i4_ret != RMR_OK)
    {
        return MMSBTLR_DEVICE_ERROR;
    }

    return MMSBTLR_OK;
#else
    return MMSBTLR_OK;
#endif
}


INT32 sbtl_stc_type_drv(HANDLE_T         h_stc,
                           STC_CTRL_TYPE_T* pt_type)
{
#ifndef __KERNEL__
    INT32           i4_ret;
    RM_COND_T       t_cond;
    SIZE_T          z_size;

    z_size = 1;
    i4_ret = rm_get(h_stc,
                    FALSE,
                    0,
                    STC_CTRL_GET_TYPE_CTRL,
                    (VOID *)pt_type,
                    &z_size,
                    &t_cond);
    
    if (i4_ret != RMR_OK)
    {
        return MMSBTLR_DEVICE_ERROR;
    }

    return MMSBTLR_OK;
#else
    return MMSBTLR_OK;
#endif
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
INT32 sbtl_dmx_open_drv(HANDLE_T            h_source,
                        HANDLE_T*           ph_dmx,
                        SBTL_DMX_PES_FMT_T  e_pes_fmt)
{
#ifndef __KERNEL__
    HANDLE_T                    h_dmx;
    INT32                       i4_ret;
    RM_COND_T                   e_cond;
    DRV_COMP_ID_T               t_source_id;
    DRV_TYPE_T                  e_source_drv_type;
    UINT32                      ui4_source_comp_flags;
    UINT8                       ui1_source_num_inp_ports;
    UINT8                       ui1_source_num_out_ports;
    UINT16                      ui2_source_comp_id;

    *ph_dmx = NULL_HANDLE;

    /* Get information of handle */
    i4_ret = rm_get_comp_info_from_handle(h_source,
                                          &e_source_drv_type,
                                          &ui2_source_comp_id,
                                          &ui1_source_num_inp_ports,
                                          &ui1_source_num_out_ports,
                                          &ui4_source_comp_flags);
    
    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    /* Open demux resource */
    i4_ret = rm_open_comp(h_source, /* source handle, buffer agent handle */
                          OBJ_COMP_REL_IGNORE,
                          DRVT_DEMUX_CONTAINER_ES_MEMORY,
                          ANY_COMP_ID,
                          NULL,
                          CTRL_TYPE_SHARED,
                          NULL,
                          NULL,
                          NULL,
                          _sbtl_dmx_nfy_fct,
                          &h_dmx,
                          &e_cond);

    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    *ph_dmx = h_dmx;
    
    if (RM_COND_OPENED != (e_cond & RM_COND_OPENED))
    {
        return MMSBTLR_DMX_FAIL;
    }

    /* connect  */
    t_source_id.pv_tag = (VOID*)e_pes_fmt;
    t_source_id.e_type = e_source_drv_type;
    t_source_id.ui2_id = ui2_source_comp_id;
    t_source_id.b_sel_out_port = TRUE;
    t_source_id.u.ui1_out_port = 0;
    i4_ret = rm_connect (h_dmx,
                         RM_CONN_TYPE_COMP_ID,
                         (VOID *)&t_source_id,
                         sizeof(DRV_COMP_ID_T),
                         NULL,
                         &e_cond);

    if (i4_ret != RMR_OK)
    {
        return MMSBTLR_DMX_FAIL;
    }
    
    if (RM_COND_CONNECTED != (e_cond & RM_COND_CONNECTED))
    {
        return MMSBTLR_DMX_FAIL;
    }
    
    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/

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
INT32 sbtl_dmx_close_drv(HANDLE_T        h_dmx)
{
#ifndef __KERNEL__
    INT32       i4_ret;
    RM_COND_T   e_cond;

    /* Disconnect */
    i4_ret = rm_disconnect (h_dmx,
                            RM_DISC_TYPE_IGNORE,
                            (VOID*)NULL,
                            sizeof(VOID*),
                            &e_cond);
    
    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    /* Close */
    i4_ret = rm_close (h_dmx, 
                       FALSE, 
                       &e_cond);

    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }
    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/
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
INT32 sbtl_dmx_enable_drv(HANDLE_T          h_dmx,
                          UINT32            ui4_stream_id,
                          x_demux_nfy_fct   pf_nfy,
                          VOID*             pv_tag)
{
#ifndef __KERNEL__
    INT32               i4_ret;
    DEMUX_NFY_INFO_T    t_nfy_info_pes;
    RM_COND_T           e_cond;

    /* set DEMUX_SET_TYPE_PID parameter */
    i4_ret = rm_set(h_dmx,
                    FALSE, /* b_sel_out_port */
                    0, /* ui1_port, */
                    DEMUX_SET_TYPE_STREAM_ID,
                    (VOID *) ui4_stream_id,
                    sizeof (STREAM_ID_T),
                    &e_cond);
    
    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    /* set DEMUX_SET_TYPE_NFY_FCT parameter */
    t_nfy_info_pes.pv_tag = pv_tag;
    t_nfy_info_pes.pf_demux_nfy = pf_nfy;
    i4_ret = rm_set(h_dmx,
                    FALSE, /* b_sel_out_port */
                    0, /* ui1_port, */
                    DEMUX_SET_TYPE_NFY_FCT,
                    (VOID *) &t_nfy_info_pes,
                    sizeof (DEMUX_NFY_INFO_T),
                    &e_cond);

    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    /* set DEMUX_SET_TYPE_ALLOC_BUFFER parameter */
    i4_ret = rm_set(h_dmx,
                    FALSE,                          /* b_sel_out_port */
                    0,                              /* ui1_port, */
                    DEMUX_SET_TYPE_ALLOC_BUFFER,
                    (VOID *) (1024 * 64),
                    sizeof(SIZE_T),
                    &e_cond);

    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    /* set ctrl enable */
    i4_ret = rm_set(h_dmx,
                    FALSE, /* b_sel_out_port */
                    0, /* ui1_port, */
                    DEMUX_SET_TYPE_CTRL,
                    (VOID *) (TRUE),
                    sizeof(VOID *),
                    &e_cond);

    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/

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
INT32 sbtl_dmx_disable_drv(HANDLE_T          h_dmx)
{
#ifndef __KERNEL__
    INT32               i4_ret;
    RM_COND_T           e_cond;
    
    /* set ctrl disable */
    i4_ret = rm_set(h_dmx,
                    FALSE,                  /* b_sel_out_port */
                    0,                      /* ui1_port, */
                    DEMUX_SET_TYPE_CTRL,
                    (VOID *) (FALSE),
                    sizeof(VOID *),
                    &e_cond);

    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }

    /* set DEMUX_SET_TYPE_FREE_BUFFER parameter */
    i4_ret = rm_set(h_dmx,
                    FALSE,                  /* b_sel_out_port */
                    0,                      /* ui1_port, */
                    DEMUX_SET_TYPE_FREE_BUFFER,
                    (VOID *) (FALSE),
                    sizeof(VOID *),
                    &e_cond);

    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DMX_FAIL;
    }
    
    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/

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
INT32 sbtl_dmx_retrive_data_drv(HANDLE_T        h_dmx,
                                UINT32          ui4_tag,
                                UINT32          ui4_length,
                                UINT8*          aui1_buf)
{
#ifndef __KERNEL__
    DEMUX_READ_BUFFER_INFO_T    t_read_buf;
    SIZE_T                      z_size;
    INT32                       i4_ret;
    RM_COND_T                   e_cond;
    
    t_read_buf.pui1_dest = aui1_buf; 
    t_read_buf.pv_src_tag = (VOID *)ui4_tag;
    t_read_buf.z_skip_len = 0;
    t_read_buf.z_copy_len = ui4_length;

    z_size = sizeof (DEMUX_READ_BUFFER_INFO_T);

    i4_ret = rm_get(h_dmx,
                    FALSE,
                    0,
                    DEMUX_GET_TYPE_READ_BUFFER,
                    (VOID *)&t_read_buf,
                    &z_size,
                    &e_cond);
    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DEVICE_ERROR;
    }

    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/

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
INT32 sbtl_dmx_release_data_drv(HANDLE_T        h_dmx,
                                UINT32          ui4_tag,
                                UINT32          ui4_length)
{
#ifndef __KERNEL__
    DEMUX_UNLOCK_BUFFER_INFO_T  t_unlock_buf;
    SIZE_T                      z_size;
    INT32                       i4_ret;
    RM_COND_T                   e_cond;

    t_unlock_buf.pv_src_tag = (VOID *)ui4_tag;
    t_unlock_buf.z_src_len = ui4_length;
    z_size = sizeof (DEMUX_UNLOCK_BUFFER_INFO_T);
    
    i4_ret = rm_set(h_dmx,
                    FALSE,
                    0,
                    DEMUX_SET_TYPE_UNLOCK_BUFFER,
                    (VOID *)&t_unlock_buf,
                    z_size,
                    &e_cond);
    
    if (RMR_OK != i4_ret)
    {
        return MMSBTLR_DEVICE_ERROR;
    }
    return MMSBTLR_OK;
#else /*__KERNEL__*/
    return MMSBTLR_OK;
#endif /*__KERNEL__*/

}



