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
 * $RCSfile: mem_cli.c,v $
 * $Revision: #2 $
 * $Date: 2012/06/26 $
 * $Author: xiaokuan.shi $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "inc/x_common.h"
#include "os/inc/os.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "os/semaphore/sema_lib.h"
#include "os/thread/thread_lib.h"
#include "os/msgQ/msgq_lib.h"
#include "os/memory/mem_lib.h"
#include "os/memory/mem_cli.h"


#ifdef CLI_LVL_GUEST
extern MEM_GLOBAL_T *pt_mem_global;  /* carved from default partition */
extern MEM_PART_T *pt_def_part;      /* default partition */
extern HANDLE_T h_def_part_hdl;      /* default partition handle */

static INT32 os_cli_show_mem_all(INT32 i4_argc, const CHAR** pps_argv);
static INT32 os_cli_show_mem_part_stats(INT32 i4_argc, const CHAR** pps_argv);
static INT32 os_cli_show_mem_part_detail(INT32 i4_argc, const CHAR** pps_argv);
static VOID os_mem_part_print(HANDLE_T h_part_hdl);
static INT32 os_cli_get_dbg_level(INT32 i4_argc, const CHAR** pps_argv);
static INT32 os_cli_set_dbg_level(INT32 i4_argc, const CHAR** pps_argv);

#ifdef LINUX_TURNKEY_SOLUTION
#if 0
static INT32 os_mem_cli_start_query(INT32 i4_argc, const CHAR   **pps_argv);
static INT32 os_mem_cli_show_mem(INT32 i4_argc, const CHAR   **pps_argv);
static INT32 os_mem_cli_stop_query(INT32 i4_argc, const CHAR   **pps_argv);
#endif
#endif
#endif

#ifdef CLI_LVL_ALL
static INT32 os_cli_mem_alloc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 os_cli_mem_realloc(INT32 i4_argc, const CHAR** pps_argv);
static INT32 os_cli_msgq_perf(INT32 i4_argc, const CHAR** pps_argv);
static INT32 os_cli_msgq_health_pause(INT32 i4_argc, const CHAR** pps_argv);
static INT32 os_cli_msgq_health_resume(INT32 i4_argc, const CHAR** pps_argv);

static VOID os_cli_ma1(VOID* pv_arg);
static VOID os_cli_ma2(VOID* pv_arg);
static VOID os_cli_ma3(VOID* pv_arg);
static VOID os_cli_ma4(VOID* pv_arg);
static VOID os_cli_ma5(VOID* pv_arg);
static VOID os_cli_ma6(VOID* pv_arg);
static VOID os_cli_mra1(VOID);
static VOID os_cli_msgq_rcv(VOID);
static VOID os_cli_msgq_send(VOID);

static HANDLE_T h_cli_ma_part;
static SIZE_T z_cli_ma_size = 1024;
static SIZE_T z_cli_ma_incr = 64;
static INT32 i4_cli_ma_loop = 100;
static UINT32 i4_cli_ma_delay = (UINT32) 3000;

static INT32 i4_cli_qp_loop = 100;
static SIZE_T z_cli_qp_size = 32;

#endif

#ifdef CLI_LVL_GUEST
/* Middleware OS command table */
static CLI_EXEC_T at_os_cmd_tbl[] =
{
 	{ "show_part_all",
      "spa",
      os_cli_show_mem_all,
      NULL,
      "Show all memory partitions",
      CLI_SUPERVISOR },

 	{ "show_part",
      "sp",
      os_cli_show_mem_part_stats,
      NULL,
      "sp [part_handle]",
      CLI_SUPERVISOR },

 	{ "show_part_detail",
      "spd",
      os_cli_show_mem_part_detail,
      NULL,
      "spd [part_handle]",
      CLI_SUPERVISOR },

#ifdef CLI_LVL_ALL
 	{ "mem_alloc",
      "ma",
      os_cli_mem_alloc,
      NULL,
      "ma <mem_size> <loop_count> <alloc_delay_in_ms>",
      CLI_SUPERVISOR },

 	{ "mem_realloc",
      "mra",
      os_cli_mem_realloc,
      NULL,
      "mra <mem_size> <mem_incr> <loop_count> <alloc_delay_in_ms>",
      CLI_SUPERVISOR },

 	{ "msgq_perf",
      "qp",
      os_cli_msgq_perf,
      NULL,
      "qp <msg_size> <loop_count>",
      CLI_SUPERVISOR },

    { "msgq_health_pause",
      "qhp",
      os_cli_msgq_health_pause,
      NULL,
      "Pause message queues health checking",
      CLI_SUPERVISOR },

    { "msgq_health_resume",
      "qhr",
      os_cli_msgq_health_resume,
      NULL,
      "Resume message queues health checking",
      CLI_SUPERVISOR },

 	{ "show_thread_all",
      "sta",
      os_cli_show_thread_all,
      NULL,
      "Show all threads",
      CLI_SUPERVISOR },

 	{ "show_sema_all",
      "ssa",
      os_cli_show_sema_all,
      NULL,
      "Show all semaphores",
      CLI_SUPERVISOR },

#if 0   /* later */
 	{ "pmerge",
      NULL,
      os_cli_mem_part_merge,
      NULL,
      "Syntax: pmerge [partition_handle]",
      CLI_SUPERVISOR },
#endif
#endif

 	{ CLI_GET_DBG_LVL_STR,
      NULL,
      os_cli_get_dbg_level,
      NULL,
      CLI_GET_DBG_LVL_HELP_STR,
      CLI_SUPERVISOR },

	{ CLI_SET_DBG_LVL_STR,
      NULL,
      os_cli_set_dbg_level,
      NULL,
      CLI_SET_DBG_LVL_HELP_STR,
      CLI_SUPERVISOR },
      
#ifdef LINUX_TURNKEY_SOLUTION
#if 0
    { "mem_monitor_on",
      "mmon",
      os_mem_cli_start_query,
      NULL,
      "mmon [Refresh Interval]",
      CLI_GUEST },

    { "mem_monitor_show",
      "mmshow",
      os_mem_cli_show_mem,
      NULL,
      "Print out peak memory usage",
      CLI_GUEST },

    { "mem_monitor_off",
      "mmoff",
      os_mem_cli_stop_query,
      NULL,
      "Stop memory monitor",
      CLI_GUEST },
#endif      
#endif

    END_OF_CLI_CMD_TBL
};


/* Middleware OS root command table */
CLI_EXEC_T at_os_root_cmd_tbl[] =
{
    { "os",
      NULL,
      NULL,
      at_os_cmd_tbl,
      "OS Commands",
#ifdef SYS_RES_CHK
      CLI_GUEST },
#else
      CLI_SUPERVISOR },
#endif

    END_OF_CLI_CMD_TBL
};


