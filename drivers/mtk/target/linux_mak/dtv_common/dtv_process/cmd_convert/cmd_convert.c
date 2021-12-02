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
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $SWAuthor: Jun Zuo $
 *
 * Description: 
 *         This file contains all driver startup implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "x_dbg_drv.h"
#include "x_dbg.h"
#include "cmd_convert.h"
#include "app_man_parse.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
static _CMDCVT_DUP_INFO t_cmdcvt_info;

static INT32 g_param =0;

pthread_mutex_t g_sendcmd_mutex = PTHREAD_MUTEX_INITIALIZER ; 
typedef struct _CMD_COND_T
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    pthread_condattr_t cond_condattr;
    pthread_cond_t shutdown_cond;
    pthread_t  t_stdn_thread;
    BOOL b_shutdown_end;
} CMD_COND_T;
static CMD_COND_T t_pthread_cond;
static cmd_convert_nfy pf_cmd_convert_recv_cmd = NULL;
/*----------------------------------------------------------------------------
 * Name: _cmd_convert_read_stdin
 *
 * Description:
 *       Read standard in data
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: 0 : success
 ----------------------------------------------------------------------------*/
 /* TODO: Enhance t_cmdcvt_info.ps_rdin_info */
static INT32 _cmd_convert_read_stdin(VOID)
{
    CHAR   *ps_beg = NULL;  
    CHAR   *ps_end = NULL;   
    INT32  i4_eof  = 0; /* Set 1 is end. */
    INT32  i4_read_in_len              = 0;   
    CHAR   s_stand_inp_buf[CMDCVT_READ_DATA_LEN] = {0};

    if ((i4_read_in_len = read (t_cmdcvt_info.i4_fdin_dup, &(s_stand_inp_buf [0]), (CMDCVT_READ_DATA_LEN - 1))) > 0)
    {
        if((s_stand_inp_buf [0] == '\n')&&(i4_read_in_len == 1)) /* Only one char '\n' */
        {
            snprintf(t_cmdcvt_info.ps_rdin_info, 2, "\n");
            return i4_eof;
        }
        
        ps_beg = &(s_stand_inp_buf [0]);
        while ((ps_end = strchr (ps_beg, '\n')) != NULL)
        {
            (*ps_end) = '\0';

            t_cmdcvt_info.ps_rdin_info = strncat (t_cmdcvt_info.ps_rdin_info, ps_beg, strlen(ps_beg));
            
#if CMDCVT_DBG_ENABLE 
            write (t_cmdcvt_info.i4_fdout_dup, "[_cmd_convert_read_stdin]:t_cmdcvt_info.ps_rdin_info = ", strlen("[_cmd_convert_read_stdin]:t_cmdcvt_info.ps_rdin_info = "));
            write (t_cmdcvt_info.i4_fdout_dup, t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
            write (t_cmdcvt_info.i4_fdout_dup, "\n", 1);
#endif

            if (pf_cmd_convert_recv_cmd != NULL)
            {
                pf_cmd_convert_recv_cmd();
            }
/*            t_cmdcvt_info.ps_rdin_info[0] = '\0';*/

            /* Find the beginning of the next line. */
            ps_beg = ps_end + 1;
            while ((ps_beg < &(s_stand_inp_buf [i4_read_in_len])) &&
                   ((*ps_beg) == '\0'))
            {
                ps_beg++;
            }
            t_cmdcvt_info.ps_rdin_info[0] = '\0';
        }

        t_cmdcvt_info.ps_rdin_info = strncat (t_cmdcvt_info.ps_rdin_info, ps_beg, strlen(ps_beg));


    }
    else
    {
        i4_eof = 1;

        if (strlen (t_cmdcvt_info.ps_rdin_info) > 0)
        {
            t_cmdcvt_info.ps_rdin_info[0] = '\0';
        }
    }

    return i4_eof;

}

/*----------------------------------------------------------------------------
 * Name: _cmd_convert_dup_process
 *
 * Description:
 *       process standout and standerr dup info
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: 0 : success -1: failed
 ----------------------------------------------------------------------------*/
static INT32 _cmd_convert_dup_process(INT32 i4_pipe_fd, INT32 i4_fd_dup, CHAR * ps_info )
{
    int  i4_eof  = 0; 				/* Set 1 is end. */
    int  i4_read_out_len = 0;   
    char   s_stand_outp_buf[CMDCVT_READ_DATA_LEN] = {0}; 
    if ((i4_read_out_len = read (i4_pipe_fd, &(s_stand_outp_buf [0]), (CMDCVT_READ_DATA_LEN - 1))) > 0)
    {       
    	pthread_mutex_lock(&g_sendcmd_mutex); 
        write (i4_fd_dup,s_stand_outp_buf,i4_read_out_len); 
        pthread_mutex_unlock(&g_sendcmd_mutex);            
    }
    else
    {
        i4_eof = 1;
        if (strlen (ps_info) > 0)
        {
            ps_info[0] = '\0';
        }
    }
    return i4_eof;
}

/*return : -1 --> error , 0 --> not care, 
1 --> pre-shutdown from am, 2 --> pre-shutdwon from dtv_mtk_app */
static INT32 _cmd_convert_cmd_is_shutdown(CHAR* ps_cmd)
{
    AMP_CMD_T * pt_amp_cmd = NULL;
    CHAR * s_keyword = NULL;
    CHAR * s_value = NULL;
    INT32 i4_ret = -1;
     /* 0 --> not care, 1 --> pre-shutdown from am, 2 --> pre-shutdwon from dtv_mtk_app */
    INT32 i4_shundown_status = 0; 
    
    i4_ret = amp_parse_cmd (ps_cmd, &pt_amp_cmd);

    if((pt_amp_cmd == NULL)&&(i4_ret != 1))
    {
        CMDCVT_DBG_INFO("\n Error in %s, %d\n",__FUNCTION__,__LINE__);
        return CMDCVT_AMBR_FAIL;
    }

    switch(i4_ret)
    {
        case 0:
            if(pt_amp_cmd->pt_data_field != NULL)
            {
                if(pt_amp_cmd->pt_data_field->pt_data_field_node_list_head != NULL)
                {
                    s_keyword = pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->s_keyword;
                }
            }   
            if(pt_amp_cmd->pt_data_field != NULL)
            {
                if(pt_amp_cmd->pt_data_field->pt_data_field_node_list_head != NULL)
                {
                    if(pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value != NULL)
                    {
                        if(pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head != NULL)
                        {
                            s_value = pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head->s_value;
                        }   
                    }   
                }   
            }   
            if (s_keyword == NULL)
            {
                i4_shundown_status = 0; /* some error ignore */
                break;
            }
            if(strcmp(pt_amp_cmd->s_src, CMDCVT_MACRO_AM) == 0) 
            {
                if(strcmp(s_keyword, CMDCVT_MACRO_SHTDN) == 0) /* should be set as input */
                {
                    i4_shundown_status = 1;
                    break;
                }   
            }   
            if(strcmp(pt_amp_cmd->s_src, CMDCVT_MACRO_AP_DTVCTX) == 0) 
            {
                if(strcmp(s_keyword, CMDCVT_MACRO_SHTDN) == 0) /* should be set as output */
                {
                    i4_shundown_status = 2;
                    break;
                }   
            }   
            

            break;
        default:
            i4_shundown_status = 0; /* ignore */
            break;
    }
    
    if(pt_amp_cmd != NULL)
    {
        amp_free(pt_amp_cmd);
    }

    return i4_shundown_status;
    
}   

static VOID* _cmd_convert_wait_cond(VOID * p_arg)
{
    struct timespec to;
    INT32 i4_ret = 0; 

    clock_gettime(CLOCK_MONOTONIC, &to);
    to.tv_sec += CMDCVT_TIMEOUT_4_SHUTDOWN;

    if (pthread_mutex_lock(&t_pthread_cond.mutex) != 0)
    {
        CMDCVT_DBG_INFO("_cmd_convert_wait_cond failed to acquire mutex \n");
        return NULL;
    }

    i4_ret = pthread_cond_timedwait(&t_pthread_cond.cond, &t_pthread_cond.mutex, &to);
    if(i4_ret == ETIMEDOUT)
    {
        cmd_convert_send_cmd(":dtv_app_mtk,dtv_app_mtk,:shutdowned=3rd_party\n");
    }
    
    t_pthread_cond.b_shutdown_end = TRUE;

    pthread_cond_signal(&t_pthread_cond.shutdown_cond);

    if (pthread_mutex_unlock(&t_pthread_cond.mutex) != 0)
    {
        CMDCVT_DBG_INFO("_cmd_convert_wait_cond failed to unlock mutex \n");
        return NULL;
    }
        
    return NULL;
}   

static VOID _cmd_convert_trigger_shutdown(VOID)
{
    if (pthread_create(&t_pthread_cond.t_stdn_thread, NULL, _cmd_convert_wait_cond, NULL) != 0) 
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: Fail to create pthread !!!\n\n");
    }
}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_monitor
 *
 * Description:
 *       Read standard in data
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: 0 : success
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_monitor(VOID)
{
    int    i4_num_fd = 0;
    fd_set t_fd_rd;
    int   i4_done;
    int   i4_status;
    struct timeval timeout;

    timeout.tv_sec = 0; /* second */
    timeout.tv_usec = 100000 ; /* Microseconds */

#if 0
    i4_done = 0;

    while (! (i4_done))
#endif    
    {
        /* Set the select bitmask. */
        FD_ZERO (&t_fd_rd);

        if(t_cmdcvt_info.i4_fdin_dup >= 0)
        {
            FD_SET (t_cmdcvt_info.i4_fdin_dup, &t_fd_rd);
        }
        i4_num_fd = t_cmdcvt_info.i4_fdin_dup;

        if(t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD] >= 0)
        {
            FD_SET (t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD], &t_fd_rd);
        }
        if(t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD] >= 0)
        {
            FD_SET (t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD], &t_fd_rd);
        }

        if (i4_num_fd < t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD])
        {
            i4_num_fd = t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD];
        }
        if (i4_num_fd < t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD])
        {
            i4_num_fd = t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD];
        }
        /* Wait for some data on the pipes */
        /* and then process the data.      */
        if ((i4_status = select ((i4_num_fd + 1), &t_fd_rd, NULL, NULL, NULL/*&timeout*/)) > 0)
        {
#if CMDCVT_DBG_ENABLE 
            write (t_cmdcvt_info.i4_fdout_dup, "[monitor_1]:[t_cmdcvt_info.ps_rdin_info=", strlen("[monitor_1]:[t_cmdcvt_info.ps_rdin_info="));
            write (t_cmdcvt_info.i4_fdout_dup, t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
            write (t_cmdcvt_info.i4_fdout_dup, "]\n", 2);
#endif        
            if ((t_cmdcvt_info.i4_fdin_dup >= 0) &&
                (FD_ISSET (t_cmdcvt_info.i4_fdin_dup, &t_fd_rd)))
            {
                /* Data is available on the    */
                /* 't_cmdcvt_info.i4_fdin_dup' */
                /* pipe. Process it.           */

                if(0 != _cmd_convert_read_stdin())
                {
                    close (t_cmdcvt_info.i4_fdin_dup);

                    t_cmdcvt_info.i4_fdin_dup = -1;
                }
            }

            if ((t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD] >= 0) &&
                (FD_ISSET (t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD], &t_fd_rd)))
            {
                /* Data is available on the    */
                /* 't_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD]' */
                /* pipe. Process it.           */
                if(0 != _cmd_convert_dup_process(t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD],
                                t_cmdcvt_info.i4_fdout_dup,
                                t_cmdcvt_info.ps_rdout_info))
                {
                    close (t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD]);

                    t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD] = -1;
                }
            }
            
            if ((t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD] >=0) &&
                (FD_ISSET (t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD], &t_fd_rd)))
            {
                /* Data is available on the    */
                /* 't_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD]' */
                /* pipe. Process it.           */
                if(0 != _cmd_convert_dup_process(t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD],
                                t_cmdcvt_info.i4_fderr_dup,
                                t_cmdcvt_info.ps_rderr_info))
                {
                    close (t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD]);

                    t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD] = -1;
                }
            }

