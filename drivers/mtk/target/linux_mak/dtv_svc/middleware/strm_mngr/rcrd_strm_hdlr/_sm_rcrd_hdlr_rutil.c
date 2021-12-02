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
 * $RCSfile: _sm_rcrd_hdlr_rutil.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/10 $
 * $SWAuthor: Lianming Lin $
 * $MD5HEX: 9651a12385e0d96d6be213500a0475f8 $
 *
 * Description: This file handle message come from Record utility moduel 
 *              within record stream handler 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "handle/x_handle.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"


#include "rec_util/rec_util.h"
#include "strm_mngr/rcrd_strm_hdlr/_sm_rcrd_hdlr_rutil.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  _sm_rcrd_strm_rec_util_nfy_fct
 *
 * Description: This function is record utilty notify function
 *
 * Inputs:  
 *       h_rec_util:       record utilty handle
 *       pv_nfy_tag:       notify tag
 *       e_ntfy_code:      record utility condition
 *       ui4_data:         additonal data
 *
 * Outputs: 
 *       Send message to stream handler to update record utility condition in SM
 *
 * Returns: 
 *       TRUE: can reset record utility DB
 *       FALSE: can not reset DB
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
static BOOL _sm_rcrd_strm_rec_util_nfy_fct(HANDLE_T              h_rec_util,
                                           VOID*                 pv_nfy_tag,
                                           REC_UTIL_NTFY_CODE_T  e_ntfy_code,
                                           UINT32                ui4_data)
{
    SM_RCRD_STRM_HDLR_MSG_T     t_msg;
    
    if (NULL == pv_nfy_tag)
    {
        return FALSE;
    }

    t_msg.e_msg_type = SM_RCRD_STRM_MSG_TYPE_RUTIL;
    t_msg.h_stream= (HANDLE_T)pv_nfy_tag;
    
    t_msg.u.t_rutil_msg.e_rutil_cond = e_ntfy_code;
    t_msg.u.t_rutil_msg.ui4_reason = 0;
    t_msg.u.t_rutil_msg.ui4_data = ui4_data;
    
    sm_rs_send_msg(&t_msg);
    return TRUE;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name:  sm_rcrd_strm_rutil_open
 *
 * Description: This function opens record utility for writting TS stream
 *
 * Inputs:  
 *       h_stream:  stream handle
 *       pt_rs_obj: stream object
 *
 * Outputs: 
 *       pt_rs_obj: open a record utilty and get a handle
 *
 * Returns: 
 *        SMR_OK: Succeed
 *        Others: Fail
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-14 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_rutil_open(HANDLE_T          h_stream, 
                              SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                      i4_ret;
    REC_UTIL_COND_T            e_cond;
    /*SM_RCRD_STRM_HDLR_MSG_T    t_msg;*/

    if (NULL == pt_rs_obj)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_rutil_open:NULl argument\r\n"));
        return SMR_INV_ARG;
    }

    /*Open record utility*/
    i4_ret = rec_util_open(pt_rs_obj->t_pmt_info.ai1_name_rec_util,
                           _sm_rcrd_strm_rec_util_nfy_fct,
                           (VOID*)(h_stream),
                           &(pt_rs_obj->t_rutil_info.h_record_util));
    if (i4_ret != SMR_OK)
    {
        SM_RSH_DBG_ERR(("sm_rcrd_strm_rutil_open:open record utility err=%d\r\n", 
                        i4_ret));
        return i4_ret;   
    }
    sm_rs_send_msg_do_event(SM_EVN_RSH_RUTI_OPENED,
                            h_stream,
                            0);
    
    /*Get record utility state*/
    i4_ret = rec_util_get_cond(pt_rs_obj->t_rutil_info.h_record_util,
                               &e_cond);
    if ((SMR_OK == i4_ret) && (REC_UTIL_COND_READY == e_cond))
    {
        sm_rs_send_msg_do_event(SM_EVN_RSH_RUTI_READY,
                                h_stream,
                                0);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name:  sm_sess_rcrd_rutil_close
 *
 * Description: This function closes record utility
 *
 * Inputs:  
 *       h_stream:  stream handle
 *       pt_rs_obj: stream object
 *
 * Outputs: 
 *       pt_sess_obj: close record utility
 *
 * Returns: NULL
 *
 * Author : lianming lin(mtk40234)
 *
 * History:
 *   (1)2009-03-10 : initial
 ------------------------------------------------------------------------------*/
INT32 sm_rcrd_strm_rutil_close(HANDLE_T          h_stream, 
                               SM_RCRD_STREAM_T* pt_rs_obj)
{
    INT32                      i4_ret = SMR_OK;
    /*REC_UTIL_COND_T            e_cond;*/
    /*SM_RCRD_STRM_HDLR_MSG_T    t_msg;*/

    if (pt_rs_obj->t_rutil_info.h_record_util != NULL_HANDLE)
    {
        i4_ret = rec_util_close(pt_rs_obj->t_rutil_info.h_record_util);
        if (i4_ret != RMR_OK)
        {
            /*assume closed OK.*/
            SM_RSH_DBG_ERR(("sm_rcrd_strm_rutil_close:close record utility err=%d\r\n", 
                            i4_ret));
        }
        pt_rs_obj->t_rutil_info.h_record_util = NULL_HANDLE;
    }
    else
    {
        /* Pretend closed, if handle is not existed */
        /*sm_rs_send_msg_do_event(SM_EVN_RSH_RUTI_CLOSED,
                                h_stream,
                                0);*/
    }

    /*Send out object state closed*/
    sm_rs_send_msg_do_event(SM_EVN_RSH_RUTI_CLOSED,
                            h_stream,
                            0);
        
    return i4_ret;
}