/*-----------------------------------------------------------------------------
 * Name: os_cli_get_dbg_level
 *
 * Description: This API gets the current setting of IO Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32
os_cli_get_dbg_level(INT32        i4_argc,
                     const CHAR   **pps_argv)
{
    INT32       i4_return;


	if (i4_argc != 1)
	{
		x_dbg_stmt("os.gdl\n\r");
		return CLIR_INV_CMD_USAGE;
	}

    i4_return = x_cli_show_dbg_level(os_get_dbg_level());

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: os_cli_set_dbg_level
 *
 * Description: This API sets the debug level of IO Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32
os_cli_set_dbg_level(INT32        i4_argc,
                     const CHAR   **pps_argv)
{
    INT32       i4_return;
    UINT16      ui2_dbg_level;


    /* Check arguments */
	if ((i4_argc != 2) ||
	    (pps_argv == NULL))
	{
		x_dbg_stmt("OS [level]\n\r");
		x_dbg_stmt("  [level]: a=api, e=error, i=info, n=none\n\r");
		return CLIR_INV_CMD_USAGE;
	}

    i4_return = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_return == CLIR_OK)
    {
        if (os_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_return = CLIR_CMD_EXEC_ERROR;
        }
        else
        {
            i4_return = x_cli_show_dbg_level(os_get_dbg_level());
        }
    }
    else
    {
		x_dbg_stmt("OS [level]\n\r");
		x_dbg_stmt("  [level]: a=api, e=error, i=info, n=none\n\r");
		return CLIR_INV_CMD_USAGE;
    }

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: os_cli_show_mem_all
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_show_mem_all(INT32        i4_argc,
                    const CHAR   **pps_argv)
{
    HANDLE_T h_hdl;
    MEM_PART_T *pt_part;
    INT32 i4_ret;
    SIZE_T z_total_free;
    SIZE_T z_biggest_free;
    SIZE_T z_free_obj;
    SIZE_T z_allocated_obj;
    SIZE_T z_total_size;
    SIZE_T z_alloc_size;
    CHAR s_name[PART_NAME_LEN + 4];


    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return(OSR_NOT_INIT);
    }

    i4_ret = x_sema_lock(h_os_mem_gsema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);;
    }

    pt_part = pt_mem_global->pt_active;
    while (pt_part != NULL)
    {
        os_mem_cli_stats(pt_part->h_part_hdl,
                         &z_total_free,
                         &z_biggest_free,
                         &z_free_obj,
                         &z_allocated_obj,
                         &z_total_size,
                         &z_alloc_size,
                         &(s_name[0]));

        h_hdl = (pt_part->ui1_part_index == OS_MEM_DEFAULT_PART) ?
                                        NULL_HANDLE : pt_part->h_part_hdl;

        x_dbg_stmt("\nMemory Partition Statistics:\n \
                    Handle: %d\n \
                    Name: %s\n \
                    Allocation Type(%d): %s\n \
                    Total Size:         %8d Bytes\n \
                    Total Free Size:    %8d Bytes\n \
                    Biggest Free Block: %8d Bytes\n \
                    Allocated Blocks: %d\n \
                    Free Blocks: %d\n",
                    h_hdl, s_name,
                    z_alloc_size,
                    ((z_alloc_size == 0) ? "Dynamic" : "Fixed"),
                    z_total_size, z_total_free, z_biggest_free,
                    z_allocated_obj, z_free_obj);

        pt_part = pt_part->pt_next;
    }

    (VOID) x_sema_unlock(h_os_mem_gsema_hdl);

    return(CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: os_cli_show_mem_part_stats
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_show_mem_part_stats(INT32        i4_argc,
                           const CHAR   **pps_argv)
{
    HANDLE_T h_mem_hdl;
    SIZE_T z_total_free;
    SIZE_T z_biggest_free;
    SIZE_T z_free_obj;
    SIZE_T z_allocated_obj;
    SIZE_T z_total_size;
    SIZE_T z_alloc_size;
    INT32 i4_ret;
    CHAR s_name[32];


    if (i4_argc != (INT32) 0)
    {
        h_mem_hdl = (HANDLE_T) x_strtoull(pps_argv[1], NULL, 0);
        if (h_mem_hdl == (HANDLE_T) 0)
        {
            h_mem_hdl = NULL_HANDLE;
        }
    }
    else
    {
        h_mem_hdl = NULL_HANDLE;
    }

    i4_ret = os_mem_scan_mem(h_mem_hdl);
    if (i4_ret == OSR_OUT_BOUND)
    {
        x_dbg_stmt("Memory partition %d might have been corrupted\n",
                    h_mem_hdl);
    }

    os_mem_cli_stats(h_mem_hdl,
                     &z_total_free,
                     &z_biggest_free,
                     &z_free_obj,
                     &z_allocated_obj,
                     &z_total_size,
                     &z_alloc_size,
                     &(s_name[0]));

    x_dbg_stmt("\nMemory Partition Statistics:\n \
                Handle: %d\n \
                Name: %s\n \
                Allocation Type(%d): %s\n \
                Total Size:         %8d Bytes\n \
                Total Free Size:    %8d Bytes\n \
                Biggest Free Block: %8d Bytes\n \
                Allocated Blocks: %d\n \
                Free Blocks: %d\n",
                h_mem_hdl, s_name,
                z_alloc_size,
                ((z_alloc_size == 0) ? "Dynamic" : "Fixed"),
                z_total_size, z_total_free, z_biggest_free,
                z_allocated_obj, z_free_obj);

    return(CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: os_cli_show_mem_part_detail
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_show_mem_part_detail(INT32        i4_argc,
                           const CHAR** pps_argv)
{
    HANDLE_T h_mem_hdl;
    SIZE_T z_total_free;
    SIZE_T z_biggest_free;
    SIZE_T z_free_obj;
    SIZE_T z_allocated_obj;
    SIZE_T z_total_size;
    SIZE_T z_alloc_size;
    CHAR s_name[30];


    if (i4_argc != (INT32) 0)
    {
        h_mem_hdl = (HANDLE_T) x_strtoull(pps_argv[1], NULL, 0);
        if (h_mem_hdl == (HANDLE_T) 0)
        {
            h_mem_hdl = NULL_HANDLE;
        }
    }
    else
    {
        h_mem_hdl = NULL_HANDLE;
    }

    os_mem_part_print(h_mem_hdl);

    os_mem_cli_stats(h_mem_hdl,
                     &z_total_free,
                     &z_biggest_free,
                     &z_free_obj,
                     &z_allocated_obj,
                     &z_total_size,
                     &z_alloc_size,
                     &(s_name[0]));

    x_dbg_stmt("\nMemory Partition Statistics:\n \
                Handle: %d\n \
                Name: %s\n \
                Allocation Type(%d): %s\n \
                Total Size:         %8d Bytes\n \
                Total Free Size:    %8d Bytes\n \
                Biggest Free Block: %8d Bytes\n \
                Allocated Blocks: %d\n \
                Free Blocks: %d\n\n",
                h_mem_hdl, s_name,
                z_alloc_size,
                ((z_alloc_size == 0) ? "Dynamic" : "Fixed"),
                z_total_size, z_total_free, z_biggest_free,
                z_allocated_obj, z_free_obj);

    return(CLIR_OK);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_part_stats()
 *
 * Description:
 *      This routine prints detailed stats of a partition. it is called for
 *      for debug purpose. it performs
 *      1. check if memory was initialized.
 *      2. check handle validity (NULL_HANDLE for default partition).
 *      3. call os_mem_stats().
 *      4. go through bucket array and print out each memory obj.
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_part_stats(HANDLE_T h_part_hdl)
{
    MEM_FREEOBJ_T *pt_obj;
    MEM_BUCKET_T *pt_bucket;
    MEM_PART_T *pt_part;
    INT32 i4_ret, i4_i;


    if (b_os_mem_inited != TRUE)
    {
        return;
    }

    if (h_part_hdl == 0)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **)&pt_part);
        if (i4_ret != OSR_OK)
        {
            return;
        }
    }

    if ((pt_part->ui1_flags != OS_MEM_PART_FLAG_ACTIVE) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_EXTERNAL) &&
        (pt_part->ui1_flags != OS_MEM_PART_FLAG_PERMANENT))
    {
        return;
    }

    if (pt_part->z_alloc_size != OS_MEM_DYNAMIC_ALLOC)
    {
        return;
    }

    i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return;
    }

    /* pt_obj has a bucket index set at memory allocation time */
    i4_i = OS_MEM_FIXED_INDEX;
    pt_bucket = &(pt_part->t_bucket[i4_i]);

    x_dbg_stmt("Partition bucket index: min %d, max %d\n",
            pt_part->ui1_min_index, pt_part->ui1_max_index);

    while (i4_i < OS_MEM_BUCKET_SIZE)
    {
        GET_LINK_HEAD(pt_obj, pt_bucket->pt_head);

        if (LINK_NOT_EMPTY(pt_obj, NULL))
        {
            x_dbg_stmt("Bucket[%d]:\n", (int) i4_i);
            do
            {
                x_dbg_stmt("  0x%x/%dB/0x%x/0x%x, \n",
                       (int) pt_obj, (int) (pt_obj->z_size - sizeof(MEM_OBJ_T)),
                       (int) pt_obj->pt_ftend, (int) pt_obj->pt_bkend);

                GET_LINK_NEXT(pt_obj, pt_obj);
            } while (pt_obj != pt_bucket->pt_head);
        }
        ++i4_i;
        ++pt_bucket;
    }

    if (x_sema_unlock(pt_part->h_sema_hdl) != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }
}
#if 0
/*-----------------------------------------------------------------------------
 * Name: os_cli_mem_part_merge
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_mem_part_merge(INT32        i4_argc,
                      const CHAR   **pps_argv)
{
    HANDLE_T h_mem_hdl;


    if (i4_argc != (INT32) 0)
    {
        h_mem_hdl = (HANDLE_T) str_to_int(pps_argv[1]);
    }
    else
    {
        h_mem_hdl = NULL_HANDLE;
    }

    os_mem_part_merge(h_mem_hdl);

    return(CLIR_OK);
}
#endif


#if 0
/*-----------------------------------------------------------------------------
 * Name: os_cli_scan_mem_all
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_scan_mem_all(INT32        i4_argc,
                    const CHAR   **pps_argv)
{

    /* no arguments needed */

    os_mem_scan_mem_all();
    return(CLIR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: os_cli_scan_mem
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_scan_mem(INT32        i4_argc,
                const CHAR** pps_argv)
{
    HANDLE_T h_mem_hdl;
    INT32 i4_ret;


    /* mscan [partition_handle] */

    if (i4_argc != (INT32) 0)
    {
        /* just default partition for now */
        /*h_mem_hdl = (HANDLE_T) str_to_int(pps_argv[1]);*/
        h_mem_hdl = (HANDLE_T) 0;
    }
    else
    {
        h_mem_hdl = NULL_HANDLE;
    }

    i4_ret = os_mem_scan_mem(h_mem_hdl);
    if (i4_ret == OSR_OUT_BOUND)
    {
        x_dbg_stmt("Memory partition %d might have been corrupted\n",
                    h_mem_hdl);
    }
    else
    {
        x_dbg_stmt("Memory partition %d is OK\n", h_mem_hdl);
    }

    return(CLIR_OK);
}
#endif

/*----------------------------------------------------------------------------
 * Function: os_mem_scan_mem()
 *
 * Description:
 *      this routine scan a partition for integrety.
 *
 * Inputs:
 *      h_part_hdl : partition handle.
 *
 * Outputs: -
 *
 * Returns:
 *---------------------------------------------------------------------------*/
INT32
os_mem_scan_mem(HANDLE_T    h_part_hdl)
{
    MEM_PART_T *pt_part;
    MEM_FREEOBJ_T *pt_obj, *pt_last;
    UINT8 *start, *end;
    INT32 i4_ret;


    i4_ret = OSR_OK;

    if (h_part_hdl == NULL_HANDLE)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **) &pt_part);
    }

    if (i4_ret != OSR_OK)
    {
         return(i4_ret);
    }

    if (pt_part->z_alloc_size == (SIZE_T) OS_MEM_DYNAMIC_ALLOC)
    {
        i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
        if (i4_ret != OSR_OK)
        {
            return(OSR_FAIL);
        }

        start = (UINT8 *) pt_part->pv_start;
        end = start + pt_part->z_size;;
        pt_obj = pt_part->pt_first->pt_bkend;
        pt_last = pt_part->pt_last;
        while (pt_obj != pt_last)
        {
            if ((((UINT32) pt_obj->pt_ftend) & 0x3) ||
                ((UINT8 *) (pt_obj->pt_ftend) < start) ||
                (((UINT32) pt_obj->pt_bkend) & 0x3) ||
                ((UINT8 *) (pt_obj->pt_bkend) >= end) ||
                (pt_obj->ui1_index >= (UINT8) OS_MEM_BUCKET_SIZE) ||
                (pt_obj->ui1_part_index != pt_part->ui1_part_index) ||
                ((pt_obj->ui2_flags != OS_MEM_FLAG_FREE) &&
                 (pt_obj->ui2_flags != OS_MEM_FLAG_ALLOCATED)))
            {
                /* the memory object is probably corrupted */
                i4_ret = OSR_OUT_BOUND;
                break;
            }

            pt_obj = pt_obj->pt_bkend;
        }

        (VOID) x_sema_unlock(pt_part->h_sema_hdl);
    }

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_part_print()
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns:
 *---------------------------------------------------------------------------*/
