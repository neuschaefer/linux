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

#include "x_wgl_button.h"
#include "x_wgl_cc.h"
#include "x_wgl_edit.h"
#include "x_wgl_frame.h"
#include "x_wgl_icon.h"
#include "x_wgl_lb.h"
#include "x_wgl_progress.h"
#include "x_wgl_sets.h"
#include "x_wgl_text.h"
#include "x_wgl_tktp.h"
#include "x_wgl_hts.h"

#include "x_dbg.h"
#include "x_common.h"
#include "x_dbg_drv.h"
#include "x_sys_name.h"
#include "x_cli.h"
#include "x_rm.h"
#include "x_tm.h"
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#endif

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
#include <sys/shm.h>

#include "ioctl_param.h"
#include "util/ssma.h"
#include "directfb.h"

#include <dlfcn.h>
#include <signal.h>
#include <sched.h>
//#include <mcheck.h>

/* Allocate 2^24 Bytes for the system memory. */
#define SYS_MEM_SIZE ((SIZE_T) 24 * 1024 * 1024)  
/* Linux kernel page offset */
#define PAGE_OFFSET  0xc0000000 
/* Linux resource manager device file for driver */
#define MODULE_RM_DEV "/dev/rmmgr"
/* Linux memory device file */
#define MEMORY_MAP_DEV "/dev/mem"
#define MMAP_SIZE 20*1024


#define MTRACE_SIGNAL_ID 30

#if 1 /* MHF - Linux Basara for mmp */
static UINT32 g_ui4_fbm_usr_address_start = 0;
static INT32 g_i4_fbm_usr_mem_size = 0;
BOOL is_fbm_usr_mem(INT32 usr_address)
{
    return (usr_address < g_ui4_fbm_usr_address_start + g_i4_fbm_usr_mem_size
        && usr_address >= g_ui4_fbm_usr_address_start);
}
#endif

/* TMPORARY SOLUTION OF CC DATA 
 * FIX ME: We should not use app data directly
 */

#ifndef SINGLE_PROCESS /*Yan CLI*/
#define ENABLE_AM_START_APPCVT  1
#else
#undef ENABLE_AM_START_APPCVT 
#endif

#if ENABLE_AM_START_APPCVT /*Yan CLI*/
#define RUN_TIME_OPTION         "--am"
#define CLI_OPTION              "--cli"
#define SVC_CLI_BY_AM_THRD_NAME                "svc_cli_by_am_thrd"
#define SVC_CLI_BY_AM_THRD_DEFAULT_PRIORITY    ((UINT8)   128)
#define SVC_CLI_BY_AM_THRD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)
#endif

static BOOL b_tigger_mtrace = FALSE;


#if ENABLE_AM_START_APPCVT
static HANDLE_T h_svc_cli_by_am_thrd;
#endif


static VOID cb_mtrace_signal_entry(int signo)
{
    b_tigger_mtrace = !b_tigger_mtrace;

    if (b_tigger_mtrace)
    {
        printf("\nenable mtrace\n");
        setenv("MALLOC_TRACE", "/tmp/mnt/dtv_svc_mtrace.log", 1);
//        mtrace();
    }
    else
    {
        printf("\ndisable mtrace\n");
//        muntrace ();
    }
}

static VOID set_mtrace_signal()
{
    struct sigaction sysact;

    /*setup signal handler */
    sigemptyset(&sysact.sa_mask);
    sysact.sa_flags = SA_RESTART;
    sysact.sa_handler = cb_mtrace_signal_entry;
    sigaction(MTRACE_SIGNAL_ID, &sysact, NULL);

}

/*-----------------------------------------------------------------------------
                    global data
 ----------------------------------------------------------------------------*/
#ifdef DTV_SVC_CC_FONTS
/* SN_FONT_MONO_SP_SERF: Courier10 */
static WGL_CC_FONT_REG_INFO_RES_PARAM_T at_res_param_mono_sp_serf[] = 
{
    {
        WGL_CC_MAKE_FONT_TBL_ID(1280, 720),
        { 0, 0, 0 }, 
        { 0, 0, 0 }, 
        { 1, 1, 0 }
    },        
    
    END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T
};    