#if CMDCVT_DBG_ENABLE 
            write (t_cmdcvt_info.i4_fdout_dup, "[monitor_2]:[t_cmdcvt_info.ps_rdin_info=", strlen("[monitor_1]:[t_cmdcvt_info.ps_rdin_info="));
            write (t_cmdcvt_info.i4_fdout_dup, t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
            write (t_cmdcvt_info.i4_fdout_dup, "]\n", 2);
#endif

#if 0            
            /* If the 'stdout' and 'stderr' pipes are closed */
            /* then we can also terminate this loop.         */
            i4_done = ((pt_node_info->t_pipe_set.ai4_stdout_pipe [PIPE_RD] < 0)  &&
                      (pt_node_info->t_pipe_set.ai4_stderr_pipe [PIPE_RD] < 0))? 1 : 0;
#endif                      

        }
        else
        {
            i4_done = (i4_status < 0)? 1 : 0;
        }
    }

    return CMDCVT_AMBR_OK;

}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_monitor_timeout
 *
 * Description:
 *       Read standard in data
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: 0 : success
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_monitor_timeout(VOID)
{
    int    i4_num_fd = 0;
    fd_set t_fd_rd;
    int   i4_done;
    int   i4_status;
    struct timeval timeout;

    timeout.tv_sec = 0; /* second */
    timeout.tv_usec = 10000 ; /* Microseconds */

#if 0
    i4_done = 0;

    while (! (i4_done))
#endif    
    {
        /* Set the select bitmask. */
        FD_ZERO (&t_fd_rd);

        if(t_cmdcvt_info.i4_fdin_dup >= 0)
        {
            FD_SET (t_cmdcvt_info.i4_fdin_dup, &t_fd_rd);
        }
        i4_num_fd = t_cmdcvt_info.i4_fdin_dup;

        if(t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD] >= 0)
        {
            FD_SET (t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD], &t_fd_rd);
        }
        if(t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD] >= 0)
        {
            FD_SET (t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD], &t_fd_rd);
        }

        if (i4_num_fd < t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD])
        {
            i4_num_fd = t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD];
        }
        if (i4_num_fd < t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD])
        {
            i4_num_fd = t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD];
        }
        /* Wait for some data on the pipes */
        /* and then process the data.      */
        if ((i4_status = select ((i4_num_fd + 1), &t_fd_rd, NULL, NULL, &timeout)) > 0)
        {
#if CMDCVT_DBG_ENABLE 
            write (t_cmdcvt_info.i4_fdout_dup, "[monitor_1]:[t_cmdcvt_info.ps_rdin_info=", strlen("[monitor_1]:[t_cmdcvt_info.ps_rdin_info="));
            write (t_cmdcvt_info.i4_fdout_dup, t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
            write (t_cmdcvt_info.i4_fdout_dup, "]\n", 2);
#endif        
            if ((t_cmdcvt_info.i4_fdin_dup >= 0) &&
                (FD_ISSET (t_cmdcvt_info.i4_fdin_dup, &t_fd_rd)))
            {
                /* Data is available on the    */
                /* 't_cmdcvt_info.i4_fdin_dup' */
                /* pipe. Process it.           */
                if(0 != _cmd_convert_read_stdin())
                {
                    close (t_cmdcvt_info.i4_fdin_dup);

                    t_cmdcvt_info.i4_fdin_dup = -1;
                }
            }

            if ((t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD] >= 0) &&
                (FD_ISSET (t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD], &t_fd_rd)))
            {
                /* Data is available on the    */
                /* 't_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD]' */
                /* pipe. Process it.           */
                if(0 != _cmd_convert_dup_process(t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD],
                                t_cmdcvt_info.i4_fdout_dup,
                                t_cmdcvt_info.ps_rdout_info))
                {
                    close (t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD]);

                    t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD] = -1;
                }
            }
            
            if ((t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD] >=0) &&
                (FD_ISSET (t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD], &t_fd_rd)))
            {
                /* Data is available on the    */
                /* 't_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_RD]' */
                /* pipe. Process it.           */
                if(0 != _cmd_convert_dup_process(t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD],
                                t_cmdcvt_info.i4_fderr_dup,
                                t_cmdcvt_info.ps_rderr_info))
                {
                    close (t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD]);

                    t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_RD] = -1;
                }
            }

