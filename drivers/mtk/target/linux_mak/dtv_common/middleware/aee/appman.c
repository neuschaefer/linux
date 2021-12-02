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
 * $RCSfile: appman.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 0772fbcbfc330a3368874e31e29a6d08 $
 *
 * Description: 
 *         This file contains the implementation of the application manager
 *         APIs.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "aee/x_appman.h"
#include "aee/x_aee.h"
#include "aee/_aee.h"
#include "os/inc/x_os.h"
#include "os/msgQ/msgq_lib.h"
#include "dbg/u_dbg.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define APPMAN_MSG_INIT         0
#define APPMAN_MSG_EXIT         1
#define APPMAN_MSG_PAUSE        2
#define APPMAN_MSG_RESUME       3
#define APPMAN_MSG_PROCESS_MSG  4
#ifdef MW_FAST_INIT
#define APPMAN_MSG_POST_INIT    5
#endif

#define APPMAN_MSG_PRIORITY 1


typedef struct
{
    UINT32         ui4_command;
    x_app_nfy_fct  pf_nfy;
    VOID*          pv_tag;
    union
    {
        APP_EXIT_MODE_T  e_exit_mode;
        struct
        {
            UINT32  ui4_type;
            SIZE_T  z_msg_len;
        } prc_msg_t;
    } u;
}   APP_MSG_HEADER_T;

typedef struct
{
    APP_FCT_TBL_T  t_fct_tbl;
    HANDLE_T       h_msg_queue;
    VOID*          pv_msg_buffer;
    SIZE_T         z_msg_size;
    CHAR           s_name[AEE_NAME_MAX_LENGTH + 1];
}   APPMAN_MAIN_ARG_T;


