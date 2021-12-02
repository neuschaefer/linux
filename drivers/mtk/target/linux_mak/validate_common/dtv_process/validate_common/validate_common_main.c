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
                    include files
-----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/shm.h>
#include <dlfcn.h>
#include <signal.h>
#include <sched.h>
#include "cli/cli.h"
#include "dbg/dbg.h"
#include "file_mngr/x_fm.h"
#include "file_mngr/fm_ufs.h"
#include "start/version.h"
#include "handle/handle.h"
#include "handle/x_handle_grp.h"
#include "os/inc/x_os.h"

#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/inc/c_os.h"

#include "cmpb/cmpb_cli.h"
#include "cmpb/cmpb_util.h"
#include "cmpb/IMtkPb_ErrorCode.h"
#include "cmpb/IMtkPb_Ctrl.h"
#include "file_mngr/c_fm.h"

#include "svctx/u_svctx.h"
#include "svctx/x_svctx.h"
#include "svctx/c_svctx.h"

#include "dbg/x_dbg.h"
#include "dbg/x_dbg_drv.h"
#include "cli/x_cli.h"
#include "dm/dm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_NUM_HANDLES      ((unsigned short) 4096)
#define SYS_MEM_SIZE ((unsigned int) 12 * 1024 * 1024)  
#define CMPB_PRIVATE_CONTAINER_HEADER_SIZE 14
#define DRV_CLI_DEV   "/dev/cli"
#define DTV_SVC_SIGNAL_ID 31
#define CMD_PASSSTR 0
#define INIT_THREAD_NAME        "init_thread"
#define INIT_THREAD_STACK_SIZE  ((SIZE_T) 1024*8)
#define INIT_THREAD_PRIORITY    ((UINT8)  128)

typedef struct _GEN_CONFIG_T
{
    unsigned short  ui2_version;

    void*  pv_config;

    unsigned int  z_config_size;

    THREAD_DESCR_T  t_mheg5_thread;
}   GEN_CONFIG_T;

typedef struct _STREAM_SEGMENT_T
{
    UINT32  ui4_ms;
    UINT32  ui4_size;
    UINT32  ui4_next_offset;
}STREAM_SEGMENT_T;

/*-----------------------------------------------------------------------------
                    static data declarations
-----------------------------------------------------------------------------*/
int g_main_argc;
char ** g_main_argv;
static HANDLE_T  h_sys_sema;  /* Used to synchronize the init thread. */
static BOOL b_tigger_dtv_svc_cli = FALSE;
extern x_dbg_data_rec_fct _pf_ext_rec;
static BOOL b_cli_in_mw_mode = TRUE; /* default in MW CLI mode */

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
int c_rpc_init_client(void);
int c_rpc_start_client(void);

int os_init(const void *pv_addr, unsigned int z_size);
int x_rtos_init (GEN_CONFIG_T*  pt_config);

/*INT32 cmpb_cli_init(VOID);*/

/*-----------------------------------------------------------------------------
                    Internal functions declarations
 ----------------------------------------------------------------------------*/
INT32 x_time_msrt_begin(
    const CHAR*           s_name,
    UINT16                ui2_tms_lvl,
    const CHAR*           s_comment )
{
    return 0;
}

UINT16 time_msrt_get_level( UINT16 ui2_tms_mdl_level )
{
    return 0;
}

UINT32 time_msrt_get_flag( VOID )
{
   return 0;
}