VOID
os_mem_part_print(HANDLE_T h_part_hdl)
{
    MEM_PART_T *pt_part;
    MEM_FREEOBJ_T *pt_obj;
    INT32 i4_ret, i4_free, i4_alloc;


    i4_ret = OSR_OK;

    if (h_part_hdl == NULL_HANDLE)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **) &pt_part);
    }

    if (i4_ret != OSR_OK)
    {
         return;
    }

    i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return;
    }

    i4_free = i4_alloc = 0;
    pt_obj = pt_part->pt_first->pt_bkend;
    while (pt_obj != pt_part->pt_last)
    {
        if ((((UINT32) pt_obj->pt_ftend) & 0x3) ||
            (((UINT32) pt_obj->pt_bkend) & 0x3) ||
            (pt_obj->ui1_index >= (UINT8) OS_MEM_BUCKET_SIZE) ||
            (pt_obj->ui1_part_index != pt_part->ui1_part_index) ||
            ((pt_obj->ui2_flags != OS_MEM_FLAG_FREE) &&
             (pt_obj->ui2_flags != OS_MEM_FLAG_ALLOCATED)))
        {
            /* the memory object is probably corrupted */
            break;
        }

        if (pt_obj->ui2_flags == OS_MEM_FLAG_FREE)
        {
            ++i4_free;

            x_dbg_stmt("-->[%d] FREE, Mem %x (%d), ft %x, bk %x\n",
                        i4_free, pt_obj,
                        (((UINT32) pt_obj->pt_bkend - (UINT32) pt_obj) - sizeof(MEM_OBJ_T)),
                        pt_obj->pt_ftend, pt_obj->pt_bkend);
        }
        else if (pt_obj->ui2_flags == OS_MEM_FLAG_ALLOCATED)
        {
            ++i4_alloc;
#ifdef CLI_LVL_ALL
            x_dbg_stmt("[%d] Allocated at %x (%d), ft %x, bk %x; BY %-16s\n",
                        i4_alloc, pt_obj,
                        (((UINT32) pt_obj->pt_bkend - (UINT32) pt_obj) - sizeof(MEM_OBJ_T)),
                        pt_obj->pt_ftend, pt_obj->pt_bkend,
                        ((pt_obj->pv_thread != NULL)
                            ? (((OS_THREAD_T *) (pt_obj->pv_thread))->s_name)
                            : "???"));
#else
            x_dbg_stmt("[%d] Allocated at %x (%d), ft %x, bk %x; BY %-16s\n",
                        i4_alloc, pt_obj,
                        (((UINT32) pt_obj->pt_bkend - (UINT32) pt_obj) - sizeof(MEM_OBJ_T)),
                        pt_obj->pt_ftend, pt_obj->pt_bkend,
                        "???");
#endif
        }
        else
        {
            x_dbg_stmt("[%x] ??? Mem %x (%d), ft %x, bk %x\n",
                        pt_obj->ui2_flags, pt_obj,
                        (((UINT32) pt_obj->pt_bkend - (UINT32) pt_obj) - sizeof(MEM_OBJ_T)),
                        pt_obj->pt_ftend, pt_obj->pt_bkend);
        }

        pt_obj = pt_obj->pt_bkend;
    }

    (VOID) x_sema_unlock(pt_part->h_sema_hdl);
}


/*----------------------------------------------------------------------------
 * Function: os_mem_cli_stats()
 *
 * Description:
 *      this API returns statistics of specified partition. it performs
 *      1. check if memory was initialized.
 *      2. check handle validity (NULL_HANDLE for default partition).
 *      3. lock partion sema.
 *      3. go through partition free bucket.
 *
 * Inputs:
 *      h_part_hdl: memory partition handle.
 *
 * Outputs: -
 *      *z_totalfree: total size of all free memory block.
 *      *z_bigfree: size of biggest free memory block.
 *      *z_freeobj: number of free memory blocks.
 *      *z_allocatedobj: number of allocated memory blocks.
 *      *z_totalsize: total partition size.
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
os_mem_cli_stats(HANDLE_T h_part_hdl,
                 SIZE_T *z_totalfree,
                 SIZE_T *z_bigfree,
                 SIZE_T *z_freeobj,
                 SIZE_T *z_allocatedobj,
                 SIZE_T *z_totalsize,
                 SIZE_T *z_allocsize,
                 CHAR   *ps_name)
{
    INT32 i4_n, i4_ret;
    MEM_BUCKET_T *pt_bucket;
    MEM_FREEOBJ_T *pt_obj;
    MEM_PART_T *pt_part;

    *z_totalfree = 0;
    *z_bigfree = 0;
    *z_freeobj = 0;
    *z_allocatedobj = 0;
    *z_totalsize = 0;

    if ((pt_mem_global == NULL) || (b_os_mem_inited != TRUE))
    {
        return;
    }

    if (h_part_hdl == NULL_HANDLE)
    {
        pt_part = pt_def_part;
    }
    else
    {
        i4_ret = os_check_handle(h_part_hdl,
                                 HT_GROUP_OS_MEMORY,
                                 (VOID **)&pt_part);
        if (i4_ret != OSR_OK)
        {
            return;
        }
    }

    i4_ret = x_sema_lock(pt_part->h_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return;
    }

    if (pt_part->z_alloc_size == OS_MEM_DYNAMIC_ALLOC)
    {
        /* free memory */
        for (i4_n = OS_MEM_FIXED_INDEX , pt_bucket = &(pt_part->t_bucket[i4_n]);
                i4_n < OS_MEM_BUCKET_SIZE; i4_n++, pt_bucket++)
        {
            GET_LINK_HEAD(pt_obj, pt_bucket->pt_head);

            if (LINK_NOT_EMPTY(pt_obj, NULL))
            {
                do
                {
                    *z_totalfree += pt_obj->z_size;     /* total free memory */

                    if (*z_bigfree < pt_obj->z_size)
                    {
                        *z_bigfree = pt_obj->z_size;
                    }

                    GET_LINK_NEXT(pt_obj, pt_obj);
                } while (pt_obj != pt_bucket->pt_head);
            }
        }

        if (*z_bigfree >= sizeof(MEM_OBJ_T))
        {
            *z_bigfree -= (sizeof(MEM_OBJ_T) + 7) & ~0x7;
        }
        else
        {
            *z_bigfree = 0;
        }
    }
    else
    {
        *z_bigfree = pt_part->z_alloc_size;
        *z_totalfree = pt_part->z_alloc_size * pt_part->ui4_free_obj;
    }

    *z_allocsize = pt_part->z_alloc_size;
    *z_totalsize = pt_part->z_size;

    *z_allocatedobj = pt_part->ui4_alloc_obj;   /* total # of allocated obj*/
    *z_freeobj = pt_part->ui4_free_obj;

    x_strcpy(ps_name, pt_part->s_name);
    ps_name[PART_NAME_LEN] = '\0';

    if (x_sema_unlock(pt_part->h_sema_hdl) != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }
}

/*===============add for query memory info================*/
#ifdef LINUX_TURNKEY_SOLUTION
#if 0
static BOOL g_b_is_stop = FALSE;
static UINT32 g_ui4_qry_intv = 500; /*ms*/

struct mem_info {
    int used;
    int dfb_video;
    int dfb_system;
    int fbm;
};
static struct mem_info g_current_mem = {0};
static struct mem_info g_max_mem;


extern int DirectFBDump(int *video, int *system, int *fbm);

static inline VOID _os_mem_cli_show_mem(const CHAR *title, struct mem_info *info)
{
    x_dbg_stmt("%s\t:[used]%d kB, [dfb_video]%d kB, [dfb_system]%d kB, [fbm]%d kB\n",
                title,
                info->used,
                info->dfb_video/1024,
                info->dfb_system/1024,
                info->fbm/1024);
}

/*-----------------------------------------------------------------------------
 * Name: _os_mem_cli_read_proc_meminfo
 *
 * Description: Read /proc/meminfo into a buffer.
 *
 * Inputs: - file_buf: Starting address of output buffer
 *              buf_size: Size of the output buffer
 *
 * Outputs: - 
 *
 * Returns: -1 on error
 *           0 on success
 ----------------------------------------------------------------------------*/
static int _os_mem_cli_read_proc_meminfo(CHAR *file_buf, size_t buf_size)
{
    FILE *fp;
    size_t bytes_read;

    if (file_buf == NULL)
    {
        return -1;
    }

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        return -1;
    }
    bytes_read = fread(file_buf, 1, buf_size, fp);
    fclose(fp);

    if (bytes_read == 0 || bytes_read == buf_size)
    {
        x_dbg_stmt(("Buffer is not large enough.\n"));
        return -1;
    }
    file_buf[bytes_read] = '\0';
    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: _os_cli_get_mem
 *
 * Description: Search for requested mem info from a buffer.
 *
 * Inputs: 
 *
 * Outputs: - 
 *
 * Returns: used memory for match string.
 ----------------------------------------------------------------------------*/
static int _os_cli_get_mem(
    CHAR *buffer,
    CHAR *match_string, 
    CHAR *format_string)
{
    CHAR *ps_match_line;
    int mem_size;

    if (buffer == NULL || match_string == NULL)
    {
        return 0;
    }
    /* Find the starting address of match string.*/
    ps_match_line = strstr(buffer, match_string);
    if (ps_match_line == NULL)
    {
        return 0;
    }
    sscanf(ps_match_line, format_string, &mem_size);
    DBG_INFO(("%s is: %d.\n",match_string, mem_size));
    return mem_size;
}

/*-----------------------------------------------------------------------------
 * Name: _os_mem_cli_get_used_mem
 *
 * Description: Get current used memory from /proc/meminfo
 *
 * Inputs: 
 *
 * Outputs: - 
 *
 * Returns: used memory.
 ----------------------------------------------------------------------------*/
static int _os_mem_cli_get_used_mem(VOID)
{
    /* Observer will influence the object being observed. -- Bohr 
     * This memory monitor will add 1k memory usage.
     */
    CHAR file_buf[1024];
    int ret;

    int current_total, current_free, current_buffer, current_cached;
    int current_avaliable, current_used;  

    ret = _os_mem_cli_read_proc_meminfo(file_buf, sizeof(file_buf));
    if (ret != 0 || file_buf == NULL)
    {
        x_dbg_stmt(("Error: Can not read /proc/meminfo.\n"));
        return 0;
    }
    
    current_total = _os_cli_get_mem(file_buf, "MemTotal:","MemTotal: %d kB");
    current_free = _os_cli_get_mem(file_buf, "MemFree:", "MemFree: %d kB");
    current_buffer = _os_cli_get_mem(file_buf, "Buffers:", "Buffers: %d kB");
    current_cached = _os_cli_get_mem(file_buf, "Cached:", "Cached: %d kB");
    
    current_avaliable = current_free + current_buffer + current_cached;
    current_used = current_total - current_avaliable;

    return current_used;
}

