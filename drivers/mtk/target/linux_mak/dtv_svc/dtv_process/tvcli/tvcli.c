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
 * $Revision: #2 $
 * $Date: 2012/04/28 $
 * $SWAuthor: Jun Zuo $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 include files
 ----------------------------------------------------------------------------*/
#include "x_dbg.h"
#include "x_common.h"
#include "x_dbg_drv.h"
#include "x_cli.h"
#include "tvcli.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
// #include <sys/shm.h>
#include <dlfcn.h>
#include <signal.h>
#include <sched.h>
#include <errno.h>

/*-----------------------------------------------------------------------------
 macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define FIFO_DTV_READ            "/tmp/fifo_dtv_read"
#define FIFO_DTV_WRITE           "/tmp/fifo_dtv_write"
#define TTY_MAN_EXIT             "TTY_MAN_EXIT\n"
#define MON_STD_BUF_SIZE         2046
#define MON_READ_BYTE_LEN        128
#define AM_KEY_ENTER             0x0A
#define AM_KEY_CR                0x0D

#define DTV_SVC_SIGNAL_ID 31
#define CMD_PASSSTR 0

/* Driver CLI device file */
#define DRV_CLI_DEV   "/dev/cli"

#define STR_INPUT_FIFO "-input_fifo"

static pthread_t  t_fifo_dtv_read_thrd;
static pthread_t  t_dtv_mwcli_stdin_thrd;
static int i4_fifo_dtv_read_fd = -1;

static BOOL b_tigger_dtv_svc_cli = FALSE;
static BOOL b_enalble_mw_cli_fifo = FALSE;
static BOOL b_tty_man_exist = FALSE;

static BOOL b_cli_in_mw_mode = TRUE; /* default in MW CLI mode */
static INT32 gi4_dtv_svc_policy = SCHED_OTHER; /* normal scheduling priority */
static struct sched_param gt_dtv_svc_sp = {.sched_priority =1};

extern INT32 a_msg_convert_send_cmd(CHAR* ps_cmd);
extern x_dbg_data_rec_fct _pf_ext_rec;

/*-----------------------------------------------------------------------------
 functions
 ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Name: show_current_scheduler
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static VOID _show_current_scheduler()
{
    gi4_dtv_svc_policy = sched_getscheduler(0) ;

    switch (gi4_dtv_svc_policy)
    {
    case SCHED_RR:
        printf("Current scheduler is SCHED_RR\n");
        break;

    case SCHED_OTHER:
        printf("Current scheduler is SCHED_OTHER\n");
        break;

    case SCHED_FIFO:
        printf("Current scheduler is SCHED_FIFO\n");
        break;
    }
    printf("\n -- Function : %s, Line : %d -- \n",__FUNCTION__,__LINE__);
    /*printf(".sched_priority = %d\n", gt_dtv_svc_sp.sched_priority);*/
}

/*----------------------------------------------------------------------------
 * Name: cb_dtv_svc_signal_entry
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static VOID _cb_dtv_svc_signal_entry(int signo)
{
    if(b_enalble_mw_cli_fifo == TRUE)
    {
        printf("Input:  MW\n");
    }
    else
    {
        printf("\n[DTV SVC] CLI of dtv_svc process activated. Please input 'Enter' or 'Tab'+'Enter' key to enter CLI console.\n");
    }
    b_tigger_dtv_svc_cli = TRUE;

    /* Raise the scheduler priority */
    sched_setscheduler (0, SCHED_FIFO, &gt_dtv_svc_sp);
    _show_current_scheduler();
}

/*----------------------------------------------------------------------------
 * Name: _set_dtv_svc_console_signal
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static VOID _set_dtv_svc_console_signal()
{
    struct sigaction sysact;

    /*setup signal handler */
    sigemptyset(&sysact.sa_mask);
    sysact.sa_flags = SA_RESTART;
    sysact.sa_handler = _cb_dtv_svc_signal_entry;
    sigaction(DTV_SVC_SIGNAL_ID, &sysact, NULL);

}