#if CMDCVT_DBG_ENABLE 
            write (t_cmdcvt_info.i4_fdout_dup, "[monitor_2]:[t_cmdcvt_info.ps_rdin_info=", strlen("[monitor_1]:[t_cmdcvt_info.ps_rdin_info="));
            write (t_cmdcvt_info.i4_fdout_dup, t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
            write (t_cmdcvt_info.i4_fdout_dup, "]\n", 2);
#endif

#if 0            
            /* If the 'stdout' and 'stderr' pipes are closed */
            /* then we can also terminate this loop.         */
            i4_done = ((pt_node_info->t_pipe_set.ai4_stdout_pipe [PIPE_RD] < 0)  &&
                      (pt_node_info->t_pipe_set.ai4_stderr_pipe [PIPE_RD] < 0))? 1 : 0;
#endif                      

        }
        else
        {
            if (i4_status == 0) /*timeout*/
            {
                return CMDCVT_MSG_TIMEOUT;    
            }
            i4_done = (i4_status < 0)? 1 : 0;
        }
    }

    return CMDCVT_MSG_OK;

}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_reset_buf
 *
 * Description:
 *       Parse the message: is  cli or app cmd
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: CMDCVT_MSG_CLI_CMD, cli cmd
 *          CMDCVT_MSG_APP_CMD, app cmd
 *          -1, error parse
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_reset_buf(VOID)
{
    /* clear stdin buff */
    if (t_cmdcvt_info.ps_rdin_info != NULL)
    {
        t_cmdcvt_info.ps_rdin_info[0] = '\0';
    }

    return CMDCVT_AMBR_OK;
}