static BOOL _os_mem_cli_compare(struct mem_info *max_mem, 
                                struct mem_info *current_mem)
{
    BOOL b_changed = FALSE;
    
    if (max_mem->used < current_mem->used)
    {
        max_mem->used = current_mem->used;
        x_dbg_stmt("{Memory} system memory peak changed\n");
        b_changed = TRUE;
    }

    if (max_mem->dfb_video < current_mem->dfb_video)
    {
        x_dbg_stmt("{Memory} dfb_video peak changed\n");
        max_mem->dfb_video = current_mem->dfb_video;
        b_changed = TRUE;
    }

    if (max_mem->dfb_system < current_mem->dfb_system)
    {
        x_dbg_stmt("{Memory} dfb_system peak changed\n");
        max_mem->dfb_system = current_mem->dfb_system;
        b_changed = TRUE;
    }

    if (max_mem->fbm < current_mem->fbm)
    {
        x_dbg_stmt("{Memory} fbm peak changed\n");
        max_mem->fbm = current_mem->fbm;
        b_changed = TRUE;
    }

    return b_changed;
}


static VOID os_mem_cli_query_entry(VOID *pv_data)
{
    UINT32  *pui4_query_interval;
    
    BOOL b_changed;
    
    pui4_query_interval = (UINT32*)(pv_data);
    if(*pui4_query_interval == 0)
    {
        *pui4_query_interval = 50;/*ms*/
    }

    while(!g_b_is_stop)
    {
        g_current_mem.used = _os_mem_cli_get_used_mem();
        DirectFBDump(&(g_current_mem.dfb_video),
                     &(g_current_mem.dfb_system),
                     &(g_current_mem.fbm)); 

        b_changed = _os_mem_cli_compare(&g_max_mem, &g_current_mem);

        if (b_changed)
        {
            _os_mem_cli_show_mem("{Memory}MAX",&g_max_mem);
        }

        x_thread_delay(*pui4_query_interval);
    }
    x_thread_exit();
}

/*-----------------------------------------------------------------------------
 * Name: os_cli_start_query_mem_info
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_mem_cli_start_query(INT32        i4_argc,
                       const CHAR **pps_argv)
{
    HANDLE_T h_hnd;

    if(i4_argc >= 2)
    {
        g_ui4_qry_intv = (UINT32)x_strtoll(pps_argv[1], NULL, 0);
    }
    else
    {
        g_ui4_qry_intv = 500; /*ms*/
    }       

    g_b_is_stop = FALSE;
    x_thread_create(&h_hnd,
                    "os_mem_monitor",
                    4096,
                    210,
                    (x_os_thread_main_fct)os_mem_cli_query_entry,
                    sizeof(UINT32),
                    (VOID*)(&g_ui4_qry_intv));

    return(CLIR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: os_cli_stop_query_mem_info
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_mem_cli_stop_query(INT32        i4_argc,
                      const CHAR **pps_argv)
{
    g_b_is_stop = TRUE;
    x_memset(&g_max_mem, 0, sizeof(g_current_mem));
    return(CLIR_OK);
}

static INT32
os_mem_cli_show_mem(INT32        i4_argc,
                    const CHAR **pps_argv)
{
    _os_mem_cli_show_mem("{Memory}Current",&g_current_mem);
    _os_mem_cli_show_mem("{Memory}MAX",&g_max_mem);  
    return 0;
}
#endif
/* End Memory monitor. */
#endif /* LINUX_TURNKEY_SOLUTION */

#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: os_cli_mem_alloc
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_mem_alloc(INT32        i4_argc,
                 const CHAR** pps_argv)
{
    HANDLE_T h_hdl1, h_hdl2, h_hdl3, h_hdl4, h_hdl5;
    HANDLE_T h_hdl6, h_hdl7, h_hdl8, h_hdl9, h_hdl10;
    HANDLE_T h_hdl11, h_hdl12, h_hdl13, h_hdl14, h_hdl15, h_hdl16;
    INT32 i4_ret, i4_i;


#if 0
    h_cli_ma_part = (SIZE_T) x_strtoull(pps_argv[1], NULL, 0);
    z_cli_ma_size = (SIZE_T) x_strtoull(pps_argv[2], NULL, 0);
    i4_cli_ma_loop = (INT32) x_strtoull(pps_argv[3], NULL, 0);
    if (h_cli_ma_part == (HANDLE_T) 0)
    {
        h_cli_ma_part = NULL_HANDLE;
    }
#endif

    h_cli_ma_part = NULL_HANDLE;
    z_cli_ma_size = (SIZE_T) x_strtoull(pps_argv[1], NULL, 0);
    i4_cli_ma_loop = (INT32) x_strtoull(pps_argv[2], NULL, 0);
    i4_cli_ma_delay = (UINT32) x_strtoull(pps_argv[3], NULL, 0);

    if (z_cli_ma_size == 0)
    {
        z_cli_ma_size = 739;
    }

    if (i4_cli_ma_loop == 0)
    {
        i4_cli_ma_loop = 100;
    }

    if (i4_cli_ma_delay == 0)
    {
        i4_cli_ma_delay = (UINT32) 3000;
    }

    i4_i = 1;
    i4_ret = x_thread_create(&h_hdl1,
                             "os_ma1",
                             2048,
                             200,
                             (x_os_thread_main_fct) os_cli_ma1,
                             sizeof(i4_i),
                             &i4_i);
    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 2;
    i4_ret = x_thread_create(&h_hdl2,
                             "os_ma2",
                             2048,
                             200,
                             (x_os_thread_main_fct) os_cli_ma2,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 3;
    i4_ret = x_thread_create(&h_hdl3,
                             "os_ma3",
                             2048,
                             201,
                             (x_os_thread_main_fct) os_cli_ma2,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 11;
    i4_ret = x_thread_create(&h_hdl11,
                             "os_ma11",
                             2048,
                             201,
                             (x_os_thread_main_fct) os_cli_ma4,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 12;
    i4_ret = x_thread_create(&h_hdl12,
                             "os_ma12",
                             2048,
                             201,
                             (x_os_thread_main_fct) os_cli_ma4,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 13;
    i4_ret = x_thread_create(&h_hdl13,
                             "os_ma13",
                             2048,
                             199,
                             (x_os_thread_main_fct) os_cli_ma5,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 14;
    i4_ret = x_thread_create(&h_hdl14,
                             "os_ma14",
                             2048,
                             199,
                             (x_os_thread_main_fct) os_cli_ma5,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 15;
    i4_ret = x_thread_create(&h_hdl15,
                             "os_ma15",
                             2048,
                             199,
                             (x_os_thread_main_fct) os_cli_ma6,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 16;
    i4_ret = x_thread_create(&h_hdl16,
                             "os_ma16",
                             2048,
                             199,
                             (x_os_thread_main_fct) os_cli_ma6,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 4;
    i4_ret = x_thread_create(&h_hdl4,
                             "os_ma4",
                             2048,
                             128,
                             (x_os_thread_main_fct) os_cli_ma2,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 5;
    i4_ret = x_thread_create(&h_hdl5,
                             "os_ma5",
                             2048,
                             128,
                             (x_os_thread_main_fct) os_cli_ma2,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 6;
    i4_ret = x_thread_create(&h_hdl6,
                             "os_ma6",
                             2048,
                             202,
                             (x_os_thread_main_fct) os_cli_ma3,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 7;
    i4_ret = x_thread_create(&h_hdl7,
                             "os_ma7",
                             2048,
                             200,
                             (x_os_thread_main_fct) os_cli_ma3,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 8;
    i4_ret = x_thread_create(&h_hdl8,
                             "os_ma8",
                             2048,
                             130,
                             (x_os_thread_main_fct) os_cli_ma3,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 9;
    i4_ret = x_thread_create(&h_hdl9,
                             "os_ma9",
                             2048,
                             128,
                             (x_os_thread_main_fct) os_cli_ma3,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_i = 10;
    i4_ret = x_thread_create(&h_hdl10,
                             "os_ma10",
                             2048,
                             129,
                             (x_os_thread_main_fct) os_cli_ma3,
                             sizeof(i4_i),
                             &i4_i);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }
    else
    {
        x_dbg_stmt("ma %d %d %d\n",
                    z_cli_ma_size, i4_cli_ma_loop, i4_cli_ma_delay);

        return(OSR_OK);
    }
}


static VOID
os_cli_ma1(VOID *pv_arg)
{
    VOID *pv_m1, *pv_m2, *pv_m3, *pv_m4, *pv_m5;
    VOID *pv_m6, *pv_m7, *pv_m8, *pv_m9, *pv_m10, *pv_big, *pv_mid;
    INT32 i4_loop, i4_i;
    SIZE_T z_z, z_big, z_mid;
    UINT32 ui4_alloc, ui4_free, ui4_index, i4_delay;


    ui4_index = *((UINT32*)pv_arg);
    i4_delay = (UINT32) (i4_cli_ma_delay + (os_get_fine_tick() & 0xf));
    i4_loop = i4_cli_ma_loop;
    i4_i = 0;
    ui4_free = ui4_alloc = 0;
    z_z = z_cli_ma_size << 1;

    do
    {
        z_big = (SIZE_T) (os_get_fine_tick() & 0x7fffff);
        while (z_big < (SIZE_T) (3 * (1 << 20)))
        {
            z_big = (z_big << 1);
        }
        while (z_big > (SIZE_T) (7 * (1 << 20)))
        {
            z_big = (z_big >> 1);
        }

        z_mid = (SIZE_T) (os_get_fine_tick() & 0x1fffff);
        while (z_mid < (SIZE_T) (8 * (1 << 16)))
        {
            z_mid = (z_mid << 1);
        }
        while (z_mid > (SIZE_T) (17 * (1 << 16)))
        {
            z_mid = (z_mid >> 1);
        }

        x_thread_delay(i4_delay >> 1);
        pv_m4 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 3));
        ++ui4_alloc;
        pv_m1 = x_mem_part_alloc(h_cli_ma_part, (z_z + i4_i));
        ++ui4_alloc;
        pv_m3 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 2));
        ++ui4_alloc;

        x_thread_delay(i4_delay >> 1);
        pv_m5 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 4));
        ++ui4_alloc;
        pv_m2 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 1));
        ++ui4_alloc;

        x_thread_delay(i4_delay >> 1);
        pv_m6 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 6));
        ++ui4_alloc;
        pv_m7 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 9));
        ++ui4_alloc;

        pv_mid = x_mem_part_alloc(h_cli_ma_part, (z_mid + i4_i));
        ++ui4_alloc;
        if (!pv_mid)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma1: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, (z_mid + i4_i));
        }

        x_thread_delay(i4_delay >> 1);
        pv_m8 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 7));
        ++ui4_alloc;
        pv_m9 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 5));
        ++ui4_alloc;
        pv_m10 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 8));
        ++ui4_alloc;

        if (!pv_m1 || !pv_m2 || !pv_m3 || !pv_m4 || !pv_m5 ||
            !pv_m6 || !pv_m7 || !pv_m8 || !pv_m9 || !pv_m10) 
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma1: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, z_z);
        }

        pv_big = x_mem_part_alloc(h_cli_ma_part, (z_big + i4_i));
        ++ui4_alloc;
        if (!pv_big)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma1: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, (z_big + i4_i));
        }

        {
            x_thread_delay(i4_delay >> 1);
            if (pv_m3)
            {
                x_mem_free(pv_m3);
                ++ui4_free;
            }
            if (pv_m1)
            {
                x_mem_free(pv_m1);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m2)
            {
                x_mem_free(pv_m2);
                ++ui4_free;
            }
            if (pv_m5)
            {
                x_mem_free(pv_m5);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m4)
            {
                x_mem_free(pv_m4);
                ++ui4_free;
            }
            if (pv_big)
            {
                x_mem_free(pv_big);
                ++ui4_free;
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m8)
            {
                x_mem_free(pv_m8);
                ++ui4_free;
            }
            if (pv_m6)
            {
                x_mem_free(pv_m6);
                ++ui4_free;
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m7)
            {
                x_mem_free(pv_m7);
                ++ui4_free;
            }

            if (pv_mid)
            {
                x_mem_free(pv_mid);
                ++ui4_free;
            }

            if (pv_m9)
            {
                x_mem_free(pv_m9);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m10)
            {
                x_mem_free(pv_m10);
                ++ui4_free;
            }
        }

        --i4_loop;
        if ((i4_loop & 0x00000fff) == 0x00000fff)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma1: %d %d %d, %d loops (alloc %d, free %d)\n",
                    ui4_index, z_cli_ma_size, i4_cli_ma_loop, i4_cli_ma_delay,
                    (i4_cli_ma_loop - i4_loop),
                    ui4_alloc, ui4_free);
        }
        if (++i4_i > 79)
        {
            i4_i = 0;
        }
    } while (i4_loop != 0);

    x_thread_exit();
}


