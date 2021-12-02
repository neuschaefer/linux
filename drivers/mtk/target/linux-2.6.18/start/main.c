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
 * $RCSfile: main.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains all driver startup implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "x_cli.h"
#include "x_dbg.h"
#include "x_common.h"
#include "x_start_common.h"
#include "u_dlm.h"
#include "x_os.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/shm.h>
#include <signal.h>
#include <sched.h>

#include "x_dbg_drv.h"
#include "ioctl_param.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Allocate 2^24 Bytes for the system memory. */
#define SYS_MEM_SIZE ((SIZE_T) 12 * 1024 * 1024)  
/* Linux kernel page offset */
#define PAGE_OFFSET  0xc0000000 
/* Linux resource manager device file for driver */
#define MODULE_RM_DEV "/dev/rmmgr"
/* Linux memory device file */
#define MEMORY_MAP_DEV "/dev/mem"
#define MMAP_SIZE 16*1024

/* Driver CLI device file */
#define DRV_CLI_DEV   "/dev/cli"

/* MHF Linux - Jun Zuo */
#define MW_CLI_SIGNAL_ID 31

#define CMD_PASSSTR 0

extern x_dbg_data_rec_fct _pf_ext_rec;
extern void ssma_init(void * p_start_addr, size_t total_size);

/* MHF Linux - Jun ZUo */
static BOOL b_tigger_mw_cli = FALSE;

static BOOL b_cli_in_mw_mode = TRUE; /* default in MW/APP CLI mode */
static INT32 gi4_dtv_ap_policy = SCHED_OTHER; /* normal scheduling priority */
static struct sched_param gt_dtv_ap_sp = {.sched_priority =1};

static VOID show_current_scheduler()
{

    gi4_dtv_ap_policy = sched_getscheduler(0) ;

    switch (gi4_dtv_ap_policy)
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
    /*printf(".sched_priority = %d\n", gt_dtv_ap_sp.sched_priority);*/
}

VOID cb_signal_entry(int signo)
{
    printf("\n[DTV APP] CLI of dtvap process activated. Please input 'Enter' or 'Tab'+'Enter' key to enter CLI console.\n");
    b_tigger_mw_cli = TRUE;

    /* Raise the scheduler priority */
    sched_setscheduler (0, SCHED_FIFO, &gt_dtv_ap_sp);
    show_current_scheduler();
}

VOID set_console_signal()
{
    struct sigaction sysact;

    /*setup signal handler */
    sigemptyset(&sysact.sa_mask);
    sysact.sa_flags = SA_RESTART;
    sysact.sa_sigaction = cb_signal_entry;
    sigaction(MW_CLI_SIGNAL_ID, &sysact, NULL);

}
/*-----------------------------------------------------------------------------
 * Name: main
 *
 * Description: This API is the LINUX entry point, allocates a chunk of memory
 *              for the middleware system and calls "x_sys_start".
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

/* Extern argc and argv for linux gl dfb */
int g_main_argc;
char ** g_main_argv;

#if 1 /* MHF - Linux Basara for mmp */
static UINT32 g_ui4_fbm_usr_address_start = 0;
static INT32 g_i4_fbm_usr_mem_size = 0;