/*-----------------------------------------------------------------------------
 * Name: appman_main
 *
 * Description: This API is the application's main function, implemented as a
 *              message loop.
 *
 * Inputs:  h_app   Contains the application handle.
 *          pv_arg  References the function's argument (see APPMAN_MAIN_ARG_T).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID appman_main (HANDLE_T  h_app,
                         VOID*     pv_arg)
{
    APPMAN_MAIN_ARG_T*  pt_arg;
    APP_FCT_TBL_T*      pt_fct_tbl;
    APP_MSG_HEADER_T*   pt_msg_header;
    HANDLE_T            h_msg_queue;
    BOOL                b_exit;
    BOOL                b_paused;
    BOOL                b_resumed;
    const CHAR * sz_thisApp = NULL;
    
    pt_arg = (APPMAN_MAIN_ARG_T *) pv_arg;
    sz_thisApp = pt_arg->s_name;
    if(sz_thisApp == NULL)
    {
        sz_thisApp = "NULL";
    }
    
    pt_fct_tbl = & pt_arg->t_fct_tbl;

    pt_msg_header = (APP_MSG_HEADER_T *) pt_arg->pv_msg_buffer;
    
    h_msg_queue = pt_arg->h_msg_queue;
    
    b_exit    = FALSE;
    b_paused  = FALSE;
    b_resumed = FALSE;
    
    while (! b_exit)
    {
        SIZE_T       z_size;
        INT32        i4_res;
        UINT16       ui2_index;
        
        z_size = pt_arg->z_msg_size;
        
        i4_res = x_msg_q_receive(& ui2_index,
                                 pt_arg->pv_msg_buffer,
                                 & z_size,
                                 & h_msg_queue,
                                 1,
                                 X_MSGQ_OPTION_WAIT);
        
        if (i4_res == OSR_OK)
        {
            x_app_nfy_fct  pf_nfy;
            VOID*          pv_tag;

            pf_nfy = pt_msg_header->pf_nfy;
            pv_tag = pt_msg_header->pv_tag;
        
            switch (pt_msg_header->ui4_command)
            {
                case APPMAN_MSG_PROCESS_MSG:
                {
                    if ((pf_nfy == NULL)
                        ||
                        ! pf_nfy(h_app, pv_tag, APP_NFY_PRE_PROCESS_MSG))
                    {
                        UINT32  ui4_type;
                        VOID*   pv_msg;
                        SIZE_T  z_msg_len;
                        
                        ui4_type  = pt_msg_header->u.prc_msg_t.ui4_type;
                        z_msg_len = pt_msg_header->u.prc_msg_t.z_msg_len;

                        pv_msg = (z_msg_len != 0) ?
                            (VOID *) (pt_msg_header + 1) :
                            NULL;
                        
                        i4_res = pt_fct_tbl->pf_process_msg(h_app,
                                                            ui4_type,
                                                            pv_msg,
                                                            z_msg_len,
                                                            b_paused);
                        
                        if (pf_nfy != NULL)
                        {
                            pf_nfy(h_app, pv_tag,
                                   (i4_res == AEER_OK) ?
                                   APP_NFY_PROCESS_MSG_OK :
                                   APP_NFY_PROCESS_MSG_FAILED);
                        }
                    }
                }
                break;
                
                case APPMAN_MSG_PAUSE:
                {
                    if (b_paused)
                    {
                        x_dbg_stmt("appman_main(%s) already PAUSED %d\n",sz_thisApp,__LINE__);
                    }
                    else if ((pf_nfy != NULL) &&
                         pf_nfy(h_app, pv_tag, APP_NFY_PRE_PAUSE))
                    {
                        x_dbg_stmt("appman_main(%s) ignore PAUSE %d\n",sz_thisApp,__LINE__);
                    }
                    else
                    {
                        i4_res = pt_fct_tbl->pf_pause(h_app);                        
                        if (pf_nfy != NULL)
                        {
                            pf_nfy(h_app, pv_tag,
                                   (i4_res == AEER_OK) ?
                                   APP_NFY_PAUSE_OK :
                                   APP_NFY_PAUSE_FAILED);
                        }
                        
                        b_paused = (i4_res == AEER_OK);
                        if (b_paused)
                        {
                            b_resumed = FALSE;
                        }
                        else
                        {
                            x_dbg_stmt("appman_main(%s) PAUSE fail %d\n",sz_thisApp,__LINE__);
                        }
                    }                   
                }
                break;
                
                case APPMAN_MSG_RESUME:
                {
                    if (b_resumed)
                    {
                        x_dbg_stmt("appman_main(%s) already RESUME %d\n",sz_thisApp,__LINE__);
                    }
                    else if((pf_nfy != NULL) &&
                        pf_nfy(h_app, pv_tag, APP_NFY_PRE_RESUME))
                    {
                        x_dbg_stmt("appman_main(%s) ignore RESUME %d\n",sz_thisApp,__LINE__);
                    }
                    else
                    {
                        i4_res = pt_fct_tbl->pf_resume(h_app);
                        if (pf_nfy != NULL)
                        {
                            pf_nfy(h_app, pv_tag,
                                   (i4_res == AEER_OK) ?
                                   APP_NFY_RESUME_OK :
                                   APP_NFY_RESUME_FAILED);
                        }
                        
                        b_resumed = (i4_res == AEER_OK);
                        if (b_resumed)
                        {
                            b_paused = FALSE;
                        }
                        else
                        {
                            x_dbg_stmt("appman_main(%s) RESUME fail %d\n",sz_thisApp,__LINE__);
                        }
                    }
                }
                break;
                
                case APPMAN_MSG_EXIT:
                {
                    if ((pf_nfy == NULL)
                        ||
                        ! pf_nfy(h_app, pv_tag, APP_NFY_PRE_EXIT))
                    {
                        APP_EXIT_MODE_T  e_exit_mode;

                        e_exit_mode = pt_msg_header->u.e_exit_mode;
                        
                        i4_res = pt_fct_tbl->pf_exit(h_app, e_exit_mode);
                        
                        if (pf_nfy != NULL)
                        {
                            pf_nfy(h_app, pv_tag,
                                   (i4_res == AEER_OK) ?
                                   APP_NFY_EXIT_OK :
                                   APP_NFY_EXIT_FAILED);
                        }
                        
                        b_exit = (i4_res == AEER_OK);
                    }
                }
                break;

                case APPMAN_MSG_INIT:
                {
                    if (pf_nfy != NULL)
                    {
                        b_exit = pf_nfy(h_app, pv_tag, APP_NFY_PRE_INIT);
                    }
                    
                    if (! b_exit)
                    {
                        i4_res = pt_fct_tbl->pf_init(pt_arg->s_name,
                                                     h_app);
                        
                        if (pf_nfy != NULL)
                        {
                            pf_nfy(h_app, pv_tag,
                                   (i4_res == AEER_OK) ?
                                   APP_NFY_INIT_OK :
                                   APP_NFY_INIT_FAILED);
                        }
                        
                        b_exit = (i4_res != AEER_OK);
                    }
                }
                break;
                
#ifdef MW_FAST_INIT
                case APPMAN_MSG_POST_INIT:
                {
                    if (pf_nfy != NULL)
                    {
                        b_exit = pf_nfy(h_app, pv_tag, APP_NFY_PRE_INIT);
                    }
                    
                    if (! b_exit)
                    {
                        i4_res = pt_fct_tbl->pf_post_init(pt_arg->s_name,
                                                     h_app);
                        
                        if (pf_nfy != NULL)
                        {
                            pf_nfy(h_app, pv_tag,
                                   (i4_res == AEER_OK) ?
                                   APP_NFY_INIT_OK :
                                   APP_NFY_INIT_FAILED);
                        }
                        
                        b_exit = (i4_res != AEER_OK);
                    }
                }
                break;
#endif
                
                default:
                    ABORT(DBG_CAT_MESSAGE, DBG_ABRT_INVALID_MESSAGE);
            }
        }
        else
        {
            b_exit = TRUE;
        }
    }

    x_mem_free(pt_arg->pv_msg_buffer);
    
    x_msg_q_delete(h_msg_queue);
}

/*-----------------------------------------------------------------------------
 * Name: appman_receive_data
 *
 * Description: This API is the application's receive_data function (see
 *              x_aee_send_data), which maps to x_app_send_msg.
 *
 * Inputs:  h_app       Contains the application handle.
 *          ui4_type    Contains the type of data.
 *          pv_data     References the data buffer.
 *          z_data_len  Contains the length (in bytes) of the data buffer.
 *
 * Outputs: -
 *
 * Returns: AEER_OK             Routine successful.
 *          AEER_OUT_OF_MEMORY  Out of memory.
 *          AEER_FAIL           Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 appman_receive_data (HANDLE_T  h_app,
                                  UINT32    ui4_type,
                                  VOID*     pv_data,
                                  SIZE_T    z_data_len)
{
    return x_app_send_msg (h_app,
                           ui4_type,
                           pv_data,
                           z_data_len,
                           NULL,
                           NULL);
}

/*-----------------------------------------------------------------------------
 * Name: appman_send_msg
 *
 * Description: This API sends a message to the application's message loop. The
 *              message is supposed to be already defined when this API is
 *              called.
 *
 * Inputs:  h_app          Contains the application handle.
 *          pt_msg_header  References the message header.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
static INT32 appman_send_msg (HANDLE_T                 h_app,
                              const APP_MSG_HEADER_T*  pt_msg_header)
{
    HANDLE_T  h_msg_queue;
    INT32     i4_res;
    
    i4_res = aee_get_msg_queue(h_app, & h_msg_queue);
    
    if (i4_res == AEER_OK)
    {
        i4_res = x_msg_q_send(h_msg_queue,
                              (VOID *) pt_msg_header,
                              sizeof(*pt_msg_header),
                              APPMAN_MSG_PRIORITY);

        i4_res = (i4_res == OSR_OK) ? AEER_OK : AEER_FAIL;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_app_start
 *
 * Description: This API starts an application. The application's init function
 *              will be called first since this API automatically sends a
 *              INIT message to the application's message loop.
 *
 * Inputs:  pt_fct_tbl  References the application's function table.
 *          pt_desc     References the application's description (resources,
 *                      authorization flags, etc).
 *          ps_name     References the application's name.
 *          pf_nfy      References the notify function.
 *          pv_tag      References the tag passed to the notify function.
 *
 * Outputs: ph_app  Contains the application handle.
 *
 * Returns: AEER_OK       Routine successful.
 *          AEER_INV_ARG  Invalid argument.
 *          AEER_FAIL     Routine failed.
 ----------------------------------------------------------------------------*/