static VOID
os_cli_ma2(VOID *pv_arg)
{
    VOID *pv_m1, *pv_m2, *pv_m3, *pv_m4, *pv_m5;
    VOID *pv_m6, *pv_m7, *pv_m8, *pv_m9, *pv_m10;
    VOID *pv_m11, *pv_m12, *pv_m13, *pv_m14, *pv_m15;
    VOID *pv_m16, *pv_m17, *pv_m18, *pv_m19, *pv_m20;
    SIZE_T z_m1, z_m2, z_m3, z_m4, z_m5;
    SIZE_T z_m6, z_m7, z_m8, z_m9, z_m10;
    INT32 i4_loop, i4_i;
    UINT32 ui4_alloc, ui4_free, ui4_index, i4_delay;


    ui4_index = *((UINT32*)pv_arg);
    i4_loop = i4_cli_ma_loop;
    i4_delay = (UINT32) (i4_cli_ma_delay + (os_get_fine_tick() & 0x7));
    i4_delay = i4_delay >> 1;
    i4_i = 0;
    ui4_free = ui4_alloc = 0;
    x_thread_delay(i4_delay);
    do
    {
        z_m1 = (z_cli_ma_size + i4_i) >> 12;
        z_m4 = (z_cli_ma_size + i4_i) >> 8;
        z_m3 = (z_cli_ma_size + i4_i) >> 4;
        z_m2 = (z_cli_ma_size + i4_i) >> 2;
        z_m5 = (z_cli_ma_size + i4_i) >> 1;

        z_m10 = (z_cli_ma_size + i4_i) >> 12;
        if (z_m10 < 20)
        {
            z_m10 = 23;
        }
        z_m9 = (z_cli_ma_size + i4_i) >> 11;
        if (z_m9 < 20)
        {
            z_m9 = 29;
        }
        z_m8 = (z_cli_ma_size + i4_i) >> 10;
        if (z_m8 < 20)
        {
            z_m8 = 19;
        }
        z_m6 = (z_cli_ma_size + i4_i) >> 12;
        if (z_m6 < 20)
        {
            z_m6 = 25;
        }
        z_m7 = (z_cli_ma_size + i4_i) >> 9;
        if (z_m7 < 20)
        {
            z_m7 = 31;
        }

        pv_m1 = x_mem_part_alloc(h_cli_ma_part, z_m1);
        ++ui4_alloc;
        x_thread_delay(i4_delay);

        pv_m4 = x_mem_part_alloc(h_cli_ma_part, z_m4);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m3 = x_mem_part_alloc(h_cli_ma_part, z_m3);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m2 = x_mem_part_alloc(h_cli_ma_part, z_m2);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m5 = x_mem_part_alloc(h_cli_ma_part, z_m5);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m8 = x_mem_part_alloc(h_cli_ma_part, z_m8);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m7 = x_mem_part_alloc(h_cli_ma_part, z_m7);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m9 = x_mem_part_alloc(h_cli_ma_part, z_m9);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m10 = x_mem_part_alloc(h_cli_ma_part, z_m10);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m6 = x_mem_part_alloc(h_cli_ma_part, z_m6);
        ++ui4_alloc;
        x_thread_delay(i4_delay);

        if (!pv_m1 || !pv_m2|| !pv_m3 || !pv_m4 || !pv_m5 ||
            !pv_m6 || !pv_m7|| !pv_m8 || !pv_m9 || !pv_m10)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma2: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, z_cli_ma_size);
        }

        pv_m11 = x_mem_part_alloc(h_cli_ma_part, z_m1);
        ++ui4_alloc;
        x_thread_delay(i4_delay);

        pv_m14 = x_mem_part_alloc(h_cli_ma_part, z_m4);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m13 = x_mem_part_alloc(h_cli_ma_part, z_m3);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m12 = x_mem_part_alloc(h_cli_ma_part, z_m2);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m15 = x_mem_part_alloc(h_cli_ma_part, z_m5);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m18 = x_mem_part_alloc(h_cli_ma_part, z_m8);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m17 = x_mem_part_alloc(h_cli_ma_part, z_m7);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m19 = x_mem_part_alloc(h_cli_ma_part, z_m9);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m20 = x_mem_part_alloc(h_cli_ma_part, z_m10);
        ++ui4_alloc;
        /*x_thread_delay(i4_delay);*/

        pv_m16 = x_mem_part_alloc(h_cli_ma_part, z_m6);
        ++ui4_alloc;
        x_thread_delay(i4_delay);

        if (!pv_m11 || !pv_m12|| !pv_m13 || !pv_m14 || !pv_m15 ||
            !pv_m16 || !pv_m17|| !pv_m18 || !pv_m19 || !pv_m20)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma2: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, z_cli_ma_size);
        }

        if (pv_m3 != NULL)
        {
            x_mem_free(pv_m3);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m8 != NULL)
        {
            x_mem_free(pv_m8);
            ++ui4_free;
        }
        if (pv_m1 != NULL)
        {
            x_mem_free(pv_m1);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m6 != NULL)
        {
            x_mem_free(pv_m6);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m5 != NULL)
        {
            x_mem_free(pv_m5);
            ++ui4_free;
        }
        if (pv_m10 != NULL)
        {
            x_mem_free(pv_m10);
            ++ui4_free;
            x_thread_delay(i4_delay);
        }
        if (pv_m2 != NULL)
        {
            x_mem_free(pv_m2);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m7 != NULL)
        {
            x_mem_free(pv_m7);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m4 != NULL)
        {
            x_mem_free(pv_m4);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m9 != NULL)
        {
            x_mem_free(pv_m9);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }

        x_thread_delay(i4_delay);

        if (pv_m13 != NULL)
        {
            x_mem_free(pv_m13);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m18 != NULL)
        {
            x_mem_free(pv_m18);
            ++ui4_free;
        }
        if (pv_m11 != NULL)
        {
            x_mem_free(pv_m11);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m16 != NULL)
        {
            x_mem_free(pv_m16);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m15 != NULL)
        {
            x_mem_free(pv_m15);
            ++ui4_free;
        }
        if (pv_m20 != NULL)
        {
            x_mem_free(pv_m20);
            ++ui4_free;
            x_thread_delay(i4_delay);
        }
        if (pv_m12 != NULL)
        {
            x_mem_free(pv_m12);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m17 != NULL)
        {
            x_mem_free(pv_m17);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m14 != NULL)
        {
            x_mem_free(pv_m14);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }
        if (pv_m19 != NULL)
        {
            x_mem_free(pv_m19);
            ++ui4_free;
            /*x_thread_delay(i4_delay);*/
        }

        --i4_loop;
        if ((i4_loop & 0x00000fff) == 0x00000fff)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma2 %d %d %d, %d loops (alloc %d, free %d)\n",
                    ui4_index, z_cli_ma_size, i4_cli_ma_loop, i4_cli_ma_delay,
                    (i4_cli_ma_loop - i4_loop),
                    ui4_alloc, ui4_free);
        }
        if (++i4_i > 2048)
        {
            i4_i = 0;
        }
    } while (i4_loop != 0);

    x_thread_exit();
}