/*----------------------------------------------------------------------------
 * Name: cmd_convert_parse
 *
 * Description:
 *       Parse the message: is  cli or app cmd
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: CMDCVT_MSG_CLI_CMD, cli cmd
 *          CMDCVT_MSG_APP_CMD, app cmd
 *          -1, error parse
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_parse(VOID)
{
    INT32 i4_ret = -1;
    INT32 i4_cmd_type = -1;
    AMP_CMD_T * pt_amp_cmd = NULL;
    CHAR * s_src;
    CHAR * s_dst;
    
#if CMDCVT_DBG_ENABLE 
    write (t_cmdcvt_info.i4_fdout_dup, "[cmd_convert_parse]:[t_cmdcvt_info.ps_rdin_info=", strlen("[cmd_convert_parse]:[t_cmdcvt_info.ps_rdin_info="));
    write (t_cmdcvt_info.i4_fdout_dup, t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
    write (t_cmdcvt_info.i4_fdout_dup, "]\n", 2);
#endif

    i4_ret = amp_parse_cmd (&t_cmdcvt_info.ps_rdin_info[0], &pt_amp_cmd);

    if((pt_amp_cmd == NULL)&&(i4_ret != 1))
    {
        return CMDCVT_AMBR_FAIL;
    }
    
    switch(i4_ret)
    {
    case 0:
    {
        s_src = pt_amp_cmd->s_src;
        s_dst = pt_amp_cmd->s_dst;
        if((strcmp(s_src, CMDCVT_MACRO_AM)== 0)&&(strcmp(s_dst, CMDCVT_MACRO_CLI)== 0))
        {
            i4_cmd_type = CMDCVT_MSG_CLI_CMD; /* cli cmd */
        }
        else if(strcmp(s_src, CMDCVT_MACRO_AM)== 0)
        {
            if((pt_amp_cmd->pt_data_field != NULL)&&
                (pt_amp_cmd->pt_data_field->pt_data_field_node_list_head != NULL)&&
                (pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value != NULL)&&
                (pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head != NULL))
            {
                i4_cmd_type = CMDCVT_MSG_APP_CMD; /* ap msg */
            }
        }
        else
        {
        }
        
        if(pt_amp_cmd != NULL)
        {
            amp_free(pt_amp_cmd);
        }
        return i4_cmd_type;
    }
        break;
    case 1: /* s_cmd_str would not be parsed and pass to stdin directly */
    {
        if(t_cmdcvt_info.ps_rdin_info[0] == '\n')
        {
#if CMDCVT_DBG_ENABLE 
            write (t_cmdcvt_info.i4_fdout_dup, "[cmd_convert_parse]:[input only one char '\n'] \n", strlen("[cmd_convert_parse]:[input only one char '\n'] \n"));
#endif
            write(t_cmdcvt_info.i4_pipe_stdin_fd[CMDCVT_PIPE_WR], "\n", 1);
        }
        else if(t_cmdcvt_info.ps_rdin_info[0] != 0)
        {
#if CMDCVT_DBG_ENABLE 
            write (t_cmdcvt_info.i4_fdout_dup, "[cmd_convert_parse]:[cli un-regonize info....] \n", strlen("[cmd_convert_parse]:[cli un-regonize info....] \n"));
#endif
            write(t_cmdcvt_info.i4_pipe_stdin_fd[CMDCVT_PIPE_WR], t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
            write(t_cmdcvt_info.i4_pipe_stdin_fd[CMDCVT_PIPE_WR], "\n", 1);
        }
        i4_ret = -1; /* not cli or ap std cmd */
    }
        break;
    case -1:
    case -2:
    default:
        break;
    }

    if(pt_amp_cmd != NULL)
    {
        amp_free(pt_amp_cmd);
    }
    return i4_ret;
}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_parse_ap_cmd
 *
 * Description:
 *       Parse app message to each components.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_parse_ap_cmd(VOID)
{
    CMDCVT_APP_MSG_TYPE_T e_cmdcvt_msg = -1;
    AMP_CMD_T * pt_amp_cmd = NULL;
    CHAR * s_keyword = NULL;
    CHAR * s_value = NULL;
    INT32 i4_ret = -1;
    
    i4_ret = amp_parse_cmd (&t_cmdcvt_info.ps_rdin_info[0], &pt_amp_cmd);

    if((pt_amp_cmd == NULL)&&(i4_ret != 1))
    {
        CMDCVT_DBG_INFO("\n Error in %s, %d\n",__FUNCTION__,__LINE__);
        return CMDCVT_AMBR_FAIL;
    }

    switch(i4_ret)
    {
    case 0:
    {
        if ((pt_amp_cmd->pt_data_field->pt_data_field_node_list_head != NULL)&&
            (pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->s_keyword != NULL))
        {            
            
            s_keyword = pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->s_keyword;
            if ((pt_amp_cmd->pt_data_field->pt_data_field_node_list_head!= NULL)&&
                (pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value!= NULL))
            {    
                s_value = pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head->s_value;
            }
            do{
                if(strcmp(s_keyword, CMDCVT_MACRO_START) == 0)
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value, CMDCVT_MACRO_MENU) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MENU_START;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NAV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NAV_START;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_EPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_EPG_START;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_WZD) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_WZD_START;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_MMP) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MMP_START;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_DTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_DTV_START;
                    }
#ifdef NETTV_SUPPORT
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_START;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_IPEPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_IPEPG_START;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_REDBTN) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_REDBTN_START;
                    }
