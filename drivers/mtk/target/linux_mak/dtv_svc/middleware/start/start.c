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
 * $RCSfile: start.c,v $
 * $Revision: #6 $
 * $Date: 2012/07/16 $
 * $Author: hongjun.chu $
 *
 * Description:
 *         This file contains all Middleware startup implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "aee/aee.h"
#ifdef MW_ATV_MANAGER
#include "atv_mngr/atv_mngr.h"
#include "atv_mngr/x_atv_mngr.h"
#endif
#include "cdb/cdb_api.h"
#include "cli/cli.h"
#include "config/cfg.h"
#include "conn_mngr/cm.h"
#include "dbg/dbg.h"
#include "dt/dt.h"
#include "dsm/dsm.h"
#include "evctx/x_evctx_slctr.h"
#include "rrctx/x_rrctx_slctr.h"
#include "file_mngr/fm_ufs.h"
#include "file_mngr/x_fm.h"
#include "flm/flm.h"
#include "font/x_fe.h"
#include "graphic/gl.h"
#include "handle/handle.h"
#include "inc/common.h"
#include "io_mngr/iom.h"
/*#include "network/inc/net_if.h"*/
#include "os/inc/os.h"
#include "prc_mngr/prc_mngr.h"
#include "res_mngr/rm.h"
#include "res_mngr/drv/rm_drv_mc_nfy.h"
#include "scdb/scdb_api.h"
#include "start/start.h"
#include "start/version.h"
#include "strm_mngr/sm_slctr.h"
#include "svctx/svctx.h"
#include "svl/svl.h"
#include "svl_bldr/sb_slctr.h"
#include "tbl_mngr/sec_mngr/secm_api.h"
#include "tbl_mngr/tm.h"
#include "time_msrt/time_msrt.h"
#include "tsl/tsl.h"
#include "wgl/wgl.h"
#include "pcl/pcl.h"
#ifdef MW_FAST_INIT_SUSPEND
#ifndef MW_FAST_INIT_SUSPEND_CUST
#include "pcl/x_pcl.h"
#endif
#endif
#include "vbi_fltr/vbi_fltr.h"
#include "dm/dm.h"
#include "cl/zip_eng/x_cl_zip_eng.h"
#include "rwlock/rwl_cli.h" 
#ifndef NO_NWL_INIT
#include "nwl/nwl.h"
#endif
#ifndef NO_LOL_INIT
#include "lol/lol.h"
#endif
#ifdef MW_DVBS_DEV_ENABLE 
#include "satl/satl.h"
#endif
#ifndef NO_LST_MNGR_INIT
#include "lst_mngr/lst_mngr.h"
#endif

#ifdef MW_EDB_SUPPORT
#include "edb/edb_slctr.h"
#include "edb/edb_pool.h"
#endif



#ifdef TIME_MEASUREMENT
#include "time_msrt/u_time_msrt_name.h"
#endif

#ifdef FIRMWARE_UPGRADE
#include "dlm/x_dlm.h"
#endif

#include "mtp_mngr/x_mtp.h"

#include "df_mngr/df_mngr.h"

#include "chip_spec/chip_spec.h"

#ifdef ANDROID
#include <stdio.h>
#include <time.h>
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Abort stuff. */
#ifdef ABORT
#undef ABORT
#endif

#define ABORT(_cat, _val) dbg_abort ((DBG_MOD_START | _cat | _val))

#define DBG_ABRT_RTOS_INIT_FAILURE      ((UINT32) 0x00000000)
#define DBG_ABRT_DRV_INIT_FAILURE       ((UINT32) 0x00000001)
#define DBG_ABRT_CUSTOM_INIT_FAILURE    ((UINT32) 0x00000002)
#define DBG_ABRT_APPL_INIT_FAILURE      ((UINT32) 0x00000003)
#define DBG_ABRT_INTERNAL_INIT_FAILURE  ((UINT32) 0x00000004)
#define DBG_ABRT_CREATE_SYS_SEMA        ((UINT32) 0x00000005)
#define DBG_ABRT_WAIT_SYS_SEMA          ((UINT32) 0x00000006)
#define DBG_ABRT_UNLOCK_SYS_SEMA        ((UINT32) 0x00000007)
#define DBG_ABRT_CREATE_SYS_THREAD      ((UINT32) 0x00000008)
#define DBG_ABRT_CLI_REG_FAILURE        ((UINT32) 0x00000009)

/* Initialization thread arguments. */
#define INIT_THREAD_NAME        "init_thread"
#define INIT_THREAD_STACK_SIZE  ((SIZE_T) 1024*8)
#ifndef INIT_THREAD_PRIORITY
#define INIT_THREAD_PRIORITY    ((UINT8)  50)
#endif

