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
 * $RCSfile: minfo.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media info public APIs  
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
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo_cli.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "mutil/mm_util.h"
/*#include "libc/ctype.h"*/
#include "util/x_lnk_list.h"

#ifdef CLI_LVL_ALL
#include "mutil/minfo/minfo_cli.h"
#endif

#else

#include "x_os.h"
#include "x_common.h"
#include "common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "minfo.h"
#include "minfo_dbg.h"
#include "u_minfo.h"
#include "minfo_cli.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "../mm_util.h"
#include "u_dbg.h"

#ifdef CLI_LVL_ALL
#include "minfo_cli.h"
#endif

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
SLIST_T(_MINFO_HANDLER_REG_T)  gt_minfo_handler_reg_list = {NULL};
static BOOL                           gb_init = FALSE;
static HANDLE_T                       gh_notify_async_exe_thread = NULL_HANDLE;
static HANDLE_T                       gh_notify_async_exe_msgq   = NULL_HANDLE;
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  minfo_match_handler
 *
 * Description: This match the media format and retrieve the handler function table.
 *
 * Inputs:  pt_format         specifies sixteen characters of a handler name, and the character is case sensitive.
 *
 * Outputs: pt_handler_fct_tbl retrieve the handler function table which match the media format. 
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 minfo_match_handler (MEDIA_FORMAT_T* pt_format, MINFO_HANDLER_FCT_TBL_T* pt_handler_fct_tbl)
{
    MINFO_HANDLER_REG_T*     pt_reg; 

    SLIST_FOR_EACH(pt_reg, &gt_minfo_handler_reg_list, t_link)
    {
        if ((pt_format->e_media_type == pt_reg->t_favor_media_format.e_media_type)
            && (pt_format->t_media_subtype.u.e_aud_subtype == pt_reg->t_favor_media_format.t_media_subtype.u.e_aud_subtype))
        {
            /*TBD*/
	     /*it might need to invoke is_hdlr confirm function to double check.*/		
	     *pt_handler_fct_tbl = pt_reg->t_handler_fct_tbl;
            return MINFOR_OK;
        }          
    }
    return MINFOR_HANDLER_NOT_FOUND;
}

/*-----------------------------------------------------------------------------
 * Name:  minfo_register_handler
 *
 * Description: This API register the handler.
 *
 * Inputs:  ps_name         specifies sixteen characters of a handler name, and the character is case sensitive.
 *          ps_favor        specify the favor file extension string list, for example "mp3"
 *          pt_fct_tbl      specify the function table of the engine.
 *
 * Outputs: NONE.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/

INT32 minfo_register_handler (  
                            const CHAR*                         ps_name,    
                            const MEDIA_FORMAT_T*               pt_favor_media_format,
                            const MINFO_HANDLER_FCT_TBL_T*      pt_fct_tbl)
{
    MINFO_HANDLER_REG_T* pt_handler_reg = NULL;
    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if ( (ps_name == NULL) 
        || (pt_favor_media_format == NULL) 
        || (pt_fct_tbl == NULL) )
    {
        return MINFOR_INV_ARG;
    }
    
    /************************************************************************/
    /* Perform register handler                                              */
    /************************************************************************/

    /* check if the engine is exist?*/
    SLIST_FOR_EACH(pt_handler_reg, &gt_minfo_handler_reg_list, t_link)
    {
        if (x_strncmp(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN) == 0)
        {
            return MINFOR_NAME_EXIST;
        }
    }

    /* malloc node */
    pt_handler_reg = _mm_util_mem_alloc(sizeof(MINFO_HANDLER_REG_T));
    if (pt_handler_reg == NULL)
    {
        return MINFOR_OUT_OF_MEMORY;
    }

    x_memset(pt_handler_reg, 0, sizeof(MINFO_HANDLER_REG_T));

    /* fill data */
    x_strncpy(pt_handler_reg->s_handler_name, ps_name, MAX_HANDLER_NAME_LEN);
    pt_handler_reg->t_favor_media_format = *pt_favor_media_format;
    
    pt_handler_reg->t_handler_fct_tbl.pf_is_hdlr= pt_fct_tbl->pf_is_hdlr;
    pt_handler_reg->t_handler_fct_tbl.pf_fopen = pt_fct_tbl->pf_fopen;
    pt_handler_reg->t_handler_fct_tbl.pf_mopen = pt_fct_tbl->pf_mopen;
    pt_handler_reg->t_handler_fct_tbl.pf_close = pt_fct_tbl->pf_close;
    pt_handler_reg->t_handler_fct_tbl.pf_get_info = pt_fct_tbl->pf_get_info;
    pt_handler_reg->t_handler_fct_tbl.pf_parse = pt_fct_tbl->pf_parse;
    pt_handler_reg->t_handler_fct_tbl.pf_get_rec_nums = pt_fct_tbl->pf_get_rec_nums;
    pt_handler_reg->t_handler_fct_tbl.pf_minfo_cli_init = pt_fct_tbl->pf_minfo_cli_init;