/*----------------------------------------------------------------------------
 * Name: _close_drv_cli_dtv
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
void _close_drv_cli_dtv (int* pi4_cli)
{
    if (pi4_cli)
    {
        if (*pi4_cli)
        {
            close(*pi4_cli);
            *pi4_cli = 0;
        }
    }
}

/*----------------------------------------------------------------------------
 * Name: _proc_cli_via_fifo
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static int _proc_cli_via_fifo (
                int         i4_inp,
                int         i4_out,
                const char* ps_prefix,
                char*       ps_info_buff,
                char*       ps_output_buff,
                int *       pi4_is_overflow)
{
    int   i4_len;
    char  s_inp[MON_READ_BYTE_LEN] = {0};
    int   i4_eof = 0; /* Set 1 is end. */
    int   i4_loop_i = 0;

    if(NULL == ps_info_buff || NULL == ps_output_buff || NULL == pi4_is_overflow)
    {
        return -1;
    }

    if ((i4_len = read (i4_inp, &(s_inp [0]), (MON_READ_BYTE_LEN - 1))) > 0)
    {
        if(strcmp(s_inp,TTY_MAN_EXIT)==0)
        {
            /* tty_man exist, need to re-entry later */
            b_tty_man_exist = TRUE;
            s_inp[0] = '\0';
            return i4_eof;
        }

        /* discard the last one char '\n', due to is append by fifo self */
        i4_len --;

        /* Call mw cli function */
        for(i4_loop_i = 0; i4_loop_i < i4_len; i4_loop_i++)
        {
/*            c_dbg_stmt("\n -- Function : %s, Line %d, s_inp[%d]= %c-- \n",
                     __FUNCTION__,__LINE__,i4_loop_i,s_inp[i4_loop_i]);*/
            _pf_ext_rec(s_inp[i4_loop_i]);
        }
        write(1,"\n",sizeof("\n"));
        s_inp[0] = '\0';
    }
    else
    {
        /*printf("Input FD is closed!!! \r\n");*/

        i4_eof = 1;

        if (strlen (ps_info_buff) > 0)
        {
            ps_info_buff[0] = '\0';
        }
    }

    return i4_eof;
}

/*----------------------------------------------------------------------------
 * Name: _tvcli_create_fifo
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static int _tvcli_create_fifo()
{
    int i4_return = -1;

    /* create fifo for ttymanager */
    unlink(FIFO_DTV_WRITE);

    umask(0);
    i4_return = mkfifo(FIFO_DTV_WRITE, 0777);
    if(i4_return < 0)
    {
        printf("\n -- Error : Function : %s, Line : %d, error string is : %s --\n",
                __FUNCTION__, __LINE__,strerror(errno));
        return -1;
    }

    return 0;
}

/*----------------------------------------------------------------------------
 * Name: _tvcli_open_read_fifo
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static int _tvcli_open_read_fifo()
{
/*    c_dbg_stmt("\n -- Function : %s, Line : %d --\n", __FUNCTION__, __LINE__);*/
    i4_fifo_dtv_read_fd = open(FIFO_DTV_WRITE, O_RDONLY);
    if(i4_fifo_dtv_read_fd == -1)
    {
        printf("\n -- Error : Function : %s, Line : %d, error string is : %s --\n",
                __FUNCTION__, __LINE__,strerror(errno));
        return -1;
    }
/*    c_dbg_stmt("\n -- Function : %s, Line : %d, fd = %d --\n", __FUNCTION__, __LINE__,i4_fifo_dtv_read_fd);*/

    return 0;
}