#define INIT_SYS_SET_TIMESTAMP(watch_tag)     x_os_set_timestamp(watch_tag)

typedef enum{    
    INIT_SYS_START = 0,
    INIT_SYS_AFTER_X_DRV_INIT,
    INIT_SYS_BEFORE_X_APPL_INIT,
    INIT_SYS_AFTER_X_APPL_INIT,
    INIT_SYS_END
}SYS_INIT_T;

static CHAR _aszInitSYSStr[][19] = 
{
    "inisys-start",            
    "af-x_drv_init",            
    "be-x_appl_init",             
    "af-x_appl_init",             
    "inisys-end",
};

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static BOOL b_init = FALSE;    /* Keeps track of the initialization status. */

static UINT64 ui8_init_map = INIT_MAP_ALL;

static HANDLE_T  h_sys_sema;  /* Used to synchronize the init thread. */

static X_CONFIG_T*  pt_sys_config;


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
#ifdef SYS_MEM_CHK
#ifndef LINUX_TURNKEY_SOLUTION
extern VOID x_handle_dbg_log_start(VOID);
#endif
extern INT32 os_cli_init(VOID);
#endif

static VOID init_sys (VOID);
static VOID init_sys_thread_main (VOID* pv_arg);
static VOID init_sys_thread (VOID);

#ifdef ANDROID
#define ANDROID_LOG_WITH_TIMESTAMP(x)     \
do {                                      \
    struct timespec time;                        \
    clock_gettime(CLOCK_MONOTONIC, &time); \
    printf("{At 00:00:%lu:%lu}", time.tv_sec, time.tv_nsec / 1000000);      \
    printf x ;                            \
} while(0)
#endif



/*-----------------------------------------------------------------------------
 * Name: init_sys
 *
 * Description: This API initializes the rest of the system. Note that this API
 *              may be called from a system thread or from the boot thread. If
 *              any of the initializations fail, this routine will abort. Note
 *              that this API accesses the variable "pt_sys_config".
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
extern INT32 x_custom_init_boot (GEN_CONFIG_T*  pt_config);
static VOID init_sys (VOID)
{
    /* At this stage, we can initialize the debug libraries trace  */
    /* buffer. Someone might actually need it. Note that we ignore */
    /* the debug libraries return value.                           */
    
    INIT_SYS_SET_TIMESTAMP(_aszInitSYSStr[INIT_SYS_START]);
#ifndef NO_DBG_TRACE_INIT
    if ((ui8_init_map & INIT_MAP_DBG_TRACE) != 0)
    {
        dbg_trace_init (pt_sys_config->ui2_dbg_num_trace_recs);
    }
#endif

#ifndef NO_RM_INIT
    if ((ui8_init_map & INIT_MAP_RES_MNGR) != 0)
    {
        if (rm_init (pt_sys_config->ui2_num_comps, &(pt_sys_config->t_rm_thread)) != RMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }

        /* Initialize the multi client notification handlers. */
        rm_drv_mc_nfy_init ();
    }
     /* Initialize TMS module right here so that every other module
               could use it. */