#if 1/*MKV DivXPlus*/
    if((pt_favor_media_format->e_media_type == MEDIA_TYPE_CONTAINER &&
        pt_favor_media_format->t_media_subtype.u.e_contnr_subtype == MEDIA_CONTNR_SUBTYPE_MATROSKA) ||
       (pt_favor_media_format->e_media_type == MEDIA_TYPE_AUDIO_ES &&
       (pt_favor_media_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP2 ||
        pt_favor_media_format->t_media_subtype.u.e_aud_subtype == MEDIA_AUD_SUBTYPE_MP3)))
    {
        pt_handler_reg->t_handler_fct_tbl.pf_set_info = pt_fct_tbl->pf_set_info;
    }
    else
    {
        pt_handler_reg->t_handler_fct_tbl.pf_set_info = NULL;
    }
#endif

    /* add new node */
    SLIST_INSERT_HEAD(pt_handler_reg, &gt_minfo_handler_reg_list, t_link);

    return MINFOR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  minfo_send_notify_msg
 *
 * Description: 
 *
 * Inputs:  
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 minfo_send_notify_msg (MINFO_NOTIFY_ASYNC_EXEC_MSG_T* pt_msg)
{
    INT32 i4_ret;

    /************************************************************************/
    /* Check parameter                                                      */
    /************************************************************************/
    if (pt_msg == NULL) 
    {
        return MINFOR_INV_ARG;
    }
    /************************************************************************/
    /* Do send message                                                      */
    /************************************************************************/


    i4_ret = x_msg_q_send( gh_notify_async_exe_msgq,
                       pt_msg,
                       sizeof( MINFO_NOTIFY_ASYNC_EXEC_MSG_T ),
                       10 );
    if ( i4_ret != OSR_OK )
    {
        DBG_ERROR(("[minfo] gh_notify_async_exe_msgq full\n"));
        return MINFOR_MSG_Q_ERR;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: minfo_notify_async_exec_thread_main
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
static VOID minfo_notify_async_exec_thread_main( VOID*  pv_arg )
{
    INT32                           i4_ret;
    SIZE_T                          z_msg_size;  
    UINT16                          ui2_index;
    MINFO_NOTIFY_ASYNC_EXEC_MSG_T   t_msg;
    HANDLE_TYPE_T                   e_handle_type;
    VOID*                           pv_obj = NULL;
    MINFO_OBJ_T*                    pt_obj;
    x_minfo_nfy_fct                 pf_nfy;
    handler_parse_fct               pf_parse = NULL;
    handler_close_fct               pf_close = NULL;

    while(1)
    {
        z_msg_size = sizeof(MINFO_NOTIFY_ASYNC_EXEC_MSG_T);
        
        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &gh_notify_async_exe_msgq,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_OK )
        {
            i4_ret = handle_get_type_obj(t_msg.h_minfo, &e_handle_type, &pv_obj);
            if ((i4_ret != HR_OK) 
                || ((e_handle_type != HT_MINFO_FILE_TYPE) && (e_handle_type != HT_MINFO_MEM_TYPE))
                || (pv_obj == NULL))
            {
                continue;
            }
            pt_obj = (MINFO_OBJ_T*)pv_obj;
            if (t_msg.e_msg_type == MINFO_MSG_NOTIFY_TYPE)
            {
                pf_nfy = pt_obj->pf_nfy;
                if (pf_nfy)
                {
                    pf_nfy(t_msg.h_minfo, t_msg.u.t_nfy_msg.e_event, t_msg.u.t_nfy_msg.pv_data1, t_msg.u.t_nfy_msg.pv_data2, pt_obj->pv_tag);
                }
                if (t_msg.u.t_nfy_msg.e_event == MINFO_NFY_EVENT_HANDLE_CLOSED)
                {
                    switch (e_handle_type)
                    {
                        case HT_MINFO_FILE_TYPE:
                        case HT_MINFO_MEM_TYPE:
                            if (pt_obj->h_file)
                            {
#ifndef __NO_FM__
                                x_fm_close(pt_obj->h_file);
#endif
                                pt_obj->h_file = NULL_HANDLE;
                            }
                            pf_close = pt_obj->t_handler_fct_tbl.pf_close;
                            if (pf_close)
                            {
                                pf_close(pv_obj);
                            }
                            x_sema_unlock(pt_obj->h_sync_lock);                            
                            break;
                    }
                }
            }
            else if (t_msg.e_msg_type == MINFO_MSG_ASYNC_EXEC_TYPE)
            {
                if (t_msg.u.t_async_exec_msg.b_start_parsing)
                {
                    pf_parse = pt_obj->t_handler_fct_tbl.pf_parse;
                    if (pf_parse)
                    {                     
                        MUTIL_DBG_LOG_MW_0(("File parsing: begin parse file! \r\n"));
                        i4_ret = pf_parse(pv_obj);
                        MUTIL_DBG_LOG_MW_0(("File parsing: end parse file, ret = %d! \r\n",i4_ret));
                    }
                    if (i4_ret == MINFOR_OK)
                    {
                        /* send parse done notify */
                        t_msg.u.t_nfy_msg.e_event = MINFO_NFY_EVENT_PARSING_DONE;
                        pt_obj->b_parsing_successful = TRUE;                         
                        MUTIL_DBG_LOG_MW_0(("File parsing: parse file ok! \r\n"));
                    }
                    /*else if(i4_ret == MINFOR_MOOV_SIZE_EXCE_MAX)
                    {
                        t_msg.u.t_nfy_msg.e_event = MINFO_NFY_EVENT_OUT_OF_SPEC;
                    }*/
                    else
                    {
                        /* send error msg */
                        t_msg.u.t_nfy_msg.e_event = MINFO_NFY_EVENT_ERROR;
                       
                        MUTIL_DBG_LOG_MW_0(("File parsing: parse file fail! \r\n"));
                    }
                    pt_obj->b_parsing_done = TRUE;
                    t_msg.h_minfo = pt_obj->h_minfo;
                    t_msg.e_msg_type = MINFO_MSG_NOTIFY_TYPE;
                    t_msg.u.t_nfy_msg.pv_data1 = NULL;
                    t_msg.u.t_nfy_msg.pv_data2 = NULL;
                    minfo_send_notify_msg (&t_msg);
                }
            }

        }
    }
}

/*-----------------------------------------------------------------------------
 * Name:  minfo_init
 *
 * Description: This function is a selector's API. This function is called by system initial procedure when system boot-up to init media format recognizer module.
 *
 * Inputs:  NONe.
 * Outputs: NONE.
 *
 * Returns: MINFOR_OK    Success. The routine was successful and the operation has been completed.
 ----------------------------------------------------------------------------*/
INT32 minfo_init (void)
{ /*it could be called multiple times when mp3 aac ... recognizer inital*/

    INT32 i4_ret;
    
    if (gb_init == TRUE)
    {
        return MINFOR_OK;
    }

    if (gh_notify_async_exe_msgq == NULL_HANDLE)
    {
        /* Create notify and async execute msg queue */
        i4_ret = x_msg_q_create( &gh_notify_async_exe_msgq,
                                 (CHAR*)MINFO_NFY_ASYNC_EXEC_MSG_Q_NAME,
                                 sizeof( MINFO_NOTIFY_ASYNC_EXEC_MSG_T ),
                                 MINFO_NFY_ASYNC_EXEC_NUM_OF_MSGS );
        if ( i4_ret != OSR_OK )
        {
            return MINFOR_INITIAL_ERR;
        }
    }

    if (gh_notify_async_exe_thread == NULL_HANDLE)
    {
        /* Start image Selector Thread */
        i4_ret = x_thread_create( &gh_notify_async_exe_thread,
                          MINFO_NFY_ASYNC_EXEC_THREAD_NAME,
                          MINFO_NFY_ASYNC_EXEC_THREAD_DEFAULT_STACK_SIZE,
                          MINFO_NFY_ASYNC_EXEC_THREAD_DEFAULT_PRIORITY,
                          &minfo_notify_async_exec_thread_main,
                          0,
                          NULL );
        if ( i4_ret != OSR_OK )
        {
            return MINFOR_INITIAL_ERR;
        }
    }

    gb_init = TRUE;
    return MINFOR_OK;
}


