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
 * $RCSfile: cm_preconn.c,v $
 * $Revision: #1 $
 * $Date: 
 * $Author: 
 *
 * Description:
 *         This header file contains the implementation of the connection-
 *         related APIs.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "conn_mngr/cm_conn_api.h"
#include "conn_mngr/cm_pipe_api.h"
#include "conn_mngr/cm_handler_api.h"
#include "conn_mngr/brdcst/u_brdcst.h"
#include "conn_mngr/_cm.h"
#include "svl/x_svl.h"
#include "file_mngr/x_fm.h"
#include "svl_bldr/u_sb.h"
#include "dbg/dbg.h"

#ifdef MW_FAST_BOOT_PRE_CONNECT
/*-----------------------------------------------------------------------------
 * Name: _pre_connect_pipe_nfy
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 *              
 ----------------------------------------------------------------------------*/
static VOID _pre_connect_pipe_nfy(
    HANDLE_T                    h_cm_pipe,
    CM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_data)
{
    return;
}

/*-----------------------------------------------------------------------------
 * Name: _pre_connect_conn_nfy
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: 
 *              
 ----------------------------------------------------------------------------*/
static VOID _pre_connect_conn_nfy(
    HANDLE_T                    h_conn,
    CM_COND_T                   e_nfy_cond,
    VOID*                       pv_nfy_tag,
    UINT32                      ui4_data)
{
    if(e_nfy_cond == CM_COND_CONNECTED)
    {
        DBG_INFO((DBG_PREFIX"{pre-connect} connected!\n"));
    }
    else if(e_nfy_cond == CM_COND_CONNECTION_INFO)
    {
        if(ui4_data == CM_CONN_INFO_PRE_CONNECTION_END)
        {
            cm_disconnect_pre_conn(h_conn);
        }
    }
    else if(e_nfy_cond == CM_COND_DISCONNECTED)
    {
        DBG_INFO((DBG_PREFIX"{pre-connect} disconnected!\n"));
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name: cm_load_pre_conn_info
 *
 * Description: This API is called by cm_store_pre_conn_info and load 
 *                    pre-connect svc info from eeprom
 *
 * Inputs:  
 *
 * Outputs: pt_info  Contains the pre-connect svc information.
 *
 * Returns: CMR_OK    Routine successful.
 *              CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/

INT32 cm_load_pre_conn_info(CM_PRE_CONNECT_INFO_T* pt_info)
{
#ifdef MW_FAST_BOOT_PRE_CONNECT

    HANDLE_T h_mw_eep = 0;
    UINT64   ui8_pos  = 0;
    UINT32   ui4_n    = 0;
    INT32    i4_ret   = CMR_OK;

    i4_ret = x_fm_open(FM_ROOT_HANDLE,
                       //MW_EEP_PATH,
                       (SN_DEV_PATH "/" SN_EEPROM_3),
                       FM_READ_WRITE,
                       0777,
                       TRUE,
                       &h_mw_eep);
    if(i4_ret != FMR_OK)
    {
        DBG_ERROR((DBG_PREFIX "open MW eeprom path(%s) failed!\n", (SN_DEV_PATH "/" SN_EEPROM_3)));
        return CMR_FAIL;
    }

    do
    {
        i4_ret = x_fm_lseek(h_mw_eep, 
                            //(INT64)MW_EEP_DTV_SRC_OFFSET, 
                            (INT64)992,
                            FM_SEEK_BGN, 
                            &ui8_pos);
        if(i4_ret != FMR_OK)
        {
            DBG_ERROR((DBG_PREFIX "seek %d failed!\n", 992));
            break;
        }
        
        i4_ret = x_fm_read(h_mw_eep, 
                          (VOID*)pt_info,
                          sizeof(CM_PRE_CONNECT_INFO_T), 
                          &ui4_n);
        
        if((i4_ret != FMR_OK) || (ui4_n != (sizeof(CM_PRE_CONNECT_INFO_T))))
        {
            DBG_ERROR((DBG_PREFIX "read DTV src fail:%d\n", i4_ret));
            break;
        }

    }while(0);
    
    DBG_INFO((DBG_PREFIX "pt_info loaded value: 0x%x,0x%x,0x%x\n", 
                                    pt_info->b_tv_src,
                                    pt_info->ui1_svl_lst,
                                    pt_info->ui4_channel_id));

    i4_ret = x_fm_close( h_mw_eep );
    if(i4_ret != FMR_OK)
    {
        DBG_ERROR((DBG_PREFIX "close eeprom failed:%d\n", i4_ret));
    }
#endif
    return CMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cm_store_pre_conn_info
 *
 * Description: This API is called by x_cm_store_pre_conn_info and store 
 *                    pre-connect svc info into eeprom
 *
 * Inputs:  b_tv_src: Does current svc is from tv source
 *             ui1_svl_lst: the current svc list
 *             ui4_channel_id: the current svc's channel id
 *
 * Outputs: 
 *
 * Returns: CMR_OK    Routine successful.
 *              CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/

INT32 cm_store_pre_conn_info(BOOL   b_tv_src, 
                                  UINT8  ui1_svl_lst, 
                                  UINT32 ui4_channel_id)
{
#ifdef MW_FAST_BOOT_PRE_CONNECT
    HANDLE_T h_mw_eep = 0;
    UINT64   ui8_pos  = 0;
    INT32    i4_ret   = CMR_OK;
    UINT32   ui4_n    = 0;
    CM_PRE_CONNECT_INFO_T t_info;

    x_memset(&t_info, 0, sizeof(CM_PRE_CONNECT_INFO_T));
    
    /* prevent to write the same content to eeprom */
    i4_ret = cm_load_pre_conn_info(&t_info);
    
    if(i4_ret == CMR_OK)
    {
        if((t_info.b_tv_src       == b_tv_src)        &&
           (t_info.ui1_svl_lst    == ui1_svl_lst)     &&
           (t_info.ui4_channel_id == ui4_channel_id))
        {           
            DBG_INFO((DBG_PREFIX"{pre_connect} pre_connect_info is equal\n"));
            return CMR_OK;
        }
    }
    
    /* start to write b_dtv_src, svl_lst, channel_id to eeprom */
    t_info.b_tv_src         = b_tv_src;
    t_info.ui1_svl_lst      = ui1_svl_lst;
    t_info.ui4_channel_id   = ui4_channel_id;

    DBG_INFO((DBG_PREFIX "DTV src:(%s), SVL ID:(0x%x), Channel ID:(0x%x)\n", 
                                  (t_info.b_tv_src==TRUE)?"TRUE":"FALSE", 
                                   t_info.ui1_svl_lst,
                                   t_info.ui4_channel_id));

    i4_ret = x_fm_open(FM_ROOT_HANDLE,
                       //MW_EEP_PATH,
                       (SN_DEV_PATH "/" SN_EEPROM_3),
                       FM_READ_WRITE,
                       0777,
                       TRUE,
                       &h_mw_eep);
    
    if(i4_ret != FMR_OK)
    {
        DBG_ERROR((DBG_PREFIX "open MW eeprom path(%s) failed!\n", (SN_DEV_PATH "/" SN_EEPROM_3)));
        return CMR_FAIL;
    }

    do
    {
        i4_ret = x_fm_lseek(h_mw_eep, 
                            //(INT64)MW_EEP_SVL_LST_OFFSET, 
                            (INT64)992,
                            FM_SEEK_BGN, 
                            &ui8_pos);
        if(i4_ret != FMR_OK)
        {
            DBG_ERROR((DBG_PREFIX "seek %d failed!\n", MW_EEP_SVL_LST_OFFSET));
            break;
        }
        
        i4_ret = x_fm_write(h_mw_eep, 
                            (VOID*)&t_info, 
                            sizeof(CM_PRE_CONNECT_INFO_T),
                            &ui4_n);
        if((i4_ret != FMR_OK) && (ui4_n != sizeof(CM_PRE_CONNECT_INFO_T)))
        {
            DBG_ERROR((DBG_PREFIX "write src type fail:%d\n", i4_ret));
            break;
        }

    }while(0);

    i4_ret = x_fm_close( h_mw_eep );
    if(i4_ret != FMR_OK)
    {
        DBG_ERROR((DBG_PREFIX "close eeprom failed:%d\n", i4_ret));
    }
#endif

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pre_connect
 *
 * Description: This API is called by start function and perform the pre-connect operation 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: CMR_OK    Routine successful.
 *              CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/

INT32 cm_pre_connect (VOID)
{
#ifdef MW_FAST_BOOT_PRE_CONNECT
    CM_COND_T   e_cond  = CM_COND_CLOSED;
    INT32       i4_res  = CMR_OK;
    UINT32      ui4_ver = 0;
    HANDLE_T    h_pipe  = NULL_HANDLE;
    HANDLE_T    h_conn  = NULL_HANDLE;
    HANDLE_T    h_svl   = NULL_HANDLE;
    
    BRDCST_TYPE_T         e_brdcst_type = BRDCST_TYPE_UNKNOWN;
    SVL_REC_T             t_svl_rec;
    CM_PRE_CONNECT_INFO_T t_pre_conn_info;
    CM_COMMAND_T          at_cmds[12];/* command arrays */

    x_memset(&t_svl_rec, 0, sizeof(SVL_REC_T));
    x_memset(&t_pre_conn_info, 0, sizeof(CM_PRE_CONNECT_INFO_T));
    
    /* read pre connect info from SN_DEV_PATH "/" SN_EEPROM_4, offset 64, size 6*/
    i4_res = cm_load_pre_conn_info(&t_pre_conn_info);
    if(i4_res != CMR_OK)
    {
        x_dbg_stmt("{pre-connect} load pre-connect info failed!\n");
        return CMR_FAIL;
    }

    if(t_pre_conn_info.b_tv_src == FALSE)
    {
        x_dbg_stmt("{pre_connect} it is not tv svc\n");
        return CMR_OK;
    }
    
    /* get svl rec id by channel id */
    i4_res = x_svl_open(t_pre_conn_info.ui1_svl_lst,
                        NULL,
                        NULL,
                        & h_svl);

    if(i4_res != SVLR_OK)
    {
        x_dbg_stmt("{pre-connect} open svl(%d) failed!\n",t_pre_conn_info.ui1_svl_lst);
        return CMR_OK;
    }

    i4_res = x_svl_get_rec_by_channel(h_svl,
                                      t_pre_conn_info.ui4_channel_id,
                                      SB_VNET_ALL, 
                                      0, 
                                      &t_svl_rec,
                                      &ui4_ver);
    
    e_brdcst_type = t_svl_rec.uheader.t_rec_hdr.e_brdcst_type ;
    
    if(i4_res != SVLR_OK)
    {
        x_dbg_stmt("{pre-connect} get svl rec by channel id(%d) failed!\n",t_pre_conn_info.ui4_channel_id);
        return CMR_OK;
    }
    else if((e_brdcst_type != BRDCST_TYPE_DVB)  &&
            (e_brdcst_type != BRDCST_TYPE_ATSC) &&
            (e_brdcst_type != BRDCST_TYPE_ISDB) &&
            (e_brdcst_type != BRDCST_TYPE_DTMB))
    {
        x_dbg_stmt("{pre-connect} pre connect svc is not dtv svc e_brdcst_type(%d)!\n",e_brdcst_type);
        return CMR_OK;
    }
    else
    {
        x_dbg_stmt("=====================\n");
        x_dbg_stmt("channel id:(%d), svl rec id:(%d), channel id in rec:(%d), brdcst type:(%d)\n", 
                                t_pre_conn_info.ui4_channel_id, 
                                t_svl_rec.ui2_svl_rec_id, 
                                t_svl_rec.uheader.t_rec_hdr.ui4_channel_id, 
                                t_svl_rec.uheader.t_rec_hdr.e_brdcst_type);
        x_dbg_stmt("=====================\n");
    }

    at_cmds[0].e_code       = CONN_SRC_TYPE;
    at_cmds[0].u.ps_name    = "brdcst";
    at_cmds[1].e_code       = CONN_SRC_NAME;
    at_cmds[1].u.ps_name    = "src_TunerGrp_0";
    at_cmds[2].e_code       = BRDCST_CONN_SVC_LST_ID;
    at_cmds[2].u.ui2_number = t_pre_conn_info.ui1_svl_lst;
    at_cmds[3].e_code       = BRDCST_CONN_SVL_REC_ID;
    at_cmds[3].u.ui2_number = t_svl_rec.ui2_svl_rec_id; 
    at_cmds[4].e_code       = BRDCST_CONN_DISC_IF_COMP_BUSY;    
    at_cmds[4].u.b_boolean  = FALSE;
    at_cmds[5].e_code       = BRDCST_CONN_PAT_TIME_OUT;
    at_cmds[5].u.ui4_number = 3000;
    at_cmds[6].e_code       = BRDCST_CONN_PMT_TIME_OUT;
    at_cmds[6].u.ui4_number = 3000;
    at_cmds[7].e_code       = BRDCST_CONN_TUNER_NO_RIGHTS_OK;
    at_cmds[7].u.b_boolean  = TRUE;
    at_cmds[8].e_code       = BRDCST_CONN_SIGNAL_LOST_DISCONNECT;
    at_cmds[8].u.b_boolean  = FALSE;
    at_cmds[9].e_code       = BRDCST_CONN_WITHOUT_SCDB;
    at_cmds[9].u.b_boolean  = TRUE;
    at_cmds[10].e_code      = BRDCST_CONN_BE_PRE_CONN;
    at_cmds[10].u.b_boolean = TRUE;
    at_cmds[11].e_code      = CONN_END;    

    i4_res = cm_open_pipe_op(at_cmds,
                             254,       /*the lowest priority*/
                             RM_PIPE_FLAG_ALLOW_SLAVE_SET_OP,
                             NULL,
                             _pre_connect_pipe_nfy,
                             &h_pipe,
                             &e_cond);
    
    if ((i4_res == CMR_OK) &&
        (e_cond == CM_COND_OPENED))
    {
        i4_res = cm_connect_op(h_pipe,
                               at_cmds,
                               NULL,
                               _pre_connect_conn_nfy,
                               &h_conn,
                               &e_cond);

        if(i4_res != CMR_OK)
        {
            /*if pre connection failed we still need to return ok to go on running the normal routine*/
            x_dbg_stmt("pre-connect failed (%d)\n",i4_res);
        }
    }
    
    x_dbg_stmt(" pre-connect return (%d)\n",i4_res);
#endif
    return CMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: cm_disconnect_pre_conn
 *
 * Description: This API disconnect the pre-connect 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: CMR_OK    Routine successful.
 *              CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/

INT32 cm_disconnect_pre_conn (HANDLE_T    h_cm_conn)
{
#ifdef MW_FAST_BOOT_PRE_CONNECT
    INT32             i4_res    = CMR_OK;
    CM_COND_T         e_cm_cond = CM_COND_CLOSED; 
    HANDLE_T          h_cm_pipe = NULL_HANDLE;
    HANDLE_TYPE_T     e_type    = INV_HANDLE_TYPE;
    CM_CONNECTION_T*  pt_conn   = NULL;

    i4_res = handle_get_type_obj(h_cm_conn, &e_type, (VOID**)(& pt_conn));

    if((i4_res == HR_OK) && (e_type = CMT_CONNECTION) && (pt_conn != NULL))
    {
        h_cm_pipe = pt_conn->h_pipe;

        cm_disconnect_op (h_cm_conn,&e_cm_cond);
            
        cm_close_pipe_op(h_cm_pipe, & e_cm_cond);
    }
#endif
    return CMR_OK;
}