/* SN_FONT_PROP_SP_SERF: Oranda */
static WGL_CC_FONT_REG_INFO_RES_PARAM_T at_res_param_prop_sp_serf[] = 
{
    {
        WGL_CC_MAKE_FONT_TBL_ID(1280, 720),
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 1, 1, 0 }
    },
    
    END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T
};    

/* SN_FONT_MONO_SP_WO_SERF: Lettergothic (mono space simulation) */
static WGL_CC_FONT_REG_INFO_RES_PARAM_T at_res_param_mono_sp_wo_serf[] = 
{
    {
        WGL_CC_MAKE_FONT_TBL_ID(1280, 720),
        { 0, 0, 22 },
        { 0, 0, 24 },
        { 1, 1, 28 }
    },
        
    END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T
};    

/* SN_FONT_PROP_SP_WO_SERF: Tiresias */
static WGL_CC_FONT_REG_INFO_RES_PARAM_T at_res_param_prop_sp_wo_serf[] = 
{
    {
        WGL_CC_MAKE_FONT_TBL_ID(1280, 720),
        { 0, 0, 0 },
        { 1, 3, 0 },
        { 2, 6, 0 }
    },
        
    END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T
};    

/* SN_FONT_CASUAL: Misterearl */
static WGL_CC_FONT_REG_INFO_RES_PARAM_T at_res_param_casual[] = 
{
    {
        WGL_CC_MAKE_FONT_TBL_ID(1280, 720),
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 1, 0 }
    },
        
    END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T
};    

/* SN_FONT_CURSIVE: Kaufman */
static WGL_CC_FONT_REG_INFO_RES_PARAM_T at_res_param_cursive[] = 
{
    {
        WGL_CC_MAKE_FONT_TBL_ID(1280, 720),
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 0, 0, 0 }
    },
        
    END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T
};    

/* SN_FONT_SMALL_CAPITALS: Copperplate */
static WGL_CC_FONT_REG_INFO_RES_PARAM_T at_res_param_small_capitals[] = 
{
    {
        WGL_CC_MAKE_FONT_TBL_ID(1280, 720),
        { 0, 0, 0 },
        { 0, 0, 0 },
        { 1, 1, 0 }
    },
        
    END_OF_ARRAY_FONT_REG_INFO_RES_PARAM_T
};    

 
WGL_CC_FONT_REG_INFO_T  at_g_cc_font_reg_table[] = 
{
    { SN_FONT_MONO_SP_SERF,     at_res_param_mono_sp_serf},
    { SN_FONT_PROP_SP_SERF,     at_res_param_prop_sp_serf},
    { SN_FONT_MONO_SP_WO_SERF,  at_res_param_mono_sp_wo_serf},
    { SN_FONT_PROP_SP_WO_SERF,  at_res_param_prop_sp_wo_serf},
    { SN_FONT_CASUAL,           at_res_param_casual},
    { SN_FONT_CURSIVE,          at_res_param_cursive},
    { SN_FONT_SMALL_CAPITALS,   at_res_param_small_capitals},
    
    END_OF_ARRAY_FONT_REG_INFO_T
};

unsigned char font_file_courier10[] =
{
    #include "courier10.i"
};

unsigned char font_file_oranda[] =
{
    #include "oranda.i"
};

unsigned char font_file_lettergothic[] =
{
    #include "lettergothic.i"
};

unsigned char font_file_tiresias[] =
{
    #include "tiresias.i"
};

unsigned char font_file_misterearl[] =
{
    #include "misterearl.i"
};

unsigned char font_file_kaufman[] =
{
    #include "kaufman.i"
};

unsigned char font_file_copperplate[] =
{
    #include "copperplate.i"
};
#endif /* DTV_SVC_CC_FONTS */