static VOID
os_cli_ma3(VOID *pv_arg)
{
    VOID *pv_m1, *pv_m2, *pv_m3, *pv_m4, *pv_m5;
    VOID *pv_m6, *pv_m7, *pv_m8, *pv_m9, *pv_m10;
    INT32 i4_loop, i4_i;
    SIZE_T z_z;
    UINT32 ui4_alloc, ui4_free, ui4_index, i4_j, i4_delay;

    ui4_index = *((UINT32*)pv_arg);
    i4_delay = (UINT32) (i4_cli_ma_delay + (os_get_fine_tick() & 0xf));
    i4_loop = i4_cli_ma_loop;
    i4_i = 0;
    ui4_free = ui4_alloc = 0;
    z_z = z_cli_ma_size >> 1;
    do
    {
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m4 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 3));
        ++ui4_alloc;
        pv_m1 = x_mem_part_alloc(h_cli_ma_part, (z_z + i4_i));
        ++ui4_alloc;
        pv_m3 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 2));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m5 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 4));
        ++ui4_alloc;
        pv_m2 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 1));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m6 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 6));
        ++ui4_alloc;
        pv_m7 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 9));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m8 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 7));
        ++ui4_alloc;
        pv_m9 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 5));
        ++ui4_alloc;
        pv_m10 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 8));
        ++ui4_alloc;
        if (!pv_m1 || !pv_m2 || !pv_m3 || !pv_m4 || !pv_m5 ||
            !pv_m6 || !pv_m7 || !pv_m8 || !pv_m9 || !pv_m10) 
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma3: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, z_z);
        }

        {
            x_thread_delay(i4_delay >> 1);
            if (pv_m3)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j) = 0xccddeeff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m1)
            {
                for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j) = 0xddeeffcc;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m2)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j) = 0xeeffccdd;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m5)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j) = 0xffccddee;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m4)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j) = 0xcdefcdef;
                    i4_j += 4;
                }
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m8)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j) = 0xdefcdefc;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m6)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j) = 0xefcdefcd;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m7)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j) = 0xfcdefcde;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m9)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j) = 0xfcfceded;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m10)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j) = 0xededfcfc;
                    i4_j += 4;
                }
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m3)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m3) + i4_j) != (UINT32) 0xccddeeff)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m3 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m3) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j),
                                    0xccddeeff);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m3);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m1)
            {
                for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m1) + i4_j) != (UINT32) 0xddeeffcc)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m1 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m1) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j),
                                    0xddeeffcc);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m1);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m2)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m2) + i4_j) != (UINT32) 0xeeffccdd)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m2 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m2) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j),
                                    0xeeffccdd);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m2);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m5)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m5) + i4_j) != (UINT32) 0xffccddee)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m5 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m5) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j),
                                    0xffccddee);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m5);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m4)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m4) + i4_j) != (UINT32) 0xcdefcdef)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m4 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m4) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j),
                                    0xcdefcdef);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m4);
                ++ui4_free;
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m8)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m8) + i4_j) != (UINT32) 0xdefcdefc)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m8 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m8) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j),
                                    0xdefcdefc);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m8);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m6)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m6) + i4_j) != (UINT32) 0xefcdefcd)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m6 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m6) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j),
                                    0xefcdefcd);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m6);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m7)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m7) + i4_j) != (UINT32) 0xfcdefcde)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m7 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m7) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j),
                                    0xfcdefcde);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m7);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m9)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m9) + i4_j) != (UINT32) 0xfcfceded)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m9 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m9) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j),
                                    0xfcfceded);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m9);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m10)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m10) + i4_j) != (UINT32) 0xededfcfc)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma3: loop %d, @m10 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m10) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j),
                                    0xededfcfc);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m10);
                ++ui4_free;
            }
        }

        /*x_thread_delay(i4_delay >> 1);*/

        --i4_loop;
        if ((i4_loop & 0x00000fff) == 0x00000fff)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma3 %d %d %d, %d loops (alloc %d, free %d)\n",
                    ui4_index, z_z, i4_cli_ma_loop, i4_cli_ma_delay,
                    (i4_cli_ma_loop - i4_loop),
                    ui4_alloc, ui4_free);
        }
        if (++i4_i > 79)
        {
            i4_i = 0;
        }
    } while (i4_loop != 0);

    x_thread_exit();
}


static VOID
os_cli_ma4(VOID *pv_arg)
{
    VOID *pv_m1, *pv_m2, *pv_m3, *pv_m4, *pv_m5;
    VOID *pv_m6, *pv_m7, *pv_m8, *pv_m9, *pv_m10;
    INT32 i4_loop, i4_i, i4_n;
    SIZE_T z_z;
    UINT32 ui4_alloc, ui4_free, ui4_index, i4_delay, i4_j;
    UINT32 ui4_err1, ui4_err2, ui4_err3, ui4_err4, ui4_err5;
    UINT32 ui4_err6, ui4_err7, ui4_err8, ui4_err9, ui4_err10;

    ui4_index = *((UINT32*)pv_arg);
    i4_delay = (UINT32) (i4_cli_ma_delay + (os_get_fine_tick() & 0xf));
    i4_loop = i4_cli_ma_loop;
    i4_i = 0;
    ui4_free = ui4_alloc = 0;
    z_z = z_cli_ma_size >> 1;
    do
    {
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m4 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 3));
        ++ui4_alloc;
        pv_m1 = x_mem_part_alloc(h_cli_ma_part, (z_z + i4_i));
        ++ui4_alloc;
        pv_m3 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 2));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m5 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 4));
        ++ui4_alloc;
        pv_m2 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 1));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m6 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 6));
        ++ui4_alloc;
        pv_m7 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 9));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m8 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 7));
        ++ui4_alloc;
        pv_m9 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 5));
        ++ui4_alloc;
        pv_m10 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 8));
        ++ui4_alloc;
        if (!pv_m1 || !pv_m2 || !pv_m3 || !pv_m4 || !pv_m5 ||
            !pv_m6 || !pv_m7 || !pv_m8 || !pv_m9 || !pv_m10) 
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma4: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, z_z);
        }

        {
            x_thread_delay(i4_delay >> 1);
            if (pv_m3)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j) = 0xccddeeff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m1)
            {
                for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j) = 0xddeeffcc;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m2)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j) = 0xeeffccdd;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m5)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j) = 0xffccddee;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m4)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j) = 0xcdefcdef;
                    i4_j += 4;
                }
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m8)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j) = 0xdefcdefc;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m6)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j) = 0xefcdefcd;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m7)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j) = 0xfcdefcde;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m9)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j) = 0xfcfceded;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m10)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j) = 0xededfcfc;
                    i4_j += 4;
                }
            }

            i4_n = 1;
            ui4_err1 = ui4_err2 = ui4_err3 = ui4_err4 = ui4_err5 = 0;
            ui4_err6 = ui4_err7 = ui4_err8 = ui4_err9 = ui4_err10 = 0;
            while (i4_n < 100000)
            { 
                x_thread_delay(i4_delay >> 1);
                if (pv_m3)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m3) + i4_j) != (UINT32) 0xccddeeff)
                        {
                            if (++ui4_err3 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m3 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m3) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j),
                                    0xccddeeff);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m3) + i4_j) = 0xccddeeff;
                                ui4_err3 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m1)
                {
                    for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m1) + i4_j) != (UINT32) 0xddeeffcc)
                        {
                            if (++ui4_err1 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m1 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m1) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j),
                                    0xddeeffcc);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m1) + i4_j) = 0xddeeffcc;
                                ui4_err1 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m2)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m2) + i4_j) != (UINT32) 0xeeffccdd)
                        {
                            if (++ui4_err2 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m2 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m2) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j),
                                    0xeeffccdd);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m2) + i4_j) = 0xeeffccdd;
                                ui4_err2 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m5)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m5) + i4_j) != (UINT32) 0xffccddee)
                        {
                            if (++ui4_err5 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m5 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m5) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j),
                                    0xffccddee);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m5) + i4_j) = 0xffccddee;
                                ui4_err5 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m4)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m4) + i4_j) != (UINT32) 0xcdefcdef)
                        {
                            if (++ui4_err4 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m4 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m4) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j),
                                    0xcdefcdef);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m4) + i4_j) = 0xcdefcdef;
                                ui4_err4 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }

                x_thread_delay(i4_delay >> 1);
                if (pv_m8)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m8) + i4_j) != (UINT32) 0xdefcdefc)
                        {
                            if (++ui4_err8 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m8 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m8) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j),
                                    0xdefcdefc);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m8) + i4_j) = 0xdefcdefc;
                                ui4_err8 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m6)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m6) + i4_j) != (UINT32) 0xefcdefcd)
                        {
                            if (++ui4_err6 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m6 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m6) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j),
                                    0xefcdefcd);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m6) + i4_j) = 0xefcdefcd;
                                ui4_err6 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m7)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m7) + i4_j) != (UINT32) 0xfcdefcde)
                        {
                            if (++ui4_err7 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m7 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m7) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j),
                                    0xfcdefcde);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m7) + i4_j) = 0xfcdefcde;
                                ui4_err7 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m9)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m9) + i4_j) != (UINT32) 0xfcfceded)
                        {
                            if (++ui4_err9 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m9 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m9) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j),
                                    0xfcfceded);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m9) + i4_j) = 0xfcfceded;
                                ui4_err9 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m10)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m10) + i4_j) != (UINT32) 0xededfcfc)
                        {
                            if (++ui4_err10 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma4: %d read, loop %d, @m10 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m10) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j),
                                    0xededfcfc);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m10) + i4_j) = 0xededfcfc;
                                ui4_err10 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                ++i4_n;
            }

            if (pv_m3)
            {
                x_mem_free(pv_m3);
                ++ui4_free;
            }
            if (pv_m1)
            {
                x_mem_free(pv_m1);
                ++ui4_free;
            }
            if (pv_m2)
            {
                x_mem_free(pv_m2);
                ++ui4_free;
            }
            if (pv_m5)
            {
                x_mem_free(pv_m5);
                ++ui4_free;
            }
            if (pv_m4)
            {
                x_mem_free(pv_m4);
                ++ui4_free;
            }
            if (pv_m6)
            {
                x_mem_free(pv_m6);
                ++ui4_free;
            }
            if (pv_m8)
            {
                x_mem_free(pv_m8);
                ++ui4_free;
            }
            if (pv_m7)
            {
                x_mem_free(pv_m7);
                ++ui4_free;
            }
            if (pv_m9)
            {
                x_mem_free(pv_m9);
                ++ui4_free;
            }
            if (pv_m10)
            {
                x_mem_free(pv_m10);
                ++ui4_free;
            }
        }

        --i4_loop;
/*
        if ((i4_loop & 0x00000fff) == 0x00000fff)
*/
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma4 %d %d %d, %d loops (alloc %d, free %d)\n",
                    ui4_index, z_z, i4_cli_ma_loop, i4_cli_ma_delay,
                    (i4_cli_ma_loop - i4_loop),
                    ui4_alloc, ui4_free);
        }
        if (++i4_i > 79)
        {
            i4_i = 0;
        }
    } while (i4_loop != 0);

    x_thread_exit();
}