#endif
                }
                else if(strcmp(s_keyword, CMDCVT_MACRO_STOP) == 0)
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value, CMDCVT_MACRO_MENU) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MENU_STOP;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NAV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NAV_STOP;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_EPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_EPG_STOP;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_WZD) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_WZD_STOP;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_MMP) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MMP_STOP;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_DTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_DTV_STOP;
                    }
#ifdef NETTV_SUPPORT
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_STOP;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_IPEPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_IPEPG_STOP;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_REDBTN) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_REDBTN_STOP;
                    }
#endif
                }
                else if(strcmp(s_keyword, CMDCVT_MACRO_RESUME) == 0)
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value, CMDCVT_MACRO_MENU) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MENU_RESUME;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NAV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NAV_RESUME;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_EPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_EPG_RESUME;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_WZD) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_WZD_RESUME;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_MMP) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MMP_RESUME;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_DTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_DTV_RESUME;
                    }
#ifdef NETTV_SUPPORT
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_RESUME;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_IPEPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_IPEPG_RESUME;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_REDBTN) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_REDBTN_RESUME;
                    }
#endif
                }
                else if(strcmp(s_keyword, CMDCVT_MACRO_PAUSE) == 0)
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value, CMDCVT_MACRO_MENU) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MENU_PAUSE;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NAV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NAV_PAUSE;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_EPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_EPG_PAUSE;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_WZD) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_WZD_PAUSE;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_MMP) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MMP_PAUSE;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_DTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_DTV_PAUSE;
                    }
#ifdef NETTV_SUPPORT
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_PAUSE;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_IPEPG) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_IPEPG_PAUSE;
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_NETTV_REDBTN) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_NETTV_REDBTN_PAUSE;
                    }
#endif
                }
            
                else if(strcmp(s_keyword, CMDCVT_MACRO_BTN) == 0)
                {      
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value, CMDCVT_MACRO_VOLUME_UP) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_VOLUME_UP;                
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_VOLUME_DOWN) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_VOLUME_DOWN;                
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_MUTE) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_MUTE;                
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_CHANNEL_UP) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_CHANNEL_UP;                
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_CHANNEL_DOWN) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_CHANNEL_DOWN;                
                    }
                    else if(strcmp(s_value, CMDCVT_MACRO_CHANNEL_PRE) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_CHANNEL_PRE;                
                    }
                }
                else if(strcmp(s_keyword, CMDCVT_MACRO_VIEWPORTMODE) == 0)
                {
                    e_cmdcvt_msg = CMDCVT_MSG_APCMD_VIEWPORT_CHANGE;
                }
                else if (strcmp(s_keyword, CMDCVT_MACRO_STATUS) == 0)
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    g_param = atol(s_value);
                    e_cmdcvt_msg = CMDCVT_MSG_APCMD_STATUS ;
                }
                else if(strcmp(s_keyword, CMDCVT_MACRO_SHTDNED) == 0)
                {
#if 0           
                    c_dbg_stmt("\n  ---- Function : %s, Line : %d , s_value = %s-----  \n", __FUNCTION__,__LINE__,s_value);
                    if(strcmp(s_value, CMDCVT_MACRO_3DAP) == 0)
                    {
                        c_dbg_stmt("\n  ---- Function : %s, Line : %d -----  \n", __FUNCTION__,__LINE__);
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_3DAP_SHTDN;
                    }
#else
                    if(strcmp(pt_amp_cmd->s_dst,CMDCVT_MACRO_AP_DTVCTX) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_3DAP_SHTDN;
                    }   
#endif
                }   
                else if(strcmp(s_keyword, CMDCVT_MACRO_SHTDN) == 0) /* for test */
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value, CMDCVT_MACRO_3DAP) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_3DAP_PRE_SHTDN;
                    }
                }   
                else if(strcmp(s_keyword,CMDCVT_MACRO_TVMODE) == 0)
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value,CMDCVT_MACRO_VIDEOAUDIO) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_TVMODE_VIDEOAUDIO;
                    }
                    else if(strcmp(s_value,CMDCVT_MACRO_VIDEOONLY) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_TVMODE_VIDEOONLY;
                    }
                }
                else if(strcmp(s_keyword,CMDCVT_MARCO_GINGA) == 0)
                {
                    if (s_value == NULL)
                    {
                        break;
                    }
                    if(strcmp(s_value,CMDCVT_MARCO_GINGA_APP_LIST) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_GINGA_SHOW_APP_LIST;
                    }
                    else if(strcmp(s_value,CMDCVT_MARCO_GINGA_SC) == 0)
                    {
                        e_cmdcvt_msg = CMDCVT_MSG_APCMD_GINGA_SHOW_STICKER;
                    }
                }
                else if(strcmp(s_keyword,CMDCVT_MACRO_WAITNAVKEY) == 0)
                {
                    e_cmdcvt_msg = CMDCVT_MSG_APCMD_WAITNAVKEY;
                }
                else if(strcmp(s_keyword,CMDCVT_MACRO_WAITNAVACK) == 0)
                {
                    e_cmdcvt_msg = CMDCVT_MSG_APCMD_WAITNAVACK;
                }
                else if(strcmp(s_keyword,CMDCVT_MACRO_GET) == 0)
                {
                    e_cmdcvt_msg = CMDCVT_MSG_APCMD_GET;
                }
                else if(strcmp(s_keyword,CMDCVT_MACRO_VISIBLE_CHECK) == 0)
                {
                    e_cmdcvt_msg = CMDCVT_MSG_APCMD_VISIBLE_CHECK;
                }                     
            }while(0);
        }
        if(pt_amp_cmd != NULL)
        {
            amp_free(pt_amp_cmd);
        }
        return (INT32)e_cmdcvt_msg;
        
    }
        break;
    case 1: /* s_cmd_str would not be parsed */
        i4_ret = -1;
        break;
    case -1:
    case -2:
    default:
        break;
    }

    if(pt_amp_cmd != NULL)
    {
        amp_free(pt_amp_cmd);
    }
    return i4_ret;
}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_parse_cli_cmd
 *
 * Description:
 *       Sent cli cmd to mw cli library.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_parse_cli_cmd(VOID)
{
    AMP_CMD_T * pt_amp_cmd = NULL;
    CHAR * s_keyword;
    INT32 i4_ret = -1;
    
#if CMDCVT_DBG_ENABLE 
    write (t_cmdcvt_info.i4_fdout_dup, "[cmd_convert_parse_cli_cmd]:[t_cmdcvt_info.ps_rdin_info=", strlen("[cmd_convert_parse_cli_cmd]:[t_cmdcvt_info.ps_rdin_info="));
    write (t_cmdcvt_info.i4_fdout_dup, t_cmdcvt_info.ps_rdin_info, strlen(t_cmdcvt_info.ps_rdin_info));
    write (t_cmdcvt_info.i4_fdout_dup, "]\n", 2);
#endif

    i4_ret = amp_parse_cmd (&t_cmdcvt_info.ps_rdin_info[0], &pt_amp_cmd);

    if(pt_amp_cmd == NULL)
    {
        CMDCVT_DBG_INFO("\n Error in %s, %d\n",__FUNCTION__,__LINE__);
        return CMDCVT_AMBR_FAIL;
    }

    switch(i4_ret)
    {
    case 0:
    {
        if((pt_amp_cmd->pt_data_field != NULL)&&(pt_amp_cmd->pt_data_field->pt_data_field_node_list_head != NULL))
        {
            s_keyword = pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->s_keyword;

            write(t_cmdcvt_info.i4_pipe_stdin_fd[CMDCVT_PIPE_WR], s_keyword, strlen(s_keyword));
        }

        write(t_cmdcvt_info.i4_pipe_stdin_fd[CMDCVT_PIPE_WR], "\n", 1);
    }
        break;
    case 1: /* s_cmd_str would not be parsed */
        i4_ret = -1;
        break;
    case -1:
    case -2:
    default:
        break;
    }

    if(pt_amp_cmd != NULL)
    {
        amp_free(pt_amp_cmd);
    }
    
    return i4_ret;

}