INT32 x_app_start (APP_FCT_TBL_T*     pt_fct_tbl,
                   const APP_DESC_T*  pt_desc,
                   const CHAR*        ps_name,
                   x_app_nfy_fct      pf_nfy,
                   VOID*              pv_tag,
                   HANDLE_T*          ph_app)
{
    INT32  i4_res;

    if ((pt_fct_tbl == NULL)
        || (pt_desc == NULL)
        || (ps_name == NULL)
        || (ph_app == NULL)
        || (pt_fct_tbl->pf_init == NULL)
#ifdef MW_FAST_INIT
     //   || (pt_fct_tbl->pf_post_init == NULL)
#endif
        || (pt_fct_tbl->pf_exit == NULL)
        || (pt_fct_tbl->pf_pause== NULL)
        || (pt_fct_tbl->pf_resume == NULL)
        || (pt_fct_tbl->pf_process_msg == NULL)
        || (pt_desc->ui2_msg_count == 0))
    {
        i4_res = AEER_INV_ARG;
    }
    else
    {
        VOID*   pv_msg_buffer;
        SIZE_T  z_msg_size;
        
        z_msg_size = sizeof(APP_MSG_HEADER_T) + pt_desc->ui2_max_msg_size;
        
        pv_msg_buffer = x_mem_alloc(z_msg_size);

        if (pv_msg_buffer != NULL)
        {
            HANDLE_T  h_msg_queue;

            h_msg_queue = NULL_HANDLE;
            
            i4_res = x_msg_q_create(& h_msg_queue,
                                    ps_name,
                                    z_msg_size,
                                    pt_desc->ui2_msg_count);
            
            if (i4_res == OSR_OK)
            {
                AEE_APP_DESC_T     t_aee_app_desc;
                APPMAN_MAIN_ARG_T  t_arg;
                
                t_arg.t_fct_tbl     = *pt_fct_tbl;
                t_arg.h_msg_queue   = h_msg_queue;
                t_arg.pv_msg_buffer = pv_msg_buffer;
                t_arg.z_msg_size    = z_msg_size;
                
                x_strncpy(t_arg.s_name, ps_name, AEE_NAME_MAX_LENGTH);
                t_arg.s_name[AEE_NAME_MAX_LENGTH] = '\0';
                
                t_aee_app_desc.ui8_flags        = pt_desc->ui8_flags;
                t_aee_app_desc.t_thread_desc    = pt_desc->t_thread_desc;
                t_aee_app_desc.ui4_app_group_id = pt_desc->ui4_app_group_id;
                t_aee_app_desc.ui4_app_id       = pt_desc->ui4_app_id;
                t_aee_app_desc.t_max_res        = pt_desc->t_max_res;
                
                t_aee_app_desc.pf_main         = appman_main;
                t_aee_app_desc.pv_arg          = & t_arg;
                t_aee_app_desc.z_arg_size      = sizeof(t_arg);
                t_aee_app_desc.pf_receive_data = appman_receive_data;
                
                i4_res = x_aee_create(& t_aee_app_desc,
                                      ps_name,
                                      ph_app);
                
                if (i4_res == AEER_OK)
                {
                    APP_MSG_HEADER_T  t_msg;
                    
                    aee_set_msg_queue(*ph_app, h_msg_queue);
                    
                    t_msg.ui4_command = APPMAN_MSG_INIT;
                    t_msg.pf_nfy      = pf_nfy;
                    t_msg.pv_tag      = pv_tag;
                    
                    i4_res = appman_send_msg(*ph_app, & t_msg);
                }
                else
                {
                    x_mem_free(pv_msg_buffer);
                    
                    x_msg_q_delete(h_msg_queue);

                    i4_res = AEER_FAIL;
                }
            }
            else
            {
                x_mem_free(pv_msg_buffer);
                
                i4_res = AEER_FAIL;
            }
        }
        else
        {
            i4_res = AEER_OUT_OF_MEMORY;
        }
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_app_exit
 *
 * Description: This API sends a message to the application for it to exit.
 *
 * Inputs:  h_app        Contains the handle to the application.
 *          e_exit_mode  Contains the exit mode.
 *          pf_nfy       References the notify function.
 *          pv_tag       References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 x_app_exit (HANDLE_T         h_app,
                  APP_EXIT_MODE_T  e_exit_mode,
                  x_app_nfy_fct    pf_nfy,
                  VOID*            pv_tag)
{
    APP_MSG_HEADER_T  t_msg;
    
    t_msg.ui4_command = APPMAN_MSG_EXIT;
    t_msg.pf_nfy      = pf_nfy;
    t_msg.pv_tag      = pv_tag;
    
    t_msg.u.e_exit_mode = e_exit_mode;
    
    return appman_send_msg(h_app, & t_msg);
}

/*-----------------------------------------------------------------------------
 * Name: x_app_pause
 *
 * Description: This API sends a message to the application for it to pause.
 *
 * Inputs:  h_app   Contains the handle to the application.
 *          pf_nfy  References the notify function.
 *          pv_tag  References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 x_app_pause (HANDLE_T       h_app,
                   x_app_nfy_fct  pf_nfy,
                   VOID*          pv_tag)
{
    APP_MSG_HEADER_T  t_msg;
    
    t_msg.ui4_command = APPMAN_MSG_PAUSE;
    t_msg.pf_nfy      = pf_nfy;
    t_msg.pv_tag      = pv_tag;
    
    return appman_send_msg(h_app, & t_msg);
}

#ifdef MW_FAST_INIT
/*-----------------------------------------------------------------------------
 * Name: x_app_post_init
 *
 * Description: This API sends a message to the application for it to post init.
 *
 * Inputs:  h_app   Contains the handle to the application.
 *          pf_nfy  References the notify function.
 *          pv_tag  References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 x_app_post_init (HANDLE_T       h_app,
                   x_app_nfy_fct  pf_nfy,
                   VOID*          pv_tag)
{
    APP_MSG_HEADER_T  t_msg;
    
    t_msg.ui4_command = APPMAN_MSG_POST_INIT;
    t_msg.pf_nfy      = pf_nfy;
    t_msg.pv_tag      = pv_tag;
    
    return appman_send_msg(h_app, & t_msg);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: x_app_resume
 *
 * Description: This API sends a message to the application for it to resume.
 *
 * Inputs:  h_app   Contains the handle to the application.
 *          pf_nfy  References the notify function.
 *          pv_tag  References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 x_app_resume (HANDLE_T       h_app,
                    x_app_nfy_fct  pf_nfy,
                    VOID*          pv_tag)
{
    APP_MSG_HEADER_T  t_msg;
    
    t_msg.ui4_command = APPMAN_MSG_RESUME;
    t_msg.pf_nfy      = pf_nfy;
    t_msg.pv_tag      = pv_tag;
    
    return appman_send_msg(h_app, & t_msg);
}

/*-----------------------------------------------------------------------------
 * Name: x_app_send_msg
 *
 * Description: This API sends a message to the application. The format of the
 *              message is not known by the application manager.
 *
 * Inputs:  h_app      Contains the handle to the application.
 *          ui4_type   Contains the type of the message.
 *          pv_msg     References the message data. Can be NULL.
 *          z_msg_len  Contains the length (in bytes) of the message data.
 *          pf_nfy     References the notify function.
 *          pv_tag     References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK              Routine successful.
 *          AEER_OUT_OF_MEMORY   Out of memory.
 *          AEER_FAIL            Routine failed.
 ----------------------------------------------------------------------------*/
INT32 x_app_send_msg (HANDLE_T       h_app,
                      UINT32         ui4_type,
                      const VOID*    pv_msg,
                      SIZE_T         z_msg_len,
                      x_app_nfy_fct  pf_nfy,
                      VOID*          pv_tag)
{
    HANDLE_T  h_msg_queue;
    INT32     i4_res;
    
    i4_res = aee_get_msg_queue(h_app, & h_msg_queue);
    
    if (i4_res == AEER_OK)
    {
        APP_MSG_HEADER_T  t_msg_header;
        
        t_msg_header.ui4_command = APPMAN_MSG_PROCESS_MSG;
        t_msg_header.pf_nfy      = pf_nfy;
        t_msg_header.pv_tag      = pv_tag;
        
        t_msg_header.u.prc_msg_t.ui4_type  = ui4_type;
        t_msg_header.u.prc_msg_t.z_msg_len = (pv_msg != NULL) ? z_msg_len : 0;
        
        i4_res = msg_q_send_2_parts(h_msg_queue,
                                    (VOID *) & t_msg_header,
                                    sizeof(t_msg_header),
                                    pv_msg,
                                    z_msg_len,
                                    APPMAN_MSG_PRIORITY);
        
        i4_res = (i4_res == OSR_OK) ? AEER_OK : AEER_FAIL;
    }
    
    return i4_res;
}