/*----------------------------------------------------------------------------
 * Name: _tvcli_close_read_pipe
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static int _tvcli_close_read_fifo()
{
    if (i4_fifo_dtv_read_fd != -1)
    {
        close(i4_fifo_dtv_read_fd);
        i4_fifo_dtv_read_fd = -1;
    }

    return 0;
}

/*----------------------------------------------------------------------------
 * Name: _monitor_fifo_dtv_read
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static void* _monitor_fifo_dtv_read(void * p_arg)
{
    int   i4_done = 0;
    char s_prefix_none[16] = "MW_CLI>";
    char *ps_prefix = NULL;
    char  *ps_temp_out_buff = NULL;
    char  *ps_temp_complete_buff = NULL;
    int   i4_overflow = 0;
    int i4_out_fd;

    ps_temp_out_buff = (char *)calloc(1,MON_STD_BUF_SIZE);
    if(ps_temp_out_buff == NULL)
    {
        printf("\n -- Error: Function : %s, Line : %d, no memory left --\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    ps_temp_complete_buff = (char *)calloc(1,MON_STD_BUF_SIZE);
    if(ps_temp_complete_buff == NULL)
    {
        printf("\n -- Error: Function : %s, Line : %d, no memory left --\n", __FUNCTION__, __LINE__);
        free(ps_temp_out_buff);
        return NULL;
    }

    _tvcli_create_fifo();

    _tvcli_open_read_fifo();

    while (! (i4_done))
    {
        i4_out_fd = 1; /* STDOUT */
        ps_prefix = s_prefix_none;

        if (0 == _pf_ext_rec)
        {
           sleep(10);
           continue;
        }


        if(0 != _proc_cli_via_fifo (i4_fifo_dtv_read_fd,
                            i4_out_fd,
                            ps_prefix,
                            ps_temp_out_buff,
                            ps_temp_complete_buff,
                            &i4_overflow))
        {
            _tvcli_close_read_fifo();

            usleep(50000);

            _tvcli_open_read_fifo();
        }
    }

    free(ps_temp_out_buff);
    ps_temp_out_buff = NULL;
    free(ps_temp_complete_buff);
    ps_temp_out_buff = NULL;

    pthread_exit(NULL);

    return NULL;

}

/*----------------------------------------------------------------------------
 * Name: _monitor_dtv_mw_cli_stdin
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static void* _monitor_dtv_mw_cli_stdin(void * p_arg)
{
    unsigned int ui4_quit = 0;
    unsigned int ui4_keys = 0;
    char s_msg_cmd[32] = { 0 };
    char * ps_msg_cmd = s_msg_cmd;
    char c_key;
    static BOOL b_toggle_flag = 1;

    ui4_quit = ('q' << 24) | ('u' << 16) | ('i' << 8) | 't';

    while (1)
    {
        if (0 == _pf_ext_rec)
        {
            sleep(10);
            continue;
        }

        if (b_tigger_dtv_svc_cli == FALSE)
        {
            sleep(1);
            continue;
        }

        c_key = getchar();
        if (c_key < 0)
        {
            continue;
        }

        ui4_keys = (ui4_keys << 8) | c_key;
        if (ui4_keys == ui4_quit)
        {
            printf("t\n"); /* complete the word "quit" in screen */

            c_key = 0x08; /* Back Space key */
            /* Remove "qui" */
            _pf_ext_rec(c_key);
            _pf_ext_rec(c_key);
            _pf_ext_rec(c_key);

#ifndef ANDROID
    /*in android project there is no am module*/
            ps_msg_cmd = (char *) ":am,am,:dtv_cli_off\n";
       //     a_msg_convert_send_cmd(ps_msg_cmd);
#endif

            b_tigger_dtv_svc_cli = FALSE;

            /* Lower the scheduler priority */
            sched_setscheduler(0, SCHED_OTHER, &gt_dtv_svc_sp);
            _show_current_scheduler();
        }
        else
        {
            if(((c_key == AM_KEY_ENTER)||(c_key == AM_KEY_CR))&&
                    (b_tty_man_exist == FALSE))
            {
                if (b_toggle_flag == 0)
                {
                    b_toggle_flag = 1;
                    write(1, "\n", sizeof("\n"));
                }
                else if (b_toggle_flag == 1)
                {
                    b_toggle_flag = 0;
                }
                if (b_toggle_flag == 0) /* only input char "enter" */
                {
                    _pf_ext_rec(c_key);
                }
            }
            else
            {
                _pf_ext_rec(c_key);
            }
        }
    }

    pthread_exit(NULL);

    return NULL;
}

/*----------------------------------------------------------------------------
 * Name: _tvcli_init_fifo
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static int _tvcli_init_fifo()
{
    /* create thread for mw cli fifo with ttymanager */
    if (pthread_create(&t_fifo_dtv_read_thrd , NULL, _monitor_fifo_dtv_read, NULL) != 0)
    {
        printf("__line:%d, __func:%s\n", __LINE__, __FUNCTION__);
        return -1;
    }
    /* create thread for mw cli stdin thread */
    if (pthread_create(&t_dtv_mwcli_stdin_thrd , NULL, _monitor_dtv_mw_cli_stdin, NULL) != 0)
    {
        printf("__line:%d, __func:%s\n", __LINE__, __FUNCTION__);
        return -1;
    }

    return 0;
}