INT32 cmd_convert_send_cmd(CHAR* ps_cmd)
{
    INT32 i4_ret = -1;
    if (ps_cmd != NULL)    
    {
        i4_ret = _cmd_convert_cmd_is_shutdown(ps_cmd);
        switch (i4_ret)
        {
            case 1: /* should send to stdin , seems the cmd from am */
            	write(CMDCVT_IO_STDIN, "\n",strlen("\n")); 
                write(CMDCVT_IO_STDIN, ps_cmd, strlen(ps_cmd)); 
                break;
            case 2: /* should send to stdout */
                t_pthread_cond.b_shutdown_end = FALSE;
                _cmd_convert_trigger_shutdown();
                pthread_mutex_lock(&g_sendcmd_mutex);   
                write(t_cmdcvt_info.i4_fdout_dup,"\n",strlen("\n"));
                write(t_cmdcvt_info.i4_fdout_dup, ps_cmd, strlen(ps_cmd));                    
                pthread_mutex_unlock(&g_sendcmd_mutex);
                break;
            case -1: 
            case 0:
            default:
            	pthread_mutex_lock(&g_sendcmd_mutex);
            	write(t_cmdcvt_info.i4_fdout_dup,"\n",strlen("\n"));
                write(t_cmdcvt_info.i4_fdout_dup, ps_cmd, strlen(ps_cmd)); 
                pthread_mutex_unlock(&g_sendcmd_mutex);
                break;
        }   
    }
    return CMDCVT_AMBR_OK;
}

INT32 cmd_convert_send_shtdn_signal(VOID)
{
    if (pthread_mutex_lock(&t_pthread_cond.mutex) != 0)
    {
        printf("_cmd_convert_wait_cond failed to acquire mutex \n");
        return CMDCVT_AMBR_FAIL;  
    }
    
    pthread_cond_signal(&t_pthread_cond.cond);

    if (pthread_mutex_unlock(&t_pthread_cond.mutex) != 0)
    {
        printf("_cmd_convert_wait_cond failed to unlock mutex \n");
        return CMDCVT_AMBR_FAIL; 
    }

    return CMDCVT_AMBR_OK;
}

VOID cmd_convert_wait_shtdn_signal(VOID)
{
    if (pthread_mutex_lock(&t_pthread_cond.mutex) != 0)
    {
        CMDCVT_DBG_INFO("_cmd_convert_wait_cond failed to acquire mutex \n");
        return ;
    }
           
    pthread_cond_wait(&t_pthread_cond.shutdown_cond, &t_pthread_cond.mutex);
           
    if (pthread_mutex_unlock(&t_pthread_cond.mutex) != 0)
    {
        CMDCVT_DBG_INFO("_cmd_convert_wait_cond failed to unlock mutex \n");
        return ;
    }

}