BOOL is_fbm_usr_mem(INT32 usr_address)
{
    if(usr_address < g_ui4_fbm_usr_address_start + g_i4_fbm_usr_mem_size
        && usr_address >= g_ui4_fbm_usr_address_start)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif


void Close_CLI_Dev (INT32* pi4_cli)
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

int main(int argc, char *argv[])
{
    VOID* pv_mem_addr;
    VOID* pv_shm_addr;
    VOID* pv_phy_addr;
    INT32 i4_fdioctrl;
    INT32 i4_fdmem;
#if 0    
    INT32 i4_shmid;
#endif    
    UINT32 ui4_tmp;
   
#if 1 /* Support MM feature */    
    INT32 i4_mmap_off;
    INT32 i4_fbm_size;
#endif

    /* MHF Linux - Jun Zuo */
    BOOL b_drv_cli = FALSE;
    UINT32 ui4_quit = 0, ui4_keys;
    CHAR c_key;

    INT32 i4_fdcli, cmd = CMD_PASSSTR;

    g_main_argc = argc;
    g_main_argv = argv;
    
    i4_fdioctrl=open(MODULE_RM_DEV, O_RDWR);
    if (i4_fdioctrl == -1)
    {
       return -1;
    }

    i4_fdmem = open(MEMORY_MAP_DEV,O_RDWR);
    if (i4_fdmem == -1)
    {
       return -1;
    }

#if 0 /* need to export shmget in kernel, "taint" liscense */
    ioctl(i4_fdioctrl,IOCTRL_MALLOC_SHM,SYS_MEM_SIZE);
    i4_shmid = shmget((key_t)7879,SYS_MEM_SIZE,SHM_R | SHM_W | IPC_CREAT);
#endif
#ifndef LINUX_MEM_SOLUTION /* MHF Linux - Jun Zuo */
    if ((pv_mem_addr = (VOID*) malloc (SYS_MEM_SIZE)) == NULL)
    {
        DBG_ABORT (DBG_MOD_BOARD | DBG_CAT_MEMORY);
    }
#else /* use linux memory manager directly */
    pv_mem_addr = NULL;
#endif    
    ui4_tmp = MMAP_SIZE;
    ioctl(i4_fdioctrl,IOCTRL_MALLOC_SHM,&ui4_tmp);
    pv_phy_addr = (VOID *)(ui4_tmp - PAGE_OFFSET);
    pv_shm_addr=mmap(0,  MMAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, i4_fdmem, (unsigned long)pv_phy_addr);

    ssma_init(pv_shm_addr, (size_t)MMAP_SIZE);
    ioctl(i4_fdioctrl,IOCTRL_SET_USER_VIRT_START,pv_shm_addr);

#if 1 /* Support MM feature */

    /*TMP code of magic number, we should get size by ioctl*/
    i4_fbm_size = ioctl(i4_fdioctrl, IOCTL_GET_FBM_SIZE, 0);
   
    i4_mmap_off = 0;

#if 1 /* MHF - Linux Basara for mmp */
    g_i4_fbm_usr_mem_size = i4_fbm_size;
    g_ui4_fbm_usr_address_start = mmap((void*)NULL,i4_fbm_size, PROT_READ | PROT_WRITE, MAP_SHARED, i4_fdioctrl, (unsigned long)i4_mmap_off);
#else
    mmap((void*)NULL,i4_fbm_size, PROT_READ | PROT_WRITE, MAP_SHARED, i4_fdioctrl, (unsigned long)i4_mmap_off);
#endif
    
#endif    
        
#if 0 /* need to export shmat in kernel, "taint" license */
    if (i4_shmid == -1)
    {
        return ;
    }
    pv_mem_addr = shmat(i4_shmid,NULL,0);
    if (pv_mem_addr == -1)
    {
        return;
    }
#endif
    
    x_sys_start (pv_mem_addr, SYS_MEM_SIZE, NULL);

    /*TODO:When to do this? */
    /*close(i4_fdioctrl);*/
    ui4_quit = ('q' << 24) | ('u' << 16) | ('i' << 8) | 't';
    
    set_console_signal();
    while (1) /* MW demo console */
    {
        if (0 == _pf_ext_rec)
        {
           sleep(10);
           continue;
        }
        /* MHF Linux - Jun Zuo */
        if(b_tigger_mw_cli == FALSE)
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
                Close_CLI_Dev(&i4_fdcli);
                b_cli_in_mw_mode = TRUE; /* Switch back to MW mode */
                printf("Switch back to MW mode before quit.\n");
                printf("Input:  MW\n");
            }

            b_tigger_mw_cli = FALSE;

            /* Lower the scheduler priority */
            sched_setscheduler (0, SCHED_OTHER, &gt_dtv_ap_sp);
            show_current_scheduler();
        }
        else if (c_key == 9) /* Ctrl-I */
        {
            b_cli_in_mw_mode = (b_cli_in_mw_mode)? FALSE: TRUE;
            if (b_cli_in_mw_mode)
            {
                Close_CLI_Dev(&i4_fdcli);
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
        else /* driver CLI */
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

    return (0);
}