#if ENABLE_AM_START_APPCVT
static HANDLE_T h_svc_cli_by_am_thrd;
#endif


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
#ifdef DTV_SVC_CC_FONTS
BOOL sys_init_cc_font(VOID)
{
    BOOL   b_success;
    UINT32 ui4_font_size_courier      = GET_UINT32_FROM_PTR_BIG_END (&(font_file_courier10 [0]));
    UINT32 ui4_font_size_oranda       = GET_UINT32_FROM_PTR_BIG_END (&(font_file_oranda [0]));
    UINT32 ui4_font_size_lettergothic = GET_UINT32_FROM_PTR_BIG_END (&(font_file_lettergothic [0]));
    UINT32 ui4_font_size_tiresias     = GET_UINT32_FROM_PTR_BIG_END (&(font_file_tiresias [0]));
    UINT32 ui4_font_size_misterearl   = GET_UINT32_FROM_PTR_BIG_END (&(font_file_misterearl [0]));
    UINT32 ui4_font_size_kaufman      = GET_UINT32_FROM_PTR_BIG_END (&(font_file_kaufman [0]));
    UINT32 ui4_font_size_copperplate  = GET_UINT32_FROM_PTR_BIG_END (&(font_file_copperplate [0]));
    
    b_success = FALSE;

    /* font sizes for: 1280 x 720 */
    if(
        (x_fe_add_mem_font(SN_FONT_MONO_SP_SERF,    &(font_file_courier10 [4]),    ui4_font_size_courier,      31, 32, 34) == FER_OK)  &&
        (x_fe_add_mem_font(SN_FONT_PROP_SP_SERF,    &(font_file_oranda [4]),       ui4_font_size_oranda,       33, 34, 36) == FER_OK)  &&
        (x_fe_add_mem_font(SN_FONT_MONO_SP_WO_SERF, &(font_file_lettergothic [4]), ui4_font_size_lettergothic, 31, 32, 34) == FER_OK)  &&
        (x_fe_add_mem_font(SN_FONT_PROP_SP_WO_SERF, &(font_file_tiresias [4]),     ui4_font_size_tiresias,     28, 31, 34) == FER_OK)  &&
        (x_fe_add_mem_font(SN_FONT_CASUAL,          &(font_file_misterearl [4]),   ui4_font_size_misterearl,   31, 32, 33) == FER_OK)  &&
        (x_fe_add_mem_font(SN_FONT_CURSIVE,         &(font_file_kaufman [4]),      ui4_font_size_kaufman,      30, 31, 32) == FER_OK)  &&
        (x_fe_add_mem_font(SN_FONT_SMALL_CAPITALS,  &(font_file_copperplate [4]),  ui4_font_size_copperplate,  35, 36, 38) == FER_OK) )
    {
        b_success = TRUE;
    }
    
    return b_success;
}
#endif /* DTV_SVC_CC_FONTS */

#if ENABLE_AM_START_APPCVT /*Yan CLI*/
static VOID _svc_cli_by_am_thread(
    VOID *pv_data)
{
    INT32          i4_ret;

    while (1)
    {
        cmd_convert_monitor();
        
        i4_ret = cmd_convert_parse();
        if (i4_ret == 0) /* cli cmd */
        {
            cmd_convert_parse_cli_cmd();
        }
        cmd_convert_reset_buf();
   	}
}
#endif

#ifdef SINGLE_PROCESS
int g_main_argc;
char ** g_main_argv;
#endif

extern VOID os_linux_set_fbm_info(UINT32 ui4_virt_addr, UINT32 ui4_size);
extern VOID x_sys_start (VOID* pv_mem_addr, SIZE_T z_mem_size, VOID* pv_arg);
extern INT32 c_rpc_init_server();
extern VOID tv_cli_init( int *argc, char *(*argv[]));
extern VOID tv_cli_approach();