static BOOL init_raw_dev (VOID)
{
    BOOL b_success;
    HANDLE_T    h_dev_dir;

    b_success = FALSE;

    DRV_TYPE_T t_drv_type;
    UINT16 ui2_unit_id_img_1 = 1;
    UINT16 ui2_unit_id_img_2 = 3;
    UINT16 ui2_unit_id_svl_1 = 2;
    UINT16 ui2_unit_id_svl_2 = 4;
    UINT32        ui4_mode;

    t_drv_type = DRVT_NAND_FLASH;
    ui4_mode = FM_MODE_TYPE_BLK;

    ui2_unit_id_img_1 = 2*2;
    ui2_unit_id_img_2 = 3*2;
    ui2_unit_id_svl_1 = 6*2;
    ui2_unit_id_svl_2 = 7*2;

    /* if (x_fm_create_dir (FM_ROOT_HANDLE, SN_DEV_PATH, 0666) == FMR_OK) */
    if ( x_fm_open_dir(FM_ROOT_HANDLE, SN_DEV_PATH, &h_dev_dir) == FMR_OK)
    {
        /* Mount all NOR Flash devices and EEPROM devices. */
        /* This piece of code must be enhanced so that we can cycle through all  */
        /* possible raw file system devices and mount them correctly. Also, this */
        /* is the place wer we can mount other devices, which are used to store  */
        /* the configuration database etc.                                       */

        if ((x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_1, (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), t_drv_type, ui2_unit_id_img_1) == FMR_OK) &&
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_2, (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), t_drv_type, ui2_unit_id_svl_1) == FMR_OK) &&
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_3, (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), t_drv_type, ui2_unit_id_img_2) == FMR_OK) &&
            #ifdef DUAL_CHANNEL_SUPPORT
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_4, (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), t_drv_type, ui2_unit_id_svl_2) == FMR_OK) &&
            #endif
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_EEPROM_0,    (FM_MODE_TYPE_CHR | FM_MODE_DEV_TRUE | 0666), DRVT_EEPROM,    0) == FMR_OK) &&
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_EEPROM_3,    (FM_MODE_TYPE_CHR | FM_MODE_DEV_TRUE | 0666), DRVT_EEPROM,    3) == FMR_OK) &&
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_EEPROM_4,    (FM_MODE_TYPE_CHR | FM_MODE_DEV_TRUE | 0666), DRVT_EEPROM,    4) == FMR_OK))
        {
            b_success = TRUE;
        }
        /* rootfs A*/
        x_fm_make_entry(FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_4, (ui4_mode | FM_MODE_DEV_TRUE | 0666), t_drv_type, 4);

        /* rootfs B*/
        x_fm_make_entry(FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_5, (ui4_mode | FM_MODE_DEV_TRUE | 0666), t_drv_type, 5);

        /* basic */
        x_fm_make_entry(FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_9, (ui4_mode | FM_MODE_DEV_VIRT | 0666), t_drv_type, 9);

        x_fm_create_dir(FM_ROOT_HANDLE, "/basic", 0666);

        /* attach basic dir to MTD Block 9 */
        x_fm_attach_file_system(FM_ROOT_HANDLE,
                                SN_DEV_PATH "/" SN_MTD_BLK_9,
                                FM_ROOT_HANDLE,
                                "/basic");

        x_fm_create_dir(FM_ROOT_HANDLE, "/basic/upgrade", 0666);


        /* 3rd */
        x_fm_make_entry(FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_10, (ui4_mode | FM_MODE_DEV_VIRT | 0666), t_drv_type, 10);

        x_fm_create_dir(FM_ROOT_HANDLE, "/3rd", 0666);

        /* attach 3rd dir to MTD Block 10 */
        x_fm_attach_file_system(FM_ROOT_HANDLE,
                                SN_DEV_PATH "/" SN_MTD_BLK_10,
                                FM_ROOT_HANDLE,
                                "/3rd");

        x_fm_create_dir(FM_ROOT_HANDLE, "/3rd/upgrade", 0666);
    }

    x_fm_create_dir(FM_ROOT_HANDLE, "/mnt/", 0666);
    x_fm_close(h_dev_dir);

    return (b_success);
}

static VOID cb_dtv_svc_signal_entry(int signo)
{
    printf("\n[DTV SVC] CLI of dtv_svc process activated. Please input 'Enter' or 'Tab'+'Enter' key to enter CLI console.\n");
    b_tigger_dtv_svc_cli = TRUE;
}

static VOID set_dtv_svc_console_signal()
{
    struct sigaction sysact;

    /*setup signal handler */
    sigemptyset(&sysact.sa_mask);
    sysact.sa_flags = SA_RESTART;
    sysact.sa_handler = cb_dtv_svc_signal_entry;
    sigaction(DTV_SVC_SIGNAL_ID, &sysact, NULL);
}

