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
 *
 * Description:
 *      Middleware driver - Bufer Agent
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_cli.h"

#include "x_rm_dev_types.h"
#include "x_dbg.h"
#include "x_cli.h"
#include "drv_comp_id.h"
#include "bagt_mw.h"
#include "block_mw.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
typedef struct _BUF_AGENT_INT_DATA_T
{
    buf_agt_mw_nfy_fct          pf_drv_nfy;
    buf_agt_mw_get_position_fct pf_get_pos;
    x_buf_agt_nfy_fct           pf_mw_nfy;
    x_buf_agt_data_req_ack_fct  pf_mw_ack_fct;
    VOID*                       pv_drv_tag;
    VOID*                       pv_mw_tag;
    BUF_AGT_SRC_ALIGN_INFO_T    t_align_info;
}   BUF_AGENT_INT_DATA_T;

/*----------------------------------------------------------------------------
 * Static variables
 *---------------------------------------------------------------------------*/
static BOOL                     gb_bagt_inited = FALSE;
static BUF_AGENT_INT_DATA_T     gat_bagt_data[BAGT_COMP_NS];

#ifdef CLI_SUPPORT
static UINT16                   ui2_bagt_dbg_level;
#if 0
static HANDLE_T                 h_bagt = NULL_HANDLE;
static BUF_AGT_DATA_BLK_T       t_data_blk;
static BUF_AGT_FCT_TAB_T        t_fct_tab = {0};

static INT32 _buf_agt_cli_mw_get_align(
    INT32          i4_argc, 
    const CHAR**   pps_argv);

static INT32 _buf_agt_cli_mw_get_fct(
    INT32          i4_argc, 
    const CHAR**   pps_argv);

static INT32 _buf_agt_cli_mw_get_pos(
    INT32          i4_argc, 
    const CHAR**   pps_argv);