int main(int argc, char *argv[])
{
    INT32       i4_return   = 0;
    VOID*       pv_mem_addr = NULL;
    
    VOID* pv_shm_addr;
    VOID* pv_phy_addr;
    INT32 i4_fdioctrl;
    INT32 i4_fdmem;
    UINT32 ui4_tmp;
    UINT32 ui4_fbm_phy_addr_start;
/* Support MM feature */    
    //INT32 i4_mmap_off;
    INT32 i4_fbm_size;

    char *ps_config_file_name = NULL;
#if ENABLE_AM_START_APPCVT /*Yan CLI*/
    UINT32 ui4_loop_i = 0;
#endif

#ifdef SINGLE_PROCESS
    g_main_argc = argc;
    g_main_argv = argv;
#endif

    tv_cli_init(&argc, &argv);

    /* Used for debug : segment fault or signal 0x1 */
    dlopen("libarm_sig_tracer.so", RTLD_NOW);

    i4_fdioctrl=open(MODULE_RM_DEV, O_RDWR);
    if (i4_fdioctrl == -1)
    {
       return -1;
    }

    i4_fdmem = open(MEMORY_MAP_DEV,O_RDWR);
    if (i4_fdmem == -1)
    {
       close(i4_fdioctrl);
       return -1;
    }
    
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
    
/* Support MM feature */
    /*TMP code of magic number, we should get size by ioctl*/
    i4_fbm_size = ioctl(i4_fdioctrl, IOCTL_GET_FBM_SIZE, 0);   
    //i4_mmap_off = 0;
    ui4_fbm_phy_addr_start = ioctl(i4_fdioctrl, IOCTL_GET_FBM_PHY_ADDR, 0);
#if 1 /* MHF - Linux Basara for mmp */
    g_i4_fbm_usr_mem_size = i4_fbm_size;
    g_ui4_fbm_usr_address_start = (UINT32) mmap((void*) NULL, i4_fbm_size, PROT_READ | PROT_WRITE, MAP_SHARED,
            i4_fdioctrl, (unsigned long) ui4_fbm_phy_addr_start);
    os_linux_set_fbm_info(g_ui4_fbm_usr_address_start, i4_fbm_size);
    
#else
    mmap((void*)NULL,i4_fbm_size, PROT_READ | PROT_WRITE, MAP_SHARED, i4_fdioctrl, (unsigned long)i4_mmap_off);
#endif    

    printf("dtv_svc_main: fbm phy addr = %x, share mem addr = %x, vir addr = %x, size = %d \n", 
    (unsigned int)ui4_fbm_phy_addr_start, (unsigned int)pv_shm_addr, 
    (unsigned int)g_ui4_fbm_usr_address_start,(int)g_i4_fbm_usr_mem_size);

#if 0
    /* init the direct frame buffer */
    DirectFBInit(&argc, &argv );
#endif

    if (argc && argv) {
        int i = 0;
        for (i = 1; i < argc; i++) {
            if (strcmp (argv[i], "-c") == 0) {
                ps_config_file_name = argv[i + 1];
            }
        }
    }

    set_mtrace_signal();
    
    x_sys_start (pv_mem_addr, SYS_MEM_SIZE, (VOID*)ps_config_file_name);
    
    c_rpc_init_server();

#if ENABLE_AM_START_APPCVT /*Yan CLI*/
    if((argc > 1) && (argv != NULL))
    {
        for (ui4_loop_i = 1; ui4_loop_i < argc; ui4_loop_i++)
        {
            if(x_strcmp(argv[ui4_loop_i], RUN_TIME_OPTION)==0)
            {
			    i4_return = cmd_convert_init();
			    if (i4_return != 0)
			    {
			    	close(i4_fdmem);
                    close(i4_fdioctrl);
			        return (i4_return);
			    }

			    i4_return = x_thread_create(&h_svc_cli_by_am_thrd,
			                             SVC_CLI_BY_AM_THRD_NAME,
			                             SVC_CLI_BY_AM_THRD_DEFAULT_STACK_SIZE,
			                             SVC_CLI_BY_AM_THRD_DEFAULT_PRIORITY,
			                             _svc_cli_by_am_thread,
			                             0,
			                             NULL);
			    if (i4_return != OSR_OK)
			    {		
			    	close(i4_fdmem);
                    close(i4_fdioctrl);
			        return (i4_return);
			    }

                break;
            }
/*
            else if(x_strcmp(argv[ui4_loop_i], CLI_OPTION)==0)
            {
                close(0);
                open("/dev/ttyS0", O_RDWR);
            }
*/
        }
    }
#endif

    tv_cli_approach();

    return (i4_return);
}