static VOID
os_cli_ma5(VOID *pv_arg)
{
    VOID *pv_m1, *pv_m2, *pv_m3, *pv_m4, *pv_m5;
    VOID *pv_m6, *pv_m7, *pv_m8, *pv_m9, *pv_m10;
    INT32 i4_loop, i4_i;
    SIZE_T z_z;
    UINT32 ui4_alloc, ui4_free, ui4_index, i4_delay, i4_j;

    ui4_index = *((UINT32*)pv_arg);
    i4_delay = (UINT32) (i4_cli_ma_delay + (os_get_fine_tick() & 0xf));
    i4_loop = i4_cli_ma_loop;
    i4_i = 0;
    ui4_free = ui4_alloc = 0;
    z_z = z_cli_ma_size >> 1;
    do
    {
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m4 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 3));
        ++ui4_alloc;
        pv_m1 = x_mem_part_alloc(h_cli_ma_part, (z_z + i4_i));
        ++ui4_alloc;
        pv_m3 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 2));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m5 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 4));
        ++ui4_alloc;
        pv_m2 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 1));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m6 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 6));
        ++ui4_alloc;
        pv_m7 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 9));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m8 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 7));
        ++ui4_alloc;
        pv_m9 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 5));
        ++ui4_alloc;
        pv_m10 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 8));
        ++ui4_alloc;
        if (!pv_m1 || !pv_m2 || !pv_m3 || !pv_m4 || !pv_m5 ||
            !pv_m6 || !pv_m7 || !pv_m8 || !pv_m9 || !pv_m10) 
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma5: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, z_z);
        }

        {
            x_thread_delay(i4_delay >> 1);
            if (pv_m3)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m1)
            {
                for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m2)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m5)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m4)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m8)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m6)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m7)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m9)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m10)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m3)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m3) + i4_j) != (UINT32) 0xffffffff)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m3 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m3) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j),
                                    0xffffffff);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m3);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m1)
            {
                for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m1) + i4_j) != (UINT32) 0xffffffff)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m1 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m1) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j),
                                    0xffffffff);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m1);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m2)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m2) + i4_j) != (UINT32) 0x00000000)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m2 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m2) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j),
                                    0x00000000);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m2);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m5)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m5) + i4_j) != (UINT32) 0xffffffff)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m5 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m5) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j),
                                    0xffffffff);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m5);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m4)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m4) + i4_j) != (UINT32) 0x00000000)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m4 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m4) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j),
                                    0x00000000);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m4);
                ++ui4_free;
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m8)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m8) + i4_j) != (UINT32) 0x00000000)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m8 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m8) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j),
                                    0x00000000);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m8);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m6)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m6) + i4_j) != (UINT32) 0x00000000)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m6 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m6) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j),
                                    0x00000000);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m6);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m7)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m7) + i4_j) != (UINT32) 0xffffffff)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m7 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m7) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j),
                                    0xffffffff);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m7);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m9)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m9) + i4_j) != (UINT32) 0xffffffff)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m9 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m9) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j),
                                    0xffffffff);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m9);
                ++ui4_free;
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m10)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                {
                    if (*(UINT32 *) (((UINT8 *)pv_m10) + i4_j) != (UINT32) 0x00000000)
                    {
                        x_dbg_stmt("{MEM_ERR} mtest%d: ma5: loop %d, @m10 %x: %x, %x\n",
                                    ui4_index, (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m10) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j),
                                    0x00000000);
                    }
                    i4_j += 4;
                }
                x_mem_free(pv_m10);
                ++ui4_free;
            }
        }

        /*x_thread_delay(i4_delay >> 1);*/

        --i4_loop;
        if ((i4_loop & 0x00000fff) == 0x00000fff)
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma5 %d %d %d, %d loops (alloc %d, free %d)\n",
                    ui4_index, z_z, i4_cli_ma_loop, i4_cli_ma_delay,
                    (i4_cli_ma_loop - i4_loop),
                    ui4_alloc, ui4_free);
        }
        if (++i4_i > 79)
        {
            i4_i = 0;
        }
    } while (i4_loop != 0);

    x_thread_exit();
}


static VOID
os_cli_ma6(VOID *pv_arg)
{
    VOID *pv_m1, *pv_m2, *pv_m3, *pv_m4, *pv_m5;
    VOID *pv_m6, *pv_m7, *pv_m8, *pv_m9, *pv_m10;
    INT32 i4_loop, i4_i, i4_n;
    SIZE_T z_z;
    UINT32 ui4_alloc, ui4_free, ui4_index, i4_delay, i4_j;
    UINT32 ui4_err1, ui4_err2, ui4_err3, ui4_err4, ui4_err5;
    UINT32 ui4_err6, ui4_err7, ui4_err8, ui4_err9, ui4_err10;

    ui4_index = *((UINT32*)pv_arg);
    i4_delay = (UINT32) (i4_cli_ma_delay + (os_get_fine_tick() & 0xf));
    i4_loop = i4_cli_ma_loop;
    i4_i = 0;
    ui4_free = ui4_alloc = 0;
    z_z = z_cli_ma_size >> 1;
    do
    {
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m4 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 3));
        ++ui4_alloc;
        pv_m1 = x_mem_part_alloc(h_cli_ma_part, (z_z + i4_i));
        ++ui4_alloc;
        pv_m3 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 2));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m5 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 4));
        ++ui4_alloc;
        pv_m2 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 1));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m6 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 6));
        ++ui4_alloc;
        pv_m7 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 9));
        ++ui4_alloc;
        x_thread_delay(i4_cli_ma_delay >> 1);
        pv_m8 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 7));
        ++ui4_alloc;
        pv_m9 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 5));
        ++ui4_alloc;
        pv_m10 = x_mem_part_alloc(h_cli_ma_part, ((z_z + i4_i) >> 8));
        ++ui4_alloc;
        if (!pv_m1 || !pv_m2 || !pv_m3 || !pv_m4 || !pv_m5 ||
            !pv_m6 || !pv_m7 || !pv_m8 || !pv_m9 || !pv_m10) 
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma6: x_mem_part_alloc(%d, %d) failed\n",
                        ui4_index, h_cli_ma_part, z_z);
        }

        {
            x_thread_delay(i4_delay >> 1);
            if (pv_m3)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m1)
            {
                for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m2)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m5)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m4)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }

            x_thread_delay(i4_delay >> 1);
            if (pv_m8)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m6)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m7)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m9)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j) = 0xffffffff;
                    i4_j += 4;
                }
            }
            x_thread_delay(i4_delay >> 1);
            if (pv_m10)
            {
                for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                {
                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j) = 0x00000000;
                    i4_j += 4;
                }
            }

            i4_n = 1;
            ui4_err1 = ui4_err2 = ui4_err3 = ui4_err4 = ui4_err5 = 0;
            ui4_err6 = ui4_err7 = ui4_err8 = ui4_err9 = ui4_err10 = 0;
            while (i4_n < 100000)
            { 
                x_thread_delay(i4_delay >> 1);
                if (pv_m3)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 2); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m3) + i4_j) != (UINT32) 0xffffffff)
                        {
                            if (++ui4_err3 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m3 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m3) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m3) + i4_j),
                                    0xffffffff);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m3) + i4_j) = 0xffffffff;
                                ui4_err3 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m1)
                {
                    for (i4_j = 0; i4_j < (UINT32) (z_z + i4_i); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m1) + i4_j) != (UINT32) 0xffffffff)
                        {
                            if (++ui4_err1 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m1 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m1) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m1) + i4_j),
                                    0xffffffff);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m1) + i4_j) = 0xffffffff;
                                ui4_err1 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m2)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 1); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m2) + i4_j) != (UINT32) 0x00000000)
                        {
                            if (++ui4_err2 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m2 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m2) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m2) + i4_j),
                                    0x00000000);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m2) + i4_j) = 0x00000000;
                                ui4_err2 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m5)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 4); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m5) + i4_j) != (UINT32) 0xffffffff)
                        {
                            if (++ui4_err5 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m5 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m5) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m5) + i4_j),
                                    0xffffffff);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m5) + i4_j) = 0xffffffff;
                                ui4_err5 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m4)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 3); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m4) + i4_j) != (UINT32) 0x00000000)
                        {
                            if (++ui4_err4 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m4 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m4) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m4) + i4_j),
                                    0x00000000);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m4) + i4_j) = 0x00000000;
                                ui4_err4 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }

                x_thread_delay(i4_delay >> 1);
                if (pv_m8)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 7); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m8) + i4_j) != (UINT32) 0x00000000)
                        {
                            if (++ui4_err8 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m8 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m8) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m8) + i4_j),
                                    0x00000000);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m8) + i4_j) = 0x00000000;
                                ui4_err8 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m6)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 6); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m6) + i4_j) != (UINT32) 0x00000000)
                        {
                            if (++ui4_err6 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m6 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m6) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m6) + i4_j),
                                    0x00000000);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m6) + i4_j) = 0x00000000;
                                ui4_err6 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m7)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 9); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m7) + i4_j) != (UINT32) 0xffffffff)
                        {
                            if (++ui4_err7 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m7 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m7) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m7) + i4_j),
                                    0xffffffff);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m7) + i4_j) = 0xffffffff;
                                ui4_err7 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m9)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 5); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m9) + i4_j) != (UINT32) 0xffffffff)
                        {
                            if (++ui4_err9 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m9 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m9) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m9) + i4_j),
                                    0xffffffff);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m9) + i4_j) = 0xffffffff;
                                ui4_err7 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                x_thread_delay(i4_delay >> 1);
                if (pv_m10)
                {
                    for (i4_j = 0; i4_j < (UINT32) ((z_z + i4_i) >> 8); )
                    {
                        if (*(UINT32 *) (((UINT8 *)pv_m10) + i4_j) != (UINT32) 0x00000000)
                        {
                            if (++ui4_err10 < 16)
                            {
                            x_dbg_stmt("{MEM_ERR} mtest%d: ma6: %d read, loop %d, @m10 %x: %x, %x\n",
                                    ui4_index, i4_n,
                                    (i4_cli_ma_loop - i4_loop + 1), 
                                    (((UINT8 *)pv_m10) + i4_j),
                                    *(UINT32 *) (((UINT8 *)pv_m10) + i4_j),
                                    0x00000000);
                            }
                            else
                            {
                                *(UINT32 *) (((UINT8 *)pv_m10) + i4_j) = 0x00000000;
                                ui4_err10 = 0;
                            }
                        }
                        i4_j += 4;
                    }
                }
                ++i4_n;
            }

            if (pv_m3)
            {
                x_mem_free(pv_m3);
                ++ui4_free;
            }
            if (pv_m1)
            {
                x_mem_free(pv_m1);
                ++ui4_free;
            }
            if (pv_m2)
            {
                x_mem_free(pv_m2);
                ++ui4_free;
            }
            if (pv_m5)
            {
                x_mem_free(pv_m5);
                ++ui4_free;
            }
            if (pv_m4)
            {
                x_mem_free(pv_m4);
                ++ui4_free;
            }
            if (pv_m6)
            {
                x_mem_free(pv_m6);
                ++ui4_free;
            }
            if (pv_m8)
            {
                x_mem_free(pv_m8);
                ++ui4_free;
            }
            if (pv_m7)
            {
                x_mem_free(pv_m7);
                ++ui4_free;
            }
            if (pv_m9)
            {
                x_mem_free(pv_m9);
                ++ui4_free;
            }
            if (pv_m10)
            {
                x_mem_free(pv_m10);
                ++ui4_free;
            }
        }

        --i4_loop;