BOOL cmd_convert_get_shtdn_status(VOID)
{
    return t_pthread_cond.b_shutdown_end;
}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_update_ap_status
 *
 * Description:
 *       Update DTV APP status to AM.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: 0, success; -1, failed;
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_update_ap_status(CHAR* ps_app_name, INT32 i4_status, INT32 i4_ret)
{
    CHAR* ps_ap_status = NULL;
    CHAR* ps_output_buff = NULL;

    ps_ap_status = (CHAR *)malloc(CMDCVT_CMD_TYPE_LEN);
    if(ps_ap_status == NULL)
    {
        CMDCVT_DBG_INFO("\n Error in %s, %d\n",__FUNCTION__,__LINE__);
        return CMDCVT_AMBR_OUT_OF_MEM;
    }
    else
    {
        memset(ps_ap_status, 0, CMDCVT_CMD_TYPE_LEN);
    }
    
    ps_output_buff = (CHAR *)malloc(CMDCVT_OUTPUT_BUF_LEN);
    if(ps_output_buff == NULL)
    {
        CMDCVT_DBG_INFO("\n Error in %s, %d\n",__FUNCTION__,__LINE__);
        if(ps_ap_status != NULL)
        {
            free(ps_ap_status);
        }
        return CMDCVT_AMBR_OUT_OF_MEM;
    }
    else
    {
        memset(ps_output_buff, 0, CMDCVT_OUTPUT_BUF_LEN);
    }

    if(i4_ret == 0)
    {
        switch(i4_status)
        {
        case 1: /* AMB_REQUEST_START */
            strncat(ps_ap_status, CMDCVT_MACRO_STARTED, CMDCVT_CMD_TYPE_LEN-1);
            break;
        case 2: /* AMB_REQUEST_RESUME */
            strncat(ps_ap_status, CMDCVT_MACRO_RESUMED, CMDCVT_CMD_TYPE_LEN-1);
            break;
        case 3: /* AMB_REQUEST_PAUSE */
            strncat(ps_ap_status, CMDCVT_MACRO_PAUSED, CMDCVT_CMD_TYPE_LEN-1);
            break;
        case 4: /* AMB_REQUEST_EXIT */
            strncat(ps_ap_status, CMDCVT_MACRO_STOPPED, CMDCVT_CMD_TYPE_LEN-1);
            break;
        default:
            break;
        }   

        snprintf(ps_output_buff, CMDCVT_OUTPUT_BUF_LEN-1, "\n:%s,%s,:%s=%s\n", CMDCVT_MACRO_AP_DTVCTX, CMDCVT_MACRO_AM, ps_ap_status, ps_app_name);

				pthread_mutex_lock(&g_sendcmd_mutex);
        write(t_cmdcvt_info.i4_fdout_dup, ps_output_buff, strlen(ps_output_buff));
        pthread_mutex_unlock(&g_sendcmd_mutex);
    }
    else
    {
        snprintf(ps_output_buff, CMDCVT_OUTPUT_BUF_LEN-1, "\n:%s,%s,:%s=%s,%s=%d\n",CMDCVT_MACRO_AP_DTVCTX,CMDCVT_MACRO_AM,
            CMDCVT_MACRO_FAILED,ps_app_name,CMDCVT_MACRO_ERR_CODE,(int)i4_ret);

        pthread_mutex_lock(&g_sendcmd_mutex);
        write(t_cmdcvt_info.i4_fdout_dup, ps_output_buff, strlen(ps_output_buff));
        pthread_mutex_unlock(&g_sendcmd_mutex);
    }

    /* reset head data after write finished */
/*    ps_ap_head_data[0] = '\0';*/

    if(ps_ap_status != NULL)
    {
        free(ps_ap_status);
    }
    if(ps_output_buff != NULL)
    {
        free(ps_output_buff);
    }

    return CMDCVT_AMBR_OK;
}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_init
 *
 * Description:
 *       Init cmd convert message.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: 0, success; -1, error;
 ----------------------------------------------------------------------------*/
static BOOL b_inited = FALSE;