/*----------------------------------------------------------------------------
 * Name: tv_cli_init
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
void tv_cli_init(int * argc, char *(*argv[]))
{
    int tmp_argc;
    char **tmp_argv;
    int i4_return = -1;

    if((argc != NULL)&&(argv != NULL))
    {
        tmp_argc = *argc;
        tmp_argv = *argv;

        if (tmp_argc && tmp_argv) {
            int i = 0;
            for (i = 1; i < tmp_argc; i++) {
                if(strcmp(tmp_argv[i],STR_INPUT_FIFO) == 0)
                {
                    b_enalble_mw_cli_fifo = TRUE;
                }
            }
        }
    }

    if(b_enalble_mw_cli_fifo == TRUE)
    {
        i4_return = _tvcli_init_fifo();
        if(i4_return != 0)
        {
            return;
        }
    }

    return;
}

/*----------------------------------------------------------------------------
 * Name: tv_cli_approach
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 ----------------------------------------------------------------------------*/
void tv_cli_approach()
{
    int i4_fdcli = 0, cmd = CMD_PASSSTR;
    unsigned int ui4_quit = 0;
    unsigned int ui4_keys = 0;
    char c_key;

    _set_dtv_svc_console_signal();

    if(b_enalble_mw_cli_fifo == TRUE)
    {
        pthread_join(t_fifo_dtv_read_thrd, NULL);
        pthread_join(t_dtv_mwcli_stdin_thrd, NULL);
    }
    else
    {
        ui4_quit = ('q' << 24) | ('u' << 16) | ('i' << 8) | 't';

        while (1)
        {
            if (0 == _pf_ext_rec)
            {
                sleep(10);
                continue;
            }

            if(b_tigger_dtv_svc_cli == FALSE)
            {
                sleep(1);
                continue;
            }

            c_key = getchar();
            if (c_key < 0)
            {
                continue;
            }

            ui4_keys = (ui4_keys << 8) | c_key;
            if(ui4_keys == ui4_quit)
            {
                printf("t\n"); /* complete the word "quit" in screen */

                if (b_cli_in_mw_mode)
                {
                    c_key = 0x08; /* Back Space key */
                    /* Remove "qui" */
                    _pf_ext_rec(c_key);
                    _pf_ext_rec(c_key);
                    _pf_ext_rec(c_key);
                }
                else
                {
                    _close_drv_cli_dtv(&i4_fdcli);
                    b_cli_in_mw_mode = TRUE; /* Switch back to MW mode */
                    printf("Switch back to MW mode before quit.\n");
                    printf("Input:  MW\n");
                }

                b_tigger_dtv_svc_cli = FALSE;

                /* Lower the scheduler priority */
                sched_setscheduler (0, SCHED_OTHER, &gt_dtv_svc_sp);
                _show_current_scheduler();
            }
            else if (c_key == 9) /* Ctrl-I */
            {
                b_cli_in_mw_mode = (b_cli_in_mw_mode)? FALSE: TRUE;
                if (b_cli_in_mw_mode)
                {
                    _close_drv_cli_dtv(&i4_fdcli);
                    printf("Input:  MW\n");
                }
                else
                {
                    printf("Input:  CLI\n");

                    i4_fdcli = open(DRV_CLI_DEV, O_RDWR);
                    if (i4_fdcli < 0)
                    {
                        printf("Cannot open driver CLI device /dev/cli!\n");
                        printf("Switch back to MW mode.\n");
                        printf("Input:  MW\n");
                        b_cli_in_mw_mode = TRUE;
                    }
                }
            }
            else if (b_cli_in_mw_mode)
            {
                _pf_ext_rec(c_key);
            }
            else
            {
                if (c_key != 0x08) /* Back Space */
                {
                    putchar(c_key);
                }
                if (ioctl(i4_fdcli, cmd, &c_key) < 0)
                {
                    printf("Cannot do ioctl to CLI device!\n");
                }
            }
        }
    }
}