/*
        if ((i4_loop & 0x00000fff) == 0x00000fff)
*/
        {
            x_dbg_stmt("{MEM_CLI} mtest%d: ma4 %d %d %d, %d loops (alloc %d, free %d)\n",
                    ui4_index, z_z, i4_cli_ma_loop, i4_cli_ma_delay,
                    (i4_cli_ma_loop - i4_loop),
                    ui4_alloc, ui4_free);
        }
        if (++i4_i > 79)
        {
            i4_i = 0;
        }
    } while (i4_loop != 0);

    x_thread_exit();
}


/*-----------------------------------------------------------------------------
 * Name: os_cli_mem_realloc
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_mem_realloc(INT32        i4_argc,
                   const CHAR** pps_argv)
{
    HANDLE_T h_hdl1;
    INT32 i4_ret;


#if 0
    h_cli_ma_part = (SIZE_T) x_strtoull(pps_argv[1], NULL, 0);
    z_cli_ma_size = (SIZE_T) x_strtoull(pps_argv[2], NULL, 0);
    i4_cli_ma_loop = (INT32) x_strtoull(pps_argv[3], NULL, 0);
    if (h_cli_ma_part == (HANDLE_T) 0)
    {
        h_cli_ma_part = NULL_HANDLE;
    }
#endif

    h_cli_ma_part = NULL_HANDLE;
    z_cli_ma_size = (SIZE_T) x_strtoull(pps_argv[1], NULL, 0);
    z_cli_ma_incr = (SIZE_T) x_strtoull(pps_argv[2], NULL, 0);
    i4_cli_ma_loop = (INT32) x_strtoull(pps_argv[3], NULL, 0);
    i4_cli_ma_delay = (INT32) x_strtoull(pps_argv[4], NULL, 0);

    if (z_cli_ma_size == 0)
    {
        z_cli_ma_size = 739;
    }

    if (i4_cli_ma_loop == 0)
    {
        i4_cli_ma_loop = 100;
    }

    if (i4_cli_ma_delay == 0)
    {
        i4_cli_ma_delay = 3000;
    }

    if (z_cli_ma_incr > (SIZE_T) (1 << 12))
    {
        z_cli_ma_incr = (SIZE_T) (1 << 12);
    }

    i4_ret = x_thread_create(&h_hdl1,
                             "os_cli_mra1",
                             2048,
                             200,
                             (x_os_thread_main_fct) os_cli_mra1,
                             (SIZE_T) 0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }
    else
    {
        x_dbg_stmt("mra %d %d %d %d\n",
                    z_cli_ma_size, z_cli_ma_incr,
                    i4_cli_ma_loop, i4_cli_ma_delay);

        return(OSR_OK);
    }
}


static VOID
os_cli_mra1(VOID)
{
    VOID *pv_m1, *pv_m2;
    SIZE_T z_m1;
    INT32 i4_loop;
    UINT32 ui4_t1, ui4_t2;


    x_thread_delay(i4_cli_ma_delay);
    i4_loop = i4_cli_ma_loop;
    z_m1 = z_cli_ma_size;
    pv_m1 = x_mem_part_alloc(h_cli_ma_part, z_m1);
    do
    {
        z_m1 += z_cli_ma_incr;
        ui4_t1 = os_get_fine_tick();
        pv_m2 = x_mem_part_realloc(h_cli_ma_part, pv_m1, z_m1);
        ui4_t2 = os_get_fine_tick();

        x_dbg_stmt("x_mem_realloc(%d): elapsed time %d\n",
                    z_cli_ma_incr, (ui4_t2 - ui4_t1));

        pv_m1 = pv_m2;
        x_thread_delay(i4_cli_ma_delay);

        --i4_loop;
    } while (i4_loop != 0);

    x_mem_free(pv_m2);
    x_thread_exit();
}


/*-----------------------------------------------------------------------------
 * Name: os_cli_msgq_perf
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32
os_cli_msgq_perf(INT32        i4_argc,
                 const CHAR** pps_argv)
{
    HANDLE_T h_secm_msgr, h_secm_msgs;
    INT32 i4_ret;


    z_cli_qp_size = (SIZE_T) x_strtoull(pps_argv[1], NULL, 0);
    i4_cli_qp_loop = (INT32) x_strtoull(pps_argv[2], NULL, 0);
    if (z_cli_qp_size > 512)
    {
        z_cli_qp_size = 512;
    }

    if (i4_cli_qp_loop == 0)
    {
        i4_cli_qp_loop = 100;
    }

    i4_ret = x_thread_create(&h_secm_msgr,
                             "os_msgr",
                             2048,
                             140,
                             (x_os_thread_main_fct) os_cli_msgq_rcv,
                             (SIZE_T) 0,
                             NULL);
    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }

    i4_ret = x_thread_create(&h_secm_msgs,
                             "os_msgs",
                             2048,
                             140,
                             (x_os_thread_main_fct) os_cli_msgq_send,
                             (SIZE_T) 0,
                             NULL);

    if (i4_ret != OSR_OK)
    {
        if (i4_ret == OSR_EXIST)
        {
            x_dbg_stmt("A testing is in progress\n");
            return(OSR_OK);
        }
        else
        {
            return(i4_ret);
        }
    }
    else
    {
        x_dbg_stmt("qp %d %d\n", z_cli_qp_size, i4_cli_qp_loop);
        return(OSR_OK);
    }
}


static VOID
os_cli_msgq_rcv()
{
    UINT8 *pui1_msg;
    HANDLE_T h_msgq[1];
    INT32 i4_ret;
    INT32 i4_loop;
    SIZE_T z_size;
    UINT16 ui2_idx;


    pui1_msg = (UINT8 *) x_mem_alloc(z_cli_qp_size);
    if (pui1_msg == NULL)
    {
        x_dbg_stmt("x_mem_alloc(%d) failed\n", z_cli_qp_size);
        return;
    }

    i4_ret = x_msg_q_create(&(h_msgq[0]),
                            OS_MSGQ_QP_NAME,
                            z_cli_qp_size,
                            5);

    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("x_msg_q_create(%s) failed %d\n",
                    "_os_cli_msgq", i4_ret);
    }

    i4_loop = i4_cli_qp_loop;
    do
    {
        z_size = z_cli_qp_size;
        i4_ret = x_msg_q_receive(&ui2_idx,
                                  pui1_msg,
                                  &z_size,
                                  h_msgq,
                                  1,
                                  X_MSGQ_OPTION_WAIT);

        if (i4_ret != OSR_OK)
        {
            x_dbg_stmt("x_msg_q_reecive(%s) failed %d\n",
                        "_os_cli_msgq", i4_ret);
        }

        x_thread_delay(500);

        --i4_loop;
    } while (i4_loop != 0);

    x_mem_free(pui1_msg);

    x_msg_q_delete(h_msgq[0]);

    x_dbg_stmt("qp %d %d DONE\n", z_cli_qp_size, i4_cli_qp_loop);

    x_thread_exit();
}


static VOID
os_cli_msgq_send(VOID)
{
    UINT8 *pui1_msg;
    HANDLE_T h_msgq;
    INT32 i4_ret;
    INT32 i4_loop;


    pui1_msg = (UINT8 *) x_mem_alloc(z_cli_qp_size);
    if (pui1_msg == NULL)
    {
        x_dbg_stmt("x_mem_alloc(%d) failed\n", z_cli_qp_size);
        return;
    }

    x_thread_delay(1000);

    i4_ret = x_msg_q_attach(&h_msgq, OS_MSGQ_QP_NAME);

    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("x_msg_q_attach(%s) failed %d\n",
                    "_os_cli_msgq", i4_ret);
    }

    i4_loop = i4_cli_qp_loop;
    do
    {
        *(UINT32 *) pui1_msg = (UINT32) i4_loop;

        i4_ret = x_msg_q_send(h_msgq,
                              pui1_msg,
                              z_cli_qp_size,
                              100);

        if (i4_ret != OSR_OK)
        {
            if (i4_ret == OSR_TOO_MANY)
            {
                x_thread_delay(3000);
            }
            else
            {
                x_dbg_stmt("x_msg_q_send(%s) failed %d\n",
                            "_os_cli_msgq", i4_ret);
            }
        }

        x_thread_delay(500);

        --i4_loop;
    } while (i4_loop != 0);

    x_mem_free(pui1_msg);

    x_msg_q_delete(h_msgq);

    x_thread_exit();
}

/*-----------------------------------------------------------------------------
 * Name: os_cli_msgq_health_pause
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32 os_cli_msgq_health_pause (INT32         i4_argc,
                                       const CHAR**  pps_argv)
{
    msg_q_health_check(FALSE);
    
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: os_cli_msgq_health_resume
 *
 * Description:
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32 os_cli_msgq_health_resume (INT32         i4_argc,
                                        const CHAR**  pps_argv)
{
    msg_q_health_check(TRUE);
    
    return CLIR_OK;
}
#endif
#endif  /* CLI_LVL_ALL */