#ifdef TIME_MEASUREMENT
    if ((ui8_init_map & INIT_MAP_TIME_MSRT) != 0)
    {
        if (time_msrt_init (128) != TMSR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif /* TIME_MEASUREMENT */
#endif

#if 0
#ifdef MW_FAST_INIT_SUSPEND
#ifndef MW_FAST_INIT_SUSPEND_CUST
   // before x_drv_init
   if (x_pcl_set_power_suspend() != PCLR_OK)
   {
       ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
   }
#endif
#endif
#endif

#if 0
#ifdef MW_FAST_INIT_SUSPEND
#ifndef MW_FAST_INIT_SUSPEND_CUST
    #if 0 // def MW_FAST_INIT_USE_SOFTWARE_SUSPEND
    system("mkswap /dev/mtdblock11");
    system("swapon /dev/mtdblock11");
    system("echo reboot > /sys/power/disk");
    //system("echo disk > /sys/power/state");//trigger in x_drv_init
    #else // if defined(MW_FAST_INIT_USE_TUXONICE)
    system("echo 1 > /proc/sys/vm/drop_caches");
    system("mkswap /dev/mtdblock11");
    system("swapon /dev/mtdblock11");
    //system("cd /sys/power/tuxonice");
    system("echo swap:/dev/mtdblock11 > /sys/power/tuxonice/resume");
    system("echo 1 > /sys/power/tuxonice/no_pageset2");
    system("echo 0 > /sys/power/tuxonice/user_interface/enabled");
    //system("echo > /sys/power/tuxonice/do_hibernate");
    #endif
#endif
#endif
#endif

#ifndef NO_X_DRV_INIT
    /* And now, let the drivers initialize. */
    /* Before anything else gets done, initialize the RTOS. */
    if ((ui8_init_map & INIT_MAP_X_DRV) != 0)
    {
    	  #ifdef ANDROID
    	  ANDROID_LOG_WITH_TIMESTAMP(("<<<<<Before x_drv_init() ...>>>>>\n"));
    	  #endif
        if (x_drv_init (&(pt_sys_config->t_drv_config)) != INITR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_DRV_INIT_FAILURE);
        }
        #ifdef ANDROID
    	  ANDROID_LOG_WITH_TIMESTAMP(("<<<<<After x_drv_init() ...>>>>>\n"));
    	  #endif
    }
#endif
    INIT_SYS_SET_TIMESTAMP(_aszInitSYSStr[INIT_SYS_AFTER_X_DRV_INIT]);

#if 1

#ifdef SYS_DRV_POST_START
    if (pt_sys_config->pt_drv_post_start_fct)
    {
        pt_sys_config->pt_drv_post_start_fct(NULL);
    }
#endif


#ifdef ANDROID
    ANDROID_LOG_WITH_TIMESTAMP(("<<<<<Before MW init ...>>>>>\n"));
#endif


#ifdef MW_ATV_MANAGER
    /* Initialize ATV manager */
    if ((ui8_init_map & INIT_MAP_ATV_MNGR) != 0)
    {
        if (atv_manager_init(&(pt_sys_config->t_atv_mngr_cfg)) != ATVMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

    /* At this stage, we can initialize the debug libraries output device. */
    /* Note that we ignore the debug libraries return value.               */
#ifndef NO_DBG_OUTPUT_INIT
    if ((ui8_init_map & INIT_MAP_DBG_OUTPUT) != 0)
    {
        dbg_output_init (pt_sys_config->ps_dbg_output_name,
                         pt_sys_config->e_dbg_output_type,
                         pt_sys_config->pv_dbg_output_info);
    }
#endif

#ifndef NO_TM_INIT
    /* And now, initialize the rest of the middleware. */
    if ((ui8_init_map & INIT_MAP_TBL_MNGR) != 0)
    {
        if (tm_init () != TMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_SECM_INIT
    if ((ui8_init_map & INIT_MAP_SEC_MNGR) != 0)
    {
        if (secm_init (pt_sys_config->ui2_log_filter,
                       pt_sys_config->ui2_sec_req,
                       pt_sys_config->ui2_max_req,
                       &(pt_sys_config->t_secm_thread)) != SECMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

    /*
       Initialize File Manager library. This initialization
       is done prior to CoreDB initialization.
    */
#ifndef NO_FM_INIT
    if ((ui8_init_map & INIT_MAP_FM) != 0)
    {
        if (fm_init() != FMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

//printf("init_sys  %d \n",__LINE__);

#ifndef NO_DM_INIT
    /*
     *  temporarily ignore auto mount parameter
     */
    if ((ui8_init_map & INIT_MAP_DM) != 0)
    {
        if (dm_init(&(pt_sys_config->t_dm_init)) != DMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_CL_ZIP_INIT
    if ((ui8_init_map & INIT_MAP_CL_ZIP) != 0)
    {
        if ( x_cl_zip_init() != TRUE )
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif    

#ifndef NO_DB_CORE_INIT
    if ((ui8_init_map & INIT_MAP_CORE_DB) != 0)
    {
        if (db_core_init (pt_sys_config->ui2_cdb_num_db) != DBR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_SCDB_INIT
    if ((ui8_init_map & INIT_MAP_STREAM_COMP_DB) != 0)
    {
        if (scdb_init () != SCDBR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_SVL_INIT
    if ((ui8_init_map & INIT_MAP_SVL) != 0)
    {
        if (svl_init (pt_sys_config->ui2_svl_max_num) != SVLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_TSL_INIT
    if ((ui8_init_map & INIT_MAP_TSL) != 0)
    {
        if (tsl_init (pt_sys_config->ui2_tsl_max_num) != TSLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_NWL_INIT 
    if ((ui8_init_map & INIT_MAP_NWL) != 0)
    {
        if (nwl_init (pt_sys_config->ui2_nwl_max_num) != NWLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif    
    

#ifndef NO_LOL_INIT 
    if ((ui8_init_map & INIT_MAP_LOL) != 0)
    {
        if (lol_init (pt_sys_config->ui2_nwl_max_num) != LOLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif    
    
#ifdef MW_DVBS_DEV_ENABLE
#ifndef NO_SATL_INIT 
    if ((ui8_init_map & INIT_MAP_SATL) != 0)
    {
        if (satl_init (pt_sys_config->ui2_satl_max_num) != SATLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif    
#endif

#ifndef NO_LST_MNGR_INIT 
    if ((ui8_init_map & INIT_MAP_LST_MNGR) != 0)
    {
        UINT16 ui2_max_num = 0;
        if (pt_sys_config->ui2_svl_max_num > ui2_max_num)
        {
            ui2_max_num = pt_sys_config->ui2_svl_max_num;
        }
        if (pt_sys_config->ui2_tsl_max_num > ui2_max_num)
        {
            ui2_max_num = pt_sys_config->ui2_tsl_max_num;
        }
        #ifndef NO_NWL_INIT 
        if (pt_sys_config->ui2_nwl_max_num > ui2_max_num)
        {
            ui2_max_num = pt_sys_config->ui2_nwl_max_num;
        }
        #endif
        #ifndef NO_SATL_INIT
        if (pt_sys_config->ui2_satl_max_num > ui2_max_num)
        {
            ui2_max_num = pt_sys_config->ui2_satl_max_num;
        }
        #endif
        if (lst_mngr_init (ui2_max_num) != LSTMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif    


#ifndef NO_CM_INIT
    if ((ui8_init_map & INIT_MAP_CONN_MNGR) != 0)
    {
        if (cm_init (pt_sys_config->ui2_cm_num_handlers) != CMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif



#ifndef NO_PM_INIT
    if ((ui8_init_map & INIT_MAP_PROC_MNGR) != 0)
    {
        if (pm_init (pt_sys_config->ui2_pm_num_ca,
                     pt_sys_config->ui2_pm_num_service,
                     pt_sys_config->ui2_pm_num_comps,
                     &(pt_sys_config->t_pm_thread)) != PMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifdef CHIP_SPEC_SUPPORT
    #ifndef NO_X_CHIP_SPEC_INIT
    if ((ui8_init_map & INIT_TYPE_CHIP_SEPC) != 0)
    {
        if (chip_spec_init() != CHIPSPECR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_CUSTOM_INIT_FAILURE);
        }
    }
    #endif
#endif


#ifdef MW_FAST_BOOT_PRE_CONNECT
#ifndef NO_X_CUSTOM_INIT
    /* Once the middleware has ben initialized, initialize the customization. */
    if ((ui8_init_map & INIT_MAP_X_CUSTOM) != 0)
    {
        if (x_custom_init_boot(&(pt_sys_config->t_custom_config)) != INITR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_CUSTOM_INIT_FAILURE);
        }
    }
#endif
#endif

#ifdef MW_FAST_BOOT_PRE_CONNECT
    if ((ui8_init_map & INIT_MAP_PRE_CONNECT) != 0)
    {
        if (cm_pre_connect() != CMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_SM_INIT
    if ((ui8_init_map & INIT_MAP_STREAM_MNGR) != 0)
    {
        if (sm_init (&(pt_sys_config->t_sm_thread)) != SMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_FONT_INIT
    if ((ui8_init_map & INIT_MAP_FONT) != 0)
    {
        if (fe_init(pt_sys_config->ui4_hori_dpi,
                    pt_sys_config->ui4_vert_dpi) != FER_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_SB_INIT
    if ((ui8_init_map & INIT_MAP_SB) != 0)
    {
        if (sb_init (&(pt_sys_config->t_sb_thread)) != SBR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_AEE_INIT
    if ((ui8_init_map & INIT_MAP_AEE) != 0)
    {
        if (aee_init() != AEER_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_CFG_INIT
    if ((ui8_init_map & INIT_MAP_CFG) != 0)
    {
        if (cfg_init() != CFGR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_FLM_INIT
    if ((ui8_init_map & INIT_MAP_FLM) != 0)
    {
        if (flm_init(pt_sys_config->ui2_flm_max_num, pt_sys_config->ui2_flm_max_size) != FLMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_EVCTX_INIT
    if ((ui8_init_map & INIT_MAP_EVCTX) != 0)
    {
        if (evctx_init (&(pt_sys_config->t_evctx_thread)) != EVCTXR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_EDB_INIT
    if ((ui8_init_map & INIT_MAP_EDB) != 0)
    {
#ifdef MW_EDB_SUPPORT
        if(pt_sys_config->z_edb_mem_size == 0)
        {
		    pt_sys_config->z_edb_mem_size = 12*1024*1024; 
        }
        if (edb_init (pt_sys_config->ui1_edb_max_num,pt_sys_config->z_edb_mem_size,&(pt_sys_config->t_edb_thread)) != EDBR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
        
        if (edb_pool_init() != EDBR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
#endif
        
    }
#endif


#ifndef NO_RRCTX_INIT
    if ((ui8_init_map & INIT_MAP_RRCTX) != 0)
    {
        if (rrctx_init (&(pt_sys_config->t_rrctx_thread)) != RRCTXR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif


#ifndef NO_IOM_INIT
    if ((ui8_init_map & INIT_MAP_IO_MNGR) != 0)
    {
#ifndef ANDROID
        if (iom_init (pt_sys_config->b_apdemon_irrc_enabled,
                      &(pt_sys_config->t_iom_thread),
                      pt_sys_config->e_colormode,
                      pt_sys_config->ui4_width,
                      pt_sys_config->ui4_height) != IOMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
#else
        if (iom_init (pt_sys_config->b_apdemon_irrc_enabled,
                      &(pt_sys_config->t_iom_thread),
                      0,
                      0,
                      0) != IOMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
#endif
    }
#endif


#ifndef NO_GL_INIT
    if ((ui8_init_map & INIT_MAP_GRAPHIC_LIB) != 0)
    {
        if (gl_init (pt_sys_config->e_colormode, pt_sys_config->ui4_width, pt_sys_config->ui4_height) != GLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_WGL_INIT
    if ((ui8_init_map & INIT_MAP_WIDGET_LIB) != 0)
    {
/* For Linux we need a version with IOM not initialized */
#ifdef LINUX_TURNKEY_SOLUTION_PHASE2                      
/*#ifdef WGL_WITHOUT_IOM*/
        if (wgl_init_without_iom(
#else
        if (wgl_init (
#endif        
            pt_sys_config->ui4_width,
            pt_sys_config->ui4_height,
            pt_sys_config->e_colormode,
            pt_sys_config->b_auto_scale_view,
            pt_sys_config->b_no_inter_compose,
            pt_sys_config->b_support_compose_anim,
            pt_sys_config->b_osd_compress,
            pt_sys_config->b_compressed_compose,
            pt_sys_config->ui2_clut_sz,
            pt_sys_config->pt_clut,
            &(pt_sys_config->t_color_key),
            pt_sys_config->ui2_num_widgets,
            pt_sys_config->ui1_max_floated_widget,
            pt_sys_config->ui2_key_first_repeat_delay,
            pt_sys_config->ui2_key_repeat_delay,
            &(pt_sys_config->t_wm_thread)) != WGLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_SVCTX_INIT
    if ((ui8_init_map & INIT_MAP_SVCTX) != 0)
    {
        if (svctx_init (pt_sys_config->ui2_svctx_max_num,
                        &(pt_sys_config->t_svctx_thread)) != SVCTXR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifndef NO_VBIF_INIT
    if ((ui8_init_map & INIT_MAP_VBI_FLTR) != 0)
    {
        if (vbif_init ( &(pt_sys_config->t_vbif_thread) ) != VBIFR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

#ifdef ANDROID
    ANDROID_LOG_WITH_TIMESTAMP(("<<<<<Before MW customized parts init ...>>>>>\n"));
#endif

#ifndef NO_X_CUSTOM_INIT
    /* Once the middleware has ben initialized, initialize the customization. */
    if ((ui8_init_map & INIT_MAP_X_CUSTOM) != 0)
    {
        if (x_custom_init (&(pt_sys_config->t_custom_config)) != INITR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_CUSTOM_INIT_FAILURE);
        }
    }
#endif
//printf("init_sys  %d \n",__LINE__);

#ifdef ANDROID
    ANDROID_LOG_WITH_TIMESTAMP(("<<<<<After MW customized parts init ...>>>>>\n"));
#endif

#ifndef NO_DT_INIT
    /*
       Initialize system DateTime library. This initialization
       must be called after x_driver_init() and table manager
       init(), at this point we will try to get the current time
       from the time table download from the stream.
    */
    if ((ui8_init_map & INIT_MAP_DATE_TIME_LIB) != 0)
    {
        if (dt_init (&(pt_sys_config->t_dt_config)) != DTR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif

//printf("init_sys  %d \n",__LINE__);


#ifdef MW_ISDB_SUPPORT
#ifndef NO_DFM_INIT
    /*
       Initialize system download function manager library in ISDB spec. 
    */
    if ((ui8_init_map & INIT_MAP_DFM) != 0)
    {
        if (dfm_init (pt_sys_config->ui2_num_svc_objs, pt_sys_config->ui2_num_logo_objs) != DFMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif
#endif /*MW_ISDB_SUPPORT*/
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_DSM_INIT
    /*
       Initialize DSM module.
    */
    if ((ui8_init_map & INIT_MAP_DEV_STAT_MNGR) != 0)
    {
        if (dsm_init (&(pt_sys_config->t_dsm_config)) != DSMR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif    
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_PCL_INIT
    if ((ui8_init_map & INIT_MAP_PCL) != 0)
    {
        if (pcl_init () != PCLR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
#endif
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_MTP_INIT
    #ifdef MTP_ENABLE
    if ((ui8_init_map & INIT_MAP_MTP) != 0)
    {
        if (mtp_init () != MTPR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }
    }
    #endif    
#endif
//printf("init_sys  %d \n",__LINE__);

#ifdef MW_FAST_INIT_SUSPEND
#ifndef MW_FAST_INIT_SUSPEND_CUST
   if (x_pcl_set_power_suspend() != PCLR_OK)
   {
       ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
   }
#endif
#endif
//printf("init_sys  %d \n",__LINE__);

#ifndef NO_X_APPL_INIT
    /* And at last, initialize the application part. */
    if ((ui8_init_map & INIT_MAP_X_APPL) != 0)
    {
        INIT_SYS_SET_TIMESTAMP(_aszInitSYSStr[INIT_SYS_BEFORE_X_APPL_INIT]);
        if (0)//x_appl_init (&(pt_sys_config->t_appl_config)) != INITR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_APPL_INIT_FAILURE);
        }
        INIT_SYS_SET_TIMESTAMP(_aszInitSYSStr[INIT_SYS_AFTER_X_APPL_INIT]);
    }
#endif
//printf("init_sys  %d \n",__LINE__);

#ifdef TIME_MEASUREMENT
    TMS_BEGIN (TMS_SBY_TO_PWR);
#endif
//printf("init_sys  %d \n",__LINE__);

#ifdef CLI_LVL_ALL
    rwl_cli_init();
#endif
	//printf("init_sys  %d \n",__LINE__);

    INIT_SYS_SET_TIMESTAMP(_aszInitSYSStr[INIT_SYS_END]);
    
#ifdef ANDROID
    ANDROID_LOG_WITH_TIMESTAMP(("<<<<<After MW init ...>>>>>\n"));
#endif
	printf("init_sys over %d \n",__LINE__);
#endif
}


/*-----------------------------------------------------------------------------
 * Name: init_sys_thread_main
 *
 * Description: This is the system thread main entry and will initialize the
 *              system, unlock the waiting main entry and then terminate. If
 *              any error occurs, this API will abort.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID init_sys_thread_main (VOID* pv_arg)
{
    /* Initalize the system in the system thread. */
    init_sys ();

    /* And unlock the waiting thread. */
    if ((x_sema_unlock (h_sys_sema)) != OSR_OK)
    {
        ABORT (DBG_CAT_INIT, DBG_ABRT_UNLOCK_SYS_SEMA);
    }
}


/*-----------------------------------------------------------------------------
 * Name: init_sys_thread
 *
 * Description: This API will initialize the rest of a system directly or
 *              indirectly va a specialy created initialization thread. If this
 *              API detects any error, it will simply abort.
 *
 * Inputs:  pt_config  References the configuration structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID init_sys_thread (VOID)
{
    HANDLE_T h_sys_thread;


    h_sys_sema = NULL_HANDLE;

    /* If the handle or OS library are not initialized, do a direct system init. */
    if (((ui8_init_map & INIT_MAP_HANDLE) == 0)  ||
        ((ui8_init_map & INIT_MAP_OS)     == 0))
    {
        init_sys ();
    }
    else
    {
        /* Create binary semaphore to wait on. */
        if ((x_sema_create (&h_sys_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK)) != OSR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_CREATE_SYS_SEMA);
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
            ABORT (DBG_CAT_INIT, DBG_ABRT_CREATE_SYS_THREAD);
        }

        /* And now wait on semaphore until unlocked. */
        if ((x_sema_lock (h_sys_sema, X_SEMA_OPTION_WAIT)) != OSR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_WAIT_SYS_SEMA);
        }

        /* Ok, done, free up the semaphore resource. The initialization */
        /* thread will have terminated by itself.                       */
        x_sema_delete (h_sys_sema);
    }
}


/*-----------------------------------------------------------------------------
 * Name: start_set_init_map
 *
 * Description: This API sets the initialization map variable.
 *
 * Inputs:  ui8_set_init_map  Contains the initialization map.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID start_set_init_map (UINT64  ui8_set_init_map)
{
    ui8_init_map = ui8_set_init_map;
}


/*-----------------------------------------------------------------------------
 * Name: start_get_init_map
 *
 * Description: This API returns the initialization map variable.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The initialization map.
 ----------------------------------------------------------------------------*/
UINT64 start_get_init_map (VOID)
{
    return (ui8_init_map);
}


/*-----------------------------------------------------------------------------
 * Name: x_start_def_config
 *
 * Description: This API initializes the configuration structure to default
 *              values.
 *
 * Inputs:  pt_config  References the configuration structure.
 *
 * Outputs: pt_config  References the initialized configuration structure.
 *
 * Returns: INITR_OK       Routine successful.
 *          INITR_INV_ARG  Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_start_def_config (X_CONFIG_T*  pt_config)
{
    INT32 i4_return;


    i4_return = INITR_INV_ARG;

    if (pt_config != NULL)
    {
        i4_return = INITR_OK;

        x_memset (((VOID*) pt_config), 0, sizeof (X_CONFIG_T));

        /* Set some default values. */
#ifndef NO_RM_INIT
        pt_config->t_rm_thread.z_stack_size = RM_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_rm_thread.ui2_num_msgs = RM_NUM_OF_MSGS;
        pt_config->t_rm_thread.ui1_priority = RM_THREAD_DEFAULT_PRIORITY;
#endif

#ifndef NO_IOM_INIT
        pt_config->t_iom_thread.z_stack_size = IOM_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_iom_thread.ui2_num_msgs = IOM_NUM_OF_MSGS;
        pt_config->t_iom_thread.ui1_priority = IOM_THREAD_DEFAULT_PRIORITY;
#endif

#ifndef NO_PM_INIT
        pt_config->t_pm_thread.z_stack_size = PM_DEFAULT_STACK_SIZE;
        pt_config->t_pm_thread.ui2_num_msgs = PM_DEFAULT_NUM_MSGS;
        pt_config->t_pm_thread.ui1_priority = PM_DEFAULT_PRIORITY;
#endif

#ifndef NO_SM_INIT
        pt_config->t_sm_thread.z_stack_size = SM_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_sm_thread.ui2_num_msgs = SM_DEFAULT_NUM_OF_MSGS;
        pt_config->t_sm_thread.ui1_priority = SM_THREAD_DEFAULT_PRIORITY;
#endif

#ifdef FFR_FIX_ME_LATER
        pt_config->t_sb_thread.z_stack_size = 1;
        pt_config->t_sb_thread.ui2_num_msgs = 1;
        pt_config->t_sb_thread.ui1_priority = 1;

        pt_config->t_evctx_thread.z_stack_size = 1;
        pt_config->t_evctx_thread.ui2_num_msgs = 1;
        pt_config->t_evctx_thread.ui1_priority = 1;
#endif

#ifndef NO_RRCTX_INIT
        pt_config->t_rrctx_thread.z_stack_size = RRCTX_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_rrctx_thread.ui2_num_msgs = RRCTX_DEFAULT_NUM_OF_MSGS;
        pt_config->t_rrctx_thread.ui1_priority = RRCTX_THREAD_DEFAULT_PRIORITY;
#endif

#ifdef MW_ATV_MANAGER
        pt_config->t_atv_mngr_cfg.t_rtx_thread_info.z_stack_size = ATVM_RTX_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_atv_mngr_cfg.t_rtx_thread_info.ui2_num_msgs = ATVM_RTX_NUM_OF_MSGS;
        pt_config->t_atv_mngr_cfg.t_rtx_thread_info.ui1_priority = ATVM_RTX_THREAD_DEFAULT_PRIORITY;
#endif

#ifndef NO_WGL_INIT
        pt_config->t_wm_thread.z_stack_size = WM_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_wm_thread.ui2_num_msgs = WM_NUM_OF_MSGS;
        pt_config->t_wm_thread.ui1_priority = WM_MSG_DEFAULT_PRIORITY;
#endif

#ifndef NO_VBIF_INIT
        pt_config->t_vbif_thread.z_stack_size = VBIF_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_vbif_thread.ui2_num_msgs = VBIF_NUM_OF_MSGS;
        pt_config->t_vbif_thread.ui1_priority = VBIF_THREAD_DEFAULT_PRIORITY;
#endif

        pt_config->t_cli_thread.z_stack_size = CLI_THREAD_DEFAULT_STACK_SIZE;
        pt_config->t_cli_thread.ui2_num_msgs = CLI_NUM_OF_MSGS;
        pt_config->t_cli_thread.ui1_priority = CLI_THREAD_DEFAULT_PRIORITY;

#ifdef FFR_FIX_ME_LATER
        pt_config->t_secm_thread.z_stack_size = 1;
        pt_config->t_secm_thread.ui2_num_msgs = 1;
        pt_config->t_secm_thread.ui1_priority = 1;
#endif

    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_start
 *
 * Description: This API is the entry point for the Middleware software
 *              initialization.
 *
 * Inputs:  pt_config  References the configuration structure.
 *
 * Outputs: -
 *
 * Returns: INITR_OK            Routine successful.
 *          INITR_ALREDAY_INIT  Alreday initialized.
 *          INITR_INV_ARG       Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 x_start (X_CONFIG_T*  pt_config)
{
    INT32 i4_return;


    i4_return = INITR_ALREADY_INIT;

    if (! (b_init))
    {
        i4_return = INITR_INV_ARG;

        if (pt_config != NULL)
        {
            i4_return = INITR_OK;

            /* Initialize the RTOS first. */
            if ((ui8_init_map & INIT_MAP_X_RTOS) != 0)
            {
                if (x_rtos_init (&(pt_config->t_rtos_config)) != INITR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_RTOS_INIT_FAILURE);
                }
            }

            /* Before anything else gets done, initialize the */
            /* model name and serial number.                  */
            if ((ui8_init_map & INIT_MAP_VERSION) != 0)
            {
                if (start_set_model_name (pt_config->ps_model_name, &(pt_config->pv_mem_addr), &(pt_config->z_mem_size)) != INITR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
                }

                if (start_set_serial_number (pt_config->ps_serial_number, &(pt_config->pv_mem_addr), &(pt_config->z_mem_size)) != INITR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
                }
            }

            /* And at last, initialize the handle and os library and resource manager. */
            if ((ui8_init_map & INIT_MAP_HANDLE) != 0)
            {
                if (handle_init (pt_config->ui2_num_handles, &(pt_config->pv_mem_addr), &(pt_config->z_mem_size)) != HR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
                }
            }

            if ((ui8_init_map & INIT_MAP_OS) != 0)
            {
                if (os_init (pt_config->pv_mem_addr, pt_config->z_mem_size) != OSR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
                }
            }

#ifdef SYS_MEM_CHK
#ifndef LINUX_TURNKEY_SOLUTION
            x_handle_dbg_log_start();
#endif
            x_mem_dbg_log_start();
            x_msg_q_dbg_log_start();
            x_thread_dbg_log_start();
#endif
            /* Initialize CLI module right here so that every other module */
            /* can register itself with the CLI module.                    */
            if ((ui8_init_map & INIT_MAP_CLI) != 0)
            {
                if (cli_init (&(pt_config->t_cli_thread)) != CLIR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
                }
            }

            /* Register the start module with the CLI module. This will allow the */
            /* printing of the BUILD, CUSTOMER, VERSION & MODEL name.             */
            if (start_reg_cli () != HR_OK)
            {
                ABORT (DBG_CAT_INIT, DBG_ABRT_CLI_REG_FAILURE);
            }

            /* Register the Handle library with the CLI module but */
            /* only if the Handle library was initalized.          */
            if ((ui8_init_map & INIT_MAP_HANDLE) != 0)
            {
                if (handle_reg_cli () != HR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_CLI_REG_FAILURE);
                }
            }

            /* Register the OS library with the CLI module but */
            /* only if the OS library was initalized.          */
            if ((ui8_init_map & INIT_MAP_OS) != 0)
            {
                if (os_reg_cli () != OSR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_CLI_REG_FAILURE);
                }
            }

#ifdef NETSTACK
            if ((ui8_init_map & INIT_MAP_NETSTACK) != 0)
            {
                if (net_netif_init () != OSR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
                }
            }
#endif

#ifndef ANDROID
#ifdef FIRMWARE_UPGRADE
            if ((ui8_init_map & INIT_MAP_DLM) != 0)
            {
                if (x_dlm_init () != OSR_OK)
                {
                    ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
                }
            }
#endif
#endif

            /* If this API returns then all went well. Consider */
            /* everything initialized.                          */
            pt_sys_config = pt_config;

            init_sys_thread ();

#ifdef SYS_MEM_CHK
            os_cli_init();
#endif

            /* Initialization done. */
            b_init = TRUE;
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_sys_exit
 *
 * Description: This API will exit the system. Note that this API will never
 *              return.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID x_sys_exit (VOID)
{
    DBG_ABORT (SYS_EXIT);
}