static INT32 _buf_agt_cli_mw_set_align(
    INT32           i4_argc, 
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_mw_set_nfy(
    INT32           i4_argc, 
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_mw_set_flush(
    INT32           i4_argc, 
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_mw_ack(
    INT32           i4_argc, 
    const CHAR**    pps_argv);
#endif /* 0 */

static INT32 _buf_agt_cli_drv_data_consumed(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_flush(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_data_req(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_get_src_info(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_disconn(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_conn(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static UINT16 _buf_agt_get_dbg_level(VOID);

static INT32 _buf_agt_cli_get_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_set_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv);

#undef DBG_LEVEL_MODULE
#define DBG_LEVEL_MODULE                _buf_agt_get_dbg_level()

/* IO Manager command table */
static CLI_EXEC_T at_bagt_cmd_tbl[] =
{
#if 0
    {"mwgetalign",      "mga",  _buf_agt_cli_mw_get_align,      NULL,   "get DMA source alignment from MW",             CLI_SUPERVISOR},
    {"mwgetfct",        "mgf",  _buf_agt_cli_mw_get_fct,        NULL,   "get ack function from MW",                     CLI_SUPERVISOR},
    {"mwgetposition",   "mgp",  _buf_agt_cli_mw_get_pos,        NULL,   "get playing position from MW",                 CLI_SUPERVISOR},
    {"mwsetalign",      "msa",  _buf_agt_cli_mw_set_align,      NULL,   "set DMA source alignment from MW",             CLI_SUPERVISOR},
    {"mwsetnfy",        "msn",  _buf_agt_cli_mw_set_nfy,        NULL,   "set notify func from MW",                      CLI_SUPERVISOR},
    {"mwsetflush",      "msf",  _buf_agt_cli_mw_set_flush,      NULL,   "Send flush done from MW",                      CLI_SUPERVISOR},    
    {"mwack",           "ma",   _buf_agt_cli_mw_ack,            NULL,   "Send ack from MW",                             CLI_SUPERVISOR},
#endif /* 0 */
    {"drvconnect",      "dc",   _buf_agt_cli_drv_conn,          NULL,   "Connect Fake driver to Buffer Agent",          CLI_SUPERVISOR},
    {"drvdisconnect",   "dd",   _buf_agt_cli_drv_disconn,       NULL,   "Disconnect Fake driver",                       CLI_SUPERVISOR},
    {"drvgetsource",    "dgs",  _buf_agt_cli_drv_get_src_info,  NULL,   "Get DMA source information from Fake driver",  CLI_SUPERVISOR},
    {"drvdatareq",      "ddr",  _buf_agt_cli_drv_data_req,      NULL,   "Send data request from Fake driver",           CLI_SUPERVISOR},
    {"drvflush",        "df",   _buf_agt_cli_drv_flush,         NULL,   "Send flush from Fake driver",                  CLI_SUPERVISOR},
    {"drvdataconsumed", "ddc",  _buf_agt_cli_drv_data_consumed, NULL,   "Send data consumed info from Fake driver",     CLI_SUPERVISOR},
 	{CLI_GET_DBG_LVL_STR, NULL, _buf_agt_cli_get_dbg_level,     NULL,   CLI_GET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
	{CLI_SET_DBG_LVL_STR, NULL, _buf_agt_cli_set_dbg_level,     NULL,   CLI_SET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* IO Manager root command table */
static CLI_EXEC_T at_bagt_root_cmd_tbl[] =
{
	{"bagt",     NULL,        NULL,       at_bagt_cmd_tbl,      "Buffer Agent commands",   CLI_SUPERVISOR},
	END_OF_CLI_CMD_TBL
};
#endif

/*----------------------------------------------------------------------------
 * Static functions
 *---------------------------------------------------------------------------*/
#ifdef CLI_SUPPORT
#if 0
/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_handle_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static VOID _buf_agt_cli_mw_handle_nfy_fct(
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data)
{
    switch (e_nfy_cond)
    {
        case RM_COND_IGNORE:
            break;
            
        case RM_COND_CLOSED:
        case RM_COND_CLOSING:
        {
            /* Set the RTC handle to NULL */
            h_bagt = NULL_HANDLE;
        }
        break;

        default:
            break;
    }
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_handle
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_handle(VOID)
{
    INT32              i4_ret;
    OBJ_TO_COMP_REL_T  e_obj_to_comp_rel = OBJ_COMP_REL_IGNORE;
    DRV_TYPE_T         e_type            = DRVT_BUF_AGENT;
    UINT16             ui2_id            = ANY_COMP_ID;
    const CHAR*        ps_name           = NULL;
    CTRL_TYPE_T        e_ctrl            = CTRL_TYPE_SHARED;
    VOID*              pv_comp_sel_data  = NULL;
    rm_comp_sel_fct    pf_comp_sel       = NULL;
    VOID*              pv_tag            = NULL;
    rm_nfy_fct         pf_nfy            = _buf_agt_cli_mw_handle_nfy_fct;
    RM_COND_T          e_rm_cond;
        
    if (h_bagt == NULL_HANDLE)
    {
        i4_ret = rm_open_comp(NULL_HANDLE,
                              e_obj_to_comp_rel,
                              e_type,
                              ui2_id,
                              ps_name,
                              e_ctrl,
                              pv_comp_sel_data,
                              pf_comp_sel,
                              pv_tag,
                              pf_nfy,
                              &h_bagt,
                              &e_rm_cond);
        if (i4_ret == RMR_OK)
        {
            i4_rc = BAGTR_OK;
        }
        else
        {
            i4_rc = BAGTR_FAILED;
        }
    }
    else
    {
        /* handle is already initialized. */
        i4_ret = BAGTR_OK;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_align
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_align(
    INT32          i4_argc, 
    const CHAR**   pps_argv)
{
    BUF_AGT_SRC_ALIGN_INFO_T    t_info;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    z_len = sizeof(BUF_AGT_SRC_ALIGN_INFO_T);
    i4_ret = rm_get(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_GET_TYPE_SRC_ALIGN_INFO,
                    (VOID*)&t_info,
                    &z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("get the align info: align = %d, min_size = %d\n",
                   t_info.ui4_align,
                   t_info.ui4_min_blk_size);
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't get align info\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_fct(
    INT32          i4_argc, 
    const CHAR**   pps_argv)
{
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    z_len = sizeof(BUF_AGT_FCT_TAB_T);
    i4_ret = rm_get(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_GET_TYPE_ACK_FCT_TAB,
                    (VOID*)&t_fct_tab,
                    &z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("get ack fct tab ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't get ack fct tab\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_pos
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_pos(
    INT32          i4_argc, 
    const CHAR**   pps_argv)
{
    BUF_AGT_POSITION_T          t_pos;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    z_len = sizeof(BUF_AGT_POSITION_T);
    i4_ret = rm_get(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_GET_TYPE_POSITION,
                    (VOID*)&t_pos,
                    &z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("get pos req_id %d, offset %d\n",
                   t_pos.ui4_current_req_id,
                   t_pos.ui4_current_byte_offset);
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't get position\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_set_align
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_set_align(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    BUF_AGT_SRC_ALIGN_INFO_T    t_info;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    if (i4_argc < 4)
    {
        x_dbg_stmt("msa [align] [max] [min]\n");
        return BAGTR_OK;
    }

    t_info.ui4_align = x_strtoll(pps_argv[1], NULL, 10);
    t_info.ui4_min_blk_size = x_strtoll(pps_argv[2], NULL, 10);
    z_len = sizeof(BUF_AGT_SRC_ALIGN_INFO_T);

    i4_ret = rm_set(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_SRC_ALIGN_INFO,
                    (VOID*)&t_info,
                    z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("set the align info: align = %d, min_size = %d\n",
                   t_info.ui4_align,
                   t_info.ui4_min_blk_size);
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't set align info\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_cli_mw_nfy_fct(
    VOID*                  pv_nfy_tag,
    BUF_AGT_NFY_COND_T     e_nfy_cond,
    UINT32                 ui4_data_1,
    UINT32                 ui4_data_2)
{
    switch (e_nfy_cond)
    {
        case BUF_AGT_NFY_COND_DATA_REQ:
            t_data_blk = *(BUF_AGT_DATA_BLK_T*)ui4_data_1;
            x_dbg_stmt("_buf_agt_cli_mw_nfy_fct BUF_AGT_NFY_COND_DATA_REQ req id %d, buf addr %x, size %d\n",
                       t_data_blk.ui4_req_id,
                       t_data_blk.pui1_buff,
                       t_data_blk.z_size);
            break;
        case BUF_AGT_NFY_COND_FLUSH_REQ:
            x_dbg_stmt("_buf_agt_cli_mw_nfy_fct BUF_AGT_NFY_COND_FLUSH_REQ\n");
            break;
        case BUF_AGT_NFY_COND_DATA_CONSUMED:
            x_dbg_stmt("_buf_agt_cli_mw_nfy_fct BUF_AGT_NFY_COND_DATA_CONSUMED req_id %d\n",
                       ui4_data_1);
            break;
        default:
            x_dbg_stmt("unknown _buf_agt_cli_mw_nfy_fct condition\n");
            break;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_data_consumed
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_set_nfy(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    BUF_AGT_NFY_INFO_T          t_info;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    t_info.pf_nfy = _buf_agt_cli_mw_nfy_fct;
    t_info.pv_tag = NULL;
    z_len = sizeof(BUF_AGT_NFY_INFO_T);

    i4_ret = rm_set(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_NFY_FCT,
                    (VOID*)&t_info,
                    z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("set nfy fct ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't set nfy fct\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_set_flush
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_set_flush(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    i4_ret = rm_set(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_FLUSH_DONE,
                    (VOID*)NULL,
                    0,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("set flush done ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't flush done\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_ack
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_ack(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    BOOL                            b_req;
    BUF_AGT_DATA_REQ_ACK_COND_T     e_ack_cond = BUF_AGT_DATA_REQ_ACK_COND_DONE;

    if (0 == x_strcmp(pps_argv[1], "c"))
    {
        e_ack_cond = BUF_AGT_DATA_REQ_ACK_COND_CANCELED; 
    }
    else if (0 == x_strcmp(pps_argv[1], "e"))
    {
        e_ack_cond = BUF_AGT_DATA_REQ_ACK_COND_ERROR; 
    }

    b_req = t_fct_tab.pf_data_req_ack(0,
                                      e_ack_cond,
                                      &t_data_blk);

    if (b_req)
    {
        x_dbg_stmt("pf_data_req_ack ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("pf_data_req_ack failed\n");
        return BAGTR_FAILED;
    }
}
#endif /* 0 */

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_data_consumed
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_data_consumed(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32               i4_ret;
    BUF_AGT_DATA_BLK_T  t_data_blk;
    UINT16              ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = 0;
    }
    else
    {
        ui2_id = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
    }

    t_data_blk.pui1_buff    = (UINT8*)0x008800;
    t_data_blk.z_size       = 1280;
    t_data_blk.ui4_req_id   = 888;

    i4_ret = buf_agt_mw_data_consumed(ui2_id,
                                      &t_data_blk);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_consumed failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_consumed ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_flush
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_flush(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32               i4_ret;
    UINT16          ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = 0;
    }
    else
    {
        ui2_id = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_flush(ui2_id);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_flush failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_flush ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_data_req
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_data_req(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32               i4_ret;
    BUF_AGT_DATA_BLK_T  t_data_blk;
    UINT16              ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = 0;
    }
    else
    {
        ui2_id = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
    }

    t_data_blk.pui1_buff    = (UINT8*)0x007700;
    t_data_blk.z_size       = 10240;
    t_data_blk.ui4_req_id   = 365;

    i4_ret = buf_agt_mw_data_req(ui2_id,
                                 &t_data_blk);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_req failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_req ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_get_src_info
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_get_src_info(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    BUF_AGT_SRC_ALIGN_INFO_T    t_data_blk;
    INT32                       i4_ret;
    UINT16                      ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = 0;
    }
    else
    {
        ui2_id = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_get_source_info(ui2_id,
                                        &t_data_blk);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_get_source_info failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_get_source_info align %d, min size %d\n",
               t_data_blk.ui4_align,
               t_data_blk.ui4_min_blk_size);
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_disconn
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_disconn(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;
    UINT16          ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = 0;
    }
    else
    {
        ui2_id = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_disconnect(ui2_id);

    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_disconnect failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_disconnect ok\n");
    }


    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_cli_nfy_fct(
    VOID*                     pv_tag,
    BUF_AGT_MW_NFY_COND_T     e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2)
{
    BUF_AGT_DATA_BLK_T*       pt_data_blk;

    switch (e_nfy_cond)
    {
        case BUF_AGT_MW_NFY_COND_DATA_REQ_DONE:
            x_dbg_stmt("[BAGT] BUF_AGT_MW_NFY_COND_DATA_REQ_DONE notify\n");
            break;

        case BUF_AGT_MW_NFY_COND_DATA_REQ_ERROR:
            x_dbg_stmt("[BAGT] BUF_AGT_MW_NFY_COND_DATA_REQ_ERROR notify\n");
            break;

        case BUF_AGT_MW_NFY_COND_DATA_REQ_CANCELED:
            x_dbg_stmt("[BAGT] BUF_AGT_MW_NFY_COND_DATA_REQ_CANCELED notify\n");
            break;

        case BUF_AGT_MW_NFY_COND_FLUSH_DONE:
            x_dbg_stmt("[BAGT] Flush done notify\n");
            return TRUE;

        default:
            x_dbg_stmt("[BAGT] Unknown notify cond %d\n",
                   e_nfy_cond);
            return TRUE;
    }

    pt_data_blk = (BUF_AGT_DATA_BLK_T*)ui4_data_1;
    
    x_dbg_stmt("[BAGT] data blk req_id %d, buf addr %x, buf size %d\n",
           pt_data_blk->ui4_req_id,
           pt_data_blk->pui1_buff,
           pt_data_blk->z_size);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_get_position_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_cli_get_position_fct(
    VOID*       pv_tag,
    UINT32*     pui4_current_req_id,
    UINT32*     pui4_current_byte_offset)
{
    *pui4_current_req_id = 100;
    *pui4_current_byte_offset = 123;

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_conn
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_conn(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;
    UINT16          ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = 0;
    }
    else
    {
        ui2_id = (UINT16)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_connect(ui2_id,
                                _buf_agt_cli_nfy_fct,
                                _buf_agt_cli_get_position_fct,
                                NULL);

    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_connect failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_connect ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
static UINT16 _buf_agt_get_dbg_level(VOID)
{
    if (gb_bagt_inited)
    {
        return ui2_bagt_dbg_level;
    }
    else
    {
        return 0;
    }
}

/*-----------------------------------------------------------------------------
 * Name: iom_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_set_dbg_level(
    UINT16          ui2_level)
{
    if (gb_bagt_inited)
    {
        ui2_bagt_dbg_level = ui2_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_get_dbg_level
 *
 * Description: This API gets the current setting of IO Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_get_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;

    /* */

    i4_ret = x_cli_show_dbg_level(_buf_agt_get_dbg_level());

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_set_dbg_level
 *
 * Description: This API sets the debug level of IO Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_set_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;
    UINT16          ui2_dbg_level;

    /* bagt.sdl [level] */

    /* Check arguments */
	if ((i4_argc != 2) ||
	    (pps_argv == NULL))
	{
		x_dbg_stmt("bagt.sdl [level]\n\r");
		x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
		return CLIR_INV_CMD_USAGE;
	}

    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_ret == CLIR_OK)
    {
        if (_buf_agt_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_ret = CLIR_CMD_EXEC_ERROR;
        }
        else
        {
            i4_ret = x_cli_show_dbg_level(_buf_agt_get_dbg_level());
        }
    }
    else
    {
		x_dbg_stmt("gagt.sdl [level]\n\r");
		x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
		return CLIR_INV_CMD_USAGE;
    }

    return i4_ret;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_req_ack_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_req_ack_fct(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_DATA_REQ_ACK_COND_T e_ack_cond,
    BUF_AGT_DATA_BLK_T*         pt_data_blk)
{
    BUF_AGT_MW_NFY_COND_T       e_mw_ack_cond;
    BOOL                        b_ret;

    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy)
    {
        /* TODO: Add debug information */
        return FALSE;
    }

    /* convert the notify condition */
    switch (e_ack_cond)
    {
        case BUF_AGT_DATA_REQ_ACK_COND_DONE:
            e_mw_ack_cond = BUF_AGT_MW_NFY_COND_DATA_REQ_DONE;
            break;
        case BUF_AGT_DATA_REQ_ACK_COND_ERROR:
            e_mw_ack_cond = BUF_AGT_MW_NFY_COND_DATA_REQ_ERROR;
            break;
        case BUF_AGT_DATA_REQ_ACK_COND_CANCELED:
            e_mw_ack_cond = BUF_AGT_MW_NFY_COND_DATA_REQ_CANCELED;
            break;
        default:
            /* TODO: Add debug information */
            return FALSE;
    }

    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                    e_mw_ack_cond,
                                    (UINT32)pt_data_blk,
                                    0);
    if (FALSE == b_ret)
    {
    }

    return b_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_src_align_info
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_src_align_info(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_SRC_ALIGN_INFO_T*   pt_align_info)
{
    *pt_align_info = gat_bagt_data[ui2_buf_agt_id].t_align_info;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_ack_fct_tab
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_ack_fct_tab(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_FCT_TAB_T*          pt_ack_fct)
{
    pt_ack_fct->pf_data_req_ack = gat_bagt_data[ui2_buf_agt_id].pf_mw_ack_fct;
    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_position
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_position(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_POSITION_T*         pt_position)
{
    BOOL                        b_ret;

    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_get_pos)
    {
        return BAGTR_NOT_REGISTERED;
    }

    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_get_pos(
                                    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                    &pt_position->ui4_current_req_id,
                                    &pt_position->ui4_current_byte_offset);
    if (FALSE == b_ret)
    {
        return BAGTR_FAILED;
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_nfy_fct(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_NFY_INFO_T*         pt_nfy_info)
{
    pt_nfy_info->pv_previous_tag    = gat_bagt_data[ui2_buf_agt_id].pv_mw_tag;
    pt_nfy_info->pf_previous_nfy    = gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy;

    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag         = pt_nfy_info->pv_tag;
    gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy         = pt_nfy_info->pf_nfy;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_src_align_info
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_src_align_info(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_SRC_ALIGN_INFO_T*   pt_align_info)
{
    UINT32                      ui4_align;

    /* error checking 
        1. ui4_align should be > 0.
        2. ui4_align should be powered of 2. */
    if (pt_align_info->ui4_align == 0)
    {
        return BAGTR_FAILED;
    }

    ui4_align = pt_align_info->ui4_align;
    
    while (ui4_align)
    {
        if (ui4_align & 0x1)
        {
            /* get non-zero bit, doing check */
            ui4_align >>= 1;
            if (ui4_align)
            {
                return BAGTR_FAILED;
            }
        }
        else
        {
            ui4_align >>= 1;
        }
    }

    /* Send the notify to audio driver if someone changes the alignment info */
    if (NULL != gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy)
    {
        BOOL b_ret;

        b_ret = gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy(
                                        gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                        BUF_AGT_MW_NFY_COND_ALIGN_INFO,
                                        (UINT32)pt_align_info,
                                        0);
        if (FALSE == b_ret)
        {
            return BAGTR_FAILED;
        }
    }

    gat_bagt_data[ui2_buf_agt_id].t_align_info = *pt_align_info;
    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_flush_done
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_flush_done(
    UINT16                      ui2_buf_agt_id)
{
    BOOL                        b_ret;

    if (gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy)
    {
        b_ret = gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                    BUF_AGT_MW_NFY_COND_FLUSH_DONE,
                                    0,
                                    0);
        if (FALSE == b_ret)
        {
        }
    }
    else
    {
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_drv_init
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_drv_init(VOID)
{
    INT32       i4_idx;

    if (gb_bagt_inited)
    {
        return TRUE;
    }

    for (i4_idx = 0; i4_idx < BAGT_COMP_NS; i4_idx++)
    {
        gat_bagt_data[i4_idx].pf_drv_nfy                     = NULL;
        gat_bagt_data[i4_idx].pf_get_pos                     = NULL;
        gat_bagt_data[i4_idx].pf_mw_nfy                      = NULL;
        gat_bagt_data[i4_idx].pv_drv_tag                     = NULL;
        gat_bagt_data[i4_idx].pv_mw_tag                      = NULL;
        gat_bagt_data[i4_idx].pf_mw_ack_fct          = _buf_agt_req_ack_fct;
        /* the default alignment value is 1 */
        gat_bagt_data[i4_idx].t_align_info.ui4_align         = BLOCK_DEF_ALIGNMENT;
        gat_bagt_data[i4_idx].t_align_info.ui4_min_blk_size  = 0;
    }

#ifdef CLI_SUPPORT
    ui2_bagt_dbg_level = DBG_LEVEL_NONE;
#endif /* CLI_SUPPORT */

    gb_bagt_inited = TRUE;

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID*           pv_data,
    SIZE_T*         pz_size)
{
    INT32           i4_ret = RMR_OK;

    /* Check if initialized */
    if (!gb_bagt_inited)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    /* Check component ID */
    if ((pt_comp_id == NULL) ||
        (pt_comp_id->e_type != DRVT_BUF_AGENT) ||
        (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1)))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    /* handle get type */
    switch (e_get_type)
    {
        case BUF_AGT_GET_TYPE_SRC_ALIGN_INFO:
            /* Get source DMA alignment information */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_SRC_ALIGN_INFO_T)))
            {
                *pz_size = sizeof(BUF_AGT_SRC_ALIGN_INFO_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_src_align_info(pt_comp_id->ui2_id,
                                                (BUF_AGT_SRC_ALIGN_INFO_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;

        case BUF_AGT_GET_TYPE_ACK_FCT_TAB:
            /* Get Acknowledgement function table */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_FCT_TAB_T)))
            {
                *pz_size = sizeof(BUF_AGT_FCT_TAB_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_ack_fct_tab(pt_comp_id->ui2_id,
                                             (BUF_AGT_FCT_TAB_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;

        case BUF_AGT_GET_TYPE_POSITION:
            /* Get Position of media data which is playing on */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_POSITION_T)))
            {
                *pz_size = sizeof(BUF_AGT_POSITION_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_position(pt_comp_id->ui2_id,
                                          (BUF_AGT_POSITION_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;

        default:
            i4_ret = RMR_DRV_INV_GET_INFO;
            break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_set_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_data,
    SIZE_T          z_size)
{
    INT32           i4_ret = RMR_OK;

    /* Check if initialized */
    if (!gb_bagt_inited)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    /* Check component ID */
    if ((pt_comp_id == NULL) || 
        (pt_comp_id->e_type != DRVT_BUF_AGENT) ||
        (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1)))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    /* handle set type here */
    switch (e_set_type)
    {
        case BUF_AGT_SET_TYPE_NFY_FCT:
            /* Set Notify function table */
            if ((NULL == pv_data) ||
                (z_size < sizeof(BUF_AGT_NFY_INFO_T)))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4_ret = _buf_agt_set_nfy_fct(pt_comp_id->ui2_id,
                                         (BUF_AGT_NFY_INFO_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;

        case BUF_AGT_SET_TYPE_SRC_ALIGN_INFO:
            /* Set source DMA alignment information */
            if ((NULL == pv_data) ||
                (z_size < sizeof(BUF_AGT_SRC_ALIGN_INFO_T)))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4_ret = _buf_agt_set_src_align_info(pt_comp_id->ui2_id,
                                                (BUF_AGT_SRC_ALIGN_INFO_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;

        case BUF_AGT_SET_TYPE_FLUSH_DONE:
            /* Notify decoder that the flush request is finished */
            i4_ret = _buf_agt_set_flush_done(pt_comp_id->ui2_id);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;

        default:
            i4_ret = RMR_DRV_INV_SET_INFO;
            break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_init
 *
 * Description: This API initializes Buffer Agent CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: BAGTR_OK                    Routine successful.
 *          BAGTR_CLI_ERROR             CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 _buf_agt_cli_init(VOID)
{
#ifdef CLI_SUPPORT
    INT32       i4_ret;

    /* Attach Buffer Agent CLI command table to CLI */
    i4_ret = x_cli_attach_cmd_tbl(at_bagt_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_ret != CLIR_OK)
    {
        return BAGTR_CLI_ERROR;
    }
    return BAGTR_OK;
#else
    return BAGTR_OK;
#endif
}

//=====================================================================
// Interface functions

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_connect
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pf_nfy          References.
 *          pf_get_pos      References.
 *          pv_tag          References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_connect(
    UINT16                      ui2_buf_agt_id,
    buf_agt_mw_nfy_fct          pf_nfy,
    buf_agt_mw_get_position_fct pf_get_pos, 
    VOID*                       pv_tag)
{
    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* pt_nfy and pf_get_pos can't be NULL.
       pv_tag could be NULL */
    if ((NULL == pf_nfy) || (NULL == pf_get_pos))
    {
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if Buffer Agent has been connected */
    if ((gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy) ||
        (gat_bagt_data[ui2_buf_agt_id].pf_get_pos))
    {
        return BAGTR_ALREADY_CONN;
    }

    /* set the value */
    gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy = pf_nfy;
    gat_bagt_data[ui2_buf_agt_id].pf_get_pos = pf_get_pos;
    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag = pv_tag;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_disconnect
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_disconnect(
    UINT16  ui2_buf_agt_id)
{
    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* set the value */
    gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy = NULL;
    gat_bagt_data[ui2_buf_agt_id].pf_get_pos = NULL;
    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag = NULL;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_get_source_info
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pt_data_blk     References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_get_source_info( 
    UINT16                    ui2_buf_agt_id,
    BUF_AGT_SRC_ALIGN_INFO_T* pt_data_blk)
{
    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check parameter */
    if (NULL == pt_data_blk)
    {
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    *pt_data_blk = gat_bagt_data[ui2_buf_agt_id].t_align_info;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_data_req
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pt_data_blk     References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_data_req(
    UINT16                  ui2_buf_agt_id,
    BUF_AGT_DATA_BLK_T*     pt_data_blk)
{
    BOOL                    b_ret;

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check parameter */
    if (NULL == pt_data_blk)
    {
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        return BAGTR_NOT_REGISTERED;
    }

    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_REQ,
                                    (UINT32)pt_data_blk,
                                    (UINT32)NULL);
    if (b_ret)
    {
        return BAGTR_OK;
    }
    else
    {
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_flush
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_flush(
    UINT16                    ui2_buf_agt_id)
{
    BOOL                    b_ret;

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        return BAGTR_NOT_REGISTERED;
    }

    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_FLUSH_REQ,
                                    (UINT32)NULL,
                                    (UINT32)NULL);

    if (b_ret)
    {
        return BAGTR_OK;
    }
    else
    {
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_data_consumed
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pt_data_blk     References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_data_consumed(
    UINT16                  ui2_buf_agt_id,
    BUF_AGT_DATA_BLK_T*     pt_data_blk)
{
    BOOL                    b_ret;

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check parameter */
    if (NULL == pt_data_blk)
    {
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        return BAGTR_NOT_REGISTERED;
    }

    b_ret =  gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_CONSUMED,
                                    (UINT32)pt_data_blk->ui4_req_id,
                                    (UINT32)NULL);

    if (b_ret)
    {
        return BAGTR_OK;
    }
    else
    {
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_connect
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_set_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 _buf_agt_connect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_CONN_TYPE_T  e_conn_type,
    const VOID*      pv_conn_info,
    SIZE_T           z_conn_info_len,
    VOID*            pv_tag,
    x_rm_nfy_fct     pf_nfy)
{
    /* Check component type and id */

    if (NULL == pt_comp_id)
    {
        return RMR_DRV_INV_CONN_INFO;
    }    

    if (DRVT_BUF_AGENT != pt_comp_id->e_type)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    return RMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_disconn
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_set_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 _buf_agt_disconn(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_DISC_TYPE_T  e_disc_type,
    const VOID*      pv_disc_info,
    SIZE_T           z_disc_info_len)
{
    /* Check component type and id */
    if (NULL == pt_comp_id)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (DRVT_BUF_AGENT != pt_comp_id->e_type)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1))
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    return RMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set
 *
 * Description: Initialize middleware buffer agent driver
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_init(void)
{
    INT32 i4_ret;

    /* Component ID */
    DRV_COMP_REG_T t_comp_reg;

    /* Function table */
    DRV_COMP_FCT_TBL_T t_comp_fct;

    /* Setup component ID */
    t_comp_reg.e_type = DRVT_BUF_AGENT;
    t_comp_reg.e_id_type = ID_TYPE_RANGE;
    t_comp_reg.u.t_range.ui2_first_id = BAGT_COMP_1;
    t_comp_reg.u.t_range.ui2_delta_id = 1;
    t_comp_reg.u.t_range.ui2_num_of_ids = BAGT_COMP_NS - 1;
    t_comp_reg.u.t_range.pv_first_tag = NULL;
    t_comp_reg.u.t_range.pv_delta_tag = NULL;
    t_comp_reg.u.t_range.ui1_port = 1;

    /* Setup component function table */
    t_comp_fct.pf_rm_connect = _buf_agt_connect;
    t_comp_fct.pf_rm_disconnect = _buf_agt_disconn;
    t_comp_fct.pf_rm_get = _buf_agt_get;
    t_comp_fct.pf_rm_set = _buf_agt_set;

    /* init cli */
    i4_ret = _buf_agt_cli_init();
    if (BAGTR_OK != i4_ret)
    {
        return RMR_DRV_ERROR;
    }

    /* init buffer agent here */    
    if (!_buf_agt_drv_init())   /* Init failed. */
    {
        return RMR_DRV_ERROR;
    }

    /* Register component to Resource Manager database */
    i4_ret = x_rm_reg_comp(&t_comp_reg, 1, 1, "", 0, &t_comp_fct, NULL, 0);

    return i4_ret;
}