static void Close_CLI_Dev (INT32* pi4_cli)
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

static void _enable_cli_env()
{
    UINT32 ui4_quit = 0;
    UINT32 ui4_keys = 0;
    CHAR c_key;
    INT32 i4_fdcli = 0, cmd = CMD_PASSSTR;
    
    ui4_quit = ('q' << 24) | ('u' << 16) | ('i' << 8) | 't';

	set_dtv_svc_console_signal();

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
                Close_CLI_Dev(&i4_fdcli);
                b_cli_in_mw_mode = TRUE; /* Switch back to MW mode */
                printf("Switch back to MW mode before quit.\n");
                printf("Input:  MW\n");
            }

            b_tigger_dtv_svc_cli = FALSE; 
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


static VOID _rpc_env_init()
{
    GEN_CONFIG_T  t_rtos_config = {0};
    void*       pv_mem_addr = 0;
    unsigned int z_mem_size = 0xc00000;
    
    printf("_rpc_env_init enter\n");
    x_rtos_init (&t_rtos_config);
    handle_init (MAX_NUM_HANDLES, &pv_mem_addr, &z_mem_size);
    os_init (pv_mem_addr, z_mem_size);

    c_rpc_init_client();
    c_rpc_start_client();
    printf("_rpc_env_init exit\n");
}

static VOID init_sys (VOID)
{
    DM_INIT_PARM_T t_dm_init;

    t_dm_init.ui4_automnt_cond_ns = 0;
    t_dm_init.pt_automnt_cond = NULL;
    
    printf("start_reg_cli\n");
    start_reg_cli();
/*
    printf("cmpb_cli_init\n");
    cmpb_cli_init();
*/  
    printf("fm_init\n");
    fm_init();

    printf("dm_init\n");
    dm_init(&t_dm_init);

    printf("init_raw_dev\n");
    init_raw_dev();
    
    dbg_output_init (NULL,
                     0,
                     NULL);
}

static VOID init_sys_thread_main (VOID* pv_arg)
{       
    /* Initalize the system in the system thread. */
    init_sys ();

    /* And unlock the waiting thread. */
    x_sema_unlock (h_sys_sema);
}

static VOID init_sys_thread (VOID)
{
    HANDLE_T h_sys_thread;


    h_sys_sema = NULL_HANDLE;

    /* Create binary semaphore to wait on. */
    if ((x_sema_create (&h_sys_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK)) != OSR_OK)
    {
        while(1);
    }

    /* Create initialization thread to perform initialization. */
    if ((x_thread_create (&h_sys_thread,
                          INIT_THREAD_NAME,
                          INIT_THREAD_STACK_SIZE,
                          INIT_THREAD_PRIORITY,
                          init_sys_thread_main,
                          0,
                          NULL)) != OSR_OK)
    {
        while(1);
    }

    /* And now wait on semaphore until unlocked. */
    if ((x_sema_lock (h_sys_sema, X_SEMA_OPTION_WAIT)) != OSR_OK)
    {
        while(1);
    }

    /* Ok, done, free up the semaphore resource. The initialization */
    /* thread will have terminated by itself.                       */
    x_sema_delete (h_sys_sema);
}


int main(void)
{
    THREAD_DESCR_T  t_cli_thread;
    int policy;
    struct sched_param sp = {.sched_priority =1};

    policy = sched_getscheduler(0);
    switch (policy)
    {
    case SCHED_RR:
        printf("SCHED_RR\n");
        break;
        
    case SCHED_OTHER:
        printf("SCHED_OTHER\n");
        break;
        
    case SCHED_FIFO:
        printf("SCHED_OTHER\n");
        break;
    }

    sched_setscheduler (0, SCHED_FIFO, &sp); 
   
    _rpc_env_init();

    t_cli_thread.z_stack_size = 32768;
    t_cli_thread.ui2_num_msgs = 2048;
    t_cli_thread.ui1_priority = 128;

    printf("cli_init\n");
    cli_init(&t_cli_thread);
    
    printf("init_sys_thread\n");
    init_sys_thread();
    
    _enable_cli_env();

    return 0;
}
