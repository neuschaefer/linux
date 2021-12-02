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
 * $RCSfile: midxbuld.c,v $
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
#include "inc/common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/mm_util_memory.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/midxbuld_cli.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util.h"
/*#include "libc/ctype.h"*/
#include "util/x_lnk_list.h"

#else

#include "x_common.h"
#include "common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_midxbuld.h"
#include "midxbuld.h"
#include "midxbuld_cli.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "../mm_util_memory.h"
#include "../mm_util.h"
#include "u_dbg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
SLIST_T(_MIDXBULD_HANDLER_REG_T)         gt_midxbuld_handler_reg_list = {NULL};
static BOOL                              gb_init = FALSE;
static HANDLE_T                          gh_notify_async_exe_thread = NULL_HANDLE;
static HANDLE_T                          gh_notify_async_exe_msgq   = NULL_HANDLE;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 

/*-----------------------------------------------------------------------------
 * Name:  midxbuld_match_handler
 *
 * Description: This match the media format and retrieve the handler function table.
 *
 * Inputs:  pt_format          specifies sixteen characters of a handler name, and the character is case sensitive.
 *
 * Outputs: pt_handler_fct_tbl retrieve the handler function table which match the media format. 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 midxbuld_match_handler (MEDIA_FORMAT_T* pt_format, MIDXBULD_HANDLER_FCT_TBL_T* pt_handler_fct_tbl)
{
    MIDXBULD_HANDLER_REG_T*     pt_reg; 

    SLIST_FOR_EACH(pt_reg, &gt_midxbuld_handler_reg_list, t_link)
    {
        if ((pt_format->e_media_type == pt_reg->t_favor_media_format.e_media_type)
            && (pt_format->t_media_subtype.u.e_aud_subtype == pt_reg->t_favor_media_format.t_media_subtype.u.e_aud_subtype))
        {
            *pt_handler_fct_tbl = pt_reg->t_handler_fct_tbl;
            return MIDXBULDR_OK;
        }          
    }
    return MIDXBULDR_HANDLER_NOT_FOUND;
}

/*-----------------------------------------------------------------------------
 * Name:  midxbuld_register_handler
 *
 * Description: This API register the handler.
 *
 * Inputs:  ps_name         specifies sixteen characters of a handler name, and the character is case sensitive.
 *          ps_favor        specify the favor file extension string list, for example "mp3"
 *          pt_fct_tbl      specify the function table of the engine.
 *
 * Outputs: NONE.
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/

INT32 midxbuld_register_handler (  
                        const CHAR*                       ps_name,    
                        const MEDIA_FORMAT_T*             pt_favor_media_format,
                        const MIDXBULD_HANDLER_FCT_TBL_T* pt_fct_tbl)
{
    MIDXBULD_HANDLER_REG_T* pt_handler_reg = NULL;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (ps_name == NULL) 
        || (pt_favor_media_format == NULL) 
        || (pt_fct_tbl == NULL) )
    {
        return MIDXBULDR_INV_ARG;
    }
    
    /************************************************************************/
    /* Perform register handler                                              */
    /************************************************************************/

    /* check if the engine is exist?*/
    SLIST_FOR_EACH(pt_handler_reg, &gt_midxbuld_handler_reg_list, t_link)
    {
        if (x_strncmp(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN) == 0)
        {
            return MIDXBULDR_NAME_EXIST;
        }
    }

    /* malloc node */
    pt_handler_reg = _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_HANDLER_REG_T), MM_MEM_MIDX_TAG);
    if (pt_handler_reg == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }

    x_memset(pt_handler_reg, 0, sizeof(MIDXBULD_HANDLER_REG_T));

    /* fill data */
    x_strncpy(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN);
    pt_handler_reg->t_favor_media_format = *pt_favor_media_format;
    
    pt_handler_reg->t_handler_fct_tbl.pf_fopen = pt_fct_tbl->pf_fopen;
    pt_handler_reg->t_handler_fct_tbl.pf_mopen = pt_fct_tbl->pf_mopen;
    pt_handler_reg->t_handler_fct_tbl.pf_close = pt_fct_tbl->pf_close;
    pt_handler_reg->t_handler_fct_tbl.pf_start_indexing = pt_fct_tbl->pf_start_indexing;
    pt_handler_reg->t_handler_fct_tbl.pf_get_info = pt_fct_tbl->pf_get_info;
    pt_handler_reg->t_handler_fct_tbl.pf_set_info = pt_fct_tbl->pf_set_info;
    pt_handler_reg->t_handler_fct_tbl.pf_midxbuld_cli_init
                                            = pt_fct_tbl->pf_midxbuld_cli_init;

    /* add new node */
    SLIST_INSERT_HEAD(pt_handler_reg, &gt_midxbuld_handler_reg_list, t_link);

    return MIDXBULDR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  midxbuld_send_notify_msg
 *
 * Description: 
 *
 * Inputs:  
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 midxbuld_send_notify_msg (MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T* pt_msg)
{
    INT32 i4_ret;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_msg == NULL) 
    {
        return MIDXBULDR_INV_ARG;
    }
    /************************************************************************/
    /* Do send message                                                      */
    /************************************************************************/


    i4_ret = x_msg_q_send( gh_notify_async_exe_msgq,
                       pt_msg,
                       sizeof( MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T ),
                       10 );
    if ( i4_ret != OSR_OK )
    {
        DBG_ERROR(("[midxbuld] gh_notify_async_exe_msgq full\n"));
        return MIDXBULDR_MSG_Q_ERR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: midxbuld_notify_async_exec_thread_main
 *
 * Description: 
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID midxbuld_notify_async_exec_thread_main(VOID* pv_arg)
{
    INT32                               i4_ret;
    SIZE_T                              z_msg_size;  
    UINT16                              ui2_index;
    MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T    t_msg;
    HANDLE_TYPE_T                       e_handle_type;
    VOID*                               pv_obj = NULL;
    MIDXBULD_OBJ_T*                     pt_obj;
    x_midxbuld_nfy_fct                  pf_nfy;
    handler_close_fct                   pf_close;
    handler_start_indexing_fct          pf_start_indexing;

    while(1)
    {
        z_msg_size = sizeof(MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T);
        
        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &gh_notify_async_exe_msgq,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_OK )
        {
            i4_ret = handle_get_type_obj(t_msg.h_midxbuld, &e_handle_type, &pv_obj);
            if ((i4_ret != HR_OK) 
                || ((e_handle_type != HT_MIDXBULD_FILE_TYPE) && (e_handle_type != HT_MIDXBULD_MEM_TYPE))
                || (pv_obj == NULL))
            {
                continue;
            }
            pt_obj = (MIDXBULD_OBJ_T*)pv_obj;
            if (t_msg.e_msg_type == MIDXBULD_MSG_NOTIFY_TYPE)
            {
                pf_nfy = pt_obj->pf_nfy;
                if (pf_nfy)
                {
                    pf_nfy(t_msg.h_midxbuld, t_msg.u.t_nfy_msg.e_event, t_msg.u.t_nfy_msg.pv_data1, t_msg.u.t_nfy_msg.pv_data2, pt_obj->pv_tag);
                }
            }
            else if (t_msg.e_msg_type == MIDXBULD_MSG_ASYNC_EXEC_TYPE)
            {
                switch( t_msg.u.t_async_exec_msg.e_async_exec)
                {
                    case MIDXBULD_ASYNC_EXEC_CLOSE:
                        pf_nfy = pt_obj->pf_nfy;
                        if (pf_nfy)
                        {
                            pf_nfy(t_msg.h_midxbuld, MIDXBULD_NFY_EVENT_HANDLE_CLOSED, NULL, NULL, pt_obj->pv_tag);
                        }
                        /* close handle */
                        pf_close = pt_obj->t_handler_fct_tbl.pf_close;
                        if (pf_close)
                        {
                            pf_close(pv_obj);
                        }
                        x_sema_unlock(pt_obj->h_sync_lock);
                        break;
                    case MIDXBULD_ASYNC_EXEC_START_INDEXING:
                        {
                            UINT32 ui4_range_id = 0;
                        
                             /* start indexing */
                            pf_start_indexing = pt_obj->t_handler_fct_tbl.pf_start_indexing;
                            if (pf_start_indexing)
                            {
                                MUTIL_DBG_LOG_MW_0(("File index build: begin index build! \r\n"));
                                i4_ret = pf_start_indexing(pv_obj, &ui4_range_id);
                                MUTIL_DBG_LOG_MW_0(("File index build: end index build, ret = %d! \r\n",i4_ret));
                            }
                            pf_nfy = pt_obj->pf_nfy;
                            if (pf_nfy)
                            {
                                if (i4_ret == MIDXBULDR_OK)
                                {
                                    MUTIL_DBG_LOG_MW_0(("File index build: index build ok, range id = %d! \r\n",ui4_range_id));
                                    pf_nfy(t_msg.h_midxbuld, MIDXBULD_NFY_EVENT_INDEXING_DONE, (VOID*)ui4_range_id, NULL, pt_obj->pv_tag);
                                }
                                else if (i4_ret == MIDXBULDR_ABORT_INDEXING)
                                {
                                    MUTIL_DBG_LOG_MW_0(("File index build: index build abort! \r\n"));
                                    pf_nfy(t_msg.h_midxbuld, MIDXBULD_NFY_EVENT_INDEXING_ABORT, NULL, NULL, pt_obj->pv_tag);
                                }
                                else
                                {
                                    MUTIL_DBG_LOG_MW_0(("File index build: index build failed! \r\n"));
                                    pf_nfy(t_msg.h_midxbuld, MIDXBULD_NFY_EVENT_ERROR, NULL, NULL, pt_obj->pv_tag);
                                }
                            }
                        }
                        break;
                }
            }
        }
    }
}
/*-----------------------------------------------------------------------------
 * Name:  midxbuld_init
 *
 * Description: This function is a selector's API. This function is called by system initial procedure when system boot-up to init media format recognizer module.
 *
 * Inputs:  NONe.
 * Outputs: NONE.
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 midxbuld_init (void)
{ /*it could be called multiple times when mp3 aac ... recognizer inital*/

    INT32 i4_ret;
    
    if (gb_init == TRUE)
    {
        return MIDXBULDR_OK;
    }

    if (gh_notify_async_exe_msgq == NULL_HANDLE)
    {
        /* Create notify and async execute msg queue */
        i4_ret = x_msg_q_create( &gh_notify_async_exe_msgq,
                                 (CHAR*)MIDXBULD_NFY_ASYNC_EXEC_MSG_Q_NAME,
                                 sizeof( MIDXBULD_NOTIFY_ASYNC_EXEC_MSG_T ),
                                 MIDXBULD_NFY_ASYNC_EXEC_NUM_OF_MSGS );
        if ( i4_ret != OSR_OK )
        {
            return MIDXBULDR_INITIAL_ERR;
        }
    }

    if (gh_notify_async_exe_thread == NULL_HANDLE)
    {
        /* Start image Selector Thread */
        i4_ret = x_thread_create( &gh_notify_async_exe_thread,
                          MIDXBULD_NFY_ASYNC_EXEC_THREAD_NAME,
                          MIDXBULD_NFY_ASYNC_EXEC_THREAD_DEFAULT_STACK_SIZE,
                          MIDXBULD_NFY_ASYNC_EXEC_THREAD_DEFAULT_PRIORITY,
                          &midxbuld_notify_async_exec_thread_main,
                          0,
                          NULL );
        if ( i4_ret != OSR_OK )
        {
            return MIDXBULDR_INITIAL_ERR;
        }
    }

    gb_init = TRUE;
    return MIDXBULDR_OK;
}