INT32 cmd_convert_init(VOID)
{
    if (b_inited)
    {
        return CMDCVT_AMBR_OK;
    }
    b_inited = TRUE;
    
    t_cmdcvt_info.ps_rdin_info = calloc(1, CMDCVT_READ_STDIN_BYTE_LEN);
    if (t_cmdcvt_info.ps_rdin_info == NULL)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: calloc error!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }

    t_cmdcvt_info.ps_rdout_info = calloc(1, CMDCVT_READ_STDOUT_BYTE_LEN);
    if (t_cmdcvt_info.ps_rdout_info == NULL)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: calloc error!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }
    
    t_cmdcvt_info.ps_rderr_info = calloc(1, CMDCVT_READ_STDOUT_BYTE_LEN);
    if (t_cmdcvt_info.ps_rderr_info == NULL)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: calloc error!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }

    if(pipe(t_cmdcvt_info.i4_pipe_stdin_fd)==-1)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: create pipe error!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }
    if(pipe(t_cmdcvt_info.i4_pipe_stdout_fd)==-1)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: create pipe error!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }
    if(pipe(t_cmdcvt_info.i4_pipe_stderr_fd)==-1)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: create pipe error!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }

    /* Replace stand in */
    t_cmdcvt_info.i4_fdin_dup = dup(CMDCVT_IO_STDIN);
    close(CMDCVT_IO_STDIN);
    
    dup2(t_cmdcvt_info.i4_pipe_stdin_fd[CMDCVT_PIPE_RD], CMDCVT_IO_STDIN);
    close(t_cmdcvt_info.i4_pipe_stdin_fd[CMDCVT_PIPE_RD]);

    /* Replace stand out */
    t_cmdcvt_info.i4_fdout_dup = dup(CMDCVT_IO_STDOUT);
    close(CMDCVT_IO_STDOUT);
    
    dup2(t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_WR], CMDCVT_IO_STDOUT);
    close(t_cmdcvt_info.i4_pipe_stdout_fd[CMDCVT_PIPE_WR]);

    /* Replace stand err */
    t_cmdcvt_info.i4_fderr_dup = dup(CMDCVT_IO_STDERR);
    close(CMDCVT_IO_STDERR);
    
    dup2(t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_WR], CMDCVT_IO_STDERR);
    close(t_cmdcvt_info.i4_pipe_stderr_fd[CMDCVT_PIPE_WR]);

    /* Init pthread_cond_t for "shutdown" sequence */
    if (pthread_mutex_init(&t_pthread_cond.mutex, NULL) != 0) 
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: Fail to initialize mutex!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }
    if (pthread_condattr_init(&t_pthread_cond.cond_condattr) != 0)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: Fail to initialize condattr!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }
    if (pthread_condattr_setclock(&t_pthread_cond.cond_condattr,CLOCK_MONOTONIC) != 0)
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: Fail to set condattr CLOCK_MONOTONIC!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }
    if (pthread_cond_init(&t_pthread_cond.cond, &t_pthread_cond.cond_condattr) != 0) 
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: Fail to initialize cond!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }

    
    if (pthread_cond_init(&t_pthread_cond.shutdown_cond, NULL) != 0) 
    {
        CMDCVT_DBG_INFO("CMD_CONVERT: Fail to initialize shutdown_cond!!!\n\n");
        return CMDCVT_AMBR_FAIL;
    }
    

    t_pthread_cond.b_shutdown_end = TRUE;
    
    return CMDCVT_AMBR_OK;
}

/*----------------------------------------------------------------------------
 * Name: cmd_convert_exit
 *
 * Description:
 *       exit cmd convert message.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: 0, success; -1, error;
 ----------------------------------------------------------------------------*/
INT32 cmd_convert_exit(VOID)
{
    if (t_cmdcvt_info.ps_rdin_info != NULL)
    {
        free(t_cmdcvt_info.ps_rdin_info);
    }
    
    if (t_cmdcvt_info.ps_rdout_info != NULL)
    {
        free(t_cmdcvt_info.ps_rdout_info);
    }

    if (t_cmdcvt_info.ps_rderr_info != NULL)
    {
        free(t_cmdcvt_info.ps_rderr_info);
    }
    return CMDCVT_AMBR_OK;
}


INT32 cmd_convert_get_param(VOID)
{
    return g_param;    
}

INT32 cmd_convert_get_headdata(CHAR** pps_data)
{
    AMP_CMD_T * pt_amp_cmd = NULL;
    INT32 i4_ret = -1;
    *pps_data = NULL;
    i4_ret = amp_parse_cmd (&t_cmdcvt_info.ps_rdin_info[0], &pt_amp_cmd);
    
    if((pt_amp_cmd == NULL)&&(i4_ret != 1))
    {
        CMDCVT_DBG_INFO("\n Error in %s, %d\n",__FUNCTION__,__LINE__);
        return CMDCVT_AMBR_FAIL;
    }
    if (i4_ret == 0)
    {
        if (pt_amp_cmd->s_head_data != NULL)
        {
            *pps_data = (char *)calloc(1,strlen(pt_amp_cmd->s_head_data) + 1);
            if(NULL == *pps_data)
            {
                i4_ret = CMDCVT_AMBR_OUT_OF_MEM;
            }
            else
            {
                strncpy( *pps_data, pt_amp_cmd->s_head_data, strlen(pt_amp_cmd->s_head_data));   
            }
        }
        else
        {
            i4_ret = CMDCVT_AMBR_FAIL;
        }
    }
    if(pt_amp_cmd != NULL)
    {
        amp_free(pt_amp_cmd);
    }
    return i4_ret;
}

INT32 cmd_convert_get_valuedata(CHAR** pps_data)
{
    AMP_CMD_T * pt_amp_cmd = NULL;
    INT32 i4_ret = -1;
    *pps_data = NULL;
    CHAR* s_value = NULL;
    i4_ret = amp_parse_cmd (&t_cmdcvt_info.ps_rdin_info[0], &pt_amp_cmd);
    
    if((pt_amp_cmd == NULL)&&(i4_ret != 1))
    {
        CMDCVT_DBG_INFO("\n Error in %s, %d\n",__FUNCTION__,__LINE__);
        return CMDCVT_AMBR_FAIL;
    }
    if (i4_ret == 0)
    {
        s_value = pt_amp_cmd->pt_data_field->pt_data_field_node_list_head->pt_data_value->pt_data_value_node_list_head->s_value;
        if (s_value != NULL)
        {
            *pps_data = (char *)calloc(1,strlen(s_value) + 1);
            if(NULL == *pps_data)
            {
                i4_ret = CMDCVT_AMBR_OUT_OF_MEM;
            }
            else
            {
                strncpy( *pps_data, s_value, strlen(s_value) );   
            }
        }
        else
        {
            i4_ret = CMDCVT_AMBR_FAIL;
        }
    }
    if(pt_amp_cmd != NULL)
    {
        amp_free(pt_amp_cmd);
    }
    return i4_ret;
}
INT32 cmd_convert_register_nfy(VOID* pf_nfy)
{
    INT32 i4_ret = CMDCVT_AMBR_OK;
    if (pf_cmd_convert_recv_cmd != NULL)
    {
        /*always set to new function, return -4 notify caller*/
        i4_ret = CMDCVT_AMBR_BE_REGISTERED;
    }
    pf_cmd_convert_recv_cmd = pf_nfy;
    return i4_ret;
}

